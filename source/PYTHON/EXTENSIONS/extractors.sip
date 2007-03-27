// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: extractors.sip,v 1.7.8.4 2007/03/27 21:25:46 amoll Exp $
//
// Author:
//   Oliver Kohlbacher
//

%ModuleHeaderCode
	#include <BALL/KERNEL/extractors.h>
	using namespace BALL;
%End

AtomList  								atoms(const AtomContainer& /fragment/, const String& /expression/ = String());
//AtomList 									all_atoms(const Composite& /fragment/);
PDBAtomList  							PDBAtoms(const AtomContainer& /fragment/, const String& /expression/ = String());
BondList  								bonds(const AtomContainer& /fragment/, bool /selected_only/ = false);
BondList  								bonds(const Atom& /atom/);
AtomContainerList  				atomContainers(const AtomContainer& /fragment/, bool /selected_only/ = false);
FragmentList  						fragments(const AtomContainer& /fragment/, bool /selected_only/ = false);
MoleculeList  						molecules(const AtomContainer& /fragment/, bool /selected_only/ = false);
ResidueList  							residues(const AtomContainer& /fragment/, bool /selected_only/ = false);
SecondaryStructureList  	secondaryStructures(const AtomContainer& /fragment/, bool /selected_only/ = false);
ChainList  								chains(const AtomContainer& /fragment/, bool /selected_only/ = false);
ProteinList 							proteins(const AtomContainer& /fragment/, bool /selected_only/ = false);
NucleotideList  					nucleotides(const AtomContainer& /fragment/, bool /selected_only/ = false);
NucleicAcidList  					nucleicAcids(const AtomContainer& /fragment/, bool /selected_only/ = false);

