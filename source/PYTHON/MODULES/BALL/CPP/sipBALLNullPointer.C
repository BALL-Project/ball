#include "sipBALLDeclBALL.h"
#include "sipBALLNullPointer.h"


PyObject *sipClass_NullPointer;

static void sipDealloc_NullPointer(sipThisType *);

static PyTypeObject sipType_NullPointer = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_NullPointer,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_NullPointer,
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

extern "C" const void *sipCast_NullPointer(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_NullPointer)
		return ptr;

	if ((res = sipCast_GeneralException((GeneralException *)(NullPointer *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_NullPointer(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (NullPointer *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_NullPointer(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_NullPointer
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
			sipNew = new NullPointer( a0, a1);
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
		const NullPointer * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_NullPointer,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_NullPointer(a0obj,(NullPointer **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new NullPointer(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_NullPointer);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_NullPointer,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (NullPointer *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_NullPointer[] = {
	{NULL}
};

int sipCanConvertTo_NullPointer(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_NullPointer);
}

void sipConvertTo_NullPointer(PyObject *sipPy,NullPointer **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_NullPointer);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (NullPointer *)sipConvertToCpp(sipPy,sipClass_NullPointer,sipIsErr);
}

NullPointer *sipForceConvertTo_NullPointer(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_NullPointer(valobj))
	{
		NullPointer *val;

		sipConvertTo_NullPointer(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_NullPointer);

	*iserrp = 1;

	return NULL;
}
