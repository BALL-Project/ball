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

extern "C" PyObject *sipDo_Substring_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

   try
   {
			ptr -> Substring::destroy();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Substring,sipName_BALL_destroy);

	return NULL;
}

extern "C" PyObject *sipDo_Substring_toString(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			String *res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

   try
   {
			res = new String(ptr -> Substring::toString());
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipNewCppToSelf(res,sipClass_String,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Substring,sipName_BALL_toString);

	return NULL;
}

extern "C" PyObject *sipDo_Substring_bind(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		const String * a0;
		PyObject *a0obj;
		Index * a1 = NULL;
		PyObject *a1obj = NULL;
		int a2 = string::npos;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|Ii",sipCanConvertTo_String,&a0obj,sipCanConvertTo_Index,&a1obj,&a2))
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

   try
   {
			res = &ptr -> Substring::bind(* a0,* a1, a2);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipMapCppToSelf(res,sipClass_Substring);
		}
	}

	{
		const Substring * a0;
		PyObject *a0obj;
		Index * a1 = NULL;
		PyObject *a1obj = NULL;
		int a2 = string::npos;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|Ii",sipCanConvertTo_Substring,&a0obj,sipCanConvertTo_Index,&a1obj,&a2))
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

   try
   {
			res = &ptr -> Substring::bind(* a0,* a1, a2);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp1)
				delete a1;

			return sipMapCppToSelf(res,sipClass_Substring);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Substring,sipName_BALL_bind);

	return NULL;
}

extern "C" PyObject *sipDo_Substring_unbind(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

   try
   {
			ptr -> Substring::unbind();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Substring,sipName_BALL_unbind);

	return NULL;
}

extern "C" PyObject *sipDo_Substring_getBoundString(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			String *res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Substring::getBoundString();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const String *res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Substring::getBoundString();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Substring,sipName_BALL_getBoundString);

	return NULL;
}

extern "C" PyObject *sipDo_Substring_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Substring::set(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const Substring * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Substring,&a0obj))
		{
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Substring(a0obj,(Substring **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Substring::set(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const char * a0;
		int a1 = string::npos;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"s|i",&a0,&a1))
		{
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

   try
   {
			ptr -> Substring::set( a0, a1);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Substring,sipName_BALL_set);

	return NULL;
}

extern "C" PyObject *sipDo_Substring_c_str(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			char *res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Substring::c_str();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (res == NULL)
			{
				Py_INCREF(Py_None);
				return Py_None;
			}

			return PyString_FromString(res);
		}
	}

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const char *res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Substring::c_str();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (res == NULL)
			{
				Py_INCREF(Py_None);
				return Py_None;
			}

			return PyString_FromString(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Substring,sipName_BALL_c_str);

	return NULL;
}

extern "C" PyObject *sipDo_Substring_getFirstIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Index *res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

   try
   {
			res = new Index(ptr -> Substring::getFirstIndex());
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			PyObject *resobj = sipConvertFrom_Index(res);
			delete res;

			return resobj;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Substring,sipName_BALL_getFirstIndex);

	return NULL;
}

extern "C" PyObject *sipDo_Substring_getLastIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Index *res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

   try
   {
			res = new Index(ptr -> Substring::getLastIndex());
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			PyObject *resobj = sipConvertFrom_Index(res);
			delete res;

			return resobj;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Substring,sipName_BALL_getLastIndex);

	return NULL;
}

extern "C" PyObject *sipDo_Substring_size(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Substring::size();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Substring,sipName_BALL_size);

	return NULL;
}

extern "C" PyObject *sipDo_Substring_GetItemOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		Index * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Index,&a0obj))
		{
			char res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Substring::operator[](* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return PyString_FromStringAndSize(&res,1);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Substring,sipName_BALL_GetItemOp);

	return NULL;
}

extern "C" PyObject *sipDo_Substring_toLower(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Substring *res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

   try
   {
			res = &ptr -> Substring::toLower();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Substring);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Substring,sipName_BALL_toLower);

	return NULL;
}

extern "C" PyObject *sipDo_Substring_toUpper(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Substring *res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

   try
   {
			res = &ptr -> Substring::toUpper();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Substring);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Substring,sipName_BALL_toUpper);

	return NULL;
}

extern "C" PyObject *sipDo_Substring_isBound(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Substring::isBound();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Substring,sipName_BALL_isBound);

	return NULL;
}

extern "C" PyObject *sipDo_Substring_isEmpty(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Substring::isEmpty();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Substring,sipName_BALL_isEmpty);

	return NULL;
}

extern "C" PyObject *sipDo_Substring_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Substring)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Substring *ptr;

			if ((ptr = (Substring *)sipGetCppPtr(sipThis,sipClass_Substring)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Substring::isValid();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Substring,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_Substring(const void *ptr,PyObject *targetClass)
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
	int sipArgsParsed = 0;

	// See if there is something pending.

	sipNew = sipGetPending(&sipFlags);

	if (sipNew == NULL)
	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,"-"))
		{
   try
   {
			sipNew = new Substring();
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
		const Substring * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_Substring,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_Substring(a0obj,(Substring **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new Substring(* a0, (bool)a1);
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
		Index * a1 = NULL;
		PyObject *a1obj = NULL;
		int a2 = string::npos;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|Ii",sipCanConvertTo_String,&a0obj,sipCanConvertTo_Index,&a1obj,&a2))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new Substring(* a0,* a1, a2);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;
		}
	}

	if (sipNew == NULL)
	{
		const Substring * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_Substring,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Substring(a0obj,(Substring **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new Substring(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_Substring);
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
	{sipName_BALL_GetItemOp, sipDo_Substring_GetItemOp, METH_VARARGS, NULL},
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

void sipConvertTo_Substring(PyObject *sipPy,Substring **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_Substring);
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
