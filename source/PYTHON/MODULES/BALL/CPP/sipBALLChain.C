#include "sipBALLDeclBALL.h"
#include "sipBALLChain.h"


PyObject *sipClass_Chain;

static void sipDealloc_Chain(sipThisType *);
static PyObject * sip__str__Chain(PyObject *a0);

static PyTypeObject sipType_Chain = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Chain,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Chain,
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
	sip__str__Chain,
	0,
	0,
	0,
	Py_TPFLAGS_DEFAULT,
	0,
	0,
	0,
};

sipChain::sipChain()
   : Chain()
{
	sipCommonCtor(sipPyMethods,5);
}

sipChain::sipChain(const Chain& a0,bool a1)
   : Chain(a0,a1)
{
	sipCommonCtor(sipPyMethods,5);
}

sipChain::sipChain(const String& a0)
   : Chain(a0)
{
	sipCommonCtor(sipPyMethods,5);
}

sipChain::sipChain(const Chain& a0)
   : Chain(a0)
{
	sipCommonCtor(sipPyMethods,5);
}

sipChain::~sipChain()
 throw()
{
	sipCommonDtor(sipPyThis);
}

void sipChain::deselect()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_deselect,&relLock))
		sipSelectable::sipVH_deselect(&sipPyMethods[0],sipPyThis,relLock);
	else
		Composite::deselect();
}

void sipChain::select()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_select,&relLock))
		sipSelectable::sipVH_select(&sipPyMethods[1],sipPyThis,relLock);
	else
		Composite::select();
}

bool sipChain::isValid() const
 throw()
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_isValid,&relLock) ?
		sipObject::sipVH_isValid(&sipPyMethods[2],sipPyThis,relLock) :
		Chain::isValid();
}

void sipChain::destroy()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_destroy,&relLock))
		sipComposite::sipVH_destroy(&sipPyMethods[3],sipPyThis,relLock);
	else
		Chain::destroy();
}

void sipChain::clear()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipObject::sipVH_clear(&sipPyMethods[4],sipPyThis,relLock);
	else
		Chain::clear();
}

static PyObject *sipDo_Chain_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

			ptr -> Chain::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_Chain_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

			ptr -> Chain::destroy();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_destroy);

	return NULL;
}

static PyObject *sipDo_Chain_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		const Chain * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_Chain,&a0obj,&a1))
		{
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Chain(a0obj,(Chain **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Chain::set(* a0, (bool)a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_Chain_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		Chain * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_Chain,&a0obj,&a1))
		{
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Chain(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Chain::get(* a0, (bool)a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_get);

	return NULL;
}

static PyObject *sipDo_Chain_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		Chain * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Chain,&a0obj))
		{
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Chain(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Chain::swap(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_swap);

	return NULL;
}

static PyObject *sipDo_Chain_getProtein(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Protein *res;
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Chain::getProtein();
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
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Chain::getProtein();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_getProtein);

	return NULL;
}

static PyObject *sipDo_Chain_getSecondaryStructure(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"i",&a0))
		{
			SecondaryStructure *res;
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Chain::getSecondaryStructure( a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_SecondaryStructure);
		}
	}

	{
		int a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"i",&a0))
		{
			const SecondaryStructure *res;
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Chain::getSecondaryStructure( a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_SecondaryStructure);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_getSecondaryStructure);

	return NULL;
}

static PyObject *sipDo_Chain_getResidue(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"i",&a0))
		{
			Residue *res;
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Chain::getResidue( a0);
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
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Chain::getResidue( a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_getResidue);

	return NULL;
}

static PyObject *sipDo_Chain_getNTerminal(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Residue *res;
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Chain::getNTerminal();
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
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Chain::getNTerminal();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_getNTerminal);

	return NULL;
}

static PyObject *sipDo_Chain_getCTerminal(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Residue *res;
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Chain::getCTerminal();
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
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Chain::getCTerminal();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_getCTerminal);

	return NULL;
}

static PyObject *sipDo_Chain_getPDBAtom(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"i",&a0))
		{
			PDBAtom *res;
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Chain::getPDBAtom( a0);
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
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Chain::getPDBAtom( a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_getPDBAtom);

	return NULL;
}

static PyObject *sipDo_Chain_countSecondaryStructures(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Chain::countSecondaryStructures();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_countSecondaryStructures);

	return NULL;
}

static PyObject *sipDo_Chain_countResidues(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Chain::countResidues();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_countResidues);

	return NULL;
}

static PyObject *sipDo_Chain_countPDBAtoms(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Chain::countPDBAtoms();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_countPDBAtoms);

	return NULL;
}

static PyObject *sipDo_Chain_prepend(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		SecondaryStructure * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_SecondaryStructure,&a0obj))
		{
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_SecondaryStructure(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Chain::prepend(* a0);
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
		Residue * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Residue,&a0obj))
		{
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Residue(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Chain::prepend(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_prepend);

	return NULL;
}

static PyObject *sipDo_Chain_append(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		SecondaryStructure * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_SecondaryStructure,&a0obj))
		{
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_SecondaryStructure(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Chain::append(* a0);
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
		Residue * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Residue,&a0obj))
		{
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Residue(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Chain::append(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_append);

	return NULL;
}

static PyObject *sipDo_Chain_insert(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		SecondaryStructure * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_SecondaryStructure,&a0obj))
		{
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_SecondaryStructure(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Chain::insert(* a0);
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
		Residue * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Residue,&a0obj))
		{
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Residue(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Chain::insert(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_insert);

	return NULL;
}

static PyObject *sipDo_Chain_insertBefore(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		SecondaryStructure * a0;
		PyObject *a0obj;
		Composite * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_SecondaryStructure,&a0obj,sipCanConvertTo_Composite,&a1obj))
		{
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_SecondaryStructure(a0obj,&a0,1,&iserr);
			sipConvertTo_Composite(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Chain::insertBefore(* a0,* a1);
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
		Residue * a0;
		PyObject *a0obj;
		Composite * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_Residue,&a0obj,sipCanConvertTo_Composite,&a1obj))
		{
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Residue(a0obj,&a0,1,&iserr);
			sipConvertTo_Composite(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Chain::insertBefore(* a0,* a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_insertBefore);

	return NULL;
}

static PyObject *sipDo_Chain_insertAfter(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		SecondaryStructure * a0;
		PyObject *a0obj;
		Composite * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_SecondaryStructure,&a0obj,sipCanConvertTo_Composite,&a1obj))
		{
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_SecondaryStructure(a0obj,&a0,1,&iserr);
			sipConvertTo_Composite(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Chain::insertAfter(* a0,* a1);
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
		Residue * a0;
		PyObject *a0obj;
		Composite * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_Residue,&a0obj,sipCanConvertTo_Composite,&a1obj))
		{
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Residue(a0obj,&a0,1,&iserr);
			sipConvertTo_Composite(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Chain::insertAfter(* a0,* a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_insertAfter);

	return NULL;
}

static PyObject *sipDo_Chain_remove(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		SecondaryStructure * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_SecondaryStructure,&a0obj))
		{
			bool res;
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_SecondaryStructure(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Chain::remove(* a0);
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
		Residue * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Residue,&a0obj))
		{
			bool res;
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Residue(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Chain::remove(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_remove);

	return NULL;
}

static PyObject *sipDo_Chain_spliceBefore(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		Chain * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Chain,&a0obj))
		{
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Chain(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Chain::spliceBefore(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_spliceBefore);

	return NULL;
}

static PyObject *sipDo_Chain_spliceAfter(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		Chain * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Chain,&a0obj))
		{
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Chain(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Chain::spliceAfter(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_spliceAfter);

	return NULL;
}

static PyObject *sipDo_Chain_splice(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		Chain * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Chain,&a0obj))
		{
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Chain(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Chain::splice(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_splice);

	return NULL;
}

static PyObject *sipDo_Chain_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Chain)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Chain *ptr;

			if ((ptr = (Chain *)sipGetCppPtr(sipThis,sipClass_Chain)) == NULL)
				return NULL;

			res = ptr -> Chain::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Chain,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Chain(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_Chain)
		return ptr;

	if ((res = sipCast_AtomContainer((AtomContainer *)(Chain *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_Chain(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipChain *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (Chain *)sipThis -> u.cppPtr;
			else
				delete (sipChain *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}
static PyObject * sip__str__Chain(PyObject *a0)
{
#line 61 "chain.sip"
  Chain* ptr;
  if ((ptr = (Chain*)sipGetCppPtr((sipThisType*)a0,sipClass_Chain)) == NULL)
    return NULL;

  return PyString_FromString(String(String("Chain ") + ptr->getName()
				+ " { " + String(ptr->countResidues()) + " residues }").c_str());
#line 1536 "sipBALLChain.cpp"
}

PyObject *sipNew_Chain(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Chain
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
			sipNew = new sipChain();
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
		const Chain * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_Chain,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_Chain(a0obj,(Chain **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipChain(* a0, (bool)a1);
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
			sipNew = new sipChain(* a0);
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
		const Chain * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_Chain,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Chain(a0obj,(Chain **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipChain(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_Chain);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Chain,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (Chain *)sipNew;
			else
				delete (sipChain *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipChain *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Chain[] = {
	{sipName_BALL_clear, sipDo_Chain_clear, METH_VARARGS, NULL},
	{sipName_BALL_destroy, sipDo_Chain_destroy, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_Chain_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_Chain_get, METH_VARARGS, NULL},
	{sipName_BALL_swap, sipDo_Chain_swap, METH_VARARGS, NULL},
	{sipName_BALL_getProtein, sipDo_Chain_getProtein, METH_VARARGS, NULL},
	{sipName_BALL_getSecondaryStructure, sipDo_Chain_getSecondaryStructure, METH_VARARGS, NULL},
	{sipName_BALL_getResidue, sipDo_Chain_getResidue, METH_VARARGS, NULL},
	{sipName_BALL_getNTerminal, sipDo_Chain_getNTerminal, METH_VARARGS, NULL},
	{sipName_BALL_getCTerminal, sipDo_Chain_getCTerminal, METH_VARARGS, NULL},
	{sipName_BALL_getPDBAtom, sipDo_Chain_getPDBAtom, METH_VARARGS, NULL},
	{sipName_BALL_countSecondaryStructures, sipDo_Chain_countSecondaryStructures, METH_VARARGS, NULL},
	{sipName_BALL_countResidues, sipDo_Chain_countResidues, METH_VARARGS, NULL},
	{sipName_BALL_countPDBAtoms, sipDo_Chain_countPDBAtoms, METH_VARARGS, NULL},
	{sipName_BALL_prepend, sipDo_Chain_prepend, METH_VARARGS, NULL},
	{sipName_BALL_append, sipDo_Chain_append, METH_VARARGS, NULL},
	{sipName_BALL_insert, sipDo_Chain_insert, METH_VARARGS, NULL},
	{sipName_BALL_insertBefore, sipDo_Chain_insertBefore, METH_VARARGS, NULL},
	{sipName_BALL_insertAfter, sipDo_Chain_insertAfter, METH_VARARGS, NULL},
	{sipName_BALL_remove, sipDo_Chain_remove, METH_VARARGS, NULL},
	{sipName_BALL_spliceBefore, sipDo_Chain_spliceBefore, METH_VARARGS, NULL},
	{sipName_BALL_spliceAfter, sipDo_Chain_spliceAfter, METH_VARARGS, NULL},
	{sipName_BALL_splice, sipDo_Chain_splice, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_Chain_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_Chain(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Chain);
}

void sipConvertTo_Chain(PyObject *sipPy,Chain **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_Chain);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Chain *)sipConvertToCpp(sipPy,sipClass_Chain,sipIsErr);
}

Chain *sipForceConvertTo_Chain(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Chain(valobj))
	{
		Chain *val;

		sipConvertTo_Chain(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Chain);

	*iserrp = 1;

	return NULL;
}
