#include "sipBALLDeclBALL.h"
#include "sipBALLCharmmFF.h"



PyObject *sipClass_CharmmFF;

static void sipDealloc_CharmmFF(sipThisType *);

static PyTypeObject sipType_CharmmFF = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_CharmmFF,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_CharmmFF,
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

sipCharmmFF::sipCharmmFF()
    : CharmmFF()
{
	sipCommonCtor(sipPyMethods,1);
}

sipCharmmFF::sipCharmmFF(System& a0)
    : CharmmFF(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipCharmmFF::sipCharmmFF(System& a0,const Options& a1)
    : CharmmFF(a0,a1)
{
	sipCommonCtor(sipPyMethods,1);
}

sipCharmmFF::sipCharmmFF(const CharmmFF& a0)
    : CharmmFF(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipCharmmFF::~sipCharmmFF()
 
{
	sipCommonDtor(sipPyThis);
}

bool sipCharmmFF::specificSetup()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_specificSetup,&relLock) ?
		sipForceField::sipVH_specificSetup(&sipPyMethods[0],sipPyThis,relLock) :
		CharmmFF::specificSetup();
}

static PyObject *sipDo_CharmmFF_specificSetup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmFF)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			CharmmFF *ptr;

			if ((ptr = (CharmmFF *)sipGetCppPtr(sipThis,sipClass_CharmmFF)) == NULL)
				return NULL;

			res = ptr -> CharmmFF::specificSetup();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmFF,sipName_BALL_specificSetup);

	return NULL;
}

static PyObject *sipDo_CharmmFF_getStretchEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmFF)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			CharmmFF *ptr;

			if ((ptr = (CharmmFF *)sipGetCppPtr(sipThis,sipClass_CharmmFF)) == NULL)
				return NULL;

			res = ptr -> CharmmFF::getStretchEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmFF,sipName_BALL_getStretchEnergy);

	return NULL;
}

static PyObject *sipDo_CharmmFF_getBendEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmFF)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			CharmmFF *ptr;

			if ((ptr = (CharmmFF *)sipGetCppPtr(sipThis,sipClass_CharmmFF)) == NULL)
				return NULL;

			res = ptr -> CharmmFF::getBendEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmFF,sipName_BALL_getBendEnergy);

	return NULL;
}

static PyObject *sipDo_CharmmFF_getTorsionEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmFF)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			CharmmFF *ptr;

			if ((ptr = (CharmmFF *)sipGetCppPtr(sipThis,sipClass_CharmmFF)) == NULL)
				return NULL;

			res = ptr -> CharmmFF::getTorsionEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmFF,sipName_BALL_getTorsionEnergy);

	return NULL;
}

static PyObject *sipDo_CharmmFF_getImproperTorsionEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmFF)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			CharmmFF *ptr;

			if ((ptr = (CharmmFF *)sipGetCppPtr(sipThis,sipClass_CharmmFF)) == NULL)
				return NULL;

			res = ptr -> CharmmFF::getImproperTorsionEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmFF,sipName_BALL_getImproperTorsionEnergy);

	return NULL;
}

static PyObject *sipDo_CharmmFF_getProperTorsionEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmFF)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			CharmmFF *ptr;

			if ((ptr = (CharmmFF *)sipGetCppPtr(sipThis,sipClass_CharmmFF)) == NULL)
				return NULL;

			res = ptr -> CharmmFF::getProperTorsionEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmFF,sipName_BALL_getProperTorsionEnergy);

	return NULL;
}

static PyObject *sipDo_CharmmFF_getNonbondedEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmFF)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			CharmmFF *ptr;

			if ((ptr = (CharmmFF *)sipGetCppPtr(sipThis,sipClass_CharmmFF)) == NULL)
				return NULL;

			res = ptr -> CharmmFF::getNonbondedEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmFF,sipName_BALL_getNonbondedEnergy);

	return NULL;
}

static PyObject *sipDo_CharmmFF_getVdWEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmFF)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			CharmmFF *ptr;

			if ((ptr = (CharmmFF *)sipGetCppPtr(sipThis,sipClass_CharmmFF)) == NULL)
				return NULL;

			res = ptr -> CharmmFF::getVdWEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmFF,sipName_BALL_getVdWEnergy);

	return NULL;
}

static PyObject *sipDo_CharmmFF_getESEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmFF)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			CharmmFF *ptr;

			if ((ptr = (CharmmFF *)sipGetCppPtr(sipThis,sipClass_CharmmFF)) == NULL)
				return NULL;

			res = ptr -> CharmmFF::getESEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmFF,sipName_BALL_getESEnergy);

	return NULL;
}

static PyObject *sipDo_CharmmFF_getSolvationEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmFF)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			CharmmFF *ptr;

			if ((ptr = (CharmmFF *)sipGetCppPtr(sipThis,sipClass_CharmmFF)) == NULL)
				return NULL;

			res = ptr -> CharmmFF::getSolvationEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmFF,sipName_BALL_getSolvationEnergy);

	return NULL;
}

static PyObject *sipDo_CharmmFF_hasInitializedParameters(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmFF)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			CharmmFF *ptr;

			if ((ptr = (CharmmFF *)sipGetCppPtr(sipThis,sipClass_CharmmFF)) == NULL)
				return NULL;

			res = ptr -> CharmmFF::hasInitializedParameters();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmFF,sipName_BALL_hasInitializedParameters);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_CharmmFF(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_CharmmFF)
		return ptr;

	if ((res = sipCast_ForceField((ForceField *)(CharmmFF *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_CharmmFF(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipCharmmFF *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (CharmmFF *)sipThis -> u.cppPtr;
			else
				delete (sipCharmmFF *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_CharmmFF(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_CharmmFF
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
			sipNew = new sipCharmmFF();
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

			sipNew = new sipCharmmFF(* a0);
		}
	}

	if (sipNew == NULL)
	{
		System *a0;
		PyObject *a0obj;
		const Options *a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-II",sipCanConvertTo_System,&a0obj,sipCanConvertTo_Options,&a1obj))
		{
			int iserr = 0;

			sipConvertTo_System(a0obj,&a0,1,&iserr);
			sipConvertTo_Options(a1obj,(Options **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipCharmmFF(* a0,* a1);
		}
	}

	if (sipNew == NULL)
	{
		const CharmmFF *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_CharmmFF,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_CharmmFF(a0obj,(CharmmFF **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipCharmmFF(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_CharmmFF);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_CharmmFF,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (CharmmFF *)sipNew;
			else
				delete (sipCharmmFF *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipCharmmFF *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_CharmmFF[] = {
	{sipName_BALL_specificSetup, sipDo_CharmmFF_specificSetup, METH_VARARGS, NULL},
	{sipName_BALL_getStretchEnergy, sipDo_CharmmFF_getStretchEnergy, METH_VARARGS, NULL},
	{sipName_BALL_getBendEnergy, sipDo_CharmmFF_getBendEnergy, METH_VARARGS, NULL},
	{sipName_BALL_getTorsionEnergy, sipDo_CharmmFF_getTorsionEnergy, METH_VARARGS, NULL},
	{sipName_BALL_getImproperTorsionEnergy, sipDo_CharmmFF_getImproperTorsionEnergy, METH_VARARGS, NULL},
	{sipName_BALL_getProperTorsionEnergy, sipDo_CharmmFF_getProperTorsionEnergy, METH_VARARGS, NULL},
	{sipName_BALL_getNonbondedEnergy, sipDo_CharmmFF_getNonbondedEnergy, METH_VARARGS, NULL},
	{sipName_BALL_getVdWEnergy, sipDo_CharmmFF_getVdWEnergy, METH_VARARGS, NULL},
	{sipName_BALL_getESEnergy, sipDo_CharmmFF_getESEnergy, METH_VARARGS, NULL},
	{sipName_BALL_getSolvationEnergy, sipDo_CharmmFF_getSolvationEnergy, METH_VARARGS, NULL},
	{sipName_BALL_hasInitializedParameters, sipDo_CharmmFF_hasInitializedParameters, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_CharmmFF(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_CharmmFF);
}

void sipConvertTo_CharmmFF(PyObject *sipPy,CharmmFF **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_CharmmFF);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (CharmmFF *)sipConvertToCpp(sipPy,sipClass_CharmmFF,sipIsErr);
}

CharmmFF *sipForceConvertTo_CharmmFF(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_CharmmFF(valobj))
	{
		CharmmFF *val;

		sipConvertTo_CharmmFF(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_CharmmFF);
	*iserrp = 1;

	return NULL;
}
