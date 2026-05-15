---
id: SEED-005-4
status: dormant
planted: 2026-05-15
planted_during: BALLView 1.6 Modernization · Phase 5 (Qt 6 + Renderer Backend Spike) · Plan 05-07
trigger_when: PIPE-01 begins (the v2 / future-phase programmable-pipeline rewrite)
scope: Small (re-run smoke-check after the spike-pipeline-substitution fix lands; paste DIAG into a follow-up artifact)
---

# SEED-005-4: Post-PIPE-01 spike full-render BALLVIEW_GL_DIAG re-capture

## Why This Matters

Plans 05-05 (GL-core spike) and 05-06 (QRhi spike) both shipped working
SPIKE-01 implementations behind the two-layer
`BALL_SPIKE_BACKEND=<GLCore|QRhi>` + `BALLVIEW_USE_SPIKE_BACKEND=1` gate.
Both spike paths construct correctly through `RendererFactory` — proving the
gate, the env-var, and the factory wiring — but **crash during early init
before `initializeGL()` reaches the `BALLVIEW_GL_DIAG` emission point**.

The root cause (documented identically in both 05-05 and 05-06 SUMMARYs):
the env-var gate substitutes a `Renderer` for `Kind::OpenGL_Fixed` but
downstream `RenderSetup` + `Scene` paint hooks invoke `GLRenderer`-specific
virtuals (`bufferRepresentation`, `setFogIntensity`, the immediate-mode
per-primitive `render*_()` virtuals) that the bare-bones spike renderers
do not implement. The matching `RenderSurface` substitution also isn't
wired by the env-var gate (only `Kind::OpenGL_Fixed → makeSurface` runs,
which still constructs `GLRenderWindow`).

Consequence for Plan 05-07's driver-behaviour artifact: no live
`BALLVIEW_GL_DIAG renderer_backend=<Core|QRhi> qrhi_backend_name=Metal ...`
line is capturable on any platform from a spike-built BALLView.

## Empirical Gap This Resolves

Per `.planning/phases/05-.../05-DRIVER-BEHAVIOUR.md` §Gap 4 — Post-PIPE-01
spike full-render re-capture. Specifically:

- Live `BALLVIEW_GL_DIAG` from a Core-profile context (expected
  `gl_profile=core` on Linux/Windows; on macOS, `gl_version=4.1 Metal`).
- Live `BALLVIEW_GL_DIAG renderer_backend=QRhi qrhi_backend_name=Metal`
  on macOS-arm64 (the Plan 05-06 predicted line — predicted from
  `QRhi::backendName()` API verification, not a live render).
- Live picking + text overlay verification of the spike paths.

Note: by the time PIPE-01 runs, "the spike" may be the production
renderer (per the SPIKE-02 decision in Plan 08). So this seed effectively
becomes "re-capture driver behaviour for the chosen post-Phase-5 renderer."

## Proposed Resolution

Triggered at PIPE-01 phase opening. PIPE-01 is the programmable-pipeline
rewrite that resolves the downstream-virtual limitation either by:

- (a) Extending `Renderer`'s virtual surface to cover the missing entry
  points so the spike actually substitutes cleanly. OR
- (b) Re-shaping `RenderSetup` + `Scene` to talk only to the boundary
  contract (Renderer + RenderSurface) and drop the `GLRenderer`-specific
  calls. OR
- (c) The post-decision concrete renderer (whichever Plan 08 picks)
  ships as a full replacement, not as a spike-mode substitution.

Once PIPE-01 has a working concrete renderer that reaches
`initializeGL()` and emits the DIAG line:

1. Run `bash .planning/phases/02.2-ci-and-build-smoke-matrix/scripts/render-smoke-check.sh
   <binary>` on each platform (macOS, Linux, Windows — gated by
   SEED-005-1 + SEED-005-2 + SEED-005-3 being resolved).
2. Capture the DIAG line verbatim.
3. Append to PIPE-01's own driver-behaviour record (a new artifact under
   PIPE-01's phase dir; do NOT retroactively edit `05-DRIVER-BEHAVIOUR.md`,
   which captures the Phase-5-end state for SPIKE-02's decision context).

## Verification

After PIPE-01 lands and this seed is resolved:

- The PIPE-01 phase has its own `<phase>-DRIVER-BEHAVIOUR.md` (or equivalent)
  artifact with live DIAG captures from all three OSes.
- The Plan 05-07 artifact's Gap 4 is annotated with a forward-pointer:
  "Resolved by PIPE-01 — see `.planning/phases/<pipe01-phase>/...`".

## Suggested Home

PIPE-01 phase Wave 0 (after the pipeline rewrite is functional but before
the phase closes — the driver-behaviour data is one of the standard
phase-closing artifacts).

Estimated effort: <2 hours assuming all three platforms have green CI smoke
steps by then (Paths A in SEED-005-1, SEED-005-2, SEED-005-3 all resolved).
The capture itself is `grep BALLVIEW_GL_DIAG ci-logs` + paste.
