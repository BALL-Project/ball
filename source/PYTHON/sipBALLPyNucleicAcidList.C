#include "sipBALLDeclBALL.h"
#include "sipBALLPyNucleicAcidList.h"



PyObject *sipConvertFrom_PyNucleicAcidList(const PyNucleicAcidList *sipCpp)
{
	if (sipCpp == NULL)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}

#line 10 "pyNucleicAcidList.sip"
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyNucleicAcidList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		NucleicAcid* obj = *it;
		PyObject *inst;

		if ((inst = sipMapCppToSelf(obj,sipClass_NucleicAcid)) == NULL || PyList_Append(pl,inst) < 0)
		{
			Py_DECREF(pl);
			return NULL;
		}
	}

	return pl;
#line 42 "./sipBALLPyNucleicAcidList.cpp"
}

PyObject *sipClass_PyNucleicAcidList;

int sipCanConvertTo_PyNucleicAcidList(PyObject *sipPy)
{
#line 35 "pyNucleicAcidList.sip"
	return PyList_Check(sipPy);
#line 51 "./sipBALLPyNucleicAcidList.cpp"
}

int sipConvertTo_PyNucleicAcidList(PyObject *sipPy,PyNucleicAcidList **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return false;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_PyNucleicAcidList);
		else
			*sipCppPtr = NULL;

		return false;
	}

#line 39 "pyNucleicAcidList.sip"
	// Convert a Python list of NucleicAcid instances to an NucleicAcidList object on the
	// heap.
 
	PyNucleicAcidList *nucleic_acid_list = new PyNucleicAcidList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		NucleicAcid* nucleic_acid;
 
		nucleic_acid = sipForceConvertTo_NucleicAcid(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete nucleic_acid_list;
			return 0;
		}
 
		nucleic_acid_list -> push_back(nucleic_acid);
	}
 
	*sipCppPtr = nucleic_acid_list;

	return 1;
#line 93 "./sipBALLPyNucleicAcidList.cpp"
}

PyNucleicAcidList *sipForceConvertTo_PyNucleicAcidList(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_PyNucleicAcidList(valobj))
	{
		PyNucleicAcidList *val;

		sipConvertTo_PyNucleicAcidList(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_PyNucleicAcidList);
	*iserrp = 1;

	return NULL;
}
