// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

%MappedType PyAtomDict
{
%TypeHeaderCode
	#include <BALL/DATATYPE/hashMap.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
	#include <BALL/KERNEL/atom.h>
	using namespace BALL;
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
 
%ConvertToTypeCode
	// Convert a Python dictionary to a PyAtomDict object on the
	// heap.
 
	// create a list of all keys
	PyObject* key_list = PyDict_Keys(sipPy);
	if (key_list == NULL)
	{
		return 0;
	}
		
	PyAtomDict* atom_dict = new PyAtomDict;
 
	for (int i = 0; i < PyList_GET_SIZE(key_list); ++i)
	{
		PyObject* key = PyList_GET_ITEM(key_list,i);
		if (key == NULL)
		{
			break;
		}
		Atom* atom = reinterpret_cast<Atom*>(BALL_FORCE_CONVERT_TO_TYPE(key, Atom));
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
		atom_dict -> insert(std::pair<Atom*,float>(atom, value));
	}
 
	*sipCppPtr = atom_dict;

	return 1;
%End
};

