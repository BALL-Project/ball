// $Id: pyAtomContainerList.sip,v 1.2 2001/12/22 16:39:15 oliver Exp $

%MappedType PyAtomContainerList
{
%HeaderCode
	#include <BALL/PYTHON/pyAtomContainerList.h>
	#include "sipBALLAtomContainer.h"
	#include <BALL/PYTHON/pyBALLSipHelper.h>
%End

%ConvertFromTypeCode
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyAtomContainerList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		AtomContainer& obj = **it;
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
	// Convert a Python list of AtomContainer instances to an AtomContainerList object on the
	// heap.
 
	PyAtomContainerList *atom_container_list = new PyAtomContainerList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		AtomContainer* atom_container;
 
		atom_container = sipForceConvertTo_AtomContainer(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete atom_container_list;
			return 0;
		}
 
		atom_container_list -> push_back(atom_container);
	}
 
	*sipCppPtr = atom_container_list;

	return 1;
%End
};

