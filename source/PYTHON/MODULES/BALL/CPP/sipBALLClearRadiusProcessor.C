#include "sipBALLDeclBALL.h"
#include "sipBALLClearRadiusProcessor.h"



PyObject *sipClass_ClearRadiusProcessor;

static void sipDealloc_ClearRadiusProcessor(sipThisType *);

static PyTypeObject sipType_ClearRadiusProcessor = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_ClearRadiusProcessor,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_ClearRadiusProcessor,
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

sipClearRadiusProcessor::sipClearRadiusProcessor()
    : ClearRadiusProcessor()
{
	sipCommonCtor(sipPyMethods,1);
}

sipClearRadiusProcessor::sipClearRadiusProcessor(const ClearRadiusProcessor& a0)
    : ClearRadiusProcessor(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipClearRadiusProcessor::~sipClearRadiusProcessor()
 
{
	sipCommonDtor(sipPyThis);
}

Processor::Result sipClearRadiusProcessor::operator ()(Atom& a0)

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL___call__,&relLock) ?
		sipAtomProcessor::sipVH_CallOperator(&sipPyMethods[0],sipPyThis,relLock,a0) :
		ClearRadiusProcessor::operator ()(a0);
}

static PyObject *sipDo_ClearRadiusProcessor___call__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ClearRadiusProcessor)) == NULL)
		return NULL;

	{
#line 26 "defaultProcessors.sip"
	printf("TEST\n");
	return 0;
#line 82 "sipBALLClearRadiusProcessor.cpp"
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ClearRadiusProcessor,sipName_BALL___call__);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_ClearRadiusProcessor(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_ClearRadiusProcessor)
		return ptr;

	if ((res = sipCast_AtomProcessor((AtomProcessor *)(ClearRadiusProcessor *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_ClearRadiusProcessor(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipClearRadiusProcessor *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (ClearRadiusProcessor *)sipThis -> u.cppPtr;
			else
				delete (sipClearRadiusProcessor *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_ClearRadiusProcessor(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_ClearRadiusProcessor
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
			sipNew = new sipClearRadiusProcessor();
		}
	}

	if (sipNew == NULL)
	{
		const ClearRadiusProcessor *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_ClearRadiusProcessor,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_ClearRadiusProcessor(a0obj,(ClearRadiusProcessor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipClearRadiusProcessor(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_ClearRadiusProcessor);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_ClearRadiusProcessor,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (ClearRadiusProcessor *)sipNew;
			else
				delete (sipClearRadiusProcessor *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipClearRadiusProcessor *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_ClearRadiusProcessor[] = {
	{sipName_BALL___call__, sipDo_ClearRadiusProcessor___call__, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_ClearRadiusProcessor(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_ClearRadiusProcessor);
}

void sipConvertTo_ClearRadiusProcessor(PyObject *sipPy,ClearRadiusProcessor **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_ClearRadiusProcessor);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (ClearRadiusProcessor *)sipConvertToCpp(sipPy,sipClass_ClearRadiusProcessor,sipIsErr);
}

ClearRadiusProcessor *sipForceConvertTo_ClearRadiusProcessor(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_ClearRadiusProcessor(valobj))
	{
		ClearRadiusProcessor *val;

		sipConvertTo_ClearRadiusProcessor(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_ClearRadiusProcessor);
	*iserrp = 1;

	return NULL;
}
