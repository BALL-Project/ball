// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/MOLMEC/CHARMM/charmmBend.h>
#include <BALL/MOLMEC/CHARMM/charmm.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>

using namespace std;

namespace BALL
{

	// default constructor
	CharmmBend::CharmmBend()
	{
		// set component name
		setName("CHARMM Bend");
	}

	// constructor
	CharmmBend::CharmmBend(ForceField& force_field)
		: BendComponent(force_field)
	{
		// set component name
		setName( "CHARMM Bend" );
	}

	// destructor
	CharmmBend::~CharmmBend()
	{
	}


	// setup the internal datastructures for the component
	bool CharmmBend::setup()
		throw(Exception::TooManyErrors)
	{
		// clear old bends:
		bend_.clear();

		if (getForceField() == 0) 
		{
			Log.error() << "CharmmBend::setup: component not bound to force field" << endl;
			return false;
		}

 		Options& options = getForceField()->options;
		if (options.has(CHARMM_BENDS_ENABLED))
		{
			if (!options.getBool(CHARMM_BENDS_ENABLED))
			{
				setEnabled(false);
				return true;
			}
			else
			{
				setEnabled(true);
			}
		}

		// extract parameter section (if necessary)
		CharmmFF* charmm_force_field = dynamic_cast<CharmmFF*>(force_field_);
		if ((charmm_force_field == 0) || !charmm_force_field->hasInitializedParameters())
		{
			bool result = false;
			result = bend_parameters_.extractSection(getForceField()->getParameters(), "QuadraticAngleBend");

			if (!result)
			{
				Log.error() << "cannot find section QuadraticAngleBend" << endl;
				return false;
			}
		}

		// retrieve all bend parameters
		vector<Atom*>::const_iterator	atom_it = getForceField()->getAtoms().begin();
		Atom::BondIterator it1;
		Atom::BondIterator it2;
		QuadraticAngleBend::Data	this_bend;
		for ( ; atom_it != getForceField()->getAtoms().end(); ++atom_it) 
		{
			for (it2 = (*atom_it)->beginBond(); +it2 ; ++it2) 
			{
				if (it2->getType() == Bond::TYPE__HYDROGEN) continue; // Skip H-bonds!
				for (it1 = it2, ++it1; +it1 ; ++it1 ) 
				{
					if (it1->getType() == Bond::TYPE__HYDROGEN) continue; // Skip H-bonds!				
					this_bend.atom1 = (*it2).getPartner(**atom_it);
					this_bend.atom2 = (*atom_it);
					this_bend.atom3 = (*it1).getPartner(**atom_it);

					if (!getForceField()->getUseSelection() || (getForceField()->getUseSelection()
					    && this_bend.atom1->isSelected()
					    && this_bend.atom2->isSelected()
					    && this_bend.atom3->isSelected()))
					{ 

						Atom::Type atom_type_a1 = this_bend.atom1->getType();
						Atom::Type atom_type_a2 = this_bend.atom2->getType();
						Atom::Type atom_type_a3 = this_bend.atom3->getType();

						// retrieve the parameters. QuadraticAngleBend assumes
						// that the second atom is the central atom, the order
						// of the other two atoms doesn't matter
						QuadraticAngleBend::Values values;
						if (!bend_parameters_.assignParameters(values, atom_type_a1, atom_type_a2, atom_type_a3))
						{
							getForceField()->error() << "cannot find bend parameters for atoms "
		            << this_bend.atom1->getFullName() << ", "
								<< this_bend.atom2->getFullName() << ", and "
								<< this_bend.atom3->getFullName() << " (types are "
								<< force_field_->getParameters().getAtomTypes().getTypeName(atom_type_a1) << "-"
								<< force_field_->getParameters().getAtomTypes().getTypeName(atom_type_a2) << "-"
								<< force_field_->getParameters().getAtomTypes().getTypeName(atom_type_a3) << ")" << endl;

							getForceField()->getUnassignedAtoms().insert(it2->getPartner(**atom_it));
							getForceField()->getUnassignedAtoms().insert(*atom_it);
							getForceField()->getUnassignedAtoms().insert(it1->getPartner(**atom_it));
							// this stretch will not cause energies or forces
							values.k = 0.0;
							values.theta0 = 0.0;
						}
						this_bend.values = values;
						bend_.push_back(this_bend);
					}
				}
			}
		}

		// everything went well
		return true;
	}

} // namespace BALL 
