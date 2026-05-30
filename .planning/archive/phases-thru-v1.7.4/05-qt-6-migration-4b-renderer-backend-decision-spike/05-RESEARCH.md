# Phase 5: Qt 6 Migration (4b) + Renderer Backend Decision Spike — Research

**Researched:** 2026-05-15
**Domain:** Qt 5 → Qt 6 migration, QSurfaceFormat compat profile under Qt 6, renderer boundary spike (GL-core vs QRhi), per-platform GL driver behaviour
**Confidence:** HIGH (codebase facts direct-read; Qt 6 API facts cited from official docs via Context7; CI/package facts web-verified)

---

<user_constraints>
## User Constraints (from CONTEXT.md)

### Locked Decisions

- **D-01:** Qt 6.5 LTS minimum — `FIND_PACKAGE(Qt6 6.5 ...)` in CMake.
- **D-02:** Linux CI runner `ubuntu-22.04` → `ubuntu-24.04`.
- **D-03:** macOS = Homebrew `qt`; Windows = vcpkg `qt6-base` (+ `qt6-tools` only if a soft find requires it).
- **D-04:** Component set is Qt 5 list with Qt 6 spellings PLUS NEW `Qt6::OpenGLWidgets`.
- **D-05:** No `Qt6::Core5Compat` — inline replacement of `QRegExp` → `QRegularExpression` and `QDesktopWidget` → `QGuiApplication::screens()` / `QScreen`.
- **D-06:** `QSurfaceFormat::CompatibilityProfile` uniform on all 3 OSes in `main.C` / `GLRenderWindow`.
- **D-07:** macOS GL-deprecation warnings left visible at runtime (no `GL_SILENCE_DEPRECATION`).
- **D-08:** Phase 02.2 render smoke check must pass on Qt 6 — non-blank PNG on macOS + Linux is a Phase 5 blocker.
- **D-09:** Same `QSurfaceFormat` on Windows — no per-OS divergence.

### Claude's Discretion

- Spike backend scope (QRhi vs GL-core vs both).
- Spike acceptance bar.
- Sequencing the inline `QRegExp` / `QDesktopWidget` replacement.
- Whether `Qt6::OpenGLWidgets` is found via the existing `FIND_PACKAGE` block or a separate find.

### Deferred Ideas (OUT OF SCOPE)

- PIPE-01 (full programmable-pipeline rewrite — v2).
- Phase 5.1 build-warnings/latent-bugs cleanup.
- Per-OS `QSurfaceFormat` tuning.
- Audit-and-minimize Qt component set.
</user_constraints>

<phase_requirements>
## Phase Requirements

| ID | Description | Research Support |
|----|-------------|------------------|
| QT6-01 | BALLView builds against Qt 6 with `QGLWidget`-era APIs fully removed | §Standard Stack, §Qt 5→6 API Delta, §CMake Migration |
| QT6-02 | Qt-deprecated VIEW APIs (`QRegExp`, `QDesktopWidget`) replaced with Qt 6 equivalents | §Qt 5→6 API Delta — exact files + line numbers catalogued |
| SPIKE-01 | Throwaway prototype renders demo molecule through at least one backend candidate (GL-core and/or QRhi) behind `RendererFactory`, demonstrating picking + text overlay | §Renderer Boundary Plug-In Pattern, §Backend Tradeoffs |
| SPIKE-02 | Decision record names chosen backend, rationale, per-platform criteria, scoped PIPE-01 task list | §SPIKE-02 Decision Record Template, §Per-Platform Driver Checks |
</phase_requirements>

---

## Summary

Phase 5 has two sequenced halves. The **migration half** (QT6-01, QT6-02) replaces Qt5-specific names in CMake and source — the API surface change is mechanically bounded to about a dozen files. The **spike half** (SPIKE-01, SPIKE-02) adds 1-2 new files implementing a throwaway backend prototype behind the Phase 02.1 `RendererFactory` boundary, then writes a decision record. The spike is **strictly throwaway** — not wired into the GUI, not shipped as default, not production-tested.

The migration is lower risk than Phase 2 because the `QOpenGLWidget` port is already done. The blast radius is CMakeLists.txt, a handful of VIEW source files with `QRegExp` / `QDesktopWidget`, and the CI workflow. The one non-trivial risk is the `QSurfaceFormat::CompatibilityProfile` enforcement under Qt 6's changed defaults: Qt 6 on macOS will default to a GL 4.1 core context if no format is forced, silently breaking all of `glRenderer.C`'s fixed-function calls. The existing `GLRenderWindow::gl_format_` static + `QSurfaceFormat::setDefaultFormat()` call in `main.C` already set the correct format — Phase 5 must verify this mechanism still fires correctly under Qt 6's changed context-creation order.

A **critical CI constraint**: Ubuntu 24.04 apt ships Qt 6.4.2, which is below the D-01 floor of 6.5. The Linux CI step cannot satisfy D-01 via `apt-get` alone and will need either `jurplel/install-qt-action` (aqtinstall) or a PPA. This is the highest-confidence blocker discovered in this research.

**Primary recommendation:** Migrate CMake first (D-01 / D-04), then do the inline API replacements (D-05), then verify the compat-profile format still holds under Qt 6, then smoke-check (D-08). Spike comes last against the working Qt 6 build. For the spike, prototype **both** GL-core and QRhi behind separate `Kind` values so SPIKE-02 has real comparison data.

---

## Architectural Responsibility Map

| Capability | Primary Tier | Secondary Tier | Rationale |
|------------|-------------|----------------|-----------|
| Qt 6 CMake FIND_PACKAGE / component wiring | Build system | — | CMakeLists.txt is the single configure-time authority |
| QSurfaceFormat compat-profile enforcement | Frontend Server (Qt widget layer) | — | Must be set before QApplication constructs; `main.C` + `GLRenderWindow::gl_format_` |
| `QRegExp` → `QRegularExpression` replacement | VIEW source files | — | Pure compile-time API swap, no architectural change |
| `QDesktopWidget` → `QScreen` replacement | VIEW source files | — | 3 call-sites; stereo/multi-display code |
| Spike `Kind` enum extension | `RendererFactory` (BUILD boundary) | — | One-line addition to `rendererFactory.h` |
| Spike renderer prototype | New file `coreGLRenderer.C` or `rhiRenderer.C` | — | Implements `Renderer::renderRepresentations_()` + `capabilities()` |
| Spike surface prototype (QRhi only) | New file `qtRhiSurface.C` | — | Implements `RenderSurface::beginFrame/endFrame/nativeHandle` |
| SPIKE-02 decision record | `.planning/` document | — | No production code — a planning artefact |
| CI runner bump ubuntu-22.04 → ubuntu-24.04 | `.github/workflows/ci.yml` | CMakePresets.json | Matrix field + apt package list change |
| Render smoke check (D-08) | CI job | Phase 02.2 script | Reuses existing `render-smoke-check.sh` |

---

## Standard Stack

### Core: Qt 6 Module Renames

All Qt 5 modules have direct Qt 6 equivalents with changed CMake target names. The only addition is `Qt6::OpenGLWidgets` (new in Qt 6 — QOpenGLWidget was split out of `Qt::OpenGL`).

| Qt 5 CMake target | Qt 6 CMake target | Notes |
|-------------------|-------------------|-------|
| `Qt5::Core` | `Qt6::Core` | `QRegExp` removed; use `QRegularExpression` |
| `Qt5::Network` | `Qt6::Network` | No API changes affecting this codebase |
| `Qt5::Xml` | `Qt6::Xml` | No API changes affecting this codebase |
| `Qt5::OpenGL` | `Qt6::OpenGL` | OpenGL helper classes (QOpenGLContext, QOpenGLFunctions, etc.) |
| `Qt5::Widgets` | `Qt6::Widgets` | `QDesktopWidget` / `QApplication::desktop()` removed |
| `Qt5::PrintSupport` | `Qt6::PrintSupport` | No relevant changes |
| `Qt5::Test` | `Qt6::Test` | No relevant changes |
| `Qt5LinguistTools` | `Qt6LinguistTools` | Optional, unchanged pattern |
| *(not present)* | `Qt6::OpenGLWidgets` | **NEW — required.** `QOpenGLWidget` lives here in Qt 6. Without this, `glRenderWindow.h` fails to find the class. `[CITED: doc.qt.io/qt-6/opengl-changes-qt6.html]` |

### Qt 6 Package Sources (per D-03)

| Platform | Source | Expected version | Verified |
|----------|--------|-----------------|----------|
| macOS | `brew install qt` | 6.9.x (satisfies 6.5 floor) | `[ASSUMED]` — Homebrew keeps `qt` at current; was 6.9.x at research time |
| Windows | vcpkg `qtbase` | 6.10.3 (latest at research time) | `[VERIFIED: vcpkg.io/en/package/qtbase.html]` |
| Linux CI | **NOT via apt** — see critical note below | 6.4.2 via apt (too old) | `[VERIFIED: packages.ubuntu.com/noble/qt6-base-dev]` |

**CRITICAL: Ubuntu 24.04 noble apt ships Qt 6.4.2 — below the D-01 minimum of 6.5.** `[VERIFIED: packages.ubuntu.com/noble/qt6-base-dev]`

The D-02 runner bump to `ubuntu-24.04` alone does NOT satisfy D-01. The CI workflow needs one of:
1. `jurplel/install-qt-action` (wraps aqtinstall — installs any Qt version from download.qt.io into a CI workspace) — most reliable, widely used. `[CITED: github.com/jurplel/install-qt-action]`
2. PPA `ppa:okirby/qt6-backports` — available, but adds an external PPA dependency to CI. `[VERIFIED: launchpad.net — ppa:okirby/qt6-backports exists]`
3. Build Qt 6.5+ from source — impractical for CI.

**Recommendation: use `jurplel/install-qt-action` on the Linux CI matrix entry.** This is the standard practice for Qt CI when the distro-apt version is insufficient. It preserves the ubuntu-24.04 runner (for the newer GCC D-02 needs) while satisfying D-01.

### vcpkg Package Name Change (Windows)

The current `vcpkg.json` declares `qt5-base`. In Qt 6, vcpkg's port is named **`qtbase`** (not `qt6-base`). `[VERIFIED: vcpkg.io/en/package/qtbase.html]` The Windows manifest must change `"qt5-base"` → `"qtbase"`. All Boost/Eigen/GLEW/TBB/libsvm ports keep their current names unchanged.

---

## Qt 5 → 6 API Surface Delta

Every concrete file that needs editing, what changes, and confidence level.

### CMakeLists.txt (lines 291–311)

| Current | Replace with | Notes |
|---------|-------------|-------|
| `SET(QT_MIN_VERSION 5.15)` | `SET(QT_MIN_VERSION 6.5)` | D-01 |
| `FIND_PACKAGE(Qt5 ${QT_MIN_VERSION} REQUIRED Core Network Xml)` | `FIND_PACKAGE(Qt6 ${QT_MIN_VERSION} REQUIRED Core Network Xml)` | D-01 |
| `LIST(APPEND BALL_DEP_LIBRARIES Qt5::Core Qt5::Network Qt5::Xml)` | `LIST(APPEND BALL_DEP_LIBRARIES Qt6::Core Qt6::Network Qt6::Xml)` | D-04 |
| `FIND_PACKAGE(Qt5 ${QT_MIN_VERSION} REQUIRED OpenGL PrintSupport Test Widgets)` | `FIND_PACKAGE(Qt6 ${QT_MIN_VERSION} REQUIRED OpenGL PrintSupport Test Widgets OpenGLWidgets)` | D-04; add OpenGLWidgets here (same FIND_PACKAGE call is fine) |
| `LIST(APPEND VIEW_DEP_LIBRARIES Qt5::OpenGL Qt5::PrintSupport Qt5::Test Qt5::Widgets)` | Same with Qt6:: prefix PLUS `Qt6::OpenGLWidgets` | D-04 |
| `FIND_PACKAGE(Qt5 ... WebEngine ...)` block | Change Qt5 → Qt6 | Leave optional; WebEngine is already disabled |

Also: `CMAKE_PREFIX_PATH` in `CMakePresets.json` `macos-homebrew` preset currently hard-codes `/opt/homebrew/opt/qt@5;...` → change to `/opt/homebrew/opt/qt;...` (Homebrew's unversioned `qt` tap). `[VERIFIED: confirmed from glRenderWindow.C + ci.yml + CMakePresets.json direct read]`

### source/VIEW/WIDGETS/pyWidget.C + include/BALL/VIEW/WIDGETS/pyWidget.h

Full `QRegExp` → `QRegularExpression` port. `[VERIFIED: grep of codebase]`

| File | Line | Current | Replace with |
|------|------|---------|-------------|
| `pyWidget.h` | 67 | `vector<QRegExp> python_patterns;` | `vector<QRegularExpression> python_patterns;` |
| `pyWidget.h` | 68 | `vector<QRegExp> BALL_patterns;` | `vector<QRegularExpression> BALL_patterns;` |
| `pyWidget.h` | 69 | `QRegExp string_pattern;` | `QRegularExpression string_pattern;` |
| `pyWidget.h` | 70 | `QRegExp comment_pattern;` | `QRegularExpression comment_pattern;` |
| `pyWidget.C` | 56 | `QRegExp(delim + *lit + delim)` | `QRegularExpression(delim + *lit + delim)` |
| `pyWidget.C` | 63 | same pattern | same replacement |
| `pyWidget.C` | 66 | `QRegExp("\".*\"")` | `QRegularExpression("\".*\"")` |
| `pyWidget.C` | 67 | `QRegExp("#.*")` | `QRegularExpression("#.*")` |
| `pyWidget.C` | 76 | `const QRegExp& expression` | `const QRegularExpression& expression` |
| `pyWidget.C` | 88 | same | same |

**QRegExp → QRegularExpression API differences to handle:** `[CITED: doc.qt.io/qt-6/qregexp.html]`
- `QRegExp::indexIn(text, offset)` → `re.match(text, offset).capturedStart()` or `re.globalMatch(text)`
- `QRegExp::matchedLength()` → `match.capturedLength()`
- Pattern syntax is Perl-compatible in `QRegularExpression` (anchors, escaping are the same for simple patterns). The patterns in pyWidget (`"\".*\""`, `"#.*"`, word-delimited keyword patterns) are simple enough to port without regex-syntax changes.
- The `pyWidget.C:76-88` loop uses the expression object — verify it uses `.indexIn()` or `.exactMatch()` and port accordingly.

### source/VIEW/DIALOGS/stageSettings.C

`QDesktopWidget` → `QScreen`. `[VERIFIED: grep of codebase]`

| Line | Current | Replace with |
|------|---------|-------------|
| 24 | `#include <QtWidgets/QDesktopWidget>` | `#include <QtGui/QScreen>` + `#include <QtGui/QGuiApplication>` |
| 433 | `QDesktopWidget* desktop = QApplication::desktop();` | Remove (unused after line changes) |
| 435 | `QApplication::desktop()->screenGeometry(left_screen_index)` | `QGuiApplication::screens().value(left_screen_index)->geometry()` |
| 436 | `QApplication::desktop()->screenGeometry(right_screen_index)` | `QGuiApplication::screens().value(right_screen_index)->geometry()` |

### source/VIEW/WIDGETS/scene.C

`QDesktopWidget` → `QScreen`. `[VERIFIED: grep of codebase]`

| Line | Current | Replace with |
|------|---------|-------------|
| 84 | `#include <QtWidgets/QDesktopWidget>` | `#include <QtGui/QScreen>` + `#include <QtGui/QGuiApplication>` |
| 2581 | `QDesktopWidget* desktop = QApplication::desktop();` | Remove |
| 2583 | `desktop->screen(left_screen_index)` | `QGuiApplication::screens().value(left_screen_index)` returns `QScreen*`; `QScreen` does not subclass `QWidget` in Qt 6, so if the code passes the result to a QWidget-expecting API, the call site needs refactoring |
| 2584 | `desktop->screen(right_screen_index)` | same |

**Important:** `QDesktopWidget::screen(int)` returned a `QWidget*`. `QScreen` is not a `QWidget`. At `scene.C:2583-2584`, the returned pointers (`left_screen`, `right_screen`) are used as parent pointers to `new GLRenderWindow(left_screen, ...)` — but `GLRenderWindow` takes `QWidget*`, not `QScreen*`. In Qt 6, the correct idiom for multi-screen window placement is `QWidget::setScreen(QScreen*)` or constructing with `QWidget* parent = nullptr` and then calling `window->setScreen()` + `window->move(screen->geometry().topLeft())`. This call-site is inside the stereo/multi-display guard-deferred paths (`addGlWindow`, `enterStereo` — the 9 residual `new GLRenderWindow` sites from Phase 02.1 VERIFICATION.md). These paths are already guard-deferred and will need a careful fix but will not break the primary rendering path.

### source/VIEW/KERNEL/MODES/interactionMode.C

| Line | Current | Replace with |
|------|---------|-------------|
| 5 | `#include <QtWidgets/QDesktopWidget>` | Remove (confirmed unused beyond include) |

### source/PLUGIN/pluginManager.C

| Line | Current | Replace with |
|------|---------|-------------|
| 408 | `value.split(";", QString::SkipEmptyParts)` | `value.split(";", Qt::SkipEmptyParts)` — `QString::SkipEmptyParts` was moved to the `Qt::` namespace in Qt 6. `[CITED: doc.qt.io/qt-6/qtcore-changes-qt6.html]` |

**Note:** This file is in `source/PLUGIN/` not `source/VIEW/` — it's a core BALL file, not a VIEW-only change. Still within Phase 5 scope (QT6-01 requires a clean build).

### include/BALL/VIEW/RENDERING/glRenderWindow.h

The compat-profile format initialization at line 50 uses:

```cpp
#ifndef BALL_OS_LINUX
#  if !defined(BALL_OS_LINUX) || QT_VERSION < QT_VERSION_CHECK(5, 7, 0) || QT_VERSION > QT_VERSION_CHECK(5, 7, 1)
    fmt.setStereo(true);
#  endif
#endif
```

Under Qt 6 this `QT_VERSION` check is always true (6.x > 5.7.1). The stereo-buffer guard was a Qt 5.7.0/5.7.1-specific Linux bug. In Qt 6 the outer `#ifndef BALL_OS_LINUX` structure still correctly disables stereo on Linux/macOS, and the inner `QT_VERSION` guard becomes dead code but harmless. Leave as-is for this phase.

### glRenderWindow.C — QSurfaceFormat::DeprecatedFunctions (D-06 verification)

Current `gl_format_` at `glRenderWindow.C:37-56` sets:
```cpp
fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
fmt.setVersion(2, 1);
```

Under Qt 6, `QSurfaceFormat::CompatibilityProfile` is still valid and still forces the legacy fixed-function path. `[CITED: doc.qt.io/qt-6/qsurfaceformat.html]` On macOS, Apple's driver caps CompatibilityProfile at OpenGL 2.1 (not 4.1 core), which is exactly what `glRenderer.C` needs.

**One addition to consider (D-06):** In Qt 6, it is recommended to also set `QSurfaceFormat::DeprecatedFunctions` option for compat profile, to explicitly request that deprecated fixed-function calls remain available: `fmt.setOptions(QSurfaceFormat::DeprecatedFunctions)`. Without it, on some drivers a compat context may not expose all legacy entry points. `[CITED: doc.qt.io/qt-6/qsurfaceformat.html]` `[VERIFIED: wiki.qt.io/How_to_use_OpenGL_Core_Profile_with_Qt]`

The existing `main.C:68` call `QSurfaceFormat::setDefaultFormat(BALL::VIEW::GLRenderWindow::gl_format_)` sets the format **before** `QApplication` is constructed — this is the correct Qt 6 pattern. `[CITED: doc.qt.io/qt-6/qopenglwidget.html]`

### Header Include Changes (Qt 6 module reorganization)

`QOpenGLWidget` header: In Qt 6, `#include <QtOpenGLWidgets/QOpenGLWidget>` (was `<QtWidgets/QOpenGLWidget>`). `[CITED: doc.qt.io/qt-6/opengl-changes-qt6.html]`

Files to audit for this change:
- `include/BALL/VIEW/RENDERING/glRenderWindow.h` — includes `<QtWidgets/QOpenGLWidget>`
- Any file including it transitively

---

## Architecture Patterns

### System Architecture Diagram

```
main.C (sets QSurfaceFormat::setDefaultFormat before QApplication)
        |
        v
QApplication
        |
        v
MainFrame → Scene → RendererFactory::makeRenderer(Kind::OpenGL_Fixed)
                 ↘  RendererFactory::makeSurface(Kind::OpenGL_Fixed, parent)
                  ↓
         GLRenderWindow (QOpenGLWidget subclass)
                  |  beginFrame / endFrame (RenderSurface interface)
                  v
         GLRenderer::renderRepresentations_() 
                  |
                  v
         glRenderer.C fixed-function GL calls (compat profile context)
                  |
                  v
         Phase 02.2 smoke check: -export-png → Scene::exportPNG() → non-blank PNG

----- Spike adds below this line (throwaway, not wired into GUI default) -----

         RendererFactory::makeRenderer(Kind::OpenGL_Core)   [new Kind]
                  |
                  v
         CoreGLRenderer::renderRepresentations_()  [new file]
                  |  (uses GL 3.2+ core VBOs + shaders)
                  v
         color-buffer picking FBO + glReadPixels → pickObjects()
                  |
                  v
         QPainter text overlay or bitmap glyph atlas

         RendererFactory::makeRenderer(Kind::QRhi)   [new Kind, optional]
                  |
                  v
         RhiRenderer::renderRepresentations_()  [new file]
         RhiSurface (QRhiWidget subclass, RenderSurface impl)  [new file]
                  |
                  v
         QRhi command buffer → vertex buffer upload → draw call
                  |
                  v
         grabFramebuffer() for per-platform pixel comparison
```

### Recommended Project Structure for Spike

```
include/BALL/VIEW/RENDERING/
├── rendererFactory.h          (add Kind::OpenGL_Core, Kind::QRhi)
├── renderSurface.h            (unchanged)
└── RENDERERS/
    ├── renderer.h             (unchanged)
    └── coreGLRenderer.h       (SPIKE-01 new file — throwaway)

source/VIEW/RENDERING/
├── rendererFactory.C          (add switch cases for new Kinds)
└── RENDERERS/
    ├── coreGLRenderer.C       (SPIKE-01 new file — throwaway)
    └── rhiRenderer.C          (SPIKE-01 new file — throwaway, if QRhi spike)

source/VIEW/RENDERING/
    └── qtRhiSurface.C         (SPIKE-01 new file — QRhiWidget subclass, if QRhi)
```

All new spike files should have a header comment: `// THROWAWAY SPIKE — Phase 5 prototype only. Not production code. Remove before PIPE-01.`

### Pattern: Adding a new RendererFactory::Kind

1. `rendererFactory.h` — add `OpenGL_Core` and/or `QRhi` to `enum class Kind`
2. `rendererFactory.C` — add `case Kind::OpenGL_Core: return new CoreGLRenderer();` in `makeRenderer`; add `case Kind::QRhi: return new RhiSurface(parent);` in `makeSurface`
3. New renderer file — implement `renderRepresentations_()` and `capabilities()`
4. Wire via a CMake option (`-DBALL_SPIKE_BACKEND=GLCore|QRhi|OFF`) — controlled at build time so the spike is never exposed in a release build

**Zero changes to `scene.C`, `RenderSetup`, or `GLRenderWindow`.** The factory boundary from Phase 02.1 ensures this. `[VERIFIED: 02.1-VERIFICATION.md — "scene.C has ZERO dynamic_cast<GLRenderWindow|GLRenderer>"]`

### Anti-Patterns to Avoid

- **Wiring the spike backend into the GUI toolbar/preferences** — that's PIPE-01 scope, not Phase 5.
- **Writing production unit tests for spike backends** — they are throwaway; a smoke assertion per backend in the spike test plan is sufficient.
- **Pulling in Phase 5.1 warning-cleanup** — Phase 5.1 is explicitly a separate phase; Qt 5 deprecation noise clears after Phase 5 lands. Do not add `-Wno-*` suppressions or fix unrelated warnings.
- **Setting `GL_SILENCE_DEPRECATION`** on macOS — D-07 explicitly forbids this.
- **Using `Qt5Compat` shim** — D-05 forbids it; `QRegExp` must be replaced inline.

---

## Don't Hand-Roll

| Problem | Don't Build | Use Instead | Why |
|---------|-------------|-------------|-----|
| Qt version installation in Linux CI | Custom apt PPA management or source build | `jurplel/install-qt-action` | Mature GH Action, wraps aqtinstall, supports exact Qt version pinning, cached |
| QRegExp replacement logic | Custom regex class | `QRegularExpression` | Already in Qt Core; Perl-compatible; no compat module needed |
| GL-core object picking | Custom ray-cast intersection math | Color-buffer picking via FBO + `glReadPixels` | Standard modern GL approach; GPU-side, no GL_SELECT legacy |
| QRhi shader management | Hand-rolled SPIR-V pipeline | `QRhiShaderResourceBindings` + `QShader` tooling | Qt toolchain handles cross-platform shader transpilation |
| Per-platform `QSurfaceFormat` logic | Platform #ifdef chains | Single `gl_format_` static (already in place) + `setDefaultFormat()` before QApplication | Qt 6 supports compat profile uniformly; D-09 says no divergence |

---

## Runtime State Inventory

Phase 5 is an API migration and spike, not a rename/rebrand. No runtime state inventory is required.

---

## Common Pitfalls

### Pitfall 1: Ubuntu 24.04 Qt6 Version Below D-01 Floor

**What goes wrong:** `apt-get install qt6-base-dev` on ubuntu-24.04 installs Qt 6.4.2. CMake `FIND_PACKAGE(Qt6 6.5 ...)` fails with "Could not find Qt6 version >= 6.5".
**Why it happens:** Ubuntu 24.04 (noble) packaged Qt 6.4.2 before Qt 6.5 LTS released, and Ubuntu's LTS policy does not backport major library versions into the main repo.
**How to avoid:** Add `jurplel/install-qt-action` to the `linux-x64` matrix entry in `ci.yml`. Set `version: '6.5.*'` or `'6.8.*'`. Remove the apt Qt packages from the Linux install step (keep xvfb/Mesa for headless rendering).
**Warning signs:** CI configure step fails with "Could NOT find Qt6 ... version 6.4.2 ... required 6.5.0".

### Pitfall 2: Qt 6 Defaults to Core Profile on macOS (D-06 blocker)

**What goes wrong:** If `QSurfaceFormat::setDefaultFormat()` is not called, or if the format is set but `setOptions(DeprecatedFunctions)` is absent, Qt 6 on macOS may hand out a GL 4.1 core context. `glRenderer.C`'s `glBegin`/`glEnd`/`glMatrixMode`/`gluLookAt` calls fail silently or produce a blank scene. Phase 02.2 smoke check fires: blank PNG = CI red.
**Why it happens:** Qt 6 changed its default surface format — it no longer defaults to a compat profile on macOS.
**How to avoid:** Verify `GLRenderWindow::gl_format_` still has `CompatibilityProfile` + optionally add `DeprecatedFunctions`. Verify `main.C:67-68` still runs the format+setAttribute pair before `QApplication`. Add a CI assertion: grep the `BALLVIEW_GL_DIAG` stdout line for `gl_profile=compatibility`.
**Warning signs:** Blank PNG from smoke check; `BALLVIEW_GL_DIAG gl_profile=core` in CI logs.

### Pitfall 3: vcpkg Package Name Change (qt5-base → qtbase)

**What goes wrong:** `vcpkg.json` still has `"qt5-base"` → vcpkg errors "no port named qt5-base" or silently installs nothing → Windows build fails to find Qt6.
**How to avoid:** Change `"qt5-base"` → `"qtbase"` in `vcpkg.json`. The `qtbase` port in vcpkg provides Qt6 Base (Core, Gui, Widgets, Network, OpenGL, etc.) including `Qt6::OpenGLWidgets`. `[VERIFIED: vcpkg.io/en/package/qtbase.html]`
**Warning signs:** `cmake --preset ci-windows` errors "find_package Qt6 could not find Qt6Config.cmake".

### Pitfall 4: QOpenGLWidget Header Path Changed in Qt 6

**What goes wrong:** `#include <QtWidgets/QOpenGLWidget>` fails to compile under Qt 6 because `QOpenGLWidget` moved to `QtOpenGLWidgets`. `[CITED: doc.qt.io/qt-6/opengl-changes-qt6.html]`
**How to avoid:** Change `#include <QtWidgets/QOpenGLWidget>` → `#include <QtOpenGLWidgets/QOpenGLWidget>` in `glRenderWindow.h` (and any file that included it directly).
**Warning signs:** `error: 'QOpenGLWidget' file not found` or `no such file or directory 'QtWidgets/QOpenGLWidget'`.

### Pitfall 5: `QDesktopWidget::screen(int)` Returns `QWidget*` but Qt 6's `QScreen` Is Not `QWidget`

**What goes wrong:** `scene.C:2583-2584` uses the `QDesktopWidget::screen()` result as a `QWidget*` parent for `new GLRenderWindow(left_screen, ...)`. Replacing `desktop->screen(i)` with `QGuiApplication::screens().value(i)` gives `QScreen*`, which does not downcast to `QWidget*`. Passing it to a `QWidget*` parameter is a compile error.
**Why it happens:** `QDesktopWidget::screen(int)` was a weird legacy API that returned a widget; Qt 6's screen model cleanly separates screens from widgets.
**How to avoid:** These are the 9 guard-deferred stereo sites from Phase 02.1. The fix is to pass `nullptr` as parent and call `window->setScreen(QGuiApplication::screens().value(i))` + `window->move(screen->geometry().topLeft())` after construction. This is inside already-deferred stereo paths; address them in Phase 5 but do not break the primary path.
**Warning signs:** `error: cannot convert 'QScreen*' to 'QWidget*'` in `scene.C`.

### Pitfall 6: CMakePresets.json macOS Qt Path Still Points to qt@5

**What goes wrong:** `CMAKE_PREFIX_PATH` in `macos-homebrew` preset is `/opt/homebrew/opt/qt@5;/opt/homebrew`. After switching to `brew install qt`, the Qt 6 CMake config files are at `/opt/homebrew/opt/qt/`. `find_package(Qt6)` silently fails to find Qt.
**How to avoid:** Update preset to `/opt/homebrew/opt/qt;/opt/homebrew`. The Homebrew formula for Qt 6 is `qt` (without `@6`).
**Warning signs:** macOS configure step "Could NOT find Qt6".

### Pitfall 7: QRhiWidget API Stability Caveat (SPIKE only)

**What goes wrong:** `QRhiWidget` was added in Qt 6.7 as a tech preview. The `QRhi` family has **limited source compatibility guarantees** — API may change across minor Qt releases (6.7, 6.8, 6.9). This is acceptable for a throwaway spike but must not be presented as a production API in SPIKE-02's decision record.
**Why it happens:** Qt's cross-platform RHI abstraction is still being stabilised.
**How to avoid:** In SPIKE-02 decision record, note the stability limitation. If QRhi is chosen for PIPE-01, note that PIPE-01 must target the then-current Qt 6.x version and may need minor API updates.
**Mitigation:** At the D-01 floor of Qt 6.5, `QRhiWidget` may not exist. Check: `QRhiWidget` was introduced in Qt 6.7. If Homebrew qt / vcpkg qtbase provide 6.7+, the spike can use it; otherwise the QRhi spike targets 6.7+ only.

### Pitfall 8: `QString::SkipEmptyParts` Moved to `Qt::` Namespace

**What goes wrong:** `pluginManager.C:408` uses `QString::SkipEmptyParts` which was removed from the `QString` class scope in Qt 6; it is now `Qt::SkipEmptyParts`.
**How to avoid:** Change to `Qt::SkipEmptyParts`. This is a one-token fix but will be a hard compile error under Qt 6. `[CITED: doc.qt.io/qt-6/qtcore-changes-qt6.html]`
**Warning signs:** `error: 'SkipEmptyParts' is not a member of 'QString'`.

### Pitfall 9: Spike Scope Creep into PIPE-01

**What goes wrong:** The prototype "just needs a GUI toggle" to be useful → quickly becomes the production backend → Phase 5 balloons.
**Why it happens:** Once a prototype renders correctly, the temptation to wire it into the real app is high.
**How to avoid:** The spike backend should only be activatable via a CMake flag (`-DBALL_SPIKE_BACKEND=GLCore`), never at runtime without recompilation. The SPIKE-02 decision record must state explicitly: "This prototype is not production code."

---

## Code Examples

### CMakeLists.txt: Qt 6 FIND_PACKAGE block (replaces lines 291–311)

```cmake
# Source: locked decision D-01 / D-04 + [CITED: doc.qt.io/qt-6/cmake-get-started.html]
SET(QT_MIN_VERSION 6.5)

FIND_PACKAGE(Qt6 ${QT_MIN_VERSION} REQUIRED Core Network Xml)

LIST(APPEND BALL_DEP_LIBRARIES Qt6::Core
                               Qt6::Network
                               Qt6::Xml)
LIST(APPEND BALL_PROJECT_COMPILE_DEFNS "-DQT_NO_KEYWORDS")

IF (BALL_HAS_VIEW)
  FIND_PACKAGE(Qt6 ${QT_MIN_VERSION} REQUIRED OpenGL PrintSupport Test Widgets OpenGLWidgets)
  FIND_PACKAGE(Qt6LinguistTools ${QT_MIN_VERSION})

  IF(NOT Qt6LinguistTools_FOUND)
    MESSAGE(WARNING "Qt6LinguistTools not found: translations cannot be created.")
  ENDIF()

  LIST(APPEND VIEW_DEP_LIBRARIES Qt6::OpenGL
                                 Qt6::PrintSupport
                                 Qt6::Test
                                 Qt6::Widgets
                                 Qt6::OpenGLWidgets)
  # ...
ENDIF()
```

### glRenderWindow.C: QSurfaceFormat for Qt 6 compat profile

```cpp
// Source: [CITED: doc.qt.io/qt-6/qsurfaceformat.html] + D-06
QSurfaceFormat GLRenderWindow::gl_format_ = [] {
  QSurfaceFormat fmt;
  fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
  fmt.setOptions(QSurfaceFormat::DeprecatedFunctions);  // ADD: explicit legacy entry points
  fmt.setVersion(2, 1);
  fmt.setDepthBufferSize(24);
  fmt.setStencilBufferSize(8);
  fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
  fmt.setSamples(4);
  // stereo guard stays as-is (dead QT_VERSION check is harmless under Qt 6)
  return fmt;
}();
```

### glRenderWindow.h: Updated QOpenGLWidget header

```cpp
// Source: [CITED: doc.qt.io/qt-6/opengl-changes-qt6.html]
// Qt 6: QOpenGLWidget moved from QtWidgets to QtOpenGLWidgets
#include <QtOpenGLWidgets/QOpenGLWidget>
```

### pyWidget.C: QRegularExpression replacement pattern

```cpp
// Source: [CITED: doc.qt.io/qt-6/qregexp.html migration guide]
#include <QtCore/QRegularExpression>
// ...
// Before:  python_patterns.push_back(QRegExp(delim + *lit + delim));
python_patterns.push_back(QRegularExpression(delim + *lit + delim));
// ...
// Before:  const QRegExp& expression = python_patterns[p];
//          int pos = expression.indexIn(text, 0);
//          while (pos >= 0) { highlight(pos, expression.matchedLength()); pos = expression.indexIn(text, pos + expression.matchedLength()); }
const QRegularExpression& expression = python_patterns[p];
QRegularExpressionMatchIterator it = expression.globalMatch(text);
while (it.hasNext()) {
  QRegularExpressionMatch m = it.next();
  highlight(m.capturedStart(), m.capturedLength());
}
```

### stageSettings.C / scene.C: QDesktopWidget → QScreen

```cpp
// Source: [CITED: doc.qt.io/qt-6/widgets-changes-qt6.html]
#include <QtGui/QScreen>
#include <QtGui/QGuiApplication>
// ...
// Before:  QDesktopWidget* desktop = QApplication::desktop();
//          QRect geom = QApplication::desktop()->screenGeometry(screen_index);
QList<QScreen*> screens = QGuiApplication::screens();
QRect geom = screens.value(screen_index) ? screens.value(screen_index)->geometry() : QRect();

// For the 'QWidget*' parent pattern in scene.C:2583 (guard-deferred stereo):
// Before:  QWidget* left_screen = desktop->screen(left_screen_index);
//          GLRenderWindow* w = new GLRenderWindow(left_screen, "left eye", Qt::FramelessWindowHint);
QScreen* screen = QGuiApplication::screens().value(left_screen_index);
GLRenderWindow* w = new GLRenderWindow(nullptr, "left eye", Qt::FramelessWindowHint);
if (screen) { w->setScreen(screen); w->move(screen->geometry().topLeft()); }
```

### RendererFactory: Adding Spike Backend Kinds

```cpp
// Source: [VERIFIED: rendererFactory.h direct read]
// rendererFactory.h — add to Kind enum:
enum class Kind
{
  OpenGL_Fixed,   ///< GLRenderer (fixed-function compat profile)
  Raytracer,      ///< CPU raytracer
  POV,
  STL,
  VRML,
  // SPIKE only — compile with -DBALL_SPIKE_BACKEND=GLCore or =QRhi:
  OpenGL_Core,    ///< THROWAWAY: GL 3.2+ core-profile renderer spike
  QRhi            ///< THROWAWAY: QRhiWidget-based renderer spike
};
```

### GL-Core Spike: Color-Buffer Picking Pattern

```cpp
// Source: [CITED: opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-an-opengl-hack/]
// Modern GL has no glRenderMode(GL_SELECT). Use a separate FBO with uint IDs encoded as colors.
// Render pass 1 (into picking FBO): each object draws with color = packUint(object_id)
// Render pass 2 (normal): regular shaded scene to screen
// On mouse click:
GLuint id = 0;
glBindFramebuffer(GL_READ_FRAMEBUFFER, picking_fbo_);
glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &id);
glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
// id → look up GeometricObject* in the object table
```

---

## GL-Core vs QRhi Spike Backend Tradeoffs

### Recommendation: Prototype BOTH

The SPIKE-02 decision record needs real comparative data. Prototyping only one backend produces a half-answer. Given the Phase 02.1 boundary, adding a second backend is ~1 additional file + a Kind value. The marginal cost is low; the decision quality is higher. The research recommendation is to prototype both.

### GL-Core Backend (Kind::OpenGL_Core)

**What it is:** A `Renderer` subclass that uses GL 3.2+ core profile: VAO/VBO geometry upload, GLSL shaders, no fixed-function calls. Inherits `renderRepresentations_()` from `Renderer` base.

**Pros for this codebase:**
- No new widget type — same `GLRenderWindow` (`QOpenGLWidget`) is the surface
- Can reuse the existing `QSurfaceFormat` machinery — just request `CoreProfile` instead of `CompatibilityProfile` for the spike context
- Picking is well-understood: color-buffer FBO + `glReadPixels` (`[CITED: opengl-tutorial.org]`) — no `glSelectBuffer`/`GL_SELECT`
- Text overlay: `QPainter` on `QOpenGLWidget` (already proven in Phase 2) or a bitmap glyph texture
- Available in Qt 6.5 — no version constraint beyond D-01

**Cons:**
- Must write VAO/VBO wrappers and a shader pair (vertex + fragment) for every geometry type in `glRenderer.C` — this is the PIPE-01 work in miniature
- macOS: Apple Silicon supports GL 4.1 core; the spike cap at 3.2 leaves headroom
- Picking FBO readback adds one extra render pass per click event

**macOS specifics:** Apple Silicon supports GL 4.1 core profile; compat profile caps at 2.1. The core-profile spike forces a context switch — the **same process cannot have both a compat and a core context** through `QOpenGLWidget` without setting up a second shared context. For the spike, the simplest approach is: build with `BALL_SPIKE_BACKEND=GLCore`, which substitutes the core renderer for the compat renderer. Do not try to run both simultaneously.

### QRhi Backend (Kind::QRhi)

**What it is:** A `Renderer` subclass + `RenderSurface` subclass that wraps `QRhiWidget`. Uses Qt's cross-API RHI layer (OpenGL ES2/Metal/Vulkan/D3D).

**Pros for this codebase:**
- Single code path across macOS (Metal), Windows (D3D11/12), Linux (Vulkan/OpenGL)
- `QRhiWidget` handles context creation, resize, and buffer management
- Eliminates macOS GL deprecation warnings entirely — Metal backend never triggers them
- `grabFramebuffer()` for pixel comparison is built in

**Cons:**
- `QRhiWidget` requires **Qt 6.7+** (introduced in 6.7 as tech preview). At D-01 floor 6.5, `QRhiWidget` is not available. `[VERIFIED: doc.qt.io/qt-6/whatsnew67.html]`
- The QRhi API has limited source/binary compatibility guarantees — no guarantee code written for 6.7 still compiles on 6.10 without changes. `[CITED: doc.qt.io/qt-6/qrhi.html]`
- Requires writing GLSL/HLSL/MSL shaders and using `QShader` compilation tools
- `RenderSurface::nativeHandle()` semantics differ — for QRhi the native handle is the `QRhi*` pointer, not a GL context
- More boilerplate: `QRhiBuffer`, `QRhiShaderResourceBindings`, `QRhiGraphicsPipeline` setup per geometry type

**Version constraint handling:** The QRhi spike should be wrapped in `#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)` guards and behind `BALL_SPIKE_BACKEND=QRhi` CMake option. If the available Qt is < 6.7, the QRhi spike is unavailable and the decision record falls back to the GL-core prototype only.

### Decision Record Inputs (What SPIKE-02 Must Capture)

| Criterion | GL-Core | QRhi |
|-----------|---------|------|
| macOS rendering | GL compat caps at 2.1; core at 4.1; no GL deprecation with core | Metal native; zero GL deprecation warnings |
| Windows rendering | OpenGL core (driver quality varies; Intel iGPU: GL 4.0+) | D3D11/12 native; most reliable driver path |
| Linux rendering | OpenGL core (Mesa: excellent) | Vulkan or OpenGL ES2 via Mesa |
| Picking implementation | Color-buffer FBO + glReadPixels | `QRhiReadbackResult` async readback |
| Text overlay | QPainter on QOpenGLWidget (existing code) | Custom shader or QPainter composite |
| PIPE-01 scope | Must write shaders only for GL; platform-specific quirks remain | Single abstraction; platform quirks handled by Qt |
| Qt version floor | 6.5 (D-01) | 6.7 (above D-01) |
| API stability | Stable | Limited stability guarantee |

---

## Per-Platform macOS / Windows Driver Behaviour Checks

These are the checks the spike must record for SPIKE-02.

### macOS (Apple Silicon)

| Check | How to Verify | What to Record |
|-------|--------------|---------------|
| GL vendor / renderer string | `BALLVIEW_GL_DIAG` stdout line already emits `gl_vendor` + `gl_renderer` | Expect `"Apple"` + `"Apple M*"` |
| GL version (compat profile) | `gl_version` in DIAG line | Expect `"2.1 ATI-..."` or `"2.1 Apple..."` |
| GL version (core profile) | Spike prints via `glGetString(GL_VERSION)` at init | Expect `"4.1 ..."` |
| GL deprecation warning | Present at runtime without `GL_SILENCE_DEPRECATION` (D-07) | Record exact warning text for SPIKE-02 "known liabilities" section |
| MSAA support (4× samples) | `fmt.samples()` in DIAG line | Should be `4` for compat; verify for core context too |
| Framebuffer format | `default_fbo` in DIAG line | Verify non-zero (FBO is the default FBO for QOpenGLWidget) |
| Metal availability for QRhi | `QRhi::Metal` backend enumerates, spike context creates | If QRhi spike: record whether Metal RHI initialises without error |

### Windows (Intel iGPU + NVIDIA targets for CI)

| Check | How to Verify | What to Record |
|-------|--------------|---------------|
| GL vendor / renderer string | Same DIAG line on Windows CI runner | GitHub windows-2022 runners have Intel iGPU (GL 4.0+) |
| Core profile support | `BALLVIEW_GL_DIAG gl_profile=core` in core-profile spike build | Intel HD: GL 4.0 core; record exact version |
| D3D11 availability for QRhi | QRhi backend enumeration | Record which D3D level is selected |
| ANGLE removal | In Qt 6, ANGLE is removed on Windows. `QT_OPENGL=angle` has no effect. `[CITED: doc.qt.io/qt-6/opengl-changes-qt6.html]` | Verify OpenGL32 (not ANGLE) is the Windows GL path |

### Linux (Mesa / software renderer)

| Check | How to Verify | What to Record |
|-------|--------------|---------------|
| Software Mesa compat profile | `LIBGL_ALWAYS_SOFTWARE=1` + xvfb, same as CI | `gl_vendor="Mesa/X.org"`, `gl_profile=compatibility` |
| Software Mesa core profile | Spike build with `LIBGL_ALWAYS_SOFTWARE=1` | Mesa supports GL 3.3 core in software (llvmpipe) |

---

## State of the Art

| Old Approach | Current Approach | When Changed | Impact |
|--------------|------------------|--------------|--------|
| `QGLWidget` fixed-function | `QOpenGLWidget` compat profile | Phase 2 (already done) | Complete |
| `Qt5::OpenGL` owns QOpenGLWidget | `Qt6::OpenGLWidgets` new module | Qt 6.0 | Need `Qt6::OpenGLWidgets` in CMake |
| `QRegExp` in Qt Core | `QRegularExpression` (PCRE2-based) | Qt 5 deprecated, Qt 6 removed | Inline replacement, no external dep |
| `QDesktopWidget` / `QApplication::desktop()` | `QScreen` / `QGuiApplication::screens()` | Qt 5 deprecated, Qt 6 removed | 3 call-site fixes |
| `QString::SkipEmptyParts` | `Qt::SkipEmptyParts` | Qt 6.0 | 1 call-site fix |
| `glRenderMode(GL_SELECT)` picking | Color-buffer FBO + `glReadPixels` picking | GL 3.0 core (deprecated); gone in core profile | Required for GL-core spike |
| ANGLE on Windows (Qt 5) | Direct OpenGL32 on Windows (Qt 6) | Qt 6.0 | No ANGLE DLL; OpenGL ICD must be present |
| `QRhiWidget` (tech preview) | `QRhiWidget` (production, Qt 6.8+) | Qt 6.7 preview → 6.8 stabilisation | Available if builder uses Qt 6.7+ |

---

## Environment Availability

| Dependency | Required By | Available (CI) | Version | Fallback |
|------------|------------|----------------|---------|----------|
| Qt 6.5+ (macOS Homebrew `qt`) | QT6-01, D-01, D-03 | ✓ (Homebrew `qt`) | ~6.9.x `[ASSUMED]` | None — required |
| Qt 6.5+ (Ubuntu 24.04 apt) | QT6-01, D-01, D-02 | ✗ apt gives 6.4.2 | 6.4.2 `[VERIFIED]` | `jurplel/install-qt-action` |
| Qt 6.5+ (Windows vcpkg `qtbase`) | QT6-01, D-01, D-03 | ✓ vcpkg | 6.10.3 `[VERIFIED]` | None — required |
| Qt 6.7+ (for QRhiWidget spike) | SPIKE-01 (QRhi path) | Depends on Qt version | Unknown without install | GL-core spike only |
| xvfb + software-Mesa (Linux) | D-08 smoke check | ✓ ubuntu-24.04 apt | Mesa 23.x `[ASSUMED]` | None |
| GitHub runner `ubuntu-24.04` | D-02 | ✓ `[VERIFIED: github.com/actions/runner-images/issues/9848]` | ubuntu 24.04 | None |

**Missing dependencies with no fallback:**
- Qt 6.5+ on Linux CI via apt: must use `jurplel/install-qt-action` or equivalent. This is a **plan-blocker** for the CI plan wave.

**Missing dependencies with fallback:**
- QRhi spike: requires Qt 6.7+. If Qt 6.7 is not available, fall back to GL-core spike only; SPIKE-02 can still be written with GL-core data and a note that QRhi was not prototyped due to version constraint.

---

## Validation Architecture

> `workflow.nyquist_validation` is not explicitly `false` in `.planning/config.json` — section is included.

### Test Framework

| Property | Value |
|----------|-------|
| Framework | Phase 02.2 render smoke check script (`render-smoke-check.sh`) + BALLVIEW_GL_DIAG stdout assertions |
| Config file | `.planning/phases/02.2-ci-and-build-smoke-matrix/scripts/render-smoke-check.sh` |
| Quick run command | `bash .../render-smoke-check.sh <path-to-BALLView>` |
| Full suite command | CI build matrix (`cmake --build ... --target BALL VIEW BALLView`) + smoke check |

There is no unit test framework (gtest etc.) wired for VIEW rendering. All Qt 6 validation is integration-level (build + smoke check) per Phase 02.2 precedent.

### Phase Requirements → Test Map

| Req ID | Behavior | Test Type | Automated Command | File Exists? |
|--------|----------|-----------|-------------------|-------------|
| QT6-01 | BALLView builds against Qt 6 on macOS/Linux/Windows | Build integration | `cmake --build --preset ci-{platform} --target BALL VIEW BALLView` | ✅ ci.yml |
| QT6-01 | No `QGLWidget`-era symbols remain | Static grep lint | `bash .../check-no-legacy-gl-symbols.sh $GITHUB_WORKSPACE` | ✅ ci.yml lint job |
| QT6-01 | GL context is compat profile (not accidentally core) | Diagnostic assertion | `grep 'BALLVIEW_GL_DIAG.*gl_profile=compatibility' ci-log` | ✅ (new assertion in CI — Wave 0 gap) |
| QT6-02 | `QRegExp` / `QDesktopWidget` compile-clean | Build integration | Build success (compile error otherwise) | ✅ via build |
| SPIKE-01 | Spike backend renders demo molecule (non-blank PNG) | Smoke integration | `BALL_SPIKE_BACKEND=GLCore bash .../render-smoke-check.sh <binary>` | ❌ Wave 0 gap — needs variant |
| SPIKE-01 | Spike picking returns non-empty object list | Manual verification | Human-run interactive session on macOS | Manual only |
| SPIKE-01 | Text overlay renders visible text | Manual verification | Human-run interactive session | Manual only |
| SPIKE-02 | Decision record exists and contains required sections | Document review | `ls .planning/phases/05-*/05-SPIKE-DECISION.md` | ❌ Wave 0 gap |
| D-08 | Non-blank PNG on macOS + Linux (regression vs Phase 2) | Smoke integration | Existing `render-smoke-check.sh` | ✅ ci.yml |

### Sampling Rate

- **Per task commit:** `cmake --build ... --target BALL VIEW BALLView` (compile correctness)
- **Per wave merge (migration wave):** Full CI run — build matrix + smoke check on macOS + Linux
- **Phase gate:** All 3 OS build jobs green + smoke check non-blank PNG on macOS + Linux before `/gsd-verify-work`

### Wave 0 Gaps

- [ ] `ci.yml`: Add GL profile assertion step — grep `BALLVIEW_GL_DIAG` stdout for `gl_profile=compatibility` after smoke check. This is the D-08 / D-06 correctness oracle.
- [ ] `ci.yml`: Add `jurplel/install-qt-action` step for `linux-x64` matrix entry replacing apt Qt packages.
- [ ] Spike smoke variant: a separate CI step (non-blocking, macOS only) that builds with `BALL_SPIKE_BACKEND=GLCore` and runs smoke check — proves the spike backend can render at all.
- [ ] `.planning/phases/05-.../05-SPIKE-DECISION.md` — created by the planner as the SPIKE-02 output artifact (template to fill during spike execution).

---

## Assumptions Log

| # | Claim | Section | Risk if Wrong |
|---|-------|---------|---------------|
| A1 | Homebrew `qt` (unversioned) provides Qt 6.9.x on Apple Silicon at plan time | Standard Stack, Environment | If Homebrew pins to a different version, D-01 CMake check still gates it; low risk |
| A2 | QRhiWidget stabilised in Qt 6.8 (no longer tech-preview) | Backend Tradeoffs | If still tech-preview, QRhi spike has even weaker ABI guarantees; impacts only the spike, not migration |
| A3 | macOS Apple Silicon `gl_vendor` string in DIAG line will contain "Apple" | Per-Platform Checks | Cosmetic only |
| A4 | `jurplel/install-qt-action` supports Qt 6.5 installation on ubuntu-24.04 | Environment Availability | Well-documented in action README; low risk |

---

## Open Questions (RESOLVED)

1. **Qt version on ubuntu-24.04 via `jurplel/install-qt-action`: which minor to pin?**
   - What we know: Qt 6.5 is the minimum (D-01); Qt 6.8 is the current LTS.
   - What's unclear: Whether to pin `6.5.*` (closest to floor) or `6.8.*` (current LTS, longer support) for Linux CI.
   - Recommendation: Pin `6.5.3` (last Qt 6.5 LTS patch as of research date) to stay at the floor; this maximises compatibility with the macOS Homebrew `qt` version the user actually builds against. If Homebrew is on 6.9, pin Linux to `6.8.*` instead to reduce the gap.
   - **Resolution:** Pin Linux CI to Qt 6.5.3 via `jurplel/install-qt-action` (floor-matching policy). If `brew info qt` reports >= 6.8 at execution time, bump Linux pin to 6.8.x to reduce cross-platform drift.

2. **QRhi spike: Qt 6.7 vs 6.8 minimum?**
   - What we know: `QRhiWidget` added in Qt 6.7 as tech preview; Qt 6.8 is an LTS.
   - What's unclear: Whether the macOS Homebrew `qt` bottle at plan time is 6.7+ (enabling QRhi spike) or needs a version pin.
   - Recommendation: The planner should verify `brew info qt` at plan time. If it's >= 6.7, prototype both GL-core and QRhi. If not, GL-core only.
   - **Resolution:** Plan 06 (QRhi spike) is gated by `#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)`. Homebrew `qt` at plan-execution time provides Qt >= 6.7, so the QRhi arm is feasible; if a CI host reports < 6.7, the QRhi spike Kind is omitted at CMake configure time with a documented note.

3. **Stereo/multi-display `QDesktopWidget::screen()` → `QScreen*` type mismatch: stub or fix?**
   - What we know: These are the 9 guard-deferred stereo sites from Phase 02.1; they already don't execute in the normal path.
   - What's unclear: Whether to fully fix them in Phase 5 (adds scope) or leave a compile-time `#if QT_VERSION >= 0x060000 TODO` stub.
   - Recommendation: Fix them correctly (per the code example above) since they are already in the files Phase 5 touches; the fix is ~5 lines per site. Do not defer further.
   - **Resolution:** Fix the 9 stereo `QDesktopWidget::screen(int)` sites correctly per the code example in §Stereo / Multi-Display QDesktopWidget Sites — handled inside Plan 02. No `#if QT_VERSION` stubs.

---

## Sources

### Primary (HIGH confidence)
- `[CITED: doc.qt.io/qt-6/opengl-changes-qt6.html]` — Qt6 OpenGL module reorganisation, QOpenGLWidget move to Qt OpenGL Widgets, ANGLE removal on Windows
- `[CITED: doc.qt.io/qt-6/qsurfaceformat.html]` — CompatibilityProfile + DeprecatedFunctions; setDefaultFormat before QApplication
- `[CITED: doc.qt.io/qt-6/qtcore-changes-qt6.html]` — QRegExp removal; Qt::SkipEmptyParts namespace move
- `[CITED: doc.qt.io/qt-6/widgets-changes-qt6.html]` — QDesktopWidget removal; QScreen replacement
- `[CITED: doc.qt.io/qt-6/qregexp.html]` — QRegExp → QRegularExpression migration guide
- `[CITED: doc.qt.io/qt-6/qrhiwidget.html]` — QRhiWidget API; limited ABI guarantees; Qt 6.7 introduction
- `[CITED: doc.qt.io/qt-6/whatsnew67.html]` — QRhiWidget added as tech preview in Qt 6.7
- `[VERIFIED: packages.ubuntu.com/noble/qt6-base-dev]` — Ubuntu 24.04 ships Qt 6.4.2 (below 6.5 floor)
- `[VERIFIED: vcpkg.io/en/package/qtbase.html]` — vcpkg Qt6 port is named `qtbase`, version 6.10.3
- `[CITED: github.com/jurplel/install-qt-action]` — GH Action for exact Qt version install via aqtinstall
- `[CITED: opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-an-opengl-hack/]` — Color-buffer FBO picking pattern (GL_SELECT replacement)
- `[VERIFIED: glRenderWindow.C direct read]` — existing gl_format_ static + QSurfaceFormat setup; initializeGL DIAG-01 line
- `[VERIFIED: rendererFactory.h direct read]` — current Kind enum, makeRenderer/makeSurface signatures
- `[VERIFIED: 02.1-VERIFICATION.md direct read]` — boundary confirmed in place; 9 residual stereo sites documented
- `[VERIFIED: grep of codebase]` — QRegExp sites (pyWidget.h:67-70, pyWidget.C:56-88), QDesktopWidget sites (stageSettings.C:433, scene.C:2581-2584, interactionMode.C:5), SkipEmptyParts (pluginManager.C:408)
- `[VERIFIED: vcpkg.json direct read]` — current manifest declares "qt5-base"
- `[VERIFIED: CMakePresets.json direct read]` — qt@5 prefix path in macos-homebrew preset
- `[VERIFIED: ci.yml direct read]` — ubuntu-22.04 runner; apt Qt5 packages; smoke check structure

### Secondary (MEDIUM confidence)
- `[CITED: developer.apple.com/forums/thread/694866]` — Apple Silicon: OpenGL deprecated since Mojave (2018), emulated on Metal; compat profile caps at GL 2.1
- `[CITED: wiki.qt.io/How_to_use_OpenGL_Core_Profile_with_Qt]` — Core profile + DeprecatedFunctions usage pattern under Qt

### Tertiary (LOW confidence)
- `[ASSUMED]` Homebrew `qt` at plan time provides Qt 6.9.x (needs verification at plan time with `brew info qt`)
- `[ASSUMED]` QRhiWidget is no longer tech-preview in Qt 6.8 (QRhi API stability page is ambiguous)

---

## Metadata

**Confidence breakdown:**
- Standard stack: HIGH — package names verified against official registries
- API delta: HIGH — grepped directly from codebase; verified against Qt 6 official docs
- Architecture: HIGH — renderer boundary verified from Phase 02.1 artefacts
- CI / Ubuntu 24.04 pitfall: HIGH — verified against packages.ubuntu.com
- Spike tradeoffs: MEDIUM — QRhi API stability section is ambiguous; GL-core pattern is well-documented

**Research date:** 2026-05-15
**Valid until:** 2026-08-15 (90 days — Qt and vcpkg port versions change quarterly; Ubuntu packages stable within a release)

---

## RESEARCH COMPLETE
