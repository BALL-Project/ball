// $Id: pyChainList.C,v 1.1 2000/06/27 13:10:22 oliver Exp $

#include <BALL/PYTHON/pyChainList.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/baseFragment.h>

namespace BALL
{

	PyChainList::PyChainList()
		: List<Chain*>()
	{
	}

	PyChainList::PyChainList(const PyChainList& new_list)
		: List<Chain*>(new_list)
  {
	}

	PyChainList::~PyChainList()
	{
	}

	PyChainList::PyChainList(const BaseFragment& fragment, bool selected_only )
	{
		set(fragment, selected_only);
	}

	void PyChainList::set(const BaseFragment& fragment, bool selected_only)
	{
		// clear the old contents of the list
		clear();

		// iterate over all chains
		BaseFragmentConstIterator it = fragment.beginBaseFragment();

    for (; +it; ++it)
    {
      Chain* chain = const_cast<Chain*>(&dynamic_cast<const Chain&>(*it));
      if ((chain != 0) && (it->isSelected() || !selected_only))
      {
        // store the chain pointer in the list
        push_back(chain);
			}
		}
	}
}
