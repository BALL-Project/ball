#include "sipBALLDeclBALL.h"
#include "sipBALLPlane3.h"



PyObject *sipClass_Plane3;

static void sipDealloc_Plane3(sipThisType *);

static PyTypeObject sipType_Plane3 = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Plane3,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Plane3,
	0,
	0,
	0,
	0,
	0,
};

static PyObject *sipDo_Plane3_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Plane3)) == NULL)
		return NULL;

	{
		Plane3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Plane3,&a0obj))
		{
			Plane3 *ptr;

			if ((ptr = (Plane3 *)sipGetCppPtr(sipThis,sipClass_Plane3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Plane3(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Plane3::swap(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Plane3,sipName_BALL_swap);

	return NULL;
}

static PyObject *sipDo_Plane3_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Plane3)) == NULL)
		return NULL;

	{
		const Plane3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Plane3,&a0obj))
		{
			Plane3 *ptr;

			if ((ptr = (Plane3 *)sipGetCppPtr(sipThis,sipClass_Plane3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Plane3(a0obj,(Plane3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Plane3::set(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj))
		{
			Plane3 *ptr;

			if ((ptr = (Plane3 *)sipGetCppPtr(sipThis,sipClass_Plane3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Plane3::set(* a0,* a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;
		const Vector3 *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"III",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj,sipCanConvertTo_Vector3,&a2obj))
		{
			Plane3 *ptr;

			if ((ptr = (Plane3 *)sipGetCppPtr(sipThis,sipClass_Plane3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);
			sipConvertTo_Vector3(a2obj,(Vector3 **)&a2,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Plane3::set(* a0,* a1,* a2);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Plane3,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_Plane3_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Plane3)) == NULL)
		return NULL;

	{
		Plane3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Plane3,&a0obj))
		{
			Plane3 *ptr;

			if ((ptr = (Plane3 *)sipGetCppPtr(sipThis,sipClass_Plane3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Plane3(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Plane3::get(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		Vector3 *a0;
		PyObject *a0obj;
		Vector3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj))
		{
			Plane3 *ptr;

			if ((ptr = (Plane3 *)sipGetCppPtr(sipThis,sipClass_Plane3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Plane3::get(* a0,* a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Plane3,sipName_BALL_get);

	return NULL;
}

static PyObject *sipDo_Plane3_normalize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Plane3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Plane3 *ptr;

			if ((ptr = (Plane3 *)sipGetCppPtr(sipThis,sipClass_Plane3)) == NULL)
				return NULL;

			ptr -> Plane3::normalize();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Plane3,sipName_BALL_normalize);

	return NULL;
}

static PyObject *sipDo_Plane3_hessify(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Plane3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Plane3 *ptr;

			if ((ptr = (Plane3 *)sipGetCppPtr(sipThis,sipClass_Plane3)) == NULL)
				return NULL;

			ptr -> Plane3::hessify();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Plane3,sipName_BALL_hessify);

	return NULL;
}

static PyObject *sipDo_Plane3_has(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Plane3)) == NULL)
		return NULL;

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			bool res;
			Plane3 *ptr;

			if ((ptr = (Plane3 *)sipGetCppPtr(sipThis,sipClass_Plane3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Plane3::has(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Line3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Line3,&a0obj))
		{
			bool res;
			Plane3 *ptr;

			if ((ptr = (Plane3 *)sipGetCppPtr(sipThis,sipClass_Plane3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Line3(a0obj,(Line3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Plane3::has(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Plane3,sipName_BALL_has);

	return NULL;
}

static PyObject *sipDo_Plane3_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Plane3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Plane3 *ptr;

			if ((ptr = (Plane3 *)sipGetCppPtr(sipThis,sipClass_Plane3)) == NULL)
				return NULL;

			res = ptr -> Plane3::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Plane3,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Plane3(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Plane3)
		return ptr;

	return NULL;
}

static void sipDealloc_Plane3(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (Plane3 *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_Plane3(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Plane3
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
			sipNew = new Plane3();
	}
	}

	if (sipNew == NULL)
	{
		const Plane3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_Plane3,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Plane3(a0obj,(Plane3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Plane3(* a0);
	}
	}

	if (sipNew == NULL)
	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj))
		{
			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Plane3(* a0,* a1);
	}
	}

	if (sipNew == NULL)
	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;
		const Vector3 *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"-III",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj,sipCanConvertTo_Vector3,&a2obj))
		{
			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);
			sipConvertTo_Vector3(a2obj,(Vector3 **)&a2,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Plane3(* a0,* a1,* a2);
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
			sipNew = new Plane3( a0, a1, a2, a3);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_Plane3);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Plane3,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (Plane3 *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Plane3[] = {
	{sipName_BALL_swap, sipDo_Plane3_swap, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_Plane3_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_Plane3_get, METH_VARARGS, NULL},
	{sipName_BALL_normalize, sipDo_Plane3_normalize, METH_VARARGS, NULL},
	{sipName_BALL_hessify, sipDo_Plane3_hessify, METH_VARARGS, NULL},
	{sipName_BALL_has, sipDo_Plane3_has, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_Plane3_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_Plane3(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Plane3);
}

void sipConvertTo_Plane3(PyObject *sipPy,Plane3 **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_Plane3);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Plane3 *)sipConvertToCpp(sipPy,sipClass_Plane3,sipIsErr);
}

Plane3 *sipForceConvertTo_Plane3(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Plane3(valobj))
	{
		Plane3 *val;

		sipConvertTo_Plane3(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Plane3);
	*iserrp = 1;

	return NULL;
}
