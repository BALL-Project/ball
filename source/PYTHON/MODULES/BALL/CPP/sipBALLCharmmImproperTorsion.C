#include "sipBALLDeclBALL.h"
#include "sipBALLCharmmImproperTorsion.h"



PyObject *sipClass_CharmmImproperTorsion;

static void sipDealloc_CharmmImproperTorsion(sipThisType *);

static PyTypeObject sipType_CharmmImproperTorsion = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_CharmmImproperTorsion,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_CharmmImproperTorsion,
	0,
	0,
	0,
	0,
	0,
};

sipCharmmImproperTorsion::sipCharmmImproperTorsion(): CharmmImproperTorsion()
{
	sipCommonCtor(sipPyMethods,4);
}

sipCharmmImproperTorsion::sipCharmmImproperTorsion(ForceField& a0): CharmmImproperTorsion(a0)
{
	sipCommonCtor(sipPyMethods,4);
}

sipCharmmImproperTorsion::sipCharmmImproperTorsion(const CharmmImproperTorsion& a0,bool a1): CharmmImproperTorsion(a0,a1)
{
	sipCommonCtor(sipPyMethods,4);
}

sipCharmmImproperTorsion::sipCharmmImproperTorsion(const CharmmImproperTorsion& a0): CharmmImproperTorsion(a0)
{
	sipCommonCtor(sipPyMethods,4);
}

sipCharmmImproperTorsion::~sipCharmmImproperTorsion()
{
	sipCommonDtor(sipPyThis);
}
double sipCharmmImproperTorsion::getEnergy() const
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_getEnergy,&relLock) ?
		sipForceFieldComponent::sipVH_getEnergy(&sipPyMethods[0],sipPyThis,relLock) :
		ForceFieldComponent::getEnergy();
}
bool sipCharmmImproperTorsion::setup()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_setup,&relLock) ?
		sipForceFieldComponent::sipVH_setup(&sipPyMethods[1],sipPyThis,relLock) :
		CharmmImproperTorsion::setup();
}
double sipCharmmImproperTorsion::updateEnergy()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_updateEnergy,&relLock) ?
		sipForceFieldComponent::sipVH_updateEnergy(&sipPyMethods[2],sipPyThis,relLock) :
		CharmmImproperTorsion::updateEnergy();
}
void sipCharmmImproperTorsion::updateForces()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_updateForces,&relLock))
		sipForceFieldComponent::sipVH_updateForces(&sipPyMethods[3],sipPyThis,relLock);
	else
		CharmmImproperTorsion::updateForces();
}

static PyObject *sipDo_CharmmImproperTorsion_setup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmImproperTorsion)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			CharmmImproperTorsion *ptr;

			if ((ptr = (CharmmImproperTorsion *)sipGetCppPtr(sipThis,sipClass_CharmmImproperTorsion)) == NULL)
				return NULL;

			res = ptr -> CharmmImproperTorsion::setup();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_CharmmImproperTorsion,sipName_BALL_setup);

	return NULL;
}

static PyObject *sipDo_CharmmImproperTorsion_updateEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmImproperTorsion)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			double res;
			CharmmImproperTorsion *ptr;

			if ((ptr = (CharmmImproperTorsion *)sipGetCppPtr(sipThis,sipClass_CharmmImproperTorsion)) == NULL)
				return NULL;

			res = ptr -> CharmmImproperTorsion::updateEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_CharmmImproperTorsion,sipName_BALL_updateEnergy);

	return NULL;
}

static PyObject *sipDo_CharmmImproperTorsion_updateForces(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmImproperTorsion)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			CharmmImproperTorsion *ptr;

			if ((ptr = (CharmmImproperTorsion *)sipGetCppPtr(sipThis,sipClass_CharmmImproperTorsion)) == NULL)
				return NULL;

			ptr -> CharmmImproperTorsion::updateForces();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_CharmmImproperTorsion,sipName_BALL_updateForces);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_CharmmImproperTorsion(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_CharmmImproperTorsion)
		return ptr;

	if ((res = sipCast_ForceFieldComponent((ForceFieldComponent *)(CharmmImproperTorsion *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_CharmmImproperTorsion(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipCharmmImproperTorsion *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (CharmmImproperTorsion *)sipThis -> u.cppPtr;
			else
				delete (sipCharmmImproperTorsion *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_CharmmImproperTorsion(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_CharmmImproperTorsion
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
			sipNew = new sipCharmmImproperTorsion();
		}
	}

	if (sipNew == NULL)
	{
		ForceField *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_ForceField,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_ForceField(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipCharmmImproperTorsion(* a0);
		}
	}

	if (sipNew == NULL)
	{
		const CharmmImproperTorsion *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_CharmmImproperTorsion,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_CharmmImproperTorsion(a0obj,(CharmmImproperTorsion **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipCharmmImproperTorsion(* a0, (bool)a1);
		}
	}

	if (sipNew == NULL)
	{
		const CharmmImproperTorsion *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_CharmmImproperTorsion,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_CharmmImproperTorsion(a0obj,(CharmmImproperTorsion **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipCharmmImproperTorsion(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_CharmmImproperTorsion);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_CharmmImproperTorsion,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (CharmmImproperTorsion *)sipNew;
			else
				delete (sipCharmmImproperTorsion *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipCharmmImproperTorsion *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_CharmmImproperTorsion[] = {
	{sipName_BALL_setup, sipDo_CharmmImproperTorsion_setup, METH_VARARGS, NULL},
	{sipName_BALL_updateEnergy, sipDo_CharmmImproperTorsion_updateEnergy, METH_VARARGS, NULL},
	{sipName_BALL_updateForces, sipDo_CharmmImproperTorsion_updateForces, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_CharmmImproperTorsion(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_CharmmImproperTorsion);
}

void sipConvertTo_CharmmImproperTorsion(PyObject *sipPy,CharmmImproperTorsion **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_CharmmImproperTorsion);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (CharmmImproperTorsion *)sipConvertToCpp(sipPy,sipClass_CharmmImproperTorsion,sipIsErr);
}

CharmmImproperTorsion *sipForceConvertTo_CharmmImproperTorsion(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_CharmmImproperTorsion(valobj))
	{
		CharmmImproperTorsion *val;

		sipConvertTo_CharmmImproperTorsion(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_CharmmImproperTorsion);
	*iserrp = 1;

	return NULL;
}
