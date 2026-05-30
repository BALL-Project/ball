---
phase: 5
slug: qt-6-migration-4b-renderer-backend-decision-spike
status: draft
nyquist_compliant: false
wave_0_complete: false
created: 2026-05-15
---

# Phase 5 — Validation Strategy

> Per-phase validation contract for feedback sampling during execution.
> Derived from `05-RESEARCH.md` § Validation Architecture.

---

## Test Infrastructure

| Property | Value |
|----------|-------|
| **Framework** | Phase 02.2 render smoke check (`render-smoke-check.sh`) + `BALLVIEW_GL_DIAG` stdout assertions + CMake build matrix |
| **Config file** | `.planning/phases/02.2-ci-and-build-smoke-matrix/scripts/render-smoke-check.sh` |
| **Quick run command** | `cmake --build --preset ci-{platform} --target BALL VIEW BALLView` |
| **Full suite command** | CI build matrix (macOS / Linux / Windows) + `render-smoke-check.sh` (macOS + Linux) |
| **Estimated runtime** | ~8–15 min per OS in CI (cold) / ~2–4 min incremental local |

There is no unit-test framework wired for VIEW rendering. All Qt 6 + spike validation is **integration-level** (build + smoke check + GL diagnostic grep) per Phase 02.2 precedent.

---

## Sampling Rate

- **After every task commit:** `cmake --build --preset ci-{host} --target BALL VIEW BALLView` (compile correctness)
- **After every plan wave:** Full CI run on the affected matrix entries — build matrix + smoke check on macOS + Linux
- **Before `/gsd-verify-work`:** All 3 OS build jobs green AND smoke check produces non-blank PNG on macOS + Linux AND `gl_profile=compatibility` appears in `BALLVIEW_GL_DIAG` stdout
- **Max feedback latency:** ~4 min for a local incremental build; ~15 min for a full CI matrix

---

## Per-Task Verification Map

| Task ID | Plan | Wave | Requirement | Threat Ref | Secure Behavior | Test Type | Automated Command | File Exists | Status |
|---------|------|------|-------------|------------|-----------------|-----------|-------------------|-------------|--------|
| 05-XX-XX | Qt6 bring-up | 1 | QT6-01 | — | N/A | Build integration | `cmake --build --preset ci-macos --target BALL VIEW BALLView` | ✅ ci.yml | ⬜ pending |
| 05-XX-XX | Qt6 bring-up | 1 | QT6-01 | — | N/A | Static grep lint | `bash scripts/check-no-legacy-gl-symbols.sh $GITHUB_WORKSPACE` | ❌ W0 (new lint script) | ⬜ pending |
| 05-XX-XX | Qt6 bring-up | 1 | QT6-01 / D-06 / D-08 | — | Compat profile uniform across OSes | Diagnostic assertion | `grep 'BALLVIEW_GL_DIAG.*gl_profile=compatibility' <ci-log>` | ❌ W0 (new CI grep step) | ⬜ pending |
| 05-XX-XX | QRegExp/Desktop port | 1 | QT6-02 | — | N/A | Build integration | Build success (compile error otherwise) | ✅ via build | ⬜ pending |
| 05-XX-XX | Smoke check / D-08 | 1 | QT6-01 (criterion 3) | — | No pixel regression vs Phase 2 | Smoke integration | `bash .../render-smoke-check.sh <BALLView-binary>` | ✅ ci.yml | ⬜ pending |
| 05-XX-XX | Spike backend | 2 | SPIKE-01 | — | Non-blank PNG via spike Kind | Smoke integration | `BALL_SPIKE_BACKEND=GLCore bash .../render-smoke-check.sh <binary>` | ❌ W0 (new CI variant — macOS only, non-blocking) | ⬜ pending |
| 05-XX-XX | Spike backend | 2 | SPIKE-01 | — | Picking returns non-empty object list | Manual | Interactive macOS session — click molecule, verify pick result | Manual only | ⬜ pending |
| 05-XX-XX | Spike backend | 2 | SPIKE-01 | — | Text overlay renders visible glyphs | Manual | Interactive session — confirm overlay visible | Manual only | ⬜ pending |
| 05-XX-XX | Decision record | 2 | SPIKE-02 | — | Decision record exists with required sections | Document review | `ls .planning/phases/05-*/05-SPIKE-DECISION.md` AND grep required headings | ❌ W0 (artifact created at plan time) | ⬜ pending |

*Status: ⬜ pending · ✅ green · ❌ red · ⚠️ flaky*

*Task IDs (`05-XX-XX`) are placeholders — the planner fills these in when PLAN.md files are written. Mapping rows by **Plan + Wave + Requirement** is the contract; row count and grouping may change as plans are sliced.*

---

## Wave 0 Requirements

- [ ] `ci.yml`: Add `jurplel/install-qt-action` step for the `linux-x64` matrix entry (replacing apt Qt6 packages — Ubuntu 24.04 ships Qt 6.4.2, below the D-01 6.5 floor)
- [ ] `ci.yml`: Add a GL-profile assertion step — grep `BALLVIEW_GL_DIAG` stdout for `gl_profile=compatibility` after smoke check (D-06 / D-08 correctness oracle)
- [ ] `scripts/check-no-legacy-gl-symbols.sh`: New static-grep lint script — fails CI if any of `QGLWidget`, `QtOpenGL/qgl.h`, `QRegExp`, `QDesktopWidget`, or `Qt::SkipEmptyParts` appear in `source/` or `include/` after Phase 5
- [ ] CI spike variant: non-blocking macOS-only step that builds with `BALL_SPIKE_BACKEND=GLCore` and runs the smoke check against the spike Kind (proves SPIKE-01 backend renders at all)
- [ ] `.planning/phases/05-.../05-SPIKE-DECISION.md`: Template scaffold for SPIKE-02 output — sections for chosen backend, rationale, per-platform (macOS/Windows) acceptance criteria, scoped PIPE-01 task list

---

## Manual-Only Verifications

| Behavior | Requirement | Why Manual | Test Instructions |
|----------|-------------|------------|-------------------|
| Spike backend picking returns non-empty object list when user clicks a molecule | SPIKE-01 | Picking requires a live GL context, mouse input, and a running event loop; no headless harness exists for VIEW interaction | (1) Launch `BALLView` with `BALL_SPIKE_BACKEND=GLCore`. (2) Open the demo molecule. (3) Click on an atom. (4) Confirm the selection panel shows the picked atom (or equivalent log line). |
| Spike backend text overlay renders visible glyphs | SPIKE-01 | Overlay is a visual property — pixel-perfect comparison is brittle and pulls in PIPE-01 scope | (1) Launch `BALLView` with each spike Kind. (2) Confirm text overlay is legible at default DPI on Apple Silicon and on a Windows test machine. (3) Record screenshots in the decision record. |
| Per-platform driver behaviour (macOS Apple Silicon, Windows Intel iGPU / NVIDIA) | SPIKE-01 (criterion 4) | Driver-specific quirks (GL_VERSION strings, MSAA support, framebuffer formats) require physical / VM access to representative hardware | Run BALLView on each platform with `BALLVIEW_GL_DIAG` enabled; record `gl_vendor`, `gl_renderer`, `gl_version`, `gl_profile`, MSAA samples, and any console warnings in the SPIKE decision record. |
| macOS GL-deprecation warnings remain visible at runtime | D-07 | Behavioural / human-observed | Launch BALLView on macOS; confirm the deprecation banner / log lines appear (no `GL_SILENCE_DEPRECATION` suppression). |

---

## Validation Sign-Off

- [ ] All tasks have `<automated>` verify or Wave 0 dependencies
- [ ] Sampling continuity: no 3 consecutive tasks without automated verify (build is the default automated probe — every task touching code triggers a `cmake --build` per Phase 02.2 convention)
- [ ] Wave 0 covers all MISSING references (5 items above)
- [ ] No watch-mode flags (CI is one-shot)
- [ ] Feedback latency < 15 min (full CI matrix); < 4 min (incremental local)
- [ ] `nyquist_compliant: true` set in frontmatter when planner has filled in real task IDs

**Approval:** pending
