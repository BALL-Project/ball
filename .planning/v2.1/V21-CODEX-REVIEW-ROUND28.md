# Track A v2.1 Cluster R28 — Codex CLI Round 28 (2026-05-20)

**Status:** Complete
**Verdict:** NEEDS-FIXES
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.1.0-rc1 pre-tag review

## Probe answers

**P28-1. Release-notes factual accuracy.** Mostly supported, but not clean enough to tag. The JSON load performance claim is conservative and supported by local logs: v2.1 current median is ~219-232 ms no-props and ~426-445 ms +3 props versus the documented v2.0 profile ~3,588 ms / ~3,750 ms, so 7-14x is defensible, though the milestone still says "16x" in places. The save:load ~1:1 claim is supported by current medians (~311:219 and ~435:426 in `build/test`, ~275:232 and ~426:445 in `build`). The schema minor numbers are correct in code: store `1 -> 2`, system `0 -> 1`.

Two release-note facts are wrong or unsupported:

- `RELEASE-NOTES-v2.1.md` says v2.0 readers reject the higher minor and fail cleanly. That contradicts the schema policy and code: v2.0 `moleculeStoreJson.h` explicitly says readers silently accept higher minor, and the v2.0 System loader does not appear to reject `format_minor`. The honest claim is: v2.1 emits higher minors; v2.1 reads v2.0; older readers may accept the document while ignoring new fields / losing v2.1 fidelity, so v2.1 JSON should not be treated as v2.0-compatible.
- `RELEASE-NOTES-v2.1.md` and `MILESTONE-CONTEXT-v2.1.md` say "29 sites" for P3. The actual CORE_ONLY helper call count is 24, and `P3-CLASSIFICATION.md` says all 24 sites route through the helper. Use 24 unless there is a separate, documented 29-count basis.

The `284/284 PASS` claim is supported by `build/test/Testing/Temporary/LastTest.log` enumerating 284 tests through the final test. The `sizeof(Atom)` 360 -> 368 and `sizeof(Bond)` 288 -> 296 claims are supported by `Sizeof_test.C` history and the added 8-byte `composite_handle_packed_`. The 139 B SoA line is supported by `Sizeof_test.C` arithmetic. I did not find public signature removals in the spot-check; P3 explicitly preserved the NMR / Selector / AtomVector interfaces.

**P28-2. D42 vs P4.2 consistency.** Not consistent. `V21-DECISIONS.md` still has D42 as "V21-BOND-PROPERTY-JSON deferred to v2.2" and D43 as "not patched in v2.1", with no appended reversal after commits `45df3714d` and `67827ad7c`. `BACKLOG.md` still says `V21-BOND-PROPERTY-JSON` is "was v2.1 P4.2 -> now v2.2" and the "Closed" section says "(none yet)", while the code and release notes say P4.2 shipped. `V21-ROADMAP.md` also still has the P4.2 row struck through as deferred. This is a must-fix documentation contradiction.

**P28-3. Cross-reference integrity.** Referenced files exist: `P4.2-FINDINGS.md`, `P4.3-FINDINGS.md`, `BACKLOG.md`, `V21-DECISIONS.md`, and review files R17 through R27. R28 did not exist before this report, as expected. The problem is not dangling links; it is stale linked content. `P4.2-FINDINGS.md` is still "BLOCKED" and recommends Option C, while the subsequent implementation chose Option A. That is acceptable as a historical finding only if `V21-DECISIONS.md`, `BACKLOG.md`, and `V21-ROADMAP.md` clearly record the reversal. They currently do not.

**P28-4. Over-promise / honesty check.** The release notes are honest that v2.1 is not the memory release and they disclose the +8 B Atom growth in both "What's NOT" and "Known limitations". That disclosure is prominent enough. No shipped code suggests the thin-handle / D13 closure landed. The over-promise risk is instead compatibility wording: "v2.0 readers reject the higher minor" overstates safety; the code indicates older readers may accept and silently drop v2.1 semantics.

**P28-5. Shipped-code sanity.** The high-risk P4.2 code looks sane. `systemJson.C` reconstructs via `createBond(*fresh, *atom_b)`, deletes `fresh` on `TooManyBonds`, deletes unused `fresh` for duplicate/self cases, warns on duplicate-pair collapse, and leaves the first bond's order/type/properties intact. The rollback guard remains armed through bond restoration. `SystemJson_test.C` covers property round-trip, no-property multi-bond reconstruction, and duplicate store-pair collapse. `moleculeStore.h` has the `bonds_.empty()` guard in `for_each_bond_of`, and `moleculeStore.C` has matching guards in `bond_degree` / `bonds_of`.

The RTTI helper is the only CORE_ONLY production Atom RTTI hit outside the documented `source/CONCEPT/composite.C` helper body under the CI grep pattern. VIEW/APPLICATIONS still contain Atom RTTI, as documented. Note: `include/BALL/COMMON/rtti.h` contains template bodies mentioning `Atom`, but the CI gate scopes production CORE_ONLY module source paths and excludes this generic header; this matches the documented gate rather than being a new shipped defect.

**P28-6. Branch/merge readiness.** Branch is `v2.1` at `e5a12e502`, matching the supplied state. The working tree was clean before this report. The root has ignored in-source CMake artifacts (`CMakeCache.txt`, `CMakeFiles/`, `Makefile`, `Testing/`, etc.), but they are not tracked and are covered by `.gitignore`; they should not block rc1. For final merge to `master`, check that the `.planning` tree is intentionally carried forward or excluded by policy, because it is large release-process history.

**P28-7. Overall.** NEEDS-FIXES before tagging rc1. The shipped code passes the spot-check, but the release docs currently contain release-blocking contradictions and at least two inaccurate factual claims.

## Must-fix before rc1

1. Fix P4.2 documentation consistency:
   - Amend `V21-DECISIONS.md` after D42/D43 with the maintainer reversal: Option A implemented in P4.2, commits `45df3714d` + `67827ad7c`, bond properties and Bond* graph reconstruction shipped, duplicate-pair System load collapses with warning.
   - Move `V21-BOND-PROPERTY-JSON` out of the deferred backlog in `BACKLOG.md` and list it under Closed/delivered. Keep only any true v2.2 follow-up, such as store-native bond thin-handle work.
   - Update `V21-ROADMAP.md` P4.2 so it no longer says deferred to v2.2 / R16 C-B7 carries forward.

2. Fix release-note factual claims:
   - Replace "29 sites" with the verified "24 sites" wording, or add a precise explanation if a different count is intended.
   - Replace "v2.0 readers reject the higher minor / fail cleanly" with the behavior the code supports: v2.1 emits bumped minors and reads v2.0 JSON; older readers are not guaranteed to preserve v2.1-only fields and may silently ignore them.

3. Normalize the performance headline across docs:
   - `RELEASE-NOTES-v2.1.md` uses 7-14x; `MILESTONE-CONTEXT-v2.1.md`, `V21-DECISIONS.md`, and historical P4 docs still say 16x in current-scope summaries. Use one current-release phrasing, preferably "7-14x in release notes; local current logs show up to ~16x on this machine" if both are retained.

## Overall verdict

**NEEDS-FIXES.** I do not see a shipped-code blocker in the P4.2 loader, CSR guard, RTTI helper, or benchmark helper. The blocker is release integrity: the docs disagree on whether P4.2 shipped, the Atom RTTI site count is wrong, and the upgrade-note claim about v2.0 readers rejecting higher minors is not supported by the code.
