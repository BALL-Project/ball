#include "sipBALLDeclBALL.h"
#include "sipBALLVector3.h"


PyObject *sipClass_Vector3;

static void sipDealloc_Vector3(sipThisType *);
extern "C" PyObject * sip__str__Vector3(PyObject *a0);

static PyTypeObject sipType_Vector3 = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Vector3,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Vector3,
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
	sip__str__Vector3,
	0,
	0,
	0,
	Py_TPFLAGS_DEFAULT,
	0,
	0,
	0,
};

extern "C" PyObject *sipDo_Vector3_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		float a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"f",&a0))
		{
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

   try
   {
			ptr -> Vector3::set( a0);
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
		float a0;
		float a1;
		float a2;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"fff",&a0,&a1,&a2))
		{
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

   try
   {
			ptr -> Vector3::set( a0, a1, a2);
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
		const Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Vector3::set(* a0);
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
		float a0;
		const Angle * a1;
		PyObject *a1obj;
		const Angle * a2;
		PyObject *a2obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"fII",&a0,sipCanConvertTo_Angle,&a1obj,sipCanConvertTo_Angle,&a2obj))
		{
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Angle(a1obj,(Angle **)&a1,1,&iserr);
			sipConvertTo_Angle(a2obj,(Angle **)&a2,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Vector3::set( a0,* a1,* a2);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_set);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Vector3::get(* a0);
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
		float a0;
		Angle * a1;
		PyObject *a1obj;
		Angle * a2;
		PyObject *a2obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"fII",&a0,sipCanConvertTo_Angle,&a1obj,sipCanConvertTo_Angle,&a2obj))
		{
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Angle(a1obj,&a1,1,&iserr);
			sipConvertTo_Angle(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Vector3::get( a0,* a1,* a2);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_get);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Vector3::swap(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_swap);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_getLength(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Vector3::getLength();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_getLength);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_getSquareLength(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Vector3::getSquareLength();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_getSquareLength);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_normalize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Vector3 *res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

   try
   {
			res = &ptr -> Vector3::normalize();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_normalize);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_negate(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Vector3 *res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

   try
   {
			res = &ptr -> Vector3::negate();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_negate);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_getZero(PyObject *,PyObject *sipArgs)
{
	int sipArgsParsed = 0;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,"-"))
		{
			const Vector3 *res;

   try
   {
			res = &Vector3::getZero();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_getZero);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_getUnit(PyObject *,PyObject *sipArgs)
{
	int sipArgsParsed = 0;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,"-"))
		{
			const Vector3 *res;

   try
   {
			res = &Vector3::getUnit();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_getUnit);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_GetItemOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		Index * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Index,&a0obj))
		{
			float res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Vector3::operator[](* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return PyFloat_FromDouble((double)res);
		}
	}

	{
		Index * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Index,&a0obj))
		{
			float res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Vector3::operator[](* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_GetItemOp);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_AddOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const Vector3 *res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

   try
   {
			res = &ptr -> Vector3::operator+();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	{
		const Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			Vector3 *res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = new Vector3(ptr -> Vector3::operator+(* a0));
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipNewCppToSelf(res,sipClass_Vector3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_AddOp);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_SubOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Vector3 *res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

   try
   {
			res = new Vector3(ptr -> Vector3::operator-());
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipNewCppToSelf(res,sipClass_Vector3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	{
		const Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			Vector3 *res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = new Vector3(ptr -> Vector3::operator-(* a0));
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipNewCppToSelf(res,sipClass_Vector3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_SubOp);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_IAddOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		const Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			Vector3 *res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = &ptr -> Vector3::operator+=(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_IAddOp);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_ISubOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		const Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			Vector3 *res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = &ptr -> Vector3::operator-=(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_ISubOp);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_IMultOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		float a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"f",&a0))
		{
			Vector3 *res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

   try
   {
			res = &ptr -> Vector3::operator*=( a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_IMultOp);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_IDivOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		float a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"f",&a0))
		{
			Vector3 *res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

   try
   {
			res = &ptr -> Vector3::operator/=( a0);
   }
   catch (DivisionByZero e)
   {
      DivisionByZero *my_exception = new DivisionByZero(e);
      PyErr_SetObject(sipClass_DivisionByZero, sipNewCppToSelf(my_exception,sipClass_DivisionByZero,SIP_SIMPLE | SIP_PY_OWNED));
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_IDivOp);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_MultOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		float a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"f",&a0))
		{
			Vector3 *res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

   try
   {
			res = new Vector3(ptr -> Vector3::operator*( a0));
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipNewCppToSelf(res,sipClass_Vector3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	{
		const Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			float res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Vector3::operator*(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_MultOp);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_DivOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		float a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"f",&a0))
		{
			Vector3 *res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

   try
   {
			res = new Vector3(ptr -> Vector3::operator/( a0));
   }
   catch (DivisionByZero e)
   {
      DivisionByZero *my_exception = new DivisionByZero(e);
      PyErr_SetObject(sipClass_DivisionByZero, sipNewCppToSelf(my_exception,sipClass_DivisionByZero,SIP_SIMPLE | SIP_PY_OWNED));
      return NULL;
		}

			return sipNewCppToSelf(res,sipClass_Vector3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_DivOp);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_ModOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		const Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			Vector3 *res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = new Vector3(ptr -> Vector3::operator%(* a0));
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipNewCppToSelf(res,sipClass_Vector3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_ModOp);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_getDistance(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		const Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			float res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Vector3::getDistance(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_getDistance);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_getSquareDistance(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		const Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			float res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Vector3::getSquareDistance(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_getSquareDistance);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_getAngle(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		const Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			Angle *res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = new Angle(ptr -> Vector3::getAngle(* a0));
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipNewCppToSelf(res,sipClass_Angle,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_getAngle);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_getOrthogonalProjection(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		const Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			Vector3 *res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = new Vector3(ptr -> Vector3::getOrthogonalProjection(* a0));
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipNewCppToSelf(res,sipClass_Vector3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_getOrthogonalProjection);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_getPerpendicularNormalization(PyObject *,PyObject *sipArgs)
{
	int sipArgsParsed = 0;

	{
		const Vector3 * a0;
		PyObject *a0obj;
		const Vector3 * a1;
		PyObject *a1obj;
		const Vector3 * a2;
		PyObject *a2obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-III",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj,sipCanConvertTo_Vector3,&a2obj))
		{
			Vector3 *res;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);
			sipConvertTo_Vector3(a2obj,(Vector3 **)&a2,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = new Vector3(Vector3::getPerpendicularNormalization(* a0,* a1,* a2));
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipNewCppToSelf(res,sipClass_Vector3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_getPerpendicularNormalization);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_getTripleProduct(PyObject *,PyObject *sipArgs)
{
	int sipArgsParsed = 0;

	{
		const Vector3 * a0;
		PyObject *a0obj;
		const Vector3 * a1;
		PyObject *a1obj;
		const Vector3 * a2;
		PyObject *a2obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-III",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj,sipCanConvertTo_Vector3,&a2obj))
		{
			float res;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);
			sipConvertTo_Vector3(a2obj,(Vector3 **)&a2,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = Vector3::getTripleProduct(* a0,* a1,* a2);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_getTripleProduct);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_CmpOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		const Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			bool res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Vector3::operator==(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_CmpOp);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_isZero(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Vector3::isZero();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_isZero);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_isOrthogonalTo(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			bool res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Vector3::isOrthogonalTo(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_isOrthogonalTo);

	return NULL;
}

extern "C" PyObject *sipDo_Vector3_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Vector3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Vector3 *ptr;

			if ((ptr = (Vector3 *)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Vector3::isValid();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Vector3,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_Vector3(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_Vector3)
		return ptr;

	if ((res = sipCast_PersistentObject((PersistentObject *)(Vector3 *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_DivisionByZero((DivisionByZero *)(Vector3 *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_DivisionByZero((DivisionByZero *)(Vector3 *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_Vector3(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (Vector3 *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}
extern "C" PyObject * sip__str__Vector3(PyObject *a0)
{
#line 71 "vector3.sip"
  Vector3* ptr;
  if ((ptr = (Vector3*)sipGetCppPtr((sipThisType*)a0,sipClass_Vector3)) == NULL)
    return NULL;

  static String tmp;
  tmp = "(";
  tmp += String(ptr->x) + " ";
  tmp += String(ptr->y) + " ";
  tmp += String(ptr->z) + ")";
  return PyString_FromString(tmp.c_str());
#line 1592 "sipBALLVector3.cpp"
}

PyObject *sipNew_Vector3(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Vector3
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
			sipNew = new Vector3();
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
		float a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-f",&a0))
		{
   try
   {
			sipNew = new Vector3( a0);
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
		float a0;
		float a1;
		float a2;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-fff",&a0,&a1,&a2))
		{
   try
   {
			sipNew = new Vector3( a0, a1, a2);
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
		const Vector3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_Vector3,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new Vector3(* a0);
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
		float a0;
		const Angle * a1;
		PyObject *a1obj;
		const Angle * a2;
		PyObject *a2obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-fII",&a0,sipCanConvertTo_Angle,&a1obj,sipCanConvertTo_Angle,&a2obj))
		{
			int iserr = 0;

			sipConvertTo_Angle(a1obj,(Angle **)&a1,1,&iserr);
			sipConvertTo_Angle(a2obj,(Angle **)&a2,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new Vector3( a0,* a1,* a2);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_Vector3);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Vector3,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (Vector3 *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Vector3_z(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Vector3 *ptr;

	if ((ptr = (Vector3 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Vector3)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> z;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Vector3,sipName_BALL_z);
		return NULL;
	}

	ptr -> z = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Vector3_y(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Vector3 *ptr;

	if ((ptr = (Vector3 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Vector3)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> y;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Vector3,sipName_BALL_y);
		return NULL;
	}

	ptr -> y = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Vector3_x(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Vector3 *ptr;

	if ((ptr = (Vector3 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Vector3)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> x;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Vector3,sipName_BALL_x);
		return NULL;
	}

	ptr -> x = val;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Vector3[] = {
	{sipName_BALL_set, sipDo_Vector3_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_Vector3_get, METH_VARARGS, NULL},
	{sipName_BALL_swap, sipDo_Vector3_swap, METH_VARARGS, NULL},
	{sipName_BALL_getLength, sipDo_Vector3_getLength, METH_VARARGS, NULL},
	{sipName_BALL_getSquareLength, sipDo_Vector3_getSquareLength, METH_VARARGS, NULL},
	{sipName_BALL_normalize, sipDo_Vector3_normalize, METH_VARARGS, NULL},
	{sipName_BALL_negate, sipDo_Vector3_negate, METH_VARARGS, NULL},
	{sipName_BALL_getZero, sipDo_Vector3_getZero, METH_VARARGS, NULL},
	{sipName_BALL_getUnit, sipDo_Vector3_getUnit, METH_VARARGS, NULL},
	{sipName_BALL_GetItemOp, sipDo_Vector3_GetItemOp, METH_VARARGS, NULL},
	{sipName_BALL_AddOp, sipDo_Vector3_AddOp, METH_VARARGS, NULL},
	{sipName_BALL_SubOp, sipDo_Vector3_SubOp, METH_VARARGS, NULL},
	{sipName_BALL_IAddOp, sipDo_Vector3_IAddOp, METH_VARARGS, NULL},
	{sipName_BALL_ISubOp, sipDo_Vector3_ISubOp, METH_VARARGS, NULL},
	{sipName_BALL_IMultOp, sipDo_Vector3_IMultOp, METH_VARARGS, NULL},
	{sipName_BALL_IDivOp, sipDo_Vector3_IDivOp, METH_VARARGS, NULL},
	{sipName_BALL_MultOp, sipDo_Vector3_MultOp, METH_VARARGS, NULL},
	{sipName_BALL_DivOp, sipDo_Vector3_DivOp, METH_VARARGS, NULL},
	{sipName_BALL_ModOp, sipDo_Vector3_ModOp, METH_VARARGS, NULL},
	{sipName_BALL_getDistance, sipDo_Vector3_getDistance, METH_VARARGS, NULL},
	{sipName_BALL_getSquareDistance, sipDo_Vector3_getSquareDistance, METH_VARARGS, NULL},
	{sipName_BALL_getAngle, sipDo_Vector3_getAngle, METH_VARARGS, NULL},
	{sipName_BALL_getOrthogonalProjection, sipDo_Vector3_getOrthogonalProjection, METH_VARARGS, NULL},
	{sipName_BALL_getPerpendicularNormalization, sipDo_Vector3_getPerpendicularNormalization, METH_VARARGS, NULL},
	{sipName_BALL_getTripleProduct, sipDo_Vector3_getTripleProduct, METH_VARARGS, NULL},
	{sipName_BALL_CmpOp, sipDo_Vector3_CmpOp, METH_VARARGS, NULL},
	{sipName_BALL_isZero, sipDo_Vector3_isZero, METH_VARARGS, NULL},
	{sipName_BALL_isOrthogonalTo, sipDo_Vector3_isOrthogonalTo, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_Vector3_isValid, METH_VARARGS, NULL},
	{NULL}
};

PyMethodDef sipClassVarTab_Vector3[] = {
	{sipName_BALL_z, sipGetSetVar_Vector3_z, 0, NULL},
	{sipName_BALL_y, sipGetSetVar_Vector3_y, 0, NULL},
	{sipName_BALL_x, sipGetSetVar_Vector3_x, 0, NULL},
	{NULL}
};

PyMethodDef *sipClassVarHierTab_Vector3[] = {
	sipClassVarTab_Vector3,
	NULL
};

int sipCanConvertTo_Vector3(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Vector3);
}

void sipConvertTo_Vector3(PyObject *sipPy,Vector3 **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_Vector3);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Vector3 *)sipConvertToCpp(sipPy,sipClass_Vector3,sipIsErr);
}

Vector3 *sipForceConvertTo_Vector3(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Vector3(valobj))
	{
		Vector3 *val;

		sipConvertTo_Vector3(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Vector3);

	*iserrp = 1;

	return NULL;
}
