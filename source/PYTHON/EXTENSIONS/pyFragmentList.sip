// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyFragmentList.sip,v 1.5 2003/12/18 19:03:20 oliver Exp $
//

%MappedType PyFragmentList
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyFragmentList.h>
	#include "sipBALLFragment.h"
  #include <BALL/PYTHON/pyBALLSipHelper.h>
%End

%ConvertFromTypeCode
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
		return NULL;

	// Convert the list.

	for (PyFragmentList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		Fragment& obj = **it;
		PyObject *inst;

		if ((inst = pyMapBALLObjectToSip(obj)) == NULL || PyList_Append(pl,inst) < 0)
		{
			Py_DECREF(pl);
			return NULL;
		}
	}

	return pl;
%End
 
%ConvertToTypeCode
	// Convert a Python list of Fragment instances to an FragmentList object on the
	// heap.
 
	PyFragmentList *fragment_list = new PyFragmentList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		Fragment* fragment;
 
		fragment = reinterpret_cast<Fragment*>(sipForceConvertTo_Fragment(PyList_GET_ITEM(sipPy,i),sipIsErr));
 
		if (*sipIsErr)
		{
			delete fragment_list;
			return 0;
		}
 
		fragment_list -> push_back(fragment);
	}
 
	*sipCppPtr = fragment_list;

	return 1;
%End
};

