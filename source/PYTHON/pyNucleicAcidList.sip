// $Id: pyNucleicAcidList.sip,v 1.2 2000/07/03 11:11:11 oliver Exp $

class PyNucleicAcidList
{
%HeaderCode
	#include <BALL/PYTHON/pyNucleicAcidList.h>
	#include "sipBALLNucleicAcid.h"
  #include <BALL/PYTHON/pyBALLSipHelper.h>
%End

%ConvertFromClassCode
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyNucleicAcidList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		NucleicAcid& obj = **it;
		PyObject *inst;

		if ((inst = pyMapBALLObjectToSip(obj)) == NULL || PyList_Append(pl,inst) < 0)
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
	// Convert a Python list of NucleicAcid instances to an NucleicAcidList object on the
	// heap.
 
	PyNucleicAcidList *nucleic_acid_list = new PyNucleicAcidList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		NucleicAcid* nucleic_acid;
 
		nucleic_acid = sipForceConvertTo_NucleicAcid(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete nucleic_acid_list;
			return 0;
		}
 
		nucleic_acid_list -> push_back(nucleic_acid);
	}
 
	*sipCppPtr = nucleic_acid_list;

	return 1;
%End
};

