#include "sipBALLDeclBALL.h"
#include "sipBALLFragmentDB.h"


PyObject *sipClass_FragmentDB;

static void sipDealloc_FragmentDB(sipThisType *);

static PyTypeObject sipType_FragmentDB = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_FragmentDB,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_FragmentDB,
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

static PyObject *sipDo_FragmentDB_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FragmentDB)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			FragmentDB *ptr;

			if ((ptr = (FragmentDB *)sipGetCppPtr(sipThis,sipClass_FragmentDB)) == NULL)
				return NULL;

   try
   {
			ptr -> FragmentDB::destroy();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FragmentDB,sipName_BALL_destroy);

	return NULL;
}

static PyObject *sipDo_FragmentDB_init(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FragmentDB)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			FragmentDB *ptr;

			if ((ptr = (FragmentDB *)sipGetCppPtr(sipThis,sipClass_FragmentDB)) == NULL)
				return NULL;

   try
   {
			ptr -> FragmentDB::init();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FragmentDB,sipName_BALL_init);

	return NULL;
}

static PyObject *sipDo_FragmentDB_setFilename(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FragmentDB)) == NULL)
		return NULL;

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			FragmentDB *ptr;

			if ((ptr = (FragmentDB *)sipGetCppPtr(sipThis,sipClass_FragmentDB)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> FragmentDB::setFilename(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FragmentDB,sipName_BALL_setFilename);

	return NULL;
}

static PyObject *sipDo_FragmentDB_getFilename(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FragmentDB)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const String *res;
			FragmentDB *ptr;

			if ((ptr = (FragmentDB *)sipGetCppPtr(sipThis,sipClass_FragmentDB)) == NULL)
				return NULL;

   try
   {
			res = &ptr -> FragmentDB::getFilename();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FragmentDB,sipName_BALL_getFilename);

	return NULL;
}

static PyObject *sipDo_FragmentDB_has(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FragmentDB)) == NULL)
		return NULL;

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			bool res;
			FragmentDB *ptr;

			if ((ptr = (FragmentDB *)sipGetCppPtr(sipThis,sipClass_FragmentDB)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> FragmentDB::has(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FragmentDB,sipName_BALL_has);

	return NULL;
}

static PyObject *sipDo_FragmentDB_getFragmentType(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FragmentDB)) == NULL)
		return NULL;

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			short res;
			FragmentDB *ptr;

			if ((ptr = (FragmentDB *)sipGetCppPtr(sipThis,sipClass_FragmentDB)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> FragmentDB::getFragmentType(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FragmentDB,sipName_BALL_getFragmentType);

	return NULL;
}

static PyObject *sipDo_FragmentDB_getFragment(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FragmentDB)) == NULL)
		return NULL;

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			const Fragment *res;
			FragmentDB *ptr;

			if ((ptr = (FragmentDB *)sipGetCppPtr(sipThis,sipClass_FragmentDB)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> FragmentDB::getFragment(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_Fragment);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FragmentDB,sipName_BALL_getFragment);

	return NULL;
}

static PyObject *sipDo_FragmentDB_getReferenceFragment(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FragmentDB)) == NULL)
		return NULL;

	{
		const Fragment * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Fragment,&a0obj))
		{
			const Fragment *res;
			FragmentDB *ptr;

			if ((ptr = (FragmentDB *)sipGetCppPtr(sipThis,sipClass_FragmentDB)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Fragment(a0obj,(Fragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> FragmentDB::getReferenceFragment(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Fragment);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FragmentDB,sipName_BALL_getReferenceFragment);

	return NULL;
}

static PyObject *sipDo_FragmentDB_getResidue(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FragmentDB)) == NULL)
		return NULL;

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			const Residue *res;
			FragmentDB *ptr;

			if ((ptr = (FragmentDB *)sipGetCppPtr(sipThis,sipClass_FragmentDB)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> FragmentDB::getResidue(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_Residue);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FragmentDB,sipName_BALL_getResidue);

	return NULL;
}

static PyObject *sipDo_FragmentDB_getFragmentCopy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FragmentDB)) == NULL)
		return NULL;

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			Fragment *res;
			FragmentDB *ptr;

			if ((ptr = (FragmentDB *)sipGetCppPtr(sipThis,sipClass_FragmentDB)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> FragmentDB::getFragmentCopy(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_Fragment);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FragmentDB,sipName_BALL_getFragmentCopy);

	return NULL;
}

static PyObject *sipDo_FragmentDB_getMoleculeCopy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FragmentDB)) == NULL)
		return NULL;

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			Molecule *res;
			FragmentDB *ptr;

			if ((ptr = (FragmentDB *)sipGetCppPtr(sipThis,sipClass_FragmentDB)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> FragmentDB::getMoleculeCopy(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_Molecule);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FragmentDB,sipName_BALL_getMoleculeCopy);

	return NULL;
}

static PyObject *sipDo_FragmentDB_getResidueCopy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FragmentDB)) == NULL)
		return NULL;

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			Residue *res;
			FragmentDB *ptr;

			if ((ptr = (FragmentDB *)sipGetCppPtr(sipThis,sipClass_FragmentDB)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> FragmentDB::getResidueCopy(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_Residue);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FragmentDB,sipName_BALL_getResidueCopy);

	return NULL;
}

static PyObject *sipDo_FragmentDB_getDefaultNamingStandard(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FragmentDB)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const String *res;
			FragmentDB *ptr;

			if ((ptr = (FragmentDB *)sipGetCppPtr(sipThis,sipClass_FragmentDB)) == NULL)
				return NULL;

   try
   {
			res = &ptr -> FragmentDB::getDefaultNamingStandard();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FragmentDB,sipName_BALL_getDefaultNamingStandard);

	return NULL;
}

static PyObject *sipDo_FragmentDB_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FragmentDB)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			FragmentDB *ptr;

			if ((ptr = (FragmentDB *)sipGetCppPtr(sipThis,sipClass_FragmentDB)) == NULL)
				return NULL;

   try
   {
			res = ptr -> FragmentDB::isValid();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FragmentDB,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_FragmentDB(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_FragmentDB)
		return ptr;

	return NULL;
}

static void sipDealloc_FragmentDB(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (FragmentDB *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_FragmentDB(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_FragmentDB
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
			sipNew = new FragmentDB();
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
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_String,&a0obj))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new FragmentDB(* a0);
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
		const FragmentDB * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_FragmentDB,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_FragmentDB(a0obj,(FragmentDB **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new FragmentDB(* a0, (bool)a1);
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
		const FragmentDB * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_FragmentDB,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_FragmentDB(a0obj,(FragmentDB **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new FragmentDB(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_FragmentDB);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_FragmentDB,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (FragmentDB *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_FragmentDB_tree(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	ResourceEntry *val;
	FragmentDB *ptr;

	if ((ptr = (FragmentDB *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_FragmentDB)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> tree;

		valobj = sipMapCppToSelf(val,sipClass_ResourceEntry);

		return valobj;
	}

	val = sipForceConvertTo_ResourceEntry(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_FragmentDB,sipName_BALL_tree);
		return NULL;
	}

	ptr -> tree = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_FragmentDB_build_bonds(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	BuildBondsProcessor *val;
	FragmentDB *ptr;

	if ((ptr = (FragmentDB *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_FragmentDB)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = &ptr -> build_bonds;

		valobj = sipMapCppToSelf(val,sipClass_BuildBondsProcessor);

		return valobj;
	}

	val = sipForceConvertTo_BuildBondsProcessor(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_FragmentDB,sipName_BALL_build_bonds);
		return NULL;
	}

	ptr -> build_bonds = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_FragmentDB_add_hydrogens(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	AddHydrogensProcessor *val;
	FragmentDB *ptr;

	if ((ptr = (FragmentDB *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_FragmentDB)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = &ptr -> add_hydrogens;

		valobj = sipMapCppToSelf(val,sipClass_AddHydrogensProcessor);

		return valobj;
	}

	val = sipForceConvertTo_AddHydrogensProcessor(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_FragmentDB,sipName_BALL_add_hydrogens);
		return NULL;
	}

	ptr -> add_hydrogens = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_FragmentDB_normalize_names(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	NormalizeNamesProcessor *val;
	FragmentDB *ptr;

	if ((ptr = (FragmentDB *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_FragmentDB)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = &ptr -> normalize_names;

		valobj = sipMapCppToSelf(val,sipClass_NormalizeNamesProcessor);

		return valobj;
	}

	val = sipForceConvertTo_NormalizeNamesProcessor(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_FragmentDB,sipName_BALL_normalize_names);
		return NULL;
	}

	ptr -> normalize_names = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_FragmentDB[] = {
	{sipName_BALL_destroy, sipDo_FragmentDB_destroy, METH_VARARGS, NULL},
	{sipName_BALL_init, sipDo_FragmentDB_init, METH_VARARGS, NULL},
	{sipName_BALL_setFilename, sipDo_FragmentDB_setFilename, METH_VARARGS, NULL},
	{sipName_BALL_getFilename, sipDo_FragmentDB_getFilename, METH_VARARGS, NULL},
	{sipName_BALL_has, sipDo_FragmentDB_has, METH_VARARGS, NULL},
	{sipName_BALL_getFragmentType, sipDo_FragmentDB_getFragmentType, METH_VARARGS, NULL},
	{sipName_BALL_getFragment, sipDo_FragmentDB_getFragment, METH_VARARGS, NULL},
	{sipName_BALL_getReferenceFragment, sipDo_FragmentDB_getReferenceFragment, METH_VARARGS, NULL},
	{sipName_BALL_getResidue, sipDo_FragmentDB_getResidue, METH_VARARGS, NULL},
	{sipName_BALL_getFragmentCopy, sipDo_FragmentDB_getFragmentCopy, METH_VARARGS, NULL},
	{sipName_BALL_getMoleculeCopy, sipDo_FragmentDB_getMoleculeCopy, METH_VARARGS, NULL},
	{sipName_BALL_getResidueCopy, sipDo_FragmentDB_getResidueCopy, METH_VARARGS, NULL},
	{sipName_BALL_getDefaultNamingStandard, sipDo_FragmentDB_getDefaultNamingStandard, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_FragmentDB_isValid, METH_VARARGS, NULL},
	{NULL}
};

PyMethodDef sipClassVarTab_FragmentDB[] = {
	{sipName_BALL_tree, sipGetSetVar_FragmentDB_tree, 0, NULL},
	{sipName_BALL_build_bonds, sipGetSetVar_FragmentDB_build_bonds, 0, NULL},
	{sipName_BALL_add_hydrogens, sipGetSetVar_FragmentDB_add_hydrogens, 0, NULL},
	{sipName_BALL_normalize_names, sipGetSetVar_FragmentDB_normalize_names, 0, NULL},
	{NULL}
};

PyMethodDef *sipClassVarHierTab_FragmentDB[] = {
	sipClassVarTab_FragmentDB,
	NULL
};

int sipCanConvertTo_FragmentDB(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_FragmentDB);
}

void sipConvertTo_FragmentDB(PyObject *sipPy,FragmentDB **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_FragmentDB);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (FragmentDB *)sipConvertToCpp(sipPy,sipClass_FragmentDB,sipIsErr);
}

FragmentDB *sipForceConvertTo_FragmentDB(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_FragmentDB(valobj))
	{
		FragmentDB *val;

		sipConvertTo_FragmentDB(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_FragmentDB);

	*iserrp = 1;

	return NULL;
}
