#include "sipBALLDeclBALL.h"
#include "sipBALLFDPBFastAtomStruct.h"


PyObject *sipClass_FDPB_FastAtomStruct;

static void sipDealloc_FDPB_FastAtomStruct(sipThisType *);

static PyTypeObject sipType_FDPB_FastAtomStruct = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_FastAtomStruct,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_FDPB_FastAtomStruct,
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

extern "C" const void *sipCast_FDPB_FastAtomStruct(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_FDPB_FastAtomStruct)
		return ptr;

	return NULL;
}

static void sipDealloc_FDPB_FastAtomStruct(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (FDPB::FastAtomStruct *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_FDPB_FastAtomStruct(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_FDPB_FastAtomStruct
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
			sipNew = new FDPB::FastAtomStruct();
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
		const FDPB::FastAtomStruct * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_FDPB_FastAtomStruct,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_FDPB_FastAtomStruct(a0obj,(FDPB::FastAtomStruct **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new FDPB::FastAtomStruct(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_FastAtomStruct);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_FDPB_FastAtomStruct,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (FDPB::FastAtomStruct *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_FDPB_FastAtomStruct_index(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Index *val;
	FDPB::FastAtomStruct *ptr;

	if ((ptr = (FDPB::FastAtomStruct *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_FDPB_FastAtomStruct)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = &ptr -> index;

		valobj = sipConvertFrom_Index(val);

		return valobj;
	}

	val = sipForceConvertTo_Index(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_FastAtomStruct,sipName_BALL_index);
		return NULL;
	}

	ptr -> index = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_FDPB_FastAtomStruct_z(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	FDPB::FastAtomStruct *ptr;

	if ((ptr = (FDPB::FastAtomStruct *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_FDPB_FastAtomStruct)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> z;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_FastAtomStruct,sipName_BALL_z);
		return NULL;
	}

	ptr -> z = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_FDPB_FastAtomStruct_y(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	FDPB::FastAtomStruct *ptr;

	if ((ptr = (FDPB::FastAtomStruct *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_FDPB_FastAtomStruct)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> y;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_FastAtomStruct,sipName_BALL_y);
		return NULL;
	}

	ptr -> y = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_FDPB_FastAtomStruct_x(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	FDPB::FastAtomStruct *ptr;

	if ((ptr = (FDPB::FastAtomStruct *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_FDPB_FastAtomStruct)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> x;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_FastAtomStruct,sipName_BALL_x);
		return NULL;
	}

	ptr -> x = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_FDPB_FastAtomStruct_r(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	FDPB::FastAtomStruct *ptr;

	if ((ptr = (FDPB::FastAtomStruct *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_FDPB_FastAtomStruct)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> r;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_FastAtomStruct,sipName_BALL_r);
		return NULL;
	}

	ptr -> r = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_FDPB_FastAtomStruct_q(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	FDPB::FastAtomStruct *ptr;

	if ((ptr = (FDPB::FastAtomStruct *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_FDPB_FastAtomStruct)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> q;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_FastAtomStruct,sipName_BALL_q);
		return NULL;
	}

	ptr -> q = val;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_FDPB_FastAtomStruct[] = {
	{NULL}
};

PyMethodDef sipClassVarTab_FDPB_FastAtomStruct[] = {
	{sipName_BALL_index, sipGetSetVar_FDPB_FastAtomStruct_index, 0, NULL},
	{sipName_BALL_z, sipGetSetVar_FDPB_FastAtomStruct_z, 0, NULL},
	{sipName_BALL_y, sipGetSetVar_FDPB_FastAtomStruct_y, 0, NULL},
	{sipName_BALL_x, sipGetSetVar_FDPB_FastAtomStruct_x, 0, NULL},
	{sipName_BALL_r, sipGetSetVar_FDPB_FastAtomStruct_r, 0, NULL},
	{sipName_BALL_q, sipGetSetVar_FDPB_FastAtomStruct_q, 0, NULL},
	{NULL}
};

PyMethodDef *sipClassVarHierTab_FDPB_FastAtomStruct[] = {
	sipClassVarTab_FDPB_FastAtomStruct,
	NULL
};

int sipCanConvertTo_FDPB_FastAtomStruct(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_FDPB_FastAtomStruct);
}

void sipConvertTo_FDPB_FastAtomStruct(PyObject *sipPy,FDPB::FastAtomStruct **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_FastAtomStruct);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (FDPB::FastAtomStruct *)sipConvertToCpp(sipPy,sipClass_FDPB_FastAtomStruct,sipIsErr);
}

FDPB::FastAtomStruct *sipForceConvertTo_FDPB_FastAtomStruct(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_FDPB_FastAtomStruct(valobj))
	{
		FDPB::FastAtomStruct *val;

		sipConvertTo_FDPB_FastAtomStruct(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_FastAtomStruct);

	*iserrp = 1;

	return NULL;
}
