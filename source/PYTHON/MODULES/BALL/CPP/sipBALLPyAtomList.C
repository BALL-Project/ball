#include "sipBALLDeclBALL.h"
#include "sipBALLPyAtomList.h"



PyObject *sipConvertFrom_PyAtomList(const PyAtomList *sipCpp)
{
	if (sipCpp == NULL)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}

#line 15 "pyAtomList.sip"
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
#line 42 "sipBALLPyAtomList.cpp"
}

PyObject *sipClass_PyAtomList;

int sipCanConvertTo_PyAtomList(PyObject *sipPy)
{
#line 40 "pyAtomList.sip"
	return PyList_Check(sipPy);
#line 51 "sipBALLPyAtomList.cpp"
}

int sipConvertTo_PyAtomList(PyObject *sipPy,PyAtomList **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return 0;

#line 44 "pyAtomList.sip"
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
#line 83 "sipBALLPyAtomList.cpp"
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
