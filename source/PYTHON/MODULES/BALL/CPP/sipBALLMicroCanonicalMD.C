#include "sipBALLDeclBALL.h"
#include "sipBALLMicroCanonicalMD.h"


PyObject *sipClass_MicroCanonicalMD;

static void sipDealloc_MicroCanonicalMD(sipThisType *);

static PyTypeObject sipType_MicroCanonicalMD = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_MicroCanonicalMD,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_MicroCanonicalMD,
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

sipMicroCanonicalMD::sipMicroCanonicalMD()
   : MicroCanonicalMD()
{
	sipCommonCtor(sipPyMethods,7);
}

sipMicroCanonicalMD::sipMicroCanonicalMD(ForceField& a0)
   : MicroCanonicalMD(a0)
{
	sipCommonCtor(sipPyMethods,7);
}

sipMicroCanonicalMD::sipMicroCanonicalMD(ForceField& a0,SnapShotManager * a1)
   : MicroCanonicalMD(a0,a1)
{
	sipCommonCtor(sipPyMethods,7);
}

sipMicroCanonicalMD::sipMicroCanonicalMD(ForceField& a0,SnapShotManager * a1,const Options& a2)
   : MicroCanonicalMD(a0,a1,a2)
{
	sipCommonCtor(sipPyMethods,7);
}

sipMicroCanonicalMD::sipMicroCanonicalMD(const MicroCanonicalMD& a0,bool a1)
   : MicroCanonicalMD(a0,a1)
{
	sipCommonCtor(sipPyMethods,7);
}

sipMicroCanonicalMD::sipMicroCanonicalMD(const MicroCanonicalMD& a0)
   : MicroCanonicalMD(a0)
{
	sipCommonCtor(sipPyMethods,7);
}

sipMicroCanonicalMD::~sipMicroCanonicalMD()

{
	sipCommonDtor(sipPyThis);
}

void sipMicroCanonicalMD::simulateTime(double a0,bool a1)

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_simulateTime,&relLock))
		sipMolecularDynamics::sipVH_simulateTime(&sipPyMethods[0],sipPyThis,relLock,a0,a1);
	else
		MicroCanonicalMD::simulateTime(a0,a1);
}

void sipMicroCanonicalMD::simulateIterations(int a0,bool a1)

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_simulateIterations,&relLock))
		sipMolecularDynamics::sipVH_simulateIterations(&sipPyMethods[1],sipPyThis,relLock,a0,a1);
	else
		MicroCanonicalMD::simulateIterations(a0,a1);
}

void sipMicroCanonicalMD::simulate(bool a0)

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_simulate,&relLock))
		sipMolecularDynamics::sipVH_simulate(&sipPyMethods[2],sipPyThis,relLock,a0);
	else
		MicroCanonicalMD::simulate(a0);
}

void sipMicroCanonicalMD::setTimeStep(double a0)

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_setTimeStep,&relLock))
		sipMolecularDynamics::sipVH_setTimeStep(&sipPyMethods[3],sipPyThis,relLock,a0);
	else
		MicroCanonicalMD::setTimeStep(a0);
}

bool sipMicroCanonicalMD::specificSetup()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_specificSetup,&relLock) ?
		sipMolecularDynamics::sipVH_specificSetup(&sipPyMethods[4],sipPyThis,relLock) :
		MicroCanonicalMD::specificSetup();
}

bool sipMicroCanonicalMD::setup(ForceField& a0,SnapShotManager * a1,const Options& a2)

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[5],sipPyThis,NULL,sipName_BALL_setup,&relLock) ?
		sipMolecularDynamics::sipVH_setup(&sipPyMethods[5],sipPyThis,relLock,a0,a1,a2) :
		MicroCanonicalMD::setup(a0,a1,a2);
}

bool sipMicroCanonicalMD::setup(ForceField& a0,SnapShotManager * a1)

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[6],sipPyThis,NULL,sipName_BALL_setup,&relLock) ?
		sipMolecularDynamics::sipVH_setup(&sipPyMethods[6],sipPyThis,relLock,a0,a1) :
		MicroCanonicalMD::setup(a0,a1);
}

static PyObject *sipDo_MicroCanonicalMD_setup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MicroCanonicalMD)) == NULL)
		return NULL;

	{
		ForceField * a0;
		PyObject *a0obj;
		SnapShotManager * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_ForceField,&a0obj,sipCanConvertTo_SnapShotManager,&a1obj))
		{
			bool res;
			MicroCanonicalMD *ptr;

			if ((ptr = (MicroCanonicalMD *)sipGetCppPtr(sipThis,sipClass_MicroCanonicalMD)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_ForceField(a0obj,&a0,1,&iserr);
			sipConvertTo_SnapShotManager(a1obj,&a1,0,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> MicroCanonicalMD::setup(* a0, a1);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

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
			MicroCanonicalMD *ptr;

			if ((ptr = (MicroCanonicalMD *)sipGetCppPtr(sipThis,sipClass_MicroCanonicalMD)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_ForceField(a0obj,&a0,1,&iserr);
			sipConvertTo_SnapShotManager(a1obj,&a1,0,&iserr);
			sipConvertTo_Options(a2obj,(Options **)&a2,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> MicroCanonicalMD::setup(* a0, a1,* a2);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_MicroCanonicalMD,sipName_BALL_setup);

	return NULL;
}

static PyObject *sipDo_MicroCanonicalMD_specificSetup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MicroCanonicalMD)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			MicroCanonicalMD *ptr;

			if ((ptr = (MicroCanonicalMD *)sipGetCppPtr(sipThis,sipClass_MicroCanonicalMD)) == NULL)
				return NULL;

   try
   {
			res = ptr -> MicroCanonicalMD::specificSetup();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_MicroCanonicalMD,sipName_BALL_specificSetup);

	return NULL;
}

static PyObject *sipDo_MicroCanonicalMD_setTimeStep(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MicroCanonicalMD)) == NULL)
		return NULL;

	{
		double a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"d",&a0))
		{
			MicroCanonicalMD *ptr;

			if ((ptr = (MicroCanonicalMD *)sipGetCppPtr(sipThis,sipClass_MicroCanonicalMD)) == NULL)
				return NULL;

   try
   {
			ptr -> MicroCanonicalMD::setTimeStep( a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_MicroCanonicalMD,sipName_BALL_setTimeStep);

	return NULL;
}

static PyObject *sipDo_MicroCanonicalMD_simulate(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MicroCanonicalMD)) == NULL)
		return NULL;

	{
		long a0 = false;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"|l",&a0))
		{
			MicroCanonicalMD *ptr;

			if ((ptr = (MicroCanonicalMD *)sipGetCppPtr(sipThis,sipClass_MicroCanonicalMD)) == NULL)
				return NULL;

   try
   {
			ptr -> MicroCanonicalMD::simulate( (bool)a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_MicroCanonicalMD,sipName_BALL_simulate);

	return NULL;
}

static PyObject *sipDo_MicroCanonicalMD_simulateIterations(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MicroCanonicalMD)) == NULL)
		return NULL;

	{
		int a0;
		long a1 = false;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"i|l",&a0,&a1))
		{
			MicroCanonicalMD *ptr;

			if ((ptr = (MicroCanonicalMD *)sipGetCppPtr(sipThis,sipClass_MicroCanonicalMD)) == NULL)
				return NULL;

   try
   {
			ptr -> MicroCanonicalMD::simulateIterations( a0, (bool)a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_MicroCanonicalMD,sipName_BALL_simulateIterations);

	return NULL;
}

static PyObject *sipDo_MicroCanonicalMD_simulateTime(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MicroCanonicalMD)) == NULL)
		return NULL;

	{
		double a0;
		long a1 = false;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"d|l",&a0,&a1))
		{
			MicroCanonicalMD *ptr;

			if ((ptr = (MicroCanonicalMD *)sipGetCppPtr(sipThis,sipClass_MicroCanonicalMD)) == NULL)
				return NULL;

   try
   {
			ptr -> MicroCanonicalMD::simulateTime( a0, (bool)a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_MicroCanonicalMD,sipName_BALL_simulateTime);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_MicroCanonicalMD(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_MicroCanonicalMD)
		return ptr;

	if ((res = sipCast_MolecularDynamics((MolecularDynamics *)(MicroCanonicalMD *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_MicroCanonicalMD(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipMicroCanonicalMD *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (MicroCanonicalMD *)sipThis -> u.cppPtr;
			else
				delete (sipMicroCanonicalMD *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_MicroCanonicalMD(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_MicroCanonicalMD
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
			sipNew = new sipMicroCanonicalMD();
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
			sipNew = new sipMicroCanonicalMD(* a0);
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
			sipNew = new sipMicroCanonicalMD(* a0, a1);
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
			sipNew = new sipMicroCanonicalMD(* a0, a1,* a2);
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
		const MicroCanonicalMD * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_MicroCanonicalMD,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_MicroCanonicalMD(a0obj,(MicroCanonicalMD **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipMicroCanonicalMD(* a0, (bool)a1);
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
		const MicroCanonicalMD * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_MicroCanonicalMD,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_MicroCanonicalMD(a0obj,(MicroCanonicalMD **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipMicroCanonicalMD(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_MicroCanonicalMD);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_MicroCanonicalMD,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (MicroCanonicalMD *)sipNew;
			else
				delete (sipMicroCanonicalMD *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipMicroCanonicalMD *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_MicroCanonicalMD[] = {
	{sipName_BALL_setup, sipDo_MicroCanonicalMD_setup, METH_VARARGS, NULL},
	{sipName_BALL_specificSetup, sipDo_MicroCanonicalMD_specificSetup, METH_VARARGS, NULL},
	{sipName_BALL_setTimeStep, sipDo_MicroCanonicalMD_setTimeStep, METH_VARARGS, NULL},
	{sipName_BALL_simulate, sipDo_MicroCanonicalMD_simulate, METH_VARARGS, NULL},
	{sipName_BALL_simulateIterations, sipDo_MicroCanonicalMD_simulateIterations, METH_VARARGS, NULL},
	{sipName_BALL_simulateTime, sipDo_MicroCanonicalMD_simulateTime, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_MicroCanonicalMD(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_MicroCanonicalMD);
}

void sipConvertTo_MicroCanonicalMD(PyObject *sipPy,MicroCanonicalMD **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_MicroCanonicalMD);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (MicroCanonicalMD *)sipConvertToCpp(sipPy,sipClass_MicroCanonicalMD,sipIsErr);
}

MicroCanonicalMD *sipForceConvertTo_MicroCanonicalMD(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_MicroCanonicalMD(valobj))
	{
		MicroCanonicalMD *val;

		sipConvertTo_MicroCanonicalMD(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_MicroCanonicalMD);

	*iserrp = 1;

	return NULL;
}
