#include "sipBALLDeclBALL.h"
#include "sipBALLPyChainList.h"



PyObject *sipConvertFrom_PyChainList(const PyChainList *sipCpp)
{
	if (sipCpp == NULL)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}

#line 12 "pyChainList.sip"
	PyObject *pl;

	if ((pl = PyList_New(0)) == NULL)
	{
		return NULL;
	}

	// Convert the list.

	for (PyChainList::ConstIterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
	{
		Chain& obj = **it;
		PyObject *inst;

		if ((inst = pyMapBALLObjectToSip(obj)) == NULL || PyList_Append(pl,inst) < 0)
		{
			Py_DECREF(pl);
			return NULL;
		}
	}

	return pl;
#line 42 "../CPP/sipBALLPyChainList.cpp"
}

PyObject *sipClass_PyChainList;

int sipCanConvertTo_PyChainList(PyObject *sipPy)
{
#line 37 "pyChainList.sip"
	return PyList_Check(sipPy);
#line 51 "../CPP/sipBALLPyChainList.cpp"
}

int sipConvertTo_PyChainList(PyObject *sipPy,PyChainList **sipCppPtr,int sipNoNull,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return false;

	if (sipPy == Py_None)
	{
		if (sipNoNull)
			sipNullArgument(sipName_BALL_PyChainList);
		else
			*sipCppPtr = NULL;

		return false;
	}

#line 41 "pyChainList.sip"
	// Convert a Python list of Chain instances to an ChainList object on the
	// heap.
 
	PyChainList *chain_list = new PyChainList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		Chain* chain;
 
		chain = sipForceConvertTo_Chain(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete chain_list;
			return 0;
		}
 
		chain_list -> push_back(chain);
	}
 
	*sipCppPtr = chain_list;

	return 1;
#line 93 "../CPP/sipBALLPyChainList.cpp"
}

PyChainList *sipForceConvertTo_PyChainList(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_PyChainList(valobj))
	{
		PyChainList *val;

		sipConvertTo_PyChainList(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_PyChainList);
	*iserrp = 1;

	return NULL;
}
