#include "sipBALLDeclBALL.h"
#include "sipBALLExpression.h"



PyObject *sipClass_Expression;

static void sipDealloc_Expression(sipThisType *);

static PyTypeObject sipType_Expression = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Expression,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Expression,
	0,
	0,
	0,
	0,
	0,
};

sipExpression::sipExpression(): Expression()
{
	sipCommonCtor(sipPyMethods,1);
}

sipExpression::sipExpression(const Expression& a0): Expression(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipExpression::sipExpression(const String& a0): Expression(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipExpression::~sipExpression()
{
	sipCommonDtor(sipPyThis);
}
bool sipExpression::operator()(const Atom& a0) const
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_Operator__call__,&relLock) ?
		sipExpression::sipVH_Operator__call__(&sipPyMethods[0],sipPyThis,relLock,a0) :
		Expression::operator()(a0);
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipExpression::sipVH_Operator__call__(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,const Atom& a0)
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

		sipBadVirtualResultType(sipName_BALL_Expression,sipName_BALL_Operator__call__);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

static PyObject *sipDo_Expression_hasPredicate(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Expression)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			bool res;
			Expression *ptr;

			if ((ptr = (Expression *)sipGetCppPtr(sipThis,sipClass_Expression)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Expression::hasPredicate(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Expression,sipName_BALL_hasPredicate);

	return NULL;
}

static PyObject *sipDo_Expression_Operator__call__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Expression)) == NULL)
		return NULL;

	{
		const Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			bool res;
			Expression *ptr;

			if ((ptr = (Expression *)sipGetCppPtr(sipThis,sipClass_Expression)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,(Atom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Expression::operator()(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Expression,sipName_BALL_Operator__call__);

	return NULL;
}

static PyObject *sipDo_Expression_getPredicate(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Expression)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		const String *a1 = NULL;
		PyObject *a1obj = NULL;

		if (sipParseArgs(sipArgs,"I|I",sipCanConvertTo_String,&a0obj,sipCanConvertTo_String,&a1obj))
		{
			ExpressionPredicate *res;
			Expression *ptr;

			if ((ptr = (Expression *)sipGetCppPtr(sipThis,sipClass_Expression)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Expression::getPredicate(* a0,* a1);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipMapCppToSelf(res,sipClass_ExpressionPredicate);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Expression,sipName_BALL_getPredicate);

	return NULL;
}

static PyObject *sipDo_Expression_setExpression(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Expression)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			Expression *ptr;

			if ((ptr = (Expression *)sipGetCppPtr(sipThis,sipClass_Expression)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Expression::setExpression(* a0);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Expression,sipName_BALL_setExpression);

	return NULL;
}

static PyObject *sipDo_Expression_getExpression(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Expression)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			const String *res;
			Expression *ptr;

			if ((ptr = (Expression *)sipGetCppPtr(sipThis,sipClass_Expression)) == NULL)
				return NULL;

			res = &ptr -> Expression::getExpression();

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Expression,sipName_BALL_getExpression);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Expression(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Expression)
		return ptr;

	return NULL;
}

static void sipDealloc_Expression(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipExpression *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (Expression *)sipThis -> u.cppPtr;
			else
				delete (sipExpression *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_Expression(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Expression
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
			sipNew = new sipExpression();
	}
	}

	if (sipNew == NULL)
	{
		const Expression *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_Expression,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Expression(a0obj,(Expression **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipExpression(* a0);
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

			sipNew = new sipExpression(* a0);

			if (istemp0)
				delete a0;
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_Expression);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Expression,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (Expression *)sipNew;
			else
				delete (sipExpression *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipExpression *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Expression[] = {
	{sipName_BALL_hasPredicate, sipDo_Expression_hasPredicate, METH_VARARGS, NULL},
	{sipName_BALL_Operator__call__, sipDo_Expression_Operator__call__, METH_VARARGS, NULL},
	{sipName_BALL_getPredicate, sipDo_Expression_getPredicate, METH_VARARGS, NULL},
	{sipName_BALL_setExpression, sipDo_Expression_setExpression, METH_VARARGS, NULL},
	{sipName_BALL_getExpression, sipDo_Expression_getExpression, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_Expression(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Expression);
}

void sipConvertTo_Expression(PyObject *sipPy,Expression **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_Expression);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Expression *)sipConvertToCpp(sipPy,sipClass_Expression,sipIsErr);
}

Expression *sipForceConvertTo_Expression(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Expression(valobj))
	{
		Expression *val;

		sipConvertTo_Expression(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Expression);
	*iserrp = 1;

	return NULL;
}
