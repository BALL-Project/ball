// $Id: pyBondList.C,v 1.1 2000/07/18 21:18:41 oliver Exp $

#include <BALL/PYTHON/pyBondList.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/baseFragment.h>
#include <BALL/DATATYPE/hashSet.h>

namespace BALL
{

	PyBondList::PyBondList()
		: List<Bond*>()
	{
	}

	PyBondList::PyBondList(const PyBondList& new_list)
		: List<Bond*>(new_list)
  {
	}

	PyBondList::~PyBondList()
	{
	}

	PyBondList::PyBondList(const BaseFragment& fragment, bool selected_only )
	{
		set(fragment, selected_only);
	}

	PyBondList::PyBondList(const Atom& atom)
	{
		set(atom);
	}

	void PyBondList::set(const BaseFragment& fragment, bool selected_only)
	{
		// clear the old contents of the list
		clear();

		// iterate over all atoms
		HashSet<Bond*> bond_set;
		AtomIterator atom_it = fragment.beginAtom();
		for (; +atom_it; ++atom_it)
		{
			if (atom_it->isSelected() || !selected_only)
			{
				// it the atom is selected or selection is irrelevant,
				// insert all bonds into the bond_set
				Atom::BondIterator bond_it = atom_it->beginBond();
				for (; +bond_it; ++bond_it)
				{
					bond_set.insert(&*bond_it);
				}
			}
		}
		
		// copy the results from the hash set to the list
		for (HashSet<Bond*>::Iterator it = bond_set.begin(); +it; ++it)
		{
			push_back(*it);
		}		
	}

	void PyBondList::set(const Atom& atom)
	{
		// clear the old contents of the list
		clear();

		Atom::BondIterator bond_it = atom.beginBond();
		for (; +bond_it; ++bond_it)
		{
			push_back(&*bond_it);
		}
	}	
}
