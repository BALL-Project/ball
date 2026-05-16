# KERNELV2.md — BALL 2.1 Kernel Redesign Plan

**Status:** PLANNING — adopted after 2 rounds of Codex peer review
**Target release:** BALL 2.1 (with v2.1-preview milestone after K2)
**Predecessor:** BALL 2.0 (post-1.6 baseline)
**Created:** 2026-05-16
**Last revised:** 2026-05-16 (post-Codex Round 3; v3 baseline)

---

## 1. Executive Summary

BALL's KERNEL subsystem implements the molecular hierarchy via the **Composite pattern** (`Atom : Composite + PropertyManager`). This design imposes ~800 B/atom overhead, fixed `Bond* bond_[12]` arrays ([atom.h:982](include/BALL/KERNEL/atom.h#L982)), heavyweight Bond objects ([bond.h:55](include/BALL/KERNEL/bond.h#L55)), macro-based iteration, RTTI-driven selection, and tree-walking traversal that defeats vectorization in force-field hot paths.

**This plan does NOT propose a clean-room replacement.** Two rounds of Codex peer review confirmed that the Composite-based object model is structurally welded to:
- 2,463 `Atom*`/`Atom&` reference sites
- 111 `BALL_FOREACH_*` macro call sites
- 53 `dynamic_cast<>` invocations on KERNEL types
- File-format persistence that writes raw pointer-graph links ([composite.C:102–107](source/CONCEPT/composite.C#L102), [atom.C:126–128](source/KERNEL/atom.C#L126), [bond.C:129–130](source/KERNEL/bond.C#L129))
- ~207k LOC of dependent subsystems
- Python bindings exposing Composite APIs directly ([composite.sip](source/PYTHON/EXTENSIONS/BALL/composite.sip))
- A fixed `Atom` binary layout including `bond_[12]`, position, velocity, force ([atom.h:970–986](include/BALL/KERNEL/atom.h#L970))

Instead, this plan proposes a **parallel-model migration**: introduce a `MoleculeStore` (SoA columns + handle facade) **alongside** Composite — with sync state held in an *external registry* keyed by `System*`, not embedded in Atom/Composite (which would force a binary ABI break). Hot paths route through the store; legacy code continues to use Composite. KERNEL v2 ships when MoleculeStore is the canonical data path for force-field workflows and selection queries.

### Release milestones

| Milestone | Scope | Calendar Time |
|---|---|---|
| **v2.1-preview** | K0 + K1 + K2 (foundation + nonbonded SoA + compiled selection) | **8–14 months** |
| **v2.1.0** | + K3 minimum (FORMAT + MOLMEC bonded + SCORING) + K4 Python | **16–24 months total** |
| **v2.1.1+** | Remaining K3 subsystems (DOCKING, QSAR, NMR, XRAY, STRUCTURE) | **+6–10 months** |
| **v2.1 final** | All of K0–K4 complete | **22–34 months total** |

### Compatibility promise

- **Source compatible** across v2.0 → v2.1 — existing client code recompiles unchanged.
- **ABI compatible** for all KERNEL+CONCEPT exported types: no fields, vtable entries, or `friend class` semantics changed. New types (`MoleculeStore`, `AtomView`, `CompiledExpression`) are additive exports.
- **File formats unchanged** in v2.1.
- `BALL_FOREACH_*` macros preserved; deprecated for *new internal code* only, with a `[[deprecated]]` signal on the equivalent inline range function.

---

## 2. Goals & Non-Goals

### Goals (v2.1)
1. Force-field workloads (MOLMEC AMBER/CHARMM/MMFF94, SCORING, SOLVATION) run nonbonded inner loops on SoA columns from MoleculeStore — not Composite traversal.
2. MoleculeStore is the canonical hot-path data structure for new code.
3. Hot-path **additional** memory footprint is bounded to ≤150 B/atom (SoA columns + adjacency + selection). Composite shadow is unchanged; total memory does not decrease in v2.1.
4. Selection/Expression queries are compiled and cached. Repeated queries no longer re-walk a heap-allocated AST per atom.
5. No file-format break.
6. Python bindings continue to work for the v2.0 API surface; new bindings added for MoleculeStore, AtomView, CompiledExpression.
7. **Composite remains the source of truth for structure** in v2.1. MoleculeStore is a snapshot. Structural mutations happen through Composite; the store re-syncs.

### Non-Goals (deferred to v2.2 or v3.0)
1. Removing Composite as Atom's base class (v3.0 — requires file-format + ABI break).
2. Full hierarchical SoA for proteins / NucleicAcid / SecondaryStructure / Fragment.
3. Rewriting VIEW (opportunistic SoA reads in vertex-buffer fill only).
4. New binary persistence format.
5. GPU-resident molecular data.
6. Thread-safe Composite *mutation*. v2.1 = thread-compatible snapshots only.
7. Incremental delta sync. v2.1 = full re-sync on detected change. Delta sync is v2.1.1 experimental.

---

## 3. Target Architecture

### 3.1 Two-tier model with external registry

```
   ┌──────────────────────────────────────────────────────┐
   │  Composite tree (UNCHANGED in v2.1 — preserves ABI)  │
   │  Atom : Composite + PropertyManager                  │
   └──────────────────────────┬───────────────────────────┘
                              │ snapshot / writeback (out-of-band)
   ┌──────────────────────────▼───────────────────────────┐
   │  MoleculeStoreRegistry — external, keyed by System*  │
   │   ↓                                                  │
   │  MoleculeStore (SoA columns, CSR bond adjacency,     │
   │  selection bitmap, stable IDs, string pool,          │
   │  generation counter)                                  │
   └──────────────────────────────────────────────────────┘
                              │
              ┌───────────────┴────────────────┐
              │                                │
   ┌──────────▼────────┐          ┌────────────▼─────────┐
   │ AtomView (NEW)    │          │ Hot-path SoA spans   │
   │ value-type handle │          │ (force-field inner   │
   │ generation-checked│          │  loops, batched      │
   │                   │          │  selection)          │
   └───────────────────┘          └──────────────────────┘
```

**Critical design choice:** sync state lives in `MoleculeStoreRegistry` — an external `std::unordered_map<System*, std::unique_ptr<MoleculeStore>>`. **No new fields, vtables, or `friend class` semantics are added to Atom, Bond, Composite, AtomContainer, or PropertyManager.** This preserves binary ABI.

The cost of this choice is that Composite mutations cannot proactively notify the store. The plan addresses this with three mechanisms, in order of strength:

1. **`Composite::modification_stamp_` and `selection_stamp_` observation** ([composite.h:445](include/BALL/CONCEPT/composite.h#L445), [composite.C:325](source/CONCEPT/composite.C#L325)) — propagated to root on every structural mutation, the store observes these.
2. **A mandatory `markDirty(System&)` API** that mutators bypassing the stamp (e.g., direct `Atom::setPosition` in tight loops, minimizer `const_cast<AtomVector&>` patterns) must call.
3. **The Mutation Contract Table (§5.1)** — exhaustive catalogue of every Composite/Atom/Bond mutation path; required dirty action; test that verifies it.

### 3.2 MoleculeStore data layout

```cpp
struct MoleculeStore {
  // Per-atom parallel columns (SoA), all sized to n_atoms
  std::vector<Vector3>  positions;        // 24 B
  std::vector<Vector3>  velocities;       // 24 B
  std::vector<Vector3>  forces;           // 24 B
  std::vector<float>    charges;          // 4 B
  std::vector<uint8_t>  element;          // 1 B
  std::vector<int8_t>   formal_charge;    // 1 B
  std::vector<uint8_t>  selection;        // 1 B (bit-packed later)
  std::vector<uint32_t> residue_idx;      // 4 B
  std::vector<uint32_t> name_offset;      // 4 B (into string_pool)
  std::vector<uint64_t> stable_id;        // 8 B
  std::vector<Atom*>    back_ptr;         // 8 B — handle→Composite

  std::string string_pool;

  struct BondRecord { uint32_t a, b; uint8_t order, type; uint16_t flags; };
  std::vector<BondRecord> bonds;
  std::vector<Bond*>      bond_back_ptr;

  std::vector<uint32_t> bond_csr_off;
  std::vector<uint32_t> bond_csr_idx;

  std::vector<uint32_t> residue_csr_off;
  std::vector<uint32_t> residue_csr_idx;

  std::unordered_map<uint64_t, NamedProps> custom_props;

  std::shared_mutex     rw_mutex;
  uint64_t              generation = 0;
  PreciseTime           last_observed_mod_stamp;
  PreciseTime           last_observed_sel_stamp;
};
```

**Hot-path additional memory budget:** ~103 B/atom (columns) + 16–24 B/atom (bond CSR). Within the 150 B/atom budget.

### 3.3 Handle / View layer

```cpp
class AtomView {
  MoleculeStore* store_;
  uint32_t       idx_;
  uint64_t       generation_;
public:
  Vector3 getPosition() const;
  void    setPosition(Vector3 const& p);
  float   getCharge() const;
  String  getName() const;
  BondViewRange bonds() const;
  bool isValid() const noexcept { return store_ && store_->generation == generation_; }
  Atom* asComposite() const { return store_->back_ptr[idx_]; }
};
```

AtomView is a **new type for new code**. Old `Atom*` call sites continue to use the real `Atom`.

### 3.4 Compiled expression / selection

```cpp
struct ElementEq   { uint8_t element; };
struct ChargeRange { float lo, hi; };
struct UserPred    { std::function<bool(MoleculeStore const&, uint32_t)> fn; };
struct AndNode; struct OrNode; struct NotNode;
using Pred = std::variant<ElementEq, ChargeRange, UserPred,
                          std::unique_ptr<AndNode>,
                          std::unique_ptr<OrNode>,
                          std::unique_ptr<NotNode>>;
struct AndNode { std::vector<Pred> ops; };
struct OrNode  { std::vector<Pred> ops; };
struct NotNode { Pred op; };

class CompiledExpression {
  Pred root_;
public:
  std::vector<uint8_t> evaluate(MoleculeStore const&) const;
};
```

`UserPred` preserves exact semantics for `Expression::registerPredicate`-registered custom predicates.

---

## 4. Migration Strategy — Six Phases

### Phase K0: Foundation + Validation Infrastructure (10–14 weeks)

**Goal:** MoleculeStore exists; sync is correct on the full FORMAT-driven regression corpus; STRUCTURE-generated systems round-trip; Python builds.

**Deliverables:**
- `BALL/KERNEL/MoleculeStore.{h,C}` + `MoleculeStoreRegistry`
- `MoleculeStore::syncFrom(System const&)` (full sync, idempotent)
- `MoleculeStore::writeBackTo(System&) const`
- **Mutation Contract Table (§5.1) fully populated** — exhaustive audit of every Composite/Atom/Bond mutation entry point with its stamp behavior, required dirty action, owner, and CI test
- **Pointer-fixup validation for bonds** (dangling, duplicate, intermolecular semantics)
- **STRUCTURE round-trip validation corpus**: 20+ systems produced by bond perception, ring perception, builders, protonation processors
- **FORMAT sync stub (test scaffolding only)**: `PDBFile::read(System&)` opportunistically syncs the store after load. Single PDB reader only — production-wide FORMAT migration is K3 item #1.
- **Python CI baseline** (build + import + v2.0 test corpus on every commit touching KERNEL)
- Unit + integration tests: round-trip identity for 30+ systems (small ligand, 1FAT, nucleic acid, multi-chain complex, STRUCTURE-built molecules, edge cases, malformed-but-tolerated PDB)
- Benchmark harness
- **CI gates added:** ABI diff (Linux libabigail + macOS nm-diff + Windows dumpbin), plugin load fixture (per §6), ASAN/UBSAN

**Exit criteria:**
1. Builds on macOS arm64 / Linux x86_64 / Windows MSVC.
2. Round-trip preserves: positions, velocities, forces, charges, names, element, formal_charge, bonds (a/b/order/type/flags), residue assignment, intermolecular vs intramolecular classification, stable Atom* identity.
3. STRUCTURE corpus round-trips without semantic loss.
4. FORMAT sync stub adds < 1% overhead on 100k-atom PDB read.
5. Python CI smoke green.
6. ABI diff gates green on all three OSes (no incompatible changes to existing KERNEL+CONCEPT exports; new symbols allowlisted).
7. Mutation Contract Table covers ≥95% of grep'd mutation entry points.
8. Benchmark: full re-sync amortizes within 5 force-field steps on a 100k-atom system.

**Effort: 10–14 weeks, 1.0 FTE.**

---

### Phase K1: Hot-path opt-in — MOLMEC nonbonded (12–20 weeks)

**Goal:** Force-field nonbonded kernels (Coulomb + vdW) for AMBER, CHARMM, MMFF94 use SoA inner loops with snapshot/flush bracketing.

**Deliverables:**
- `BALL/MOLMEC/COMMON/SoAView.{h,C}` — custom `Span<T>` (C++17, since `std::span` is C++20), generation-checked
- `ForceField::sync()` / `ForceField::flush()` brackets
- Port nonbonded:
  - `MMFF94NonBonded` Coulomb + vdW ([MMFF94NonBonded.C:151–177](source/MOLMEC/MMFF94/MMFF94NonBonded.C#L151))
  - `AmberNonBonded`
  - `CharmmNonBonded` ([charmmNonBonded.C:451](source/MOLMEC/CHARMM/charmmNonBonded.C#L451))
- **Snapshot-with-pair-list integration**: pair lists, exclusion lists, 1-4 scaling tables use MoleculeStore indices instead of `Atom*` pairs
- **Minimizer/MD compatibility shim**: the 11 files using `const_cast<AtomVector&>(getAtoms())` ([strangLBFGS.C:279](source/MOLMEC/MINIMIZATION/strangLBFGS.C#L279), [conjugateGradient.C:760](source/MOLMEC/MINIMIZATION/conjugateGradient.C#L760), etc.) are wrapped with `markDirty()` per the Mutation Contract Table
- **Per-force-field rollback flags**: `BALL_DISABLE_SOA_MMFF94=1` etc. disables SoA path at runtime
- Benchmark report on: 1FAT (~1k), 1AKE+solvent (~25k), DPPC membrane (~100k)

**Exit criteria:**
1. Ported nonbonded kernels reproduce legacy results within 1e-6 relative tolerance.
2. SoA Coulomb ≥2× speedup on 100k-atom benchmark; vdW ≥1.5×.
3. 1FAT energy minimization converges to identical geometry within numerical tolerance.
4. MD on 25k-atom water box: 1000 steps, energy drift ≤ legacy drift.
5. Per-FF rollback flags work.
6. ABI diff gate still green.
7. Mutation Contract Table: 100% coverage on minimizer/MD paths.

**Effort: 12–20 weeks, 1.0 FTE.**

---

### Phase K2: Selection & Expression compilation (4–6 weeks)

**Goal:** `Expression`/`Selector` use compiled cached predicates; parser is re-entrant; semantics exactly preserved.

**Deliverables:**
- `CompiledExpression` class (variant-based AST with boxed children — recursive `std::variant` requires boxing in C++17)
- `Expression::compile()` caches by `(expression_string, registered_predicate_set_hash)`
- `Selector::apply(System&)` uses compiled path when store attached; falls back to legacy `ExpressionTree::operator()(Atom&)` ([selector.C:84–94](source/KERNEL/selector.C#L84))
- `UserPred` fallback path for `Expression::registerPredicate`-registered custom predicates (preserves side effects)
- **Parser re-entrancy fix**: global `ExpressionParser::state` ([expressionParser.C:146–169](source/KERNEL/expressionParser.C#L146)) wrapped in mutex + thread-local re-entrant path
- **Selection mutation service** (`SelectionService`): a single non-ABI-breaking entry point for selection bitmap mutation. BALLView, Python bindings, and Selector route selection changes through this service; direct `Composite::select()` calls still work but produce a *stale* store bitmap until next `syncFrom`. The staleness contract is documented and asserted in debug.
- TSAN CI job exercising parallel parse + select

**Exit criteria:**
1. Compiled-expression selection returns bit-identical results to legacy on ≥100-expression corpus.
2. Repeated selection on 100k-atom system: ≥10× speedup.
3. TSAN clean on parallel parse+select test.
4. ABI diff gate still green.
5. SelectionService routes ≥90% of selection mutations in BALLView and Python paths.

**Effort: 4–6 weeks, 1.0 FTE.**

---

### Milestone: **v2.1-preview release** at end of K2

After K0+K1+K2: ship a `v2.1-preview` build that contains MoleculeStore + nonbonded SoA + compiled selection. This is a real release, intended for early adopters and benchmarking, NOT positioned as production v2.1. Full v2.1 requires K3 minimum + K4 (see §11).

---

### Phase K3: Subsystem rollout (9–15 months)

**Goal:** All non-VIEW dependent subsystems route hot paths through MoleculeStore. STRUCTURE last.

**Sequencing:**

| Order | Subsystem | LOC | Risk | Effort |
|-------|-----------|-----|------|--------|
| 1 | **FORMAT (production migration — all PDB/mol2/hin/BALL readers + writers + golden corpus)** | ~23k | Low | 4–6 weeks |
| 2 | **MOLMEC bonded** (AMBER bonded, CHARMM bonded, MMFF94 bonded, SOLVATION) | ~26k + 8k | Medium | 10–14 weeks |
| 3 | **SCORING / DOCKING** (already uses `vector<Atom*>` caches) | ~15k + 12k | Low–Medium | 8–10 weeks |
| 4 | **QSAR / NMR / XRAY / ENERGY** | ~30k | Low | 4–6 weeks |
| 5 | **STRUCTURE** (geometry, builders, bond perception, ring detection) | ~48k | High | 12–18 weeks |

**Distinction K0 FORMAT vs K3 FORMAT:**
- K0: single PDBFile sync stub for test scaffolding; not production-grade.
- K3: all FORMAT readers (PDB, mol2, hin, BALL native) and writers wired to the store; golden-file CI corpus expanded to 100+ files.

**Per-subsystem checklist:**
- Profile hot paths on v2.1 benchmark corpus.
- Replace `BALL_FOREACH_*` in hot paths with range-based loops over SoA; macros stay in cold paths.
- Wrap `dynamic_cast<Atom*>(composite)` with `isKindOf<Atom>` + `static_cast` where runtime type is constrained.
- Audit friend-class accesses (9 in KERNEL+CONCEPT).
- Add subsystem regression test exercising both legacy and SoA paths.

**Exit criteria:**
1. All listed subsystems compile and pass regression on tri-OS CI.
2. Composite tree traversal time ≤10% of total on 100k-atom benchmark (vs ≥40% baseline).
3. ABI diff gate still green.

**Effort: 9–15 months, 1.0 FTE + 0.5 FTE testing.**

---

### Phase K4: Python bindings + VIEW integration (3–4 months, overlaps with late K3)

**Goal:** Python users see no break; MoleculeStore bindings added; VIEW reads positions from store; selection bidirectional sync via SelectionService.

**Timing:** K4 starts when K3 items 1–3 (FORMAT, MOLMEC bonded, SCORING/DOCKING) complete. **K4 overlaps with K3 items 4–5 (QSAR/NMR/XRAY/ENERGY, STRUCTURE)**, shaving 3–4 months off the sequential total.

**Deliverables:**
- Python bindings for `MoleculeStore`, `AtomView`, `CompiledExpression` (SIP or pybind11 per v1.6 Phase 6 decision)
- Existing `Atom`, `Bond`, `Residue` bindings preserved
- `View::Scene` vertex-buffer fill reads from MoleculeStore positions when present
- BALLView selection: click → `SelectionService::select()` → both Composite and store updated atomically (no ABI break since SelectionService is new)
- Migration guide: `KERNELV2-MIGRATION.md` for downstream plugin authors
- Plugin compatibility fixture (per §6)

**Exit criteria:**
1. Python test corpus runs unchanged.
2. BALLView renders 100k-atom system at ≥30 fps on reference GPU.
3. v2.0-compiled plugin loads against v2.1 on tri-OS.
4. Migration guide reviewed by ≥1 external plugin maintainer.

**Effort: 3–4 months, 1.0 FTE, K3-overlap-capable.**

---

### Phase K5: Store-as-source-of-truth (DEFERRED to v3.0)

Codex confirmed: realistic effort is 12–18 months **on top of** K0–K4. Requires file-format migration and plugin ABI break. v2.1 ships without this.

---

## 5. Cross-Cutting Concerns

### 5.1 Mutation Contract Table — the architectural keystone

The external-registry architecture is **provably coherent only if every mutation that affects store-mirrored state either advances a Composite stamp the store observes OR calls `markDirty()`**. This table is the K0 deliverable that proves it.

**Format (to be filled in K0; partial illustrative entries below):**

| Operation | Source location | Existing stamp? | Required dirty action | Owner | CI test |
|---|---|---|---|---|---|
| `Composite::appendChild` | [composite.C:630](source/CONCEPT/composite.C#L630) | `stamp(BOTH)` propagated to root | None (store observes) | KERNEL | `test_sync_appendChild` |
| `Composite::removeChild` | [composite.C:1098](source/CONCEPT/composite.C#L1098) | `stamp(BOTH)` propagated | None (store observes) | KERNEL | `test_sync_removeChild` |
| `Composite::splice*` | [composite.h:516](include/BALL/CONCEPT/composite.h#L516) | `stamp(BOTH)` propagated | None | KERNEL | `test_sync_splice` |
| `Composite::select()` | [composite.h:598](include/BALL/CONCEPT/composite.h#L598) | `stamp(SELECTION)` propagated | None — store selection stale until next sync | KERNEL | `test_selection_stale_after_direct_select` |
| `Atom::setPosition` | atom.iC inline | **NO STAMP** — hot path | **Caller MUST call `markDirty(POSITIONS)`** OR use `AtomView::setPosition()` | MOLMEC, STRUCTURE | `test_setPosition_dirty_required` |
| `Atom::setForce` | atom.iC inline | **NO STAMP** | **Caller MUST call `markDirty(FORCES)`** OR use `AtomView::setForce()` | MOLMEC | `test_setForce_dirty_required` |
| `Atom::setCharge` | atom.iC inline | **NO STAMP** | **Caller MUST call `markDirty(CHARGES)`** | FORMAT, STRUCTURE | `test_setCharge_dirty_required` |
| `Bond::create` | bond.C | **TBD — audit in K0** | TBD | KERNEL | `test_sync_bondCreate` |
| Minimizer `const_cast<AtomVector&>(...).positions = ...` | 11 files, e.g. [strangLBFGS.C:279](source/MOLMEC/MINIMIZATION/strangLBFGS.C#L279) | **NO STAMP** | **Force-field's `flush()` calls `markDirty(BOTH)` automatically** | MOLMEC | `test_minimizer_post_step_sync` |
| `PropertyManager::setProperty` | property.h | **NO STAMP** | **Caller MUST `markDirty(PROPERTIES)`** for properties promoted to columns | All | `test_property_dirty_required` |
| `AtomContainer::insert` | [atomContainer.C:276](source/KERNEL/atomContainer.C#L276) | Calls `Composite::appendChild` → stamps | None | KERNEL | `test_sync_atomContainer_insert` |

**K0 deliverable:** complete this table for ≥95% of grep'd mutation entry points and back every row with a CI test. The architecture is not approved until this table is filled and green.

### 5.2 Snapshot policy

**v2.1 default: full snapshot on detected change.**
- `MoleculeStore::syncFrom(System)` is full re-population on:
  - Detected advance of `Composite::modification_stamp_` (structural)
  - Detected advance of `Composite::selection_stamp_` (selection)
  - Explicit `markDirty()` call (positions / forces / charges / properties)
- Snapshot is owned by the registry; ForceField/Selector borrow read-only references during their operation.
- Position/force write-back is explicit (`ForceField::flush()` call).
- **Incremental delta-sync is v2.1.1 experimental** behind a flag.

### 5.3 Thread safety

**v2.1 contract: "thread-compatible snapshots; no concurrent Composite mutation."**
- MoleculeStore exposes `std::shared_mutex` — many readers OK.
- Composite tree mutation is NOT thread-safe (unchanged from v2.0).
- Expression parser made re-entrant in K2.
- Selection bitmap mutation goes through SelectionService (K2 deliverable); direct `Composite::select()` calls leave the bitmap stale (asserted in debug, documented in user guide).

### 5.4 Persistence

- v2.1 writes the same Composite-based persistence format as v2.0. MoleculeStore is reconstructed via `syncFrom(System)` after load.
- **Persistence pointer-fixup validation** (K0) handles malformed-file tolerance, duplicate bonds, dangling bond pointers, intermolecular bond semantics.
- Golden-file CI: 50+ files in K0 → 100+ in K3 FORMAT.

### 5.5 ABI preservation strategy

- **No changes to Atom, Bond, Composite, AtomContainer, or PropertyManager**: no new fields, vtable entries, or `friend class` declarations.
- All new state in `MoleculeStoreRegistry`.
- **Friend-class audit (K0)**: 9 friend declarations in KERNEL+CONCEPT. Default is **friends stay** — removal is only attempted if proven source-only and non-ABI-visible (no new exported symbols, no vtable change, no layout change). Where removal is safe, accessors are added as non-exported `private static` helpers. Public exported accessors are NOT added. All decisions catalogued in `KERNELV2-ARCH.md`.
- Windows-specific: `std::vector<Atom*>` template export discipline ([atom.h:1009–1035](include/BALL/KERNEL/atom.h#L1009)). Any new exported template instantiation in MoleculeStore follows the same `BALL_EXPORT` pattern.

### 5.6 dynamic_cast removal

53 sites; per-site audit in K3. Mostly replaced with `isKindOf<>` + `static_cast`; some remain for safety-critical paths.

### 5.7 BALL_FOREACH_* macros

111 sites. Macros preserved. Equivalent inline range functions gain `[[deprecated]]` in v2.1. Macro removal is post-v2.1 work.

---

## 6. CI / Quality Gates

All gates must be GREEN before any phase merges to main.

| Gate | What it checks | Implementation | Phase |
|---|---|---|---|
| Build matrix | Compiles | macOS arm64 / Linux x86_64 / Windows MSVC | K0 |
| Unit tests | All v2.0 tests + new MoleculeStore tests | ctest | K0 |
| **ABI compatibility (multi-platform)** | No incompatible changes to existing KERNEL+CONCEPT exports; new symbols allowlisted | Linux: libabigail `abidiff` against v2.0 baseline + allowlist of new symbol names. macOS: `nm -gU` diff + Mach-O field comparison via custom script. Windows: `dumpbin /exports` diff + Microsoft `apivalidator` on the import library | K0 |
| **Plugin load fixture (cross-platform)** | v2.0-compiled plugin loads and runs against HEAD | Linux: `dlopen` + symbol resolve + factory call on real `Atom*`. macOS: same with `dlopen` from `.dylib`. Windows: explicit MSVC toolset alignment (vc143 lock), v2.0-compiled DLL using v2.0 `BALLConfig.cmake`, `LoadLibrary` + `GetProcAddress` for `extern "C"` factory symbol, real cross-DLL `Atom*` method calls (not just symbol presence) | K0 |
| **Python binding compile + import** | SIP/pybind compiles, Python imports BALL | CI script invokes Python | K0 |
| **Python test corpus** | All v2.0 Python tests pass | pytest | K0 |
| **ASAN / UBSAN** | Memory + UB clean | Clang sanitizer build | K0 |
| **TSAN** | Thread-sanitizer clean on parser, selection, FF workers | Clang TSAN build | K2 |
| **Persistence golden files** | 50+ files round-trip byte-equal | Custom diff harness | K0 |
| **FORMAT fuzz** | Random PDB mutations round-trip without crash | libFuzzer | K1 |
| **Benchmark thresholds** | No regression > 5% on baseline benchmarks | Custom harness on dedicated runner | K1 |
| **Per-FF rollback flags** | `BALL_DISABLE_SOA_*` switches still work | Smoke test job | K1 |
| **STRUCTURE corpus** | 20+ (K0) → 50+ (K3) STRUCTURE-generated systems round-trip | Custom suite | K0 |
| **Mutation Contract Table coverage** | Every row has a passing CI test | Required by table itself | K0 |

---

## 7. Effort & Timeline

| Phase | Scope | Calendar Time | Engineering Effort |
|---|---|---|---|
| K0 | Foundation + validation infra (incl. Mutation Contract Table, FORMAT stub, STRUCTURE corpus, Python CI, ABI tooling) | 10–14 weeks | 1.0 FTE |
| K1 | Hot-path nonbonded (MMFF94 + AMBER + CHARMM) + minimizer/MD shim | 12–20 weeks | 1.0 FTE |
| K2 | Selection / Expression compilation + parser re-entrancy + SelectionService | 4–6 weeks | 1.0 FTE |
| **v2.1-preview ship gate** | K0 + K1 + K2 sequential | **26–40 weeks (~8–14 months)** | — |
| K3 | Subsystem rollout (FORMAT → MOLMEC bonded → SCORING/DOCKING → QSAR/NMR/XRAY/ENERGY → STRUCTURE) | 9–15 months | 1.0 FTE + 0.5 FTE testing |
| K4 | Python bindings + VIEW integration (**overlaps last 3–4 months of K3**) | 3–4 months sequential / 0 added if fully overlapped | 1.0 FTE |
| **v2.1.0 ship gate** | K0–K2 + K3 minimum (FORMAT + MOLMEC bonded + SCORING) + K4 | **~16–24 months total** | — |
| **v2.1 full ship gate** | All of K0–K4 incl. STRUCTURE | **~22–34 months total** | ~1.5 FTE average |
| K5 | Store-as-source-of-truth (DEFERRED to v3.0) | +12–18 months | 2.0 FTE |

**Honest reading of the totals (per Codex Round 2):**
- Sequential K0+K1+K2 = 26–40 weeks ≈ 6–10 months base; **add 25–40% integration drag** → 8–14 months for v2.1-preview.
- K3 + (K4 partially overlapped) = 9–15 months + 1–2 months tail.
- Best case: 16 months. Realistic: 22 months. Worst case: 34 months.
- **Assuming sub-1.0 FTE (the realistic case for a community-maintained project): all timelines multiply by `1/(actual FTE)`.**

---

## 8. Risk Register

| ID | Risk | Likelihood | Impact | Mitigation |
|---|---|---|---|---|
| R1 | Sync overhead dominates on small systems | Med | High | Skip sync if `modification_stamp_` unchanged; per-FF rollback flag |
| R2 | Numerical regressions from SIMD reduction order | Med | High | Kahan summation in regression mode; CI has both paths |
| R3 | STRUCTURE bond/ring perception breaks under SoA | Med | High | Port STRUCTURE last; corpus in K0 catches regressions early |
| R4 | Friend-class refactor blocks separation | Med | Med | Audit in K0; source-only fixes documented in KERNELV2-ARCH.md |
| R5 | Plugin ABI inadvertently broken | Med | Critical | Multi-platform ABI gate + plugin load fixture from K0 |
| R6 | BALLView selection bidirectional sync introduces lag | Low | Med | Profile in K4; SelectionService async-update fallback |
| R7 | Python binding generator decision (v1.6 Phase 6) lands late | Med | Med | K4 starts only after Phase 6 decision committed |
| R8 | Engineering capacity < 1.0 FTE drags timeline | High | High | Honest scheduling; ship v2.1-preview after K2 even if K3 slips |
| R9 | MoleculeStore design wrong for some workflow (e.g., MD trajectory streaming) | Low | High | K0 includes external user review |
| **R10** | **Store/Composite divergence after un-stamped mutations** (setPosition, setForce, setCharge, Bond::create) | **High** | **High** | **Mutation Contract Table (§5.1) is gating: every entry has a CI test; markDirty() is mandatory; failure to call it is caught in debug builds via stamp-vs-store-state assertion** |
| **R11** | Windows DLL/template export instability ([atom.h:1009–1035](include/BALL/KERNEL/atom.h#L1009)) | Med | High | MoleculeStore template instantiations follow `BALL_EXPORT` discipline; Windows ABI gate from K0 uses `dumpbin /exports` diff |
| **R12** | Minimizer/MD `const_cast<AtomVector&>` pattern violates snapshot contract | High | High | K1 deliverable: wrap all 11 files in compatibility shim; `ForceField::flush()` calls `markDirty(BOTH)` automatically; Mutation Contract row + CI test |
| **R13** | Custom user-registered predicates lose semantics under CompiledExpression | Med | Med | `UserPred` boxed `std::function` fallback preserves all behavior |
| **R14** | Parser global state is a real reentrancy bug | High | Med | K2 deliverable: mutex + thread-local path; TSAN gate |
| **R15** | Persistence pointer-fixup edge cases | Med | High | K0 deliverable: pointer-fixup validator + golden-file corpus with malformed cases |
| **R16** | **Selection bitmap goes stale after direct `Composite::select()` outside SelectionService** | **High** | **Med** | **Documented stale-until-resync contract; debug-build assertion; SelectionService funnel for all new selection mutation sites** |

---

## 9. Open Questions

1. **String type:** keep `BALL::String` externally; use `std::string` internally for string pool.
2. **`stable_id` derivation:** runtime-only `(System*) ^ atom_index ^ generation_seed`; not persisted.
3. **PropertyManager flattening:** promote properties with >5% atom coverage to typed columns.
4. **Custom allocator for MoleculeStore columns:** out of scope for v2.1; revisit in v2.2.
5. **MD trajectory storage:** out of scope; MoleculeStore = single frame.
6. **K3 parallelization:** if engineering capacity allows, FORMAT/MOLMEC/SCORING ports can run in parallel — reduces calendar time but not effort.
7. **v2.1.0 release gate:** is K3 partial (FORMAT + MOLMEC + SCORING done, others pending) acceptable for production v2.1.0? **Recommendation: yes; remaining subsystems ship in v2.1.1+.**

---

## 10. Out-of-scope (explicit)

- Removing `friend class` declarations entirely.
- Migrating BALL::String → std::string.
- Replacing Vector3 → Eigen::Vector3f.
- Custom allocator for MoleculeStore.
- Multi-trajectory storage.
- File-format v2.
- Removing Composite from Atom inheritance (v3.0).
- Concurrent Composite mutation.
- Incremental delta sync (v2.1.1 experimental).

---

## 11. Release Gates

### v2.1-preview (after K0+K1+K2)
Ships when ALL of:
1. K0, K1, K2 exit criteria met.
2. ABI diff gate green on tri-OS.
3. Mutation Contract Table 100% covered.
4. v2.1-preview labeled as **technology preview** in release notes; not production-recommended.

### v2.1.0 (production)
Ships when ALL of:
1. v2.1-preview gates met.
2. K3 minimum complete: **FORMAT + MOLMEC bonded + SCORING + DOCKING** (SCORING and DOCKING are bundled as K3 item 3 and ship together; see §4 K3 table).
3. K4 complete (Python bindings + VIEW integration).
4. Reference benchmark: AMBER energy + gradient on 100k-atom system runs ≥2× faster than v2.0.
5. All v2.0 regression tests pass on tri-OS CI.
6. No file-format break.
7. Migration guide published.

### v2.1.1+
Remaining K3 subsystems (QSAR, NMR, XRAY, ENERGY, then STRUCTURE) ship as point releases.

---

## Appendix A: Codebase Migration Surface

| Metric | Count | Source |
|---|---|---|
| `Atom*` / `Atom&` references | 2,463 | grep |
| `BALL_FOREACH_*` macro uses | 111 | grep |
| `dynamic_cast<KernelType*>` invocations | 53 | grep |
| `friend class` in KERNEL+CONCEPT | 9 | grep |
| KERNEL header files | ~40 | ls |
| KERNEL source files | ~28 | ls |
| Dependent subsystem LOC (VIEW+STRUCTURE+MOLMEC+FORMAT+QSAR+SCORING+DOCKING) | ~207k | wc -l |
| Python binding files | 12 | find source/PYTHON |
| Minimizer/MD files using `const_cast<AtomVector&>` | 11 | grep |

---

## Appendix B: Review History

| Round | Reviewer | Findings | Resolution |
|---|---|---|---|
| Initial drafts | Self (with Explore agent) | Architectural sketch, optimistic effort estimates | Drafted KERNELV2 v1 |
| Round 1 | Codex CLI (codex exec) | Underestimated effort, missing CI gates, ABI break risks, FORMAT misordered, Python CI too late, missing mutation hooks, missing R10–R15 | Wrote v2 incorporating all corrections |
| Round 2 | Codex CLI (codex exec) | Selection sync contradiction, ABI gate wording, v2.1 naming inconsistency, missing Mutation Contract Table, plugin fixture cross-platform underspecified, timeline math loose | Wrote v3 (this document) with §5.1 Mutation Contract Table, SelectionService, multi-platform ABI gate, v2.1-preview milestone, explicit K3/K4 overlap |

---

*End of KERNELV2 v3. Approved planning baseline pending implementation kickoff.*
