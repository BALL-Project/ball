// $Id: pyBondList.sip,v 1.2 2001/07/29 17:31:17 oliver Exp $

%MappedType PyBondList
{
%HeaderCode
	#include <BALL/PYTHON/pyBondList.h>
	#include "sipBALLBond.h"
  #include <BALL/PYTHON/pyBALLSipHelper.h>
%End

%ConvertFromTypeCode
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
%End
 
%CanConvertToTypeCode
	return PyList_Check(sipPy);
%End
 
%ConvertToTypeCode
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
%End
};

