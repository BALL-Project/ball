#include "sipBALLDeclBALL.h"
#include "sipBALLPyBondList.h"


int sipCanConvertTo_PyBondList(PyObject *sipPy)
{
#line 37 "pyBondList.sip"
	return PyList_Check(sipPy);
#line 14 "sipBALLPyBondList.cpp"
}

int sipConvertTo_PyBondList(PyObject *sipPy,PyBondList **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return 0;

#line 41 "pyBondList.sip"
	// Convert a Python list of Bond instances to an BondList object on the
	// heap.
 
	PyBondList *bond_list = new PyBondList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		Bond* bond;
 
		bond = sipForceConvertTo_Bond(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete bond_list;
			return 0;
		}
 
		bond_list -> push_back(bond);
	}
 
	*sipCppPtr = bond_list;

	return 1;
#line 46 "sipBALLPyBondList.cpp"
}

PyBondList *sipForceConvertTo_PyBondList(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_PyBondList(valobj))
	{
		PyBondList *val;

		sipConvertTo_PyBondList(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_PyBondList);

	*iserrp = 1;

	return NULL;
}

PyObject *sipConvertFrom_PyBondList(const PyBondList *sipCpp)
{
#line 12 "pyBondList.sip"
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyBondList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		Bond& obj = **it;
		PyObject *inst;

		if ((inst = pyMapBALLObjectToSip(obj)) == NULL || PyList_Append(pl,inst) < 0)
		{
			Py_DECREF(pl);
			return NULL;
		}
	}

	return pl;
#line 95 "sipBALLPyBondList.cpp"
}
