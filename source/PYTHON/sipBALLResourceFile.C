#include "sipBALLDeclBALL.h"
#include "sipBALLResourceFile.h"



PyObject *sipClass_ResourceFile;

static void sipDealloc_ResourceFile(sipThisType *);

static PyTypeObject sipType_ResourceFile = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_ResourceFile,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_ResourceFile,
	0,
	0,
	0,
	0,
	0,
};

static PyObject *sipDo_ResourceFile_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			ResourceFile *ptr;

			if ((ptr = (ResourceFile *)sipGetCppPtr(sipThis,sipClass_ResourceFile)) == NULL)
				return NULL;

			ptr -> ResourceFile::destroy();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_ResourceFile,sipName_BALL_destroy);

	return NULL;
}

static PyObject *sipDo_ResourceFile_open(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceFile)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			bool res;
			ResourceFile *ptr;

			if ((ptr = (ResourceFile *)sipGetCppPtr(sipThis,sipClass_ResourceFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceFile::open(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_ResourceFile,sipName_BALL_open);

	return NULL;
}

static PyObject *sipDo_ResourceFile_close(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			ResourceFile *ptr;

			if ((ptr = (ResourceFile *)sipGetCppPtr(sipThis,sipClass_ResourceFile)) == NULL)
				return NULL;

			ptr -> ResourceFile::close();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_ResourceFile,sipName_BALL_close);

	return NULL;
}

static PyObject *sipDo_ResourceFile_saveAs(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceFile)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			ResourceFile *ptr;

			if ((ptr = (ResourceFile *)sipGetCppPtr(sipThis,sipClass_ResourceFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> ResourceFile::saveAs(* a0);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_ResourceFile,sipName_BALL_saveAs);

	return NULL;
}

static PyObject *sipDo_ResourceFile_save(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceFile)) == NULL)
		return NULL;

	{
		const ResourceEntry *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_ResourceEntry,&a0obj))
		{
			ResourceFile *ptr;

			if ((ptr = (ResourceFile *)sipGetCppPtr(sipThis,sipClass_ResourceFile)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_ResourceEntry(a0obj,(ResourceEntry **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> ResourceFile::save(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			ResourceFile *ptr;

			if ((ptr = (ResourceFile *)sipGetCppPtr(sipThis,sipClass_ResourceFile)) == NULL)
				return NULL;

			ptr -> ResourceFile::save();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_ResourceFile,sipName_BALL_save);

	return NULL;
}

static PyObject *sipDo_ResourceFile_getSize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Size *res;
			ResourceFile *ptr;

			if ((ptr = (ResourceFile *)sipGetCppPtr(sipThis,sipClass_ResourceFile)) == NULL)
				return NULL;

			res = new Size(ptr -> ResourceFile::getSize());

			return sipNewCppToSelf(res,sipClass_Size,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_ResourceFile,sipName_BALL_getSize);

	return NULL;
}

static PyObject *sipDo_ResourceFile_getRoot(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			ResourceEntry *res;
			ResourceFile *ptr;

			if ((ptr = (ResourceFile *)sipGetCppPtr(sipThis,sipClass_ResourceFile)) == NULL)
				return NULL;

			res = &ptr -> ResourceFile::getRoot();

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_ResourceFile,sipName_BALL_getRoot);

	return NULL;
}

static PyObject *sipDo_ResourceFile_getEntry(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceFile)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			ResourceEntry *res;
			ResourceFile *ptr;

			if ((ptr = (ResourceFile *)sipGetCppPtr(sipThis,sipClass_ResourceFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceFile::getEntry(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			const ResourceEntry *res;
			ResourceFile *ptr;

			if ((ptr = (ResourceFile *)sipGetCppPtr(sipThis,sipClass_ResourceFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceFile::getEntry(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_ResourceFile,sipName_BALL_getEntry);

	return NULL;
}

static PyObject *sipDo_ResourceFile_getValue(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceFile)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			String *res;
			ResourceFile *ptr;

			if ((ptr = (ResourceFile *)sipGetCppPtr(sipThis,sipClass_ResourceFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceFile::getValue(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			const String *res;
			ResourceFile *ptr;

			if ((ptr = (ResourceFile *)sipGetCppPtr(sipThis,sipClass_ResourceFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceFile::getValue(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_ResourceFile,sipName_BALL_getValue);

	return NULL;
}

static PyObject *sipDo_ResourceFile_insert(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceFile)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		const String *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_String,&a0obj,sipCanConvertTo_String,&a1obj))
		{
			ResourceEntry *res;
			ResourceFile *ptr;

			if ((ptr = (ResourceFile *)sipGetCppPtr(sipThis,sipClass_ResourceFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceFile::insert(* a0,* a1);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_ResourceFile,sipName_BALL_insert);

	return NULL;
}

static PyObject *sipDo_ResourceFile_removeKey(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceFile)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			bool res;
			ResourceFile *ptr;

			if ((ptr = (ResourceFile *)sipGetCppPtr(sipThis,sipClass_ResourceFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceFile::removeKey(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_ResourceFile,sipName_BALL_removeKey);

	return NULL;
}

static PyObject *sipDo_ResourceFile_hasKey(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceFile)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			bool res;
			ResourceFile *ptr;

			if ((ptr = (ResourceFile *)sipGetCppPtr(sipThis,sipClass_ResourceFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceFile::hasKey(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_ResourceFile,sipName_BALL_hasKey);

	return NULL;
}

static PyObject *sipDo_ResourceFile_isEmpty(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			ResourceFile *ptr;

			if ((ptr = (ResourceFile *)sipGetCppPtr(sipThis,sipClass_ResourceFile)) == NULL)
				return NULL;

			res = ptr -> ResourceFile::isEmpty();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_ResourceFile,sipName_BALL_isEmpty);

	return NULL;
}

static PyObject *sipDo_ResourceFile_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			ResourceFile *ptr;

			if ((ptr = (ResourceFile *)sipGetCppPtr(sipThis,sipClass_ResourceFile)) == NULL)
				return NULL;

			res = ptr -> ResourceFile::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_ResourceFile,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_ResourceFile(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_ResourceFile)
		return ptr;

	if ((res = sipCast_File((File *)(ResourceFile *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_ResourceFile(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (ResourceFile *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_ResourceFile(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_ResourceFile
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
			sipNew = new ResourceFile();
		}
	}

	if (sipNew == NULL)
	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_String,&a0obj))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new ResourceFile(* a0);

			if (istemp0)
				delete a0;
		}
	}

	if (sipNew == NULL)
	{
		const ResourceFile *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_ResourceFile,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_ResourceFile(a0obj,(ResourceFile **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new ResourceFile(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_ResourceFile);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_ResourceFile,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (ResourceFile *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_ResourceFile[] = {
	{sipName_BALL_destroy, sipDo_ResourceFile_destroy, METH_VARARGS, NULL},
	{sipName_BALL_open, sipDo_ResourceFile_open, METH_VARARGS, NULL},
	{sipName_BALL_close, sipDo_ResourceFile_close, METH_VARARGS, NULL},
	{sipName_BALL_saveAs, sipDo_ResourceFile_saveAs, METH_VARARGS, NULL},
	{sipName_BALL_save, sipDo_ResourceFile_save, METH_VARARGS, NULL},
	{sipName_BALL_getSize, sipDo_ResourceFile_getSize, METH_VARARGS, NULL},
	{sipName_BALL_getRoot, sipDo_ResourceFile_getRoot, METH_VARARGS, NULL},
	{sipName_BALL_getEntry, sipDo_ResourceFile_getEntry, METH_VARARGS, NULL},
	{sipName_BALL_getValue, sipDo_ResourceFile_getValue, METH_VARARGS, NULL},
	{sipName_BALL_insert, sipDo_ResourceFile_insert, METH_VARARGS, NULL},
	{sipName_BALL_removeKey, sipDo_ResourceFile_removeKey, METH_VARARGS, NULL},
	{sipName_BALL_hasKey, sipDo_ResourceFile_hasKey, METH_VARARGS, NULL},
	{sipName_BALL_isEmpty, sipDo_ResourceFile_isEmpty, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_ResourceFile_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_ResourceFile(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_ResourceFile);
}

void sipConvertTo_ResourceFile(PyObject *sipPy,ResourceFile **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_ResourceFile);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (ResourceFile *)sipConvertToCpp(sipPy,sipClass_ResourceFile,sipIsErr);
}

ResourceFile *sipForceConvertTo_ResourceFile(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_ResourceFile(valobj))
	{
		ResourceFile *val;

		sipConvertTo_ResourceFile(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_ResourceFile);
	*iserrp = 1;

	return NULL;
}
