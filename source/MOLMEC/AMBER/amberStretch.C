// $Id: amberStretch.C,v 1.13 2001/06/26 02:40:33 oliver Exp $

#include <BALL/MOLMEC/AMBER/amberStretch.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>

using namespace std;

namespace BALL 
{

	// default constructor
	AmberStretch::AmberStretch()
		:	ForceFieldComponent(),
			stretch_()
	{	
		// set component name
		setName( "Amber Stretch" );
	}


	// constructor
	AmberStretch::AmberStretch(ForceField& force_field)
		 : 	ForceFieldComponent(force_field),
				stretch_()
	{
		// set component name
		setName( "Amber Stretch" );
	}


	// copy constructor
	AmberStretch::AmberStretch(const AmberStretch&	component, bool clone_deep)
		:	ForceFieldComponent(component, clone_deep)
	{
		stretch_ = component.stretch_;
	}

	// destructor
	AmberStretch::~AmberStretch()
	{
	}


	// setup the internal datastructures for the component
	bool AmberStretch::setup()
	{
		if (getForceField() == 0) 
		{
			Log.error() << "AmberStretch::setup(): component not bound to force field" << endl;
			return false;
		}

		// throw away the old contents
		stretch_.clear();

		AmberFF* amber_force_field = dynamic_cast<AmberFF*>(force_field_);
		if ((amber_force_field == 0) || !amber_force_field->hasInitializedParameters())
		{
			bool result = stretch_parameters_.extractSection(getForceField()->getParameters(), "QuadraticBondStretch");

			if (result == false) 
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

					if (use_selection == false ||
					   (use_selection == true && 
					   (bond.getFirstAtom()->isSelected() || bond.getSecondAtom()->isSelected())))
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
							Log.error() << "cannot find stretch parameters for atom types " 
													<< force_field_->getParameters().getAtomTypes().getTypeName(atom_type_A) << "-" 
													<< force_field_->getParameters().getAtomTypes().getTypeName(atom_type_B)
													<< " (atoms are: " << stretch_.back().atom1->getFullName() 
													<< "/" << stretch_.back().atom2->getFullName() << ")" << endl;

							// we don't want to get any force or energy component
							// from this stretch
							values.k = 0.0;
							values.r0 = 1.0;	
						}
						stretch_.back().values = values;
					}
 				}
			}
		}
		
		// Everything went well.
		return true;
	}

	// update bond lists if the selection has changed
	void AmberStretch::update()
	{
		if ((getForceField() == 0) || (getForceField()->getSystem() == 0))
		{
			return;
		}

		if (getForceField()->getUpdateTime().isOlderThan(getForceField()->getSystem()->getSelectionTime()))
		{
			setup();
		}
	}
	

	// calculates the current energy of this component
	double AmberStretch::updateEnergy()
	{
		// initial energy is zero
		energy_ = 0;

		// iterate over all bonds, sum up the energies
		for (Size i = 0; i < stretch_.size(); i++)
		{
			double distance = (stretch_[i].atom1->getPosition()).getDistance(stretch_[i].atom2->getPosition());
			energy_ += stretch_[i].values.k * (distance - stretch_[i].values.r0) * (distance - stretch_[i].values.r0);
		}
		
		return energy_;
	}

	// calculates and adds its forces to the current forces of the force field
	void AmberStretch::updateForces()
	{
		// iterate over all bonds, update the forces
		for (Size i = 0 ; i < stretch_.size(); i++)
		{
			Atom& atom1(*stretch_[i].atom1);
			Atom& atom2(*stretch_[i].atom2);
			Vector3 direction(atom1.getPosition() - atom2.getPosition());
			double distance = direction.getLength(); 

			if (distance != 0.0) 
			{
				// unit conversion: from kJ/(mol A) -> N
				//   kJ -> J: 1e3
				//   A  -> m: 1e10
				//   J/mol -> J: Avogadro
				direction *= 1e13 / Constants::AVOGADRO * 2 * stretch_[i].values.k * (distance - stretch_[i].values.r0) / distance;

				if (atom1.isSelected()) 
				{
					atom1.setForce(atom1.getForce() - direction);
				}
				if (atom2.isSelected()) 
				{
					atom2.setForce(atom2.getForce() + direction);
				}
			}
		}                                                                                                          
	}

} // namespace BALL
