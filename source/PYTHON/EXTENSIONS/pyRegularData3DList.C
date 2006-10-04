// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyRegularData3DList.C,v 1.1.2.1 2006/10/04 11:33:09 amoll Exp $
//

#include <BALL/PYTHON/pyRegularData3DList.h>

namespace BALL
{

	PyRegularData3DList::PyRegularData3DList()
		: List<RegularData3D*>()
	{
	}

	PyRegularData3DList::PyRegularData3DList(const PyRegularData3DList& composite_list)
		: List<RegularData3D*>(composite_list)
  {
	}

	PyRegularData3DList::PyRegularData3DList(const List<std::pair<RegularData3D*, String> > l)
		: List<RegularData3D*>()
  {
		List<std::pair<RegularData3D*, String> >::const_iterator it(l.begin());
		for (; it != composite_list.end(); ++it)
		{
			push_back(const_cast<RegularData3D*>(*it->first));
		}
	}

	PyRegularData3DList::~PyRegularData3DList()
		throw()
	{
	}

}
