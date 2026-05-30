---
phase: 02-rendering-port-4a
plan: 01
subsystem: infra
tags: [opengl, qt5, qglwidget, qopenglwidget, grep-gate, verification]

# Dependency graph
requires:
  - phase: 01-build-baseline
    provides: working modern-toolchain build (libBALL/libVIEW/BALLView on Qt 5.15)
provides:
  - "Reusable grep gate script (check-no-legacy-gl-symbols.sh) for RENDER-01/03/07 regression detection"
  - "Resolution of Open Question A1: raytracer/BufferedRenderer worker path issues no GL"
affects: [02-02, 02-03, 02-04]

# Tech tracking
tech-stack:
  added: []
  patterns:
    - "Scripted grep symbol gate as a Wave 0 verification asset (CI-lint-ready)"

key-files:
  created:
    - .planning/phases/02-rendering-port-4a/scripts/check-no-legacy-gl-symbols.sh
    - .planning/phases/02-rendering-port-4a/02-A1-FINDINGS.md
  modified: []

key-decisions:
  - "A1 CONFIRMED: raytracer worker thread issues zero GL — threading scope for Plans 03/04 stays minimal (no QOffscreenSurface/shared context)"
  - "Grep gate covers 11 legacy symbols across source/VIEW + include/BALL/VIEW; exit 1 on any hit so it doubles as a CI lint"

patterns-established:
  - "Wave 0 verification assets live under phases/<phase>/scripts/ and are committed as plan deliverables"

requirements-completed: [RENDER-01, RENDER-03, RENDER-07]

# Metrics
duration: 3min
completed: 2026-05-14
---

# Phase 2 Plan 01: Wave 0 Verification Assets Summary

**Scripted "no legacy Qt GL symbols" grep gate plus a written A1 resolution confirming the raytracer worker thread issues no GL — the threading scope signal for the rest of Phase 2.**

## Performance

- **Duration:** 3 min
- **Started:** 2026-05-14T07:28:33Z
- **Completed:** 2026-05-14T07:31:52Z
- **Tasks:** 2
- **Files modified:** 2 (both created)

## Accomplishments

- Created `check-no-legacy-gl-symbols.sh` — a reusable, CI-lint-ready grep gate scanning `source/VIEW` + `include/BALL/VIEW` for 11 legacy Qt GL symbols (`QGLWidget`, `QGLFormat`, `QGLContext`, `QGL::`, `QGLPixelBuffer`, `QtOpenGL/qgl`, `swapBuffers`, `setAutoBufferSwap`, `safeBufferSwap`, `grabFrameBuffer`, `convertToGLFormat`). It exits 1 with a file:line listing on the current pre-port tree (51 hits) and will exit 0 only once the port is complete.
- Resolved RESEARCH.md Open Question A1 with a written verdict + grep evidence: **A1 CONFIRMED** — the raytracer / `BufferedRenderer` worker-thread render path issues no OpenGL calls. The threading rework in Plans 03/04 therefore stays on the minimal path (worker-thread `makeCurrent()` removal; no `QOffscreenSurface` or shared context needed).

## Task Commits

Each task was committed atomically:

1. **Task 1: Create the no-legacy-Qt-GL-symbols grep gate script** - `98b6fb1` (chore)
2. **Task 2: Resolve Open Question A1 — does the raytracer worker path touch GL?** - `8902867` (docs)

**Plan metadata:** see final `docs(02-01)` commit

## Files Created/Modified

- `.planning/phases/02-rendering-port-4a/scripts/check-no-legacy-gl-symbols.sh` - Executable grep gate; resolves repo root robustly (arg or script-relative), scans the two VIEW trees, prints offending file:line:match, exits 0/1.
- `.planning/phases/02-rendering-port-4a/02-A1-FINDINGS.md` - A1 investigation: VERDICT line, grep evidence, and an implication note for Plan 03 (renderSetup.C cleanup) and Plan 04 (scene.C threading).

## Decisions Made

- **A1 CONFIRMED.** `RenderSetup::run()` worker loop only drives `renderToBuffer_()` for `!gl_renderer_`; the `BufferedRenderer` branch (`renderSetup.C:378-381`) calls a pure-CPU `renderToBuffer`. grep over `bufferedRenderer.h`, `raytracingRenderer.{h,C}`, `rtfactRenderer.{h,C}` found no live GL. `TilingRenderer` does contain GL but is only ever driven synchronously on the GUI thread via `updateGL()` (`scene.C:2167`), never `start()`-ed as a worker thread (the one `tr_rs->start()` that could pair with it is inside `#ifdef BALL_HAS_RTFACT`, out of scope).
- Grep gate exits non-zero on any hit so the same script works unchanged as a per-task regression check and a future CI lint step.

## Deviations from Plan

None - plan executed exactly as written.

## Issues Encountered

- During A1 investigation, `TilingRenderer` was found to contain fixed-function GL calls, which initially looked like it could refute A1. Resolved by tracing its `RenderSetup` usage: the only in-scope `TilingRenderer` runs synchronously on the GUI thread (`updateGL()`), and no `TilingRenderer` is ever `start()`-ed on a worker thread. A1 remains CONFIRMED; this caveat is documented in the findings file for Plan 03/04.
- Minor: the findings file initially had two lines matching the `A1 (CONFIRMED|REFUTED)` pattern (the verdict heading + a back-reference in the implication note). Reworded the back-reference so exactly one VERDICT line exists, per the acceptance criteria.

## User Setup Required

None - no external service configuration required.

## Next Phase Readiness

- Wave 0 verification assets are in place. Plans 02-04 can run the grep gate after each task to catch RENDER-01/03/07 regressions.
- A1 is resolved in the affirmative — Plan 03 (renderSetup.C) and Plan 04 (scene.C threading) proceed on the minimal-scope threading path with no orchestrator escalation needed.
- No source code was modified by this plan (verification assets only); `git status --porcelain source include` is clean.

## Self-Check: PASSED

- FOUND: `.planning/phases/02-rendering-port-4a/scripts/check-no-legacy-gl-symbols.sh` (executable, exit 1, 51 hits)
- FOUND: `.planning/phases/02-rendering-port-4a/02-A1-FINDINGS.md` (1 VERDICT line, Plan 03 + Plan 04 named)
- FOUND: commit `98b6fb1` (Task 1)
- FOUND: commit `8902867` (Task 2)

---
*Phase: 02-rendering-port-4a*
*Completed: 2026-05-14*
