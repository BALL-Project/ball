// $Id: pyBaseFragmentList.sip,v 1.1 2000/07/18 21:28:05 oliver Exp $

class PyBaseFragmentList
{
%HeaderCode
	#include <BALL/PYTHON/pyBaseFragmentList.h>
	#include "sipBALLBaseFragment.h"
	#include <BALL/PYTHON/pyBALLSipHelper.h>
%End

%ConvertFromClassCode
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyBaseFragmentList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		BaseFragment& obj = **it;
		PyObject *inst;

		if ((inst = pyMapBALLObjectToSip(obj)) == NULL || PyList_Append(pl,inst) < 0)
		{
			Py_DECREF(pl);
			return NULL;
		}
	}

	return pl;
%End
 
%CanConvertToClassCode
	return PyList_Check(sipPy);
%End
 
%ConvertToClassCode
	// Convert a Python list of BaseFragment instances to an BaseFragmentList object on the
	// heap.
 
	PyBaseFragmentList *base_fragment_list = new PyBaseFragmentList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		BaseFragment* base_fragment;
 
		base_fragment = sipForceConvertTo_BaseFragment(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete base_fragment_list;
			return 0;
		}
 
		base_fragment_list -> push_back(base_fragment);
	}
 
	*sipCppPtr = base_fragment_list;

	return 1;
%End
};

