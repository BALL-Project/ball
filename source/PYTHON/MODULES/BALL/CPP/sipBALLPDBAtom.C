#include "sipBALLDeclBALL.h"
#include "sipBALLPDBAtom.h"


PyObject *sipClass_PDBAtom;

static void sipDealloc_PDBAtom(sipThisType *);
extern "C" PyObject * sip__str__PDBAtom(PyObject *a0);

static PyTypeObject sipType_PDBAtom = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_PDBAtom,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_PDBAtom,
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
	sip__str__PDBAtom,
	0,
	0,
	0,
	Py_TPFLAGS_DEFAULT,
	0,
	0,
	0,
};

sipPDBAtom::sipPDBAtom()
   : PDBAtom()
{
	sipCommonCtor(sipPyMethods,5);
}

sipPDBAtom::sipPDBAtom(const PDBAtom& a0,bool a1)
   : PDBAtom(a0,a1)
{
	sipCommonCtor(sipPyMethods,5);
}

sipPDBAtom::sipPDBAtom(Element& a0,const String& a1,const String& a2,AtomType a3,const Vector3& a4,const Vector3& a5,const Vector3& a6,float a7,float a8,char a9,char a10,char a11,float a12,float a13)
   : PDBAtom(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13)
{
	sipCommonCtor(sipPyMethods,5);
}

sipPDBAtom::sipPDBAtom(const PDBAtom& a0)
   : PDBAtom(a0)
{
	sipCommonCtor(sipPyMethods,5);
}

sipPDBAtom::~sipPDBAtom()
 throw()
{
	sipCommonDtor(sipPyThis);
}

void sipPDBAtom::deselect()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_deselect,&relLock))
		sipSelectable::sipVH_deselect(&sipPyMethods[0],sipPyThis,relLock);
	else
		Composite::deselect();
}

void sipPDBAtom::select()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_select,&relLock))
		sipSelectable::sipVH_select(&sipPyMethods[1],sipPyThis,relLock);
	else
		Composite::select();
}

bool sipPDBAtom::isValid() const
 throw()
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_isValid,&relLock) ?
		sipObject::sipVH_isValid(&sipPyMethods[2],sipPyThis,relLock) :
		PDBAtom::isValid();
}

void sipPDBAtom::destroy()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_destroy,&relLock))
		sipComposite::sipVH_destroy(&sipPyMethods[3],sipPyThis,relLock);
	else
		PDBAtom::destroy();
}

void sipPDBAtom::clear()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipObject::sipVH_clear(&sipPyMethods[4],sipPyThis,relLock);
	else
		PDBAtom::clear();
}

extern "C" PyObject *sipDo_PDBAtom_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PDBAtom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

			ptr -> PDBAtom::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PDBAtom,sipName_BALL_clear);

	return NULL;
}

extern "C" PyObject *sipDo_PDBAtom_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PDBAtom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

			ptr -> PDBAtom::destroy();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PDBAtom,sipName_BALL_destroy);

	return NULL;
}

extern "C" PyObject *sipDo_PDBAtom_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PDBAtom)) == NULL)
		return NULL;

	{
		const PDBAtom * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_PDBAtom,&a0obj,&a1))
		{
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_PDBAtom(a0obj,(PDBAtom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> PDBAtom::set(* a0, (bool)a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PDBAtom,sipName_BALL_set);

	return NULL;
}

extern "C" PyObject *sipDo_PDBAtom_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PDBAtom)) == NULL)
		return NULL;

	{
		PDBAtom * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_PDBAtom,&a0obj,&a1))
		{
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_PDBAtom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> PDBAtom::get(* a0, (bool)a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PDBAtom,sipName_BALL_get);

	return NULL;
}

extern "C" PyObject *sipDo_PDBAtom_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PDBAtom)) == NULL)
		return NULL;

	{
		PDBAtom * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_PDBAtom,&a0obj))
		{
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_PDBAtom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> PDBAtom::swap(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PDBAtom,sipName_BALL_swap);

	return NULL;
}

extern "C" PyObject *sipDo_PDBAtom_getProtein(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PDBAtom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Protein *res;
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

   try
   {
			res = ptr -> PDBAtom::getProtein();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Protein);
		}
	}

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const Protein *res;
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

   try
   {
			res = ptr -> PDBAtom::getProtein();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Protein);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PDBAtom,sipName_BALL_getProtein);

	return NULL;
}

extern "C" PyObject *sipDo_PDBAtom_getChain(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PDBAtom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Chain *res;
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

   try
   {
			res = ptr -> PDBAtom::getChain();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Chain);
		}
	}

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const Chain *res;
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

   try
   {
			res = ptr -> PDBAtom::getChain();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Chain);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PDBAtom,sipName_BALL_getChain);

	return NULL;
}

extern "C" PyObject *sipDo_PDBAtom_getResidue(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PDBAtom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Residue *res;
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

   try
   {
			res = ptr -> PDBAtom::getResidue();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Residue);
		}
	}

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const Residue *res;
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

   try
   {
			res = ptr -> PDBAtom::getResidue();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_Residue);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PDBAtom,sipName_BALL_getResidue);

	return NULL;
}

extern "C" PyObject *sipDo_PDBAtom_setBranchDesignator(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PDBAtom)) == NULL)
		return NULL;

	{
		char a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"c",&a0))
		{
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

   try
   {
			ptr -> PDBAtom::setBranchDesignator( a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PDBAtom,sipName_BALL_setBranchDesignator);

	return NULL;
}

extern "C" PyObject *sipDo_PDBAtom_getBranchDesignator(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PDBAtom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			char res;
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

   try
   {
			res = ptr -> PDBAtom::getBranchDesignator();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyString_FromStringAndSize(&res,1);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PDBAtom,sipName_BALL_getBranchDesignator);

	return NULL;
}

extern "C" PyObject *sipDo_PDBAtom_setRemotenessIndicator(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PDBAtom)) == NULL)
		return NULL;

	{
		char a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"c",&a0))
		{
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

   try
   {
			ptr -> PDBAtom::setRemotenessIndicator( a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PDBAtom,sipName_BALL_setRemotenessIndicator);

	return NULL;
}

extern "C" PyObject *sipDo_PDBAtom_getRemotenessIndicator(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PDBAtom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			char res;
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

   try
   {
			res = ptr -> PDBAtom::getRemotenessIndicator();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyString_FromStringAndSize(&res,1);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PDBAtom,sipName_BALL_getRemotenessIndicator);

	return NULL;
}

extern "C" PyObject *sipDo_PDBAtom_setAlternateLocationIndicator(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PDBAtom)) == NULL)
		return NULL;

	{
		char a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"c",&a0))
		{
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

   try
   {
			ptr -> PDBAtom::setAlternateLocationIndicator( a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PDBAtom,sipName_BALL_setAlternateLocationIndicator);

	return NULL;
}

extern "C" PyObject *sipDo_PDBAtom_getAlternateLocationIndicator(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PDBAtom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			char res;
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

   try
   {
			res = ptr -> PDBAtom::getAlternateLocationIndicator();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyString_FromStringAndSize(&res,1);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PDBAtom,sipName_BALL_getAlternateLocationIndicator);

	return NULL;
}

extern "C" PyObject *sipDo_PDBAtom_setOccupancy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PDBAtom)) == NULL)
		return NULL;

	{
		float a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"f",&a0))
		{
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

   try
   {
			ptr -> PDBAtom::setOccupancy( a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PDBAtom,sipName_BALL_setOccupancy);

	return NULL;
}

extern "C" PyObject *sipDo_PDBAtom_getOccupancy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PDBAtom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

   try
   {
			res = ptr -> PDBAtom::getOccupancy();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PDBAtom,sipName_BALL_getOccupancy);

	return NULL;
}

extern "C" PyObject *sipDo_PDBAtom_setTemperatureFactor(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PDBAtom)) == NULL)
		return NULL;

	{
		float a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"f",&a0))
		{
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

   try
   {
			ptr -> PDBAtom::setTemperatureFactor( a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PDBAtom,sipName_BALL_setTemperatureFactor);

	return NULL;
}

extern "C" PyObject *sipDo_PDBAtom_getTemperatureFactor(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PDBAtom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			float res;
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

   try
   {
			res = ptr -> PDBAtom::getTemperatureFactor();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_PDBAtom,sipName_BALL_getTemperatureFactor);

	return NULL;
}

extern "C" PyObject *sipDo_PDBAtom_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PDBAtom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			PDBAtom *ptr;

			if ((ptr = (PDBAtom *)sipGetCppPtr(sipThis,sipClass_PDBAtom)) == NULL)
				return NULL;

			res = ptr -> PDBAtom::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PDBAtom,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_PDBAtom(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_PDBAtom)
		return ptr;

	if ((res = sipCast_Atom((Atom *)(PDBAtom *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_PDBAtom(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipPDBAtom *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (PDBAtom *)sipThis -> u.cppPtr;
			else
				delete (sipPDBAtom *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}
extern "C" PyObject * sip__str__PDBAtom(PyObject *a0)
{
#line 60 "PDBAtom.sip"
  PDBAtom* ptr;
  if ((ptr = (PDBAtom*)sipGetCppPtr((sipThisType*)a0,sipClass_PDBAtom)) == NULL)
    return NULL;

  return PyString_FromString(String(String("PDBAtom ") + ptr->getName()
        + " { " + ptr->getElement().getSymbol() + " @ ("
        + String(ptr->getPosition().x) + " " + String(ptr->getPosition().y) + " "
        + String(ptr->getPosition().z) + " }").c_str());
#line 977 "sipBALLPDBAtom.cpp"
}

PyObject *sipNew_PDBAtom(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_PDBAtom
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
			sipNew = new sipPDBAtom();
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
		const PDBAtom * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_PDBAtom,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_PDBAtom(a0obj,(PDBAtom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipPDBAtom(* a0, (bool)a1);
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
		Element * a0;
		PyObject *a0obj;
		const String * a1;
		PyObject *a1obj;
		const String * a2;
		PyObject *a2obj;
		AtomType * a3;
		PyObject *a3obj;
		const Vector3 * a4;
		PyObject *a4obj;
		const Vector3 * a5;
		PyObject *a5obj;
		const Vector3 * a6;
		PyObject *a6obj;
		float a7;
		float a8;
		char a9;
		char a10;
		char a11;
		float a12;
		float a13;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-IIIIIIIffcccff",sipCanConvertTo_Element,&a0obj,sipCanConvertTo_String,&a1obj,sipCanConvertTo_String,&a2obj,sipCanConvertTo_AtomType,&a3obj,sipCanConvertTo_Vector3,&a4obj,sipCanConvertTo_Vector3,&a5obj,sipCanConvertTo_Vector3,&a6obj,&a7,&a8,&a9,&a10,&a11,&a12,&a13))
		{
			int iserr = 0;

			sipConvertTo_Element(a0obj,&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);
			int istemp2 = sipConvertTo_String(a2obj,(String **)&a2,1,&iserr);
			int istemp3 = sipConvertTo_AtomType(a3obj,&a3,1,&iserr);
			sipConvertTo_Vector3(a4obj,(Vector3 **)&a4,1,&iserr);
			sipConvertTo_Vector3(a5obj,(Vector3 **)&a5,1,&iserr);
			sipConvertTo_Vector3(a6obj,(Vector3 **)&a6,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipPDBAtom(* a0,* a1,* a2,* a3,* a4,* a5,* a6, a7, a8, a9, a10, a11, a12, a13);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp1)
				delete a1;

			if (istemp2)
				delete a2;

			if (istemp3)
				delete a3;
		}
	}

	if (sipNew == NULL)
	{
		const PDBAtom * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_PDBAtom,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_PDBAtom(a0obj,(PDBAtom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipPDBAtom(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_PDBAtom);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_PDBAtom,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (PDBAtom *)sipNew;
			else
				delete (sipPDBAtom *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipPDBAtom *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_PDBAtom[] = {
	{sipName_BALL_clear, sipDo_PDBAtom_clear, METH_VARARGS, NULL},
	{sipName_BALL_destroy, sipDo_PDBAtom_destroy, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_PDBAtom_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_PDBAtom_get, METH_VARARGS, NULL},
	{sipName_BALL_swap, sipDo_PDBAtom_swap, METH_VARARGS, NULL},
	{sipName_BALL_getProtein, sipDo_PDBAtom_getProtein, METH_VARARGS, NULL},
	{sipName_BALL_getChain, sipDo_PDBAtom_getChain, METH_VARARGS, NULL},
	{sipName_BALL_getResidue, sipDo_PDBAtom_getResidue, METH_VARARGS, NULL},
	{sipName_BALL_setBranchDesignator, sipDo_PDBAtom_setBranchDesignator, METH_VARARGS, NULL},
	{sipName_BALL_getBranchDesignator, sipDo_PDBAtom_getBranchDesignator, METH_VARARGS, NULL},
	{sipName_BALL_setRemotenessIndicator, sipDo_PDBAtom_setRemotenessIndicator, METH_VARARGS, NULL},
	{sipName_BALL_getRemotenessIndicator, sipDo_PDBAtom_getRemotenessIndicator, METH_VARARGS, NULL},
	{sipName_BALL_setAlternateLocationIndicator, sipDo_PDBAtom_setAlternateLocationIndicator, METH_VARARGS, NULL},
	{sipName_BALL_getAlternateLocationIndicator, sipDo_PDBAtom_getAlternateLocationIndicator, METH_VARARGS, NULL},
	{sipName_BALL_setOccupancy, sipDo_PDBAtom_setOccupancy, METH_VARARGS, NULL},
	{sipName_BALL_getOccupancy, sipDo_PDBAtom_getOccupancy, METH_VARARGS, NULL},
	{sipName_BALL_setTemperatureFactor, sipDo_PDBAtom_setTemperatureFactor, METH_VARARGS, NULL},
	{sipName_BALL_getTemperatureFactor, sipDo_PDBAtom_getTemperatureFactor, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_PDBAtom_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_PDBAtom(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_PDBAtom);
}

void sipConvertTo_PDBAtom(PyObject *sipPy,PDBAtom **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_PDBAtom);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (PDBAtom *)sipConvertToCpp(sipPy,sipClass_PDBAtom,sipIsErr);
}

PDBAtom *sipForceConvertTo_PDBAtom(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_PDBAtom(valobj))
	{
		PDBAtom *val;

		sipConvertTo_PDBAtom(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_PDBAtom);

	*iserrp = 1;

	return NULL;
}
