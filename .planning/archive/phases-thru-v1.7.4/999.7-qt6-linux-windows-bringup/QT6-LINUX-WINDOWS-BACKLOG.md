---
phase: 999.7-qt6-linux-windows-bringup
type: backlog
status: dormant
gates: Phase 5 must land; CI carry-forwards (SEED-005-1, SEED-005-2) needed; Windows hardware/VM access needed for SEED-005-3
milestone: v1.6
created: 2026-05-15
---

# Phase 999.7: Qt 6 Linux + Windows Bring-Up and CI Fixup

## Status: DORMANT — promote when Phase 5.1 closes (or when a contributor has Windows access)

Phase 5 delivered the Qt 6 migration baseline on **macOS Apple Silicon** end-to-end:
configure green, link green, BALLView starts, default renderer emits a non-blank PNG
via the compatibility-profile path, both spike backends (GL-Core + QRhi) build green,
and the SPIKE-02 decision record was authored ("GL-Core for v1.6.x → QRhi for v2").

Linux and Windows runtime verification was deferred — three CI/tooling issues block
the matrix and there is no Windows hardware available to the current author. This
backlog entry consolidates that scope into a single promotion-ready phase.

## Scope (what this phase must deliver)

### 1. Linux runtime green
- **Resolve SEED-005-1**: `jurplel/install-qt-action` v4 + aqtinstall 3.3.0 no longer
  accepts `qtbase qttools qtopengl` as `--modules` values. Either drop the modules
  argument and let the default install run, or migrate to the v3 syntax. Verify
  the resulting Qt 6.5+ install is complete for BALLView's link surface
  (Core, Gui, Widgets, OpenGL, OpenGLWidgets, Network, Xml, optionally Sql, PrintSupport).
- BALLView builds end-to-end on `ubuntu-24.04` against the install-qt-action Qt 6.5.
- The Phase 02.2 render smoke check produces a non-blank PNG on Linux under Qt 6.
- Capture `BALLVIEW_GL_DIAG` from the live Linux run and update `05-DRIVER-BEHAVIOUR.md`
  (the matrix row currently marked GAP) — or copy into a new
  `999.7-DRIVER-BEHAVIOUR.md` if Phase 5 is closed.

### 2. Windows runtime green
- **Resolve SEED-005-2**: the `vcpkg.json` `builtin-baseline` (`c1ce926ddf75…`) is not
  resolvable against the runner's pre-installed vcpkg checkout. Either pin a newer
  baseline (a commit known to be in the runner image), drop the baseline entirely
  and let vcpkg auto-pick, or vendor the vcpkg version-cache. Verify the manifest
  resolves `qtbase` cleanly.
- **Resolve SEED-005-3**: Windows VM (or contributor with a Windows machine) executes
  the build + smoke check; capture `BALLVIEW_GL_DIAG` and driver behaviour into
  the matrix (default backend; GL-Core spike — the QRhi spike row stays optional
  since the Qt 6.5 LTS floor disallows QRhiWidget, and that's documented in the
  SPIKE-02 decision).

### 3. macOS CI assertion relaxation (zero-cost)
- The `.github/workflows/ci.yml` step "Assert GL compat profile" on macOS at lines
  331 and 387 (post-Phase-5 line numbers may have shifted — grep for
  `gl_profile=compatibility`) asserts `gl_profile=compatibility`, but two
  verbatim live captures (M4 Max local + macos-latest software runner) both emit
  `gl_profile=none` because Apple's GL 2.1 implementation doesn't expose the
  Core/Compat profile distinction at version 2.1. Relax to
  `gl_profile=(compatibility|none)` on macOS — keep the strict assertion on
  Linux where the GL implementation does expose it.
- Reference: `05-DRIVER-BEHAVIOUR.md` §"BALLVIEW_GL_DIAG Captures" and
  `05-SPIKE-DECISION.md` §3 — the original D-08 oracle should be retired for
  Apple Silicon and revisited once SPIKE-01-derived GL ≥ 3.2 lands in PIPE-01.

### 4. Cross-platform spike re-verification (optional, post-PIPE-01)
- **SEED-005-4** stays carried forward, gated on PIPE-01: once the spike renderers
  implement enough downstream virtuals to render the demo molecule end-to-end on
  macOS, re-capture each platform×backend cell with full-render evidence (PNG +
  picking-roundtrip + text overlay confirmation).
- Out of scope for v1.6 unless PIPE-01 lands first.

## Why this is a backlog phase, not a Phase 5.1 fold-in

- **Phase 5.1** is already on the active roadmap with a different focus (warnings
  + latent bugs from Phase 4's tri-OS CI matrix). Folding Linux/Windows Qt 6
  bring-up into Phase 5.1 would blur its scope and slip its delivery.
- **Promotion conditions**: (a) Phase 5.1 has closed (so warnings noise has
  cleared) AND (b) one of: a contributor has Windows access, OR the project
  decides to ship v1.6.0 macOS-only with a "Linux/Windows: build from source,
  unverified" footnote.
- **Estimated effort**: SEED-005-1 + SEED-005-2 + SEED-005-3 each Small/Medium
  (per their seed files); macOS CI relaxation is a one-line change. Total
  ≈ 2 days for a contributor with Windows access.

## Seeds consolidated by this phase

| Seed | Scope | Effort |
|------|-------|--------|
| `seeds/SEED-005-1-ci-aqtinstall-modules-fixup.md` | Fix Linux Qt install | Small |
| `seeds/SEED-005-2-ci-vcpkg-baseline-fixup.md` | Fix Windows vcpkg manifest | Small |
| `seeds/SEED-005-3-windows-vm-driver-capture.md` | Windows VM + driver capture | Medium |
| `seeds/SEED-005-4-post-pipe01-spike-recapture.md` | Spike re-verification (post-PIPE-01) | Small |
| *(new)* macOS gl_profile CI relaxation | One-line ci.yml change | Trivial |

## References

- `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-DRIVER-BEHAVIOUR.md` — per-platform matrix with the GAP rows this phase fills
- `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-SPIKE-DECISION.md` §3 — per-platform acceptance criteria the runtime verification must satisfy
- `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/deferred-items.md` — carry-forward log
- `.planning/seeds/SEED-005-1.md` .. `SEED-005-4.md` — per-issue technical detail

## Out of scope

- The three CR-01/02/03 Qt 6 code-review regressions (HTMLPage SSL signal, QSignalMapper rename, QLayout::setMargin) — those belong in Phase 5.1's warnings/latent-bugs scope.
- Production-quality CI on Linux/Windows beyond "build green + smoke check passes" — the existing macOS CI gates are the v1.6 quality bar; tri-OS parity is a v2 concern.
- Code signing / notarization / installer formats on Linux/Windows — Phase 8 owns that.
