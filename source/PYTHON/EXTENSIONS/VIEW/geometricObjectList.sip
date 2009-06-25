// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

%MappedType GeometricObjectList
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/representation.h>
	#include <BALL/VIEW/KERNEL/geometricObject.h>
	typedef Representation::GeometricObjectList GeometricObjectList;
%End

%ConvertFromTypeCode
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (GeometricObjectList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		GeometricObject* obj = *it;

		if (PyList_Append(pl, sipConvertFromType(obj, sipType_GeometricObject, NULL, SIP_NO_CONVERTORS, NULL, 0)) < 0)
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
 
	GeometricObjectList *geometric_object_list = new GeometricObjectList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		GeometricObject* geometric_object;
 
		geometric_object = reinterpret_cast<GeometricObject*>(sipForceConvertToType(PyList_GET_ITEM(sipPy,i), sipType_GeometricObject, NULL, SIP_NO_CONVERTORS, NULL, sipIsErr));
 
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

