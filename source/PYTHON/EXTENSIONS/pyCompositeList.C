// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyCompositeList.C,v 1.2 2003/11/15 12:53:57 oliver Exp $
//

#include <BALL/PYTHON/pyCompositeList.h>
#include <BALL/CONCEPT/composite.h>

namespace BALL
{

	PyCompositeList::PyCompositeList()
		: List<Composite*>()
	{
	}

	PyCompositeList::PyCompositeList(const PyCompositeList& composite_list)
		: List<Composite*>(composite_list)
  {
	}

	PyCompositeList::PyCompositeList(const List<Composite*>& composite_list)
		: List<Composite*>(composite_list)
  {
	}

	PyCompositeList::PyCompositeList(const HashSet<Composite*>& composite_set)
		: List<Composite*>()
  {
		std::copy(composite_set.begin(), composite_set.end(), std::back_inserter<List<Composite*> >(*this));
	}

	PyCompositeList::~PyCompositeList()
		throw()
	{
	}

}
