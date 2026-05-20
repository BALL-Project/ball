// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// v2.1 P5.3 (V21-MEDIAN-OF-N-BENCH): tiny header-only helper to turn
// a vector of per-run timing samples into reproducible summary
// statistics (median + p99 + min/max + MAD + coefficient of
// variation). Used by the benchmark tests (JsonBench_test,
// SelectorBench_test) so release-claim numbers are median-of-N
// rather than single-run.
//
// Per D44 (R26 trim): this is the ENTIRETY of v2.1 P5 — a
// benchmark-reporting cleanup. It does NOT add a pinned-baseline CI
// comparator (deferred to v2.2, V21-CI-PERF-GATES); the tests keep
// their existing fixed pass/fail thresholds and simply gate on the
// median sample instead of a single run.
//
// Quoted-include from test/*.C (same directory).

#ifndef BALL_TEST_BENCHSTATS_H
#define BALL_TEST_BENCHSTATS_H

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <vector>

namespace BALLTest
{
	struct BenchStats
	{
		std::size_t n      = 0;
		double      median = 0.0;
		double      p99    = 0.0;
		double      min    = 0.0;
		double      max    = 0.0;
		double      mad    = 0.0;   // median absolute deviation (robust spread)
		double      cov    = 0.0;   // coefficient of variation = stddev / mean
	};

	// Compute summary stats over `samples` (a copy is sorted internally).
	inline BenchStats computeBenchStats(std::vector<double> samples)
	{
		BenchStats s;
		if (samples.empty()) return s;
		std::sort(samples.begin(), samples.end());
		s.n   = samples.size();
		s.min = samples.front();
		s.max = samples.back();

		// median
		s.median = (s.n % 2 == 1)
			? samples[s.n / 2]
			: 0.5 * (samples[s.n / 2 - 1] + samples[s.n / 2]);

		// p99 (nearest-rank; for small N this is effectively the max)
		std::size_t p99_idx = static_cast<std::size_t>(std::ceil(0.99 * s.n));
		if (p99_idx >= 1) p99_idx -= 1;
		if (p99_idx >= s.n) p99_idx = s.n - 1;
		s.p99 = samples[p99_idx];

		// MAD: median of |x - median|
		std::vector<double> dev;
		dev.reserve(s.n);
		for (double x : samples) dev.push_back(std::fabs(x - s.median));
		std::sort(dev.begin(), dev.end());
		s.mad = (s.n % 2 == 1)
			? dev[s.n / 2]
			: 0.5 * (dev[s.n / 2 - 1] + dev[s.n / 2]);

		// CoV: stddev / mean (population stddev — descriptive, not inferential)
		double mean = 0.0;
		for (double x : samples) mean += x;
		mean /= static_cast<double>(s.n);
		double var = 0.0;
		for (double x : samples) { const double d = x - mean; var += d * d; }
		var /= static_cast<double>(s.n);
		s.cov = (mean != 0.0) ? (std::sqrt(var) / mean) : 0.0;

		return s;
	}

	// Print one summary line to stderr.
	inline void reportBenchStats(const char* label, const BenchStats& s,
	                             const char* unit = "ms")
	{
		std::fprintf(stderr,
			"  [bench] %-38s n=%zu median=%.2f%s p99=%.2f min=%.2f max=%.2f "
			"MAD=%.2f CoV=%.1f%%\n",
			label, s.n, s.median, unit, s.p99, s.min, s.max, s.mad,
			s.cov * 100.0);
	}
}

#endif // BALL_TEST_BENCHSTATS_H
