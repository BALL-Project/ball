// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyChainList.C,v 1.1.2.1 2003/01/07 13:21:46 anker Exp $

#include <BALL/PYTHON/pyChainList.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/atomContainer.h>

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
		throw()
	{
	}

	PyChainList::PyChainList(const AtomContainer& fragment, bool selected_only )
	{
		set(fragment, selected_only);
	}

	void PyChainList::set(const AtomContainer& fragment, bool selected_only)
	{
		// clear the old contents of the list
		clear();

		// iterate over all chains
		AtomContainerConstIterator it = fragment.beginAtomContainer();

    for (; +it; ++it)
    {
      const Chain* chain = dynamic_cast<const Chain*>(&*it);
      if ((chain != 0) && (it->isSelected() || !selected_only))
      {
        // store the chain pointer in the list
        push_back(const_cast<Chain*>(chain));
			}
		}
	}
}
