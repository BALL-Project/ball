#include "sipBALLDeclBALL.h"
#include "sipBALLCharmmBend.h"


PyObject *sipClass_CharmmBend;

static void sipDealloc_CharmmBend(sipThisType *);

static PyTypeObject sipType_CharmmBend = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_CharmmBend,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_CharmmBend,
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

sipCharmmBend::sipCharmmBend()
   : CharmmBend()
{
	sipCommonCtor(sipPyMethods,4);
}

sipCharmmBend::sipCharmmBend(ForceField& a0)
   : CharmmBend(a0)
{
	sipCommonCtor(sipPyMethods,4);
}

sipCharmmBend::sipCharmmBend(const CharmmBend& a0,bool a1)
   : CharmmBend(a0,a1)
{
	sipCommonCtor(sipPyMethods,4);
}

sipCharmmBend::sipCharmmBend(const CharmmBend& a0)
   : CharmmBend(a0)
{
	sipCommonCtor(sipPyMethods,4);
}

sipCharmmBend::~sipCharmmBend()

{
	sipCommonDtor(sipPyThis);
}

double sipCharmmBend::getEnergy() const

{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_getEnergy,&relLock) ?
		sipForceFieldComponent::sipVH_getEnergy(&sipPyMethods[0],sipPyThis,relLock) :
		ForceFieldComponent::getEnergy();
}

void sipCharmmBend::updateForces()

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_updateForces,&relLock))
		sipForceFieldComponent::sipVH_updateForces(&sipPyMethods[1],sipPyThis,relLock);
	else
		CharmmBend::updateForces();
}

double sipCharmmBend::updateEnergy()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_updateEnergy,&relLock) ?
		sipForceFieldComponent::sipVH_updateEnergy(&sipPyMethods[2],sipPyThis,relLock) :
		CharmmBend::updateEnergy();
}

bool sipCharmmBend::setup()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_setup,&relLock) ?
		sipForceFieldComponent::sipVH_setup(&sipPyMethods[3],sipPyThis,relLock) :
		CharmmBend::setup();
}

extern "C" PyObject *sipDo_CharmmBend_setup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmBend)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			CharmmBend *ptr;

			if ((ptr = (CharmmBend *)sipGetCppPtr(sipThis,sipClass_CharmmBend)) == NULL)
				return NULL;

   try
   {
			res = ptr -> CharmmBend::setup();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmBend,sipName_BALL_setup);

	return NULL;
}

extern "C" PyObject *sipDo_CharmmBend_updateEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmBend)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			CharmmBend *ptr;

			if ((ptr = (CharmmBend *)sipGetCppPtr(sipThis,sipClass_CharmmBend)) == NULL)
				return NULL;

   try
   {
			res = ptr -> CharmmBend::updateEnergy();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmBend,sipName_BALL_updateEnergy);

	return NULL;
}

extern "C" PyObject *sipDo_CharmmBend_updateForces(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CharmmBend)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			CharmmBend *ptr;

			if ((ptr = (CharmmBend *)sipGetCppPtr(sipThis,sipClass_CharmmBend)) == NULL)
				return NULL;

   try
   {
			ptr -> CharmmBend::updateForces();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_CharmmBend,sipName_BALL_updateForces);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_CharmmBend(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_CharmmBend)
		return ptr;

	if ((res = sipCast_ForceFieldComponent((ForceFieldComponent *)(CharmmBend *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_CharmmBend(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipCharmmBend *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (CharmmBend *)sipThis -> u.cppPtr;
			else
				delete (sipCharmmBend *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_CharmmBend(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_CharmmBend
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
			sipNew = new sipCharmmBend();
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
			sipNew = new sipCharmmBend(* a0);
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
		const CharmmBend * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_CharmmBend,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_CharmmBend(a0obj,(CharmmBend **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipCharmmBend(* a0, (bool)a1);
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
		const CharmmBend * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_CharmmBend,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_CharmmBend(a0obj,(CharmmBend **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipCharmmBend(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_CharmmBend);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_CharmmBend,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (CharmmBend *)sipNew;
			else
				delete (sipCharmmBend *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipCharmmBend *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_CharmmBend[] = {
	{sipName_BALL_setup, sipDo_CharmmBend_setup, METH_VARARGS, NULL},
	{sipName_BALL_updateEnergy, sipDo_CharmmBend_updateEnergy, METH_VARARGS, NULL},
	{sipName_BALL_updateForces, sipDo_CharmmBend_updateForces, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_CharmmBend(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_CharmmBend);
}

void sipConvertTo_CharmmBend(PyObject *sipPy,CharmmBend **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_CharmmBend);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (CharmmBend *)sipConvertToCpp(sipPy,sipClass_CharmmBend,sipIsErr);
}

CharmmBend *sipForceConvertTo_CharmmBend(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_CharmmBend(valobj))
	{
		CharmmBend *val;

		sipConvertTo_CharmmBend(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_CharmmBend);

	*iserrp = 1;

	return NULL;
}
