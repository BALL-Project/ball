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

## Phase gate (success criteria) — REVISED 2026-05-19

**Revision note:** original P1 plan called for dual-write of every
Composite / PropertyManager / Selectable mutation. The maintainer's
2026-05-19 decision on R17c P17c-9 moved all mutation-mirror
wiring to **P2**, since P2's inheritance flip is the natural
moment to introduce side-table writes alongside the read flip.
Doing dual-write in P1 would be deleted code at P2.

The shipped P1 closure verifies:

1. `MoleculeStore` carries the three new SoA segments via PImpl:
   - `composite_nodes_` (vector of `CompositeNode`, 5-link per
     D22b, 48 B per node)  ✅
   - `property_columns_` (`PropertyColumnRegistry` with 10
     well-known FF columns predeclared + typed columns + string
     intern pool per column per D23b)  ✅
   - `sparse_bag_` (unordered_map<atom_idx, vector<SparseProperty>>
     fallback per D23b)  ✅
   - `selected_bits_` (`unique_ptr<atomic<uint64_t>[]>` + word
     capacity per D24b)  ✅
2. Allocator / accessor primitives are correct and tested:
   - `allocate_composite_node_` / `release_composite_node_` /
     `node_` round-trip (SideTableParity_test)  ✅
   - `Composite::getCompositeHandle_` / `setCompositeHandle_` /
     `getNode_` opaque-slot round-trip + topology view  ✅
   - `PropertyColumn<T>::set/get/clear/isSet` + presence bitmap  ✅
   - `StringPropertyColumn` intern pool dedup  ✅
   - registry `registerColumn` / `findColumn` / cap enforcement  ✅
   - `sparse_set_/find_/clear_` + `promote_sparse_` matching-type
     transfer  ✅
   - `selected_bits_` `resize_/set_selected_/is_selected_` with
     relaxed atomics  ✅
3. **NO** mutation wiring (`Composite::appendChild`, `PropertyManager::
   setProperty`, `Selectable::select` etc. continue to use v0 inline
   state unchanged). P2 lands the dual-write atomically with the
   inheritance flip.
4. All v2.0 CORE_ONLY tests still PASS + 2 new tests:
   - `SideTableParity_test`  ✅
   - `CompositeIteratorStability_test`  ✅ (D31)
   - Total: 284/284 PASS (was 282 in v2.0.0).
5. `BALL_EMPTY_BASES` macro defined  ✅; application to concrete
   classes happens at P2 with the inheritance flip.

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
| **P1.3** | Composite handle infrastructure (NOT mutation mirror — deferred to P2 per maintainer decision 2026-05-19) | `include/BALL/CONCEPT/composite.h` adds a private opaque `std::uint64_t composite_handle_packed_ = 0;` (8 B, P1-transient); `source/CONCEPT/composite.C` adds `getCompositeHandle_()` / `setCompositeHandle_(CompositeHandle)` accessors + `getNode_() -> CompositeTopologyView` by-value (per D31b). `_moleculeStoreInternal.h` gains `allocate_composite_node_()` / `release_composite_node_()`. **No mutation paths in composite.C are touched at P1.3.** The dual-write design originally specified in P1.3 is moved to P2, where Atom's inheritance flip is the natural moment to introduce side-table writes alongside the read-flip. | full ctest passes; `SideTableParity_test` (skeleton) verifies `side_tables_` is reachable + empty; `allocate_composite_node_` round-trip works |
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

## MSVC CI scope during P1 (R17c P17c-9 clarification)

Per D34b, the Windows GHA job starts running at P1 close on a
v2.0-equivalent baseline. During P1, the Windows job runs:

- **In scope for P1 close gating:** BALL/core build under
  MSVC + VS 2022 (everything `BALL_CORE_ONLY=ON` compiles),
  `SideTableParity_test` runs and passes, `Sizeof_test` runs (but
  the v2.1 size pins are NOT yet required to hold — Atom still
  inherits D2/D3/D4).
- **NOT in scope for P1 close gating:** `sizeof(Atom) ≤ 32 B`
  and `sizeof(Bond) ≤ 24 B` pins. Those become mandatory only at
  **P2 close** when the thin-handle flip actually claims them.
- **Why run MSVC at all in P1:** catches MSVC-specific build
  breakage early on the side-table infra commits. Avoids a
  surprise pile-up of Windows compile errors at the P2 close
  gate. The `BALL_EMPTY_BASES` macro is in place so P2 can rely
  on it without further per-file changes.

## Next action

After this plan commits: spawn R18 (Codex CLI) on this file +
V21-DECISIONS.md. Then execute P1.1 → P1.9 atomically.
