// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

%MappedType RepresentationList
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/representationManager.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>	
	using namespace BALL;
	using namespace BALL::VIEW;
	typedef RepresentationManager::RepresentationList RepresentationList;

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

		if (PyList_Append(pl, sipConvertFromType(obj, sipType_Representation, 0)) < 0)
		{
			Py_DECREF(pl);
			return NULL;
		}
	}

	return pl;
%End
 
%ConvertToTypeCode
  if (sipIsErr == NULL)
    return (PyList_Check(sipPy));

	// Convert a Python list of Representation instances to an RepresentationList object on the
	// heap.
 
	RepresentationList *representation_list = new RepresentationList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		Representation* representation;
 
		representation = reinterpret_cast<Representation*>(sipForceConvertToType(PyList_GET_ITEM(sipPy,i), sipType_Representation, NULL, SIP_NO_CONVERTORS, NULL, sipIsErr));
 
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

