#include "sipBALLDeclBALL.h"
#include "sipBALLEmbeddable.h"


PyObject *sipClass_Embeddable;

static void sipDealloc_Embeddable(sipThisType *);

static PyTypeObject sipType_Embeddable = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Embeddable,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Embeddable,
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

sipEmbeddable::sipEmbeddable()
   : Embeddable()
{
	sipCommonCtor(sipPyMethods,1);
}

sipEmbeddable::sipEmbeddable(const Embeddable& a0)
   : Embeddable(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipEmbeddable::~sipEmbeddable()
 throw()
{
	sipCommonDtor(sipPyThis);
}

void sipEmbeddable::registerThis()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_registerThis,&relLock))
		sipEmbeddable::sipVH_registerThis(&sipPyMethods[0],sipPyThis,relLock);
	else
		Embeddable::registerThis();
}

// The common handler for all classes that inherit this virtual member
// function.

void sipEmbeddable::sipVH_registerThis(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
{
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(O)",sipThis -> sipSelf);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_Embeddable,sipName_BALL_registerThis);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

extern "C" PyObject *sipDo_Embeddable_unregisterThis(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Embeddable)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Embeddable *ptr;

			if ((ptr = (Embeddable *)sipGetCppPtr(sipThis,sipClass_Embeddable)) == NULL)
				return NULL;

			ptr -> Embeddable::unregisterThis();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Embeddable,sipName_BALL_unregisterThis);

	return NULL;
}

extern "C" PyObject *sipDo_Embeddable_registerThis(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Embeddable)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Embeddable *ptr;

			if ((ptr = (Embeddable *)sipGetCppPtr(sipThis,sipClass_Embeddable)) == NULL)
				return NULL;

			ptr -> Embeddable::registerThis();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Embeddable,sipName_BALL_registerThis);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_Embeddable(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Embeddable)
		return ptr;

	return NULL;
}

static void sipDealloc_Embeddable(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipEmbeddable *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (Embeddable *)sipThis -> u.cppPtr;
			else
				delete (sipEmbeddable *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_Embeddable(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Embeddable
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
			sipNew = new sipEmbeddable();
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
		const Embeddable * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_Embeddable,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Embeddable(a0obj,(Embeddable **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipEmbeddable(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_Embeddable);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Embeddable,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (Embeddable *)sipNew;
			else
				delete (sipEmbeddable *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipEmbeddable *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Embeddable[] = {
	{sipName_BALL_unregisterThis, sipDo_Embeddable_unregisterThis, METH_VARARGS, NULL},
	{sipName_BALL_registerThis, sipDo_Embeddable_registerThis, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_Embeddable(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Embeddable);
}

void sipConvertTo_Embeddable(PyObject *sipPy,Embeddable **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_Embeddable);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Embeddable *)sipConvertToCpp(sipPy,sipClass_Embeddable,sipIsErr);
}

Embeddable *sipForceConvertTo_Embeddable(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Embeddable(valobj))
	{
		Embeddable *val;

		sipConvertTo_Embeddable(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Embeddable);

	*iserrp = 1;

	return NULL;
}
