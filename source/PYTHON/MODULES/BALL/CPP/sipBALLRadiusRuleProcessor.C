#include "sipBALLDeclBALL.h"
#include "sipBALLRadiusRuleProcessor.h"


PyObject *sipClass_RadiusRuleProcessor;

static void sipDealloc_RadiusRuleProcessor(sipThisType *);

static PyTypeObject sipType_RadiusRuleProcessor = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_RadiusRuleProcessor,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_RadiusRuleProcessor,
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

sipRadiusRuleProcessor::sipRadiusRuleProcessor()
   : RadiusRuleProcessor()
{
	sipCommonCtor(sipPyMethods,3);
}

sipRadiusRuleProcessor::sipRadiusRuleProcessor(INIFile& a0,const String& a1)
   : RadiusRuleProcessor(a0,a1)
{
	sipCommonCtor(sipPyMethods,3);
}

sipRadiusRuleProcessor::sipRadiusRuleProcessor(const RadiusRuleProcessor& a0)
   : RadiusRuleProcessor(a0)
{
	sipCommonCtor(sipPyMethods,3);
}

sipRadiusRuleProcessor::~sipRadiusRuleProcessor()

{
	sipCommonDtor(sipPyThis);
}

bool sipRadiusRuleProcessor::finish()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_finish,&relLock) ?
		sipRuleProcessor::sipVH_finish(&sipPyMethods[0],sipPyThis,relLock) :
		RuleProcessor::finish();
}

bool sipRadiusRuleProcessor::start()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_start,&relLock) ?
		sipRuleProcessor::sipVH_start(&sipPyMethods[1],sipPyThis,relLock) :
		RuleProcessor::start();
}

Processor::Result sipRadiusRuleProcessor::operator()(Atom& a0)

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_CallOp,&relLock) ?
		sipAtomProcessor::sipVH_CallOp(&sipPyMethods[2],sipPyThis,relLock,a0) :
		RadiusRuleProcessor::operator()(a0);
}

static PyObject *sipDo_RadiusRuleProcessor_CallOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RadiusRuleProcessor)) == NULL)
		return NULL;

	{
		Atom * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			Processor::Result res;
			RadiusRuleProcessor *ptr;

			if ((ptr = (RadiusRuleProcessor *)sipGetCppPtr(sipThis,sipClass_RadiusRuleProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> RadiusRuleProcessor::operator()(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_RadiusRuleProcessor,sipName_BALL_CallOp);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_RadiusRuleProcessor(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_RadiusRuleProcessor)
		return ptr;

	if ((res = sipCast_RuleProcessor((RuleProcessor *)(RadiusRuleProcessor *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_RadiusRuleProcessor(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipRadiusRuleProcessor *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (RadiusRuleProcessor *)sipThis -> u.cppPtr;
			else
				delete (sipRadiusRuleProcessor *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_RadiusRuleProcessor(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_RadiusRuleProcessor
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
			sipNew = new sipRadiusRuleProcessor();
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
			sipNew = new sipRadiusRuleProcessor(* a0,* a1);
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
		const RadiusRuleProcessor * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_RadiusRuleProcessor,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_RadiusRuleProcessor(a0obj,(RadiusRuleProcessor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipRadiusRuleProcessor(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_RadiusRuleProcessor);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_RadiusRuleProcessor,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (RadiusRuleProcessor *)sipNew;
			else
				delete (sipRadiusRuleProcessor *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipRadiusRuleProcessor *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_RadiusRuleProcessor[] = {
	{sipName_BALL_CallOp, sipDo_RadiusRuleProcessor_CallOp, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_RadiusRuleProcessor(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_RadiusRuleProcessor);
}

void sipConvertTo_RadiusRuleProcessor(PyObject *sipPy,RadiusRuleProcessor **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_RadiusRuleProcessor);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (RadiusRuleProcessor *)sipConvertToCpp(sipPy,sipClass_RadiusRuleProcessor,sipIsErr);
}

RadiusRuleProcessor *sipForceConvertTo_RadiusRuleProcessor(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_RadiusRuleProcessor(valobj))
	{
		RadiusRuleProcessor *val;

		sipConvertTo_RadiusRuleProcessor(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_RadiusRuleProcessor);

	*iserrp = 1;

	return NULL;
}
