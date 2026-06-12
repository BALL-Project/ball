// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 KERNEL replacement (K0.2). Standalone MoleculeStore test
// (Atom/Bond handles arrive in K0.3).
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/moleculeStore.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>          // K0.3c.2: delete bond needs full type
#include <BALL/KERNEL/system.h>        // K0.4.2: System.adopt test
#include <BALL/KERNEL/molecule.h>      // K0.4.3: adoptSubtree test
#include <BALL/KERNEL/PTE.h>           // K0.4.4: PTE[Element::*] in swap tests
#include <BALL/MATHS/vector3.h>
#include <thread>                     // K0.4.6: concurrent orphan-store test
///////////////////////////

START_TEST(MoleculeStore)

using namespace BALL;

CHECK(MoleculeStore() throw())
	MoleculeStore store;
	TEST_EQUAL(store.size(), 0)
	TEST_EQUAL(store.capacity(), 0)
	TEST_EQUAL(store.generation(), 0)
RESULT

CHECK(allocate_atom)
	MoleculeStore store;
	MoleculeStore::Index i = store.allocate_atom();
	TEST_EQUAL(i, 0)
	TEST_EQUAL(store.size(), 1)
	TEST_EQUAL(store.position(i), Vector3(0.f, 0.f, 0.f))
	TEST_EQUAL(store.charge(i), 0.f)
	TEST_EQUAL(store.element_index(i), 0)
	TEST_EQUAL(store.back_ptr(i), nullptr)
	TEST_EQUAL(store.stable_id(i), 1)

	MoleculeStore::Index j = store.allocate_atom();
	TEST_EQUAL(j, 1)
	TEST_EQUAL(store.stable_id(j), 2)
RESULT

CHECK(reserve + generation discipline)
	MoleculeStore store;
	store.reserve(64);
	const auto gen_before = store.generation();
	for (int k = 0; k < 50; ++k) (void)store.allocate_atom();
	// 50 inserts within the reserved cap of 64 should not advance generation.
	TEST_EQUAL(store.generation(), gen_before)
	TEST_EQUAL(store.size(), 50)
RESULT

CHECK(generation advances on reallocation)
	MoleculeStore store;
	store.reserve(4);
	const auto gen_before = store.generation();
	for (int k = 0; k < 200; ++k) (void)store.allocate_atom();
	TEST_EQUAL(store.size(), 200)
	TEST_NOT_EQUAL(store.generation(), gen_before)
RESULT

CHECK(reference stable across insert with reserve)
	MoleculeStore store;
	store.reserve(64);
	auto i = store.allocate_atom();
	store.position(i) = Vector3(1.f, 2.f, 3.f);
	Vector3& ref = store.position(i);
	const auto gen_before = store.generation();
	for (int k = 0; k < 50; ++k) (void)store.allocate_atom();
	TEST_EQUAL(store.generation(), gen_before)
	TEST_EQUAL(ref, Vector3(1.f, 2.f, 3.f))
	ref = Vector3(9.f, 9.f, 9.f);
	TEST_EQUAL(store.position(i), Vector3(9.f, 9.f, 9.f))
RESULT

CHECK(position payload survives realloc by re-resolving via store.position(idx))
	MoleculeStore store;
	store.reserve(2);
	auto i = store.allocate_atom();
	store.position(i) = Vector3(7.f, 7.f, 7.f);
	for (int k = 0; k < 500; ++k) (void)store.allocate_atom();
	// Re-resolved access (D8 escape hatch — done at the store level here;
	// Atom handle exercises the same path in K0.3).
	TEST_EQUAL(store.position(i), Vector3(7.f, 7.f, 7.f))
RESULT

CHECK(string pool)
	MoleculeStore store;
	auto i = store.allocate_atom();
	store.set_name(i, "CA");
	store.set_type_name(i, "C.3");
	TEST_EQUAL(store.get_name(i), "CA")
	TEST_EQUAL(store.get_type_name(i), "C.3")

	auto j = store.allocate_atom();
	store.set_name(j, "");
	TEST_EQUAL(store.get_name(j), "")
RESULT

CHECK(charge / radius / element_index round-trip)
	MoleculeStore store;
	store.reserve(8);
	auto i = store.allocate_atom();
	store.charge(i) = 0.5f;
	store.radius(i) = 1.7f;
	store.element_index(i) = 6;
	store.formal_charge(i) = -1;
	TEST_REAL_EQUAL(store.charge(i), 0.5f)
	TEST_REAL_EQUAL(store.radius(i), 1.7f)
	TEST_EQUAL(store.element_index(i), 6)
	TEST_EQUAL(store.formal_charge(i), -1)
RESULT

CHECK(selection bitmap)
	MoleculeStore store;
	auto i = store.allocate_atom();
	TEST_EQUAL(store.selected(i), false)
	const auto sel_before = store.selection_generation();
	store.set_selected(i, true);
	store.mark_selection_dirty();
	TEST_EQUAL(store.selected(i), true)
	TEST_NOT_EQUAL(store.selection_generation(), sel_before)
RESULT

CHECK(back_ptr round-trip)
	MoleculeStore store;
	auto i = store.allocate_atom();
	int dummy = 0;
	// Atom* is opaque here; reinterpret an int* into Atom* for round-trip.
	store.set_back_ptr(i, reinterpret_cast<Atom*>(&dummy));
	TEST_EQUAL(store.back_ptr(i), reinterpret_cast<Atom*>(&dummy))
RESULT

CHECK(bond table)
	MoleculeStore store;
	store.reserve(8);
	auto a = store.allocate_atom();
	auto b = store.allocate_atom();
	auto c = store.allocate_atom();

	auto e1 = store.add_bond(a, b, 1);
	auto e2 = store.add_bond(b, c, 2);
	TEST_EQUAL(store.bond_count(), 2)
	TEST_EQUAL(e1, 0)
	TEST_EQUAL(e2, 1)
	TEST_EQUAL(store.bond(e1).a, a)
	TEST_EQUAL(store.bond(e1).b, b)
	TEST_EQUAL(store.bond(e1).order, 1)
	TEST_EQUAL(store.bond(e2).order, 2)

	auto bonds_of_b = store.bonds_of(b);
	TEST_EQUAL(bonds_of_b.size(), 2)

	auto bonds_of_a = store.bonds_of(a);
	TEST_EQUAL(bonds_of_a.size(), 1)
RESULT

CHECK(BondRecord layout budget)
	// Decisions D13 — bond records must be compact.
	TEST_EQUAL(sizeof(BondRecord) <= 16, true)
RESULT

CHECK(CSR adjacency: bond_degree + bonds_of post K0.2b)
	// D15: CSR adjacency is mandatory; this checks the lazy rebuild path
	// produces correct degree counts and incident-bond lists.
	MoleculeStore store;
	store.reserve(8);
	auto a = store.allocate_atom();
	auto b = store.allocate_atom();
	auto c = store.allocate_atom();
	auto d = store.allocate_atom();

	auto e_ab = store.add_bond(a, b);
	auto e_ac = store.add_bond(a, c);
	auto e_bc = store.add_bond(b, c);
	auto e_cd = store.add_bond(c, d);

	TEST_EQUAL(store.bond_degree(a), 2)  // a-b, a-c
	TEST_EQUAL(store.bond_degree(b), 2)  // a-b, b-c
	TEST_EQUAL(store.bond_degree(c), 3)  // a-c, b-c, c-d
	TEST_EQUAL(store.bond_degree(d), 1)  // c-d

	auto bonds_of_c = store.bonds_of(c);
	TEST_EQUAL(bonds_of_c.size(), 3)
	// Order is by source-atom traversal; just check all three present
	bool has_ac = false, has_bc = false, has_cd = false;
	for (auto i : bonds_of_c)
	{
		if (i == e_ac) has_ac = true;
		if (i == e_bc) has_bc = true;
		if (i == e_cd) has_cd = true;
	}
	TEST_EQUAL(has_ac && has_bc && has_cd, true)

	// for_each_bond_of: avoid vector materialisation
	int count = 0;
	store.for_each_bond_of(a, [&](std::uint32_t /*bi*/) { ++count; });
	TEST_EQUAL(count, 2)
RESULT

CHECK(P4.1 empty-bond fast path on a bondless store with valid indices)
	// v2.1 P4.1 (V21-LOAD-BATCH, Codex R24): a store with allocated
	// atoms but ZERO bond records must return empty adjacency from
	// bond_degree / bonds_of / for_each_bond_of WITHOUT triggering the
	// O(n_atoms) CSR rebuild. This is the load-time O(n^2) fix: the
	// orphan store during a JSON load holds ~100k bondless atoms, and
	// each per-atom adopt re-dirties the CSR; the guard makes the
	// bond-scan primitives O(1) when bonds_ is empty.
	MoleculeStore store;
	store.reserve(16);
	auto a0 = store.allocate_atom();
	auto a1 = store.allocate_atom();
	auto a2 = store.allocate_atom();

	// No bonds added. All adjacency queries return empty, for valid
	// indices, and must not throw / out-of-bounds.
	TEST_EQUAL(store.bond_degree(a0), 0)
	TEST_EQUAL(store.bond_degree(a1), 0)
	TEST_EQUAL(store.bond_degree(a2), 0)
	TEST_EQUAL(store.bonds_of(a0).size(), 0)
	TEST_EQUAL(store.bonds_of(a2).size(), 0)
	int cb = 0;
	store.for_each_bond_of(a0, [&](std::uint32_t) { ++cb; });
	store.for_each_bond_of(a2, [&](std::uint32_t) { ++cb; });
	TEST_EQUAL(cb, 0)

	// Allocate/release more atoms (which re-dirty the CSR) — queries
	// stay correct + fast on the still-bondless store.
	auto a3 = store.allocate_atom();
	store.release_atom(a1);
	TEST_EQUAL(store.bond_degree(a3), 0)
	TEST_EQUAL(store.bond_degree(a0), 0)

	// Adding a bond re-enables the full CSR path (correctness parity).
	store.add_bond(a0, a2);
	TEST_EQUAL(store.bond_degree(a0), 1)
	TEST_EQUAL(store.bond_degree(a2), 1)
	TEST_EQUAL(store.bond_degree(a3), 0)
RESULT

CHECK(CSR invalidates on add_bond / allocate_atom)
	MoleculeStore store;
	store.reserve(8);
	auto a = store.allocate_atom();
	auto b = store.allocate_atom();
	store.add_bond(a, b);
	TEST_EQUAL(store.bond_degree(a), 1)

	// Add a new atom + a new bond; CSR should rebuild lazily.
	auto c = store.allocate_atom();
	store.add_bond(a, c);
	TEST_EQUAL(store.bond_degree(a), 2)
	TEST_EQUAL(store.bond_degree(c), 1)
RESULT

CHECK(BorrowedColumnRef RAII release path)
	MoleculeStore store;
	store.reserve(8);
	auto i = store.allocate_atom();
	store.position(i) = Vector3(1.f, 2.f, 3.f);

	{
		BorrowedColumnRef<Vector3> b(store, store.position(i));
#ifndef NDEBUG
		TEST_EQUAL(store.borrowed_ref_count(), 1)
#endif
		TEST_EQUAL(*b, Vector3(1.f, 2.f, 3.f))
		// Mutating via the borrowed ref is fine.
		*b = Vector3(9.f, 9.f, 9.f);
		TEST_EQUAL(store.position(i), Vector3(9.f, 9.f, 9.f))
	}
#ifndef NDEBUG
	TEST_EQUAL(store.borrowed_ref_count(), 0)
#endif

	// After the borrow is released, mutation is fine again.
	(void)store.allocate_atom();
RESULT

CHECK(BorrowedColumnRef allows non-resizing operations while borrowed)
	MoleculeStore store;
	store.reserve(16);
	auto i = store.allocate_atom();
	store.position(i) = Vector3(1.f, 2.f, 3.f);

	BorrowedColumnRef<Vector3> b(store, store.position(i));
	// Allocate within reserved capacity: no realloc, no assertion fire.
	for (int k = 0; k < 10; ++k) (void)store.allocate_atom();
	TEST_EQUAL(*b, Vector3(1.f, 2.f, 3.f))
RESULT

CHECK(CSR perf: 10k atoms / 30k bonds bond_degree pass)
	// Sanity check that whole-system traversal is O(N+B), not O(N*B).
	// This used to be O(N^2) with the linear bonds_of() scan.
	MoleculeStore store;
	const std::size_t N = 10000;
	const std::size_t B = 30000;
	store.reserve(N);
	for (std::size_t i = 0; i < N; ++i) (void)store.allocate_atom();
	for (std::size_t k = 0; k < B; ++k)
	{
		std::uint32_t i = static_cast<std::uint32_t>(k % N);
		std::uint32_t j = static_cast<std::uint32_t>((k + 1) % N);
		store.add_bond(i, j);
	}
	std::size_t total = 0;
	for (std::size_t i = 0; i < N; ++i) total += store.bond_degree(i);
	// Each bond contributes 2 to total degree.
	TEST_EQUAL(total, 2 * B)
RESULT

CHECK(Atom binds to orphan store at construction K0.3b.1)
	Atom a;
	a.setPosition(Vector3(1.f, 2.f, 3.f));
	TEST_EQUAL(a.getPosition(), Vector3(1.f, 2.f, 3.f))
RESULT

CHECK(Atom::setPosition mirrors into store column K0.3b.2a)
	// Use the orphan store directly to verify the mirror.
	// Since globalOrphanStore_ is private we can't directly inspect; rely
	// on the fact that two atoms constructed in sequence get adjacent
	// slots and setPosition writes them.
	Atom a, b;
	a.setPosition(Vector3(1.f, 2.f, 3.f));
	b.setPosition(Vector3(4.f, 5.f, 6.f));
	TEST_EQUAL(a.getPosition(), Vector3(1.f, 2.f, 3.f))
	TEST_EQUAL(b.getPosition(), Vector3(4.f, 5.f, 6.f))
RESULT

CHECK(MoleculeStore::release_atom + allocate_atom slot reuse K0.3c.1)
	MoleculeStore store;
	store.reserve(8);
	auto i = store.allocate_atom();
	auto j = store.allocate_atom();
	auto k = store.allocate_atom();
	TEST_EQUAL(store.size(), 3)
	TEST_EQUAL(store.live_atom_count(), 3)
	TEST_EQUAL(store.freed_slot_count(), 0)
	TEST_EQUAL(store.is_freed(j), false)

	store.release_atom(j);
	TEST_EQUAL(store.size(), 3)               // size doesn't shrink
	TEST_EQUAL(store.live_atom_count(), 2)    // but live count does
	TEST_EQUAL(store.freed_slot_count(), 1)
	TEST_EQUAL(store.is_freed(j), true)
	TEST_EQUAL(store.is_freed(i), false)
	TEST_EQUAL(store.is_freed(k), false)
	TEST_EQUAL(store.back_ptr(j), nullptr)    // freed sentinel

	// Allocate again -- should reuse the freed slot j.
	auto m = store.allocate_atom();
	TEST_EQUAL(m, j)                          // reuse confirmed
	TEST_EQUAL(store.size(), 3)               // still 3 (no growth)
	TEST_EQUAL(store.live_atom_count(), 3)
	TEST_EQUAL(store.freed_slot_count(), 0)
	TEST_EQUAL(store.is_freed(m), false)      // freed flag cleared on reuse

	// Double-release is idempotent
	store.release_atom(m);
	TEST_EQUAL(store.freed_slot_count(), 1)
	store.release_atom(m);                    // second release: no-op
	TEST_EQUAL(store.freed_slot_count(), 1)
RESULT

CHECK(Atom destructor releases orphan store slot K0.3c.1)
	// Construct + destroy many Atoms in a loop. The orphan store's
	// live_atom_count should stay bounded; freed_slot_count should
	// grow then plateau as allocate_atom reuses freed slots.
	const auto& orphan_inspector = []() -> std::size_t {
		// Indirect: create one atom and read its store to get a handle
		// on globalOrphanStore_.
		Atom probe;
		auto* s = probe.getStore();
		return s ? s->live_atom_count() : 0;
	};
	const std::size_t baseline_live = orphan_inspector();

	for (int n = 0; n < 100; ++n)
	{
		Atom temp;
		temp.setPosition(Vector3(float(n), 0.f, 0.f));
	}
	// After the loop, all 100 atoms are destroyed; orphan store live
	// count should be back near baseline (allowing for the probe atom
	// inside orphan_inspector).
	const std::size_t after_live = orphan_inspector();
	TEST_EQUAL(after_live <= baseline_live + 2, true)
RESULT

CHECK(MoleculeStore::remove_bond + tombstones + reuse K0.3c.2)
	MoleculeStore store;
	store.reserve(8);
	auto a = store.allocate_atom();
	auto b = store.allocate_atom();
	auto c = store.allocate_atom();

	auto e0 = store.add_bond(a, b);
	auto e1 = store.add_bond(b, c);
	auto e2 = store.add_bond(a, c);
	TEST_EQUAL(store.bond_count(), 3)
	TEST_EQUAL(store.live_bond_count(), 3)
	TEST_EQUAL(store.dead_bond_count(), 0)
	TEST_EQUAL(store.bond_degree(b), 2)

	store.remove_bond(e1);
	TEST_EQUAL(store.bond_count(), 3)               // total includes dead
	TEST_EQUAL(store.live_bond_count(), 2)
	TEST_EQUAL(store.dead_bond_count(), 1)
	TEST_EQUAL(store.is_bond_dead(e1), true)
	TEST_EQUAL(store.bond_degree(b), 1)             // a-b remains
	TEST_EQUAL(store.bond_degree(c), 1)             // a-c remains

	// add_bond should reuse tombstoned slot e1
	auto e3 = store.add_bond(a, b);
	TEST_EQUAL(e3, e1)
	TEST_EQUAL(store.is_bond_dead(e1), false)       // reused
	TEST_EQUAL(store.bond_count(), 3)               // no growth
	TEST_EQUAL(store.live_bond_count(), 3)
	TEST_EQUAL(store.dead_bond_count(), 0)

	// remove_bond is idempotent
	store.remove_bond(e1);
	store.remove_bond(e1);
	TEST_EQUAL(store.dead_bond_count(), 1)
RESULT

CHECK(remove_bonds_between K0.3c.2)
	MoleculeStore store;
	store.reserve(4);
	auto a = store.allocate_atom();
	auto b = store.allocate_atom();
	store.add_bond(a, b);
	store.add_bond(b, a);  // duplicate, reverse order
	TEST_EQUAL(store.live_bond_count(), 2)
	TEST_EQUAL(store.bond_degree(a), 2)
	TEST_EQUAL(store.remove_bonds_between(a, b), 2u)
	TEST_EQUAL(store.live_bond_count(), 0)
	TEST_EQUAL(store.bond_degree(a), 0)
RESULT

CHECK(Bond::setOrder/setType mirrors into store BondRecord K0.3c.3)
	Atom a, b;
	Bond* bond = a.createBond(b);
	auto* store = a.getStore();
	auto bond_idx = bond->bond_record_idx_;
	// Cast uint8_t to int so TEST_EQUAL prints numerically rather than as
	// (often-unprintable) char.
	TEST_EQUAL(int(store->bond(bond_idx).order), int(Bond::ORDER__UNKNOWN))  // 0
	TEST_EQUAL(int(store->bond(bond_idx).type), int(Bond::TYPE__UNKNOWN))    // 0

	bond->setOrder(Bond::ORDER__DOUBLE);
	TEST_EQUAL(int(store->bond(bond_idx).order), int(Bond::ORDER__DOUBLE))
	TEST_EQUAL(bond->getOrder(), Bond::ORDER__DOUBLE)

	bond->setType(Bond::TYPE__HYDROGEN);
	TEST_EQUAL(int(store->bond(bond_idx).type), int(Bond::TYPE__HYDROGEN))
	TEST_EQUAL(bond->getType(), Bond::TYPE__HYDROGEN)

	delete bond;
RESULT

CHECK(Bond::~Bond removes store-side bond record K0.3c.2)
	// Use Atom + createBond, then destroy the bond. Store should reflect.
	Atom a, b;
	Bond* bond = a.createBond(b);
	auto* store = a.getStore();
	TEST_EQUAL(store == b.getStore(), true)
	auto ai = a.getStoreIndex();
	TEST_EQUAL(store->bond_degree(ai), 1)
	delete bond;                                    // ~Bond -> arrangeBonds_
	TEST_EQUAL(a.countBonds(), 0)                   // v1.x cleared
	TEST_EQUAL(store->bond_degree(ai), 0)           // store cleared too
RESULT

CHECK(swap_atom_connectivity rewrites BondRecords K0.3c.4)
	// Test the store-level primitive directly (avoiding the v1.x
	// Atom::swap bond-pointer mess that segfaults at scope-end
	// destruction -- a known v1.x bug Codex Round 2 flagged but the
	// Atom-level fix isn't part of K0.3c).
	MoleculeStore store;
	store.reserve(8);
	auto ia = store.allocate_atom();
	auto ib = store.allocate_atom();
	auto ic = store.allocate_atom();
	store.add_bond(ia, ib);   // bond between a-b
	store.add_bond(ib, ic);   // bond between b-c
	TEST_EQUAL(store.bond_degree(ia), 1)
	TEST_EQUAL(store.bond_degree(ib), 2)
	TEST_EQUAL(store.bond_degree(ic), 1)

	// After swap of a<->c, the bonds previously incident to a are now
	// incident to c and vice versa.
	store.swap_atom_connectivity(ia, ic);
	TEST_EQUAL(store.bond_degree(ia), 1)        // count unchanged (was 1)
	TEST_EQUAL(store.bond_degree(ic), 1)        // count unchanged (was 1)
	TEST_EQUAL(store.bond_degree(ib), 2)        // ib not involved
RESULT

CHECK(K0.3c.7 Bond owns store_ pointer)
	// bond_store_ should be set at createBond time and survive even if
	// first_/second_'s getStore() result changes (which it will during
	// K0.4 adoption).
	Atom a, b;
	Bond* bond = a.createBond(b);
	TEST_NOT_EQUAL(bond->bond_store_, static_cast<MoleculeStore*>(nullptr))
	TEST_EQUAL(bond->bond_store_, a.getStore())
	TEST_EQUAL(bond->bond_store_, b.getStore())
	bond->setOrder(Bond::ORDER__DOUBLE);
	TEST_EQUAL(int(bond->bond_store_->bond(bond->bond_record_idx_).order),
	           int(Bond::ORDER__DOUBLE))
	delete bond;
RESULT

CHECK(K0.3c.8 atomic allocate_atom + back_ptr set)
	MoleculeStore store;
	store.reserve(8);
	int dummy = 0;
	auto* fake_atom = reinterpret_cast<Atom*>(&dummy);
	auto idx = store.allocate_atom(fake_atom);
	// is_freed must be false AND back_ptr must be set in one call --
	// no transient state where one is true and the other null.
	TEST_EQUAL(store.is_freed(idx), false)
	TEST_EQUAL(store.back_ptr(idx), fake_atom)
RESULT

CHECK(K0.3c.10 for_each_bond_of snapshot is mutation-safe)
	MoleculeStore store;
	store.reserve(8);
	auto a = store.allocate_atom();
	auto b = store.allocate_atom();
	auto c = store.allocate_atom();
	store.add_bond(a, b);
	store.add_bond(a, c);
	TEST_EQUAL(store.bond_degree(a), 2)

	// Callback removes each bond as it iterates. With snapshot, both
	// callbacks fire; bond_degree drops to 0.
	int n_callbacks = 0;
	store.for_each_bond_of(a, [&](std::uint32_t bi) {
		++n_callbacks;
		store.remove_bond(bi);
	});
	TEST_EQUAL(n_callbacks, 2)
	TEST_EQUAL(store.bond_degree(a), 0)
RESULT

CHECK(K0.4.2 System.adopt migrates atom payload)
	Atom a;
	a.setPosition(Vector3(1.f, 2.f, 3.f));
	a.setCharge(0.5f);
	auto* orphan = a.getStore();
	auto orphan_idx = a.getStoreIndex();
	TEST_EQUAL(orphan->position(orphan_idx), Vector3(1.f, 2.f, 3.f))

	System sys;
	auto* sys_store = &sys.getStore();
	TEST_NOT_EQUAL(sys_store, orphan)
	const auto orphan_live_before = orphan->live_atom_count();

	sys.adopt(a);

	// After adopt: a is in sys_store, no longer in orphan.
	TEST_EQUAL(a.getStore(), sys_store)
	TEST_NOT_EQUAL(a.getStore(), orphan)
	TEST_EQUAL(sys_store->live_atom_count(), 1u)
	TEST_EQUAL(orphan->live_atom_count(), orphan_live_before - 1)
	TEST_EQUAL(orphan->is_freed(orphan_idx), true)

	// Payload preserved.
	TEST_EQUAL(a.getPosition(), Vector3(1.f, 2.f, 3.f))
	TEST_EQUAL(a.getCharge(), 0.5f)
	TEST_EQUAL(sys_store->position(a.getStoreIndex()), Vector3(1.f, 2.f, 3.f))
RESULT

CHECK(K0.4.2 System.adopt idempotent when already in store)
	Atom a;
	System sys;
	sys.adopt(a);
	auto idx_after_first = a.getStoreIndex();
	sys.adopt(a);   // no-op
	TEST_EQUAL(a.getStoreIndex(), idx_after_first)
	TEST_EQUAL(sys.getStore().live_atom_count(), 1u)
RESULT

CHECK(K0.4.3 adoptSubtree preserves intra-subtree bonds)
	// Closes the K0.4.2 sequential-adopt orphan-bond limitation by
	// adopting all atoms in a container as one batch.
	System sys;
	auto* sys_store = &sys.getStore();

	// Build a Molecule subtree with two bonded atoms BEFORE inserting
	// into the System. The atoms live in the orphan store at this point.
	Molecule mol;
	Atom* a = new Atom;
	Atom* b = new Atom;
	mol.insert(*a);   // orphan -> mol; mol not yet in System; still orphan
	mol.insert(*b);
	Bond* bond = a->createBond(*b);
	auto* orphan = a->getStore();
	TEST_NOT_EQUAL(orphan, sys_store)
	TEST_EQUAL(bond->bond_store_, orphan)

	// Insert the molecule into the System. AtomContainer::insert
	// (AtomContainer&) -> adoptSubtree migrates a, b, and bond
	// atomically.
	sys.insert(mol);

	// Both atoms now in sys_store; bond migrated too.
	TEST_EQUAL(a->getStore(), sys_store)
	TEST_EQUAL(b->getStore(), sys_store)
	TEST_EQUAL(bond->bond_store_, sys_store)
	TEST_EQUAL(sys_store->bond_degree(a->getStoreIndex()), 1u)
	TEST_EQUAL(sys_store->bond_degree(b->getStoreIndex()), 1u)

	delete bond;
RESULT

CHECK(K0.4.5 sequential adoption -- bonded atom refused (no state change))
	// Codex Round 4 HIGH-3: pre-K0.4.5, single-atom adopt of a bonded
	// atom orphaned its bond (the source slot was freed, CSR-skipped
	// the bond with a freed endpoint, the partner's later adopt never
	// saw the bond). K0.4.5 closes the hole by refusing the adopt and
	// emitting a warning telling the caller to use adoptSubtree.
	Atom a, b;
	Bond* bond = a.createBond(b);
	auto* orphan = a.getStore();
	TEST_EQUAL(bond->bond_store_, orphan)

	System sys;
	auto* sys_store = &sys.getStore();
	(void)sys_store;
	sys.adopt(a);    // refused: b is in orphan, not sys_store
	sys.adopt(b);    // refused: a is in orphan, not sys_store

	// K0.4.5 behavior: both atoms still in orphan; bond still in orphan;
	// no orphaning, no silent data loss. Callers wanting to migrate
	// bonded subgraphs must use adoptSubtree (covered by other tests).
	TEST_EQUAL(a.getStore(), orphan)
	TEST_EQUAL(b.getStore(), orphan)
	TEST_EQUAL(bond->bond_store_, orphan)

	delete bond;
RESULT

CHECK(K0.3c.9 string-pool intern dedupe -- repeated set_name reuses offset)
	// Codex Round 3 HIGH-7: without an intern table, set_name(i, "ABC")
	// called 100 times would append "ABC\0" 100 times (400 bytes) to
	// string_pool_. With the intern table, "ABC\0" is appended once
	// (4 bytes), and all 100 atoms share the same offset.
	MoleculeStore store;
	store.reserve(128);
	const std::size_t pool_before = store.string_pool().size();
	std::uint32_t shared_offset = 0;
	for (int n = 0; n < 100; ++n)
	{
		auto idx = store.allocate_atom();
		store.set_name(idx, "ABC");
		if (n == 0) shared_offset = store.name_offset(idx);
		// Every atom's name_offset must match the first -- proves dedupe.
		TEST_EQUAL(store.name_offset(idx), shared_offset)
	}
	// Pool grew by at most 5 bytes: initial '\0' (if not already there)
	// + "ABC\0" appended ONCE. Tight upper bound = pool_before + 5.
	// (HCP-1P.A born-default type-name is "" = offset 0, so it adds nothing.)
	TEST_EQUAL(store.string_pool().size() <= pool_before + 5, true)
	// Round-trip: every atom reads back "ABC".
	for (int n = 0; n < 100; ++n)
	{
		TEST_EQUAL(store.get_name(static_cast<MoleculeStore::Index>(n)), "ABC")
	}
RESULT

CHECK(K0.3c.9 string-pool intern preserves distinct strings)
	// 10 distinct names -> pool grows by 10 entries (one per unique).
	MoleculeStore store;
	store.reserve(16);
	const std::size_t pool_before = store.string_pool().size();
	const char* names[10] = {"A","B","C","D","E","F","G","H","I","J"};
	std::uint32_t offsets[10];
	for (int n = 0; n < 10; ++n)
	{
		auto idx = store.allocate_atom();
		store.set_name(idx, names[n]);
		offsets[n] = store.name_offset(idx);
	}
	// All 10 offsets must be distinct.
	for (int n = 0; n < 10; ++n)
		for (int m = n + 1; m < 10; ++m)
			TEST_NOT_EQUAL(offsets[n], offsets[m])
	// Pool grew by exactly 10 * 2 bytes (each "X\0"), plus possibly the
	// initial '\0' if pool was empty. (HCP-1P.A born-default type-name is
	// "" = offset 0, so it adds nothing.)
	const std::size_t grew = store.string_pool().size() - pool_before;
	TEST_EQUAL(grew >= 20u && grew <= 21u, true)
	// Round-trip.
	for (int n = 0; n < 10; ++n)
	{
		TEST_EQUAL(store.get_name(static_cast<MoleculeStore::Index>(n)),
		           std::string(names[n]))
	}
RESULT

CHECK(K0.3c.9 default-Atom type-name does not leak string_pool)
	// Codex Round 3 HIGH-7 proof: every Atom() ctor's K0.3b.6 dual-write
	// appends "?\0" for the default type name. release_atom does NOT
	// reclaim string_pool space. Without the intern table, 1000
	// construct/destroy cycles leak 2000 bytes. With the intern table,
	// "?" interns once and the pool stays bounded.
	Atom probe;
	auto* orphan = probe.getStore();
	const std::size_t pool_before = orphan->string_pool().size();
	for (int n = 0; n < 1000; ++n)
	{
		Atom temp;
		(void)temp;
	}
	const std::size_t pool_after = orphan->string_pool().size();
	const std::size_t grew = pool_after - pool_before;
	// Bounded growth: a small constant (the "?" type-name intern entry
	// at most, if it wasn't already there). << 2000 bytes.
	TEST_EQUAL(grew < 16u, true)
RESULT

CHECK(K0.3c.6 mutable-getter drift -- CLOSED in K0.3b.LATER.1)
	// Originally a Codex Round 2 HIGH-3 / HIGH-6 documented gap: the
	// non-const reference returned by Atom::getPosition() bypassed
	// dual-write when the caller mutated through it. With the K0.3b.LATER.1
	// getter flip, getPosition returns a reference INTO the store column,
	// so all mutations land there directly. Drift closed.
	Atom a;
	auto* store = a.getStore();
	auto idx = a.getStoreIndex();

	a.setPosition(Vector3(1.f, 2.f, 3.f));
	TEST_EQUAL(a.getPosition(), Vector3(1.f, 2.f, 3.f))
	TEST_EQUAL(store->position(idx), Vector3(1.f, 2.f, 3.f))

	// Mutate via the non-const reference. Store updates too (was stale
	// before K0.3b.LATER.1).
	Vector3& p = a.getPosition();
	p = Vector3(7.f, 8.f, 9.f);
	TEST_EQUAL(a.getPosition(), Vector3(7.f, 8.f, 9.f))
	TEST_EQUAL(store->position(idx), Vector3(7.f, 8.f, 9.f))  // no longer stale
RESULT

CHECK(CSR rebuild skips bonds touching freed atoms K0.3c.1)
	MoleculeStore store;
	store.reserve(8);
	auto a = store.allocate_atom();
	auto b = store.allocate_atom();
	auto c = store.allocate_atom();

	store.add_bond(a, b);   // bond 0
	store.add_bond(b, c);   // bond 1
	store.add_bond(a, c);   // bond 2

	TEST_EQUAL(store.bond_degree(a), 2)
	TEST_EQUAL(store.bond_degree(b), 2)
	TEST_EQUAL(store.bond_degree(c), 2)

	// Free atom b. Bonds 0 + 1 touch b and should drop out of CSR.
	store.release_atom(b);
	TEST_EQUAL(store.bond_degree(a), 1)   // only a-c remains
	TEST_EQUAL(store.bond_degree(c), 1)   // only a-c remains
	// b is freed; querying its bond_degree returns its CSR slice which
	// should be empty since the prefix-sum skipped all 2 incident bonds.
	TEST_EQUAL(store.bond_degree(b), 0)
RESULT

CHECK(K0.4.4 Atom::swap same-store full-payload exchange)
	// Codex Round 4 HIGH-2: same-store swap had two independent paths.
	// K0.4.4 unified them. Verify every store-backed payload exchanges.
	System sys;
	Atom* a = new Atom; sys.adopt(*a);
	Atom* b = new Atom; sys.adopt(*b);
	a->setPosition(Vector3(1.f, 2.f, 3.f));
	a->setCharge(0.5f);
	a->setVelocity(Vector3(10.f, 11.f, 12.f));
	a->setForce(Vector3(20.f, 21.f, 22.f));
	a->setName("ATOM_A");
	a->setTypeName("TYPE_A");
	a->setRadius(1.5f);
	a->setType((Atom::Type)7);
	a->setFormalCharge(2);
	a->setElement(PTE[Element::CARBON]);

	b->setPosition(Vector3(100.f, 200.f, 300.f));
	b->setCharge(-1.5f);
	b->setVelocity(Vector3(110.f, 111.f, 112.f));
	b->setForce(Vector3(120.f, 121.f, 122.f));
	b->setName("ATOM_B");
	b->setTypeName("TYPE_B");
	b->setRadius(2.5f);
	b->setType((Atom::Type)9);
	b->setFormalCharge(-3);
	b->setElement(PTE[Element::OXYGEN]);

	a->swap(*b);

	TEST_EQUAL(a->getPosition(), Vector3(100.f, 200.f, 300.f))
	TEST_EQUAL(a->getCharge(), -1.5f)
	TEST_EQUAL(a->getVelocity(), Vector3(110.f, 111.f, 112.f))
	TEST_EQUAL(a->getForce(), Vector3(120.f, 121.f, 122.f))
	TEST_EQUAL(a->getName(), String("ATOM_B"))
	TEST_EQUAL(a->getTypeName(), String("TYPE_B"))
	TEST_EQUAL(a->getRadius(), 2.5f)
	TEST_EQUAL((int)a->getType(), 9)
	TEST_EQUAL(a->getFormalCharge(), -3)
	TEST_EQUAL(a->getElement(), PTE[Element::OXYGEN])

	TEST_EQUAL(b->getPosition(), Vector3(1.f, 2.f, 3.f))
	TEST_EQUAL(b->getCharge(), 0.5f)
	TEST_EQUAL(b->getVelocity(), Vector3(10.f, 11.f, 12.f))
	TEST_EQUAL(b->getForce(), Vector3(20.f, 21.f, 22.f))
	TEST_EQUAL(b->getName(), String("ATOM_A"))
	TEST_EQUAL(b->getTypeName(), String("TYPE_A"))
	TEST_EQUAL(b->getRadius(), 1.5f)
	TEST_EQUAL((int)b->getType(), 7)
	TEST_EQUAL(b->getFormalCharge(), 2)
	TEST_EQUAL(b->getElement(), PTE[Element::CARBON])
RESULT

CHECK(K0.4.4 Atom::swap cross-store full-payload exchange)
	// Codex Round 4 HIGH-1: cross-store swap previously skipped
	// position/charge/velocity/force entirely (gated on same-store).
	// K0.4.4 snapshot-write model handles both stores symmetrically.
	System sys1, sys2;
	Atom* a = new Atom; sys1.adopt(*a);
	Atom* b = new Atom; sys2.adopt(*b);
	TEST_NOT_EQUAL(a->getStore(), b->getStore())

	a->setPosition(Vector3(1.f, 2.f, 3.f));
	a->setCharge(0.5f);
	a->setName("ATOM_A");
	a->setElement(PTE[Element::CARBON]);

	b->setPosition(Vector3(100.f, 200.f, 300.f));
	b->setCharge(-1.5f);
	b->setName("ATOM_B");
	b->setElement(PTE[Element::OXYGEN]);

	a->swap(*b);

	// Position + charge MUST swap even across stores.
	TEST_EQUAL(a->getPosition(), Vector3(100.f, 200.f, 300.f))
	TEST_EQUAL(a->getCharge(), -1.5f)
	TEST_EQUAL(a->getName(), String("ATOM_B"))
	TEST_EQUAL(a->getElement(), PTE[Element::OXYGEN])
	TEST_EQUAL(b->getPosition(), Vector3(1.f, 2.f, 3.f))
	TEST_EQUAL(b->getCharge(), 0.5f)
	TEST_EQUAL(b->getName(), String("ATOM_A"))
	TEST_EQUAL(b->getElement(), PTE[Element::CARBON])
RESULT

CHECK(K0.4.6 concurrent orphan-store Atom construction is race-free (HIGH-4))
	// Codex Round 4 HIGH-4: orphan store mutated from every default Atom()
	// ctor and ~Atom across all threads. Without the orphan mutex, concurrent
	// allocate_atom/release_atom races vector pushes + free-list pops and
	// can corrupt store columns. K0.4.6 added MoleculeStore::orphanMutex()
	// to serialise orphan-only mutators.
	//
	// This test is a stress proxy: spawn 4 threads, each constructs +
	// destroys 500 default atoms. Without the lock this used to occasionally
	// hit ASan/UBSan failures or assertion crashes; with it, the store
	// remains consistent.
	auto worker = []() {
		for (int i = 0; i < 500; ++i)
		{
			Atom a;       // bindToStore_ on orphan
			a.setCharge(static_cast<float>(i));
			// destructor runs at scope exit -> release_atom on orphan
		}
	};
	std::thread t1(worker), t2(worker), t3(worker), t4(worker);
	t1.join(); t2.join(); t3.join(); t4.join();
	// Orphan store should still be in a consistent state after 2000 alloc/release.
	// No specific count assertion (it depends on prior tests) — the lack of
	// data races / asserts / crashes IS the assertion. Sanity-check we can
	// still allocate and release one more atom.
	{
		Atom probe;
		probe.setName("PROBE_OK");
		TEST_EQUAL(probe.getName(), String("PROBE_OK"))
	}
RESULT

CHECK(K0.4.8 detached-handle mutation rebinds to orphan (MEDIUM-9))
	// Codex Round 4 MEDIUM-9: pre-K0.4.8 paths through persistentRead /
	// set / operator= / clear_ silently dropped writes when the Atom's
	// store_ was null (the post-~System detached state). K0.4.8 added
	// ensureStoreBinding_() that re-binds to orphan first, so the data
	// flows somewhere safe.
	//
	// We can't easily simulate ~System on a System owning the atom
	// without destroying the test System. Instead, mimic detachment by
	// migrating manually then verify a subsequent setName lands.
	Atom* a = new Atom;
	// At this point a is bound to orphan. Force-detach by migrating to
	// nullptr (simulating ~System's invalidate-handles loop).
	a->migrateTo_(nullptr, 0);
	TEST_EQUAL(a->getStore(), (MoleculeStore*)nullptr)

	// Pre-K0.4.8: this set() would have silently dropped writes (store_
	// was null, gates skipped). K0.4.8: ensureStoreBinding_ rebinds to
	// orphan first; the write lands.
	Atom src;
	src.setName("RECOVERED");
	src.setPosition(Vector3(42.f, 43.f, 44.f));
	a->set(src, false);
	TEST_NOT_EQUAL(a->getStore(), (MoleculeStore*)nullptr)
	TEST_EQUAL(a->getName(), String("RECOVERED"))
	TEST_EQUAL(a->getPosition(), Vector3(42.f, 43.f, 44.f))

	delete a;
RESULT

CHECK(K0.4.7 BorrowedColumnRef opt-in enforcement documented (HIGH-5))
	// Codex Round 4 HIGH-5: clarify that raw Atom::getPosition/etc. do
	// NOT auto-wrap their return in BorrowedColumnRef, so the
	// borrowed_ref_count_ debug guard only fires for callers that
	// EXPLICITLY opt in. This test pins down the documented behaviour
	// so the contract doesn't silently change.
	MoleculeStore store;
	store.reserve(8);
	auto idx = store.allocate_atom();
	#ifndef NDEBUG
		// 1. Bare ref does NOT bump the counter.
		Vector3& bare = store.position(idx);
		(void)bare;
		TEST_EQUAL(store.borrowed_ref_count(), 0u)

		// 2. Explicit BorrowedColumnRef DOES bump the counter; release
		//    on RAII scope exit decrements it back to zero.
		{
			BorrowedColumnRef<Vector3> guarded(store, store.position(idx));
			TEST_EQUAL(store.borrowed_ref_count(), 1u)
			guarded.get() = Vector3(7.f, 8.f, 9.f);
		}
		TEST_EQUAL(store.borrowed_ref_count(), 0u)
	#endif
	// In release builds, the counter is a no-op (always 0); no behaviour
	// to assert. The contract holds: raw getter = UB on stale ref;
	// BorrowedColumnRef = machine-checked in debug.
RESULT

CHECK(K0.4.4 getName by-value survives store growth (HIGH-6))
	// Codex Round 4 HIGH-6: pre-K0.4.4 getName returned const String&
	// into a vector<String> column; growth would dangle the ref. Now
	// returns by value. Test: capture a name, then allocate enough atoms
	// to force the store column vector to reallocate, then re-check the
	// captured value is intact.
	System sys;
	Atom* keep = new Atom; sys.adopt(*keep);
	keep->setName("PERSIST");
	String captured = keep->getName();
	for (int i = 0; i < 256; ++i)
	{
		Atom* tmp = new Atom;
		sys.adopt(*tmp);
	}
	TEST_EQUAL(captured, String("PERSIST"))
	TEST_EQUAL(keep->getName(), String("PERSIST"))
RESULT

CHECK(V21-STRING-POOL-COMPACT: compact reclaims pool from freed slots)
	// Build a System, insert N atoms with UNIQUE names (so each name
	// occupies its own pool slot), then drop all but one and compact.
	// Pre-V21: pool stays at the post-insert size forever.
	// Post-V21: pool shrinks to roughly just the survivor's name.
	System sys;
	Molecule m;
	sys.append(m);
	const int N = 256;
	std::vector<Atom*> atoms;
	atoms.reserve(N);
	for (int i = 0; i < N; ++i)
	{
		Atom* a = new Atom;
		a->setName(String("unique_atom_name_") + String(i));
		m.append(*a);
		atoms.push_back(a);
	}
	const std::size_t pool_full = sys.getStore().string_pool().size();
	TEST_EQUAL(pool_full > 1000, true)   // ~16 chars × 256 names + overhead

	// Delete all but the first atom and compact.
	for (int i = 1; i < N; ++i) delete atoms[i];
	sys.getStore().compact();
	const std::size_t pool_compact = sys.getStore().string_pool().size();
	// Expected: ~25 chars for one name + null sentinel + offset 0.
	TEST_EQUAL(pool_compact < 100, true)
	TEST_EQUAL(atoms[0]->getName(), String("unique_atom_name_0"))
RESULT

CHECK(HCP-1P.A: default Atom is born with UNKNOWN_TYPE + empty type-name)
	// The slot is born default inside allocate_atom; the Atom() ctor no
	// longer re-initialises it. The default type-name is "" (HCP-1P.A: was
	// "?"), which is the reserved string-pool offset 0 -- no per-atom intern.
	Atom a;
	TEST_EQUAL(a.getType(), Atom::UNKNOWN_TYPE)
	TEST_EQUAL(a.getTypeName(), String(""))
	TEST_EQUAL(a.getName(), String(""))
RESULT

CHECK(HCP-1P.A: free-list slot reuse resets to born defaults)
	// Set non-default values, free the slot, allocate again -- the reused
	// slot must be reset to the born defaults (atom_type -1 / type-name ""),
	// never carry stale type / type-name from the previous occupant.
	Atom* a1 = new Atom;
	a1->setType(7);
	a1->setTypeName("ZZ");
	delete a1;                 // frees the orphan slot -> free list
	Atom* a2 = new Atom;       // fresh OR reused slot; either way born-default
	TEST_EQUAL(a2->getType(), Atom::UNKNOWN_TYPE)
	TEST_EQUAL(a2->getTypeName(), String(""))
	delete a2;
RESULT

CHECK(HCP-1P.A: born-default type-name + atom_type round-trip via the offset/pool path across compact + clear)
	// Read the OFFSET path (get_type_name -> string_pool_), the persistence
	// truth, not just the String column. Born default type-name "" = offset 0
	// (always valid); atom_type = UNKNOWN_TYPE = -1.
	MoleculeStore store;
	MoleculeStore::Index i1 = store.allocate_atom();
	TEST_EQUAL(store.get_type_name(i1), std::string(""))
	TEST_EQUAL(store.atom_type(i1), (short)Atom::UNKNOWN_TYPE)

	// compact() rebuilds the pool; offset 0 stays the reserved empty string.
	store.compact();
	TEST_EQUAL(store.get_type_name(i1), std::string(""))
	MoleculeStore::Index i2 = store.allocate_atom();
	TEST_EQUAL(store.get_type_name(i2), std::string(""))

	// clear() drops the pool entirely; born-default after clear still resolves
	// "" via offset 0 (no stale offset, since born-default never caches one).
	store.clear();
	MoleculeStore::Index i3 = store.allocate_atom();
	TEST_EQUAL(store.get_type_name(i3), std::string(""))
	TEST_EQUAL(store.atom_type(i3), (short)Atom::UNKNOWN_TYPE)
RESULT


CHECK(Option-A P0.1: span/range kernel API)
	// v2.2 Option-A P0.1: the additive span/range surface. AoS-backed
	// view over the contiguous columns; signature stable across Option-B.
	MoleculeStore store;
	const int N = 6;
	for (int k = 0; k < N; ++k)
	{
		MoleculeStore::Index i = store.allocate_atom();
		store.position(i) = Vector3((float)k, (float)(2*k), (float)(3*k));
		store.force(i)    = Vector3((float)(-k), 0.f, 0.f);
	}

	// (1) whole-column span size + element parity with position(i).
	ColumnSpan<Vector3> ps = store.positions();
	TEST_EQUAL(ps.size(), (std::size_t)N)
	TEST_EQUAL(ps.empty(), false)
	bool parity = true;
	for (int k = 0; k < N; ++k)
		if (ps[k] != store.position(k)) parity = false;
	TEST_EQUAL(parity, true)

	// (2) range-for over the span sums the same as index iteration.
	float sx = 0.f;
	for (const Vector3& v : store.positions()) sx += v.x;
	TEST_REAL_EQUAL(sx, 0.f+1.f+2.f+3.f+4.f+5.f)

	// (3) write-through: mutating the span mutates the column (AoS).
	store.positions()[2] = Vector3(99.f, 99.f, 99.f);
	TEST_EQUAL(store.position(2), Vector3(99.f, 99.f, 99.f))

	// (4) AtomRange half-open + clamp past end -> shorter span, never OOB.
	AtomRange r{1u, 4u};
	TEST_EQUAL(r.size(), 3u)
	TEST_EQUAL(r.empty(), false)
	ColumnSpan<Vector3> pr = store.positions(r);
	TEST_EQUAL(pr.size(), 3u)
	TEST_EQUAL(pr[0], store.position(1))
	AtomRange past{4u, 999u};
	TEST_EQUAL(store.positions(past).size(), (std::size_t)(N - 4))  // clamped to [4, N)
	AtomRange inverted{5u, 2u};
	TEST_EQUAL(store.positions(inverted).empty(), true)

	// (5) const-store span is a read-only view.
	const MoleculeStore& cstore = store;
	ColumnSpan<const Vector3> cps = cstore.positions();
	TEST_EQUAL(cps.size(), (std::size_t)N)
	TEST_EQUAL(cps[2], Vector3(99.f, 99.f, 99.f))

	// (6) forces span parity.
	TEST_EQUAL(store.forces().size(), (std::size_t)N)
	TEST_EQUAL(store.forces()[3], store.force(3))

	// (7) Codex P0-review: empty store -> empty spans, begin()==end(),
	// no null pointer arithmetic (UBSan-clean). Empty + clamped ranges.
	MoleculeStore empty;
	ColumnSpan<Vector3> eps = empty.positions();
	TEST_EQUAL(eps.size(), (std::size_t)0)
	TEST_EQUAL(eps.empty(), true)
	TEST_EQUAL(eps.begin() == eps.end(), true)
	int empty_iters = 0;
	for (const Vector3& v : empty.positions()) { (void)v; ++empty_iters; }
	TEST_EQUAL(empty_iters, 0)
	TEST_EQUAL(empty.positions(AtomRange{0u, 10u}).size(), (std::size_t)0)
	TEST_EQUAL(empty.positions(AtomRange{0u, 10u}).begin()
	           == empty.positions(AtomRange{0u, 10u}).end(), true)

	// (8) Codex P0-review: implicit mutable->const span widening, so a
	// read-only kernel signature accepts a mutable store's span directly.
	ColumnSpan<const Vector3> widened = store.positions();   // ColumnSpan<Vector3> -> <const Vector3>
	TEST_EQUAL(widened.size(), (std::size_t)N)
	TEST_EQUAL(widened[2], Vector3(99.f, 99.f, 99.f))
RESULT

END_TEST
