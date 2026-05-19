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
| **P2.1** | Wire `Composite` mutations to maintain `composite_nodes_` | `source/CONCEPT/composite.C`: every mutation path adds side-table update under a `// V2.1 P2.1: side-table mirror` comment. Side-table lookup reaches the store via `Composite::getCompositeStore_()` helper (walks parent chain up to `System`, returns its store; nullptr if free-standing). | full ctest passes; `SideTableParity_test` extended to assert post-mutation parity for `appendChild + removeChild + insertBefore + insertAfter + spliceBefore + spliceAfter + splice + destroyChildren_` |
| **P2.2** | Wire `PropertyManager` mutations to side tables | `source/CONCEPT/property.C`: every mutation path. Uses D23b sparse-first lookup precedence (well-known column fast-path; mismatched-type → sparse bag with override semantics). | parity asserted for setProperty(INT/FLOAT/STRING) + clearProperty + clear + swap + operator= |
| **P2.3** | Wire `Selectable` mutations to `selected_bits_` | `source/CONCEPT/selectable.C`: every mutation path. Atomic word ops via `set_selected_`. Preserves tree-propagation counters on parent Composites (those stay on v0 inline, unchanged). | parity asserted for select/deselect/setSelected; tree-propagation tests in Selector_test continue to pass |
| **P2.4** | Extend `System::adopt` + `adoptSubtree` to migrate side-table state | `source/KERNEL/system.C`: when an atom is adopted from one store to another, migrate its composite_handle + property entries + selection bit. release_composite_node_ on the source side. | adoption tests still pass; migration parity test added to SideTableParity_test |
| **P2.5** | `compact()` integration | `source/KERNEL/moleculeStore.C`: `compact()` calls `promote_sparse_` for any sparse property name above the 10% fill threshold; rebuilds `composite_nodes_` free list. | promotion verified via parity test; compact() preserves parity |

Estimated effort: 1-2 days execution + R20b planning review +
R21 close review.

## Wiring approach detail (P2.1 reach-store helper)

The fundamental challenge for parallel maintenance: when
`composite.C`'s `appendChild(child)` runs, it needs to find the
MoleculeStore to update the side table. The store isn't known to
`Composite` directly.

**Approach:** `Composite::getCompositeStore_()` walks the parent
chain to find the root, casts to `System*` (via dynamic_cast for
now — yes, P3 RTTI removal will replace this), and returns
`System::getMoleculeStore()`. If no System ancestor (free-standing
Composite tree), returns nullptr and the side-table maintenance
is a no-op for that mutation.

**Caching:** for tree mutations the store reaches don't change.
Each Composite caches the store pointer the first time it's
resolved; cache invalidates on parent change.

**Free-standing Composites:** maintenance no-ops are intentional.
Free-standing Composites are not in any store, so they have no
side-table state to maintain.

**Atom-specific shortcut:** Atom has `store_` directly. When
the mutation is on an Atom subclass, `getCompositeStore_()` can
just return `static_cast<Atom*>(this)->store_`. Saves the
parent-chain walk. (Implementation detail: virtual override on
Atom returns store_ directly.)

## Codex review gates (per D33b)

- **R20b**: P2 planning review on THIS document (post-pivot).
- **R21**: P2 close review after all 5 sub-phases land.

## Next action

After this plan commits: spawn R20b in foreground (8-10 min).
If R20b returns GO, execute P2.1 → P2.5 sequentially.
If R20b returns NEEDS-FIXES, apply fixes and re-run.
