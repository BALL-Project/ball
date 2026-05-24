# V22 HCP-2 — collapsed handle API (design-lock, step-by-step)

**Status:** DESIGN-LOCK in progress. The user-visible heart of the collapse; still
**dual existence** (handles READ the HCP-1 role columns; v0 classes remain until H4).
Sub-steps (plan §6a): **HCP-2a** role-aware handles → **HCP-2b** `StructureQuery` →
**HCP-2c** deferred mirrors → **HCP-2d** SS-as-annotation. This doc locks **HCP-2a**
first (step-by-step); 2b–2d get their own lock when reached.

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
