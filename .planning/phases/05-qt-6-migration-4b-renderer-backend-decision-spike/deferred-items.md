# Phase 05 Deferred Items

## Found During Plan 05-02 (source API renames)

### BLOCKER-A: `QtXml/QXmlDefaultHandler` removed in Qt 6 — DEFERRED-STUB 2026-05-15 (Plan 05-05)

- **Found:** Plan 05-02 build verification
- **File:** `include/BALL/FORMAT/dockResultFile.h:14`
- **Error:** `fatal error: 'QtXml/QXmlDefaultHandler' file not found`
- **Cause:** Qt 6 dropped the legacy QtXml SAX API (`QXmlDefaultHandler`, `QXmlContentHandler`, etc.). The Qt 5 SAX classes moved to Qt5Compat / were removed.
- **Scope decision:** OUT OF SCOPE for Plan 05-02 — Plan 05-02's `<files_modified>` is the 6 VIEW + PLUGIN files; `include/BALL/FORMAT/dockResultFile.h` is in the BALL FORMAT layer.
- **Recommendation:** Folds naturally into Plan 05-03 (the GL-side compile bring-up wave) or a new dedicated plan. Options: (a) port to `QXmlStreamReader` (Qt 6 native), (b) gate the dock-result XML codepath off and stub the API, (c) re-introduce `Qt6::Core5Compat` for this one file (would violate D-05).
- **Resolution (Plan 05-05, commit `3691232`):** Adopted option (b) — temporarily stubbed the dead `QXmlAttributes` overload of `DockResultFile::attributesToHashMap()` under a `#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)` guard. All BALL readers in `source/FORMAT/dockResultFile.C` go through `QXmlStreamReader::attributes()` (returns `QXmlStreamAttributes`); the SAX overload is dead code under Qt 6, so stubbing it is behaviourally safe. Full port to `QXmlStreamReader` is **deferred** to a future cleanup task (label suggestion: BLOCKER-A2 / Phase 5.1 candidate).

### BLOCKER-B: `QMutexLocker` is now a template in Qt 6 — RESOLVED 2026-05-15 (Plan 05-05)

- **Found:** Plan 05-02 build verification (Task 2 stageSettings.C compile attempt)
- **File:** `include/BALL/SYSTEM/mutex.h:26` — `#define BALL_DEFAULT_MUTEXLOCKER_TYPE QMutexLocker`
- **Error:** `use of class template 'QMutexLocker' requires template arguments` — Qt 6 made `QMutexLocker` a class template (`template <typename Mutex> class QMutexLocker`).
- **Scope decision:** OUT OF SCOPE for Plan 05-02. This is in BALL/SYSTEM (the system layer), not in the 6 VIEW + PLUGIN files Plan 05-02 owns. The `mutex.h` cascade is large — there's a `TMutexLocker<>` wrapper template that aliases through.
- **Resolution (Plan 05-05, commit `204de36`):** Plumb the explicit `QMutex` template argument through `BALL_DEFAULT_MUTEXLOCKER_TYPE QMutexLocker<QMutex>`. Plus: Qt 6 also removed `QMutex::Recursive` / `QMutex::NonRecursive` constants (recursive mutexes are a separate `QRecursiveMutex` class now), so the `TMutex<QMutex>` specialisation was simplified to construct a plain `QMutex()` unconditionally (the BALL hot path uses non-recursive). `QReadWriteLock::Recursive` / `NonRecursive` still exist in Qt 6 — preserved.

### BLOCKER-C: `QtWidgets/QOpenGLWidget` header moved in Qt 6 — RESOLVED 2026-05-15 (Plan 05-03)

- **Found:** Plan 05-02 build verification
- **File:** `include/BALL/VIEW/RENDERING/glRenderWindow.h:32` — `#include <QtWidgets/QOpenGLWidget>`
- **Error:** `'QtWidgets/QOpenGLWidget' file not found` — Qt 6 moved `QOpenGLWidget` from `QtWidgets` to `QtOpenGLWidgets` (Pitfall 4 in 05-RESEARCH.md).
- **Scope decision:** OUT OF SCOPE — the plan explicitly says "Do NOT modify glRenderWindow.{h,C} here — Plan 03 owns that file."
- **Recommendation:** Plan 05-03 (or whichever plan owns `glRenderWindow.{h,C}`).
- **Resolution:** Plan 05-03 Task 1, commit `07c32bb` — `#include <QtOpenGLWidgets/QOpenGLWidget>`. Verified by isolated compile of `source/VIEW/RENDERING/glRenderWindow.C` (no `'QtWidgets/QOpenGLWidget' file not found` error).

## Found During Plan 05-03 (QSurfaceFormat compat profile + QtOpenGLWidgets)

### BLOCKER-D: `Qt::WindowFlags w_flags = 0` no longer accepts implicit int conversion in Qt 6 — RESOLVED (widened) 2026-05-15 (Plan 05-05)

- **Found:** Plan 05-03 build verification (isolated compile of `glRenderWindow.C` after Task 1).
- **File:** `include/BALL/VIEW/RENDERING/glRenderWindow.h:51-52` — `Qt::WindowFlags w_flags = 0` in the two constructors that take a `QWidget*` parent.
- **Error:** `no viable conversion from 'int' to 'Qt::WindowFlags' (aka 'QFlags<WindowType>')` — Qt 6 made `QFlags::QFlags(int)` no longer implicit. Canonical fix: `Qt::WindowFlags w_flags = Qt::WindowFlags()` or `= {}` (C++17 brace-init).
- **Scope decision:** OUT OF SCOPE for Plan 05-03 — Task 1 `<action>` says verbatim: "Do not modify any other line in this header." Lines 51-52 are unrelated to BLOCKER-C / Pitfall 4 / D-06; the Qt::WindowFlags issue is a separate Qt 6 stricter-QFlags change.
- **Resolution (Plan 05-05, commit `d33f58d`):** Surveyed the codebase and found **28 affected default-arg sites**, not the originally-scoped 2 — every dialog/widget constructor across `include/BALL/VIEW/DIALOGS/`, `include/BALL/VIEW/WIDGETS/`, and `source/EXTENSIONS/{BALLAXY,JUPYTER,PRESENTABALL}/include/` carried the same `Qt::WindowFlags w_flags = 0` legacy idiom. All 28 sites uniformly rewritten as `= Qt::WindowFlags()` via a perl one-liner. Binary-compatible: every caller in `scene.C` already passes an explicit flag value, so the default-arg change does not affect any existing call.

## Found During Plan 05-05 (GL-core spike + Qt 6 link bring-up)

### BLOCKER-A2: Full QtXml SAX → QXmlStreamReader port for DockResultFile (deferred)

- **Found:** Plan 05-05 during BLOCKER-A resolution.
- **Status:** **DEFERRED**. The Qt 5 SAX overload of `DockResultFile::attributesToHashMap(const QXmlAttributes&)` is stubbed under `#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)` (commit `3691232`). All BALL readers already use the modern `QXmlStreamReader` path, so the stubbed overload is dead code — but a future cleanup pass should formally retire the `QXmlAttributes` parameter from the header signature and clean up the include comment.
- **Suggested home:** Phase 5.1 build-warnings/latent-bugs cleanup (already in `.planning/phases/05.1-build-warnings-and-latent-bugs/05.1-BACKLOG.md` scope).
- **Risk:** Low — the stub is in dead code, and a no-op overload kept under an ifdef will not regress Qt 6 builds. The only real downside is a faint cognitive load on future readers.

### BLOCKER-E: Qt 6 API surface sweep beyond original BLOCKER-A/B/D scope

- **Found:** Plan 05-05 BALLView build verification (cascade of compile errors after BLOCKER-B and BLOCKER-D landed).
- **Status:** **RESOLVED** in commit `a0c28bc` — Qt 6 API sweep covering ~14 distinct API removals across ~37 files (QString::null, Qt::MidButton, QtWidgets/QAction header path, QtWidgets/QActionGroup, QtWidgets/QShortcut, QtGui/QOpenGLFramebufferObject → QtOpenGL, QtWebEngineWidgets/QWebEnginePage → QtWebEngineCore, QTableWidgetItem::setBackgroundColor/backgroundColor, QList/Tree::setItem{Selected,Expanded}, QWheelEvent::delta()/pos(), QString::sprintf, QFontMetrics::width(QChar), QLineF::intersect, QPalette::foreground/background, QStyleOption::init, QApplication::globalStrut, QLayout::setMargin, QPainter::setRedirected, qVariantFromValue, QSpontaneKeyEvent::setSpontaneous static-ification, HTMLPage::certificateError signal-conversion, rotateMode.C QFlags-vs-Modifier ambiguous operator, labelDialog.ui QComboBox::autoCompletion property, downloadElectronDensity.C QFile incomplete-type include). All mechanical, source-compatible, no behaviour change in production paths. See Plan 05-05 SUMMARY §"Deviations from Plan" §"Qt 6 API surface sweep beyond original BLOCKER scope".
- **Carry-forward:** None — the sweep is complete as of `a0c28bc`. Future Qt 6 minor-version bumps may surface additional deprecation warnings but no further removals are anticipated within the 6.5–6.11 range.
