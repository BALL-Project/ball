#include "sipBALLDeclBALL.h"
#include "sipBALLstd__openmode.h"


PyObject *sipClass_std__openmode;

static void sipDealloc_std__openmode(sipThisType *);

static PyTypeObject sipType_std__openmode = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_std__openmode,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_std__openmode,
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

extern "C" const void *sipCast_std__openmode(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_std__openmode)
		return ptr;

	return NULL;
}

static void sipDealloc_std__openmode(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (std__openmode *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_std__openmode(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_std__openmode
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
			sipNew = new std__openmode();
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
		const std__openmode * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_std__openmode,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_std__openmode(a0obj,(std__openmode **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new std__openmode(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_std__openmode);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_std__openmode,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (std__openmode *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_std__openmode[] = {
	{NULL}
};

int sipCanConvertTo_std__openmode(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_std__openmode);
}

void sipConvertTo_std__openmode(PyObject *sipPy,std__openmode **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_std__openmode);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (std__openmode *)sipConvertToCpp(sipPy,sipClass_std__openmode,sipIsErr);
}

std__openmode *sipForceConvertTo_std__openmode(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_std__openmode(valobj))
	{
		std__openmode *val;

		sipConvertTo_std__openmode(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_std__openmode);

	*iserrp = 1;

	return NULL;
}
