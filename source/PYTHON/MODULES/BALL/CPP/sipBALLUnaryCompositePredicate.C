#include "sipBALLDeclBALL.h"
#include "sipBALLUnaryCompositePredicate.h"


PyObject *sipClass_UnaryCompositePredicate;

static void sipDealloc_UnaryCompositePredicate(sipThisType *);

static PyTypeObject sipType_UnaryCompositePredicate = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_UnaryCompositePredicate,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_UnaryCompositePredicate,
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

sipUnaryCompositePredicate::sipUnaryCompositePredicate()
   throw() : UnaryCompositePredicate()
{
	sipCommonCtor(sipPyMethods,1);
}

sipUnaryCompositePredicate::sipUnaryCompositePredicate(const UnaryCompositePredicate& a0)
   : UnaryCompositePredicate(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipUnaryCompositePredicate::~sipUnaryCompositePredicate()
 throw()
{
	sipCommonDtor(sipPyThis);
}

bool sipUnaryCompositePredicate::operator()(const Composite& a0) const
 throw()
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_CallOp,&relLock) ?
		sipUnaryCompositePredicate::sipVH_CallOp(&sipPyMethods[0],sipPyThis,relLock,a0) :
		UnaryCompositePredicate::operator()(a0);
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipUnaryCompositePredicate::sipVH_CallOp(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,const Composite& a0)
{
	bool res;
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_Composite);

	sipArgs = Py_BuildValue("(ON)",sipThis -> sipSelf,a0obj);

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

		sipBadVirtualResultType(sipName_BALL_UnaryCompositePredicate,sipName_BALL_CallOp);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

extern "C" PyObject *sipDo_UnaryCompositePredicate_CallOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_UnaryCompositePredicate)) == NULL)
		return NULL;

	{
		const Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			bool res;
			UnaryCompositePredicate *ptr;

			if ((ptr = (UnaryCompositePredicate *)sipGetCppPtr(sipThis,sipClass_UnaryCompositePredicate)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> UnaryCompositePredicate::operator()(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_UnaryCompositePredicate,sipName_BALL_CallOp);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_UnaryCompositePredicate(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_UnaryCompositePredicate)
		return ptr;

	return NULL;
}

static void sipDealloc_UnaryCompositePredicate(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipUnaryCompositePredicate *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (UnaryCompositePredicate *)sipThis -> u.cppPtr;
			else
				delete (sipUnaryCompositePredicate *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_UnaryCompositePredicate(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_UnaryCompositePredicate
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
			sipNew = new sipUnaryCompositePredicate();
		}
	}

	if (sipNew == NULL)
	{
		const UnaryCompositePredicate * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_UnaryCompositePredicate,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_UnaryCompositePredicate(a0obj,(UnaryCompositePredicate **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipUnaryCompositePredicate(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_UnaryCompositePredicate);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_UnaryCompositePredicate,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (UnaryCompositePredicate *)sipNew;
			else
				delete (sipUnaryCompositePredicate *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipUnaryCompositePredicate *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_UnaryCompositePredicate[] = {
	{sipName_BALL_CallOp, sipDo_UnaryCompositePredicate_CallOp, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_UnaryCompositePredicate(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_UnaryCompositePredicate);
}

void sipConvertTo_UnaryCompositePredicate(PyObject *sipPy,UnaryCompositePredicate **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_UnaryCompositePredicate);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (UnaryCompositePredicate *)sipConvertToCpp(sipPy,sipClass_UnaryCompositePredicate,sipIsErr);
}

UnaryCompositePredicate *sipForceConvertTo_UnaryCompositePredicate(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_UnaryCompositePredicate(valobj))
	{
		UnaryCompositePredicate *val;

		sipConvertTo_UnaryCompositePredicate(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_UnaryCompositePredicate);

	*iserrp = 1;

	return NULL;
}
