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
		PyObject *inst = sipConvertFromType(&obj, sipType_ColorRGBA, 0);
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
		ColorRGBA* c = reinterpret_cast<ColorRGBA*>(sipForceConvertToType(PyList_GET_ITEM(sipPy,i), sipType_ColorRGBA, NULL, SIP_NO_CONVERTORS, NULL,sipIsErr));
 
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

