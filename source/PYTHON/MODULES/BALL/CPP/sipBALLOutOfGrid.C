#include "sipBALLDeclBALL.h"
#include "sipBALLOutOfGrid.h"


PyObject *sipClass_OutOfGrid;

static void sipDealloc_OutOfGrid(sipThisType *);

static PyTypeObject sipType_OutOfGrid = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_OutOfGrid,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_OutOfGrid,
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

extern "C" const void *sipCast_OutOfGrid(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_OutOfGrid)
		return ptr;

	if ((res = sipCast_GeneralException((GeneralException *)(OutOfGrid *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_OutOfGrid(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (OutOfGrid *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_OutOfGrid(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_OutOfGrid
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
			sipNew = new OutOfGrid( a0, a1);
		}
	}

	if (sipNew == NULL)
	{
		const OutOfGrid * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_OutOfGrid,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_OutOfGrid(a0obj,(OutOfGrid **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new OutOfGrid(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_OutOfGrid);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_OutOfGrid,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (OutOfGrid *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_OutOfGrid[] = {
	{NULL}
};

int sipCanConvertTo_OutOfGrid(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_OutOfGrid);
}

void sipConvertTo_OutOfGrid(PyObject *sipPy,OutOfGrid **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_OutOfGrid);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (OutOfGrid *)sipConvertToCpp(sipPy,sipClass_OutOfGrid,sipIsErr);
}

OutOfGrid *sipForceConvertTo_OutOfGrid(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_OutOfGrid(valobj))
	{
		OutOfGrid *val;

		sipConvertTo_OutOfGrid(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_OutOfGrid);

	*iserrp = 1;

	return NULL;
}
