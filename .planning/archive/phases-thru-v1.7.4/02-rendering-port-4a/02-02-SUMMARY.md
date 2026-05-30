---
phase: 02-rendering-port-4a
plan: 02
subsystem: rendering
tags: [opengl, qt5, qopenglwidget, qsurfaceformat, paintgl, qpainter, context-sharing]

# Dependency graph
requires:
  - phase: 02-rendering-port-4a
    provides: "Wave 0 grep gate + A1 resolution (raytracer worker issues no GL)"
provides:
  - "GLRenderWindow rebased on QOpenGLWidget with a compatibility-profile QSurfaceFormat (GL 2.1)"
  - "GL work relocated into initializeGL/resizeGL/paintGL; manual buffer-swap machinery deleted"
  - "On-screen text reimplemented as a QPainter overlay"
  - "BALLView main() establishes global context sharing + default surface format before QApplication"
affects: [02-03, 02-04]

# Tech tracking
tech-stack:
  added: []
  patterns:
    - "QOpenGLWidget GUI-thread render path: initializeGL/resizeGL/paintGL, auto-swap, update()-driven repaint"
    - "QPainter overlay on the GL widget for 2D text (replaces removed QGLWidget::renderText)"
    - "Global GL context sharing via Qt::AA_ShareOpenGLContexts + QSurfaceFormat::setDefaultFormat in main()"

key-files:
  created: []
  modified:
    - include/BALL/VIEW/RENDERING/glRenderWindow.h
    - source/VIEW/RENDERING/glRenderWindow.C
    - source/APPLICATIONS/BALLVIEW/main.C

key-decisions:
  - "gl_format_ promoted from protected to public so BALLView main() can install it as the default surface format (Rule 3 — blocking issue from the Task 1 base-class swap)"
  - "QGLFormat::hasOpenGL() probe in main() dropped — the symbol is removed with QGLWidget and QOpenGLWidget negotiates/​reports context availability itself"
  - "paintGL() delegates to the existing refresh() body rather than inlining it — keeps the RenderWindow virtual contract intact for Plans 03/04"
  - "lockGLContext()/unlockGLContext() kept (not deleted) but annotated GUI-thread-only; caller audit deferred to Plans 03/04 per the plan"

requirements-completed: [RENDER-01, RENDER-03, RENDER-05]

# Metrics
duration: 12min
completed: 2026-05-14
---

# Phase 2 Plan 02: GLRenderWindow QOpenGLWidget Port Summary

**GLRenderWindow rebased from the removed-in-Qt6 QGLWidget onto QOpenGLWidget — compatibility-profile QSurfaceFormat, GL work moved into initializeGL/resizeGL/paintGL, manual buffer-swap machinery deleted, on-screen text reimplemented as a QPainter overlay, and global context sharing wired into BALLView's main().**

## Performance

- **Duration:** ~12 min
- **Started:** 2026-05-14T07:27Z
- **Completed:** 2026-05-14T07:39:41Z
- **Tasks:** 2
- **Files modified:** 3

## Accomplishments

- **Base class swap:** `GLRenderWindow` now derives from `QOpenGLWidget` instead of the Qt4-era `QGLWidget`. Header include swapped to `<QtWidgets/QOpenGLWidget>` + `<QtGui/QSurfaceFormat>`.
- **QSurfaceFormat:** the static `gl_format_` is now a `QSurfaceFormat` built by a lambda initializer requesting `CompatibilityProfile` + version 2.1 + depth 24 + stencil 8 + `DoubleBuffer` + 4× samples. The existing `BALL_OS_DARWIN` / Linux-5.7 stereo `#ifdef` guards are preserved verbatim around `setStereo(true)`. `DirectRendering` was dropped (no QSurfaceFormat equivalent; it is the default).
- **Constructors:** all three constructors updated to `QOpenGLWidget(...)` + `setFormat(gl_format_)`. The 3-arg context-sharing constructor dropped its share argument — sharing is now global via `Qt::AA_ShareOpenGLContexts`. `setAutoBufferSwap(false)` calls and the `format().rgba()` probe deleted.
- **Render path relocated:** `initializeGL()` (GLEW init + `checkGL()`), `resizeGL(w,h)` (GL viewport sync), and `paintGL()` (`ignore_events_` guard + `refresh()` texture blit) implemented as `QOpenGLWidget` overrides. The `paintEvent` override is gone; `customEvent()` now calls `update()` instead of `refresh()`+`safeBufferSwap()`.
- **Manual swap machinery deleted:** `safeBufferSwap()` removed entirely (declaration + definition) — `QOpenGLWidget` swaps automatically after `paintGL()` returns.
- **Text overlay:** `renderText(int,int,...)` reimplemented as a `QPainter` overlay on the widget (same pattern as `Scene::renderText_`). The 3-arg world-coordinate overload was already a dead early-return and is kept as a documented stub.
- **main.C:** `Qt::AA_ShareOpenGLContexts` and `QSurfaceFormat::setDefaultFormat(GLRenderWindow::gl_format_)` are now set before `QApplication` is constructed. The removed `QGLFormat::hasOpenGL()` probe was dropped.

## Task Commits

Each task was committed atomically:

1. **Task 1: Swap base class + QSurfaceFormat in glRenderWindow.h/.C; wire main.C** — `e840761` (feat)
2. **Task 2: Relocate rendering into initializeGL/resizeGL/paintGL, delete manual swap, QPainter text overlay** — `05108e9` (feat)

**Plan metadata:** see final `docs(02-02)` commit.

## Files Created/Modified

- `include/BALL/VIEW/RENDERING/glRenderWindow.h` — base class → `QOpenGLWidget`; `gl_format_` → public `QSurfaceFormat`; `initializeGL`/`resizeGL`/`paintGL` override declarations replace `paintEvent`; `safeBufferSwap()` declaration removed.
- `source/VIEW/RENDERING/glRenderWindow.C` — `QSurfaceFormat` lambda initializer; constructors on `QOpenGLWidget`; `initializeGL`/`resizeGL`/`paintGL` bodies; `customEvent()` uses `update()`; `paintEvent` + `safeBufferSwap()` deleted; `renderText` int-overload is now a `QPainter` overlay.
- `source/APPLICATIONS/BALLVIEW/main.C` — global context sharing + default surface format set before `QApplication`; removed `QGLFormat::hasOpenGL()` probe; added `<QtCore/QCoreApplication>`, `<QtGui/QSurfaceFormat>`, and the `GLRenderWindow` header.

## Verification

- **Symbol gate:** `grep -E "QGLWidget|QGLFormat|QGLContext|QGL::|QtOpenGL/qgl|swapBuffers|setAutoBufferSwap|safeBufferSwap"` over `glRenderWindow.{h,C}` returns **zero hits** (comments were reworded to avoid the legacy tokens entirely). The Wave 0 `check-no-legacy-gl-symbols.sh` gate reports no hits in `glRenderWindow.{h,C}` or `main.C`.
- **TU compile (the plan's gate for this wave):** `glRenderWindow.C.o` and `BALLView/main.C.o` both compile **clean** — only pre-existing, out-of-scope `QFlags` / `sprintf` deprecation warnings, no errors.
- **`make VIEW -j8`:** fails only in the not-yet-ported downstream TUs — this is the expected, plan-acknowledged intermediate state (see below).

## Expected Downstream Build Failures (handed to Plans 03/04)

`make VIEW -j8` does **not** fully link yet. Every remaining error is confined to files this plan does not touch, exactly as the plan and RESEARCH.md anticipated:

| File | Errors | Owner |
|------|--------|-------|
| `source/VIEW/RENDERING/glOffscreenTarget.C` | `QGLPixelBuffer` constructor (line 43); `grabFrameBuffer` not a member of `GLRenderWindow` (lines 58, 68, 79) | Plan 03/04 (RESEARCH §6 — `QGLPixelBuffer` → `QOpenGLFramebufferObject`, `grabFrameBuffer` → `grabFramebuffer`) |
| `source/VIEW/RENDERING/renderSetup.C` | `safeBufferSwap` not a member (line 196); `QGLContext*` vs `QOpenGLContext*` comparison (line 272); `grabFrameBuffer` not a member (line 415) | Plan 03 (RESEARCH §2 — `QGLContext::currentContext()` → `QOpenGLContext::currentContext()`, drop `safeBufferSwap`, `grabFrameBuffer` → `grabFramebuffer`) |

`glRenderWindow.{h,C}` and `main.C` themselves produce **no errors**. `scene.C` / `glRenderer.C` also still reference old symbols and are owned by Plans 03/04.

## Deviations from Plan

### Auto-fixed Issues

**1. [Rule 3 - Blocking] Promoted `gl_format_` from protected to public**
- **Found during:** Task 2 (surfaced when compiling `main.C`)
- **Issue:** The plan's `<interfaces>` block has `main.C` reference `GLRenderWindow::gl_format_` via `QSurfaceFormat::setDefaultFormat(...)`, but the original member was `protected` (it had only ever been used internally by the constructors). `main.C` could not compile against a protected member.
- **Fix:** Moved the `static QSurfaceFormat gl_format_;` declaration into the `public:` section of `glRenderWindow.h` with a doc comment explaining why. No behavioural change — only access widened.
- **Files modified:** `include/BALL/VIEW/RENDERING/glRenderWindow.h`
- **Commit:** `05108e9`

**2. [Rule 3 - Blocking] Dropped the `QGLFormat::hasOpenGL()` probe in `main.C`**
- **Found during:** Task 1
- **Issue:** `main.C` opened with an early `if (!QGLFormat::hasOpenGL())` OpenGL-availability check. `QGLFormat` is a legacy Qt GL symbol removed with `QGLWidget` and forbidden by the RENDER-01 grep gate; it has no static QOpenGL equivalent.
- **Fix:** Removed the probe and its `QMessageBox`, replaced with an explanatory comment. `QOpenGLWidget` negotiates the context itself and reports failure at widget-creation time, so an early static probe is neither available nor necessary.
- **Files modified:** `source/APPLICATIONS/BALLVIEW/main.C`
- **Commit:** `e840761`

**3. [Rule 1 - Cleanliness] Reworded code comments to keep the symbol grep gate clean**
- **Found during:** Task 2
- **Issue:** Several explanatory comments still contained the literal strings `QGLWidget`, `QGL::`, `swapBuffers`, `safeBufferSwap` (e.g. "`QGL::DepthBuffer`" annotations on the new `QSurfaceFormat` setters). The Wave 0 grep gate is line-based and would flag these comment lines.
- **Fix:** Reworded all such comments to describe the old API without the literal forbidden tokens (e.g. "`// was DepthBuffer`", "the old Qt4-era GL widget"). No code change.
- **Files modified:** `source/VIEW/RENDERING/glRenderWindow.C`
- **Commit:** `05108e9`

## Issues Encountered

- The CMake `Makefile` indirection meant the per-TU isolated-compile target path was not the conventional `source/VIEW/CMakeFiles/...` — the actual rule lives at `build/CMakeFiles/VIEW.dir/source/VIEW/RENDERING/glRenderWindow.C.o`. Located it via `find`; no impact on the result.

## User Setup Required

None — no external service configuration required. A clean rebuild of VIEW will be needed once Plans 03/04 land (base-class swap changes vtable layout), per the Validation strategy's per-wave sampling note.

## Known Stubs

- `GLRenderWindow::renderText(float x, float y, float z, ...)` — the 3D world-coordinate text overload. It was **already** a dead early-return (`return;`) before this port and has no callers in VIEW. Kept as a documented stub; reviving world-space text would need a manual `gluProject` + `QPainter` overlay. This is pre-existing intentional dead code, not a regression introduced by this plan.

## Next Phase Readiness

- `glRenderWindow.{h,C}` and `main.C` are fully ported and compile clean as translation units. Plans 03 and 04 can proceed against the new `QOpenGLWidget` API surface.
- Plan 03 (`renderSetup.C`): replace `QGLContext::currentContext()` → `QOpenGLContext::currentContext()`, drop the `safeBufferSwap()` call, `grabFrameBuffer` → `grabFramebuffer`. The threading scope stays minimal (A1 confirmed in Plan 01).
- Plan 04 (`scene.C` / `glOffscreenTarget.C` / `glRenderer.C`): rework `QGLPixelBuffer` → `QOpenGLFramebufferObject`, the stereo capability probe, the `swapBuffers` sync block, and `convertToGLFormat`.
- RENDER-02/04/05/06/08 visual confirmation remains a human gate at the end of Plan 04 — not verifiable in this wave.

## Self-Check: PASSED

- FOUND: `include/BALL/VIEW/RENDERING/glRenderWindow.h` (modified, `public QOpenGLWidget`)
- FOUND: `source/VIEW/RENDERING/glRenderWindow.C` (modified, `paintGL`/`initializeGL`/`resizeGL` defined)
- FOUND: `source/APPLICATIONS/BALLVIEW/main.C` (modified, `AA_ShareOpenGLContexts` + `setDefaultFormat`)
- FOUND: commit `e840761` (Task 1)
- FOUND: commit `05108e9` (Task 2)
- VERIFIED: `glRenderWindow.C.o` and `BALLView/main.C.o` build with zero errors
- VERIFIED: zero legacy Qt GL symbols in `glRenderWindow.{h,C}`

---
*Phase: 02-rendering-port-4a*
*Completed: 2026-05-14*
