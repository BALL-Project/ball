#include "sipBALLDeclBALL.h"
#include "sipBALLPropertyManager.h"


PyObject *sipClass_PropertyManager;

static void sipDealloc_PropertyManager(sipThisType *);

static PyTypeObject sipType_PropertyManager = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_PropertyManager,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_PropertyManager,
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

static PyObject *sipDo_PropertyManager_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PropertyManager)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			PropertyManager *ptr;

			if ((ptr = (PropertyManager *)sipGetCppPtr(sipThis,sipClass_PropertyManager)) == NULL)
				return NULL;

   try
   {
			ptr -> PropertyManager::clear();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PropertyManager,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_PropertyManager_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PropertyManager)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			PropertyManager *ptr;

			if ((ptr = (PropertyManager *)sipGetCppPtr(sipThis,sipClass_PropertyManager)) == NULL)
				return NULL;

   try
   {
			ptr -> PropertyManager::destroy();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PropertyManager,sipName_BALL_destroy);

	return NULL;
}

static PyObject *sipDo_PropertyManager_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PropertyManager)) == NULL)
		return NULL;

	{
		const PropertyManager * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_PropertyManager,&a0obj))
		{
			PropertyManager *ptr;

			if ((ptr = (PropertyManager *)sipGetCppPtr(sipThis,sipClass_PropertyManager)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_PropertyManager(a0obj,(PropertyManager **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> PropertyManager::set(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PropertyManager,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_PropertyManager_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PropertyManager)) == NULL)
		return NULL;

	{
		PropertyManager * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_PropertyManager,&a0obj))
		{
			PropertyManager *ptr;

			if ((ptr = (PropertyManager *)sipGetCppPtr(sipThis,sipClass_PropertyManager)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_PropertyManager(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> PropertyManager::get(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PropertyManager,sipName_BALL_get);

	return NULL;
}

static PyObject *sipDo_PropertyManager_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PropertyManager)) == NULL)
		return NULL;

	{
		PropertyManager * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_PropertyManager,&a0obj))
		{
			PropertyManager *ptr;

			if ((ptr = (PropertyManager *)sipGetCppPtr(sipThis,sipClass_PropertyManager)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_PropertyManager(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> PropertyManager::swap(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PropertyManager,sipName_BALL_swap);

	return NULL;
}

static PyObject *sipDo_PropertyManager_setProperty(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PropertyManager)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"i",&a0))
		{
			PropertyManager *ptr;

			if ((ptr = (PropertyManager *)sipGetCppPtr(sipThis,sipClass_PropertyManager)) == NULL)
				return NULL;

   try
   {
			ptr -> PropertyManager::setProperty( a0);
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

	{
		NamedProperty * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_NamedProperty,&a0obj))
		{
			PropertyManager *ptr;

			if ((ptr = (PropertyManager *)sipGetCppPtr(sipThis,sipClass_PropertyManager)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_NamedProperty(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> PropertyManager::setProperty(* a0);
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

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			PropertyManager *ptr;

			if ((ptr = (PropertyManager *)sipGetCppPtr(sipThis,sipClass_PropertyManager)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> PropertyManager::setProperty(* a0);
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

	{
		const String * a0;
		PyObject *a0obj;
		float a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"If",sipCanConvertTo_String,&a0obj,&a1))
		{
			PropertyManager *ptr;

			if ((ptr = (PropertyManager *)sipGetCppPtr(sipThis,sipClass_PropertyManager)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> PropertyManager::setProperty(* a0, a1);
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

	{
		const String * a0;
		PyObject *a0obj;
		String * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_String,&a0obj,sipCanConvertTo_String,&a1obj))
		{
			PropertyManager *ptr;

			if ((ptr = (PropertyManager *)sipGetCppPtr(sipThis,sipClass_PropertyManager)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> PropertyManager::setProperty(* a0,* a1);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const String * a0;
		PyObject *a0obj;
		PersistentObject * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_String,&a0obj,sipCanConvertTo_PersistentObject,&a1obj))
		{
			PropertyManager *ptr;

			if ((ptr = (PropertyManager *)sipGetCppPtr(sipThis,sipClass_PropertyManager)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			sipConvertTo_PersistentObject(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> PropertyManager::setProperty(* a0,* a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PropertyManager,sipName_BALL_setProperty);

	return NULL;
}

static PyObject *sipDo_PropertyManager_clearProperty(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PropertyManager)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"i",&a0))
		{
			PropertyManager *ptr;

			if ((ptr = (PropertyManager *)sipGetCppPtr(sipThis,sipClass_PropertyManager)) == NULL)
				return NULL;

   try
   {
			ptr -> PropertyManager::clearProperty( a0);
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

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			PropertyManager *ptr;

			if ((ptr = (PropertyManager *)sipGetCppPtr(sipThis,sipClass_PropertyManager)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> PropertyManager::clearProperty(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PropertyManager,sipName_BALL_clearProperty);

	return NULL;
}

static PyObject *sipDo_PropertyManager_toggleProperty(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PropertyManager)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"i",&a0))
		{
			PropertyManager *ptr;

			if ((ptr = (PropertyManager *)sipGetCppPtr(sipThis,sipClass_PropertyManager)) == NULL)
				return NULL;

   try
   {
			ptr -> PropertyManager::toggleProperty( a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PropertyManager,sipName_BALL_toggleProperty);

	return NULL;
}

static PyObject *sipDo_PropertyManager_countProperties(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PropertyManager)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			PropertyManager *ptr;

			if ((ptr = (PropertyManager *)sipGetCppPtr(sipThis,sipClass_PropertyManager)) == NULL)
				return NULL;

   try
   {
			res = ptr -> PropertyManager::countProperties();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PropertyManager,sipName_BALL_countProperties);

	return NULL;
}

static PyObject *sipDo_PropertyManager_getProperty(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PropertyManager)) == NULL)
		return NULL;

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			const NamedProperty *res;
			PropertyManager *ptr;

			if ((ptr = (PropertyManager *)sipGetCppPtr(sipThis,sipClass_PropertyManager)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = &ptr -> PropertyManager::getProperty(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_NamedProperty);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PropertyManager,sipName_BALL_getProperty);

	return NULL;
}

static PyObject *sipDo_PropertyManager_countNamedProperties(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PropertyManager)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			PropertyManager *ptr;

			if ((ptr = (PropertyManager *)sipGetCppPtr(sipThis,sipClass_PropertyManager)) == NULL)
				return NULL;

   try
   {
			res = ptr -> PropertyManager::countNamedProperties();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PropertyManager,sipName_BALL_countNamedProperties);

	return NULL;
}

static PyObject *sipDo_PropertyManager_hasProperty(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PropertyManager)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"i",&a0))
		{
			bool res;
			PropertyManager *ptr;

			if ((ptr = (PropertyManager *)sipGetCppPtr(sipThis,sipClass_PropertyManager)) == NULL)
				return NULL;

   try
   {
			res = ptr -> PropertyManager::hasProperty( a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipConvertFromBool((int)res);
		}
	}

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			bool res;
			PropertyManager *ptr;

			if ((ptr = (PropertyManager *)sipGetCppPtr(sipThis,sipClass_PropertyManager)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> PropertyManager::hasProperty(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PropertyManager,sipName_BALL_hasProperty);

	return NULL;
}

static PyObject *sipDo_PropertyManager_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PropertyManager)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			PropertyManager *ptr;

			if ((ptr = (PropertyManager *)sipGetCppPtr(sipThis,sipClass_PropertyManager)) == NULL)
				return NULL;

   try
   {
			res = ptr -> PropertyManager::isValid();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PropertyManager,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_PropertyManager(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_PropertyManager)
		return ptr;

	return NULL;
}

static void sipDealloc_PropertyManager(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (PropertyManager *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_PropertyManager(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_PropertyManager
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
			sipNew = new PropertyManager();
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
		const PropertyManager * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_PropertyManager,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_PropertyManager(a0obj,(PropertyManager **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new PropertyManager(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_PropertyManager);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_PropertyManager,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (PropertyManager *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_PropertyManager[] = {
	{sipName_BALL_clear, sipDo_PropertyManager_clear, METH_VARARGS, NULL},
	{sipName_BALL_destroy, sipDo_PropertyManager_destroy, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_PropertyManager_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_PropertyManager_get, METH_VARARGS, NULL},
	{sipName_BALL_swap, sipDo_PropertyManager_swap, METH_VARARGS, NULL},
	{sipName_BALL_setProperty, sipDo_PropertyManager_setProperty, METH_VARARGS, NULL},
	{sipName_BALL_clearProperty, sipDo_PropertyManager_clearProperty, METH_VARARGS, NULL},
	{sipName_BALL_toggleProperty, sipDo_PropertyManager_toggleProperty, METH_VARARGS, NULL},
	{sipName_BALL_countProperties, sipDo_PropertyManager_countProperties, METH_VARARGS, NULL},
	{sipName_BALL_getProperty, sipDo_PropertyManager_getProperty, METH_VARARGS, NULL},
	{sipName_BALL_countNamedProperties, sipDo_PropertyManager_countNamedProperties, METH_VARARGS, NULL},
	{sipName_BALL_hasProperty, sipDo_PropertyManager_hasProperty, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_PropertyManager_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_PropertyManager(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_PropertyManager);
}

void sipConvertTo_PropertyManager(PyObject *sipPy,PropertyManager **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_PropertyManager);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (PropertyManager *)sipConvertToCpp(sipPy,sipClass_PropertyManager,sipIsErr);
}

PropertyManager *sipForceConvertTo_PropertyManager(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_PropertyManager(valobj))
	{
		PropertyManager *val;

		sipConvertTo_PropertyManager(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_PropertyManager);

	*iserrp = 1;

	return NULL;
}
