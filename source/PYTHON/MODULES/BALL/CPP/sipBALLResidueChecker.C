#include "sipBALLDeclBALL.h"
#include "sipBALLResidueChecker.h"


PyObject *sipClass_ResidueChecker;

static void sipDealloc_ResidueChecker(sipThisType *);

static PyTypeObject sipType_ResidueChecker = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_ResidueChecker,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_ResidueChecker,
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

sipResidueChecker::sipResidueChecker()
   : ResidueChecker()
{
	sipCommonCtor(sipPyMethods,1);
}

sipResidueChecker::sipResidueChecker(FragmentDB& a0)
   : ResidueChecker(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipResidueChecker::sipResidueChecker(const ResidueChecker& a0,bool a1)
   : ResidueChecker(a0,a1)
{
	sipCommonCtor(sipPyMethods,1);
}

sipResidueChecker::sipResidueChecker(const ResidueChecker& a0)
   : ResidueChecker(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipResidueChecker::~sipResidueChecker()

{
	sipCommonDtor(sipPyThis);
}

Processor::Result sipResidueChecker::operator()(Residue& a0)

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_CallOp,&relLock) ?
		sipResidueProcessor::sipVH_CallOp(&sipPyMethods[0],sipPyThis,relLock,a0) :
		ResidueChecker::operator()(a0);
}

static PyObject *sipDo_ResidueChecker_getStatus(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResidueChecker)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			ResidueChecker *ptr;

			if ((ptr = (ResidueChecker *)sipGetCppPtr(sipThis,sipClass_ResidueChecker)) == NULL)
				return NULL;

   try
   {
			res = ptr -> ResidueChecker::getStatus();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ResidueChecker,sipName_BALL_getStatus);

	return NULL;
}

static PyObject *sipDo_ResidueChecker_CallOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResidueChecker)) == NULL)
		return NULL;

	{
		Residue * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Residue,&a0obj))
		{
			Processor::Result res;
			ResidueChecker *ptr;

			if ((ptr = (ResidueChecker *)sipGetCppPtr(sipThis,sipClass_ResidueChecker)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Residue(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> ResidueChecker::operator()(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ResidueChecker,sipName_BALL_CallOp);

	return NULL;
}

static PyObject *sipDo_ResidueChecker_start(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResidueChecker)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			ResidueChecker *ptr;

			if ((ptr = (ResidueChecker *)sipGetCppPtr(sipThis,sipClass_ResidueChecker)) == NULL)
				return NULL;

   try
   {
			res = ptr -> ResidueChecker::start();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ResidueChecker,sipName_BALL_start);

	return NULL;
}

static PyObject *sipDo_ResidueChecker_finish(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResidueChecker)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			ResidueChecker *ptr;

			if ((ptr = (ResidueChecker *)sipGetCppPtr(sipThis,sipClass_ResidueChecker)) == NULL)
				return NULL;

   try
   {
			res = ptr -> ResidueChecker::finish();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ResidueChecker,sipName_BALL_finish);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_ResidueChecker(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_ResidueChecker)
		return ptr;

	if ((res = sipCast_ResidueProcessor((ResidueProcessor *)(ResidueChecker *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_ResidueChecker(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipResidueChecker *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (ResidueChecker *)sipThis -> u.cppPtr;
			else
				delete (sipResidueChecker *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_ResidueChecker(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_ResidueChecker
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
			sipNew = new sipResidueChecker();
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
		FragmentDB * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_FragmentDB,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_FragmentDB(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipResidueChecker(* a0);
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
		const ResidueChecker * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_ResidueChecker,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_ResidueChecker(a0obj,(ResidueChecker **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipResidueChecker(* a0, (bool)a1);
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
		const ResidueChecker * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_ResidueChecker,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_ResidueChecker(a0obj,(ResidueChecker **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipResidueChecker(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_ResidueChecker);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_ResidueChecker,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (ResidueChecker *)sipNew;
			else
				delete (sipResidueChecker *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipResidueChecker *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_ResidueChecker[] = {
	{sipName_BALL_getStatus, sipDo_ResidueChecker_getStatus, METH_VARARGS, NULL},
	{sipName_BALL_CallOp, sipDo_ResidueChecker_CallOp, METH_VARARGS, NULL},
	{sipName_BALL_start, sipDo_ResidueChecker_start, METH_VARARGS, NULL},
	{sipName_BALL_finish, sipDo_ResidueChecker_finish, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_ResidueChecker(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_ResidueChecker);
}

void sipConvertTo_ResidueChecker(PyObject *sipPy,ResidueChecker **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_ResidueChecker);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (ResidueChecker *)sipConvertToCpp(sipPy,sipClass_ResidueChecker,sipIsErr);
}

ResidueChecker *sipForceConvertTo_ResidueChecker(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_ResidueChecker(valobj))
	{
		ResidueChecker *val;

		sipConvertTo_ResidueChecker(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_ResidueChecker);

	*iserrp = 1;

	return NULL;
}
