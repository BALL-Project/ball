#include "sipBALLDeclBALL.h"
#include "sipBALLGridIndex3D.h"



PyObject *sipClass_GridIndex3D;

static void sipDealloc_GridIndex3D(sipThisType *);

static PyTypeObject sipType_GridIndex3D = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_GridIndex3D,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_GridIndex3D,
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

const void *sipCast_GridIndex3D(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_GridIndex3D)
		return ptr;

	return NULL;
}

static void sipDealloc_GridIndex3D(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (GridIndex3D *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_GridIndex3D(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_GridIndex3D
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
			sipNew = new GridIndex3D();
		}
	}

	if (sipNew == NULL)
	{
		const GridIndex3D *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_GridIndex3D,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_GridIndex3D(a0obj,(GridIndex3D **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new GridIndex3D(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_GridIndex3D);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_GridIndex3D,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (GridIndex3D *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_GridIndex3D_z(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Position *val;
	GridIndex3D *ptr;

	if ((ptr = (GridIndex3D *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_GridIndex3D)) == NULL)
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
		sipBadSetType(sipName_BALL_GridIndex3D,sipName_BALL_z);
		return NULL;
	}

	ptr -> z = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_GridIndex3D_y(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Position *val;
	GridIndex3D *ptr;

	if ((ptr = (GridIndex3D *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_GridIndex3D)) == NULL)
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
		sipBadSetType(sipName_BALL_GridIndex3D,sipName_BALL_y);
		return NULL;
	}

	ptr -> y = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_GridIndex3D_x(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Position *val;
	GridIndex3D *ptr;

	if ((ptr = (GridIndex3D *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_GridIndex3D)) == NULL)
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
		sipBadSetType(sipName_BALL_GridIndex3D,sipName_BALL_x);
		return NULL;
	}

	ptr -> x = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_GridIndex3D[] = {
	{NULL}
};

PyMethodDef sipClassVarTab_GridIndex3D[] = {
	{sipName_BALL_z, sipGetSetVar_GridIndex3D_z, 0, NULL},
	{sipName_BALL_y, sipGetSetVar_GridIndex3D_y, 0, NULL},
	{sipName_BALL_x, sipGetSetVar_GridIndex3D_x, 0, NULL},
	{NULL}
};

PyMethodDef *sipClassVarHierTab_GridIndex3D[] = {
	sipClassVarTab_GridIndex3D,
	NULL
};

int sipCanConvertTo_GridIndex3D(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_GridIndex3D);
}

void sipConvertTo_GridIndex3D(PyObject *sipPy,GridIndex3D **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_GridIndex3D);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (GridIndex3D *)sipConvertToCpp(sipPy,sipClass_GridIndex3D,sipIsErr);
}

GridIndex3D *sipForceConvertTo_GridIndex3D(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_GridIndex3D(valobj))
	{
		GridIndex3D *val;

		sipConvertTo_GridIndex3D(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_GridIndex3D);
	*iserrp = 1;

	return NULL;
}
