#include "sipBALLDeclBALL.h"
#include "sipBALLAngle.h"



PyObject *sipClass_Angle;

static void sipDealloc_Angle(sipThisType *);
static PyObject *sipPyInternalRepr_Angle(sipThisType *);

static PyTypeObject sipType_Angle = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Angle,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Angle,
	0,
	0,
	0,
	0,
	(reprfunc)sipPyInternalRepr_Angle,
};

static PyObject *sipDo_Angle_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Angle)) == NULL)
		return NULL;

	{
		Angle *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Angle,&a0obj))
		{
			Angle *ptr;

			if ((ptr = (Angle *)sipGetCppPtr(sipThis,sipClass_Angle)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Angle(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Angle::swap(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Angle,sipName_BALL_swap);

	return NULL;
}

static PyObject *sipDo_Angle_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Angle)) == NULL)
		return NULL;

	{
		float a0;
		long a1 = true;

		if (sipParseArgs(sipArgs,"f|l",&a0,&a1))
		{
			Angle *ptr;

			if ((ptr = (Angle *)sipGetCppPtr(sipThis,sipClass_Angle)) == NULL)
				return NULL;

			ptr -> Angle::set( a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		const Angle *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Angle,&a0obj))
		{
			Angle *ptr;

			if ((ptr = (Angle *)sipGetCppPtr(sipThis,sipClass_Angle)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Angle(a0obj,(Angle **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Angle::set(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Angle,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_Angle_toRadian(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Angle)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			Angle *ptr;

			if ((ptr = (Angle *)sipGetCppPtr(sipThis,sipClass_Angle)) == NULL)
				return NULL;

			res = ptr -> Angle::toRadian();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Angle,sipName_BALL_toRadian);

	return NULL;
}

static PyObject *sipDo_Angle_toDegree(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Angle)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			Angle *ptr;

			if ((ptr = (Angle *)sipGetCppPtr(sipThis,sipClass_Angle)) == NULL)
				return NULL;

			res = ptr -> Angle::toDegree();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Angle,sipName_BALL_toDegree);

	return NULL;
}

static PyObject *sipDo_Angle_normalize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Angle)) == NULL)
		return NULL;

	{
		long a0;

		if (sipParseArgs(sipArgs,"l",&a0))
		{
			Angle *ptr;

			if ((ptr = (Angle *)sipGetCppPtr(sipThis,sipClass_Angle)) == NULL)
				return NULL;

			ptr -> Angle::normalize( (Angle::Range)a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Angle,sipName_BALL_normalize);

	return NULL;
}

static PyObject *sipDo_Angle_negate(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Angle)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Angle *ptr;

			if ((ptr = (Angle *)sipGetCppPtr(sipThis,sipClass_Angle)) == NULL)
				return NULL;

			ptr -> Angle::negate();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Angle,sipName_BALL_negate);

	return NULL;
}

static PyObject *sipDo_Angle_Operator__pos__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Angle)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Angle *res;
			Angle *ptr;

			if ((ptr = (Angle *)sipGetCppPtr(sipThis,sipClass_Angle)) == NULL)
				return NULL;

			res = new Angle(ptr -> Angle::operator+());

			return sipNewCppToSelf(res,sipClass_Angle,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Angle,sipName_BALL_Operator__pos__);

	return NULL;
}

static PyObject *sipDo_Angle_Operator__neg__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Angle)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Angle *res;
			Angle *ptr;

			if ((ptr = (Angle *)sipGetCppPtr(sipThis,sipClass_Angle)) == NULL)
				return NULL;

			res = new Angle(ptr -> Angle::operator-());

			return sipNewCppToSelf(res,sipClass_Angle,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Angle,sipName_BALL_Operator__neg__);

	return NULL;
}

static PyObject *sipDo_Angle_Operator__add__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Angle)) == NULL)
		return NULL;

	{
		const Angle *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Angle,&a0obj))
		{
			Angle *res;
			Angle *ptr;

			if ((ptr = (Angle *)sipGetCppPtr(sipThis,sipClass_Angle)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Angle(a0obj,(Angle **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = new Angle(ptr -> Angle::operator+(* a0));

			return sipNewCppToSelf(res,sipClass_Angle,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Angle,sipName_BALL_Operator__add__);

	return NULL;
}

static PyObject *sipDo_Angle_Operator__sub__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Angle)) == NULL)
		return NULL;

	{
		const Angle *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Angle,&a0obj))
		{
			Angle *res;
			Angle *ptr;

			if ((ptr = (Angle *)sipGetCppPtr(sipThis,sipClass_Angle)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Angle(a0obj,(Angle **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = new Angle(ptr -> Angle::operator-(* a0));

			return sipNewCppToSelf(res,sipClass_Angle,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Angle,sipName_BALL_Operator__sub__);

	return NULL;
}

static PyObject *sipDo_Angle_Operator__div__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Angle)) == NULL)
		return NULL;

	{
		const Angle *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Angle,&a0obj))
		{
			Angle *res;
			Angle *ptr;

			if ((ptr = (Angle *)sipGetCppPtr(sipThis,sipClass_Angle)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Angle(a0obj,(Angle **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = new Angle(ptr -> Angle::operator/(* a0));

			return sipNewCppToSelf(res,sipClass_Angle,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Angle,sipName_BALL_Operator__div__);

	return NULL;
}

static PyObject *sipDo_Angle_Operator__cmp__(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Angle)) == NULL)
		return NULL;

	{
		const Angle *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Angle,&a0obj))
		{
			bool res;
			Angle *ptr;

			if ((ptr = (Angle *)sipGetCppPtr(sipThis,sipClass_Angle)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Angle(a0obj,(Angle **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Angle::operator==(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Angle,sipName_BALL_Operator__cmp__);

	return NULL;
}

static PyObject *sipDo_Angle_isEquivalent(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Angle)) == NULL)
		return NULL;

	{
		Angle *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Angle,&a0obj))
		{
			bool res;
			Angle *ptr;

			if ((ptr = (Angle *)sipGetCppPtr(sipThis,sipClass_Angle)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Angle(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Angle::isEquivalent(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Angle,sipName_BALL_isEquivalent);

	return NULL;
}

static PyObject *sipDo_Angle_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Angle)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Angle *ptr;

			if ((ptr = (Angle *)sipGetCppPtr(sipThis,sipClass_Angle)) == NULL)
				return NULL;

			res = ptr -> Angle::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Angle,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Angle(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Angle)
		return ptr;

	return NULL;
}

static void sipDealloc_Angle(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (Angle *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

static PyObject *sipPyInternalRepr_Angle(sipThisType *sipThis)
{
#line 60 "angle.sip"
  Angle* ptr;
  if ((ptr = (Angle*)sipGetCppPtr(sipThis,sipClass_Angle)) == NULL)
    return NULL;

  return PyString_FromString(String(ptr->toRadian()).c_str());
#line 555 "../CPP/sipBALLAngle.cpp"
}

PyObject *sipNew_Angle(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Angle
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
			sipNew = new Angle();
	}
	}

	if (sipNew == NULL)
	{
		const Angle *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_Angle,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Angle(a0obj,(Angle **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new Angle(* a0);
	}
	}

	if (sipNew == NULL)
	{
		float a0;
		long a1 = true;

		if (sipParseArgs(sipArgs,"-f|l",&a0,&a1))
		{
			sipNew = new Angle( a0, (bool)a1);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_Angle);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Angle,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (Angle *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Angle[] = {
	{sipName_BALL_swap, sipDo_Angle_swap, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_Angle_set, METH_VARARGS, NULL},
	{sipName_BALL_toRadian, sipDo_Angle_toRadian, METH_VARARGS, NULL},
	{sipName_BALL_toDegree, sipDo_Angle_toDegree, METH_VARARGS, NULL},
	{sipName_BALL_normalize, sipDo_Angle_normalize, METH_VARARGS, NULL},
	{sipName_BALL_negate, sipDo_Angle_negate, METH_VARARGS, NULL},
	{sipName_BALL_Operator__pos__, sipDo_Angle_Operator__pos__, METH_VARARGS, NULL},
	{sipName_BALL_Operator__neg__, sipDo_Angle_Operator__neg__, METH_VARARGS, NULL},
	{sipName_BALL_Operator__add__, sipDo_Angle_Operator__add__, METH_VARARGS, NULL},
	{sipName_BALL_Operator__sub__, sipDo_Angle_Operator__sub__, METH_VARARGS, NULL},
	{sipName_BALL_Operator__div__, sipDo_Angle_Operator__div__, METH_VARARGS, NULL},
	{sipName_BALL_Operator__cmp__, sipDo_Angle_Operator__cmp__, METH_VARARGS, NULL},
	{sipName_BALL_isEquivalent, sipDo_Angle_isEquivalent, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_Angle_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_Angle(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Angle);
}

void sipConvertTo_Angle(PyObject *sipPy,Angle **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_Angle);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Angle *)sipConvertToCpp(sipPy,sipClass_Angle,sipIsErr);
}

Angle *sipForceConvertTo_Angle(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Angle(valobj))
	{
		Angle *val;

		sipConvertTo_Angle(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Angle);
	*iserrp = 1;

	return NULL;
}
