#include "sipBALLDeclBALL.h"
#include "sipBALLPyWidget.h"



PyObject *sipClass_PyWidget;

static void sipDealloc_PyWidget(sipThisType *);

static PyTypeObject sipType_PyWidget = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_PyWidget,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_PyWidget,
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

sipPyWidget::sipPyWidget(const PyWidget& a0)
    : PyWidget(a0)
{
	sipCommonCtor(sipPyMethods,12);
}

sipPyWidget::~sipPyWidget()
  throw()
{
	sipCommonDtor(sipPyThis);
}

void sipPyWidget::registerThis()
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_registerThis,&relLock))
		sipEmbeddable::sipVH_registerThis(&sipPyMethods[0],sipPyThis,relLock);
	else
		ModularWidget::registerThis();
}

void sipPyWidget::checkMenu(MainControl& a0)
 throw()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_checkMenu,&relLock))
		sipModularWidget::sipVH_checkMenu(&sipPyMethods[1],sipPyThis,relLock,a0);
	else
		ModularWidget::checkMenu(a0);
}

void sipPyWidget::cursorRight(bool a0,bool a1)

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_cursorRight,&relLock))
		sipPyWidget::sipVH_cursorRight(&sipPyMethods[2],sipPyThis,relLock,a0,a1);
	else
		PyWidget::cursorRight(a0,a1);
}

void sipPyWidget::backspace()

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_backspace,&relLock))
		sipPyWidget::sipVH_backspace(&sipPyMethods[3],sipPyThis,relLock);
	else
		PyWidget::backspace();
}

void sipPyWidget::cursorLeft(bool a0,bool a1)

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_cursorLeft,&relLock))
		sipPyWidget::sipVH_cursorLeft(&sipPyMethods[4],sipPyThis,relLock,a0,a1);
	else
		PyWidget::cursorLeft(a0,a1);
}

void sipPyWidget::newLine()

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[5],sipPyThis,NULL,sipName_BALL_newLine,&relLock))
		sipPyWidget::sipVH_newLine(&sipPyMethods[5],sipPyThis,relLock);
	else
		PyWidget::newLine();
}

void sipPyWidget::cursorDown(bool a0)

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[6],sipPyThis,NULL,sipName_BALL_cursorDown,&relLock))
		sipPyWidget::sipVH_cursorDown(&sipPyMethods[6],sipPyThis,relLock,a0);
	else
		PyWidget::cursorDown(a0);
}

void sipPyWidget::cursorUp(bool a0)

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[7],sipPyThis,NULL,sipName_BALL_cursorUp,&relLock))
		sipPyWidget::sipVH_cursorUp(&sipPyMethods[7],sipPyThis,relLock,a0);
	else
		PyWidget::cursorUp(a0);
}

void sipPyWidget::finalizeWidget(MainControl& a0)

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[8],sipPyThis,NULL,sipName_BALL_finalizeWidget,&relLock))
		sipModularWidget::sipVH_finalizeWidget(&sipPyMethods[8],sipPyThis,relLock,a0);
	else
		PyWidget::finalizeWidget(a0);
}

void sipPyWidget::initializeWidget(MainControl& a0)

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[9],sipPyThis,NULL,sipName_BALL_initializeWidget,&relLock))
		sipModularWidget::sipVH_initializeWidget(&sipPyMethods[9],sipPyThis,relLock,a0);
	else
		PyWidget::initializeWidget(a0);
}

void sipPyWidget::stopInterpreter()

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[10],sipPyThis,NULL,sipName_BALL_stopInterpreter,&relLock))
		sipPyWidget::sipVH_stopInterpreter(&sipPyMethods[10],sipPyThis,relLock);
	else
		PyWidget::stopInterpreter();
}

void sipPyWidget::startInterpreter()

{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[11],sipPyThis,NULL,sipName_BALL_startInterpreter,&relLock))
		sipPyWidget::sipVH_startInterpreter(&sipPyMethods[11],sipPyThis,relLock);
	else
		PyWidget::startInterpreter();
}

// The common handler for all classes that inherit this virtual member
// function.

void sipPyWidget::sipVH_cursorRight(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,bool a0,bool a1)
{
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(Oii)",sipThis -> sipSelf,a0,a1);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_PyWidget,sipName_BALL_cursorRight);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipPyWidget::sipVH_backspace(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_PyWidget,sipName_BALL_backspace);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipPyWidget::sipVH_cursorLeft(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,bool a0,bool a1)
{
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(Oii)",sipThis -> sipSelf,a0,a1);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_PyWidget,sipName_BALL_cursorLeft);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipPyWidget::sipVH_newLine(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_PyWidget,sipName_BALL_newLine);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipPyWidget::sipVH_cursorDown(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,bool a0)
{
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(Oi)",sipThis -> sipSelf,a0);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_PyWidget,sipName_BALL_cursorDown);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipPyWidget::sipVH_cursorUp(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock,bool a0)
{
	PyObject *resobj;
	PyObject *sipArgs;

	sipArgs = Py_BuildValue("(Oi)",sipThis -> sipSelf,a0);

	if (sipArgs == NULL)
		goto reportError;

	resobj = sipEvalMethod(&pymc -> pyMethod,sipArgs);

	Py_DECREF(sipArgs);

	if (resobj != NULL)
	{
		Py_DECREF(resobj);

		if (resobj == Py_None)
			goto releaseLock;

		sipBadVirtualResultType(sipName_BALL_PyWidget,sipName_BALL_cursorUp);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipPyWidget::sipVH_stopInterpreter(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_PyWidget,sipName_BALL_stopInterpreter);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

// The common handler for all classes that inherit this virtual member
// function.

void sipPyWidget::sipVH_startInterpreter(const sipMethodCache *pymc,sipThisType *sipThis,int sipRelLock)
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

		sipBadVirtualResultType(sipName_BALL_PyWidget,sipName_BALL_startInterpreter);
	}

reportError:
	PyErr_Print();

releaseLock:
	sipCondReleaseLock(sipRelLock);
}

static PyObject *sipDo_PyWidget_countInstances(PyObject *,PyObject *sipArgs)
{
	int sipArgsParsed = 0;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,"-"))
		{
			int res;

			res = PyWidget::countInstances();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PyWidget,sipName_BALL_countInstances);

	return NULL;
}

static PyObject *sipDo_PyWidget_getInstance(PyObject *,PyObject *sipArgs)
{
	int sipArgsParsed = 0;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_String,&a0obj))
		{
			PyWidget *res;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = PyWidget::getInstance(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_PyWidget);
		}
	}

	{
		Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_Position,&a0obj))
		{
			PyWidget *res;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = PyWidget::getInstance(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_PyWidget);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PyWidget,sipName_BALL_getInstance);

	return NULL;
}

static PyObject *sipDo_PyWidget_startInterpreter(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PyWidget)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			PyWidget *ptr;

			if ((ptr = (PyWidget *)sipGetCppPtr(sipThis,sipClass_PyWidget)) == NULL)
				return NULL;

			ptr -> PyWidget::startInterpreter();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PyWidget,sipName_BALL_startInterpreter);

	return NULL;
}

static PyObject *sipDo_PyWidget_stopInterpreter(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PyWidget)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			PyWidget *ptr;

			if ((ptr = (PyWidget *)sipGetCppPtr(sipThis,sipClass_PyWidget)) == NULL)
				return NULL;

			ptr -> PyWidget::stopInterpreter();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PyWidget,sipName_BALL_stopInterpreter);

	return NULL;
}

static PyObject *sipDo_PyWidget_initializeWidget(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PyWidget)) == NULL)
		return NULL;

	{
		MainControl *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_MainControl,&a0obj))
		{
			PyWidget *ptr;

			if ((ptr = (PyWidget *)sipGetCppPtr(sipThis,sipClass_PyWidget)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_MainControl(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> PyWidget::initializeWidget(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PyWidget,sipName_BALL_initializeWidget);

	return NULL;
}

static PyObject *sipDo_PyWidget_finalizeWidget(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PyWidget)) == NULL)
		return NULL;

	{
		MainControl *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_MainControl,&a0obj))
		{
			PyWidget *ptr;

			if ((ptr = (PyWidget *)sipGetCppPtr(sipThis,sipClass_PyWidget)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_MainControl(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> PyWidget::finalizeWidget(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PyWidget,sipName_BALL_finalizeWidget);

	return NULL;
}

static PyObject *sipDo_PyWidget_cursorUp(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PyWidget)) == NULL)
		return NULL;

	{
		long a0 = false;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"|l",&a0))
		{
			PyWidget *ptr;

			if ((ptr = (PyWidget *)sipGetCppPtr(sipThis,sipClass_PyWidget)) == NULL)
				return NULL;

			ptr -> PyWidget::cursorUp( (bool)a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PyWidget,sipName_BALL_cursorUp);

	return NULL;
}

static PyObject *sipDo_PyWidget_cursorDown(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PyWidget)) == NULL)
		return NULL;

	{
		long a0 = false;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"|l",&a0))
		{
			PyWidget *ptr;

			if ((ptr = (PyWidget *)sipGetCppPtr(sipThis,sipClass_PyWidget)) == NULL)
				return NULL;

			ptr -> PyWidget::cursorDown( (bool)a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PyWidget,sipName_BALL_cursorDown);

	return NULL;
}

static PyObject *sipDo_PyWidget_newLine(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PyWidget)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			PyWidget *ptr;

			if ((ptr = (PyWidget *)sipGetCppPtr(sipThis,sipClass_PyWidget)) == NULL)
				return NULL;

			ptr -> PyWidget::newLine();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PyWidget,sipName_BALL_newLine);

	return NULL;
}

static PyObject *sipDo_PyWidget_cursorLeft(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PyWidget)) == NULL)
		return NULL;

	{
		long a0 = false;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"|ll",&a0,&a1))
		{
			PyWidget *ptr;

			if ((ptr = (PyWidget *)sipGetCppPtr(sipThis,sipClass_PyWidget)) == NULL)
				return NULL;

			ptr -> PyWidget::cursorLeft( (bool)a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PyWidget,sipName_BALL_cursorLeft);

	return NULL;
}

static PyObject *sipDo_PyWidget_backspace(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PyWidget)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			PyWidget *ptr;

			if ((ptr = (PyWidget *)sipGetCppPtr(sipThis,sipClass_PyWidget)) == NULL)
				return NULL;

			ptr -> PyWidget::backspace();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PyWidget,sipName_BALL_backspace);

	return NULL;
}

static PyObject *sipDo_PyWidget_cursorRight(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_PyWidget)) == NULL)
		return NULL;

	{
		long a0 = false;
		long a1 = true;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"|ll",&a0,&a1))
		{
			PyWidget *ptr;

			if ((ptr = (PyWidget *)sipGetCppPtr(sipThis,sipClass_PyWidget)) == NULL)
				return NULL;

			ptr -> PyWidget::cursorRight( (bool)a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_PyWidget,sipName_BALL_cursorRight);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_PyWidget(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_PyWidget)
		return ptr;

	if ((res = sipCast_ModularWidget((ModularWidget *)(PyWidget *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_PyWidget(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipPyWidget *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (PyWidget *)sipThis -> u.cppPtr;
			else
				delete (sipPyWidget *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_PyWidget(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_PyWidget
	};

	sipThisType *sipThis = NULL;
	const void *sipNew = NULL;
	int sipFlags = SIP_PY_OWNED;
	int sipArgsParsed = 0;

	// See if there is something pending.

	sipNew = sipGetPending(&sipFlags);

	if (sipNew == NULL)
	{
		const PyWidget *a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_PyWidget,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_PyWidget(a0obj,(PyWidget **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipPyWidget(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_PyWidget);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_PyWidget,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (PyWidget *)sipNew;
			else
				delete (sipPyWidget *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipPyWidget *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_PyWidget[] = {
	{sipName_BALL_countInstances, sipDo_PyWidget_countInstances, METH_VARARGS, NULL},
	{sipName_BALL_getInstance, sipDo_PyWidget_getInstance, METH_VARARGS, NULL},
	{sipName_BALL_startInterpreter, sipDo_PyWidget_startInterpreter, METH_VARARGS, NULL},
	{sipName_BALL_stopInterpreter, sipDo_PyWidget_stopInterpreter, METH_VARARGS, NULL},
	{sipName_BALL_initializeWidget, sipDo_PyWidget_initializeWidget, METH_VARARGS, NULL},
	{sipName_BALL_finalizeWidget, sipDo_PyWidget_finalizeWidget, METH_VARARGS, NULL},
	{sipName_BALL_cursorUp, sipDo_PyWidget_cursorUp, METH_VARARGS, NULL},
	{sipName_BALL_cursorDown, sipDo_PyWidget_cursorDown, METH_VARARGS, NULL},
	{sipName_BALL_newLine, sipDo_PyWidget_newLine, METH_VARARGS, NULL},
	{sipName_BALL_cursorLeft, sipDo_PyWidget_cursorLeft, METH_VARARGS, NULL},
	{sipName_BALL_backspace, sipDo_PyWidget_backspace, METH_VARARGS, NULL},
	{sipName_BALL_cursorRight, sipDo_PyWidget_cursorRight, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_PyWidget(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_PyWidget);
}

void sipConvertTo_PyWidget(PyObject *sipPy,PyWidget **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_PyWidget);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (PyWidget *)sipConvertToCpp(sipPy,sipClass_PyWidget,sipIsErr);
}

PyWidget *sipForceConvertTo_PyWidget(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_PyWidget(valobj))
	{
		PyWidget *val;

		sipConvertTo_PyWidget(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_PyWidget);
	*iserrp = 1;

	return NULL;
}
