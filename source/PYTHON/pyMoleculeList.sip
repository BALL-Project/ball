
class PyMoleculeList
{
%HeaderCode
#include <BALL/PYTHON/pyMoleculeList.h>
#include "sipBALLMolecule.h"
%End

%ConvertFromClassCode
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyMoleculeList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		Molecule* obj = *it;
		PyObject *inst;

		if ((inst = sipMapCppToSelf(obj,sipClass_Molecule)) == NULL || PyList_Append(pl,inst) < 0)
		{
			Py_DECREF(pl);
			return NULL;
		}
	}

	return pl;
%End
 
%CanConvertToClassCode
	return PyList_Check(sipPy);
%End
 
%ConvertToClassCode
	// Convert a Python list of Molecule instances to an MoleculeList object on the
	// heap.
 
	PyMoleculeList *molecule_list = new PyMoleculeList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		Molecule* molecule;
 
		molecule = sipForceConvertTo_Molecule(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
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

