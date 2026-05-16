---
milestone: v1.6.1
milestone_name: "Strict corrective release for v1.6.0"
status: draft (post-review revision, awaiting /gsd-new-milestone)
drafted: 2026-05-15
revised: 2026-05-15 (post claude + codex synthesis review; see .planning/REVIEW-v1.6.1.md)
predecessor: v1.6 (which shipped as v1.6.0 with broken Windows installer + empty macOS Info.plist version strings + release.yml still pinned to Qt 5)
---

# Milestone v1.6.1 — Context (Pre-Workflow Draft, Revised Post-Review)

> This file is a lightweight scope doc for the v1.6.1 milestone, intended to be
> picked up by `/gsd-new-milestone v1.6.1` (which scans for
> `.planning/MILESTONE-CONTEXT.md`). It captures the scope, in-flight work, and
> release criteria so the formal workflow can short-circuit its questioning step.
>
> **Revised 2026-05-15** after a synthesis review (claude + codex 0.128.0
> adversarial) flagged 4 critical findings, 6 significant concerns, and 5
> notable risks. Full review at [`REVIEW-v1.6.1.md`](REVIEW-v1.6.1.md). Key
> change: Phase 5.2 PR triage moved OUT of v1.6.1 (deferred to v1.6.2) per
> codex's "scope drift" finding. Release.yml Qt 5 → Qt 6 added as critical
> pre-tag fix. B3 measurement made explicit.

## Goal

**Strict corrective release: ship v1.6.0's intended payload, nothing more.**
v1.6.0 tagged on 2026-05-15 (commit `afa00c2`, on a Qt 5 codebase) but the
Windows installer was skipped (LNK1104 `tbb12_debug.lib`), the macOS bundle's
Info.plist had empty version strings, tri-OS CI was never green on the release
branch, and `release.yml` was never updated to follow Phase 5's Qt 6 migration
(it still installs `qt@5` and calls `qt@5/bin/macdeployqt` — Qt 5 paths).
v1.6.1 is the corrective re-release: same modernization payload, plus the
release pipeline fixed to match the Qt 6 codebase, plus the small color-defaults
user-facing bug fixed.

Phase 5.1 already landed the source-level fixes; v1.6.1 is verification +
release.yml repair + re-tag + Phase 4.1 (color defaults).

### Scope discipline (post-review)

Codex's review flagged that the original draft drifted into a minor-release
shape ("PR triage" + "optional Phase 8 slice" + "maybe Phase 9 folds in" all
added to a "patch release"). Revised scope is **strictly corrective**:

- **In scope:** Phase 5.1 verification, release.yml Qt 6 repair, Phase 4.1 if
  landed by tag-date, tag + artifacts + release notes, B3 baseline measurement.
- **Out of scope (deferred to v1.6.2):** Upstream PR triage (Phase 999.5),
  Phase 8 notarization slice, Phase 9 test work, Tier-C warning cleanup,
  artifact-action major-version bumps.

## Goal one-liner

> *"v1.6.1: deliver what v1.6.0 promised — clean tri-OS installers, populated
> macOS bundle metadata, and the persisted-config color-shadow bug fixed."*

## In scope

### 1. Phase 5.1 carry-forward (LANDED)

Source-level work complete in 14 plans on `v1.6-modernization`:

- **A1–A6** Tier-A correctness fixes (getline recursion, pointer truncation,
  self-assign, tautological compare, format-overflow, stringop-truncation)
- **B1+B2** Tier-B Windows DLL hygiene (`extern template class` / `BALL_EXPORT`
  migration in `vector3.{h,C}` + `atom.{h,C}`)
- **B3** project-wide `#pragma warning(disable: 4251)` in `global.h`
- **B4+B5** Tier-B cosmetic Windows fixes
- **D1+D6** Qt6LinguistTools `QUIET` + vcpkg `qttools`
- **D2** GitHub Actions Node 24 action pins (`@v5`)
- **D3** Linux apt-archives cache narrowed (was misclassified in BACKLOG as ccache)
- **D4** Windows release `--config Release` (Windows installer unblock)
- **D5** macOS `CFBundleIdentifier` (`de.uni-tuebingen.ball.ballview`) +
  `CFBundleVersion` (via `${CMAKE_PROJECT_VERSION}` fix in commit `61aff67`)

Plus two in-cycle gap fixes:
- vcpkg `builtin-baseline` regression repair (commit `08ec9c5`)
- Linux `aqtinstall` Qt 6.5+ module-list fix (commit `54da903`)

**Verification gate:** tri-OS CI run on commit `54da903` (or successor) must be
green, with the warning-census drops visible per UAT tests 3–11 in
[`05.1-UAT.md`](.planning/phases/05.1-build-warnings-and-latent-bugs/05.1-UAT.md).

### 2. **`release.yml` Qt 5 → Qt 6 repair (CRITICAL — pre-tag fix)**

**Surfaced by codex review (REVIEW-v1.6.1.md CRITICAL #1).** `release.yml` was
authored in commit `a186fb5` (BEFORE Phase 5 / Qt 6 migration). It still
installs `qt@5` via Homebrew and calls `/opt/homebrew/opt/qt@5/bin/macdeployqt`.
v1.6.0 worked because it tagged on a Qt 5 codebase (verified — `git show v1.6.0:CMakeLists.txt`
shows `FIND_PACKAGE(Qt5 ...)`). The current `v1.6-modernization` HEAD requires
Qt 6 (`FIND_PACKAGE(Qt6 ...)` at [CMakeLists.txt:319](CMakeLists.txt#L319)).
**Without this fix, any v1.6.1 release attempt will fail at CMake configure.**

Required changes in [`.github/workflows/release.yml`](.github/workflows/release.yml):
- Line 69: `brew install qt@5 ...` → `brew install qt ...` (Qt 6)
- Line 134: `/opt/homebrew/opt/qt@5/bin/macdeployqt` → `/opt/homebrew/opt/qt/bin/macdeployqt`
- Lines 245, 268, 269: update stale "Qt5" comments to "Qt6"

Windows side already uses vcpkg's Qt 6 `qtbase` port (verified — `vcpkg.json:6`,
release.yml uses `windeployqt` from `vcpkg_installed`). No Windows-side Qt fix
needed.

**Verification gate:** match ci.yml's Qt 6 setup pattern from [ci.yml:111-116](.github/workflows/ci.yml#L111).
After fix, run `gh workflow run release.yml -f tag=<test-tag>` against a
throwaway test tag (or rely on tri-OS CI green as proof — CI exercises the same
Homebrew install path).

### 3. Phase 4.1 — Config Color-Defaults Fix (IN PARALLEL JOB — user-driven)

Persisted `~/.BALLView` config was silently shadowing compiled element / residue
color defaults. Real user-facing bug. Promoted from backlog 999.4. User is
running this in a separate job in parallel.

**Verification gate:** Phase 4.1's own SUMMARY + UAT. Once it lands on
`v1.6-modernization` and merges with Phase 5.1, the v1.6.1 milestone has the
patch payload it needs.

### 4. B3 C4251 baseline measurement (must run BEFORE any further source merges)

**Surfaced by codex review (REVIEW-v1.6.1.md SIGNIFICANT #8).** Phase 5.1
Plan 08 applied the project-wide `#pragma warning(disable: 4251)` in `global.h`
without measurement (CI cancellations prevented the post-fix count from being
recorded). UAT expects retroactive validation on the next clean tri-OS CI run.

**If any source-touching work lands before this measurement** (Phase 4.1 merge,
PR triage cherry-picks, etc.), the post-B1+B2 baseline is no longer cleanly
reachable — we'll never know whether the pragma was needed or already redundant.

**Required action:** on the first clean tri-OS CI run after commit `54da903`
(or its successor), record the C4251 count from the Windows MSVC log to
[`05.1-08-SUMMARY.md`](.planning/phases/05.1-build-warnings-and-latent-bugs/05.1-08-SUMMARY.md).
One-line update; ~5 minutes of work. Block all further v1.6.1 source merges
until this measurement is recorded.

### 5. Re-tag and re-release (PENDING — gated on 1 + 2 + 3 + 4)

- Tag `v1.6.1` on the green HEAD of `v1.6-modernization` after CI verification
- `gh workflow run release.yml -f tag=v1.6.1`
- Confirm both installers ship:
  - `BALLView-v1.6.1-macos-arm64.zip` (with populated Info.plist + ad-hoc resign
    from D4-mac already in place)
  - `BALLView-v1.6.1-windows-x64.zip` (the missing v1.6.0 installer)
- **Edit v1.6.0's release notes** to point at v1.6.1 (it was an incomplete
  release; users should grab v1.6.1 instead). Do NOT delete the v1.6.0 tag
  (release tag immutability + downstream caching).
- Verify the new GitHub Release is **published**, not draft
- Verify the macOS app passes `codesign --verify --deep --strict` AFTER zipping
  (D4-mac's ad-hoc resign + stapler order matters)

### Deferred to v1.6.2 (per codex review — scope-discipline cuts)

The following items appeared in the original v1.6.1 draft but were moved out
post-review (REVIEW-v1.6.1.md CRITICAL #3 — "scope drift" finding). They land
in v1.6.2 instead:

- **Upstream PR triage (5 PRs: #640, #600, #554, #550, #546).** Goes into
  Phase 999.5 ("Open-PR triage") which already exists as a backlog phase.
  Reason: codex's archaeology of `source/KERNEL/residue.C` revealed that PRs
  #554 (omega torsion) and #546 (insertion-code printing) are partially-implemented
  in the current code — cherry-picking blindly creates duplicate API or
  overwrites modernized logic. Pre-cherry-pick archaeology per-PR is required;
  not patch-release work.
- **Optional Phase 8 macOS notarization slice.** Requires Developer ID Application
  certificate; if not in hand, full Phase 8 is the natural slot (v1.6.2 or v1.7).
- **Phase 9 test-suite work** (user's commits `b2bb718` + `61bf5a7`). Already
  landed on the branch but treat as v1.6.2 deliverable for accounting clarity.
- **Action artifact pin bumps** (`upload-artifact@v4` → v6, `download-artifact@v4` → v7).
  Cross breaking-change majors; not patch-release work.
- **Tier-C warning bulk cleanup** (~3700 warnings). Explicitly out of Phase 5.1
  scope per CONTEXT.md D-04.

### Promoted from backlog: Phase 999.7 Linux part (post-review)

**Surfaced by claude review (REVIEW-v1.6.1.md SIGNIFICANT #6).** The Linux
aqtinstall fix in commit `54da903` is partial work for [Phase 999.7](.planning/ROADMAP.md#L348)
("Qt 6 Linux + Windows Bring-Up and CI Fixup"). Phase 999.7's dormancy
condition ("Phase 5.1 closes AND a contributor has Windows access") is partly
satisfied — Phase 5.1 is done. Promote the **Linux part only** to active
status; Windows part stays dormant until a contributor has Windows access.

Commit `54da903` becomes the first piece of Phase 999.7 work; record it as
such in 999.7's progress notes.

### Promoted from backlog: Phase 999.2 Ninja build-generator switch — COMPLETE (2026-05-16)

Was [`BACKLOG · TARGETED FOR v2.0`](.planning/ROADMAP.md#phase-9992-ninja-build-generator-switch-complete--v161--2026-05-16)
in the v2.0 substrate-modernization bundle. Promoted into v1.6.1 active on
2026-05-16, landed and verified the same day. **Why now, not v2.0:** With
Phase 4's Windows `blocking: true` flip, every CI cycle was eating ~80 min
of Windows wall-clock under MSBuild (baseline: run 25899905204 Windows
Build = 4625s, CMAKE_<LANG>_COMPILER_LAUNCHER=ccache silently ignored by
MSBuild). Two prep commits (`d5f5566` Windows `--parallel`, `9c932eb`
choco install ccache) had already landed; the third move — flip the
generator to Ninja + provision ninja on all three runners — paid back its
own implementation cost on the first warm-cache re-run. Pure build-tooling
change; risk surface was CI YAML + `CMakePresets.json` only, NOT BALL/VIEW
source — cleared the v1.6.1 scope-discipline bar.

**Result (CI run [25953405453](https://github.com/BALL-Project/ball/actions/runs/25953405453)):**

| Metric | Before (MSBuild baseline) | After cold-cache (Ninja, run 1) | After warm-cache (Ninja, run 2) |
|---|---:|---:|---:|
| Windows Build step | 4625s | 4818s (populating cache) | **55s** (87× speedup) |
| Windows total job | ~80 min | ~85 min | **~4.7 min** |
| ccache hit rate | 0% (ignored) | 0% (cold) | **~98.9%** (wall-clock-derived) |
| macOS Build | 67s | 31s | 39s (runner noise) |
| Linux Build | 99s | 25s | 34s (runner noise) |

Windows total job ~4.7 min is well under the 10-min standing
conditional-disable threshold — the `continue-on-error: false` Windows
blocking flip from Phase 04-04 (Plan 04-04 Task 3) holds with
genuine per-iteration headroom.

Implementation:
[PLAN.md](.planning/phases/999.2-ninja-generator-switch/PLAN.md),
[999.2-SUMMARY.md](.planning/phases/999.2-ninja-generator-switch/999.2-SUMMARY.md).
Commits: `68f8f86` (planning), `1141881` (CMakePresets Ninja),
`0bd1c16` (CI ninja installs + msvc-dev-cmd), `bbac526` (ccache stats
instrumentation), `88ce213` (phase close).

## Out of scope (defer to v1.7 / v1.6.2 / later)

| Item | Why deferred | Target |
|------|--------------|--------|
| **Upstream PR triage** (#640, #600, #554, #550, #546) | Per codex review: PRs #554 / #546 partially-implemented in current Residue; per-PR archaeology required before cherry-pick. Phase 999.5 slot is the proper home. | v1.6.2 |
| **Phase 8 macOS notarization slice** | Requires Developer ID cert; full Phase 8 is the natural home | v1.6.2 or v1.7 |
| **Phase 6** (Python Bindings) | Vertical-slice decision spike (SIP 6 vs pybind11 vs nanobind) is its own milestone-shape | v1.7-track |
| **Phase 8 full** (BUILD-linux.md, BUILD-windows.md, license review, signing wiring) | Bigger scope than a patch | v1.6.2 or v1.7 |
| **Phase 9** (Test Suite Triage) | Already in flight via commits `b2bb718` + `61bf5a7`; treat as v1.6.2 deliverable for accounting clarity | v1.6.2 |
| **Action artifact pins** (`upload-artifact@v4` → v6, `download-artifact@v4` → v7) | Deferred from Phase 5.1 Plan 11; cross breaking-change majors | v1.6.2 |
| **Tier-C warning bulk cleanup** (~3700 warnings) | Explicitly out of Phase 5.1 scope per CONTEXT.md D-04 | v1.6.2 or v1.7 |
| **BALLView UI refresh** (SEED-001) | The v1.7 milestone proper | v1.7 |

## Release criteria (revised post-review)

A v1.6.1 release is shippable when **all eight** are true:

1. **`release.yml` Qt 6 repair landed** — `brew install qt` and `qt/bin/macdeployqt`
   in [release.yml:69, 134](.github/workflows/release.yml#L69) (CRITICAL pre-tag)
2. **Tri-OS CI green** on the target HEAD — macOS arm64, Linux x64, Windows x64
   all reach `[100%] Built target BALL` + their respective smoke checks
3. **Phase 5.1 UAT tests 3–11 PASS** — warning census on each toolchain shows
   the targeted categories at zero on the matched files
4. **B3 C4251 baseline recorded** in [05.1-08-SUMMARY.md](.planning/phases/05.1-build-warnings-and-latent-bugs/05.1-08-SUMMARY.md)
   — post-pragma Windows MSVC count from the first clean tri-OS CI run
5. **Phase 4.1 merged + UAT-confirmed** — color defaults bug user-verified fixed
6. **Both installers attached to the GitHub Release** — Windows zip + macOS arm64
   zip — and the GitHub Release is **published**, not draft
7. **Artifacts built from the exact `v1.6.1` tag** — not a moved HEAD; cross-check
   `Tagger SHA == release artifact build commit`
8. **macOS app passes `codesign --verify --deep --strict`** AFTER zipping (D4-mac
   stapler order); v1.6.0's release notes edited to point at v1.6.1

## Carry-over from v1.6 STATE

The v1.6 milestone's accumulated context (decisions, blockers, todos) survives
the milestone switch — symmetric with `gsd-sdk query state.milestone-switch`.
v1.6.1 builds on:

- Phase 1–5 modernization decisions (all locked)
- Phase 02.1 renderer boundary (foundation for Phase 5 backend swap)
- Phase 02.2 CI matrix (the verification oracle)
- Phase 5 Qt 6 + SPIKE-02 decision (GL-Core for v1.6.x → QRhi for v2)
- Phase 5.1 LEARNINGS (BACKLOG misclassification patterns, Codex cross-check
  value, vcpkg baseline trap, nested-PROJECT shadow)

## Roadmap delta from v1.6

If `/gsd-new-milestone v1.6.1` is run, the resulting ROADMAP.md changes are:

- **Insert Phase 5.2 — Upstream PR Triage and Integration.** Sequenced after
  Phase 5.1; depends on no prior phase technically (the PRs target `master`),
  but rebases against `v1.6-modernization` which assumes Phase 1–5 is in.
  Suggested plan breakdown:
  - Plan A — #640 FindXDR investigate-then-close-or-rebase
  - Plan B — #600 .travis.yml close + cherry-pick of orthogonal changes
  - Plan C — #554 + #546 C++ slice extraction (bundled — both touch `residue.C`,
    both have `.sip` parts that defer)
  - Plan D — #550 HYP residue rebase + merge (touches `residue.C` too;
    sequence after Plan C OR merge with Plan C if conflict surface is small)
- **Optional: insert "Phase 8.0 — macOS notarization slice"** if Developer ID
  is available. Otherwise Phase 8 stays whole and defers.
- **Phase 4.1** stays in ROADMAP.md as already-open.
- **STATE.md frontmatter** flips `milestone: v1.6` → `milestone: v1.6.1`;
  status resets to `planning`.

## Open questions

1. **Developer ID Application certificate available?** Drives whether the
   optional Phase 8 slice lands in v1.6.1 or defers.
2. **Phase 9 timing?** If the in-flight test-suite work (`b2bb718`, `61bf5a7`)
   reaches a stable point before v1.6.1 tags, it can fold into the patch
   release as a bonus.
3. **XDR still a BALL dependency?** Drives #640's disposition: if XDR is gone
   (replaced or no longer used by any active code path), the PR closes as
   obsolete; if it's still in use, the PR rebases. Quick check during Plan A:
   `grep -rn "XDR\|FindXDR" CMakeLists.txt include/ source/ | grep -v ball_contrib`.
4. **Maintainer policy on auto-closing PRs from departed contributors?** The
   2015-era PRs from `smoe` (#554, #550, #546) are technical wins, but the
   author may not be available to rebase. Phase 5.2 should default to
   maintainer-driven cherry-picks rather than waiting for the author. Confirm
   this is OK for the institutional record.
5. **v1.6.0 retraction strategy?** v1.6.0 is already tagged + a partial release
   exists (macOS only). Options: (a) leave v1.6.0 as-is and just ship v1.6.1
   normally, (b) edit the v1.6.0 release notes to point at v1.6.1, (c) delete
   the v1.6.0 release. (a) is simplest; recommend (a) + (b) for clarity.

## Next action

Run `/gsd-new-milestone v1.6.1` once tri-OS CI green is confirmed on commit
`54da903`. The workflow will pick this file up and short-circuit its
requirements-gathering step.
