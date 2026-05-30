---
phase: 02-rendering-port-4a
verified: 2026-05-14T11:30:00Z
status: passed
score: 5/5 must-haves verified
overrides_applied: 0
deferred:
  - truth: "BALLView displays a molecule in the embedded 3D scene on Linux and Windows"
    addressed_in: "Phase 4 (dependency model) + Phase 9 (CI matrix)"
    evidence: "REQUIREMENTS.md traceability marks RENDER-08 'Pending — needs CI/dependency model (cannot be verified before Phase 4/9)'. No per-OS graphics code exists; the HiDPI fix is devicePixelRatioF()-generic. Linux/Windows render confirmation requires the CI matrix and dependency overhaul, neither of which exists yet."
---

# Phase 2: Rendering Port (4a) Verification Report

**Phase Goal:** BALLView's embedded 3D scene renders molecules again on all three platforms by moving off the removed-in-Qt6 `QGLWidget`.
**Verified:** 2026-05-14T11:30:00Z
**Status:** passed
**Re-verification:** No — initial verification

## Goal Achievement

### Observable Truths

| # | Truth (ROADMAP Success Criteria) | Status | Evidence |
| --- | --- | --- | --- |
| 1 | BALLView displays a molecule in the embedded 3D scene on macOS, Linux, and Windows | ✓ VERIFIED (macOS) / deferred (Linux+Win) | macOS half: human verified "Approved" — demo peptide renders embedded in main window. Linux/Windows half: deferred to Phase 4/9 (see Deferred Items) — structurally addressed (no per-OS graphics code; DPR-generic HiDPI fix) but unverifiable without CI matrix. |
| 2 | The scene widget renders inside the main window — no detached/mis-sized native window | ✓ VERIFIED | Human visual check (RENDER-02): peptide visible inside Scene dock area, not a detached/blank window. Debug round 3 (`5ca7a47`) fixed render-inside-paintGL so the embedded FBO is correct. |
| 3 | The user can rotate, zoom, pick, and select with mouse and keyboard | ✓ VERIFIED | Human visual check (RENDER-06): left-drag rotates, scroll zooms, click-pick selects. Confirmed "Approved". |
| 4 | The raytracer renderer still produces output via CPU pixel-buffer path; on-screen text via QPainter overlay | ✓ VERIFIED | Human check (RENDER-04/05). A1 CONFIRMED: raytracer worker is CPU-only, output blits via `paintGL()` texture. Text overlay: `grep -c QPainter glRenderWindow.C` = 5. |
| 5 | `RenderSetup`, `scene.C`, `glOffscreenTarget.C`, `glRenderer.C`, `glRenderWindow.{h,C}` compile with zero `QGLWidget`/`QGLContext`/`QGLFormat` references | ✓ VERIFIED | Live (non-comment) legacy-symbol grep over `source/VIEW` + `include/BALL/VIEW` = 0 hits. `make VIEW BALLView -j8` builds clean (exit 0). The 6 remaining grep-gate hits are historical references inside `//` comments; the 1 real `QtOpenGL/qgl.h` include is in `glDisplayList.h` (out of phase scope, logged for Phase 5). |

**Score:** 5/5 truths verified (macOS goal fully met; Linux/Windows half of Truth 1 properly deferred)

### Deferred Items

| # | Item | Addressed In | Evidence |
| --- | --- | --- | --- |
| 1 | Molecule renders embedded on Linux + Windows (RENDER-08) | Phase 4 (deps) + Phase 9 (CI matrix) | REQUIREMENTS.md traceability already marks RENDER-08 "Pending — needs CI/dependency model". No per-OS graphics code; HiDPI fix is `devicePixelRatioF()`-generic so Linux/Windows are structurally unaffected. Cannot be verified here. |

### Required Artifacts

| Artifact | Expected | Status | Details |
| --- | --- | --- | --- |
| `include/BALL/VIEW/RENDERING/glRenderWindow.h` | `QOpenGLWidget` base + `static QSurfaceFormat gl_format_` | ✓ VERIFIED | `public QOpenGLWidget` ×1, `static QSurfaceFormat gl_format_` ×1, zero QGL* symbols |
| `source/VIEW/RENDERING/glRenderWindow.C` | `paintGL/initializeGL/resizeGL`, QPainter text overlay | ✓ VERIFIED | All 3 GL overrides defined; `QPainter` ×5; compiles clean |
| `source/APPLICATIONS/BALLVIEW/main.C` | `AA_ShareOpenGLContexts` + `setDefaultFormat` before QApplication | ✓ VERIFIED | Both present ×1 each |
| `source/VIEW/RENDERING/renderSetup.C` | `QOpenGLContext`, `grabFramebuffer`, GUI-thread-only makeCurrent | ✓ VERIFIED | `QOpenGLContext` ×2, `grabFramebuffer` ×1, zero QGLContext/grabFrameBuffer |
| `source/VIEW/RENDERING/RENDERERS/glRenderer.C` | `convertToFormat(Format_RGBA8888).mirrored()` | ✓ VERIFIED | `Format_RGBA8888` ×2 with `.mirrored()` |
| `source/VIEW/RENDERING/glOffscreenTarget.C` | `QOpenGLFramebufferObject` | ✓ VERIFIED | `QOpenGLFramebufferObject` ×4, no QGLPixelBuffer |
| `source/VIEW/WIDGETS/scene.C` | `QSurfaceFormat`+`QOpenGLContext` probe, swap-sync removed, stereo guard-deferred | ✓ VERIFIED | `QSurfaceFormat` ×6; probe still assigns `gl_format_`; 4 stereo/multi-display paths guard-deferred with logged messages; `updateGL` count = 31 (Scene's own method untouched) |
| `.planning/phases/02-rendering-port-4a/scripts/check-no-legacy-gl-symbols.sh` | Reusable grep gate | ✓ VERIFIED | Exists, executable, runs |
| `.planning/phases/02-rendering-port-4a/02-A1-FINDINGS.md` | A1 resolution | ✓ VERIFIED | A1 CONFIRMED verdict + grep evidence + Plan 03/04 implication note |

### Key Link Verification

| From | To | Via | Status | Details |
| --- | --- | --- | --- | --- |
| `main.C` | `GLRenderWindow::gl_format_` | `setDefaultFormat` before QApplication | ✓ WIRED | Both attribute + default format set pre-construction |
| `glRenderWindow.C paintGL()` | render body | GL work inside paintGL (debug round 3) | ✓ WIRED | `5ca7a47` moved render into paintGL; instrumented run confirms paintGL called every frame |
| `renderSetup.C run() worker loop` | GL context | no makeCurrent on worker thread | ✓ WIRED | All 13 makeCurrent sites are GUI-thread; A1 CONFIRMED raytracer worker is CPU-only |
| `scene.C` stereo probe | `gl_format_` | `QOpenGLContext::create()` + `format().stereo()` | ✓ WIRED | Probe rewritten with QSurfaceFormat; still assigns `gl_format_` (lines 3143-3167) |

### Behavioral Spot-Checks

| Behavior | Command | Result | Status |
| --- | --- | --- | --- |
| VIEW + BALLView build clean | `make VIEW BALLView -j8` | All targets built, exit 0 | ✓ PASS |
| Build artifact produced | `ls build/bin/BALLView.app` | BALLView.app exists | ✓ PASS |
| Legacy GL symbol grep gate (live code) | grep excluding comments | 0 live hits | ✓ PASS |
| Embedded molecule renders on macOS | manual GUI launch | Human "Approved" | ✓ PASS (human) |

### Requirements Coverage

| Requirement | Source Plan | Description | Status | Evidence |
| --- | --- | --- | --- | --- |
| RENDER-01 | 02-01,02-02 | `GLRenderWindow` on `QOpenGLWidget`, `QSurfaceFormat` compat profile | ✓ SATISFIED | Header + format verified; grep gate clean |
| RENDER-02 | 02-04 | Molecule in embedded scene on macOS | ✓ SATISFIED | Human "Approved" — embedded render confirmed |
| RENDER-03 | 02-02 | GL renderer on GUI thread via `paintGL()`; manual swap removed | ✓ SATISFIED | paintGL defined; no swapBuffers/safeBufferSwap/setAutoBufferSwap |
| RENDER-04 | 02-03,02-04 | Raytracer CPU pixel-buffer path blitted as texture | ✓ SATISFIED | A1 CONFIRMED; human-verified raytracer output |
| RENDER-05 | 02-02 | On-screen text via `QPainter` overlay | ✓ SATISFIED | QPainter overlay in glRenderWindow.C; human-verified legible text |
| RENDER-06 | 02-04 | Mouse/keyboard interaction works | ✓ SATISFIED | Human-verified rotate/zoom/pick/select |
| RENDER-07 | 02-01,02-02,02-03,02-04 | All 5 files compile with zero QGL* references | ✓ SATISFIED | Zero live legacy symbols; clean build |
| RENDER-08 | 02-04 | Builds + renders on Linux + Windows | ⏸ DEFERRED | Correctly deferred to Phase 4/9 — needs CI matrix + dependency model. No per-OS graphics code; structurally addressed. |

No orphaned requirements — all 8 phase requirement IDs are claimed by plans and accounted for.

### Anti-Patterns Found

| File | Line | Pattern | Severity | Impact |
| --- | --- | --- | --- | --- |
| `include/BALL/VIEW/RENDERING/glDisplayList.h` | 13 | `#include <QtOpenGL/qgl.h>` | ℹ️ Info | Out of phase scope (not in any plan's files_modified). Compiles fine on Qt 5.15; a Qt 6 blocker logged in `deferred-items.md` for Phase 5. Not a Qt 5 build blocker, not a phase failure. |
| `scene.C`, `glRenderWindow.C` | various | `QGLWidget` literal in `//` comments (6 lines) | ℹ️ Info | Historical-context comments only — no live code. Trips the line-based grep gate but represents zero functional legacy usage. |

`GLRenderWindow::renderText(float,float,float,...)` and `GLOffscreenTarget::tryUsePixelBuffer()` are documented intentional stubs (pre-existing dead code / API-stability scaffolding) — not regressions.

### Human Verification Required

None outstanding. The blocking human smoke checkpoint (Plan 04 Task 3) was completed — user replied "Approved" confirming embedded molecule render, resize survival, and rotate/zoom/pick on macOS. This satisfied the `[HUMAN-GATED]` must-have and RENDER-02/04/05/06.

### Gaps Summary

No gaps. All 5 ROADMAP success criteria are met: the macOS embedded-render goal is fully achieved and human-verified after three debugger rounds that fixed structural QGLWidget→QOpenGLWidget mismatches (deferred GL init, HiDPI device-pixel viewport, render-inside-paintGL). All 5 target files compile clean with zero live legacy Qt GL symbols and the full `make VIEW BALLView` build is green.

RENDER-08 (Linux + Windows render) is the only non-met item, and it is a correctly documented carry-forward — REQUIREMENTS.md traceability already marks it "Pending — needs CI/dependency model (cannot be verified before Phase 4/9)". It is structurally addressed (no per-OS graphics code; the HiDPI fix is `devicePixelRatioF()`-generic) but cannot be verified without the CI matrix (Phase 9) and dependency model (Phase 4). Per GSD deferral rules this does not constitute a phase failure.

---

_Verified: 2026-05-14T11:30:00Z_
_Verifier: Claude (gsd-verifier)_
