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

sipFloatRegularData2D::sipFloatRegularData2D()
   throw()  : FloatRegularData2D()
{
	sipCommonCtor(sipPyMethods,1);
}

sipFloatRegularData2D::sipFloatRegularData2D(const FloatRegularData2D& a0)
   throw(OutOfMemory)  : FloatRegularData2D(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipFloatRegularData2D::sipFloatRegularData2D(float a0,float a1,float a2,float a3,int a4,int a5)
   throw(OutOfMemory)  : FloatRegularData2D(a0,a1,a2,a3,a4,a5)
{
	sipCommonCtor(sipPyMethods,1);
}

sipFloatRegularData2D::sipFloatRegularData2D(const Vector2& a0,const Vector2& a1,int a2,int a3)
   throw(OutOfMemory)  : FloatRegularData2D(a0,a1,a2,a3)
{
	sipCommonCtor(sipPyMethods,1);
}

sipFloatRegularData2D::sipFloatRegularData2D(const Vector2& a0,const Vector2& a1,float a2)
   throw(OutOfMemory)  : FloatRegularData2D(a0,a1,a2)
{
	sipCommonCtor(sipPyMethods,1);
}

sipFloatRegularData2D::~sipFloatRegularData2D()
  throw()
{
	sipCommonDtor(sipPyThis);
}

void sipFloatRegularData2D::clear()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipFloatRegularData2D::sipVH_clear(&sipPyMethods[0],sipPyThis,relLock);
	else
		FloatRegularData2D::clear();
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

static PyObject *sipDo_FloatRegularData2D_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		const FloatRegularData2D *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_FloatRegularData2D,&a0obj))
		{
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_FloatRegularData2D(a0obj,(FloatRegularData2D **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> FloatRegularData2D::set(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData2D::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_isValid);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getMaxX(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData2D::getMaxX();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_getMaxX);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getMaxY(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData2D::getMaxY();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_getMaxY);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getMinX(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData2D::getMinX();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_getMinX);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getMinY(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData2D::getMinY();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_getMinY);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getMaxXIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData2D::getMaxXIndex();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_getMaxXIndex);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getMaxYIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData2D::getMaxYIndex();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_getMaxYIndex);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getSize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_getSize);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getXSpacing(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData2D::getXSpacing();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_getXSpacing);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getYSpacing(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData2D::getYSpacing();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_getYSpacing);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		const Vector2 *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector2,&a0obj))
		{
			GridIndex2D *res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector2(a0obj,(Vector2 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = new GridIndex2D(ptr -> FloatRegularData2D::getIndex(* a0));

			return sipNewCppToSelf(res,sipClass_GridIndex2D,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	{
		float a0;
		float a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"ff",&a0,&a1))
		{
			GridIndex2D *res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = new GridIndex2D(ptr -> FloatRegularData2D::getIndex( a0, a1));

			return sipNewCppToSelf(res,sipClass_GridIndex2D,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_getIndex);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D___getitem__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		const Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			float res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,(Position **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> FloatRegularData2D::operator [](* a0);

			if (istemp0)
				delete a0;

			return PyFloat_FromDouble((double)res);
		}
	}

	{
		const Vector2 *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector2,&a0obj))
		{
			float res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector2(a0obj,(Vector2 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> FloatRegularData2D::operator [](* a0);

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL___getitem__);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getGridCoordinates(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		const Position *a0;
		PyObject *a0obj;
		const Position *a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_Position,&a0obj,sipCanConvertTo_Position,&a1obj))
		{
			Vector2 *res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,(Position **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Position(a1obj,(Position **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = new Vector2(ptr -> FloatRegularData2D::getGridCoordinates(* a0,* a1));

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipNewCppToSelf(res,sipClass_Vector2,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	{
		const Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			Vector2 *res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,(Position **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = new Vector2(ptr -> FloatRegularData2D::getGridCoordinates(* a0));

			if (istemp0)
				delete a0;

			return sipNewCppToSelf(res,sipClass_Vector2,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_getGridCoordinates);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getOrigin(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Vector2 *res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = &ptr -> FloatRegularData2D::getOrigin();

			return sipMapCppToSelf(res,sipClass_Vector2);
		}
	}

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const Vector2 *res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = &ptr -> FloatRegularData2D::getOrigin();

			return sipMapCppToSelf(res,sipClass_Vector2);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_getOrigin);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_setOrigin(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		const Vector2 *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector2,&a0obj))
		{
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector2(a0obj,(Vector2 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> FloatRegularData2D::setOrigin(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		float a0;
		float a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"ff",&a0,&a1))
		{
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			ptr -> FloatRegularData2D::setOrigin( a0, a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_setOrigin);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getDimension(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Vector2 *res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = &ptr -> FloatRegularData2D::getDimension();

			return sipMapCppToSelf(res,sipClass_Vector2);
		}
	}

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const Vector2 *res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = &ptr -> FloatRegularData2D::getDimension();

			return sipMapCppToSelf(res,sipClass_Vector2);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_getDimension);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_has(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		const Vector2 *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector2,&a0obj))
		{
			bool res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector2(a0obj,(Vector2 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> FloatRegularData2D::has(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	{
		float a0;
		float a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"ff",&a0,&a1))
		{
			bool res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData2D::has( a0, a1);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_has);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getInterpolatedValue(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		const Vector2 *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector2,&a0obj))
		{
			float res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector2(a0obj,(Vector2 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> FloatRegularData2D::getInterpolatedValue(* a0);

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_getInterpolatedValue);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getMaxValue(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData2D::getMaxValue();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_getMaxValue);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_getMinValue(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData2D::getMinValue();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_getMinValue);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D_rescale(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		float a0;
		float a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"ff",&a0,&a1))
		{
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			ptr -> FloatRegularData2D::rescale( a0, a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL_rescale);

	return NULL;
}

static PyObject *sipDo_FloatRegularData2D___cmp__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData2D)) == NULL)
		return NULL;

	{
		const FloatRegularData2D *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_FloatRegularData2D,&a0obj))
		{
			bool res;
			FloatRegularData2D *ptr;

			if ((ptr = (FloatRegularData2D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData2D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_FloatRegularData2D(a0obj,(FloatRegularData2D **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> FloatRegularData2D::operator ==(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FloatRegularData2D,sipName_BALL___cmp__);

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
	int sipArgsParsed = 0;

	// See if there is something pending.

	sipNew = sipGetPending(&sipFlags);

	if (sipNew == NULL)
	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,"-"))
		{
			sipNew = new sipFloatRegularData2D();
		}
	}

	if (sipNew == NULL)
	{
		const FloatRegularData2D *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_FloatRegularData2D,&a0obj))
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
		float a0;
		float a1;
		float a2;
		float a3;
		int a4;
		int a5;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-ffffii",&a0,&a1,&a2,&a3,&a4,&a5))
		{
			sipNew = new sipFloatRegularData2D( a0, a1, a2, a3, a4, a5);
		}
	}

	if (sipNew == NULL)
	{
		const Vector2 *a0;
		PyObject *a0obj;
		const Vector2 *a1;
		PyObject *a1obj;
		int a2;
		int a3;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-IIii",sipCanConvertTo_Vector2,&a0obj,sipCanConvertTo_Vector2,&a1obj,&a2,&a3))
		{
			int iserr = 0;

			sipConvertTo_Vector2(a0obj,(Vector2 **)&a0,1,&iserr);
			sipConvertTo_Vector2(a1obj,(Vector2 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipFloatRegularData2D(* a0,* a1, a2, a3);
		}
	}

	if (sipNew == NULL)
	{
		const Vector2 *a0;
		PyObject *a0obj;
		const Vector2 *a1;
		PyObject *a1obj;
		float a2;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-IIf",sipCanConvertTo_Vector2,&a0obj,sipCanConvertTo_Vector2,&a1obj,&a2))
		{
			int iserr = 0;

			sipConvertTo_Vector2(a0obj,(Vector2 **)&a0,1,&iserr);
			sipConvertTo_Vector2(a1obj,(Vector2 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipFloatRegularData2D(* a0,* a1, a2);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_FloatRegularData2D);
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
	{sipName_BALL_set, sipDo_FloatRegularData2D_set, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_FloatRegularData2D_isValid, METH_VARARGS, NULL},
	{sipName_BALL_getMaxX, sipDo_FloatRegularData2D_getMaxX, METH_VARARGS, NULL},
	{sipName_BALL_getMaxY, sipDo_FloatRegularData2D_getMaxY, METH_VARARGS, NULL},
	{sipName_BALL_getMinX, sipDo_FloatRegularData2D_getMinX, METH_VARARGS, NULL},
	{sipName_BALL_getMinY, sipDo_FloatRegularData2D_getMinY, METH_VARARGS, NULL},
	{sipName_BALL_getMaxXIndex, sipDo_FloatRegularData2D_getMaxXIndex, METH_VARARGS, NULL},
	{sipName_BALL_getMaxYIndex, sipDo_FloatRegularData2D_getMaxYIndex, METH_VARARGS, NULL},
	{sipName_BALL_getSize, sipDo_FloatRegularData2D_getSize, METH_VARARGS, NULL},
	{sipName_BALL_getXSpacing, sipDo_FloatRegularData2D_getXSpacing, METH_VARARGS, NULL},
	{sipName_BALL_getYSpacing, sipDo_FloatRegularData2D_getYSpacing, METH_VARARGS, NULL},
	{sipName_BALL_getIndex, sipDo_FloatRegularData2D_getIndex, METH_VARARGS, NULL},
	{sipName_BALL___getitem__, sipDo_FloatRegularData2D___getitem__, METH_VARARGS, NULL},
	{sipName_BALL_getGridCoordinates, sipDo_FloatRegularData2D_getGridCoordinates, METH_VARARGS, NULL},
	{sipName_BALL_getOrigin, sipDo_FloatRegularData2D_getOrigin, METH_VARARGS, NULL},
	{sipName_BALL_setOrigin, sipDo_FloatRegularData2D_setOrigin, METH_VARARGS, NULL},
	{sipName_BALL_getDimension, sipDo_FloatRegularData2D_getDimension, METH_VARARGS, NULL},
	{sipName_BALL_has, sipDo_FloatRegularData2D_has, METH_VARARGS, NULL},
	{sipName_BALL_getInterpolatedValue, sipDo_FloatRegularData2D_getInterpolatedValue, METH_VARARGS, NULL},
	{sipName_BALL_getMaxValue, sipDo_FloatRegularData2D_getMaxValue, METH_VARARGS, NULL},
	{sipName_BALL_getMinValue, sipDo_FloatRegularData2D_getMinValue, METH_VARARGS, NULL},
	{sipName_BALL_rescale, sipDo_FloatRegularData2D_rescale, METH_VARARGS, NULL},
	{sipName_BALL___cmp__, sipDo_FloatRegularData2D___cmp__, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_FloatRegularData2D(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_FloatRegularData2D);
}

void sipConvertTo_FloatRegularData2D(PyObject *sipPy,FloatRegularData2D **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_FloatRegularData2D);
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
