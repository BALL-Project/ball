
class PyFragmentList
{
%HeaderCode
#include <BALL/PYTHON/pyFragmentList.h>
#include "sipBALLFragment.h"
%End

%ConvertFromClassCode
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
		return NULL;

	// Convert the list.

	for (PyFragmentList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		Fragment* obj = *it;
		PyObject *inst;

		if ((inst = sipMapCppToSelf(obj,sipClass_Fragment)) == NULL || PyList_Append(pl,inst) < 0)
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
	// Convert a Python list of Fragment instances to an FragmentList object on the
	// heap.
 
	PyFragmentList *fragment_list = new PyFragmentList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		Fragment* fragment;
 
		fragment = sipForceConvertTo_Fragment(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
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

