#include "sipBALLDeclBALL.h"
#include "sipBALLBaseFragment.h"



PyObject *sipClass_BaseFragment;

static void sipDealloc_BaseFragment(sipThisType *);
static PyObject *sipPyInternalRepr_BaseFragment(sipThisType *);

static PyTypeObject sipType_BaseFragment = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_BaseFragment,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_BaseFragment,
	0,
	0,
	0,
	0,
	(reprfunc)sipPyInternalRepr_BaseFragment,
};

sipBaseFragment::sipBaseFragment(): BaseFragment()
{
	sipCommonCtor(sipPyMethods,5);
}

sipBaseFragment::sipBaseFragment(const BaseFragment& a0,bool a1): BaseFragment(a0,a1)
{
	sipCommonCtor(sipPyMethods,5);
}

sipBaseFragment::sipBaseFragment(const String& a0): BaseFragment(a0)
{
	sipCommonCtor(sipPyMethods,5);
}

sipBaseFragment::sipBaseFragment(const BaseFragment& a0): BaseFragment(a0)
{
	sipCommonCtor(sipPyMethods,5);
}

sipBaseFragment::~sipBaseFragment()
{
	sipCommonDtor(sipPyThis);
}
void sipBaseFragment::select()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_select,&relLock))
		sipSelectable::sipVH_select(&sipPyMethods[0],sipPyThis,relLock);
	else
		Composite::select();
}
void sipBaseFragment::deselect()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_deselect,&relLock))
		sipSelectable::sipVH_deselect(&sipPyMethods[1],sipPyThis,relLock);
	else
		Composite::deselect();
}
void sipBaseFragment::clear()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipObject::sipVH_clear(&sipPyMethods[2],sipPyThis,relLock);
	else
		BaseFragment::clear();
}
void sipBaseFragment::destroy()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_destroy,&relLock))
		sipObject::sipVH_destroy(&sipPyMethods[3],sipPyThis,relLock);
	else
		BaseFragment::destroy();
}
bool sipBaseFragment::isValid() const
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_isValid,&relLock) ?
		sipObject::sipVH_isValid(&sipPyMethods[4],sipPyThis,relLock) :
		BaseFragment::isValid();
}

static PyObject *sipDo_BaseFragment_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			ptr -> BaseFragment::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_BaseFragment_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			ptr -> BaseFragment::destroy();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_destroy);

	return NULL;
}

static PyObject *sipDo_BaseFragment_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		const BaseFragment *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"I|l",sipCanConvertTo_BaseFragment,&a0obj,&a1))
		{
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BaseFragment::set(* a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_BaseFragment_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		BaseFragment *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"I|l",sipCanConvertTo_BaseFragment,&a0obj,&a1))
		{
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BaseFragment::get(* a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_get);

	return NULL;
}

static PyObject *sipDo_BaseFragment_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		BaseFragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_BaseFragment,&a0obj))
		{
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BaseFragment::swap(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_swap);

	return NULL;
}

static PyObject *sipDo_BaseFragment_setName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BaseFragment::setName(* a0);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_setName);

	return NULL;
}

static PyObject *sipDo_BaseFragment_getName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			String *res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			res = &ptr -> BaseFragment::getName();

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const String *res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			res = &ptr -> BaseFragment::getName();

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_getName);

	return NULL;
}

static PyObject *sipDo_BaseFragment_getMolecule(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Molecule *res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			res = ptr -> BaseFragment::getMolecule();

			return sipMapCppToSelf(res,sipClass_Molecule);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const Molecule *res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			res = ptr -> BaseFragment::getMolecule();

			return sipMapCppToSelf(res,sipClass_Molecule);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_getMolecule);

	return NULL;
}

static PyObject *sipDo_BaseFragment_getSuperBaseFragment(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			BaseFragment *res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			res = ptr -> BaseFragment::getSuperBaseFragment();

			return sipMapCppToSelf(res,sipClass_BaseFragment);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const BaseFragment *res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			res = ptr -> BaseFragment::getSuperBaseFragment();

			return sipMapCppToSelf(res,sipClass_BaseFragment);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_getSuperBaseFragment);

	return NULL;
}

static PyObject *sipDo_BaseFragment_getBaseFragment(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			BaseFragment *res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> BaseFragment::getBaseFragment(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_BaseFragment);
		}
	}

	{
		Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			const BaseFragment *res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> BaseFragment::getBaseFragment(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_BaseFragment);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_getBaseFragment);

	return NULL;
}

static PyObject *sipDo_BaseFragment_getAtom(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			Atom *res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> BaseFragment::getAtom(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_Atom);
		}
	}

	{
		Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			const Atom *res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> BaseFragment::getAtom(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_Atom);
		}
	}

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			Atom *res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> BaseFragment::getAtom(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_Atom);
		}
	}

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			const Atom *res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> BaseFragment::getAtom(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_Atom);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_getAtom);

	return NULL;
}

static PyObject *sipDo_BaseFragment_countBaseFragments(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			res = ptr -> BaseFragment::countBaseFragments();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_countBaseFragments);

	return NULL;
}

static PyObject *sipDo_BaseFragment_countAtoms(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			res = ptr -> BaseFragment::countAtoms();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_countAtoms);

	return NULL;
}

static PyObject *sipDo_BaseFragment_countBonds(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			res = ptr -> BaseFragment::countBonds();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_countBonds);

	return NULL;
}

static PyObject *sipDo_BaseFragment_countInterBonds(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			res = ptr -> BaseFragment::countInterBonds();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_countInterBonds);

	return NULL;
}

static PyObject *sipDo_BaseFragment_countIntraBonds(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			res = ptr -> BaseFragment::countIntraBonds();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_countIntraBonds);

	return NULL;
}

static PyObject *sipDo_BaseFragment_prepend(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BaseFragment::prepend(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		BaseFragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_BaseFragment,&a0obj))
		{
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BaseFragment::prepend(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_prepend);

	return NULL;
}

static PyObject *sipDo_BaseFragment_append(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BaseFragment::append(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		BaseFragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_BaseFragment,&a0obj))
		{
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BaseFragment::append(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_append);

	return NULL;
}

static PyObject *sipDo_BaseFragment_insert(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BaseFragment::insert(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		BaseFragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_BaseFragment,&a0obj))
		{
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BaseFragment::insert(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_insert);

	return NULL;
}

static PyObject *sipDo_BaseFragment_insertBefore(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		Atom *a0;
		PyObject *a0obj;
		Composite *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_Atom,&a0obj,sipCanConvertTo_Composite,&a1obj))
		{
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);
			sipConvertTo_Composite(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BaseFragment::insertBefore(* a0,* a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		BaseFragment *a0;
		PyObject *a0obj;
		Composite *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_BaseFragment,&a0obj,sipCanConvertTo_Composite,&a1obj))
		{
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,&a0,1,&iserr);
			sipConvertTo_Composite(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BaseFragment::insertBefore(* a0,* a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_insertBefore);

	return NULL;
}

static PyObject *sipDo_BaseFragment_insertAfter(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		Atom *a0;
		PyObject *a0obj;
		Composite *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_Atom,&a0obj,sipCanConvertTo_Composite,&a1obj))
		{
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);
			sipConvertTo_Composite(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BaseFragment::insertAfter(* a0,* a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	{
		BaseFragment *a0;
		PyObject *a0obj;
		Composite *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_BaseFragment,&a0obj,sipCanConvertTo_Composite,&a1obj))
		{
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,&a0,1,&iserr);
			sipConvertTo_Composite(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BaseFragment::insertAfter(* a0,* a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_insertAfter);

	return NULL;
}

static PyObject *sipDo_BaseFragment_remove(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			bool res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> BaseFragment::remove(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	{
		BaseFragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_BaseFragment,&a0obj))
		{
			bool res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> BaseFragment::remove(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_remove);

	return NULL;
}

static PyObject *sipDo_BaseFragment_spliceBefore(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		BaseFragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_BaseFragment,&a0obj))
		{
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BaseFragment::spliceBefore(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_spliceBefore);

	return NULL;
}

static PyObject *sipDo_BaseFragment_spliceAfter(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		BaseFragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_BaseFragment,&a0obj))
		{
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BaseFragment::spliceAfter(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_spliceAfter);

	return NULL;
}

static PyObject *sipDo_BaseFragment_splice(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		BaseFragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_BaseFragment,&a0obj))
		{
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> BaseFragment::splice(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_splice);

	return NULL;
}

static PyObject *sipDo_BaseFragment_destroyBonds(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			ptr -> BaseFragment::destroyBonds();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_destroyBonds);

	return NULL;
}

static PyObject *sipDo_BaseFragment_isSubBaseFragmentOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		const BaseFragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_BaseFragment,&a0obj))
		{
			bool res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> BaseFragment::isSubBaseFragmentOf(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_isSubBaseFragmentOf);

	return NULL;
}

static PyObject *sipDo_BaseFragment_isSuperBaseFragmentOf(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		const BaseFragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_BaseFragment,&a0obj))
		{
			bool res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> BaseFragment::isSuperBaseFragmentOf(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_isSuperBaseFragmentOf);

	return NULL;
}

static PyObject *sipDo_BaseFragment_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			res = ptr -> BaseFragment::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_isValid);

	return NULL;
}

static PyObject *sipDo_BaseFragment_apply(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_BaseFragment)) == NULL)
		return NULL;

	{
		AtomProcessor *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_AtomProcessor,&a0obj))
		{
			bool res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_AtomProcessor(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> BaseFragment::apply(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	{
		CompositeProcessor *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_CompositeProcessor,&a0obj))
		{
			bool res;
			BaseFragment *ptr;

			if ((ptr = (BaseFragment *)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_CompositeProcessor(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> BaseFragment::apply(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_BaseFragment,sipName_BALL_apply);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_BaseFragment(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_BaseFragment)
		return ptr;

	if ((res = sipCast_Composite((Composite *)(BaseFragment *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_PropertyManager((PropertyManager *)(BaseFragment *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_BaseFragment(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipBaseFragment *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (BaseFragment *)sipThis -> u.cppPtr;
			else
				delete (sipBaseFragment *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

static PyObject *sipPyInternalRepr_BaseFragment(sipThisType *sipThis)
{
#line 73 "baseFragment.sip"
  BaseFragment* ptr;
  if ((ptr = (BaseFragment*)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
    return NULL;

  return PyString_FromString(String(String("BaseFragment ") + ptr->getName() 
						+ " { " + String(ptr->countAtoms()) + " atoms }").c_str());
#line 1548 "../CPP/sipBALLBaseFragment.cpp"
}

PyObject *sipNew_BaseFragment(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_BaseFragment
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
			sipNew = new sipBaseFragment();
	}
	}

	if (sipNew == NULL)
	{
		const BaseFragment *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_BaseFragment,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipBaseFragment(* a0, (bool)a1);
	}
	}

	if (sipNew == NULL)
	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_String,&a0obj))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipBaseFragment(* a0);

			if (istemp0)
				delete a0;
	}
	}

	if (sipNew == NULL)
	{
		const BaseFragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_BaseFragment,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipBaseFragment(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_BaseFragment);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_BaseFragment,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (BaseFragment *)sipNew;
			else
				delete (sipBaseFragment *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipBaseFragment *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_BaseFragment[] = {
	{sipName_BALL_clear, sipDo_BaseFragment_clear, METH_VARARGS, NULL},
	{sipName_BALL_destroy, sipDo_BaseFragment_destroy, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_BaseFragment_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_BaseFragment_get, METH_VARARGS, NULL},
	{sipName_BALL_swap, sipDo_BaseFragment_swap, METH_VARARGS, NULL},
	{sipName_BALL_setName, sipDo_BaseFragment_setName, METH_VARARGS, NULL},
	{sipName_BALL_getName, sipDo_BaseFragment_getName, METH_VARARGS, NULL},
	{sipName_BALL_getMolecule, sipDo_BaseFragment_getMolecule, METH_VARARGS, NULL},
	{sipName_BALL_getSuperBaseFragment, sipDo_BaseFragment_getSuperBaseFragment, METH_VARARGS, NULL},
	{sipName_BALL_getBaseFragment, sipDo_BaseFragment_getBaseFragment, METH_VARARGS, NULL},
	{sipName_BALL_getAtom, sipDo_BaseFragment_getAtom, METH_VARARGS, NULL},
	{sipName_BALL_countBaseFragments, sipDo_BaseFragment_countBaseFragments, METH_VARARGS, NULL},
	{sipName_BALL_countAtoms, sipDo_BaseFragment_countAtoms, METH_VARARGS, NULL},
	{sipName_BALL_countBonds, sipDo_BaseFragment_countBonds, METH_VARARGS, NULL},
	{sipName_BALL_countInterBonds, sipDo_BaseFragment_countInterBonds, METH_VARARGS, NULL},
	{sipName_BALL_countIntraBonds, sipDo_BaseFragment_countIntraBonds, METH_VARARGS, NULL},
	{sipName_BALL_prepend, sipDo_BaseFragment_prepend, METH_VARARGS, NULL},
	{sipName_BALL_append, sipDo_BaseFragment_append, METH_VARARGS, NULL},
	{sipName_BALL_insert, sipDo_BaseFragment_insert, METH_VARARGS, NULL},
	{sipName_BALL_insertBefore, sipDo_BaseFragment_insertBefore, METH_VARARGS, NULL},
	{sipName_BALL_insertAfter, sipDo_BaseFragment_insertAfter, METH_VARARGS, NULL},
	{sipName_BALL_remove, sipDo_BaseFragment_remove, METH_VARARGS, NULL},
	{sipName_BALL_spliceBefore, sipDo_BaseFragment_spliceBefore, METH_VARARGS, NULL},
	{sipName_BALL_spliceAfter, sipDo_BaseFragment_spliceAfter, METH_VARARGS, NULL},
	{sipName_BALL_splice, sipDo_BaseFragment_splice, METH_VARARGS, NULL},
	{sipName_BALL_destroyBonds, sipDo_BaseFragment_destroyBonds, METH_VARARGS, NULL},
	{sipName_BALL_isSubBaseFragmentOf, sipDo_BaseFragment_isSubBaseFragmentOf, METH_VARARGS, NULL},
	{sipName_BALL_isSuperBaseFragmentOf, sipDo_BaseFragment_isSuperBaseFragmentOf, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_BaseFragment_isValid, METH_VARARGS, NULL},
	{sipName_BALL_apply, sipDo_BaseFragment_apply, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_BaseFragment(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_BaseFragment);
}

void sipConvertTo_BaseFragment(PyObject *sipPy,BaseFragment **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_BaseFragment);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (BaseFragment *)sipConvertToCpp(sipPy,sipClass_BaseFragment,sipIsErr);
}

BaseFragment *sipForceConvertTo_BaseFragment(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_BaseFragment(valobj))
	{
		BaseFragment *val;

		sipConvertTo_BaseFragment(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_BaseFragment);
	*iserrp = 1;

	return NULL;
}
