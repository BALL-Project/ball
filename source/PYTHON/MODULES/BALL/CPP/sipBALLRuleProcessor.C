#include "sipBALLDeclBALL.h"
#include "sipBALLRuleProcessor.h"


PyObject *sipClass_RuleProcessor;

static void sipDealloc_RuleProcessor(sipThisType *);

static PyTypeObject sipType_RuleProcessor = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_RuleProcessor,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_RuleProcessor,
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

sipRuleProcessor::sipRuleProcessor()
   : RuleProcessor()
{
	sipCommonCtor(sipPyMethods,3);
}

sipRuleProcessor::sipRuleProcessor(INIFile& a0,const String& a1)
   : RuleProcessor(a0,a1)
{
	sipCommonCtor(sipPyMethods,3);
}

sipRuleProcessor::sipRuleProcessor(const RuleProcessor& a0)
   : RuleProcessor(a0)
{
	sipCommonCtor(sipPyMethods,3);
}

sipRuleProcessor::~sipRuleProcessor()

{
	sipCommonDtor(sipPyThis);
}

Processor::Result sipRuleProcessor::operator()(Atom& a0)

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_CallOp,&relLock) ?
		sipAtomProcessor::sipVH_CallOp(&sipPyMethods[0],sipPyThis,relLock,a0) :
		RuleProcessor::operator()(a0);
}

bool sipRuleProcessor::finish()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_finish,&relLock) ?
		sipRuleProcessor::sipVH_finish(&sipPyMethods[1],sipPyThis,relLock) :
		RuleProcessor::finish();
}

bool sipRuleProcessor::start()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_start,&relLock) ?
		sipRuleProcessor::sipVH_start(&sipPyMethods[2],sipPyThis,relLock) :
		RuleProcessor::start();
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipRuleProcessor::sipVH_finish(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
{
	bool res;
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(O)",sipThis -> sipSelf);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		res = (bool)PyInt_AsLong(resobj);

		Py_DECREF(resobj);

		if (PyErr_Occurred() == NULL)
		{
			goto releaseLock;
		}

		sipBadVirtualResultType(sipName_BALL_RuleProcessor,sipName_BALL_finish);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipRuleProcessor::sipVH_start(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
{
	bool res;
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(O)",sipThis -> sipSelf);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		res = (bool)PyInt_AsLong(resobj);

		Py_DECREF(resobj);

		if (PyErr_Occurred() == NULL)
		{
			goto releaseLock;
		}

		sipBadVirtualResultType(sipName_BALL_RuleProcessor,sipName_BALL_start);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

static PyObject *sipDo_RuleProcessor_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RuleProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			RuleProcessor *ptr;

			if ((ptr = (RuleProcessor *)sipGetCppPtr(sipThis,sipClass_RuleProcessor)) == NULL)
				return NULL;

   try
   {
			ptr -> RuleProcessor::clear();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RuleProcessor,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_RuleProcessor_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RuleProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			RuleProcessor *ptr;

			if ((ptr = (RuleProcessor *)sipGetCppPtr(sipThis,sipClass_RuleProcessor)) == NULL)
				return NULL;

   try
   {
			ptr -> RuleProcessor::destroy();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RuleProcessor,sipName_BALL_destroy);

	return NULL;
}

static PyObject *sipDo_RuleProcessor_initialize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RuleProcessor)) == NULL)
		return NULL;

	{
		INIFile * a0;
		PyObject *a0obj;
		const String * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_INIFile,&a0obj,sipCanConvertTo_String,&a1obj))
		{
			bool res;
			RuleProcessor *ptr;

			if ((ptr = (RuleProcessor *)sipGetCppPtr(sipThis,sipClass_RuleProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_INIFile(a0obj,&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> RuleProcessor::initialize(* a0,* a1);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp1)
				delete a1;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RuleProcessor,sipName_BALL_initialize);

	return NULL;
}

static PyObject *sipDo_RuleProcessor_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RuleProcessor)) == NULL)
		return NULL;

	{
		const RuleProcessor * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_RuleProcessor,&a0obj))
		{
			RuleProcessor *ptr;

			if ((ptr = (RuleProcessor *)sipGetCppPtr(sipThis,sipClass_RuleProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_RuleProcessor(a0obj,(RuleProcessor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> RuleProcessor::set(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RuleProcessor,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_RuleProcessor_start(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RuleProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			RuleProcessor *ptr;

			if ((ptr = (RuleProcessor *)sipGetCppPtr(sipThis,sipClass_RuleProcessor)) == NULL)
				return NULL;

   try
   {
			res = ptr -> RuleProcessor::start();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RuleProcessor,sipName_BALL_start);

	return NULL;
}

static PyObject *sipDo_RuleProcessor_finish(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RuleProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			RuleProcessor *ptr;

			if ((ptr = (RuleProcessor *)sipGetCppPtr(sipThis,sipClass_RuleProcessor)) == NULL)
				return NULL;

   try
   {
			res = ptr -> RuleProcessor::finish();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RuleProcessor,sipName_BALL_finish);

	return NULL;
}

static PyObject *sipDo_RuleProcessor_CallOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RuleProcessor)) == NULL)
		return NULL;

	{
		Atom * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			Processor::Result res;
			RuleProcessor *ptr;

			if ((ptr = (RuleProcessor *)sipGetCppPtr(sipThis,sipClass_RuleProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> RuleProcessor::operator()(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_RuleProcessor,sipName_BALL_CallOp);

	return NULL;
}

static PyObject *sipDo_RuleProcessor_evaluate(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RuleProcessor)) == NULL)
		return NULL;

	{
		const Atom * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			String *res;
			RuleProcessor *ptr;

			if ((ptr = (RuleProcessor *)sipGetCppPtr(sipThis,sipClass_RuleProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,(Atom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = new String(ptr -> RuleProcessor::evaluate(* a0));
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipNewCppToSelf(res,sipClass_String,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RuleProcessor,sipName_BALL_evaluate);

	return NULL;
}

static PyObject *sipDo_RuleProcessor_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RuleProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			RuleProcessor *ptr;

			if ((ptr = (RuleProcessor *)sipGetCppPtr(sipThis,sipClass_RuleProcessor)) == NULL)
				return NULL;

   try
   {
			res = ptr -> RuleProcessor::isValid();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RuleProcessor,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_RuleProcessor(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_RuleProcessor)
		return ptr;

	if ((res = sipCast_AtomProcessor((AtomProcessor *)(RuleProcessor *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_RuleProcessor(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipRuleProcessor *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (RuleProcessor *)sipThis -> u.cppPtr;
			else
				delete (sipRuleProcessor *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_RuleProcessor(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_RuleProcessor
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
			sipNew = new sipRuleProcessor();
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
			sipNew = new sipRuleProcessor(* a0,* a1);
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
		const RuleProcessor * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_RuleProcessor,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_RuleProcessor(a0obj,(RuleProcessor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipRuleProcessor(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_RuleProcessor);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_RuleProcessor,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (RuleProcessor *)sipNew;
			else
				delete (sipRuleProcessor *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipRuleProcessor *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_RuleProcessor[] = {
	{sipName_BALL_clear, sipDo_RuleProcessor_clear, METH_VARARGS, NULL},
	{sipName_BALL_destroy, sipDo_RuleProcessor_destroy, METH_VARARGS, NULL},
	{sipName_BALL_initialize, sipDo_RuleProcessor_initialize, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_RuleProcessor_set, METH_VARARGS, NULL},
	{sipName_BALL_start, sipDo_RuleProcessor_start, METH_VARARGS, NULL},
	{sipName_BALL_finish, sipDo_RuleProcessor_finish, METH_VARARGS, NULL},
	{sipName_BALL_CallOp, sipDo_RuleProcessor_CallOp, METH_VARARGS, NULL},
	{sipName_BALL_evaluate, sipDo_RuleProcessor_evaluate, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_RuleProcessor_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_RuleProcessor(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_RuleProcessor);
}

void sipConvertTo_RuleProcessor(PyObject *sipPy,RuleProcessor **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_RuleProcessor);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (RuleProcessor *)sipConvertToCpp(sipPy,sipClass_RuleProcessor,sipIsErr);
}

RuleProcessor *sipForceConvertTo_RuleProcessor(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_RuleProcessor(valobj))
	{
		RuleProcessor *val;

		sipConvertTo_RuleProcessor(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_RuleProcessor);

	*iserrp = 1;

	return NULL;
}
