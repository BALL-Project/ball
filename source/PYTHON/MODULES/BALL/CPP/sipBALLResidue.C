#include "sipBALLDeclBALL.h"
#include "sipBALLResidue.h"


PyObject *sipClass_Residue;

static void sipDealloc_Residue(sipThisType *);
static PyObject * sip__str__Residue(PyObject *a0);

static PyTypeObject sipType_Residue = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Residue,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Residue,
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
	sip__str__Residue,
	0,
	0,
	0,
	Py_TPFLAGS_DEFAULT,
	0,
	0,
	0,
};

sipResidue::sipResidue()
   : Residue()
{
	sipCommonCtor(sipPyMethods,5);
}

sipResidue::sipResidue(const Residue& a0,bool a1)
   : Residue(a0,a1)
{
	sipCommonCtor(sipPyMethods,5);
}

sipResidue::sipResidue(const String& a0,const String& a1,char a2)
   : Residue(a0,a1,a2)
{
	sipCommonCtor(sipPyMethods,5);
}

sipResidue::sipResidue(const Residue& a0)
   : Residue(a0)
{
	sipCommonCtor(sipPyMethods,5);
}

sipResidue::~sipResidue()
 throw()
{
	sipCommonDtor(sipPyThis);
}

void sipResidue::deselect()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_deselect,&relLock))
		sipSelectable::sipVH_deselect(&sipPyMethods[0],sipPyThis,relLock);
	else
		Composite::deselect();
}

void sipResidue::select()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_select,&relLock))
		sipSelectable::sipVH_select(&sipPyMethods[1],sipPyThis,relLock);
	else
		Composite::select();
}

bool sipResidue::isValid() const
 throw()
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_isValid,&relLock) ?
		sipObject::sipVH_isValid(&sipPyMethods[2],sipPyThis,relLock) :
		Residue::isValid();
}

void sipResidue::destroy()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_destroy,&relLock))
		sipComposite::sipVH_destroy(&sipPyMethods[3],sipPyThis,relLock);
	else
		Residue::destroy();
}

void sipResidue::clear()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipObject::sipVH_clear(&sipPyMethods[4],sipPyThis,relLock);
	else
		Residue::clear();
}

static PyObject *sipDo_Residue_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

			ptr -> Residue::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_Residue_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

			ptr -> Residue::destroy();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_destroy);

	return NULL;
}

static PyObject *sipDo_Residue_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		const Residue * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_Residue,&a0obj,&a1))
		{
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Residue(a0obj,(Residue **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Residue::set(* a0, (bool)a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_Residue_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		Residue * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I|l",sipCanConvertTo_Residue,&a0obj,&a1))
		{
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Residue(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Residue::get(* a0, (bool)a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_get);

	return NULL;
}

static PyObject *sipDo_Residue_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		Residue * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Residue,&a0obj))
		{
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Residue(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Residue::swap(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_swap);

	return NULL;
}

static PyObject *sipDo_Residue_getFullName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		long a0 = Residue::ADD_VARIANT_EXTENSIONS;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"|l",&a0))
		{
			String *res;
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

   try
   {
			res = new String(ptr -> Residue::getFullName( (Residue::FullNameType)a0));
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipNewCppToSelf(res,sipClass_String,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_getFullName);

	return NULL;
}

static PyObject *sipDo_Residue_hasTorsionPhi(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Residue::hasTorsionPhi();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_hasTorsionPhi);

	return NULL;
}

static PyObject *sipDo_Residue_getTorsionPhi(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Angle *res;
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

   try
   {
			res = new Angle(ptr -> Residue::getTorsionPhi());
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipNewCppToSelf(res,sipClass_Angle,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_getTorsionPhi);

	return NULL;
}

static PyObject *sipDo_Residue_hasTorsionPsi(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Residue::hasTorsionPsi();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_hasTorsionPsi);

	return NULL;
}

static PyObject *sipDo_Residue_getTorsionPsi(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Angle *res;
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

   try
   {
			res = new Angle(ptr -> Residue::getTorsionPsi());
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipNewCppToSelf(res,sipClass_Angle,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_getTorsionPsi);

	return NULL;
}

static PyObject *sipDo_Residue_getProtein(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Protein *res;
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Residue::getProtein();
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
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Residue::getProtein();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_getProtein);

	return NULL;
}

static PyObject *sipDo_Residue_getChain(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Chain *res;
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Residue::getChain();
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
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Residue::getChain();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_getChain);

	return NULL;
}

static PyObject *sipDo_Residue_getPDBAtom(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		Position * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			PDBAtom *res;
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Residue::getPDBAtom(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_PDBAtom);
		}
	}

	{
		Position * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			const PDBAtom *res;
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Residue::getPDBAtom(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_PDBAtom);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_getPDBAtom);

	return NULL;
}

static PyObject *sipDo_Residue_setID(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Residue::setID(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_setID);

	return NULL;
}

static PyObject *sipDo_Residue_getID(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const String *res;
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

   try
   {
			res = &ptr -> Residue::getID();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_getID);

	return NULL;
}

static PyObject *sipDo_Residue_setInsertionCode(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		char a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"c",&a0))
		{
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

   try
   {
			ptr -> Residue::setInsertionCode( a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_setInsertionCode);

	return NULL;
}

static PyObject *sipDo_Residue_getInsertionCode(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			char res;
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Residue::getInsertionCode();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_getInsertionCode);

	return NULL;
}

static PyObject *sipDo_Residue_countPDBAtoms(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Residue::countPDBAtoms();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_countPDBAtoms);

	return NULL;
}

static PyObject *sipDo_Residue_prepend(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		PDBAtom * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_PDBAtom,&a0obj))
		{
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_PDBAtom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Residue::prepend(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_prepend);

	return NULL;
}

static PyObject *sipDo_Residue_append(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		PDBAtom * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_PDBAtom,&a0obj))
		{
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_PDBAtom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Residue::append(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_append);

	return NULL;
}

static PyObject *sipDo_Residue_insert(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		PDBAtom * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_PDBAtom,&a0obj))
		{
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_PDBAtom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Residue::insert(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_insert);

	return NULL;
}

static PyObject *sipDo_Residue_insertBefore(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		PDBAtom * a0;
		PyObject *a0obj;
		Composite * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_PDBAtom,&a0obj,sipCanConvertTo_Composite,&a1obj))
		{
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_PDBAtom(a0obj,&a0,1,&iserr);
			sipConvertTo_Composite(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Residue::insertBefore(* a0,* a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_insertBefore);

	return NULL;
}

static PyObject *sipDo_Residue_insertAfter(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		PDBAtom * a0;
		PyObject *a0obj;
		Composite * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_PDBAtom,&a0obj,sipCanConvertTo_Composite,&a1obj))
		{
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_PDBAtom(a0obj,&a0,1,&iserr);
			sipConvertTo_Composite(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Residue::insertAfter(* a0,* a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_insertAfter);

	return NULL;
}

static PyObject *sipDo_Residue_remove(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		PDBAtom * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_PDBAtom,&a0obj))
		{
			bool res;
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_PDBAtom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> Residue::remove(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_remove);

	return NULL;
}

static PyObject *sipDo_Residue_spliceBefore(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		Residue * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Residue,&a0obj))
		{
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Residue(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Residue::spliceBefore(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_spliceBefore);

	return NULL;
}

static PyObject *sipDo_Residue_spliceAfter(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		Residue * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Residue,&a0obj))
		{
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Residue(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Residue::spliceAfter(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_spliceAfter);

	return NULL;
}

static PyObject *sipDo_Residue_splice(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		Residue * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_Residue,&a0obj))
		{
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Residue(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> Residue::splice(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_splice);

	return NULL;
}

static PyObject *sipDo_Residue_isAminoAcid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Residue::isAminoAcid();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_isAminoAcid);

	return NULL;
}

static PyObject *sipDo_Residue_isTerminal(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Residue::isTerminal();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_isTerminal);

	return NULL;
}

static PyObject *sipDo_Residue_isNTerminal(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Residue::isNTerminal();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_isNTerminal);

	return NULL;
}

static PyObject *sipDo_Residue_isCTerminal(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

   try
   {
			res = ptr -> Residue::isCTerminal();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_isCTerminal);

	return NULL;
}

static PyObject *sipDo_Residue_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Residue)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			Residue *ptr;

			if ((ptr = (Residue *)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
				return NULL;

			res = ptr -> Residue::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_Residue,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Residue(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_Residue)
		return ptr;

	if ((res = sipCast_Fragment((Fragment *)(Residue *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_Residue(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipResidue *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (Residue *)sipThis -> u.cppPtr;
			else
				delete (sipResidue *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}
static PyObject * sip__str__Residue(PyObject *a0)
{
#line 77 "residue.sip"
  Residue* ptr;
  if ((ptr = (Residue*)sipGetCppPtr((sipThisType*)a0,sipClass_Residue)) == NULL)
    return NULL;

  return PyString_FromString(String(String("Residue ") + ptr->getName() + ptr->getID()
        + " { " + String(ptr->countAtoms()) + " atoms }").c_str());
#line 1598 "sipBALLResidue.cpp"
}

PyObject *sipNew_Residue(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Residue
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
			sipNew = new sipResidue();
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
		const Residue * a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|l",sipCanConvertTo_Residue,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_Residue(a0obj,(Residue **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipResidue(* a0, (bool)a1);
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
		const String * a1 = NULL;
		PyObject *a1obj = NULL;
		char a2 = ' ';

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I|Ic",sipCanConvertTo_String,&a0obj,sipCanConvertTo_String,&a1obj,&a2))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipResidue(* a0,* a1, a2);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;
		}
	}

	if (sipNew == NULL)
	{
		const Residue * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_Residue,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Residue(a0obj,(Residue **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipResidue(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_Residue);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Residue,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (Residue *)sipNew;
			else
				delete (sipResidue *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipResidue *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Residue[] = {
	{sipName_BALL_clear, sipDo_Residue_clear, METH_VARARGS, NULL},
	{sipName_BALL_destroy, sipDo_Residue_destroy, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_Residue_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_Residue_get, METH_VARARGS, NULL},
	{sipName_BALL_swap, sipDo_Residue_swap, METH_VARARGS, NULL},
	{sipName_BALL_getFullName, sipDo_Residue_getFullName, METH_VARARGS, NULL},
	{sipName_BALL_hasTorsionPhi, sipDo_Residue_hasTorsionPhi, METH_VARARGS, NULL},
	{sipName_BALL_getTorsionPhi, sipDo_Residue_getTorsionPhi, METH_VARARGS, NULL},
	{sipName_BALL_hasTorsionPsi, sipDo_Residue_hasTorsionPsi, METH_VARARGS, NULL},
	{sipName_BALL_getTorsionPsi, sipDo_Residue_getTorsionPsi, METH_VARARGS, NULL},
	{sipName_BALL_getProtein, sipDo_Residue_getProtein, METH_VARARGS, NULL},
	{sipName_BALL_getChain, sipDo_Residue_getChain, METH_VARARGS, NULL},
	{sipName_BALL_getPDBAtom, sipDo_Residue_getPDBAtom, METH_VARARGS, NULL},
	{sipName_BALL_setID, sipDo_Residue_setID, METH_VARARGS, NULL},
	{sipName_BALL_getID, sipDo_Residue_getID, METH_VARARGS, NULL},
	{sipName_BALL_setInsertionCode, sipDo_Residue_setInsertionCode, METH_VARARGS, NULL},
	{sipName_BALL_getInsertionCode, sipDo_Residue_getInsertionCode, METH_VARARGS, NULL},
	{sipName_BALL_countPDBAtoms, sipDo_Residue_countPDBAtoms, METH_VARARGS, NULL},
	{sipName_BALL_prepend, sipDo_Residue_prepend, METH_VARARGS, NULL},
	{sipName_BALL_append, sipDo_Residue_append, METH_VARARGS, NULL},
	{sipName_BALL_insert, sipDo_Residue_insert, METH_VARARGS, NULL},
	{sipName_BALL_insertBefore, sipDo_Residue_insertBefore, METH_VARARGS, NULL},
	{sipName_BALL_insertAfter, sipDo_Residue_insertAfter, METH_VARARGS, NULL},
	{sipName_BALL_remove, sipDo_Residue_remove, METH_VARARGS, NULL},
	{sipName_BALL_spliceBefore, sipDo_Residue_spliceBefore, METH_VARARGS, NULL},
	{sipName_BALL_spliceAfter, sipDo_Residue_spliceAfter, METH_VARARGS, NULL},
	{sipName_BALL_splice, sipDo_Residue_splice, METH_VARARGS, NULL},
	{sipName_BALL_isAminoAcid, sipDo_Residue_isAminoAcid, METH_VARARGS, NULL},
	{sipName_BALL_isTerminal, sipDo_Residue_isTerminal, METH_VARARGS, NULL},
	{sipName_BALL_isNTerminal, sipDo_Residue_isNTerminal, METH_VARARGS, NULL},
	{sipName_BALL_isCTerminal, sipDo_Residue_isCTerminal, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_Residue_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_Residue(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Residue);
}

void sipConvertTo_Residue(PyObject *sipPy,Residue **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_Residue);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Residue *)sipConvertToCpp(sipPy,sipClass_Residue,sipIsErr);
}

Residue *sipForceConvertTo_Residue(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Residue(valobj))
	{
		Residue *val;

		sipConvertTo_Residue(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Residue);

	*iserrp = 1;

	return NULL;
}
