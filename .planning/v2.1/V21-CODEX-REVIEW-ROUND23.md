# Track A v2.1 Cluster R23 — Codex CLI Round 23 (2026-05-19)

**Status:** Complete
**Verdict:** NEEDS-FIXES
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.1 P3 close review

## Probe answers

**P23-1. Helper correctness:** `BALL::detail::compositeAsAtom_(Composite*)` is semantically equivalent to the old `RTTI::isKindOf<Atom>(&c) ? RTTI::castTo<Atom>(c) : nullptr` for v2.1. `include/BALL/COMMON/rtti.h` implements `RTTI::isKindOf<T>(u)` as `dynamic_cast<const T*>(u) != 0` and `RTTI::castTo<T>(u)` as `const_cast<T*>(dynamic_cast<const T*>(&u))`; it does not use unsafe `static_cast`.

**P23-2. shiftModel1D / shiftModel2D short-circuit:** The new `if (Atom* atom = !system_ ? detail::compositeAsAtom_(&composite) : nullptr)` is equivalent to `if (!system_ && RTTI::isKindOf<Atom>(&composite))` followed by the cast. In both forms, the Atom check is only evaluated when `system_` is null, and the branch body only runs for Atom composites.

**P23-3. HBond / EF / JohnsonBovey:** Given an `RTTI::isKindOf<Atom>` filter, replacing `RTTI::castTo<Atom>(object)` with the helper returns the same `Atom*`, because both are `dynamic_cast`-based. Those three refactors are behavior-preserving.

**P23-4. moleculeObjectCreator chain:** The `PersistentObject& -> dynamic_cast<Composite*> -> compositeAsAtom_()` chain is equivalent for Atoms. `Atom` inherits `Composite`, and `Composite` inherits `PersistentObject`, so any `PersistentObject&` that is actually an Atom also successfully casts to `Composite*`, then to `Atom*`. A non-Composite PersistentObject returns null before the helper.

**P23-5. AtomVector const-cast:** `const_cast<Atom*>(detail::compositeAsAtom_(&*it))` is semantically identical to `const_cast<Atom*>(dynamic_cast<const Atom*>(&*it))`; the const overload of the helper returns the same `const Atom*` result.

**P23-6. CI grep gate regex correctness:** The gate is not robust enough for P3 close.

- It catches `dynamic_cast<Atom *>`, `dynamic_cast< Atom*>`, and the common `dynamic_cast<const Atom *>` forms in local testing.
- It does not catch `dynamic_cast<Atom*const>`.
- It does not catch multiline `dynamic_cast<\n  Atom*\n>` forms.
- It does not catch `RTTI::castTo<Atom>`, which is a related Atom RTTI cast and is implemented via `dynamic_cast`.
- Current tree still has CORE_ONLY `RTTI::castTo<Atom>` hits outside `source/CONCEPT/composite.C`: `source/CONCEPT/molecularInformation.C:198`, `source/NMR/anisotropyShiftProcessor.C:305`, `source/FORMAT/HINFile.C:462`, `source/STRUCTURE/HBondProcessor.C:176`, and `source/KERNEL/standardPredicates.C:168`.

The first two are especially relevant because they are in touched P3 sites and still re-enter Atom RTTI immediately after helper-based classification. `anisotropyShiftProcessor.C:305` should reuse the helper result instead of calling `RTTI::castTo<Atom>(composite)`. `molecularInformation.C:198` can route the `TYPE__ATOM` name branch through `detail::compositeAsAtom_(&composite)` as well.

**P23-7. VIEW + APPLICATIONS exclusion:** VIEW and APPLICATIONS are not touched by P3 and still contain old Atom RTTI patterns, as expected for the Phase 4/D37 backlog. Verified examples include `source/VIEW/KERNEL/mainControl.C`, `source/VIEW/MODELS/*`, `source/VIEW/DIALOGS/atomOverview.C`, and others. The P3 changes do not break those sites; they remain excluded from the CORE_ONLY gate.

**P23-8. Test coverage:** The following tests explicitly exercise touched behavior:

- `test/Selector_test.C`: `Selector::operator()(Composite&)`, selected atom count/list behavior.
- `test/GeometricProperties_test.C`: geometric processors over composite traversal.
- `test/DisulfidBondProcessor_test.C`: `connect(Atom*)`, `disconnect()`, and residue/atom connect paths.
- `test/MOL2File_test.C`: MOL2 read/write and subfragment/static set handling, including direct child handling around `containsAtomChilds_`.
- `test/AtomVector_test.C`: assignment from `Composite`, `set(const Composite&, bool)`, selected-only behavior, position save/reset.
- `test/ShiftModel_test.C`: standard shift model registration and `chemical shift/BPTI`, which exercises `ShiftModel` plus registered modules.
- `test/RandomCoilShiftProcessor_test.C`, `test/AnisotropyShiftProcessor_test.C`, `test/CreateSpectrumProcessor_test.C`, `test/HaighMallionShiftProcessor_test.C`, `test/JohnsonBoveyShiftProcessor_test.C`, `test/EFShiftProcessor_test.C`, `test/ClearShiftProcessor_test.C`: direct operator/module tests for those processors.

Coverage gaps remain: I found no explicit test file for `HBondShiftProcessor`, `EmpiricalHSShiftProcessor`, `ShiftModel1D`, `ShiftModel2D`, `SimpleExperiment1D`, `MolecularInformation`, or `MoleculeObjectCreator`. Some may be indirectly exercised through `ShiftModel_test` or VIEW code paths, but there is no named direct test in `test/`.

**P23-9. v2.2 forward-compat:** The call-site pattern is forward-compatible where the Atom decision is expressed as `if (Atom* atom = detail::compositeAsAtom_(&c))`. A v2.2 store-handle identity implementation can preserve that interface. The two remaining touched-site `RTTI::castTo<Atom>` uses weaken this property and should be removed before P4.

**P23-10. Overall:** P3 is close but not a clean close yet. The helper substitution itself is semantically sound; the close blocker is enforcement/completeness, not runtime behavior.

## Overall verdict

**NEEDS-FIXES before P4.**

Must-fix before P4:

1. Replace the remaining touched-site Atom RTTI casts:
   - `source/NMR/anisotropyShiftProcessor.C:305` should bind `const Atom* patom` from `detail::compositeAsAtom_(&composite)` and reuse it.
   - `source/CONCEPT/molecularInformation.C:198` should use the helper in the `TYPE__ATOM` branch.
2. Strengthen the D41.1 gate to include `RTTI::castTo<Atom>` and document/handle any deliberate non-Composite-derived exceptions. As written, the gate can pass while Atom RTTI remains in CORE_ONLY modules.
3. Decide whether `source/KERNEL/standardPredicates.C:168`, `source/STRUCTURE/HBondProcessor.C:176`, and `source/FORMAT/HINFile.C:462` are legitimate out-of-scope iterator casts or should also route through the helper. The gate should make that decision explicit instead of silently missing them.

v2.1.x cleanup:

- Consider direct smoke tests for `MolecularInformation`, `MoleculeObjectCreator`, `SimpleExperiment1D`, `ShiftModel1D`, `ShiftModel2D`, `HBondShiftProcessor`, and `EmpiricalHSShiftProcessor`.
- Consider making the grep gate use `grep -E`/`perl` with a clearer expression and a small comment listing unsupported multiline cases if multiline matching remains intentionally out of scope.

Recommendation: fix the two touched-site casts and the CI gate, rerun the grep gate plus focused NMR/CONCEPT tests, then re-run the P3 close review. Do not enter P4 on the current gate because it does not enforce the stated P3 invariant.
