#include "sipBALLDeclBALL.h"
#include "sipBALLCompositeDescriptor.h"



PyObject *sipClass_CompositeDescriptor;

static void sipDealloc_CompositeDescriptor(sipThisType *);
static PyObject *sipPyInternalRepr_CompositeDescriptor(sipThisType *);

static PyTypeObject sipType_CompositeDescriptor = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_CompositeDescriptor,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_CompositeDescriptor,
	0,
	0,
	0,
	0,
	(reprfunc)sipPyInternalRepr_CompositeDescriptor,
};

sipCompositeDescriptor::sipCompositeDescriptor(): CompositeDescriptor()
{
	sipCommonCtor(sipPyMethods,3);
}

sipCompositeDescriptor::sipCompositeDescriptor(const CompositeDescriptor& a0,bool a1): CompositeDescriptor(a0,a1)
{
	sipCommonCtor(sipPyMethods,3);
}

sipCompositeDescriptor::sipCompositeDescriptor(const CompositeDescriptor& a0): CompositeDescriptor(a0)
{
	sipCommonCtor(sipPyMethods,3);
}

sipCompositeDescriptor::~sipCompositeDescriptor()
{
	sipCommonDtor(sipPyThis);
}
void sipCompositeDescriptor::clear()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipCompositeDescriptor::sipVH_clear(&sipPyMethods[0],sipPyThis,relLock);
	else
		CompositeDescriptor::clear();
}
void sipCompositeDescriptor::destroy()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_destroy,&relLock))
		sipCompositeDescriptor::sipVH_destroy(&sipPyMethods[1],sipPyThis,relLock);
	else
		CompositeDescriptor::destroy();
}
bool sipCompositeDescriptor::isValid() const
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_isValid,&relLock) ?
		sipCompositeDescriptor::sipVH_isValid(&sipPyMethods[2],sipPyThis,relLock) :
		CompositeDescriptor::isValid();
}

// The common handler for all classes that inherit this virtual member
// function.

void sipCompositeDescriptor::sipVH_clear(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_CompositeDescriptor,sipName_BALL_clear);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipCompositeDescriptor::sipVH_destroy(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_CompositeDescriptor,sipName_BALL_destroy);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipCompositeDescriptor::sipVH_isValid(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_CompositeDescriptor,sipName_BALL_isValid);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

static PyObject *sipDo_CompositeDescriptor_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CompositeDescriptor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			CompositeDescriptor *ptr;

			if ((ptr = (CompositeDescriptor *)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
				return NULL;

			ptr -> CompositeDescriptor::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_CompositeDescriptor,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_CompositeDescriptor_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CompositeDescriptor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			CompositeDescriptor *ptr;

			if ((ptr = (CompositeDescriptor *)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
				return NULL;

			ptr -> CompositeDescriptor::destroy();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_CompositeDescriptor,sipName_BALL_destroy);

	return NULL;
}

static PyObject *sipDo_CompositeDescriptor_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CompositeDescriptor)) == NULL)
		return NULL;

	{
		CompositeDescriptor *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"I|l",sipCanConvertTo_CompositeDescriptor,&a0obj,&a1))
		{
			CompositeDescriptor *ptr;

			if ((ptr = (CompositeDescriptor *)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_CompositeDescriptor(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> CompositeDescriptor::set(* a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_CompositeDescriptor,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_CompositeDescriptor_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CompositeDescriptor)) == NULL)
		return NULL;

	{
		CompositeDescriptor *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_CompositeDescriptor,&a0obj))
		{
			CompositeDescriptor *ptr;

			if ((ptr = (CompositeDescriptor *)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_CompositeDescriptor(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> CompositeDescriptor::swap(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_CompositeDescriptor,sipName_BALL_swap);

	return NULL;
}

static PyObject *sipDo_CompositeDescriptor_setName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CompositeDescriptor)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			CompositeDescriptor *ptr;

			if ((ptr = (CompositeDescriptor *)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> CompositeDescriptor::setName(* a0);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_CompositeDescriptor,sipName_BALL_setName);

	return NULL;
}

static PyObject *sipDo_CompositeDescriptor_getName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CompositeDescriptor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			String *res;
			CompositeDescriptor *ptr;

			if ((ptr = (CompositeDescriptor *)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
				return NULL;

			res = &ptr -> CompositeDescriptor::getName();

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const String *res;
			CompositeDescriptor *ptr;

			if ((ptr = (CompositeDescriptor *)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
				return NULL;

			res = &ptr -> CompositeDescriptor::getName();

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_CompositeDescriptor,sipName_BALL_getName);

	return NULL;
}

static PyObject *sipDo_CompositeDescriptor_setQuaternion(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CompositeDescriptor)) == NULL)
		return NULL;

	{
		const Quaternion *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Quaternion,&a0obj))
		{
			CompositeDescriptor *ptr;

			if ((ptr = (CompositeDescriptor *)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Quaternion(a0obj,(Quaternion **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> CompositeDescriptor::setQuaternion(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;
		float a1;

		if (sipParseArgs(sipArgs,"If",sipCanConvertTo_Vector3,&a0obj,&a1))
		{
			CompositeDescriptor *ptr;

			if ((ptr = (CompositeDescriptor *)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> CompositeDescriptor::setQuaternion(* a0, a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Angle *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Angle,&a1obj))
		{
			CompositeDescriptor *ptr;

			if ((ptr = (CompositeDescriptor *)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Angle(a1obj,(Angle **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> CompositeDescriptor::setQuaternion(* a0,* a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_CompositeDescriptor,sipName_BALL_setQuaternion);

	return NULL;
}

static PyObject *sipDo_CompositeDescriptor_getQuaternion(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CompositeDescriptor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Quaternion *res;
			CompositeDescriptor *ptr;

			if ((ptr = (CompositeDescriptor *)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
				return NULL;

			res = &ptr -> CompositeDescriptor::getQuaternion();

			return sipMapCppToSelf(res,sipClass_Quaternion);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const Quaternion *res;
			CompositeDescriptor *ptr;

			if ((ptr = (CompositeDescriptor *)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
				return NULL;

			res = &ptr -> CompositeDescriptor::getQuaternion();

			return sipMapCppToSelf(res,sipClass_Quaternion);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_CompositeDescriptor,sipName_BALL_getQuaternion);

	return NULL;
}

static PyObject *sipDo_CompositeDescriptor_getComposite(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CompositeDescriptor)) == NULL)
		return NULL;

	{
#line 34 "compositeDescriptor.sip"
  sipThisType *sipThis;

  if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CompositeDescriptor)) == NULL)
    return NULL;

  if (sipParseArgs(sipArgs,""))
  {
    Composite *res;
    CompositeDescriptor *ptr;

    if ((ptr = (CompositeDescriptor *)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
      return NULL;

    res = ptr -> CompositeDescriptor::getComposite();
    return pyMapBALLObjectToSip(*res);
  }
	return NULL;
#line 571 "../CPP/sipBALLCompositeDescriptor.cpp"
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_CompositeDescriptor,sipName_BALL_getComposite);

	return NULL;
}

static PyObject *sipDo_CompositeDescriptor_update(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CompositeDescriptor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			CompositeDescriptor *ptr;

			if ((ptr = (CompositeDescriptor *)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
				return NULL;

			ptr -> CompositeDescriptor::update();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_CompositeDescriptor,sipName_BALL_update);

	return NULL;
}

static PyObject *sipDo_CompositeDescriptor_drawEntity(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CompositeDescriptor)) == NULL)
		return NULL;

	{
		long a0 = false;

		if (sipParseArgs(sipArgs,"|l",&a0))
		{
			CompositeDescriptor *ptr;

			if ((ptr = (CompositeDescriptor *)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
				return NULL;

			ptr -> CompositeDescriptor::drawEntity( (bool)a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_CompositeDescriptor,sipName_BALL_drawEntity);

	return NULL;
}

static PyObject *sipDo_CompositeDescriptor_drawDirect(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CompositeDescriptor)) == NULL)
		return NULL;

	{
		long a0 = false;
		long a1 = false;

		if (sipParseArgs(sipArgs,"|ll",&a0,&a1))
		{
			CompositeDescriptor *ptr;

			if ((ptr = (CompositeDescriptor *)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
				return NULL;

			ptr -> CompositeDescriptor::drawDirect( (bool)a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_CompositeDescriptor,sipName_BALL_drawDirect);

	return NULL;
}

static PyObject *sipDo_CompositeDescriptor_isShallowCopy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CompositeDescriptor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			CompositeDescriptor *ptr;

			if ((ptr = (CompositeDescriptor *)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
				return NULL;

			res = ptr -> CompositeDescriptor::isShallowCopy();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_CompositeDescriptor,sipName_BALL_isShallowCopy);

	return NULL;
}

static PyObject *sipDo_CompositeDescriptor_isDeepCopy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CompositeDescriptor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			CompositeDescriptor *ptr;

			if ((ptr = (CompositeDescriptor *)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
				return NULL;

			res = ptr -> CompositeDescriptor::isDeepCopy();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_CompositeDescriptor,sipName_BALL_isDeepCopy);

	return NULL;
}

static PyObject *sipDo_CompositeDescriptor_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CompositeDescriptor)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			CompositeDescriptor *ptr;

			if ((ptr = (CompositeDescriptor *)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
				return NULL;

			res = ptr -> CompositeDescriptor::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_CompositeDescriptor,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_CompositeDescriptor(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_CompositeDescriptor)
		return ptr;

	return NULL;
}

static void sipDealloc_CompositeDescriptor(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipCompositeDescriptor *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (CompositeDescriptor *)sipThis -> u.cppPtr;
			else
				delete (sipCompositeDescriptor *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

static PyObject *sipPyInternalRepr_CompositeDescriptor(sipThisType *sipThis)
{
#line 69 "compositeDescriptor.sip"
  CompositeDescriptor* ptr;
  if ((ptr = (CompositeDescriptor*)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
    return NULL;

	String tmp = String("CompositeDescriptor { ") + ptr->getName() + " }";
  return PyString_FromString(tmp.c_str());
#line 796 "../CPP/sipBALLCompositeDescriptor.cpp"
}

PyObject *sipNew_CompositeDescriptor(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_CompositeDescriptor
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
			sipNew = new sipCompositeDescriptor();
	}
	}

	if (sipNew == NULL)
	{
		const CompositeDescriptor *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_CompositeDescriptor,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_CompositeDescriptor(a0obj,(CompositeDescriptor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipCompositeDescriptor(* a0, (bool)a1);
	}
	}

	if (sipNew == NULL)
	{
		const CompositeDescriptor *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_CompositeDescriptor,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_CompositeDescriptor(a0obj,(CompositeDescriptor **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipCompositeDescriptor(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_CompositeDescriptor);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_CompositeDescriptor,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (CompositeDescriptor *)sipNew;
			else
				delete (sipCompositeDescriptor *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipCompositeDescriptor *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_CompositeDescriptor[] = {
	{sipName_BALL_clear, sipDo_CompositeDescriptor_clear, METH_VARARGS, NULL},
	{sipName_BALL_destroy, sipDo_CompositeDescriptor_destroy, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_CompositeDescriptor_set, METH_VARARGS, NULL},
	{sipName_BALL_swap, sipDo_CompositeDescriptor_swap, METH_VARARGS, NULL},
	{sipName_BALL_setName, sipDo_CompositeDescriptor_setName, METH_VARARGS, NULL},
	{sipName_BALL_getName, sipDo_CompositeDescriptor_getName, METH_VARARGS, NULL},
	{sipName_BALL_setQuaternion, sipDo_CompositeDescriptor_setQuaternion, METH_VARARGS, NULL},
	{sipName_BALL_getQuaternion, sipDo_CompositeDescriptor_getQuaternion, METH_VARARGS, NULL},
	{sipName_BALL_getComposite, sipDo_CompositeDescriptor_getComposite, METH_VARARGS, NULL},
	{sipName_BALL_update, sipDo_CompositeDescriptor_update, METH_VARARGS, NULL},
	{sipName_BALL_drawEntity, sipDo_CompositeDescriptor_drawEntity, METH_VARARGS, NULL},
	{sipName_BALL_drawDirect, sipDo_CompositeDescriptor_drawDirect, METH_VARARGS, NULL},
	{sipName_BALL_isShallowCopy, sipDo_CompositeDescriptor_isShallowCopy, METH_VARARGS, NULL},
	{sipName_BALL_isDeepCopy, sipDo_CompositeDescriptor_isDeepCopy, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_CompositeDescriptor_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_CompositeDescriptor(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_CompositeDescriptor);
}

void sipConvertTo_CompositeDescriptor(PyObject *sipPy,CompositeDescriptor **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_CompositeDescriptor);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (CompositeDescriptor *)sipConvertToCpp(sipPy,sipClass_CompositeDescriptor,sipIsErr);
}

CompositeDescriptor *sipForceConvertTo_CompositeDescriptor(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_CompositeDescriptor(valobj))
	{
		CompositeDescriptor *val;

		sipConvertTo_CompositeDescriptor(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_CompositeDescriptor);
	*iserrp = 1;

	return NULL;
}
