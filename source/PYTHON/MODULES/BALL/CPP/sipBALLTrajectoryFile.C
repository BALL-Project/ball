#include "sipBALLDeclBALL.h"
#include "sipBALLTrajectoryFile.h"



PyObject *sipClass_TrajectoryFile;

static void sipDealloc_TrajectoryFile(sipThisType *);

static PyTypeObject sipType_TrajectoryFile = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_TrajectoryFile,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_TrajectoryFile,
	0,
	0,
	0,
	0,
	0,
};

sipTrajectoryFile::sipTrajectoryFile(): TrajectoryFile()
{
	sipCommonCtor(sipPyMethods,7);
}

sipTrajectoryFile::sipTrajectoryFile(const TrajectoryFile& a0): TrajectoryFile(a0)
{
	sipCommonCtor(sipPyMethods,7);
}

sipTrajectoryFile::sipTrajectoryFile(const String& a0,OpenMode a1): TrajectoryFile(a0,a1)
{
	sipCommonCtor(sipPyMethods,7);
}

sipTrajectoryFile::~sipTrajectoryFile()
{
	sipCommonDtor(sipPyThis);
}
void sipTrajectoryFile::clear()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipTrajectoryFile::sipVH_clear(&sipPyMethods[0],sipPyThis,relLock);
	else
		TrajectoryFile::clear();
}
bool sipTrajectoryFile::readHeader()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_readHeader,&relLock) ?
		sipTrajectoryFile::sipVH_readHeader(&sipPyMethods[1],sipPyThis,relLock) :
		TrajectoryFile::readHeader();
}
bool sipTrajectoryFile::updateHeader(const SnapShotManager& a0)
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_updateHeader,&relLock) ?
		sipTrajectoryFile::sipVH_updateHeader(&sipPyMethods[2],sipPyThis,relLock,a0) :
		TrajectoryFile::updateHeader(a0);
}
bool sipTrajectoryFile::writeHeader()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_writeHeader,&relLock) ?
		sipTrajectoryFile::sipVH_writeHeader(&sipPyMethods[3],sipPyThis,relLock) :
		TrajectoryFile::writeHeader();
}
bool sipTrajectoryFile::read(SnapShotManager& a0)
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_read,&relLock) ?
		sipTrajectoryFile::sipVH_read(&sipPyMethods[4],sipPyThis,relLock,a0) :
		TrajectoryFile::read(a0);
}
bool sipTrajectoryFile::write(const SnapShotManager& a0)
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[5],sipPyThis,NULL,sipName_BALL_write,&relLock) ?
		sipTrajectoryFile::sipVH_write(&sipPyMethods[5],sipPyThis,relLock,a0) :
		TrajectoryFile::write(a0);
}
bool sipTrajectoryFile::append(const SnapShot& a0)
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[6],sipPyThis,NULL,sipName_BALL_append,&relLock) ?
		sipTrajectoryFile::sipVH_append(&sipPyMethods[6],sipPyThis,relLock,a0) :
		TrajectoryFile::append(a0);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipTrajectoryFile::sipVH_clear(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_TrajectoryFile,sipName_BALL_clear);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipTrajectoryFile::sipVH_readHeader(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_TrajectoryFile,sipName_BALL_readHeader);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipTrajectoryFile::sipVH_updateHeader(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,const SnapShotManager& a0)
{
	bool res;
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_SnapShotManager);

	sipArgs = Py_BuildValue("(OO)",sipThis -> sipSelf,a0obj);

	Py_XDECREF(a0obj);

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

		sipBadVirtualResultType(sipName_BALL_TrajectoryFile,sipName_BALL_updateHeader);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipTrajectoryFile::sipVH_writeHeader(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_TrajectoryFile,sipName_BALL_writeHeader);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipTrajectoryFile::sipVH_read(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,SnapShotManager& a0)
{
	bool res;
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_SnapShotManager);

	sipArgs = Py_BuildValue("(OO)",sipThis -> sipSelf,a0obj);

	Py_XDECREF(a0obj);

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

		sipBadVirtualResultType(sipName_BALL_TrajectoryFile,sipName_BALL_read);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipTrajectoryFile::sipVH_write(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,const SnapShotManager& a0)
{
	bool res;
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_SnapShotManager);

	sipArgs = Py_BuildValue("(OO)",sipThis -> sipSelf,a0obj);

	Py_XDECREF(a0obj);

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

		sipBadVirtualResultType(sipName_BALL_TrajectoryFile,sipName_BALL_write);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipTrajectoryFile::sipVH_append(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,const SnapShot& a0)
{
	bool res;
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_SnapShot);

	sipArgs = Py_BuildValue("(OO)",sipThis -> sipSelf,a0obj);

	Py_XDECREF(a0obj);

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

		sipBadVirtualResultType(sipName_BALL_TrajectoryFile,sipName_BALL_append);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

static PyObject *sipDo_TrajectoryFile_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_TrajectoryFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			TrajectoryFile *ptr;

			if ((ptr = (TrajectoryFile *)sipGetCppPtr(sipThis,sipClass_TrajectoryFile)) == NULL)
				return NULL;

			ptr -> TrajectoryFile::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_TrajectoryFile,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_TrajectoryFile_readHeader(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_TrajectoryFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			TrajectoryFile *ptr;

			if ((ptr = (TrajectoryFile *)sipGetCppPtr(sipThis,sipClass_TrajectoryFile)) == NULL)
				return NULL;

			res = ptr -> TrajectoryFile::readHeader();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_TrajectoryFile,sipName_BALL_readHeader);

	return NULL;
}

static PyObject *sipDo_TrajectoryFile_updateHeader(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_TrajectoryFile)) == NULL)
		return NULL;

	{
		const SnapShotManager *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_SnapShotManager,&a0obj))
		{
			bool res;
			TrajectoryFile *ptr;

			if ((ptr = (TrajectoryFile *)sipGetCppPtr(sipThis,sipClass_TrajectoryFile)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_SnapShotManager(a0obj,(SnapShotManager **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> TrajectoryFile::updateHeader(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_TrajectoryFile,sipName_BALL_updateHeader);

	return NULL;
}

static PyObject *sipDo_TrajectoryFile_writeHeader(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_TrajectoryFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			TrajectoryFile *ptr;

			if ((ptr = (TrajectoryFile *)sipGetCppPtr(sipThis,sipClass_TrajectoryFile)) == NULL)
				return NULL;

			res = ptr -> TrajectoryFile::writeHeader();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_TrajectoryFile,sipName_BALL_writeHeader);

	return NULL;
}

static PyObject *sipDo_TrajectoryFile_read(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_TrajectoryFile)) == NULL)
		return NULL;

	{
		SnapShotManager *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_SnapShotManager,&a0obj))
		{
			bool res;
			TrajectoryFile *ptr;

			if ((ptr = (TrajectoryFile *)sipGetCppPtr(sipThis,sipClass_TrajectoryFile)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_SnapShotManager(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> TrajectoryFile::read(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_TrajectoryFile,sipName_BALL_read);

	return NULL;
}

static PyObject *sipDo_TrajectoryFile_write(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_TrajectoryFile)) == NULL)
		return NULL;

	{
		const SnapShotManager *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_SnapShotManager,&a0obj))
		{
			bool res;
			TrajectoryFile *ptr;

			if ((ptr = (TrajectoryFile *)sipGetCppPtr(sipThis,sipClass_TrajectoryFile)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_SnapShotManager(a0obj,(SnapShotManager **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> TrajectoryFile::write(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_TrajectoryFile,sipName_BALL_write);

	return NULL;
}

static PyObject *sipDo_TrajectoryFile_append(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_TrajectoryFile)) == NULL)
		return NULL;

	{
		const SnapShot *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_SnapShot,&a0obj))
		{
			bool res;
			TrajectoryFile *ptr;

			if ((ptr = (TrajectoryFile *)sipGetCppPtr(sipThis,sipClass_TrajectoryFile)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_SnapShot(a0obj,(SnapShot **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> TrajectoryFile::append(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_TrajectoryFile,sipName_BALL_append);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_TrajectoryFile(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_TrajectoryFile)
		return ptr;

	if ((res = sipCast_File((File *)(TrajectoryFile *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_TrajectoryFile(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipTrajectoryFile *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (TrajectoryFile *)sipThis -> u.cppPtr;
			else
				delete (sipTrajectoryFile *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_TrajectoryFile(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_TrajectoryFile
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
			sipNew = new sipTrajectoryFile();
	}
	}

	if (sipNew == NULL)
	{
		const TrajectoryFile *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_TrajectoryFile,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_TrajectoryFile(a0obj,(TrajectoryFile **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipTrajectoryFile(* a0);
	}
	}

	if (sipNew == NULL)
	{
		const String *a0;
		PyObject *a0obj;
		OpenMode *a1 = (OpenMode *)&File::IN;
		PyObject *a1obj = NULL;

		if (sipParseArgs(sipArgs,"-I|I",sipCanConvertTo_String,&a0obj,sipCanConvertTo_OpenMode,&a1obj))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_OpenMode(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipTrajectoryFile(* a0,* a1);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_TrajectoryFile);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_TrajectoryFile,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (TrajectoryFile *)sipNew;
			else
				delete (sipTrajectoryFile *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipTrajectoryFile *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_TrajectoryFile[] = {
	{sipName_BALL_clear, sipDo_TrajectoryFile_clear, METH_VARARGS, NULL},
	{sipName_BALL_readHeader, sipDo_TrajectoryFile_readHeader, METH_VARARGS, NULL},
	{sipName_BALL_updateHeader, sipDo_TrajectoryFile_updateHeader, METH_VARARGS, NULL},
	{sipName_BALL_writeHeader, sipDo_TrajectoryFile_writeHeader, METH_VARARGS, NULL},
	{sipName_BALL_read, sipDo_TrajectoryFile_read, METH_VARARGS, NULL},
	{sipName_BALL_write, sipDo_TrajectoryFile_write, METH_VARARGS, NULL},
	{sipName_BALL_append, sipDo_TrajectoryFile_append, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_TrajectoryFile(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_TrajectoryFile);
}

void sipConvertTo_TrajectoryFile(PyObject *sipPy,TrajectoryFile **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_TrajectoryFile);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (TrajectoryFile *)sipConvertToCpp(sipPy,sipClass_TrajectoryFile,sipIsErr);
}

TrajectoryFile *sipForceConvertTo_TrajectoryFile(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_TrajectoryFile(valobj))
	{
		TrajectoryFile *val;

		sipConvertTo_TrajectoryFile(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_TrajectoryFile);
	*iserrp = 1;

	return NULL;
}
