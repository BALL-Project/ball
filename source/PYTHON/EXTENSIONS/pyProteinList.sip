// $Id: pyProteinList.sip,v 1.1 2001/09/03 16:53:14 oliver Exp $

%MappedType PyProteinList
{
%HeaderCode
	#include <BALL/PYTHON/pyProteinList.h>
	#include "sipBALLProtein.h"
  #include <BALL/PYTHON/pyBALLSipHelper.h>
%End

%ConvertFromTypeCode
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyProteinList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		Protein& obj = **it;
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
	// Convert a Python list of Protein instances to an ProteinList object on the
	// heap.
 
	PyProteinList *protein_list = new PyProteinList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		Protein* protein;
 
		protein = sipForceConvertTo_Protein(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete protein_list;
			return 0;
		}
 
		protein_list -> push_back(protein);
	}
 
	*sipCppPtr = protein_list;

	return 1;
%End
};

