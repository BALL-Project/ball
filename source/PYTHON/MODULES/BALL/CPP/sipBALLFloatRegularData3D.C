#include "sipBALLDeclBALL.h"
#include "sipBALLFloatRegularData3D.h"



PyObject *sipClass_FloatRegularData3D;

static void sipDealloc_FloatRegularData3D(sipThisType *);

static PyTypeObject sipType_FloatRegularData3D = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_FloatRegularData3D,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_FloatRegularData3D,
	0,
	0,
	0,
	0,
	0,
};

sipFloatRegularData3D::sipFloatRegularData3D(): FloatRegularData3D()
{
	sipCommonCtor(sipPyMethods,1);
}

sipFloatRegularData3D::sipFloatRegularData3D(const FloatRegularData3D& a0,bool a1): FloatRegularData3D(a0,a1)
{
	sipCommonCtor(sipPyMethods,1);
}

sipFloatRegularData3D::sipFloatRegularData3D(float a0,float a1,float a2,float a3,float a4,float a5,int a6,int a7,int a8): FloatRegularData3D(a0,a1,a2,a3,a4,a5,a6,a7,a8)
{
	sipCommonCtor(sipPyMethods,1);
}

sipFloatRegularData3D::sipFloatRegularData3D(Vector3& a0,Vector3& a1,int a2,int a3,int a4): FloatRegularData3D(a0,a1,a2,a3,a4)
{
	sipCommonCtor(sipPyMethods,1);
}

sipFloatRegularData3D::sipFloatRegularData3D(const Vector3& a0,const Vector3& a1,float a2): FloatRegularData3D(a0,a1,a2)
{
	sipCommonCtor(sipPyMethods,1);
}

sipFloatRegularData3D::sipFloatRegularData3D(const FloatRegularData3D& a0): FloatRegularData3D(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipFloatRegularData3D::~sipFloatRegularData3D()
{
	sipCommonDtor(sipPyThis);
}
void sipFloatRegularData3D::clear()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipFloatRegularData3D::sipVH_clear(&sipPyMethods[0],sipPyThis,relLock);
	else
		FloatRegularData3D::clear();
}

// The common handler for all classes that inherit this virtual member
// function.

void sipFloatRegularData3D::sipVH_clear(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_FloatRegularData3D,sipName_BALL_clear);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

static PyObject *sipDo_FloatRegularData3D_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			ptr -> FloatRegularData3D::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		const FloatRegularData3D *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_FloatRegularData3D,&a0obj))
		{
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_FloatRegularData3D(a0obj,(FloatRegularData3D **)&a0,1,&iserr);

			if (iserr)
				return NULL;

					try
		{
ptr -> FloatRegularData3D::set(* a0);
		}
		catch (OutOfMemory e)
		{
			PyErr_SetString(PyExc_Exception, "OutOfMemory");
			return NULL;
		}

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData3D::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_isValid);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_getMaxX(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData3D::getMaxX();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_getMaxX);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_getMaxY(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData3D::getMaxY();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_getMaxY);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_getMaxZ(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData3D::getMaxZ();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_getMaxZ);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_getMinX(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData3D::getMinX();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_getMinX);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_getMinY(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData3D::getMinY();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_getMinY);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_getMinZ(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData3D::getMinZ();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_getMinZ);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_getMaxXIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData3D::getMaxXIndex();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_getMaxXIndex);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_getMaxYIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData3D::getMaxYIndex();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_getMaxYIndex);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_getMaxZIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData3D::getMaxZIndex();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_getMaxZIndex);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_getSize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData3D::getSize();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_getSize);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_getXSpacing(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData3D::getXSpacing();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_getXSpacing);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_getYSpacing(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData3D::getYSpacing();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_getYSpacing);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_getZSpacing(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			res = ptr -> FloatRegularData3D::getZSpacing();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_getZSpacing);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_getIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			GridIndex *res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

					try
		{
res = new GridIndex(ptr -> FloatRegularData3D::getIndex(* a0));
		}
		catch (OutOfGrid e)
		{
			PyErr_SetString(PyExc_Exception, "OutOfGrid");
			return NULL;
		}

			return sipNewCppToSelf(res,sipClass_GridIndex,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	{
		float a0;
		float a1;
		float a2;

		if (sipParseArgs(sipArgs,"fff",&a0,&a1,&a2))
		{
			GridIndex *res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

					try
		{
res = new GridIndex(ptr -> FloatRegularData3D::getIndex( a0, a1, a2));
		}
		catch (OutOfGrid e)
		{
			PyErr_SetString(PyExc_Exception, "OutOfGrid");
			return NULL;
		}

			return sipNewCppToSelf(res,sipClass_GridIndex,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_getIndex);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_getGridCoordinates(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		const Position *a0;
		PyObject *a0obj;
		const Position *a1;
		PyObject *a1obj;
		const Position *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"III",sipCanConvertTo_Position,&a0obj,sipCanConvertTo_Position,&a1obj,sipCanConvertTo_Position,&a2obj))
		{
			Vector3 *res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,(Position **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Position(a1obj,(Position **)&a1,1,&iserr);
			int istemp2 = sipConvertTo_Position(a2obj,(Position **)&a2,1,&iserr);

			if (iserr)
				return NULL;

					try
		{
res = new Vector3(ptr -> FloatRegularData3D::getGridCoordinates(* a0,* a1,* a2));
		}
		catch (OutOfGrid e)
		{
			PyErr_SetString(PyExc_Exception, "OutOfGrid");
			return NULL;
		}

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			if (istemp2)
				delete a2;

			return sipNewCppToSelf(res,sipClass_Vector3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			Vector3 *res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

					try
		{
res = new Vector3(ptr -> FloatRegularData3D::getGridCoordinates(* a0));
		}
		catch (OutOfGrid e)
		{
			PyErr_SetString(PyExc_Exception, "OutOfGrid");
			return NULL;
		}

			return sipNewCppToSelf(res,sipClass_Vector3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	{
		const Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			Vector3 *res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,(Position **)&a0,1,&iserr);

			if (iserr)
				return NULL;

					try
		{
res = new Vector3(ptr -> FloatRegularData3D::getGridCoordinates(* a0));
		}
		catch (OutOfGrid e)
		{
			PyErr_SetString(PyExc_Exception, "OutOfGrid");
			return NULL;
		}

			if (istemp0)
				delete a0;

			return sipNewCppToSelf(res,sipClass_Vector3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_getGridCoordinates);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_getBoxIndices(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		const Vector3 *a0;
		PyObject *a0obj;
		Position *a1;
		PyObject *a1obj;
		Position *a2;
		PyObject *a2obj;
		Position *a3;
		PyObject *a3obj;
		Position *a4;
		PyObject *a4obj;
		Position *a5;
		PyObject *a5obj;
		Position *a6;
		PyObject *a6obj;
		Position *a7;
		PyObject *a7obj;
		Position *a8;
		PyObject *a8obj;

		if (sipParseArgs(sipArgs,"IIIIIIIII",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Position,&a1obj,sipCanConvertTo_Position,&a2obj,sipCanConvertTo_Position,&a3obj,sipCanConvertTo_Position,&a4obj,sipCanConvertTo_Position,&a5obj,sipCanConvertTo_Position,&a6obj,sipCanConvertTo_Position,&a7obj,sipCanConvertTo_Position,&a8obj))
		{
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Position(a1obj,&a1,1,&iserr);
			int istemp2 = sipConvertTo_Position(a2obj,&a2,1,&iserr);
			int istemp3 = sipConvertTo_Position(a3obj,&a3,1,&iserr);
			int istemp4 = sipConvertTo_Position(a4obj,&a4,1,&iserr);
			int istemp5 = sipConvertTo_Position(a5obj,&a5,1,&iserr);
			int istemp6 = sipConvertTo_Position(a6obj,&a6,1,&iserr);
			int istemp7 = sipConvertTo_Position(a7obj,&a7,1,&iserr);
			int istemp8 = sipConvertTo_Position(a8obj,&a8,1,&iserr);

			if (iserr)
				return NULL;

					try
		{
ptr -> FloatRegularData3D::getBoxIndices(* a0,* a1,* a2,* a3,* a4,* a5,* a6,* a7,* a8);
		}
		catch (OutOfGrid e)
		{
			PyErr_SetString(PyExc_Exception, "OutOfGrid");
			return NULL;
		}

			if (istemp1)
				delete a1;

			if (istemp2)
				delete a2;

			if (istemp3)
				delete a3;

			if (istemp4)
				delete a4;

			if (istemp5)
				delete a5;

			if (istemp6)
				delete a6;

			if (istemp7)
				delete a7;

			if (istemp8)
				delete a8;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_getBoxIndices);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_getOrigin(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Vector3 *res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			res = &ptr -> FloatRegularData3D::getOrigin();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const Vector3 *res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			res = &ptr -> FloatRegularData3D::getOrigin();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_getOrigin);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_setOrigin(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> FloatRegularData3D::setOrigin(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		float a0;
		float a1;
		float a2;

		if (sipParseArgs(sipArgs,"fff",&a0,&a1,&a2))
		{
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			ptr -> FloatRegularData3D::setOrigin( a0, a1, a2);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_setOrigin);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_getDimension(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Vector3 *res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			res = &ptr -> FloatRegularData3D::getDimension();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const Vector3 *res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			res = &ptr -> FloatRegularData3D::getDimension();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_getDimension);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_has(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			bool res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> FloatRegularData3D::has(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_has);

	return NULL;
}

static PyObject *sipDo_FloatRegularData3D_getInterpolatedValue(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatRegularData3D)) == NULL)
		return NULL;

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			float res;
			FloatRegularData3D *ptr;

			if ((ptr = (FloatRegularData3D *)sipGetCppPtr(sipThis,sipClass_FloatRegularData3D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

					try
		{
res = ptr -> FloatRegularData3D::getInterpolatedValue(* a0);
		}
		catch (OutOfGrid e)
		{
			PyErr_SetString(PyExc_Exception, "OutOfGrid");
			return NULL;
		}

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatRegularData3D,sipName_BALL_getInterpolatedValue);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_FloatRegularData3D(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_FloatRegularData3D)
		return ptr;

	return NULL;
}

static void sipDealloc_FloatRegularData3D(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipFloatRegularData3D *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (FloatRegularData3D *)sipThis -> u.cppPtr;
			else
				delete (sipFloatRegularData3D *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_FloatRegularData3D(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_FloatRegularData3D
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
			sipNew = new sipFloatRegularData3D();
	}
	}

	if (sipNew == NULL)
	{
		const FloatRegularData3D *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_FloatRegularData3D,&a0obj,&a1))
		{
		try
		{
			int iserr = 0;

			sipConvertTo_FloatRegularData3D(a0obj,(FloatRegularData3D **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipFloatRegularData3D(* a0, (bool)a1);
		}
		catch (OutOfMemory e)
		{
			PyErr_SetString(PyExc_Exception, "OutOfMemory");
			return NULL;
		}
	}
	}

	if (sipNew == NULL)
	{
		float a0;
		float a1;
		float a2;
		float a3;
		float a4;
		float a5;
		int a6;
		int a7;
		int a8;

		if (sipParseArgs(sipArgs,"-ffffffiii",&a0,&a1,&a2,&a3,&a4,&a5,&a6,&a7,&a8))
		{
		try
		{
			sipNew = new sipFloatRegularData3D( a0, a1, a2, a3, a4, a5, a6, a7, a8);
		}
		catch (OutOfMemory e)
		{
			PyErr_SetString(PyExc_Exception, "OutOfMemory");
			return NULL;
		}
	}
	}

	if (sipNew == NULL)
	{
		Vector3 *a0;
		PyObject *a0obj;
		Vector3 *a1;
		PyObject *a1obj;
		int a2;
		int a3;
		int a4;

		if (sipParseArgs(sipArgs,"-IIiii",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj,&a2,&a3,&a4))
		{
		try
		{
			int iserr = 0;

			sipConvertTo_Vector3(a0obj,&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipFloatRegularData3D(* a0,* a1, a2, a3, a4);
		}
		catch (OutOfMemory e)
		{
			PyErr_SetString(PyExc_Exception, "OutOfMemory");
			return NULL;
		}
	}
	}

	if (sipNew == NULL)
	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;
		float a2;

		if (sipParseArgs(sipArgs,"-IIf",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj,&a2))
		{
		try
		{
			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipFloatRegularData3D(* a0,* a1, a2);
		}
		catch (OutOfMemory e)
		{
			PyErr_SetString(PyExc_Exception, "OutOfMemory");
			return NULL;
		}
	}
	}

	if (sipNew == NULL)
	{
		const FloatRegularData3D *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_FloatRegularData3D,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_FloatRegularData3D(a0obj,(FloatRegularData3D **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipFloatRegularData3D(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_FloatRegularData3D);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_FloatRegularData3D,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (FloatRegularData3D *)sipNew;
			else
				delete (sipFloatRegularData3D *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipFloatRegularData3D *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_FloatRegularData3D[] = {
	{sipName_BALL_clear, sipDo_FloatRegularData3D_clear, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_FloatRegularData3D_set, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_FloatRegularData3D_isValid, METH_VARARGS, NULL},
	{sipName_BALL_getMaxX, sipDo_FloatRegularData3D_getMaxX, METH_VARARGS, NULL},
	{sipName_BALL_getMaxY, sipDo_FloatRegularData3D_getMaxY, METH_VARARGS, NULL},
	{sipName_BALL_getMaxZ, sipDo_FloatRegularData3D_getMaxZ, METH_VARARGS, NULL},
	{sipName_BALL_getMinX, sipDo_FloatRegularData3D_getMinX, METH_VARARGS, NULL},
	{sipName_BALL_getMinY, sipDo_FloatRegularData3D_getMinY, METH_VARARGS, NULL},
	{sipName_BALL_getMinZ, sipDo_FloatRegularData3D_getMinZ, METH_VARARGS, NULL},
	{sipName_BALL_getMaxXIndex, sipDo_FloatRegularData3D_getMaxXIndex, METH_VARARGS, NULL},
	{sipName_BALL_getMaxYIndex, sipDo_FloatRegularData3D_getMaxYIndex, METH_VARARGS, NULL},
	{sipName_BALL_getMaxZIndex, sipDo_FloatRegularData3D_getMaxZIndex, METH_VARARGS, NULL},
	{sipName_BALL_getSize, sipDo_FloatRegularData3D_getSize, METH_VARARGS, NULL},
	{sipName_BALL_getXSpacing, sipDo_FloatRegularData3D_getXSpacing, METH_VARARGS, NULL},
	{sipName_BALL_getYSpacing, sipDo_FloatRegularData3D_getYSpacing, METH_VARARGS, NULL},
	{sipName_BALL_getZSpacing, sipDo_FloatRegularData3D_getZSpacing, METH_VARARGS, NULL},
	{sipName_BALL_getIndex, sipDo_FloatRegularData3D_getIndex, METH_VARARGS, NULL},
	{sipName_BALL_getGridCoordinates, sipDo_FloatRegularData3D_getGridCoordinates, METH_VARARGS, NULL},
	{sipName_BALL_getBoxIndices, sipDo_FloatRegularData3D_getBoxIndices, METH_VARARGS, NULL},
	{sipName_BALL_getOrigin, sipDo_FloatRegularData3D_getOrigin, METH_VARARGS, NULL},
	{sipName_BALL_setOrigin, sipDo_FloatRegularData3D_setOrigin, METH_VARARGS, NULL},
	{sipName_BALL_getDimension, sipDo_FloatRegularData3D_getDimension, METH_VARARGS, NULL},
	{sipName_BALL_has, sipDo_FloatRegularData3D_has, METH_VARARGS, NULL},
	{sipName_BALL_getInterpolatedValue, sipDo_FloatRegularData3D_getInterpolatedValue, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_FloatRegularData3D(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_FloatRegularData3D);
}

void sipConvertTo_FloatRegularData3D(PyObject *sipPy,FloatRegularData3D **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_FloatRegularData3D);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (FloatRegularData3D *)sipConvertToCpp(sipPy,sipClass_FloatRegularData3D,sipIsErr);
}

FloatRegularData3D *sipForceConvertTo_FloatRegularData3D(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_FloatRegularData3D(valobj))
	{
		FloatRegularData3D *val;

		sipConvertTo_FloatRegularData3D(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_FloatRegularData3D);
	*iserrp = 1;

	return NULL;
}
