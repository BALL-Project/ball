---
review_target: v1.6.1 milestone draft + ROADMAP.md
reviewers:
  - claude (orchestrator self-review)
  - codex 0.128.0 (adversarial review, /tmp/codex-v1.6.1-review-output.md)
date: 2026-05-15
status: pre-execution
---

# v1.6.1 Plan Review — Synthesis

Two adversarial reviews run in parallel. Codex caught two showstoppers I missed; I caught one issue Codex missed (Phase 999.5/999.7 already exist for this work). Combined findings below, classified by criticality.

## CRITICAL (must address before tagging)

### 1. `release.yml` still uses Qt 5 — will fail on v1.6.1 [codex]

[`.github/workflows/release.yml:64-79`](.github/workflows/release.yml#L64) installs `qt@5` via Homebrew, and [line 134](.github/workflows/release.yml#L134) calls `/opt/homebrew/opt/qt@5/bin/macdeployqt`. But the codebase now requires Qt 6 ([CMakeLists.txt:319](CMakeLists.txt#L319): `FIND_PACKAGE(Qt6 ${QT_MIN_VERSION} REQUIRED Core Network Xml)`). The ci-macos preset's `CMAKE_PREFIX_PATH` looks at `/opt/homebrew/opt/qt` (Qt 6) which is NOT installed by release.yml.

**v1.6.0 either tagged on a still-Qt-5 commit, or by coincidence found Qt 6 from a pre-installed Homebrew runner image.** Either way, the v1.6.1 release will fail to configure unless this is fixed.

**Fix:** in release.yml, swap `brew install qt@5` → `brew install qt` and `qt@5/bin/macdeployqt` → `qt/bin/macdeployqt`. Mirror the Qt 6 install pattern from [ci.yml:111-116](.github/workflows/ci.yml#L111).

### 2. PRs #554 and #546 are partially-implemented in the current codebase [codex]

Current `Residue` already has `hasTorsionOmega()` / `getTorsionOmega()` ([source/KERNEL/residue.C:234-291](source/KERNEL/residue.C#L234)) and insertion-code storage/accessors. Cherry-picking the 2015 PRs as written either creates duplicate API or overwrites modernized logic. My PR triage plan said "rebase + cherry-pick" without recognizing this.

**Fix:** before any cherry-pick, per-PR archaeology pass: "already present" / "partial overlap" / "still missing" / "test added." Codex's recommendation. Each rebase plan must produce this archaeology note as its first deliverable.

### 3. v1.6.1 scope is dishonest about being a "patch release" [codex + claude]

Stated goal: "mostly verification + re-tag + a small user-facing bug fix." Actual scope: Phase 5.1 verification (11 UAT tests pending) + Phase 4.1 + new Phase 5.2 with 4 plans of PR archaeology + optional Phase 8 slice + maybe Phase 9 work folds in. That's not a patch release — that's a minor release branded as a patch.

**Fix:** strip v1.6.1 to the strict corrective release per codex's recommendation:
- Phase 5.1 verification (close out the 9 pending UAT tests)
- Phase 4.1 only if it lands by the tag-date
- release.yml Qt 6 repair (the find above)
- Tag + artifacts + release notes
- **Move Phase 5.2 PR triage → v1.6.2** (use existing Phase 999.5 slot)
- **Move Phase 8 macOS notarization → v1.6.2** unless Developer ID is in hand by tag-date

### 4. Verification basis on commit `54da903` is unproven [codex]

Phase 5.1 UAT shows 3/12 passing, 9 pending. Milestone draft requires tri-OS green on `54da903` as a release criterion, but that commit is described as "just fixed" (Linux aqtinstall fix I pushed an hour ago), not proven. If the fix doesn't actually unblock Linux, the v1.6.1 release criteria can't be met.

**Fix:** before v1.6.1 enters the planning loop, wait for CI run on `54da903` (or successor) to complete. If green, the criterion is met; if not, fix-and-re-push.

## SIGNIFICANT (should address)

### 5. Phase 5.2 sequencing instructions contradict each other [codex]

`MILESTONE-CONTEXT.md` caveats say "land #550 HYP first, then rebase #554 C++ slice on top." Roadmap-delta section says "Plan C (#554 + #546) before Plan D (#550)." Inconsistent.

**Fix:** moot if Phase 5.2 moves to v1.6.2 (CRITICAL #3 above). If kept, pick one and update both sections.

### 6. Phase 999.5 (PR triage) already exists in the backlog; Phase 999.7 (Linux + Windows Qt 6 bring-up) covers the aqtinstall fix [claude]

My MILESTONE-CONTEXT.md drafted "Phase 5.2" as a new phase for PR triage, but [Phase 999.5](.planning/ROADMAP.md#L315) ("Open-PR triage") and [Phase 999.7](.planning/ROADMAP.md#L348) ("Qt 6 Linux + Windows Bring-Up and CI Fixup") already exist for exactly this work. Phase 999.7 explicitly says it's DORMANT "until Phase 5.1 closes AND a contributor has Windows access" — Phase 5.1 just closed.

**Fix:** if PR triage stays in v1.6.1, promote Phase 999.5 instead of creating a new Phase 5.2. If Phase 5.2 moves to v1.6.2 (per CRITICAL #3), promote 999.5 to active for v1.6.2. The Linux aqtinstall fix I pushed (commit `54da903`) is partial 999.7 work — promote 999.7 to active (Linux part only — Windows part defers until a contributor has Windows access).

### 7. Warning-regression criterion too narrow for PR payload [codex]

Release criterion #6 names C4717/C4311/C4910/C4834 specifically. Phase 5.2 touches `residue.C`, rotamer logic, peptide-builder data, PDB naming. Could regress in a non-named warning category or a behavioral category (HYP recognition, omega rotamer, `getFullName()` formatting) with zero named-warning hits.

**Fix:** add per-PR-area scripted behavior checks (HYP recognition test, omega rotamer test, insertion-code `getFullName()` regex test) as part of the Phase 5.2 plan's acceptance criteria, if Phase 5.2 stays in v1.6.1.

### 8. B3 C4251 baseline can be clobbered before measurement [codex + claude]

Phase 5.1 Plan 08 applied the global C4251 pragma without measurement. UAT expects retroactive validation on "next clean tri-OS CI run." If Phase 5.2 or Phase 4.1 lands before that measurement happens, the pre-pragma baseline is no longer cleanly reachable.

**Fix:** **measure B3 first** on the next clean CI run after `54da903`. Make it an explicit pre-Phase-5.2 step. Record the post-fix C4251 count in [05.1-08-SUMMARY.md](.planning/phases/05.1-build-warnings-and-latent-bugs/05.1-08-SUMMARY.md) before any further source merges.

### 9. Release-time hazards under-specified [codex]

Release criteria require both installers attached, but not:
- That the GitHub Release is **published** (not draft)
- That artifacts were built from the exact tag (not from a moved HEAD)
- That the macOS app passes `codesign --verify` AFTER zipping (D4-mac stapler order matters)
- That release notes supersede the v1.6.0 incomplete release

**Fix:** expand release criteria to cover these. Also: edit v1.6.0's release notes to point at v1.6.1 ("v1.6.0 had broken Windows installer + empty macOS metadata; v1.6.1 is the corrected release — please use v1.6.1").

### 10. Default-decision policy is scaling into different risk classes [codex + claude]

Phase 5.1 used default-decision policy for B3 pragma (cosmetic), D3 4-option mismatch (CI-only), D5 ID-string choice (cosmetic-with-Phase-8-implication), D4 no-op verification. All defensible individually. Phase 5.2 proposes maintainer-driven cherry-picks of 2015 chemistry PRs — that's correctness-sensitive territory, NOT cosmetic.

**Fix:** new explicit policy for v1.6.2: chemistry / correctness-sensitive PR cherry-picks REQUIRE either author engagement OR per-PR behavior test before merge. Not a default-decision call.

## NOTABLE (worth recording, non-blocking)

### 11. Roadmap progress table is stale [codex]

[`.planning/ROADMAP.md:258-260`](.planning/ROADMAP.md#L258) progress table says Phase 5 = 5/8 and Phase 5.1 = 13/14, but the phase headers say both are complete. `/gsd-new-milestone` would consume inconsistent state.

**Fix:** sync the progress table to reflect 8/8 and 14/14 (and Phase 5.1 → Complete). One-line edit.

### 12. CI concurrency cancellations will recur on v1.6.1 work [codex]

Phase 5.1 LEARNINGS records every push cancelling the prior CI run within ~5 min. v1.6.1 work will repeat this unless pushes are batched.

**Fix:** for v1.6.1, batch commits per plan and only push at plan-complete boundaries. Or accept that intra-wave CI cancellations are normal and only the post-wave run matters.

### 13. PRs are mergeable=UNKNOWN on the GitHub API [claude]

All 5 PRs returned `mergeable: UNKNOWN` from `gh pr view`. GitHub hasn't recomputed merge state recently. May indicate stale state or active conflict. Pre-flight compute step needed before any cherry-pick.

**Fix:** moot if Phase 5.2 defers to v1.6.2. If kept, add as Plan A pre-step.

### 14. Core value of project is build-and-render, not PR cleanup [codex]

[PROJECT.md:11-15](.planning/PROJECT.md#L11) is explicit that the modernization milestone is "build and visibly render molecules on macOS, Linux, and Windows." PR cleanup is non-core. Including it in the v1.6.1 critical path is scope drift from the project's stated core value.

**Fix:** treat PR cleanup as v1.6.2 work, not v1.6.1.

### 15. CMakeLists.txt nesting bug pattern may repeat [claude]

UAT discovered the `${PROJECT_VERSION}` shadowing bug in `source/APPLICATIONS/BALLVIEW/CMakeLists.txt`. Three other apps also call nested `PROJECT()` without VERSION ([source/APPLICATIONS/TOOLS](source/APPLICATIONS/TOOLS/CMakeLists.txt), `MMFF94`, `UTILITIES`). Phase 8 work that adds bundle identity to those apps would re-encounter the same bug.

**Fix:** add a one-line check to Phase 8 plans that future bundle-property uses `${CMAKE_PROJECT_VERSION}` not `${PROJECT_VERSION}`.

## What changes from this review

Concrete actions for the v1.6.1 plan (will update MILESTONE-CONTEXT.md after this review):

1. **Move Phase 5.2 PR triage out of v1.6.1.** Use existing Phase 999.5 slot. Defer to v1.6.2.
2. **Add a CRITICAL pre-tag fix to v1.6.1: release.yml Qt 6 repair.** This is mandatory; without it the release will fail.
3. **Add a CRITICAL pre-Phase-5.2 step: measure B3 C4251 baseline** on the next clean tri-OS CI run. Record in 05.1-08-SUMMARY.md.
4. **Sync the ROADMAP.md progress table** to reflect 8/8 and 14/14 for Phase 5 / 5.1.
5. **Promote Phase 999.7 (Linux part only) to active** — the Linux aqtinstall fix in commit `54da903` is 999.7 work, and explicitly labelling it as such gives the change institutional provenance.
6. **Expand release criteria** to cover: published-not-draft, exact-tag-build, codesign --verify after zip, supersede v1.6.0 release notes.
7. **Resolve the Phase 5.2 sequencing contradiction** in MILESTONE-CONTEXT.md (moot if it moves to v1.6.2 — just delete the section).

## Confidence after review

The plan was directionally right (corrective release for v1.6.0) but the scope had drifted into a minor-release shape. Codex's `release.yml` Qt 5 finding was the biggest single risk — without that fix, v1.6.1 would fail to ship. The PR triage idea was sound but premature for v1.6.1 given the partial-overlap with current code.

Updated plan should be solid once the actions above are applied.
