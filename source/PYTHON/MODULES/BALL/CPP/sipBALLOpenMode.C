#include "sipBALLDeclBALL.h"
#include "sipBALLOpenMode.h"


PyObject *sipClass_OpenMode;

static void sipDealloc_OpenMode(sipThisType *);
extern "C" PyObject * sip__str__OpenMode(PyObject *a0);

static PyTypeObject sipType_OpenMode = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_OpenMode,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_OpenMode,
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
	sip__str__OpenMode,
	0,
	0,
	0,
	Py_TPFLAGS_DEFAULT,
	0,
	0,
	0,
};

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_OpenMode(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_OpenMode)
		return ptr;

	return NULL;
}

static void sipDealloc_OpenMode(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (OpenMode *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}
extern "C" PyObject * sip__str__OpenMode(PyObject *a0)
{
#line 106 "file.sip"
  OpenMode* ptr;
  if ((ptr = (OpenMode*)sipGetCppPtr((sipThisType*)a0,sipClass_OpenMode)) == NULL)
    return NULL;
	
	String mode_string;
	switch ((int)*ptr)
	{
		case (int)File::APP: 		mode_string = "File::APP"; break;
		case (int)File::BINARY:	mode_string = "File::BINARY"; break;
		case (int)File::ATE: 		mode_string = "File::ATE"; break;
		case (int)File::TRUNC: 	mode_string = "File::TRUNC"; break;
		case (int)File::OUT: 		mode_string = "File::OUT"; break;
		case (int)File::IN: 			mode_string = "File::IN"; break;
		default:						mode_string = *ptr;
	}

  return PyString_FromString(mode_string.c_str());
#line 82 "sipBALLOpenMode.cpp"
}

PyObject *sipNew_OpenMode(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_OpenMode
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
			sipNew = new OpenMode();
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
		const OpenMode * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_OpenMode,&a0obj))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_OpenMode(a0obj,(OpenMode **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new OpenMode(* a0);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_OpenMode);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_OpenMode,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (OpenMode *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_OpenMode[] = {
	{NULL}
};

int sipCanConvertTo_OpenMode(PyObject *sipPy)
{
#line 125 "file.sip"
  // automatic conversion of Py integers to Position
  return (PyInt_Check(sipPy) || sipIsSubClassInstance(sipPy, sipClass_OpenMode));
#line 174 "sipBALLOpenMode.cpp"
}

int sipConvertTo_OpenMode(PyObject *sipPy,OpenMode **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return 0;

#line 129 "file.sip"
  if (PyInt_Check(sipPy))
  {
		int mode = PyInt_AS_LONG(sipPy);
		File::OpenMode result;
		switch (mode)
		{
			case File::OUT:		result = File::OUT; break;
			case File::BINARY:	result = File::BINARY; break;
			case File::ATE:		result = File::ATE; break;
			case File::TRUNC:	result = File::TRUNC; break;
			case File::APP:		result = File::APP; break;

			case File::IN:
			default: result = File::IN;
		};
    *sipCppPtr = new OpenMode(result);

    return 1;
  }

  *sipCppPtr = (OpenMode*)sipConvertToCpp(sipPy, sipClass_OpenMode, sipIsErr);

  return 0;
#line 206 "sipBALLOpenMode.cpp"
}

OpenMode *sipForceConvertTo_OpenMode(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_OpenMode(valobj))
	{
		OpenMode *val;

		sipConvertTo_OpenMode(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_OpenMode);

	*iserrp = 1;

	return NULL;
}
