#include "sipBALLDeclBALL.h"
#include "sipBALLFileNotFound.h"


PyObject *sipClass_FileNotFound;

static void sipDealloc_FileNotFound(sipThisType *);

static PyTypeObject sipType_FileNotFound = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_FileNotFound,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_FileNotFound,
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

extern "C" PyObject *sipDo_FileNotFound_getFilename(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FileNotFound)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			String *res;
			FileNotFound *ptr;

			if ((ptr = (FileNotFound *)sipGetCppPtr(sipThis,sipClass_FileNotFound)) == NULL)
				return NULL;

   try
   {
			res = new String(ptr -> FileNotFound::getFilename());
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipNewCppToSelf(res,sipClass_String,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FileNotFound,sipName_BALL_getFilename);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_FileNotFound(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_FileNotFound)
		return ptr;

	if ((res = sipCast_GeneralException((GeneralException *)(FileNotFound *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_FileNotFound(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (FileNotFound *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_FileNotFound(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_FileNotFound
	};

	sipThisType *sipThis = NULL;
	const void *sipNew = NULL;
	int sipFlags = SIP_PY_OWNED;
	int sipArgsParsed = 0;

	// See if there is something pending.

	sipNew = sipGetPending(&sipFlags);

	if (sipNew == NULL)
	{
		const char * a0;
		int a1;
		const String * a2;
		PyObject *a2obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-siI",&a0,&a1,sipCanConvertTo_String,&a2obj))
		{
			int iserr = 0;

			int istemp2 = sipConvertTo_String(a2obj,(String **)&a2,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new FileNotFound( a0, a1,* a2);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp2)
				delete a2;
		}
	}

	if (sipNew == NULL)
	{
		const FileNotFound * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_FileNotFound,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_FileNotFound(a0obj,(FileNotFound **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new FileNotFound(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_FileNotFound);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_FileNotFound,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (FileNotFound *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_FileNotFound[] = {
	{sipName_BALL_getFilename, sipDo_FileNotFound_getFilename, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_FileNotFound(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_FileNotFound);
}

void sipConvertTo_FileNotFound(PyObject *sipPy,FileNotFound **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_FileNotFound);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (FileNotFound *)sipConvertToCpp(sipPy,sipClass_FileNotFound,sipIsErr);
}

FileNotFound *sipForceConvertTo_FileNotFound(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_FileNotFound(valobj))
	{
		FileNotFound *val;

		sipConvertTo_FileNotFound(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_FileNotFound);

	*iserrp = 1;

	return NULL;
}
