#include "sipBALLDeclBALL.h"
#include "sipBALLNormalizeNamesProcessor.h"



PyObject *sipClass_NormalizeNamesProcessor;

static void sipDealloc_NormalizeNamesProcessor(sipThisType *);

static PyTypeObject sipType_NormalizeNamesProcessor = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_NormalizeNamesProcessor,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_NormalizeNamesProcessor,
	0,
	0,
	0,
	0,
	0,
};

sipNormalizeNamesProcessor::sipNormalizeNamesProcessor(): NormalizeNamesProcessor()
{
	sipCommonCtor(sipPyMethods,3);
}

sipNormalizeNamesProcessor::sipNormalizeNamesProcessor(FragmentDB& a0): NormalizeNamesProcessor(a0)
{
	sipCommonCtor(sipPyMethods,3);
}

sipNormalizeNamesProcessor::sipNormalizeNamesProcessor(const NormalizeNamesProcessor& a0): NormalizeNamesProcessor(a0)
{
	sipCommonCtor(sipPyMethods,3);
}

sipNormalizeNamesProcessor::~sipNormalizeNamesProcessor()
{
	sipCommonDtor(sipPyThis);
}
bool sipNormalizeNamesProcessor::start()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_start,&relLock) ?
		sipNormalizeNamesProcessor::sipVH_start(&sipPyMethods[0],sipPyThis,relLock) :
		NormalizeNamesProcessor::start();
}
bool sipNormalizeNamesProcessor::finish()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_finish,&relLock) ?
		sipNormalizeNamesProcessor::sipVH_finish(&sipPyMethods[1],sipPyThis,relLock) :
		NormalizeNamesProcessor::finish();
}
Processor::Result sipNormalizeNamesProcessor::operator()(Fragment& a0)
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_Operator__call__,&relLock) ?
		sipFragmentProcessor::sipVH_Operator__call__(&sipPyMethods[2],sipPyThis,relLock,a0) :
		NormalizeNamesProcessor::operator()(a0);
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipNormalizeNamesProcessor::sipVH_start(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_NormalizeNamesProcessor,sipName_BALL_start);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipNormalizeNamesProcessor::sipVH_finish(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_NormalizeNamesProcessor,sipName_BALL_finish);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

static PyObject *sipDo_NormalizeNamesProcessor_setFragmentDB(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_NormalizeNamesProcessor)) == NULL)
		return NULL;

	{
		FragmentDB *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_FragmentDB,&a0obj))
		{
			NormalizeNamesProcessor *ptr;

			if ((ptr = (NormalizeNamesProcessor *)sipGetCppPtr(sipThis,sipClass_NormalizeNamesProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_FragmentDB(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> NormalizeNamesProcessor::setFragmentDB(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_NormalizeNamesProcessor,sipName_BALL_setFragmentDB);

	return NULL;
}

static PyObject *sipDo_NormalizeNamesProcessor_setNamingStandard(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_NormalizeNamesProcessor)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			NormalizeNamesProcessor *ptr;

			if ((ptr = (NormalizeNamesProcessor *)sipGetCppPtr(sipThis,sipClass_NormalizeNamesProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> NormalizeNamesProcessor::setNamingStandard(* a0);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_NormalizeNamesProcessor,sipName_BALL_setNamingStandard);

	return NULL;
}

static PyObject *sipDo_NormalizeNamesProcessor_getNamingStandard(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_NormalizeNamesProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			const String *res;
			NormalizeNamesProcessor *ptr;

			if ((ptr = (NormalizeNamesProcessor *)sipGetCppPtr(sipThis,sipClass_NormalizeNamesProcessor)) == NULL)
				return NULL;

			res = &ptr -> NormalizeNamesProcessor::getNamingStandard();

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_NormalizeNamesProcessor,sipName_BALL_getNamingStandard);

	return NULL;
}

static PyObject *sipDo_NormalizeNamesProcessor_start(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_NormalizeNamesProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			NormalizeNamesProcessor *ptr;

			if ((ptr = (NormalizeNamesProcessor *)sipGetCppPtr(sipThis,sipClass_NormalizeNamesProcessor)) == NULL)
				return NULL;

			res = ptr -> NormalizeNamesProcessor::start();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_NormalizeNamesProcessor,sipName_BALL_start);

	return NULL;
}

static PyObject *sipDo_NormalizeNamesProcessor_finish(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_NormalizeNamesProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			NormalizeNamesProcessor *ptr;

			if ((ptr = (NormalizeNamesProcessor *)sipGetCppPtr(sipThis,sipClass_NormalizeNamesProcessor)) == NULL)
				return NULL;

			res = ptr -> NormalizeNamesProcessor::finish();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_NormalizeNamesProcessor,sipName_BALL_finish);

	return NULL;
}

static PyObject *sipDo_NormalizeNamesProcessor_Operator__call__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_NormalizeNamesProcessor)) == NULL)
		return NULL;

	{
		Fragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Fragment,&a0obj))
		{
			Processor::Result res;
			NormalizeNamesProcessor *ptr;

			if ((ptr = (NormalizeNamesProcessor *)sipGetCppPtr(sipThis,sipClass_NormalizeNamesProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Fragment(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> NormalizeNamesProcessor::operator()(* a0);

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_NormalizeNamesProcessor,sipName_BALL_Operator__call__);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_NormalizeNamesProcessor(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_NormalizeNamesProcessor)
		return ptr;

	if ((res = sipCast_FragmentProcessor((FragmentProcessor *)(NormalizeNamesProcessor *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_NormalizeNamesProcessor(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipNormalizeNamesProcessor *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (NormalizeNamesProcessor *)sipThis -> u.cppPtr;
			else
				delete (sipNormalizeNamesProcessor *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_NormalizeNamesProcessor(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_NormalizeNamesProcessor
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
			sipNew = new sipNormalizeNamesProcessor();
	}
	}

	if (sipNew == NULL)
	{
		FragmentDB *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_FragmentDB,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_FragmentDB(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipNormalizeNamesProcessor(* a0);
	}
	}

	if (sipNew == NULL)
	{
		const NormalizeNamesProcessor *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_NormalizeNamesProcessor,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_NormalizeNamesProcessor(a0obj,(NormalizeNamesProcessor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipNormalizeNamesProcessor(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_NormalizeNamesProcessor);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_NormalizeNamesProcessor,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (NormalizeNamesProcessor *)sipNew;
			else
				delete (sipNormalizeNamesProcessor *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipNormalizeNamesProcessor *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_NormalizeNamesProcessor[] = {
	{sipName_BALL_setFragmentDB, sipDo_NormalizeNamesProcessor_setFragmentDB, METH_VARARGS, NULL},
	{sipName_BALL_setNamingStandard, sipDo_NormalizeNamesProcessor_setNamingStandard, METH_VARARGS, NULL},
	{sipName_BALL_getNamingStandard, sipDo_NormalizeNamesProcessor_getNamingStandard, METH_VARARGS, NULL},
	{sipName_BALL_start, sipDo_NormalizeNamesProcessor_start, METH_VARARGS, NULL},
	{sipName_BALL_finish, sipDo_NormalizeNamesProcessor_finish, METH_VARARGS, NULL},
	{sipName_BALL_Operator__call__, sipDo_NormalizeNamesProcessor_Operator__call__, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_NormalizeNamesProcessor(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_NormalizeNamesProcessor);
}

void sipConvertTo_NormalizeNamesProcessor(PyObject *sipPy,NormalizeNamesProcessor **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_NormalizeNamesProcessor);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (NormalizeNamesProcessor *)sipConvertToCpp(sipPy,sipClass_NormalizeNamesProcessor,sipIsErr);
}

NormalizeNamesProcessor *sipForceConvertTo_NormalizeNamesProcessor(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_NormalizeNamesProcessor(valobj))
	{
		NormalizeNamesProcessor *val;

		sipConvertTo_NormalizeNamesProcessor(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_NormalizeNamesProcessor);
	*iserrp = 1;

	return NULL;
}
