#include "sipBALLDeclBALL.h"
#include "sipBALLPointGrid.h"



PyObject *sipClass_PointGrid;

static void sipDealloc_PointGrid(sipThisType *);

static PyTypeObject sipType_PointGrid = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_PointGrid,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_PointGrid,
	0,
	0,
	0,
	0,
	0,
};

static PyObject *sipDo_PointGrid_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		const PointGrid *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_PointGrid,&a0obj))
		{
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_PointGrid(a0obj,(PointGrid **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> PointGrid::set(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_PointGrid_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			res = ptr -> PointGrid::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_isValid);

	return NULL;
}

static PyObject *sipDo_PointGrid_getMaxX(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			res = ptr -> PointGrid::getMaxX();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_getMaxX);

	return NULL;
}

static PyObject *sipDo_PointGrid_getMaxY(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			res = ptr -> PointGrid::getMaxY();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_getMaxY);

	return NULL;
}

static PyObject *sipDo_PointGrid_getMaxZ(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			res = ptr -> PointGrid::getMaxZ();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_getMaxZ);

	return NULL;
}

static PyObject *sipDo_PointGrid_getMinX(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			res = ptr -> PointGrid::getMinX();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_getMinX);

	return NULL;
}

static PyObject *sipDo_PointGrid_getMinY(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			res = ptr -> PointGrid::getMinY();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_getMinY);

	return NULL;
}

static PyObject *sipDo_PointGrid_getMinZ(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			res = ptr -> PointGrid::getMinZ();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_getMinZ);

	return NULL;
}

static PyObject *sipDo_PointGrid_getMaxXIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Size *res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			res = new Size(ptr -> PointGrid::getMaxXIndex());

			return sipNewCppToSelf(res,sipClass_Size,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_getMaxXIndex);

	return NULL;
}

static PyObject *sipDo_PointGrid_getMaxYIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Size *res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			res = new Size(ptr -> PointGrid::getMaxYIndex());

			return sipNewCppToSelf(res,sipClass_Size,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_getMaxYIndex);

	return NULL;
}

static PyObject *sipDo_PointGrid_getMaxZIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Size *res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			res = new Size(ptr -> PointGrid::getMaxZIndex());

			return sipNewCppToSelf(res,sipClass_Size,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_getMaxZIndex);

	return NULL;
}

static PyObject *sipDo_PointGrid_getSize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Size *res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			res = new Size(ptr -> PointGrid::getSize());

			return sipNewCppToSelf(res,sipClass_Size,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_getSize);

	return NULL;
}

static PyObject *sipDo_PointGrid_getXSpacing(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			res = ptr -> PointGrid::getXSpacing();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_getXSpacing);

	return NULL;
}

static PyObject *sipDo_PointGrid_getYSpacing(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			res = ptr -> PointGrid::getYSpacing();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_getYSpacing);

	return NULL;
}

static PyObject *sipDo_PointGrid_getZSpacing(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			res = ptr -> PointGrid::getZSpacing();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_getZSpacing);

	return NULL;
}

static PyObject *sipDo_PointGrid_getIndex(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			GridIndex *res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = new GridIndex(ptr -> PointGrid::getIndex(* a0));

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
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			res = new GridIndex(ptr -> PointGrid::getIndex( a0, a1, a2));

			return sipNewCppToSelf(res,sipClass_GridIndex,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_getIndex);

	return NULL;
}

static PyObject *sipDo_PointGrid_getData(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
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
			float res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,(Position **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Position(a1obj,(Position **)&a1,1,&iserr);
			int istemp2 = sipConvertTo_Position(a2obj,(Position **)&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> PointGrid::getData(* a0,* a1,* a2);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			if (istemp2)
				delete a2;

			return PyFloat_FromDouble((double)res);
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			float res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> PointGrid::getData(* a0);

			return PyFloat_FromDouble((double)res);
		}
	}

	{
		const Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			float res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,(Position **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> PointGrid::getData(* a0);

			if (istemp0)
				delete a0;

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_getData);

	return NULL;
}

static PyObject *sipDo_PointGrid_Operator__getitem__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		const Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			float res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,(Position **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> PointGrid::operator[](* a0);

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
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> PointGrid::operator[](* a0);

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_Operator__getitem__);

	return NULL;
}

static PyObject *sipDo_PointGrid_getGridCoordinates(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
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
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,(Position **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_Position(a1obj,(Position **)&a1,1,&iserr);
			int istemp2 = sipConvertTo_Position(a2obj,(Position **)&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = new Vector3(ptr -> PointGrid::getGridCoordinates(* a0,* a1,* a2));

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
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = new Vector3(ptr -> PointGrid::getGridCoordinates(* a0));

			return sipNewCppToSelf(res,sipClass_Vector3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	{
		const Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			Vector3 *res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,(Position **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = new Vector3(ptr -> PointGrid::getGridCoordinates(* a0));

			if (istemp0)
				delete a0;

			return sipNewCppToSelf(res,sipClass_Vector3,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_getGridCoordinates);

	return NULL;
}

static PyObject *sipDo_PointGrid_getBoxIndices(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
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
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
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

			ptr -> PointGrid::getBoxIndices(* a0,* a1,* a2,* a3,* a4,* a5,* a6,* a7,* a8);

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

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_getBoxIndices);

	return NULL;
}

static PyObject *sipDo_PointGrid_getOrigin(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Vector3 *res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			res = &ptr -> PointGrid::getOrigin();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const Vector3 *res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			res = &ptr -> PointGrid::getOrigin();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_getOrigin);

	return NULL;
}

static PyObject *sipDo_PointGrid_setOrigin(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> PointGrid::setOrigin(* a0);

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
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			ptr -> PointGrid::setOrigin( a0, a1, a2);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_setOrigin);

	return NULL;
}

static PyObject *sipDo_PointGrid_getDimension(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Vector3 *res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			res = &ptr -> PointGrid::getDimension();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const Vector3 *res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			res = &ptr -> PointGrid::getDimension();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_getDimension);

	return NULL;
}

static PyObject *sipDo_PointGrid_has(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			bool res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> PointGrid::has(* a0);

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
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			res = ptr -> PointGrid::has( a0, a1, a2);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_has);

	return NULL;
}

static PyObject *sipDo_PointGrid_getInterpolatedValue(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PointGrid)) == NULL)
		return NULL;

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			float res;
			PointGrid *ptr;

			if ((ptr = (PointGrid *)sipGetCppPtr(sipThis,sipClass_PointGrid)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> PointGrid::getInterpolatedValue(* a0);

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_PointGrid,sipName_BALL_getInterpolatedValue);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_PointGrid(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_PointGrid)
		return ptr;

	return NULL;
}

static void sipDealloc_PointGrid(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (PointGrid *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_PointGrid(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_PointGrid
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
			sipNew = new PointGrid();
		}
	}

	if (sipNew == NULL)
	{
		const PointGrid *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_PointGrid,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_PointGrid(a0obj,(PointGrid **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new PointGrid(* a0, (bool)a1);
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

			sipNew = new PointGrid( a0, a1, a2, a3, a4, a5,* a6,* a7,* a8);

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

			sipNew = new PointGrid(* a0,* a1,* a2,* a3,* a4);

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

			sipNew = new PointGrid(* a0,* a1, a2);
		}
	}

	if (sipNew == NULL)
	{
		const PointGrid *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_PointGrid,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_PointGrid(a0obj,(PointGrid **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new PointGrid(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_PointGrid);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_PointGrid,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (PointGrid *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_PointGrid[] = {
	{sipName_BALL_set, sipDo_PointGrid_set, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_PointGrid_isValid, METH_VARARGS, NULL},
	{sipName_BALL_getMaxX, sipDo_PointGrid_getMaxX, METH_VARARGS, NULL},
	{sipName_BALL_getMaxY, sipDo_PointGrid_getMaxY, METH_VARARGS, NULL},
	{sipName_BALL_getMaxZ, sipDo_PointGrid_getMaxZ, METH_VARARGS, NULL},
	{sipName_BALL_getMinX, sipDo_PointGrid_getMinX, METH_VARARGS, NULL},
	{sipName_BALL_getMinY, sipDo_PointGrid_getMinY, METH_VARARGS, NULL},
	{sipName_BALL_getMinZ, sipDo_PointGrid_getMinZ, METH_VARARGS, NULL},
	{sipName_BALL_getMaxXIndex, sipDo_PointGrid_getMaxXIndex, METH_VARARGS, NULL},
	{sipName_BALL_getMaxYIndex, sipDo_PointGrid_getMaxYIndex, METH_VARARGS, NULL},
	{sipName_BALL_getMaxZIndex, sipDo_PointGrid_getMaxZIndex, METH_VARARGS, NULL},
	{sipName_BALL_getSize, sipDo_PointGrid_getSize, METH_VARARGS, NULL},
	{sipName_BALL_getXSpacing, sipDo_PointGrid_getXSpacing, METH_VARARGS, NULL},
	{sipName_BALL_getYSpacing, sipDo_PointGrid_getYSpacing, METH_VARARGS, NULL},
	{sipName_BALL_getZSpacing, sipDo_PointGrid_getZSpacing, METH_VARARGS, NULL},
	{sipName_BALL_getIndex, sipDo_PointGrid_getIndex, METH_VARARGS, NULL},
	{sipName_BALL_getData, sipDo_PointGrid_getData, METH_VARARGS, NULL},
	{sipName_BALL_Operator__getitem__, sipDo_PointGrid_Operator__getitem__, METH_VARARGS, NULL},
	{sipName_BALL_getGridCoordinates, sipDo_PointGrid_getGridCoordinates, METH_VARARGS, NULL},
	{sipName_BALL_getBoxIndices, sipDo_PointGrid_getBoxIndices, METH_VARARGS, NULL},
	{sipName_BALL_getOrigin, sipDo_PointGrid_getOrigin, METH_VARARGS, NULL},
	{sipName_BALL_setOrigin, sipDo_PointGrid_setOrigin, METH_VARARGS, NULL},
	{sipName_BALL_getDimension, sipDo_PointGrid_getDimension, METH_VARARGS, NULL},
	{sipName_BALL_has, sipDo_PointGrid_has, METH_VARARGS, NULL},
	{sipName_BALL_getInterpolatedValue, sipDo_PointGrid_getInterpolatedValue, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_PointGrid(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_PointGrid);
}

void sipConvertTo_PointGrid(PyObject *sipPy,PointGrid **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_PointGrid);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (PointGrid *)sipConvertToCpp(sipPy,sipClass_PointGrid,sipIsErr);
}

PointGrid *sipForceConvertTo_PointGrid(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_PointGrid(valobj))
	{
		PointGrid *val;

		sipConvertTo_PointGrid(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_PointGrid);
	*iserrp = 1;

	return NULL;
}
