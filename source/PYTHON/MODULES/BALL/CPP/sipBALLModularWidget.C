#include "sipBALLDeclBALL.h"
#include "sipBALLModularWidget.h"


PyObject *sipClass_ModularWidget;

static void sipDealloc_ModularWidget(sipThisType *);

static PyTypeObject sipType_ModularWidget = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_ModularWidget,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_ModularWidget,
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

sipModularWidget::sipModularWidget()
   : ModularWidget()
{
	sipCommonCtor(sipPyMethods,4);
}

sipModularWidget::sipModularWidget(const ModularWidget& a0)
   : ModularWidget(a0)
{
	sipCommonCtor(sipPyMethods,4);
}

sipModularWidget::~sipModularWidget()
 throw()
{
	sipCommonDtor(sipPyThis);
}

void sipModularWidget::registerThis()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_registerThis,&relLock))
		sipEmbeddable::sipVH_registerThis(&sipPyMethods[0],sipPyThis,relLock);
	else
		ModularWidget::registerThis();
}

void sipModularWidget::checkMenu(MainControl& a0)
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_checkMenu,&relLock))
		sipModularWidget::sipVH_checkMenu(&sipPyMethods[1],sipPyThis,relLock,a0);
	else
		ModularWidget::checkMenu(a0);
}

void sipModularWidget::finalizeWidget(MainControl& a0)

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_finalizeWidget,&relLock))
		sipModularWidget::sipVH_finalizeWidget(&sipPyMethods[2],sipPyThis,relLock,a0);
	else
		ModularWidget::finalizeWidget(a0);
}

void sipModularWidget::initializeWidget(MainControl& a0)

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_initializeWidget,&relLock))
		sipModularWidget::sipVH_initializeWidget(&sipPyMethods[3],sipPyThis,relLock,a0);
	else
		ModularWidget::initializeWidget(a0);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipModularWidget::sipVH_checkMenu(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,MainControl& a0)
{
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_MainControl);

	sipArgs = Py_BuildValue("(ON)",sipThis -> sipSelf,a0obj);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_ModularWidget,sipName_BALL_checkMenu);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipModularWidget::sipVH_finalizeWidget(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,MainControl& a0)
{
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_MainControl);

	sipArgs = Py_BuildValue("(ON)",sipThis -> sipSelf,a0obj);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_ModularWidget,sipName_BALL_finalizeWidget);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipModularWidget::sipVH_initializeWidget(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,MainControl& a0)
{
	PyObject *resobj;
	PyObject *sipArgs;
	PyObject *a0obj;

	a0obj = sipMapCppToSelf(&a0,sipClass_MainControl);

	sipArgs = Py_BuildValue("(ON)",sipThis -> sipSelf,a0obj);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_ModularWidget,sipName_BALL_initializeWidget);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

static PyObject *sipDo_ModularWidget_registerThis(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ModularWidget)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			ModularWidget *ptr;

			if ((ptr = (ModularWidget *)sipGetCppPtr(sipThis,sipClass_ModularWidget)) == NULL)
				return NULL;

			ptr -> ModularWidget::registerThis();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ModularWidget,sipName_BALL_registerThis);

	return NULL;
}

static PyObject *sipDo_ModularWidget_unregisterThis(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ModularWidget)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			ModularWidget *ptr;

			if ((ptr = (ModularWidget *)sipGetCppPtr(sipThis,sipClass_ModularWidget)) == NULL)
				return NULL;

			ptr -> ModularWidget::unregisterThis();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ModularWidget,sipName_BALL_unregisterThis);

	return NULL;
}

static PyObject *sipDo_ModularWidget_setIdentifier(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ModularWidget)) == NULL)
		return NULL;

	{
		const String * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			ModularWidget *ptr;

			if ((ptr = (ModularWidget *)sipGetCppPtr(sipThis,sipClass_ModularWidget)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> ModularWidget::setIdentifier(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ModularWidget,sipName_BALL_setIdentifier);

	return NULL;
}

static PyObject *sipDo_ModularWidget_getIdentifier(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ModularWidget)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			const String *res;
			ModularWidget *ptr;

			if ((ptr = (ModularWidget *)sipGetCppPtr(sipThis,sipClass_ModularWidget)) == NULL)
				return NULL;

   try
   {
			res = &ptr -> ModularWidget::getIdentifier();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ModularWidget,sipName_BALL_getIdentifier);

	return NULL;
}

static PyObject *sipDo_ModularWidget_registerWidget(PyObject *,PyObject *sipArgs)
{
	int sipArgsParsed = 0;

	{
		ModularWidget * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_ModularWidget,&a0obj))
		{

			int iserr = 0;

			sipConvertTo_ModularWidget(a0obj,&a0,0,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ModularWidget::registerWidget( a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ModularWidget,sipName_BALL_registerWidget);

	return NULL;
}

static PyObject *sipDo_ModularWidget_initializeWidget(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ModularWidget)) == NULL)
		return NULL;

	{
		MainControl * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_MainControl,&a0obj))
		{
			ModularWidget *ptr;

			if ((ptr = (ModularWidget *)sipGetCppPtr(sipThis,sipClass_ModularWidget)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_MainControl(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> ModularWidget::initializeWidget(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ModularWidget,sipName_BALL_initializeWidget);

	return NULL;
}

static PyObject *sipDo_ModularWidget_finalizeWidget(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ModularWidget)) == NULL)
		return NULL;

	{
		MainControl * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_MainControl,&a0obj))
		{
			ModularWidget *ptr;

			if ((ptr = (ModularWidget *)sipGetCppPtr(sipThis,sipClass_ModularWidget)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_MainControl(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			ptr -> ModularWidget::finalizeWidget(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_ModularWidget,sipName_BALL_finalizeWidget);

	return NULL;
}

static PyObject *sipDo_ModularWidget_checkMenu(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_ModularWidget)) == NULL)
		return NULL;

	{
		MainControl * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_MainControl,&a0obj))
		{
			ModularWidget *ptr;

			if ((ptr = (ModularWidget *)sipGetCppPtr(sipThis,sipClass_ModularWidget)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_MainControl(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> ModularWidget::checkMenu(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_ModularWidget,sipName_BALL_checkMenu);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_ModularWidget(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_ModularWidget)
		return ptr;

	if ((res = sipCast_Embeddable((Embeddable *)(ModularWidget *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_ModularWidget(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipModularWidget *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (ModularWidget *)sipThis -> u.cppPtr;
			else
				delete (sipModularWidget *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_ModularWidget(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_ModularWidget
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
			sipNew = new sipModularWidget();
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
		const ModularWidget * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_ModularWidget,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_ModularWidget(a0obj,(ModularWidget **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new sipModularWidget(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_ModularWidget);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_ModularWidget,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (ModularWidget *)sipNew;
			else
				delete (sipModularWidget *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipModularWidget *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_ModularWidget[] = {
	{sipName_BALL_registerThis, sipDo_ModularWidget_registerThis, METH_VARARGS, NULL},
	{sipName_BALL_unregisterThis, sipDo_ModularWidget_unregisterThis, METH_VARARGS, NULL},
	{sipName_BALL_setIdentifier, sipDo_ModularWidget_setIdentifier, METH_VARARGS, NULL},
	{sipName_BALL_getIdentifier, sipDo_ModularWidget_getIdentifier, METH_VARARGS, NULL},
	{sipName_BALL_registerWidget, sipDo_ModularWidget_registerWidget, METH_VARARGS, NULL},
	{sipName_BALL_initializeWidget, sipDo_ModularWidget_initializeWidget, METH_VARARGS, NULL},
	{sipName_BALL_finalizeWidget, sipDo_ModularWidget_finalizeWidget, METH_VARARGS, NULL},
	{sipName_BALL_checkMenu, sipDo_ModularWidget_checkMenu, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_ModularWidget(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_ModularWidget);
}

void sipConvertTo_ModularWidget(PyObject *sipPy,ModularWidget **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_ModularWidget);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (ModularWidget *)sipConvertToCpp(sipPy,sipClass_ModularWidget,sipIsErr);
}

ModularWidget *sipForceConvertTo_ModularWidget(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_ModularWidget(valobj))
	{
		ModularWidget *val;

		sipConvertTo_ModularWidget(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_ModularWidget);

	*iserrp = 1;

	return NULL;
}
