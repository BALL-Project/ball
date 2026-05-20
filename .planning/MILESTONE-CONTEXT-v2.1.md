# BALL 2.1 — Milestone Context

**Status:** READY FOR rc1. **Authored:** 2026-05-20.
**Companion:** `RELEASE-NOTES-v2.1.md` (user-facing summary).
**Predecessor:** v2.0.0 (commit `7a8dc7caf`).
**Branch:** `v2.1` (forked from the v2.0.0 tag).

Project-level handover for v2.1: what shipped, the decisions that
shaped it, what was deferred to v2.2 and why, and how to navigate
the planning tree.

---

## Scope of v2.1

v2.1 was *originally* scoped as the thin-handle refactor to hit the
D13 memory budget (delete the D2/D3/D4 base-class inheritance from
Atom, move the state to per-store side tables, target
`sizeof(Atom) ≤ 32 B`). During P0–P2 that scope was found to be
**too entangled to land safely in one milestone**:

- The inheritance flip touches the tree-node identity, RTTI surface,
  selection propagation, adoption boundary, PropertyManager surface,
  AtomIterator, and SIP bindings simultaneously (R20).
- Mutation-path mirroring to the side tables is destruction-order
  fragile against the current dual representation (P2.1.1 / D39).

So v2.1 was **re-scoped** (D40) to ship the **substrate + the
independent wins**, deferring the flip to v2.2:

- P1–P2: side-table scaffolding (the v2.2 flip's substrate).
- P3: Atom-RTTI consolidation (29 sites → 1 helper) — v2.2 prep.
- P4: JSON closures — **load 7–14× faster** (median-of-5; the
  single-run no-props figure was ~16×) + bond round-trip.
- P5: reproducible benchmark reporting.
- P6: release.

v2.1 is honestly a **"JSON + API hygiene + v2.2 prep"** release, not
the memory release. v2.2 is the thin-handle flip.

---

## What shipped vs plan

| Phase | Planned | Shipped |
|---|---|---|
| P0 | Design lock (thin-handle) | Done — D22–D34 locked; R17/R17b/R17c GO (3 iterations) |
| P1 | Side-table infra | Done — composite_nodes_ + property columns + sparse bag + selected bits; CompositeHandle/Node; R19 GO |
| P2 | Parallel side-table maintenance | **Re-scoped** — scaffolding (virtual hook + helper) only; mutation wiring deferred to v2.2 (D39); R20→R20d |
| P3 | Atom-RTTI removal | Done — 29 sites → `detail::compositeAsAtom_` + CI grep gate; R22 (NEEDS-FIXES, ShiftModule) → R23 close |
| P4 | JSON closures | Done — P4.1 load 7–14× (R24) + P4.2 bond props/graph (R25 NEEDS-FIXES → fixed) + P4.4 schema bump; P4.3 dropped (misframed) |
| P5 | Perf + iter API + gen-guard | **Trimmed** to P5.3 median-bench only (R26); P5.1/P5.2-heavy/P5.4 → v2.2 (D44); R27 close GO |
| P6 | Release | This document + RELEASE-NOTES-v2.1 + rc1 + R28 + v2.1.0 |

Two phases were materially re-scoped mid-flight (P2, P5) and one
sub-phase dropped (P4.3) — each driven by an adversarial-review
finding that the original premise was wrong. The audit trail
records every pivot.

---

## Decisions (full log in `V21-DECISIONS.md`)

The v2.1 D-series runs D22–D44. Headline calls:

- **D22b** Composite side-table: unified 5-link node (~48 B), not
  the original 3-link 24 B (R17 corrected the iterator-semantics
  requirement).
- **D23b** PropertyManager → per-store typed columns + sparse-first
  lookup + 65k dynamic-name cap + promote-once (R17b corrected the
  replacement semantics + unbounded growth).
- **D24b** Selectable → `unique_ptr<atomic<u64>[]>` rebuilt under
  exclusive lock (R17b: `vector<atomic>` isn't Cpp17MoveInsertable).
- **D39** Mutation wiring deferred to v2.2 (destruction-order
  fragility).
- **D40** Honest framing: v2.1 = JSON + API + v2.2 prep, NOT D13.
- **D41.1** No `Composite::isAtom_()` virtual — per-site RTTI
  refactor (would be invalid post-flip).
- **D42** V21-BOND-PROPERTY-JSON: initially deferred, then
  **implemented** in P4.2 (maintainer reversed) via option A
  (reconstruct Bond* graph in loader). Closes R16 C-B7 + D43.
- **D44** P5 trimmed to median-bench; STORE-ITER-API +
  pinned-baseline gates + generation-guard → v2.2.
- **D34d** MSVC CI → v2.2 (no sizeof target in v2.1).

---

## Adversarial review history

| Round | Subject | Outcome |
|---|---|---|
| R17 / R17b / R17c | P0 design lock (D22–D34) | NO-GO → NO-GO → **GO** (3 iterations; 3 BLOCKERs + 4 BUGs + new BLOCKERs caught) |
| R19 | P1 close | GO (D31b CI gate added) |
| R20 / R20b / R20c / R20d | P2 planning | NO-GO → GO (8 BLOCKERs; led to D39 re-scope) |
| R21 / R21b / R21c / R21d | P2–P6 roadmap | NEEDS-FIXES ×3 → **GO** (D40/D41/D34d) |
| R22 / R23 | P3 plan + close | NEEDS-FIXES (ShiftModule) → GO (29 sites consolidated) |
| R24 / R25 | P4 plan + close | GO / NEEDS-FIXES → fixed (2 boundary bugs in bond reconstruction) |
| R26 / R27 | P5 plan + close | NEEDS-FIXES (trim) → GO |
| R28 | rc1 pre-tag | _pending_ |

**Yield across R17–R27:** ~20 BLOCKER/BUG findings caught and fixed
in-cycle; several "premise falsified" findings (P2 inheritance
entanglement, P4.2 no-Bond*-on-load, P4.3 element-loss-at-setElement,
P5 materialisation-not-real) that redirected scope before code was
wasted. Zero BLOCKERs shipped.

---

## Benchmark snapshot (v2.1, 100k atoms, Darwin arm64 release)

| Metric | v2.0 | v2.1 |
|---|---:|---:|
| JSON load, no props | ~3,588 ms | **~260 ms** (14×) |
| JSON load, +3 props | ~3,750 ms | **~530 ms** (7×) |
| JSON save, no props | ~233 ms | ~290 ms |
| Selection speedup (synthetic median) | ~62× | ~55× (unchanged path) |
| SoA columns / atom | 139 B ✓ | 139 B ✓ |
| `sizeof(Atom)` | 360 B | 368 B (+8 P1.3 slot) |
| Full-fat / atom | 499 B | 507 B (3.2× over D13) |

The memory line is the v2.2 story; everything else is v2.1's
delivered value.

---

## Navigation (`.planning/v2.1/`)

```
MILESTONE-v2.1-KICKOFF.md     original (thin-handle) scope
V21-DECISIONS.md              D22–D44 normative decision log
V21-ROADMAP.md                P2–P6 roadmap (trimmed P5)
BACKLOG.md                    v2.2 deferrals (tracked IDs)
P1-PLAN.md / P2-PLAN.md       phase plans
P3-CLASSIFICATION.md          the 29 RTTI sites + refactor
P4-PROFILE.md                 the O(n²) load profile
P4.1-DESIGN.md                V21-LOAD-BATCH design
P4.2-FINDINGS.md              no-Bond*-on-load discovery + options
P4.3-FINDINGS.md              element-loss-at-setElement (drop)
V21-CODEX-REVIEW-ROUND{17..27}.md   adversarial review records
RELEASE-NOTES-v2.1.md         user-facing summary
MILESTONE-CONTEXT-v2.1.md     this document
```

Source artifacts touched: `include/BALL/KERNEL/_moleculeStoreInternal.h`
(side tables), `include/BALL/CONCEPT/composite.h` (handle slot +
helper), `source/KERNEL/{moleculeStore,systemJson,moleculeStoreJson}.C`
(load-batch + bond JSON), the 17 RTTI-refactored module files,
`test/BenchStats.h`.

---

## Hand-off to v2.2

v2.2 is the thin-handle flip. Its substrate is in place:

1. **Side tables exist** (P1) — composite_nodes_, property columns,
   sparse bag, selected bits. v2.2 wires mutations to them (D39)
   and flips Atom's inheritance to the empty D17* shims (D26a +
   BALL_EMPTY_BASES).
2. **RTTI is consolidated** (P3) — the v2.2 flip replaces one
   `detail::compositeAsAtom_` definition, not 29 call sites.
3. **Backlog is explicit** (`BACKLOG.md`): V21-STORE-ITER-API,
   V21-CI-PERF-GATES, V21-GENERATION-GUARD, V21-MSVC-CI-PORTABILITY,
   V21-BOND-PROPERTY-JSON (done in P4.2), V21-VIEW-RTTI.
4. **Targets**: `sizeof(Atom)` 368 → ~32 B; full-fat ≤160 B/atom
   (D13 met); MSVC EBO verification gates the sizeof claim (D34d).

*Authored 2026-05-20 P6.*
