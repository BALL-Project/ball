// $Id: pyAtomDict.sip,v 1.4 2001/08/16 00:30:48 oliver Exp $

%MappedType PyAtomDict
{
%HeaderCode
	#include <BALL/DATATYPE/hashMap.h>
	#include "sipBALLAtom.h"
  #include <BALL/PYTHON/pyBALLSipHelper.h>
	typedef HashMap<const Atom*, float> PyAtomDict;
%End

%ConvertFromTypeCode
	PyObject* dict = PyDict_New();

	if (dict == NULL)
	{
		return NULL;
	}

	// Convert the hash map to a dictionary.

	for (PyAtomDict::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		PyObject* key = pyMapBALLObjectToSip(const_cast<Atom&>(*(it->first)));
		PyObject* value = PyFloat_FromDouble(it->second);

		if ((key == NULL) || (value == NULL) || PyDict_SetItem(dict,key,value) < 0)
		{
			Py_DECREF(dict);
			return NULL;
		}
	}

	return dict;
%End
 
%CanConvertToTypeCode
	std::cerr << "can convert to" << std::endl;
	return PyDict_Check(sipPy);
%End
 
%ConvertToTypeCode
	// Convert a Python dictionary to a PyAtomDict object on the
	// heap.
 
	// create a list of all keys
	PyObject* key_list = PyDict_Keys(sipPy);
	if (key_list == NULL)
	{
		std::cerr << "cannot create key list" << std::endl;
		return 0;
	}
		
	PyAtomDict* atom_dict = new PyAtomDict;
 
	for (int i = 0; i < PyList_GET_SIZE(key_list); ++i)
	{
		PyObject* key = PyList_GET_ITEM(key_list,i);
		if (key == NULL)
		{
			std::cerr << "cannot get item" << std::endl;
			break;
		}
		Atom* atom = sipForceConvertTo_Atom(key,sipIsErr);
		if (*sipIsErr)
		{
			std::cerr << "cannot convert to atom" << std::endl;
			break;
		}
		PyObject* float_obj = PyDict_GetItem(sipPy,key);
		if (!PyFloat_Check(float_obj))
		{
			std::cerr << "cannot get dict item" << std::endl;
			break;
		}
		double value = PyFloat_AsDouble(float_obj);
		atom_dict -> insert(pair<Atom*,float>(atom, value));
		std::cerr << "converted 1 item" << std::endl;
	}
 
	*sipCppPtr = atom_dict;

	return 1;
%End
};

