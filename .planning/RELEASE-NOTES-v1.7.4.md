# BALLView v1.7.4 — Release notes (DRAFT)

**Status:** draft. Finalize during the v1.7.4 freeze window per
`.planning/RELEASE-PROCESS.md`. This file lives in the handoff package
so it travels with the implementation work.

**Codename:** *"Inspector depth + the contract"*
**Predecessor:** v1.7.3 (or v1.7.2 if 1.7.3 is skipped)
**Branch:** `v1.7.4-inspector-depth`

> **Codex-reconciled 2026-05-22.** All 9 controllers named consistently;
> `apply()` returns `bool`; the PyBALL binding refresh is a v1.7.4
> deliverable (not deferred to v2.1); per-section reset is a single
> `Controller::reset()`.

---

## v1.7.4 in one paragraph

v1.7.4 is the **architecture-discipline release** for BALLView.
It lands the command-shaped Controller contract the v1.7.x
Architecture Review demanded (closing the dual-path mutation defect
class that drove ~30 UFG bug reports), adds contract tests that keep
the rule in place, and ships the first visible Inspector-depth feature
on top of the closed contract — a histogram-with-handles for
value-based colorings.

If you build plugins or extend BALLView's Controller layer, **read the
Architecture contract section below** — owner-side setters are now
`friend`-scoped to their matching Controllers, and the PyBALL binding
surface moved from owners to Controllers in this release.

---

## What's new in v1.7.4

### Architecture

- **`Controller::apply()` is now contract-bound** (`999.59`,
  v1.7.x-24). All **9** mutating Controllers — Model / Coloring /
  Material / Clipping / Label / Stage / Camera / Light / Stereo —
  validate preconditions, guard against re-entry (a nest-aware
  GUI-thread guard, now required even in Clipping which previously
  omitted it), mutate the single owner, emit exactly one typed event,
  request their declared per-domain render invalidation (soft refresh
  for most; a display-list rebuild for Model; scene-structural side
  effects for Stage), and capture a reversible intent payload. `apply()`
  returns `bool` (`true` = mutated, `false` = dropped/rejected). See the
  `ARCHITECTURE-CONTRACT.md` doc bundled with the v1.7.4 handover
  package, and `.planning/v1.7.x-ARCHITECTURE-REVIEW.md` for the
  motivating analysis.
- **Owner-side mutation is now Controller-scoped.** `Stage::set*` and
  `Representation::set*` methods are private; mutation flows through a
  single `StageMutation` facade (Stage) and a single
  `RepresentationBuilder` friend (Representation), not a pile of
  per-controller friends. Direct external mutation does not compile.
  (For loaders and file-IO TUs, see "Migration notes" below.)
- **Contract tests** (`999.60`, v1.7.x-25) run on every PR. They
  exercise all 9 Controllers via Inspector / toolbar / menu / Python
  surfaces and assert identical owner postconditions; the toolbar/menu
  surfaces run through registered CommandRegistry command bodies, so the
  menu/toolbar path is genuinely tested. Selection-consumer agreement
  (tree / scene / control / action) is verified in the same harness.
  Linux CI; blocking.
- **Re-entrancy is designed, not assumed.** A nest-aware
  `ControllerApplyGuard` protects every `apply()` from synchronous
  re-entry through the notification bus. The v1.7.x-13 freeze class
  cannot recur.

### Python (PyBALL)

- **Controller-centric bindings ship in v1.7.4** (`999.59-06`). The
  binding surface moved from owner setters to the Controllers:
  `representation.sip` no longer exposes the now-private setters; a new
  `controllers.sip` exposes the 9 Controllers. A one-release deprecation
  shim keeps the legacy attribute path working with a `DeprecationWarning`
  (removed in v1.8). A CI SIP-refresh build-check regenerates and compiles
  the bindings so a future setter rename can't silently break PyBALL.

### Qt + paint discipline

- **Qt-lifetime conventions doc** (`999.61`, v1.7.x-26) lives at
  `.planning/codebase/QT-LIFETIME.md`. Five rules + four case studies
  + cookbook. A blocking `lifetime-discipline` CI lint rejects
  `deleteLater()` inside destructors.
- **Opaque-paint contract** (`999.62`, v1.7.x-35 new) is codified.
  Every container that hosts an animated child carries
  `WA_OpaquePaintEvent` by structural contract, not per-site patch.
  The 5 UFG-05/09/10 manual fixes from rc2/rc3 are now redundant with
  the base-class pattern. An `opaque-paint-lint` CI job enforces the
  rule — warn-only first (with a whitelist + synthetic self-tests),
  flipping to blocking after one clean week. Dark mode (v1.8) lands on
  this foundation.

### Inspector depth — the visible win

- **Coloring · Value-range histogram** (`999.63`, v1.7.x-17b). Replaces
  the v1.7.1 min/max text inputs with a histogram of the observed
  values across the active selection, plus two draggable handles, plus
  four presets (`Auto-fit` / `Full range` / `Robust 5–95%` / `Reset`).
  Dragging shows a live render-only preview (not a mutation); the change
  commits on release through the contract. Works for B-factor /
  Occupancy / Distance / Atom-charge / Forces / Temperature-factor /
  Custom value colorings.
- **Per-section "Reset section"** (`999.64`, v1.7.x-36 new). Every
  Inspector section's header carries a Reset glyph on hover. Click =
  revert that section's Controller to method-defined defaults through a
  single `Controller::reset()` (replacing the older two-step
  `revert(); apply()`), captured as one undoable command (consumed in
  v2.0).

---

## Breaking changes

### Source-level (plugin authors / out-of-tree extension code)

- `Stage::setBackgroundColor()`, `setFogIntensity()`, …, and the
  matching `Representation::setX()` methods are now `private`. Mutation
  flows through the `StageMutation` facade / `RepresentationBuilder`
  friend, both reachable only from the matching Controllers. **External
  code mutating these directly will fail to compile** with a
  `no matching public member function` / `is private` diagnostic.

  **Migration.** Route mutation through the matching Controller. For
  Python: the SIP-generated bindings now expose Controllers, not
  owners — `mainControl.coloringController().setMethod("Element")`,
  not `representation.coloring = ...`.

  **File loaders.** Loaders that populate Representations at
  construction time go through `RepresentationBuilder` (new
  `friend`-class in `KERNEL`), not direct setters. If you have a
  custom format reader, see the migration note in
  `999.59-04-SUMMARY.md`.

### On-disk format

None in v1.7.4. (The `BALLWARD` Ward-tree format break shipped in
v1.7.2 and is documented under that release.)

### CLI / scripts

None.

---

## Migration notes

### For end users

- **First launch with v1.7.4:** indistinguishable from v1.7.3 except
  for the new value-range histogram in Coloring sections and the Reset
  hover affordance. No layout changes; workspace and session state
  carry forward verbatim.

### For Python users (PyBALL)

- The binding surface is **Controller-centric** in v1.7.4, and the
  bindings were regenerated in this release. The shim from the legacy
  direct-setter surface is preserved for one release with a
  `DeprecationWarning`. Update scripts to use the Controller surface;
  the legacy shim is **removed in v1.8**.

  Example diff:
  ```python
  # before (v1.7.3 and earlier) — still works in v1.7.4 with a DeprecationWarning
  rep.coloring_method = "Element"
  rep.background_color = (0, 0, 0)

  # after (v1.7.4)
  ctrl = main.coloring_controller()
  ctrl.set_method("Element").apply()
  main.stage_controller().set_background_color(0, 0, 0).apply()
  ```

  The deprecation warnings print location + suggested-replacement at
  Python import time; check stderr.

### For plugin developers

- If your plugin subclasses `Controller`, you must override `apply()`
  through the §2 contract shape from `ARCHITECTURE-CONTRACT.md`
  (returning `bool`, taking the guard, declaring its invalidation).
  Stubs / read-only Controllers are no longer supported; the bus and
  the contract tests both reject them.
- If your plugin mutates `Stage` or `Representation` directly, route
  through a Controller or — for inherently scene-side construction —
  through `RepresentationBuilder` with the documented escape-hatch
  comment.

---

## Known issues (carried from v1.7.x)

- **UFG-23** — SES surface triangles can drop out during rotation on
  Apple Silicon. Pre-existing; not a v1.7.4 regression. Rendering
  triage continues; expected fix in v1.7.5 or v1.8.

---

## Phase index

| Phase | Title | v1.7.x-NN |
|-------|-------|-----------|
| `999.59` | Controller cut-over · command-shaped `apply()` + owner-narrowing + PyBALL binding refresh | -24 |
| `999.60` | Contract tests · cross-surface parity | -25 |
| `999.61` | Qt-lifetime audit + conventions doc | -26 |
| `999.62` | Opaque-paint contract codified | -35 (new) |
| `999.63` | Coloring · value-range histogram | -17b |
| `999.64` | Per-section "Reset section" affordance | -36 (new) |

---

## Acknowledgements

The v1.7.x Architecture Review (`.planning/v1.7.x-ARCHITECTURE-REVIEW.md`)
distilled the regression class; Codex's adversarial round of corrections
sharpened the command-shape into a transactional contract and grounded
the owner-narrowing fallout, the controller count, and the Python
binding work that ship in this release.

---

*v1.7.4 release notes drafted 2026-05-22 in the v1.7.4 handoff
package; Codex-reconciled 2026-05-22. Finalize at tag time per
`RELEASE-PROCESS.md`.*
