#include "sipBALLDeclBALL.h"
#include "sipBALLIndexUnderflow.h"


PyObject *sipClass_IndexUnderflow;

static void sipDealloc_IndexUnderflow(sipThisType *);

static PyTypeObject sipType_IndexUnderflow = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_IndexUnderflow,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_IndexUnderflow,
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

const void *sipCast_IndexUnderflow(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_IndexUnderflow)
		return ptr;

	if ((res = sipCast_GeneralException((GeneralException *)(IndexUnderflow *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_IndexUnderflow(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (IndexUnderflow *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_IndexUnderflow(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_IndexUnderflow
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
		Index * a2 = NULL;
		PyObject *a2obj = NULL;
		int a3 = 0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-si|Ii",&a0,&a1,sipCanConvertTo_Index,&a2obj,&a3))
		{
			int iserr = 0;

			int istemp2 = sipConvertTo_Index(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new IndexUnderflow( a0, a1,* a2, a3);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp2)
				delete a2;
		}
	}

	if (sipNew == NULL)
	{
		const IndexUnderflow * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_IndexUnderflow,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_IndexUnderflow(a0obj,(IndexUnderflow **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new IndexUnderflow(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_IndexUnderflow);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_IndexUnderflow,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (IndexUnderflow *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_IndexUnderflow[] = {
	{NULL}
};

int sipCanConvertTo_IndexUnderflow(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_IndexUnderflow);
}

void sipConvertTo_IndexUnderflow(PyObject *sipPy,IndexUnderflow **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_IndexUnderflow);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (IndexUnderflow *)sipConvertToCpp(sipPy,sipClass_IndexUnderflow,sipIsErr);
}

IndexUnderflow *sipForceConvertTo_IndexUnderflow(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_IndexUnderflow(valobj))
	{
		IndexUnderflow *val;

		sipConvertTo_IndexUnderflow(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_IndexUnderflow);

	*iserrp = 1;

	return NULL;
}
