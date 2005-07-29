// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: extractors.h,v 1.17.6.1 2005/07/29 12:37:54 amoll Exp $
//

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
			using the python <tt>for</tt> loop, it is then possible to
			iterate over the objects of the list.	 \par
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
		\ingroup PythonExtensions		
	*/
	//@{

	/**	Extract atoms matching an expression.
			This method extracts all atoms of a kernel data structure into
			a list that match the  \link Expression Expression \endlink  <tt>expression</tt>.
			@param fragment the AtomContainer containing the atoms
			@param expression the expression that selects the atoms
	*/
	BALL_EXPORT PyAtomList* atoms(const AtomContainer& fragment, const String& expression);
			
	/**	Extract all atoms.
			This method extracts all atoms of a kernel data structure into a list. 
			@param fragment the AtomContainer containing the atoms
	*/
	BALL_EXPORT PyAtomList* atoms(const AtomContainer& fragment);

	/**	Extract atoms matching an expression.
			This method extracts all atoms from an atom list  
      that match the \link Expression Expression \endlink  <tt>expression</tt>.
			@param atoms the AtomContainer containing the atoms
			@param expression the expression that selects the atoms
	*/
	BALL_EXPORT PyAtomList* atoms(const PyAtomList& atoms, const String& expression);
			
	/**	Extract PDB atoms matching an expression.
			This method extracts all PDB atoms of a kernel data structure into
			a list that match the  \link Expression Expression \endlink  <tt>expression</tt>.
			@param fragment the AtomContainer containing the atoms
			@param expression the expression that selects the atoms
	*/
	BALL_EXPORT PyPDBAtomList* PDBAtoms(const AtomContainer& fragment, const String& expression);
			
	/**	Extract all PDB atoms.
			This method extracts all PDB atoms of a kernel data structure into
			a list. 
			@param fragment the AtomContainer containing the atoms
	*/
	BALL_EXPORT PyPDBAtomList* PDBAtoms(const AtomContainer& fragment);

	/**	Extract all bonds from a kernel data structure.
			This function extracts all bonds from the atoms contained in the base fragment.
			If <tt>selected_only</tt> is set to <b>true</b>, only bonds are extracted where
			both atoms are selected.
	*/
	BALL_EXPORT PyBondList* bonds(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all bonds from an atom.
			This function returns all bonds of an atom.
	*/
	BALL_EXPORT PyBondList* bonds(const Atom& atom);

	/**	Extract all base fragments from a kernel data structure.
	*/
	BALL_EXPORT PyAtomContainerList* atomContainers(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all residues from a kernel data structure.
	*/
	BALL_EXPORT PyResidueList* residues(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all fragments from a kernel data structure.
	*/
	BALL_EXPORT PyFragmentList* fragments(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all molecules from a kernel data structure.
	*/
	BALL_EXPORT PyMoleculeList* molecules(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all proteins from a kernel data structure.
	*/
	BALL_EXPORT PyProteinList* proteins(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all secondary structures from a kernel data structure.
	*/
	BALL_EXPORT PySecondaryStructureList* secondaryStructures(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all chains from a kernel data structure.
	*/
	BALL_EXPORT PyChainList* chains(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all nucleic acids from a kernel data structure.
	*/
	BALL_EXPORT PyNucleicAcidList* nucleicAcids(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all nucleotides from a kernel data structure.
	*/
	BALL_EXPORT PyNucleotideList* nucleotides(const AtomContainer& fragment, bool selected_only = false);

	//@}
   
} // namespace BALL

#endif // BALL_PYTHON_EXTRACTORS_H
