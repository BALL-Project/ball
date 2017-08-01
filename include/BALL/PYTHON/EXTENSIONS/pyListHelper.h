// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyListHelper.h,v 1.1.2.2 2007/03/28 15:43:38 amoll Exp $
//
//
#ifndef BALL_PYTHON_PYLIST_HELPER
#define BALL_PYTHON_PYLIST_HELPER

#include <BALL/DATATYPE/regularData3D.h>

namespace BALL
{

typedef std::list<RegularData3D*> RegularData3DList;

// Convert the list.
#define BALL_CONVERT_LIST_FROM(TYPE)\
	PyObject *pl;\
	\
	if ((pl = PyList_New(0)) == NULL) return NULL;\
	\
	for (TYPE##List::const_iterator it = sipCpp->begin(); it != sipCpp->end(); ++it)\
	{\
		PyObject *inst = BALL_CONVERT_FROM_INSTANCE(*it, TYPE, 0);\
		\
		if (inst == NULL || PyList_Append(pl,inst) < 0)\
		{\
			Py_DECREF(pl);\
			return NULL;\
		}\
	}\
	\
	return pl;
 

// Convert a Python list of TYPE instances to a TYPEList object on the heap.
#define BALL_CONVERT_LIST_TO(TYPE)\
	if (sipIsErr == NULL) return PyList_Check(sipPy);\
	\
	TYPE##List* alist = new TYPE##List;\
 	\
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)\
	{\
		TYPE* a = reinterpret_cast<TYPE*>(BALL_FORCE_CONVERT_TO_TYPE(PyList_GET_ITEM(sipPy,i), TYPE));\
 		\
		if (*sipIsErr)\
		{\
			delete alist;\
			return 0;\
		}\
 		\
		alist->push_back(a);\
	}\
 	\
	*sipCppPtr = alist;\
	\
	return 1;

}

#endif
