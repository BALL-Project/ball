# i18n Workflow — Phase 999.46+ (BALLView Refresh)

This document captures the translation workflow for the BALLView
Refresh (v1.7 Wave 4). It carries forward the `[i18n]` commit-prefix
discipline introduced informally during Phase 999.45 and answers the
maintainer Q4 question (resolved 2026-05-17 as: ship English +
best-effort de_DE; community translation round during v1.7 cycle;
follow-up translation PRs land in v1.7.x patch releases; **NOT a v1.7
ship-date blocker**).

## Scope

The Phase 999.46 menu remap (8 task-coded top-level menus) +
CommandPalette + per-action standard menu roles invalidate roughly
40 % of the strings in `data/BALLView/translations/BALLView-de_DE.ts`
and the much smaller `BALLView-zh_TW.ts`. The new CommandPalette UI
adds a fresh batch of strings (mode hints, placeholder, tooltip
text). About the same scale of churn is expected from Phases
999.47 (onboarding) and 999.48 (legacy-dialog deletion).

## Per-phase mechanics

For any phase that touches user-visible strings (menu titles, dialog
labels, tooltips, status-bar messages, palette text):

1. **During implementation:** Write strings inside `tr(...)` /
   `QObject::tr(...)` calls. Do **not** hand-translate inline.
2. **Before phase close:** Run `lupdate` against the VIEW + BALLView
   sources to merge new source strings into the `.ts` files:
   ```bash
   /opt/homebrew/opt/qt/bin/lupdate \
     source/VIEW source/APPLICATIONS/BALLVIEW \
     -ts data/BALLView/translations/BALLView-de_DE.ts \
        data/BALLView/translations/BALLView-zh_TW.ts \
     -no-obsolete  # OR -no-recursive if you want to keep stale ones for review
   ```
   On CI Linux: `apt install qt6-l10n-tools` then `lupdate6` is the
   binary name. On the maintainer's Windows box, use the Qt installer's
   `lupdate.exe`.
3. **Commit:** the regenerated `.ts` files in a separate commit
   prefixed `[i18n]`:
   ```
   [i18n](999.46/08): refresh translations after menu remap (lupdate run 2026-05-17)
   ```
   Body lists which `.ts` files were touched and any unusual deltas
   (large string removals, file-format version bumps, etc.).
4. **Do NOT hand-fill the translations in the same commit.** Leave
   the new strings untranslated (`<translation type="unfinished"/>`)
   so community translators see a clean diff.

## Community translation round

Once a v1.7 RC tag is cut and the `.ts` files have the full new
string surface from 999.46 + 999.47 + 999.48:

1. **Tag the RC** with the standard release process. The `.ts`
   refresh is part of the RC tag, not a follow-up.
2. **Mailing-list announce** to the BALLView translator list
   (`ballview-i18n@…` — maintainer to confirm exact address). Email
   template:
   - Subject: `[BALLView v1.7 RC] translation round open — <NN> new strings need de_DE / zh_TW / your-locale`
   - Body: brief description of what changed (menu remap to 8
     task-coded menus, new command palette UI, onboarding panel),
     link to the RC tag's `.ts` files in the GitHub repo, deadline
     (typically RC + 4 weeks but explicitly **not blocking** the
     v1.7.0 ship date — completed translations land in v1.7.x
     point releases), and contact for first-time translators
     (point at the Qt Linguist tutorial).
3. **Track translations as PRs.** Each translator submits a
   `[i18n]`-prefixed PR with their completed `.ts`. CI runs lupdate
   in `-no-obsolete` mode to validate the file remains parseable;
   no behavioral test is gated on translation completion.

## Translator-facing primer (for the mailing-list announce)

- Tool: [Qt Linguist](https://doc.qt.io/qt-6/linguist-translators.html)
  (ships with the Qt installer; standalone packages on most Linux
  distros under `qttools` or `qt6-l10n-tools`).
- Workflow: open the `.ts` file in Linguist, fill in translations,
  save, run `lrelease` to compile to `.qm`, test in BALLView with
  `LANG=de_DE.UTF-8 ./BALLView`.
- Conventions: keep Qt `&` mnemonic markers; preserve `%1`/`%2`
  placeholders verbatim; keep accelerators consistent (e.g. don't
  reassign `Ctrl+O` to something else inside a translated tooltip).

## Audit pull-in resolution

Per the 2026-05-17 audit pull-in (Q4 re-pinned to v1.7 cycle), this
phase ships the i18n process doc but does **not** wait for translator
completion. Subsequent BALLView Refresh phases (999.47 onboarding,
999.48 BALL_UI_V2 flip + legacy delete) follow the same pattern:
land English + lupdate refresh; community translations arrive
asynchronously and ride v1.7.x point releases.

## File checklist (Phase 999.46)

- [x] `data/BALLView/translations/BALLView-de_DE.ts` — refreshed via
      `lupdate` (Qt 6.11.0). 113 new source strings found, 793
      obsolete entries preserved for translator review, 9
      auto-translated via same-text heuristic.
- [ ] `data/BALLView/translations/BALLView-zh_TW.ts` — **deferred**:
      `lupdate` reports "won't be updated: it does not specify any
      target languages. To set a target language, open the file in
      Qt Linguist." Pre-existing condition (not introduced by this
      phase). Translator round needs to open the file in Qt Linguist
      and set the target language header before subsequent
      `lupdate` runs will take. Logged as deferred-item.
- [x] This document (`I18N-WORKFLOW.md`) — published.
- [ ] Mailing-list announce — deferred to v1.7 RC notes (per Q4).
