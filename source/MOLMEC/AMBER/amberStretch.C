// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/MOLMEC/AMBER/amberStretch.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>

using namespace std;

namespace BALL 
{

	// default constructor
	AmberStretch::AmberStretch()
	{
		// set component name
		setName( "Amber Stretch" );
	}


	// constructor
	AmberStretch::AmberStretch(ForceField& force_field)
		 : StretchComponent(force_field)
	{
		// set component name
		setName( "Amber Stretch" );
	}

	// destructor
	AmberStretch::~AmberStretch()
	{
	}

	// setup the internal datastructures for the component
	bool AmberStretch::setup()
		throw(Exception::TooManyErrors)
	{
		if (getForceField() == 0) 
		{
			Log.error() << "AmberStretch::setup(): component not bound to force field" << endl;
			return false;
		}

		// throw away the old contents
		stretch_.clear();

 		Options& options = getForceField()->options;
		if (options.has(AMBER_STRETCH_ENABLED))
		{
			if (!options.getBool(AMBER_STRETCH_ENABLED))
			{
				setEnabled(false);
				return true;
			}
			else
			{
				setEnabled(true);
			}
		}

		AmberFF* amber_force_field = dynamic_cast<AmberFF*>(force_field_);
		if ((amber_force_field == 0) || !amber_force_field->hasInitializedParameters())
		{
			bool result = stretch_parameters_.extractSection(getForceField()->getParameters(), "QuadraticBondStretch");

			if (!result) 
			{
				Log.error() << "cannot find section QuadraticBondStretch" << endl;
				return false;
			}
		}

		QuadraticBondStretch::Values values;
		bool use_selection = getForceField()->getUseSelection();

		// retrieve all stretch parameters
		Atom::BondIterator bond_iterator;
		AtomVector::ConstIterator atom_it = getForceField()->getAtoms().begin();
		for ( ; atom_it != getForceField()->getAtoms().end(); ++atom_it)
		{
			for (Atom::BondIterator it = (*atom_it)->beginBond(); +it ; ++it) 
			{
				if (*atom_it == it->getFirstAtom()) 
				{
					Bond&	bond = const_cast<Bond&>(*it);
					if (bond.getType() == Bond::TYPE__HYDROGEN)
					{	
						// Ignore hydrogen bonds!
						continue;
					}

					if (!use_selection ||
							(use_selection && bond.getFirstAtom()->isSelected() && 
							 									bond.getSecondAtom()->isSelected()))
					{
						Atom::Type atom_type_A = bond.getFirstAtom()->getType();
						Atom::Type atom_type_B = bond.getSecondAtom()->getType();
						stretch_.push_back(QuadraticBondStretch::Data());
						stretch_.back().atom1 = bond.getFirstAtom();
						stretch_.back().atom2 = bond.getSecondAtom();
			
						// Pay attention to the symmetric database input
						if (stretch_parameters_.hasParameters(atom_type_A, atom_type_B)) 
						{
							stretch_parameters_.assignParameters(values, atom_type_A, atom_type_B);
						} 
						else if (stretch_parameters_.hasParameters(atom_type_A, Atom::ANY_TYPE)) 
						{
							stretch_parameters_.assignParameters(values, atom_type_A, Atom::ANY_TYPE);
						} 
						else if (stretch_parameters_.hasParameters(Atom::ANY_TYPE, atom_type_B)) 
						{
							stretch_parameters_.assignParameters(values, Atom::ANY_TYPE, atom_type_B); 
						} 
						else if (stretch_parameters_.hasParameters(Atom::ANY_TYPE, Atom::ANY_TYPE)) 
						{
							stretch_parameters_.assignParameters(values,Atom::ANY_TYPE, Atom::ANY_TYPE);
						} 
						else 
						{
							getForceField()->error() << "cannot find stretch parameters for atom types " 
								<< force_field_->getParameters().getAtomTypes().getTypeName(atom_type_A) << "-" 
								<< force_field_->getParameters().getAtomTypes().getTypeName(atom_type_B)
								<< " (atoms are: " 
								<< stretch_.back().atom1->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID) 
								<< "/" << stretch_.back().atom2->getFullName(
										Atom::ADD_VARIANT_EXTENSIONS_AND_ID) << ")" << endl;

							// we don't want to get any force or energy component
							// from this stretch
							values.k = 0.0;
							values.r0 = 1.0;	

							getForceField()->getUnassignedAtoms().insert(bond.getFirstAtom());
							getForceField()->getUnassignedAtoms().insert(bond.getSecondAtom());
						}
						stretch_.back().values = values;
					}
 				}
			}
		}
		
		// Everything went well.
		return true;
	}
} // namespace BALL
