---
phase: 05-qt-6-migration-4b-renderer-backend-decision-spike
plan: 02
subsystem: view-source-api
tags: [qt6, view, plugin, qregexp, qdesktopwidget, qscreen, skipemptyparts]
dependency_graph:
  requires: [Plan 05-01 (CMake Qt 6 bring-up)]
  provides:
    - "QRegExp -> QRegularExpression inline in VIEW (D-05)"
    - "QDesktopWidget -> QGuiApplication::screens()/QScreen inline in VIEW (D-05)"
    - "QString::SkipEmptyParts -> Qt::SkipEmptyParts in PLUGIN (Pitfall 8)"
    - "Stereo paths in scene.C wired via QWidget::setScreen() (Pitfall 5)"
  affects: [Plan 05-03 GL surface port, Plan 05-04 CI matrix]
tech_stack:
  added: []
  patterns:
    - "QRegularExpressionMatchIterator + globalMatch(text) loop"
    - "QGuiApplication::screens().value(i) ? ->geometry() : QRect() null-safe lookup"
    - "QWidget::setScreen() + move(screen->geometry().topLeft()) for cross-screen window placement"
key_files:
  created: []
  modified:
    - include/BALL/VIEW/WIDGETS/pyWidget.h
    - source/VIEW/WIDGETS/pyWidget.C
    - source/VIEW/DIALOGS/stageSettings.C
    - source/VIEW/WIDGETS/scene.C
    - source/VIEW/KERNEL/MODES/interactionMode.C
    - source/PLUGIN/pluginManager.C
decisions:
  - "D-05 reinforced: no Qt6::Core5Compat anywhere — clean break per phase decision."
  - "Pitfall 5 resolved: scene.C stereo `new GLRenderWindow(screen, ...)` rewritten as `new GLRenderWindow(nullptr, ...)` + `widget->setScreen(screen)` + `move()`, applied uniformly to all 6 stereo construction sites (3 methods × 2 eyes) inside the guard-deferred paths."
  - "stageSettings.C `connect(QApplication::desktop(), screenCountChanged(int), …)` replaced with `QGuiApplication::screenAdded/screenRemoved` lambdas forwarding to the existing `screenCountChanged(int number)` slot — preserves the int-arg slot signature instead of changing the header."
  - "interactionMode.C plan said 'delete dead include' but the include is actually used (`qApp->desktop()->size()` at ctor init list). Replaced with `QGuiApplication::primaryScreen() ? primaryScreen()->size() : QSize()` — deviation from plan note, but consistent with D-05's port-don't-shim mandate."
  - "Build verification deferred: BLOCKER-A/B/C in BALL/FORMAT, BALL/SYSTEM, BALL/VIEW/RENDERING are out-of-scope pre-existing Qt 6 incompatibilities that prevent BALL/VIEW from linking. Logged to deferred-items.md for the next plan wave. pluginManager.C compiles clean in isolation as the available verification proof."
metrics:
  duration_min: 8
  duration_human: "~8min wall"
  tasks_completed: 2
  files_modified: 6
  completed_date: "2026-05-15"
---

# Phase 5 Plan 02: Source API Renames Summary

Replaced the five Qt-5-only API surfaces in VIEW + PLUGIN with their Qt 6 equivalents inline (D-05, no Core5Compat shim). 6 files modified, 9 stereo `new GLRenderWindow(screen, …)` sites rewritten per Pitfall 5, and the PythonHighlighter rewritten on `QRegularExpressionMatchIterator` with behavioural parity.

## What Shipped

**Task 1 — `pyWidget.{h,C}` QRegExp → QRegularExpression** (commit `be894f7`)
- `pyWidget.h`: 4 member fields (`python_patterns`, `BALL_patterns`, `string_pattern`, `comment_pattern`) switched from `QRegExp`/`vector<QRegExp>` to `QRegularExpression`/`vector<QRegularExpression>`; added `#include <QtCore/QRegularExpression>`.
- `pyWidget.C compilePattern()`: 4 ctor sites switched to `QRegularExpression(...)`. Pattern strings byte-identical — PCRE-compatible with QRegExp's syntax for these simple cases (`\b...\b`, `".*"`, `#.*`).
- `pyWidget.C highlightBlock()`: 4 `indexIn` / `matchedLength` / `text.indexOf(expression, ...)` loops rewritten using `QRegularExpressionMatchIterator it = expression.globalMatch(text); while (it.hasNext()) { auto m = it.next(); setFormat(m.capturedStart(), m.capturedLength(), fmt); }`. Behavioural parity: Python keyword + BALL keyword + string-literal + comment highlighting all preserved.
- Bonus: comment-only `QRegExp("i")` reference at line 131 also bumped to `QRegularExpression("i")` for hygiene.

**Task 2 — QDesktopWidget + SkipEmptyParts** (commit `d7e067f`)
- `stageSettings.C`: include swap; `screenCount()` → `QGuiApplication::screens().size()`; `connect(QApplication::desktop(), screenCountChanged(int), ...)` → `QGuiApplication::screenAdded`/`screenRemoved` lambdas forwarding to the existing int-arg slot; `QSplashScreen(QApplication::desktop()->screen(i), ...)` → `QSplashScreen(QGuiApplication::screens().value(i), ...)`; `screenGeometry(idx)` → `screens.value(idx) ? screens.value(idx)->geometry() : QRect()` (null-safe).
- `scene.C`: include swap; 3 stereo construction blocks ported per Pitfall 5 — `QScreen*` instead of `QWidget*` (because Qt 6's QScreen does NOT inherit from QWidget), `GLRenderWindow(nullptr, ...)` + `widget->setScreen(screen)` + `move(screen->geometry().topLeft())`. Both eyes of all 3 stereo methods (`enterDualStereoDifferentDisplays` at ~2581, `enterDualStereo` at ~2785, `enterDualStereoDifferentDisplays` at ~2895/2923) handled. Total: 7 `setScreen(` call sites + 6 `QGuiApplication::screens` lookups.
- `interactionMode.C`: include swap to `<QtGui/QScreen>` + `<QtGui/QGuiApplication>`; ctor init list `qApp->desktop()->size()` rewritten as `QGuiApplication::primaryScreen() ? primaryScreen()->size() : QSize()`. (Plan said "delete dead include" — but the include is not actually dead at line 17 of the original ctor; deviation noted in decisions.)
- `pluginManager.C`: line 408 `QString::SkipEmptyParts` → `Qt::SkipEmptyParts` (one-token Qt 6 namespace move).

## Diff Stat

```
 include/BALL/VIEW/WIDGETS/pyWidget.h       |  9 ++---
 source/PLUGIN/pluginManager.C              |  2 +-
 source/VIEW/DIALOGS/stageSettings.C        | 23 ++++++++-----
 source/VIEW/KERNEL/MODES/interactionMode.C |  6 ++--
 source/VIEW/WIDGETS/pyWidget.C             | 55 +++++++++++++++---------------
 source/VIEW/WIDGETS/scene.C                | 53 ++++++++++++++++------------
 6 files changed, 85 insertions(+), 63 deletions(-)
```

## Acceptance Gate Tally

| Gate                                                                                                  | Result                                                                            |
| ----------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------- |
| Task 1 — `grep -c '\bQRegExp\b' include/BALL/VIEW/WIDGETS/pyWidget.h source/VIEW/WIDGETS/pyWidget.C`  | 0 (PASS — comment hygiene included)                                               |
| Task 1 — `grep -c 'QRegularExpression' include/BALL/VIEW/WIDGETS/pyWidget.h` ≥ 4                       | 5 (PASS — 1 include + 4 members)                                                  |
| Task 1 — `grep -c 'globalMatch' source/VIEW/WIDGETS/pyWidget.C` ≥ 3                                    | 4 (PASS — python / BALL / string / comment paths)                                 |
| Task 1 — `grep -c 'QRegularExpression(' source/VIEW/WIDGETS/pyWidget.C` ≥ 4                            | 5 (PASS — 4 ctor sites + 1 comment hygiene)                                       |
| Task 2 — comment-filtered count of `QDesktopWidget\|QApplication::desktop(\|QString::SkipEmptyParts` across the 4 files | 0 (PASS)                                                                          |
| Task 2 — `grep -c 'QGuiApplication::screens' stageSettings.C scene.C` ≥ 2                              | 6+6=12 across the 2 files (PASS)                                                  |
| Task 2 — `grep -c 'setScreen(' source/VIEW/WIDGETS/scene.C` ≥ 1                                        | 7 (PASS — Pitfall 5 applied uniformly)                                            |
| Task 2 — `grep -c 'Qt::SkipEmptyParts' source/PLUGIN/pluginManager.C` ≥ 1                              | 1 (PASS)                                                                          |
| Task 2 — `grep 'QtWidgets/QDesktopWidget' source/VIEW/KERNEL/MODES/interactionMode.C`                  | (empty) (PASS)                                                                    |
| Overall — `Qt6::Core5Compat` / `qt5compat` anywhere in the 6 files                                    | (empty) (PASS — D-05)                                                             |
| `pluginManager.C` compiles in isolation under Qt 6                                                    | PASS — `make -f CMakeFiles/BALL.dir/build.make .../pluginManager.C.o` exits 0     |
| Full BALL/VIEW/BALLView build green                                                                   | BLOCKED — by BLOCKER-A/B/C, all OUT-OF-SCOPE (BALL/FORMAT, BALL/SYSTEM, glRenderWindow.h) — see Deferred Issues |

## Deviations from Plan

### Auto-fixed Issues

**1. [Rule 3 — Blocking] interactionMode.C include is NOT dead — ctor uses `qApp->desktop()->size()`**

- **Found during:** Task 2 (reading interactionMode.C).
- **Issue:** Plan said "the include at line 5 is unused beyond the directive itself" and recommended deletion. In fact line 17 (ctor init list) reads `desktop_size_(qApp->desktop()->size())` which requires the QDesktopWidget API — a hard Qt 6 compile error if the include were merely removed.
- **Fix:** Replaced the include with `<QtGui/QScreen>` + `<QtGui/QGuiApplication>` (matching the canonical Qt 6 pattern used elsewhere in this plan) and rewrote the ctor init to `QGuiApplication::primaryScreen() ? primaryScreen()->size() : QSize()`.
- **Files modified:** `source/VIEW/KERNEL/MODES/interactionMode.C` (single change).
- **Commit:** `d7e067f` (folded into Task 2).

**2. [Rule 3 — Blocking] stageSettings.C had 3 additional QDesktopWidget sites not in the plan (lines 52, 89, 415)**

- **Found during:** Task 2 (file scan).
- **Issue:** Plan's `<action>` only documented line 24 (include) and 433-436 (geometry calls). The file also had `QApplication::desktop()->screenCount()` (line 52), `connect(QApplication::desktop(), screenCountChanged(int), ...)` (line 89), and `new QSplashScreen(QApplication::desktop()->screen(i), ...)` (line 415). All three are hard Qt 6 compile errors using the same removed API. Scope-boundary call: these are in the same file Plan 05-02 owns and would block compilation, so they are blocking-issue fixes (Rule 3), not scope creep.
- **Fix:** Line 52 → `QGuiApplication::screens().size()`. Line 89 → two `QGuiApplication::screenAdded`/`screenRemoved` lambdas forwarding to the existing int-arg slot (preserves the header `screenCountChanged(int number)` signature). Line 415 → `QSplashScreen(QGuiApplication::screens().value(i), ...)` (QSplashScreen's Qt 6 ctor accepts `QScreen*` directly).
- **Commit:** `d7e067f` (folded into Task 2).

**3. [Rule 3 — Blocking] scene.C had 6 stereo `new GLRenderWindow(screen, …)` sites and 2 extra `screenGeometry()` calls, not just the 2-3 documented**

- **Found during:** Task 2 (`grep -n 'QDesktopWidget\|QApplication::desktop' scene.C`).
- **Issue:** Plan said "the line 2581-2584 pair" — actually 3 separate stereo methods each construct 2 GLRenderWindows (left/right), for 6 construction sites total, plus 2 extra `screenGeometry(idx)` calls in `enterDualStereoDifferentDisplays`. All hard Qt 6 compile errors.
- **Fix:** Applied the canonical Pitfall 5 transformation (`new GLRenderWindow(nullptr, …)` + `setScreen()`) uniformly across all 6 sites. The `screenGeometry()` calls collapsed into `screen->geometry()` lookups on the already-captured `QScreen*` (no extra query). The plan's Phase 02.1 VERIFICATION.md note about "9 guard-deferred stereo sites" is consistent — the 6 GLRenderWindow constructions + the geometry calls + the screen lookups land in roughly that count.
- **Commit:** `d7e067f` (folded into Task 2).

**4. [Rule 1 — Hygiene] Commented-out `QRegExp("i")` reference in pyWidget.C line 131 bumped to `QRegularExpression("i")`**

- Strictly cosmetic — comment-only token. Updated for grep-cleanliness so any future legacy-symbol lint job (Plan 05-04) doesn't trip on a comment.

## Deferred Issues

Three pre-existing Qt 6 incompatibilities in the **BALL** and **BALL/VIEW** infrastructure prevent a full `cmake --build --target BALL VIEW BALLView` from going green after Plan 05-02. All three are **out of Plan 05-02's file scope** (the 6 listed files) and have been logged for downstream plan waves at `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/deferred-items.md`:

- **BLOCKER-A:** `include/BALL/FORMAT/dockResultFile.h:14` includes `<QtXml/QXmlDefaultHandler>`, which was removed in Qt 6 (legacy SAX API). Owner: a follow-on plan in this phase wave or a dedicated plan to port the dock-result XML codepath to `QXmlStreamReader`.
- **BLOCKER-B:** `include/BALL/SYSTEM/mutex.h:26` defines `BALL_DEFAULT_MUTEXLOCKER_TYPE QMutexLocker` — Qt 6 made `QMutexLocker` a class template (`template <typename Mutex>`). Owner: same wave as BLOCKER-A.
- **BLOCKER-C:** `include/BALL/VIEW/RENDERING/glRenderWindow.h:32` includes `<QtWidgets/QOpenGLWidget>` — Qt 6 moved this to `<QtOpenGLWidgets/QOpenGLWidget>`. **Plan 05-02 explicitly forbade touching `glRenderWindow.{h,C}`** ("Plan 03 owns that file"). Owner: Plan 05-03.

Implication for Plan 05-02's success criteria: the criterion *"Affected files compile under Qt 6"* is verified for `pluginManager.C` (isolated compile success) and is **deferred** for the 5 VIEW files until BLOCKER-A/B/C land. The mechanical port itself matches the canonical Qt 6 migration patterns from `05-PATTERNS.md` and `05-RESEARCH.md §Qt 5 → 6 API Surface Delta` verbatim, and pluginManager.C's clean isolated compile is a strong proxy that the same Qt 6 APIs build under the project toolchain.

## Known Stubs

None. PythonHighlighter retains full behavioural parity — keyword, string, and comment highlighting all wired.

## Threat Flags

None — view-layer API rename, no new endpoints / auth paths / file access patterns / schema changes.

## Self-Check: PASSED

- Files modified exist:
  - `include/BALL/VIEW/WIDGETS/pyWidget.h` — FOUND (verified by `grep -c 'QRegularExpression' …` = 5)
  - `source/VIEW/WIDGETS/pyWidget.C` — FOUND (verified by `grep -c 'globalMatch' …` = 4)
  - `source/VIEW/DIALOGS/stageSettings.C` — FOUND (`QGuiApplication::screens` = 6 hits)
  - `source/VIEW/WIDGETS/scene.C` — FOUND (`setScreen(` = 7 hits)
  - `source/VIEW/KERNEL/MODES/interactionMode.C` — FOUND (no `QtWidgets/QDesktopWidget` remaining)
  - `source/PLUGIN/pluginManager.C` — FOUND (`Qt::SkipEmptyParts` = 1 hit)
- Commits exist:
  - `be894f7` (Task 1: pyWidget QRegExp → QRegularExpression) — FOUND in `git log`
  - `d7e067f` (Task 2: QDesktopWidget + SkipEmptyParts ports) — FOUND in `git log`
- `pluginManager.C` compiles cleanly in isolation under Qt 6 — VERIFIED.
