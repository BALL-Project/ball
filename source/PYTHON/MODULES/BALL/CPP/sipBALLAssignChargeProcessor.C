#include "sipBALLDeclBALL.h"
#include "sipBALLAssignChargeProcessor.h"



PyObject *sipClass_AssignChargeProcessor;

static void sipDealloc_AssignChargeProcessor(sipThisType *);

static PyTypeObject sipType_AssignChargeProcessor = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_AssignChargeProcessor,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_AssignChargeProcessor,
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

sipAssignChargeProcessor::sipAssignChargeProcessor()
    : AssignChargeProcessor()
{
	sipCommonCtor(sipPyMethods,3);
}

sipAssignChargeProcessor::sipAssignChargeProcessor(const String& a0)
    : AssignChargeProcessor(a0)
{
	sipCommonCtor(sipPyMethods,3);
}

sipAssignChargeProcessor::sipAssignChargeProcessor(const AssignChargeProcessor& a0)
    : AssignChargeProcessor(a0)
{
	sipCommonCtor(sipPyMethods,3);
}

sipAssignChargeProcessor::~sipAssignChargeProcessor()
 
{
	sipCommonDtor(sipPyThis);
}

bool sipAssignChargeProcessor::finish()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_finish,&relLock) ?
		sipAssignRadiusProcessor::sipVH_finish(&sipPyMethods[0],sipPyThis,relLock) :
		AssignRadiusProcessor::finish();
}

Processor::Result sipAssignChargeProcessor::operator ()(Atom& a0)

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL___call__,&relLock) ?
		sipAtomProcessor::sipVH_CallOperator(&sipPyMethods[1],sipPyThis,relLock,a0) :
		AssignChargeProcessor::operator ()(a0);
}

bool sipAssignChargeProcessor::start()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_start,&relLock) ?
		sipAssignRadiusProcessor::sipVH_start(&sipPyMethods[2],sipPyThis,relLock) :
		AssignChargeProcessor::start();
}

static PyObject *sipDo_AssignChargeProcessor_start(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AssignChargeProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			AssignChargeProcessor *ptr;

			if ((ptr = (AssignChargeProcessor *)sipGetCppPtr(sipThis,sipClass_AssignChargeProcessor)) == NULL)
				return NULL;

			res = ptr -> AssignChargeProcessor::start();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AssignChargeProcessor,sipName_BALL_start);

	return NULL;
}

static PyObject *sipDo_AssignChargeProcessor___call__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AssignChargeProcessor)) == NULL)
		return NULL;

	{
		Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			Processor::Result res;
			AssignChargeProcessor *ptr;

			if ((ptr = (AssignChargeProcessor *)sipGetCppPtr(sipThis,sipClass_AssignChargeProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> AssignChargeProcessor::operator ()(* a0);

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AssignChargeProcessor,sipName_BALL___call__);

	return NULL;
}

static PyObject *sipDo_AssignChargeProcessor_getTotalCharge(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AssignChargeProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			AssignChargeProcessor *ptr;

			if ((ptr = (AssignChargeProcessor *)sipGetCppPtr(sipThis,sipClass_AssignChargeProcessor)) == NULL)
				return NULL;

			res = ptr -> AssignChargeProcessor::getTotalCharge();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AssignChargeProcessor,sipName_BALL_getTotalCharge);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_AssignChargeProcessor(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_AssignChargeProcessor)
		return ptr;

	if ((res = sipCast_AssignRadiusProcessor((AssignRadiusProcessor *)(AssignChargeProcessor *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_AssignChargeProcessor(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipAssignChargeProcessor *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (AssignChargeProcessor *)sipThis -> u.cppPtr;
			else
				delete (sipAssignChargeProcessor *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_AssignChargeProcessor(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_AssignChargeProcessor
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
			sipNew = new sipAssignChargeProcessor();
		}
	}

	if (sipNew == NULL)
	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_String,&a0obj))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipAssignChargeProcessor(* a0);

			if (istemp0)
				delete a0;
		}
	}

	if (sipNew == NULL)
	{
		const AssignChargeProcessor *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_AssignChargeProcessor,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_AssignChargeProcessor(a0obj,(AssignChargeProcessor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipAssignChargeProcessor(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_AssignChargeProcessor);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_AssignChargeProcessor,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (AssignChargeProcessor *)sipNew;
			else
				delete (sipAssignChargeProcessor *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipAssignChargeProcessor *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_AssignChargeProcessor[] = {
	{sipName_BALL_start, sipDo_AssignChargeProcessor_start, METH_VARARGS, NULL},
	{sipName_BALL___call__, sipDo_AssignChargeProcessor___call__, METH_VARARGS, NULL},
	{sipName_BALL_getTotalCharge, sipDo_AssignChargeProcessor_getTotalCharge, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_AssignChargeProcessor(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_AssignChargeProcessor);
}

void sipConvertTo_AssignChargeProcessor(PyObject *sipPy,AssignChargeProcessor **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_AssignChargeProcessor);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (AssignChargeProcessor *)sipConvertToCpp(sipPy,sipClass_AssignChargeProcessor,sipIsErr);
}

AssignChargeProcessor *sipForceConvertTo_AssignChargeProcessor(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_AssignChargeProcessor(valobj))
	{
		AssignChargeProcessor *val;

		sipConvertTo_AssignChargeProcessor(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_AssignChargeProcessor);
	*iserrp = 1;

	return NULL;
}
