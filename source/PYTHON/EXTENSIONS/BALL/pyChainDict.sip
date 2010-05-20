// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

%MappedType PyChainDict
{
%TypeHeaderCode
	#include <BALL/DATATYPE/hashMap.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
	#include <BALL/KERNEL/chain.h>
	typedef HashMap<const Chain*, float> PyChainDict;
%End

%ConvertFromTypeCode
	PyObject* dict = PyDict_New();

	if (dict == NULL)
	{
		return NULL;
	}

	// Convert the hash map to a dictionary.

	for (PyChainDict::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		PyObject* key = pyMapBALLObjectToSip(const_cast<Chain&>(*(it->first)));
		PyObject* value = PyFloat_FromDouble(it->second);

		if ((key == NULL) || (value == NULL) || PyDict_SetItem(dict,key,value) < 0)
		{
			Py_DECREF(dict);
			return NULL;
		}
	}

	return dict;
%End
 
%ConvertToTypeCode
	// Convert a Python dictionary to a PyChainDict object on the
	// heap.
 
	// create a list of all keys
	PyObject* key_list = PyDict_Keys(sipPy);
	if (key_list == NULL)
	{
		return 0;
	}
		
	PyChainDict* chain_dict = new PyChainDict;
 
	for (int i = 0; i < PyList_GET_SIZE(key_list); ++i)
	{
		PyObject* key = PyList_GET_ITEM(key_list,i);
		if (key == NULL)
		{
			break;
		}
		Chain* chain = reinterpret_cast<Chain*>(sipForceConvertTo_Chain(key,sipIsErr));
		if (*sipIsErr)
		{
			break;
		}
		PyObject* float_obj = PyDict_GetItem(sipPy,key);
		if (!PyFloat_Check(float_obj))
		{
			break;
		}
		double value = PyFloat_AsDouble(float_obj);
		chain_dict -> insert(std::pair<Chain*,float>(chain, value));
	}
 
	*sipCppPtr = chain_dict;

	return 1;
%End
};

