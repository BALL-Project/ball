#include "sipBALLDeclBALL.h"
#include "sipBALLAmberTorsion.h"


PyObject *sipClass_AmberTorsion;

static void sipDealloc_AmberTorsion(sipThisType *);

static PyTypeObject sipType_AmberTorsion = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_AmberTorsion,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_AmberTorsion,
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

sipAmberTorsion::sipAmberTorsion()
   : AmberTorsion()
{
	sipCommonCtor(sipPyMethods,4);
}

sipAmberTorsion::sipAmberTorsion(ForceField& a0)
   : AmberTorsion(a0)
{
	sipCommonCtor(sipPyMethods,4);
}

sipAmberTorsion::sipAmberTorsion(const AmberTorsion& a0,bool a1)
   : AmberTorsion(a0,a1)
{
	sipCommonCtor(sipPyMethods,4);
}

sipAmberTorsion::sipAmberTorsion(const AmberTorsion& a0)
   : AmberTorsion(a0)
{
	sipCommonCtor(sipPyMethods,4);
}

sipAmberTorsion::~sipAmberTorsion()

{
	sipCommonDtor(sipPyThis);
}

double sipAmberTorsion::getEnergy() const

{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_getEnergy,&relLock) ?
		sipForceFieldComponent::sipVH_getEnergy(&sipPyMethods[0],sipPyThis,relLock) :
		ForceFieldComponent::getEnergy();
}

void sipAmberTorsion::updateForces()

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_updateForces,&relLock))
		sipForceFieldComponent::sipVH_updateForces(&sipPyMethods[1],sipPyThis,relLock);
	else
		AmberTorsion::updateForces();
}

double sipAmberTorsion::updateEnergy()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_updateEnergy,&relLock) ?
		sipForceFieldComponent::sipVH_updateEnergy(&sipPyMethods[2],sipPyThis,relLock) :
		AmberTorsion::updateEnergy();
}

bool sipAmberTorsion::setup()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_setup,&relLock) ?
		sipForceFieldComponent::sipVH_setup(&sipPyMethods[3],sipPyThis,relLock) :
		AmberTorsion::setup();
}

extern "C" PyObject *sipDo_AmberTorsion_setup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberTorsion)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			AmberTorsion *ptr;

			if ((ptr = (AmberTorsion *)sipGetCppPtr(sipThis,sipClass_AmberTorsion)) == NULL)
				return NULL;

   try
   {
			res = ptr -> AmberTorsion::setup();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AmberTorsion,sipName_BALL_setup);

	return NULL;
}

extern "C" PyObject *sipDo_AmberTorsion_updateEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberTorsion)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			AmberTorsion *ptr;

			if ((ptr = (AmberTorsion *)sipGetCppPtr(sipThis,sipClass_AmberTorsion)) == NULL)
				return NULL;

   try
   {
			res = ptr -> AmberTorsion::updateEnergy();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AmberTorsion,sipName_BALL_updateEnergy);

	return NULL;
}

extern "C" PyObject *sipDo_AmberTorsion_updateForces(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AmberTorsion)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			AmberTorsion *ptr;

			if ((ptr = (AmberTorsion *)sipGetCppPtr(sipThis,sipClass_AmberTorsion)) == NULL)
				return NULL;

   try
   {
			ptr -> AmberTorsion::updateForces();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AmberTorsion,sipName_BALL_updateForces);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_AmberTorsion(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_AmberTorsion)
		return ptr;

	if ((res = sipCast_ForceFieldComponent((ForceFieldComponent *)(AmberTorsion *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_AmberTorsion(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipAmberTorsion *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (AmberTorsion *)sipThis -> u.cppPtr;
			else
				delete (sipAmberTorsion *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_AmberTorsion(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_AmberTorsion
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
			sipNew = new sipAmberTorsion();
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
			sipNew = new sipAmberTorsion(* a0);
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
		const AmberTorsion * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_AmberTorsion,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_AmberTorsion(a0obj,(AmberTorsion **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipAmberTorsion(* a0, (bool)a1);
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
		const AmberTorsion * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_AmberTorsion,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_AmberTorsion(a0obj,(AmberTorsion **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipAmberTorsion(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_AmberTorsion);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_AmberTorsion,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (AmberTorsion *)sipNew;
			else
				delete (sipAmberTorsion *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipAmberTorsion *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_AmberTorsion[] = {
	{sipName_BALL_setup, sipDo_AmberTorsion_setup, METH_VARARGS, NULL},
	{sipName_BALL_updateEnergy, sipDo_AmberTorsion_updateEnergy, METH_VARARGS, NULL},
	{sipName_BALL_updateForces, sipDo_AmberTorsion_updateForces, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_AmberTorsion(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_AmberTorsion);
}

void sipConvertTo_AmberTorsion(PyObject *sipPy,AmberTorsion **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_AmberTorsion);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (AmberTorsion *)sipConvertToCpp(sipPy,sipClass_AmberTorsion,sipIsErr);
}

AmberTorsion *sipForceConvertTo_AmberTorsion(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_AmberTorsion(valobj))
	{
		AmberTorsion *val;

		sipConvertTo_AmberTorsion(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_AmberTorsion);

	*iserrp = 1;

	return NULL;
}
