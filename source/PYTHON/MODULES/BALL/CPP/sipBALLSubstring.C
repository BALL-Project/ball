#include "sipBALLDeclBALL.h"
#include "sipBALLSubstring.h"



PyObject *sipClass_Substring;

static void sipDealloc_Substring(sipThisType *);

static PyTypeObject sipType_Substring = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Substring,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Substring,
	0,
	0,
	0,
	0,
	0,
};

static PyObject *sipDo_Substring_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			ptr -> Substring::destroy();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Substring,sipName_BALL_destroy);

	return NULL;
}

static PyObject *sipDo_Substring_toString(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			String *res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			res = new String(ptr -> Substring::toString());

			return sipNewCppToSelf(res,sipClass_String,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Substring,sipName_BALL_toString);

	return NULL;
}

static PyObject *sipDo_Substring_bind(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		Index *a1 = NULL;
		PyObject *a1obj = NULL;
		int a2 = string::npos;

		if (sipParseArgs(sipArgs,"I|Ii",sipCanConvertTo_String,&a0obj,sipCanConvertTo_Index,&a1obj,&a2))
		{
			Substring *res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = &ptr -> Substring::bind(* a0,* a1, a2);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipMapCppToSelf(res,sipClass_Substring);
		}
	}

	{
		const Substring *a0;
		PyObject *a0obj;
		Index *a1 = NULL;
		PyObject *a1obj = NULL;
		int a2 = string::npos;

		if (sipParseArgs(sipArgs,"I|Ii",sipCanConvertTo_Substring,&a0obj,sipCanConvertTo_Index,&a1obj,&a2))
		{
			Substring *res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Substring(a0obj,(Substring **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = &ptr -> Substring::bind(* a0,* a1, a2);

			if (istemp1)
				delete a1;

			return sipMapCppToSelf(res,sipClass_Substring);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Substring,sipName_BALL_bind);

	return NULL;
}

static PyObject *sipDo_Substring_unbind(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			ptr -> Substring::unbind();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Substring,sipName_BALL_unbind);

	return NULL;
}

static PyObject *sipDo_Substring_getBoundString(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			String *res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			res = ptr -> Substring::getBoundString();

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const String *res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			res = ptr -> Substring::getBoundString();

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Substring,sipName_BALL_getBoundString);

	return NULL;
}

static PyObject *sipDo_Substring_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Substring::set(* a0);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const Substring *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Substring,&a0obj))
		{
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Substring(a0obj,(Substring **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Substring::set(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const char *a0;
		int a1 = string::npos;

		if (sipParseArgs(sipArgs,"s|i",&a0,&a1))
		{
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			ptr -> Substring::set( a0, a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Substring,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_Substring_c_str(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			char *res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			res = ptr -> Substring::c_str();

			if (res == NULL)
			{
				Py_INCREF(Py_None);
				return Py_None;
			}

			return PyString_FromString(res);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const char *res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			res = ptr -> Substring::c_str();

			if (res == NULL)
			{
				Py_INCREF(Py_None);
				return Py_None;
			}

			return PyString_FromString(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Substring,sipName_BALL_c_str);

	return NULL;
}

static PyObject *sipDo_Substring_getFirstIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Index *res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			res = new Index(ptr -> Substring::getFirstIndex());

			return sipNewCppToSelf(res,sipClass_Index,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Substring,sipName_BALL_getFirstIndex);

	return NULL;
}

static PyObject *sipDo_Substring_getLastIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Index *res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			res = new Index(ptr -> Substring::getLastIndex());

			return sipNewCppToSelf(res,sipClass_Index,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Substring,sipName_BALL_getLastIndex);

	return NULL;
}

static PyObject *sipDo_Substring_size(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			res = ptr -> Substring::size();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Substring,sipName_BALL_size);

	return NULL;
}

static PyObject *sipDo_Substring_Operator__getitem__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		Index *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Index,&a0obj))
		{
			char res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Substring::operator[](* a0);

			if (istemp0)
				delete a0;

			return PyString_FromStringAndSize(&res,1);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Substring,sipName_BALL_Operator__getitem__);

	return NULL;
}

static PyObject *sipDo_Substring_toLower(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Substring *res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			res = &ptr -> Substring::toLower();

			return sipMapCppToSelf(res,sipClass_Substring);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Substring,sipName_BALL_toLower);

	return NULL;
}

static PyObject *sipDo_Substring_toUpper(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Substring *res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			res = &ptr -> Substring::toUpper();

			return sipMapCppToSelf(res,sipClass_Substring);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Substring,sipName_BALL_toUpper);

	return NULL;
}

static PyObject *sipDo_Substring_isBound(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			res = ptr -> Substring::isBound();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Substring,sipName_BALL_isBound);

	return NULL;
}

static PyObject *sipDo_Substring_isEmpty(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			res = ptr -> Substring::isEmpty();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Substring,sipName_BALL_isEmpty);

	return NULL;
}

static PyObject *sipDo_Substring_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			res = ptr -> Substring::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Substring,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Substring(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Substring)
		return ptr;

	return NULL;
}

static void sipDealloc_Substring(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (Substring *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_Substring(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Substring
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
			sipNew = new Substring();
	}
	}

	if (sipNew == NULL)
	{
		const Substring *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_Substring,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_Substring(a0obj,(Substring **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Substring(* a0, (bool)a1);
	}
	}

	if (sipNew == NULL)
	{
		const String *a0;
		PyObject *a0obj;
		Index *a1 = NULL;
		PyObject *a1obj = NULL;
		int a2 = string::npos;

		if (sipParseArgs(sipArgs,"-I|Ii",sipCanConvertTo_String,&a0obj,sipCanConvertTo_Index,&a1obj,&a2))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Substring(* a0,* a1, a2);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;
	}
	}

	if (sipNew == NULL)
	{
		const Substring *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_Substring,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Substring(a0obj,(Substring **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Substring(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_Substring);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Substring,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (Substring *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Substring[] = {
	{sipName_BALL_destroy, sipDo_Substring_destroy, METH_VARARGS, NULL},
	{sipName_BALL_toString, sipDo_Substring_toString, METH_VARARGS, NULL},
	{sipName_BALL_bind, sipDo_Substring_bind, METH_VARARGS, NULL},
	{sipName_BALL_unbind, sipDo_Substring_unbind, METH_VARARGS, NULL},
	{sipName_BALL_getBoundString, sipDo_Substring_getBoundString, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_Substring_set, METH_VARARGS, NULL},
	{sipName_BALL_c_str, sipDo_Substring_c_str, METH_VARARGS, NULL},
	{sipName_BALL_getFirstIndex, sipDo_Substring_getFirstIndex, METH_VARARGS, NULL},
	{sipName_BALL_getLastIndex, sipDo_Substring_getLastIndex, METH_VARARGS, NULL},
	{sipName_BALL_size, sipDo_Substring_size, METH_VARARGS, NULL},
	{sipName_BALL_Operator__getitem__, sipDo_Substring_Operator__getitem__, METH_VARARGS, NULL},
	{sipName_BALL_toLower, sipDo_Substring_toLower, METH_VARARGS, NULL},
	{sipName_BALL_toUpper, sipDo_Substring_toUpper, METH_VARARGS, NULL},
	{sipName_BALL_isBound, sipDo_Substring_isBound, METH_VARARGS, NULL},
	{sipName_BALL_isEmpty, sipDo_Substring_isEmpty, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_Substring_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_Substring(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Substring);
}

void sipConvertTo_Substring(PyObject *sipPy,Substring **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_Substring);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Substring *)sipConvertToCpp(sipPy,sipClass_Substring,sipIsErr);
}

Substring *sipForceConvertTo_Substring(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Substring(valobj))
	{
		Substring *val;

		sipConvertTo_Substring(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Substring);
	*iserrp = 1;

	return NULL;
}
