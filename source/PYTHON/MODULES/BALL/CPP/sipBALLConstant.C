#include "sipBALLDeclBALL.h"
#include "sipBALLConstant.h"



PyObject *sipClass_Constant;

static void sipDealloc_Constant(sipThisType *);

static PyTypeObject sipType_Constant = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Constant,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Constant,
	0,
	0,
	0,
	0,
	0,
};

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Constant(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Constant)
		return ptr;

	return NULL;
}

static void sipDealloc_Constant(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (Constant *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_Constant(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Constant
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
			sipNew = new Constant();
	}
	}

	if (sipNew == NULL)
	{
		const Constant *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_Constant,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Constant(a0obj,(Constant **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Constant(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_Constant);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Constant,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (Constant *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Constant[] = {
	{NULL}
};

int sipCanConvertTo_Constant(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Constant);
}

void sipConvertTo_Constant(PyObject *sipPy,Constant **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_Constant);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Constant *)sipConvertToCpp(sipPy,sipClass_Constant,sipIsErr);
}

Constant *sipForceConvertTo_Constant(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Constant(valobj))
	{
		Constant *val;

		sipConvertTo_Constant(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Constant);
	*iserrp = 1;

	return NULL;
}
