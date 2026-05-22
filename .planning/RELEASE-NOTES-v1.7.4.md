# BALLView v1.7.4 — Release notes (DRAFT)

**Status:** draft. Finalize during the v1.7.4 freeze window per
`.planning/RELEASE-PROCESS.md`. This file lives in the handoff package
so it travels with the implementation work.

**Codename:** *"Inspector depth + the contract"*
**Predecessor:** v1.7.3 (or v1.7.2 if 1.7.3 is skipped)
**Branch:** `v1.7.4-inspector-depth`

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
`friend`-scoped to their matching Controllers.

---

## What's new in v1.7.4

### Architecture

- **`Controller::apply()` is now contract-bound** (`999.59`,
  v1.7.x-24). Every mutating Controller — Model / Coloring / Material
  / Clipping / Label / Stage / Camera / Light / Stereo — validates
  preconditions, guards against re-entry, mutates the single owner,
  emits exactly one typed event, requests soft render invalidation,
  and captures a reversible intent payload. See the
  `ARCHITECTURE-CONTRACT.md` doc bundled with the v1.7.4 handover
  package, and `.planning/v1.7.x-ARCHITECTURE-REVIEW.md` for the
  motivating analysis.
- **Owner-side mutation is now Controller-scoped.** `Stage::set*` and
  `Representation::set*` methods are `friend`-scoped to their matching
  Controllers. Direct external mutation does not compile. (For loaders
  and file-IO TUs, see "Migration notes" below.)
- **Contract tests** (`999.60`, v1.7.x-25) run on every PR. They
  exercise every Controller via Inspector / toolbar / menu / Python
  surfaces and assert identical owner postconditions. Selection-
  consumer agreement (tree / scene / control / action) is verified
  in the same harness. Linux CI; blocking.
- **Re-entrancy is designed, not assumed.** A new `ApplyGuard` RAII
  protects every `apply()` from synchronous re-entry through the
  notification bus. The v1.7.x-13 freeze class cannot recur.

### Qt + paint discipline

- **Qt-lifetime conventions doc** (`999.61`, v1.7.x-26) lives at
  `.planning/codebase/QT-LIFETIME.md`. Five rules + four case studies
  + cookbook. A blocking `lifetime-discipline` CI lint rejects
  `deleteLater()` inside destructors.
- **Opaque-paint contract** (`999.62`, v1.7.x-35 new) is codified.
  Every container that hosts an animated child carries
  `WA_OpaquePaintEvent` by structural contract, not per-site patch.
  The 5 UFG-05/09/10 manual fixes from rc2/rc3 are now redundant with
  the base-class pattern. A `opaque-paint-lint` CI job enforces the
  rule. Dark mode (v1.8) lands on this foundation.

### Inspector depth — the visible win

- **Coloring · Value-range histogram** (`999.63`, v1.7.x-17b). Replaces
  the v1.7.1 min/max text inputs with a histogram of the observed
  values across the active selection, plus two draggable handles, plus
  four presets (`Auto-fit` / `Full range` / `Robust 5–95%` / `Reset`).
  Works for B-factor / Occupancy / Distance / Atom-charge / Forces /
  Temperature-factor / Custom value colorings.
- **Per-section "Reset section"** (`999.64`, v1.7.x-36 new). Every
  Inspector section's header carries a Reset glyph on hover. Click =
  revert that section's Controller to method-defined defaults, through
  the contract. Captured as one undoable command (consumed in v2.0).

---

## Breaking changes

### Source-level (plugin authors / out-of-tree extension code)

- `Stage::setBackgroundColor()`, `setFogIntensity()`, …, and the
  matching `Representation::setX()` methods are now `private` and
  `friend`-scoped to their matching Controllers. **External code
  mutating these directly will fail to compile** with a
  `no matching public member function` diagnostic.

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

- The binding surface is **Controller-centric** in v1.7.4. The
  shim from the legacy direct-setter surface is preserved for one
  release with a deprecation warning. Update scripts to use the
  Controller surface; the legacy shim is **removed in v1.8**.

  Example diff:
  ```python
  # before (v1.7.3 and earlier)
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
  through the §2 contract shape from `ARCHITECTURE-CONTRACT.md`.
  Stubs / read-only Controllers are no longer supported; the bus and
  the contract tests both reject them.
- If your plugin mutates `Stage` or `Representation` directly, route
  through a Controller or — for inherently scene-side operations —
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
| `999.59` | Controller cut-over · command-shaped `apply()` | -24 |
| `999.60` | Contract tests · cross-surface parity | -25 |
| `999.61` | Qt-lifetime audit + conventions doc | -26 |
| `999.62` | Opaque-paint contract codified | -35 (new) |
| `999.63` | Coloring · value-range histogram | -17b |
| `999.64` | Per-section "Reset section" affordance | -36 (new) |

---

## Acknowledgements

The v1.7.x Architecture Review (`.planning/v1.7.x-ARCHITECTURE-REVIEW.md`)
distilled the regression class; Codex's adversarial round of corrections
sharpened the command-shape into a transactional contract.

---

*v1.7.4 release notes drafted 2026-05-22 in the v1.7.4 handoff
package. Finalize at tag time per `RELEASE-PROCESS.md`.*
