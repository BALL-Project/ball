#include "sipBALLDeclBALL.h"
#include "sipBALLPyBaseFragmentList.h"



PyObject *sipConvertFrom_PyBaseFragmentList(const PyBaseFragmentList *sipCpp)
{
	if (sipCpp == NULL)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}

#line 12 "pyBaseFragmentList.sip"
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
#line 42 "../CPP/sipBALLPyBaseFragmentList.cpp"
}

PyObject *sipClass_PyBaseFragmentList;

int sipCanConvertTo_PyBaseFragmentList(PyObject *sipPy)
{
#line 37 "pyBaseFragmentList.sip"
	return PyList_Check(sipPy);
#line 51 "../CPP/sipBALLPyBaseFragmentList.cpp"
}

int sipConvertTo_PyBaseFragmentList(PyObject *sipPy,PyBaseFragmentList **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return false;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_PyBaseFragmentList);
		else
			*sipCppPtr = NULL;

		return false;
	}

#line 41 "pyBaseFragmentList.sip"
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
#line 93 "../CPP/sipBALLPyBaseFragmentList.cpp"
}

PyBaseFragmentList *sipForceConvertTo_PyBaseFragmentList(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_PyBaseFragmentList(valobj))
	{
		PyBaseFragmentList *val;

		sipConvertTo_PyBaseFragmentList(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_PyBaseFragmentList);
	*iserrp = 1;

	return NULL;
}
