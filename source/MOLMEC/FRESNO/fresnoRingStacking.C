// $Id: fresnoRingStacking.C,v 1.1.2.1 2004/05/13 10:06:37 anker Exp $
// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

// CH---pi interaction for the SLICK energy function

#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/MOLMEC/FRESNO/fresno.h>
#include <BALL/MOLMEC/FRESNO/fresnoRingStacking.h>
#include <BALL/MOLMEC/FRESNO/fresnoDesolvation.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/KERNEL/residueIterator.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/protein.h>

#define DEBUG 1

#ifdef DEBUG
#include <BALL/FORMAT/HINFile.h>
#endif

using namespace std;

namespace BALL
{


	FresnoRingStacking::AromaticRing::AromaticRing()
		throw()
		:	ring_atoms_(),
			centre_(0.0f),
			normal_vector_(0.0f)
	{
	}

	FresnoRingStacking::AromaticRing::AromaticRing
		(const AromaticRing& aromatic_ring)
		throw()
		:	ring_atoms_(aromatic_ring.ring_atoms_),
			centre_(aromatic_ring.centre_),
			normal_vector_(aromatic_ring.normal_vector_)
	{
	}

	FresnoRingStacking::AromaticRing::AromaticRing
		(const std::vector<const Atom*>& atoms)
		throw()
		:	ring_atoms_(),
			centre_(0.0f),
			normal_vector_(0.0f)
	{
		setRing(atoms);
	}

	void FresnoRingStacking::AromaticRing::computeCentre_()
		throw()
	{
		std::vector<const Atom*>::const_iterator iter = ring_atoms_.begin();
		Size x = 0;
		for (; iter != ring_atoms_.end(); ++iter)
		{ 
			centre_ += (*iter)->getPosition();
			++x;
		}
		centre_ = centre_ / x;
	}


	void FresnoRingStacking::AromaticRing::computeNormalVector_()
		throw()
	{
		Vector3 v13 = ring_atoms_[2]->getPosition() - ring_atoms_[0]->getPosition();
		Vector3 v15 = ring_atoms_[4]->getPosition() - ring_atoms_[0]->getPosition();
		normal_vector_ = v13 % v15;
		normal_vector_.normalize();
	}


	const Vector3& FresnoRingStacking::AromaticRing::getCentre() const
		throw()
	{
		return(centre_);
	}


	const Vector3& FresnoRingStacking::AromaticRing::getNormalVector() const
		throw()
	{
		return(normal_vector_);
	}


	void FresnoRingStacking::AromaticRing::dump(ostream& s) const
		throw()
	{
		std::vector<const Atom*>::const_iterator it = ring_atoms_.begin();
		for (; it != ring_atoms_.end(); ++it)
		{
			s << (*it)->getFullName() << endl;
		}
		s << "ring centre: " << centre_ << std::endl;
		s << "ring normal: " << normal_vector_ << std::endl;
		s << std::endl;
	}


	void FresnoRingStacking::AromaticRing::setRing
		(const std::vector<const Atom*>& atoms)
		throw()
	{
		ring_atoms_ = atoms;
		computeCentre_();
		computeNormalVector_();
	}


	const std::vector<const Atom*>& FresnoRingStacking::AromaticRing::getRing() const
		throw()
	{
		return(ring_atoms_);
	}


	FresnoRingStacking::CHGroup::~CHGroup()
		throw()
	{
	}


	FresnoRingStacking::CHGroup::CHGroup(const Atom* C_Atom, const Atom* H_Atom)
		throw()
	{
		setAtoms(C_Atom, H_Atom);
	}


	FresnoRingStacking::CHGroup::CHGroup(const CHGroup& CH_groups)
		throw()
		:	H_atom_(CH_groups.H_atom_),
			C_atom_(CH_groups.C_atom_)
	{
	}


	void FresnoRingStacking::CHGroup::dump(std::ostream s) const
		throw()
	{
		s << "C: " << C_atom_ << endl;
		s << "H: " << H_atom_ << endl;
	}


	void FresnoRingStacking::CHGroup::setAtoms(const Atom* C_Atom, const Atom* H_Atom)
		throw()
	{
		C_atom_ = C_Atom;
		H_atom_ = H_Atom;
		return;
	}


	// Default constructor
	FresnoRingStacking::FresnoRingStacking ()  
		throw ()
		:	ForceFieldComponent(),
			possible_interactions_(),
			all_CH_groups_(),
			all_aromatic_rings_()
	{ 
		setName("Fresno RingStacking");
	}


	// Detailed constructor
	FresnoRingStacking::FresnoRingStacking (ForceField& force_field)  
		throw() 
		: ForceFieldComponent(force_field),
			possible_interactions_(),
			all_CH_groups_(),
			all_aromatic_rings_()
	{ 
		setName("Fresno RingStacking");
	}


	// Copy constructor
	FresnoRingStacking::FresnoRingStacking (const FresnoRingStacking& frs )
		throw()
		: ForceFieldComponent(frs),
			possible_interactions_(frs.possible_interactions_),
			all_CH_groups_(frs.all_CH_groups_),
			all_aromatic_rings_(frs.all_aromatic_rings_)
	{
	}


	// Destructor
	FresnoRingStacking::~FresnoRingStacking()
		throw()
	{
		clear();
	}


	void FresnoRingStacking::dump(std::ostream s) const
		throw()
	{
		s << "aromatic rings in receptor: " << all_aromatic_rings_.size() << endl;
		s << "CH groups in ligand: " << all_CH_groups_.size() << endl;
		s << "possible interactions: " << possible_interactions_.size() << endl;
	} 


	// Clearing function
	void FresnoRingStacking::clear()
		throw()
	{ 
		possible_interactions_.clear();
		all_CH_groups_.clear();
		all_aromatic_rings_.clear();
	}


	// Set up atomic properties for the calculation of the scoring 
	// contribution
	bool FresnoRingStacking::setup()
		throw()
	{

		ForceField* force_field = getForceField();
		if (force_field == 0)
		{
			Log.error() << "FresnoRingStacking::setup(): "
				<< "component not bound to force field." << endl;
			return false;
		}

		// Clear all data structures
		clear();

		// read the protein and the ligand 
		// of which the binding sites have to be found 
		const Protein& protein 
			= (const Protein&)*((FresnoFF*)force_field)->getProtein();

		// [anker] Grober Unfug
		// vector <AromaticRing> all_aromatic_rings_;		

		// Iterate over all residues and find aromatic ones
		ResidueConstIterator res_it = protein.beginResidue();
		for (; +res_it; ++res_it)
		{ 
			string residue_name = res_it->getFullName();  
			std::vector<const Atom*> current_atoms;

			// Find aromatic rings by their names: Trp, Phe, Tyr
			if (residue_name == "TRP") 
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
						current_atoms.push_back(&*at_it);
					}
					if (AtomName =="CE2" || AtomName ==" CD2" || AtomName == "CE3"
							|| AtomName == "CZ2"|| AtomName == "CZ3"|| AtomName == "CH2")
					{
						current_atoms.push_back(&*at_it);
					}
				}
				AromaticRing* current_ring = new AromaticRing(current_atoms);
				all_aromatic_rings_.push_back(*current_ring);
			}
			else
			{
				// Now Tyrosine and Phenylalanine
				if ((residue_name == "TYR") || (residue_name == "PHE")) 
				{	
					AtomConstIterator at_it  = res_it->beginAtom();
					for (; +at_it; ++at_it)
					{
						string AtomName;
						AtomName= at_it->getName();

						if (AtomName == "CG" || AtomName == "CD1" || AtomName == "CD2" 
								|| AtomName == "CE1" || AtomName == "CE2" || AtomName == "CZ") 
						{  
							current_atoms.push_back(&*at_it);
						}
					}
					AromaticRing* current_ring = new AromaticRing(current_atoms);
					all_aromatic_rings_.push_back(*current_ring);
				}
			}
		}

		// The following piece of code only works for simple sugars, i. e.
		// those wihtout aromatic side chains and only aliphatic carbons. 

		const Molecule& ligand = *((FresnoFF*)force_field)->getLigand();
		AtomConstIterator lig_it(ligand.beginAtom());
		for (; +lig_it; ++lig_it) 
		{ 
			if (lig_it->getElement() == PTE[Element::C])
			{	
				const Atom* aliphatic_C = &*lig_it;	//Zeiger auf ein C-Atom		
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
		
		return(true);
	}


	double FresnoRingStacking::updateEnergy()
		throw()
	{
		#ifdef DEBUG
		Molecule debug_molecule;
		#endif

		// Reset the energy value.
		energy_ = 0.0f;

		// Two temporyr vectors we need for the angle calculation
		Vector3 HC;
		Vector3 HX;

		// Iterate over all possible interactions
		vector< pair<const AromaticRing*, const CHGroup*> >::const_iterator 
			inter_it;
		for (inter_it = possible_interactions_.begin(); 
				inter_it != possible_interactions_.end(); ++inter_it)
		{
			const Vector3& ring_centre = inter_it->first->getCentre();
			const Vector3& C_atom = inter_it->second->getCAtom()->getPosition();

			// Check distance C --- X
			// ????? Thes constants schouldn't be constants
			if ((ring_centre - C_atom).getLength() < 4.5)
			{
#ifdef DEBUG
					Log.info() << "distance C --- x: " 
						<< (ring_centre - C_atom).getLength() << endl;
#endif
				// Check angle (C, H, X)
				const Vector3& H_atom = inter_it->second->getHAtom()->getPosition();
				const Vector3& C_atom = inter_it->second->getCAtom()->getPosition();
				// We need two vectors for defining the angle
				HC = C_atom - H_atom;
				HX = ring_centre - H_atom;
				float angle_CHX = HC.getAngle(HX).toDegree();
				// ????? This constant shouldn't be a constant
				if (angle_CHX >= 120)
				{
#ifdef DEBUG
					Log.info() << "angle (C, H, X):  " << angle_CHX << endl;
#endif
					const Vector3& normal = inter_it->first->getNormalVector();
					// Check the projected distance
					float projected_distance_XH 
						= (ring_centre + (-HX * normal) * normal - H_atom).getLength();

#ifdef DEBUG
					Atom* atom_ptr_H = new Atom();
					atom_ptr_H->setElement(PTE[Element::Fe]);
					atom_ptr_H->setName("H");
					atom_ptr_H->setPosition(H_atom);

					Atom* atom_ptr_X = new Atom();
					atom_ptr_X->setElement(PTE[Element::Fe]);
					atom_ptr_X->setName("X");
					atom_ptr_X->setPosition(ring_centre);

					Atom* atom_ptr_N = new Atom();
					atom_ptr_N->setElement(PTE[Element::S]);
					atom_ptr_N->setName("N");
					atom_ptr_N->setPosition(ring_centre + normal);

					Atom* atom_ptr_L = new Atom();
					atom_ptr_L->setElement(PTE[Element::K]);
					atom_ptr_L->setName("L");
					atom_ptr_L->setPosition(ring_centre + (-HX * normal) * normal);

					atom_ptr_H->createBond(*atom_ptr_L);
					atom_ptr_X->createBond(*atom_ptr_N);
					atom_ptr_X->createBond(*atom_ptr_L);

					debug_molecule.insert(*atom_ptr_H);
					debug_molecule.insert(*atom_ptr_X);
					debug_molecule.insert(*atom_ptr_N);
					debug_molecule.insert(*atom_ptr_L);
					Log.info() << "projected distance H --- X: " 
						<< projected_distance_XH << endl;
#endif

					if (projected_distance_XH >= 1.0 && projected_distance_XH <= 1.2)
					{
						// Found an interaction, count it.
#ifdef DEBUG
						Log.info() << "projected distance H --- X: " 
							<< projected_distance_XH << endl;
#endif
						energy_ += 1.0f;
					}
				}
			}
		}
		Log.info() << "CHPI: energy is " << energy_ << endl;
#ifdef DEBUG
		HINFile debug_file("debug.hin", std::ios::out);
		debug_file << debug_molecule;
		debug_file.close();
#endif
		return(energy_);
	}
}
