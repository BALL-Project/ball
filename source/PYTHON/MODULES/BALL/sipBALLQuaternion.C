#include "sipBALLDeclBALL.h"
#include "sipBALLQuaternion.h"



PyObject *sipClass_Quaternion;

static void sipDealloc_Quaternion(sipThisType *);

static PyTypeObject sipType_Quaternion = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Quaternion,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Quaternion,
	0,
	0,
	0,
	0,
	0,
};

static PyObject *sipDo_Quaternion_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Quaternion)) == NULL)
		return NULL;

	{
		const Quaternion *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Quaternion,&a0obj))
		{
			Quaternion *ptr;

			if ((ptr = (Quaternion *)sipGetCppPtr(sipThis,sipClass_Quaternion)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Quaternion(a0obj,(Quaternion **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Quaternion::set(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;
		float a1;

		if (sipParseArgs(sipArgs,"If",sipCanConvertTo_Vector3,&a0obj,&a1))
		{
			Quaternion *ptr;

			if ((ptr = (Quaternion *)sipGetCppPtr(sipThis,sipClass_Quaternion)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Quaternion::set(* a0, a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		float a0;
		float a1;
		float a2;
		float a3;

		if (sipParseArgs(sipArgs,"ffff",&a0,&a1,&a2,&a3))
		{
			Quaternion *ptr;

			if ((ptr = (Quaternion *)sipGetCppPtr(sipThis,sipClass_Quaternion)) == NULL)
				return NULL;

			ptr -> Quaternion::set( a0, a1, a2, a3);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Quaternion,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_Quaternion_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Quaternion)) == NULL)
		return NULL;

	{
		Quaternion *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Quaternion,&a0obj))
		{
			Quaternion *ptr;

			if ((ptr = (Quaternion *)sipGetCppPtr(sipThis,sipClass_Quaternion)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Quaternion(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Quaternion::get(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Quaternion,sipName_BALL_get);

	return NULL;
}

static PyObject *sipDo_Quaternion_setIdentity(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Quaternion)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Quaternion *ptr;

			if ((ptr = (Quaternion *)sipGetCppPtr(sipThis,sipClass_Quaternion)) == NULL)
				return NULL;

			ptr -> Quaternion::setIdentity();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Quaternion,sipName_BALL_setIdentity);

	return NULL;
}

static PyObject *sipDo_Quaternion_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Quaternion)) == NULL)
		return NULL;

	{
		Quaternion *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Quaternion,&a0obj))
		{
			Quaternion *ptr;

			if ((ptr = (Quaternion *)sipGetCppPtr(sipThis,sipClass_Quaternion)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Quaternion(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Quaternion::swap(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Quaternion,sipName_BALL_swap);

	return NULL;
}

static PyObject *sipDo_Quaternion_getAngle(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Quaternion)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			Quaternion *ptr;

			if ((ptr = (Quaternion *)sipGetCppPtr(sipThis,sipClass_Quaternion)) == NULL)
				return NULL;

			res = ptr -> Quaternion::getAngle();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Quaternion,sipName_BALL_getAngle);

	return NULL;
}

static PyObject *sipDo_Quaternion_getAxis(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Quaternion)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Vector3 *res;
			Quaternion *ptr;

			if ((ptr = (Quaternion *)sipGetCppPtr(sipThis,sipClass_Quaternion)) == NULL)
				return NULL;

			res = new Vector3(ptr -> Quaternion::getAxis());

			return sipNewCppToSelf(res,sipClass_Vector3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Quaternion,sipName_BALL_getAxis);

	return NULL;
}

static PyObject *sipDo_Quaternion_getRotationMatrix(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Quaternion)) == NULL)
		return NULL;

	{
		Matrix4x4 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Matrix4x4,&a0obj))
		{
			Matrix4x4 *res;
			Quaternion *ptr;

			if ((ptr = (Quaternion *)sipGetCppPtr(sipThis,sipClass_Quaternion)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Matrix4x4(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = &ptr -> Quaternion::getRotationMatrix(* a0);

			return sipMapCppToSelf(res,sipClass_Matrix4x4);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Quaternion,sipName_BALL_getRotationMatrix);

	return NULL;
}

static PyObject *sipDo_Quaternion_getInverse(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Quaternion)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Quaternion *res;
			Quaternion *ptr;

			if ((ptr = (Quaternion *)sipGetCppPtr(sipThis,sipClass_Quaternion)) == NULL)
				return NULL;

			res = new Quaternion(ptr -> Quaternion::getInverse());

			return sipNewCppToSelf(res,sipClass_Quaternion,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Quaternion,sipName_BALL_getInverse);

	return NULL;
}

static PyObject *sipDo_Quaternion_getConjugate(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Quaternion)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Quaternion *res;
			Quaternion *ptr;

			if ((ptr = (Quaternion *)sipGetCppPtr(sipThis,sipClass_Quaternion)) == NULL)
				return NULL;

			res = new Quaternion(ptr -> Quaternion::getConjugate());

			return sipNewCppToSelf(res,sipClass_Quaternion,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Quaternion,sipName_BALL_getConjugate);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Quaternion(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Quaternion)
		return ptr;

	return NULL;
}

static void sipDealloc_Quaternion(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (Quaternion *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_Quaternion(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Quaternion
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
			sipNew = new Quaternion();
		}
	}

	if (sipNew == NULL)
	{
		const Quaternion *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_Quaternion,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Quaternion(a0obj,(Quaternion **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Quaternion(* a0);
		}
	}

	if (sipNew == NULL)
	{
		const Vector3 *a0;
		PyObject *a0obj;
		float a1;

		if (sipParseArgs(sipArgs,"-If",sipCanConvertTo_Vector3,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Quaternion(* a0, a1);
		}
	}

	if (sipNew == NULL)
	{
		float a0;
		float a1;
		float a2;
		float a3;

		if (sipParseArgs(sipArgs,"-ffff",&a0,&a1,&a2,&a3))
		{
			sipNew = new Quaternion( a0, a1, a2, a3);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_Quaternion);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Quaternion,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (Quaternion *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Quaternion_i(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Quaternion *ptr;

	if ((ptr = (Quaternion *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Quaternion)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> i;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Quaternion,sipName_BALL_i);
		return NULL;
	}

	ptr -> i = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Quaternion_j(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Quaternion *ptr;

	if ((ptr = (Quaternion *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Quaternion)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> j;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Quaternion,sipName_BALL_j);
		return NULL;
	}

	ptr -> j = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Quaternion_k(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Quaternion *ptr;

	if ((ptr = (Quaternion *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Quaternion)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> k;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Quaternion,sipName_BALL_k);
		return NULL;
	}

	ptr -> k = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Quaternion_angle(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Quaternion *ptr;

	if ((ptr = (Quaternion *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Quaternion)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> angle;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Quaternion,sipName_BALL_angle);
		return NULL;
	}

	ptr -> angle = val;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Quaternion[] = {
	{sipName_BALL_set, sipDo_Quaternion_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_Quaternion_get, METH_VARARGS, NULL},
	{sipName_BALL_setIdentity, sipDo_Quaternion_setIdentity, METH_VARARGS, NULL},
	{sipName_BALL_swap, sipDo_Quaternion_swap, METH_VARARGS, NULL},
	{sipName_BALL_getAngle, sipDo_Quaternion_getAngle, METH_VARARGS, NULL},
	{sipName_BALL_getAxis, sipDo_Quaternion_getAxis, METH_VARARGS, NULL},
	{sipName_BALL_getRotationMatrix, sipDo_Quaternion_getRotationMatrix, METH_VARARGS, NULL},
	{sipName_BALL_getInverse, sipDo_Quaternion_getInverse, METH_VARARGS, NULL},
	{sipName_BALL_getConjugate, sipDo_Quaternion_getConjugate, METH_VARARGS, NULL},
	{NULL}
};

PyMethodDef sipClassVarTab_Quaternion[] = {
	{sipName_BALL_i, sipGetSetVar_Quaternion_i, 0, NULL},
	{sipName_BALL_j, sipGetSetVar_Quaternion_j, 0, NULL},
	{sipName_BALL_k, sipGetSetVar_Quaternion_k, 0, NULL},
	{sipName_BALL_angle, sipGetSetVar_Quaternion_angle, 0, NULL},
	{NULL}
};

PyMethodDef *sipClassVarHierTab_Quaternion[] = {
	sipClassVarTab_Quaternion,
	NULL
};

int sipCanConvertTo_Quaternion(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Quaternion);
}

void sipConvertTo_Quaternion(PyObject *sipPy,Quaternion **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_Quaternion);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Quaternion *)sipConvertToCpp(sipPy,sipClass_Quaternion,sipIsErr);
}

Quaternion *sipForceConvertTo_Quaternion(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Quaternion(valobj))
	{
		Quaternion *val;

		sipConvertTo_Quaternion(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Quaternion);
	*iserrp = 1;

	return NULL;
}
