// $Id: extractors.C,v 1.1 2001/09/03 16:53:07 oliver Exp $

#include <BALL/PYTHON/pyAtomList.h>
#include <BALL/PYTHON/pyPDBAtomList.h>
#include <BALL/PYTHON/pyBondList.h>
#include <BALL/PYTHON/pyAtomContainerList.h>
#include <BALL/PYTHON/pyFragmentList.h>
#include <BALL/PYTHON/pyMoleculeList.h>
#include <BALL/PYTHON/pyResidueList.h>
#include <BALL/PYTHON/pySecondaryStructureList.h>
#include <BALL/PYTHON/pyChainList.h>
#include <BALL/PYTHON/pyProteinList.h>
#include <BALL/PYTHON/pyNucleotideList.h>
#include <BALL/PYTHON/pyNucleicAcidList.h>

#include <BALL/KERNEL/atomContainer.h>

namespace BALL
{
	PyAtomList* atoms(const AtomContainer& fragment, const String& expression)
	{
		return new PyAtomList(fragment, expression);
	}

	PyPDBAtomList* PDBAtoms(const AtomContainer& fragment, const String& expression)
	{
		return new PyPDBAtomList(fragment, expression);
	}

	PyAtomList* atoms(const AtomContainer& fragment)
	{
		return new PyAtomList(fragment);
	}

	PyPDBAtomList* PDBAtoms(const AtomContainer& fragment)
	{
		return new PyPDBAtomList(fragment);
	}

	PyBondList* bonds(const AtomContainer& fragment, bool selected_only)
	{
		return new PyBondList(fragment, selected_only);
	}

	PyBondList* bonds(const Atom& atom)
	{
		return new PyBondList(atom);
	}

	PyAtomContainerList* atomContainers(const AtomContainer& fragment, bool selected_only)
	{
		return new PyAtomContainerList(fragment, selected_only);
	}

	PyFragmentList* fragments(const AtomContainer& fragment, bool selected_only)
	{
		return new PyFragmentList(fragment, selected_only);
	}

	PyMoleculeList* molecules(const AtomContainer& fragment, bool selected_only)
	{
		return new PyMoleculeList(fragment, selected_only);
	}

	PyResidueList* residues(const AtomContainer& fragment, bool selected_only)
	{
		return new PyResidueList(fragment, selected_only);
	}

	PySecondaryStructureList* secondaryStructures(const AtomContainer& fragment, bool selected_only)
	{
		return new PySecondaryStructureList(fragment, selected_only);
	}

	PyChainList* chains(const AtomContainer& fragment, bool selected_only)
	{
		return new PyChainList(fragment, selected_only);
	}

	PyProteinList* proteins(const AtomContainer& fragment, bool selected_only)
	{
		return new PyProteinList(fragment, selected_only);
	}

	PyNucleotideList* nucleotides(const AtomContainer& fragment, bool selected_only)
	{
		return new PyNucleotideList(fragment, selected_only);
	}

	PyNucleicAcidList* nucleicAcids(const AtomContainer& fragment, bool selected_only)
	{
		return new PyNucleicAcidList(fragment, selected_only);
	}

}
