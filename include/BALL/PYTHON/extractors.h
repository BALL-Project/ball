// $Id: extractors.h,v 1.8 2001/07/16 14:49:29 amoll Exp $

#ifndef BALL_PYTHON_EXTRACTORS_H
#define BALL_PYTHON_EXTRACTORS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{
	class PyAtomList;
	class PyBondList;
	class PyAtomContainerList;
	class PyPDBAtomList;
	class PyResidueList;
	class PyFragmentList;
	class PyMoleculeList;
	class PyProteinList;
	class PySecondaryStructureList;
	class PyChainList;
	class PyNucleotideList;
	class PyNucleicAcidList;
	class AtomContainer;
	class Atom;

	/**	Extraction functions for external iteration.
			The following set of functions can by used from python to
			compile lists of objects from BALL kernel data structures.
			using the python {\tt for} loop, it is then possible to
			iterate over the objects of the list.	\\
			For example, the BALL code
			\begin{verbatim}
				System S = ..;
				AtomIterator it;
				for (it = S.begin(); +it; ++it)
				{
					cout << it->getFullName();
				}
			\end{verbatim}
			can be rewritten using the extractor functions in Python as:
			\begin{verbatim}
				S = System(...);
				for atom in atoms(S):
					print atom.getFullName()
			\end{verbatim}
			{\bf Definition:} \URL{BALL/PYTHON/extractors.h}
	*/
	//@{

	/**	Extract atoms matching an expression.
			This method extracts all atoms of a kernel data structure into
			a list that match the \Ref{Expression} {\tt expression}.
			@param fragment the AtomContainer containing the atoms
			@param expression the expression that selects the atoms
	*/
	PyAtomList* atoms(const AtomContainer& fragment, const String& expression);
			
	/**	Extract all atoms.
			This method extracts all atoms of a kernel data structure into a list. 
			@param fragment the AtomContainer containing the atoms
	*/
	PyAtomList* atoms(const AtomContainer& fragment);

	/**	Extract PDB atoms matching an expression.
			This method extracts all PDB atoms of a kernel data structure into
			a list that match the \Ref{Expression} {\tt expression}.
			@param fragment the AtomContainer containing the atoms
			@param expression the expression that selects the atoms
	*/
	PyPDBAtomList* PDBAtoms(const AtomContainer& fragment, const String& expression);
			
	/**	Extract all PDB atoms.
			This method extracts all PDB atoms of a kernel data structure into
			a list. 
			@param fragment the AtomContainer containing the atoms
	*/
	PyPDBAtomList* PDBAtoms(const AtomContainer& fragment);

	/**	Extract all bonds from a kernel data structure.
			This function extracts all bonds from the atoms contained in the base fragment.
			If {\tt selected_only} is set to {\bf true}, only bonds are extracted where
			both atoms are selected.
	*/
	PyBondList* bonds(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all bonds from an atom.
			This function returns all bonds of an atom.
	*/
	PyBondList* bonds(const Atom& atom);

	/**	Extract all base fragments from a kernel data structure.
	*/
	PyAtomContainerList* atomContainers(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all residues from a kernel data structure.
	*/
	PyResidueList* residues(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all fragments from a kernel data structure.
	*/
	PyFragmentList* fragments(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all molecules from a kernel data structure.
	*/
	PyMoleculeList* molecules(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all proteins from a kernel data structure.
	*/
	PyProteinList* proteins(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all secondary structures from a kernel data structure.
	*/
	PySecondaryStructureList* secondaryStructures(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all chains from a kernel data structure.
	*/
	PyChainList* chains(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all nucleic acids from a kernel data structure.
	*/
	PyNucleicAcidList* nucleicAcids(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all nucleotides from a kernel data structure.
	*/
	PyNucleotideList* nucleotides(const AtomContainer& fragment, bool selected_only = false);

	//@}

} // namespace BALL

#endif // BALL_PYTHON_EXTRACTORS_H
