#include "sipBALLDeclBALL.h"
#include "sipBALLAtomType.h"



PyObject *sipClass_AtomType;

static void sipDealloc_AtomType(sipThisType *);
static PyObject *sipPyInternalRepr_AtomType(sipThisType *);

static PyTypeObject sipType_AtomType = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_AtomType,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_AtomType,
	0,
	0,
	0,
	0,
	(reprfunc)sipPyInternalRepr_AtomType,
};

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_AtomType(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_AtomType)
		return ptr;

	return NULL;
}

static void sipDealloc_AtomType(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (AtomType *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

static PyObject *sipPyInternalRepr_AtomType(sipThisType *sipThis)
{
#line 122 "atom.sip"
	AtomType* ptr;
  if ((ptr = (AtomType*)sipGetCppPtr(sipThis,sipClass_AtomType)) == NULL)
    return NULL;

  return PyString_FromString(String(*ptr).c_str());
#line 59 "../CPP/sipBALLAtomType.cpp"
}

PyObject *sipNew_AtomType(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_AtomType
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
			sipNew = new AtomType();
	}
	}

	if (sipNew == NULL)
	{
		const AtomType *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_AtomType,&a0obj))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_AtomType(a0obj,(AtomType **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new AtomType(* a0);

			if (istemp0)
				delete a0;
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_AtomType);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_AtomType,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (AtomType *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_AtomType[] = {
	{NULL}
};

int sipCanConvertTo_AtomType(PyObject *sipPy)
{
#line 129 "atom.sip"
	// automatic conversion of Py integers to Position 
	return (PyInt_Check(sipPy) || sipIsSubClassInstance(sipPy, sipClass_AtomType));
#line 134 "../CPP/sipBALLAtomType.cpp"
}

int sipConvertTo_AtomType(PyObject *sipPy,AtomType **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return false;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_AtomType);
		else
			*sipCppPtr = NULL;

		return false;
	}

#line 133 "atom.sip"
	if (PyInt_Check(sipPy))
	{
		*sipCppPtr = new AtomType(PyInt_AS_LONG(sipPy));
	
		return 1;
	}
	
	*sipCppPtr = (AtomType*)sipConvertToCpp(sipPy, sipClass_AtomType, sipIsErr);

	return 0;
#line 163 "../CPP/sipBALLAtomType.cpp"
}

AtomType *sipForceConvertTo_AtomType(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_AtomType(valobj))
	{
		AtomType *val;

		sipConvertTo_AtomType(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_AtomType);
	*iserrp = 1;

	return NULL;
}
