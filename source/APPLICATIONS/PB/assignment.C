// $Id: assignment.C,v 1.5 2000/06/02 09:32:53 oliver Exp $

#include "global.h"
#include "assignment.h"

#include <BALL/STRUCTURE/defaultProcessors.h>

using namespace std;

void assignCharges(System& system)
{
	if (clear_charges)
	{
		// clear all charges in the system
		system.apply(clear_charge_proc);
	}
	else 
	{
		// assign the current charge set
		if (use_charge_rules)
		{
			system.apply(charge_rules);
		}
		else 
		{
			system.apply(charges);
		}
	}

	if (verbose)
	{
		AtomIterator atom_it = system.beginAtom();
		float charge = 0.0;
		for (; +atom_it; ++atom_it)
		{
			charge += atom_it->getCharge();
		}
		Log.info() << "assigned a total charge of " << charge << " e0" << endl;
	}
}


void assignRadii(System& system)
{
	// assign the current charge set
	if (use_radius_rules)
	{
		system.apply(radius_rules);
	}
	else 
	{
		system.apply(radii);
	}
}

void normalizeNames(System& system)
{
	if (normalize_names)
	{
		// create a fragment DB id required
		if (frag_db == 0)
		{
			frag_db = new FragmentDB;
		}
		
		// apply the normalize names processor
		system.apply(frag_db->normalize_names);
	}
}
		
void buildBonds(System& system)
{
	if (build_bonds)
	{
		// create a fragment DB id required
		if (frag_db == 0)
		{
			frag_db = new FragmentDB;
		}
		
		// apply the normalize names processor
		system.apply(frag_db->build_bonds);
	}
}
		
