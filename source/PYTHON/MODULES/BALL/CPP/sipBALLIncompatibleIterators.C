#include "sipBALLDeclBALL.h"
#include "sipBALLIncompatibleIterators.h"


PyObject *sipClass_IncompatibleIterators;

static void sipDealloc_IncompatibleIterators(sipThisType *);

static PyTypeObject sipType_IncompatibleIterators = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_IncompatibleIterators,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_IncompatibleIterators,
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

extern "C" const void *sipCast_IncompatibleIterators(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_IncompatibleIterators)
		return ptr;

	if ((res = sipCast_GeneralException((GeneralException *)(IncompatibleIterators *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_IncompatibleIterators(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (IncompatibleIterators *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_IncompatibleIterators(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_IncompatibleIterators
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
			sipNew = new IncompatibleIterators( a0, a1);
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
		const IncompatibleIterators * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_IncompatibleIterators,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_IncompatibleIterators(a0obj,(IncompatibleIterators **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new IncompatibleIterators(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_IncompatibleIterators);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_IncompatibleIterators,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (IncompatibleIterators *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_IncompatibleIterators[] = {
	{NULL}
};

int sipCanConvertTo_IncompatibleIterators(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_IncompatibleIterators);
}

void sipConvertTo_IncompatibleIterators(PyObject *sipPy,IncompatibleIterators **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_IncompatibleIterators);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (IncompatibleIterators *)sipConvertToCpp(sipPy,sipClass_IncompatibleIterators,sipIsErr);
}

IncompatibleIterators *sipForceConvertTo_IncompatibleIterators(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_IncompatibleIterators(valobj))
	{
		IncompatibleIterators *val;

		sipConvertTo_IncompatibleIterators(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_IncompatibleIterators);

	*iserrp = 1;

	return NULL;
}
