---
phase: 05-qt-6-migration-4b-renderer-backend-decision-spike
plan: 05
type: execute
wave: 4
depends_on: [01, 02, 03, 04]
files_modified:
  - include/BALL/VIEW/RENDERING/rendererFactory.h
  - source/VIEW/RENDERING/rendererFactory.C
  - include/BALL/VIEW/RENDERING/RENDERERS/coreGLRenderer.h
  - source/VIEW/RENDERING/RENDERERS/coreGLRenderer.C
  - source/VIEW/RENDERING/RENDERERS/sources.cmake
  - CMakeLists.txt
  - .github/workflows/ci.yml
autonomous: true
requirements: [SPIKE-01]
must_haves:
  truths:
    - "RendererFactory::Kind has a new value OpenGL_Core, guarded by #ifdef BALL_SPIKE_BACKEND_GLCORE so the production default is unchanged"
    - "BALLView built with -DBALL_SPIKE_BACKEND=GLCore renders the demo molecule via the new CoreGLRenderer (non-blank PNG on macOS smoke check)"
    - "CoreGLRenderer overrides Renderer::renderRepresentations_() and Renderer::capabilities(); it does NOT implement the immediate-mode per-primitive virtuals"
    - "Color-buffer FBO picking is implemented (CoreGLRenderer::pickObjects() reads from a picking FBO via glReadPixels) — the deliverable's picking demonstration"
    - "Text overlay renders via QPainter on the QOpenGLWidget surface (reuses Phase 2 pattern; demonstration of SPIKE-01 text-overlay deliverable)"
    - "Spike backend is NEVER the production default: scene.C / RendererFactory still construct Kind::OpenGL_Fixed unless BALL_SPIKE_BACKEND=GLCore is passed at configure"
    - "Throwaway provenance is in every new file: header comment says 'THROWAWAY SPIKE — Phase 5 prototype only. Not production code. Remove before PIPE-01.'"
  artifacts:
    - path: "include/BALL/VIEW/RENDERING/rendererFactory.h"
      provides: "RendererFactory::Kind extended with OpenGL_Core under spike ifdef"
      contains: "OpenGL_Core"
    - path: "source/VIEW/RENDERING/rendererFactory.C"
      provides: "makeRenderer/makeSurface switch cases for spike Kinds"
      contains: "BALL_SPIKE_BACKEND_GLCORE"
    - path: "include/BALL/VIEW/RENDERING/RENDERERS/coreGLRenderer.h"
      provides: "CoreGLRenderer class declaration (Renderer subclass)"
      contains: "class BALL_VIEW_EXPORT CoreGLRenderer"
      min_lines: 30
    - path: "source/VIEW/RENDERING/RENDERERS/coreGLRenderer.C"
      provides: "CoreGLRenderer impl: VAO/VBO upload, GLSL shader pair, color-buffer picking FBO, QPainter text overlay"
      contains: "renderRepresentations_"
      min_lines: 150
    - path: "source/VIEW/RENDERING/RENDERERS/sources.cmake"
      provides: "coreGLRenderer.C registered in SOURCES_LIST"
      contains: "coreGLRenderer.C"
    - path: "CMakeLists.txt"
      provides: "BALL_SPIKE_BACKEND option (OFF default); -DBALL_SPIKE_BACKEND_GLCORE define plumbed through BALL_PROJECT_COMPILE_DEFNS when set"
      contains: "BALL_SPIKE_BACKEND"
  key_links:
    - from: "source/VIEW/RENDERING/rendererFactory.C"
      to: "CoreGLRenderer"
      via: "case Kind::OpenGL_Core: return new CoreGLRenderer; (ifdef-guarded)"
      pattern: "new CoreGLRenderer"
    - from: "CMakeLists.txt option"
      to: "BALL_PROJECT_COMPILE_DEFNS"
      via: "IF (BALL_SPIKE_BACKEND STREQUAL GLCore) ... LIST(APPEND ... -DBALL_SPIKE_BACKEND_GLCORE)"
      pattern: "BALL_SPIKE_BACKEND_GLCORE"
    - from: ".github/workflows/ci.yml"
      to: "spike smoke variant (non-blocking, macOS only)"
      via: "extra cmake configure + build + smoke-check step with -DBALL_SPIKE_BACKEND=GLCore and continue-on-error: true"
      pattern: "BALL_SPIKE_BACKEND=GLCore"
---

<objective>
Land the GL-core profile spike backend behind the Phase 02.1 `Renderer` / `RenderSurface` / `RendererFactory` boundary, with picking and text overlay. This is the SPIKE-01 deliverable for the GL-core candidate.

Purpose: One of the two backend prototypes the SPIKE-02 decision record (Plan 08) needs real comparative data for. The plan is deliberately a throwaway — every new file carries the "THROWAWAY SPIKE — Phase 5 prototype only. Not production code. Remove before PIPE-01." header. The spike is opted in via a CMake option (`-DBALL_SPIKE_BACKEND=GLCore`), never via a runtime toggle, so production builds NEVER hit it. The new `Kind::OpenGL_Core` value is `#ifdef`-guarded so it does not appear in default builds.

Output: 7 files modified/created. A `cmake --preset ci-macos -DBALL_SPIKE_BACKEND=GLCore` build produces a BALLView binary that defaults to the spike renderer (via a one-line override in scene.C registration that is itself ifdef-guarded — see action below) and the smoke check produces a non-blank PNG through the GL-core path.

CRITICAL: This plan deliberately keeps the spike opt-in at CMake configure time only. There is no GUI toggle. There is no runtime selection. The spike is invoked only when `BALL_SPIKE_BACKEND=GLCore` is passed. Production builds construct `Kind::OpenGL_Fixed` exactly as today.
</objective>

<execution_context>
@$HOME/.claude/get-shit-done/workflows/execute-plan.md
@$HOME/.claude/get-shit-done/templates/summary.md
</execution_context>

<context>
@.planning/STATE.md
@.planning/RENDERER-INTERFACE-BOUNDARY.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-CONTEXT.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-RESEARCH.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-PATTERNS.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-VALIDATION.md
@include/BALL/VIEW/RENDERING/rendererFactory.h
@source/VIEW/RENDERING/rendererFactory.C
@include/BALL/VIEW/RENDERING/RENDERERS/renderer.h
@source/VIEW/RENDERING/RENDERERS/glRenderer.C
@source/VIEW/RENDERING/RENDERERS/sources.cmake

<interfaces>
<!-- Spike contract — derived from RENDERER-INTERFACE-BOUNDARY.md + 05-PATTERNS.md §rendererFactory.h §rendererFactory.C §coreGLRenderer.h §coreGLRenderer.C. -->

rendererFactory.h — extend `enum class Kind` (lines 35-42 per 05-PATTERNS.md):
  Add `OpenGL_Core` value behind `#ifdef BALL_SPIKE_BACKEND_GLCORE`. Doc comment: "THROWAWAY: GL 3.2+ core-profile renderer spike (Phase 5)".

rendererFactory.C — extend the `makeRenderer` and `makeSurface` switches (lines 24-77 per 05-PATTERNS.md):
  `makeRenderer`: add `#ifdef BALL_SPIKE_BACKEND_GLCORE case Kind::OpenGL_Core: return new CoreGLRenderer; #endif` (raw pointer per project convention — see Pattern Map §Raw-pointer ownership).
  `makeSurface`: add the matching case returning `new GLRenderWindow(parent)` (spike reuses the QOpenGLWidget surface; a separate core-profile context is NOT required at this scope — the Plan 03 compat-profile context is replaced via a CMake-gated alternative format if BALL_SPIKE_BACKEND_GLCORE is set; see Task 3 action).

CoreGLRenderer class shape (declared in `coreGLRenderer.h`, defined in `coreGLRenderer.C`):
  - Subclass `Renderer` (the abstract base at `include/BALL/VIEW/RENDERING/RENDERERS/renderer.h`).
  - Override `init(Scene&)` — initialise VAOs/VBOs/shader pair on first call.
  - Override `renderRepresentations_(const RepresentationList&)` — retained-mode batched draw; build per-representation VBOs, issue draw calls.
  - Override `capabilities()` — returns `Caps{ retained_mode: true, offscreen: false, picking: true, stereo: false }`.
  - Override `pickObjects(Position x1, y1, x2, y2, std::list<GeometricObject*>&)` — color-buffer FBO + glReadPixels picking (the SPIKE-01 picking-demonstration deliverable).
  - QPainter text overlay: the existing `GLRenderWindow::paintGL()` already drives `QPainter` after `Renderer::render_*` finishes (Phase 2 pattern from `glRenderWindow.C`). The spike reuses this — CoreGLRenderer does not own text rendering; it just leaves the framebuffer in a state where `QPainter` can over-draw cleanly (i.e., GL state restored to defaults at end-of-render).

CMake plumbing in CMakeLists.txt (single new block, near the existing options):
  OPTION(BALL_SPIKE_BACKEND "Renderer backend spike (OFF|GLCore|QRhi) -- throwaway Phase 5 prototype" "OFF")
  IF (BALL_SPIKE_BACKEND STREQUAL "GLCore")
    LIST(APPEND BALL_PROJECT_COMPILE_DEFNS "-DBALL_SPIKE_BACKEND_GLCORE")
    MESSAGE(STATUS "Phase 5 spike: GL-core renderer ENABLED (throwaway; not for release builds)")
  ENDIF()
  # Plan 06 will add the QRhi arm of the same option.

sources.cmake — add `coreGLRenderer.C` to SOURCES_LIST (one filename, no path prefix; headers NOT listed). Per Pattern Map §sources.cmake registration.

CI variant — add a non-blocking macOS-only step in ci.yml that configures with `-DBALL_SPIKE_BACKEND=GLCore`, builds BALLView, and runs the smoke check. `continue-on-error: true` (D-VAL Wave 0 gap). This step PROVES SPIKE-01 (renders demo molecule via spike Kind) on every CI push that touches the spike.

Spike override of the production default (CRITICAL constraint to keep spike off the default code path):
  - Production builds (`BALL_SPIKE_BACKEND=OFF`, the default) MUST continue to instantiate `Kind::OpenGL_Fixed` in scene.C exactly as today. ZERO change.
  - Spike builds (`-DBALL_SPIKE_BACKEND=GLCore`) need a way to actually USE the new Kind, otherwise the spike never runs. Options:
    a) Touch scene.C registerRenderers_() with an `#ifdef BALL_SPIKE_BACKEND_GLCORE` block that swaps the constructed Kind. ✗ — re-touches scene.C, defeating the Phase 02.1 containment promise.
    b) Make `RendererFactory::makeRenderer(Kind::OpenGL_Fixed)` return a `CoreGLRenderer` when `BALL_SPIKE_BACKEND_GLCORE` is defined. ✗ — confusing semantics, masks the Kind.
    c) Add an environment-variable gate inside `RendererFactory::makeRenderer` that, IF `BALL_SPIKE_BACKEND_GLCORE` is compiled in AND `BALLVIEW_USE_SPIKE_BACKEND=1` is set at runtime, returns the spike renderer instead of GLRenderer. ✓ — opt-in is compile + env; production paths never see it; scene.C is untouched.
  Choose option (c). See Task 2 action for the exact wiring.

Header-comment provenance — every new file (coreGLRenderer.h, coreGLRenderer.C) starts with:
  // -*- Mode: C++; tab-width: 2; -*-
  // vi: set ts=2:
  //
  // THROWAWAY SPIKE — Phase 5 prototype only. Not production code. Remove before PIPE-01.
</interfaces>
</context>

<tasks>

<task type="auto" tdd="false">
  <name>Task 1: Add the BALL_SPIKE_BACKEND CMake option + extend RendererFactory::Kind</name>
  <files>CMakeLists.txt, include/BALL/VIEW/RENDERING/rendererFactory.h</files>
  <read_first>
    - CMakeLists.txt (read the area around the post-Plan-01 Qt6 block — the Phase 5 option belongs near other BALL_* options, NOT inside `IF(BALL_HAS_VIEW)`).
    - include/BALL/VIEW/RENDERING/rendererFactory.h (full file — short, &lt; 60 lines).
    - 05-PATTERNS.md §`include/BALL/VIEW/RENDERING/rendererFactory.h` (verbatim before/after with the spike Kinds under `#ifdef BALL_SPIKE_BACKEND_GLCORE` / `_QRHI`).
    - 05-PATTERNS.md §Shared Patterns §`LIST(APPEND ...)` for CMake variable accumulation (Phase 04-04 lesson — never use `SET("..." string)` for the compile-defns list).
    - .planning/RENDERER-INTERFACE-BOUNDARY.md §`RendererFactory` (the boundary doc — Phase 5 swap is "one new case + new files").
  </read_first>
  <behavior>
    - CMakeLists.txt declares `OPTION(BALL_SPIKE_BACKEND "..." "OFF")` plus an `IF (BALL_SPIKE_BACKEND STREQUAL "GLCore")` arm that appends `-DBALL_SPIKE_BACKEND_GLCORE` to `BALL_PROJECT_COMPILE_DEFNS` via `LIST(APPEND ...)` and emits a `MESSAGE(STATUS ...)` line naming the spike as throwaway.
    - rendererFactory.h adds `OpenGL_Core` to `enum class Kind` under `#ifdef BALL_SPIKE_BACKEND_GLCORE`. The QRhi case is added by Plan 06 — leave a placeholder comment line in rendererFactory.h indicating where Plan 06's `Kind::QRhi` block will land (helps avoid ifdef-block merge conflicts).
    - Default builds (BALL_SPIKE_BACKEND=OFF) produce zero functional change — the new option compiles to no compile-defn, the Kind enum body is byte-equivalent to today's.
  </behavior>
  <action>
    In CMakeLists.txt, add a new option block immediately after the existing `BALL_HAS_VIEW` / `BALL_DEBUG_*` options (search for `OPTION(BALL_HAS_VIEW` to locate the conventional position). Use the canonical pattern from `<interfaces>`. The `LIST(APPEND BALL_PROJECT_COMPILE_DEFNS "-DBALL_SPIKE_BACKEND_GLCORE")` line is load-bearing — DO NOT use `SET(...)` (Phase 04-04 lesson).

    In include/BALL/VIEW/RENDERING/rendererFactory.h, locate `enum class Kind { ... }` (lines ~35-42). Add the new value at the end of the enum body, behind `#ifdef BALL_SPIKE_BACKEND_GLCORE`, with the Doxygen comment shown in `<interfaces>`. Immediately below the GLCore ifdef, add a placeholder C++ comment `// (Plan 06 inserts BALL_SPIKE_BACKEND_QRHI block here)` so the next plan's ifdef block has a clean landing site.

    Match project conventions: modeline at top of any new file (none here — both files exist); `BALL_EXPORT` macro on exported types if relevant (Kind enum is not a class, no export macro needed); 2-space indent, Allman braces.

    Do NOT modify scene.C. Do NOT modify renderer.h (Phase 02.1's `capabilities()` method is non-pure-virtual per STATE.md — CoreGLRenderer just overrides it). Do NOT add a runtime toggle in this task (that lands in Task 2).
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; grep -q 'OPTION(BALL_SPIKE_BACKEND' CMakeLists.txt &amp;&amp; grep -qE 'LIST\(APPEND BALL_PROJECT_COMPILE_DEFNS "-DBALL_SPIKE_BACKEND_GLCORE"\)' CMakeLists.txt &amp;&amp; grep -q 'OpenGL_Core' include/BALL/VIEW/RENDERING/rendererFactory.h &amp;&amp; grep -q '#ifdef BALL_SPIKE_BACKEND_GLCORE' include/BALL/VIEW/RENDERING/rendererFactory.h &amp;&amp; cmake --preset ci-macos -S . 2>&amp;1 | tee /tmp/cfg-default.log | grep -q 'Configuring done' &amp;&amp; ! grep -q 'BALL_SPIKE_BACKEND_GLCORE' /tmp/cfg-default.log &amp;&amp; cmake --preset ci-macos -S . -DBALL_SPIKE_BACKEND=GLCore 2>&amp;1 | tee /tmp/cfg-spike.log | grep -q 'Phase 5 spike: GL-core renderer ENABLED'</automated>
  </verify>
  <acceptance_criteria>
    - `grep -c 'OPTION(BALL_SPIKE_BACKEND' CMakeLists.txt` returns `>= 1`.
    - `grep -cE 'LIST\(APPEND BALL_PROJECT_COMPILE_DEFNS "-DBALL_SPIKE_BACKEND_GLCORE"\)' CMakeLists.txt` returns `>= 1` (and NOT `SET(...)` — Phase 04-04 lesson).
    - `grep -c '#ifdef BALL_SPIKE_BACKEND_GLCORE' include/BALL/VIEW/RENDERING/rendererFactory.h` returns `>= 1`.
    - `grep -c 'OpenGL_Core' include/BALL/VIEW/RENDERING/rendererFactory.h` returns `>= 1` (under the ifdef).
    - Default configure (`cmake --preset ci-macos -S .`) succeeds AND does NOT log `BALL_SPIKE_BACKEND_GLCORE` (option default is OFF → no compile-defn).
    - Spike configure (`cmake --preset ci-macos -S . -DBALL_SPIKE_BACKEND=GLCore`) succeeds AND logs the `MESSAGE(STATUS)` "Phase 5 spike: GL-core renderer ENABLED" line.
    - Default build still produces BALLView identical in behaviour to post-Plan-04 baseline (no functional change yet).
  </acceptance_criteria>
  <done>
    BALL_SPIKE_BACKEND option exists and toggles the BALL_SPIKE_BACKEND_GLCORE compile-defn cleanly; rendererFactory.h compiles in both default and spike configurations; the new Kind value is invisible in default builds.
  </done>
</task>

<task type="auto" tdd="false">
  <name>Task 2: Implement CoreGLRenderer + wire RendererFactory env-var spike opt-in</name>
  <files>include/BALL/VIEW/RENDERING/RENDERERS/coreGLRenderer.h, source/VIEW/RENDERING/RENDERERS/coreGLRenderer.C, source/VIEW/RENDERING/RENDERERS/sources.cmake, source/VIEW/RENDERING/rendererFactory.C</files>
  <read_first>
    - include/BALL/VIEW/RENDERING/RENDERERS/renderer.h (full file — the abstract base; identify the signatures of `init`, `renderRepresentations_`, `capabilities`, `pickObjects`, the `Caps` struct, and `RepresentationList` typedef).
    - source/VIEW/RENDERING/RENDERERS/glRenderer.C (the existing concrete impl — read `init()` and a representative immediate-mode method to understand the project's GL setup conventions; the spike does NOT inherit any immediate-mode code, it just borrows the init scaffolding pattern).
    - source/VIEW/RENDERING/rendererFactory.C (full file — short; the switch-case shape).
    - source/VIEW/RENDERING/RENDERERS/sources.cmake (full file — the SOURCES_LIST pattern).
    - 05-PATTERNS.md §`include/BALL/VIEW/RENDERING/RENDERERS/coreGLRenderer.h`, §`source/VIEW/RENDERING/RENDERERS/coreGLRenderer.C`, §`source/VIEW/RENDERING/rendererFactory.C`, §sources.cmake registration.
    - 05-RESEARCH.md §GL-Core Backend, §Code Examples §`GL-Core Spike: Color-Buffer Picking Pattern`.
    - .planning/RENDERER-INTERFACE-BOUNDARY.md §"What the Phase 5 swap then looks like" (the 4-step containment recipe).
  </read_first>
  <behavior>
    - coreGLRenderer.h declares `class BALL_VIEW_EXPORT CoreGLRenderer : public Renderer` in the `BALL::VIEW` namespace with overrides for `init`, `renderRepresentations_`, `capabilities`, `pickObjects`. Includes the throwaway-spike header comment. Include guard `BALL_VIEW_RENDERING_RENDERERS_COREGLRENDERER_H`.
    - coreGLRenderer.C implements:
      * `init(Scene&)` — creates the picking FBO, a single GLSL 330 core shader pair (vertex + fragment) sufficient to draw points/lines/triangles with per-vertex color (the demo molecule is rendered as colored spheres/sticks — minimum: render representations as colored point sprites or simple triangle meshes; pixel-perfect parity with `GLRenderer` is NOT a SPIKE-01 acceptance criterion).
      * `renderRepresentations_(const RepresentationList&)` — iterates representations, builds VBO data from each, issues draw calls. Restores GL state (matrix mode, depth test) so the `QPainter` text-overlay path in `GLRenderWindow::paintGL()` (Phase 2) still composites cleanly on top.
      * `capabilities()` — returns `Renderer::Caps{ true, false, true, false }` (retained, picking; no offscreen, no stereo).
      * `pickObjects(Position x1, y1, x2, y2, std::list<GeometricObject*>& objects)` — performs a second pass into the picking FBO with per-object ID-encoded colors, then `glReadPixels(GL_RED_INTEGER, GL_UNSIGNED_INT, ...)` to recover the clicked ID, then looks up the GeometricObject* by ID and appends to `objects`. The ID→object map is built once per render; for the SPIKE-01 demonstration, a single-pixel readback at (x1, y1) is acceptable (no marquee select).
    - sources.cmake adds `coreGLRenderer.C` to SOURCES_LIST (one line, alphabetical or after `glRenderer.C` — match neighbouring style; NO header listed, NO path prefix).
    - rendererFactory.C adds the `#ifdef BALL_SPIKE_BACKEND_GLCORE case Kind::OpenGL_Core: return new CoreGLRenderer; #endif` to both `makeRenderer` and `makeSurface` (the `makeSurface` case returns `new GLRenderWindow(parent)` — spike reuses the existing QOpenGLWidget surface, no new RenderSurface class needed for the GL-core spike).
    - **Runtime override** (the option-c env-var gate from `<interfaces>`): At the top of `makeRenderer(Kind kind)`, BEFORE the switch, add:
        `#ifdef BALL_SPIKE_BACKEND_GLCORE`
        `  if (kind == Kind::OpenGL_Fixed && std::getenv("BALLVIEW_USE_SPIKE_BACKEND") != nullptr) {`
        `    return new CoreGLRenderer;`
        `  }`
        `#endif`
      This makes the spike opt-in at runtime ONLY when (1) `-DBALL_SPIKE_BACKEND=GLCore` was passed at configure AND (2) the user sets `BALLVIEW_USE_SPIKE_BACKEND=1` at launch. Production builds (#ifdef off) compile to a byte-identical factory — the override is gone.
    - Throwaway provenance on every new file's header (modeline + the 4-line SPIKE comment).
  </behavior>
  <action>
    1. **Create `include/BALL/VIEW/RENDERING/RENDERERS/coreGLRenderer.h`** per 05-PATTERNS.md §coreGLRenderer.h. Modeline + throwaway header comment + include guard + `#include <BALL/VIEW/RENDERING/RENDERERS/renderer.h>` + namespace wrapper + class declaration with the four overrides. Use `BALL_VIEW_EXPORT` on the class. Use the exact override signatures from `renderer.h` (read it first).

    2. **Create `source/VIEW/RENDERING/RENDERERS/coreGLRenderer.C`** per 05-PATTERNS.md §coreGLRenderer.C. Modeline + throwaway header comment + includes (`coreGLRenderer.h`, `<QtGui/QOpenGLFunctions_3_2_Core>` or higher, `<cstdlib>` for `getenv`). Implement the four overrides. The GLSL pair can be inlined as a `const char*` (vertex shader: pass-through MVP transform of `vec3 in_pos` and `vec3 in_color`; fragment shader: writes `in_color` for the main pass, writes `vec4(packUint(object_id), 1.0)` for the picking pass — use a uniform branch). Picking FBO: `glGenFramebuffers` + `glGenTextures(GL_R32UI)` + `glFramebufferTexture2D`. The `pickObjects` impl follows the canonical pattern in 05-RESEARCH.md §Color-Buffer Picking. Restore default GL state (matrix mode MODELVIEW, depth test enabled) before returning from `renderRepresentations_()` so the QPainter overlay in `GLRenderWindow::paintGL()` composites cleanly.

    3. **Edit `source/VIEW/RENDERING/RENDERERS/sources.cmake`** — add `coreGLRenderer.C` to SOURCES_LIST. Match neighbouring style (no path prefix; one filename per line; no headers).

    4. **Edit `source/VIEW/RENDERING/rendererFactory.C`** —
       - Add `#include <BALL/VIEW/RENDERING/RENDERERS/coreGLRenderer.h>` under `#ifdef BALL_SPIKE_BACKEND_GLCORE` at the top.
       - In `makeRenderer`, BEFORE the existing switch statement, add the `#ifdef BALL_SPIKE_BACKEND_GLCORE` runtime-env-var override block from `<interfaces>` so `Kind::OpenGL_Fixed` is intercepted only when the env var is set.
       - Inside the switch in both `makeRenderer` and `makeSurface`, add the new `case Kind::OpenGL_Core:` arms behind `#ifdef BALL_SPIKE_BACKEND_GLCORE`. `makeRenderer` returns `new CoreGLRenderer;`; `makeSurface` returns `new GLRenderWindow(parent);` (spike reuses the QOpenGLWidget surface).
       - Add `#include <cstdlib>` for `std::getenv`.

    Anti-rules:
       - Do NOT modify `scene.C`. The Phase 02.1 boundary is the load-bearing reason this plan is &lt; 200 lines of new code; preserve it. See RENDERER-INTERFACE-BOUNDARY.md §"What the Phase 5 swap then looks like".
       - Do NOT add a GUI toggle, menu entry, Preferences pane, or runtime setting beyond the env-var gate.
       - Do NOT add unit tests (RESEARCH §Anti-Patterns §"Writing production unit tests for spike backends").
       - Do NOT modify `Renderer::capabilities()` to be pure-virtual — STATE.md notes it was intentionally left non-pure in Phase 02.1.
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; test -f include/BALL/VIEW/RENDERING/RENDERERS/coreGLRenderer.h &amp;&amp; test -f source/VIEW/RENDERING/RENDERERS/coreGLRenderer.C &amp;&amp; grep -q 'coreGLRenderer.C' source/VIEW/RENDERING/RENDERERS/sources.cmake &amp;&amp; grep -q 'THROWAWAY SPIKE' include/BALL/VIEW/RENDERING/RENDERERS/coreGLRenderer.h source/VIEW/RENDERING/RENDERERS/coreGLRenderer.C &amp;&amp; grep -q 'BALLVIEW_USE_SPIKE_BACKEND' source/VIEW/RENDERING/rendererFactory.C &amp;&amp; cmake --preset ci-macos -S . -DBALL_SPIKE_BACKEND=GLCore 2>&amp;1 | tail -10 &amp;&amp; cmake --build --preset ci-macos --target BALLView -j$(sysctl -n hw.ncpu) 2>&amp;1 | tee /tmp/spike-build.log | tail -30 &amp;&amp; BALLVIEW_USE_SPIKE_BACKEND=1 bash .planning/phases/02.2-ci-and-build-smoke-matrix/scripts/render-smoke-check.sh build/ci-macos/bin/BALLView.app/Contents/MacOS/BALLView 2>&amp;1 | tee /tmp/spike-smoke.log | tail -20 &amp;&amp; cmake --preset ci-macos -S . 2>&amp;1 | tail -5 &amp;&amp; cmake --build --preset ci-macos --target BALLView -j$(sysctl -n hw.ncpu) 2>&amp;1 | tail -5 &amp;&amp; bash .planning/phases/02.2-ci-and-build-smoke-matrix/scripts/render-smoke-check.sh build/ci-macos/bin/BALLView.app/Contents/MacOS/BALLView 2>&amp;1 | tail -5</automated>
  </verify>
  <acceptance_criteria>
    - Both new files exist: `include/BALL/VIEW/RENDERING/RENDERERS/coreGLRenderer.h`, `source/VIEW/RENDERING/RENDERERS/coreGLRenderer.C`.
    - Both new files start with the `THROWAWAY SPIKE — Phase 5 prototype only.` header comment (`grep -c 'THROWAWAY SPIKE'` returns >= 1 on each).
    - `grep -c 'coreGLRenderer.C' source/VIEW/RENDERING/RENDERERS/sources.cmake` returns `>= 1`.
    - `grep -c 'BALLVIEW_USE_SPIKE_BACKEND' source/VIEW/RENDERING/rendererFactory.C` returns `>= 1` (the env-var opt-in gate).
    - `grep -c 'case Kind::OpenGL_Core' source/VIEW/RENDERING/rendererFactory.C` returns `>= 2` (one in makeRenderer, one in makeSurface — both under `#ifdef BALL_SPIKE_BACKEND_GLCORE`).
    - Spike configure (`cmake --preset ci-macos -S . -DBALL_SPIKE_BACKEND=GLCore`) AND build (`cmake --build --preset ci-macos --target BALLView`) both exit 0.
    - With the spike build, `BALLVIEW_USE_SPIKE_BACKEND=1 bash .../render-smoke-check.sh ...BALLView` produces a non-blank PNG (SPIKE-01 deliverable: spike Kind renders demo molecule). Acceptance: PNG file size > 1 KB (the existing smoke check's own non-blank assertion).
    - Default configure + build (no `-DBALL_SPIKE_BACKEND`) still exits 0 AND the resulting binary produces a non-blank PNG via the production `Kind::OpenGL_Fixed` path (regression check — no functional change to default builds).
    - In the spike build with `BALLVIEW_USE_SPIKE_BACKEND` UNSET, the smoke check produces the same non-blank PNG as the default build (env-var gate gated correctly).
    - scene.C is BYTE-IDENTICAL to its post-Plan-02 state (`git diff source/VIEW/WIDGETS/scene.C` shows no Plan 05 changes — boundary preserved per RENDERER-INTERFACE-BOUNDARY.md).
    - No GUI toggle / menu entry / Preferences pane added (`grep -rn 'spike' source/VIEW/DIALOGS source/APPLICATIONS/BALLVIEW 2>/dev/null | grep -vE 'THROWAWAY SPIKE'` returns no UI-wiring lines).
  </acceptance_criteria>
  <done>
    A `-DBALL_SPIKE_BACKEND=GLCore` build + `BALLVIEW_USE_SPIKE_BACKEND=1` env var produces a non-blank rendered PNG of the demo molecule through `CoreGLRenderer` with picking implemented. The default build path is byte-identical to post-Plan-04. scene.C unchanged. SPIKE-01 (GL-core arm) deliverable: ✓ renders, ✓ picks, ✓ text overlay via existing QPainter path.
  </done>
</task>

<task type="auto" tdd="false">
  <name>Task 3: Wire the spike smoke-check variant into ci.yml (non-blocking, macOS only)</name>
  <files>.github/workflows/ci.yml</files>
  <read_first>
    - .github/workflows/ci.yml (read the macOS build + smoke-check steps from Plan 04's edits — the spike variant step lands AFTER the regular smoke check + GL profile assertion).
    - 05-PATTERNS.md §`.github/workflows/ci.yml` (item 4 — "Spike smoke variant step").
    - 05-VALIDATION.md §Wave 0 Requirements (item 4 — "CI spike variant: non-blocking macOS-only step").
  </read_first>
  <behavior>
    - A new step "Spike smoke check (macOS — GLCore backend)" runs AFTER the regular macOS smoke check + GL profile assertion, ONLY when `matrix.os == 'macos-arm64'`, with `continue-on-error: true` (non-blocking — spike CI red does not block PR merge per CONTEXT.md "throwaway spike" framing).
    - The step configures the build tree with `-DBALL_SPIKE_BACKEND=GLCore`, rebuilds BALLView, then invokes the smoke-check script with `BALLVIEW_USE_SPIKE_BACKEND=1` environment override.
    - The step records its output to `/tmp/ballview-spike-smoke.log` so SPIKE-02 (Plan 08) has a referenced artifact when assembling the decision record.
  </behavior>
  <action>
    In `.github/workflows/ci.yml`, after the macOS "Assert GL compat profile" step added by Plan 04, insert a new step:

       - name: Spike smoke check (macOS — GLCore backend)
         if: matrix.os == 'macos-arm64' && matrix.smoke
         continue-on-error: true
         run: |
           cmake --preset ci-macos -S . -DBALL_SPIKE_BACKEND=GLCore
           cmake --build --preset ci-macos --target BALLView -j$(sysctl -n hw.ncpu)
           BALLVIEW_USE_SPIKE_BACKEND=1 bash .planning/phases/02.2-ci-and-build-smoke-matrix/scripts/render-smoke-check.sh \
             build/ci-macos/bin/BALLView.app/Contents/MacOS/BALLView 2>&1 | tee /tmp/ballview-spike-smoke.log

    Then add an `actions/upload-artifact` step (same matrix gate, same continue-on-error) that uploads `/tmp/ballview-spike-smoke.log` as `spike-smoke-log-glcore-macos` so Plan 08 can reference it from the decision record.

    Do NOT add the spike variant on Linux or Windows in this plan — Plan 07 (per-platform driver checks) is where Linux/Windows behaviour gets recorded; the SPIKE-01 deliverable's core "renders + picks + overlay" demonstration is macOS-only at CI level per VALIDATION §Wave 0.
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; grep -q 'Spike smoke check (macOS' .github/workflows/ci.yml &amp;&amp; grep -q 'BALL_SPIKE_BACKEND=GLCore' .github/workflows/ci.yml &amp;&amp; grep -q 'BALLVIEW_USE_SPIKE_BACKEND=1' .github/workflows/ci.yml &amp;&amp; grep -q 'continue-on-error: true' .github/workflows/ci.yml &amp;&amp; python3 -c "import yaml; yaml.safe_load(open('.github/workflows/ci.yml'))"</automated>
  </verify>
  <acceptance_criteria>
    - `grep -c 'Spike smoke check (macOS' .github/workflows/ci.yml` returns `>= 1`.
    - `grep -c 'BALL_SPIKE_BACKEND=GLCore' .github/workflows/ci.yml` returns `>= 1`.
    - `grep -c 'BALLVIEW_USE_SPIKE_BACKEND=1' .github/workflows/ci.yml` returns `>= 1`.
    - The new spike step has `continue-on-error: true` (non-blocking) — verify by `yq` or `python -c "import yaml; w=yaml.safe_load(open('.github/workflows/ci.yml')); ..."` that the step is marked non-blocking.
    - YAML parses cleanly: `python3 -c "import yaml; yaml.safe_load(open('.github/workflows/ci.yml'))"` exits 0.
    - Real CI run: blocking jobs stay green; the new spike variant runs on macOS and reports its outcome (green or red) without gating PR merge (continue-on-error semantics).
    - The uploaded `spike-smoke-log-glcore-macos` artifact contains the smoke-check stdout from the GL-core build.
  </acceptance_criteria>
  <done>
    The GL-core spike has a CI-level demonstration on macOS. PR pushes report whether the spike Kind renders + picks + overlays end-to-end. Plan 08 (SPIKE-02 decision record) can cite the CI artifact instead of relying solely on local screenshots.
  </done>
</task>

</tasks>

<verification>
- `cmake --preset ci-macos -S . -DBALL_SPIKE_BACKEND=GLCore && cmake --build --preset ci-macos --target BALLView` exits 0.
- `BALLVIEW_USE_SPIKE_BACKEND=1 bash .planning/phases/02.2-ci-and-build-smoke-matrix/scripts/render-smoke-check.sh build/ci-macos/bin/BALLView.app/Contents/MacOS/BALLView` exits 0 with a non-blank PNG.
- Default (no `-DBALL_SPIKE_BACKEND`) build still produces a non-blank PNG (regression check).
- `git diff source/VIEW/WIDGETS/scene.C` shows no change in this plan (Phase 02.1 boundary preserved).
- `grep -c 'THROWAWAY SPIKE' include/BALL/VIEW/RENDERING/RENDERERS/coreGLRenderer.h source/VIEW/RENDERING/RENDERERS/coreGLRenderer.C` returns `>= 1` per file.
- CI matrix: blocking jobs green; spike variant step runs (green or red — both acceptable, non-blocking).
- Task 2 is intentionally high-complexity — splitting at the picking-FBO boundary is a documented contingency if execution stalls.
</verification>

<success_criteria>
- SPIKE-01 (GL-core arm) deliverable met: a `-DBALL_SPIKE_BACKEND=GLCore` build with `BALLVIEW_USE_SPIKE_BACKEND=1` renders the demo molecule via `CoreGLRenderer`, implements picking via color-buffer FBO, and preserves the QPainter text overlay path.
- Default builds are byte-identical to post-Plan-04 (no production-path regression).
- Scene.C unchanged — Phase 02.1 boundary held.
- All new files carry the THROWAWAY SPIKE provenance comment.
- CI captures the spike rendering as an uploaded artifact for SPIKE-02 reference.
</success_criteria>

<output>
After completion, create `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-05-SUMMARY.md` with: a diff-stat of the 7 files, the local smoke-check PNG byte count for both default and spike builds, the CI run URL showing the spike variant outcome, the GLSL shader pair (vertex + fragment) as inlined in `coreGLRenderer.C`, and a 1-paragraph engineer's note on any tradeoffs hit during impl (e.g., MSAA on/off with the picking FBO, restoration of GL state for the QPainter overlay). This SUMMARY is one of Plan 08's primary inputs.
</output>
