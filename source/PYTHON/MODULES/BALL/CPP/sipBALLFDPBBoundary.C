#include "sipBALLDeclBALL.h"
#include "sipBALLFDPBBoundary.h"


PyObject *sipClass_FDPB_Boundary;

static void sipDealloc_FDPB_Boundary(sipThisType *);

static PyTypeObject sipType_FDPB_Boundary = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Boundary,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_FDPB_Boundary,
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

extern "C" const void *sipCast_FDPB_Boundary(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_FDPB_Boundary)
		return ptr;

	return NULL;
}

static void sipDealloc_FDPB_Boundary(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (FDPB::Boundary *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_FDPB_Boundary(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_FDPB_Boundary
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
			sipNew = new FDPB::Boundary();
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
		const FDPB::Boundary * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_FDPB_Boundary,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_FDPB_Boundary(a0obj,(FDPB::Boundary **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new FDPB::Boundary(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_Boundary);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_FDPB_Boundary,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (FDPB::Boundary *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_FDPB_Boundary[] = {
	{NULL}
};

int sipCanConvertTo_FDPB_Boundary(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_FDPB_Boundary);
}

void sipConvertTo_FDPB_Boundary(PyObject *sipPy,FDPB::Boundary **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_Boundary);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (FDPB::Boundary *)sipConvertToCpp(sipPy,sipClass_FDPB_Boundary,sipIsErr);
}

FDPB::Boundary *sipForceConvertTo_FDPB_Boundary(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_FDPB_Boundary(valobj))
	{
		FDPB::Boundary *val;

		sipConvertTo_FDPB_Boundary(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Boundary);

	*iserrp = 1;

	return NULL;
}
