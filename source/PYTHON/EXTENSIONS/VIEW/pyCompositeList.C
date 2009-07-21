// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyCompositeList.C,v 1.6.20.1 2007-03-25 21:37:21 oliver Exp $
//

#include <BALL/PYTHON/pyCompositeList.h>
#include <BALL/CONCEPT/composite.h>

namespace BALL
{

	PyCompositeList::PyCompositeList()
		: std::list<Composite*>()
	{
	}

	PyCompositeList::PyCompositeList(const PyCompositeList& composite_list)
		: std::list<Composite*>(composite_list)
  {
	}

	PyCompositeList::PyCompositeList(const std::list<Composite*>& composite_list)
		: std::list<Composite*>(composite_list)
  {
	}

	PyCompositeList::PyCompositeList(const std::list<const Composite*>& composite_list)
		: std::list<Composite*>()
  {
		std::list<const Composite*>::const_iterator it(composite_list.begin());
		for (; it != composite_list.end(); ++it)
		{
			push_back(const_cast<Composite*>(*it));
		}
	}

	PyCompositeList& PyCompositeList::operator = (const std::list<const Composite*>& composite_list)
	{
		clear();

		std::list<const Composite*>::const_iterator it(composite_list.begin());
		for (; it != composite_list.end(); ++it)
		{
			push_back(const_cast<Composite*>(*it));
		}

		return *this;
	}

	PyCompositeList::PyCompositeList(const HashSet<Composite*>& composite_set)
		: std::list<Composite*>()
  {
		std::copy(composite_set.begin(), composite_set.end(), std::inserter(*this, begin()));
	}

	PyCompositeList::PyCompositeList(const HashSet<const Composite*>& composite_set)
		: std::list<Composite*>()
  {
		HashSet<const Composite*>::ConstIterator it(composite_set.begin());
		for (; +it; ++it)
		{
			push_back(const_cast<Composite*>(*it));
		}
	}

	PyCompositeList::~PyCompositeList()
		throw()
	{
	}

}
