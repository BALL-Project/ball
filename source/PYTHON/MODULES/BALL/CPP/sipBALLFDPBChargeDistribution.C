#include "sipBALLDeclBALL.h"
#include "sipBALLFDPBChargeDistribution.h"


PyObject *sipClass_FDPB_ChargeDistribution;

static void sipDealloc_FDPB_ChargeDistribution(sipThisType *);

static PyTypeObject sipType_FDPB_ChargeDistribution = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_ChargeDistribution,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_FDPB_ChargeDistribution,
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

extern "C" const void *sipCast_FDPB_ChargeDistribution(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_FDPB_ChargeDistribution)
		return ptr;

	return NULL;
}

static void sipDealloc_FDPB_ChargeDistribution(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (FDPB::ChargeDistribution *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_FDPB_ChargeDistribution(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_FDPB_ChargeDistribution
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
			sipNew = new FDPB::ChargeDistribution();
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
		const FDPB::ChargeDistribution * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_FDPB_ChargeDistribution,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_FDPB_ChargeDistribution(a0obj,(FDPB::ChargeDistribution **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new FDPB::ChargeDistribution(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_ChargeDistribution);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_FDPB_ChargeDistribution,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (FDPB::ChargeDistribution *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_FDPB_ChargeDistribution[] = {
	{NULL}
};

int sipCanConvertTo_FDPB_ChargeDistribution(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_FDPB_ChargeDistribution);
}

void sipConvertTo_FDPB_ChargeDistribution(PyObject *sipPy,FDPB::ChargeDistribution **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_ChargeDistribution);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (FDPB::ChargeDistribution *)sipConvertToCpp(sipPy,sipClass_FDPB_ChargeDistribution,sipIsErr);
}

FDPB::ChargeDistribution *sipForceConvertTo_FDPB_ChargeDistribution(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_FDPB_ChargeDistribution(valobj))
	{
		FDPB::ChargeDistribution *val;

		sipConvertTo_FDPB_ChargeDistribution(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_ChargeDistribution);

	*iserrp = 1;

	return NULL;
}
