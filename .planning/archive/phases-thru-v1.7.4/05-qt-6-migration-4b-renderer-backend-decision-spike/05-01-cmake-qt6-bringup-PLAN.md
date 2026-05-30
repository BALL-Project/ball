---
phase: 05-qt-6-migration-4b-renderer-backend-decision-spike
plan: 01
type: execute
wave: 1
depends_on: []
files_modified:
  - CMakeLists.txt
  - CMakePresets.json
  - vcpkg.json
autonomous: true
requirements: [QT6-01]
must_haves:
  truths:
    - "D-01: QT_MIN_VERSION is 6.5 and FIND_PACKAGE(Qt6 6.5 ...) is invoked"
    - "D-03: macOS preset CMAKE_PREFIX_PATH points at Homebrew unversioned qt; Windows vcpkg manifest declares qtbase (not qt5-base)"
    - "D-04: VIEW_DEP_LIBRARIES contains Qt6::OpenGLWidgets in addition to the renamed Qt6:: components"
    - "BALL/VIEW/BALLView configure step succeeds against Qt 6 on macOS-arm64 via ci-macos preset"
  artifacts:
    - path: "CMakeLists.txt"
      provides: "Qt6 FIND_PACKAGE block + LIST(APPEND ...) for Qt6:: components incl. OpenGLWidgets"
      contains: "FIND_PACKAGE(Qt6 ${QT_MIN_VERSION} REQUIRED ... OpenGLWidgets)"
    - path: "CMakePresets.json"
      provides: "macos-homebrew preset pointing at Homebrew qt (Qt 6)"
      contains: "/opt/homebrew/opt/qt"
    - path: "vcpkg.json"
      provides: "Windows Qt6 dependency declaration"
      contains: "qtbase"
  key_links:
    - from: "CMakeLists.txt"
      to: "Qt6::OpenGLWidgets target"
      via: "FIND_PACKAGE(Qt6 ... OpenGLWidgets) + LIST(APPEND VIEW_DEP_LIBRARIES Qt6::OpenGLWidgets)"
      pattern: "Qt6::OpenGLWidgets"
    - from: "CMakePresets.json macos-homebrew"
      to: "/opt/homebrew/opt/qt"
      via: "CMAKE_PREFIX_PATH"
      pattern: "/opt/homebrew/opt/qt(?!@5)"
---

<objective>
Rename the CMake + package-manifest layer from Qt 5 to Qt 6 so BALL/VIEW/BALLView **configure** cleanly against Qt 6.5 LTS or newer on all three platforms.

Purpose: Locks in user decisions D-01 (Qt 6.5 floor), D-03 (Homebrew qt + vcpkg qtbase), D-04 (Qt6:: components + new Qt6::OpenGLWidgets). The Qt6::OpenGLWidgets line is load-bearing — without it, the existing `glRenderWindow.h` (Phase 2 QOpenGLWidget port) cannot link under Qt 6 because Qt 6 split `QOpenGLWidget` out of `Qt::OpenGL` into its own module.

Output: CMakeLists.txt + CMakePresets.json + vcpkg.json updated. Configure step green on macOS (Homebrew qt); other-platform configure is gated by Plan 04 CI changes.
</objective>

<execution_context>
@$HOME/.claude/get-shit-done/workflows/execute-plan.md
@$HOME/.claude/get-shit-done/templates/summary.md
</execution_context>

<context>
@.planning/STATE.md
@.planning/ROADMAP.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-CONTEXT.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-RESEARCH.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-PATTERNS.md
@CMakeLists.txt
@CMakePresets.json
@vcpkg.json

<interfaces>
<!-- Canonical Qt 6 CMake block (D-01 + D-04, from 05-RESEARCH.md §Code Examples and 05-PATTERNS.md). -->

CMakeLists.txt — replace the current Qt 5 block at lines 291-311 with:

```
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

Also: rename the optional WebEngine block (`IF(USE_QTWEBENGINE)` ~line 314) from Qt5 → Qt6 (leave the option disabled).

CMakePresets.json — `macos-homebrew` preset:
  `CMAKE_PREFIX_PATH`: `/opt/homebrew/opt/qt@5;/opt/homebrew` → `/opt/homebrew/opt/qt;/opt/homebrew`

vcpkg.json — replace `"qt5-base"` with `"qtbase"`. All other ports unchanged.

vcpkg.json — verify `builtin-baseline` is present (40-char SHA pattern from Plan 04-04). If absent, add it using the current vcpkg HEAD SHA (`git ls-remote https://github.com/microsoft/vcpkg HEAD | awk '{print $1}'`).
</interfaces>
</context>

<tasks>

<task type="auto" tdd="false">
  <name>Task 1: Rewrite the CMakeLists.txt Qt block (Qt5 → Qt6 + Qt6::OpenGLWidgets)</name>
  <files>CMakeLists.txt</files>
  <read_first>
    - CMakeLists.txt lines 285-325 (the current Qt 5 FIND_PACKAGE block + the optional USE_QTWEBENGINE block) — must be read in one pass to see both the required Qt5 block AND the optional WebEngine Qt5 block at line ~314.
    - 05-PATTERNS.md §`CMakeLists.txt` lines 291–311 (analog — same block, Qt5→Qt6 rename) — contains the verbatim target Qt6 block.
    - 05-RESEARCH.md §CMakeLists.txt (lines 291–311) table for the line-by-line rename mapping.
  </read_first>
  <behavior>
    - QT_MIN_VERSION becomes `6.5` (D-01).
    - The two FIND_PACKAGE invocations target `Qt6` (not `Qt5`); the VIEW-gated one adds `OpenGLWidgets` to the REQUIRED component list (D-04).
    - `LIST(APPEND BALL_DEP_LIBRARIES ...)` and `LIST(APPEND VIEW_DEP_LIBRARIES ...)` use `Qt6::` prefixes; VIEW list gains `Qt6::OpenGLWidgets` as the last entry.
    - `LIST(APPEND BALL_PROJECT_COMPILE_DEFNS "-DQT_NO_KEYWORDS")` stays unchanged (already a list-append per Phase 04 lesson).
    - The optional `IF(USE_QTWEBENGINE)` block (~line 314) gets Qt5 → Qt6 renames too (leave disabled by default).
    - `Qt6LinguistTools` find stays soft (no REQUIRED) with the existing WARNING fallback message preserved/added.
  </behavior>
  <action>
    Rewrite the Qt block at CMakeLists.txt lines 291-311 verbatim per the canonical block in `<interfaces>` above (D-01, D-04). Additionally, locate the `IF(USE_QTWEBENGINE)` block near line 314 and rename `Qt5` → `Qt6` for both the FIND_PACKAGE and any `LIST(APPEND ...)` Qt5::WebEngine* lines (leave option default OFF — no scope creep).

    Use `LIST(APPEND ...)` for any new compile definitions or library targets — never `SET("... string")` (Phase 04-04 lesson — `BALL_PROJECT_COMPILE_DEFNS` is a CMake list; mixing styles produces ADD_DEFINITIONS garbage on MSVC). Do NOT introduce `Qt6::Core5Compat` — D-05 forbids the compat shim.
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball && cmake --preset ci-macos -S . 2>&amp;1 | tee /tmp/cmake-configure.log | tail -40 &amp;&amp; grep -q 'Qt6' /tmp/cmake-configure.log &amp;&amp; ! grep -qE 'Qt5::|FIND_PACKAGE\(Qt5' CMakeLists.txt</automated>
  </verify>
  <acceptance_criteria>
    - `grep -nE '^[^#]*FIND_PACKAGE\(Qt5' CMakeLists.txt` returns no lines (Qt5 FIND_PACKAGE removed; comment-only references allowed).
    - `grep -cE 'Qt6::OpenGLWidgets' CMakeLists.txt` is >= 1 (D-04 component added).
    - `grep -nE 'SET\(QT_MIN_VERSION 6\.5\)' CMakeLists.txt` returns a match (D-01).
    - `cmake --preset ci-macos -S .` configure step exits 0 on macOS with Homebrew qt 6.5+ installed; configure log contains `Found Qt6` and `Qt6::OpenGLWidgets`.
    - No `Qt6::Core5Compat` token anywhere in CMakeLists.txt (D-05 forbidden).
  </acceptance_criteria>
  <done>
    CMakeLists.txt declares Qt 6.5 floor, finds all required Qt6 components incl. OpenGLWidgets, appends them to BALL_DEP_LIBRARIES / VIEW_DEP_LIBRARIES using LIST(APPEND ...), and `cmake --preset ci-macos` configures green on the local macOS box.
  </done>
</task>

<task type="auto" tdd="false">
  <name>Task 2: Update CMakePresets.json (macOS Homebrew qt path) + vcpkg.json (qt5-base → qtbase)</name>
  <files>CMakePresets.json, vcpkg.json</files>
  <read_first>
    - CMakePresets.json lines 1-60 (read whole file in one pass — find the `macos-homebrew` preset block, lines ~18-26 per 05-PATTERNS.md).
    - vcpkg.json (whole file — short; the Phase 04 pattern includes `builtin-baseline` SHA).
    - 05-PATTERNS.md §`CMakePresets.json` and §`vcpkg.json` for the verbatim target diffs and the `builtin-baseline` requirement.
    - 05-RESEARCH.md §vcpkg Package Name Change for the qtbase port verification.
  </read_first>
  <behavior>
    - `macos-homebrew` preset `CMAKE_PREFIX_PATH` no longer references `qt@5` — uses Homebrew's unversioned `qt` formula (D-03; Phase 5 Pitfall 6 from RESEARCH).
    - `vcpkg.json` declares `"qtbase"` (not `"qt5-base"`) — the vcpkg Qt 6 port name (D-03, [VERIFIED: vcpkg.io/en/package/qtbase.html]).
    - `vcpkg.json` includes a valid 40-char `builtin-baseline` SHA (Phase 04-04 pattern; required for vcpkg manifest mode reproducibility).
    - No other preset / port lines change (no scope creep into other deps).
  </behavior>
  <action>
    In CMakePresets.json: change the `macos-homebrew` preset's `CMAKE_PREFIX_PATH` from `/opt/homebrew/opt/qt@5;/opt/homebrew` to `/opt/homebrew/opt/qt;/opt/homebrew`. The `ci-macos` preset inherits from `macos-homebrew` and picks up the change automatically — do not duplicate the path there.

    In vcpkg.json: change `"qt5-base"` to `"qtbase"`. If `builtin-baseline` is absent, add it as a top-level field with the current vcpkg HEAD SHA (obtained via `git ls-remote https://github.com/microsoft/vcpkg HEAD | awk '{print $1}'`). Do not modify any other dependency port names; do not add new vcpkg ports (no scope creep).

    Per D-05: do NOT add `qt5compat` or `qtwidgets-5compat` to the manifest — there is no Core5Compat shim.
  </action>
  <verify>
    <automated>grep -q '/opt/homebrew/opt/qt"' CMakePresets.json &amp;&amp; ! grep -qE '/opt/homebrew/opt/qt@5' CMakePresets.json &amp;&amp; grep -q '"qtbase"' vcpkg.json &amp;&amp; ! grep -q '"qt5-base"' vcpkg.json &amp;&amp; grep -qE '"builtin-baseline":[[:space:]]*"[0-9a-f]{40}"' vcpkg.json</automated>
  </verify>
  <acceptance_criteria>
    - `grep -c 'qt@5' CMakePresets.json` returns `0`.
    - `grep -c '/opt/homebrew/opt/qt"' CMakePresets.json` returns `>= 1`.
    - `grep -c '"qt5-base"' vcpkg.json` returns `0`.
    - `grep -c '"qtbase"' vcpkg.json` returns `1`.
    - `jq -r '."builtin-baseline"' vcpkg.json` prints a 40-char hex SHA (matches `^[0-9a-f]{40}$`).
    - `cmake --preset ci-macos -S .` still configures green (re-run of Task 1's command must remain green).
    - No `qt5compat` token in vcpkg.json (D-05).
  </acceptance_criteria>
  <done>
    macOS preset points at the Homebrew Qt 6 formula; Windows vcpkg manifest declares `qtbase` with a pinned `builtin-baseline`. macOS configure stays green; Windows configure will be exercised in Plan 04.
  </done>
</task>

</tasks>

<verification>
- `cmake --preset ci-macos -S .` exits 0 and logs `Found Qt6` + `Qt6::OpenGLWidgets`.
- `grep -nE 'Qt5::|FIND_PACKAGE\(Qt5' CMakeLists.txt` returns no non-comment lines.
- `grep -c qt@5 CMakePresets.json` returns 0.
- `grep -c '"qtbase"' vcpkg.json` returns 1; `grep -c '"qt5-base"' vcpkg.json` returns 0.
- `grep -E '"builtin-baseline":[[:space:]]*"[0-9a-f]{40}"' vcpkg.json` matches.
- No `Qt6::Core5Compat` or `qt5compat` token anywhere (D-05 audit).
</verification>

<success_criteria>
- Configure step (`cmake --preset ci-macos`) green on macOS Apple Silicon with Homebrew `qt` (Qt 6.5+).
- All Qt6 component targets (`Qt6::Core`, `Qt6::Network`, `Qt6::Xml`, `Qt6::OpenGL`, `Qt6::PrintSupport`, `Qt6::Test`, `Qt6::Widgets`, `Qt6::OpenGLWidgets`) declared and resolved.
- Windows manifest ready for Plan 04 CI exercise (`qtbase` + `builtin-baseline`).
- D-05 audit passes (no Core5Compat anywhere).
</success_criteria>

<output>
After completion, create `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-01-SUMMARY.md` capturing the configure log excerpt showing `Found Qt6` + the resolved Qt6 version, the vcpkg baseline SHA used, and any deviations from the planned changes.
</output>
