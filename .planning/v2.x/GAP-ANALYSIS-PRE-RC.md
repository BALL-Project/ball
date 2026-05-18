# Pre-RC Gap Analysis — BALL 2.0

**Status:** ACTIVE (2026-05-18). Triggered by maintainer directive: digest
all known gaps, break into separate issues, adversarial-review each
with Codex, identify root causes, align with still-open items, plan
parallel fix execution, **only cut a new RC once all gaps closed**.

This document is the single source of truth for what blocks the next
RC. Each gap below has: ID, severity, code references, root-cause
hypothesis (filled after adversarial review), parallel-execution lane,
and exit criteria.

Loop autopilot is **paused** while this analysis runs. K0.6.2+ resumes
only after the gap closure plan lands.

---

## Tier A — RC blockers (newly reported, 2026-05-18)

### A1 — BALLView welcome-screen redraw artifacts
**Severity:** Blocker (user-visible on every launch).
**Reported:** maintainer screenshot, 2026-05-18.
**Evidence:** Right pane "Representation" tab bar duplicated and
overlapping; welcome panel paints over GL scene with red/green/yellow
leftover pixels; Logs dock buttons stacked 3-4×; stray tiny Logs
widget in bottom-right corner; "Skip this screen on startup" button
duplicated.
**Code surface (suspect):**
- `source/APPLICATIONS/BALLVIEW/main.C` — startup sequence
- `source/VIEW/WIDGETS/mainControl.C` — dock construction +
  `restoreState()`
- `source/VIEW/DIALOGS/welcome*` (if exists) — sample-card welcome
- `source/VIEW/WIDGETS/scene.C` — QOpenGLWidget paint clearing
- BALLView `QSettings` storage (~/Library/Preferences on macOS)
**Root cause (hypothesis):**
- Dock list built BOTH from code-default AND from saved state without
  dedup — docks appear twice on QSettings restore.
- Welcome panel parent is a `QOpenGLWidget`; the GL widget's paint
  doesn't honor the overlay so welcome paints leave residue.
- The duplicated tab/button rendering matches multiple dock instances
  drawing into the same screen region.
**Parallel lane:** Independent of all kernel work; can run
concurrently with A2 + Tier B.
**Owner:** Spawned chip "Fix BALLView redraw artifacts in welcome/dock
layout" (background task).
**Exit:** Welcome panel + Representation panel + Logs dock render
cleanly on (a) fresh QSettings, (b) saved-state restore. Verification
screenshot attached to fix commit.

### A2 — BALLView crash on Myoglobin load after clearScene
**Severity:** Blocker (data-loss adjacent — crash).
**Reported:** maintainer report, 2026-05-18.
**Evidence:** Click Myoglobin (1YMG) sample card → crash after a
clearScene operation.
**Code surface (suspect):**
- `source/APPLICATIONS/BALLVIEW/main.C` — sample-card click handler
- `source/VIEW/WIDGETS/mainControl.C` — `clearComposites()` /
  `removeComposite()`
- `source/VIEW/KERNEL/scene.C` — renderer thread + primitive list
  swap
- `source/VIEW/PRIMITIVES/*` — primitive list ownership
- `source/KERNEL/system.C` — `~System` handle invalidation (K0.4)
  may matter IF BALLView is built from v2.0 branch
**Root cause (hypothesis):**
1. Use-after-free: clearScene deletes System; sample loader (or
   renderer thread) still holds Atom*/Composite* refs.
2. Renderer thread race: clearScene swaps primitive list mid-paint.
3. Welcome panel re-entry: stale dispatch into deleted slots.
4. K0.4 handle invalidation (if v2.0 build): every held Atom* fault
   after `~System` runs `migrateTo_(nullptr)`.
**Parallel lane:** Independent of all kernel work; runs concurrently
with A1 + Tier B. Diagnosis MAY surface a K0 contract gap that needs
back-propagation into KERNEL-V2-DECISIONS.md.
**Owner:** Spawned chip "Fix BALLView crash on sample-load after
clearScene" (background task).
**Exit:** Clear scene → load any sample → render → repeat 5× without
crash. Backtrace + root-cause writeup attached. If root cause is K0
contract, follow-up issue filed.

---

## Tier B — K0.6 in flight (paused mid-flight)

### B1 — K0.6.2 JSON reader
**Status:** Was about to start when pivot came in. Writer (K0.6.1)
landed in commit `4433a350b`. Reader parses a v2 JSON doc back into
a `MoleculeStore`, allocates slots, fills columns, rebuilds bond
table, restores selection + stable-id map, replays release_atom on
is_freed[i]==1 slots.
**Severity:** Important (no writer-without-reader story).
**Parallel lane:** Sequential after gap-closure plan land; can
overlap with Tier A fixes once Tier B unfreezes.
**Exit:** Round-trip identity test on the K0.6a corpus.

### B2 — K0.6.3 schema versioning + invariants
**Status:** Pending.
**Severity:** Important.
**Parallel lane:** Sequential after B1.
**Exit:** Future-version rejection test + bit-exact float opt test.

### B3 — K0.6.4 v1→v2 converter tool
**Status:** Pending. **GATED on FORMAT module re-enable (Tier D2)**.
**Severity:** Important for migration story; Optional for RC if
fresh v2.0 users don't need v1 import.
**Parallel lane:** Blocked on FORMAT.

### B4 — K0.6.5 System-level JSON (full tree + property bag)
**Status:** Pending.
**Severity:** Important — replaces v1.x TextPersistenceManager use
case for full System save/load.
**Parallel lane:** Sequential after B1/B2.

### B5 — K0.6.6 Codex Round 7 adversarial review
**Status:** Pending.
**Severity:** Required before RC.
**Parallel lane:** Sequential after B1–B4.

---

## Tier C — Codex-deferred FYIs

### C1 — R4 MEDIUM-7 Element pointer identity loss
**Status:** Documented as v2.0 regression in
`K0-CODEX-REVIEW-ROUND4.md`.
**Severity:** FYI for users — `setElement(custom_Element_instance)`
loses identity through PTE canonicalisation on readback.
**Action:** Document in RELEASE-NOTES-v2.0.md draft (Tier D4).

### C2 — R4 LOW-10 sizeof(Atom) unmeasured
**Status:** Deferred to K0.7 perf phase.
**Severity:** Budget risk only.
**Action:** Confirmed K0.7 scope.

### C3 — R6 FYI-4 getCompiled shared_ptr semantics if caller outlives store
**Status:** Documented behaviour; no caller in tree triggers the
hazard. Hand out weak_ptrs or doc the contract.
**Severity:** API docs.
**Action:** Tier D4 (release notes) OR small fix to add a doc
comment + test.

### C4 — R6 FYI-6 AndNode tmp-bitmap churn for deep ASTs
**Status:** Optimisation, not correctness.
**Severity:** Performance — bounded per-node, not per-child.
**Action:** Deferred to K0.7 with scratch-buffer / arena work.

### C5 — R6 OPEN-5 benchmark gate too soft (5× test vs 10× design)
**Status:** Test asserts ≥5×, prints actual ratio (median 50.7×).
Codex flagged the gate as insufficient proof.
**Severity:** Test gate weakness.
**Action:** Tighten test gate to ≥10× in Tier D3 (K0.7 perf phase)
when 100k-atom corpus is in place.

---

## Tier D — Pre-RC infrastructure

### D1 — `BUILD-windows.md` doc
**Status:** Tracked in `GAPS-FROM-V1.7.md` §1.
**Severity:** Medium — downstream users lack a from-source Windows
guide.
**Parallel lane:** Independent doc work.
**Exit:** Doc reviewed + lifted from CI workflow content + tested on
a clean Windows VM.

### D2 — Module re-enable (Track B)
**Status:** Roadmap defines `B1: FORMAT ∥ STRUCTURE → B2: XRAY ∥ NMR ∥
ENERGY → B3: MOLMEC → B4: QSAR ∥ SOLVATION → SCORING → DOCKING →
PYTHON`. Per `MODULE-REENABLE-PLAN.md`.
**Severity:** Required to satisfy the K0.5 gate's full test set
(Expression_test, Selector_test, StandardPredicates_test1..4 all
need FORMAT/PDBFile.h). Without FORMAT, the K0.5 ≥10× speedup claim
holds only against the subset we can build.
**Parallel lane:** B1 (FORMAT ∥ STRUCTURE) can start parallel to A1/A2;
later waves serialize on B1.
**Exit:** Full `ctest -j8` run with all ~246 tests green.

### D3 — K0.7 perf budget verification
**Status:** Pending. Covers C2 (sizeof) + C5 (benchmark gate) +
100k-atom workload memory budget per D13/D14.
**Severity:** Required before RC (per V2.0-ROADMAP §7 Gates).
**Parallel lane:** After D2 (need module re-enable for realistic
workload).

### D4 — K0.8 cleanup + `RELEASE-NOTES-v2.0.md`
**Status:** Pending.
**Severity:** Required before RC.
**Parallel lane:** Final phase; consolidates C1/C2/C3/C4/C5 + Tier A
fixes into the release notes.

---

## Parallel execution plan

```
Lane 1 (A1):  BALLView redraw fix     [chip spawned]
Lane 2 (A2):  BALLView crash fix      [chip spawned]   ──┐
Lane 3 (D2):  Module re-enable B1                          ├─ RC GATE
              (FORMAT ∥ STRUCTURE)                       ──┤
Lane 4 (B):   K0.6 reader → schema → System-level → R7  ──┤
Lane 5 (D1):  BUILD-windows.md draft                     ──┤
Lane 6 (D3):  K0.7 perf budget after D2                  ──┤
Lane 7 (D4):  K0.8 release-notes + Tier C consolidation ──┘
```

Lanes 1, 2, 3, 5 can run in parallel from day 1. Lane 4 (K0.6) waits
on this gap-analysis document landing; Lane 6 (D3) waits on Lane 3;
Lane 7 (D4) waits on everything else.

---

## RC gate criteria

Cut a new RC ONLY when ALL of the following hold:

- [ ] **A1** redraw artifacts fixed; verification screenshot in commit
- [ ] **A2** crash fixed; backtrace + RCA + fix commit; no regression
      on the K0 kernel sweep
- [ ] **B1–B5** K0.6 JSON persistence complete + Codex Round 7 returns
      no OPEN findings
- [ ] **D1** BUILD-windows.md merged
- [ ] **D2** All disabled modules re-enabled; full `ctest -j8` green
- [ ] **D3** K0.7 perf budget verified at 100k atoms; benchmark gate
      tightened to ≥10×; sizeof(Atom) ≤ K0 design budget
- [ ] **D4** RELEASE-NOTES-v2.0.md drafted; Tier C items documented;
      `MILESTONE-CONTEXT-v2.0.md` authored
- [ ] Tri-OS CI (macOS-arm64 + Linux-x64 + Windows-x64) green
- [ ] Final Codex Round 8 adversarial review on the full RC tree
      returns no blockers

---

## Adversarial-review status

Codex CLI reviews to run (one per issue cluster):

| Cluster | Codex run | Status |
|---|---|---|
| A1 + A2 BALLView UI | Round-7-UI: paint/dock + crash diagnosis | PENDING (spawn this turn) |
| Tier B K0.6 JSON design | Round-7-K0.6: writer + planned reader/converter design audit | PENDING |
| Tier C/D pre-RC | Round-8: full v2.0 RC tree audit | LATER (after all fixes land) |

---

## Next actions (this turn)

1. Land this gap analysis (this commit).
2. Spawn Codex Round-7-UI in background to triage A1 + A2 against the
   actual source (the spawned ccd chips do the fix; Codex provides
   independent root-cause confirmation + bug-pattern audit).
3. Spawn Codex Round-7-K0.6 in background to audit the K0.6.1 writer
   + the planned reader/converter shape against the JSON pivot.
4. Pause loop autopilot (no ScheduleWakeup this turn). Next maintainer
   prompt or explicit /loop reactivates.
