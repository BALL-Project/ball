// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyNucleotideList.sip,v 1.5 2003/12/18 19:03:21 oliver Exp $
//

%MappedType PyNucleotideList
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyNucleotideList.h>
	#include "sipBALLNucleotide.h"
  #include <BALL/PYTHON/pyBALLSipHelper.h>
%End

%ConvertFromTypeCode
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
%End
 
%ConvertToTypeCode
	// Convert a Python list of Nucleotide instances to an NucleotideList object on the
	// heap.
 
	PyNucleotideList *nucleotide_list = new PyNucleotideList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		Nucleotide* nucleotide;
 
		nucleotide = reinterpret_cast<Nucleotide*>(sipForceConvertTo_Nucleotide(PyList_GET_ITEM(sipPy,i),sipIsErr));
 
		if (*sipIsErr)
		{
			delete nucleotide_list;
			return 0;
		}
 
		nucleotide_list -> push_back(nucleotide);
	}
 
	*sipCppPtr = nucleotide_list;

	return 1;
%End
};

