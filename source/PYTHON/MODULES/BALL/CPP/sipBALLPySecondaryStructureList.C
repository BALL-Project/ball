#include "sipBALLDeclBALL.h"
#include "sipBALLPySecondaryStructureList.h"



PyObject *sipConvertFrom_PySecondaryStructureList(const PySecondaryStructureList *sipCpp)
{
	if (sipCpp == NULL)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}

#line 12 "pySecondaryStructureList.sip"
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PySecondaryStructureList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		SecondaryStructure& obj = **it;
		PyObject *inst;

		if ((inst = pyMapBALLObjectToSip(obj)) == NULL || PyList_Append(pl,inst) < 0)
		{
			Py_DECREF(pl);
			return NULL;
		}
	}

	return pl;
#line 42 "sipBALLPySecondaryStructureList.cpp"
}

PyObject *sipClass_PySecondaryStructureList;

int sipCanConvertTo_PySecondaryStructureList(PyObject *sipPy)
{
#line 37 "pySecondaryStructureList.sip"
	return PyList_Check(sipPy);
#line 51 "sipBALLPySecondaryStructureList.cpp"
}

int sipConvertTo_PySecondaryStructureList(PyObject *sipPy,PySecondaryStructureList **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return 0;

#line 41 "pySecondaryStructureList.sip"
	// Convert a Python list of SecondaryStructure instances to an SecondaryStructureList object on the
	// heap.
 
	PySecondaryStructureList *sec_struc_list = new PySecondaryStructureList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		SecondaryStructure* sec_struc;
 
		sec_struc = sipForceConvertTo_SecondaryStructure(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete sec_struc_list;
			return 0;
		}
 
		sec_struc_list -> push_back(sec_struc);
	}
 
	*sipCppPtr = sec_struc_list;

	return 1;
#line 83 "sipBALLPySecondaryStructureList.cpp"
}

PySecondaryStructureList *sipForceConvertTo_PySecondaryStructureList(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_PySecondaryStructureList(valobj))
	{
		PySecondaryStructureList *val;

		sipConvertTo_PySecondaryStructureList(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_PySecondaryStructureList);
	*iserrp = 1;

	return NULL;
}
