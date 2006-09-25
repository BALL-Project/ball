// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorRGBAVector.sip,v 1.2.2.1 2006/09/25 11:37:36 amoll Exp $
//

%MappedType PyColorRGBAVector
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/colorRGBA.h>
	#include <BALL/PYTHON/pyColorRGBAVector.h>
	#include "sipBALLColorRGBA.h"
%End

%ConvertFromTypeCode
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the vector.

	for (PyColorRGBAVector::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		ColorRGBA* obj = (ColorRGBA*)&*it;

		if (PyList_Append(pl, sipConvertFromInstance(obj, sipClass_ColorRGBA), 0) < 0)
		{
			Py_DECREF(pl);
			return NULL;
		}
	}

	return pl;
%End
 
%ConvertToTypeCode
	// Convert a Python list of ColorRGBA instances to an ColorRGBAVector object on the
	// heap.
 
	PyColorRGBAVector *c_vector = new PyColorRGBAVector;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		ColorRGBA color;
 
		color = *reinterpret_cast<ColorRGBA*>(sipForceConvertTo_ColorRGBA(PyList_GET_ITEM(sipPy,i),sipIsErr));
 
		if (*sipIsErr)
		{
			delete c_vector;
			return 0;
		}
 
		c_vector->push_back(color);
	}
 
	*sipCppPtr = c_vector;

	return 1;
%End
};

