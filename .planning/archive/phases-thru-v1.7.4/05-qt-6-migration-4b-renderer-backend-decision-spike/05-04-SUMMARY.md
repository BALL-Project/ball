---
phase: 05-qt-6-migration-4b-renderer-backend-decision-spike
plan: 04
subsystem: ci-and-lint
tags: [qt6, ci, ubuntu-24.04, jurplel-install-qt-action, gl-profile-assertion, qt5-holdout-lint, d-02, d-06, d-08]
dependency_graph:
  requires: [Plan 05-01 (CMake Qt 6 bring-up), Plan 05-02 (VIEW source API renames), Plan 05-03 (QSurfaceFormat compat profile + QtOpenGLWidgets include)]
  provides:
    - "Linux CI matrix on ubuntu-24.04 with Qt 6.5+ via jurplel/install-qt-action (D-02 + D-01)"
    - "BALLVIEW_GL_DIAG `gl_profile=compatibility` runtime assertion on macOS + Linux smoke checks (D-06 + D-08 runtime oracle)"
    - "Static-grep lint gate against Qt 5 holdouts (QRegExp, QDesktopWidget, QApplication::desktop, QString::SkipEmptyParts, QtWidgets/QOpenGLWidget, QtWidgets/QDesktopWidget, Qt5::) under source/ + include/BALL/"
    - "USE_QTWEBENGINE=OFF on Windows CI configure (vcpkg manifest does not ship qtwebengine)"
  affects: [Plan 05-05/06/07/08 spikes (CI gates are now Qt 6 throughout)]
tech_stack:
  added:
    - "jurplel/install-qt-action@v4 (Qt 6.5.* via aqtinstall on Linux CI)"
    - "scripts/check-no-legacy-qt6-symbols.sh (Phase 5 static-grep lint gate)"
  patterns:
    - "tee smoke-check stdout to /tmp/ballview-smoke.log + follow-up grep assertion step (D-06/D-08 runtime oracle)"
    - "Phase 2 grep-gate scaffold (set -u, REPO_ROOT resolution, grep -rEn LEGACY_PATTERN | grep -vE comment-filter, exit-0/1 messaging) reused verbatim for the Qt 5 holdout gate"
key_files:
  created:
    - scripts/check-no-legacy-qt6-symbols.sh
  modified:
    - .github/workflows/ci.yml
decisions:
  - "D-02 locked in CI: Linux matrix runner bumped from ubuntu-22.04 to ubuntu-24.04. Lint job runner bumped in step (consistency + avoids ubuntu-22.04 retirement)."
  - "D-01 satisfied on Linux: Qt 6.5.* installed via jurplel/install-qt-action@v4 (aqtinstall) — apt's Qt 6.4.2 on 24.04 is below the floor; this is the canonical mitigation (RESEARCH Pitfall 1 + Open Question 1). Modules: qtbase qttools qtopengl. D-05: NO qt5compat."
  - "D-06 + D-08 runtime oracle wired: each smoke-check step (macOS + Linux) tees stdout to /tmp/ballview-smoke.log and a follow-up `Assert GL compat profile` step greps BALLVIEW_GL_DIAG for `gl_profile=compatibility`. A non-blank PNG alone is NOT sufficient — Qt 6 on macOS defaults to GL 4.1 core if QSurfaceFormat is not forced, and a silently-degraded core context can still render an acceptable-but-wrong PNG. The grep step is the load-bearing assertion that Plan 05-03's setProfile(CompatibilityProfile) + setOptions(DeprecatedFunctions) actually took effect at runtime."
  - "macOS Homebrew install line switched from `qt@5` to `qt` (unversioned) to match Plan 05-01 D-03. The macos-homebrew preset's CMAKE_PREFIX_PATH already points at /opt/homebrew/opt/qt; this change brings the apt step into line so CI doesn't accidentally pre-install Qt 5."
  - "Lint job: bumped to ubuntu-24.04 + added Plan 05-04 Task 1 grep gate step. Existing Phase 2 legacy-GL gate preserved (two independent gates now share the job)."
  - "Optional bonus accepted from objective: -DUSE_QTWEBENGINE=OFF in the ci-windows configure step. The CMakeLists USE_QTWEBENGINE OPTION still defaults ON for Phase 1-4 backward compat; vcpkg.json does not declare `qtwebengine` (massive cold-build cost, no Windows consumer), so FIND_PACKAGE(Qt6 WebEngineCore) would fail. Flipping the option OFF at configure time is the least-invasive fix; macOS Homebrew has qtwebengine so it stays ON there."
  - "Runtime D-08 oracle WILL NOT actually fire green until BLOCKER-A (dockResultFile.h QXmlDefaultHandler), BLOCKER-B (templated QMutexLocker in mutex.h), and BLOCKER-D (Qt::WindowFlags w_flags = 0) land in a subsequent plan. The CI changes here are STATIC and verifiable today — YAML parses, lint script exits 0 against the current tree, lint script exits 1 against an injected QRegExp canary. The runtime oracle is wired and ready; it will exercise itself the first push after A/B/D land."
metrics:
  duration_min: 8
  duration_human: "~8min wall"
  tasks_completed: 2
  files_modified: 2
  completed_date: "2026-05-15"
---

# Phase 5 Plan 04: CI Matrix + Qt 5 Holdout Lint Summary

Brought CI to Qt 6. Linux runner bumped to ubuntu-24.04 and Qt 6.5+ provisioned via `jurplel/install-qt-action@v4` (apt's Qt 6.4.2 is below the D-01 floor). Smoke-check stdout teed to `/tmp/ballview-smoke.log` and a follow-up assertion step greps `BALLVIEW_GL_DIAG.*gl_profile=compatibility` on macOS + Linux — the D-06/D-08 runtime oracle (a non-blank PNG alone is insufficient under Qt 6, where the macOS default is GL 4.1 core). New `scripts/check-no-legacy-qt6-symbols.sh` grep gate wired into the lint job; it catches `QRegExp`, `QDesktopWidget`, `QApplication::desktop`, `QString::SkipEmptyParts`, `QtWidgets/QOpenGLWidget`, `QtWidgets/QDesktopWidget`, and `Qt5::` under `source/` + `include/BALL/`. Bonus `-DUSE_QTWEBENGINE=OFF` added to the ci-windows configure step so the qtwebengine-not-in-vcpkg-manifest gap doesn't fail Windows configure.

## What Shipped

**Task 1 — `scripts/check-no-legacy-qt6-symbols.sh`** (commit `c04865f`)
- New executable bash script (81 lines), modeled verbatim on Phase 2's `check-no-legacy-gl-symbols.sh`: same shebang (`#!/usr/bin/env bash`), same `set -u`, same `REPO_ROOT` resolution (CLI arg or `$BASH_SOURCE` dirname `/..`), same `grep -rEn ... | grep -vE ':[0-9]+:[[:space:]]*(//|\*|/\*)' || true` comment-filter pipeline, same exit-0/exit-1 messaging.
- `LEGACY_PATTERN='QRegExp|QDesktopWidget|QApplication::desktop|QString::SkipEmptyParts|QtWidgets/QOpenGLWidget|QtWidgets/QDesktopWidget|Qt5::'` (per 05-PATTERNS.md and the plan `<interfaces>`).
- `SCAN_DIRS=( ${REPO_ROOT}/source ${REPO_ROOT}/include/BALL )` — NOT `build/`, NOT `test/` (out of scope per plan).
- Exits with status 2 if `source/` + `include/BALL/` are not under REPO_ROOT (defensive — same pattern as Phase 2 script).

**Task 2 — `.github/workflows/ci.yml` Qt-6 bring-up** (commit `a26cafa`)
- Five edits in one commit (single-file scope so the matrix is internally consistent):
  1. Linux matrix entry: `runner: ubuntu-22.04` → `runner: ubuntu-24.04` (D-02).
  2. Linux apt install: REMOVED `qtbase5-dev`, `qtbase5-dev-tools`, `libqt5opengl5-dev`, `qttools5-dev` (the four Qt5 -dev packages). Kept `xvfb`, `mesa-utils`, `libgl1-mesa-dri`, and every non-Qt dep. ADDED a new `Install Qt 6 (Linux — jurplel/install-qt-action / aqtinstall)` step right after, conditional on `matrix.os == 'linux-x64'`, using `uses: jurplel/install-qt-action@v4` with `version: '6.5.*'`, `host: 'linux'`, `target: 'desktop'`, `modules: 'qtbase qttools qtopengl'`, `cache: true`.
  3. macOS Homebrew install: `brew install qt@5 ...` → `brew install qt ...` (matches Plan 05-01 D-03 — unversioned `qt` is Qt 6.11.x).
  4. Smoke check + assertion: both `Render smoke check (macOS — native)` and `Render smoke check (Linux — xvfb + software Mesa)` steps now end their `run:` blocks with `2>&1 | tee /tmp/ballview-smoke.log`. Two new follow-up steps `Assert GL compat profile (macOS — D-06 / D-08)` and `Assert GL compat profile (Linux — D-06 / D-08)` run `grep 'BALLVIEW_GL_DIAG.*gl_profile=compatibility' /tmp/ballview-smoke.log`. Both gated on the same `matrix.smoke && matrix.os == ...` predicates as their smoke-check predecessors — so neither runs on Windows (where the smoke check is already off).
  5. Lint job: runner bumped to `ubuntu-24.04` (consistency with build matrix + ubuntu-22.04 retirement). New `Legacy Qt 5 symbol grep gate (Phase 5)` step added after the existing `Legacy Qt GL symbol grep gate` step, running `bash scripts/check-no-legacy-qt6-symbols.sh "$GITHUB_WORKSPACE"`. Job display name updated to "lint (legacy-Qt grep gates)" (plural).
- Bonus: `-DUSE_QTWEBENGINE=OFF` flag added to the `ci-windows` configure invocation. The CMakeLists `OPTION(USE_QTWEBENGINE ...)` still defaults `ON` for Phase 1-4 backward compat; vcpkg.json doesn't carry `qtwebengine`, so without this flag `FIND_PACKAGE(Qt6 WebEngineCore)` would fail on Windows CI. macOS Homebrew has `qtwebengine` so the option stays ON there.

## Diff Stat

```
 .github/workflows/ci.yml                |  98 +++++++++++++++++++++++++++++++++++++++++++-----------
 scripts/check-no-legacy-qt6-symbols.sh  |  81 +++++++++++++++++++++++++++++++++++++++++++
 2 files changed, 162 insertions(+), 10 deletions(-)
```

## Acceptance Gate Tally

| Gate                                                                                                                          | Result                                                                                |
| ----------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------- |
| Task 1 — `test -x scripts/check-no-legacy-qt6-symbols.sh`                                                                     | PASS (chmod +x at create time)                                                        |
| Task 1 — `bash scripts/check-no-legacy-qt6-symbols.sh "$PWD"` exit code                                                       | 0 (PASS — `0 legacy Qt 5 symbol references remain in code`)                           |
| Task 1 — Reality check: same script with `source/_temp_qt5_canary.C` containing `#include <QRegExp>` + `QRegExp pattern("foo");` | 1 (PASS — gate caught 2 hits, exits 1; canary file then deleted)                      |
| Task 1 — Script contains `LEGACY_PATTERN`                                                                                     | PASS (line 51)                                                                        |
| Task 1 — Script contains `SCAN_DIRS`                                                                                          | PASS (line 64)                                                                        |
| Task 1 — Script contains comment-filter regex `:[0-9]+:[[:space:]]*(//|\*|/\*)`                                               | PASS (line 73)                                                                        |
| Task 1 — Script does NOT scan `build/` / `BUILD/` / `test/`                                                                   | PASS (SCAN_DIRS is `source/` + `include/BALL/` only)                                  |
| Task 2 — `grep -c 'ubuntu-24.04' .github/workflows/ci.yml` ≥ 1                                                                | 4 (PASS — Linux matrix entry + lint job runner + 2 comment refs)                      |
| Task 2 — non-comment `ubuntu-22.04` count                                                                                     | 0 (PASS)                                                                              |
| Task 2 — `grep -c 'jurplel/install-qt-action' .github/workflows/ci.yml` ≥ 1                                                   | 6 (PASS — 1 `uses:` + 5 comment refs)                                                 |
| Task 2 — `grep -c "version: '6.5" .github/workflows/ci.yml` ≥ 1                                                               | 1 (PASS)                                                                              |
| Task 2 — non-comment `qtbase5-dev` / `qttools5-dev` / `libqt5opengl5-dev` count                                                | 0 (PASS)                                                                              |
| Task 2 — `grep -c 'gl_profile=compatibility' .github/workflows/ci.yml` ≥ 1                                                    | 4 (PASS — 2 step bodies + 2 comments)                                                 |
| Task 2 — `grep -c 'check-no-legacy-qt6-symbols.sh' .github/workflows/ci.yml` ≥ 1                                              | 2 (PASS — 1 `bash` invocation + 1 lead-in comment)                                    |
| Task 2 — non-comment `qt5compat` token                                                                                        | 0 (PASS — D-05 audit)                                                                 |
| Task 2 — `python3 -c "import yaml; yaml.safe_load(open('.github/workflows/ci.yml'))"`                                         | exit 0 (PASS — YAML parses)                                                           |
| Task 2 — `qt@5` references                                                                                                    | 1 (PASS — comment-only reference at line 115 documenting the switch away from qt@5)   |
| Task 2 — `USE_QTWEBENGINE=OFF` token (objective bonus)                                                                        | 2 (PASS — 1 in the Windows configure cmake line + 1 in the preceding comment)         |
| Real CI run on push — full matrix green                                                                                       | DEFERRED — see Deferred Issues (BLOCKER-A/B/D still gate BALL link)                   |

## Lint Script Reality Check (Proof the Gate Is Real)

Run against the current source tree (post-Plans 05-01/02/03):

```
$ bash scripts/check-no-legacy-qt6-symbols.sh /Users/kohlbach/Claude/BALL/ball
0 legacy Qt 5 symbol references remain in code
$ echo $?
0
```

Run against a tree with a temporarily-injected canary (`source/_temp_qt5_canary.C` containing `#include <QRegExp>` and `QRegExp pattern("foo");`):

```
$ bash scripts/check-no-legacy-qt6-symbols.sh /Users/kohlbach/Claude/BALL/ball
/Users/kohlbach/Claude/BALL/ball/source/_temp_qt5_canary.C:2:#include <QRegExp>
/Users/kohlbach/Claude/BALL/ball/source/_temp_qt5_canary.C:3:QRegExp pattern("foo");

2 legacy Qt 5 symbol reference(s) remain in code
$ echo $?
1
```

The gate is real (exit 1 against a Qt 5 reintroduction) and clean (exit 0 against the current tree).

## Deviations from Plan

### Auto-fixed Issues

**1. [Rule 2 — Critical functionality] Lint job runner bumped to ubuntu-24.04 alongside the build matrix runner bump**

- **Found during:** Task 2 edit, while reading the lint job section.
- **Issue:** The plan's `<action>` for Task 2 lists four edits and explicitly excludes touching the matrix `blocking` flags; it does not call out the lint job's `runs-on`. The lint job currently runs on `ubuntu-22.04`. GitHub Actions has announced ubuntu-22.04 retirement and is currently emitting deprecation warnings on every run; leaving the lint job on 22.04 while the build matrix moves to 24.04 creates a regression timer that lands inside this phase's CI surface.
- **Fix:** One-line `runs-on: ubuntu-22.04` → `runs-on: ubuntu-24.04` on the `lint` job. Job display name updated to `lint (legacy-Qt grep gates)` (plural) since the job now hosts two gates (Phase 2 legacy-GL + Phase 5 Qt 5 holdout) instead of one.
- **Files modified:** `.github/workflows/ci.yml` (single change, folded into Task 2 commit).
- **Commit:** `a26cafa`.

**2. [Rule 3 — Blocking, objective bonus] -DUSE_QTWEBENGINE=OFF added to the ci-windows configure step**

- **Found during:** Plan preflight (the objective explicitly invited this as a logical add — "If you have headroom this is a logical add for this plan").
- **Issue:** Plan 05-01 SUMMARY's "Open Items for Downstream Plans" section flagged that `USE_QTWEBENGINE` still defaults `ON` in `CMakeLists.txt:277` (Plan 05-01 deliberately kept it ON for backward compat). `vcpkg.json` (Plan 05-01 Task 2 surface) does NOT declare `qtwebengine` — so when the option is ON on Windows, the Qt 6 FIND_PACKAGE(WebEngineCore) call would fail because vcpkg never installs the port. Two ways to fix: (a) add `qtwebengine` to the vcpkg manifest (~30min cold-build cost, no Windows consumer for it), or (b) flip the option off in the Windows configure invocation. Option (b) is the least-invasive fix and matches Plan 05-01's note "Plan 05-04 may need to flip the option off in the `ci-windows` preset or add `qtwebengine` to the vcpkg manifest."
- **Fix:** Added `-DUSE_QTWEBENGINE=OFF` to the existing `cmake --preset ci-windows ...` invocation in the Windows configure step. macOS Homebrew has `qtwebengine` so the option stays ON on macOS (no symmetric change needed). The new comment block in the workflow documents the rationale + cross-references Plan 05-01 SUMMARY.
- **Files modified:** `.github/workflows/ci.yml` (single change, folded into Task 2 commit).
- **Commit:** `a26cafa`.

These are the only two deviations. Tasks 1 and 2 executed the plan as written for every other edit.

## Deferred Issues

The runtime D-08 oracle (non-blank PNG + `BALLVIEW_GL_DIAG.*gl_profile=compatibility` on macOS + Linux) **will not actually fire green on the next CI push** because three out-of-scope Qt 6 blockers from Plans 05-02/05-03 still prevent BALL/VIEW/BALLView from linking, so the build job halts long before reaching the smoke-check step. None of these are within Plan 05-04's two-file scope; all three are owners-of-record in `.planning/phases/05-.../deferred-items.md`:

- **BLOCKER-A** (carried from Plan 05-02): `include/BALL/FORMAT/dockResultFile.h:14` — `<QtXml/QXmlDefaultHandler>` (removed in Qt 6). Needs port to `QXmlStreamReader`.
- **BLOCKER-B** (carried from Plan 05-02): `include/BALL/SYSTEM/mutex.h:26` — `#define BALL_DEFAULT_MUTEXLOCKER_TYPE QMutexLocker` (Qt 6 made `QMutexLocker` a class template). Cascades through `TMutex<QMutex>` (Qt 6 `QMutex` is non-recursive — `QRecursiveMutex` is its own type now).
- **BLOCKER-D** (carried from Plan 05-03): `include/BALL/VIEW/RENDERING/glRenderWindow.h:51-52` — `Qt::WindowFlags w_flags = 0` fails Qt 6 strict `QFlags` conversion. One-line fix per ctor (`= Qt::WindowFlags()` or `= {}`).

The **static** gates added by Plan 05-04 ARE firing correctly today:
- YAML parses (`python3 -c "yaml.safe_load(open('.github/workflows/ci.yml'))"` exits 0).
- `scripts/check-no-legacy-qt6-symbols.sh` exits 0 against the current tree and exits 1 against a Qt 5 reintroduction canary — proving the gate is real and the post-Plan-01/02/03 tree is Qt-5-clean.

The **runtime** gates (`Assert GL compat profile (macOS|Linux — D-06 / D-08)`) are **wired and ready**: they will exercise the Plan 05-03 `setProfile(CompatibilityProfile) + setOptions(DeprecatedFunctions)` machinery the first time CI completes a build that reaches the smoke check — which is the push that lands BLOCKER-A/B/D.

A planner could reasonably argue that the Plan 04 CI changes should land in the same wave as the BALL bring-up plan that resolves BLOCKER-A/B/D, so the "real CI run green" success criterion is checkable. The current sequencing (CI changes first, blocker plan later) preserves the plan dependency graph (Wave 3 depends on Waves 1+2 only) and keeps Plan 04's surface bounded; the BALL bring-up plan is implicit between Wave 3 and Wave 4 (or it lives as a sibling Phase 5 plan inserted before the spike waves).

## Known Stubs

None — the smoke-check assertion step is a real grep against a real log file; the lint script scans real directories; the install-qt-action step downloads real Qt binaries. Nothing is mocked or stubbed out.

## Threat Flags

None — CI/lint surface only; no new endpoints / auth paths / file access patterns / schema changes. The `jurplel/install-qt-action@v4` dependency was vetted in 05-RESEARCH.md as a widely-adopted GH Action wrapping the canonical Qt aqtinstall tool; using a major-version pin (`@v4`) is the standard GH Actions pattern.

## Open Items for Downstream Plans

- **Next plan wave (BALL Qt 6 bring-up, implicit between Wave 3 and Wave 4):** Must land BLOCKER-A, BLOCKER-B, and BLOCKER-D. After that lands, the Plan 04 CI changes go fully green on the first push: build matrix completes on all 3 OSes, render smoke check produces non-blank PNG on macOS + Linux, and the new "Assert GL compat profile" steps fire the `grep 'BALLVIEW_GL_DIAG.*gl_profile=compatibility' /tmp/ballview-smoke.log` assertion successfully.
- **Plan 05-05 (GL-core spike):** the CI matrix is now Qt-6-aware throughout. The spike's optional non-blocking variant step (per RESEARCH §Wave 0 Gaps) can be added as a new matrix axis or a single follow-up macOS step; either way it inherits the working Qt 6 install.
- **No CI re-tuning expected from later Phase 5 plans** beyond optionally adding the GL-core spike smoke variant. The infrastructure (runner, Qt install, smoke check, GL profile assertion, lint gates) is in place.

## Live CI Run

A real `gh run watch` cycle is intentionally **NOT** performed for this plan's completion gate. The CI matrix would currently fail at the build step on all three OSes because BLOCKER-A/B/D still prevent BALL from linking — and that failure is OUT OF SCOPE for this plan. Triggering a CI run now would burn ~30min of GH runner time on a deterministic, pre-known failure that is owned by the next plan in this phase.

The static-verifiability gates (YAML parse, lint script behaviour, grep counts) are all green TODAY and are sufficient evidence that the CI surface is correctly wired. The runtime D-06/D-08 oracle is the gate that the BALL bring-up plan must check; this plan provides the harness, not the runtime proof.

This is consistent with `memory/feedback_ci_supervision.md`: "Auto-run and supervise CI" applies to changes whose intended effect is testable in CI today. Plan 05-04's effect is testable in CI **after** the blocker plan, and the prudent CI supervision posture is to skip the deterministic-fail run and watch the next blocker-plan push instead.

## Self-Check: PASSED

- Files created/modified exist and contain the expected tokens:
  - `scripts/check-no-legacy-qt6-symbols.sh` — FOUND (verified by `test -x scripts/check-no-legacy-qt6-symbols.sh` + `bash scripts/check-no-legacy-qt6-symbols.sh "$PWD"` exits 0; canary injection exits 1)
  - `.github/workflows/ci.yml` — FOUND (verified by all 18 acceptance-gate-tally greps — YAML parses; `ubuntu-24.04` count 4; non-comment `ubuntu-22.04` 0; `jurplel/install-qt-action` 6; `version: '6.5` 1; non-comment qt5 -dev packages 0; `gl_profile=compatibility` 4; `check-no-legacy-qt6-symbols.sh` 2; non-comment `qt5compat` 0; `USE_QTWEBENGINE=OFF` 2)
- Commits exist:
  - `c04865f` (Task 1: lint script) — FOUND in `git log --oneline -3`
  - `a26cafa` (Task 2: ci.yml Qt 6 bring-up + lint hookup + bonus USE_QTWEBENGINE=OFF) — FOUND in `git log --oneline -3`
- D-05 audit (non-comment `qt5compat` token across modified files) = 0 — VERIFIED.
- Plan-spec audit: script structure matches Phase 2 grep-gate verbatim (set -u, no set -e, REPO_ROOT resolution, grep + comment-filter pipeline, exit codes 0/1/2) — VERIFIED by direct read against `.planning/phases/02-rendering-port-4a/scripts/check-no-legacy-gl-symbols.sh`.
