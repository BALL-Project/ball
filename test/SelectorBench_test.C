// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 KERNEL — K0.5.6 compiled-selection benchmark.
// Spec §7.3 + V2.0-ROADMAP K0.5 gate row.
//
// Builds a synthetic 50k-atom System with representative element/name
// distribution, then times the v1.x ExpressionTree per-atom path
// against the new CompiledExpression bitmap path. Asserts a
// conservative >=5x speedup gate (CI-noise safe); the full >=10x
// K0.5 gate is verified in K0.7's broader perf phase.
//
// Reports the actual speedup so failures still surface in the test
// log.
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/expression.h>
#include <BALL/KERNEL/expressionTree.h>
#include <BALL/KERNEL/compiledExpression.h>
#include <BALL/KERNEL/moleculeStore.h>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>
#include <string>
///////////////////////////

using namespace BALL;
using clk = std::chrono::steady_clock;

namespace
{
	// Build N atoms into `sys`. Pre-reserve the store so allocate_atom
	// never reallocates (otherwise the per-atom adopt loop dominates).
	void build_corpus_(System& sys, std::size_t n)
	{
		sys.getStore().reserve(n + 64);
		// Distribution rough-matches typical protein-with-solvent atoms:
		// ~50% carbon, ~25% hydrogen, ~15% nitrogen, ~10% oxygen.
		// Atom names rotate through CA / CB / N / C / O.
		const Element* els[4] = {
			&PTE[Element::CARBON], &PTE[Element::HYDROGEN],
			&PTE[Element::NITROGEN], &PTE[Element::OXYGEN]
		};
		const int weights[4] = {50, 75, 90, 100};  // cumulative
		const char* names[5] = {"CA", "CB", "N", "C", "O"};
		for (std::size_t i = 0; i < n; ++i)
		{
			Atom* a = new Atom;
			int r = static_cast<int>(i * 2654435761u % 100);
			int e = 0;
			while (e < 3 && r >= weights[e]) ++e;
			a->setElement(*els[e]);
			a->setName(names[i % 5]);
			a->setCharge(((i % 7) - 3) * 0.1f);
			sys.adopt(*a);
		}
	}

	struct Pair { double v1_ms; double v2_ms; double ratio; };

	// Time both paths and return the speedup ratio (v1 / v2).
	// Each path runs `iters` times to keep variance low.
	Pair time_pair_(System& sys, const char* source, int iters)
	{
		auto& store = sys.getStore();
		const String src_str(source);
		Expression expr(src_str);
		const ExpressionTree* tree = expr.getExpressionTree();
		auto compiled = expr.getCompiled(store);
		std::vector<std::uint8_t> bm;

		// Warm both paths once so first-call costs (cache fill, etc.)
		// don't dominate.
		std::size_t warm_hits = 0;
		for (std::size_t i = 0; i < store.size(); ++i)
		{
			if (store.is_freed(i)) continue;
			Atom* a = store.back_ptr(static_cast<MoleculeStore::Index>(i));
			if (a && (*tree)(*a)) ++warm_hits;
		}
		compiled->evaluate(store, bm);
		(void)warm_hits;

		// Time v1.x path.
		auto t0 = clk::now();
		std::size_t v1_total = 0;
		for (int k = 0; k < iters; ++k)
		{
			std::size_t hits = 0;
			for (std::size_t i = 0; i < store.size(); ++i)
			{
				if (store.is_freed(i)) continue;
				Atom* a = store.back_ptr(static_cast<MoleculeStore::Index>(i));
				if (a && (*tree)(*a)) ++hits;
			}
			v1_total += hits;
		}
		auto t1 = clk::now();

		// Time new bitmap path.
		auto t2 = clk::now();
		std::size_t v2_total = 0;
		for (int k = 0; k < iters; ++k)
		{
			compiled->evaluate(store, bm);
			std::size_t hits = 0;
			for (std::uint8_t v : bm) hits += v;
			v2_total += hits;
		}
		auto t3 = clk::now();

		// Cross-check correctness — both paths must produce the same
		// selection count.
		TEST_EQUAL(v1_total, v2_total)

		const double v1_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
		const double v2_ms = std::chrono::duration<double, std::milli>(t3 - t2).count();
		const double ratio = (v2_ms > 0.0) ? (v1_ms / v2_ms) : 0.0;

		std::cerr << "  [K0.5.6] " << source
			<< "  v1=" << v1_ms << "ms  v2=" << v2_ms << "ms"
			<< "  speedup=" << ratio << "x"
			<< "  (selected=" << (v1_total / iters) << ")"
			<< std::endl;

		return {v1_ms, v2_ms, ratio};
	}
} // namespace

START_TEST(SelectorBench)

CHECK(K0.5.6 fast-path selection >=5x faster than v1.x ExpressionTree)
	const std::size_t N = 50000;
	const int iters = 5;
	System sys;
	build_corpus_(sys, N);
	TEST_EQUAL(sys.getStore().live_atom_count(), N)

	// Representative queries. Inner-loop leaf cost dominates at this size.
	const char* queries[] = {
		"element(C)",
		"name(CA)",
		"charge(<0)",
		"element(C) AND name(CA)",
	};

	std::vector<double> ratios;
	for (const char* q : queries)
	{
		Pair p = time_pair_(sys, q, iters);
		ratios.push_back(p.ratio);
	}

	// Gate: median ratio >= 5x. Conservative vs the K0.5 design gate
	// (10x) to absorb CI noise; the full 10x verification lives in K0.7
	// with longer iter counts + a 100k corpus.
	std::vector<double> sorted = ratios;
	std::sort(sorted.begin(), sorted.end());
	const double median = sorted[sorted.size() / 2];
	std::cerr << "  [K0.5.6] median speedup across "
		<< sorted.size() << " queries = " << median << "x" << std::endl;
	TEST_EQUAL(median >= 5.0, true)
RESULT

CHECK(K0.5.6 OwnedPred slow path stays within 2x of v1.x)
	// The OwnedPred path goes through back_ptr + virtual dispatch per
	// atom — strictly slower than the column-scan fast leaves. We just
	// want to confirm it doesn't regress massively against the v1.x
	// path (it's calling the SAME ExpressionPredicate underneath, plus
	// one back_ptr dereference per atom).
	const std::size_t N = 10000;  // smaller corpus; ring detection is O(N)
	const int iters = 3;
	System sys;
	build_corpus_(sys, N);

	auto& store = sys.getStore();
	Expression expr("inRing()");
	const ExpressionTree* tree = expr.getExpressionTree();
	auto compiled = expr.getCompiled(store);
	std::vector<std::uint8_t> bm;

	auto t0 = clk::now();
	std::size_t v1 = 0;
	for (int k = 0; k < iters; ++k)
		for (std::size_t i = 0; i < store.size(); ++i)
		{
			if (store.is_freed(i)) continue;
			Atom* a = store.back_ptr(static_cast<MoleculeStore::Index>(i));
			if (a && (*tree)(*a)) ++v1;
		}
	auto t1 = clk::now();

	auto t2 = clk::now();
	std::size_t v2 = 0;
	for (int k = 0; k < iters; ++k)
	{
		compiled->evaluate(store, bm);
		for (std::uint8_t v : bm) v2 += v;
	}
	auto t3 = clk::now();

	TEST_EQUAL(v1, v2)
	const double v1_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
	const double v2_ms = std::chrono::duration<double, std::milli>(t3 - t2).count();
	std::cerr << "  [K0.5.6] inRing() OwnedPred path: v1=" << v1_ms
		<< "ms  v2=" << v2_ms << "ms  ratio=" << (v1_ms / v2_ms) << "x" << std::endl;
	// Soft gate: OwnedPred must be at least 0.5x (i.e. no worse than 2x
	// slower than v1.x). Typically 0.8-1.2x in practice.
	TEST_EQUAL((v1_ms / v2_ms) >= 0.5, true)
RESULT

END_TEST
