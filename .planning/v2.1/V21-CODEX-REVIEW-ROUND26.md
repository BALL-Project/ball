# Track A v2.1 Cluster R26 — Codex CLI Round 26 (2026-05-20)

**Status:** Complete
**Verdict:** NEEDS-FIXES
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.1 P5 planning review (perf + iter API + generation-guard)

## Probe answers

**P26-1. P5.4 generation-guard: the field exists, but the planned check is not
semantically ready.** `Atom` already carries `store_`, `store_idx_`, and
`store_generation_` in `atom.h`; this is not a hidden Atom layout expansion.
`bindToStore_()` and `migrateTo_()` populate `store_generation_` from
`MoleculeStore::generation()`. However, the inline getters in `atom.iC`
currently dereference `store_->... (store_idx_)` directly and do not check
`store_generation_` at all.

The blocker is not field absence. The blocker is meaning. `MoleculeStore`
advances `generation_` on capacity-changing allocation/reserve and on compact/
clear. Those events invalidate column references and compiled-expression
offset assumptions, but they do not necessarily invalidate an Atom handle that
stores `(store*, index)`. A pre-existing Atom remains addressable after vector
reallocation or `compact()` if its slot still exists. A naive
`store_generation_ == store_->generation()` check in every getter would
therefore false-trip after ordinary store growth unless all live Atom handles
were refreshed on each generation bump. That refresh machinery does not exist
and would be a larger mutation/back-pointer sweep design.

Conclusion: P5.4 is scoped incorrectly as "add debug asserts." It should either
be cut from v2.1 or redesigned as a different guard: slot-lifetime generation,
explicit borrowed-column generation, or a narrower `clear()/destroyed store`
trap. Do not land the current D25 wording as-is.

**P26-2. P5.1 STORE-ITER-API is risky public API in the wrong release.**
The API is public and explicitly v2.2-forward-looking, while v2.2 is where the
Atom/store relationship changes. Shipping `MoleculeStore::iterAtoms()` now
locks in source/ABI expectations before the inheritance flip and mutation
wiring settle. Yielding `Atom*` is probably source-compatible in the shallow
sense that `Atom` survives v2.2, but it commits the iterator value category and
invalidation contract before the real thin-handle shape is known.

The actual v2.1 consumer value is also weak. Current fast paths already scan
store indices and use `back_ptr(i)` where needed. A public iterator wrapper
around that is convenience, not a release-critical performance primitive.

**P26-3. "Skip Atom* materialization" is not true for v2.1.** In v2.1, atoms
are still heap `Atom*` objects and `MoleculeStore` already has a `back_ptr`
column. An iterator yielding `Atom*` cannot skip Atom materialization; the
objects already exist, and the iterator must either read `back_ptr(i)` or walk
the existing Composite tree. The materialization win only becomes real when
v2.2 can represent atoms as store rows without mandatory heap handles. P5.1 is
therefore a v2.2-shaped feature mis-scheduled into v2.1.

**P26-4. P5.2 perf-gate calibration is not implementable by the current
harness without new infrastructure.** `JsonBench_test.C`, `SelectorBench_test.C`,
and `MemoryBudget_test.C` are ClassTest programs with fixed local thresholds
and stderr reporting. They do not read a pinned baseline file, compute
`max(2 * median, median + 6 * MAD)`, classify CoV > 30% as advisory, or expose
machine-readable metrics for a GitHub Action comparator.

P5.2 therefore needs either a new benchmark runner/comparison script or a
substantial test-harness refactor. That can be done, but it is more than a
small ctest tweak. For v2.1, this is overbuilt relative to the remaining risk,
especially after P4 already tightened `JsonBench_test` load gates enough to
catch the known O(n^2) regression.

**P26-5. P5.2 and P5.3 are ordered backwards.** P5.2 depends on medians, MAD,
and repeat-run reporting. P5.3 is the work that creates that reporting. The
correct order is P5.3 first, then any baseline collection/comparison layer.

**P26-6. Trim recommendation.** Do not run full P5 as planned. The minimum
credible v2.1 P5 is:

1. Land P5.3 as a small benchmark-reporting cleanup: median, p99, MAD, min/max,
   and CoV across N runs for the existing benchmark checks.
2. Keep the current fixed ctest thresholds for v2.1 release gates, possibly
   tightening only where P4 already produced stable local/CI evidence.
3. Replace P5.4 implementation with a short design correction or defer it.
   If any code lands, it must prove the guard does not false-trip on reserve,
   reallocation, compact, or ordinary adoption.
4. Defer P5.1 STORE-ITER-API and full P5.2 baseline-file CI gates to v2.2,
   after the thin-handle and mutation model is known.

This lets v2.1 ship honestly as "JSON + API cleanup + v2.2 prep + measured perf
wins" without adding public v2.2-shaped API or a fragile CI perf system.

**P26-7. D25 generation-guard is not coherent with D39 as currently phrased.**
D39 defers mutation wiring and the thin-handle lifecycle to v2.2. The current
store generation still changes for column-storage events, not for every
semantic handle-staleness event. In v2.1, a per-deref Atom handle check against
store generation would mostly detect "a store vector reallocated since this
handle bound," which is not stale-handle misuse. Real stale-handle detection is
more relevant once v2.2 has row handles, mutation wiring, compaction/remapping
rules, and a slot-generation concept.

**P26-8. Overall.** Full P5 is not a GO. It is fixable by trimming and
resequencing.

## Recommended P5 scope

**P5.1 V21-STORE-ITER-API:** Defer to v2.2. If the maintainer wants to keep
work in v2.1, limit it to `.planning/v2.1/P5-STORE-ITER-API.md` as a non-shipped
design memo with an explicit "no public API in v2.1" conclusion.

**P5.3 V21-MEDIAN-OF-N-BENCH:** Move before P5.2 and keep it. This is the best
remaining v2.1 value: it makes release numbers reproducible without committing
to a new CI policy. It should be a focused helper/refactor used by
`JsonBench_test`, `SelectorBench_test`, and any relevant memory/iteration
timing output.

**P5.2 V21-CI-PERF-GATES:** Defer the pinned `PERF-BASELINES.json` gate to
v2.2 or v2.1.x. For v2.1, optionally create advisory-only metric output or a
local script, but do not make release depend on five clean CI baseline runs plus
a new comparator unless the harness work is already complete.

**P5.4 GenerationGuard:** Do not implement the current
`handle.generation == store->generation_` check. Acceptable v2.1 outcomes are:

- Defer fully to v2.2 with a note that store structural generation is not Atom
  handle generation.
- Or land only a design memo/test sketch defining the correct semantics:
  slot generation for freed/reused/remapped rows, separate from column-reference
  generation.
- Or implement a much narrower debug assertion only if tests cover normal
  reserve/reallocation/compact/adoption paths and prove they do not trap.

**P5.5 Close review:** Renumber as planned after the trimmed scope lands.

## Overall verdict

**NEEDS-FIXES.** P5 as written mixes three different goals: benchmark
reproducibility, public v2.2-facing API, and stale-handle validation. Only the
benchmark reporting cleanup is clearly v2.1-shaped. The iterator API does not
deliver its stated materialization win in the current heap-Atom model, and the
generation guard would be semantically wrong if implemented as a direct
store-generation compare. Resequence P5.3 before any perf gating, defer P5.1
and heavy P5.2 to v2.2, and redesign or cut P5.4.
