#include "sipBALLDeclBALL.h"
#include "sipBALLIndex.h"



PyObject *sipClass_Index;

static void sipDealloc_Index(sipThisType *);
static PyObject * sip__str__Index(PyObject *a0);

static PyTypeObject sipType_Index = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Index,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Index,
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
	sip__str__Index,
	0,
	0,
	0,
	Py_TPFLAGS_DEFAULT,
	0,
	0,
	0,
};

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Index(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Index)
		return ptr;

	return NULL;
}

static void sipDealloc_Index(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (Index *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}
static PyObject * sip__str__Index(PyObject *a0)
{
#line 9 "global.sip"
	Index* ptr;
  if ((ptr = (Index*)sipGetCppPtr((sipThisType*)a0,sipClass_Index)) == NULL)
    return NULL;

  return PyString_FromString(String(*ptr).c_str());
#line 71 "sipBALLIndex.cpp"
}

PyObject *sipNew_Index(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Index
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
			sipNew = new Index();
		}
	}

	if (sipNew == NULL)
	{
		const Index *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_Index,&a0obj))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,(Index **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Index(* a0);

			if (istemp0)
				delete a0;
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_Index);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Index,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (Index *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Index[] = {
	{NULL}
};

int sipCanConvertTo_Index(PyObject *sipPy)
{
#line 16 "global.sip"
	// automatic conversion of Py integers to Position 
	return (PyInt_Check(sipPy) || sipIsSubClassInstance(sipPy, sipClass_Index));
#line 147 "sipBALLIndex.cpp"
}

int sipConvertTo_Index(PyObject *sipPy,Index **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return 0;

#line 20 "global.sip"
	if (PyInt_Check(sipPy))
	{
		*sipCppPtr = new Index(PyInt_AS_LONG(sipPy));
	
		return 1;
	}
	
	*sipCppPtr = (Index*)sipConvertToCpp(sipPy, sipClass_Index, sipIsErr);

	return 0;
#line 166 "sipBALLIndex.cpp"
}

Index *sipForceConvertTo_Index(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Index(valobj))
	{
		Index *val;

		sipConvertTo_Index(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Index);
	*iserrp = 1;

	return NULL;
}
