// $Id: extractors.sip,v 1.1 2000/07/18 21:27:32 oliver Exp $

%HeaderCode
	#include <BALL/PYTHON/extractors.h>
	using namespace BALL;
%End
PyAtomList* 								atoms(const BaseFragment& /fragment/, const String& /expression/);
PyAtomList*									atoms(const BaseFragment& /fragment/);
PyPDBAtomList* 							PDBAtoms(const BaseFragment& /fragment/, const String& /expression/);
PyPDBAtomList* 							PDBAtoms(const BaseFragment& /fragment/);
PyBondList* 								bonds(const BaseFragment& /fragment/, bool /selected_only/ = false);
PyBondList* 								bonds(const Atom& /atom/);
PyBaseFragmentList* 				baseFragments(const BaseFragment& /fragment/, bool /selected_only/ = false);
PyFragmentList* 						fragments(const BaseFragment& /fragment/, bool /selected_only/ = false);
PyMoleculeList* 						molecules(const BaseFragment& /fragment/, bool /selected_only/ = false);
PyResidueList* 							residues(const BaseFragment& /fragment/, bool /selected_only/ = false);
PySecondaryStructureList* 	secondaryStructures(const BaseFragment& /fragment/, bool /selected_only/ = false);
PyChainList* 								chains(const BaseFragment& /fragment/, bool /selected_only/ = false);
PyProteinList* 							proteins(const BaseFragment& /fragment/, bool /selected_only/ = false);
PyNucleotideList* 					nucleotides(const BaseFragment& /fragment/, bool /selected_only/ = false);
PyNucleicAcidList* 					nucleicAcids(const BaseFragment& /fragment/, bool /selected_only/ = false);

