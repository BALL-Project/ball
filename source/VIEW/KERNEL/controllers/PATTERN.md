# `*Controller` extraction pattern — Phase 999.44

This directory contains the **Controller layer** for the BALLView
Refresh (Handover Phase 4 / Phase 999.44). Each Controller owns the
presentation logic for one legacy dialog. Both the legacy dialog
(during the migration window) and the new Inspector section bind to
the same Controller, so the two surfaces are consistent
*by construction* — not by message-bus discipline.

This is the primary risk mitigation called out in Handover
§04-phase-inspector.md §Risks ("Mitigation: extract controllers in a
refactor commit *before* introducing the section UI").

## Pattern

A Controller is a `QObject` that:

1. Lives under `BALL::VIEW`, header at
   `include/BALL/VIEW/KERNEL/controllers/<name>Controller.h`,
   impl at `source/VIEW/KERNEL/controllers/<name>Controller.C`.
2. Has **zero `<QtWidgets/*>` includes** — it's headless-testable.
   Allowed includes: `<QtCore/*>`, `<QtGui/QColor>`, `<BALL/...>`.
3. Mirrors the legacy dialog's settable state via `Q_PROPERTY`
   declarations + matching `xxxChanged(...)` signals.
4. Provides `apply()` (push state → model) and `revert()` (pull
   model → state, emitting `xxxChanged` along the way).
5. Initial landing can be a **read-only mirror** — `apply()` is a
   `Log::warn` stub + `appliedStub` signal. The follow-up cut-over
   plan migrates the legacy dialog's `apply()` body into the
   Controller's `apply()` and removes the stub.

The exemplar is `StageController` (this directory). Read it
end-to-end before adding a new Controller.

## Extraction status

| # | Controller            | Source dialog                                                           | Status                              |
|---|-----------------------|-------------------------------------------------------------------------|-------------------------------------|
| 1 | StageController       | `source/VIEW/DIALOGS/stageSettings.C` (720 LOC)                         | CUT-OVER 999.44 Plan 06 (mutating)  |
| 2 | LightController       | `source/VIEW/DIALOGS/lightSettings.C` (538 LOC)                         | LANDED 999.44 Plan 05 (read-only)   |
| 3 | CameraController      | `source/VIEW/DIALOGS/setCamera.C` (94 LOC)                              | LANDED 999.44 Plan 05 (read-only)   |
| 4 | StereoController      | `source/VIEW/DIALOGS/stereoSettingsDialog.C` (159 LOC)                  | LANDED 999.44 Plan 05 (read-only)   |
| 5 | ModelController       | `source/VIEW/DIALOGS/modelSettingsDialog.C` (385 LOC)                   | LANDED 999.44 Plan 04 (read-only)   |
| 6 | ColoringController    | `source/VIEW/DIALOGS/coloringSettingsDialog.C` (878 LOC)                | LANDED 999.44 Plan 04 (read-only)   |
| 7 | MaterialController    | `source/VIEW/DIALOGS/materialSettings.C` (280 LOC)                      | LANDED 999.44 Plan 04 (read-only)   |
| 8 | LabelController       | `source/VIEW/DIALOGS/labelDialog.C` (227 LOC)                           | LANDED 999.44 Plan 04 (read-only)   |
| 9 | ClippingController    | `source/VIEW/DIALOGS/clippingDialog.C` (119 LOC) + `setClippingPlane.C` (92 LOC) | LANDED 999.44 Plan 04 (read-only)   |

## Mutation cut-over status

- StageController — **CUT-OVER COMPLETE in 999.44 Plan 06.**
  apply() now pushes mirrored fields to Stage (backgroundColor /
  coordinate-system / fog / eye-distance / focal-distance). Both
  legacy StageSettings dialog and Inspector StageSection +
  BackgroundSection mutate the Stage via this single method.
- All other 8 Controllers — read-only mirror; cut-over deferred to
  v1.7 RC patch cycle. Sections bind to mirrors so users see live
  values; legacy dialogs (reachable via Tools › Legacy Settings) own
  mutation during the migration window. Cut-over recipe is the
  StageController::apply() shape — copy-paste-modify per controller.

Total deferred LOC: ~3072 across 9 dialog files (8 controllers — Clipping merges the
two clipping dialogs into one Controller per Handover §architecture).

## Migration window invariant

While Controllers are partial (read-only mirror), the legacy
dialogs MUST remain reachable via `Tools › Legacy Settings` so end
users keep access to settings the Inspector hasn't yet exposed.
Phase 999.48 will remove the `Tools › Legacy Settings` submenu
once every section is wired and every Controller has migrated its
`apply()` path.

## Renderer-interface-boundary binding

`CameraController` + `StereoController` + (parts of) `StageController`
touch renderer/scene settings. They MUST bind through the
`Renderer` / `RenderSurface` interface from `.planning/RENDERER-
INTERFACE-BOUNDARY.md` (SEED-001 step 5), NOT today's `GLRenderer`
directly. `StageController`'s initial extraction skipped this
binding because it is read-only; the cut-over plan must reintroduce
it before pushing mutations.
