#include "sipBALLDeclBALL.h"
#include "sipBALLSystem.h"


PyObject *sipClass_System;

static void sipDealloc_System(sipThisType *);
extern "C" PyObject * sip__str__System(PyObject *a0);

static PyTypeObject sipType_System = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_System,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_System,
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
	sip__str__System,
	0,
	0,
	0,
	Py_TPFLAGS_DEFAULT,
	0,
	0,
	0,
};

sipSystem::sipSystem()
   : System()
{
	sipCommonCtor(sipPyMethods,5);
}

sipSystem::sipSystem(const System& a0,bool a1)
   : System(a0,a1)
{
	sipCommonCtor(sipPyMethods,5);
}

sipSystem::sipSystem(const String& a0)
   : System(a0)
{
	sipCommonCtor(sipPyMethods,5);
}

sipSystem::sipSystem(const System& a0)
   : System(a0)
{
	sipCommonCtor(sipPyMethods,5);
}

sipSystem::~sipSystem()
 throw()
{
	sipCommonDtor(sipPyThis);
}

void sipSystem::deselect()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_deselect,&relLock))
		sipSelectable::sipVH_deselect(&sipPyMethods[0],sipPyThis,relLock);
	else
		Composite::deselect();
}

void sipSystem::select()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_select,&relLock))
		sipSelectable::sipVH_select(&sipPyMethods[1],sipPyThis,relLock);
	else
		Composite::select();
}

bool sipSystem::isValid() const
 throw()
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_isValid,&relLock) ?
		sipObject::sipVH_isValid(&sipPyMethods[2],sipPyThis,relLock) :
		System::isValid();
}

void sipSystem::destroy()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_destroy,&relLock))
		sipComposite::sipVH_destroy(&sipPyMethods[3],sipPyThis,relLock);
	else
		System::destroy();
}

void sipSystem::clear()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipObject::sipVH_clear(&sipPyMethods[4],sipPyThis,relLock);
	else
		System::clear();
}

extern "C" PyObject *sipDo_System_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_System)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			System *ptr;

			if ((ptr = (System *)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
				return NULL;

			ptr -> System::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_System,sipName_BALL_clear);

	return NULL;
}

extern "C" PyObject *sipDo_System_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_System)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			System *ptr;

			if ((ptr = (System *)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
				return NULL;

			ptr -> System::destroy();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_System,sipName_BALL_destroy);

	return NULL;
}

extern "C" PyObject *sipDo_System_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_System)) == NULL)
		return NULL;

	{
		const System * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_System,&a0obj,&a1))
		{
			System *ptr;

			if ((ptr = (System *)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_System(a0obj,(System **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> System::set(* a0, (bool)a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_System,sipName_BALL_set);

	return NULL;
}

extern "C" PyObject *sipDo_System_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_System)) == NULL)
		return NULL;

	{
		System * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_System,&a0obj,&a1))
		{
			System *ptr;

			if ((ptr = (System *)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_System(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> System::get(* a0, (bool)a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_System,sipName_BALL_get);

	return NULL;
}

extern "C" PyObject *sipDo_System_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_System)) == NULL)
		return NULL;

	{
		System * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_System,&a0obj))
		{
			System *ptr;

			if ((ptr = (System *)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_System(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> System::swap(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_System,sipName_BALL_swap);

	return NULL;
}

extern "C" PyObject *sipDo_System_setName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_System)) == NULL)
		return NULL;

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			System *ptr;

			if ((ptr = (System *)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> System::setName(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_System,sipName_BALL_setName);

	return NULL;
}

extern "C" PyObject *sipDo_System_getName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_System)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const String *res;
			System *ptr;

			if ((ptr = (System *)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
				return NULL;

   try
   {
			res = &ptr -> System::getName();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_System,sipName_BALL_getName);

	return NULL;
}

extern "C" PyObject *sipDo_System_countMolecules(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_System)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			System *ptr;

			if ((ptr = (System *)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
				return NULL;

   try
   {
			res = ptr -> System::countMolecules();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_System,sipName_BALL_countMolecules);

	return NULL;
}

extern "C" PyObject *sipDo_System_countFragments(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_System)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			System *ptr;

			if ((ptr = (System *)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
				return NULL;

   try
   {
			res = ptr -> System::countFragments();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_System,sipName_BALL_countFragments);

	return NULL;
}

extern "C" PyObject *sipDo_System_countAtoms(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_System)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			System *ptr;

			if ((ptr = (System *)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
				return NULL;

   try
   {
			res = ptr -> System::countAtoms();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_System,sipName_BALL_countAtoms);

	return NULL;
}

extern "C" PyObject *sipDo_System_prepend(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_System)) == NULL)
		return NULL;

	{
		Molecule * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Molecule,&a0obj))
		{
			System *ptr;

			if ((ptr = (System *)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Molecule(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> System::prepend(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_System,sipName_BALL_prepend);

	return NULL;
}

extern "C" PyObject *sipDo_System_append(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_System)) == NULL)
		return NULL;

	{
		Molecule * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Molecule,&a0obj))
		{
			System *ptr;

			if ((ptr = (System *)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Molecule(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> System::append(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_System,sipName_BALL_append);

	return NULL;
}

extern "C" PyObject *sipDo_System_insert(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_System)) == NULL)
		return NULL;

	{
		Molecule * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Molecule,&a0obj))
		{
			System *ptr;

			if ((ptr = (System *)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Molecule(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> System::insert(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_System,sipName_BALL_insert);

	return NULL;
}

extern "C" PyObject *sipDo_System_insertBefore(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_System)) == NULL)
		return NULL;

	{
		Molecule * a0;
		PyObject *a0obj;
		Composite * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_Molecule,&a0obj,sipCanConvertTo_Composite,&a1obj))
		{
			System *ptr;

			if ((ptr = (System *)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Molecule(a0obj,&a0,1,&iserr);
			sipConvertTo_Composite(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> System::insertBefore(* a0,* a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_System,sipName_BALL_insertBefore);

	return NULL;
}

extern "C" PyObject *sipDo_System_insertAfter(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_System)) == NULL)
		return NULL;

	{
		Molecule * a0;
		PyObject *a0obj;
		Composite * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_Molecule,&a0obj,sipCanConvertTo_Composite,&a1obj))
		{
			System *ptr;

			if ((ptr = (System *)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Molecule(a0obj,&a0,1,&iserr);
			sipConvertTo_Composite(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> System::insertAfter(* a0,* a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_System,sipName_BALL_insertAfter);

	return NULL;
}

extern "C" PyObject *sipDo_System_remove(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_System)) == NULL)
		return NULL;

	{
		Molecule * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Molecule,&a0obj))
		{
			bool res;
			System *ptr;

			if ((ptr = (System *)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Molecule(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> System::remove(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_System,sipName_BALL_remove);

	return NULL;
}

extern "C" PyObject *sipDo_System_spliceBefore(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_System)) == NULL)
		return NULL;

	{
		System * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_System,&a0obj))
		{
			System *ptr;

			if ((ptr = (System *)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_System(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> System::spliceBefore(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_System,sipName_BALL_spliceBefore);

	return NULL;
}

extern "C" PyObject *sipDo_System_spliceAfter(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_System)) == NULL)
		return NULL;

	{
		System * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_System,&a0obj))
		{
			System *ptr;

			if ((ptr = (System *)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_System(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> System::spliceAfter(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_System,sipName_BALL_spliceAfter);

	return NULL;
}

extern "C" PyObject *sipDo_System_splice(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_System)) == NULL)
		return NULL;

	{
		System * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_System,&a0obj))
		{
			System *ptr;

			if ((ptr = (System *)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_System(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> System::splice(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_System,sipName_BALL_splice);

	return NULL;
}

extern "C" PyObject *sipDo_System_destroyBonds(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_System)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			System *ptr;

			if ((ptr = (System *)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
				return NULL;

   try
   {
			ptr -> System::destroyBonds();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_System,sipName_BALL_destroyBonds);

	return NULL;
}

extern "C" PyObject *sipDo_System_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_System)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			System *ptr;

			if ((ptr = (System *)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
				return NULL;

			res = ptr -> System::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_System,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_System(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_System)
		return ptr;

	if ((res = sipCast_AtomContainer((AtomContainer *)(System *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_PropertyManager((PropertyManager *)(System *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_System(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipSystem *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (System *)sipThis -> u.cppPtr;
			else
				delete (sipSystem *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}
extern "C" PyObject * sip__str__System(PyObject *a0)
{
#line 50 "system.sip"
  System* ptr;
  if ((ptr = (System*)sipGetCppPtr((sipThisType*)a0,sipClass_System)) == NULL)
    return NULL;

  return PyString_FromString(String(String("System ") + ptr->getName() 
					+ " { " + String(ptr->countMolecules()) + " molecules,  " 
					+ String(ptr->countAtoms()) + " atoms }").c_str());
#line 1085 "sipBALLSystem.cpp"
}

PyObject *sipNew_System(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_System
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
			sipNew = new sipSystem();
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
		const System * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_System,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_System(a0obj,(System **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipSystem(* a0, (bool)a1);
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
			sipNew = new sipSystem(* a0);
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
		const System * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_System,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_System(a0obj,(System **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipSystem(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_System);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_System,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (System *)sipNew;
			else
				delete (sipSystem *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipSystem *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_System[] = {
	{sipName_BALL_clear, sipDo_System_clear, METH_VARARGS, NULL},
	{sipName_BALL_destroy, sipDo_System_destroy, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_System_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_System_get, METH_VARARGS, NULL},
	{sipName_BALL_swap, sipDo_System_swap, METH_VARARGS, NULL},
	{sipName_BALL_setName, sipDo_System_setName, METH_VARARGS, NULL},
	{sipName_BALL_getName, sipDo_System_getName, METH_VARARGS, NULL},
	{sipName_BALL_countMolecules, sipDo_System_countMolecules, METH_VARARGS, NULL},
	{sipName_BALL_countFragments, sipDo_System_countFragments, METH_VARARGS, NULL},
	{sipName_BALL_countAtoms, sipDo_System_countAtoms, METH_VARARGS, NULL},
	{sipName_BALL_prepend, sipDo_System_prepend, METH_VARARGS, NULL},
	{sipName_BALL_append, sipDo_System_append, METH_VARARGS, NULL},
	{sipName_BALL_insert, sipDo_System_insert, METH_VARARGS, NULL},
	{sipName_BALL_insertBefore, sipDo_System_insertBefore, METH_VARARGS, NULL},
	{sipName_BALL_insertAfter, sipDo_System_insertAfter, METH_VARARGS, NULL},
	{sipName_BALL_remove, sipDo_System_remove, METH_VARARGS, NULL},
	{sipName_BALL_spliceBefore, sipDo_System_spliceBefore, METH_VARARGS, NULL},
	{sipName_BALL_spliceAfter, sipDo_System_spliceAfter, METH_VARARGS, NULL},
	{sipName_BALL_splice, sipDo_System_splice, METH_VARARGS, NULL},
	{sipName_BALL_destroyBonds, sipDo_System_destroyBonds, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_System_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_System(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_System);
}

void sipConvertTo_System(PyObject *sipPy,System **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_System);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (System *)sipConvertToCpp(sipPy,sipClass_System,sipIsErr);
}

System *sipForceConvertTo_System(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_System(valobj))
	{
		System *val;

		sipConvertTo_System(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_System);

	*iserrp = 1;

	return NULL;
}
