#include "sipBALLDeclBALL.h"
#include "sipBALLBoundingBoxProcessor.h"



PyObject *sipClass_BoundingBoxProcessor;

static void sipDealloc_BoundingBoxProcessor(sipThisType *);

static PyTypeObject sipType_BoundingBoxProcessor = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_BoundingBoxProcessor,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_BoundingBoxProcessor,
	0,
	0,
	0,
	0,
	0,
};

sipBoundingBoxProcessor::sipBoundingBoxProcessor(): BoundingBoxProcessor()
{
	sipCommonCtor(sipPyMethods,3);
}

sipBoundingBoxProcessor::sipBoundingBoxProcessor(const BoundingBoxProcessor& a0): BoundingBoxProcessor(a0)
{
	sipCommonCtor(sipPyMethods,3);
}

sipBoundingBoxProcessor::~sipBoundingBoxProcessor()
{
	sipCommonDtor(sipPyThis);
}
bool sipBoundingBoxProcessor::start()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_start,&relLock) ?
		sipBoundingBoxProcessor::sipVH_start(&sipPyMethods[0],sipPyThis,relLock) :
		BoundingBoxProcessor::start();
}
bool sipBoundingBoxProcessor::finish()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_finish,&relLock) ?
		sipBoundingBoxProcessor::sipVH_finish(&sipPyMethods[1],sipPyThis,relLock) :
		BoundingBoxProcessor::finish();
}
Processor::Result sipBoundingBoxProcessor::operator()(Atom& a0)
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_Operator__call__,&relLock) ?
		sipAtomProcessor::sipVH_Operator__call__(&sipPyMethods[2],sipPyThis,relLock,a0) :
		BoundingBoxProcessor::operator()(a0);
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipBoundingBoxProcessor::sipVH_start(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_BoundingBoxProcessor,sipName_BALL_start);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipBoundingBoxProcessor::sipVH_finish(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_BoundingBoxProcessor,sipName_BALL_finish);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

static PyObject *sipDo_BoundingBoxProcessor_start(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BoundingBoxProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			BoundingBoxProcessor *ptr;

			if ((ptr = (BoundingBoxProcessor *)sipGetCppPtr(sipThis,sipClass_BoundingBoxProcessor)) == NULL)
				return NULL;

			res = ptr -> BoundingBoxProcessor::start();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BoundingBoxProcessor,sipName_BALL_start);

	return NULL;
}

static PyObject *sipDo_BoundingBoxProcessor_finish(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BoundingBoxProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			BoundingBoxProcessor *ptr;

			if ((ptr = (BoundingBoxProcessor *)sipGetCppPtr(sipThis,sipClass_BoundingBoxProcessor)) == NULL)
				return NULL;

			res = ptr -> BoundingBoxProcessor::finish();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BoundingBoxProcessor,sipName_BALL_finish);

	return NULL;
}

static PyObject *sipDo_BoundingBoxProcessor_Operator__call__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BoundingBoxProcessor)) == NULL)
		return NULL;

	{
		Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			Processor::Result res;
			BoundingBoxProcessor *ptr;

			if ((ptr = (BoundingBoxProcessor *)sipGetCppPtr(sipThis,sipClass_BoundingBoxProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> BoundingBoxProcessor::operator()(* a0);

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BoundingBoxProcessor,sipName_BALL_Operator__call__);

	return NULL;
}

static PyObject *sipDo_BoundingBoxProcessor_getBox(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BoundingBoxProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Box3 *res;
			BoundingBoxProcessor *ptr;

			if ((ptr = (BoundingBoxProcessor *)sipGetCppPtr(sipThis,sipClass_BoundingBoxProcessor)) == NULL)
				return NULL;

			res = new Box3(ptr -> BoundingBoxProcessor::getBox());

			return sipNewCppToSelf(res,sipClass_Box3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BoundingBoxProcessor,sipName_BALL_getBox);

	return NULL;
}

static PyObject *sipDo_BoundingBoxProcessor_getLower(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BoundingBoxProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			const Vector3 *res;
			BoundingBoxProcessor *ptr;

			if ((ptr = (BoundingBoxProcessor *)sipGetCppPtr(sipThis,sipClass_BoundingBoxProcessor)) == NULL)
				return NULL;

			res = &ptr -> BoundingBoxProcessor::getLower();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BoundingBoxProcessor,sipName_BALL_getLower);

	return NULL;
}

static PyObject *sipDo_BoundingBoxProcessor_getUpper(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BoundingBoxProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			const Vector3 *res;
			BoundingBoxProcessor *ptr;

			if ((ptr = (BoundingBoxProcessor *)sipGetCppPtr(sipThis,sipClass_BoundingBoxProcessor)) == NULL)
				return NULL;

			res = &ptr -> BoundingBoxProcessor::getUpper();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BoundingBoxProcessor,sipName_BALL_getUpper);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_BoundingBoxProcessor(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_BoundingBoxProcessor)
		return ptr;

	if ((res = sipCast_AtomProcessor((AtomProcessor *)(BoundingBoxProcessor *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_BoundingBoxProcessor(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipBoundingBoxProcessor *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (BoundingBoxProcessor *)sipThis -> u.cppPtr;
			else
				delete (sipBoundingBoxProcessor *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_BoundingBoxProcessor(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_BoundingBoxProcessor
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
			sipNew = new sipBoundingBoxProcessor();
	}
	}

	if (sipNew == NULL)
	{
		const BoundingBoxProcessor *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_BoundingBoxProcessor,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_BoundingBoxProcessor(a0obj,(BoundingBoxProcessor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipBoundingBoxProcessor(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_BoundingBoxProcessor);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_BoundingBoxProcessor,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (BoundingBoxProcessor *)sipNew;
			else
				delete (sipBoundingBoxProcessor *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipBoundingBoxProcessor *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_BoundingBoxProcessor[] = {
	{sipName_BALL_start, sipDo_BoundingBoxProcessor_start, METH_VARARGS, NULL},
	{sipName_BALL_finish, sipDo_BoundingBoxProcessor_finish, METH_VARARGS, NULL},
	{sipName_BALL_Operator__call__, sipDo_BoundingBoxProcessor_Operator__call__, METH_VARARGS, NULL},
	{sipName_BALL_getBox, sipDo_BoundingBoxProcessor_getBox, METH_VARARGS, NULL},
	{sipName_BALL_getLower, sipDo_BoundingBoxProcessor_getLower, METH_VARARGS, NULL},
	{sipName_BALL_getUpper, sipDo_BoundingBoxProcessor_getUpper, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_BoundingBoxProcessor(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_BoundingBoxProcessor);
}

void sipConvertTo_BoundingBoxProcessor(PyObject *sipPy,BoundingBoxProcessor **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_BoundingBoxProcessor);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (BoundingBoxProcessor *)sipConvertToCpp(sipPy,sipClass_BoundingBoxProcessor,sipIsErr);
}

BoundingBoxProcessor *sipForceConvertTo_BoundingBoxProcessor(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_BoundingBoxProcessor(valobj))
	{
		BoundingBoxProcessor *val;

		sipConvertTo_BoundingBoxProcessor(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_BoundingBoxProcessor);
	*iserrp = 1;

	return NULL;
}
