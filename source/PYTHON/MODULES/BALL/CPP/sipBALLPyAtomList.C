#include "sipBALLDeclBALL.h"
#include "sipBALLPyAtomList.h"


int sipCanConvertTo_PyAtomList(PyObject *sipPy)
{
#line 40 "pyAtomList.sip"
	std::cerr << "PyAtomList::CanConvertTo = " << PyList_Check(sipPy) << std::endl;
	return PyList_Check(sipPy);
#line 15 "sipBALLPyAtomList.cpp"
}

int sipConvertTo_PyAtomList(PyObject *sipPy,PyAtomList **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return 0;

#line 45 "pyAtomList.sip"
	// Convert a Python list of Atom instances to an AtomList object on the
	// heap.
 
	std::cerr << "PyAtomList::ConvertTo "  << std::endl;
	PyAtomList *atom_list = new PyAtomList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		Atom* atom;
 
		atom = sipForceConvertTo_Atom(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete atom_list;
			std::cerr << "PyAtomList::ConvertTo --  sipIsErr"  << std::endl;
			return 0;
		}
 
		atom_list -> push_back(atom);
	}
 
	std::cerr << "PyAtomList::ConvertTo --  SUCCESS"  << std::endl;
	*sipCppPtr = atom_list;

	return 1;
#line 50 "sipBALLPyAtomList.cpp"
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

PyObject *sipConvertFrom_PyAtomList(const PyAtomList *sipCpp)
{
#line 11 "pyAtomList.sip"
	PyObject *pl;

	std::cerr << "PyAtomList:: ConvertFrom:" << std::endl;
	if ((pl = PyList_New(0)) == NULL)
	{
		std::cerr << "PyAtomList:: pl == NULL" << std::endl;
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
			std::cerr << "PyAtomList:: pyMap == NULL" << std::endl;
			return NULL;
		}
	}

	std::cerr << "PyAtomList:: ConvertFrom -- SUCCESS" << std::endl;
	return pl;
#line 103 "sipBALLPyAtomList.cpp"
}
