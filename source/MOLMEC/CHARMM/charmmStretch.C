// $Id: charmmStretch.C,v 1.3 2000/02/14 22:44:08 oliver Exp $

#include <BALL/MOLMEC/CHARMM/charmmStretch.h>
#include <BALL/MOLMEC/CHARMM/charmm.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>

using namespace std;

namespace BALL 
{

	// default constructor
	CharmmStretch::CharmmStretch()
		:	ForceFieldComponent(),
			stretch_(0),
			number_of_stretches_(0)
	{	
		// set component name
		setName("CHARMM Stretch");

		setForceField(0);
	}


	// constructor
	CharmmStretch::CharmmStretch(ForceField* force_field)
		 : 	ForceFieldComponent(),
				stretch_(0),
				number_of_stretches_(0)
	{
		// set component name
		setName("CHARMM Stretch");

		setForceField(force_field);
	}


	// copy constructor
	CharmmStretch::CharmmStretch(const CharmmStretch&	component, bool clone_deep)
		:	ForceFieldComponent(component, clone_deep)
	{
		number_of_stretches_ = component.number_of_stretches_;
		stretch_ = new QuadraticBondStretch::Data[number_of_stretches_];

		for (Size i = 0; i < number_of_stretches_; i++) 
		{
			stretch_[i] = component.stretch_[i];
		}
	}

	// destructor
	CharmmStretch::~CharmmStretch()
	{
		// clear the stretches array
		delete [] stretch_;
		stretch_ = 0;
		number_of_stretches_ = 0;
	}


	// setup the internal datastructures for the component
	bool CharmmStretch::setup()
	{
		if (getForceField() == 0) 
		{
			Log.error() << "CharmmStretch::setup(): component not bound to force field" << endl;
			return false;
		}

		delete [] stretch_;
		stretch_ = 0;

		// and memorize the number of stretches
		number_of_stretches_ = 0;
		Atom::BondIterator bond_iterator;

		vector<Atom*>::const_iterator atom_it = getForceField()->getAtoms().begin();
		for (; atom_it != getForceField()->getAtoms().end(); ++atom_it)
		{
			for (bond_iterator = (*atom_it)->beginBond(); +bond_iterator; ++bond_iterator)
			{
				if ( getForceField()->getUseSelection() == false ||
				    (getForceField()->getUseSelection() == true &&
				    ((*bond_iterator).getFirstAtom()->isSelected() && (*bond_iterator).getSecondAtom()->isSelected())))
				{
					if ((*bond_iterator).getPartner(**atom_it) == (*bond_iterator).getSecondAtom())
					{
						number_of_stretches_++;
					}
				}
			}
		}


		// are there bonds ?
		if ( number_of_stretches_  == 0) 
		{
			return true;
		}

		// allocate space for all stretches
		stretch_ = new QuadraticBondStretch::Data[number_of_stretches_];
		
		CharmmFF* charmm_force_field = dynamic_cast<CharmmFF*>(force_field_);
		if ((charmm_force_field == 0) || !charmm_force_field->hasInitializedParameters())
		{
			bool result = stretch_parameters_.extractSection(getForceField()->getParameters(), "QuadraticBondStretch");

			if (result == false) 
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

					if (getForceField()->getUseSelection() == false ||
					   (getForceField()->getUseSelection() == true && 
					   (bond.getFirstAtom()->isSelected() && bond.getSecondAtom()->isSelected())))
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
										Log.warn() << "cannot find stretch parameters for atoms " 
															 << stretch_[i].atom1->getFullName() << " and " 
															 << stretch_[i].atom2->getFullName() << " (types are "
															 << force_field_->getParameters().getAtomTypes().getTypeName(atom_type_A) << "-" 
															 << force_field_->getParameters().getAtomTypes().getTypeName(atom_type_B) << ")" << endl;
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

	// calculates the current energy of this component
	float CharmmStretch::updateEnergy()
	{

		// initial energy is zero
		energy_ = 0;

		// iterate over all bonds, sum up the energies
		for (Size i = 0; i < number_of_stretches_; i++)
		{
			if (getForceField()->getUseSelection() == false ||
			   (getForceField()->getUseSelection() == true && 
			   (stretch_[i].atom1->isSelected() || stretch_[i].atom2->isSelected())))
			{
				float distance = (stretch_[i].atom1->getPosition()).getDistance(stretch_[i].atom2->getPosition());
				energy_ += stretch_[i].values.k * (distance - stretch_[i].values.r0) * (distance - stretch_[i].values.r0);

			}
		}
		
		return energy_;
	}

	// calculates and adds its forces to the current forces of the force field
	void CharmmStretch::updateForces()
	{

		// iterate over all bonds, update the forces
		for (Size i = 0 ; i < number_of_stretches_; i++)
		{
			if (getForceField()->getUseSelection() == false ||
			   (getForceField()->getUseSelection() == true && 
			   (stretch_[i].atom1->isSelected() || stretch_[i].atom2->isSelected())))
			{

				Vector3 direction(stretch_[i].atom1->getPosition() - stretch_[i].atom2->getPosition());
				float distance = direction.getLength(); 

				if (distance != 0) 
				{
					// unit conversion: from kJ/(mol A) -> N
					//   kJ -> J: 1e3
					//   A  -> m: 1e10
					//   J/mol -> J: Avogadro
					direction *= 1e13 / Constants::AVOGADRO * 2 * stretch_[i].values.k * (distance - stretch_[i].values.r0)/distance;

					if (getForceField()->getUseSelection() == false)
					{
						stretch_[i].atom1->setForce(stretch_[i].atom1->getForce() - direction);
						stretch_[i].atom2->setForce(stretch_[i].atom2->getForce() + direction);
					} else {
						if (stretch_[i].atom1->isSelected())
						{
							stretch_[i].atom1->setForce(stretch_[i].atom1->getForce() - direction);
						}
						if (stretch_[i].atom2->isSelected())
						{
							stretch_[i].atom2->setForce(stretch_[i].atom2->getForce() + direction);
						}
					}
				}
			}
		}                                                                                                          
	}

} // namespace BALL
