---
phase: 02-rendering-port-4a
plan: 03
subsystem: rendering
tags: [opengl, qt5, qopenglcontext, qopenglframebufferobject, grabframebuffer, threading]

# Dependency graph
requires:
  - phase: 02-rendering-port-4a
    provides: "GLRenderWindow rebased on QOpenGLWidget (Plan 02); A1 confirmed (raytracer worker issues no GL)"
provides:
  - "renderSetup.C consumes the QOpenGLWidget API: QOpenGLContext::currentContext(), grabFramebuffer(), update()-driven repaint"
  - "RenderSetup::makeCurrent() is a documented GUI-thread-only helper; the raytracer worker loop touches no widget GL context"
  - "glRenderer.C label-texture upload uses QImage::convertToFormat(Format_RGBA8888).mirrored() instead of the removed convertToGLFormat"
  - "GLOffscreenTarget is QOpenGLFramebufferObject-backed (always-FBO path), no QGLPixelBuffer"
affects: [02-04]

# Tech tracking
tech-stack:
  added: []
  patterns:
    - "GUI-thread-only makeCurrent helper: worker QThread never touches the QOpenGLWidget GL context (raytracer is CPU-only)"
    - "QOpenGLFramebufferObject offscreen target bound against the shared GLRenderWindow context"
    - "QImage::convertToFormat(Format_RGBA8888).mirrored() replaces QGLWidget::convertToGLFormat (byte-order + vertical flip)"

key-files:
  created: []
  modified:
    - source/VIEW/RENDERING/renderSetup.C
    - source/VIEW/RENDERING/RENDERERS/glRenderer.C
    - source/VIEW/RENDERING/glOffscreenTarget.C
    - include/BALL/VIEW/RENDERING/glOffscreenTarget.h

key-decisions:
  - "renderSetup.h needed no changes — the QImage forward-decl and QThread base are API-stable; all migration was in the .C file"
  - "RenderSetup::resize()'s removed safeBufferSwap() call replaced with gl_target_->update() (Rule 3 — blocking: symbol deleted in Plan 02); QOpenGLWidget auto-swaps after paintGL()"
  - "GLOffscreenTarget::tryUsePixelBuffer() kept as a no-op rather than deleted — preserves the public API; the offscreen target is now unconditionally FBO-backed"
  - "All 13 RenderSetup::makeCurrent() call sites kept — every one is on a GUI-thread path; the worker loop run() never calls makeCurrent() (A1 confirmed)"

requirements-completed: [RENDER-04, RENDER-07]

# Metrics
duration: 9min
completed: 2026-05-14
---

# Phase 2 Plan 03: Renderer-Side QOpenGLWidget API Consumers Summary

**renderSetup.C, glRenderer.C, and glOffscreenTarget.{h,C} cleaned of all legacy Qt GL symbols and compiled clean against the QOpenGLWidget base — QOpenGLContext/grabFramebuffer in renderSetup, Format_RGBA8888+mirrored() label upload in glRenderer, and a QOpenGLFramebufferObject-backed GLOffscreenTarget. scene.C is the sole remaining VIEW build failure, owned by Plan 04.**

## Performance

- **Duration:** ~9 min
- **Tasks:** 2
- **Files modified:** 4

## Accomplishments

### Task 1 — renderSetup.{h,C}

- `QGLContext::currentContext()` → `QOpenGLContext::currentContext()` (renderSetup.C:272); added `#include <QtGui/QOpenGLContext>`.
- `RenderSetup::makeCurrent()` rewritten to the GUI-thread-only form and documented as such: it compares `QOpenGLContext::currentContext()` against `gl_target_->context()` and only ever runs on the GUI thread.
- `gl_target_->grabFrameBuffer(true)` → `gl_target_->grabFramebuffer()` (no-arg form, renderSetup.C:415).
- The `RenderSetup::resize()` path still referenced `safeBufferSwap()` — a symbol deleted by Plan 02. Replaced the `gl_target_->safeBufferSwap()` call with `gl_target_->update()` (QOpenGLWidget auto-swaps after `paintGL()`; a repaint request is the correct replacement).
- Added a clarifying comment at the `run()` worker loop noting the loop drives only non-GL renderers and must never touch the GUI-thread-affine GL context.
- `renderSetup.h` required **no changes** — `class QImage;` forward-decl and `RenderSetup : public QThread` are API-stable.

### makeCurrent() call-site audit (kept vs removed)

All 13 `RenderSetup::makeCurrent()` call sites were audited per the plan. **Every one was kept** — each is on a GUI-thread-driven path, and none is reachable from the `run()` worker loop:

| Line | Method | Path | Disposition |
|------|--------|------|-------------|
| 130 | `init()` | GUI-thread setup | KEPT |
| 165 | `resize()` | GUI-thread resize event | KEPT |
| 414 | `exportPNG()` | GUI-thread export action | KEPT |
| 485 | `bufferRepresentation()` | GUI-thread representation update | KEPT |
| 499 | `removeRepresentation()` | GUI-thread representation update | KEPT |
| 511 | `setLights()` | GUI-thread lighting setup | KEPT |
| 532 | `updateBackgroundColor()` | GUI-thread setup | KEPT |
| 543 | `setupEnvironmentMap()` | GUI-thread setup | KEPT |
| 558 | `prepareGridTextures()` | GUI-thread texture setup | KEPT |
| 574 | `removeGridTextures()` | GUI-thread texture setup | KEPT |
| 583 | `mapViewportTo3D()` | GUI-thread picking helper | KEPT |
| 591 | `map3DToViewport()` | GUI-thread picking helper | KEPT |
| 601 | `pickObjects()` | GUI-thread picking | KEPT |

`RenderSetup::run()` (the worker `QThread` loop) calls only `renderToBuffer_()` for `!gl_renderer_`, and the raytracer's `renderToBuffer_()` branch (`BufferedRenderer::renderToBuffer`) issues no GL — A1 CONFIRMED in `02-A1-FINDINGS.md`. Therefore **no `makeCurrent()` call needed to be removed**: the worker loop already never reaches one. The change is purely making the helper GUI-thread-correct (`QOpenGLContext`) and documenting the invariant.

### Task 2 — glRenderer.C + glOffscreenTarget.{h,C}

- **glRenderer.C (~1155):** `QImage gldata = QGLWidget::convertToGLFormat(pm);` → `QImage gldata = pm.convertToFormat(QImage::Format_RGBA8888).mirrored();`. This reproduces both jobs the removed helper did: GL-native byte order (`Format_RGBA8888` on little-endian, all 3 targets) and the vertical flip (`.mirrored()`). The following `glDrawPixels(...)` call is unchanged. No dangling `QtOpenGL`/`QGLWidget` include — glRenderer.C never included one.
- **glOffscreenTarget.h:** forward-decl `class QGLPixelBuffer;` → `class QOpenGLFramebufferObject;`; member `boost::shared_ptr<QGLPixelBuffer> pixel_buffer_` → `boost::shared_ptr<QOpenGLFramebufferObject> fbo_`; the now-unused `bool use_pixel_buffer_` member removed.
- **glOffscreenTarget.C:** `#include <QtOpenGL/QGLPixelBuffer>` → `#include <QtGui/QOpenGLFramebufferObject>`. The `QGLPixelBuffer::hasOpenGLPbuffers()` capability gate is gone — always-FBO path. `resize()` makes the shared context current and constructs a `QOpenGLFramebufferObject(width, height, CombinedDepthStencil)`. `prepareRendering()` does `share_from_->makeCurrent()` then `fbo_->bind()`. `getImage()`/`prepareUpscaling()`/`updateImageTile()` use `fbo_->toImage()`, with `share_from_->grabFramebuffer()` (renamed from `grabFrameBuffer`) as the fallback. `tryUsePixelBuffer()` kept as a documented no-op for API stability.

## Task Commits

1. **Task 1: Port renderSetup.C — QOpenGLContext, grabFramebuffer, GUI-thread-only makeCurrent** — `9b090f6` (feat)
2. **Task 2: Port glRenderer convertToGLFormat + glOffscreenTarget to QOpenGLFramebufferObject** — `32f8005` (feat)

**Plan metadata:** see final `docs(02-03)` commit.

## Files Created/Modified

- `source/VIEW/RENDERING/renderSetup.C` — `QOpenGLContext::currentContext()`, GUI-thread-only `makeCurrent()` with documentation, `grabFramebuffer()`, `safeBufferSwap()`→`update()`, `<QtGui/QOpenGLContext>` include, worker-loop comment.
- `source/VIEW/RENDERING/RENDERERS/glRenderer.C` — label-texture upload uses `convertToFormat(Format_RGBA8888).mirrored()`.
- `source/VIEW/RENDERING/glOffscreenTarget.C` — `QOpenGLFramebufferObject` replaces `QGLPixelBuffer`; always-FBO path; `grabFramebuffer()`.
- `include/BALL/VIEW/RENDERING/glOffscreenTarget.h` — `QOpenGLFramebufferObject` forward-decl + member; removed `use_pixel_buffer_`.

## Verification

- **Symbol gate (this plan's files):** `grep -E "QGLPixelBuffer|QGLWidget|QGLFormat|QGLContext|QGL::|convertToGLFormat|grabFrameBuffer|QtOpenGL/qgl"` over `renderSetup.{C,h}`, `glRenderer.C`, `glOffscreenTarget.{C,h}` returns **zero hits** (two comment lines that initially tripped the line-based gate were reworded, per the precedent set in Plan 02).
- `grep -c QOpenGLContext renderSetup.C` = 2; `grep -c grabFramebuffer renderSetup.C` = 1; `grep -c grabFrameBuffer renderSetup.C` = 0.
- `grep -c Format_RGBA8888 glRenderer.C` = 2, with `.mirrored()` on the same conversion.
- `grep -c QOpenGLFramebufferObject glOffscreenTarget.C` = 4.
- **`make VIEW -j8`:** `renderSetup.C.o`, `glRenderer.C.o`, `glOffscreenTarget.C.o` all compile clean (object files present in `build/CMakeFiles/VIEW.dir/...`, only pre-existing out-of-scope `std::move`/`sprintf` deprecation warnings). The build log is at `/tmp/02-03-build.log`.

## Expected Downstream Build Failure (handed to Plan 04)

`make VIEW -j8` does not fully link. **The sole remaining compile failure is `source/VIEW/WIDGETS/scene.C`** — exactly the expected, plan-acknowledged state for this wave:

| File | Errors | Owner |
|------|--------|-------|
| `source/VIEW/WIDGETS/scene.C` | `safeBufferSwap` not a member (line 1651); `swapBuffers` not a member (line 1666); `grabFrameBuffer` not a member (line 2096); `QGLFormat`/`QGLWidget`/`QGL::` stereo-capability probe (lines 3065-3079); `#include <QtOpenGL/QGLPixelBuffer>` (line 86) | Plan 04 (RESEARCH §4) |

No other VIEW translation unit fails. This plan's three source files are clean.

## Deviations from Plan

### Auto-fixed Issues

**1. [Rule 3 - Blocking] Replaced the removed `safeBufferSwap()` call in `RenderSetup::resize()`**
- **Found during:** Task 1
- **Issue:** `RenderSetup::resize()` (renderSetup.C:196) called `gl_target_->safeBufferSwap()`. Plan 02 deleted `safeBufferSwap()` from `GLRenderWindow` entirely. The plan's `<interfaces>` block did not call out this specific call site (it focused on `QGLContext`, `makeCurrent`, `grabFramebuffer`), but the Plan 02 SUMMARY explicitly handed `safeBufferSwap not a member (line 196)` to Plan 03. The file would not compile without addressing it.
- **Fix:** Replaced `gl_target_->safeBufferSwap()` with `gl_target_->update()`. `QOpenGLWidget` swaps automatically after `paintGL()`; the post-resize manual swap is correctly expressed as a repaint request. Also dropped the now-redundant `render_mutex_` lock/unlock that wrapped only that one call.
- **Files modified:** `source/VIEW/RENDERING/renderSetup.C`
- **Commit:** `9b090f6`

**2. [Rule 1 - Cleanliness] Reworded two code comments to keep the symbol grep gate clean**
- **Found during:** Task 2
- **Issue:** Explanatory comments added in Task 2 contained the literal strings `QGLWidget` and `QGLPixelBuffer` (describing the old APIs being replaced). The Wave 0 `check-no-legacy-gl-symbols.sh` gate is line-based and flagged them.
- **Fix:** Reworded both comments to describe the removed APIs without the literal forbidden tokens ("the legacy Qt4-era GL-widget format helper", "the legacy Qt4-era pixel-buffer class"). No code change. Same approach as Plan 02 deviation #3.
- **Files modified:** `source/VIEW/RENDERING/RENDERERS/glRenderer.C`, `source/VIEW/RENDERING/glOffscreenTarget.C`
- **Commit:** `32f8005`

### A5 FBO Defer

**None.** `GLOffscreenTarget` was fully reworked to `QOpenGLFramebufferObject` with no deferred path. The discretionary A5 concern (FBO-vs-pbuffer semantics blocking the tiling-export path) did not materialize: the tiling path uses `prepareUpscaling()`/`updateImageTile()`, which read images via `fbo_->toImage()` / `grabFramebuffer()` and composite through a redirected `QPainter` — no pbuffer-specific semantics were relied on. The full visual confirmation of the tiling/offscreen export path remains a human gate (RENDER-08 territory), but the structural rework is complete and compiles.

## Issues Encountered

- The `check-no-legacy-gl-symbols.sh` gate is line-based, so explanatory comments mentioning the old API names tripped it (same as Plan 02). Resolved by rewording — no functional impact.

## Out-of-Scope Discovery (logged, not fixed)

- `include/BALL/VIEW/RENDERING/glDisplayList.h:13` still has `#include <QtOpenGL/qgl.h>`. This file is **not** in any 02-rendering-port-4a plan's `files_modified` list. It compiles fine on Qt 5.15 (`qgl.h` still exists) — it is a Qt 6 blocker, not a Qt 5 build blocker. Logged to `.planning/phases/02-rendering-port-4a/deferred-items.md`; recommend folding into Phase 5 (Qt 6 migration) or a dedicated follow-up task. Not fixed here per scope boundary.

## User Setup Required

None. A clean rebuild of VIEW will be needed once Plan 04 lands, per the Validation strategy's per-wave sampling note.

## Known Stubs

- `GLOffscreenTarget::tryUsePixelBuffer(bool)` is now a documented no-op. The offscreen target is unconditionally FBO-backed; there is no longer a pixel-buffer-vs-window choice to make. The method is retained only so the public API surface is unchanged for any external caller (none found in `source/VIEW` / `include/BALL/VIEW`). This is intentional API-stability scaffolding, not unwired functionality — it does not block the plan's goal.

## Next Phase Readiness

- `renderSetup.{C,h}`, `glRenderer.C`, `glOffscreenTarget.{h,C}` are fully ported and compile clean as translation units against the `QOpenGLWidget` API.
- Plan 04 (`scene.C`) is the last file: it must port the `QGLFormat`/`QGLWidget` stereo-capability probe (lines 3063-3082), drop the `swapBuffers`/`safeBufferSwap` manual-sync block (lines 1644-1672), rename `grabFrameBuffer` → `grabFramebuffer` (line 2096), and resolve the `<QtOpenGL/QGLPixelBuffer>` include (line 86). After Plan 04, `make VIEW` should link fully.
- RENDER-02/04/05/06/08 visual confirmation remains a human gate at the end of Plan 04.

## Self-Check: PASSED

- FOUND: `source/VIEW/RENDERING/renderSetup.C` (modified — `QOpenGLContext::currentContext()`, `grabFramebuffer()`)
- FOUND: `source/VIEW/RENDERING/RENDERERS/glRenderer.C` (modified — `Format_RGBA8888` + `.mirrored()`)
- FOUND: `source/VIEW/RENDERING/glOffscreenTarget.C` (modified — `QOpenGLFramebufferObject`)
- FOUND: `include/BALL/VIEW/RENDERING/glOffscreenTarget.h` (modified — `QOpenGLFramebufferObject` member)
- FOUND: commit `9b090f6` (Task 1)
- FOUND: commit `32f8005` (Task 2)
- VERIFIED: `renderSetup.C.o`, `glRenderer.C.o`, `glOffscreenTarget.C.o` build with zero errors
- VERIFIED: zero legacy Qt GL symbols in the four files this plan touches; remaining gate hits are confined to `scene.C` (Plan 04) and `glDisplayList.h` (logged as out-of-scope)

---
*Phase: 02-rendering-port-4a*
*Completed: 2026-05-14*
