#include "sipBALLDeclBALL.h"
#include "sipBALLInvalidRange.h"


PyObject *sipClass_InvalidRange;

static void sipDealloc_InvalidRange(sipThisType *);

static PyTypeObject sipType_InvalidRange = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_InvalidRange,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_InvalidRange,
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

extern "C" const void *sipCast_InvalidRange(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_InvalidRange)
		return ptr;

	if ((res = sipCast_GeneralException((GeneralException *)(InvalidRange *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_InvalidRange(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (InvalidRange *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_InvalidRange(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_InvalidRange
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

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-si",&a0,&a1))
		{
   try
   {
			sipNew = new InvalidRange( a0, a1);
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
		const InvalidRange * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_InvalidRange,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_InvalidRange(a0obj,(InvalidRange **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new InvalidRange(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_InvalidRange);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_InvalidRange,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (InvalidRange *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_InvalidRange[] = {
	{NULL}
};

int sipCanConvertTo_InvalidRange(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_InvalidRange);
}

void sipConvertTo_InvalidRange(PyObject *sipPy,InvalidRange **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_InvalidRange);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (InvalidRange *)sipConvertToCpp(sipPy,sipClass_InvalidRange,sipIsErr);
}

InvalidRange *sipForceConvertTo_InvalidRange(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_InvalidRange(valobj))
	{
		InvalidRange *val;

		sipConvertTo_InvalidRange(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_InvalidRange);

	*iserrp = 1;

	return NULL;
}
