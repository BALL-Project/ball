#include "sipBALLDeclBALL.h"
#include "sipBALLResourceEntry.h"



PyObject *sipClass_ResourceEntry;

static void sipDealloc_ResourceEntry(sipThisType *);

static PyTypeObject sipType_ResourceEntry = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_ResourceEntry,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_ResourceEntry,
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

static PyObject *sipDo_ResourceEntry_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			ptr -> ResourceEntry::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			ptr -> ResourceEntry::destroy();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_destroy);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		const ResourceEntry *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_ResourceEntry,&a0obj,&a1))
		{
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_ResourceEntry(a0obj,(ResourceEntry **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> ResourceEntry::set(* a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		ResourceEntry *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_ResourceEntry,&a0obj,&a1))
		{
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_ResourceEntry(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> ResourceEntry::get(* a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_get);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_getRoot(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			ResourceEntry *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			res = &ptr -> ResourceEntry::getRoot();

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const ResourceEntry *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			res = &ptr -> ResourceEntry::getRoot();

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_getRoot);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_getParent(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			ResourceEntry *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			res = ptr -> ResourceEntry::getParent();

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const ResourceEntry *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			res = ptr -> ResourceEntry::getParent();

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_getParent);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_getChild(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			ResourceEntry *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::getChild(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	{
		Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			const ResourceEntry *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::getChild(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_getChild);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_getEntry(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			ResourceEntry *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::getEntry(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			const ResourceEntry *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::getEntry(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_getEntry);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_getKey(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const String *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			res = &ptr -> ResourceEntry::getKey();

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_getKey);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_setValue(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> ResourceEntry::setValue(* a0);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_setValue);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_getValue(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			String *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			res = &ptr -> ResourceEntry::getValue();

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const String *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			res = &ptr -> ResourceEntry::getValue();

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_getValue);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_getPath(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			String *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			res = new String(ptr -> ResourceEntry::getPath());

			return sipNewCppToSelf(res,sipClass_String,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_getPath);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_countChildren(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			res = ptr -> ResourceEntry::countChildren();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_countChildren);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_countDescendants(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			res = ptr -> ResourceEntry::countDescendants();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_countDescendants);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_getSize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			res = ptr -> ResourceEntry::getSize();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_getSize);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_getDepth(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			res = ptr -> ResourceEntry::getDepth();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_getDepth);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_insertChild(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		const String *a1;
		PyObject *a1obj;
		long a2 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II|l",sipCanConvertTo_String,&a0obj,sipCanConvertTo_String,&a1obj,&a2))
		{
			ResourceEntry *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::insertChild(* a0,* a1, (bool)a2);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	{
		ResourceEntry *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_ResourceEntry,&a0obj,&a1))
		{
			ResourceEntry *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_ResourceEntry(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::insertChild(* a0, (bool)a1);

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_insertChild);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_insertSibling(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		const String *a1;
		PyObject *a1obj;
		long a2 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II|l",sipCanConvertTo_String,&a0obj,sipCanConvertTo_String,&a1obj,&a2))
		{
			ResourceEntry *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::insertSibling(* a0,* a1, (bool)a2);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_insertSibling);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_insert(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;
		const String *a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_String,&a0obj,sipCanConvertTo_String,&a1obj))
		{
			ResourceEntry *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::insert(* a0,* a1);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_insert);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_mergeChildrenOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		ResourceEntry *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_ResourceEntry,&a0obj,&a1))
		{
			bool res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_ResourceEntry(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::mergeChildrenOf(* a0, (bool)a1);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_mergeChildrenOf);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_removeKey(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			bool res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::removeKey(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_removeKey);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_findChild(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			ResourceEntry *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::findChild(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			const ResourceEntry *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::findChild(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_findChild);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_findDescendant(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			ResourceEntry *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::findDescendant(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			const ResourceEntry *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::findDescendant(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_findDescendant);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_findEntry(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			ResourceEntry *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::findEntry(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			const ResourceEntry *res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::findEntry(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_ResourceEntry);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_findEntry);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_hasChild(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			bool res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::hasChild(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_hasChild);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_isEmpty(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			res = ptr -> ResourceEntry::isEmpty();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_isEmpty);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_isParentOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		const ResourceEntry *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_ResourceEntry,&a0obj))
		{
			bool res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_ResourceEntry(a0obj,(ResourceEntry **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::isParentOf(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_isParentOf);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_isChildOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		const ResourceEntry *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_ResourceEntry,&a0obj))
		{
			bool res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_ResourceEntry(a0obj,(ResourceEntry **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::isChildOf(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_isChildOf);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_isAncestorOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		const ResourceEntry *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_ResourceEntry,&a0obj))
		{
			bool res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_ResourceEntry(a0obj,(ResourceEntry **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::isAncestorOf(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_isAncestorOf);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_isDescendantOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		const ResourceEntry *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_ResourceEntry,&a0obj))
		{
			bool res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_ResourceEntry(a0obj,(ResourceEntry **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::isDescendantOf(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_isDescendantOf);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_isRelatedWith(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		const ResourceEntry *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_ResourceEntry,&a0obj))
		{
			bool res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_ResourceEntry(a0obj,(ResourceEntry **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> ResourceEntry::isRelatedWith(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_isRelatedWith);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_isRoot(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			res = ptr -> ResourceEntry::isRoot();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_isRoot);

	return NULL;
}

static PyObject *sipDo_ResourceEntry_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ResourceEntry)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			ResourceEntry *ptr;

			if ((ptr = (ResourceEntry *)sipGetCppPtr(sipThis,sipClass_ResourceEntry)) == NULL)
				return NULL;

			res = ptr -> ResourceEntry::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ResourceEntry,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_ResourceEntry(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_ResourceEntry)
		return ptr;

	return NULL;
}

static void sipDealloc_ResourceEntry(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (ResourceEntry *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_ResourceEntry(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_ResourceEntry
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
			sipNew = new ResourceEntry();
		}
	}

	if (sipNew == NULL)
	{
		const ResourceEntry *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_ResourceEntry,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_ResourceEntry(a0obj,(ResourceEntry **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new ResourceEntry(* a0, (bool)a1);
		}
	}

	if (sipNew == NULL)
	{
		const String *a0;
		PyObject *a0obj;
		const String *a1;
		PyObject *a1obj;
		ResourceEntry *a2 = NULL;
		PyObject *a2obj = NULL;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-II|I",sipCanConvertTo_String,&a0obj,sipCanConvertTo_String,&a1obj,sipCanConvertTo_ResourceEntry,&a2obj))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);
			sipConvertTo_ResourceEntry(a2obj,&a2,0,&iserr);

			if (iserr)
				return NULL;

			sipNew = new ResourceEntry(* a0,* a1, a2);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;
		}
	}

	if (sipNew == NULL)
	{
		const ResourceEntry *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_ResourceEntry,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_ResourceEntry(a0obj,(ResourceEntry **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new ResourceEntry(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_ResourceEntry);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_ResourceEntry,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (ResourceEntry *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_ResourceEntry[] = {
	{sipName_BALL_clear, sipDo_ResourceEntry_clear, METH_VARARGS, NULL},
	{sipName_BALL_destroy, sipDo_ResourceEntry_destroy, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_ResourceEntry_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_ResourceEntry_get, METH_VARARGS, NULL},
	{sipName_BALL_getRoot, sipDo_ResourceEntry_getRoot, METH_VARARGS, NULL},
	{sipName_BALL_getParent, sipDo_ResourceEntry_getParent, METH_VARARGS, NULL},
	{sipName_BALL_getChild, sipDo_ResourceEntry_getChild, METH_VARARGS, NULL},
	{sipName_BALL_getEntry, sipDo_ResourceEntry_getEntry, METH_VARARGS, NULL},
	{sipName_BALL_getKey, sipDo_ResourceEntry_getKey, METH_VARARGS, NULL},
	{sipName_BALL_setValue, sipDo_ResourceEntry_setValue, METH_VARARGS, NULL},
	{sipName_BALL_getValue, sipDo_ResourceEntry_getValue, METH_VARARGS, NULL},
	{sipName_BALL_getPath, sipDo_ResourceEntry_getPath, METH_VARARGS, NULL},
	{sipName_BALL_countChildren, sipDo_ResourceEntry_countChildren, METH_VARARGS, NULL},
	{sipName_BALL_countDescendants, sipDo_ResourceEntry_countDescendants, METH_VARARGS, NULL},
	{sipName_BALL_getSize, sipDo_ResourceEntry_getSize, METH_VARARGS, NULL},
	{sipName_BALL_getDepth, sipDo_ResourceEntry_getDepth, METH_VARARGS, NULL},
	{sipName_BALL_insertChild, sipDo_ResourceEntry_insertChild, METH_VARARGS, NULL},
	{sipName_BALL_insertSibling, sipDo_ResourceEntry_insertSibling, METH_VARARGS, NULL},
	{sipName_BALL_insert, sipDo_ResourceEntry_insert, METH_VARARGS, NULL},
	{sipName_BALL_mergeChildrenOf, sipDo_ResourceEntry_mergeChildrenOf, METH_VARARGS, NULL},
	{sipName_BALL_removeKey, sipDo_ResourceEntry_removeKey, METH_VARARGS, NULL},
	{sipName_BALL_findChild, sipDo_ResourceEntry_findChild, METH_VARARGS, NULL},
	{sipName_BALL_findDescendant, sipDo_ResourceEntry_findDescendant, METH_VARARGS, NULL},
	{sipName_BALL_findEntry, sipDo_ResourceEntry_findEntry, METH_VARARGS, NULL},
	{sipName_BALL_hasChild, sipDo_ResourceEntry_hasChild, METH_VARARGS, NULL},
	{sipName_BALL_isEmpty, sipDo_ResourceEntry_isEmpty, METH_VARARGS, NULL},
	{sipName_BALL_isParentOf, sipDo_ResourceEntry_isParentOf, METH_VARARGS, NULL},
	{sipName_BALL_isChildOf, sipDo_ResourceEntry_isChildOf, METH_VARARGS, NULL},
	{sipName_BALL_isAncestorOf, sipDo_ResourceEntry_isAncestorOf, METH_VARARGS, NULL},
	{sipName_BALL_isDescendantOf, sipDo_ResourceEntry_isDescendantOf, METH_VARARGS, NULL},
	{sipName_BALL_isRelatedWith, sipDo_ResourceEntry_isRelatedWith, METH_VARARGS, NULL},
	{sipName_BALL_isRoot, sipDo_ResourceEntry_isRoot, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_ResourceEntry_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_ResourceEntry(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_ResourceEntry);
}

void sipConvertTo_ResourceEntry(PyObject *sipPy,ResourceEntry **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_ResourceEntry);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (ResourceEntry *)sipConvertToCpp(sipPy,sipClass_ResourceEntry,sipIsErr);
}

ResourceEntry *sipForceConvertTo_ResourceEntry(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_ResourceEntry(valobj))
	{
		ResourceEntry *val;

		sipConvertTo_ResourceEntry(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_ResourceEntry);
	*iserrp = 1;

	return NULL;
}
