#include "sipBALLDeclBALL.h"
#include "sipBALLMOL2File.h"



PyObject *sipClass_MOL2File;

static void sipDealloc_MOL2File(sipThisType *);

static PyTypeObject sipType_MOL2File = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_MOL2File,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_MOL2File,
	0,
	0,
	0,
	0,
	0,
};

sipMOL2File::sipMOL2File(): MOL2File()
{
	sipCommonCtor(sipPyMethods,2);
}

sipMOL2File::sipMOL2File(const String& a0,OpenMode a1): MOL2File(a0,a1)
{
	sipCommonCtor(sipPyMethods,2);
}

sipMOL2File::sipMOL2File(const MOL2File& a0): MOL2File(a0)
{
	sipCommonCtor(sipPyMethods,2);
}

sipMOL2File::~sipMOL2File()
{
	sipCommonDtor(sipPyThis);
}
void sipMOL2File::write(const System& a0)
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_write,&relLock))
		sipMOL2File::sipVH_write(&sipPyMethods[0],sipPyThis,relLock,a0);
	else
		MOL2File::write(a0);
}
void sipMOL2File::read(System& a0)
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_read,&relLock))
		sipMOL2File::sipVH_read(&sipPyMethods[1],sipPyThis,relLock,a0);
	else
		MOL2File::read(a0);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipMOL2File::sipVH_write(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,const System& a0)
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

		sipBadVirtualResultType(sipName_BALL_MOL2File,sipName_BALL_write);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipMOL2File::sipVH_read(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,System& a0)
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

		sipBadVirtualResultType(sipName_BALL_MOL2File,sipName_BALL_read);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

static PyObject *sipDo_MOL2File_write(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MOL2File)) == NULL)
		return NULL;

	{
		const System *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_System,&a0obj))
		{
			MOL2File *ptr;

			if ((ptr = (MOL2File *)sipGetCppPtr(sipThis,sipClass_MOL2File)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_System(a0obj,(System **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> MOL2File::write(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MOL2File,sipName_BALL_write);

	return NULL;
}

static PyObject *sipDo_MOL2File_read(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MOL2File)) == NULL)
		return NULL;

	{
		System *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_System,&a0obj))
		{
			MOL2File *ptr;

			if ((ptr = (MOL2File *)sipGetCppPtr(sipThis,sipClass_MOL2File)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_System(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> MOL2File::read(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_MOL2File,sipName_BALL_read);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_MOL2File(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_MOL2File)
		return ptr;

	if ((res = sipCast_File((File *)(MOL2File *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_MOL2File(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipMOL2File *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (MOL2File *)sipThis -> u.cppPtr;
			else
				delete (sipMOL2File *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_MOL2File(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_MOL2File
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
			sipNew = new sipMOL2File();
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

			sipNew = new sipMOL2File(* a0,* a1);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;
	}
	}

	if (sipNew == NULL)
	{
		const MOL2File *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_MOL2File,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_MOL2File(a0obj,(MOL2File **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipMOL2File(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_MOL2File);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_MOL2File,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (MOL2File *)sipNew;
			else
				delete (sipMOL2File *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipMOL2File *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_MOL2File[] = {
	{sipName_BALL_write, sipDo_MOL2File_write, METH_VARARGS, NULL},
	{sipName_BALL_read, sipDo_MOL2File_read, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_MOL2File(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_MOL2File);
}

void sipConvertTo_MOL2File(PyObject *sipPy,MOL2File **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_MOL2File);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (MOL2File *)sipConvertToCpp(sipPy,sipClass_MOL2File,sipIsErr);
}

MOL2File *sipForceConvertTo_MOL2File(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_MOL2File(valobj))
	{
		MOL2File *val;

		sipConvertTo_MOL2File(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_MOL2File);
	*iserrp = 1;

	return NULL;
}
