// $Id: forEach.h,v 1.3 2000/04/14 13:35:07 amoll Exp $

#ifndef BALL_KERNEL_FOREACH_H
#define BALL_KERNEL_FOREACH_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

/** @name Convenient iteration macros
*/
//@{

/** Molecule iterator macro.
    Convenience macro for easy iteration over all molecules of {\em molecule_container}.\\

    {\bf Framework:} BALL/KERNEL\\
    {\bf Definition:} \URL{BALL/KERNEL/forEach.h}
		\\
    {\bf Category:} \Ref{Molecule} iterator\\
    {\bf Concept:} iterator design pattern\\
  	{\bf Implementation notes:} Macro is implemented by one \Ref{MoleculeIterator} instance.\\
    {\bf Complexity:} O(n), n:="number of \Ref{Composite} instances in {\em molecule_container}"\\

    @param   molecule_container container of \Ref{Molecule} instances
    @param   molecule_iterator \Ref{MoleculeIterator} instance
    @see     Molecule
    @see     MoleculeIterator
*/
#define BALL_FOREACH_MOLECULE(molecule_container, molecule_iterator) \
for (molecule_iterator = (molecule_container).beginMolecule(); \
     !molecule_iterator.isEnd(); ++molecule_iterator)


      
/** Fragment iterator macro.
    Convenience macro for easy iteration over all fragments of {\em fragment_container}.\\

		{\bf Framework:} BALL/KERNEL\\
		{\bf Definition:} \URL{BALL/KERNEL/forEach.h}
		\\
		{\bf Category:} \Ref{Fragment} iterator\\
    {\bf Concept:} iterator design pattern\\
		{\bf Implementation notes:} Macro is implemented by one \Ref{FragmentIterator} instance.\\
    {\bf Complexity:} O(n), n:="number of \Ref{Composite} instances in {\em fragment_container}"\\

    @param   fragment_container container of \Ref{Fragment} instances
    @param   fragment_iterator \Ref{FragmentIterator} instance
    @see     Fragment
    @see     FragmentIterator
*/
#define BALL_FOREACH_FRAGMENT(fragment_container, fragment_iterator) \
for (fragment_iterator = (fragment_container).beginFragment(); \
     !fragment_iterator.isEnd(); ++fragment_iterator)



/** BaseFragment iterator macro.
    Convenience macro for easy iteration over all base fragments of {\em basefragment_container}.\\

		{\bf Framework:} BALL/KERNEL\\
		{\bf Definition:} \URL{BALL/KERNEL/forEach.h}
		\\
		{\bf Category:} \Ref{BaseFragment} iterator\\
    {\bf Concept:} iterator design pattern\\
		{\bf Implementation notes:} Macro is implemented by one \Ref{BaseFragmentIterator} instance.\\
    {\bf Complexity:} O(n), n:="number of \Ref{Composite} instances in {\em base_fragment_container}"\\

    @param   base_fragment_container container of \Ref{BaseFragment} instances
    @param   base_fragment_iterator \Ref{BaseFragmentIterator} instance
    @see     BaseFragment
    @see     BaseFragmentIterator
*/
#define BALL_FOREACH_BASEFRAGMENT(base_fragment_container, base_fragment_iterator) \
for (base_fragment_iterator = (base_fragment_container).beginBaseFragment(); \
     !base_fragment_iterator.isEnd(); ++base_fragment_iterator)



/** Atom iterator macro.
    Convenience macro for easy iteration over all atoms of {\em atom_container}.

		{\bf Framework:} BALL/KERNEL\\
    {\bf Definition:} \URL{BALL/KERNEL/foreach.h}
		\\
		{\bf Category:} \Ref{Atom} iterator\\
		{\bf Concept:} iterator design pattern\\
		{\bf Implementation notes:} Macro is implemented by one \Ref{AtomIterator} instance.\\
		{\bf Complexity:} O(n), n:="number of \Ref{Composite} instances in {\em atom_container}"\\

    @param   atom_container container of \Ref{Atom} instances
    @param   atom_iterator \Ref{AtomIterator} instance
    @see     Atom
    @see     AtomIterator
*/
#define BALL_FOREACH_ATOM(atom_container, atom_iterator) \
for (atom_iterator = (atom_container).beginAtom(); \
     !atom_iterator.isEnd(); ++atom_iterator)



/** Atom bond iterator macro.
    Convenience macro for easy iteration over all bonds of {\em bond_container}.\\

		{\bf Framework:} BALL/KERNEL\\
		{\bf Definition:} \URL{BALL/KERNEL/forEach.h}
		\\
		{\bf Category:} \Ref{Bond} iterator\\
		{\bf Concept:} iterator design pattern\\
		{\bf Implementation notes:} Macro is implemented by one \Ref{AtomBondIterator} instance.\\
		{\bf Complexity:} O(1)="max. 8 \Ref{Bond} instances in {\em bond_container}"\\

    @param   bond_container \Ref{Atom} instance container of \Ref{Bond} instances
    @param   atom_iterator \Ref{AtomBondIterator} instance
    @see     Atom
    @see     Bond
    @see     AtomBondIterator
*/
#define BALL_FOREACH_ATOM_BOND(bond_container, bond_iterator) \
for (bond_iterator = (bond_container).beginBond(); \
     !bond_iterator.isEnd(); ++bond_iterator)



/** Atom pair iterator macro.
    Convenience macro for easy iteration over all atom pairs of {\em atom_container}.\\

		{\bf Framework:} BALL/KERNEL\\
		{\bf Definition:} \URL{BALL/KERNEL/forEach.h}
		{\bf Category:} \Ref{Atom} iterator\\
		{\bf Concept:} iterator design pattern\\
		{\bf Implementation notes:} Macro is implemented by two \Ref{AtomIterator} instances. All atom pairs are unique. The linear order of the \Ref{Atom} instances determines the pairing.\\
		{\bf Complexity:} O(n), n:="number of \Ref{Composite} instances in {\em atom_container}"\\

    @param   atom_container container of \Ref{Atom} instances
    @param   first_atom_iterator first \Ref{AtomIterator} instance
    @param   second_atom_iterator second \Ref{AtomIterator} instance
    @see     Atom
    @see     AtomIterator
*/
#define BALL_FOREACH_ATOM_PAIR(atom_container, first_atom_iterator, second_atom_iterator) \
for (first_atom_iterator = (atom_container).beginAtom(); \
     !first_atom_iterator.isEnd(); ++first_atom_iterator) \
  for (second_atom_iterator = (atom_container).beginAtom(); \
       !second_atom_iterator.isEnd(); ++second_atom_iterator) \
    if ((*first_atom_iterator) < (*second_atom_iterator))



/** Bond iterator macro.
    Convenience macro for easy iteration over all bonds of {\em atom_container} that is also a bond-container.\\

		{\bf Framework:} BALL/KERNEL\\
		{\bf Definition:} \URL{BALL/KERNEL/forEach.h}
		\\
		{\bf Category:} \Ref{Bond} iterator\\
		{\bf Concept:} iterator design pattern\\
		{\bf Implementation notes:} Macro is implemented by one \Ref{AtomIterator} instance and one \Ref{AtomBondIterator} instance.\\
		{\bf Complexity:} O(n), n:="number of \Ref{Composite} instances in {\em atom_container}"\\

    @param   atom_container container of \Ref{Atom} instances
    @param   atom_iterator \Ref{AtomIterator} instance which is the container of \Ref{Bond} instances
    @param   bond_iterator \Ref{AtomBondIterator} instance
    @see     Atom
    @see     Bond
    @see     AtomIterator
    @see     AtomBondIterator
*/
#define BALL_FOREACH_BOND(atom_container, atom_iterator, bond_iterator) \
for (atom_iterator = (atom_container).beginAtom(); \
     !atom_iterator.isEnd(); ++atom_iterator) \
  for (bond_iterator = (*atom_iterator).beginBond(); \
       !bond_iterator.isEnd(); ++bond_iterator) \
    if ((*bond_iterator).getFirstAtom() == &(*atom_iterator) \
				|| !(atom_container).isAncestorOf(*(*bond_iterator).getFirstAtom()))

      

/** Intermolecular bond iterator macro.
    Convenience macro for easy iteration over all intermolecular bonds of {\em atom_container} that is also a bond-container.\\

		{\bf Framework:} BALL/KERNEL\\
		{\bf Definition:} \URL{BALL/KERNEL/forEach.h}
		\\
		{\bf Category:} \Ref{Bond} iterator\\
		{\bf Concept:} iterator design pattern\\
		{\bf Implementation notes:} Macro is implemented by one \Ref{AtomIterator} instance and one \Ref{AtomBondIterator} instance.\\
		{\bf Complexity:} O(n), n:="number of \Ref{Composite} instances in {\em atom_container}"\\

    @param   atom_container container of \Ref{Atom} instances
    @param   atom_iterator \Ref{AtomIterator} instance which is the container of \Ref{Bond} instances
    @param   bond_iterator \Ref{AtomBondIterator} instance
    @see     Atom
    @see     Bond
    @see     AtomIterator
    @see     AtomBondIterator
*/
#define BALL_FOREACH_INTERBOND(atom_container, atom_iterator, bond_iterator) \
for (atom_iterator = (atom_container).beginAtom(); \
     !atom_iterator.isEnd(); ++atom_iterator) \
  for (bond_iterator = (*atom_iterator).beginBond(); \
       !bond_iterator.isEnd(); ++bond_iterator) \
    if (((*bond_iterator).getFirstAtom() == &(*atom_iterator) \
				 && !(atom_container).isAncestorOf(*(*bond_iterator).getSecondAtom())) \
				 || !(atom_container).isAncestorOf(*(*bond_iterator).getFirstAtom()))



/** Intramolecular bond iterator macro.
    Convenience macro for easy iteration over all intramolecular (internal) bonds of {\em atom_container} that is also a bond-container.\\

		{\bf Framework:} BALL/KERNEL\\
		{\bf Definition:} \URL{BALL/KERNEL/forEach.h}
		\\
		{\bf Category:} \Ref{Bond} iterator\\
		{\bf Concept:} iterator design pattern\\
		{\bf Implementation notes:} Macro is implemented by one \Ref{AtomIterator} instance and one \Ref{AtomBondIterator} instance.\\
		{\bf Complexity:} O(n), n:="number of \Ref{Composite} instances in {\em atom_container}"\\

    @param   atom_container container of \Ref{Atom} instances
    @param   atom_iterator \Ref{AtomIterator} instance which is the container of \Ref{Bond} instances
    @param   bond_iterator \Ref{AtomBondIterator} instance
    @see     Atom
    @see     Bond
    @see     AtomIterator
    @see     AtomBondIterator
*/
#define BALL_FOREACH_INTRABOND(atom_container, atom_iterator, bond_iterator) \
for (atom_iterator = (atom_container).beginAtom(); \
     !atom_iterator.isEnd(); ++atom_iterator) \
  for (bond_iterator = (*atom_iterator).beginBond(); \
       !bond_iterator.isEnd(); ++bond_iterator) \
    if ((*bond_iterator).getFirstAtom() == &(*atom_iterator) \
				&& (atom_container).isAncestorOf(*(*bond_iterator).getSecondAtom()))


      
/** Protein iterator macro.
    Convenience macro for easy iteration over all proteins of {\em protein_container}.\\

		{\bf Framework:} BALL/KERNEL\\
		{\bf Definition:} \URL{BALL/KERNEL/forEach.h}
		\\
		{\bf Category:} \Ref{Protein} iterator\\
		{\bf Concept:} iterator design pattern\\
		{\bf Implementation notes:} Macro is implemented by one \Ref{ProteinIterator} instance.\\
		{\bf Complexity:} O(n), n:="number of \Ref{Composite} instances in {\em protein_container}"\\

    @param   protein_container container of \Ref{Protein} instances
    @param   protein_iterator \Ref{ProteinIterator} instance
    @see     Protein
    @see     ProteinIterator
*/
#define BALL_FOREACH_PROTEIN(protein_container, protein_iterator) \
for (protein_iterator = (protein_container).beginProtein(); \
     !protein_iterator.isEnd(); ++protein_iterator)


      
/** Chain iterator macro.
    Convenience macro for easy iteration over all chains of {\em chain_container}.\\

		{\bf Framework:} BALL/KERNEL\\
		{\bf Definition:} \URL{BALL/KERNEL/forEach.h}
		\\
		{\bf Category:} \Ref{Chain} iterator\\
		{\bf Concept:} iterator design pattern\\
		{\bf Implementation notes:} Macro is implemented by one \Ref{ChainIterator} instance.\\
		{\bf Complexity:} O(n), n:="number of \Ref{Composite} instances in {\em chain_container}"\\

    @param   chain_container container of \Ref{Chain} instances
    @param   chain_iterator \Ref{ChainIterator} instance
    @see     Chain
    @see     ChainIterator
*/
#define BALL_FOREACH_CHAIN(chain_container, chain_iterator) \
for (chain_iterator = (chain_container).beginChain(); \
     !chain_iterator.isEnd(); ++chain_iterator)


      
/** SecondaryStructure iterator macro.
    Convenience macro for easy iteration over all secondary structures of {\em secondary_structure_container}.\\

		{\bf Framework:} BALL/KERNEL\\
		{\bf Definition:} \URL{BALL/KERNEL/forEach.h}
		\\
		{\bf Category:} \Ref{SecondaryStructure} iterator\\
		{\bf Concept:} iterator design pattern\\
		{\bf Implementation notes:} Macro is implemented by one \Ref{SecondaryStructureIterator} instance.\\
		{\bf Complexity:} O(n), n:="number of \Ref{Composite} instances in {\em secondary_structure_container}"\\

    @param   secondary_structure_container container of \Ref{SecondaryStructure} instances
    @param   secondary_structure_iterator \Ref{SecondaryStructureIterator} instance
    @see     SecondaryStructure
    @see     SecondaryStructureIterator
*/
#define BALL_FOREACH_SECONDARYSTRUCTURE(secondary_structure_container, secondary_structure_iterator) \
for (secondary_structure_iterator = (secondary_structure_container).beginSecondaryStructure(); \
     !secondary_structure_iterator.isEnd(); ++secondary_structure_iterator)


  
/** Residue iterator macro.
    Convenience macro for easy iteration over all residues of {\em residue_container}.\\

		{\bf Framework:} BALL/KERNEL\\
		{\bf Definition:} \URL{BALL/KERNEL/forEach.h}
		\\
		{\bf Category:} \Ref{Residue} iterator\\
		{\bf Concept:} iterator design pattern\\
		{\bf Implementation notes:} Macro is implemented by one \Ref{ResidueIterator} instance.\\
		{\bf Complexity:} O(n), n:="number of \Ref{Composite} instances in {\em residue_container}"\\

    @param   residue_container container of \Ref{Residue} instances
    @param   residue_iterator \Ref{ResidueIterator} instance
    @see     Residue
    @see     ResidueIterator
*/
#define BALL_FOREACH_RESIDUE(residue_container, residue_iterator) \
for (residue_iterator = (residue_container).beginResidue(); \
     !residue_iterator.isEnd(); \
     ++residue_iterator)



/** PDBAtom iterator macro.
    Convenience macro for easy iteration over all \Ref{PDBAtom}s of {\em pdb_atom_container}.\\

		{\bf Framework:} BALL/KERNEL\\
		{\bf Definition:} \URL{BALL/KERNEL/forEach.h}
		\\
		{\bf Category:} \Ref{PDBAtom} iterator\\
		{\bf Concept:} iterator design pattern\\
		{\bf Implementation notes:} Macro is implemented by one \Ref{PDBAtomIterator} instance.\\
		{\bf Complexity:} O(n), n:="number of \Ref{Composite} instances in {\em pdb_atom_container}"\\

    @param   pdb_atom_container container of \Ref{PDBAtom} instances
    @param   pdb_atom_iterator \Ref{PDBAtomIterator} instance
    @see     PDBAtom
    @see     PDBAtomIterator
*/
#define BALL_FOREACH_PDBATOM(pdb_atom_container, pdb_atom_iterator) \
for (pdb_atom_iterator = (pdb_atom_container).beginPDBAtom(); \
     !pdb_atom_iterator.isEnd(); ++pdb_atom_iterator)

//@}

#endif // BALL_KERNEL_FOREACH_H
