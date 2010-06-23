// SIP support for std::list 
// by Giovanni Bajo 
// Public domain


// ****************************************************
// SIP generic implementation for std::list<>
// ****************************************************
// ALas, this template-based generic implementation is valid only
// if the element type is a SIP-wrapped type. For basic types (int, double, etc.) 
// we are forced to cut & paste to provide a specialization.

template<TYPE>
%MappedType deque<TYPE>
{
%TypeHeaderCode
	#include <deque>
	using namespace std;
%End

%ConvertFromTypeCode
    PyObject *l;

    // Create the Python list of the correct length.
    if ((l = PyList_New(sipCpp -> size())) == NULL)
        return NULL;

    // Go through each element in the C++ instance and convert it to a
    // wrapped P2d.
		int i = 0;
    for (std::deque<TYPE>::const_iterator it = sipCpp->begin(); it != sipCpp -> end(); ++it, ++i)
    {
        TYPE *cpp = new TYPE(*it);
        PyObject *pobj;

        // Get the Python wrapper for the Type instance, creating a new
        // one if necessary, and handle any ownership transfer.
        if ((pobj = BALL_CONVERT_FROM_INSTANCE(cpp, TYPE, sipTransferObj)) == NULL)
        {
            // There was an error so garbage collect the Python list.
            Py_DECREF(l);
            return NULL;
        }

        // Add the wrapper to the list.
        PyList_SET_ITEM(l, i, pobj);
    }

    // Return the Python list.
    return l;
%End

%ConvertToTypeCode
    // Check if type is compatible
    if (sipIsErr == NULL)
    {
        // Must be any iterable
        PyObject *i = PyObject_GetIter(sipPy);
        bool iterable = (i != NULL);
        Py_XDECREF(i);
        return iterable;
    }

    // Iterate over the object
    PyObject *iterator = PyObject_GetIter(sipPy);
    PyObject *item;

    std::deque<TYPE> *V = new std::deque<TYPE>();

    while ((item = PyIter_Next(iterator)))
    {
        if (!BALL_CAN_CONVERT_TO_INSTANCE(item, TYPE))
        {
            PyErr_Format(PyExc_TypeError, "object in iterable cannot be converted to TYPE");
            *sipIsErr = 1;
            break;
        }

        int state;
        TYPE* p = BALL_CONVERT_TO_INSTANCE(item, TYPE, state);

        if (!*sipIsErr)
            V->push_back(*p);

        BALL_RELEASE_INSTANCE(p, TYPE, state);
        Py_DECREF(item);
    }

    Py_DECREF(iterator);

    if (*sipIsErr)
    {
        delete V;
        return 0;
    }

    *sipCppPtr = V;
    return sipGetState(sipTransferObj);
%End
};

template<TYPE *>
%MappedType deque<TYPE *>
{
%TypeHeaderCode
	#include <deque>
	using namespace std;
%End

%ConvertFromTypeCode
    PyObject *l;

    // Create the Python list of the correct length.
    if ((l = PyList_New(sipCpp -> size())) == NULL)
        return NULL;

    // Go through each element in the C++ instance and convert it to a
    // wrapped P2d.
		int i = 0;
    for (std::deque<TYPE *>::const_iterator it = sipCpp->begin(); it != sipCpp -> end(); ++it, ++i)
    {
        TYPE *cpp = *it;
        PyObject *pobj;

        // Get the Python wrapper for the Type instance, creating a new
        // one if necessary, and handle any ownership transfer.
        if ((pobj = BALL_CONVERT_FROM_INSTANCE(cpp, TYPE, sipTransferObj)) == NULL)
        {
            // There was an error so garbage collect the Python list.
            Py_DECREF(l);
            return NULL;
        }

        // Add the wrapper to the list.
        PyList_SET_ITEM(l, i, pobj);
    }

    // Return the Python list.
    return l;
%End

%ConvertToTypeCode
    // Check if type is compatible
    if (sipIsErr == NULL)
    {
        // Must be any iterable
        PyObject *i = PyObject_GetIter(sipPy);
        bool iterable = (i != NULL);
        Py_XDECREF(i);
        return iterable;
    }

    // Iterate over the object
    PyObject *iterator = PyObject_GetIter(sipPy);
    PyObject *item;

    std::deque<TYPE *> *V = new std::deque<TYPE *>();

    while ((item = PyIter_Next(iterator)))
    {
        if (!BALL_CAN_CONVERT_TO_INSTANCE(item, TYPE))
        {
            PyErr_Format(PyExc_TypeError, "object in iterable cannot be converted to TYPE");
            *sipIsErr = 1;
            break;
        }

        int state;
        TYPE* p = reinterpret_cast<TYPE*>(BALL_CONVERT_TO_INSTANCE(item, TYPE, state));

        if (*sipIsErr) {
					delete V;
					return 0;
				} else {
					V->push_back(p);
				}
    }

    *sipCppPtr = V;
    return sipGetState(sipTransferObj);
%End
};

