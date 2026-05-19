# Track A v2.1 Cluster R22 — Codex CLI Round 22 (2026-05-19)

**Status:** Complete
**Verdict:** NEEDS-FIXES
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.1 P3 planning review

## Probe answers

| # | Probe | Verdict | Notes |
|---|---|---|---|
| P22-1 | Pattern A correctness | NEEDS-FIXES | `Composite::apply(UnaryProcessor<T>&)` routes to preorder traversal (`include/BALL/CONCEPT/composite.h:2132`). Generic `T` machinery uses `dynamic_cast<T*>` (`composite.h:1695` and other template bodies), so typed dispatch does recursively filter mixed trees. However `T=Atom` has explicit non-template overloads in `source/CONCEPT/composite.C:1904` and `:1930`; those use `Composite::isAtom()` plus `static_cast<Atom*>`, not `dynamic_cast`. Also, the NMR processors are not standalone: they derive through public `ShiftModule : UnaryProcessor<Composite>` (`include/BALL/NMR/shiftModule.h:41`) and `ShiftModel` stores `std::list<ShiftModule*>` (`include/BALL/NMR/shiftModel.h:45`) and calls `(*it)->operator()(composite)` (`source/NMR/shiftModel.C:258`). Simple per-class retyping would break that base interface. |
| P22-2 | Pattern B helper namespace | NEEDS-FIXES | `BALL::v21_internal` is not an existing convention in the current tree. Existing internal helpers use `BALL::detail` in JSON code (`source/KERNEL/moleculeStoreJson.C`, `include/BALL/KERNEL/propertyJson.h`). If the helper must be shared across modules, declare it in a small internal header and define it once in a source file. Prefer `BALL::detail::compositeAsAtom_` unless the team intentionally wants a new milestone-tagged namespace, in which case document the convention in the classification/decision notes. |
| P22-3 | AtomVector ctor refactor | NEEDS-FIXES | In-tree constructor call sites found are compatible with `AtomContainer` (`System`, `Molecule`), but the public C++ and SIP APIs currently accept `Composite` (`include/BALL/MOLMEC/COMMON/atomVector.h:63`, `:97`; `source/PYTHON/EXTENSIONS/BALL/atomVector.sip:9`). Changing the signature outright is an API break. Also local implementation can avoid site-local RTTI without narrowing the API by using `AtomConstIterator(composite)` because `AtomIteratorTraits` already binds to any `Composite` and filters via `KernelPredicate<Atom>`. |
| P22-4 | MOL2File typed atom iteration | GO-with-correction | `AtomContainer` does provide `beginAtom()/endAtom()` via `BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Atom)` (`include/BALL/KERNEL/atomContainer.h:395`). But the classified site is `containsAtomChilds_`, which checks only direct children via `beginChildComposite()` (`source/FORMAT/MOL2File.C:989`). Replacing it with recursive `beginAtom()` changes semantics if nested atom containers exist. Use direct child iteration with the central helper, or verify recursive behavior is intended before changing it. |
| P22-5 | NMR shift processor retypes | NEEDS-FIXES | All audited NMR processors in the classification are constrained by `ShiftModule`. `RandomCoilShiftProcessor`, `AnisotropyShiftProcessor`, `CreateSpectrumProcessor`, `HaighMallionShiftProcessor`, `ShiftModel`, `ShiftModel1D`, `ShiftModel2D`, `HBondShiftProcessor`, `EmpiricalHSShiftProcessor`, `JohnsonBoveyShiftProcessor`, `EFShiftProcessor`, `ClearShiftProcessor`, and `Experiment`/`SimpleExperiment1D` all inherit `ShiftModule` and declare `operator()(Composite&)`. A clean P3 plan needs either a module-wide `ShiftModule` API migration to `UnaryProcessor<Atom>` with SIP/docs/test updates, or a bridge/helper plan that keeps `ShiftModule` stable while removing local casts. |
| P22-6 | CI grep exception scope | NEEDS-FIXES | The generic `Composite::apply<T>` bodies live in `include/BALL/CONCEPT/composite.h`, but the Atom-specialized preorder bodies live in `source/CONCEPT/composite.C` (`:1904`, `:1930`). There is no `composite.iC` body for this path. Grep exceptions must cover `include/BALL/CONCEPT/composite.h` and `source/CONCEPT/composite.C` for the central traversal implementation. |
| P22-7 | `Selector::operator()(Composite&)` public API | NEEDS-FIXES | `Selector` is exported and publicly derives `UnaryProcessor<Composite>` (`include/BALL/KERNEL/selector.h:30`) with `operator()(Composite&)` (`:91`). It is also SIP-bound and used by VIEW/APPLICATIONS. Retyping it to `UnaryProcessor<Atom>` is a public API/ABI change. For v2.1, keep the Composite-facing API and route the Atom test through the central helper, or explicitly classify it as a planned public API break. |
| P22-8 | Overall P3 plan | NEEDS-FIXES | P3 can still ship, but P3.1 overstates Pattern A. The biggest required change is to replan NMR around `ShiftModule`, correct the `Composite::apply<Atom>` central-dispatch description, and avoid public API narrowing for `Selector` and `AtomVector` unless backed by compatibility overloads/bindings. |

## New findings

1. `Composite::apply<Atom>` is already special-cased and uses existing virtual `Composite::isAtom()` (`include/BALL/CONCEPT/composite.h:771`, `source/CONCEPT/composite.C:1911`, `:1934`). This contradicts the classification's claim that the Atom dispatch is centralised `dynamic_cast`.
2. The NMR plan is not a "standalone processors" retype. `ShiftModule` is the real interface boundary; changing only derived classes will not compile.
3. `MOL2File::containsAtomChilds_` currently means direct Atom children, not descendant Atoms. `beginAtom()` is recursive over the composite subtree, so it is not a drop-in replacement without a behavior decision.
4. `AtomVector` can likely keep the public `Composite` API and remove local RTTI by using existing `AtomConstIterator` over the `Composite`; this is less disruptive than narrowing to `AtomContainer&`.

## Recommended commit plan

1. Revise `P3-CLASSIFICATION.md` before P3.2: mark NMR Pattern A as blocked by `ShiftModule`; correct `Composite::apply<Atom>` to "central `isAtom()`/static_cast specialization" rather than `dynamic_cast`.
2. P3.2: keep `AtomVector(const Composite&, bool)` and `operator=(const Composite&)`; refactor `set(const Composite&, bool)` to use `AtomConstIterator` over the composite. Add an `AtomContainer` overload only if useful, not as a replacement.
3. P3.4a: introduce the central helper in a named internal place (`BALL::detail` preferred unless `v21_internal` is adopted as a documented new convention). Use it for `Selector`, disulfide pair checks, molecule object/info paths, and MOL2 direct-child checks.
4. P3.4b: handle NMR as one coherent API commit or small stack centered on `ShiftModule`. Either migrate `ShiftModule` and all derived classes/bindings/tests to `UnaryProcessor<Atom>`, or keep `ShiftModule::operator()(Composite&)` as a compatibility bridge and move the cast/filter into the central helper. Do not retype individual derived processors first.
5. P3.6 grep gate: exclude only the central helper plus `include/BALL/CONCEPT/composite.h` and `source/CONCEPT/composite.C` traversal internals. Keep VIEW/APPLICATIONS excluded per roadmap/D37.

## Open questions from P3-CLASSIFICATION.md

1. `compositeAsAtom_` namespace: corrected. `BALL::v21_internal` is not an existing convention. Prefer `BALL::detail` or explicitly document the new `v21_internal` convention before introducing it.
2. NMR commit granularity: corrected. Do not split by individual processor under the current plan; the first NMR commit must address the shared `ShiftModule` interface.
3. CI grep scope: confirm roadmap scope. CORE_ONLY gate should exclude VIEW/APPLICATIONS; exception files should include `include/BALL/CONCEPT/composite.h` and `source/CONCEPT/composite.C`, not `composite.iC`.

## Overall verdict

**NEEDS-FIXES.** The high-level goal is still viable, but P3.2/P3.4 should not start from the current classification. The plan needs a corrected central-dispatch model, a `ShiftModule`-aware NMR strategy, API-preserving treatment for `Selector`/`AtomVector`, and a narrower MOL2 behavior decision before implementation commits land.
