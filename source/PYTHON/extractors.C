// $Id: extractors.C,v 1.5 2000/07/12 19:35:14 oliver Exp $

#include <BALL/PYTHON/pyAtomList.h>
#include <BALL/PYTHON/pyPDBAtomList.h>
#include <BALL/PYTHON/pyBondList.h>
#include <BALL/PYTHON/pyBaseFragmentList.h>
#include <BALL/PYTHON/pyFragmentList.h>
#include <BALL/PYTHON/pyMoleculeList.h>
#include <BALL/PYTHON/pyResidueList.h>
#include <BALL/PYTHON/pySecondaryStructureList.h>
#include <BALL/PYTHON/pyChainList.h>
#include <BALL/PYTHON/pyProteinList.h>
#include <BALL/PYTHON/pyNucleotideList.h>
#include <BALL/PYTHON/pyNucleicAcidList.h>

#include <BALL/KERNEL/baseFragment.h>

namespace BALL
{
	PyAtomList* atoms(const BaseFragment& fragment, const String& expression)
	{
		return new PyAtomList(fragment, expression);
	}

	PyPDBAtomList* PDBAtoms(const BaseFragment& fragment, const String& expression)
	{
		return new PyPDBAtomList(fragment, expression);
	}

	PyAtomList* atoms(const BaseFragment& fragment)
	{
		return new PyAtomList(fragment);
	}

	PyPDBAtomList* PDBAtoms(const BaseFragment& fragment)
	{
		return new PyPDBAtomList(fragment);
	}

	PyBondList* bonds(const BaseFragment& fragment, bool selected_only)
	{
		return new PyBondList(fragment, selected_only);
	}

	PyBondList* bonds(const Atom& atom)
	{
		return new PyBondList(atom);
	}

	PyBaseFragmentList* baseFragments(const BaseFragment& fragment, bool selected_only)
	{
		return new PyBaseFragmentList(fragment, selected_only);
	}

	PyFragmentList* fragments(const BaseFragment& fragment, bool selected_only)
	{
		return new PyFragmentList(fragment, selected_only);
	}

	PyMoleculeList* molecules(const BaseFragment& fragment, bool selected_only)
	{
		return new PyMoleculeList(fragment, selected_only);
	}

	PyResidueList* residues(const BaseFragment& fragment, bool selected_only)
	{
		return new PyResidueList(fragment, selected_only);
	}

	PySecondaryStructureList* secondaryStructures(const BaseFragment& fragment, bool selected_only)
	{
		return new PySecondaryStructureList(fragment, selected_only);
	}

	PyChainList* chains(const BaseFragment& fragment, bool selected_only)
	{
		return new PyChainList(fragment, selected_only);
	}

	PyProteinList* proteins(const BaseFragment& fragment, bool selected_only)
	{
		return new PyProteinList(fragment, selected_only);
	}

	PyNucleotideList* nucleotides(const BaseFragment& fragment, bool selected_only)
	{
		return new PyNucleotideList(fragment, selected_only);
	}

	PyNucleicAcidList* nucleicAcids(const BaseFragment& fragment, bool selected_only)
	{
		return new PyNucleicAcidList(fragment, selected_only);
	}

}
