#include "sipBALLDeclBALL.h"
#include "sipBALLResidueProcessor.h"


PyObject *sipClass_ResidueProcessor;

static void sipDealloc_ResidueProcessor(sipThisType *);

static PyTypeObject sipType_ResidueProcessor = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_ResidueProcessor,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_ResidueProcessor,
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

sipResidueProcessor::sipResidueProcessor()
   : ResidueProcessor()
{
	sipCommonCtor(sipPyMethods,1);
}

sipResidueProcessor::sipResidueProcessor(const ResidueProcessor& a0)
   : ResidueProcessor(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipResidueProcessor::~sipResidueProcessor()

{
	sipCommonDtor(sipPyThis);
}

Processor::Result sipResidueProcessor::operator()(Residue& a0)

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_CallOp,&relLock) ?
		sipResidueProcessor::sipVH_CallOp(&sipPyMethods[0],sipPyThis,relLock,a0) :
		ResidueProcessor::operator()(a0);
}

// The common handler for all classes that inherit this virtual member
// function.

Processor::Result sipResidueProcessor::sipVH_CallOp(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,Residue& a0)
{
	Processor::Result res;
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_Residue);

	sipArgs = Py_BuildValue("(ON)",sipThis -> sipSelf,a0obj);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		res = (Processor::Result)PyInt_AsLong(resobj);

		Py_DECREF(resobj);

		if (PyErr_Occurred() == NULL)
		{
			goto releaseLock;
		}

		sipBadVirtualResultType(sipName_BALL_ResidueProcessor,sipName_BALL_CallOp);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

static PyObject *sipDo_ResidueProcessor_start(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResidueProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			ResidueProcessor *ptr;

			if ((ptr = (ResidueProcessor *)sipGetCppPtr(sipThis,sipClass_ResidueProcessor)) == NULL)
				return NULL;

   try
   {
			res = ptr -> ResidueProcessor::start();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResidueProcessor,sipName_BALL_start);

	return NULL;
}

static PyObject *sipDo_ResidueProcessor_finish(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResidueProcessor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			ResidueProcessor *ptr;

			if ((ptr = (ResidueProcessor *)sipGetCppPtr(sipThis,sipClass_ResidueProcessor)) == NULL)
				return NULL;

   try
   {
			res = ptr -> ResidueProcessor::finish();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResidueProcessor,sipName_BALL_finish);

	return NULL;
}

static PyObject *sipDo_ResidueProcessor_CallOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResidueProcessor)) == NULL)
		return NULL;

	{
		Residue * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Residue,&a0obj))
		{
			Processor::Result res;
			ResidueProcessor *ptr;

			if ((ptr = (ResidueProcessor *)sipGetCppPtr(sipThis,sipClass_ResidueProcessor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Residue(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> ResidueProcessor::operator()(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ResidueProcessor,sipName_BALL_CallOp);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_ResidueProcessor(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_ResidueProcessor)
		return ptr;

	return NULL;
}

static void sipDealloc_ResidueProcessor(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipResidueProcessor *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (ResidueProcessor *)sipThis -> u.cppPtr;
			else
				delete (sipResidueProcessor *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_ResidueProcessor(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_ResidueProcessor
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
			sipNew = new sipResidueProcessor();
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
		const ResidueProcessor * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_ResidueProcessor,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_ResidueProcessor(a0obj,(ResidueProcessor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipResidueProcessor(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_ResidueProcessor);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_ResidueProcessor,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (ResidueProcessor *)sipNew;
			else
				delete (sipResidueProcessor *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipResidueProcessor *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_ResidueProcessor[] = {
	{sipName_BALL_start, sipDo_ResidueProcessor_start, METH_VARARGS, NULL},
	{sipName_BALL_finish, sipDo_ResidueProcessor_finish, METH_VARARGS, NULL},
	{sipName_BALL_CallOp, sipDo_ResidueProcessor_CallOp, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_ResidueProcessor(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_ResidueProcessor);
}

void sipConvertTo_ResidueProcessor(PyObject *sipPy,ResidueProcessor **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_ResidueProcessor);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (ResidueProcessor *)sipConvertToCpp(sipPy,sipClass_ResidueProcessor,sipIsErr);
}

ResidueProcessor *sipForceConvertTo_ResidueProcessor(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_ResidueProcessor(valobj))
	{
		ResidueProcessor *val;

		sipConvertTo_ResidueProcessor(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_ResidueProcessor);

	*iserrp = 1;

	return NULL;
}
