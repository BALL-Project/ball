#include "sipBALLDeclBALL.h"
#include "sipBALLAtomDict.h"



PyObject *sipClass_AtomDict;

static void sipDealloc_AtomDict(sipThisType *);

static PyTypeObject sipType_AtomDict = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_AtomDict,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_AtomDict,
	0,
	0,
	0,
	0,
	0,
};

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_AtomDict(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_AtomDict)
		return ptr;

	return NULL;
}

static void sipDealloc_AtomDict(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (AtomDict *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_AtomDict(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_AtomDict
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
			sipNew = new AtomDict();
		}
	}

	if (sipNew == NULL)
	{
		const AtomDict *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_AtomDict,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_AtomDict(a0obj,(AtomDict **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new AtomDict(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_AtomDict);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_AtomDict,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (AtomDict *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_AtomDict[] = {
	{NULL}
};

int sipCanConvertTo_AtomDict(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_AtomDict);
}

void sipConvertTo_AtomDict(PyObject *sipPy,AtomDict **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_AtomDict);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (AtomDict *)sipConvertToCpp(sipPy,sipClass_AtomDict,sipIsErr);
}

AtomDict *sipForceConvertTo_AtomDict(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_AtomDict(valobj))
	{
		AtomDict *val;

		sipConvertTo_AtomDict(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_AtomDict);
	*iserrp = 1;

	return NULL;
}
