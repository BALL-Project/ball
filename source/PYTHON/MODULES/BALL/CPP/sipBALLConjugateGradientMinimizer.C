#include "sipBALLDeclBALL.h"
#include "sipBALLConjugateGradientMinimizer.h"



PyObject *sipClass_ConjugateGradientMinimizer;

static void sipDealloc_ConjugateGradientMinimizer(sipThisType *);

static PyTypeObject sipType_ConjugateGradientMinimizer = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_ConjugateGradientMinimizer,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_ConjugateGradientMinimizer,
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

sipConjugateGradientMinimizer::sipConjugateGradientMinimizer()
    : ConjugateGradientMinimizer()
{
	sipCommonCtor(sipPyMethods,10);
}

sipConjugateGradientMinimizer::sipConjugateGradientMinimizer(ForceField& a0)
    : ConjugateGradientMinimizer(a0)
{
	sipCommonCtor(sipPyMethods,10);
}

sipConjugateGradientMinimizer::sipConjugateGradientMinimizer(ForceField& a0,SnapShotManager * a1)
    : ConjugateGradientMinimizer(a0,a1)
{
	sipCommonCtor(sipPyMethods,10);
}

sipConjugateGradientMinimizer::sipConjugateGradientMinimizer(ForceField& a0,SnapShotManager * a1,const Options& a2)
    : ConjugateGradientMinimizer(a0,a1,a2)
{
	sipCommonCtor(sipPyMethods,10);
}

sipConjugateGradientMinimizer::sipConjugateGradientMinimizer(ForceField& a0,const Options& a1)
    : ConjugateGradientMinimizer(a0,a1)
{
	sipCommonCtor(sipPyMethods,10);
}

sipConjugateGradientMinimizer::sipConjugateGradientMinimizer(const ConjugateGradientMinimizer& a0,bool a1)
    : ConjugateGradientMinimizer(a0,a1)
{
	sipCommonCtor(sipPyMethods,10);
}

sipConjugateGradientMinimizer::sipConjugateGradientMinimizer(const ConjugateGradientMinimizer& a0)
    : ConjugateGradientMinimizer(a0)
{
	sipCommonCtor(sipPyMethods,10);
}

sipConjugateGradientMinimizer::~sipConjugateGradientMinimizer()
 
{
	sipCommonDtor(sipPyThis);
}

void sipConjugateGradientMinimizer::finishIteration()

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_finishIteration,&relLock))
		sipEnergyMinimizer::sipVH_finishIteration(&sipPyMethods[0],sipPyThis,relLock);
	else
		EnergyMinimizer::finishIteration();
}

void sipConjugateGradientMinimizer::takeSnapShot() const

{
	int relLock;

	if (sipIsPyMethod((sipMethodCache *)&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_takeSnapShot,&relLock))
		sipEnergyMinimizer::sipVH_takeSnapShot(&sipPyMethods[1],sipPyThis,relLock);
	else
		EnergyMinimizer::takeSnapShot();
}

void sipConjugateGradientMinimizer::printEnergy() const

{
	int relLock;

	if (sipIsPyMethod((sipMethodCache *)&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_printEnergy,&relLock))
		sipEnergyMinimizer::sipVH_printEnergy(&sipPyMethods[2],sipPyThis,relLock);
	else
		EnergyMinimizer::printEnergy();
}

void sipConjugateGradientMinimizer::updateForces()

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_updateForces,&relLock))
		sipEnergyMinimizer::sipVH_updateForces(&sipPyMethods[3],sipPyThis,relLock);
	else
		EnergyMinimizer::updateForces();
}

double sipConjugateGradientMinimizer::updateEnergy()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_updateEnergy,&relLock) ?
		sipEnergyMinimizer::sipVH_updateEnergy(&sipPyMethods[4],sipPyThis,relLock) :
		EnergyMinimizer::updateEnergy();
}

bool sipConjugateGradientMinimizer::isConverged() const

{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[5],sipPyThis,NULL,sipName_BALL_isConverged,&relLock) ?
		sipEnergyMinimizer::sipVH_isConverged(&sipPyMethods[5],sipPyThis,relLock) :
		EnergyMinimizer::isConverged();
}

bool sipConjugateGradientMinimizer::minimize(int a0,bool a1)

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[6],sipPyThis,NULL,sipName_BALL_minimize,&relLock) ?
		sipEnergyMinimizer::sipVH_minimize(&sipPyMethods[6],sipPyThis,relLock,a0,a1) :
		ConjugateGradientMinimizer::minimize(a0,a1);
}

void sipConjugateGradientMinimizer::updateDirection()

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[7],sipPyThis,NULL,sipName_BALL_updateDirection,&relLock))
		sipEnergyMinimizer::sipVH_updateDirection(&sipPyMethods[7],sipPyThis,relLock);
	else
		ConjugateGradientMinimizer::updateDirection();
}

bool sipConjugateGradientMinimizer::findStep()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[8],sipPyThis,NULL,sipName_BALL_findStep,&relLock) ?
		sipEnergyMinimizer::sipVH_findStep(&sipPyMethods[8],sipPyThis,relLock) :
		ConjugateGradientMinimizer::findStep();
}

bool sipConjugateGradientMinimizer::specificSetup()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[9],sipPyThis,NULL,sipName_BALL_specificSetup,&relLock) ?
		sipEnergyMinimizer::sipVH_specificSetup(&sipPyMethods[9],sipPyThis,relLock) :
		ConjugateGradientMinimizer::specificSetup();
}

static PyObject *sipDo_ConjugateGradientMinimizer_specificSetup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ConjugateGradientMinimizer)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			ConjugateGradientMinimizer *ptr;

			if ((ptr = (ConjugateGradientMinimizer *)sipGetCppPtr(sipThis,sipClass_ConjugateGradientMinimizer)) == NULL)
				return NULL;

			res = ptr -> ConjugateGradientMinimizer::specificSetup();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ConjugateGradientMinimizer,sipName_BALL_specificSetup);

	return NULL;
}

static PyObject *sipDo_ConjugateGradientMinimizer_setStepLength(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ConjugateGradientMinimizer)) == NULL)
		return NULL;

	{
		double a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"d",&a0))
		{
			ConjugateGradientMinimizer *ptr;

			if ((ptr = (ConjugateGradientMinimizer *)sipGetCppPtr(sipThis,sipClass_ConjugateGradientMinimizer)) == NULL)
				return NULL;

			ptr -> ConjugateGradientMinimizer::setStepLength( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ConjugateGradientMinimizer,sipName_BALL_setStepLength);

	return NULL;
}

static PyObject *sipDo_ConjugateGradientMinimizer_getStepLength(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ConjugateGradientMinimizer)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			ConjugateGradientMinimizer *ptr;

			if ((ptr = (ConjugateGradientMinimizer *)sipGetCppPtr(sipThis,sipClass_ConjugateGradientMinimizer)) == NULL)
				return NULL;

			res = ptr -> ConjugateGradientMinimizer::getStepLength();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ConjugateGradientMinimizer,sipName_BALL_getStepLength);

	return NULL;
}

static PyObject *sipDo_ConjugateGradientMinimizer_findStep(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ConjugateGradientMinimizer)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			ConjugateGradientMinimizer *ptr;

			if ((ptr = (ConjugateGradientMinimizer *)sipGetCppPtr(sipThis,sipClass_ConjugateGradientMinimizer)) == NULL)
				return NULL;

			res = ptr -> ConjugateGradientMinimizer::findStep();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ConjugateGradientMinimizer,sipName_BALL_findStep);

	return NULL;
}

static PyObject *sipDo_ConjugateGradientMinimizer_updateDirection(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ConjugateGradientMinimizer)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			ConjugateGradientMinimizer *ptr;

			if ((ptr = (ConjugateGradientMinimizer *)sipGetCppPtr(sipThis,sipClass_ConjugateGradientMinimizer)) == NULL)
				return NULL;

			ptr -> ConjugateGradientMinimizer::updateDirection();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ConjugateGradientMinimizer,sipName_BALL_updateDirection);

	return NULL;
}

static PyObject *sipDo_ConjugateGradientMinimizer_minimize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ConjugateGradientMinimizer)) == NULL)
		return NULL;

	{
		int a0 = 0;
		long a1 = false;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"|il",&a0,&a1))
		{
			bool res;
			ConjugateGradientMinimizer *ptr;

			if ((ptr = (ConjugateGradientMinimizer *)sipGetCppPtr(sipThis,sipClass_ConjugateGradientMinimizer)) == NULL)
				return NULL;

			res = ptr -> ConjugateGradientMinimizer::minimize( a0, (bool)a1);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ConjugateGradientMinimizer,sipName_BALL_minimize);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_ConjugateGradientMinimizer(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_ConjugateGradientMinimizer)
		return ptr;

	if ((res = sipCast_EnergyMinimizer((EnergyMinimizer *)(ConjugateGradientMinimizer *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_ConjugateGradientMinimizer(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipConjugateGradientMinimizer *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (ConjugateGradientMinimizer *)sipThis -> u.cppPtr;
			else
				delete (sipConjugateGradientMinimizer *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_ConjugateGradientMinimizer(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_ConjugateGradientMinimizer
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
			sipNew = new sipConjugateGradientMinimizer();
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

			sipNew = new sipConjugateGradientMinimizer(* a0);
		}
	}

	if (sipNew == NULL)
	{
		ForceField *a0;
		PyObject *a0obj;
		SnapShotManager *a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-II",sipCanConvertTo_ForceField,&a0obj,sipCanConvertTo_SnapShotManager,&a1obj))
		{
			int iserr = 0;

			sipConvertTo_ForceField(a0obj,&a0,1,&iserr);
			sipConvertTo_SnapShotManager(a1obj,&a1,0,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipConjugateGradientMinimizer(* a0, a1);
		}
	}

	if (sipNew == NULL)
	{
		ForceField *a0;
		PyObject *a0obj;
		SnapShotManager *a1;
		PyObject *a1obj;
		const Options *a2;
		PyObject *a2obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-III",sipCanConvertTo_ForceField,&a0obj,sipCanConvertTo_SnapShotManager,&a1obj,sipCanConvertTo_Options,&a2obj))
		{
			int iserr = 0;

			sipConvertTo_ForceField(a0obj,&a0,1,&iserr);
			sipConvertTo_SnapShotManager(a1obj,&a1,0,&iserr);
			sipConvertTo_Options(a2obj,(Options **)&a2,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipConjugateGradientMinimizer(* a0, a1,* a2);
		}
	}

	if (sipNew == NULL)
	{
		ForceField *a0;
		PyObject *a0obj;
		const Options *a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-II",sipCanConvertTo_ForceField,&a0obj,sipCanConvertTo_Options,&a1obj))
		{
			int iserr = 0;

			sipConvertTo_ForceField(a0obj,&a0,1,&iserr);
			sipConvertTo_Options(a1obj,(Options **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipConjugateGradientMinimizer(* a0,* a1);
		}
	}

	if (sipNew == NULL)
	{
		const ConjugateGradientMinimizer *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_ConjugateGradientMinimizer,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_ConjugateGradientMinimizer(a0obj,(ConjugateGradientMinimizer **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipConjugateGradientMinimizer(* a0, (bool)a1);
		}
	}

	if (sipNew == NULL)
	{
		const ConjugateGradientMinimizer *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_ConjugateGradientMinimizer,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_ConjugateGradientMinimizer(a0obj,(ConjugateGradientMinimizer **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipConjugateGradientMinimizer(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_ConjugateGradientMinimizer);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_ConjugateGradientMinimizer,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (ConjugateGradientMinimizer *)sipNew;
			else
				delete (sipConjugateGradientMinimizer *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipConjugateGradientMinimizer *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_ConjugateGradientMinimizer_options(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Options *val;
	ConjugateGradientMinimizer *ptr;

	if ((ptr = (ConjugateGradientMinimizer *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_ConjugateGradientMinimizer)) == NULL)
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
		sipBadSetType(sipName_BALL_ConjugateGradientMinimizer,sipName_BALL_options);
		return NULL;
	}

	ptr -> options = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_ConjugateGradientMinimizer[] = {
	{sipName_BALL_specificSetup, sipDo_ConjugateGradientMinimizer_specificSetup, METH_VARARGS, NULL},
	{sipName_BALL_setStepLength, sipDo_ConjugateGradientMinimizer_setStepLength, METH_VARARGS, NULL},
	{sipName_BALL_getStepLength, sipDo_ConjugateGradientMinimizer_getStepLength, METH_VARARGS, NULL},
	{sipName_BALL_findStep, sipDo_ConjugateGradientMinimizer_findStep, METH_VARARGS, NULL},
	{sipName_BALL_updateDirection, sipDo_ConjugateGradientMinimizer_updateDirection, METH_VARARGS, NULL},
	{sipName_BALL_minimize, sipDo_ConjugateGradientMinimizer_minimize, METH_VARARGS, NULL},
	{NULL}
};

PyMethodDef sipClassVarTab_ConjugateGradientMinimizer[] = {
	{sipName_BALL_options, sipGetSetVar_ConjugateGradientMinimizer_options, 0, NULL},
	{NULL}
};

PyMethodDef *sipClassVarHierTab_ConjugateGradientMinimizer[] = {
	sipClassVarTab_ConjugateGradientMinimizer,
	sipClassVarTab_EnergyMinimizer,
	NULL
};

int sipCanConvertTo_ConjugateGradientMinimizer(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_ConjugateGradientMinimizer);
}

void sipConvertTo_ConjugateGradientMinimizer(PyObject *sipPy,ConjugateGradientMinimizer **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_ConjugateGradientMinimizer);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (ConjugateGradientMinimizer *)sipConvertToCpp(sipPy,sipClass_ConjugateGradientMinimizer,sipIsErr);
}

ConjugateGradientMinimizer *sipForceConvertTo_ConjugateGradientMinimizer(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_ConjugateGradientMinimizer(valobj))
	{
		ConjugateGradientMinimizer *val;

		sipConvertTo_ConjugateGradientMinimizer(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_ConjugateGradientMinimizer);
	*iserrp = 1;

	return NULL;
}
