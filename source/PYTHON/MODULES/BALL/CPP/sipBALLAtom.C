#include "sipBALLDeclBALL.h"
#include "sipBALLAtom.h"



PyObject *sipClass_Atom;

static void sipDealloc_Atom(sipThisType *);
static PyObject *sipPyInternalRepr_Atom(sipThisType *);

static PyTypeObject sipType_Atom = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Atom,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Atom,
	0,
	0,
	0,
	0,
	(reprfunc)sipPyInternalRepr_Atom,
};

sipAtom::sipAtom(): Atom()
{
	sipCommonCtor(sipPyMethods,5);
}

sipAtom::sipAtom(const Atom& a0,bool a1): Atom(a0,a1)
{
	sipCommonCtor(sipPyMethods,5);
}

sipAtom::sipAtom(Element& a0,const String& a1,const String& a2,int a3,const Vector3& a4,const Vector3& a5,const Vector3& a6,float a7,float a8): Atom(a0,a1,a2,a3,a4,a5,a6,a7,a8)
{
	sipCommonCtor(sipPyMethods,5);
}

sipAtom::sipAtom(const Atom& a0): Atom(a0)
{
	sipCommonCtor(sipPyMethods,5);
}

sipAtom::~sipAtom()
{
	sipCommonDtor(sipPyThis);
}
void sipAtom::select()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_select,&relLock))
		sipSelectable::sipVH_select(&sipPyMethods[0],sipPyThis,relLock);
	else
		Composite::select();
}
void sipAtom::deselect()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_deselect,&relLock))
		sipSelectable::sipVH_deselect(&sipPyMethods[1],sipPyThis,relLock);
	else
		Composite::deselect();
}
void sipAtom::clear()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipObject::sipVH_clear(&sipPyMethods[2],sipPyThis,relLock);
	else
		Atom::clear();
}
void sipAtom::destroy()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_destroy,&relLock))
		sipObject::sipVH_destroy(&sipPyMethods[3],sipPyThis,relLock);
	else
		Atom::destroy();
}
bool sipAtom::isValid() const
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_isValid,&relLock) ?
		sipObject::sipVH_isValid(&sipPyMethods[4],sipPyThis,relLock) :
		Atom::isValid();
}

static PyObject *sipDo_Atom_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			ptr -> Atom::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_Atom_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			ptr -> Atom::destroy();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_destroy);

	return NULL;
}

static PyObject *sipDo_Atom_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		const Atom *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"I|l",sipCanConvertTo_Atom,&a0obj,&a1))
		{
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,(Atom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Atom::set(* a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_Atom_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		Atom *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"I|l",sipCanConvertTo_Atom,&a0obj,&a1))
		{
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Atom::get(* a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_get);

	return NULL;
}

static PyObject *sipDo_Atom_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Atom::swap(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_swap);

	return NULL;
}

static PyObject *sipDo_Atom_setElement(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		Element *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Element,&a0obj))
		{
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Element(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Atom::setElement(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_setElement);

	return NULL;
}

static PyObject *sipDo_Atom_getElement(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			const Element *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			res = &ptr -> Atom::getElement();

			return sipMapCppToSelf(res,sipClass_Element);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_getElement);

	return NULL;
}

static PyObject *sipDo_Atom_setCharge(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		float a0;

		if (sipParseArgs(sipArgs,"f",&a0))
		{
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			ptr -> Atom::setCharge( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_setCharge);

	return NULL;
}

static PyObject *sipDo_Atom_getCharge(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			res = ptr -> Atom::getCharge();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_getCharge);

	return NULL;
}

static PyObject *sipDo_Atom_getMolecule(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Molecule *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			res = ptr -> Atom::getMolecule();

			return sipMapCppToSelf(res,sipClass_Molecule);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const Molecule *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			res = ptr -> Atom::getMolecule();

			return sipMapCppToSelf(res,sipClass_Molecule);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_getMolecule);

	return NULL;
}

static PyObject *sipDo_Atom_getFragment(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Fragment *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			res = ptr -> Atom::getFragment();

			return sipMapCppToSelf(res,sipClass_Fragment);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const Fragment *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			res = ptr -> Atom::getFragment();

			return sipMapCppToSelf(res,sipClass_Fragment);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_getFragment);

	return NULL;
}

static PyObject *sipDo_Atom_setName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Atom::setName(* a0);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_setName);

	return NULL;
}

static PyObject *sipDo_Atom_getName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			String *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			res = &ptr -> Atom::getName();

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const String *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			res = &ptr -> Atom::getName();

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_getName);

	return NULL;
}

static PyObject *sipDo_Atom_getFullName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		long a0 = Atom::ADD_VARIANT_EXTENSIONS;

		if (sipParseArgs(sipArgs,"|l",&a0))
		{
			String *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			res = new String(ptr -> Atom::getFullName( (Atom::FullNameType)a0));

			return sipNewCppToSelf(res,sipClass_String,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_getFullName);

	return NULL;
}

static PyObject *sipDo_Atom_setPosition(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Atom::setPosition(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_setPosition);

	return NULL;
}

static PyObject *sipDo_Atom_getPosition(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Vector3 *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			res = &ptr -> Atom::getPosition();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const Vector3 *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			res = &ptr -> Atom::getPosition();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_getPosition);

	return NULL;
}

static PyObject *sipDo_Atom_setRadius(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		float a0;

		if (sipParseArgs(sipArgs,"f",&a0))
		{
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			ptr -> Atom::setRadius( a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_setRadius);

	return NULL;
}

static PyObject *sipDo_Atom_getRadius(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			float res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			res = ptr -> Atom::getRadius();

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_getRadius);

	return NULL;
}

static PyObject *sipDo_Atom_setType(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		AtomType *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_AtomType,&a0obj))
		{
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_AtomType(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Atom::setType(* a0);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_setType);

	return NULL;
}

static PyObject *sipDo_Atom_getType(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			AtomType *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			res = new AtomType(ptr -> Atom::getType());

			return sipNewCppToSelf(res,sipClass_AtomType,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_getType);

	return NULL;
}

static PyObject *sipDo_Atom_getTypeName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			String *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			res = new String(ptr -> Atom::getTypeName());

			return sipNewCppToSelf(res,sipClass_String,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_getTypeName);

	return NULL;
}

static PyObject *sipDo_Atom_setTypeName(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Atom::setTypeName(* a0);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_setTypeName);

	return NULL;
}

static PyObject *sipDo_Atom_setVelocity(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Atom::setVelocity(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_setVelocity);

	return NULL;
}

static PyObject *sipDo_Atom_getVelocity(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			const Vector3 *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			res = &ptr -> Atom::getVelocity();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_getVelocity);

	return NULL;
}

static PyObject *sipDo_Atom_setForce(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		const Vector3 *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Vector3,&a0obj))
		{
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Atom::setForce(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_setForce);

	return NULL;
}

static PyObject *sipDo_Atom_getForce(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Vector3 *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			res = &ptr -> Atom::getForce();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const Vector3 *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			res = &ptr -> Atom::getForce();

			return sipMapCppToSelf(res,sipClass_Vector3);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_getForce);

	return NULL;
}

static PyObject *sipDo_Atom_countBonds(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			res = ptr -> Atom::countBonds();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_countBonds);

	return NULL;
}

static PyObject *sipDo_Atom_getBond(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			Bond *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Atom::getBond(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_Bond);
		}
	}

	{
		Position *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Position,&a0obj))
		{
			const Bond *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_Position(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Atom::getBond(* a0);

			if (istemp0)
				delete a0;

			return sipMapCppToSelf(res,sipClass_Bond);
		}
	}

	{
		const Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			Bond *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,(Atom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Atom::getBond(* a0);

			return sipMapCppToSelf(res,sipClass_Bond);
		}
	}

	{
		const Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			const Bond *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,(Atom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Atom::getBond(* a0);

			return sipMapCppToSelf(res,sipClass_Bond);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_getBond);

	return NULL;
}

static PyObject *sipDo_Atom_createBond(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			Bond *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Atom::createBond(* a0);

			return sipMapCppToSelf(res,sipClass_Bond);
		}
	}

	{
		Bond *a0;
		PyObject *a0obj;
		Atom *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_Bond,&a0obj,sipCanConvertTo_Atom,&a1obj))
		{
			Bond *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Bond(a0obj,&a0,1,&iserr);
			sipConvertTo_Atom(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Atom::createBond(* a0,* a1);

			return sipMapCppToSelf(res,sipClass_Bond);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_createBond);

	return NULL;
}

static PyObject *sipDo_Atom_cloneBond(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		Bond *a0;
		PyObject *a0obj;
		Atom *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"II",sipCanConvertTo_Bond,&a0obj,sipCanConvertTo_Atom,&a1obj))
		{
			Bond *res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Bond(a0obj,&a0,1,&iserr);
			sipConvertTo_Atom(a1obj,&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Atom::cloneBond(* a0,* a1);

			return sipMapCppToSelf(res,sipClass_Bond);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_cloneBond);

	return NULL;
}

static PyObject *sipDo_Atom_destroyBond(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		const Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			bool res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,(Atom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Atom::destroyBond(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_destroyBond);

	return NULL;
}

static PyObject *sipDo_Atom_destroyBonds(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			ptr -> Atom::destroyBonds();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_destroyBonds);

	return NULL;
}

static PyObject *sipDo_Atom_hasBond(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		const Bond *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Bond,&a0obj))
		{
			bool res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Bond(a0obj,(Bond **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Atom::hasBond(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_hasBond);

	return NULL;
}

static PyObject *sipDo_Atom_isBondedTo(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		const Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			bool res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,(Atom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Atom::isBondedTo(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_isBondedTo);

	return NULL;
}

static PyObject *sipDo_Atom_isBonded(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			res = ptr -> Atom::isBonded();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_isBonded);

	return NULL;
}

static PyObject *sipDo_Atom_isGeminal(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		const Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			bool res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,(Atom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Atom::isGeminal(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_isGeminal);

	return NULL;
}

static PyObject *sipDo_Atom_isVicinal(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		const Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Atom,&a0obj))
		{
			bool res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,(Atom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = ptr -> Atom::isVicinal(* a0);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_isVicinal);

	return NULL;
}

static PyObject *sipDo_Atom_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Atom)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Atom *ptr;

			if ((ptr = (Atom *)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
				return NULL;

			res = ptr -> Atom::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Atom,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Atom(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_Atom)
		return ptr;

	if ((res = sipCast_Composite((Composite *)(Atom *)ptr,targetClass)) != NULL)
		return res;

	if ((res = sipCast_PropertyManager((PropertyManager *)(Atom *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_Atom(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipAtom *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (Atom *)sipThis -> u.cppPtr;
			else
				delete (sipAtom *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

static PyObject *sipPyInternalRepr_Atom(sipThisType *sipThis)
{
#line 101 "atom.sip"
  Atom* ptr;
  if ((ptr = (Atom*)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
    return NULL;

  return PyString_FromString(String(String("Atom ") + ptr->getName() 
				+ " { " + ptr->getElement().getSymbol() + " @ (" 
				+ String(ptr->getPosition().x) + " " + String(ptr->getPosition().y) + " " 
				+ String(ptr->getPosition().z) + " }").c_str());
#line 1631 "../CPP/sipBALLAtom.cpp"
}

PyObject *sipNew_Atom(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Atom
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
			sipNew = new sipAtom();
	}
	}

	if (sipNew == NULL)
	{
		const Atom *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_Atom,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_Atom(a0obj,(Atom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipAtom(* a0, (bool)a1);
	}
	}

	if (sipNew == NULL)
	{
		Element *a0;
		PyObject *a0obj;
		const String *a1;
		PyObject *a1obj;
		const String *a2 = NULL;
		PyObject *a2obj = NULL;
		int a3 = Atom::UNKNOWN_TYPE;
		Vector3 a4def = Vector3(0,0,0);
		const Vector3 *a4 = &a4def;
		PyObject *a4obj = NULL;
		Vector3 a5def = Vector3(0,0,0);
		const Vector3 *a5 = &a5def;
		PyObject *a5obj = NULL;
		Vector3 a6def = Vector3(0,0,0);
		const Vector3 *a6 = &a6def;
		PyObject *a6obj = NULL;
		float a7 = 0;
		float a8 = 0;

		if (sipParseArgs(sipArgs,"-II|IiIIIff",sipCanConvertTo_Element,&a0obj,sipCanConvertTo_String,&a1obj,sipCanConvertTo_String,&a2obj,&a3,sipCanConvertTo_Vector3,&a4obj,sipCanConvertTo_Vector3,&a5obj,sipCanConvertTo_Vector3,&a6obj,&a7,&a8))
		{
			int iserr = 0;

			sipConvertTo_Element(a0obj,&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);
			int istemp2 = sipConvertTo_String(a2obj,(String **)&a2,1,&iserr);
			sipConvertTo_Vector3(a4obj,(Vector3 **)&a4,1,&iserr);
			sipConvertTo_Vector3(a5obj,(Vector3 **)&a5,1,&iserr);
			sipConvertTo_Vector3(a6obj,(Vector3 **)&a6,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipAtom(* a0,* a1,* a2, a3,* a4,* a5,* a6, a7, a8);

			if (istemp1)
				delete a1;

			if (istemp2)
				delete a2;
	}
	}

	if (sipNew == NULL)
	{
		const Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_Atom,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Atom(a0obj,(Atom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipAtom(* a0);
	}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_Atom);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Atom,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (Atom *)sipNew;
			else
				delete (sipAtom *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipAtom *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Atom[] = {
	{sipName_BALL_clear, sipDo_Atom_clear, METH_VARARGS, NULL},
	{sipName_BALL_destroy, sipDo_Atom_destroy, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_Atom_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_Atom_get, METH_VARARGS, NULL},
	{sipName_BALL_swap, sipDo_Atom_swap, METH_VARARGS, NULL},
	{sipName_BALL_setElement, sipDo_Atom_setElement, METH_VARARGS, NULL},
	{sipName_BALL_getElement, sipDo_Atom_getElement, METH_VARARGS, NULL},
	{sipName_BALL_setCharge, sipDo_Atom_setCharge, METH_VARARGS, NULL},
	{sipName_BALL_getCharge, sipDo_Atom_getCharge, METH_VARARGS, NULL},
	{sipName_BALL_getMolecule, sipDo_Atom_getMolecule, METH_VARARGS, NULL},
	{sipName_BALL_getFragment, sipDo_Atom_getFragment, METH_VARARGS, NULL},
	{sipName_BALL_setName, sipDo_Atom_setName, METH_VARARGS, NULL},
	{sipName_BALL_getName, sipDo_Atom_getName, METH_VARARGS, NULL},
	{sipName_BALL_getFullName, sipDo_Atom_getFullName, METH_VARARGS, NULL},
	{sipName_BALL_setPosition, sipDo_Atom_setPosition, METH_VARARGS, NULL},
	{sipName_BALL_getPosition, sipDo_Atom_getPosition, METH_VARARGS, NULL},
	{sipName_BALL_setRadius, sipDo_Atom_setRadius, METH_VARARGS, NULL},
	{sipName_BALL_getRadius, sipDo_Atom_getRadius, METH_VARARGS, NULL},
	{sipName_BALL_setType, sipDo_Atom_setType, METH_VARARGS, NULL},
	{sipName_BALL_getType, sipDo_Atom_getType, METH_VARARGS, NULL},
	{sipName_BALL_getTypeName, sipDo_Atom_getTypeName, METH_VARARGS, NULL},
	{sipName_BALL_setTypeName, sipDo_Atom_setTypeName, METH_VARARGS, NULL},
	{sipName_BALL_setVelocity, sipDo_Atom_setVelocity, METH_VARARGS, NULL},
	{sipName_BALL_getVelocity, sipDo_Atom_getVelocity, METH_VARARGS, NULL},
	{sipName_BALL_setForce, sipDo_Atom_setForce, METH_VARARGS, NULL},
	{sipName_BALL_getForce, sipDo_Atom_getForce, METH_VARARGS, NULL},
	{sipName_BALL_countBonds, sipDo_Atom_countBonds, METH_VARARGS, NULL},
	{sipName_BALL_getBond, sipDo_Atom_getBond, METH_VARARGS, NULL},
	{sipName_BALL_createBond, sipDo_Atom_createBond, METH_VARARGS, NULL},
	{sipName_BALL_cloneBond, sipDo_Atom_cloneBond, METH_VARARGS, NULL},
	{sipName_BALL_destroyBond, sipDo_Atom_destroyBond, METH_VARARGS, NULL},
	{sipName_BALL_destroyBonds, sipDo_Atom_destroyBonds, METH_VARARGS, NULL},
	{sipName_BALL_hasBond, sipDo_Atom_hasBond, METH_VARARGS, NULL},
	{sipName_BALL_isBondedTo, sipDo_Atom_isBondedTo, METH_VARARGS, NULL},
	{sipName_BALL_isBonded, sipDo_Atom_isBonded, METH_VARARGS, NULL},
	{sipName_BALL_isGeminal, sipDo_Atom_isGeminal, METH_VARARGS, NULL},
	{sipName_BALL_isVicinal, sipDo_Atom_isVicinal, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_Atom_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_Atom(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Atom);
}

void sipConvertTo_Atom(PyObject *sipPy,Atom **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_Atom);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Atom *)sipConvertToCpp(sipPy,sipClass_Atom,sipIsErr);
}

Atom *sipForceConvertTo_Atom(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Atom(valobj))
	{
		Atom *val;

		sipConvertTo_Atom(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Atom);
	*iserrp = 1;

	return NULL;
}
