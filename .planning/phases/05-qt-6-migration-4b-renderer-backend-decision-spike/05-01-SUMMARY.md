---
phase: 05-qt-6-migration-4b-renderer-backend-decision-spike
plan: 01
subsystem: build-system
tags: [qt6, cmake, vcpkg, dependency-rename]
dependency_graph:
  requires: [Phase 04 vcpkg manifest, Phase 02.1 renderer boundary]
  provides:
    - "Qt 6.5 LTS floor (D-01)"
    - "Qt6::OpenGLWidgets in VIEW_DEP_LIBRARIES (D-04)"
    - "macOS preset -> Homebrew unversioned qt (D-03)"
    - "vcpkg.json qtbase + pinned builtin-baseline (D-03)"
  affects: [Plan 05-02 source API renames, Plan 05-04 CI matrix]
tech_stack:
  added: [Qt 6.11.x via Homebrew unversioned qt, Qt6::OpenGLWidgets, vcpkg qtbase, builtin-baseline c1ce926ddf75f166cbdd444bbaf463410ac99b17]
  patterns: [Qt5 -> Qt6 verbatim rename, LIST(APPEND ...) for Qt6:: components, soft Qt6LinguistTools with WARNING fallback]
key_files:
  created: []
  modified:
    - CMakeLists.txt
    - CMakePresets.json
    - vcpkg.json
    - cmake/BALLConfig.cmake.in
    - cmake/BALLMacros.cmake
    - cmake/BALLViewTranslations.cmake
    - source/APPLICATIONS/BALLVIEW/CMakeLists.txt
    - source/APPLICATIONS/MMFF94/CMakeLists.txt
    - source/APPLICATIONS/TOOLS/CMakeLists.txt
    - source/APPLICATIONS/UTILITIES/CMakeLists.txt
    - source/EXTENSIONS/BALLAXY/CMakeLists.txt
    - source/EXTENSIONS/JUPYTER/CMakeLists.txt
    - source/EXTENSIONS/PRESENTABALL/CMakeLists.txt
    - source/EXTENSIONS/SPACENAV/CMakeLists.txt
    - source/EXTENSIONS/VRPN/CMakeLists.txt
    - source/EXTENSIONS/VRPNHD/CMakeLists.txt
    - source/PYTHON/EXTENSIONS/CMakeLists.txt
decisions:
  - "D-01 locked: QT_MIN_VERSION=6.5 with FIND_PACKAGE(Qt6 6.5 ...) invocations on top-level + VIEW block"
  - "D-04 locked: Qt6::OpenGLWidgets appended to VIEW_DEP_LIBRARIES; required component on the VIEW-gated FIND_PACKAGE so glRenderWindow can link"
  - "D-03 locked: macOS preset CMAKE_PREFIX_PATH -> /opt/homebrew/opt/qt (Homebrew unversioned qt, currently 6.11.x); vcpkg.json declares qtbase with pinned builtin-baseline (c1ce926d…)"
  - "D-05 audited green: no Qt6::Core5Compat or qt5compat anywhere in build files; clean break"
  - "Rule 3 cascade: 13 additional CMakeLists/cmake files in the wider build needed Qt5 -> Qt6 macro/target renames to keep configure unblocked; these renames are scope-creep relative to the literal plan but are blocking-issue fixes"
metrics:
  duration_min: 2
  duration_human: "~2min wall (light dev box; fast configure cycle)"
  tasks_completed: 2
  files_modified: 17
  completed_date: "2026-05-15"
---

# Phase 5 Plan 01: CMake Qt 6 Bring-Up Summary

Renamed BALL/VIEW/BALLView's CMake + package-manifest layer from Qt 5 to Qt 6 (Qt 6.5 LTS floor, Homebrew unversioned `qt`, vcpkg `qtbase` + pinned baseline) so configure goes green against Qt 6.11 on macOS-arm64 via the `ci-macos` preset.

## What Shipped

**Task 1 — CMakeLists.txt Qt block rewrite** (commit `a805847`)
- `QT_MIN_VERSION` bumped from `5.15` to `6.5` (D-01).
- Top-level `FIND_PACKAGE(Qt6 6.5 REQUIRED Core Network Xml)`; VIEW-gated `FIND_PACKAGE(Qt6 6.5 REQUIRED OpenGL PrintSupport Test Widgets OpenGLWidgets)` (D-04 — `Qt6::OpenGLWidgets` is the new home for `QOpenGLWidget`, which `GLRenderWindow` inherits from).
- `LIST(APPEND BALL_DEP_LIBRARIES ...)` / `LIST(APPEND VIEW_DEP_LIBRARIES ...)` switched to `Qt6::` prefixes; `Qt6::OpenGLWidgets` added as the last VIEW entry.
- `Qt6LinguistTools` left as a soft find with a `MESSAGE(WARNING ...)` fallback (Plan 05-04 wires CI to satisfy it).
- `IF(USE_QTWEBENGINE)` block renamed Qt5 → Qt6 (`WebEngineCore` is the Qt 6 module name); option still defaults `ON`, so the block runs but doesn't block configure on macOS (qtwebengine is present in Homebrew).
- `BALL_PROJECT_COMPILE_DEFNS` `-DQT_NO_KEYWORDS` left untouched (still via `LIST(APPEND ...)` per Plan 04-04 lesson — no `ADD_DEFINITIONS` regression).
- Updated openssl-Windows-deploy loop at lines 844-857: rename `QT5_LINK_LIBRARIES` / `QT5_IMPORTED_TARGETS` to Qt6 variants; corresponding rename in `cmake/BALLConfig.cmake.in` so downstream `find_dependency()` consumers get `Qt6Core` / `Qt6OpenGLWidgets` etc.

**Task 2 — CMakePresets.json + vcpkg.json** (commit `cd912fc`)
- `macos-homebrew` preset: `CMAKE_PREFIX_PATH` switched from `/opt/homebrew/opt/qt@5;/opt/homebrew` to `/opt/homebrew/opt/qt;/opt/homebrew`. The `ci-macos` preset inherits and automatically picks up the change.
- `vcpkg.json`: `"qt5-base"` → `"qtbase"` (vcpkg's Qt 6 port). All other ports unchanged.
- `vcpkg.json`: `"builtin-baseline": "c1ce926ddf75f166cbdd444bbaf463410ac99b17"` (current vcpkg HEAD SHA at time of plan, 40-char hex; Phase 04-04 manifest-mode reproducibility pattern).

## Configure Log Excerpt (Proof)

Final `cmake --preset ci-macos -S .` run exits 0 in ~14s wall:

```
-- ...
-- Setting CTDConverter version to its default value (v2.0)
-- Could not find SpaceNavigator libraries. Disabling the SpaceNavigator plugin
-- Could not find VRPN libraries. Disabling the VRPN plugin
-- Could not find VRPN libraries. Disabling the VRPNHD plugin
-- Python support has not been enabled. Disabling the Jupyter plugin.
-- Found Doxygen: /opt/homebrew/bin/doxygen (found version "1.16.1")
-- Found LATEX: /Library/TeX/texbin/latex
--
-- You have successfully configured BALL.
-- Configuring done (11.2s)
-- Generating done (2.8s)
-- Build files have been written to: /Users/kohlbach/Claude/BALL/ball/build/ci-macos
```

Qt 6 resolution proof from `build/ci-macos/CMakeCache.txt`:

```
Qt6OpenGLWidgets_DIR:PATH=/opt/homebrew/opt/qt/lib/cmake/Qt6OpenGLWidgets
Qt6_DIR:PATH=/opt/homebrew/opt/qt/lib/cmake/Qt6
```

Homebrew `qt` resolves to `/opt/homebrew/Cellar/qt/6.11.0` (`qmake -v` reports `Using Qt version 6.11.0` — well above the 6.5 floor).

NOTE on `<verify>` automated gate: the gate's `grep -q 'Qt6' /tmp/cmake-configure.log` matches against the configure stdout. Modern Qt 6 `Qt6Config.cmake` is **silent on config-mode finds by default** (no `Found Qt6 X.Y` message gets emitted unless `--debug-find-pkg=Qt6` or `QUIET` is unset and a status hook fires). The literal "Qt6" token does not appear in the configure log; the load-bearing proof is `Qt6_DIR` + `Qt6OpenGLWidgets_DIR` in `CMakeCache.txt` (shown above) and a non-zero `Qt6` token count in the cache itself. All acceptance-criteria greps against source files pass.

## Acceptance Gate Tally

| Gate                                                                              | Result                                                                    |
| --------------------------------------------------------------------------------- | ------------------------------------------------------------------------- |
| Task 1 — `cmake --preset ci-macos -S .` exits 0                                  | PASS (exit 0, ~14s)                                                       |
| Task 1 — `grep -nE '^[^#]*FIND_PACKAGE\(Qt5' CMakeLists.txt`                       | empty (PASS)                                                              |
| Task 1 — `grep -cE 'Qt6::OpenGLWidgets' CMakeLists.txt` ≥ 1                       | 1 (PASS — D-04)                                                           |
| Task 1 — `grep -nE 'SET\(QT_MIN_VERSION 6\.5\)' CMakeLists.txt`                    | line 291 (PASS — D-01)                                                    |
| Task 1 — No `Qt6::Core5Compat` token                                              | PASS (D-05)                                                               |
| Task 2 — `grep -c 'qt@5' CMakePresets.json`                                       | 0 (PASS)                                                                  |
| Task 2 — `grep -c '/opt/homebrew/opt/qt' CMakePresets.json` ≥ 1                  | 1 (PASS — at line 22, embedded in semicolon-joined CMAKE_PREFIX_PATH)     |
| Task 2 — `grep -c '"qt5-base"' vcpkg.json`                                        | 0 (PASS)                                                                  |
| Task 2 — `grep -c '"qtbase"' vcpkg.json`                                          | 1 (PASS)                                                                  |
| Task 2 — `builtin-baseline` 40-char hex SHA                                       | `c1ce926ddf75f166cbdd444bbaf463410ac99b17` (PASS)                          |
| Task 2 — `ci-macos` reconfigures green                                            | PASS (exit 0)                                                             |
| Overall — D-05 audit: no `Qt6::Core5Compat` / `qt5compat` anywhere               | PASS (empty grep across CMakeLists/CMakePresets/vcpkg.json/cmake/)        |

## Deviations from Plan

### Auto-fixed Issues

**1. [Rule 3 - Blocking] Cascade-renamed Qt5 macros in 12 downstream CMakeLists / cmake/ files**

- **Found during:** Task 1, when the first ci-macos configure attempt threw `Unknown CMake command "QT5_WRAP_CPP"` from `source/EXTENSIONS/PRESENTABALL/CMakeLists.txt:37` after the top-level Qt5 → Qt6 rename.
- **Issue:** The plan's `<files_modified>` only listed `CMakeLists.txt`, `CMakePresets.json`, and `vcpkg.json`, but the wider tree carried Qt5-specific helper macros (`QT5_WRAP_CPP`, `QT5_WRAP_UI`, `QT5_ADD_RESOURCES`, `QT5_CREATE_TRANSLATION`, `QT5_ADD_TRANSLATION`) and target names (`Qt5::Core`, `Qt5::Widgets`, `Qt5::WebEngineWidgets`, `Qt5Widgets_EXECUTABLE_COMPILE_FLAGS`, `Qt5Widgets_UIC_EXECUTABLE`) in 12 additional files. These vanish in Qt 6 — `Qt6Config.cmake` ships `QT6_WRAP_CPP` etc. and the `Qt5Widgets_UIC_EXECUTABLE` variable is replaced by the `Qt6::uic` IMPORTED target. Without the renames, configure could not get past the first extension parse.
- **Fix:** Mechanical Qt5 → Qt6 rename in:
  - `cmake/BALLMacros.cmake` (`QT5_WRAP_UI_BALL` → `QT6_WRAP_UI_BALL` + switch to `Qt6::uic` IMPORTED target via `if(TARGET Qt6::uic)` guard)
  - `cmake/BALLViewTranslations.cmake` (translation macros + `Qt6LinguistTools_FOUND` guard)
  - `cmake/BALLConfig.cmake.in` (downstream `find_dependency()` template)
  - `source/EXTENSIONS/{PRESENTABALL,VRPN,VRPNHD,SPACENAV,JUPYTER,BALLAXY}/CMakeLists.txt`
  - `source/APPLICATIONS/{BALLVIEW,TOOLS,UTILITIES,MMFF94}/CMakeLists.txt`
  - `source/PYTHON/EXTENSIONS/CMakeLists.txt` (BALL_PYTHON_SUPPORT=OFF by default, but kept consistent)
- **Files modified:** 13 (listed in frontmatter `key_files.modified`)
- **Commit:** `a805847` (folded into Task 1 because they are inseparable — Task 1's configure-green done criterion cannot hold without them)

This is a scope-creep call I made deliberately: the plan's literal `files_modified: [CMakeLists.txt, CMakePresets.json, vcpkg.json]` is too narrow to reach Task 1's done criterion ("cmake --preset ci-macos configures green"). Per Rule 3, all renames are blocking-issue fixes (configure halts at the first un-renamed macro), not optional cleanup.

## Known Stubs

None.

## Threat Flags

None — build-system layer only; no new endpoints / auth paths / file access patterns.

## Open Items for Downstream Plans

- **Plan 05-02 (source API renames):** still has the original VIEW-source surface to migrate — `QRegExp` → `QRegularExpression`, `QDesktopWidget` → `QGuiApplication::screens()` / `QScreen`, `QtWidgets/QOpenGLWidget` → `QtOpenGLWidgets/QOpenGLWidget`, `QString::SkipEmptyParts` → `Qt::SkipEmptyParts`, plus `QSurfaceFormat::DeprecatedFunctions` opt-in on the gl_format_ static initializer (D-06).
- **Plan 05-04 (CI matrix):** Linux runner bump (`ubuntu-22.04` → `ubuntu-24.04`, D-02), apt → `jurplel/install-qt-action`, GL profile assertion step. macOS CI inherits the working Homebrew qt path automatically.
- **Plan 05-04 (Windows):** vcpkg manifest with `qtbase` + pinned baseline is in place; the first cold-build CI run on Windows will pay the qt 6 build cost (carry-forward note from Phase 04 D-03).
- **NB — `USE_QTWEBENGINE` still defaults `ON`:** on macOS this is fine (Homebrew `qtwebengine` available), but on a stock-vcpkg Windows runner that does not include `qtwebengine`, configure will warn-and-disable. The plan does not require disabling it; Plan 05-04 may need to flip the option off in the `ci-windows` preset or add `qtwebengine` to the vcpkg manifest.

## Self-Check: PASSED

- Files modified exist:
  - `CMakeLists.txt` — FOUND (verified by `grep -nE 'SET\(QT_MIN_VERSION 6\.5\)' CMakeLists.txt` returning line 291)
  - `CMakePresets.json` — FOUND (verified by `grep '/opt/homebrew/opt/qt;' CMakePresets.json` matching line 22)
  - `vcpkg.json` — FOUND (verified by `grep '"qtbase"' vcpkg.json` matching line 7 + builtin-baseline line)
  - 13 cascade files in `cmake/` + `source/` — FOUND (all listed in frontmatter, verified by git status pre-commit)
- Commits exist:
  - `a805847` (Task 1: CMakeLists.txt Qt 6 + Rule 3 cascades) — FOUND in `git log`
  - `cd912fc` (Task 2: CMakePresets + vcpkg.json) — FOUND in `git log`
- `cmake --preset ci-macos` exits 0 — VERIFIED (final run after both commits, exit code 0).
