// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// v2.1 P1.3 (D22b + D31b): SideTableParity_test.
//
// In P1, MoleculeStore carries 3 side tables (composite_nodes_,
// property_columns_, selected_bits_) that are POPULATED via explicit
// allocate/release/set calls but NOT yet maintained automatically
// during Composite / PropertyManager / Selectable mutations. The
// dual-write design originally specified at P1 plan-time was moved
// to P2 per the maintainer's 2026-05-19 decision on R17c P17c-9.
//
// What P1's SideTableParity_test verifies right now (P1.3 scope):
//   - MoleculeStore::sideTables_() returns a usable reference.
//   - composite_nodes_ starts empty.
//   - allocate_composite_node_() round-trip: assigns a fresh handle,
//     yields a zero-topology node, release returns the slot to the
//     free list, next allocate reuses the slot.
//   - Composite::getCompositeHandle_() / setCompositeHandle_()
//     memcpy round-trip correctly (no kind/idx corruption).
//   - Composite::getNode_() reflects the v0 inline topology.
//
// P2 adds the actual mutation-mirror parity assertions (assert
// composite_nodes_ matches inline tree after appendChild / removeChild
// etc.) using the same test file. P1.6 / P1.8 add property column
// and selection bit parity sections to this file.
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/moleculeStore.h>
#include <BALL/KERNEL/_moleculeStoreInternal.h>   // D31b: P1.3 wiring TU
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
#include <BALL/CONCEPT/composite.h>
///////////////////////////

START_TEST(SideTableParity)

using namespace BALL;

CHECK(sideTables_ accessor + empty initial state)
	MoleculeStore store;
	MoleculeStoreSideTables& s = store.sideTables_();
	TEST_EQUAL(s.composite_nodes_.size(), 0)
	TEST_EQUAL(s.composite_free_list_.size(), 0)
RESULT

CHECK(allocate_composite_node_ -- append path)
	MoleculeStore store;
	MoleculeStoreSideTables& s = store.sideTables_();

	CompositeHandle h0 = s.allocate_composite_node_(CompositeKind::ATOM);
	TEST_EQUAL(h0.kind == CompositeKind::ATOM, true)
	// P2.1.1 fix: idx=0 reserved as null sentinel; first allocation
	// yields idx >= 1. composite_nodes_ size is 2 (dummy at 0 + h0
	// at 1).
	TEST_EQUAL(h0.idx, 1u)
	TEST_EQUAL(s.composite_nodes_.size(), 2u)
	TEST_EQUAL(s.composite_free_list_.size(), 0u)

	CompositeHandle h1 = s.allocate_composite_node_(CompositeKind::MOLECULE);
	TEST_EQUAL(h1.kind == CompositeKind::MOLECULE, true)
	TEST_EQUAL(h1.idx, 2u)
	TEST_EQUAL(s.composite_nodes_.size(), 3u)

	// Topology starts zeroed.
	const CompositeNode& n0 = s.node_(h0);
	TEST_EQUAL(n0.parent.isNull(), true)
	TEST_EQUAL(n0.first_child.isNull(), true)
	TEST_EQUAL(n0.last_child.isNull(), true)
	TEST_EQUAL(n0.next_sibling.isNull(), true)
	TEST_EQUAL(n0.prev_sibling.isNull(), true)
	TEST_EQUAL(n0.child_count, 0u)
RESULT

CHECK(release_composite_node_ + free-list reuse)
	MoleculeStore store;
	MoleculeStoreSideTables& s = store.sideTables_();

	CompositeHandle h0 = s.allocate_composite_node_(CompositeKind::ATOM);
	CompositeHandle h1 = s.allocate_composite_node_(CompositeKind::ATOM);
	// P2.1.1 fix: dummy at idx=0 + h0 at idx=1 + h1 at idx=2 = size 3.
	TEST_EQUAL(s.composite_nodes_.size(), 3u)

	// Release h0: vector size unchanged, free list grows.
	s.release_composite_node_(h0);
	TEST_EQUAL(s.composite_nodes_.size(), 3u)
	TEST_EQUAL(s.composite_free_list_.size(), 1u)

	// Topology is zeroed on release (no stale links).
	TEST_EQUAL(s.composite_nodes_[h0.idx].parent.isNull(), true)
	TEST_EQUAL(s.composite_nodes_[h0.idx].kind == CompositeKind::NONE, true)

	// Next allocate reuses the slot.
	CompositeHandle h2 = s.allocate_composite_node_(CompositeKind::BOND);
	TEST_EQUAL(h2.idx, h0.idx)
	TEST_EQUAL(h2.kind == CompositeKind::BOND, true)
	TEST_EQUAL(s.composite_nodes_.size(), 3u)
	TEST_EQUAL(s.composite_free_list_.size(), 0u)

	// Releasing the null handle is a no-op.
	s.release_composite_node_(CompositeHandle{});
	TEST_EQUAL(s.composite_free_list_.size(), 0u)
RESULT

CHECK(Composite::getCompositeHandle_ / setCompositeHandle_ round-trip)
	Atom a;  // Atom is a Composite

	// Default-constructed: opaque slot is 0 → NULL handle.
	CompositeHandle h0 = a.getCompositeHandle_();
	TEST_EQUAL(h0.kind == CompositeKind::NONE, true)
	TEST_EQUAL(h0.idx, 0u)
	TEST_EQUAL(h0.isNull(), true)

	// Set a non-NULL handle, read back the same bytes.
	CompositeHandle want;
	want.kind = CompositeKind::ATOM;
	want.idx  = 42;
	a.setCompositeHandle_(want);
	CompositeHandle got = a.getCompositeHandle_();
	TEST_EQUAL(got.kind == CompositeKind::ATOM, true)
	TEST_EQUAL(got.idx, 42u)
	TEST_EQUAL(got.isNull(), false)
RESULT

CHECK(Composite::getNode_ reflects v0 inline topology)
	// Build a small Atom→Molecule tree using v0 inline pointers.
	Molecule mol;
	Atom a;
	mol.insert(a);

	// getNode_() on the atom should expose its parent + sibling
	// neighbours via the by-value view.
	CompositeTopologyView atom_view = a.getNode_();
	TEST_EQUAL(atom_view.parent, &mol)
	TEST_EQUAL(atom_view.first_child, (Composite*)0)
	TEST_EQUAL(atom_view.last_child, (Composite*)0)
	TEST_EQUAL(atom_view.next_sibling, (Composite*)0)
	TEST_EQUAL(atom_view.prev_sibling, (Composite*)0)
	TEST_EQUAL(atom_view.child_count, 0u)

	// getNode_() on the molecule sees its one child.
	CompositeTopologyView mol_view = mol.getNode_();
	TEST_EQUAL(mol_view.parent, (Composite*)0)
	TEST_EQUAL(mol_view.first_child, &a)
	TEST_EQUAL(mol_view.last_child, &a)
	TEST_EQUAL(mol_view.child_count, 1u)
RESULT

CHECK(PropertyColumnRegistry -- well-known FF columns lazily materialise and stay cap-exempt)
	MoleculeStore store;
	MoleculeStoreSideTables& s = store.sideTables_();
	PropertyColumnRegistry& reg = s.property_columns_;

	// HCP-1P.C: construction allocates NOTHING -- the 10 well-known columns
	// materialise lazily on first write (was: 10 eager columns at ctor).
	TEST_EQUAL(reg.columnCount(), 0u)
	TEST_EQUAL(reg.dynamicCount(), 0u)
	TEST_EQUAL(reg.maxDynamic(), 65536u)

	// Well-known names are flagged via the shared schema (independent of
	// whether the column has been materialised yet).
	TEST_EQUAL(reg.isWellKnown("PARTIAL_CHARGE"), true)
	TEST_EQUAL(reg.isWellKnown("FORMAL_CHARGE"), true)
	TEST_EQUAL(reg.isWellKnown("MMFF94_TYPE"), true)
	TEST_EQUAL(reg.isWellKnown("AMBER_TYPE"), true)
	TEST_EQUAL(reg.isWellKnown("RADIUS"), true)
	TEST_EQUAL(reg.isWellKnown("EPSILON"), true)
	TEST_EQUAL(reg.isWellKnown("HYBRIDIZATION"), true)
	TEST_EQUAL(reg.isWellKnown("IS_AROMATIC"), true)
	TEST_EQUAL(reg.isWellKnown("ATOM_TYPE_NAME"), true)
	TEST_EQUAL(reg.isWellKnown("STEREO_DESCRIPTOR"), true)
	TEST_EQUAL(reg.isWellKnown("not_a_real_property"), false)

	// Materialising a well-known column on first registerColumn increments the
	// materialised count but is EXEMPT from the dynamic-cap accounting; the
	// canonical schema type is used regardless of the type passed in.
	PropertyColumnBase* pc = reg.registerColumn("PARTIAL_CHARGE", PropertyColumnType::FLOAT);
	TEST_NOT_EQUAL(pc, (PropertyColumnBase*)nullptr)
	TEST_EQUAL(pc->type() == PropertyColumnType::FLOAT, true)
	TEST_EQUAL(reg.columnCount(), 1u)
	TEST_EQUAL(reg.dynamicCount(), 0u)        // well-known: not counted against cap
	// Idempotent: second register returns the same materialised column.
	TEST_EQUAL(reg.registerColumn("PARTIAL_CHARGE", PropertyColumnType::FLOAT), pc)
	TEST_EQUAL(reg.columnCount(), 1u)
	// A second well-known materialises too, still cap-exempt.
	reg.registerColumn("AMBER_TYPE", PropertyColumnType::STRING);
	TEST_EQUAL(reg.columnCount(), 2u)
	TEST_EQUAL(reg.dynamicCount(), 0u)

	// HCP-1P.C: a well-known name always materialises with its CANONICAL
	// schema type, ignoring a mismatched caller type (fresh registry).
	MoleculeStore store_mt;
	auto* mt = store_mt.sideTables_().property_columns_.registerColumn("PARTIAL_CHARGE", PropertyColumnType::INT);
	TEST_NOT_EQUAL(mt, (PropertyColumnBase*)nullptr)
	TEST_EQUAL(mt->type() == PropertyColumnType::FLOAT, true)
RESULT

CHECK(PropertyColumn<float> -- set / get / clear / presence)
	MoleculeStore store;
	// HCP-1P.C: well-known columns materialise on first registerColumn (lazy);
	// findColumn before any write now returns nullptr (deliberate).
	auto* col = store.sideTables_().property_columns_.registerColumn("PARTIAL_CHARGE", PropertyColumnType::FLOAT);
	TEST_NOT_EQUAL(col, (PropertyColumnBase*)nullptr)
	TEST_EQUAL(col->type() == PropertyColumnType::FLOAT, true)

	auto* float_col = static_cast<PropertyColumn<float>*>(col);
	TEST_EQUAL(float_col->isSet(0), false)
	TEST_EQUAL(float_col->get(0), 0.0f)

	float_col->set(0, 0.25f);
	float_col->set(7, -1.5f);
	TEST_EQUAL(float_col->isSet(0), true)
	TEST_EQUAL(float_col->isSet(3), false)
	TEST_EQUAL(float_col->isSet(7), true)
	PRECISION(1e-6)
	TEST_REAL_EQUAL(float_col->get(0),  0.25f)
	TEST_REAL_EQUAL(float_col->get(7), -1.5f)

	float_col->clear(0);
	TEST_EQUAL(float_col->isSet(0), false)
	TEST_EQUAL(float_col->get(0), 0.0f)
	TEST_EQUAL(float_col->isSet(7), true)
RESULT

CHECK(StringPropertyColumn -- intern pool dedups repeated strings)
	MoleculeStore store;
	// HCP-1P.C: lazy materialise on first registerColumn.
	auto* col = store.sideTables_().property_columns_.registerColumn("AMBER_TYPE", PropertyColumnType::STRING);
	TEST_NOT_EQUAL(col, (PropertyColumnBase*)nullptr)
	auto* str_col = static_cast<StringPropertyColumn*>(col);

	// First set: pool grows by len("C")+1 = 2 bytes (plus initial 1-byte
	// empty-string sentinel, total 3).
	str_col->set(0, "C");
	std::size_t pool_after_first = str_col->poolSize();
	TEST_EQUAL(pool_after_first, 3u)

	// Second set of same value: intern hit; pool size unchanged.
	str_col->set(1, "C");
	TEST_EQUAL(str_col->poolSize(), pool_after_first)

	// Reads work for both.
	TEST_EQUAL(str_col->get(0), "C")
	TEST_EQUAL(str_col->get(1), "C")

	// Distinct value: pool grows.
	str_col->set(2, "CA");
	TEST_EQUAL(str_col->poolSize(), pool_after_first + 3u)  // "CA\0" = 3 bytes
	TEST_EQUAL(str_col->get(2), "CA")
	TEST_EQUAL(str_col->isSet(2), true)
	TEST_EQUAL(str_col->isSet(3), false)
RESULT

CHECK(registerColumn -- dynamic columns count against cap)
	MoleculeStore store;
	PropertyColumnRegistry& reg = store.sideTables_().property_columns_;

	// Well-known count doesn't reflect into dynamicCount.
	TEST_EQUAL(reg.dynamicCount(), 0u)

	auto* a = reg.registerColumn("my_debug_flag", PropertyColumnType::BOOL);
	TEST_NOT_EQUAL(a, (PropertyColumnBase*)nullptr)
	TEST_EQUAL(reg.dynamicCount(), 1u)
	TEST_EQUAL(reg.isWellKnown("my_debug_flag"), false)

	// Idempotent: re-register same name returns existing.
	auto* a2 = reg.registerColumn("my_debug_flag", PropertyColumnType::BOOL);
	TEST_EQUAL(a, a2)
	TEST_EQUAL(reg.dynamicCount(), 1u)

	// Different type, same name: returns the existing (does NOT
	// create a parallel column). Type-conflict resolution is the
	// sparse-bag's job per D23b.
	auto* a3 = reg.registerColumn("my_debug_flag", PropertyColumnType::INT);
	TEST_EQUAL(a3, a)
RESULT

CHECK(setMaxDynamic -- registerColumn returns nullptr past cap)
	MoleculeStore store;
	PropertyColumnRegistry& reg = store.sideTables_().property_columns_;
	reg.setMaxDynamic(3);

	TEST_NOT_EQUAL(reg.registerColumn("a", PropertyColumnType::INT),   (PropertyColumnBase*)nullptr)
	TEST_NOT_EQUAL(reg.registerColumn("b", PropertyColumnType::INT),   (PropertyColumnBase*)nullptr)
	TEST_NOT_EQUAL(reg.registerColumn("c", PropertyColumnType::INT),   (PropertyColumnBase*)nullptr)
	TEST_EQUAL    (reg.registerColumn("d", PropertyColumnType::INT),   (PropertyColumnBase*)nullptr)
	TEST_EQUAL    (reg.dynamicCount(), 3u)

	// Already-registered names continue to work after cap.
	auto* a = reg.findColumn("a");
	TEST_NOT_EQUAL(a, (const PropertyColumnBase*)nullptr)
RESULT

CHECK(sparse_bag_ -- set / find / replace / clear)
	MoleculeStore store;
	MoleculeStoreSideTables& s = store.sideTables_();

	MoleculeStoreSideTables::SparseProperty p;
	p.name    = "debug_marker";
	p.type    = PropertyColumnType::INT;
	p.v_int   = 42;
	bool fresh = s.sparse_set_(7, p);
	TEST_EQUAL(fresh, true)

	auto* found = s.sparse_find_(7, "debug_marker");
	TEST_NOT_EQUAL(found, (MoleculeStoreSideTables::SparseProperty*)nullptr)
	TEST_EQUAL(found->v_int, 42)

	// Mismatched-type replace stays sparse and overrides the entry.
	p.type    = PropertyColumnType::STRING;
	p.v_int   = 0;
	p.v_string = "now a string";
	bool fresh2 = s.sparse_set_(7, p);
	TEST_EQUAL(fresh2, false)
	auto* found2 = s.sparse_find_(7, "debug_marker");
	TEST_EQUAL(found2->type == PropertyColumnType::STRING, true)
	TEST_EQUAL(found2->v_string, "now a string")

	// Clear by name; entry removed; bucket pruned when empty.
	bool removed = s.sparse_clear_(7, "debug_marker");
	TEST_EQUAL(removed, true)
	TEST_EQUAL(s.sparse_find_(7, "debug_marker"), (const MoleculeStoreSideTables::SparseProperty*)nullptr)
	TEST_EQUAL(s.sparse_bag_.count(7), 0u)

	// Clear of missing name returns false.
	TEST_EQUAL(s.sparse_clear_(7, "anything"), false)
RESULT

CHECK(promote_sparse_ -- moves matching-type entries to dense column)
	MoleculeStore store;
	MoleculeStoreSideTables& s = store.sideTables_();

	// Stash 5 sparse FLOAT entries under name "promote_me" + 1
	// mismatched STRING entry at idx 999 (stays sparse).
	for (std::uint32_t i = 0; i < 5; ++i)
	{
		MoleculeStoreSideTables::SparseProperty p;
		p.name    = "promote_me";
		p.type    = PropertyColumnType::FLOAT;
		p.v_float = float(i) * 0.5f;
		s.sparse_set_(i, p);
	}
	{
		MoleculeStoreSideTables::SparseProperty p;
		p.name     = "promote_me";
		p.type     = PropertyColumnType::STRING;
		p.v_string = "oddball";
		s.sparse_set_(999, p);
	}

	std::size_t moved = s.promote_sparse_("promote_me", PropertyColumnType::FLOAT);
	TEST_EQUAL(moved, 5u)

	// Dense column now exists and holds the 5 FLOAT entries.
	auto* col = s.property_columns_.findColumn("promote_me");
	TEST_NOT_EQUAL(col, (PropertyColumnBase*)nullptr)
	TEST_EQUAL(col->type() == PropertyColumnType::FLOAT, true)
	auto* float_col = static_cast<PropertyColumn<float>*>(col);
	PRECISION(1e-6)
	TEST_REAL_EQUAL(float_col->get(0), 0.0f)
	TEST_REAL_EQUAL(float_col->get(4), 2.0f)
	TEST_EQUAL(float_col->isSet(0), true)
	TEST_EQUAL(float_col->isSet(4), true)

	// Mismatched-type STRING entry at 999 stayed sparse.
	auto* still_sparse = s.sparse_find_(999, "promote_me");
	TEST_NOT_EQUAL(still_sparse, (const MoleculeStoreSideTables::SparseProperty*)nullptr)
	TEST_EQUAL(still_sparse->v_string, "oddball")
RESULT

CHECK(selected_bits_ -- resize + atomic set/clear/is_selected)
	MoleculeStore store;
	MoleculeStoreSideTables& s = store.sideTables_();

	// Initial state: no words allocated.
	TEST_EQUAL(s.selected_bits_word_capacity_, 0u)
	TEST_EQUAL(s.is_selected_(0), false)
	TEST_EQUAL(s.is_selected_(63), false)
	TEST_EQUAL(s.is_selected_(100000), false)

	// Resize covers 200 atoms -> 4 words.
	s.resize_selected_bits_(200);
	TEST_EQUAL(s.selected_bits_word_capacity_, 4u)

	s.set_selected_(5, true);
	s.set_selected_(64, true);
	s.set_selected_(199, true);
	TEST_EQUAL(s.is_selected_(5), true)
	TEST_EQUAL(s.is_selected_(63), false)
	TEST_EQUAL(s.is_selected_(64), true)
	TEST_EQUAL(s.is_selected_(199), true)

	s.set_selected_(64, false);
	TEST_EQUAL(s.is_selected_(64), false)
	TEST_EQUAL(s.is_selected_(5), true)
	TEST_EQUAL(s.is_selected_(199), true)

	// Grow: contents preserved.
	s.resize_selected_bits_(500);
	TEST_EQUAL(s.selected_bits_word_capacity_, 8u)
	TEST_EQUAL(s.is_selected_(5), true)
	TEST_EQUAL(s.is_selected_(199), true)
	TEST_EQUAL(s.is_selected_(450), false)

	s.set_selected_(450, true);
	TEST_EQUAL(s.is_selected_(450), true)

	// Shrink: out-of-range reads return false.
	s.resize_selected_bits_(100);
	TEST_EQUAL(s.selected_bits_word_capacity_, 2u)
	TEST_EQUAL(s.is_selected_(5), true)
	TEST_EQUAL(s.is_selected_(450), false)
RESULT

CHECK(P2.1.0 -- Composite::getCompositeStore_() virtual hook)
	// v2.1 P2.1.0 (D36 + R20b-2 + R20d): virtual store-reach hook.
	// Default Composite returns nullptr (free-standing); Atom returns
	// store_; System returns store_.get(). Verified here so the
	// dispatch contract is locked before P2.1.1 wires the actual
	// composite-mutation maintenance.
	{
		// Free-standing non-Atom Composite: nullptr.
		Molecule mol_free;
		TEST_EQUAL(mol_free.getCompositeStore_(), (MoleculeStore*)nullptr)
	}

	{
		// Atom: orphan store binding from default ctor.
		Atom a;
		MoleculeStore* atom_store = a.getCompositeStore_();
		TEST_NOT_EQUAL(atom_store, (MoleculeStore*)nullptr)
		TEST_EQUAL(atom_store, a.getStore())
	}

	{
		// System: its own store_.
		System sys;
		MoleculeStore* sys_store = sys.getCompositeStore_();
		TEST_NOT_EQUAL(sys_store, (MoleculeStore*)nullptr)
	}

	{
		// Tree: Molecule inserted into System. Molecule doesn't
		// override getCompositeStore_; the default walks parent chain
		// to System whose override returns its store. So once mol is
		// inside sys, mol's getCompositeStore_() returns sys's store.
		System sys;
		Molecule mol;
		Atom a;
		mol.insert(a);
		sys.insert(mol);
		TEST_NOT_EQUAL(sys.getCompositeStore_(), (MoleculeStore*)nullptr)
		// mol reaches sys's store via parent-chain walk.
		TEST_EQUAL(mol.getCompositeStore_(), sys.getCompositeStore_())
		// The atom moves to System's store via adoptSubtree, so its
		// direct override returns the same store.
		TEST_EQUAL(a.getCompositeStore_(), sys.getCompositeStore_())
	}
RESULT

CHECK(P2.1.1 -- mirrorToSideTable_ direct API)
	// P2.1.1 scope: scaffolding only. The mutation-path wiring of
	// appendChild/removeChild/etc. was attempted but the cascading
	// destructor mirror calls (when ~System tears down children
	// mid-teardown) corrupted the heap. P2.1.2 ships the wiring with
	// proper destruction-in-progress detection.
	//
	// This test exercises the mirror helper DIRECTLY (not through
	// Composite mutations) so the helper's contract is locked
	// independently of the wiring sequence.
	Atom a_parent;
	Atom a_c1;
	Atom a_c2;

	// Call mirror directly on isolated atoms. Each gets a handle in
	// the orphan store via its Atom::getCompositeStore_ override.
	// They have no parent_ so nodes get default-topology (all-null
	// neighbors, child_count=0).
	a_parent.mirrorToSideTable_();
	a_c1.mirrorToSideTable_();
	a_c2.mirrorToSideTable_();

	MoleculeStore* store = a_parent.getCompositeStore_();
	TEST_NOT_EQUAL(store, (MoleculeStore*)nullptr)
	MoleculeStoreSideTables& s = store->sideTables_();

	CompositeHandle p_h  = a_parent.getCompositeHandle_();
	CompositeHandle c1_h = a_c1.getCompositeHandle_();
	CompositeHandle c2_h = a_c2.getCompositeHandle_();
	TEST_EQUAL(p_h.isNull(), false)
	TEST_EQUAL(c1_h.isNull(), false)
	TEST_EQUAL(c2_h.isNull(), false)

	TEST_EQUAL(p_h.kind == CompositeKind::ATOM, true)
	TEST_EQUAL(c1_h.kind == CompositeKind::ATOM, true)
	TEST_EQUAL(c2_h.kind == CompositeKind::ATOM, true)

	// All three atoms are free-standing -- their nodes have null
	// neighbors and child_count=0.
	const CompositeNode& p_n  = s.node_(p_h);
	const CompositeNode& c1_n = s.node_(c1_h);
	const CompositeNode& c2_n = s.node_(c2_h);
	TEST_EQUAL(p_n.parent.isNull(),  true)
	TEST_EQUAL(p_n.child_count, 0u)
	TEST_EQUAL(c1_n.parent.isNull(), true)
	TEST_EQUAL(c1_n.child_count, 0u)
	TEST_EQUAL(c2_n.parent.isNull(), true)
	TEST_EQUAL(c2_n.child_count, 0u)
RESULT

CHECK(P2.1.1 -- free-standing non-Atom Composite gets no handle)
	// Molecule has no override of getCompositeStore_ -- the default
	// walks parent (null) -> returns nullptr -> mirror is a no-op.
	// Confirms the design: only Composites with a reachable store
	// get handles, and free-standing non-Atom Composites stay
	// unrepresented in any store's side table.
	Molecule mol_free;
	mol_free.mirrorToSideTable_();
	TEST_EQUAL(mol_free.getCompositeHandle_().isNull(), true)
RESULT


CHECK(sizeof pins -- D22b CompositeHandle 8 B / CompositeNode 48 B)
	// These are static_asserted in _moleculeStoreInternal.h but pin
	// them in the test as well so a layout regression shows up in
	// ctest, not just at compile time.
	TEST_EQUAL(sizeof(CompositeHandle), 8u)
	TEST_EQUAL(sizeof(CompositeNode), 48u)
RESULT

END_TEST
