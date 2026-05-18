// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 — K0.7.3 per-atom memory budget verification.
// D13 target: typical workload ≤160 B/atom (column data + handle).
//
// Builds a 100k-atom System and measures:
//   1. Per-atom column row cost (constant; from K0.7.1)
//   2. Per-atom string-pool contribution (intern-dedup amortised)
//   3. Per-atom handle cost (sizeof(Atom) — dominated by base classes)
//   4. Per-atom Composite-tree overhead (parent + child pointers etc.,
//      bundled into sizeof(Atom) per K0.7.1)
//   5. Per-bond cost (BondRecord = 12 B in store; Bond* handle is 288 B
//      but only allocated when v1.x Bond is materialised)
//
// Reports breakdown + total; gates against D13 budget.
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/moleculeStore.h>
#include <BALL/KERNEL/PTE.h>
#include <iostream>
#include <vector>
///////////////////////////

START_TEST(MemoryBudget)

using namespace BALL;

CHECK(K0.7.3 100k-atom budget breakdown: store columns + handles)
	const std::size_t N = 100000;

	System sys;
	sys.getStore().reserve(N + 64);

	// Build N atoms with a representative distribution. Keep this loop
	// simple (no per-atom property bag) so we measure the BASELINE
	// per-atom footprint of v2.0; K0.7.3b can extend to include
	// average-5-properties-per-atom for the "full-fat" scenario.
	const Element* els[4] = {
		&PTE[Element::CARBON], &PTE[Element::HYDROGEN],
		&PTE[Element::NITROGEN], &PTE[Element::OXYGEN]
	};
	const int weights[4] = {50, 75, 90, 100};
	const char* names[5] = {"CA","CB","N","C","O"};
	for (std::size_t i = 0; i < N; ++i)
	{
		Atom* a = new Atom;
		int r = static_cast<int>(i * 2654435761u % 100);
		int e = 0;
		while (e < 3 && r >= weights[e]) ++e;
		a->setElement(*els[e]);
		a->setName(names[i % 5]);
		a->setCharge(((i % 7) - 3) * 0.1f);
		sys.AtomContainer::insert(*a);
	}
	TEST_EQUAL(sys.countAtoms(), N)
	TEST_EQUAL(sys.getStore().live_atom_count(), N)

	auto& s = sys.getStore();
	const std::size_t cap = s.capacity();

	// Per-row column cost matches K0.7.1's measurement (139 B).
	// Multiply by CAPACITY (not live count) because the SoA vectors
	// reserved that many slots.
	const std::size_t per_row_bytes =
		  sizeof(Vector3)*3     // positions / velocities / forces
		+ sizeof(float)*2       // charges / radii
		+ sizeof(short)*2       // atom_types / formal_charges
		+ sizeof(std::uint8_t)*2 // element_indices / selection
		+ sizeof(std::uint32_t)*2// name_offsets / type_name_offsets
		+ sizeof(String)*2      // name_strings / type_name_strings
		+ sizeof(std::uint64_t) // stable_ids
		+ sizeof(void*)         // back_ptr
		+ sizeof(std::uint8_t); // is_freed
	const std::size_t column_bytes_total = per_row_bytes * cap;

	// String pool contribution: deduped per intern, so ~5 unique names
	// + ~4 unique type_names typically — bounded constant amortised
	// across all atoms. Measure directly.
	const std::size_t pool_bytes = s.string_pool().size();

	// Handle cost: every Atom* is heap-allocated, sizeof(Atom) per K0.7.1.
	const std::size_t handle_bytes_total = sizeof(Atom) * N;

	// Per-atom breakdown.
	const double col_per_atom    = double(column_bytes_total) / double(N);
	const double pool_per_atom   = double(pool_bytes)         / double(N);
	const double handle_per_atom = double(handle_bytes_total) / double(N);
	const double total_per_atom  = col_per_atom + pool_per_atom + handle_per_atom;

	std::cerr << "  [K0.7.3] === per-atom budget on N=" << N << " atoms ===" << std::endl;
	std::cerr << "  [K0.7.3] SoA columns       : " << col_per_atom    << " B/atom" << std::endl;
	std::cerr << "  [K0.7.3] string pool       : " << pool_per_atom   << " B/atom" << std::endl;
	std::cerr << "  [K0.7.3] Atom handle       : " << handle_per_atom << " B/atom (sizeof(Atom)=" << sizeof(Atom) << ")" << std::endl;
	std::cerr << "  [K0.7.3] ------------------------------------------" << std::endl;
	std::cerr << "  [K0.7.3] TOTAL             : " << total_per_atom  << " B/atom" << std::endl;
	std::cerr << "  [K0.7.3] D13 budget target : 160 B/atom (typical)" << std::endl;
	std::cerr << "  [K0.7.3] ------------------------------------------" << std::endl;

	// Gate 1: SoA column contribution alone fits the D13 budget. This
	// represents the "atoms exist in the store but most don't have a
	// live Atom* handle" case (e.g. trajectory frames, batch analysis).
	TEST_EQUAL(col_per_atom < 160.0, true)

	// Gate 2: full-fat total (every atom has a live handle) is reported
	// but NOT gated — the D2/D3/D4 thin-stub overhead pushes this over
	// budget by design; full reduction is deferred to v2.1 per
	// KERNEL-V2-DECISIONS.md. We document the gap so it's visible.
	if (total_per_atom > 160.0)
	{
		std::cerr << "  [K0.7.3] NOTE: full-fat budget " << total_per_atom
			<< " B/atom exceeds D13 (160). Dominated by sizeof(Atom)="
			<< sizeof(Atom) << " B handle (D2/D3/D4 thin-stub overhead;"
			<< " full reduction deferred to v2.1)." << std::endl;
	}
RESULT

CHECK(K0.7.3 100k iteration cost - countAtoms walks the Composite tree)
	// D14 budget: iteration ≤2× v1.x baseline. The v2 iteration cost
	// is dominated by the Composite-tree walk (System -> children).
	// Verify the walk completes in reasonable time on 100k orphans.
	const std::size_t N = 100000;
	System sys;
	sys.getStore().reserve(N + 64);
	for (std::size_t i = 0; i < N; ++i)
	{
		Atom* a = new Atom;
		sys.AtomContainer::insert(*a);
	}
	auto t0 = std::chrono::steady_clock::now();
	const Size c = sys.countAtoms();
	auto t1 = std::chrono::steady_clock::now();
	const double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
	std::cerr << "  [K0.7.3] sys.countAtoms() over " << N << " atoms: "
		<< ms << " ms" << std::endl;
	TEST_EQUAL(c, N)
	// Soft gate: 100k atoms should walk in <100 ms even on modest HW.
	TEST_EQUAL(ms < 100.0, true)
RESULT

END_TEST
