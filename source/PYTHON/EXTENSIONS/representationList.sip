// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: representationList.sip,v 1.1 2003/12/18 19:03:22 oliver Exp $
//

%MappedType RepresentationList
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/primitiveManager.h>
	#include "sipBALLRepresentation.h"
  #include <BALL/PYTHON/pyBALLSipHelper.h>	
	using namespace BALL;
	using namespace BALL::VIEW;
	typedef PrimitiveManager::RepresentationList RepresentationList;

%End

%ConvertFromTypeCode
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (RepresentationList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		Representation* obj = *it;

		if (PyList_Append(pl, sipMapCppToSelf(obj, sipClass_Representation)) < 0)
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
 
	RepresentationList *representation_list = new RepresentationList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		Representation* representation;
 
		representation = reinterpret_cast<Representation*>(sipForceConvertTo_Representation(PyList_GET_ITEM(sipPy,i),sipIsErr));
 
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

