// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: extractors.sip,v 1.4 2003/11/15 10:59:06 oliver Exp $
//

%ModuleHeaderCode
	#include <BALL/PYTHON/extractors.h>
	using namespace BALL;
%End
PyAtomList* 								atoms(const AtomContainer& /fragment/, const String& /expression/);
PyAtomList*									atoms(const AtomContainer& /fragment/);
PyAtomList* 								atoms(const PyAtomList& /atoms/, const String& /expression/);
PyAtomList*									atoms(const PyAtomList& /atoms/);
PyPDBAtomList* 							PDBAtoms(const AtomContainer& /fragment/, const String& /expression/);
PyPDBAtomList* 							PDBAtoms(const AtomContainer& /fragment/);
PyBondList* 								bonds(const AtomContainer& /fragment/, bool /selected_only/ = false);
PyBondList* 								bonds(const Atom& /atom/);
PyAtomContainerList* 				atomContainers(const AtomContainer& /fragment/, bool /selected_only/ = false);
PyFragmentList* 						fragments(const AtomContainer& /fragment/, bool /selected_only/ = false);
PyMoleculeList* 						molecules(const AtomContainer& /fragment/, bool /selected_only/ = false);
PyResidueList* 							residues(const AtomContainer& /fragment/, bool /selected_only/ = false);
PySecondaryStructureList* 	secondaryStructures(const AtomContainer& /fragment/, bool /selected_only/ = false);
PyChainList* 								chains(const AtomContainer& /fragment/, bool /selected_only/ = false);
PyProteinList* 							proteins(const AtomContainer& /fragment/, bool /selected_only/ = false);
PyNucleotideList* 					nucleotides(const AtomContainer& /fragment/, bool /selected_only/ = false);
PyNucleicAcidList* 					nucleicAcids(const AtomContainer& /fragment/, bool /selected_only/ = false);

