// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

%MappedType PyColorRGBAVector
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyColorRGBAVector.h>
%End

%ConvertFromTypeCode
	PyObject* pl = PyList_New(0);

	if (pl == NULL) return NULL;

	// Convert the list.

	PyColorRGBAVector::ConstIterator it = sipCpp->begin();
	for (; it != sipCpp->end(); ++it)
	{
		ColorRGBA& obj = *(ColorRGBA*)&*it;
		PyObject *inst = BALL_CONVERT_FROM_INSTANCE(&obj, ColorRGBA, 0);
		if (inst == NULL || PyList_Append(pl,inst) < 0)
		{
			Py_DECREF(pl);
			return NULL;
		}
	}

	return pl;
%End
 
%ConvertToTypeCode
	// Convert a Python list of ColorRGBA instances to an ColorRGBAVector object on the heap.
	
	if (sipIsErr == NULL) return PyList_Check(sipPy);
 
	PyColorRGBAVector* cl = new PyColorRGBAVector;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		ColorRGBA* c = reinterpret_cast<ColorRGBA*>(BALL_FORCE_CONVERT_TO_TYPE(PyList_GET_ITEM(sipPy,i), ColorRGBA));
 
		if (*sipIsErr)
		{
			delete cl;
			return 0;
		}
 
		cl-> push_back(*c);
	}
 
	*sipCppPtr = cl;

	return 1;
%End
};

