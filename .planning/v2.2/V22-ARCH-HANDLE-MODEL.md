# BALL v3.0 (on the v2.2 branch) — Handle-Model Architecture

**Status:** Design direction (R30 review pending).
**Authored:** 2026-05-20, post-R29 / D52 (option A).
**Scope:** the keystone architecture for the full handle-based
redesign — the thing R29 said W0 was missing. This resolves the
"a tree node is not a C++ Composite object" conceptual hole.

This is a **direction** doc, not a full spec. It locks the model
and the phasing; per-cluster detail comes in the phase rounds.

---

## The core idea

Today: `Atom : Composite, PropertyManager, Selectable` — a heap
object (~368 B) that IS-A node in the Composite tree. All of BALL
consumes atoms as `Composite&`/`Atom&`.

v3.0: **`Atom` is a value handle** — `{ MoleculeStore* store_;
uint32_t idx_; uint32_t generation_; }`, ~16 B, copyable, **no base
classes**. All atom state lives in the store + side tables. The
handle is a typed cursor; deref goes through the store.

The hard part isn't the handle — it's that **the whole API speaks
`Composite&`**. v2.2 redesigns that surface. Per **D55 = A2**
(maintainer 2026-05-20), the redesign goes all the way to a **flat
SoA topology**: the molecular hierarchy becomes store metadata, not
a C++ object tree.

> **The entire molecular hierarchy becomes store-backed handles.**

- **`Atom` / `Bond`** — value handles `{store*, idx, generation}`.
  Their memory (O(100k+)) is the entire D13 problem.
- **`Molecule` / `Chain` / `Residue` / `SecondaryStructure` /
  `Nucleotide` / `Fragment` / `Protein` / `NucleicAcid`** — also
  value handles `{store*, container_idx, generation}` over a
  **container metadata table** in the store. O(thousands) rows;
  memory irrelevant, but unifying them onto the handle model kills
  the parallel `Composite` object hierarchy (the A2 win).
- **`System`** stays a C++ object — it **owns** the `MoleculeStore`
  (which now also holds the container metadata table + per-atom
  group columns). It is the one root object.

There is no Composite object tree for the molecular hierarchy. The
tree is **data**: per-atom `parent_container_idx` + per-container
`{kind, parent_container_idx, name, …}` rows + CSR grouping.

---

## Resolving R29's conceptual hole

### 1. Tree representation (flat SoA topology — A2)

There is **no Composite object tree** for the molecular hierarchy.
The tree is **data in the store**:

- **Per-atom column `parent_container_idx`** — the immediate
  container row each atom belongs to.
- **Container metadata table** (new store segment): one row per
  Molecule/Chain/Residue/SecondaryStructure/Nucleotide/Fragment/
  Protein/NucleicAcid — `{ kind, parent_container_idx, name,
  type-specific fields, generation }`. The container's own parent
  links it up the hierarchy; the System root is `parent == NONE`.
- **Grouping CSR** — keyed by `parent_container_idx`, rebuilt on
  dirty exactly like the existing bond CSR. "Direct atoms of
  container C" = O(deg) CSR slice; "all atoms under C" = recursive
  container-subtree walk + union.

The v2.1 `composite_nodes_` 5-link side table is **superseded** by
this flatter model (parent-id column + container table + CSR) — a
single parent index per atom + per container, not a full
sibling-linked node. Container ordering for traversal is recovered
from the CSR (and optionally made contiguous at `compact()`).

### 2. Hierarchy navigation (handles, no `Composite*`/`dynamic_cast`)

- `atom.getResidue()/getChain()/getMolecule()` walk the **container
  metadata rows'** parent chain, filtering by `kind`, and return a
  **container handle** (`Residue`/`Chain`/`Molecule` value handle).
- `atom.getParent()` returns the immediate container handle.
- There is no `Composite*` for an atom OR a container in the
  molecular hierarchy, so `dynamic_cast<Atom*>(Composite*)` and the
  whole "is this composite an atom?" question **dissolve**.
- `detail::compositeAsAtom_(Composite*)` is **removed** (premise
  gone). The CI grep gate flips to forbidding any new
  `Composite*`-in-molecular-hierarchy assumption.
- **Kind discrimination** (when a generic container handle must be
  resolved to its concrete kind) reads the metadata row's `kind`
  tag — a tagged-union/visitor pattern, not RTTI.

### 3. Iteration / visitor / processor API (the bulk of the work)

The redesign introduces a **handle-yielding traversal layer**:

```cpp
// New: atom iteration yields Atom by value (a handle), not Atom&.
for (Atom a : container.atoms()) { ... a.getPosition() ... }

// Processor dispatch: UnaryProcessor<Atom>::operator()(Atom)
// is invoked with a handle by value (was Atom&).
container.apply(atom_processor);   // walks tree, yields atom handles
```

- `AtomIterator` is **redesigned** to yield `Atom` (value), backed
  by the grouping CSR (`parent_container_idx`) + the store. It no
  longer derives from `CompositeIterator`.
- `UnaryProcessor<Atom>::operator()` signature changes from
  `(Atom&)` to `(Atom)` (by value handle). **This is the single
  largest consumer break** — every processor over atoms.
- The traversal walks the **container metadata table** (handles,
  not objects) and yields atom handles at the leaves via the
  grouping CSR. `Composite::apply<T>` for the molecular hierarchy
  is re-expressed over container handles.
- Predicates (`UnaryPredicate<Atom>`, `Expression`, `Selector`)
  take `Atom` (value/`const Atom&`-to-temporary).

A **migration shim** eases the consumer churn: `Atom` provides the
same method names (`getPosition`, `getName`, `setProperty`,
`select`, `countBonds`, `getBond`, `getParent`, …) so most consumer
*bodies* compile unchanged; only the *signatures* (`Atom&` →
`Atom`) and the RTTI/`Composite*` sites must change.

### 4. Destruction model (the P2.1.1 trap dissolves)

With atoms AND containers as handles, **there are no per-Atom or
per-container destructors** in the molecular hierarchy — only
`~System`, which frees the store wholesale (atom columns, container
table, CSRs, side tables in one pass). No object-dtor cascade, no
`removeChild` writing into a half-destroyed tree, no mid-cascade
mutation. D45's intent is achieved structurally and completely
(A2 removes even the container-side teardown writes that R29 P29-1
flagged under A1).

### 5. Property surface (controlled break per D50/D52.3-5)

`Atom::setProperty(name, value)` / `getProperty(name)` /
`hasProperty` / `clearProperty` forward to the store columns +
sparse bag (value semantics — fine). **Broken:** the
mutable-reference surface — `NamedProperty& getNamedProperty(Position)`,
`NamedPropertyIterator beginNamedProperty()`, `BitVector&` — because
a column store can't hand out a live mutable reference. Replaced by:
- `Atom::eachProperty([](const NamedProperty&){...})` visitor, and
- `Atom::propertyNames()` / value getters.
All enumerated in `V22-API-BREAK-LEDGER.md`.

### 6. Bonds (D51, coherent now)

`Bond` becomes a handle too. `Atom::bond_[]` is removed;
`countBonds()`/`getBond(i)` resolve through the store CSR.
`BondIterator` yields `Bond` handles. Bond properties live in
store-side bond columns.

### 7. SIP / Python

Off for the v2.2 handle phases (CMake proves SIP not built).
Bindings are rewritten against the handle API when PyBALL v2
happens. **Note (D54):** when the Python wrappers ARE rebuilt, they
compile with `BALL_PYTHON_WRAPPER` so handle-validity checks are
always on in scripting (clean exception, not segfault), independent
of build type.

---

## Phasing (replaces the kickoff W0–W7)

This is the largest milestone of the project. Phases:

| Phase | Name | Deliverable |
|---|---|---|
| **H0** | Research + design lock | This doc + per-area design (traversal API shape, handle validity/generation, container-atom child-list storage). Codex R30 (this gate). |
| **H1** | Handle types + store-backed reads | `Atom`/`Bond` + container handles (`Molecule`/`Chain`/`Residue`/…) as value types reading from the store / container-metadata table, *alongside* the existing objects (dual existence) so nothing breaks yet. Add the `parent_container_idx` column + container table + grouping CSR. |
| **H2** | Handle-yielding traversal API | New `atoms()`/`AtomIterator`(handle)/`apply` over the grouping CSR + container-handle traversal + the migration shim. Old `Composite&` API still present. |
| **H3** | Consumer migration (clustered) | Move modules to the handle API in dependency clusters (KERNEL → STRUCTURE → MOLMEC/QSAR → SCORING/DOCKING → FORMAT → NMR/ENERGY/SOLVATION → VIEW). Each cluster its own commit + review. PDB/naming/residue layer is the heaviest cluster (A2). |
| **H4** | Flip — delete the molecular `Composite` object tree | Once no consumer needs the object form, delete `Atom : Composite` + the container objects' inline tree state; the store metadata is the sole source of truth. `sizeof(Atom)` drops. |
| **H5** | Bond unify (D51) | Bond → handle; remove `Atom::bond_[]`; bond properties → store columns. |
| **H6** | Property break + generation guard | The property-surface break + slot-generation stale-handle detection (D54: `BALL_DEBUG` + `BALL_PYTHON_WRAPPER`). |
| **H7** | VIEW/APPLICATIONS + MSVC CI | The 54 VIEW RTTI sites + Windows EBO/sizeof verification. |
| **H8** | D13 verification + perf gates + release | Confirm ≤160 B/atom (assert), pinned-baseline gates, v2.2.0-rc1 → v2.2.0. |

Per-phase Codex review at planning + close. Expect this to be the
longest milestone by far.

---

## Open sub-questions — RESOLVED (H0)

1. **Version:** **stays v2.2** (D53). The 2.x line is the
   1.x→modern-SoA modernization band; a breaking change within it
   is justified by the line's purpose. Release notes must be loud
   about the breaks.
2. **Handle validity:** detected in **`BALL_DEBUG` builds AND the
   Python/SIP wrapper layer** (D54); zero in release C++. Needs a
   per-slot generation counter distinct from the structural one.
3. **Migration shim lifetime:** dual `Composite&`/handle API through
   H2–H3, deleted at H4 (the flip). *Lean confirmed; locked at H2
   planning.*
4. **`AtomContainer`:** under A2 (D55) it is **not** a heavy object —
   its API re-expresses on container handles + the grouping CSR.
   The container hierarchy is store metadata, not an object tree.

---

## Reality check

This is a months-long, break-the-world redesign — exactly what
option A (+ A2 flat topology) means. It is the correct end state
(the only path to D13 + a coherent SoA kernel with no parallel
object hierarchy), but it is not a quick milestone. The phasing is
built to keep the tree **green at every phase** (dual existence
H1–H3; flip only at H4 when no consumer needs the object form), so
the redesign is incremental, not a big-bang rewrite.

## Next action

H0 sub-questions resolved (D53/D54/D55). Codex **R30** reviews this
A2 architecture (does it resolve R29's NO-GO?), then H1 (handle
types + store-backed reads, dual
existence) begins.
