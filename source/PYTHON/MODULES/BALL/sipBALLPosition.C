#include "sipBALLDeclBALL.h"
#include "sipBALLPosition.h"



PyObject *sipClass_Position;

static void sipDealloc_Position(sipThisType *);
static PyObject *sipPyInternalRepr_Position(sipThisType *);

static PyTypeObject sipType_Position = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Position,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Position,
	0,
	0,
	0,
	0,
	(reprfunc)sipPyInternalRepr_Position,
};

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Position(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Position)
		return ptr;

	return NULL;
}

static void sipDealloc_Position(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (Position *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

static PyObject *sipPyInternalRepr_Position(sipThisType *sipThis)
{
#line 37 "global.sip"
	Position* ptr;
  if ((ptr = (Position*)sipGetCppPtr(sipThis,sipClass_Position)) == NULL)
    return NULL;

  return PyString_FromString(String(*ptr).c_str());
#line 59 "./sipBALLPosition.cpp"
}

PyObject *sipNew_Position(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Position
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
			sipNew = new Position();
		}
	}

	if (sipNew == NULL)
	{
		const Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_Position,&a0obj))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,(Position **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Position(* a0);

			if (istemp0)
				delete a0;
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_Position);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Position,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (Position *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Position[] = {
	{NULL}
};

int sipCanConvertTo_Position(PyObject *sipPy)
{
#line 44 "global.sip"
	// automatic conversion of Py integers to Position 
	return (PyInt_Check(sipPy) || sipIsSubClassInstance(sipPy, sipClass_Position));
#line 134 "./sipBALLPosition.cpp"
}

int sipConvertTo_Position(PyObject *sipPy,Position **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return false;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_Position);
		else
			*sipCppPtr = NULL;

		return false;
	}

#line 48 "global.sip"
	if (PyInt_Check(sipPy))
	{
		*sipCppPtr = new Position(PyInt_AS_LONG(sipPy));
	
		return 1;
	}
	
	*sipCppPtr = (Position*)sipConvertToCpp(sipPy, sipClass_Position, sipIsErr);

	return 0;
#line 163 "./sipBALLPosition.cpp"
}

Position *sipForceConvertTo_Position(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Position(valobj))
	{
		Position *val;

		sipConvertTo_Position(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Position);
	*iserrp = 1;

	return NULL;
}
