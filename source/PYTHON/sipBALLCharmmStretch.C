#include "sipBALLDeclBALL.h"
#include "sipBALLCharmmStretch.h"



PyObject *sipClass_CharmmStretch;

static void sipDealloc_CharmmStretch(sipThisType *);

static PyTypeObject sipType_CharmmStretch = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_CharmmStretch,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_CharmmStretch,
	0,
	0,
	0,
	0,
	0,
};

sipCharmmStretch::sipCharmmStretch(): CharmmStretch()
{
	sipCommonCtor(sipPyMethods,4);
}

sipCharmmStretch::sipCharmmStretch(ForceField& a0): CharmmStretch(a0)
{
	sipCommonCtor(sipPyMethods,4);
}

sipCharmmStretch::sipCharmmStretch(const CharmmStretch& a0,bool a1): CharmmStretch(a0,a1)
{
	sipCommonCtor(sipPyMethods,4);
}

sipCharmmStretch::sipCharmmStretch(const CharmmStretch& a0): CharmmStretch(a0)
{
	sipCommonCtor(sipPyMethods,4);
}

sipCharmmStretch::~sipCharmmStretch()
{
	sipCommonDtor(sipPyThis);
}
double sipCharmmStretch::getEnergy() const
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_getEnergy,&relLock) ?
		sipForceFieldComponent::sipVH_getEnergy(&sipPyMethods[0],sipPyThis,relLock) :
		ForceFieldComponent::getEnergy();
}
bool sipCharmmStretch::setup()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_setup,&relLock) ?
		sipForceFieldComponent::sipVH_setup(&sipPyMethods[1],sipPyThis,relLock) :
		CharmmStretch::setup();
}
double sipCharmmStretch::updateEnergy()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_updateEnergy,&relLock) ?
		sipForceFieldComponent::sipVH_updateEnergy(&sipPyMethods[2],sipPyThis,relLock) :
		CharmmStretch::updateEnergy();
}
void sipCharmmStretch::updateForces()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_updateForces,&relLock))
		sipForceFieldComponent::sipVH_updateForces(&sipPyMethods[3],sipPyThis,relLock);
	else
		CharmmStretch::updateForces();
}

static PyObject *sipDo_CharmmStretch_setup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmStretch)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			CharmmStretch *ptr;

			if ((ptr = (CharmmStretch *)sipGetCppPtr(sipThis,sipClass_CharmmStretch)) == NULL)
				return NULL;

			res = ptr -> CharmmStretch::setup();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_CharmmStretch,sipName_BALL_setup);

	return NULL;
}

static PyObject *sipDo_CharmmStretch_updateEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmStretch)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			double res;
			CharmmStretch *ptr;

			if ((ptr = (CharmmStretch *)sipGetCppPtr(sipThis,sipClass_CharmmStretch)) == NULL)
				return NULL;

			res = ptr -> CharmmStretch::updateEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_CharmmStretch,sipName_BALL_updateEnergy);

	return NULL;
}

static PyObject *sipDo_CharmmStretch_updateForces(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmStretch)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			CharmmStretch *ptr;

			if ((ptr = (CharmmStretch *)sipGetCppPtr(sipThis,sipClass_CharmmStretch)) == NULL)
				return NULL;

			ptr -> CharmmStretch::updateForces();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_CharmmStretch,sipName_BALL_updateForces);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_CharmmStretch(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_CharmmStretch)
		return ptr;

	if ((res = sipCast_ForceFieldComponent((ForceFieldComponent *)(CharmmStretch *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_CharmmStretch(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipCharmmStretch *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (CharmmStretch *)sipThis -> u.cppPtr;
			else
				delete (sipCharmmStretch *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_CharmmStretch(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_CharmmStretch
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
			sipNew = new sipCharmmStretch();
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

			sipNew = new sipCharmmStretch(* a0);
		}
	}

	if (sipNew == NULL)
	{
		const CharmmStretch *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_CharmmStretch,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_CharmmStretch(a0obj,(CharmmStretch **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipCharmmStretch(* a0, (bool)a1);
		}
	}

	if (sipNew == NULL)
	{
		const CharmmStretch *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_CharmmStretch,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_CharmmStretch(a0obj,(CharmmStretch **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipCharmmStretch(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_CharmmStretch);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_CharmmStretch,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (CharmmStretch *)sipNew;
			else
				delete (sipCharmmStretch *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipCharmmStretch *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_CharmmStretch[] = {
	{sipName_BALL_setup, sipDo_CharmmStretch_setup, METH_VARARGS, NULL},
	{sipName_BALL_updateEnergy, sipDo_CharmmStretch_updateEnergy, METH_VARARGS, NULL},
	{sipName_BALL_updateForces, sipDo_CharmmStretch_updateForces, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_CharmmStretch(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_CharmmStretch);
}

void sipConvertTo_CharmmStretch(PyObject *sipPy,CharmmStretch **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_CharmmStretch);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (CharmmStretch *)sipConvertToCpp(sipPy,sipClass_CharmmStretch,sipIsErr);
}

CharmmStretch *sipForceConvertTo_CharmmStretch(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_CharmmStretch(valobj))
	{
		CharmmStretch *val;

		sipConvertTo_CharmmStretch(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_CharmmStretch);
	*iserrp = 1;

	return NULL;
}
