#include "sipBALLDeclBALL.h"
#include "sipBALLPyCompositeDescriptorList.h"



PyObject *sipConvertFrom_PyCompositeDescriptorList(const PyCompositeDescriptorList *sipCpp)
{
	if (sipCpp == NULL)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}

#line 14 "pyCompositeDescriptorList.sip"
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyCompositeDescriptorList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		CompositeDescriptor* obj = *it;
		PyObject *inst;

		if ((inst = sipMapCppToSelf(obj,sipClass_CompositeDescriptor)) == NULL || PyList_Append(pl,inst) < 0)
		{
			Py_DECREF(pl);
			return NULL;
		}
	}

	return pl;
#line 42 "./sipBALLPyCompositeDescriptorList.cpp"
}

PyObject *sipClass_PyCompositeDescriptorList;

int sipCanConvertTo_PyCompositeDescriptorList(PyObject *sipPy)
{
#line 39 "pyCompositeDescriptorList.sip"
	return PyList_Check(sipPy);
#line 51 "./sipBALLPyCompositeDescriptorList.cpp"
}

int sipConvertTo_PyCompositeDescriptorList(PyObject *sipPy,PyCompositeDescriptorList **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return false;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_PyCompositeDescriptorList);
		else
			*sipCppPtr = NULL;

		return false;
	}

#line 43 "pyCompositeDescriptorList.sip"
	// Convert a Python list of CompositeDescriptor instances to an CompositeDescriptorList object on the
	// heap.
 
	PyCompositeDescriptorList *atom_list = new PyCompositeDescriptorList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		CompositeDescriptor* atom;
 
		atom = sipForceConvertTo_CompositeDescriptor(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete atom_list;
			return 0;
		}
 
		atom_list -> push_back(atom);
	}
 
	*sipCppPtr = atom_list;

	return 1;
#line 93 "./sipBALLPyCompositeDescriptorList.cpp"
}

PyCompositeDescriptorList *sipForceConvertTo_PyCompositeDescriptorList(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_PyCompositeDescriptorList(valobj))
	{
		PyCompositeDescriptorList *val;

		sipConvertTo_PyCompositeDescriptorList(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_PyCompositeDescriptorList);
	*iserrp = 1;

	return NULL;
}
