#include "sipBALLDeclBALL.h"
#include "sipBALLSelector.h"



PyObject *sipClass_Selector;

static void sipDealloc_Selector(sipThisType *);

static PyTypeObject sipType_Selector = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Selector,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Selector,
	0,
	0,
	0,
	0,
	0,
};

sipSelector::sipSelector(): Selector()
{
	sipCommonCtor(sipPyMethods,2);
}

sipSelector::sipSelector(const Selector& a0): Selector(a0)
{
	sipCommonCtor(sipPyMethods,2);
}

sipSelector::sipSelector(const String& a0): Selector(a0)
{
	sipCommonCtor(sipPyMethods,2);
}

sipSelector::~sipSelector()
{
	sipCommonDtor(sipPyThis);
}
Processor::Result sipSelector::operator()(Composite& a0)
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_Operator__call__,&relLock) ?
		sipCompositeProcessor::sipVH_Operator__call__(&sipPyMethods[0],sipPyThis,relLock,a0) :
		Selector::operator()(a0);
}
bool sipSelector::start()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_start,&relLock) ?
		sipSelector::sipVH_start(&sipPyMethods[1],sipPyThis,relLock) :
		Selector::start();
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipSelector::sipVH_start(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_Selector,sipName_BALL_start);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

static PyObject *sipDo_Selector_Operator__call__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Selector)) == NULL)
		return NULL;

	{
		Composite *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			Processor::Result res;
			Selector *ptr;

			if ((ptr = (Selector *)sipGetCppPtr(sipThis,sipClass_Selector)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Selector::operator()(* a0);

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Selector,sipName_BALL_Operator__call__);

	return NULL;
}

static PyObject *sipDo_Selector_start(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Selector)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Selector *ptr;

			if ((ptr = (Selector *)sipGetCppPtr(sipThis,sipClass_Selector)) == NULL)
				return NULL;

			res = ptr -> Selector::start();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Selector,sipName_BALL_start);

	return NULL;
}

static PyObject *sipDo_Selector_getNumberOfSelectedAtoms(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Selector)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Size *res;
			Selector *ptr;

			if ((ptr = (Selector *)sipGetCppPtr(sipThis,sipClass_Selector)) == NULL)
				return NULL;

			res = new Size(ptr -> Selector::getNumberOfSelectedAtoms());

			return sipNewCppToSelf(res,sipClass_Size,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Selector,sipName_BALL_getNumberOfSelectedAtoms);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Selector(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_Selector)
		return ptr;

	if ((res = sipCast_CompositeProcessor((CompositeProcessor *)(Selector *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_Selector(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipSelector *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (Selector *)sipThis -> u.cppPtr;
			else
				delete (sipSelector *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_Selector(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Selector
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
			sipNew = new sipSelector();
		}
	}

	if (sipNew == NULL)
	{
		const Selector *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_Selector,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Selector(a0obj,(Selector **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipSelector(* a0);
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

			sipNew = new sipSelector(* a0);

			if (istemp0)
				delete a0;
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_Selector);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Selector,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (Selector *)sipNew;
			else
				delete (sipSelector *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipSelector *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Selector[] = {
	{sipName_BALL_Operator__call__, sipDo_Selector_Operator__call__, METH_VARARGS, NULL},
	{sipName_BALL_start, sipDo_Selector_start, METH_VARARGS, NULL},
	{sipName_BALL_getNumberOfSelectedAtoms, sipDo_Selector_getNumberOfSelectedAtoms, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_Selector(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Selector);
}

void sipConvertTo_Selector(PyObject *sipPy,Selector **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_Selector);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Selector *)sipConvertToCpp(sipPy,sipClass_Selector,sipIsErr);
}

Selector *sipForceConvertTo_Selector(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Selector(valobj))
	{
		Selector *val;

		sipConvertTo_Selector(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Selector);
	*iserrp = 1;

	return NULL;
}
