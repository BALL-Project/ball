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
