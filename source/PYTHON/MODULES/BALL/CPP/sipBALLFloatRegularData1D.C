#include "sipBALLDeclBALL.h"
#include "sipBALLFloatRegularData1D.h"



PyObject *sipClass_FloatRegularData1D;

static void sipDealloc_FloatRegularData1D(sipThisType *);

static PyTypeObject sipType_FloatRegularData1D = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_FloatRegularData1D,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_FloatRegularData1D,
	0,
	0,
	0,
	0,
	0,
};

sipFloatRegularData1D::sipFloatRegularData1D(): FloatRegularData1D()
{
	sipCommonCtor(sipPyMethods,2);
}

sipFloatRegularData1D::sipFloatRegularData1D(const FloatRegularData1D& a0): FloatRegularData1D(a0)
{
	sipCommonCtor(sipPyMethods,2);
}

sipFloatRegularData1D::~sipFloatRegularData1D()
{
	sipCommonDtor(sipPyThis);
}
void sipFloatRegularData1D::clear()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipFloatRegularData1D::sipVH_clear(&sipPyMethods[0],sipPyThis,relLock);
	else
		FloatRegularData1D::clear();
}
void sipFloatRegularData1D::destroy()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_destroy,&relLock))
		sipFloatRegularData1D::sipVH_destroy(&sipPyMethods[1],sipPyThis,relLock);
	else
		FloatRegularData1D::destroy();
}

// The common handler for all classes that inherit this virtual member
// function.

void sipFloatRegularData1D::sipVH_clear(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_FloatRegularData1D,sipName_BALL_clear);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipFloatRegularData1D::sipVH_destroy(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_FloatRegularData1D,sipName_BALL_destroy);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

static PyObject *sipDo_FloatRegularData1D_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData1D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			FloatRegularData1D *ptr;

			if ((ptr = (FloatRegularData1D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData1D)) == NULL)
				return NULL;

			ptr -> FloatRegularData1D::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData1D,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_FloatRegularData1D_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData1D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			FloatRegularData1D *ptr;

			if ((ptr = (FloatRegularData1D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData1D)) == NULL)
				return NULL;

			ptr -> FloatRegularData1D::destroy();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData1D,sipName_BALL_destroy);

	return NULL;
}

static PyObject *sipDo_FloatRegularData1D_getSize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData1D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			FloatRegularData1D *ptr;

			if ((ptr = (FloatRegularData1D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData1D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData1D::getSize();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData1D,sipName_BALL_getSize);

	return NULL;
}

static PyObject *sipDo_FloatRegularData1D_getLower(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData1D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			double res;
			FloatRegularData1D *ptr;

			if ((ptr = (FloatRegularData1D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData1D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData1D::getLower();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData1D,sipName_BALL_getLower);

	return NULL;
}

static PyObject *sipDo_FloatRegularData1D_getUpper(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData1D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			double res;
			FloatRegularData1D *ptr;

			if ((ptr = (FloatRegularData1D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData1D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData1D::getUpper();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData1D,sipName_BALL_getUpper);

	return NULL;
}

static PyObject *sipDo_FloatRegularData1D_setUpper(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData1D)) == NULL)
		return NULL;

	{
		double a0;

		if (sipParseArgs(sipArgs,"d",&a0))
		{
			FloatRegularData1D *ptr;

			if ((ptr = (FloatRegularData1D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData1D)) == NULL)
				return NULL;

			ptr -> FloatRegularData1D::setUpper( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData1D,sipName_BALL_setUpper);

	return NULL;
}

static PyObject *sipDo_FloatRegularData1D_setLower(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData1D)) == NULL)
		return NULL;

	{
		double a0;

		if (sipParseArgs(sipArgs,"d",&a0))
		{
			FloatRegularData1D *ptr;

			if ((ptr = (FloatRegularData1D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData1D)) == NULL)
				return NULL;

			ptr -> FloatRegularData1D::setLower( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData1D,sipName_BALL_setLower);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_FloatRegularData1D(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_FloatRegularData1D)
		return ptr;

	return NULL;
}

static void sipDealloc_FloatRegularData1D(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipFloatRegularData1D *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (FloatRegularData1D *)sipThis -> u.cppPtr;
			else
				delete (sipFloatRegularData1D *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_FloatRegularData1D(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_FloatRegularData1D
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
			sipNew = new sipFloatRegularData1D();
	}
	}

	if (sipNew == NULL)
	{
		const FloatRegularData1D *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_FloatRegularData1D,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_FloatRegularData1D(a0obj,(FloatRegularData1D **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipFloatRegularData1D(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_FloatRegularData1D);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_FloatRegularData1D,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (FloatRegularData1D *)sipNew;
			else
				delete (sipFloatRegularData1D *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipFloatRegularData1D *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_FloatRegularData1D[] = {
	{sipName_BALL_clear, sipDo_FloatRegularData1D_clear, METH_VARARGS, NULL},
	{sipName_BALL_destroy, sipDo_FloatRegularData1D_destroy, METH_VARARGS, NULL},
	{sipName_BALL_getSize, sipDo_FloatRegularData1D_getSize, METH_VARARGS, NULL},
	{sipName_BALL_getLower, sipDo_FloatRegularData1D_getLower, METH_VARARGS, NULL},
	{sipName_BALL_getUpper, sipDo_FloatRegularData1D_getUpper, METH_VARARGS, NULL},
	{sipName_BALL_setUpper, sipDo_FloatRegularData1D_setUpper, METH_VARARGS, NULL},
	{sipName_BALL_setLower, sipDo_FloatRegularData1D_setLower, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_FloatRegularData1D(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_FloatRegularData1D);
}

void sipConvertTo_FloatRegularData1D(PyObject *sipPy,FloatRegularData1D **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_FloatRegularData1D);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (FloatRegularData1D *)sipConvertToCpp(sipPy,sipClass_FloatRegularData1D,sipIsErr);
}

FloatRegularData1D *sipForceConvertTo_FloatRegularData1D(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_FloatRegularData1D(valobj))
	{
		FloatRegularData1D *val;

		sipConvertTo_FloatRegularData1D(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_FloatRegularData1D);
	*iserrp = 1;

	return NULL;
}
