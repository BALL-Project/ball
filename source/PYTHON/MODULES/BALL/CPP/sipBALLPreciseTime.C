#include "sipBALLDeclBALL.h"
#include "sipBALLPreciseTime.h"



PyObject *sipClass_PreciseTime;

static void sipDealloc_PreciseTime(sipThisType *);
static PyObject *sipPyInternalRepr_PreciseTime(sipThisType *);

static PyTypeObject sipType_PreciseTime = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_PreciseTime,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_PreciseTime,
	0,
	0,
	0,
	0,
	(reprfunc)sipPyInternalRepr_PreciseTime,
};

static PyObject *sipDo_PreciseTime_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PreciseTime)) == NULL)
		return NULL;

	{
		long a0;
		long a1;

		if (sipParseArgs(sipArgs,"ll",&a0,&a1))
		{
			PreciseTime *ptr;

			if ((ptr = (PreciseTime *)sipGetCppPtr(sipThis,sipClass_PreciseTime)) == NULL)
				return NULL;

			ptr -> PreciseTime::set( a0, a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const PreciseTime *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_PreciseTime,&a0obj))
		{
			PreciseTime *ptr;

			if ((ptr = (PreciseTime *)sipGetCppPtr(sipThis,sipClass_PreciseTime)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_PreciseTime(a0obj,(PreciseTime **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> PreciseTime::set(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PreciseTime,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_PreciseTime_getSeconds(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PreciseTime)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			long res;
			PreciseTime *ptr;

			if ((ptr = (PreciseTime *)sipGetCppPtr(sipThis,sipClass_PreciseTime)) == NULL)
				return NULL;

			res = ptr -> PreciseTime::getSeconds();

			return PyInt_FromLong(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PreciseTime,sipName_BALL_getSeconds);

	return NULL;
}

static PyObject *sipDo_PreciseTime_getMicroSeconds(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PreciseTime)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			long res;
			PreciseTime *ptr;

			if ((ptr = (PreciseTime *)sipGetCppPtr(sipThis,sipClass_PreciseTime)) == NULL)
				return NULL;

			res = ptr -> PreciseTime::getMicroSeconds();

			return PyInt_FromLong(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PreciseTime,sipName_BALL_getMicroSeconds);

	return NULL;
}

static PyObject *sipDo_PreciseTime_now(PyObject *,PyObject *sipArgs)
{

	{
		if (sipParseArgs(sipArgs,"-"))
		{
			const PreciseTime *res;

			res = &PreciseTime::now();

			return sipMapCppToSelf(res,sipClass_PreciseTime);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PreciseTime,sipName_BALL_now);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_PreciseTime(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_PreciseTime)
		return ptr;

	return NULL;
}

static void sipDealloc_PreciseTime(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (PreciseTime *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

static PyObject *sipPyInternalRepr_PreciseTime(sipThisType *sipThis)
{
#line 29 "timeStamp.sip"
  PreciseTime* ptr;
  if ((ptr = (PreciseTime*)sipGetCppPtr(sipThis,sipClass_PreciseTime)) == NULL)
    return NULL;

	char buf[128];
	long secs = ptr->getSeconds();
	strftime(buf, 127, "\045Y\045m\045d\045H\045M\045S", localtime(&secs));
	String time_str(buf);
	time_str.append(".");
	String usec_str(ptr->getMicroSeconds() / 1.0e6);
	time_str.append(usec_str.after("."));

  return PyString_FromString(time_str.c_str());
#line 203 "../CPP/sipBALLPreciseTime.cpp"
}

PyObject *sipNew_PreciseTime(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_PreciseTime
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
			sipNew = new PreciseTime();
	}
	}

	if (sipNew == NULL)
	{
		const PreciseTime *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_PreciseTime,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_PreciseTime(a0obj,(PreciseTime **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new PreciseTime(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_PreciseTime);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_PreciseTime,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (PreciseTime *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_PreciseTime[] = {
	{sipName_BALL_set, sipDo_PreciseTime_set, METH_VARARGS, NULL},
	{sipName_BALL_getSeconds, sipDo_PreciseTime_getSeconds, METH_VARARGS, NULL},
	{sipName_BALL_getMicroSeconds, sipDo_PreciseTime_getMicroSeconds, METH_VARARGS, NULL},
	{sipName_BALL_now, sipDo_PreciseTime_now, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_PreciseTime(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_PreciseTime);
}

void sipConvertTo_PreciseTime(PyObject *sipPy,PreciseTime **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_PreciseTime);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (PreciseTime *)sipConvertToCpp(sipPy,sipClass_PreciseTime,sipIsErr);
}

PreciseTime *sipForceConvertTo_PreciseTime(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_PreciseTime(valobj))
	{
		PreciseTime *val;

		sipConvertTo_PreciseTime(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_PreciseTime);
	*iserrp = 1;

	return NULL;
}
