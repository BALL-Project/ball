// $Id: extractors.C,v 1.2 2000/06/27 13:10:17 oliver Exp $

#include <BALL/PYTHON/pyAtomList.h>
#include <BALL/PYTHON/pyPDBAtomList.h>
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
	PyAtomList* atoms(const BaseFragment& fragment, bool selected_only)
	{
		return new PyAtomList(fragment, selected_only);
	}

	PyPDBAtomList* PDBAtoms(const BaseFragment& fragment, bool selected_only)
	{
		return new PyPDBAtomList(fragment, selected_only);
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
