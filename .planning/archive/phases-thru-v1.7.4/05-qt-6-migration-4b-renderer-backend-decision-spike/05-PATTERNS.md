# Phase 5: Qt 6 Migration (4b) + Renderer Backend Decision Spike — Pattern Map

**Mapped:** 2026-05-15
**Files analyzed:** 19 (new/modified)
**Analogs found:** 17 / 19

---

## File Classification

| New/Modified File | Role | Data Flow | Closest Analog | Match Quality |
|-------------------|------|-----------|----------------|---------------|
| `CMakeLists.txt` (lines 291–311) | build config | transform | `CMakeLists.txt` lines 291–311 (current Qt 5 block) | exact — same block, Qt5→Qt6 rename |
| `CMakePresets.json` (macos-homebrew preset) | build config | config | `CMakePresets.json` existing presets (lines 18–26) | exact — same preset, path update |
| `vcpkg.json` | build config | config | `vcpkg.json` (current Qt 5 manifest) | exact — same manifest, port rename |
| `.github/workflows/ci.yml` | CI workflow | batch | `.github/workflows/ci.yml` (current — Phase 04 pattern) | exact — same matrix structure |
| `source/APPLICATIONS/BALLVIEW/main.C` | source migration | request-response | `source/APPLICATIONS/BALLVIEW/main.C` lines 67–68 | exact — D-06 adds `DeprecatedFunctions` option |
| `source/VIEW/RENDERING/glRenderWindow.C` | source migration | transform | `source/VIEW/RENDERING/glRenderWindow.C` lines 37–56 | exact — QSurfaceFormat static initializer |
| `include/BALL/VIEW/RENDERING/glRenderWindow.h` | source migration | transform | `include/BALL/VIEW/RENDERING/glRenderWindow.h` line 32 | exact — header path change |
| `include/BALL/VIEW/WIDGETS/pyWidget.h` | source migration | transform | `include/BALL/VIEW/WIDGETS/pyWidget.h` lines 67–70 | exact — QRegExp→QRegularExpression |
| `source/VIEW/WIDGETS/pyWidget.C` | source migration | transform | `source/VIEW/WIDGETS/pyWidget.C` lines 56–112 | exact — QRegExp usage pattern |
| `source/VIEW/DIALOGS/stageSettings.C` | source migration | transform | `source/VIEW/DIALOGS/stageSettings.C` lines 24, 433–436 | exact — QDesktopWidget sites |
| `source/VIEW/WIDGETS/scene.C` | source migration | transform | `source/VIEW/WIDGETS/scene.C` lines 84, 2581–2584 | exact — QDesktopWidget stereo sites |
| `source/VIEW/KERNEL/MODES/interactionMode.C` | source migration | transform | `source/VIEW/KERNEL/MODES/interactionMode.C` line 5 | exact — dead include removal |
| `source/PLUGIN/pluginManager.C` | source migration | transform | `source/PLUGIN/pluginManager.C` line 408 | exact — one-token SkipEmptyParts fix |
| `include/BALL/VIEW/RENDERING/rendererFactory.h` | model/config | CRUD | `include/BALL/VIEW/RENDERING/rendererFactory.h` (exists) | exact — add `Kind::OpenGL_Core`, `Kind::QRhi` values |
| `source/VIEW/RENDERING/rendererFactory.C` | service | CRUD | `source/VIEW/RENDERING/rendererFactory.C` (exists) | exact — add new switch cases |
| `include/BALL/VIEW/RENDERING/RENDERERS/coreGLRenderer.h` | new renderer subclass | request-response | `include/BALL/VIEW/RENDERING/RENDERERS/renderer.h` (abstract base) | role-match — same Renderer subclass pattern |
| `source/VIEW/RENDERING/RENDERERS/coreGLRenderer.C` | new renderer subclass | request-response | `source/VIEW/RENDERING/RENDERERS/glRenderer.C` | role-match — same Renderer subclass impl |
| `scripts/check-no-legacy-qt6-symbols.sh` | utility/lint | batch | `.planning/phases/02-rendering-port-4a/scripts/check-no-legacy-gl-symbols.sh` | exact — same grep-gate shell script pattern |
| `.planning/phases/05-.../05-SPIKE-DECISION.md` | doc | — | `.planning/RENDERER-INTERFACE-BOUNDARY.md` (doc structure) | partial — planning artefact, no code analog |

---

## Pattern Assignments

### `CMakeLists.txt` lines 291–311 (build config, transform)

**Analog:** `CMakeLists.txt` lines 291–311 (the current Qt 5 block — this IS the file being changed)

**Current Qt 5 block** (lines 291–311 — read this before editing):
```cmake
SET(QT_MIN_VERSION 5.15)

FIND_PACKAGE(Qt5 ${QT_MIN_VERSION} REQUIRED Core Network Xml)

LIST(APPEND BALL_DEP_LIBRARIES Qt5::Core
                               Qt5::Network
                               Qt5::Xml)
LIST(APPEND BALL_PROJECT_COMPILE_DEFNS "-DQT_NO_KEYWORDS")

IF (BALL_HAS_VIEW)
  FIND_PACKAGE(Qt5 ${QT_MIN_VERSION} REQUIRED OpenGL PrintSupport Test Widgets)
  FIND_PACKAGE(Qt5LinguistTools ${QT_MIN_VERSION})

  LIST(APPEND VIEW_DEP_LIBRARIES Qt5::OpenGL
                                 Qt5::PrintSupport
                                 Qt5::Test
                                 Qt5::Widgets)
```

**Target Qt 6 block** (D-01, D-04 — verbatim from RESEARCH.md §Code Examples):
```cmake
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
```

**Key rules:**
- `LIST(APPEND ...)` style is mandatory — do NOT use `SET("... string")`. See `BALL_PROJECT_COMPILE_DEFNS` usage at line 298 as the canonical example (Plan 04-04 lesson: mixing styles produces ADD_DEFINITIONS garbage on MSVC).
- `Qt6::OpenGLWidgets` is the new required module — `QOpenGLWidget` moved out of `Qt::OpenGL` into its own module. Missing this causes `glRenderWindow.h` to fail.
- Also update the optional `IF(USE_QTWEBENGINE)` block at line 314 (Qt5 → Qt6 rename, leave disabled).

---

### `CMakePresets.json` — `macos-homebrew` preset (build config, config)

**Analog:** `CMakePresets.json` lines 18–26 (current `macos-homebrew` preset)

**Current** (lines 18–26):
```json
{
  "name": "macos-homebrew",
  "displayName": "macOS / Homebrew",
  "inherits": "base",
  "cacheVariables": {
    "CMAKE_PREFIX_PATH": "/opt/homebrew/opt/qt@5;/opt/homebrew",
    "BISON_EXECUTABLE": "/opt/homebrew/opt/bison/bin/bison",
    "FLEX_EXECUTABLE": "/opt/homebrew/opt/flex/bin/flex"
  }
}
```

**Change:** `qt@5` → `qt` (Homebrew's unversioned Qt 6 formula). No other preset fields change. The `ci-macos` preset inherits from `macos-homebrew` and picks up the change automatically.

```json
"CMAKE_PREFIX_PATH": "/opt/homebrew/opt/qt;/opt/homebrew",
```

---

### `vcpkg.json` (build config, config)

**Analog:** `vcpkg.json` (current file — exact pattern, single port rename)

**Current** (`vcpkg.json` line 6):
```json
"qt5-base",
```

**Change:** `"qt5-base"` → `"qtbase"` (vcpkg's Qt 6 port name; verified at vcpkg.io/en/package/qtbase.html). All other entries (`boost-*`, `eigen3`, `fftw3`, `glew`, `tbb`, `libsvm`) stay unchanged.

**Also required:** add a pinned `"builtin-baseline"` if not already present — the Phase 04 Plan 04-04 pattern requires a 40-char SHA. Current `vcpkg.json` does NOT have `builtin-baseline`; it must be added. Use `git ls-remote https://github.com/microsoft/vcpkg HEAD` to get the current SHA.

---

### `.github/workflows/ci.yml` (CI workflow, batch)

**Analog:** `.github/workflows/ci.yml` (current file — Phase 04 matrix-driven pattern)

**Read first:** `.github/workflows/ci.yml` lines 44–347 (full matrix + steps).

**Changes required (four distinct edits — all in the Linux matrix entry and steps):**

**1. Runner bump** (line 59 area — `linux-x64` matrix entry):
```yaml
# before:
runner: ubuntu-22.04
# after:
runner: ubuntu-24.04
```

**2. jurplel/install-qt-action step** (replaces the apt Qt packages in the Linux `Install dependencies` step):
```yaml
- name: Install Qt (Linux — aqtinstall via jurplel/install-qt-action)
  if: matrix.os == 'linux-x64'
  uses: jurplel/install-qt-action@v4
  with:
    version: '6.5.*'
    host: 'linux'
    target: 'desktop'
    modules: 'qtbase qttools qtopengl qt5compat'
    cache: true
```
Remove `qtbase5-dev qtbase5-dev-tools libqt5opengl5-dev qttools5-dev` from the apt install list. Keep `xvfb mesa-utils libgl1-mesa-dri` for headless smoke check.

**3. GL profile assertion step** (add after `Render smoke check (Linux)` step — Wave 0 gap from RESEARCH.md):
```yaml
- name: Assert GL compat profile (Linux)
  if: matrix.smoke && matrix.os == 'linux-x64'
  run: |
    # D-06 / D-08 oracle: verify the context is compatibility profile, not core.
    grep 'BALLVIEW_GL_DIAG.*gl_profile=compatibility' /tmp/ballview-smoke.log
```

**4. Spike smoke variant step** (optional non-blocking step, macOS only — Wave 0 gap):
```yaml
- name: Spike smoke check (macOS — GLCore backend)
  if: matrix.os == 'macos-arm64'
  continue-on-error: true
  run: |
    cmake --build --preset ci-macos --target BALLView -DBALL_SPIKE_BACKEND=GLCore -j$(sysctl -n hw.ncpu)
    bash .planning/phases/02.2-ci-and-build-smoke-matrix/scripts/render-smoke-check.sh \
      build/ci-macos/bin/BALLView.app/Contents/MacOS/BALLView
```

**Pattern rules from Phase 04:**
- All per-OS logic lives IN the matrix entry, not in scattered `if:` chains (D-11).
- `continue-on-error: ${{ !matrix.blocking }}` is the blocking toggle — do not add new `continue-on-error` fields to individual steps except the non-blocking spike variant.
- Cache keys use `hashFiles(...)` (see `vcpkg-${{ matrix.os }}-${{ hashFiles('vcpkg.json', ...) }}`).

---

### `source/APPLICATIONS/BALLVIEW/main.C` (source migration, request-response)

**Analog:** `source/APPLICATIONS/BALLVIEW/main.C` lines 62–68 (the current pre-QApplication setup)

**Current** (lines 62–68):
```cpp
qInstallMessageHandler(logMessages);

// QOpenGLWidget: all GL contexts must share (display lists / textures across
// multiple scene windows) and inherit the compatibility-profile 2.1 format.
// Both must be set BEFORE the QApplication is constructed.
QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
QSurfaceFormat::setDefaultFormat(BALL::VIEW::GLRenderWindow::gl_format_);
```

**No code change needed here.** The `QSurfaceFormat::setDefaultFormat` call at line 68 is already the correct Qt 6 pattern (must fire before QApplication). What changes is in `glRenderWindow.C` — the `gl_format_` static initializer must add `DeprecatedFunctions` (see next section). The call in `main.C` stays identical.

**D-06 verification:** confirm `Qt::AA_ShareOpenGLContexts` (line 67) is still valid in Qt 6 — it is: the attribute exists unchanged in Qt 6.

---

### `source/VIEW/RENDERING/glRenderWindow.C` + `include/BALL/VIEW/RENDERING/glRenderWindow.h` (source migration, transform)

**Analog:** `source/VIEW/RENDERING/glRenderWindow.C` lines 37–56 (the `gl_format_` static init)

**Current** (`glRenderWindow.C` lines 37–56):
```cpp
QSurfaceFormat GLRenderWindow::gl_format_ = [] {
  QSurfaceFormat fmt;
  fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
  fmt.setVersion(2, 1);
  fmt.setDepthBufferSize(24);
  fmt.setStencilBufferSize(8);
  fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
  fmt.setSamples(4);
#ifndef BALL_OS_DARWIN
#  if !defined(BALL_OS_LINUX) || QT_VERSION < QT_VERSION_CHECK(5, 7, 0) || QT_VERSION > QT_VERSION_CHECK(5, 7, 1)
  fmt.setStereo(true);
#  endif
#endif
  return fmt;
}();
```

**Target** (D-06 — add `DeprecatedFunctions` option):
```cpp
QSurfaceFormat GLRenderWindow::gl_format_ = [] {
  QSurfaceFormat fmt;
  fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
  fmt.setOptions(QSurfaceFormat::DeprecatedFunctions);  // Qt 6: explicit legacy entry points
  fmt.setVersion(2, 1);
  fmt.setDepthBufferSize(24);
  fmt.setStencilBufferSize(8);
  fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
  fmt.setSamples(4);
#ifndef BALL_OS_DARWIN
#  if !defined(BALL_OS_LINUX) || QT_VERSION < QT_VERSION_CHECK(5, 7, 0) || QT_VERSION > QT_VERSION_CHECK(5, 7, 1)
  fmt.setStereo(true);
#  endif
#endif
  return fmt;
}();
```

**`glRenderWindow.h` line 32 — header path change (Pitfall 4):**
```cpp
// before:
#include <QtWidgets/QOpenGLWidget>
// after (Qt 6 — QOpenGLWidget moved to QtOpenGLWidgets):
#include <QtOpenGLWidgets/QOpenGLWidget>
```

---

### `include/BALL/VIEW/WIDGETS/pyWidget.h` (source migration, transform)

**Analog:** `include/BALL/VIEW/WIDGETS/pyWidget.h` lines 67–70 (current QRegExp member declarations)

**Current** (lines 67–70):
```cpp
vector<QRegExp> python_patterns;
vector<QRegExp> BALL_patterns;
QRegExp         string_pattern;
QRegExp         comment_pattern;
```

**Target** (D-05 — inline replacement, no Core5Compat):
```cpp
#include <QtCore/QRegularExpression>   // add to includes (replace or alongside QtCore/QRegExp if present)

vector<QRegularExpression> python_patterns;
vector<QRegularExpression> BALL_patterns;
QRegularExpression         string_pattern;
QRegularExpression         comment_pattern;
```

**Include guard note:** the current file includes `<QtCore/QRegExp>` indirectly. Remove any explicit `QRegExp` include; add `<QtCore/QRegularExpression>`.

---

### `source/VIEW/WIDGETS/pyWidget.C` (source migration, transform)

**Analog:** `source/VIEW/WIDGETS/pyWidget.C` lines 49–112 (the entire `PythonHighlighter` class implementation)

**Current pattern** (lines 49–68 — `compilePattern()` + construction sites):
```cpp
void PythonHighlighter::compilePattern()
{
  python_patterns.clear();
  QString delim("\\b");
  QStringList::iterator lit = python_keywords.begin();
  for (; lit != python_keywords.end(); lit++)
  {
    python_patterns.push_back(QRegExp(delim + *lit + delim));   // line 56
  }

  BALL_patterns.clear();
  lit = BALL_keywords.begin();
  for (; lit != BALL_keywords.end(); lit++)
  {
    BALL_patterns.push_back(QRegExp(delim + *lit + delim));     // line 63
  }

  string_pattern  = QRegExp("\".*\"");  // line 66
  comment_pattern = QRegExp("#.*");     // line 67
}
```

**Target** (D-05 — QRegularExpression replacement):
```cpp
python_patterns.push_back(QRegularExpression(delim + *lit + delim));  // line 56
// ...
BALL_patterns.push_back(QRegularExpression(delim + *lit + delim));    // line 63

string_pattern  = QRegularExpression("\".*\"");  // line 66
comment_pattern = QRegularExpression("#.*");      // line 67
```

**Current pattern** (`highlightBlock` loop — lines 74–84, and 86–97 for BALL_patterns):
```cpp
const QRegExp& expression = python_patterns[p];
Index index = expression.indexIn(text);
while (index >= 0)
{
  int length = expression.matchedLength();
  setFormat(index, length, python_format);
  index = text.indexOf(expression, index + length);
}
```

**Target** (QRegularExpressionMatchIterator pattern):
```cpp
const QRegularExpression& expression = python_patterns[p];
QRegularExpressionMatchIterator it = expression.globalMatch(text);
while (it.hasNext())
{
  QRegularExpressionMatch m = it.next();
  setFormat(m.capturedStart(), m.capturedLength(), python_format);
}
```

**Same iterator pattern applies to** `string_pattern` (lines 98–104) and `comment_pattern` (lines 106–112).

---

### `source/VIEW/DIALOGS/stageSettings.C` (source migration, transform)

**Analog:** `source/VIEW/DIALOGS/stageSettings.C` lines 24, 433–436 (QDesktopWidget sites)

**Current** (line 24, includes block):
```cpp
#include <QtWidgets/QDesktopWidget>
```

**Target:**
```cpp
#include <QtGui/QScreen>
#include <QtGui/QGuiApplication>
```

**Current** (lines 433–436):
```cpp
QDesktopWidget* desktop = QApplication::desktop();

QRect left_screen_geom  = QApplication::desktop()->screenGeometry(left_screen_index);
QRect right_screen_geom = QApplication::desktop()->screenGeometry(right_screen_index);
```

**Target:**
```cpp
QList<QScreen*> screens = QGuiApplication::screens();
QRect left_screen_geom  = screens.value(left_screen_index) ? screens.value(left_screen_index)->geometry() : QRect();
QRect right_screen_geom = screens.value(right_screen_index) ? screens.value(right_screen_index)->geometry() : QRect();
```

---

### `source/VIEW/WIDGETS/scene.C` (source migration, transform)

**Analog:** `source/VIEW/WIDGETS/scene.C` lines 84, 2581–2584 (QDesktopWidget stereo sites)

**Current** (line 84):
```cpp
#include <QtWidgets/QDesktopWidget>
```

**Target:**
```cpp
#include <QtGui/QScreen>
#include <QtGui/QGuiApplication>
```

**Current** (lines 2581–2584):
```cpp
QDesktopWidget* desktop = QApplication::desktop();

QWidget* left_screen  = desktop->screen(left_screen_index);
QWidget* right_screen = desktop->screen(right_screen_index);
```

**Target** (Pitfall 5 — `QScreen*` is not `QWidget*`; these are the guard-deferred stereo sites):
```cpp
// desktop and QWidget* screen() no longer exist in Qt 6.
// QScreen* is not QWidget* — pass nullptr as parent, then setScreen() after construction.
QScreen* left_screen_obj  = QGuiApplication::screens().value(left_screen_index);
QScreen* right_screen_obj = QGuiApplication::screens().value(right_screen_index);
// ...
GLRenderWindow* w = new GLRenderWindow(nullptr, "left eye", Qt::FramelessWindowHint);
if (left_screen_obj)
{
  w->setScreen(left_screen_obj);
  w->move(left_screen_obj->geometry().topLeft());
}
```

---

### `source/VIEW/KERNEL/MODES/interactionMode.C` (source migration, transform)

**Analog:** `source/VIEW/KERNEL/MODES/interactionMode.C` line 5

**Change:** Remove the dead include (verified by RESEARCH.md — no actual QDesktopWidget usage beyond the include):
```cpp
// remove:
#include <QtWidgets/QDesktopWidget>
```

---

### `source/PLUGIN/pluginManager.C` (source migration, transform)

**Analog:** `source/PLUGIN/pluginManager.C` line 408

**Current** (line 408):
```cpp
autoactivate_plugins_ = value.split(";", QString::SkipEmptyParts);
```

**Target** (Pitfall 8 — one-token fix):
```cpp
autoactivate_plugins_ = value.split(";", Qt::SkipEmptyParts);
```

---

### `include/BALL/VIEW/RENDERING/rendererFactory.h` (model, CRUD)

**Analog:** `include/BALL/VIEW/RENDERING/rendererFactory.h` lines 35–42 (the existing `Kind` enum)

**Current** (lines 35–42):
```cpp
enum class Kind
{
  OpenGL_Fixed,   ///< today's GLRenderer (fixed-function compat profile)
  Raytracer,      ///< CPU raytracer (BALL_HAS_RTFACT)
  POV,            ///< POV-Ray file renderer
  STL,            ///< STL file renderer
  VRML            ///< VRML file renderer
};
```

**Target** (add spike backends behind CMake option guard):
```cpp
enum class Kind
{
  OpenGL_Fixed,   ///< today's GLRenderer (fixed-function compat profile)
  Raytracer,      ///< CPU raytracer (BALL_HAS_RTFACT)
  POV,            ///< POV-Ray file renderer
  STL,            ///< STL file renderer
  VRML,           ///< VRML file renderer
  // SPIKE only — compile with -DBALL_SPIKE_BACKEND=GLCore or =QRhi.
  // These values are NOT production backends. Remove before PIPE-01.
#ifdef BALL_SPIKE_BACKEND_GLCORE
  OpenGL_Core,    ///< THROWAWAY: GL 3.2+ core-profile renderer spike (Phase 5)
#endif
#ifdef BALL_SPIKE_BACKEND_QRHI
  QRhi            ///< THROWAWAY: QRhiWidget-based renderer spike (Phase 5, requires Qt 6.7+)
#endif
};
```

**Header boilerplate pattern** (copy from `rendererFactory.h` lines 1–13):
```cpp
// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_RENDERERFACTORY_H
#define BALL_VIEW_RENDERING_RENDERERFACTORY_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

class QWidget;
```

---

### `source/VIEW/RENDERING/rendererFactory.C` (service, CRUD)

**Analog:** `source/VIEW/RENDERING/rendererFactory.C` lines 24–83 (the complete switch-case pattern)

**Current `makeRenderer` switch** (lines 24–53):
```cpp
Renderer* makeRenderer(Kind kind)
{
  switch (kind)
  {
    case Kind::OpenGL_Fixed:
      return new GLRenderer;

    case Kind::Raytracer:
#ifdef BALL_HAS_RTFACT
      return new RTfactRenderer();
#else
      return new GLRenderer;
#endif

    case Kind::POV:
      return new POVRenderer;

    case Kind::STL:
      return new STLRenderer;

    case Kind::VRML:
      return new VRMLRenderer;

    default:
      return 0;
  }
}
```

**Add spike cases** (following the same pattern — ifdef-gated, raw-pointer return):
```cpp
#ifdef BALL_SPIKE_BACKEND_GLCORE
    case Kind::OpenGL_Core:
      return new CoreGLRenderer;
#endif
#ifdef BALL_SPIKE_BACKEND_QRHI
    case Kind::QRhi:
      return new RhiRenderer;
#endif
```

**Current `makeSurface` switch** (lines 55–77 — pattern for the spike `QRhi` surface case):
```cpp
RenderSurface* makeSurface(Kind kind, QWidget* parent)
{
  switch (kind)
  {
    case Kind::OpenGL_Fixed:
      return new GLRenderWindow(parent);

    case Kind::Raytracer:
      return new GLRenderWindow(parent);

    case Kind::POV:
    case Kind::STL:
    case Kind::VRML:
      return 0;           // file renderers have no on-screen surface

    default:
      return 0;
  }
}
```

**Add spike surface cases:**
```cpp
#ifdef BALL_SPIKE_BACKEND_GLCORE
    case Kind::OpenGL_Core:
      return new GLRenderWindow(parent);  // GL-core spike reuses QOpenGLWidget surface
#endif
#ifdef BALL_SPIKE_BACKEND_QRHI
    case Kind::QRhi:
      return new QtRhiSurface(parent);    // QRhiWidget-based surface
#endif
```

**sources.cmake pattern** — add new `.C` files to `source/VIEW/RENDERING/RENDERERS/sources.cmake` (lines 1–11), matching the existing one-filename-per-line format. Do NOT add header files:
```cmake
SET(SOURCES_LIST
  glRenderer.C
  renderer.C
  ...
  coreGLRenderer.C      # ADD for spike
)
```

---

### `include/BALL/VIEW/RENDERING/RENDERERS/coreGLRenderer.h` (new renderer subclass, request-response)

**Analog:** `include/BALL/VIEW/RENDERING/RENDERERS/renderer.h` (abstract base — copy the class shell pattern)

**Header pattern** (copy from `renderer.h` lines 1–12 for modeline + include guard + BALL includes):
```cpp
// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// THROWAWAY SPIKE — Phase 5 prototype only. Not production code. Remove before PIPE-01.

#ifndef BALL_VIEW_RENDERING_RENDERERS_COREGLRENDERER_H
#define BALL_VIEW_RENDERING_RENDERERS_COREGLRENDERER_H

#ifndef BALL_VIEW_RENDERING_RENDERERS_RENDERER_H
# include <BALL/VIEW/RENDERING/RENDERERS/renderer.h>
#endif
```

**Class shell pattern** (from `renderer.h` lines 54–99 — copy constructor/destructor/override shape):
```cpp
namespace BALL
{
  namespace VIEW
  {
    /** GL 3.2+ core-profile renderer spike.
     *  THROWAWAY: Phase 5 prototype. Remove before PIPE-01.
     *  Overrides renderRepresentations_() and capabilities(); ignores per-primitive virtuals.
     */
    class BALL_VIEW_EXPORT CoreGLRenderer : public Renderer
    {
      public:
        CoreGLRenderer();
        virtual ~CoreGLRenderer() {}

        virtual bool init(Scene& scene) override;

        // Retained-mode entry point — override INSTEAD of per-primitive virtuals.
        virtual void renderRepresentations_(const RepresentationList& representations) override;

        // Capability query — callers branch on this, not on concrete type.
        virtual Caps capabilities() const override;

        virtual void pickObjects(Position x1, Position y1, Position x2, Position y2,
                                 std::list<GeometricObject*>& objects) override;
    };
  } // namespace VIEW
} // namespace BALL
```

---

### `source/VIEW/RENDERING/RENDERERS/coreGLRenderer.C` (new renderer subclass, request-response)

**Analog:** `source/VIEW/RENDERING/RENDERERS/glRenderer.C` (the existing concrete Renderer subclass — copy `init()` structure and `capabilities()` shape)

**File header + include pattern** (copy from `rendererFactory.C` lines 1–15 for modeline + namespace structure):
```cpp
// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// THROWAWAY SPIKE — Phase 5 prototype only. Not production code. Remove before PIPE-01.

#include <BALL/VIEW/RENDERING/RENDERERS/coreGLRenderer.h>
// GL headers:
#ifdef BALL_HAS_GLEW
# include <GL/glew.h>
#endif
#include <QtGui/QOpenGLFunctions_3_2_Core>
```

**`capabilities()` pattern** (from `renderer.h` lines 191–204 — the Caps struct):
```cpp
Renderer::Caps CoreGLRenderer::capabilities() const
{
  return Caps{
    /* retained_mode */ true,   // this backend overrides renderRepresentations_()
    /* offscreen     */ false,
    /* picking       */ true,   // color-buffer FBO picking
    /* stereo        */ false   // spike does not implement stereo
  };
}
```

**Color-buffer picking pattern** (from RESEARCH.md §Code Examples):
```cpp
// Render pass 1 (picking FBO): each object color-encodes its ID.
// On mouse click:
GLuint id = 0;
glBindFramebuffer(GL_READ_FRAMEBUFFER, picking_fbo_);
glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &id);
glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
// id → look up GeometricObject* in the object table
```

---

### `scripts/check-no-legacy-qt6-symbols.sh` (utility/lint, batch)

**Analog:** `.planning/phases/02-rendering-port-4a/scripts/check-no-legacy-gl-symbols.sh` (full file — copy verbatim structure)

**Shell script structure** (from Phase 02 script lines 1–70 — copy exactly):
```bash
#!/usr/bin/env bash
# check-no-legacy-qt6-symbols.sh
# Wave 0 verification gate for Phase 5 Qt 6 migration.
# Exits 0 if no legacy symbols remain; exits 1 if any are found.
# Usage: check-no-legacy-qt6-symbols.sh [REPO_ROOT]

set -u

# Resolve repo root (same pattern as check-no-legacy-gl-symbols.sh lines 28-40)
if [[ $# -ge 1 && -n "${1:-}" ]]; then
  REPO_ROOT="$1"
else
  SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
  REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
fi
```

**Legacy symbol set for Qt 6 migration** (replaces Phase 02's GL pattern):
```bash
LEGACY_PATTERN='QRegExp|QDesktopWidget|QApplication::desktop|QString::SkipEmptyParts|QtWidgets/QOpenGLWidget|QtWidgets/QDesktopWidget|Qt5::'
```

**Scan directories:**
```bash
SCAN_DIRS=(
  "${REPO_ROOT}/source"
  "${REPO_ROOT}/include/BALL"
)
```

**Grep + comment-filter pattern** (copy from Phase 02 script lines 57–67 verbatim):
```bash
HITS="$(grep -rEn "${LEGACY_PATTERN}" "${SCAN_DIRS[@]}" 2>/dev/null \
  | grep -vE ':[0-9]+:[[:space:]]*(//|\*|/\*)' || true)"

if [[ -n "${HITS}" ]]; then
  echo "${HITS}"
  COUNT="$(printf '%s\n' "${HITS}" | grep -c '')"
  echo ""
  echo "${COUNT} legacy Qt 5 symbol reference(s) remain in code"
  exit 1
fi

echo "0 legacy Qt 5 symbol references remain in code"
exit 0
```

**CI wiring** (add to the `lint` job in `ci.yml` after the existing legacy-GL step — same `lint` job structure as lines 338–347):
```yaml
- name: Legacy Qt 5 symbol grep gate
  run: |
    bash scripts/check-no-legacy-qt6-symbols.sh "$GITHUB_WORKSPACE"
```

---

## Shared Patterns

### BALL modeline + namespace wrapper
**Source:** Every `.h` / `.C` file in the project (canonical examples: `rendererFactory.h` lines 1–3, `glRenderWindow.C` lines 1–3)
**Apply to:** All new spike files (`coreGLRenderer.h`, `coreGLRenderer.C`, `rhiRenderer.C`, etc.)
```cpp
// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
```
Plus include guard pattern: `#ifndef BALL_VIEW_RENDERING_RENDERERS_{CLASSNAME}_H`.
Plus namespace wrapper: `namespace BALL { namespace VIEW { ... } }`.

### BALL_VIEW_EXPORT macro
**Source:** `rendererFactory.h` lines 44–46, `renderSurface.h` line 25, `renderer.h` line 54
**Apply to:** All new spike class declarations
```cpp
class BALL_VIEW_EXPORT CoreGLRenderer : public Renderer { ... };
```

### Raw-pointer ownership (no `std::unique_ptr`)
**Source:** `rendererFactory.C` lines 24–77 — all factory functions return raw `Renderer*` / `RenderSurface*`
**Apply to:** `rendererFactory.C` spike cases, spike constructor return paths
```cpp
// Return raw pointer — caller owns. Do NOT use std::unique_ptr.
return new CoreGLRenderer;
```

### `LIST(APPEND ...)` for CMake variable accumulation
**Source:** `CMakeLists.txt` lines 295–298 (BALL_DEP_LIBRARIES, BALL_PROJECT_COMPILE_DEFNS)
**Apply to:** Any new `-D` defines or library targets added in Phase 5 (e.g., `-DBALL_SPIKE_BACKEND_GLCORE`)
```cmake
LIST(APPEND BALL_PROJECT_COMPILE_DEFNS "-DBALL_SPIKE_BACKEND_GLCORE")
# NOT: SET(BALL_PROJECT_COMPILE_DEFNS "${BALL_PROJECT_COMPILE_DEFNS} -DBALL_SPIKE_BACKEND_GLCORE")
```

### `IF(BALL_OS_DARWIN)` / `IF(BALL_COMPILER_MSVC)` for per-OS branching
**Source:** `glRenderWindow.C` lines 45–53 (the `#ifndef BALL_OS_DARWIN` stereo guard)
**Apply to:** Any platform-specific code in spike renderer (e.g., Metal availability check for QRhi path)
```cpp
#ifndef BALL_OS_DARWIN
  // Linux/Windows-specific path
#endif
```

### sources.cmake registration
**Source:** `source/VIEW/RENDERING/RENDERERS/sources.cmake` lines 1–17; `source/VIEW/RENDERING/sources.cmake` lines 1–16
**Apply to:** Every new `.C` file in `source/VIEW/RENDERING/RENDERERS/` (spike renderer files)
```cmake
SET(SOURCES_LIST
  glRenderer.C
  renderer.C
  ...
  coreGLRenderer.C     # add here — one filename per line, NO path prefix
)
```
Headers are NOT listed in any `.cmake` file.

---

## No Analog Found

| File | Role | Data Flow | Reason |
|------|------|-----------|--------|
| `.planning/phases/05-.../05-SPIKE-DECISION.md` | doc/artefact | — | Planning decision record; no code analog. Template from RESEARCH.md §SPIKE-02 Decision Record Template. Use the table in RESEARCH.md §GL-Core vs QRhi Spike Backend Tradeoffs as the decision record structure. |

---

## Metadata

**Analog search scope:** `include/BALL/VIEW/RENDERING/`, `source/VIEW/RENDERING/`, `source/VIEW/WIDGETS/`, `source/VIEW/DIALOGS/`, `source/VIEW/KERNEL/MODES/`, `source/PLUGIN/`, `source/APPLICATIONS/BALLVIEW/`, `CMakeLists.txt`, `CMakePresets.json`, `vcpkg.json`, `.github/workflows/ci.yml`, `.planning/phases/02-rendering-port-4a/scripts/`
**Files scanned:** 19 new/modified + 12 analog source files read
**Pattern extraction date:** 2026-05-15

---

## PATTERN MAPPING COMPLETE
