// $Id: pySecondaryStructureList.sip,v 1.2 2001/12/22 16:39:16 oliver Exp $

%MappedType PySecondaryStructureList
{
%HeaderCode
	#include <BALL/PYTHON/pySecondaryStructureList.h>
	#include "sipBALLSecondaryStructure.h"
  #include <BALL/PYTHON/pyBALLSipHelper.h>
%End

%ConvertFromTypeCode
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PySecondaryStructureList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		SecondaryStructure& obj = **it;
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
	// Convert a Python list of SecondaryStructure instances to an SecondaryStructureList object on the
	// heap.
 
	PySecondaryStructureList *sec_struc_list = new PySecondaryStructureList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		SecondaryStructure* sec_struc;
 
		sec_struc = sipForceConvertTo_SecondaryStructure(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete sec_struc_list;
			return 0;
		}
 
		sec_struc_list -> push_back(sec_struc);
	}
 
	*sipCppPtr = sec_struc_list;

	return 1;
%End
};

