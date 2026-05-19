# BALL 2.1 — Detailed Roadmap P2–P6

**Authored:** 2026-05-19, post-P2.1.1 ship.
**Status:** DRAFT (R21 review pending).
**Predecessors:** R17/R17b/R17c P0 design lock GO; R19 P1 close GO; R20-R20d P2 planning iteration; P2.1.0 + P2.1.1 shipped (`6c3e915b9` + `205a05f4f`).
**Successors:** R21 roadmap review; per-phase planning + close reviews (R22+).

## Strategic posture

R20-R20d converged on D36's two-phase split: v2.1 scaffolding +
RTTI cleanup + JSON improvements; v2.2 inheritance flip +
delete v0 inline state. The P2.1.1 destruction-order discovery
sharpens this: **mutation-path mirroring of v0 state to side
tables is itself destruction-order-fragile**. The cascading
~System → ~Composite → parent->removeChild path runs mirror
calls during partial teardown, corrupting the heap.

This roadmap **doubles down on D36**: v2.1 ships scaffolding +
direct-API parity tests + RTTI cleanup + JSON. **No mutation
wiring in v2.1.** v2.2 owns the wiring + inheritance flip as
one atomic surgery, where it has destructor-order control via
the new inheritance hierarchy.

## D39. Mutation wiring deferred to v2.2 (new decision)

**Decision:** v2.1 does NOT wire `Composite` / `PropertyManager` /
`Selectable` mutation paths to maintain side-table state. The
side tables remain unpopulated by production mutations through
the v2.1 release.

**Rationale:**
- P2.1.1 attempt revealed destruction-order corruption when
  ~System cascades child destruction → mirror calls touch
  store whose lifetime is interleaved.
- The "destruction-in-progress" guard fix is itself fragile:
  each new Composite mutation path needs an audit; future
  refactors of teardown order could re-expose the bug.
- D36 already plans v2.2 to flip Atom's inheritance to
  `D17Composite + D17PropertyManager + D17Selectable` empty
  shims. At that flip, the new hierarchy gives clean control:
  D17Composite's destructor can release the side-table handle
  in a known order, with no v0 destruction cascade to interleave.
- v2.1 still delivers measurable value (D40 below): RTTI
  cleanup (24 sites), JSON closures (V21-LOAD-BATCH, V21-BOND-
  PROPERTY-JSON, V21-ELEMENT-INSTANCE-ID), perf gates +
  benchmarks, store-iter API. The wiring deferral does not
  affect any of those.

**Trade-off acknowledged:** side tables are exercised only via
direct-API parity tests in v2.1. There is no production use of
the side-table state. This is documented as the v2.1 → v2.2
plan in RELEASE-NOTES-v2.1.md.

**Alternative considered (rejected):** per-Composite
`is_being_destroyed_` flag. Rejected because every new
Composite subclass + every new teardown path needs an audit;
the guard becomes a long-tail correctness burden.

## D40. v2.1 deliverable summary — honest framing

**Framing (R21 P21-2 + R21-F):** v2.1 is **NOT a thin-handle /
memory-budget release**. It is a **"JSON improvements + API
hygiene + v2.2 preparation"** minor release. RELEASE-NOTES-v2.1.md
must frame it that way. The headline D13 budget claim that v2.0
deferred remains deferred to v2.2.

User-visible value v2.1 actually delivers:
- JSON load 15× → 2× (P4.1, gated on profiling result)
- Bond-property JSON round-trip closes v2.0 known gap (P4.2)
- Custom-Element pointer-identity preserved across JSON (P4.3)
- Store-native iteration API for hot-path consumers (P5.1)
- Generation-guard `BALL_DEBUG` mode catches stale-handle bugs (P5.4)

Maintainer-value v2.1 delivers (not user-visible but real):
- 24-site Atom-RTTI removal in kernel prepares v2.2
- Side-table scaffolding shipped + direct-API parity tests
- CI perf gates calibrated for ~2× regression detection
- v2.1.0 cuts a stable mid-milestone snapshot

What v2.1 ships (after this roadmap closes):

1. **Side-table SCAFFOLDING** (P1 done + P2.1.0/1 done):
   - `MoleculeStoreSideTables` PImpl with composite_nodes_ +
     property_columns_ (10 well-known + 65k cap) + sparse_bag_
     + selected_bits_ atomic-word array
   - `Composite::mirrorToSideTable_()` helper + virtual hooks
   - `CompositeHandle` 8 B + `CompositeNode` 48 B
   - Direct-API parity tests
2. **RTTI cleanup** (P3): 24 Atom-RTTI sites in kernel
   removed; VIEW + APPLICATIONS deferred per D37.
3. **JSON closures** (P4):
   - V21-LOAD-BATCH (close 15× save/load gap)
   - V21-BOND-PROPERTY-JSON
   - V21-ELEMENT-INSTANCE-ID
4. **Perf + iteration** (P5):
   - V21-STORE-ITER-API (public)
   - V21-CI-PERF-GATES (calibrated to ~2× regression)
   - V21-MEDIAN-OF-N-BENCH
   - generation-guard `BALL_DEBUG` mode
5. **Release** (P6):
   - RELEASE-NOTES-v2.1.md
   - MILESTONE-CONTEXT-v2.1.md
   - v2.1.0-rc1 → v2.1.0

What v2.1 does NOT ship:
- `sizeof(Atom) ≤ 32 B` (v2.2)
- Mutation wiring (v2.2)
- Inheritance flip (v2.2)
- D13 budget closure (v2.2)
- Windows MSVC CI (D34c → moved to v2.1.0-rc1 prep, may slip
  to v2.2 if Phase 4 vcpkg isn't ready)

## Phase plan

### P2 (closed)

Originally "parallel side-table maintenance"; per D39, mutation
wiring deferred to v2.2. P2 closure = P2.1.0 + P2.1.1 already
shipped.

P2 STATUS: **DONE.** R21 roadmap review (this document) gates
P3. No separate R-21-close needed for P2 since the scaffolding
work was reviewed in R19.

### P3 — Atom-RTTI removal in kernel (24 sites)

**Goal:** delete every `dynamic_cast<Atom*>(...)` and
`RTTI::isKindOf<Atom>(...)` from KERNEL / STRUCTURE / FORMAT /
QSAR / MOLMEC / SCORING / SOLVATION / DOCKING / ENERGY / NMR /
CONCEPT modules.

**Distribution (R21 P21-3 confirmed):** 13 in NMR, 2 in
STRUCTURE/disulfid, 1 each in selector, MOL2File, atomVector,
geometricProperties, molecularInformation, moleculeObjectCreator
+ a few others — 24 total.

**Strategy revision (R21 P21-4 + R21-F2):** Do **NOT** default to
`Composite::isAtom_()` as a universal RTTI replacement. That
virtual would be wrong-or-useless when v2.2 removes
`Atom : Composite`. Instead, **classify each site individually
and refactor in place**:

**Sub-phases:**

| # | Subject | Strategy |
|---|---|---|
| P3.1 | **Per-site classification.** Read each of the 24 sites; assign one of three categories: (T) typed-call-path-available — caller's context already knows it's an Atom, refactor to take `Atom*` directly; (S) store-backed-identity — caller has a Composite* but can query `store->back_ptr(idx)` or similar to confirm it's an Atom; (V) visitor-needed — genuine tree-discrimination case requiring runtime dispatch. | per-site spreadsheet in P3.1 commit message |
| P3.2 | Remove (T) typed-call-path sites | refactor in place; tests of touched module pass |
| P3.3 | Remove (S) store-backed sites | use `Atom::getStore()` / `back_ptr_` / `store_idx_` for identity check; no new virtual |
| P3.4 | Remove (V) visitor sites — ONLY if (T)/(S) don't apply | introduce minimal visitor pattern if needed; document why (T)/(S) wasn't sufficient |
| P3.5 | SIP Python bindings | not in CORE_ONLY; deferred to v2.1.x or v2.2 |
| P3.6 | Close review (R23) | Codex reviews each removal for correctness + RTTI-leak grep gate |

**Anti-pattern explicitly rejected:** adding
`Composite::isAtom_()` as a default replacement. v2.2 inheritance
flip removes the Atom : Composite hierarchy; any code relying on
`isAtom_()` virtual dispatch becomes invalid at that flip.
Per-site refactor is more work but produces correct v2.2-ready
code.

**Risk:** the 24 sites span 11 modules; some may have subtle
behavioral coupling. Mitigation: each site removed in its own
commit (or small batch) with the related test exercised.

**Test coverage gates:** Every touched module's existing tests
must pass. CI grep gate added to ci-v2.yml that fails the
build if `dynamic_cast<.*Atom.*>` or `isKindOf<Atom>` appears
in the CORE_ONLY module set (excluding documented exceptions
in VIEW/APPLICATIONS).

### P4 — JSON closures

**Goal:** ship the three deferred JSON features per D40.

**Sub-phases:**

| # | Subject | Deliverable |
|---|---|---|
| P4.0 | **Profile-driven load analysis** (R21-F1) | Before designing the batch loader, profile a representative 100k-atom load. Bucket time into: (a) JSON parse via nlohmann, (b) heap Atom allocation, (c) Composite tree insertion + adoption (parent/sibling pointer chains), (d) store-column writes via slot_map, (e) PropertyManager bag restoration, (f) bond record restoration. The 15× gap target attribution is conditional on which bucket dominates. Write `P4-PROFILE.md` with bucket times before P4.1 design lock. |
| P4.1 | V21-LOAD-BATCH | Design + implementation TARGETED at the dominant bucket from P4.0. If (b) dominates → batch Atom allocation. If (c) dominates → batch Composite insertion (defer adoption until all atoms allocated). If (d) dominates → batch column writes. JsonBench_test gates measurable improvement; the SPECIFIC numeric target ("8× load speedup" / "≤2× save:load ratio") is set in P4.1 commit message based on P4.0 profile + theoretical best-case for the chosen optimization. |
| P4.2 | V21-BOND-PROPERTY-JSON | Serialise Bond handle's PropertyManager bag (MMFF94SBMB, MMFF94RBL, VIRTUAL__BOND, HBondProcessor annotations) in K0.6 JSON. New `bond_property_columns` schema section per D30a MINOR bump. Per R21 P21-6: clean win — Bond's v0 PropertyManager is directly readable; no side-table involvement needed. Identity: serialize in live `bonds[]` row order. |
| P4.3 | V21-ELEMENT-INSTANCE-ID | Custom-Element round-trip via instance-id table. v2.0 placeholder behavior (pointer-identity loss) closed. |
| P4.4 | JSON MINOR bump to 1.1 | schema version + reader/writer match. v2.0 readers fail cleanly on v2.1 JSON (per D30). |
| P4.5 | Close review (R25) | Codex reviews JSON round-trip correctness + backward read + profile-driven design rationale |

**Risk:** V21-LOAD-BATCH changes the JSON loader fast path —
introduces a new performance-sensitive code path. Mitigation:
P4.0 profile gates the design; JsonBench_test gates the perf
claim; existing MoleculeStoreJson_test verifies correctness.

### P5 — Perf + iteration API + generation-guard

**Goal:** complete the v2.1 perf/instrumentation story.

**Sub-phases:**

| # | Subject | Deliverable |
|---|---|---|
| P5.1 | V21-STORE-ITER-API | New public `MoleculeStore::iterAtoms()` / `iterBonds()` returning lightweight iterators that skip `Atom*` materialisation. Hot loops can use this; existing v0 iteration paths unchanged. |
| P5.2 | V21-CI-PERF-GATES | **Concrete calibration methodology (R21 P21-8):** collect 5 clean CI runs per benchmark on GHA macos-15 baseline (Apple Clang Release CORE_ONLY). Each benchmark internally runs N=5 iterations; report median + min/max + coefficient of variation. Gate at `max(2 × baseline_median, baseline_median + 6 × MAD)` (MAD = median absolute deviation; robust to outliers). Benchmarks with CoV > 30% are marked "advisory" (warning-only) until calibration improves. |
| P5.3 | V21-MEDIAN-OF-N-BENCH | Bench harness refactor to report median + p99 + MAD across N runs; release-claim numbers reproducible. |
| P5.4 | Generation-guard `BALL_DEBUG` mode (R21 P21-9) | Per D25: in `BALL_DEBUG` builds, every Atom handle deref asserts `handle.generation == store->generation_`. **Verification required:** (a) #ifdef BALL_DEBUG wraps the entire check in inline getters in `atom.iC`; (b) release build's disassembly (or `nm`/`strings`) of `Atom::getPosition()` etc. shows no compare against `generation_`; (c) new debug-mode test asserts that stale-handle deref traps. CI runs both release + debug suites. |
| P5.5 | Close review (R27) | Codex reviews perf gates + iter API completeness + generation-guard #ifdef proof |

**Risk:** STORE-ITER-API is new public API surface (per D33b
this needed a planning review — handled in R26 below). Must be
forward-compatible with v2.2's inheritance flip.

### P6 — Release

**Goal:** publish v2.1.0.

**Sub-phases:**

| # | Subject | Deliverable |
|---|---|---|
| P6.1 | RELEASE-NOTES-v2.1.md | Draft per D40 honest framing — "JSON improvements + API + v2.2 prep" not "thin-handle"; include explicit "Not in v2.1" section listing sizeof + wiring + D13 |
| P6.2 | MILESTONE-CONTEXT-v2.1.md | Project-level handover doc |
| P6.3 | v2.1.0-rc1 tag | Pre-tag review R28 |
| P6.4 | Bake period | Wait for issue reports; iterate to rc2/rc3 if needed |
| P6.5 | v2.1.0 final | Tag + GitHub release |
| P6.6 | **Branch strategy after final tag (R21 P21-10 / R21-F4)** | Tag from `v2.1`. Merge `v2.1` → `master` (fast-forward if master hasn't diverged; otherwise rebase or merge-with-explicit-resolution). **DO NOT merge `v2.1` into `v2.0`** — v2.0 is the maintenance branch for v2.0.x patch releases; shared critical fixes are cherry-picked across `v2.0` ↔ `v2.1` ↔ `master` as needed per D32b's policy. After v2.1.0 final, the v2.2 milestone branch forks from `v2.1.0` tag (mirroring how `v2.1` forked from `v2.0.0`). |

## Codex review cadence (per D33b, revised R21 P21-12)

| Round | Phase / subject | Status |
|---|---|---|
| **R21** | This roadmap | iterating (R21 NEEDS-FIXES, this revision → R21b) |
| R22 | P3 planning — RTTI removal strategy + per-site classification | needed (RTTI replacement can change behavior) |
| R23 | P3 close | needed |
| R24 | P4 planning — JSON closures + load-batch design (post-profile) | needed (JSON load/schema changes user-visible) |
| R25 | P4 close | needed |
| ~~R26~~ | ~~P5 planning~~ — **downgraded to maintainer checklist** | P5 is mostly mechanical: perf gates + bench harness + #ifdef guard. STORE-ITER-API is a short API surface — handled in a small API-design memo in P5.1 commit message rather than full Codex round. |
| R26 (was R27) | P5 close | needed (perf claim verification) |
| R27 (was R28) | v2.1.0-rc1 pre-tag | needed |

Total revised: **6 more Codex rounds** (R21 + R21b + R22-R23 + R24-R25 + R26 + R27).

R21b needs to GO before P3 starts. Per the convergence pattern
(R20→R20d took 4 rounds), expect 1-2 more iterations after this
revision.

## Risk register (v2.1 milestone level)

- **R-V21.A (LOW):** D39 deferral might be seen as
  "incomplete" by reviewers. Mitigation: D40 explicitly lists
  v2.1 deliverables — RTTI + JSON + perf are real wins.
- **R-V21.B (MEDIUM):** P3 RTTI removal in 24 sites may
  uncover non-obvious behavioral coupling. Mitigation:
  per-site commits + per-site test verification.
- **R-V21.C (LOW):** P4 V21-LOAD-BATCH may not hit the 8×
  improvement target (current 15× save/load → target 2× → 8×
  load speedup). Mitigation: even partial improvement is a
  win; the gate is "improvement," not "specific magnitude."
- **R-V21.D (MEDIUM):** P5 STORE-ITER-API design needs to
  forward-compat with v2.2's inheritance flip. Mitigation:
  R26 planning review explicitly evaluates the v2.2-flip
  compatibility.
- **R-V21.E (LOW):** Windows MSVC CI (D34c) may slip past
  v2.1.0 if Phase 4 vcpkg isn't ready. Mitigation: documented
  as a v2.1.x backlog item; doesn't block v2.1.0 if CI runs
  Apple Clang only.

## Estimated timeline

- R21 + iterations: ~1 day (4 codex rounds × 10 min + fixes)
- P3 RTTI removal: 1-2 days (24 sites + R22/R23)
- P4 JSON closures: 1-2 days (3 features + R24/R25)
- P5 perf: 1 day (4 sub-phases + R26/R27)
- P6 release: 0.5 day (notes + rc1 + tag)

**Total v2.1 milestone:** ~5-7 working days from this roadmap
lock to v2.1.0 final tag.

## Next action

After this roadmap commits: spawn R21. Iterate until GO. Then
execute P3 → P4 → P5 → P6 sequentially with per-phase Codex
gates.
