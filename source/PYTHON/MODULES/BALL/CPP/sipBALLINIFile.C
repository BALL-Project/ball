#include "sipBALLDeclBALL.h"
#include "sipBALLINIFile.h"



PyObject *sipClass_INIFile;

static void sipDealloc_INIFile(sipThisType *);

static PyTypeObject sipType_INIFile = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_INIFile,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_INIFile,
	0,
	0,
	0,
	0,
	0,
};

static PyObject *sipDo_INIFile_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			ptr -> INIFile::destroy();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_destroy);

	return NULL;
}

static PyObject *sipDo_INIFile_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			ptr -> INIFile::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_INIFile_read(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			res = ptr -> INIFile::read();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_read);

	return NULL;
}

static PyObject *sipDo_INIFile_write(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			res = ptr -> INIFile::write();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_write);

	return NULL;
}

static PyObject *sipDo_INIFile_getFilename(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			const String *res;
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			res = &ptr -> INIFile::getFilename();

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_getFilename);

	return NULL;
}

static PyObject *sipDo_INIFile_setFilename(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> INIFile::setFilename(* a0);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_setFilename);

	return NULL;
}

static PyObject *sipDo_INIFile_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			res = ptr -> INIFile::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_isValid);

	return NULL;
}

static PyObject *sipDo_INIFile_getLine(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(sipArgs,"i",&a0))
		{
			const String *res;
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			res = ptr -> INIFile::getLine( a0);

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_getLine);

	return NULL;
}

static PyObject *sipDo_INIFile_setLine(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		int a0;
		const String *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"iI",&a0,sipCanConvertTo_String,&a1obj))
		{
			bool res;
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> INIFile::setLine( a0,* a1);

			if (istemp1)
				delete a1;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_setLine);

	return NULL;
}

static PyObject *sipDo_INIFile_deleteLine(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			bool res;
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> INIFile::deleteLine(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_deleteLine);

	return NULL;
}

static PyObject *sipDo_INIFile_insertLine(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		const String *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_String,&a0obj,sipCanConvertTo_String,&a1obj))
		{
			bool res;
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> INIFile::insertLine(* a0,* a1);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_insertLine);

	return NULL;
}

static PyObject *sipDo_INIFile_getOriginalNumberOfLines(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			res = ptr -> INIFile::getOriginalNumberOfLines();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_getOriginalNumberOfLines);

	return NULL;
}

static PyObject *sipDo_INIFile_getNumberOfLines(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			res = ptr -> INIFile::getNumberOfLines();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_getNumberOfLines);

	return NULL;
}

static PyObject *sipDo_INIFile_hasSection(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			bool res;
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> INIFile::hasSection(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_hasSection);

	return NULL;
}

static PyObject *sipDo_INIFile_getSectionName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			String *res;
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> INIFile::getSectionName(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_getSectionName);

	return NULL;
}

static PyObject *sipDo_INIFile_getNumberOfSections(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			res = ptr -> INIFile::getNumberOfSections();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_getNumberOfSections);

	return NULL;
}

static PyObject *sipDo_INIFile_getSectionFirstLine(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			int res;
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> INIFile::getSectionFirstLine(* a0);

			if (istemp0)
				delete a0;

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_getSectionFirstLine);

	return NULL;
}

static PyObject *sipDo_INIFile_getSectionLastLine(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			int res;
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> INIFile::getSectionLastLine(* a0);

			if (istemp0)
				delete a0;

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_getSectionLastLine);

	return NULL;
}

static PyObject *sipDo_INIFile_getSectionLength(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			int res;
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> INIFile::getSectionLength(* a0);

			if (istemp0)
				delete a0;

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_getSectionLength);

	return NULL;
}

static PyObject *sipDo_INIFile_hasEntry(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		const String *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_String,&a0obj,sipCanConvertTo_String,&a1obj))
		{
			bool res;
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> INIFile::hasEntry(* a0,* a1);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_hasEntry);

	return NULL;
}

static PyObject *sipDo_INIFile_getValue(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		const String *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_String,&a0obj,sipCanConvertTo_String,&a1obj))
		{
			String *res;
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = new String(ptr -> INIFile::getValue(* a0,* a1));

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipNewCppToSelf(res,sipClass_String,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_getValue);

	return NULL;
}

static PyObject *sipDo_INIFile_setValue(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		const String *a1;
		PyObject *a1obj;
		const String *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"III",sipCanConvertTo_String,&a0obj,sipCanConvertTo_String,&a1obj,sipCanConvertTo_String,&a2obj))
		{
			bool res;
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);
			int istemp2 = sipConvertTo_String(a2obj,(String **)&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> INIFile::setValue(* a0,* a1,* a2);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			if (istemp2)
				delete a2;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_setValue);

	return NULL;
}

static PyObject *sipDo_INIFile_Operator__cmp__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_INIFile)) == NULL)
		return NULL;

	{
		const INIFile *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_INIFile,&a0obj))
		{
			bool res;
			INIFile *ptr;

			if ((ptr = (INIFile *)sipGetCppPtr(sipThis,sipClass_INIFile)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_INIFile(a0obj,(INIFile **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> INIFile::operator==(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_INIFile,sipName_BALL_Operator__cmp__);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_INIFile(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_INIFile)
		return ptr;

	return NULL;
}

static void sipDealloc_INIFile(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (INIFile *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_INIFile(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_INIFile
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
			sipNew = new INIFile();
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

			sipNew = new INIFile(* a0);

			if (istemp0)
				delete a0;
	}
	}

	if (sipNew == NULL)
	{
		const INIFile *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_INIFile,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_INIFile(a0obj,(INIFile **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new INIFile(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_INIFile);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_INIFile,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (INIFile *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_INIFile[] = {
	{sipName_BALL_destroy, sipDo_INIFile_destroy, METH_VARARGS, NULL},
	{sipName_BALL_clear, sipDo_INIFile_clear, METH_VARARGS, NULL},
	{sipName_BALL_read, sipDo_INIFile_read, METH_VARARGS, NULL},
	{sipName_BALL_write, sipDo_INIFile_write, METH_VARARGS, NULL},
	{sipName_BALL_getFilename, sipDo_INIFile_getFilename, METH_VARARGS, NULL},
	{sipName_BALL_setFilename, sipDo_INIFile_setFilename, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_INIFile_isValid, METH_VARARGS, NULL},
	{sipName_BALL_getLine, sipDo_INIFile_getLine, METH_VARARGS, NULL},
	{sipName_BALL_setLine, sipDo_INIFile_setLine, METH_VARARGS, NULL},
	{sipName_BALL_deleteLine, sipDo_INIFile_deleteLine, METH_VARARGS, NULL},
	{sipName_BALL_insertLine, sipDo_INIFile_insertLine, METH_VARARGS, NULL},
	{sipName_BALL_getOriginalNumberOfLines, sipDo_INIFile_getOriginalNumberOfLines, METH_VARARGS, NULL},
	{sipName_BALL_getNumberOfLines, sipDo_INIFile_getNumberOfLines, METH_VARARGS, NULL},
	{sipName_BALL_hasSection, sipDo_INIFile_hasSection, METH_VARARGS, NULL},
	{sipName_BALL_getSectionName, sipDo_INIFile_getSectionName, METH_VARARGS, NULL},
	{sipName_BALL_getNumberOfSections, sipDo_INIFile_getNumberOfSections, METH_VARARGS, NULL},
	{sipName_BALL_getSectionFirstLine, sipDo_INIFile_getSectionFirstLine, METH_VARARGS, NULL},
	{sipName_BALL_getSectionLastLine, sipDo_INIFile_getSectionLastLine, METH_VARARGS, NULL},
	{sipName_BALL_getSectionLength, sipDo_INIFile_getSectionLength, METH_VARARGS, NULL},
	{sipName_BALL_hasEntry, sipDo_INIFile_hasEntry, METH_VARARGS, NULL},
	{sipName_BALL_getValue, sipDo_INIFile_getValue, METH_VARARGS, NULL},
	{sipName_BALL_setValue, sipDo_INIFile_setValue, METH_VARARGS, NULL},
	{sipName_BALL_Operator__cmp__, sipDo_INIFile_Operator__cmp__, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_INIFile(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_INIFile);
}

void sipConvertTo_INIFile(PyObject *sipPy,INIFile **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_INIFile);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (INIFile *)sipConvertToCpp(sipPy,sipClass_INIFile,sipIsErr);
}

INIFile *sipForceConvertTo_INIFile(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_INIFile(valobj))
	{
		INIFile *val;

		sipConvertTo_INIFile(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_INIFile);
	*iserrp = 1;

	return NULL;
}
