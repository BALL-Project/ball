#include "sipBALLDeclBALL.h"
#include "sipBALLRegularData2D.h"


PyObject *sipClass_RegularData2D;

static void sipDealloc_RegularData2D(sipThisType *);

static PyTypeObject sipType_RegularData2D = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_RegularData2D,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_RegularData2D,
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

sipRegularData2D::sipRegularData2D()
   throw() : RegularData2D()
{
	sipCommonCtor(sipPyMethods,1);
}

sipRegularData2D::sipRegularData2D(const RegularData2D& a0)
   throw(OutOfMemory) : RegularData2D(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipRegularData2D::sipRegularData2D(float a0,float a1,float a2,float a3,int a4,int a5)
   throw(OutOfMemory) : RegularData2D(a0,a1,a2,a3,a4,a5)
{
	sipCommonCtor(sipPyMethods,1);
}

sipRegularData2D::sipRegularData2D(const Vector2& a0,const Vector2& a1,int a2,int a3)
   throw(OutOfMemory) : RegularData2D(a0,a1,a2,a3)
{
	sipCommonCtor(sipPyMethods,1);
}

sipRegularData2D::sipRegularData2D(const Vector2& a0,const Vector2& a1,float a2)
   throw(OutOfMemory) : RegularData2D(a0,a1,a2)
{
	sipCommonCtor(sipPyMethods,1);
}

sipRegularData2D::~sipRegularData2D()
 throw()
{
	sipCommonDtor(sipPyThis);
}

void sipRegularData2D::clear()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipRegularData2D::sipVH_clear(&sipPyMethods[0],sipPyThis,relLock);
	else
		RegularData2D::clear();
}

// The common handler for all classes that inherit this virtual member
// function.

void sipRegularData2D::sipVH_clear(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_RegularData2D,sipName_BALL_clear);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

extern "C" PyObject *sipDo_RegularData2D_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			ptr -> RegularData2D::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_clear);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		const RegularData2D * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_RegularData2D,&a0obj))
		{
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_RegularData2D(a0obj,(RegularData2D **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> RegularData2D::set(* a0);
   }
   catch (OutOfMemory e)
   {
      OutOfMemory *my_exception = new OutOfMemory(e);
      PyErr_SetObject(sipClass_OutOfMemory, sipNewCppToSelf(my_exception,sipClass_OutOfMemory,SIP_SIMPLE | SIP_PY_OWNED));
      return NULL;
		}

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_set);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			res = ptr -> RegularData2D::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_isValid);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_getMaxX(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			res = ptr -> RegularData2D::getMaxX();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_getMaxX);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_getMaxY(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			res = ptr -> RegularData2D::getMaxY();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_getMaxY);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_getMinX(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			res = ptr -> RegularData2D::getMinX();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_getMinX);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_getMinY(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			res = ptr -> RegularData2D::getMinY();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_getMinY);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_getMaxXIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			res = ptr -> RegularData2D::getMaxXIndex();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_getMaxXIndex);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_getMaxYIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			res = ptr -> RegularData2D::getMaxYIndex();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_getMaxYIndex);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_getSize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			res = ptr -> RegularData2D::getSize();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_getSize);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_getXSpacing(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			res = ptr -> RegularData2D::getXSpacing();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_getXSpacing);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_getYSpacing(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			res = ptr -> RegularData2D::getYSpacing();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_getYSpacing);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_getIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		const Vector2 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector2,&a0obj))
		{
			RegularData2D::GridIndex *res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector2(a0obj,(Vector2 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = new RegularData2D::GridIndex(ptr -> RegularData2D::getIndex(* a0));
   }
   catch (OutOfGrid e)
   {
      OutOfGrid *my_exception = new OutOfGrid(e);
      PyErr_SetObject(sipClass_OutOfGrid, sipNewCppToSelf(my_exception,sipClass_OutOfGrid,SIP_SIMPLE | SIP_PY_OWNED));
      return NULL;
		}

			return sipNewCppToSelf(res,sipClass_RegularData2D_GridIndex,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	{
		float a0;
		float a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"ff",&a0,&a1))
		{
			RegularData2D::GridIndex *res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

   try
   {
			res = new RegularData2D::GridIndex(ptr -> RegularData2D::getIndex( a0, a1));
   }
   catch (OutOfGrid e)
   {
      OutOfGrid *my_exception = new OutOfGrid(e);
      PyErr_SetObject(sipClass_OutOfGrid, sipNewCppToSelf(my_exception,sipClass_OutOfGrid,SIP_SIMPLE | SIP_PY_OWNED));
      return NULL;
		}

			return sipNewCppToSelf(res,sipClass_RegularData2D_GridIndex,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_getIndex);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_GetItemOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		const Vector2 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector2,&a0obj))
		{
			float res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector2(a0obj,(Vector2 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> RegularData2D::operator[](* a0);
   }
   catch (OutOfGrid e)
   {
      OutOfGrid *my_exception = new OutOfGrid(e);
      PyErr_SetObject(sipClass_OutOfGrid, sipNewCppToSelf(my_exception,sipClass_OutOfGrid,SIP_SIMPLE | SIP_PY_OWNED));
      return NULL;
		}

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_GetItemOp);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_getGridCoordinates(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		const Position * a0;
		PyObject *a0obj;
		const Position * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_Position,&a0obj,sipCanConvertTo_Position,&a1obj))
		{
			Vector2 *res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,(Position **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Position(a1obj,(Position **)&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = new Vector2(ptr -> RegularData2D::getGridCoordinates(* a0,* a1));
   }
   catch (OutOfGrid e)
   {
      OutOfGrid *my_exception = new OutOfGrid(e);
      PyErr_SetObject(sipClass_OutOfGrid, sipNewCppToSelf(my_exception,sipClass_OutOfGrid,SIP_SIMPLE | SIP_PY_OWNED));
      return NULL;
		}

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			return sipNewCppToSelf(res,sipClass_Vector2,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	{
		const Position * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			Vector2 *res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,(Position **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = new Vector2(ptr -> RegularData2D::getGridCoordinates(* a0));
   }
   catch (OutOfGrid e)
   {
      OutOfGrid *my_exception = new OutOfGrid(e);
      PyErr_SetObject(sipClass_OutOfGrid, sipNewCppToSelf(my_exception,sipClass_OutOfGrid,SIP_SIMPLE | SIP_PY_OWNED));
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipNewCppToSelf(res,sipClass_Vector2,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_getGridCoordinates);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_getOrigin(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Vector2 *res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			res = &ptr -> RegularData2D::getOrigin();

			return sipMapCppToSelf(res,sipClass_Vector2);
		}
	}

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const Vector2 *res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			res = &ptr -> RegularData2D::getOrigin();

			return sipMapCppToSelf(res,sipClass_Vector2);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_getOrigin);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_setOrigin(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		const Vector2 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector2,&a0obj))
		{
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector2(a0obj,(Vector2 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> RegularData2D::setOrigin(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		float a0;
		float a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"ff",&a0,&a1))
		{
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			ptr -> RegularData2D::setOrigin( a0, a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_setOrigin);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_getDimension(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Vector2 *res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			res = &ptr -> RegularData2D::getDimension();

			return sipMapCppToSelf(res,sipClass_Vector2);
		}
	}

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const Vector2 *res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			res = &ptr -> RegularData2D::getDimension();

			return sipMapCppToSelf(res,sipClass_Vector2);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_getDimension);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_has(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		const Vector2 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector2,&a0obj))
		{
			bool res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector2(a0obj,(Vector2 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> RegularData2D::has(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	{
		float a0;
		float a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"ff",&a0,&a1))
		{
			bool res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			res = ptr -> RegularData2D::has( a0, a1);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_has);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_getInterpolatedValue(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		const Vector2 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector2,&a0obj))
		{
			float res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector2(a0obj,(Vector2 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> RegularData2D::getInterpolatedValue(* a0);
   }
   catch (OutOfGrid e)
   {
      OutOfGrid *my_exception = new OutOfGrid(e);
      PyErr_SetObject(sipClass_OutOfGrid, sipNewCppToSelf(my_exception,sipClass_OutOfGrid,SIP_SIMPLE | SIP_PY_OWNED));
      return NULL;
		}

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_getInterpolatedValue);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_getMaxValue(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			res = ptr -> RegularData2D::getMaxValue();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_getMaxValue);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_getMinValue(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			res = ptr -> RegularData2D::getMinValue();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_getMinValue);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_rescale(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		float a0;
		float a1;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"ff",&a0,&a1))
		{
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			ptr -> RegularData2D::rescale( a0, a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_rescale);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData2D_CmpOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData2D)) == NULL)
		return NULL;

	{
		const RegularData2D * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_RegularData2D,&a0obj))
		{
			bool res;
			RegularData2D *ptr;

			if ((ptr = (RegularData2D *)sipGetCppPtr(sipThis,sipClass_RegularData2D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_RegularData2D(a0obj,(RegularData2D **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> RegularData2D::operator==(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData2D,sipName_BALL_CmpOp);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_RegularData2D(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_RegularData2D)
		return ptr;

	if ((res = sipCast_OutOfMemory((OutOfMemory *)(RegularData2D *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_OutOfMemory((OutOfMemory *)(RegularData2D *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_OutOfMemory((OutOfMemory *)(RegularData2D *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_OutOfMemory((OutOfMemory *)(RegularData2D *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_OutOfMemory((OutOfMemory *)(RegularData2D *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_OutOfGrid((OutOfGrid *)(RegularData2D *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_OutOfGrid((OutOfGrid *)(RegularData2D *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_OutOfGrid((OutOfGrid *)(RegularData2D *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_OutOfGrid((OutOfGrid *)(RegularData2D *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_OutOfGrid((OutOfGrid *)(RegularData2D *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_OutOfGrid((OutOfGrid *)(RegularData2D *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_RegularData2D(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipRegularData2D *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (RegularData2D *)sipThis -> u.cppPtr;
			else
				delete (sipRegularData2D *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_RegularData2D(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_RegularData2D
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
			sipNew = new sipRegularData2D();
		}
	}

	if (sipNew == NULL)
	{
		const RegularData2D * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_RegularData2D,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_RegularData2D(a0obj,(RegularData2D **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipRegularData2D(* a0);
   }
   catch (OutOfMemory e)
   {
      OutOfMemory *my_exception = new OutOfMemory(e);
      PyErr_SetObject(sipClass_OutOfMemory, sipNewCppToSelf(my_exception,sipClass_OutOfMemory,SIP_SIMPLE | SIP_PY_OWNED));
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
		int a4;
		int a5;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-ffffii",&a0,&a1,&a2,&a3,&a4,&a5))
		{
   try
   {
			sipNew = new sipRegularData2D( a0, a1, a2, a3, a4, a5);
   }
   catch (OutOfMemory e)
   {
      OutOfMemory *my_exception = new OutOfMemory(e);
      PyErr_SetObject(sipClass_OutOfMemory, sipNewCppToSelf(my_exception,sipClass_OutOfMemory,SIP_SIMPLE | SIP_PY_OWNED));
      return NULL;
		}
		}
	}

	if (sipNew == NULL)
	{
		const Vector2 * a0;
		PyObject *a0obj;
		const Vector2 * a1;
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

   try
   {
			sipNew = new sipRegularData2D(* a0,* a1, a2, a3);
   }
   catch (OutOfMemory e)
   {
      OutOfMemory *my_exception = new OutOfMemory(e);
      PyErr_SetObject(sipClass_OutOfMemory, sipNewCppToSelf(my_exception,sipClass_OutOfMemory,SIP_SIMPLE | SIP_PY_OWNED));
      return NULL;
		}
		}
	}

	if (sipNew == NULL)
	{
		const Vector2 * a0;
		PyObject *a0obj;
		const Vector2 * a1;
		PyObject *a1obj;
		float a2;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-IIf",sipCanConvertTo_Vector2,&a0obj,sipCanConvertTo_Vector2,&a1obj,&a2))
		{
			int iserr = 0;

			sipConvertTo_Vector2(a0obj,(Vector2 **)&a0,1,&iserr);
			sipConvertTo_Vector2(a1obj,(Vector2 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipRegularData2D(* a0,* a1, a2);
   }
   catch (OutOfMemory e)
   {
      OutOfMemory *my_exception = new OutOfMemory(e);
      PyErr_SetObject(sipClass_OutOfMemory, sipNewCppToSelf(my_exception,sipClass_OutOfMemory,SIP_SIMPLE | SIP_PY_OWNED));
      return NULL;
		}
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_RegularData2D);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_RegularData2D,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (RegularData2D *)sipNew;
			else
				delete (sipRegularData2D *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipRegularData2D *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_RegularData2D[] = {
	{sipName_BALL_clear, sipDo_RegularData2D_clear, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_RegularData2D_set, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_RegularData2D_isValid, METH_VARARGS, NULL},
	{sipName_BALL_getMaxX, sipDo_RegularData2D_getMaxX, METH_VARARGS, NULL},
	{sipName_BALL_getMaxY, sipDo_RegularData2D_getMaxY, METH_VARARGS, NULL},
	{sipName_BALL_getMinX, sipDo_RegularData2D_getMinX, METH_VARARGS, NULL},
	{sipName_BALL_getMinY, sipDo_RegularData2D_getMinY, METH_VARARGS, NULL},
	{sipName_BALL_getMaxXIndex, sipDo_RegularData2D_getMaxXIndex, METH_VARARGS, NULL},
	{sipName_BALL_getMaxYIndex, sipDo_RegularData2D_getMaxYIndex, METH_VARARGS, NULL},
	{sipName_BALL_getSize, sipDo_RegularData2D_getSize, METH_VARARGS, NULL},
	{sipName_BALL_getXSpacing, sipDo_RegularData2D_getXSpacing, METH_VARARGS, NULL},
	{sipName_BALL_getYSpacing, sipDo_RegularData2D_getYSpacing, METH_VARARGS, NULL},
	{sipName_BALL_getIndex, sipDo_RegularData2D_getIndex, METH_VARARGS, NULL},
	{sipName_BALL_GetItemOp, sipDo_RegularData2D_GetItemOp, METH_VARARGS, NULL},
	{sipName_BALL_getGridCoordinates, sipDo_RegularData2D_getGridCoordinates, METH_VARARGS, NULL},
	{sipName_BALL_getOrigin, sipDo_RegularData2D_getOrigin, METH_VARARGS, NULL},
	{sipName_BALL_setOrigin, sipDo_RegularData2D_setOrigin, METH_VARARGS, NULL},
	{sipName_BALL_getDimension, sipDo_RegularData2D_getDimension, METH_VARARGS, NULL},
	{sipName_BALL_has, sipDo_RegularData2D_has, METH_VARARGS, NULL},
	{sipName_BALL_getInterpolatedValue, sipDo_RegularData2D_getInterpolatedValue, METH_VARARGS, NULL},
	{sipName_BALL_getMaxValue, sipDo_RegularData2D_getMaxValue, METH_VARARGS, NULL},
	{sipName_BALL_getMinValue, sipDo_RegularData2D_getMinValue, METH_VARARGS, NULL},
	{sipName_BALL_rescale, sipDo_RegularData2D_rescale, METH_VARARGS, NULL},
	{sipName_BALL_CmpOp, sipDo_RegularData2D_CmpOp, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_RegularData2D(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_RegularData2D);
}

void sipConvertTo_RegularData2D(PyObject *sipPy,RegularData2D **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_RegularData2D);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (RegularData2D *)sipConvertToCpp(sipPy,sipClass_RegularData2D,sipIsErr);
}

RegularData2D *sipForceConvertTo_RegularData2D(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_RegularData2D(valobj))
	{
		RegularData2D *val;

		sipConvertTo_RegularData2D(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_RegularData2D);

	*iserrp = 1;

	return NULL;
}
