// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyGeometricObjectList.sip,v 1.3 2003/12/11 02:32:54 amoll Exp $
//

%MappedType PyGeometricObjectList
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyGeometricObjectList.h>
	#include "sipBALLGeometricObject.h"
  //#include <BALL/PYTHON/pyBALLSipHelper.h>
%End

%ConvertFromTypeCode
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyGeometricObjectList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		GeometricObject& obj = **it;
		PyObject *inst;

		if ((inst = sipMapCppToSelf(&obj, sipClass_GeometricObject)) == NULL || PyList_Append(pl,inst) < 0)
		{
			Py_DECREF(pl);
			return NULL;
		}
	}

	return pl;
%End
 
%ConvertToTypeCode
	// Convert a Python list of GeometricObject instances to an GeometricObjectList object on the
	// heap.
 
	PyGeometricObjectList *geometric_object_list = new PyGeometricObjectList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		GeometricObject* geometric_object;
 
		geometric_object = sipForceConvertTo_GeometricObject(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete geometric_object_list;
			return 0;
		}
 
		geometric_object_list -> push_back(geometric_object);
	}
 
	*sipCppPtr = geometric_object_list;

	return 1;
%End
};

