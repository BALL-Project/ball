// $Id: pyBondList.C,v 1.3 2001/07/25 11:34:51 oliver Exp $

#include <BALL/PYTHON/pyBondList.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atomContainer.h>
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
		throw()
	{
	}

	PyBondList::PyBondList(const AtomContainer& fragment, bool selected_only )
	{
		set(fragment, selected_only);
	}

	PyBondList::PyBondList(const Atom& atom)
	{
		set(atom);
	}

	void PyBondList::set(const AtomContainer& fragment, bool selected_only)
	{
		// clear the old contents of the list
		clear();

		// iterate over all atoms
		HashSet<Bond*> bond_set;
		AtomConstIterator atom_it = fragment.beginAtom();
		for (; +atom_it; ++atom_it)
		{
			if (atom_it->isSelected() || !selected_only)
			{
				// it the atom is selected or selection is irrelevant,
				// insert all bonds into the bond_set
				Atom::BondConstIterator bond_it = atom_it->beginBond();
				for (; +bond_it; ++bond_it)
				{
					bond_set.insert(const_cast<Bond*>(&*bond_it));
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

		Atom::BondConstIterator bond_it = atom.beginBond();
		for (; +bond_it; ++bond_it)
		{
			push_back(const_cast<Bond*>(&*bond_it));
		}
	}	
}
