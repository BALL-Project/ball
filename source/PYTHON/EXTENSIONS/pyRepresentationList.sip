// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyRepresentationList.sip,v 1.1 2003/11/15 10:59:07 oliver Exp $
//

%MappedType PyRepresentationList
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyRepresentationList.h>
	#include "sipBALLRepresentation.h"
  #include <BALL/PYTHON/pyBALLSipHelper.h>
%End

%ConvertFromTypeCode
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyRepresentationList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		Representation& obj = **it;
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
	// Convert a Python list of Representation instances to an RepresentationList object on the
	// heap.
 
	PyRepresentationList *representation_list = new PyRepresentationList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		Representation* representation;
 
		representation = sipForceConvertTo_Representation(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete representation_list;
			return 0;
		}
 
		representation_list -> push_back(representation);
	}
 
	*sipCppPtr = representation_list;

	return 1;
%End
};

