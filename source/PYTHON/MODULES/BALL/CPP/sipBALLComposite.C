#include "sipBALLDeclBALL.h"
#include "sipBALLComposite.h"


PyObject *sipClass_Composite;

static void sipDealloc_Composite(sipThisType *);

static PyTypeObject sipType_Composite = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Composite,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Composite,
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

sipComposite::sipComposite()
   : Composite()
{
	sipCommonCtor(sipPyMethods,5);
}

sipComposite::sipComposite(const Composite& a0,bool a1)
   : Composite(a0,a1)
{
	sipCommonCtor(sipPyMethods,5);
}

sipComposite::sipComposite(const Composite& a0)
   : Composite(a0)
{
	sipCommonCtor(sipPyMethods,5);
}

sipComposite::~sipComposite()
 throw()
{
	sipCommonDtor(sipPyThis);
}

bool sipComposite::isValid() const
 throw()
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_isValid,&relLock) ?
		sipObject::sipVH_isValid(&sipPyMethods[0],sipPyThis,relLock) :
		Composite::isValid();
}

void sipComposite::deselect()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_deselect,&relLock))
		sipSelectable::sipVH_deselect(&sipPyMethods[1],sipPyThis,relLock);
	else
		Composite::deselect();
}

void sipComposite::select()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_select,&relLock))
		sipSelectable::sipVH_select(&sipPyMethods[2],sipPyThis,relLock);
	else
		Composite::select();
}

void sipComposite::destroy()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_destroy,&relLock))
		sipComposite::sipVH_destroy(&sipPyMethods[3],sipPyThis,relLock);
	else
		Composite::destroy();
}

void sipComposite::clear()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipObject::sipVH_clear(&sipPyMethods[4],sipPyThis,relLock);
	else
		Composite::clear();
}

// The common handler for all classes that inherit this virtual member
// function.

void sipComposite::sipVH_destroy(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_Composite,sipName_BALL_destroy);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

extern "C" PyObject *sipDo_Composite_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			ptr -> Composite::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_clear);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			ptr -> Composite::destroy();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		long a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"l",&a0))
		{
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			ptr -> Composite::destroy( (bool)a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_destroy);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		const Composite * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_Composite,&a0obj,&a1))
		{
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Composite::set(* a0, (bool)a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_set);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		Composite * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_Composite,&a0obj,&a1))
		{
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Composite::get(* a0, (bool)a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_get);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_getDegree(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::getDegree();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_getDegree);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_countDescendants(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::countDescendants();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_countDescendants);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_getPathLength(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		const Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			int res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Composite::getPathLength(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_getPathLength);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_getDepth(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::getDepth();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_getDepth);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_getHeight(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::getHeight();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_getHeight);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_getRoot(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Composite *res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = &ptr -> Composite::getRoot();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Composite);
		}
	}

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const Composite *res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = &ptr -> Composite::getRoot();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Composite);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_getRoot);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_getLowestCommonAncestor(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			Composite *res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Composite::getLowestCommonAncestor(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Composite);
		}
	}

	{
		const Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			const Composite *res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Composite::getLowestCommonAncestor(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Composite);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_getLowestCommonAncestor);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_getParent(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Composite *res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::getParent();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Composite);
		}
	}

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const Composite *res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::getParent();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Composite);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_getParent);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_getChild(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		Index * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Index,&a0obj))
		{
			Composite *res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Composite::getChild(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_Composite);
		}
	}

	{
		Index * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Index,&a0obj))
		{
			const Composite *res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Composite::getChild(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_Composite);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_getChild);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_getSibling(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		Index * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Index,&a0obj))
		{
			Composite *res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Composite::getSibling(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_Composite);
		}
	}

	{
		Index * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Index,&a0obj))
		{
			const Composite *res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Composite::getSibling(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_Composite);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_getSibling);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_getFirstChild(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Composite *res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::getFirstChild();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Composite);
		}
	}

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const Composite *res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::getFirstChild();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Composite);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_getFirstChild);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_getLastChild(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Composite *res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::getLastChild();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Composite);
		}
	}

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const Composite *res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::getLastChild();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Composite);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_getLastChild);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_getModificationTime(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const PreciseTime *res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			res = &ptr -> Composite::getModificationTime();

			return sipMapCppToSelf(res,sipClass_PreciseTime);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_getModificationTime);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_getSelectionTime(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const PreciseTime *res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			res = &ptr -> Composite::getSelectionTime();

			return sipMapCppToSelf(res,sipClass_PreciseTime);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_getSelectionTime);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_expand(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			ptr -> Composite::expand();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_expand);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_collapse(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			ptr -> Composite::collapse();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_collapse);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_prependChild(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Composite::prependChild(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_prependChild);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_appendChild(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Composite::appendChild(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_appendChild);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_insertParent(PyObject *,PyObject *sipArgs)
{
	int sipArgsParsed = 0;

	{
		Composite * a0;
		PyObject *a0obj;
		Composite * a1;
		PyObject *a1obj;
		Composite * a2;
		PyObject *a2obj;
		long a3 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-III|l",sipCanConvertTo_Composite,&a0obj,sipCanConvertTo_Composite,&a1obj,sipCanConvertTo_Composite,&a2obj,&a3))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,&a0,1,&iserr);
			sipConvertTo_Composite(a1obj,&a1,1,&iserr);
			sipConvertTo_Composite(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = Composite::insertParent(* a0,* a1,* a2, (bool)a3);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_insertParent);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_insertBefore(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Composite::insertBefore(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_insertBefore);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_insertAfter(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Composite::insertAfter(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_insertAfter);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_spliceBefore(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Composite::spliceBefore(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_spliceBefore);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_spliceAfter(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Composite::spliceAfter(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_spliceAfter);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_splice(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Composite::splice(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_splice);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_removeChild(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Composite::removeChild(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_removeChild);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_replace(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Composite::replace(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_replace);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Composite::swap(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_swap);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_select(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			ptr -> Composite::select();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_select);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_deselect(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			ptr -> Composite::deselect();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_deselect);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_isExpanded(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::isExpanded();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_isExpanded);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_isCollapsed(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::isCollapsed();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_isCollapsed);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_isEmpty(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::isEmpty();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_isEmpty);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_isRoot(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::isRoot();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_isRoot);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_isRootOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		const Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Composite::isRootOf(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_isRootOf);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_isInterior(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::isInterior();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_isInterior);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_hasChild(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::hasChild();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_hasChild);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_isChildOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		const Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Composite::isChildOf(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_isChildOf);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_isFirstChild(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::isFirstChild();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_isFirstChild);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_isFirstChildOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		const Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Composite::isFirstChildOf(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_isFirstChildOf);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_isLastChild(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::isLastChild();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_isLastChild);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_isLastChildOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		const Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Composite::isLastChildOf(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_isLastChildOf);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_hasParent(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::hasParent();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_hasParent);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_isParentOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		const Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Composite::isParentOf(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_isParentOf);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_hasSibling(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::hasSibling();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_hasSibling);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_isSiblingOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		const Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Composite::isSiblingOf(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_isSiblingOf);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_hasPreviousSibling(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::hasPreviousSibling();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_hasPreviousSibling);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_isPreviousSiblingOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		const Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Composite::isPreviousSiblingOf(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_isPreviousSiblingOf);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_hasNextSibling(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::hasNextSibling();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_hasNextSibling);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_isNextSiblingOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		const Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Composite::isNextSiblingOf(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_isNextSiblingOf);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_isDescendantOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		const Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Composite::isDescendantOf(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_isDescendantOf);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_isAncestorOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		const Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Composite::isAncestorOf(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_isAncestorOf);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_isRelatedWith(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		const Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Composite::isRelatedWith(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_isRelatedWith);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_isHomomorph(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		const Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Composite,&a0obj))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Composite::isHomomorph(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_isHomomorph);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_containsSelection(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Composite::containsSelection();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_containsSelection);

	return NULL;
}

extern "C" PyObject *sipDo_Composite_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Composite)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Composite *ptr;

			if ((ptr = (Composite *)sipGetCppPtr(sipThis,sipClass_Composite)) == NULL)
				return NULL;

			res = ptr -> Composite::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Composite,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_Composite(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_Composite)
		return ptr;

	if ((res = sipCast_PersistentObject((PersistentObject *)(Composite *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_Object((Object *)(Composite *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_Selectable((Selectable *)(Composite *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_Composite(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipComposite *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (Composite *)sipThis -> u.cppPtr;
			else
				delete (sipComposite *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_Composite(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Composite
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
			sipNew = new sipComposite();
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
		const Composite * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_Composite,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipComposite(* a0, (bool)a1);
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
		const Composite * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_Composite,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Composite(a0obj,(Composite **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipComposite(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_Composite);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Composite,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (Composite *)sipNew;
			else
				delete (sipComposite *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipComposite *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Composite[] = {
	{sipName_BALL_clear, sipDo_Composite_clear, METH_VARARGS, NULL},
	{sipName_BALL_destroy, sipDo_Composite_destroy, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_Composite_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_Composite_get, METH_VARARGS, NULL},
	{sipName_BALL_getDegree, sipDo_Composite_getDegree, METH_VARARGS, NULL},
	{sipName_BALL_countDescendants, sipDo_Composite_countDescendants, METH_VARARGS, NULL},
	{sipName_BALL_getPathLength, sipDo_Composite_getPathLength, METH_VARARGS, NULL},
	{sipName_BALL_getDepth, sipDo_Composite_getDepth, METH_VARARGS, NULL},
	{sipName_BALL_getHeight, sipDo_Composite_getHeight, METH_VARARGS, NULL},
	{sipName_BALL_getRoot, sipDo_Composite_getRoot, METH_VARARGS, NULL},
	{sipName_BALL_getLowestCommonAncestor, sipDo_Composite_getLowestCommonAncestor, METH_VARARGS, NULL},
	{sipName_BALL_getParent, sipDo_Composite_getParent, METH_VARARGS, NULL},
	{sipName_BALL_getChild, sipDo_Composite_getChild, METH_VARARGS, NULL},
	{sipName_BALL_getSibling, sipDo_Composite_getSibling, METH_VARARGS, NULL},
	{sipName_BALL_getFirstChild, sipDo_Composite_getFirstChild, METH_VARARGS, NULL},
	{sipName_BALL_getLastChild, sipDo_Composite_getLastChild, METH_VARARGS, NULL},
	{sipName_BALL_getModificationTime, sipDo_Composite_getModificationTime, METH_VARARGS, NULL},
	{sipName_BALL_getSelectionTime, sipDo_Composite_getSelectionTime, METH_VARARGS, NULL},
	{sipName_BALL_expand, sipDo_Composite_expand, METH_VARARGS, NULL},
	{sipName_BALL_collapse, sipDo_Composite_collapse, METH_VARARGS, NULL},
	{sipName_BALL_prependChild, sipDo_Composite_prependChild, METH_VARARGS, NULL},
	{sipName_BALL_appendChild, sipDo_Composite_appendChild, METH_VARARGS, NULL},
	{sipName_BALL_insertParent, sipDo_Composite_insertParent, METH_VARARGS, NULL},
	{sipName_BALL_insertBefore, sipDo_Composite_insertBefore, METH_VARARGS, NULL},
	{sipName_BALL_insertAfter, sipDo_Composite_insertAfter, METH_VARARGS, NULL},
	{sipName_BALL_spliceBefore, sipDo_Composite_spliceBefore, METH_VARARGS, NULL},
	{sipName_BALL_spliceAfter, sipDo_Composite_spliceAfter, METH_VARARGS, NULL},
	{sipName_BALL_splice, sipDo_Composite_splice, METH_VARARGS, NULL},
	{sipName_BALL_removeChild, sipDo_Composite_removeChild, METH_VARARGS, NULL},
	{sipName_BALL_replace, sipDo_Composite_replace, METH_VARARGS, NULL},
	{sipName_BALL_swap, sipDo_Composite_swap, METH_VARARGS, NULL},
	{sipName_BALL_select, sipDo_Composite_select, METH_VARARGS, NULL},
	{sipName_BALL_deselect, sipDo_Composite_deselect, METH_VARARGS, NULL},
	{sipName_BALL_isExpanded, sipDo_Composite_isExpanded, METH_VARARGS, NULL},
	{sipName_BALL_isCollapsed, sipDo_Composite_isCollapsed, METH_VARARGS, NULL},
	{sipName_BALL_isEmpty, sipDo_Composite_isEmpty, METH_VARARGS, NULL},
	{sipName_BALL_isRoot, sipDo_Composite_isRoot, METH_VARARGS, NULL},
	{sipName_BALL_isRootOf, sipDo_Composite_isRootOf, METH_VARARGS, NULL},
	{sipName_BALL_isInterior, sipDo_Composite_isInterior, METH_VARARGS, NULL},
	{sipName_BALL_hasChild, sipDo_Composite_hasChild, METH_VARARGS, NULL},
	{sipName_BALL_isChildOf, sipDo_Composite_isChildOf, METH_VARARGS, NULL},
	{sipName_BALL_isFirstChild, sipDo_Composite_isFirstChild, METH_VARARGS, NULL},
	{sipName_BALL_isFirstChildOf, sipDo_Composite_isFirstChildOf, METH_VARARGS, NULL},
	{sipName_BALL_isLastChild, sipDo_Composite_isLastChild, METH_VARARGS, NULL},
	{sipName_BALL_isLastChildOf, sipDo_Composite_isLastChildOf, METH_VARARGS, NULL},
	{sipName_BALL_hasParent, sipDo_Composite_hasParent, METH_VARARGS, NULL},
	{sipName_BALL_isParentOf, sipDo_Composite_isParentOf, METH_VARARGS, NULL},
	{sipName_BALL_hasSibling, sipDo_Composite_hasSibling, METH_VARARGS, NULL},
	{sipName_BALL_isSiblingOf, sipDo_Composite_isSiblingOf, METH_VARARGS, NULL},
	{sipName_BALL_hasPreviousSibling, sipDo_Composite_hasPreviousSibling, METH_VARARGS, NULL},
	{sipName_BALL_isPreviousSiblingOf, sipDo_Composite_isPreviousSiblingOf, METH_VARARGS, NULL},
	{sipName_BALL_hasNextSibling, sipDo_Composite_hasNextSibling, METH_VARARGS, NULL},
	{sipName_BALL_isNextSiblingOf, sipDo_Composite_isNextSiblingOf, METH_VARARGS, NULL},
	{sipName_BALL_isDescendantOf, sipDo_Composite_isDescendantOf, METH_VARARGS, NULL},
	{sipName_BALL_isAncestorOf, sipDo_Composite_isAncestorOf, METH_VARARGS, NULL},
	{sipName_BALL_isRelatedWith, sipDo_Composite_isRelatedWith, METH_VARARGS, NULL},
	{sipName_BALL_isHomomorph, sipDo_Composite_isHomomorph, METH_VARARGS, NULL},
	{sipName_BALL_containsSelection, sipDo_Composite_containsSelection, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_Composite_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_Composite(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Composite);
}

void sipConvertTo_Composite(PyObject *sipPy,Composite **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_Composite);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Composite *)sipConvertToCpp(sipPy,sipClass_Composite,sipIsErr);
}

Composite *sipForceConvertTo_Composite(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Composite(valobj))
	{
		Composite *val;

		sipConvertTo_Composite(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Composite);

	*iserrp = 1;

	return NULL;
}
