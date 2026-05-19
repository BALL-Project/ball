# P3 — Atom-RTTI Removal: Per-Site Classification

**Phase:** P3.1 (first deliverable).
**Status:** DRAFT (R22 planning review pending).
**Source:** `.planning/v2.1/V21-ROADMAP.md` P3 section + D41.1.

## Method

Each of the 24 Atom-RTTI sites in CORE_ONLY modules is classified
into one of three categories:

- **(T) Typed-call-path-available**: caller's context already
  knows the parameter is an Atom; can refactor to take `Atom*` /
  `Atom&` directly. No runtime discrimination needed.
- **(S) Store-backed-identity**: caller has a `Composite*` but
  the surrounding code can use the store's `back_ptr_[i]`,
  `getStore()`, or `store_idx_` for type/identity confirmation.
  No `dynamic_cast`, but uses store side-state.
- **(V) Visitor-needed**: the site is genuinely walking a
  mixed-kind Composite tree and needs to filter to Atom nodes.
  Refactor to `UnaryProcessor<Atom>` (typed Processor) and call
  `composite.apply(typed_processor)`. The `Composite::apply<T>`
  template still uses `dynamic_cast` internally — that's the
  centralized RTTI use that v2.2 replaces.

**Per D41.1:** explicitly do NOT add `Composite::isAtom_()`
virtual. Per-site refactor only.

## Sites (24 total)

| # | Site | File:Line | Category | Refactor approach |
|---|---|---|---|---|
| 1 | `Selector::operator()(Composite&)` | source/KERNEL/selector.C:87 | **V** | This is a `KernelPredicate<Composite>` selecting by predicate tree. The body currently does `if (RTTI::isKindOf<Atom>(&c)) { Atom& atom = static_cast<Atom&>(c); ... }`. Refactor: change to a typed dispatch — Selector::operator()(Atom&) overload taking Atom directly + framework dispatch routes only atoms here. **Sub-option:** if Selector's existing API contract requires it to accept Composite (for non-Atom predicates), keep the cast but route through `composite.apply<Atom>(*this)` which centralizes the dynamic_cast in `Composite::apply`. |
| 2 | `GeometricProperties::operator()(Composite&)` (counts atoms in subtree) | source/STRUCTURE/geometricProperties.C:202 | **V** | UnaryProcessor walking composite tree; filter to Atom via apply<Atom>. |
| 3 | `DisulfidBondProcessor` first branch | source/STRUCTURE/disulfidBondProcessor.C:224 | **V** | Compares two composites; if both atoms, evaluate disulfide. Pair-wise visitor (both arms must be Atom). Two RTTI calls in one expression — refactor to `(atom1 = compositeAsAtom_(c1); atom2 = compositeAsAtom_(c2); if (atom1 && atom2) ...)`. The `compositeAsAtom_` helper is a single dynamic_cast in source/CONCEPT/composite.iC or similar; centralizes the two callsites. v2.2 replaces the helper. |
| 4 | `DisulfidBondProcessor` second branch | source/STRUCTURE/disulfidBondProcessor.C:369 | **V** | Same pattern as #3; share the `compositeAsAtom_` helper. |
| 5 | `MOL2File::write` iterating children | source/FORMAT/MOL2File.C:995 | **V** | Walking child composites of an AtomContainer; only Atom kids get written as MOL2 atom records. Refactor: iterate via `container.beginAtom() ... container.endAtom()` (AtomContainer already has typed atom iteration). NO dynamic_cast needed. |
| 6 | `AtomVector` ctor populating from composite | source/MOLMEC/COMMON/atomVector.C:74 | **T** | AtomVector is conceptually "vector of Atom*". Currently iterates a composite container and casts each. Refactor: take an AtomContainer reference and use its typed Atom iteration (begin_atom/end_atom). |
| 7 | `ShiftModel1D::operator()(Composite&)` filter | source/NMR/shiftModel1D.C:223 | **V** | UnaryProcessor<Composite>; filter to Atom. Refactor to UnaryProcessor<Atom>. |
| 8 | `ShiftModel1D::operator()(Composite&)` cast | source/NMR/shiftModel1D.C:225 | **V** | Same site as #7 (pair). Removed together. |
| 9 | `RandomCoilShiftProcessor::operator()` | source/NMR/randomCoilShiftProcessor.C:68 | **V** | UnaryProcessor<Composite> → UnaryProcessor<Atom>. |
| 10 | `AnisotropyShiftProcessor::operator()` filter | source/NMR/anisotropyShiftProcessor.C:299 | **V** | UnaryProcessor<Composite> → UnaryProcessor<Atom>. |
| 11 | `CreateSpectrumProcessor::operator()` | source/NMR/createSpectrumProcessor.C:118 | **V** | UnaryProcessor<Composite> → UnaryProcessor<Atom>. |
| 12 | `HaighMallionShiftProcessor::operator()` | source/NMR/haighMallionShiftProcessor.C:426 | **V** | UnaryProcessor<Composite> → UnaryProcessor<Atom>. |
| 13 | `ShiftModel::operator()` cast | source/NMR/shiftModel.C:246 | **V** | UnaryProcessor<Composite> → UnaryProcessor<Atom>. |
| 14 | `HBondShiftProcessor::operator()` filter | source/NMR/HBondShiftProcessor.C:412 | **V** | UnaryProcessor<Composite> → UnaryProcessor<Atom>. |
| 15 | `EmpiricalHSShiftProcessor::operator()` filter | source/NMR/empiricalHSShiftProcessor.C:318 | **V** | UnaryProcessor<Composite> → UnaryProcessor<Atom>. |
| 16 | `EmpiricalHSShiftProcessor::operator()` cast | source/NMR/empiricalHSShiftProcessor.C:320 | **V** | Same site as #15 (pair). Removed together. |
| 17 | `JohnsonBoveyShiftProcessor::operator()` filter | source/NMR/johnsonBoveyShiftProcessor.C:473 | **V** | UnaryProcessor<Composite> → UnaryProcessor<Atom>. |
| 18 | `EFShiftProcessor::operator()` filter | source/NMR/EFShiftProcessor.C:314 | **V** | UnaryProcessor<Composite> → UnaryProcessor<Atom>. |
| 19 | `ShiftModel2D::operator()` filter | source/NMR/shiftModel2D.C:375 | **V** | UnaryProcessor<Composite> → UnaryProcessor<Atom>. |
| 20 | `ShiftModel2D::operator()` cast | source/NMR/shiftModel2D.C:377 | **V** | Same site as #19 (pair). |
| 21 | `Experiment::operator()` cast | source/NMR/experiment.C:36 | **V** | UnaryProcessor<Composite> → UnaryProcessor<Atom>. |
| 22 | `ClearShiftProcessor::operator()` | source/NMR/clearShiftProcessor.C:29 | **V** | UnaryProcessor<Composite> → UnaryProcessor<Atom>. |
| 23 | `MoleculeObjectCreator::convert` | source/CONCEPT/moleculeObjectCreator.C:73 | **V** | Type-discrimination in a serialization path. Use the `compositeAsAtom_` helper (centralized). |
| 24 | `MolecularInformation::collect` | source/CONCEPT/molecularInformation.C:79 | **V** | Type-discrimination for displaying info. Use `compositeAsAtom_` helper. |

## Category distribution

- (T) Typed-call-path: **1** site (#6 AtomVector)
- (S) Store-backed-identity: **0** sites
- (V) Visitor-needed: **23** sites (most via UnaryProcessor<Atom> retype; some via compositeAsAtom_ helper)

## Two refactor patterns

### Pattern A — Retype `UnaryProcessor<Composite>` to `UnaryProcessor<Atom>`

Most NMR shift processors follow this template:

```cpp
// BEFORE:
class FooProcessor : public UnaryProcessor<Composite> {
public:
    Processor::Result operator()(Composite& composite) override {
        if (RTTI::isKindOf<Atom>(&composite)) {
            Atom& atom = dynamic_cast<Atom&>(composite);
            // ... atom-specific work ...
        }
        return Processor::CONTINUE;
    }
};

// AFTER (P3):
class FooProcessor : public UnaryProcessor<Atom> {
public:
    Processor::Result operator()(Atom& atom) override {
        // ... atom-specific work (no cast needed) ...
        return Processor::CONTINUE;
    }
};
```

**Caller change:** code that did `system.apply(foo_processor)` for
a `UnaryProcessor<Composite>` continues to work because
`Composite::apply<T>` is templated. The template-internal dispatch
still uses RTTI (one centralised dynamic_cast in the apply
machinery), but the 23 callsite-level RTTI uses go away.

### Pattern B — `compositeAsAtom_()` helper (for non-processor sites)

Sites #1, #3, #4, #23, #24 don't fit the simple processor retype.
They have local logic that needs an `Atom*` from a `Composite*` in
non-traversal contexts. Introduce a helper:

```cpp
// new helper in source/CONCEPT/composite.C (already includes
// internal headers per D31b; appropriate location):
namespace BALL { namespace v21_internal {
    // Centralised Composite->Atom cast. v2.2 replaces this with
    // store-handle-based identity. ALL Atom-RTTI in CORE_ONLY
    // code routes through this helper after P3.
    inline Atom* compositeAsAtom_(Composite* c) {
        return c ? dynamic_cast<Atom*>(c) : nullptr;
    }
} }
```

Callers replace `dynamic_cast<Atom*>(c)` with
`v21_internal::compositeAsAtom_(c)`.

After P3, the CI grep gate enforces:
- No `dynamic_cast<.*Atom.*>` in source/{KERNEL,STRUCTURE,FORMAT,
  QSAR,MOLMEC,SCORING,SOLVATION,DOCKING,ENERGY,NMR,CONCEPT}/.
- No `isKindOf<Atom>` in the same set.
- EXCEPTION: the `compositeAsAtom_` helper itself (source/CONCEPT/
  composite.C) and `Composite::apply<T>` template (header).

## Sub-phase mapping to commits

Per V21-ROADMAP.md:
- **P3.2** (typed-call-path): refactor #6 (AtomVector ctor) — 1 commit.
- **P3.3** (store-backed-identity): no sites — skip phase.
- **P3.4** (visitor pattern): refactor #1-5, #7-24 — 23 sites in ~5-7 commits grouped by module (NMR batch, STRUCTURE batch, FORMAT batch, KERNEL batch, CONCEPT batch). Each commit lands the pattern A retype OR introduces and applies the compositeAsAtom_ helper.
- **P3.5** (SIP Python bindings): not in CORE_ONLY; deferred to v2.1.x or v2.2.
- **P3.6** (close review R23): full ctest + grep gate enforcement.

## Test gates per commit

Each P3 commit must:
1. Build clean (no warnings beyond the v2.0 baseline set).
2. Pass the directly-impacted test (e.g. `HaighMallionShiftProcessor_test` for #12).
3. Pass full CORE_ONLY ctest after the last commit in the batch.
4. CI grep gate (added with the helper introduction) passes on the
   final commit — no RTTI in CORE_ONLY sources outside the
   documented exceptions.

## Anti-pattern explicitly rejected (D41.1)

`Composite::isAtom_()` virtual on Composite. The v2.2 inheritance
flip removes Atom : Composite — any code relying on a Composite
virtual to identify Atoms becomes invalid. Per-site refactor is
more work but produces correct v2.2-ready code.

## Risk register (P3-scoped)

- **R-P3.A (LOW):** UnaryProcessor<Atom> retype may break callers
  that pass these processors via `UnaryProcessor<Composite>&`
  references (LSP-narrow). Mitigation: per-site grep for the
  exact processor's usage; refactor caller as needed.
- **R-P3.B (LOW):** AtomVector refactor (#6) changes ctor
  signature from `(Composite*)` to `(AtomContainer&)`. Callers
  may exist that pass non-AtomContainer Composites. Mitigation:
  audit callers; provide a converting overload if needed.
- **R-P3.C (LOW):** the `compositeAsAtom_` helper is a v2.1-only
  bridge that v2.2 must replace. Mitigation: document in V21-
  DECISIONS.md as a v2.2-removable name; add to backlog.

## Open questions for R22 planning review

1. Should `compositeAsAtom_` live in `namespace BALL` (public-ish)
   or in `namespace BALL::v21_internal` (clearly transient)?
2. Should each NMR processor retype be a separate commit, or
   batched by module (one commit for all NMR shift processors)?
   Trade-off: granular blame vs. atomic test surface.
3. Does the CI grep gate need to allow VIEW + APPLICATIONS
   sites (54 per D37 / V21-VIEW-RTTI), or do we add an exclude
   list? Roadmap says VIEW excluded — confirm CI rule.

## Next action

Spawn R22 (Codex CLI) on this classification + V21-ROADMAP P3
section. If R22 GO, execute P3.2 → P3.4 → P3.6 sequentially.
