// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyCompositeList.C,v 1.3 2004/02/11 18:52:28 oliver Exp $
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
		std::copy(composite_set.begin(), composite_set.end(), std::inserter(*this, begin()));
	}

	PyCompositeList::PyCompositeList(const HashSet<const Composite*>& composite_set)
		: List<Composite*>()
  {
		HashSet<const Composite*>::ConstIterator it(composite_set.begin());
		for (; +it; ++it)
		{
			push_back(const_cast<Composite*>(*it));
		}
	}

	PyCompositeList::operator VIEW::Representation::CompositeSet () const
	{
		VIEW::Representation::CompositeSet tmp;
		std::copy(begin(), end(), std::inserter(tmp, tmp.begin()));
		return tmp;
	}

	PyCompositeList::~PyCompositeList()
		throw()
	{
	}

}
