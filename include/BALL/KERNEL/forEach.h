// $Id: forEach.h,v 1.7 2001/06/29 19:58:37 amoll Exp $

#ifndef BALL_KERNEL_FOREACH_H
#define BALL_KERNEL_FOREACH_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

/** @name Convenient iteration macros
*/
//@{

/** Molecule iterator macro.
    Convenience macro for easy iteration over all molecules of {\em molecule_container}.
   	@param   molecule_container container of \Ref{Molecule} instances
    @param   molecule_iterator \Ref{MoleculeIterator} instance
    @see     Molecule
    @see     MoleculeIterator
*/
#define BALL_FOREACH_MOLECULE(molecule_container, molecule_iterator) \
for (molecule_iterator = (molecule_container).beginMolecule(); \
     !molecule_iterator.isEnd(); ++molecule_iterator)


/** Fragment iterator macro.
    Convenience macro for easy iteration over all fragments of {\em fragment_container}.
    @param   fragment_container container of \Ref{Fragment} instances
    @param   fragment_iterator \Ref{FragmentIterator} instance
    @see     Fragment
    @see     FragmentIterator
*/
#define BALL_FOREACH_FRAGMENT(fragment_container, fragment_iterator) \
for (fragment_iterator = (fragment_container).beginFragment(); \
     !fragment_iterator.isEnd(); ++fragment_iterator)


/** AtomContainer iterator macro.
    Convenience macro for easy iteration over all atom containers of {\em atom_container}.
    @param   atom_containers container of \Ref{AtomContainer} instances
    @param   atom_container_iterator \Ref{AtomContainerIterator} instance
    @see     AtomContainer
    @see     AtomContainerIterator
*/
#define BALL_FOREACH_ATOMCONTAINER(atom_containers, atom_container_iterator) \
for (atom_container_iterator = (atom_containers).beginAtomContainer(); \
     !atom_container_iterator.isEnd(); ++atom_container_iterator)


/** Atom iterator macro.
    Convenience macro for easy iteration over all atoms of {\em atom_container}.
    @param   atom_container container of \Ref{Atom} instances
    @param   atom_iterator \Ref{AtomIterator} instance
    @see     Atom
    @see     AtomIterator
*/
#define BALL_FOREACH_ATOM(atom_container, atom_iterator) \
for (atom_iterator = (atom_container).beginAtom(); \
     !atom_iterator.isEnd(); ++atom_iterator)


/** Atom bond iterator macro.
    Convenience macro for easy iteration over all bonds of {\em bond_container}.
    @param   bond_container \Ref{Atom} instance container of \Ref{Bond} instances
    @param   bond_iterator \Ref{AtomBondIterator} instance
    @see     Atom
    @see     Bond
    @see     AtomBondIterator
*/
#define BALL_FOREACH_ATOM_BOND(bond_container, bond_iterator) \
for (bond_iterator = (bond_container).beginBond(); \
     !bond_iterator.isEnd(); ++bond_iterator)


/** Atom pair iterator macro.
    Convenience macro for easy iteration over all atom pairs of {\em atom_container}.
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
    Convenience macro for easy iteration over all bonds of all atoms in {\em atom_container}.
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
    Convenience macro for easy iteration over all 
		intermolecular bonds of atoms in {\em atom_container}.
    @param   atom_container container of \Ref{Atom} instances
    @param   atom_iterator \Ref{AtomIterator} instance which is the container of \Ref{Bond} instances
    @param   bond_iterator \Ref{AtomBondIterator} instance
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
    Convenience macro for easy iteration over all 
		intramolecular (internal) bonds of {\em atom_container}.
    @param   atom_container container of \Ref{Atom} instances
    @param   atom_iterator \Ref{AtomIterator} instance which is the container of \Ref{Bond} instances
    @param   bond_iterator \Ref{AtomBondIterator} instance
*/
#define BALL_FOREACH_INTRABOND(atom_container, atom_iterator, bond_iterator) \
for (atom_iterator = (atom_container).beginAtom(); \
     !atom_iterator.isEnd(); ++atom_iterator) \
  for (bond_iterator = (*atom_iterator).beginBond(); \
       !bond_iterator.isEnd(); ++bond_iterator) \
    if ((*bond_iterator).getFirstAtom() == &(*atom_iterator) \
				&& (atom_container).isAncestorOf(*(*bond_iterator).getSecondAtom()))

      
/** Protein iterator macro.
    Convenience macro for easy iteration over all proteins of {\em protein_container}.
    @param   protein_container container of \Ref{Protein} instances
    @param   protein_iterator \Ref{ProteinIterator} instance
    @see     Protein
*/
#define BALL_FOREACH_PROTEIN(protein_container, protein_iterator) \
for (protein_iterator = (protein_container).beginProtein(); \
     !protein_iterator.isEnd(); ++protein_iterator)


/** Chain iterator macro.
    Convenience macro for easy iteration over all chains of {\em chain_container}.
    @param   chain_container container of \Ref{Chain} instances
    @param   chain_iterator \Ref{ChainIterator} instance
*/
#define BALL_FOREACH_CHAIN(chain_container, chain_iterator) \
for (chain_iterator = (chain_container).beginChain(); \
     !chain_iterator.isEnd(); ++chain_iterator)


/** SecondaryStructure iterator macro.
    Convenience macro for easy iteration over all secondary structures 
		of {\em secondary_structure_container}.
    @param   secondary_structure_container container of \Ref{SecondaryStructure} instances
    @param   secondary_structure_iterator \Ref{SecondaryStructureIterator} instance
*/
#define BALL_FOREACH_SECONDARYSTRUCTURE(secondary_structure_container, secondary_structure_iterator) \
for (secondary_structure_iterator = (secondary_structure_container).beginSecondaryStructure(); \
     !secondary_structure_iterator.isEnd(); ++secondary_structure_iterator)


/** Residue iterator macro.
    Convenience macro for easy iteration over all residues of {\em residue_container}.
    @param   residue_container container of \Ref{Residue} instances
    @param   residue_iterator \Ref{ResidueIterator} instance
*/
#define BALL_FOREACH_RESIDUE(residue_container, residue_iterator) \
for (residue_iterator = (residue_container).beginResidue(); \
     !residue_iterator.isEnd(); \
     ++residue_iterator)


/** PDBAtom iterator macro.
    Convenience macro for easy iteration over all \Ref{PDBAtom}s of {\em pdb_atom_container}.
    @param   pdb_atom_container container of \Ref{PDBAtom} instances
    @param   pdb_atom_iterator \Ref{PDBAtomIterator} instance
*/
#define BALL_FOREACH_PDBATOM(pdb_atom_container, pdb_atom_iterator) \
for (pdb_atom_iterator = (pdb_atom_container).beginPDBAtom(); \
     !pdb_atom_iterator.isEnd(); ++pdb_atom_iterator)

//@}

#endif // BALL_KERNEL_FOREACH_H
