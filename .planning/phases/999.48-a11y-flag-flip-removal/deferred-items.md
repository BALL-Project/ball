# Phase 999.48 — Deferred items (v1.7 RC patch)

Phase 999.48 scope-trimmed the Handover §8.8 9-file legacy DELETION and
the Tools › Legacy Settings submenu retirement to the v1.7 RC patch.
Reasons + breadcrumbs below so the RC-patch agent can finish the work
without re-running the dependency audit.

## Why the 9-file deletion deferred

The 9 dialog files form a **tightly coupled cluster** that cannot be
unwound without first landing the deferred Controller cut-over plans
(8 plans noted in `.planning/STATE.md` "Stopped At" field).

Specifically:

| File                            | Active dependency (blocking deletion)                                     |
|---------------------------------|---------------------------------------------------------------------------|
| `displayProperties.{C,h,ui}`    | mainControl.C:2059,2063,2098,2121 ; representationManager.C:336 ; molecularControl.C:446,460 ; geometricControl.C:402 ; mainframe.C:55 ; pluginDialog.C:8 ; demoTutorialDialog.C:10 |
| `modelSettingsDialog.{C,h,ui}`  | **displayProperties.C:11,142,451,471** (active rendering pipeline)        |
| `materialSettings.{C,h,ui}`     | scene.C:142 (`new MaterialSettings(this)`) ; demoTutorialDialog.C:211     |
| `lightSettings.{C,h,ui}`        | scene.C:141 (`new LightSettings(this)`) ; demoTutorialDialog.C:126,186,382|
| `stageSettings.{C,h,ui}`        | scene.C:161 (`new StageSettings(this)`) ; demoTutorialDialog.C:110,171    |
| `stereoSettingsDialog.{C,h,ui}` | NO active code dependency — was claimed safe-to-delete by initial audit, but compilation breaks because `displayProperties.C` indirectly depends on it via the Preferences dialog stack registration mechanism |
| `clippingDialog.{C,h,ui}`       | geometricControl.C:686 (`ClippingDialog dialog;` stack-allocated)         |
| `coloringSettingsDialog.{C,h,ui}` | **displayProperties.C:12,148,487** (active rendering pipeline)          |
| `labelDialog.{C,h,ui}`          | mainframe.C:213 (`new LabelDialog(this, ...)`)                            |

**The lynchpin is `displayProperties.C`** — line 451 (`rep.setModelProcessor(model_settings_->createModelProcessor(...))`) and line 487 (`rep.setColorProcessor(coloring_settings_->createColorProcessor(...))`) are still on the production rendering path. Removing the model/color settings dialogs requires moving the `createModelProcessor` + `createColorProcessor` factories somewhere else first — that's exactly the work the deferred Controller cut-over plans do (Plan 999.44-04a through Plan 999.44-04h per STATE).

## Why the Tools › Legacy Settings menu deferred

`mainframe.C:382-394` builds the submenu from
`VIEW::LegacySettingsHelper::legacyStackNames()`. Deleting the submenu
now would orphan the LegacySettingsHelper without removing the
preference-stack pages it surfaces — half-removed surface confuses
users more than fully present or fully absent surface. Defers
together with the 9-file deletion above as a single retirement cycle.

## Recommended sequence for the RC patch

1. Land the 8 Controller cut-over plans (Plan 999.44-04a..04h) to
   migrate `createModelProcessor` / `createColorProcessor` /
   `apply()` machinery out of the legacy dialogs into the Inspector
   sections via the Controller layer.
2. With the rendering pipeline detached from the dialogs:
   - Delete `displayProperties.{C,h,ui}` + `.sip` first (lynchpin)
   - Cascade-delete the 8 dependent dialog file triples
   - Update `source/VIEW/DIALOGS/sources.cmake` (drop the 9 .ui +
     9 .C entries; the headers globbed from `include/BALL/VIEW/DIALOGS/`
     auto-drop when the .h files are deleted)
   - Delete `displayProperties.sip` + `lightSettings.sip` from
     `source/PYTHON/EXTENSIONS/VIEW/` and their `%Include` lines in
     `VIEW.sip:91-92`
3. Delete `LegacySettingsHelper` and the `Tools › Legacy Settings`
   submenu block in `mainframe.C:370-394`

## Acceptance criteria for the RC patch

- `grep -r "displayProperties\|modelSettingsDialog\|materialSettings\|lightSettings\|stageSettings\|stereoSettingsDialog\|clippingDialog\|coloringSettingsDialog\|labelDialog" source/ include/` → 0 source-file matches (only `.planning/` history references remain)
- BALLView links green on macOS + Linux + Windows
- The 6 Inspector sections (Model / Stage / Coloring / Material / Light / Clipping) still drive the rendered scene end-to-end
