# BALL v2.2 — H0 Design Addendum (the 7 R30 contracts)

**Status:** design lock addendum (R31 review pending).
**Authored:** 2026-05-20, post-R30.
**Resolves:** the 7 contracts R30 said must be locked before H1.
**Companion:** `V22-ARCH-HANDLE-MODEL.md` (A2 architecture),
`V22-DECISIONS.md` (D45–D55).

R30 confirmed A2 resolves R29's conceptual hole but flagged 7
under-specified contracts. This addendum locks each. New decisions
are D56–D62.

---

## D56. Container orphan store (detached-construction lifecycle)

**Contract (R30 BLOCKER #1):** `new Molecule; m.insert(atom);
sys.insert(m)` must keep working. Detached containers + their
atoms live in a **process-global orphan store**, exactly mirroring
the existing atom orphan store (`MoleculeStore::orphanStore()`).

- The orphan store holds **both** orphan atoms (already does) **and**
  orphan container rows (new). A detached `Molecule` handle's
  `container_idx` points into the orphan store's container table.
- `System::insert(container_handle)` **migrates the whole detached
  subtree** — container rows + their atom rows + parent links +
  properties + selection — from the orphan store into the System's
  store (extends `adoptSubtree`, already O(n) per v2.1 P4.1).
- The orphan container store is mutex-protected like the orphan
  atom store (D16 / V21-ORPHAN-MUTATOR-LOCK pattern).
- A container handle is a `{store*, container_idx, generation}`;
  when detached, `store*` is the orphan store; after insert, it's
  the System store. The handle is rebound on migration (same as
  atom `migrateTo_`).

**Why a store, not a builder object:** keeping the handle model
uniform (orphan store = "the System you don't have yet") avoids a
second representation. Construction code (`new Molecule`) returns a
handle bound to the orphan store; nothing in consumer code changes
shape.

## D57. Ordered child edges (deterministic traversal)

**Contract (R30 BLOCKER #2):** child order must be deterministic
and match today's insertion/tree order — `parent_container_idx`
alone is insufficient (free-list slot order ≠ tree order).

**Decision:** each container row owns an **ordered child list** —
a `std::vector<ChildRef>` where `ChildRef = { kind:u8, idx:u32 }`
addresses either a child container row or a child atom row. This is
the explicit edge/order structure R30 required.

- Preorder/postorder `apply` walks each container's `ChildRef`
  vector in order → reproduces today's `first_child_`/`next_`
  traversal exactly.
- `insert`/`append` push to the vector; `prepend`/`insertBefore`/
  `insertAfter` splice at the right ordinal (the v0 sibling-pointer
  semantics become vector ops).
- `parent_container_idx` (on atoms + container rows) is the reverse
  edge for upward navigation (`getParent`, `getResidue`…).
- The **grouping CSR** (D55) is derived/cached from these edges for
  fast "all atoms under C"; it is an optimization layer, not the
  source of truth — the `ChildRef` vectors are.
- Contiguous store ranges (D55 optional) remain a `compact()`-time
  optimization layered on top.

**Cost:** one small vector per container (O(thousands) containers),
negligible. The ordered-edge model is the v0 tree, just stored as
indexed edges instead of pointers.

## D58. Per-kind container row schema (tagged payload)

**Contract (R30 HIGH #3):** the `{kind, parent, name, …}` sketch is
too vague. Each container kind gets a typed payload.

```
ContainerRow (common):
  kind                : ContainerKind     // Molecule/Chain/Residue/...
  parent_container_idx: u32 (NONE = root child)
  name                : string-pool offset
  children            : vector<ChildRef>  (D57)
  property_ref        : index into container property columns (D59)
  selection_count     : u32               (D60)
  generation          : u32
  payload             : KindPayload (tagged union, below)

KindPayload by kind:
  Molecule          : {}
  Protein           : { id : string }
  Chain             : { id : string }
  Residue           : { id : string, insertion_code : char,
                        residue_flags : u16 (amino-acid/terminal/...) }
  SecondaryStructure: { type : u8 }
  Nucleotide        : { id : string, insertion_code : char }
  NucleicAcid       : { id : string }
  Fragment          : {}
```

- Container **properties** (PropertyManager on containers) get their
  own column registry + sparse bag in the store (parallel to the
  atom property columns, D23b) — D59.
- Container **methods** that compute (Residue torsions, terminal
  predicates, `Chain::getNTerminal`, `Protein::getResidueByID`)
  become **handle methods** querying the row payload + ordered
  children + atom data. `getResidueByID` / `getAtom(name)` do a
  linear scan over ordered children (same complexity as today's
  tree scan) — D61 adds an index only if profiling demands.

## D59. Container property + selection storage

**Contract (R30 HIGH #3 + MEDIUM #6):** containers carry
PropertyManager bags + selection counters + timestamps. Under A2
those objects are gone.

- **Container properties:** a `container_property_columns` registry
  + sparse bag in the store, keyed by `container_idx` — identical
  mechanism to the atom property columns (v2.1 D23b). Full
  PropertyManager surface forwards through container handles (D49
  applies to containers too; mutable-reference surface is the same
  controlled break).
- **Selection counters (D46):** `selection_count` on each container
  row. An atom `select()` sets its bit AND walks
  `parent_container_idx` up, bumping each container row's
  `selection_count`. "Is anything selected in this molecule" =
  `row.selection_count > 0` (O(1)). This is D46's "counters on
  containers" made concrete now that containers are rows.
- **Timestamps:** `modification_stamp` / `selection_stamp` were
  per-Composite. Decision: store a single per-System modification
  generation + an optional per-container `selection_stamp` column
  ONLY if a consumer needs per-container staleness (VIEW does for
  redraw). Default: per-System stamp; add the column in H7 (VIEW)
  if needed. The fine-grained per-Composite stamp is a **documented
  break** (ledger) unless a consumer requires it.

## D60. Dual-existence source-of-truth rule + parity (H1–H3)

**Contract (R30 HIGH #4):** during H1–H3 the v0 objects and the new
store tables coexist. Divergence risk is the top execution hazard.

**Rule:** through H1–H3, **the v0 object tree remains the source of
truth; the store tables are a verified mirror.** Every forward
mutation writes the object tree first (unchanged v0 path), then
mirrors into the store tables. Reads in new handle code go to the
store tables. A **parity test** (`HierarchyParity_test`) asserts,
after each mutation-heavy operation, that the store tables exactly
reproduce the object tree (structure + order + properties +
selection).

- The mirror writes are **forward-only** (per D45 — never during
  destruction). Container/atom teardown frees store rows wholesale,
  no per-object mirror-on-destroy.
- At **H4 (the flip)**, the direction inverts in one atomic step:
  store tables become the source of truth, the object tree is
  deleted, and the parity test is retired (nothing to compare).
- The mirror is **complete-hierarchy** (R30's warning): every
  insert/remove/splice/swap/clear/setProperty/select on any
  container or atom mirrors. H2 enumerates the full mutation set
  (the v0 `Composite` mutation methods + `AtomContainer` +
  `PropertyManager` + `Selectable`); H3 migrates readers.

This is heavier than v2.1's atom-only side-table maintenance, and
it is the crux execution risk. The parity test + forward-only rule
are the guardrails.

## D61. Pointer-returning API break policy (ledger seed)

**Contract (R30 HIGH #5):** APIs returning `Atom*`/`Bond*`/
`Molecule*`/`Chain*`/`Residue*`/`AtomContainer*`/`Composite*` can't
return raw pointers to objects that no longer exist post-flip.

**Policy:**
- Methods that return a single related entity (`getResidue()`,
  `getAtom(name)`, `getNTerminal()`, `getParent()`) return the
  corresponding **value handle** (`Residue`, `Atom`, …). A null
  result is a **null handle** (`{store=nullptr}`, `bool`-testable),
  not a `nullptr`. This is a **signature break** (`Residue*` →
  `Residue`) but most call sites (`if (res) res->...`) compile with
  the handle's `operator bool` + `operator->`-style accessors.
- Methods returning mutable references into former inline state
  (`NamedProperty&`, `BitVector&`) are **broken** → visitor / value
  accessors (D49).
- Every break is enumerated in **`V22-API-BREAK-LEDGER.md`** with a
  migration note, seeded now with: the `*`→handle return changes,
  the property mutable-ref removals, `Composite*` upcasts, and the
  per-Composite timestamp removal (D59).
- Per-break Codex sign-off at the W2/W3/H4 commits (D50).

## D62. Persistence (JSON) of the flat topology

**Contract (R30 MEDIUM #7):** K0.6 JSON currently serializes atoms
+ bonds + per-atom/molecule/system property bags + the Composite
*object* tree (molecules → atom_indices). Under A2 the topology is
store data.

**Decision:** the System JSON schema gains a **`containers`**
section — the container rows (`kind`, `parent`, `name`, payload,
ordered `children`, properties) — replacing the molecules/
atom_indices encoding. The loader rebuilds the container table +
edges + atom `parent_container_idx` directly (no Composite object
construction). Schema **MAJOR** bump to 2 (this is a format break —
v2.1 readers cannot read v2.2 topology). Validation: no cycles, no
orphan parents, every atom's parent resolves. Lands in H6 alongside
the property break. v2.1→v2.2 JSON is a one-way upgrade (a
converter, like K0.6.4 v1→v2, is a v2.2 backlog item if needed).

---

## Phasing refinement (R30 P30-6: split H1/H3)

R30 said H1 + H3 are too coarse. Refined:

| Phase | Was | Now |
|---|---|---|
| **H1a** | (part of H1) | container row schema + container table + ordered-edge `ChildRef` storage + container orphan store — store-side only, no consumer change, parity test skeleton. |
| **H1b** | (part of H1) | Atom/Bond/container value-handle TYPES reading from the store, alongside v0 objects (dual existence; v0 = source of truth per D60). |
| **H2** | traversal API | handle-yielding iterators + `apply` over `ChildRef` edges + full mutation-mirror set + `HierarchyParity_test` green. **Carry-overs from H1a (R32):** (a) container-PROPERTY migration in `migrate_subtree_from` (H1a moves topology+payload+selection only; properties deferred) + a parity test for it; (b) the mutation-mirror wiring MUST use `ContainerTable::reparent_child` (remove-before-add) for moves/splices, never the bare attach ops, to avoid stale old-parent edges. |
| **H3a** | consumer migration | KERNEL + STRUCTURE clusters. |
| **H3b** | consumer migration | MOLMEC/QSAR/SCORING/DOCKING. |
| **H3c** | consumer migration | **FORMAT (PDB/naming/residue — the heavy one)** + NMR/ENERGY/SOLVATION. |
| **H4** | the flip | store becomes source of truth; delete the molecular Composite object tree; retire parity test. |
| H5–H8 | (unchanged) | Bond unify; property break + generation guard; VIEW + MSVC CI; D13 verify + release. |

## A1-first de-risk — explicitly declined

R30 (and R29) offered A1-first as a de-risk. **Maintainer chose
A2-now (D55).** This addendum keeps A2 but front-loads the risk
into H1a (a non-invasive store-side prototype with parity tests)
before any consumer migration — capturing R30's "narrow H1 to a
prototype/parity gate" guidance without falling back to A1.

## Decisions added

| # | Contract |
|---|---|
| D56 | Container orphan store (detached construction) |
| D57 | Ordered child-edge `ChildRef` lists (deterministic traversal) |
| D58 | Per-kind container row tagged payload schema |
| D59 | Container property columns + selection counters + timestamp policy |
| D60 | Dual-existence: v0 = source of truth H1–H3, parity test, flip at H4 |
| D61 | Pointer→handle return break policy + ledger seed |
| D62 | JSON `containers` section, schema MAJOR bump to 2 |

## Next action

Codex **R31** reviews this addendum (do D56–D62 close R30's 7
contracts + is H1a now a buildable, bounded first step?). On GO,
H1a (container table + edges + orphan store + parity skeleton)
begins.
