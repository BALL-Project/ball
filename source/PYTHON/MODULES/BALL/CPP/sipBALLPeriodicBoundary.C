#include "sipBALLDeclBALL.h"
#include "sipBALLPeriodicBoundary.h"


PyObject *sipClass_PeriodicBoundary;

static void sipDealloc_PeriodicBoundary(sipThisType *);

static PyTypeObject sipType_PeriodicBoundary = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_PeriodicBoundary,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_PeriodicBoundary,
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

static PyObject *sipDo_PeriodicBoundary_setup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PeriodicBoundary)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			PeriodicBoundary *ptr;

			if ((ptr = (PeriodicBoundary *)sipGetCppPtr(sipThis,sipClass_PeriodicBoundary)) == NULL)
				return NULL;

   try
   {
			res = ptr -> PeriodicBoundary::setup();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PeriodicBoundary,sipName_BALL_setup);

	return NULL;
}

static PyObject *sipDo_PeriodicBoundary_generateMoleculesVector(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PeriodicBoundary)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			PeriodicBoundary *ptr;

			if ((ptr = (PeriodicBoundary *)sipGetCppPtr(sipThis,sipClass_PeriodicBoundary)) == NULL)
				return NULL;

   try
   {
			res = ptr -> PeriodicBoundary::generateMoleculesVector();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PeriodicBoundary,sipName_BALL_generateMoleculesVector);

	return NULL;
}

static PyObject *sipDo_PeriodicBoundary_enable(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PeriodicBoundary)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			PeriodicBoundary *ptr;

			if ((ptr = (PeriodicBoundary *)sipGetCppPtr(sipThis,sipClass_PeriodicBoundary)) == NULL)
				return NULL;

   try
   {
			ptr -> PeriodicBoundary::enable();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PeriodicBoundary,sipName_BALL_enable);

	return NULL;
}

static PyObject *sipDo_PeriodicBoundary_disable(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PeriodicBoundary)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			PeriodicBoundary *ptr;

			if ((ptr = (PeriodicBoundary *)sipGetCppPtr(sipThis,sipClass_PeriodicBoundary)) == NULL)
				return NULL;

   try
   {
			ptr -> PeriodicBoundary::disable();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PeriodicBoundary,sipName_BALL_disable);

	return NULL;
}

static PyObject *sipDo_PeriodicBoundary_getBox(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PeriodicBoundary)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Box3 *res;
			PeriodicBoundary *ptr;

			if ((ptr = (PeriodicBoundary *)sipGetCppPtr(sipThis,sipClass_PeriodicBoundary)) == NULL)
				return NULL;

   try
   {
			res = new Box3(ptr -> PeriodicBoundary::getBox());
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PeriodicBoundary,sipName_BALL_getBox);

	return NULL;
}

static PyObject *sipDo_PeriodicBoundary_setBox(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PeriodicBoundary)) == NULL)
		return NULL;

	{
		const Box3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Box3,&a0obj))
		{
			PeriodicBoundary *ptr;

			if ((ptr = (PeriodicBoundary *)sipGetCppPtr(sipThis,sipClass_PeriodicBoundary)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Box3(a0obj,(Box3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> PeriodicBoundary::setBox(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PeriodicBoundary,sipName_BALL_setBox);

	return NULL;
}

static PyObject *sipDo_PeriodicBoundary_addSolvent(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PeriodicBoundary)) == NULL)
		return NULL;

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			int res;
			PeriodicBoundary *ptr;

			if ((ptr = (PeriodicBoundary *)sipGetCppPtr(sipThis,sipClass_PeriodicBoundary)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> PeriodicBoundary::addSolvent(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PeriodicBoundary,sipName_BALL_addSolvent);

	return NULL;
}

static PyObject *sipDo_PeriodicBoundary_removeSolvent(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PeriodicBoundary)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			PeriodicBoundary *ptr;

			if ((ptr = (PeriodicBoundary *)sipGetCppPtr(sipThis,sipClass_PeriodicBoundary)) == NULL)
				return NULL;

   try
   {
			res = ptr -> PeriodicBoundary::removeSolvent();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PeriodicBoundary,sipName_BALL_removeSolvent);

	return NULL;
}

static PyObject *sipDo_PeriodicBoundary_isEnabled(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PeriodicBoundary)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			PeriodicBoundary *ptr;

			if ((ptr = (PeriodicBoundary *)sipGetCppPtr(sipThis,sipClass_PeriodicBoundary)) == NULL)
				return NULL;

   try
   {
			res = ptr -> PeriodicBoundary::isEnabled();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PeriodicBoundary,sipName_BALL_isEnabled);

	return NULL;
}

static PyObject *sipDo_PeriodicBoundary_updateMolecules(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PeriodicBoundary)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			PeriodicBoundary *ptr;

			if ((ptr = (PeriodicBoundary *)sipGetCppPtr(sipThis,sipClass_PeriodicBoundary)) == NULL)
				return NULL;

   try
   {
			ptr -> PeriodicBoundary::updateMolecules();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PeriodicBoundary,sipName_BALL_updateMolecules);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_PeriodicBoundary(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_PeriodicBoundary)
		return ptr;

	return NULL;
}

static void sipDealloc_PeriodicBoundary(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (PeriodicBoundary *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_PeriodicBoundary(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_PeriodicBoundary
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
			sipNew = new PeriodicBoundary();
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
		const ForceField * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_ForceField,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_ForceField(a0obj,(ForceField **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new PeriodicBoundary(* a0);
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
		const PeriodicBoundary * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_PeriodicBoundary,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_PeriodicBoundary(a0obj,(PeriodicBoundary **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new PeriodicBoundary(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_PeriodicBoundary);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_PeriodicBoundary,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (PeriodicBoundary *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_PeriodicBoundary_options(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Options *val;
	PeriodicBoundary *ptr;

	if ((ptr = (PeriodicBoundary *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_PeriodicBoundary)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> options;

		valobj = sipMapCppToSelf(val,sipClass_Options);

		return valobj;
	}

	val = sipForceConvertTo_Options(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_PeriodicBoundary,sipName_BALL_options);
		return NULL;
	}

	ptr -> options = val;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_PeriodicBoundary[] = {
	{sipName_BALL_setup, sipDo_PeriodicBoundary_setup, METH_VARARGS, NULL},
	{sipName_BALL_generateMoleculesVector, sipDo_PeriodicBoundary_generateMoleculesVector, METH_VARARGS, NULL},
	{sipName_BALL_enable, sipDo_PeriodicBoundary_enable, METH_VARARGS, NULL},
	{sipName_BALL_disable, sipDo_PeriodicBoundary_disable, METH_VARARGS, NULL},
	{sipName_BALL_getBox, sipDo_PeriodicBoundary_getBox, METH_VARARGS, NULL},
	{sipName_BALL_setBox, sipDo_PeriodicBoundary_setBox, METH_VARARGS, NULL},
	{sipName_BALL_addSolvent, sipDo_PeriodicBoundary_addSolvent, METH_VARARGS, NULL},
	{sipName_BALL_removeSolvent, sipDo_PeriodicBoundary_removeSolvent, METH_VARARGS, NULL},
	{sipName_BALL_isEnabled, sipDo_PeriodicBoundary_isEnabled, METH_VARARGS, NULL},
	{sipName_BALL_updateMolecules, sipDo_PeriodicBoundary_updateMolecules, METH_VARARGS, NULL},
	{NULL}
};

PyMethodDef sipClassVarTab_PeriodicBoundary[] = {
	{sipName_BALL_options, sipGetSetVar_PeriodicBoundary_options, 0, NULL},
	{NULL}
};

PyMethodDef *sipClassVarHierTab_PeriodicBoundary[] = {
	sipClassVarTab_PeriodicBoundary,
	NULL
};

int sipCanConvertTo_PeriodicBoundary(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_PeriodicBoundary);
}

void sipConvertTo_PeriodicBoundary(PyObject *sipPy,PeriodicBoundary **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_PeriodicBoundary);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (PeriodicBoundary *)sipConvertToCpp(sipPy,sipClass_PeriodicBoundary,sipIsErr);
}

PeriodicBoundary *sipForceConvertTo_PeriodicBoundary(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_PeriodicBoundary(valobj))
	{
		PeriodicBoundary *val;

		sipConvertTo_PeriodicBoundary(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_PeriodicBoundary);

	*iserrp = 1;

	return NULL;
}
