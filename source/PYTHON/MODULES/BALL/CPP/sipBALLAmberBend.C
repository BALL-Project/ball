#include "sipBALLDeclBALL.h"
#include "sipBALLAmberBend.h"



PyObject *sipClass_AmberBend;

static void sipDealloc_AmberBend(sipThisType *);

static PyTypeObject sipType_AmberBend = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_AmberBend,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_AmberBend,
	0,
	0,
	0,
	0,
	0,
};

sipAmberBend::sipAmberBend(): AmberBend()
{
	sipCommonCtor(sipPyMethods,4);
}

sipAmberBend::sipAmberBend(ForceField& a0): AmberBend(a0)
{
	sipCommonCtor(sipPyMethods,4);
}

sipAmberBend::sipAmberBend(const AmberBend& a0,bool a1): AmberBend(a0,a1)
{
	sipCommonCtor(sipPyMethods,4);
}

sipAmberBend::sipAmberBend(const AmberBend& a0): AmberBend(a0)
{
	sipCommonCtor(sipPyMethods,4);
}

sipAmberBend::~sipAmberBend()
{
	sipCommonDtor(sipPyThis);
}
double sipAmberBend::getEnergy() const
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_getEnergy,&relLock) ?
		sipForceFieldComponent::sipVH_getEnergy(&sipPyMethods[0],sipPyThis,relLock) :
		ForceFieldComponent::getEnergy();
}
bool sipAmberBend::setup()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_setup,&relLock) ?
		sipForceFieldComponent::sipVH_setup(&sipPyMethods[1],sipPyThis,relLock) :
		AmberBend::setup();
}
double sipAmberBend::updateEnergy()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_updateEnergy,&relLock) ?
		sipForceFieldComponent::sipVH_updateEnergy(&sipPyMethods[2],sipPyThis,relLock) :
		AmberBend::updateEnergy();
}
void sipAmberBend::updateForces()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_updateForces,&relLock))
		sipForceFieldComponent::sipVH_updateForces(&sipPyMethods[3],sipPyThis,relLock);
	else
		AmberBend::updateForces();
}

static PyObject *sipDo_AmberBend_setup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberBend)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			AmberBend *ptr;

			if ((ptr = (AmberBend *)sipGetCppPtr(sipThis,sipClass_AmberBend)) == NULL)
				return NULL;

			res = ptr -> AmberBend::setup();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_AmberBend,sipName_BALL_setup);

	return NULL;
}

static PyObject *sipDo_AmberBend_updateEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberBend)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			double res;
			AmberBend *ptr;

			if ((ptr = (AmberBend *)sipGetCppPtr(sipThis,sipClass_AmberBend)) == NULL)
				return NULL;

			res = ptr -> AmberBend::updateEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_AmberBend,sipName_BALL_updateEnergy);

	return NULL;
}

static PyObject *sipDo_AmberBend_updateForces(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberBend)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			AmberBend *ptr;

			if ((ptr = (AmberBend *)sipGetCppPtr(sipThis,sipClass_AmberBend)) == NULL)
				return NULL;

			ptr -> AmberBend::updateForces();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_AmberBend,sipName_BALL_updateForces);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_AmberBend(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_AmberBend)
		return ptr;

	if ((res = sipCast_ForceFieldComponent((ForceFieldComponent *)(AmberBend *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_AmberBend(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipAmberBend *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (AmberBend *)sipThis -> u.cppPtr;
			else
				delete (sipAmberBend *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_AmberBend(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_AmberBend
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
			sipNew = new sipAmberBend();
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

			sipNew = new sipAmberBend(* a0);
	}
	}

	if (sipNew == NULL)
	{
		const AmberBend *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_AmberBend,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_AmberBend(a0obj,(AmberBend **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipAmberBend(* a0, (bool)a1);
	}
	}

	if (sipNew == NULL)
	{
		const AmberBend *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_AmberBend,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_AmberBend(a0obj,(AmberBend **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipAmberBend(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_AmberBend);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_AmberBend,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (AmberBend *)sipNew;
			else
				delete (sipAmberBend *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipAmberBend *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_AmberBend[] = {
	{sipName_BALL_setup, sipDo_AmberBend_setup, METH_VARARGS, NULL},
	{sipName_BALL_updateEnergy, sipDo_AmberBend_updateEnergy, METH_VARARGS, NULL},
	{sipName_BALL_updateForces, sipDo_AmberBend_updateForces, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_AmberBend(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_AmberBend);
}

void sipConvertTo_AmberBend(PyObject *sipPy,AmberBend **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_AmberBend);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (AmberBend *)sipConvertToCpp(sipPy,sipClass_AmberBend,sipIsErr);
}

AmberBend *sipForceConvertTo_AmberBend(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_AmberBend(valobj))
	{
		AmberBend *val;

		sipConvertTo_AmberBend(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_AmberBend);
	*iserrp = 1;

	return NULL;
}
