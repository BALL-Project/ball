---
phase: 02-rendering-port-4a
plan: 04
subsystem: view-rendering
tags: [opengl, qopenglwidget, scene, paintgl, hidpi, stereo, human-verified]

# Dependency graph
requires:
  - phase: 02-rendering-port-4a
    provides: GLRenderWindow on QOpenGLWidget (02-02), renderer subsystem ported (02-03)
provides:
  - "scene.C fully ported off QGLWidget/QGLFormat/QGLContext"
  - "GL scene renders correctly embedded in the main window on macOS (human-verified)"
  - "GL render driven from GLRenderWindow::paintGL() — the QOpenGLWidget-correct model"
affects: [02.1, 5]

# Tech tracking
tech-stack:
  added: []
  patterns:
    - "All GL framebuffer rendering happens inside paintGL() (QOpenGLWidget contract)"
    - "Device-pixel-ratio factor applied at glViewport/gluPickMatrix sites; logical coords kept elsewhere"

key-files:
  created: []
  modified:
    - source/VIEW/WIDGETS/scene.C
    - source/VIEW/RENDERING/glRenderWindow.C
    - include/BALL/VIEW/RENDERING/glRenderWindow.h
    - include/BALL/VIEW/WIDGETS/scene.h
    - source/VIEW/RENDERING/renderSetup.C
    - source/VIEW/RENDERING/RENDERERS/glRenderer.C
    - include/BALL/VIEW/RENDERING/RENDERERS/glRenderer.h

key-decisions:
  - "GL scene must render INSIDE paintGL(): QGLWidget allowed render-anywhere-then-swap; QOpenGLWidget's default FBO is only valid inside paintGL(). Scene::eventFilter no longer swallows the GL widget's Paint/Resize events; ignoreEvents(true) no longer forced on GL-driven windows."
  - "Scene GL-context init deferred to GLRenderWindow::initializeGL() — QOpenGLWidget creates its context lazily on first show, unlike QGLWidget's eager constructor context."
  - "HiDPI: GLRenderer keeps logical coords; a devicePixelRatioF() factor is applied only at glViewport/gluPickMatrix. No-op on non-Retina, so Linux/Windows are unaffected."
  - "Top-level Qt::Window stereo / multi-display GLRenderWindow paths guard-deferred to Phase 5 (Core Value is the embedded scene)."

patterns-established:
  - "paintGL() branches on active renderer: GLRenderer -> drive renderToBuffer() with one clear+draw; raytracer -> refresh() CPU-buffer texture blit"

requirements-completed: [RENDER-02, RENDER-04, RENDER-05, RENDER-06, RENDER-07]

# Metrics
duration: ~4h (incl. 3 debug rounds + human verification cycles)
completed: 2026-05-14
---

# Phase 2 Plan 04: scene.C Port + Human Verification Summary

**The final and most central file (`scene.C`) is ported off `QGLWidget`, and the
embedded 3D scene renders correctly on macOS — human-verified after three debug
rounds that uncovered structural QGLWidget→QOpenGLWidget mismatches the mechanical
port alone did not catch.**

## What was delivered

**Task 1-2 (mechanical port):** `scene.C` rewritten — the `QGLWidget`/`QGLFormat`
stereo-capability probe replaced with `QSurfaceFormat` + a transient
`QOpenGLContext`; the manual cross-renderer buffer-swap synchronisation block
replaced with `update()`; `grabFrameBuffer()` → `grabFramebuffer()`; the
`<QtOpenGL/QGLPixelBuffer>` include dropped. `Scene::updateGL()` and its call
sites left untouched (it is Scene's own `QWidget` method). The 4 top-level
`Qt::Window` stereo / multi-display paths guard-deferred to Phase 5.

**Task 3 (human smoke check) — passed after 3 debug rounds.** The mechanical port
compiled and linked, but the running GUI exposed three structural defects that
`QGLWidget` had masked. All three were diagnosed and fixed:

| # | Defect | Root cause | Fix commit |
|---|--------|-----------|-----------|
| 1 | `NoDisplayListAvailable` crash on launch | `Scene` ctor allocated display lists, but `QOpenGLWidget` creates its GL context lazily on first show (`QGLWidget` did it eagerly in its ctor) | `29aa3d2` — defer GL init to `GLRenderWindow::initializeGL()` |
| 2 | Molecule mis-projected — geometry rendered into a quarter of the framebuffer | HiDPI: `QOpenGLWidget`'s FBO is device-pixel sized (2× on Retina), but `GLRenderer` set `glViewport` with logical pixels | `81d1145` — device-pixel-ratio factor at `glViewport`/`gluPickMatrix` |
| 3 | Scene blank until first rotate; blank on resize; z-fighting/banding | `GLRenderWindow::paintGL()` was never called (0×) — `Scene::eventFilter` swallowed its Paint events + `ignoreEvents(true)` was forced; the scene was drawn from an event handler into an FBO `QOpenGLWidget` considers undefined outside `paintGL()` | `5ca7a47` — render GL scene inside `paintGL()` |

Plus `207b1b9` — silenced two benign startup warnings (transient 0×0 resize log;
hardcoded `Sans Serif` font family in 6 `.ui` files).

## Verification

- **Static/compile (RENDER-07):** `scene.C` and all 5 renderer-subsystem files
  compile clean; the grep gate (`check-no-legacy-gl-symbols.sh`) is clean across
  `source/VIEW` + `include/BALL/VIEW` (the one known carve-out, `glDisplayList.h`'s
  `QtOpenGL/qgl.h`, is a Qt 6 blocker logged for Phase 5, not a Qt 5 build blocker).
- **Instrumented headless run:** `GLRenderWindow::paintGL()` now runs every frame,
  each with exactly one `glClear(COLOR|DEPTH)` + one `GLRenderer::renderToBuffer()`
  into the QOpenGLWidget default FBO; viewport matches the device-pixel FBO; 0 GL
  errors, 0 exceptions; clean startup log.
- **Human visual verification (RENDER-02/04/05/06):** user confirmed "Approved" —
  the demo peptide renders embedded in the main window immediately on launch, as a
  correct ball-and-stick model (round cylinders + spheres), survives window resize,
  and rotate/zoom/pick work.

## Not done in this plan (correctly deferred)

- **RENDER-08** (renders on Linux + Windows): cannot be verified without the CI
  matrix (Phase 9) and the dependency model (Phase 4). Remains Pending — flagged
  in REQUIREMENTS.md traceability.
- Top-level stereo / multi-display windows: guard-deferred to Phase 5.

## Notes for downstream phases

- **Phase 02.1** (renderer boundary): the `RenderSetup*` back-reference added to
  `GLRenderWindow` and the `paintGL()`-drives-render flow are exactly the seams
  the `RenderSurface` extraction will formalise.
- **Phase 5** (Qt 6): `glDisplayList.h` still includes the legacy `QtOpenGL/qgl.h`
  — a Qt 6 removal blocker, logged in `deferred-items.md`.
