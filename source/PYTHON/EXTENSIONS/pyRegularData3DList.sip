// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyRegularData3DList.sip,v 1.1.2.2 2006/10/04 11:48:03 amoll Exp $
//

%MappedType PyRegularData3DList
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyRegularData3DList.h>
	#include "sipBALLRegularData3D.h"
  #include <BALL/PYTHON/pyBALLSipHelper.h>
%End

%ConvertFromTypeCode
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyRegularData3DList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		RegularData3D& obj = **it;
		PyObject *inst = (PyObject*)sipConvertFromInstance(&obj, sipClass_RegularData3D, 0);
		if (inst == NULL || PyList_Append(pl,inst) < 0)
		{
			Py_DECREF(pl);
			return NULL;
		}
	}

	return pl;
%End
 
%ConvertToTypeCode
	// Convert a Python list of RegularData3D instances to an RegularData3DList object on the
	// heap.
	//
	
	if (sipIsErr == NULL)
	{
		return PyList_Check(sipPy);
	}
 
	PyRegularData3DList *regd_list = new PyRegularData3DList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		RegularData3D* regd;
 
		regd = reinterpret_cast<RegularData3D*>(sipForceConvertTo_RegularData3D(PyList_GET_ITEM(sipPy,i),sipIsErr));
 
		if (*sipIsErr)
		{
			delete regd_list;
			return 0;
		}
 
		regd_list -> push_back(regd);
	}
 
	*sipCppPtr = regd_list;

	return 1;
%End
};

