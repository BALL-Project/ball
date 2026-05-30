# 02-A1 Findings — Does the raytracer / BufferedRenderer worker path touch GL?

**Resolves:** RESEARCH.md Open Question 1 / Assumptions Log A1
**Investigated:** 2026-05-14 (read-and-document only — no source files modified)
**Method:** Direct source read of the BufferedRenderer/RaytracingRenderer hierarchy +
`RenderSetup::run()` worker loop + `grep` for GL call prefixes.

---

## VERDICT: A1 CONFIRMED

The raytracer / `BufferedRenderer` worker-thread render path issues **no OpenGL calls**.
`RenderSetup::run()` on a worker thread only drives `BufferedRenderer::renderToBuffer`,
which writes a CPU pixel buffer (`FrameBufferPtr`) and contains zero GL. This is the
"simple removal" threading path — no `QOffscreenSurface` + shared context is needed.

---

## Evidence

### 1. The worker loop only renders `!gl_renderer_`

`source/VIEW/RENDERING/renderSetup.C:305-310` — `RenderSetup::run()` worker loop:

```
while (!about_to_quit_)
{
    t.start();
    // NOTE: GLRenderers currently *have* to render in the GUI thread!
    if (!gl_renderer_)          // renderSetup.C:309
        renderToBuffer_();      // renderSetup.C:310
    ...
}
```

So the worker thread does work only when the renderer is **not** a `GLRenderer`.

### 2. `renderToBuffer_()` dispatches by renderer type

`source/VIEW/RENDERING/renderSetup.C:378-395`:

- `renderSetup.C:378-381` — `BufferedRenderer` branch: `((BufferedRenderer*)renderer)->renderToBuffer(target, *stage_);` — **no GL call** in this branch.
- `renderSetup.C:382-390` — `GLRenderer` branch contains `glFlush()` (`renderSetup.C:389`), but `run()` guards this off (`!gl_renderer_`); it is only reached when `renderToBuffer_()` is called directly from the GUI thread (`Scene::handleRenderToBufferFinishedEvent_()`, `scene.C:1599`).
- `renderSetup.C:391-395` — `TilingRenderer` branch contains `glFlush()` (`renderSetup.C:394`). See item 5 — `TilingRenderer` is never started on a worker thread in in-scope code.

### 3. `BufferedRenderer::renderToBuffer` is pure CPU

`include/BALL/VIEW/RENDERING/RENDERERS/bufferedRenderer.h:62-77` — `renderToBuffer()`
calls `prepareBufferedRendering()` → `renderToBufferImpl(buffer)` → writes the
`FrameBufferPtr`. grep evidence:

- `grep -En "gl[A-Z]|glu[A-Z]|GLEW|QOpenGL|makeCurrent|QGL" include/BALL/VIEW/RENDERING/RENDERERS/bufferedRenderer.h` → **none found**
- `grep -En "gl[A-Z]|glu[A-Z]|GLEW|QOpenGL|makeCurrent|QGL" include/BALL/VIEW/RENDERING/RENDERERS/raytracingRenderer.h source/VIEW/RENDERING/RENDERERS/raytracingRenderer.C` → **none found**

`RaytracingRenderer` (`raytracingRenderer.{h,C}`) derives from `BufferedRenderer` and
adds only format-support predicates and `intersectRaysWithGeometry()` — no GL.

### 4. Concrete raytracer impl (`rtfactRenderer`) also issues no GL

- `grep -En "gl[A-Z]|glu[A-Z]|makeCurrent|QGL" source/VIEW/RENDERING/RENDERERS/rtfactRenderer.{h,C}` → only hit is `rtfactRenderer.C:190`, a **commented-out** `//glOrtho(...)` line. No live GL.
- `BALL_HAS_RTFACT` / RTfact is out of scope project-wide (PROJECT.md "Out of Scope"); even so, it confirms the pattern: raytracers are CPU-buffer producers.

### 5. `TilingRenderer` does contain GL, but never runs on a worker thread

`source/VIEW/RENDERING/RENDERERS/tilingRenderer.C` issues fixed-function GL
(`glGetIntegerv`, `glMatrixMode`, `glViewport`, `glFrustum`, `glFlush`, etc. at
`tilingRenderer.C:116-186`). However:

- The only in-scope `TilingRenderer` `RenderSetup` (`scene.C:2163-2178`) is driven
  **synchronously on the GUI thread**: it pushes the `RenderSetup`, calls `updateGL()`,
  then pops it — it never calls `RenderSetup::start()` (no worker thread).
- The only `tr_rs->start()` that could pair with a `TilingRenderer` is at
  `scene.C:2208`, but that block is inside `#ifdef BALL_HAS_RTFACT` and wraps a
  `t_RaytracingRenderer` (a `BufferedRenderer`, CPU-only) — not a `TilingRenderer`.
- Conclusion: no `TilingRenderer` is ever `start()`-ed as a `QThread`. Its GL calls
  always execute on the GUI thread.

---

## Implication note for Plan 03 and Plan 04

**Plan 03 (`renderSetup.C` cleanup):** With A1 resolved in the affirmative, the
threading scope is the *minimal* path. `RenderSetup::makeCurrent()` (`renderSetup.C:269-276`) becomes a
GUI-thread-only helper — the `run()` worker loop never needs a current GL context
because the only thing it drives (`BufferedRenderer::renderToBuffer`) is pure CPU.
The worker-thread `makeCurrent()` call can simply be removed; no `QOffscreenSurface`
or shared `QOpenGLContext` is required. The raytracer's CPU buffer still reaches the
screen via `GLRenderWindow::refresh()` running inside `paintGL()` on the GUI thread.

**Plan 04 (`scene.C` threading):** No escalation. The hybrid model RESEARCH.md describes
is already structurally in place — `GLRenderer` renders GUI-thread, the raytracer is a
CPU-buffer worker. Plan 04 can delete the manual swap/sync machinery
(`swapBuffers`/`setAutoBufferSwap`/`safeBufferSwap`, the `keep_in_sync_` loop) and rely
on `RenderToBufferFinishedEvent` → GUI-thread `update()` → `paintGL()` without
rearchitecting the renderer. The one caution carried forward: keep `TilingRenderer`'s
GL-bearing path GUI-thread-only (it already is), and do not introduce a worker-thread
`start()` for it.
