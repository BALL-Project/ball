// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// v2.2 H3d.D phase 0 baseline benchmark for AssignBondOrderProcessor.
//
// Per V22-H3d-DESIGN.md D-H3d.3-R2 h3d.D phase 0: capture pre-migration
// timing baseline for the AStar (default) and ILP bond-order assignment
// strategies on representative MOL2 fixtures. ALL h3d.D migration commits
// gate on <=5% regression vs the numbers this benchmark prints.
//
// Inputs come from test/data/*.mol2 (the assignBondOrderProcessor test
// fixtures), so the benchmark exercises real-world bond-order resolution
// rather than a synthetic toy.
//
// To run after a build:
//   cd build && ninja AssignBondOrder_bench
//   BALL_DATA_PATH=$(realpath ../data) \
//     DYLD_LIBRARY_PATH=$(realpath ./lib) \
//     ./bin/BENCHMARKS/AssignBondOrder_bench
//
// Record the section timings into V22-DECISIONS.md as the baseline; the
// h3d.D close-review re-runs after each migration commit and computes
// % delta. If any section grows by >5% the commit MUST be reworked or
// surface a DR for waiver before landing.

#include <BALL/CONCEPT/benchmark.h>

#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
#include <BALL/DATATYPE/options.h>

#include <string>
#include <vector>

using namespace BALL;

START_BENCHMARK(AssignBondOrder, 1.0, "$Id: AssignBondOrder_bench.C v2.2-H3d.D-phase0 $")

// Representative MOL2 fixtures from test/data. These are the same
// inputs the AssignBondOrderProcessor unit test covers, so any timing
// regression here will also show as a slower test. Picked to span:
//   - small organic ligand (1b5i_ligand)
//   - peptide fragment (AAG, alanyl-alanyl-glycine)
//   - aromatic heterocycle (BEWCUB)
//   - mixed bond-order resolution (AAA, AGLYSL01)
// BALL_TEST_DATA_DIR_LIT is injected by CMake to point at
// ${CMAKE_SOURCE_DIR}/test/data; resolves at build time regardless of
// where the bench is run from.
#ifndef BALL_TEST_DATA_DIR_LIT
#  error "BALL_TEST_DATA_DIR_LIT must be defined; see source/BENCHMARKS/CMakeLists.txt"
#endif

#define FX(name) BALL_TEST_DATA_DIR_LIT "/" name

const char* const FIXTURES[] = {
	FX("1b5i_ligand.mol2"),
	FX("AAG.mol2"),
	FX("AssignBondOrderProcessor_test_AAA.mol2"),
	FX("AssignBondOrderProcessor_test_AGLYSL01_out.mol2"),
	FX("AssignBondOrderProcessor_test_BEWCUB.mol2"),
};
const Size NR_FIXTURES = sizeof(FIXTURES) / sizeof(FIXTURES[0]);

// Number of passes per fixture; lifts timing out of single-call jitter.
const Size PASSES_PER_FIXTURE = 10;

START_SECTION(AStar bond-order assignment (default), 1.0)

	for (Size f = 0; f < NR_FIXTURES; ++f)
	{
		for (Size p = 0; p < PASSES_PER_FIXTURE; ++p)
		{
			System sys;
			MOL2File mol2(FIXTURES[f], std::ios::in);
			mol2 >> sys;
			mol2.close();
			if (sys.countAtoms() == 0) continue;

			AssignBondOrderProcessor abo;
			abo.options.set(AssignBondOrderProcessor::Option::ALGORITHM,
			                AssignBondOrderProcessor::Algorithm::A_STAR);

			START_TIMER
				sys.apply(abo);
			STOP_TIMER
		}
	}

END_SECTION

// ILP path is gated on BALL_HAS_LPSOLVE (config.h). When lpsolve is
// linked the section runs; otherwise it is omitted entirely (running
// it would log a per-call "no lpsolve" error and the timing would
// measure error-path cost rather than ILP cost). All h3d.D ILP-
// touching commits MUST re-run this section in a configured build
// before landing.
#include <BALL/CONFIG/config.h>
#ifdef BALL_HAS_LPSOLVE
START_SECTION(ILP bond-order assignment, 1.0)

	for (Size f = 0; f < NR_FIXTURES; ++f)
	{
		for (Size p = 0; p < PASSES_PER_FIXTURE; ++p)
		{
			System sys;
			MOL2File mol2(FIXTURES[f], std::ios::in);
			mol2 >> sys;
			mol2.close();
			if (sys.countAtoms() == 0) continue;

			AssignBondOrderProcessor abo;
			abo.options.set(AssignBondOrderProcessor::Option::ALGORITHM,
			                AssignBondOrderProcessor::Algorithm::ILP);

			START_TIMER
				sys.apply(abo);
			STOP_TIMER
		}
	}

END_SECTION
#endif // BALL_HAS_LPSOLVE

START_SECTION(FPT bond-order assignment, 1.0)

	for (Size f = 0; f < NR_FIXTURES; ++f)
	{
		for (Size p = 0; p < PASSES_PER_FIXTURE; ++p)
		{
			System sys;
			MOL2File mol2(FIXTURES[f], std::ios::in);
			mol2 >> sys;
			mol2.close();
			if (sys.countAtoms() == 0) continue;

			AssignBondOrderProcessor abo;
			abo.options.set(AssignBondOrderProcessor::Option::ALGORITHM,
			                AssignBondOrderProcessor::Algorithm::FPT);

			START_TIMER
				sys.apply(abo);
			STOP_TIMER
		}
	}

END_SECTION

END_BENCHMARK
