#include "sipBALLDeclBALL.h"
#include "sipBALLFile.h"



PyObject *sipClass_File;

static void sipDealloc_File(sipThisType *);

static PyTypeObject sipType_File = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_File,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_File,
	0,
	0,
	0,
	0,
	0,
};

static PyObject *sipDo_File_open(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_File)) == NULL)
		return NULL;

	{
#line 19 "file.sip"

    const String *a0;
    PyObject *a0obj;
    int a1 = 1;

    if (sipParseArgs(sipArgs,"I|i",sipCanConvertTo_String,&a0obj,&a1))
    {
      bool res;
      File *ptr;

      if ((ptr = (File *)sipGetCppPtr(sipThis,sipClass_File)) == NULL)
        return NULL;

      int iserr = 0;

      int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

      if (iserr)
        return NULL;

      res = ptr -> File::open(* a0, a1);

			if (res == false)
			{
				Log.error() << "Cannot open file " << *a0 << std::endl;
			}

      if (istemp0)
        delete a0;

      return sipConvertFromBool((int)res);
    }
#line 70 "./sipBALLFile.cpp"
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_File,sipName_BALL_open);

	return NULL;
}

static PyObject *sipDo_File_reopen(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_File)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			File *ptr;

			if ((ptr = (File *)sipGetCppPtr(sipThis,sipClass_File)) == NULL)
				return NULL;

			res = ptr -> File::reopen();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_File,sipName_BALL_reopen);

	return NULL;
}

static PyObject *sipDo_File_close(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_File)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			File *ptr;

			if ((ptr = (File *)sipGetCppPtr(sipThis,sipClass_File)) == NULL)
				return NULL;

			ptr -> File::close();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_File,sipName_BALL_close);

	return NULL;
}

static PyObject *sipDo_File_getName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_File)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			const String *res;
			File *ptr;

			if ((ptr = (File *)sipGetCppPtr(sipThis,sipClass_File)) == NULL)
				return NULL;

			res = &ptr -> File::getName();

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_File,sipName_BALL_getName);

	return NULL;
}

static PyObject *sipDo_File_getSize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_File)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			File *ptr;

			if ((ptr = (File *)sipGetCppPtr(sipThis,sipClass_File)) == NULL)
				return NULL;

			res = ptr -> File::getSize();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_File,sipName_BALL_getSize);

	return NULL;
}

static PyObject *sipDo_File_getOpenMode(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_File)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			File *ptr;

			if ((ptr = (File *)sipGetCppPtr(sipThis,sipClass_File)) == NULL)
				return NULL;

			res = ptr -> File::getOpenMode();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_File,sipName_BALL_getOpenMode);

	return NULL;
}

static PyObject *sipDo_File_copyTo(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_File)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			bool res;
			File *ptr;

			if ((ptr = (File *)sipGetCppPtr(sipThis,sipClass_File)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> File::copyTo(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_File,sipName_BALL_copyTo);

	return NULL;
}

static PyObject *sipDo_File_moveTo(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_File)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			bool res;
			File *ptr;

			if ((ptr = (File *)sipGetCppPtr(sipThis,sipClass_File)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> File::moveTo(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_File,sipName_BALL_moveTo);

	return NULL;
}

static PyObject *sipDo_File_remove(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_File)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			File *ptr;

			if ((ptr = (File *)sipGetCppPtr(sipThis,sipClass_File)) == NULL)
				return NULL;

			res = ptr -> File::remove();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_File,sipName_BALL_remove);

	return NULL;
}

static PyObject *sipDo_File_renameTo(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_File)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			bool res;
			File *ptr;

			if ((ptr = (File *)sipGetCppPtr(sipThis,sipClass_File)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> File::renameTo(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_File,sipName_BALL_renameTo);

	return NULL;
}

static PyObject *sipDo_File_truncate(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_File)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(sipArgs,"i",&a0))
		{
			bool res;
			File *ptr;

			if ((ptr = (File *)sipGetCppPtr(sipThis,sipClass_File)) == NULL)
				return NULL;

			res = ptr -> File::truncate( a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_File,sipName_BALL_truncate);

	return NULL;
}

static PyObject *sipDo_File_isOpen(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_File)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			File *ptr;

			if ((ptr = (File *)sipGetCppPtr(sipThis,sipClass_File)) == NULL)
				return NULL;

			res = ptr -> File::isOpen();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_File,sipName_BALL_isOpen);

	return NULL;
}

static PyObject *sipDo_File_isClosed(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_File)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			File *ptr;

			if ((ptr = (File *)sipGetCppPtr(sipThis,sipClass_File)) == NULL)
				return NULL;

			res = ptr -> File::isClosed();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_File,sipName_BALL_isClosed);

	return NULL;
}

static PyObject *sipDo_File_isAccessible(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_File)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			File *ptr;

			if ((ptr = (File *)sipGetCppPtr(sipThis,sipClass_File)) == NULL)
				return NULL;

			res = ptr -> File::isAccessible();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_File,sipName_BALL_isAccessible);

	return NULL;
}

static PyObject *sipDo_File_isCanonized(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_File)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			File *ptr;

			if ((ptr = (File *)sipGetCppPtr(sipThis,sipClass_File)) == NULL)
				return NULL;

			res = ptr -> File::isCanonized();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_File,sipName_BALL_isCanonized);

	return NULL;
}

static PyObject *sipDo_File_isReadable(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_File)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			File *ptr;

			if ((ptr = (File *)sipGetCppPtr(sipThis,sipClass_File)) == NULL)
				return NULL;

			res = ptr -> File::isReadable();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_File,sipName_BALL_isReadable);

	return NULL;
}

static PyObject *sipDo_File_isWritable(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_File)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			File *ptr;

			if ((ptr = (File *)sipGetCppPtr(sipThis,sipClass_File)) == NULL)
				return NULL;

			res = ptr -> File::isWritable();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_File,sipName_BALL_isWritable);

	return NULL;
}

static PyObject *sipDo_File_isExecutable(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_File)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			File *ptr;

			if ((ptr = (File *)sipGetCppPtr(sipThis,sipClass_File)) == NULL)
				return NULL;

			res = ptr -> File::isExecutable();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_File,sipName_BALL_isExecutable);

	return NULL;
}

static PyObject *sipDo_File_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_File)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			File *ptr;

			if ((ptr = (File *)sipGetCppPtr(sipThis,sipClass_File)) == NULL)
				return NULL;

			res = ptr -> File::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_File,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_File(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_File)
		return ptr;

	return NULL;
}

static void sipDealloc_File(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (File *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_File(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_File
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
			sipNew = new File();
		}
	}

	if (sipNew == NULL)
	{
		const String *a0;
		PyObject *a0obj;
		int a1 = 1;

		if (sipParseArgs(sipArgs,"-I|i",sipCanConvertTo_String,&a0obj,&a1))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new File(* a0, a1);

			if (istemp0)
				delete a0;
		}
	}

	if (sipNew == NULL)
	{
		const File *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_File,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_File(a0obj,(File **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new File(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_File);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_File,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (File *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_File[] = {
	{sipName_BALL_open, sipDo_File_open, METH_VARARGS, NULL},
	{sipName_BALL_reopen, sipDo_File_reopen, METH_VARARGS, NULL},
	{sipName_BALL_close, sipDo_File_close, METH_VARARGS, NULL},
	{sipName_BALL_getName, sipDo_File_getName, METH_VARARGS, NULL},
	{sipName_BALL_getSize, sipDo_File_getSize, METH_VARARGS, NULL},
	{sipName_BALL_getOpenMode, sipDo_File_getOpenMode, METH_VARARGS, NULL},
	{sipName_BALL_copyTo, sipDo_File_copyTo, METH_VARARGS, NULL},
	{sipName_BALL_moveTo, sipDo_File_moveTo, METH_VARARGS, NULL},
	{sipName_BALL_remove, sipDo_File_remove, METH_VARARGS, NULL},
	{sipName_BALL_renameTo, sipDo_File_renameTo, METH_VARARGS, NULL},
	{sipName_BALL_truncate, sipDo_File_truncate, METH_VARARGS, NULL},
	{sipName_BALL_isOpen, sipDo_File_isOpen, METH_VARARGS, NULL},
	{sipName_BALL_isClosed, sipDo_File_isClosed, METH_VARARGS, NULL},
	{sipName_BALL_isAccessible, sipDo_File_isAccessible, METH_VARARGS, NULL},
	{sipName_BALL_isCanonized, sipDo_File_isCanonized, METH_VARARGS, NULL},
	{sipName_BALL_isReadable, sipDo_File_isReadable, METH_VARARGS, NULL},
	{sipName_BALL_isWritable, sipDo_File_isWritable, METH_VARARGS, NULL},
	{sipName_BALL_isExecutable, sipDo_File_isExecutable, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_File_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_File(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_File);
}

void sipConvertTo_File(PyObject *sipPy,File **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_File);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (File *)sipConvertToCpp(sipPy,sipClass_File,sipIsErr);
}

File *sipForceConvertTo_File(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_File(valobj))
	{
		File *val;

		sipConvertTo_File(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_File);
	*iserrp = 1;

	return NULL;
}
