# Track A v2.1 Cluster R21 — Codex CLI Round 21 (2026-05-19)

**Status:** Complete
**Verdict:** NEEDS-FIXES
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.1 roadmap P2-P6

## Probe answers

| # | Probe | Verdict | Notes |
|---|---|---|---|
| P21-1 | D39 deferral correctness | GO | Deferring mutation wiring is defensible and is the right call for v2.1. A per-Composite `is_being_destroyed_` flag or RAII guard would cover the obvious `~Composite -> parent_->removeChild(*this)` path, but not the full long tail: `destroy()`, `clear()`, `destroyChildren()`, splice/replace/adopt rollback, parent-chain store lookup during partial teardown, and non-Atom Composite ownership. A single base guard only helps if every mutator and every cascading teardown path consistently enters through guarded code. The P2.1.1 failure proves this is not a local patch; it is lifecycle surgery. Vote: keep D39. |
| P21-2 | D40 v2.1 value proposition | NEEDS-FIXES | The remaining v2.1 value is real, but the roadmap must stop selling v2.1 as thin-handle/memory-budget progress. Observable user value: faster JSON load if P4 proves it, bond-property JSON round-trip, custom Element identity preservation, fewer RTTI dependencies preparing v2.2, and a new store iteration API. Perf gates and scaffolding are mostly maintainer value. This is enough for a v2.1 minor only if release notes explicitly frame it as "JSON + API + v2.2 prep"; otherwise many items could be v2.0.x patches. |
| P21-3 | P3 RTTI count | GO | Re-grep confirmed 24 matches in the requested module set: `rg -n "dynamic_cast\\s*<\\s*(const\\s+)?Atom\\s*\\*|RTTI::isKindOf\\s*<\\s*Atom\\s*>|isKindOf\\s*<\\s*Atom\\s*>" source/KERNEL source/STRUCTURE source/FORMAT source/QSAR source/MOLMEC source/SCORING source/SOLVATION source/DOCKING source/ENERGY source/NMR source/CONCEPT \| wc -l` -> 24. Distribution: 13 NMR, 2 STRUCTURE/disulfid, and one each in selector, MOL2File, atomVector, geometricProperties, molecularInformation, moleculeObjectCreator. |
| P21-4 | P3.3 `isAtom_()` virtual | NEEDS-FIXES | Adding one virtual slot to `Composite` is cheap because it already has a vtable, but `isAtom_()` exists only to replace RTTI while Atom still inherits Composite. In v2.2, if Atom no longer derives from Composite, the override disappears and any `Composite* -> Atom*` replacement based on `static_cast<Atom*>` is invalid by design. Prefer per-site refactors to typed `Atom*` or store/back-ptr lookups. Visitor is probably overkill for all 24 sites, but P3 planning must reserve it only for genuine tree-discrimination sites and must not use `isAtom_()` as the default hammer. |
| P21-5 | P4 V21-LOAD-BATCH design | NEEDS-FIXES | The roadmap's load-speed claim is under-specified. Existing System JSON load allocates `new Atom` for every live atom, inserts molecules/atoms through Composite APIs, then patches store columns via `slot_map`. If the 15x gap comes mainly from heap Atom construction and Composite insertion/adoption, batching atom-row construction inside `MoleculeStore` will not close it. P4.1 must first profile the 100k load into buckets: JSON parse, Atom allocation, Composite insert/adopt, store column writes, property restore, bond restore. Then target the dominant bucket. |
| P21-6 | P4 V21-BOND-PROPERTY-JSON storage | GO | Clean v2.1 win. D23b leaves non-Atom PMs v0-only, but the JSON path can read Bond's v0 `PropertyManager` directly through `detail::properties_to_json(const PropertyManager&)`, the same helper already used for System/Molecule/Atom bags. No side table is required. Required design detail: serialize bond properties in the same live `bonds[]` order, because `BondRecord` identity is row-order, not `(a,b)`. |
| P21-7 | P5 STORE-ITER-API forward-compat | GO | Stable if the public API is phrased as `MoleculeStore::iterAtoms()` yielding `Atom*` or `const Atom*`. v2.2 can drop Atom's Composite inheritance without changing the existence of `Atom` or store back-pointers. Do not expose `Composite*`, `CompositeHandle`, or node-table details through this API. |
| P21-8 | P5 calibrated perf gates | NEEDS-FIXES | "Median of N=5" is a start but not a calibration method. Roadmap should specify baseline collection: at least 5 clean CI workflow runs per benchmark on GHA macos-15, each benchmark internally N=5; record median, min/max, and coefficient of variation. Gate at max(2x baseline median, baseline median + 6 sigma/robust MAD equivalent) or mark noisy benchmarks as advisory until variance is known. |
| P21-9 | P5 generation-guard `BALL_DEBUG` | NEEDS-FIXES | D25 says debug-only, but the current hot Atom getters in `include/BALL/KERNEL/atom.iC` dereference `store_` directly with no guard yet. P5 must require compile-time proof: guard code behind `#ifdef BALL_DEBUG`, release assembly or grep check showing no `generation_` compare in inline getters, and a debug test that stale deref traps. As written, "free in release" is an intent, not verified. |
| P21-10 | P6 release branch strategy | NEEDS-FIXES | Roadmap is silent and conflicts with D29's explicit "After v2.1.0 final tag, merge v2.1 -> master." P6 must state final branch action: tag from `v2.1`, keep `v2.0` maintenance separate, cherry-pick shared critical fixes as needed, then merge or fast-forward `v2.1` into `master` unless master has diverged. It should not merge v2.1 into v2.0. |
| P21-11 | D34c MSVC CI revision | NEEDS-FIXES | Since D40 removes the v2.1 inheritance flip and `sizeof(Atom) <= 32 B` target, MSVC EBO verification is no longer a v2.1 release blocker. Keeping "may slip past v2.1.0" is acceptable only if formalized: D34c gate moves to v2.2 P0/P2 where the size target matters. For v2.1, Windows CI is a non-blocking backlog item unless normal CORE_ONLY portability is claimed. |
| P21-12 | Codex review burden | NEEDS-FIXES | Eight more reviews is disproportionate after the pivot. Keep R22/R23 for P3 because RTTI replacement can change behavior. Keep R24/R25 for P4 because JSON load/schema changes are user-visible. Consolidate P5 into a maintainer checklist plus close review unless STORE-ITER-API design is nontrivial; R26 planning can be a short API-only review. Keep R28 pre-tag. |
| P21-13 | Overall executability | NEEDS-FIXES | Executable direction, but not executable as written. Required fixes: profile-driven P4 load plan, P3 replacement mechanism per site, concrete perf-gate methodology, branch strategy, D34c gate relocation, and release-value wording that admits v2.1 no longer closes D13. |

## New findings

**R21-F1 — P4.1 load-batch target is not justified by the current loader shape.**  
The roadmap says "JSON load batches atom row construction; defers Composite-tree insertion until all atoms allocated." Existing `loadSystemJSON` already separates allocation, insertion, column patching, stable-id restore, and bond restore, but it still constructs heap `Atom`s and inserts through the Composite tree before store-column replay. If Composite insertion/adoption dominates the 3.8 s load, row batching will miss the target. Fix: add a profiling sub-step before design lock and make the performance target conditional on the measured bottleneck.

**R21-F2 — P3 must not default to `Composite::isAtom_()` as the RTTI replacement.**  
That virtual is a temporary bridge that becomes wrong or useless when v2.2 removes `Atom : Composite`. The roadmap should classify the 24 sites first and prefer typed call paths or store-backed identity. Use a visitor only where a traversal truly needs runtime discrimination.

**R21-F3 — D34c is now stale after D39/D40.**  
MSVC EBO was a release gate because v2.1 previously claimed the thin-handle size target. D40 explicitly removes that target from v2.1. Move MSVC-size gating to v2.2, or document Windows CI as non-blocking v2.1 backlog.

**R21-F4 — P6 branch strategy must be written down.**  
D29 says merge `v2.1` into `master` after final tag. The roadmap says nothing. Add the post-tag branch action and explicitly say v2.1 is not merged back into the `v2.0` maintenance branch.

## Overall verdict

**NEEDS-FIXES.** D39 is the correct engineering call; trying to salvage mutation wiring with a destruction flag in v2.1 would create a lifecycle audit project with little user-visible payoff. D40 is also acceptable as a minor-release pivot if the release is framed honestly: v2.1 becomes JSON/API/perf/RTTI cleanup plus v2.2 preparation, not the D13 memory-budget release.

The roadmap needs one revision before GO. The load-batch plan must become profile-driven, P3 must avoid a throwaway `isAtom_()` default, P5 gates need a concrete calibration recipe, and P6 must resolve branch/MSVC/review-cadence drift introduced by the pivot.
