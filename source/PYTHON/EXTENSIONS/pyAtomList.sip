// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyAtomList.sip,v 1.5.6.1 2005/11/16 20:01:24 amoll Exp $
//

%MappedType PyAtomList
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyAtomList.h>
	#include "sipBALLAtom.h"
  #include <BALL/PYTHON/pyBALLSipHelper.h>
%End
%ConvertFromTypeCode
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
%End
 
%ConvertToTypeCode
	// Convert a Python list of Atom instances to an AtomList object on the
	// heap.
 
	if (sipIsErr == NULL)
	{
		return PyList_Check(sipPy);
	}

	PyAtomList *atom_list = new PyAtomList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		Atom* atom;
 
		atom = reinterpret_cast<Atom*>(sipForceConvertTo_Atom(PyList_GET_ITEM(sipPy,i),sipIsErr));
 
		if (*sipIsErr)
		{
			delete atom_list;
			return 0;
		}
 
		atom_list -> push_back(atom);
	}
 
	*sipCppPtr = atom_list;

	return 1;
%End
};

