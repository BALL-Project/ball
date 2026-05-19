# v2.1-P2 — Parallel Side-Table Maintenance: PLAN (rewritten)

**Phase:** P2 (post-R20 NO-GO pivot; lower-risk than original P2).
**Status:** Planning.
**Authored:** 2026-05-19 (rewritten after R20 + maintainer decision
to adopt Option B + RTTI removal).
**Decisions consumed:** D22b, D23b, D24b, D31b, **D36** (two-phase
flip across v2.1+v2.2), D37 (RTTI removal scope).
**Supersedes:** original P2-PLAN.md (the inheritance-flip plan that
R20 found NO-GO).

## Goal

Maintain the v2.1 side tables in parallel with the v0 inline
`Composite + PropertyManager + Selectable` state. Every mutation
through the v0 API ALSO writes to the matching side table. No
inheritance change. No source-compatibility break.

**This is the work P1.3, P1.6, and P1.8 deferred.** Per the
post-R20 pivot, the inheritance flip itself moves to v2.2 (D36);
P2 ships only the parallel maintenance + invariant verification.

## Why this is safer than the original P2

The original P2 (R20 NO-GO) tried to flip Atom's inheritance and
forward through empty shim bases. R20 found that breaks:

- `Composite*` iterator payload (`AtomIterator` filters
  `CompositeIterator`)
- `dynamic_cast<Atom*>(Composite*)` RTTI surface
- `RTTI::isKindOf<Atom>` checks
- Selection tree propagation counters on parent Composites
- `System::adopt` migration boundary (only core columns migrate)
- Full `PropertyManager` surface beyond setProperty/getProperty
- SIP Python bindings
- Sub-phase atomicity (placeholder forwarders break runtime tests)

By NOT flipping inheritance in v2.1, every one of these concerns
becomes a v2.2 concern with a full release cycle to address them.
v2.1 ships in a known-correct shape; v2.2 has the bake time to
de-risk the flip.

## Out of scope for P2

- ANY change to Atom's inheritance chain (v2.2).
- RTTI removal (P3; v2.1 still uses the old RTTI surface
  internally — D37 cleans it up post-P2).
- Reading from side tables (kept as v2.2's source-of-truth flip).
- JSON load batching (P4).
- Perf gates / iter API (P5).

## Phase gate (success criteria)

1. **Every** `Composite::appendChild / removeChild / insertBefore /
   insertAfter / spliceBefore / spliceAfter / splice / insertParent
   / destroyChildren_ / destroy` call ALSO maintains the matching
   `composite_nodes_` entry through `MoleculeStoreSideTables`.
2. **Every** `PropertyManager::setProperty / clearProperty / clear
   / swap / set / operator=` call ALSO maintains the matching
   `property_columns_` (or `sparse_bag_`) entry.
3. **Every** `Selectable::select / deselect / setSelected /
   select_(bool) / deselect_(bool)` call ALSO maintains the
   matching `selected_bits_` bit.
4. `System::adopt` and `System::adoptSubtree` migrate the
   side-table state alongside the core columns.
5. `SideTableParity_test` extended with end-to-end parity
   assertions: after each v0 mutation, the side table reflects
   the same state.
6. CORE_ONLY ctest: 284/284 PASS preserved.
7. Codex R21 close review (the planning review is R20b on this
   document).
8. `MoleculeStore_test` 100-run stress: 0/100 flakes.

## Risk register (post-pivot, much smaller than original P2)

- **R-P2.A (LOW risk):** parallel maintenance can drift from v0
  inline state if a mutation path is missed. *Mitigation:*
  `SideTableParity_test` catches this; full ctest catches
  observable regressions even if parity is stale (because reads
  come from v0 inline through v2.1).
- **R-P2.B (LOW risk):** side-table maintenance adds runtime
  overhead to mutations. *Mitigation:* mutations are rare relative
  to reads in BALL workloads; the overhead is amortized.
  Benchmarked at P5.
- **R-P2.C (LOW risk):** orphan→system adoption can lose
  side-table state if migration is incomplete. *Mitigation:* P2.4
  extends `adopt` + `adoptSubtree` for the new state explicitly.

## Sub-phases

P2 splits into **5 atomic commits** (smaller than original 9
because no shim-class work):

| # | Subject | Deliverable | Gate |
|---|---|---|---|
| **P2.1** | Wire `Composite` mutations to maintain `composite_nodes_` | `source/CONCEPT/composite.C`: every mutation path adds side-table update under a `// V2.1 P2.1: side-table mirror` comment. Store-reach helper uses a VIRTUAL hook (R20b-2 fix): `Composite::getCompositeStore_()` returns nullptr default; `System::getCompositeStore_()` returns &store_; `Atom::getCompositeStore_()` returns store_. NO new `dynamic_cast<System*>` introduced (D37 RTTI hygiene preserved). | full ctest passes; `SideTableParity_test` extended to assert post-mutation parity for `appendChild + removeChild + insertBefore + insertAfter + spliceBefore + spliceAfter + splice + destroyChildren_` |
| **P2.2** | Wire `PropertyManager` mutations to side tables — **Atom-instance only** (R20c-3 scope decision) | `source/CONCEPT/property.C`: every named-property mutation path checks `dynamic_cast<Atom*>(this) || /* better: */ this->getAtomIdx_()` (a new virtual `PropertyManager::getAtomIdx_()` that returns `(MoleculeStore*, atom_idx)` for Atom-derived PMs and `nullptr` for AtomContainer/Molecule/Residue/Bond PMs) — non-Atom PMs are v0-only in v2.1. Side-table writes use D23b sparse-first lookup precedence. **Bit-property mutators in scope, sparse-only (R20c-2 decision):** `setProperty(Property)`, `clearProperty(Property)`, `toggleProperty(Property)` go to the sparse_bag_ keyed by atom_idx with a synthetic name like `"_bit_<n>"` for Property bit `<n>`. Packed-bool column deferred to v2.2 if profiling shows it matters. Iteration via `beginNamedProperty/endNamedProperty` stays on v0 inline. | parity asserted for Atom's `setProperty(INT/FLOAT/STRING/NamedProperty/Property bit)`, `clearProperty` (name + bit), `toggleProperty`, `clear`, `swap`, `set`, `operator=`. Plus negative tests: `mol.setProperty("foo", 5)` (Molecule, non-Atom) does NOT write to side tables; `mol.getProperty("foo")` still returns 5 from v0 inline. |
| **P2.3** | Wire `Selectable` mutations to `selected_bits_` | `source/CONCEPT/selectable.C`: every mutation path. Atomic word ops via `set_selected_`. Preserves tree-propagation counters on parent Composites (those stay on v0 inline, unchanged). | parity asserted for select/deselect/setSelected; tree-propagation tests in Selector_test continue to pass; SideTableParity_test docs that single-thread parity is the invariant — NOT a cross-thread renderer-sync guarantee. |
| **P2.4** | Extend `System::adopt` + `adoptSubtree` for side-table migration — **subtree transaction** (R20c-1 closure) | `source/KERNEL/system.C` implements an explicit **subtree transaction**: (1) **Scan phase:** DFS the source subtree; collect every Composite node's v0 inline state + every Atom's source-store side-table state (composite_handle, property column entries, sparse bag entries, selected bit). All snapshotted in a local `MigrationPlan` struct. NO destination writes yet. (2) **Allocate phase:** allocate every destination composite_nodes_ row needed + every destination atom row + dest property column entries via registerColumn (may throw on cap). If ANY allocation fails, the partial destination state is rolled back via the free list before returning the error; source side-table state is UNTOUCHED. (3) **Write phase:** write all destination topology (parent/first/last/prev/next handles) + property column entries + selected bits using the freshly-allocated destination handles. Cannot fail (all allocations done). (4) **Commit phase:** rewrite every source Atom's `store_` pointer + composite_handle to point at destination; release source side-table rows; bump destination generation. **Backfill:** identical algorithm even when source has no prior side-table state (free-standing subtree case from R20b-1): scan phase reads from v0 inline pointers, allocate+write phases populate the destination side-table from those. Atom-local state migrates per the 4-stream contract; non-Atom Composite nodes get topology-only entries (no property/selection migration since non-Atom PMs are v0-only per P2.2). | adoption tests still pass; new `AdoptionSideTableParity_test` covers: (a) adopt from System-A to System-B (cross-system migration); (b) sys.insert(mol) of free-standing molecule (backfill); (c) registry-cap-during-adopt failure preserves source. |
| **P2.5** | `compact()` integration | `source/KERNEL/moleculeStore.C`: `compact()` calls `promote_sparse_` for any sparse property name above the 10% fill threshold; rebuilds `composite_nodes_` free list. Existing exclusive-access contract preserved. New stress test exercises repeated compact/adopt/mutate parity (P20b-6 follow-up). | promotion verified via parity test; compact() preserves parity; 100-run stress shows 0/100 parity drift. |

Estimated effort: 1-2 days execution + R20b planning review +
R21 close review.

## Wiring approach detail (P2.1 reach-store helper — REVISED post-R20b)

The fundamental challenge for parallel maintenance: when
`composite.C`'s `appendChild(child)` runs, it needs to find the
MoleculeStore to update the side table. The store isn't known to
`Composite` directly.

**Approach (R20b-2 fix — virtual hook, NOT dynamic_cast):**

```cpp
class Composite {
public:
    // Default: free-standing Composite has no store.
    virtual MoleculeStore* getCompositeStore_() { return nullptr; }
};
class Atom : public Composite, ... {
public:
    // Atom always has store_ from its v0 binding (orphan by default).
    MoleculeStore* getCompositeStore_() override { return store_; }
};
class System : public AtomContainer {
public:
    // System owns its store; expose to side-table maintenance.
    MoleculeStore* getCompositeStore_() override { return store_.get(); }
};
```

Composite already has a vtable (inherits PersistentObject); adding
this virtual is free in space + 1 vtable slot.

For a Composite-mutation on `mol` where `mol` is a Molecule (no
override), `getCompositeStore_()` returns nullptr → side-table
maintenance no-ops at the mutation point. **The state is rebuilt
at adoption time by P2.4's backfill pass.**

**Free-standing Composite subtrees (R20b-1):**

A common pattern: build `Molecule mol; Atom a; mol.insert(a);
mol.insert(b); ...` BEFORE adding `mol` to any System. During
construction, side-table mutations no-op on the Molecule node
(nullptr store from `Composite::getCompositeStore_()` default).

When `sys.insert(mol)` adopts the molecule, P2.4's backfill pass
walks the v0 inline Composite subtree DFS, allocates destination
`composite_nodes_` for each non-Atom Composite, rewrites
parent/first/last/prev/next handles to match the v0 pointer
structure, and migrates Atom-local state per the 4-stream rule.

After backfill, the side table represents the full subtree
correctly. Subsequent mutations on `mol` reach the System store
via the parent chain → `System::getCompositeStore_()`.

**Caching:** for tree mutations the store reach doesn't change.
Cache the result on `Composite::store_cache_` (LOW priority; P5
perf phase if needed).

## Codex review gates (per D33b)

- **R20 / R20b / R20c / R20d**: P2 planning review iterations.
  R20 NO-GO, R20b NEEDS-FIX, R20c NEEDS-FIX (narrow), R20d
  expected GO after this revision.
- **R21**: P2 close review after all 5 sub-phases land.

## Test split (R20c-5 follow-up)

P2's parity test suite is large enough to warrant splitting from a
single monolithic `SideTableParity_test`. Per R20c-5:

- `SideTableParity_test` (existing, P1 baseline) — handle/node/
  column/sparse/atomic primitives.
- `CompositeSideTableParity_test` (P2.1 new) — tree mutations.
- `PropertySideTableParity_test` (P2.2 new) — named + bit + Atom-only.
- `SelectionSideTableParity_test` (P2.3 new) — selection + tree
  propagation interaction.
- `AdoptionSideTableParity_test` (P2.4 new) — subtree transaction +
  backfill + failure modes.

Shared helpers stay in `SideTableParity_test.C` (or a `_helpers.h`).

## Next action

After this plan commits: spawn R20b in foreground (8-10 min).
If R20b returns GO, execute P2.1 → P2.5 sequentially.
If R20b returns NEEDS-FIXES, apply fixes and re-run.
