#include "sipBALLDeclBALL.h"
#include "sipBALLPyAtomList.h"



PyObject *sipConvertFrom_PyAtomList(const PyAtomList *sipCpp)
{
	if (sipCpp == NULL)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}

#line 12 "pyAtomList.sip"
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyAtomList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		Atom& obj = **it;
		PyObject *inst;

		if ((inst = pyMapBALLObjectToSip(obj)) == NULL || PyList_Append(pl,inst) < 0)
		{
			Py_DECREF(pl);
			return NULL;
		}
	}

	return pl;
#line 42 "./sipBALLPyAtomList.cpp"
}

PyObject *sipClass_PyAtomList;

int sipCanConvertTo_PyAtomList(PyObject *sipPy)
{
#line 37 "pyAtomList.sip"
	return PyList_Check(sipPy);
#line 51 "./sipBALLPyAtomList.cpp"
}

int sipConvertTo_PyAtomList(PyObject *sipPy,PyAtomList **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return false;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_PyAtomList);
		else
			*sipCppPtr = NULL;

		return false;
	}

#line 41 "pyAtomList.sip"
	// Convert a Python list of Atom instances to an AtomList object on the
	// heap.
 
	PyAtomList *atom_list = new PyAtomList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		Atom* atom;
 
		atom = sipForceConvertTo_Atom(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete atom_list;
			return 0;
		}
 
		atom_list -> push_back(atom);
	}
 
	*sipCppPtr = atom_list;

	return 1;
#line 93 "./sipBALLPyAtomList.cpp"
}

PyAtomList *sipForceConvertTo_PyAtomList(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_PyAtomList(valobj))
	{
		PyAtomList *val;

		sipConvertTo_PyAtomList(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_PyAtomList);
	*iserrp = 1;

	return NULL;
}
