#include "sipBALLDeclBALL.h"
#include "sipBALLRegularData3D.h"


PyObject *sipClass_RegularData3D;

static void sipDealloc_RegularData3D(sipThisType *);

static PyTypeObject sipType_RegularData3D = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_RegularData3D,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_RegularData3D,
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

sipRegularData3D::sipRegularData3D()
   throw() : RegularData3D()
{
	sipCommonCtor(sipPyMethods,1);
}

sipRegularData3D::sipRegularData3D(const RegularData3D& a0)
   throw(OutOfMemory) : RegularData3D(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipRegularData3D::sipRegularData3D(float a0,float a1,float a2,float a3,float a4,float a5,int a6,int a7,int a8)
   throw(OutOfMemory) : RegularData3D(a0,a1,a2,a3,a4,a5,a6,a7,a8)
{
	sipCommonCtor(sipPyMethods,1);
}

sipRegularData3D::sipRegularData3D(Vector3& a0,Vector3& a1,int a2,int a3,int a4)
   throw(OutOfMemory) : RegularData3D(a0,a1,a2,a3,a4)
{
	sipCommonCtor(sipPyMethods,1);
}

sipRegularData3D::sipRegularData3D(const Vector3& a0,const Vector3& a1,float a2)
   throw(OutOfMemory) : RegularData3D(a0,a1,a2)
{
	sipCommonCtor(sipPyMethods,1);
}

sipRegularData3D::~sipRegularData3D()
 throw()
{
	sipCommonDtor(sipPyThis);
}

void sipRegularData3D::clear()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipRegularData3D::sipVH_clear(&sipPyMethods[0],sipPyThis,relLock);
	else
		RegularData3D::clear();
}

// The common handler for all classes that inherit this virtual member
// function.

void sipRegularData3D::sipVH_clear(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_RegularData3D,sipName_BALL_clear);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

extern "C" PyObject *sipDo_RegularData3D_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			ptr -> RegularData3D::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_clear);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		const RegularData3D * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_RegularData3D,&a0obj))
		{
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_RegularData3D(a0obj,(RegularData3D **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> RegularData3D::set(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_set);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			res = ptr -> RegularData3D::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_isValid);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_getMaxX(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			res = ptr -> RegularData3D::getMaxX();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_getMaxX);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_getMaxY(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			res = ptr -> RegularData3D::getMaxY();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_getMaxY);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_getMaxZ(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			res = ptr -> RegularData3D::getMaxZ();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_getMaxZ);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_getMinX(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			res = ptr -> RegularData3D::getMinX();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_getMinX);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_getMinY(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			res = ptr -> RegularData3D::getMinY();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_getMinY);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_getMinZ(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			res = ptr -> RegularData3D::getMinZ();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_getMinZ);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_getMaxXIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			res = ptr -> RegularData3D::getMaxXIndex();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_getMaxXIndex);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_getMaxYIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			res = ptr -> RegularData3D::getMaxYIndex();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_getMaxYIndex);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_getMaxZIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			res = ptr -> RegularData3D::getMaxZIndex();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_getMaxZIndex);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_getSize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			res = ptr -> RegularData3D::getSize();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_getSize);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_getXSpacing(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			res = ptr -> RegularData3D::getXSpacing();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_getXSpacing);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_getYSpacing(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			res = ptr -> RegularData3D::getYSpacing();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_getYSpacing);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_getZSpacing(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			res = ptr -> RegularData3D::getZSpacing();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_getZSpacing);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_getIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		const Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			RegularData3D::GridIndex *res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = new RegularData3D::GridIndex(ptr -> RegularData3D::getIndex(* a0));
   }
   catch (OutOfGrid e)
   {
      OutOfGrid *my_exception = new OutOfGrid(e);
      PyErr_SetObject(sipClass_OutOfGrid, sipNewCppToSelf(my_exception,sipClass_OutOfGrid,SIP_SIMPLE | SIP_PY_OWNED));
      return NULL;
		}

			return sipNewCppToSelf(res,sipClass_RegularData3D_GridIndex,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	{
		float a0;
		float a1;
		float a2;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"fff",&a0,&a1,&a2))
		{
			RegularData3D::GridIndex *res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

   try
   {
			res = new RegularData3D::GridIndex(ptr -> RegularData3D::getIndex( a0, a1, a2));
   }
   catch (OutOfGrid e)
   {
      OutOfGrid *my_exception = new OutOfGrid(e);
      PyErr_SetObject(sipClass_OutOfGrid, sipNewCppToSelf(my_exception,sipClass_OutOfGrid,SIP_SIMPLE | SIP_PY_OWNED));
      return NULL;
		}

			return sipNewCppToSelf(res,sipClass_RegularData3D_GridIndex,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_getIndex);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_getGridCoordinates(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		const Position * a0;
		PyObject *a0obj;
		const Position * a1;
		PyObject *a1obj;
		const Position * a2;
		PyObject *a2obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"III",sipCanConvertTo_Position,&a0obj,sipCanConvertTo_Position,&a1obj,sipCanConvertTo_Position,&a2obj))
		{
			Vector3 *res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,(Position **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Position(a1obj,(Position **)&a1,1,&iserr);
			int istemp2 = sipConvertTo_Position(a2obj,(Position **)&a2,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = new Vector3(ptr -> RegularData3D::getGridCoordinates(* a0,* a1,* a2));
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

			if (istemp2)
				delete a2;

			return sipNewCppToSelf(res,sipClass_Vector3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	{
		const Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			Vector3 *res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = new Vector3(ptr -> RegularData3D::getGridCoordinates(* a0));
   }
   catch (OutOfGrid e)
   {
      OutOfGrid *my_exception = new OutOfGrid(e);
      PyErr_SetObject(sipClass_OutOfGrid, sipNewCppToSelf(my_exception,sipClass_OutOfGrid,SIP_SIMPLE | SIP_PY_OWNED));
      return NULL;
		}

			return sipNewCppToSelf(res,sipClass_Vector3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	{
		const Position * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			Vector3 *res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,(Position **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = new Vector3(ptr -> RegularData3D::getGridCoordinates(* a0));
   }
   catch (OutOfGrid e)
   {
      OutOfGrid *my_exception = new OutOfGrid(e);
      PyErr_SetObject(sipClass_OutOfGrid, sipNewCppToSelf(my_exception,sipClass_OutOfGrid,SIP_SIMPLE | SIP_PY_OWNED));
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipNewCppToSelf(res,sipClass_Vector3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_getGridCoordinates);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_getBoxIndices(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		const Vector3 * a0;
		PyObject *a0obj;
		Position * a1;
		PyObject *a1obj;
		Position * a2;
		PyObject *a2obj;
		Position * a3;
		PyObject *a3obj;
		Position * a4;
		PyObject *a4obj;
		Position * a5;
		PyObject *a5obj;
		Position * a6;
		PyObject *a6obj;
		Position * a7;
		PyObject *a7obj;
		Position * a8;
		PyObject *a8obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"IIIIIIIII",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Position,&a1obj,sipCanConvertTo_Position,&a2obj,sipCanConvertTo_Position,&a3obj,sipCanConvertTo_Position,&a4obj,sipCanConvertTo_Position,&a5obj,sipCanConvertTo_Position,&a6obj,sipCanConvertTo_Position,&a7obj,sipCanConvertTo_Position,&a8obj))
		{
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
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
			ptr -> RegularData3D::getBoxIndices(* a0,* a1,* a2,* a3,* a4,* a5,* a6,* a7,* a8);
   }
   catch (OutOfGrid e)
   {
      OutOfGrid *my_exception = new OutOfGrid(e);
      PyErr_SetObject(sipClass_OutOfGrid, sipNewCppToSelf(my_exception,sipClass_OutOfGrid,SIP_SIMPLE | SIP_PY_OWNED));
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

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_getBoxIndices);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_getOrigin(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Vector3 *res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			res = &ptr -> RegularData3D::getOrigin();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const Vector3 *res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			res = &ptr -> RegularData3D::getOrigin();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_getOrigin);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_setOrigin(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		const Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> RegularData3D::setOrigin(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		float a0;
		float a1;
		float a2;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"fff",&a0,&a1,&a2))
		{
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			ptr -> RegularData3D::setOrigin( a0, a1, a2);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_setOrigin);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_getDimension(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Vector3 *res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			res = &ptr -> RegularData3D::getDimension();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const Vector3 *res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			res = &ptr -> RegularData3D::getDimension();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_getDimension);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_has(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		const Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			bool res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> RegularData3D::has(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_has);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_getInterpolatedValue(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		const Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			float res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> RegularData3D::getInterpolatedValue(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_getInterpolatedValue);

	return NULL;
}

extern "C" PyObject *sipDo_RegularData3D_CmpOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_RegularData3D)) == NULL)
		return NULL;

	{
		const RegularData3D * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_RegularData3D,&a0obj))
		{
			bool res;
			RegularData3D *ptr;

			if ((ptr = (RegularData3D *)sipGetCppPtr(sipThis,sipClass_RegularData3D)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_RegularData3D(a0obj,(RegularData3D **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> RegularData3D::operator==(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_RegularData3D,sipName_BALL_CmpOp);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_RegularData3D(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_RegularData3D)
		return ptr;

	if ((res = sipCast_OutOfMemory((OutOfMemory *)(RegularData3D *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_OutOfMemory((OutOfMemory *)(RegularData3D *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_OutOfMemory((OutOfMemory *)(RegularData3D *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_OutOfMemory((OutOfMemory *)(RegularData3D *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_OutOfMemory((OutOfMemory *)(RegularData3D *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_OutOfGrid((OutOfGrid *)(RegularData3D *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_OutOfGrid((OutOfGrid *)(RegularData3D *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_OutOfGrid((OutOfGrid *)(RegularData3D *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_OutOfGrid((OutOfGrid *)(RegularData3D *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_OutOfGrid((OutOfGrid *)(RegularData3D *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_OutOfGrid((OutOfGrid *)(RegularData3D *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_OutOfGrid((OutOfGrid *)(RegularData3D *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_RegularData3D(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipRegularData3D *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (RegularData3D *)sipThis -> u.cppPtr;
			else
				delete (sipRegularData3D *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_RegularData3D(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_RegularData3D
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
			sipNew = new sipRegularData3D();
		}
	}

	if (sipNew == NULL)
	{
		const RegularData3D * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_RegularData3D,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_RegularData3D(a0obj,(RegularData3D **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipRegularData3D(* a0);
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
		float a4;
		float a5;
		int a6;
		int a7;
		int a8;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-ffffffiii",&a0,&a1,&a2,&a3,&a4,&a5,&a6,&a7,&a8))
		{
   try
   {
			sipNew = new sipRegularData3D( a0, a1, a2, a3, a4, a5, a6, a7, a8);
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
		Vector3 * a0;
		PyObject *a0obj;
		Vector3 * a1;
		PyObject *a1obj;
		int a2;
		int a3;
		int a4;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-IIiii",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj,&a2,&a3,&a4))
		{
			int iserr = 0;

			sipConvertTo_Vector3(a0obj,&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipRegularData3D(* a0,* a1, a2, a3, a4);
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
		const Vector3 * a0;
		PyObject *a0obj;
		const Vector3 * a1;
		PyObject *a1obj;
		float a2;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-IIf",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj,&a2))
		{
			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipRegularData3D(* a0,* a1, a2);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_RegularData3D);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_RegularData3D,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (RegularData3D *)sipNew;
			else
				delete (sipRegularData3D *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipRegularData3D *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_RegularData3D[] = {
	{sipName_BALL_clear, sipDo_RegularData3D_clear, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_RegularData3D_set, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_RegularData3D_isValid, METH_VARARGS, NULL},
	{sipName_BALL_getMaxX, sipDo_RegularData3D_getMaxX, METH_VARARGS, NULL},
	{sipName_BALL_getMaxY, sipDo_RegularData3D_getMaxY, METH_VARARGS, NULL},
	{sipName_BALL_getMaxZ, sipDo_RegularData3D_getMaxZ, METH_VARARGS, NULL},
	{sipName_BALL_getMinX, sipDo_RegularData3D_getMinX, METH_VARARGS, NULL},
	{sipName_BALL_getMinY, sipDo_RegularData3D_getMinY, METH_VARARGS, NULL},
	{sipName_BALL_getMinZ, sipDo_RegularData3D_getMinZ, METH_VARARGS, NULL},
	{sipName_BALL_getMaxXIndex, sipDo_RegularData3D_getMaxXIndex, METH_VARARGS, NULL},
	{sipName_BALL_getMaxYIndex, sipDo_RegularData3D_getMaxYIndex, METH_VARARGS, NULL},
	{sipName_BALL_getMaxZIndex, sipDo_RegularData3D_getMaxZIndex, METH_VARARGS, NULL},
	{sipName_BALL_getSize, sipDo_RegularData3D_getSize, METH_VARARGS, NULL},
	{sipName_BALL_getXSpacing, sipDo_RegularData3D_getXSpacing, METH_VARARGS, NULL},
	{sipName_BALL_getYSpacing, sipDo_RegularData3D_getYSpacing, METH_VARARGS, NULL},
	{sipName_BALL_getZSpacing, sipDo_RegularData3D_getZSpacing, METH_VARARGS, NULL},
	{sipName_BALL_getIndex, sipDo_RegularData3D_getIndex, METH_VARARGS, NULL},
	{sipName_BALL_getGridCoordinates, sipDo_RegularData3D_getGridCoordinates, METH_VARARGS, NULL},
	{sipName_BALL_getBoxIndices, sipDo_RegularData3D_getBoxIndices, METH_VARARGS, NULL},
	{sipName_BALL_getOrigin, sipDo_RegularData3D_getOrigin, METH_VARARGS, NULL},
	{sipName_BALL_setOrigin, sipDo_RegularData3D_setOrigin, METH_VARARGS, NULL},
	{sipName_BALL_getDimension, sipDo_RegularData3D_getDimension, METH_VARARGS, NULL},
	{sipName_BALL_has, sipDo_RegularData3D_has, METH_VARARGS, NULL},
	{sipName_BALL_getInterpolatedValue, sipDo_RegularData3D_getInterpolatedValue, METH_VARARGS, NULL},
	{sipName_BALL_CmpOp, sipDo_RegularData3D_CmpOp, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_RegularData3D(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_RegularData3D);
}

void sipConvertTo_RegularData3D(PyObject *sipPy,RegularData3D **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_RegularData3D);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (RegularData3D *)sipConvertToCpp(sipPy,sipClass_RegularData3D,sipIsErr);
}

RegularData3D *sipForceConvertTo_RegularData3D(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_RegularData3D(valobj))
	{
		RegularData3D *val;

		sipConvertTo_RegularData3D(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_RegularData3D);

	*iserrp = 1;

	return NULL;
}
