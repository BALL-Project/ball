#include "sipBALLDeclBALL.h"
#include "sipBALLSnapShotManager.h"



PyObject *sipClass_SnapShotManager;

static void sipDealloc_SnapShotManager(sipThisType *);

static PyTypeObject sipType_SnapShotManager = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_SnapShotManager,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_SnapShotManager,
	0,
	0,
	0,
	0,
	0,
};

sipSnapShotManager::sipSnapShotManager(): SnapShotManager()
{
	sipCommonCtor(sipPyMethods,5);
}

sipSnapShotManager::sipSnapShotManager(const System * a0,const ForceField * a1,TrajectoryFile * a2,bool a3): SnapShotManager(a0,a1,a2,a3)
{
	sipCommonCtor(sipPyMethods,5);
}

sipSnapShotManager::sipSnapShotManager(const System * a0,const ForceField * a1,const Options& a2,TrajectoryFile * a3,bool a4): SnapShotManager(a0,a1,a2,a3,a4)
{
	sipCommonCtor(sipPyMethods,5);
}

sipSnapShotManager::sipSnapShotManager(const SnapShotManager& a0): SnapShotManager(a0)
{
	sipCommonCtor(sipPyMethods,5);
}

sipSnapShotManager::~sipSnapShotManager()
{
	sipCommonDtor(sipPyThis);
}
bool sipSnapShotManager::setup()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_setup,&relLock) ?
		sipSnapShotManager::sipVH_setup(&sipPyMethods[0],sipPyThis,relLock) :
		SnapShotManager::setup();
}
void sipSnapShotManager::clear()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipSnapShotManager::sipVH_clear(&sipPyMethods[1],sipPyThis,relLock);
	else
		SnapShotManager::clear();
}
bool sipSnapShotManager::isValid() const
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_isValid,&relLock) ?
		sipSnapShotManager::sipVH_isValid(&sipPyMethods[2],sipPyThis,relLock) :
		SnapShotManager::isValid();
}
void sipSnapShotManager::takeSnapShot()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_takeSnapShot,&relLock))
		sipSnapShotManager::sipVH_takeSnapShot(&sipPyMethods[3],sipPyThis,relLock);
	else
		SnapShotManager::takeSnapShot();
}
void sipSnapShotManager::flushToDisk()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_flushToDisk,&relLock))
		sipSnapShotManager::sipVH_flushToDisk(&sipPyMethods[4],sipPyThis,relLock);
	else
		SnapShotManager::flushToDisk();
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipSnapShotManager::sipVH_setup(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_SnapShotManager,sipName_BALL_setup);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

void sipSnapShotManager::sipVH_clear(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
{
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(O)",sipThis -> sipSelf);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_SnapShotManager,sipName_BALL_clear);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipSnapShotManager::sipVH_isValid(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_SnapShotManager,sipName_BALL_isValid);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

void sipSnapShotManager::sipVH_takeSnapShot(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
{
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(O)",sipThis -> sipSelf);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_SnapShotManager,sipName_BALL_takeSnapShot);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipSnapShotManager::sipVH_flushToDisk(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
{
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(O)",sipThis -> sipSelf);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_SnapShotManager,sipName_BALL_flushToDisk);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

static PyObject *sipDo_SnapShotManager_setup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SnapShotManager)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			SnapShotManager *ptr;

			if ((ptr = (SnapShotManager *)sipGetCppPtr(sipThis,sipClass_SnapShotManager)) == NULL)
				return NULL;

			res = ptr -> SnapShotManager::setup();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_SnapShotManager,sipName_BALL_setup);

	return NULL;
}

static PyObject *sipDo_SnapShotManager_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SnapShotManager)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			SnapShotManager *ptr;

			if ((ptr = (SnapShotManager *)sipGetCppPtr(sipThis,sipClass_SnapShotManager)) == NULL)
				return NULL;

			ptr -> SnapShotManager::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_SnapShotManager,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_SnapShotManager_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SnapShotManager)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			SnapShotManager *ptr;

			if ((ptr = (SnapShotManager *)sipGetCppPtr(sipThis,sipClass_SnapShotManager)) == NULL)
				return NULL;

			res = ptr -> SnapShotManager::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_SnapShotManager,sipName_BALL_isValid);

	return NULL;
}

static PyObject *sipDo_SnapShotManager_setFlushToDiskFrequency(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SnapShotManager)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(sipArgs,"i",&a0))
		{
			SnapShotManager *ptr;

			if ((ptr = (SnapShotManager *)sipGetCppPtr(sipThis,sipClass_SnapShotManager)) == NULL)
				return NULL;

			ptr -> SnapShotManager::setFlushToDiskFrequency( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_SnapShotManager,sipName_BALL_setFlushToDiskFrequency);

	return NULL;
}

static PyObject *sipDo_SnapShotManager_getFlushToDiskFrequency(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SnapShotManager)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			SnapShotManager *ptr;

			if ((ptr = (SnapShotManager *)sipGetCppPtr(sipThis,sipClass_SnapShotManager)) == NULL)
				return NULL;

			res = ptr -> SnapShotManager::getFlushToDiskFrequency();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_SnapShotManager,sipName_BALL_getFlushToDiskFrequency);

	return NULL;
}

static PyObject *sipDo_SnapShotManager_takeSnapShot(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SnapShotManager)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			SnapShotManager *ptr;

			if ((ptr = (SnapShotManager *)sipGetCppPtr(sipThis,sipClass_SnapShotManager)) == NULL)
				return NULL;

			ptr -> SnapShotManager::takeSnapShot();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_SnapShotManager,sipName_BALL_takeSnapShot);

	return NULL;
}

static PyObject *sipDo_SnapShotManager_flushToDisk(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SnapShotManager)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			SnapShotManager *ptr;

			if ((ptr = (SnapShotManager *)sipGetCppPtr(sipThis,sipClass_SnapShotManager)) == NULL)
				return NULL;

			ptr -> SnapShotManager::flushToDisk();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_SnapShotManager,sipName_BALL_flushToDisk);

	return NULL;
}

static PyObject *sipDo_SnapShotManager_getNumberOfSnapShots(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SnapShotManager)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			SnapShotManager *ptr;

			if ((ptr = (SnapShotManager *)sipGetCppPtr(sipThis,sipClass_SnapShotManager)) == NULL)
				return NULL;

			res = ptr -> SnapShotManager::getNumberOfSnapShots();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_SnapShotManager,sipName_BALL_getNumberOfSnapShots);

	return NULL;
}

static PyObject *sipDo_SnapShotManager_getNumberOfAtoms(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SnapShotManager)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			SnapShotManager *ptr;

			if ((ptr = (SnapShotManager *)sipGetCppPtr(sipThis,sipClass_SnapShotManager)) == NULL)
				return NULL;

			res = ptr -> SnapShotManager::getNumberOfAtoms();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_SnapShotManager,sipName_BALL_getNumberOfAtoms);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_SnapShotManager(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_SnapShotManager)
		return ptr;

	return NULL;
}

static void sipDealloc_SnapShotManager(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipSnapShotManager *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (SnapShotManager *)sipThis -> u.cppPtr;
			else
				delete (sipSnapShotManager *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_SnapShotManager(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_SnapShotManager
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
			sipNew = new sipSnapShotManager();
	}
	}

	if (sipNew == NULL)
	{
		const System *a0;
		PyObject *a0obj;
		const ForceField *a1;
		PyObject *a1obj;
		TrajectoryFile *a2;
		PyObject *a2obj;
		long a3 = true;

		if (sipParseArgs(sipArgs,"-III|l",sipCanConvertTo_System,&a0obj,sipCanConvertTo_ForceField,&a1obj,sipCanConvertTo_TrajectoryFile,&a2obj,&a3))
		{
			int iserr = 0;

			sipConvertTo_System(a0obj,(System **)&a0,0,&iserr);
			sipConvertTo_ForceField(a1obj,(ForceField **)&a1,0,&iserr);
			sipConvertTo_TrajectoryFile(a2obj,&a2,0,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipSnapShotManager( a0, a1, a2, (bool)a3);
	}
	}

	if (sipNew == NULL)
	{
		const System *a0;
		PyObject *a0obj;
		const ForceField *a1;
		PyObject *a1obj;
		const Options *a2;
		PyObject *a2obj;
		TrajectoryFile *a3;
		PyObject *a3obj;
		long a4 = true;

		if (sipParseArgs(sipArgs,"-IIII|l",sipCanConvertTo_System,&a0obj,sipCanConvertTo_ForceField,&a1obj,sipCanConvertTo_Options,&a2obj,sipCanConvertTo_TrajectoryFile,&a3obj,&a4))
		{
			int iserr = 0;

			sipConvertTo_System(a0obj,(System **)&a0,0,&iserr);
			sipConvertTo_ForceField(a1obj,(ForceField **)&a1,0,&iserr);
			sipConvertTo_Options(a2obj,(Options **)&a2,1,&iserr);
			sipConvertTo_TrajectoryFile(a3obj,&a3,0,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipSnapShotManager( a0, a1,* a2, a3, (bool)a4);
	}
	}

	if (sipNew == NULL)
	{
		const SnapShotManager *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_SnapShotManager,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_SnapShotManager(a0obj,(SnapShotManager **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipSnapShotManager(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_SnapShotManager);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_SnapShotManager,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (SnapShotManager *)sipNew;
			else
				delete (sipSnapShotManager *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipSnapShotManager *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_SnapShotManager[] = {
	{sipName_BALL_setup, sipDo_SnapShotManager_setup, METH_VARARGS, NULL},
	{sipName_BALL_clear, sipDo_SnapShotManager_clear, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_SnapShotManager_isValid, METH_VARARGS, NULL},
	{sipName_BALL_setFlushToDiskFrequency, sipDo_SnapShotManager_setFlushToDiskFrequency, METH_VARARGS, NULL},
	{sipName_BALL_getFlushToDiskFrequency, sipDo_SnapShotManager_getFlushToDiskFrequency, METH_VARARGS, NULL},
	{sipName_BALL_takeSnapShot, sipDo_SnapShotManager_takeSnapShot, METH_VARARGS, NULL},
	{sipName_BALL_flushToDisk, sipDo_SnapShotManager_flushToDisk, METH_VARARGS, NULL},
	{sipName_BALL_getNumberOfSnapShots, sipDo_SnapShotManager_getNumberOfSnapShots, METH_VARARGS, NULL},
	{sipName_BALL_getNumberOfAtoms, sipDo_SnapShotManager_getNumberOfAtoms, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_SnapShotManager(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_SnapShotManager);
}

void sipConvertTo_SnapShotManager(PyObject *sipPy,SnapShotManager **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_SnapShotManager);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (SnapShotManager *)sipConvertToCpp(sipPy,sipClass_SnapShotManager,sipIsErr);
}

SnapShotManager *sipForceConvertTo_SnapShotManager(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_SnapShotManager(valobj))
	{
		SnapShotManager *val;

		sipConvertTo_SnapShotManager(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_SnapShotManager);
	*iserrp = 1;

	return NULL;
}
