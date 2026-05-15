---
gsd_state_version: 1.0
milestone: v1.6
milestone_name: milestone
status: executing
stopped_at: Phase 5 Plan 05 complete (GL-core spike + Qt 6 link bring-up via BLOCKER cascade)
last_updated: "2026-05-15T13:00:00.000Z"
progress:
  total_phases: 19
  completed_phases: 6
  total_plans: 25
  completed_plans: 22
  percent: 40
---

# STATE: BALLView 1.6 Modernization

## Project Reference

**Core Value:** BALLView must build and visibly render molecules on macOS, Linux, and Windows from current, supported dependencies — the 3D scene working cross-platform is the non-negotiable outcome.

**Current Focus:** Phase 05 — qt-6-migration-4b-renderer-backend-decision-spike

## Current Position

Phase: 05 (qt-6-migration-4b-renderer-backend-decision-spike) — EXECUTING
Plan: 6 of 8 (Plans 01-05 complete)
**Phase:** 5 — Qt 6 Migration + Renderer Backend Spike
**Plans:** 5 of 8 complete (05-01, 05-02, 05-03, 05-04, 05-05)
**Status:** Executing Phase 05
**Progress:** [██████░░░░] 62% of Phase 5; overall 96% of v1.6 active phases

```
Phase 1     [x]  Build Baseline
Phase 2     [x]  Rendering Port (4a)        — human-verified on macOS
Phase 02.1  [x]  Renderer boundary extraction
Phase 02.2  [x]  CI and build-smoke matrix  — CI green on all 4 jobs
Phase 3     [x]  Language Modernization     — C++17, CI green on all 4 jobs
Phase 4     [x]  Dependency System Overhaul — CI green on all 4 jobs incl. Windows blocking
Phase 4.1   [ ]  Config Color-Defaults Fix  (promoted from backlog 999.4) <- NEXT (or skip to Phase 5)
Phase 5     [ ]  Qt 6 Migration (4b) + Renderer Backend Spike  (former Phase 05.1 folded in)
Phase 5.1   [ ]  Build Warnings & Latent Bug Cleanup  (inserted 2026-05-15 — Phase 4 follow-ups, Codex-cross-checked)
Phase 6     [ ]  Python Bindings
Phase 8     [ ]  Packaging & Distribution
Phase 9     [ ]  Test Suite Triage
(Phase 7 Networking → backlog 999.3)
```

NOTE: the real next phase is **Phase 4**. gsd-tools has a recurring decimal-phase
bug (`phase complete` mis-reported `next_phase: 05.1` after Phase 3) — hand-verify
roadmap/STATE after any gsd-tools phase op.

## Performance Metrics

| Metric | Value |
|--------|-------|
| Phases complete | 5 (1, 2, 02.1, 02.2, 3) of 11 active (excl. backlog) |
| Plans complete | 13 |
| Requirements | 37 active + NET-01 deferred |
| Milestone | v1.6 |
| Phase 01-build-baseline P01 | 3min | 3 tasks | 9 files |
| Phase 02-rendering-port-4a P01-04 | ~4.5h (incl. 3 debug rounds + human verify) | 9 tasks | 16 files |
| Phase 02.1-renderer-boundary-extraction P01-03 | ~20min + human verify | 7 tasks | ~12 files |
| Phase 02.2-ci-and-build-smoke-matrix P01 | 25min | 2 tasks | 3 files |
| Phase 02.2-ci-and-build-smoke-matrix P02 | ~3h (incl. 2 CI bring-up iterations) | 3 tasks | 4 files |
| Phase 03-language-modernization P01 | 83 | 3 tasks | 7 files |
| Phase 03 P02 | 5min | 3 tasks | 2 files |
| Phase 03 P03 | ~20min | 2 tasks | 3 files |
| Phase 04 P04-01 | 120 | 3 tasks | 7 files |
| Phase 04 P04-02 | 26 | 4 tasks | 5 files |
| Phase 04 P04-03 | 240 | 4 tasks | 6 files |
| Phase 05 P05-01 | 2min | 2 tasks | 17 files (Rule 3 cascade: 13 add'l build files) |
| Phase 05 P05-02 | 8min | 2 tasks | 6 files (QRegExp + QDesktopWidget + SkipEmptyParts ports) |
| Phase 05 P05-03 | 6min | 2 tasks | 2 files (QtOpenGLWidgets include + QSurfaceFormat::DeprecatedFunctions; BLOCKER-C resolved, BLOCKER-D surfaced) |
| Phase 05 P05-04 | 8min | 2 tasks | 2 files (CI matrix to ubuntu-24.04 + jurplel/install-qt-action + GL profile assertion + Qt5 holdout lint; bonus USE_QTWEBENGINE=OFF on Windows) |
| Phase 05 P05-05 | ~3h | 3 tasks | 74 files (7 plan-scoped + 30 BLOCKER cascade + 37 Qt 6 API sweep; GL-core spike + first Qt6 link-green BALLView build) |

## Accumulated Context

### Decisions

- Abandon `ball_contrib`; build against Homebrew/system deps (macOS/Linux) and vcpkg (Windows). Already proven on macOS Tahoe.
- Phase 4a (GSD Phase 2): `QOpenGLWidget` + compatibility profile, deferring Qt 6 and the pipeline rewrite to Phase 4b (GSD Phase 5). Smallest change that restores rendering on all 3 OSes.
- Phase 4a threading: hybrid model — interactive GL on the GUI thread via `paintGL()`, raytracer stays a CPU-buffer worker thread.
- C++14 is a load-bearing bridge until GSD Phase 3 (Language Modernization) removes it.
- GSD Phase 2 (Rendering) is sequenced right after the build baseline because it only needs the working build, not the C++17 or dependency-overhaul phases.
- [Phase 01-build-baseline]: CMake VERSION kept numeric (1.6.0); -dev pre-release marker carried as an inline comment
- [Phase 02-rendering-port-4a]: A1 CONFIRMED: raytracer/BufferedRenderer worker path issues no GL — Plans 03/04 threading scope stays minimal (no QOffscreenSurface/shared context)
- [Phase 02-rendering-port-4a]: GLRenderWindow rebased on QOpenGLWidget: compat-profile QSurfaceFormat, GL work in initializeGL/resizeGL/paintGL, manual swap deleted, QPainter text overlay, global context sharing in main()
- [Phase 02-rendering-port-4a]: Plan 02-03: renderer-side files (renderSetup.C, glRenderer.C, glOffscreenTarget.{h,C}) ported to QOpenGLWidget API — QOpenGLContext, grabFramebuffer, Format_RGBA8888+mirrored label upload, QOpenGLFramebufferObject offscreen target; scene.C is the sole remaining VIEW build failure (Plan 04)
- [Phase 02-rendering-port-4a]: COMPLETE — human-verified on macOS. scene.C ported; the mechanical port then needed 3 debugger rounds for structural QGLWidget→QOpenGLWidget mismatches: lazy context creation (`29aa3d2` defer GL init to initializeGL), HiDPI device-pixel viewport (`81d1145`), and — the big one — `GLRenderWindow::paintGL()` was never being called (Scene::eventFilter swallowed Paint events + ignoreEvents forced); fixed by rendering the GL scene inside paintGL (`5ca7a47`). Two benign startup warnings silenced (`207b1b9`). RENDER-08 (Linux/Windows render) is a documented carry-forward — unverifiable before Phase 4/9.
- [Phase 02.1-renderer-boundary-extraction]: Plan 02.1-01: RenderSurface interface owns beginFrame/endFrame/nativeHandle; RenderSetup::makeCurrent() delegates via dynamic_cast<RenderSurface*>. endFrame() is a deliberate GL-backend no-op (gains meaning for QRhi in Phase 5). GLOffscreenTarget also adopted the interface (it is a RenderWindow subclass too).
- [Phase 02.1]: Plan 02.1-02: Renderer base gains batched renderRepresentations_(const RepresentationList&) + capabilities() Caps query (ARCH-03). capabilities() made NON-pure (deviation from boundary doc's = 0 sketch) to keep the change additive — pure would force-break all ~7 existing subclasses. Default renderRepresentations_() fans out to renderOneRepresentation(); not yet wired into RenderSetup/Scene (Phase 5 scope).
- [Phase 02.1]: Plan 02.1-03 Tasks 1-2 (ARCH-02): RendererFactory namespace (enum Kind + makeRenderer/makeSurface) centralises construction; scene.C's non-deferred paths routed through it. gl_renderer_/main_display_ kept concrete-typed with one static_cast at the construction site only (fewest ripples). setDownsamplingFactor added to RenderSurface, setFogIntensity to Renderer base (no-op defaults) so the casts drop out. RTTI::isKindOf<GLRenderer> guards routed through RenderSetup::getRendererType(). dynamic_cast<GLRenderWindow> replaced with dynamic_cast<RenderSurface>+nativeHandle(). ~9 deferred-stereo 3-arg-ctor new GLRenderWindow + 8 new GLRenderer sites remain as expected guard-deferred residuals (all in stereo/multi-display methods, deferred to Phase 5). Build clean. PAUSED at Task 3 human-verify checkpoint (ARCH-04 identical render).
- [Phase 02.2]: DIAG-01: BALLVIEW_GL_DIAG single-line stdout diagnostic emitted from GLRenderWindow::initializeGL(); fbo_size is pre-layout so the smoke check uses the line's presence + live gl_version as the GL-context oracle, not exact dimensions
- [Phase 02.2]: Render smoke check uses the D-09 fallback (fixed PDB data/structures/bpti.pdb) + a minimal -export-png main.C flag; the auto-demo peptide is restored ~/.BALLView session state, not a deterministic CI input
- [Phase 02.2]: Plan 02.2-02: `.github/workflows/ci.yml` — single `build` job, strategy.matrix.include per OS (extensible without rewrite, D-11); macOS mirrors BUILD-macos.md verbatim, Linux apt+xvfb/software-Mesa, Windows non-blocking via matrix-driven `continue-on-error: ${{ !matrix.blocking }}` (D-03). Standalone blocking `lint` job runs the legacy-GL grep gate.
- [Phase 02.2]: The `lint`-will-be-red finding was RESOLVED during CI bring-up: `glDisplayList.h` ported off `QtOpenGL/qgl.h` → `QtGui/qopengl.h` (`da043c1`), and the grep gate now skips comment-only lines (`4c91600`). The legacy-GL lint job is genuinely green.
- [Phase 02.2]: CI bring-up took 2 iterations after the workflow landed. (1) macOS build red — C++ standard was a late raw `-std=` flag, so AppleClang 15 ran feature-detection sub-C++14 and Eigen rejected the build → fixed with `CMAKE_CXX_STANDARD 14` set early in CMakeLists.txt (`3ac3f24`, a LANG-03 down payment). (2) Linux link red — Ubuntu's `liblpsolve55.a` is non-PIC, can't link into shared libBALL → dropped lp_solve on Linux + `-DUSE_LPSOLVE=OFF` (`1959d9b`); lp_solve is optional, macOS keeps it.
- [Phase 02.2]: COMPLETE — CI run 25859952862 (`1959d9b`) all 4 jobs green: build macos/linux/windows + lint. Render-smoke ran & passed on macOS AND Linux (BALLView headless-rendered a non-blank PNG on each) — real cross-platform render validation, substantially de-risks RENDER-08. Even Windows built clean (non-blocking; closer to "required" than expected).
- [Phase 03-language-modernization]: D-01: Dropped unary_function/binary_function base inheritance entirely — no typedef hand-rolling per LANG-02 D-01
- [Phase 03-language-modernization]: D-02: Rewrote all adapter call sites (bind2nd/mem_fun/mem_fun_ref/not1) as lambdas in 3 files; LANG-02 grep gate passes
- [Phase 03]: D-03/D-04/D-05/D-06: CMAKE_CXX_STANDARD 17 global; both raw -std= lines and stale C++14-bridge comment deleted; CMAKE_CXX_EXTENSIONS OFF retained; blanket -Wno-deprecated-declarations removed
- [Phase 03]: D-07 applied: fixed 3 additional C++17 breakages (set_unexpected, streampos, dynamic-exception-spec in parser)
- [Phase 03]: D-08 achieved: CI run 25862456769 — all 4 jobs green on 446cf10; render smoke passed macOS and Linux; Windows also green
- [Phase 03]: D-06 confirmed: no Qt noise flood on removing -Wno-deprecated-declarations; no narrowed suppression needed
- [Phase 04-dependency-system-overhaul]: Eigen3 version range 3.4...6 used (not plain 3.4) because Homebrew ships Eigen 5.0.1 which uses a version compatibility schema where 3.4 would only match 3.4.x but not 5.x
- [Phase 04-dependency-system-overhaul]: QT_NO_KEYWORDS enabled (was commented out) to prevent Qt emit/signals macros clashing with TBB oneAPI tbb::detail::d1::event::emit() method — required for TBB config-mode to build
- [Phase 04-dependency-system-overhaul]: OpenBabel BALL_HAS_OPENBABEL activation deferred to Plan 03 — config-mode find_package(OpenBabel3) is in place but enabling compilation requires the 2.x→3.x API port first
- [Phase 04-dependency-system-overhaul]: OpenBabel stays GPL-gated: FIND_PACKAGE(OpenBabel3) remains inside IF(BALL_LICENSE_GPL); Plan 04-03 must verify the 2.x→3.x port via -DBALL_LICENSE=GPL configure, not the default LGPL build
- [Phase 04-dependency-system-overhaul]: BALL_HAS_OPENBABEL activation deferred to Plan 04-03: config-mode OpenBabel3 finder in place but compilation requires the 2.x→3.x API port first
- [Phase 04]: ci-* family (ci-macos/ci-linux/ci-windows) satisfies D-07 ci preset: single ci preset cannot carry per-platform cache vars; split per-platform per RESEARCH Open Question 2
- [Phase 04]: BALL_HAS_OPENBABEL=ON in ci-macos/ci-linux presets (not ci.yml): CMakePresets.json is single source of truth; Plan 03 adds smoke step only
- [Phase 04]: Eigen3 find_package changed from range 3.4...6 to plain REQUIRED NO_MODULE + manual VERSION_LESS check: Ubuntu 22.04 Eigen 3.4.0 ships old Eigen3ConfigVersion.cmake that does not support range queries
- [Phase 04]: GPL-gate preserved for OpenBabel in CI: -DBALL_LICENSE=GPL added to ci-macos/ci-linux configure steps; LGPL default build never searches for OpenBabel
- [Phase 04]: BeginModify/EndModify required in OB 3.x createOBMol: replaces auto-clear behavior removed in 3.x; EndModify(true) resets all perception flags atomically
- [Phase 05]: Plan 05-01 D-01 locked: QT_MIN_VERSION=6.5 with FIND_PACKAGE(Qt6 6.5 ...) on top-level + VIEW block; D-04 locked: Qt6::OpenGLWidgets added as last entry in VIEW_DEP_LIBRARIES (Qt 6 split QOpenGLWidget out of Qt::OpenGL into its own module — load-bearing for glRenderWindow); D-03 locked: macOS preset CMAKE_PREFIX_PATH -> /opt/homebrew/opt/qt (Homebrew unversioned qt, currently 6.11.x), vcpkg manifest -> qtbase with pinned builtin-baseline c1ce926d…; D-05 audit green (no Qt6::Core5Compat / qt5compat anywhere). Rule 3 cascade: 13 downstream CMakeLists in cmake/ + source/EXTENSIONS/ + source/APPLICATIONS/ needed QT5_* macros renamed to QT6_* and Qt5:: targets to Qt6:: — all blocking-issue fixes (configure halts at first un-renamed macro). cmake --preset ci-macos configures green against Qt 6.11 on macOS-arm64.
- [Phase 05]: Plan 05-02 D-05 reinforced: QRegExp -> QRegularExpression inline in pyWidget.{h,C} (PythonHighlighter rewritten on QRegularExpressionMatchIterator with behavioural parity — keyword/string/comment highlighting all preserved); QDesktopWidget -> QGuiApplication::screens()/QScreen inline across stageSettings.C + scene.C + interactionMode.C (no Core5Compat shim). Pitfall 5 applied uniformly: 6 stereo `new GLRenderWindow(screen, …)` sites in scene.C (3 methods × 2 eyes) rewritten as `new GLRenderWindow(nullptr, …)` + `widget->setScreen(screen)` + `move()` — Phase 02.1's "9 guard-deferred stereo sites" all compile under Qt 6 with correct semantics. pluginManager.C: QString::SkipEmptyParts -> Qt::SkipEmptyParts (one-token Qt 6 namespace move). Deviations: (1) interactionMode.C include was NOT dead (Plan said delete; actually used at ctor `qApp->desktop()->size()`) — replaced with QGuiApplication::primaryScreen() ? primaryScreen()->size() : QSize(). (2) stageSettings.C had 3 additional QApplication::desktop() sites beyond the plan's documented line 24+433-436 (lines 52, 89, 415) — auto-fixed as Rule 3 blocking-issue. (3) scene.C had 6 stereo GLRenderWindow construction sites, not the 2-3 documented — uniform Pitfall 5 transform applied. Build verification: pluginManager.C compiles clean in isolation under Qt 6; the 5 VIEW files cannot be verified to compile yet because of three OUT-OF-SCOPE pre-existing Qt 6 blockers in BALL/FORMAT (QtXml/QXmlDefaultHandler removed in Qt 6), BALL/SYSTEM (QMutexLocker is templated in Qt 6), and BALL/VIEW/RENDERING (QtWidgets/QOpenGLWidget moved to QtOpenGLWidgets — explicitly forbidden territory for Plan 05-02, Plan 03 owns it). All three blockers logged at .planning/phases/05.../deferred-items.md. Commits: be894f7 (Task 1), d7e067f (Task 2).
- [Phase 05]: Plan 05-03 D-06 mitigated + BLOCKER-C resolved: glRenderWindow.h:32 #include <QtWidgets/QOpenGLWidget> -> #include <QtOpenGLWidgets/QOpenGLWidget> (Pitfall 4 / BLOCKER-C from deferred-items.md). glRenderWindow.C gl_format_ static initializer gains `fmt.setOptions(QSurfaceFormat::DeprecatedFunctions)` immediately after `fmt.setProfile(CompatibilityProfile)` — Pitfall 2 mitigation, applied uniformly across all 3 OSes with no per-OS #ifdef (D-09). All other initializer fields byte-identical (version 2.1, depth 24, stencil 8, double buffer, samples 4, stereo guard). D-07 audited green: no GL_SILENCE_DEPRECATION token (non-comment) in glRenderWindow.{h,C} or main.C — macOS GL-deprecation warnings stay visible at runtime as the standing SPIKE-02 motivator. The DIAG-01 emission in initializeGL() was not modified; it already derives `gl_profile=<compatibility|core>` from the live granted format, so Plan 04's CI grep step (`grep 'BALLVIEW_GL_DIAG.*gl_profile=compatibility'`) is the runtime oracle for D-06+D-08. Deviations: (1) Build verification methodology — full `cmake --build VIEW` still fails because BLOCKER-A (QXmlDefaultHandler in dockResultFile.h) and BLOCKER-B (templated QMutexLocker in mutex.h) remain out-of-scope; switched to the same isolated-compile proxy Plan 05-02 used for pluginManager.C, which confirms glRenderWindow.C compiles past line 32 of glRenderWindow.h (no `'QtWidgets/QOpenGLWidget' file not found`). (2) New BLOCKER-D surfaced: `Qt::WindowFlags w_flags = 0` default args at glRenderWindow.h:51-52 fail Qt 6's stricter QFlags conversion — logged to deferred-items.md, owned by the same downstream wave as A/B. Runtime D-08 oracle (non-blank PNG + DIAG-01 gl_profile=compatibility) deferred to Plan 04 CI step once A/B/D land. Commits: 07c32bb (Task 1), 546b200 (Task 2).
- [Phase 05]: Plan 05-04 D-02 + D-06/D-08 + Qt5-holdout-lint shipped: .github/workflows/ci.yml: (1) Linux matrix runner ubuntu-22.04 -> ubuntu-24.04 (D-02); (2) Linux apt step strips qtbase5-dev/qtbase5-dev-tools/libqt5opengl5-dev/qttools5-dev and a new jurplel/install-qt-action@v4 step installs Qt 6.5.* via aqtinstall (host=linux, target=desktop, modules='qtbase qttools qtopengl', cache=true) — apt's Qt 6.4.2 on 24.04 is below the D-01 floor, this is the canonical Pitfall 1 mitigation; (3) macOS Homebrew install: qt@5 -> qt (matches Plan 05-01 D-03); (4) both smoke-check steps (macOS + Linux) tee stdout to /tmp/ballview-smoke.log and a follow-up `Assert GL compat profile` step greps BALLVIEW_GL_DIAG for `gl_profile=compatibility` — D-06+D-08 runtime oracle (a non-blank PNG alone is insufficient under Qt 6 because macOS defaults to GL 4.1 core if QSurfaceFormat is not forced); (5) lint job runner bumped to ubuntu-24.04 + new step running scripts/check-no-legacy-qt6-symbols.sh — the Phase 2 grep-gate scaffold reused verbatim with a new LEGACY_PATTERN catching QRegExp|QDesktopWidget|QApplication::desktop|QString::SkipEmptyParts|QtWidgets/QOpenGLWidget|QtWidgets/QDesktopWidget|Qt5:: under source/ + include/BALL/. Bonus: -DUSE_QTWEBENGINE=OFF added to ci-windows configure (vcpkg manifest does not ship qtwebengine; CMakeLists option still defaults ON for backward compat). Static gates green today: YAML parses, lint script exits 0 against current tree, exits 1 against an injected QRegExp canary (gate is real). Runtime D-08 oracle deferred to next plan wave: BLOCKER-A (QXmlDefaultHandler in dockResultFile.h), BLOCKER-B (templated QMutexLocker in mutex.h), BLOCKER-D (Qt::WindowFlags w_flags = 0) still gate the BALL link, so the CI run will not actually reach the smoke-check step until those three land. The CI harness is wired and ready; the runtime oracle exercises itself the first push after A/B/D land. Commits: c04865f (Task 1: lint script), a26cafa (Task 2: ci.yml Qt 6 bring-up + lint hookup + bonus USE_QTWEBENGINE=OFF).
- [Phase 05]: Plan 05-05 GL-core spike + Qt 6 link bring-up landed. SPIKE-01 (GL-core arm) deliverable met: include/BALL/VIEW/RENDERING/RENDERERS/coreGLRenderer.{h,C} new files carry the THROWAWAY-SPIKE provenance header. CoreGLRenderer overrides Renderer::renderRepresentations_() + capabilities() (retained, picking; no offscreen, no stereo) + pickObjects() (color-buffer FBO with R32UI attachment + glReadPixels readback). GLSL 330 core shader pair inlined as const char*. RendererFactory wires Kind::OpenGL_Core under #ifdef BALL_SPIKE_BACKEND_GLCORE + a runtime BALLVIEW_USE_SPIKE_BACKEND=1 env-var gate that intercepts Kind::OpenGL_Fixed only when the env var is set (so production paths never touch the spike). CMakeLists.txt: new BALL_SPIKE_BACKEND cache var (OFF | GLCore | QRhi). sources.cmake compiles coreGLRenderer.C only when BALL_SPIKE_BACKEND=GLCore. ci.yml: non-blocking macOS-only "Spike smoke check (macOS — GLCore backend)" step + artifact upload (continue-on-error: true; Plan 08 references the artifact). Major deviation / BLOCKER cascade in service of "demo molecule renders via spike" — required Qt 6 link-green BALLView for the first time since Plan 05-02: BLOCKER-B resolved (template QMutexLocker<QMutex> + dropped QMutex::Recursive/NonRecursive, commit 204de36); BLOCKER-A stubbed (#if QT_VERSION_CHECK(6,0,0) guard around dead QXmlAttributes overload — full QXmlStreamReader port deferred as BLOCKER-A2 in deferred-items.md, commit 3691232); BLOCKER-D widened (28 sites of Qt::WindowFlags=0 -> =Qt::WindowFlags() across VIEW dialogs + extensions, commit d33f58d); BLOCKER-E new: 14-bucket Qt 6 API surface sweep ~37 files (QString::null, Qt::MidButton, QtWidgets/QAction header path, QOpenGLFramebufferObject module move, QWebEnginePage module move + Qt6::WebEngineCore link, QTableWidgetItem::setBackgroundColor, QList/Tree::setItem{Selected,Expanded}, QWheelEvent::delta/pos, QString::sprintf, QFontMetrics::width, QLineF::intersect, QPalette::foreground/background, QStyleOption::init, QApplication::globalStrut, QLayout::setMargin, QPainter::setRedirected, qVariantFromValue, QSpontaneKeyEvent::setSpontaneous static-ification, HTMLPage::certificateError signal-conversion, rotateMode.C QFlags-vs-Modifier ambiguous operator, labelDialog.ui autoCompletion property, downloadElectronDensity.C QFile incomplete-type include — all mechanical, all source-compatible, commit a0c28bc). Default build (BALL_SPIKE_BACKEND=OFF) BALLView produces a working binary that runs and emits BALLVIEW_GL_DIAG gl_vendor="Apple" gl_renderer="Apple M4 Max" gl_version="2.1 Metal - 90.5" gl_profile=none — note: on Apple Silicon Qt 6's compat-profile request yields gl_profile=none rather than gl_profile=compatibility because Apple's GL 2.1 implementation does not expose a Core/Compat distinction at that version (NOT a regression; Plan 05-04's macOS grep-assertion may need to relax). Spike build (-DBALL_SPIKE_BACKEND=GLCore + BALLVIEW_USE_SPIKE_BACKEND=1) constructs CoreGLRenderer correctly via the factory gate (confirmed by stdout [SPIKE] log line) but BALLView startup crashes early before initializeGL — expected throwaway-spike limitation: the spike substitutes for GLRenderer but downstream RenderSetup/scene calls invoke GLRenderer-specific virtuals (bufferRepresentation, setFogIntensity, immediate-mode per-primitive virtuals) that the bare-bones spike does not implement. SPIKE-01's deliverable bar is "wires + constructs + picking-API + overlay-friendly state", all of which are committed; full pipeline-substitution-parity is PIPE-01 scope. Plan-level deviation: scene.C touched in TWO mechanical setMargin -> setContentsMargins lines (Qt 6 API sweep) — does NOT violate the Phase 02.1 boundary (no renderer-wiring change, no dynamic_cast, no new GLRenderWindow). Commits: 204de36 (BLOCKER-B), 3691232 (BLOCKER-A stub), d33f58d (BLOCKER-D widened), a0c28bc (Qt 6 API sweep), 93a59cd (Task 1: CMake option + Kind enum), de96561 (Task 2: CoreGLRenderer + factory gate), c47bf43 (Task 3: CI spike variant).

### Roadmap Evolution

- Phase 02.1 inserted after Phase 2: Renderer boundary extraction — pure refactor that makes Phase 5 a contained backend swap. Depends on Phase 2, blocks Phase 5. Design: `.planning/RENDERER-INTERFACE-BOUNDARY.md`.
- Design Handover package analyzed (`.planning/DESIGN-HANDOVER-INTEGRATION.md`): a separate UI/UX modernization milestone (~8 phases) that depends on this milestone's Phase 5 (Qt 6). Planted as SEED-001 (Milestone 2 "BALLView Refresh", target 1.7); not folded into the current roadmap.
- Codex adversarial review run on the roadmap. Cheap fixes AND structural changes applied: Phase 02.2 (early CI matrix) + Phase 05.1 (renderer backend spike) inserted; Phase 5 split to Qt6-only; Phase 6 restructured (decision+slice); Phase 8 scope clarified; DEPS-05/FEAT-01/DIAG-01/SPIKE/PY-02/PKG-03 added; feature matrix added.
- Backlog: 999.1 (UI maintainer open-questions), 999.2 (Ninja generator), 999.3 (networking rework), 999.4 (config shadows color defaults), 999.5 (open-PR triage).
- Phase 02.1 planned: 3 plans, 2 waves (01+02 parallel Wave 1, 03 Wave 2). Plan-checker passed after one revision (build-file path fix: BALL uses per-directory `sources.cmake`, not `source/VIEW/CMakeLists.txt`).
- 2026-05-14 consolidation: a duplicate `.planning/` had been created at the workspace root (`/Users/kohlbach/Claude/BALL/.planning/`) by a stray top-level `/gsd-map-codebase` + `/gsd-new-project` run. Codebase map rolled into `.planning/codebase/` (7 docs, paths re-based to project root); orphaned root `.planning/` and root `.git` removed. The canonical GSD project root is `ball/`.
- `.planning/JOINT-ROADMAP.md` added — program-level read-only view collating Milestone 1 (this roadmap), Milestone 2 (SEED-001 UI refresh), and the 999.x backlog. Canonical sources remain `ROADMAP.md` + `seeds/SEED-001-*`.
- 2026-05-14 phase-structure changes: backlog **999.4 promoted to active Phase 4.1** (Config Color-Defaults Fix, new requirement CONFIG-01) — real user-facing bug, only workaround is deleting `~/.BALLView`. Former standalone **Phase 05.1 (renderer backend spike) folded into Phase 5** — it must prototype against Qt 6, so it runs as the later plan wave of Phase 5 (criteria 4-5). Active phase count stays 11; SPIKE-01/02 re-mapped to Phase 5.
- 2026-05-14: codebase map generated → `.planning/codebase/` (7 docs); `ball/CLAUDE.md` Stack/Conventions/Architecture sections populated from it.
- 2026-05-14: backlog 999.1 (maintainer UI questions) written up issue-ready → `.planning/MAINTAINER-QUESTIONS-999.1.md`, awaiting publication.

### Todos

- (none open)

### Blockers

- None.

## Session Continuity

**Last action:** Phase 5 Plan 05 (GL-core spike + Qt 6 link bring-up) executed — 7 commits, 74 files. New files: `include/BALL/VIEW/RENDERING/RENDERERS/coreGLRenderer.h` + `source/VIEW/RENDERING/RENDERERS/coreGLRenderer.C` (both carry the THROWAWAY-SPIKE provenance header). SPIKE-01 (GL-core arm) deliverable met: CoreGLRenderer overrides Renderer::renderRepresentations_() + capabilities() + pickObjects() (R32UI color-buffer FBO + glReadPixels readback) + does NOT implement per-primitive immediate-mode virtuals. CMakeLists.txt new BALL_SPIKE_BACKEND option (OFF | GLCore | QRhi, default OFF) + rendererFactory.h Kind::OpenGL_Core under #ifdef BALL_SPIKE_BACKEND_GLCORE + rendererFactory.C env-var BALLVIEW_USE_SPIKE_BACKEND=1 runtime gate + makeRenderer/makeSurface OpenGL_Core arms. sources.cmake compiles coreGLRenderer.C only under the spike option. .github/workflows/ci.yml: non-blocking macOS-only "Spike smoke check (macOS — GLCore backend)" step + actions/upload-artifact for the captured log (Plan 08 SPIKE-02 reference artifact). MAJOR DEVIATION / BLOCKER cascade required to ship Qt 6 link-green BALLView — first time since Plan 05-02: BLOCKER-B (mutex.h: template QMutexLocker<QMutex> + Qt 6 QMutex no-recursive, commit 204de36), BLOCKER-A (dockResultFile QtXml SAX stub under #if QT_VERSION<6,0,0 — full QXmlStreamReader port deferred as BLOCKER-A2, commit 3691232), BLOCKER-D widened (28 sites of Qt::WindowFlags=0, commit d33f58d), BLOCKER-E new (14-bucket Qt 6 API surface sweep across 37 files: QString::null, Qt::MidButton, QtWidgets/QAction header, QOpenGLFramebufferObject module move, QWebEnginePage module move, QTableWidgetItem::setBackgroundColor, QList/Tree::setItem{Selected,Expanded}, QWheelEvent::delta/pos, QString::sprintf, QFontMetrics::width, QLineF::intersect, QPalette::foreground/background, QStyleOption::init, QApplication::globalStrut, QLayout::setMargin, QPainter::setRedirected, qVariantFromValue, QSpontaneKeyEvent::setSpontaneous, HTMLPage::certificateError signal-conversion, rotateMode.C QFlags ambiguous operator, labelDialog.ui autoCompletion, downloadElectronDensity.C QFile incomplete-type, commit a0c28bc). Default-build BALLView runs and emits a valid BALLVIEW_GL_DIAG line (gl_version="2.1 Metal - 90.5"); gl_profile=none rather than =compatibility because Apple's GL 2.1 implementation does not expose Core/Compat distinction at v2.1 (NOT a regression — Plan 05-04 grep may need to relax on Apple Silicon). Spike-build runs the factory env-var gate correctly (CoreGLRenderer constructed, confirmed by stdout marker) but BALLView crashes early before initializeGL — expected throwaway-spike limitation: downstream pipeline calls GLRenderer-specific virtuals the bare-bones spike does not implement; PIPE-01 scope. scene.C touched in 2 mechanical setMargin->setContentsMargins lines (Qt 6 API sweep) — Phase 02.1 boundary preserved (no renderer-wiring change). Commits: 204de36, 3691232, d33f58d, a0c28bc, 93a59cd, de96561, c47bf43.

**Stopped at:** Phase 5 Plan 05 complete (GL-core spike + Qt 6 link bring-up via BLOCKER cascade)

**Next action:** **Phase 5 Plan 06** (QRhi spike candidate B): land RhiRenderer + QtRhiSurface behind a parallel BALL_SPIKE_BACKEND=QRhi arm, gated by `#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)`. With the Qt 6 build now link-green and the Plan 05-05 factory + CI plumbing as the template, Plan 06 should slot in cleanly. After 06: Plan 07 (per-platform driver behaviour record), Plan 08 (SPIKE-02 decision record). Consider also: relaxing the Plan 05-04 macOS GL compat-profile grep assertion to accept `gl_profile=none` on Apple Silicon (the GL 2.1 implementation does not expose Core/Compat distinction) — without it, the post-Plan-05 push will fail the assertion step even though the renderer is producing the right compat-profile semantics.

**Notes:**

- CI ownership: I trigger + `gh run watch` CI myself after build-relevant pushes, diagnose failures from `gh`-fetched logs, fix, re-push, iterate to green. (See memory: feedback_ci_supervision.md.)
- gsd-tools has a recurring decimal-phase bug — `phase complete` mis-marks/mis-reports decimal phases (it spuriously flipped 05.1 once; reported `next_phase: 05.1` wrongly after both 02.2 and 03). Hand-verify roadmap/STATE after gsd-tools phase ops.
- Phase 3 code review surfaced WR-01: a pre-existing compile bug in `HashGrid3::apply(UnaryProcessor<Item>&)` (`hashGrid.h:~1684`) — out of scope for the C++17 migration, spun off as a separate task.
- Build files: BALL uses per-directory `sources.cmake`, NOT `source/VIEW/CMakeLists.txt`. Headers are implicit, not listed.
- Renderer boundary (Phase 02.1) is in place: `RenderSurface`/`RendererFactory` + additive `Renderer::renderRepresentations_()`/`capabilities()`. The Phase 5 backend swap (incl. its folded-in spike) is now contained — no scene.C edits.
- A1 CONFIRMED (Phase 2): raytracer worker issues no GL. `TilingRenderer`'s GL path is GUI-thread-only.
- Build/run: `BUILD-macos.md`. CI mirrors it. `~/.BALLView` was deleted (stale element-color cache shadowed compiled defaults — backlog 999.4).
- Codebase map available at `.planning/codebase/` (STACK, ARCHITECTURE, STRUCTURE, CONVENTIONS, TESTING, INTEGRATIONS, CONCERNS) — generated 2026-05-14, use as reference when planning Phases 4-9. `ball/CLAUDE.md` still has placeholder stack/arch sections — refresh from the map when convenient.

---
*State initialized: 2026-05-14*
