// $Id: pyChainList.sip,v 1.2 2001/07/29 17:31:17 oliver Exp $

%MappedType PyChainList
{
%HeaderCode
	#include <BALL/PYTHON/pyChainList.h>
	#include "sipBALLChain.h"
  #include <BALL/PYTHON/pyBALLSipHelper.h>
%End

%ConvertFromTypeCode
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyChainList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		Chain& obj = **it;
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
	// Convert a Python list of Chain instances to an ChainList object on the
	// heap.
 
	PyChainList *chain_list = new PyChainList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		Chain* chain;
 
		chain = sipForceConvertTo_Chain(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete chain_list;
			return 0;
		}
 
		chain_list -> push_back(chain);
	}
 
	*sipCppPtr = chain_list;

	return 1;
%End
};

