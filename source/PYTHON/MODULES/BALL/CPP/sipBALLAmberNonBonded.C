#include "sipBALLDeclBALL.h"
#include "sipBALLAmberNonBonded.h"


PyObject *sipClass_AmberNonBonded;

static void sipDealloc_AmberNonBonded(sipThisType *);

static PyTypeObject sipType_AmberNonBonded = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_AmberNonBonded,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_AmberNonBonded,
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

sipAmberNonBonded::sipAmberNonBonded()
   : AmberNonBonded()
{
	sipCommonCtor(sipPyMethods,6);
}

sipAmberNonBonded::sipAmberNonBonded(ForceField& a0)
   : AmberNonBonded(a0)
{
	sipCommonCtor(sipPyMethods,6);
}

sipAmberNonBonded::sipAmberNonBonded(const AmberNonBonded& a0,bool a1)
   : AmberNonBonded(a0,a1)
{
	sipCommonCtor(sipPyMethods,6);
}

sipAmberNonBonded::sipAmberNonBonded(const AmberNonBonded& a0)
   : AmberNonBonded(a0)
{
	sipCommonCtor(sipPyMethods,6);
}

sipAmberNonBonded::~sipAmberNonBonded()
 throw()
{
	sipCommonDtor(sipPyThis);
}

double sipAmberNonBonded::getEnergy() const

{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_getEnergy,&relLock) ?
		sipForceFieldComponent::sipVH_getEnergy(&sipPyMethods[0],sipPyThis,relLock) :
		ForceFieldComponent::getEnergy();
}

bool sipAmberNonBonded::setup()
 throw()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_setup,&relLock) ?
		sipForceFieldComponent::sipVH_setup(&sipPyMethods[1],sipPyThis,relLock) :
		AmberNonBonded::setup();
}

double sipAmberNonBonded::getVdwEnergy() const
 throw()
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_getVdwEnergy,&relLock) ?
		sipAmberNonBonded::sipVH_getVdwEnergy(&sipPyMethods[2],sipPyThis,relLock) :
		AmberNonBonded::getVdwEnergy();
}

double sipAmberNonBonded::getElectrostaticEnergy() const
 throw()
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_getElectrostaticEnergy,&relLock) ?
		sipAmberNonBonded::sipVH_getElectrostaticEnergy(&sipPyMethods[3],sipPyThis,relLock) :
		AmberNonBonded::getElectrostaticEnergy();
}

void sipAmberNonBonded::updateForces()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_updateForces,&relLock))
		sipForceFieldComponent::sipVH_updateForces(&sipPyMethods[4],sipPyThis,relLock);
	else
		AmberNonBonded::updateForces();
}

double sipAmberNonBonded::updateEnergy()
 throw()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[5],sipPyThis,NULL,sipName_BALL_updateEnergy,&relLock) ?
		sipForceFieldComponent::sipVH_updateEnergy(&sipPyMethods[5],sipPyThis,relLock) :
		AmberNonBonded::updateEnergy();
}

// The common handler for all classes that inherit this virtual member
// function.

double sipAmberNonBonded::sipVH_getVdwEnergy(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_AmberNonBonded,sipName_BALL_getVdwEnergy);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

double sipAmberNonBonded::sipVH_getElectrostaticEnergy(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_AmberNonBonded,sipName_BALL_getElectrostaticEnergy);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

static PyObject *sipDo_AmberNonBonded_setup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberNonBonded)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			AmberNonBonded *ptr;

			if ((ptr = (AmberNonBonded *)sipGetCppPtr(sipThis,sipClass_AmberNonBonded)) == NULL)
				return NULL;

			res = ptr -> AmberNonBonded::setup();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AmberNonBonded,sipName_BALL_setup);

	return NULL;
}

static PyObject *sipDo_AmberNonBonded_updateEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberNonBonded)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			AmberNonBonded *ptr;

			if ((ptr = (AmberNonBonded *)sipGetCppPtr(sipThis,sipClass_AmberNonBonded)) == NULL)
				return NULL;

			res = ptr -> AmberNonBonded::updateEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AmberNonBonded,sipName_BALL_updateEnergy);

	return NULL;
}

static PyObject *sipDo_AmberNonBonded_updateForces(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberNonBonded)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			AmberNonBonded *ptr;

			if ((ptr = (AmberNonBonded *)sipGetCppPtr(sipThis,sipClass_AmberNonBonded)) == NULL)
				return NULL;

			ptr -> AmberNonBonded::updateForces();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AmberNonBonded,sipName_BALL_updateForces);

	return NULL;
}

static PyObject *sipDo_AmberNonBonded_getElectrostaticEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberNonBonded)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			AmberNonBonded *ptr;

			if ((ptr = (AmberNonBonded *)sipGetCppPtr(sipThis,sipClass_AmberNonBonded)) == NULL)
				return NULL;

			res = ptr -> AmberNonBonded::getElectrostaticEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AmberNonBonded,sipName_BALL_getElectrostaticEnergy);

	return NULL;
}

static PyObject *sipDo_AmberNonBonded_getVdwEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberNonBonded)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			AmberNonBonded *ptr;

			if ((ptr = (AmberNonBonded *)sipGetCppPtr(sipThis,sipClass_AmberNonBonded)) == NULL)
				return NULL;

			res = ptr -> AmberNonBonded::getVdwEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AmberNonBonded,sipName_BALL_getVdwEnergy);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_AmberNonBonded(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_AmberNonBonded)
		return ptr;

	if ((res = sipCast_ForceFieldComponent((ForceFieldComponent *)(AmberNonBonded *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_AmberNonBonded(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipAmberNonBonded *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (AmberNonBonded *)sipThis -> u.cppPtr;
			else
				delete (sipAmberNonBonded *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_AmberNonBonded(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_AmberNonBonded
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
			sipNew = new sipAmberNonBonded();
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
		ForceField * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_ForceField,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_ForceField(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipAmberNonBonded(* a0);
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
		const AmberNonBonded * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_AmberNonBonded,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_AmberNonBonded(a0obj,(AmberNonBonded **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipAmberNonBonded(* a0, (bool)a1);
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
		const AmberNonBonded * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_AmberNonBonded,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_AmberNonBonded(a0obj,(AmberNonBonded **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipAmberNonBonded(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_AmberNonBonded);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_AmberNonBonded,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (AmberNonBonded *)sipNew;
			else
				delete (sipAmberNonBonded *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipAmberNonBonded *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_AmberNonBonded[] = {
	{sipName_BALL_setup, sipDo_AmberNonBonded_setup, METH_VARARGS, NULL},
	{sipName_BALL_updateEnergy, sipDo_AmberNonBonded_updateEnergy, METH_VARARGS, NULL},
	{sipName_BALL_updateForces, sipDo_AmberNonBonded_updateForces, METH_VARARGS, NULL},
	{sipName_BALL_getElectrostaticEnergy, sipDo_AmberNonBonded_getElectrostaticEnergy, METH_VARARGS, NULL},
	{sipName_BALL_getVdwEnergy, sipDo_AmberNonBonded_getVdwEnergy, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_AmberNonBonded(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_AmberNonBonded);
}

void sipConvertTo_AmberNonBonded(PyObject *sipPy,AmberNonBonded **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_AmberNonBonded);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (AmberNonBonded *)sipConvertToCpp(sipPy,sipClass_AmberNonBonded,sipIsErr);
}

AmberNonBonded *sipForceConvertTo_AmberNonBonded(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_AmberNonBonded(valobj))
	{
		AmberNonBonded *val;

		sipConvertTo_AmberNonBonded(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_AmberNonBonded);

	*iserrp = 1;

	return NULL;
}
