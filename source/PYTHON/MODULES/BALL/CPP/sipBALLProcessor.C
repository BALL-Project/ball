#include "sipBALLDeclBALL.h"
#include "sipBALLProcessor.h"



PyObject *sipClass_Processor;

static void sipDealloc_Processor(sipThisType *);

static PyTypeObject sipType_Processor = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Processor,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Processor,
	0,
	0,
	0,
	0,
	0,
};

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Processor(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Processor)
		return ptr;

	return NULL;
}

static void sipDealloc_Processor(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (Processor *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_Processor(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Processor
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
			sipNew = new Processor();
	}
	}

	if (sipNew == NULL)
	{
		const Processor *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_Processor,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Processor(a0obj,(Processor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Processor(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_Processor);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Processor,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (Processor *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Processor[] = {
	{NULL}
};

int sipCanConvertTo_Processor(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Processor);
}

void sipConvertTo_Processor(PyObject *sipPy,Processor **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_Processor);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Processor *)sipConvertToCpp(sipPy,sipClass_Processor,sipIsErr);
}

Processor *sipForceConvertTo_Processor(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Processor(valobj))
	{
		Processor *val;

		sipConvertTo_Processor(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Processor);
	*iserrp = 1;

	return NULL;
}
