template<FIRSTTYPE, SECONDTYPE>
%MappedType pair<FIRSTTYPE, SECONDTYPE>
{
%TypeHeaderCode
	#include <utility>
	using namespace std;
%End

%ConvertFromTypeCode
    PyObject *t;

    // Create the Python list of the correct length.
    if ((t = PyTuple_New(2)) == NULL)
        return NULL;

		PyObject* tmp;

		FIRSTTYPE* first = new FIRSTTYPE(sipCpp->first);
		if ((tmp = BALL_CONVERT_FROM_INSTANCE(first, FIRSTTYPE, sipTransferObj)) == NULL)
		{
			Py_DECREF(t);
			return NULL;
		}
		PyTuple_SET_ITEM(t, 0, tmp);

		SECONDTYPE* second = new SECONDTYPE(sipCpp->second);
		if ((tmp = BALL_CONVERT_FROM_INSTANCE(second, SECONDTYPE, sipTransferObj)) == NULL)
		{
			Py_DECREF(t);
			return NULL;
		}
		PyTuple_SET_ITEM(t, 1, tmp);

    return t;
%End

%ConvertToTypeCode
    // Check if type is compatible
    if (sipIsErr == NULL)
    {
        // Must be any iterable
				if(PyTuple_Check(sipPy)) {
					return PyTuple_Size(sipPy) == 2;
				}

				return false;
    }

		int state;
		if(!BALL_CAN_CONVERT_TO_INSTANCE(PyTuple_GET_ITEM(sipPy, 0), FIRSTTYPE))
		{
			PyErr_Format(PyExc_TypeError, "Object 0 in tuple cannot be converted to FIRSTTYPE");
			*sipIsErr = 1;
			return 0;
		}
		FIRSTTYPE* first  = BALL_CONVERT_TO_INSTANCE(PyTuple_GET_ITEM(sipPy, 0), FIRSTTYPE, state);

		if(!BALL_CAN_CONVERT_TO_INSTANCE(PyTuple_GET_ITEM(sipPy, 1), SECONDTYPE))
		{
			PyErr_Format(PyExc_TypeError, "Object 1 in tuple cannot be converted to SECONDTYPE");
			*sipIsErr = 1;
			return 0;
		}
		SECONDTYPE* second = BALL_CONVERT_TO_INSTANCE(PyTuple_GET_ITEM(sipPy, 1), SECONDTYPE, state);

		std::pair<FIRSTTYPE, SECONDTYPE>* result = NULL;

		if(!*sipIsErr) {
			result = new std::pair<FIRSTTYPE, SECONDTYPE>(*first, *second);
		}

    BALL_RELEASE_INSTANCE(first , FIRSTTYPE, state);
    BALL_RELEASE_INSTANCE(second, SECONDTYPE, state);

		*sipCppPtr = result;

		return sipGetState(sipTransferObj);
%End
};

//
// Specialization for double
//

template<FIRSTTYPE, double>
%MappedType pair<FIRSTTYPE, double>
{
%TypeHeaderCode
	#include <utility>
	using namespace std;
%End

%ConvertFromTypeCode
    PyObject *t;

    // Create the Python list of the correct length.
    if ((t = PyTuple_New(2)) == NULL)
        return NULL;

		PyObject* tmp;

		FIRSTTYPE* first = new FIRSTTYPE(sipCpp->first);
		if ((tmp = BALL_CONVERT_FROM_INSTANCE(first, FIRSTTYPE, sipTransferObj)) == NULL)
		{
			Py_DECREF(t);
			return NULL;
		}
		PyTuple_SET_ITEM(t, 0, tmp);
		PyTuple_SET_ITEM(t, 1, PyFloat_FromDouble(sipCpp->second));

    return t;
%End

%ConvertToTypeCode
    // Check if type is compatible
    if (sipIsErr == NULL)
    {
				return PyTuple_Check(sipPy) && (PyTuple_Size(sipPy) == 2) && PyNumber_Check(PyTuple_GET_ITEM(sipPy, 1));
    }

		PyObject* tmp = PyTuple_GET_ITEM(sipPy, 0);
		if(!BALL_CAN_CONVERT_TO_INSTANCE(tmp, FIRSTTYPE))
		{
			PyErr_Format(PyExc_TypeError, "Object 0 in tuple cannot be converted to FIRSTTYPE");
			*sipIsErr = 1;
			return 0;
		}

		int state;
		FIRSTTYPE* first  = BALL_CONVERT_TO_INSTANCE(PyTuple_GET_ITEM(sipPy, 0), FIRSTTYPE, state);

		if(!PyNumber_Check(PyTuple_GET_ITEM(sipPy, 1)))
		{
			PyErr_Format(PyExc_TypeError, "Object 1 in tuple cannot be converted to double");
			*sipIsErr = 1;
			return 0;
		}

		std::pair<FIRSTTYPE,double>* result = 0;

		if(!*sipIsErr) {
			result = new std::pair<FIRSTTYPE, double>(*first, PyFloat_AsDouble(PyNumber_Float(PyTuple_GET_ITEM(sipPy, 1))));
		}

    BALL_RELEASE_INSTANCE(first, FIRSTTYPE, state);

		*sipCppPtr = result;

		return sipGetState(sipTransferObj);
%End
};

//
// Specialization for Position, Position
//

%MappedType pair<Position, Position>
{
%TypeHeaderCode
	#include <utility>
	using namespace std;
%End

%ConvertFromTypeCode
    PyObject *t;

    // Create the Python list of the correct length.
    if ((t = PyTuple_New(2)) == NULL)
        return NULL;

		PyTuple_SET_ITEM(t, 0, PyInt_FromLong(sipCpp->first));
		PyTuple_SET_ITEM(t, 1, PyInt_FromLong(sipCpp->second));

    return t;
%End

%ConvertToTypeCode
    // Check if type is compatible
    if (sipIsErr == NULL)
    {
				return    PyTuple_Check(sipPy) 
				       && (PyTuple_Size(sipPy) == 2) 
				       && PyNumber_Check(PyTuple_GET_ITEM(sipPy, 0))
				       && PyNumber_Check(PyTuple_GET_ITEM(sipPy, 1));
    }

		std::pair<Position,Position>* result = 0;

		if(!*sipIsErr) {
			result = new std::pair<Position, Position>(PyInt_AsLong(PyNumber_Int(PyTuple_GET_ITEM(sipPy, 0))), PyInt_AsLong(PyNumber_Int(PyTuple_GET_ITEM(sipPy, 1))));
		}


		*sipCppPtr = result;

		return sipGetState(sipTransferObj);
%End
};
