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

sipFloatRegularData1D::sipFloatRegularData1D()
   throw()  : FloatRegularData1D()
{
	sipCommonCtor(sipPyMethods,2);
}

sipFloatRegularData1D::sipFloatRegularData1D(const FloatRegularData1D& a0)
   throw()  : FloatRegularData1D(a0)
{
	sipCommonCtor(sipPyMethods,2);
}

sipFloatRegularData1D::~sipFloatRegularData1D()
  throw()
{
	sipCommonDtor(sipPyThis);
}

void sipFloatRegularData1D::destroy()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_destroy,&relLock))
		sipFloatRegularData1D::sipVH_destroy(&sipPyMethods[0],sipPyThis,relLock);
	else
		FloatRegularData1D::destroy();
}

void sipFloatRegularData1D::clear()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipFloatRegularData1D::sipVH_clear(&sipPyMethods[1],sipPyThis,relLock);
	else
		FloatRegularData1D::clear();
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

static PyObject *sipDo_FloatRegularData1D_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData1D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData1D,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_FloatRegularData1D_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData1D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData1D,sipName_BALL_destroy);

	return NULL;
}

static PyObject *sipDo_FloatRegularData1D_getSize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData1D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData1D,sipName_BALL_getSize);

	return NULL;
}

static PyObject *sipDo_FloatRegularData1D_getLowerBound(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData1D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			FloatRegularData1D *ptr;

			if ((ptr = (FloatRegularData1D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData1D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData1D::getLowerBound();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData1D,sipName_BALL_getLowerBound);

	return NULL;
}

static PyObject *sipDo_FloatRegularData1D_getUpperBound(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData1D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			FloatRegularData1D *ptr;

			if ((ptr = (FloatRegularData1D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData1D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData1D::getUpperBound();

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData1D,sipName_BALL_getUpperBound);

	return NULL;
}

static PyObject *sipDo_FloatRegularData1D_setBoundaries(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData1D)) == NULL)
		return NULL;

	{
		double a0;
		double a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"dd",&a0,&a1))
		{
			FloatRegularData1D *ptr;

			if ((ptr = (FloatRegularData1D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData1D)) == NULL)
				return NULL;

			ptr -> FloatRegularData1D::setBoundaries( a0, a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData1D,sipName_BALL_setBoundaries);

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
	int sipArgsParsed = 0;

	// See if there is something pending.

	sipNew = sipGetPending(&sipFlags);

	if (sipNew == NULL)
	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,"-"))
		{
			sipNew = new sipFloatRegularData1D();
		}
	}

	if (sipNew == NULL)
	{
		const FloatRegularData1D *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_FloatRegularData1D,&a0obj))
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
		sipNoCtor(sipArgsParsed,sipName_BALL_FloatRegularData1D);
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
	{sipName_BALL_getLowerBound, sipDo_FloatRegularData1D_getLowerBound, METH_VARARGS, NULL},
	{sipName_BALL_getUpperBound, sipDo_FloatRegularData1D_getUpperBound, METH_VARARGS, NULL},
	{sipName_BALL_setBoundaries, sipDo_FloatRegularData1D_setBoundaries, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_FloatRegularData1D(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_FloatRegularData1D);
}

void sipConvertTo_FloatRegularData1D(PyObject *sipPy,FloatRegularData1D **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_FloatRegularData1D);
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
