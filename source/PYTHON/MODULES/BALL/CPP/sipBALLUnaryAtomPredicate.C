#include "sipBALLDeclBALL.h"
#include "sipBALLUnaryAtomPredicate.h"



PyObject *sipClass_UnaryAtomPredicate;

static void sipDealloc_UnaryAtomPredicate(sipThisType *);

static PyTypeObject sipType_UnaryAtomPredicate = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_UnaryAtomPredicate,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_UnaryAtomPredicate,
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

sipUnaryAtomPredicate::sipUnaryAtomPredicate()
   throw()  : UnaryAtomPredicate()
{
	sipCommonCtor(sipPyMethods,1);
}

sipUnaryAtomPredicate::sipUnaryAtomPredicate(const UnaryAtomPredicate& a0)
    : UnaryAtomPredicate(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipUnaryAtomPredicate::~sipUnaryAtomPredicate()
  throw()
{
	sipCommonDtor(sipPyThis);
}

bool sipUnaryAtomPredicate::operator ()(const Atom& a0) const
 throw()
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[0],sipPyThis,NULL,sipName_BALL___call__,&relLock) ?
		sipUnaryAtomPredicate::sipVH_CallOperator(&sipPyMethods[0],sipPyThis,relLock,a0) :
		UnaryAtomPredicate::operator ()(a0);
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipUnaryAtomPredicate::sipVH_CallOperator(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,const Atom& a0)
{
	bool res;
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_Atom);

	sipArgs = Py_BuildValue("(OO)",sipThis -> sipSelf,a0obj);

	Py_XDECREF(a0obj);

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

		sipBadVirtualResultType(sipName_BALL_UnaryAtomPredicate,sipName_BALL___call__);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

static PyObject *sipDo_UnaryAtomPredicate___call__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_UnaryAtomPredicate)) == NULL)
		return NULL;

	{
		const Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			bool res;
			UnaryAtomPredicate *ptr;

			if ((ptr = (UnaryAtomPredicate *)sipGetCppPtr(sipThis,sipClass_UnaryAtomPredicate)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,(Atom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> UnaryAtomPredicate::operator ()(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_UnaryAtomPredicate,sipName_BALL___call__);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_UnaryAtomPredicate(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_UnaryAtomPredicate)
		return ptr;

	return NULL;
}

static void sipDealloc_UnaryAtomPredicate(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipUnaryAtomPredicate *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (UnaryAtomPredicate *)sipThis -> u.cppPtr;
			else
				delete (sipUnaryAtomPredicate *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_UnaryAtomPredicate(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_UnaryAtomPredicate
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
			sipNew = new sipUnaryAtomPredicate();
		}
	}

	if (sipNew == NULL)
	{
		const UnaryAtomPredicate *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_UnaryAtomPredicate,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_UnaryAtomPredicate(a0obj,(UnaryAtomPredicate **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipUnaryAtomPredicate(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_UnaryAtomPredicate);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_UnaryAtomPredicate,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (UnaryAtomPredicate *)sipNew;
			else
				delete (sipUnaryAtomPredicate *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipUnaryAtomPredicate *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_UnaryAtomPredicate[] = {
	{sipName_BALL___call__, sipDo_UnaryAtomPredicate___call__, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_UnaryAtomPredicate(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_UnaryAtomPredicate);
}

void sipConvertTo_UnaryAtomPredicate(PyObject *sipPy,UnaryAtomPredicate **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_UnaryAtomPredicate);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (UnaryAtomPredicate *)sipConvertToCpp(sipPy,sipClass_UnaryAtomPredicate,sipIsErr);
}

UnaryAtomPredicate *sipForceConvertTo_UnaryAtomPredicate(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_UnaryAtomPredicate(valobj))
	{
		UnaryAtomPredicate *val;

		sipConvertTo_UnaryAtomPredicate(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_UnaryAtomPredicate);
	*iserrp = 1;

	return NULL;
}
