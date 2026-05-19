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
	TEST_EQUAL(h0.idx, 0u)
	TEST_EQUAL(s.composite_nodes_.size(), 1u)
	TEST_EQUAL(s.composite_free_list_.size(), 0u)

	CompositeHandle h1 = s.allocate_composite_node_(CompositeKind::MOLECULE);
	TEST_EQUAL(h1.kind == CompositeKind::MOLECULE, true)
	TEST_EQUAL(h1.idx, 1u)
	TEST_EQUAL(s.composite_nodes_.size(), 2u)

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
	TEST_EQUAL(s.composite_nodes_.size(), 2u)

	// Release h0: vector size unchanged, free list grows.
	s.release_composite_node_(h0);
	TEST_EQUAL(s.composite_nodes_.size(), 2u)
	TEST_EQUAL(s.composite_free_list_.size(), 1u)

	// Topology is zeroed on release (no stale links).
	TEST_EQUAL(s.composite_nodes_[h0.idx].parent.isNull(), true)
	TEST_EQUAL(s.composite_nodes_[h0.idx].kind == CompositeKind::NONE, true)

	// Next allocate reuses the slot.
	CompositeHandle h2 = s.allocate_composite_node_(CompositeKind::BOND);
	TEST_EQUAL(h2.idx, h0.idx)
	TEST_EQUAL(h2.kind == CompositeKind::BOND, true)
	TEST_EQUAL(s.composite_nodes_.size(), 2u)
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

CHECK(sizeof(CompositeHandle) and sizeof(CompositeNode) -- D22b pins)
	// These are static_asserted in _moleculeStoreInternal.h but pin
	// them in the test as well so a layout regression shows up in
	// ctest, not just at compile time.
	TEST_EQUAL(sizeof(CompositeHandle), 8u)
	TEST_EQUAL(sizeof(CompositeNode), 48u)
RESULT

END_TEST
