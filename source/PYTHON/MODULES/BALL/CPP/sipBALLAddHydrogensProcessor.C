#include "sipBALLDeclBALL.h"
#include "sipBALLAddHydrogensProcessor.h"



PyObject *sipClass_AddHydrogensProcessor;

static void sipDealloc_AddHydrogensProcessor(sipThisType *);

static PyTypeObject sipType_AddHydrogensProcessor = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_AddHydrogensProcessor,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_AddHydrogensProcessor,
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

sipAddHydrogensProcessor::sipAddHydrogensProcessor()
    : AddHydrogensProcessor()
{
	sipCommonCtor(sipPyMethods,3);
}

sipAddHydrogensProcessor::sipAddHydrogensProcessor(const FragmentDB& a0)
    : AddHydrogensProcessor(a0)
{
	sipCommonCtor(sipPyMethods,3);
}

sipAddHydrogensProcessor::sipAddHydrogensProcessor(const AddHydrogensProcessor& a0)
    : AddHydrogensProcessor(a0)
{
	sipCommonCtor(sipPyMethods,3);
}

sipAddHydrogensProcessor::~sipAddHydrogensProcessor()
 
{
	sipCommonDtor(sipPyThis);
}

Processor::Result sipAddHydrogensProcessor::operator ()(Fragment& a0)

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL___call__,&relLock) ?
		sipFragmentProcessor::sipVH_CallOperator(&sipPyMethods[0],sipPyThis,relLock,a0) :
		AddHydrogensProcessor::operator ()(a0);
}

bool sipAddHydrogensProcessor::finish()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_finish,&relLock) ?
		sipAddHydrogensProcessor::sipVH_finish(&sipPyMethods[1],sipPyThis,relLock) :
		AddHydrogensProcessor::finish();
}

bool sipAddHydrogensProcessor::start()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_start,&relLock) ?
		sipAddHydrogensProcessor::sipVH_start(&sipPyMethods[2],sipPyThis,relLock) :
		AddHydrogensProcessor::start();
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipAddHydrogensProcessor::sipVH_finish(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_AddHydrogensProcessor,sipName_BALL_finish);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipAddHydrogensProcessor::sipVH_start(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_AddHydrogensProcessor,sipName_BALL_start);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

static PyObject *sipDo_AddHydrogensProcessor_start(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AddHydrogensProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			AddHydrogensProcessor *ptr;

			if ((ptr = (AddHydrogensProcessor *)sipGetCppPtr(sipThis,sipClass_AddHydrogensProcessor)) == NULL)
				return NULL;

			res = ptr -> AddHydrogensProcessor::start();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AddHydrogensProcessor,sipName_BALL_start);

	return NULL;
}

static PyObject *sipDo_AddHydrogensProcessor_finish(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AddHydrogensProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			AddHydrogensProcessor *ptr;

			if ((ptr = (AddHydrogensProcessor *)sipGetCppPtr(sipThis,sipClass_AddHydrogensProcessor)) == NULL)
				return NULL;

			res = ptr -> AddHydrogensProcessor::finish();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AddHydrogensProcessor,sipName_BALL_finish);

	return NULL;
}

static PyObject *sipDo_AddHydrogensProcessor___call__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AddHydrogensProcessor)) == NULL)
		return NULL;

	{
		Fragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Fragment,&a0obj))
		{
			Processor::Result res;
			AddHydrogensProcessor *ptr;

			if ((ptr = (AddHydrogensProcessor *)sipGetCppPtr(sipThis,sipClass_AddHydrogensProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Fragment(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> AddHydrogensProcessor::operator ()(* a0);

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AddHydrogensProcessor,sipName_BALL___call__);

	return NULL;
}

static PyObject *sipDo_AddHydrogensProcessor_getNumberOfInsertedHydrogens(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AddHydrogensProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			AddHydrogensProcessor *ptr;

			if ((ptr = (AddHydrogensProcessor *)sipGetCppPtr(sipThis,sipClass_AddHydrogensProcessor)) == NULL)
				return NULL;

			res = ptr -> AddHydrogensProcessor::getNumberOfInsertedHydrogens();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AddHydrogensProcessor,sipName_BALL_getNumberOfInsertedHydrogens);

	return NULL;
}

static PyObject *sipDo_AddHydrogensProcessor_setFragmentDB(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AddHydrogensProcessor)) == NULL)
		return NULL;

	{
		const FragmentDB *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_FragmentDB,&a0obj))
		{
			AddHydrogensProcessor *ptr;

			if ((ptr = (AddHydrogensProcessor *)sipGetCppPtr(sipThis,sipClass_AddHydrogensProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_FragmentDB(a0obj,(FragmentDB **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> AddHydrogensProcessor::setFragmentDB(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AddHydrogensProcessor,sipName_BALL_setFragmentDB);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_AddHydrogensProcessor(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_AddHydrogensProcessor)
		return ptr;

	if ((res = sipCast_FragmentProcessor((FragmentProcessor *)(AddHydrogensProcessor *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_AddHydrogensProcessor(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipAddHydrogensProcessor *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (AddHydrogensProcessor *)sipThis -> u.cppPtr;
			else
				delete (sipAddHydrogensProcessor *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_AddHydrogensProcessor(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_AddHydrogensProcessor
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
			sipNew = new sipAddHydrogensProcessor();
		}
	}

	if (sipNew == NULL)
	{
		const FragmentDB *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_FragmentDB,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_FragmentDB(a0obj,(FragmentDB **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipAddHydrogensProcessor(* a0);
		}
	}

	if (sipNew == NULL)
	{
		const AddHydrogensProcessor *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_AddHydrogensProcessor,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_AddHydrogensProcessor(a0obj,(AddHydrogensProcessor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipAddHydrogensProcessor(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_AddHydrogensProcessor);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_AddHydrogensProcessor,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (AddHydrogensProcessor *)sipNew;
			else
				delete (sipAddHydrogensProcessor *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipAddHydrogensProcessor *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_AddHydrogensProcessor[] = {
	{sipName_BALL_start, sipDo_AddHydrogensProcessor_start, METH_VARARGS, NULL},
	{sipName_BALL_finish, sipDo_AddHydrogensProcessor_finish, METH_VARARGS, NULL},
	{sipName_BALL___call__, sipDo_AddHydrogensProcessor___call__, METH_VARARGS, NULL},
	{sipName_BALL_getNumberOfInsertedHydrogens, sipDo_AddHydrogensProcessor_getNumberOfInsertedHydrogens, METH_VARARGS, NULL},
	{sipName_BALL_setFragmentDB, sipDo_AddHydrogensProcessor_setFragmentDB, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_AddHydrogensProcessor(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_AddHydrogensProcessor);
}

void sipConvertTo_AddHydrogensProcessor(PyObject *sipPy,AddHydrogensProcessor **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_AddHydrogensProcessor);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (AddHydrogensProcessor *)sipConvertToCpp(sipPy,sipClass_AddHydrogensProcessor,sipIsErr);
}

AddHydrogensProcessor *sipForceConvertTo_AddHydrogensProcessor(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_AddHydrogensProcessor(valobj))
	{
		AddHydrogensProcessor *val;

		sipConvertTo_AddHydrogensProcessor(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_AddHydrogensProcessor);
	*iserrp = 1;

	return NULL;
}
