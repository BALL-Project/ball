#include "sipBALLDeclBALL.h"
#include "sipBALLFDPBDefault.h"


PyObject *sipClass_FDPB_Default;

static void sipDealloc_FDPB_Default(sipThisType *);

static PyTypeObject sipType_FDPB_Default = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Default,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_FDPB_Default,
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

extern "C" const void *sipCast_FDPB_Default(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_FDPB_Default)
		return ptr;

	return NULL;
}

static void sipDealloc_FDPB_Default(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (FDPB::Default *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_FDPB_Default(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_FDPB_Default
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
			sipNew = new FDPB::Default();
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
		const FDPB::Default * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_FDPB_Default,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_FDPB_Default(a0obj,(FDPB::Default **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new FDPB::Default(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_Default);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_FDPB_Default,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (FDPB::Default *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_FDPB_Default[] = {
	{NULL}
};

int sipCanConvertTo_FDPB_Default(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_FDPB_Default);
}

void sipConvertTo_FDPB_Default(PyObject *sipPy,FDPB::Default **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_Default);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (FDPB::Default *)sipConvertToCpp(sipPy,sipClass_FDPB_Default,sipIsErr);
}

FDPB::Default *sipForceConvertTo_FDPB_Default(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_FDPB_Default(valobj))
	{
		FDPB::Default *val;

		sipConvertTo_FDPB_Default(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Default);

	*iserrp = 1;

	return NULL;
}
