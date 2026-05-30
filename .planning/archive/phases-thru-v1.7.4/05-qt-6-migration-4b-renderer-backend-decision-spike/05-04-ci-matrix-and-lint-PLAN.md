---
phase: 05-qt-6-migration-4b-renderer-backend-decision-spike
plan: 04
type: execute
wave: 3
depends_on: [01, 02, 03]
files_modified:
  - .github/workflows/ci.yml
  - scripts/check-no-legacy-qt6-symbols.sh
autonomous: true
requirements: [QT6-01, QT6-02]
must_haves:
  truths:
    - "D-02: Linux CI matrix runner is ubuntu-24.04 (not ubuntu-22.04)"
    - "D-02: Qt 6.5+ is installed on Linux CI via jurplel/install-qt-action (apt's Qt 6.4.2 is below the D-01 floor)"
    - "D-06 + D-08: CI asserts BALLVIEW_GL_DIAG stdout contains gl_profile=compatibility after smoke check (non-blank PNG is not enough — confirms compat profile, not silently-degraded core)"
    - "D-08: Phase 02.2 smoke check (non-blank PNG on macOS + Linux) remains a blocking merge gate under Qt 6"
    - "Legacy-Qt5-symbol lint job fails CI if QRegExp, QDesktopWidget, QApplication::desktop, QString::SkipEmptyParts, QtWidgets/QOpenGLWidget, or Qt5:: appear in source/ or include/ after Phase 5"
  artifacts:
    - path: ".github/workflows/ci.yml"
      provides: "Linux matrix runner bump, jurplel/install-qt-action step, GL profile assertion step, new legacy-Qt5-symbol lint step"
      contains: "ubuntu-24.04"
    - path: "scripts/check-no-legacy-qt6-symbols.sh"
      provides: "Static-grep lint script — exits 1 if any legacy Qt 5 symbol survives in source"
      contains: "LEGACY_PATTERN"
  key_links:
    - from: ".github/workflows/ci.yml linux-x64 matrix entry"
      to: "Qt 6.5+ installation"
      via: "jurplel/install-qt-action@v4 with version: '6.5.*'"
      pattern: "jurplel/install-qt-action"
    - from: ".github/workflows/ci.yml smoke-check follow-up step"
      to: "BALLVIEW_GL_DIAG gl_profile=compatibility"
      via: "grep on captured stdout log"
      pattern: "gl_profile=compatibility"
    - from: ".github/workflows/ci.yml lint job"
      to: "scripts/check-no-legacy-qt6-symbols.sh"
      via: "bash invocation in the lint job"
      pattern: "check-no-legacy-qt6-symbols.sh"
---

<objective>
Bring CI to Qt 6 — bump the Linux runner, install Qt 6.5+ via the canonical aqtinstall GitHub Action, add the D-06/D-08 runtime assertion (`gl_profile=compatibility` in the diag line), and add a new static-grep lint job that fails CI if any legacy Qt 5 symbol survives in source.

Purpose: Plans 01-03 deliver the Qt 6 build locally. This plan is the regression net: it forces every push to demonstrate (1) Qt 6.5+ resolves on Linux without falling back to apt's 6.4.2, (2) the compat profile is actually live at runtime under Qt 6 (not silently degraded to core), and (3) no Qt 5 holdouts crept back in. Without (1) the Linux build never reaches the smoke check; without (2) Pitfall 2 ships silently; without (3) Phase 5 leaves Qt 5 footguns for Phase 6+.

Output: 1 workflow + 1 new lint script. The full CI matrix turns green against the Qt 6 source tree.
</objective>

<execution_context>
@$HOME/.claude/get-shit-done/workflows/execute-plan.md
@$HOME/.claude/get-shit-done/templates/summary.md
</execution_context>

<context>
@.planning/STATE.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-CONTEXT.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-RESEARCH.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-PATTERNS.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-VALIDATION.md
@.github/workflows/ci.yml
@.planning/phases/02-rendering-port-4a/scripts/check-no-legacy-gl-symbols.sh
@.planning/phases/02.2-ci-and-build-smoke-matrix/scripts/render-smoke-check.sh

<interfaces>
<!-- CI surface changes are bounded to .github/workflows/ci.yml + one new script. -->

ci.yml — four distinct edits, all per 05-PATTERNS.md §`.github/workflows/ci.yml`:

  1. Linux matrix entry runner: `runner: ubuntu-22.04` → `runner: ubuntu-24.04` (D-02).

  2. Linux "Install dependencies" step: REMOVE apt Qt packages (`qtbase5-dev`, `qtbase5-dev-tools`, `libqt5opengl5-dev`, `qttools5-dev`); ADD a `jurplel/install-qt-action@v4` step before the configure step, conditional on `matrix.os == 'linux-x64'`:
       version: '6.5.*'
       host:    'linux'
       target:  'desktop'
       modules: 'qtbase qttools qtopengl'   (do NOT include qt5compat — D-05)
       cache:   true
     Keep `xvfb mesa-utils libgl1-mesa-dri` in the apt list (headless smoke check still needs these).

  3. After the existing "Render smoke check" step on macOS + Linux, add a new step "Assert GL compat profile" that greps the captured smoke-check stdout for `BALLVIEW_GL_DIAG.*gl_profile=compatibility`. The smoke-check stdout must be redirected to a file (`/tmp/ballview-smoke.log` per 05-PATTERNS.md) so the grep step has something to read. If the existing smoke step does not already tee to a file, update its `run:` block to do so (one extra line — `| tee /tmp/ballview-smoke.log`). The new step is `matrix.smoke && (matrix.os == 'macos-arm64' || matrix.os == 'linux-x64')` — blocking on the same OSes as the smoke check itself.

  4. In the standalone `lint` job, add a step that runs `bash scripts/check-no-legacy-qt6-symbols.sh "$GITHUB_WORKSPACE"`. Keep the existing legacy-GL lint step.

scripts/check-no-legacy-qt6-symbols.sh — new file, modeled verbatim on `.planning/phases/02-rendering-port-4a/scripts/check-no-legacy-gl-symbols.sh` per 05-PATTERNS.md §`scripts/check-no-legacy-qt6-symbols.sh`. Pattern set:
  LEGACY_PATTERN='QRegExp|QDesktopWidget|QApplication::desktop|QString::SkipEmptyParts|QtWidgets/QOpenGLWidget|QtWidgets/QDesktopWidget|Qt5::'
  SCAN_DIRS=( "${REPO_ROOT}/source" "${REPO_ROOT}/include/BALL" )
Use the Phase 02 script's comment-filter pattern verbatim (filters out `// ...` and `/* ... */` lines):
  grep -rEn "${LEGACY_PATTERN}" "${SCAN_DIRS[@]}" 2>/dev/null | grep -vE ':[0-9]+:[[:space:]]*(//|\*|/\*)' || true
Exit 1 if any hits remain after comment filtering; exit 0 otherwise. Echo the hit count on failure for diagnostic clarity.

NOTE on script location: per 05-PATTERNS.md, the canonical analog lives under `.planning/phases/02-rendering-port-4a/scripts/`. Place the NEW Qt6 lint script at `scripts/check-no-legacy-qt6-symbols.sh` (repo top-level `scripts/` directory) — that is the path 05-PATTERNS.md and 05-VALIDATION.md reference for CI wiring. If `scripts/` does not yet exist at repo root, create it and add a sibling `README.md` is NOT required (the existing Phase 02 script under `.planning/phases/02-.../scripts/` is the reference; the Phase 5 script lives at top-level for direct CI discovery).
</interfaces>
</context>

<tasks>

<task type="auto" tdd="false">
  <name>Task 1: Write the new static-grep lint script for Qt 5 holdouts</name>
  <files>scripts/check-no-legacy-qt6-symbols.sh</files>
  <read_first>
    - .planning/phases/02-rendering-port-4a/scripts/check-no-legacy-gl-symbols.sh (full file — short bash script; this is the canonical analog).
    - 05-PATTERNS.md §`scripts/check-no-legacy-qt6-symbols.sh` (verbatim shell + LEGACY_PATTERN + SCAN_DIRS + grep+filter block).
  </read_first>
  <behavior>
    - Script is invokable as `bash scripts/check-no-legacy-qt6-symbols.sh [REPO_ROOT]`. With no arg, it resolves repo root via `$(cd "$(dirname "$BASH_SOURCE[0]")" && pwd)/..`.
    - Scans `${REPO_ROOT}/source` and `${REPO_ROOT}/include/BALL` recursively.
    - Pattern set (extended grep): `QRegExp|QDesktopWidget|QApplication::desktop|QString::SkipEmptyParts|QtWidgets/QOpenGLWidget|QtWidgets/QDesktopWidget|Qt5::`.
    - Comment-filter: lines matching `:N:<whitespace>(// or * or /*)` are excluded (header prose / Doxygen blocks do not invalidate the gate — 05-PATTERNS.md grep gate hygiene).
    - Exits 1 with hit listing + count if any non-comment match survives; exits 0 with `0 legacy Qt 5 symbol references remain in code` otherwise.
    - Uses `set -u` (the Phase 02 convention); does NOT use `set -e` because the grep + `|| true` pattern relies on the failing grep being non-fatal.
  </behavior>
  <action>
    Create `scripts/check-no-legacy-qt6-symbols.sh` with executable permission (`chmod +x`). Model the structure verbatim on `.planning/phases/02-rendering-port-4a/scripts/check-no-legacy-gl-symbols.sh`: same shebang (`#!/usr/bin/env bash`), same `set -u`, same `REPO_ROOT` resolution (CLI arg or `$BASH_SOURCE` dirname `/..`), same `grep -rEn ... | grep -vE ':[0-9]+:[[:space:]]*(//|\*|/\*)' || true` pipeline, same exit-0/exit-1 messaging.

    Use the exact LEGACY_PATTERN and SCAN_DIRS from `<interfaces>` above. Do NOT add `BUILD/` or `build/` to SCAN_DIRS (build artifacts are not source). Do NOT add `test/` (out of scope — Phase 9 territory). The grep gate must filter comments to avoid self-invalidation (cited project convention).

    `chmod +x scripts/check-no-legacy-qt6-symbols.sh` after creation.
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; test -x scripts/check-no-legacy-qt6-symbols.sh &amp;&amp; bash scripts/check-no-legacy-qt6-symbols.sh "$PWD" 2>&amp;1 | tee /tmp/qt6-lint.log &amp;&amp; tail -2 /tmp/qt6-lint.log | grep -qE '(0 legacy Qt 5 symbol references remain in code|[0-9]+ legacy Qt 5 symbol reference)'</automated>
  </verify>
  <acceptance_criteria>
    - File exists and is executable (`test -x scripts/check-no-legacy-qt6-symbols.sh`).
    - Running `bash scripts/check-no-legacy-qt6-symbols.sh "$PWD"` AFTER Plans 01/02/03 land produces `0 legacy Qt 5 symbol references remain in code` and exits 0.
    - Running it BEFORE Plans 01/02/03 land (against the Qt 5 tree) returns >0 hits and exits 1 — this proves the gate is real, not a no-op. (Executor verifies by running once at baseline, then after Plans 01-03 commits.)
    - Script body contains `LEGACY_PATTERN`, `SCAN_DIRS`, and the comment-filter regex `:[0-9]+:[[:space:]]*(//|\*|/\*)` (per 05-PATTERNS.md hygiene rule).
    - Script does NOT scan `build/`, `BUILD/`, or `test/` directories.
  </acceptance_criteria>
  <done>
    The Qt 5 holdout lint script is in place, executable, and exits 0 against the post-Plan-01/02/03 source tree; the gate is real (non-zero exit against a hypothetical Qt 5 reintroduction).
  </done>
</task>

<task type="auto" tdd="false">
  <name>Task 2: Update ci.yml — Linux runner + Qt install action + GL profile assertion + lint hookup</name>
  <files>.github/workflows/ci.yml</files>
  <read_first>
    - .github/workflows/ci.yml (full file — read lines 1-360 in one pass to see matrix structure, Linux apt install step, smoke-check step, and standalone lint job).
    - 05-PATTERNS.md §`.github/workflows/ci.yml` for the verbatim runner bump, jurplel/install-qt-action snippet, GL profile assertion step, and lint hookup.
    - 05-RESEARCH.md §Pitfall 1 (Ubuntu 24.04 Qt 6.4.2 below floor — jurplel/install-qt-action is the mitigation) and Open Question 1 (version pin recommendation 6.5.*).
  </read_first>
  <behavior>
    - Linux matrix entry sets `runner: ubuntu-24.04` (D-02).
    - A `jurplel/install-qt-action@v4` step runs before the configure step on Linux, pinned to Qt `6.5.*`, host `linux`, target `desktop`, modules `qtbase qttools qtopengl`, with `cache: true`. The four Qt5 apt packages are removed from the Linux apt install list. xvfb + Mesa stays.
    - The smoke-check step tees its stdout to `/tmp/ballview-smoke.log` (on the OSes where it runs — macOS + Linux). A follow-up step "Assert GL compat profile" runs after the smoke check (same OS gating), greps that log for `BALLVIEW_GL_DIAG.*gl_profile=compatibility`, and exits non-zero if the line is absent.
    - The standalone `lint` job gains one new step that runs the Plan 04 Task 1 script: `bash scripts/check-no-legacy-qt6-symbols.sh "$GITHUB_WORKSPACE"`. The existing legacy-GL lint step is preserved.
    - No new `continue-on-error: true` is added except in the legitimate non-blocking spike variant (which is OUT OF SCOPE for this plan — Plan 04 does NOT add the spike step yet; Plan 05 adds it).
    - The matrix's `continue-on-error: ${{ !matrix.blocking }}` pattern from Phase 04 is preserved.
  </behavior>
  <action>
    Edit `.github/workflows/ci.yml`:

    1. **Runner bump (D-02)** — In the matrix `include` list, find the entry for `os: linux-x64` and change `runner: ubuntu-22.04` to `runner: ubuntu-24.04`.

    2. **jurplel/install-qt-action step** — In the Linux apt install step's package list, remove `qtbase5-dev`, `qtbase5-dev-tools`, `libqt5opengl5-dev`, and `qttools5-dev` (keep `xvfb`, `mesa-utils`, `libgl1-mesa-dri`, and any other non-Qt packages). Insert a new step BEFORE the configure step (between "Install dependencies (Linux)" and "Configure"), conditional on `if: matrix.os == 'linux-x64'`, using `uses: jurplel/install-qt-action@v4` with `with:` parameters `version: '6.5.*'`, `host: 'linux'`, `target: 'desktop'`, `modules: 'qtbase qttools qtopengl'`, `cache: true`. Do NOT add `qt5compat` to modules (D-05).

    3. **GL profile assertion (D-06 + D-08)** — Locate the existing "Render smoke check (macOS)" and "Render smoke check (Linux)" steps. If their `run:` blocks do not already redirect stdout to a file, modify the script invocation to `bash ... 2>&1 | tee /tmp/ballview-smoke.log`. Then add a new step after each: name "Assert GL compat profile (macOS)" / "(Linux)", with `if: matrix.smoke && matrix.os == 'macos-arm64'` / `linux-x64`, running `grep 'BALLVIEW_GL_DIAG.*gl_profile=compatibility' /tmp/ballview-smoke.log`. The grep step's non-zero exit becomes the assertion failure.

    4. **Lint hookup** — In the standalone `lint` job (separate from `build`), add a new step "Legacy Qt 5 symbol grep gate" that runs `bash scripts/check-no-legacy-qt6-symbols.sh "$GITHUB_WORKSPACE"`. Place it after the existing "No legacy GL symbols" step (same job, same checkout).

    Do NOT touch the matrix `blocking` flags (Windows stays at whatever Plan 04-04 left it — currently `blocking: true`). Do NOT pre-add the Plan 05 spike variant step. Do NOT change cache keys (`hashFiles(...)` pattern is correct as-is).
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; grep -q 'ubuntu-24.04' .github/workflows/ci.yml &amp;&amp; ! grep -qE '^[^#]*ubuntu-22\.04' .github/workflows/ci.yml &amp;&amp; grep -q 'jurplel/install-qt-action' .github/workflows/ci.yml &amp;&amp; grep -q "version: '6.5" .github/workflows/ci.yml &amp;&amp; grep -q 'gl_profile=compatibility' .github/workflows/ci.yml &amp;&amp; grep -q 'check-no-legacy-qt6-symbols.sh' .github/workflows/ci.yml &amp;&amp; ! grep -qE '^[^#]*qtbase5-dev' .github/workflows/ci.yml &amp;&amp; (command -v actionlint &gt;/dev/null &amp;&amp; actionlint .github/workflows/ci.yml || python3 -c "import yaml; yaml.safe_load(open('.github/workflows/ci.yml'))")</automated>
  </verify>
  <acceptance_criteria>
    - `grep -c 'ubuntu-24.04' .github/workflows/ci.yml` returns `>= 1`; `grep -vE '^[[:space:]]*#' .github/workflows/ci.yml | grep -c 'ubuntu-22.04'` returns `0`.
    - `grep -c 'jurplel/install-qt-action@v4' .github/workflows/ci.yml` returns `>= 1` AND `grep -c "version: '6.5" .github/workflows/ci.yml` returns `>= 1`.
    - `grep -vE '^[[:space:]]*#' .github/workflows/ci.yml | grep -cE 'qtbase5-dev|qttools5-dev|libqt5opengl5-dev'` returns `0` (Qt5 apt packages gone from non-comment lines).
    - `grep -c 'BALLVIEW_GL_DIAG.*gl_profile=compatibility' .github/workflows/ci.yml` returns `>= 1`.
    - `grep -c 'check-no-legacy-qt6-symbols.sh' .github/workflows/ci.yml` returns `>= 1`.
    - YAML parses cleanly: `python3 -c "import yaml; yaml.safe_load(open('.github/workflows/ci.yml'))"` exits 0, OR `actionlint .github/workflows/ci.yml` exits 0 if available.
    - After push, the full GitHub Actions CI matrix (build × {macos-arm64, linux-x64, windows-x64} + lint) turns green; the smoke-check + GL profile assertion + new Qt5 lint step all pass. (Per memory: feedback_ci_supervision.md — supervise the run, fix iterate to green.)
    - `qt5compat` is NOT in the modules list (D-05 audit).
  </acceptance_criteria>
  <done>
    Linux CI runs on ubuntu-24.04, installs Qt 6.5+ via aqtinstall, asserts the live GL context is compatibility profile post-smoke, and the lint job gates Qt 5 symbol regressions. Full CI matrix green on a real push.
  </done>
</task>

</tasks>

<verification>
- Plans 01/02/03 changes already merged into the working tree before this plan begins (depends_on: [01, 02, 03]).
- `bash scripts/check-no-legacy-qt6-symbols.sh "$PWD"` exits 0 locally.
- YAML parses; `actionlint` (if installed) reports no errors.
- Real CI run on a push or PR turns all jobs green: `build:macos-arm64`, `build:linux-x64`, `build:windows-x64`, `lint`. Smoke check produces non-blank PNG on macOS + Linux; GL profile assertion grep matches on both. (See memory: supervise CI via `gh run watch` + iterate.)
- D-05 audit in CI: `qt5compat` not in modules; D-09 audit: no per-OS QSurfaceFormat divergence introduced.
</verification>

<success_criteria>
- Full CI matrix green on Qt 6 against the Plans 01-03 source tree.
- D-06/D-08 oracle (`gl_profile=compatibility` in DIAG output) wired and asserted on every push.
- Qt 5 symbol regressions gated by the new lint script.
- Linux installs Qt 6.5+ reliably via jurplel/install-qt-action (apt's 6.4.2 bypassed).
</success_criteria>

<output>
After completion, create `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-04-SUMMARY.md` with the green CI run URL (from `gh run watch`), the runtime captured `BALLVIEW_GL_DIAG` line for each smoke-check platform (macOS + Linux), and the lint job log excerpt showing the new Qt5 grep gate firing green. Phase 5 migration half (criteria 1-3) is now landed at this point.
</output>
