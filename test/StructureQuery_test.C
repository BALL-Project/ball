// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// v2.2 HCP-2b.1: StructureQuery_test.
//
// HCP-2b.1 introduces BALL::StructureQuery -- free functions over the HCP-2a
// role-aware handles (MoleculeHandle/FragmentHandle) that answer the v0
// polymer-structure questions (chains/residues/getResidueByID/getNTerminal/
// getCTerminal/secondary-structure lookup) by ROLE over the container subtree.
//
// This test builds a container table BY HAND (as ContainerHandle_test does --
// the H2 mutation-mirror does this automatically at runtime) shaped like a real
// protein with BOTH direct chain->residue AND chain->SS->residue depth, plus a
// LEADING and a TRAILING non-amino (water) residue, so it pins:
//   * residues found at either depth (canonical-depth-honouring, D-2b.2),
//   * nTerminal/cTerminal SKIP the waters (v0 isAminoAcid() semantics, D-2b.5),
//   * firstResidue/lastResidue RETURN the waters (generic, no kind filter).
//
// READ-only; header-only; no store mutation.
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/moleculeStore.h>
#include <BALL/KERNEL/_moleculeStoreInternal.h>   // build the table by hand
#include <BALL/KERNEL/containerKind.h>
#include <BALL/KERNEL/containerRole.h>
#include <BALL/KERNEL/containerHandle.h>
#include <BALL/KERNEL/structureQuery.h>
#include <vector>
///////////////////////////

using namespace BALL;

START_TEST(StructureQuery)

using namespace BALL;
using namespace BALL::StructureQuery;

CHECK(structure queries over a protein -> chain -> [SS] -> residue table)
	MoleculeStore store;
	ContainerTable& t = store.sideTables_().container_table_;

	// Protein "1ABC"
	std::uint32_t prot = t.allocate(ContainerKind::PROTEIN);
	t.row(prot).name_offset       = t.intern("PROT");
	t.row(prot).payload.id_offset = t.intern("1ABC");

	// Chain A
	std::uint32_t chainA = t.allocate(ContainerKind::CHAIN);
	t.row(chainA).name_offset = t.intern("A");
	t.append_child(prot, ChildRef(ChildRef::CONTAINER, chainA));

	// Leading WATER residue (non-amino) -- directly under the chain.
	std::uint32_t resWat0 = t.allocate(ContainerKind::RESIDUE);
	t.row(resWat0).name_offset       = t.intern("HOH");
	t.row(resWat0).payload.id_offset = t.intern("W0");
	store.container_set_residue_kind_(resWat0, ResidueKind::WATER);
	t.append_child(chainA, ChildRef(ChildRef::CONTAINER, resWat0));

	// SecondaryStructure (helix) -- owning layer during dual existence.
	std::uint32_t ss = t.allocate(ContainerKind::SECONDARY_STRUCTURE);
	t.row(ss).payload.ss_type = (std::uint8_t)1;
	t.append_child(chainA, ChildRef(ChildRef::CONTAINER, ss));

	// Two AMINO_ACID residues UNDER the SS layer.
	std::uint32_t resAla = t.allocate(ContainerKind::RESIDUE);
	t.row(resAla).name_offset       = t.intern("ALA");
	t.row(resAla).payload.id_offset = t.intern("1");
	store.container_set_residue_kind_(resAla, ResidueKind::AMINO_ACID);
	t.append_child(ss, ChildRef(ChildRef::CONTAINER, resAla));
	t.append_child(resAla, ChildRef(ChildRef::ATOM, 10));
	t.append_child(resAla, ChildRef(ChildRef::ATOM, 11));

	std::uint32_t resGly = t.allocate(ContainerKind::RESIDUE);
	t.row(resGly).name_offset       = t.intern("GLY");
	t.row(resGly).payload.id_offset = t.intern("2");
	store.container_set_residue_kind_(resGly, ResidueKind::AMINO_ACID);
	t.append_child(ss, ChildRef(ChildRef::CONTAINER, resGly));

	// One AMINO_ACID residue DIRECTLY under the chain (after the SS).
	std::uint32_t resVal = t.allocate(ContainerKind::RESIDUE);
	t.row(resVal).name_offset       = t.intern("VAL");
	t.row(resVal).payload.id_offset = t.intern("3");
	store.container_set_residue_kind_(resVal, ResidueKind::AMINO_ACID);
	t.append_child(chainA, ChildRef(ChildRef::CONTAINER, resVal));

	// Trailing WATER residue (non-amino) -- directly under the chain.
	std::uint32_t resWat1 = t.allocate(ContainerKind::RESIDUE);
	t.row(resWat1).name_offset       = t.intern("HOH");
	t.row(resWat1).payload.id_offset = t.intern("W1");
	store.container_set_residue_kind_(resWat1, ResidueKind::WATER);
	t.append_child(chainA, ChildRef(ChildRef::CONTAINER, resWat1));

	MoleculeHandle mol(store, prot);

	// chains(): exactly the one CHAIN-role fragment.
	std::vector<FragmentHandle> chs = chains(mol);
	TEST_EQUAL(chs.size(), 1u)
	TEST_EQUAL(chs[0].getStoreIndex(), chainA)
	TEST_EQUAL(chs[0].getFragmentRole() == FragmentRole::CHAIN, true)

	// residues(MoleculeHandle): all 5 RESIDUE-role fragments, preorder, at BOTH
	// depths (W0, ALA+GLY under SS, VAL, W1).
	std::vector<FragmentHandle> res = residues(mol);
	TEST_EQUAL(res.size(), 5u)
	TEST_EQUAL(res[0].getStoreIndex(), resWat0)   // leading water
	TEST_EQUAL(res[1].getStoreIndex(), resAla)    // under SS
	TEST_EQUAL(res[2].getStoreIndex(), resGly)    // under SS
	TEST_EQUAL(res[3].getStoreIndex(), resVal)    // direct child after SS
	TEST_EQUAL(res[4].getStoreIndex(), resWat1)   // trailing water

	// residues(FragmentHandle): same set rooted at the chain.
	FragmentHandle chainHandle(store, chainA);
	TEST_EQUAL(residues(chainHandle).size(), 5u)

	// secondaryStructures(): the one SS fragment.
	std::vector<FragmentHandle> sss = secondaryStructures(mol);
	TEST_EQUAL(sss.size(), 1u)
	TEST_EQUAL(sss[0].getStoreIndex(), ss)

	// fragmentsByRole(): the generic building block matches the sugar.
	TEST_EQUAL(fragmentsByRole(mol, FragmentRole::RESIDUE).size(), 5u)
	TEST_EQUAL(fragmentsByRole(mol, FragmentRole::CHAIN).size(), 1u)
	TEST_EQUAL(fragmentsByRole(mol, FragmentRole::SECONDARY_STRUCTURE).size(), 1u)

	// firstResidue / lastResidue: generic, NO kind filter -> the waters.
	TEST_EQUAL((bool)firstResidue(mol), true)
	TEST_EQUAL(firstResidue(mol).getStoreIndex(), resWat0)
	TEST_EQUAL((bool)lastResidue(mol), true)
	TEST_EQUAL(lastResidue(mol).getStoreIndex(), resWat1)

	// nTerminal / cTerminal: v0-compatible -> SKIP the waters, return first/last
	// AMINO_ACID in preorder (ALA / VAL).
	FragmentHandle nterm = nTerminal(mol);
	TEST_EQUAL((bool)nterm, true)
	TEST_EQUAL(nterm.getStoreIndex(), resAla)
	TEST_EQUAL(nterm.getResidueKind() == ResidueKind::AMINO_ACID, true)
	FragmentHandle cterm = cTerminal(mol);
	TEST_EQUAL((bool)cterm, true)
	TEST_EQUAL(cterm.getStoreIndex(), resVal)
	TEST_EQUAL(cterm.getResidueKind() == ResidueKind::AMINO_ACID, true)

	// residueByID(): first RESIDUE-role fragment with the matching id.
	FragmentHandle by_id = residueByID(mol, "2");
	TEST_EQUAL((bool)by_id, true)
	TEST_EQUAL(by_id.getStoreIndex(), resGly)
	TEST_EQUAL((bool)residueByID(mol, "nope"), false)

	// secondaryStructureOf(): a residue under the SS layer resolves to it; a
	// residue directly under the chain (VAL) has no enclosing SS.
	FragmentHandle alaHandle(store, resAla);
	FragmentHandle ssOfAla = secondaryStructureOf(alaHandle);
	TEST_EQUAL((bool)ssOfAla, true)
	TEST_EQUAL(ssOfAla.getStoreIndex(), ss)
	FragmentHandle valHandle(store, resVal);
	TEST_EQUAL((bool)secondaryStructureOf(valHandle), false)
RESULT

CHECK(empty / amino-acid-free subtrees -> null terminal handles)
	MoleculeStore store;
	ContainerTable& t = store.sideTables_().container_table_;

	// A molecule with a single chain holding ONLY a water residue.
	std::uint32_t mol_idx = t.allocate(ContainerKind::MOLECULE);
	std::uint32_t chain   = t.allocate(ContainerKind::CHAIN);
	t.append_child(mol_idx, ChildRef(ChildRef::CONTAINER, chain));
	std::uint32_t wat = t.allocate(ContainerKind::RESIDUE);
	store.container_set_residue_kind_(wat, ResidueKind::WATER);
	t.append_child(chain, ChildRef(ChildRef::CONTAINER, wat));

	MoleculeHandle mol(store, mol_idx);

	// One residue exists, but it is not an amino acid -> no terminus (v0 returns 0).
	TEST_EQUAL(residues(mol).size(), 1u)
	TEST_EQUAL((bool)firstResidue(mol), true)        // generic helper sees the water
	TEST_EQUAL((bool)nTerminal(mol), false)          // amino-acid-filtered -> null
	TEST_EQUAL((bool)cTerminal(mol), false)

	// A molecule with NO residues at all.
	std::uint32_t bare = t.allocate(ContainerKind::MOLECULE);
	MoleculeHandle bareMol(store, bare);
	TEST_EQUAL(residues(bareMol).size(), 0u)
	TEST_EQUAL((bool)firstResidue(bareMol), false)
	TEST_EQUAL((bool)lastResidue(bareMol), false)
	TEST_EQUAL((bool)nTerminal(bareMol), false)
	TEST_EQUAL((bool)cTerminal(bareMol), false)
RESULT

END_TEST
