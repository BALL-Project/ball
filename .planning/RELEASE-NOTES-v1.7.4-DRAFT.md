# DRAFT — Inspector Depth + the Controller Contract (tentative v1.7.4)

> **STATUS: DRAFT / UNRELEASED.** Tag held pending (1) the UAT batch
> (`.planning/v1.7.4-UAT-BATCH.md`) and (2) the version-sequencing decision
> (this work vs. the deferred v1.7.3 legacy-dialog deletion). Folds into
> `RELEASE-NOTES-v1.7.md` with the final number once decided. Built on
> `v1.7.4-inspector-depth` @ `f96e9aac62` (off the v1.7.2 tag).

## Headline

A structural milestone: every mutating BALLView control now routes through a
single **command-shaped `Controller::apply()`** contract (validate → guard →
mutate the one owner → emit one event → declared invalidation → record a
reversible payload). The `Stage` / `Representation` owner setters are
**privatized** behind narrow `StageMutation` / `RepresentationBuilder` friends —
external mutation no longer compiles — so there is exactly one mutation path per
domain. On top of that contract: the first visible Inspector-depth feature (a
coloring value-range **histogram**) and a per-section **Reset**.

Transparent to non-GUI `libBALL`/`libVIEW` consumers: same APIs, same SONAMEs.

## What's new

- **Command-shaped controller contract (999.59).** All 9 controllers
  (Model / Coloring / Material / Clipping / Label / Stage / Camera / Light /
  Stereo) on one `bool apply()` shape with a nest-aware re-entrancy guard;
  Clipping gained the guard it previously omitted. A reversible `ApplyPayload`
  is captured on every mutation (capture-only; v2.0 consumes it for undo). A
  minimal `ControllerJob` async scaffold lands the GUI-thread-only story.
- **Owner-API narrowing (999.59-04).** `Stage`/`Representation` setters
  privatized; ~47 call sites across 14 files migrated through the controller /
  builder surfaces. A cross-surface **contract-test harness** (Linux CI gate,
  9 per-controller parity fixtures + owner-narrowing compile-fail checks)
  proves menu/toolbar/Inspector reach identical owner state.
- **Coloring · value-range histogram (999.63).** Replaces the min/max text
  inputs with a histogram of the active selection's value distribution, two
  draggable handles (live preview while dragging, commit on release), and four
  presets (Auto-fit / Full range / Robust 5–95% / Reset). The drag preview is a
  proven **render-only** path — a contract test asserts it mutates nothing.
- **Per-section Reset (999.64).** Every Inspector section header carries a Reset
  glyph that reverts that section to method defaults in a single
  `Controller::reset()` event (replacing the legacy two-step revert+apply).
- **PyBALL binding surface refreshed (999.59-06).** `controllers.sip` exposes
  the controllers; `representation.sip` drops the privatized setters and keeps
  legacy calls working for one release via `DeprecationWarning` shims. (SIP
  remains build-disabled; full regeneration rides v2.1.)

## Engineering / CI

- New blocking CI gates: **contract-tests** (Linux), **sip-refresh**
  (binding-symbol lint), **opaque-paint** (container flicker lint), alongside
  the existing **lifetime-discipline** lint.
- New design-system docs: `QT-LIFETIME.md` (999.61), `THEME.md` + the
  `opaqueContainer` theme token (999.62 + close-out).
- Opaque-paint lint found + fixed a real flicker gap in the bottom log drawer.
- Verified green on all 4 platforms (linux-x64/arm64, windows-x64, macos-arm64).

## Deferred / known

- **v1.7.3 legacy-dialog deletion** — separate, still on its branch.
- **PyBALL runtime** — v2.1 (Phase 6 generator bake-off).
- **Transparent-surface render artifacts** — pre-existing fixed-function
  limitation; v2 / PIPE-01.
