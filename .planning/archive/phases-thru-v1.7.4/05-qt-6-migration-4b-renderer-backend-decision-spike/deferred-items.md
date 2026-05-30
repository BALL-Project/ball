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

## Found During Plan 05-07 (per-platform driver behaviour record)

Plan 05-07 authored `.planning/phases/05-.../05-DRIVER-BEHAVIOUR.md` as a partial deliverable per its `<Reasonable-Call Justification>` section. Five empirical gaps were enumerated in the artifact's §Empirical Gaps Carried Forward; each has a corresponding seed in `.planning/seeds/` if external action is required to resolve it. Summary index:

### GAP-1: Linux default-build smoke under Qt 6.5+ via aqtinstall — DEFERRED (seed planted)

- **Found:** Plan 05-07 CI investigation (run 25910030073).
- **Symptom:** `jurplel/install-qt-action@v4` step fails — `ERROR : The packages ['qtbase', 'qtopengl', 'qttools'] were not found while parsing XML of package information!`. aqt-3.3 module-name set differs from the originally-configured values.
- **Suggested home:** Phase 5.1 build-warnings/latent-bugs cleanup OR dedicated 1-task CI hotfix.
- **Seed:** `.planning/seeds/SEED-005-1-ci-aqtinstall-modules-fixup.md` (status: dormant, scope: Small, ~1 yaml line).
- **Resolves:** Plan 05-07 §Gap 1 — Linux row of driver-behaviour table moves from GAP to ✓ once landed.

### GAP-2: Windows vcpkg baseline commit not in runner image — DEFERRED (seed planted)

- **Found:** Plan 05-07 CI investigation (run 25910030073).
- **Symptom:** `Configure (Windows)` step fails — `error: while checking out baseline from commit 'c1ce926ddf75f166cbdd444bbaf463410ac99b17', failed to git show versions/baseline.json. fatal: path 'versions/baseline.json' exists on disk, but not in 'c1ce926ddf75f166cbdd444bbaf463410ac99b17'`. The runner-image vcpkg checkout has rolled past the pinned baseline.
- **Suggested home:** Phase 5.1 OR dedicated 1-task CI hotfix.
- **Seed:** `.planning/seeds/SEED-005-2-ci-vcpkg-baseline-fixup.md` (status: dormant, scope: Small, options: re-pin baseline OR add `git fetch` step).
- **Resolves:** Plan 05-07 §Gap 2 — Windows row moves from GAP to build-green (driver capture still gated on GAP-3).

### GAP-3: Windows driver-behaviour capture (hardware/VM access) — DEFERRED (seed planted)

- **Found:** Plan 05-07 — no Windows hardware available this session; CI Windows path also blocked by GAP-2.
- **Symptom:** No live `BALLVIEW_GL_DIAG` from Windows; ANGLE-removal cannot be empirically verified; Windows picking + overlay validation unverified.
- **Suggested home:** Phase 5.1 (Path A: CI artifact upload step) OR PIPE-01 opening (Path B: VM) OR opportunistic (Path C: user's Windows box).
- **Seed:** `.planning/seeds/SEED-005-3-windows-vm-driver-capture.md` (status: dormant, scope: Medium).
- **Resolves:** Plan 05-07 §Gap 3 — Windows row of driver-behaviour table moves from GAP to ✓.
- **Dependency:** Path A depends on GAP-2 (SEED-005-2) being resolved first.

### GAP-4: Post-PIPE-01 spike full-render BALLVIEW_GL_DIAG re-capture — DEFERRED (seed planted)

- **Found:** Plans 05-05 + 05-06 SPIKE-01 runtime smoke evidence (re-cited in 05-07).
- **Symptom:** Both spike paths (GL-core, QRhi) construct correctly through the factory env-var gate but crash before `initializeGL()` due to downstream `RenderSetup` invoking `GLRenderer`-specific virtuals not in the bare-bones spike renderers. No `BALLVIEW_GL_DIAG renderer_backend=Core/QRhi` line is capturable today.
- **Status:** Expected throwaway-spike limitation — SPIKE-01 deliverable bar is "constructs + factory-wires + correct API shape + caveats documented", not pixel-perfect parity. **PIPE-01 scope**, not a SPIKE-01 deliverable miss.
- **Suggested home:** PIPE-01 phase Wave 0 (after pipeline rewrite functional, before phase closes).
- **Seed:** `.planning/seeds/SEED-005-4-post-pipe01-spike-recapture.md` (status: dormant, scope: Small, triggers when PIPE-01 begins).
- **Resolves:** Plan 05-07 §Gap 4 — driver-behaviour record for the chosen post-Phase-5 renderer captured in PIPE-01's own phase dir; the Plan 05-07 artifact is forward-annotated.

### GAP-5: D-07 macOS deprecation banner observability under Qt 6 — EMPIRICAL FINDING DOCUMENTED

- **Found:** Plan 05-07 local macOS-arm64 / Qt 6.11 / macOS 26.4.1 BALLView runs.
- **Symptom:** No GL-deprecation banner emitted to stdout or stderr. Apple's canonical deprecation text is documented (cited from developer docs) but is NOT runtime-observable in the standard CLI path under Qt 6's `QOpenGLFunctions_*` wrappers.
- **Status:** Documented empirical finding. **No code change required** — D-07's framing in the SPIKE-02 decision record (Plan 08) should be adjusted: long-term API risk is real (Apple's policy: GL will be removed), day-to-day runtime noise is not present in Qt 6 + Apple Silicon.
- **Suggested home:** Plan 08 (SPIKE-02 decision record) incorporates the finding directly. Phase 5.1 may optionally add a `log stream` based capture if the runtime-observability framing is reinstated.
- **No seed required** — the finding is captured in `.planning/phases/05-.../05-DRIVER-BEHAVIOUR.md` §D-07.
