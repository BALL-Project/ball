#include "sipBALLDeclBALL.h"
#include "sipBALLFDPB.h"


PyObject *sipClass_FDPB;

static void sipDealloc_FDPB(sipThisType *);

static PyTypeObject sipType_FDPB = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_FDPB,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_FDPB,
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

extern "C" PyObject *sipDo_FDPB_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FDPB)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			FDPB *ptr;

			if ((ptr = (FDPB *)sipGetCppPtr(sipThis,sipClass_FDPB)) == NULL)
				return NULL;

   try
   {
			ptr -> FDPB::destroy();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FDPB,sipName_BALL_destroy);

	return NULL;
}

extern "C" PyObject *sipDo_FDPB_destroyGrids(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FDPB)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			FDPB *ptr;

			if ((ptr = (FDPB *)sipGetCppPtr(sipThis,sipClass_FDPB)) == NULL)
				return NULL;

   try
   {
			ptr -> FDPB::destroyGrids();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FDPB,sipName_BALL_destroyGrids);

	return NULL;
}

extern "C" PyObject *sipDo_FDPB_setup(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FDPB)) == NULL)
		return NULL;

	{
		System * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_System,&a0obj))
		{
			bool res;
			FDPB *ptr;

			if ((ptr = (FDPB *)sipGetCppPtr(sipThis,sipClass_FDPB)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_System(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> FDPB::setup(* a0);
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
		System * a0;
		PyObject *a0obj;
		Options * a1;
		PyObject *a1obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"II",sipCanConvertTo_System,&a0obj,sipCanConvertTo_Options,&a1obj))
		{
			bool res;
			FDPB *ptr;

			if ((ptr = (FDPB *)sipGetCppPtr(sipThis,sipClass_FDPB)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_System(a0obj,&a0,1,&iserr);
			sipConvertTo_Options(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> FDPB::setup(* a0,* a1);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FDPB,sipName_BALL_setup);

	return NULL;
}

extern "C" PyObject *sipDo_FDPB_setupEpsGrid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FDPB)) == NULL)
		return NULL;

	{
		System * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_System,&a0obj))
		{
			bool res;
			FDPB *ptr;

			if ((ptr = (FDPB *)sipGetCppPtr(sipThis,sipClass_FDPB)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_System(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> FDPB::setupEpsGrid(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FDPB,sipName_BALL_setupEpsGrid);

	return NULL;
}

extern "C" PyObject *sipDo_FDPB_setupSASGrid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FDPB)) == NULL)
		return NULL;

	{
		System * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_System,&a0obj))
		{
			bool res;
			FDPB *ptr;

			if ((ptr = (FDPB *)sipGetCppPtr(sipThis,sipClass_FDPB)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_System(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> FDPB::setupSASGrid(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FDPB,sipName_BALL_setupSASGrid);

	return NULL;
}

extern "C" PyObject *sipDo_FDPB_setupAtomArray(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FDPB)) == NULL)
		return NULL;

	{
		System * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"I",sipCanConvertTo_System,&a0obj))
		{
			bool res;
			FDPB *ptr;

			if ((ptr = (FDPB *)sipGetCppPtr(sipThis,sipClass_FDPB)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_System(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = ptr -> FDPB::setupAtomArray(* a0);
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FDPB,sipName_BALL_setupAtomArray);

	return NULL;
}

extern "C" PyObject *sipDo_FDPB_setupKappaGrid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FDPB)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			FDPB *ptr;

			if ((ptr = (FDPB *)sipGetCppPtr(sipThis,sipClass_FDPB)) == NULL)
				return NULL;

   try
   {
			res = ptr -> FDPB::setupKappaGrid();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FDPB,sipName_BALL_setupKappaGrid);

	return NULL;
}

extern "C" PyObject *sipDo_FDPB_setupQGrid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FDPB)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			FDPB *ptr;

			if ((ptr = (FDPB *)sipGetCppPtr(sipThis,sipClass_FDPB)) == NULL)
				return NULL;

   try
   {
			res = ptr -> FDPB::setupQGrid();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FDPB,sipName_BALL_setupQGrid);

	return NULL;
}

extern "C" PyObject *sipDo_FDPB_setupPhiGrid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FDPB)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			FDPB *ptr;

			if ((ptr = (FDPB *)sipGetCppPtr(sipThis,sipClass_FDPB)) == NULL)
				return NULL;

   try
   {
			res = ptr -> FDPB::setupPhiGrid();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FDPB,sipName_BALL_setupPhiGrid);

	return NULL;
}

extern "C" PyObject *sipDo_FDPB_setupBoundary(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FDPB)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			FDPB *ptr;

			if ((ptr = (FDPB *)sipGetCppPtr(sipThis,sipClass_FDPB)) == NULL)
				return NULL;

   try
   {
			res = ptr -> FDPB::setupBoundary();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FDPB,sipName_BALL_setupBoundary);

	return NULL;
}

extern "C" PyObject *sipDo_FDPB_solve(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FDPB)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			bool res;
			FDPB *ptr;

			if ((ptr = (FDPB *)sipGetCppPtr(sipThis,sipClass_FDPB)) == NULL)
				return NULL;

   try
   {
			res = ptr -> FDPB::solve();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FDPB,sipName_BALL_solve);

	return NULL;
}

extern "C" PyObject *sipDo_FDPB_getEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FDPB)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			FDPB *ptr;

			if ((ptr = (FDPB *)sipGetCppPtr(sipThis,sipClass_FDPB)) == NULL)
				return NULL;

   try
   {
			res = ptr -> FDPB::getEnergy();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FDPB,sipName_BALL_getEnergy);

	return NULL;
}

extern "C" PyObject *sipDo_FDPB_getReactionFieldEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FDPB)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			FDPB *ptr;

			if ((ptr = (FDPB *)sipGetCppPtr(sipThis,sipClass_FDPB)) == NULL)
				return NULL;

   try
   {
			res = ptr -> FDPB::getReactionFieldEnergy();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FDPB,sipName_BALL_getReactionFieldEnergy);

	return NULL;
}

extern "C" PyObject *sipDo_FDPB_calculateReactionFieldEnergy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FDPB)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			double res;
			FDPB *ptr;

			if ((ptr = (FDPB *)sipGetCppPtr(sipThis,sipClass_FDPB)) == NULL)
				return NULL;

   try
   {
			res = ptr -> FDPB::calculateReactionFieldEnergy();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FDPB,sipName_BALL_calculateReactionFieldEnergy);

	return NULL;
}

extern "C" PyObject *sipDo_FDPB_getNumberOfIterations(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FDPB)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			int res;
			FDPB *ptr;

			if ((ptr = (FDPB *)sipGetCppPtr(sipThis,sipClass_FDPB)) == NULL)
				return NULL;

   try
   {
			res = ptr -> FDPB::getNumberOfIterations();
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

	sipNoMethod(sipArgsParsed,sipName_BALL_FDPB,sipName_BALL_getNumberOfIterations);

	return NULL;
}

extern "C" PyObject *sipDo_FDPB_getErrorCode(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;
	int sipArgsParsed = 0;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_FDPB)) == NULL)
		return NULL;

	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,""))
		{
			Index *res;
			FDPB *ptr;

			if ((ptr = (FDPB *)sipGetCppPtr(sipThis,sipClass_FDPB)) == NULL)
				return NULL;

   try
   {
			res = new Index(ptr -> FDPB::getErrorCode());
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			PyObject *resobj = sipConvertFrom_Index(res);
			delete res;

			return resobj;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FDPB,sipName_BALL_getErrorCode);

	return NULL;
}

extern "C" PyObject *sipDo_FDPB_getErrorMessage(PyObject *,PyObject *sipArgs)
{
	int sipArgsParsed = 0;

	{
		Index * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_Index,&a0obj))
		{
			String *res;

			int iserr = 0;

			int istemp0 = sipConvertTo_Index(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			res = new String(FDPB::getErrorMessage(* a0));
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}

			if (istemp0)
				delete a0;

			return sipNewCppToSelf(res,sipClass_String,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipArgsParsed,sipName_BALL_FDPB,sipName_BALL_getErrorMessage);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_FDPB(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_FDPB)
		return ptr;

	return NULL;
}

static void sipDealloc_FDPB(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (FDPB *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_FDPB(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_FDPB
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
			sipNew = new FDPB();
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
		const FDPB * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_FDPB,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_FDPB(a0obj,(FDPB **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new FDPB(* a0);
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
			sipNew = new FDPB(* a0);
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
		Options * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_Options,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Options(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new FDPB(* a0);
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
			sipNew = new FDPB(* a0,* a1);
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
		sipNoCtor(sipArgsParsed,sipName_BALL_FDPB);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_FDPB,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (FDPB *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_FDPB_phi_grid(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	RegularData3D *val;
	FDPB *ptr;

	if ((ptr = (FDPB *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_FDPB)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> phi_grid;

		valobj = sipMapCppToSelf(val,sipClass_RegularData3D);

		return valobj;
	}

	val = sipForceConvertTo_RegularData3D(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_FDPB,sipName_BALL_phi_grid);
		return NULL;
	}

	ptr -> phi_grid = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_FDPB_q_grid(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	RegularData3D *val;
	FDPB *ptr;

	if ((ptr = (FDPB *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_FDPB)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> q_grid;

		valobj = sipMapCppToSelf(val,sipClass_RegularData3D);

		return valobj;
	}

	val = sipForceConvertTo_RegularData3D(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_FDPB,sipName_BALL_q_grid);
		return NULL;
	}

	ptr -> q_grid = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_FDPB_kappa_grid(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	RegularData3D *val;
	FDPB *ptr;

	if ((ptr = (FDPB *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_FDPB)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> kappa_grid;

		valobj = sipMapCppToSelf(val,sipClass_RegularData3D);

		return valobj;
	}

	val = sipForceConvertTo_RegularData3D(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_FDPB,sipName_BALL_kappa_grid);
		return NULL;
	}

	ptr -> kappa_grid = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_FDPB_results(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Options *val;
	FDPB *ptr;

	if ((ptr = (FDPB *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_FDPB)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = &ptr -> results;

		valobj = sipMapCppToSelf(val,sipClass_Options);

		return valobj;
	}

	val = sipForceConvertTo_Options(valobj,&iserr);

	if (iserr)
	{
		sipBadSetType(sipName_BALL_FDPB,sipName_BALL_results);
		return NULL;
	}

	ptr -> results = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_FDPB_options(PyObject *sipThisObj,PyObject *valobj)
{
	int iserr = 0;
	Options *val;
	FDPB *ptr;

	if ((ptr = (FDPB *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_FDPB)) == NULL)
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
		sipBadSetType(sipName_BALL_FDPB,sipName_BALL_options);
		return NULL;
	}

	ptr -> options = *val;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_FDPB[] = {
	{sipName_BALL_destroy, sipDo_FDPB_destroy, METH_VARARGS, NULL},
	{sipName_BALL_destroyGrids, sipDo_FDPB_destroyGrids, METH_VARARGS, NULL},
	{sipName_BALL_setup, sipDo_FDPB_setup, METH_VARARGS, NULL},
	{sipName_BALL_setupEpsGrid, sipDo_FDPB_setupEpsGrid, METH_VARARGS, NULL},
	{sipName_BALL_setupSASGrid, sipDo_FDPB_setupSASGrid, METH_VARARGS, NULL},
	{sipName_BALL_setupAtomArray, sipDo_FDPB_setupAtomArray, METH_VARARGS, NULL},
	{sipName_BALL_setupKappaGrid, sipDo_FDPB_setupKappaGrid, METH_VARARGS, NULL},
	{sipName_BALL_setupQGrid, sipDo_FDPB_setupQGrid, METH_VARARGS, NULL},
	{sipName_BALL_setupPhiGrid, sipDo_FDPB_setupPhiGrid, METH_VARARGS, NULL},
	{sipName_BALL_setupBoundary, sipDo_FDPB_setupBoundary, METH_VARARGS, NULL},
	{sipName_BALL_solve, sipDo_FDPB_solve, METH_VARARGS, NULL},
	{sipName_BALL_getEnergy, sipDo_FDPB_getEnergy, METH_VARARGS, NULL},
	{sipName_BALL_getReactionFieldEnergy, sipDo_FDPB_getReactionFieldEnergy, METH_VARARGS, NULL},
	{sipName_BALL_calculateReactionFieldEnergy, sipDo_FDPB_calculateReactionFieldEnergy, METH_VARARGS, NULL},
	{sipName_BALL_getNumberOfIterations, sipDo_FDPB_getNumberOfIterations, METH_VARARGS, NULL},
	{sipName_BALL_getErrorCode, sipDo_FDPB_getErrorCode, METH_VARARGS, NULL},
	{sipName_BALL_getErrorMessage, sipDo_FDPB_getErrorMessage, METH_VARARGS, NULL},
	{NULL}
};

PyMethodDef sipClassVarTab_FDPB[] = {
	{sipName_BALL_phi_grid, sipGetSetVar_FDPB_phi_grid, 0, NULL},
	{sipName_BALL_q_grid, sipGetSetVar_FDPB_q_grid, 0, NULL},
	{sipName_BALL_kappa_grid, sipGetSetVar_FDPB_kappa_grid, 0, NULL},
	{sipName_BALL_results, sipGetSetVar_FDPB_results, 0, NULL},
	{sipName_BALL_options, sipGetSetVar_FDPB_options, 0, NULL},
	{NULL}
};

PyMethodDef *sipClassVarHierTab_FDPB[] = {
	sipClassVarTab_FDPB,
	NULL
};

int sipCanConvertTo_FDPB(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_FDPB);
}

void sipConvertTo_FDPB(PyObject *sipPy,FDPB **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_FDPB);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (FDPB *)sipConvertToCpp(sipPy,sipClass_FDPB,sipIsErr);
}

FDPB *sipForceConvertTo_FDPB(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_FDPB(valobj))
	{
		FDPB *val;

		sipConvertTo_FDPB(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_FDPB);

	*iserrp = 1;

	return NULL;
}
