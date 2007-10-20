// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: extractors.C,v 1.2.8.2 2007/03/27 21:16:34 amoll Exp $
//
// Author:
//   Oliver Kohlbacher
//

#include <BALL/KERNEL/extractors.h>

#include <BALL/KERNEL/expression.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/nucleicAcid.h>

namespace BALL
{

	AtomList atoms(const AtomContainer& fragment, const String& expression)
	{
		AtomList result;

    // iterate over all atoms
		AtomConstIterator it = fragment.beginAtom();

    if (expression == "")
    {
      for (; +it; ++it)
      {
        // store the atom pointer in the list
        result.push_back(const_cast<Atom*>(&*it));
			}
		}
    else
    {
      Expression match(expression);
      for (; +it; ++it)
      {
        if (match(*it))
        {
          // store the atom pointer in the list
          result.push_back(const_cast<Atom*>(&*it));
				}
			}
		}
		return result;
	}

	AtomList atoms(const AtomList& atom_list, const String& expression)
	{
		AtomList result;

    // iterate over all atoms
		AtomList::const_iterator it = atom_list.begin();

    Expression match(expression);
    for (; it != atom_list.end(); ++it)
    {
      if (match(**it))
      {
        // store the atom pointer in the list
        result.push_back(const_cast<Atom*>(*it));
			}
		}
		return result;
	}

	PDBAtomList PDBAtoms(const AtomContainer& fragment, const String& expression)
	{
		PDBAtomList result;

    // iterate over all atoms
		AtomConstIterator it = fragment.beginAtom();

    if (expression == "")
    {
      for (; +it; ++it)
      {
        const PDBAtom* pdb_atom = dynamic_cast<const PDBAtom*>(&*it);
        if (pdb_atom != 0)
        {
          // store the pdb atom pointer in the list
          result.push_back(const_cast<PDBAtom*>(pdb_atom));
				}
			}
		}
    else
    {
      Expression match(expression);
      for (; +it; ++it)
      {
        const PDBAtom* pdb_atom = dynamic_cast<const PDBAtom*>(&*it);
        if ((pdb_atom != 0) && match(*pdb_atom))
        {
          // store the pdb atom pointer in the list
          result.push_back(const_cast<PDBAtom*>(pdb_atom));
				}
			}
		}
		return result;
	}

	BondList bonds(const AtomContainer& fragment, bool selected_only)
	{
    // iterate over all atoms
    HashSet<Bond*> bond_set;
    AtomConstIterator atom_it = fragment.beginAtom();
    for (; +atom_it; ++atom_it)
    {
      if (atom_it->isSelected() || !selected_only)
      {
        // it the atom is selected or selection is irrelevant,
        // insert all bonds into the bond_set
        Atom::BondConstIterator bond_it = atom_it->beginBond();
        for (; +bond_it; ++bond_it)
        {
          bond_set.insert(const_cast<Bond*>(&*bond_it));
				}
			}
		}

    // copy the results from the hash set to the list
		BondList result;
    for (HashSet<Bond*>::Iterator it = bond_set.begin(); +it; ++it)
    {
      result.push_back(*it);
		}   

		return result;
	}

	BondList bonds(const Atom& atom)
	{
		BondList result;
    Atom::BondConstIterator bond_it = atom.beginBond();
    for (; +bond_it; ++bond_it)
    {
			result.push_back(const_cast<Bond*>(&*bond_it));
		}
		return result;
	}

	AtomContainerList atomContainers(const AtomContainer& fragment, bool selected_only)
	{
		AtomContainerList result;

    // iterate over all base fragments
    AtomContainerConstIterator it = fragment.beginAtomContainer();
    for (; +it; ++it)
    {
      if (!selected_only || it->isSelected())
      {
        // store the base fragment pointer in the list
        result.push_back(const_cast<AtomContainer*>(&*it));
			}
		}
		return result;
	}

	FragmentList fragments(const AtomContainer& fragment, bool selected_only)
	{	
    // iterate over all fragments
		FragmentList result;
    AtomContainerConstIterator it = fragment.beginAtomContainer();

    for (; +it; ++it)
    {
      const Fragment* fragment = dynamic_cast<const Fragment*>(&*it);
      if ((fragment != 0) && (it->isSelected() || !selected_only))
      {
        // store the fragment pointer in the list
				result.push_back(const_cast<Fragment*>(fragment));
			}
		}
		return result;
	}

	MoleculeList molecules(const AtomContainer& fragment, bool selected_only)
	{
    // iterate over all molecules
    AtomContainerConstIterator it = fragment.beginAtomContainer();
		MoleculeList result;
    for (; +it; ++it)
    {
      const Molecule* molecule = dynamic_cast<const Molecule*>(&*it);
      if ((molecule != 0) && (it->isSelected() || !selected_only))
      {
        // store the molecule pointer in the list
        result.push_back(const_cast<Molecule*>(molecule));
			}
		}
		return result;
	}

	ResidueList residues(const AtomContainer& fragment, bool selected_only)
	{
    // iterate over all residues
    AtomContainerConstIterator it = fragment.beginAtomContainer();
		ResidueList result;
    for (; +it; ++it)
    {
      const Residue* residue = dynamic_cast<const Residue*>(&*it);
      if ((residue != 0) && (it->isSelected() || !selected_only))
      {
        // store the residue pointer in the list
        result.push_back(const_cast<Residue*>(residue));
			}
		}
		return result;
	}

	SecondaryStructureList secondaryStructures(const AtomContainer& fragment, bool selected_only)
	{
    // iterate over all secondary structures
    AtomContainerConstIterator it = fragment.beginAtomContainer();
		SecondaryStructureList result;
    for (; +it; ++it)
    {
      const SecondaryStructure* sec_struct = dynamic_cast<const SecondaryStructure*>(&*it);
      if ((sec_struct != 0) && (it->isSelected() || !selected_only))
      {
        // store the sec_struct pointer in the list
        result.push_back(const_cast<SecondaryStructure*>(sec_struct));
			}
		}
		return result;
	}

	ChainList chains(const AtomContainer& fragment, bool selected_only)
	{
    // iterate over all chains
    AtomContainerConstIterator it = fragment.beginAtomContainer();
		ChainList result;
    for (; +it; ++it)
    {
      const Chain* chain = dynamic_cast<const Chain*>(&*it);
      if ((chain != 0) && (it->isSelected() || !selected_only))
      {
        // store the chain pointer in the list
        result.push_back(const_cast<Chain*>(chain));
			}
		}
		return result;
	}

	ProteinList proteins(const AtomContainer& fragment, bool selected_only)
	{
    // iterate over all proteins
    AtomContainerConstIterator it = fragment.beginAtomContainer();
		ProteinList result;
    for (; +it; ++it)
    {
      const Protein* protein = dynamic_cast<const Protein*>(&*it);
      if ((protein != 0) && (it->isSelected() || !selected_only))
      {
        // store the protein pointer in the list
        result.push_back(const_cast<Protein*>(protein));
			}
		}
		return result;
	}

	NucleotideList nucleotides(const AtomContainer& fragment, bool selected_only)
	{
    // iterate over all proteins
    AtomContainerConstIterator it = fragment.beginAtomContainer();
		NucleotideList result;
    for (; +it; ++it)
    {
      const Nucleotide* nucleotide = dynamic_cast<const Nucleotide*>(&*it);
      if ((nucleotide != 0) && (it->isSelected() || !selected_only))
      {
        // store the nucleotide pointer in the list
        result.push_back(const_cast<Nucleotide*>(nucleotide));
			}
		}
		return result;
	}

	NucleicAcidList nucleicAcids(const AtomContainer& fragment, bool selected_only)
	{
    // iterate over all proteins
    AtomContainerConstIterator it = fragment.beginAtomContainer();
		NucleicAcidList result;
    for (; +it; ++it)
    {
      const NucleicAcid* nucleic_acid = dynamic_cast<const NucleicAcid*>(&*it);
      if ((nucleic_acid != 0) && (it->isSelected() || !selected_only))
      {
        // store the nucleic acid pointer in the list
        result.push_back(const_cast<NucleicAcid*>(nucleic_acid));
			}
		}
		return result;
	}

}
