// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Stretch.C,v 1.1.2.15 2005/03/28 00:43:56 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94Stretch.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/SYSTEM/path.h>

#define BALL_DEBUG_MMFF

using namespace std;

namespace BALL 
{

	// Constant CS
	#define CUBIC_STRENGTH_CONSTANT -2.0

	// Constant 7 / 12 * CS^2
	#define KCS 7.0 / 3.0

	// Constant 143.9325 / 2
	#define K0 71.96625


	// default constructor
	MMFF94Stretch::MMFF94Stretch()
		:	ForceFieldComponent()
	{	
		// set component name
		setName("MMFF94 Stretch");
	}


	// constructor
	MMFF94Stretch::MMFF94Stretch(ForceField& force_field)
		: ForceFieldComponent(force_field)
	{
		// set component name
		setName("MMFF94 Stretch");
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

		stretch_.clear();
		
		// a working instance to put the current values in and push it back
		Stretch dummy_stretch;

		MMFF94& mmff = *(MMFF94*)getForceField();

		const MMFF94BondStretchParameters& stretch_data = mmff.getStretchParameters();
		MMFF94BondStretchParameters::StretchMap::ConstIterator stretch_it;

		const vector<Bond*>& bonds = mmff.getBonds();
		
		vector<Bond*>::const_iterator bond_it = bonds.begin();
		for (; bond_it != bonds.end(); bond_it++)
		{
			stretch_it = stretch_data.getParameters(**bond_it);
			
			Atom& atom1 = *(Atom*)(*bond_it)->getFirstAtom();
			Atom& atom2 = *(Atom*)(*bond_it)->getSecondAtom();

			if (!+stretch_it)
			{
				getForceField()->error() << "cannot find stretch parameters for atom types " 
 					<< atom1.getType() << " "
 					<< atom2.getType() << " "
					<< " (atoms are: " 
					<< atom1.getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID) << " " 
					<< atom2.getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID) 
					<< ")" << std::endl;

				getForceField()->getUnassignedAtoms().insert(&atom1);
				getForceField()->getUnassignedAtoms().insert(&atom2);
				continue;
			}

			const bool is_sbmb = (**bond_it).hasProperty("MMFF94SBMB");
			dummy_stretch.sbmb = is_sbmb;
			dummy_stretch.atom1 = &atom1; 
			dummy_stretch.atom2 = &atom2;

			const MMFF94BondStretchParameters::BondData& data = stretch_it->second;

			if (is_sbmb)
			{
				dummy_stretch.r0 = data.r0_sbmb;
				dummy_stretch.kb = data.kb_sbmb;
			}
			else
			{
				dummy_stretch.r0 = data.r0_normal;
				dummy_stretch.kb = data.kb_normal;
			}

			stretch_.push_back(dummy_stretch);
		}

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

		for (Size i = 0 ; i < stretch_.size(); i++)
		{
			const Vector3 direction(stretch_[i].atom1->getPosition() - stretch_[i].atom2->getPosition());
			double distance = direction.getLength(); 
			const double delta(distance - (double) stretch_[i].r0);
			stretch_[i].delta_r = delta;
			const double delta_2(delta * delta);

			double eb_ij = (double) K0 * (double) stretch_[i].kb * delta_2 *
				            ((double) 1.0 + (double) CUBIC_STRENGTH_CONSTANT * delta + (double) KCS * delta_2);

#ifdef BALL_DEBUG_MMFF
			Log.info() << stretch_[i].atom1->getFullName() << " -> " 
								 << stretch_[i].atom2->getFullName() 
								 << "   r0: " << stretch_[i].r0
								 << "   D: " << delta << "   E: " << eb_ij << std::endl;
#endif

			energy_ += eb_ij;
		}


		return energy_;
	}

	// calculates and adds its forces to the current forces of the force field
	void MMFF94Stretch::updateForces()
	{
		if (getForceField() == 0)
		{
			return;
		}

		// iterate over all bonds, update the forces
		for (Size i = 0 ; i < stretch_.size(); i++)
		{
			Vector3 direction(stretch_[i].atom1->getPosition() - stretch_[i].atom2->getPosition());
			const double r0(stretch_[i].r0);
			const double delta(direction.getLength() - r0);

			const double a(143.9325 / 2  * r0);

			double force = -(2 * a * delta + 
											 3 * a * CUBIC_STRENGTH_CONSTANT * delta * delta + 
											 4 * a * KCS * delta * delta);

			direction.normalize();
			direction *= force;

			stretch_[i].atom1->getForce()-= direction;
			stretch_[i].atom2->getForce()+= direction;
		}                                                                                                          
	}

} // namespace BALL
