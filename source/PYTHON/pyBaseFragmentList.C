// $Id: pyBaseFragmentList.C,v 1.1 2000/07/03 11:11:08 oliver Exp $

#include <BALL/PYTHON/pyBaseFragmentList.h>
#include <BALL/KERNEL/baseFragment.h>

namespace BALL
{

	PyBaseFragmentList::PyBaseFragmentList()
		: List<BaseFragment*>()
	{
	}

	PyBaseFragmentList::PyBaseFragmentList(const PyBaseFragmentList& new_list)
		: List<BaseFragment*>(new_list)
  {
	}

	PyBaseFragmentList::~PyBaseFragmentList()
	{
	}

	PyBaseFragmentList::PyBaseFragmentList(const BaseFragment& fragment, bool selected_only )
	{
		set(fragment, selected_only);
	}

	void PyBaseFragmentList::set(const BaseFragment& fragment, bool selected_only)
	{
		// clear the old contents of the list
		clear();

		// iterate over all base fragments
		BaseFragmentConstIterator it = fragment.beginBaseFragment();
		for (; +it; ++it)
		{
			if (!selected_only || it->isSelected())
			{
				// store the base fragment pointer in the list
				push_back(const_cast<BaseFragment*>(&*it));
			}
		}
	}
}
