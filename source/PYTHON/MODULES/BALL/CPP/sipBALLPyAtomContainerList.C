#include "sipBALLDeclBALL.h"
#include "sipBALLPyAtomContainerList.h"


int sipCanConvertTo_PyAtomContainerList(PyObject *sipPy)
{
#line 37 "pyAtomContainerList.sip"
	return PyList_Check(sipPy);
#line 14 "sipBALLPyAtomContainerList.cpp"
}

int sipConvertTo_PyAtomContainerList(PyObject *sipPy,PyAtomContainerList **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return 0;

#line 41 "pyAtomContainerList.sip"
	// Convert a Python list of AtomContainer instances to an AtomContainerList object on the
	// heap.
 
	PyAtomContainerList *atom_container_list = new PyAtomContainerList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		AtomContainer* atom_container;
 
		atom_container = sipForceConvertTo_AtomContainer(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete atom_container_list;
			return 0;
		}
 
		atom_container_list -> push_back(atom_container);
	}
 
	*sipCppPtr = atom_container_list;

	return 1;
#line 46 "sipBALLPyAtomContainerList.cpp"
}

PyAtomContainerList *sipForceConvertTo_PyAtomContainerList(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_PyAtomContainerList(valobj))
	{
		PyAtomContainerList *val;

		sipConvertTo_PyAtomContainerList(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_PyAtomContainerList);

	*iserrp = 1;

	return NULL;
}

PyObject *sipConvertFrom_PyAtomContainerList(const PyAtomContainerList *sipCpp)
{
#line 12 "pyAtomContainerList.sip"
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyAtomContainerList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		AtomContainer& obj = **it;
		PyObject *inst;

		if ((inst = pyMapBALLObjectToSip(obj)) == NULL || PyList_Append(pl,inst) < 0)
		{
			Py_DECREF(pl);
			return NULL;
		}
	}

	return pl;
#line 95 "sipBALLPyAtomContainerList.cpp"
}
