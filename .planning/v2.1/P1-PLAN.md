# v2.1-P1 — Side-Table Infrastructure: PLAN

**Phase:** P1 (the wedge between v2.0 Atom-as-Composite and v2.1 thin handle).
**Status:** Planning.
**Authored:** 2026-05-19 (immediately post-R17 fix commit).
**Decisions consumed:** D22a, D23a, D24a, D26a, D30a, D31, D34.

## Goal

Extend `MoleculeStore` with three new SoA segments — populated and
maintained in parallel with the v2.0 inline `Composite` /
`PropertyManager` / `Selectable` state — so that P2 can flip
inheritance without changing storage shape.

**Atom/Composite/PropertyManager/Selectable continue to inherit
exactly as in v2.0.** No public-API surface change in P1. No
performance regression target met; just parity + correctness.

## Out of scope for P1

- Deleting D2/D3/D4 inheritance from Atom/Bond (P2/P3).
- Reading from side tables instead of inline state (P2).
- Bond property columns (P3; bond identity unresolved per R17 P17-6).
- JSON load batching (P4).
- Generation-guard debug-mode macro (P5).
- New store-native iteration API (P5).

## Phase gate (success criteria)

1. `MoleculeStore` carries:
   - `composite_nodes_` (vector of `CompositeNode`, 5-link per D22a)
   - `property_columns_` (registry + dense columns + sparse bag per D23a)
   - `selected_bits_` (vector of `std::atomic<uint64_t>` per D24a)
2. Every public Composite mutation API (`appendChild`,
   `removeChild`, `insertBefore`, `insertAfter`, `clear`,
   `destroyChildren`, all internal `splice_*_` helpers, etc.)
   writes BOTH to the inline pointers AND to `composite_nodes_`.
3. Every public PropertyManager mutation API (`setProperty`,
   `clearProperty`, `clear`, `swap`, `set`, `operator=`) writes
   BOTH to the inline `properties_` vector AND to the registry +
   columns + sparse bag.
4. Every public Selectable mutation (`select`, `deselect`,
   `setSelected(bool)`, internal `select_`, `deselect_`) writes
   BOTH to the inline `selected_` flag AND to `selected_bits_`.
5. New test `SideTableParity_test` runs typical workloads
   (Composite tree mutations, force-field property assignment,
   selection toggle) and asserts side tables match inline state
   exactly after each operation.
6. All 282 v2.0 CORE_ONLY tests still PASS (zero regression).
7. `MoleculeStore_test` 100-run stress: 0/100 flakes.
8. `BALL_EMPTY_BASES` macro defined and applied to Atom + Bond
   (preparation only; P2 actually exercises it).
9. `CompositeIteratorStability_test` lands and passes (D31).

## Phase budget

- Side-table memory: 4.8 MB (composite_nodes_) + ~5 MB
  (property_columns_ for 10 well-known FF columns at 100k atoms)
  + 12.5 KB (selected_bits_) = **~9.8 MB added per 100k-atom
  System**. Acceptable — System total still under v1.x baseline.
- Build time: +~3 sub-files; expected +~1 min compile.
- Test time: +~2 s for SideTableParity_test, +~1 s for
  CompositeIteratorStability_test.

## Sub-phases

P1 splits into 9 atomic commits, executed sequentially:

| # | Subject | Deliverable | Gate |
|---|---|---|---|
| **P1.1** | `BALL_EMPTY_BASES` macro | `include/BALL/COMMON/macros.h` | macro compiles on Clang; MSVC verification deferred to D34 CI |
| **P1.2** | `CompositeHandle` + `CompositeNode` types + `composite_nodes_` storage | `include/BALL/KERNEL/moleculeStore.h`, `source/KERNEL/moleculeStore.C` (allocator + release + handle pool) | unit test creates / frees N nodes, asserts vector growth & handle stability |
| **P1.3** | Wire `Composite` mutations to write `composite_nodes_` | `source/CONCEPT/composite.C` (every mutation path adds side-table update under a `// V2.1 P1: side-table mirror` comment) | full ctest passes; `SideTableParity_test` (skeleton) asserts post-mutation parity for `appendChild` + `removeChild` |
| **P1.4** | `PropertyColumnRegistry` + `PropertyColumn<T>` + well-known FF column predeclaration | `include/BALL/KERNEL/propertyColumns.h` + `.C`; predeclares PARTIAL_CHARGE / FORMAL_CHARGE / MMFF94_TYPE / AMBER_TYPE / RADIUS / EPSILON / HYBRIDIZATION / IS_AROMATIC / ATOM_TYPE_NAME / STEREO_DESCRIPTOR | unit test registers + sets + gets values across types; 256-col cap throws on overflow |
| **P1.5** | Sparse fallback `HashMap<atom_idx, PropertyBag>` + dynamic-column promotion logic at `compact()` | `propertyColumns.C` (`promote_if_full_()` called from `MoleculeStore::compact`) | unit test fills sparse bag to ≥10%, asserts promotion to dense column on next compact() |
| **P1.6** | Wire `PropertyManager` mutations to side tables | `source/CONCEPT/property.C` (every mutation path adds side-table update); v2.0 demux preserved (mismatched-type stays in sparse bag, no throw) | `SideTableParity_test` asserts post-setProperty parity for INT/FLOAT/STRING + mixed-type-same-name case |
| **P1.7** | `selected_bits_` vector of `std::atomic<uint64_t>` + atomic API | `include/BALL/KERNEL/moleculeStore.h` (`is_selected` / `set_selected` / `set_range_selected`) | unit test sets/clears bits, range-set under concurrent reader thread, asserts no torn reads / no data race under TSan |
| **P1.8** | Wire `Selectable` mutations to side bitmap | `source/CONCEPT/selectable.C` (every mutation path adds side-bitmap update); preserves contains_selection_ tree-propagation | `SideTableParity_test` asserts post-select / post-deselect parity |
| **P1.9** | `CompositeIteratorStability_test` for D31 | `test/CompositeIteratorStability_test.C` exercises insertion-during-iteration | passes; no node-table caching detected in iterator implementations |

Each sub-phase ends with: full CORE_ONLY ctest pass + commit.

## Risk register

- **R-P1.A**: D22a's 5-link node design accidentally regresses an
  O(1) Composite mutation to O(degree) by walking sibling links
  twice (insertBefore + maintaining last_child consistency).
  *Mitigation:* explicit invariant in P1.2 commit message — all
  splice ops touch O(1) sibling pointers; verified by sub-step P1.3
  test.
- **R-P1.B**: `std::vector<std::atomic<uint64_t>>` is not
  trivially-resizable (atomic types are not move-constructible in
  the C++17 model). *Mitigation:* `selected_bits_` is sized at
  store construction to capacity; `MoleculeStore::reserve_atoms_`
  rebuilds it via swap-with-new-vector. The bitmap reserves more
  than the live atom count, so per-add resize is rare.
- **R-P1.C**: Sparse fallback bag's `HashMap<atom_idx,
  PropertyBag>` may bloat the per-store memory when a workload
  has many low-fill dynamic properties (Track B FORMAT readers
  set debug-only annotation properties on a few atoms).
  *Mitigation:* P1 doesn't fail on this; document as P5 perf
  measurement; promotion logic at compact() handles eventual
  cleanup.
- **R-P1.D**: `compact()` and column promotion interact with
  the v2.0 generation-counter contract (D17). Mid-promotion, an
  in-flight `CompiledExpression` could see column state in an
  inconsistent shape. *Mitigation:* promotion runs INSIDE
  `compact()`'s existing generation-bump + cache-invalidate
  window. No new contract needed.

## Verification artifacts produced in P1

- `test/SideTableParity_test.C` — runs the full Composite +
  PropertyManager + Selectable mutation API and asserts each
  side-table reflects inline state.
- `test/CompositeIteratorStability_test.C` — verifies D31
  iterator invariant.
- `test/PropertyColumn_test.C` — unit-tests the registry + columns
  + sparse bag.
- `test/SelectedBits_test.C` — unit-tests the atomic word vector
  including a 4-thread concurrent stress.

## Codex review gates

Per D33 (revised review cadence):

- **R18 (P1 planning review):** this document, before any P1
  code lands. Probes:
  - is the 9-substep split correct, or are there hidden
    coupling between P1.4 (PropertyColumn) and P1.6 (wire) that
    would mean we should land them as one commit?
  - does the SideTableParity_test design catch the right
    parity bugs?
  - is the BALL_EMPTY_BASES macro really needed in P1 if MSVC
    CI doesn't run until D34?
- **R19 (P1 close review):** after all 9 sub-phases land. Probes
  TBD; likely focused on actual parity correctness + perf
  parity.

## Next action

After this plan commits: spawn R18 (Codex CLI) on this file +
V21-DECISIONS.md. Then execute P1.1 → P1.9 atomically.
