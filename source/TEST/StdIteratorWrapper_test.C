#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

#include <BALL/FORMAT/PDBFile.h>

#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/nucleicAcid.h>
#include <BALL/KERNEL/nucleotide.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/secondaryStructure.h>
#include <BALL/KERNEL/system.h>

using namespace BALL;

START_TEST(StdIteratorWrapper)

	PDBFile f;
	f.open(BALL_TEST_DATA_PATH(1BNA.pdb));
	System s;

	f >> s;
CHECK(AtomContainer)

	// AtomIterator
	{
		AtomContainer* cont = &s;
		AtomIterator old = cont->beginAtom();
		for(AtomIterator it = cont->atoms().begin(); it != cont->atoms().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const AtomContainer* cont = &s;
		AtomConstIterator old = cont->beginAtom();
		for(AtomConstIterator it = cont->atoms().begin(); it != cont->atoms().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}

	// AtomContainerIterator
	{
		AtomContainer* cont = &s;
		AtomContainerIterator old = cont->beginAtomContainer();
		for(AtomContainerIterator it = cont->atomContainers().begin(); it != cont->atomContainers().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const AtomContainer* cont = &s;
		AtomContainerConstIterator old = cont->beginAtomContainer();
		for(AtomContainerConstIterator it = cont->atomContainers().begin(); it != cont->atomContainers().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
RESULT

CHECK(Chain)
	// PDBAtomIterator
	{
		Chain* cont = s.getProtein(0)->getChain(0);
		PDBAtomIterator old = cont->beginPDBAtom();
		for(PDBAtomIterator it = cont->pdbAtoms().begin(); it != cont->pdbAtoms().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const Chain* cont = s.getProtein(0)->getChain(0);
		PDBAtomConstIterator old = cont->beginPDBAtom();
		for(PDBAtomConstIterator it = cont->pdbAtoms().begin(); it != cont->pdbAtoms().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}

	// ResidueIterator
	{
		Chain* cont = s.getProtein(0)->getChain(0);
		ResidueIterator old = cont->beginResidue();
		for(ResidueIterator it = cont->residues().begin(); it != cont->residues().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const Chain* cont = s.getProtein(0)->getChain(0);
		ResidueConstIterator old = cont->beginResidue();
		for(ResidueConstIterator it = cont->residues().begin(); it != cont->residues().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}

	// SecondaryStructureIterator
	{
		Chain* cont = s.getProtein(0)->getChain(0);
		SecondaryStructureIterator old = cont->beginSecondaryStructure();
		for(SecondaryStructureIterator it = cont->secondaryStructures().begin(); it != cont->secondaryStructures().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const Chain* cont = s.getProtein(0)->getChain(0);
		SecondaryStructureConstIterator old = cont->beginSecondaryStructure();
		for(SecondaryStructureConstIterator it = cont->secondaryStructures().begin(); it != cont->secondaryStructures().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
RESULT

CHECK(Molecule)
	{
		Molecule* cont = s.getProtein(0);
		FragmentIterator old = cont->beginFragment();
		for(FragmentIterator it = cont->fragments().begin(); it != cont->fragments().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const Molecule* cont = s.getProtein(0);
		FragmentConstIterator old = cont->beginFragment();
		for(FragmentConstIterator it = cont->fragments().begin(); it != cont->fragments().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
RESULT

/*
CHECK(NucleicAcid)
Nucleotide, nucleotides)
RESULT
*/

CHECK(Protein)
	{
		Protein* cont = s.getProtein(0);
		ChainIterator old = cont->beginChain();
		for(ChainIterator it = cont->chains().begin(); it != cont->chains().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const Protein* cont = s.getProtein(0);
		ChainConstIterator old = cont->beginChain();
		for(ChainConstIterator it = cont->chains().begin(); it != cont->chains().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		Protein* cont = s.getProtein(0);
		SecondaryStructureIterator old = cont->beginSecondaryStructure();
		for(SecondaryStructureIterator it = cont->secondaryStructures().begin(); it != cont->secondaryStructures().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const Protein* cont = s.getProtein(0);
		SecondaryStructureConstIterator old = cont->beginSecondaryStructure();
		for(SecondaryStructureConstIterator it = cont->secondaryStructures().begin(); it != cont->secondaryStructures().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		Protein* cont = s.getProtein(0);
		ResidueIterator old = cont->beginResidue();
		for(ResidueIterator it = cont->residues().begin(); it != cont->residues().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const Protein* cont = s.getProtein(0);
		ResidueConstIterator old = cont->beginResidue();
		for(ResidueConstIterator it = cont->residues().begin(); it != cont->residues().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		Protein* cont = s.getProtein(0);
		PDBAtomIterator old = cont->beginPDBAtom();
		for(PDBAtomIterator it = cont->pdbAtoms().begin(); it != cont->pdbAtoms().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const Protein* cont = s.getProtein(0);
		PDBAtomConstIterator old = cont->beginPDBAtom();
		for(PDBAtomConstIterator it = cont->pdbAtoms().begin(); it != cont->pdbAtoms().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
RESULT

CHECK(Residue)
	{
		Residue* cont = s.getProtein(0)->getResidue(0);
		PDBAtomIterator old = cont->beginPDBAtom();
		for(PDBAtomIterator it = cont->pdbAtoms().begin(); it != cont->pdbAtoms().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const Residue* cont = s.getProtein(0)->getResidue(0);
		PDBAtomConstIterator old = cont->beginPDBAtom();
		for(PDBAtomConstIterator it = cont->pdbAtoms().begin(); it != cont->pdbAtoms().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
RESULT

CHECK(SecondaryStructure)
	{
		SecondaryStructure* cont = s.getProtein(0)->getSecondaryStructure(0);
		ResidueIterator old = cont->beginResidue();
		for(ResidueIterator it = cont->residues().begin(); it != cont->residues().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const SecondaryStructure* cont = s.getProtein(0)->getSecondaryStructure(0);
		ResidueConstIterator old = cont->beginResidue();
		for(ResidueConstIterator it = cont->residues().begin(); it != cont->residues().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		SecondaryStructure* cont = s.getProtein(0)->getSecondaryStructure(0);
		PDBAtomIterator old = cont->beginPDBAtom();
		for(PDBAtomIterator it = cont->pdbAtoms().begin(); it != cont->pdbAtoms().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const SecondaryStructure* cont = s.getProtein(0)->getSecondaryStructure(0);
		PDBAtomConstIterator old = cont->beginPDBAtom();
		for(PDBAtomConstIterator it = cont->pdbAtoms().begin(); it != cont->pdbAtoms().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
RESULT

CHECK(System)
	// NucleicAcidIterator
	{
		System* cont = &s;
		NucleicAcidIterator old = cont->beginNucleicAcid();
		for(NucleicAcidIterator it = cont->nucleicAcids().begin(); it != cont->nucleicAcids().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const System* cont = &s;
		NucleicAcidConstIterator old = cont->beginNucleicAcid();
		for(NucleicAcidConstIterator it = cont->nucleicAcids().begin(); it != cont->nucleicAcids().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}

	// NucleotideIterator
	{
		System* cont = &s;
		NucleotideIterator old = cont->beginNucleotide();
		for(NucleotideIterator it = cont->nucleotides().begin(); it != cont->nucleotides().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const System* cont = &s;
		NucleotideConstIterator old = cont->beginNucleotide();
		for(NucleotideConstIterator it = cont->nucleotides().begin(); it != cont->nucleotides().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}

	// ProteinIterator
	{
		System* cont = &s;
		ProteinIterator old = cont->beginProtein();
		for(ProteinIterator it = cont->proteins().begin(); it != cont->proteins().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const System* cont = &s;
		ProteinConstIterator old = cont->beginProtein();
		for(ProteinConstIterator it = cont->proteins().begin(); it != cont->proteins().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}

	// MoleculeIterator
	{
		System* cont = &s;
		MoleculeIterator old = cont->beginMolecule();
		for(MoleculeIterator it = cont->molecules().begin(); it != cont->molecules().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const System* cont = &s;
		MoleculeConstIterator old = cont->beginMolecule();
		for(MoleculeConstIterator it = cont->molecules().begin(); it != cont->molecules().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}

	// AtomIterator
	{
		System* cont = &s;
		AtomIterator old = cont->beginAtom();
		for(AtomIterator it = cont->atoms().begin(); it != cont->atoms().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const System* cont = &s;
		AtomConstIterator old = cont->beginAtom();
		for(AtomConstIterator it = cont->atoms().begin(); it != cont->atoms().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}

	// AtomContainerIterator
	{
		System* cont = &s;
		AtomContainerIterator old = cont->beginAtomContainer();
		for(AtomContainerIterator it = cont->atomContainers().begin(); it != cont->atomContainers().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const System* cont = &s;
		AtomContainerConstIterator old = cont->beginAtomContainer();
		for(AtomContainerConstIterator it = cont->atomContainers().begin(); it != cont->atomContainers().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}

	// ResidueIterator
	{
		System* cont = &s;
		ResidueIterator old = cont->beginResidue();
		for(ResidueIterator it = cont->residues().begin(); it != cont->residues().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const System* cont = &s;
		ResidueConstIterator old = cont->beginResidue();
		for(ResidueConstIterator it = cont->residues().begin(); it != cont->residues().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}

	// FragmentIterator
	{
		System* cont = &s;
		FragmentIterator old = cont->beginFragment();
		for(FragmentIterator it = cont->fragments().begin(); it != cont->fragments().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const System* cont = &s;
		FragmentConstIterator old = cont->beginFragment();
		for(FragmentConstIterator it = cont->fragments().begin(); it != cont->fragments().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}

	// ChainIterator
	{
		System* cont = &s;
		ChainIterator old = cont->beginChain();
		for(ChainIterator it = cont->chains().begin(); it != cont->chains().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const System* cont = &s;
		ChainConstIterator old = cont->beginChain();
		for(ChainConstIterator it = cont->chains().begin(); it != cont->chains().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}

	// SecondaryStructureIterator
	{
		System* cont = &s;
		SecondaryStructureIterator old = cont->beginSecondaryStructure();
		for(SecondaryStructureIterator it = cont->secondaryStructures().begin(); it != cont->secondaryStructures().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
	{
		const System* cont = &s;
		SecondaryStructureConstIterator old = cont->beginSecondaryStructure();
		for(SecondaryStructureConstIterator it = cont->secondaryStructures().begin(); it != cont->secondaryStructures().end(); ++it,++old) {
			bool res = it == old;
			TEST_EQUAL(res, true);
		}
	}
RESULT

END_TEST
