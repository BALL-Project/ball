#include "sipBALLDeclBALL.h"
#include "sipBALLLine3.h"


PyObject *sipClass_Line3;

static void sipDealloc_Line3(sipThisType *);

static PyTypeObject sipType_Line3 = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Line3,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Line3,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	Py_TPFLAGS_DEFAULT,
	0,
	0,
	0,
};

static PyObject *sipDo_Line3_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Line3)) == NULL)
		return NULL;

	{
		Line3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Line3,&a0obj))
		{
			Line3 *ptr;

			if ((ptr = (Line3 *)sipGetCppPtr(sipThis,sipClass_Line3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Line3(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Line3::swap(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Line3,sipName_BALL_swap);

	return NULL;
}

static PyObject *sipDo_Line3_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Line3)) == NULL)
		return NULL;

	{
		const Line3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Line3,&a0obj))
		{
			Line3 *ptr;

			if ((ptr = (Line3 *)sipGetCppPtr(sipThis,sipClass_Line3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Line3(a0obj,(Line3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Line3::set(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const Vector3 * a0;
		PyObject *a0obj;
		const Vector3 * a1;
		PyObject *a1obj;
		long a2 = Line3::FORM__PARAMETER;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II|l",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj,&a2))
		{
			Line3 *ptr;

			if ((ptr = (Line3 *)sipGetCppPtr(sipThis,sipClass_Line3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Line3::set(* a0,* a1, (Line3::Form)a2);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Line3,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_Line3_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Line3)) == NULL)
		return NULL;

	{
		Line3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Line3,&a0obj))
		{
			Line3 *ptr;

			if ((ptr = (Line3 *)sipGetCppPtr(sipThis,sipClass_Line3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Line3(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Line3::get(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		Vector3 * a0;
		PyObject *a0obj;
		Vector3 * a1;
		PyObject *a1obj;
		long a2 = Line3::FORM__PARAMETER;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II|l",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj,&a2))
		{
			Line3 *ptr;

			if ((ptr = (Line3 *)sipGetCppPtr(sipThis,sipClass_Line3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Line3::get(* a0,* a1, (Line3::Form)a2);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Line3,sipName_BALL_get);

	return NULL;
}

static PyObject *sipDo_Line3_normalize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Line3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Line3 *ptr;

			if ((ptr = (Line3 *)sipGetCppPtr(sipThis,sipClass_Line3)) == NULL)
				return NULL;

   try
   {
			ptr -> Line3::normalize();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Line3,sipName_BALL_normalize);

	return NULL;
}

static PyObject *sipDo_Line3_has(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Line3)) == NULL)
		return NULL;

	{
		const Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			bool res;
			Line3 *ptr;

			if ((ptr = (Line3 *)sipGetCppPtr(sipThis,sipClass_Line3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Line3::has(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Line3,sipName_BALL_has);

	return NULL;
}

static PyObject *sipDo_Line3_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Line3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Line3 *ptr;

			if ((ptr = (Line3 *)sipGetCppPtr(sipThis,sipClass_Line3)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Line3::isValid();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Line3,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Line3(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Line3)
		return ptr;

	return NULL;
}

static void sipDealloc_Line3(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (Line3 *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_Line3(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Line3
	};

	sipThisType *sipThis = NULL;
	const void *sipNew = NULL;
	int sipFlags = SIP_PY_OWNED;
	int sipArgsParsed = 0;

	// See if there is something pending.

	sipNew = sipGetPending(&sipFlags);

	if (sipNew == NULL)
	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,"-"))
		{
   try
   {
			sipNew = new Line3();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}
		}
	}

	if (sipNew == NULL)
	{
		const Line3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_Line3,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Line3(a0obj,(Line3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new Line3(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}
		}
	}

	if (sipNew == NULL)
	{
		const Vector3 * a0;
		PyObject *a0obj;
		const Vector3 * a1;
		PyObject *a1obj;
		long a2 = Line3::FORM__PARAMETER;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-II|l",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj,&a2))
		{
			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new Line3(* a0,* a1, (Line3::Form)a2);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_Line3);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Line3,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (Line3 *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Line3_d(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Vector3 *val;
	Line3 *ptr;

	if ((ptr = (Line3 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Line3)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = &ptr -> d;

		valobj = sipMapCppToSelf(val,sipClass_Vector3);

		return valobj;
	}

	val = sipForceConvertTo_Vector3(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_Line3,sipName_BALL_d);
		return NULL;
	}

	ptr -> d = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Line3_p(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Vector3 *val;
	Line3 *ptr;

	if ((ptr = (Line3 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Line3)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = &ptr -> p;

		valobj = sipMapCppToSelf(val,sipClass_Vector3);

		return valobj;
	}

	val = sipForceConvertTo_Vector3(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_Line3,sipName_BALL_p);
		return NULL;
	}

	ptr -> p = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Line3[] = {
	{sipName_BALL_swap, sipDo_Line3_swap, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_Line3_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_Line3_get, METH_VARARGS, NULL},
	{sipName_BALL_normalize, sipDo_Line3_normalize, METH_VARARGS, NULL},
	{sipName_BALL_has, sipDo_Line3_has, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_Line3_isValid, METH_VARARGS, NULL},
	{NULL}
};

PyMethodDef sipClassVarTab_Line3[] = {
	{sipName_BALL_d, sipGetSetVar_Line3_d, 0, NULL},
	{sipName_BALL_p, sipGetSetVar_Line3_p, 0, NULL},
	{NULL}
};

PyMethodDef *sipClassVarHierTab_Line3[] = {
	sipClassVarTab_Line3,
	NULL
};

int sipCanConvertTo_Line3(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Line3);
}

void sipConvertTo_Line3(PyObject *sipPy,Line3 **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_Line3);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Line3 *)sipConvertToCpp(sipPy,sipClass_Line3,sipIsErr);
}

Line3 *sipForceConvertTo_Line3(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Line3(valobj))
	{
		Line3 *val;

		sipConvertTo_Line3(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Line3);

	*iserrp = 1;

	return NULL;
}
