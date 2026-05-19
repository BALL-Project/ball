# P3 — Atom-RTTI Removal: Per-Site Classification (Rev 2 post-R22)

**Phase:** P3.1 first deliverable.
**Status:** Revised after R22 corrections (commit 95b6f991d).
**Source:** R22 review findings + reality-check on NMR + public API.

## Strategy (revised post-R22)

**Original plan** (R22-corrected): retype processors from
`UnaryProcessor<Composite>` to `UnaryProcessor<Atom>`. Rejected
because:

- NMR cluster (13+ sites) is bound by `ShiftModule : UnaryProcessor<Composite>`
  base class. `ShiftModel` stores `std::list<ShiftModule*>` and
  dispatches via `(*it)->operator()(Composite&)`. Retyping
  individual derived processors breaks the base interface.
- `Selector` and `AtomVector` are public API + SIP-bound.
  Narrowing their Composite-facing signatures is an ABI break.
- `MOL2File::containsAtomChilds_` checks DIRECT children only;
  `beginAtom()` is recursive across the subtree — semantic
  mismatch.
- `Composite::apply<Atom>` is ALREADY specialised via `isAtom()`
  virtual + `static_cast` (composite.h:771 + composite.C:1911,
  1934). The classification's claim of "centralised
  dynamic_cast in apply<T>" was incorrect for the Atom case.

**Revised plan:** single `BALL::detail::compositeAsAtom_()` helper.
All 24 sites become callers of the helper. No public API
changes. No processor retyping. v2.2's inheritance flip
replaces ONE helper definition, not 24 sites.

```cpp
// new file include/BALL/CONCEPT/_composite_atom_cast.h (private)
// OR add to existing include/BALL/CONCEPT/composite.h public:
namespace BALL { namespace detail {
    /// Centralised v2.1 dynamic_cast<Atom*>(c) helper. The only
    /// CORE_ONLY-module Atom-RTTI usage allowed after P3.
    /// v2.2 inheritance flip replaces this with handle-based
    /// identity. Use this at every call site that needs to ask
    /// "is this Composite an Atom?" in v2.1 code.
    inline Atom* compositeAsAtom_(Composite* c) noexcept {
        return c ? dynamic_cast<Atom*>(c) : nullptr;
    }
    inline const Atom* compositeAsAtom_(const Composite* c) noexcept {
        return c ? dynamic_cast<const Atom*>(c) : nullptr;
    }
} }
```

## Per-site refactor (all 24 sites use the same pattern)

Every site replaces its inline `dynamic_cast<Atom*>(...)` or
`if (RTTI::isKindOf<Atom>(&c)) ... static_cast<Atom*>(...)` with
`if (Atom* a = detail::compositeAsAtom_(&c)) { ... }`.

For paired `isKindOf + dynamic_cast` sites (e.g.
`empiricalHSShiftProcessor.C:318+320`), both go.

| # | Site | File:Line | Notes |
|---|---|---|---|
| 1 | `Selector::operator()(Composite&)` | source/KERNEL/selector.C:87 | Replace `isKindOf+cast` with helper. Selector stays `UnaryProcessor<Composite>`. |
| 2 | `GeometricProperties::operator()(Composite&)` | source/STRUCTURE/geometricProperties.C:202 | Same. |
| 3 | `DisulfidBondProcessor` first branch | source/STRUCTURE/disulfidBondProcessor.C:224 | Two `isKindOf` calls per branch → two helper calls; condition becomes `(atom1=helper(c1); atom2=helper(c2); if (atom1 && atom2) ...)`. |
| 4 | `DisulfidBondProcessor` second branch | source/STRUCTURE/disulfidBondProcessor.C:369 | Same pattern. |
| 5 | `MOL2File::containsAtomChilds_` | source/FORMAT/MOL2File.C:995 | Keep direct-children iteration via `beginChildComposite/endChildComposite` (semantic preserved); helper replaces the cast. |
| 6 | `AtomVector::set(const Composite&, bool)` | source/MOLMEC/COMMON/atomVector.C:74 | **Don't change ctor signature** (public API). Use `AtomConstIterator(composite)` internally to iterate atoms; or keep the cast and route through the helper. AtomConstIterator path is cleaner because it removes the iteration boilerplate too. |
| 7-9 | `ShiftModel1D::operator()(Composite&)` (3 lines pair) | source/NMR/shiftModel1D.C:223,225 | `isKindOf` + `dynamic_cast` pair → single `compositeAsAtom_` call. ShiftModule base stays `UnaryProcessor<Composite>`. |
| 10 | `RandomCoilShiftProcessor::operator()` | source/NMR/randomCoilShiftProcessor.C:68 | Same. |
| 11 | `AnisotropyShiftProcessor::operator()` | source/NMR/anisotropyShiftProcessor.C:299 | The `!isKindOf<Atom>` filter — invert with `if (!helper(&c)) return CONTINUE;`. |
| 12 | `CreateSpectrumProcessor::operator()` | source/NMR/createSpectrumProcessor.C:118 | Same. |
| 13 | `HaighMallionShiftProcessor::operator()` | source/NMR/haighMallionShiftProcessor.C:426 | Same. |
| 14 | `ShiftModel::operator()` cast | source/NMR/shiftModel.C:246 | Same. |
| 15 | `HBondShiftProcessor::operator()` filter | source/NMR/HBondShiftProcessor.C:412 | Same. |
| 16-17 | `EmpiricalHSShiftProcessor::operator()` filter+cast pair | source/NMR/empiricalHSShiftProcessor.C:318,320 | Helper. |
| 18 | `JohnsonBoveyShiftProcessor::operator()` filter | source/NMR/johnsonBoveyShiftProcessor.C:473 | Helper. |
| 19 | `EFShiftProcessor::operator()` filter | source/NMR/EFShiftProcessor.C:314 | Helper. |
| 20-21 | `ShiftModel2D::operator()` filter+cast pair | source/NMR/shiftModel2D.C:375,377 | Helper. |
| 22 | `Experiment::operator()` cast | source/NMR/experiment.C:36 | Helper. |
| 23 | `ClearShiftProcessor::operator()` | source/NMR/clearShiftProcessor.C:29 | Helper. |
| 24 | `MoleculeObjectCreator::convert` | source/CONCEPT/moleculeObjectCreator.C:73 | Helper. |
| 25 | `MolecularInformation::collect` | source/CONCEPT/molecularInformation.C:79 | Helper. |

(Note: 25 entries above because some original sites are pairs;
the underlying file:line count is still 24 RTTI uses.)

## Naming convention (R22-P22-2 fix)

Helper lives in `BALL::detail` namespace per existing JSON helpers'
convention (`detail::properties_to_json`, etc.). Not
`BALL::v21_internal` — that namespace doesn't exist in tree.

Helper header location: extend `include/BALL/CONCEPT/composite.h`
with a `namespace detail { ... }` block at the bottom (after the
Composite class definition + necessary `class Atom` forward decl).
This avoids a new header file while keeping the helper accessible
from any TU that already includes composite.h. The forward decl
of Atom is sufficient for the inline helper signature; callers
that USE the helper must include `<BALL/KERNEL/atom.h>` for the
return type to be complete (standard incomplete-type gate).

## Sub-phase mapping (revised)

P3 collapses to **3 commits** (down from ~5-7):

| Commit | Subject | Sites |
|---|---|---|
| **P3.2** | Introduce `detail::compositeAsAtom_` helper + apply to 5 non-NMR sites (Selector, GeometricProperties, DisulfidBond×2, MOL2File) | 6 sites |
| **P3.3** | Apply to 13 NMR sites + CONCEPT (moleculeObjectCreator + molecularInformation) | 15 sites |
| **P3.4** | AtomVector refactor (use AtomConstIterator) — separate commit because it changes iteration mechanism, not just the cast | 1 site |
| **P3.5** | (skipped — no Python work) |
| **P3.6** | CI grep gate + R23 close review | — |

CI grep gate exclusions:
- `include/BALL/CONCEPT/composite.h` (helper definition +
  `Composite::apply<Atom>` specialization region)
- `source/CONCEPT/composite.C` (apply specialization bodies)
- `source/VIEW/**` + `source/APPLICATIONS/**` (V21-VIEW-RTTI
  backlog per D37)

## Test gates per commit

Each P3 commit:
1. Build clean (no new warnings).
2. The directly-impacted module's tests pass.
3. Full CORE_ONLY ctest after each commit.
4. CI grep gate added after P3.3; PASS on all subsequent
   commits.

## Anti-pattern explicitly rejected (D41.1)

`Composite::isAtom_()` virtual on Composite. v2.2 inheritance
flip removes Atom : Composite — any code relying on a Composite
virtual to identify Atoms becomes invalid.

**NOTE post-R22:** `Composite::isAtom()` (no trailing underscore)
already EXISTS in v0 (composite.h:771). It returns false by
default and Atom overrides to true. `Composite::apply<Atom>` uses
it internally via `static_cast`. This existing virtual is
**preserved** in v2.1 because `Composite::apply<Atom>` depends on
it; v2.2 inheritance flip will refactor both `apply<T>` and the
helper together.

## Risk register (P3-scoped, revised)

- **R-P3.A (LOW):** the `compositeAsAtom_` helper is technically
  a new public API surface (lives in `BALL::detail`). Document
  in V21-DECISIONS.md as a v2.2-removable name. Add to
  `.planning/v2.1/BACKLOG.md` as `V21-COMPOSITEASATOM-REMOVAL`.
- **R-P3.B (LOW):** the helper's `dynamic_cast` is still a
  dynamic_cast — the v2.1 RTTI removal is "consolidate not
  eliminate." Document this honestly in RELEASE-NOTES-v2.1.md
  framing.
- **R-P3.C (LOW):** AtomVector's `AtomConstIterator` refactor
  may have edge cases where the iterator behaves differently
  from the v0 cast loop (e.g. on non-AtomContainer composites
  passed in). Mitigation: keep both behaviors testable; if
  refactor changes any observable output, revert P3.4 and just
  use the helper in place.

## Open questions (now closed per R22)

1. Helper namespace: **`BALL::detail`** (R22 P22-2 fix).
2. NMR commit granularity: **all NMR in one commit** (no
   processor retyping needed; pure helper substitution).
3. CI gate exclusions: **`include/BALL/CONCEPT/composite.h` +
   `source/CONCEPT/composite.C` + VIEW + APPLICATIONS** (R22
   P22-6 fix).

## Next action

Spawn R22b on this revised classification. Expected GO given the
simplified scope (helper substitution at 24 sites; no API
changes; no processor retyping).
