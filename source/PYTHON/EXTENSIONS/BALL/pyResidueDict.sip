// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

%MappedType PyResidueDict
{
%TypeHeaderCode
	#include <BALL/DATATYPE/hashMap.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
	#include <BALL/KERNEL/residue.h>
	typedef HashMap<const Residue*, float> PyResidueDict;
%End

%ConvertFromTypeCode
	PyObject* dict = PyDict_New();

	if (dict == NULL)
	{
		return NULL;
	}

	// Convert the hash map to a dictionary.

	for (PyResidueDict::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		PyObject* key = pyMapBALLObjectToSip(const_cast<Residue&>(*(it->first)));
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
	// Convert a Python dictionary to a PyResidueDict object on the
	// heap.
 
	// create a list of all keys
	PyObject* key_list = PyDict_Keys(sipPy);
	if (key_list == NULL)
	{
		return 0;
	}
		
	PyResidueDict* residue_dict = new PyResidueDict;
 
	for (int i = 0; i < PyList_GET_SIZE(key_list); ++i)
	{
		PyObject* key = PyList_GET_ITEM(key_list,i);
		if (key == NULL)
		{
			break;
		}
		Residue* residue = reinterpret_cast<Residue*>(sipForceConvertTo_Residue(key,sipIsErr));
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
		residue_dict -> insert(std::pair<Residue*,float>(residue, value));
	}
 
	*sipCppPtr = residue_dict;

	return 1;
%End
};

