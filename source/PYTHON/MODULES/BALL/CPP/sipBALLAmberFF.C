#include "sipBALLDeclBALL.h"
#include "sipBALLAmberFF.h"



PyObject *sipClass_AmberFF;

static void sipDealloc_AmberFF(sipThisType *);

static PyTypeObject sipType_AmberFF = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_AmberFF,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_AmberFF,
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

sipAmberFF::sipAmberFF()
    : AmberFF()
{
	sipCommonCtor(sipPyMethods,1);
}

sipAmberFF::sipAmberFF(System& a0)
    : AmberFF(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipAmberFF::sipAmberFF(System& a0,Options& a1)
    : AmberFF(a0,a1)
{
	sipCommonCtor(sipPyMethods,1);
}

sipAmberFF::sipAmberFF(AmberFF& a0)
    : AmberFF(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipAmberFF::~sipAmberFF()
 
{
	sipCommonDtor(sipPyThis);
}

bool sipAmberFF::specificSetup()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_specificSetup,&relLock) ?
		sipForceField::sipVH_specificSetup(&sipPyMethods[0],sipPyThis,relLock) :
		AmberFF::specificSetup();
}

static PyObject *sipDo_AmberFF_specificSetup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberFF)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			AmberFF *ptr;

			if ((ptr = (AmberFF *)sipGetCppPtr(sipThis,sipClass_AmberFF)) == NULL)
				return NULL;

			res = ptr -> AmberFF::specificSetup();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AmberFF,sipName_BALL_specificSetup);

	return NULL;
}

static PyObject *sipDo_AmberFF_getStretchEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberFF)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			AmberFF *ptr;

			if ((ptr = (AmberFF *)sipGetCppPtr(sipThis,sipClass_AmberFF)) == NULL)
				return NULL;

			res = ptr -> AmberFF::getStretchEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AmberFF,sipName_BALL_getStretchEnergy);

	return NULL;
}

static PyObject *sipDo_AmberFF_getBendEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberFF)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			AmberFF *ptr;

			if ((ptr = (AmberFF *)sipGetCppPtr(sipThis,sipClass_AmberFF)) == NULL)
				return NULL;

			res = ptr -> AmberFF::getBendEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AmberFF,sipName_BALL_getBendEnergy);

	return NULL;
}

static PyObject *sipDo_AmberFF_getTorsionEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberFF)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			AmberFF *ptr;

			if ((ptr = (AmberFF *)sipGetCppPtr(sipThis,sipClass_AmberFF)) == NULL)
				return NULL;

			res = ptr -> AmberFF::getTorsionEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AmberFF,sipName_BALL_getTorsionEnergy);

	return NULL;
}

static PyObject *sipDo_AmberFF_getNonbondedEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberFF)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			AmberFF *ptr;

			if ((ptr = (AmberFF *)sipGetCppPtr(sipThis,sipClass_AmberFF)) == NULL)
				return NULL;

			res = ptr -> AmberFF::getNonbondedEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AmberFF,sipName_BALL_getNonbondedEnergy);

	return NULL;
}

static PyObject *sipDo_AmberFF_getESEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberFF)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			AmberFF *ptr;

			if ((ptr = (AmberFF *)sipGetCppPtr(sipThis,sipClass_AmberFF)) == NULL)
				return NULL;

			res = ptr -> AmberFF::getESEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AmberFF,sipName_BALL_getESEnergy);

	return NULL;
}

static PyObject *sipDo_AmberFF_getVdWEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberFF)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			AmberFF *ptr;

			if ((ptr = (AmberFF *)sipGetCppPtr(sipThis,sipClass_AmberFF)) == NULL)
				return NULL;

			res = ptr -> AmberFF::getVdWEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AmberFF,sipName_BALL_getVdWEnergy);

	return NULL;
}

static PyObject *sipDo_AmberFF_hasInitializedParameters(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberFF)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			AmberFF *ptr;

			if ((ptr = (AmberFF *)sipGetCppPtr(sipThis,sipClass_AmberFF)) == NULL)
				return NULL;

			res = ptr -> AmberFF::hasInitializedParameters();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AmberFF,sipName_BALL_hasInitializedParameters);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_AmberFF(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_AmberFF)
		return ptr;

	if ((res = sipCast_ForceField((ForceField *)(AmberFF *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_AmberFF(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipAmberFF *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (AmberFF *)sipThis -> u.cppPtr;
			else
				delete (sipAmberFF *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_AmberFF(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_AmberFF
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
			sipNew = new sipAmberFF();
		}
	}

	if (sipNew == NULL)
	{
		System *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_System,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_System(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipAmberFF(* a0);
		}
	}

	if (sipNew == NULL)
	{
		System *a0;
		PyObject *a0obj;
		Options *a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-II",sipCanConvertTo_System,&a0obj,sipCanConvertTo_Options,&a1obj))
		{
			int iserr = 0;

			sipConvertTo_System(a0obj,&a0,1,&iserr);
			sipConvertTo_Options(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipAmberFF(* a0,* a1);
		}
	}

	if (sipNew == NULL)
	{
		AmberFF *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_AmberFF,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_AmberFF(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipAmberFF(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_AmberFF);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_AmberFF,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (AmberFF *)sipNew;
			else
				delete (sipAmberFF *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipAmberFF *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_AmberFF[] = {
	{sipName_BALL_specificSetup, sipDo_AmberFF_specificSetup, METH_VARARGS, NULL},
	{sipName_BALL_getStretchEnergy, sipDo_AmberFF_getStretchEnergy, METH_VARARGS, NULL},
	{sipName_BALL_getBendEnergy, sipDo_AmberFF_getBendEnergy, METH_VARARGS, NULL},
	{sipName_BALL_getTorsionEnergy, sipDo_AmberFF_getTorsionEnergy, METH_VARARGS, NULL},
	{sipName_BALL_getNonbondedEnergy, sipDo_AmberFF_getNonbondedEnergy, METH_VARARGS, NULL},
	{sipName_BALL_getESEnergy, sipDo_AmberFF_getESEnergy, METH_VARARGS, NULL},
	{sipName_BALL_getVdWEnergy, sipDo_AmberFF_getVdWEnergy, METH_VARARGS, NULL},
	{sipName_BALL_hasInitializedParameters, sipDo_AmberFF_hasInitializedParameters, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_AmberFF(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_AmberFF);
}

void sipConvertTo_AmberFF(PyObject *sipPy,AmberFF **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_AmberFF);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (AmberFF *)sipConvertToCpp(sipPy,sipClass_AmberFF,sipIsErr);
}

AmberFF *sipForceConvertTo_AmberFF(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_AmberFF(valobj))
	{
		AmberFF *val;

		sipConvertTo_AmberFF(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_AmberFF);
	*iserrp = 1;

	return NULL;
}
