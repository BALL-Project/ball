#include "sipBALLDeclBALL.h"
#include "sipBALLChargeRuleProcessor.h"


PyObject *sipClass_ChargeRuleProcessor;

static void sipDealloc_ChargeRuleProcessor(sipThisType *);

static PyTypeObject sipType_ChargeRuleProcessor = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_ChargeRuleProcessor,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_ChargeRuleProcessor,
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

sipChargeRuleProcessor::sipChargeRuleProcessor()
   : ChargeRuleProcessor()
{
	sipCommonCtor(sipPyMethods,3);
}

sipChargeRuleProcessor::sipChargeRuleProcessor(INIFile& a0,const String& a1)
   : ChargeRuleProcessor(a0,a1)
{
	sipCommonCtor(sipPyMethods,3);
}

sipChargeRuleProcessor::sipChargeRuleProcessor(const ChargeRuleProcessor& a0)
   : ChargeRuleProcessor(a0)
{
	sipCommonCtor(sipPyMethods,3);
}

sipChargeRuleProcessor::~sipChargeRuleProcessor()

{
	sipCommonDtor(sipPyThis);
}

bool sipChargeRuleProcessor::finish()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_finish,&relLock) ?
		sipRuleProcessor::sipVH_finish(&sipPyMethods[0],sipPyThis,relLock) :
		RuleProcessor::finish();
}

bool sipChargeRuleProcessor::start()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_start,&relLock) ?
		sipRuleProcessor::sipVH_start(&sipPyMethods[1],sipPyThis,relLock) :
		RuleProcessor::start();
}

Processor::Result sipChargeRuleProcessor::operator()(Atom& a0)

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_CallOp,&relLock) ?
		sipAtomProcessor::sipVH_CallOp(&sipPyMethods[2],sipPyThis,relLock,a0) :
		ChargeRuleProcessor::operator()(a0);
}

static PyObject *sipDo_ChargeRuleProcessor_CallOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ChargeRuleProcessor)) == NULL)
		return NULL;

	{
		Atom * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			Processor::Result res;
			ChargeRuleProcessor *ptr;

			if ((ptr = (ChargeRuleProcessor *)sipGetCppPtr(sipThis,sipClass_ChargeRuleProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> ChargeRuleProcessor::operator()(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ChargeRuleProcessor,sipName_BALL_CallOp);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_ChargeRuleProcessor(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_ChargeRuleProcessor)
		return ptr;

	if ((res = sipCast_RuleProcessor((RuleProcessor *)(ChargeRuleProcessor *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_ChargeRuleProcessor(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipChargeRuleProcessor *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (ChargeRuleProcessor *)sipThis -> u.cppPtr;
			else
				delete (sipChargeRuleProcessor *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_ChargeRuleProcessor(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_ChargeRuleProcessor
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
			sipNew = new sipChargeRuleProcessor();
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
		INIFile * a0;
		PyObject *a0obj;
		const String * a1 = NULL;
		PyObject *a1obj = NULL;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|I",sipCanConvertTo_INIFile,&a0obj,sipCanConvertTo_String,&a1obj))
		{
			int iserr = 0;

			sipConvertTo_INIFile(a0obj,&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipChargeRuleProcessor(* a0,* a1);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp1)
				delete a1;
		}
	}

	if (sipNew == NULL)
	{
		const ChargeRuleProcessor * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_ChargeRuleProcessor,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_ChargeRuleProcessor(a0obj,(ChargeRuleProcessor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipChargeRuleProcessor(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_ChargeRuleProcessor);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_ChargeRuleProcessor,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (ChargeRuleProcessor *)sipNew;
			else
				delete (sipChargeRuleProcessor *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipChargeRuleProcessor *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_ChargeRuleProcessor[] = {
	{sipName_BALL_CallOp, sipDo_ChargeRuleProcessor_CallOp, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_ChargeRuleProcessor(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_ChargeRuleProcessor);
}

void sipConvertTo_ChargeRuleProcessor(PyObject *sipPy,ChargeRuleProcessor **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_ChargeRuleProcessor);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (ChargeRuleProcessor *)sipConvertToCpp(sipPy,sipClass_ChargeRuleProcessor,sipIsErr);
}

ChargeRuleProcessor *sipForceConvertTo_ChargeRuleProcessor(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_ChargeRuleProcessor(valobj))
	{
		ChargeRuleProcessor *val;

		sipConvertTo_ChargeRuleProcessor(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_ChargeRuleProcessor);

	*iserrp = 1;

	return NULL;
}
