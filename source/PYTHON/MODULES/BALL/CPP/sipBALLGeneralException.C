#include "sipBALLDeclBALL.h"
#include "sipBALLGeneralException.h"


PyObject *sipClass_GeneralException;

static void sipDealloc_GeneralException(sipThisType *);

static PyTypeObject sipType_GeneralException = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_GeneralException,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_GeneralException,
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

static PyObject *sipDo_GeneralException_getName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_GeneralException)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const char *res;
			GeneralException *ptr;

			if ((ptr = (GeneralException *)sipGetCppPtr(sipThis,sipClass_GeneralException)) == NULL)
				return NULL;

   try
   {
			res = ptr -> GeneralException::getName();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (res == NULL)
			{
				Py_INCREF(Py_None);
				return Py_None;
			}

			return PyString_FromString(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_GeneralException,sipName_BALL_getName);

	return NULL;
}

static PyObject *sipDo_GeneralException_getMessage(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_GeneralException)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const char *res;
			GeneralException *ptr;

			if ((ptr = (GeneralException *)sipGetCppPtr(sipThis,sipClass_GeneralException)) == NULL)
				return NULL;

   try
   {
			res = ptr -> GeneralException::getMessage();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (res == NULL)
			{
				Py_INCREF(Py_None);
				return Py_None;
			}

			return PyString_FromString(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_GeneralException,sipName_BALL_getMessage);

	return NULL;
}

static PyObject *sipDo_GeneralException_getLine(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_GeneralException)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			GeneralException *ptr;

			if ((ptr = (GeneralException *)sipGetCppPtr(sipThis,sipClass_GeneralException)) == NULL)
				return NULL;

   try
   {
			res = ptr -> GeneralException::getLine();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_GeneralException,sipName_BALL_getLine);

	return NULL;
}

static PyObject *sipDo_GeneralException_getFile(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_GeneralException)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const char *res;
			GeneralException *ptr;

			if ((ptr = (GeneralException *)sipGetCppPtr(sipThis,sipClass_GeneralException)) == NULL)
				return NULL;

   try
   {
			res = ptr -> GeneralException::getFile();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (res == NULL)
			{
				Py_INCREF(Py_None);
				return Py_None;
			}

			return PyString_FromString(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_GeneralException,sipName_BALL_getFile);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_GeneralException(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_GeneralException)
		return ptr;

	return NULL;
}

static void sipDealloc_GeneralException(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (GeneralException *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_GeneralException(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_GeneralException
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
			sipNew = new GeneralException();
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
		const char * a0;
		int a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-si",&a0,&a1))
		{
   try
   {
			sipNew = new GeneralException( a0, a1);
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
		const char * a0;
		int a1;
		const String * a2;
		PyObject *a2obj;
		const String * a3;
		PyObject *a3obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-siII",&a0,&a1,sipCanConvertTo_String,&a2obj,sipCanConvertTo_String,&a3obj))
		{
			int iserr = 0;

			int istemp2 = sipConvertTo_String(a2obj,(String **)&a2,1,&iserr);
			int istemp3 = sipConvertTo_String(a3obj,(String **)&a3,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new GeneralException( a0, a1,* a2,* a3);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp2)
				delete a2;

			if (istemp3)
				delete a3;
		}
	}

	if (sipNew == NULL)
	{
		const GeneralException * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_GeneralException,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_GeneralException(a0obj,(GeneralException **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new GeneralException(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_GeneralException);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_GeneralException,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (GeneralException *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_GeneralException[] = {
	{sipName_BALL_getName, sipDo_GeneralException_getName, METH_VARARGS, NULL},
	{sipName_BALL_getMessage, sipDo_GeneralException_getMessage, METH_VARARGS, NULL},
	{sipName_BALL_getLine, sipDo_GeneralException_getLine, METH_VARARGS, NULL},
	{sipName_BALL_getFile, sipDo_GeneralException_getFile, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_GeneralException(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_GeneralException);
}

void sipConvertTo_GeneralException(PyObject *sipPy,GeneralException **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_GeneralException);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (GeneralException *)sipConvertToCpp(sipPy,sipClass_GeneralException,sipIsErr);
}

GeneralException *sipForceConvertTo_GeneralException(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_GeneralException(valobj))
	{
		GeneralException *val;

		sipConvertTo_GeneralException(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_GeneralException);

	*iserrp = 1;

	return NULL;
}
