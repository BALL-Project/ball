#include "sipBALLDeclBALL.h"
#include "sipBALLAtomContainer.h"


PyObject *sipClass_AtomContainer;

static void sipDealloc_AtomContainer(sipThisType *);
static PyObject * sip__str__AtomContainer(PyObject *a0);

static PyTypeObject sipType_AtomContainer = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_AtomContainer,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_AtomContainer,
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
	sip__str__AtomContainer,
	0,
	0,
	0,
	Py_TPFLAGS_DEFAULT,
	0,
	0,
	0,
};

sipAtomContainer::sipAtomContainer()
   : AtomContainer()
{
	sipCommonCtor(sipPyMethods,5);
}

sipAtomContainer::sipAtomContainer(const AtomContainer& a0,bool a1)
   : AtomContainer(a0,a1)
{
	sipCommonCtor(sipPyMethods,5);
}

sipAtomContainer::sipAtomContainer(const String& a0)
   : AtomContainer(a0)
{
	sipCommonCtor(sipPyMethods,5);
}

sipAtomContainer::sipAtomContainer(const AtomContainer& a0)
   : AtomContainer(a0)
{
	sipCommonCtor(sipPyMethods,5);
}

sipAtomContainer::~sipAtomContainer()
 throw()
{
	sipCommonDtor(sipPyThis);
}

void sipAtomContainer::deselect()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_deselect,&relLock))
		sipSelectable::sipVH_deselect(&sipPyMethods[0],sipPyThis,relLock);
	else
		Composite::deselect();
}

void sipAtomContainer::select()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_select,&relLock))
		sipSelectable::sipVH_select(&sipPyMethods[1],sipPyThis,relLock);
	else
		Composite::select();
}

bool sipAtomContainer::isValid() const
 throw()
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_isValid,&relLock) ?
		sipObject::sipVH_isValid(&sipPyMethods[2],sipPyThis,relLock) :
		AtomContainer::isValid();
}

void sipAtomContainer::destroy()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_destroy,&relLock))
		sipComposite::sipVH_destroy(&sipPyMethods[3],sipPyThis,relLock);
	else
		AtomContainer::destroy();
}

void sipAtomContainer::clear()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipObject::sipVH_clear(&sipPyMethods[4],sipPyThis,relLock);
	else
		AtomContainer::clear();
}

static PyObject *sipDo_AtomContainer_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			ptr -> AtomContainer::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_AtomContainer_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			ptr -> AtomContainer::destroy();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_destroy);

	return NULL;
}

static PyObject *sipDo_AtomContainer_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		const AtomContainer * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_AtomContainer,&a0obj,&a1))
		{
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_AtomContainer(a0obj,(AtomContainer **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> AtomContainer::set(* a0, (bool)a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_AtomContainer_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		AtomContainer * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_AtomContainer,&a0obj,&a1))
		{
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_AtomContainer(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> AtomContainer::get(* a0, (bool)a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_get);

	return NULL;
}

static PyObject *sipDo_AtomContainer_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		AtomContainer * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_AtomContainer,&a0obj))
		{
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_AtomContainer(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> AtomContainer::swap(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_swap);

	return NULL;
}

static PyObject *sipDo_AtomContainer_setName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> AtomContainer::setName(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_setName);

	return NULL;
}

static PyObject *sipDo_AtomContainer_getName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const String *res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

   try
   {
			res = &ptr -> AtomContainer::getName();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_getName);

	return NULL;
}

static PyObject *sipDo_AtomContainer_getSuperAtomContainer(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			AtomContainer *res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

   try
   {
			res = ptr -> AtomContainer::getSuperAtomContainer();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_AtomContainer);
		}
	}

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const AtomContainer *res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

   try
   {
			res = ptr -> AtomContainer::getSuperAtomContainer();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_AtomContainer);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_getSuperAtomContainer);

	return NULL;
}

static PyObject *sipDo_AtomContainer_getAtomContainer(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		Position * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			AtomContainer *res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> AtomContainer::getAtomContainer(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_AtomContainer);
		}
	}

	{
		Position * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			const AtomContainer *res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> AtomContainer::getAtomContainer(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_AtomContainer);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_getAtomContainer);

	return NULL;
}

static PyObject *sipDo_AtomContainer_getAtom(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		Position * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			Atom *res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> AtomContainer::getAtom(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_Atom);
		}
	}

	{
		Position * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			const Atom *res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> AtomContainer::getAtom(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_Atom);
		}
	}

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			Atom *res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> AtomContainer::getAtom(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_Atom);
		}
	}

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			const Atom *res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> AtomContainer::getAtom(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_Atom);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_getAtom);

	return NULL;
}

static PyObject *sipDo_AtomContainer_countAtomContainers(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

   try
   {
			res = ptr -> AtomContainer::countAtomContainers();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_countAtomContainers);

	return NULL;
}

static PyObject *sipDo_AtomContainer_countAtoms(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

   try
   {
			res = ptr -> AtomContainer::countAtoms();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_countAtoms);

	return NULL;
}

static PyObject *sipDo_AtomContainer_countBonds(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

   try
   {
			res = ptr -> AtomContainer::countBonds();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_countBonds);

	return NULL;
}

static PyObject *sipDo_AtomContainer_countInterBonds(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

   try
   {
			res = ptr -> AtomContainer::countInterBonds();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_countInterBonds);

	return NULL;
}

static PyObject *sipDo_AtomContainer_countIntraBonds(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

   try
   {
			res = ptr -> AtomContainer::countIntraBonds();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_countIntraBonds);

	return NULL;
}

static PyObject *sipDo_AtomContainer_prepend(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		Atom * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> AtomContainer::prepend(* a0);
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
		AtomContainer * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_AtomContainer,&a0obj))
		{
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_AtomContainer(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> AtomContainer::prepend(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_prepend);

	return NULL;
}

static PyObject *sipDo_AtomContainer_append(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		Atom * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> AtomContainer::append(* a0);
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
		AtomContainer * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_AtomContainer,&a0obj))
		{
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_AtomContainer(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> AtomContainer::append(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_append);

	return NULL;
}

static PyObject *sipDo_AtomContainer_insert(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		Atom * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> AtomContainer::insert(* a0);
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
		AtomContainer * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_AtomContainer,&a0obj))
		{
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_AtomContainer(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> AtomContainer::insert(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_insert);

	return NULL;
}

static PyObject *sipDo_AtomContainer_insertBefore(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		Atom * a0;
		PyObject *a0obj;
		Composite * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_Atom,&a0obj,sipCanConvertTo_Composite,&a1obj))
		{
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);
			sipConvertTo_Composite(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> AtomContainer::insertBefore(* a0,* a1);
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
		AtomContainer * a0;
		PyObject *a0obj;
		Composite * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_AtomContainer,&a0obj,sipCanConvertTo_Composite,&a1obj))
		{
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_AtomContainer(a0obj,&a0,1,&iserr);
			sipConvertTo_Composite(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> AtomContainer::insertBefore(* a0,* a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_insertBefore);

	return NULL;
}

static PyObject *sipDo_AtomContainer_insertAfter(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		Atom * a0;
		PyObject *a0obj;
		Composite * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_Atom,&a0obj,sipCanConvertTo_Composite,&a1obj))
		{
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);
			sipConvertTo_Composite(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> AtomContainer::insertAfter(* a0,* a1);
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
		AtomContainer * a0;
		PyObject *a0obj;
		Composite * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_AtomContainer,&a0obj,sipCanConvertTo_Composite,&a1obj))
		{
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_AtomContainer(a0obj,&a0,1,&iserr);
			sipConvertTo_Composite(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> AtomContainer::insertAfter(* a0,* a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_insertAfter);

	return NULL;
}

static PyObject *sipDo_AtomContainer_remove(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		Atom * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			bool res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> AtomContainer::remove(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipConvertFromBool((int)res);
		}
	}

	{
		AtomContainer * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_AtomContainer,&a0obj))
		{
			bool res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_AtomContainer(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> AtomContainer::remove(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_remove);

	return NULL;
}

static PyObject *sipDo_AtomContainer_spliceBefore(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		AtomContainer * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_AtomContainer,&a0obj))
		{
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_AtomContainer(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> AtomContainer::spliceBefore(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_spliceBefore);

	return NULL;
}

static PyObject *sipDo_AtomContainer_spliceAfter(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		AtomContainer * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_AtomContainer,&a0obj))
		{
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_AtomContainer(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> AtomContainer::spliceAfter(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_spliceAfter);

	return NULL;
}

static PyObject *sipDo_AtomContainer_splice(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		AtomContainer * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_AtomContainer,&a0obj))
		{
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_AtomContainer(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> AtomContainer::splice(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_splice);

	return NULL;
}

static PyObject *sipDo_AtomContainer_destroyBonds(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

   try
   {
			ptr -> AtomContainer::destroyBonds();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_destroyBonds);

	return NULL;
}

static PyObject *sipDo_AtomContainer_isSubAtomContainerOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		const AtomContainer * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_AtomContainer,&a0obj))
		{
			bool res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_AtomContainer(a0obj,(AtomContainer **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> AtomContainer::isSubAtomContainerOf(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_isSubAtomContainerOf);

	return NULL;
}

static PyObject *sipDo_AtomContainer_isSuperAtomContainerOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		const AtomContainer * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_AtomContainer,&a0obj))
		{
			bool res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_AtomContainer(a0obj,(AtomContainer **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> AtomContainer::isSuperAtomContainerOf(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_isSuperAtomContainerOf);

	return NULL;
}

static PyObject *sipDo_AtomContainer_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			res = ptr -> AtomContainer::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_isValid);

	return NULL;
}

static PyObject *sipDo_AtomContainer_apply(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_AtomContainer)) == NULL)
		return NULL;

	{
		AtomProcessor * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_AtomProcessor,&a0obj))
		{
			bool res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_AtomProcessor(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> AtomContainer::apply(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipConvertFromBool((int)res);
		}
	}

	{
		CompositeProcessor * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_CompositeProcessor,&a0obj))
		{
			bool res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_CompositeProcessor(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> AtomContainer::apply(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipConvertFromBool((int)res);
		}
	}

	{
		FragmentProcessor * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_FragmentProcessor,&a0obj))
		{
			bool res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_FragmentProcessor(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> AtomContainer::apply(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipConvertFromBool((int)res);
		}
	}

	{
		ResidueProcessor * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_ResidueProcessor,&a0obj))
		{
			bool res;
			AtomContainer *ptr;

			if ((ptr = (AtomContainer *)sipGetCppPtr(sipThis,sipClass_AtomContainer)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_ResidueProcessor(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> AtomContainer::apply(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_AtomContainer,sipName_BALL_apply);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_AtomContainer(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_AtomContainer)
		return ptr;

	if ((res = sipCast_Composite((Composite *)(AtomContainer *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_PropertyManager((PropertyManager *)(AtomContainer *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_AtomContainer(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipAtomContainer *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (AtomContainer *)sipThis -> u.cppPtr;
			else
				delete (sipAtomContainer *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}
static PyObject * sip__str__AtomContainer(PyObject *a0)
{
#line 77 "atomContainer.sip"
  AtomContainer* ptr;
  if ((ptr = (AtomContainer*)sipGetCppPtr((sipThisType*)a0,sipClass_AtomContainer)) == NULL)
    return NULL;

  return PyString_FromString(String(String("AtomContainer ") + ptr->getName() 
						+ " { " + String(ptr->countAtoms()) + " atoms }").c_str());
#line 1914 "sipBALLAtomContainer.cpp"
}

PyObject *sipNew_AtomContainer(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_AtomContainer
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
			sipNew = new sipAtomContainer();
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
		const AtomContainer * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_AtomContainer,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_AtomContainer(a0obj,(AtomContainer **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipAtomContainer(* a0, (bool)a1);
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
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_String,&a0obj))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipAtomContainer(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;
		}
	}

	if (sipNew == NULL)
	{
		const AtomContainer * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_AtomContainer,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_AtomContainer(a0obj,(AtomContainer **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipAtomContainer(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_AtomContainer);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_AtomContainer,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (AtomContainer *)sipNew;
			else
				delete (sipAtomContainer *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipAtomContainer *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_AtomContainer[] = {
	{sipName_BALL_clear, sipDo_AtomContainer_clear, METH_VARARGS, NULL},
	{sipName_BALL_destroy, sipDo_AtomContainer_destroy, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_AtomContainer_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_AtomContainer_get, METH_VARARGS, NULL},
	{sipName_BALL_swap, sipDo_AtomContainer_swap, METH_VARARGS, NULL},
	{sipName_BALL_setName, sipDo_AtomContainer_setName, METH_VARARGS, NULL},
	{sipName_BALL_getName, sipDo_AtomContainer_getName, METH_VARARGS, NULL},
	{sipName_BALL_getSuperAtomContainer, sipDo_AtomContainer_getSuperAtomContainer, METH_VARARGS, NULL},
	{sipName_BALL_getAtomContainer, sipDo_AtomContainer_getAtomContainer, METH_VARARGS, NULL},
	{sipName_BALL_getAtom, sipDo_AtomContainer_getAtom, METH_VARARGS, NULL},
	{sipName_BALL_countAtomContainers, sipDo_AtomContainer_countAtomContainers, METH_VARARGS, NULL},
	{sipName_BALL_countAtoms, sipDo_AtomContainer_countAtoms, METH_VARARGS, NULL},
	{sipName_BALL_countBonds, sipDo_AtomContainer_countBonds, METH_VARARGS, NULL},
	{sipName_BALL_countInterBonds, sipDo_AtomContainer_countInterBonds, METH_VARARGS, NULL},
	{sipName_BALL_countIntraBonds, sipDo_AtomContainer_countIntraBonds, METH_VARARGS, NULL},
	{sipName_BALL_prepend, sipDo_AtomContainer_prepend, METH_VARARGS, NULL},
	{sipName_BALL_append, sipDo_AtomContainer_append, METH_VARARGS, NULL},
	{sipName_BALL_insert, sipDo_AtomContainer_insert, METH_VARARGS, NULL},
	{sipName_BALL_insertBefore, sipDo_AtomContainer_insertBefore, METH_VARARGS, NULL},
	{sipName_BALL_insertAfter, sipDo_AtomContainer_insertAfter, METH_VARARGS, NULL},
	{sipName_BALL_remove, sipDo_AtomContainer_remove, METH_VARARGS, NULL},
	{sipName_BALL_spliceBefore, sipDo_AtomContainer_spliceBefore, METH_VARARGS, NULL},
	{sipName_BALL_spliceAfter, sipDo_AtomContainer_spliceAfter, METH_VARARGS, NULL},
	{sipName_BALL_splice, sipDo_AtomContainer_splice, METH_VARARGS, NULL},
	{sipName_BALL_destroyBonds, sipDo_AtomContainer_destroyBonds, METH_VARARGS, NULL},
	{sipName_BALL_isSubAtomContainerOf, sipDo_AtomContainer_isSubAtomContainerOf, METH_VARARGS, NULL},
	{sipName_BALL_isSuperAtomContainerOf, sipDo_AtomContainer_isSuperAtomContainerOf, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_AtomContainer_isValid, METH_VARARGS, NULL},
	{sipName_BALL_apply, sipDo_AtomContainer_apply, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_AtomContainer(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_AtomContainer);
}

void sipConvertTo_AtomContainer(PyObject *sipPy,AtomContainer **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_AtomContainer);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (AtomContainer *)sipConvertToCpp(sipPy,sipClass_AtomContainer,sipIsErr);
}

AtomContainer *sipForceConvertTo_AtomContainer(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_AtomContainer(valobj))
	{
		AtomContainer *val;

		sipConvertTo_AtomContainer(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_AtomContainer);

	*iserrp = 1;

	return NULL;
}
