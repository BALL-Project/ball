#include "sipBALLDeclBALL.h"
#include "sipBALLExpressionPredicate.h"


PyObject *sipClass_ExpressionPredicate;

static void sipDealloc_ExpressionPredicate(sipThisType *);

static PyTypeObject sipType_ExpressionPredicate = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_ExpressionPredicate,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_ExpressionPredicate,
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

sipExpressionPredicate::sipExpressionPredicate()
   : ExpressionPredicate()
{
	sipCommonCtor(sipPyMethods,2);
}

sipExpressionPredicate::sipExpressionPredicate(const String& a0)
   : ExpressionPredicate(a0)
{
	sipCommonCtor(sipPyMethods,2);
}

sipExpressionPredicate::sipExpressionPredicate(const ExpressionPredicate& a0)
   : ExpressionPredicate(a0)
{
	sipCommonCtor(sipPyMethods,2);
}

sipExpressionPredicate::~sipExpressionPredicate()
 throw()
{
	sipCommonDtor(sipPyThis);
}

void sipExpressionPredicate::setArgument(const String& a0)
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_setArgument,&relLock))
		sipExpressionPredicate::sipVH_setArgument(&sipPyMethods[0],sipPyThis,relLock,a0);
	else
		ExpressionPredicate::setArgument(a0);
}

bool sipExpressionPredicate::operator()(const Atom& a0) const
 throw()
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_CallOp,&relLock) ?
		sipUnaryAtomPredicate::sipVH_CallOp(&sipPyMethods[1],sipPyThis,relLock,a0) :
		ExpressionPredicate::operator()(a0);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipExpressionPredicate::sipVH_setArgument(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,const String& a0)
{
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_String);

	sipArgs = Py_BuildValue("(ON)",sipThis -> sipSelf,a0obj);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_ExpressionPredicate,sipName_BALL_setArgument);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

static PyObject *sipDo_ExpressionPredicate_CallOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ExpressionPredicate)) == NULL)
		return NULL;

	{
		const Atom * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			bool res;
			ExpressionPredicate *ptr;

			if ((ptr = (ExpressionPredicate *)sipGetCppPtr(sipThis,sipClass_ExpressionPredicate)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,(Atom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ExpressionPredicate::operator()(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ExpressionPredicate,sipName_BALL_CallOp);

	return NULL;
}

static PyObject *sipDo_ExpressionPredicate_setArgument(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ExpressionPredicate)) == NULL)
		return NULL;

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			ExpressionPredicate *ptr;

			if ((ptr = (ExpressionPredicate *)sipGetCppPtr(sipThis,sipClass_ExpressionPredicate)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> ExpressionPredicate::setArgument(* a0);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ExpressionPredicate,sipName_BALL_setArgument);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_ExpressionPredicate(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_ExpressionPredicate)
		return ptr;

	if ((res = sipCast_UnaryAtomPredicate((UnaryAtomPredicate *)(ExpressionPredicate *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_ExpressionPredicate(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipExpressionPredicate *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (ExpressionPredicate *)sipThis -> u.cppPtr;
			else
				delete (sipExpressionPredicate *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_ExpressionPredicate(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_ExpressionPredicate
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
			sipNew = new sipExpressionPredicate();
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
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_String,&a0obj))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipExpressionPredicate(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;
		}
	}

	if (sipNew == NULL)
	{
		const ExpressionPredicate * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_ExpressionPredicate,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_ExpressionPredicate(a0obj,(ExpressionPredicate **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipExpressionPredicate(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_ExpressionPredicate);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_ExpressionPredicate,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (ExpressionPredicate *)sipNew;
			else
				delete (sipExpressionPredicate *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipExpressionPredicate *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_ExpressionPredicate[] = {
	{sipName_BALL_CallOp, sipDo_ExpressionPredicate_CallOp, METH_VARARGS, NULL},
	{sipName_BALL_setArgument, sipDo_ExpressionPredicate_setArgument, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_ExpressionPredicate(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_ExpressionPredicate);
}

void sipConvertTo_ExpressionPredicate(PyObject *sipPy,ExpressionPredicate **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_ExpressionPredicate);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (ExpressionPredicate *)sipConvertToCpp(sipPy,sipClass_ExpressionPredicate,sipIsErr);
}

ExpressionPredicate *sipForceConvertTo_ExpressionPredicate(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_ExpressionPredicate(valobj))
	{
		ExpressionPredicate *val;

		sipConvertTo_ExpressionPredicate(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_ExpressionPredicate);

	*iserrp = 1;

	return NULL;
}
