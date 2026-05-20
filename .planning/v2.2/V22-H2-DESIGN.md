# BALL v2.2 — H2 Design: mutation-mirror + handle-yielding traversal

**Status:** design lock (R35 review pending).
**Authored:** 2026-05-20, post-H1b (R34 GO).
**Companion:** `V22-H0-ADDENDUM.md` (D56–D62), `V22-H1b-DESIGN.md`
(D63–D66a), `V22-DECISIONS.md`, `V22-API-BREAK-LEDGER.md`.
**Phase:** H2 — "handle-yielding iterators + `apply` over `ChildRef`
edges + the full mutation-mirror set + `HierarchyParity_test` green."

H2 is the **crux execution risk** (D60): through H1–H3 the v0 object
tree is the source of truth and the container table is a *verified
mirror*. H2 wires every forward v0 mutation to mirror into the table and
proves parity after each. The historical hazard is the **P2.1.1
destruction-cascade trap** (R29): mirroring while `~Composite` tears down
a subtree corrupts state. This doc locks the architecture, the
destruction guard that defuses that trap, the full mutation→table-op
mapping, and the handle-yielding traversal layer.

---

## Grounding (the v0 surface H2 hooks)

- **Mutation methods (Composite):** `prependChild`, `appendChild`,
  `insertBefore`, `insertAfter`, `spliceBefore`, `spliceAfter`, `splice`,
  `swap`, `removeChild`, `clear`, `destroy` (`include/BALL/CONCEPT/
  composite.h`). `AtomContainer`/typed containers forward to these.
- **Property:** `PropertyManager::setProperty/clearProperty`.
- **Selection:** `Selectable::select/deselect` + `Composite` selection
  counter propagation (`updateSelection_`).
- **Destruction model (`source/CONCEPT/composite.C`):** `~Composite()` =
  `if (parent_) parent_->removeChild(*this); destroyChildren_();`.
  `destroyChildren_()` iterates `first_child_` and `delete`s each
  auto-deletable child → that child's `~Composite` calls
  `parent_->removeChild(*this)` **on the mid-destruction parent**. This
  is the exact P2.1.1 trap.
- **v2.1 mirror precedent (superseded):** `Composite::mirrorToSideTable_`
  + `getCompositeStore_` + `getCompositeHandle_/setCompositeHandle_` (the
  8-byte `composite_handle_packed_` slot) + `compositeKindForSideTable_`.
  These target the A1-era `composite_nodes_` 5-link side table, which A2
  **supersedes** (arch doc). H2 stops calling the `composite_nodes_`
  mirror and reuses the slot for the container-row binding (D68).

## D67. Mirror architecture: per-mutation forward hooks, v0 first

- Every forward mutation **writes the v0 tree first** (the existing v0
  code path, unchanged), **then** calls a mirror hook that applies the
  equivalent op to the `ContainerTable`. v0 stays the source of truth
  (D60); the table is updated to match.
- Hooks live **inside the v0 `Composite`/`AtomContainer` mutation
  methods** (after the v0 write), not in a separate observer — this keeps
  the mirror synchronous with the v0 write and impossible to forget.
- The **`composite_nodes_` mirror (`mirrorToSideTable_`) is retired** in
  H2: nothing reads it (D36), and A2 replaces it with the container
  table. Its slot is reused (D68); the `composite_nodes_` table +
  `mirrorToSideTable_` become dead code, removed in an H2 cleanup
  sub-step (or left inert if removal risks churn — decided at H2 close).
- Mirror writes are **forward-only** (D45/D60): never from a destructor
  cascade (D69). Atom/container teardown frees table rows wholesale at
  `~System`; individual destruction may orphan rows (acceptable — see
  D69).

## D68. v0-object → container-row binding (reuse the 8-byte slot)

Each v0 **container** object needs to know its `ContainerTable` row index
so a mutation hook can address it in O(1).

- **Reuse `Composite::composite_handle_packed_`** (the existing 8-byte
  slot, freed by retiring the `composite_nodes_` mirror) to store the
  container row index. New accessors `getContainerRow_()/setContainerRow_
  (u32)` read/write the low 32 bits; `0` = unbound (row 0 is the table
  sentinel). No growth of `Composite`/`Atom` (the slot already exists).
- **Atoms** do NOT use this slot for a row — an atom is a leaf addressed
  by its `Atom::getStoreIndex()` (the existing handle index). The mirror
  discriminates atom-vs-container children via `detail::compositeAsAtom_`
  (still valid in dual existence; removed only at H4).
- **Lazy allocate-on-first-mirror:** a container's row is allocated the
  first time it participates in a mirrored mutation while it has a
  reachable store (`getCompositeStore_() != null`); the row index is
  written back to the slot. A free-standing container (no store) is
  mirrored later, at adoption (the orphan→System migration already
  carries rows per D56).
- **System** is the implicit root: it is not a container row; a
  top-level molecule's `parent_container_idx == CONTAINER_NONE`
  (matches H1a).

## D69. Destruction guard — the P2.1.1 trap defused

**Rule:** `removeChild(child)` (and the per-child removal inside
`clear()`/`destroyChildren_`) **mirrors the detach iff `this` (the
parent) is NOT being destroyed.**

- Add a `being_destroyed_` flag to `Composite`, set **true at the very
  start of `~Composite`** (before `destroyChildren_()`), and likewise at
  the start of `destroy()`'s self-teardown.
- `removeChild`'s mirror hook checks **`this->being_destroyed_`** (the
  PARENT's flag), not the child's.

Why this is correct, traced against `~Composite`:

1. **`delete residue` (top-level):** `~Residue → chain.removeChild
   (residue)`. `chain` is alive (`chain.being_destroyed_ == false`) →
   **mirror fires**: chain's row drops the residue ChildRef. Correct —
   the live parent's child list shrank. The residue's own row is now
   orphaned (unreachable from root); it leaks until wholesale free
   (acceptable per D60).
2. **…then `residue.destroyChildren_()` `delete`s its atoms:** `~Atom →
   residue.removeChild(atom)`. `residue.being_destroyed_ == true` →
   **mirror skipped**. Correct — we never mutate the dying residue's
   mirror (the trap). Its atom rows leak with it.
3. **`~System` cascade:** `~System → … → delete molecule → ~Molecule →
   system.removeChild(molecule)`. `system.being_destroyed_ == true` →
   skipped; and each deeper level's parent is also destroying → skipped.
   The whole table is freed wholesale at `~System` (a single
   `ContainerTable` clear), no per-object mirror.

**Parity is preserved** because the parity test compares the structure
**reachable from the live roots**; orphaned/leaked rows are unreachable
and never compared. This is the same forward-only guarantee R32 verified
for `migrate_subtree_from`; D69 extends it to the destruction path.

`clear()` on a **live** container (not a destructor) sets no flag, so its
per-child removals **do** mirror (the live container correctly ends with
empty children). `clear()` is distinct from `~Composite`.

## D70. Full mutation → ContainerTable-op mapping

| v0 mutation (on parent P, child C) | ContainerTable op | Notes |
|---|---|---|
| `appendChild(C)` | `append_child(P, ref(C))` | ref(C)=ATOM(idx) or CONTAINER(row) |
| `prependChild(C)` | `prepend_child(P, ref(C))` | |
| `insertBefore(C)` *(C inserted before `this`)* | `insert_child_before(parentOf(this), ref(C), ref(this))` | v0 `insertBefore` inserts C as a sibling before the receiver |
| `insertAfter(C)` | `insert_child_after(parentOf(this), ref(C), ref(this))` | |
| `spliceBefore/After/splice(C)` | move each child of C under P at the splice point via `reparent_child` (remove-before-add, R32) | splice relocates C's children |
| `swap(C)` | exchange the two rows' child lists + payload, fix reverse edges | mirror the v0 `swap` semantics |
| `removeChild(C)` | `remove_child(P, ref(C))` **unless P.being_destroyed_** (D69) | clears reverse edge |
| `clear()` (live) | `remove_child` per child (mirrors) → empty | not on a destructing object |
| `setProperty/clearProperty` (container) | container property column / sparse bag keyed by row (D59) | atom props already store-backed |
| `select/deselect` (atom) | `bump_selection_up(parentRow, ±1)` (D59) | walk the row parent chain |
| **reparent (any move)** | always `reparent_child` (remove-before-add), never bare attach | R32 LOW |
| container-PROPERTY migration | fill in `migrate_subtree_from` | H1a/R32 carry-over (now wired) |

`ref(C)` = `ContainerChildRef`-style: `compositeAsAtom_(C)` →
`ATOM(C.getStoreIndex())`, else `CONTAINER(C.getContainerRow_())`
(allocating C's row first if unbound, D68).

## D71. Handle-yielding traversal layer

New read API over the table (tested in isolation in H2; consumers
migrate in H3):

- **`AtomIterator` (handle-yielding):** yields `Atom` by value, backed by
  a preorder walk of the container subtree's `ChildRef` edges, emitting
  atom children at the leaves. Replaces the `CompositeIterator`-derived
  atom traversal for the table path. (The grouping CSR (D55/D57) is an
  optional cache; the `ChildRef` vectors are the source of truth.)
- **Container-handle iterators:** e.g. `ResidueHandle`-yielding iteration
  over a chain's residue children, etc., filtered by `ContainerKind`.
- **`apply` over `ChildRef`:** a preorder/postorder walk yielding handles
  to a processor `operator()(Atom)` / `operator()(<ContainerHandle>)`.
  This is the table-side re-expression of `Composite::apply<T>`.
- Predicates/processors take handles **by value** (the D61 Class-E
  signature break; deferred to H3 consumer migration — H2 only provides
  the handle-yielding iterators + `apply`, exercised by tests).

H2 does **not** migrate any consumer (that is H3) and does not delete the
v0 `Composite&` traversal (that is H4). It adds the parallel handle path.

## D72. Sub-phasing + parity-after-each-mutation

| Sub | Scope |
|---|---|
| **H2a** | `being_destroyed_` flag + D69 guard + the container-row binding (D68) + retire the `composite_nodes_` mirror. The append/remove/insert mirror hooks. `HierarchyParity_test` asserts parity after append/prepend/insert/remove/`delete`/`~System`. |
| **H2b** | splice/swap/clear mirror + container `setProperty` + `select/deselect` counter mirror + container-property migration in `migrate_subtree_from`. Parity after each. |
| **H2c** | handle-yielding `AtomIterator` + container-handle iterators + `apply` over `ChildRef`. A traversal test compares handle-yield order against the v0 `Composite` preorder. |
| **H2d** | full-surface parity sweep: a randomized mutation sequence (insert/remove/splice/swap/clear/select/setProperty/delete) with a parity assert after **every** step; `~System` wholesale-free check. H2 close + Codex review. |

**The parity test is the guardrail.** After every covered mutation it
asserts the table reachable from the live roots reproduces the v0 tree:
structure, ordered `ChildRef` sequence, reverse parent links, payload,
container properties, and selection counters (the R32 P31-4 comparator,
now run incrementally).

## Risks / open questions for R35

1. **Atom store-index timing in `removeChild` during `delete atom`:**
   `~Atom` (derived) releases the atom slot before `~Composite` (base)
   runs `parent->removeChild`. The mirror needs the atom's store index at
   that point — confirm `getStoreIndex()` is still readable (the handle
   fields are not zeroed until after, or capture earlier). May require
   capturing the atom index in `~Atom` before slot release, or mirroring
   the atom-detach from `~Atom` rather than `~Composite`.
2. **`being_destroyed_` set in `~Composite` (base, runs last):** confirm
   no derived destructor (`~AtomContainer`, `~Residue`, …) mutates
   children before `~Composite` sets the flag. If any does, set the flag
   earlier (a guarded entry in the most-derived path or `destroy()`).
3. **Free-standing container mutation before adoption:** a `new Molecule;
   m.insert(atom)` sequence mutates with no reachable store; the mirror
   defers to adoption (D56 migration). Confirm the orphan store path
   (D56) covers the full pre-adoption mutation set, or that deferral is
   sound.
4. **Selection counter double-count / `updateSelection_` interaction:**
   the v0 selection counters and the table `selection_count` must not
   diverge; confirm the mirror hooks the leaf `select/deselect`, not the
   propagated `updateSelection_`, to avoid double counting.
5. **`swap` semantics:** v0 `swap` exchanges position in the tree;
   confirm the row-level child-list+payload exchange reproduces it
   exactly (including reverse edges and any atom `parent_container_idx`).

## Decisions added

| # | Contract |
|---|---|
| D67 | Per-mutation forward mirror hooks in v0 methods (v0 first, then table); retire the `composite_nodes_` mirror |
| D68 | v0-container→row binding reuses the 8-byte `composite_handle_packed_` slot; atoms use `getStoreIndex`; lazy allocate-on-first-mirror |
| D69 | Destruction guard: `being_destroyed_` set at `~Composite` entry; `removeChild` skips the mirror iff the PARENT is being destroyed (defuses P2.1.1) |
| D70 | Full mutation→ContainerTable-op mapping (incl. reparent_child for all moves) |
| D71 | Handle-yielding `AtomIterator`/container iterators/`apply` over `ChildRef`; consumers migrate in H3 |
| D72 | H2a–H2d sub-phasing; parity asserted after every covered mutation |

## Next action

Codex **R35** reviews this H2 design (is the destruction guard correct
and complete, is the mutation mapping faithful, are the 5 open risks
real/closed, is H2a a bounded first step?). On GO, implement H2a.
