// $Id: pyPDBAtomList.sip,v 1.2 2001/07/29 17:31:17 oliver Exp $

%MappedType PyPDBAtomList
{
%HeaderCode
	#include <BALL/PYTHON/pyPDBAtomList.h>
	#include "sipBALLPDBAtom.h"
  #include <BALL/PYTHON/pyBALLSipHelper.h>
%End

%ConvertFromTypeCode
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyPDBAtomList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		PDBAtom& obj = **it;
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
	// Convert a Python list of PDBAtom instances to an PDBAtomList object on the
	// heap.
 
	PyPDBAtomList *PDB_atom_list = new PyPDBAtomList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		PDBAtom* PDB_atom;
 
		PDB_atom = sipForceConvertTo_PDBAtom(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete PDB_atom_list;
			return 0;
		}
 
		PDB_atom_list -> push_back(PDB_atom);
	}
 
	*sipCppPtr = PDB_atom_list;

	return 1;
%End
};

