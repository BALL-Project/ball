#include "sipBALLDeclBALL.h"
#include "sipBALLForceField.h"


PyObject *sipClass_ForceField;

static void sipDealloc_ForceField(sipThisType *);

static PyTypeObject sipType_ForceField = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_ForceField,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_ForceField,
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

sipForceField::sipForceField()
   : ForceField()
{
	sipCommonCtor(sipPyMethods,1);
}

sipForceField::sipForceField(System& a0)
   : ForceField(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipForceField::sipForceField(System& a0,Options& a1)
   : ForceField(a0,a1)
{
	sipCommonCtor(sipPyMethods,1);
}

sipForceField::sipForceField(const ForceField& a0)
   : ForceField(a0)
{
	sipCommonCtor(sipPyMethods,1);
}

sipForceField::~sipForceField()

{
	sipCommonDtor(sipPyThis);
}

bool sipForceField::specificSetup()

{
	int relLock;

	return sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_specificSetup,&relLock) ?
		sipForceField::sipVH_specificSetup(&sipPyMethods[0],sipPyThis,relLock) :
		ForceField::specificSetup();
}

// The common handler for all classes that inherit this virtual member
// function.

bool sipForceField::sipVH_specificSetup(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
{
	bool res;
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(O)",sipThis -> sipSelf);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		res = (bool)PyInt_AsLong(resobj);

		Py_DECREF(resobj);

		if (PyErr_Occurred() == NULL)
		{
			goto releaseLock;
		}

		sipBadVirtualResultType(sipName_BALL_ForceField,sipName_BALL_specificSetup);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);

	return res;
}

static PyObject *sipDo_ForceField_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceField)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

   try
   {
			res = ptr -> ForceField::isValid();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceField,sipName_BALL_isValid);

	return NULL;
}

static PyObject *sipDo_ForceField_setup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceField)) == NULL)
		return NULL;

	{
		System * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_System,&a0obj))
		{
			bool res;
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_System(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> ForceField::setup(* a0);
   }
   catch (FileNotFound e)
   {
      FileNotFound *my_exception = new FileNotFound(e);
      PyErr_SetObject(sipClass_FileNotFound, sipNewCppToSelf(my_exception,sipClass_FileNotFound,SIP_SIMPLE | SIP_PY_OWNED));
      return NULL;
		}

			return sipConvertFromBool((int)res);
		}
	}

	{
		System * a0;
		PyObject *a0obj;
		Options * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_System,&a0obj,sipCanConvertTo_Options,&a1obj))
		{
			bool res;
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_System(a0obj,&a0,1,&iserr);
			sipConvertTo_Options(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> ForceField::setup(* a0,* a1);
   }
   catch (FileNotFound e)
   {
      FileNotFound *my_exception = new FileNotFound(e);
      PyErr_SetObject(sipClass_FileNotFound, sipNewCppToSelf(my_exception,sipClass_FileNotFound,SIP_SIMPLE | SIP_PY_OWNED));
      return NULL;
		}

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceField,sipName_BALL_setup);

	return NULL;
}

static PyObject *sipDo_ForceField_specificSetup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceField)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

   try
   {
			res = ptr -> ForceField::specificSetup();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceField,sipName_BALL_specificSetup);

	return NULL;
}

static PyObject *sipDo_ForceField_setName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceField)) == NULL)
		return NULL;

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> ForceField::setName(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceField,sipName_BALL_setName);

	return NULL;
}

static PyObject *sipDo_ForceField_getName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceField)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			String *res;
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

   try
   {
			res = new String(ptr -> ForceField::getName());
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceField,sipName_BALL_getName);

	return NULL;
}

static PyObject *sipDo_ForceField_getNumberOfAtoms(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceField)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

   try
   {
			res = ptr -> ForceField::getNumberOfAtoms();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceField,sipName_BALL_getNumberOfAtoms);

	return NULL;
}

static PyObject *sipDo_ForceField_getNumberOfMovableAtoms(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceField)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

   try
   {
			res = ptr -> ForceField::getNumberOfMovableAtoms();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceField,sipName_BALL_getNumberOfMovableAtoms);

	return NULL;
}

static PyObject *sipDo_ForceField_getAtoms(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceField)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const AtomVector *res;
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

   try
   {
			res = &ptr -> ForceField::getAtoms();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_AtomVector);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceField,sipName_BALL_getAtoms);

	return NULL;
}

static PyObject *sipDo_ForceField_getSystem(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceField)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			System *res;
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

   try
   {
			res = ptr -> ForceField::getSystem();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_System);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceField,sipName_BALL_getSystem);

	return NULL;
}

static PyObject *sipDo_ForceField_getUseSelection(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceField)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

   try
   {
			res = ptr -> ForceField::getUseSelection();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceField,sipName_BALL_getUseSelection);

	return NULL;
}

static PyObject *sipDo_ForceField_setUseSelection(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceField)) == NULL)
		return NULL;

	{
		long a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"l",&a0))
		{
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

   try
   {
			ptr -> ForceField::setUseSelection( (bool)a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceField,sipName_BALL_setUseSelection);

	return NULL;
}

static PyObject *sipDo_ForceField_countComponents(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceField)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

   try
   {
			res = ptr -> ForceField::countComponents();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceField,sipName_BALL_countComponents);

	return NULL;
}

static PyObject *sipDo_ForceField_insertComponent(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceField)) == NULL)
		return NULL;

	{
		ForceFieldComponent * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_ForceFieldComponent,&a0obj))
		{
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_ForceFieldComponent(a0obj,&a0,0,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> ForceField::insertComponent( a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceField,sipName_BALL_insertComponent);

	return NULL;
}

static PyObject *sipDo_ForceField_removeComponent(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceField)) == NULL)
		return NULL;

	{
		const ForceFieldComponent * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_ForceFieldComponent,&a0obj))
		{
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_ForceFieldComponent(a0obj,(ForceFieldComponent **)&a0,0,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> ForceField::removeComponent( a0);
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
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> ForceField::removeComponent(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceField,sipName_BALL_removeComponent);

	return NULL;
}

static PyObject *sipDo_ForceField_getComponent(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceField)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"i",&a0))
		{
			ForceFieldComponent *res;
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

   try
   {
			res = ptr -> ForceField::getComponent( a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return sipMapCppToSelf(res,sipClass_ForceFieldComponent);
		}
	}

	{
		String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			ForceFieldComponent *res;
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> ForceField::getComponent(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_ForceFieldComponent);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceField,sipName_BALL_getComponent);

	return NULL;
}

static PyObject *sipDo_ForceField_getEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceField)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

   try
   {
			res = ptr -> ForceField::getEnergy();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceField,sipName_BALL_getEnergy);

	return NULL;
}

static PyObject *sipDo_ForceField_updateEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceField)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

   try
   {
			res = ptr -> ForceField::updateEnergy();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceField,sipName_BALL_updateEnergy);

	return NULL;
}

static PyObject *sipDo_ForceField_updateForces(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceField)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

   try
   {
			ptr -> ForceField::updateForces();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceField,sipName_BALL_updateForces);

	return NULL;
}

static PyObject *sipDo_ForceField_getRMSGradient(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceField)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

   try
   {
			res = ptr -> ForceField::getRMSGradient();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceField,sipName_BALL_getRMSGradient);

	return NULL;
}

static PyObject *sipDo_ForceField_getUpdateFrequency(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceField)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

   try
   {
			res = ptr -> ForceField::getUpdateFrequency();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceField,sipName_BALL_getUpdateFrequency);

	return NULL;
}

static PyObject *sipDo_ForceField_update(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ForceField)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			ForceField *ptr;

			if ((ptr = (ForceField *)sipGetCppPtr(sipThis,sipClass_ForceField)) == NULL)
				return NULL;

   try
   {
			ptr -> ForceField::update();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ForceField,sipName_BALL_update);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_ForceField(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_ForceField)
		return ptr;

	if ((res = sipCast_FileNotFound((FileNotFound *)(ForceField *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_FileNotFound((FileNotFound *)(ForceField *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_ForceField(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipForceField *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (ForceField *)sipThis -> u.cppPtr;
			else
				delete (sipForceField *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_ForceField(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_ForceField
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
			sipNew = new sipForceField();
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
		System * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_System,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_System(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipForceField(* a0);
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
		System * a0;
		PyObject *a0obj;
		Options * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-II",sipCanConvertTo_System,&a0obj,sipCanConvertTo_Options,&a1obj))
		{
			int iserr = 0;

			sipConvertTo_System(a0obj,&a0,1,&iserr);
			sipConvertTo_Options(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipForceField(* a0,* a1);
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
		const ForceField * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_ForceField,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_ForceField(a0obj,(ForceField **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipForceField(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_ForceField);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_ForceField,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (ForceField *)sipNew;
			else
				delete (sipForceField *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipForceField *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_ForceField_periodic_boundary(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	PeriodicBoundary *val;
	ForceField *ptr;

	if ((ptr = (ForceField *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_ForceField)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = &ptr -> periodic_boundary;

		valobj = sipMapCppToSelf(val,sipClass_PeriodicBoundary);

		return valobj;
	}

	val = sipForceConvertTo_PeriodicBoundary(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_ForceField,sipName_BALL_periodic_boundary);
		return NULL;
	}

	ptr -> periodic_boundary = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *sipGetSetVar_ForceField_options(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Options *val;
	ForceField *ptr;

	if ((ptr = (ForceField *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_ForceField)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = &ptr -> options;

		valobj = sipMapCppToSelf(val,sipClass_Options);

		return valobj;
	}

	val = sipForceConvertTo_Options(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_ForceField,sipName_BALL_options);
		return NULL;
	}

	ptr -> options = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_ForceField[] = {
	{sipName_BALL_isValid, sipDo_ForceField_isValid, METH_VARARGS, NULL},
	{sipName_BALL_setup, sipDo_ForceField_setup, METH_VARARGS, NULL},
	{sipName_BALL_specificSetup, sipDo_ForceField_specificSetup, METH_VARARGS, NULL},
	{sipName_BALL_setName, sipDo_ForceField_setName, METH_VARARGS, NULL},
	{sipName_BALL_getName, sipDo_ForceField_getName, METH_VARARGS, NULL},
	{sipName_BALL_getNumberOfAtoms, sipDo_ForceField_getNumberOfAtoms, METH_VARARGS, NULL},
	{sipName_BALL_getNumberOfMovableAtoms, sipDo_ForceField_getNumberOfMovableAtoms, METH_VARARGS, NULL},
	{sipName_BALL_getAtoms, sipDo_ForceField_getAtoms, METH_VARARGS, NULL},
	{sipName_BALL_getSystem, sipDo_ForceField_getSystem, METH_VARARGS, NULL},
	{sipName_BALL_getUseSelection, sipDo_ForceField_getUseSelection, METH_VARARGS, NULL},
	{sipName_BALL_setUseSelection, sipDo_ForceField_setUseSelection, METH_VARARGS, NULL},
	{sipName_BALL_countComponents, sipDo_ForceField_countComponents, METH_VARARGS, NULL},
	{sipName_BALL_insertComponent, sipDo_ForceField_insertComponent, METH_VARARGS, NULL},
	{sipName_BALL_removeComponent, sipDo_ForceField_removeComponent, METH_VARARGS, NULL},
	{sipName_BALL_getComponent, sipDo_ForceField_getComponent, METH_VARARGS, NULL},
	{sipName_BALL_getEnergy, sipDo_ForceField_getEnergy, METH_VARARGS, NULL},
	{sipName_BALL_updateEnergy, sipDo_ForceField_updateEnergy, METH_VARARGS, NULL},
	{sipName_BALL_updateForces, sipDo_ForceField_updateForces, METH_VARARGS, NULL},
	{sipName_BALL_getRMSGradient, sipDo_ForceField_getRMSGradient, METH_VARARGS, NULL},
	{sipName_BALL_getUpdateFrequency, sipDo_ForceField_getUpdateFrequency, METH_VARARGS, NULL},
	{sipName_BALL_update, sipDo_ForceField_update, METH_VARARGS, NULL},
	{NULL}
};

PyMethodDef sipClassVarTab_ForceField[] = {
	{sipName_BALL_periodic_boundary, sipGetSetVar_ForceField_periodic_boundary, 0, NULL},
	{sipName_BALL_options, sipGetSetVar_ForceField_options, 0, NULL},
	{NULL}
};

PyMethodDef *sipClassVarHierTab_ForceField[] = {
	sipClassVarTab_ForceField,
	NULL
};

int sipCanConvertTo_ForceField(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_ForceField);
}

void sipConvertTo_ForceField(PyObject *sipPy,ForceField **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_ForceField);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (ForceField *)sipConvertToCpp(sipPy,sipClass_ForceField,sipIsErr);
}

ForceField *sipForceConvertTo_ForceField(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_ForceField(valobj))
	{
		ForceField *val;

		sipConvertTo_ForceField(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_ForceField);

	*iserrp = 1;

	return NULL;
}
