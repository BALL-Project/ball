// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyCompositeList.C,v 1.1 2003/11/15 10:59:07 oliver Exp $
//

#include <BALL/PYTHON/pyCompositeList.h>
#include <BALL/KERNEL/composite.h>

namespace BALL
{

	PyCompositeList::PyCompositeList()
		: List<Composite*>()
	{
	}

	PyCompositeList::PyCompositeList(const PyCompositeList& new_list)
		: List<Composite*>(new_list)
  {
	}

	PyCompositeList::~PyCompositeList()
		throw()
	{
	}

	PyCompositeList::PyCompositeList(const Composite& fragment, bool selected_only )
	{
		set(fragment, selected_only);
	}

	void PyCompositeList::set(const Composite& fragment, bool selected_only)
	{
		// clear the old contents of the list
		clear();

		// iterate over all composites
		CompositeConstIterator it = fragment.beginComposite();

    for (; +it; ++it)
    {
      const Composite* composite = dynamic_cast<const Composite*>(&*it);
      if ((composite != 0) && (it->isSelected() || !selected_only))
      {
        // store the composite pointer in the list
        push_back(const_cast<Composite*>(composite));
			}
		}
	}
}
