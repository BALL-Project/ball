#include "sipBALLDeclBALL.h"
#include "sipBALLSphere3.h"


PyObject *sipClass_Sphere3;

static void sipDealloc_Sphere3(sipThisType *);

static PyTypeObject sipType_Sphere3 = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Sphere3,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Sphere3,
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

extern "C" PyObject *sipDo_Sphere3_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Sphere3)) == NULL)
		return NULL;

	{
		Sphere3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Sphere3,&a0obj))
		{
			Sphere3 *ptr;

			if ((ptr = (Sphere3 *)sipGetCppPtr(sipThis,sipClass_Sphere3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Sphere3(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Sphere3::swap(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Sphere3,sipName_BALL_swap);

	return NULL;
}

extern "C" PyObject *sipDo_Sphere3_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Sphere3)) == NULL)
		return NULL;

	{
		const Sphere3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Sphere3,&a0obj))
		{
			Sphere3 *ptr;

			if ((ptr = (Sphere3 *)sipGetCppPtr(sipThis,sipClass_Sphere3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Sphere3(a0obj,(Sphere3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Sphere3::set(* a0);
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
		float a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"If",sipCanConvertTo_Vector3,&a0obj,&a1))
		{
			Sphere3 *ptr;

			if ((ptr = (Sphere3 *)sipGetCppPtr(sipThis,sipClass_Sphere3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Sphere3::set(* a0, a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Sphere3,sipName_BALL_set);

	return NULL;
}

extern "C" PyObject *sipDo_Sphere3_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Sphere3)) == NULL)
		return NULL;

	{
		Sphere3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Sphere3,&a0obj))
		{
			Sphere3 *ptr;

			if ((ptr = (Sphere3 *)sipGetCppPtr(sipThis,sipClass_Sphere3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Sphere3(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Sphere3::get(* a0);
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
		float a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"If",sipCanConvertTo_Vector3,&a0obj,&a1))
		{
			Sphere3 *ptr;

			if ((ptr = (Sphere3 *)sipGetCppPtr(sipThis,sipClass_Sphere3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Sphere3::get(* a0, a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Sphere3,sipName_BALL_get);

	return NULL;
}

extern "C" PyObject *sipDo_Sphere3_has(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Sphere3)) == NULL)
		return NULL;

	{
		const Vector3 * a0;
		PyObject *a0obj;
		long a1 = false;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_Vector3,&a0obj,&a1))
		{
			bool res;
			Sphere3 *ptr;

			if ((ptr = (Sphere3 *)sipGetCppPtr(sipThis,sipClass_Sphere3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Sphere3::has(* a0, (bool)a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Sphere3,sipName_BALL_has);

	return NULL;
}

extern "C" PyObject *sipDo_Sphere3_isEmpty(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Sphere3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Sphere3 *ptr;

			if ((ptr = (Sphere3 *)sipGetCppPtr(sipThis,sipClass_Sphere3)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Sphere3::isEmpty();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Sphere3,sipName_BALL_isEmpty);

	return NULL;
}

extern "C" PyObject *sipDo_Sphere3_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Sphere3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Sphere3 *ptr;

			if ((ptr = (Sphere3 *)sipGetCppPtr(sipThis,sipClass_Sphere3)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Sphere3::isValid();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Sphere3,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_Sphere3(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Sphere3)
		return ptr;

	return NULL;
}

static void sipDealloc_Sphere3(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (Sphere3 *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_Sphere3(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Sphere3
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
			sipNew = new Sphere3();
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
		const Sphere3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_Sphere3,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Sphere3(a0obj,(Sphere3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new Sphere3(* a0);
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
		float a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-If",sipCanConvertTo_Vector3,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new Sphere3(* a0, a1);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_Sphere3);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Sphere3,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (Sphere3 *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Sphere3_radius(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Sphere3 *ptr;

	if ((ptr = (Sphere3 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Sphere3)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> radius;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Sphere3,sipName_BALL_radius);
		return NULL;
	}

	ptr -> radius = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Sphere3_p(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Vector3 *val;
	Sphere3 *ptr;

	if ((ptr = (Sphere3 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Sphere3)) == NULL)
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
		sipBadSetType(sipName_BALL_Sphere3,sipName_BALL_p);
		return NULL;
	}

	ptr -> p = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Sphere3[] = {
	{sipName_BALL_swap, sipDo_Sphere3_swap, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_Sphere3_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_Sphere3_get, METH_VARARGS, NULL},
	{sipName_BALL_has, sipDo_Sphere3_has, METH_VARARGS, NULL},
	{sipName_BALL_isEmpty, sipDo_Sphere3_isEmpty, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_Sphere3_isValid, METH_VARARGS, NULL},
	{NULL}
};

PyMethodDef sipClassVarTab_Sphere3[] = {
	{sipName_BALL_radius, sipGetSetVar_Sphere3_radius, 0, NULL},
	{sipName_BALL_p, sipGetSetVar_Sphere3_p, 0, NULL},
	{NULL}
};

PyMethodDef *sipClassVarHierTab_Sphere3[] = {
	sipClassVarTab_Sphere3,
	NULL
};

int sipCanConvertTo_Sphere3(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Sphere3);
}

void sipConvertTo_Sphere3(PyObject *sipPy,Sphere3 **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_Sphere3);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Sphere3 *)sipConvertToCpp(sipPy,sipClass_Sphere3,sipIsErr);
}

Sphere3 *sipForceConvertTo_Sphere3(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Sphere3(valobj))
	{
		Sphere3 *val;

		sipConvertTo_Sphere3(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Sphere3);

	*iserrp = 1;

	return NULL;
}
