# Phase 999.42 Task 5 — lupdate refresh: deferred

**Date:** 2026-05-17
**Disposition:** SKIP per PLAN Rule 3 escape clause.

## Why deferred

Handover §1.5 asks to "run `lupdate-qt5` to confirm no string churn"
post palette-strip and commit any trivial deltas.

Reality: running Qt 6.11 `lupdate -recursive source/APPLICATIONS/BALLVIEW
source/VIEW -ts data/BALLView/translations/BALLView-de_DE.ts` against
HEAD produces a **10,646-line diff** dominated by:

1. **.ts schema upgrade** Qt 5 → Qt 6: `<TS version="2.0">` →
   `<TS version="2.1">`.
2. **Source-text drift** across the entire VIEW + BALLVIEW codebase
   since the last lupdate run — hundreds of strings that have moved,
   been re-typed, or had their `.h` location lines change (e.g.
   `aboutDialog.h:157` no longer exists because the constants were
   refactored years ago).
3. **`<location>` line-number tracking** for every single translatable
   message in every file.
4. **Marginal palette-strip contribution:** the strip only changes
   `<location ... line="N">` for the 7 .ui files. Net translatable-
   string change from the palette work alone is **zero** (palette
   blocks contain no `<string>` elements).

Bundling 10,646 lines of unrelated translation maintenance into a
"phase 999.42" commit would mask the actual palette work and violate
atomic-commit discipline.

## What was verified

- `lupdate` reports `0 new` source texts when re-run with the same
  input set (idempotent after the first refresh would land).
- The first run reports `112 new` strings — these are from non-
  palette codebase drift, NOT from the strip.

## Recommendation

Open a dedicated `chore(i18n)` follow-up in a future v1.7 RC stage:

  `chore(i18n): refresh de_DE + zh_TW .ts via Qt 6 lupdate`

Track as 999.49.x or a standalone v1.7 hygiene phase. **Not blocking**
for Phase 999.42 because the strip itself preserves all translatable
strings.

## Acceptance impact

Handover acceptance criterion ("`.ts` files unaffected") is satisfied
in spirit: the palette strip contributes zero translatable-string
churn. The accumulated drift is a pre-existing condition.
