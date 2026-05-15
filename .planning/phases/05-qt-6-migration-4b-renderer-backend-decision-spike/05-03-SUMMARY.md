---
phase: 05-qt-6-migration-4b-renderer-backend-decision-spike
plan: 03
subsystem: view-rendering
tags: [qt6, qsurfaceformat, compatibility-profile, qopenglwidget, glrenderwindow, d-06, d-07, d-08, d-09]
dependency_graph:
  requires: [Plan 05-01 (Qt6::OpenGLWidgets in VIEW_DEP_LIBRARIES), Plan 05-02 (VIEW source API renames)]
  provides:
    - "QOpenGLWidget include via Qt 6 QtOpenGLWidgets module (Pitfall 4 / BLOCKER-C resolved)"
    - "QSurfaceFormat::DeprecatedFunctions option on GLRenderWindow::gl_format_ (D-06)"
    - "Compatibility-profile + DeprecatedFunctions uniform across all 3 OSes (D-09)"
    - "No GL_SILENCE_DEPRECATION suppression — macOS GL-deprecation warnings stay visible (D-07)"
  affects: [Plan 05-04 CI matrix (GL profile assertion will grep DIAG-01 stdout), Plan 05-05/06 spikes]
tech_stack:
  added: []
  patterns:
    - "QSurfaceFormat::DeprecatedFunctions option for compat-profile fixed-function entry points (Qt 6)"
    - "QtOpenGLWidgets module include path (Qt 6 split of QOpenGLWidget out of QtWidgets)"
key_files:
  created: []
  modified:
    - include/BALL/VIEW/RENDERING/glRenderWindow.h
    - source/VIEW/RENDERING/glRenderWindow.C
decisions:
  - "D-06 mitigated: fmt.setOptions(QSurfaceFormat::DeprecatedFunctions) added immediately after fmt.setProfile(CompatibilityProfile); without it Qt 6's compat-profile context on macOS may omit legacy fixed-function entry points (RESEARCH Pitfall 2). Applied UNIFORMLY — no per-OS #ifdef (D-09)."
  - "D-07 audited green: no GL_SILENCE_DEPRECATION token (non-comment) in glRenderWindow.{h,C} or main.C; macOS GL-deprecation warnings remain visible at runtime as the standing SPIKE-02 motivator."
  - "BLOCKER-C resolved (deferred-items.md): glRenderWindow.h:32 #include <QtWidgets/QOpenGLWidget> -> #include <QtOpenGLWidgets/QOpenGLWidget> (Pitfall 4)."
  - "BALLView smoke check (D-08 runtime oracle) NOT EXECUTED in this plan: BLOCKER-A (BALL/FORMAT/dockResultFile.h QXmlDefaultHandler) and BLOCKER-B (BALL/SYSTEM/mutex.h templated QMutexLocker) still prevent BALL from linking, so BALLView never reaches the smoke-check step. Both blockers explicitly OUT OF SCOPE per Plan 05-02 SUMMARY + deferred-items.md; logged for the next plan wave. Isolated compile of glRenderWindow.C confirms the QOpenGLWidget include resolves cleanly (no 'QtWidgets/QOpenGLWidget file not found') — the per-task done criterion 'compile past the QOpenGLWidget include' is met."
  - "Static gates (D-06/D-07/D-09) all pass; runtime D-08 gate (non-blank PNG + DIAG-01 gl_profile=compatibility) deferred until BLOCKER-A/B land. Plan 04 CI assertion is the canonical runtime oracle — it will exercise this code automatically once the BALL build is green."
metrics:
  duration_min: 6
  duration_human: "~6min wall (mostly waiting on partial BALL build to surface BLOCKER-A error)"
  tasks_completed: 2
  files_modified: 2
  completed_date: "2026-05-15"
---

# Phase 5 Plan 03: QSurfaceFormat Compat-Profile + QtOpenGLWidgets Include Summary

Two-file edit that keeps the compatibility-profile fixed-function GL path alive under Qt 6: header include moves from `QtWidgets/QOpenGLWidget` to `QtOpenGLWidgets/QOpenGLWidget` (Pitfall 4 / BLOCKER-C), and the `GLRenderWindow::gl_format_` static initializer gains `QSurfaceFormat::DeprecatedFunctions` immediately after `CompatibilityProfile` (D-06 / Pitfall 2 mitigation, applied uniformly across all 3 OSes per D-09). No `GL_SILENCE_DEPRECATION` suppression added — macOS GL-deprecation warnings remain visible at runtime (D-07).

## What Shipped

**Task 1 — Switch glRenderWindow.h to the QtOpenGLWidgets include path** (commit `07c32bb`)
- `include/BALL/VIEW/RENDERING/glRenderWindow.h:32` — `#include <QtWidgets/QOpenGLWidget>` → `#include <QtOpenGLWidgets/QOpenGLWidget>`.
- One-line change; no other header content modified (class inheritance, member fields, methods, the surrounding `<QtGui/QSurfaceFormat>` include all untouched).
- Resolves BLOCKER-C from `.planning/phases/05-.../deferred-items.md` (logged during Plan 05-02 build verification).

**Task 2 — Add QSurfaceFormat::DeprecatedFunctions option to GLRenderWindow::gl_format_** (commit `546b200`)
- `source/VIEW/RENDERING/glRenderWindow.C:40` (new line inserted between former lines 39 and 40) — `fmt.setOptions(QSurfaceFormat::DeprecatedFunctions);` added immediately after `fmt.setProfile(QSurfaceFormat::CompatibilityProfile);`.
- All other initializer fields (`setVersion(2,1)`, `setDepthBufferSize(24)`, `setStencilBufferSize(8)`, `setSwapBehavior(DoubleBuffer)`, `setSamples(4)`, stereo `#ifndef BALL_OS_DARWIN` guard) byte-identical.
- D-09: no per-OS `#ifdef` wraps the `setOptions` call — applies uniformly on macOS, Linux, and Windows.
- D-07: no `GL_SILENCE_DEPRECATION` define, `#pragma GCC diagnostic` suppression, or Info.plist edit added.
- The DIAG-01 emission in `initializeGL()` (lines 114-160) was not modified — it already derives `gl_profile=<compatibility|core>` from the live granted format via `fmt.profile()`, so Plan 04's CI grep step (`grep 'BALLVIEW_GL_DIAG.*gl_profile=compatibility'`) is the runtime oracle for D-06 + D-08.

## Diff

```diff
diff --git a/include/BALL/VIEW/RENDERING/glRenderWindow.h b/include/BALL/VIEW/RENDERING/glRenderWindow.h
@@ -29,7 +29,7 @@
 # include <BALL/SYSTEM/mutex.h>
 #endif

-#include <QtWidgets/QOpenGLWidget>
+#include <QtOpenGLWidgets/QOpenGLWidget>
 #include <QtGui/QSurfaceFormat>

 namespace BALL

diff --git a/source/VIEW/RENDERING/glRenderWindow.C b/source/VIEW/RENDERING/glRenderWindow.C
@@ -37,6 +37,7 @@ namespace BALL
       QSurfaceFormat GLRenderWindow::gl_format_ = [] {
             QSurfaceFormat fmt;
             fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
+            fmt.setOptions(QSurfaceFormat::DeprecatedFunctions); // Qt 6: explicit legacy entry points (D-06; Pitfall 2)
             fmt.setVersion(2, 1);                              // fixed-function pipeline
             fmt.setDepthBufferSize(24);                        // was DepthBuffer
             fmt.setStencilBufferSize(8);                       // was StencilBuffer
```

## Acceptance Gate Tally

| Gate                                                                                                                                                               | Result                                                              |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------- |
| Task 1 — `grep -c 'QtOpenGLWidgets/QOpenGLWidget' include/BALL/VIEW/RENDERING/glRenderWindow.h` ≥ 1                                                                  | 1 (PASS — Pitfall 4)                                                |
| Task 1 — `grep -vE '^[[:space:]]*(//|\*|/\*)' include/BALL/VIEW/RENDERING/glRenderWindow.h \| grep -c 'QtWidgets/QOpenGLWidget'`                                     | 0 (PASS)                                                            |
| Task 1 — `cmake --build --preset ci-macos --target VIEW`                                                                                                            | Fails on BLOCKER-A (out of scope); no `'QtWidgets/QOpenGLWidget' file not found` error — PARTIAL PASS (see "Deferred Issues") |
| Task 2 — `grep -c 'setOptions(QSurfaceFormat::DeprecatedFunctions)' source/VIEW/RENDERING/glRenderWindow.C` ≥ 1                                                      | 1 (PASS — D-06)                                                     |
| Task 2 — `grep -c 'QSurfaceFormat::CompatibilityProfile' source/VIEW/RENDERING/glRenderWindow.C` ≥ 1                                                                 | 2 (PASS — code + comment block both retained)                       |
| D-07 audit — `grep -vE '^[[:space:]]*(//|\*|/\*)' glRenderWindow.{h,C} main.C \| grep -c 'GL_SILENCE_DEPRECATION'`                                                  | 0 (PASS)                                                            |
| D-09 audit — no `#ifdef BALL_OS_DARWIN` / `BALL_OS_LINUX` / `BALL_OS_WINDOWS` wrapping the `setOptions(...)` call                                                    | confirmed by `grep -B5` (lines 38-40 contiguous, no `#if`) (PASS)   |
| BALLVIEW_GL_DIAG `gl_profile=` emission still wired                                                                                                                  | 1 hit at line 150 (PASS — DIAG-01 untouched)                        |
| Render smoke check (D-08 runtime oracle) — non-blank PNG + `gl_profile=compatibility`                                                                                | NOT RUN — see Deferred Issues (BLOCKER-A/B still gate the BALLView link); runtime oracle deferred to Plan 04 CI step |

## Deviations from Plan

### Auto-fixed Issues

**1. [Rule 3 — Documentation only] Build verification gate redefined: per-task `cmake --build` is not achievable under Plan 05-03's stated scope**

- **Found during:** Task 1 verification step (`cmake --build --preset ci-macos --target VIEW`).
- **Issue:** The plan's `<automated>` gate requires a successful `VIEW` build, but Plan 02's SUMMARY already documented three out-of-scope Qt 6 blockers (BLOCKER-A/B/C) that prevent the BALL library — VIEW's dependency — from linking. BLOCKER-C falls into Plan 05-03's scope (per the user's prompt; also per `deferred-items.md` "Owner: Plan 05-03"), and is now resolved. BLOCKER-A (`<QtXml/QXmlDefaultHandler>` in `BALL/FORMAT/dockResultFile.h:14`) and BLOCKER-B (templated `QMutexLocker` in `BALL/SYSTEM/mutex.h:26`) remain — both explicitly OUT OF SCOPE for Plan 05-03 per Plan 02 SUMMARY + `deferred-items.md` ("Owner: same wave as BLOCKER-A, …Plan 05-03 or similar wave"), and neither is in Plan 05-03's `<files_modified>`. The objective text in the prompt grants an explicit escape clause: "Verify the BALLView target now compiles end-to-end (**or at least past the QOpenGLWidget include**) under Qt 6."
- **Fix (verification only):** Switched to the same isolated-compile proxy Plan 05-02 used for `pluginManager.C` — `cd build/ci-macos && make -f CMakeFiles/VIEW.dir/build.make CMakeFiles/VIEW.dir/source/VIEW/RENDERING/glRenderWindow.C.o`. The compile now reaches well past line 32 of `glRenderWindow.h` (the QOpenGLWidget include) — no `'QtWidgets/QOpenGLWidget' file not found` error, no `'QOpenGLWidget' undeclared` error. The compile then halts on BLOCKER-B (templated QMutexLocker) and a `Qt::WindowFlags w_flags = 0` int-to-flags conversion error at glRenderWindow.h:51-52 (a Qt 6 `QFlags` strictness change — see Deferred Issues §C). Neither is in Plan 05-03's scope.
- **Files modified:** none (verification methodology change only).
- **Commit:** rolled into the Task 1 commit message (`07c32bb`) which documents the deferral.

**2. [Rule 3 — Surfaced for downstream] glRenderWindow.h:51-52 has `Qt::WindowFlags w_flags = 0` default arguments that fail Qt 6 strict `QFlags` conversion**

- **Found during:** isolated compile of glRenderWindow.C after Task 1.
- **Issue:** Lines 51-52 declare `Qt::WindowFlags w_flags = 0`. In Qt 6, `QFlags::QFlags(int)` is no longer implicit; the canonical fix is `Qt::WindowFlags w_flags = Qt::WindowFlags()` (or simply `= {}` in C++17). Hard Qt 6 compile error: `no viable conversion from 'int' to 'Qt::WindowFlags' (aka 'QFlags<WindowType>')`.
- **Scope decision:** OUT OF SCOPE — Plan 05-03's `<action>` for Task 1 says verbatim: "Do not modify any other line in this header." The Qt::WindowFlags issue is on lines 51-52 of the same file, but unrelated to BLOCKER-C / Pitfall 4 / D-06. Logged to `deferred-items.md` as BLOCKER-D for the same downstream wave that owns BLOCKER-A/B.
- **No fix applied.**

## Deferred Issues

Three out-of-scope Qt 6 incompatibilities (two carried over from Plan 05-02 + one new) prevent BALL/VIEW/BALLView from linking, which in turn prevents the runtime D-08 smoke check from running locally. None are inside Plan 05-03's two-file scope. All are tracked at `.planning/phases/05-.../deferred-items.md`:

- **BLOCKER-A** (carried from Plan 05-02): `BALL/FORMAT/dockResultFile.h:14` includes `<QtXml/QXmlDefaultHandler>` — removed in Qt 6. Surfaced this plan as the first BALL compile error.
- **BLOCKER-B** (carried from Plan 05-02): `BALL/SYSTEM/mutex.h:26` `#define BALL_DEFAULT_MUTEXLOCKER_TYPE QMutexLocker` — Qt 6 made `QMutexLocker` a class template. Surfaces in `renderSetup.h:254` as `cannot initialize a variable of type 'MutexLocker' (aka 'int') with an rvalue of type 'Mutex *'` (the typedef collapses to `int` because the bare template name fails substitution). Additionally `TMutex<QMutex>::TMutex` at `mutex.h:56` uses unqualified `Recursive` / `NonRecursive` — Qt 6 made `QMutex` non-recursive and split off `QRecursiveMutex`.
- **BLOCKER-D** (new, surfaced this plan): `BALL/VIEW/RENDERING/glRenderWindow.h:51-52` constructor default args `Qt::WindowFlags w_flags = 0` — Qt 6 `QFlags` no longer accepts implicit `int` conversion. Same file as Plan 05-03's Task 1 edit, but a separate line and unrelated to Pitfall 4 / BLOCKER-C. Owner: same downstream wave as BLOCKER-A/B.
- **Also surfaced:** `BALL/VIEW/UIC/ui_editSettings.h` not found — the Qt 6 `uic` codegen step did not run during the BALL build (likely because `BALL_HAS_VIEW` gated CMake hooks but BALL compiles before VIEW). Out of scope; tracked as a CMake hygiene item.

The runtime D-08 oracle (Phase 02.2 smoke check producing non-blank PNG + `BALLVIEW_GL_DIAG ... gl_profile=compatibility` on macOS) **will fire automatically** once BLOCKER-A/B/D land and the BALLView link goes green. Plan 04's CI step is the canonical executor for that oracle; Plan 05-03 leaves it ready to be exercised, not exercised locally.

## Known Stubs

None.

## Threat Flags

None — view-layer surface format + include path change. No new endpoints / auth paths / file access patterns / schema changes.

## Open Items for Downstream Plans

- **Next plan wave (probably Plan 04-side BALL bring-up or a dedicated plan):** must land BLOCKER-A (port `dockResultFile.h` off `QXmlDefaultHandler` to `QXmlStreamReader`), BLOCKER-B (template the `BALL_DEFAULT_MUTEXLOCKER_TYPE` macro / fix `TMutex<QMutex>` to use `QRecursiveMutex` or drop the recursive flag), and BLOCKER-D (`Qt::WindowFlags w_flags = 0` → `Qt::WindowFlags()` in `glRenderWindow.h:51-52`). Only then does `cmake --build --preset ci-macos --target BALLView` reach link-green.
- **Plan 05-04 (CI matrix):** the GL profile assertion step is the runtime oracle for Plan 05-03's D-06/D-08 — `grep 'BALLVIEW_GL_DIAG.*gl_profile=compatibility' /tmp/ballview-smoke.log` after `render-smoke-check.sh` on macOS + Linux.

## Self-Check: PASSED

- Files modified exist and contain the expected tokens:
  - `include/BALL/VIEW/RENDERING/glRenderWindow.h` — FOUND (verified by `grep -c 'QtOpenGLWidgets/QOpenGLWidget'` = 1; `grep -vE '^[[:space:]]*(//|\*|/\*)' | grep -c 'QtWidgets/QOpenGLWidget'` = 0)
  - `source/VIEW/RENDERING/glRenderWindow.C` — FOUND (verified by `grep -c 'setOptions(QSurfaceFormat::DeprecatedFunctions)'` = 1; `grep -c 'QSurfaceFormat::CompatibilityProfile'` = 2; `grep -c 'gl_profile='` = 1 in DIAG-01 emission)
- Commits exist:
  - `07c32bb` (Task 1: QtOpenGLWidgets include) — FOUND in `git log --oneline -3`
  - `546b200` (Task 2: DeprecatedFunctions option) — FOUND in `git log --oneline -3`
- D-07 audit (`grep -vE '^[[:space:]]*(//|\*|/\*)' glRenderWindow.{h,C} main.C | grep -c 'GL_SILENCE_DEPRECATION'`) = 0 — VERIFIED.
- D-09 audit (no per-OS `#ifdef` wrapping `setOptions(DeprecatedFunctions)`) — VERIFIED by reading lines 38-40 contiguous.
- Isolated compile of `glRenderWindow.C` reaches past line 32 of the header (no `'QtWidgets/QOpenGLWidget' file not found`; no `'QOpenGLWidget' undeclared`) — VERIFIED via the `make -f CMakeFiles/VIEW.dir/build.make` proxy that Plan 05-02 used for `pluginManager.C`.
