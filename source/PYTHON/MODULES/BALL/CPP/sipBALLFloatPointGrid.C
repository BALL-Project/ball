#include "sipBALLDeclBALL.h"
#include "sipBALLFloatPointGrid.h"



PyObject *sipClass_FloatPointGrid;

static void sipDealloc_FloatPointGrid(sipThisType *);
static PyObject *sipPyInternalRepr_FloatPointGrid(sipThisType *);

static PyTypeObject sipType_FloatPointGrid = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_FloatPointGrid,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_FloatPointGrid,
	0,
	0,
	0,
	0,
	(reprfunc)sipPyInternalRepr_FloatPointGrid,
};

static PyObject *sipDo_FloatPointGrid_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		const FloatPointGrid *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_FloatPointGrid,&a0obj))
		{
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_FloatPointGrid(a0obj,(FloatPointGrid **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> FloatPointGrid::set(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			res = ptr -> FloatPointGrid::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_isValid);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_getMaxX(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			res = ptr -> FloatPointGrid::getMaxX();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_getMaxX);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_getMaxY(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			res = ptr -> FloatPointGrid::getMaxY();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_getMaxY);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_getMaxZ(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			res = ptr -> FloatPointGrid::getMaxZ();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_getMaxZ);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_getMinX(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			res = ptr -> FloatPointGrid::getMinX();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_getMinX);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_getMinY(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			res = ptr -> FloatPointGrid::getMinY();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_getMinY);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_getMinZ(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			res = ptr -> FloatPointGrid::getMinZ();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_getMinZ);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_getMaxXIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Size *res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			res = new Size(ptr -> FloatPointGrid::getMaxXIndex());

			return sipNewCppToSelf(res,sipClass_Size,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_getMaxXIndex);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_getMaxYIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Size *res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			res = new Size(ptr -> FloatPointGrid::getMaxYIndex());

			return sipNewCppToSelf(res,sipClass_Size,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_getMaxYIndex);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_getMaxZIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Size *res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			res = new Size(ptr -> FloatPointGrid::getMaxZIndex());

			return sipNewCppToSelf(res,sipClass_Size,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_getMaxZIndex);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_getSize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Size *res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			res = new Size(ptr -> FloatPointGrid::getSize());

			return sipNewCppToSelf(res,sipClass_Size,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_getSize);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_getXSpacing(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			res = ptr -> FloatPointGrid::getXSpacing();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_getXSpacing);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_getYSpacing(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			res = ptr -> FloatPointGrid::getYSpacing();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_getYSpacing);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_getZSpacing(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			res = ptr -> FloatPointGrid::getZSpacing();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_getZSpacing);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_getIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			GridIndex *res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = new GridIndex(ptr -> FloatPointGrid::getIndex(* a0));

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
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			res = new GridIndex(ptr -> FloatPointGrid::getIndex( a0, a1, a2));

			return sipNewCppToSelf(res,sipClass_GridIndex,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_getIndex);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_Operator__getitem__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		const Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			float res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,(Position **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> FloatPointGrid::operator[](* a0);

			if (istemp0)
				delete a0;

			return PyFloat_FromDouble((double)res);
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			float res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> FloatPointGrid::operator[](* a0);

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_Operator__getitem__);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_getGridCoordinates(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
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
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,(Position **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Position(a1obj,(Position **)&a1,1,&iserr);
			int istemp2 = sipConvertTo_Position(a2obj,(Position **)&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = new Vector3(ptr -> FloatPointGrid::getGridCoordinates(* a0,* a1,* a2));

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
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = new Vector3(ptr -> FloatPointGrid::getGridCoordinates(* a0));

			return sipNewCppToSelf(res,sipClass_Vector3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	{
		const Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			Vector3 *res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,(Position **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = new Vector3(ptr -> FloatPointGrid::getGridCoordinates(* a0));

			if (istemp0)
				delete a0;

			return sipNewCppToSelf(res,sipClass_Vector3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_getGridCoordinates);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_getBoxIndices(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
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
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
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

			ptr -> FloatPointGrid::getBoxIndices(* a0,* a1,* a2,* a3,* a4,* a5,* a6,* a7,* a8);

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

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_getBoxIndices);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_getOrigin(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Vector3 *res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			res = &ptr -> FloatPointGrid::getOrigin();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const Vector3 *res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			res = &ptr -> FloatPointGrid::getOrigin();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_getOrigin);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_setOrigin(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> FloatPointGrid::setOrigin(* a0);

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
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			ptr -> FloatPointGrid::setOrigin( a0, a1, a2);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_setOrigin);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_getDimension(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Vector3 *res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			res = &ptr -> FloatPointGrid::getDimension();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const Vector3 *res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			res = &ptr -> FloatPointGrid::getDimension();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_getDimension);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_has(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			bool res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> FloatPointGrid::has(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	{
		float a0;
		float a1;
		float a2;

		if (sipParseArgs(sipArgs,"fff",&a0,&a1,&a2))
		{
			bool res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			res = ptr -> FloatPointGrid::has( a0, a1, a2);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_has);

	return NULL;
}

static PyObject *sipDo_FloatPointGrid_getInterpolatedValue(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FloatPointGrid)) == NULL)
		return NULL;

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			float res;
			FloatPointGrid *ptr;

			if ((ptr = (FloatPointGrid *)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> FloatPointGrid::getInterpolatedValue(* a0);

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_FloatPointGrid,sipName_BALL_getInterpolatedValue);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_FloatPointGrid(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_FloatPointGrid)
		return ptr;

	return NULL;
}

static void sipDealloc_FloatPointGrid(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (FloatPointGrid *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

static PyObject *sipPyInternalRepr_FloatPointGrid(sipThisType *sipThis)
{
#line 87 "pointGrid.sip"
  FloatPointGrid* ptr;
  if ((ptr = (FloatPointGrid*)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
    return NULL;

  return PyString_FromString(String(String("FloatPointGrid ") 
        + " { " + String(ptr->getMaxXIndex()) + "x"
        + String(ptr->getMaxYIndex()) + "x" + String(ptr->getMaxZIndex()) 
				+ " }").c_str());
#line 1069 "../CPP/sipBALLFloatPointGrid.cpp"
}

PyObject *sipNew_FloatPointGrid(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_FloatPointGrid
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
			sipNew = new FloatPointGrid();
		}
	}

	if (sipNew == NULL)
	{
		const FloatPointGrid *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_FloatPointGrid,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_FloatPointGrid(a0obj,(FloatPointGrid **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new FloatPointGrid(* a0, (bool)a1);
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
		Size *a6;
		PyObject *a6obj;
		Size *a7;
		PyObject *a7obj;
		Size *a8;
		PyObject *a8obj;

		if (sipParseArgs(sipArgs,"-ffffffIII",&a0,&a1,&a2,&a3,&a4,&a5,sipCanConvertTo_Size,&a6obj,sipCanConvertTo_Size,&a7obj,sipCanConvertTo_Size,&a8obj))
		{
			int iserr = 0;

			int istemp6 = sipConvertTo_Size(a6obj,&a6,1,&iserr);
			int istemp7 = sipConvertTo_Size(a7obj,&a7,1,&iserr);
			int istemp8 = sipConvertTo_Size(a8obj,&a8,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new FloatPointGrid( a0, a1, a2, a3, a4, a5,* a6,* a7,* a8);

			if (istemp6)
				delete a6;

			if (istemp7)
				delete a7;

			if (istemp8)
				delete a8;
		}
	}

	if (sipNew == NULL)
	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;
		Size *a2;
		PyObject *a2obj;
		Size *a3;
		PyObject *a3obj;
		Size *a4;
		PyObject *a4obj;

		if (sipParseArgs(sipArgs,"-IIIII",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj,sipCanConvertTo_Size,&a2obj,sipCanConvertTo_Size,&a3obj,sipCanConvertTo_Size,&a4obj))
		{
			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);
			int istemp2 = sipConvertTo_Size(a2obj,&a2,1,&iserr);
			int istemp3 = sipConvertTo_Size(a3obj,&a3,1,&iserr);
			int istemp4 = sipConvertTo_Size(a4obj,&a4,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new FloatPointGrid(* a0,* a1,* a2,* a3,* a4);

			if (istemp2)
				delete a2;

			if (istemp3)
				delete a3;

			if (istemp4)
				delete a4;
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
			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new FloatPointGrid(* a0,* a1, a2);
		}
	}

	if (sipNew == NULL)
	{
		const FloatPointGrid *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_FloatPointGrid,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_FloatPointGrid(a0obj,(FloatPointGrid **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new FloatPointGrid(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_FloatPointGrid);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_FloatPointGrid,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (FloatPointGrid *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_FloatPointGrid[] = {
	{sipName_BALL_set, sipDo_FloatPointGrid_set, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_FloatPointGrid_isValid, METH_VARARGS, NULL},
	{sipName_BALL_getMaxX, sipDo_FloatPointGrid_getMaxX, METH_VARARGS, NULL},
	{sipName_BALL_getMaxY, sipDo_FloatPointGrid_getMaxY, METH_VARARGS, NULL},
	{sipName_BALL_getMaxZ, sipDo_FloatPointGrid_getMaxZ, METH_VARARGS, NULL},
	{sipName_BALL_getMinX, sipDo_FloatPointGrid_getMinX, METH_VARARGS, NULL},
	{sipName_BALL_getMinY, sipDo_FloatPointGrid_getMinY, METH_VARARGS, NULL},
	{sipName_BALL_getMinZ, sipDo_FloatPointGrid_getMinZ, METH_VARARGS, NULL},
	{sipName_BALL_getMaxXIndex, sipDo_FloatPointGrid_getMaxXIndex, METH_VARARGS, NULL},
	{sipName_BALL_getMaxYIndex, sipDo_FloatPointGrid_getMaxYIndex, METH_VARARGS, NULL},
	{sipName_BALL_getMaxZIndex, sipDo_FloatPointGrid_getMaxZIndex, METH_VARARGS, NULL},
	{sipName_BALL_getSize, sipDo_FloatPointGrid_getSize, METH_VARARGS, NULL},
	{sipName_BALL_getXSpacing, sipDo_FloatPointGrid_getXSpacing, METH_VARARGS, NULL},
	{sipName_BALL_getYSpacing, sipDo_FloatPointGrid_getYSpacing, METH_VARARGS, NULL},
	{sipName_BALL_getZSpacing, sipDo_FloatPointGrid_getZSpacing, METH_VARARGS, NULL},
	{sipName_BALL_getIndex, sipDo_FloatPointGrid_getIndex, METH_VARARGS, NULL},
	{sipName_BALL_Operator__getitem__, sipDo_FloatPointGrid_Operator__getitem__, METH_VARARGS, NULL},
	{sipName_BALL_getGridCoordinates, sipDo_FloatPointGrid_getGridCoordinates, METH_VARARGS, NULL},
	{sipName_BALL_getBoxIndices, sipDo_FloatPointGrid_getBoxIndices, METH_VARARGS, NULL},
	{sipName_BALL_getOrigin, sipDo_FloatPointGrid_getOrigin, METH_VARARGS, NULL},
	{sipName_BALL_setOrigin, sipDo_FloatPointGrid_setOrigin, METH_VARARGS, NULL},
	{sipName_BALL_getDimension, sipDo_FloatPointGrid_getDimension, METH_VARARGS, NULL},
	{sipName_BALL_has, sipDo_FloatPointGrid_has, METH_VARARGS, NULL},
	{sipName_BALL_getInterpolatedValue, sipDo_FloatPointGrid_getInterpolatedValue, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_FloatPointGrid(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_FloatPointGrid);
}

void sipConvertTo_FloatPointGrid(PyObject *sipPy,FloatPointGrid **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_FloatPointGrid);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (FloatPointGrid *)sipConvertToCpp(sipPy,sipClass_FloatPointGrid,sipIsErr);
}

FloatPointGrid *sipForceConvertTo_FloatPointGrid(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_FloatPointGrid(valobj))
	{
		FloatPointGrid *val;

		sipConvertTo_FloatPointGrid(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_FloatPointGrid);
	*iserrp = 1;

	return NULL;
}
