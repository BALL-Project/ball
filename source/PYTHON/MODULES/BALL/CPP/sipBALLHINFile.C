#include "sipBALLDeclBALL.h"
#include "sipBALLHINFile.h"


PyObject *sipClass_HINFile;

static void sipDealloc_HINFile(sipThisType *);

static PyTypeObject sipType_HINFile = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_HINFile,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_HINFile,
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

sipHINFile::sipHINFile()
   : HINFile()
{
	sipCommonCtor(sipPyMethods,2);
}

sipHINFile::sipHINFile(String& a0,std__openmode a1)
   : HINFile(a0,a1)
{
	sipCommonCtor(sipPyMethods,2);
}

sipHINFile::sipHINFile(const HINFile& a0)
   : HINFile(a0)
{
	sipCommonCtor(sipPyMethods,2);
}

sipHINFile::~sipHINFile()

{
	sipCommonDtor(sipPyThis);
}

void sipHINFile::read(System& a0)

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_read,&relLock))
		sipHINFile::sipVH_read(&sipPyMethods[0],sipPyThis,relLock,a0);
	else
		HINFile::read(a0);
}

void sipHINFile::write(System& a0)

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_write,&relLock))
		sipHINFile::sipVH_write(&sipPyMethods[1],sipPyThis,relLock,a0);
	else
		HINFile::write(a0);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipHINFile::sipVH_read(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,System& a0)
{
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_System);

	sipArgs = Py_BuildValue("(ON)",sipThis -> sipSelf,a0obj);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_HINFile,sipName_BALL_read);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipHINFile::sipVH_write(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,System& a0)
{
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_System);

	sipArgs = Py_BuildValue("(ON)",sipThis -> sipSelf,a0obj);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_HINFile,sipName_BALL_write);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

static PyObject *sipDo_HINFile_write(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_HINFile)) == NULL)
		return NULL;

	{
		System * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_System,&a0obj))
		{
			HINFile *ptr;

			if ((ptr = (HINFile *)sipGetCppPtr(sipThis,sipClass_HINFile)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_System(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> HINFile::write(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_HINFile,sipName_BALL_write);

	return NULL;
}

static PyObject *sipDo_HINFile_read(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_HINFile)) == NULL)
		return NULL;

	{
		System * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_System,&a0obj))
		{
			HINFile *ptr;

			if ((ptr = (HINFile *)sipGetCppPtr(sipThis,sipClass_HINFile)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_System(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> HINFile::read(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_HINFile,sipName_BALL_read);

	return NULL;
}

static PyObject *sipDo_HINFile_hasPeriodicBoundary(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_HINFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			HINFile *ptr;

			if ((ptr = (HINFile *)sipGetCppPtr(sipThis,sipClass_HINFile)) == NULL)
				return NULL;

   try
   {
			res = ptr -> HINFile::hasPeriodicBoundary();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_HINFile,sipName_BALL_hasPeriodicBoundary);

	return NULL;
}

static PyObject *sipDo_HINFile_getPeriodicBoundary(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_HINFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Box3 *res;
			HINFile *ptr;

			if ((ptr = (HINFile *)sipGetCppPtr(sipThis,sipClass_HINFile)) == NULL)
				return NULL;

   try
   {
			res = new Box3(ptr -> HINFile::getPeriodicBoundary());
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipNewCppToSelf(res,sipClass_Box3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_HINFile,sipName_BALL_getPeriodicBoundary);

	return NULL;
}

static PyObject *sipDo_HINFile_getTemperature(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_HINFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			HINFile *ptr;

			if ((ptr = (HINFile *)sipGetCppPtr(sipThis,sipClass_HINFile)) == NULL)
				return NULL;

   try
   {
			res = ptr -> HINFile::getTemperature();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_HINFile,sipName_BALL_getTemperature);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_HINFile(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_HINFile)
		return ptr;

	if ((res = sipCast_File((File *)(HINFile *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_HINFile(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipHINFile *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (HINFile *)sipThis -> u.cppPtr;
			else
				delete (sipHINFile *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_HINFile(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_HINFile
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
			sipNew = new sipHINFile();
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
		String * a0;
		PyObject *a0obj;
		std__openmode * a1 = (std__openmode *)&File::IN;
		PyObject *a1obj = NULL;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|I",sipCanConvertTo_String,&a0obj,sipCanConvertTo_std__openmode,&a1obj))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,&a0,1,&iserr);
			sipConvertTo_std__openmode(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipHINFile(* a0,* a1);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;
		}
	}

	if (sipNew == NULL)
	{
		const HINFile * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_HINFile,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_HINFile(a0obj,(HINFile **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipHINFile(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_HINFile);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_HINFile,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (HINFile *)sipNew;
			else
				delete (sipHINFile *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipHINFile *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_HINFile[] = {
	{sipName_BALL_write, sipDo_HINFile_write, METH_VARARGS, NULL},
	{sipName_BALL_read, sipDo_HINFile_read, METH_VARARGS, NULL},
	{sipName_BALL_hasPeriodicBoundary, sipDo_HINFile_hasPeriodicBoundary, METH_VARARGS, NULL},
	{sipName_BALL_getPeriodicBoundary, sipDo_HINFile_getPeriodicBoundary, METH_VARARGS, NULL},
	{sipName_BALL_getTemperature, sipDo_HINFile_getTemperature, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_HINFile(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_HINFile);
}

void sipConvertTo_HINFile(PyObject *sipPy,HINFile **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_HINFile);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (HINFile *)sipConvertToCpp(sipPy,sipClass_HINFile,sipIsErr);
}

HINFile *sipForceConvertTo_HINFile(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_HINFile(valobj))
	{
		HINFile *val;

		sipConvertTo_HINFile(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_HINFile);

	*iserrp = 1;

	return NULL;
}
