// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Torsion.C,v 1.1.2.1 2005/03/17 13:48:25 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94Torsion.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>

#include <algorithm>

using namespace std;

namespace BALL 
{

	// default constructor
	MMFF94Torsion::MMFF94Torsion()
		:	ForceFieldComponent()
	{	
		// set component name
		setName("MMFF94 Torsion");
	}


	// constructor
	MMFF94Torsion::MMFF94Torsion(ForceField& force_field)
		:	ForceFieldComponent(force_field)
	{
		// set component name
		setName( "MMFF94 Torsion" );
	}


	// copy constructor
	MMFF94Torsion::MMFF94Torsion(const MMFF94Torsion& component)
		:	ForceFieldComponent(component)
	{
	}

	// destructor
	MMFF94Torsion::~MMFF94Torsion()
	{
	}


	// setup the internal datastructures for the component
	bool MMFF94Torsion::setup()
		throw(Exception::TooManyErrors)
	{
		if (getForceField() == 0) 
		{
			Log.error() << "MMFF94Torsion::setup: component not bound to force field" << endl;
			return false;
		}

		// clear torsion array

		// extract the torsion parameters from the parameter file
		bool result;
		MMFF94* MMFF94_force_field = dynamic_cast<MMFF94*>(force_field_);
		bool has_initialized_parameters = true;
		if ((MMFF94_force_field == 0) || !MMFF94_force_field->hasInitializedParameters())
		{
			has_initialized_parameters = false;
		}

		if (!has_initialized_parameters)
		{
			result = true; // torsion_parameters_.extractSection(getForceField()->getParameters(), "Torsions");

			if (!result) 
			{
				Log.error() << "MMFF94Torsion::setup: cannot find section Torsions" << endl;
				return false;
			}
		}

		// calculate the torsions
		vector<Atom*>::const_iterator atom_it = getForceField()->getAtoms().begin();
		Atom::BondIterator it1;
		Atom::BondIterator it2;
		Atom::BondIterator it3;

		Atom*	a1;
		Atom*	a2;
		Atom*	a3;
		Atom*	a4;

		bool use_selection = getForceField()->getUseSelection();

		// proper torsion will be added to the torsion vector

		return true;
	}

	// calculates the current energy of this component
	double MMFF94Torsion::updateEnergy() 
	{
		double cosphi;

		Vector3	a21;
		Vector3 a23;
		Vector3 a34;
		Vector3 cross2321;
		Vector3 cross2334;

		energy_ = 0;

		/*
		vector<SingleMMFF94Torsion>::const_iterator it = torsion_.begin(); 

		bool use_selection = getForceField()->getUseSelection();

		for (; it != torsion_.end(); it++) 
		{
			if (!use_selection ||
					(use_selection  &&
					(it->atom1->ptr->isSelected() || it->atom2->ptr->isSelected() 
					 || it->atom3->ptr->isSelected() || it->atom4->ptr->isSelected())))
			{
				a21 = it->atom1->position - it->atom2->position;
				a23 = it->atom3->position - it->atom2->position;
				a34 = it->atom4->position - it->atom3->position;

				cross2321 = a23 % a21;
				cross2334 = a23 % a34;

				double length_cross2321 = cross2321.getLength();
				double length_cross2334 = cross2334.getLength();

				if (length_cross2321 != 0 && length_cross2334 != 0) 
				{
					cross2321 /= length_cross2321;
					cross2334 /= length_cross2334;

					cosphi = cross2321 * cross2334;

					if (cosphi > 1.0)
					{
						cosphi = 1.0;
					}
					if (cosphi < -1.0)
					{
						cosphi = -1.0;
					}

//   					energy_ += it->V * (1 + cos(it->f * acos(cosphi) - it->phase));
				}
			}
		}
*/
		return energy_;
	}

	// calculates and adds its forces to the current forces of the force field
	void MMFF94Torsion::updateForces()
	{
		bool use_selection = getForceField()->getUseSelection();
	}

} // namespace BALL
