#include "sipBALLDeclBALL.h"
#include "sipBALLGenericPDBFile.h"


PyObject *sipClass_GenericPDBFile;

static void sipDealloc_GenericPDBFile(sipThisType *);

static PyTypeObject sipType_GenericPDBFile = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_GenericPDBFile,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_GenericPDBFile,
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

sipGenericPDBFile::sipGenericPDBFile()
   : GenericPDBFile()
{
	sipCommonCtor(sipPyMethods,2);
}

sipGenericPDBFile::sipGenericPDBFile(const GenericPDBFile& a0)
   : GenericPDBFile(a0)
{
	sipCommonCtor(sipPyMethods,2);
}

sipGenericPDBFile::~sipGenericPDBFile()
 throw()
{
	sipCommonDtor(sipPyThis);
}

float sipGenericPDBFile::getVersion() const

{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_getVersion,&relLock) ?
		sipGenericPDBFile::sipVH_getVersion(&sipPyMethods[0],sipPyThis,relLock) :
		GenericPDBFile::getVersion();
}

void sipGenericPDBFile::clear(int a0)

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipGenericPDBFile::sipVH_clear(&sipPyMethods[1],sipPyThis,relLock,a0);
	else
		GenericPDBFile::clear(a0);
}

// The common handler for all classes that inherit this virtual member
// function.

float sipGenericPDBFile::sipVH_getVersion(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
{
	float res;
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(O)",sipThis -> sipSelf);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		res = (float)PyFloat_AsDouble(resobj);

		Py_DECREF(resobj);

		if (PyErr_Occurred() == NULL)
		{
			goto releaseLock;
		}

		sipBadVirtualResultType(sipName_BALL_GenericPDBFile,sipName_BALL_getVersion);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

void sipGenericPDBFile::sipVH_clear(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,int a0)
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

		sipBadVirtualResultType(sipName_BALL_GenericPDBFile,sipName_BALL_clear);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

extern "C" PyObject *sipDo_GenericPDBFile_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_GenericPDBFile)) == NULL)
		return NULL;

	{
		int a0 = 0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"|i",&a0))
		{
			GenericPDBFile *ptr;

			if ((ptr = (GenericPDBFile *)sipGetCppPtr(sipThis,sipClass_GenericPDBFile)) == NULL)
				return NULL;

   try
   {
			ptr -> GenericPDBFile::clear( a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_GenericPDBFile,sipName_BALL_clear);

	return NULL;
}

extern "C" PyObject *sipDo_GenericPDBFile_getVersion(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_GenericPDBFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			GenericPDBFile *ptr;

			if ((ptr = (GenericPDBFile *)sipGetCppPtr(sipThis,sipClass_GenericPDBFile)) == NULL)
				return NULL;

   try
   {
			res = ptr -> GenericPDBFile::getVersion();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_GenericPDBFile,sipName_BALL_getVersion);

	return NULL;
}

extern "C" PyObject *sipDo_GenericPDBFile_selectModel(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_GenericPDBFile)) == NULL)
		return NULL;

	{
		Index * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Index,&a0obj))
		{
			GenericPDBFile *ptr;

			if ((ptr = (GenericPDBFile *)sipGetCppPtr(sipThis,sipClass_GenericPDBFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> GenericPDBFile::selectModel(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_GenericPDBFile,sipName_BALL_selectModel);

	return NULL;
}

extern "C" PyObject *sipDo_GenericPDBFile_selectAllModels(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_GenericPDBFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			GenericPDBFile *ptr;

			if ((ptr = (GenericPDBFile *)sipGetCppPtr(sipThis,sipClass_GenericPDBFile)) == NULL)
				return NULL;

   try
   {
			ptr -> GenericPDBFile::selectAllModels();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_GenericPDBFile,sipName_BALL_selectAllModels);

	return NULL;
}

extern "C" PyObject *sipDo_GenericPDBFile_getSelectedModel(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_GenericPDBFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Index *res;
			GenericPDBFile *ptr;

			if ((ptr = (GenericPDBFile *)sipGetCppPtr(sipThis,sipClass_GenericPDBFile)) == NULL)
				return NULL;

   try
   {
			res = new Index(ptr -> GenericPDBFile::getSelectedModel());
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			PyObject *resobj = sipConvertFrom_Index(res);
			delete res;

			return resobj;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_GenericPDBFile,sipName_BALL_getSelectedModel);

	return NULL;
}

extern "C" PyObject *sipDo_GenericPDBFile_getCurrentModel(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_GenericPDBFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Index *res;
			GenericPDBFile *ptr;

			if ((ptr = (GenericPDBFile *)sipGetCppPtr(sipThis,sipClass_GenericPDBFile)) == NULL)
				return NULL;

   try
   {
			res = new Index(ptr -> GenericPDBFile::getCurrentModel());
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			PyObject *resobj = sipConvertFrom_Index(res);
			delete res;

			return resobj;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_GenericPDBFile,sipName_BALL_getCurrentModel);

	return NULL;
}

extern "C" PyObject *sipDo_GenericPDBFile_getRecordString(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_GenericPDBFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			char *res;
			GenericPDBFile *ptr;

			if ((ptr = (GenericPDBFile *)sipGetCppPtr(sipThis,sipClass_GenericPDBFile)) == NULL)
				return NULL;

   try
   {
			res = ptr -> GenericPDBFile::getRecordString();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (res == NULL)
			{
				Py_INCREF(Py_None);
				return Py_None;
			}

			return PyString_FromString(res);
		}
	}

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const char *res;
			GenericPDBFile *ptr;

			if ((ptr = (GenericPDBFile *)sipGetCppPtr(sipThis,sipClass_GenericPDBFile)) == NULL)
				return NULL;

   try
   {
			res = ptr -> GenericPDBFile::getRecordString();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (res == NULL)
			{
				Py_INCREF(Py_None);
				return Py_None;
			}

			return PyString_FromString(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_GenericPDBFile,sipName_BALL_getRecordString);

	return NULL;
}

extern "C" PyObject *sipDo_GenericPDBFile_getRecordNumber(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_GenericPDBFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Index *res;
			GenericPDBFile *ptr;

			if ((ptr = (GenericPDBFile *)sipGetCppPtr(sipThis,sipClass_GenericPDBFile)) == NULL)
				return NULL;

   try
   {
			res = new Index(ptr -> GenericPDBFile::getRecordNumber());
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			PyObject *resobj = sipConvertFrom_Index(res);
			delete res;

			return resobj;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_GenericPDBFile,sipName_BALL_getRecordNumber);

	return NULL;
}

extern "C" PyObject *sipDo_GenericPDBFile_readLine(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_GenericPDBFile)) == NULL)
		return NULL;

	{
		char * a0;
		int a1;
		long a2;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"sil",&a0,&a1,&a2))
		{
			bool res;
			GenericPDBFile *ptr;

			if ((ptr = (GenericPDBFile *)sipGetCppPtr(sipThis,sipClass_GenericPDBFile)) == NULL)
				return NULL;

   try
   {
			res = ptr -> GenericPDBFile::readLine( a0, a1, (bool)a2);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_GenericPDBFile,sipName_BALL_readLine);

	return NULL;
}

extern "C" PyObject *sipDo_GenericPDBFile_readFirstRecord(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_GenericPDBFile)) == NULL)
		return NULL;

	{
		long a0 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"|l",&a0))
		{
			bool res;
			GenericPDBFile *ptr;

			if ((ptr = (GenericPDBFile *)sipGetCppPtr(sipThis,sipClass_GenericPDBFile)) == NULL)
				return NULL;

   try
   {
			res = ptr -> GenericPDBFile::readFirstRecord( (bool)a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_GenericPDBFile,sipName_BALL_readFirstRecord);

	return NULL;
}

extern "C" PyObject *sipDo_GenericPDBFile_readNextRecord(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_GenericPDBFile)) == NULL)
		return NULL;

	{
		long a0 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"|l",&a0))
		{
			bool res;
			GenericPDBFile *ptr;

			if ((ptr = (GenericPDBFile *)sipGetCppPtr(sipThis,sipClass_GenericPDBFile)) == NULL)
				return NULL;

   try
   {
			res = ptr -> GenericPDBFile::readNextRecord( (bool)a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_GenericPDBFile,sipName_BALL_readNextRecord);

	return NULL;
}

extern "C" PyObject *sipDo_GenericPDBFile_readRecords(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_GenericPDBFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			GenericPDBFile *ptr;

			if ((ptr = (GenericPDBFile *)sipGetCppPtr(sipThis,sipClass_GenericPDBFile)) == NULL)
				return NULL;

   try
   {
			res = ptr -> GenericPDBFile::readRecords();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_GenericPDBFile,sipName_BALL_readRecords);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_GenericPDBFile(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_GenericPDBFile)
		return ptr;

	if ((res = sipCast_File((File *)(GenericPDBFile *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_PropertyManager((PropertyManager *)(GenericPDBFile *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_GenericPDBFile(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipGenericPDBFile *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (GenericPDBFile *)sipThis -> u.cppPtr;
			else
				delete (sipGenericPDBFile *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_GenericPDBFile(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_GenericPDBFile
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
			sipNew = new sipGenericPDBFile();
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
		const GenericPDBFile * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_GenericPDBFile,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_GenericPDBFile(a0obj,(GenericPDBFile **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipGenericPDBFile(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_GenericPDBFile);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_GenericPDBFile,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (GenericPDBFile *)sipNew;
			else
				delete (sipGenericPDBFile *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipGenericPDBFile *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_GenericPDBFile[] = {
	{sipName_BALL_clear, sipDo_GenericPDBFile_clear, METH_VARARGS, NULL},
	{sipName_BALL_getVersion, sipDo_GenericPDBFile_getVersion, METH_VARARGS, NULL},
	{sipName_BALL_selectModel, sipDo_GenericPDBFile_selectModel, METH_VARARGS, NULL},
	{sipName_BALL_selectAllModels, sipDo_GenericPDBFile_selectAllModels, METH_VARARGS, NULL},
	{sipName_BALL_getSelectedModel, sipDo_GenericPDBFile_getSelectedModel, METH_VARARGS, NULL},
	{sipName_BALL_getCurrentModel, sipDo_GenericPDBFile_getCurrentModel, METH_VARARGS, NULL},
	{sipName_BALL_getRecordString, sipDo_GenericPDBFile_getRecordString, METH_VARARGS, NULL},
	{sipName_BALL_getRecordNumber, sipDo_GenericPDBFile_getRecordNumber, METH_VARARGS, NULL},
	{sipName_BALL_readLine, sipDo_GenericPDBFile_readLine, METH_VARARGS, NULL},
	{sipName_BALL_readFirstRecord, sipDo_GenericPDBFile_readFirstRecord, METH_VARARGS, NULL},
	{sipName_BALL_readNextRecord, sipDo_GenericPDBFile_readNextRecord, METH_VARARGS, NULL},
	{sipName_BALL_readRecords, sipDo_GenericPDBFile_readRecords, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_GenericPDBFile(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_GenericPDBFile);
}

void sipConvertTo_GenericPDBFile(PyObject *sipPy,GenericPDBFile **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_GenericPDBFile);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (GenericPDBFile *)sipConvertToCpp(sipPy,sipClass_GenericPDBFile,sipIsErr);
}

GenericPDBFile *sipForceConvertTo_GenericPDBFile(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_GenericPDBFile(valobj))
	{
		GenericPDBFile *val;

		sipConvertTo_GenericPDBFile(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_GenericPDBFile);

	*iserrp = 1;

	return NULL;
}
