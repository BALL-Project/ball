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
#include "BenchStats.h"        // v2.1 P5.3 median-of-N reporting
#include <algorithm>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
///////////////////////////

using namespace BALL;
using clk = std::chrono::steady_clock;

namespace { constexpr int kBenchRuns = 5; }   // v2.1 P5.3 median-of-N

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

	// v2.1 P5.3 (V21-MEDIAN-OF-N-BENCH): time kBenchRuns saves + loads,
	// report median + p99 + MAD + CoV, and gate on the MEDIAN sample
	// (reproducible vs single-run). Existing fixed thresholds kept
	// (D44 — no pinned-baseline CI comparator in v2.1).
	std::string serialized;
	std::vector<double> save_samples, load_samples;
	std::size_t out_len = 0;
	for (int run = 0; run < kBenchRuns; ++run)
	{
		std::ostringstream os;
		auto t0 = clk::now();
		saveSystemJSON(sys, os);
		auto t1 = clk::now();
		save_samples.push_back(std::chrono::duration<double, std::milli>(t1 - t0).count());
		serialized = os.str();
		out_len = serialized.size();
	}
	for (int run = 0; run < kBenchRuns; ++run)
	{
		System dst;
		std::istringstream is(serialized);
		auto t2 = clk::now();
		loadSystemJSON(dst, is);
		auto t3 = clk::now();
		load_samples.push_back(std::chrono::duration<double, std::milli>(t3 - t2).count());
		TEST_EQUAL(dst.countAtoms(),     N)
		TEST_EQUAL(dst.countMolecules(), n_mols)
	}

	const BALLTest::BenchStats save_st = BALLTest::computeBenchStats(save_samples);
	const BALLTest::BenchStats load_st = BALLTest::computeBenchStats(load_samples);
	const double bytes_per_atom = double(out_len) / double(N);
	std::cerr << "  [K0.7.4] save(100k, no props): " << (out_len / 1024)
		<< " KB, " << bytes_per_atom << " B/atom JSON" << std::endl;
	BALLTest::reportBenchStats("save(100k, no props)", save_st);
	BALLTest::reportBenchStats("load(100k, no props)", load_st);

	// K0.8 -> v2.1 P4.1: O(n^2) CSR rebuild fixed (load 16x faster,
	// ~220ms median local). Gates kept at 1500/2000ms: ample headroom
	// over local+CI variance, still catch the O(n^2) regression
	// (~3500ms+). Gate on the MEDIAN (P5.3).
	TEST_EQUAL(save_st.median < 1500.0, true)
	TEST_EQUAL(load_st.median < 2000.0, true)
RESULT

CHECK(K0.7.4 saveSystemJSON profile at 100k atoms WITH per-atom properties)
	const std::size_t N = 100000;
	const std::size_t n_mols = 100;
	System sys;
	build_(sys, N, n_mols, /*with_properties=*/true);

	// v2.1 P5.3: median-of-N (see no-props CHECK above).
	std::string serialized;
	std::vector<double> save_samples, load_samples;
	std::size_t out_len = 0;
	for (int run = 0; run < kBenchRuns; ++run)
	{
		std::ostringstream os;
		auto t0 = clk::now();
		saveSystemJSON(sys, os);
		auto t1 = clk::now();
		save_samples.push_back(std::chrono::duration<double, std::milli>(t1 - t0).count());
		serialized = os.str();
		out_len = serialized.size();
	}
	for (int run = 0; run < kBenchRuns; ++run)
	{
		System dst;
		std::istringstream is(serialized);
		auto t2 = clk::now();
		loadSystemJSON(dst, is);
		auto t3 = clk::now();
		load_samples.push_back(std::chrono::duration<double, std::milli>(t3 - t2).count());
		TEST_EQUAL(dst.countAtoms(), N)
	}

	const BALLTest::BenchStats save_st = BALLTest::computeBenchStats(save_samples);
	const BALLTest::BenchStats load_st = BALLTest::computeBenchStats(load_samples);
	const double bytes_per_atom = double(out_len) / double(N);
	std::cerr << "  [K0.7.4] save(100k, +3 props/atom): " << (out_len / 1024)
		<< " KB, " << bytes_per_atom << " B/atom JSON" << std::endl;
	BALLTest::reportBenchStats("save(100k, +3 props/atom)", save_st);
	BALLTest::reportBenchStats("load(100k, +3 props/atom)", load_st);

	// v2.1 P4.1: post O(n^2)-fix load (with props) ~402ms median local.
	// Gates kept at 2000/2500ms; gate on the MEDIAN (P5.3).
	TEST_EQUAL(save_st.median < 2000.0, true)
	TEST_EQUAL(load_st.median < 2500.0, true)
RESULT

END_TEST
