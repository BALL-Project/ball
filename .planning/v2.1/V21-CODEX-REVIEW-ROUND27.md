# Track A v2.1 Cluster R27 — Codex CLI Round 27 (2026-05-20)

**Status:** Complete
**Verdict:** GO for P6
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.1 P5 close review (median-of-N bench reporting)

## Probe answers

**P27-1. Stats math correctness.** GO. `computeBenchStats` is correct for the intended descriptive benchmark summaries:

- Empty input returns the zero-initialized `BenchStats` struct before indexing.
- Samples are sorted once; `n`, `min`, and `max` are set from the sorted vector.
- Median is correct for odd and even `n`; even `n` averages the two middle samples.
- p99 uses nearest-rank `ceil(0.99 * n) - 1`, with a defensive clamp. For `n=5`, `ceil(4.95)-1 = 4`, so p99 is max. For `n=4`, `ceil(3.96)-1 = 3`, so p99 is max. For `n=1`, `ceil(0.99)-1 = 0`, so no underflow. The empty-input guard handles `n=0` before the p99 path.
- MAD is computed as the median of `fabs(x - median)`, using the same odd/even median rule over sorted deviations.
- CoV uses population stddev divided by mean: mean over `n`, variance over `n`, `sqrt(var) / mean`. The `mean != 0.0` guard avoids divide-by-zero and returns `0.0` for zero-mean samples.

One non-blocking note: if callers ever pass negative samples, CoV could be negative because it divides by mean rather than `abs(mean)`. Timing/speedup samples are non-negative, so this is irrelevant for P5.3.

**P27-2. Gate semantics change.** GO. `JsonBench_test.C` now runs five saves and five loads per CHECK and gates on `save_st.median` / `load_st.median`, with the same fixed thresholds retained. This is less flaky than a single-run gate because one scheduler or cold-cache outlier no longer fails the bench. It does not weaken detection for the known O(n^2) JSON load regression: that regression slows every run, so the median still crosses the existing 2000/2500 ms gates. A partial/intermittent slowdown affecting only one or two of five samples could be hidden by the median, but that is exactly the noise-rejection tradeoff of median gating and is acceptable for these fixed-threshold smoke gates.

**P27-3. No v2.2-shaped scaffolding leaked.** GO. P5.3 is test-harness reporting only. The new helper lives under `test/`, is included with `"BenchStats.h"`, and the only code changes are `JsonBench_test.C` and `SelectorBench_test.C`. There is no pinned baseline file, no CI comparator, no public iteration API, and no generation-guard implementation in this commit. This matches D44's trimmed P5 scope.

**P27-4. BenchStats.h placement + reuse.** GO. `BenchStats.h` is header-only, quoted-included from two test translation units, and both functions are `inline`, so there is no ODR issue. In practice these are separate test executables anyway. The header is self-contained for its implementation: `<algorithm>`, `<cmath>`, `<cstddef>`, `<cstdio>`, and `<vector>` are all present.

**P27-5. kBenchRuns=5 cost.** GO. The added cost is acceptable. I ran the targeted benches locally:

```text
ctest -R 'JsonBench|SelectorBench' --output-on-failure
JsonBench_test      Passed    7.56 sec
SelectorBench_test  Passed    0.21 sec
Total Test time     7.77 sec
```

That is well inside normal CI tolerance and does not create an obvious timeout risk.

**P27-6. Overall.** GO. P5 is a clean close.

## Overall verdict

GO for P6. The stats math is correct, the median gates preserve the intended fixed-threshold regression signal while reducing flake, and the implementation stays inside the D44 P5.3-only scope.
