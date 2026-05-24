# V22 HCP-2 — collapsed handle API (design-lock, step-by-step)

**Status:** IN PROGRESS. The user-visible heart of the collapse; still
**dual existence** (handles READ the HCP-1 role columns; v0 classes remain until H4).
Sub-steps (plan §6a): **HCP-2a** role-aware handles ✅ DONE (commit 2177ec59b) →
**HCP-2b** `StructureQuery` — **2b.1 ✅ DONE (commit 5c7c2ff4f)**, 2b.2 deprecated
iterator alias DEFERRED to HCP-3 migration → **HCP-2c** deferred mirrors (NEXT) →
**HCP-2d** SS-as-annotation. Each sub-step gets its own design-lock when reached.

---

## HCP-2a — role-aware `MoleculeHandle` / `FragmentHandle`

### Goal
Collapse the **8 kind-tagged** typed handles (`MoleculeHandle`/`ChainHandle`/
`FragmentHandle`/`ProteinHandle`/`NucleicAcidHandle`/`ResidueHandle`/`NucleotideHandle`/
`SecondaryStructureHandle`, today each pinned to one `ContainerKind`) down to **2
role-aware handles** + reuse `Atom`:
- **`MoleculeHandle`** — any molecule-level row; today's v0 kinds {MOLECULE, PROTEIN,
  NUCLEIC_ACID}. Exposes `getMoleculeRole()` (derived, `container_molecule_role_`).
- **`FragmentHandle`** — any fragment-level row; v0 kinds {CHAIN, FRAGMENT, RESIDUE,
  NUCLEOTIDE, SECONDARY_STRUCTURE}. Exposes `getFragmentRole()` + `getResidueKind()`
  + `getSSKind()` (derived) + role-asserting `getID()`/`getInsertionCode()`/SS code.
Role enums + store accessors already exist (HCP-1a: `container_molecule_role_`/
`_fragment_role_`/`_residue_kind_`/`_ss_kind_`, derived from `container_kind_` during
dual existence). HCP-2a is purely the **handle layer** over them.

### The 3 design decisions to lock

**D-2a.1 — multi-kind acceptance (the `as<T>()` rework).** `ContainerHandleBase::as<T>()`
+ the typed-handle ctor today gate on a SINGLE `T::KIND == container_kind_`. A
role-aware handle accepts a **SET** of kinds. Replace the single-tag check with a
static membership predicate per handle:
```
static bool acceptsKind(ContainerKind k);   // MoleculeHandle: MOLECULE|PROTEIN|NUCLEIC_ACID
                                             // FragmentHandle: CHAIN|FRAGMENT|RESIDUE|NUCLEOTIDE|SECONDARY_STRUCTURE
```
`as<T>()` and the debug/Python kind-assert call `T::acceptsKind(kind)` instead of
`== T::KIND`. (Keep `KIND` on the *deprecated* single-kind aliases for source compat.)
This stays a tag compare — **no RTTI**, no size change (handles stay 24 B; pinned by
`static_assert`).

**D-2a.2 — role-asserting getters move onto the 2 handles.** The kind-specific getters
on the old typed handles migrate:
- `MoleculeHandle::getID()` — valid when role is PROTEIN/NUCLEIC_ACID (debug/Python
  `BALL_CONTAINER_HANDLE_CHECKS` assert; release UB on misuse, the documented contract).
- `FragmentHandle::getID()` + `getInsertionCode()` — valid when role is RESIDUE/
  NUCLEOTIDE; `getSSKind()`/`getSSTypeCode()` — valid when role is SECONDARY_STRUCTURE.
Generic getters (`getName`/`getKind`/parent/child/selection) stay on the base.

**D-2a.3 — dual-existence naming + deprecated legacy handles (D63). [REVISED per R1.]**
During dual existence the v0 C++ classes `Molecule`/`Fragment`/`Protein`/… still exist,
so the role-aware handles KEEP the `Handle` suffix (`MoleculeHandle`/`FragmentHandle`) —
no clash. The other 6 typed handles are NOT plain `using` aliases (R1 FLAW: a
`using ResidueHandle = FragmentHandle` widens `as<ResidueHandle>()` to accept ANY
fragment kind — inverting the existing `ContainerHandle_test` wrong-kind narrow — and
cannot preserve `ResidueHandle::KIND`). Instead they are **`[[deprecated]]` no-data
subclass wrappers** that KEEP their single-kind narrow:
```
class [[deprecated("use MoleculeHandle + getMoleculeRole()")]] BALL_EXPORT ProteinHandle
  : public MoleculeHandle
{ public:
  static constexpr ContainerKind KIND = ContainerKind::PROTEIN;           // legacy single-kind
  static bool acceptsKind(ContainerKind k) { return k == ContainerKind::PROTEIN; }
  ProteinHandle() = default;
  ProteinHandle(MoleculeStore& s, std::uint32_t i) : MoleculeHandle(s, i) { /* single-kind assert */ }
};   // inherits getMoleculeRole/getID from MoleculeHandle; NO data members (stays 24 B)
```
Same for `NucleicAcidHandle` (NUCLEIC_ACID) over `MoleculeHandle`; `ChainHandle`(CHAIN),
`ResidueHandle`(RESIDUE), `NucleotideHandle`(NUCLEOTIDE), `SecondaryStructureHandle`
(SECONDARY_STRUCTURE) over `FragmentHandle`. This preserves `as<OldHandle>()` =
old-kind narrow + `OldHandle::KIND` + the wrong-kind-returns-null test, while emitting a
deprecation warning. **`SecondaryStructureHandle::getTypeCode()` is kept** (forwards to
the FragmentHandle SS code) for source compat. At **H4** these wrappers are deleted and
`MoleculeHandle`/`FragmentHandle` renamed to canonical `Molecule`/`Fragment` (D63).

> Note (R1 item 6): the role-asserting getters (D-2a.2) assert the DERIVED **role**
> (getMoleculeRole/getFragmentRole/getResidueKind/getSSKind), not the v0 `kind`, so the
> code is already correct when `ContainerKind` shrinks to {MOLECULE,FRAGMENT} at H4. The
> legacy wrappers' single-kind `acceptsKind`/`KIND` are the ONLY kind-tag dependence and
> are deleted at H4 with the wrappers.

### Scope / non-goals (HCP-2a)
- READ-ONLY role accessors over existing columns. No new mutation, no new store state.
- `ContainerKind` does NOT shrink to {MOLECULE,FRAGMENT} yet (deferred to H4 — during
  dual existence the v0 kind still drives role derivation; shrinking early forces
  premature consumer changes).
- No production consumer yet (dual existence) — so this is additive + low-risk.

### Touch points
- `include/BALL/KERNEL/containerHandle.h`: rework the typed-handle macro → role-aware
  `MoleculeHandle`/`FragmentHandle` with `acceptsKind` + role-asserting getters; the 6
  `[[deprecated]]` aliases; keep base `as<T>()` calling `acceptsKind`.
- `test/ContainerHandle_test.C`: role-aware acceptance (a `MoleculeHandle` binds a
  PROTEIN row; `getMoleculeRole()==PROTEIN`), role-asserting getters, `as<>()` over the
  kind set, and a deprecated-alias still-compiles smoke (with the warning suppressed).
- size pins (24 B) re-asserted.

### Verify
rc-gated build (libBALL + ALL test bins) + full ctest + ContainerHandle_test focused;
Codex HCP-2a code-review. Atomic commit; push.

### Gate
Codex **HCP-2a design AGREE** on D-2a.1/.2/.3 before code.

## HCP-2a-R1 design review

Verdict: **NEEDS-REVISION**. Proceed with the 2 role-aware primary handles, but do
not implement the 6 legacy names as plain `using OldHandle = MoleculeHandle` /
`FragmentHandle` aliases. Use deprecated no-data compatibility wrapper classes (or an
equivalent mechanism) if source compatibility for `OldHandle::KIND` and
`as<OldHandle>()` is intended.

1. **D-2a.1 (`as<T>()` multi-kind): WEAK.** Replacing `T::KIND == kind` with
   `T::acceptsKind(kind)` is sound for the new `MoleculeHandle` / `FragmentHandle`
   primaries and the debug/Python ctor assert, and grep found no production template
   consumers of `T::KIND` beyond `containerHandle.h` itself. The weak point is the
   alias plan: a plain `using ResidueHandle = FragmentHandle` cannot preserve
   `ResidueHandle::KIND == RESIDUE`; it resolves to the primary handle's members and
   makes `as<ResidueHandle>()` accept every fragment-level kind. The existing
   `ContainerHandle_test.C` wrong-kind narrow (`CHAIN` as `ResidueHandle` returns null)
   would invert unless the deprecated names remain distinct no-data wrappers with
   single-kind `acceptsKind`.

2. **D-2a.2 (role-asserting getters): SOUND.** Moving `getID`,
   `getInsertionCode`, and SS payload access onto the role-aware handles with
   `BALL_CONTAINER_HANDLE_CHECKS` role assertions is the same contract class as the
   current raw typed-handle ctor: debug/Python catches misuse, release C++ treats
   wrong-role getter calls as caller UB. Returning empty strings, `' '`, or `0` for
   wrong-role live rows would be a regression because it would silently merge
   "wrong role" with legitimate empty/default payloads; keep sentinel behavior only in
   store-level out-of-range accessors, not in valid-handle role dispatch.

3. **D-2a.3 (deprecated aliases): FLAW.** Plain aliases widen legacy handles enough to
   break safe-narrow semantics: `ProteinHandle` would accept `MOLECULE` and
   `NUCLEIC_ACID`, `ResidueHandle` would accept `CHAIN` / `FRAGMENT` /
   `NUCLEOTIDE` / `SECONDARY_STRUCTURE`, and old `as<OldKindHandle>()` no longer
   means old-kind narrow. Consumer grep (`include` + `source` + `test`, excluding
   vendored json) found active handle consumers only in `test/ContainerHandle_test.C`
   and declarations in `include/BALL/KERNEL/containerHandle.h`; the only non-code
   comment hit is `_moleculeStoreInternal.h` mentioning legacy
   `SecondaryStructureHandle`. Affected test calls are: layout pins for all 8 names;
   `ProteinHandle ph(...).getID()`; `ResidueHandle rh(...).getID()` /
   `getInsertionCode()`; `SecondaryStructureHandle ssh(...).getTypeCode()`;
   `parent.as<ChainHandle>()`; wrong-kind `parent.as<ResidueHandle>()`; stale-handle
   tests using `ChainHandle` / `ResidueHandle`. If compatibility wrappers are used,
   also keep the old `SecondaryStructureHandle::getTypeCode()` spelling, either on
   the wrapper or as a deprecated forward to `getSSTypeCode()`.

4. **Naming under dual existence: SOUND.** Keeping the `Handle` suffix through dual
   existence is consistent with D63 and avoids colliding with v0 `Molecule` /
   `Fragment`. The H4 plan remains a canonical-name migration audit, not a one-line
   alias, which is the right constraint.

5. **Scope: SOUND.** HCP-2a should stay read-only and handle-layer-only: no
   `ContainerKind` shrink, no new store state, and no consumer migration. Pulling
   HCP-2b `StructureQuery`, HCP-2c mirrors, or HCP-2d SS-as-annotation into this step
   would blur the gate and make failures harder to isolate.

6. **New risk / missed dependency: WEAK.** The design should explicitly require
   role assertion helpers to assert the derived role, not the old kind, so the code is
   ready for the later `ContainerKind` shrink. It should also spell out the exact
   compatibility surface for old getter names (`getTypeCode` in particular) and for
   `OldHandle::KIND`; otherwise the implementation can compile today while quietly
   eroding the deprecation-window contract.

## HCP-2a-R1b confirmation

Verdict: **AGREE** — implement HCP-2a as designed.

1. **ADDRESSED.** Deprecated no-data subclass wrappers preserve old-kind narrow:
   `as<ProteinHandle>()` calls `ProteinHandle::acceptsKind(k)` and accepts only
   `PROTEIN`, so wrong-kind rows still return null and `ProteinHandle::KIND` remains
   the legacy single-kind constant.

2. **ADDRESSED.** `as<T>()` via `T::acceptsKind(kind)` is consistent for the multi-kind
   primaries and the single-kind wrappers because each concrete handle type supplies
   its own predicate; constructing `T(store, idx)` is viable through the wrapper's thin
   ctor over the primary, and the no-data inheritance keeps the handles at 24 B.

3. **ADDRESSED.** Role-asserting getters asserting the derived role instead of v0 kind
   is the right H4-ready posture; the only remaining kind-tag dependence is the
   deprecated wrappers' `KIND`/single-kind `acceptsKind`, which is intentionally removed
   when the wrappers are deleted at H4.

4. **ADDRESSED.** No remaining design blocker found for HCP-2a; keep
   `SecondaryStructureHandle::getTypeCode()` as the compatibility spelling during dual
   existence and delete it with the wrapper at H4.

## HCP-2a-CR code review

Verdict: **GO** — the implemented HCP-2a role-aware container-handle patch matches
the agreed R1b design; no blocking correctness, ABI, or compatibility issue found.

1. **SOUND.** `as<T>()` now dispatches through `T::acceptsKind(kind)`, which is correct
   for the multi-kind `MoleculeHandle` / `FragmentHandle` primaries and for the
   deprecated single-kind wrappers; grep found no repo caller using `as<>()` with a
   type lacking `acceptsKind`.
2. **SOUND.** The role-asserting getters check the derived role, not the transient v0
   kind: molecule `getID()` requires `PROTEIN` / `NUCLEIC_ACID`, fragment `getID()` /
   `getInsertionCode()` require `RESIDUE`, and `getTypeCode()` requires
   `SECONDARY_STRUCTURE`; this is the H4-ready contract and keeps the same
   debug/Python-only check, release-UB misuse model as the prior typed constructors.
3. **SOUND.** The deprecated wrappers preserve legacy `KIND`, single-kind
   `acceptsKind`, inherited getters, and no-data layout; the base constructor may accept
   the broader primary set, but the wrapper constructor then enforces the legacy
   single-kind in checked builds.
4. **SOUND.** Size / ABI pins hold for the primaries and the test covers all wrapper
   sizes at 24 B; adding `containerRole.h` stays inside the public D66a boundary and
   does not pull private store internals into `containerHandle.h`.
5. **SOUND.** The retained legacy checks plus the new HCP-2a CHECK cover role-aware
   acceptance, role accessors, happy-path role-asserting getters, multi-kind `as<>()`,
   and preserved single-kind wrapper narrowing; negative debug exception tests would
   be additive, not a blocker.
6. **SOUND.** No new missed handle name, include cycle, vtable/slicing concern, or
   production `as<>()` caller issue found; `build-core` `ctest -R ContainerHandle_test
   --output-on-failure` passed.

---

## HCP-2b — `BALL::StructureQuery` free-function namespace (step-by-step: 2b.1)

### Goal
A `namespace BALL::StructureQuery` of free functions that answer the polymer-structure
questions the v0 typed containers answered via member functions + typed iterators
(`Protein::residues()`/`chains()`/`getResidueByID`/`getNTerminal`/`getCTerminal`,
`Chain::getNTerminal`/`getCTerminal`, etc.), but over the **role-aware handles**
(HCP-2a) + the HCP-1a role columns. Canonical-depth-honouring (Molecule → Chain →
[SecondaryStructure] → Residue → Atom), so a query finds RESIDUE-role fragments whether
they sit directly under a chain or under a chain→SS layer (SS is still an owning layer
during dual existence; becomes a non-owning annotation at HCP-2d — the query is written
against the ROLE, so it survives that change unchanged).

### Scope split (step-by-step)
- **HCP-2b.1 (THIS step):** the StructureQuery free functions, READ-only, returning
  value-handle vectors. Header-only over the existing `ContainerHandleBase` navigation
  (`countChildren`/`getChildContainer`) + role accessors. New `structureQuery.h` + test.
- **HCP-2b.2 (DEFERRED):** the `[[deprecated]]` role-filtered iterator alias for the
  ~40-file v0 `ResidueIterator`/`ChainIterator` consumer surface (D-HC5). That's a
  consumer-compat layer (closer to HCP-3 migration); not needed to establish the query
  API and would blur this step's gate.

### API (D-2b.1) — entry point is a `MoleculeHandle` (no SYSTEM row exists)
```
namespace BALL { namespace StructureQuery {
  // direct CHAIN-role children of a molecule
  std::vector<FragmentHandle> chains(const MoleculeHandle&);
  // all RESIDUE-role fragments in a molecule's (or chain's) subtree, canonical-depth
  std::vector<FragmentHandle> residues(const MoleculeHandle&);
  std::vector<FragmentHandle> residues(const FragmentHandle&);          // chain/SS subtree
  // all SECONDARY_STRUCTURE-role fragments in a molecule's (or chain's) subtree
  std::vector<FragmentHandle> secondaryStructures(const MoleculeHandle&);
  std::vector<FragmentHandle> secondaryStructures(const FragmentHandle&);
  // generic role filter over a subtree (the building block; the above are sugar)
  std::vector<FragmentHandle> fragmentsByRole(const ContainerHandleBase&, FragmentRole);
  // generic first/last RESIDUE-role fragment in preorder (no kind filter) -- building block
  FragmentHandle firstResidue(const ContainerHandleBase&);   // molecule or chain
  FragmentHandle lastResidue(const ContainerHandleBase&);
  // v0-compatible polymer N-/C-terminus: first/last RESIDUE-role fragment that is an
  // AMINO_ACID (matches v0 getNTerminal/getCTerminal, which filter via isAminoAcid())
  FragmentHandle nTerminal(const ContainerHandleBase&);   // molecule or chain
  FragmentHandle cTerminal(const ContainerHandleBase&);
  // first RESIDUE-role fragment whose getID() == id (preorder); null handle if none
  FragmentHandle residueByID(const MoleculeHandle&, const String& id);
  // the enclosing SECONDARY_STRUCTURE-role fragment of a residue (walk parents); null if none
  FragmentHandle secondaryStructureOf(const FragmentHandle& residue);
}}
```

### Decisions to lock
- **D-2b.1 return type = `std::vector<Handle>` (this step).** Simplest correct
  value-semantics API; handles are 24 B values. A lazy range / callback form is a later
  optimization (note it; not now) — avoids premature iterator design while the deferred
  HCP-2b.2 alias + HCP-2c mirrors are still pending.
- **D-2b.2 role-based, not kind-based.** Filtering uses `getFragmentRole()`/
  `getMoleculeRole()` (derived), NOT `getKind()` — so the queries are correct after the
  H4 `ContainerKind` shrink + the HCP-2d SS-as-annotation change. Canonical depth is
  honoured by recursing the whole subtree and filtering by role (not by fixed depth).
- **D-2b.3 null-handle idiom.** Single-result queries (`nTerminal`/`residueByID`/
  `secondaryStructureOf`) return a null `FragmentHandle` (`operator bool` false) when
  absent — consistent with the D61 null-handle pointer-return replacement.
- **D-2b.4 traversal source.** Walk via `ContainerHandleBase::countChildren()` +
  `getChildContainer(i)` (containers) ; atoms are leaves (skipped for fragment queries).
  Read-only; no store mutation; no new store state.
- **D-2b.5 v0-compatible terminal semantics (R1->R1b fix).** v0 `getNTerminal`/
  `getCTerminal` (residue.h:460-489) iterate `beginResidue()`/`rbeginResidue()` and return
  the FIRST/LAST residue with `isAminoAcid() == true` (= `hasProperty(PROPERTY__AMINO_ACID)`,
  which materialises to `ResidueKind::AMINO_ACID` per atomContainer.C:53). So `nTerminal`/
  `cTerminal` filter RESIDUE-role fragments to `getResidueKind() == ResidueKind::AMINO_ACID`
  — NOT the first/last arbitrary RESIDUE-role fragment. The generic first/last-RESIDUE
  building block is exposed separately as `firstResidue`/`lastResidue` (no kind filter), so
  callers that want the unfiltered terminus (or a nucleotide analog via the generic helper +
  a `ResidueKind` test) are served without polluting the v0-faithful terminal API. Both
  terminal functions return a null `FragmentHandle` when the subtree has no amino-acid
  residue, exactly as v0 returns `0`.

### Scope / non-goals
- READ-only; dual existence; no consumer migration; no SYSTEM handle (entry is a
  MoleculeHandle the caller obtains from the v0 Molecule's container row).
- No deprecated iterator alias (HCP-2b.2).

### Touch points
- NEW `include/BALL/KERNEL/structureQuery.h` (header-only, over containerHandle.h).
- NEW `test/StructureQuery_test.C` (+ test CMake registration) — build a Protein→Chain→
  [SS]→Residue→Atom table, assert chains/residues/residueByID/firstResidue/lastResidue/
  nTerminal/cTerminal/secondaryStructures/secondaryStructureOf/fragmentsByRole over it.
  MUST include a chain with a leading/trailing NON-amino residue (e.g. a HOH water or a
  hetero group) so the test pins that `nTerminal`/`cTerminal` skip it (matching v0
  `isAminoAcid()`) while `firstResidue`/`lastResidue` return it.
- D31b/D66a: structureQuery.h includes only public headers (containerHandle.h).

### Gate
Codex **HCP-2b.1 design AGREE** before code; then implement + rc-gated build + ctest +
Codex code-review + commit.

## HCP-2b1-R1 design review
Verdict: **NEEDS-REVISION**. The subtree/role-query design is sound, but `nTerminal`/
`cTerminal` must not be specified as first/last arbitrary RESIDUE-role fragment: v0
`getNTerminal`/`getCTerminal` search the first/last amino-acid residue (`isAminoAcid()`),
so HCP-2b.1 should filter `FragmentRole::RESIDUE` plus `ResidueKind::AMINO_ACID` (or
explicitly split a generic first/last-residue helper from the v0-compatible terminal API).

1. **SOUND**: Full-subtree recursion plus derived `FragmentRole` correctly covers direct Chain→Residue and Chain→SecondaryStructure→Residue shapes; nested domain/group RESIDUE descendants should be collected by role, not excluded by fixed depth.
2. **FLAW**: `countChildren()`/`getChildContainer(i)` traversal is complete, skips atoms correctly, and has no tree-edge revisit risk, but first/last RESIDUE-role preorder does not match v0 terminal semantics because v0 skips non-amino residues.
3. **SOUND**: `std::vector<FragmentHandle>` by value is acceptable for HCP-2b.1; 24 B handles are cheap enough, and a callback/range layer can be added later without invalidating the simple query API.
4. **WEAK**: `MoleculeHandle` is the right entry with no SYSTEM row, and first-preorder `residueByID` matches v0, but the replacement surface remains weak until terminal semantics are fixed.
5. **SOUND**: Deferring the deprecated iterator alias to HCP-2b.2 is the right cut; it is consumer-compat work, while HCP-2b.1 can stay header-only with no store state or `.C`.
6. **SOUND**: No blocking encapsulation/naming risk found; public-header-only `structureQuery.h` and `FragmentHandle` returns for chain/SS/residue are consistent because roles disambiguate them.

## HCP-2b1-R1b design review
Verdict: **AGREE**. The R1 blocker is resolved; implementation may proceed.

A. **SOUND**: `nTerminal`/`cTerminal` now match v0 by selecting the first/last RESIDUE-role fragment whose `getResidueKind() == ResidueKind::AMINO_ACID`; this is the correct table mapping for v0 `Residue::isAminoAcid()` (`hasProperty(PROPERTY__AMINO_ACID)`).
B. **SOUND**: Splitting `firstResidue`/`lastResidue` introduces no naming/overload collision found, and reverse traversal over the same preorder child tree can correctly implement "last" by scanning children in reverse and returning the deepest/rightmost RESIDUE-role match.
C. **SOUND**: The required leading/trailing non-amino residue test is adequate because it distinguishes v0-compatible terminal selection from the unfiltered `firstResidue`/`lastResidue` helpers on both ends.

## HCP-2b1-CR code review
Verdict: **GO**. No blocking correctness, encapsulation, or v0-compatibility issue found
in the implemented HCP-2b.1 patch; `build-core` `StructureQuery_test` builds and passes.

1. **SOUND**: `nTerminal`/`cTerminal` filter RESIDUE-role fragments to
   `ResidueKind::AMINO_ACID`, matching v0 `getNTerminal`/`getCTerminal`
   `isAminoAcid()` semantics. Keeping the last amino-acid residue while walking forward
   preorder is equivalent to v0 `rbeginResidue()` over the same bidirectional residue
   order.
2. **SOUND**: `visitFragments_` walks every ordered child container via
   `countChildren()`/`getChildContainer(i)`, skips atom leaves through the null-container
   result, visits fragment containers once in preorder, and recurses through SS layers so
   direct chain residues and chain->SS residues are both found.
3. **SOUND**: Narrowing each child with `as<FragmentHandle>()` is correct for
   CHAIN/FRAGMENT/RESIDUE/NUCLEOTIDE/SECONDARY_STRUCTURE rows, and role filtering keeps
   the API H4-ready. A molecule-level child would not be visited as a fragment, but its
   subtree would still be traversed; no silent result corruption found.
4. **SOUND**: `residueByID` calls the role-asserting `getID()` only after
   `getFragmentRole() == FragmentRole::RESIDUE`; no query path calls a role-asserting
   getter on the wrong role.
5. **SOUND**: `structureQuery.h` includes only `containerHandle.h` plus `<vector>` and
   does not cross the `_moleculeStoreInternal.h` boundary; names and header-only inline
   definitions are consistent with the intended public API.
6. **SOUND**: `StructureQuery_test` pins both residue depths, ordered 5-residue preorder,
   terminal skipping of leading/trailing water, generic first/last returning water,
   `residueByID` hit/miss, `secondaryStructureOf` hit/miss, and empty/amino-free null
   terminals.
7. **SOUND**: No new lambda lifetime, handle-copy, missing-inline, recursion, or overload
   ambiguity issue found.

---

## HCP-2c — deferred mutation mirrors + role/property refinement (design-lock)

### Goal
Land the three H2 carry-overs the H2d review explicitly deferred (V22-H2D-REVIEW.md):
**(1)** container `setProperty`/`clearProperty` **role-refinement** (so
`MoleculeRole` stops returning `UNKNOWN` for non-protein/NA molecules); **(2)** the
`replace`/`insertParent` **materialise-the-new-member** mirror (rooting an *unmaterialised*
subtree into an already-materialised tree); **(3)** the **rooted full-subtree-replacement**
mirror (`set`/`operator=`). Still **dual existence** — v0 is the source of truth (D60);
these extend the forward-only v0 → ContainerTable mirror so the mirror stays faithful under
the remaining mutation surface. Lifts the H2d sweep scope restriction once landed.

### Current gap (verified surface map, 2026-05-24)
- `MoleculeStore::container_molecule_role_` (moleculeStore.C:636) switches on
  `ContainerKind`: PROTEIN→PROTEIN, NUCLEIC_ACID→NUCLEIC_ACID, **MOLECULE→UNKNOWN**
  (can't distinguish solvent/small-molecule — the v0 `Molecule::IS_SOLVENT` bit is a
  container property, unmirrored). `residue_kind` IS stored in payload (set at
  materialisation, atomContainer.C:51-61) and read by the handle.
- `ContainerPayload` (= 8 B, `static_assert`-pinned) has `id_offset`(4) +
  `insertion_code`(1) + `ss_type`(1) + `residue_kind`(1) + **1 free pad byte**.
- `replace` (composite.C:1201) mirrors via `mirrorRederiveOwnRow_()` on the new slot's
  parent — correct for an already-rooted member, **NOT** for an unmaterialised one.
- `insertParent` (composite.C:977, used by PDBFileDetails SS grouping) mirrors ONLY when
  the new parent is already materialised; the unmaterialised case is deliberately skipped
  (design note 1050-1067) → the deferred materialise-the-new-member.
- `Composite::set`/`AtomContainer::set`/`operator=` (composite.C:359, atomContainer.C:187/206)
  deep-clone or shallow-copy and **never touch the container row store**.
- Container `setProperty`/`clearProperty`: **no store mirror at all** (D59 unimplemented).
- `migrate_subtree_from` (moleculeStore.C:572) migrates topology + payload + names +
  selection; container properties not migrated (none stored yet).

### Decisions to lock

- **D-2c.1 — `molecule_role` stored in the payload pad byte.** Add
  `MoleculeRole molecule_role = MoleculeRole::UNKNOWN;` to `ContainerPayload` (fills the
  free pad byte → payload stays 8 B, the existing `static_assert` still holds). Set it at
  materialisation in `writeContainerScalars_`: `Protein`→PROTEIN, `NucleicAcid`→NUCLEIC_ACID,
  plain `Molecule` with `IS_SOLVENT`→SOLVENT else SMALL_MOLECULE. `container_molecule_role_`
  reads `payload.molecule_role` (drop the kind switch), mirroring exactly how `residue_kind`
  is stored + read. This is the visible win: `getMoleculeRole()` stops returning UNKNOWN.
- **D-2c.2 — honest role taxonomy (no over-claiming).** v0 `Molecule` carries ONLY the
  `IS_SOLVENT` identity bit (molecule.h:44) — there is NO molecule-level WATER/LIGAND/ION
  distinction in v0. So the refined molecule role is exactly {PROTEIN, NUCLEIC_ACID,
  SOLVENT, SMALL_MOLECULE}; WATER/LIGAND/ION stay residue-level (`ResidueKind`, already
  mirrored). Do NOT synthesise molecule-level WATER/LIGAND/ION the v0 tree can't justify.
- **D-2c.3 — identity-bit role-refinement mirror.** `AtomContainer::setProperty(NamedProperty)`
  / `setProperty(Property)` / `clearProperty(Property)` gain a guarded forward hook
  (`mirrorRefineRole_`): when the touched bit is an IDENTITY bit it re-derives + writes the
  payload role — `Molecule::IS_SOLVENT` → `molecule_role`; `Residue::PROPERTY__AMINO_ACID/
  __WATER/__NON_STANDARD` → `residue_kind`. No-op when unbound / being-destroyed (the same
  guard class as the existing scalar mirror). Non-identity properties are NOT mirrored
  (D-2c.4). Forward-only; v0 stays source of truth.
- **D-2c.4 — the GENERAL container-property bag is DEFERRED to H4 (the flip), not HCP-2c.**
  Rationale: through dual existence NO consumer reads arbitrary container properties from
  the store — the read-only handles expose only scalar identity + role + navigation, and v0
  `PropertyManager` remains the property source of truth (D60). A full per-row NamedProperty
  bag is parity substrate needed only when the store BECOMES the source of truth (H4). The
  H2d carry-over the review named is the *role-refinement* (D-2c.3), which D-2c.1/.3 deliver
  WITHOUT a general bag (role lives in the payload, derived at materialisation + refined on
  identity-bit change). Building the general bag now is speculative store state. **[Codex:
  challenge this descope — is any HCP-2/HCP-3 consumer going to need store-side container
  properties before H4?]**
- **D-2c.5 — materialise-the-new-member (`insertParent` / `replace`).** When a mutation
  roots a NEW container subtree that has no rows yet INTO an already-materialised parent,
  materialise that subtree via the existing recursive idempotent `materialiseContainer_`
  (system.C:257) and splice its root edge into the parent's child list (re-derive the
  parent's row from v0 order). Covers (a) `insertParent` with an unmaterialised new parent
  rooted under a materialised tree (the PDB SS-grouping case), and (b) `replace` where the
  replacement is an unmaterialised orphan. Guarded: only fires when the parent IS
  materialised (else the whole subtree materialises later at adoption, unchanged).
- **D-2c.6 — rooted full-subtree replacement (`set` / `operator=`).** When `set(deep)` /
  `operator=` overwrites a MATERIALISED container's content (clone replaces children), the
  old child rows must be released and the cloned subtree re-materialised under the same row.
  Hook in `AtomContainer::set` after the clone/copy: if `this` is materialised, clear the
  row's children (release the old child subtree rows, NOT the row itself — the handle to
  `this` stays valid, generation preserved), then materialise the new children + re-derive.
  **`persistentRead` is NOT separately hooked**: it is the same replacement class, but the
  AGREED PR-removal milestone (task #61) deletes the persistence stream framework, so
  `persistentRead` ceases to exist — building a hook for it would be throwaway work. (If
  task #61 is reordered after HCP-2c, the same D-2c.6 hook would serve it.)
- **D-2c.7 — cross-store move.** A move of a materialised subtree between two distinct live
  System stores is the materialise-into-dst (D-2c.5) + release-from-src (the existing
  `mirrorRemoveChild_` / `release_source_subtree_`) composition; no NEW mechanism. Audit
  whether any v0 API actually moves a materialised container across stores during dual
  existence (orphan→System adoption already works via `migrate_subtree_from`); if none does,
  note it covered-by-composition and add a targeted parity case rather than new code.
  **[Codex: is there a live cross-store-move path I'm missing?]**

### Step-by-step (each green + committed; rc-gated build + full ctest + Codex CR)
- **HCP-2c.1** — D-2c.1/.2/.3: `molecule_role` payload + materialisation derivation +
  `container_molecule_role_` reader + identity-bit role-refinement mirror. Extend
  `ContainerHandle_test` (a solvent Molecule → SOLVENT; flipping IS_SOLVENT refines the
  role) + `HierarchyParity_test` (role parity after a property flip). Self-contained,
  highest-value, lowest-risk → first.
- **HCP-2c.2** — D-2c.5: materialise-the-new-member for `insertParent`/`replace`. Parity:
  add the rooted-replace-with-orphan op the H2d sweep excluded (HierarchyParity_test:1161).
- **HCP-2c.3** — D-2c.6: rooted full-replacement (`set`/`operator=`) re-materialise. Parity:
  add the `set`/`operator=` op the H2d sweep excluded (HierarchyParity_test:1065).
- **HCP-2c.4** — D-2c.7: cross-store-move audit + parity case (code only if a live path
  needs it). Then lift the H2d scope-restriction comments + HCP-2 close-review (Codex).

### Scope / non-goals
- Still READ-only handles; no consumer migration; `ContainerKind` does NOT shrink (H4).
- General container-property bag deferred to H4 (D-2c.4); persistentRead hook not built
  (D-2c.6); SS-as-annotation is HCP-2d.

### Touch points
- `include/BALL/KERNEL/_moleculeStoreInternal.h` (payload `molecule_role`),
  `source/KERNEL/moleculeStore.C` (`container_molecule_role_` reader + setter),
  `source/KERNEL/atomContainer.C` (`writeContainerScalars_` role derivation +
  `mirrorRefineRole_` + `set` re-materialise hook),
  `source/CONCEPT/composite.C` (`insertParent`/`replace` materialise-new-member),
  `test/ContainerHandle_test.C` + `test/HierarchyParity_test.C` (+ lift sweep restrictions).
- ABI change (payload field) → FULL rebuild (libBALL + all test bins) before ctest.

### Gate
Codex **HCP-2c design AGREE** (challenge D-2c.4 descope + D-2c.7 cross-store) BEFORE code;
then implement 2c.1 → 2c.4 step-by-step, each rc-gated green + Codex code-review + commit.

## HCP-2c-R1 design review

Verdict: **AGREE-WITH-FIXES**. The payload role and HCP-2c scope are directionally right,
but code must not proceed with the current "AtomContainer setProperty hook covers identity
bits" and "cross-store move needs no new mechanism" claims. Required fixes before coding:
(1) make identity-bit role refinement fire from every real bit-write path, including JSON
property restore and mutable BitVector bypasses, or explicitly close those bypasses; (2)
add an owned release/rebind mechanism for cross-store materialised container moves; (3)
when copying/migrating rows, copy the new `payload.molecule_role`; (4) make `persistentRead`
sequencing explicit: either #61 lands first, or HCP-2c hooks persistentRead too.

1. **D-2c.1 payload byte: SOUND.** `ContainerPayload` is currently 7 bytes of fields plus
   one pad byte (`id_offset` + `insertion_code` + `ss_type` + `residue_kind`,
   `include/BALL/KERNEL/_moleculeStoreInternal.h:494`), and the role enums are u8-backed
   (`include/BALL/KERNEL/containerRole.h:42`), so adding `molecule_role` should keep the
   `sizeof(ContainerPayload) == 8` pin (`include/BALL/KERNEL/_moleculeStoreInternal.h:544`).
   No endianness concern exists for live memory, and JSON StoreFormat does not serialize
   `ContainerPayload` bytes; it serializes v0 molecule/property state separately
   (`source/KERNEL/systemJson.C:139`). Reading `payload.molecule_role` for PROTEIN/NA is
   sound only if materialisation writes it for every molecule-level kind and
   `ContainerTable::migrate_one_` copies it alongside id/insertion/ss/selection
   (`source/KERNEL/moleculeStore.C:504`).

2. **D-2c.2 honest taxonomy: SOUND.** v0 `Molecule` exposes only `IS_SOLVENT` as a
   molecule property (`include/BALL/KERNEL/molecule.h:42`); the solvent predicate also
   tests only that bit (`source/KERNEL/standardPredicates.C:151`). I found no v0
   molecule-level WATER/LIGAND/ION classifier. The honest mirrored set for HCP-2c is
   therefore PROTEIN, NUCLEIC_ACID, SOLVENT, SMALL_MOLECULE; WATER/LIGAND/ION remain
   unsupported at molecule role until a real source of truth exists.

3. **D-2c.3 identity-bit refinement mirror: FLAW.** Hooking only `AtomContainer`
   `setProperty`/`clearProperty` cannot cover all v0 bit writes because `PropertyManager`
   methods are non-virtual inline functions (`include/BALL/CONCEPT/property.h:444`,
   `include/BALL/CONCEPT/property.iC:304`), JSON restore takes a `PropertyManager&` and
   calls `pm.setProperty(bit)` (`source/KERNEL/propertyJson.C:100`,
   `source/KERNEL/propertyJson.C:147`), and callers can mutate the exposed BitVector
   directly (`include/BALL/CONCEPT/property.h:411`). This misses a real dual-existence path:
   `loadSystemJSON` inserts/materialises a `Molecule`, then restores its properties
   (`source/KERNEL/systemJson.C:423`, `source/KERNEL/systemJson.C:426`), so an
   `IS_SOLVENT` bit loaded from JSON would not refine the already-materialised row. Fix by
   moving the hook to the actual bit-write substrate, adding explicit post-restore
   resync/refinement for container PropertyManagers, and covering `toggleProperty` /
   `getBitVector()` bypass policy.

4. **D-2c.4 general property bag descope: SOUND.** I do not see an HCP-2/HCP-3 consumer
   that reads arbitrary container properties from the store table: handles expose roles and
   scalar payload only, `StructureQuery` reads role/residue kind (`include/BALL/KERNEL/structureQuery.h:141`),
   and JSON StoreFormat reads/writes v0 `PropertyManager` bags directly
   (`source/KERNEL/systemJson.C:139`, `source/KERNEL/systemJson.C:425`). Deferring the
   general NamedProperty container bag is deferred parity substrate, not a correctness
   blocker before H4, as long as D-2c.3 mirrors the identity bits that feed role columns.

5. **D-2c.5 materialise-the-new-member: WEAK.** The recursive materialiser is idempotent
   for same-store already-bound rows (`source/KERNEL/system.C:257`,
   `source/KERNEL/system.C:266`), and re-deriving the affected parent after
   `insertParent`/`replace` matches the current mirror design (`source/CONCEPT/composite.C:1068`,
   `source/CONCEPT/composite.C:1211`). The weak point is implementation plumbing and stale
   bindings: `materialiseContainer_` is file-local in `system.C`, while the new callers are
   in `composite.C`; make it a single shared helper, not a duplicate. Also ensure
   replace-with-orphan into a materialised parent releases/rebinds any source rows if the
   "orphan" is actually bound to another store; otherwise this collapses into D-2c.7's
   cross-store leak.

6. **D-2c.6 set/operator= re-materialise + persistentRead: WEAK.** The replacement shape
   is right: keep this row/generation, release old child subtree rows so aliases stale via
   generation bump (`source/KERNEL/moleculeStore.C:331`), then rebuild children from the
   cloned v0 tree. It needs an explicit recursive release helper for child container rows;
   `container_clear_children_` only clears a vector and does not free child rows
   (`source/KERNEL/moleculeStore.C:452`). Do not leave `persistentRead` as an implicit
   maybe: the stream path still exists today (`source/KERNEL/atomContainer.C:176`,
   `source/KERNEL/system.C:425`). Either require task #61 to land before HCP-2c, or add the
   same full-subtree replacement hook to `persistentRead` and delete it with #61.

7. **D-2c.7 cross-store move: FLAW.** There is a live v0 API path: appending/inserting a
   materialised subtree from one rooted `System` under another rooted container calls
   `Composite::appendChild`, which removes it from the old parent (`source/CONCEPT/composite.C:928`),
   then `System::adoptSubtree` materialises it into the destination store
   (`source/KERNEL/atomContainer.C:538`, `source/KERNEL/atomContainer.C:541`,
   `source/KERNEL/system.C:415`). That allocates/rebinds destination rows when
   `getContainerRowStore_() != dst`, but it does not release the source container subtree;
   `mirrorRemoveChild_` removes only the old parent edge (`source/CONCEPT/composite.C:216`),
   and `release_source_subtree_` is private to `migrate_subtree_from`
   (`source/KERNEL/moleculeStore.C:559`). Cross-store move therefore needs dedicated
   source-row release/rebind handling, plus a parity/stale-handle test.

8. **Sequencing / step-by-step: WEAK.** Start with the payload field, but 2c.1 must include
   a full rebuild and all row-copy/migration updates for the new byte. Then fix identity-bit
   refinement before relying on role tests, land materialise/release helpers before
   `replace`/`set`, and do the cross-store move fix before lifting H2d restrictions. The
   H2d carry-overs are otherwise all represented: role refinement, materialise-new-member,
   and rooted full replacement (`.planning/v2.2/V22-H2D-REVIEW.md:5`).

## HCP-2c-R1 fixes (design revision → supersedes the noted decisions)

Addressing HCP-2c-R1 (AGREE-WITH-FIXES). The four required fixes + the two WEAKs:

- **FIX-1 (D-2c.3 FLAW → revised) — role refinement fires at every REAL v0 path, raw poke
  documented out-of-contract.** v0 `PropertyManager::setProperty/clearProperty/toggleProperty`
  are non-virtual inline (property.h:444) and JSON restore writes bits via a
  `PropertyManager&` (propertyJson.C:100/147) after the container is already materialised
  (systemJson.C:423-426). So a single `AtomContainer::setProperty` override is insufficient.
  Revised mechanism — a `mirrorRefineRole_()` (re-derives `molecule_role`/`residue_kind`
  from the live v0 object) called from THREE sites:
  (a) materialisation (`writeContainerScalars_`) — captures the state at adoption;
  (b) the `AtomContainer` property API — override `setProperty(Property)`,
      `clearProperty(Property)`, `setProperty(const NamedProperty&)` AND `toggleProperty`
      to refine after the base call (the common runtime path);
  (c) an explicit **post-restore resync** at the JSON load site (systemJson.C ~426): after
      `restoreProperties(mol)` on a materialised container, call `mirrorRefineRole_()`.
  Direct `getBitVector()` mutation (property.h:411) is documented OUT of the mirror
  contract — identical to D60 (the mirror tracks the v0 API, not raw-memory pokes); no
  dual-existence consumer reads `molecule_role` as authoritative for a raw-poked molecule,
  and H4 recomputes it. (If a raw-poke path is later found in production code, it gets an
  explicit resync call — none exists today per the R1 grep.)
- **FIX-2 (D-2c.7 FLAW → real code item, NOT composition).** There IS a live cross-store
  move: rooted `appendChild`/insert of a materialised subtree from System A under System B
  → `adoptSubtree` materialises into B's store (atomContainer.C:538-541, system.C:415) but
  NEVER releases A's source rows (`mirrorRemoveChild_` removes only the old parent edge;
  `release_source_subtree_` is private to `migrate_subtree_from`). HCP-2c.4 adds a
  **dedicated cross-store release/rebind**: when adoption rebinds a subtree whose
  `getContainerRowStore_()` is a DIFFERENT live store, release the source subtree rows
  (expose a `release_container_subtree_(root)` on the table; reuse the
  `release_source_subtree_` logic) so source handles go stale (generation bump) and A's
  table doesn't leak. Add a stale-source-handle parity/`ContainerHandle`-style test.
- **FIX-3 (D-2c.1) — migrate copies `molecule_role`.** `ContainerTable::migrate_one_`
  (moleculeStore.C:504) must copy `payload.molecule_role` alongside id/insertion/ss/
  residue_kind/selection. If it copies the whole `ContainerPayload` by value this is
  automatic; verify and, if field-wise, add the field. Same for any row-copy in
  orphan→System migration.
- **FIX-4 (D-2c.6) — persistentRead IS hooked now, deleted with #61 (no sequencing dep).**
  Choose Codex option B: HCP-2c.3 applies the SAME full-subtree-replacement hook to
  `persistentRead` (atomContainer.C:176, system.C:425) so the mirror is faithful TODAY,
  with NO ordering dependency on task #61; #61 later deletes `persistentRead` + its hook
  along with the persistence framework. Add a recursive **`release_container_subtree_`**
  helper (the existing `container_clear_children_`, moleculeStore.C:452, only clears the
  edge vector — it does NOT free child rows); the re-materialise path must release the old
  child subtree rows (generation bump → stale aliases, moleculeStore.C:331) before
  rebuilding from the clone.
- **FIX-5 (D-2c.5 WEAK) — single shared materialiser, no duplicate.** Promote
  `materialiseContainer_` (file-local in system.C:257) to a shared internal entry (e.g. a
  `MoleculeStore::materialiseSubtree_(AtomContainer&)` or a shared `detail::` in a common
  TU) callable from BOTH system.C and composite.C, so `insertParent`/`replace`/`set`/cross-
  store all use ONE materialiser. A replace-with-orphan whose orphan is bound to ANOTHER
  store routes through the FIX-2 cross-store release/rebind.

### Revised step-by-step (supersedes the earlier list)
> STATUS: **2c.1 DONE** (commit 73212d856, Codex HCP-2c1-CR GO). **2c.2 NEXT.**
> Carry-over from 2c1-CR item 7 (WEAK): add a `migrate_one_` molecule_role +
> residue_kind parity assertion in 2c.4 (where cross-store migration is the focus).
- **HCP-2c.1** — D-2c.1/.2/.3(revised): `molecule_role` payload + `migrate_one_` copy
  (FIX-3) + materialisation derivation + `mirrorRefineRole_` from the AtomContainer property
  API + post-JSON-restore resync (FIX-1) + `container_molecule_role_` reads payload.
  ABI change → FULL rebuild. Tests: solvent-Molecule→SOLVENT, IS_SOLVENT flip refines,
  JSON-load solvent round-trip refines, parity after a property flip.
- **HCP-2c.2** — FIX-5 shared materialiser + D-2c.5 materialise-the-new-member
  (`insertParent`/`replace`, same-store). Parity: the rooted replace-with-(same-store-)orphan
  op H2d excluded.
- **HCP-2c.3** — `release_container_subtree_` helper (FIX-4) + D-2c.6 rooted full-replacement
  (`set`/`operator=` AND `persistentRead`) re-materialise. Parity: the `set`/`operator=` op
  H2d excluded; assert replaced-subtree handles go stale.
- **HCP-2c.4** — FIX-2 cross-store release/rebind + stale-source test; THEN lift the H2d
  sweep scope restrictions (HierarchyParity_test:1065/1161) + HCP-2 close-review (Codex).

### Gate (revised)
Codex **HCP-2c-R1b** tight confirmation that FIX-1..FIX-5 resolve the R1 FLAW/WEAK items →
AGREE before code.

## HCP-2c-R1b design review

Verdict: **AGREE** — the revised FIX-1..FIX-5 close the R1 FLAW/WEAK findings; implementation
of HCP-2c.1 proceeds.

A. **SOUND.** FIX-1 closes D-2c.3: materialisation covers initial identity bits,
AtomContainer-level property wrappers cover the real typed v0 mutation path, and the
post-JSON resync belongs immediately after `sys.insert(*m)` + `json_to_properties(*m, ...)`
at `source/KERNEL/systemJson.C:423-427`; no remaining production path was found that
materialises a container and then changes molecule/residue identity bits except raw
`getBitVector()` / `operator BitVector&` pokes, which are correctly out of the mirror
contract (D60).
B. **SOUND.** FIX-2 closes D-2c.7: live cross-store rooted adoption needs a dedicated
release/rebind, and exposing/reusing the `release_source_subtree_` post-order logic from
`source/KERNEL/moleculeStore.C:559` is the right primitive because it frees container rows,
clears migrated atom reverse edges, and makes source handles stale through release
generation bumps.
C. **SOUND.** FIX-4 closes D-2c.6: `container_clear_children_` only clears an edge vector
(`source/KERNEL/moleculeStore.C:452`), so full replacement needs a recursive
`release_container_subtree_`; routing `set`/`operator=` and current `persistentRead`
through it is correct, and released child rows go stale via `ContainerTable::release`
generation bump at `source/KERNEL/moleculeStore.C:331`.
D. **SOUND.** FIX-5 closes D-2c.5: one shared internal materialiser callable from
`system.C` and `composite.C` avoids duplicate adoption semantics, and replace-with-foreign-
orphan correctly routes through FIX-2 rather than a same-store rederive.
E. **SOUND.** FIX-3 and sequencing are sound: `migrate_one_` is field-wise today
(`source/KERNEL/moleculeStore.C:504`), so `payload.molecule_role` must be copied or the
whole payload copied; the revised 2c.1 -> 2c.4 order puts ABI/full rebuild first, then
helpers before replace/set, then cross-store release before lifting H2d restrictions.
F. **SOUND.** No new design blocker introduced by the fixes themselves; implementation
should preserve the full `PropertyManager::setProperty/clearProperty` overload surface when
adding AtomContainer property wrappers.

## HCP-2c1-CR code review

Verdict: **GO** — no implementation blocker found in the uncommitted HCP-2c.1 diff.

1. **SOUND.** `BALL::Property` is the right parameter type for the bit-property hook:
   it matches `PropertyManager::setProperty(Property)` (`BALL_SIZE_TYPE`) instead of
   accidentally binding to `AtomContainer::Property`; the `using` declarations keep the
   named/string overloads visible, and `Molecule::IS_SOLVENT` / residue enum callers still
   resolve to the bit overload without ambiguity. Caveat: these base methods are non-virtual,
   so this is static overload replacement for typed `AtomContainer`/derived calls, not
   polymorphic dispatch through `PropertyManager&`.
2. **SOUND.** FIX-1 coverage is complete for HCP-2c.1: materialisation derives the initial
   role, typed `AtomContainer::{set,clear,toggle}Property` calls resync after bit flips, and
   `loadSystemJSON` explicitly resyncs after `json_to_properties`; production grep found no
   bound identity-bit mutation through `getBitVector()` / `operator BitVector&`, so raw bit
   pokes remain acceptably out of contract.
3. **SOUND.** The resync cost is limited to already-bound containers; parse/build paths that
   stamp identity bits before `System::insert` hit the unbound no-op, atom-level hot loops are
   unaffected, and the few bound molecule/residue identity flips found are not tight loops.
4. **SOUND.** `container_molecule_role_` correctly prefers the stored payload and falls back
   only for legacy/hand-built PROTEIN and NUCLEIC_ACID rows; materialised plain `Molecule`
   rows now get SMALL_MOLECULE/SOLVENT, so real rows should not read UNKNOWN unless they were
   directly allocated and never scalar-synced.
5. **SOUND.** `migrate_one_` now copies both `molecule_role` and `residue_kind`; copying
   `residue_kind` fixes an existing field-wise migration drop and does not conflict with later
   derivation because migration is preserving source row state, not re-materialising from v0.
6. **SOUND.** The taxonomy is honest for v0: PROTEIN/NUCLEIC_ACID come from concrete types,
   SOLVENT comes only from `Molecule::IS_SOLVENT`, and the remaining plain `Molecule` case is
   SMALL_MOLECULE; no WATER/LIGAND/ION role is synthesized without v0 evidence.
7. **WEAK.** The new tests pin materialisation derivation, set/clear/toggle refinement,
   `MoleculeHandle` surfacing, and JSON post-restore resync; they do not directly pin
   `migrate_one_` role/residue-kind parity, so FIX-3 is code-reviewed rather than test-locked.
8. **SOUND.** No new ABI/layout, serialization, include-cycle, overload, or destruction-time
   issue found: `ContainerPayload` remains 8 B under the existing static assert, the field is
   not part of external JSON, KERNEL includes stay TU-local, and `mirrorResyncScalars_` already
   guards destruction/unbound rows.
