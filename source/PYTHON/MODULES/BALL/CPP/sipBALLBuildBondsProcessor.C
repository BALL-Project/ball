#include "sipBALLDeclBALL.h"
#include "sipBALLBuildBondsProcessor.h"



PyObject *sipClass_BuildBondsProcessor;

static void sipDealloc_BuildBondsProcessor(sipThisType *);

static PyTypeObject sipType_BuildBondsProcessor = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_BuildBondsProcessor,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_BuildBondsProcessor,
	0,
	0,
	0,
	0,
	0,
};

sipBuildBondsProcessor::sipBuildBondsProcessor(): BuildBondsProcessor()
{
	sipCommonCtor(sipPyMethods,3);
}

sipBuildBondsProcessor::sipBuildBondsProcessor(const FragmentDB& a0): BuildBondsProcessor(a0)
{
	sipCommonCtor(sipPyMethods,3);
}

sipBuildBondsProcessor::sipBuildBondsProcessor(const BuildBondsProcessor& a0): BuildBondsProcessor(a0)
{
	sipCommonCtor(sipPyMethods,3);
}

sipBuildBondsProcessor::~sipBuildBondsProcessor()
{
	sipCommonDtor(sipPyThis);
}
bool sipBuildBondsProcessor::finish()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_finish,&relLock) ?
		sipBuildBondsProcessor::sipVH_finish(&sipPyMethods[0],sipPyThis,relLock) :
		BuildBondsProcessor::finish();
}
bool sipBuildBondsProcessor::start()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_start,&relLock) ?
		sipBuildBondsProcessor::sipVH_start(&sipPyMethods[1],sipPyThis,relLock) :
		BuildBondsProcessor::start();
}
Processor::Result sipBuildBondsProcessor::operator()(Fragment& a0)
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_Operator__call__,&relLock) ?
		sipFragmentProcessor::sipVH_Operator__call__(&sipPyMethods[2],sipPyThis,relLock,a0) :
		BuildBondsProcessor::operator()(a0);
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipBuildBondsProcessor::sipVH_finish(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_BuildBondsProcessor,sipName_BALL_finish);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipBuildBondsProcessor::sipVH_start(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_BuildBondsProcessor,sipName_BALL_start);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

static PyObject *sipDo_BuildBondsProcessor_finish(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BuildBondsProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			BuildBondsProcessor *ptr;

			if ((ptr = (BuildBondsProcessor *)sipGetCppPtr(sipThis,sipClass_BuildBondsProcessor)) == NULL)
				return NULL;

			res = ptr -> BuildBondsProcessor::finish();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BuildBondsProcessor,sipName_BALL_finish);

	return NULL;
}

static PyObject *sipDo_BuildBondsProcessor_start(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BuildBondsProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			BuildBondsProcessor *ptr;

			if ((ptr = (BuildBondsProcessor *)sipGetCppPtr(sipThis,sipClass_BuildBondsProcessor)) == NULL)
				return NULL;

			res = ptr -> BuildBondsProcessor::start();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BuildBondsProcessor,sipName_BALL_start);

	return NULL;
}

static PyObject *sipDo_BuildBondsProcessor_Operator__call__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BuildBondsProcessor)) == NULL)
		return NULL;

	{
		Fragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Fragment,&a0obj))
		{
			Processor::Result res;
			BuildBondsProcessor *ptr;

			if ((ptr = (BuildBondsProcessor *)sipGetCppPtr(sipThis,sipClass_BuildBondsProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Fragment(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> BuildBondsProcessor::operator()(* a0);

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BuildBondsProcessor,sipName_BALL_Operator__call__);

	return NULL;
}

static PyObject *sipDo_BuildBondsProcessor_getNumberOfBondsBuilt(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BuildBondsProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			BuildBondsProcessor *ptr;

			if ((ptr = (BuildBondsProcessor *)sipGetCppPtr(sipThis,sipClass_BuildBondsProcessor)) == NULL)
				return NULL;

			res = ptr -> BuildBondsProcessor::getNumberOfBondsBuilt();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BuildBondsProcessor,sipName_BALL_getNumberOfBondsBuilt);

	return NULL;
}

static PyObject *sipDo_BuildBondsProcessor_setFragmentDB(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BuildBondsProcessor)) == NULL)
		return NULL;

	{
		const FragmentDB *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_FragmentDB,&a0obj))
		{
			BuildBondsProcessor *ptr;

			if ((ptr = (BuildBondsProcessor *)sipGetCppPtr(sipThis,sipClass_BuildBondsProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_FragmentDB(a0obj,(FragmentDB **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BuildBondsProcessor::setFragmentDB(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BuildBondsProcessor,sipName_BALL_setFragmentDB);

	return NULL;
}

static PyObject *sipDo_BuildBondsProcessor_buildFragmentBonds(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BuildBondsProcessor)) == NULL)
		return NULL;

	{
		Fragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Fragment,&a0obj))
		{
			int res;
			BuildBondsProcessor *ptr;

			if ((ptr = (BuildBondsProcessor *)sipGetCppPtr(sipThis,sipClass_BuildBondsProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Fragment(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> BuildBondsProcessor::buildFragmentBonds(* a0);

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BuildBondsProcessor,sipName_BALL_buildFragmentBonds);

	return NULL;
}

static PyObject *sipDo_BuildBondsProcessor_buildInterFragmentBonds(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BuildBondsProcessor)) == NULL)
		return NULL;

	{
		Fragment *a0;
		PyObject *a0obj;
		Fragment *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_Fragment,&a0obj,sipCanConvertTo_Fragment,&a1obj))
		{
			int res;
			BuildBondsProcessor *ptr;

			if ((ptr = (BuildBondsProcessor *)sipGetCppPtr(sipThis,sipClass_BuildBondsProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Fragment(a0obj,&a0,1,&iserr);
			sipConvertTo_Fragment(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> BuildBondsProcessor::buildInterFragmentBonds(* a0,* a1);

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BuildBondsProcessor,sipName_BALL_buildInterFragmentBonds);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_BuildBondsProcessor(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_BuildBondsProcessor)
		return ptr;

	if ((res = sipCast_FragmentProcessor((FragmentProcessor *)(BuildBondsProcessor *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_BuildBondsProcessor(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipBuildBondsProcessor *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (BuildBondsProcessor *)sipThis -> u.cppPtr;
			else
				delete (sipBuildBondsProcessor *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_BuildBondsProcessor(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_BuildBondsProcessor
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
			sipNew = new sipBuildBondsProcessor();
	}
	}

	if (sipNew == NULL)
	{
		const FragmentDB *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_FragmentDB,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_FragmentDB(a0obj,(FragmentDB **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipBuildBondsProcessor(* a0);
	}
	}

	if (sipNew == NULL)
	{
		const BuildBondsProcessor *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_BuildBondsProcessor,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_BuildBondsProcessor(a0obj,(BuildBondsProcessor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipBuildBondsProcessor(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_BuildBondsProcessor);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_BuildBondsProcessor,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (BuildBondsProcessor *)sipNew;
			else
				delete (sipBuildBondsProcessor *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipBuildBondsProcessor *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_BuildBondsProcessor[] = {
	{sipName_BALL_finish, sipDo_BuildBondsProcessor_finish, METH_VARARGS, NULL},
	{sipName_BALL_start, sipDo_BuildBondsProcessor_start, METH_VARARGS, NULL},
	{sipName_BALL_Operator__call__, sipDo_BuildBondsProcessor_Operator__call__, METH_VARARGS, NULL},
	{sipName_BALL_getNumberOfBondsBuilt, sipDo_BuildBondsProcessor_getNumberOfBondsBuilt, METH_VARARGS, NULL},
	{sipName_BALL_setFragmentDB, sipDo_BuildBondsProcessor_setFragmentDB, METH_VARARGS, NULL},
	{sipName_BALL_buildFragmentBonds, sipDo_BuildBondsProcessor_buildFragmentBonds, METH_VARARGS, NULL},
	{sipName_BALL_buildInterFragmentBonds, sipDo_BuildBondsProcessor_buildInterFragmentBonds, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_BuildBondsProcessor(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_BuildBondsProcessor);
}

void sipConvertTo_BuildBondsProcessor(PyObject *sipPy,BuildBondsProcessor **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_BuildBondsProcessor);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (BuildBondsProcessor *)sipConvertToCpp(sipPy,sipClass_BuildBondsProcessor,sipIsErr);
}

BuildBondsProcessor *sipForceConvertTo_BuildBondsProcessor(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_BuildBondsProcessor(valobj))
	{
		BuildBondsProcessor *val;

		sipConvertTo_BuildBondsProcessor(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_BuildBondsProcessor);
	*iserrp = 1;

	return NULL;
}
