# Phase 05 Deferred Items

## Found During Plan 05-02 (source API renames)

### BLOCKER-A: `QtXml/QXmlDefaultHandler` removed in Qt 6

- **Found:** Plan 05-02 build verification
- **File:** `include/BALL/FORMAT/dockResultFile.h:14`
- **Error:** `fatal error: 'QtXml/QXmlDefaultHandler' file not found`
- **Cause:** Qt 6 dropped the legacy QtXml SAX API (`QXmlDefaultHandler`, `QXmlContentHandler`, etc.). The Qt 5 SAX classes moved to Qt5Compat / were removed.
- **Scope decision:** OUT OF SCOPE for Plan 05-02 — Plan 05-02's `<files_modified>` is the 6 VIEW + PLUGIN files; `include/BALL/FORMAT/dockResultFile.h` is in the BALL FORMAT layer.
- **Recommendation:** Folds naturally into Plan 05-03 (the GL-side compile bring-up wave) or a new dedicated plan. Options: (a) port to `QXmlStreamReader` (Qt 6 native), (b) gate the dock-result XML codepath off and stub the API, (c) re-introduce `Qt6::Core5Compat` for this one file (would violate D-05).
- **Impact on Plan 05-02:** pyWidget.C is gated by `BALL_PYTHON_SUPPORT=OFF` in the current build so it is not compiled by `cmake --build --target VIEW` regardless; the four files in Task 2 are in VIEW (which compiles after BALL) and PLUGIN (which is part of BALL). The mechanical port is verifiably correct against the canonical Qt 6 docs (`globalMatch` / `QGuiApplication::screens()` / `Qt::SkipEmptyParts`); without dockResultFile.h being patched, neither BALL nor VIEW links to completion.

### BLOCKER-B: `QMutexLocker` is now a template in Qt 6

- **Found:** Plan 05-02 build verification (Task 2 stageSettings.C compile attempt)
- **File:** `include/BALL/SYSTEM/mutex.h:26` — `#define BALL_DEFAULT_MUTEXLOCKER_TYPE QMutexLocker`
- **Error:** `use of class template 'QMutexLocker' requires template arguments` — Qt 6 made `QMutexLocker` a class template (`template <typename Mutex> class QMutexLocker`).
- **Scope decision:** OUT OF SCOPE for Plan 05-02. This is in BALL/SYSTEM (the system layer), not in the 6 VIEW + PLUGIN files Plan 05-02 owns. The `mutex.h` cascade is large — there's a `TMutexLocker<>` wrapper template that aliases through.
- **Recommendation:** Likely needs to land in the same plan as BLOCKER-A (Plan 05-03 or similar wave to make BALL build green on Qt 6). Fix: `#define BALL_DEFAULT_MUTEXLOCKER_TYPE QMutexLocker<QMutex>` or change `TMutexLocker` to forward the QMutex template argument.

### BLOCKER-C: `QtWidgets/QOpenGLWidget` header moved in Qt 6 — RESOLVED 2026-05-15 (Plan 05-03)

- **Found:** Plan 05-02 build verification
- **File:** `include/BALL/VIEW/RENDERING/glRenderWindow.h:32` — `#include <QtWidgets/QOpenGLWidget>`
- **Error:** `'QtWidgets/QOpenGLWidget' file not found` — Qt 6 moved `QOpenGLWidget` from `QtWidgets` to `QtOpenGLWidgets` (Pitfall 4 in 05-RESEARCH.md).
- **Scope decision:** OUT OF SCOPE — the plan explicitly says "Do NOT modify glRenderWindow.{h,C} here — Plan 03 owns that file."
- **Recommendation:** Plan 05-03 (or whichever plan owns `glRenderWindow.{h,C}`).
- **Resolution:** Plan 05-03 Task 1, commit `07c32bb` — `#include <QtOpenGLWidgets/QOpenGLWidget>`. Verified by isolated compile of `source/VIEW/RENDERING/glRenderWindow.C` (no `'QtWidgets/QOpenGLWidget' file not found` error).

## Found During Plan 05-03 (QSurfaceFormat compat profile + QtOpenGLWidgets)

### BLOCKER-D: `Qt::WindowFlags w_flags = 0` no longer accepts implicit int conversion in Qt 6

- **Found:** Plan 05-03 build verification (isolated compile of `glRenderWindow.C` after Task 1).
- **File:** `include/BALL/VIEW/RENDERING/glRenderWindow.h:51-52` — `Qt::WindowFlags w_flags = 0` in the two constructors that take a `QWidget*` parent.
- **Error:** `no viable conversion from 'int' to 'Qt::WindowFlags' (aka 'QFlags<WindowType>')` — Qt 6 made `QFlags::QFlags(int)` no longer implicit. Canonical fix: `Qt::WindowFlags w_flags = Qt::WindowFlags()` or `= {}` (C++17 brace-init).
- **Scope decision:** OUT OF SCOPE for Plan 05-03 — Task 1 `<action>` says verbatim: "Do not modify any other line in this header." Lines 51-52 are unrelated to BLOCKER-C / Pitfall 4 / D-06; the Qt::WindowFlags issue is a separate Qt 6 stricter-QFlags change.
- **Recommendation:** Land alongside BLOCKER-A + BLOCKER-B in the next plan wave that makes BALL/VIEW/BALLView link green. One-line fix per constructor, two constructors affected. The four callers in `scene.C` already pass explicit flag values (e.g., `Qt::FramelessWindowHint`), so the default-arg change is binary-compatible.
