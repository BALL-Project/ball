// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94NonBonded.C,v 1.1.2.1 2005/03/17 13:48:24 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94NonBonded.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/COMMON/support.h>

using namespace std;

// define a macro for the square function
#define SQR(x) ((x) * (x))

#ifdef BALL_HAS_INLINE_TPL_ARGS
#	define BALL_TPL_ARG_INLINE inline
#else
#	define BALL_TPL_ARG_INLINE
#endif

namespace BALL 
{

	// default constructor
	MMFF94NonBonded::MMFF94NonBonded()
		throw()
		:	ForceFieldComponent(),
			algorithm_type_(MolmecSupport::BRUTE_FORCE)
	{	
		// set component name
		setName("MMFF94 NonBonded");
	}


	// constructor
	MMFF94NonBonded::MMFF94NonBonded(ForceField& force_field)
		throw()
		:	ForceFieldComponent(force_field),
			algorithm_type_(MolmecSupport::BRUTE_FORCE)
	{
		// set component name
		setName("MMFF94 NonBonded");
	}


	// copy constructor
	MMFF94NonBonded::MMFF94NonBonded(const MMFF94NonBonded& component)
		throw()
		:	ForceFieldComponent(component),
			algorithm_type_(component.algorithm_type_)
	{
	}


	// destructor
	MMFF94NonBonded::~MMFF94NonBonded()
		throw()
	{
		clear();
	}

	const MMFF94NonBonded& MMFF94NonBonded::operator = (const MMFF94NonBonded& anb)
		throw()
	{
		// catch self assignment
		if (&anb == this)
		{
			return *this;
		}

		// call the base class operator
		ForceFieldComponent::operator = (anb);

		algorithm_type_ = anb.algorithm_type_;

		return *this;
	}

	void MMFF94NonBonded::clear()
		throw()
	{
		algorithm_type_ = MolmecSupport::BRUTE_FORCE;
	}


	bool MMFF94NonBonded::operator == (const MMFF94NonBonded& anb)
		throw()
	{
		return (this == &anb);
	}

	// This function determines the most efficient way to calculate all
	// non-bonded atom pairs that depends on the number of atoms of the
	// system. The function return value 0 if the number of atoms is so
	// small that the brute force all against all comparison is the most
	// efficient way. Otherwise it returns 1. 
	MolmecSupport::PairListAlgorithmType
	MMFF94NonBonded::determineMethodOfAtomPairGeneration()
		throw()
	{
		if (force_field_->getAtoms().size() > 900) 
		{ 
			return MolmecSupport::HASH_GRID;
		} 
		else
		{
			return MolmecSupport::BRUTE_FORCE;
		}
	}

	void MMFF94NonBonded::update()
		throw(Exception::TooManyErrors)
	{
		if (getForceField() == 0) 
		{
			Log.error() << "MMFF94NonBonded::update(): component not bound to a force field" << endl;
			return;
		}

		// Calculate all non bonded atom pairs
		ForceField::PairVector atom_pair_vector;

		MolmecSupport::calculateNonBondedAtomPairs
			(atom_pair_vector, getForceField()->getAtoms(), 
			 getForceField()->periodic_boundary.getBox(),
			 cut_off_, force_field_->periodic_boundary.isEnabled(), 
			 algorithm_type_);

		if (getForceField()->getSystem()->containsSelection())
		{
			// eliminate all those pairs where none of the two atoms is selected
			Size number_of_selected_pairs = MolmecSupport::sortNonBondedAtomPairsAfterSelection(atom_pair_vector);
			atom_pair_vector.resize(number_of_selected_pairs);
		}

		// Build the vector "non_bonded_" with the atom pairs and parameters
		buildVectorOfNonBondedAtomPairs(atom_pair_vector);
	}

	// setup the internal datastructures for the component
	bool MMFF94NonBonded::setup()
		throw(Exception::TooManyErrors)
	{
		if (getForceField() == 0) 
		{
			Log.error() << "MMFF94NonBonded::setup(): component not bound to a force field" << endl;
			return false;
		}

		// clear vector of non-bonded atom pairs
		clear();
 
		// Set the options for the non-bonded atom pairs
		Options& options = getForceField()->options;

		// when using periodic boundary conditions, all
		// cutoffs must be smaller than the smallest linear extension of
		// the box - we use the minimum image convention!
		if (getForceField()->periodic_boundary.isEnabled())
		{
			SimpleBox3 box = getForceField()->periodic_boundary.getBox();
			float max_cut_off = 0.5 * Maths::min(box.getWidth(), box.getHeight(), box.getDepth());

			if (cut_off_> max_cut_off)
			{
				Log.error() << "MMFF94NonBonded::setup(): "
										<< "electrostatic cutoff may not exceed half" << endl
										<< "the box dimension when using periodic boundary conditions!" << endl;
				return false;
			}
		}
				
		// extract the Lennard-Jones parameters
		MMFF94* MMFF94_force_field = dynamic_cast<MMFF94*>(force_field_);
		bool has_initialized_parameters = false;
		if ((MMFF94_force_field != 0) && (MMFF94_force_field->hasInitializedParameters()))
		{
			has_initialized_parameters = true;
		}
			
		if (!has_initialized_parameters)
		{
			bool result = true; // = van_der_waals_.extractSection(getForceField()->getParameters(), "LennardJones");

			if (!result) 
			{	
				Log.error() << "MMFF94NonBonded::setup(): "
					<< "cannot find section LennardJones in " << getForceField()->getParameters().getFilename() << endl;
				return false;
			}
		}

		// Determine the most efficient way to calculate all non bonded atom pairs
		algorithm_type_ = determineMethodOfAtomPairGeneration();

		// build the nonbonded pairs
		update();

		return true;
	}


	// Build a vector of non-bonded atom pairs with the vdw parameters 
	// The vector starts with 1-4 interactions
	void MMFF94NonBonded::buildVectorOfNonBondedAtomPairs(const vector<pair<Atom*, Atom*> >& atom_vector)
		throw(Exception::TooManyErrors)
	{
		// throw away the old rubbish

		// resize non_bonded_ if necessary
		if (non_bonded_.capacity() < atom_vector.size())
		{
			// reserve the required size plus 20% 
			// to avoid frequent resizing)
			non_bonded_.reserve((Size)((double)atom_vector.size() * 1.2));
		}

		// Iterate over all atom pairs in atom_vector and test if the atoms
		// are part of a torsion
		vector<Position> non_torsions;
		non_torsions.reserve(atom_vector.size());

		Atom*	atom1;
		Atom* atom2;
		Atom::Type	type_atom1;
		Atom::Type  type_atom2;

		// Iterate and search torsions, fill the atom pairs that have a torsion
		// in non_bonded_
		for (Position i = 0; i < (Size)atom_vector.size(); ++i) 
		{
			atom1 = atom_vector[i].first;
			atom2 = atom_vector[i].second;
			if (!atom1->isVicinal(*atom2)) 
			{
				// store the non-torsions for later appending in the non_torsions
				// vector
				non_torsions.push_back(i);
			}
			else
			{
				type_atom1 = atom1->getType();
				type_atom2 = atom2->getType();
//   				tmp.atom1 = &(Atom::getAttributes()[atom1->getIndex()]);
//   				tmp.atom2 = &(Atom::getAttributes()[atom2->getIndex()]);

				/*
				if (!lennard_jones.assignParameters(tmp.values, type_atom1, type_atom2)) 
				{
					// hydrogen bond parameters are assigned later - do nothing!
					if (!hydrogen_bond.hasParameters(type_atom1, type_atom2))
					{
						getForceField()->error() << "MMFF94NonBonded::setup(): "
												<< "cannot find vdw parameters for types "
												<< atom1->getTypeName() << "-" << atom2->getTypeName() 
												<< " (" << atom1->getFullName() << "-" << atom2->getFullName() << ")" << endl;
						tmp.values.A = 0;
						tmp.values.B = 0;

						getForceField()->getUnassignedAtoms().insert(atom1);
						getForceField()->getUnassignedAtoms().insert(atom2);
					}
				}
				*/

//   				non_bonded_.push_back(tmp);
			}
		}

		// Determine and set the number of 1-4 interactions (torsions)

		// Iterate and search non torsions, fill them in the vector non_bonded_
		for (Position i = 0; i < (Size)non_torsions.size(); ++i) 
		{
			atom1 = atom_vector[non_torsions[i]].first;
			atom2 = atom_vector[non_torsions[i]].second;

			type_atom1 = atom1->getType();
			type_atom2 = atom2->getType();
//   			tmp.atom1 = &(Atom::getAttributes()[atom1->getIndex()]);
//   			tmp.atom2 = &(Atom::getAttributes()[atom2->getIndex()]);

			/*
			if (lennard_jones.hasParameters(type_atom1, type_atom2)) 
			{
				lennard_jones.assignParameters(tmp.values, type_atom1, type_atom2);
			} 
			else 
			{
				Log.error() << "MMFF94NonBonded::setup(): "
					<< "cannot find Lennard Jones parameters for types "
					<< getForceField()->getParameters().getAtomTypes().getTypeName(type_atom1) 
					<< "-"
					<< getForceField()->getParameters().getAtomTypes().getTypeName(type_atom2) 
					<< " (" << atom1->getFullName() << "-" << atom2->getFullName() << ")"
					<< endl;
				tmp.atom1 = &(Atom::getAttributes()[atom1->getIndex()]);
				tmp.atom2 = &(Atom::getAttributes()[atom2->getIndex()]);
				tmp.values.A = 0;
				tmp.values.B = 0;
			}
			*/
//   			non_bonded_.push_back(tmp);
		}

	}

	// Compute the non-bonded energy (i.e. electrostatic, vdW, and H-bonds)
	double MMFF94NonBonded::updateEnergy()
		throw()
	{
		return energy_; 
  } // end of MMFF94NonBonded::updateEnergy 
	
	// This method MMFF94calculates the current forces resulting from
	// van-der-Waals and electrostatic interactions 
	void MMFF94NonBonded::updateForces()
		throw()
	{
		if (getForceField() == 0)
		{
			return;
		}
	} // end of method MMFF94NonBonded::updateForces()

	double MMFF94NonBonded::getElectrostaticEnergy() const
		throw()
	{
		return electrostatic_energy_;
	}

	double MMFF94NonBonded::getVdwEnergy() const
		throw()
	{
		return vdw_energy_;
	}

} // namespace BALL
