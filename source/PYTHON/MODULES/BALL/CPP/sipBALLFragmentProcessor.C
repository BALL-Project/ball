#include "sipBALLDeclBALL.h"
#include "sipBALLFragmentProcessor.h"



PyObject *sipClass_FragmentProcessor;

static void sipDealloc_FragmentProcessor(sipThisType *);

static PyTypeObject sipType_FragmentProcessor = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_FragmentProcessor,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_FragmentProcessor,
	0,
	0,
	0,
	0,
	0,
};

sipFragmentProcessor::sipFragmentProcessor(): FragmentProcessor()
{
	sipCommonCtor(sipPyMethods,1);
}

sipFragmentProcessor::sipFragmentProcessor(const FragmentProcessor& a0): FragmentProcessor(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipFragmentProcessor::~sipFragmentProcessor()
{
	sipCommonDtor(sipPyThis);
}
Processor::Result sipFragmentProcessor::operator()(Fragment& a0)
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_Operator__call__,&relLock) ?
		sipFragmentProcessor::sipVH_Operator__call__(&sipPyMethods[0],sipPyThis,relLock,a0) :
		FragmentProcessor::operator()(a0);
}

// The common handler for all classes that inherit this virtual member
// function.

Processor::Result sipFragmentProcessor::sipVH_Operator__call__(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,Fragment& a0)
{
	Processor::Result res;
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_Fragment);

	sipArgs = Py_BuildValue("(OO)",sipThis -> sipSelf,a0obj);

	Py_XDECREF(a0obj);

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

		sipBadVirtualResultType(sipName_BALL_FragmentProcessor,sipName_BALL_Operator__call__);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

static PyObject *sipDo_FragmentProcessor_start(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FragmentProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			FragmentProcessor *ptr;

			if ((ptr = (FragmentProcessor *)sipGetCppPtr(sipThis,sipClass_FragmentProcessor)) == NULL)
				return NULL;

			res = ptr -> FragmentProcessor::start();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FragmentProcessor,sipName_BALL_start);

	return NULL;
}

static PyObject *sipDo_FragmentProcessor_finish(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FragmentProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			FragmentProcessor *ptr;

			if ((ptr = (FragmentProcessor *)sipGetCppPtr(sipThis,sipClass_FragmentProcessor)) == NULL)
				return NULL;

			res = ptr -> FragmentProcessor::finish();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FragmentProcessor,sipName_BALL_finish);

	return NULL;
}

static PyObject *sipDo_FragmentProcessor_Operator__call__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FragmentProcessor)) == NULL)
		return NULL;

	{
		Fragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Fragment,&a0obj))
		{
			Processor::Result res;
			FragmentProcessor *ptr;

			if ((ptr = (FragmentProcessor *)sipGetCppPtr(sipThis,sipClass_FragmentProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Fragment(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> FragmentProcessor::operator()(* a0);

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FragmentProcessor,sipName_BALL_Operator__call__);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_FragmentProcessor(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_FragmentProcessor)
		return ptr;

	return NULL;
}

static void sipDealloc_FragmentProcessor(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipFragmentProcessor *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (FragmentProcessor *)sipThis -> u.cppPtr;
			else
				delete (sipFragmentProcessor *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_FragmentProcessor(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_FragmentProcessor
	};

	sipThisType *sipThis = NULL;
	const void *sipNew = NULL;
	int sipFlags = SIP_PY_OWNED;

	// See if there is something pending.

	sipNew = sipGetPending(&sipFlags);

	if (sipNew == NULL)
	{
		if (sipParseArgs(sipArgs,"-"))
		{
			sipNew = new sipFragmentProcessor();
	}
	}

	if (sipNew == NULL)
	{
		const FragmentProcessor *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_FragmentProcessor,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_FragmentProcessor(a0obj,(FragmentProcessor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipFragmentProcessor(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_FragmentProcessor);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_FragmentProcessor,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (FragmentProcessor *)sipNew;
			else
				delete (sipFragmentProcessor *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipFragmentProcessor *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_FragmentProcessor[] = {
	{sipName_BALL_start, sipDo_FragmentProcessor_start, METH_VARARGS, NULL},
	{sipName_BALL_finish, sipDo_FragmentProcessor_finish, METH_VARARGS, NULL},
	{sipName_BALL_Operator__call__, sipDo_FragmentProcessor_Operator__call__, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_FragmentProcessor(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_FragmentProcessor);
}

void sipConvertTo_FragmentProcessor(PyObject *sipPy,FragmentProcessor **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_FragmentProcessor);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (FragmentProcessor *)sipConvertToCpp(sipPy,sipClass_FragmentProcessor,sipIsErr);
}

FragmentProcessor *sipForceConvertTo_FragmentProcessor(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_FragmentProcessor(valobj))
	{
		FragmentProcessor *val;

		sipConvertTo_FragmentProcessor(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_FragmentProcessor);
	*iserrp = 1;

	return NULL;
}
