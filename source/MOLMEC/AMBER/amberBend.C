// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/MOLMEC/AMBER/amberBend.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>

using namespace std;

namespace BALL
{

	// default constructor
	AmberBend::AmberBend()
	{
		// set component name
		setName("Amber Bend");
	}


	// constructor
	AmberBend::AmberBend(ForceField& force_field)
		: BendComponent(force_field)
	{
		// set component name
		setName("Amber Bend");
	}

	// destructor
	AmberBend::~AmberBend()
	{
	}

	// setup the internal datastructures for the component
	bool AmberBend::setup()
		throw(Exception::TooManyErrors)
	{
		// clear old bends:
		bend_.clear();

		if (getForceField() == 0) 
		{
			Log.error() << "AmberBend::setup: component not bound to force field" << endl;
			return false;
		}

 		Options& options = getForceField()->options;
		if (options.has(AMBER_BEND_ENABLED))
		{
			if (!options.getBool(AMBER_BEND_ENABLED))
			{
				setEnabled(false);
				return true;
			}
			else
			{
				setEnabled(true);
			}
		}

		static QuadraticAngleBend bend_parameters;
		static bool result = false;
		AmberFF* amber_force_field = dynamic_cast<AmberFF*>(force_field_);
		if ((amber_force_field == 0) || !amber_force_field->hasInitializedParameters())
		{
			result = bend_parameters.extractSection(getForceField()->getParameters(), "QuadraticAngleBend");

			if (result == false) 
			{
				Log.error() << "AmberBend::setup: cannot find section QuadraticAngleBend" << endl;
				return false;
			}
		}

		// Assign all bend parameters.
		bool use_selection = getForceField()->getUseSelection();
		vector<Atom*>::const_iterator	atom_it = getForceField()->getAtoms().begin();
		Atom::BondIterator it1;
		Atom::BondIterator it2;
		QuadraticAngleBend::Data this_bend;
		for ( ; atom_it != getForceField()->getAtoms().end(); ++atom_it) 
		{
			for (it2 = (*atom_it)->beginBond(); +it2 ; ++it2) 
			{
				if (it2->getType() == Bond::TYPE__HYDROGEN) continue; // Skip H-bonds
				for (it1 = it2, ++it1; +it1 ; ++it1) 
				{
					if (it1->getType() == Bond::TYPE__HYDROGEN) continue; // Skip H-Bonds;
					this_bend.atom1 = it2->getPartner(**atom_it);
					this_bend.atom2 = (*atom_it);
					this_bend.atom3 = it1->getPartner(**atom_it);

					if ((use_selection == false) ||
					   (use_selection == true &&
					   (   this_bend.atom1->isSelected()
							&& this_bend.atom2->isSelected()
							&& this_bend.atom3->isSelected())))
					{ 

						Atom::Type atom_type_a1 = this_bend.atom1->getType();
						Atom::Type atom_type_a2 = this_bend.atom2->getType();
						Atom::Type atom_type_a3 = this_bend.atom3->getType();

						// check for parameters
						if (!bend_parameters.assignParameters(this_bend.values, atom_type_a1, atom_type_a2, atom_type_a3))
						{
							// handle wildcards: if the atom type is not known, try to match *-A2-* 
							if (!bend_parameters.assignParameters(this_bend.values, Atom::ANY_TYPE, atom_type_a2, Atom::ANY_TYPE))
							{
								// complain if nothing was found
								getForceField()->error() << "AmberBend::setup: cannot find bend parameters for atom types:"
									<< force_field_->getParameters().getAtomTypes().getTypeName(atom_type_a1) << "-"
									<< force_field_->getParameters().getAtomTypes().getTypeName(atom_type_a2) << "-"
									<< force_field_->getParameters().getAtomTypes().getTypeName(atom_type_a3) 
									<< " (atoms are: " << this_bend.atom1->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID) << "/" 
									                   << this_bend.atom2->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID) << "/" 
									                   << this_bend.atom3->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID) << ")" << endl;

								getForceField()->getUnassignedAtoms().insert(it2->getPartner(**atom_it));
   							getForceField()->getUnassignedAtoms().insert(*atom_it);
								getForceField()->getUnassignedAtoms().insert(it1->getPartner(**atom_it));
							}
							else
							{
								// store the bend parameters otherwise
								bend_.push_back(this_bend);
							}
						} 
						else 
						{
							// store the bend parameters otherwise
							bend_.push_back(this_bend);
						}
					}
				}
			}
		}

		// everything went well
		return true;
	}

} // namespace BALL 
