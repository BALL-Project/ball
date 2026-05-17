# BALLView v1.7 — BALLView Refresh (DRAFT)

**Status:** DRAFT — populated incrementally as v1.7 Wave 4 phases land. Will be finalized at v1.7-RC1 tag time.

**v1.7 is the "BALLView Refresh" milestone.** It re-skins, re-arranges, and modernizes the BALLView GUI on top of the v1.6 rendering + build foundation, without changing the underlying molecular-modelling library surface. Net visual + UX changes are substantial; net behavioral changes to scripted/non-GUI workflows are zero.

If you're a non-GUI consumer of `libBALL` / `libVIEW`, **v1.7 is a transparent upgrade** — same APIs, same headers, same .so/.dylib SONAMEs. The Refresh lives entirely above the rendering-boundary contract pinned in Phase 02.1.

---

## What's new in v1.7

(*Sections to be expanded as Wave 4 phases close. Headline list:*)

- Neutral single-theme palette (Phase 999.40) + theming-palette removal (Phase 999.42)
- Hi-DPI SVG icon set replaces the legacy XPM pixmap tree (Phase 999.41)
- Unified Inspector dock replaces Display/Model/Material modal dialogs (Phase 999.44)
- 3-preset workspace consolidation + status-bar workspace label (Phase 999.45) — *see also "Removed in v1.7" below*
- Refreshed menus + command palette (Ctrl+K / ⌘K) (Phase 999.46)
- First-run onboarding (Phase 999.47)
- Accessibility pass: QAccessible names, focusable controls, WCAG-AA contrast scorer in CI, tooltip-lint gate (Phase 999.48)
- `BALL_UI_V2` build flag retired — the refreshed UI is now the only UI; OFF-cell matrix axis dropped from CI (Phase 999.48)

---

## Removed in v1.7

### The legacy 5-dock "Classic" workspace preset (Phase 999.49)

Per the maintainer-Q2 commitment captured at the v1.6 → v1.7 boundary ("v1.X deletes Classic"), the Classic 5-dock workspace preset has been removed in v1.7. Users previously on Classic are **silently auto-migrated to the refreshed Default workspace** on first launch with v1.7. User-saved layouts (UserDefined presets stored under `[Workspace]/userDefined__*` in `BALLView.ini`) and the Focused preset are unaffected. The workspace picker (`View → Workspace` and `Window → Workspace Preset`) now lists **Default + Focused** (+ any UserDefined presets).

If you want a custom dock arrangement, rearrange the docks to your liking and use `View → Workspace → Save current as…` to persist it as a UserDefined preset.

The auto-migration is a one-shot rewrite of the `[Workspace]/currentPreset` INI key — `"Classic"` → `"Default"`. It is idempotent (subsequent launches no-op) and logs a single info-level message (`v1.7 migration: legacy 5-dock workspace preset retired — falling back to Default.`) the first time it fires.

**Source-level removal:**
- `WorkspaceManager::Classic` enum value (`include/BALL/VIEW/KERNEL/workspaceManager.h`) — deleted. Numeric values of the remaining presets are preserved (Default=0, Focused=2, UserDefined=3); the value `1` is permanently retired and must not be reused by future presets.
- `data/BALLView/workspaces/classic.layout` — deleted.
- All `case Classic:` switch branches and `addPresetAction(Classic)` call sites — deleted.
- The Phase 999.45 first-run prompt's two-button "Try new / Keep my layout (Classic)" dialog simplified to a one-button informational notice.

---

## Migration notes

### For end users

- **First launch with v1.7:** if you were on Classic, you'll see the refreshed Default workspace. No action required.
- **First launch with v1.7 from a fresh install:** you'll see the refreshed Default workspace and a one-time informational notice about the workspace refresh.
- **If you want a 5-dock-like layout back:** drag docks (`MolecularControl`, `GeometricControl`, `DatasetControl`, `LogView`, `FileObserver`) to your preferred locations, then `View → Workspace → Save current as…` to save under any name.

### For maintainers / external integrators

- The `BALL::VIEW::WorkspaceManager::Classic` enum value is gone. External code referencing it will fail to compile with a `'Classic' is not a member of 'BALL::VIEW::WorkspaceManager'` diagnostic. Migrate references to `BALL::VIEW::WorkspaceManager::Default`.
- The `Preset` enum is no longer dense — value `1` is now a hole. Switch statements over `Preset` should not assume contiguity. Compiler-driven completeness checks (`-Wswitch`) catch any missed branches at build time.

---

*Draft created 2026-05-17 during Phase 999.49. Sections marked "to be expanded" will be filled in as v1.7 Wave 4 phases finalize and at RC1 tag time.*
