#include "sipBALLDeclBALL.h"
#include "sipBALLVersionInfo.h"


PyObject *sipClass_VersionInfo;

static void sipDealloc_VersionInfo(sipThisType *);
extern "C" PyObject * sip__str__VersionInfo(PyObject *a0);

static PyTypeObject sipType_VersionInfo = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_VersionInfo,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_VersionInfo,
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
	sip__str__VersionInfo,
	0,
	0,
	0,
	Py_TPFLAGS_DEFAULT,
	0,
	0,
	0,
};

extern "C" PyObject *sipDo_VersionInfo_getVersion(PyObject *,PyObject *sipArgs)
{
	int sipArgsParsed = 0;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,"-"))
		{
			const char *res;

   try
   {
			res = VersionInfo::getVersion();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_VersionInfo,sipName_BALL_getVersion);

	return NULL;
}

extern "C" PyObject *sipDo_VersionInfo_getMajorRevision(PyObject *,PyObject *sipArgs)
{
	int sipArgsParsed = 0;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,"-"))
		{
			int res;

   try
   {
			res = VersionInfo::getMajorRevision();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_VersionInfo,sipName_BALL_getMajorRevision);

	return NULL;
}

extern "C" PyObject *sipDo_VersionInfo_getMinorRevision(PyObject *,PyObject *sipArgs)
{
	int sipArgsParsed = 0;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,"-"))
		{
			int res;

   try
   {
			res = VersionInfo::getMinorRevision();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_VersionInfo,sipName_BALL_getMinorRevision);

	return NULL;
}

extern "C" PyObject *sipDo_VersionInfo_getType(PyObject *,PyObject *sipArgs)
{
	int sipArgsParsed = 0;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,"-"))
		{
			VersionInfo::Type res;

   try
   {
			res = VersionInfo::getType();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_VersionInfo,sipName_BALL_getType);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_VersionInfo(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_VersionInfo)
		return ptr;

	return NULL;
}

static void sipDealloc_VersionInfo(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (VersionInfo *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}
extern "C" PyObject * sip__str__VersionInfo(PyObject *a0)
{
#line 26 "version.sip"
  return PyString_FromString(VersionInfo::getVersion());  	
#line 192 "sipBALLVersionInfo.cpp"
}

PyObject *sipNew_VersionInfo(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_VersionInfo
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
			sipNew = new VersionInfo();
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
		const VersionInfo * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_VersionInfo,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_VersionInfo(a0obj,(VersionInfo **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new VersionInfo(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_VersionInfo);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_VersionInfo,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (VersionInfo *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_VersionInfo[] = {
	{sipName_BALL_getVersion, sipDo_VersionInfo_getVersion, METH_VARARGS, NULL},
	{sipName_BALL_getMajorRevision, sipDo_VersionInfo_getMajorRevision, METH_VARARGS, NULL},
	{sipName_BALL_getMinorRevision, sipDo_VersionInfo_getMinorRevision, METH_VARARGS, NULL},
	{sipName_BALL_getType, sipDo_VersionInfo_getType, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_VersionInfo(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_VersionInfo);
}

void sipConvertTo_VersionInfo(PyObject *sipPy,VersionInfo **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_VersionInfo);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (VersionInfo *)sipConvertToCpp(sipPy,sipClass_VersionInfo,sipIsErr);
}

VersionInfo *sipForceConvertTo_VersionInfo(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_VersionInfo(valobj))
	{
		VersionInfo *val;

		sipConvertTo_VersionInfo(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_VersionInfo);

	*iserrp = 1;

	return NULL;
}
