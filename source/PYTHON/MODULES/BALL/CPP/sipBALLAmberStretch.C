#include "sipBALLDeclBALL.h"
#include "sipBALLAmberStretch.h"



PyObject *sipClass_AmberStretch;

static void sipDealloc_AmberStretch(sipThisType *);

static PyTypeObject sipType_AmberStretch = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_AmberStretch,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_AmberStretch,
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

sipAmberStretch::sipAmberStretch()
    : AmberStretch()
{
	sipCommonCtor(sipPyMethods,4);
}

sipAmberStretch::sipAmberStretch(ForceField& a0)
    : AmberStretch(a0)
{
	sipCommonCtor(sipPyMethods,4);
}

sipAmberStretch::sipAmberStretch(const AmberStretch& a0,bool a1)
    : AmberStretch(a0,a1)
{
	sipCommonCtor(sipPyMethods,4);
}

sipAmberStretch::sipAmberStretch(const AmberStretch& a0)
    : AmberStretch(a0)
{
	sipCommonCtor(sipPyMethods,4);
}

sipAmberStretch::~sipAmberStretch()
 
{
	sipCommonDtor(sipPyThis);
}

double sipAmberStretch::getEnergy() const

{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_getEnergy,&relLock) ?
		sipForceFieldComponent::sipVH_getEnergy(&sipPyMethods[0],sipPyThis,relLock) :
		ForceFieldComponent::getEnergy();
}

void sipAmberStretch::updateForces()

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_updateForces,&relLock))
		sipForceFieldComponent::sipVH_updateForces(&sipPyMethods[1],sipPyThis,relLock);
	else
		AmberStretch::updateForces();
}

double sipAmberStretch::updateEnergy()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_updateEnergy,&relLock) ?
		sipForceFieldComponent::sipVH_updateEnergy(&sipPyMethods[2],sipPyThis,relLock) :
		AmberStretch::updateEnergy();
}

bool sipAmberStretch::setup()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_setup,&relLock) ?
		sipForceFieldComponent::sipVH_setup(&sipPyMethods[3],sipPyThis,relLock) :
		AmberStretch::setup();
}

static PyObject *sipDo_AmberStretch_setup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberStretch)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			AmberStretch *ptr;

			if ((ptr = (AmberStretch *)sipGetCppPtr(sipThis,sipClass_AmberStretch)) == NULL)
				return NULL;

			res = ptr -> AmberStretch::setup();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AmberStretch,sipName_BALL_setup);

	return NULL;
}

static PyObject *sipDo_AmberStretch_updateEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberStretch)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			AmberStretch *ptr;

			if ((ptr = (AmberStretch *)sipGetCppPtr(sipThis,sipClass_AmberStretch)) == NULL)
				return NULL;

			res = ptr -> AmberStretch::updateEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AmberStretch,sipName_BALL_updateEnergy);

	return NULL;
}

static PyObject *sipDo_AmberStretch_updateForces(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberStretch)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			AmberStretch *ptr;

			if ((ptr = (AmberStretch *)sipGetCppPtr(sipThis,sipClass_AmberStretch)) == NULL)
				return NULL;

			ptr -> AmberStretch::updateForces();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AmberStretch,sipName_BALL_updateForces);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_AmberStretch(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_AmberStretch)
		return ptr;

	if ((res = sipCast_ForceFieldComponent((ForceFieldComponent *)(AmberStretch *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_AmberStretch(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipAmberStretch *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (AmberStretch *)sipThis -> u.cppPtr;
			else
				delete (sipAmberStretch *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_AmberStretch(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_AmberStretch
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
			sipNew = new sipAmberStretch();
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

			sipNew = new sipAmberStretch(* a0);
		}
	}

	if (sipNew == NULL)
	{
		const AmberStretch *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_AmberStretch,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_AmberStretch(a0obj,(AmberStretch **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipAmberStretch(* a0, (bool)a1);
		}
	}

	if (sipNew == NULL)
	{
		const AmberStretch *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_AmberStretch,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_AmberStretch(a0obj,(AmberStretch **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipAmberStretch(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_AmberStretch);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_AmberStretch,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (AmberStretch *)sipNew;
			else
				delete (sipAmberStretch *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipAmberStretch *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_AmberStretch[] = {
	{sipName_BALL_setup, sipDo_AmberStretch_setup, METH_VARARGS, NULL},
	{sipName_BALL_updateEnergy, sipDo_AmberStretch_updateEnergy, METH_VARARGS, NULL},
	{sipName_BALL_updateForces, sipDo_AmberStretch_updateForces, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_AmberStretch(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_AmberStretch);
}

void sipConvertTo_AmberStretch(PyObject *sipPy,AmberStretch **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_AmberStretch);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (AmberStretch *)sipConvertToCpp(sipPy,sipClass_AmberStretch,sipIsErr);
}

AmberStretch *sipForceConvertTo_AmberStretch(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_AmberStretch(valobj))
	{
		AmberStretch *val;

		sipConvertTo_AmberStretch(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_AmberStretch);
	*iserrp = 1;

	return NULL;
}
