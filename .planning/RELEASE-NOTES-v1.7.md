# BALLView v1.7 — BALLView Refresh

**Status:** v1.7.0 shipped 2026-05-19 (commit `97d4c4d62d`). **v1.7.1**
(2026-05-21) is the first **signed + notarized macOS** release — Developer
ID + Apple notarization went live (Phase 8a). See "v1.7.1 patch cycle" below;
rc1 (`v1.7.1-rc1`) validated, final tag follows.

**v1.7 is the "BALLView Refresh" milestone.** It re-skins, re-arranges, and modernizes the BALLView GUI on top of the v1.6 rendering + build foundation, without changing the underlying molecular-modelling library surface. Net visual + UX changes are substantial; net behavioral changes to scripted/non-GUI workflows are zero.

If you're a non-GUI consumer of `libBALL` / `libVIEW`, **v1.7 is a transparent upgrade** — same APIs, same headers, same .so/.dylib SONAMEs. The Refresh lives entirely above the rendering-boundary contract pinned in Phase 02.1.

---

## What's new in v1.7

- Neutral single-theme palette (Phase 999.40) + theming-palette removal (Phase 999.42)
- Hi-DPI SVG icon set replaces the legacy XPM pixmap tree (Phase 999.41)
- Unified Inspector dock replaces Display/Model/Material modal dialogs (Phase 999.44)
- 3-preset workspace consolidation + status-bar workspace label (Phase 999.45) — *see also "Removed in v1.7" below*
- Refreshed menus + command palette (Ctrl+K / ⌘K) (Phase 999.46)
- First-run onboarding (Phase 999.47)
- Accessibility pass: QAccessible names, focusable controls, WCAG-AA contrast scorer in CI, tooltip-lint gate (Phase 999.48)
- `BALL_UI_V2` build flag retired — the refreshed UI is now the only UI; OFF-cell matrix axis dropped from CI (Phase 999.48)

---

## v1.7.4 patch cycle (2026-05-29) — Inspector Depth + the Controller Contract

The structural milestone of the v1.7.x line: every mutating BALLView control now
routes through a single command-shaped `Controller::apply()` (validate → guard →
mutate the one owner → emit one event → declared invalidation → record a
reversible payload). `Stage`/`Representation` owner setters are privatized behind
narrow `StageMutation`/`RepresentationBuilder` friends — external mutation no
longer compiles — so there is exactly one mutation path per domain. On top of the
contract: a coloring value-range **histogram** (drag handles + presets, render-only
drag preview) and a per-section **Reset**. New CI gates: contract-tests (Linux),
sip-refresh, opaque-paint, lifetime-discipline. Transparent to non-GUI consumers.
Full detail: [`RELEASE-NOTES-v1.7.4-DRAFT.md`](RELEASE-NOTES-v1.7.4-DRAFT.md).
Known issue: minor Inspector right-edge clipping when scrolled (cosmetic) →
v1.7.5. The deferred v1.7.3 legacy-dialog deletion remains on its own branch.

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

## v1.7.1 patch cycle

v1.7.1 is the post-ship stabilization cycle. All RC-era user-feedback
items (UFG-01 … UFG-22, UFG-24 … UFG-27) shipped fixed in v1.7.0. The
v1.7.1 patches address regressions and polish surfaced in post-ship
acceptance testing. Full per-item detail: `.planning/v1.7.x-PATCH-QUEUE.md`.

**Functional fixes**
- **Add Hydrogens toolbar button** now works again (was a dead toolbar
  action) — v1.7.x-10.
- **Energy Optimization toolbar button** now works again and honours the
  existing selection (optimizes the selected system) — v1.7.x-11/12.
- **Select / optimize no longer freeze the app** — the per-click full-scene
  rebuild + paint-storm that saturated the GUI thread is gone (diff-based
  selection sync) — v1.7.x-13/14.
- **Deleting a structure from the tree no longer crashes** (selection set
  no longer retains freed descendants) — v1.7.x-27.
- **Stuck "update is running" lock-out fixed** — a representation that
  failed to buffer could pin the busy-state forever, silently refusing
  selection + deletion and greying every toolbar action; the render lock is
  now always released — v1.7.x-29.
- **Selection mechanism** corrected: top-level-only selection (no
  descendant leakage), pre-existing selection is preserved (optimize no
  longer overwrites it), and a "expand selection to residues" quick action
  was added — v1.7.x-12/19.
- **Real RCSB sample structures** ship in place of the earlier placeholders.

**UI / accessibility**
- Inspector **Model** section gains type-specific controls (sphere/bond/
  probe/tube radii) — v1.7.x-16.
- Inspector **Coloring** section gains a per-method value range (min/max)
  for value-based colorings — v1.7.x-17.
- **Per-section reset-to-defaults** in the Inspector — v1.7.x-18.
- **Text-size accessibility preference** (Normal / Large / XL) — v1.7.x-21.
- **Tooltips + keyboard-shortcut hints** swept across actions — v1.7.x-22.
- **Disabled toolbar/menu icons** now render visibly greyed (38% opacity)
  so actionable vs unavailable is distinguishable — v1.7.x-28.
- The POVRay scene-export action is now intentionally icon-less.

**Internal / hardening** (no user-visible change)
- Controller `apply()` re-entrancy guard across all presentation
  controllers + a headless contract-test net (13 checks) — v1.7.x-24/25.

---

## v1.7.2 patch cycle

v1.7.2 is the BALLView **Inspector/UX patch cycle** — it finishes wiring the
v1.7 Inspector and lands the UX polish deferred during the v1.7.0 RC cycle:

- **Inspector Clipping + Label sections** — the `ClippingController` /
  `LabelController` cut-overs (Phase 999.51) — the Inspector now drives clipping
  planes and labels directly, not just the legacy dialogs.
- **Inspector material Transparency control** (Phase 999.52, closes GitHub #527)
  — a working transparency slider on the Material section, on the GL-honored
  `Representation::setTransparency()` path.
- **Startup warning when no renderer is available** (Phase 999.54, closes #501).
- **Inspector ↔ rendering-pipeline cut-over (the structural work):** the legacy
  Display/Material/Light/Stage/Coloring/Model settings dialogs no longer sit on
  the production render path. Model/Coloring processor factories moved to a
  headless MODELS layer (999.57); `scene.C` no longer constructs the
  Material/Light/Stage dialogs (999.58); and representation creation
  (molecule-load default rep + project restore) moved off `DisplayProperties`
  onto a headless `RepresentationBuilder` (999.65). The Inspector now drives the
  scene end-to-end; the legacy dialogs remain present but dormant.

A **build-acceleration pre-phase** (Phase 999.50) opened the cycle: it eliminated
the `poseClustering.C` boost template-metaprogramming cost that dominated the
Windows build (**~58 min → ~14 min**) and added `release.yml` ccache reuse.

**Deferred to v1.7.3** (work-in-progress on the `v1.7.3-legacy-deletion` branch):
the actual **deletion of the 9 legacy dialog files + the Tools → Legacy Settings
menu** (Phase 999.53) — the render pipeline is now detached from them, but their
removal is a self-contained cleanup that ships as its own validated unit. The
**PDF tutorial refresh** (Phase 999.56) also defers. Full detail:
`.planning/v1.7.2-BUILD-ACCEL-PLAN.md` + `.planning/v1.7.x-PATCH-QUEUE.md`.

### Breaking changes

#### PoseClustering Ward cluster-tree on-disk format changed (BUILD-ACCEL-06)

The Ward cluster-tree serializer no longer uses `boost::serialization`
archives. `boost::serialization` was the single heaviest Boost component to
compile and dominated the multi-minute build of `poseClustering.C`; the data
the tree actually holds is trivial. It is now written with a hand-rolled,
magic+version-headed format — the literal magic string `"BALLWARD"` followed
by a format version — in both a binary and a text variant (the `bool binary`
flag still selects between them). This drops that translation unit's compile
from tens of minutes to a few seconds and removes the `boost::serialization`
dependency from the tree entirely.

**Impact — old trees will NOT load.** Ward cluster-tree files
(`.ward`/`.dat`) serialized by **v1.7.1 and earlier** were boost archives and
**will not deserialize** under v1.7.2. They now **fail loudly** via a
`BALL::Exception::InvalidFormat` magic/version mismatch (with `__FILE__` /
`__LINE__`) rather than silently mis-parsing. This is a niche docking-research
format, so the impact is minimal:

- `DockPoseClustering` (which **writes** the tree) and
  `ExtractClustersFromWardTree` (which **reads** it) interoperate on the new
  `"BALLWARD"` format — regenerate any persisted tree by re-running
  `DockPoseClustering`.
- The public API is unchanged: `serializeWardClusterTree(std::ostream&, bool binary)`
  and `deserializeWardClusterTree(std::istream&, bool binary)` signatures and
  the `bool binary` flag are byte-identical to v1.7.1.
- The committed test fixture `test/data/PoseClustering_wardtree.dat` was
  regenerated in the new TEXT format and still deserializes to 6 clusters at
  `extractClustersForThreshold(0.5)`.

---

## Known issues (v1.7.2)

- **Transparent surfaces render with faceted / see-through-triangle
  artifacts.** When a surface (e.g. SE Surface) is made transparent, its own
  triangles blend in arbitrary order and back faces show through, producing a
  hatched/faceted look. This is a fundamental limitation of the Qt5
  compatibility-profile fixed-function renderer: it does unsorted alpha
  blending with backface culling disabled and no depth-sorting / order-
  independent transparency. The transparency control itself works correctly
  (the representation does become transparent); only the blended rendering
  quality is affected. A correct fix (order-independent transparency / depth
  peeling) requires the modern GL-core/QRhi renderer and is scoped to **v2
  (PIPE-01)** per the Phase 5 SPIKE-02 decision. Not a v1.7.x regression —
  pre-existing in the fixed-function path.

## Known issues (v1.7.1)

See `.planning/v1.7-USER-FEEDBACK-GATE.md` for status and investigation
detail.

- **SES surface triangles can drop out during rotation on Apple Silicon**
  (UFG-23). When a Solvent-Excluded Surface is rendered Solid and rotated
  interactively, some triangles momentarily disappear. Root cause is the
  Apple OpenGL→Metal translation layer not faithfully honouring
  fixed-function two-sided lighting (`GL_LIGHT_MODEL_TWO_SIDE`) for the
  inconsistently-wound SES mesh — *not* a regression in BALL's renderer or
  surface generator (both are functionally unchanged from v1.6). Under
  investigation. **Workaround:** view the structure with a different model
  (e.g. Ball-and-Stick / Cartoon), or rotate slowly.

---

*Created 2026-05-17 (Phase 999.49). v1.7.0 finalized 2026-05-19;*
*v1.7.1 patch-cycle section + refreshed known-issues added 2026-05-20.*
