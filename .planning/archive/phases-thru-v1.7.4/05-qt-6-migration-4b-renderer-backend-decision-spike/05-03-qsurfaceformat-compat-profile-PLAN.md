---
phase: 05-qt-6-migration-4b-renderer-backend-decision-spike
plan: 03
type: execute
wave: 2
depends_on: [01]
files_modified:
  - include/BALL/VIEW/RENDERING/glRenderWindow.h
  - source/VIEW/RENDERING/glRenderWindow.C
autonomous: true
requirements: [QT6-01]
must_haves:
  truths:
    - "D-06: QSurfaceFormat::CompatibilityProfile is set uniformly on all 3 OSes (no per-OS branching)"
    - "D-06: QSurfaceFormat::DeprecatedFunctions option is set so compat-profile drivers expose legacy fixed-function entry points"
    - "D-07: No GL_SILENCE_DEPRECATION suppression added — macOS GL-deprecation warnings remain visible at runtime"
    - "D-08: The Phase 02.2 render smoke check still produces a non-blank PNG on macOS under Qt 6 (compat profile not silently degraded to core)"
    - "D-09: The same QSurfaceFormat is used on Windows — no platform-specific tuning"
    - "BALLVIEW_GL_DIAG stdout line emits gl_profile=compatibility under Qt 6 (D-08 oracle)"
  artifacts:
    - path: "include/BALL/VIEW/RENDERING/glRenderWindow.h"
      provides: "QOpenGLWidget include via Qt 6's QtOpenGLWidgets module"
      contains: "QtOpenGLWidgets/QOpenGLWidget"
    - path: "source/VIEW/RENDERING/glRenderWindow.C"
      provides: "QSurfaceFormat static initializer with DeprecatedFunctions option"
      contains: "QSurfaceFormat::DeprecatedFunctions"
  key_links:
    - from: "source/VIEW/RENDERING/glRenderWindow.C gl_format_ static initializer"
      to: "QSurfaceFormat::CompatibilityProfile + DeprecatedFunctions"
      via: "setProfile + setOptions in the static lambda"
      pattern: "QSurfaceFormat::CompatibilityProfile"
    - from: "source/VIEW/RENDERING/glRenderWindow.C initializeGL"
      to: "BALLVIEW_GL_DIAG stdout line"
      via: "DIAG-01 diagnostic emission (already in place from Phase 02.2)"
      pattern: "BALLVIEW_GL_DIAG.*gl_profile="
---

<objective>
Keep the compatibility-profile fixed-function GL path alive under Qt 6. Qt 6 changed its default surface format — on macOS without an explicit format, the context falls back to GL 4.1 core, which silently breaks every `glBegin`/`glEnd`/`glMatrixMode`/`gluLookAt` call in `glRenderer.C` (Pitfall 2). This plan enforces D-06 + D-07 + D-09 in the two files that actually own the QSurfaceFormat decision.

Purpose: Without this plan, Plan 01 + Plan 02 will build clean but the smoke check (D-08) will produce a blank PNG → CI red. This plan is the D-06/D-08 correctness oracle in source form. It deliberately leaves macOS GL-deprecation warnings visible (D-07) as a standing reminder that compat profile on macOS is a stopgap and not a destination — SPIKE-02 (Plan 08) references that noise as one of the motivations for PIPE-01.

Output: 2 files modified. The Plan 04 CI step that greps `BALLVIEW_GL_DIAG ... gl_profile=compatibility` after smoke check is the runtime oracle for this plan.
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
@include/BALL/VIEW/RENDERING/glRenderWindow.h
@source/VIEW/RENDERING/glRenderWindow.C
@source/APPLICATIONS/BALLVIEW/main.C

<interfaces>
<!-- D-06 / D-07 / D-08 / D-09 land in two files. main.C is read for verification only — its line 68 QSurfaceFormat::setDefaultFormat call already runs before QApplication construction and is correct under Qt 6 (see Pattern Map §main.C — no code change there). -->

glRenderWindow.h (line ~32 — the QOpenGLWidget include — Pitfall 4):
  before: `#include <QtWidgets/QOpenGLWidget>`
  after:  `#include <QtOpenGLWidgets/QOpenGLWidget>`

glRenderWindow.C (lines ~37-56, the `GLRenderWindow::gl_format_` static initializer):
  - Keep `fmt.setProfile(QSurfaceFormat::CompatibilityProfile)` exactly as today (D-06, D-09).
  - ADD `fmt.setOptions(QSurfaceFormat::DeprecatedFunctions)` immediately after `setProfile()` — RESEARCH Pitfall 2 mitigation; explicit legacy entry point request.
  - Keep `fmt.setVersion(2, 1)` (Apple driver caps compat at 2.1 anyway; same as today).
  - Keep the rest of the initializer (depth 24, stencil 8, double buffer, samples 4, stereo guard) byte-identical.
  - Leave the dead `QT_VERSION_CHECK(5, 7, 0)` inner guard alone — it is harmless under Qt 6 (always-true; no behaviour change).

Do NOT add `GL_SILENCE_DEPRECATION` (D-07 forbids).
Do NOT add per-OS QSurfaceFormat divergence (D-09 forbids).
Do NOT touch the `BALLVIEW_GL_DIAG` emission in `initializeGL()` — Phase 02.2 already emits it and the line includes `gl_profile=<compatibility|core>` derived from the live context's `QSurfaceFormat::profile()`. Verify it still emits under Qt 6 by reading it.
</interfaces>
</context>

<tasks>

<task type="auto" tdd="false">
  <name>Task 1: Switch glRenderWindow.h to the QtOpenGLWidgets include path</name>
  <files>include/BALL/VIEW/RENDERING/glRenderWindow.h</files>
  <read_first>
    - include/BALL/VIEW/RENDERING/glRenderWindow.h (full file — short, &lt; 100 lines).
    - 05-PATTERNS.md §`include/BALL/VIEW/RENDERING/glRenderWindow.h` (verbatim before/after) and §`source/VIEW/RENDERING/glRenderWindow.C + include/BALL/VIEW/RENDERING/glRenderWindow.h`.
    - 05-RESEARCH.md §Pitfall 4 (QOpenGLWidget header path change).
  </read_first>
  <behavior>
    - The header includes `<QtOpenGLWidgets/QOpenGLWidget>` (Qt 6 location) and no longer includes `<QtWidgets/QOpenGLWidget>` (Qt 5 location).
    - The include style matches the project's existing internal-include convention (tab-indented `#\tinclude <...>` if used elsewhere in this file; otherwise plain `#include`).
    - No other declarations change (class inheritance, member fields, methods all untouched).
  </behavior>
  <action>
    In `include/BALL/VIEW/RENDERING/glRenderWindow.h`, locate the existing `#include <QtWidgets/QOpenGLWidget>` line (around line 32 per 05-PATTERNS.md). Replace it with `#include <QtOpenGLWidgets/QOpenGLWidget>`. Preserve any existing `#ifndef` guard around the include if present in the surrounding context (match neighbouring includes' style). Do not modify any other line in this header.
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; grep -q 'QtOpenGLWidgets/QOpenGLWidget' include/BALL/VIEW/RENDERING/glRenderWindow.h &amp;&amp; ! grep -qE '^[^/]*<QtWidgets/QOpenGLWidget>' include/BALL/VIEW/RENDERING/glRenderWindow.h &amp;&amp; cmake --build --preset ci-macos --target VIEW -j$(sysctl -n hw.ncpu) 2>&amp;1 | tee /tmp/glrw-h-build.log | tail -20</automated>
  </verify>
  <acceptance_criteria>
    - `grep -c 'QtOpenGLWidgets/QOpenGLWidget' include/BALL/VIEW/RENDERING/glRenderWindow.h` returns `>= 1`.
    - `grep -vE '^[[:space:]]*(//|\*|/\*)' include/BALL/VIEW/RENDERING/glRenderWindow.h | grep -c 'QtWidgets/QOpenGLWidget'` returns `0`.
    - `cmake --build --preset ci-macos --target VIEW` exits 0; no `error: 'QOpenGLWidget' file not found` or `error: no such file or directory 'QtWidgets/QOpenGLWidget'`.
  </acceptance_criteria>
  <done>
    glRenderWindow.h includes QOpenGLWidget from its Qt 6 home (`QtOpenGLWidgets`); VIEW compiles against Qt 6 with the Plan 01 CMake changes applied.
  </done>
</task>

<task type="auto" tdd="false">
  <name>Task 2: Add QSurfaceFormat::DeprecatedFunctions option to GLRenderWindow::gl_format_ (D-06)</name>
  <files>source/VIEW/RENDERING/glRenderWindow.C</files>
  <read_first>
    - source/VIEW/RENDERING/glRenderWindow.C lines 1-80 (the static initializer at lines 37-56 plus surrounding context — modeline, includes, and DIAG-01 emission near initializeGL).
    - source/APPLICATIONS/BALLVIEW/main.C lines 62-75 (read-only — verify the `QSurfaceFormat::setDefaultFormat(...)` call still runs before `QApplication` is constructed; do NOT modify this file).
    - 05-PATTERNS.md §`source/VIEW/RENDERING/glRenderWindow.C` and §`source/APPLICATIONS/BALLVIEW/main.C` (verbatim before/after; main.C explicitly "no code change needed").
    - 05-RESEARCH.md §Pitfall 2 (the macOS compat-profile blocker) and §`glRenderWindow.C — QSurfaceFormat::DeprecatedFunctions`.
  </read_first>
  <behavior>
    - `GLRenderWindow::gl_format_` static initializer calls `fmt.setOptions(QSurfaceFormat::DeprecatedFunctions)` immediately after `fmt.setProfile(QSurfaceFormat::CompatibilityProfile)`.
    - All other initializer fields (version 2.1, depth 24, stencil 8, double-buffer, 4× samples, stereo `#ifndef BALL_OS_DARWIN` guard) are unchanged.
    - D-07: no `GL_SILENCE_DEPRECATION` define, no `#pragma GCC diagnostic` suppression, no Info.plist edit.
    - D-09: no per-OS `#ifdef` around the `setOptions` call — it applies uniformly.
    - DIAG-01 emission at the top of `initializeGL()` still prints a single line of the form `BALLVIEW_GL_DIAG gl_vendor=... gl_renderer=... gl_version=... gl_profile=<compatibility|core> ...` (Phase 02.2; no edit needed here, just read-verify it still compiles).
  </behavior>
  <action>
    In `source/VIEW/RENDERING/glRenderWindow.C`, locate the `GLRenderWindow::gl_format_` static initializer (lambda assigned to the static member, around lines 37-56 per 05-PATTERNS.md). Immediately after the line `fmt.setProfile(QSurfaceFormat::CompatibilityProfile);`, add a new line: `fmt.setOptions(QSurfaceFormat::DeprecatedFunctions);` — match the surrounding 2-space indentation and Allman-brace style of the lambda body.

    Do not change any other line in the initializer. Do not touch the `initializeGL()` DIAG-01 emission. Do not modify `main.C` (its line 68 `QSurfaceFormat::setDefaultFormat(BALL::VIEW::GLRenderWindow::gl_format_)` is already the correct Qt 6 pattern per 05-PATTERNS.md). Do not introduce `GL_SILENCE_DEPRECATION` (D-07).
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; grep -q 'QSurfaceFormat::DeprecatedFunctions' source/VIEW/RENDERING/glRenderWindow.C &amp;&amp; ! grep -qE '^[^/]*GL_SILENCE_DEPRECATION' source/VIEW/RENDERING/glRenderWindow.C include/BALL/VIEW/RENDERING/glRenderWindow.h source/APPLICATIONS/BALLVIEW/main.C &amp;&amp; cmake --build --preset ci-macos --target BALLView -j$(sysctl -n hw.ncpu) 2>&amp;1 | tee /tmp/glrw-c-build.log | tail -20 &amp;&amp; bash .planning/phases/02.2-ci-and-build-smoke-matrix/scripts/render-smoke-check.sh build/ci-macos/bin/BALLView.app/Contents/MacOS/BALLView 2>&amp;1 | tee /tmp/smoke.log | grep -E 'BALLVIEW_GL_DIAG.*gl_profile=compatibility'</automated>
  </verify>
  <acceptance_criteria>
    - `grep -c 'setOptions(QSurfaceFormat::DeprecatedFunctions)' source/VIEW/RENDERING/glRenderWindow.C` returns `>= 1`.
    - `grep -c 'QSurfaceFormat::CompatibilityProfile' source/VIEW/RENDERING/glRenderWindow.C` is unchanged from pre-edit (still `>= 1`).
    - `grep -vE '^[[:space:]]*(//|\*|/\*)' source/VIEW/RENDERING/glRenderWindow.C include/BALL/VIEW/RENDERING/glRenderWindow.h source/APPLICATIONS/BALLVIEW/main.C | grep -c 'GL_SILENCE_DEPRECATION'` returns `0` (D-07 audit).
    - `cmake --build --preset ci-macos --target BALLView -j$(sysctl -n hw.ncpu)` exits 0.
    - `bash .planning/phases/02.2-ci-and-build-smoke-matrix/scripts/render-smoke-check.sh build/ci-macos/bin/BALLView.app/Contents/MacOS/BALLView` exits 0 AND emits a stdout line matching `BALLVIEW_GL_DIAG.*gl_profile=compatibility` (D-06 + D-08 runtime oracle).
    - The smoke check produces a non-blank PNG under Qt 6 on macOS (script's own assertion — D-08 blocker if it fails).
  </acceptance_criteria>
  <done>
    `GLRenderWindow::gl_format_` requests the compatibility profile with `DeprecatedFunctions` option uniformly across all OSes; macOS smoke check produces a non-blank PNG and the DIAG-01 line reports `gl_profile=compatibility`; no GL deprecation suppression is in place.
  </done>
</task>

</tasks>

<verification>
- `grep -c 'QtOpenGLWidgets/QOpenGLWidget' include/BALL/VIEW/RENDERING/glRenderWindow.h` returns `>= 1`.
- `grep -c 'setOptions(QSurfaceFormat::DeprecatedFunctions)' source/VIEW/RENDERING/glRenderWindow.C` returns `>= 1`.
- D-07 audit: `grep -vE '^[[:space:]]*(//|\*|/\*)' source/VIEW/RENDERING/glRenderWindow.C include/BALL/VIEW/RENDERING/glRenderWindow.h source/APPLICATIONS/BALLVIEW/main.C | grep -c 'GL_SILENCE_DEPRECATION'` returns `0`.
- D-09 audit: there is no `#ifdef BALL_OS_DARWIN` / `#ifdef BALL_OS_LINUX` / `#ifdef BALL_OS_WINDOWS` wrapping the `setOptions(...)` call (uniform application).
- macOS smoke check produces non-blank PNG and `gl_profile=compatibility` (the D-08 blocker behaviour).
</verification>

<success_criteria>
- macOS Qt 6 build green; smoke check non-blank; DIAG-01 reports compat profile.
- Plan 04's CI grep step (`grep 'BALLVIEW_GL_DIAG.*gl_profile=compatibility'`) will pass on macOS + Linux.
- D-07 deprecation warnings remain visible at runtime (verified manually in Plan 07 / SPIKE-02).
</success_criteria>

<output>
After completion, create `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-03-SUMMARY.md` with the diff for the 2 files, the captured `BALLVIEW_GL_DIAG` stdout line from the local smoke check, the smoke check PNG byte count (proxy for "non-blank"), and a confirmation that macOS GL-deprecation banner is visible at runtime per D-07.
</output>
