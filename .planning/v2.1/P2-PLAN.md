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
| **P2.2** | Wire `PropertyManager` mutations to side tables | `source/CONCEPT/property.C`: every mutation path. Uses D23b sparse-first lookup precedence (well-known column fast-path; mismatched-type → sparse bag with override semantics). **Bit-property mutators IN SCOPE (R20b-3 fix):** `setProperty(Property)`, `clearProperty(Property)`, `toggleProperty(Property)` map to a packed bool column or stay sparse (P2.2.1 decides). Iteration via `beginNamedProperty/endNamedProperty` stays on v0 inline (v2.2's responsibility to replace). | parity asserted for `setProperty(INT/FLOAT/STRING)`, `setProperty(NamedProperty)`, `setProperty(Property)` bit mutator, `clearProperty` (both name + bit), `clear`, `swap`, `set`, `operator=`. Includes new explicit test that `setProperty("foo", 5)` then `beginNamedProperty()` still finds "foo". |
| **P2.3** | Wire `Selectable` mutations to `selected_bits_` | `source/CONCEPT/selectable.C`: every mutation path. Atomic word ops via `set_selected_`. Preserves tree-propagation counters on parent Composites (those stay on v0 inline, unchanged). | parity asserted for select/deselect/setSelected; tree-propagation tests in Selector_test continue to pass; SideTableParity_test docs that single-thread parity is the invariant — NOT a cross-thread renderer-sync guarantee. |
| **P2.4** | Extend `System::adopt` + `adoptSubtree` for side-table migration **+ free-standing-subtree backfill** (R20b-1 fix) | `source/KERNEL/system.C`: 4-stream migration with **transactional semantics** (R20b-4 fix): (1) snapshot composite_handle + property column entries + sparse bag entries + selected bit on source; (2) allocate destination side-table rows; (3) write destination data; (4) only then release source rows. On registry-cap failure: keep source rows untouched and fall back to sparse (no partial mirror). **Backfill pass:** when `sys.insert(mol)` adopts a free-standing Composite subtree (Molecule/Chain/Residue) whose pre-adoption mutations were no-ops on side tables, walk the v0 inline tree DFS allocating destination `composite_nodes_` entries + reconstructing parent/first/last/prev/next links. Backfill applies to non-Atom Composite nodes in the subtree; Atom-local state migrates per the 4-stream rule. | adoption tests still pass; migration parity test asserts full side-table state matches v0 inline AFTER adopt completes; failure-mode test asserts source state is preserved if destination fails. |
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

- **R20b**: P2 planning review on THIS document (post-pivot).
- **R21**: P2 close review after all 5 sub-phases land.

## Next action

After this plan commits: spawn R20b in foreground (8-10 min).
If R20b returns GO, execute P2.1 → P2.5 sequentially.
If R20b returns NEEDS-FIXES, apply fixes and re-run.
