#include "sipBALLDeclBALL.h"
#include "sipBALLDCDHeader.h"


PyObject *sipClass_DCDHeader;

static void sipDealloc_DCDHeader(sipThisType *);

static PyTypeObject sipType_DCDHeader = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_DCDHeader,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_DCDHeader,
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

const void *sipCast_DCDHeader(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_DCDHeader)
		return ptr;

	return NULL;
}

static void sipDealloc_DCDHeader(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (DCDHeader *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_DCDHeader(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_DCDHeader
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
			sipNew = new DCDHeader();
		}
	}

	if (sipNew == NULL)
	{
		const DCDHeader * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_DCDHeader,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_DCDHeader(a0obj,(DCDHeader **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new DCDHeader(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_DCDHeader);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_DCDHeader,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (DCDHeader *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_DCDHeader_end_atomnumber_block(PyObject *sipThisObj,PyObject *valobj)
{
	int val;
	DCDHeader *ptr;

	if ((ptr = (DCDHeader *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_DCDHeader)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> end_atomnumber_block;

		valobj = PyInt_FromLong((long)val);

		return valobj;
	}

	val = (int)PyInt_AsLong(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_DCDHeader,sipName_BALL_end_atomnumber_block);
		return NULL;
	}

	ptr -> end_atomnumber_block = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_DCDHeader_number_of_atoms(PyObject *sipThisObj,PyObject *valobj)
{
	int val;
	DCDHeader *ptr;

	if ((ptr = (DCDHeader *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_DCDHeader)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> number_of_atoms;

		valobj = PyInt_FromLong((long)val);

		return valobj;
	}

	val = (int)PyInt_AsLong(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_DCDHeader,sipName_BALL_number_of_atoms);
		return NULL;
	}

	ptr -> number_of_atoms = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_DCDHeader_start_atomnumber_block(PyObject *sipThisObj,PyObject *valobj)
{
	int val;
	DCDHeader *ptr;

	if ((ptr = (DCDHeader *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_DCDHeader)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> start_atomnumber_block;

		valobj = PyInt_FromLong((long)val);

		return valobj;
	}

	val = (int)PyInt_AsLong(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_DCDHeader,sipName_BALL_start_atomnumber_block);
		return NULL;
	}

	ptr -> start_atomnumber_block = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_DCDHeader_end_title_block(PyObject *sipThisObj,PyObject *valobj)
{
	int val;
	DCDHeader *ptr;

	if ((ptr = (DCDHeader *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_DCDHeader)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> end_title_block;

		valobj = PyInt_FromLong((long)val);

		return valobj;
	}

	val = (int)PyInt_AsLong(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_DCDHeader,sipName_BALL_end_title_block);
		return NULL;
	}

	ptr -> end_title_block = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_DCDHeader_number_of_comments(PyObject *sipThisObj,PyObject *valobj)
{
	int val;
	DCDHeader *ptr;

	if ((ptr = (DCDHeader *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_DCDHeader)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> number_of_comments;

		valobj = PyInt_FromLong((long)val);

		return valobj;
	}

	val = (int)PyInt_AsLong(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_DCDHeader,sipName_BALL_number_of_comments);
		return NULL;
	}

	ptr -> number_of_comments = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_DCDHeader_start_title_block(PyObject *sipThisObj,PyObject *valobj)
{
	int val;
	DCDHeader *ptr;

	if ((ptr = (DCDHeader *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_DCDHeader)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> start_title_block;

		valobj = PyInt_FromLong((long)val);

		return valobj;
	}

	val = (int)PyInt_AsLong(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_DCDHeader,sipName_BALL_start_title_block);
		return NULL;
	}

	ptr -> start_title_block = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_DCDHeader_end_info_block(PyObject *sipThisObj,PyObject *valobj)
{
	int val;
	DCDHeader *ptr;

	if ((ptr = (DCDHeader *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_DCDHeader)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> end_info_block;

		valobj = PyInt_FromLong((long)val);

		return valobj;
	}

	val = (int)PyInt_AsLong(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_DCDHeader,sipName_BALL_end_info_block);
		return NULL;
	}

	ptr -> end_info_block = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_DCDHeader_time_step_length(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	DCDHeader *ptr;

	if ((ptr = (DCDHeader *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_DCDHeader)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> time_step_length;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_DCDHeader,sipName_BALL_time_step_length);
		return NULL;
	}

	ptr -> time_step_length = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_DCDHeader_BALL_flag(PyObject *sipThisObj,PyObject *valobj)
{
	int val;
	DCDHeader *ptr;

	if ((ptr = (DCDHeader *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_DCDHeader)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> BALL_flag;

		valobj = PyInt_FromLong((long)val);

		return valobj;
	}

	val = (int)PyInt_AsLong(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_DCDHeader,sipName_BALL_BALL_flag);
		return NULL;
	}

	ptr -> BALL_flag = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_DCDHeader_steps_between_saves(PyObject *sipThisObj,PyObject *valobj)
{
	int val;
	DCDHeader *ptr;

	if ((ptr = (DCDHeader *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_DCDHeader)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> steps_between_saves;

		valobj = PyInt_FromLong((long)val);

		return valobj;
	}

	val = (int)PyInt_AsLong(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_DCDHeader,sipName_BALL_steps_between_saves);
		return NULL;
	}

	ptr -> steps_between_saves = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_DCDHeader_step_number_of_starting_time(PyObject *sipThisObj,PyObject *valobj)
{
	int val;
	DCDHeader *ptr;

	if ((ptr = (DCDHeader *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_DCDHeader)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> step_number_of_starting_time;

		valobj = PyInt_FromLong((long)val);

		return valobj;
	}

	val = (int)PyInt_AsLong(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_DCDHeader,sipName_BALL_step_number_of_starting_time);
		return NULL;
	}

	ptr -> step_number_of_starting_time = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_DCDHeader_number_of_coordinate_sets(PyObject *sipThisObj,PyObject *valobj)
{
	int val;
	DCDHeader *ptr;

	if ((ptr = (DCDHeader *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_DCDHeader)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> number_of_coordinate_sets;

		valobj = PyInt_FromLong((long)val);

		return valobj;
	}

	val = (int)PyInt_AsLong(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_DCDHeader,sipName_BALL_number_of_coordinate_sets);
		return NULL;
	}

	ptr -> number_of_coordinate_sets = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_DCDHeader_start_info_block(PyObject *sipThisObj,PyObject *valobj)
{
	int val;
	DCDHeader *ptr;

	if ((ptr = (DCDHeader *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_DCDHeader)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> start_info_block;

		valobj = PyInt_FromLong((long)val);

		return valobj;
	}

	val = (int)PyInt_AsLong(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_DCDHeader,sipName_BALL_start_info_block);
		return NULL;
	}

	ptr -> start_info_block = val;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_DCDHeader[] = {
	{NULL}
};

PyMethodDef sipClassVarTab_DCDHeader[] = {
	{sipName_BALL_end_atomnumber_block, sipGetSetVar_DCDHeader_end_atomnumber_block, 0, NULL},
	{sipName_BALL_number_of_atoms, sipGetSetVar_DCDHeader_number_of_atoms, 0, NULL},
	{sipName_BALL_start_atomnumber_block, sipGetSetVar_DCDHeader_start_atomnumber_block, 0, NULL},
	{sipName_BALL_end_title_block, sipGetSetVar_DCDHeader_end_title_block, 0, NULL},
	{sipName_BALL_number_of_comments, sipGetSetVar_DCDHeader_number_of_comments, 0, NULL},
	{sipName_BALL_start_title_block, sipGetSetVar_DCDHeader_start_title_block, 0, NULL},
	{sipName_BALL_end_info_block, sipGetSetVar_DCDHeader_end_info_block, 0, NULL},
	{sipName_BALL_time_step_length, sipGetSetVar_DCDHeader_time_step_length, 0, NULL},
	{sipName_BALL_BALL_flag, sipGetSetVar_DCDHeader_BALL_flag, 0, NULL},
	{sipName_BALL_steps_between_saves, sipGetSetVar_DCDHeader_steps_between_saves, 0, NULL},
	{sipName_BALL_step_number_of_starting_time, sipGetSetVar_DCDHeader_step_number_of_starting_time, 0, NULL},
	{sipName_BALL_number_of_coordinate_sets, sipGetSetVar_DCDHeader_number_of_coordinate_sets, 0, NULL},
	{sipName_BALL_start_info_block, sipGetSetVar_DCDHeader_start_info_block, 0, NULL},
	{NULL}
};

PyMethodDef *sipClassVarHierTab_DCDHeader[] = {
	sipClassVarTab_DCDHeader,
	NULL
};

int sipCanConvertTo_DCDHeader(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_DCDHeader);
}

void sipConvertTo_DCDHeader(PyObject *sipPy,DCDHeader **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_DCDHeader);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (DCDHeader *)sipConvertToCpp(sipPy,sipClass_DCDHeader,sipIsErr);
}

DCDHeader *sipForceConvertTo_DCDHeader(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_DCDHeader(valobj))
	{
		DCDHeader *val;

		sipConvertTo_DCDHeader(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_DCDHeader);

	*iserrp = 1;

	return NULL;
}
