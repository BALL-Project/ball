// $Id: assignment.C,v 1.3 2000/05/25 11:02:40 oliver Exp $

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
		
