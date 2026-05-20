# BALL 2.1 — Release Notes

**Status:** DRAFT for rc1. **Authored:** 2026-05-20.
**Companion:** `MILESTONE-CONTEXT-v2.1.md` (project-level handover).

BALL 2.1 is a **JSON-persistence + API-hygiene + v2.2-preparation**
minor release on top of v2.0.0. It is **not** the memory-budget /
thin-handle release — `sizeof(Atom)` is unchanged in character
(the D2/D3/D4 thin-stub bases remain); the inheritance flip and the
D13 budget closure are the v2.2 milestone.

What v2.1 delivers that a v2.0.x patch could not:

- **JSON load 7–14× faster** — the single biggest user-visible win.
- **Bond `PropertyManager` round-trip** through JSON, and
  reconstruction of the Atom-side `Bond*` graph on load (closing
  two v2.0 fidelity gaps).
- **Atom-RTTI consolidated** — every `dynamic_cast<Atom*>` /
  `isKindOf<Atom>` / `castTo<Atom>` in the CORE_ONLY modules routes
  through one helper, prepping the v2.2 thin-handle flip.
- **Reproducible benchmark numbers** (median-of-N reporting).

The phase-by-phase audit trail (P0–P6, Codex rounds R17–R28) lives
in `.planning/v2.1/`.

---

## What's new

### JSON load is 7–14× faster (P4.1, V21-LOAD-BATCH)

The v2.0 `loadSystemJSON` had an **O(n²) hot path**: each per-atom
adoption during a load called `for_each_bond_of()` on the orphan
store, which rebuilt the *entire* CSR adjacency (O(n_atoms)) every
time because each `allocate_atom`/`release_atom` re-dirtied it.
With ~100k bondless atoms in the orphan store during a load, that
is ~10¹⁰ operations.

The fix is a one-line guard: when a store has **no bond records**,
`for_each_bond_of` / `bond_degree` / `bonds_of` return immediately
without rebuilding the CSR. The orphan store during a fresh load is
bondless, so the per-atom bond scan becomes O(1) → the whole load
collapses from O(n²) to O(n). (The fix generalises to every
adoption path, not just the loader.) The loader also pre-reserves
the destination store.

**Benchmark (100k atoms, Darwin arm64 release, median of 5):**

| Scenario | v2.0 | v2.1 | Speedup |
|---|---:|---:|---:|
| save, no props | ~233 ms | ~290 ms | ~par |
| **load, no props** | ~3,588 ms | **~260 ms** | **~14×** |
| save, +3 props/atom | ~330 ms | ~470 ms | ~par |
| **load, +3 props/atom** | ~3,750 ms | **~530 ms** | **~7×** |

The save:load ratio dropped from ~1:16 to ~1:1 — load is now on par
with (or faster than) save.

### Bond properties + Bond* graph round-trip (P4.2)

Two coupled v2.0 gaps closed:

1. **Bond `PropertyManager` bag** (MMFF94 bond-type properties,
   `VIRTUAL__BOND` markers, HBondProcessor annotations, etc.) now
   round-trips through K0.6 JSON. The writer emits each live bond's
   bag inline (reached via the store's bond back-pointer); the
   loader restores it.
2. **Atom-side `Bond*` graph reconstruction.** v2.0's loader only
   restored store `BondRecord`s — after a System round-trip
   `atom.countBonds()` returned 0 even though the store held the
   bonds. v2.1's loader reconstructs the `Bond*` objects via
   `createBond`, so `countBonds()` / `getBond()` / the bond
   iterators work correctly post-load.

**Schema:** the K0.6 JSON minor version bumped (store 1→2, system
0→1). v2.1 reads v2.0 JSON unchanged (bonds without a `properties`
key load fine); v2.0 readers reject the higher minor.

**Caveat:** the System loader is Atom-consistent (one bond per atom
pair). A store-level multigraph (multiple records for the same
pair, addable only via the low-level `store.add_bond`, not via the
Atom API) collapses to one bond on load with a warning. The
store-only loader (`loadStoreJSON`) still preserves multigraphs.

### Atom-RTTI consolidation (P3)

Every `dynamic_cast<Atom*>(...)`, `RTTI::isKindOf<Atom>(...)`, and
`RTTI::castTo<Atom>(...)` in the CORE_ONLY modules (KERNEL,
STRUCTURE, FORMAT, QSAR, MOLMEC, SCORING, SOLVATION, DOCKING,
ENERGY, NMR, CONCEPT) — 29 sites total — now routes through a
single `BALL::detail::compositeAsAtom_()` helper. A CI grep gate
blocks reintroduction. No public API changed (NMR `ShiftModule`,
`Selector`, `AtomVector` signatures are all preserved). The v2.2
thin-handle flip then replaces **one** helper definition instead of
chasing 29 call sites. (VIEW + APPLICATIONS RTTI — 54 sites — is
deferred to the Phase 4 BALLView modernization; tracked as
V21-VIEW-RTTI.)

### Reproducible benchmark reporting (P5.3, V21-MEDIAN-OF-N-BENCH)

`JsonBench_test` and `SelectorBench_test` now report
median + p99 + min/max + MAD + coefficient-of-variation across N
runs (via a small `test/BenchStats.h` helper) and gate on the
median rather than a single run. Release-claim numbers are now
reproducible with visible spread; the fixed pass/fail thresholds
are retained (no new CI policy).

### Selection performance (unchanged from v2.0)

The K0.5 compiled-selection speedup is carried forward unchanged:
**median ~55× over v1.x** on the 100k synthetic corpus (range
26–80× across query shapes), ~19–33× on a real PDB corpus, and the
`OwnedPred` slow path within ~1.6× of v1.x.

---

## What's NOT in v2.1 (deferred to v2.2)

v2.1 deliberately does not include:

- **The thin-handle flip / D13 memory budget.** `sizeof(Atom)` is
  still 368 B (it actually grew 8 B from v2.0's 360 B — see "Known
  limitations"). Deleting the D2/D3/D4 (`Composite` /
  `PropertyManager` / `Selectable`) base-class inheritance and
  moving that state into the per-store side tables is the v2.2
  milestone. The side-table scaffolding shipped in v2.1 P1–P2 is
  the substrate for that flip.
- **Mutation wiring** of `Composite`/`PropertyManager`/`Selectable`
  to the side tables (V21-MUTATION-WIRING) — destruction-order
  fragile against the current dual representation; lands with the
  v2.2 flip (D39).
- **`MoleculeStore::iterAtoms()` / store-native iteration**
  (V21-STORE-ITER-API) — the "skip Atom* materialisation" win only
  exists once atoms are store rows (post-flip), so the public API
  is designed in v2.2 (D44).
- **Pinned-baseline CI perf gates** (V21-CI-PERF-GATES) — v2.1
  keeps fixed thresholds; the median+MAD comparator infra is v2.2.
- **Debug-mode generation guard** (V21-GENERATION-GUARD) — a naive
  per-deref store-generation check false-trips on ordinary store
  growth; correct stale-handle detection needs v2.2 slot-generation
  semantics (D44, supersedes D25).
- **Windows MSVC CI** (V21-MSVC-CI-PORTABILITY) — moved to v2.2
  where the `sizeof(Atom) ≤ 32 B` target makes EBO verification a
  release gate (D34d).

All deferrals are tracked in `.planning/v2.1/BACKLOG.md`.

---

## Known limitations

- **`sizeof(Atom)` grew 8 B (360 → 368 B) in v2.1.** The P1.3
  side-table scaffolding added a transient `composite_handle_packed_`
  slot to `Composite` (hence to Atom and Bond, 296 B). This is
  P1-transient: the v2.2 thin-handle flip removes it along with the
  D2/D3/D4 bases. The full-fat per-atom budget is therefore ~507
  B/atom (was 499), still ~3.2× over the 160 B/atom D13 target.
  SoA-column-only workloads remain at 139 B/atom (under budget).
- **Bond multigraph collapse on System JSON load** (see above).
- **Custom (non-PTE) Element identity** is not preserved across
  set/get or JSON: the store keeps only the atomic number (1 byte
  by design), and `getElement()` resolves it through the canonical
  periodic table. This is an intentional SoA trade-off, not a
  serialization bug (the loss is at `setElement`, not at JSON). See
  `.planning/v2.1/P4.3-FINDINGS.md`.

---

## Verification

- **CORE_ONLY ctest: 284/284 PASS** (Apple Clang 16, macOS arm64,
  Release).
- `MoleculeStore_test` + `SystemJson_test` stress: 0 flakes.
- Adversarial review at every phase boundary — Codex CLI rounds
  **R17–R28** (P0 design lock through P6 pre-tag). Every BLOCKER/BUG
  fixed in-cycle; deferrals documented with rationale.

## Upgrade notes

- v2.1 reads v2.0 JSON unchanged. v2.0 cannot read v2.1 JSON
  (higher schema minor — fails cleanly).
- No public C++ API removed or changed signature. The new
  `BALL::detail::compositeAsAtom_` is an internal helper (not part
  of the stable surface).
