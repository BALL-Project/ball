#include "sipBALLDeclBALL.h"
#include "sipBALLGradient.h"


PyObject *sipClass_Gradient;

static void sipDealloc_Gradient(sipThisType *);

static PyTypeObject sipType_Gradient = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Gradient,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Gradient,
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

extern "C" PyObject *sipDo_Gradient_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Gradient)) == NULL)
		return NULL;

	{
		const Gradient * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Gradient,&a0obj))
		{
			Gradient *ptr;

			if ((ptr = (Gradient *)sipGetCppPtr(sipThis,sipClass_Gradient)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Gradient(a0obj,(Gradient **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Gradient::set(* a0);
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
		const AtomVector * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_AtomVector,&a0obj))
		{
			Gradient *ptr;

			if ((ptr = (Gradient *)sipGetCppPtr(sipThis,sipClass_Gradient)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_AtomVector(a0obj,(AtomVector **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Gradient::set(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Gradient,sipName_BALL_set);

	return NULL;
}

extern "C" PyObject *sipDo_Gradient_negate(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Gradient)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Gradient *ptr;

			if ((ptr = (Gradient *)sipGetCppPtr(sipThis,sipClass_Gradient)) == NULL)
				return NULL;

   try
   {
			ptr -> Gradient::negate();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Gradient,sipName_BALL_negate);

	return NULL;
}

extern "C" PyObject *sipDo_Gradient_normalize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Gradient)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Gradient *ptr;

			if ((ptr = (Gradient *)sipGetCppPtr(sipThis,sipClass_Gradient)) == NULL)
				return NULL;

   try
   {
			ptr -> Gradient::normalize();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Gradient,sipName_BALL_normalize);

	return NULL;
}

extern "C" PyObject *sipDo_Gradient_size(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Gradient)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			Gradient *ptr;

			if ((ptr = (Gradient *)sipGetCppPtr(sipThis,sipClass_Gradient)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Gradient::size();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Gradient,sipName_BALL_size);

	return NULL;
}

extern "C" PyObject *sipDo_Gradient_invalidate(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Gradient)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Gradient *ptr;

			if ((ptr = (Gradient *)sipGetCppPtr(sipThis,sipClass_Gradient)) == NULL)
				return NULL;

   try
   {
			ptr -> Gradient::invalidate();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Gradient,sipName_BALL_invalidate);

	return NULL;
}

extern "C" PyObject *sipDo_Gradient_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Gradient)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Gradient *ptr;

			if ((ptr = (Gradient *)sipGetCppPtr(sipThis,sipClass_Gradient)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Gradient::isValid();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Gradient,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_Gradient(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Gradient)
		return ptr;

	return NULL;
}

static void sipDealloc_Gradient(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (Gradient *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_Gradient(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Gradient
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
			sipNew = new Gradient();
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
		const AtomVector * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_AtomVector,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_AtomVector(a0obj,(AtomVector **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new Gradient(* a0);
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
		const Gradient * a0;
		PyObject *a0obj;
		long a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-Il",sipCanConvertTo_Gradient,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_Gradient(a0obj,(Gradient **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new Gradient(* a0, (bool)a1);
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
		const Gradient * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_Gradient,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Gradient(a0obj,(Gradient **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new Gradient(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_Gradient);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Gradient,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (Gradient *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Gradient_rms(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Gradient *ptr;

	if ((ptr = (Gradient *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Gradient)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> rms;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Gradient,sipName_BALL_rms);
		return NULL;
	}

	ptr -> rms = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Gradient_inv_norm(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Gradient *ptr;

	if ((ptr = (Gradient *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Gradient)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> inv_norm;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Gradient,sipName_BALL_inv_norm);
		return NULL;
	}

	ptr -> inv_norm = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Gradient_norm(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Gradient *ptr;

	if ((ptr = (Gradient *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Gradient)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> norm;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Gradient,sipName_BALL_norm);
		return NULL;
	}

	ptr -> norm = val;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Gradient[] = {
	{sipName_BALL_set, sipDo_Gradient_set, METH_VARARGS, NULL},
	{sipName_BALL_negate, sipDo_Gradient_negate, METH_VARARGS, NULL},
	{sipName_BALL_normalize, sipDo_Gradient_normalize, METH_VARARGS, NULL},
	{sipName_BALL_size, sipDo_Gradient_size, METH_VARARGS, NULL},
	{sipName_BALL_invalidate, sipDo_Gradient_invalidate, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_Gradient_isValid, METH_VARARGS, NULL},
	{NULL}
};

PyMethodDef sipClassVarTab_Gradient[] = {
	{sipName_BALL_rms, sipGetSetVar_Gradient_rms, 0, NULL},
	{sipName_BALL_inv_norm, sipGetSetVar_Gradient_inv_norm, 0, NULL},
	{sipName_BALL_norm, sipGetSetVar_Gradient_norm, 0, NULL},
	{NULL}
};

PyMethodDef *sipClassVarHierTab_Gradient[] = {
	sipClassVarTab_Gradient,
	NULL
};

int sipCanConvertTo_Gradient(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Gradient);
}

void sipConvertTo_Gradient(PyObject *sipPy,Gradient **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_Gradient);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Gradient *)sipConvertToCpp(sipPy,sipClass_Gradient,sipIsErr);
}

Gradient *sipForceConvertTo_Gradient(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Gradient(valobj))
	{
		Gradient *val;

		sipConvertTo_Gradient(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Gradient);

	*iserrp = 1;

	return NULL;
}
