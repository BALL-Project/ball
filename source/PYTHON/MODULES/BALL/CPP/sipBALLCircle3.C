#include "sipBALLDeclBALL.h"
#include "sipBALLCircle3.h"



PyObject *sipClass_Circle3;

static void sipDealloc_Circle3(sipThisType *);

static PyTypeObject sipType_Circle3 = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Circle3,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Circle3,
	0,
	0,
	0,
	0,
	0,
};

static PyObject *sipDo_Circle3_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Circle3)) == NULL)
		return NULL;

	{
		Circle3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Circle3,&a0obj))
		{
			Circle3 *ptr;

			if ((ptr = (Circle3 *)sipGetCppPtr(sipThis,sipClass_Circle3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Circle3(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Circle3::swap(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Circle3,sipName_BALL_swap);

	return NULL;
}

static PyObject *sipDo_Circle3_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Circle3)) == NULL)
		return NULL;

	{
		const Circle3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Circle3,&a0obj))
		{
			Circle3 *ptr;

			if ((ptr = (Circle3 *)sipGetCppPtr(sipThis,sipClass_Circle3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Circle3(a0obj,(Circle3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Circle3::set(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;
		float a2;

		if (sipParseArgs(sipArgs,"IIf",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj,&a2))
		{
			Circle3 *ptr;

			if ((ptr = (Circle3 *)sipGetCppPtr(sipThis,sipClass_Circle3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Circle3::set(* a0,* a1, a2);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Circle3,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_Circle3_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Circle3)) == NULL)
		return NULL;

	{
		Circle3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Circle3,&a0obj))
		{
			Circle3 *ptr;

			if ((ptr = (Circle3 *)sipGetCppPtr(sipThis,sipClass_Circle3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Circle3(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Circle3::get(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		Vector3 *a0;
		PyObject *a0obj;
		Vector3 *a1;
		PyObject *a1obj;
		float a2;

		if (sipParseArgs(sipArgs,"IIf",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj,&a2))
		{
			Circle3 *ptr;

			if ((ptr = (Circle3 *)sipGetCppPtr(sipThis,sipClass_Circle3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Circle3::get(* a0,* a1, a2);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Circle3,sipName_BALL_get);

	return NULL;
}

static PyObject *sipDo_Circle3_has(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Circle3)) == NULL)
		return NULL;

	{
		const Vector3 *a0;
		PyObject *a0obj;
		long a1 = false;

		if (sipParseArgs(sipArgs,"I|l",sipCanConvertTo_Vector3,&a0obj,&a1))
		{
			bool res;
			Circle3 *ptr;

			if ((ptr = (Circle3 *)sipGetCppPtr(sipThis,sipClass_Circle3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Circle3::has(* a0, (bool)a1);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Circle3,sipName_BALL_has);

	return NULL;
}

static PyObject *sipDo_Circle3_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Circle3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Circle3 *ptr;

			if ((ptr = (Circle3 *)sipGetCppPtr(sipThis,sipClass_Circle3)) == NULL)
				return NULL;

			res = ptr -> Circle3::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Circle3,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Circle3(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Circle3)
		return ptr;

	return NULL;
}

static void sipDealloc_Circle3(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (Circle3 *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_Circle3(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Circle3
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
			sipNew = new Circle3();
	}
	}

	if (sipNew == NULL)
	{
		const Circle3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_Circle3,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Circle3(a0obj,(Circle3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Circle3(* a0);
	}
	}

	if (sipNew == NULL)
	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;
		float a2;

		if (sipParseArgs(sipArgs,"-IIf",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj,&a2))
		{
			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Circle3(* a0,* a1, a2);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_Circle3);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Circle3,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (Circle3 *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Circle3[] = {
	{sipName_BALL_swap, sipDo_Circle3_swap, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_Circle3_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_Circle3_get, METH_VARARGS, NULL},
	{sipName_BALL_has, sipDo_Circle3_has, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_Circle3_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_Circle3(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Circle3);
}

void sipConvertTo_Circle3(PyObject *sipPy,Circle3 **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_Circle3);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Circle3 *)sipConvertToCpp(sipPy,sipClass_Circle3,sipIsErr);
}

Circle3 *sipForceConvertTo_Circle3(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Circle3(valobj))
	{
		Circle3 *val;

		sipConvertTo_Circle3(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Circle3);
	*iserrp = 1;

	return NULL;
}
