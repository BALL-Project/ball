#include "sipBALLDeclBALL.h"
#include "sipBALLOptions.h"



PyObject *sipClass_Options;

static void sipDealloc_Options(sipThisType *);
static PyObject * sip__str__Options(PyObject *a0);

static PyTypeObject sipType_Options = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Options,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Options,
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
	sip__str__Options,
	0,
	0,
	0,
	Py_TPFLAGS_DEFAULT,
	0,
	0,
	0,
};

static PyObject *sipDo_Options_isInteger(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			bool res;
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Options::isInteger(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_isInteger);

	return NULL;
}

static PyObject *sipDo_Options_isBool(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			bool res;
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Options::isBool(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_isBool);

	return NULL;
}

static PyObject *sipDo_Options_isReal(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			bool res;
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Options::isReal(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_isReal);

	return NULL;
}

static PyObject *sipDo_Options_isVector(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			bool res;
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Options::isVector(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_isVector);

	return NULL;
}

static PyObject *sipDo_Options_isSet(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			bool res;
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Options::isSet(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_isSet);

	return NULL;
}

static PyObject *sipDo_Options_setName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Options::setName(* a0);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_setName);

	return NULL;
}

static PyObject *sipDo_Options_getName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const String *res;
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			res = &ptr -> Options::getName();

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_getName);

	return NULL;
}

static PyObject *sipDo_Options_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			String *res;
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = new String(ptr -> Options::get(* a0));

			if (istemp0)
				delete a0;

			return sipNewCppToSelf(res,sipClass_String,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_get);

	return NULL;
}

static PyObject *sipDo_Options_getBool(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			bool res;
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Options::getBool(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_getBool);

	return NULL;
}

static PyObject *sipDo_Options_getReal(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			double res;
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Options::getReal(* a0);

			if (istemp0)
				delete a0;

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_getReal);

	return NULL;
}

static PyObject *sipDo_Options_getVector(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			Vector3 *res;
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = new Vector3(ptr -> Options::getVector(* a0));

			if (istemp0)
				delete a0;

			return sipNewCppToSelf(res,sipClass_Vector3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_getVector);

	return NULL;
}

static PyObject *sipDo_Options_getInteger(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			long res;
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Options::getInteger(* a0);

			if (istemp0)
				delete a0;

			return PyInt_FromLong(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_getInteger);

	return NULL;
}

static PyObject *sipDo_Options_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		const String *a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_String,&a0obj,sipCanConvertTo_String,&a1obj))
		{
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Options::set(* a0,* a1);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_Options_setReal(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		double a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"Id",sipCanConvertTo_String,&a0obj,&a1))
		{
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Options::setReal(* a0, a1);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_setReal);

	return NULL;
}

static PyObject *sipDo_Options_setVector(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_String,&a0obj,sipCanConvertTo_Vector3,&a1obj))
		{
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Options::setVector(* a0,* a1);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_setVector);

	return NULL;
}

static PyObject *sipDo_Options_setInteger(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		long a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"Il",sipCanConvertTo_String,&a0obj,&a1))
		{
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Options::setInteger(* a0, a1);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_setInteger);

	return NULL;
}

static PyObject *sipDo_Options_setBool(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		long a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"Il",sipCanConvertTo_String,&a0obj,&a1))
		{
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Options::setBool(* a0, (bool)a1);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_setBool);

	return NULL;
}

static PyObject *sipDo_Options_setDefault(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		const String *a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_String,&a0obj,sipCanConvertTo_String,&a1obj))
		{
			String *res;
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = new String(ptr -> Options::setDefault(* a0,* a1));

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipNewCppToSelf(res,sipClass_String,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_setDefault);

	return NULL;
}

static PyObject *sipDo_Options_setDefaultInteger(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		long a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"Il",sipCanConvertTo_String,&a0obj,&a1))
		{
			long res;
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Options::setDefaultInteger(* a0, a1);

			if (istemp0)
				delete a0;

			return PyInt_FromLong(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_setDefaultInteger);

	return NULL;
}

static PyObject *sipDo_Options_setDefaultReal(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		double a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"Id",sipCanConvertTo_String,&a0obj,&a1))
		{
			double res;
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Options::setDefaultReal(* a0, a1);

			if (istemp0)
				delete a0;

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_setDefaultReal);

	return NULL;
}

static PyObject *sipDo_Options_setDefaultBool(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		long a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"Il",sipCanConvertTo_String,&a0obj,&a1))
		{
			bool res;
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Options::setDefaultBool(* a0, (bool)a1);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_setDefaultBool);

	return NULL;
}

static PyObject *sipDo_Options_readOptionFile(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Options)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			bool res;
			Options *ptr;

			if ((ptr = (Options *)sipGetCppPtr(sipThis,sipClass_Options)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Options::readOptionFile(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Options,sipName_BALL_readOptionFile);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Options(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Options)
		return ptr;

	return NULL;
}

static void sipDealloc_Options(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (Options *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}
static PyObject * sip__str__Options(PyObject *a0)
{
#line 40 "options.sip"
  Options* ptr;
  if ((ptr = (Options*)sipGetCppPtr((sipThisType*)a0,sipClass_Options)) == NULL)
    return NULL;

  return PyString_FromString(String(String("Options ") + ptr->getName()
        + " { " + String(ptr->getSize()) + " entries }").c_str());
#line 1026 "sipBALLOptions.cpp"
}

PyObject *sipNew_Options(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Options
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
			sipNew = new Options();
		}
	}

	if (sipNew == NULL)
	{
		const Options *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_Options,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_Options(a0obj,(Options **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Options(* a0, (bool)a1);
		}
	}

	if (sipNew == NULL)
	{
		const Options *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_Options,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Options(a0obj,(Options **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Options(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_Options);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Options,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (Options *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Options[] = {
	{sipName_BALL_isInteger, sipDo_Options_isInteger, METH_VARARGS, NULL},
	{sipName_BALL_isBool, sipDo_Options_isBool, METH_VARARGS, NULL},
	{sipName_BALL_isReal, sipDo_Options_isReal, METH_VARARGS, NULL},
	{sipName_BALL_isVector, sipDo_Options_isVector, METH_VARARGS, NULL},
	{sipName_BALL_isSet, sipDo_Options_isSet, METH_VARARGS, NULL},
	{sipName_BALL_setName, sipDo_Options_setName, METH_VARARGS, NULL},
	{sipName_BALL_getName, sipDo_Options_getName, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_Options_get, METH_VARARGS, NULL},
	{sipName_BALL_getBool, sipDo_Options_getBool, METH_VARARGS, NULL},
	{sipName_BALL_getReal, sipDo_Options_getReal, METH_VARARGS, NULL},
	{sipName_BALL_getVector, sipDo_Options_getVector, METH_VARARGS, NULL},
	{sipName_BALL_getInteger, sipDo_Options_getInteger, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_Options_set, METH_VARARGS, NULL},
	{sipName_BALL_setReal, sipDo_Options_setReal, METH_VARARGS, NULL},
	{sipName_BALL_setVector, sipDo_Options_setVector, METH_VARARGS, NULL},
	{sipName_BALL_setInteger, sipDo_Options_setInteger, METH_VARARGS, NULL},
	{sipName_BALL_setBool, sipDo_Options_setBool, METH_VARARGS, NULL},
	{sipName_BALL_setDefault, sipDo_Options_setDefault, METH_VARARGS, NULL},
	{sipName_BALL_setDefaultInteger, sipDo_Options_setDefaultInteger, METH_VARARGS, NULL},
	{sipName_BALL_setDefaultReal, sipDo_Options_setDefaultReal, METH_VARARGS, NULL},
	{sipName_BALL_setDefaultBool, sipDo_Options_setDefaultBool, METH_VARARGS, NULL},
	{sipName_BALL_readOptionFile, sipDo_Options_readOptionFile, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_Options(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Options);
}

void sipConvertTo_Options(PyObject *sipPy,Options **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_Options);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Options *)sipConvertToCpp(sipPy,sipClass_Options,sipIsErr);
}

Options *sipForceConvertTo_Options(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Options(valobj))
	{
		Options *val;

		sipConvertTo_Options(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Options);
	*iserrp = 1;

	return NULL;
}
