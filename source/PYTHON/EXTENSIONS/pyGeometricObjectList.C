// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyGeometricObjectList.C,v 1.1 2003/12/11 02:15:00 amoll Exp $
//

#include <BALL/PYTHON/pyGeometricObjectList.h>
#include <BALL/VIEW/KERNEL/geometricObject.h>

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
