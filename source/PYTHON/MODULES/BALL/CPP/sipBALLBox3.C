#include "sipBALLDeclBALL.h"
#include "sipBALLBox3.h"


PyObject *sipClass_Box3;

static void sipDealloc_Box3(sipThisType *);
extern "C" PyObject * sip__str__Box3(PyObject *a0);

static PyTypeObject sipType_Box3 = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Box3,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Box3,
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
	sip__str__Box3,
	0,
	0,
	0,
	Py_TPFLAGS_DEFAULT,
	0,
	0,
	0,
};

extern "C" PyObject *sipDo_Box3_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Box3)) == NULL)
		return NULL;

	{
		Box3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Box3,&a0obj))
		{
			Box3 *ptr;

			if ((ptr = (Box3 *)sipGetCppPtr(sipThis,sipClass_Box3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Box3(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Box3::set(* a0);
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
		Vector3 * a0;
		PyObject *a0obj;
		Vector3 * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj))
		{
			Box3 *ptr;

			if ((ptr = (Box3 *)sipGetCppPtr(sipThis,sipClass_Box3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Box3::set(* a0,* a1);
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
		float a3;
		float a4;
		float a5;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"ffffff",&a0,&a1,&a2,&a3,&a4,&a5))
		{
			Box3 *ptr;

			if ((ptr = (Box3 *)sipGetCppPtr(sipThis,sipClass_Box3)) == NULL)
				return NULL;

   try
   {
			ptr -> Box3::set( a0, a1, a2, a3, a4, a5);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Box3,sipName_BALL_set);

	return NULL;
}

extern "C" PyObject *sipDo_Box3_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Box3)) == NULL)
		return NULL;

	{
		Box3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Box3,&a0obj))
		{
			Box3 *ptr;

			if ((ptr = (Box3 *)sipGetCppPtr(sipThis,sipClass_Box3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Box3(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Box3::get(* a0);
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
		Vector3 * a0;
		PyObject *a0obj;
		Vector3 * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj))
		{
			Box3 *ptr;

			if ((ptr = (Box3 *)sipGetCppPtr(sipThis,sipClass_Box3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Box3::get(* a0,* a1);
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
		float a3;
		float a4;
		float a5;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"ffffff",&a0,&a1,&a2,&a3,&a4,&a5))
		{
			Box3 *ptr;

			if ((ptr = (Box3 *)sipGetCppPtr(sipThis,sipClass_Box3)) == NULL)
				return NULL;

   try
   {
			ptr -> Box3::get( a0, a1, a2, a3, a4, a5);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Box3,sipName_BALL_get);

	return NULL;
}

extern "C" PyObject *sipDo_Box3_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Box3)) == NULL)
		return NULL;

	{
		Box3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Box3,&a0obj))
		{
			Box3 *ptr;

			if ((ptr = (Box3 *)sipGetCppPtr(sipThis,sipClass_Box3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Box3(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Box3::swap(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Box3,sipName_BALL_swap);

	return NULL;
}

extern "C" PyObject *sipDo_Box3_getSurface(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Box3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			Box3 *ptr;

			if ((ptr = (Box3 *)sipGetCppPtr(sipThis,sipClass_Box3)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Box3::getSurface();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Box3,sipName_BALL_getSurface);

	return NULL;
}

extern "C" PyObject *sipDo_Box3_getVolume(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Box3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			Box3 *ptr;

			if ((ptr = (Box3 *)sipGetCppPtr(sipThis,sipClass_Box3)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Box3::getVolume();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Box3,sipName_BALL_getVolume);

	return NULL;
}

extern "C" PyObject *sipDo_Box3_getWidth(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Box3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			Box3 *ptr;

			if ((ptr = (Box3 *)sipGetCppPtr(sipThis,sipClass_Box3)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Box3::getWidth();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Box3,sipName_BALL_getWidth);

	return NULL;
}

extern "C" PyObject *sipDo_Box3_getHeight(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Box3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			Box3 *ptr;

			if ((ptr = (Box3 *)sipGetCppPtr(sipThis,sipClass_Box3)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Box3::getHeight();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Box3,sipName_BALL_getHeight);

	return NULL;
}

extern "C" PyObject *sipDo_Box3_getDepth(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Box3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			Box3 *ptr;

			if ((ptr = (Box3 *)sipGetCppPtr(sipThis,sipClass_Box3)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Box3::getDepth();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Box3,sipName_BALL_getDepth);

	return NULL;
}

extern "C" PyObject *sipDo_Box3_join(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Box3)) == NULL)
		return NULL;

	{
		Box3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Box3,&a0obj))
		{
			Box3 *ptr;

			if ((ptr = (Box3 *)sipGetCppPtr(sipThis,sipClass_Box3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Box3(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Box3::join(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Box3,sipName_BALL_join);

	return NULL;
}

extern "C" PyObject *sipDo_Box3_CmpOp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Box3)) == NULL)
		return NULL;

	{
		const Box3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Box3,&a0obj))
		{
			bool res;
			Box3 *ptr;

			if ((ptr = (Box3 *)sipGetCppPtr(sipThis,sipClass_Box3)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Box3(a0obj,(Box3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Box3::operator==(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Box3,sipName_BALL_CmpOp);

	return NULL;
}

extern "C" PyObject *sipDo_Box3_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Box3)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Box3 *ptr;

			if ((ptr = (Box3 *)sipGetCppPtr(sipThis,sipClass_Box3)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Box3::isValid();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Box3,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_Box3(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Box3)
		return ptr;

	return NULL;
}

static void sipDealloc_Box3(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (Box3 *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}
extern "C" PyObject * sip__str__Box3(PyObject *a0)
{
#line 42 "box3.sip"
  	Box3* ptr;
	  if ((ptr = (Box3*)sipGetCppPtr((sipThisType*)a0,sipClass_Box3)) == NULL)
	    return NULL;

   static String tmp;
   tmp = "Box3 { (";
   tmp += String(ptr->a.x) + " ";
   tmp += String(ptr->a.y) + " ";
   tmp += String(ptr->a.z) + ") (";
   tmp += String(ptr->b.x) + " ";
   tmp += String(ptr->b.y) + " ";
   tmp += String(ptr->b.z) + ") }";
   return PyString_FromString(tmp.c_str());
#line 678 "sipBALLBox3.cpp"
}

PyObject *sipNew_Box3(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Box3
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
			sipNew = new Box3();
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
		Box3 * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_Box3,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Box3(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new Box3(* a0);
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
		Vector3 * a0;
		PyObject *a0obj;
		Vector3 * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-II",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj))
		{
			int iserr = 0;

			sipConvertTo_Vector3(a0obj,&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new Box3(* a0,* a1);
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
		float a3;
		float a4;
		float a5;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-ffffff",&a0,&a1,&a2,&a3,&a4,&a5))
		{
   try
   {
			sipNew = new Box3( a0, a1, a2, a3, a4, a5);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_Box3);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Box3,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (Box3 *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Box3_b(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Vector3 *val;
	Box3 *ptr;

	if ((ptr = (Box3 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Box3)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = &ptr -> b;

		valobj = sipMapCppToSelf(val,sipClass_Vector3);

		return valobj;
	}

	val = sipForceConvertTo_Vector3(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_Box3,sipName_BALL_b);
		return NULL;
	}

	ptr -> b = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Box3_a(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Vector3 *val;
	Box3 *ptr;

	if ((ptr = (Box3 *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Box3)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = &ptr -> a;

		valobj = sipMapCppToSelf(val,sipClass_Vector3);

		return valobj;
	}

	val = sipForceConvertTo_Vector3(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_Box3,sipName_BALL_a);
		return NULL;
	}

	ptr -> a = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Box3[] = {
	{sipName_BALL_set, sipDo_Box3_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_Box3_get, METH_VARARGS, NULL},
	{sipName_BALL_swap, sipDo_Box3_swap, METH_VARARGS, NULL},
	{sipName_BALL_getSurface, sipDo_Box3_getSurface, METH_VARARGS, NULL},
	{sipName_BALL_getVolume, sipDo_Box3_getVolume, METH_VARARGS, NULL},
	{sipName_BALL_getWidth, sipDo_Box3_getWidth, METH_VARARGS, NULL},
	{sipName_BALL_getHeight, sipDo_Box3_getHeight, METH_VARARGS, NULL},
	{sipName_BALL_getDepth, sipDo_Box3_getDepth, METH_VARARGS, NULL},
	{sipName_BALL_join, sipDo_Box3_join, METH_VARARGS, NULL},
	{sipName_BALL_CmpOp, sipDo_Box3_CmpOp, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_Box3_isValid, METH_VARARGS, NULL},
	{NULL}
};

PyMethodDef sipClassVarTab_Box3[] = {
	{sipName_BALL_b, sipGetSetVar_Box3_b, 0, NULL},
	{sipName_BALL_a, sipGetSetVar_Box3_a, 0, NULL},
	{NULL}
};

PyMethodDef *sipClassVarHierTab_Box3[] = {
	sipClassVarTab_Box3,
	NULL
};

int sipCanConvertTo_Box3(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Box3);
}

void sipConvertTo_Box3(PyObject *sipPy,Box3 **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_Box3);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Box3 *)sipConvertToCpp(sipPy,sipClass_Box3,sipIsErr);
}

Box3 *sipForceConvertTo_Box3(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Box3(valobj))
	{
		Box3 *val;

		sipConvertTo_Box3(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Box3);

	*iserrp = 1;

	return NULL;
}
