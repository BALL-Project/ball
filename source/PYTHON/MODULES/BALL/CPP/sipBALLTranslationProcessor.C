#include "sipBALLDeclBALL.h"
#include "sipBALLTranslationProcessor.h"



PyObject *sipClass_TranslationProcessor;

static void sipDealloc_TranslationProcessor(sipThisType *);

static PyTypeObject sipType_TranslationProcessor = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_TranslationProcessor,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_TranslationProcessor,
	0,
	0,
	0,
	0,
	0,
};

sipTranslationProcessor::sipTranslationProcessor(): TranslationProcessor()
{
	sipCommonCtor(sipPyMethods,1);
}

sipTranslationProcessor::sipTranslationProcessor(const Vector3& a0): TranslationProcessor(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipTranslationProcessor::sipTranslationProcessor(const TranslationProcessor& a0): TranslationProcessor(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipTranslationProcessor::~sipTranslationProcessor()
{
	sipCommonDtor(sipPyThis);
}
Processor::Result sipTranslationProcessor::operator()(Atom& a0)
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_Operator__call__,&relLock) ?
		sipAtomProcessor::sipVH_Operator__call__(&sipPyMethods[0],sipPyThis,relLock,a0) :
		TranslationProcessor::operator()(a0);
}

static PyObject *sipDo_TranslationProcessor_setTranslation(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_TranslationProcessor)) == NULL)
		return NULL;

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			TranslationProcessor *ptr;

			if ((ptr = (TranslationProcessor *)sipGetCppPtr(sipThis,sipClass_TranslationProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> TranslationProcessor::setTranslation(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_TranslationProcessor,sipName_BALL_setTranslation);

	return NULL;
}

static PyObject *sipDo_TranslationProcessor_getTranslation(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_TranslationProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			const Vector3 *res;
			TranslationProcessor *ptr;

			if ((ptr = (TranslationProcessor *)sipGetCppPtr(sipThis,sipClass_TranslationProcessor)) == NULL)
				return NULL;

			res = &ptr -> TranslationProcessor::getTranslation();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_TranslationProcessor,sipName_BALL_getTranslation);

	return NULL;
}

static PyObject *sipDo_TranslationProcessor_Operator__call__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_TranslationProcessor)) == NULL)
		return NULL;

	{
		Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			Processor::Result res;
			TranslationProcessor *ptr;

			if ((ptr = (TranslationProcessor *)sipGetCppPtr(sipThis,sipClass_TranslationProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> TranslationProcessor::operator()(* a0);

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_TranslationProcessor,sipName_BALL_Operator__call__);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_TranslationProcessor(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_TranslationProcessor)
		return ptr;

	if ((res = sipCast_AtomProcessor((AtomProcessor *)(TranslationProcessor *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_TranslationProcessor(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipTranslationProcessor *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (TranslationProcessor *)sipThis -> u.cppPtr;
			else
				delete (sipTranslationProcessor *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_TranslationProcessor(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_TranslationProcessor
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
			sipNew = new sipTranslationProcessor();
		}
	}

	if (sipNew == NULL)
	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_Vector3,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipTranslationProcessor(* a0);
		}
	}

	if (sipNew == NULL)
	{
		const TranslationProcessor *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_TranslationProcessor,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_TranslationProcessor(a0obj,(TranslationProcessor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipTranslationProcessor(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_TranslationProcessor);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_TranslationProcessor,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (TranslationProcessor *)sipNew;
			else
				delete (sipTranslationProcessor *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipTranslationProcessor *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_TranslationProcessor[] = {
	{sipName_BALL_setTranslation, sipDo_TranslationProcessor_setTranslation, METH_VARARGS, NULL},
	{sipName_BALL_getTranslation, sipDo_TranslationProcessor_getTranslation, METH_VARARGS, NULL},
	{sipName_BALL_Operator__call__, sipDo_TranslationProcessor_Operator__call__, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_TranslationProcessor(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_TranslationProcessor);
}

void sipConvertTo_TranslationProcessor(PyObject *sipPy,TranslationProcessor **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_TranslationProcessor);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (TranslationProcessor *)sipConvertToCpp(sipPy,sipClass_TranslationProcessor,sipIsErr);
}

TranslationProcessor *sipForceConvertTo_TranslationProcessor(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_TranslationProcessor(valobj))
	{
		TranslationProcessor *val;

		sipConvertTo_TranslationProcessor(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_TranslationProcessor);
	*iserrp = 1;

	return NULL;
}
