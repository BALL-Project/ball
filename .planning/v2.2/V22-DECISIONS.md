# BALL 2.2 — Decision Log

**Status:** W0 design lock (in progress).
**Authored:** 2026-05-20.
**Companion:** `MILESTONE-v2.2-KICKOFF.md`.

D-decisions continue the v2.x series (v2.1 ends at D44). v2.2
starts at **D45**. These lock the W0 answers to the kickoff's
Q1–Q7 (maintainer, 2026-05-20).

---

## D45. Store owns all side-table rows; Atom/Bond dtor is a pure slot-release (Q1)

**Decision:** the `MoleculeStore` (via `MoleculeStoreSideTables`)
**owns** every side-table row — composite node, property column
cells, sparse-bag entries, selection bit. An Atom/Bond destructor
does **not** mutate the store; it only marks its own slot freed
(the existing `release_atom`/`release_bond` index-return path). No
Atom→store back-mutation runs during teardown.

**Why:** this is the structural fix for the P2.1.1 destruction-order
trap. The earlier failure was mirror calls writing *into* the store
during the `~System → ~Composite → parent->removeChild` cascade,
when the store's own lifetime was interleaved with the destruction
chain. If the store owns the rows and the handle dtor is a pure
index-release (no write-back), there is no interleaved mutation to
corrupt. Teardown order becomes: children mark slots freed → System
frees the store → store frees all rows in one pass.

**Implications:**
- `~D17Composite` / `~Atom` must NOT call any
  `mirrorToSideTable_`-style write. The slot-free is the only action.
- The store's destructor / `clear()` frees `composite_nodes_`,
  property columns, sparse bag, and `selected_bits_` wholesale.
- W1 mutation wiring writes side-table state on **forward**
  mutations (insert/setProperty/select) only — never on
  destruction.

## D46. Selection: keep parent-side counters; bitmap writes bump them (Q2)

**Decision:** keep the `Composite` parent-side selection counters
(`number_of_selected_children_`, `contains_selection_`, the
selection time stamp). Atom selection state lives in
`selected_bits_`, and a `select()`/`deselect()` on an Atom both
sets the bit AND propagates the parent-counter update up the tree
(as v0 `Composite::select_`/`deselect_` already do).

**Why:** molecule-/system-level "is anything selected here" and
"how many selected" queries are common (GUI, selection-aware
processors). Counters keep these O(1); recompute-from-bitmap would
be O(atoms) per query.

**Implications:**
- The counters move OFF the Atom (Atom has no inline Composite
  state post-flip) but stay ON the non-Atom Composites
  (Molecule/Chain/Residue/System keep their counters, via the
  side-table node or their own still-inline state — see D47).
- The Atom selection write path: `selected_bits_.set(idx)` +
  walk parents bumping counters. Single-thread-per-System per D16.

## D47. Tree-node identity preserved via `composite_nodes_`; AtomIterator unchanged (Q3)

**Decision:** Atom stays "in the composite tree." Its tree links
(parent/first_child/last_child/next/prev) move from inline pointers
into the `composite_nodes_` side table (already designed in v2.1
D22b). `D17Composite` forwards `getParent()`, `beginChildComposite`,
the `apply()` traversal, and the iterator step functions through
the side-table node. `AtomIterator` (= `CompositeIterator` filtered
by `KernelPredicate<Atom>`) therefore works **unchanged** — the
CompositeIterator still yields the Atom because the Atom still has a
tree node.

**Why:** reimplementing AtomIterator + `apply()` over store rows
would be a massive consumer-visible change (every processor uses
them). Preserving tree-node identity via the side table is the
minimal-blast-radius path. (R20 P20-5 flagged the iterator break as
a BLOCKER; this is the answer.)

**Implications:**
- `D17Composite::getParent()` etc. resolve through
  `store->composite_nodes_[handle.idx]` (the by-value
  `CompositeTopologyView` from v2.1 D31b, which yields `Composite*`).
- Iterators cache `Composite*` only, never `CompositeNode*` (D31b
  invariant — already CI-gated).
- The atoms-have-a-tree-node contract means W1 must populate
  `composite_nodes_` for every atom on insert (not lazily).

## D48. Adoption migrates full side-table state (Q4)

**Decision:** `System::adopt(Atom&)` and `adoptSubtree(AtomContainer&)`
migrate the **complete** side-table state — composite-node links,
property columns + sparse-bag entries, and the selection bit —
alongside the core SoA columns, before releasing the source slot.

**Why:** R20-3 flagged data-loss: today adoption migrates only core
columns. Post-flip, property/selection/tree state lives in the side
tables; adoption that drops them silently loses data.

**Implications:**
- W1/W2 extend the adopt/adoptSubtree migration passes.
- The batched `adoptSubtree` (already O(n) per v2.1 P4.1's CSR
  fix) gets parallel passes for the side-table segments.
- Migration is copy-then-release (same shape as the core-column
  migration), under the orphan mutex for orphan→System moves.

## D49. PropertyManager full surface forwarding (Q5)

**Decision:** `D17PropertyManager` forwards the **entire** v0
`PropertyManager` surface to the column + sparse-bag store: scalar
named properties, bit (BitVector) properties, named-property
iteration (`beginNamedProperty`/`endNamedProperty`), index access,
`set`/`get`/`swap`/`operator=`, and persistence. Nothing is
narrowed for Atom.

**Why:** the whole-surface forwarding is what D26's "preserve the
API" promise rests on for properties. Force-field + analysis code
uses named-property iteration heavily.

**Implications:**
- Named-property iteration over a column+sparse representation
  needs a **materialising adapter**: `beginNamedProperty()` yields
  a synthesized `NamedProperty` view by walking the registered
  columns (where `present_` is set for this atom) + the sparse bag.
  This adapter is the trickiest part of D49 — it must present a
  stable iteration order and the same `NamedProperty` value
  semantics as v0.
- Bit properties: the v0 `PropertyManager` BitVector maps to the
  per-atom bit columns / sparse bag (the v2.1 D23b typed
  `SparseKey` already distinguishes bit vs named).
- OBJECT/SMART_OBJECT properties: same placeholder semantics as
  v2.0 JSON (pointer identity not preserved); stay in the sparse
  bag.

## D50. Controlled API break allowed where forwarding is impractical (Q6)

**Decision (deviation from v2.1 D26):** v2.2 **is a breaking
release.** Where forwarding the v0 API through the shims is clean,
it is preserved (source-compat). Where forwarding is impractical,
perf-prohibitive, or semantically impossible post-flip, a
**documented, deliberate API break is permitted.**

**Why:** v2.1 D26 promised full preservation, but that promise was
made when v2.1 was still trying to keep the inheritance. With the
inheritance actually deleted in v2.2, some surfaces cannot be
preserved without absurd cost (e.g. returning a mutable
`Composite&` reference to inline state that no longer exists, or
APIs that hand out addresses of former inline members). The
maintainer accepts a controlled break rather than contorting the
design to preserve every signature.

**Implications:**
- v2.2 RELEASE-NOTES gets a **"Breaking changes"** section (v2.1
  had none). Each break is enumerated with the migration path.
- Breaks require explicit justification in the relevant W2/W3
  commit + a Codex-review sign-off that the break was necessary
  (not lazy). Default is still "forward if reasonably possible."
- Candidate breaks to watch (decided per-site in W2): any API
  returning `&` to a former inline Composite/PropertyManager/
  Selectable member; `Composite*`↔`Atom*` implicit upcast in
  consumer code that the helper can't cover; SIP signatures.
- A **`V22-API-BREAK-LEDGER.md`** artifact tracks every break with
  rationale + migration note (feeds the release notes).

## D51. Bond unified into a store-backed coherent representation (Q7)

**Decision:** the Bond thin-handle flip (W3) **unifies** the
currently-dual bond representation. Today bonds exist as: (a) store
`BondRecord` rows + CSR, (b) `Atom::bond_[]` inline arrays, and (c)
heap `Bond*` objects (carrying `PropertyManager`). v2.2 makes the
store the single source of truth: `Bond` becomes a store-backed
handle (like Atom), `Atom::bond_[]` is replaced by store-CSR
queries, and bond properties live in store-side bond property
columns (parallel to atom property columns).

**Why:** the dual representation is the root of multiple gaps
(V21-BOND-GRAPH-RECONSTRUCT, the P4.2 multigraph-collapse caveat,
bond-property JSON needing Bond* reconstruction). Unifying resolves
them coherently rather than patching each. It also delivers the
Bond memory win (`sizeof(Bond)` 296 → ~24 B).

**Implications:**
- `Atom::countBonds()`/`getBond(i)` resolve through the store CSR
  (which v2.1 P4.1 already made O(1) for the empty case and O(deg)
  otherwise) instead of the inline `bond_[]`.
- Bond properties get store-side columns; the P4.2 JSON path
  reads/writes them store-side (no Bond* reconstruction needed —
  closes V21-BOND-PROPERTY-JSON's awkwardness at the root).
- The store-multigraph question resolves: bonds are store rows;
  the Atom API's one-bond-per-pair is a query convention, not a
  storage constraint.
- This is the largest single piece of W3 and may warrant its own
  sub-design + Codex round.

---

## Cross-decision summary

| # | Q | Decision (1-line) |
|---|---|---|
| D45 | Q1 | Store owns rows; handle dtor = pure slot-release (fixes P2.1.1) |
| D46 | Q2 | Keep parent selection counters; bitmap writes bump them |
| D47 | Q3 | Tree-node identity via `composite_nodes_`; AtomIterator unchanged |
| D48 | Q4 | adopt/adoptSubtree migrate full side-table state |
| D49 | Q5 | `D17PropertyManager` forwards the whole surface (+ iteration adapter) |
| D50 | Q6 | **Controlled API break allowed** — v2.2 is a breaking release |
| D51 | Q7 | Bond unified into a store-backed handle (resolves dual-graph) |

---

## W1 prep (implementation consequences)

- **W1 (mutation wiring) is the prerequisite + the destruction-order
  proving ground.** Per D45, wiring writes side-table state on
  forward mutations only; the dtor path stays write-free. W1 lands
  *with Atom still inheriting v0 bases* (dual-write), so the side
  tables become the verified source of truth before W2 deletes the
  inline state.
- **D47 + D49 are the highest-complexity forwards**: tree iteration
  through the node table, and the named-property iteration adapter.
- **D50 means W2/W3 produce a `V22-API-BREAK-LEDGER.md`** as breaks
  are decided.
- **D51 (Bond unify) is the long pole of W3** and may be split.

## Next action

Codex R29 reviews this W0 design lock (D45–D51) before W1 starts —
the same gate that caught R17's BLOCKERs for v2.1. Then W1
(mutation wiring) begins.

*Authored 2026-05-20 W0.*
