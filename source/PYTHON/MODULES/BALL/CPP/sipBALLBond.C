#include "sipBALLDeclBALL.h"
#include "sipBALLBond.h"



PyObject *sipClass_Bond;

static void sipDealloc_Bond(sipThisType *);
static PyObject *sipPyInternalRepr_Bond(sipThisType *);

static PyTypeObject sipType_Bond = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Bond,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Bond,
	0,
	0,
	0,
	0,
	(reprfunc)sipPyInternalRepr_Bond,
};

sipBond::sipBond(): Bond()
{
	sipCommonCtor(sipPyMethods,5);
}

sipBond::sipBond(const Bond& a0,bool a1): Bond(a0,a1)
{
	sipCommonCtor(sipPyMethods,5);
}

sipBond::sipBond(const String& a0,Atom& a1,Atom& a2,short a3,short a4): Bond(a0,a1,a2,a3,a4)
{
	sipCommonCtor(sipPyMethods,5);
}

sipBond::sipBond(const Bond& a0): Bond(a0)
{
	sipCommonCtor(sipPyMethods,5);
}

sipBond::~sipBond()
{
	sipCommonDtor(sipPyThis);
}
void sipBond::select()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_select,&relLock))
		sipSelectable::sipVH_select(&sipPyMethods[0],sipPyThis,relLock);
	else
		Composite::select();
}
void sipBond::deselect()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_deselect,&relLock))
		sipSelectable::sipVH_deselect(&sipPyMethods[1],sipPyThis,relLock);
	else
		Composite::deselect();
}
void sipBond::clear()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipObject::sipVH_clear(&sipPyMethods[2],sipPyThis,relLock);
	else
		Bond::clear();
}
void sipBond::destroy()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_destroy,&relLock))
		sipObject::sipVH_destroy(&sipPyMethods[3],sipPyThis,relLock);
	else
		Bond::destroy();
}
bool sipBond::isValid() const
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_isValid,&relLock) ?
		sipObject::sipVH_isValid(&sipPyMethods[4],sipPyThis,relLock) :
		Bond::isValid();
}

static PyObject *sipDo_Bond_createBond(PyObject *,PyObject *sipArgs)
{

	{
		Bond *a0;
		PyObject *a0obj;
		Atom *a1;
		PyObject *a1obj;
		Atom *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"-III",sipCanConvertTo_Bond,&a0obj,sipCanConvertTo_Atom,&a1obj,sipCanConvertTo_Atom,&a2obj))
		{
			Bond *res;

			int iserr = 0;

			sipConvertTo_Bond(a0obj,&a0,1,&iserr);
			sipConvertTo_Atom(a1obj,&a1,1,&iserr);
			sipConvertTo_Atom(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = Bond::createBond(* a0,* a1,* a2);

			return sipMapCppToSelf(res,sipClass_Bond);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_createBond);

	return NULL;
}

static PyObject *sipDo_Bond_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			ptr -> Bond::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_Bond_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			ptr -> Bond::destroy();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_destroy);

	return NULL;
}

static PyObject *sipDo_Bond_finalize(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			ptr -> Bond::finalize();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_finalize);

	return NULL;
}

static PyObject *sipDo_Bond_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		Bond *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Bond,&a0obj))
		{
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Bond(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Bond::swap(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_swap);

	return NULL;
}

static PyObject *sipDo_Bond_setFirstAtom(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,0,&iserr);

			if (iserr)
				return NULL;

			ptr -> Bond::setFirstAtom( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_setFirstAtom);

	return NULL;
}

static PyObject *sipDo_Bond_getFirstAtom(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Atom *res;
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			res = ptr -> Bond::getFirstAtom();

			return sipMapCppToSelf(res,sipClass_Atom);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const Atom *res;
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			res = ptr -> Bond::getFirstAtom();

			return sipMapCppToSelf(res,sipClass_Atom);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_getFirstAtom);

	return NULL;
}

static PyObject *sipDo_Bond_getPartner(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		const Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			Atom *res;
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,(Atom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Bond::getPartner(* a0);

			return sipMapCppToSelf(res,sipClass_Atom);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_getPartner);

	return NULL;
}

static PyObject *sipDo_Bond_setSecondAtom(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,0,&iserr);

			if (iserr)
				return NULL;

			ptr -> Bond::setSecondAtom( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_setSecondAtom);

	return NULL;
}

static PyObject *sipDo_Bond_getSecondAtom(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Atom *res;
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			res = ptr -> Bond::getSecondAtom();

			return sipMapCppToSelf(res,sipClass_Atom);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const Atom *res;
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			res = ptr -> Bond::getSecondAtom();

			return sipMapCppToSelf(res,sipClass_Atom);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_getSecondAtom);

	return NULL;
}

static PyObject *sipDo_Bond_setName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Bond::setName(* a0);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_setName);

	return NULL;
}

static PyObject *sipDo_Bond_getName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			const String *res;
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			res = &ptr -> Bond::getName();

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_getName);

	return NULL;
}

static PyObject *sipDo_Bond_setOrder(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		short a0;

		if (sipParseArgs(sipArgs,"h",&a0))
		{
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			ptr -> Bond::setOrder( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_setOrder);

	return NULL;
}

static PyObject *sipDo_Bond_getOrder(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			short res;
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			res = ptr -> Bond::getOrder();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_getOrder);

	return NULL;
}

static PyObject *sipDo_Bond_setType(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		short a0;

		if (sipParseArgs(sipArgs,"h",&a0))
		{
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			ptr -> Bond::setType( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_setType);

	return NULL;
}

static PyObject *sipDo_Bond_getType(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			short res;
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			res = ptr -> Bond::getType();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_getType);

	return NULL;
}

static PyObject *sipDo_Bond_getLength(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			res = ptr -> Bond::getLength();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_getLength);

	return NULL;
}

static PyObject *sipDo_Bond_getBond(PyObject *,PyObject *sipArgs)
{

	{
		Atom *a0;
		PyObject *a0obj;
		Atom *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Atom,&a0obj,sipCanConvertTo_Atom,&a1obj))
		{
			Bond *res;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);
			sipConvertTo_Atom(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = Bond::getBond(* a0,* a1);

			return sipMapCppToSelf(res,sipClass_Bond);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_getBond);

	return NULL;
}

static PyObject *sipDo_Bond_getBoundAtom(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		const Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			Atom *res;
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,(Atom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Bond::getBoundAtom(* a0);

			return sipMapCppToSelf(res,sipClass_Atom);
		}
	}

	{
		const Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			const Atom *res;
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,(Atom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Bond::getBoundAtom(* a0);

			return sipMapCppToSelf(res,sipClass_Atom);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_getBoundAtom);

	return NULL;
}

static PyObject *sipDo_Bond_isBondOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		const Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			bool res;
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,(Atom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Bond::isBondOf(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_isBondOf);

	return NULL;
}

static PyObject *sipDo_Bond_isBound(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			res = ptr -> Bond::isBound();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_isBound);

	return NULL;
}

static PyObject *sipDo_Bond_isInterBond(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			res = ptr -> Bond::isInterBond();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_isInterBond);

	return NULL;
}

static PyObject *sipDo_Bond_isInterBondOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		const AtomContainer *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_AtomContainer,&a0obj))
		{
			bool res;
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_AtomContainer(a0obj,(AtomContainer **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Bond::isInterBondOf(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const System *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_System,&a0obj))
		{
			bool res;
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_System(a0obj,(System **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Bond::isInterBondOf(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_isInterBondOf);

	return NULL;
}

static PyObject *sipDo_Bond_isIntraBond(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			res = ptr -> Bond::isIntraBond();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_isIntraBond);

	return NULL;
}

static PyObject *sipDo_Bond_isIntraBondOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		const AtomContainer *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_AtomContainer,&a0obj))
		{
			bool res;
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_AtomContainer(a0obj,(AtomContainer **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Bond::isIntraBondOf(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const System *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_System,&a0obj))
		{
			bool res;
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_System(a0obj,(System **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Bond::isIntraBondOf(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_isIntraBondOf);

	return NULL;
}

static PyObject *sipDo_Bond_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Bond)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Bond *ptr;

			if ((ptr = (Bond *)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
				return NULL;

			res = ptr -> Bond::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Bond,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Bond(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_Bond)
		return ptr;

	if ((res = sipCast_Composite((Composite *)(Bond *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_PropertyManager((PropertyManager *)(Bond *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_Bond(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipBond *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (Bond *)sipThis -> u.cppPtr;
			else
				delete (sipBond *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

static PyObject *sipPyInternalRepr_Bond(sipThisType *sipThis)
{
#line 90 "bond.sip"
  Bond* ptr;
  if ((ptr = (Bond*)sipGetCppPtr(sipThis,sipClass_Bond)) == NULL)
    return NULL;

	Atom* a1 = ptr->getFirstAtom();
	Atom* a2 = ptr->getSecondAtom();
	String tmp("Bond {");
	if ((a1 != 0) && (a2 != 0))
	{
		tmp += a1->getFullName();
		tmp += " - ";
		tmp += a2->getFullName();
		tmp += ", ";
		tmp += String(ptr->getLength());
		tmp += " A";
		switch (ptr->getOrder())
		{
			case Bond::ORDER__SINGLE:
				tmp += ", single";
				break;
			case Bond::ORDER__DOUBLE:
				tmp += ", double";
				break;
			case Bond::ORDER__AROMATIC:
				tmp += ", aromatic";
				break;
			case Bond::ORDER__TRIPLE:
				tmp += ", triple";
				break;
			default:
				tmp += ", unknown";
		}
		tmp += " }";
	}
  return PyString_FromString(tmp.c_str());
#line 1141 "../CPP/sipBALLBond.cpp"
}

PyObject *sipNew_Bond(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Bond
	};

	sipThisType *sipThis = NULL;
	const void *sipNew = NULL;
	int sipFlags = SIP_PY_OWNED;

	// See if there is something pending.

	sipNew = sipGetPending(&sipFlags);

	if (sipNew == NULL)
	{
		if (sipParseArgs(sipArgs,"-"))
		{
			sipNew = new sipBond();
	}
	}

	if (sipNew == NULL)
	{
		const Bond *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_Bond,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_Bond(a0obj,(Bond **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipBond(* a0, (bool)a1);
	}
	}

	if (sipNew == NULL)
	{
		const String *a0;
		PyObject *a0obj;
		Atom *a1;
		PyObject *a1obj;
		Atom *a2;
		PyObject *a2obj;
		short a3 = Bond::ORDER__UNKNOWN;
		short a4 = Bond::TYPE__UNKNOWN;

		if (sipParseArgs(sipArgs,"-III|hh",sipCanConvertTo_String,&a0obj,sipCanConvertTo_Atom,&a1obj,sipCanConvertTo_Atom,&a2obj,&a3,&a4))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			sipConvertTo_Atom(a1obj,&a1,1,&iserr);
			sipConvertTo_Atom(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipBond(* a0,* a1,* a2, a3, a4);

			if (istemp0)
				delete a0;
	}
	}

	if (sipNew == NULL)
	{
		const Bond *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_Bond,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Bond(a0obj,(Bond **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipBond(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_Bond);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Bond,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (Bond *)sipNew;
			else
				delete (sipBond *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipBond *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Bond[] = {
	{sipName_BALL_createBond, sipDo_Bond_createBond, METH_VARARGS, NULL},
	{sipName_BALL_clear, sipDo_Bond_clear, METH_VARARGS, NULL},
	{sipName_BALL_destroy, sipDo_Bond_destroy, METH_VARARGS, NULL},
	{sipName_BALL_finalize, sipDo_Bond_finalize, METH_VARARGS, NULL},
	{sipName_BALL_swap, sipDo_Bond_swap, METH_VARARGS, NULL},
	{sipName_BALL_setFirstAtom, sipDo_Bond_setFirstAtom, METH_VARARGS, NULL},
	{sipName_BALL_getFirstAtom, sipDo_Bond_getFirstAtom, METH_VARARGS, NULL},
	{sipName_BALL_getPartner, sipDo_Bond_getPartner, METH_VARARGS, NULL},
	{sipName_BALL_setSecondAtom, sipDo_Bond_setSecondAtom, METH_VARARGS, NULL},
	{sipName_BALL_getSecondAtom, sipDo_Bond_getSecondAtom, METH_VARARGS, NULL},
	{sipName_BALL_setName, sipDo_Bond_setName, METH_VARARGS, NULL},
	{sipName_BALL_getName, sipDo_Bond_getName, METH_VARARGS, NULL},
	{sipName_BALL_setOrder, sipDo_Bond_setOrder, METH_VARARGS, NULL},
	{sipName_BALL_getOrder, sipDo_Bond_getOrder, METH_VARARGS, NULL},
	{sipName_BALL_setType, sipDo_Bond_setType, METH_VARARGS, NULL},
	{sipName_BALL_getType, sipDo_Bond_getType, METH_VARARGS, NULL},
	{sipName_BALL_getLength, sipDo_Bond_getLength, METH_VARARGS, NULL},
	{sipName_BALL_getBond, sipDo_Bond_getBond, METH_VARARGS, NULL},
	{sipName_BALL_getBoundAtom, sipDo_Bond_getBoundAtom, METH_VARARGS, NULL},
	{sipName_BALL_isBondOf, sipDo_Bond_isBondOf, METH_VARARGS, NULL},
	{sipName_BALL_isBound, sipDo_Bond_isBound, METH_VARARGS, NULL},
	{sipName_BALL_isInterBond, sipDo_Bond_isInterBond, METH_VARARGS, NULL},
	{sipName_BALL_isInterBondOf, sipDo_Bond_isInterBondOf, METH_VARARGS, NULL},
	{sipName_BALL_isIntraBond, sipDo_Bond_isIntraBond, METH_VARARGS, NULL},
	{sipName_BALL_isIntraBondOf, sipDo_Bond_isIntraBondOf, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_Bond_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_Bond(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Bond);
}

void sipConvertTo_Bond(PyObject *sipPy,Bond **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_Bond);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Bond *)sipConvertToCpp(sipPy,sipClass_Bond,sipIsErr);
}

Bond *sipForceConvertTo_Bond(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Bond(valobj))
	{
		Bond *val;

		sipConvertTo_Bond(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Bond);
	*iserrp = 1;

	return NULL;
}
