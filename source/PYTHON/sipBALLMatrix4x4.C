#include "sipBALLDeclBALL.h"
#include "sipBALLMatrix4x4.h"



PyObject *sipClass_Matrix4x4;

static void sipDealloc_Matrix4x4(sipThisType *);

static PyTypeObject sipType_Matrix4x4 = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Matrix4x4,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Matrix4x4,
	0,
	0,
	0,
	0,
	0,
};

static PyObject *sipDo_Matrix4x4_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		const Matrix4x4 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Matrix4x4,&a0obj))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Matrix4x4(a0obj,(Matrix4x4 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::set(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const Vector4 *a0;
		PyObject *a0obj;
		const Vector4 *a1;
		PyObject *a1obj;
		const Vector4 *a2;
		PyObject *a2obj;
		const Vector4 *a3;
		PyObject *a3obj;

		if (sipParseArgs(sipArgs,"IIII",sipCanConvertTo_Vector4,&a0obj,sipCanConvertTo_Vector4,&a1obj,sipCanConvertTo_Vector4,&a2obj,sipCanConvertTo_Vector4,&a3obj))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector4(a0obj,(Vector4 **)&a0,1,&iserr);
			sipConvertTo_Vector4(a1obj,(Vector4 **)&a1,1,&iserr);
			sipConvertTo_Vector4(a2obj,(Vector4 **)&a2,1,&iserr);
			sipConvertTo_Vector4(a3obj,(Vector4 **)&a3,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::set(* a0,* a1,* a2,* a3);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		float a0;
		float a1;
		float a2;
		float a3;
		float a4;
		float a5;
		float a6;
		float a7;
		float a8;
		float a9;
		float a10;
		float a11;
		float a12;
		float a13;
		float a14;
		float a15;

		if (sipParseArgs(sipArgs,"ffffffffffffffff",&a0,&a1,&a2,&a3,&a4,&a5,&a6,&a7,&a8,&a9,&a10,&a11,&a12,&a13,&a14,&a15))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			ptr -> Matrix4x4::set( a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		float a0 = 1;

		if (sipParseArgs(sipArgs,"|f",&a0))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			ptr -> Matrix4x4::set( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		Matrix4x4 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Matrix4x4,&a0obj))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Matrix4x4(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::get(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		Vector4 *a0;
		PyObject *a0obj;
		Vector4 *a1;
		PyObject *a1obj;
		Vector4 *a2;
		PyObject *a2obj;
		Vector4 *a3;
		PyObject *a3obj;

		if (sipParseArgs(sipArgs,"IIII",sipCanConvertTo_Vector4,&a0obj,sipCanConvertTo_Vector4,&a1obj,sipCanConvertTo_Vector4,&a2obj,sipCanConvertTo_Vector4,&a3obj))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector4(a0obj,&a0,1,&iserr);
			sipConvertTo_Vector4(a1obj,&a1,1,&iserr);
			sipConvertTo_Vector4(a2obj,&a2,1,&iserr);
			sipConvertTo_Vector4(a3obj,&a3,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::get(* a0,* a1,* a2,* a3);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		float a0;
		float a1;
		float a2;
		float a3;
		float a4;
		float a5;
		float a6;
		float a7;
		float a8;
		float a9;
		float a10;
		float a11;
		float a12;
		float a13;
		float a14;
		float a15;

		if (sipParseArgs(sipArgs,"ffffffffffffffff",&a0,&a1,&a2,&a3,&a4,&a5,&a6,&a7,&a8,&a9,&a10,&a11,&a12,&a13,&a14,&a15))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			ptr -> Matrix4x4::get( a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_get);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		Matrix4x4 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Matrix4x4,&a0obj))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Matrix4x4(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::swap(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_swap);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_getTrace(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			res = ptr -> Matrix4x4::getTrace();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_getTrace);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_getZero(PyObject *,PyObject *sipArgs)
{

	{
		if (sipParseArgs(sipArgs,"-"))
		{
			const Matrix4x4 *res;

			res = &Matrix4x4::getZero();

			return sipMapCppToSelf(res,sipClass_Matrix4x4);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_getZero);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_getIdentity(PyObject *,PyObject *sipArgs)
{

	{
		if (sipParseArgs(sipArgs,"-"))
		{
			const Matrix4x4 *res;

			res = &Matrix4x4::getIdentity();

			return sipMapCppToSelf(res,sipClass_Matrix4x4);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_getIdentity);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_setIdentity(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			ptr -> Matrix4x4::setIdentity();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_setIdentity);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_transpose(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			ptr -> Matrix4x4::transpose();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_transpose);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_getRow(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			Vector4 *res;
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = new Vector4(ptr -> Matrix4x4::getRow(* a0));

			if (istemp0)
				delete a0;

			return sipNewCppToSelf(res,sipClass_Vector4,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_getRow);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_getColumn(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			Vector4 *res;
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = new Vector4(ptr -> Matrix4x4::getColumn(* a0));

			if (istemp0)
				delete a0;

			return sipNewCppToSelf(res,sipClass_Vector4,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_getColumn);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_setRow(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		Position *a0;
		PyObject *a0obj;
		const Vector4 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_Position,&a0obj,sipCanConvertTo_Vector4,&a1obj))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);
			sipConvertTo_Vector4(a1obj,(Vector4 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::setRow(* a0,* a1);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_setRow);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_setColumn(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		Position *a0;
		PyObject *a0obj;
		const Vector4 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_Position,&a0obj,sipCanConvertTo_Vector4,&a1obj))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);
			sipConvertTo_Vector4(a1obj,(Vector4 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::setColumn(* a0,* a1);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_setColumn);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_isEqual(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		const Matrix4x4 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Matrix4x4,&a0obj))
		{
			bool res;
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Matrix4x4(a0obj,(Matrix4x4 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Matrix4x4::isEqual(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_isEqual);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_getDiagonal(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Vector4 *res;
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			res = &ptr -> Matrix4x4::getDiagonal();

			return sipMapCppToSelf(res,sipClass_Vector4);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_getDiagonal);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_invert(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		Matrix4x4 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Matrix4x4,&a0obj))
		{
			bool res;
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Matrix4x4(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Matrix4x4::invert(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			res = ptr -> Matrix4x4::invert();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_invert);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_getDeterminant(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			res = ptr -> Matrix4x4::getDeterminant();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_getDeterminant);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_translate(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		float a0;
		float a1;
		float a2;

		if (sipParseArgs(sipArgs,"fff",&a0,&a1,&a2))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			ptr -> Matrix4x4::translate( a0, a1, a2);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::translate(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_translate);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_setTranslation(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		float a0;
		float a1;
		float a2;

		if (sipParseArgs(sipArgs,"fff",&a0,&a1,&a2))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			ptr -> Matrix4x4::setTranslation( a0, a1, a2);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::setTranslation(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_setTranslation);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_scale(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		float a0;
		float a1;
		float a2;

		if (sipParseArgs(sipArgs,"fff",&a0,&a1,&a2))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			ptr -> Matrix4x4::scale( a0, a1, a2);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		float a0;

		if (sipParseArgs(sipArgs,"f",&a0))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			ptr -> Matrix4x4::scale( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::scale(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_scale);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_setScale(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		float a0;
		float a1;
		float a2;

		if (sipParseArgs(sipArgs,"fff",&a0,&a1,&a2))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			ptr -> Matrix4x4::setScale( a0, a1, a2);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		float a0;

		if (sipParseArgs(sipArgs,"f",&a0))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			ptr -> Matrix4x4::setScale( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::setScale(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_setScale);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_rotateX(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		const Angle *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Angle,&a0obj))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Angle(a0obj,(Angle **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::rotateX(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_rotateX);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_setRotationX(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		const Angle *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Angle,&a0obj))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Angle(a0obj,(Angle **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::setRotationX(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_setRotationX);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_rotateY(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		const Angle *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Angle,&a0obj))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Angle(a0obj,(Angle **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::rotateY(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_rotateY);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_setRotationY(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		const Angle *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Angle,&a0obj))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Angle(a0obj,(Angle **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::setRotationY(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_setRotationY);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_rotateZ(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		const Angle *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Angle,&a0obj))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Angle(a0obj,(Angle **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::rotateZ(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_rotateZ);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_setRotationZ(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		const Angle *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Angle,&a0obj))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Angle(a0obj,(Angle **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::setRotationZ(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_setRotationZ);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_rotate(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		const Angle *a0;
		PyObject *a0obj;
		float a1;
		float a2;
		float a3;

		if (sipParseArgs(sipArgs,"Ifff",sipCanConvertTo_Angle,&a0obj,&a1,&a2,&a3))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Angle(a0obj,(Angle **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::rotate(* a0, a1, a2, a3);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const Angle *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_Angle,&a0obj,sipCanConvertTo_Vector3,&a1obj))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Angle(a0obj,(Angle **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::rotate(* a0,* a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const Angle *a0;
		PyObject *a0obj;
		const Vector4 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_Angle,&a0obj,sipCanConvertTo_Vector4,&a1obj))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Angle(a0obj,(Angle **)&a0,1,&iserr);
			sipConvertTo_Vector4(a1obj,(Vector4 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::rotate(* a0,* a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_rotate);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_setRotation(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		const Angle *a0;
		PyObject *a0obj;
		float a1;
		float a2;
		float a3;

		if (sipParseArgs(sipArgs,"Ifff",sipCanConvertTo_Angle,&a0obj,&a1,&a2,&a3))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Angle(a0obj,(Angle **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::setRotation(* a0, a1, a2, a3);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const Angle *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_Angle,&a0obj,sipCanConvertTo_Vector3,&a1obj))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Angle(a0obj,(Angle **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::setRotation(* a0,* a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const Angle *a0;
		PyObject *a0obj;
		const Vector4 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_Angle,&a0obj,sipCanConvertTo_Vector4,&a1obj))
		{
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Angle(a0obj,(Angle **)&a0,1,&iserr);
			sipConvertTo_Vector4(a1obj,(Vector4 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Matrix4x4::setRotation(* a0,* a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_setRotation);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_isIdentity(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			res = ptr -> Matrix4x4::isIdentity();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_isIdentity);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_isRegular(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			res = ptr -> Matrix4x4::isRegular();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_isRegular);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_isSingular(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			res = ptr -> Matrix4x4::isSingular();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_isSingular);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_isSymmetric(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			res = ptr -> Matrix4x4::isSymmetric();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_isSymmetric);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_isLowerTriangular(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			res = ptr -> Matrix4x4::isLowerTriangular();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_isLowerTriangular);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_isUpperTriangular(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			res = ptr -> Matrix4x4::isUpperTriangular();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_isUpperTriangular);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_isDiagonal(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			res = ptr -> Matrix4x4::isDiagonal();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_isDiagonal);

	return NULL;
}

static PyObject *sipDo_Matrix4x4_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Matrix4x4)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Matrix4x4 *ptr;

			if ((ptr = (Matrix4x4 *)sipGetCppPtr(sipThis,sipClass_Matrix4x4)) == NULL)
				return NULL;

			res = ptr -> Matrix4x4::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Matrix4x4,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Matrix4x4(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Matrix4x4)
		return ptr;

	return NULL;
}

static void sipDealloc_Matrix4x4(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (Matrix4x4 *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_Matrix4x4(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Matrix4x4
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
			sipNew = new Matrix4x4();
		}
	}

	if (sipNew == NULL)
	{
		const Matrix4x4 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_Matrix4x4,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Matrix4x4(a0obj,(Matrix4x4 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Matrix4x4(* a0);
		}
	}

	if (sipNew == NULL)
	{
		const Vector4 *a0;
		PyObject *a0obj;
		const Vector4 *a1;
		PyObject *a1obj;
		const Vector4 *a2;
		PyObject *a2obj;
		const Vector4 *a3;
		PyObject *a3obj;

		if (sipParseArgs(sipArgs,"-IIII",sipCanConvertTo_Vector4,&a0obj,sipCanConvertTo_Vector4,&a1obj,sipCanConvertTo_Vector4,&a2obj,sipCanConvertTo_Vector4,&a3obj))
		{
			int iserr = 0;

			sipConvertTo_Vector4(a0obj,(Vector4 **)&a0,1,&iserr);
			sipConvertTo_Vector4(a1obj,(Vector4 **)&a1,1,&iserr);
			sipConvertTo_Vector4(a2obj,(Vector4 **)&a2,1,&iserr);
			sipConvertTo_Vector4(a3obj,(Vector4 **)&a3,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Matrix4x4(* a0,* a1,* a2,* a3);
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
		float a6;
		float a7;
		float a8;
		float a9;
		float a10;
		float a11;
		float a12;
		float a13;
		float a14;
		float a15;

		if (sipParseArgs(sipArgs,"-ffffffffffffffff",&a0,&a1,&a2,&a3,&a4,&a5,&a6,&a7,&a8,&a9,&a10,&a11,&a12,&a13,&a14,&a15))
		{
			sipNew = new Matrix4x4( a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_Matrix4x4);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Matrix4x4,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (Matrix4x4 *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Matrix4x4_m11(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Matrix4x4 *ptr;

	if ((ptr = (Matrix4x4 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Matrix4x4)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> m11;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Matrix4x4,sipName_BALL_m11);
		return NULL;
	}

	ptr -> m11 = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Matrix4x4_m12(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Matrix4x4 *ptr;

	if ((ptr = (Matrix4x4 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Matrix4x4)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> m12;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Matrix4x4,sipName_BALL_m12);
		return NULL;
	}

	ptr -> m12 = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Matrix4x4_m13(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Matrix4x4 *ptr;

	if ((ptr = (Matrix4x4 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Matrix4x4)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> m13;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Matrix4x4,sipName_BALL_m13);
		return NULL;
	}

	ptr -> m13 = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Matrix4x4_m14(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Matrix4x4 *ptr;

	if ((ptr = (Matrix4x4 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Matrix4x4)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> m14;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Matrix4x4,sipName_BALL_m14);
		return NULL;
	}

	ptr -> m14 = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Matrix4x4_m21(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Matrix4x4 *ptr;

	if ((ptr = (Matrix4x4 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Matrix4x4)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> m21;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Matrix4x4,sipName_BALL_m21);
		return NULL;
	}

	ptr -> m21 = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Matrix4x4_m22(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Matrix4x4 *ptr;

	if ((ptr = (Matrix4x4 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Matrix4x4)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> m22;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Matrix4x4,sipName_BALL_m22);
		return NULL;
	}

	ptr -> m22 = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Matrix4x4_m23(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Matrix4x4 *ptr;

	if ((ptr = (Matrix4x4 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Matrix4x4)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> m23;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Matrix4x4,sipName_BALL_m23);
		return NULL;
	}

	ptr -> m23 = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Matrix4x4_m24(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Matrix4x4 *ptr;

	if ((ptr = (Matrix4x4 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Matrix4x4)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> m24;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Matrix4x4,sipName_BALL_m24);
		return NULL;
	}

	ptr -> m24 = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Matrix4x4_m31(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Matrix4x4 *ptr;

	if ((ptr = (Matrix4x4 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Matrix4x4)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> m31;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Matrix4x4,sipName_BALL_m31);
		return NULL;
	}

	ptr -> m31 = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Matrix4x4_m32(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Matrix4x4 *ptr;

	if ((ptr = (Matrix4x4 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Matrix4x4)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> m32;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Matrix4x4,sipName_BALL_m32);
		return NULL;
	}

	ptr -> m32 = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Matrix4x4_m33(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Matrix4x4 *ptr;

	if ((ptr = (Matrix4x4 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Matrix4x4)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> m33;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Matrix4x4,sipName_BALL_m33);
		return NULL;
	}

	ptr -> m33 = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Matrix4x4_m34(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Matrix4x4 *ptr;

	if ((ptr = (Matrix4x4 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Matrix4x4)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> m34;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Matrix4x4,sipName_BALL_m34);
		return NULL;
	}

	ptr -> m34 = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Matrix4x4_m41(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Matrix4x4 *ptr;

	if ((ptr = (Matrix4x4 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Matrix4x4)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> m41;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Matrix4x4,sipName_BALL_m41);
		return NULL;
	}

	ptr -> m41 = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Matrix4x4_m42(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Matrix4x4 *ptr;

	if ((ptr = (Matrix4x4 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Matrix4x4)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> m42;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Matrix4x4,sipName_BALL_m42);
		return NULL;
	}

	ptr -> m42 = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Matrix4x4_m43(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Matrix4x4 *ptr;

	if ((ptr = (Matrix4x4 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Matrix4x4)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> m43;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Matrix4x4,sipName_BALL_m43);
		return NULL;
	}

	ptr -> m43 = val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_Matrix4x4_m44(PyObject *sipThisObj,PyObject *valobj)
{
	float val;
	Matrix4x4 *ptr;

	if ((ptr = (Matrix4x4 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Matrix4x4)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> m44;

		valobj = PyFloat_FromDouble((double)val);

		return valobj;
	}

	val = (float)PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Matrix4x4,sipName_BALL_m44);
		return NULL;
	}

	ptr -> m44 = val;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Matrix4x4[] = {
	{sipName_BALL_set, sipDo_Matrix4x4_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_Matrix4x4_get, METH_VARARGS, NULL},
	{sipName_BALL_swap, sipDo_Matrix4x4_swap, METH_VARARGS, NULL},
	{sipName_BALL_getTrace, sipDo_Matrix4x4_getTrace, METH_VARARGS, NULL},
	{sipName_BALL_getZero, sipDo_Matrix4x4_getZero, METH_VARARGS, NULL},
	{sipName_BALL_getIdentity, sipDo_Matrix4x4_getIdentity, METH_VARARGS, NULL},
	{sipName_BALL_setIdentity, sipDo_Matrix4x4_setIdentity, METH_VARARGS, NULL},
	{sipName_BALL_transpose, sipDo_Matrix4x4_transpose, METH_VARARGS, NULL},
	{sipName_BALL_getRow, sipDo_Matrix4x4_getRow, METH_VARARGS, NULL},
	{sipName_BALL_getColumn, sipDo_Matrix4x4_getColumn, METH_VARARGS, NULL},
	{sipName_BALL_setRow, sipDo_Matrix4x4_setRow, METH_VARARGS, NULL},
	{sipName_BALL_setColumn, sipDo_Matrix4x4_setColumn, METH_VARARGS, NULL},
	{sipName_BALL_isEqual, sipDo_Matrix4x4_isEqual, METH_VARARGS, NULL},
	{sipName_BALL_getDiagonal, sipDo_Matrix4x4_getDiagonal, METH_VARARGS, NULL},
	{sipName_BALL_invert, sipDo_Matrix4x4_invert, METH_VARARGS, NULL},
	{sipName_BALL_getDeterminant, sipDo_Matrix4x4_getDeterminant, METH_VARARGS, NULL},
	{sipName_BALL_translate, sipDo_Matrix4x4_translate, METH_VARARGS, NULL},
	{sipName_BALL_setTranslation, sipDo_Matrix4x4_setTranslation, METH_VARARGS, NULL},
	{sipName_BALL_scale, sipDo_Matrix4x4_scale, METH_VARARGS, NULL},
	{sipName_BALL_setScale, sipDo_Matrix4x4_setScale, METH_VARARGS, NULL},
	{sipName_BALL_rotateX, sipDo_Matrix4x4_rotateX, METH_VARARGS, NULL},
	{sipName_BALL_setRotationX, sipDo_Matrix4x4_setRotationX, METH_VARARGS, NULL},
	{sipName_BALL_rotateY, sipDo_Matrix4x4_rotateY, METH_VARARGS, NULL},
	{sipName_BALL_setRotationY, sipDo_Matrix4x4_setRotationY, METH_VARARGS, NULL},
	{sipName_BALL_rotateZ, sipDo_Matrix4x4_rotateZ, METH_VARARGS, NULL},
	{sipName_BALL_setRotationZ, sipDo_Matrix4x4_setRotationZ, METH_VARARGS, NULL},
	{sipName_BALL_rotate, sipDo_Matrix4x4_rotate, METH_VARARGS, NULL},
	{sipName_BALL_setRotation, sipDo_Matrix4x4_setRotation, METH_VARARGS, NULL},
	{sipName_BALL_isIdentity, sipDo_Matrix4x4_isIdentity, METH_VARARGS, NULL},
	{sipName_BALL_isRegular, sipDo_Matrix4x4_isRegular, METH_VARARGS, NULL},
	{sipName_BALL_isSingular, sipDo_Matrix4x4_isSingular, METH_VARARGS, NULL},
	{sipName_BALL_isSymmetric, sipDo_Matrix4x4_isSymmetric, METH_VARARGS, NULL},
	{sipName_BALL_isLowerTriangular, sipDo_Matrix4x4_isLowerTriangular, METH_VARARGS, NULL},
	{sipName_BALL_isUpperTriangular, sipDo_Matrix4x4_isUpperTriangular, METH_VARARGS, NULL},
	{sipName_BALL_isDiagonal, sipDo_Matrix4x4_isDiagonal, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_Matrix4x4_isValid, METH_VARARGS, NULL},
	{NULL}
};

PyMethodDef sipClassVarTab_Matrix4x4[] = {
	{sipName_BALL_m11, sipGetSetVar_Matrix4x4_m11, 0, NULL},
	{sipName_BALL_m12, sipGetSetVar_Matrix4x4_m12, 0, NULL},
	{sipName_BALL_m13, sipGetSetVar_Matrix4x4_m13, 0, NULL},
	{sipName_BALL_m14, sipGetSetVar_Matrix4x4_m14, 0, NULL},
	{sipName_BALL_m21, sipGetSetVar_Matrix4x4_m21, 0, NULL},
	{sipName_BALL_m22, sipGetSetVar_Matrix4x4_m22, 0, NULL},
	{sipName_BALL_m23, sipGetSetVar_Matrix4x4_m23, 0, NULL},
	{sipName_BALL_m24, sipGetSetVar_Matrix4x4_m24, 0, NULL},
	{sipName_BALL_m31, sipGetSetVar_Matrix4x4_m31, 0, NULL},
	{sipName_BALL_m32, sipGetSetVar_Matrix4x4_m32, 0, NULL},
	{sipName_BALL_m33, sipGetSetVar_Matrix4x4_m33, 0, NULL},
	{sipName_BALL_m34, sipGetSetVar_Matrix4x4_m34, 0, NULL},
	{sipName_BALL_m41, sipGetSetVar_Matrix4x4_m41, 0, NULL},
	{sipName_BALL_m42, sipGetSetVar_Matrix4x4_m42, 0, NULL},
	{sipName_BALL_m43, sipGetSetVar_Matrix4x4_m43, 0, NULL},
	{sipName_BALL_m44, sipGetSetVar_Matrix4x4_m44, 0, NULL},
	{NULL}
};

PyMethodDef *sipClassVarHierTab_Matrix4x4[] = {
	sipClassVarTab_Matrix4x4,
	NULL
};

int sipCanConvertTo_Matrix4x4(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Matrix4x4);
}

void sipConvertTo_Matrix4x4(PyObject *sipPy,Matrix4x4 **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_Matrix4x4);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Matrix4x4 *)sipConvertToCpp(sipPy,sipClass_Matrix4x4,sipIsErr);
}

Matrix4x4 *sipForceConvertTo_Matrix4x4(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Matrix4x4(valobj))
	{
		Matrix4x4 *val;

		sipConvertTo_Matrix4x4(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Matrix4x4);
	*iserrp = 1;

	return NULL;
}
