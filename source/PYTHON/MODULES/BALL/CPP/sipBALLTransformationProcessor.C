#include "sipBALLDeclBALL.h"
#include "sipBALLTransformationProcessor.h"


PyObject *sipClass_TransformationProcessor;

static void sipDealloc_TransformationProcessor(sipThisType *);

static PyTypeObject sipType_TransformationProcessor = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_TransformationProcessor,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_TransformationProcessor,
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
	0,
	0,
	0,
	0,
	Py_TPFLAGS_DEFAULT,
	0,
	0,
	0,
};

sipTransformationProcessor::sipTransformationProcessor()
   : TransformationProcessor()
{
	sipCommonCtor(sipPyMethods,1);
}

sipTransformationProcessor::sipTransformationProcessor(const Matrix4x4& a0)
   : TransformationProcessor(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipTransformationProcessor::sipTransformationProcessor(const TransformationProcessor& a0)
   : TransformationProcessor(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipTransformationProcessor::~sipTransformationProcessor()

{
	sipCommonDtor(sipPyThis);
}

Processor::Result sipTransformationProcessor::operator()(Atom& a0)

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_CallOp,&relLock) ?
		sipAtomProcessor::sipVH_CallOp(&sipPyMethods[0],sipPyThis,relLock,a0) :
		TransformationProcessor::operator()(a0);
}

static PyObject *sipDo_TransformationProcessor_setTransformation(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_TransformationProcessor)) == NULL)
		return NULL;

	{
		const Matrix4x4 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Matrix4x4,&a0obj))
		{
			TransformationProcessor *ptr;

			if ((ptr = (TransformationProcessor *)sipGetCppPtr(sipThis,sipClass_TransformationProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Matrix4x4(a0obj,(Matrix4x4 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> TransformationProcessor::setTransformation(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_TransformationProcessor,sipName_BALL_setTransformation);

	return NULL;
}

static PyObject *sipDo_TransformationProcessor_getTransformation(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_TransformationProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const Matrix4x4 *res;
			TransformationProcessor *ptr;

			if ((ptr = (TransformationProcessor *)sipGetCppPtr(sipThis,sipClass_TransformationProcessor)) == NULL)
				return NULL;

   try
   {
			res = &ptr -> TransformationProcessor::getTransformation();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Matrix4x4);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_TransformationProcessor,sipName_BALL_getTransformation);

	return NULL;
}

static PyObject *sipDo_TransformationProcessor_CallOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_TransformationProcessor)) == NULL)
		return NULL;

	{
		Atom * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			Processor::Result res;
			TransformationProcessor *ptr;

			if ((ptr = (TransformationProcessor *)sipGetCppPtr(sipThis,sipClass_TransformationProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> TransformationProcessor::operator()(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_TransformationProcessor,sipName_BALL_CallOp);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_TransformationProcessor(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_TransformationProcessor)
		return ptr;

	if ((res = sipCast_AtomProcessor((AtomProcessor *)(TransformationProcessor *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_TransformationProcessor(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipTransformationProcessor *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (TransformationProcessor *)sipThis -> u.cppPtr;
			else
				delete (sipTransformationProcessor *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_TransformationProcessor(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_TransformationProcessor
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
			sipNew = new sipTransformationProcessor();
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
		const Matrix4x4 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_Matrix4x4,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Matrix4x4(a0obj,(Matrix4x4 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipTransformationProcessor(* a0);
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
		const TransformationProcessor * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_TransformationProcessor,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_TransformationProcessor(a0obj,(TransformationProcessor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipTransformationProcessor(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_TransformationProcessor);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_TransformationProcessor,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (TransformationProcessor *)sipNew;
			else
				delete (sipTransformationProcessor *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipTransformationProcessor *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_TransformationProcessor[] = {
	{sipName_BALL_setTransformation, sipDo_TransformationProcessor_setTransformation, METH_VARARGS, NULL},
	{sipName_BALL_getTransformation, sipDo_TransformationProcessor_getTransformation, METH_VARARGS, NULL},
	{sipName_BALL_CallOp, sipDo_TransformationProcessor_CallOp, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_TransformationProcessor(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_TransformationProcessor);
}

void sipConvertTo_TransformationProcessor(PyObject *sipPy,TransformationProcessor **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_TransformationProcessor);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (TransformationProcessor *)sipConvertToCpp(sipPy,sipClass_TransformationProcessor,sipIsErr);
}

TransformationProcessor *sipForceConvertTo_TransformationProcessor(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_TransformationProcessor(valobj))
	{
		TransformationProcessor *val;

		sipConvertTo_TransformationProcessor(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_TransformationProcessor);

	*iserrp = 1;

	return NULL;
}
