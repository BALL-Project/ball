#include "sipBALLDeclBALL.h"
#include "sipBALLPyProteinList.h"


int sipCanConvertTo_PyProteinList(PyObject *sipPy)
{
#line 37 "pyProteinList.sip"
	return PyList_Check(sipPy);
#line 14 "sipBALLPyProteinList.cpp"
}

int sipConvertTo_PyProteinList(PyObject *sipPy,PyProteinList **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return 0;

#line 41 "pyProteinList.sip"
	// Convert a Python list of Protein instances to an ProteinList object on the
	// heap.
 
	PyProteinList *protein_list = new PyProteinList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		Protein* protein;
 
		protein = sipForceConvertTo_Protein(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete protein_list;
			return 0;
		}
 
		protein_list -> push_back(protein);
	}
 
	*sipCppPtr = protein_list;

	return 1;
#line 46 "sipBALLPyProteinList.cpp"
}

PyProteinList *sipForceConvertTo_PyProteinList(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_PyProteinList(valobj))
	{
		PyProteinList *val;

		sipConvertTo_PyProteinList(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_PyProteinList);

	*iserrp = 1;

	return NULL;
}

PyObject *sipConvertFrom_PyProteinList(const PyProteinList *sipCpp)
{
#line 12 "pyProteinList.sip"
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyProteinList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		Protein& obj = **it;
		PyObject *inst;

		if ((inst = pyMapBALLObjectToSip(obj)) == NULL || PyList_Append(pl,inst) < 0)
		{
			Py_DECREF(pl);
			return NULL;
		}
	}

	return pl;
#line 95 "sipBALLPyProteinList.cpp"
}
