// $Id: extractors.h,v 1.2 2000/06/27 13:07:23 oliver Exp $

#ifndef BALL_PYTHON_EXTRACTORS_H
#define BALL_PYTHON_EXTRACTORS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{
	class PyAtomList;
	class PyPDBAtomList;
	class PyResidueList;
	class PyFragmentList;
	class PyMoleculeList;
	class PyProteinList;
	class PySecondaryStructureList;
	class PyChainList;
	class PyNucleotideList;
	class PyNucleicAcidList;
	class BaseFragment;

	/**	Extraction functions for external iteration.
			The following set of functions can by used from python to
			compile lists of objects from BALL kernel data structures.
			using the python {\tt for} loop, it is then possible to
			iterate over the objects of the list.
			\\
			For example, the BALL code
			\begin{verbatim}
			System S = ..;
			AtomIterator it;
			for (it = S.begin(); +it; ++it)
			{
				cout << it->getFullName();
			}
			\end{verbatim}
			can be rewritten using the se extractor functions in Python as:
			\begin{verbatim}
			S = System(...);
			for atom in atoms(S):
				print atom.getFullName()
			\end{verbatim}
			\\
			{\bf Definition:} \URL{BALL/PYTHON/extractors.h}
			\\
			{\bf Framework:} PYTHON scripting
	*/
	//@{
	/**	Extract all atoms from a kernel data structure.
	*/
	PyAtomList* atoms(const BaseFragment& fragment, bool selected_only = false);

	/**	Extract all PDB atoms from a kernel data structure.
	*/
	PyPDBAtomList* PDBAtoms(const BaseFragment& fragment, bool selected_only = false);

	/**	Extract all residues from a kernel data structure.
	*/
	PyResidueList* residues(const BaseFragment& fragment, bool selected_only = false);

	/**	Extract all fragments from a kernel data structure.
	*/
	PyFragmentList* fragments(const BaseFragment& fragment, bool selected_only = false);

	/**	Extract all molecules from a kernel data structure.
	*/
	PyMoleculeList* molecules(const BaseFragment& fragment, bool selected_only = false);

	/**	Extract all proteins from a kernel data structure.
	*/
	PyProteinList* proteins(const BaseFragment& fragment, bool selected_only = false);

	/**	Extract all secondary structures from a kernel data structure.
	*/
	PySecondaryStructureList* secondaryStructures(const BaseFragment& fragment, bool selected_only = false);

	/**	Extract all chains from a kernel data structure.
	*/
	PyChainList* chains(const BaseFragment& fragment, bool selected_only = false);

	/**	Extract all nucleic acids from a kernel data structure.
	*/
	PyNucleicAcidList* nucleicAcids(const BaseFragment& fragment, bool selected_only = false);

	/**	Extract all nucleotides from a kernel data structure.
	*/
	PyNucleotideList* nucleotides(const BaseFragment& fragment, bool selected_only = false);

	//@}
} // namespace BALL

#endif // BALL_PYTHON_EXTRACTORS_H
