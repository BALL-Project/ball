// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94OutOfPlaneBend.C,v 1.1.2.1 2005/04/06 16:01:53 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94OutOfPlaneBend.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/SYSTEM/path.h>

#include <math.h>

//     #define BALL_DEBUG_MMFF

using namespace std;

namespace BALL 
{

	/// 0.043844 / 2
	#define K0 0.021922

	// default constructor
	MMFF94OutOfPlaneBend::MMFF94OutOfPlaneBend()
		:	ForceFieldComponent()
	{	
		// set component name
		setName("MMFF94 OutOfPlaneBend");
	}


	// constructor
	MMFF94OutOfPlaneBend::MMFF94OutOfPlaneBend(ForceField& force_field)
		: ForceFieldComponent(force_field)
	{
		// set component name
		setName("MMFF94 OutOfPlaneBend");
	}


	// copy constructor
	MMFF94OutOfPlaneBend::MMFF94OutOfPlaneBend(const MMFF94OutOfPlaneBend&	component)
		:	ForceFieldComponent(component)
	{
	}

	// destructor
	MMFF94OutOfPlaneBend::~MMFF94OutOfPlaneBend()
	{
	}


	// setup the internal datastructures for the component
	bool MMFF94OutOfPlaneBend::setup()
		throw(Exception::TooManyErrors)
	{
		if (getForceField() == 0) 
		{
			Log.error() << "MMFF94OutOfPlaneBend::setup(): component not bound to force field" << endl;
			return false;
		}

		if (!parameters_.isInitialized())
		{
			Path    path;
			String  filename(path.find("MMFF94/MMFFOOP.PAR"));

			if (filename == "") 
			{
				throw Exception::FileNotFound(__FILE__, __LINE__, filename);
			}

			parameters_.readParameters(filename);
		}

		oop_bends_.clear();

		bool use_selection = getForceField()->getUseSelection();

		// a working instance to put the current values in and push it back
		OutOfPlaneBend this_bend;

 		const MMFF94AtomTypeEquivalences& equiv= ((MMFF94*)getForceField())->getEquivalences();

		vector<Atom*>::const_iterator	atom_it = getForceField()->getAtoms().begin();
		Atom::BondIterator bond_it;
		for ( ; atom_it != getForceField()->getAtoms().end(); ++atom_it) 
		{
			// search for tricoordinate centers (atoms with three bonds)
			const Size nr_bonds = (*atom_it)->countBonds();
			if (nr_bonds < 3) continue;

			// count hydrogen bonds
			Size nr_hydrogen_bonds = 0;
			for (bond_it = (*atom_it)->beginBond(); +bond_it; ++bond_it)
			{
				if (it2->getType() == Bond::TYPE__HYDROGEN) nr_hydrogen_bonds++;
			}

			if (nr_bonds - nr_hydrogen_bonds != 3) continue;

			vector<Atom*> partners;

			Atom& central_atom = *atom_it;

			for (bond_it = central_atom.beginBond(); +bond_it; ++bond_it)
			{
				Bond& bond = *bond_it;

				if (bond.getType() == Bond::TYPE__HYDROGEN) continue; // Skip H-Bonds;

				Atom* partner = bond.getPartner(central_atom);

				partners.push_back(partner);
			}

			if (use_selection && (!central_atom.isSelected() ||
														!partners[1]->isSelected() ||
														!partners[2]->isSelected() ||
														!partners[3]->isSelected()))
			{
				continue;
			}


			this_bend.i= &Atom::getAttributes()[partners[0]->getIndex()];
			this_bend.j= &Atom::getAttributes()[central_atom->getIndex()];
			this_bend.k= &Atom::getAttributes()[partners[1]->getIndex()];
			this_bend.l= &Atom::getAttributes()[partners[2]->getIndex()];

			Position type_i = partners[0]->getType();
			Position type_j = central_atom.getType();
			Position type_k = partners[1]->getType();
			Position type_l = partners[2]->getType();

			// check for parameters in a step down procedure
			// we ignore the step 1-1-1, as it is currently superflous
			
			// full parameters
			if (parameters_.getParameters(this_bend.k_oop, type_i, type_j, type_k, type_l))
					|| // 2-2-2-2
					parameters_.getParameters(this_bend.k_oop, 
																		equiv.getEquivalence(type_i, 2),
																		equiv.getEquivalence(type_j, 2),
																		equiv.getEquivalence(type_k, 2),
																		equiv.getEquivalence(type_l, 2))
					|| // 3-2-3-3
					parameters_.getParameters(this_bend.k_oop, 
																		equiv.getEquivalence(type_i, 3),
																		equiv.getEquivalence(type_j, 2),
																		equiv.getEquivalence(type_k, 3),
																		equiv.getEquivalence(type_l, 3))
					|| // 4-2-4-4
					parameters_.getParameters(this_bend.k_oop, 
																		equiv.getEquivalence(type_i, 4),
																		equiv.getEquivalence(type_j, 2),
																		equiv.getEquivalence(type_k, 4),
																		equiv.getEquivalence(type_l, 4))
					|| // 0-2-0-0 = full wildcard matching
					parameters_.getParameters(this_bend.k_oop, 0, equiv.getEquivalence(type_j, 2), 0, 0);

			{
				bends_.push_back(bend);
				continue;
			}

			// complain if nothing was found
			getForceField()->error() << "MMFF94OutOfPlaneBend::setup: cannot find OutOfPlaneBend parameters for atom types:"
															 << central_atom->getType() << " "
															 << partners[0]->getType() << " "
															 << partners[1]->getType() << " "
															 << partners[2]->getType() << " "
															 << " (atoms are: " << 
															 << this_bend.j->ptr->getFullName() << "/" 
															 << this_bend.i->ptr->getFullName() << "/" 
															 << this_bend.k->ptr->getFullName() << "/" 
															 << this_bend.l->ptr->getFullName() << ")" << endl;

			getForceField()->getUnassignedAtoms().insert(partners[0]);
			getForceField()->getUnassignedAtoms().insert(partners[1]);
			getForceField()->getUnassignedAtoms().insert(partners[2]);
			getForceField()->getUnassignedAtoms().insert(&central_atom);
		}

		// everything went well
		return true;
	}

	// calculates the current energy of this component
	double MMFF94OutOfPlaneBend::updateEnergy()
	{
		energy_ = 0;
		
		vector<OutOfPlaneBend>::iterator bend_it = bends_.begin();

		float radian_to_degree = 180.0 / Constants::PI;
		float degree_to_radian= Constants::PI / 180;

		for (; bend_it != bends_.end(); ++bend_it) 
		{
		}
		
		return energy_;
	}


	// calculates and adds its forces to the current forces of the force field
	void MMFF94OutOfPlaneBend::updateForces()
	{
	}

} // namespace BALL
