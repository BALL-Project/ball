#include "sipBALLDeclBALL.h"
#include "sipBALLSurface.h"



PyObject *sipClass_Surface;

static void sipDealloc_Surface(sipThisType *);

static PyTypeObject sipType_Surface = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Surface,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Surface,
	0,
	0,
	0,
	0,
	0,
};

static PyObject *sipDo_Surface_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Surface)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Surface *ptr;

			if ((ptr = (Surface *)sipGetCppPtr(sipThis,sipClass_Surface)) == NULL)
				return NULL;

			ptr -> Surface::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Surface,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_Surface_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Surface)) == NULL)
		return NULL;

	{
		const Surface *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Surface,&a0obj))
		{
			Surface *ptr;

			if ((ptr = (Surface *)sipGetCppPtr(sipThis,sipClass_Surface)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Surface(a0obj,(Surface **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Surface::set(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Surface,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_Surface_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Surface)) == NULL)
		return NULL;

	{
		Surface *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Surface,&a0obj))
		{
			Surface *ptr;

			if ((ptr = (Surface *)sipGetCppPtr(sipThis,sipClass_Surface)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Surface(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Surface::get(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Surface,sipName_BALL_get);

	return NULL;
}

static PyObject *sipDo_Surface_readMSMSFile(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Surface)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		const String *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_String,&a0obj,sipCanConvertTo_String,&a1obj))
		{
			Surface *ptr;

			if ((ptr = (Surface *)sipGetCppPtr(sipThis,sipClass_Surface)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Surface::readMSMSFile(* a0,* a1);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Surface,sipName_BALL_readMSMSFile);

	return NULL;
}

static PyObject *sipDo_Surface_getArea(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Surface)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			Surface *ptr;

			if ((ptr = (Surface *)sipGetCppPtr(sipThis,sipClass_Surface)) == NULL)
				return NULL;

			res = ptr -> Surface::getArea();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Surface,sipName_BALL_getArea);

	return NULL;
}

static PyObject *sipDo_Surface_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Surface)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Surface *ptr;

			if ((ptr = (Surface *)sipGetCppPtr(sipThis,sipClass_Surface)) == NULL)
				return NULL;

			res = ptr -> Surface::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Surface,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Surface(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Surface)
		return ptr;

	return NULL;
}

static void sipDealloc_Surface(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (Surface *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_Surface(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Surface
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
			sipNew = new Surface();
	}
	}

	if (sipNew == NULL)
	{
		const Surface *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_Surface,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Surface(a0obj,(Surface **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Surface(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_Surface);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Surface,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (Surface *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Surface[] = {
	{sipName_BALL_clear, sipDo_Surface_clear, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_Surface_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_Surface_get, METH_VARARGS, NULL},
	{sipName_BALL_readMSMSFile, sipDo_Surface_readMSMSFile, METH_VARARGS, NULL},
	{sipName_BALL_getArea, sipDo_Surface_getArea, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_Surface_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_Surface(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Surface);
}

void sipConvertTo_Surface(PyObject *sipPy,Surface **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_Surface);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Surface *)sipConvertToCpp(sipPy,sipClass_Surface,sipIsErr);
}

Surface *sipForceConvertTo_Surface(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Surface(valobj))
	{
		Surface *val;

		sipConvertTo_Surface(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Surface);
	*iserrp = 1;

	return NULL;
}
