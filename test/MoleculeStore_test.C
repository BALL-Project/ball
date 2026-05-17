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

END_TEST
