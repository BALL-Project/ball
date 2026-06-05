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
// v2.2 H4 commit 6.a coverage: need the v0 classes for the visitor test.
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <unordered_map>

// HCP-2a: this test intentionally exercises the [[deprecated]] legacy single-kind
// handle wrappers (ProteinHandle/ChainHandle/ResidueHandle/NucleotideHandle/
// NucleicAcidHandle/SecondaryStructureHandle -- backward-compat single-kind narrow)
// ALONGSIDE the new role-aware MoleculeHandle/FragmentHandle. Silence the expected
// deprecation warnings for the legacy spellings.
#if defined(__GNUC__) || defined(__clang__)
#	pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
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
	// R34 LOW: an out-of-range child index also yields a null handle
	// (not a non-null handle bound to the sentinel row 0).
	TEST_EQUAL((bool)rh.getChildContainer(99), false)

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

CHECK(HCP-2a -- role-aware MoleculeHandle / FragmentHandle over the role columns)
	MoleculeStore store;
	ContainerTable& t = store.sideTables_().container_table_;

	std::uint32_t prot  = t.allocate(ContainerKind::PROTEIN);
	t.row(prot).payload.id_offset = t.intern("1ABC");
	std::uint32_t na    = t.allocate(ContainerKind::NUCLEIC_ACID);
	t.row(na).payload.id_offset   = t.intern("DNA1");
	std::uint32_t chain = t.allocate(ContainerKind::CHAIN);
	std::uint32_t res   = t.allocate(ContainerKind::RESIDUE);
	t.row(res).payload.id_offset       = t.intern("GLY");
	t.row(res).payload.insertion_code  = 'B';
	store.container_set_residue_kind_(res, ResidueKind::AMINO_ACID);
	std::uint32_t ss    = t.allocate(ContainerKind::SECONDARY_STRUCTURE);
	t.row(ss).payload.ss_type = (std::uint8_t)1;

	// MoleculeHandle is role-aware: a SET of molecule-level kinds (multi-kind narrow).
	TEST_EQUAL(MoleculeHandle::acceptsKind(ContainerKind::MOLECULE), true)
	TEST_EQUAL(MoleculeHandle::acceptsKind(ContainerKind::PROTEIN), true)
	TEST_EQUAL(MoleculeHandle::acceptsKind(ContainerKind::NUCLEIC_ACID), true)
	TEST_EQUAL(MoleculeHandle::acceptsKind(ContainerKind::RESIDUE), false)

	MoleculeHandle mh(store, prot);
	TEST_EQUAL(mh.getMoleculeRole() == MoleculeRole::PROTEIN, true)
	TEST_EQUAL(mh.getID(), "1ABC")                         // role-asserting: PROTEIN carries id
	MoleculeHandle mh_na(store, na);
	TEST_EQUAL(mh_na.getMoleculeRole() == MoleculeRole::NUCLEIC_ACID, true)
	TEST_EQUAL(mh_na.getID(), "DNA1")

	// FragmentHandle is role-aware: a SET of fragment-level kinds.
	TEST_EQUAL(FragmentHandle::acceptsKind(ContainerKind::CHAIN), true)
	TEST_EQUAL(FragmentHandle::acceptsKind(ContainerKind::RESIDUE), true)
	TEST_EQUAL(FragmentHandle::acceptsKind(ContainerKind::SECONDARY_STRUCTURE), true)
	TEST_EQUAL(FragmentHandle::acceptsKind(ContainerKind::PROTEIN), false)

	FragmentHandle fh_ch(store, chain);
	TEST_EQUAL(fh_ch.getFragmentRole() == FragmentRole::CHAIN, true)
	FragmentHandle fh_res(store, res);
	TEST_EQUAL(fh_res.getFragmentRole() == FragmentRole::RESIDUE, true)
	TEST_EQUAL(fh_res.getResidueKind() == ResidueKind::AMINO_ACID, true)
	TEST_EQUAL(fh_res.getID(), "GLY")                      // role-asserting: RESIDUE carries id
	TEST_EQUAL(fh_res.getInsertionCode(), 'B')
	FragmentHandle fh_ss(store, ss);
	TEST_EQUAL(fh_ss.getFragmentRole() == FragmentRole::SECONDARY_STRUCTURE, true)
	TEST_EQUAL((int)fh_ss.getTypeCode(), 1)                // role-asserting: SS carries type code
	TEST_EQUAL(fh_ss.getSSKind() == store.container_ss_kind_(ss), true)  // handle forwards the derived kind

	// as<>() multi-kind narrow: a PROTEIN row narrows to the role-aware MoleculeHandle.
	ContainerHandleBase base_prot(store, prot);
	MoleculeHandle as_mh = base_prot.as<MoleculeHandle>();
	TEST_EQUAL((bool)as_mh, true)
	TEST_EQUAL(as_mh.getMoleculeRole() == MoleculeRole::PROTEIN, true)
	// a RESIDUE row does NOT narrow to MoleculeHandle, but DOES to FragmentHandle.
	ContainerHandleBase base_res(store, res);
	TEST_EQUAL((bool)base_res.as<MoleculeHandle>(), false)
	TEST_EQUAL((bool)base_res.as<FragmentHandle>(), true)

	// The [[deprecated]] legacy wrappers STILL narrow single-kind (preserved):
	// ProteinHandle binds only PROTEIN, so as<ProteinHandle>() on a NUCLEIC_ACID
	// row is null, while as<NucleicAcidHandle>() is not.
	ContainerHandleBase base_na(store, na);
	TEST_EQUAL((bool)base_na.as<ProteinHandle>(), false)
	TEST_EQUAL((bool)base_na.as<NucleicAcidHandle>(), true)
	TEST_EQUAL(ProteinHandle::KIND == ContainerKind::PROTEIN, true)   // legacy KIND preserved
RESULT


CHECK(H4 commit 6.a -- ContainerHandleBase::propertyNames + eachProperty)
	// v2.2 H4 commit 6.a (D-H4.6 R2): the visitor surface on the
	// container handle goes through the container_back_ptr bridge
	// to reach the v0 PropertyManager bag. This test exercises the
	// happy path (set props on v0, observe via handle).
	System sys;
	Molecule* mol = new Molecule;
	sys.insert(*mol);
	// Set two NamedProperties on the v0 molecule.
	mol->setProperty("foo", String("bar"));
	mol->setProperty("baz", 42);

	auto& store = sys.getStore();
	std::uint32_t mol_idx = mol->getContainerRow_();
	ContainerHandleBase h(store, mol_idx);

	// propertyNames returns the keys by value.
	auto names = h.propertyNames();
	TEST_EQUAL(names.size(), 2)
	bool has_foo = false, has_baz = false;
	for (auto& n : names)
	{
		if (n == "foo") has_foo = true;
		if (n == "baz") has_baz = true;
	}
	TEST_EQUAL(has_foo, true)
	TEST_EQUAL(has_baz, true)

	// eachProperty visits each property exactly once.
	int visited = 0;
	h.eachProperty([&visited](const NamedProperty& p)
	{
		(void)p;
		++visited;
	});
	TEST_EQUAL(visited, 2)
RESULT


CHECK(H4 commit 6.a -- empty container yields empty visitor)
	System sys;
	Molecule* mol = new Molecule;
	sys.insert(*mol);
	auto& store = sys.getStore();
	ContainerHandleBase h(store, mol->getContainerRow_());

	auto names = h.propertyNames();
	TEST_EQUAL(names.size(), 0)

	int visited = 0;
	h.eachProperty([&visited](const NamedProperty&) { ++visited; });
	TEST_EQUAL(visited, 0)
RESULT


CHECK(H4 commit 6.a -- null handle visitor surface is a safe no-op)
	ContainerHandleBase null_handle;
	auto names = null_handle.propertyNames();
	TEST_EQUAL(names.size(), 0)
	int visited = 0;
	null_handle.eachProperty([&visited](const NamedProperty&) { ++visited; });
	TEST_EQUAL(visited, 0)
RESULT


CHECK(H4 commit 7b.3 -- ContainerHandleBase::countAtoms)
	// v2.2 H4 commit 7b.3 (D-H4.15 R3 counts cluster). Mirrors
	// AtomContainer::countAtoms through the container_back_ptr bridge.
	System sys;
	Molecule* mol = new Molecule;
	sys.insert(*mol);
	Atom* a1 = new Atom;
	Atom* a2 = new Atom;
	Atom* a3 = new Atom;
	mol->insert(*a1);
	mol->insert(*a2);
	mol->insert(*a3);

	auto& store = sys.getStore();
	ContainerHandleBase h(store, mol->getContainerRow_());
	TEST_EQUAL(h.countAtoms(), 3)

	// Null handle: 0.
	ContainerHandleBase null_h;
	TEST_EQUAL(null_h.countAtoms(), 0)
RESULT

END_TEST
