#include "sipBALLDeclBALL.h"
#include "sipBALLBit.h"


PyObject *sipClass_Bit;

static void sipDealloc_Bit(sipThisType *);

static PyTypeObject sipType_Bit = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Bit,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Bit,
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

sipBit::sipBit()
   throw() : Bit()
{
	sipCommonCtor(sipPyMethods,1);
}

sipBit::sipBit(const Bit& a0)
   throw() : Bit(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipBit::~sipBit()
 throw()
{
	sipCommonDtor(sipPyThis);
}

void sipBit::clear()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipBit::sipVH_clear(&sipPyMethods[0],sipPyThis,relLock);
	else
		Bit::clear();
}

// The common handler for all classes that inherit this virtual member
// function.

void sipBit::sipVH_clear(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
{
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(O)",sipThis -> sipSelf);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_Bit,sipName_BALL_clear);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

static PyObject *sipDo_Bit_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bit)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Bit *ptr;

			if ((ptr = (Bit *)sipGetCppPtr(sipThis,sipClass_Bit)) == NULL)
				return NULL;

			ptr -> Bit::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Bit,sipName_BALL_clear);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Bit(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Bit)
		return ptr;

	return NULL;
}

static void sipDealloc_Bit(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipBit *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (Bit *)sipThis -> u.cppPtr;
			else
				delete (sipBit *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_Bit(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Bit
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
			sipNew = new sipBit();
		}
	}

	if (sipNew == NULL)
	{
		const Bit * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_Bit,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Bit(a0obj,(Bit **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipBit(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_Bit);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Bit,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (Bit *)sipNew;
			else
				delete (sipBit *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipBit *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Bit[] = {
	{sipName_BALL_clear, sipDo_Bit_clear, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_Bit(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Bit);
}

void sipConvertTo_Bit(PyObject *sipPy,Bit **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_Bit);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Bit *)sipConvertToCpp(sipPy,sipClass_Bit,sipIsErr);
}

Bit *sipForceConvertTo_Bit(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Bit(valobj))
	{
		Bit *val;

		sipConvertTo_Bit(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Bit);

	*iserrp = 1;

	return NULL;
}
