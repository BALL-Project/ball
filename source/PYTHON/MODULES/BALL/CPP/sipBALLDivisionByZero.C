#include "sipBALLDeclBALL.h"
#include "sipBALLDivisionByZero.h"


PyObject *sipClass_DivisionByZero;

static void sipDealloc_DivisionByZero(sipThisType *);

static PyTypeObject sipType_DivisionByZero = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_DivisionByZero,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_DivisionByZero,
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

const void *sipCast_DivisionByZero(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_DivisionByZero)
		return ptr;

	if ((res = sipCast_GeneralException((GeneralException *)(DivisionByZero *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_DivisionByZero(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (DivisionByZero *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_DivisionByZero(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_DivisionByZero
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
			sipNew = new DivisionByZero( a0, a1);
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
		const DivisionByZero * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_DivisionByZero,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_DivisionByZero(a0obj,(DivisionByZero **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new DivisionByZero(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_DivisionByZero);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_DivisionByZero,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (DivisionByZero *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_DivisionByZero[] = {
	{NULL}
};

int sipCanConvertTo_DivisionByZero(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_DivisionByZero);
}

void sipConvertTo_DivisionByZero(PyObject *sipPy,DivisionByZero **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_DivisionByZero);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (DivisionByZero *)sipConvertToCpp(sipPy,sipClass_DivisionByZero,sipIsErr);
}

DivisionByZero *sipForceConvertTo_DivisionByZero(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_DivisionByZero(valobj))
	{
		DivisionByZero *val;

		sipConvertTo_DivisionByZero(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_DivisionByZero);

	*iserrp = 1;

	return NULL;
}
