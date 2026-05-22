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

## v1.6.2 Requirements (Build acceleration + scope-cleanup patch release)

Added 2026-05-16 when v1.6.2 milestone opened. Each REQ maps to one of the 7 in-scope phases + 1 stretch from [`MILESTONE-CONTEXT.md`](MILESTONE-CONTEXT.md).

### Build acceleration cluster

- [x] **BUILD-ACCEL-01** (Phase 999.16): `target_precompile_headers(BALL PRIVATE ...)` + `target_precompile_headers(VIEW PRIVATE ...)` wired with auditable header sets; ccache `sloppiness = pch_defines,time_macros,include_file_mtime` configured on all 3 CI runners; cold-cache Windows Build step measurably reduced (target ≥15%); decision documented in `cmake/PCH.md`
- [x] **BUILD-ACCEL-02** (Phase 999.17): Windows CMake build tree (`build/ci-windows/{CMakeCache.txt,CMakeFiles/,build.ninja,vcpkg_installed/}`) cached across CI runs via `actions/cache`, keyed on hash of `CMakeLists.txt`+`cmake/**`+`vcpkg.json`+`CMakePresets.json`; warm-cache `Configure (Windows)` step collapses from ~2.5 min to seconds
- [x] **BUILD-ACCEL-03** (Phase 999.18): CI workflow triggers are path-aware (planning-only / doc-only changes skip the build matrix); concurrency groups cancel obsolete in-flight runs on rapid pushes to the same branch
- [x] **BUILD-ACCEL-04** (Phase 999.19): Per-TU build profiling artifact (Ninja `.ninja_log` + parsed timing breakdown) uploaded as a workflow artifact for every Build job; identifies compile-time outliers for future targeted optimization
- [x] **BUILD-ACCEL-05** (Phase 999.20): GitHub Actions artifact pins bumped — `upload-artifact@v4 → v6`, `download-artifact@v4 → v7`; CI still green on a tri-OS run post-bump
- [x] **BUILD-ACCEL-06** (Phase 999.50 · v1.7.2): `boost::serialization` eliminated from `source/DOCKING/COMMON/poseClusteringSerialization.C` — replaced with a hand-rolled magic+version-headed reader/writer for the Ward cluster tree; public API (`serializeWardClusterTree`/`deserializeWardClusterTree`) and the `bool binary` flag unchanged; committed fixture `test/data/PoseClustering_wardtree.dat` regenerated in the new format; `DockPoseClustering` writer + `ExtractClustersFromWardTree` reader rebuilt against it; `PoseClustering_test3` (round-trip + fixture read → 6 clusters @ 0.5) and `PoseClustering_test1` (graphviz) green; the serialization TU compiles in seconds with no `boost/archive` or `adj_list_serialize` includes remaining; format break noted in RELEASE-NOTES "Breaking changes"
- [x] **BUILD-ACCEL-07** (Phase 999.50 · v1.7.2): `release.yml` restores/saves an `actions/cache` for the ccache dir (key aligned with `ci.yml` where possible) so a release tag of an already-CI-built commit hits warm cache instead of building cold

### Source-level + cleanup

- [x] **TEST-CLOSE-01** (Phase 9): Three baseline failures from `PHASE-9-BASELINE.md` triaged — `Directory_test` (KNOWN-PASSING — false alarm), `AmberFF_test` (FIX: ARM FP tolerance loosen, commit 41bfae621), `AssignBondOrderProcessor_test2` (QUARANTINE: WILL_FAIL TRUE, commit edfa0857a) — each fixed, quarantined with a note, or documented; 09-TRIAGE.md decision record at `.planning/phases/09-test-suite-triage/09-TRIAGE.md` — Complete 2026-05-16
- [x] **TEST-CLOSE-02** (Phase 9): CI test gatekeeper flipped from `continue-on-error: true` to blocking on macOS + Linux in ci.yml + release.yml; tri-OS CI green post-flip — Complete 2026-05-16
- [x] **TRIAGE-01** (Phase 999.14): Open issues + open PRs on `BALL-Project/ball` categorized into the 5 audit buckets (close-as-fixed / close-as-obsolete / close-as-stale / keep / needs-investigation), bulk-closed with templated comments per category; decision audit trail in `.planning/triage-999.14/decisions.md` — Complete 2026-05-16 (3 rounds: ext. round + maintainer batch + reconciliation; 82 net closes; 99 kept; 2 needs-investigation)
- [x] **TRIAGE-02** (Phase 999.14): Stale-doc audit task — all `.planning/phases/**/*VERIFICATION.md` files scanned for the HUMAN-UAT-disagreement pattern; affected files annotated with the f176b8b banner + `resolution_log:` frontmatter — Complete 2026-05-16 (7 files inspected; 1 had UAT sibling; f176b8b pattern already applied to 05-VERIFICATION.md in commit f176b8b; no new banners needed)
- [x] **TRIAGE-03** (Phase 999.14): The 5-PR legacy bundle (#640 FindXDR, #600 Travis-CI, #554 Omega torsion, #550 hydroxyproline, #546 residue insertion code) merged or closed per categorization in TRIAGE-01 — Complete 2026-05-16 (#600 closed-obsolete, #640 merged 9c6d868, #554/#546/#550 C++ slice merged 45dce69; bundle-escape DID NOT FIRE)

### Tiny dead-code + grammar cleanups

- [x] **DEADCODE-01** (Phase 999.21): Stubbed `DockResultFile::attributesToHashMap(const QXmlAttributes&)` Qt 5 SAX overload removed from header + impl; CMake build still green — Complete 2026-05-16 (commit 914622d8f; 30 lines removed; local macOS BALL build green; Phase 5 BLOCKER-A closed)
- [x] **WARN-CENSUS-01** (Phase 999.22, CENSUS-ONLY): Tri-OS warning census published at `.planning/phases/999.22-warning-census/CENSUS.md` with per-warning-category × file-path × likely-cause categorization into (a)/(b)/(c) deferral buckets; three follow-up backlog stubs filed (999.22a/b/c). **No warning execution in v1.6.2** per Open Q3 resolution. — Complete 2026-05-16 (CI run 25970862407; 200 remaining -Wdeprecated-copy on Linux vs pre-fix estimate ~3,716; 95% reduction from parallel-session; 999.22a/b/c stubs filed in ROADMAP.md)
- [ ] **GRAMMAR-01** (Phase 999.23, STRETCH — may defer to v1.7): CIF Bison grammar shift-reduce conflict count reconciled (3 vs 5 per BACKLOG/SUMMARY); each conflict either confirmed-benign-and-documented or fixed at the grammar rule

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
| CI-02 | Phase 9 — Test Suite Triage | Pending — CI wiring landed (commits `b2bb718`+`61bf5a7`); 3 failures remain — see TEST-CLOSE-01/02 (v1.6.2) |
| NET-01 | Deferred (1.6.x) — backlog 999.3 | Deferred |
| PIPE-01 | Deferred (v2) — backlog 999.6 — *de-risked + scoped by Phase 5 SPIKE-02* | Deferred |
| BUILD-ACCEL-01 | Phase 999.16 — Build acceleration: PCH | Complete (v1.6.2, 2026-05-16) — Windows cold -35.6% (4818s→3103s), Linux -15%, macOS -13% (AppleClang excluded) |
| BUILD-ACCEL-02 | Phase 999.17 — Build acceleration: Windows tree cache | Complete (v1.6.2, 2026-05-16) — warm Configure -57.7% (149s→63s); restore-key fallback on structural change 76s |
| BUILD-ACCEL-03 | Phase 999.18 — CI hygiene: path-aware triggers + concurrency | Complete (v1.6.2, 2026-05-16) — paths-ignore lands in d10da9a (+doc/** top-up); de94e86 docs-only push empirically skipped CI |
| BUILD-ACCEL-04 | Phase 999.19 — CI hygiene: per-TU build profiling artifact | Complete (v1.6.2, 2026-05-16) — ninja-log artifacts (90-day retention) uploaded per platform; top-20 slowest TUs printed inline; baseline data captured (dockResultFile.C top outlier at 14.6s Windows / 7.6s Linux / 4.4s macOS) |
| BUILD-ACCEL-05 | Phase 999.20 — CI hygiene: action artifact pins v6/v7 | Complete (v1.6.2, 2026-05-16) — upload v4→v6 in 1d3da02, download v4→v7 in 3cb03c8; CI run 25970222837 green tri-OS + linux-arm64 + coverage |
| TEST-CLOSE-01 | Phase 9 — Test Suite Triage | Complete (v1.6.2, 2026-05-16) — Directory_test KNOWN-PASSING; AmberFF_test FIX 41bfae621; AssignBondOrderProcessor_test2 QUARANTINE edfa0857a; 09-TRIAGE.md |
| TEST-CLOSE-02 | Phase 9 — Test Suite Triage | Complete (v1.6.2, 2026-05-16) — gatekeeper flipped to blocking in ci.yml + release.yml; tri-OS CI green post-flip |
| TRIAGE-01 | Phase 999.14 — GitHub issue + PR triage | Complete (v1.6.2, 2026-05-16) — 82 net closes; audit trail in decisions.md |
| TRIAGE-02 | Phase 999.14 — Stale-doc audit bundled task | Complete (v1.6.2, 2026-05-16) — 7 VERIFICATION.md files audited; f176b8b already applied; STALE-DOCS-AUDIT.md |
| TRIAGE-03 | Phase 999.14 — 5-PR legacy bundle merge | Complete (v1.6.2, 2026-05-16) — #600 closed-obsolete; #640 merged 9c6d868; #554/#546/#550 C++ slice merged 45dce69 |
| DEADCODE-01 | Phase 999.21 — DockResultFile QtXml dead-code cleanup | Complete (v1.6.2, 2026-05-16) — commit 914622d8f; 30 lines removed; BALL macOS build green |
| WARN-CENSUS-01 | Phase 999.22 — Tier-C warning census (CENSUS-ONLY) | Complete (v1.6.2, 2026-05-16) — CENSUS.md at phases/999.22-warning-census/; 200 remaining -Wdeprecated-copy on Linux; 999.22a/b/c stubs filed |
| GRAMMAR-01 | Phase 999.23 — CIF Bison grammar audit (STRETCH) | Pending (v1.6.2 or v1.7) |

**Coverage:**
- v1 requirements: 38 active + NET-01 deferred to 1.6.x
- v1.6.2 requirements: 12 (BUILD-ACCEL ×5 + TEST-CLOSE ×2 + TRIAGE ×3 + DEADCODE-01 + WARN-CENSUS-01) + GRAMMAR-01 stretch
- Mapped to phases: 50 active + 1 deferred + 1 v2 ✓
- Unmapped: 0
- v2: PIPE-01 (full pipeline rewrite — now de-risked by the Phase 5 backend spike)

## v1.7.x patch-cycle requirements

Added 2026-05-21 when the v1.7.x patch-cycle runway (Phases 999.51–999.56) was registered from [`v1.7.x-PATCH-QUEUE.md`](v1.7.x-PATCH-QUEUE.md). Each REQ maps to one phase.

- [x] **VIEW-INSP-01** _(✓ UAT PASS 2026-05-21 — clipping renders + visibly clips)_ (Phase 999.51 · v1.7.x-01): `ClippingController::apply()` cut over from log-only stub to functional apply (reusing the legacy Clipping dialog's parameter parsing); a `ClippingSection` added to the Inspector Representation tab mirroring the Model/Coloring/Material section pattern; `STUB` marker removed — _IMPLEMENTATION-COMPLETE / AWAITING GUI HUMAN-VERIFY (Plan 999.51-01 commits 81118c56e3/87ea5544e3/995da19d91; VIEW build green BALL_UI_V2=ON; STUB removed; legacy ClippingDialog untouched). Checkbox flips to [x] after the blocking running-BALLView human-verify confirms the section renders + visibly clips._
- [x] **VIEW-INSP-02** _(✓ UAT PASS 2026-05-21 — labels appear; surface-normal artifact traced to a pre-existing renderLabel_ blend-state leak, fixed in 8e14034b9d)_ (Phase 999.51 · v1.7.x-02): same shape for `LabelController::apply()` + a `LabelSection` in the Inspector — _IMPLEMENTATION-COMPLETE / AWAITING GUI HUMAN-VERIFY (Plan 999.51-02 commits 3d2102a5b2/8ad9fc6076/590d7d6b9e; VIEW build green BALL_UI_V2=ON; STUB removed; apply() ports LabelDialog::accept → MODEL_LABEL Representation + LabelModel + MainControl insert/update; legacy LabelDialog untouched). Checkbox flips to [x] after the blocking running-BALLView human-verify confirms the section renders + a label visibly appears._
- [x] **VIEW-INSP-03** _(✓ UAT PASS 2026-05-21 — Material transparency slider visibly works; closes GH #527)_ (Phase 999.52 · v1.7.x-30 · GH #527): a working transparency control in the Inspector `materialSection`, superseding the broken legacy `MaterialSettings` transparency slider — _IMPLEMENTATION-COMPLETE / AWAITING GUI HUMAN-VERIFY (Plan 999.52-01 commits ce4061a93d/7374d42ae8; libVIEW + BALLView.app green BALL_UI_V2=ON). MaterialController gained an int transparency (0-255) property driving `Representation::setTransparency()` + `rep_->update(false)` — the per-vertex alpha path the interactive GLRenderer honors — NOT the dead `Stage::Material.transparency` field (the #527 bug; grep-verified unwritten). MaterialSection shows a raw 0-255 Transparency slider sharing Representation state with the Model-section slider. Legacy MaterialSettings dialog untouched (Phase 999.53 deletes it). Checkbox flips to [x] after the blocking running-BALLView human-verify confirms the Material-tab slider visibly changes transparency, the Model-section slider reflects the same value, and reset works._
- [x] **VIEW-CLEAN-02** (Phase 999.57 · prerequisite for 999.53) — _✓ UAT PASS 2026-05-21_: `createModelProcessor`/`createColorProcessor` relocated out of the legacy `modelSettingsDialog`/`coloringSettingsDialog` into headless `ModelProcessorFactory`/`ColorProcessorFactory` (MODELS layer, no QWidget dep); `displayProperties.C:451/487` + `modelController`/`coloringController` repointed onto the factories (grep-proven no legacy reach-throughs); CONFIG-01 compiled-default invariant preserved; render UAT confirmed Model + Coloring representations render correctly. _(scene.C's MaterialSettings/LightSettings/Stage + the remaining displayProperties consumers moved to VIEW-CLEAN-03 / Phase 999.58.)_
- [ ] **VIEW-CLEAN-03** (Phase 999.58 · 2nd prerequisite for 999.53): `scene.C` no longer constructs `MaterialSettings`/`LightSettings`/`StageSettings` (lines 141/142/161) — their Preferences-stack + `applyPreferences` + **stereo render-path** (scene.C:2664-3165) responsibilities relocated into Inspector Stage/Light/Material coverage; the Material/Light/Stage Inspector sections drive lights/materials/stereo end-to-end (GUI-verified); tri-OS green
- [ ] **VIEW-CLEAN-01** (Phase 999.53 · v1.7.x-08, depends on VIEW-CLEAN-02 + VIEW-CLEAN-03): the 9 superseded legacy VIEW dialog files (commit `7f56f80d89` breadcrumb) + `displayProperties.{C,h,ui,.sip}` + `lightSettings.sip` + the `Tools › Legacy Settings` submenu / `LegacySettingsHelper` deleted; `grep -r` for all 9 names in `source/`+`include/` returns 0; tri-OS CI green
- [x] **VIEW-UX-01** (Phase 999.54 · v1.7.x-31 · GH #501): BALLView emits a `Log`/status-bar warning at startup when zero renderer plugins are discovered
- [~] **BUILD-ACCEL-08** (Phase 999.55 · v1.7.x-09 PCH half): **ATTEMPTED + REVERTED.** Extending PCH to the test build via `REUSE_FROM BALL`/`VIEW` forces test TUs to inherit the SHARED library's compile-definitions (`NDEBUG` etc.), which flipped `Debug_test`'s debug-macro behavior under GCC → Linux runtime failure (CI run 26233705613; macOS never caught it — AppleClang has PCH guarded off). Reverted in `test/CMakeLists.txt`. The **library** PCH (BUILD-ACCEL-01 / 999.16: Windows −35.6% cold, Linux −15%) already delivers the build-time win; test-suite PCH is dropped as not worth the cross-platform fragility. No further action.
- [ ] **DOCS-01** (Phase 999.56 · v1.7.x-32 · GH #560): the PDF tutorial refreshed against v1.7.x — build system (Homebrew/vcpkg), screenshots, URLs, developer list, and the GitHub Releases download path

## v1.7.4 requirements

Added 2026-05-22 when the v1.7.4 design handover ("Inspector depth + the contract", 6 phases in 2 waves) was ingested. Each REQ maps to one phase. Phase IDs were renumbered **+1** from the handover (`999.58–999.63` → `999.59–999.64`) to clear the collision with the in-flight `999.58-scene-settings-stereo-cutover`. The two new patch IDs the handover minted (`v1.7.x-30/-31`) were re-minted to `v1.7.x-35/-36` because -30/-31 are already used in this repo (#527 / #501). Derived from each task doc's acceptance criteria. v1.7.4 sequences AFTER v1.7.2 + v1.7.3. None complete. See [`ROADMAP.md`](ROADMAP.md) "v1.7.4 milestone" and [`v1.7.4-ARCHITECTURE-CONTRACT.md`](v1.7.4-ARCHITECTURE-CONTRACT.md).

- [ ] **ARCH-CONTRACT-01** (Phase 999.59 · v1.7.x-24 · Wave A · GATING): every mutating Controller in `source/VIEW/KERNEL/controllers/` carries a command-shaped `apply()` conforming to `v1.7.4-ARCHITECTURE-CONTRACT.md` §2 (validate → `ApplyGuard` re-entrancy guard → capture → mutate the single owner → emit ONE typed event → soft render-invalidate → record reversible `ApplyPayload`); the 5 read-only-mirror Controllers (Camera/Light/Stereo + Clipping/Label tails) become mutating; `Stage`/`Representation` setters are `friend`-scoped to their matching Controller so direct external mutation does not compile
- [ ] **ARCH-CONTRACT-02** (Phase 999.60 · v1.7.x-25 · Wave A): contract tests enforce the apply() contract by behavior — a `ContractTestHarness` runs each operation through Inspector/toolbar/menu/Python and asserts bit-equal owner postconditions; the four-way selection-consumer agreement test (tree/scene/control/action) passes; re-entrancy is dropped not stacked; owner-narrowing compile-fail tests assert the right diagnostic; a blocking Linux `contract-tests` CI job runs ≥25 tests green
- [ ] **TEST-CONTRACT-01** (Phase 999.60 · v1.7.x-25 · Wave A): each of the 8 still-mutating Controllers has a per-Controller parity test file under `test/contract/`; `ctest -L contract` is wired and blocking in CI; PyBALL-surface tests `GTEST_SKIP()` cleanly when `BALL_HAS_PYBALL=OFF`
- [ ] **ARCH-CONTRACT-03** (Phase 999.61 · v1.7.x-26 · Wave A): the 6 UFG-30/32/18/26-flagged lifetime sites are migrated to `QPointer<T>` + parent-rooted ownership; cross-thread signal/slot uses `Qt::QueuedConnection` explicitly; a blocking `lifetime-discipline` CI lint rejects `deleteLater()` inside QObject destructors and hits zero current sites
- [ ] **LIFETIME-DOC-01** (Phase 999.61 · v1.7.x-26 · Wave A): `.planning/codebase/QT-LIFETIME.md` exists and is canonical — 5 rules + 4 case studies + cookbook
- [ ] **ARCH-CONTRACT-04** (Phase 999.62 · v1.7.x-35 new · Wave A): any container hosting an animated child (a `QPropertyAnimation` target) carries `WA_OpaquePaintEvent` + `setAutoFillBackground(true)` within 3 ancestor hops (factored into the `InspectorSection` base); a centralized opaque-container rule lives in `theme-neutral.qss` as a token; a blocking `opaque-paint-lint` CI job enforces it (STATUS → BLOCKING after a clean week)
- [ ] **OPAQUE-PAINT-01** (Phase 999.62 · v1.7.x-35 new · Wave A): `data/BALLView/theme/THEME.md` design-system reference exists, documents the opaque-paint rule + the dynamic-property toggle + the dark-mode (v1.8) relationship, and back-references `v1.7.4-ARCHITECTURE-CONTRACT.md` §10; the existing UFG-05/09/10 fixes still hold cross-platform
- [ ] **INSP-COLORING-VALUE-RANGE-01** (Phase 999.63 · v1.7.x-17b · Wave B): a `ValueRangeWidget` shows a histogram of the active selection's scalar value (B-factor/Occupancy/Charge/Distance/Custom) for value-based coloring, with two draggable handles (live preview on drag, commit on release through `ColoringController::setRange(min,max)` → the command-shaped `apply()`) and four presets (Auto-fit / Full range / Robust 5–95% / Reset); `ColoringController::distribution()` exposes a cached bin-summary read-model so the widget never walks the Composite tree; 50k-atom selection re-bin <50 ms
- [ ] **INSP-RESET-01** (Phase 999.64 · v1.7.x-36 new · Wave B): every `InspectorSection` header carries a Reset glyph on hover; clicking invokes the matching `Controller::reset()` which captures-current → invokes method-defined defaults → emits exactly ONE event via the command-shape contract (one `ApplyPayload`, undoable in v2.0); all 14 sections implement `reset()` (read-only sections as a documented no-op)

---
*Requirements defined: 2026-05-14*
*Last updated: 2026-05-14 — Codex structural changes applied: Phase 02.2 (CI) + Phase 05.1 (backend spike) inserted; Phase 5 split (Qt6-only); Phase 6 restructured (decision+slice); Phase 8 scope clarified; DEPS-05/FEAT-01/DIAG-01/SPIKE/PY-02/PKG-03 added; feature matrix added.*
*Updated 2026-05-14 (consolidation): CONFIG-01 added → Phase 4.1 (promoted from backlog 999.4); SPIKE-01/SPIKE-02 re-mapped from former Phase 05.1 into Phase 5 (folded in). Active count 37 → 38.*
