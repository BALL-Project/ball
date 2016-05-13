// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/MOLMEC/CHARMM/charmmStretch.h>
#include <BALL/MOLMEC/CHARMM/charmm.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>

using namespace std;

namespace BALL 
{

	// default constructor
	CharmmStretch::CharmmStretch()
	{
		// set component name
		setName("CHARMM Stretch");
	}


	// constructor
	CharmmStretch::CharmmStretch(ForceField& force_field)
	 : StretchComponent(force_field)
	{
		// set component name
		setName("CHARMM Stretch");
	}

	// destructor
	CharmmStretch::~CharmmStretch()
	{
	}


	// setup the internal datastructures for the component
	bool CharmmStretch::setup()
		throw(Exception::TooManyErrors)
	{
		if (getForceField() == 0) 
		{
			Log.error() << "CharmmStretch::setup(): component not bound to force field" << endl;
			return false;
		}

		stretch_.clear();

 		Options& options = getForceField()->options;
		if (options.has(CHARMM_STRETCHES_ENABLED))
		{
			if (!options.getBool(CHARMM_STRETCHES_ENABLED))
			{
				setEnabled(false);
				return true;
			}
			else
			{
				setEnabled(true);
			}
		}

		Atom::BondIterator bond_iterator;

		//Ok, this variable can be eliminated in favour of stretch_.size()
		//however i was to afraid to break anything
		unsigned int number_of_stretches_ = 0;

		vector<Atom*>::const_iterator atom_it = getForceField()->getAtoms().begin();
		for (; atom_it != getForceField()->getAtoms().end(); ++atom_it)
		{
			for (bond_iterator = (*atom_it)->beginBond(); +bond_iterator; ++bond_iterator)
			{
				if (bond_iterator->getType() == Bond::TYPE__HYDROGEN) continue; // Skip H-bonds!
				if (!getForceField()->getUseSelection() || (getForceField()->getUseSelection()
				    && (*bond_iterator).getFirstAtom()->isSelected()
				    && (*bond_iterator).getSecondAtom()->isSelected()))
				{
					if ((*bond_iterator).getPartner(**atom_it) == (*bond_iterator).getSecondAtom())
					{
						number_of_stretches_++;
					}
				}
			}
		}


		// are there bonds ?
		if (number_of_stretches_  == 0) 
		{
			return true;
		}

		// allocate space for all stretches
		stretch_.resize(number_of_stretches_);
		
		CharmmFF* charmm_force_field = dynamic_cast<CharmmFF*>(force_field_);
		if ((charmm_force_field == 0) || !charmm_force_field->hasInitializedParameters())
		{
			bool result = stretch_parameters_.extractSection(getForceField()->getParameters(), "QuadraticBondStretch");

			if (!result)
			{
				Log.error() << "cannot find section QuadraticBondStretch" << endl;
				return false;
			}
		}

		QuadraticBondStretch::Values values;

		// retrieve all stretch parameters
		atom_it = getForceField()->getAtoms().begin();
		Size i = 0;
		for ( ; atom_it != getForceField()->getAtoms().end(); ++atom_it)
		{
			for (Atom::BondIterator it = (*atom_it)->beginBond(); +it ; ++it) 
			{
				if (*atom_it == (*it).getFirstAtom()) 
				{
					
					Bond&	bond = const_cast<Bond&>(*it);
					if (bond.getType() == Bond::TYPE__HYDROGEN) continue; // Skip H-bonds!
					if (!getForceField()->getUseSelection() || (getForceField()->getUseSelection()
					    && bond.getFirstAtom()->isSelected()
					    && bond.getSecondAtom()->isSelected()))
					{

						Atom::Type atom_type_A = bond.getFirstAtom()->getType();
						Atom::Type atom_type_B = bond.getSecondAtom()->getType();
			
						stretch_[i].atom1 = bond.getFirstAtom();
						stretch_[i].atom2 = bond.getSecondAtom();
			
						// when retrieving the parameters, order does not matter
						// first, we try an exact match, than we try wildcards
						if (!stretch_parameters_.assignParameters(values, atom_type_A, atom_type_B)) 
						{
							if (!stretch_parameters_.assignParameters(values, atom_type_A, Atom::ANY_TYPE)) 
							{
								if (!stretch_parameters_.assignParameters(values, Atom::ANY_TYPE, atom_type_B))
								{
									if (!stretch_parameters_.assignParameters(values, Atom::ANY_TYPE, Atom::ANY_TYPE))
									{
										getForceField()->error() << "cannot find stretch parameters for atoms " 
															 << stretch_[i].atom1->getFullName() << " and " 
															 << stretch_[i].atom2->getFullName() << " (types are "
															 << force_field_->getParameters().getAtomTypes().getTypeName(atom_type_A) << "-" 
															 << force_field_->getParameters().getAtomTypes().getTypeName(atom_type_B) << ")" << endl;

										getForceField()->getUnassignedAtoms().insert(bond.getFirstAtom());
										getForceField()->getUnassignedAtoms().insert(bond.getSecondAtom());

										// we don`t want to get any force or energy component
										// from this stretch
										values.k = 0.0;
										values.r0 = 1.0;	
									}
								}
							}
						}

						// store the stretch parameters
						stretch_[i].values = values;
						i++;
					}
 				}
			}
		}
		
		// Everything went well.
		return true;
	}

} // namespace BALL
