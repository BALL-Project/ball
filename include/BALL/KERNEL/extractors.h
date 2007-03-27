// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: extractors.h,v 1.2.8.3 2007/03/27 22:41:31 amoll Exp $
//
// Author:
//   Oliver Kohlbacher
//

#ifndef BALL_KERNEL_EXTRACTORS_H
#define BALL_KERNEL_EXTRACTORS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

namespace BALL 
{

	class Atom;
	class PDBAtom;
	class Bond;
	class AtomContainer;
	class Fragment;
	class Molecule;
	class Residue;
	class SecondaryStructure;
	class Chain;
	class Protein;
	class Nucleotide;
	class NucleicAcid;

	/**	Kernel object list classes.
			These classes can be used to hold pointers to 
			kernel objects. They are returned by kernel extractors.
			These lists are a convenient way to execute operations
			on subsets of kernel structures and an often convenient,
			albeit slower, alternative to iterators.
			\p
			They are just convenient tyepdefs, so they will behave exactly
			as any STL list. In order to create these lists from 
			kernel objects, use \link extractors extractors \endlink.
			\ingroup KernelMiscellaneous
	*/
	//@{
	///
	typedef std::list<Atom*> AtomList;
	///
	typedef std::list<Bond*> BondList;
	///
	typedef std::list<AtomContainer*> AtomContainerList;
	///
	typedef std::list<PDBAtom*> PDBAtomList;
	///
	typedef std::list<Residue*> ResidueList;
	///
	typedef std::list<Fragment*> FragmentList;
	///
	typedef std::list<Molecule*> MoleculeList;
	///
	typedef std::list<Protein*> ProteinList;
	///
	typedef std::list<SecondaryStructure*> SecondaryStructureList;
	///
	typedef std::list<Chain*> ChainList;
	///
	typedef std::list<Nucleotide*> NucleotideList;
	///
	typedef std::list<NucleicAcid*> NucleicAcidList;

	//@}
   
	/**	Extraction functions for external iteration.
			The following set of functions can by used from python to
			compile lists of objects from BALL kernel data structures.

			\ingroup KernelMiscellaneous
	*/
	//@{

	/**	Extract atoms matching an expression.
			This method extracts all atoms of a kernel data structure into
			a list that match the  \link Expression Expression \endlink  <tt>expression</tt>.
			If no expression is given, all atoms will be extracted.
			@param fragment the AtomContainer containing the atoms.
			@param expression the expression that selects the atoms (default: no expression)
	*/
	BALL_EXPORT AtomList atoms(const AtomContainer& fragment, const String& expression = String());
			
	/**	Extract atoms matching an expression.
			This method extracts all atoms from an atom list  
      that match the \link Expression Expression \endlink  <tt>expression</tt>.
			@param atoms the AtomContainer containing the atoms
			@param expression the expression that selects the atoms
	*/
	BALL_EXPORT AtomList atoms(const AtomList& atoms, const String& expression);
			
	/**	Extract PDB atoms matching an expression.
			This method extracts all PDB atoms of a kernel data structure into
			a list that match the  \link Expression Expression \endlink  <tt>expression</tt>.
			If no expression is given, all PDB atoms will be returned.
			@param fragment the AtomContainer containing the atoms
			@param expression the expression that selects the atoms (default: no expression)
	*/
	BALL_EXPORT PDBAtomList PDBAtoms(const AtomContainer& fragment, const String& expression = String());
			
	/**	Extract all bonds from a kernel data structure.
			This function extracts all bonds from the atoms contained in the base fragment.
			If <tt>selected_only</tt> is set to <b>true</b>, only bonds are extracted where
			both atoms are selected.
	*/
	BALL_EXPORT BondList bonds(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all bonds from an atom.
			This function returns all bonds of an atom.
	*/
	BALL_EXPORT BondList bonds(const Atom& atom);

	/**	Extract all base fragments from a kernel data structure.
	*/
	BALL_EXPORT AtomContainerList atomContainers(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all residues from a kernel data structure.
	*/
	BALL_EXPORT ResidueList residues(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all fragments from a kernel data structure.
	*/
	BALL_EXPORT FragmentList fragments(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all molecules from a kernel data structure.
	*/
	BALL_EXPORT MoleculeList molecules(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all proteins from a kernel data structure.
	*/
	BALL_EXPORT ProteinList proteins(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all secondary structures from a kernel data structure.
	*/
	BALL_EXPORT SecondaryStructureList secondaryStructures(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all chains from a kernel data structure.
	*/
	BALL_EXPORT ChainList chains(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all nucleic acids from a kernel data structure.
	*/
	BALL_EXPORT NucleicAcidList nucleicAcids(const AtomContainer& fragment, bool selected_only = false);

	/**	Extract all nucleotides from a kernel data structure.
	*/
	BALL_EXPORT NucleotideList nucleotides(const AtomContainer& fragment, bool selected_only = false);

	//@}
	
} // namespace BALL

#endif // BALL_KERNEL_EXTRACTORS_H
