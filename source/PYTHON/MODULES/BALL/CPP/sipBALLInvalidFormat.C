#include "sipBALLDeclBALL.h"
#include "sipBALLInvalidFormat.h"


PyObject *sipClass_InvalidFormat;

static void sipDealloc_InvalidFormat(sipThisType *);

static PyTypeObject sipType_InvalidFormat = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_InvalidFormat,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_InvalidFormat,
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

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_InvalidFormat(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_InvalidFormat)
		return ptr;

	if ((res = sipCast_GeneralException((GeneralException *)(InvalidFormat *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_InvalidFormat(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (InvalidFormat *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_InvalidFormat(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_InvalidFormat
	};

	sipThisType *sipThis = NULL;
	const void *sipNew = NULL;
	int sipFlags = SIP_PY_OWNED;
	int sipArgsParsed = 0;

	// See if there is something pending.

	sipNew = sipGetPending(&sipFlags);

	if (sipNew == NULL)
	{
		const char * a0;
		int a1;
		const String * a2;
		PyObject *a2obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-siI",&a0,&a1,sipCanConvertTo_String,&a2obj))
		{
			int iserr = 0;

			int istemp2 = sipConvertTo_String(a2obj,(String **)&a2,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new InvalidFormat( a0, a1,* a2);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp2)
				delete a2;
		}
	}

	if (sipNew == NULL)
	{
		const InvalidFormat * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_InvalidFormat,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_InvalidFormat(a0obj,(InvalidFormat **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new InvalidFormat(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_InvalidFormat);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_InvalidFormat,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (InvalidFormat *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_InvalidFormat[] = {
	{NULL}
};

int sipCanConvertTo_InvalidFormat(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_InvalidFormat);
}

void sipConvertTo_InvalidFormat(PyObject *sipPy,InvalidFormat **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_InvalidFormat);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (InvalidFormat *)sipConvertToCpp(sipPy,sipClass_InvalidFormat,sipIsErr);
}

InvalidFormat *sipForceConvertTo_InvalidFormat(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_InvalidFormat(valobj))
	{
		InvalidFormat *val;

		sipConvertTo_InvalidFormat(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_InvalidFormat);

	*iserrp = 1;

	return NULL;
}
