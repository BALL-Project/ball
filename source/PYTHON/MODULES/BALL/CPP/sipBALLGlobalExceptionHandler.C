#include "sipBALLDeclBALL.h"
#include "sipBALLGlobalExceptionHandler.h"


PyObject *sipClass_GlobalExceptionHandler;

static void sipDealloc_GlobalExceptionHandler(sipThisType *);

static PyTypeObject sipType_GlobalExceptionHandler = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_GlobalExceptionHandler,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_GlobalExceptionHandler,
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

static PyObject *sipDo_GlobalExceptionHandler_setName(PyObject *,PyObject *sipArgs)
{
	int sipArgsParsed = 0;

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_String,&a0obj))
		{

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			GlobalExceptionHandler::setName(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_GlobalExceptionHandler,sipName_BALL_setName);

	return NULL;
}

static PyObject *sipDo_GlobalExceptionHandler_setMessage(PyObject *,PyObject *sipArgs)
{
	int sipArgsParsed = 0;

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_String,&a0obj))
		{

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			GlobalExceptionHandler::setMessage(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_GlobalExceptionHandler,sipName_BALL_setMessage);

	return NULL;
}

static PyObject *sipDo_GlobalExceptionHandler_setLine(PyObject *,PyObject *sipArgs)
{
	int sipArgsParsed = 0;

	{
		int a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-i",&a0))
		{

   try
   {
			GlobalExceptionHandler::setLine( a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_GlobalExceptionHandler,sipName_BALL_setLine);

	return NULL;
}

static PyObject *sipDo_GlobalExceptionHandler_setFile(PyObject *,PyObject *sipArgs)
{
	int sipArgsParsed = 0;

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_String,&a0obj))
		{

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			GlobalExceptionHandler::setFile(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_GlobalExceptionHandler,sipName_BALL_setFile);

	return NULL;
}

static PyObject *sipDo_GlobalExceptionHandler_set(PyObject *,PyObject *sipArgs)
{
	int sipArgsParsed = 0;

	{
		const String * a0;
		PyObject *a0obj;
		int a1;
		const String * a2;
		PyObject *a2obj;
		const String * a3;
		PyObject *a3obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-IiII",sipCanConvertTo_String,&a0obj,&a1,sipCanConvertTo_String,&a2obj,sipCanConvertTo_String,&a3obj))
		{

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp2 = sipConvertTo_String(a2obj,(String **)&a2,1,&iserr);
			int istemp3 = sipConvertTo_String(a3obj,(String **)&a3,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			GlobalExceptionHandler::set(* a0, a1,* a2,* a3);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			if (istemp2)
				delete a2;

			if (istemp3)
				delete a3;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_GlobalExceptionHandler,sipName_BALL_set);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_GlobalExceptionHandler(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_GlobalExceptionHandler)
		return ptr;

	return NULL;
}

static void sipDealloc_GlobalExceptionHandler(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (GlobalExceptionHandler *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_GlobalExceptionHandler(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_GlobalExceptionHandler
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
			sipNew = new GlobalExceptionHandler();
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
		const GlobalExceptionHandler * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_GlobalExceptionHandler,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_GlobalExceptionHandler(a0obj,(GlobalExceptionHandler **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new GlobalExceptionHandler(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_GlobalExceptionHandler);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_GlobalExceptionHandler,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (GlobalExceptionHandler *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_GlobalExceptionHandler[] = {
	{sipName_BALL_setName, sipDo_GlobalExceptionHandler_setName, METH_VARARGS, NULL},
	{sipName_BALL_setMessage, sipDo_GlobalExceptionHandler_setMessage, METH_VARARGS, NULL},
	{sipName_BALL_setLine, sipDo_GlobalExceptionHandler_setLine, METH_VARARGS, NULL},
	{sipName_BALL_setFile, sipDo_GlobalExceptionHandler_setFile, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_GlobalExceptionHandler_set, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_GlobalExceptionHandler(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_GlobalExceptionHandler);
}

void sipConvertTo_GlobalExceptionHandler(PyObject *sipPy,GlobalExceptionHandler **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_GlobalExceptionHandler);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (GlobalExceptionHandler *)sipConvertToCpp(sipPy,sipClass_GlobalExceptionHandler,sipIsErr);
}

GlobalExceptionHandler *sipForceConvertTo_GlobalExceptionHandler(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_GlobalExceptionHandler(valobj))
	{
		GlobalExceptionHandler *val;

		sipConvertTo_GlobalExceptionHandler(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_GlobalExceptionHandler);

	*iserrp = 1;

	return NULL;
}
