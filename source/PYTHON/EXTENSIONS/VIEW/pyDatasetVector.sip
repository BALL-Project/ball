// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

%MappedType PyDatasetVector
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyDatasetVector.h>
%End

%ConvertFromTypeCode
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyDatasetVector::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		Dataset* obj = *it;

		if (PyList_Append(pl, sipConvertFromType(obj, sipType_Dataset, 0)) < 0)
		{
			Py_DECREF(pl);
			return NULL;
		}
	}

	return pl;
%End
 
%ConvertToTypeCode
	// Convert a Python list of Dataset instances to an DatasetVector object on the
	// heap.
	//
	
	if (sipIsErr == NULL)
	{
		return PyList_Check(sipPy);
	}
 
	PyDatasetVector *dvector = new PyDatasetVector;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		Dataset* set;
		set = reinterpret_cast<Dataset*>(sipForceConvertToType(PyList_GET_ITEM(sipPy,i), sipType_Dataset, NULL, SIP_NO_CONVERTORS, NULL, sipIsErr));
 
		if (*sipIsErr)
		{
			delete dvector;
			return 0;
		}
 
		dvector -> push_back(set);
	}
 
	*sipCppPtr = dvector;

	return 1;
%End
};

