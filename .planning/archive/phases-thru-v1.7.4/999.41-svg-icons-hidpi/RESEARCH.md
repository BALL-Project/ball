---
phase: 999.41
plan: research
type: research
created: 2026-05-17
handover_source: /Users/kohlbach/Claude/BALL/Claude Design Handover/revitalization/02-phase-icons.md
roadmap_phase: 999.41
---

# Phase 999.41 — SVG icons + HiDPI fixup (Handover Phase 2): Research

## 1. Spec source

`02-phase-icons.md` (Handover Phase 2). Goal: replace the PNG/XPM icon
pipeline with SVG so toolbar/dock chrome renders crisply on Retina/4K
and gains a clean tinting boundary.

## 2. Real-world ground truth (vs. spec)

The spec was written against a half-imagined directory layout; reality
diverges in three places that change the plan shape:

| Spec assumed                                              | Repo actually has                                                |
| --------------------------------------------------------- | ---------------------------------------------------------------- |
| `source/APPLICATIONS/BALLVIEW/images/` for PNG icons      | `data/graphics/icons/{16x16,22x22,scalable}/{actions,categories,mimetype}/*.png` (and a handful of `.svg` already in `scalable/actions/`) |
| `IconLoader::getIcon()` is the only call surface          | Confirmed — 28 call sites in BALLView + VIEW + 4 EXTENSIONS plugins (see §3) |
| Per-icon QSS tinting via `currentColor` on hover/selected | Maintainer-Q3 = **single neutral theme only**; we ship one tint  |
| Replace every `IconLoader::getIcon(...)` call site        | Out of scope for v1.7 — too churny for a build-flag-gated change |
| Refresh `BALLView.icns`/.ico                              | Out of scope — packaging Phase 8 owns the app-icon refresh       |

The PNG inventory under `data/graphics/icons/22x22/actions/` is 27
files; `scalable/actions/` already contains 4 SVGs (carry-over from a
prior partial effort). We don't need ~80 icons — the v1.7 toolbar +
dock surface uses ~14 unique names.

## 3. Icon call-site census

`grep -rn "\.getIcon\|IconLoader::instance" source/` (excluding the
loader itself + plugin-API forwards in EXTENSIONS/):

| File                                            | Calls | Icons referenced |
| ----------------------------------------------- | ----- | ---------------- |
| `source/APPLICATIONS/BALLVIEW/mainframe.C`      | 6     | actions/view-fullscreen, view-restore, process-stop, quickopen-file, quicksave |
| `source/VIEW/WIDGETS/scene.C`                   | 6     | actions/measure, continuous-loop, molecule-assign-bond-orders, molecule-minimize, molecule-add-hydrogens |
| `source/VIEW/WIDGETS/propertyEditor.C`          | 4     | actions/list-add, edit-delete, edit-copy |
| `source/VIEW/WIDGETS/SDWidget.C`                | 1     | actions/document-save |
| `source/VIEW/DIALOGS/pluginDialog.C`            | 3     | actions/list-add, edit-delete |
| `source/VIEW/KERNEL/modularWidget.C`            | 1     | (dynamic, filename param) |
| `source/VIEW/KERNEL/mainControl.C`              | 1     | categories/preferences |
| `source/VIEW/KERNEL/MODES/editMode.C`           | 4     | actions/mode-edit, molecule-set-element, create-bond |

→ **14 unique action icons** + 1 categories icon. v1.7 only needs SVG
for the set that maps cleanly to Lucide; the rest stay PNG (already
present, the loader will fall back gracefully).

## 4. Loader-shim strategy (the v1.7 wedge)

Replacing 28 call sites under a build flag would require either
preprocessor cascades or two parallel APIs. Neither is worth it for one
release. Instead:

**`IconLoader::loadIcon_()` becomes resource-aware.**

```cpp
// New first-try path, only compiled under BALL_UI_V2:
//   try the Qt resource :/icons/<name>.svg
//   if found: return QIcon(":/icons/<name>.svg") cached, done
// Fallback (always compiled): existing PNG-from-disk walk.
```

This means:

- Zero churn at call sites. `IconLoader::instance().getIcon("actions/quicksave")`
  now picks the SVG when present and the flag is ON, otherwise the PNG.
- Plugins built against the old API keep working — the cache contract
  is unchanged (`const QIcon&` from a per-name cache).
- Qt 6's built-in `QSvgIconEngine` does the per-DPR rendering for us;
  no custom `QIconEngine` is needed in v1.7. The Handover spec's
  `ThemedIconEngine` (with per-DPR tint cache, mode-aware tint colors)
  is a v1.8 follow-up and is documented as deferred.

## 5. Tinting under the single-theme constraint

The Handover spec calls for `--ink-soft` / `--ink` / `--accent`
tinting via `QPainter::CompositionMode_SourceIn`. Maintainer-Q3
forces single-theme, so tinting reduces to "render the SVG as
authored". Lucide SVGs use `stroke="currentColor"` — at render
time Qt resolves that against the QSS `color:` property of the
hosting widget, so toolbar icons inherit the toolbar's chosen ink
color automatically without us needing a `QIconEngine`. This is
the simplest possible v1.7 implementation.

If maintainer-Q3 is later reversed and we need active per-mode
tinting, the `ThemedIconEngine` design from the spec is the path.
Captured as a v1.8 TODO in the loader header.

## 6. Toolbar icon size

`mainframe.C:295`:
```cpp
tb->setIconSize(QSize(22,22));
```

Becomes (under `#ifdef BALL_UI_V2`):
```cpp
tb->setIconSize(QSize(Theme::kIconToolbar, Theme::kIconToolbar));  // 20
```

`Theme::kIconToolbar` is already defined in `tokens.h` from 999.40.

## 7. XPM cleanup — what stays

The Handover spec wants `icons.{h,C}` deleted along with the
`bucky_64x64_xpm` and friends. v1.7 leaves these in place:

- `source/APPLICATIONS/BALLVIEW/icons.{h,C}` — `bucky_64x64_xpm`
  for `setWindowIcon` (mainframe.C:71)
- `source/VIEW/KERNEL/serverWidget.C` — `mini_ray_xpm_` (line 23)
- `source/VIEW/KERNEL/mainControl.C` — `simulation_running_xpm_`
  (line 87)

Removing these requires either an SVG app icon (Phase 8 packaging
refresh) or churn at the affected `setWindowIcon`/`QPixmap` call
sites. v1.7 leaves a `TODO(Phase 999.42 + Phase 8)` comment.

## 8. Lucide vendoring decision

- **Pin:** Lucide v0.460.0 (per spec). MIT license, single-file SVGs.
- **Storage:** `source/VIEW/KERNEL/theme/icons/lucide/` (sibling to
  `theme.qrc`, joins the existing theme QRC under a `/icons/` prefix).
- **Quantity:** 14 SVGs for v1.7 (one per unique BALLView toolbar/dock
  action key we'd actually want to upgrade). Not the full ~80 from the
  spec — anything not in the v1.7 wedge stays on its current PNG.
- **Authoring:** SVGs are stripped of the Lucide `<title>` and have a
  single-line license header comment so the diff stays scannable.

## 9. Mapping table (v1.7 subset)

| BALLView name                                  | Lucide source             | Notes                                |
| ---------------------------------------------- | ------------------------- | ------------------------------------ |
| actions/quickopen-file                         | folder-open               |                                      |
| actions/quicksave                              | save                      |                                      |
| actions/document-save                          | save                      | same SVG, aliased                    |
| actions/view-fullscreen                        | maximize                  |                                      |
| actions/view-restore                           | minimize-2                |                                      |
| actions/process-stop                           | octagon-x                 |                                      |
| actions/measure                                | ruler                     |                                      |
| actions/list-add                               | plus                      |                                      |
| actions/edit-delete                            | trash-2                   |                                      |
| actions/edit-copy                              | copy                      |                                      |
| actions/molecule-add-hydrogens                 | atom                      | closest visual match                 |
| actions/molecule-minimize                      | shrink                    |                                      |
| actions/molecule-assign-bond-orders            | link-2                    |                                      |
| categories/preferences                         | settings                  |                                      |

(`continuous-loop`, `mode-edit`, `molecule-set-element`, `create-bond`
left on PNG for now — no Lucide equivalent that's obviously better at
the v1.7 quality bar; revisit in 999.42+.)

## 10. CMake wiring

Already in place from 999.40:
- `OPTION(BALL_UI_V2 ... OFF)` at `CMakeLists.txt:289`
- `TARGET_COMPILE_DEFINITIONS(VIEW PUBLIC BALL_UI_V2)` at `:648`
- `QT6_ADD_RESOURCES(theme.qrc)` at `source/VIEW/KERNEL/sources.cmake:37`

To add in 999.41:
- `FIND_PACKAGE(Qt6 ${QT_MIN_VERSION} REQUIRED Svg)` inside the
  `IF(BALL_HAS_VIEW)` block.
- `LIST(APPEND VIEW_DEP_LIBRARIES Qt6::Svg)`.
- Extend `theme.qrc` with the icon files under a `/icons` prefix
  (single QRC keeps the resource graph simple).

## 11. Build verification

macOS-only (per orchestrator scope for Wave 4):
```sh
cmake --build build/ci-macos --target BALLView -j 8
```

Build dir at `build/ci-macos` was last configured against Qt 6.11 +
Homebrew Eigen + Boost; `BALL_UI_V2` is OFF by default — re-configure
ON for one verification round at the end.

## 12. Out of scope (forwarded)

- **999.42** (palette removal): the IconLoader PNG fallback path stays
  intact; this phase only adds a preferred SVG branch above it.
- **999.43** (simple-dialog cleanup): unaffected.
- **v1.8** ThemedIconEngine with tint cache + per-DPR composite +
  light/dark variants (post-Q3 reversal).
- **Phase 8** packaging: `.icns`/`.ico` regeneration, app icon refresh,
  XPM bucky removal.
