// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Stretch.C,v 1.1.2.1 2005/03/17 13:48:25 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94Stretch.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>

using namespace std;

namespace BALL 
{

	// default constructor
	MMFF94Stretch::MMFF94Stretch()
		:	ForceFieldComponent()
	{	
		// set component name
		setName( "MMFF94 Stretch" );
	}


	// constructor
	MMFF94Stretch::MMFF94Stretch(ForceField& force_field)
		 : 	ForceFieldComponent(force_field)
	{
		// set component name
		setName( "MMFF94 Stretch" );
	}


	// copy constructor
	MMFF94Stretch::MMFF94Stretch(const MMFF94Stretch&	component)
		:	ForceFieldComponent(component)
	{
	}

	// destructor
	MMFF94Stretch::~MMFF94Stretch()
	{
	}


	// setup the internal datastructures for the component
	bool MMFF94Stretch::setup()
		throw(Exception::TooManyErrors)
	{
		if (getForceField() == 0) 
		{
			Log.error() << "MMFF94Stretch::setup(): component not bound to force field" << endl;
			return false;
		}

		// throw away the old contents

		MMFF94* MMFF94_force_field = dynamic_cast<MMFF94*>(force_field_);
		if ((MMFF94_force_field == 0) || !MMFF94_force_field->hasInitializedParameters())
		{
			bool result = true; // = stretch_parameters_.extractSection(getForceField()->getParameters(), "BondStretch");

			if (!result) 
			{
				Log.error() << "cannot find section QuadraticBondStretch" << endl;
				return false;
			}
		}

//   		QuadraticBondStretch::Values values;
		bool use_selection = getForceField()->getUseSelection();
		
		// Everything went well.
		return true;
	}

	// update bond lists if the selection has changed
	void MMFF94Stretch::update()
		throw(Exception::TooManyErrors)
	{
	}
	

	// calculates the current energy of this component
	double MMFF94Stretch::updateEnergy()
	{
		// initial energy is zero
		energy_ = 0;

		bool use_selection = getForceField()->getUseSelection();

		return energy_;
	}

	// calculates and adds its forces to the current forces of the force field
	void MMFF94Stretch::updateForces()
	{
		if (getForceField() == 0)
		{
			return;
		}

		bool use_selection = getForceField()->getUseSelection();

		/*
		// iterate over all bonds, update the forces
		for (Size i = 0 ; i < stretch_.size(); i++)
		{
			Atom::StaticAtomAttributes& atom1(*stretch_[i].atom1);
			Atom::StaticAtomAttributes& atom2(*stretch_[i].atom2);
			Vector3 direction(atom1.position - atom2.position);
			double distance = direction.getLength(); 

			if (distance != 0.0) 
			{
				// unit conversion: from kJ/(mol A) -> N
				//   kJ -> J: 1e3
				//   A  -> m: 1e10
				//   J/mol -> J: Avogadro
//   				direction *= 1e13 / Constants::AVOGADRO * 2 * stretch_[i].values.k * (distance - stretch_[i].values.r0) / distance;

				if (!use_selection || atom1.ptr->isSelected()) 
				{
					atom1.force -= direction;
				}
				if (!use_selection || atom2.ptr->isSelected()) 
				{
					atom2.force += direction;
				}
			}
		}                                                                                                          
		*/
	}

} // namespace BALL
