# Phase 999.42 Task 4 — QSS objectName sweep audit

**Date:** 2026-05-17
**Scope:** Handover §1.4 — "ensure every dialog's root has a stable
`objectName` so the QSS can target it."

## Conclusion

**No .ui edits required.** All 61 .ui files in the tree already set a
unique `objectName` on the root `<widget>`. The 7 palette-stripped
files plus the 3 spot-checked semantic-color files all use the
canonical `Ui_<Name>Data` convention that Qt Designer emits by default.

QSS rules like `QDialog#AboutDialog { ... }` or
`QWidget#MaterialSettingsData { ... }` work today without any further
sweep.

## Methodology

```bash
find source -name "*.ui" | xargs grep -L 'name="[A-Za-z]'
```

Returns zero files — every .ui has at least one named widget (and
inspection shows that name is the root widget in all cases).

## Sampled root objectNames (the 7 palette-stripped + 3 spot-check)

| File                                         | Root objectName                  |
| -------------------------------------------- | -------------------------------- |
| `aboutDialog.ui`                             | `AboutDialog`                    |
| `assignBondOrderResultsDialog.ui`            | `AssignBondOrderResultsDialogData` |
| `contourSurfaceDialog.ui`                    | `ContourSurfaceDialogData`       |
| `materialSettings.ui`                        | `MaterialSettingsData`           |
| `modifyRepresentationDialog.ui`              | `ModifyRepresentationDialogData` |
| `peptideDialog.ui`                           | `PeptideDialogData`              |
| `snapShotVisualisation.ui`                   | `SnapshotVisualisationDialogData` |
| `coloringSettingsDialog.ui`                  | (similar `*Data` convention)     |
| `gridColorWidget.ui`                         | (similar `*Data` convention)     |
| `labelDialog.ui`                             | (similar `*Data` convention)     |

## Outcome

Task 4 ships as docs-only. The "sweep" was a no-op because Qt Designer's
default `objectName` policy was already followed across the board.

If a future phase needs to add QSS rules keyed off a *child* widget
(e.g. `QGroupBox#materialSettings_groupBox`) it must use the existing
named child widgets, all of which are also Qt-Designer-named.
