// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Slick-development Team, Marcel Schumann $
// ----------------------------------------------------

#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/SCORING/COMPONENTS/CHPI.h>
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
		CHPI::AromaticRing::AromaticRing()
			:	ring_atoms_(),
				centre_(0.0f),
				normal_vector_(0.0f)
		{
		}

		CHPI::AromaticRing::AromaticRing
			(const AromaticRing& aromatic_ring)
			:	ring_atoms_(aromatic_ring.ring_atoms_),
				centre_(aromatic_ring.centre_),
				normal_vector_(aromatic_ring.normal_vector_)
		{
		}

		CHPI::AromaticRing::AromaticRing
			(const std::vector<Atom*>& atoms)
			:	ring_atoms_(),
				centre_(0.0f),
				normal_vector_(0.0f)
		{
			setRing(atoms);
		}


		void CHPI::AromaticRing::computeCentre_()
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


		void CHPI::AromaticRing::computeNormalVector_()
		{
			Vector3 v13 = ring_atoms_[2]->getPosition() - ring_atoms_[0]->getPosition();
			Vector3 v15 = ring_atoms_[4]->getPosition() - ring_atoms_[0]->getPosition();
			normal_vector_ = v13 % v15;
			normal_vector_.normalize();
		}


		const Vector3& CHPI::AromaticRing::getCentre() const
		{
			return(centre_);
		}


		const Vector3& CHPI::AromaticRing::getNormalVector() const
		{
			return(normal_vector_);
		}


		void CHPI::AromaticRing::update()
		{
			computeCentre_();
			computeNormalVector_();
		}


		void CHPI::AromaticRing::dump(ostream& s) const
		{
			vector<Atom*>::const_iterator it = ring_atoms_.begin();
			for (; it != ring_atoms_.end(); ++it)
			{
				s << (*it)->getFullName() << endl;
			}
			s << "ring centre: " << centre_ << std::endl;
			s << "ring normal: " << normal_vector_ << std::endl;
			s << std::endl;
		}


		void CHPI::AromaticRing::setRing
			(const std::vector<Atom*>& atoms)
		{
			ring_atoms_ = atoms;
			computeCentre_();
			computeNormalVector_();
		}


		const std::vector<Atom*>& CHPI::AromaticRing::getRing() const
		{
			return(ring_atoms_);
		}


		CHPI::CHGroup::~CHGroup()
		{
		}


		CHPI::CHGroup::CHGroup(const Atom* C_Atom, const Atom* H_Atom)
		{
			setAtoms(C_Atom, H_Atom);
		}


		CHPI::CHGroup::CHGroup(const CHGroup& CH_groups)
			:	H_atom_(CH_groups.H_atom_),
				C_atom_(CH_groups.C_atom_)
		{
		}


		void CHPI::CHGroup::dump(std::ostream& s) const
		{
			s << "C: " << C_atom_ << endl;
			s << "H: " << H_atom_ << endl;
		}


		void CHPI::CHGroup::setAtoms(const Atom* C_Atom, const Atom* H_Atom)
		{
			C_atom_ = C_Atom;
			H_atom_ = H_Atom;
			return;
		}


		const String CHPI::Option::VERBOSITY
			 = "verbosity";
		const String CHPI::Option::CX_DISTANCE_UPPER
			 = "CX_distance_upper";
		const String CHPI::Option::CHX_ANGLE_LOWER
			 = "CHX_angle_lower";
		const String CHPI::Option::HX_PROJECTED_DISTANCE_LOWER
			 = "HX_projected_distance_lower";
		const String CHPI::Option::HX_PROJECTED_DISTANCE_UPPER
			 = "HX_projected_distance_upper";
		const String CHPI::Option::DISTANCE_TOLERANCE
			 = "distance_tolerance";
		const String CHPI::Option::ANGLE_TOLERANCE
			 = "angle_tolerance";
		const String CHPI::Option::LIMIT
			 = "CHPI_sigmoid_limit";
		const String CHPI::Option::CREATE_INTERACTION_FILE
			 = "CHPI_create_interaction_file";

		const Size CHPI::Default::VERBOSITY = 0;
		const float CHPI::Default::CX_DISTANCE_UPPER = 4.5f;
		const float CHPI::Default::CHX_ANGLE_LOWER = 110.0f;
		const float CHPI::Default::HX_PROJECTED_DISTANCE_LOWER = 0.7f;
		const float CHPI::Default::HX_PROJECTED_DISTANCE_UPPER = 1.7f;
		const float CHPI::Default::DISTANCE_TOLERANCE = 0.25f;
		const float CHPI::Default::ANGLE_TOLERANCE = 25.0f;
		const float CHPI::Default::LIMIT = 0.01f;
		const bool CHPI::Default::CREATE_INTERACTION_FILE = true;



		// Default constructor
		CHPI::CHPI ()
			:	ScoringComponent(),
				possible_interactions_(),
				ligand_CH_groups_(),
				receptor_aromatic_rings_(),
				CX_distance_upper_(0.0f),
				CHX_angle_lower_(0.0f),
				HX_projected_distance_lower_(0.0f),
				HX_projected_distance_upper_(0.0f)
		{
			setName("CHPI");
			gridable_ = 0;
			atom_pairwise_ = 0;
		}


		// Detailed constructor
		CHPI::CHPI (ScoringFunction& sf)
			: ScoringComponent(sf),
				possible_interactions_(),
				ligand_CH_groups_(),
				receptor_aromatic_rings_(),
				CX_distance_upper_(0.0f),
				CHX_angle_lower_(0.0f),
				HX_projected_distance_lower_(0.0f),
				HX_projected_distance_upper_(0.0f)
		{
			setName("CHPI");
			gridable_ = 0;
			atom_pairwise_ = 0;
		}


		// Copy constructor
		CHPI::CHPI (const CHPI& frs )
			: ScoringComponent(frs),
				possible_interactions_(frs.possible_interactions_),
				ligand_CH_groups_(frs.ligand_CH_groups_),
				receptor_aromatic_rings_(frs.receptor_aromatic_rings_),
				CX_distance_upper_(frs.CX_distance_upper_),
				CHX_angle_lower_(frs.CHX_angle_lower_),
				HX_projected_distance_lower_(frs.HX_projected_distance_lower_),
				HX_projected_distance_upper_(frs.HX_projected_distance_upper_)
		{
			setName("CHPI");
			gridable_ = 0;
			atom_pairwise_ = 0;
		}


		// Destructor
		CHPI::~CHPI()
		{
			clear();
		}


		void CHPI::clear()
		{
			possible_interactions_.clear();

			for (Size i = 0; i < ligand_CH_groups_.size(); i++)
			{
				delete ligand_CH_groups_[i];
			}
			for (Size i = 0; i < receptor_aromatic_rings_.size(); i++)
			{
				delete receptor_aromatic_rings_[i];
			}
			for (Size i = 0; i < receptor_CH_groups_.size(); i++)
			{
				delete receptor_CH_groups_[i];
			}
			for (Size i = 0; i < ligand_aromatic_rings_.size(); i++)
			{
				delete ligand_aromatic_rings_[i];
			}

			receptor_aromatic_rings_.clear();
			ligand_CH_groups_.clear();
		}


		// Set up atomic properties for the calculation of the scoring
		// contribution
		bool CHPI::setup()
		{
			Timer timer;
			timer.start();

			ScoringFunction* scoring_function = getScoringFunction();
			if (scoring_function == 0)
			{
				Log.error() << "CHPI::setup(): "
					<< "component not bound to scoring function." << endl;
				return false;
			}

			// Clear all data structures
			clear();

			// Set all paramters
			options = getScoringFunction()->getOptions();
			options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
			options.setDefaultReal(Option::CX_DISTANCE_UPPER, Default::CX_DISTANCE_UPPER);
			options.setDefaultReal(Option::CHX_ANGLE_LOWER, Default::CHX_ANGLE_LOWER);
			options.setDefaultReal(Option::HX_PROJECTED_DISTANCE_LOWER,
					Default::HX_PROJECTED_DISTANCE_LOWER);
			options.setDefaultReal(Option::HX_PROJECTED_DISTANCE_UPPER,
					Default::HX_PROJECTED_DISTANCE_UPPER);
			options.setDefaultReal(Option::DISTANCE_TOLERANCE,
					Default::DISTANCE_TOLERANCE);
			options.setDefaultReal(Option::ANGLE_TOLERANCE,
					Default::ANGLE_TOLERANCE);
			options.setDefaultReal(Option::LIMIT,
					Default::LIMIT);
			options.setDefaultBool(Option::CREATE_INTERACTION_FILE,
					Default::CREATE_INTERACTION_FILE);

			CX_distance_upper_ = options.getReal(Option::CX_DISTANCE_UPPER);
			CHX_angle_lower_ = options.getReal(Option::CHX_ANGLE_LOWER);
			HX_projected_distance_lower_ = options.getReal(Option::HX_PROJECTED_DISTANCE_LOWER);
			HX_projected_distance_upper_ = options.getReal(Option::HX_PROJECTED_DISTANCE_UPPER);

			// The distance tolerance for creating smooth scores in interaction
			// estimation (in units of Angstrom). This is just half of the
			// tolerance, so double this value in order to get the full tolerance
			// width.
			distance_tolerance_ = options.getReal(Option::DISTANCE_TOLERANCE);
			// The angular tolerance in units of degrees
			angle_tolerance_ = options.getReal(Option::ANGLE_TOLERANCE);
			// The cutoff limit for stacking scoring terms
			limit_ = options.getReal(Option::LIMIT);

			// Will we write a HIN file containing the interactions?
			write_interactions_file_ = options.getBool(Option::CREATE_INTERACTION_FILE);

			verbosity_ = options.getInteger(Option::VERBOSITY);

			// All ring-CH-pairs with a distance larger than this value would result in a score of zero, so simple ignore those pairs!
			distance_cutoff_ = CX_distance_upper_+distance_tolerance_;

			/// Find aromatic rings in receptor
			AtomContainer* receptor = scoring_function_->getReceptor();

			// First, find the rings of the receptor (via SSSR)
			vector<vector<Atom*> > SSSR_r;
			rp_.calculateSSSR(SSSR_r, *receptor);

			// Now look for aromatic rings
			receptor_aromatic_rings_.clear();
			ap_.aromatize(SSSR_r, *receptor);
			vector<vector<Atom*> >::iterator SSSR_it;
			for (SSSR_it = SSSR_r.begin(); SSSR_it != SSSR_r.end(); ++SSSR_it)
			{
				vector<Atom*>::iterator ring_atom_it = SSSR_it->begin();
				if ((*ring_atom_it)->hasProperty("IsAromatic"))
				{
					CHPI::AromaticRing* current_ring = new CHPI::AromaticRing(*SSSR_it);
					receptor_aromatic_rings_.push_back(current_ring);
				}
			}

			/// Find CH-groups within the receptor
			receptor_CH_groups_.clear();
			AtomConstIterator rec_it(receptor->beginAtom());
			Vector3 center = scoring_function_->getLigandCenter();
			for (; +rec_it; ++rec_it)
			{
				if (rec_it->getElement() == PTE[Element::C] && rec_it->countBonds() == 4)
				{
					// The aliphatic C-Atom of this putative interaction
					const Atom* aliphatic_C = &*rec_it;

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
							if (aliphatic_C->getPosition().getDistance(center) < 12)
							{
								CHGroup* chgroup = new CHGroup(aliphatic_C, partner);
								receptor_CH_groups_.push_back(chgroup);
							}
						}
					}
				}
			}

			cout << "CHPI::setup() found " << receptor_aromatic_rings_.size() << " aromatic rings within the receptor" << endl;
			cout << "CHPI::setup() found " << receptor_CH_groups_.size() << " CH-groups within the receptor" << endl;

			setupLigand();

			// remember the setup time in order to known later whether the receptor-structure was modified (translation of entire system or rotation of side-chains)
			update_time_stamp_.stamp();

			timer.stop();

			if (verbosity_ > 9)
			{
				Log.info() << "CHPI::setup(): "
					<< timer.getCPUTime() << " s" << std::endl;
			}

			return(true);
		}


		void CHPI::setupLigand()
		{
			// delete CH-groups and interactions of previous ligand
			possible_interactions_.clear();
			for (Size i = 0; i < ligand_CH_groups_.size(); i++)
			{
				delete ligand_CH_groups_[i];
			}
			for (Size i = 0; i < ligand_aromatic_rings_.size(); i++)
			{
				delete ligand_aromatic_rings_[i];
			}
			ligand_CH_groups_.clear();
			ligand_aromatic_rings_.clear();

			AtomContainer* ligand = scoring_function_->getLigand();

			/// Find CH-groups within the ligand
			// The following piece of code only works for simple sugars, i. e.
			// those without aromatic side chains and only aliphatic carbons.
			AtomConstIterator lig_it(ligand->beginAtom());
			for (; +lig_it; ++lig_it)
			{
				if (lig_it->getElement() == PTE[Element::C] && lig_it->countBonds() == 4)
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
							CHGroup* chgroup = new CHGroup(aliphatic_C, partner);
							ligand_CH_groups_.push_back(chgroup);
						}
					}
				}
			}

			/// Find aromatic rings within the ligand
			vector<vector<Atom*> > SSSR_r;
			rp_.calculateSSSR(SSSR_r, *ligand);

			// Now look for aromatic rings
			ligand_aromatic_rings_.clear();
			ap_.aromatize(SSSR_r, *ligand);
			vector<vector<Atom*> >::iterator SSSR_it;
			for (SSSR_it = SSSR_r.begin(); SSSR_it != SSSR_r.end(); ++SSSR_it)
			{
				vector<Atom*>::iterator ring_atom_it = SSSR_it->begin();
				if ((*ring_atom_it)->hasProperty("IsAromatic"))
				{
					CHPI::AromaticRing* current_ring = new CHPI::AromaticRing(*SSSR_it);
					ligand_aromatic_rings_.push_back(current_ring);
				}
			}

			cout<<"CHPI::setupLigand() found "<<ligand_CH_groups_.size()<<" CH-groups within ligand"<<endl;
			cout<<"CHPI::setupLigand() found "<<ligand_aromatic_rings_.size()<<" aromatic rings within ligand"<<endl;

			calculatePossibleInteractions();
		}


		void CHPI::update(const vector<std::pair<Atom*, Atom*> >&)
		{
			// ignore 'atom_pairs', since this component does not calculate a pair-wise score

			/// Recalculate ring-centers and normal-vectors
			if (update_time_stamp_.isOlderThan(scoring_function_->getReceptor()->getModificationTime()))
			{
				cout<<"Receptor has been translated or rotated; updating the aromatic ring centers and normal-vectors..."<<endl;
				for (vector < CHPI::AromaticRing* > ::const_iterator r_it = receptor_aromatic_rings_.begin(); r_it != receptor_aromatic_rings_.end(); ++r_it)
				{
					(*r_it)->update();
				}
				update_time_stamp_.stamp();
			}
			for (vector < CHPI::AromaticRing* > ::const_iterator l_it = ligand_aromatic_rings_.begin(); l_it != ligand_aromatic_rings_.end(); ++l_it)
			{
				(*l_it)->update();
			}

			calculatePossibleInteractions();
		}


		void CHPI::calculatePossibleInteractions()
		{
			/// Build pairs of aromatic rings and CH-groups
			possible_interactions_.clear();

			// pairs of receptor-aromatic-ring and ligand-CH-group
			for (vector < CHPI::AromaticRing* > ::const_iterator ring_it = receptor_aromatic_rings_.begin(); ring_it != receptor_aromatic_rings_.end(); ++ring_it)
			{
				for (vector < CHGroup* > ::const_iterator ch_it = ligand_CH_groups_.begin(); ch_it != ligand_CH_groups_.end(); ++ch_it)
				{
					if ((*ring_it)->getCentre().getDistance((*ch_it)->getCAtom()->getPosition()) < distance_cutoff_)
					{
						possible_interactions_.push_back(make_pair(*ring_it, *ch_it));
					}
				}
			}

			// pairs of ligand-aromatic-ring and receptor-CH-group
			for (vector < CHPI::AromaticRing* > ::const_iterator ring_it = ligand_aromatic_rings_.begin(); ring_it != ligand_aromatic_rings_.end(); ++ring_it)
			{
				for (vector < CHGroup* > ::const_iterator ch_it = receptor_CH_groups_.begin(); ch_it != receptor_CH_groups_.end(); ++ch_it)
				{
					if ((*ring_it)->getCentre().getDistance((*ch_it)->getCAtom()->getPosition()) < distance_cutoff_)
					{
						possible_interactions_.push_back(make_pair(*ring_it, *ch_it));
					}
				}
			}
		}


		double CHPI::updateScore()
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
			vector< pair<const AromaticRing*, const CHGroup*> >::const_iterator inter_it;
			for (inter_it = possible_interactions_.begin(); inter_it != possible_interactions_.end(); ++inter_it)
			{
				const Vector3& ring_centre = inter_it->first->getCentre();
				const Vector3& C_atom = inter_it->second->getCAtom()->getPosition();

				// calculate the distance ring center <--> C atom
				distance = (ring_centre - C_atom).getLength();

				// compute a score for that interaction
				CX_score = getScoringFunction()->getBaseFunction()->calculate(distance,
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

					CHX_score = getScoringFunction()->getBaseFunction()->calculate(angle_CHX,
								CHX_angle_lower_ + angle_tolerance_,
								CHX_angle_lower_ - angle_tolerance_);

					// if (angle_CHX >= CHX_angle_lower_)
					if (CHX_score > limit_)
					{
						const Vector3& normal = inter_it->first->getNormalVector();
						// Check the projected distance
						float projected_distance_XH = (ring_centre + (-HX * normal) * normal - H_atom).getLength();

						// Calculate a score for the H---X distance. Note that the upper
						// and lower limits in the first base_function() have to be
						// chosen so that lower > upper in order to invert the base
						// function. The whole term has to provide something similar to a
						// Gauss curve.
						HX_score = getScoringFunction()->getBaseFunction()->calculate(
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
									<< inter_it->second->getCAtom()->getFullName()
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
				Log.info() << "CHPI::updateEnergy(): "
					<< timer.getCPUTime() << " s" << std::endl;
				Log.info() << "CHPI: energy is " << score_ << endl;
			}

			if (write_interactions_file_)
			{
				HINFile interactions_file("CHPI_interactions.hin", std::ios::out);
				interactions_file << interactions_molecule;
				interactions_file.close();
			}

			return score_;
		}
}
