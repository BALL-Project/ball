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
		:	ForceFieldComponent()
	{	
		// set component name
		setName("CHARMM Bend");
	}


	// constructor
	CharmmBend::CharmmBend(ForceField& force_field)
		:	ForceFieldComponent(force_field)
	{
		// set component name
		setName( "CHARMM Bend" );
	}


	// copy constructor
	CharmmBend::CharmmBend(const CharmmBend&	component)
		:	ForceFieldComponent(component)
	{
		bend_ = component.bend_;
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

			if (result == false) 
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
					this_bend.atom1 = &Atom::getAttributes()[(*it2).getPartner(**atom_it)->getIndex()];
					this_bend.atom2 = &Atom::getAttributes()[(*atom_it)->getIndex()];
					this_bend.atom3 = &Atom::getAttributes()[(*it1).getPartner(**atom_it)->getIndex()];

					if (getForceField()->getUseSelection() == false ||
					   (getForceField()->getUseSelection() == true && 
					   (this_bend.atom1->ptr->isSelected() 
							&& this_bend.atom2->ptr->isSelected() 
							&& this_bend.atom3->ptr->isSelected())))
					{ 

						Atom::Type atom_type_a1 = this_bend.atom1->type;
						Atom::Type atom_type_a2 = this_bend.atom2->type;
						Atom::Type atom_type_a3 = this_bend.atom3->type;

						// retrieve the parameters. QuadraticAngleBend assumes
						// that the second atom is the central atom, the order
						// of the other two atoms doesn't matter
						QuadraticAngleBend::Values values;
						if (!bend_parameters_.assignParameters(values, atom_type_a1, atom_type_a2, atom_type_a3))
						{
							getForceField()->error() << "cannot find bend parameters for atoms "
								<< this_bend.atom1->ptr->getFullName() << ", " << this_bend.atom2->ptr->getFullName() 
								<< ", and " << this_bend.atom3->ptr->getFullName() << " (types are " 
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

	// calculates the current energy of this component
	double CharmmBend::updateEnergy()
	{
		double length;
		energy_ = 0;

		for (Size i = 0 ; i < bend_.size() ; i++) 
		{

			if (getForceField()->getUseSelection() == false ||
					(getForceField()->getUseSelection() == true  &&
					(bend_[i].atom1->ptr->isSelected() 
					 || bend_[i].atom2->ptr->isSelected() 
					 || bend_[i].atom3->ptr->isSelected())))
			{

				Vector3 v1 = bend_[i].atom1->position - bend_[i].atom2->position;
				length = v1.getLength();

				if (length == 0) 
				{
					continue;
				}

				double inverse_length = 1 / length;
				v1 *= inverse_length;
				Vector3 v2 = bend_[i].atom3->position - bend_[i].atom2->position;
				length = v2.getLength();

				if (length == 0) 
				{
					continue;
				}

				inverse_length = 1/length;
				v2 *= inverse_length;

				double costheta = v1 * v2;
				double theta;
				if (costheta > 1.0) 
				{	
					theta = 0.0;
				}
				else if (costheta < -1.0) 
				{
					theta = Constants::PI;
				}
				else 
				{
					theta = acos(costheta);
				}
			

				energy_ += bend_[i].values.k * (theta - bend_[i].values.theta0) * (theta - bend_[i].values.theta0);
			}

		}

		return energy_;
	}

	// calculates and adds its forces to the current forces of the force field
	void CharmmBend::updateForces()
	{

		double length;

		for (Size i = 0; i < bend_.size(); i++) 
		{
			if (getForceField()->getUseSelection() == false ||
					(getForceField()->getUseSelection()  == true  &&
					(bend_[i].atom1->ptr->isSelected() 
					 || bend_[i].atom2->ptr->isSelected() || bend_[i].atom3->ptr->isSelected())))
			{
				// Calculate the vector between atom1 and atom2,
				// test if the vector has length larger than 0 and normalize it

				Vector3 v1 = bend_[i].atom1->position - bend_[i].atom2->position;
				length = v1.getLength();
				if (length == 0) continue;
				double inverse_length_v1 = 1/length;
				v1 *= inverse_length_v1 ;

				// Calculate the vector between atom3 and atom2,
				// test if the vector has length larger than 0 and normalize it

				Vector3 v2 = bend_[i].atom3->position - bend_[i].atom2->position;
				length = v2.getLength();
				if (length == 0) continue;
				double inverse_length_v2 = 1/length;
				v2 *= inverse_length_v2;

				// Calculate the cos of theta and then theta
				double costheta = v1 * v2;
				double theta;
				if (costheta > 1.0) theta = 0.0;
				else if (costheta < -1.0) theta = Constants::PI;
				else theta = acos(costheta);

				// unit conversion: kJ/(mol A) -> N
				// kJ -> J: 1e3
				// A -> m : 1e10
				// J/mol -> mol: Avogadro
				double factor = 1e13 / Constants::AVOGADRO * 2 * bend_[i].values.k * (theta - bend_[i].values.theta0);

				// Calculate the cross product of v1 and v2, test if it has length unequal 0,
				// and normalize it.

				Vector3 cross = v1 % v2;
				if ((length = cross.getLength()) != 0) 
				{
					cross *= (1/length);
				} 
				else 
				{
					continue;
				}

				Vector3 n1 = v1 % cross;
				Vector3 n2 = v2 % cross; 
				n1 *= factor * inverse_length_v1;
				n2 *= factor * inverse_length_v2;

				if (getForceField()->getUseSelection() == false)
				{
					bend_[i].atom1->force -= n1;
					bend_[i].atom2->force += n1;
					bend_[i].atom2->force -= n2;
					bend_[i].atom3->force += n2;
				} 
				else 
				{
					if (bend_[i].atom1->ptr->isSelected()) 
					{
						bend_[i].atom1->force -= n1;
					}
	
					if (bend_[i].atom2->ptr->isSelected())
					{
						bend_[i].atom2->force += n1;
						bend_[i].atom2->force -= n2;
					}
					if (bend_[i].atom3->ptr->isSelected())
					{
						bend_[i].atom3->force += n2;
					}
				}
			}
		}
	}

} // namespace BALL 
