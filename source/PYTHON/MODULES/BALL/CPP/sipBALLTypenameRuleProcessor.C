#include "sipBALLDeclBALL.h"
#include "sipBALLTypenameRuleProcessor.h"


PyObject *sipClass_TypenameRuleProcessor;

static void sipDealloc_TypenameRuleProcessor(sipThisType *);

static PyTypeObject sipType_TypenameRuleProcessor = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_TypenameRuleProcessor,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_TypenameRuleProcessor,
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

sipTypenameRuleProcessor::sipTypenameRuleProcessor()
   : TypenameRuleProcessor()
{
	sipCommonCtor(sipPyMethods,3);
}

sipTypenameRuleProcessor::sipTypenameRuleProcessor(INIFile& a0,const String& a1)
   : TypenameRuleProcessor(a0,a1)
{
	sipCommonCtor(sipPyMethods,3);
}

sipTypenameRuleProcessor::sipTypenameRuleProcessor(const TypenameRuleProcessor& a0)
   : TypenameRuleProcessor(a0)
{
	sipCommonCtor(sipPyMethods,3);
}

sipTypenameRuleProcessor::~sipTypenameRuleProcessor()

{
	sipCommonDtor(sipPyThis);
}

bool sipTypenameRuleProcessor::finish()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_finish,&relLock) ?
		sipRuleProcessor::sipVH_finish(&sipPyMethods[0],sipPyThis,relLock) :
		RuleProcessor::finish();
}

bool sipTypenameRuleProcessor::start()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_start,&relLock) ?
		sipRuleProcessor::sipVH_start(&sipPyMethods[1],sipPyThis,relLock) :
		RuleProcessor::start();
}

Processor::Result sipTypenameRuleProcessor::operator()(Atom& a0)

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_CallOp,&relLock) ?
		sipAtomProcessor::sipVH_CallOp(&sipPyMethods[2],sipPyThis,relLock,a0) :
		TypenameRuleProcessor::operator()(a0);
}

extern "C" PyObject *sipDo_TypenameRuleProcessor_CallOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_TypenameRuleProcessor)) == NULL)
		return NULL;

	{
		Atom * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			Processor::Result res;
			TypenameRuleProcessor *ptr;

			if ((ptr = (TypenameRuleProcessor *)sipGetCppPtr(sipThis,sipClass_TypenameRuleProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> TypenameRuleProcessor::operator()(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_TypenameRuleProcessor,sipName_BALL_CallOp);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_TypenameRuleProcessor(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_TypenameRuleProcessor)
		return ptr;

	if ((res = sipCast_RuleProcessor((RuleProcessor *)(TypenameRuleProcessor *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_TypenameRuleProcessor(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipTypenameRuleProcessor *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (TypenameRuleProcessor *)sipThis -> u.cppPtr;
			else
				delete (sipTypenameRuleProcessor *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_TypenameRuleProcessor(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_TypenameRuleProcessor
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
			sipNew = new sipTypenameRuleProcessor();
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
			sipNew = new sipTypenameRuleProcessor(* a0,* a1);
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
		const TypenameRuleProcessor * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_TypenameRuleProcessor,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_TypenameRuleProcessor(a0obj,(TypenameRuleProcessor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipTypenameRuleProcessor(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_TypenameRuleProcessor);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_TypenameRuleProcessor,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (TypenameRuleProcessor *)sipNew;
			else
				delete (sipTypenameRuleProcessor *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipTypenameRuleProcessor *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_TypenameRuleProcessor[] = {
	{sipName_BALL_CallOp, sipDo_TypenameRuleProcessor_CallOp, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_TypenameRuleProcessor(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_TypenameRuleProcessor);
}

void sipConvertTo_TypenameRuleProcessor(PyObject *sipPy,TypenameRuleProcessor **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_TypenameRuleProcessor);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (TypenameRuleProcessor *)sipConvertToCpp(sipPy,sipClass_TypenameRuleProcessor,sipIsErr);
}

TypenameRuleProcessor *sipForceConvertTo_TypenameRuleProcessor(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_TypenameRuleProcessor(valobj))
	{
		TypenameRuleProcessor *val;

		sipConvertTo_TypenameRuleProcessor(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_TypenameRuleProcessor);

	*iserrp = 1;

	return NULL;
}
