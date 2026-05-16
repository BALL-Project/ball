---
status: resolved
resolution: "User confirmed 2026-05-16 — visual recheck on macOS shows the three trigger symptoms (mis-projected cylinders, off-viewport, 0×0 startup error) are all fixed. Phase 2 + Phase 5 render path rewrite (Plans 05-05/06 GL-core spike + render-smoke green on CI run 25953405453) closed the underlying issue. The original Phase 2 bug (renderToBuffer outside paintGL) was the root cause; the post-2026-05-14 fix sweep addressed it."
trigger: "BALLView renders embedded but ball-and-stick bonds draw as enormous mis-projected cylinders off the viewport edges after QGLWidget->QOpenGLWidget port"
created: 2026-05-14
resolved_at: 2026-05-16
updated: 2026-05-16
audit_notes:
  - "2026-05-16 (ROADMAP-AUDIT-V1.6.2.md §D4): Phase 2 + Phase 5 render path was substantially rewritten post-2026-05-14 (Plan 05-05/06 GL-core spike + render-smoke green on CI run 25953405453). The original Phase 2 bug (renderToBuffer outside paintGL — see Eliminated section below) was fixed."
  - "2026-05-16 user visual recheck confirmed all three trigger symptoms gone; status flipped resolved, file moved to .planning/debug/resolved/."
---

## Current Focus

hypothesis: CONFIRMED (2nd root cause) — GL render path still followed the QGLWidget
  "render anywhere, then present" model. GLRenderWindow::paintGL() was NEVER invoked by
  Qt (Scene::eventFilter swallowed QEvent::Paint; Scene::init + RenderSetup::run set
  ignoreEvents(true)). The GL scene was drawn from Scene::handleRenderToBufferFinishedEvent_
  — an event handler — into a QOpenGLWidget FBO that is only valid/current inside paintGL().
test: BALL_VIEW_DEBUG instrumentation counted paintGL/renderToBuffer invocations and FBO
  binding; before fix GLRenderWindow::paintGL ran 0x while Scene::paintGL ran 1233x.
expecting: GLRenderWindow::paintGL runs, GLRenderer::renderToBuffer runs inside it into
  fbo=1, exactly one clear+draw per frame — CONFIRMED (14 paintGL = 14 renderToBuffer,
  all fbo=1, viewport stable at 2160x1154, 0 GL errors, 0 exceptions).
next_action: user visual recheck of the three symptoms

## Symptoms

expected: RQI tripeptide renders as recognizable ball-and-stick molecule embedded in Scene viewport
actual: bonds draw as enormous mis-projected green/cyan cylinders extending far off viewport edges; molecule unrecognizable
errors: "Cannot resize window. Size 0 x 0 is not supported" in startup log
reproduction: build BALL VIEW BALLView, run headless smoke, demo builds RQI peptide automatically
started: after QGLWidget->QOpenGLWidget port (GSD Phase 2)

## Eliminated

## Evidence

- timestamp: 2026-05-14
  checked: GLRenderer::renderToBuffer (glRenderer.C:547)
  found: renderToBuffer does NOT set its own viewport or projection — relies entirely on state set by setSize()/initPerspective()/updateCamera() earlier
  implication: whatever viewport+frustum setSize installs is what renderToBuffer uses

- timestamp: 2026-05-14
  checked: GLRenderer::setSize (glRenderer.C:2075), setProjection (2375)
  found: setSize sets width_/height_, x_scale_/y_scale_ from aspect ratio, glViewport(0,0,width_,height_), initPerspective->setProjection->glFrustum(left_..). left_=-2*x_scale_ etc.
  implication: viewport and frustum both derive from width_/height_. Internally consistent ONLY if width_/height_ match the real framebuffer pixel size.

- timestamp: 2026-05-14
  checked: Scene::resizeEvent (scene.C:558), RenderSetup::resize (renderSetup.C:152), Scene::initializeGLContext (scene.C:3376)
  found: renderers get Scene::width()/height() = LOGICAL pixels. QOpenGLWidget default FBO is DEVICE pixels (Retina 2x).
  implication: glViewport gets logical px into a device-px FBO -> viewport covers only bottom-left quarter on Retina

- timestamp: 2026-05-14
  checked: instrumented GLRenderer::renderToBuffer to print active GL_VIEWPORT + GL_FRAMEBUFFER_BINDING
  found: active viewport=0,0,857x577 while QOpenGLWidget FBO (binding=1) is device-pixel sized 1714x1154 (dpr=2)
  implication: ROOT CAUSE — GLRenderer renders into bottom-left quarter of the framebuffer; QOpenGLWidget composites the whole FBO -> molecule grossly mis-placed/mis-scaled

- timestamp: 2026-05-14
  checked: post-fix instrumentation
  found: logical=857x577 pixel_ratio=2 glViewport=1714x1154 — viewport now matches FBO exactly; frustum aspect 1.485 correct; 0 GL errors, 0 exceptions; 0x0 resize no longer produces NaN frustum
  implication: HiDPI fix (commit 81d1145) verified at log level; user confirmed it WORKED (molecule correctly sized + centered)

# ---- Second investigation: three remaining visual bugs ----

- timestamp: 2026-05-14
  checked: user visual recheck after 81d1145
  found: (1) ball-and-stick bonds render as flat horizontally-banded blocks (z-fighting look), (2) window empty until first mouse rotate, (3) resize blanks the scene
  implication: not projection (size/centering fine) — points at the present/compositing model, all three likely one root cause

- timestamp: 2026-05-14
  checked: GLRenderWindow::paintGL (only called refresh() — the raytracer CPU-buffer texture-blit), Scene::handleRenderToBufferFinishedEvent_ (called renderer->renderToBuffer() for GLRenderer, i.e. drew the scene from an EVENT HANDLER), GLRenderer::renderToBuffer (does its own glClear + glDrawBuffer(GL_BACK) + depth-tested draw straight into the bound FBO; never writes m_pixels)
  found: GL scene was being drawn outside paintGL(); paintGL() then blitted the stale/empty m_pixels texture on top with depth-test disabled
  implication: classic QGLWidget->QOpenGLWidget porting bug — QOpenGLWidget's default FBO is only valid/current inside paintGL()

- timestamp: 2026-05-14
  checked: instrumented Scene::paintGL, Scene::handleRenderToBufferFinishedEvent_, GLRenderWindow::paintGL, GLRenderer::renderToBuffer with BALL_VIEW_DEBUG; headless smoke run
  found: Scene::paintGL ran 1233x, handleRenderToBufferFinishedEvent_ 1231x, but GLRenderWindow::paintGL ran 0x and GLRenderer::renderToBuffer ran 0x
  implication: ROOT CAUSE — GLRenderWindow::paintGL() is NEVER invoked by Qt. Scene::eventFilter swallows the GL widget's QEvent::Paint (filter_out=true) and Scene::init()+RenderSetup::run() set ignoreEvents(true). The widget is a fully passive surface; the scene only ever reached the FBO via the out-of-paintGL event-handler draw, whose result QOpenGLWidget treats as undefined. Empty-until-rotate, blank-on-resize and banding all follow.

- timestamp: 2026-05-14
  checked: post-fix instrumentation, headless smoke run
  found: GLRenderWindow::paintGL ran 14x, each immediately followed by exactly one GLRenderer::renderToBuffer clear+draw into fbo=1 (the QOpenGLWidget default FBO); viewport stabilised at 2160x1154 (dpr=2); ignore_events_=0; 0 GL errors, 0 exceptions; Scene::paintGL spin dropped from 1233 to ~11
  implication: fix verified at log level — single clear+draw per frame, drawn in the only place the FBO is valid; awaiting user visual recheck

## Resolution

root_cause: |
  TWO root causes from the QGLWidget->QOpenGLWidget port, fixed in two commits.

  (1) HiDPI viewport mismatch [commit 81d1145, user-confirmed FIXED]:
  QOpenGLWidget's default framebuffer is device-pixel sized (Retina dpr=2 =>
  2x logical). GLRenderer::setSize()/setupStereo() uploaded glViewport() in
  logical pixels, so the scene rendered into only the bottom-left quarter of
  the FBO -> grossly mis-projected/oversized geometry.

  (2) GL render path still used the QGLWidget "render anywhere, then present"
  model [this commit]: GLRenderWindow::paintGL() was NEVER invoked by Qt --
  Scene::eventFilter() swallowed the GL widget's QEvent::Paint, and
  Scene::init() + RenderSetup::run() set ignoreEvents(true), making the widget
  a fully passive surface. The GL scene was instead drawn from
  Scene::handleRenderToBufferFinishedEvent_ (an event handler) via
  GLRenderer::renderToBuffer(), which does its own glClear + glDrawBuffer +
  depth-tested draw straight into the bound FBO. QOpenGLWidget's default FBO
  is only valid/current INSIDE paintGL(); rendered anywhere else its contents
  are undefined. paintGL() then ran refresh() and blitted the stale/empty
  m_pixels CPU texture (the raytracer path) on top with depth-test disabled.
  This produced all three symptoms: empty window until an interaction forced
  a repaint, blank-on-resize, and z-fighting/banding from the stale blit
  composited over the undefined FBO.
fix: |
  Commit 1 (81d1145): GLRenderer::pixel_ratio_ — see prior notes.

  Commit 2 (this commit) — render the GL scene INSIDE paintGL():
  - GLRenderWindow gains a non-owning RenderSetup* (setRenderSetup()).
    RenderSetup registers itself in init() and resize().
  - GLRenderWindow::paintGL(): if a GLRenderer drives the window, call
    render_setup_->renderToBuffer() here (GLRenderer::renderToBuffer already
    does its own glClear(COLOR|DEPTH) + depth-tested draw) — the only place
    QOpenGLWidget guarantees a valid, current default FBO. Buffered/raytracer
    renderers keep the existing refresh() texture-blit branch.
  - Scene::handleRenderToBufferFinishedEvent_: for a GLRenderer, no longer
    renders into the FBO from the event handler — it just lets the existing
    update() request schedule the real paintGL(). Raytracer path unchanged
    (still does the CPU-buffer handoff via refresh()).
  - Scene::eventFilter: stop swallowing QEvent::Paint AND QEvent::Resize for
    the GL window, so GLRenderWindow::paintGL()/resizeGL() actually run and
    the QOpenGLWidget recreates its FBO on resize.
  - Scene::init() + RenderSetup::run(): stop calling ignoreEvents(true) for a
    GL-renderer-driven window (run() still does for a non-GL raytracer
    worker, where the window is a passive blit surface).
  Raytracer path and platform-independence preserved (devicePixelRatioF()==1
  and the refresh() branch untouched on non-HiDPI / raytracer setups).
verification: |
  Commit 1: see prior notes; user confirmed the molecule is correctly sized
  and centered.
  Commit 2: BALL_VIEW_DEBUG instrumentation, headless smoke test.
  Before fix: GLRenderWindow::paintGL ran 0x, GLRenderer::renderToBuffer 0x,
  Scene::paintGL spun 1233x. After fix: GLRenderWindow::paintGL ran 14x, each
  followed by exactly one GLRenderer::renderToBuffer clear+draw into fbo=1
  (the QOpenGLWidget default FBO), viewport stable at 2160x1154 (dpr=2),
  ignore_events_=0, 0 GL errors, 0 exceptions, Scene::paintGL spin down to
  ~11. Production binary (instrumentation removed) re-run: alive 14s, 0
  exceptions, 0 GL errors. Awaiting user visual recheck of the three symptoms.
files_changed:
  - include/BALL/VIEW/RENDERING/RENDERERS/glRenderer.h
  - source/VIEW/RENDERING/RENDERERS/glRenderer.C
  - source/VIEW/RENDERING/renderSetup.C
  - include/BALL/VIEW/RENDERING/glRenderWindow.h
  - source/VIEW/RENDERING/glRenderWindow.C
  - source/VIEW/WIDGETS/scene.C
