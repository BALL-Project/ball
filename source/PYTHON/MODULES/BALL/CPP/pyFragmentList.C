// $Id: pyFragmentList.C,v 1.2 2000/08/30 19:58:38 oliver Exp $

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
