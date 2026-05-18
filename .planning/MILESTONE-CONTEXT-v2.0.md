# BALL 2.0 — Milestone Context

**Status:** DRAFT for tag. **Authored:** 2026-05-18.
**Companion:** `RELEASE-NOTES-v2.0.md` (user-facing summary).

This document is the project-level handover for v2.0: what was
shipped, why the design landed the way it did, what wasn't shipped
and where it went, and how to navigate the planning tree.

---

## Scope of v2.0

v2.0's scope is the **KERNEL rewrite**: replace v1.x's Composite-tree
atom model (`Atom : Composite`) with a Struct-of-Arrays
`MoleculeStore` plus a thin Atom/Bond handle layer. Everything
downstream (FORMAT, MOLMEC, STRUCTURE, NMR, QSAR, SOLVATION, SCORING,
DOCKING, ENERGY, XRAY, VIEW, BALLView) was disabled during K0 and is
re-enabled in Track B.

K0 is now **complete** (phases K0.0 → K0.8). Track B is the next
major workstream.

---

## What shipped vs original plan

| Phase | Original spec | Shipped |
|---|---|---|
| K0.1 | Build baseline + green-bar 48 tests | Done |
| K0.2 | MoleculeStore skeleton + CSR + live-ref contract | Done |
| K0.3 | Atom-as-handle + dual-write, per-field flip | Done (.1-.7 dual-write; .LATER.1-10 flip; .c.1-10 fixes) |
| K0.4 | Per-System store + adoption | Done (.1 ownership; .2 adopt; .3 auto-adopt; .4-.8 R4 fixes) |
| K0.5 | Compiled selection ≥10× | Done (median 61.8× at 100k atoms; .0-.8 sub-phases) |
| K0.6 | **Binary persistence v2** + v1→v2 converter | **PIVOTED to JSON** (maintainer decision 2026-05-18); converter deferred to Track B |
| K0.7 | Memory + perf budget | Done — D13 documented as **PARTIAL** (SoA meets, full-handle misses by 3.1×) |
| K0.8 | Release-notes + cleanup | This document + RELEASE-NOTES-v2.0.md |

The K0.6 pivot is the only material spec change. Rationale: typical
BALL workloads (≤10k atoms) don't justify the 3–4 week custom-binary
budget, JSON gives debuggability / schema evolution / portability
for free, and a binary fast-path can be added later if profiling
shows the JSON path is on the hot path. Decision captured in
`K0.6-SUBPHASES.md` decision log.

---

## Architecture invariants

### D-decisions (full list in `KERNEL-V2-DECISIONS.md`)

- **D1** Atom-as-handle: `(MoleculeStore*, store_idx, generation)`. Sole authority for atom payload data is the store.
- **D2/D3/D4** Composite / PropertyManager / Selectable kept as base classes (thin-stub forwarders). Full deletion deferred to v2.1.
- **D5** Store ownership: per-System. `System` holds `unique_ptr<MoleculeStore>`.
- **D6** `Atom*` identity preserved. Handles are heap-allocated; pointer compare + `std::set<Atom*>` work unchanged.
- **D7** Reference-return getters: documented opt-in live-reference contract; `BorrowedColumnRef` RAII helper provided but not auto-wrapped.
- **D8** Generation counter on store + handle for stale-handle detection.
- **D11** Compiled selection in K0 (done; median 61.8×).
- **D12** Persistence format: **PIVOTED — JSON via nlohmann::json**, not custom binary.
- **D13** Memory budget: **PARTIAL** — see RELEASE-NOTES "Workload guidance".
- **D14** Iteration ≤2× v1.x baseline — not directly benchmarked; selector eval shows 61.8× margin.
- **D15** CSR bond adjacency, lazy-rebuilt on dirty.
- **D16** Per-System single-thread by contract; orphan store mutex-protected for concurrent default `Atom()` ctors.

### Adversarial review history

| Round | Subject | Findings | Outcome |
|---|---|---|---|
| R1 | K0 architecture + handover design | n/a | Captured in `K0-CODEX-REVIEW.md` |
| R2 | K0 design lock + roadmap | several BLOCKERs | D2/D3/D4 amended thin-stub; D6 heap-cost explicit |
| R3 | K0.3c (5 sub-phases) | 7+8+10 closed; 9 deferred → K0.3c.9 | Done |
| R4 | K0.3b.LATER + K0.4 | 6 HIGH + 3 MED + 1 LOW | K0.4.4-K0.4.8 (5 commits); MEDIUM-7 + LOW-10 deferred to v2.1 + K0.7 |
| R5 | K0.4 follow-up (manual; Codex CLI hung) | n/a | Manual self-review; verdict: go for K0.5 |
| R6 | K0.5 (compiled selection) | 3 OPEN closed inline (K0.5.7+8) | Done |
| R7 | K0.6.0-K0.6.3 (writer + reader + schema) | 4 OPEN closed inline (K0.6.3b) | Done |
| R8 | K0.6.5b (System + property + orphan) | 2 OPEN + 4 FYI; closed inline (K0.6.5c) | Done |
| R9 | K0.7 perf | 0 OPEN; doc-only findings | D13 wording corrected; M-of-N + gate tightening → v2.1 |

Every HIGH/OPEN closed before next phase. v2.0 ships with the FYI
items documented in `RELEASE-NOTES-v2.0.md`'s "Things deferred to
v2.1" table.

---

## Navigation

### Planning artifacts (`.planning/v2.x/`)

```
HANDOVER.md                       initial v2.x context-transfer
SEEDS-FORWARD.md                  pre-K0 brainstorming
CORE-MOLECULESTORE-RESEARCH.md    pre-design research
KERNEL-V2-DECISIONS.md            normative D-decision log
V2.0-ROADMAP.md                   phase graph + Track A/B split
MODULE-REENABLE-PLAN.md           Track B sequence
PARALLEL-EXECUTION.md             per-file-owner concurrency map
K0.5-SUBPHASES.md                 K0.5 breakdown
K0.6-SUBPHASES.md                 K0.6 breakdown (with JSON pivot decision log)
K0.7-SUBPHASES.md                 K0.7 breakdown
COMPILED-SELECTION-SPEC.md        K0.5 design contract
STORE-FORMAT-SPEC.md              ORIGINAL binary spec (superseded by K0.6 JSON pivot)
GAPS-FROM-V1.7.md                 v1.7 retrospective gaps to address in v2.x
OPEN-QUESTIONS.md                 v2.x forward maintainer-decision queue
GAP-ANALYSIS-PRE-RC.md            wrong-chat side-quest (BALLView bugs — see "Out of scope" below)
K0-CODEX-REVIEW.md                R1
K0-CODEX-REVIEW-ROUND2.md         R2 design lock
K0-CODEX-REVIEW-ROUND3.md         R3 K0.3c
K0-CODEX-REVIEW-ROUND4.md         R4 K0.3b.LATER + K0.4
K0-CODEX-REVIEW-ROUND5.md         R5 manual self-review
K0-CODEX-REVIEW-ROUND6.md         R6 K0.5
K0-CODEX-REVIEW-ROUND7.md         R7 K0.6.0-K0.6.3
K0-CODEX-REVIEW-ROUND8.md         R8 K0.6.5b
K0-CODEX-REVIEW-ROUND9.md         R9 K0.7
```

### Source artifacts

```
include/BALL/KERNEL/moleculeStore.h     SoA store (private, friended to JSON loaders)
include/BALL/KERNEL/atom.h              Atom handle (post-K0.3b.LATER deletes)
include/BALL/KERNEL/bond.h              Bond handle (store-aware)
include/BALL/KERNEL/system.h            System with per-instance store
include/BALL/KERNEL/compiledExpression.h Compiled-selection AST + cache
include/BALL/KERNEL/moleculeStoreJson.h JSON writer/reader for store
include/BALL/KERNEL/systemJson.h        JSON writer/reader for System + Molecule
include/BALL/KERNEL/propertyJson.h      PropertyManager JSON helpers
include/BALL/EXTERNAL/nlohmann_json.hpp Vendored JSON header (v3.11.3 MIT)

source/KERNEL/moleculeStore.C           Store impl + clear() + restore_stable_ids_for_load_
source/KERNEL/atom.C                    Atom impl + writeStore* + readStoreElement_
source/KERNEL/bond.C                    Bond impl
source/KERNEL/system.C                  System impl + adopt + adoptSubtree
source/KERNEL/compiledExpression.C      Eval engine + parser→AST lowerer + LRU cache
source/KERNEL/moleculeStoreJson.C       JSON writer + reader + bit-exact float helpers
source/KERNEL/systemJson.C              System JSON writer + 2-phase loader
source/KERNEL/propertyJson.C            PropertyManager → JSON
source/KERNEL/expressionParser.C        Mutex-serialised parse()

test/MoleculeStore_test.C               Store unit tests
test/CompiledExpression_test.C          K0.5 + integration with Expression
test/SelectorBench_test.C               100k bench at ≥10× gate (K0.7.2 update)
test/MoleculeStoreJson_test.C           K0.6.1-K0.6.3b store JSON
test/SystemJson_test.C                  K0.6.5/.5b/.5c System JSON
test/Sizeof_test.C                      sizeof pins (K0.7.1)
test/MemoryBudget_test.C                100k modeled budget (K0.7.3)
test/JsonBench_test.C                   JSON I/O profile (K0.7.4)
```

---

## What's NOT in v2.0

### Out of scope by design

- **BALLView refresh** — that's the parallel v1.8 wave (Phases
  999.44-999.48), not part of v2.0. v2.0 ships the KERNEL substrate
  rewrite; v1.8 ships the UI refresh on the v1.7 kernel; v2.0 + v1.8
  merge concerns are v2.0.1 work.
- **PyBALL v2** — Phase 6, gated on the binding-tool bake-off
  (Q1 in `OPEN-QUESTIONS.md`). v2.0 ships without Python bindings.
- **GUI bug reports** — two BALLView bug chips (welcome-screen
  redraw + Myoglobin-load crash) were filed via ccd_session task
  spawn on 2026-05-18 then maintainer-clarified as belonging to a
  different chat. Left as background tasks; not v2.0 scope. The
  `GAP-ANALYSIS-PRE-RC.md` artifact from that side-quest stays in
  the tree as triage notes.

### Deferred to v2.0.x patch releases

- **K0.6.4 v1→v2 JSON converter** — gated on FORMAT module re-enable
  (Track B). Ships in the patch release that closes Track B B1.
- **Full v1.x test surface (`Expression_test`, `Selector_test`, etc.)** —
  excluded from core build because they `#include
  <BALL/FORMAT/PDBFile.h>`. Re-enables with Track B.

### Deferred to v2.1

See `RELEASE-NOTES-v2.0.md` "Things deferred to v2.1" table.

---

## Hand-off notes

### For someone picking up Track B (module re-enable)

1. Sequence per `MODULE-REENABLE-PLAN.md`: FORMAT ∥ STRUCTURE → XRAY
   ∥ NMR ∥ ENERGY → MOLMEC → QSAR ∥ SOLVATION → SCORING → DOCKING →
   PYTHON.
2. Use `BALL_CORE_ONLY=OFF` to enable additional modules.
3. The K0.5 selector speedup claim should be re-verified once
   `Expression_test` / `Selector_test` / `StandardPredicates_test1..4`
   are buildable again.
4. K0.6.4 v1→v2 converter ships in the same patch that closes B1
   (FORMAT). Read `.bal` text files via the still-present
   `TextPersistenceManager` path; emit JSON via `saveSystemJSON`.

### For someone picking up v2.1

The v2.1 deferral list (`RELEASE-NOTES-v2.0.md`) sized by Codex R9:
1. **D2/D3/D4 actual removal** (biggest win — `sizeof(Atom)`
   360 → ~64 B, full-handle D13 met).
2. **JSON load batching** (closes the 15× save/load ratio).
3. **AndNode tmp-bitmap reuse** (minor selector perf).
4. **Element-instance id-table** (custom Element round-trip).
5. **CI-calibrated tight perf gates** (regression detection at 2×
   drift, not 100×).
6. **Median-of-N benchmark numbers** (reproducible release claims).

### Build instructions

`BUILD-macos.md` + `BUILD-linux.md` cover the current core-only build.
`BUILD-windows.md` is a documented gap (per `GAPS-FROM-V1.7.md` §1)
and lands in K0.8 or v2.0.x patch.

---

*Authored 2026-05-18 K0.8.*
