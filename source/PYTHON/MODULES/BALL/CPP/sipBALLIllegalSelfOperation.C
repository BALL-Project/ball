#include "sipBALLDeclBALL.h"
#include "sipBALLIllegalSelfOperation.h"


PyObject *sipClass_IllegalSelfOperation;

static void sipDealloc_IllegalSelfOperation(sipThisType *);

static PyTypeObject sipType_IllegalSelfOperation = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_IllegalSelfOperation,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_IllegalSelfOperation,
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

extern "C" const void *sipCast_IllegalSelfOperation(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_IllegalSelfOperation)
		return ptr;

	if ((res = sipCast_GeneralException((GeneralException *)(IllegalSelfOperation *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_IllegalSelfOperation(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (IllegalSelfOperation *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_IllegalSelfOperation(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_IllegalSelfOperation
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
			sipNew = new IllegalSelfOperation( a0, a1);
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
		const IllegalSelfOperation * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_IllegalSelfOperation,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_IllegalSelfOperation(a0obj,(IllegalSelfOperation **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new IllegalSelfOperation(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_IllegalSelfOperation);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_IllegalSelfOperation,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (IllegalSelfOperation *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_IllegalSelfOperation[] = {
	{NULL}
};

int sipCanConvertTo_IllegalSelfOperation(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_IllegalSelfOperation);
}

void sipConvertTo_IllegalSelfOperation(PyObject *sipPy,IllegalSelfOperation **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_IllegalSelfOperation);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (IllegalSelfOperation *)sipConvertToCpp(sipPy,sipClass_IllegalSelfOperation,sipIsErr);
}

IllegalSelfOperation *sipForceConvertTo_IllegalSelfOperation(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_IllegalSelfOperation(valobj))
	{
		IllegalSelfOperation *val;

		sipConvertTo_IllegalSelfOperation(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_IllegalSelfOperation);

	*iserrp = 1;

	return NULL;
}
