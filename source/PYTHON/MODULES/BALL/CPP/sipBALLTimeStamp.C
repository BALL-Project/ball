#include "sipBALLDeclBALL.h"
#include "sipBALLTimeStamp.h"



PyObject *sipClass_TimeStamp;

static void sipDealloc_TimeStamp(sipThisType *);
static PyObject * sip__str__TimeStamp(PyObject *a0);

static PyTypeObject sipType_TimeStamp = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_TimeStamp,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_TimeStamp,
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
	sip__str__TimeStamp,
	0,
	0,
	0,
	Py_TPFLAGS_DEFAULT,
	0,
	0,
	0,
};

sipTimeStamp::sipTimeStamp()
   throw()  : TimeStamp()
{
	sipCommonCtor(sipPyMethods,1);
}

sipTimeStamp::sipTimeStamp(const TimeStamp& a0)
    : TimeStamp(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipTimeStamp::~sipTimeStamp()
  throw()
{
	sipCommonDtor(sipPyThis);
}

void sipTimeStamp::stamp(const PreciseTime& a0)
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_stamp,&relLock))
		sipTimeStamp::sipVH_stamp(&sipPyMethods[0],sipPyThis,relLock,a0);
	else
		TimeStamp::stamp(a0);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipTimeStamp::sipVH_stamp(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,const PreciseTime& a0)
{
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_PreciseTime);

	sipArgs = Py_BuildValue("(OO)",sipThis -> sipSelf,a0obj);

	Py_XDECREF(a0obj);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_TimeStamp,sipName_BALL_stamp);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

static PyObject *sipDo_TimeStamp_isNewerThan(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_TimeStamp)) == NULL)
		return NULL;

	{
		const PreciseTime *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_PreciseTime,&a0obj))
		{
			bool res;
			TimeStamp *ptr;

			if ((ptr = (TimeStamp *)sipGetCppPtr(sipThis,sipClass_TimeStamp)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_PreciseTime(a0obj,(PreciseTime **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> TimeStamp::isNewerThan(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const TimeStamp *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_TimeStamp,&a0obj))
		{
			bool res;
			TimeStamp *ptr;

			if ((ptr = (TimeStamp *)sipGetCppPtr(sipThis,sipClass_TimeStamp)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_TimeStamp(a0obj,(TimeStamp **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> TimeStamp::isNewerThan(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_TimeStamp,sipName_BALL_isNewerThan);

	return NULL;
}

static PyObject *sipDo_TimeStamp_isOlderThan(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_TimeStamp)) == NULL)
		return NULL;

	{
		const PreciseTime *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_PreciseTime,&a0obj))
		{
			bool res;
			TimeStamp *ptr;

			if ((ptr = (TimeStamp *)sipGetCppPtr(sipThis,sipClass_TimeStamp)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_PreciseTime(a0obj,(PreciseTime **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> TimeStamp::isOlderThan(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const TimeStamp *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_TimeStamp,&a0obj))
		{
			bool res;
			TimeStamp *ptr;

			if ((ptr = (TimeStamp *)sipGetCppPtr(sipThis,sipClass_TimeStamp)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_TimeStamp(a0obj,(TimeStamp **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> TimeStamp::isOlderThan(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_TimeStamp,sipName_BALL_isOlderThan);

	return NULL;
}

static PyObject *sipDo_TimeStamp_stamp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_TimeStamp)) == NULL)
		return NULL;

	{
		const PreciseTime *a0 = &PreciseTime::ZERO;
		PyObject *a0obj = NULL;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"|I",sipCanConvertTo_PreciseTime,&a0obj))
		{
			TimeStamp *ptr;

			if ((ptr = (TimeStamp *)sipGetCppPtr(sipThis,sipClass_TimeStamp)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_PreciseTime(a0obj,(PreciseTime **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> TimeStamp::stamp(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_TimeStamp,sipName_BALL_stamp);

	return NULL;
}

static PyObject *sipDo_TimeStamp_getTime(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_TimeStamp)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const PreciseTime *res;
			TimeStamp *ptr;

			if ((ptr = (TimeStamp *)sipGetCppPtr(sipThis,sipClass_TimeStamp)) == NULL)
				return NULL;

			res = &ptr -> TimeStamp::getTime();

			return sipMapCppToSelf(res,sipClass_PreciseTime);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_TimeStamp,sipName_BALL_getTime);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_TimeStamp(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_TimeStamp)
		return ptr;

	return NULL;
}

static void sipDealloc_TimeStamp(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipTimeStamp *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (TimeStamp *)sipThis -> u.cppPtr;
			else
				delete (sipTimeStamp *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}
static PyObject * sip__str__TimeStamp(PyObject *a0)
{
#line 68 "timeStamp.sip"
  TimeStamp* ptr;
  if ((ptr = (TimeStamp*)sipGetCppPtr((sipThisType*)a0,sipClass_TimeStamp)) == NULL)
    return NULL;

	long secs = ptr->getTime().getSeconds();
	char buf[128];
	strftime(buf, 127, "\045Y\045m\045d\045H\045M\045S", localtime(&secs));
	String time_str(buf);
	time_str.append(".");
	String usec_str(ptr->getTime().getMicroSeconds() / 1.0e6);
	time_str.append(usec_str.after("."));

  return PyString_FromString(time_str.c_str());
#line 353 "sipBALLTimeStamp.cpp"
}

PyObject *sipNew_TimeStamp(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_TimeStamp
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
			sipNew = new sipTimeStamp();
		}
	}

	if (sipNew == NULL)
	{
		const TimeStamp *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_TimeStamp,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_TimeStamp(a0obj,(TimeStamp **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipTimeStamp(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_TimeStamp);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_TimeStamp,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (TimeStamp *)sipNew;
			else
				delete (sipTimeStamp *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipTimeStamp *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_TimeStamp[] = {
	{sipName_BALL_isNewerThan, sipDo_TimeStamp_isNewerThan, METH_VARARGS, NULL},
	{sipName_BALL_isOlderThan, sipDo_TimeStamp_isOlderThan, METH_VARARGS, NULL},
	{sipName_BALL_stamp, sipDo_TimeStamp_stamp, METH_VARARGS, NULL},
	{sipName_BALL_getTime, sipDo_TimeStamp_getTime, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_TimeStamp(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_TimeStamp);
}

void sipConvertTo_TimeStamp(PyObject *sipPy,TimeStamp **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_TimeStamp);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (TimeStamp *)sipConvertToCpp(sipPy,sipClass_TimeStamp,sipIsErr);
}

TimeStamp *sipForceConvertTo_TimeStamp(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_TimeStamp(valobj))
	{
		TimeStamp *val;

		sipConvertTo_TimeStamp(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_TimeStamp);
	*iserrp = 1;

	return NULL;
}
