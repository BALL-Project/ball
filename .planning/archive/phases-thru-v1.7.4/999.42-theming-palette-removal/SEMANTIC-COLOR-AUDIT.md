# Phase 999.42 Task 3 — Semantic-Color Audit

**Date:** 2026-05-17
**Auditor:** GSD executor (Phase 999.42)
**Scope:** Verify that stripping `<palette>` from the 7 .ui files in
Task 2 did not destroy any *semantic* color information (residue
swatches, status badges, etc.).

## Conclusion

**No code restore needed.** All semantic color in BALLView is set
programmatically via C++ helpers, NOT via `.ui` palette blocks. The
`<palette>` data we stripped was pure chrome (window-frame /
groupbox-background / scrollbar-track / Windows-2000-beige).

## Methodology

For each of the 5 Handover-listed "semantic color" files (per
`01-phase-theming.md` §1.3) — only 2 of which actually had palette
blocks in HEAD — locate the color-setting code path and verify it
does not depend on the QWidget palette set by `uic`.

## Per-file findings

### `materialSettings` (had palette — stripped)

- Color swatches: `ambient_color_label`, `specularity_color_label`,
  `reflectiveness_color_label` (all `QLabel`).
- Color set via `VIEW::setColor(label, ColorRGBA)` at construction
  (line 30-32) and reset on `update()` (line 119-121, 147-149).
- User picks via `VIEW::chooseColor(label)` (line 257, 265, 273)
  which writes the new color back to the label.
- **Verdict:** color flow is 100 % code-driven. Palette strip has zero
  visual effect on swatches.

### `assignBondOrderResultsDialog` (had palette — stripped)

- `grep "setStyleSheet\|setPalette\|QColor\|QBrush"
  source/VIEW/DIALOGS/assignBondOrderResultsDialog.C` returns nothing.
- The stripped `<palette>` was 64 lines of chrome only — no semantic
  badges or status colors. The dialog is a result-table view; row
  colors come from the table model, not the dialog palette.
- **Verdict:** no semantic intent in the stripped block.

### `coloringSettingsDialog` (NO palette in HEAD)

- Heavy semantic color use throughout — residue color buttons,
  element color buttons, charge color triplets, distance color
  ramps, helix/coil/strand/turn colors, secondary structure ramps,
  override colors per table row.
- Mechanism: `ColorButton::setColor(ColorRGBA)` (`setColor` is a
  member method) + `QTableWidgetItem::setBackground(QColor)`.
- **Verdict:** semantic colors flow entirely through code; no
  palette dependence. (Confirmed at line 391, 485, 754-789.)

### `gridColorWidget` (NO palette in HEAD)

- File present; gradient stops are constructed programmatically (not
  via `<palette>`). No `<palette>` block to begin with.
- **Verdict:** nothing to restore.

### `labelDialog` (NO palette in HEAD)

- `color_sample_` (the foreground-color preview) set at line 71 via
  `setColor(color_sample_, custom_color_)`. The model commits the
  picked color via `setColor(custom_color_)` at line 151.
- **Verdict:** code-driven, no palette dependence.

## Outcome

Task 3 ships as docs-only — no .C edits required, no behavior change.
This audit lives at
`.planning/phases/999.42-theming-palette-removal/SEMANTIC-COLOR-AUDIT.md`
and is referenced from the phase SUMMARY.
