---
phase: 05-qt-6-migration-4b-renderer-backend-decision-spike
plan: 06
type: execute
wave: 5
depends_on: [05]
files_modified:
  - include/BALL/VIEW/RENDERING/rendererFactory.h
  - source/VIEW/RENDERING/rendererFactory.C
  - include/BALL/VIEW/RENDERING/RENDERERS/rhiRenderer.h
  - source/VIEW/RENDERING/RENDERERS/rhiRenderer.C
  - include/BALL/VIEW/RENDERING/qtRhiSurface.h
  - source/VIEW/RENDERING/qtRhiSurface.C
  - source/VIEW/RENDERING/sources.cmake
  - source/VIEW/RENDERING/RENDERERS/sources.cmake
  - CMakeLists.txt
autonomous: true
requirements: [SPIKE-01]
must_haves:
  truths:
    - "RendererFactory::Kind has a new value QRhi, guarded by #ifdef BALL_SPIKE_BACKEND_QRHI"
    - "Wrapped under #if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0) — QRhiWidget is unavailable on older Qt and a clear CMake-time MESSAGE(WARNING ...) explains this fallback"
    - "BALLView built with -DBALL_SPIKE_BACKEND=QRhi renders the demo molecule via QRhiRenderer + QtRhiSurface (non-blank PNG on macOS smoke check, Metal backend)"
    - "Spike backend is NEVER the production default and is NEVER in the GUI — same env-var gate pattern as Plan 05 (BALLVIEW_USE_SPIKE_BACKEND=1)"
    - "Throwaway provenance in every new file: 'THROWAWAY SPIKE — Phase 5 prototype only. Not production code. Remove before PIPE-01.'"
    - "SPIKE-01 text overlay demonstrated via QRhi path; picking sub-criterion is satisfied by Plan 05 GL-core arm per accepted deferred caveat in 05-RESEARCH.md §QRhi Backend cons."
    - "If the resolved Qt version is < 6.7 at configure time with -DBALL_SPIKE_BACKEND=QRhi, the build emits a clear MESSAGE(WARNING ...) directing the user to GL-core spike instead, and the QRhi compile defn is NOT set"
  artifacts:
    - path: "include/BALL/VIEW/RENDERING/rendererFactory.h"
      provides: "Kind::QRhi enum value under #ifdef BALL_SPIKE_BACKEND_QRHI"
      contains: "QRhi"
    - path: "source/VIEW/RENDERING/rendererFactory.C"
      provides: "makeRenderer/makeSurface switch cases for Kind::QRhi"
      contains: "BALL_SPIKE_BACKEND_QRHI"
    - path: "include/BALL/VIEW/RENDERING/RENDERERS/rhiRenderer.h"
      provides: "QRhiRenderer class — Renderer subclass overriding renderRepresentations_ and capabilities"
      contains: "class BALL_VIEW_EXPORT QRhiRenderer"
      min_lines: 30
    - path: "source/VIEW/RENDERING/RENDERERS/rhiRenderer.C"
      provides: "QRhiRenderer impl using QRhi command buffer + QRhiBuffer + QShader"
      contains: "QRhi"
      min_lines: 120
    - path: "include/BALL/VIEW/RENDERING/qtRhiSurface.h"
      provides: "QtRhiSurface class — RenderSurface impl wrapping QRhiWidget"
      contains: "class BALL_VIEW_EXPORT QtRhiSurface"
      min_lines: 30
    - path: "source/VIEW/RENDERING/qtRhiSurface.C"
      provides: "QtRhiSurface impl (QRhiWidget subclass; beginFrame/endFrame/nativeHandle)"
      contains: "QRhiWidget"
      min_lines: 60
    - path: "CMakeLists.txt"
      provides: "GLCore + QRhi arms of BALL_SPIKE_BACKEND; QRhi arm gated on Qt6Version >= 6.7"
      contains: "BALL_SPIKE_BACKEND_QRHI"
  key_links:
    - from: "source/VIEW/RENDERING/rendererFactory.C makeRenderer"
      to: "QRhiRenderer"
      via: "case Kind::QRhi: return new QRhiRenderer; (ifdef-guarded)"
      pattern: "new QRhiRenderer"
    - from: "source/VIEW/RENDERING/rendererFactory.C makeSurface"
      to: "QtRhiSurface"
      via: "case Kind::QRhi: return new QtRhiSurface(parent); (ifdef-guarded)"
      pattern: "new QtRhiSurface"
    - from: "CMakeLists.txt"
      to: "BALL_PROJECT_COMPILE_DEFNS -DBALL_SPIKE_BACKEND_QRHI"
      via: "IF (BALL_SPIKE_BACKEND STREQUAL QRhi AND Qt6Core_VERSION VERSION_GREATER_EQUAL 6.7) ... LIST(APPEND ...)"
      pattern: "BALL_SPIKE_BACKEND_QRHI"
---

<objective>
Land the QRhi backend spike — the second of the two SPIKE-01 prototypes. QRhi is Qt 6's cross-API rendering abstraction (Metal on macOS / D3D11/12 on Windows / Vulkan or GL on Linux). At the D-01 floor (Qt 6.5) QRhiWidget is unavailable; the resolved Homebrew `qt` is 6.11 (verified — Phase 5 plan-time `brew info qt`) so the spike is feasible. The plan is gated to `QT_VERSION >= 6.7` at both compile time AND CMake time — if a contributor builds against an older Qt, they get a clear `MESSAGE(WARNING ...)` directing them to the GL-core spike (Plan 05).

Purpose: Gives the SPIKE-02 decision record (Plan 08) real comparative data for QRhi alongside the GL-core arm — the CONTEXT.md Claude's-discretion item ("Recommendation: prototype BOTH so the SPIKE-02 decision record can compare with real data") materialised. The implementation is throwaway (header comment on every file); it never appears in production builds (no GUI toggle, env-var-gated opt-in only).

Output: 9 files modified/created (5 new: rhiRenderer.{h,C}, qtRhiSurface.{h,C}, plus the new RENDERING/sources.cmake entry for qtRhiSurface.C). Adds the QRhi arm to the existing `BALL_SPIKE_BACKEND` option introduced by Plan 05; rendererFactory gets one more case in each switch.
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
@include/BALL/VIEW/RENDERING/renderSurface.h
@include/BALL/VIEW/RENDERING/RENDERERS/renderer.h
@include/BALL/VIEW/RENDERING/rendererFactory.h
@source/VIEW/RENDERING/rendererFactory.C
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-05-SUMMARY.md

<interfaces>
<!-- QRhi spike contract -- mirrors Plan 05's GL-core arm. -->

CMakeLists.txt — extend the `BALL_SPIKE_BACKEND` option introduced in Plan 05:
  IF (BALL_SPIKE_BACKEND STREQUAL "QRhi")
    IF (Qt6Core_VERSION VERSION_LESS 6.7)
      MESSAGE(WARNING "Phase 5 spike: QRhi backend requested but Qt ${Qt6Core_VERSION} < 6.7 (QRhiWidget unavailable). Build proceeds without spike; rerun with -DBALL_SPIKE_BACKEND=GLCore or upgrade Qt.")
    ELSE()
      LIST(APPEND BALL_PROJECT_COMPILE_DEFNS "-DBALL_SPIKE_BACKEND_QRHI")
      MESSAGE(STATUS "Phase 5 spike: QRhi renderer ENABLED (throwaway; Qt ${Qt6Core_VERSION})")
    ENDIF()
  ENDIF()
The block belongs immediately after Plan 05's `GLCore` arm in CMakeLists.txt.

rendererFactory.h — fill in the Plan 05 placeholder comment `// (Plan 06 inserts BALL_SPIKE_BACKEND_QRHI block here)` with:
  #ifdef BALL_SPIKE_BACKEND_QRHI
    QRhi            ///< THROWAWAY: QRhiWidget-based renderer spike (Phase 5, requires Qt 6.7+)
  #endif

rendererFactory.C — add to both switches under `#ifdef BALL_SPIKE_BACKEND_QRHI`:
  makeRenderer: `case Kind::QRhi: return new QRhiRenderer;`
  makeSurface:  `case Kind::QRhi: return new QtRhiSurface(parent);`
Also extend the Plan 05 env-var gate at the top of makeRenderer so the SAME env var routes to QRhiRenderer when only QRHI is compiled in:
  #if defined(BALL_SPIKE_BACKEND_GLCORE) || defined(BALL_SPIKE_BACKEND_QRHI)
    if (kind == Kind::OpenGL_Fixed && std::getenv("BALLVIEW_USE_SPIKE_BACKEND") != nullptr) {
  #  if defined(BALL_SPIKE_BACKEND_QRHI)
      return new QRhiRenderer;   // QRhi wins if both are defined (unlikely)
  #  else
      return new CoreGLRenderer;
  #  endif
    }
  #endif

RenderSurface impl pattern (`renderSurface.h` defines the interface):
  - `beginFrame()` — opens a QRhi command buffer / begins a frame on the QRhiWidget.
  - `endFrame()`   — submits the command buffer.
  - `nativeHandle()` — returns the `QRhi*` pointer cast to `void*` (opaque; only the matching backend casts it back, per RENDERER-INTERFACE-BOUNDARY.md §RenderSurface).
  - The class inherits `RenderSurface` (from `renderSurface.h`); for QRhi it also inherits `QRhiWidget` (Qt 6.7+) so the widget IS the surface. This is the multi-inheritance pattern the existing `GLRenderWindow` uses for the GL/QOpenGLWidget side, so it fits the project's idiom.

QRhi shader / draw setup (`rhiRenderer.C`):
  - Minimal shader pair (vertex + fragment) compiled at build time via `QShader::fromSerialized` OR compiled at runtime from GLSL via `QShaderBaker` (the latter is simpler for a spike and avoids needing the Qt `qsb` tool in the build).
  - One `QRhiBuffer` for vertex data (re-uploaded per representation as the simplest impl — performance not measured in this spike).
  - One `QRhiGraphicsPipeline`.
  - `pickObjects` — for the spike, return an empty `std::list<GeometricObject*>` and emit a `BALLVIEW_GL_DIAG spike_qrhi_picking=not-implemented` line so SPIKE-02 can note this as a caveat. (Color-buffer picking is more involved under QRhi — `QRhiReadbackResult` async readback — and falls outside the minimum SPIKE-01 deliverable for the QRhi arm; the GL-core arm satisfies the "demonstrating picking" criterion of SPIKE-01.) Document this explicitly in 05-06-SUMMARY.md.

CI — the CI step variant for QRhi mirrors Plan 05's macOS-only non-blocking step but uses `-DBALL_SPIKE_BACKEND=QRhi` and runs ONLY IF Qt >= 6.7. Add this as a separate step in `.github/workflows/ci.yml` (same `continue-on-error: true`, same artifact upload). Plan 05's step stays unchanged.

Conditional execution: If at plan execution time `brew info qt` reports a version < 6.7 on the local box AND the CI Linux runner via jurplel/install-qt-action is pinned to 6.5.* (Plan 04 default), then the QRhi spike must be skipped on Linux at runtime (the `Qt6Core_VERSION VERSION_LESS 6.7` CMake check handles this — the compile defn never lands; the CI Linux spike step then triggers the WARNING and produces no QRhi binary, which is acceptable per CONTEXT.md "deferred default 'both, with rationale in 05-RESEARCH.md if the planner trims to one'"). macOS at 6.11 satisfies the constraint; Plan 06's required deliverable is "QRhi spike renders on macOS via Metal". If executor finds Homebrew qt has regressed below 6.7, raise it to user; do NOT silently downgrade to GL-core only — document the discrepancy in 05-06-SUMMARY.md.
</interfaces>
</context>

<tasks>

<task type="auto" tdd="false">
  <name>Task 1: Extend BALL_SPIKE_BACKEND option (QRhi arm) + RendererFactory Kind/switches</name>
  <files>CMakeLists.txt, include/BALL/VIEW/RENDERING/rendererFactory.h, source/VIEW/RENDERING/rendererFactory.C</files>
  <read_first>
    - CMakeLists.txt — the `OPTION(BALL_SPIKE_BACKEND ...)` block added by Plan 05; locate `IF (BALL_SPIKE_BACKEND STREQUAL "GLCore")` to find the insertion site for the QRhi arm.
    - include/BALL/VIEW/RENDERING/rendererFactory.h — the post-Plan-05 file with the `// (Plan 06 inserts ...)` placeholder comment.
    - source/VIEW/RENDERING/rendererFactory.C — the post-Plan-05 file with the env-var gate + GL-core cases.
    - 05-PATTERNS.md §`include/BALL/VIEW/RENDERING/rendererFactory.h` (the canonical Kind enum with both ifdef-blocks).
    - 05-PATTERNS.md §`source/VIEW/RENDERING/rendererFactory.C` (the canonical switch cases for both spike Kinds).
    - 05-RESEARCH.md §Pitfall 7 (QRhiWidget API stability + Qt 6.7+ requirement).
  </read_first>
  <behavior>
    - CMakeLists.txt's `BALL_SPIKE_BACKEND` block now handles both `GLCore` (Plan 05) and `QRhi` (this plan). The QRhi arm guards on `Qt6Core_VERSION VERSION_LESS 6.7` and emits a clear WARNING with remediation steps when the Qt floor is not met.
    - rendererFactory.h's placeholder is replaced with the `#ifdef BALL_SPIKE_BACKEND_QRHI ... QRhi ... #endif` block. The Plan 05 GLCore block stays unchanged.
    - rendererFactory.C's env-var gate handles both compile-time variants (the canonical block from `<interfaces>`); both switches gain the new `Kind::QRhi` case under ifdef. The new cases call `new QRhiRenderer` (created in Task 2) and `new QtRhiSurface(parent)` (also created in Task 2).
    - Default builds (no `-DBALL_SPIKE_BACKEND`) remain byte-identical to post-Plan-05 default builds — both ifdef arms collapse to nothing.
  </behavior>
  <action>
    1. **CMakeLists.txt**: After Plan 05's `IF (BALL_SPIKE_BACKEND STREQUAL "GLCore") ... ENDIF()` block, add the QRhi arm verbatim per `<interfaces>` above. Use the `Qt6Core_VERSION` variable (set by `FIND_PACKAGE(Qt6 Core ...)` from Plan 01 — verify with the executor's read of CMakeLists.txt that the variable spelling matches the project's actual `Qt6Core_VERSION` or `Qt6_VERSION`; if neither exists, fall back to `Qt6Widgets_VERSION` which is reliably populated by Plan 01's `FIND_PACKAGE(Qt6 ... Widgets ...)` call). Use `LIST(APPEND BALL_PROJECT_COMPILE_DEFNS "-DBALL_SPIKE_BACKEND_QRHI")` — NEVER `SET("..." string)` (Phase 04-04 lesson). The WARNING message must reference the GL-core alternative so a developer is not stuck.

    2. **rendererFactory.h**: Replace the Plan 05 placeholder comment `// (Plan 06 inserts BALL_SPIKE_BACKEND_QRHI block here)` with the `#ifdef BALL_SPIKE_BACKEND_QRHI ... QRhi ... #endif` block. Doxygen comment per `<interfaces>`. Match indentation of surrounding enum entries.

    3. **rendererFactory.C**: Update the Plan 05 env-var gate at the top of `makeRenderer` to handle the combined GLCore + QRhi case per `<interfaces>` (the `#if defined(...) || defined(...)` block with internal `#if defined(BALL_SPIKE_BACKEND_QRHI) ... #else ... #endif`). Also add the `#include <BALL/VIEW/RENDERING/RENDERERS/rhiRenderer.h>` and `#include <BALL/VIEW/RENDERING/qtRhiSurface.h>` under `#ifdef BALL_SPIKE_BACKEND_QRHI` at the top of the file. Inside the switches, add the new `case Kind::QRhi:` arms under `#ifdef BALL_SPIKE_BACKEND_QRHI`.

    Anti-rules: Do NOT modify scene.C. Do NOT touch the GL-core path / files. Do NOT introduce `Qt5Compat` or `Qt6::Core5Compat` (D-05 — still applies).
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; grep -q 'BALL_SPIKE_BACKEND_QRHI' CMakeLists.txt &amp;&amp; grep -qE 'Qt6.*VERSION_LESS 6\.7' CMakeLists.txt &amp;&amp; grep -q '#ifdef BALL_SPIKE_BACKEND_QRHI' include/BALL/VIEW/RENDERING/rendererFactory.h &amp;&amp; grep -q 'Kind::QRhi' include/BALL/VIEW/RENDERING/rendererFactory.h &amp;&amp; grep -cE '#ifdef BALL_SPIKE_BACKEND_QRHI' source/VIEW/RENDERING/rendererFactory.C | grep -qE '^[2-9]' &amp;&amp; cmake --preset ci-macos -S . -DBALL_SPIKE_BACKEND=QRhi 2>&amp;1 | tee /tmp/cfg-qrhi.log | grep -qE '(Phase 5 spike: QRhi renderer ENABLED|Phase 5 spike: QRhi backend requested but Qt)'</automated>
  </verify>
  <acceptance_criteria>
    - `grep -c 'BALL_SPIKE_BACKEND_QRHI' CMakeLists.txt` returns `>= 1`.
    - `grep -cE 'VERSION_LESS 6\.7' CMakeLists.txt` returns `>= 1` (the Qt 6.7 floor check).
    - `grep -c '#ifdef BALL_SPIKE_BACKEND_QRHI' include/BALL/VIEW/RENDERING/rendererFactory.h` returns `>= 1`.
    - `grep -c '#ifdef BALL_SPIKE_BACKEND_QRHI' source/VIEW/RENDERING/rendererFactory.C` returns `>= 2` (one in env-var gate, at least one in each switch — counted unfused).
    - Default configure (`cmake --preset ci-macos -S .`) succeeds and does NOT log either spike status line.
    - QRhi configure (`cmake --preset ci-macos -S . -DBALL_SPIKE_BACKEND=QRhi`) succeeds and logs EITHER `Phase 5 spike: QRhi renderer ENABLED` (if Qt >= 6.7 — expected on macOS Homebrew qt 6.11) OR `Phase 5 spike: QRhi backend requested but Qt ... < 6.7` (if Qt < 6.7).
    - GLCore configure (`cmake --preset ci-macos -S . -DBALL_SPIKE_BACKEND=GLCore`) still succeeds — Plan 05 path not regressed.
    - LIST(APPEND ...) is used for the compile-defn — `grep -cE 'SET\(BALL_PROJECT_COMPILE_DEFNS' CMakeLists.txt` returns `0` (anti-pattern check).
  </acceptance_criteria>
  <done>
    BALL_SPIKE_BACKEND=QRhi flips on `-DBALL_SPIKE_BACKEND_QRHI` when Qt >= 6.7, or emits a clear, actionable warning otherwise; rendererFactory has the new Kind and both switch cases; default builds unaffected.
  </done>
</task>

<task type="auto" tdd="false">
  <name>Task 2: Implement QRhiRenderer + QtRhiSurface (the QRhi spike code)</name>
  <files>include/BALL/VIEW/RENDERING/RENDERERS/rhiRenderer.h, source/VIEW/RENDERING/RENDERERS/rhiRenderer.C, include/BALL/VIEW/RENDERING/qtRhiSurface.h, source/VIEW/RENDERING/qtRhiSurface.C, source/VIEW/RENDERING/RENDERERS/sources.cmake, source/VIEW/RENDERING/sources.cmake</files>
  <read_first>
    - include/BALL/VIEW/RENDERING/RENDERERS/renderer.h — abstract base, the same overrides Plan 05's CoreGLRenderer implemented.
    - include/BALL/VIEW/RENDERING/renderSurface.h — RenderSurface interface (beginFrame / endFrame / nativeHandle).
    - source/VIEW/RENDERING/RENDERERS/glRenderer.C — existing concrete impl; reference for init scaffolding only.
    - .planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-05-SUMMARY.md (Plan 05's summary — copy the shader pair / picking-pass structure where applicable to maintain symmetry between the two spike backends).
    - include/BALL/VIEW/RENDERING/glRenderWindow.h — for the multi-inheritance pattern (RenderWindow + QOpenGLWidget); QtRhiSurface mirrors this with `RenderSurface + QRhiWidget`.
    - source/VIEW/RENDERING/RENDERERS/sources.cmake — the SOURCES_LIST pattern (no path, no headers).
    - source/VIEW/RENDERING/sources.cmake — the parent dir's sources.cmake for `qtRhiSurface.C` registration.
    - 05-PATTERNS.md §sources.cmake registration.
    - 05-RESEARCH.md §QRhi Backend (Kind::QRhi) — tradeoffs section; §Code Examples (no direct QRhi snippet — synthesise from Qt 6.7+ QRhiWidget docs cited in §Sources).
    - 05-RESEARCH.md §Pitfall 7 — note QRhi API stability caveats in the file header comments.
  </read_first>
  <behavior>
    - `rhiRenderer.h` declares `class BALL_VIEW_EXPORT QRhiRenderer : public Renderer` in `BALL::VIEW`. Overrides: `init(Scene&)`, `renderRepresentations_(const RepresentationList&)`, `capabilities()`, `pickObjects(...)`. Throwaway header comment. Wrapped entirely in `#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)` so an older Qt fails to declare the class gracefully (the CMake check would have prevented compile in this case anyway — the QT_VERSION_CHECK is belt-and-braces).
    - `rhiRenderer.C` implements the four overrides using a QRhi command buffer model. `init` obtains the `QRhi*` via the QRhiWidget's `rhi()` accessor (cached pointer). `renderRepresentations_` builds vertex data from the representation list and uploads to a `QRhiBuffer`, sets up a `QRhiGraphicsPipeline` with a minimal vertex/fragment shader pair compiled at runtime via `QShaderBaker`, issues a draw call. `capabilities()` returns `Caps{ retained_mode: true, offscreen: false, picking: false, stereo: false }` (picking is deferred per `<interfaces>`). `pickObjects` returns empty + emits a DIAG line.
    - `qtRhiSurface.h` declares `class BALL_VIEW_EXPORT QtRhiSurface : public RenderSurface, public QRhiWidget` (multi-inheritance, mirroring `GLRenderWindow`). Overrides `beginFrame()`, `endFrame()`, `nativeHandle()`. Constructor takes `QWidget* parent`. Includes throwaway header comment.
    - `qtRhiSurface.C` implements the three RenderSurface verbs:
      * `beginFrame()` — calls `QRhiWidget::beginFrame()` (or equivalent QRhi API) and stores the command buffer pointer.
      * `endFrame()`   — calls `QRhiWidget::endFrame()` to submit.
      * `nativeHandle()` — returns `static_cast<void*>(rhi())` (the `QRhi*` opaque handle for backend-specific code to downcast).
    - Both `.C` files are added to their respective `sources.cmake`:
      * `source/VIEW/RENDERING/RENDERERS/sources.cmake` — `rhiRenderer.C`.
      * `source/VIEW/RENDERING/sources.cmake` — `qtRhiSurface.C`.
    - All new files start with the THROWAWAY SPIKE 4-line provenance comment + the QRhi-API-stability note: `// QRhi API has limited source/binary compatibility guarantees -- see 05-RESEARCH.md Pitfall 7.`
    - QRhi build produces a non-blank PNG on macOS via Metal (the macOS Apple Silicon Metal backend; verify in acceptance criteria).
  </behavior>
  <action>
    Create the four new source files following the canonical patterns from 05-PATTERNS.md §Shared Patterns (modeline + BALL_EXPORT macro + namespace wrapper + sources.cmake registration). For each new file, the first 5 lines are:

       // -*- Mode: C++; tab-width: 2; -*-
       // vi: set ts=2:
       //
       // THROWAWAY SPIKE -- Phase 5 prototype only. Not production code. Remove before PIPE-01.
       // QRhi API has limited source/binary compatibility guarantees -- see 05-RESEARCH.md Pitfall 7.

    1. **rhiRenderer.h**: Include guard `BALL_VIEW_RENDERING_RENDERERS_RHIRENDERER_H`. After the project includes, wrap the entire class body in `#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)`. Class declaration with the four overrides; constructor + virtual destructor. Use `BALL_VIEW_EXPORT`.

    2. **rhiRenderer.C**: Includes — `rhiRenderer.h`, `<rhi/qrhi.h>` (Qt 6.7+), `<QtWidgets/QApplication>` for the QShaderBaker context if needed, `<cstdlib>` for getenv. Implement the four overrides. Use `QShaderBaker::compile` at runtime to produce QShader objects from inline GLSL strings (no external `.qsb` files; the spike avoids adding the `qsb` tool to the build). Vertex shader: pass-through MVP transform of `vec3 in_pos` + `vec3 in_color`. Fragment shader: writes the interpolated color. Set up `QRhiBuffer` for vertex data, `QRhiGraphicsPipeline` with the shader pair. `renderRepresentations_` rebuilds vertex data per call (acceptable for a spike; performance is not a SPIKE-01 deliverable). `pickObjects` returns empty list and emits `std::cerr << "BALLVIEW_GL_DIAG spike_qrhi_picking=not-implemented" << std::endl;`.

    3. **qtRhiSurface.h**: Include guard `BALL_VIEW_RENDERING_QTRHISURFACE_H`. Wrap in `#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)`. Class declaration inheriting `public RenderSurface, public QRhiWidget`. Overrides `beginFrame`, `endFrame`, `nativeHandle`. Constructor `explicit QtRhiSurface(QWidget* parent = nullptr)`. Use `BALL_VIEW_EXPORT`.

    4. **qtRhiSurface.C**: Implement the three verbs. `nativeHandle()` returns `static_cast<void*>(this->rhi())`. `beginFrame()` and `endFrame()` delegate to the QRhiWidget hooks (`initialize(QRhiCommandBuffer*)` / `render(QRhiCommandBuffer*)` — verify against the QRhiWidget signature in the Qt 6.7+ header; if the QRhiWidget interface differs from the simple `beginFrame/endFrame` shape, document the adaptation in the action's resulting summary).

    5. **sources.cmake updates**: 
       - `source/VIEW/RENDERING/RENDERERS/sources.cmake`: add `rhiRenderer.C` alphabetically or after `coreGLRenderer.C` (Plan 05's addition).
       - `source/VIEW/RENDERING/sources.cmake`: add `qtRhiSurface.C` alphabetically. NO header files listed; NO path prefix; one filename per line per the existing pattern.

    Anti-rules: Do NOT modify scene.C. Do NOT pull in QRhi files when `BALL_SPIKE_BACKEND_QRHI` is not defined (the `#if QT_VERSION_CHECK` + the ifdef in `rendererFactory.C` ensure the QRhi headers and links are only pulled when both compile and Qt-version conditions are met). Do NOT implement picking in the QRhi arm (covered by the GL-core arm's color-buffer FBO; documented as a known caveat). Do NOT add a QRhi GUI toggle.
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; test -f include/BALL/VIEW/RENDERING/RENDERERS/rhiRenderer.h &amp;&amp; test -f source/VIEW/RENDERING/RENDERERS/rhiRenderer.C &amp;&amp; test -f include/BALL/VIEW/RENDERING/qtRhiSurface.h &amp;&amp; test -f source/VIEW/RENDERING/qtRhiSurface.C &amp;&amp; grep -q 'THROWAWAY SPIKE' include/BALL/VIEW/RENDERING/RENDERERS/rhiRenderer.h include/BALL/VIEW/RENDERING/qtRhiSurface.h source/VIEW/RENDERING/RENDERERS/rhiRenderer.C source/VIEW/RENDERING/qtRhiSurface.C &amp;&amp; grep -q 'rhiRenderer.C' source/VIEW/RENDERING/RENDERERS/sources.cmake &amp;&amp; grep -q 'qtRhiSurface.C' source/VIEW/RENDERING/sources.cmake &amp;&amp; cmake --preset ci-macos -S . -DBALL_SPIKE_BACKEND=QRhi 2>&amp;1 | tail -5 &amp;&amp; cmake --build --preset ci-macos --target BALLView -j$(sysctl -n hw.ncpu) 2>&amp;1 | tee /tmp/qrhi-build.log | tail -30 &amp;&amp; BALLVIEW_USE_SPIKE_BACKEND=1 bash .planning/phases/02.2-ci-and-build-smoke-matrix/scripts/render-smoke-check.sh build/ci-macos/bin/BALLView.app/Contents/MacOS/BALLView 2>&amp;1 | tee /tmp/qrhi-smoke.log | tail -20 &amp;&amp; cmake --preset ci-macos -S . 2>&amp;1 | tail -5 &amp;&amp; cmake --build --preset ci-macos --target BALLView -j$(sysctl -n hw.ncpu) 2>&amp;1 | tail -5</automated>
  </verify>
  <acceptance_criteria>
    - All four new files exist and start with the THROWAWAY SPIKE + QRhi API-stability provenance comment (verified by `grep -c 'THROWAWAY SPIKE'` and `grep -c 'QRhi API has limited'` returning >= 1 on each).
    - `grep -c 'rhiRenderer.C' source/VIEW/RENDERING/RENDERERS/sources.cmake` returns `>= 1`.
    - `grep -c 'qtRhiSurface.C' source/VIEW/RENDERING/sources.cmake` returns `>= 1`.
    - `cmake --preset ci-macos -S . -DBALL_SPIKE_BACKEND=QRhi` succeeds (no error; if Qt < 6.7 it emits the WARNING but still configures green).
    - `cmake --build --preset ci-macos --target BALLView` succeeds (exit 0) when Qt >= 6.7.
    - `BALLVIEW_USE_SPIKE_BACKEND=1 bash .../render-smoke-check.sh ...BALLView` produces a non-blank PNG via the QRhi path on macOS Apple Silicon (Metal backend) — PNG size > 1 KB per the smoke check's own assertion. (On the local Homebrew qt 6.11 macOS box this is the binding constraint; if it fails, the executor reports back to the user with the build/smoke logs.)
    - Default (no `-DBALL_SPIKE_BACKEND`) build still produces a non-blank PNG via `Kind::OpenGL_Fixed` (regression check).
    - GL-core spike (Plan 05) still works: `cmake --preset ci-macos -S . -DBALL_SPIKE_BACKEND=GLCore && cmake --build ...` → non-blank PNG.
    - scene.C unchanged (`git diff source/VIEW/WIDGETS/scene.C` since Plan 02 shows no Plan 06 lines).
    - `grep -rn 'spike' source/VIEW/DIALOGS source/APPLICATIONS/BALLVIEW 2>/dev/null | grep -vE 'THROWAWAY SPIKE'` returns no UI-wiring lines.
    - DIAG line for picking caveat present in spike runtime stdout: `grep 'spike_qrhi_picking=not-implemented' /tmp/qrhi-smoke.log` matches.
  </acceptance_criteria>
  <done>
    QRhiRenderer + QtRhiSurface compile and render the demo molecule on macOS (Metal backend) via the QRhi spike Kind; the GL-core spike + default builds are unaffected; scene.C unchanged. SPIKE-01 (QRhi arm) deliverable: renders molecule + text overlay (via QRhiWidget's own painting path) — picking explicitly deferred to GL-core arm, caveat documented.
  </done>
</task>

</tasks>

<verification>
- `cmake --preset ci-macos -S . -DBALL_SPIKE_BACKEND=QRhi` succeeds; build green; spike smoke produces non-blank PNG.
- `cmake --preset ci-macos -S . -DBALL_SPIKE_BACKEND=GLCore` (Plan 05 path) still produces non-blank PNG.
- Default configure + build (`cmake --preset ci-macos -S .` + build) still produces non-blank PNG via `Kind::OpenGL_Fixed`.
- `git diff source/VIEW/WIDGETS/scene.C` shows no change in this plan.
- All four new files carry THROWAWAY SPIKE + QRhi API-stability provenance comments.
- LIST(APPEND ...) used for the QRhi compile-defn — no `SET(...)` regression.
</verification>

<success_criteria>
- SPIKE-01 (QRhi arm) deliverable: macOS Metal-backed QRhi rendering of the demo molecule via `QRhiRenderer` + `QtRhiSurface`, behind `-DBALL_SPIKE_BACKEND=QRhi`.
- Default + GL-core builds untouched.
- Qt-version floor handled with a clear actionable WARNING when Qt < 6.7.
- Picking caveat documented and emitted at runtime (DIAG line); SPIKE-02 (Plan 08) cites it.
- Scene.C still byte-identical to its post-Plan-02 state.
</success_criteria>

<output>
After completion, create `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-06-SUMMARY.md` with: a diff-stat of the 9 files, the QRhi backend that Qt selected at runtime on macOS (Metal/Vulkan/etc. — captured from QRhiWidget's `rhi()->backendName()` if available or via the QSG_INFO=1 / QT_LOGGING_RULES=qt.rhi.*=true env trick), the QRhi smoke-check PNG byte count, a snippet of the inline GLSL shader pair, the exact `QRhiWidget` API signatures used (since these are unstable across Qt minors per Pitfall 7), and a deliberate paragraph on why picking was deferred to the GL-core arm. This SUMMARY plus Plan 05's SUMMARY are Plan 08's two primary inputs.
</output>
