#include "sipBALLDeclBALL.h"
#include "sipBALLGridIndex.h"



PyObject *sipClass_GridIndex;

static void sipDealloc_GridIndex(sipThisType *);

static PyTypeObject sipType_GridIndex = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_GridIndex,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_GridIndex,
	0,
	0,
	0,
	0,
	0,
};

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_GridIndex(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_GridIndex)
		return ptr;

	return NULL;
}

static void sipDealloc_GridIndex(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (GridIndex *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_GridIndex(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_GridIndex
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
			sipNew = new GridIndex();
		}
	}

	if (sipNew == NULL)
	{
		const GridIndex *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_GridIndex,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_GridIndex(a0obj,(GridIndex **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new GridIndex(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_GridIndex);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_GridIndex,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (GridIndex *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_GridIndex_x(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Position *val;
	GridIndex *ptr;

	if ((ptr = (GridIndex *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_GridIndex)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = &ptr -> x;

		valobj = sipMapCppToSelf(val,sipClass_Position);

		return valobj;
	}

	val = sipForceConvertTo_Position(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_GridIndex,sipName_BALL_x);
		return NULL;
	}

	ptr -> x = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_GridIndex_y(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Position *val;
	GridIndex *ptr;

	if ((ptr = (GridIndex *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_GridIndex)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = &ptr -> y;

		valobj = sipMapCppToSelf(val,sipClass_Position);

		return valobj;
	}

	val = sipForceConvertTo_Position(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_GridIndex,sipName_BALL_y);
		return NULL;
	}

	ptr -> y = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_GridIndex_z(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Position *val;
	GridIndex *ptr;

	if ((ptr = (GridIndex *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_GridIndex)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = &ptr -> z;

		valobj = sipMapCppToSelf(val,sipClass_Position);

		return valobj;
	}

	val = sipForceConvertTo_Position(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_GridIndex,sipName_BALL_z);
		return NULL;
	}

	ptr -> z = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_GridIndex[] = {
	{NULL}
};

PyMethodDef sipClassVarTab_GridIndex[] = {
	{sipName_BALL_x, sipGetSetVar_GridIndex_x, 0, NULL},
	{sipName_BALL_y, sipGetSetVar_GridIndex_y, 0, NULL},
	{sipName_BALL_z, sipGetSetVar_GridIndex_z, 0, NULL},
	{NULL}
};

PyMethodDef *sipClassVarHierTab_GridIndex[] = {
	sipClassVarTab_GridIndex,
	NULL
};

int sipCanConvertTo_GridIndex(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_GridIndex);
}

void sipConvertTo_GridIndex(PyObject *sipPy,GridIndex **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_GridIndex);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (GridIndex *)sipConvertToCpp(sipPy,sipClass_GridIndex,sipIsErr);
}

GridIndex *sipForceConvertTo_GridIndex(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_GridIndex(valobj))
	{
		GridIndex *val;

		sipConvertTo_GridIndex(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_GridIndex);
	*iserrp = 1;

	return NULL;
}
