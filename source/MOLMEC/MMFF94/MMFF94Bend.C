// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Bend.C,v 1.1.2.1 2005/03/17 13:48:24 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94Bend.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>

using namespace std;

namespace BALL 
{

	// default constructor
	MMFF94Bend::MMFF94Bend()
		:	ForceFieldComponent()
	{	
		// set component name
		setName("MMFF94 Bend");
	}


	// constructor
	MMFF94Bend::MMFF94Bend(ForceField& force_field)
		:	ForceFieldComponent(force_field)
	{
		// set component name
		setName("MMFF94 Bend");
	}


	// copy constructor
	MMFF94Bend::MMFF94Bend(const MMFF94Bend&	component)
		:	ForceFieldComponent(component)
	{
	}

	// destructor
	MMFF94Bend::~MMFF94Bend()
	{
	}

	// setup the internal datastructures for the component
	bool MMFF94Bend::setup()
		throw(Exception::TooManyErrors)
	{
		// clear old bends:

		if (getForceField() == 0) 
		{
			Log.error() << "MMFF94Bend::setup: component not bound to force field" << endl;
			return false;
		}

		static bool result = false;
		MMFF94* MMFF94_force_field = dynamic_cast<MMFF94*>(force_field_);
		if ((MMFF94_force_field == 0) || !MMFF94_force_field->hasInitializedParameters())
		{
			result = true; // bend_parameters.extractSection(getForceField()->getParameters(), "AngleBend");

			if (result == false) 
			{
				Log.error() << "MMFF94Bend::setup: cannot find section AngleBend" << endl;
				return false;
			}
		}

		// Assign all bend parameters.
		bool use_selection = getForceField()->getUseSelection();
		vector<Atom*>::const_iterator	atom_it = getForceField()->getAtoms().begin();
		Atom::BondIterator it1;
		Atom::BondIterator it2;

		// everything went well
		return true;
	}

	// calculates the current energy of this component
	double MMFF94Bend::updateEnergy()
	{
		energy_ = 0;
		
		Vector3 v1, v2;
		bool use_selection = getForceField()->getUseSelection();
		
		return energy_;
	}

	// calculates and adds its forces to the current forces of the force field
	void MMFF94Bend::updateForces()
	{
		if ((getForceField() == 0) || (getForceField()->getSystem() == 0))
		{
			return;
		}

		bool use_selection = getForceField()->getUseSelection();
	}

} // namespace BALL 
