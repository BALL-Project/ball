// $Id: pyAtomList.sip,v 1.4 2001/08/16 00:30:48 oliver Exp $

%MappedType PyAtomList
{
%HeaderCode
	#include <BALL/PYTHON/pyAtomList.h>
	#include "sipBALLAtom.h"
  #include <BALL/PYTHON/pyBALLSipHelper.h>
%End
%ConvertFromTypeCode
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
%End
 
%CanConvertToTypeCode
	std::cerr << "PyAtomList::CanConvertTo = " << PyList_Check(sipPy) << std::endl;
	return PyList_Check(sipPy);
%End
 
%ConvertToTypeCode
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
%End
};

