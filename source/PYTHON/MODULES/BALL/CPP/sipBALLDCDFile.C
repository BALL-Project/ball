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

sipDCDFile::sipDCDFile()
   throw() : DCDFile()
{
	sipCommonCtor(sipPyMethods,5);
}

sipDCDFile::sipDCDFile(const DCDFile& a0)
   throw(FileNotFound) : DCDFile(a0)
{
	sipCommonCtor(sipPyMethods,5);
}

sipDCDFile::sipDCDFile(const String& a0,std__openmode a1)
   throw() : DCDFile(a0,a1)
{
	sipCommonCtor(sipPyMethods,5);
}

sipDCDFile::~sipDCDFile()
 throw()
{
	sipCommonDtor(sipPyThis);
}

bool sipDCDFile::read(SnapShot& a0)
 throw()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_read,&relLock) ?
		sipTrajectoryFile::sipVH_read(&sipPyMethods[0],sipPyThis,relLock,a0) :
		DCDFile::read(a0);
}

bool sipDCDFile::append(const SnapShot& a0)
 throw()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_append,&relLock) ?
		sipTrajectoryFile::sipVH_append(&sipPyMethods[1],sipPyThis,relLock,a0) :
		DCDFile::append(a0);
}

bool sipDCDFile::writeHeader()
 throw()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_writeHeader,&relLock) ?
		sipTrajectoryFile::sipVH_writeHeader(&sipPyMethods[2],sipPyThis,relLock) :
		DCDFile::writeHeader();
}

bool sipDCDFile::readHeader()
 throw()
{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_readHeader,&relLock) ?
		sipTrajectoryFile::sipVH_readHeader(&sipPyMethods[3],sipPyThis,relLock) :
		DCDFile::readHeader();
}

void sipDCDFile::clear()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipTrajectoryFile::sipVH_clear(&sipPyMethods[4],sipPyThis,relLock);
	else
		DCDFile::clear();
}

static PyObject *sipDo_DCDFile_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_DCDFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
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

	sipNoMethod(sipArgsParsed,sipName_BALL_DCDFile,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_DCDFile_CmpOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_DCDFile)) == NULL)
		return NULL;

	{
		const DCDFile * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_DCDFile,&a0obj))
		{
			bool res;
			DCDFile *ptr;

			if ((ptr = (DCDFile *)sipGetCppPtr(sipThis,sipClass_DCDFile)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_DCDFile(a0obj,(DCDFile **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> DCDFile::operator==(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_DCDFile,sipName_BALL_CmpOp);

	return NULL;
}

static PyObject *sipDo_DCDFile_isSwappingBytes(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_DCDFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			DCDFile *ptr;

			if ((ptr = (DCDFile *)sipGetCppPtr(sipThis,sipClass_DCDFile)) == NULL)
				return NULL;

			res = ptr -> DCDFile::isSwappingBytes();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_DCDFile,sipName_BALL_isSwappingBytes);

	return NULL;
}

static PyObject *sipDo_DCDFile_hasVelocities(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_DCDFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			DCDFile *ptr;

			if ((ptr = (DCDFile *)sipGetCppPtr(sipThis,sipClass_DCDFile)) == NULL)
				return NULL;

			res = ptr -> DCDFile::hasVelocities();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_DCDFile,sipName_BALL_hasVelocities);

	return NULL;
}

static PyObject *sipDo_DCDFile_init(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_DCDFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			DCDFile *ptr;

			if ((ptr = (DCDFile *)sipGetCppPtr(sipThis,sipClass_DCDFile)) == NULL)
				return NULL;

			res = ptr -> DCDFile::init();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_DCDFile,sipName_BALL_init);

	return NULL;
}

static PyObject *sipDo_DCDFile_readHeader(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_DCDFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
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

	sipNoMethod(sipArgsParsed,sipName_BALL_DCDFile,sipName_BALL_readHeader);

	return NULL;
}

static PyObject *sipDo_DCDFile_writeHeader(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_DCDFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
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

	sipNoMethod(sipArgsParsed,sipName_BALL_DCDFile,sipName_BALL_writeHeader);

	return NULL;
}

static PyObject *sipDo_DCDFile_append(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_DCDFile)) == NULL)
		return NULL;

	{
		const SnapShot * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_SnapShot,&a0obj))
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

	sipNoMethod(sipArgsParsed,sipName_BALL_DCDFile,sipName_BALL_append);

	return NULL;
}

static PyObject *sipDo_DCDFile_read(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_DCDFile)) == NULL)
		return NULL;

	{
		SnapShot * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_SnapShot,&a0obj))
		{
			bool res;
			DCDFile *ptr;

			if ((ptr = (DCDFile *)sipGetCppPtr(sipThis,sipClass_DCDFile)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_SnapShot(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> DCDFile::read(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_DCDFile,sipName_BALL_read);

	return NULL;
}

static PyObject *sipDo_DCDFile_getHeader(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_DCDFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const DCDHeader *res;
			DCDFile *ptr;

			if ((ptr = (DCDFile *)sipGetCppPtr(sipThis,sipClass_DCDFile)) == NULL)
				return NULL;

			res = &ptr -> DCDFile::getHeader();

			return sipMapCppToSelf(res,sipClass_DCDHeader);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_DCDFile,sipName_BALL_getHeader);

	return NULL;
}

static PyObject *sipDo_DCDFile_setHeader(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_DCDFile)) == NULL)
		return NULL;

	{
		const DCDHeader * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_DCDHeader,&a0obj))
		{
			DCDFile *ptr;

			if ((ptr = (DCDFile *)sipGetCppPtr(sipThis,sipClass_DCDFile)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_DCDHeader(a0obj,(DCDHeader **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> DCDFile::setHeader(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_DCDFile,sipName_BALL_setHeader);

	return NULL;
}

static PyObject *sipDo_DCDFile_enableVelocityStorage(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_DCDFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			DCDFile *ptr;

			if ((ptr = (DCDFile *)sipGetCppPtr(sipThis,sipClass_DCDFile)) == NULL)
				return NULL;

			ptr -> DCDFile::enableVelocityStorage();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_DCDFile,sipName_BALL_enableVelocityStorage);

	return NULL;
}

static PyObject *sipDo_DCDFile_disableVelocityStorage(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_DCDFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			DCDFile *ptr;

			if ((ptr = (DCDFile *)sipGetCppPtr(sipThis,sipClass_DCDFile)) == NULL)
				return NULL;

			ptr -> DCDFile::disableVelocityStorage();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_DCDFile,sipName_BALL_disableVelocityStorage);

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

	if ((res = sipCast_FileNotFound((FileNotFound *)(DCDFile *)ptr,targetClass)) != NULL)
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
	int sipArgsParsed = 0;

	// See if there is something pending.

	sipNew = sipGetPending(&sipFlags);

	if (sipNew == NULL)
	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,"-"))
		{
			sipNew = new sipDCDFile();
		}
	}

	if (sipNew == NULL)
	{
		const DCDFile * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_DCDFile,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_DCDFile(a0obj,(DCDFile **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipDCDFile(* a0);
   }
   catch (FileNotFound e)
   {
      FileNotFound *my_exception = new FileNotFound(e);
      PyErr_SetObject(sipClass_FileNotFound, sipNewCppToSelf(my_exception,sipClass_FileNotFound,SIP_SIMPLE | SIP_PY_OWNED));
      return NULL;
		}
		}
	}

	if (sipNew == NULL)
	{
		const String * a0;
		PyObject *a0obj;
		std__openmode * a1 = (std__openmode *)&File::IN;
		PyObject *a1obj = NULL;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|I",sipCanConvertTo_String,&a0obj,sipCanConvertTo_std__openmode,&a1obj))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			sipConvertTo_std__openmode(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipDCDFile(* a0,* a1);

			if (istemp0)
				delete a0;
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_DCDFile);
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
	{sipName_BALL_CmpOp, sipDo_DCDFile_CmpOp, METH_VARARGS, NULL},
	{sipName_BALL_isSwappingBytes, sipDo_DCDFile_isSwappingBytes, METH_VARARGS, NULL},
	{sipName_BALL_hasVelocities, sipDo_DCDFile_hasVelocities, METH_VARARGS, NULL},
	{sipName_BALL_init, sipDo_DCDFile_init, METH_VARARGS, NULL},
	{sipName_BALL_readHeader, sipDo_DCDFile_readHeader, METH_VARARGS, NULL},
	{sipName_BALL_writeHeader, sipDo_DCDFile_writeHeader, METH_VARARGS, NULL},
	{sipName_BALL_append, sipDo_DCDFile_append, METH_VARARGS, NULL},
	{sipName_BALL_read, sipDo_DCDFile_read, METH_VARARGS, NULL},
	{sipName_BALL_getHeader, sipDo_DCDFile_getHeader, METH_VARARGS, NULL},
	{sipName_BALL_setHeader, sipDo_DCDFile_setHeader, METH_VARARGS, NULL},
	{sipName_BALL_enableVelocityStorage, sipDo_DCDFile_enableVelocityStorage, METH_VARARGS, NULL},
	{sipName_BALL_disableVelocityStorage, sipDo_DCDFile_disableVelocityStorage, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_DCDFile(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_DCDFile);
}

void sipConvertTo_DCDFile(PyObject *sipPy,DCDFile **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_DCDFile);
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
