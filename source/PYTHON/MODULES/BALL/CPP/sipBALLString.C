#include "sipBALLDeclBALL.h"
#include "sipBALLString.h"



PyObject *sipClass_String;

static void sipDealloc_String(sipThisType *);
static PyObject *sipPyInternalRepr_String(sipThisType *);

static PyTypeObject sipType_String = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_String,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_String,
	0,
	0,
	0,
	0,
	(reprfunc)sipPyInternalRepr_String,
};

static PyObject *sipDo_String_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			ptr -> String::destroy();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_destroy);

	return NULL;
}

static PyObject *sipDo_String_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		Index *a1 = NULL;
		PyObject *a1obj = NULL;
		Size *a2 = (Size *)&string::npos;
		PyObject *a2obj = NULL;

		if (sipParseArgs(sipArgs,"I|II",sipCanConvertTo_String,&a0obj,sipCanConvertTo_Index,&a1obj,sipCanConvertTo_Size,&a2obj))
		{
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);
			int istemp2 = sipConvertTo_Size(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> String::set(* a0,* a1,* a2);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			if (istemp2)
				delete a2;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const char *a0;
		Index *a1 = NULL;
		PyObject *a1obj = NULL;
		Size *a2 = (Size *)&string::npos;
		PyObject *a2obj = NULL;

		if (sipParseArgs(sipArgs,"s|II",&a0,sipCanConvertTo_Index,&a1obj,sipCanConvertTo_Size,&a2obj))
		{
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);
			int istemp2 = sipConvertTo_Size(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> String::set( a0,* a1,* a2);

			if (istemp1)
				delete a1;

			if (istemp2)
				delete a2;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		char a0;
		Size *a1 = NULL;
		PyObject *a1obj = NULL;

		if (sipParseArgs(sipArgs,"c|I",&a0,sipCanConvertTo_Size,&a1obj))
		{
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp1 = sipConvertTo_Size(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> String::set( a0,* a1);

			if (istemp1)
				delete a1;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		short a0;

		if (sipParseArgs(sipArgs,"h",&a0))
		{
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			ptr -> String::set( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		int a0;

		if (sipParseArgs(sipArgs,"i",&a0))
		{
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			ptr -> String::set( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		long a0;

		if (sipParseArgs(sipArgs,"l",&a0))
		{
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			ptr -> String::set( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		float a0;

		if (sipParseArgs(sipArgs,"f",&a0))
		{
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			ptr -> String::set( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		double a0;

		if (sipParseArgs(sipArgs,"d",&a0))
		{
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			ptr -> String::set( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_String_setCompareMode(PyObject *,PyObject *sipArgs)
{

	{
		long a0;

		if (sipParseArgs(sipArgs,"-l",&a0))
		{

			String::setCompareMode( (String::CompareMode)a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_setCompareMode);

	return NULL;
}

static PyObject *sipDo_String_getCompareMode(PyObject *,PyObject *sipArgs)
{

	{
		if (sipParseArgs(sipArgs,"-"))
		{
			String::CompareMode res;

			res = String::getCompareMode();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_getCompareMode);

	return NULL;
}

static PyObject *sipDo_String_toBool(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = ptr -> String::toBool();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_toBool);

	return NULL;
}

static PyObject *sipDo_String_toChar(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			char res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = ptr -> String::toChar();

			return PyString_FromStringAndSize(&res,1);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_toChar);

	return NULL;
}

static PyObject *sipDo_String_toShort(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			short res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = ptr -> String::toShort();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_toShort);

	return NULL;
}

static PyObject *sipDo_String_toInt(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = ptr -> String::toInt();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_toInt);

	return NULL;
}

static PyObject *sipDo_String_toLong(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			long res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = ptr -> String::toLong();

			return PyInt_FromLong(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_toLong);

	return NULL;
}

static PyObject *sipDo_String_toFloat(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = ptr -> String::toFloat();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_toFloat);

	return NULL;
}

static PyObject *sipDo_String_toDouble(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			double res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = ptr -> String::toDouble();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_toDouble);

	return NULL;
}

static PyObject *sipDo_String_toLower(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		Index *a0 = NULL;
		PyObject *a0obj = NULL;
		Size *a1 = (Size *)&string::npos;
		PyObject *a1obj = NULL;

		if (sipParseArgs(sipArgs,"|II",sipCanConvertTo_Index,&a0obj,sipCanConvertTo_Size,&a1obj))
		{
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);
			int istemp1 = sipConvertTo_Size(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> String::toLower(* a0,* a1);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_toLower);

	return NULL;
}

static PyObject *sipDo_String_toUpper(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		Index *a0 = NULL;
		PyObject *a0obj = NULL;
		Size *a1 = (Size *)&string::npos;
		PyObject *a1obj = NULL;

		if (sipParseArgs(sipArgs,"|II",sipCanConvertTo_Index,&a0obj,sipCanConvertTo_Size,&a1obj))
		{
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);
			int istemp1 = sipConvertTo_Size(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> String::toUpper(* a0,* a1);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_toUpper);

	return NULL;
}

static PyObject *sipDo_String_getSubstring(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		Index *a0 = NULL;
		PyObject *a0obj = NULL;
		Size *a1 = (Size *)&string::npos;
		PyObject *a1obj = NULL;

		if (sipParseArgs(sipArgs,"|II",sipCanConvertTo_Index,&a0obj,sipCanConvertTo_Size,&a1obj))
		{
			Substring *res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);
			int istemp1 = sipConvertTo_Size(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = new Substring(ptr -> String::getSubstring(* a0,* a1));

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipNewCppToSelf(res,sipClass_Substring,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_getSubstring);

	return NULL;
}

static PyObject *sipDo_String_Operator__call__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		Index *a0;
		PyObject *a0obj;
		Size *a1 = (Size *)&string::npos;
		PyObject *a1obj = NULL;

		if (sipParseArgs(sipArgs,"I|I",sipCanConvertTo_Index,&a0obj,sipCanConvertTo_Size,&a1obj))
		{
			Substring *res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);
			int istemp1 = sipConvertTo_Size(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = new Substring(ptr -> String::operator()(* a0,* a1));

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipNewCppToSelf(res,sipClass_Substring,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_Operator__call__);

	return NULL;
}

static PyObject *sipDo_String_before(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		Index *a1 = NULL;
		PyObject *a1obj = NULL;

		if (sipParseArgs(sipArgs,"I|I",sipCanConvertTo_String,&a0obj,sipCanConvertTo_Index,&a1obj))
		{
			Substring *res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = new Substring(ptr -> String::before(* a0,* a1));

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipNewCppToSelf(res,sipClass_Substring,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_before);

	return NULL;
}

static PyObject *sipDo_String_through(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		Index *a1 = NULL;
		PyObject *a1obj = NULL;

		if (sipParseArgs(sipArgs,"I|I",sipCanConvertTo_String,&a0obj,sipCanConvertTo_Index,&a1obj))
		{
			Substring *res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = new Substring(ptr -> String::through(* a0,* a1));

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipNewCppToSelf(res,sipClass_Substring,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_through);

	return NULL;
}

static PyObject *sipDo_String_from(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		Index *a1 = NULL;
		PyObject *a1obj = NULL;

		if (sipParseArgs(sipArgs,"I|I",sipCanConvertTo_String,&a0obj,sipCanConvertTo_Index,&a1obj))
		{
			Substring *res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = new Substring(ptr -> String::from(* a0,* a1));

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipNewCppToSelf(res,sipClass_Substring,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_from);

	return NULL;
}

static PyObject *sipDo_String_after(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		Index *a1 = NULL;
		PyObject *a1obj = NULL;

		if (sipParseArgs(sipArgs,"I|I",sipCanConvertTo_String,&a0obj,sipCanConvertTo_Index,&a1obj))
		{
			Substring *res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = new Substring(ptr -> String::after(* a0,* a1));

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipNewCppToSelf(res,sipClass_Substring,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_after);

	return NULL;
}

static PyObject *sipDo_String_countFields(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		const char *a0 = String::CHARACTER_CLASS__WHITESPACE;

		if (sipParseArgs(sipArgs,"|s",&a0))
		{
			Size *res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = new Size(ptr -> String::countFields( a0));

			return sipNewCppToSelf(res,sipClass_Size,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_countFields);

	return NULL;
}

static PyObject *sipDo_String_getField(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		Index *a0;
		PyObject *a0obj;
		const char *a1 = String::CHARACTER_CLASS__WHITESPACE;

		if (sipParseArgs(sipArgs,"I|s",sipCanConvertTo_Index,&a0obj,&a1))
		{
			String *res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = new String(ptr -> String::getField(* a0, a1));

			if (istemp0)
				delete a0;

			return sipNewCppToSelf(res,sipClass_String,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_getField);

	return NULL;
}

static PyObject *sipDo_String_trimLeft(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		const char *a0 = String::CHARACTER_CLASS__WHITESPACE;

		if (sipParseArgs(sipArgs,"|s",&a0))
		{
			String *res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = &ptr -> String::trimLeft( a0);

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_trimLeft);

	return NULL;
}

static PyObject *sipDo_String_trimRight(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		const char *a0 = String::CHARACTER_CLASS__WHITESPACE;

		if (sipParseArgs(sipArgs,"|s",&a0))
		{
			String *res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = &ptr -> String::trimRight( a0);

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_trimRight);

	return NULL;
}

static PyObject *sipDo_String_trim(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		const char *a0 = String::CHARACTER_CLASS__WHITESPACE;

		if (sipParseArgs(sipArgs,"|s",&a0))
		{
			String *res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = &ptr -> String::trim( a0);

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_trim);

	return NULL;
}

static PyObject *sipDo_String_truncate(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		Size *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Size,&a0obj))
		{
			String *res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Size(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = &ptr -> String::truncate(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_truncate);

	return NULL;
}

static PyObject *sipDo_String_left(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		Size *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Size,&a0obj))
		{
			Substring *res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Size(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = new Substring(ptr -> String::left(* a0));

			if (istemp0)
				delete a0;

			return sipNewCppToSelf(res,sipClass_Substring,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_left);

	return NULL;
}

static PyObject *sipDo_String_right(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		Size *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Size,&a0obj))
		{
			Substring *res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Size(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = new Substring(ptr -> String::right(* a0));

			if (istemp0)
				delete a0;

			return sipNewCppToSelf(res,sipClass_Substring,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_right);

	return NULL;
}

static PyObject *sipDo_String_instr(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		Index *a1 = NULL;
		PyObject *a1obj = NULL;

		if (sipParseArgs(sipArgs,"I|I",sipCanConvertTo_String,&a0obj,sipCanConvertTo_Index,&a1obj))
		{
			Substring *res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = new Substring(ptr -> String::instr(* a0,* a1));

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipNewCppToSelf(res,sipClass_Substring,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_instr);

	return NULL;
}

static PyObject *sipDo_String_Operator__add__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		const char *a0;

		if (sipParseArgs(sipArgs,"s",&a0))
		{
			String *res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = new String(ptr -> String::operator+( a0));

			return sipNewCppToSelf(res,sipClass_String,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	{
		char a0;

		if (sipParseArgs(sipArgs,"c",&a0))
		{
			String *res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = new String(ptr -> String::operator+( a0));

			return sipNewCppToSelf(res,sipClass_String,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_Operator__add__);

	return NULL;
}

static PyObject *sipDo_String_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> String::swap(* a0);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_swap);

	return NULL;
}

static PyObject *sipDo_String_reverse(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		Index *a0 = NULL;
		PyObject *a0obj = NULL;
		Size *a1 = (Size *)&string::npos;
		PyObject *a1obj = NULL;

		if (sipParseArgs(sipArgs,"|II",sipCanConvertTo_Index,&a0obj,sipCanConvertTo_Size,&a1obj))
		{
			String *res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);
			int istemp1 = sipConvertTo_Size(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = &ptr -> String::reverse(* a0,* a1);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_reverse);

	return NULL;
}

static PyObject *sipDo_String_substitute(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		const String *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_String,&a0obj,sipCanConvertTo_String,&a1obj))
		{
			Index *res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = new Index(ptr -> String::substitute(* a0,* a1));

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipNewCppToSelf(res,sipClass_Index,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_substitute);

	return NULL;
}

static PyObject *sipDo_String_has(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		char a0;

		if (sipParseArgs(sipArgs,"c",&a0))
		{
			bool res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = ptr -> String::has( a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_has);

	return NULL;
}

static PyObject *sipDo_String_hasSubstring(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		Index *a1 = NULL;
		PyObject *a1obj = NULL;

		if (sipParseArgs(sipArgs,"I|I",sipCanConvertTo_String,&a0obj,sipCanConvertTo_Index,&a1obj))
		{
			bool res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> String::hasSubstring(* a0,* a1);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_hasSubstring);

	return NULL;
}

static PyObject *sipDo_String_hasPrefix(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			bool res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> String::hasPrefix(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_hasPrefix);

	return NULL;
}

static PyObject *sipDo_String_hasSuffix(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			bool res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> String::hasSuffix(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_hasSuffix);

	return NULL;
}

static PyObject *sipDo_String_isEmpty(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = ptr -> String::isEmpty();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_isEmpty);

	return NULL;
}

static PyObject *sipDo_String_isAlpha(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = ptr -> String::isAlpha();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_isAlpha);

	return NULL;
}

static PyObject *sipDo_String_isAlnum(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = ptr -> String::isAlnum();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_isAlnum);

	return NULL;
}

static PyObject *sipDo_String_isDigit(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = ptr -> String::isDigit();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_isDigit);

	return NULL;
}

static PyObject *sipDo_String_isSpace(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = ptr -> String::isSpace();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_isSpace);

	return NULL;
}

static PyObject *sipDo_String_isWhitespace(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = ptr -> String::isWhitespace();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_isWhitespace);

	return NULL;
}

static PyObject *sipDo_String_compare(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		Index *a1 = NULL;
		PyObject *a1obj = NULL;

		if (sipParseArgs(sipArgs,"I|I",sipCanConvertTo_String,&a0obj,sipCanConvertTo_Index,&a1obj))
		{
			int res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> String::compare(* a0,* a1);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return PyInt_FromLong((long)res);
		}
	}

	{
		const String *a0;
		PyObject *a0obj;
		Index *a1;
		PyObject *a1obj;
		Size *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"III",sipCanConvertTo_String,&a0obj,sipCanConvertTo_Index,&a1obj,sipCanConvertTo_Size,&a2obj))
		{
			int res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);
			int istemp2 = sipConvertTo_Size(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> String::compare(* a0,* a1,* a2);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			if (istemp2)
				delete a2;

			return PyInt_FromLong((long)res);
		}
	}

	{
		const char *a0;
		Index *a1 = NULL;
		PyObject *a1obj = NULL;

		if (sipParseArgs(sipArgs,"s|I",&a0,sipCanConvertTo_Index,&a1obj))
		{
			int res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> String::compare( a0,* a1);

			if (istemp1)
				delete a1;

			return PyInt_FromLong((long)res);
		}
	}

	{
		const char *a0;
		Index *a1;
		PyObject *a1obj;
		Size *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"sII",&a0,sipCanConvertTo_Index,&a1obj,sipCanConvertTo_Size,&a2obj))
		{
			int res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);
			int istemp2 = sipConvertTo_Size(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> String::compare( a0,* a1,* a2);

			if (istemp1)
				delete a1;

			if (istemp2)
				delete a2;

			return PyInt_FromLong((long)res);
		}
	}

	{
		char a0;
		Index *a1 = NULL;
		PyObject *a1obj = NULL;

		if (sipParseArgs(sipArgs,"c|I",&a0,sipCanConvertTo_Index,&a1obj))
		{
			int res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> String::compare( a0,* a1);

			if (istemp1)
				delete a1;

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_compare);

	return NULL;
}

static PyObject *sipDo_String_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = ptr -> String::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_isValid);

	return NULL;
}

static PyObject *sipDo_String_size(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_String)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			String *ptr;

			if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
				return NULL;

			res = ptr -> String::size();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_String,sipName_BALL_size);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_String(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_String)
		return ptr;

	return NULL;
}

static void sipDealloc_String(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (String *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

static PyObject *sipPyInternalRepr_String(sipThisType *sipThis)
{
#line 230 "string.sip"
  const char *s;
  String *ptr;

  if ((ptr = (String *)sipGetCppPtr(sipThis,sipClass_String)) == NULL)
    return NULL;

  if ((s = ptr->c_str()) == NULL)
  {
    Py_INCREF(Py_None);
    return Py_None;
  }

  return PyString_FromString(s);
#line 2033 "../CPP/sipBALLString.cpp"
}

PyObject *sipNew_String(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_String
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
			sipNew = new String();
		}
	}

	if (sipNew == NULL)
	{
		const String *a0;
		PyObject *a0obj;
		Index *a1;
		PyObject *a1obj;
		Size *a2 = (Size *)&string::npos;
		PyObject *a2obj = NULL;

		if (sipParseArgs(sipArgs,"-II|I",sipCanConvertTo_String,&a0obj,sipCanConvertTo_Index,&a1obj,sipCanConvertTo_Size,&a2obj))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);
			int istemp2 = sipConvertTo_Size(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new String(* a0,* a1,* a2);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			if (istemp2)
				delete a2;
		}
	}

	if (sipNew == NULL)
	{
		const char *a0;
		Index *a1 = NULL;
		PyObject *a1obj = NULL;
		Size *a2 = (Size *)&string::npos;
		PyObject *a2obj = NULL;

		if (sipParseArgs(sipArgs,"-s|II",&a0,sipCanConvertTo_Index,&a1obj,sipCanConvertTo_Size,&a2obj))
		{
			int iserr = 0;

			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);
			int istemp2 = sipConvertTo_Size(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new String( a0,* a1,* a2);

			if (istemp1)
				delete a1;

			if (istemp2)
				delete a2;
		}
	}

	if (sipNew == NULL)
	{
		char a0;
		Size *a1 = NULL;
		PyObject *a1obj = NULL;

		if (sipParseArgs(sipArgs,"-c|I",&a0,sipCanConvertTo_Size,&a1obj))
		{
			int iserr = 0;

			int istemp1 = sipConvertTo_Size(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new String( a0,* a1);

			if (istemp1)
				delete a1;
		}
	}

	if (sipNew == NULL)
	{
		short a0;

		if (sipParseArgs(sipArgs,"-h",&a0))
		{
			sipNew = new String( a0);
		}
	}

	if (sipNew == NULL)
	{
		int a0;

		if (sipParseArgs(sipArgs,"-i",&a0))
		{
			sipNew = new String( a0);
		}
	}

	if (sipNew == NULL)
	{
		long a0;

		if (sipParseArgs(sipArgs,"-l",&a0))
		{
			sipNew = new String( a0);
		}
	}

	if (sipNew == NULL)
	{
		float a0;

		if (sipParseArgs(sipArgs,"-f",&a0))
		{
			sipNew = new String( a0);
		}
	}

	if (sipNew == NULL)
	{
		double a0;

		if (sipParseArgs(sipArgs,"-d",&a0))
		{
			sipNew = new String( a0);
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

			sipNew = new String(* a0);

			if (istemp0)
				delete a0;
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_String);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_String,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (String *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_String[] = {
	{sipName_BALL_destroy, sipDo_String_destroy, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_String_set, METH_VARARGS, NULL},
	{sipName_BALL_setCompareMode, sipDo_String_setCompareMode, METH_VARARGS, NULL},
	{sipName_BALL_getCompareMode, sipDo_String_getCompareMode, METH_VARARGS, NULL},
	{sipName_BALL_toBool, sipDo_String_toBool, METH_VARARGS, NULL},
	{sipName_BALL_toChar, sipDo_String_toChar, METH_VARARGS, NULL},
	{sipName_BALL_toShort, sipDo_String_toShort, METH_VARARGS, NULL},
	{sipName_BALL_toInt, sipDo_String_toInt, METH_VARARGS, NULL},
	{sipName_BALL_toLong, sipDo_String_toLong, METH_VARARGS, NULL},
	{sipName_BALL_toFloat, sipDo_String_toFloat, METH_VARARGS, NULL},
	{sipName_BALL_toDouble, sipDo_String_toDouble, METH_VARARGS, NULL},
	{sipName_BALL_toLower, sipDo_String_toLower, METH_VARARGS, NULL},
	{sipName_BALL_toUpper, sipDo_String_toUpper, METH_VARARGS, NULL},
	{sipName_BALL_getSubstring, sipDo_String_getSubstring, METH_VARARGS, NULL},
	{sipName_BALL_Operator__call__, sipDo_String_Operator__call__, METH_VARARGS, NULL},
	{sipName_BALL_before, sipDo_String_before, METH_VARARGS, NULL},
	{sipName_BALL_through, sipDo_String_through, METH_VARARGS, NULL},
	{sipName_BALL_from, sipDo_String_from, METH_VARARGS, NULL},
	{sipName_BALL_after, sipDo_String_after, METH_VARARGS, NULL},
	{sipName_BALL_countFields, sipDo_String_countFields, METH_VARARGS, NULL},
	{sipName_BALL_getField, sipDo_String_getField, METH_VARARGS, NULL},
	{sipName_BALL_trimLeft, sipDo_String_trimLeft, METH_VARARGS, NULL},
	{sipName_BALL_trimRight, sipDo_String_trimRight, METH_VARARGS, NULL},
	{sipName_BALL_trim, sipDo_String_trim, METH_VARARGS, NULL},
	{sipName_BALL_truncate, sipDo_String_truncate, METH_VARARGS, NULL},
	{sipName_BALL_left, sipDo_String_left, METH_VARARGS, NULL},
	{sipName_BALL_right, sipDo_String_right, METH_VARARGS, NULL},
	{sipName_BALL_instr, sipDo_String_instr, METH_VARARGS, NULL},
	{sipName_BALL_Operator__add__, sipDo_String_Operator__add__, METH_VARARGS, NULL},
	{sipName_BALL_swap, sipDo_String_swap, METH_VARARGS, NULL},
	{sipName_BALL_reverse, sipDo_String_reverse, METH_VARARGS, NULL},
	{sipName_BALL_substitute, sipDo_String_substitute, METH_VARARGS, NULL},
	{sipName_BALL_has, sipDo_String_has, METH_VARARGS, NULL},
	{sipName_BALL_hasSubstring, sipDo_String_hasSubstring, METH_VARARGS, NULL},
	{sipName_BALL_hasPrefix, sipDo_String_hasPrefix, METH_VARARGS, NULL},
	{sipName_BALL_hasSuffix, sipDo_String_hasSuffix, METH_VARARGS, NULL},
	{sipName_BALL_isEmpty, sipDo_String_isEmpty, METH_VARARGS, NULL},
	{sipName_BALL_isAlpha, sipDo_String_isAlpha, METH_VARARGS, NULL},
	{sipName_BALL_isAlnum, sipDo_String_isAlnum, METH_VARARGS, NULL},
	{sipName_BALL_isDigit, sipDo_String_isDigit, METH_VARARGS, NULL},
	{sipName_BALL_isSpace, sipDo_String_isSpace, METH_VARARGS, NULL},
	{sipName_BALL_isWhitespace, sipDo_String_isWhitespace, METH_VARARGS, NULL},
	{sipName_BALL_compare, sipDo_String_compare, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_String_isValid, METH_VARARGS, NULL},
	{sipName_BALL_size, sipDo_String_size, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_String(PyObject *sipPy)
{
#line 252 "string.sip"
	// automatic conversion of Py-Strings to Strings
	return (PyString_Check(sipPy) || sipIsSubClassInstance(sipPy,sipClass_String));
#line 2286 "../CPP/sipBALLString.cpp"
}

int sipConvertTo_String(PyObject *sipPy,String **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return false;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_String);
		else
			*sipCppPtr = NULL;

		return false;
	}

#line 256 "string.sip"
  if (PyString_Check(sipPy))
  {
    *sipCppPtr = new String(PyString_AS_STRING(sipPy));

    return 1;
  }

  *sipCppPtr = (String*)sipConvertToCpp(sipPy,sipClass_String,sipIsErr);

  return 0;
#line 2315 "../CPP/sipBALLString.cpp"
}

String *sipForceConvertTo_String(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_String(valobj))
	{
		String *val;

		sipConvertTo_String(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_String);
	*iserrp = 1;

	return NULL;
}
