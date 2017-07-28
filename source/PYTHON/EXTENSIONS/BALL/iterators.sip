class AtomIterator
{
%TypeHeaderCode
	#include <BALL/KERNEL/atomIterator.h>
%End
	AtomIterator& __iter__();
%MethodCode
	sipRes = sipCpp;
%End

	Atom& __next__();
%MethodCode
	if(sipCpp->isEnd())
	{
		PyErr_SetNone(PyExc_StopIteration);
		return NULL;
	}

	sipRes = &**sipCpp;

	++*sipCpp;
%End
};

class FragmentIterator
{
%TypeHeaderCode
	#include <BALL/KERNEL/fragmentIterator.h>
%End
	FragmentIterator& __iter__();
%MethodCode
	sipRes = sipCpp;
%End

	Fragment& __next__();
%MethodCode
	if(sipCpp->isEnd())
	{
		PyErr_SetNone(PyExc_StopIteration);
		return NULL;
	}

	sipRes = &**sipCpp;

	++*sipCpp;
%End
};

class ChainIterator
{
%TypeHeaderCode
	#include <BALL/KERNEL/chainIterator.h>
%End
	ChainIterator& __iter__();
%MethodCode
	sipRes = sipCpp;
%End

	Chain& __next__();
%MethodCode
	if(sipCpp->isEnd())
	{
		PyErr_SetNone(PyExc_StopIteration);
		return NULL;
	}

	sipRes = &**sipCpp;

	++*sipCpp;
%End
};

class ResidueIterator
{
%TypeHeaderCode
	#include <BALL/KERNEL/residueIterator.h>
%End
	ResidueIterator& __iter__();
%MethodCode
	sipRes = sipCpp;
%End

	Residue& __next__();
%MethodCode
	if(sipCpp->isEnd())
	{
		PyErr_SetNone(PyExc_StopIteration);
		return NULL;
	}

	sipRes = &**sipCpp;

	++*sipCpp;
%End
};

class MoleculeIterator
{
%TypeHeaderCode
	#include <BALL/KERNEL/moleculeIterator.h>
%End
	MoleculeIterator& __iter__();
%MethodCode
	sipRes = sipCpp;
%End

	Molecule& __next__();
%MethodCode
	if(sipCpp->isEnd())
	{
		PyErr_SetNone(PyExc_StopIteration);
		return NULL;
	}

	sipRes = &**sipCpp;

	++*sipCpp;
%End
};

class ProteinIterator
{
%TypeHeaderCode
	#include <BALL/KERNEL/proteinIterator.h>
%End
	ProteinIterator& __iter__();
%MethodCode
	sipRes = sipCpp;
%End

	Protein& __next__();
%MethodCode
	if(sipCpp->isEnd())
	{
		PyErr_SetNone(PyExc_StopIteration);
		return NULL;
	}

	sipRes = &**sipCpp;

	++*sipCpp;
%End
};

class PDBAtomIterator
{
%TypeHeaderCode
	#include <BALL/KERNEL/atomIterator.h>
%End
	PDBAtomIterator& __iter__();
%MethodCode
	sipRes = sipCpp;
%End

	PDBAtom& __next__();
%MethodCode
	if(sipCpp->isEnd())
	{
		PyErr_SetNone(PyExc_StopIteration);
		return NULL;
	}

	sipRes = &**sipCpp;

	++*sipCpp;
%End
};

class SecondaryStructureIterator
{
%TypeHeaderCode
	#include <BALL/KERNEL/secondaryStructureIterator.h>
%End
	SecondaryStructureIterator& __iter__();
%MethodCode
	sipRes = sipCpp;
%End

	SecondaryStructure& __next__();
%MethodCode
	if(sipCpp->isEnd())
	{
		PyErr_SetNone(PyExc_StopIteration);
		return NULL;
	}

	sipRes = &**sipCpp;

	++*sipCpp;
%End
};
