#include "sipBALLDeclBALL.h"
#include "sipBALLIllegalKey.h"


PyObject *sipClass_IllegalKey;

static void sipDealloc_IllegalKey(sipThisType *);

static PyTypeObject sipType_IllegalKey = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_IllegalKey,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_IllegalKey,
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

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_IllegalKey(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_IllegalKey)
		return ptr;

	if ((res = sipCast_GeneralException((GeneralException *)(IllegalKey *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_IllegalKey(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (IllegalKey *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_IllegalKey(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_IllegalKey
	};

	sipThisType *sipThis = NULL;
	const void *sipNew = NULL;
	int sipFlags = SIP_PY_OWNED;
	int sipArgsParsed = 0;

	// See if there is something pending.

	sipNew = sipGetPending(&sipFlags);

	if (sipNew == NULL)
	{
		const char * a0;
		int a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-si",&a0,&a1))
		{
   try
   {
			sipNew = new IllegalKey( a0, a1);
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
		const IllegalKey * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_IllegalKey,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_IllegalKey(a0obj,(IllegalKey **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new IllegalKey(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_IllegalKey);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_IllegalKey,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (IllegalKey *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_IllegalKey[] = {
	{NULL}
};

int sipCanConvertTo_IllegalKey(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_IllegalKey);
}

void sipConvertTo_IllegalKey(PyObject *sipPy,IllegalKey **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_IllegalKey);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (IllegalKey *)sipConvertToCpp(sipPy,sipClass_IllegalKey,sipIsErr);
}

IllegalKey *sipForceConvertTo_IllegalKey(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_IllegalKey(valobj))
	{
		IllegalKey *val;

		sipConvertTo_IllegalKey(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_IllegalKey);

	*iserrp = 1;

	return NULL;
}
