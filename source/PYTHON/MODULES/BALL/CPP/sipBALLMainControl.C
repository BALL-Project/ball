#include "sipBALLDeclBALL.h"
#include "sipBALLMainControl.h"



PyObject *sipClass_MainControl;

static void sipDealloc_MainControl(sipThisType *);

static PyTypeObject sipType_MainControl = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_MainControl,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_MainControl,
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

sipMainControl::sipMainControl(const MainControl& a0)
   throw()  : MainControl(a0)
{
	sipCommonCtor(sipPyMethods,4);
}

sipMainControl::~sipMainControl()
  throw()
{
	sipCommonDtor(sipPyThis);
}

void sipMainControl::registerThis()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_registerThis,&relLock))
		sipEmbeddable::sipVH_registerThis(&sipPyMethods[0],sipPyThis,relLock);
	else
		Embeddable::registerThis();
}

bool sipMainControl::isValid() const
 throw()
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_isValid,&relLock) ?
		sipMainControl::sipVH_isValid(&sipPyMethods[1],sipPyThis,relLock) :
		MainControl::isValid();
}

void sipMainControl::destroy()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_destroy,&relLock))
		sipMainControl::sipVH_destroy(&sipPyMethods[2],sipPyThis,relLock);
	else
		MainControl::destroy();
}

void sipMainControl::clear()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipMainControl::sipVH_clear(&sipPyMethods[3],sipPyThis,relLock);
	else
		MainControl::clear();
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipMainControl::sipVH_isValid(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
{
	bool res;
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(O)",sipThis -> sipSelf);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		res = (bool)PyInt_AsLong(resobj);

		Py_DECREF(resobj);

		if (PyErr_Occurred() == NULL)
		{
			goto releaseLock;
		}

		sipBadVirtualResultType(sipName_BALL_MainControl,sipName_BALL_isValid);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

// The common handler for all classes that inherit this virtual member
// function.

void sipMainControl::sipVH_destroy(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
{
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(O)",sipThis -> sipSelf);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_MainControl,sipName_BALL_destroy);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipMainControl::sipVH_clear(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
{
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(O)",sipThis -> sipSelf);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_MainControl,sipName_BALL_clear);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

static PyObject *sipDo_MainControl_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MainControl)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			MainControl *ptr;

			if ((ptr = (MainControl *)sipGetCppPtr(sipThis,sipClass_MainControl)) == NULL)
				return NULL;

			ptr -> MainControl::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_MainControl,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_MainControl_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MainControl)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			MainControl *ptr;

			if ((ptr = (MainControl *)sipGetCppPtr(sipThis,sipClass_MainControl)) == NULL)
				return NULL;

			ptr -> MainControl::destroy();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_MainControl,sipName_BALL_destroy);

	return NULL;
}

static PyObject *sipDo_MainControl_insert(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MainControl)) == NULL)
		return NULL;

	{
		const Composite *a0;
		PyObject *a0obj;
		const String *a1 = NULL;
		PyObject *a1obj = NULL;
		Vector3 a2def = Vector3(0,0,0);
		const Vector3 *a2 = &a2def;
		PyObject *a2obj = NULL;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|II",sipCanConvertTo_Composite,&a0obj,sipCanConvertTo_String,&a1obj,sipCanConvertTo_Vector3,&a2obj))
		{
			CompositeDescriptor *res;
			MainControl *ptr;

			if ((ptr = (MainControl *)sipGetCppPtr(sipThis,sipClass_MainControl)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);
			sipConvertTo_Vector3(a2obj,(Vector3 **)&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> MainControl::insert(* a0,* a1,* a2);

			if (istemp1)
				delete a1;

			return sipMapCppToSelf(res,sipClass_CompositeDescriptor);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_MainControl,sipName_BALL_insert);

	return NULL;
}

static PyObject *sipDo_MainControl_remove(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MainControl)) == NULL)
		return NULL;

	{
		const Composite *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			bool res;
			MainControl *ptr;

			if ((ptr = (MainControl *)sipGetCppPtr(sipThis,sipClass_MainControl)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> MainControl::remove(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const CompositeDescriptor *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_CompositeDescriptor,&a0obj))
		{
			bool res;
			MainControl *ptr;

			if ((ptr = (MainControl *)sipGetCppPtr(sipThis,sipClass_MainControl)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_CompositeDescriptor(a0obj,(CompositeDescriptor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> MainControl::remove(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_MainControl,sipName_BALL_remove);

	return NULL;
}

static PyObject *sipDo_MainControl_getDescriptorList(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MainControl)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			PyCompositeDescriptorList *res;
			MainControl *ptr;

			if ((ptr = (MainControl *)sipGetCppPtr(sipThis,sipClass_MainControl)) == NULL)
				return NULL;

			res = new PyCompositeDescriptorList(ptr -> MainControl::getDescriptorList());

			PyObject *resobj = sipConvertFrom_PyCompositeDescriptorList(res);
			delete res;

			return resobj;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_MainControl,sipName_BALL_getDescriptorList);

	return NULL;
}

static PyObject *sipDo_MainControl_setName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MainControl)) == NULL)
		return NULL;

	{
		const Composite *a0;
		PyObject *a0obj;
		const String *a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_Composite,&a0obj,sipCanConvertTo_String,&a1obj))
		{
			bool res;
			MainControl *ptr;

			if ((ptr = (MainControl *)sipGetCppPtr(sipThis,sipClass_MainControl)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> MainControl::setName(* a0,* a1);

			if (istemp1)
				delete a1;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_MainControl,sipName_BALL_setName);

	return NULL;
}

static PyObject *sipDo_MainControl_getName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MainControl)) == NULL)
		return NULL;

	{
		const Composite *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			String *res;
			MainControl *ptr;

			if ((ptr = (MainControl *)sipGetCppPtr(sipThis,sipClass_MainControl)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> MainControl::getName(* a0);

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	{
		const Composite *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			const String *res;
			MainControl *ptr;

			if ((ptr = (MainControl *)sipGetCppPtr(sipThis,sipClass_MainControl)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> MainControl::getName(* a0);

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_MainControl,sipName_BALL_getName);

	return NULL;
}

static PyObject *sipDo_MainControl_getDescriptor(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MainControl)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			CompositeDescriptor *res;
			MainControl *ptr;

			if ((ptr = (MainControl *)sipGetCppPtr(sipThis,sipClass_MainControl)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> MainControl::getDescriptor(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_CompositeDescriptor);
		}
	}

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			const CompositeDescriptor *res;
			MainControl *ptr;

			if ((ptr = (MainControl *)sipGetCppPtr(sipThis,sipClass_MainControl)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> MainControl::getDescriptor(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_CompositeDescriptor);
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			CompositeDescriptor *res;
			MainControl *ptr;

			if ((ptr = (MainControl *)sipGetCppPtr(sipThis,sipClass_MainControl)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> MainControl::getDescriptor(* a0);

			return sipMapCppToSelf(res,sipClass_CompositeDescriptor);
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			const CompositeDescriptor *res;
			MainControl *ptr;

			if ((ptr = (MainControl *)sipGetCppPtr(sipThis,sipClass_MainControl)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> MainControl::getDescriptor(* a0);

			return sipMapCppToSelf(res,sipClass_CompositeDescriptor);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_MainControl,sipName_BALL_getDescriptor);

	return NULL;
}

static PyObject *sipDo_MainControl_setCenter(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MainControl)) == NULL)
		return NULL;

	{
		const Composite *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_Composite,&a0obj,sipCanConvertTo_Vector3,&a1obj))
		{
			bool res;
			MainControl *ptr;

			if ((ptr = (MainControl *)sipGetCppPtr(sipThis,sipClass_MainControl)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> MainControl::setCenter(* a0,* a1);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_MainControl,sipName_BALL_setCenter);

	return NULL;
}

static PyObject *sipDo_MainControl_getCenter(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MainControl)) == NULL)
		return NULL;

	{
		const Composite *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			Vector3 *res;
			MainControl *ptr;

			if ((ptr = (MainControl *)sipGetCppPtr(sipThis,sipClass_MainControl)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> MainControl::getCenter(* a0);

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	{
		const Composite *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			const Vector3 *res;
			MainControl *ptr;

			if ((ptr = (MainControl *)sipGetCppPtr(sipThis,sipClass_MainControl)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> MainControl::getCenter(* a0);

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_MainControl,sipName_BALL_getCenter);

	return NULL;
}

static PyObject *sipDo_MainControl_update(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MainControl)) == NULL)
		return NULL;

	{
		const Composite *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			bool res;
			MainControl *ptr;

			if ((ptr = (MainControl *)sipGetCppPtr(sipThis,sipClass_MainControl)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> MainControl::update(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_MainControl,sipName_BALL_update);

	return NULL;
}

static PyObject *sipDo_MainControl_updateAll(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MainControl)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			MainControl *ptr;

			if ((ptr = (MainControl *)sipGetCppPtr(sipThis,sipClass_MainControl)) == NULL)
				return NULL;

			ptr -> MainControl::updateAll();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_MainControl,sipName_BALL_updateAll);

	return NULL;
}

static PyObject *sipDo_MainControl_isInserted(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MainControl)) == NULL)
		return NULL;

	{
		const Composite *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			bool res;
			MainControl *ptr;

			if ((ptr = (MainControl *)sipGetCppPtr(sipThis,sipClass_MainControl)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> MainControl::isInserted(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const CompositeDescriptor *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_CompositeDescriptor,&a0obj))
		{
			bool res;
			MainControl *ptr;

			if ((ptr = (MainControl *)sipGetCppPtr(sipThis,sipClass_MainControl)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_CompositeDescriptor(a0obj,(CompositeDescriptor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> MainControl::isInserted(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_MainControl,sipName_BALL_isInserted);

	return NULL;
}

static PyObject *sipDo_MainControl_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_MainControl)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			MainControl *ptr;

			if ((ptr = (MainControl *)sipGetCppPtr(sipThis,sipClass_MainControl)) == NULL)
				return NULL;

			res = ptr -> MainControl::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_MainControl,sipName_BALL_isValid);

	return NULL;
}

static PyObject *sipDo_MainControl_countInstances(PyObject *,PyObject *sipArgs)
{
	int sipArgsParsed = 0;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,"-"))
		{
			int res;

			res = MainControl::countInstances();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_MainControl,sipName_BALL_countInstances);

	return NULL;
}

static PyObject *sipDo_MainControl_getInstance(PyObject *,PyObject *sipArgs)
{
	int sipArgsParsed = 0;

	{
		Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_Position,&a0obj))
		{
			MainControl *res;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = MainControl::getInstance(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_MainControl);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_MainControl,sipName_BALL_getInstance);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_MainControl(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_MainControl)
		return ptr;

	if ((res = sipCast_Embeddable((Embeddable *)(MainControl *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_MainControl(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipMainControl *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (MainControl *)sipThis -> u.cppPtr;
			else
				delete (sipMainControl *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_MainControl(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_MainControl
	};

	sipThisType *sipThis = NULL;
	const void *sipNew = NULL;
	int sipFlags = SIP_PY_OWNED;
	int sipArgsParsed = 0;

	// See if there is something pending.

	sipNew = sipGetPending(&sipFlags);

	if (sipNew == NULL)
	{
		const MainControl *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_MainControl,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_MainControl(a0obj,(MainControl **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipMainControl(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_MainControl);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_MainControl,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (MainControl *)sipNew;
			else
				delete (sipMainControl *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipMainControl *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_MainControl[] = {
	{sipName_BALL_clear, sipDo_MainControl_clear, METH_VARARGS, NULL},
	{sipName_BALL_destroy, sipDo_MainControl_destroy, METH_VARARGS, NULL},
	{sipName_BALL_insert, sipDo_MainControl_insert, METH_VARARGS, NULL},
	{sipName_BALL_remove, sipDo_MainControl_remove, METH_VARARGS, NULL},
	{sipName_BALL_getDescriptorList, sipDo_MainControl_getDescriptorList, METH_VARARGS, NULL},
	{sipName_BALL_setName, sipDo_MainControl_setName, METH_VARARGS, NULL},
	{sipName_BALL_getName, sipDo_MainControl_getName, METH_VARARGS, NULL},
	{sipName_BALL_getDescriptor, sipDo_MainControl_getDescriptor, METH_VARARGS, NULL},
	{sipName_BALL_setCenter, sipDo_MainControl_setCenter, METH_VARARGS, NULL},
	{sipName_BALL_getCenter, sipDo_MainControl_getCenter, METH_VARARGS, NULL},
	{sipName_BALL_update, sipDo_MainControl_update, METH_VARARGS, NULL},
	{sipName_BALL_updateAll, sipDo_MainControl_updateAll, METH_VARARGS, NULL},
	{sipName_BALL_isInserted, sipDo_MainControl_isInserted, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_MainControl_isValid, METH_VARARGS, NULL},
	{sipName_BALL_countInstances, sipDo_MainControl_countInstances, METH_VARARGS, NULL},
	{sipName_BALL_getInstance, sipDo_MainControl_getInstance, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_MainControl(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_MainControl);
}

void sipConvertTo_MainControl(PyObject *sipPy,MainControl **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_MainControl);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (MainControl *)sipConvertToCpp(sipPy,sipClass_MainControl,sipIsErr);
}

MainControl *sipForceConvertTo_MainControl(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_MainControl(valobj))
	{
		MainControl *val;

		sipConvertTo_MainControl(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_MainControl);
	*iserrp = 1;

	return NULL;
}
