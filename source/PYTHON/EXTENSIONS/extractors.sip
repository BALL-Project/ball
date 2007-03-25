// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: extractors.sip,v 1.7.8.1 2007/03/25 21:37:08 oliver Exp $
//
// Author:
//   Oliver Kohlbacher
//

%ModuleHeaderCode
	#include <BALL/KERNEL/extractors.h>
	using namespace BALL;
%End
<<<<<<< extractors.sip
PyAtomList 								atoms(const AtomContainer& /fragment/, const String& /expression/ = String());
PyAtomList 								atoms(const PyAtomList& /atoms/, const String& /expression/);
//PyAtomList	 							PDBAtoms(const AtomContainer& /fragment/, const String& /expression/ = String());
//BondList* 								bonds(const AtomContainer& /fragment/, bool /selected_only/ = false);
//BondList* 								bonds(const Atom& /atom/);
//AtomContainerList* 				atomContainers(const AtomContainer& /fragment/, bool /selected_only/ = false);
//FragmentList* 						fragments(const AtomContainer& /fragment/, bool /selected_only/ = false);
//MoleculeList* 						molecules(const AtomContainer& /fragment/, bool /selected_only/ = false);
//ResidueList* 							residues(const AtomContainer& /fragment/, bool /selected_only/ = false);
//SecondaryStructureList* 	secondaryStructures(const AtomContainer& /fragment/, bool /selected_only/ = false);
//ChainList* 								chains(const AtomContainer& /fragment/, bool /selected_only/ = false);
//ProteinList*							proteins(const AtomContainer& /fragment/, bool /selected_only/ = false);
//NucleotideList* 					nucleotides(const AtomContainer& /fragment/, bool /selected_only/ = false);
//NucleicAcidList* 					nucleicAcids(const AtomContainer& /fragment/, bool /selected_only/ = false);
//
//
=======
PyAtomList* 								atoms(const AtomContainer& /fragment/, const String& /expression/);
PyAtomList*									atoms(const AtomContainer& /fragment/);
PyAtomList* 								atoms(const PyAtomList& /atoms/, const String& /expression/);
PyAtomList*									all_atoms(const Composite& /fragment/);
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
PyProteinList*							proteins(const AtomContainer& /fragment/, bool /selected_only/ = false);
PyNucleotideList* 					nucleotides(const AtomContainer& /fragment/, bool /selected_only/ = false);
PyNucleicAcidList* 					nucleicAcids(const AtomContainer& /fragment/, bool /selected_only/ = false);

>>>>>>> 1.6.10.1
