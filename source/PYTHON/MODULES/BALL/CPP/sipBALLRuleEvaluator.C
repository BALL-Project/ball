#include "sipBALLDeclBALL.h"
#include "sipBALLRuleEvaluator.h"



PyObject *sipClass_RuleEvaluator;

static void sipDealloc_RuleEvaluator(sipThisType *);

static PyTypeObject sipType_RuleEvaluator = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_RuleEvaluator,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_RuleEvaluator,
	0,
	0,
	0,
	0,
	0,
};

sipRuleEvaluator::sipRuleEvaluator(): RuleEvaluator()
{
	sipCommonCtor(sipPyMethods,1);
}

sipRuleEvaluator::sipRuleEvaluator(INIFile& a0,const String& a1): RuleEvaluator(a0,a1)
{
	sipCommonCtor(sipPyMethods,1);
}

sipRuleEvaluator::sipRuleEvaluator(const RuleEvaluator& a0): RuleEvaluator(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipRuleEvaluator::~sipRuleEvaluator()
{
	sipCommonDtor(sipPyThis);
}
void sipRuleEvaluator::clear()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipRuleEvaluator::sipVH_clear(&sipPyMethods[0],sipPyThis,relLock);
	else
		RuleEvaluator::clear();
}

// The common handler for all classes that inherit this virtual member
// function.

void sipRuleEvaluator::sipVH_clear(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
{
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(O)",sipThis -> sipSelf);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_RuleEvaluator,sipName_BALL_clear);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

static PyObject *sipDo_RuleEvaluator_initialize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RuleEvaluator)) == NULL)
		return NULL;

	{
		INIFile *a0;
		PyObject *a0obj;
		const String *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_INIFile,&a0obj,sipCanConvertTo_String,&a1obj))
		{
			bool res;
			RuleEvaluator *ptr;

			if ((ptr = (RuleEvaluator *)sipGetCppPtr(sipThis,sipClass_RuleEvaluator)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_INIFile(a0obj,&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> RuleEvaluator::initialize(* a0,* a1);

			if (istemp1)
				delete a1;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_RuleEvaluator,sipName_BALL_initialize);

	return NULL;
}

static PyObject *sipDo_RuleEvaluator_getPrefix(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RuleEvaluator)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			const String *res;
			RuleEvaluator *ptr;

			if ((ptr = (RuleEvaluator *)sipGetCppPtr(sipThis,sipClass_RuleEvaluator)) == NULL)
				return NULL;

			res = &ptr -> RuleEvaluator::getPrefix();

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_RuleEvaluator,sipName_BALL_getPrefix);

	return NULL;
}

static PyObject *sipDo_RuleEvaluator_setPrefix(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RuleEvaluator)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			RuleEvaluator *ptr;

			if ((ptr = (RuleEvaluator *)sipGetCppPtr(sipThis,sipClass_RuleEvaluator)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> RuleEvaluator::setPrefix(* a0);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_RuleEvaluator,sipName_BALL_setPrefix);

	return NULL;
}

static PyObject *sipDo_RuleEvaluator_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RuleEvaluator)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			RuleEvaluator *ptr;

			if ((ptr = (RuleEvaluator *)sipGetCppPtr(sipThis,sipClass_RuleEvaluator)) == NULL)
				return NULL;

			ptr -> RuleEvaluator::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_RuleEvaluator,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_RuleEvaluator_Operator__call__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RuleEvaluator)) == NULL)
		return NULL;

	{
		const Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			String *res;
			RuleEvaluator *ptr;

			if ((ptr = (RuleEvaluator *)sipGetCppPtr(sipThis,sipClass_RuleEvaluator)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,(Atom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = new String(ptr -> RuleEvaluator::operator()(* a0));

			return sipNewCppToSelf(res,sipClass_String,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_RuleEvaluator,sipName_BALL_Operator__call__);

	return NULL;
}

static PyObject *sipDo_RuleEvaluator_Operator__cmp__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RuleEvaluator)) == NULL)
		return NULL;

	{
		const RuleEvaluator *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_RuleEvaluator,&a0obj))
		{
			bool res;
			RuleEvaluator *ptr;

			if ((ptr = (RuleEvaluator *)sipGetCppPtr(sipThis,sipClass_RuleEvaluator)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_RuleEvaluator(a0obj,(RuleEvaluator **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> RuleEvaluator::operator==(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_RuleEvaluator,sipName_BALL_Operator__cmp__);

	return NULL;
}

static PyObject *sipDo_RuleEvaluator_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RuleEvaluator)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			RuleEvaluator *ptr;

			if ((ptr = (RuleEvaluator *)sipGetCppPtr(sipThis,sipClass_RuleEvaluator)) == NULL)
				return NULL;

			res = ptr -> RuleEvaluator::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_RuleEvaluator,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_RuleEvaluator(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_RuleEvaluator)
		return ptr;

	return NULL;
}

static void sipDealloc_RuleEvaluator(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipRuleEvaluator *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (RuleEvaluator *)sipThis -> u.cppPtr;
			else
				delete (sipRuleEvaluator *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_RuleEvaluator(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_RuleEvaluator
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
			sipNew = new sipRuleEvaluator();
	}
	}

	if (sipNew == NULL)
	{
		INIFile *a0;
		PyObject *a0obj;
		const String *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_INIFile,&a0obj,sipCanConvertTo_String,&a1obj))
		{
			int iserr = 0;

			sipConvertTo_INIFile(a0obj,&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipRuleEvaluator(* a0,* a1);

			if (istemp1)
				delete a1;
	}
	}

	if (sipNew == NULL)
	{
		const RuleEvaluator *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_RuleEvaluator,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_RuleEvaluator(a0obj,(RuleEvaluator **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipRuleEvaluator(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_RuleEvaluator);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_RuleEvaluator,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (RuleEvaluator *)sipNew;
			else
				delete (sipRuleEvaluator *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipRuleEvaluator *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_RuleEvaluator[] = {
	{sipName_BALL_initialize, sipDo_RuleEvaluator_initialize, METH_VARARGS, NULL},
	{sipName_BALL_getPrefix, sipDo_RuleEvaluator_getPrefix, METH_VARARGS, NULL},
	{sipName_BALL_setPrefix, sipDo_RuleEvaluator_setPrefix, METH_VARARGS, NULL},
	{sipName_BALL_clear, sipDo_RuleEvaluator_clear, METH_VARARGS, NULL},
	{sipName_BALL_Operator__call__, sipDo_RuleEvaluator_Operator__call__, METH_VARARGS, NULL},
	{sipName_BALL_Operator__cmp__, sipDo_RuleEvaluator_Operator__cmp__, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_RuleEvaluator_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_RuleEvaluator(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_RuleEvaluator);
}

void sipConvertTo_RuleEvaluator(PyObject *sipPy,RuleEvaluator **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_RuleEvaluator);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (RuleEvaluator *)sipConvertToCpp(sipPy,sipClass_RuleEvaluator,sipIsErr);
}

RuleEvaluator *sipForceConvertTo_RuleEvaluator(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_RuleEvaluator(valobj))
	{
		RuleEvaluator *val;

		sipConvertTo_RuleEvaluator(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_RuleEvaluator);
	*iserrp = 1;

	return NULL;
}
