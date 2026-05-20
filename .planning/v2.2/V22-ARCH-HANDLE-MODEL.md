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
`Composite&`**. v3.0 redesigns that surface. The redesign is
bounded by one principle:

> **Containers stay objects; atoms/bonds become handles.**

The molecular hierarchy splits cleanly by cardinality:
- **Containers** — System, Protein, NucleicAcid, Molecule, Chain,
  Residue, SecondaryStructure, Nucleotide, Fragment. There are
  O(thousands) of these; their per-object memory is irrelevant.
  **They stay C++ `Composite` objects, unchanged.**
- **Leaves** — Atom (O(100k+)), Bond (O(100k+)). These become
  **value handles**. Their memory is the entire D13 problem.

So the tree is **heterogeneous**: interior nodes are `Composite`
objects; leaf nodes are atom handles referenced by their parent
container's child list.

---

## Resolving R29's conceptual hole

### 1. Tree representation (heterogeneous)

A container `Composite` keeps its inline child/sibling pointers
**for its container children** (Molecule→Chain→Residue…). But its
**atom children** are stored as a list of atom indices (a
`std::vector<uint32_t>` into the store) rather than as `Composite*`
child pointers.

`composite_nodes_` (the v2.1 side table) is **repurposed**: it's
the per-atom tree-link record (parent container + position), so an
atom handle can answer `getParent()` → the owning container
`Composite*`. Atoms are always leaves (D52: assert no atom
children), so an atom node needs only `{ parent_container_ptr,
sibling_index }`, not the full 5-link node. This shrinks
`CompositeNode` for the atom case.

### 2. `Composite*` → atom: kind-tagged, no `dynamic_cast`

There is no `Composite*` for an atom. Code that asked
"is this composite an atom?" is rewritten per its real intent:
- **Traversal that wants atoms** → uses the atom-yielding iterator
  (below), never sees a `Composite*` for an atom.
- **A `Composite*` that might be a container OR an atom-parent** →
  containers are still `Composite*`; atoms are never `Composite*`,
  so the question dissolves.

`detail::compositeAsAtom_(Composite*)` is **removed** (its premise
— atoms are Composites — is gone). The CI grep gate flips to
forbidding `Composite*`-to-atom assumptions.

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
  by the container's atom-index list + the store. It no longer
  derives from `CompositeIterator`.
- `UnaryProcessor<Atom>::operator()` signature changes from
  `(Atom&)` to `(Atom)` (by value handle). **This is the single
  largest consumer break** — every processor over atoms.
- `Composite::apply<T>` keeps working for container types; a new
  atom-aware traversal walks containers (objects) and yields atom
  handles at the leaves.
- Predicates (`UnaryPredicate<Atom>`, `Expression`, `Selector`)
  take `Atom` (value/`const Atom&`-to-temporary).

A **migration shim** eases the consumer churn: `Atom` provides the
same method names (`getPosition`, `getName`, `setProperty`,
`select`, `countBonds`, `getBond`, `getParent`, …) so most consumer
*bodies* compile unchanged; only the *signatures* (`Atom&` →
`Atom`) and the RTTI/`Composite*` sites must change.

### 4. Destruction model (the P2.1.1 trap dissolves)

With atoms as handles, **there are no per-Atom destructors**.
Container teardown frees its atom-index list (a vector clear) +
tells the store to release those slots. No object-dtor cascade, no
`removeChild` writing into a half-destroyed tree, no mid-cascade
mutation. D45's intent is achieved structurally.

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

Off for v3.0 (CMake proves SIP not built). Bindings rewritten
against the handle API when PyBALL v2 happens (post-v3.0).

---

## Phasing (replaces the kickoff W0–W7)

This is the largest milestone of the project. Phases:

| Phase | Name | Deliverable |
|---|---|---|
| **H0** | Research + design lock | This doc + per-area design (traversal API shape, handle validity/generation, container-atom child-list storage). Codex R30 (this gate). |
| **H1** | Handle type + store-backed reads | `Atom`/`Bond` as value handles reading from store/side-tables, *alongside* the existing object (dual existence) so nothing breaks yet. |
| **H2** | Handle-yielding traversal API | New `atoms()`/`AtomIterator`(handle)/`apply` over handles + the migration shim. Old `Composite&` atom API still present. |
| **H3** | Consumer migration (clustered) | Move modules to the handle API in dependency clusters (KERNEL → STRUCTURE → MOLMEC/QSAR → SCORING/DOCKING → FORMAT → NMR/ENERGY/SOLVATION → VIEW). Each cluster its own commit + review. |
| **H4** | Delete `Atom : Composite` | Once no consumer needs the object form, delete the inheritance + inline state. `sizeof(Atom)` drops. |
| **H5** | Bond unify (D51) | Bond → handle; remove `Atom::bond_[]`. |
| **H6** | Property break + generation guard | The property-surface break + slot-generation stale-handle detection. |
| **H7** | VIEW/APPLICATIONS + MSVC CI | The 54 VIEW RTTI sites + Windows EBO/sizeof verification. |
| **H8** | D13 verification + perf gates + release | Confirm ≤160 B/atom (assert), pinned-baseline gates, v3.0.0-rc1 → v3.0.0. |

Per-phase Codex review at planning + close. Expect this to be the
longest milestone by far.

---

## Open sub-questions for the maintainer (H0)

1. **Version rename v2.2 → v3.0?** (D52.1) A break-everything
   redesign is a MAJOR bump. Recommend renaming now (relabel the
   branch + planning); the work is identical.
2. **Handle validity:** should a stale `Atom` handle (slot freed /
   store realloc'd) be detected always (cost per deref), in
   `BALL_DEBUG` only, or never (caller contract)? *Lean: debug-only
   slot-generation check (the v2.1 D44/D25 thread, now with the
   right semantics).*
3. **Migration shim lifetime:** keep the dual `Composite&` atom API
   through H2–H3 (eases migration) then delete at H4 — or break
   hard at H1? *Lean: dual through H3, delete at H4.*
4. **`AtomContainer` itself:** it's a container (stays an object),
   but it currently stores atoms as Composite children. Confirm its
   child storage migrates to the atom-index list (H1/H2).

---

## Reality check

This is a months-long, break-the-world redesign — exactly what
"option A" means. It is the correct end state (it's the only path
to D13 + a coherent SoA kernel), but it is not a quick milestone.
The phasing above is built to keep the tree **green at every
phase** (dual existence H1–H3; flip only at H4 when no consumer
needs the object form), so the redesign is incremental, not a
big-bang rewrite.

## Next action

Maintainer confirms: (a) the v3.0 rename, (b) sub-questions 2–4.
Then Codex R30 reviews this architecture (does it actually resolve
R29's NO-GO?), and H1 (handle type + store-backed reads, dual
existence) begins.
