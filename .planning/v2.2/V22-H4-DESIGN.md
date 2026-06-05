# V22-H4-DESIGN — The Flip (v0 retirement + canonical-name reconciliation)

**Status:** DRAFT-R2 (post-Codex H4-DR round 1 NOT-GO findings applied).
**Authored:** 2026-06-04 immediately post-H3d closing-CR round 4 GO at
`4e71e63f4`.
**Predecessor:** H3d (closed); see `V22-H3d-DESIGN.md` + D-H3d.CLOSE.
**Companion:** `.planning/v2.x/V2X-ROADMAP.md` §2 H4 row.
**DR needed:** YES — Codex H4-DR round 2 against this R2 draft.

## R1 -> R2 change summary

R1 was structurally wrong on three counts: System cannot be "deleted",
Atom cannot be deleted in one commit, and bridge removal at commit 13
strands JSON/PDB-origin/property persistence. Codex H4-DR round 1
findings folded in:

- D-H4.1 reordered: front-loaded enabling commits (iterator/apply/
  property persistence/AtomContainer) BEFORE any v0 class deletion;
  System refactor-not-deletion.
- D-H4.2 reverse-alias lifetime extended (deprecate v2.3, remove v2.4).
- D-H4.3 HierarchyParity replaced THEN retired (table-only invariant
  + JSON round-trip tests stand in first).
- D-H4.4 bridge removal moved AFTER JSON/PDB-origin rewrite.
- D-H4.6 + new D-H4.11 split: property persistence API-break is its
  own decision; consumer migration is not enough.
- 5 new decisions D-H4.11..D-H4.15 cover Composite::apply<T>,
  Composite-backed iterators, JSON load/save rewrite, PDBAtom +
  FORMAT finalization, and AtomContainer fate.
- 16-commit plan replaced by 15-commit plan front-loading enablers.
- LOW: companion-doc path corrected (`.planning/v2.x/V2X-ROADMAP.md`).

---

## Why H4

Through H1a–H3d the v0 `Composite`-derived `Atom`/`Bond`/molecular
container objects have been the source of truth, with the
MoleculeStore table mirroring them under the dual-existence
invariant (D60). H4 inverts that:

- `Atom` is no longer `: public Composite, public PropertyManager,
  public Selectable`. The polymorphic heavy object becomes a
  trivial value handle.
- The molecular container objects' inline tree state is deleted;
  the container table (H1a + HCP-1 role columns) becomes the sole
  source of truth.
- `System` survives as the C++ store owner (D55) but loses its
  `: public AtomContainer, public Composite` bases.
- `HierarchyParity_test` is retired AFTER table-only invariant +
  JSON round-trip tests stand in for it.
- `sizeof(BALL::Atom) ≤ 32 B` and `sizeof(BALL::Bond) ≤ 32 B` are
  asserted on the H4 build compiler (cross-compiler verification
  at H8).
- Canonical names reconcile: `AtomHandle`/`BondHandle`/`*Handle`
  rename to `Atom`/`Bond`/`...` per the D63 audit; reverse aliases
  let consumer code that adopted the handle name continue compiling
  through v2.3, deprecated removal at v2.4.

H4 is the breaking commit point of v2.2. Every consumer cluster
migrated through H3a–H3d is the precondition; H4 finishes the
remaining shared infrastructure (iterators, apply, JSON, PDB-origin,
AtomContainer) and THEN deletes the v0 classes.

## Decisions

### D-H4.1 (R2) — Front-loaded enabling commits, then atomic class deletions

The 15-commit sequence below front-loads the shared infrastructure
that the v0 deletion depends on. Per-class deletions happen ONLY
after every consumer/iterator/JSON path can target the handle/table
representation without the v0 bridge.

Bottom-up deletion order *within* the molecular hierarchy (D-H4.1
R1) was wrong because shared surfaces (AtomContainer, iterators,
apply, JSON) cross-cut every class. Correct order: collapse the
shared surfaces FIRST, then collapse the per-class wrappers.

### D-H4.2 (R2) — Reverse-alias strategy with extended deprecation

For each v0 class deleted, the corresponding `*Handle` is renamed to
the canonical name. The OLD `*Handle` name is kept as a `using` alias
through the v2.3 cycle (deprecated, with `[[deprecated]]` attribute);
removed at v2.4 cycle open. Downstream code that adopted
`AtomHandle` during H3a–H3d has the v2.2 → v2.3 → v2.4 window to
migrate, matching the v2.x band's release-cadence (V2X-ROADMAP §4a).

```cpp
// After H4: include/BALL/KERNEL/atom.h
class BALL_EXPORT Atom { ... };          // was AtomHandle
[[deprecated("Use BALL::Atom; AtomHandle removed at v2.4")]]
using AtomHandle = Atom;                 // through v2.3; gone at v2.4
```

### D-H4.3 (R2) — HierarchyParity replaced then retired

Codex H4-DR R1 MEDIUM: retiring parity at commit 1 with "no new test
surface" strips the only broad topology oracle too early.

Replacement plan:
- Commit 1 adds **table-only topology invariant test**
  (container-row parent/ChildRef consistency, no orphan leak) +
  **JSON round-trip test** (load → save → load equality on the
  store's full state). These run on every H4 commit alongside the
  existing test surface.
- Parity test is RETIRED only at the commit IMMEDIATELY BEFORE the
  first v0 class is deleted (i.e. just before commit 8 in the new
  plan).

### D-H4.4 (R2) — Dual-existence bridge removal at the end

The bridge methods (`MoleculeStore::back_ptr(i)`, `bond_back_ptr(i)`,
`bond_idx_of(const Bond*)`, `bond_sid_of(const Bond*)`) are removed
in commit 12, AFTER:
- JSON load/save rewritten off `back_ptr` (D-H4.13 + commit 5)
- PDB-origin persistence rewritten off bridge (D-H4.14 + commit 5)
- Property bag persistence rewritten (D-H4.6 + commit 6)
- `AtomContainer` shared surface collapsed (D-H4.15 + commit 7)
- All v0 classes deleted (commits 8-11)

### D-H4.5 — Inline tree state deletion

(unchanged from R1)

For each container class, `Composite::child_` / `parent_` /
`next_sibling_` / `previous_sibling_` inline pointer state is
DELETED at the moment the class drops its `: public Composite`
base. The container table (`ContainerRow.parent_idx` + ChildRef
ordered edges) is sole source. Iteration moves to the handle-
yielding iterators (D-H4.12 + commit 4).

### D-H4.6 (R2) — Property persistence API break + Selectable removal

Codex H4-DR R1 HIGH: D-H4.6 R1 over-claimed completeness. JSON and
property helpers still take `PropertyManager&`. Split into:

(a) Consumer-surface migration (already DONE in H3a): value-semantics
    `getProperty/setProperty/hasProperty/clearProperty` on the handle.
(b) PERSISTENCE API break (NEW in H4 commit 6): JSON load/save +
    bag serialization stops taking `PropertyManager&` and switches
    to a property-column visitor (`eachProperty(visitor)`) +
    `propertyNames()`. The mutable-reference `NamedProperty&` /
    `BitVector&` surface is REMOVED at this commit per D49/D52.
(c) Selectable base deletion (commit 8 onwards): selection storage
    is already on `selected_bits_`; the `: public Selectable` base
    drops when Atom v0 deletes.

### D-H4.7 — `sizeof(Atom) ≤ 32 B` enforcement

(unchanged: keep 32 B at H4; cross-compiler verification at H8)

```cpp
static_assert(sizeof(BALL::Atom) <= 32,
  "D13: sizeof(BALL::Atom) must be ≤ 32 B post-H4 flip");
static_assert(sizeof(BALL::Bond) <= 32,
  "D13: sizeof(BALL::Bond) must be ≤ 32 B post-H4 flip");
```

### D-H4.8 — V21-COMPOSITEASATOM-REMOVAL closure

(unchanged from R1)

The `detail::compositeAsAtom_` single-dynamic-cast helper (v2.1
D41.1's RTTI consolidation point) becomes dead code at the moment
`Atom` no longer inherits from `Composite`. The CI grep gate
(`D41.1`-named) is removed alongside the helper in commit 13. The
30 VIEW sites that still call `compositeAsAtom_` are out of scope
here (H7's responsibility).

### D-H4.9 — Atomic commit cadence + tree-green gating

(unchanged from R1, with one addition)

Each H4 commit MUST:
1. Build clean on macOS + CORE_ONLY.
2. Run the full ctest (currently 289/289 GREEN).
3. Run HandleKeyLeakGate_test (no new leak introductions).
4. Run AssignBondOrder_bench within 5% of D-H3d.D.BASELINE.
5. **NEW (CR R1):** Run the table-only topology invariant + JSON
   round-trip tests (added in commit 1).
6. Be reviewable as a self-contained step.

### D-H4.10 — Codex DR cadence

R1 NOT-GO findings folded in. H4-DR round 2 runs against THIS draft
(R2). Standing surface protocol applies: ≤3 NOT-GO rounds before
escalation.

### D-H4.11 (NEW per CR R1) — System as store-owner refactor (not deletion)

Codex H4-DR R1 CRITICAL: D55 says System stays a C++ store owner;
the R1 plan's commit-12 "Delete System v0" was structurally wrong.

System survives H4 with:
- `: public AtomContainer, public Composite` bases STRIPPED (the
  inheritance disappears with the rest of the molecular hierarchy)
- The `MoleculeStore` ownership relationship preserved (System
  still owns + lifetime-manages its store)
- Container-row index for System's root container remains the
  table-side identifier
- Iteration through System remains via the handle-yielding
  `AtomIterator` / `MoleculeIterator` (D-H4.12)
- JSON persistence through System remains; the store-table format
  is what's serialized

Commit 11 (in the new plan) is "System base strip" — not deletion.

### D-H4.12 (NEW per CR R1) — Iterator + apply replacement

Codex H4-DR R1 CRITICAL: Atom cannot be deleted while
`CompositeIteratorTraits`, `AtomIterator`, `PDBAtomIterator`,
`AtomContainerIterator`, etc. are still `Composite*`/`Composite&`
backed.

Decision: in commits 3-4 of the new plan, replace
`CompositeIteratorTraits`-backed iterators with handle/table-backed
iterators that yield `AtomHandle` / `MoleculeHandle` / etc. by
value. The iterator categories survive (input-iterator semantics
match the handle case). Callers that used `*it` to get a reference
get a handle by value instead — type changes but spelling stays.

`Composite::apply<T>()` as the free template entry point dies when
the molecular classes drop their Composite base. Migrated callers
already go through `StructureQuery::apply` on handles. Commit 3
replaces the remaining v0 `Composite::apply<T>` callers in the
test surface and any unmigrated production sites.

### D-H4.13 (NEW per CR R1) — JSON load/save rewrite off bridge

Codex H4-DR R1 CRITICAL: JSON still uses `back_ptr`/`bond_back_ptr`
for atom and bond property bags
(`source/KERNEL/moleculeStoreJson.C:160`, 206;
`source/KERNEL/systemJson.C:438`).

Decision: commit 5 rewrites the JSON load/save path to operate on
store-table values directly:
- Atom property bag serialization reads/writes property_columns_ +
  sparse_bag via property-name iteration.
- Bond property bag serialization reads/writes bond property
  columns directly through bond_idx (not bond_back_ptr).
- The JSON wire schema is UNCHANGED (H6b is the schema freeze; H4
  preserves compatibility).
- A JSON round-trip test (D-H4.3) catches any wire-format drift.

### D-H4.14 (NEW per CR R1) — PDBAtom + FORMAT finalization

Codex H4-DR R1 HIGH: PDBAtom + FORMAT/PDBFile dispatch was deferred
from H3 to H4 and not covered in R1.

Decision: commit 5 (same commit as D-H4.13's JSON rewrite, since
the FORMAT/PDBFile loader goes through systemJson) converts:
- `PDBAtom` v0 constructors that materialized typed instances → 
  store-slot factories that populate atom-row PDB-origin columns
  (already present from HCP-1).
- `PDBFile` load path stops producing `PDBAtom*` instances and
  produces store atoms with PDB-origin payload set.
- `PDBFile` save path reads PDB-origin payload from atom-row columns.

The 54 VIEW Atom-RTTI sites that test for `PDBAtom` specifically
remain out of scope (V21-VIEW-RTTI / D37; H7's job).

### D-H4.15 (NEW per CR R1) — AtomContainer collapse

Codex H4-DR R1 HIGH: AtomContainer is the heavy shared base. Either
its public API needs `ContainerHandleBase` / role-handle methods or
a compatibility wrapper.

Decision: AtomContainer's public methods (the ones called via
`AtomContainer&` in production code) get equivalent free functions
in the `StructureQuery::` namespace OR member methods on the role
handles (`MoleculeHandle::countAtoms()`, `FragmentHandle::beginAtom()`,
etc.). Commit 7 lands this collapse before any v0 container class
is deleted. The `: public AtomContainer` base disappears in the
per-class deletion commits (8-11).

## Revised 15-commit plan

| # | Commit | Description |
|---|---|---|
| 0 | DR-R2 prep | Codex H4-DR round 2 + iterate to GO |
| 1 | Test scaffolding | Table-only topology invariant + JSON round-trip tests (D-H4.3 replacement of parity oracle) |
| 2 | API-break ledger | Enumerate every H4 break in V22-API-BREAK-LEDGER.md with migration notes + reverse-alias policy (D-H4.2) |
| 3 | apply replacement | Replace `Composite::apply<T>` molecular callers with `StructureQuery::apply` on handles (D-H4.12) |
| 4 | Iterator replacement | Replace Composite-backed molecular iterators (`AtomIterator`, `MoleculeIterator`, `PDBAtomIterator`, `AtomContainerIterator`) with handle/table-backed ones (D-H4.12) |
| 5 | JSON + PDB rewrite | JSON load/save off `back_ptr`/`bond_back_ptr`; PDBAtom + PDBFile route through store-slot factories (D-H4.13 + D-H4.14) |
| 6 | Property persistence | PropertyManager& removal from JSON + bag serialization; visitor + propertyNames() API (D-H4.6 b) |
| 7 | AtomContainer collapse | Public AtomContainer API moves to role handles + StructureQuery free functions (D-H4.15) |
| 7.5 | Parity retire | Drop HierarchyParity_test immediately before first v0 deletion (D-H4.3) |
| 8 | Delete v0 Atom | `Atom : Composite, PropertyManager, Selectable` → `class Atom { ... };` canonical from AtomHandle; reverse alias `[[deprecated]] using AtomHandle = Atom` |
| 9 | Delete v0 Bond | Same shape |
| 10 | Delete v0 containers | Bottom-up molecular containers (Fragment, Residue, Chain, Protein, Nucleotide, NucleicAcid, SecondaryStructure, Molecule) — *may need sub-commits per CR R1 atomicity concern* |
| 11 | System base strip | Strip `System : AtomContainer, Composite` (System survives as store owner per D-H4.11) |
| 12 | Drop bridge | `back_ptr`/`bond_back_ptr`/`bond_idx_of`/`bond_sid_of` removed (D-H4.4) |
| 13 | sizeof asserts + helper retire | static_asserts in atom.h + bond.h; `compositeAsAtom_` + D41.1 CI gate retired (D-H4.7, D-H4.8) |
| 14 | H4 close-CR | Codex H4-close-review; iterate to GO |

Estimate: 14 working commits + 1 design DR (round 2) + 1 closing CR =
~16 commits across the H4 cycle. Commit 10 may sub-divide depending
on actual blast radius per Codex H4-DR R1 finding 7 (Atom deletion
atomicity probe applies equally to multi-container deletion).

## Out of scope for H4 (unchanged)

- **VIEW Atom-RTTI sites (54)**: V21-VIEW-RTTI / D37 — H7's job.
- **Bond column layout / `Atom::bond_[]` removal**: H5's job.
- **JSON schema v2 freeze**: H6b's job (H4 keeps current wire format).
- **MSVC + vcpkg CI**: H7's job. Bring-up may start in parallel
  after commit 1.
- **`sizeof ≤ 32 B` cross-compiler verification**: H8's job.
- **100k-atom store-footprint test**: H8's job.

## Open questions resolved by CR R1

1. **Container order** — neither pure bottom-up nor top-down; correct
   order is: shared surfaces first (commits 3, 4, 5, 6, 7), THEN
   role-specific containers bottom-up (commit 10), then System
   base strip (commit 11) where System SURVIVES as the store owner.
2. **Reverse-alias lifetime** — deprecate through v2.3 release window
   (with `[[deprecated]]`); remove at v2.4 cycle open.
3. **HierarchyParity timing** — replace with table invariants + JSON
   round-trip in commit 1; retire in commit 7.5 right before first
   v0 deletion.
4. **Bridge timing** — commit 12, AFTER all v0 classes are deleted
   AND JSON / PDB / property persistence are rewritten.
5. **`sizeof` budget** — 32 B at H4 (no looser).
6. **AssignBondOrder regression** — keep 5% (D-H3d.D.BASELINE
   threshold).
7. **Atom deletion atomicity** — Atom v0 deletion is commit 8 of a
   pre-sliced sequence (commits 3-7 land the enablers first). May
   still sub-divide if the test surface exposes too much churn.
8. **AtomContainer** — D-H4.15 NEW. Public API moves to role
   handles + StructureQuery free functions; base disappears in
   per-class deletion commits.

## Open questions for Codex H4-DR round 2

(seeded by what's still uncertain in R2)

1. **D-H4.12 iterator-replacement signature**: callers used `*it` to
   get `Atom&`. Replacements yield `AtomHandle` by value. Source
   files that did `Atom& a = *it; a.setX(...);` now compile against
   handle write methods — this is a controlled break. Magnitude
   audit: how many sites?
2. **D-H4.13 JSON wire format equality**: JSON round-trip test
   verifies binary equality of the produced JSON before/after the
   rewrite. Acceptable, or should the test verify SEMANTIC equality
   (round-trip a randomized System, compare via store invariants)?
3. **D-H4.14 PDB load determinism**: store-slot factory order vs.
   the v0 PDBFile parse order — does the store assign the same
   `Index` sequence as the v0 path did, or do we need a parse-order-
   stable allocation policy?
4. **D-H4.15 AtomContainer public API**: an audit of which methods
   need migration (count + name + frequency). Do we have a full
   inventory yet, or does commit 7 need its own scoping pass?
5. **Commit 10 atomicity**: 8 container classes deleted in one
   commit OR a sub-sequence? Risk: bisect cost vs. tree-green
   cadence.

---

*Authored 2026-06-04 as pre-DR draft R1; revised to R2 same day
after Codex H4-DR round 1 NOT-GO. Codex H4-DR round 2 is the gate
to implementation.*
