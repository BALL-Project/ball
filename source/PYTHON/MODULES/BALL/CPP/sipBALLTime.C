#include "sipBALLDeclBALL.h"
#include "sipBALLTime.h"



PyObject *sipClass_Time;

static void sipDealloc_Time(sipThisType *);
static PyObject *sipPyInternalRepr_Time(sipThisType *);

static PyTypeObject sipType_Time = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Time,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Time,
	0,
	0,
	0,
	0,
	(reprfunc)sipPyInternalRepr_Time,
};

static PyObject *sipDo_Time_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Time)) == NULL)
		return NULL;

	{
		long a0;
		long a1;

		if (sipParseArgs(sipArgs,"ll",&a0,&a1))
		{
			Time *ptr;

			if ((ptr = (Time *)sipGetCppPtr(sipThis,sipClass_Time)) == NULL)
				return NULL;

			ptr -> Time::set( a0, a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const Time *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Time,&a0obj))
		{
			Time *ptr;

			if ((ptr = (Time *)sipGetCppPtr(sipThis,sipClass_Time)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Time(a0obj,(Time **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Time::set(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Time,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_Time_getSeconds(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Time)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			long res;
			Time *ptr;

			if ((ptr = (Time *)sipGetCppPtr(sipThis,sipClass_Time)) == NULL)
				return NULL;

			res = ptr -> Time::getSeconds();

			return PyInt_FromLong(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Time,sipName_BALL_getSeconds);

	return NULL;
}

static PyObject *sipDo_Time_getMicroSeconds(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Time)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			long res;
			Time *ptr;

			if ((ptr = (Time *)sipGetCppPtr(sipThis,sipClass_Time)) == NULL)
				return NULL;

			res = ptr -> Time::getMicroSeconds();

			return PyInt_FromLong(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Time,sipName_BALL_getMicroSeconds);

	return NULL;
}

static PyObject *sipDo_Time_now(PyObject *,PyObject *sipArgs)
{

	{
		if (sipParseArgs(sipArgs,"-"))
		{
			const Time *res;

			res = &Time::now();

			return sipMapCppToSelf(res,sipClass_Time);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Time,sipName_BALL_now);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Time(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Time)
		return ptr;

	return NULL;
}

static void sipDealloc_Time(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (Time *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

static PyObject *sipPyInternalRepr_Time(sipThisType *sipThis)
{
#line 29 "timeStamp.sip"
  Time* ptr;
  if ((ptr = (Time*)sipGetCppPtr(sipThis,sipClass_Time)) == NULL)
    return NULL;

	char buf[128];
	long secs = ptr->getSeconds();
	strftime(buf, 127, "\045Y\045m\045d\045H\045M\045S", localtime(&secs));
	String time_str(buf);
	time_str.append(".");
	String usec_str(ptr->getMicroSeconds() / 1.0e6);
	time_str.append(usec_str.after("."));

  return PyString_FromString(time_str.c_str());
#line 203 "../CPP/sipBALLTime.cpp"
}

PyObject *sipNew_Time(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Time
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
			sipNew = new Time();
	}
	}

	if (sipNew == NULL)
	{
		const Time *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_Time,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Time(a0obj,(Time **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Time(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_Time);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Time,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (Time *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Time[] = {
	{sipName_BALL_set, sipDo_Time_set, METH_VARARGS, NULL},
	{sipName_BALL_getSeconds, sipDo_Time_getSeconds, METH_VARARGS, NULL},
	{sipName_BALL_getMicroSeconds, sipDo_Time_getMicroSeconds, METH_VARARGS, NULL},
	{sipName_BALL_now, sipDo_Time_now, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_Time(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Time);
}

void sipConvertTo_Time(PyObject *sipPy,Time **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_Time);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Time *)sipConvertToCpp(sipPy,sipClass_Time,sipIsErr);
}

Time *sipForceConvertTo_Time(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Time(valobj))
	{
		Time *val;

		sipConvertTo_Time(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Time);
	*iserrp = 1;

	return NULL;
}
