// $Id: amberBend.C,v 1.9 2000/03/26 12:52:24 oliver Exp $

#include <BALL/MOLMEC/AMBER/amberBend.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>

using namespace std;

namespace BALL 
{

	// default constructor
	AmberBend::AmberBend()
		:	ForceFieldComponent()
	{	
		// set component name
		setName("Amber Bend");

		setForceField(0);
	}


	// constructor
	AmberBend::AmberBend(ForceField* force_field)
		:	ForceFieldComponent(force_field)
	{
		// set component name
		setName( "Amber Bend" );

		setForceField(force_field);
	}


	// copy constructor
	AmberBend::AmberBend(const AmberBend&	component, bool deep)
		:	ForceFieldComponent(component, deep)
	{
		bend_ = component.bend_;
	}

	// destructor
	AmberBend::~AmberBend()
	{
	}


	// setup the internal datastructures for the component
	bool AmberBend::setup()
	{
		// clear old bends:
		bend_.clear();

		if (getForceField() == 0) 
		{
			Log.error() << "AmberBend::setup: component not bound to force field" << endl;
			return false;
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

		// retrieve all bend parameters
		vector<Atom*>::const_iterator	atom_it = getForceField()->getAtoms().begin();
		Atom::BondIterator it1;
		Atom::BondIterator it2;
		QuadraticAngleBend::Data	this_bend;
		for ( ; atom_it != getForceField()->getAtoms().end(); ++atom_it) 
		{
			for (it2 = (*atom_it)->beginBond(); +it2 ; ++it2) 
			{
				for (it1 = it2, ++it1; +it1 ; ++it1) 
				{
				
					this_bend.atom1 = it2->getPartner(**atom_it);
					this_bend.atom2 = *atom_it;
					this_bend.atom3 = it1->getPartner(**atom_it);

					if (getForceField()->getUseSelection() == false ||
					   (getForceField()->getUseSelection() == true && 
					   (this_bend.atom1->isSelected() && this_bend.atom2->isSelected() && this_bend.atom3->isSelected())))
					{ 

						Atom::Type atom_type_a1 = this_bend.atom1->getType();
						Atom::Type atom_type_a2 = this_bend.atom2->getType();
						Atom::Type atom_type_a3 = this_bend.atom3->getType();

						// check for parameters
						if (!bend_parameters.assignParameters(this_bend.values, atom_type_a1, atom_type_a2, atom_type_a3))
						{
							// complain if nothing was found
							Log.error() << "AmberBend::setup: cannot find bend parameters for atom types:"
								<< force_field_->getParameters().getAtomTypes().getTypeName(atom_type_a1) << "-"
								<< force_field_->getParameters().getAtomTypes().getTypeName(atom_type_a2) << "-"
								<< force_field_->getParameters().getAtomTypes().getTypeName(atom_type_a3) << endl;
						} else {
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

	// calculates the current energy of this component
	double AmberBend::updateEnergy()
	{
		double length;
		energy_ = 0;

		for (Size i = 0 ; i < bend_.size() ; i++) 
		{

			if (getForceField()->getUseSelection() == false ||
					(getForceField()->getUseSelection() == true  &&
					(bend_[i].atom1->isSelected() || bend_[i].atom2->isSelected() || bend_[i].atom3->isSelected())))
			{

				Vector3 v1 = bend_[i].atom1->getPosition() - bend_[i].atom2->getPosition();
				length = v1.getLength();

				if (length == 0) 
				{
					continue;
				}

				double inverse_length = 1 / length;
				v1 *= inverse_length;
				Vector3 v2 = bend_[i].atom3->getPosition() - bend_[i].atom2->getPosition();
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
	void AmberBend::updateForces()
	{

		double length;

		for (Size i = 0; i < bend_.size(); i++) 
		{
			if (getForceField()->getUseSelection() == false ||
					(getForceField()->getUseSelection()  == true  &&
					(bend_[i].atom1->isSelected() || bend_[i].atom2->isSelected() || bend_[i].atom3->isSelected())))
			{

				// Calculate the vector between atom1 and atom2,
				// test if the vector has length larger than 0 and normalize it

				Vector3 v1 = bend_[i].atom1->getPosition() - bend_[i].atom2->getPosition();
				length = v1.getLength();
				if (length == 0) continue;
				double inverse_length_v1 = 1/length;
				v1 *= inverse_length_v1 ;

				// Calculate the vector between atom3 and atom2,
				// test if the vector has length larger than 0 and normalize it

				Vector3 v2 = bend_[i].atom3->getPosition() - bend_[i].atom2->getPosition();
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
				} else {
					continue;
				}

				Vector3 n1 = v1 % cross;
				Vector3 n2 = v2 % cross; 
				n1 *= factor * inverse_length_v1;
				n2 *= factor * inverse_length_v2;

				if (getForceField()->getUseSelection() == false)
				{
					bend_[i].atom1->getForce() -= n1;
					bend_[i].atom2->getForce() += n1;
					bend_[i].atom2->getForce() -= n2;
					bend_[i].atom3->getForce() += n2;
				} else {
					if (bend_[i].atom1->isSelected()) 
					{
						bend_[i].atom1->getForce() -= n1;
					}
	
					if (bend_[i].atom2->isSelected())
					{
						bend_[i].atom2->getForce() += n1;
						bend_[i].atom2->getForce() -= n2;
					}
					if (bend_[i].atom3->isSelected())
					{
						bend_[i].atom3->getForce() += n2;
					}
				}
			}
		}
	}

} // namespace BALL 
