// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyFragmentList.C,v 1.1.2.1 2003/01/07 13:21:46 anker Exp $

#include <BALL/PYTHON/pyFragmentList.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/atomContainer.h>

namespace BALL
{

	PyFragmentList::PyFragmentList()
		: List<Fragment*>()
	{
	}

	PyFragmentList::PyFragmentList(const PyFragmentList& new_list)
		: List<Fragment*>(new_list)
  {
	}

	PyFragmentList::~PyFragmentList()
		throw()
	{
	}

	PyFragmentList::PyFragmentList(const AtomContainer& fragment, bool selected_only )
	{
		set(fragment, selected_only);
	}

	void PyFragmentList::set(const AtomContainer& fragment, bool selected_only)
	{
		// clear the old contents of the list
		clear();

		// iterate over all fragments
		AtomContainerConstIterator it = fragment.beginAtomContainer();

		for (; +it; ++it)
		{
			const Fragment* fragment = dynamic_cast<const Fragment*>(&*it);
			if ((fragment != 0) && (it->isSelected() || !selected_only))
			{
				// store the fragment pointer in the list
				push_back(const_cast<Fragment*>(fragment));
			}
		}
	}
}
