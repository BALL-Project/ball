// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyGeometricObjectList.C,v 1.2 2003/12/11 14:22:24 amoll Exp $
//

#include <BALL/PYTHON/pyGeometricObjectList.h>

namespace BALL
{

	PyGeometricObjectList::PyGeometricObjectList()
		: List<GeometricObject*>()
	{
	}

	PyGeometricObjectList::PyGeometricObjectList(const PyGeometricObjectList& geometricObject_list)
		: List<GeometricObject*>(geometricObject_list)
  {
	}

	PyGeometricObjectList::PyGeometricObjectList(const List<GeometricObject*>& geometricObject_list)
		: List<GeometricObject*>(geometricObject_list)
  {
	}

	PyGeometricObjectList::~PyGeometricObjectList()
		throw()
	{
	}

}
