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

---

# Post-R29 strategic decision (2026-05-20)

## D52. Option A — full handle-based API redesign (resolves R29 NO-GO)

R29 NO-GO'd D45–D51: they answered the mechanical Q1–Q7 but not
the conceptual core — after deleting `Atom : Composite`, BALL still
passes atoms everywhere as `Composite&`/`Composite*` (iterators,
visitors, predicates, processors, `dynamic_cast`). A side-table
"tree node" is not a C++ `Composite` object.

**Maintainer decision (2026-05-20): Option A.** Commit to the full
handle-based API redesign. Atoms (and bonds) become value handles,
not C++ objects in the `Composite` hierarchy. The traversal /
visitor / predicate / iterator API surface is redesigned to operate
on handles. `Atom : Composite` inheritance is deleted.

This is the only option that actually reaches the D13 budget
(option B re-introduces a per-atom heap object; option C/D leave
D13 open). The maintainer accepts that this is an
**architecture-wide, breaking, multi-phase effort** (the R20/R29
wall is the codebase confirming the scope).

### D52.1 — Version: this is v3.0, not v2.2

A full handle redesign that breaks the public `Composite&`/`Atom&`
API across all of BALL is, by semantic versioning, a **MAJOR**
bump. **Recommendation: rename the milestone v2.2 → v3.0.** The
`v2.2` branch + planning are carried forward as `v3.0` (the work is
identical; only the label changes). Pending maintainer
confirmation; until then the branch stays `v2.2` and docs say
"v3.0 (currently on the v2.2 branch)".

### D52.2 — The handle model (keystone)

Detailed in `V22-ARCH-HANDLE-MODEL.md`. Summary:
- `Atom` / `Bond` become small **value handles** `(store*, idx,
  generation)` — copyable, ~16 B, no base classes.
- All atom state (payload + tree links + properties + selection +
  bonds) lives in the store / side tables, reached via the handle.
- **Container Composites** (Molecule/Chain/Residue/SecondaryStructure/
  Protein/NucleicAcid/Nucleotide/Fragment/System) **stay as C++
  objects** (they are O(thousands), not O(100k) — their memory is
  irrelevant). The tree is heterogeneous: containers are objects;
  atoms are handle leaves.
- Iteration / visitor / processor APIs gain handle-based forms
  (`UnaryProcessor<Atom>` invoked with an `Atom` by value);
  `AtomIterator` yields `Atom` by value.
- `detail::compositeAsAtom_` is replaced by a tree-node-kind check
  that constructs an `Atom` handle (no `dynamic_cast`).

### D52.3 — R29's 7 required decisions, resolved under A

1. **Identity model:** handle-based (D52.2). Atoms are value
   handles; container Composites stay objects.
2. **`Composite*`→`Atom*` replacement:** kind-tagged tree node +
   handle construction; no cross-hierarchy `dynamic_cast`.
3. **Iterator/traversal/visitor rewrite scope:** full — the
   `Composite` traversal templates + `UnaryProcessor<Atom>`
   dispatch + AtomIterator are redesigned for handles. This is the
   bulk of the work and the source of the consumer breaks.
4. **Destruction-mode rules:** D45 refined — the store owns all
   atom rows; container teardown frees its child *handle list* (no
   per-atom object dtor cascade, because atoms aren't objects).
   The P2.1.1 trap dissolves: there are no per-Atom destructors
   running mid-cascade.
5. **Property API break list:** `NamedProperty&` /
   `NamedPropertyIterator` / `BitVector&` mutable-reference returns
   are **broken** (replaced by value accessors + a handle-scoped
   property view). Enumerated in `V22-API-BREAK-LEDGER.md`.
6. **Heterogeneous-tree storage:** containers inline; atoms in
   `composite_nodes_`; the traversal API resolves both via a
   uniform node abstraction that yields either a `Composite*`
   (container) or an `Atom` handle (atom leaf).
7. **SIP/Python policy:** Python/SIP stays **off** for the v3.0
   handle phases (PyBALL v2 is a separate post-v3.0 effort); CI
   proves SIP is not built. The bindings are rewritten when PyBALL
   v2 happens, against the handle API.

### D52.4 — D45–D51 status under A

- **D45** (store owns rows) — REINFORCED + extended: with atoms as
  handles, there are no per-Atom dtors at all; the trap dissolves.
- **D46** (selection counters) — kept, on container Composites
  (which stay objects); atom selection in `selected_bits_` bumps
  container counters.
- **D47** (AtomIterator unchanged) — SUPERSEDED: AtomIterator is
  redesigned to yield handles. "Unchanged" was the wrong goal.
- **D48** (adopt migrates side-table state) — kept.
- **D49** (full PropertyManager forwarding) — SUPERSEDED by D52.3-5:
  the mutable-reference surface is a controlled break, not a
  transparent forward.
- **D50** (controlled break) — ESCALATED: v3.0 is openly a breaking
  release; the ledger now has an a-priori expected-break list
  (D52.3-5).
- **D51** (Bond unify) — kept, now coherent (Bond is also a handle).

### D52.5 — Phasing reset for the redesign scale

The W0–W7 phase plan in the kickoff is replaced by the
handle-redesign phasing in `V22-ARCH-HANDLE-MODEL.md` (research →
handle type → traversal/visitor API → consumer migration in module
clusters → property surface → bond unify → VIEW → release). This is
the largest milestone of the project; expect a research phase + many
review rounds (R30+).

*Post-R29 decision recorded 2026-05-20.*

---

# H0 confirmations + the container fork (2026-05-20)

## D53. Milestone stays v2.2 (no v3.0 rename)

**Decision (maintainer):** keep the milestone labelled **v2.2**,
overriding D52.1's v3.0-rename recommendation. Rationale: the
**entire 2.x line is a gradual shift from the 1.x architecture to a
modernized 2.x SoA kernel.** The handle redesign is a continuation
of that arc, not a separate major era — so a breaking change within
2.x is justified by the line's stated purpose. (Semver purists
would call it 3.0; the project treats 2.x as the modernization
band.)

**Implication:** v2.2 RELEASE-NOTES must be very loud about the
breaking changes (the `V22-API-BREAK-LEDGER.md` + a prominent
"Breaking changes" section), since the minor-version label
under-signals the magnitude.

## D54. Handle-validity check: BALL_DEBUG **and** Python wrappers

**Decision (maintainer):** stale-handle detection
(`handle.generation == store slot-generation`) is compiled in:
- always in `BALL_DEBUG` builds (developer safety), AND
- always in the **Python/SIP wrapper layer**, regardless of build
  type — scripting is interactive and error-prone; a stale handle
  in a Python session must raise a clean exception, not segfault.

Release C++ builds (non-debug, non-Python) pay zero — the
hot-path deref has no check.

**Implication:** the handle deref has a `#if defined(BALL_DEBUG) ||
defined(BALL_PYTHON_WRAPPER)` guard around the generation compare.
The Python wrapper layer is compiled with `BALL_PYTHON_WRAPPER`
defined. This needs a slot-generation counter on the store (per
freed/reused slot), distinct from the structural `generation_`
(per v2.1 D44's correction). Filed into H6 (generation guard).

## D55. OPEN — container model: A1 (containers stay objects) vs A2 (hierarchy as store metadata)

The maintainer's "do we still need atom containers?" + "ranges vs
array of indices" questions open a fork that `V22-ARCH-HANDLE-MODEL.md`
(written assuming **A1**) does not yet resolve. **This is the next
design decision; H1 should not start until it's locked.**

### Grounding facts (from the code, 2026-05-20)

- The store has **no ordering contract** — free-list slot reuse
  means store-index order ≠ composite-traversal order. "Ranges of
  atom indices" per container would require a NEW ordering contract
  + reorder-on-mutation (or reorder-at-compact).
- `atom.getResidue()/getChain()/getMolecule()` walk UP the parent
  chain (ancestor-by-type search).
- `AtomContainer` is a rich object: `getAtom(pos)`, `getAtom(name)`,
  `countAtoms`, `countAtomContainers`, `getSuperAtomContainer`,
  `append/prepend/insert`, the atom + container iterators.

### A1 — Containers stay C++ objects; atoms are handle leaves

(What `V22-ARCH-HANDLE-MODEL.md` currently assumes.)
- Molecule/Chain/Residue/System remain `Composite` objects.
- Each container holds its atom children as **a per-container array
  of store indices** (works with the free-list store; no ordering
  contract needed).
- `atom.getParent()` → the owning container object via the atom's
  tree-link record.
- **Pro:** preserves the `AtomContainer` API surface; smaller blast
  radius; the container hierarchy code (PDB build, naming, residue
  semantics) is largely unchanged.
- **Con:** keeps a parallel object hierarchy alongside the SoA
  store; not the "fully flat" modern end state. Atom grouping is an
  array-of-indices per container (fine, but not range-cheap).

### A2 — Hierarchy as store metadata; containers become views

(What "do we still need atom containers?" points at.)
- The hierarchy becomes **per-atom group-id columns**: each atom
  row carries `molecule_id`, `chain_id`, `residue_id` (or a single
  `parent_container_id`). A small side table holds container
  metadata (residue name, chain id, …).
- "Atoms of residue R" = a **CSR-style grouping** keyed by
  `residue_id` (rebuilt on dirty, exactly like the existing bond
  CSR) — OR a contiguous **range** `[begin_R, end_R)` IF the store
  adopts an ordering contract (reorder at `compact()` so
  traversal order == store order).
- Container "objects" shrink to lightweight **handles/views** over
  the metadata table; `AtomContainer` as a heavy object largely
  disappears.
- **Pro:** the genuinely modern flat-SoA topology (cf. MDAnalysis /
  OpenMM topology); no per-atom-adjacent object hierarchy; cleanest
  end state for the 2.x modernization arc.
- **Con:** the **largest** break — kills the container object
  hierarchy too, not just atom leaves; rewrites PDB build, naming,
  residue/chain semantics, and every `getResidue()`-style consumer.
  Substantially more than A1.

### Ranges vs array (sub-answer, applies to A2)

- **Per-atom group-id columns + CSR grouping** is the
  free-list-friendly choice — no ordering contract, mirrors the
  bond CSR already in the store. **Recommended base.**
- **Contiguous ranges** are an *optional* `compact()`-time
  optimization (reorder so a container's atoms are contiguous →
  O(1) range, cache-optimal). Not a correctness requirement; layer
  it on later if profiling wants it. Making ranges the *base* would
  force expensive mid-array reordering on every atom insert.

### Recommendation

**A2 is the right end state** for the 2.x modernization arc (it's
what the question is really asking for), but it is materially bigger
than A1 and rewrites the container/PDB/naming layer. **A1 is a
valid, much-lower-risk intermediate that still delivers the D13 atom
memory win** (the container objects are O(thousands) — they don't
move the D13 needle; only the atom handle does).

**Pragmatic path:** do **A1 first** (atom/bond handles; containers
stay objects) — this captures the entire D13 budget win, since the
budget is per-atom and containers are negligible. Then evaluate A2
(flatten the hierarchy) as a *separate* follow-on (v2.3 / later)
once the handle infrastructure is proven. A2 doesn't help D13; it's
an architecture-cleanliness + traversal-perf play.

Grouping storage when A2 happens: per-atom group-id columns + CSR
(base); contiguous ranges as a compact()-time optimization.

**This needs the maintainer's call: A1-now-A2-later, or A2-now.**

### D55 RESOLVED — A2 (flat SoA topology, now)

**Decision (maintainer 2026-05-20): A2.** Commit to the full flat
SoA topology in v2.2. The molecular hierarchy becomes store
metadata, not a C++ object tree. This is the genuine modern end
state for the 2.x arc; the maintainer accepts the larger break
(container/PDB/naming/residue layer) to avoid carrying a parallel
object hierarchy.

**Locked A2 model:**
- **System** stays a C++ object — it owns the `MoleculeStore`.
- **`MoleculeStore` gains a container metadata table:** one row per
  Molecule/Chain/Residue/SecondaryStructure/Nucleotide/Fragment/
  Protein/NucleicAcid — `{ kind, parent_container_idx, name,
  type-specific fields, generation }`. O(thousands) rows; memory
  irrelevant.
- **Per-atom columns gain `parent_container_idx`** (the immediate
  container row). The hierarchy is reconstructed by walking
  container rows' `parent_container_idx` up to the System root.
- **Container classes become value handles/views** —
  `Molecule`/`Chain`/`Residue`/… are `{ store*, container_idx,
  generation }` handles over the metadata table, same pattern as
  `Atom`/`Bond`. No heavy `Composite` object per container.
- **Grouping = CSR by `parent_container_idx`** (base; mirrors the
  bond CSR). "Direct atoms of container C" = CSR lookup; "all atoms
  under C" = recursive container-subtree walk + union. Contiguous
  ranges remain an optional `compact()`-time optimization (reorder
  so a container's atoms are contiguous), NOT a base requirement
  (the free-list store has no ordering contract).
- **`AtomContainer`** as a heavy object disappears; its API
  (`getAtom`, `countAtoms`, `append/insert`, sub-container nav)
  re-expresses on the container-handle + store grouping.
- **`atom.getResidue()/getChain()/getMolecule()`** = walk the
  container-row parent chain filtering by kind (was: walk Composite
  parent pointers).

**Consequences vs the A1-assuming `V22-ARCH-HANDLE-MODEL.md`:**
- The whole molecular hierarchy (not just atoms/bonds) becomes
  store-backed handles. `Composite` as a base class largely exits
  the molecular kernel (System may keep a thin form as store owner).
- Bigger consumer break: every `Molecule*`/`Chain*`/`Residue*`
  consumer + PDB build + naming. Enumerated in the API-break ledger.
- Does not change the D13 atom-memory math (containers were already
  negligible); A2's value is architectural coherence + traversal
  perf + killing the dual hierarchy, per the maintainer's modern-2.x
  goal.

`V22-ARCH-HANDLE-MODEL.md` is updated to the A2 model (container
section rewritten). R30 reviews the A2 architecture.

*H0 confirmations + container fork recorded 2026-05-20. D55
resolved A2 2026-05-20.*
