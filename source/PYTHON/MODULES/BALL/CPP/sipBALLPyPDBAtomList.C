#include "sipBALLDeclBALL.h"
#include "sipBALLPyPDBAtomList.h"


int sipCanConvertTo_PyPDBAtomList(PyObject *sipPy)
{
#line 37 "pyPDBAtomList.sip"
	return PyList_Check(sipPy);
#line 14 "sipBALLPyPDBAtomList.cpp"
}

int sipConvertTo_PyPDBAtomList(PyObject *sipPy,PyPDBAtomList **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return 0;

#line 41 "pyPDBAtomList.sip"
	// Convert a Python list of PDBAtom instances to an PDBAtomList object on the
	// heap.
 
	PyPDBAtomList *PDB_atom_list = new PyPDBAtomList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		PDBAtom* PDB_atom;
 
		PDB_atom = sipForceConvertTo_PDBAtom(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete PDB_atom_list;
			return 0;
		}
 
		PDB_atom_list -> push_back(PDB_atom);
	}
 
	*sipCppPtr = PDB_atom_list;

	return 1;
#line 46 "sipBALLPyPDBAtomList.cpp"
}

PyPDBAtomList *sipForceConvertTo_PyPDBAtomList(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_PyPDBAtomList(valobj))
	{
		PyPDBAtomList *val;

		sipConvertTo_PyPDBAtomList(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_PyPDBAtomList);

	*iserrp = 1;

	return NULL;
}

PyObject *sipConvertFrom_PyPDBAtomList(const PyPDBAtomList *sipCpp)
{
#line 12 "pyPDBAtomList.sip"
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyPDBAtomList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		PDBAtom& obj = **it;
		PyObject *inst;

		if ((inst = pyMapBALLObjectToSip(obj)) == NULL || PyList_Append(pl,inst) < 0)
		{
			Py_DECREF(pl);
			return NULL;
		}
	}

	return pl;
#line 95 "sipBALLPyPDBAtomList.cpp"
}
