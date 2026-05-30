---
phase: 05-qt-6-migration-4b-renderer-backend-decision-spike
plan: 02
type: execute
wave: 2
depends_on: [01]
files_modified:
  - include/BALL/VIEW/WIDGETS/pyWidget.h
  - source/VIEW/WIDGETS/pyWidget.C
  - source/VIEW/DIALOGS/stageSettings.C
  - source/VIEW/WIDGETS/scene.C
  - source/VIEW/KERNEL/MODES/interactionMode.C
  - source/PLUGIN/pluginManager.C
autonomous: true
requirements: [QT6-02]
must_haves:
  truths:
    - "D-05: QRegExp is gone — replaced inline with QRegularExpression, no Qt6::Core5Compat shim"
    - "D-05: QDesktopWidget is gone — replaced inline with QGuiApplication::screens() / QScreen"
    - "pluginManager.C QString::SkipEmptyParts replaced with Qt::SkipEmptyParts (Qt 6 namespace move)"
    - "scene.C stereo paths compile under Qt 6 — QScreen* is wired via QWidget::setScreen() (Pitfall 5), not as a QWidget* parent"
    - "PythonHighlighter still highlights python + BALL keywords + strings + comments (behavioural parity with Qt 5)"
  artifacts:
    - path: "include/BALL/VIEW/WIDGETS/pyWidget.h"
      provides: "QRegularExpression member declarations"
      contains: "QRegularExpression"
    - path: "source/VIEW/WIDGETS/pyWidget.C"
      provides: "PythonHighlighter using QRegularExpressionMatchIterator"
      contains: "globalMatch"
    - path: "source/VIEW/DIALOGS/stageSettings.C"
      provides: "Multi-screen geometry via QGuiApplication::screens()"
      contains: "QGuiApplication::screens()"
    - path: "source/VIEW/WIDGETS/scene.C"
      provides: "Stereo window placement via QWidget::setScreen()"
      contains: "setScreen("
    - path: "source/PLUGIN/pluginManager.C"
      provides: "Qt::SkipEmptyParts split"
      contains: "Qt::SkipEmptyParts"
  key_links:
    - from: "source/VIEW/WIDGETS/pyWidget.C compilePattern()"
      to: "QRegularExpression construction"
      via: "push_back(QRegularExpression(...))"
      pattern: "QRegularExpression\\("
    - from: "source/VIEW/WIDGETS/pyWidget.C highlightBlock()"
      to: "QRegularExpressionMatchIterator iteration"
      via: "expression.globalMatch(text)"
      pattern: "globalMatch"
    - from: "source/VIEW/DIALOGS/stageSettings.C"
      to: "screen geometry"
      via: "QGuiApplication::screens().value(i)->geometry()"
      pattern: "QGuiApplication::screens\\(\\)"
    - from: "source/VIEW/WIDGETS/scene.C stereo construction"
      to: "QScreen-based window placement"
      via: "new GLRenderWindow(nullptr, ...); w->setScreen(...)"
      pattern: "setScreen\\("
---

<objective>
Replace the five Qt-5-only API surfaces in VIEW + PLUGIN with their Qt 6 equivalents — inline, no Core5Compat shim (D-05). This is the source-side counterpart to Plan 01's CMake bring-up: with Qt6 components linked, these files would otherwise produce hard compile errors.

Purpose: Honours D-05 (clean break — no carry-over compat module) and lands QT6-02 (Qt-deprecated VIEW APIs replaced). Includes the one out-of-VIEW touch in `source/PLUGIN/pluginManager.C` (`QString::SkipEmptyParts` → `Qt::SkipEmptyParts`) because it is a hard Qt 6 compile error and belongs to the same migration sweep.

Output: 6 files modified; BALL + VIEW + BALLView build clean against Qt 6 (modulo Plan 03's QSurfaceFormat change and Plan 04's CI wiring).
</objective>

<execution_context>
@$HOME/.claude/get-shit-done/workflows/execute-plan.md
@$HOME/.claude/get-shit-done/templates/summary.md
</execution_context>

<context>
@.planning/STATE.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-CONTEXT.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-RESEARCH.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-PATTERNS.md
@include/BALL/VIEW/WIDGETS/pyWidget.h
@source/VIEW/WIDGETS/pyWidget.C
@source/VIEW/DIALOGS/stageSettings.C
@source/VIEW/WIDGETS/scene.C
@source/VIEW/KERNEL/MODES/interactionMode.C
@source/PLUGIN/pluginManager.C

<interfaces>
<!-- API translation table (D-05). All replacements inline; no Qt6::Core5Compat. -->

Replacement summary (file:line targets from 05-RESEARCH.md §Qt 5 → 6 API Surface Delta — read that section for exact line numbers):

1. pyWidget.h:67-70 — `QRegExp` member fields → `QRegularExpression` (include `<QtCore/QRegularExpression>`).
2. pyWidget.C:56,63,66,67 — `QRegExp(...)` ctors → `QRegularExpression(...)` ctors.
3. pyWidget.C:74-112 — `expression.indexIn(text)` / `expression.matchedLength()` loop → `QRegularExpressionMatchIterator it = expression.globalMatch(text); while (it.hasNext()) { auto m = it.next(); setFormat(m.capturedStart(), m.capturedLength(), fmt); }` per the canonical pattern in 05-PATTERNS.md §`source/VIEW/WIDGETS/pyWidget.C`.
4. stageSettings.C:24,433-436 — `#include <QtWidgets/QDesktopWidget>` → `#include <QtGui/QScreen>` + `#include <QtGui/QGuiApplication>`; replace `QApplication::desktop()->screenGeometry(i)` with `QGuiApplication::screens().value(i) ? QGuiApplication::screens().value(i)->geometry() : QRect()`.
5. scene.C:84,2581-2584 — same include swap; Pitfall 5: `QScreen*` is NOT a `QWidget*`, so the stereo `new GLRenderWindow(left_screen, ...)` pattern must change to `new GLRenderWindow(nullptr, ...)` followed by `w->setScreen(screen); w->move(screen->geometry().topLeft());` per 05-PATTERNS.md §`source/VIEW/WIDGETS/scene.C`. These are the 9 guard-deferred stereo sites surfaced in Phase 02.1 VERIFICATION.md — fix them correctly per Open Question 3 recommendation (do not stub).
6. interactionMode.C:5 — delete the dead `#include <QtWidgets/QDesktopWidget>` line. Verify the file has no other QDesktopWidget reference before removing.
7. pluginManager.C:408 — `value.split(";", QString::SkipEmptyParts)` → `value.split(";", Qt::SkipEmptyParts)` (Qt 6 namespace move, [CITED: doc.qt.io/qt-6/qtcore-changes-qt6.html]).

Anti-rules:
- Do NOT add `Qt6::Core5Compat` or `QT_FORCE_ASSERTS` or `QT_DEPRECATED_WARNINGS` defines.
- Do NOT touch sites outside the lines above (no scope creep into Phase 5.1 warnings).
- Do NOT modify `glRenderWindow.{h,C}` here — Plan 03 owns that file.
</interfaces>
</context>

<tasks>

<task type="auto" tdd="false">
  <name>Task 1: Port pyWidget {h,C} from QRegExp to QRegularExpression (D-05)</name>
  <files>include/BALL/VIEW/WIDGETS/pyWidget.h, source/VIEW/WIDGETS/pyWidget.C</files>
  <read_first>
    - include/BALL/VIEW/WIDGETS/pyWidget.h (full file — short header) — line 67-70 has the QRegExp members.
    - source/VIEW/WIDGETS/pyWidget.C lines 1-130 (one pass — pre-compile + highlightBlock fit here).
    - 05-PATTERNS.md §`include/BALL/VIEW/WIDGETS/pyWidget.h` and §`source/VIEW/WIDGETS/pyWidget.C` for the verbatim before/after for both the member declarations and the iterator loop.
    - 05-RESEARCH.md §`source/VIEW/WIDGETS/pyWidget.C + include/BALL/VIEW/WIDGETS/pyWidget.h` line-by-line table.
  </read_first>
  <behavior>
    - Header member fields use `QRegularExpression` (not `QRegExp`), with `<QtCore/QRegularExpression>` included.
    - `compilePattern()` constructs `QRegularExpression(...)` instances using the exact same pattern strings (`delim + *lit + delim`, `"\".*\""`, `"#.*"`) — semantics preserved because these patterns are PCRE-compatible.
    - `highlightBlock()` (or whatever method drives the highlighter — verify in source) iterates matches via `QRegularExpressionMatchIterator` returned from `globalMatch(text)`; per match, calls `setFormat(m.capturedStart(), m.capturedLength(), fmt)`.
    - Behavioural parity: keyword highlighting, string-literal highlighting (`"..."`), and comment highlighting (`#...`) all still fire.
    - Member underscore convention: existing member names already follow project style (`python_patterns`, etc.) — only the *type* changes.
  </behavior>
  <action>
    In `include/BALL/VIEW/WIDGETS/pyWidget.h`:
    1. Replace the existing `<QtCore/QRegExp>` include (if present) with `#include <QtCore/QRegularExpression>`. Match the project's tab-indented internal-include style if present in this header.
    2. At lines 67-70 (per 05-RESEARCH.md), change the four field types from `QRegExp` / `vector<QRegExp>` to `QRegularExpression` / `vector<QRegularExpression>`.

    In `source/VIEW/WIDGETS/pyWidget.C`:
    1. Add `#include <QtCore/QRegularExpression>` (top of file, alphabetically with other Qt includes).
    2. At the four `compilePattern()` construction sites (lines 56, 63, 66, 67 per 05-RESEARCH.md), change `QRegExp(...)` to `QRegularExpression(...)`. Keep the pattern strings byte-identical — D-05 is an API swap, not a regex rewrite.
    3. Rewrite the highlighter loop (lines 74-112 per 05-RESEARCH.md) using `QRegularExpressionMatchIterator` per the canonical pattern in 05-PATTERNS.md — replace each `indexIn` / `matchedLength` / `text.indexOf(expression, ...)` block with the `globalMatch + while (it.hasNext())` shape. Apply the same transformation to the `string_pattern` and `comment_pattern` blocks.

    Do NOT change member naming, do NOT add Core5Compat, do NOT touch other files.
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; cmake --build --preset ci-macos --target VIEW -j$(sysctl -n hw.ncpu) 2>&amp;1 | tee /tmp/pywidget-build.log | tail -30 &amp;&amp; ! grep -qE 'QRegExp' include/BALL/VIEW/WIDGETS/pyWidget.h source/VIEW/WIDGETS/pyWidget.C &amp;&amp; grep -q 'QRegularExpression' include/BALL/VIEW/WIDGETS/pyWidget.h &amp;&amp; grep -q 'globalMatch' source/VIEW/WIDGETS/pyWidget.C</automated>
  </verify>
  <acceptance_criteria>
    - `grep -c '\bQRegExp\b' include/BALL/VIEW/WIDGETS/pyWidget.h source/VIEW/WIDGETS/pyWidget.C` returns `0` (no `QRegExp` token remains; comment removal is acceptable).
    - `grep -c 'QRegularExpression' include/BALL/VIEW/WIDGETS/pyWidget.h` returns `>= 4` (4 member fields).
    - `grep -c 'globalMatch' source/VIEW/WIDGETS/pyWidget.C` returns `>= 3` (python keywords, BALL keywords, plus string/comment loops — at minimum 3; allows for 4 if all paths separate).
    - `grep -c 'QRegularExpression(' source/VIEW/WIDGETS/pyWidget.C` returns `>= 4` (four construction sites).
    - `cmake --build --preset ci-macos --target VIEW -j$(sysctl -n hw.ncpu)` exits 0 with zero `error:` lines from pyWidget files.
    - No `Qt6::Core5Compat` or `QT_DISABLE_DEPRECATED_BEFORE` token introduced (D-05).
  </acceptance_criteria>
  <done>
    pyWidget.h declares its regex members as `QRegularExpression`; pyWidget.C compiles clean on Qt 6 and PythonHighlighter still fires for keywords, strings, and comments (verified by build success — interactive verification is non-blocking).
  </done>
</task>

<task type="auto" tdd="false">
  <name>Task 2: Port the 4 QDesktopWidget sites + pluginManager SkipEmptyParts</name>
  <files>source/VIEW/DIALOGS/stageSettings.C, source/VIEW/WIDGETS/scene.C, source/VIEW/KERNEL/MODES/interactionMode.C, source/PLUGIN/pluginManager.C</files>
  <read_first>
    - source/VIEW/DIALOGS/stageSettings.C — read lines 1-30 (includes) and lines 420-450 (the call site at line 433-436).
    - source/VIEW/WIDGETS/scene.C — read lines 80-95 (includes) and lines 2570-2610 (the stereo construction site at 2581-2584). Look for ALL `desktop` / `QDesktopWidget` / `QApplication::desktop` tokens in this file (there may be additional guard-deferred stereo sites beyond the line 2581-2584 pair).
    - source/VIEW/KERNEL/MODES/interactionMode.C — read lines 1-20 to confirm the include at line 5 is unused beyond the directive itself.
    - source/PLUGIN/pluginManager.C — read lines 400-420 (the SkipEmptyParts site at line 408).
    - 05-PATTERNS.md §`source/VIEW/DIALOGS/stageSettings.C`, §`source/VIEW/WIDGETS/scene.C`, §`source/VIEW/KERNEL/MODES/interactionMode.C`, §`source/PLUGIN/pluginManager.C` — verbatim before/after.
    - 05-RESEARCH.md §Pitfall 5 (the QScreen* / QWidget* type mismatch in scene.C, the recommended `setScreen` / `move` pattern).
  </read_first>
  <behavior>
    - stageSettings.C: includes `<QtGui/QScreen>` + `<QtGui/QGuiApplication>`; no `<QtWidgets/QDesktopWidget>`. Multi-screen geometry comes from `QGuiApplication::screens().value(i) ? screens.value(i)->geometry() : QRect()` (null-safe — `value()` returns default-constructed pointer for out-of-range index).
    - scene.C: same include swap. The stereo `new GLRenderWindow(left_screen, ...)` pattern (and any siblings — the 9 guard-deferred sites) uses `new GLRenderWindow(nullptr, ...)` plus `if (screen) { w->setScreen(screen); w->move(screen->geometry().topLeft()); }`. Compiles under Qt 6; semantically equivalent to the Qt 5 behaviour of placing the stereo window on the chosen screen.
    - interactionMode.C: the dead `#include <QtWidgets/QDesktopWidget>` is removed; no other code in the file references it.
    - pluginManager.C: `QString::SkipEmptyParts` → `Qt::SkipEmptyParts` (Pitfall 8 — hard Qt 6 compile error otherwise).
  </behavior>
  <action>
    For each file:

    1. **stageSettings.C**: At line 24, replace `#include <QtWidgets/QDesktopWidget>` with two lines: `#include <QtGui/QScreen>` and `#include <QtGui/QGuiApplication>`. At lines 433-436, replace the `QDesktopWidget* desktop = QApplication::desktop();` + `QApplication::desktop()->screenGeometry(i)` calls with `QList<QScreen*> screens = QGuiApplication::screens();` and `QRect geom = screens.value(i) ? screens.value(i)->geometry() : QRect();` (one declaration of `screens`, two `value(i)` lookups). Match the canonical block in 05-PATTERNS.md §`source/VIEW/DIALOGS/stageSettings.C`.

    2. **scene.C**: At line 84, same include swap as stageSettings.C. Locate the stereo construction site near lines 2581-2584. Replace `QDesktopWidget* desktop = QApplication::desktop();` + the `desktop->screen(i)` calls + the `new GLRenderWindow(left_screen, ...)` / `new GLRenderWindow(right_screen, ...)` pair with the canonical Pitfall 5 fix: `QScreen* left_screen_obj = QGuiApplication::screens().value(left_screen_index);` (same for right), then `GLRenderWindow* w = new GLRenderWindow(nullptr, ...);` followed by `if (left_screen_obj) { w->setScreen(left_screen_obj); w->move(left_screen_obj->geometry().topLeft()); }`. Repeat for the right-eye window. If the codebase grep reveals additional sibling stereo sites in this file (Phase 02.1 noted ~9 deferred sites), apply the same transformation.

    3. **interactionMode.C**: Verify no QDesktopWidget references remain beyond the include line — `grep -nE 'QDesktopWidget|QApplication::desktop' source/VIEW/KERNEL/MODES/interactionMode.C` should return only line 5. Delete the include line.

    4. **pluginManager.C**: At line 408, change `QString::SkipEmptyParts` to `Qt::SkipEmptyParts`. No include changes (`Qt::` namespace is available via `<QtCore/Qt>` which is already transitively included).

    Do NOT touch `glRenderWindow.{h,C}` (Plan 03's territory). Do NOT add Core5Compat. Do NOT introduce `QT_DEPRECATED_*` defines. Do NOT touch sites beyond those listed above (no Phase 5.1 warning cleanup creep).
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; cmake --build --preset ci-macos --target BALL VIEW BALLView -j$(sysctl -n hw.ncpu) 2>&amp;1 | tee /tmp/qt6-srcport-build.log | tail -50 &amp;&amp; grep -vE '^[[:space:]]*(//|\*|/\*)' source/VIEW/DIALOGS/stageSettings.C source/VIEW/WIDGETS/scene.C source/VIEW/KERNEL/MODES/interactionMode.C source/PLUGIN/pluginManager.C | grep -cE 'QDesktopWidget|QApplication::desktop\(|QString::SkipEmptyParts' | grep -q '^0$' &amp;&amp; grep -c 'QGuiApplication::screens' source/VIEW/DIALOGS/stageSettings.C source/VIEW/WIDGETS/scene.C</automated>
  </verify>
  <acceptance_criteria>
    - `grep -vE '^[[:space:]]*(//|\*|/\*)' source/VIEW/DIALOGS/stageSettings.C source/VIEW/WIDGETS/scene.C source/VIEW/KERNEL/MODES/interactionMode.C source/PLUGIN/pluginManager.C | grep -cE 'QDesktopWidget|QApplication::desktop\(|QString::SkipEmptyParts'` returns `0` (comment-filtered count is zero across all four files).
    - `grep -c 'QGuiApplication::screens()' source/VIEW/DIALOGS/stageSettings.C source/VIEW/WIDGETS/scene.C` returns `>= 2` (both files use the new API).
    - `grep -c 'setScreen(' source/VIEW/WIDGETS/scene.C` returns `>= 1` (Pitfall 5 fix applied).
    - `grep -c 'Qt::SkipEmptyParts' source/PLUGIN/pluginManager.C` returns `>= 1`.
    - `grep -nE 'QtWidgets/QDesktopWidget' source/VIEW/KERNEL/MODES/interactionMode.C` returns no lines (include removed).
    - `cmake --build --preset ci-macos --target BALL VIEW BALLView` exits 0 with zero `error:` lines from the four files above.
    - No `Qt6::Core5Compat` reference anywhere in the modified files (D-05).
  </acceptance_criteria>
  <done>
    All four QDesktopWidget sites and the pluginManager SkipEmptyParts call compile clean against Qt 6; stereo paths in scene.C use `setScreen()` + `move()` instead of the Qt-5-only `QWidget*` desktop screen pattern.
  </done>
</task>

</tasks>

<verification>
- Full VIEW build green on macOS: `cmake --build --preset ci-macos --target BALL VIEW BALLView -j$(sysctl -n hw.ncpu)` exits 0.
- Comment-filtered grep audit: `grep -vE '^[[:space:]]*(//|\*|/\*)' include/BALL/VIEW/WIDGETS/pyWidget.h source/VIEW/WIDGETS/pyWidget.C source/VIEW/DIALOGS/stageSettings.C source/VIEW/WIDGETS/scene.C source/VIEW/KERNEL/MODES/interactionMode.C source/PLUGIN/pluginManager.C | grep -cE 'QRegExp|QDesktopWidget|QApplication::desktop\(|QString::SkipEmptyParts'` returns `0`.
- No `Qt6::Core5Compat` / `qt5compat` reference anywhere in the modified files (D-05).
</verification>

<success_criteria>
- 6 files modified, all five Qt 5-only API surfaces (QRegExp, QDesktopWidget include, QDesktopWidget call sites, scene.C stereo placement, QString::SkipEmptyParts) replaced inline.
- VIEW + BALLView build clean on macOS with the Plan 01 CMake changes applied.
- No Core5Compat shim introduced; clean break per D-05.
</success_criteria>

<output>
After completion, create `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-02-SUMMARY.md` with the build log tail (showing zero errors from the touched files), the diff-stat for the 6 files, and a note on any additional stereo sites in scene.C beyond the documented 4 that needed the same Pitfall 5 fix.
</output>
