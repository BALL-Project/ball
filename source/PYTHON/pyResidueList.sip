
class PyResidueList
{
%HeaderCode
#include <BALL/PYTHON/pyResidueList.h>
#include "sipBALLResidue.h"
%End

%ConvertFromClassCode
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyResidueList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		Residue* obj = *it;
		PyObject *inst;

		if ((inst = sipMapCppToSelf(obj,sipClass_Residue)) == NULL || PyList_Append(pl,inst) < 0)
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
	// Convert a Python list of Residue instances to an ResidueList object on the
	// heap.
 
	PyResidueList *residue_list = new PyResidueList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		Residue* residue;
 
		residue = sipForceConvertTo_Residue(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete residue_list;
			return 0;
		}
 
		residue_list -> push_back(residue);
	}
 
	*sipCppPtr = residue_list;

	return 1;
%End
};

