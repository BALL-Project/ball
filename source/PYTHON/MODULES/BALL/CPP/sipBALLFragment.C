#include "sipBALLDeclBALL.h"
#include "sipBALLFragment.h"



PyObject *sipClass_Fragment;

static void sipDealloc_Fragment(sipThisType *);
static PyObject *sipPyInternalRepr_Fragment(sipThisType *);

static PyTypeObject sipType_Fragment = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Fragment,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Fragment,
	0,
	0,
	0,
	0,
	(reprfunc)sipPyInternalRepr_Fragment,
};

sipFragment::sipFragment(): Fragment()
{
	sipCommonCtor(sipPyMethods,5);
}

sipFragment::sipFragment(const Fragment& a0,bool a1): Fragment(a0,a1)
{
	sipCommonCtor(sipPyMethods,5);
}

sipFragment::sipFragment(const String& a0): Fragment(a0)
{
	sipCommonCtor(sipPyMethods,5);
}

sipFragment::sipFragment(const Fragment& a0): Fragment(a0)
{
	sipCommonCtor(sipPyMethods,5);
}

sipFragment::~sipFragment()
{
	sipCommonDtor(sipPyThis);
}
void sipFragment::select()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_select,&relLock))
		sipSelectable::sipVH_select(&sipPyMethods[0],sipPyThis,relLock);
	else
		Composite::select();
}
void sipFragment::deselect()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_deselect,&relLock))
		sipSelectable::sipVH_deselect(&sipPyMethods[1],sipPyThis,relLock);
	else
		Composite::deselect();
}
void sipFragment::clear()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipObject::sipVH_clear(&sipPyMethods[2],sipPyThis,relLock);
	else
		AtomContainer::clear();
}
void sipFragment::destroy()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_destroy,&relLock))
		sipObject::sipVH_destroy(&sipPyMethods[3],sipPyThis,relLock);
	else
		AtomContainer::destroy();
}
bool sipFragment::isValid() const
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_isValid,&relLock) ?
		sipObject::sipVH_isValid(&sipPyMethods[4],sipPyThis,relLock) :
		AtomContainer::isValid();
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Fragment(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_Fragment)
		return ptr;

	if ((res = sipCast_AtomContainer((AtomContainer *)(Fragment *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_Fragment(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipFragment *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (Fragment *)sipThis -> u.cppPtr;
			else
				delete (sipFragment *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

static PyObject *sipPyInternalRepr_Fragment(sipThisType *sipThis)
{
#line 28 "fragment.sip"
  Fragment* ptr;
  if ((ptr = (Fragment*)sipGetCppPtr(sipThis,sipClass_Fragment)) == NULL)
    return NULL;

  return PyString_FromString(String(String("Fragment ") + ptr->getName()
        + " { " + String(ptr->countAtoms()) + " atoms }").c_str());
#line 140 "../CPP/sipBALLFragment.cpp"
}

PyObject *sipNew_Fragment(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Fragment
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
			sipNew = new sipFragment();
	}
	}

	if (sipNew == NULL)
	{
		const Fragment *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_Fragment,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_Fragment(a0obj,(Fragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipFragment(* a0, (bool)a1);
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

			sipNew = new sipFragment(* a0);

			if (istemp0)
				delete a0;
	}
	}

	if (sipNew == NULL)
	{
		const Fragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_Fragment,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Fragment(a0obj,(Fragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipFragment(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_Fragment);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Fragment,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (Fragment *)sipNew;
			else
				delete (sipFragment *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipFragment *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Fragment[] = {
	{NULL}
};

int sipCanConvertTo_Fragment(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Fragment);
}

void sipConvertTo_Fragment(PyObject *sipPy,Fragment **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_Fragment);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Fragment *)sipConvertToCpp(sipPy,sipClass_Fragment,sipIsErr);
}

Fragment *sipForceConvertTo_Fragment(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Fragment(valobj))
	{
		Fragment *val;

		sipConvertTo_Fragment(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Fragment);
	*iserrp = 1;

	return NULL;
}
