// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: forEach.h,v 1.16 2004/02/18 23:24:04 oliver Exp $
//

#ifndef BALL_KERNEL_FOREACH_H
#define BALL_KERNEL_FOREACH_H

// This is mostly to allow Doxygen to put the macros to where they belong
namespace BALL
{

/** @name Convenient iteration macros
* 	 \ingroup KernelMiscellaneous 
*/

//@{

/** Molecule iterator macro.
    Convenience macro for easy iteration over all molecules of {\em molecule_container}.
   	@param   molecule_container container of  \link BALL::Molecule Molecule \endlink  instances
    @param   molecule_iterator  \link BALL::MoleculeIterator MoleculeIterator \endlink  instance
    @see     Molecule
    @see     MoleculeIterator
*/
#define BALL_FOREACH_MOLECULE(molecule_container, molecule_iterator) \
for (molecule_iterator = (molecule_container).beginMolecule(); \
     !molecule_iterator.isEnd(); ++molecule_iterator)


/** Fragment iterator macro.
    Convenience macro for easy iteration over all fragments of {\em fragment_container}.
    @param   fragment_container container of  \link BALL::Fragment Fragment \endlink  instances
    @param   fragment_iterator  \link BALL::FragmentIterator FragmentIterator \endlink  instance
    @see     Fragment
    @see     FragmentIterator
*/
#define BALL_FOREACH_FRAGMENT(fragment_container, fragment_iterator) \
for (fragment_iterator = (fragment_container).beginFragment(); \
     !fragment_iterator.isEnd(); ++fragment_iterator)


/** AtomContainer iterator macro.
    Convenience macro for easy iteration over all atom containers of {\em atom_container}.
    @param   atom_containers container of  \link BALL::AtomContainer AtomContainer \endlink  instances
    @param   atom_container_iterator  \link BALL::AtomContainerIterator AtomContainerIterator \endlink  instance
    @see     AtomContainer
    @see     AtomContainerIterator
*/
#define BALL_FOREACH_ATOMCONTAINER(atom_containers, atom_container_iterator) \
for (atom_container_iterator = (atom_containers).beginAtomContainer(); \
     !atom_container_iterator.isEnd(); ++atom_container_iterator)


/** Atom iterator macro.
    Convenience macro for easy iteration over all atoms of {\em atom_container}.
    @param   atom_container container of  \link BALL::Atom Atom \endlink  instances
    @param   atom_iterator  \link BALL::AtomIterator AtomIterator \endlink  instance
    @see     Atom
    @see     AtomIterator
*/
#define BALL_FOREACH_ATOM(atom_container, atom_iterator) \
for (atom_iterator = (atom_container).beginAtom(); \
     !atom_iterator.isEnd(); ++atom_iterator)


/** Atom bond iterator macro.
    Convenience macro for easy iteration over all bonds of {\em bond_container}.
    @param   bond_container  \link BALL::Atom Atom \endlink  instance container of  \link BALL::Bond Bond \endlink  instances
    @param   bond_iterator  \link BALL::AtomBondIterator AtomBondIterator \endlink  instance
    @see     Atom
    @see     Bond
    @see     AtomBondIterator
*/
#define BALL_FOREACH_ATOM_BOND(bond_container, bond_iterator) \
for (bond_iterator = (bond_container).beginBond(); \
     !bond_iterator.isEnd(); ++bond_iterator)


/** Atom pair iterator macro.
    Convenience macro for easy iteration over all atom pairs of {\em atom_container}.
    @param   atom_container container of  \link BALL::Atom Atom \endlink  instances
    @param   first_atom_iterator first  \link BALL::AtomIterator AtomIterator \endlink  instance
    @param   second_atom_iterator second  \link BALL::AtomIterator AtomIterator \endlink  instance
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
    @param   atom_container container of  \link BALL::Atom Atom \endlink  instances
    @param   atom_iterator  \link BALL::AtomIterator AtomIterator \endlink  instance which is the container of  \link BALL::Bond Bond \endlink  instances
    @param   bond_iterator  \link BALL::AtomBondIterator AtomBondIterator \endlink  instance
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
    @param   atom_container container of  \link BALL::Atom Atom \endlink  instances
    @param   atom_iterator  \link BALL::AtomIterator AtomIterator \endlink  instance which is the container of  \link BALL::Bond Bond \endlink  instances
    @param   bond_iterator  \link BALL::AtomBondIterator AtomBondIterator \endlink  instance
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
    @param   atom_container container of  \link BALL::Atom Atom \endlink  instances
    @param   atom_iterator  \link BALL::AtomIterator AtomIterator \endlink  instance which is the container of  \link BALL::Bond Bond \endlink  instances
    @param   bond_iterator  \link BALL::AtomBondIterator AtomBondIterator \endlink  instance
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
    @param   protein_container container of  \link BALL::Protein Protein \endlink  instances
    @param   protein_iterator  \link BALL::ProteinIterator ProteinIterator \endlink  instance
    @see     Protein
*/
#define BALL_FOREACH_PROTEIN(protein_container, protein_iterator) \
for (protein_iterator = (protein_container).beginProtein(); \
     !protein_iterator.isEnd(); ++protein_iterator)


/** Chain iterator macro.
    Convenience macro for easy iteration over all chains of {\em chain_container}.
    @param   chain_container container of  \link BALL::Chain Chain \endlink  instances
    @param   chain_iterator  \link BALL::ChainIterator ChainIterator \endlink  instance
*/
#define BALL_FOREACH_CHAIN(chain_container, chain_iterator) \
for (chain_iterator = (chain_container).beginChain(); \
     !chain_iterator.isEnd(); ++chain_iterator)


/** SecondaryStructure iterator macro.
    Convenience macro for easy iteration over all secondary structures 
		of {\em secondary_structure_container}.
    @param   secondary_structure_container container of  \link BALL::SecondaryStructure SecondaryStructure \endlink  instances
    @param   secondary_structure_iterator  \link BALL::SecondaryStructureIterator SecondaryStructureIterator \endlink  instance
*/
#define BALL_FOREACH_SECONDARYSTRUCTURE(secondary_structure_container, secondary_structure_iterator) \
for (secondary_structure_iterator = (secondary_structure_container).beginSecondaryStructure(); \
     !secondary_structure_iterator.isEnd(); ++secondary_structure_iterator)


/** Residue iterator macro.
    Convenience macro for easy iteration over all residues of {\em residue_container}.
    @param   residue_container container of  \link BALL::Residue Residue \endlink  instances
    @param   residue_iterator  \link BALL::ResidueIterator ResidueIterator \endlink  instance
*/
#define BALL_FOREACH_RESIDUE(residue_container, residue_iterator) \
for (residue_iterator = (residue_container).beginResidue(); \
     !residue_iterator.isEnd(); \
     ++residue_iterator)


/** PDBAtom iterator macro.
    Convenience macro for easy iteration over all  \link BALL::PDBAtom PDBAtom \endlink s of {\em pdb_atom_container}.
    @param   pdb_atom_container container of  \link BALL::PDBAtom PDBAtom \endlink  instances
    @param   pdb_atom_iterator  \link BALL::PDBAtomIterator PDBAtomIterator \endlink  instance
*/
#define BALL_FOREACH_PDBATOM(pdb_atom_container, pdb_atom_iterator) \
for (pdb_atom_iterator = (pdb_atom_container).beginPDBAtom(); \
     !pdb_atom_iterator.isEnd(); ++pdb_atom_iterator)

//@}

} // namespace BALL

#endif // BALL_KERNEL_FOREACH_H
