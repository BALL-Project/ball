#include "sipBALLDeclBALL.h"
#include "sipBALLTypeRuleProcessor.h"


PyObject *sipClass_TypeRuleProcessor;

static void sipDealloc_TypeRuleProcessor(sipThisType *);

static PyTypeObject sipType_TypeRuleProcessor = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_TypeRuleProcessor,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_TypeRuleProcessor,
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

sipTypeRuleProcessor::sipTypeRuleProcessor()
   : TypeRuleProcessor()
{
	sipCommonCtor(sipPyMethods,3);
}

sipTypeRuleProcessor::sipTypeRuleProcessor(INIFile& a0,const String& a1)
   : TypeRuleProcessor(a0,a1)
{
	sipCommonCtor(sipPyMethods,3);
}

sipTypeRuleProcessor::sipTypeRuleProcessor(const TypeRuleProcessor& a0)
   : TypeRuleProcessor(a0)
{
	sipCommonCtor(sipPyMethods,3);
}

sipTypeRuleProcessor::~sipTypeRuleProcessor()

{
	sipCommonDtor(sipPyThis);
}

bool sipTypeRuleProcessor::finish()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_finish,&relLock) ?
		sipRuleProcessor::sipVH_finish(&sipPyMethods[0],sipPyThis,relLock) :
		RuleProcessor::finish();
}

bool sipTypeRuleProcessor::start()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_start,&relLock) ?
		sipRuleProcessor::sipVH_start(&sipPyMethods[1],sipPyThis,relLock) :
		RuleProcessor::start();
}

Processor::Result sipTypeRuleProcessor::operator()(Atom& a0)

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_CallOp,&relLock) ?
		sipAtomProcessor::sipVH_CallOp(&sipPyMethods[2],sipPyThis,relLock,a0) :
		TypeRuleProcessor::operator()(a0);
}

extern "C" PyObject *sipDo_TypeRuleProcessor_CallOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_TypeRuleProcessor)) == NULL)
		return NULL;

	{
		Atom * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			Processor::Result res;
			TypeRuleProcessor *ptr;

			if ((ptr = (TypeRuleProcessor *)sipGetCppPtr(sipThis,sipClass_TypeRuleProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> TypeRuleProcessor::operator()(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_TypeRuleProcessor,sipName_BALL_CallOp);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_TypeRuleProcessor(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_TypeRuleProcessor)
		return ptr;

	if ((res = sipCast_RuleProcessor((RuleProcessor *)(TypeRuleProcessor *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_TypeRuleProcessor(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipTypeRuleProcessor *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (TypeRuleProcessor *)sipThis -> u.cppPtr;
			else
				delete (sipTypeRuleProcessor *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_TypeRuleProcessor(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_TypeRuleProcessor
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
			sipNew = new sipTypeRuleProcessor();
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
		const String * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-II",sipCanConvertTo_INIFile,&a0obj,sipCanConvertTo_String,&a1obj))
		{
			int iserr = 0;

			sipConvertTo_INIFile(a0obj,&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipTypeRuleProcessor(* a0,* a1);
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
		const TypeRuleProcessor * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_TypeRuleProcessor,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_TypeRuleProcessor(a0obj,(TypeRuleProcessor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipTypeRuleProcessor(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_TypeRuleProcessor);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_TypeRuleProcessor,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (TypeRuleProcessor *)sipNew;
			else
				delete (sipTypeRuleProcessor *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipTypeRuleProcessor *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_TypeRuleProcessor[] = {
	{sipName_BALL_CallOp, sipDo_TypeRuleProcessor_CallOp, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_TypeRuleProcessor(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_TypeRuleProcessor);
}

void sipConvertTo_TypeRuleProcessor(PyObject *sipPy,TypeRuleProcessor **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_TypeRuleProcessor);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (TypeRuleProcessor *)sipConvertToCpp(sipPy,sipClass_TypeRuleProcessor,sipIsErr);
}

TypeRuleProcessor *sipForceConvertTo_TypeRuleProcessor(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_TypeRuleProcessor(valobj))
	{
		TypeRuleProcessor *val;

		sipConvertTo_TypeRuleProcessor(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_TypeRuleProcessor);

	*iserrp = 1;

	return NULL;
}
