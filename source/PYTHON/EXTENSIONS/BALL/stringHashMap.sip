// based on vector.sip by Giovanni Bajo

template <ValueType>
%MappedType StringHashMap<ValueType>
{
%TypeHeaderCode
#include <BALL/DATATYPE/stringHashMap.h>
#include <BALL/PYTHON/pyBALLSipHelper.h>
%End

%ConvertFromTypeCode
	PyObject *dict;

	// create the python dictionary of the correct size
	if ((dict = PyDict_New()) == NULL)
		return NULL;

	// convert the hash map to a dictionary
	for (StringHashMap<ValueType>::Iterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		// NOTE: keys into a string hash map are const!
		PyObject* key = sipConvertFromType(const_cast<String*>(&(it->first)), sipType_String, sipTransferObj);
		if (key == NULL)
		{
			Py_DECREF(dict);
			return NULL;
		}

		ValueType *cpp   = new ValueType(it->second);
		PyObject *pobj;

		// get the Python wrapper for the Type instance, creating a new one
		// if necessary, and handle any ownership transfer.
		if ((pobj = sipConvertFromType(cpp, sipType_ValueType, sipTransferObj)) == NULL)
		{
			// There was an error so garbage collect the python dict.
			Py_DECREF(dict);
			return NULL;
		}

		// insert into the dictionary
		if (PyDict_SetItem(dict, key, pobj) < 0)
		{
			Py_DECREF(dict);
			return NULL;
		}
	}

	return dict;
%End

%ConvertToTypeCode
	// Convert a Python dictionary to an object on the heap.
	
	// create a list of all keys
	PyObject* key_list = PyDict_Keys(sipPy);
	if (key_list == NULL)
	{
		return 0;
	}

	StringHashMap<ValueType> *d = new StringHashMap<ValueType>();

	for (int i=0; i<PyList_GET_SIZE(key_list); ++i)
	{
		PyObject* key = PyList_GET_ITEM(key_list, i);
		if (key == NULL)
		{
			break;
		}

		String* str_key = reinterpret_cast<String*>(sipForceConvertToType(key, sipType_String, NULL, SIP_NO_CONVERTORS, NULL, sipIsErr));
		if (*sipIsErr)
		{
			break;
		}
		PyObject* item = PyDict_GetItem(sipPy,key);
		if (!sipCanConvertToType(item, sipType_ValueType, SIP_NOT_NONE))
		{
			PyErr_Format(PyExc_TypeError, "object in dictionary cannot be converted to ValueType");
			*sipIsErr = 1;
			break;
		}

		int state;
		ValueType* p = reinterpret_cast<ValueType*>(
				sipConvertToType(item, sipType_ValueType, 0, SIP_NOT_NONE, &state, sipIsErr));

		if (!*sipIsErr)
			(*d)[*str_key] = *p;

		sipReleaseType(p, sipType_ValueType, state);
		Py_DECREF(item);
	}

	if (*sipIsErr)
	{
		delete d;
		return 0;
	}

	*sipCppPtr = d;
	return sipGetState(sipTransferObj);
%End
};

// we need to specialize for non-mapped data types
%MappedType StringHashMap<double>
{
%TypeHeaderCode
#include <BALL/DATATYPE/stringHashMap.h>
#include <BALL/PYTHON/pyBALLSipHelper.h>
%End

%ConvertFromTypeCode
	PyObject *dict;

	// create the python dictionary of the correct size
	if ((dict = PyDict_New()) == NULL)
		return NULL;

	// convert the hash map to a dictionary
	for (StringHashMap<double>::Iterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		// NOTE: keys into a string hash map are const!
		PyObject* key = sipConvertFromType(const_cast<String*>(&(it->first)), sipType_String, sipTransferObj);
		if (key == NULL)
		{
			Py_DECREF(dict);
			return NULL;
		}

		PyObject *pobj;

		// insert into the dictionary
		if (PyDict_SetItem(dict, key, PyFloat_FromDouble(it->second)) < 0)
		{
			Py_DECREF(dict);
			return NULL;
		}
	}

	return dict;
%End

%ConvertToTypeCode
	// Convert a Python dictionary to an object on the heap.
	
	// create a list of all keys
	PyObject* key_list = PyDict_Keys(sipPy);
	if (key_list == NULL)
	{
		return 0;
	}

	StringHashMap<double> *d = new StringHashMap<double>();

	for (int i=0; i<PyList_GET_SIZE(key_list); ++i)
	{
		PyObject* key = PyList_GET_ITEM(key_list, i);
		if (key == NULL)
		{
			break;
		}

		String* str_key = reinterpret_cast<String*>(sipForceConvertToType(key, sipType_String, NULL, SIP_NO_CONVERTORS, NULL, sipIsErr));
		if (*sipIsErr)
		{
			break;
		}
		PyObject* item = PyNumber_Float(PyDict_GetItem(sipPy,key));

		if (!*sipIsErr)
			(*d)[*str_key] = PyFloat_AsDouble(item);

		Py_DECREF(item);
	}

	if (*sipIsErr)
	{
		delete d;
		return 0;
	}

	*sipCppPtr = d;
	return sipGetState(sipTransferObj);
%End
};

//
// we need to specialize for non-mapped data types
%MappedType StringHashMap<float>
{
%TypeHeaderCode
#include <BALL/DATATYPE/stringHashMap.h>
#include <BALL/PYTHON/pyBALLSipHelper.h>
%End

%ConvertFromTypeCode
	PyObject *dict;

	// create the python dictionary of the correct size
	if ((dict = PyDict_New()) == NULL)
		return NULL;

	// convert the hash map to a dictionary
	for (StringHashMap<float>::Iterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		// NOTE: keys into a string hash map are const!
		PyObject* key = sipConvertFromType(const_cast<String*>(&(it->first)), sipType_String, sipTransferObj);
		if (key == NULL)
		{
			Py_DECREF(dict);
			return NULL;
		}

		PyObject *pobj;

		// insert into the dictionary
		if (PyDict_SetItem(dict, key, PyFloat_FromDouble(it->second)) < 0)
		{
			Py_DECREF(dict);
			return NULL;
		}
	}

	return dict;
%End

%ConvertToTypeCode
	// Convert a Python dictionary to an object on the heap.
	
	// create a list of all keys
	PyObject* key_list = PyDict_Keys(sipPy);
	if (key_list == NULL)
	{
		return 0;
	}

	StringHashMap<float> *d = new StringHashMap<float>();

	for (int i=0; i<PyList_GET_SIZE(key_list); ++i)
	{
		PyObject* key = PyList_GET_ITEM(key_list, i);
		if (key == NULL)
		{
			break;
		}

		String* str_key = reinterpret_cast<String*>(sipForceConvertToType(key, sipType_String, NULL, SIP_NO_CONVERTORS, NULL, sipIsErr));
		if (*sipIsErr)
		{
			break;
		}
		PyObject* item = PyNumber_Float(PyDict_GetItem(sipPy,key));

		if (!*sipIsErr)
			(*d)[*str_key] = PyFloat_AsDouble(item);

		Py_DECREF(item);
	}

	if (*sipIsErr)
	{
		delete d;
		return 0;
	}

	*sipCppPtr = d;
	return sipGetState(sipTransferObj);
%End
};

// we need to specialize for non-mapped data types
%MappedType StringHashMap<int>
{
%TypeHeaderCode
#include <BALL/DATATYPE/stringHashMap.h>
#include <BALL/PYTHON/pyBALLSipHelper.h>
%End

%ConvertFromTypeCode
	PyObject *dict;

	// create the python dictionary of the correct size
	if ((dict = PyDict_New()) == NULL)
		return NULL;

	// convert the hash map to a dictionary
	for (StringHashMap<int>::Iterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		// NOTE: keys into a string hash map are const!
		PyObject* key = sipConvertFromType(const_cast<String*>(&(it->first)), sipType_String, sipTransferObj);
		if (key == NULL)
		{
			Py_DECREF(dict);
			return NULL;
		}

		PyObject *pobj;

		// insert into the dictionary
		if (PyDict_SetItem(dict, key, PyInt_FromLong(it->second)) < 0)
		{
			Py_DECREF(dict);
			return NULL;
		}
	}

	return dict;
%End

%ConvertToTypeCode
	// Convert a Python dictionary to an object on the heap.
	
	// create a list of all keys
	PyObject* key_list = PyDict_Keys(sipPy);
	if (key_list == NULL)
	{
		return 0;
	}

	StringHashMap<int> *d = new StringHashMap<int>();

	for (int i=0; i<PyList_GET_SIZE(key_list); ++i)
	{
		PyObject* key = PyList_GET_ITEM(key_list, i);
		if (key == NULL)
		{
			break;
		}

		String* str_key = reinterpret_cast<String*>(sipForceConvertToType(key, sipType_String, NULL, SIP_NO_CONVERTORS, NULL, sipIsErr));
		if (*sipIsErr)
		{
			break;
		}
		PyObject* item = PyDict_GetItem(sipPy,key);
		int val = PyInt_AsLong(item);

		if (!*sipIsErr)
			(*d)[*str_key] = val;

		Py_DECREF(item);
	}

	if (*sipIsErr)
	{
		delete d;
		return 0;
	}

	*sipCppPtr = d;
	return sipGetState(sipTransferObj);
%End
};
