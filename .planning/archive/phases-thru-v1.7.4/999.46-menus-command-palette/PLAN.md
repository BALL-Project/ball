---
phase: 999.46
plan: 01
subsystem: VIEW/UI
type: feature
autonomous: true
wave: v1.7-wave-4
depends_on: [999.44, 999.45]
flag: BALL_UI_V2
requirements: []
---

# Phase 999.46 — Menus + command palette (Handover Phase 6)

## Objective

Re-author the BALLView top menu bar around tasks (8 top-level menus:
File · Edit · Select · View · Compute · Scripts · Window · Help),
introduce a `CommandRegistry` singleton that auto-registers every
menu-entered `QAction`, and surface every command through a
`Cmd/Ctrl+K` floating **command palette** with fuzzy match + 3 prefix
modes (`>` files, `?` help, `:` navigation).

All new code is gated behind `BALL_UI_V2`. The legacy menu structure
remains untouched in the OFF cell to keep the Classic preset
identical until Phase 999.49 deletes it.

## Context

- `Claude Design Handover/revitalization/06-phase-menus.md` — 18-row
  mapping table + command-palette UX modes (authoritative).
- Maintainer Q1 (RESOLVED 2026-05-17): **keep INLINE menubar**. Do NOT
  use full `QMenuBar` global-menu mode. Apply per-action
  `QAction::setMenuRole(...Standard)` for About / Preferences / Quit
  only — Qt auto-relocates them to the macOS application menu.
- Maintainer Q4 (RESOLVED 2026-05-17): `[i18n]` commit-prefix
  discipline; community translation round in v1.7 RC notes; **not** a
  ship-date blocker.
- 999.44 + 999.45 already landed on `v1.7-modernization` HEAD
  `a5dcee9bbe` or later. CI run 25998138996 is 8/8 green on that HEAD.
- Existing menu infrastructure lives in
  `source/VIEW/KERNEL/mainControl.C` (`insertMenuEntry`,
  `initPopupMenu`) and the `MainControl::PopUpID` enum in
  `include/BALL/VIEW/KERNEL/mainControl.h`.

## Tasks

### Task 1 — `CommandRegistry` skeleton (auto)

Files:
- `include/BALL/VIEW/KERNEL/commandRegistry.h` (NEW)
- `source/VIEW/KERNEL/commandRegistry.C` (NEW)
- `include/BALL/VIEW/KERNEL/sources.cmake` (touch — GLOB picks it up)
- `source/VIEW/KERNEL/sources.cmake` (add `commandRegistry.C` to
  `SOURCES_LIST`)

Behavior:
- Singleton `CommandRegistry::instance()`.
- `Command` POD with `id` (QString), `title` (QString),
  `category` (QString), `description` (QString),
  `shortcut` (QKeySequence), `icon` (QIcon), `trigger`
  (`std::function<void()>`), `isEnabled` (`std::function<bool()>`),
  plus a non-owning `QPointer<QAction> action` back-pointer
  (the registry never owns the QAction; the menu does).
- `void registerCommand(Command)` — idempotent on `id`.
- `void registerFromAction(QAction* action, const QString& category)`
  — convenience the menu hook calls; synthesizes a stable `id` from
  the category + action `objectName` + (fallback) action `text`.
- `QList<Command> all() const`.
- `QList<Command> search(const QString& query) const` —
  subsequence fuzzy match against `title` and `id`; results sorted
  by match-quality score (exact prefix > word boundary > subsequence).
- `QList<Command> recent() const` + `void noteTriggered(QString id)`
  — MRU list capped at 16 entries (deduped by `id`).
- **Entire class body gated behind `#ifdef BALL_UI_V2`** at the
  source level; the header remains compilable in the OFF cell
  (typedefs + empty inline stubs) so callers don't need to
  `#ifdef` around includes.

Verify (auto): macOS-arm64 `cmake --build build-ui-on` succeeds with
`BALL_UI_V2=ON` and `BALL_UI_V2=OFF`; libVIEW links.

Commit: `feat(999.46/01): add CommandRegistry singleton (BALL_UI_V2)`

### Task 2 — Auto-register hook in `MainControl::insertMenuEntry`

Files:
- `source/VIEW/KERNEL/mainControl.C`

Behavior:
- After successful `popup->addAction(...)` in both overloads of
  `insertMenuEntry`, call
  `CommandRegistry::instance().registerFromAction(action, /*category*/ menuTitle)`
  under `#ifdef BALL_UI_V2`. Category derived from the parent menu
  `popup->title()` (strip leading `&`).
- Plugin entries that route through `insertMenuEntry` get registered
  with whatever category their parent menu carries; the `setCategory()`
  override hook is exposed via
  `CommandRegistry::overrideCategory(QAction*, const QString&)` for
  plugins that want a different bucket. Default: parent-menu name.

Verify: build, libVIEW links, no behavior change with flag OFF.

Commit: `feat(999.46/02): auto-register menu actions with CommandRegistry`

### Task 3 — Menu remap (18 rows, behind `BALL_UI_V2`)

Files:
- `include/BALL/VIEW/KERNEL/mainControl.h` (additive enum values for
  new top-level menus + new submenus; existing values preserved for
  legacy cell)
- `source/VIEW/KERNEL/mainControl.C` (extend `initPopupMenu` switch
  with new IDs, ifdef-gated)
- `source/APPLICATIONS/BALLVIEW/mainframe.C` (ifdef path to wire the
  new top-level menus in correct left-to-right order; legacy path
  unchanged)

Mapping (Handover §"New menu bar mapping" — 18 rows):
1.  File › *                              → File › * (unchanged)
2.  File › Open › PDB                     → File › Open... + source sub
3.  Edit › Cut/Copy/Paste                 → Edit › *
4.  Edit › Invert Selection               → **Select** › Invert
5.  Edit › Clear Selection                → **Select** › Clear
6.  Edit › Selection by Expression        → **Select** › By Expression…
7.  Build › Add Hydrogens                 → Edit › Structure › Add Hydrogens
8.  Build › Generate Peptide              → Edit › Structure › Generate Peptide…
9.  Build › Generate Crystal              → Edit › Structure › Generate Crystal…
10. Display › *                           → View › * (renamed top-level)
11. Display › Toggle Fullscreen           → View › Enter Fullscreen (⌘⌃F / F11)
12. Display › Workspace                   → View › Workspace › *
13. Display › Viewpoint                   → View › Viewpoint › *
14. Molecular Mechanics › *               → **Compute** › Energy ›
15. Molecular Mechanics › Minimize        → Compute › Minimize…
16. Molecular Mechanics › MD              → Compute › Dynamics…
17. Molecular Mechanics › Abort           → Compute › Abort Calculation (⌥C / Alt+C)
18. Tools › * / Macros › *                → **Scripts** › * / Scripts › Macros › *
    Windows › *                           → **Window** › *
    Help › *                              → Help › * (unchanged)

8 top-level: File · Edit · Select · View · Compute · Scripts · Window · Help.

Implementation strategy: add new enum values (`SELECT`, `VIEW_TOP`,
`COMPUTE`, `SCRIPTS`, `WINDOW_TOP`, `EDIT_STRUCTURE`, `VIEW_WORKSPACE`,
`COMPUTE_ENERGY`, `SCRIPTS_MACROS`) and a v2-only branch in
`initPopupMenu` that creates them. The legacy enums (`EDIT`, `BUILD`,
`DISPLAY`, `MOLECULARMECHANICS`, `TOOLS`, `WINDOWS`, `MACRO`) are
*re-routed* to the new top-level when `BALL_UI_V2` is defined, so all
existing `insertMenuEntry(MainControl::EDIT, ...)` call-sites land in
the right reorganized location without per-callsite edits. The
`Window` menu also lists workspace presets (read
`WorkspaceManager::presetNames()` public API from 999.45).

The `Hide Inspector` toggle (999.44) already lives on `DISPLAY` which
re-routes to `View` under v2 — exactly what we want.

Verify: build OFF cell (no behavior delta), build ON cell (8 top
menus, every old shortcut still resolves via the same descriptor
string).

Commit: `feat(999.46/03): remap menu structure to 8 task-coded top-level menus`

### Task 4 — macOS `setMenuRole(...Standard)` for About / Preferences / Quit

Files:
- `source/VIEW/KERNEL/mainControl.C` (Quit + Preferences)
- `source/APPLICATIONS/BALLVIEW/mainframe.C` (About)

Behavior:
- Quit action: `quit_action->setMenuRole(QAction::QuitRole)`.
- Preferences action: `preferences_action_->setMenuRole(QAction::PreferencesRole)`.
- About action: `action->setMenuRole(QAction::AboutRole)`.
- Apply unconditionally (not gated by `BALL_UI_V2`) — Qt is a no-op
  on Linux/Windows and a positive relocation on macOS. This makes
  the OFF cell also benefit from correct app-menu placement.
- Cite Handover §06 + §09-cross-platform.md in code comment.

Verify: macOS-arm64 launch sanity (Quit + About + Preferences appear
in the BALLView app-menu, not duplicated in File/Edit/Help).

Commit: `feat(999.46/04): setMenuRole(...Standard) for About/Prefs/Quit (macOS app-menu)`

### Task 5 — `CommandPalette` widget skeleton (auto)

Files:
- `include/BALL/VIEW/WIDGETS/commandPalette.h` (NEW)
- `source/VIEW/WIDGETS/commandPalette.C` (NEW)
- `include/BALL/VIEW/WIDGETS/sources.cmake` (GLOB picks it up)
- `source/VIEW/WIDGETS/sources.cmake` (add `commandPalette.C`)

Behavior (Handover §6.3):
- `class CommandPalette : public QWidget` — frameless floating sheet,
  `Qt::Popup | Qt::FramelessWindowHint`, 560 px wide, centered on
  parent main window.
- `QLineEdit` at top with placeholder "Type a command, or ?".
- `QListView` over a `QSortFilterProxyModel` wrapping a custom
  `QAbstractListModel` backed by `CommandRegistry::search(query)`.
- Custom delegate renders: title (left, bold) + category chip + shortcut
  (right, monospace) + icon (left-most, 20px).
- `Enter` triggers the highlighted command (calls
  `Command.trigger()` then `CommandRegistry::noteTriggered(id)` then
  `close()`); `Esc` closes; `Tab` accepts current line text into
  query (no-op pass-through this phase — full completion deferred).
- When query is empty: show `CommandRegistry::recent()` as the result
  list (max 8 visible, scroll for more).
- Whole TU gated behind `#ifdef BALL_UI_V2`; header has stubs in OFF.

Wiring (in `Mainframe::Mainframe`, under `#ifdef BALL_UI_V2`):
- Construct a single `CommandPalette` parented to the mainframe.
- Install a global `QShortcut` for `Cmd+K` (macOS) /
  `Ctrl+K` (Linux/Windows) using `QKeySequence(Qt::CTRL | Qt::Key_K)`
  (Qt translates CTRL → ⌘ on macOS automatically).
- Shortcut shows/centers/focuses the palette.

Verify: build ON + OFF; macOS-arm64 launch + Cmd+K opens palette,
typing "ene" matches expected commands.

Commit: `feat(999.46/05): add CommandPalette widget with Cmd/Ctrl+K shortcut`

### Task 6 — Command-palette `?` / `>` / `:` modes (auto)

Files:
- `source/VIEW/WIDGETS/commandPalette.C` (extend filter logic)

Behavior:
- Detect first character of query:
  - `>` → restrict matches to commands whose `category == "File"`
    (per Handover: "only file actions, jump to Open Recent").
  - `?` → help mode: don't filter, but the delegate switches to
    "help row" rendering (larger description text, shortcut
    prominent). Stripped prefix searches against title +
    description.
  - `:` → navigation/jump mode: restrict matches to commands whose
    `id` starts with `view.workspace.` or `view.inspector.` or
    `file.recent.` (Inspector-section jumps when 999.47 adds them;
    workspace presets ship live from 999.45).
  - Otherwise: fuzzy search all commands (default mode).
- `?` / `>` / `:` are 3 cheap conditional branches over the search
  result — implementation cost is small.

Verify: build, manual smoke: each prefix changes the result set.

Commit: `feat(999.46/06): CommandPalette ?/>/: prefix modes`

### Task 7 — `menuMapping_test.C` (auto)

Files:
- `test/menuMapping_test.C` (NEW)
- `test/cmake/VIEWTestExecutables.cmake` (add `menuMapping_test`)

Behavior (minimal per scope-trim guidance):
- Asserts `MainControl::PopUpID` enum has the 8 new top-level values
  defined (`SELECT`, `VIEW_TOP`, `COMPUTE`, `SCRIPTS`, `WINDOW_TOP`,
  plus the preserved `FILE`, `EDIT`, `HELP`).
- Asserts `CommandRegistry::instance().all()` is empty before any
  action is registered, accepts a `registerCommand` round-trip, and
  `search("test")` returns the synthetic command.
- Deeper coverage (every old shortcut maps to a command with the
  expected title against a live `Mainframe`) deferred to v1.7 RC
  patch — flagged in SUMMARY.md.

Verify: `cmake --build build/test --target menuMapping_test && ./bin/TEST/menuMapping_test`.

Commit: `test(999.46/07): add minimal menuMapping_test`

### Task 8 — `lupdate-qt5` refresh + I18N workflow doc

Files:
- `data/BALLView/translations/*.ts` (regen — only if `lupdate-qt5` is
  available locally; otherwise commit the source/header edits and
  add a TODO that the next translation regen will pick them up)
- `.planning/phases/999.46-menus-command-palette/I18N-WORKFLOW.md`
  (NEW — process doc for translator mailing-list announce)

Behavior:
- Document `[i18n]` commit-prefix discipline.
- Document the lupdate regen + announce flow (carry over from 999.45
  i18n discipline; this phase just publishes the doc that 999.45's
  decision implied).
- Note: v1.7 RC notes will call community translators for round.

Verify: if `lupdate-qt5` is installed, run it; otherwise write a
TODO note to the I18N-WORKFLOW.md tracking the deferral. **Do not
block the phase on translator response time — Q4 says translations
are NOT a v1.7 ship blocker.**

Commit: `[i18n](999.46/08): refresh translations + add I18N-WORKFLOW.md`

## Verification

Per-task `cmake --build` smoke (BALL_UI_V2=ON + OFF on macOS-arm64).
CI dispatched after each commit; final CI run after Task 8 must be
8/8 green on the `ui_v2: [ON, OFF]` × 4 OSes matrix.

## Success criteria

- 8 top-level menus visible in BALL_UI_V2=ON cell (`File · Edit ·
  Select · View · Compute · Scripts · Window · Help`).
- About / Preferences / Quit auto-relocate to macOS app-menu (per Q1
  per-action standard-role approach, not full global menubar).
- Cmd/Ctrl+K opens the command palette; typing "ene" matches
  Compute/Minimize commands.
- `menuMapping_test` green.
- `BALL_UI_V2=OFF` cell behaviorally identical to pre-999.46.
- CI matrix 8/8 green at HEAD post-Task 8.

## Output

`999.46-SUMMARY.md` in this directory; ROADMAP §999.46 flipped to
COMPLETE; STATE.md current-position advanced; 999.47 dispatch
unblocked.
