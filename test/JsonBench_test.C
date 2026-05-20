// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 — K0.7.4 JSON writer/reader profiling at 100k atoms.
// Closes the Codex R7 FYI-7 + R8 FYI-7 perf deferrals: measure
// saveSystemJSON + loadSystemJSON cost on a representative System;
// report ms/op and bytes/atom; soft-gate against obvious regression.
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/systemJson.h>
#include <BALL/KERNEL/PTE.h>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
///////////////////////////

using namespace BALL;
using clk = std::chrono::steady_clock;

namespace
{
	// Build N atoms across `n_mols` molecules.
	void build_(System& sys, std::size_t N, std::size_t n_mols, bool with_properties)
	{
		sys.getStore().reserve(N + 64);
		const Element* els[4] = {
			&PTE[Element::CARBON], &PTE[Element::HYDROGEN],
			&PTE[Element::NITROGEN], &PTE[Element::OXYGEN]
		};
		const char* names[5] = {"CA","CB","N","C","O"};
		const std::size_t per_mol = (N + n_mols - 1) / n_mols;

		for (std::size_t mi = 0; mi < n_mols; ++mi)
		{
			Molecule* m = new Molecule;
			m->setName(String("MOL_") + String(mi));
			sys.insert(*m);
			const std::size_t start = mi * per_mol;
			const std::size_t end   = std::min(start + per_mol, N);
			for (std::size_t i = start; i < end; ++i)
			{
				Atom* a = new Atom;
				a->setElement(*els[i % 4]);
				a->setName(names[i % 5]);
				a->setCharge(((i % 7) - 3) * 0.1f);
				if (with_properties)
				{
					// Three typed properties per atom — representative
					// of an MD/structural-analysis workload.
					a->setProperty(std::string("partial_charge"),
					               static_cast<float>((i % 11) - 5) * 0.1f);
					a->setProperty(std::string("residue_seq"),
					               static_cast<int>(i % 1000));
					a->setProperty(std::string("is_aromatic"),
					               static_cast<bool>(i % 7 == 0));
				}
				m->insert(*a);
			}
		}
	}
}

START_TEST(JsonBench)

CHECK(K0.7.4 saveSystemJSON profile at 100k atoms WITHOUT per-atom properties)
	const std::size_t N = 100000;
	const std::size_t n_mols = 100;
	System sys;
	build_(sys, N, n_mols, /*with_properties=*/false);
	TEST_EQUAL(sys.countAtoms(),     N)
	TEST_EQUAL(sys.countMolecules(), n_mols)

	std::ostringstream os;
	auto t0 = clk::now();
	saveSystemJSON(sys, os);
	auto t1 = clk::now();
	const double save_ms      = std::chrono::duration<double, std::milli>(t1 - t0).count();
	const std::size_t out_len = os.str().size();
	const double bytes_per_atom = double(out_len) / double(N);

	std::cerr << "  [K0.7.4] save(100k, no props)  : " << save_ms << " ms, "
		<< (out_len / 1024) << " KB, " << bytes_per_atom << " B/atom JSON" << std::endl;

	// Load round-trip + time it.
	System dst;
	std::istringstream is(os.str());
	auto t2 = clk::now();
	loadSystemJSON(dst, is);
	auto t3 = clk::now();
	const double load_ms = std::chrono::duration<double, std::milli>(t3 - t2).count();
	std::cerr << "  [K0.7.4] load(100k, no props)  : " << load_ms << " ms" << std::endl;

	TEST_EQUAL(dst.countAtoms(),     N)
	TEST_EQUAL(dst.countMolecules(), n_mols)

	// K0.8 + 2026-05-18 (Codex R9 finding 4, partial calibration):
	// gates were 5000ms / 30000ms (K0.8) -> 1500ms / 10000ms.
	// v2.1 P4.1 (V21-LOAD-BATCH, 2026-05-20): the loadSystemJSON
	// O(n^2) CSR-rebuild was fixed (insert bucket 3414ms -> 13ms).
	// Local post-fix: load(no-props) 220ms (was ~3588ms = 16x
	// faster). Load gate tightened from 10000 to 2000ms: that's
	// ~9x headroom over local + CI variance, and STILL catches the
	// O(n^2) regression (which would bring load back to ~3500ms+,
	// exceeding 2000). Save unchanged.
	TEST_EQUAL(save_ms < 1500.0, true)
	TEST_EQUAL(load_ms < 2000.0, true)
RESULT

CHECK(K0.7.4 saveSystemJSON profile at 100k atoms WITH per-atom properties)
	const std::size_t N = 100000;
	const std::size_t n_mols = 100;
	System sys;
	build_(sys, N, n_mols, /*with_properties=*/true);

	std::ostringstream os;
	auto t0 = clk::now();
	saveSystemJSON(sys, os);
	auto t1 = clk::now();
	const double save_ms      = std::chrono::duration<double, std::milli>(t1 - t0).count();
	const std::size_t out_len = os.str().size();
	const double bytes_per_atom = double(out_len) / double(N);

	std::cerr << "  [K0.7.4] save(100k, +3 props/atom): " << save_ms << " ms, "
		<< (out_len / 1024) << " KB, " << bytes_per_atom << " B/atom JSON" << std::endl;

	System dst;
	std::istringstream is(os.str());
	auto t2 = clk::now();
	loadSystemJSON(dst, is);
	auto t3 = clk::now();
	const double load_ms = std::chrono::duration<double, std::milli>(t3 - t2).count();
	std::cerr << "  [K0.7.4] load(100k, +3 props/atom): " << load_ms << " ms" << std::endl;

	TEST_EQUAL(dst.countAtoms(),     N)
	// K0.8 + 2026-05-18 recalibration: observed 322-332 ms save /
	// 3799-3830 ms load. v2.1 P4.1 (2026-05-20): post O(n^2)-fix
	// load (with props) 402ms (was ~3750ms = 9.3x faster). Load gate
	// tightened from 10000 to 2500ms: ~6x headroom + CI variance,
	// still catches the O(n^2) regression (~3800ms+). Save unchanged.
	TEST_EQUAL(save_ms < 2000.0, true)
	TEST_EQUAL(load_ms < 2500.0, true)
RESULT

END_TEST
