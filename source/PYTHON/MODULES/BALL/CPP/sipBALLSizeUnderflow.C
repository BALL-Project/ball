#include "sipBALLDeclBALL.h"
#include "sipBALLSizeUnderflow.h"


PyObject *sipClass_SizeUnderflow;

static void sipDealloc_SizeUnderflow(sipThisType *);

static PyTypeObject sipType_SizeUnderflow = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_SizeUnderflow,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_SizeUnderflow,
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

extern "C" const void *sipCast_SizeUnderflow(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_SizeUnderflow)
		return ptr;

	if ((res = sipCast_GeneralException((GeneralException *)(SizeUnderflow *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_SizeUnderflow(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (SizeUnderflow *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_SizeUnderflow(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_SizeUnderflow
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
		int a2 = 0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-si|i",&a0,&a1,&a2))
		{
   try
   {
			sipNew = new SizeUnderflow( a0, a1, a2);
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
		const SizeUnderflow * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_SizeUnderflow,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_SizeUnderflow(a0obj,(SizeUnderflow **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new SizeUnderflow(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_SizeUnderflow);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_SizeUnderflow,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (SizeUnderflow *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_SizeUnderflow[] = {
	{NULL}
};

int sipCanConvertTo_SizeUnderflow(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_SizeUnderflow);
}

void sipConvertTo_SizeUnderflow(PyObject *sipPy,SizeUnderflow **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_SizeUnderflow);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (SizeUnderflow *)sipConvertToCpp(sipPy,sipClass_SizeUnderflow,sipIsErr);
}

SizeUnderflow *sipForceConvertTo_SizeUnderflow(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_SizeUnderflow(valobj))
	{
		SizeUnderflow *val;

		sipConvertTo_SizeUnderflow(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_SizeUnderflow);

	*iserrp = 1;

	return NULL;
}
