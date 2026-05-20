// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// v2.2 H1b: ContainerHandle_test.
//
// H1b introduces the container value-handle TYPES (Molecule/Chain/
// Residue/... as {store, idx, generation}) reading the H1a container
// table via MoleculeStore's scalar accessors. This test builds a table
// by hand (simulating what the H2 mutation-mirror wiring will do
// automatically) and exercises the PUBLIC handle API: scalar getters,
// null-handle semantics, parent/child navigation, the as<>() narrow, and
// generation-based staleness after release/reuse and after a migrate.
//
// Validity checks (assertValid_/assertKind_) are gated to BALL_DEBUG /
// BALL_PYTHON_WRAPPER (D54/D65); isValid() and as<>() are ALWAYS compiled
// (queries), so the staleness/narrow assertions below hold in Release too.
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/moleculeStore.h>
#include <BALL/KERNEL/_moleculeStoreInternal.h>   // build the table by hand
#include <BALL/KERNEL/containerKind.h>
#include <BALL/KERNEL/containerHandle.h>
#include <unordered_map>
///////////////////////////

using namespace BALL;

START_TEST(ContainerHandle)

using namespace BALL;

CHECK(layout pins -- base 24 B + typed handles add no members)
	TEST_EQUAL(sizeof(ContainerHandleBase), 24u)
	TEST_EQUAL(sizeof(MoleculeHandle), 24u)
	TEST_EQUAL(sizeof(ChainHandle), 24u)
	TEST_EQUAL(sizeof(ResidueHandle), 24u)
	TEST_EQUAL(sizeof(ProteinHandle), 24u)
	TEST_EQUAL(sizeof(SecondaryStructureHandle), 24u)
	TEST_EQUAL(sizeof(NucleotideHandle), 24u)
	TEST_EQUAL(sizeof(NucleicAcidHandle), 24u)
	TEST_EQUAL(sizeof(FragmentHandle), 24u)
RESULT

CHECK(null handle semantics)
	ResidueHandle nullh;
	TEST_EQUAL(nullh.isNull(), true)
	TEST_EQUAL((bool)nullh, false)
	TEST_EQUAL(nullh.isValid(), false)
	TEST_EQUAL(nullh.getStore() == (MoleculeStore*)nullptr, true)
RESULT

CHECK(scalar getters read the row -- kind / name / payload)
	MoleculeStore store;
	ContainerTable& t = store.sideTables_().container_table_;

	std::uint32_t prot = t.allocate(ContainerKind::PROTEIN);
	t.row(prot).name_offset       = t.intern("PROT");
	t.row(prot).payload.id_offset = t.intern("1ABC");

	std::uint32_t res = t.allocate(ContainerKind::RESIDUE);
	t.row(res).name_offset        = t.intern("ALA");
	t.row(res).payload.id_offset  = t.intern("ALA");
	t.row(res).payload.insertion_code = 'A';

	ProteinHandle ph(store, prot);
	TEST_EQUAL(ph.getKind() == ContainerKind::PROTEIN, true)
	TEST_EQUAL(ph.getName(), "PROT")
	TEST_EQUAL(ph.getID(), "1ABC")
	TEST_EQUAL((bool)ph, true)
	TEST_EQUAL(ph.isValid(), true)

	ResidueHandle rh(store, res);
	TEST_EQUAL(rh.getKind() == ContainerKind::RESIDUE, true)
	TEST_EQUAL(rh.getName(), "ALA")
	TEST_EQUAL(rh.getID(), "ALA")
	TEST_EQUAL(rh.getInsertionCode(), 'A')

	std::uint32_t ss = t.allocate(ContainerKind::SECONDARY_STRUCTURE);
	t.row(ss).payload.ss_type = 3;
	SecondaryStructureHandle ssh(store, ss);
	TEST_EQUAL((int)ssh.getTypeCode(), 3)
RESULT

CHECK(parent / child navigation + as<>() narrow)
	MoleculeStore store;
	ContainerTable& t = store.sideTables_().container_table_;

	std::uint32_t chain = t.allocate(ContainerKind::CHAIN);
	t.row(chain).name_offset = t.intern("A");
	std::uint32_t res = t.allocate(ContainerKind::RESIDUE);
	t.row(res).name_offset = t.intern("GLY");
	t.append_child(chain, ChildRef(ChildRef::CONTAINER, res));
	t.append_child(res, ChildRef(ChildRef::ATOM, 10));
	t.append_child(res, ChildRef(ChildRef::ATOM, 11));

	ResidueHandle rh(store, res);

	// Parent: residue -> chain (as a base handle), narrowed via as<>().
	ContainerHandleBase parent = rh.getParent();
	TEST_EQUAL((bool)parent, true)
	TEST_EQUAL(parent.getKind() == ContainerKind::CHAIN, true)
	ChainHandle parent_chain = parent.as<ChainHandle>();
	TEST_EQUAL((bool)parent_chain, true)
	TEST_EQUAL(parent_chain.getName(), "A")
	// Wrong-kind narrow -> null typed handle (release-checked, no RTTI).
	ResidueHandle bad = parent.as<ResidueHandle>();
	TEST_EQUAL((bool)bad, false)

	// Root has no parent.
	TEST_EQUAL((bool)parent_chain.getParent(), false)
	TEST_EQUAL(parent_chain.getParentIndex(), MoleculeStore::CONTAINER_NONE)

	// Children: 2 atom children in order.
	TEST_EQUAL(rh.countChildren(), 2u)
	ContainerChildRef c0 = rh.getChild(0);
	TEST_EQUAL(c0.is_atom, true)
	TEST_EQUAL(c0.idx, 10u)
	TEST_EQUAL(rh.getChild(1).idx, 11u)
	// An atom child resolves to a null container handle.
	TEST_EQUAL((bool)rh.getChildContainer(0), false)

	// Chain's first child is the residue container.
	ContainerHandleBase chain_child = ContainerHandleBase(store, chain).getChildContainer(0);
	TEST_EQUAL((bool)chain_child, true)
	TEST_EQUAL(chain_child.getKind() == ContainerKind::RESIDUE, true)
RESULT

CHECK(D65 generation staleness -- release then reuse)
	MoleculeStore store;
	ContainerTable& t = store.sideTables_().container_table_;

	std::uint32_t res = t.allocate(ContainerKind::RESIDUE);
	ResidueHandle rh(store, res);
	TEST_EQUAL(rh.isValid(), true)

	// Release the row: generation bumps, slot freed -> handle goes stale.
	t.release(res);
	TEST_EQUAL(rh.isValid(), false)

	// Reuse the slot: a NEW handle on the recycled slot is valid, but the
	// OLD handle stays stale (its captured generation is behind).
	std::uint32_t res_b = t.allocate(ContainerKind::RESIDUE);
	TEST_EQUAL(res_b, res)                 // same slot recycled
	ResidueHandle rh_new(store, res_b);
	TEST_EQUAL(rh_new.isValid(), true)
	TEST_EQUAL(rh.isValid(), false)        // stale alias still detected
	TEST_NOT_EQUAL(rh.getGeneration(), rh_new.getGeneration())
RESULT

CHECK(D56/D65 detached-alias staleness across migration)
	// A handle into the orphan/source table goes stale once the subtree is
	// migrated (true-move releases the source rows -> generation bump).
	MoleculeStore src_store;
	ContainerTable& src = src_store.sideTables_().container_table_;
	std::uint32_t s_root = src.allocate(ContainerKind::CHAIN);
	src.row(s_root).name_offset = src.intern("A");
	std::uint32_t s_res = src.allocate(ContainerKind::RESIDUE);
	src.append_child(s_root, ChildRef(ChildRef::CONTAINER, s_res));
	src.append_child(s_res, ChildRef(ChildRef::ATOM, 50));

	ChainHandle src_handle(src_store, s_root);
	ResidueHandle src_res_handle(src_store, s_res);
	TEST_EQUAL(src_handle.isValid(), true)
	TEST_EQUAL(src_res_handle.isValid(), true)

	MoleculeStore dst_store;
	ContainerTable& dst = dst_store.sideTables_().container_table_;
	std::unordered_map<std::uint32_t, std::uint32_t> remap;
	remap[50] = 50;
	std::uint32_t dst_root = dst.migrate_subtree_from(src, s_root, remap);

	// Source handles are now stale; destination handle is valid.
	TEST_EQUAL(src_handle.isValid(), false)
	TEST_EQUAL(src_res_handle.isValid(), false)
	ChainHandle dst_handle(dst_store, dst_root);
	TEST_EQUAL(dst_handle.isValid(), true)
	TEST_EQUAL(dst_handle.getName(), "A")
RESULT

END_TEST
