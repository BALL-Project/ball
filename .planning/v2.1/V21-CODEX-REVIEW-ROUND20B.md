# Track A v2.1 Cluster R20b — Codex CLI Round 20b (2026-05-19)

**Status:** Complete
**Verdict:** NEEDS-FIXES
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.1 P2 planning re-review — parallel side-table maintenance (post-pivot)
**Predecessor:** R20 NO-GO (original inheritance-flip plan)

## R20 BLOCKER closure status

| R20 BLOCKER | Resolution in rewritten plan | Status |
|---|---|---|
| P20-1 shim class design | D36 removes the v2.1 inheritance flip. No D17 shim bases are introduced in P2. | CLOSED |
| P20-2 cross-class composite tree consistency | Atom remains a `Composite`; existing `Composite&` insertion/traversal contracts stay source-compatible. | CLOSED |
| P20-3 Atom destructor cleanup | Atom remains on the current v0 inheritance/lifecycle path. P2 only mirrors side-table state. | CLOSED |
| P20-4 PropertyManager full surface | P2.2 now names `setProperty`, `clearProperty`, `clear`, `swap`, `set`, and `operator=`. This closes the original surface gap for v2.1 dual-write, but the plan should explicitly include bit-property mutators (`setProperty(Property)`, `clearProperty(Property)`, `toggleProperty(Property)`) because they are inline mutation paths. | CLOSED WITH FIXUP |
| P20-5 Composite iterator binding | Atom remains a `Composite`; iterator payloads remain `Composite*`/`Composite&`. | CLOSED |
| P20-6 Selection tree propagation | v0 parent counters remain source of truth; P2.3 only mirrors selected bits and preserves existing tree propagation. | CLOSED |
| P20-7 Atom_test1/2 regression risk | Reads remain from v0 inline state in v2.1, so side-table drift should not affect observable behavior. | CLOSED |
| P20-8 Orphan atom lifecycle | P2.4 explicitly extends `System::adopt` and `adoptSubtree` to migrate side-table state. However, migration/backfill completeness is under-specified for free-standing non-Atom Composite trees that had prior no-op side-table mutations. | PARTIAL |
| P20-9 Bond pointers to Atom | No inheritance change; bond endpoint pointers remain `Atom*`. | CLOSED |
| P20-10 PropertyManager bag iteration | v2.1 iteration still reads `named_properties_`, so `pm.setProperty("foo", 5); for (...)` remains correct. Side-table-only iteration is deferred to v2.2. | CLOSED |
| P20-11 Sub-phase atomicity | Five smaller commits preserve v0 reads, so placeholder-forwarder breakage is gone. P2.4 still needs a sharper atomic migration/backfill order before execution. | PARTIAL |

## Probe answers

| # | Probe | Verdict | Notes |
|---|---|---|---|
| P20b-1 | `getCompositeStore_()` reach helper | NEEDS-FIX | Adding a new `dynamic_cast<System*>` reach path in P2 conflicts with D37's immediate P3 RTTI-removal goal. Use a virtual store-reach hook instead: `Composite::getCompositeStore_()` returns nullptr by default; `System` overrides to return `&store_`; `Atom` overrides to return `store_`; parent-chain walk calls the virtual hook on ancestors. |
| P20b-2 | Atom-specific store shortcut | OK WITH FIXUP | A virtual override on `Atom` is the clean expression. Since `Composite` already has a vtable, this does not create the original EBO/shim problem. The plan should replace the impossible-looking `static_cast<Atom*>(this)` wording with the override contract. |
| P20b-3 | Free-standing Composite no-op rationale | BLOCKER | `Atom a` has an orphan-store slot, but `Molecule mol; mol.insert(a);` has no reachable System store for the Molecule node. The Atom-side orphan store cannot correctly encode a parent Molecule node that has no side-table node/store. The plan must define later backfill of the whole v0 subtree, not just no-op side-table writes. |
| P20b-4 | `side_tables_` pre-existence assumption | BLOCKER | After free-standing mutations no-op, `sys.insert(mol)` must backfill composite nodes and links for `mol` plus descendants into the System store. Staying no-op until the next mutation leaves side tables stale for already-built trees and defeats the P2 bake/parity objective. |
| P20b-5 | `System::adopt` migration completeness | NEEDS-FIX | The four streams are the right list: `composite_handle`, property columns/dynamic sparse bag, sparse bag entries, and selected bit. The plan needs an all-snapshot-before-release order and a failure rule. If property column creation can fail due to registry cap, the destination side-table update must fail before source release or fall back wholly to sparse without losing the source state. |
| P20b-6 | `compact()` integration | OK | Existing `compact()` is already heavy: shrink vectors, rebuild string pools, bump generation, and invalidate compiled expressions. Running sparse promotion and composite free-list rebuild there is acceptable if it remains under the existing exclusive-access contract. Add one stress test around repeated compact/adopt/mutate parity. |
| P20b-7 | PropertyManager iteration | OK | In v2.1 there are no side-table-only writes, and iteration remains over `named_properties_`. The P2.2 gate should include an explicit test that `setProperty` followed by `beginNamedProperty/endNamedProperty` still observes the property. |
| P20b-8 | Parity assertions vs relaxed atomics | OK | Single-threaded parity assertions are valid: relaxed atomics are immediately visible to the same thread. Cross-thread readers must not treat side tables as an ordered mirror of v0 inline state in v2.1. This is acceptable because v2.1 has no supported side-table read path, but the plan should state that parity tests are single-threaded invariant tests, not a renderer synchronization guarantee. |
| P20b-9 | P3 RTTI removal sequencing | NEEDS-FIX | If P2 lands a dynamic-cast helper, P3 must remove it. Better: do not introduce the RTTI dependency in P2. Otherwise D37 must explicitly list `Composite::getCompositeStore_()` as a P3 removal site. |
| P20b-10 | v2.2 plan completeness | OK | D38 as a placeholder is reasonable before v2.1 ships because v2.1 no longer breaks source compatibility. A detailed v2.2-P0 plan should be a release-note/backlog gate, not a P2 execution blocker. |
| P20b-11 | Overall rewrite risks | NEEDS-FIX | Dual-write should not change observable behavior while reads stay v0. The new real risk is lifetime drift: no-op side-table writes on free-standing trees must be reconciled by adoption/backfill, and no v2.1 path should read side tables except tests/internal parity checks. |

## New findings

**R20b-1 — Free-standing Composite subtree backfill is required before P2 can execute.**  
This is introduced by the rewrite's `nullptr means no-op` store-reach rule. Free-standing non-Atom Composites are legal and common enough to plan for: build a Molecule/Chain/Residue tree, mutate children and properties, then insert it into a System. P2.1 cannot maintain side-table topology without a store, so P2.4 must rebuild the side-table representation from the v0 inline Composite tree at adoption time. Migrating only Atom-local side-table rows is insufficient.

Required fix: P2.4 must specify an adoption/backfill pass that allocates destination composite nodes for every adopted Composite node, rewrites each node's `composite_handle`, reconstructs parent/first/last/prev/next links from v0 pointers, migrates Atom property/selection streams, and releases source side-table nodes only after the destination side-table subtree is complete.

**R20b-2 — New RTTI helper undermines D37 unless replaced or explicitly sequenced.**  
The revised P2.1 says `Composite::getCompositeStore_()` walks to root and `dynamic_cast`s to `System*`. D37 says kernel Atom-RTTI removal follows P2, but adding a fresh RTTI dependency in the same area is avoidable. This is not the old Atom-inheritance blocker, but it is unnecessary coupling.

Required fix: use a virtual store-reach hook with default nullptr and concrete overrides on `System` and `Atom`, or add this helper to D37's mandatory removal checklist. The virtual design is preferable and removes the issue now.

**R20b-3 — PropertyManager dual-write surface still needs inline bit mutators.**  
P2.2 covers the named-property mutation surface well enough for R20 closure, but bit-property mutators live inline (`setProperty(Property)`, `clearProperty(Property)`, `toggleProperty(Property)`) and are observable v0 mutations. If selected side-table property columns are meant to mirror the whole `PropertyManager` state, these must be included in the P2.2 deliverable and tests, even if they map to sparse/legacy fallback first.

**R20b-4 — Migration failure semantics must be written before coding.**  
P2.4 says migrate composite handles, property entries, and selection, then release source composite nodes. That is the right shape, but it lacks a transaction rule. If destination column creation or sparse insertion fails after source mutation starts, the source slot may be released with only a partial mirror in the destination store.

Required fix: snapshot all side-table streams first; allocate destination atom slots and side-table rows second; write all destination side-table data third; only then release source rows. On registry-cap failure, either keep the source rows untouched and return/fallback, or define a guaranteed sparse fallback path.

## Overall verdict

**NEEDS-FIXES for P2 execution.**

The D36 pivot successfully closes the original R20 inheritance-flip blockers: Atom remains a `Composite`, iterators/RTTI/source compatibility stay intact for v2.1, selection propagation remains v0, and reads stay on the existing inline state. The rewritten plan is directionally sound.

It is not yet a clean GO because the new dual-write design has two planning gaps that should be fixed before coding starts: avoid the new RTTI store-reach helper, and define adoption-time backfill for free-standing Composite subtrees whose earlier mutations could not be mirrored. Once those are written into P2.1/P2.4, P2 should be executable as the lower-risk parallel-maintenance phase D36 intended.
