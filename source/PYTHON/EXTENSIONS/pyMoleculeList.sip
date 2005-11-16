// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyMoleculeList.sip,v 1.5.6.1 2005/11/16 20:01:25 amoll Exp $
//

%MappedType PyMoleculeList
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyMoleculeList.h>
	#include "sipBALLMolecule.h"
  #include <BALL/PYTHON/pyBALLSipHelper.h>
%End

%ConvertFromTypeCode
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyMoleculeList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		Molecule& obj = **it;
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
	// Convert a Python list of Molecule instances to an MoleculeList object on the
	// heap.
 
	if (sipIsErr == NULL)
	{
		return PyList_Check(sipPy);
	}
 
	PyMoleculeList *molecule_list = new PyMoleculeList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		Molecule* molecule;
 
		molecule = reinterpret_cast<Molecule*>(sipForceConvertTo_Molecule(PyList_GET_ITEM(sipPy,i),sipIsErr));
 
		if (*sipIsErr)
		{
			delete molecule_list;
			return 0;
		}
 
		molecule_list -> push_back(molecule);
	}
 
	*sipCppPtr = molecule_list;

	return 1;
%End
};

