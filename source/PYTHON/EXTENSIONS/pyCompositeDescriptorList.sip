// $Id: pyCompositeDescriptorList.sip,v 1.1 2001/09/03 16:53:12 oliver Exp $

%MappedType PyCompositeDescriptorList
{
%HeaderCode
	#include <BALL/DATATYPE/list.h>
	#include <BALL/VIEW/GUI/KERNEL/compositeDescriptor.h>
	#include "sipBALLCompositeDescriptor.h"
  #include <BALL/PYTHON/pyBALLSipHelper.h>
	typedef List<CompositeDescriptor*> PyCompositeDescriptorList;
%End

%ConvertFromTypeCode
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyCompositeDescriptorList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		CompositeDescriptor* obj = *it;
		PyObject *inst;

		if ((inst = sipMapCppToSelf(obj,sipClass_CompositeDescriptor)) == NULL || PyList_Append(pl,inst) < 0)
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
	// Convert a Python list of CompositeDescriptor instances to an CompositeDescriptorList object on the
	// heap.
 
	PyCompositeDescriptorList *atom_list = new PyCompositeDescriptorList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		CompositeDescriptor* atom;
 
		atom = sipForceConvertTo_CompositeDescriptor(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete atom_list;
			return 0;
		}
 
		atom_list -> push_back(atom);
	}
 
	*sipCppPtr = atom_list;

	return 1;
%End
};

