#include "sipBALLDeclBALL.h"
#include "sipBALLPDBFile.h"



PyObject *sipClass_PDBFile;

static void sipDealloc_PDBFile(sipThisType *);

static PyTypeObject sipType_PDBFile = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_PDBFile,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_PDBFile,
	0,
	0,
	0,
	0,
	0,
};

sipPDBFile::sipPDBFile(): PDBFile()
{
	sipCommonCtor(sipPyMethods,2);
}

sipPDBFile::sipPDBFile(const String& a0,OpenMode a1): PDBFile(a0,a1)
{
	sipCommonCtor(sipPyMethods,2);
}

sipPDBFile::sipPDBFile(const PDBFile& a0): PDBFile(a0)
{
	sipCommonCtor(sipPyMethods,2);
}

sipPDBFile::~sipPDBFile()
{
	sipCommonDtor(sipPyThis);
}
void sipPDBFile::read(System& a0)
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_read,&relLock))
		sipPDBFile::sipVH_read(&sipPyMethods[0],sipPyThis,relLock,a0);
	else
		PDBFile::read(a0);
}
void sipPDBFile::read(Protein& a0)
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_read,&relLock))
		sipPDBFile::sipVH_read(&sipPyMethods[0],sipPyThis,relLock,a0);
	else
		PDBFile::read(a0);
}
void sipPDBFile::write(const System& a0)
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_write,&relLock))
		sipPDBFile::sipVH_write(&sipPyMethods[1],sipPyThis,relLock,a0);
	else
		PDBFile::write(a0);
}
void sipPDBFile::write(const Protein& a0)
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_write,&relLock))
		sipPDBFile::sipVH_write(&sipPyMethods[1],sipPyThis,relLock,a0);
	else
		PDBFile::write(a0);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipPDBFile::sipVH_read(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,System& a0)
{
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_System);

	sipArgs = Py_BuildValue("(OO)",sipThis -> sipSelf,a0obj);

	Py_XDECREF(a0obj);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_PDBFile,sipName_BALL_read);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipPDBFile::sipVH_read(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,Protein& a0)
{
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_Protein);

	sipArgs = Py_BuildValue("(OO)",sipThis -> sipSelf,a0obj);

	Py_XDECREF(a0obj);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_PDBFile,sipName_BALL_read);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipPDBFile::sipVH_write(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,const System& a0)
{
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_System);

	sipArgs = Py_BuildValue("(OO)",sipThis -> sipSelf,a0obj);

	Py_XDECREF(a0obj);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_PDBFile,sipName_BALL_write);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipPDBFile::sipVH_write(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,const Protein& a0)
{
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_Protein);

	sipArgs = Py_BuildValue("(OO)",sipThis -> sipSelf,a0obj);

	Py_XDECREF(a0obj);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_PDBFile,sipName_BALL_write);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

static PyObject *sipDo_PDBFile_read(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PDBFile)) == NULL)
		return NULL;

	{
		Protein *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Protein,&a0obj))
		{
			PDBFile *ptr;

			if ((ptr = (PDBFile *)sipGetCppPtr(sipThis,sipClass_PDBFile)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Protein(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> PDBFile::read(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		System *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_System,&a0obj))
		{
			PDBFile *ptr;

			if ((ptr = (PDBFile *)sipGetCppPtr(sipThis,sipClass_PDBFile)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_System(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> PDBFile::read(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PDBFile,sipName_BALL_read);

	return NULL;
}

static PyObject *sipDo_PDBFile_write(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PDBFile)) == NULL)
		return NULL;

	{
		const Protein *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Protein,&a0obj))
		{
			PDBFile *ptr;

			if ((ptr = (PDBFile *)sipGetCppPtr(sipThis,sipClass_PDBFile)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Protein(a0obj,(Protein **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> PDBFile::write(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const System *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_System,&a0obj))
		{
			PDBFile *ptr;

			if ((ptr = (PDBFile *)sipGetCppPtr(sipThis,sipClass_PDBFile)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_System(a0obj,(System **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> PDBFile::write(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PDBFile,sipName_BALL_write);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_PDBFile(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_PDBFile)
		return ptr;

	return NULL;
}

static void sipDealloc_PDBFile(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipPDBFile *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (PDBFile *)sipThis -> u.cppPtr;
			else
				delete (sipPDBFile *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_PDBFile(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_PDBFile
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
			sipNew = new sipPDBFile();
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

			sipNew = new sipPDBFile(* a0,* a1);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;
	}
	}

	if (sipNew == NULL)
	{
		const PDBFile *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_PDBFile,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_PDBFile(a0obj,(PDBFile **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipPDBFile(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_PDBFile);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_PDBFile,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (PDBFile *)sipNew;
			else
				delete (sipPDBFile *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipPDBFile *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_PDBFile[] = {
	{sipName_BALL_read, sipDo_PDBFile_read, METH_VARARGS, NULL},
	{sipName_BALL_write, sipDo_PDBFile_write, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_PDBFile(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_PDBFile);
}

void sipConvertTo_PDBFile(PyObject *sipPy,PDBFile **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_PDBFile);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (PDBFile *)sipConvertToCpp(sipPy,sipClass_PDBFile,sipIsErr);
}

PDBFile *sipForceConvertTo_PDBFile(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_PDBFile(valobj))
	{
		PDBFile *val;

		sipConvertTo_PDBFile(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_PDBFile);
	*iserrp = 1;

	return NULL;
}
