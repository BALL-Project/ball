// $Id: pyAtomDict.sip,v 1.1 2000/07/18 21:28:02 oliver Exp $

class PyAtomDict
{
%HeaderCode
	#include <BALL/DATATYPE/hashMap.h>
	#include "sipBALLAtom.h"
  #include <BALL/PYTHON/pyBALLSipHelper.h>
	typedef HashMap<Atom*, float> PyAtomDict;
%End

%ConvertFromClassCode
	cerr << "convert from" << endl;
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
 
%CanConvertToClassCode
	cerr << "can convert to" << endl;
	return PyDict_Check(sipPy);
%End
 
%ConvertToClassCode
	// Convert a Python dictionary to a PyAtomDict object on the
	// heap.
 
	cerr << "convert-to" << endl;
	// create a list of all keys
	PyObject* key_list = PyDict_Keys(sipPy);
	if (key_list == NULL)
	{
		cerr << "cannot create key list" << endl;
		return 0;
	}
		
	PyAtomDict* atom_dict = new PyAtomDict;
 
	for (int i = 0; i < PyList_GET_SIZE(key_list); ++i)
	{
		PyObject* key = PyList_GET_ITEM(key_list,i);
		if (key == NULL)
		{
			cerr << "cannot get item" << endl;
			break;
		}
		Atom* atom = sipForceConvertTo_Atom(key,sipIsErr);
		if (*sipIsErr)
		{
			cerr << "cannot convert to atom" << endl;
			break;
		}
		PyObject* float_obj = PyDict_GetItem(sipPy,key);
		if (!PyFloat_Check(float_obj))
		{
			cerr << "cannot get dict item" << endl;
			break;
		}
		double value = PyFloat_AsDouble(float_obj);
		atom_dict -> insert(pair<Atom*,float>(atom, value));
		cerr << "converted 1 item" << endl;
	}
 
	*sipCppPtr = atom_dict;

	return 1;
%End
};

