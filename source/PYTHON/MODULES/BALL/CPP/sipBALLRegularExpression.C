#include "sipBALLDeclBALL.h"
#include "sipBALLRegularExpression.h"



PyObject *sipClass_RegularExpression;

static void sipDealloc_RegularExpression(sipThisType *);

static PyTypeObject sipType_RegularExpression = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_RegularExpression,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_RegularExpression,
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

sipRegularExpression::sipRegularExpression()
    : RegularExpression()
{
	sipCommonCtor(sipPyMethods,2);
}

sipRegularExpression::sipRegularExpression(const RegularExpression& a0,bool a1)
    : RegularExpression(a0,a1)
{
	sipCommonCtor(sipPyMethods,2);
}

sipRegularExpression::sipRegularExpression(const String& a0,bool a1)
    : RegularExpression(a0,a1)
{
	sipCommonCtor(sipPyMethods,2);
}

sipRegularExpression::sipRegularExpression(const RegularExpression& a0)
    : RegularExpression(a0)
{
	sipCommonCtor(sipPyMethods,2);
}

sipRegularExpression::~sipRegularExpression()
  throw()
{
	sipCommonDtor(sipPyThis);
}

bool sipRegularExpression::isValid() const
 throw()
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_isValid,&relLock) ?
		sipRegularExpression::sipVH_isValid(&sipPyMethods[0],sipPyThis,relLock) :
		RegularExpression::isValid();
}

void sipRegularExpression::clear()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipRegularExpression::sipVH_clear(&sipPyMethods[1],sipPyThis,relLock);
	else
		RegularExpression::clear();
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipRegularExpression::sipVH_isValid(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_RegularExpression,sipName_BALL_isValid);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

void sipRegularExpression::sipVH_clear(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_RegularExpression,sipName_BALL_clear);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

static PyObject *sipDo_RegularExpression_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularExpression)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			RegularExpression *ptr;

			if ((ptr = (RegularExpression *)sipGetCppPtr(sipThis,sipClass_RegularExpression)) == NULL)
				return NULL;

			ptr -> RegularExpression::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularExpression,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_RegularExpression_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularExpression)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			RegularExpression *ptr;

			if ((ptr = (RegularExpression *)sipGetCppPtr(sipThis,sipClass_RegularExpression)) == NULL)
				return NULL;

			ptr -> RegularExpression::destroy();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularExpression,sipName_BALL_destroy);

	return NULL;
}

static PyObject *sipDo_RegularExpression_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularExpression)) == NULL)
		return NULL;

	{
		const RegularExpression *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_RegularExpression,&a0obj,&a1))
		{
			RegularExpression *ptr;

			if ((ptr = (RegularExpression *)sipGetCppPtr(sipThis,sipClass_RegularExpression)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_RegularExpression(a0obj,(RegularExpression **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> RegularExpression::set(* a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const String *a0;
		PyObject *a0obj;
		long a1 = false;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_String,&a0obj,&a1))
		{
			RegularExpression *ptr;

			if ((ptr = (RegularExpression *)sipGetCppPtr(sipThis,sipClass_RegularExpression)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> RegularExpression::set(* a0, (bool)a1);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularExpression,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_RegularExpression_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularExpression)) == NULL)
		return NULL;

	{
		RegularExpression *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_RegularExpression,&a0obj,&a1))
		{
			RegularExpression *ptr;

			if ((ptr = (RegularExpression *)sipGetCppPtr(sipThis,sipClass_RegularExpression)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_RegularExpression(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> RegularExpression::get(* a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularExpression,sipName_BALL_get);

	return NULL;
}

static PyObject *sipDo_RegularExpression_getPattern(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularExpression)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const String *res;
			RegularExpression *ptr;

			if ((ptr = (RegularExpression *)sipGetCppPtr(sipThis,sipClass_RegularExpression)) == NULL)
				return NULL;

			res = &ptr -> RegularExpression::getPattern();

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularExpression,sipName_BALL_getPattern);

	return NULL;
}

static PyObject *sipDo_RegularExpression_countSubexpressions(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularExpression)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			RegularExpression *ptr;

			if ((ptr = (RegularExpression *)sipGetCppPtr(sipThis,sipClass_RegularExpression)) == NULL)
				return NULL;

			res = ptr -> RegularExpression::countSubexpressions();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularExpression,sipName_BALL_countSubexpressions);

	return NULL;
}

static PyObject *sipDo_RegularExpression_match(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularExpression)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		Index *a1 = NULL;
		PyObject *a1obj = NULL;
		int a2 = 0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|Ii",sipCanConvertTo_String,&a0obj,sipCanConvertTo_Index,&a1obj,&a2))
		{
			bool res;
			RegularExpression *ptr;

			if ((ptr = (RegularExpression *)sipGetCppPtr(sipThis,sipClass_RegularExpression)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> RegularExpression::match(* a0,* a1, a2);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Substring *a0;
		PyObject *a0obj;
		Index *a1 = NULL;
		PyObject *a1obj = NULL;
		int a2 = 0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|Ii",sipCanConvertTo_Substring,&a0obj,sipCanConvertTo_Index,&a1obj,&a2))
		{
			bool res;
			RegularExpression *ptr;

			if ((ptr = (RegularExpression *)sipGetCppPtr(sipThis,sipClass_RegularExpression)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Substring(a0obj,(Substring **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> RegularExpression::match(* a0,* a1, a2);

			if (istemp1)
				delete a1;

			return sipConvertFromBool((int)res);
		}
	}

	{
		const char *a0;
		int a1 = 0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"s|i",&a0,&a1))
		{
			bool res;
			RegularExpression *ptr;

			if ((ptr = (RegularExpression *)sipGetCppPtr(sipThis,sipClass_RegularExpression)) == NULL)
				return NULL;

			res = ptr -> RegularExpression::match( a0, a1);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularExpression,sipName_BALL_match);

	return NULL;
}

static PyObject *sipDo_RegularExpression_find(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularExpression)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		Substring *a1;
		PyObject *a1obj;
		Index *a2 = NULL;
		PyObject *a2obj = NULL;
		int a3 = 0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II|Ii",sipCanConvertTo_String,&a0obj,sipCanConvertTo_Substring,&a1obj,sipCanConvertTo_Index,&a2obj,&a3))
		{
			bool res;
			RegularExpression *ptr;

			if ((ptr = (RegularExpression *)sipGetCppPtr(sipThis,sipClass_RegularExpression)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			sipConvertTo_Substring(a1obj,&a1,1,&iserr);
			int istemp2 = sipConvertTo_Index(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> RegularExpression::find(* a0,* a1,* a2, a3);

			if (istemp0)
				delete a0;

			if (istemp2)
				delete a2;

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Substring *a0;
		PyObject *a0obj;
		Substring *a1;
		PyObject *a1obj;
		Index *a2 = NULL;
		PyObject *a2obj = NULL;
		int a3 = 0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II|Ii",sipCanConvertTo_Substring,&a0obj,sipCanConvertTo_Substring,&a1obj,sipCanConvertTo_Index,&a2obj,&a3))
		{
			bool res;
			RegularExpression *ptr;

			if ((ptr = (RegularExpression *)sipGetCppPtr(sipThis,sipClass_RegularExpression)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Substring(a0obj,(Substring **)&a0,1,&iserr);
			sipConvertTo_Substring(a1obj,&a1,1,&iserr);
			int istemp2 = sipConvertTo_Index(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> RegularExpression::find(* a0,* a1,* a2, a3);

			if (istemp2)
				delete a2;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularExpression,sipName_BALL_find);

	return NULL;
}

static PyObject *sipDo_RegularExpression_isEmpty(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularExpression)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			RegularExpression *ptr;

			if ((ptr = (RegularExpression *)sipGetCppPtr(sipThis,sipClass_RegularExpression)) == NULL)
				return NULL;

			res = ptr -> RegularExpression::isEmpty();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularExpression,sipName_BALL_isEmpty);

	return NULL;
}

static PyObject *sipDo_RegularExpression_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularExpression)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			RegularExpression *ptr;

			if ((ptr = (RegularExpression *)sipGetCppPtr(sipThis,sipClass_RegularExpression)) == NULL)
				return NULL;

			res = ptr -> RegularExpression::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularExpression,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_RegularExpression(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_RegularExpression)
		return ptr;

	return NULL;
}

static void sipDealloc_RegularExpression(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipRegularExpression *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (RegularExpression *)sipThis -> u.cppPtr;
			else
				delete (sipRegularExpression *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_RegularExpression(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_RegularExpression
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
			sipNew = new sipRegularExpression();
		}
	}

	if (sipNew == NULL)
	{
		const RegularExpression *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_RegularExpression,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_RegularExpression(a0obj,(RegularExpression **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipRegularExpression(* a0, (bool)a1);
		}
	}

	if (sipNew == NULL)
	{
		const String *a0;
		PyObject *a0obj;
		long a1 = false;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_String,&a0obj,&a1))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipRegularExpression(* a0, (bool)a1);

			if (istemp0)
				delete a0;
		}
	}

	if (sipNew == NULL)
	{
		const RegularExpression *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_RegularExpression,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_RegularExpression(a0obj,(RegularExpression **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipRegularExpression(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_RegularExpression);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_RegularExpression,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (RegularExpression *)sipNew;
			else
				delete (sipRegularExpression *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipRegularExpression *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_RegularExpression[] = {
	{sipName_BALL_clear, sipDo_RegularExpression_clear, METH_VARARGS, NULL},
	{sipName_BALL_destroy, sipDo_RegularExpression_destroy, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_RegularExpression_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_RegularExpression_get, METH_VARARGS, NULL},
	{sipName_BALL_getPattern, sipDo_RegularExpression_getPattern, METH_VARARGS, NULL},
	{sipName_BALL_countSubexpressions, sipDo_RegularExpression_countSubexpressions, METH_VARARGS, NULL},
	{sipName_BALL_match, sipDo_RegularExpression_match, METH_VARARGS, NULL},
	{sipName_BALL_find, sipDo_RegularExpression_find, METH_VARARGS, NULL},
	{sipName_BALL_isEmpty, sipDo_RegularExpression_isEmpty, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_RegularExpression_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_RegularExpression(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_RegularExpression);
}

void sipConvertTo_RegularExpression(PyObject *sipPy,RegularExpression **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_RegularExpression);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (RegularExpression *)sipConvertToCpp(sipPy,sipClass_RegularExpression,sipIsErr);
}

RegularExpression *sipForceConvertTo_RegularExpression(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_RegularExpression(valobj))
	{
		RegularExpression *val;

		sipConvertTo_RegularExpression(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_RegularExpression);
	*iserrp = 1;

	return NULL;
}
