#include "sipBALLDeclBALL.h"
#include "sipBALLPyNucleotideList.h"



PyObject *sipConvertFrom_PyNucleotideList(const PyNucleotideList *sipCpp)
{
	if (sipCpp == NULL)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}

#line 12 "pyNucleotideList.sip"
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyNucleotideList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		Nucleotide& obj = **it;
		PyObject *inst;

		if ((inst = pyMapBALLObjectToSip(obj)) == NULL || PyList_Append(pl,inst) < 0)
		{
			Py_DECREF(pl);
			return NULL;
		}
	}

	return pl;
#line 42 "sipBALLPyNucleotideList.cpp"
}

PyObject *sipClass_PyNucleotideList;

int sipCanConvertTo_PyNucleotideList(PyObject *sipPy)
{
#line 37 "pyNucleotideList.sip"
	return PyList_Check(sipPy);
#line 51 "sipBALLPyNucleotideList.cpp"
}

int sipConvertTo_PyNucleotideList(PyObject *sipPy,PyNucleotideList **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return 0;

#line 41 "pyNucleotideList.sip"
	// Convert a Python list of Nucleotide instances to an NucleotideList object on the
	// heap.
 
	PyNucleotideList *nucleotide_list = new PyNucleotideList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		Nucleotide* nucleotide;
 
		nucleotide = sipForceConvertTo_Nucleotide(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete nucleotide_list;
			return 0;
		}
 
		nucleotide_list -> push_back(nucleotide);
	}
 
	*sipCppPtr = nucleotide_list;

	return 1;
#line 83 "sipBALLPyNucleotideList.cpp"
}

PyNucleotideList *sipForceConvertTo_PyNucleotideList(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_PyNucleotideList(valobj))
	{
		PyNucleotideList *val;

		sipConvertTo_PyNucleotideList(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_PyNucleotideList);
	*iserrp = 1;

	return NULL;
}
