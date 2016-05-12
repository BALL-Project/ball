// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: CHPI.C,v 1.2.10.1 2007/08/07 16:16:16 bertsch Exp $

// CH---pi interaction for the SLICK energy function

#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/SCORING/COMPONENTS/CHPISlick.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/KERNEL/residueIterator.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/protein.h>

#include <BALL/SYSTEM/timer.h>

#define DEBUG 1

#ifdef DEBUG
#include <BALL/FORMAT/HINFile.h>
#endif

using namespace std;

namespace BALL
{


	CHPISlick::AromaticRing::AromaticRing()

		:	ring_atoms_(),
			centre_(0.0f),
			normal_vector_(0.0f)
	{
	}

	CHPISlick::AromaticRing::AromaticRing
		(const AromaticRing& aromatic_ring)

		:	ring_atoms_(aromatic_ring.ring_atoms_),
			centre_(aromatic_ring.centre_),
			normal_vector_(aromatic_ring.normal_vector_)
	{
	}

	CHPISlick::AromaticRing::AromaticRing
		(const std::vector<Atom*>& atoms)

		:	ring_atoms_(),
			centre_(0.0f),
			normal_vector_(0.0f)
	{
		setRing(atoms);
	}


	void CHPISlick::AromaticRing::computeCentre_()

	{
		std::vector<Atom*>::const_iterator iter = ring_atoms_.begin();
		Size x = 0;
		for (; iter != ring_atoms_.end(); ++iter)
		{
			centre_ += (*iter)->getPosition();
			++x;
		}
		centre_ = centre_ / x;
	}


	void CHPISlick::AromaticRing::computeNormalVector_()

	{
		Vector3 v13 = ring_atoms_[2]->getPosition() - ring_atoms_[0]->getPosition();
		Vector3 v15 = ring_atoms_[4]->getPosition() - ring_atoms_[0]->getPosition();
		normal_vector_ = v13 % v15;
		normal_vector_.normalize();
	}


	const Vector3& CHPISlick::AromaticRing::getCentre() const

	{
		return(centre_);
	}


	const Vector3& CHPISlick::AromaticRing::getNormalVector() const

	{
		return(normal_vector_);
	}


	void CHPISlick::AromaticRing::dump(ostream& s) const

	{
		std::vector<Atom*>::const_iterator it = ring_atoms_.begin();
		for (; it != ring_atoms_.end(); ++it)
		{
			s << (*it)->getFullName() << endl;
		}
		s << "ring centre: " << centre_ << std::endl;
		s << "ring normal: " << normal_vector_ << std::endl;
		s << std::endl;
	}


	void CHPISlick::AromaticRing::setRing
		(const std::vector<Atom*>& atoms)

	{
		ring_atoms_ = atoms;
		computeCentre_();
		computeNormalVector_();
	}


	const std::vector<Atom*>& CHPISlick::AromaticRing::getRing() const

	{
		return(ring_atoms_);
	}


	CHPISlick::CHGroup::~CHGroup()

	{
	}


	CHPISlick::CHGroup::CHGroup(const Atom* C_Atom, const Atom* H_Atom)

	{
		setAtoms(C_Atom, H_Atom);
	}


	CHPISlick::CHGroup::CHGroup(const CHGroup& CH_groups)

		:	H_atom_(CH_groups.H_atom_),
			C_atom_(CH_groups.C_atom_)
	{
	}


	void CHPISlick::CHGroup::dump(std::ostream& s) const

	{
		s << "C: " << C_atom_ << endl;
		s << "H: " << H_atom_ << endl;
	}


	void CHPISlick::CHGroup::setAtoms(const Atom* C_Atom, const Atom* H_Atom)

	{
		C_atom_ = C_Atom;
		H_atom_ = H_Atom;
		return;
	}


	const String CHPISlick::Option::VERBOSITY
		= "verbosity";
	const String CHPISlick::Option::CX_DISTANCE_UPPER
		= "CX_distance_upper";
	const String CHPISlick::Option::CHX_ANGLE_LOWER
		= "CHX_angle_lower";
	const String CHPISlick::Option::HX_PROJECTED_DISTANCE_LOWER
		= "HX_projected_distance_lower";
	const String CHPISlick::Option::HX_PROJECTED_DISTANCE_UPPER
		= "HX_projected_distance_upper";
	const String CHPISlick::Option::DISTANCE_TOLERANCE
		= "distance_tolerance";
	const String CHPISlick::Option::ANGLE_TOLERANCE
		= "angle_tolerance";
	const String CHPISlick::Option::LIMIT
		= "CHPI_sigmoid_limit";
	const String CHPISlick::Option::CREATE_INTERACTION_FILE
		= "CHPI_create_interaction_file";

	const Size CHPISlick::Default::VERBOSITY = 0;
	const float CHPISlick::Default::CX_DISTANCE_UPPER = 4.5f;
	const float CHPISlick::Default::CHX_ANGLE_LOWER = 110.0f;
	const float CHPISlick::Default::HX_PROJECTED_DISTANCE_LOWER = 0.7f;
	const float CHPISlick::Default::HX_PROJECTED_DISTANCE_UPPER = 1.7f;
	const float CHPISlick::Default::DISTANCE_TOLERANCE = 0.25f;
	const float CHPISlick::Default::ANGLE_TOLERANCE = 25.0f;
	const float CHPISlick::Default::LIMIT = 0.01f;
	const bool CHPISlick::Default::CREATE_INTERACTION_FILE = true;



	// Default constructor
	CHPISlick::CHPISlick ()

		:	ScoringComponent(),
			possible_interactions_(),
			all_CH_groups_(),
			all_aromatic_rings_(),
			CX_distance_upper_(0.0f),
			CHX_angle_lower_(0.0f),
			HX_projected_distance_lower_(0.0f),
			HX_projected_distance_upper_(0.0f)
	{
		setName("CHPISlick");
	}


	// Detailed constructor
	CHPISlick::CHPISlick (ScoringFunction& sf)

		: ScoringComponent(sf),
			possible_interactions_(),
			all_CH_groups_(),
			all_aromatic_rings_(),
			CX_distance_upper_(0.0f),
			CHX_angle_lower_(0.0f),
			HX_projected_distance_lower_(0.0f),
			HX_projected_distance_upper_(0.0f)
	{
		setName("CHPISlick");
	}


	// Copy constructor
	CHPISlick::CHPISlick (const CHPISlick& frs )

		: ScoringComponent(frs),
			possible_interactions_(frs.possible_interactions_),
			all_CH_groups_(frs.all_CH_groups_),
			all_aromatic_rings_(frs.all_aromatic_rings_),
			CX_distance_upper_(frs.CX_distance_upper_),
			CHX_angle_lower_(frs.CHX_angle_lower_),
			HX_projected_distance_lower_(frs.HX_projected_distance_lower_),
			HX_projected_distance_upper_(frs.HX_projected_distance_upper_)
	{
	}


	// Destructor
	CHPISlick::~CHPISlick()

	{
		clear();
	}


	// Clearing function
	void CHPISlick::clear()

	{
		possible_interactions_.clear();
		all_CH_groups_.clear();
		all_aromatic_rings_.clear();
	}


	// Set up atomic properties for the calculation of the scoring
	// contribution
	bool CHPISlick::setup()

	{

		Timer timer;
		timer.start();

		ScoringFunction* scoring_function = getScoringFunction();
		if (scoring_function == 0)
		{
			Log.error() << "CHPISlick::setup(): "
				<< "component not bound to scoring function." << endl;
			return false;
		}

		// Clear all data structures
		clear();

		// Set all paramters
		Options* options = getScoringFunction()->getOptionsToModify();

		options->setDefaultInteger(Option::VERBOSITY,
				Default::VERBOSITY);
		options->setDefaultReal(Option::CX_DISTANCE_UPPER,
				Default::CX_DISTANCE_UPPER);
		options->setDefaultReal(Option::CHX_ANGLE_LOWER,
				Default::CHX_ANGLE_LOWER);
		options->setDefaultReal(Option::HX_PROJECTED_DISTANCE_LOWER,
				Default::HX_PROJECTED_DISTANCE_LOWER);
		options->setDefaultReal(Option::HX_PROJECTED_DISTANCE_UPPER,
				Default::HX_PROJECTED_DISTANCE_UPPER);
		options->setDefaultReal(Option::DISTANCE_TOLERANCE,
				Default::DISTANCE_TOLERANCE);
		options->setDefaultReal(Option::ANGLE_TOLERANCE,
				Default::ANGLE_TOLERANCE);
		options->setDefaultReal(Option::LIMIT,
				Default::LIMIT);
		options->setDefaultBool(Option::CREATE_INTERACTION_FILE,
				Default::CREATE_INTERACTION_FILE);

		CX_distance_upper_
			= options->getReal(Option::CX_DISTANCE_UPPER);
		CHX_angle_lower_
			= options->getReal(Option::CHX_ANGLE_LOWER);
		HX_projected_distance_lower_
			= options->getReal(Option::HX_PROJECTED_DISTANCE_LOWER);
		HX_projected_distance_upper_
			= options->getReal(Option::HX_PROJECTED_DISTANCE_UPPER);

		// The distance tolerance for creating smooth scores in interaction
		// estimation (in units of Angstrom). This is just half of the
		// tolerance, so double this value in order to get the full tolerance
		// width.
		distance_tolerance_ = options->getReal(Option::DISTANCE_TOLERANCE);
		// The angular tolerance in units of degrees
		angle_tolerance_ = options->getReal(Option::ANGLE_TOLERANCE);
		// The cutoff limit for stacking scoring terms
		limit_ = options->getReal(Option::LIMIT);

		// Will we write a HIN file containing the interactions?
		write_interactions_file_
			= options->getBool(Option::CREATE_INTERACTION_FILE);
		// How loud will we cry?
		verbosity_ = options->getInteger(Option::VERBOSITY);

		// Get the receptor from the scoring function
		const Protein* protein = (const Protein*)(scoring_function)->getReceptor();

		std::vector<Atom*> current_atoms;
		std::vector<Atom*> current_atoms2;
		AromaticRing* current_ring = 0;

		// Iterate over all residues and find aromatic ones
		ResidueConstIterator res_it = protein->beginResidue();
		for (; +res_it; ++res_it)
		{
			string residue_name = res_it->getFullName();
			current_atoms.clear();
			current_atoms2.clear();

			// Find aromatic rings by their names: Trp, Phe, Tyr, His
			if ((residue_name == "TRP")
					|| (residue_name == "TRP-C")
					|| (residue_name == "TRP-N")
					|| (residue_name == "TRP-M"))
			{
				AtomConstIterator at_it = res_it->beginAtom();
				for (; +at_it; ++at_it)
				{
					string AtomName = at_it->getName();

					// Find aromatic atoms by their name. Note: This only works with
					// standard PDB files.
					if (AtomName == "CE2" || AtomName == "CD2"|| AtomName == "CD1"
							|| AtomName == "CG" || AtomName == "NE1")
					{
						current_atoms.push_back(const_cast<Atom*>(&*at_it));
					}
					if (AtomName =="CE2" || AtomName ==" CD2" || AtomName == "CE3"
							|| AtomName == "CZ2"|| AtomName == "CZ3"|| AtomName == "CH2")
					{
						current_atoms2.push_back(const_cast<Atom*>(&*at_it));
					}
				}
				current_ring = new AromaticRing(current_atoms);
				all_aromatic_rings_.push_back(*current_ring);
				current_ring = new AromaticRing(current_atoms2);
				all_aromatic_rings_.push_back(*current_ring);
			}
			else
			{
				// Now Tyrosine and Phenylalanine
				if ((residue_name == "TYR")
						|| (residue_name == "TYR-C")
						|| (residue_name == "TYR-N")
						|| (residue_name == "TYR-M")
						|| (residue_name == "PHE")
						|| (residue_name == "PHE-C")
						|| (residue_name == "PHE-N")
						|| (residue_name == "PHE-M"))
				{
					AtomConstIterator at_it  = res_it->beginAtom();
					for (; +at_it; ++at_it)
					{
						string AtomName;
						AtomName= at_it->getName();

						if (AtomName == "CG" || AtomName == "CD1" || AtomName == "CD2"
								|| AtomName == "CE1" || AtomName == "CE2" || AtomName == "CZ")
						{
							current_atoms.push_back(const_cast<Atom*>(&*at_it));
						}
					}
					current_ring = new AromaticRing(current_atoms);
					all_aromatic_rings_.push_back(*current_ring);
				}
				else
				{
					// Histidine
					if ((residue_name == "HIS")
							|| (residue_name == "HIS-C")
							|| (residue_name == "HIS-N")
							|| (residue_name == "HIS-M"))
					{
						AtomConstIterator at_it  = res_it->beginAtom();
						for (; +at_it; ++at_it)
						{
							string AtomName;
							AtomName= at_it->getName();

							if (AtomName == "CG" || AtomName == "ND1" || AtomName == "CD2"
									|| AtomName == "CE1" || AtomName == "NE2")
							{
								current_atoms.push_back(const_cast<Atom*>(&*at_it));
							}
						}
						current_ring = new AromaticRing(current_atoms);
						all_aromatic_rings_.push_back(*current_ring);
					}
				}
			}
		}

		// The following piece of code only works for simple sugars, i. e.
		// those wihtout aromatic side chains and only aliphatic carbons.

		const AtomContainer* ligand = scoring_function->getLigand();

		AtomConstIterator lig_it = ligand->beginAtom();
		for (; +lig_it; ++lig_it)
		{
			if ((lig_it->getElement() == PTE[Element::C])
				&& (lig_it->countBonds() == 4))
			{
				// The aliphatic C-Atom of this putative interaction
				const Atom* aliphatic_C = &*lig_it;

				// Now iterate over all bonds and add every bound hydrogen to the
				// list of CH groups
				AtomBondConstIterator bnd_it = aliphatic_C->beginBond();
				for (; +bnd_it; ++bnd_it)
				{
					// Get the bond partner of the aliphatic carbon.
					const Atom* partner = bnd_it->getPartner(*aliphatic_C);

					// If it is a hydrogen, everything's fine
					if (partner->getElement() == PTE[Element::H])
					{
						all_CH_groups_.push_back(CHGroup(aliphatic_C, partner));
					}
				}
			}
		}

		// Build the list of all possible interactions
		vector<CHGroup>::const_iterator CH_iter = all_CH_groups_.begin();
		vector<AromaticRing>::const_iterator arom_iter;
		for (; CH_iter != all_CH_groups_.end(); ++CH_iter)
		{
			for (arom_iter = all_aromatic_rings_.begin();
					arom_iter != all_aromatic_rings_.end(); ++arom_iter)
			{
				possible_interactions_.push_back(pair<const AromaticRing*,
						const CHGroup*>(&*arom_iter, &*CH_iter));
			}
		}

		timer.stop();

		if (verbosity_ > 9)
		{
			Log.info() << "CHPISlick::setup(): "
				<< timer.getCPUTime() << " s" << std::endl;
		}

		return(true);
	}


	void CHPISlick::update(const vector<std::pair<Atom*, Atom*> >&)
	{
	}


	double CHPISlick::updateScore()

	{

		Timer timer;
		timer.start();

		// A pseudomolecule for every CHPI interaction is saved in this
		// molecule and written to disk, if the user wants it
		Molecule interactions_molecule;

		// Reset the energy value.
		score_ = 0.0f;

		// Two temporary vectors we need for the angle calculation
		Vector3 HC;
		Vector3 HX;

		// The different scores
		float CX_score;
		float CHX_score;
		float HX_score;

		// Some temporary variables
		float distance;

		// Iterate over all possible interactions
		vector< pair<const AromaticRing*, const CHGroup*> >::const_iterator
			inter_it;

		for (inter_it = possible_interactions_.begin();
				inter_it != possible_interactions_.end(); ++inter_it)
		{
			const Vector3& ring_centre = inter_it->first->getCentre();
			const Vector3& C_atom = inter_it->second->getCAtom()->getPosition();

			// calculate the distance ring center <--> C atom
			distance = (ring_centre - C_atom).getLength();

			// compute a score for that interaction

			CX_score
				= getScoringFunction()->getBaseFunction()->calculate(distance,
						CX_distance_upper_ - distance_tolerance_,
						CX_distance_upper_ + distance_tolerance_);

			if (CX_score > limit_)
			{

				// Check angle (C, H, X)
				const Vector3& H_atom = inter_it->second->getHAtom()->getPosition();
				const Vector3& C_atom = inter_it->second->getCAtom()->getPosition();
				// We need two vectors for defining the angle
				HC = C_atom - H_atom;
				HX = ring_centre - H_atom;
				float angle_CHX = HC.getAngle(HX).toDegree();

				// Calculate the angle score. Note that lower tolerance has to be
				// greater than the upper tolerance because we have to invert the
				// function
				CHX_score
					= getScoringFunction()->getBaseFunction()->calculate(angle_CHX,
							CHX_angle_lower_ + angle_tolerance_,
							CHX_angle_lower_ - angle_tolerance_);

				// if (angle_CHX >= CHX_angle_lower_)
				if (CHX_score > limit_)
				{

					const Vector3& normal = inter_it->first->getNormalVector();
					// Check the projected distance
					float projected_distance_XH
						= (ring_centre + (-HX * normal) * normal - H_atom).getLength();


					// Calculate a score for the H---X distance. Note that the upper
					// and lower limits in the first base_function() have to be
					// chosen so that lower > upper in order to invert the base
					// function. The whole term has to provide something similar to a
					// Gauss curve.
					HX_score
						= getScoringFunction()->getBaseFunction()->calculate(
								projected_distance_XH,
								HX_projected_distance_lower_ + distance_tolerance_,
								HX_projected_distance_lower_ - distance_tolerance_)
						* getScoringFunction()->getBaseFunction()->calculate(
								projected_distance_XH,
								HX_projected_distance_upper_ - distance_tolerance_,
								HX_projected_distance_upper_ + distance_tolerance_);

					if (HX_score > limit_)
					{
						float e = 1.0f / 3.0f * (CX_score + CHX_score + HX_score);
						// Found an interaction, count it.

						if (verbosity_ > 9)
						{
							Log.info() << inter_it->first->getRing()[0]->getResidue()->getFullName()
								<< ":" << inter_it->first->getRing()[0]->getResidue()->getID()
								<< " --- "
								<< inter_it->second->getCAtom()->getResidue()->getFullName()
								<< ":"
								<< inter_it->second->getCAtom()->getResidue()->getID()
								<< std::endl;
							Log.info() << "CX:  " << CX_score << "(" << distance << " A)"
								<< std::endl;
							Log.info() << "CHX: " << CHX_score << "(" << angle_CHX
								<< " deg)" << std::endl;
							Log.info() << "HpX: " << HX_score << "("
								<< projected_distance_XH << " A)" << std::endl << std::endl;
							Log.info() << "Score: " << e << std::endl;
						}

						if (write_interactions_file_)
						{
							Atom* atom_ptr_H = new Atom();
							atom_ptr_H->setElement(PTE[Element::Fe]);
							atom_ptr_H->setName("H");
							atom_ptr_H->setPosition(H_atom);
							atom_ptr_H->setCharge(e);

							Atom* atom_ptr_X = new Atom();
							atom_ptr_X->setElement(PTE[Element::Fe]);
							atom_ptr_X->setName("X");
							atom_ptr_X->setPosition(ring_centre);
							atom_ptr_X->setCharge(0.0f);

							Atom* atom_ptr_N = new Atom();
							atom_ptr_N->setElement(PTE[Element::S]);
							atom_ptr_N->setName("N");
							atom_ptr_N->setPosition(ring_centre + normal);
							atom_ptr_N->setCharge(-1.0f);

							Atom* atom_ptr_L = new Atom();
							atom_ptr_L->setElement(PTE[Element::K]);
							atom_ptr_L->setName("L");
							atom_ptr_L->setPosition(ring_centre + (-HX * normal) * normal);
							atom_ptr_L->setCharge(e);

							atom_ptr_H->createBond(*atom_ptr_L);
							atom_ptr_X->createBond(*atom_ptr_N);
							atom_ptr_X->createBond(*atom_ptr_L);

							interactions_molecule.insert(*atom_ptr_H);
							interactions_molecule.insert(*atom_ptr_X);
							interactions_molecule.insert(*atom_ptr_N);
							interactions_molecule.insert(*atom_ptr_L);
						}

						score_ += e;
					}
				}
			}
		}

		timer.stop();

		if (verbosity_ > 9)
		{
			Log.info() << "CHPISlick::updateEnergy(): "
				<< timer.getCPUTime() << " s" << std::endl;
			Log.info() << "CHPISlick: energy is " << score_ << endl;
		}

		if (write_interactions_file_)
		{
			HINFile interactions_file("CHPISlick_interactions.hin", std::ios::out);
			interactions_file << interactions_molecule;
			interactions_file.close();
		}

		return(score_);
	}
}
