#include "sipBALLDeclBALL.h"
#include "sipBALLVector4.h"



PyObject *sipClass_Vector4;

static void sipDealloc_Vector4(sipThisType *);

static PyTypeObject sipType_Vector4 = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Vector4,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Vector4,
	0,
	0,
	0,
	0,
	0,
};

static PyObject *sipDo_Vector4_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector4)) == NULL)
		return NULL;

	{
		float a0;
		float a1;
		float a2;
		float a3 = 1;

		if (sipParseArgs(sipArgs,"fff|f",&a0,&a1,&a2,&a3))
		{
			Vector4 *ptr;

			if ((ptr = (Vector4 *)sipGetCppPtr(sipThis,sipClass_Vector4)) == NULL)
				return NULL;

			ptr -> Vector4::set( a0, a1, a2, a3);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const Vector4 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector4,&a0obj))
		{
			Vector4 *ptr;

			if ((ptr = (Vector4 *)sipGetCppPtr(sipThis,sipClass_Vector4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector4(a0obj,(Vector4 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Vector4::set(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		float a0 = 1;

		if (sipParseArgs(sipArgs,"|f",&a0))
		{
			Vector4 *ptr;

			if ((ptr = (Vector4 *)sipGetCppPtr(sipThis,sipClass_Vector4)) == NULL)
				return NULL;

			ptr -> Vector4::set( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Vector4,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_Vector4_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector4)) == NULL)
		return NULL;

	{
		float a0;
		float a1;
		float a2;
		float a3;

		if (sipParseArgs(sipArgs,"ffff",&a0,&a1,&a2,&a3))
		{
			Vector4 *ptr;

			if ((ptr = (Vector4 *)sipGetCppPtr(sipThis,sipClass_Vector4)) == NULL)
				return NULL;

			ptr -> Vector4::get( a0, a1, a2, a3);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		Vector4 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector4,&a0obj))
		{
			Vector4 *ptr;

			if ((ptr = (Vector4 *)sipGetCppPtr(sipThis,sipClass_Vector4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector4(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Vector4::get(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Vector4,sipName_BALL_get);

	return NULL;
}

static PyObject *sipDo_Vector4_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector4)) == NULL)
		return NULL;

	{
		Vector4 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector4,&a0obj))
		{
			Vector4 *ptr;

			if ((ptr = (Vector4 *)sipGetCppPtr(sipThis,sipClass_Vector4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector4(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Vector4::swap(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Vector4,sipName_BALL_swap);

	return NULL;
}

static PyObject *sipDo_Vector4_getLength(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector4)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			Vector4 *ptr;

			if ((ptr = (Vector4 *)sipGetCppPtr(sipThis,sipClass_Vector4)) == NULL)
				return NULL;

			res = ptr -> Vector4::getLength();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Vector4,sipName_BALL_getLength);

	return NULL;
}

static PyObject *sipDo_Vector4_getSquareLength(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector4)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			Vector4 *ptr;

			if ((ptr = (Vector4 *)sipGetCppPtr(sipThis,sipClass_Vector4)) == NULL)
				return NULL;

			res = ptr -> Vector4::getSquareLength();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Vector4,sipName_BALL_getSquareLength);

	return NULL;
}

static PyObject *sipDo_Vector4_normalize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector4)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Vector4 *res;
			Vector4 *ptr;

			if ((ptr = (Vector4 *)sipGetCppPtr(sipThis,sipClass_Vector4)) == NULL)
				return NULL;

			res = &ptr -> Vector4::normalize();

			return sipMapCppToSelf(res,sipClass_Vector4);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Vector4,sipName_BALL_normalize);

	return NULL;
}

static PyObject *sipDo_Vector4_getZero(PyObject *,PyObject *sipArgs)
{

	{
		if (sipParseArgs(sipArgs,"-"))
		{
			const Vector4 *res;

			res = &Vector4::getZero();

			return sipMapCppToSelf(res,sipClass_Vector4);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Vector4,sipName_BALL_getZero);

	return NULL;
}

static PyObject *sipDo_Vector4_getUnit(PyObject *,PyObject *sipArgs)
{

	{
		if (sipParseArgs(sipArgs,"-"))
		{
			const Vector4 *res;

			res = &Vector4::getUnit();

			return sipMapCppToSelf(res,sipClass_Vector4);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Vector4,sipName_BALL_getUnit);

	return NULL;
}

static PyObject *sipDo_Vector4_getDistance(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector4)) == NULL)
		return NULL;

	{
		const Vector4 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector4,&a0obj))
		{
			float res;
			Vector4 *ptr;

			if ((ptr = (Vector4 *)sipGetCppPtr(sipThis,sipClass_Vector4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector4(a0obj,(Vector4 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Vector4::getDistance(* a0);

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Vector4,sipName_BALL_getDistance);

	return NULL;
}

static PyObject *sipDo_Vector4_getSquareDistance(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector4)) == NULL)
		return NULL;

	{
		const Vector4 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector4,&a0obj))
		{
			float res;
			Vector4 *ptr;

			if ((ptr = (Vector4 *)sipGetCppPtr(sipThis,sipClass_Vector4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector4(a0obj,(Vector4 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Vector4::getSquareDistance(* a0);

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Vector4,sipName_BALL_getSquareDistance);

	return NULL;
}

static PyObject *sipDo_Vector4_isOrthogonalTo(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector4)) == NULL)
		return NULL;

	{
		Vector4 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector4,&a0obj))
		{
			bool res;
			Vector4 *ptr;

			if ((ptr = (Vector4 *)sipGetCppPtr(sipThis,sipClass_Vector4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector4(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Vector4::isOrthogonalTo(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Vector4,sipName_BALL_isOrthogonalTo);

	return NULL;
}

static PyObject *sipDo_Vector4_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector4)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Vector4 *ptr;

			if ((ptr = (Vector4 *)sipGetCppPtr(sipThis,sipClass_Vector4)) == NULL)
				return NULL;

			res = ptr -> Vector4::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Vector4,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Vector4(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Vector4)
		return ptr;

	return NULL;
}

static void sipDealloc_Vector4(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (Vector4 *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_Vector4(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Vector4
	};

	sipThisType *sipThis = NULL;
	const void *sipNew = NULL;
	int sipFlags = SIP_PY_OWNED;

	// See if there is something pending.

	sipNew = sipGetPending(&sipFlags);

	if (sipNew == NULL)
	{
		if (sipParseArgs(sipArgs,"-"))
		{
			sipNew = new Vector4();
		}
	}

	if (sipNew == NULL)
	{
		float a0;
		float a1;
		float a2;
		float a3 = 1;

		if (sipParseArgs(sipArgs,"-fff|f",&a0,&a1,&a2,&a3))
		{
			sipNew = new Vector4( a0, a1, a2, a3);
		}
	}

	if (sipNew == NULL)
	{
		const Vector4 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_Vector4,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Vector4(a0obj,(Vector4 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Vector4(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_Vector4);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Vector4,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (Vector4 *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Vector4_x(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Vector4 *ptr;

	if ((ptr = (Vector4 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Vector4)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> x;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Vector4,sipName_BALL_x);
		return NULL;
	}

	ptr -> x = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Vector4_y(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Vector4 *ptr;

	if ((ptr = (Vector4 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Vector4)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> y;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Vector4,sipName_BALL_y);
		return NULL;
	}

	ptr -> y = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Vector4_z(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Vector4 *ptr;

	if ((ptr = (Vector4 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Vector4)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> z;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Vector4,sipName_BALL_z);
		return NULL;
	}

	ptr -> z = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Vector4_h(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Vector4 *ptr;

	if ((ptr = (Vector4 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Vector4)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> h;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Vector4,sipName_BALL_h);
		return NULL;
	}

	ptr -> h = val;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Vector4[] = {
	{sipName_BALL_set, sipDo_Vector4_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_Vector4_get, METH_VARARGS, NULL},
	{sipName_BALL_swap, sipDo_Vector4_swap, METH_VARARGS, NULL},
	{sipName_BALL_getLength, sipDo_Vector4_getLength, METH_VARARGS, NULL},
	{sipName_BALL_getSquareLength, sipDo_Vector4_getSquareLength, METH_VARARGS, NULL},
	{sipName_BALL_normalize, sipDo_Vector4_normalize, METH_VARARGS, NULL},
	{sipName_BALL_getZero, sipDo_Vector4_getZero, METH_VARARGS, NULL},
	{sipName_BALL_getUnit, sipDo_Vector4_getUnit, METH_VARARGS, NULL},
	{sipName_BALL_getDistance, sipDo_Vector4_getDistance, METH_VARARGS, NULL},
	{sipName_BALL_getSquareDistance, sipDo_Vector4_getSquareDistance, METH_VARARGS, NULL},
	{sipName_BALL_isOrthogonalTo, sipDo_Vector4_isOrthogonalTo, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_Vector4_isValid, METH_VARARGS, NULL},
	{NULL}
};

PyMethodDef sipClassVarTab_Vector4[] = {
	{sipName_BALL_x, sipGetSetVar_Vector4_x, 0, NULL},
	{sipName_BALL_y, sipGetSetVar_Vector4_y, 0, NULL},
	{sipName_BALL_z, sipGetSetVar_Vector4_z, 0, NULL},
	{sipName_BALL_h, sipGetSetVar_Vector4_h, 0, NULL},
	{NULL}
};

PyMethodDef *sipClassVarHierTab_Vector4[] = {
	sipClassVarTab_Vector4,
	NULL
};

int sipCanConvertTo_Vector4(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Vector4);
}

void sipConvertTo_Vector4(PyObject *sipPy,Vector4 **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_Vector4);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Vector4 *)sipConvertToCpp(sipPy,sipClass_Vector4,sipIsErr);
}

Vector4 *sipForceConvertTo_Vector4(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Vector4(valobj))
	{
		Vector4 *val;

		sipConvertTo_Vector4(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Vector4);
	*iserrp = 1;

	return NULL;
}
