# RC3 MASTER PLAN — UFG digest + codex review + fix grouping

**Branch:** `v1.7-modernization` · **HEAD:** `c9d6267405` · **Generated:** 2026-05-18

## Status snapshot

| UFG | Title | Severity | On-disk status | In-flight agent | ETA |
|---|---|---|---|---|---|
| UFG-07 | Selection name-highlight redesign | DESIGN | UNADDRESSED | none | 0.5–1 day (medium) |
| UFG-08 | WelcomeScreen blank on launch | CRITICAL | FIXED (`7297b0ea75`) | agent A (closing) | done — verify on rc3 |
| UFG-09 | Inspector tab duplication ghost | IMPORTANT | FIXED (`80dca4d3d7`) | agent A | done — verify on rc3 |
| UFG-10 | Inspector header garble (QUICK AC) | IMPORTANT | FIXED (`c14428d98b`) | agent A | done — verify on rc3 |
| UFG-11 | Scene tab Controllers stub | CRITICAL | MOSTLY FIXED (camera+light+stereo writes; **Stereo `Enabled` checkbox still mirror-only**) | agent B | 1 small follow-up (gate the checkbox) |
| UFG-12 | Scene tab defaults stale on tab-show | IMPORTANT | FIXED via `onTabChanged_` revert hook (`inspectorView.C:196-228`) | agent B | done with caveat — see codex critique |
| UFG-15 | Add Hydrogens toolbar doesn't propagate | CRITICAL | UNADDRESSED | none | 1–2 hr (small, but **redirect required — codex flagged wrong hypothesis**) |
| UFG-16 | Add Hydrogens icon unclear | MINOR | UNADDRESSED | none | <1 hr |
| UFG-17 | Logs drawer rendering broken | IMPORTANT | UNADDRESSED | none | 1–2 hr (medium) |

---

## Per-UFG analysis

### UFG-07 — Selection UX redesign: drop Type + Checked columns, convey via name highlight

**Symptom (user):** "Left-hand tree view still contains the columns to be removed" — wants single Name column, selection = colored name text, no checkbox, no Type column.

**Root cause:** `source/VIEW/WIDGETS/molecularControl.C:134-138` constructs the 3-column header (`Name [highlight]` / `Type` / `checked`); each tree item gets `Qt::ItemIsUserCheckable` + `setCheckState(2, …)` in `MyTreeWidgetItem::init_()` (lines 92-96). The checkbox at column 2 IS the BALL `composite->isSelected()` mirror, wired bidirectionally at `:95-96, 1054-1067, 1563, 1679-1684`.

**Code surface to modify:**
- `source/VIEW/WIDGETS/molecularControl.C` — column count → 1, drop `ItemIsUserCheckable` from flags, replace `setCheckState`/`checkState(2)` reads with `composite->isSelected()`, override `data(Qt::ForegroundRole)` (via subclass), rewrite `onItemClicked_`/`highlightSelection`/`setSelection_`.
- `source/VIEW/WIDGETS/molecularControl.h` — adjust `MyTreeWidgetItem` API.

**Cross-cuts:** Selection-bus contract with `SelectionInspectorAdapter` (`source/VIEW/WIDGETS/inspector/selectionAdapter.C`). Codex flagged: there are TWO selection concepts in the codebase — (a) `Composite::isSelected()` (BALL selection, what checkbox mirrors) vs (b) `QTreeWidgetItem::isSelected()` (Qt row highlight, what `MolecularControl::selected_` and `MainControl::getMolecularControlSelection()` track). `mainControl.h:379` documents them as distinct. UFG-07's request "convey selection via highlight color" merges the visuals — but the recipe must NOT silently merge the data contracts.

**Fix scope:** Medium, ~200-300 LOC.

**Fix recipe (revised after codex critique):**
1. `listview->setColumnCount(1)` + `headerItem()->setText(0, tr("Name"))`.
2. `MyTreeWidgetItem::init_()` — remove `ItemIsUserCheckable` flag; drop both `setCheckState(2,…)` calls.
3. Override `MyTreeWidgetItem::data(int column, int role)`: when `role == Qt::ForegroundRole && composite_->isSelected()`, return `treeWidget()->palette().highlight().color()` (codex correction: cannot use `palette()` on `QTreeWidgetItem` directly). Otherwise call base.
4. Replace every `checkState(2) == Qt::Checked` (lines 1063-1067, 1563, 1679-1684) with `composite_->isSelected()`.
5. Replace checkbox-click toggle path with `onItemClicked_(item, 0)` → `composite->isSelected() ? deselect() : select()`, then `item->setData(0, Qt::ForegroundRole, …)` for immediate repaint, then `setSelection_(true)`.
6. Keep `setSelection_()` (line ~1054) — repurpose it to force foreground-role repaint after external selection changes (scene-picking, selector-expression apply, etc.).
7. Explicitly clear column 2 in `QStringList sl;` at line 1264 (codex caught this dangling Type construct).
8. **DO NOT change `getMolecularControlSelection()` contract** — it still tracks Qt row highlight. The Inspector adapter contract stays intact. Document the dual-meaning in code comments.
9. A11y: keep `setAccessibleDescription` for the row, since the visual checkbox cue is gone.

**Codex critique (verbatim highlights):**
- *Medium risk.* "Do not conflate `getSelection()` / `getMolecularControlSelection()` with `Composite::isSelected()`."
- "`QTreeWidgetItem::data()` cannot use `palette()` directly. Use `treeWidget()->palette().highlight().color()`."
- "`itemClicked` is too narrow. Keyboard activation, spacebar, programmatic selection, context-menu select/deselect, selector expression, scene picking, and `NewSelectionMessage` all need consistent visual refresh."
- "`setFlags(Qt::ItemIsUserCheckable | ...)` must remove `ItemIsUserCheckable`; otherwise Qt may still expose checkable semantics."
- "Accessibility regression: replacing a checkbox with foreground color removes a standard selectable control affordance."

---

### UFG-08 — WelcomeScreen blank on first launch

**Symptom (user):** "default molecule selection is no longer visible" + "Select/deselect are no longer accessible" on first launch.

**Root cause:** Three-way: (a) `setUnifiedTitleAndToolBarOnMac(true)` ran in `Mainframe::show()` AFTER ctor's `setCentralWidget(welcome_screen_)` (`mainframe.C:142` originally) — NSWindow style-mask recomposition raced central widget first paint; (b) `WelcomeScreen` was transparent QWidget, Scene's `QOpenGLWidget` backing surface bled through the 720-px centered column; (c) `takeCentralWidget()` returned prev unparented but didn't `hide()`, and WelcomeScreen was never `raise()/update()`'d.

**Code surface modified:**
- `source/VIEW/WIDGETS/welcomeScreen.C:107-160` — `WA_OpaquePaintEvent` + `setAutoFillBackground(true)` + explicit `paintEvent` fillRect.
- `source/APPLICATIONS/BALLVIEW/mainframe.C:141-143` — `setUnifiedTitleAndToolBarOnMac(true)` moved into ctor BEFORE any `setCentralWidget`.
- `source/APPLICATIONS/BALLVIEW/mainframe.C:620-672` — `prev->hide()` + `welcome_screen_->show()/raise()/update()` in `showWelcomeScreen_`; symmetric in `hideWelcomeScreen_`.

**Cross-cuts:** UFG-09, UFG-10 share the WA_OpaquePaintEvent / setAutoFillBackground paint-hygiene pattern. UFG-02's `setUnifiedTitleAndToolBarOnMac` move is the load-bearing piece for UFG-08; do NOT revert.

**Fix scope:** Small, ~30 LOC across 2 files. Already landed.

**Fix recipe:** N/A — landed.

**Codex critique:** "No blocking edge case found. The fix addresses the three likely failure modes coherently." Risk **low to medium-low**. Caveats: (a) paint path bypasses any future QSS background on `#welcomeScreen`; (b) sample thumbnails resolve synchronously w/ placeholder fallback — no race; (c) document-load-before-first-paint shows transient welcome flash possible but persistent blank unlikely; (d) **smoke-test on macOS + Linux Wayland/X11 + Windows** before final tag — central-widget swapping around `QOpenGLWidget` is platform-sensitive.

---

### UFG-09 — Inspector tab duplication "Selecti | Selection | Representation | Scene"

**Symptom (user):** Initial empty-state Inspector shows Selection tab visible twice (once truncated as "Selecti", once full).

**Root cause:** Paint artifact. Sequence: (1) InspectorView first-paints at dock layout-default width (briefly under UFG-04's 280-px min); (2) QTabBar elides `Selection`→`Selecti`; (3) dock resize-to-min settles a frame later; QTabBar repaints full `Selection`; (4) both `InspectorTabs` AND `InspectorView` were transparent — BEFORE-resize "Selecti" pixels in the inter-tab gap were never cleared.

**Code surface modified:**
- `source/VIEW/WIDGETS/inspector/inspectorTabs.C:35` — `setAutoFillBackground(true)`.
- `source/VIEW/WIDGETS/inspector/inspectorView.C:101` — `setAutoFillBackground(true)` on root.

**Cross-cuts:** UFG-04 (the 280px minimum), UFG-05 (SectionHeader paint pattern), UFG-10 (same family).

**Fix scope:** Trivial, 2 lines. Already landed.

**Fix recipe:** N/A — landed.

**Codex critique:** **Low functional risk, medium-low regression risk.** Two important facts: (a) `setDrawBase(false)` is already set on InspectorTabs, so `PE_FrameTabBarBase` is NOT a leak path; (b) `setAutoFillBackground(true)` should be sufficient because Qt fills with `QPalette::Window` before paintEvent. Codex *recommended NOT* upgrading to `WA_OpaquePaintEvent` because that requires an explicit `paintEvent` override (the current code keeps the inherited `QTabBar::paintEvent`). Residual risk: future QSS background/border-image rules can silently disable `autoFillBackground` — keep theme.qss free of `QTabBar { background: … }` selectors.

---

### UFG-10 — Inspector header garble "QUICK AC" / "REPRESENTATION" ghosts

**Symptom (user):** Top of Scene + Representation tabs shows `QUICK AC` + percent fragments + ghosted text overlapping the section header during expand/collapse animation.

**Root cause:** UFG-05 fixed `SectionHeader` but left three uncovered surfaces: (a) `InspectorBody` — gaps between sections during `InspectorSection::onHeaderToggled_` QPropertyAnimation on maximumHeight; (b) per-tab `QScrollArea` viewport — defaults transparent on macOS Qt 5; (c) inner contents widget — 8-px outer margin surfaces stale pixels.

**Code surface modified:**
- `source/VIEW/WIDGETS/inspector/inspectorBody.C:37` (`InspectorBody`), `:65` (`QScrollArea`), `:66` (viewport), `:76` (contents widget) — 4-layer `setAutoFillBackground(true)`.

**Cross-cuts:** UFG-05 SectionHeader pattern; UFG-09 root-fill.

**Fix scope:** Trivial, 4 lines. Already landed.

**Fix recipe:** N/A — landed.

**Codex critique:** **Medium-low residual risk.** "Directionally correct but not as strong as the UFG-05 `SectionHeader` fix" — `autoFillBackground` relies on Qt's invalidation being complete during fast animation. If ghosts persist, harden `InspectorBody` and inner contents widget with the explicit `WA_OpaquePaintEvent + paintEvent fillRect` pattern (do NOT propagate to `QScrollArea` itself — scroll areas have their own viewport/update behavior; only the viewport+contents are safe to opacify). **Action item:** regression-test by rapidly toggling `Quick Actions` and `Representation` sections while also switching Workspace presets — that's the failure-mode codex sees as the only remaining test gap.

---

### UFG-11 — Scene tab Controllers stub (Camera/Light/Stereo/Background)

**Symptom (user):** Changing Background colour, Lights (Ambient slider), Stage settings, Stereo settings in the Scene tab does NOT propagate to rendered scene.

**Root cause:** Per UFG-03 audit, the 5 Scene-tab Controllers were log-only STUBS at Phase 999.44 land time. Only StageController (Plan 06) was cut over.

**Code surface modified:**
- `source/VIEW/KERNEL/controllers/cameraController.C:66-111` — writes `viewPoint`+`lookAtPosition` via `Camera` setters + `Scene::update()`.
- `source/VIEW/KERNEL/controllers/lightController.C:77-142` — upserts AMBIENT LightSource via `clearLightSources/addLightSource` + `Scene::lightsUpdated(true)`.
- `source/VIEW/KERNEL/controllers/stereoController.C:62-90` — writes eye/focal/swapSBS via `Stage` setters + `Scene::update()`.
- `StageController` already cut over (covers Background via shared accessor).

**Cross-cuts:** UFG-12 (revert hook on tab-show); ClippingController + LabelController still STUB but have no UI consumer (no `ClippingSection` / `LabelSection` in `inspectorView.C:347/386`).

**Fix scope:** Medium, ~300 LOC across 3 files. **Already landed** as 3 commits: `bc601a0ac8` (camera), `7ae6bc2b70` (light), `c9d6267405` (stereo).

**Fix recipe:** N/A — landed.

**Codex critique (THE ONE LOAD-BEARING FINDING):**
> **High / RC3-blocker if the Scene tab ships as editable:** `StereoSection` exposes an `Enabled` checkbox at `stereoSection.C:53`, writes it at `:92`, but `StereoController::apply()` deliberately ignores `enabled_`. **User can check "Enabled" and the rendered scene does not enter stereo mode.** Best RC3 fix: hide or disable the checkbox until renderer-mode wiring lands. Do not ship a live-looking checkbox that is mirror-only.

Camera degeneracy (near-parallel up-vector) and Light snapshot-clear-readd race are both flagged as low/medium latent risks not RC3-blocking. ClippingController/LabelController stubs are non-blocking (no UI consumer).

**ACTION — small follow-up fix needed:** In `stereoSection.C:53` (or `stereoSection` ctor), call `enabled_checkbox_->setEnabled(false)` + tooltip "Stereo mode wiring lands in v1.7.1 — use Display › Stereo menu". Or hide the checkbox entirely with `setVisible(false)`. ~5 LOC.

---

### UFG-12 — Scene tab default values not populated on tab-switch

**Symptom (user):** Background colour picker shows blank/default when user switches TO Scene tab — not the current scene's actual background.

**Root cause:** Controllers were constructed once with post-construct defaults (background=black, ambient=0.3, eye/focal=0) and never resynced from live Stage when user switched to Scene tab.

**Code surface modified:**
- `source/VIEW/WIDGETS/inspector/inspectorView.C:196-228` — `onTabChanged_` now calls `revert()` on Scene-tab controllers when `tab==Scene`, on Representation-tab controllers when `tab==Representation`.

**Cross-cuts:** UFG-11 (the controllers must have functional `revert()` impls — Camera/Light/Stereo all pull from live Stage in their `revert()` methods).

**Fix scope:** Trivial, ~30 LOC. Already landed.

**Fix recipe:** N/A — landed.

**Codex critique:** **Medium risk.** Three gaps:
1. **External Scene mutations still go stale while already on Scene tab.** Legacy `StageSettings`/`LightSettings`/scripts mutate `Stage` directly; `Stage` exposes no change signal. If user sits on Scene tab and an external mutation happens, Inspector stays stale until they tab away and back.
2. **`revert()` can feed back into debounced `apply()`.** `LabeledSlider::setValue()` and `SwatchButton::setColor()` emit normal user-change signals (`labeledSlider.C:104`, `swatchButton.C:46`). Section handlers debounce → `apply()` → can write quantized-back values (0.333 → 0.33 round-trip). For `LightController` this is especially nontrivial (clears/re-adds lights). **Recommended hardening (post-RC3):** wrap controller-to-widget updates in `QSignalBlocker` or add silent setters.
3. **First-paint on saved Scene-tab launch:** `loadState()` calls `setCurrentTabIndex()` before Scene controllers exist (`inspectorDock.C:54` + `mainframe.C:319` ordering). If saved tab is Scene, `currentChanged` may fire too early. Constructor's `revert()` saves it, but only if Stage is final at attach time.

**ACTION — none for RC3** if user's UFG-12 symptom (blank-on-tab-switch) is gone; the debounce-feedback hardening is a v1.7.1 polish item.

---

### UFG-15 — Add Hydrogens toolbar action doesn't propagate (REDIRECT)

**Symptom (user):** Clicking the toolbar "Add hydrogens" button does not add hydrogens to the rendered molecule. Scene unchanged.

**Root cause (REVISED after codex review):** My initial hypothesis ("Representation rebuild missing → add explicit `rep->update(true)`") is **WRONG**. Codex verified: `Scene::saturateWithHydrogens()` calls `getMainControl()->update(*ac, true)` → `MainControl::update(Composite&, bool)` at `mainControl.C:1620` → `updateRepresentationsOf(composite.getRoot(), true, true)` → forced `rep->update(true)` per `mainControl.C:761`. The rebuild path is intact.

**Actual likely root causes (per codex):**
1. **Toolbar and menu use DIFFERENT add-hydrogen algorithms.** Toolbar (`scene.C:1558`) → `Scene::saturateWithHydrogens` → uses `AddHydrogenProcessor`. Menu (`molecularStructure.C:130`) → `MolecularStructure::addHydrogens` → uses `getFragmentDB().add_hydrogens` + `getFragmentDB().build_bonds`. **If the user expects the menu behavior, the toolbar is semantically wrong.** "Saturate with Hydrogens" (the toolbar wording) vs "Add Hydrogens" (the menu wording) hints at intent divergence.
2. **Enable-gate mismatch.** Toolbar enable predicate (`scene.C:1605-1618`) requires exactly one selected `System` OR `Molecule`. But `Scene::getContainers()` can operate on any highlighted `AtomContainer` (residues, chains) and falls back to all composites. The narrower gate may suppress the action precisely when user expects it.
3. `only_highlighted_ = true` default is NOT the propagation bug — `deselect(false)` only clears Scene's `current_atom_`/`current_bond_`, not MolecularControl selection.

**Code surface to modify (revised plan):**
- `source/VIEW/WIDGETS/scene.C:1558` — change `connect(add_hydrogens_action_, ..., this, SLOT(saturateWithHydrogens()))` to route through `MolecularStructure::addHydrogens` to match menu behavior (preferred for user-perceived parity).
  - OR keep slot, rename action label/tooltip to clarify "saturate vs add hydrogens".
- `source/VIEW/WIDGETS/scene.C:1605-1618` — relax enable predicate to allow any selected `AtomContainer` (not only `System`/`Molecule`) IF we keep `saturateWithHydrogens` wired.

**Cross-cuts:** UFG-07 (selection contract — if MolecularControl row-select is the gate, the new "click-name-to-toggle" UX must update `getMolecularControlSelection()` correctly).

**Fix scope:** Small, ~30-80 LOC. Add a verification trace (atom count before/after + `RepresentationManager::getRepresentationsOf(ac->getRoot()).size()`) to confirm the diagnosis empirically before committing.

**Fix recipe (revised):**
1. **Diagnose first.** Add temporary `Log.info()` to `Scene::saturateWithHydrogens` printing `ac->countAtoms()` before/after `AddHydrogenProcessor` + `RepresentationManager::getRepresentationsOf(&ac->getRoot()).size()`.
2. If atom count grows but reps don't rebuild → confirms a different bug (CompositeManager not catching the change message). Investigate `MainControl::update` further.
3. If atom count stays flat → `AddHydrogenProcessor` is the wrong processor for the user's input (e.g. trying to add H to a residue that already has them via different chemistry).
4. **Most likely fix (high confidence):** rewire the toolbar action's `triggered()` slot to call `MolecularStructure::getInstance(0)->addHydrogens()` (the menu's slot) so toolbar and menu have identical behavior. This is the "match user expectation" path.
5. **Also fix enable predicate** (`scene.C:1605-1618`) — relax to allow `AtomContainer` (not just `System`/`Molecule`) since the menu version `MolecularStructure::addHydrogens` iterates `getMolecularControlSelection()` and applies to anything.

**Codex critique:** **Medium risk** if only changing toolbar wiring/enablement (different algorithms). **Low** for adding instrumentation. **Medium-high** for blindly forcing `rep->update(true)` — "papers over the wrong layer." 

**IMPORTANT for orchestrator:** my original UFG-15 fix recipe was wrong. Dispatch the UFG-15 agent with the **diagnose-first** instruction above.

---

### UFG-16 — Add Hydrogens icon (`atom.svg`) is unclear

**Symptom (user):** "atom w/ electrons?" — the atom-glyph doesn't communicate "add hydrogens to molecule".

**Root cause:** `actions/molecule-add-hydrogens` alias maps to `atom.svg` in `source/VIEW/KERNEL/theme/theme.qrc`. The atom glyph reads as "atom/molecule" not "additive operation."

**Code surface to modify:**
- `source/VIEW/KERNEL/theme/icons/lucide/` — vendor `plus-circle.svg` from Lucide v0.460.0 (codex caught: NOT currently vendored, only `atom.svg` and `plus.svg` are).
- `source/VIEW/KERNEL/theme/theme.qrc` — change alias to `plus-circle.svg`.
- `source/VIEW/KERNEL/theme/LICENSE-lucide.txt` — append provenance entry.
- `source/APPLICATIONS/BALLVIEW/iconBrowser.C` — keep entry in sync.

**Cross-cuts:** UFG-15 (same action's behavior). Fix the icon AND the action behavior in one wave.

**Fix scope:** Trivial, ~4 files, <20 LOC.

**Fix recipe:** Vendor Lucide `plus-circle.svg` → update qrc alias → update LICENSE provenance. Defer "composed atom + plus badge" SVG to v1.7.1 polish.

**Codex critique:** **Minor**, low implementation risk, moderate UX clarity risk if left as `atom.svg`. Use `plus-circle.svg` for RC3. `flower-2.svg` is wrong (decorative, doesn't communicate hydrogen saturation). Composed atom+plus is polish-correct but adds avoidable RC3 surface area.

---

### UFG-17 — Logs drawer rendering broken after expand

**Symptom (user):** After clicking the Logs strip: 3 stacked "Logs/Clear/Select/Logs" rows + a floating "Logs" panel with truncated text "Log Fi context) 0 (unknown function)". Expected expanded 240-px drawer NOT visible.

**Root cause:** `source/APPLICATIONS/BALLVIEW/mainframe.C:270-277` — `LogView` and `FileObserver` are `addDockWidget()`'d as actual bottom-area `QDockWidget`s, THEN `BottomDrawer` is also `addDockWidget()`'d AND reparents `log_view->widget()` into its own `QStackedWidget` (`bottomDrawer.C:140-167`) with `log_view_->hide()` only at construct time. When the drawer expands (UFG-06 fix: `animateTo` + `resizeDocks` + maximumHeight ladder), Qt's bottom-dock-area layout reasserts the outer LogView dock — producing the floating "Logs" panel + 3 stacked rows. WorkspaceManager's "hide every dock not in preset descriptor" loop (`workspaceManager.C:205-211`) hides them on `apply()`, but the BottomDrawer expand path doesn't re-apply the workspace.

**Code surface to modify:**
- `source/APPLICATIONS/BALLVIEW/mainframe.C:270-277` — after `BottomDrawer` ctor extracts the inner widgets, call `removeDockWidget(log_view)` + `removeDockWidget(file_obs)` + `log_view->hide()` + `file_obs->hide()`. Keep ModularWidget registration intact (the message-bus parent chain is independent of dock layout membership).
- Audit Windows-menu `toggleViewAction()` for LogView/FileObserver — they may still appear in the Window menu and resurrect stale shells.

**Cross-cuts:** UFG-06 (the underlying expand fix that surfaced this).

**Fix scope:** Small, ~10-20 LOC. Risk **high UI regression** if removeDockWidget is wrong; **low message-bus risk** per codex.

**Fix recipe (per codex — option B):**
```cpp
// mainframe.C ~line 270 region
addDockWidget(Qt::BottomDockWidgetArea, log_view);
addDockWidget(Qt::BottomDockWidgetArea, file_obs);

BottomDrawer* drawer = new BottomDrawer(log_view, file_obs, this);

// UFG-17 — strip the stale outer dock layout participation but keep
// the ModularWidget registration + message-bus parent chain intact.
removeDockWidget(log_view);
removeDockWidget(file_obs);
log_view->hide();
file_obs->hide();

addDockWidget(Qt::BottomDockWidgetArea, drawer);
```
Also suppress the legacy `toggleViewAction()` entries for LogView/FileObserver in the Window menu (audit `DockWidget::initializeWidget()`).

**Codex critique:** Option **B** is safest. Option A (skip `addDockWidget` entirely) breaks more legacy assumptions. Option C (re-hide in `setExpanded`) is a brittle band-aid. **`removeDockWidget()` does NOT break `LogView::receiveMessage()`** — message bus is driven by `MainControl::addModularWidget()` (in LogView ctor's `registerWidget(this)`), not by `QMainWindow` dock membership. The `ConnectionObject` parent chain is independent of QObject widget parenting.

---

## Dependency graph

```
UFG-08 (WelcomeScreen)   ──┐
UFG-09 (tab dup)         ──┼── Inspector chrome paint hygiene
UFG-10 (header garble)   ──┘    (UFG-05 SectionHeader pattern shared root)
                                 + setUnifiedTitleAndToolBarOnMac timing for UFG-08
                                 → ALL LANDED; verify on rc3 build

UFG-11 (Scene controllers)  ──┬── Scene-tab Inspector wiring
UFG-12 (defaults on switch) ──┘    (controllers' revert() drives the tab refresh)
                                    → LANDED; 1 small Stereo-checkbox follow-up needed

UFG-15 (add-H toolbar) ── UFG-16 (add-H icon) → same toolbar action,
                                                 fix in one wave

UFG-17 (Logs drawer) ── independent (BottomDrawer + mainframe wiring)
                         depends on UFG-06 already-landed expand fix

UFG-07 (selection redesign) ── independent of all above
                                 BUT shares MolecularControl ↔ Inspector
                                 selection-bus contract → verify
                                 SelectionInspectorAdapter still works
```

## Fix-wave grouping

- **Wave A — VERIFY (no new code).** UFG-08 + UFG-09 + UFG-10 are landed. Verify on RC3 build: macOS-arm64 first-launch (blank WelcomeScreen check), Inspector tab strip on empty state (no "Selecti" ghost), Inspector during rapid section expand/collapse (no "QUICK AC" / "REPRESENTATION" ghosts), AND switch Workspace presets while toggling sections (codex's only flagged uncovered case). Also smoke-test on Linux Wayland/X11 + Windows (codex flagged `QOpenGLWidget` central-widget swap is platform-sensitive).
- **Wave B — STEREO ENABLED checkbox.** Single small fix: in `stereoSection.C` ctor disable or hide the `Enabled` checkbox + add tooltip pointing to legacy `Display › Stereo` menu. ~5 LOC. RC3-blocker per codex.
- **Wave C — UFG-15 + UFG-16 (Add Hydrogens).** REDIRECT REQUIRED. Dispatch with revised plan: diagnose-first (atom count + rep count trace), most likely fix is to rewire toolbar slot to `MolecularStructure::addHydrogens` (menu's slot) for parity. Combine with UFG-16 icon swap (`atom.svg` → `plus-circle.svg`).
- **Wave D — UFG-17 (Logs drawer).** Apply codex's option B: `removeDockWidget(log_view)` + `removeDockWidget(file_obs)` + `hide()` immediately after BottomDrawer ctor. Audit Window-menu `toggleViewAction()` for stale entries.
- **Wave E — UFG-07 (selection redesign).** Largest scope, independent. Apply revised recipe with explicit attention to: (a) BALL composite-selection vs Qt row-highlight contract separation, (b) `treeWidget()->palette()` (not `palette()`), (c) accessibility + tooltip cue replacement for the lost checkbox affordance.

Waves B/C/D/E can run in parallel — they touch disjoint files.

## In-flight agent assessment

**Agent A (UFG-08+09+10):** **ON TRACK — closing successfully.** All three commits landed (`7297b0ea75`, `80dca4d3d7`, `c14428d98b`). Codex's review of each confirms the fix is directionally right. No redirect needed. **Suggested follow-up only:** Wave A verification on macOS + one Linux variant + Windows.

**Agent B (UFG-11+12):** **MOSTLY ON TRACK — one significant gap.** Camera/Light/Stereo `apply()` cut-overs landed (`bc601a0ac8`, `7ae6bc2b70`, `c9d6267405`). UFG-12 `onTabChanged_` revert hook landed at `inspectorView.C:196-228`. **GAP: codex flagged the Stereo `Enabled` checkbox is wired in the section but `StereoController::apply()` deliberately ignores `enabled_`. The user can check "Enabled" and stereo won't engage — visibly broken.** RC3-blocker. Either:
  - **Redirect agent B (preferred):** add a small follow-up commit that disables or hides the Stereo `Enabled` checkbox in `stereoSection.C` with a tooltip pointing at `Display › Stereo` menu.
  - Or accept the gap and document as known issue in RELEASE-NOTES (less preferred — user already burned by similar mirror-only behavior in UFG-03).

Both agents' debounce/revert reentrancy (codex's UFG-12 gap #2) is a **v1.7.1 hardening item** — not RC3-blocking unless verification surfaces visible drift.

## RC3 readiness checklist

- [ ] **UFG-07** — Single-column tree shipped, name highlight visible on selected items, checkbox gone, Inspector `selection` tab continues to populate on row-highlight (BALL vs Qt selection contract intact).
- [x] **UFG-08** — First-launch macOS shows WelcomeScreen with 6 sample cards visible; smoke-test Linux + Windows.
- [x] **UFG-09** — Empty-state Inspector shows `Selection | Representation | Scene` (no "Selecti" ghost) at default dock width.
- [x] **UFG-10** — Rapidly toggling QuickActions + Representation sections produces NO ghosted header text fragments; verify also during Workspace preset switch.
- [x] **UFG-11** — Background swatch + Ambient slider + eye/focal distance changes propagate to rendered scene; **Stereo `Enabled` checkbox is disabled or hidden** (small follow-up commit required).
- [x] **UFG-12** — Switching to Scene tab populates Background / Ambient / eye/focal with current Stage values (not blank defaults).
- [ ] **UFG-15** — Clicking toolbar Add Hydrogens visibly adds H atoms to a loaded molecule (1ubq sample); enable-gate fires for any highlighted AtomContainer.
- [ ] **UFG-16** — Toolbar icon for Add Hydrogens is `plus-circle.svg` (clearer additive semantic).
- [ ] **UFG-17** — Clicking Logs drawer chevron expands to 240-px panel; no floating "Logs" panel; no stacked rows.

(Boxes marked [x] are landed-on-disk; verification on the RC3 artifact still required before final tag.)

## Risk register

- **R1 — Wave-A regressions on non-macOS.** All 3 in-flight fixes are paint-hygiene + macOS-specific timing. Linux X11/Wayland + Windows untested per codex. *Mitigation:* RC3 release.yml must produce Linux + Windows artifacts, and user must smoke-test on at least one Linux variant before final tag.
- **R2 — UFG-15 algorithmic divergence.** Toolbar `saturateWithHydrogens` and menu `addHydrogens` use different chemistry (`AddHydrogenProcessor` vs `FragmentDB::add_hydrogens + build_bonds`). Whichever wins, RELEASE-NOTES should document the behavior change. *Mitigation:* fix-agent must add a smoke-test sample (e.g. 1ubq with stripped H) verifying both menu and toolbar produce the same atom-count delta.
- **R3 — UFG-17 Window menu toggle resurrect.** `DockWidget::initializeWidget()` may still register `toggleViewAction()` for LogView/FileObserver after `removeDockWidget()`. *Mitigation:* fix-agent must audit Window menu post-fix; suppress legacy toggles if they persist.
- **R4 — UFG-07 selection contract bleed.** Codex flagged the dual-meaning (`Composite::isSelected()` vs Qt row-highlight). If the fix-agent silently merges them, SelectionInspectorAdapter may stop firing correctly. *Mitigation:* the recipe explicitly preserves `getMolecularControlSelection()` semantics; reviewer must verify the adapter test (`tabOrder_test` or equivalent) still passes.
- **R5 — UFG-12 debounce-feedback drift.** Codex flagged that `revert()` can feed back through `LabeledSlider`/`SwatchButton` change-signals → debounced `apply()` → quantized writeback. Not RC3-blocking, but if verification observes slider creep on rapid tab-switch, escalate. *Mitigation:* v1.7.1 hardening item to wrap controller-to-widget setters in `QSignalBlocker`.
- **R6 — UFG-11 Stereo Enabled mirror.** If we ship without disabling the checkbox, user will check it, observe nothing, and re-file. *Mitigation:* MUST land the 5-LOC follow-up before RC3 tag.
- **R7 — Welcome → Scene swap GL flicker.** Codex noted central-widget swap around `QOpenGLWidget` is platform-sensitive. *Mitigation:* if any platform shows persistent flash, add a 1-frame deferred `Scene::update()` in `hideWelcomeScreen_` post-`setCentralWidget`.
- **R8 — `Assert GL compat profile` (D-06 / D-08) regression on macOS + Linux headless CI.** First observed on CI run `26034765141` (HEAD `b0b035185b`) after the Wave-B/C/D/E commits landed. **`render-smoke-check.sh` still times out at 60 s on the headless macos-arm64 + linux-x64 runners (same as the prior successful run), but BALLView now fails to emit a `BALLVIEW_GL_DIAG` line before the kill** — meaning the GL context is never created in time. The previous successful run on `f03b7cd4eeb6` (UFG-06 fix) DID emit `gl_profile=none` from the software renderer within the 60 s window. *Likely culprits in the change-window:* UFG-08 (`setUnifiedTitleAndToolBarOnMac` timing in mainframe ctor), UFG-17 (`removeDockWidget(log_view) + removeDockWidget(file_obs)` post-BottomDrawer-ctor — alters QMainWindow geometry computation during the show path), or a cumulative interaction. UFG-07 (this Wave) is *not* a credible culprit: `MolecularControl` ctor only flips column count and header visibility; tree items are populated AFTER MainFrame::show and AFTER QSurfaceFormat negotiation. *Mitigation:* (a) bisect by reverting UFG-17 and UFG-08 one-at-a-time on a CI-only branch; (b) consider bumping the render-smoke timeout from 60 → 120 s as a stop-gap if the change is genuine "BALLView startup is now slower" rather than "BALLView hangs"; (c) verify the spike-smoke (GLCore backend) job emits its own diag line — if it does, the regression is `QSurfaceFormat = compat`-path specific (i.e. the legacy GLRenderer code-path in MainFrame::initialize).

---

**End of plan.** Total wave-time estimate: ~1 day for Waves B/C/D + 0.5–1 day for Wave E in parallel, plus Wave A verification (~2 hours on each platform). RC3 readiness ETA: 1 working day if Wave-A verification finds no platform regressions.
