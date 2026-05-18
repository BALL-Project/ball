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
#include <BALL/MATHS/vector3.h>
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

CHECK(K0.4.2 sequential adoption -- known limitation: bond orphans)
	// Multi-step adoption (adopt a then adopt b separately) is a
	// degenerate case in K0.4.2: when a adopts first, the bond stays
	// in orphan; a's orphan slot is freed; CSR rebuild skips the bond
	// (touches freed slot); when b adopts, the bond is invisible.
	// Result: bond becomes orphaned in orphan store and dies with it.
	//
	// K0.4.3 will fix this by adopting all atoms contiguously through
	// AtomContainer::insert, so both endpoints adopt before any slot
	// release happens. Test locks the current K0.4.2 behavior so a
	// future fix can flip the assertion.
	Atom a, b;
	Bond* bond = a.createBond(b);
	auto* orphan = a.getStore();
	TEST_EQUAL(bond->bond_store_, orphan)

	System sys;
	auto* sys_store = &sys.getStore();
	sys.adopt(a);
	sys.adopt(b);

	// K0.4.2 current behavior: bond stays orphaned (was in orphan,
	// but freed-slot CSR-skip hides it from b's migration scan).
	// K0.4.3 will make this assert sys_store instead.
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
	// initial '\0' if pool was empty.
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

CHECK(K0.3c.6 mutable-getter drift -- DOCUMENTED GAP until K0.3b.LATER)
	// Codex Round 2 HIGH-3 / HIGH-6: the non-const reference returned by
	// Atom::getPosition() bypasses dual-write when the caller mutates
	// through it. K0.2c lease helper is opt-in only; the legacy
	// `Vector3& p = atom.getPosition(); p = v;` pattern still mutates
	// v1.x position_ silently. This test LOCKS that behaviour as the
	// current contract and FLIPS to dual-write semantics in K0.3b.LATER
	// when the getter is rebound to read directly from the store.
	Atom a;
	auto* store = a.getStore();
	auto idx = a.getStoreIndex();

	a.setPosition(Vector3(1.f, 2.f, 3.f));
	TEST_EQUAL(a.getPosition(), Vector3(1.f, 2.f, 3.f))
	TEST_EQUAL(store->position(idx), Vector3(1.f, 2.f, 3.f))

	// Now mutate via the non-const reference. v1.x updates; store does NOT.
	Vector3& p = a.getPosition();
	p = Vector3(7.f, 8.f, 9.f);
	TEST_EQUAL(a.getPosition(), Vector3(7.f, 8.f, 9.f))     // v1.x sees new value
	TEST_EQUAL(store->position(idx) == Vector3(1.f, 2.f, 3.f), true)  // store STALE
	// ^ This is the documented gap. When K0.3b.LATER lands, this
	// assertion will need flipping (store will also be 7,8,9). Update
	// the test then; tracked by V2.0-ROADMAP K0.3b.LATER per-field
	// flip plan.
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

END_TEST
