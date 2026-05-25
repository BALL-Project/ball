# BALL v2.2 — H2 Design: mutation-mirror + handle-yielding traversal

**Status:** design lock (R35 review pending).
**Authored:** 2026-05-20, post-H1b (R34 GO).
**Companion:** `V22-H0-ADDENDUM.md` (D56–D62), `V22-H1b-DESIGN.md`
(D63–D66a), `V22-DECISIONS.md`, `V22-API-BREAK-LEDGER.md`.
**Phase:** H2 — "handle-yielding iterators + `apply` over `ChildRef`
edges + the full mutation-mirror set + `HierarchyParity_test` green."

> **NOTE (2026-05-21, HC1 hierarchy collapse):** the mutation-mirror
> machinery here is **kind-agnostic** (row/edge ops over `ChildRef`) and
> SURVIVES the proposed System/Molecule/Fragment/Atom collapse unchanged.
> The only collapse impact on H2 is that the **handle-yielding traversal
> (H2c) filters by ROLE** instead of by the 8 `ContainerKind`s, and the v0
> `AtomContainer` binding used by the insert mirror is **transitional**
> (it dies at H4 with the v0 classes; it is not an enduring AtomContainer
> concept). See `V2X-HIERARCHY-COLLAPSE.md`.

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

## D68. v0-object → container-row binding (dedicated slot on AtomContainer)

Each v0 **container** object needs to know its `ContainerTable` row index
so a mutation hook can address it in O(1).

- **Add a dedicated `std::uint32_t container_row_idx_ = 0` to
  `AtomContainer`** (the base of every molecular container —
  Molecule/Chain/Residue/Protein/SecondaryStructure/Nucleotide/
  NucleicAcid/Fragment/System). `0` = unbound (row 0 is the sentinel).
  +4 bytes per container; containers are O(thousands) so memory is
  irrelevant. Transitional — removed at H4.
  - **NOT reusing `Composite::composite_handle_packed_` (R35 HIGH).**
    Reusing that slot while the old `getCompositeHandle_/
    setCompositeHandle_/mirrorToSideTable_` APIs still interpret it as a
    `CompositeHandle` is a footgun (a row index reinterpreted as handle
    bytes could resurrect `composite_nodes_` state), and would force
    rewriting `SideTableParity_test`. A dedicated slot leaves the
    superseded `composite_nodes_` machinery completely untouched (dead
    but inert; removed in a later cleanup, not H2).
  - The mirror hook obtains the slot via `dynamic_cast<AtomContainer*>
    (parent)` — in the molecular hierarchy the mutation receiver is
    always an `AtomContainer`. (Atoms are leaves, never a parent.)
- **Atoms** are addressed by `Atom::getStoreIndex()` (the existing handle
  index), not a row. The mirror discriminates atom-vs-container children
  via `detail::compositeAsAtom_` (valid in dual existence; removed at H4).
- **Lazy allocate-on-first-mirror:** a container's row is allocated the
  first time it participates in a mirrored mutation while it has a
  reachable store (`getCompositeStore_() != null`); the index is written
  to `container_row_idx_`. Free-standing containers (no store) are NOT
  mirrored during mutation — they are **materialized at adoption** (D73).
- **System** is the implicit root: not a container row; a top-level
  molecule's `parent_container_idx == CONTAINER_NONE` (matches H1a).

## D69. Destruction guard — the P2.1.1 trap defused

**Rule:** `removeChild(child)` (and the per-child removal inside
`clear()`/`destroyChildren_`) **mirrors the detach iff `this` (the
parent) is NOT being destroyed.**

- Add a `being_destroyed_` flag to `Composite`. **Set it true as the
  FIRST statement of each molecular DESTRUCTOR** — `~Atom`,
  `~AtomContainer`, `~Molecule`, `~Protein`, `~NucleicAcid`, `~Chain`,
  `~Residue`, `~Nucleotide`, `~SecondaryStructure`, `~Fragment`,
  `~System` — plus `~Composite` as a backstop. **Do NOT set it in
  `destroy()` or `clear()`** (R35b NEW HIGH).
  - **Why the destructors, not `destroy()`:** `destroy()`/`clear()` are
    ALSO called explicitly on a **live** object to empty it (the object
    survives). That emptying MUST be mirrored (live container → empty
    row). If the flag were set in `destroy()`, explicit `obj.destroy()`
    would wrongly skip the emptying mirror and leave a stale row (R35b).
    The destructor is the only unambiguous "this object will not survive"
    signal. Each derived destructor calls `destroy()` to do its teardown,
    so the flag set as the destructor's first line is in effect for the
    whole cascade — and is NOT set during an explicit `destroy()` on a
    live object (no destructor ran). The flag is never reset (an object
    whose destructor set it is gone).
- `removeChild`'s mirror hook checks **`this->being_destroyed_`** (the
  PARENT's flag), not the child's. Explicit `destroy()`/`clear()` on a
  live parent (flag false) therefore mirror the child removals correctly;
  destructor-driven teardown (flag true) skips them (the row leaks with
  the dying object).
- **H2a tests must cover BOTH** derived-destructor cascades (`delete
  residue`/`delete molecule`/`~System` — assert no mirror write fired,
  surviving roots parity-clean) AND explicit `destroy()`/`clear()` on a
  live container (assert the row is emptied to match the now-empty v0
  object).

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
| `spliceBefore/After/splice(C)` | for each child of C: `remove_child` from C's row, then positional insert under P at the splice point | splice relocates C's children |
| `swap(C)` | see swap scope below | mirror the v0 `swap` semantics |
| `removeChild(C)` | `remove_child(P, ref(C))` **unless P.being_destroyed_** (D69) | clears reverse edge |
| `replace(C)` | mirror as `insertBefore(C)` then `removeChild(this)` (v0 `replace` is exactly that, `composite.C:1085`) | composed of two mapped ops |
| `insertParent(...)` (R35 HIGH) | allocate the new parent row, then move the sibling range under it via remove+positional-insert | used by PDB import (`PDBFileDetails.C:166/261/307`) |
| `clear()` (live) | auto-deletable child → its `delete` re-enters `removeChild` (mirrors); **non-auto-deletable child → direct detach in v0 (`composite.C:1457`), so the clear hook must mirror that detach explicitly** (R35 MEDIUM) | skipped entirely when `being_destroyed_` |
| `set`/`clone` (deep copy into a store-reachable dest) | `clone_` builds children via `appendChild` → already mapped; add explicit parity coverage | mostly covered by appendChild |
| `setProperty/clearProperty` (container) | container property column / sparse bag keyed by row (D59) | atom props already store-backed |
| `select/deselect` (atom leaf only) | `bump_selection_up(parentRow, ±1)` (D59) once | hook the LEAF bit transition, NOT `updateSelection_` (R35: avoid double-count) |
| container-PROPERTY migration | fill in the adoption materialization (D73) | H1a/R32 carry-over |

**Positional moves (R35 HIGH).** `reparent_child` always *appends*, so it
cannot reproduce `prependChild`/`insertBefore`/`insertAfter` ordering.
Therefore a move is mirrored as **`remove_child(old_parent, ref)` then
the positional attach op** (`prepend_child` / `insert_child_before` /
`insert_child_after` / `append_child`) on the new parent. `reparent_child`
remains the convenience for the append-move case only.

**`swap` scope (R35 HIGH).** v0 `swap` exchanges tree position + child
lists + selection counters + properties + selectable state for two
arbitrary `Composite` siblings (`composite.C:1511`). H2 supports:
container↔container (exchange the two rows' child lists, payload,
selection_count, properties, and fix all reverse edges incl. child atoms'
`atom_parent_`) and atom↔atom (exchange the two ATOM ChildRefs' positions
in their parents + `atom_parent_`). **atom↔container swap is excluded in
H2** (not used in the molecular hierarchy) — asserted/guarded with a test
that the unsupported case is rejected, not silently mis-mirrored.

`ref(C)` = `compositeAsAtom_(C)` → `ATOM(C.getStoreIndex())`, else
`CONTAINER(C.container_row_idx_)` (allocating C's row first if unbound,
D68).

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

## D73. Adoption materialization (free-standing subtrees) — R35 BLOCKER

Free-standing containers have **no reachable store** (`Composite::
getCompositeStore_()` returns null with no System ancestor), so the
mirror cannot fire during free-standing mutation — and D56's
`migrate_subtree_from` (table→table) requires a source row that was never
allocated. So `new Molecule; m.insert(atom); m.insert(residue);
sys.insert(m)` builds a v0 subtree with **no container rows at all**.

**Decision:** adoption performs a **recursive v0→table materialization**.
`System::adoptSubtree` (extended) walks the adopted v0 container subtree
and, for every container lacking a row, allocates a row in the System
store's `ContainerTable`, fills payload from the v0 object (name/id/
insertion-code/ss-type/properties), binds `container_row_idx_`, and
builds the `ChildRef` edges (container children recursively, atom children
keyed by the now-migrated atom store index from the existing atom-slot
adoption). This is the production form of the H1a parity-test `mirror()`
helper.

- Free-standing mutation is therefore a **no-op for the mirror** (nothing
  to write); the table is built in one pass at adoption. Correct because
  free-standing subtrees are never parity-checked until rooted.
- D56 `migrate_subtree_from` remains the mechanism for **store→store**
  moves (a subtree that already has rows in some store moving to another),
  e.g. cross-System reparent. Adoption-from-free-standing uses
  materialization. Both converge on the same table shape.
- Ordering: atom-slot migration (existing `adoptSubtree`) runs first to
  produce the old→new atom-index map; materialization uses it for ATOM
  `ChildRef`s.
- D73 materialization is the **single container-adoption path** (both
  free-standing AND cross-System): it (re)builds rows from the v0 tree
  regardless of any prior row state, so the H1a `migrate_subtree_from`
  (container table→table) is **not** on the H2 adoption path — it remains
  tested infra. (Atom slots still migrate store→store as today.)

## D74. Container-row binding lifecycle (R35b NEW BLOCKER)

A bare `container_row_idx_` is meaningless without knowing WHICH store's
table it indexes — a container that moves between Systems would carry a
stale index into the wrong table. Fix:

- Bind **`{MoleculeStore* container_row_store_ = nullptr; std::uint32_t
  container_row_idx_ = 0;}`** on `AtomContainer` (12 B; O(thousands)
  containers; transitional, removed at H4).
- **Self-healing rule:** every mirror hook first resolves the container's
  current store `S = getCompositeStore_()`. If `container_row_store_ != S`
  (including the `nullptr`→first-bind and the moved-between-stores cases),
  the binding is **stale**: (re)materialize the container's row in `S`
  (D73) and rebind `{container_row_store_=S, container_row_idx_=new}`. If
  it matches, use `container_row_idx_` directly (O(1)). This makes
  cross-System moves and destroy/re-adopt **self-healing** without the
  migration walking v0; the previous store's row leaks (acceptable —
  v0 = truth, unreachable).
- **No release on destruction** (D69): the binding dies with the object;
  rows are freed wholesale at `~System`. A container detached
  (`removeChild`) but not destroyed keeps its binding; re-insertion into
  the same store reuses it, into a different store re-materializes.
- D73 adoption-materialization sets `{container_row_store_, container_
  row_idx_}` on every materialized v0 object.

## Risks / open questions (R35 dispositions)

**Resolved by this revision:**
- Risk #1 (atom-index timing): **R35 confirms the current order is safe** —
  `Atom::~Atom` calls `destroy()` (→ `Composite::destroy` → live-parent
  detach) BEFORE `release_atom()`, so `getStoreIndex()` is readable at the
  mirror point and the slot is not yet reused. **Contract pinned:** the
  live-parent detach mirror happens before `~Atom` releases the slot;
  H2a will not move slot release earlier.
- Risk #2 (flag-set ordering): resolved — flag set at start of both
  `destroy()` overloads, before child teardown (D69).
- Risk #3 (free-standing pre-adoption): resolved by D73 materialization.
- Risk #4 (selection double-count): resolved — hook the leaf atom
  select/deselect bit transition only, one `bump_selection_up(±1)`; do
  NOT hook `updateSelection_` (D70).
- Risk #5 (swap): scoped in D70 (container↔container, atom↔atom;
  atom↔container excluded + tested).

**Carried into implementation (verify with tests, not design):**

- The exact `insertParent` sibling-range rewrite mapping (allocate new
  parent row + move the range) — exercise against the PDB import paths.
- `swap`'s container↔container reverse-edge fixup completeness (every
  child atom's `atom_parent_` + every child container's
  `parent_container_idx`).
- The clear() non-auto-deletable direct-detach mirror branch.

## Decisions added

| # | Contract |
|---|---|
| D67 | Per-mutation forward mirror hooks in v0 methods (v0 first, then table); retire (leave inert) the `composite_nodes_` mirror |
| D68 | v0-container→row binding via dedicated `AtomContainer` members (NOT the `composite_handle_packed_` slot — R35 HIGH); atoms use `getStoreIndex`; lazy allocate-on-first-mirror; binding lifecycle in D74 |
| D69 | Destruction guard: `being_destroyed_` set as the FIRST line of each molecular DESTRUCTOR (+ `~Composite` backstop), NOT in `destroy()`/`clear()` — so explicit `destroy()`/`clear()` on a LIVE object still mirrors the emptying (R35b NEW HIGH); `removeChild` skips the mirror iff the PARENT is being destroyed (defuses P2.1.1) |
| D70 | Full mutation→ContainerTable-op mapping: positional moves = remove + positional-insert (not append-only `reparent_child`); `replace`/`insertParent`/`clear` non-auto-deletable/`swap` scoped; leaf-only selection hook (R35) |
| D71 | Handle-yielding `AtomIterator`/container iterators/`apply` over `ChildRef`; consumers migrate in H3 |
| D72 | H2a–H2d sub-phasing; parity asserted after every covered mutation (each sub-test exercises only the wired ops) |
| D73 | Adoption materialization: recursive v0→table build at `System::adoptSubtree` — the single container-adoption path (free-standing AND cross-System); D56 `migrate_subtree_from` not on this path |
| D74 | Container-row binding lifecycle: `{container_row_store_, container_row_idx_}` on `AtomContainer`; self-healing re-materialize on store mismatch; no release on destruction (R35b NEW BLOCKER) |

**Selection (R35b note):** v0 recursive container select selects all
descendant atoms. The mirror hooks only the per-atom leaf bit transition
(one `bump_selection_up(±1)` each), so a container-level select fans out
to N leaf hooks → N counter bumps, matching v0's per-atom counters. H2b
adds nested-container select/deselect parity tests.

## Next action

R35 → NEEDS-FIXES (flag placement, free-standing adoption, slot reuse,
positional moves, missing mutators) → revised → R35b → NEEDS-FIXES (2 NEW:
binding lifecycle, `destroy()`-on-live flag). This revision: D69 moves the
flag to the DESTRUCTORS (explicit `destroy()`/`clear()` on a live object
mirrors correctly); D74 adds the `{store,idx}` binding + self-healing
re-materialize. Codex **R35c** re-reviews; on GO, implement H2a.

---

## D71a. H2c addendum — `AtomHandle` yield type (maintainer decision: Option A)

D71 said the handle-yielding `AtomIterator` "yields `Atom` by value" but the
yield type was underspecified (no atom value-handle existed; in dual existence
`Atom` is still the heavy v0 `Composite`-derived object). Maintainer chose
**Option A (2026-05-25):** introduce a dedicated `AtomHandle` value type now,
the atom analog of `ContainerHandleBase`.

**`AtomHandle` (`include/BALL/KERNEL/atomHandle.h`)** — `{ MoleculeStore*,
u32 atom_idx, StableId stable_id }`:
- **Identity / validity uses `stable_id`, NOT a generation counter.** Unlike
  container rows (per-slot `generation`, D65), atom slots have only the
  store-wide `generation()` (D7) — and `release_atom()` does **not** bump it,
  so a coarse generation check would ABA-alias a recycled slot. Each *live*
  atom has a unique `StableId` (a reallocated slot draws a fresh one from
  `next_stable_id_alloc_`), so capturing `stable_id` at bind and comparing on
  `isValid()` is the ABA-safe identity. `isValid()` = `store && idx < size() &&
  !is_freed(idx) && stable_id(idx) == captured`.
- **Dual-existence bridge:** `getAtom() -> Atom*` via `MoleculeStore::back_ptr`
  (null if freed / after the H4 flip). Lets H3 consumers reach the v0 object
  during migration; at H4 the handle *is* the atom.
- **Size:** `{8 + 4 + 8}` → 24 B (padded). The D13 "16 B atom handle target"
  is the H4 *flipped-`Atom`* layout concern; this transient traversal handle is
  not stored per-atom in bulk, so 24 B is fine. Documented for the H4 audit.

**Traversal (`StructureQuery::atoms` / `apply`, header-only):**
- `atoms(const ContainerHandleBase& root) -> std::vector<AtomHandle>` — preorder
  walk of the ordered `ChildRef` edges (`getChild(i)`), emitting an `AtomHandle`
  for each atom child at the leaves, recursing into container children. The
  ordered edges (D57) make this reproduce the v0 `Composite` atom preorder.
- `apply(root, proc)` — preorder walk dispatching to `proc(AtomHandle)` /
  `proc(const ContainerHandleBase&)` (the table-side re-expression of
  `Composite::apply<T>`). Predicate/processor handle-by-value signatures (D61
  Class-E) are exercised here; consumers migrate in H3.

**Guardrail:** `AtomHandle_test` (identity/validity/ABA via release+realloc,
`getAtom` bridge, 24 B size assert) + an order-parity test asserting
`StructureQuery::atoms(rootHandle)` store-index sequence == the v0
`AtomIterator` preorder over the same built `System`.

Additive / dual-existence: the v0 `Composite&` traversal is untouched (deleted
at H4), no consumer is migrated (H3). Tree stays green. Codex **H2c-CR** close-
reviews design + implementation.

---

## H2c-CR code review

Verdict: **GO-WITH-FIXES**. The traversal layer is additive and the ordered-edge
walk looks correct, but `AtomHandle::getAtom()` must be made identity-aware
before H2c closes: after `release_atom(idx)` + slot reuse, an old handle is
invalid by `stable_id`, yet `getAtom()` currently checks only null / OOB /
freed and can return the *new* atom's `back_ptr` for the recycled slot. Required
fix: make `getAtom()` require the captured `stable_id` to match (or call
`isValid()` before returning `back_ptr`) and add an ABA regression asserting the
old handle's `getAtom()` stays null after reuse while the fresh handle bridges.

1. **AtomHandle:** **FLAW** — `isValid()` is ABA-safe and short-circuits
   `idx < size()` before `stable_id(idx)`, and the dangling-store contract
   matches the other handles, but `getAtom()` has an ABA false-positive because
   it does not check `stable_id_`.
2. **Traversal:** **SOUND** — `visitAtoms_` / `atoms(root)` and `apply(root,
   proc)` walk the public ordered `ContainerChildRef` edges in preorder; atom
   child `idx` is the atom-store index, container and atom children stay
   interleaved in the v0 child order, empty containers naturally contribute no
   atoms, and recursive `apply(cc, proc)` keeps the processor as an lvalue
   rather than repeatedly moving it.
3. **Encapsulation:** **SOUND** — the new headers include only public
   `moleculeStore.h`, `containerHandle.h`, and `atomHandle.h`; no private
   `ContainerRow`, `ChildRef`, `ContainerTable`, or `_moleculeStoreInternal.h`
   types leak through the API.
4. **Additive / no regression:** **SOUND** — no v0 traversal code or consumers
   are changed; the new API is header-only and currently reached only through
   the new test / explicit `StructureQuery` include path.
5. **Test adequacy:** **WEAK** — coverage exercises null/bind/equality/ABA
   validity, size, SS-layer atom preorder parity, fresh `getAtom()` bridging,
   and basic `apply` counts, but it misses the required ABA bridge assertion for
   stale `getAtom()`; postorder is out of scope, and multi-molecule/detached
   atom cases are optional for H2c.
6. **Other ABI/ODR/correctness:** **SOUND** — `BALL_EXPORT` on the inline 24 B
   value type matches the existing handle style, the 24 B size is acceptable for
   a transient traversal yield, and no additional ODR or ABI issue was found.
