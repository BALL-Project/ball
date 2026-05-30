# Phase 5: Qt 6 Migration (4b) + Renderer Backend Decision Spike - Context

**Gathered:** 2026-05-15
**Status:** Ready for planning

<domain>
## Phase Boundary

BALLView builds and runs against **Qt 6** with the compatibility-profile fixed-function GL path kept as the known-good backend, and a **time-boxed throwaway spike** prototypes the renderer behind the Phase 02.1 `Renderer` / `RenderSurface` / `RendererFactory` boundary to produce a **recorded decision** (GL-core vs. QRhi) that scopes the future `PIPE-01` rewrite. PIPE-01 itself stays out of scope.

The Qt 6 migration (criteria 1–3) lands first; the spike (criteria 4–5) runs against the working Qt 6 build. Plan waves should reflect that order.

**Requirements covered:** QT6-01, QT6-02, SPIKE-01, SPIKE-02.

**In scope:**
- Migrate VIEW + BALLView from Qt 5 to Qt 6 (`QGLWidget`-era APIs fully gone, `QRegExp` / `QDesktopWidget` replaced)
- Keep the compat-profile fixed-function GL path rendering under Qt 6 on all three OSes
- Time-boxed prototype rendering the demo molecule behind `RendererFactory` (with picking + text overlay)
- Per-platform macOS (Apple Silicon) + Windows driver-behaviour checks
- Decision record: chosen backend + rationale + per-platform criteria + scoped PIPE-01 task list

**Out of scope (belongs elsewhere):**
- The full programmable-pipeline rewrite (`PIPE-01` — v2)
- Build-warning + latent-bug cleanup (Phase 5.1)
- New visualization features / UI redesign

</domain>

<decisions>
## Implementation Decisions

### Qt 6 Version Target
- **D-01:** Minimum Qt version pinned to **Qt 6.5 LTS** in CMake — `FIND_PACKAGE(Qt6 6.5 ...)`. Stable, supported through 2026+, available on all three platforms. Newer Qt 6 minors (6.6/6.8/6.9) are acceptable at build time as long as they satisfy the 6.5 floor.
- **D-02:** Linux CI runner **bumped from `ubuntu-22.04` to `ubuntu-24.04`**. Ubuntu 22.04's apt ships Qt 6.2 (below the 6.5 floor); 24.04 ships Qt 6.4/6.5. One-line `matrix.runner` change in `.github/workflows/ci.yml`. Also picks up a newer GCC.
- **D-03:** macOS Qt 6 source = **Homebrew `qt`** (currently 6.9.x bottle, satisfies the 6.5 floor — no Qt online installer / `aqtinstall`, mirrors Phase 4's per-platform dep policy). Windows Qt 6 source = **vcpkg `qt6-base`** (+ `qt6-tools` only if a soft find requires it; otherwise omit to avoid the Phase 4-style cold-build cost).
- **D-04:** Component set is the **Qt 5 list with Qt 6 spellings** — `Qt6::Core`, `Qt6::Network`, `Qt6::Xml`, `Qt6::OpenGL`, `Qt6::PrintSupport`, `Qt6::Test`, `Qt6::Widgets` (+ `Qt6::LinguistTools` optional). **Add `Qt6::OpenGLWidgets`** — Qt 6 split `QOpenGLWidget` out of `Qt::OpenGL` into its own module; this is the new home for `QOpenGLWidget`, which `GLRenderWindow` inherits from (Phase 2). Audit-and-minimize is explicitly out of scope (scope creep).
- **D-05:** **No `Qt6::Core5Compat` shim.** `QRegExp` → `QRegularExpression` and `QDesktopWidget` → `QGuiApplication::screens()` / `QScreen` are replaced inline as part of the Qt 6 migration. Clean break — no carry-over compat module to remove later. Affects ~2 files (`QRegExp`) and ~3 files (`QDesktopWidget`) per the grep in `discuss-phase` analysis.

### macOS GL Profile Under Qt 6
- **D-06:** Force `QSurfaceFormat::CompatibilityProfile` (GL 2.1 / 3.0, whatever Phase 2's existing setup requested) **uniformly on all 3 OSes** in `main.C` / `GLRenderWindow`. One code path, easy to reason about. Verify Phase 2's existing `QSurfaceFormat` setup still takes effect under Qt 6's `QOpenGLWidget` defaults — Qt 6 with no explicit format picks GL 4.1 core on macOS, which would silently break fixed-function and make Phase 02.2's render smoke check fall over.
- **D-07:** macOS GL-deprecation warnings are **left visible at runtime.** No `GL_SILENCE_DEPRECATION` flag, no Info.plist suppression. They signal the spike's deadline — every macOS run prints the deprecation banner as a standing reminder that the fixed-function path is borrowed time. The SPIKE-02 decision record references the noise as a known liability that `PIPE-01` resolves.
- **D-08:** Phase 02.2's render smoke check **must still pass on Qt 6** (criterion 3: no pixel regression vs. Phase 2). The check is a non-blank-PNG assertion on macOS + Linux; macOS compat-profile silently degrading to a black framebuffer under Qt 6 is a **Phase 5 blocker**, not a tolerable warning. No re-baselining the smoke check as part of Phase 5.
- **D-09:** Same `QSurfaceFormat` used on Windows — single code path across all platforms. Windows GL drivers expose compat profile reliably; platform-specific format tuning is deferred until the spike produces actual data justifying divergence.

### Claude's Discretion
- **Spike backend scope** (QRhi vs GL-core vs both) — not locked. Recommendation: prototype **both** behind `RendererFactory::Kind` so the SPIKE-02 decision record can compare with real data. Planner / researcher may propose a leaner scope (single backend, faster cycle time) if research surfaces strong evidence for one direction. Document the chosen scope in `05-RESEARCH.md`.
- **Spike acceptance bar** — not locked. Recommendation: **Comparison-level** (renders demo molecule + picking + text overlay + pixel-equivalent on a canonical scene + a basic FPS number on macOS + Windows). Minimal is too thin to support SPIKE-02's "named winner + rationale + per-platform criteria"; Migration-ready (GUI toggle + how-to-wire docs) edges into PIPE-01 scope. Planner can choose otherwise with rationale.
- **Sequencing the inline `QRegExp` / `QDesktopWidget` replacement** — either as a dedicated early plan inside Phase 5 (clean, bisectable) or folded into the main Qt-6-bring-up plan (fewer commits, harder to attribute regressions). Planner picks.
- **Whether `Qt6::OpenGLWidgets` is found via the existing `FIND_PACKAGE(Qt5 ... OpenGL ...)` block or a separate find** — pure CMake hygiene call.

</decisions>

<canonical_refs>
## Canonical References

**Downstream agents MUST read these before planning or implementing.**

### Phase Goal & Requirements
- `.planning/ROADMAP.md` §Phase 5 — goal, success criteria 1–5, sequencing note, "UI hint: yes"
- `.planning/REQUIREMENTS.md` §Qt 6 Migration + §Renderer Backend Spike — QT6-01, QT6-02, SPIKE-01, SPIKE-02

### Renderer Boundary (Phase 02.1 — the contract Phase 5 swaps behind)
- `.planning/RENDERER-INTERFACE-BOUNDARY.md` — full design of the `Renderer` / `RenderSurface` / `RendererFactory` boundary; the "spike adds a new `Renderer` subclass and registers a new `Kind`" story lives here
- `.planning/phases/02.1-renderer-boundary-extraction/02.1-VERIFICATION.md` — what's actually in place
- `include/BALL/VIEW/RENDERING/RENDERERS/renderer.h` — abstract base; `renderRepresentations_()` + `capabilities()` are the additive entry points
- `include/BALL/VIEW/RENDERING/renderSurface.h` — surface interface (`beginFrame` / `endFrame` / `nativeHandle`)
- `include/BALL/VIEW/RENDERING/rendererFactory.h` — `RendererFactory::Kind` enum + `makeRenderer` / `makeSurface`; **new spike backends register new `Kind` values here**

### Qt 5 → Qt 6 Surface Inventory (what files this phase touches)
- `include/BALL/VIEW/RENDERING/glDisplayList.h` — already migrated off `QtOpenGL/qgl.h` to `QtGui/qopengl.h` (Phase 02.2); verify Qt 6 keeps this clean
- `source/VIEW/RENDERING/glRenderWindow.C` + `include/BALL/VIEW/WIDGETS/scene.h` — the Phase 2 `QOpenGLWidget` port, where `QSurfaceFormat` is configured
- `source/APPLICATIONS/BALLVIEW/main.C` — global context-sharing setup; site of the `QSurfaceFormat` request
- `source/VIEW/DIALOGS/stageSettings.C`, `source/VIEW/WIDGETS/pyWidget.C`, `source/VIEW/KERNEL/MODES/interactionMode.C` — remaining `QGLWidget` / `QDesktopWidget` sites surfaced by grep
- `include/BALL/VIEW/WIDGETS/pyWidget.h` — header-side `QGLWidget` reference (audit during Qt 6 bring-up)

### Build / CI (where the Qt 6 + Ubuntu 24.04 changes land)
- `CMakeLists.txt:293` — `FIND_PACKAGE(Qt5 ${QT_MIN_VERSION} REQUIRED Core Network Xml)` (becomes Qt6 with min 6.5)
- `CMakeLists.txt:295-311` — `BALL_DEP_LIBRARIES` Qt component list + the `IF(BALL_HAS_VIEW)` Qt6::OpenGL / PrintSupport / Test / Widgets block; add `Qt6::OpenGLWidgets` here
- `.github/workflows/ci.yml` — `matrix.runner: ubuntu-22.04` flips to `ubuntu-24.04`; apt package list updates for `qt6-base-dev` + `qt6-tools-dev` + `libqt6opengl6-dev`
- `CMakePresets.json` — `linux-system` / `ci-linux` presets, if any path assumptions baked in for Qt 5

### Codebase Maps (background)
- `.planning/codebase/STACK.md` §Graphics & Rendering, §Core (Qt 5 components)
- `.planning/codebase/ARCHITECTURE.md` §VIEW Layer + Rendering pipeline (RenderSetup / RendererFactory / GLRenderer / GLRenderWindow)

### Phase-3/4 Decisions That Carry Forward
- `.planning/STATE.md` Accumulated Context / Decisions — C++17 baseline (Phase 3), compat-profile fixed-function GL kept (Phase 2 A1 CONFIRMED), `ball_contrib` gone (Phase 4), `QT_NO_KEYWORDS` enabled, `Boost::regex` linking pattern
- `.planning/phases/02-rendering-port-4a/02-RESEARCH.md` — Phase 2 / 4a research; informs which Qt 6 quirks were already navigated for Qt 5 (lazy GL context, HiDPI device pixel viewport, paintGL routing through Scene's event filter)

</canonical_refs>

<code_context>
## Existing Code Insights

### Reusable Assets
- **Phase 02.1 renderer boundary** is the load-bearing reusable asset. New backends plug in as `RendererFactory::Kind` values; `scene.C` already routes through the factory. The spike's GL-core / QRhi prototypes are "new `Renderer` subclasses + new `RenderSurface` impls", not a re-architecture.
- **Phase 2's `QOpenGLWidget` infrastructure** — `GLRenderWindow` inherits `QOpenGLWidget` with a compat-profile `QSurfaceFormat`, manual swap removed, GL work in `initializeGL` / `resizeGL` / `paintGL`, global context sharing in `main()`. The Qt 6 migration keeps this; only Qt-namespace and `Qt6::OpenGLWidgets` linkage change.
- **Phase 02.2 render smoke check** (`scripts/render-smoke-check.sh` + the `-export-png` flag in `main.C`) is the regression net for D-08. Already CI-wired on macOS + Linux.
- **Phase 4's `CMakePresets.json` + `vcpkg.json`** — adding `qt6-base` to the vcpkg manifest (replacing `qt5-base`) is the Windows provisioning lever. macOS/Linux update via `BUILD-macos.md` / apt list. The `x64-windows-release` overlay triplet + `actions/cache` of `%LOCALAPPDATA%\vcpkg\archives` carry forward; the cold-build cost for qt6 will hit the first CI run after the vcpkg.json change.

### Established Patterns
- **`BALL_DEP_LIBRARIES` / `VIEW_DEP_LIBRARIES`** lists are the canonical link surface — Qt components are appended here (`CMakeLists.txt:295`, `:308`); follow the existing list-append pattern when adding `Qt6::OpenGLWidgets`.
- **Per-OS branches in `BALLConfigBoost.cmake` and `glRenderer.h`** show the project's "platform-specific include / link" idiom — Qt 6 changes should follow the same `IF(BALL_OS_DARWIN)` / `IF(BALL_COMPILER_MSVC)` style if needed.
- **`BALL_PROJECT_COMPILE_DEFNS` is a CMake list** (Plan 04-04 lesson) — any new Qt-6-specific `-D` (e.g., `QT_NO_DEPRECATED_WARNINGS_SINCE`) must use `LIST(APPEND ...)`, not `SET("... string")`. Mixing styles produces ADD_DEFINITIONS garbage on MSVC.
- **Renderer construction in `scene.C`** goes through `RendererFactory::makeRenderer(Kind)` — Phase 02.1 cleared the `new GLRenderWindow` / `dynamic_cast<GLRenderer>` sites. The spike's new backend `Kind`s slot in cleanly here.

### Integration Points
- `main.C` `QSurfaceFormat` request — single site that determines GL profile across platforms. D-06 (uniform compat profile) lands here.
- `RendererFactory::Kind` enum + `makeRenderer` / `makeSurface` switches — where SPIKE-01's prototype backends register. New enum values for `GLCoreRenderer` and/or `QRhiRenderer`, new switch cases.
- `Renderer::capabilities()` — spike backends report capabilities here; `RenderSetup` / `Scene` already branch on capability, so no upstream changes needed in the spike phase.
- `.github/workflows/ci.yml` `matrix.runner` for Linux + the apt install list for Qt 6 — D-02 lands here.

</code_context>

<specifics>
## Specific Ideas

- **The renderer boundary doc (`.planning/RENDERER-INTERFACE-BOUNDARY.md`) is the load-bearing reference** for the spike. It already enumerates the four leaks Phase 02.1 fixed, names the target boundary (RenderBackend, RenderSurface, RenderTarget), and explicitly frames Phase 5 as "implement one new backend + register it". Researcher and planner should reference it section-by-section, not just cite it.
- **The "spike's deadline" framing for macOS GL-deprecation warnings** (D-07) is intentional. They're left noisy as a standing reminder that fixed-function compat-profile on macOS is a stopgap, not a destination. SPIKE-02 must reference this noise as one of the concrete motivations for `PIPE-01`.
- **No `Qt6::Core5Compat` shim** (D-05) is a deliberate clean break. Some other Qt-5→Qt-6 migrations carry Core5Compat indefinitely; BALLView v1.6 explicitly avoids that path.

</specifics>

<deferred>
## Deferred Ideas

- **Spike backend scope decision** (QRhi vs GL-core vs both) — left to planner / researcher discretion; locked default is "both, with rationale in 05-RESEARCH.md if the planner trims to one"
- **Spike acceptance bar** — left to planner discretion; locked default is "Comparison-level (renders + picks + text overlay + pixel-equivalent on canonical scene + basic FPS on macOS+Windows)"
- **PIPE-01 itself** — the full programmable-pipeline rewrite. v2. SPIKE-02 scopes the task list; the work is a separate future phase.
- **Phase 5.1 build-warnings/latent-bugs cleanup** — separate phase that waits on Phase 5 (Qt 5 deprecation noise clears once Qt 6 lands). Backlog at `.planning/phases/05.1-build-warnings-and-latent-bugs/05.1-BACKLOG.md`.
- **`-Wdeprecated-declarations` cleanup at large** — bulk of it disappears with Qt 6; remainder lives in Phase 5.1 Tier C.
- **Audit-and-minimize Qt component set** — rejected as scope creep (D-04). Future cleanup phase if it surfaces a meaningful win.
- **Per-OS `QSurfaceFormat` tuning** — rejected for Phase 5 (D-09 keeps it uniform); revisit only if the spike produces evidence one platform genuinely needs a different profile.

</deferred>

---

*Phase: 5-qt-6-migration-4b-renderer-backend-decision-spike*
*Context gathered: 2026-05-15*
