#include "sipBALLDeclBALL.h"
#include "sipBALLPyAtomDict.h"



PyObject *sipConvertFrom_PyAtomDict(const PyAtomDict *sipCpp)
{
	if (sipCpp == NULL)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}

#line 13 "pyAtomDict.sip"
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
#line 43 "../CPP/sipBALLPyAtomDict.cpp"
}

PyObject *sipClass_PyAtomDict;

int sipCanConvertTo_PyAtomDict(PyObject *sipPy)
{
#line 39 "pyAtomDict.sip"
	cerr << "can convert to" << endl;
	return PyDict_Check(sipPy);
#line 53 "../CPP/sipBALLPyAtomDict.cpp"
}

int sipConvertTo_PyAtomDict(PyObject *sipPy,PyAtomDict **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return false;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_PyAtomDict);
		else
			*sipCppPtr = NULL;

		return false;
	}

#line 44 "pyAtomDict.sip"
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
#line 114 "../CPP/sipBALLPyAtomDict.cpp"
}

PyAtomDict *sipForceConvertTo_PyAtomDict(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_PyAtomDict(valobj))
	{
		PyAtomDict *val;

		sipConvertTo_PyAtomDict(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_PyAtomDict);
	*iserrp = 1;

	return NULL;
}
