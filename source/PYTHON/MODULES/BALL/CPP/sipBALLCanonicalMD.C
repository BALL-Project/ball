#include "sipBALLDeclBALL.h"
#include "sipBALLCanonicalMD.h"


PyObject *sipClass_CanonicalMD;

static void sipDealloc_CanonicalMD(sipThisType *);

static PyTypeObject sipType_CanonicalMD = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_CanonicalMD,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_CanonicalMD,
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

sipCanonicalMD::sipCanonicalMD()
   : CanonicalMD()
{
	sipCommonCtor(sipPyMethods,7);
}

sipCanonicalMD::sipCanonicalMD(ForceField& a0)
   : CanonicalMD(a0)
{
	sipCommonCtor(sipPyMethods,7);
}

sipCanonicalMD::sipCanonicalMD(ForceField& a0,SnapShotManager * a1)
   : CanonicalMD(a0,a1)
{
	sipCommonCtor(sipPyMethods,7);
}

sipCanonicalMD::sipCanonicalMD(ForceField& a0,SnapShotManager * a1,const Options& a2)
   : CanonicalMD(a0,a1,a2)
{
	sipCommonCtor(sipPyMethods,7);
}

sipCanonicalMD::sipCanonicalMD(const CanonicalMD& a0,bool a1)
   : CanonicalMD(a0,a1)
{
	sipCommonCtor(sipPyMethods,7);
}

sipCanonicalMD::sipCanonicalMD(const CanonicalMD& a0)
   : CanonicalMD(a0)
{
	sipCommonCtor(sipPyMethods,7);
}

sipCanonicalMD::~sipCanonicalMD()
 throw()
{
	sipCommonDtor(sipPyThis);
}

void sipCanonicalMD::simulateTime(double a0,bool a1)
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_simulateTime,&relLock))
		sipMolecularDynamics::sipVH_simulateTime(&sipPyMethods[0],sipPyThis,relLock,a0,a1);
	else
		CanonicalMD::simulateTime(a0,a1);
}

void sipCanonicalMD::simulateIterations(int a0,bool a1)
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_simulateIterations,&relLock))
		sipMolecularDynamics::sipVH_simulateIterations(&sipPyMethods[1],sipPyThis,relLock,a0,a1);
	else
		CanonicalMD::simulateIterations(a0,a1);
}

void sipCanonicalMD::simulate(bool a0)
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_simulate,&relLock))
		sipMolecularDynamics::sipVH_simulate(&sipPyMethods[2],sipPyThis,relLock,a0);
	else
		CanonicalMD::simulate(a0);
}

void sipCanonicalMD::setTimeStep(double a0)
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_setTimeStep,&relLock))
		sipMolecularDynamics::sipVH_setTimeStep(&sipPyMethods[3],sipPyThis,relLock,a0);
	else
		CanonicalMD::setTimeStep(a0);
}

bool sipCanonicalMD::specificSetup()
 throw()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_specificSetup,&relLock) ?
		sipMolecularDynamics::sipVH_specificSetup(&sipPyMethods[4],sipPyThis,relLock) :
		CanonicalMD::specificSetup();
}

bool sipCanonicalMD::setup(ForceField& a0,SnapShotManager * a1,const Options& a2)
 throw()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[5],sipPyThis,NULL,sipName_BALL_setup,&relLock) ?
		sipMolecularDynamics::sipVH_setup(&sipPyMethods[5],sipPyThis,relLock,a0,a1,a2) :
		CanonicalMD::setup(a0,a1,a2);
}

bool sipCanonicalMD::setup(ForceField& a0,SnapShotManager * a1)
 throw()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[6],sipPyThis,NULL,sipName_BALL_setup,&relLock) ?
		sipMolecularDynamics::sipVH_setup(&sipPyMethods[6],sipPyThis,relLock,a0,a1) :
		CanonicalMD::setup(a0,a1);
}

static PyObject *sipDo_CanonicalMD_setBathRelaxationTime(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CanonicalMD)) == NULL)
		return NULL;

	{
		double a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"d",&a0))
		{
			CanonicalMD *ptr;

			if ((ptr = (CanonicalMD *)sipGetCppPtr(sipThis,sipClass_CanonicalMD)) == NULL)
				return NULL;

   try
   {
			ptr -> CanonicalMD::setBathRelaxationTime( a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_CanonicalMD,sipName_BALL_setBathRelaxationTime);

	return NULL;
}

static PyObject *sipDo_CanonicalMD_getBathRelaxationTime(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CanonicalMD)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			CanonicalMD *ptr;

			if ((ptr = (CanonicalMD *)sipGetCppPtr(sipThis,sipClass_CanonicalMD)) == NULL)
				return NULL;

   try
   {
			res = ptr -> CanonicalMD::getBathRelaxationTime();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_CanonicalMD,sipName_BALL_getBathRelaxationTime);

	return NULL;
}

static PyObject *sipDo_CanonicalMD_setup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CanonicalMD)) == NULL)
		return NULL;

	{
		ForceField * a0;
		PyObject *a0obj;
		SnapShotManager * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_ForceField,&a0obj,sipCanConvertTo_SnapShotManager,&a1obj))
		{
			bool res;
			CanonicalMD *ptr;

			if ((ptr = (CanonicalMD *)sipGetCppPtr(sipThis,sipClass_CanonicalMD)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_ForceField(a0obj,&a0,1,&iserr);
			sipConvertTo_SnapShotManager(a1obj,&a1,0,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> CanonicalMD::setup(* a0, a1);

			return sipConvertFromBool((int)res);
		}
	}

	{
		ForceField * a0;
		PyObject *a0obj;
		SnapShotManager * a1;
		PyObject *a1obj;
		const Options * a2;
		PyObject *a2obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"III",sipCanConvertTo_ForceField,&a0obj,sipCanConvertTo_SnapShotManager,&a1obj,sipCanConvertTo_Options,&a2obj))
		{
			bool res;
			CanonicalMD *ptr;

			if ((ptr = (CanonicalMD *)sipGetCppPtr(sipThis,sipClass_CanonicalMD)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_ForceField(a0obj,&a0,1,&iserr);
			sipConvertTo_SnapShotManager(a1obj,&a1,0,&iserr);
			sipConvertTo_Options(a2obj,(Options **)&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> CanonicalMD::setup(* a0, a1,* a2);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CanonicalMD,sipName_BALL_setup);

	return NULL;
}

static PyObject *sipDo_CanonicalMD_specificSetup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CanonicalMD)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			CanonicalMD *ptr;

			if ((ptr = (CanonicalMD *)sipGetCppPtr(sipThis,sipClass_CanonicalMD)) == NULL)
				return NULL;

			res = ptr -> CanonicalMD::specificSetup();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CanonicalMD,sipName_BALL_specificSetup);

	return NULL;
}

static PyObject *sipDo_CanonicalMD_setTimeStep(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CanonicalMD)) == NULL)
		return NULL;

	{
		double a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"d",&a0))
		{
			CanonicalMD *ptr;

			if ((ptr = (CanonicalMD *)sipGetCppPtr(sipThis,sipClass_CanonicalMD)) == NULL)
				return NULL;

			ptr -> CanonicalMD::setTimeStep( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CanonicalMD,sipName_BALL_setTimeStep);

	return NULL;
}

static PyObject *sipDo_CanonicalMD_simulate(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CanonicalMD)) == NULL)
		return NULL;

	{
		long a0 = false;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"|l",&a0))
		{
			CanonicalMD *ptr;

			if ((ptr = (CanonicalMD *)sipGetCppPtr(sipThis,sipClass_CanonicalMD)) == NULL)
				return NULL;

			ptr -> CanonicalMD::simulate( (bool)a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CanonicalMD,sipName_BALL_simulate);

	return NULL;
}

static PyObject *sipDo_CanonicalMD_simulateIterations(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CanonicalMD)) == NULL)
		return NULL;

	{
		int a0;
		long a1 = false;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"i|l",&a0,&a1))
		{
			CanonicalMD *ptr;

			if ((ptr = (CanonicalMD *)sipGetCppPtr(sipThis,sipClass_CanonicalMD)) == NULL)
				return NULL;

			ptr -> CanonicalMD::simulateIterations( a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CanonicalMD,sipName_BALL_simulateIterations);

	return NULL;
}

static PyObject *sipDo_CanonicalMD_simulateTime(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CanonicalMD)) == NULL)
		return NULL;

	{
		double a0;
		long a1 = false;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"d|l",&a0,&a1))
		{
			CanonicalMD *ptr;

			if ((ptr = (CanonicalMD *)sipGetCppPtr(sipThis,sipClass_CanonicalMD)) == NULL)
				return NULL;

			ptr -> CanonicalMD::simulateTime( a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_CanonicalMD,sipName_BALL_simulateTime);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_CanonicalMD(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_CanonicalMD)
		return ptr;

	if ((res = sipCast_MolecularDynamics((MolecularDynamics *)(CanonicalMD *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_CanonicalMD(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipCanonicalMD *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (CanonicalMD *)sipThis -> u.cppPtr;
			else
				delete (sipCanonicalMD *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_CanonicalMD(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_CanonicalMD
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
			sipNew = new sipCanonicalMD();
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
			sipNew = new sipCanonicalMD(* a0);
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
		SnapShotManager * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-II",sipCanConvertTo_ForceField,&a0obj,sipCanConvertTo_SnapShotManager,&a1obj))
		{
			int iserr = 0;

			sipConvertTo_ForceField(a0obj,&a0,1,&iserr);
			sipConvertTo_SnapShotManager(a1obj,&a1,0,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipCanonicalMD(* a0, a1);
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
		SnapShotManager * a1;
		PyObject *a1obj;
		const Options * a2;
		PyObject *a2obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-III",sipCanConvertTo_ForceField,&a0obj,sipCanConvertTo_SnapShotManager,&a1obj,sipCanConvertTo_Options,&a2obj))
		{
			int iserr = 0;

			sipConvertTo_ForceField(a0obj,&a0,1,&iserr);
			sipConvertTo_SnapShotManager(a1obj,&a1,0,&iserr);
			sipConvertTo_Options(a2obj,(Options **)&a2,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipCanonicalMD(* a0, a1,* a2);
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
		const CanonicalMD * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_CanonicalMD,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_CanonicalMD(a0obj,(CanonicalMD **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipCanonicalMD(* a0, (bool)a1);
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
		const CanonicalMD * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_CanonicalMD,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_CanonicalMD(a0obj,(CanonicalMD **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipCanonicalMD(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_CanonicalMD);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_CanonicalMD,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (CanonicalMD *)sipNew;
			else
				delete (sipCanonicalMD *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipCanonicalMD *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_CanonicalMD[] = {
	{sipName_BALL_setBathRelaxationTime, sipDo_CanonicalMD_setBathRelaxationTime, METH_VARARGS, NULL},
	{sipName_BALL_getBathRelaxationTime, sipDo_CanonicalMD_getBathRelaxationTime, METH_VARARGS, NULL},
	{sipName_BALL_setup, sipDo_CanonicalMD_setup, METH_VARARGS, NULL},
	{sipName_BALL_specificSetup, sipDo_CanonicalMD_specificSetup, METH_VARARGS, NULL},
	{sipName_BALL_setTimeStep, sipDo_CanonicalMD_setTimeStep, METH_VARARGS, NULL},
	{sipName_BALL_simulate, sipDo_CanonicalMD_simulate, METH_VARARGS, NULL},
	{sipName_BALL_simulateIterations, sipDo_CanonicalMD_simulateIterations, METH_VARARGS, NULL},
	{sipName_BALL_simulateTime, sipDo_CanonicalMD_simulateTime, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_CanonicalMD(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_CanonicalMD);
}

void sipConvertTo_CanonicalMD(PyObject *sipPy,CanonicalMD **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_CanonicalMD);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (CanonicalMD *)sipConvertToCpp(sipPy,sipClass_CanonicalMD,sipIsErr);
}

CanonicalMD *sipForceConvertTo_CanonicalMD(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_CanonicalMD(valobj))
	{
		CanonicalMD *val;

		sipConvertTo_CanonicalMD(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_CanonicalMD);

	*iserrp = 1;

	return NULL;
}
