#include "sipBALLDeclBALL.h"
#include "sipBALLVersionInfo.h"



PyObject *sipClass_VersionInfo;

static void sipDealloc_VersionInfo(sipThisType *);

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
};

static PyObject *sipDo_VersionInfo_getVersion(PyObject *,PyObject *sipArgs)
{

	{
		if (sipParseArgs(sipArgs,"-"))
		{
			const char *res;

			res = VersionInfo::getVersion();

			if (res == NULL)
			{
				Py_INCREF(Py_None);
				return Py_None;
			}

			return PyString_FromString(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_VersionInfo,sipName_BALL_getVersion);

	return NULL;
}

static PyObject *sipDo_VersionInfo_getMajorRevision(PyObject *,PyObject *sipArgs)
{

	{
		if (sipParseArgs(sipArgs,"-"))
		{
			int res;

			res = VersionInfo::getMajorRevision();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_VersionInfo,sipName_BALL_getMajorRevision);

	return NULL;
}

static PyObject *sipDo_VersionInfo_getMinorRevision(PyObject *,PyObject *sipArgs)
{

	{
		if (sipParseArgs(sipArgs,"-"))
		{
			int res;

			res = VersionInfo::getMinorRevision();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_VersionInfo,sipName_BALL_getMinorRevision);

	return NULL;
}

static PyObject *sipDo_VersionInfo_getType(PyObject *,PyObject *sipArgs)
{

	{
		if (sipParseArgs(sipArgs,"-"))
		{
			VersionInfo::Type res;

			res = VersionInfo::getType();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_VersionInfo,sipName_BALL_getType);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_VersionInfo(const void *ptr,PyObject *targetClass)
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

PyObject *sipNew_VersionInfo(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_VersionInfo
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
			sipNew = new VersionInfo();
		}
	}

	if (sipNew == NULL)
	{
		const VersionInfo *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_VersionInfo,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_VersionInfo(a0obj,(VersionInfo **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new VersionInfo(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_VersionInfo);
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

void sipConvertTo_VersionInfo(PyObject *sipPy,VersionInfo **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_VersionInfo);
		else
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
