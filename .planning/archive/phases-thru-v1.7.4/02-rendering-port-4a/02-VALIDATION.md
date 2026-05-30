---
phase: 2
slug: rendering-port-4a
status: draft
nyquist_compliant: true
wave_0_complete: false
created: 2026-05-14
---

# Phase 2 — Validation Strategy

> Per-phase validation contract for feedback sampling during execution.
> BALL has no automated GUI/render test harness and none should be built this
> phase. Validation is build-and-grep-gated, with a mandatory human visual
> smoke check on each of the 3 platforms.

---

## Test Infrastructure

| Property | Value |
|----------|-------|
| **Framework** | None for VIEW/rendering. BALL's `test/` tree is `EXCLUDE_FROM_ALL` and untriaged (deferred to Phase 9). No GL/widget tests exist. |
| **Config file** | none — no framework install needed |
| **Quick run command** | `cd build && make VIEW -j8` (compile gate) + targeted `grep` symbol gate |
| **Full suite command** | `cd build && make clean-VIEW 2>/dev/null; make BALL VIEW BALLView -j8` then manual launch |
| **Estimated runtime** | ~60–120 s incremental VIEW build; ~5 min full clean VIEW+BALLView |

---

## Sampling Rate

- **After every task commit:** `cd build && make VIEW -j8` — the changed TUs must compile clean. For static-cleanup tasks, also run the `grep` symbol gate for the symbol(s) that task removes.
- **After every plan wave:** `make BALL VIEW BALLView -j8` from a clean VIEW (`make clean` of VIEW first — base-class swap changes vtable/moc layout), then launch BALLView once on macOS and confirm the scene renders embedded.
- **Before `/gsd-verify-work`:** Full clean build green on macOS + the manual smoke checklist passes.
- **Max feedback latency:** ~120 s (incremental VIEW build)

---

## Per-Task Verification Map

| Task ID | Plan | Wave | Requirement | Test Type | Automated Command | Status |
|---------|------|------|-------------|-----------|-------------------|--------|
| 2-XX-XX | — | — | RENDER-01 | static + compile | `grep -rn "QGLWidget\|QGLFormat\|QGLContext\|QGL::\|QtOpenGL/qgl" source/VIEW include/BALL/VIEW` → 0 hits; `make VIEW` succeeds | ⬜ pending |
| 2-XX-XX | — | — | RENDER-03 | static | `grep -rn "swapBuffers\|setAutoBufferSwap\|safeBufferSwap" source/VIEW` → 0 hits | ⬜ pending |
| 2-XX-XX | — | — | RENDER-07 | compile | `make VIEW -j8` builds the 5 changed TUs (glRenderWindow.{h,C}, renderSetup.C, scene.C, glOffscreenTarget.C, glRenderer.C) clean | ⬜ pending |
| 2-XX-XX | — | — | RENDER-02/04/05/06/08 | manual-GUI | see Manual-Only Verifications | ⬜ pending |

*Plan/Wave/Task-ID columns are filled in by the planner. Status: ⬜ pending · ✅ green · ❌ red · ⚠️ flaky*

---

## Wave 0 Requirements

*No test framework install required.* One scripted asset should be created early so RENDER-01/03/07 regressions are caught automatically:

- [ ] A `grep`-based "no legacy Qt GL symbols" check (shell snippet or script) covering `QGLWidget`, `QGLFormat`, `QGLContext`, `QGL::`, `QGLPixelBuffer`, `QtOpenGL/qgl`, `swapBuffers`, `setAutoBufferSwap`, `safeBufferSwap` across `source/VIEW` + `include/BALL/VIEW`. Cheap; can later become a CI lint.

Building a GL/GUI test harness is explicitly **out of scope** for this phase.

---

## Manual-Only Verifications

Final visual confirmation requires a human running the GUI. RENDER-02/04/05/06/08 cannot be automated — no GL test harness exists and none should be built this phase.

| Behavior | Requirement | Why Manual | Test Instructions |
|----------|-------------|------------|-------------------|
| Molecule renders embedded in the main window (not a detached/blank window) | RENDER-02 | Requires visual inspection of a running GUI; this is the Core Value | Launch BALLView with `BALL_DATA_PATH`/`BALLVIEW_DATA_PATH` set; startup demo builds a peptide — confirm it is visible inside the Scene dock area |
| Rotate / zoom / pick / select | RENDER-06 | Requires interactive input on a running GUI | Left-drag rotates; scroll/zoom changes view; click-pick selects an atom |
| Raytracer output blits as a texture | RENDER-04 | Requires switching renderer in the GUI and observing | Switch to the raytracer renderer — raytraced image appears in the scene area |
| On-screen text via QPainter overlay | RENDER-05 | Requires visual inspection | Enable FPS/info text — text is legible over the 3D scene |
| Builds + renders on Linux and Windows | RENDER-08 | Build is automatable in CI; render confirmation needs a human per OS | Build on Linux + Windows (green); human visual check repeats RENDER-02/04/05/06 on each |
| No flood of GL errors | RENDER-01/07 | Console observation on a running GUI | `checkGL()` output shows no error spam during a render session |

---

## Validation Sign-Off

- [ ] Every static/compile task has an `<automated>` verify command (grep or `make`)
- [ ] Sampling continuity: every task commit runs at least `make VIEW -j8`
- [ ] Wave 0 "no legacy Qt GL symbols" grep gate created
- [ ] Manual smoke checklist run on macOS before phase verification; Linux + Windows builds green
- [ ] `nyquist_compliant: true` set in frontmatter

**Approval:** pending
