#include "sipBALLDeclBALL.h"
#include "sipBALLProtein.h"



PyObject *sipClass_Protein;

static void sipDealloc_Protein(sipThisType *);
static PyObject *sipPyInternalRepr_Protein(sipThisType *);

static PyTypeObject sipType_Protein = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Protein,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Protein,
	0,
	0,
	0,
	0,
	(reprfunc)sipPyInternalRepr_Protein,
};

sipProtein::sipProtein(): Protein()
{
	sipCommonCtor(sipPyMethods,5);
}

sipProtein::sipProtein(const Protein& a0,bool a1): Protein(a0,a1)
{
	sipCommonCtor(sipPyMethods,5);
}

sipProtein::sipProtein(const String& a0,const String& a1): Protein(a0,a1)
{
	sipCommonCtor(sipPyMethods,5);
}

sipProtein::sipProtein(const Protein& a0): Protein(a0)
{
	sipCommonCtor(sipPyMethods,5);
}

sipProtein::~sipProtein()
{
	sipCommonDtor(sipPyThis);
}
void sipProtein::select()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[0],sipPyThis,NULL,sipName_BALL_select,&relLock))
		sipSelectable::sipVH_select(&sipPyMethods[0],sipPyThis,relLock);
	else
		Composite::select();
}
void sipProtein::deselect()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[1],sipPyThis,NULL,sipName_BALL_deselect,&relLock))
		sipSelectable::sipVH_deselect(&sipPyMethods[1],sipPyThis,relLock);
	else
		Composite::deselect();
}
void sipProtein::clear()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[2],sipPyThis,NULL,sipName_BALL_clear,&relLock))
		sipObject::sipVH_clear(&sipPyMethods[2],sipPyThis,relLock);
	else
		Protein::clear();
}
void sipProtein::destroy()
{
	int relLock;

	if (sipIsPyMethod(&sipPyMethods[3],sipPyThis,NULL,sipName_BALL_destroy,&relLock))
		sipObject::sipVH_destroy(&sipPyMethods[3],sipPyThis,relLock);
	else
		Protein::destroy();
}
bool sipProtein::isValid() const
{
	int relLock;

	return sipIsPyMethod((sipMethodCache *)&sipPyMethods[4],sipPyThis,NULL,sipName_BALL_isValid,&relLock) ?
		sipObject::sipVH_isValid(&sipPyMethods[4],sipPyThis,relLock) :
		Protein::isValid();
}

static PyObject *sipDo_Protein_clear(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Protein)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			ptr -> Protein::clear();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Protein,sipName_BALL_clear);

	return NULL;
}

static PyObject *sipDo_Protein_destroy(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Protein)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			ptr -> Protein::destroy();

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Protein,sipName_BALL_destroy);

	return NULL;
}

static PyObject *sipDo_Protein_set(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Protein)) == NULL)
		return NULL;

	{
		const Protein *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"I|l",sipCanConvertTo_Protein,&a0obj,&a1))
		{
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Protein(a0obj,(Protein **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Protein::set(* a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Protein,sipName_BALL_set);

	return NULL;
}

static PyObject *sipDo_Protein_get(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Protein)) == NULL)
		return NULL;

	{
		Protein *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"I|l",sipCanConvertTo_Protein,&a0obj,&a1))
		{
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Protein(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Protein::get(* a0, (bool)a1);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Protein,sipName_BALL_get);

	return NULL;
}

static PyObject *sipDo_Protein_swap(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Protein)) == NULL)
		return NULL;

	{
		Protein *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_Protein,&a0obj))
		{
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			int iserr = 0;

			sipConvertTo_Protein(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Protein::swap(* a0);

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Protein,sipName_BALL_swap);

	return NULL;
}

static PyObject *sipDo_Protein_getChain(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Protein)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(sipArgs,"i",&a0))
		{
			Chain *res;
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			res = ptr -> Protein::getChain( a0);

			return sipMapCppToSelf(res,sipClass_Chain);
		}
	}

	{
		int a0;

		if (sipParseArgs(sipArgs,"i",&a0))
		{
			const Chain *res;
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			res = ptr -> Protein::getChain( a0);

			return sipMapCppToSelf(res,sipClass_Chain);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Protein,sipName_BALL_getChain);

	return NULL;
}

static PyObject *sipDo_Protein_getSecondaryStructure(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Protein)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(sipArgs,"i",&a0))
		{
			SecondaryStructure *res;
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			res = ptr -> Protein::getSecondaryStructure( a0);

			return sipMapCppToSelf(res,sipClass_SecondaryStructure);
		}
	}

	{
		int a0;

		if (sipParseArgs(sipArgs,"i",&a0))
		{
			const SecondaryStructure *res;
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			res = ptr -> Protein::getSecondaryStructure( a0);

			return sipMapCppToSelf(res,sipClass_SecondaryStructure);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Protein,sipName_BALL_getSecondaryStructure);

	return NULL;
}

static PyObject *sipDo_Protein_getResidue(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Protein)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(sipArgs,"i",&a0))
		{
			Residue *res;
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			res = ptr -> Protein::getResidue( a0);

			return sipMapCppToSelf(res,sipClass_Residue);
		}
	}

	{
		int a0;

		if (sipParseArgs(sipArgs,"i",&a0))
		{
			const Residue *res;
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			res = ptr -> Protein::getResidue( a0);

			return sipMapCppToSelf(res,sipClass_Residue);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Protein,sipName_BALL_getResidue);

	return NULL;
}

static PyObject *sipDo_Protein_getNTerminal(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Protein)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Residue *res;
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			res = ptr -> Protein::getNTerminal();

			return sipMapCppToSelf(res,sipClass_Residue);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const Residue *res;
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			res = ptr -> Protein::getNTerminal();

			return sipMapCppToSelf(res,sipClass_Residue);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Protein,sipName_BALL_getNTerminal);

	return NULL;
}

static PyObject *sipDo_Protein_getCTerminal(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Protein)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			Residue *res;
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			res = ptr -> Protein::getCTerminal();

			return sipMapCppToSelf(res,sipClass_Residue);
		}
	}

	{
		if (sipParseArgs(sipArgs,""))
		{
			const Residue *res;
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			res = ptr -> Protein::getCTerminal();

			return sipMapCppToSelf(res,sipClass_Residue);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Protein,sipName_BALL_getCTerminal);

	return NULL;
}

static PyObject *sipDo_Protein_getPDBAtom(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Protein)) == NULL)
		return NULL;

	{
		int a0;

		if (sipParseArgs(sipArgs,"i",&a0))
		{
			PDBAtom *res;
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			res = ptr -> Protein::getPDBAtom( a0);

			return sipMapCppToSelf(res,sipClass_PDBAtom);
		}
	}

	{
		int a0;

		if (sipParseArgs(sipArgs,"i",&a0))
		{
			const PDBAtom *res;
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			res = ptr -> Protein::getPDBAtom( a0);

			return sipMapCppToSelf(res,sipClass_PDBAtom);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Protein,sipName_BALL_getPDBAtom);

	return NULL;
}

static PyObject *sipDo_Protein_setID(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Protein)) == NULL)
		return NULL;

	{
		const String *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"I",sipCanConvertTo_String,&a0obj))
		{
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			ptr -> Protein::setID(* a0);

			if (istemp0)
				delete a0;

			Py_INCREF(Py_None);
			return Py_None;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Protein,sipName_BALL_setID);

	return NULL;
}

static PyObject *sipDo_Protein_getID(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Protein)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			const String *res;
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			res = &ptr -> Protein::getID();

			return sipMapCppToSelf(res,sipClass_String);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Protein,sipName_BALL_getID);

	return NULL;
}

static PyObject *sipDo_Protein_countChains(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Protein)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			res = ptr -> Protein::countChains();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Protein,sipName_BALL_countChains);

	return NULL;
}

static PyObject *sipDo_Protein_countSecondaryStructures(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Protein)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			res = ptr -> Protein::countSecondaryStructures();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Protein,sipName_BALL_countSecondaryStructures);

	return NULL;
}

static PyObject *sipDo_Protein_countResidues(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Protein)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			res = ptr -> Protein::countResidues();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Protein,sipName_BALL_countResidues);

	return NULL;
}

static PyObject *sipDo_Protein_countPDBAtoms(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Protein)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			int res;
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			res = ptr -> Protein::countPDBAtoms();

			return PyInt_FromLong((long)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Protein,sipName_BALL_countPDBAtoms);

	return NULL;
}

static PyObject *sipDo_Protein_isValid(PyObject *sipThisObj,PyObject *sipArgs)
{
	sipThisType *sipThis;

	if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_Protein)) == NULL)
		return NULL;

	{
		if (sipParseArgs(sipArgs,""))
		{
			bool res;
			Protein *ptr;

			if ((ptr = (Protein *)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
				return NULL;

			res = ptr -> Protein::isValid();

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoMethod(sipName_BALL_Protein,sipName_BALL_isValid);

	return NULL;
}

// Cast a pointer to a type somewhere in its superclass hierachy.

const void *sipCast_Protein(const void *ptr,PyObject *targetClass)
{
	const void *res;

	if (targetClass == sipClass_Protein)
		return ptr;

	if ((res = sipCast_Molecule((Molecule *)(Protein *)ptr,targetClass)) != NULL)
		return res;

	return NULL;
}

static void sipDealloc_Protein(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (!sipIsSimple(sipThis))
			((sipProtein *)sipThis -> u.cppPtr) -> sipPyThis = NULL;

		if (sipIsPyOwned(sipThis))
			if (sipIsSimple(sipThis))
				delete (Protein *)sipThis -> u.cppPtr;
			else
				delete (sipProtein *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

static PyObject *sipPyInternalRepr_Protein(sipThisType *sipThis)
{
#line 47 "protein.sip"
  Protein* ptr;
  if ((ptr = (Protein*)sipGetCppPtr(sipThis,sipClass_Protein)) == NULL)
    return NULL;

  return PyString_FromString(String(String("Protein ") + ptr->getName() 
				+ " { " + String(ptr->countResidues()) + " residues }").c_str());
#line 813 "../CPP/sipBALLProtein.cpp"
}

PyObject *sipNew_Protein(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Protein
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
			sipNew = new sipProtein();
		}
	}

	if (sipNew == NULL)
	{
		const Protein *a0;
		PyObject *a0obj;
		long a1 = true;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_Protein,&a0obj,&a1))
		{
			int iserr = 0;

			sipConvertTo_Protein(a0obj,(Protein **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipProtein(* a0, (bool)a1);
		}
	}

	if (sipNew == NULL)
	{
		const String *a0;
		PyObject *a0obj;
		const String *a1 = NULL;
		PyObject *a1obj = NULL;

		if (sipParseArgs(sipArgs,"-I|I",sipCanConvertTo_String,&a0obj,sipCanConvertTo_String,&a1obj))
		{
			int iserr = 0;

			int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipProtein(* a0,* a1);

			if (istemp0)
				delete a0;

			if (istemp1)
				delete a1;
		}
	}

	if (sipNew == NULL)
	{
		const Protein *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_Protein,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Protein(a0obj,(Protein **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			sipNew = new sipProtein(* a0);
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipName_BALL_Protein);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Protein,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			if (sipFlags & SIP_SIMPLE)
				delete (Protein *)sipNew;
			else
				delete (sipProtein *)sipNew;

		return NULL;
	}

	if (!(sipFlags & SIP_SIMPLE))
		((sipProtein *)sipNew) -> sipPyThis = sipThis;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Protein[] = {
	{sipName_BALL_clear, sipDo_Protein_clear, METH_VARARGS, NULL},
	{sipName_BALL_destroy, sipDo_Protein_destroy, METH_VARARGS, NULL},
	{sipName_BALL_set, sipDo_Protein_set, METH_VARARGS, NULL},
	{sipName_BALL_get, sipDo_Protein_get, METH_VARARGS, NULL},
	{sipName_BALL_swap, sipDo_Protein_swap, METH_VARARGS, NULL},
	{sipName_BALL_getChain, sipDo_Protein_getChain, METH_VARARGS, NULL},
	{sipName_BALL_getSecondaryStructure, sipDo_Protein_getSecondaryStructure, METH_VARARGS, NULL},
	{sipName_BALL_getResidue, sipDo_Protein_getResidue, METH_VARARGS, NULL},
	{sipName_BALL_getNTerminal, sipDo_Protein_getNTerminal, METH_VARARGS, NULL},
	{sipName_BALL_getCTerminal, sipDo_Protein_getCTerminal, METH_VARARGS, NULL},
	{sipName_BALL_getPDBAtom, sipDo_Protein_getPDBAtom, METH_VARARGS, NULL},
	{sipName_BALL_setID, sipDo_Protein_setID, METH_VARARGS, NULL},
	{sipName_BALL_getID, sipDo_Protein_getID, METH_VARARGS, NULL},
	{sipName_BALL_countChains, sipDo_Protein_countChains, METH_VARARGS, NULL},
	{sipName_BALL_countSecondaryStructures, sipDo_Protein_countSecondaryStructures, METH_VARARGS, NULL},
	{sipName_BALL_countResidues, sipDo_Protein_countResidues, METH_VARARGS, NULL},
	{sipName_BALL_countPDBAtoms, sipDo_Protein_countPDBAtoms, METH_VARARGS, NULL},
	{sipName_BALL_isValid, sipDo_Protein_isValid, METH_VARARGS, NULL},
	{NULL}
};

int sipCanConvertTo_Protein(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Protein);
}

void sipConvertTo_Protein(PyObject *sipPy,Protein **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_Protein);
		else
			*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Protein *)sipConvertToCpp(sipPy,sipClass_Protein,sipIsErr);
}

Protein *sipForceConvertTo_Protein(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Protein(valobj))
	{
		Protein *val;

		sipConvertTo_Protein(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Protein);
	*iserrp = 1;

	return NULL;
}
