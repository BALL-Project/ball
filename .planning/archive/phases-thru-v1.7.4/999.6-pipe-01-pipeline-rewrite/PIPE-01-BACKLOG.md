---
phase: 999.6-pipe-01-pipeline-rewrite
type: backlog
status: dormant
gates: Phase 5 must land + Phase 5's SPIKE-02 decision record must exist
milestone: v2 (NOT v1.6)
researched: 2026-05-15
---

# Phase 999.6: PIPE-01 — Pipeline Rewrite (OpenGL fixed-function → modern backend)

## Status: DORMANT — DO NOT PROMOTE BEFORE PHASE 5 LANDS

This backlog entry captures the **known transition surface** and **research-backed
context** for the eventual OpenGL → QRhi (or GL-core) pipeline rewrite. It is
explicitly **not** a scoped task list. The scoped task list is **Phase 5's
SPIKE-02 deliverable** — a throwaway prototype runs against the live Phase 02.1
`RendererFactory` boundary, measures backend candidates on macOS/Windows, and
emits the decision record + task list that turns this dormant entry into a
plannable phase.

Pre-scoping PIPE-01 now (before the spike) risks:

1. Locking in a backend assumption (QRhi vs GL-core) that the spike would refute.
2. Missing per-platform constraints — macOS Metal vs Windows D3D11 driver
   behaviour, Apple Silicon GPU specifics — that only surface in real
   measurement.
3. Wasting planning effort on a task list that gets thrown out when the spike
   result arrives.

## Why this exists

BALL's rendering pipeline today is ~100 fixed-function OpenGL calls in
[`source/VIEW/RENDERING/RENDERERS/glRenderer.C`](../../../source/VIEW/RENDERING/RENDERERS/glRenderer.C),
driven through a compatibility-profile `QOpenGLWidget` (post Phase 2). This is
intentionally preserved through Phase 5 (Qt 6 migration keeps fixed-function
working via the compat profile). But:

- **macOS OpenGL is a dead-end.** Deprecated since macOS 10.14 (Mojave, 2018),
  stuck at OpenGL 4.1, still runs on Apple Silicon "for now" but with no
  guarantee. Metal is the official direction.
- **Windows OpenGL is alive but neglected.** Vendor driver attention is on
  D3D11/12 and Vulkan; OpenGL drivers can lag on newer GPUs.
- **Linux OpenGL is healthy** but Vulkan is the modern direction.
- **Fixed-function is removed entirely from OpenGL core 3.2+** (2009). BALL's
  ~100 fixed-function calls each need a shader+VBO replacement regardless of
  which backend wins.

The v1.6 milestone deliberately stops short of this work — modernizing the
toolchain, dependencies, language, and getting cross-OS green CI is enough for
a shippable release. PIPE-01 is the next layer down, decoupled to its own
milestone (likely v2) so v1.6 can ship without it.

## Dependencies (must be true before promotion)

- ✅ **Phase 02.1** — `Renderer` / `RenderSurface` / `RendererFactory` boundary
  in place (done 2026-05-14). PIPE-01 adds one new `Renderer` subclass + one
  new `RenderSurface` subclass; nothing else outside those files should change.
- ❌ **Phase 5** — Qt 6 migration (QT6-01, QT6-02). PIPE-01 requires Qt 6.7+
  for `QRhiWidget` (if QRhi wins) or Qt 6's modern OpenGL bindings (if GL-core
  wins). Phase 5 also closes the ~17 deferred stereo/multi-display sites in
  scene.C — those must be routed through RendererFactory before PIPE-01 can
  swap the backend.
- ❌ **Phase 5 SPIKE-01** — throwaway prototype of leading backend candidate(s)
  behind RendererFactory.
- ❌ **Phase 5 SPIKE-02** — decision record naming the chosen backend,
  rationale, per-platform acceptance criteria, and the scoped task list. This
  is what turns this dormant doc into a plannable phase.

## Known transition surface

### From Phase 02.1's `RENDERER-INTERFACE-BOUNDARY.md` (the 4 documented leaks)

1. **`RenderSetup::makeCurrent()` is GL-context-specific** (~13 call sites
   in `renderSetup.C` + `scene.C`). QRhi has no makeCurrent — it has
   beginFrame/endFrame and command buffers. GL-core still has makeCurrent but
   the verb should move behind the surface either way. **Status (2026-05-15):
   partially mitigated by Phase 02.1's `RenderSurface::beginFrame/endFrame`
   adoption — the leak is contained but not eliminated.**
2. **`scene.C` `dynamic_cast<GLRenderWindow*>` / `dynamic_cast<GLRenderer*>`**
   (~6 sites) + **`new GLRenderWindow(...)`** (main + ~9 deferred stereo
   paths). **Status: Phase 02.1 closed the main-display path; the ~17 deferred
   stereo/multi-display sites are Phase 5 work.**
3. **`GLRenderWindow` fuses surface + context + CPU buffer.** QRhi/GL-core
   surfaces have a different context model. **Status: still fused — PIPE-01
   work.**
4. **`Renderer`'s per-primitive `render*_()` virtuals are immediate-mode
   shaped** (`renderSphere_(const Sphere&)` = one draw call per object).
   Retained-mode backends (QRhi, GL-core VBOs) want batched buffer uploads.
   **Status: Phase 02.1 added the batched `renderRepresentations_()` entry
   point — additive, default fans out to per-primitive. PIPE-01's new backend
   subclass overrides the batched path.**

### Additional surface PIPE-01 will touch

5. **~100 fixed-function GL calls in `glRenderer.C`** — each needs a shader
   replacement (vertex+fragment GLSL via qsb, or raw modern GLSL for GL-core)
   + VBO/VAO management. This is the bulk of the rewrite.
6. **Text overlay — TWO distinct paths in BALL, migrate differently** (deep-dive 2026-05-15):

   **Path 1 — HUD/screen-space text** ([`source/VIEW/RENDERING/glRenderWindow.C:364`](../../../source/VIEW/RENDERING/glRenderWindow.C#L364), Phase 2). Current pattern uses Qt 5's "overpainting": `QPainter painter(this); painter.drawText(x, y, text)` directly on the `QOpenGLWidget` framebuffer after `paintGL()`. This relies on `QOpenGLPaintDevice` — `QPainter`-over-widget. **This path does NOT transfer to QRhi** — there is no equivalent QRhiWidget overpainting mechanism documented anywhere.

   **Path 2 — 3D world-space labels** ([`source/VIEW/RENDERING/RENDERERS/glRenderer.C:1128-1170`](../../../source/VIEW/RENDERING/RENDERERS/glRenderer.C#L1128), `GLRenderer::renderLabel_`). Current pattern is already 90% in the QRhi-friendly shape:
   - Creates `QImage(QImage::Format_ARGB32_Premultiplied)` ← already the format Qt docs flag as fastest for QPainter
   - Renders text via `QPainter::drawText` into the QImage
   - Converts to `Format_RGBA8888.mirrored()`
   - **BUT** positions via `glRasterPos3f` (fixed-function — dead in QRhi and GL-core 3.2+)
   - **AND** draws via `glDrawPixels` (fixed-function — dead in QRhi and GL-core 3.2+)

   **Decision (2026-05-15):**
   - **Path 1 (HUD/screen-space): Strategy D — transparent QWidget overlay. LOCKED.** Rationale: no new dependency (no Qt Quick / QML pulled in), no glyph-atlas infrastructure to build, Qt's docs explicitly support this composition (semi-transparent widgets on top of QRhiWidget). HUD text in BALL is low-update by design (mode indicators, status text, not animated metrics), so Strategy C's higher performance ceiling isn't load-bearing. If a future feature needs a per-frame-animated HUD (live FPS counter, real-time energy readout), revisit B (glyph atlas) or C (Qt Quick overlay) for that specific surface — but the default path stays D.
   - **Path 2 (3D world-space labels): Strategy A — textured-quad rendering. WORKING ASSUMPTION.** The spike verifies it renders correctly via QRhi (manual projection math + textured quad shader replaces `glRasterPos3f` + `glDrawPixels`); only escalates to B if per-label upload cost dominates.

   **Migration strategies (rationale for the decision above):**

   - **Strategy A — Textured-quad rendering** (3D labels, Path 2). Reuse the existing QPainter→QImage step. Replace `glRasterPos3f` with manual projection-matrix math to compute screen-space anchor from the 3D label vertex. Replace `glDrawPixels` with a textured quad: `QRhiTexture` from the QImage via `QRhiResourceUpdateBatch::uploadTexture()`, then draw a 2-triangle quad with a simple textured fragment shader. Dirty-flag cache the texture per-label so re-upload only happens when the label text/font changes. **Verdict: clean fit, low risk, BALL is already 90% of the way there.**

   - **Strategy B — Glyph atlas** (any text path, highest performance). Pre-render glyphs at common sizes into a large `QRhiTexture` atlas. Per-text: build a VBO of one textured-quad-per-glyph. Single draw call per label, optional batch-multi-label draw calls. **Verdict: best performance ceiling, but Qt's internal atlas isn't exposed — BALL would roll its own or vendor a third-party (e.g. `msdf-atlas-gen`). Defer until profiling shows per-label upload cost matters.**

   - **Strategy C — QQuickWidget overlay** (HUD, Path 1). Compose a sibling `QQuickWidget` over the `QRhiWidget` with `Text { … }` Qt Quick items for HUD content. Qt Quick uses QRhi internally and already has a production-grade glyph atlas. **Constraint:** the "single API per window" rule applies to BOTH widgets — they must agree on backend (both Metal on macOS, both D3D11 on Windows). Doable since BALL picks one backend anyway. **Verdict: best HUD performance with least new code, but adds Qt Quick/QML dependency to a project that has none today.**

   - **Strategy D — Transparent QWidget overlay** (HUD, Path 1). Place a normal transparent `QWidget` over the `QRhiWidget` and use `paintEvent` + `QPainter` for HUD text. Qt's widget compositor handles the overlay. The QRhiWidget docs explicitly note: *"some widgets, with semi-transparency even, can be placed on top of the QRhiWidget"*. **Verdict: simplest HUD migration, no new dependency, may be lower-performance than Strategy C (CPU-rendered QPainter onto a software-composited overlay) but acceptable for low-update HUDs (FPS counter, mode indicator).**

   **Spike's job on text overlay:** verify the locked Strategy D HUD path actually composites correctly on Metal (macOS), D3D11 (Windows), and Vulkan/OpenGL (Linux) — the single-API-per-window rule only constrains stacked QRhi-rendering widgets, but a non-rendering transparent QPainter overlay shouldn't trip it. Confirm focus/event routing through the transparent overlay reaches the QRhiWidget underneath (mouse/keyboard in the 3D scene must still work). And verify Strategy A handles the world-space label workload at the bpti.pdb baseline.

   **Known risks specific to text rendering:**
   - **Per-frame QPainter is a known bottleneck** — Qt docs explicitly say *"QPainter is currently designed for software rendering, will likely be a bottleneck in a graphics-intensive project"*. Must use dirty-flag caching everywhere; HUDs that animate per-frame (live FPS counter) need a different path (e.g. pre-rendered digit atlas).
   - **`QImage::Format` matters** — `Format_ARGB32_Premultiplied`, `Format_RGB32`, `Format_RGB16` are the fast paths; plain `Format_ARGB32` is documented as significantly slower. BALL's current code uses `Format_ARGB32_Premultiplied` for labels ✓ — keep.
   - **Glyph cache corruption on FBO readback** is a known historical Qt bug (env var `QML_USE_GLYPHCACHE_WORKAROUND` exists); unlikely to hit BALL but worth knowing.
   - **`glRasterPos` + `glDrawPixels` removal** is the deepest source-level change — affects every world-space label site, not just Path 2 above. Need to audit for any other callers.
7. **Picking** — currently a color-buffer FBO approach in
   [`source/VIEW/RENDERING/RENDERERS/glRenderer.C`](../../../source/VIEW/RENDERING/RENDERERS/glRenderer.C).
   The algorithm is backend-agnostic (render-to-FBO, read pixel). QRhi supports
   it via `QRhiRenderTarget` + readback; GL-core supports it natively. **Low
   risk — same idiom either way.**
8. **Threading** — Phase 2 already aligned BALL with GUI-thread rendering
   (raytracer is a CPU-buffer worker, no GL on worker threads). QRhiWidget
   restricts `initialize()` and `render()` to the GUI thread — aligns with
   BALL's current model. **Low risk.**
9. **Offscreen rendering** — `GLOffscreenTarget` + raytracer CPU buffer +
   `-export-png` smoke check (Phase 02.2). PIPE-01 must keep these working.
   QRhi has `QRhiRenderTarget`; GL-core has FBO. **Idiom transfers; effort
   in the implementation.**
10. **Stereo / multi-display** — Phase 5 routes the ~17 deferred sites through
    RendererFactory. QRhi has a "single API per window" rule — each
    QRhiWidget binds to one backend. For stereo with two windows, each gets
    its own QRhi instance/backend. **Constraint to verify in the spike;
    possibly requires per-window backend selection logic.**

## Backend candidates — research summary (do not decide here)

### Option A: QRhi (Qt's Rendering Hardware Interface)

**What it is:** Qt's cross-API abstraction. One C++ API; runtime picks a
backend (Metal on macOS, D3D11/12 on Windows, Vulkan/OpenGL on Linux). Shaders
authored in Vulkan-flavour GLSL, compiled via Qt Shader Tools' `qsb` to
SPIR-V + transpiled to HLSL/MSL/GLSL.

**Pros:**
- Single per-OS code path. Removes the "OpenGL is dead on macOS" risk
  permanently.
- Aligns with Qt's strategic direction — Qt Quick 3D, Qt Quick Scene Graph,
  and Qt 3D have all moved onto QRhi.
- macOS uses Metal (native, well-supported). Windows uses D3D11/12 (native,
  best driver support). Linux uses Vulkan or OpenGL.
- `QRhiWidget` (Qt 6.7+) is the portable equivalent of `QOpenGLWidget` —
  drop-in widget shape, just a different render API inside.

**Cons:**
- **No source/binary compat guarantee** for QRhi family classes (Qt docs are
  explicit). Source-incompat changes limited to minor releases, but still a
  risk for a long-lived library like BALL.
- Requires **Qt 6.7+** for `QRhiWidget` — bumps Phase 5's Qt floor (currently
  Qt 6.5 per SEED-001) up to 6.7. Likely fine since 6.8 is the current LTS.
- **Shader workflow is new for BALL** — qsb CMake integration, shader
  compilation as a build step, shader versioning across renderer revisions.
  Phase 4's CMake presets already handle this kind of pattern but new tooling
  to learn.
- **QPainter interop for text overlays is not documented** — requires
  workaround (texture upload or scene-graph overlay).
- **"Single API per window" rule** for QRhiWidget — verify it supports
  multi-display stereo.

**Maturity:** Qt 6.0 introduced QRhi internally (private API). Qt 6.6 made
QRhi "semi-public". Qt 6.7 added QRhiWidget. Qt 6.8 LTS is current. Production
use exists in Qt Quick / Qt Quick 3D / Qt Quick Scene Graph (so it's not
greenfield — but BALL-shaped molecular viz isn't a known case study).

### Option B: GL-core (modern programmable-pipeline OpenGL 3.3+ / 4.x)

**What it is:** Modern OpenGL only — VAOs, VBOs, GLSL shaders, no
fixed-function, no GLU. Same OpenGL API everywhere, but with the constraint
that macOS is stuck at 4.1 and Apple has deprecated it.

**Pros:**
- **Smaller rewrite at the surface** — `QOpenGLWidget` keeps working as the
  surface; only the renderer internals change (shaders + VBOs instead of
  glBegin/glEnd).
- Same shader source on all 3 OSes (no qsb transpilation), `QOpenGLShader`
  / `QOpenGLShaderProgram` for compile/link.
- No new Qt module dependency (Qt Shader Tools isn't needed).
- **`QPainter` integration is documented and supported** for QOpenGLWidget —
  text overlays stay easy.

**Cons:**
- **macOS OpenGL is dead-end.** Apple Silicon support is not guaranteed
  long-term. PIPE-01 on GL-core potentially needs a re-port to Metal/QRhi in
  a few years anyway.
- macOS caps out at OpenGL 4.1 (2010 spec) — no compute shaders, limited
  modern features. Acceptable for BALL's needs (sphere/stick/ribbon rendering
  doesn't need compute) but a hard ceiling.
- Windows OpenGL driver quality is uneven on lower-end GPUs.
- Linux OpenGL is fine.

**Maturity:** Modern OpenGL has been the only OpenGL since ~2009. Mature
everywhere. The risk is platform direction (Apple), not code-base maturity.

### Open questions for Phase 5 SPIKE-02 to resolve

(Captured here so the spike scope doesn't lose them — the spike answers them,
not this doc.)

1. **Which backend** — QRhi or GL-core? Or QRhi-default-with-GL-core-fallback?
2. **macOS acceptance bar** — does fixed-function OpenGL on macOS still render
   correctly post-Phase-5 across Apple Silicon GPUs? If yes for 1-2 years,
   GL-core might be acceptable. If marginal, QRhi-via-Metal is the
   forcing function.
3. **Text overlay strategy — DECIDED 2026-05-15.** HUD/screen-space text locked to Strategy D (transparent QWidget overlay with QPainter); 3D world-space labels assumed Strategy A (textured-quad), spike verifies. Surface analysis at touch point #6 above. Spike validates: (a) Strategy D composites correctly through QRhiWidget on all 3 OSes; (b) focus/event routing through the transparent overlay reaches the underlying 3D scene; (c) Strategy A meets FPS baseline on bpti.pdb. Escalation paths (B, C) documented for revisit if a future feature needs per-frame-animated HUD.
4. **Picking** — color-buffer FBO (current idiom) or 32-bit integer color
   attachment (more robust, supported in QRhi via `QRhiTexture` formats)?
5. **Stereo / multi-display** — does QRhiWidget's "single API per window"
   rule constrain BALL's stereo paths, or is per-window backend selection
   acceptable?
6. **CMake integration** — qsb shader compilation as a CMake custom_command
   per shader, or as a single Qt Shader Tools `qt_add_shaders` target?
7. **Migration cadence** — big-bang swap (drop GLRenderer, ship coreGLRenderer
   or rhiRenderer), or coexistence (`BALL_RENDERER` build option, default
   stays on legacy until N+1 release)?
8. **Performance baseline** — what's the FPS / frame-time floor on a known
   demo molecule (e.g. `bpti.pdb` Phase 02.2 smoke fixture) at typical
   viewport sizes? Spike measures both backends against this baseline on
   macOS arm64 + Windows D3D11/Vulkan.

## TODOs (backend-independent prerequisites)

These are known concrete tasks that hold regardless of whether the spike picks QRhi or GL-core. They are *not* the PIPE-01 task list (SPIKE-02 still owns that), but they will land *inside* PIPE-01 (or as a small dedicated phase ahead of it) because they have to be true before the rewrite can start.

- [ ] **Qt 6.5 LTS → Qt 6.8 LTS migration** — Phase 5 lands on Qt 6.5 (the SEED-001 minimum for Milestone 2 "BALLView Refresh"). PIPE-01 requires Qt 6.8 LTS:
  - **If QRhi wins:** Qt 6.7 added `QRhiWidget`; Qt 6.8 LTS is the current stable LTS with the longest support window — production-grade target.
  - **If GL-core wins:** bump still pays off — Qt 6.8 LTS has the most mature OpenGL bindings, fewer deprecation surprises, longer security-support runway.
  - **Surface to expect:** vcpkg `qtbase` version pin bump (currently Phase 4 sets it at the Qt 5.15 era; Phase 5 bumps to Qt 6.5; this bumps to 6.8); Homebrew `qt@6` is already at 6.8.x so macOS is essentially free; Linux distro packages vary (Ubuntu 24.04 LTS ships 6.4, so vcpkg / aqtinstall may be needed on Linux CI too); `CMakePresets.json` Qt-finder version constraints; any deprecation warnings introduced between 6.5 → 6.8 (typically minimal across an LTS-to-LTS hop).
  - **Risk:** low. LTS-to-LTS Qt 6 hops are designed to be source-compatible. The cost is mostly CI dep-provisioning + presets, not source-code change.
  - **Sequencing:** lands either as the first wave of PIPE-01 itself, or as a tiny dedicated sub-phase between Phase 5 and PIPE-01 (e.g. `999.6.1` or `5.2`) — SPIKE-02 decides which is cleaner.

## Acceptance criteria (filled in by Phase 5 SPIKE-02 — empty here)

**TBD by the spike.** Expected shape (from Phase 5 success criterion #4-5):

- BALLView renders the standard demo molecule via PIPE-01's new backend with
  no visible regression vs. the Phase-2 fixed-function baseline (same
  framebuffer → pixel-similarity test, allowing for legitimate gamma/precision
  drift).
- Picking still works (clicking on an atom selects that atom).
- Text overlay still renders correctly.
- macOS arm64 + Linux x64 + Windows x64 all render and pass the smoke check.
- Render performance on the bpti.pdb baseline is ≥ N% of the fixed-function
  baseline (threshold set by spike).
- Stereo / multi-display still works on at least one platform (per Phase 5's
  closing of those deferred sites).

## References

- [`RENDERER-INTERFACE-BOUNDARY.md`](../../RENDERER-INTERFACE-BOUNDARY.md) — Phase 02.1's boundary design + 4 leaks
- [`REQUIREMENTS.md`](../../REQUIREMENTS.md) — SPIKE-01 / SPIKE-02 / QT6-01 / QT6-02
- [`ROADMAP.md`](../../ROADMAP.md) — Phase 5 detail
- [`PROJECT.md`](../../PROJECT.md) — `PIPE-01` listed as v2/out-of-scope
- [`seeds/SEED-001-ballview-refresh-ui-milestone.md`](../../seeds/SEED-001-ballview-refresh-ui-milestone.md) — Milestone 2 UI Refresh (gated on Phase 5 + this)

## Research provenance (2026-05-15)

External sources consulted via WebSearch + WebFetch:

- [Qt 6 QRhi class docs](https://doc.qt.io/qt-6/qrhi.html) — backend model, command buffers, resource lifetime
- [Qt 6 QRhiWidget class docs](https://doc.qt.io/qt-6/qrhiwidget.html) — Qt 6.7+ widget shape, GUI-thread-only render(), single-API-per-window rule, limited compatibility guarantees
- [Graphics in Qt 6.0: QRhi, Qt Quick, Qt Quick 3D](https://www.qt.io/blog/graphics-in-qt-6.0-qrhi-qt-quick-qt-quick-3d) — strategic direction (Qt Quick / Qt 3D moved onto QRhi)
- [basysKom — Hello, RHI getting-started guide](https://www.basyskom.de/en/hello-rhi-how-to-get-started-with-qt-rhi/) — scaffolding volume (~350 lines for hello-triangle), qsb workflow, beginner gotchas, semi-public in Qt 6.6
- [QSB Manual / Qt Shader Tools](https://doc.qt.io/qt-6/qtshadertools-qsb.html) — shader compilation pipeline (GLSL → SPIR-V → HLSL/MSL transpilation)
- [Apple Developer — OpenGL deprecation](https://developer.apple.com/forums/thread/725247) — macOS OpenGL stuck at 4.1, deprecated since 10.14, Apple Silicon support not guaranteed long-term
- [OGLDev tutorial 29 — 3D color-buffer picking](https://ogldev.org/www/tutorial29/tutorial29.html) — picking algorithm (backend-agnostic)
- [Qt RHI Window Example](https://www.idedoc.com/archive/cpp/qt/Qt-6.7.2/qtgui/qtgui-rhiwindow-example.html) — minimal QRhi scaffolding reference

**Text overlay deep-dive (2026-05-15):**
- [Cube RHI Widget Example](https://doc.qt.io/Qt-6/qtwidgets-rhi-cuberhiwidget-example.html) — the documented QPainter→QImage→QRhiTexture pattern (dirty-flag cached, `QRhiResourceUpdateBatch::uploadTexture()`); explicit note that *"some widgets, with semi-transparency even, can be placed on top of the QRhiWidget"* (Strategy D)
- [QRhiTexture / QRhiTextureUploadDescription docs](https://doc.qt.io/qt-6/qrhitextureuploaddescription.html) — batched-upload support for glyph-atlas pattern (Strategy B)
- [QQuickWidget docs](https://doc.qt.io/qt-6/qquickwidget.html) — Strategy C overlay path; "single API per window" constraint applies to both QQuickWidget and QRhiWidget when stacked
- [QPainter performance note (Qt Forum)](https://forum.qt.io/topic/160428/performance-of-qpainter) — software-rendering bottleneck warning; `Format_ARGB32_Premultiplied` / `Format_RGB32` / `Format_RGB16` are the fast formats
- [QImage class docs](https://doc.qt.io/qt-6/qimage.html) — format performance ordering; `Format_ARGB32` is significantly slower than the premultiplied variant

No molecular-viz-specific QRhi case study found in the searches. BALL would be doing something close to net-new in this domain on QRhi.
