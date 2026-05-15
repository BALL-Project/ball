# Requirements: BALLView 1.6 Modernization

**Defined:** 2026-05-14
**Core Value:** BALLView must build and visibly render molecules on macOS, Linux, and Windows from current, supported dependencies.

## v1 Requirements

Requirements for the v1.6 release. Derived from `/Users/kohlbach/Claude/BALL/ROADMAP-1.6.md`.

### Build Baseline

- [x] **BUILD-01**: BALL/VIEW/BALLView configure and build on macOS (Apple Silicon) from Homebrew dependencies with a documented command
- [x] **BUILD-02**: The 8 modern-toolchain patches are committed to the repo with clear messages
- [x] **BUILD-03**: Project version is bumped to 1.6.0-dev in the CMake project declaration
- [x] **BUILD-04**: A `BUILD-macos.md` documents the dependency install and build/run flow

### Language Modernization

- [x] **LANG-01**: The codebase compiles under C++17 (the C++14 bridge flag is removed)
- [x] **LANG-02**: All uses of C++17-removed constructs (`std::unary_function`, `binary_function`, `bind2nd`, `bind1st`, `ptr_fun`, `auto_ptr`, `mem_fun`) are replaced across the 7 known files
- [x] **LANG-03**: C++ standard is set via `CMAKE_CXX_STANDARD`/`target_compile_features`, not raw `-std=` flags

### Dependency System

- [x] **DEPS-01**: `ball_contrib` is removed from the build path; CMake finds dependencies via system/Homebrew packages
- [ ] **DEPS-02**: A vcpkg manifest provides the Windows dependency set
- [x] **DEPS-03**: Stale bundled `Find*.cmake` modules (Boost, TBB, Eigen3, OpenBabel) are replaced with config-mode `find_package` where upstream provides it
- [x] **DEPS-04**: Minimum dependency versions are pinned and documented
- [x] **DEPS-05**: A `CMakePresets.json` provides stable configure presets for macOS-Homebrew, Linux-system, Windows-vcpkg, and CI
- [x] **FEAT-01**: A feature matrix classifies every optional dependency as required / optional / removed / deferred and states what each one's absence disables (see the "Feature Matrix" section below)

### Configuration (Phase 4.1 — promoted from backlog 999.4)

- [x] **CONFIG-01**: The persisted `~/.BALLView` config no longer silently shadows compiled element/residue color defaults — compiled defaults show through for un-customized colors, a stale/partial saved color block falls back to compiled defaults, and the user can reset colors to defaults from Preferences without deleting `~/.BALLView`

### Rendering — Phase 4a (IMMEDIATE PRIORITY)

- [x] **RENDER-01**: `GLRenderWindow` derives from `QOpenGLWidget` (not the removed-in-Qt6 `QGLWidget`); `QGLFormat` is replaced by `QSurfaceFormat` requesting a compatibility profile + depth/stencil/double buffer
- [ ] **RENDER-02**: BALLView displays a molecule in the embedded 3D scene on macOS (Apple Silicon) — the scene widget renders inside the main window, not a detached native window
- [x] **RENDER-03**: The interactive GL renderer runs on the GUI thread via `paintGL()`; manual `swapBuffers()`/`setAutoBufferSwap` are removed
- [ ] **RENDER-04**: The raytracer renderer continues to work via its CPU pixel-buffer path, blitted to the `QOpenGLWidget` as a texture
- [x] **RENDER-05**: On-screen text rendering (formerly `QGLWidget::renderText`) is reimplemented via a `QPainter` overlay
- [ ] **RENDER-06**: Mouse/keyboard interaction (rotate, zoom, pick, selection) works in the ported scene widget
- [x] **RENDER-07**: `RenderSetup`, `scene.C`, `glOffscreenTarget.C`, `glRenderer.C` are updated to remove all `QGLWidget`/`QGLContext`/`QGLFormat` references and compile cleanly
- [ ] **RENDER-08**: BALLView builds, launches, and renders a molecule on Linux and Windows (platform-independence verified, no regressions vs. macOS)

### Renderer Architecture (Phase 02.1 — inserted)

- [x] **ARCH-01**: A `RenderSurface` interface owns context-lifecycle verbs (`beginFrame`/`endFrame`); `RenderSetup::makeCurrent()`'s GL-specific body moves behind it
- [ ] **ARCH-02**: A `RendererFactory` constructs renderers/surfaces by enum; `scene.C` has zero `new GLRenderWindow` and zero `dynamic_cast<GLRenderWindow>`/`dynamic_cast<GLRenderer>` sites
- [x] **ARCH-03**: The `Renderer` interface gains a batched `renderRepresentations_()` + `capabilities()` entry point; existing immediate-mode renderers are untouched
- [ ] **ARCH-04**: BALLView builds and renders identically to post-Phase-2 (pure refactor, no behaviour change)

### CI & Diagnostics (Phase 02.2 — inserted)

- [ ] **CI-01**: A GitHub Actions matrix builds BALL/VIEW/BALLView on macOS-arm64, Linux, and Windows on every push, plus a headless render smoke check (load a known molecule, capture the framebuffer, assert non-blank pixels) and the legacy-GL-symbol grep lint
- [x] **DIAG-01**: BALLView logs a startup GL-capability diagnostic — GL vendor/version/profile, `QSurfaceFormat`, device-pixel ratio, default FBO size, selected renderer backend — as a debugging aid and the render smoke check's oracle

### Qt 6 Migration (Phase 5)

- [ ] **QT6-01**: BALLView builds against Qt 6, with `QGLWidget`-era APIs fully removed
- [ ] **QT6-02**: Qt-deprecated APIs in VIEW (`QRegExp`, `QDesktopWidget`) are replaced with Qt 6 equivalents

> **QT6-03 removed (Codex review):** "the rendering pipeline is modernized off
> fixed-function GL" directly contradicted `PIPE-01` (full pipeline rewrite =
> v2/out-of-scope). The pipeline rewrite is `PIPE-01`, a separate future phase.
> Phase 5 keeps the compatibility-profile fixed-function path working under Qt 6.

### Renderer Backend Spike (Phase 5 — folded in from former Phase 05.1)

- [x] **SPIKE-01**: A throwaway prototype renders the demo molecule through at least the leading backend candidate (GL-core and/or QRhi) behind the Phase 02.1 `RendererFactory`, demonstrating picking and a text overlay — *delivered with documented caveats per Plans 05-05 (GL-core arm + picking) + 05-06 (QRhi arm) + 05-07 (per-platform driver record); downstream-virtual full-render limitation is PIPE-01 scope per `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-SPIKE-DECISION.md` §5.4*
- [x] **SPIKE-02**: A decision record names the chosen backend, the rationale, per-platform (macOS/Windows) acceptance criteria, and a scoped task list for the `PIPE-01` full rewrite — *`.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-SPIKE-DECISION.md` — split decision: GL-Core for v1.6.x → QRhi for v2*

### Python Bindings (Phase 6)

- [ ] **PY-01**: A vertical slice binds and imports 5-10 representative core BALL classes for the candidate generator(s), proving ownership/lifetime, exception translation, STL-container handling, and build packaging
- [ ] **PY-02**: A decision record names the chosen binding generator (SIP 6 or pybind11/nanobind) with rationale; the chosen generator builds against Python 3.12+ and the slice module imports and exercises core BALL classes

### Packaging & Distribution (Phase 8)

- [ ] **PKG-01**: `BALLView.app` embeds its `data/` directory into `Contents/Resources` and launches by double-click without environment variables set
- [ ] **PKG-02**: The macOS build produces a `macdeployqt`-processed, notarizable universal (arm64 + x86_64) bundle
- [ ] **PKG-03**: `BUILD-linux.md` + `BUILD-windows.md` document the from-source build per platform, and a license/distribution review covers the FFTW GPL path, OpenBabel, Qt deployment mode, and bundled `data/`

### Test Suite (Phase 9)

- [ ] **CI-02**: The `test/` tree is wired into the build and `ctest` runs in CI, with failures triaged (fixed / quarantined with a note / documented as a known modernization casualty)

## Feature Matrix

Status of optional components for v1.6 — produced/verified by **FEAT-01** in Phase 4, confirmed against Phase 4 research. Initial classification from the Phase 1 review; Phase 4 adds per-platform availability notes and states the user-visible impact of absence.

**D-06 policy:** A missing optional dependency auto-disables its features with a clear configure-time `MESSAGE(STATUS ...)` line — it is never a hard configure error.

| Component | v1.6 status | Platform availability | If absent, disables |
|-----------|-------------|----------------------|---------------------|
| Qt 5.15 (→ Qt 6 in Phase 5) | **Required** | macOS / Linux / Windows | Everything (BALLView is a Qt app) |
| Boost | **Required** | macOS / Linux / Windows | Core BALL build |
| Eigen | **Required** | macOS / Linux / Windows | Core BALL build |
| GLEW | **Required-when-VIEW** | macOS / Linux / Windows (vcpkg `glew` 2.3.1) | VIEW/BALLView build (when `BALL_HAS_VIEW=ON`) |
| FFTW | **Required-for-GPL-builds-only** | macOS / Linux (GPL build only; absent in the default LGPL build) | Fourier-transform algorithms (GPL builds only) |
| OpenBabel | **Optional** (3.x; was OFF in Phase 1) | macOS: ON (ci-macos preset); Linux: ON (ci-linux preset); Windows: OFF (not a vcpkg port) | `MolecularSimilarity` + 4 TOOLS (`MolDepict`, `ProteinProtonator`, `Ligand3DGenerator`, `MolFilter`) |
| TBB | **Optional** (oneTBB; was OFF in Phase 1) | macOS / Linux / Windows (vcpkg `tbb` 2022.3.0); enabled-when-found | Parallel speedups; no functional loss when absent |
| LPSolve | **Optional** (was OFF in Phase 1) | macOS: ON; Linux CI: OFF (Ubuntu `liblpsolve55.a` is non-PIC, cannot link into shared libBALL); Windows: OFF (not a vcpkg port) | LP-based features (e.g. bond-order assignment ILP path) |
| libSVM | **Optional** (found in Phase 1) | macOS / Linux / Windows (vcpkg `libsvm` 3.35); thin finder shim (no upstream config) | SVM-based QSAR features |
| QtWebEngine | **Optional / deferred** | None (already disabled — `qt@5` has no WebEngine) | PresentaBALL, BALLaxy, Jupyter plugins |
| RTfact raytracer | **Removed** | N/A — not built on any platform | Windows-only contrib; not built — the CPU raytracer path remains |
| Python bindings (SIP) | **Deferred to Phase 6** | Pending Phase 6 generator decision | The Python interface (re-established via the Phase 6 generator decision) |
| VRPN / SpaceNavigator | **Removed** | N/A — not built on any platform | Exotic input-device plugins; not built |

## Deferred (1.6.x)

Moved out of the v1.6 active scope per the Codex adversarial review — tracked but not gating the milestone.

### Networking

- **NET-01**: `TCPServer`/`TCPServerThread` are reworked onto the modern Boost.Asio acceptor/socket model and covered by a unit test. *(Backlog 999.3. The Asio API breakage is already fixed and compiling as of Phase 1; this is the proper rework + test.)*

## v2 Requirements

Deferred beyond v1.6.

### Pipeline

- **PIPE-01**: Full programmable-pipeline rewrite of `glRenderer.C` off fixed-function GL

## Out of Scope

| Feature | Reason |
|---------|--------|
| Reviving `ball_contrib` source build | Dead end on modern toolchains; replaced by system/Homebrew/vcpkg deps |
| New molecular-modelling features/algorithms | This milestone is modernization only |
| RTfact raytracer revival | Windows-only contrib, not load-bearing |
| Programmable-pipeline GL rewrite (`PIPE-01`) | Large; Phase 2 + Phase 5 keep fixed-function via a compat profile. The Phase 5 backend spike de-risks and scopes it; the full rewrite is v2 |

## Traceability

GSD phase numbers are the canonical scheme used everywhere. The original human-authored `ROADMAP-1.6.md` labels (Phase "4a"/"4b") are kept only as aliases in phase titles.

| Requirement | GSD Phase | Status |
|-------------|-----------|--------|
| BUILD-01 | Phase 1 — Build Baseline | Complete |
| BUILD-02 | Phase 1 — Build Baseline | Complete |
| BUILD-03 | Phase 1 — Build Baseline | Complete |
| BUILD-04 | Phase 1 — Build Baseline | Complete |
| RENDER-01 | Phase 2 — Rendering Port (4a) | Complete |
| RENDER-02 | Phase 2 — Rendering Port (4a) | Complete — human-verified on macOS |
| RENDER-03 | Phase 2 — Rendering Port (4a) | Complete |
| RENDER-04 | Phase 2 — Rendering Port (4a) | Complete — human-verified on macOS |
| RENDER-05 | Phase 2 — Rendering Port (4a) | Complete |
| RENDER-06 | Phase 2 — Rendering Port (4a) | Complete — human-verified on macOS |
| RENDER-07 | Phase 2 — Rendering Port (4a) | Complete |
| RENDER-08 | Phase 2 — Rendering Port (4a) | Carry-forward — Linux/Windows render verified via Phase 02.2 CI |
| ARCH-01 | Phase 02.1 — Renderer boundary extraction | Complete |
| ARCH-02 | Phase 02.1 — Renderer boundary extraction | Complete |
| ARCH-03 | Phase 02.1 — Renderer boundary extraction | Complete |
| ARCH-04 | Phase 02.1 — Renderer boundary extraction | Complete |
| CI-01 | Phase 02.2 — CI and build-smoke matrix | Complete |
| DIAG-01 | Phase 02.2 — CI and build-smoke matrix | Complete |
| LANG-01 | Phase 3 — Language Modernization | Complete |
| LANG-02 | Phase 3 — Language Modernization | Complete |
| LANG-03 | Phase 3 — Language Modernization | Complete |
| DEPS-01 | Phase 4 — Dependency System Overhaul | Complete |
| DEPS-02 | Phase 4 — Dependency System Overhaul | Pending |
| DEPS-03 | Phase 4 — Dependency System Overhaul | Complete |
| DEPS-04 | Phase 4 — Dependency System Overhaul | Complete |
| DEPS-05 | Phase 4 — Dependency System Overhaul | Complete |
| FEAT-01 | Phase 4 — Dependency System Overhaul | Complete |
| CONFIG-01 | Phase 4.1 — Config Color-Defaults Fix | Complete |
| QT6-01 | Phase 5 — Qt 6 Migration (4b) | Complete on macOS — runtime smoke verified `3238ce3`; macOS CI `gl_profile=(compatibility\|none)` `7b28685`; Linux/Windows deferred to backlog 999.7 |
| QT6-02 | Phase 5 — Qt 6 Migration (4b) | Complete on macOS — migration sweep + CR-01/02/03 fixed inline (`81569ee` `339cb99` `3917c10`); WR-01..07 fixed inline |
| SPIKE-01 | Phase 5 — Qt 6 Migration + Renderer Backend Spike | Complete — `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-SPIKE-DECISION.md` |
| SPIKE-02 | Phase 5 — Qt 6 Migration + Renderer Backend Spike | Complete — `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-SPIKE-DECISION.md` |
| PY-01 | Phase 6 — Python Bindings | Pending |
| PY-02 | Phase 6 — Python Bindings | Pending |
| PKG-01 | Phase 8 — Packaging & Distribution | Pending |
| PKG-02 | Phase 8 — Packaging & Distribution | Pending |
| PKG-03 | Phase 8 — Packaging & Distribution | Pending |
| CI-02 | Phase 9 — Test Suite Triage | Pending |
| NET-01 | Deferred (1.6.x) — backlog 999.3 | Deferred |
| PIPE-01 | Deferred (v2) — backlog 999.6 — *de-risked + scoped by Phase 5 SPIKE-02* | Deferred |

**Coverage:**
- v1 requirements: 38 active (BUILD ×4, RENDER ×8, ARCH ×4, CI/DIAG ×2, LANG ×3, DEPS ×6, CONFIG ×1, QT6 ×2, SPIKE ×2, PY ×2, PKG ×3, CI-02 ×1) + NET-01 deferred to 1.6.x
- Mapped to phases: 38 ✓
- Unmapped: 0
- v2: PIPE-01 (full pipeline rewrite — now de-risked by the Phase 5 backend spike)

---
*Requirements defined: 2026-05-14*
*Last updated: 2026-05-14 — Codex structural changes applied: Phase 02.2 (CI) + Phase 05.1 (backend spike) inserted; Phase 5 split (Qt6-only); Phase 6 restructured (decision+slice); Phase 8 scope clarified; DEPS-05/FEAT-01/DIAG-01/SPIKE/PY-02/PKG-03 added; feature matrix added.*
*Updated 2026-05-14 (consolidation): CONFIG-01 added → Phase 4.1 (promoted from backlog 999.4); SPIKE-01/SPIKE-02 re-mapped from former Phase 05.1 into Phase 5 (folded in). Active count 37 → 38.*
