#include "sipBALLDeclBALL.h"
#include "sipBALLRegularData3DGridIndex.h"


PyObject *sipClass_RegularData3D_GridIndex;

static void sipDealloc_RegularData3D_GridIndex(sipThisType *);

static PyTypeObject sipType_RegularData3D_GridIndex = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_GridIndex,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_RegularData3D_GridIndex,
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

extern "C" const void *sipCast_RegularData3D_GridIndex(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_RegularData3D_GridIndex)
		return ptr;

	return NULL;
}

static void sipDealloc_RegularData3D_GridIndex(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (RegularData3D::GridIndex *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_RegularData3D_GridIndex(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_RegularData3D_GridIndex
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
			sipNew = new RegularData3D::GridIndex();
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
		const RegularData3D::GridIndex * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_RegularData3D_GridIndex,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_RegularData3D_GridIndex(a0obj,(RegularData3D::GridIndex **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new RegularData3D::GridIndex(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_GridIndex);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_RegularData3D_GridIndex,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (RegularData3D::GridIndex *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_RegularData3D_GridIndex_z(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Position *val;
	RegularData3D::GridIndex *ptr;

	if ((ptr = (RegularData3D::GridIndex *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_RegularData3D_GridIndex)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = &ptr -> z;

		valobj = sipConvertFrom_Position(val);

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

extern "C" PyObject *sipGetSetVar_RegularData3D_GridIndex_y(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Position *val;
	RegularData3D::GridIndex *ptr;

	if ((ptr = (RegularData3D::GridIndex *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_RegularData3D_GridIndex)) == NULL)
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
		sipBadSetType(sipName_BALL_GridIndex,sipName_BALL_y);
		return NULL;
	}

	ptr -> y = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_RegularData3D_GridIndex_x(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Position *val;
	RegularData3D::GridIndex *ptr;

	if ((ptr = (RegularData3D::GridIndex *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_RegularData3D_GridIndex)) == NULL)
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
		sipBadSetType(sipName_BALL_GridIndex,sipName_BALL_x);
		return NULL;
	}

	ptr -> x = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_RegularData3D_GridIndex[] = {
	{NULL}
};

PyMethodDef sipClassVarTab_RegularData3D_GridIndex[] = {
	{sipName_BALL_z, sipGetSetVar_RegularData3D_GridIndex_z, 0, NULL},
	{sipName_BALL_y, sipGetSetVar_RegularData3D_GridIndex_y, 0, NULL},
	{sipName_BALL_x, sipGetSetVar_RegularData3D_GridIndex_x, 0, NULL},
	{NULL}
};

PyMethodDef *sipClassVarHierTab_RegularData3D_GridIndex[] = {
	sipClassVarTab_RegularData3D_GridIndex,
	NULL
};

int sipCanConvertTo_RegularData3D_GridIndex(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_RegularData3D_GridIndex);
}

void sipConvertTo_RegularData3D_GridIndex(PyObject *sipPy,RegularData3D::GridIndex **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_GridIndex);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (RegularData3D::GridIndex *)sipConvertToCpp(sipPy,sipClass_RegularData3D_GridIndex,sipIsErr);
}

RegularData3D::GridIndex *sipForceConvertTo_RegularData3D_GridIndex(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_RegularData3D_GridIndex(valobj))
	{
		RegularData3D::GridIndex *val;

		sipConvertTo_RegularData3D_GridIndex(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_GridIndex);

	*iserrp = 1;

	return NULL;
}
