# Phase 2: Rendering Port (4a) - Research

**Researched:** 2026-05-14
**Domain:** Qt OpenGL widget migration (`QGLWidget` → `QOpenGLWidget`), threaded GL rendering, fixed-function compatibility profiles
**Confidence:** HIGH (codebase facts verified by direct read; Qt API facts cited from official 5.15 docs)

## Summary

BALLView's `GLRenderWindow` multiply-inherits from `RenderWindow` (a CPU-pixel-buffer
base) and the Qt4-era `QGLWidget`. On macOS Tahoe the Cocoa plugin force-enables
layer-backing, which `QGLWidget` cannot satisfy, so the scene is promoted to a
detached 100×30 native window and never renders inside the main window. The fix —
already decided and locked — is to rebase `GLRenderWindow` on `QOpenGLWidget`, which
is layer-backed and the only supported embedded-GL path on modern macOS. This also
removes a hard Qt 6 blocker (`QGLWidget`/`QGLContext`/`QGLFormat` are deleted in Qt 6).

The migration is mechanically well-bounded. The codebase touches a small, knowable
set of `QGLWidget`-era APIs across five files: `glRenderWindow.{h,C}` (the base class
swap, `QGLFormat`→`QSurfaceFormat`, `setAutoBufferSwap`/`swapBuffers` removal,
`renderText` removal, `paintEvent`→`paintGL`), `renderSetup.C` (`QGLContext::currentContext()`,
worker-thread `makeCurrent()`), `glOffscreenTarget.C` (`QGLPixelBuffer`,
`grabFrameBuffer`), `glRenderer.C` (one `QGLWidget::convertToGLFormat` call), and
`scene.C` (a `QGLFormat`/`QGLWidget` capability probe, `grabFrameBuffer`, two stereo/
multi-display `new GLRenderWindow(0, …, Qt::Window)` top-level paths).

The genuinely hard part is threading. `RenderSetup` is a `QThread` and worker threads
currently call `makeCurrent()` directly on the widget. `QOpenGLWidget`'s default
framebuffer is owned by, and only valid on, the GUI thread — a worker thread cannot
`makeCurrent()` it. The good news: the codebase **already** runs the interactive
`GLRenderer` on the GUI thread (`renderSetup.C:308` "GLRenderers currently *have* to
render in the GUI thread", `scene.C:1598` confirms `renderToBuffer()` is called from
the GUI-thread event handler). The raytracer already produces a CPU pixel buffer
blitted as a texture. So the hybrid model is mostly *already in place* — the work is
removing the now-invalid worker-thread `makeCurrent()` calls and the manual
`swapBuffers()` synchronisation, not rearchitecting the renderer.

**Primary recommendation:** Rebase `GLRenderWindow` on `QOpenGLWidget` with a
`QSurfaceFormat` requesting `CompatibilityProfile` + depth/stencil/double buffer +
4× samples; move all GL-context-current operations onto the GUI thread inside
`initializeGL()`/`paintGL()`/`resizeGL()`; delete manual buffer-swap code (QOpenGLWidget
swaps automatically); reimplement on-screen text as a `QPainter` overlay; treat the
two top-level stereo `Qt::Window` `GLRenderWindow` paths as the highest-risk item and
verify/stub them explicitly.

## User Constraints (from PROJECT.md — no CONTEXT.md exists yet)

> No `02-CONTEXT.md` exists. These constraints are lifted from PROJECT.md "Key Decisions"
> and "Constraints", which are authoritative for this phase. Treat as locked.

### Locked Decisions

- Port `GLRenderWindow` base from `QGLWidget` → `QOpenGLWidget`. Keep Qt 5.15. Qt 6 is Phase 4b (GSD Phase 5), explicitly out of scope here.
- `QGLFormat` → `QSurfaceFormat` requesting a **compatibility profile** — the ~100 fixed-function GL calls in `glRenderer.C` (`glBegin`/`glVertex`/`glMatrixMode`/`gluLookAt`) must keep working. Full programmable-pipeline rewrite is deferred to v2 (PIPE-01).
- **Hybrid threading:** interactive `GLRenderer` renders on the GUI thread in `paintGL()`; the raytracer stays a CPU-buffer worker thread whose buffer is blitted as a texture.
- Remove manual `swapBuffers()` / `setAutoBufferSwap` — `QOpenGLWidget` swaps automatically.
- `QGLWidget::renderText` is gone in `QOpenGLWidget` — reimplement on-screen text via a `QPainter` overlay.
- Platform independence is a **hard requirement** — no per-OS graphics code. Must build and render on macOS (Apple Silicon), Linux, Windows.
- Threading rework must be **incremental and verifiable against a running GUI** (PROJECT.md "Risk").

### Claude's Discretion

- Exact `QSurfaceFormat` field values (sample count, swap behaviour, GL version request).
- Whether the multi-display / stereo top-level `GLRenderWindow` paths are fully ported, deferred behind a runtime guard, or stubbed for this phase (see Pitfall 6 — recommend defer-with-guard).
- The internal mechanism for the `QPainter` text overlay (paint inside `paintGL()` vs. a sibling overlay widget).
- Whether to keep `GLOffscreenTarget`'s `QGLPixelBuffer` path or collapse it onto a `QOpenGLFramebufferObject` (recommend the latter; `QGLPixelBuffer` is also Qt6-removed).

### Deferred Ideas (OUT OF SCOPE)

- Programmable-pipeline / GL core-profile rewrite of `glRenderer.C` (v2, PIPE-01; Phase 4b decides direction).
- Qt 6 migration proper (`QT6-01`/`QT6-02`) — Phase 5.
- `QRegExp`/`QDesktopWidget` deprecation cleanup — Phase 5.
- Reviving RTfact (`BALL_HAS_RTFACT` paths) — out of scope project-wide.

## Phase Requirements

| ID | Description | Research Support |
|----|-------------|------------------|
| RENDER-01 | `GLRenderWindow` derives from `QOpenGLWidget`; `QGLFormat`→`QSurfaceFormat` (compat profile + depth/stencil/double buffer) | API Migration Map §1; `QSurfaceFormat` §3; current `gl_format_` at `glRenderWindow.C:26-40` maps field-by-field |
| RENDER-02 | BALLView shows a molecule embedded in the main window on macOS (not a detached native window) | Root cause already diagnosed; `QOpenGLWidget` is layer-backed → composites as a child. Verified by Qt docs: QOpenGLWidget is the supported embedded path |
| RENDER-03 | Interactive GL renderer runs on GUI thread via `paintGL()`; manual `swapBuffers`/`setAutoBufferSwap` removed | Threading §2 — codebase **already** GUI-thread-renders the GLRenderer (`renderSetup.C:308-310`, `scene.C:1597-1600`); work is removing dead swap code |
| RENDER-04 | Raytracer keeps working via CPU pixel-buffer path, blitted as a texture | `GLRenderWindow::refresh()` (`glRenderWindow.C:146-226`) already does texture-blit-of-CPU-buffer; this path is preserved, only the context-current call moves |
| RENDER-05 | On-screen text (was `QGLWidget::renderText`) reimplemented as `QPainter` overlay | §4 QPainter overlay pattern; note `scene.C:506` `renderText_()` **already** uses `QPainter` on a `QPaintDevice` — only `glRenderWindow.C:228-260` needs porting |
| RENDER-06 | Mouse/keyboard interaction (rotate/zoom/pick/select) works in ported widget | `QOpenGLWidget` is a `QWidget` — event handling unchanged; risk is event-filter wiring in `scene.C:563-599` and the `paintEvent` override |
| RENDER-07 | `RenderSetup`, `scene.C`, `glOffscreenTarget.C`, `glRenderer.C` cleaned of all `QGLWidget`/`QGLContext`/`QGLFormat` refs, compile clean | Full reference inventory in API Migration Map + Runtime State Inventory |
| RENDER-08 | Builds, launches, renders on Linux and Windows; no regressions vs. macOS | §6 CMake (`Qt5::OpenGL`→ verify; `QOpenGLWidget` lives in `Qt5::Widgets`); Validation Architecture notes human-on-3-platforms gate |

## Standard Stack

This phase adds **no new third-party libraries** — it migrates within Qt 5.15 (already
a dependency via Homebrew `qt@5` 5.15.18). The "stack" here is the set of Qt classes
replacing the removed ones.

### Core (Qt 5.15 classes)

| Class | Module | Replaces | Purpose |
|-------|--------|----------|---------|
| `QOpenGLWidget` | `Qt5::Widgets` (header `<QtWidgets/QOpenGLWidget>`) | `QGLWidget` | Embeddable, layer-backed GL widget. `[CITED: doc.qt.io/archives/qt-5.15/qopenglwidget.html]` |
| `QSurfaceFormat` | `Qt5::Gui` | `QGLFormat` | GL context format request (profile, buffers, samples). `[CITED: doc.qt.io/qt-5/qsurfaceformat.html]` |
| `QOpenGLContext` | `Qt5::Gui` | `QGLContext` | Context handle; `QOpenGLContext::currentContext()` replaces `QGLContext::currentContext()`. `[CITED: doc.qt.io/qt-5/qopenglcontext.html]` |
| `QOpenGLFramebufferObject` | `Qt5::Gui` | `QGLPixelBuffer` / `QGLFramebufferObject` | Offscreen render target for `GLOffscreenTarget` if reworked. `[ASSUMED]` for the rework path |
| `QPainter` | `Qt5::Gui` | `QGLWidget::renderText` | 2D text/overlay on the GL widget. `[CITED: doc.qt.io/archives/qt-5.15/qopenglwidget.html]` |
| `QImage::mirrored()` / `QImage` ctor | `Qt5::Gui` | `QGLWidget::convertToGLFormat` | Static helper removed; replicate with `QImage` format conversion + vertical flip. `[ASSUMED]` — see Pitfall 7 |

### Supporting

| Class | Module | Purpose | When to Use |
|-------|--------|---------|-------------|
| `QOffscreenSurface` | `Qt5::Gui` | A surface a non-GUI thread can `makeCurrent()` a *shared* context onto | Only if a worker thread genuinely needs its own GL context (the raytracer does **not** — it's CPU-only) `[CITED: doc.qt.io/qt-5/qoffscreensurface.html]` |
| `QOpenGLWindow` | `Qt5::Gui` | Top-level GL window (not a `QWidget` child) | Candidate for the stereo/multi-display `Qt::Window` paths if they must remain top-level — see Pitfall 6 `[ASSUMED]` |

### Alternatives Considered

| Instead of | Could Use | Tradeoff |
|------------|-----------|----------|
| `QOpenGLWidget` | `QOpenGLWindow` + `QWidget::createWindowContainer` | More control over the native window, but heavier and unnecessary for an embedded child widget; `QOpenGLWidget` is the documented embedded path |
| `QSurfaceFormat::CompatibilityProfile` | `CoreProfile` | Core profile would require the full `glRenderer.C` pipeline rewrite — explicitly deferred to v2. Compat profile is the locked decision |
| Reworking `GLOffscreenTarget` to `QOpenGLFramebufferObject` | Keep `QGLPixelBuffer` | `QGLPixelBuffer` still exists in Qt 5.15 but is removed in Qt 6; reworking now reduces Phase 5 debt. Discretionary |

**Installation:** None — Qt 5.15 already installed via `brew install qt@5`.

**Version verification:** `qt@5` Homebrew formula is 5.15.18 (`[VERIFIED: ROADMAP-1.6.md dependency table]`). `QOpenGLWidget` has existed since Qt 5.4 and is fully stable in 5.15. `[CITED: doc.qt.io/archives/qt-5.15/qopenglwidget.html]`

## API Migration Map

The authoritative, file-by-file mapping. All "current" facts `[VERIFIED]` by direct
read of the source files; all "Qt 5.15 replacement" facts `[CITED]` to Qt 5.15 docs.

### §1 `glRenderWindow.h` / `glRenderWindow.C` — the base class

| Current (`QGLWidget`-era) | Location | Qt 5.15 `QOpenGLWidget` replacement |
|---------------------------|----------|--------------------------------------|
| `#include <QtOpenGL/qgl.h>` | `glRenderWindow.h:28` | `#include <QtWidgets/QOpenGLWidget>` + `#include <QtGui/QSurfaceFormat>` |
| `class GLRenderWindow : public RenderWindow, public QGLWidget` | `glRenderWindow.h:37-39` | `… public QOpenGLWidget` (multiple inheritance pattern unchanged) |
| `static QGLFormat gl_format_;` | `glRenderWindow.h:97` | `static QSurfaceFormat gl_format_;` |
| `QGLFormat GLRenderWindow::gl_format_(QGL::DepthBuffer \| QGL::StereoBuffers \| QGL::DoubleBuffer \| QGL::DirectRendering \| QGL::SampleBuffers \| QGL::StencilBuffer)` | `glRenderWindow.C:26-40` | See §3 — field-by-field `QSurfaceFormat` setters |
| `QGLWidget(gl_format_)` ctor | `glRenderWindow.C:44` | `QOpenGLWidget()` then `setFormat(gl_format_)` **before** the widget is shown. `QOpenGLWidget` has no format-taking ctor. `[CITED]` |
| `QGLWidget(gl_format_, parent_widget, nullptr, w_flags)` | `glRenderWindow.C:60` | `QOpenGLWidget(parent_widget, w_flags)` + `setFormat(gl_format_)` |
| `QGLWidget(gl_format_, parent_widget, &window, w_flags)` (context-sharing ctor) | `glRenderWindow.C:80` | No share-ctor. Global context sharing via `QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts)` set **before** `QApplication` is constructed. See Pitfall 8 |
| `QGLWidget::isValid()` | `glRenderWindow.C:70` | `QOpenGLWidget::isValid()` exists — but only meaningful after `initializeGL()`. Move the check |
| `setAutoBufferSwap(false)` | `glRenderWindow.C:55,75,91` | **Delete** — `QOpenGLWidget` always swaps automatically after `paintGL()` `[CITED]` |
| `setAutoFillBackground(false)` | `glRenderWindow.C:56,76,92` | Keep — still valid on `QWidget` |
| `format().rgba()` | `glRenderWindow.C:105` | Delete — `QSurfaceFormat` has no `rgba()`; RGBA is always implied |
| `QGLWidget::resize(width,height)` | `glRenderWindow.C:141` | `QOpenGLWidget::resize(width,height)` — unchanged (`QWidget` method) |
| `paintEvent(QPaintEvent*)` override + `QGLWidget::paintEvent(e)` | `glRenderWindow.h:96`, `glRenderWindow.C:346-352` | Replace with `paintGL()` override. `QOpenGLWidget` calls `paintGL()` internally; do **not** override `paintEvent`. The `ignore_events_` guard moves into `paintGL()` |
| `customEvent()` → `refresh(); safeBufferSwap();` | `glRenderWindow.C:334-344` | `refresh()` work moves into `paintGL()`; drop `safeBufferSwap()`. Trigger a repaint with `update()` instead of posting/swapping manually |
| `makeCurrent()` in `lockGLContext()` | `glRenderWindow.C:357` | `QOpenGLWidget::makeCurrent()` exists **but is GUI-thread-only**. See Threading §2 — `lockGLContext`/`unlockGLContext` semantics must change |
| `doneCurrent()` in `unlockGLContext()` | `glRenderWindow.C:362` | `QOpenGLWidget::doneCurrent()` exists, same GUI-thread constraint |
| `swapBuffers()` in `safeBufferSwap()` | `glRenderWindow.C:381` | **Delete** `safeBufferSwap()` entirely — no manual swap exists on `QOpenGLWidget` |
| `getMainControl()->windowHandle()->isExposed()` | `glRenderWindow.C:379` | Drop with `safeBufferSwap()` |
| `QGLWidget::renderText(x, y, text, font)` | `glRenderWindow.C:244` | Removed. Reimplement via `QPainter` overlay — see §4 |
| `QGLWidget::renderText(x, y, z, text, font)` (3D-coord overload) | `glRenderWindow.C:258` | Already a no-op (`return;` at `glRenderWindow.C:251`). Reimplement as a stub or with manual `gluProject` + `QPainter`; low priority since it's dead |
| New: nothing | — | Add `initializeGL()` override — call GLEW init / `checkGL()` / texture setup here (currently scattered into `init()`) |

**Note on `init()` / `resize()` / `refresh()`:** these are `RenderWindow` virtual methods,
*not* Qt overrides. They are called by `RenderSetup` from the GUI-thread event handler
(`scene.C:1597-1602`). Their GL calls are valid **only if** the `QOpenGLWidget` context
is current. After the port, the natural place for the actual GL work (`refresh()`'s
texture blit) is inside `paintGL()`, where `QOpenGLWidget` guarantees the context and
the correct default FBO are bound.

### §2 `renderSetup.C` / `renderSetup.h`

| Current | Location | Replacement |
|---------|----------|-------------|
| `QGLContext::currentContext() != gl_target_->context()` | `renderSetup.C:272` | `QOpenGLContext::currentContext() != gl_target_->context()` — `QOpenGLWidget::context()` returns `QOpenGLContext*` `[CITED]` |
| `gl_target_->makeCurrent()` (from `RenderSetup::makeCurrent()`) | `renderSetup.C:273` | **GUI-thread only.** For the GLRenderer this is fine — it already runs GUI-thread. For the raytracer worker, this call must be **removed** — the raytracer is CPU-only and does not need a GL context (verify: `BufferedRenderer::renderToBuffer` writes `m_pixels`, no GL). See Threading §2 |
| `makeCurrent()` calls at lines 130, 165, 414, 485, 499, 511, 532, 543, 558, 574, 583, 591, 601 | `renderSetup.C` | Audit each: those reached only on the GUI thread (init, resize, exportPNG, grid/texture setup driven from GUI) stay; any reached from `RenderSetup::run()`'s worker loop must go. `run()` (`renderSetup.C:278-335`) only calls `renderToBuffer_()` for `!gl_renderer_` — and `renderToBuffer_()` for the raytracer path does no GL. So in practice the worker thread never needs `makeCurrent()` |
| `gl_target_->grabFrameBuffer(true)` | `renderSetup.C:415` | `QOpenGLWidget::grabFramebuffer()` — note: **no argument**, returns `QImage`, must be called on GUI thread with context current `[CITED]` |
| `RenderSetup : public QThread` | `renderSetup.h:45` | Unchanged — the raytracer still uses the thread. The thread just never touches GL |

### §3 `QGLFormat` → `QSurfaceFormat` field-by-field

Current `gl_format_` (`glRenderWindow.C:26-40`) and the macOS/Linux `#ifdef` guards
map as follows. `[CITED: doc.qt.io/qt-5/qsurfaceformat.html]`

| `QGLFormat` flag | `QSurfaceFormat` equivalent |
|------------------|------------------------------|
| `QGL::DepthBuffer` | `fmt.setDepthBufferSize(24);` (a nonzero size requests a depth buffer) |
| `QGL::StencilBuffer` | `fmt.setStencilBufferSize(8);` |
| `QGL::DoubleBuffer` | `fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);` |
| `QGL::SampleBuffers` | `fmt.setSamples(4);` (multisample AA) |
| `QGL::StereoBuffers` | `fmt.setStereo(true);` — keep the existing `#ifdef BALL_OS_DARWIN` / Linux-5.7 guards verbatim; quad-buffered stereo support is driver-dependent and unchanged by this port |
| `QGL::DirectRendering` | **No equivalent** — `QSurfaceFormat` has no direct-rendering flag; it is the default behaviour. Drop it |
| (implicit) compatibility profile | `fmt.setProfile(QSurfaceFormat::CompatibilityProfile);` — **the key addition**. Also `fmt.setVersion(2, 1);` to explicitly request GL 2.1 on macOS (see §below) |

`QGLWidget` historically gave a 2.1 compatibility context by default; `QOpenGLWidget`
does **not** — it must be requested explicitly or you may get a 3.2+ core context on
macOS, which breaks every `glBegin`/`glMatrixMode` call in `glRenderer.C`.

**macOS specifics `[CITED: doc.qt.io/qt-5/qsurfaceformat.html + Apple OpenGL docs]`:**
macOS exposes exactly two profile families: a **2.1 legacy/compatibility** context, or
**3.2 / 4.1 core** contexts. There is no 3.x/4.x *compatibility* profile on macOS. To
keep BALL's fixed-function pipeline, request **`CompatibilityProfile` + version 2.1**
— this is the profile that supports `glBegin`, `glMatrixMode`, `gluLookAt`, display
lists, `glDrawPixels`, `GL_TEXTURE_ENV`, etc. On Linux/Windows a compatibility profile
of a higher version also works, but requesting 2.1 compat is the safe lowest common
denominator that the existing renderer was written against. Set it once in `gl_format_`
and via `QSurfaceFormat::setDefaultFormat(gl_format_)` early in `main()` so child
contexts and shared contexts inherit it.

### §4 `scene.C`

| Current | Location | Replacement |
|---------|----------|-------------|
| `#include <QtOpenGL/QGLPixelBuffer>` | `scene.C:86` | Drop if `GLOffscreenTarget` rework removes pbuffers; otherwise `<QtGui/QOpenGLFramebufferObject>` |
| `QGLFormat test_format(QGL::DepthBuffer \| QGL::StereoBuffers \| QGL::DoubleBuffer); QGLWidget* gl_test = new QGLWidget(test_format, 0); gl_test->makeCurrent(); bool supports = gl_test->isValid();` (stereo-capability probe) | `scene.C:3063-3082` | Rewrite the probe with `QSurfaceFormat` + a transient `QOpenGLContext` (or a hidden `QOffscreenSurface`). Simpler: construct a `QSurfaceFormat` with `setStereo(true)`, create a `QOpenGLContext`, `create()`, and check `context.format().stereo()`. This block sets `gl_format_` — keep that contract |
| `main_display_->grabFrameBuffer()` | `scene.C:2096` | `main_display_->grabFramebuffer()` (no args, GUI thread) |
| `static_cast<GLRenderWindow*>(...)->swapBuffers()` | `scene.C:1666` | **Delete** — also delete the `safeBufferSwap()` call at `scene.C:1650`. Replace the whole manual-swap synchronisation block (`scene.C:1644-1672`) with `update()` calls; `QOpenGLWidget` swaps after `paintGL()` |
| `main_display_->makeCurrent()` at `scene.C:2256, 2404` | GUI thread (event handlers / setup) — `QOpenGLWidget::makeCurrent()` is valid here. Keep |
| `new_widget->makeCurrent()` in `addGlWindow()` / stereo (`scene.C:2443, 2469, 2541, 2548, 2558, 2564, 2648, 2708, 2736, 2797, 2825`) | These run on the GUI thread but on top-level `Qt::Window` widgets — see Pitfall 6. `makeCurrent()` is valid; the open question is whether a top-level `QOpenGLWidget` composites correctly |
| `Scene::updateGL()` (`scene.C:3098`, ~25 call sites) | **No change needed.** `Scene` is a plain `QWidget` (`scene.h:138-139`), and `Scene::updateGL()` is its *own* method that just calls `Scene::paintGL()` (`scene.C:3098-3102`). It is **not** `QGLWidget::updateGL()`. Leave all 25 call sites alone — this is the single biggest "looks scary, isn't" item |
| `Scene::paintEvent` → `paintGL()` (`scene.C:552`), `Scene::paintGL()` (`scene.C:524`) | `Scene` itself is not a GL widget — these stay as plain `QWidget` methods. Only `GLRenderWindow`'s paint path changes |
| `Scene::renderText_()` (`scene.C:506-522`) | **Already** uses `QPainter` on a `QPaintDevice` — this is the model for RENDER-05. The GL-widget `renderText` in `glRenderWindow.C` should converge on this same pattern |

### §5 `glRenderer.C`

| Current | Location | Replacement |
|---------|----------|-------------|
| `QImage gldata = QGLWidget::convertToGLFormat(pm);` then `glDrawPixels(...)` | `glRenderer.C:1155-1158` | `QGLWidget::convertToGLFormat` is removed. It did two things: convert to a GL-friendly byte order and flip vertically. Replace with: `QImage gldata = pm.convertToFormat(QImage::Format_RGBA8888).mirrored();` then `glDrawPixels`. Verify byte order on a big-endian-free assumption (all 3 target platforms are little-endian) — see Pitfall 7 |

### §6 `glOffscreenTarget.C` / `glOffscreenTarget.h`

| Current | Location | Replacement |
|---------|----------|-------------|
| `#include <QtOpenGL/QGLPixelBuffer>` | `glOffscreenTarget.C:12` | `<QtGui/QOpenGLFramebufferObject>` (recommended rework) |
| `QGLPixelBuffer::hasOpenGLPbuffers()` | `glOffscreenTarget.C:24,50` | `QOpenGLFramebufferObject` has no availability gate — FBOs are universally available on the target GL versions. Replace the conditional with always-FBO |
| `new QGLPixelBuffer(width, height, share_from_->format(), share_from_)` | `glOffscreenTarget.C:43` | `new QOpenGLFramebufferObject(width, height, QOpenGLFramebufferObject::CombinedDepthStencil)` — requires the share context to be current |
| `pixel_buffer_->makeCurrent()` | `glOffscreenTarget.C:32` | FBOs don't have their own context; `share_from_->makeCurrent()` then `fbo->bind()` |
| `share_from_->grabFrameBuffer()` (3 sites) | `glOffscreenTarget.C:58,68,79` | `share_from_->grabFramebuffer()` |
| `pixel_buffer_->toImage()` | `glOffscreenTarget.C:56,66,77` | `fbo->toImage()` — `QOpenGLFramebufferObject::toImage()` exists `[CITED]` |

## Threading Rework (the hard part)

### What exists today

- `RenderSetup` is a `QThread` — **one per renderer**. Two renderers exist: the interactive `GLRenderer` and the raytracer.
- `RenderSetup::run()` (`renderSetup.C:278-335`) is the worker loop. Crucially it **only** calls `renderToBuffer_()` when `!gl_renderer_` (`renderSetup.C:309`) — i.e. **only the raytracer's thread actually does work in `run()`**. The comment at line 308 is explicit: *"NOTE: GLRenderers currently *have* to render in the GUI thread!"*
- The interactive `GLRenderer` is driven from the GUI thread: `Scene::handleRenderToBufferFinishedEvent_()` (`scene.C:1597-1602`) calls `renderer->renderToBuffer()` for a `GLRenderer` directly, in the GUI-thread event handler.
- Today, worker-thread code still *touches* GL via `RenderSetup::makeCurrent()` → `gl_target_->makeCurrent()` (`renderSetup.C:269-276`) and the GUI thread does `swapBuffers()`.

**So the "hybrid model" the project wants is ~80% already implemented.** The GLRenderer
already renders GUI-thread. The raytracer already produces a CPU buffer. The migration
is *removing the parts that are now invalid*, not rebuilding the architecture.

### What must change

1. **The raytracer worker thread must never call `makeCurrent()` on the widget.**
   `QOpenGLWidget`'s context and default FBO are GUI-thread-affine `[CITED: doc.qt.io/archives/qt-5.15/qopenglwidget.html — "the threaded usage" section]`. Audit: does the raytracer path in `renderToBuffer_()` (`renderSetup.C:378-381`, `BufferedRenderer::renderToBuffer`) issue any GL calls? Expected answer: **no** — it writes `m_pixels` (CPU). If confirmed, `RenderSetup::makeCurrent()` becomes a GUI-thread-only helper and the worker loop simply never calls it. The raytracer's output reaches the screen because `GLRenderWindow::refresh()` (running in `paintGL()` on the GUI thread) blits `m_pixels` as a texture — exactly as it does today.

2. **All GL-context operations move under GUI-thread Qt callbacks.** Concretely:
   - `initializeGL()` — GLEW init, `checkGL()`, initial texture creation.
   - `resizeGL(w,h)` — viewport/texture resize (currently in `GLRenderWindow::resize()`).
   - `paintGL()` — the actual draw: for the GLRenderer, invoke `gl_renderer_->renderToBuffer(...)`; for the raytracer target, do the `refresh()` texture blit. `QOpenGLWidget` guarantees the context is current and `defaultFramebufferObject()` is bound here.

3. **Delete the manual swap/synchronisation machinery.** `setAutoBufferSwap(false)`,
   `swapBuffers()`, `safeBufferSwap()`, and the `keep_in_sync_` dependent-renderer
   swap loop in `scene.C:1644-1672` exist to manually coordinate buffer swaps across
   render threads. `QOpenGLWidget` swaps automatically after `paintGL()` returns. The
   cross-renderer sync (raytracer + GL stereo pair) needs re-expression as: worker
   finishes CPU buffer → posts `RenderToBufferFinishedEvent` → GUI thread calls
   `update()` → `paintGL()` blits. The `RenderToBufferFinishedEvent` mechanism
   (`renderSetup.h:368-381`) **stays** — it's a thread-safe handoff and is still needed.

4. **`lockGLContext()`/`unlockGLContext()`** (`glRenderWindow.C:354-364`): today they
   `makeCurrent()`/`doneCurrent()` under a mutex. After the port, any caller of these
   from a non-GUI thread is a bug. Audit callers; most likely they collapse to no-ops
   or GUI-thread-asserted helpers.

### Qt's official guidance on threaded `QOpenGLWidget`

`[CITED: doc.qt.io/archives/qt-5.15/qopenglwidget.html]` — Qt's "threaded rendering"
guidance for `QOpenGLWidget` is essentially: **you can't.** The default framebuffer is
managed by Qt on the GUI thread. The supported patterns are:
- Render everything in `paintGL()` on the GUI thread (BALL's GLRenderer — chosen path).
- Do heavy work on a worker thread into a CPU buffer or into an FBO via a **separate
  shared context on a `QOffscreenSurface`**, then hand the result to the GUI thread to
  composite in `paintGL()` (BALL's raytracer — already CPU-buffer, the simplest variant
  of this; no shared GL context needed because the raytracer is CPU-only).

This is exactly the hybrid model PROJECT.md locked. No `QOffscreenSurface` or shared
context is actually required for BALL because the only worker (raytracer) is CPU-bound.

### Minimal-risk path

Do **not** attempt to make worker threads do GL. The incremental, verifiable sequence:
1. Swap the base class + `QSurfaceFormat`, add `initializeGL`/`resizeGL`/`paintGL`, keep everything else compiling. Verify: build + GLRenderer scene appears embedded on macOS.
2. Delete manual swap code (`swapBuffers`/`setAutoBufferSwap`/`safeBufferSwap`). Verify: scene still updates on rotate/zoom.
3. Remove worker-thread `makeCurrent()` for the raytracer; confirm raytracer output still blits. Verify: switch to raytracer renderer in the GUI, image appears.
4. Reimplement text overlay. Verify: FPS/info text visible.
5. Address stereo/multi-display top-level windows last (Pitfall 6).

## Don't Hand-Roll

| Problem | Don't Build | Use Instead | Why |
|---------|-------------|-------------|-----|
| Default framebuffer management | Manual `glBindFramebuffer(0)` | `QOpenGLWidget::defaultFramebufferObject()` | `QOpenGLWidget` renders into an FBO, **not** FBO 0. Binding 0 draws into nothing visible. See Pitfall 1 |
| Buffer swapping / vsync | Manual `swapBuffers()` + exposed-check | `QOpenGLWidget` automatic swap after `paintGL()` | Qt handles swap timing, compositor integration, and the macOS layer composition you're trying to fix |
| GL context currency across threads | A mutex + `makeCurrent()`/`doneCurrent()` dance | GUI-thread-only Qt callbacks (`paintGL` etc.) | `QOpenGLWidget`'s context is thread-affine; hand-rolled locking can't make it safe `[CITED]` |
| Image → GL-texture byte conversion | Hand byte-swapping loops | `QImage::convertToFormat(QImage::Format_RGBA8888).mirrored()` | Replaces removed `convertToGLFormat`; handles stride/flip correctly |
| Offscreen render target | Custom pbuffer wrapper | `QOpenGLFramebufferObject` | `QGLPixelBuffer` is deprecated/removed; FBOs are the universal modern path |
| Stereo capability probe | Parsing GL extension strings | `QOpenGLContext` + `QSurfaceFormat::stereo()` after `create()` | Qt already negotiates this with the driver |

**Key insight:** Almost every "manual" mechanism in BALL's render path
(`setAutoBufferSwap(false)` + manual `swapBuffers()` + exposed-checks + the cross-thread
`makeCurrent()` lock) was a *workaround for `QGLWidget`'s lack of compositor
integration*. `QOpenGLWidget` exists precisely to absorb all of that. The port is as
much **deletion** as it is translation — resist the urge to port the workarounds.

## Runtime State Inventory

This is a code refactor (a widget base-class swap). No databases, services, OS
registrations, secrets, or persisted state are involved.

| Category | Items Found | Action Required |
|----------|-------------|------------------|
| Stored data | None — verified: the change is C++ class hierarchy + Qt API calls, no datastore touched | None |
| Live service config | None — verified: no external service references the renderer | None |
| OS-registered state | None — verified: no OS-level registration involved | None |
| Secrets/env vars | None — `BALL_DATA_PATH`/`BALLVIEW_DATA_PATH`/`DYLD_LIBRARY_PATH` (BUILD-macos.md) are unaffected by this phase | None |
| Build artifacts | Stale `.o`/`moc_*` files for the 5 changed translation units; `GLRenderWindow` is a `Q_OBJECT`?-check needed (it is **not** currently — no `Q_OBJECT` macro in `glRenderWindow.h`; if `paintGL` overriding requires none, fine). After the base-class swap, **a clean rebuild of VIEW is required** — moc/vtable layout changes | `make clean` of VIEW target (or full rebuild) before verification; `cmake` reconfigure not needed unless CMake module components change (see §6) |

## Common Pitfalls

### Pitfall 1: Binding framebuffer 0 instead of the QOpenGLWidget default FBO
**What goes wrong:** Any code that does `glBindFramebuffer(GL_FRAMEBUFFER, 0)` after
the port renders into nothing — the screen stays black even though GL calls succeed.
**Why it happens:** `QGLWidget` rendered into the real default framebuffer (0).
`QOpenGLWidget` renders into an internal FBO; 0 is not its target.
**How to avoid:** Audit `glRenderer.C` and `glRenderWindow.C` for `glBindFramebuffer`/
`GL_FRAMEBUFFER`/FBO-0 assumptions. Where rebinding the default is needed, use
`defaultFramebufferObject()`. `[CITED: doc.qt.io/archives/qt-5.15/qopenglwidget.html]`
**Warning signs:** Black scene, GL no errors, `paintGL` is being called.

### Pitfall 2: Getting a core profile instead of compatibility on macOS
**What goes wrong:** Every `glBegin`/`glVertex`/`glMatrixMode`/`gluLookAt`/display-list
call in `glRenderer.C` (~100 sites) generates `GL_INVALID_OPERATION` and draws nothing.
**Why it happens:** `QOpenGLWidget` does **not** default to a 2.1 compat context the
way `QGLWidget` did. On macOS, omitting an explicit profile/version can yield a 3.2+
core context.
**How to avoid:** Explicitly `fmt.setProfile(QSurfaceFormat::CompatibilityProfile)` +
`fmt.setVersion(2, 1)`, and call `QSurfaceFormat::setDefaultFormat(fmt)` in `main()`
before `QApplication`. Verify at runtime by logging `context()->format().profile()`.
**Warning signs:** Flood of `GL_INVALID_OPERATION` from `checkGL()`; molecule geometry absent but raytracer texture (which uses simpler GL) might still show.

### Pitfall 3: Calling makeCurrent() / GL from the raytracer worker thread
**What goes wrong:** Crash, GL errors, or undefined behaviour — `QOpenGLWidget`'s
context belongs to the GUI thread.
**Why it happens:** The pre-port code routed worker threads through
`RenderSetup::makeCurrent()` → `gl_target_->makeCurrent()`.
**How to avoid:** Confirm the raytracer path issues no GL (it writes `m_pixels`), then
strip worker-thread `makeCurrent()`. All GL stays in `initializeGL`/`resizeGL`/`paintGL`.
**Warning signs:** Intermittent crashes when switching to the raytracer; `QOpenGLContext`
warnings in the log about wrong-thread access.

### Pitfall 4: Overriding paintEvent instead of paintGL
**What goes wrong:** Rendering doesn't happen, or happens at the wrong time, or the
auto-swap doesn't fire.
**Why it happens:** `glRenderWindow.C:346` currently overrides `paintEvent`.
`QOpenGLWidget` drives rendering through `paintGL()`; overriding `paintEvent` bypasses
its FBO/swap machinery.
**How to avoid:** Delete the `paintEvent` override; move its logic (the `ignore_events_`
guard + paint work) into `paintGL()`. Use `update()` to schedule repaints.
**Warning signs:** Scene only repaints on resize, or never; `paintGL` breakpoint never hit.

### Pitfall 5: QPainter overlay without beginNativePainting guards
**What goes wrong:** The `QPainter` text overlay corrupts GL state or vice versa;
text doesn't appear or the 3D scene flickers.
**Why it happens:** Mixing raw GL and `QPainter` in the same `paintGL()` requires
explicit state hand-off.
**How to avoid:** See §4 below. Issue all raw GL first, then construct the `QPainter`
on the widget for text. If raw GL must follow `QPainter` setup, wrap it in
`painter.beginNativePainting()` / `painter.endNativePainting()`.
`[CITED: doc.qt.io/archives/qt-5.15/qopenglwidget.html]`
**Warning signs:** Missing/garbled text; scene geometry flickers when text is shown.

### Pitfall 6: Top-level QOpenGLWidget for stereo / multi-display
**What goes wrong:** `scene.C` creates `new GLRenderWindow(0, "Scene", Qt::Window)` in
`addGlWindow()` (`scene.C:2441`) and the stereo paths — a *top-level* GL widget. A
top-level `QOpenGLWidget` is a less-travelled path; on macOS it may hit composition
quirks similar to (though not identical to) the original bug.
**Why it happens:** `QGLWidget` was routinely used as a top-level window;
`QOpenGLWidget` is designed primarily as an embedded child.
**How to avoid:** Treat this as the highest-risk item and **schedule it last**.
Recommended for this phase: keep the code compiling but guard the multi-display/stereo
entry points behind a runtime check, or use `QOpenGLWindow` + `createWindowContainer`
if a real top-level GL surface is needed. Full stereo validation can reasonably be
deferred — it is a niche feature and not part of the Core Value (embedded scene
renders). Document the decision explicitly in the plan.
**Warning signs:** `addGlWindow()` produces a blank or detached window on macOS — the
same class of symptom as the original bug.

### Pitfall 7: convertToGLFormat replacement byte order / flip
**What goes wrong:** Label textures in `glRenderer.C:1155` render with swapped
red/blue channels or upside-down.
**Why it happens:** `QGLWidget::convertToGLFormat` did a format conversion **and** a
vertical flip. A naive replacement misses one.
**How to avoid:** `pm.convertToFormat(QImage::Format_RGBA8888).mirrored()` — `RGBA8888`
gives GL-native byte order on little-endian (all 3 targets), `.mirrored()` does the
flip. Test with an actual atom label.
**Warning signs:** Atom/bond labels with wrong colours or mirrored text.

### Pitfall 8: Context sharing for the copy-constructor / multi-window path
**What goes wrong:** `GLRenderWindow(const GLRenderWindow&, ...)` (`glRenderWindow.C:79`)
used `QGLWidget`'s share-context ctor. Without sharing, display lists / textures /
VBOs created in one window aren't visible in another.
**Why it happens:** `QOpenGLWidget` has no per-widget share-context constructor.
**How to avoid:** Set `QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts)`
**before** constructing `QApplication` (in BALLView's `main()`). This makes all
`QOpenGLWidget` contexts implicitly share. `[CITED: doc.qt.io/qt-5/qopenglwidget.html]`
**Warning signs:** Second scene window is blank or missing geometry that the first shows.

## Code Examples

### QSurfaceFormat replacing the static gl_format_
```cpp
// Source: doc.qt.io/qt-5/qsurfaceformat.html  (Qt 5.15)
// Replaces glRenderWindow.C:26-40
QSurfaceFormat GLRenderWindow::gl_format_ = [] {
    QSurfaceFormat fmt;
    fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
    fmt.setVersion(2, 1);                              // fixed-function pipeline
    fmt.setDepthBufferSize(24);                        // QGL::DepthBuffer
    fmt.setStencilBufferSize(8);                       // QGL::StencilBuffer
    fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer); // QGL::DoubleBuffer
    fmt.setSamples(4);                                 // QGL::SampleBuffers
#ifndef BALL_OS_DARWIN
#  if !defined(BALL_OS_LINUX) || QT_VERSION < QT_VERSION_CHECK(5,7,0) || QT_VERSION > QT_VERSION_CHECK(5,7,1)
    fmt.setStereo(true);                               // QGL::StereoBuffers (guarded)
#  endif
#endif
    // QGL::DirectRendering has no QSurfaceFormat equivalent — dropped (default behaviour)
    return fmt;
}();
```

### Constructor + format application (no format-taking ctor exists)
```cpp
// Replaces glRenderWindow.C:59-77
GLRenderWindow::GLRenderWindow(QWidget* parent_widget, const char* /*name*/, Qt::WindowFlags w_flags)
    : QOpenGLWidget(parent_widget, w_flags)
    /* member inits unchanged */
{
    setFormat(gl_format_);          // must be called before first show
    setAutoFillBackground(false);   // still valid
    // setAutoBufferSwap(false);    // DELETED — QOpenGLWidget auto-swaps
}
```

### initializeGL / resizeGL / paintGL skeleton
```cpp
// Source: doc.qt.io/archives/qt-5.15/qopenglwidget.html
void GLRenderWindow::initializeGL() {
#ifdef BALL_HAS_GLEW
    glewInit();
#endif
    checkGL();
    // texture creation that used to live in init() once size is known
}

void GLRenderWindow::resizeGL(int w, int h) {
    // viewport / texture resize previously in GLRenderWindow::resize()
}

void GLRenderWindow::paintGL() {
    if (ignore_events_) return;          // moved from paintEvent override
    // QOpenGLWidget guarantees: context current, defaultFramebufferObject() bound
    // For a raytracer target: blit m_pixels as texture (old refresh() body)
    // For the GLRenderer target: invoke the renderer here
    // No swapBuffers() — QOpenGLWidget swaps automatically on return
}
```

### QPainter text overlay (RENDER-05)
```cpp
// Source: doc.qt.io/archives/qt-5.15/qopenglwidget.html  ("QPainter on QOpenGLWidget")
void GLRenderWindow::paintGL() {
    // 1. all raw fixed-function GL first (scene / texture blit)
    ...
    // 2. then 2D text via QPainter on the widget itself
    QPainter painter(this);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setPen(/* ColorRGBA → QColor */);
    QFont font; font.setPixelSize(size); font.setBold(true);
    painter.setFont(font);
    painter.drawText(x, y, QString::fromStdString(text));
    // If you need raw GL AFTER this point, wrap it:
    //   painter.beginNativePainting(); ...gl...; painter.endNativePainting();
    painter.end();
}
```

### QOpenGLContext currentContext check
```cpp
// Replaces renderSetup.C:269-276
void RenderSetup::makeCurrent() {
    // GUI-thread only after the port
    if (gl_target_ &&
        QOpenGLContext::currentContext() != gl_target_->context())
        gl_target_->makeCurrent();
    else
        target->prepareRendering();
}
```

### Global context sharing (in BALLView main(), before QApplication)
```cpp
// Source: doc.qt.io/qt-5/qopenglwidget.html  ("Context Sharing")
QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
QSurfaceFormat::setDefaultFormat(GLRenderWindow::gl_format_);
QApplication app(argc, argv);
```

## State of the Art

| Old Approach | Current Approach | When Changed | Impact |
|--------------|------------------|--------------|--------|
| `QGLWidget` (NSOpenGLContext-based) | `QOpenGLWidget` (FBO-backed, layer-compatible) | Qt 5.4 (2014); `QGLWidget` deprecated, removed in Qt 6 | The whole basis of this phase |
| `QGLFormat` | `QSurfaceFormat` | Qt 5.0 | Field-by-field translatable |
| `QGLContext` | `QOpenGLContext` | Qt 5.0 | `currentContext()` rename |
| `QGLPixelBuffer` | `QOpenGLFramebufferObject` | Qt 5.0; pbuffers removed in Qt 6 | `GLOffscreenTarget` rework |
| `QGLWidget::renderText` | `QPainter` on the widget | Removed in Qt 5.0+ (never in `QOpenGLWidget`) | RENDER-05 |
| `QGLWidget::convertToGLFormat` | `QImage::convertToFormat` + `mirrored()` | Removed with `QGLWidget` | `glRenderer.C:1155` |
| Manual `swapBuffers()` + `setAutoBufferSwap` | Automatic swap after `paintGL()` | `QOpenGLWidget` design | Large code deletion |

**Deprecated/outdated:**
- `<QtOpenGL/qgl.h>`, `QGL::` enum namespace — gone in Qt 6, deprecated in 5.15.
- The entire manual buffer-swap synchronisation in `scene.C:1644-1672` — an artifact of `QGLWidget`'s lack of compositor integration.

## Assumptions Log

| # | Claim | Section | Risk if Wrong |
|---|-------|---------|---------------|
| A1 | The raytracer's `renderToBuffer_()` path issues **no** GL calls (writes only `m_pixels`) | Threading §2, Pitfall 3 | If it *does* touch GL, the worker thread needs a shared context on a `QOffscreenSurface` — significantly more work. **Verify by reading `BufferedRenderer::renderToBuffer` before planning.** |
| A2 | `Format_RGBA8888` + `mirrored()` correctly replaces `convertToGLFormat` on all 3 little-endian targets | §5, Pitfall 7 | Wrong channel order on labels — cosmetic, easily caught visually |
| A3 | The two top-level `Qt::Window` `GLRenderWindow` paths can be deferred/guarded without blocking Core Value | Pitfall 6 | If multi-display is considered must-have, scope grows; PROJECT.md Core Value is the *embedded* scene, so risk is low |
| A4 | `GLRenderWindow` has no `Q_OBJECT` macro and adding `paintGL` overrides needs none | Runtime State Inventory | If a `Q_OBJECT` is later needed (signals/slots), moc wiring must be added — verified absent today, low risk |
| A5 | `GLOffscreenTarget` rework to `QOpenGLFramebufferObject` is in-scope-friendly (discretionary) | §6 | If FBO semantics differ enough from pbuffers for the tiling renderer, could be deferred — it's an offscreen-export path, not Core Value |
| A6 | `Qt5::OpenGL` linkage can stay (it still exists in 5.15) and only `Qt5::Widgets` is strictly required for `QOpenGLWidget` | §6 / Environment | If a component is missing, CMake errors loudly at configure — caught immediately |

## Open Questions

1. **Does the raytracer path touch GL on the worker thread?**
   - What we know: `RenderSetup::run()` only calls `renderToBuffer_()` for `!gl_renderer_`; the raytracer is a `BufferedRenderer` writing `m_pixels`.
   - What's unclear: whether `BufferedRenderer::renderToBuffer` (in a separate file not read this session) calls any GL.
   - Recommendation: planner's first task should `grep` `BufferedRenderer`/raytracer sources for GL calls to confirm A1. Cheap, removes the single biggest threading uncertainty.

2. **Should the multi-display / stereo top-level windows be ported, guarded, or stubbed?**
   - What we know: `addGlWindow()` and stereo paths create top-level `Qt::Window` `GLRenderWindow`s; top-level `QOpenGLWidget` is the riskiest pattern.
   - What's unclear: whether users of this BALLView build rely on multi-display.
   - Recommendation: guard behind a runtime check / mark experimental for this phase; revisit in Phase 5. Document explicitly.

3. **`GLRenderWindow` copy-constructor semantics under `AA_ShareOpenGLContexts`.**
   - What we know: the old copy-ctor used `QGLWidget`'s share ctor; `AA_ShareOpenGLContexts` makes all contexts share globally.
   - What's unclear: whether anything relied on *selective* (not global) sharing.
   - Recommendation: global sharing is almost certainly fine and simpler; verify the copy-ctor is even still used (`grep` for the 3-arg ctor).

## Environment Availability

| Dependency | Required By | Available | Version | Fallback |
|------------|------------|-----------|---------|----------|
| Qt 5.15 (`qt@5`) | `QOpenGLWidget`, `QSurfaceFormat`, all of VIEW | ✓ | 5.15.18 (Homebrew) | — |
| `Qt5::Widgets` CMake component | `QOpenGLWidget` (it lives in QtWidgets, not QtOpenGL) | ✓ | already found at `CMakeLists.txt:297` | — |
| `Qt5::Gui` CMake component | `QSurfaceFormat`, `QOpenGLContext`, `QOpenGLFramebufferObject` | ✓ | transitively pulled by Widgets/OpenGL; pulled in explicitly is safest | — |
| `Qt5::OpenGL` CMake component | currently linked (`CMakeLists.txt:304`); still needed only if `QGLPixelBuffer`/legacy retained | ✓ | found at `CMakeLists.txt:297` | Can stay; harmless |
| GLEW (`BALL_HAS_GLEW`) | `glRenderWindow.C:6-8` GL function loading | ✓ | 2.3.1 (Homebrew, `GLEW::GLEW` target) | — |
| C++14 toolchain | BALL build baseline | ✓ | Apple Clang via `BALLCompilerSpecific.cmake` | — |

**Missing dependencies with no fallback:** None.

**Missing dependencies with fallback:** None.

**CMake note (RENDER-08):** `QOpenGLWidget` is part of **`Qt5::Widgets`**, not
`Qt5::OpenGL` `[CITED: doc.qt.io/archives/qt-5.15/qopenglwidget.html — module: QtWidgets]`.
`Qt5::Widgets` is **already** in `VIEW_DEP_LIBRARIES` (`CMakeLists.txt:307`) and the
`FIND_PACKAGE(Qt5 ... Widgets)` at `CMakeLists.txt:297`. `QSurfaceFormat`/`QOpenGLContext`/
`QOpenGLFramebufferObject` are in **`Qt5::Gui`**, transitively available. **No
`find_package` component changes are strictly required.** Recommended: add `Gui`
explicitly to the `FIND_PACKAGE` line for clarity, and keep `Qt5::OpenGL` linked until
Phase 5 (it's harmless and the `QGLPixelBuffer` removal in `glOffscreenTarget.C` is
discretionary). Verify with a clean reconfigure.

## Validation Architecture

> `.planning/config.json` was not present/readable this session; Nyquist validation is
> treated as **enabled** (the default). BALL has no automated GUI/render test harness —
> see Wave 0 Gaps. Validation here is heavily build-and-run-gated, with a mandatory
> human visual confirmation on each of the 3 platforms.

### Test Framework
| Property | Value |
|----------|-------|
| Framework | None for VIEW/rendering. BALL has a `test/` tree (`ctest`) but it is `EXCLUDE_FROM_ALL` and untriaged (ROADMAP Phase 8). No GL/widget tests exist. |
| Config file | none — see Wave 0 |
| Quick run command | `cd build && make VIEW BALLView -j8` (compile gate) |
| Full suite command | `cd build && make BALL VIEW BALLView -j8` then manual launch (see below) |

### Phase Requirements → Test Map
| Req ID | Behavior | Test Type | Automated Command | File Exists? |
|--------|----------|-----------|-------------------|--------------|
| RENDER-01 | No `QGLWidget`/`QGLFormat`/`QGLContext` symbols remain; builds | static + compile | `grep -rn "QGLWidget\|QGLFormat\|QGLContext\|QGL::\|QGLPixelBuffer\|QtOpenGL/qgl" source/VIEW include/BALL/VIEW` returns 0 hits; `make VIEW` succeeds | grep: ✅ / build: ✅ |
| RENDER-02 | Molecule renders embedded in main window on macOS (not detached) | manual-GUI | launch BALLView, load demo, observe scene area | ❌ manual-only — human gate |
| RENDER-03 | GLRenderer on GUI thread; no `swapBuffers`/`setAutoBufferSwap` | static | `grep -rn "swapBuffers\|setAutoBufferSwap\|safeBufferSwap" source/VIEW` returns 0 hits | ✅ grep |
| RENDER-04 | Raytracer output blits as texture | manual-GUI | switch renderer to raytracer in GUI, observe image | ❌ manual-only |
| RENDER-05 | On-screen text via QPainter overlay | manual-GUI | enable FPS display, observe text over scene | ❌ manual-only |
| RENDER-06 | Rotate/zoom/pick/select work | manual-GUI | drag-rotate, scroll-zoom, click-pick an atom | ❌ manual-only |
| RENDER-07 | 5 files cleaned, compile clean | compile | `make VIEW` with `-Werror`-free build of the 5 TUs | ✅ build |
| RENDER-08 | Builds + renders on Linux + Windows | compile + manual-GUI | CI/local build on each OS + human visual check | ❌ build automatable, render manual-only |

### Sampling Rate
- **Per task commit:** `make VIEW -j8` — the 5 changed TUs must compile clean. For static-cleanup tasks, also run the `grep` gate for the symbol(s) that task removes.
- **Per wave merge:** `make BALL VIEW BALLView -j8` from a clean VIEW (`make clean` of VIEW first — base-class swap changes vtable/moc layout) + launch BALLView once on macOS and confirm the scene renders embedded.
- **Phase gate:** Full clean build + the manual smoke checklist below, on macOS first; Linux + Windows builds green; RENDER-08 visual confirmation requires a human on each platform.

### Manual smoke checklist (the human gate — cannot be automated)
1. BALLView launches; main window + dock panels appear.
2. Startup demo builds a peptide; **the molecule is visible inside the Scene area** (not a detached/blank window) — this is RENDER-02, the Core Value.
3. Left-drag rotates; scroll/zoom changes view; click-pick selects an atom (RENDER-06).
4. Switch to the raytracer renderer — raytraced image appears in the scene (RENDER-04).
5. Enable FPS/info text — text is legible over the 3D scene (RENDER-05).
6. No flood of GL errors in the console (`checkGL()` output).
7. Repeat 1–6 on Linux and on Windows (RENDER-08).

### Wave 0 Gaps
- [ ] No automated render/GUI test exists and **none should be built in this phase** — building a GL test harness is out of scope; rely on the compile gate + grep gates + human smoke checklist.
- [ ] Add a `grep`-based "no legacy Qt GL symbols" check as a scripted step (could later become a CI lint) — cheap, catches RENDER-01/03/07 regressions automatically.
- [ ] Framework install: none required.
- [ ] (Optional, Phase 8 territory) wiring the `test/` tree into `ctest` — explicitly **not** this phase.

## Security Domain

Not applicable. This phase is a local GUI rendering refactor — no authentication,
session, access-control, input-validation, cryptography, or network surface is touched.
`security_enforcement` considerations: the change introduces no new external input
paths, no parsing of untrusted data, no IPC. The only "input" is OpenGL driver
behaviour, handled by Qt. No ASVS category applies.

## Sources

### Primary (HIGH confidence)
- Direct read of BALL source: `glRenderWindow.{h,C}`, `renderWindow.h`, `renderSetup.{h,C}`, `glOffscreenTarget.C`, `glRenderer.C` (excerpt), `scene.{h excerpt,C excerpts}`, `CMakeLists.txt`, `BUILD-macos.md`, `PROJECT.md`, `REQUIREMENTS.md`, `ROADMAP-1.6.md` — all line numbers cited are verified.
- `doc.qt.io/archives/qt-5.15/qopenglwidget.html` — QOpenGLWidget class, threading, FBO, QPainter, context sharing (Qt 5.15.19 archive).
- `doc.qt.io/qt-5/qsurfaceformat.html` — QSurfaceFormat profile/version/buffer setters.
- `doc.qt.io/qt-5/qopenglcontext.html`, `doc.qt.io/qt-5/qoffscreensurface.html` — context/surface APIs.

### Secondary (MEDIUM confidence)
- `forum.qt.io/topic/153179` — mixing QOpenGLFunctions and QPainter in QOpenGLWidget (corroborates the §4 overlay pattern).
- Apple OpenGL profile availability (2.1 compat vs 3.2/4.1 core) — well-established platform fact, corroborated by Qt macOS docs.

### Tertiary (LOW confidence)
- `convertToGLFormat` replacement byte-order detail (A2) — based on training knowledge of `QImage::Format_RGBA8888` semantics; verify visually with an atom label.

## Metadata

**Confidence breakdown:**
- API migration map: HIGH — every "current" fact verified by direct source read; every replacement cited to Qt 5.15 docs.
- Threading rework: HIGH on the diagnosis (codebase already GUI-thread-renders the GLRenderer, verified at `renderSetup.C:308` + `scene.C:1597`); MEDIUM on completeness — hinges on A1 (raytracer issues no GL), which the planner must confirm first.
- Pitfalls: HIGH — all are documented Qt behaviours or direct consequences of verified code.
- CMake/environment: HIGH — `find_package` lines and link lists read directly; `QOpenGLWidget` module membership cited.

**Research date:** 2026-05-14
**Valid until:** ~2026-06-14 (Qt 5.15 is frozen/stable; the codebase is a frozen 2022 snapshot — facts are unusually durable, 30 days is conservative)
