#include "sipBALLDeclBALL.h"
#include "sipBALLMolecularDynamics.h"



PyObject *sipClass_MolecularDynamics;

static void sipDealloc_MolecularDynamics(sipThisType *);

static PyTypeObject sipType_MolecularDynamics = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_MolecularDynamics,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_MolecularDynamics,
	0,
	0,
	0,
	0,
	0,
};

sipMolecularDynamics::sipMolecularDynamics(): MolecularDynamics()
{
	sipCommonCtor(sipPyMethods,6);
}

sipMolecularDynamics::sipMolecularDynamics(ForceField& a0): MolecularDynamics(a0)
{
	sipCommonCtor(sipPyMethods,6);
}

sipMolecularDynamics::sipMolecularDynamics(const MolecularDynamics& a0,bool a1): MolecularDynamics(a0,a1)
{
	sipCommonCtor(sipPyMethods,6);
}

sipMolecularDynamics::sipMolecularDynamics(const MolecularDynamics& a0): MolecularDynamics(a0)
{
	sipCommonCtor(sipPyMethods,6);
}

sipMolecularDynamics::~sipMolecularDynamics()
{
	sipCommonDtor(sipPyThis);
}
bool sipMolecularDynamics::setup(ForceField& a0,SnapShotManager * a1,const Options& a2)
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_setup,&relLock) ?
		sipMolecularDynamics::sipVH_setup(&sipPyMethods[0],sipPyThis,relLock,a0,a1,a2) :
		MolecularDynamics::setup(a0,a1,a2);
}
bool sipMolecularDynamics::setup(ForceField& a0,SnapShotManager * a1)
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_setup,&relLock) ?
		sipMolecularDynamics::sipVH_setup(&sipPyMethods[0],sipPyThis,relLock,a0,a1) :
		MolecularDynamics::setup(a0,a1);
}
bool sipMolecularDynamics::specificSetup()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_specificSetup,&relLock) ?
		sipMolecularDynamics::sipVH_specificSetup(&sipPyMethods[1],sipPyThis,relLock) :
		MolecularDynamics::specificSetup();
}
void sipMolecularDynamics::setTimeStep(double a0)
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_setTimeStep,&relLock))
		sipMolecularDynamics::sipVH_setTimeStep(&sipPyMethods[2],sipPyThis,relLock,a0);
	else
		MolecularDynamics::setTimeStep(a0);
}
void sipMolecularDynamics::simulate(bool a0)
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_simulate,&relLock))
		sipMolecularDynamics::sipVH_simulate(&sipPyMethods[3],sipPyThis,relLock,a0);
	else
		MolecularDynamics::simulate(a0);
}
void sipMolecularDynamics::simulateIterations(int a0,bool a1)
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_simulateIterations,&relLock))
		sipMolecularDynamics::sipVH_simulateIterations(&sipPyMethods[4],sipPyThis,relLock,a0,a1);
	else
		MolecularDynamics::simulateIterations(a0,a1);
}
void sipMolecularDynamics::simulateTime(double a0,bool a1)
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[5],sipPyThis,NULL,sipName_BALL_simulateTime,&relLock))
		sipMolecularDynamics::sipVH_simulateTime(&sipPyMethods[5],sipPyThis,relLock,a0,a1);
	else
		MolecularDynamics::simulateTime(a0,a1);
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipMolecularDynamics::sipVH_setup(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,ForceField& a0,SnapShotManager * a1,const Options& a2)
{
	bool res;
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;
	PyObject *a1obj;
	PyObject *a2obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_ForceField);

	a1obj = sipMapCppToSelf(a1,sipClass_SnapShotManager);

	a2obj = sipMapCppToSelf(&a2,sipClass_Options);

	sipArgs = Py_BuildValue("(OOOO)",sipThis -> sipSelf,a0obj,a1obj,a2obj);

	Py_XDECREF(a0obj);

	Py_XDECREF(a1obj);

	Py_XDECREF(a2obj);

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

		sipBadVirtualResultType(sipName_BALL_MolecularDynamics,sipName_BALL_setup);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipMolecularDynamics::sipVH_setup(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,ForceField& a0,SnapShotManager * a1)
{
	bool res;
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;
	PyObject *a1obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_ForceField);

	a1obj = sipMapCppToSelf(a1,sipClass_SnapShotManager);

	sipArgs = Py_BuildValue("(OOO)",sipThis -> sipSelf,a0obj,a1obj);

	Py_XDECREF(a0obj);

	Py_XDECREF(a1obj);

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

		sipBadVirtualResultType(sipName_BALL_MolecularDynamics,sipName_BALL_setup);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipMolecularDynamics::sipVH_specificSetup(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_MolecularDynamics,sipName_BALL_specificSetup);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

void sipMolecularDynamics::sipVH_setTimeStep(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,double a0)
{
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(Od)",sipThis -> sipSelf,a0);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_MolecularDynamics,sipName_BALL_setTimeStep);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipMolecularDynamics::sipVH_simulate(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,bool a0)
{
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(Oi)",sipThis -> sipSelf,a0);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_MolecularDynamics,sipName_BALL_simulate);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipMolecularDynamics::sipVH_simulateIterations(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,int a0,bool a1)
{
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(Oii)",sipThis -> sipSelf,a0,a1);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_MolecularDynamics,sipName_BALL_simulateIterations);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipMolecularDynamics::sipVH_simulateTime(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,double a0,bool a1)
{
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(Odi)",sipThis -> sipSelf,a0,a1);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_MolecularDynamics,sipName_BALL_simulateTime);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

static PyObject *sipDo_MolecularDynamics_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		const MolecularDynamics *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_MolecularDynamics,&a0obj))
		{
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_MolecularDynamics(a0obj,(MolecularDynamics **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> MolecularDynamics::set(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			res = ptr -> MolecularDynamics::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_isValid);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_setup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		ForceField *a0;
		PyObject *a0obj;
		SnapShotManager *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_ForceField,&a0obj,sipCanConvertTo_SnapShotManager,&a1obj))
		{
			bool res;
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_ForceField(a0obj,&a0,1,&iserr);
			sipConvertTo_SnapShotManager(a1obj,&a1,0,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> MolecularDynamics::setup(* a0, a1);

			return sipConvertFromBool((int)res);
		}
	}

	{
		ForceField *a0;
		PyObject *a0obj;
		SnapShotManager *a1;
		PyObject *a1obj;
		const Options *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"III",sipCanConvertTo_ForceField,&a0obj,sipCanConvertTo_SnapShotManager,&a1obj,sipCanConvertTo_Options,&a2obj))
		{
			bool res;
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_ForceField(a0obj,&a0,1,&iserr);
			sipConvertTo_SnapShotManager(a1obj,&a1,0,&iserr);
			sipConvertTo_Options(a2obj,(Options **)&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> MolecularDynamics::setup(* a0, a1,* a2);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_setup);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_specificSetup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			res = ptr -> MolecularDynamics::specificSetup();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_specificSetup);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_setNumberOfIteration(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(sipArgs,"i",&a0))
		{
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			ptr -> MolecularDynamics::setNumberOfIteration( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_setNumberOfIteration);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_setMaximalNumberOfIterations(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(sipArgs,"i",&a0))
		{
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			ptr -> MolecularDynamics::setMaximalNumberOfIterations( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_setMaximalNumberOfIterations);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_setMaximalSimulationTime(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		double a0;

		if (sipParseArgs(sipArgs,"d",&a0))
		{
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			ptr -> MolecularDynamics::setMaximalSimulationTime( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_setMaximalSimulationTime);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_setTimeStep(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		double a0;

		if (sipParseArgs(sipArgs,"d",&a0))
		{
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			ptr -> MolecularDynamics::setTimeStep( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_setTimeStep);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_setReferenceTemperature(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		double a0;

		if (sipParseArgs(sipArgs,"d",&a0))
		{
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			ptr -> MolecularDynamics::setReferenceTemperature( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_setReferenceTemperature);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_setCurrentTime(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		double a0;

		if (sipParseArgs(sipArgs,"d",&a0))
		{
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			ptr -> MolecularDynamics::setCurrentTime( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_setCurrentTime);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_setEnergyOutputFrequency(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(sipArgs,"i",&a0))
		{
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			ptr -> MolecularDynamics::setEnergyOutputFrequency( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_setEnergyOutputFrequency);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_setSnapShotFrequency(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(sipArgs,"i",&a0))
		{
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			ptr -> MolecularDynamics::setSnapShotFrequency( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_setSnapShotFrequency);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_getEnergyOutputFrequency(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			res = ptr -> MolecularDynamics::getEnergyOutputFrequency();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_getEnergyOutputFrequency);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_getNumberOfIteration(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			res = ptr -> MolecularDynamics::getNumberOfIteration();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_getNumberOfIteration);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_getMaximalNumberOfIterations(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			res = ptr -> MolecularDynamics::getMaximalNumberOfIterations();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_getMaximalNumberOfIterations);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_getMaximalSimulationTime(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			double res;
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			res = ptr -> MolecularDynamics::getMaximalSimulationTime();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_getMaximalSimulationTime);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_getTimeStep(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			double res;
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			res = ptr -> MolecularDynamics::getTimeStep();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_getTimeStep);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_getSnapShotFrequency(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			res = ptr -> MolecularDynamics::getSnapShotFrequency();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_getSnapShotFrequency);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_getTemperature(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			double res;
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			res = ptr -> MolecularDynamics::getTemperature();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_getTemperature);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_getTime(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			double res;
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			res = ptr -> MolecularDynamics::getTime();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_getTime);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_getTotalEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			double res;
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			res = ptr -> MolecularDynamics::getTotalEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_getTotalEnergy);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_getPotentialEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			double res;
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			res = ptr -> MolecularDynamics::getPotentialEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_getPotentialEnergy);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_getKineticEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			double res;
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			res = ptr -> MolecularDynamics::getKineticEnergy();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_getKineticEnergy);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_getForceField(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			ForceField *res;
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			res = ptr -> MolecularDynamics::getForceField();

			return sipMapCppToSelf(res,sipClass_ForceField);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_getForceField);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_simulate(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		long a0 = false;

		if (sipParseArgs(sipArgs,"|l",&a0))
		{
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			ptr -> MolecularDynamics::simulate( (bool)a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_simulate);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_simulateIterations(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		int a0;
		long a1 = false;

		if (sipParseArgs(sipArgs,"i|l",&a0,&a1))
		{
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			ptr -> MolecularDynamics::simulateIterations( a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_simulateIterations);

	return NULL;
}

static PyObject *sipDo_MolecularDynamics_simulateTime(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	{
		double a0;
		long a1 = false;

		if (sipParseArgs(sipArgs,"d|l",&a0,&a1))
		{
			MolecularDynamics *ptr;

			if ((ptr = (MolecularDynamics *)sipGetCppPtr(sipThis,sipClass_MolecularDynamics)) == NULL)
				return NULL;

			ptr -> MolecularDynamics::simulateTime( a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MolecularDynamics,sipName_BALL_simulateTime);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_MolecularDynamics(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_MolecularDynamics)
		return ptr;

	return NULL;
}

static void sipDealloc_MolecularDynamics(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipMolecularDynamics *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (MolecularDynamics *)sipThis -> u.cppPtr;
			else
				delete (sipMolecularDynamics *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_MolecularDynamics(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_MolecularDynamics
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
			sipNew = new sipMolecularDynamics();
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

			sipNew = new sipMolecularDynamics(* a0);
	}
	}

	if (sipNew == NULL)
	{
		const MolecularDynamics *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_MolecularDynamics,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_MolecularDynamics(a0obj,(MolecularDynamics **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipMolecularDynamics(* a0, (bool)a1);
	}
	}

	if (sipNew == NULL)
	{
		const MolecularDynamics *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_MolecularDynamics,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_MolecularDynamics(a0obj,(MolecularDynamics **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipMolecularDynamics(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_MolecularDynamics);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_MolecularDynamics,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (MolecularDynamics *)sipNew;
			else
				delete (sipMolecularDynamics *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipMolecularDynamics *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_MolecularDynamics_options(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Options *val;
	MolecularDynamics *ptr;

	if ((ptr = (MolecularDynamics *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_MolecularDynamics)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = &ptr -> options;

		valobj = sipMapCppToSelf(val,sipClass_Options);

		return valobj;
	}

	val = sipForceConvertTo_Options(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_MolecularDynamics,sipName_BALL_options);
		return NULL;
	}

	ptr -> options = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_MolecularDynamics[] = {
	{sipName_BALL_set, sipDo_MolecularDynamics_set, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_MolecularDynamics_isValid, METH_VARARGS, NULL},
	{sipName_BALL_setup, sipDo_MolecularDynamics_setup, METH_VARARGS, NULL},
	{sipName_BALL_specificSetup, sipDo_MolecularDynamics_specificSetup, METH_VARARGS, NULL},
	{sipName_BALL_setNumberOfIteration, sipDo_MolecularDynamics_setNumberOfIteration, METH_VARARGS, NULL},
	{sipName_BALL_setMaximalNumberOfIterations, sipDo_MolecularDynamics_setMaximalNumberOfIterations, METH_VARARGS, NULL},
	{sipName_BALL_setMaximalSimulationTime, sipDo_MolecularDynamics_setMaximalSimulationTime, METH_VARARGS, NULL},
	{sipName_BALL_setTimeStep, sipDo_MolecularDynamics_setTimeStep, METH_VARARGS, NULL},
	{sipName_BALL_setReferenceTemperature, sipDo_MolecularDynamics_setReferenceTemperature, METH_VARARGS, NULL},
	{sipName_BALL_setCurrentTime, sipDo_MolecularDynamics_setCurrentTime, METH_VARARGS, NULL},
	{sipName_BALL_setEnergyOutputFrequency, sipDo_MolecularDynamics_setEnergyOutputFrequency, METH_VARARGS, NULL},
	{sipName_BALL_setSnapShotFrequency, sipDo_MolecularDynamics_setSnapShotFrequency, METH_VARARGS, NULL},
	{sipName_BALL_getEnergyOutputFrequency, sipDo_MolecularDynamics_getEnergyOutputFrequency, METH_VARARGS, NULL},
	{sipName_BALL_getNumberOfIteration, sipDo_MolecularDynamics_getNumberOfIteration, METH_VARARGS, NULL},
	{sipName_BALL_getMaximalNumberOfIterations, sipDo_MolecularDynamics_getMaximalNumberOfIterations, METH_VARARGS, NULL},
	{sipName_BALL_getMaximalSimulationTime, sipDo_MolecularDynamics_getMaximalSimulationTime, METH_VARARGS, NULL},
	{sipName_BALL_getTimeStep, sipDo_MolecularDynamics_getTimeStep, METH_VARARGS, NULL},
	{sipName_BALL_getSnapShotFrequency, sipDo_MolecularDynamics_getSnapShotFrequency, METH_VARARGS, NULL},
	{sipName_BALL_getTemperature, sipDo_MolecularDynamics_getTemperature, METH_VARARGS, NULL},
	{sipName_BALL_getTime, sipDo_MolecularDynamics_getTime, METH_VARARGS, NULL},
	{sipName_BALL_getTotalEnergy, sipDo_MolecularDynamics_getTotalEnergy, METH_VARARGS, NULL},
	{sipName_BALL_getPotentialEnergy, sipDo_MolecularDynamics_getPotentialEnergy, METH_VARARGS, NULL},
	{sipName_BALL_getKineticEnergy, sipDo_MolecularDynamics_getKineticEnergy, METH_VARARGS, NULL},
	{sipName_BALL_getForceField, sipDo_MolecularDynamics_getForceField, METH_VARARGS, NULL},
	{sipName_BALL_simulate, sipDo_MolecularDynamics_simulate, METH_VARARGS, NULL},
	{sipName_BALL_simulateIterations, sipDo_MolecularDynamics_simulateIterations, METH_VARARGS, NULL},
	{sipName_BALL_simulateTime, sipDo_MolecularDynamics_simulateTime, METH_VARARGS, NULL},
	{NULL}
};

PyMethodDef sipClassVarTab_MolecularDynamics[] = {
	{sipName_BALL_options, sipGetSetVar_MolecularDynamics_options, 0, NULL},
	{NULL}
};

PyMethodDef *sipClassVarHierTab_MolecularDynamics[] = {
	sipClassVarTab_MolecularDynamics,
	NULL
};

int sipCanConvertTo_MolecularDynamics(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_MolecularDynamics);
}

void sipConvertTo_MolecularDynamics(PyObject *sipPy,MolecularDynamics **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_MolecularDynamics);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (MolecularDynamics *)sipConvertToCpp(sipPy,sipClass_MolecularDynamics,sipIsErr);
}

MolecularDynamics *sipForceConvertTo_MolecularDynamics(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_MolecularDynamics(valobj))
	{
		MolecularDynamics *val;

		sipConvertTo_MolecularDynamics(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_MolecularDynamics);
	*iserrp = 1;

	return NULL;
}
