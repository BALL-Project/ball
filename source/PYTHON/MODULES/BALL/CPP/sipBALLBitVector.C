#include "sipBALLDeclBALL.h"
#include "sipBALLBitVector.h"



PyObject *sipClass_BitVector;

static void sipDealloc_BitVector(sipThisType *);

static PyTypeObject sipType_BitVector = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_BitVector,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_BitVector,
	0,
	0,
	0,
	0,
	0,
};

static PyObject *sipDo_BitVector_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BitVector)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			ptr -> BitVector::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BitVector,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_BitVector_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BitVector)) == NULL)
		return NULL;

	{
		const BitVector *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"I|l",sipCanConvertTo_BitVector,&a0obj,&a1))
		{
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_BitVector(a0obj,(BitVector **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BitVector::set(* a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const char *a0;

		if (sipParseArgs(sipArgs,"s",&a0))
		{
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			ptr -> BitVector::set( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BitVector,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_BitVector_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BitVector)) == NULL)
		return NULL;

	{
		BitVector *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"I|l",sipCanConvertTo_BitVector,&a0obj,&a1))
		{
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_BitVector(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BitVector::get(* a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BitVector,sipName_BALL_get);

	return NULL;
}

static PyObject *sipDo_BitVector_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BitVector)) == NULL)
		return NULL;

	{
		BitVector *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_BitVector,&a0obj))
		{
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_BitVector(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BitVector::swap(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BitVector,sipName_BALL_swap);

	return NULL;
}

static PyObject *sipDo_BitVector_setSize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BitVector)) == NULL)
		return NULL;

	{
		int a0;
		long a1 = true;

		if (sipParseArgs(sipArgs,"i|l",&a0,&a1))
		{
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			ptr -> BitVector::setSize( a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BitVector,sipName_BALL_setSize);

	return NULL;
}

static PyObject *sipDo_BitVector_getSize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BitVector)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			res = ptr -> BitVector::getSize();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BitVector,sipName_BALL_getSize);

	return NULL;
}

static PyObject *sipDo_BitVector_countValue(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BitVector)) == NULL)
		return NULL;

	{
		long a0;

		if (sipParseArgs(sipArgs,"l",&a0))
		{
			int res;
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			res = ptr -> BitVector::countValue( (bool)a0);

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BitVector,sipName_BALL_countValue);

	return NULL;
}

static PyObject *sipDo_BitVector_Operator__getitem__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BitVector)) == NULL)
		return NULL;

	{
		Index *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Index,&a0obj))
		{
			Bit *res;
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = new Bit(ptr -> BitVector::operator[](* a0));

			if (istemp0)
				delete a0;

			return sipNewCppToSelf(res,sipClass_Bit,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	{
		Index *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Index,&a0obj))
		{
			bool res;
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> BitVector::operator[](* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BitVector,sipName_BALL_Operator__getitem__);

	return NULL;
}

static PyObject *sipDo_BitVector_setBit(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BitVector)) == NULL)
		return NULL;

	{
		Index *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"I|l",sipCanConvertTo_Index,&a0obj,&a1))
		{
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BitVector::setBit(* a0, (bool)a1);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BitVector,sipName_BALL_setBit);

	return NULL;
}

static PyObject *sipDo_BitVector_getBit(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BitVector)) == NULL)
		return NULL;

	{
		Index *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Index,&a0obj))
		{
			bool res;
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> BitVector::getBit(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	{
		Index *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Index,&a0obj))
		{
			bool res;
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> BitVector::getBit(* a0);

			if (istemp0)
				delete a0;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BitVector,sipName_BALL_getBit);

	return NULL;
}

static PyObject *sipDo_BitVector_toggleBit(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BitVector)) == NULL)
		return NULL;

	{
		Index *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Index,&a0obj))
		{
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BitVector::toggleBit(* a0);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BitVector,sipName_BALL_toggleBit);

	return NULL;
}

static PyObject *sipDo_BitVector_fill(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BitVector)) == NULL)
		return NULL;

	{
		long a0 = true;
		Index *a1 = NULL;
		PyObject *a1obj = NULL;
		Index *a2 = NULL;
		PyObject *a2obj = NULL;

		if (sipParseArgs(sipArgs,"|lII",&a0,sipCanConvertTo_Index,&a1obj,sipCanConvertTo_Index,&a2obj))
		{
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);
			int istemp2 = sipConvertTo_Index(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BitVector::fill( (bool)a0,* a1,* a2);

			if (istemp1)
				delete a1;

			if (istemp2)
				delete a2;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BitVector,sipName_BALL_fill);

	return NULL;
}

static PyObject *sipDo_BitVector_toggle(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BitVector)) == NULL)
		return NULL;

	{
		Index *a0 = NULL;
		PyObject *a0obj = NULL;
		Index *a1 = NULL;
		PyObject *a1obj = NULL;

		if (sipParseArgs(sipArgs,"|II",sipCanConvertTo_Index,&a0obj,sipCanConvertTo_Index,&a1obj))
		{
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);
			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BitVector::toggle(* a0,* a1);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BitVector,sipName_BALL_toggle);

	return NULL;
}

static PyObject *sipDo_BitVector_bitwiseOr(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BitVector)) == NULL)
		return NULL;

	{
		const BitVector *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_BitVector,&a0obj))
		{
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_BitVector(a0obj,(BitVector **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BitVector::bitwiseOr(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BitVector,sipName_BALL_bitwiseOr);

	return NULL;
}

static PyObject *sipDo_BitVector_bitwiseXor(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BitVector)) == NULL)
		return NULL;

	{
		const BitVector *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_BitVector,&a0obj))
		{
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_BitVector(a0obj,(BitVector **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BitVector::bitwiseXor(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BitVector,sipName_BALL_bitwiseXor);

	return NULL;
}

static PyObject *sipDo_BitVector_bitwiseAnd(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BitVector)) == NULL)
		return NULL;

	{
		const BitVector *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_BitVector,&a0obj))
		{
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_BitVector(a0obj,(BitVector **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BitVector::bitwiseAnd(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BitVector,sipName_BALL_bitwiseAnd);

	return NULL;
}

static PyObject *sipDo_BitVector_isAnyBit(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BitVector)) == NULL)
		return NULL;

	{
		long a0;
		Index *a1 = NULL;
		PyObject *a1obj = NULL;
		Index *a2 = NULL;
		PyObject *a2obj = NULL;

		if (sipParseArgs(sipArgs,"l|II",&a0,sipCanConvertTo_Index,&a1obj,sipCanConvertTo_Index,&a2obj))
		{
			bool res;
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);
			int istemp2 = sipConvertTo_Index(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> BitVector::isAnyBit( (bool)a0,* a1,* a2);

			if (istemp1)
				delete a1;

			if (istemp2)
				delete a2;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BitVector,sipName_BALL_isAnyBit);

	return NULL;
}

static PyObject *sipDo_BitVector_isEveryBit(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BitVector)) == NULL)
		return NULL;

	{
		long a0;
		Index *a1 = NULL;
		PyObject *a1obj = NULL;
		Index *a2 = NULL;
		PyObject *a2obj = NULL;

		if (sipParseArgs(sipArgs,"l|II",&a0,sipCanConvertTo_Index,&a1obj,sipCanConvertTo_Index,&a2obj))
		{
			bool res;
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp1 = sipConvertTo_Index(a1obj,&a1,1,&iserr);
			int istemp2 = sipConvertTo_Index(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> BitVector::isEveryBit( (bool)a0,* a1,* a2);

			if (istemp1)
				delete a1;

			if (istemp2)
				delete a2;

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BitVector,sipName_BALL_isEveryBit);

	return NULL;
}

static PyObject *sipDo_BitVector_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BitVector)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			BitVector *ptr;

			if ((ptr = (BitVector *)sipGetCppPtr(sipThis,sipClass_BitVector)) == NULL)
				return NULL;

			res = ptr -> BitVector::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BitVector,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_BitVector(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_BitVector)
		return ptr;

	return NULL;
}

static void sipDealloc_BitVector(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (BitVector *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_BitVector(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_BitVector
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
			sipNew = new BitVector();
	}
	}

	if (sipNew == NULL)
	{
		int a0;

		if (sipParseArgs(sipArgs,"-i",&a0))
		{
			sipNew = new BitVector( a0);
	}
	}

	if (sipNew == NULL)
	{
		const BitVector *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_BitVector,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_BitVector(a0obj,(BitVector **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new BitVector(* a0, (bool)a1);
	}
	}

	if (sipNew == NULL)
	{
		const char *a0;

		if (sipParseArgs(sipArgs,"-s",&a0))
		{
			sipNew = new BitVector( a0);
	}
	}

	if (sipNew == NULL)
	{
		const BitVector *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_BitVector,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_BitVector(a0obj,(BitVector **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new BitVector(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_BitVector);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_BitVector,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (BitVector *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_BitVector[] = {
	{sipName_BALL_clear, sipDo_BitVector_clear, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_BitVector_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_BitVector_get, METH_VARARGS, NULL},
	{sipName_BALL_swap, sipDo_BitVector_swap, METH_VARARGS, NULL},
	{sipName_BALL_setSize, sipDo_BitVector_setSize, METH_VARARGS, NULL},
	{sipName_BALL_getSize, sipDo_BitVector_getSize, METH_VARARGS, NULL},
	{sipName_BALL_countValue, sipDo_BitVector_countValue, METH_VARARGS, NULL},
	{sipName_BALL_Operator__getitem__, sipDo_BitVector_Operator__getitem__, METH_VARARGS, NULL},
	{sipName_BALL_setBit, sipDo_BitVector_setBit, METH_VARARGS, NULL},
	{sipName_BALL_getBit, sipDo_BitVector_getBit, METH_VARARGS, NULL},
	{sipName_BALL_toggleBit, sipDo_BitVector_toggleBit, METH_VARARGS, NULL},
	{sipName_BALL_fill, sipDo_BitVector_fill, METH_VARARGS, NULL},
	{sipName_BALL_toggle, sipDo_BitVector_toggle, METH_VARARGS, NULL},
	{sipName_BALL_bitwiseOr, sipDo_BitVector_bitwiseOr, METH_VARARGS, NULL},
	{sipName_BALL_bitwiseXor, sipDo_BitVector_bitwiseXor, METH_VARARGS, NULL},
	{sipName_BALL_bitwiseAnd, sipDo_BitVector_bitwiseAnd, METH_VARARGS, NULL},
	{sipName_BALL_isAnyBit, sipDo_BitVector_isAnyBit, METH_VARARGS, NULL},
	{sipName_BALL_isEveryBit, sipDo_BitVector_isEveryBit, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_BitVector_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_BitVector(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_BitVector);
}

void sipConvertTo_BitVector(PyObject *sipPy,BitVector **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_BitVector);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (BitVector *)sipConvertToCpp(sipPy,sipClass_BitVector,sipIsErr);
}

BitVector *sipForceConvertTo_BitVector(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_BitVector(valobj))
	{
		BitVector *val;

		sipConvertTo_BitVector(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_BitVector);
	*iserrp = 1;

	return NULL;
}
