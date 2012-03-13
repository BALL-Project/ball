// based on vector.sip by Giovanni Bajo

template <ValueType>
%MappedType StringHashMap<ValueType>
{
%TypeHeaderCode
#include <BALL/DATATYPE/stringHashMap.h>
using namespace BALL;
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
		String* skey = new String(it->first);
		PyObject* key = BALL_CONVERT_FROM_INSTANCE(skey, String, sipTransferObj);
		if (key == NULL)
		{
			Py_DECREF(dict);
			return NULL;
		}

		ValueType *cpp   = new ValueType(it->second);
		PyObject *pobj;

		// get the Python wrapper for the Type instance, creating a new one
		// if necessary, and handle any ownership transfer.
		if ((pobj = BALL_CONVERT_FROM_INSTANCE(cpp, ValueType, sipTransferObj)) == NULL)
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

		String* str_key = reinterpret_cast<String*>(BALL_FORCE_CONVERT_TO_TYPE(key, String));
		if (*sipIsErr)
		{
			break;
		}
		PyObject* item = PyDict_GetItem(sipPy,key);
		if (!BALL_CAN_CONVERT_TO_INSTANCE(item, ValueType))
		{
			PyErr_Format(PyExc_TypeError, "object in dictionary cannot be converted to ValueType");
			*sipIsErr = 1;
			break;
		}

		int state;
		ValueType* p = BALL_CONVERT_TO_INSTANCE(item, ValueType, state);

		if (!*sipIsErr)
			(*d)[*str_key] = *p;

		BALL_RELEASE_INSTANCE(p, ValueType, state);
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

template <ValueType*>
%MappedType StringHashMap<ValueType*>
{
%TypeHeaderCode
#include <BALL/DATATYPE/stringHashMap.h>
#include <BALL/PYTHON/pyBALLSipHelper.h>
using namespace BALL;
%End

%ConvertFromTypeCode
	PyObject *dict;

	// create the python dictionary of the correct size
	if ((dict = PyDict_New()) == NULL)
		return NULL;

	// convert the hash map to a dictionary
	for (StringHashMap<ValueType *>::Iterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		// NOTE: keys into a string hash map are const!
		String* skey = new String(it->first);
		PyObject* key = BALL_CONVERT_FROM_INSTANCE(skey, String, sipTransferObj);
		if (key == NULL)
		{
			Py_DECREF(dict);
			return NULL;
		}

		ValueType *cpp   = new ValueType(it->second);
		PyObject *pobj;

		// get the Python wrapper for the Type instance, creating a new one
		// if necessary, and handle any ownership transfer.
		if ((pobj = BALL_CONVERT_FROM_INSTANCE(cpp, ValueType, sipTransferObj)) == NULL)
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

	StringHashMap<ValueType*> *d = new StringHashMap<ValueType*>();

	for (int i=0; i<PyList_GET_SIZE(key_list); ++i)
	{
		PyObject* key = PyList_GET_ITEM(key_list, i);
		if (key == NULL)
		{
			break;
		}

		String* str_key = reinterpret_cast<String*>(BALL_FORCE_CONVERT_TO_TYPE(key, String));
		if (*sipIsErr)
		{
			break;
		}
		PyObject* item = PyDict_GetItem(sipPy,key);
		if (!BALL_CAN_CONVERT_TO_INSTANCE(item, ValueType))
		{
			PyErr_Format(PyExc_TypeError, "object in dictionary cannot be converted to ValueType");
			*sipIsErr = 1;
			break;
		}

		int state;
		ValueType* p = BALL_CONVERT_TO_INSTANCE(item, ValueType, state);

		if (!*sipIsErr)
			(*d)[*str_key] = *p;

		BALL_RELEASE_INSTANCE(p, ValueType, state);
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

template <float>
%MappedType StringHashMap<float>
{
%TypeHeaderCode
#include <BALL/DATATYPE/stringHashMap.h>
#include <BALL/PYTHON/pyBALLSipHelper.h>
using namespace BALL;
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
		String* skey = new String(it->first);
		PyObject* key = BALL_CONVERT_FROM_INSTANCE(skey, String, sipTransferObj);
		if (key == NULL)
		{
			Py_DECREF(dict);
			return NULL;
		}

		PyObject *pobj = PyFloat_FromDouble(it->second);

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

	StringHashMap<float> *d = new StringHashMap<float>();

	for (int i=0; i<PyList_GET_SIZE(key_list); ++i)
	{
		PyObject* key = PyList_GET_ITEM(key_list, i);
		if (key == NULL)
		{
			break;
		}

		String* str_key = reinterpret_cast<String*>(BALL_FORCE_CONVERT_TO_TYPE(key, String));
		if (*sipIsErr)
		{
			break;
		}
		PyObject* item = PyDict_GetItem(sipPy,key);
		if (!PyNumber_Check(item))
		{
			PyErr_Format(PyExc_TypeError, "object in cannot be converted to float");
			*sipIsErr = 1;
			break;
		}

		(*d)[*str_key] = (float)PyFloat_AsDouble(item);
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
