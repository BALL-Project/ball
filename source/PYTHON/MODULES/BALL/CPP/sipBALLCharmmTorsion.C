#include "sipBALLDeclBALL.h"
#include "sipBALLCharmmTorsion.h"



PyObject *sipClass_CharmmTorsion;

static void sipDealloc_CharmmTorsion(sipThisType *);

static PyTypeObject sipType_CharmmTorsion = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_CharmmTorsion,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_CharmmTorsion,
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

sipCharmmTorsion::sipCharmmTorsion()
    : CharmmTorsion()
{
	sipCommonCtor(sipPyMethods,4);
}

sipCharmmTorsion::sipCharmmTorsion(ForceField& a0)
    : CharmmTorsion(a0)
{
	sipCommonCtor(sipPyMethods,4);
}

sipCharmmTorsion::sipCharmmTorsion(const CharmmTorsion& a0,bool a1)
    : CharmmTorsion(a0,a1)
{
	sipCommonCtor(sipPyMethods,4);
}

sipCharmmTorsion::sipCharmmTorsion(const CharmmTorsion& a0)
    : CharmmTorsion(a0)
{
	sipCommonCtor(sipPyMethods,4);
}

sipCharmmTorsion::~sipCharmmTorsion()
 
{
	sipCommonDtor(sipPyThis);
}

double sipCharmmTorsion::getEnergy() const

{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_getEnergy,&relLock) ?
		sipForceFieldComponent::sipVH_getEnergy(&sipPyMethods[0],sipPyThis,relLock) :
		ForceFieldComponent::getEnergy();
}

void sipCharmmTorsion::updateForces()

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_updateForces,&relLock))
		sipForceFieldComponent::sipVH_updateForces(&sipPyMethods[1],sipPyThis,relLock);
	else
		CharmmTorsion::updateForces();
}

double sipCharmmTorsion::updateEnergy()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_updateEnergy,&relLock) ?
		sipForceFieldComponent::sipVH_updateEnergy(&sipPyMethods[2],sipPyThis,relLock) :
		CharmmTorsion::updateEnergy();
}

bool sipCharmmTorsion::setup()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_setup,&relLock) ?
		sipForceFieldComponent::sipVH_setup(&sipPyMethods[3],sipPyThis,relLock) :
		CharmmTorsion::setup();
}

static PyObject *sipDo_CharmmTorsion_setup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmTorsion)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			CharmmTorsion *ptr;

			if ((ptr = (CharmmTorsion *)sipGetCppPtr(sipThis,sipClass_CharmmTorsion)) == NULL)
				return NULL;

			res = ptr -> CharmmTorsion::setup();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmTorsion,sipName_BALL_setup);

	return NULL;
}

static PyObject *sipDo_CharmmTorsion_updateEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmTorsion)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			CharmmTorsion *ptr;

			if ((ptr = (CharmmTorsion *)sipGetCppPtr(sipThis,sipClass_CharmmTorsion)) == NULL)
				return NULL;

			res = ptr -> CharmmTorsion::updateEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmTorsion,sipName_BALL_updateEnergy);

	return NULL;
}

static PyObject *sipDo_CharmmTorsion_updateForces(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmTorsion)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			CharmmTorsion *ptr;

			if ((ptr = (CharmmTorsion *)sipGetCppPtr(sipThis,sipClass_CharmmTorsion)) == NULL)
				return NULL;

			ptr -> CharmmTorsion::updateForces();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmTorsion,sipName_BALL_updateForces);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_CharmmTorsion(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_CharmmTorsion)
		return ptr;

	if ((res = sipCast_ForceFieldComponent((ForceFieldComponent *)(CharmmTorsion *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_CharmmTorsion(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipCharmmTorsion *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (CharmmTorsion *)sipThis -> u.cppPtr;
			else
				delete (sipCharmmTorsion *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_CharmmTorsion(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_CharmmTorsion
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
			sipNew = new sipCharmmTorsion();
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

			sipNew = new sipCharmmTorsion(* a0);
		}
	}

	if (sipNew == NULL)
	{
		const CharmmTorsion *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_CharmmTorsion,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_CharmmTorsion(a0obj,(CharmmTorsion **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipCharmmTorsion(* a0, (bool)a1);
		}
	}

	if (sipNew == NULL)
	{
		const CharmmTorsion *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_CharmmTorsion,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_CharmmTorsion(a0obj,(CharmmTorsion **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipCharmmTorsion(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_CharmmTorsion);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_CharmmTorsion,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (CharmmTorsion *)sipNew;
			else
				delete (sipCharmmTorsion *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipCharmmTorsion *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_CharmmTorsion[] = {
	{sipName_BALL_setup, sipDo_CharmmTorsion_setup, METH_VARARGS, NULL},
	{sipName_BALL_updateEnergy, sipDo_CharmmTorsion_updateEnergy, METH_VARARGS, NULL},
	{sipName_BALL_updateForces, sipDo_CharmmTorsion_updateForces, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_CharmmTorsion(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_CharmmTorsion);
}

void sipConvertTo_CharmmTorsion(PyObject *sipPy,CharmmTorsion **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_CharmmTorsion);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (CharmmTorsion *)sipConvertToCpp(sipPy,sipClass_CharmmTorsion,sipIsErr);
}

CharmmTorsion *sipForceConvertTo_CharmmTorsion(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_CharmmTorsion(valobj))
	{
		CharmmTorsion *val;

		sipConvertTo_CharmmTorsion(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_CharmmTorsion);
	*iserrp = 1;

	return NULL;
}
