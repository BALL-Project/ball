#include "sipBALLDeclBALL.h"
#include "sipBALLDCDFile.h"



PyObject *sipClass_DCDFile;

static void sipDealloc_DCDFile(sipThisType *);

static PyTypeObject sipType_DCDFile = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_DCDFile,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_DCDFile,
	0,
	0,
	0,
	0,
	0,
};

sipDCDFile::sipDCDFile(): DCDFile()
{
	sipCommonCtor(sipPyMethods,7);
}

sipDCDFile::sipDCDFile(const DCDFile& a0): DCDFile(a0)
{
	sipCommonCtor(sipPyMethods,7);
}

sipDCDFile::sipDCDFile(const String& a0,OpenMode a1): DCDFile(a0,a1)
{
	sipCommonCtor(sipPyMethods,7);
}

sipDCDFile::~sipDCDFile()
{
	sipCommonDtor(sipPyThis);
}
void sipDCDFile::clear()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipTrajectoryFile::sipVH_clear(&sipPyMethods[0],sipPyThis,relLock);
	else
		DCDFile::clear();
}
bool sipDCDFile::readHeader()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_readHeader,&relLock) ?
		sipTrajectoryFile::sipVH_readHeader(&sipPyMethods[1],sipPyThis,relLock) :
		DCDFile::readHeader();
}
bool sipDCDFile::updateHeader(const SnapShotManager& a0)
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_updateHeader,&relLock) ?
		sipTrajectoryFile::sipVH_updateHeader(&sipPyMethods[2],sipPyThis,relLock,a0) :
		DCDFile::updateHeader(a0);
}
bool sipDCDFile::writeHeader()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_writeHeader,&relLock) ?
		sipTrajectoryFile::sipVH_writeHeader(&sipPyMethods[3],sipPyThis,relLock) :
		DCDFile::writeHeader();
}
bool sipDCDFile::read(SnapShotManager& a0)
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_read,&relLock) ?
		sipTrajectoryFile::sipVH_read(&sipPyMethods[4],sipPyThis,relLock,a0) :
		DCDFile::read(a0);
}
bool sipDCDFile::write(const SnapShotManager& a0)
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[5],sipPyThis,NULL,sipName_BALL_write,&relLock) ?
		sipTrajectoryFile::sipVH_write(&sipPyMethods[5],sipPyThis,relLock,a0) :
		DCDFile::write(a0);
}
bool sipDCDFile::append(const SnapShot& a0)
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[6],sipPyThis,NULL,sipName_BALL_append,&relLock) ?
		sipTrajectoryFile::sipVH_append(&sipPyMethods[6],sipPyThis,relLock,a0) :
		DCDFile::append(a0);
}

static PyObject *sipDo_DCDFile_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_DCDFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			DCDFile *ptr;

			if ((ptr = (DCDFile *)sipGetCppPtr(sipThis,sipClass_DCDFile)) == NULL)
				return NULL;

			ptr -> DCDFile::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_DCDFile,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_DCDFile_readHeader(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_DCDFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			DCDFile *ptr;

			if ((ptr = (DCDFile *)sipGetCppPtr(sipThis,sipClass_DCDFile)) == NULL)
				return NULL;

			res = ptr -> DCDFile::readHeader();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_DCDFile,sipName_BALL_readHeader);

	return NULL;
}

static PyObject *sipDo_DCDFile_updateHeader(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_DCDFile)) == NULL)
		return NULL;

	{
		const SnapShotManager *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_SnapShotManager,&a0obj))
		{
			bool res;
			DCDFile *ptr;

			if ((ptr = (DCDFile *)sipGetCppPtr(sipThis,sipClass_DCDFile)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_SnapShotManager(a0obj,(SnapShotManager **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> DCDFile::updateHeader(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_DCDFile,sipName_BALL_updateHeader);

	return NULL;
}

static PyObject *sipDo_DCDFile_writeHeader(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_DCDFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			DCDFile *ptr;

			if ((ptr = (DCDFile *)sipGetCppPtr(sipThis,sipClass_DCDFile)) == NULL)
				return NULL;

			res = ptr -> DCDFile::writeHeader();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_DCDFile,sipName_BALL_writeHeader);

	return NULL;
}

static PyObject *sipDo_DCDFile_read(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_DCDFile)) == NULL)
		return NULL;

	{
		SnapShotManager *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_SnapShotManager,&a0obj))
		{
			bool res;
			DCDFile *ptr;

			if ((ptr = (DCDFile *)sipGetCppPtr(sipThis,sipClass_DCDFile)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_SnapShotManager(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> DCDFile::read(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_DCDFile,sipName_BALL_read);

	return NULL;
}

static PyObject *sipDo_DCDFile_write(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_DCDFile)) == NULL)
		return NULL;

	{
		const SnapShotManager *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_SnapShotManager,&a0obj))
		{
			bool res;
			DCDFile *ptr;

			if ((ptr = (DCDFile *)sipGetCppPtr(sipThis,sipClass_DCDFile)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_SnapShotManager(a0obj,(SnapShotManager **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> DCDFile::write(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_DCDFile,sipName_BALL_write);

	return NULL;
}

static PyObject *sipDo_DCDFile_append(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_DCDFile)) == NULL)
		return NULL;

	{
		const SnapShot *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_SnapShot,&a0obj))
		{
			bool res;
			DCDFile *ptr;

			if ((ptr = (DCDFile *)sipGetCppPtr(sipThis,sipClass_DCDFile)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_SnapShot(a0obj,(SnapShot **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> DCDFile::append(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_DCDFile,sipName_BALL_append);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_DCDFile(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_DCDFile)
		return ptr;

	if ((res = sipCast_TrajectoryFile((TrajectoryFile *)(DCDFile *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_DCDFile(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipDCDFile *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (DCDFile *)sipThis -> u.cppPtr;
			else
				delete (sipDCDFile *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_DCDFile(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_DCDFile
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
			sipNew = new sipDCDFile();
	}
	}

	if (sipNew == NULL)
	{
		const DCDFile *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_DCDFile,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_DCDFile(a0obj,(DCDFile **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipDCDFile(* a0);
	}
	}

	if (sipNew == NULL)
	{
		const String *a0;
		PyObject *a0obj;
		OpenMode *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_String,&a0obj,sipCanConvertTo_OpenMode,&a1obj))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_OpenMode(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipDCDFile(* a0,* a1);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_DCDFile);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_DCDFile,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (DCDFile *)sipNew;
			else
				delete (sipDCDFile *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipDCDFile *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_DCDFile[] = {
	{sipName_BALL_clear, sipDo_DCDFile_clear, METH_VARARGS, NULL},
	{sipName_BALL_readHeader, sipDo_DCDFile_readHeader, METH_VARARGS, NULL},
	{sipName_BALL_updateHeader, sipDo_DCDFile_updateHeader, METH_VARARGS, NULL},
	{sipName_BALL_writeHeader, sipDo_DCDFile_writeHeader, METH_VARARGS, NULL},
	{sipName_BALL_read, sipDo_DCDFile_read, METH_VARARGS, NULL},
	{sipName_BALL_write, sipDo_DCDFile_write, METH_VARARGS, NULL},
	{sipName_BALL_append, sipDo_DCDFile_append, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_DCDFile(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_DCDFile);
}

void sipConvertTo_DCDFile(PyObject *sipPy,DCDFile **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_DCDFile);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (DCDFile *)sipConvertToCpp(sipPy,sipClass_DCDFile,sipIsErr);
}

DCDFile *sipForceConvertTo_DCDFile(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_DCDFile(valobj))
	{
		DCDFile *val;

		sipConvertTo_DCDFile(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_DCDFile);
	*iserrp = 1;

	return NULL;
}
