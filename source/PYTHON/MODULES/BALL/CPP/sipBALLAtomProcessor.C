#include "sipBALLDeclBALL.h"
#include "sipBALLAtomProcessor.h"


PyObject *sipClass_AtomProcessor;

static void sipDealloc_AtomProcessor(sipThisType *);

static PyTypeObject sipType_AtomProcessor = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_AtomProcessor,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_AtomProcessor,
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

sipAtomProcessor::sipAtomProcessor()
   : AtomProcessor()
{
	sipCommonCtor(sipPyMethods,1);
}

sipAtomProcessor::sipAtomProcessor(const AtomProcessor& a0)
   : AtomProcessor(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipAtomProcessor::~sipAtomProcessor()

{
	sipCommonDtor(sipPyThis);
}

Processor::Result sipAtomProcessor::operator()(Atom& a0)

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_CallOp,&relLock) ?
		sipAtomProcessor::sipVH_CallOp(&sipPyMethods[0],sipPyThis,relLock,a0) :
		AtomProcessor::operator()(a0);
}

// The common handler for all classes that inherit this virtual member
// function.

Processor::Result sipAtomProcessor::sipVH_CallOp(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,Atom& a0)
{
	Processor::Result res;
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_Atom);

	sipArgs = Py_BuildValue("(ON)",sipThis -> sipSelf,a0obj);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		res = (Processor::Result)PyInt_AsLong(resobj);

		Py_DECREF(resobj);

		if (PyErr_Occurred() == NULL)
		{
			goto releaseLock;
		}

		sipBadVirtualResultType(sipName_BALL_AtomProcessor,sipName_BALL_CallOp);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

extern "C" PyObject *sipDo_AtomProcessor_start(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			AtomProcessor *ptr;

			if ((ptr = (AtomProcessor *)sipGetCppPtr(sipThis,sipClass_AtomProcessor)) == NULL)
				return NULL;

   try
   {
			res = ptr -> AtomProcessor::start();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomProcessor,sipName_BALL_start);

	return NULL;
}

extern "C" PyObject *sipDo_AtomProcessor_finish(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			AtomProcessor *ptr;

			if ((ptr = (AtomProcessor *)sipGetCppPtr(sipThis,sipClass_AtomProcessor)) == NULL)
				return NULL;

   try
   {
			res = ptr -> AtomProcessor::finish();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomProcessor,sipName_BALL_finish);

	return NULL;
}

extern "C" PyObject *sipDo_AtomProcessor_CallOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomProcessor)) == NULL)
		return NULL;

	{
		Atom * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			Processor::Result res;
			AtomProcessor *ptr;

			if ((ptr = (AtomProcessor *)sipGetCppPtr(sipThis,sipClass_AtomProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> AtomProcessor::operator()(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomProcessor,sipName_BALL_CallOp);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_AtomProcessor(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_AtomProcessor)
		return ptr;

	return NULL;
}

static void sipDealloc_AtomProcessor(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipAtomProcessor *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (AtomProcessor *)sipThis -> u.cppPtr;
			else
				delete (sipAtomProcessor *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_AtomProcessor(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_AtomProcessor
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
			sipNew = new sipAtomProcessor();
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
		const AtomProcessor * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_AtomProcessor,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_AtomProcessor(a0obj,(AtomProcessor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipAtomProcessor(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_AtomProcessor);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_AtomProcessor,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (AtomProcessor *)sipNew;
			else
				delete (sipAtomProcessor *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipAtomProcessor *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_AtomProcessor[] = {
	{sipName_BALL_start, sipDo_AtomProcessor_start, METH_VARARGS, NULL},
	{sipName_BALL_finish, sipDo_AtomProcessor_finish, METH_VARARGS, NULL},
	{sipName_BALL_CallOp, sipDo_AtomProcessor_CallOp, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_AtomProcessor(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_AtomProcessor);
}

void sipConvertTo_AtomProcessor(PyObject *sipPy,AtomProcessor **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_AtomProcessor);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (AtomProcessor *)sipConvertToCpp(sipPy,sipClass_AtomProcessor,sipIsErr);
}

AtomProcessor *sipForceConvertTo_AtomProcessor(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_AtomProcessor(valobj))
	{
		AtomProcessor *val;

		sipConvertTo_AtomProcessor(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_AtomProcessor);

	*iserrp = 1;

	return NULL;
}
