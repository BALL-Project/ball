#include "sipBALLDeclBALL.h"
#include "sipBALLForceFieldComponent.h"



PyObject *sipClass_ForceFieldComponent;

static void sipDealloc_ForceFieldComponent(sipThisType *);

static PyTypeObject sipType_ForceFieldComponent = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_ForceFieldComponent,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_ForceFieldComponent,
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

sipForceFieldComponent::sipForceFieldComponent()
    : ForceFieldComponent()
{
	sipCommonCtor(sipPyMethods,4);
}

sipForceFieldComponent::sipForceFieldComponent(ForceField& a0)
    : ForceFieldComponent(a0)
{
	sipCommonCtor(sipPyMethods,4);
}

sipForceFieldComponent::sipForceFieldComponent(const ForceFieldComponent& a0,bool a1)
    : ForceFieldComponent(a0,a1)
{
	sipCommonCtor(sipPyMethods,4);
}

sipForceFieldComponent::sipForceFieldComponent(const ForceFieldComponent& a0)
    : ForceFieldComponent(a0)
{
	sipCommonCtor(sipPyMethods,4);
}

sipForceFieldComponent::~sipForceFieldComponent()
 
{
	sipCommonDtor(sipPyThis);
}

void sipForceFieldComponent::updateForces()

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_updateForces,&relLock))
		sipForceFieldComponent::sipVH_updateForces(&sipPyMethods[0],sipPyThis,relLock);
	else
		ForceFieldComponent::updateForces();
}

double sipForceFieldComponent::updateEnergy()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_updateEnergy,&relLock) ?
		sipForceFieldComponent::sipVH_updateEnergy(&sipPyMethods[1],sipPyThis,relLock) :
		ForceFieldComponent::updateEnergy();
}

double sipForceFieldComponent::getEnergy() const

{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_getEnergy,&relLock) ?
		sipForceFieldComponent::sipVH_getEnergy(&sipPyMethods[2],sipPyThis,relLock) :
		ForceFieldComponent::getEnergy();
}

bool sipForceFieldComponent::setup()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_setup,&relLock) ?
		sipForceFieldComponent::sipVH_setup(&sipPyMethods[3],sipPyThis,relLock) :
		ForceFieldComponent::setup();
}

// The common handler for all classes that inherit this virtual member
// function.

void sipForceFieldComponent::sipVH_updateForces(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_ForceFieldComponent,sipName_BALL_updateForces);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

double sipForceFieldComponent::sipVH_updateEnergy(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_ForceFieldComponent,sipName_BALL_updateEnergy);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

double sipForceFieldComponent::sipVH_getEnergy(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_ForceFieldComponent,sipName_BALL_getEnergy);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipForceFieldComponent::sipVH_setup(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
{
	bool res;
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(O)",sipThis -> sipSelf);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		res = (bool)PyInt_AsLong(resobj);

		Py_DECREF(resobj);

		if (PyErr_Occurred() == NULL)
		{
			goto releaseLock;
		}

		sipBadVirtualResultType(sipName_BALL_ForceFieldComponent,sipName_BALL_setup);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

static PyObject *sipDo_ForceFieldComponent_setup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceFieldComponent)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			ForceFieldComponent *ptr;

			if ((ptr = (ForceFieldComponent *)sipGetCppPtr(sipThis,sipClass_ForceFieldComponent)) == NULL)
				return NULL;

			res = ptr -> ForceFieldComponent::setup();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceFieldComponent,sipName_BALL_setup);

	return NULL;
}

static PyObject *sipDo_ForceFieldComponent_getForceField(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceFieldComponent)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			ForceField *res;
			ForceFieldComponent *ptr;

			if ((ptr = (ForceFieldComponent *)sipGetCppPtr(sipThis,sipClass_ForceFieldComponent)) == NULL)
				return NULL;

			res = ptr -> ForceFieldComponent::getForceField();

			return sipMapCppToSelf(res,sipClass_ForceField);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceFieldComponent,sipName_BALL_getForceField);

	return NULL;
}

static PyObject *sipDo_ForceFieldComponent_setForceField(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceFieldComponent)) == NULL)
		return NULL;

	{
		ForceField *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_ForceField,&a0obj))
		{
			ForceFieldComponent *ptr;

			if ((ptr = (ForceFieldComponent *)sipGetCppPtr(sipThis,sipClass_ForceFieldComponent)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_ForceField(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> ForceFieldComponent::setForceField(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceFieldComponent,sipName_BALL_setForceField);

	return NULL;
}

static PyObject *sipDo_ForceFieldComponent_setName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceFieldComponent)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			ForceFieldComponent *ptr;

			if ((ptr = (ForceFieldComponent *)sipGetCppPtr(sipThis,sipClass_ForceFieldComponent)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> ForceFieldComponent::setName(* a0);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceFieldComponent,sipName_BALL_setName);

	return NULL;
}

static PyObject *sipDo_ForceFieldComponent_getName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceFieldComponent)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			String *res;
			ForceFieldComponent *ptr;

			if ((ptr = (ForceFieldComponent *)sipGetCppPtr(sipThis,sipClass_ForceFieldComponent)) == NULL)
				return NULL;

			res = new String(ptr -> ForceFieldComponent::getName());

			return sipNewCppToSelf(res,sipClass_String,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceFieldComponent,sipName_BALL_getName);

	return NULL;
}

static PyObject *sipDo_ForceFieldComponent_getEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceFieldComponent)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			ForceFieldComponent *ptr;

			if ((ptr = (ForceFieldComponent *)sipGetCppPtr(sipThis,sipClass_ForceFieldComponent)) == NULL)
				return NULL;

			res = ptr -> ForceFieldComponent::getEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceFieldComponent,sipName_BALL_getEnergy);

	return NULL;
}

static PyObject *sipDo_ForceFieldComponent_updateEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceFieldComponent)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			ForceFieldComponent *ptr;

			if ((ptr = (ForceFieldComponent *)sipGetCppPtr(sipThis,sipClass_ForceFieldComponent)) == NULL)
				return NULL;

			res = ptr -> ForceFieldComponent::updateEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceFieldComponent,sipName_BALL_updateEnergy);

	return NULL;
}

static PyObject *sipDo_ForceFieldComponent_updateForces(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceFieldComponent)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			ForceFieldComponent *ptr;

			if ((ptr = (ForceFieldComponent *)sipGetCppPtr(sipThis,sipClass_ForceFieldComponent)) == NULL)
				return NULL;

			ptr -> ForceFieldComponent::updateForces();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceFieldComponent,sipName_BALL_updateForces);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_ForceFieldComponent(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_ForceFieldComponent)
		return ptr;

	return NULL;
}

static void sipDealloc_ForceFieldComponent(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipForceFieldComponent *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (ForceFieldComponent *)sipThis -> u.cppPtr;
			else
				delete (sipForceFieldComponent *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_ForceFieldComponent(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_ForceFieldComponent
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
			sipNew = new sipForceFieldComponent();
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

			sipNew = new sipForceFieldComponent(* a0);
		}
	}

	if (sipNew == NULL)
	{
		const ForceFieldComponent *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_ForceFieldComponent,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_ForceFieldComponent(a0obj,(ForceFieldComponent **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipForceFieldComponent(* a0, (bool)a1);
		}
	}

	if (sipNew == NULL)
	{
		const ForceFieldComponent *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_ForceFieldComponent,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_ForceFieldComponent(a0obj,(ForceFieldComponent **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipForceFieldComponent(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_ForceFieldComponent);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_ForceFieldComponent,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (ForceFieldComponent *)sipNew;
			else
				delete (sipForceFieldComponent *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipForceFieldComponent *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_ForceFieldComponent[] = {
	{sipName_BALL_setup, sipDo_ForceFieldComponent_setup, METH_VARARGS, NULL},
	{sipName_BALL_getForceField, sipDo_ForceFieldComponent_getForceField, METH_VARARGS, NULL},
	{sipName_BALL_setForceField, sipDo_ForceFieldComponent_setForceField, METH_VARARGS, NULL},
	{sipName_BALL_setName, sipDo_ForceFieldComponent_setName, METH_VARARGS, NULL},
	{sipName_BALL_getName, sipDo_ForceFieldComponent_getName, METH_VARARGS, NULL},
	{sipName_BALL_getEnergy, sipDo_ForceFieldComponent_getEnergy, METH_VARARGS, NULL},
	{sipName_BALL_updateEnergy, sipDo_ForceFieldComponent_updateEnergy, METH_VARARGS, NULL},
	{sipName_BALL_updateForces, sipDo_ForceFieldComponent_updateForces, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_ForceFieldComponent(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_ForceFieldComponent);
}

void sipConvertTo_ForceFieldComponent(PyObject *sipPy,ForceFieldComponent **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_ForceFieldComponent);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (ForceFieldComponent *)sipConvertToCpp(sipPy,sipClass_ForceFieldComponent,sipIsErr);
}

ForceFieldComponent *sipForceConvertTo_ForceFieldComponent(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_ForceFieldComponent(valobj))
	{
		ForceFieldComponent *val;

		sipConvertTo_ForceFieldComponent(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_ForceFieldComponent);
	*iserrp = 1;

	return NULL;
}
