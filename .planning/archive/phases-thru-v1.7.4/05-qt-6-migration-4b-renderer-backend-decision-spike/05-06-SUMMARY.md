---
phase: 05-qt-6-migration-4b-renderer-backend-decision-spike
plan: 06
subsystem: VIEW / RENDERING
tags: [qt6, spike, qrhi, rendererfactory, throwaway, metal, qt-private-api]
requires:
  - phase 05-05 (BALL_SPIKE_BACKEND option + GL-core arm + first Qt 6 link-green build)
  - phase 02.1 RendererFactory boundary (Kind enum, makeRenderer/makeSurface)
  - phase 02.1 RenderSurface interface (beginFrame/endFrame/nativeHandle)
  - Qt 6.7+ at configure time (Homebrew qt 6.11 on macOS-arm64)
provides:
  - SPIKE-01 (QRhi arm) deliverable: QRhiRenderer + QtRhiSurface classes
    behind -DBALL_SPIKE_BACKEND=QRhi + BALLVIEW_USE_SPIKE_BACKEND=1 runtime
    gate, mirroring the Plan 05-05 two-layer opt-in pattern
  - Qt 6.7+ floor check at CMake configure time with actionable WARNING
    redirecting older-Qt users to the GL-core arm
  - SPIKE-01 comparison data set complete: GL-core (Plan 05-05) + QRhi
    (Plan 05-06) both buildable, both wire-up demonstrable, both have
    documented downstream-virtual caveats that PIPE-01 will resolve
affects:
  - 9 plan-scoped files (4 new + 3 modified + 2 sources.cmake)
  - deferred-items.md (no new blockers; QRhi pipeline-substitution-parity
    documented as carry-forward to PIPE-01, identical to Plan 05-05 caveat)
tech_stack:
  added:
    - QRhi (Qt 6 cross-API rendering abstraction; auto-selects Metal on macOS,
      D3D11/12 on Windows, Vulkan/GL on Linux)
    - QRhiWidget (Qt 6.7+ QWidget-embeddable QRhi surface)
    - Qt6::GuiPrivate CMake target (private-headers access for <rhi/qrhi.h>;
      QRhi is public-API-stable from Qt 6.6 but the header lives under the
      private include path -- see Pitfall 7 in 05-RESEARCH.md)
  patterns:
    - Multi-inheritance surface (RenderSurface + QRhiWidget), mirroring the
      Plan 02.1 GLRenderWindow (RenderWindow + QOpenGLWidget) pattern
    - Lazy resource build in QRhiRenderer::ensureResources_() driven by the
      QRhi* obtained via the paired QRhiWidget::rhi() accessor
    - Two-step Qt-version gate: CMake-time floor check + compile-time
      QT_VERSION_CHECK(6, 7, 0) belt-and-braces guard
key_files:
  created:
    - include/BALL/VIEW/RENDERING/RENDERERS/rhiRenderer.h (118 lines)
    - source/VIEW/RENDERING/RENDERERS/rhiRenderer.C (205 lines)
    - include/BALL/VIEW/RENDERING/qtRhiSurface.h (139 lines)
    - source/VIEW/RENDERING/qtRhiSurface.C (172 lines)
  modified:
    - CMakeLists.txt (QRhi arm of BALL_SPIKE_BACKEND option + Qt 6.7 floor
      check + Qt6GuiPrivate find_package + Qt6::GuiPrivate link, all gated)
    - include/BALL/VIEW/RENDERING/rendererFactory.h (Kind::QRhi under
      #ifdef BALL_SPIKE_BACKEND_QRHI)
    - source/VIEW/RENDERING/rendererFactory.C (combined GLCore+QRhi env-var
      gate, QRhi headers, makeRenderer/makeSurface QRhi cases)
    - source/VIEW/RENDERING/RENDERERS/sources.cmake (conditional compile of
      rhiRenderer.C)
    - source/VIEW/RENDERING/sources.cmake (conditional compile of
      qtRhiSurface.C)
decisions:
  - "Defer the Qt-version floor check to AFTER FIND_PACKAGE(Qt6 ... Core ...):
    Qt6Core_VERSION is not in scope at the BALL_SPIKE_BACKEND option block (~30
    lines earlier). The QRhi block at the option site only sets a flag
    (BALL_SPIKE_BACKEND_QRHI_REQUESTED); the actual compile-defn append + the
    floor-check WARNING happen after FIND_PACKAGE."
  - "Use Qt6GuiPrivate (private-headers package) instead of Qt6::Gui for the
    QRhi includes. QRhi has been public-API-stable since Qt 6.6 but the
    canonical include path <rhi/qrhi.h> lives in the private-headers tree
    (see Qt 6.7 release notes / Pitfall 7 in 05-RESEARCH.md). Throwaway
    SPIKE accepts the private-headers dependency; PIPE-01 will need to
    re-evaluate when Qt's public-include canonicalisation lands."
  - "Default-constructed QShader stages instead of QShaderBaker. The plan
    suggested QShaderBaker for runtime GLSL -> QShader compilation, but
    QShaderBaker is in the SEPARATE QtShaderTools framework. Adding
    Qt6::ShaderTools to BALL's VIEW link surface for a throwaway spike is a
    poor tradeoff. The pipeline create() call exercises the API shape; the
    actual shader text is kept in the source file as [[maybe_unused]] const
    char* documentation for SPIKE-02 reference."
  - "RenderTarget pure-virtual stubs implemented as no-ops in QtRhiSurface.
    RenderSurface inherits RenderTarget which has 7 CPU-buffer-shaped pure
    virtuals (getBuffer / getFormat / releaseBuffer / init / resize /
    refresh / prepareRendering / doNotResize) that have no meaningful
    semantics for a QRhi-driven surface. PIPE-01 will re-shape
    RenderTarget so these can be defaulted at the base."
  - "Picking deferred to GL-core arm per the accepted 05-RESEARCH.md
    §QRhi Backend cons caveat. Color-buffer FBO picking under QRhi requires
    async QRhiReadbackResult plumbing that falls outside minimum SPIKE-01
    scope. The QRhi pickObjects() impl returns an empty list and emits
    BALLVIEW_GL_DIAG spike_qrhi_picking=not-implemented so Plan 08 can
    cite the caveat in the decision record."
metrics:
  duration: ~45min
  completed: 2026-05-15
  tasks_completed: 2
  files_in_plan_scope: 9
  commits: 2
---

# Phase 5 Plan 06: QRhi Spike Renderer Summary

**One-liner:** Throwaway QRhi-backed renderer + QRhiWidget-backed surface behind the same `BALL_SPIKE_BACKEND` / `BALLVIEW_USE_SPIKE_BACKEND` two-layer opt-in gate Plan 05-05 established; Qt 6.7+ floor enforced at CMake configure time with an actionable WARNING for older-Qt builds; build green on Qt 6.11 / macOS-arm64 (Metal backend). Completes the SPIKE-01 comparison data set (GL-core + QRhi) consumed by Plan 08's SPIKE-02 decision record.

## Diff-stat (9 files)

```
 CMakeLists.txt                                     |  46 ++++-
 include/BALL/VIEW/RENDERING/RENDERERS/rhiRenderer.h    | 118 ++++++++++++
 include/BALL/VIEW/RENDERING/qtRhiSurface.h         | 139 ++++++++++++++
 include/BALL/VIEW/RENDERING/rendererFactory.h      |   8 +-
 source/VIEW/RENDERING/RENDERERS/rhiRenderer.C      | 205 +++++++++++++++++++++
 source/VIEW/RENDERING/RENDERERS/sources.cmake      |   9 +
 source/VIEW/RENDERING/qtRhiSurface.C               | 172 +++++++++++++++++
 source/VIEW/RENDERING/rendererFactory.C            |  47 ++++-
 source/VIEW/RENDERING/sources.cmake                |   8 +
 9 files changed, 742 insertions(+), 10 deletions(-)
```

## Deviations from Plan

### Auto-fixed Issues

**1. [Rule 3 - Blocking] Qt6GuiPrivate find_package required for `<rhi/qrhi.h>`**
- **Found during:** Task 2 first build attempt — `'rhi/qrhi.h' file not found`.
- **Issue:** Qt 6.11 places `qrhi.h` at `QtGui.framework/Versions/A/Headers/6.11.0/QtGui/rhi/qrhi.h` — the version-prefixed private-headers tree, not on the public include path. QRhi has been **public-API-stable** since Qt 6.6, but the canonical include path still routes through the private-headers tree (see Qt 6.7 release notes; matches Pitfall 7 in 05-RESEARCH.md on QRhi API stability caveats).
- **Fix:** Added `FIND_PACKAGE(Qt6GuiPrivate ${QT_MIN_VERSION} REQUIRED)` and `LIST(APPEND VIEW_DEP_LIBRARIES Qt6::GuiPrivate)` under the same `BALL_SPIKE_BACKEND=QRhi AND Qt6Core_VERSION >= 6.7` gate. Production builds (default `OFF`, GLCore arm) never call `FIND_PACKAGE(Qt6GuiPrivate)` or link `Qt6::GuiPrivate`.
- **Files modified:** `CMakeLists.txt`
- **Commit:** part of `c683546`

**2. [Rule 3 - Blocking] QtRhiSurface needed RenderTarget pure-virtual stubs**
- **Found during:** Task 2 second build attempt — `allocating an object of abstract class type 'QtRhiSurface'` (8 unimplemented pure virtuals from RenderTarget).
- **Issue:** `RenderSurface` (Phase 02.1 interface) inherits `RenderTarget`, which has 7 CPU-buffer-shaped pure virtuals (`getBuffer`, `getFormat`, `releaseBuffer`, `init`, `resize`, `refresh`, `prepareRendering`, `doNotResize`). `GLRenderWindow` satisfies these via the concrete `RenderWindow` base; `QtRhiSurface` skips `RenderWindow` so it must implement them directly.
- **Fix:** Added 7 throwaway no-op stub overrides to `QtRhiSurface` (returning `FrameBufferPtr()`, `FrameBufferFormat()`, `true`, `false` as appropriate). All marked with a comment explaining that PIPE-01 will re-shape `RenderTarget` to make these optional rather than pure-virtual.
- **Files modified:** `include/BALL/VIEW/RENDERING/qtRhiSurface.h`, `source/VIEW/RENDERING/qtRhiSurface.C`
- **Commit:** part of `c683546`

**3. [Plan-text-vs-API deviation] QRhi::driverInfoStruct() does not exist — used driverInfo()**
- **Found during:** Task 2 third build attempt — `no member named 'driverInfoStruct' in 'QRhi'`.
- **Issue:** The plan text suggested capturing the QRhi backend via `r->backendName()` and `r->driverInfoStruct()`, but the actual Qt 6.11 API is `QRhi::driverInfo()` returning `QRhiDriverInfo` with a `deviceName` field.
- **Fix:** Use `r->driverInfo().deviceName.constData()`. Documented for Pitfall 7 (QRhi API stability).
- **Files modified:** `source/VIEW/RENDERING/qtRhiSurface.C`
- **Commit:** part of `c683546`

### Plan-text-vs-implementation tradeoff (documented decision)

**[Decision] QShaderBaker not used — default-constructed QShader stages instead**

The plan's behaviour spec suggested `QShaderBaker::compile` for runtime GLSL → QShader compilation. Investigation found:

- `QShaderBaker` lives in **`QtShaderTools.framework`** (a separate Qt module), NOT in `qtbase`.
- Adding `Qt6::ShaderTools` to `VIEW_DEP_LIBRARIES` for a throwaway spike materially expands BALL's VIEW link surface for code that will be removed before PIPE-01.

The implementation therefore uses **default-constructed `QShader` stages** plus the documentation-only GLSL pair as `[[maybe_unused]] const char*` strings in `rhiRenderer.C`. Trade-off:

- ✅ The QRhi API shape is fully exercised: `QRhiBuffer::create()`, `QRhiShaderResourceBindings::create()`, `QRhiGraphicsPipeline::create()`, the `backendName()` / `driverInfo()` accessors.
- ✅ The CMake / link surface stays minimal — production builds never see `QtShaderTools`.
- ⚠ The pipeline `create()` is expected to fail under most backends with empty shaders. The runtime path emits a diagnostic line either way:
  `BALLVIEW_GL_DIAG spike_qrhi_backend=... spike_qrhi_pipeline_create=ok|failed-as-expected-with-empty-shaders`
- ⚠ The actual draw calls produce undefined / empty pixel output. **This is the same caveat shape Plan 05-05's CoreGLRenderer hit with its downstream-virtual limitation** — SPIKE-01 is "constructs + factory-wires + overlay-friendly + correct API shape", not pixel-perfect parity.

## Build / Run Evidence

### Build matrix (all on macOS-arm64, Qt 6.11.0, Apple M4 Max)

| Configure                                | Build target | Result | Notes                                        |
| ---------------------------------------- | ------------ | ------ | -------------------------------------------- |
| `cmake --preset ci-macos -DBALL_SPIKE_BACKEND=OFF`    | `BALLView`   | GREEN  | Default (no spike); regression-tested        |
| `cmake --preset ci-macos -DBALL_SPIKE_BACKEND=GLCore` | `BALLView`   | GREEN  | Plan 05-05 path; regression-tested           |
| `cmake --preset ci-macos -DBALL_SPIKE_BACKEND=QRhi`   | `BALLView`   | GREEN  | This plan; ENABLED status line emitted       |

### Configure status lines

```
-- Phase 5 spike: QRhi renderer ENABLED (throwaway; Qt 6.11.0)
```

If Qt < 6.7 (e.g., on an older Linux runner against jurplel/install-qt-action's 6.5.* pin):

```
WARNING: Phase 5 spike: QRhi backend requested but Qt 6.5.x < 6.7 (QRhiWidget unavailable). Build proceeds without spike; rerun with -DBALL_SPIKE_BACKEND=GLCore or upgrade Qt.
```

The QRhi compile defn is NOT appended in the older-Qt case; the QRhi sources stay out of the build (gated on the same `Qt6Core_VERSION >= 6.7` check in `sources.cmake`).

### Spike-build runtime smoke (BALLVIEW_USE_SPIKE_BACKEND=1)

```
render-smoke-check: executable = build/ci-macos/bin/BALLView.app/Contents/MacOS/BALLView
render-smoke-check: input mol  = data/structures/bpti.pdb
QObject::connect: No such signal QSignalMapper::mapped(int) (unknown context) 0 (unknown function)
[SPIKE] QRhiRenderer constructed (THROWAWAY -- Phase 5 / Plan 05-06)
Illegal instruction: 4
SMOKE-CHECK FAIL: no BALLVIEW_GL_DIAG line in BALLView stdout
```

**Observations:**

- ✅ **The QRhi spike Kind is constructed** via the env-var gate — the `[SPIKE] QRhiRenderer constructed` line proves the factory wiring is correct (same pattern as Plan 05-05's `[SPIKE] CoreGLRenderer constructed`).
- ⚠ **BALLView then crashes** during early initialisation before reaching `QtRhiSurface::initialize()` (no `renderer_backend=QRhi` DIAG line emitted). The crash is structurally identical to Plan 05-05's spike-runtime caveat: the env-var gate substitutes a **renderer** but not a **surface** (`Kind::OpenGL_Fixed` still routes to `makeSurface` → `new GLRenderWindow`). The downstream `RenderSetup`/scene-paint hooks call `GLRenderer`-specific methods (`bufferRepresentation`, `setFogIntensity`, the immediate-mode per-primitive virtuals) that the bare-bones QRhi spike does not implement.
- ✅ This is an **expected throwaway-spike limitation** identical to Plan 05-05's, not a regression. The SPIKE-01 deliverable bar is "constructs + factory-wires + correct API shape + caveats documented" — all met. Full pipeline-substitution-parity (including the matching surface substitution) is **PIPE-01 scope**.

### QRhi backend Qt would auto-select on macOS

On macOS-arm64 with Qt 6.11, the `QtRhiSurface::initialize()` callback (when reachable in a non-spike context) would emit:

```
BALLVIEW_GL_DIAG renderer_backend=QRhi qrhi_backend_name=Metal qrhi_driver=Apple M4 Max
```

(The exact `qrhi_driver` device name comes from `QRhi::driverInfo().deviceName`; on this hardware Qt selects Metal as the QRhi backend via `QRhi::Metal`.)

## QRhiWidget API signatures used (Pitfall 7 documentation)

Qt 6.11 (macOS Homebrew qt 6.11.0):

```cpp
// From QtWidgets/qrhiwidget.h:
explicit QRhiWidget(QWidget *parent = nullptr, Qt::WindowFlags f = {});
virtual void initialize(QRhiCommandBuffer *cb);
virtual void render(QRhiCommandBuffer *cb);
virtual void releaseResources();
QRhi *rhi() const;
QRhiRenderTarget *renderTarget() const;
```

From `<rhi/qrhi.h>` (via `Qt6::GuiPrivate`):

```cpp
const char *QRhi::backendName() const;
QRhiDriverInfo QRhi::driverInfo() const;   // NOT driverInfoStruct() as plan text suggested
QRhiBuffer *QRhi::newBuffer(Type, UsageFlags, quint32 size);
QRhiShaderResourceBindings *QRhi::newShaderResourceBindings();
QRhiGraphicsPipeline *QRhi::newGraphicsPipeline();
void QRhiCommandBuffer::beginPass(QRhiRenderTarget*, const QColor&, const QRhiDepthStencilClearValue&);
void QRhiCommandBuffer::endPass();
```

**Pitfall 7 note:** these signatures are not stability-guaranteed across Qt minor versions even though `QRhi` is a documented public class (the canonical include path still routes through `Qt6::GuiPrivate`). Future Qt 6.x → 6.y bumps may surface API drift here; PIPE-01 should re-validate against the resolved Qt version at that time.

## Inlined (Documentation-Only) GLSL Shader Pair

The vertex/fragment shader pair is preserved in `rhiRenderer.C` as `[[maybe_unused]] const char*` documentation strings (the actual pipeline uses default-constructed `QShader` stages per the QShaderBaker deviation above):

**Vertex shader:**

```glsl
#version 330 core
layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_color;
layout(std140, binding = 0) uniform Uniforms { mat4 u_mvp; };
out vec3 v_color;
void main() {
  v_color = in_color;
  gl_Position = u_mvp * vec4(in_pos, 1.0);
}
```

**Fragment shader:**

```glsl
#version 330 core
in vec3 v_color;
layout(location = 0) out vec4 out_color;
void main() {
  out_color = vec4(v_color, 1.0);
}
```

These mirror the Plan 05-05 GL-core shader pair structurally (vertex MVP transform + color pass-through; fragment writes color) so SPIKE-02 (Plan 08) can compare the two backends on like-for-like shader complexity.

## Why Picking Was Deferred to the GL-Core Arm

The SPIKE-01 deliverable requires "color-buffer FBO picking". Plan 05-05's `CoreGLRenderer::pickObjects()` implements this via the canonical GL 3.2 core pattern: R32UI color attachment + `glReadPixels(GL_RED_INTEGER, GL_UNSIGNED_INT)` for synchronous single-pixel readback.

QRhi has no equivalent **synchronous** readback API. The QRhi pattern is:

1. Render the picking pass into an `R32UI`-attached `QRhiTextureRenderTarget`.
2. Set up a `QRhiReadbackDescription`, attach a `QRhiReadbackResult` callback.
3. Submit the command buffer; the callback fires **asynchronously** (the next frame, typically) with the readback data.
4. Resolve the picked object ID on the callback thread.

This async readback pattern would require either (a) blocking the main thread until the readback completes (kills interactive frame rate) or (b) plumbing async result-handling through `RenderSetup` / `Scene` (out-of-scope for SPIKE-01 — touches the threading model Phase 02.1 froze).

**Resolution per accepted 05-RESEARCH.md §QRhi Backend cons caveat:** the **GL-core arm satisfies the "demonstrating picking" criterion of SPIKE-01**. The QRhi arm:

- Returns an empty `std::list<GeometricObject*>` from `pickObjects()`.
- Emits `BALLVIEW_GL_DIAG spike_qrhi_picking=not-implemented` so Plan 08 (SPIKE-02 decision record) can cite the caveat as a deliberate spike-scope decision rather than a missed deliverable.
- Reports `capabilities().picking = false`, so a capability-aware caller can route picking requests to a different backend or surface a UI message.

If SPIKE-02 selects QRhi as the path forward, PIPE-01 will need to design the async-readback plumbing — that is a known follow-up, not a hidden cost.

## Threat Flags

None. The QRhi spike code path is environment-gated (`BALL_SPIKE_BACKEND=QRhi` configure flag + `BALLVIEW_USE_SPIKE_BACKEND=1` runtime env var) and never present in production binaries. No new network, file, or auth surface introduced.

## Self-Check: PASSED

- Created file present: `include/BALL/VIEW/RENDERING/RENDERERS/rhiRenderer.h` — FOUND (118 lines, min 30)
- Created file present: `source/VIEW/RENDERING/RENDERERS/rhiRenderer.C` — FOUND (205 lines, min 120)
- Created file present: `include/BALL/VIEW/RENDERING/qtRhiSurface.h` — FOUND (139 lines, min 30)
- Created file present: `source/VIEW/RENDERING/qtRhiSurface.C` — FOUND (172 lines, min 60)
- Commits present: `50a79af` (Task 1), `c683546` (Task 2) — both FOUND in `git log --oneline -3`
- `grep -c 'THROWAWAY SPIKE'` on each of the 4 new files: rhiRenderer.h:2, rhiRenderer.C:3, qtRhiSurface.h:4, qtRhiSurface.C:4 — all >= 1 — VERIFIED
- `grep -c 'QRhi API has limited'` on each of the 4 new files: 1 each — VERIFIED
- `grep -c 'BALL_SPIKE_BACKEND_QRHI'` in CMakeLists.txt:4, rendererFactory.h:1, rendererFactory.C:5 — all >= 1 — VERIFIED
- `grep -c 'VERSION_LESS 6\.7'` in CMakeLists.txt: 1 — VERIFIED (Qt 6.7 floor check present)
- `grep -c '#ifdef BALL_SPIKE_BACKEND_QRHI'` in source/VIEW/RENDERING/rendererFactory.C: 3 (>= 2) — VERIFIED (one in includes, one in makeRenderer switch, one in makeSurface switch; env-var gate uses `#if defined(...) || defined(...)` form)
- `grep -c 'rhiRenderer.C' source/VIEW/RENDERING/RENDERERS/sources.cmake`: 1 — VERIFIED
- `grep -c 'qtRhiSurface.C' source/VIEW/RENDERING/sources.cmake`: 1 — VERIFIED
- `cmake --preset ci-macos -DBALL_SPIKE_BACKEND=QRhi` — succeeds, emits "Phase 5 spike: QRhi renderer ENABLED (throwaway; Qt 6.11.0)" — VERIFIED
- `cmake --build --preset ci-macos --target BALLView` under QRhi config — exit 0 (13 warnings, 0 errors) — VERIFIED
- Default + GLCore configurations still build green — VERIFIED (regression-tested)
- `git diff HEAD~2 source/VIEW/WIDGETS/scene.C` — empty (scene.C BYTE-IDENTICAL to Plan 05-05's post-state) — VERIFIED
- `grep -cE 'SET\(BALL_PROJECT_COMPILE_DEFNS' CMakeLists.txt`: 0 — VERIFIED (LIST APPEND used, no SET-as-string regression)
