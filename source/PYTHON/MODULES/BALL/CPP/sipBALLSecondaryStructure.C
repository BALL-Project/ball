#include "sipBALLDeclBALL.h"
#include "sipBALLSecondaryStructure.h"


PyObject *sipClass_SecondaryStructure;

static void sipDealloc_SecondaryStructure(sipThisType *);
extern "C" PyObject * sip__str__SecondaryStructure(PyObject *a0);

static PyTypeObject sipType_SecondaryStructure = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_SecondaryStructure,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_SecondaryStructure,
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
	sip__str__SecondaryStructure,
	0,
	0,
	0,
	Py_TPFLAGS_DEFAULT,
	0,
	0,
	0,
};

sipSecondaryStructure::sipSecondaryStructure()
   : SecondaryStructure()
{
	sipCommonCtor(sipPyMethods,5);
}

sipSecondaryStructure::sipSecondaryStructure(const SecondaryStructure& a0,bool a1)
   : SecondaryStructure(a0,a1)
{
	sipCommonCtor(sipPyMethods,5);
}

sipSecondaryStructure::sipSecondaryStructure(const String& a0)
   : SecondaryStructure(a0)
{
	sipCommonCtor(sipPyMethods,5);
}

sipSecondaryStructure::sipSecondaryStructure(const SecondaryStructure& a0)
   : SecondaryStructure(a0)
{
	sipCommonCtor(sipPyMethods,5);
}

sipSecondaryStructure::~sipSecondaryStructure()
 throw()
{
	sipCommonDtor(sipPyThis);
}

void sipSecondaryStructure::deselect()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_deselect,&relLock))
		sipSelectable::sipVH_deselect(&sipPyMethods[0],sipPyThis,relLock);
	else
		Composite::deselect();
}

void sipSecondaryStructure::select()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_select,&relLock))
		sipSelectable::sipVH_select(&sipPyMethods[1],sipPyThis,relLock);
	else
		Composite::select();
}

bool sipSecondaryStructure::isValid() const
 throw()
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_isValid,&relLock) ?
		sipObject::sipVH_isValid(&sipPyMethods[2],sipPyThis,relLock) :
		SecondaryStructure::isValid();
}

void sipSecondaryStructure::destroy()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_destroy,&relLock))
		sipComposite::sipVH_destroy(&sipPyMethods[3],sipPyThis,relLock);
	else
		SecondaryStructure::destroy();
}

void sipSecondaryStructure::clear()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipObject::sipVH_clear(&sipPyMethods[4],sipPyThis,relLock);
	else
		SecondaryStructure::clear();
}

extern "C" PyObject *sipDo_SecondaryStructure_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

			ptr -> SecondaryStructure::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_clear);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

			ptr -> SecondaryStructure::destroy();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_destroy);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		const SecondaryStructure * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_SecondaryStructure,&a0obj,&a1))
		{
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_SecondaryStructure(a0obj,(SecondaryStructure **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> SecondaryStructure::set(* a0, (bool)a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_set);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		SecondaryStructure * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_SecondaryStructure,&a0obj,&a1))
		{
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_SecondaryStructure(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> SecondaryStructure::get(* a0, (bool)a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_get);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		SecondaryStructure * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_SecondaryStructure,&a0obj))
		{
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_SecondaryStructure(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> SecondaryStructure::swap(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_swap);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_getProtein(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Protein *res;
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

   try
   {
			res = ptr -> SecondaryStructure::getProtein();
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
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

   try
   {
			res = ptr -> SecondaryStructure::getProtein();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_getProtein);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_getChain(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Chain *res;
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

   try
   {
			res = ptr -> SecondaryStructure::getChain();
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
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

   try
   {
			res = ptr -> SecondaryStructure::getChain();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_getChain);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_getResidue(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"i",&a0))
		{
			Residue *res;
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

   try
   {
			res = ptr -> SecondaryStructure::getResidue( a0);
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
		int a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"i",&a0))
		{
			const Residue *res;
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

   try
   {
			res = ptr -> SecondaryStructure::getResidue( a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_getResidue);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_getNTerminal(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Residue *res;
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

   try
   {
			res = ptr -> SecondaryStructure::getNTerminal();
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
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

   try
   {
			res = ptr -> SecondaryStructure::getNTerminal();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_getNTerminal);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_getCTerminal(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Residue *res;
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

   try
   {
			res = ptr -> SecondaryStructure::getCTerminal();
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
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

   try
   {
			res = ptr -> SecondaryStructure::getCTerminal();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_getCTerminal);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_getPDBAtom(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"i",&a0))
		{
			PDBAtom *res;
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

   try
   {
			res = ptr -> SecondaryStructure::getPDBAtom( a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_PDBAtom);
		}
	}

	{
		int a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"i",&a0))
		{
			const PDBAtom *res;
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

   try
   {
			res = ptr -> SecondaryStructure::getPDBAtom( a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_PDBAtom);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_getPDBAtom);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_countResidues(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

   try
   {
			res = ptr -> SecondaryStructure::countResidues();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_countResidues);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_countPDBAtoms(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

   try
   {
			res = ptr -> SecondaryStructure::countPDBAtoms();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_countPDBAtoms);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_prepend(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		Residue * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Residue,&a0obj))
		{
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Residue(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> SecondaryStructure::prepend(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_prepend);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_append(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		Residue * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Residue,&a0obj))
		{
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Residue(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> SecondaryStructure::append(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_append);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_insert(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		Residue * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Residue,&a0obj))
		{
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Residue(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> SecondaryStructure::insert(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_insert);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_insertBefore(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		Residue * a0;
		PyObject *a0obj;
		Composite * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_Residue,&a0obj,sipCanConvertTo_Composite,&a1obj))
		{
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Residue(a0obj,&a0,1,&iserr);
			sipConvertTo_Composite(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> SecondaryStructure::insertBefore(* a0,* a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_insertBefore);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_insertAfter(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		Residue * a0;
		PyObject *a0obj;
		Composite * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_Residue,&a0obj,sipCanConvertTo_Composite,&a1obj))
		{
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Residue(a0obj,&a0,1,&iserr);
			sipConvertTo_Composite(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> SecondaryStructure::insertAfter(* a0,* a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_insertAfter);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_remove(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		Residue * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Residue,&a0obj))
		{
			bool res;
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Residue(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> SecondaryStructure::remove(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_remove);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_spliceBefore(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		SecondaryStructure * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_SecondaryStructure,&a0obj))
		{
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_SecondaryStructure(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> SecondaryStructure::spliceBefore(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_spliceBefore);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_spliceAfter(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		SecondaryStructure * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_SecondaryStructure,&a0obj))
		{
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_SecondaryStructure(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> SecondaryStructure::spliceAfter(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_spliceAfter);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_splice(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		SecondaryStructure * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_SecondaryStructure,&a0obj))
		{
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_SecondaryStructure(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> SecondaryStructure::splice(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_splice);

	return NULL;
}

extern "C" PyObject *sipDo_SecondaryStructure_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_SecondaryStructure)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			SecondaryStructure *ptr;

			if ((ptr = (SecondaryStructure *)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
				return NULL;

			res = ptr -> SecondaryStructure::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_SecondaryStructure,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_SecondaryStructure(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_SecondaryStructure)
		return ptr;

	if ((res = sipCast_AtomContainer((AtomContainer *)(SecondaryStructure *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_SecondaryStructure(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipSecondaryStructure *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (SecondaryStructure *)sipThis -> u.cppPtr;
			else
				delete (sipSecondaryStructure *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}
extern "C" PyObject * sip__str__SecondaryStructure(PyObject *a0)
{
#line 61 "secondaryStructure.sip"
  SecondaryStructure* ptr;
  if ((ptr = (SecondaryStructure*)sipGetCppPtr((sipThisType*)a0,sipClass_SecondaryStructure)) == NULL)
    return NULL;

  return PyString_FromString(String(String("SecondaryStructure ") + ptr->getName() 
				+ " { " + String(ptr->countResidues()) + " residues }").c_str());
#line 1290 "sipBALLSecondaryStructure.cpp"
}

PyObject *sipNew_SecondaryStructure(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_SecondaryStructure
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
			sipNew = new sipSecondaryStructure();
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
		const SecondaryStructure * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_SecondaryStructure,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_SecondaryStructure(a0obj,(SecondaryStructure **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipSecondaryStructure(* a0, (bool)a1);
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
			sipNew = new sipSecondaryStructure(* a0);
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
		const SecondaryStructure * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_SecondaryStructure,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_SecondaryStructure(a0obj,(SecondaryStructure **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipSecondaryStructure(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_SecondaryStructure);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_SecondaryStructure,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (SecondaryStructure *)sipNew;
			else
				delete (sipSecondaryStructure *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipSecondaryStructure *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_SecondaryStructure[] = {
	{sipName_BALL_clear, sipDo_SecondaryStructure_clear, METH_VARARGS, NULL},
	{sipName_BALL_destroy, sipDo_SecondaryStructure_destroy, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_SecondaryStructure_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_SecondaryStructure_get, METH_VARARGS, NULL},
	{sipName_BALL_swap, sipDo_SecondaryStructure_swap, METH_VARARGS, NULL},
	{sipName_BALL_getProtein, sipDo_SecondaryStructure_getProtein, METH_VARARGS, NULL},
	{sipName_BALL_getChain, sipDo_SecondaryStructure_getChain, METH_VARARGS, NULL},
	{sipName_BALL_getResidue, sipDo_SecondaryStructure_getResidue, METH_VARARGS, NULL},
	{sipName_BALL_getNTerminal, sipDo_SecondaryStructure_getNTerminal, METH_VARARGS, NULL},
	{sipName_BALL_getCTerminal, sipDo_SecondaryStructure_getCTerminal, METH_VARARGS, NULL},
	{sipName_BALL_getPDBAtom, sipDo_SecondaryStructure_getPDBAtom, METH_VARARGS, NULL},
	{sipName_BALL_countResidues, sipDo_SecondaryStructure_countResidues, METH_VARARGS, NULL},
	{sipName_BALL_countPDBAtoms, sipDo_SecondaryStructure_countPDBAtoms, METH_VARARGS, NULL},
	{sipName_BALL_prepend, sipDo_SecondaryStructure_prepend, METH_VARARGS, NULL},
	{sipName_BALL_append, sipDo_SecondaryStructure_append, METH_VARARGS, NULL},
	{sipName_BALL_insert, sipDo_SecondaryStructure_insert, METH_VARARGS, NULL},
	{sipName_BALL_insertBefore, sipDo_SecondaryStructure_insertBefore, METH_VARARGS, NULL},
	{sipName_BALL_insertAfter, sipDo_SecondaryStructure_insertAfter, METH_VARARGS, NULL},
	{sipName_BALL_remove, sipDo_SecondaryStructure_remove, METH_VARARGS, NULL},
	{sipName_BALL_spliceBefore, sipDo_SecondaryStructure_spliceBefore, METH_VARARGS, NULL},
	{sipName_BALL_spliceAfter, sipDo_SecondaryStructure_spliceAfter, METH_VARARGS, NULL},
	{sipName_BALL_splice, sipDo_SecondaryStructure_splice, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_SecondaryStructure_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_SecondaryStructure(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_SecondaryStructure);
}

void sipConvertTo_SecondaryStructure(PyObject *sipPy,SecondaryStructure **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_SecondaryStructure);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (SecondaryStructure *)sipConvertToCpp(sipPy,sipClass_SecondaryStructure,sipIsErr);
}

SecondaryStructure *sipForceConvertTo_SecondaryStructure(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_SecondaryStructure(valobj))
	{
		SecondaryStructure *val;

		sipConvertTo_SecondaryStructure(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_SecondaryStructure);

	*iserrp = 1;

	return NULL;
}
