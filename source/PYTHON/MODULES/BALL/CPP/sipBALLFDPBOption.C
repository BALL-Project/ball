#include "sipBALLDeclBALL.h"
#include "sipBALLFDPBOption.h"


PyObject *sipClass_FDPB_Option;

static void sipDealloc_FDPB_Option(sipThisType *);

static PyTypeObject sipType_FDPB_Option = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Option,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_FDPB_Option,
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

extern "C" const void *sipCast_FDPB_Option(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_FDPB_Option)
		return ptr;

	return NULL;
}

static void sipDealloc_FDPB_Option(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (FDPB::Option *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_FDPB_Option(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_FDPB_Option
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
			sipNew = new FDPB::Option();
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
		const FDPB::Option * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_FDPB_Option,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_FDPB_Option(a0obj,(FDPB::Option **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new FDPB::Option(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_Option);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_FDPB_Option,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (FDPB::Option *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_FDPB_Option[] = {
	{NULL}
};

int sipCanConvertTo_FDPB_Option(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_FDPB_Option);
}

void sipConvertTo_FDPB_Option(PyObject *sipPy,FDPB::Option **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_Option);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (FDPB::Option *)sipConvertToCpp(sipPy,sipClass_FDPB_Option,sipIsErr);
}

FDPB::Option *sipForceConvertTo_FDPB_Option(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_FDPB_Option(valobj))
	{
		FDPB::Option *val;

		sipConvertTo_FDPB_Option(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Option);

	*iserrp = 1;

	return NULL;
}
