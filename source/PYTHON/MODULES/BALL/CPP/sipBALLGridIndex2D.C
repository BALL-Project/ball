#include "sipBALLDeclBALL.h"
#include "sipBALLGridIndex2D.h"


PyObject *sipClass_GridIndex2D;

static void sipDealloc_GridIndex2D(sipThisType *);

static PyTypeObject sipType_GridIndex2D = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_GridIndex2D,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_GridIndex2D,
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

const void *sipCast_GridIndex2D(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_GridIndex2D)
		return ptr;

	return NULL;
}

static void sipDealloc_GridIndex2D(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (GridIndex2D *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_GridIndex2D(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_GridIndex2D
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
			sipNew = new GridIndex2D();
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
		const GridIndex2D * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_GridIndex2D,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_GridIndex2D(a0obj,(GridIndex2D **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new GridIndex2D(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_GridIndex2D);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_GridIndex2D,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (GridIndex2D *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_GridIndex2D_y(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Position *val;
	GridIndex2D *ptr;

	if ((ptr = (GridIndex2D *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_GridIndex2D)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = &ptr -> y;

		valobj = sipConvertFrom_Position(val);

		return valobj;
	}

	val = sipForceConvertTo_Position(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_GridIndex2D,sipName_BALL_y);
		return NULL;
	}

	ptr -> y = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_GridIndex2D_x(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Position *val;
	GridIndex2D *ptr;

	if ((ptr = (GridIndex2D *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_GridIndex2D)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = &ptr -> x;

		valobj = sipConvertFrom_Position(val);

		return valobj;
	}

	val = sipForceConvertTo_Position(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_GridIndex2D,sipName_BALL_x);
		return NULL;
	}

	ptr -> x = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_GridIndex2D[] = {
	{NULL}
};

PyMethodDef sipClassVarTab_GridIndex2D[] = {
	{sipName_BALL_y, sipGetSetVar_GridIndex2D_y, 0, NULL},
	{sipName_BALL_x, sipGetSetVar_GridIndex2D_x, 0, NULL},
	{NULL}
};

PyMethodDef *sipClassVarHierTab_GridIndex2D[] = {
	sipClassVarTab_GridIndex2D,
	NULL
};

int sipCanConvertTo_GridIndex2D(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_GridIndex2D);
}

void sipConvertTo_GridIndex2D(PyObject *sipPy,GridIndex2D **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_GridIndex2D);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (GridIndex2D *)sipConvertToCpp(sipPy,sipClass_GridIndex2D,sipIsErr);
}

GridIndex2D *sipForceConvertTo_GridIndex2D(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_GridIndex2D(valobj))
	{
		GridIndex2D *val;

		sipConvertTo_GridIndex2D(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_GridIndex2D);

	*iserrp = 1;

	return NULL;
}
