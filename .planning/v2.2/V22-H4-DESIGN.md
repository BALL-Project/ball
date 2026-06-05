# V22-H4-DESIGN — The Flip (v0 retirement + canonical-name reconciliation)

**Status:** DRAFT-R4 (post-Codex H4-DR round 3 NOT-GO findings applied).
**Authored:** 2026-06-04 immediately post-H3d closing-CR round 4 GO at
`4e71e63f4`.
**Predecessor:** H3d (closed); see `V22-H3d-DESIGN.md` + D-H3d.CLOSE.
**Companion:** `.planning/v2.x/V2X-ROADMAP.md` §2 H4 row.
**DR needed:** YES — Codex H4-DR round 4 against this R4 draft.

**Surface note:** Standing v2.2 protocol triggers maintainer surface
on 3 NOT-GO rounds. R3 returned NOT-GO on PDBAtom KERNEL public API
scope + bridge audit count specification — both concrete and
addressable inline. Applying R4 inline keeps the productive cycle
intact rather than surfacing on findings that are clearly
GO-WITH-FIXES quality. If R4 returns NOT-GO, the surface fires.

## R3 -> R4 change summary

R3 had two residual issues per Codex H4-DR round 3:

- D-H4.14 R3 PDB audit covered FORMAT construction + dynamic_cast
  but missed KERNEL public API (residue.h:247 getPDBAtom/
  countPDBAtoms; PDBAtomIterator.h:55 typed iterators;
  extractors.h:57 PDBAtomList; extractors.C:74 PDBAtoms() return
  type; chain.C:128 / protein.C:165 / secondaryStructure.C:155
  PDBAtom* returns). Codex H4-DR R3 CRITICAL.
- D-H4.4 R3 hard-coded "29 files" for the bridge audit. Fresh
  rg finds 34 files / 119 lines. The count drifts; the audit
  must be specified by symbol grep + category, not fixed count.
  Codex H4-DR R3 HIGH.

R4 changes:

- D-H4.14 R4 (CRITICAL): PDB finalization expanded to three
  categories: (I) FORMAT construction (existing R3 scope), (II)
  KERNEL public API migration to StructureQuery free functions
  on Atom + reverse-aliases for PDBAtomList through v2.3, (III)
  iterator-traits + extractor type collapse. Commit 5 expands
  from 5a+5b to 5a/5b/5c/5d for the three categories.
- D-H4.4 R4 (HIGH): bridge audit scope defined by rg predicate +
  5-category classification (handle-accessor inline /
  production migration / test-specific use / definition site /
  comment-doc). Counts drift between commits; the ledger
  refresh runs at commit-prep time.
- Plan grows from 18 to 20 working commits via 5a/5b/5c/5d
  split.

## R2 -> R3 change summary

R2 was still under-scoped on four counts: bridge users span 29 files
(not the smaller R2 gating list); PDBAtom is constructed beyond
PDBFile (HINFile, dockResultFile); iterator replacement needs 159
deref-to-reference call-site edits not just a type swap; AtomContainer
collapse touches 245 files / 1493 lines and needs an inventory
ledger. Codex H4-DR round 2 findings folded in:

- D-H4.4 R3: new commit 11.5 (bridge-consumer audit + migration).
  V22-H4-BRIDGE-CONSUMER-AUDIT.md enumerates 29 files.
- D-H4.14 R3: PDBAtom DELETED, not facaded. Audit ledger
  V22-H4-PDB-CONSUMER-AUDIT.md covers PDBFile + HINFile +
  dockResultFile. PDB-origin payload on atom-row columns from HCP-1.
- D-H4.12 R3: iterator migration via call-site edits, not proxy.
  Audit ledger V22-H4-ITERATOR-CONSUMER-AUDIT.md enumerates 159
  deref-to-reference sites + 234-file iterator surface. Commit 4
  splits into 4a (audit) + 4b (edit).
- D-H4.15 R3: AtomContainer inventory ledger first
  (V22-H4-ATOMCONTAINER-AUDIT.md), per-cluster migration second.
  Commit 7 splits into 7a (ledger) + 7b (per-cluster edits).
- Commit 10 split into 10a/10b/10c (leaf → mid → top) per CR R2-Q5.
- CR R2-Q2 JSON test answer: semantic equality, not binary.
- CR R2-Q3 PDB determinism answer: parse-order-stable slot
  allocation.
- Plan grows from 15 to 18 working commits + 1 design DR + 1
  closing CR ≈ 20 commits.

## R1 -> R2 change summary (retained for history)

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

### D-H4.4 (R4) — Dual-existence bridge removal at the end

Codex H4-DR R3 HIGH: R3 hard-coded "29 files" for the bridge audit
scope. Fresh `rg` over `include source test` finds 34 files / 119
lines referencing `back_ptr` | `bond_back_ptr` | `bond_idx_of` |
`bond_sid_of`. The count drifts with every commit; the audit
scope must be defined by symbol grep, not fixed count.

R4 specifies the audit ledger by predicate:

- **Commit 11.5 (R4)**: bridge-consumer audit + migration. The
  audit ledger
  `.planning/v2.2/V22-H4-BRIDGE-CONSUMER-AUDIT.md` is generated
  by running `rg -tcpp 'back_ptr|bond_back_ptr|bond_idx_of|
  bond_sid_of' include source test` at the moment the commit is
  prepared, then categorizing every hit into:
    (a) **handle accessor inline**: the hit can become an inline
        table lookup in the same header (atomHandle.h /
        bondHandle.h pattern from Codex R2)
    (b) **production migration**: the hit lives in STRUCTURE /
        FORMAT / MOLMEC / ... production code and migrates to
        the sid-keyed sites already added in H3a-d
    (c) **test-specific bridge use**: the hit is in a test
        verifying bridge behavior; the test is REWRITTEN to
        verify the post-flip behavior
    (d) **definition site**: the bridge method definitions
        themselves; deleted in commit 12
    (e) **comment / doc**: cosmetic mention; no edit needed
  Counts per category go in the ledger header; production code
  edits land within commit 11.5 itself. Test rewrites land in
  commit 11.5 as well. Definitions are removed in commit 12.

- **Commit 12** then drops the bridge methods themselves
  (`back_ptr`, `bond_back_ptr`, `bond_idx_of`, `bond_sid_of` in
  MoleculeStore + any declarations in headers).

The bridge methods (`MoleculeStore::back_ptr(i)`, `bond_back_ptr(i)`,
`bond_idx_of(const Bond*)`, `bond_sid_of(const Bond*)`) are removed
in commit 12, AFTER:
- JSON load/save rewritten off `back_ptr` (D-H4.13 + commit 5)
- PDB-origin persistence rewritten off bridge (D-H4.14 + commit 5)
- Property bag persistence rewritten (D-H4.6 + commit 6)
- `AtomContainer` shared surface collapsed (D-H4.15 + commit 7)
- All v0 classes deleted (commits 8-11)
- **R3 NEW:** Bridge-consumer audit + migration (commit 11.5)

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

### D-H4.12 (R3) — Iterator + apply replacement

Codex H4-DR R2 HIGH: R2 wording "spelling stays" was misleading.
Real audit: 234 files / 1104 iterator-related lines / **159
dereference-to-reference lines** (`Atom& a = *it`, `&*it`, postfix
pointer use). Each of those is a call-site edit, not a no-op.

R3 decision: **call-site migration, NOT compatibility proxy.**

Rationale: a proxy that yielded `Atom&` on `*it` while the backing
representation is the handle/table would resurrect the v0 reference-
identity contract — exactly what H4 is trying to retire. Better to
land the 159 deref-to-reference sites as a single migration commit
(commit 4) with the audit ledger
(`.planning/v2.2/V22-H4-ITERATOR-CONSUMER-AUDIT.md`, NEW in R3)
that enumerates each site + its replacement pattern. The audit
ledger lands BEFORE the edit, so the diff is reviewable
mechanically.

Patterns:
- `Atom& a = *it;` → `Atom a = *it;` (Atom is now a handle, copy
  is cheap)
- `Atom* p = &*it;` → DELETED (cannot take address of a handle
  through this path; consumer must use sid/AtomHandle directly)
- `for (AtomIterator it = ...; +it; ++it) { it->foo(); }` →
  unchanged (arrow operator forwards through the handle)
- `it->setX(...)` → unchanged (handle write methods exist)

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

### D-H4.14 (R4) — PDBAtom + FORMAT + KERNEL public API finalization

Codex H4-DR R3 CRITICAL: R3's "audit construction + dynamic_cast"
scope was still incomplete. Audit found PDBAtom exposed in CORE
KERNEL public API, not just FORMAT construction:

- `include/BALL/KERNEL/residue.h:247`: `getPDBAtom()`,
  `countPDBAtoms()`, `prepend/append/insert/remove(PDBAtom&)`,
  PDBAtom iterator creators.
- `include/BALL/KERNEL/PDBAtomIterator.h:55`: iterator traits typed
  on `KernelPredicate<PDBAtom>` + typedef iterators yielding
  `PDBAtom`.
- `include/BALL/KERNEL/extractors.h:57`: `PDBAtomList = std::list<
  PDBAtom*>` public extractor type.
- `source/KERNEL/extractors.C:74`: `PDBAtoms()` returns
  `PDBAtomList` and filters via `dynamic_cast<const PDBAtom*>`.
- `source/KERNEL/chain.C:128`, `protein.C:165`,
  `secondaryStructure.C:155`: return `PDBAtom*`.

This is the KERNEL public surface. Deleting the class without
migrating these APIs would leave commit 8 broken.

R4 expands the PDB finalization scope to cover three categories:

(I) **CONSTRUCTION + CAST sites** (R3 scope, retained):
    PDBFileDetails.C, HINFile.C, dockResultFile.C → store-slot
    factories.

(II) **KERNEL PUBLIC API sites** (NEW in R4): The PDB-specific
     accessor methods on `Residue`/`Chain`/`Protein`/
     `SecondaryStructure` are REPLACED by free functions in the
     `StructureQuery::` namespace that filter on
     `Atom::hasPDBOrigin()`:

     ```cpp
     // Before H4 (KERNEL public API):
     Residue r; PDBAtom* p = r.getPDBAtom(...);
     PDBAtomList list = r.PDBAtoms();
     for (auto it = r.beginPDBAtom(); +it; ++it) ...

     // After H4:
     namespace BALL::StructureQuery {
       Atom getPDBAtom(const ResidueHandle&, ...);
       std::vector<Atom> pdbAtoms(const ResidueHandle&);
       auto pdbAtomRange(const ResidueHandle&);   // range-for friendly
     }
     ```

     Reverse aliases (`[[deprecated]] using PDBAtomList = std::list<
     Atom>;`) cover transition through v2.3, removed v2.4 (same
     policy as the *Handle aliases per D-H4.2 R2).

(III) **ITERATOR TRAITS** (NEW in R4): `PDBAtomIterator` becomes
      a filtered-handle iterator over the role-handle's
      atom-CSR view that yields atoms where
      `Atom::hasPDBOrigin()`. The iterator-traits type becomes
      `HandleIteratorTraits<Atom>` filtered by predicate
      `[](const Atom& a){ return a.hasPDBOrigin(); }`. No new
      type is introduced — `PDBAtomIterator` is a `using` alias
      for the filtered handle iterator.

The pre-commit-5 audit ledger
(`.planning/v2.2/V22-H4-PDB-CONSUMER-AUDIT.md`, NEW in R3) is
expanded in R4 to enumerate three categories: (I) construction,
(II) KERNEL public API surface, (III) iterator-traits + extractor
type. Each category's edits land in separate commits within the
commit-5 cluster (5a audit ledger + 5b construction/JSON + new
**5c KERNEL public API migration** + new **5d iterator/extractor
collapse**).

`PDBAtom` itself is DELETED in commit 8 alongside `Atom`.

The 54 VIEW Atom-RTTI sites remain out of scope (V21-VIEW-RTTI /
D37; H7's job).

### D-H4.15 (R3) — AtomContainer collapse with inventory ledger

Codex H4-DR R2 HIGH: 245 files / 1493 lines reference
`AtomContainer`. R2's "moves to handles/free functions" wording
glossed over the scoping. The class has 9 method clusters per the
audit: construction/copy, parent/child access, atom lookup, counts,
mutation, splice/remove, predicates, bond application, iterators,
property behavior, and mirror hooks (atomContainer.h:29, :166, :251).

R3 splits commit 7 into two sub-commits:

- **Commit 7a (NEW): AtomContainer inventory ledger**
  `.planning/v2.2/V22-H4-ATOMCONTAINER-AUDIT.md` enumerates every
  public method on `AtomContainer`, classifies it by destination
  (role-handle member, StructureQuery free function, DELETED,
  retained-on-System), and lists every external call site (245
  files / 1493 lines).
- **Commit 7b: AtomContainer API migration**
  Per-method migration in dependency order: read-side first
  (lookup, counts, predicates, iterators), then mutation
  (insert, remove, splice), then property/bond behavior, then
  constructor/copy/destructor (the latter only at commits 8-11
  when the v0 class itself dies). Each method's migration is a
  reviewable diff against the ledger.

Commit 7b is potentially several smaller commits depending on
ledger scope; the design commits to "one logical method cluster
per commit" without pre-committing to the exact count.

## Revised commit plan (R3)

| # | Commit | Description |
|---|---|---|
| 0 | DR-R3 prep | Codex H4-DR round 3 + iterate to GO |
| 1 | Test scaffolding | Table-only topology invariant + JSON round-trip tests with semantic equality (D-H4.3 replacement of parity oracle; CR R2-Q2 answer) |
| 2 | API-break ledger | Enumerate every H4 break in V22-API-BREAK-LEDGER.md with migration notes + reverse-alias policy (D-H4.2); deprecate aliases through v2.3, remove v2.4 |
| 3 | apply replacement | Replace `Composite::apply<T>` molecular callers with `StructureQuery::apply` on handles (D-H4.12) |
| 4a | Iterator-consumer audit | Write `V22-H4-ITERATOR-CONSUMER-AUDIT.md` with the 159 deref-to-reference sites + 234-file iterator surface (D-H4.12 R3) |
| 4b | Iterator migration | Replace Composite-backed molecular iterators with handle/table-backed ones; apply the per-site edits from the 4a ledger |
| 5a | PDB-consumer audit | Write `V22-H4-PDB-CONSUMER-AUDIT.md` with 3 categories: (I) construction/cast in FORMAT, (II) KERNEL public API surface (residue.h, PDBAtomIterator.h, extractors.h, chain.C, protein.C, secondaryStructure.C), (III) iterator-traits + extractor types (D-H4.14 R4) |
| 5b | JSON + PDB construction rewrite | JSON load/save off `back_ptr`/`bond_back_ptr` (D-H4.13); FORMAT files (PDBFile/HINFile/dockResultFile) route through store-slot factories; PDB-origin payload on atom-row columns (D-H4.14 R4 category I) |
| 5c | PDB KERNEL public API migration | Residue/Chain/Protein/SecondaryStructure PDB accessors → StructureQuery free functions on Atom + hasPDBOrigin() predicate; deprecated reverse aliases for PDBAtomList through v2.3 (D-H4.14 R4 category II) |
| 5d | PDB iterator + extractor collapse | PDBAtomIterator typedef'd to filtered handle iterator on Atom with predicate hasPDBOrigin(); PDBAtomList → vector<Atom> with deprecated alias (D-H4.14 R4 category III) |
| 6 | Property persistence | PropertyManager& removal from JSON + bag serialization; visitor + propertyNames() API (D-H4.6 b) |
| 7a | AtomContainer inventory ledger | `V22-H4-ATOMCONTAINER-AUDIT.md` enumerates every public method + every call site (245 files / 1493 lines per CR R2). Classify by destination (D-H4.15 R3) |
| 7b | AtomContainer API migration | Per-method-cluster migration in dependency order: read-side → mutation → property/bond → constructor/dtor. May span several commits depending on ledger scope |
| 7.5 | Parity retire | Drop HierarchyParity_test immediately before first v0 deletion (D-H4.3) |
| 8 | Delete v0 Atom + PDBAtom | `Atom : Composite, PropertyManager, Selectable` → `class Atom { ... };` canonical from AtomHandle; reverse alias `[[deprecated]] using AtomHandle = Atom`. PDBAtom v0 class DELETED (D-H4.14 R3); PDB-origin payload lives on atom-row columns |
| 9 | Delete v0 Bond | Same shape as Atom |
| 10a | Delete v0 leaf containers | Fragment, SecondaryStructure (leaf role containers per CR R2-Q5) |
| 10b | Delete v0 mid containers | Residue, Nucleotide (chain/strand-level) |
| 10c | Delete v0 top containers | Chain, Protein, NucleicAcid, Molecule (bottom-up sequence per CR R2-Q5) |
| 11 | System base strip | Strip `System : AtomContainer, Composite` (System survives as store owner per D-H4.11) |
| 11.5 | Bridge-consumer audit + migration | Write `V22-H4-BRIDGE-CONSUMER-AUDIT.md` generated by `rg -tcpp 'back_ptr|bond_back_ptr|bond_idx_of|bond_sid_of'` at commit-prep time, classified into 5 categories (a) handle-accessor inline / (b) production migration / (c) test rewrite / (d) definition site / (e) comment-doc. Apply per-category edits in this commit. (D-H4.4 R4) |
| 12 | Drop bridge | `back_ptr`/`bond_back_ptr`/`bond_idx_of`/`bond_sid_of` removed (D-H4.4) |
| 13 | sizeof asserts + helper retire | static_asserts in atom.h + bond.h; `compositeAsAtom_` + D41.1 CI gate retired (D-H4.7, D-H4.8) |
| 14 | H4 close-CR | Codex H4-close-review; iterate to GO |

Estimate: 18 working commits (was 14) + 1 design DR (round 3) + 1
closing CR ≈ 20 commits across the H4 cycle. The growth is real:
R3 surfaces three new audit-ledger commits (4a, 5a, 7a, 11.5) +
splits one commit (10) into three sub-commits.

## Out of scope for H4 (unchanged)

- **VIEW Atom-RTTI sites (54)**: V21-VIEW-RTTI / D37 — H7's job.
- **Bond column layout / `Atom::bond_[]` removal**: H5's job.
- **JSON schema v2 freeze**: H6b's job (H4 keeps current wire format).
- **MSVC + vcpkg CI**: H7's job. Bring-up may start in parallel
  after commit 1.
- **`sizeof ≤ 32 B` cross-compiler verification**: H8's job.
- **100k-atom store-footprint test**: H8's job.

## Open questions resolved by CR R1 + R2

1. **Container order** — shared surfaces first (commits 3, 4a-b, 5a-b,
   6, 7a-b), THEN role-specific containers in 3 sub-commits per
   CR R2-Q5 (leaf → mid → top), then System base strip (commit 11).
2. **Reverse-alias lifetime** — deprecate through v2.3 release
   window with `[[deprecated]]`; remove at v2.4 cycle open.
3. **HierarchyParity timing** — replace with table invariants +
   JSON semantic-equality round-trip in commit 1; retire in
   commit 7.5 right before first v0 deletion (per CR R2-Q2:
   semantic equality, not binary).
4. **Bridge timing** — commit 12, AFTER pre-commit-11.5 audit +
   migration of the 29 bridge-reference files (per CR R2-Q1).
5. **`sizeof` budget** — 32 B at H4 (no looser).
6. **AssignBondOrder regression** — keep 5%.
7. **Atom deletion atomicity** — single commit 8 stays viable
   after enablers (3-7) land; the 159 deref-to-reference sites
   are migrated in commit 4b, not at atom deletion time.
8. **AtomContainer** — D-H4.15 R3 splits into 7a (inventory
   ledger) + 7b (per-cluster migration). 245-file scope acknowledged.
9. **Iterator magnitude (CR R2-Q1)** — 159 obvious deref-to-reference
   sites + broader 234-file/1104-line iterator API surface; D-H4.12
   R3 calls for call-site migration, not compatibility proxy.
10. **JSON wire-format vs. semantic test (CR R2-Q2)** — semantic
    equality over randomized/store-invariant round trips with
    optional canonical JSON comparison. Binary equality is too
    brittle for object ordering.
11. **PDB determinism (CR R2-Q3)** — parse-order-stable allocation.
    ATOM/HETATM records map to store slots in parse order;
    serial-to-handle mapping is preserved for CONECT records.
12. **AtomContainer inventory (CR R2-Q4)** — commit 7a writes the
    ledger BEFORE commit 7b applies the edits.
13. **Commit 10 atomicity (CR R2-Q5)** — split into 10a (leaves) +
    10b (mid) + 10c (top), bottom-up.

## Open questions for Codex H4-DR round 3

(seeded by what's still uncertain in R3)

1. **PDBAtom* removal blast radius (D-H4.14 R3)** — once the audit
   ledger lands (commit 5a), are there VIEW-only references that
   creep into FORMAT/STRUCTURE through indirect includes? If so,
   the H4/H7 boundary needs a finer split.
2. **PropertyManager& break compatibility shim (D-H4.6 R2)** —
   downstream code that took `NamedProperty&` mutable references
   has no migration path through the visitor; is a single-cycle
   read-only shim (`NamedPropertyRef` that triggers a write on
   destruction) worth introducing, or accept the break as a hard
   one per D49/D52?
3. **AtomContainer cluster boundary (D-H4.15 R3 commit 7b)** —
   does the audit ledger surface a method that crosses clusters
   (e.g. is both read-side and mutation)? If yes, commit 7b's
   per-cluster commit boundary needs further refinement.
4. **Iterator audit completeness (D-H4.12 R3 commit 4a)** — the 159
   number from Codex H4-DR R2 was a quick grep. A more thorough
   audit may surface more sites (or fewer). Should commit 4a's
   ledger include a tolerance band (e.g. ±20% spread acceptable
   without re-DR) or strict count match?
5. **Commit 11.5 bridge-consumer audit scope** — CR R2 identified
   29 files referencing the bridge. A more thorough audit may
   classify those into (a) handle accessors that can inline the
   table lookup, (b) production code that needs migration to the
   sid-keyed sites added in H3a-d, and (c) test code that exercises
   the bridge specifically. Commit 11.5's ledger needs to make
   the (a)/(b)/(c) split explicit.

---

*Authored 2026-06-04 as pre-DR draft R1; revised to R2 same day
after Codex H4-DR round 1 NOT-GO; revised to R3 same day after
Codex H4-DR round 2 NOT-GO. Codex H4-DR round 3 is the gate to
implementation. Per standing v2.2 surface protocol: ≤3 NOT-GO
rounds before maintainer surface.*
