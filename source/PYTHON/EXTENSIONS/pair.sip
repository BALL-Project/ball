// SIP support for std::pair 
// by Giovanni Bajo 
// Public domain


// ****************************************************
// SIP generic implementation for std::pair<>
// ****************************************************
// ALas, this template-based generic implementation is valid only
// if the element type is a SIP-wrapped type. For basic types (int, double, etc.) 
// we are forced to cut & paste to provide a specialization.

// NOTE: we do not yet provide all specializations, since there are a lot of combinations.
//       if you need another one (e.g. pair<String, int>), feel free to provide the specialization
//       at the end of the file!
//
template <first_type, second_type>
%MappedType pair<first_type, second_type>
{
%TypeHeaderCode
#include <utility>
#include <BALLglobal.h>
using namespace BALL;
%End

%ConvertFromTypeCode
    PyObject *t;

    // Create the Python tuple of size 2
    if ((t = PyTuple_New(2)) == NULL)
        return NULL;

		// convert first and second 
    // Go through each element in the std::pair instance and convert it to a
    // wrapped python type

		// sipCpp->first
		first_type *cpp_first = new first_type(sipCpp->first);
		PyObject *first_object;
		
		// Get the Python wrapper for the Type instance, creating a new
		// one if necessary, and handle any ownership transfer.
		if ((first_object = sipConvertFromInstance(cpp_first, sipClass_first_type, sipTransferObj)) == NULL)
		{
			// There was an error so garbage collect the Python tuple.
			Py_DECREF(t);
			return NULL;
		}

		// Add the wrapper to the tuple.
		PyTuple_SET_ITEM(t, 0, first_object);

		// sipCpp->second
		second_type *cpp_second = new second_type(sipCpp->second);
		PyObject *second_object;
		
		// Get the Python wrapper for the Type instance, creating a new
		// one if necessary, and handle any ownership transfer.
		if ((second_object = sipConvertFromInstance(cpp_second, sipClass_second_type, sipTransferObj)) == NULL)
		{
			// There was an error so garbage collect the Python tuple.
			Py_DECREF(t);
			return NULL;
		}

		// Add the wrapper to the tuple.
		PyTuple_SET_ITEM(t, 1, second_object);

    // Return the Python tuple.
    return t;
%End

%ConvertToTypeCode
		// if sipIsErr == NULL, we are only asked to check for compatibility of the types
    if (sipIsErr == NULL)
    {
			// Must be a tuple of length 2
			bool result = PyTuple_Check(sipPy);
			result &= (PyTuple_Size(sipPy) == 2);

			return result;
    }

		// Take the first element of the tuple
		PyObject *first_item  = PyTuple_GetItem(sipPy, 0);
		PyObject *second_item = PyTuple_GetItem(sipPy, 1);

    std::pair<first_type, second_type> *p = new std::pair<first_type, second_type>();

		if (!sipCanConvertToInstance(first_item, sipClass_first_type, SIP_NOT_NONE))
		{
			PyErr_Format(PyExc_TypeError, "first object in tuple cannot be converted to first_type");
			*sipIsErr = 1;
		}

		if (!sipCanConvertToInstance(second_item, sipClass_second_type, SIP_NOT_NONE))
		{
			PyErr_Format(PyExc_TypeError, "second object in tuple cannot be converted to second_type");
			*sipIsErr = 1;
		}

		int state;
		first_type* ball_first_object = reinterpret_cast<first_type*>(
				sipConvertToInstance(first_item, sipClass_first_type, 0, SIP_NOT_NONE, &state, sipIsErr));

		if (!*sipIsErr)
			p->first = *ball_first_object;

		sipReleaseInstance(ball_first_object, sipClass_first_type, state);
		Py_DECREF(first_item);

		second_type* ball_second_object = reinterpret_cast<second_type*>(
				sipConvertToInstance(second_item, sipClass_second_type, 0, SIP_NOT_NONE, &state, sipIsErr));

		if (!*sipIsErr)
			p->second = *ball_second_object;

		sipReleaseInstance(ball_second_object, sipClass_second_type, state);
		Py_DECREF(second_item);

    if (*sipIsErr)
    {
        delete p;
        return 0;
    }

    *sipCppPtr = p;

    return sipGetState(sipTransferObj);
%End
};

// specialization for Position, Position
%MappedType pair<Position, Position>
{
%TypeHeaderCode
#include <utility>
%End

%ConvertFromTypeCode
    PyObject *t;

    // Create the Python tuple of size 2
    if ((t = PyTuple_New(2)) == NULL)
        return NULL;

		// convert first and second 
    // Go through each element in the std::pair instance and convert it to a
    // wrapped python type

		// sipCpp->first
		PyTuple_SET_ITEM(t, 0, PyInt_FromSize_t(sipCpp->first));

		// sipCpp->second
		PyTuple_SET_ITEM(t, 1, PyInt_FromSize_t(sipCpp->second));
		
    // Return the Python tuple.
    return t;
%End

%ConvertToTypeCode
		// if sipIsErr == NULL, we are only asked to check for compatibility of the types
    if (sipIsErr == NULL)
    {
			// Must be a tuple of length 2
			bool result = PyTuple_Check(sipPy);
			result &= (PyTuple_Size(sipPy) == 2);

			return result;
    }

		// Take the first element of the tuple
		PyObject *first_item  = PyTuple_GetItem(sipPy, 0);
		PyObject *second_item = PyTuple_GetItem(sipPy, 1);

    std::pair<Position, Position> *p = new std::pair<Position, Position>();

		if (!PyInt_Check(first_item))
		{
			PyErr_Format(PyExc_TypeError, "first object in tuple cannot be converted to Position");
			*sipIsErr = 1;
		}

		if (!PyInt_Check(second_item))
		{
			PyErr_Format(PyExc_TypeError, "second object in tuple cannot be converted to Position");
			*sipIsErr = 1;
		}

		Position first_value  = (Position)PyInt_AsSsize_t(first_item);
		Position second_value = (Position)PyInt_AsSsize_t(second_item);

		p->first  = first_value;
		p->second = second_value;

		Py_DECREF(first_item);
		Py_DECREF(second_item);

    if (*sipIsErr)
    {
        delete p;
        return 0;
    }

    *sipCppPtr = p;

    return sipGetState(sipTransferObj);
%End
};
