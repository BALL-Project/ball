#include "sipBALLDeclBALL.h"
#include "sipBALLCharmmNonBonded.h"



PyObject *sipClass_CharmmNonBonded;

static void sipDealloc_CharmmNonBonded(sipThisType *);

static PyTypeObject sipType_CharmmNonBonded = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_CharmmNonBonded,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_CharmmNonBonded,
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

sipCharmmNonBonded::sipCharmmNonBonded()
    : CharmmNonBonded()
{
	sipCommonCtor(sipPyMethods,7);
}

sipCharmmNonBonded::sipCharmmNonBonded(ForceField& a0)
    : CharmmNonBonded(a0)
{
	sipCommonCtor(sipPyMethods,7);
}

sipCharmmNonBonded::sipCharmmNonBonded(const CharmmNonBonded& a0,bool a1)
    : CharmmNonBonded(a0,a1)
{
	sipCommonCtor(sipPyMethods,7);
}

sipCharmmNonBonded::sipCharmmNonBonded(const CharmmNonBonded& a0)
    : CharmmNonBonded(a0)
{
	sipCommonCtor(sipPyMethods,7);
}

sipCharmmNonBonded::~sipCharmmNonBonded()
  throw()
{
	sipCommonDtor(sipPyThis);
}

double sipCharmmNonBonded::getEnergy() const

{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_getEnergy,&relLock) ?
		sipForceFieldComponent::sipVH_getEnergy(&sipPyMethods[0],sipPyThis,relLock) :
		ForceFieldComponent::getEnergy();
}

double sipCharmmNonBonded::getSolvationEnergy() const
 throw()
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_getSolvationEnergy,&relLock) ?
		sipCharmmNonBonded::sipVH_getSolvationEnergy(&sipPyMethods[1],sipPyThis,relLock) :
		CharmmNonBonded::getSolvationEnergy();
}

double sipCharmmNonBonded::getVdwEnergy() const
 throw()
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_getVdwEnergy,&relLock) ?
		sipCharmmNonBonded::sipVH_getVdwEnergy(&sipPyMethods[2],sipPyThis,relLock) :
		CharmmNonBonded::getVdwEnergy();
}

double sipCharmmNonBonded::getElectrostaticEnergy() const
 throw()
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_getElectrostaticEnergy,&relLock) ?
		sipCharmmNonBonded::sipVH_getElectrostaticEnergy(&sipPyMethods[3],sipPyThis,relLock) :
		CharmmNonBonded::getElectrostaticEnergy();
}

void sipCharmmNonBonded::updateForces()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_updateForces,&relLock))
		sipForceFieldComponent::sipVH_updateForces(&sipPyMethods[4],sipPyThis,relLock);
	else
		CharmmNonBonded::updateForces();
}

double sipCharmmNonBonded::updateEnergy()
 throw()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[5],sipPyThis,NULL,sipName_BALL_updateEnergy,&relLock) ?
		sipForceFieldComponent::sipVH_updateEnergy(&sipPyMethods[5],sipPyThis,relLock) :
		CharmmNonBonded::updateEnergy();
}

bool sipCharmmNonBonded::setup()
 throw()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[6],sipPyThis,NULL,sipName_BALL_setup,&relLock) ?
		sipForceFieldComponent::sipVH_setup(&sipPyMethods[6],sipPyThis,relLock) :
		CharmmNonBonded::setup();
}

// The common handler for all classes that inherit this virtual member
// function.

double sipCharmmNonBonded::sipVH_getSolvationEnergy(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
{
	double res;
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(O)",sipThis -> sipSelf);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		res = PyFloat_AsDouble(resobj);

		Py_DECREF(resobj);

		if (PyErr_Occurred() == NULL)
		{
			goto releaseLock;
		}

		sipBadVirtualResultType(sipName_BALL_CharmmNonBonded,sipName_BALL_getSolvationEnergy);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

double sipCharmmNonBonded::sipVH_getVdwEnergy(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
{
	double res;
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(O)",sipThis -> sipSelf);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		res = PyFloat_AsDouble(resobj);

		Py_DECREF(resobj);

		if (PyErr_Occurred() == NULL)
		{
			goto releaseLock;
		}

		sipBadVirtualResultType(sipName_BALL_CharmmNonBonded,sipName_BALL_getVdwEnergy);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

double sipCharmmNonBonded::sipVH_getElectrostaticEnergy(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
{
	double res;
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(O)",sipThis -> sipSelf);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		res = PyFloat_AsDouble(resobj);

		Py_DECREF(resobj);

		if (PyErr_Occurred() == NULL)
		{
			goto releaseLock;
		}

		sipBadVirtualResultType(sipName_BALL_CharmmNonBonded,sipName_BALL_getElectrostaticEnergy);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

static PyObject *sipDo_CharmmNonBonded_setup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmNonBonded)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			CharmmNonBonded *ptr;

			if ((ptr = (CharmmNonBonded *)sipGetCppPtr(sipThis,sipClass_CharmmNonBonded)) == NULL)
				return NULL;

			res = ptr -> CharmmNonBonded::setup();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmNonBonded,sipName_BALL_setup);

	return NULL;
}

static PyObject *sipDo_CharmmNonBonded_updateEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmNonBonded)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			CharmmNonBonded *ptr;

			if ((ptr = (CharmmNonBonded *)sipGetCppPtr(sipThis,sipClass_CharmmNonBonded)) == NULL)
				return NULL;

			res = ptr -> CharmmNonBonded::updateEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmNonBonded,sipName_BALL_updateEnergy);

	return NULL;
}

static PyObject *sipDo_CharmmNonBonded_updateForces(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmNonBonded)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			CharmmNonBonded *ptr;

			if ((ptr = (CharmmNonBonded *)sipGetCppPtr(sipThis,sipClass_CharmmNonBonded)) == NULL)
				return NULL;

			ptr -> CharmmNonBonded::updateForces();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmNonBonded,sipName_BALL_updateForces);

	return NULL;
}

static PyObject *sipDo_CharmmNonBonded_getElectrostaticEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmNonBonded)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			CharmmNonBonded *ptr;

			if ((ptr = (CharmmNonBonded *)sipGetCppPtr(sipThis,sipClass_CharmmNonBonded)) == NULL)
				return NULL;

			res = ptr -> CharmmNonBonded::getElectrostaticEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmNonBonded,sipName_BALL_getElectrostaticEnergy);

	return NULL;
}

static PyObject *sipDo_CharmmNonBonded_getVdwEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmNonBonded)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			CharmmNonBonded *ptr;

			if ((ptr = (CharmmNonBonded *)sipGetCppPtr(sipThis,sipClass_CharmmNonBonded)) == NULL)
				return NULL;

			res = ptr -> CharmmNonBonded::getVdwEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmNonBonded,sipName_BALL_getVdwEnergy);

	return NULL;
}

static PyObject *sipDo_CharmmNonBonded_getSolvationEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmNonBonded)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			CharmmNonBonded *ptr;

			if ((ptr = (CharmmNonBonded *)sipGetCppPtr(sipThis,sipClass_CharmmNonBonded)) == NULL)
				return NULL;

			res = ptr -> CharmmNonBonded::getSolvationEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmNonBonded,sipName_BALL_getSolvationEnergy);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_CharmmNonBonded(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_CharmmNonBonded)
		return ptr;

	if ((res = sipCast_ForceFieldComponent((ForceFieldComponent *)(CharmmNonBonded *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_CharmmNonBonded(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipCharmmNonBonded *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (CharmmNonBonded *)sipThis -> u.cppPtr;
			else
				delete (sipCharmmNonBonded *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_CharmmNonBonded(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_CharmmNonBonded
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
			sipNew = new sipCharmmNonBonded();
		}
	}

	if (sipNew == NULL)
	{
		ForceField *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_ForceField,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_ForceField(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipCharmmNonBonded(* a0);
		}
	}

	if (sipNew == NULL)
	{
		const CharmmNonBonded *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_CharmmNonBonded,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_CharmmNonBonded(a0obj,(CharmmNonBonded **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipCharmmNonBonded(* a0, (bool)a1);
		}
	}

	if (sipNew == NULL)
	{
		const CharmmNonBonded *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_CharmmNonBonded,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_CharmmNonBonded(a0obj,(CharmmNonBonded **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipCharmmNonBonded(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_CharmmNonBonded);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_CharmmNonBonded,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (CharmmNonBonded *)sipNew;
			else
				delete (sipCharmmNonBonded *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipCharmmNonBonded *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_CharmmNonBonded[] = {
	{sipName_BALL_setup, sipDo_CharmmNonBonded_setup, METH_VARARGS, NULL},
	{sipName_BALL_updateEnergy, sipDo_CharmmNonBonded_updateEnergy, METH_VARARGS, NULL},
	{sipName_BALL_updateForces, sipDo_CharmmNonBonded_updateForces, METH_VARARGS, NULL},
	{sipName_BALL_getElectrostaticEnergy, sipDo_CharmmNonBonded_getElectrostaticEnergy, METH_VARARGS, NULL},
	{sipName_BALL_getVdwEnergy, sipDo_CharmmNonBonded_getVdwEnergy, METH_VARARGS, NULL},
	{sipName_BALL_getSolvationEnergy, sipDo_CharmmNonBonded_getSolvationEnergy, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_CharmmNonBonded(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_CharmmNonBonded);
}

void sipConvertTo_CharmmNonBonded(PyObject *sipPy,CharmmNonBonded **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_CharmmNonBonded);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (CharmmNonBonded *)sipConvertToCpp(sipPy,sipClass_CharmmNonBonded,sipIsErr);
}

CharmmNonBonded *sipForceConvertTo_CharmmNonBonded(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_CharmmNonBonded(valobj))
	{
		CharmmNonBonded *val;

		sipConvertTo_CharmmNonBonded(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_CharmmNonBonded);
	*iserrp = 1;

	return NULL;
}
