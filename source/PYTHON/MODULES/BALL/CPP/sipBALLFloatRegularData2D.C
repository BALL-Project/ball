#include "sipBALLDeclBALL.h"
#include "sipBALLFloatRegularData2D.h"



PyObject *sipClass_FloatRegularData2D;

static void sipDealloc_FloatRegularData2D(sipThisType *);

static PyTypeObject sipType_FloatRegularData2D = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_FloatRegularData2D,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_FloatRegularData2D,
	0,
	0,
	0,
	0,
	0,
};

sipFloatRegularData2D::sipFloatRegularData2D(): FloatRegularData2D()
{
	sipCommonCtor(sipPyMethods,2);
}

sipFloatRegularData2D::sipFloatRegularData2D(const FloatRegularData2D& a0): FloatRegularData2D(a0)
{
	sipCommonCtor(sipPyMethods,2);
}

sipFloatRegularData2D::~sipFloatRegularData2D()
{
	sipCommonDtor(sipPyThis);
}
void sipFloatRegularData2D::clear()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipFloatRegularData2D::sipVH_clear(&sipPyMethods[0],sipPyThis,relLock);
	else
		FloatRegularData2D::clear();
}
void sipFloatRegularData2D::destroy()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_destroy,&relLock))
		sipFloatRegularData2D::sipVH_destroy(&sipPyMethods[1],sipPyThis,relLock);
	else
		FloatRegularData2D::destroy();
}

// The common handler for all classes that inherit this virtual member
// function.

void sipFloatRegularData2D::sipVH_clear(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_FloatRegularData2D,sipName_BALL_clear);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipFloatRegularData2D::sipVH_destroy(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_FloatRegularData2D,sipName_BALL_destroy);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

static PyObject *sipDo_FloatRegularData2D_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			ptr -> FloatRegularData2D::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData2D,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			ptr -> FloatRegularData2D::destroy();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData2D,sipName_BALL_destroy);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_createGroundState(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			ptr -> FloatRegularData2D::createGroundState();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData2D,sipName_BALL_createGroundState);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getSize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData2D::getSize();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData2D,sipName_BALL_getSize);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getXSize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData2D::getXSize();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData2D,sipName_BALL_getXSize);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getYSize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData2D::getYSize();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData2D,sipName_BALL_getYSize);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getXLower(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			double res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData2D::getXLower();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData2D,sipName_BALL_getXLower);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getXUpper(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			double res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData2D::getXUpper();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData2D,sipName_BALL_getXUpper);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getYLower(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			double res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData2D::getYLower();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData2D,sipName_BALL_getYLower);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getYUpper(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			double res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData2D::getYUpper();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData2D,sipName_BALL_getYUpper);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_setUpperBound(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		float a0;

		if (sipParseArgs(sipArgs,"f",&a0))
		{
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			ptr -> FloatRegularData2D::setUpperBound( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData2D,sipName_BALL_setUpperBound);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_setLowerBound(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		float a0;

		if (sipParseArgs(sipArgs,"f",&a0))
		{
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			ptr -> FloatRegularData2D::setLowerBound( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData2D,sipName_BALL_setLowerBound);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_setXSize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(sipArgs,"i",&a0))
		{
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			ptr -> FloatRegularData2D::setXSize( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData2D,sipName_BALL_setXSize);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_setYSize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(sipArgs,"i",&a0))
		{
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			ptr -> FloatRegularData2D::setYSize( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData2D,sipName_BALL_setYSize);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_setXUpper(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		double a0;

		if (sipParseArgs(sipArgs,"d",&a0))
		{
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			ptr -> FloatRegularData2D::setXUpper( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData2D,sipName_BALL_setXUpper);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_setXLower(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		double a0;

		if (sipParseArgs(sipArgs,"d",&a0))
		{
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			ptr -> FloatRegularData2D::setXLower( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData2D,sipName_BALL_setXLower);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_setYUpper(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		double a0;

		if (sipParseArgs(sipArgs,"d",&a0))
		{
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			ptr -> FloatRegularData2D::setYUpper( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData2D,sipName_BALL_setYUpper);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_setYLower(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		double a0;

		if (sipParseArgs(sipArgs,"d",&a0))
		{
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			ptr -> FloatRegularData2D::setYLower( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData2D,sipName_BALL_setYLower);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_resize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		int a0;
		int a1;

		if (sipParseArgs(sipArgs,"ii",&a0,&a1))
		{
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			ptr -> FloatRegularData2D::resize( a0, a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData2D,sipName_BALL_resize);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_FloatRegularData2D(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_FloatRegularData2D)
		return ptr;

	return NULL;
}

static void sipDealloc_FloatRegularData2D(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipFloatRegularData2D *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (FloatRegularData2D *)sipThis -> u.cppPtr;
			else
				delete (sipFloatRegularData2D *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_FloatRegularData2D(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_FloatRegularData2D
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
			sipNew = new sipFloatRegularData2D();
	}
	}

	if (sipNew == NULL)
	{
		const FloatRegularData2D *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_FloatRegularData2D,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_FloatRegularData2D(a0obj,(FloatRegularData2D **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipFloatRegularData2D(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_FloatRegularData2D);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_FloatRegularData2D,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (FloatRegularData2D *)sipNew;
			else
				delete (sipFloatRegularData2D *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipFloatRegularData2D *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_FloatRegularData2D[] = {
	{sipName_BALL_clear, sipDo_FloatRegularData2D_clear, METH_VARARGS, NULL},
	{sipName_BALL_destroy, sipDo_FloatRegularData2D_destroy, METH_VARARGS, NULL},
	{sipName_BALL_createGroundState, sipDo_FloatRegularData2D_createGroundState, METH_VARARGS, NULL},
	{sipName_BALL_getSize, sipDo_FloatRegularData2D_getSize, METH_VARARGS, NULL},
	{sipName_BALL_getXSize, sipDo_FloatRegularData2D_getXSize, METH_VARARGS, NULL},
	{sipName_BALL_getYSize, sipDo_FloatRegularData2D_getYSize, METH_VARARGS, NULL},
	{sipName_BALL_getXLower, sipDo_FloatRegularData2D_getXLower, METH_VARARGS, NULL},
	{sipName_BALL_getXUpper, sipDo_FloatRegularData2D_getXUpper, METH_VARARGS, NULL},
	{sipName_BALL_getYLower, sipDo_FloatRegularData2D_getYLower, METH_VARARGS, NULL},
	{sipName_BALL_getYUpper, sipDo_FloatRegularData2D_getYUpper, METH_VARARGS, NULL},
	{sipName_BALL_setUpperBound, sipDo_FloatRegularData2D_setUpperBound, METH_VARARGS, NULL},
	{sipName_BALL_setLowerBound, sipDo_FloatRegularData2D_setLowerBound, METH_VARARGS, NULL},
	{sipName_BALL_setXSize, sipDo_FloatRegularData2D_setXSize, METH_VARARGS, NULL},
	{sipName_BALL_setYSize, sipDo_FloatRegularData2D_setYSize, METH_VARARGS, NULL},
	{sipName_BALL_setXUpper, sipDo_FloatRegularData2D_setXUpper, METH_VARARGS, NULL},
	{sipName_BALL_setXLower, sipDo_FloatRegularData2D_setXLower, METH_VARARGS, NULL},
	{sipName_BALL_setYUpper, sipDo_FloatRegularData2D_setYUpper, METH_VARARGS, NULL},
	{sipName_BALL_setYLower, sipDo_FloatRegularData2D_setYLower, METH_VARARGS, NULL},
	{sipName_BALL_resize, sipDo_FloatRegularData2D_resize, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_FloatRegularData2D(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_FloatRegularData2D);
}

void sipConvertTo_FloatRegularData2D(PyObject *sipPy,FloatRegularData2D **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_FloatRegularData2D);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (FloatRegularData2D *)sipConvertToCpp(sipPy,sipClass_FloatRegularData2D,sipIsErr);
}

FloatRegularData2D *sipForceConvertTo_FloatRegularData2D(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_FloatRegularData2D(valobj))
	{
		FloatRegularData2D *val;

		sipConvertTo_FloatRegularData2D(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_FloatRegularData2D);
	*iserrp = 1;

	return NULL;
}
