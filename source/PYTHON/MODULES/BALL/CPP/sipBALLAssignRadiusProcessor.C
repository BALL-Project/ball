#include "sipBALLDeclBALL.h"
#include "sipBALLAssignRadiusProcessor.h"



PyObject *sipClass_AssignRadiusProcessor;

static void sipDealloc_AssignRadiusProcessor(sipThisType *);

static PyTypeObject sipType_AssignRadiusProcessor = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_AssignRadiusProcessor,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_AssignRadiusProcessor,
	0,
	0,
	0,
	0,
	0,
};

sipAssignRadiusProcessor::sipAssignRadiusProcessor(): AssignRadiusProcessor()
{
	sipCommonCtor(sipPyMethods,3);
}

sipAssignRadiusProcessor::sipAssignRadiusProcessor(const String& a0): AssignRadiusProcessor(a0)
{
	sipCommonCtor(sipPyMethods,3);
}

sipAssignRadiusProcessor::sipAssignRadiusProcessor(const AssignRadiusProcessor& a0): AssignRadiusProcessor(a0)
{
	sipCommonCtor(sipPyMethods,3);
}

sipAssignRadiusProcessor::~sipAssignRadiusProcessor()
{
	sipCommonDtor(sipPyThis);
}
bool sipAssignRadiusProcessor::start()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_start,&relLock) ?
		sipAssignRadiusProcessor::sipVH_start(&sipPyMethods[0],sipPyThis,relLock) :
		AssignRadiusProcessor::start();
}
bool sipAssignRadiusProcessor::finish()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_finish,&relLock) ?
		sipAssignRadiusProcessor::sipVH_finish(&sipPyMethods[1],sipPyThis,relLock) :
		AssignRadiusProcessor::finish();
}
Processor::Result sipAssignRadiusProcessor::operator()(Atom& a0)
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_Operator__call__,&relLock) ?
		sipAtomProcessor::sipVH_Operator__call__(&sipPyMethods[2],sipPyThis,relLock,a0) :
		AssignRadiusProcessor::operator()(a0);
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipAssignRadiusProcessor::sipVH_start(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_AssignRadiusProcessor,sipName_BALL_start);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipAssignRadiusProcessor::sipVH_finish(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_AssignRadiusProcessor,sipName_BALL_finish);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

static PyObject *sipDo_AssignRadiusProcessor_start(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AssignRadiusProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			AssignRadiusProcessor *ptr;

			if ((ptr = (AssignRadiusProcessor *)sipGetCppPtr(sipThis,sipClass_AssignRadiusProcessor)) == NULL)
				return NULL;

			res = ptr -> AssignRadiusProcessor::start();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_AssignRadiusProcessor,sipName_BALL_start);

	return NULL;
}

static PyObject *sipDo_AssignRadiusProcessor_finish(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AssignRadiusProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			AssignRadiusProcessor *ptr;

			if ((ptr = (AssignRadiusProcessor *)sipGetCppPtr(sipThis,sipClass_AssignRadiusProcessor)) == NULL)
				return NULL;

			res = ptr -> AssignRadiusProcessor::finish();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_AssignRadiusProcessor,sipName_BALL_finish);

	return NULL;
}

static PyObject *sipDo_AssignRadiusProcessor_Operator__call__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AssignRadiusProcessor)) == NULL)
		return NULL;

	{
		Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			Processor::Result res;
			AssignRadiusProcessor *ptr;

			if ((ptr = (AssignRadiusProcessor *)sipGetCppPtr(sipThis,sipClass_AssignRadiusProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> AssignRadiusProcessor::operator()(* a0);

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_AssignRadiusProcessor,sipName_BALL_Operator__call__);

	return NULL;
}

static PyObject *sipDo_AssignRadiusProcessor_setFilename(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AssignRadiusProcessor)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			AssignRadiusProcessor *ptr;

			if ((ptr = (AssignRadiusProcessor *)sipGetCppPtr(sipThis,sipClass_AssignRadiusProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> AssignRadiusProcessor::setFilename(* a0);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_AssignRadiusProcessor,sipName_BALL_setFilename);

	return NULL;
}

static PyObject *sipDo_AssignRadiusProcessor_getFilename(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AssignRadiusProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			String *res;
			AssignRadiusProcessor *ptr;

			if ((ptr = (AssignRadiusProcessor *)sipGetCppPtr(sipThis,sipClass_AssignRadiusProcessor)) == NULL)
				return NULL;

			res = &ptr -> AssignRadiusProcessor::getFilename();

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_AssignRadiusProcessor,sipName_BALL_getFilename);

	return NULL;
}

static PyObject *sipDo_AssignRadiusProcessor_getNumberOfAssignments(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AssignRadiusProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Size *res;
			AssignRadiusProcessor *ptr;

			if ((ptr = (AssignRadiusProcessor *)sipGetCppPtr(sipThis,sipClass_AssignRadiusProcessor)) == NULL)
				return NULL;

			res = new Size(ptr -> AssignRadiusProcessor::getNumberOfAssignments());

			return sipNewCppToSelf(res,sipClass_Size,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_AssignRadiusProcessor,sipName_BALL_getNumberOfAssignments);

	return NULL;
}

static PyObject *sipDo_AssignRadiusProcessor_getNumberOfErrors(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AssignRadiusProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Size *res;
			AssignRadiusProcessor *ptr;

			if ((ptr = (AssignRadiusProcessor *)sipGetCppPtr(sipThis,sipClass_AssignRadiusProcessor)) == NULL)
				return NULL;

			res = new Size(ptr -> AssignRadiusProcessor::getNumberOfErrors());

			return sipNewCppToSelf(res,sipClass_Size,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_AssignRadiusProcessor,sipName_BALL_getNumberOfErrors);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_AssignRadiusProcessor(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_AssignRadiusProcessor)
		return ptr;

	if ((res = sipCast_AtomProcessor((AtomProcessor *)(AssignRadiusProcessor *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_AssignRadiusProcessor(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipAssignRadiusProcessor *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (AssignRadiusProcessor *)sipThis -> u.cppPtr;
			else
				delete (sipAssignRadiusProcessor *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_AssignRadiusProcessor(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_AssignRadiusProcessor
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
			sipNew = new sipAssignRadiusProcessor();
		}
	}

	if (sipNew == NULL)
	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_String,&a0obj))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipAssignRadiusProcessor(* a0);

			if (istemp0)
				delete a0;
		}
	}

	if (sipNew == NULL)
	{
		const AssignRadiusProcessor *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_AssignRadiusProcessor,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_AssignRadiusProcessor(a0obj,(AssignRadiusProcessor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipAssignRadiusProcessor(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_AssignRadiusProcessor);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_AssignRadiusProcessor,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (AssignRadiusProcessor *)sipNew;
			else
				delete (sipAssignRadiusProcessor *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipAssignRadiusProcessor *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_AssignRadiusProcessor[] = {
	{sipName_BALL_start, sipDo_AssignRadiusProcessor_start, METH_VARARGS, NULL},
	{sipName_BALL_finish, sipDo_AssignRadiusProcessor_finish, METH_VARARGS, NULL},
	{sipName_BALL_Operator__call__, sipDo_AssignRadiusProcessor_Operator__call__, METH_VARARGS, NULL},
	{sipName_BALL_setFilename, sipDo_AssignRadiusProcessor_setFilename, METH_VARARGS, NULL},
	{sipName_BALL_getFilename, sipDo_AssignRadiusProcessor_getFilename, METH_VARARGS, NULL},
	{sipName_BALL_getNumberOfAssignments, sipDo_AssignRadiusProcessor_getNumberOfAssignments, METH_VARARGS, NULL},
	{sipName_BALL_getNumberOfErrors, sipDo_AssignRadiusProcessor_getNumberOfErrors, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_AssignRadiusProcessor(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_AssignRadiusProcessor);
}

void sipConvertTo_AssignRadiusProcessor(PyObject *sipPy,AssignRadiusProcessor **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_AssignRadiusProcessor);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (AssignRadiusProcessor *)sipConvertToCpp(sipPy,sipClass_AssignRadiusProcessor,sipIsErr);
}

AssignRadiusProcessor *sipForceConvertTo_AssignRadiusProcessor(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_AssignRadiusProcessor(valobj))
	{
		AssignRadiusProcessor *val;

		sipConvertTo_AssignRadiusProcessor(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_AssignRadiusProcessor);
	*iserrp = 1;

	return NULL;
}
