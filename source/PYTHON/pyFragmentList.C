// $Id: pyFragmentList.C,v 1.2 2000/07/03 11:11:10 oliver Exp $

#include <BALL/PYTHON/pyFragmentList.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/baseFragment.h>

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
	{
	}

	PyFragmentList::PyFragmentList(const BaseFragment& fragment, bool selected_only )
	{
		set(fragment, selected_only);
	}

	void PyFragmentList::set(const BaseFragment& fragment, bool selected_only)
	{
		// clear the old contents of the list
		clear();

		// iterate over all fragments
		BaseFragmentConstIterator it = fragment.beginBaseFragment();

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
