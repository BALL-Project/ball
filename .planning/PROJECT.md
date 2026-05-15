# BALLView 1.6 Modernization

## What This Is

BALL (Biochemical ALgorithms Library) is a C++ molecular modelling framework, and
BALLView is its OpenGL-based molecular visualization GUI application. This project
modernizes the BALL/VIEW/BALLView stack — frozen at a 2022 commit against ~2016-era
dependencies — so it builds, runs, and renders on current macOS (Apple Silicon),
Linux, and Windows toolchains, culminating in a shippable v1.6 release.

## Core Value

BALLView must **build and visibly render molecules** on macOS, Linux, and Windows
from current, supported dependencies — the 3D scene working cross-platform is the
non-negotiable outcome.

## Requirements

### Validated

<!-- Confirmed working as of this review (2026-05-14), built locally on macOS Tahoe arm64. -->

- ✓ Core `libBALL` builds against modern toolchain (Boost 1.90, Eigen 5, Clang/C++14) — existing+patched
- ✓ `libVIEW` builds against Qt 5.15 — existing+patched
- ✓ `BALLView.app` builds and launches on macOS Tahoe (arm64), runs FragmentDB + AMBER demo — existing+patched

### Active

<!-- v1.6 scope. Canonical numbering = GSD phases in .planning/ROADMAP.md.
     The original ROADMAP-1.6.md "4a/4b" labels are kept only as aliases. -->

- [x] **GSD Phase 1** — Build baseline: commit the 8 modern-toolchain patches, bump version, BUILD-macos.md *(done)*
- [x] **GSD Phase 2** *(orig. "4a")* — Port `GLRenderWindow` `QGLWidget`→`QOpenGLWidget`; hybrid threading; keep fixed-function GL via a compat profile *(done — human-verified on macOS; RENDER-08 Linux/Windows carry-forward to 02.2)*
- [ ] **GSD Phase 02.1** — Renderer boundary extraction (`RenderSurface`/`RendererFactory`); makes Phase 5/05.1 a contained swap *(inserted; depends on 2)*
- [ ] **GSD Phase 02.2** — CI & build-smoke matrix: early 3-OS GitHub Actions build + render smoke check + GL-capability diagnostics *(inserted per Codex review — regression net before Phases 3-9)*
- [ ] **GSD Phase 3** — Language modernization: move off the C++14 bridge to C++17, remove `std::unary_function`/`bind2nd`/etc. (7 known files)
- [ ] **GSD Phase 4** — Dependency system overhaul: delete `ball_contrib`, Homebrew/system + vcpkg, config-mode `Find*.cmake`, `CMakePresets.json`, feature matrix
- [ ] **GSD Phase 5** *(orig. "4b")* — Qt 6 migration; deprecated VIEW APIs replaced; compat-profile GL path kept working *(split from the old oversized "Qt 6 + Pipeline")*
- [ ] **GSD Phase 05.1** — Renderer backend decision spike: prototype GL-core vs QRhi behind the 02.1 boundary; recorded decision *(de-risks the v2 `PIPE-01` rewrite)*
- [ ] **GSD Phase 6** — Python bindings: decision-first vertical slice (5-10 core classes), then commit to a generator
- [ ] **GSD Phase 8** — Packaging & distribution: notarizable macOS bundle; documented Linux/Windows build-from-source; license review
- [ ] **GSD Phase 9** — Test suite triage: wire the `test/` tree into `ctest`, triage failures *(the build matrix moved to 02.2)*

*(GSD Phase 7 "Networking Rework" deferred to backlog 999.3 per the Codex review — not core value, the Asio code already compiles. Backlog also: 999.1 UI maintainer questions, 999.2 Ninja generator.)*

### Release Policy

- **1.6 = modernized foundation.** This milestone makes BALL/BALLView build and render on current toolchains (macOS/Linux/Windows) — it is *not* a UI-polish release.
- **1.6.1 = strict corrective patch** — re-ships v1.6.0 with the Windows installer fixed, macOS Info.plist populated, `release.yml` matching the Qt 6 codebase, the config color-defaults user bug fixed (Phase 4.1), and Phase 5.1's source-level cleanup verified on tri-OS CI.
- **1.7 = "BALLView Refresh"** — the UI/UX modernization (SEED-001), a separate milestone gated on GSD Phase 5 (Qt 6).
- **2.0 = substrate modernization + breaking-API cleanup.** Carries the post-v1.7 infrastructure transitions:
  - **999.2** — Ninja replaces Make/MSBuild as the default CMake generator (faster + more uniform cross-platform builds)
  - **999.6** — PIPE-01 renderer pipeline rewrite (fixed-function GL → QRhi)
  - **999.9** — INIFile → YAML config-format migration
  - **999.10** — Remote-control architectural cleanup (deprecate TCP server/client from libBALL + libVIEW; narrow REST server moves INTO BALLView the application; PyBALL SDK class wraps it for Python users; supersedes 999.3)
  - **999.11** — In-tree Flex/Bison mmCIF parser → [gemmi](https://gemmi.readthedocs.io) (drops a custom-grammar maintenance liability; gains modern PDB-tested parser used by CCP4, Phenix, RDKit-CIF)
  - **999.12** — Audit + remove (or un-deprecate) all `BALL_DEPRECATED` / `BALL_VIEW_DEPRECATED` markers (~30 files across DOCKING/GENETICDOCK, VIEW/WIDGETS, VIEW/DIALOGS, VIEW/RENDERING, PYTHON). v2.0 is the natural breaking-removal slot; Plan-05.1-09 precedent: un-deprecate (not delete) when no replacement exists and the class is still load-bearing. Runs LAST in v2.0 so the other phases' deprecation removals (999.6 / 999.10 / Phase 6) have already cleared their slices.

  All six are substrate or correctness-sensitive changes that benefit from being grouped under a single major-version bump where users expect breaking-but-documented behavior changes (build-tooling switch, render backend swap, file-format churn, wire-protocol + library-API change, mmCIF parser swap, deprecated-API removal). Phase 5 SPIKE-02 locked the GL-Core → QRhi split-pattern for v2; 999.2 / 999.9 / 999.10 / 999.11 / 999.12 keep the v2 theme coherent. 999.10 also reshapes the library boundary — remote control is no longer a libBALL concern. 999.12 hardens the API: the deprecation markers stop being a "someday" signal and become an enforced "removed in v2.0."
- This resolves the version-numbering collision with the Claude Design Handover package (which internally assumed "1.6 = UI refresh").

### Out of Scope

- Programmable-pipeline GL rewrite of `glRenderer.C` (`PIPE-01`) — v2; Phase 2 keeps fixed-function via a compat profile, Phase 5 keeps it working under Qt 6
- Reviving the `ball_contrib` source-tarball build system — dead end on modern toolchains; replaced by system/Homebrew deps
- New molecular-modelling features or algorithms — this milestone is modernization only
- RTfact raytracer revival (Windows-only contrib) — not load-bearing
- `TCPServer` proper rework + test (`NET-01`) — deferred to 1.6.x / backlog 999.3

## Context

- **Origin:** Full code review performed 2026-05-14. Findings + roadmap at `/Users/kohlbach/Claude/BALL/ROADMAP-1.6.md`.
- **Repos:** `BALL-Project/ball` (last commit 2022-05-24) and `BALL-Project/ball_contrib` (dep snapshot ~2016-2018), cloned under `/Users/kohlbach/Claude/BALL/`.
- **Build approach adopted:** `ball_contrib` abandoned; build against Homebrew packages (`qt@5 boost eigen fftw tbb glew open-babel lp_solve libsvm flex bison`).
- **8 patches already applied locally** (uncommitted, shallow clone): CMake policy/version modernization, Boost config-mode, Eigen 5 version-header fix, C++11→C++14, Boost.Asio API breakage in `networking.{h,C}`, a latent `contourSurface.h` bug.
- **The blank-scene blocker (Phase 4a target):** `Scene` embeds `GLRenderWindow : QGLWidget`. macOS Tahoe force-enables layer-backing on every NSView; `QGLWidget` (NSOpenGLContext-based, not layer-backed) gets promoted to a detached 100×30 native window. Renderer draws into an invisible context. Confirmed via Qt `-l` platform logging.
- **Rendering architecture:** `RenderSetup` is a `QThread` (one per renderer). Worker threads `makeCurrent()` + render; GUI thread swaps buffers. `GLRenderer` uses ~100 fixed-function GL calls. The raytracer path already renders to a CPU pixel buffer that gets blitted as a texture (`glRenderWindow.C::refresh()`).
- **Build/run commands** that work today are documented in `ROADMAP-1.6.md`.

## Constraints

- **Compatibility**: Must build and render on macOS (Apple Silicon), Linux, and Windows — platform independence is a hard requirement, no per-OS graphics code if avoidable.
- **Tech stack**: C++ / CMake / Qt. GSD Phase 2 stays on Qt 5.15 + fixed-function GL (compat profile); Qt 6 is GSD Phase 5.
- **C++ standard**: Currently a load-bearing **C++14 bridge** (GSD Phase 1). The real risk of staying on C++14 is *not* an ABI clash — Boost/Eigen/Qt are consumed through headers/stable ABIs — it is **language-mode-conditional header APIs, removed STL adapters, Qt 6 build assumptions, and toolchain-default drift across the 3 compilers**. GSD Phase 3 moves to C++17 and should run early (no rendering dependency).
- **Dependencies**: System/Homebrew packages on macOS/Linux, vcpkg on Windows. No reliance on the dead `ball_contrib`.
- **Risk**: GSD Phase 2 touches the threaded renderer — the highest-risk area. Threading rework must be incremental and verifiable against a running GUI.

## Key Decisions

| Decision | Rationale | Outcome |
|----------|-----------|---------|
| Abandon `ball_contrib`, use Homebrew/system deps | 2016-era source tarballs (Qt 5.10, Boost 1.60…) don't build on macOS Tahoe / Apple Silicon | ✓ Good — full stack built |
| Bridge to C++14 (not C++17) for the build baseline | BALL's legacy code uses `std::unary_function`/`bind2nd` removed in C++17; C++14 still has them | — Pending — Phase 2 removes the bridge |
| Phase 4a: `QOpenGLWidget` + compat profile, defer Qt6/pipeline to 4b | Smallest change that restores rendering on all 3 OSes; decouples the macOS blocker from the large pipeline rewrite | — Pending |
| Phase 4a threading: hybrid (interactive GL on GUI thread, raytracer on CPU-buffer worker thread) | `QOpenGLWidget` requires default-FBO rendering on the GUI thread; the raytracer already works via a CPU buffer, so only the GL renderer needs to move | — Pending |

## Evolution

This document evolves at phase transitions and milestone boundaries.

**After each phase transition** (via `/gsd-transition`):
1. Requirements invalidated? → Move to Out of Scope with reason
2. Requirements validated? → Move to Validated with phase reference
3. New requirements emerged? → Add to Active
4. Decisions to log? → Add to Key Decisions
5. "What This Is" still accurate? → Update if drifted

**After each milestone** (via `/gsd-complete-milestone`):
1. Full review of all sections
2. Core Value check — still the right priority?
3. Audit Out of Scope — reasons still valid?
4. Update Context with current state

---
*Last updated: 2026-05-14 after initialization*
