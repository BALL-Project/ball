# Roadmap: BALLView 1.6 Modernization

**Created:** 2026-05-14
**Granularity:** standard
**Core Value:** BALLView must build and visibly render molecules on macOS, Linux, and Windows from current, supported dependencies.

This roadmap mirrors the human-authored `/Users/kohlbach/Claude/BALL/ROADMAP-1.6.md`. Phase 4a (rendering port) is broken out as its own dedicated GSD phase because it is the immediate execution priority â restoring the blank 3D scene. It is sequenced right after the build baseline because it only needs the working build that already exists, not the C++17 or dependency-overhaul work.

## Phases

- [x] **Phase 1: Build Baseline** - Commit the 8 modern-toolchain patches, bump version, document the macOS build flow
- [x] **Phase 2: Rendering Port (4a)** - Port `GLRenderWindow` from `QGLWidget` to `QOpenGLWidget` so the embedded 3D scene renders on all 3 OSes *(complete — human-verified on macOS; RENDER-08 Linux/Windows render-check carry-forward)*
- [x] **Phase 02.1: Renderer boundary extraction** - Extract a Qt-GL-free `RenderSurface`/`RendererFactory` boundary so Phase 5 is a contained backend swap *(inserted; depends on Phase 2, blocks Phase 5)* (completed 2026-05-14)
- [x] **Phase 02.2: CI and build-smoke matrix** - Early GitHub Actions build matrix (macOS-arm64/Linux/Windows) + a non-blank render smoke check + a GL-capability diagnostics log *(inserted per Codex review — pulled forward so Phases 3-9 land with a regression net)* (completed 2026-05-14)
- [x] **Phase 3: Language Modernization** - Move the codebase to C++17, remove C++17-removed constructs, set the standard via CMake (completed 2026-05-14)
- [x] **Phase 4: Dependency System Overhaul** - Delete `ball_contrib`, adopt Homebrew/system + vcpkg, config-mode `Find*.cmake`, `CMakePresets.json`, feature matrix (completed 2026-05-15; Windows CI green + blocking on run 25899905204)
- [x] **Phase 4.1: Config Color-Defaults Fix** - Stop persisted `~/.BALLView` config from silently shadowing compiled element/residue color defaults *(promoted from backlog 999.4 — real user-facing bug)* *(2026-05-15: 2 initial plans landed, verifier flagged 3 BLOCKERs (CR-01 override-signature mismatch made read path dead code; CR-02 `getSectionLastLine` off-by-one; CR-03 unguarded parse throws); 3 gap-closure plans (04.1-03/04/05) closed all three plus 4 bundled warnings; second-pass verifier confirmed 8/8 must-haves at runtime. Total: 5 plans, 5 waves, ~60 min.)* (completed 2026-05-15)
- [x] **Phase 5: Qt 6 Migration + Renderer Backend Spike** - Build against Qt 6 and replace deprecated VIEW APIs (keep the compat-profile GL path working), then a time-boxed renderer-backend decision spike behind the Phase 02.1 boundary *(former Phase 05.1 folded in — it must prototype against Qt 6)* *(complete 2026-05-15; 8 plans 05-01..05-08; SPIKE-01 delivered with documented caveats per the PIPE-01 downstream-init blocker; SPIKE-02 decision: split-pattern — GL-Core for v1.6.x → QRhi for v2)*
- [x] **Phase 5.1: Build Warnings & Latent Bug Cleanup** - Fix latent bugs and tame the warning surface surfaced by Phase 4's tri-OS CI (C4717 `getline` recursion, C4311 pointer truncation on Windows, C4910 dll-export mismatch, `-Wself-assign-field`, `-Wformat-overflow`); Codex CLI cross-checked. *(inserted 2026-05-15 — captures Phase 4 follow-ups; runs after Phase 5 so Qt 6 deprecation noise clears first; complete 2026-05-15 with 14/14 plans landed — full Tier A bug fixes + Tier B Windows DLL hygiene + Tier D build configuration; retroactive Windows CI validation expected on next clean tri-OS run once the Linux `aqtinstall` Qt 6.5.3 cancellation cascade is fixed separately)*
- [x] **Phase 999.2: Ninja build generator switch** — Switched all three `ci-*` CMake presets from MSBuild/Make → Ninja so Windows CI stops paying the 77-min MSBuild tax and the already-wired `COMPILER_LAUNCHER=ccache` becomes load-bearing. **Result: Windows Build dropped from 4818s cold → 55s warm (87× speedup; ~98.9% effective ccache hit rate). Windows total job ~4.7min, well under the 10min standing-disable threshold.** Cold-cache builds still ~85min total, but amortized cost across a dev-iteration cycle (1 cold + many warm) is dramatically lower. Pure CI-side tooling change, zero source impact. Verified on CI run [25953405453](https://github.com/BALL-Project/ball/actions/runs/25953405453) (attempt 1: cold-cache green; attempt 2: warm-cache 55s Windows Build). See [999.2-SUMMARY.md](phases/999.2-ninja-generator-switch/999.2-SUMMARY.md). Completed 2026-05-16.
- [ ] **Phase 6: Python Bindings (autowrap+Cython vs nanobind bake-off, TARGETED FOR v2.1 — FIRST STEP OF v2.1)** — Empirically decide the binding generator by building both against a 7-case cross-platform BALL slice. Tool decision is the deliverable. Detailed plan: [`PYBALLV2.md`](PYBALLV2.md) §6. Moved out of v1.6.x to v2.1 per 2026-05-16 user direction ("PyBALL changes should move to 2.x"). Sequenced as the first step of the v2.1 PyBALL milestone, BEFORE [Phase 999.15](#phase-99915-pyball-wrapping-rewrite-backlog--targeted-for-v21--conditional-on-phase-6-bake-off) bulk wrap.
- [ ] ~~**Phase 7: Networking Rework**~~ - **Deferred to backlog 999.3** — not core value, the Asio code already compiles (Phase 1); the proper rework + test is 1.6.x polish
- [ ] **Phase 8: Packaging & Distribution (TARGETED FOR v1.7)** - Notarizable macOS bundle (`data/` embedded, `macdeployqt`); signed Windows installer (SignPath Foundation); documented build-from-source for Linux/Windows; license/distribution review. Stays whole — no v1.6.2 carve-out per 2026-05-16 user direction. Blocks Phase 999.8 (auto-update).
- [x] **Phase 9: Test Suite Triage** - Wire the `test/` tree into `ctest` and triage failures *(the build matrix moved to Phase 02.2)* (completed 2026-05-16)
- [x] **Phase 999.50: v1.7.2 build acceleration — Ward serializer de-boost** - Hand-roll a compact reader/writer for `poseClusteringSerialization.C` to drop the heaviest Boost compile-time component (v1.7.1 quarantined it; v1.7.2 eliminates it); regenerate the `.dat` fixture in the new format; add release.yml ccache reuse. Canonical plan: [`v1.7.2-BUILD-ACCEL-PLAN.md`](v1.7.2-BUILD-ACCEL-PLAN.md) (completed 2026-05-21)
- [x] **Phase 999.51: Inspector controller cut-overs — Clipping + Label (v1.7.x)** - Wire the latent ClippingController/LabelController to functional apply() + add their Inspector sections (mirror Model/Coloring/Material). Reuses the legacy Clipping/Label dialogs. (completed 2026-05-21)
- [x] **Phase 999.52: Inspector material transparency control (#527) (v1.7.x)** - Add the missing transparency control to the Inspector materialSection, superseding the broken legacy MaterialSettings slider. (completed 2026-05-21)
- [ ] **Phase 999.53: Legacy dialog deletion (Wave-4 cleanup tail) (v1.7.x)** — ⛔ **BLOCKED.** The 9-file cluster's lynchpin `displayProperties.C` is still on the production rendering path (`createModelProcessor`/`createColorProcessor` factories) and referenced by `modelController`/`coloringController`/`representationManager`/`mainControl`/`molecularControl`/`geometricControl`. 999.51 only cut over Clipping+Label; deletion requires the FULL controller cut-over (the deferred 999.44-04a..04h set) to relocate the factory logic off `displayProperties.C` first — a naive deletion broke the build before (commit 7f56f80d89). Not a deletion sweep. **Prerequisite: Phase 999.57** (per maintainer direction 2026-05-21 to keep the deletion in v1.7.2). *Depends on 999.57.*
- [x] **Phase 999.54: BALLView startup warning when no renderer plugins found (#501) (v1.7.x)** - Emit a Log/status-bar warning on empty renderer-plugin discovery. (completed 2026-05-21)
- [x] **Phase 999.55: Build acceleration — PCH across BALL + VIEW (v1.7.x)** - Investigated extending PCH to the test build; the library PCH (the real win) already shipped in 999.16. Test-suite `REUSE_FROM` PCH was tried and **reverted** (it injects the library's `NDEBUG` into tests, broke `Debug_test` on GCC). Net outcome: doc reconciliation + dropped the fragile test-PCH. (closed 2026-05-21)
- [ ] **Phase 999.56: Refresh the PDF tutorial for the modernized stack (#560) (v1.7.x)** - Update tutorial content/screenshots/URLs/build flow to v1.7.x.
- [x] **Phase 999.57: Complete Inspector controller cut-over — unwire displayProperties.C from the rendering pipeline (v1.7.x) — PREREQUISITE FOR 999.53** - Relocate `createModelProcessor`/`createColorProcessor` (the rendering-path factories at displayProperties.C:451/487) out of the legacy Model/Coloring settings dialogs into the Inspector/Controller layer; and the displayProperties references in mainControl/mainframe. Highest-risk (touches the threaded renderer) — incremental + verifiable per CLAUDE.md. **Planner scope-corrected:** scene.C's MaterialSettings/LightSettings/StageSettings are NOT dead (load-bearing for the Preferences stack + the entire stereo render path) — split out as 999.58. 3 plans created; unblocks deleting the Model/Coloring/displayProperties triples. **Code+build+audit COMPLETE 2026-05-21** (all 3 plans landed; render path grep-proven detached; BALLView.app links clean BALL_UI_V2=ON) — **but Plan 03's BLOCKING running-BALLView render UAT is PENDING**, so VIEW-CLEAN-02 stays PARTIAL and the phase is not yet closed. (completed 2026-05-22)
- [x] **Phase 999.58: scene.C settings-dialog cut-over — Material/Light/Stage incl. the stereo render path (v1.7.x) — 2nd PREREQUISITE FOR 999.53** - Relocate scene.C's `MaterialSettings`/`LightSettings`/`StageSettings` (lines 141/142/161; load-bearing across ~15 sites incl. the Preferences stack, applyPreferences, and the full stereo rendering path scene.C:2664-3165) into Inspector Stage/Light/Material coverage. Independent high-risk threaded-renderer migration, comparable in size to 999.57; own GUI UAT (lights/materials/stereo must still render). Needed before 999.53 deletes those 3 dialog triples. (completed 2026-05-22)

<!-- v1.7.4 milestone — Inspector depth + the contract (ingested from design handover 2026-05-22; renumbered +1 from handover 999.58-999.63 to clear the 999.58 collision) -->
- [ ] **Phase 999.59: Controller cut-over · command-shaped `apply()` (v1.7.4 · Wave A · GATING)** - Land the command-shaped `Controller::apply()` contract (validate → guard → mutate → notify → invalidate → record reversible payload) on all 8 mutating Controllers + `ApplyGuard`/`ApplyPayload` base types; narrow `Stage`/`Representation` setters to Controller-`friend` scope. Maps to v1.7.x-24. *Depends on 999.57.* See [Phase 999.59 detail](#phase-99959-controller-cut-over--command-shaped-apply-v174--wave-a--gating).
- [ ] **Phase 999.60: Contract tests · cross-surface parity (v1.7.4 · Wave A)** - Stand up the contract-test harness proving the same operation via menu/toolbar/Inspector/Python reaches identical owner postconditions; selection-consumer agreement; re-entrancy; owner-narrowing compile-fail tests; new blocking Linux CI job. Maps to v1.7.x-25. *Depends on 999.59.* See [Phase 999.60 detail](#phase-99960-contract-tests--cross-surface-parity-v174--wave-a).
- [ ] **Phase 999.61: Qt-lifetime audit + conventions doc (v1.7.4 · Wave A)** - Codify QObject-lifetime conventions (`.planning/codebase/QT-LIFETIME.md`), audit the 6 UFG-flagged sites, adopt `QPointer<T>`, ship a `lifetime-discipline` CI lint rejecting `deleteLater()` in dtors. Maps to v1.7.x-26. *Independent (no in-flight dep).* See [Phase 999.61 detail](#phase-99961-qt-lifetime-audit--conventions-doc-v174--wave-a).
- [ ] **Phase 999.62: Opaque-paint contract codification (v1.7.4 · Wave A)** - Convert the UFG-05/09/10 opaque-paint lesson into a structural contract (`WA_OpaquePaintEvent` on every animated-child container, theme token, `THEME.md`, `opaque-paint-lint` CI job). Maps to v1.7.x-35 (new; handover proposed -30, re-minted). *Depends on 999.43.* See [Phase 999.62 detail](#phase-99962-opaque-paint-contract-codification-v174--wave-a).
- [ ] **Phase 999.63: Coloring · value-range histogram (v1.7.4 · Wave B)** - First visible Inspector-depth win: replace the v1.7.x-17 min/max text inputs with a histogram of the active selection's value distribution + 2 draggable handles + 4 presets, bound through `ColoringController::setRange()` → the command-shaped `apply()`. Maps to v1.7.x-17b. *Depends on 999.59.* See [Phase 999.63 detail](#phase-99963-coloring--value-range-histogram-v174--wave-b).
- [ ] **Phase 999.64: Per-section "Reset section" affordance (v1.7.4 · Wave B)** - Add a section-level Reset glyph to every Inspector section header that reverts that section's Controller to method-defaults through the contract (one `ApplyPayload`). Maps to v1.7.x-36 (new; handover proposed -31, re-minted). *Depends on 999.59.* See [Phase 999.64 detail](#phase-99964-per-section-reset-section-affordance-v174--wave-b).

## Phase Details

### Phase 1: Build Baseline
**Goal**: The modern-toolchain build is reproducible, committed, versioned, and documented â a clean starting point for all later work.
**Depends on**: Nothing (first phase)
**Requirements**: BUILD-01, BUILD-02, BUILD-03, BUILD-04
**Success Criteria** (what must be TRUE):
  1. A fresh checkout configures and builds BALL/VIEW/BALLView on macOS (Apple Silicon) from Homebrew dependencies using a single documented command
  2. The 8 modern-toolchain patches are committed to the repo with clear, descriptive messages
  3. The CMake `PROJECT` version declaration reads `1.6.0-dev`
  4. `BUILD-macos.md` exists and walks a new contributor through dependency install and the build/run flow
**Plans**: 1 plan
- [x] 01-01-PLAN.md — Commit the 8 toolchain patches, bump version to 1.6.0-dev, write BUILD-macos.md, verify rebuild

### Phase 2: Rendering Port (4a)
**Goal**: BALLView's embedded 3D scene renders molecules again on all three platforms by moving off the removed-in-Qt6 `QGLWidget`. This is the immediate execution priority and the highest-risk area.
**Depends on**: Phase 1 (needs the working build baseline; does NOT depend on C++17 or the dependency overhaul)
**Requirements**: RENDER-01, RENDER-02, RENDER-03, RENDER-04, RENDER-05, RENDER-06, RENDER-07, RENDER-08
**Success Criteria** (what must be TRUE):
  1. BALLView displays a molecule in the embedded 3D scene on macOS, Linux, and Windows
  2. The scene widget renders inside the main window â no detached/mis-sized native window
  3. The user can rotate, zoom, pick, and select in the scene with mouse and keyboard
  4. The raytracer renderer still produces output via its CPU pixel-buffer path, and on-screen text appears via a `QPainter` overlay
  5. `RenderSetup`, `scene.C`, `glOffscreenTarget.C`, `glRenderer.C`, and `glRenderWindow.{h,C}` compile with zero `QGLWidget`/`QGLContext`/`QGLFormat` references
**Plans**: 4 plans
- [x] 02-01-PLAN.md — Wave 0: grep symbol gate + resolve raytracer-GL open question A1
- [x] 02-02-PLAN.md — Core port: GLRenderWindow base-class swap, QSurfaceFormat compat profile, paintGL, QPainter text overlay, main.C context sharing
- [x] 02-03-PLAN.md — Renderer subsystem cleanup: renderSetup.C, glRenderer.C, glOffscreenTarget.C
- [x] 02-04-PLAN.md — scene.C port + stereo/multi-display guard-defer + human smoke check
**UI hint**: yes

### Phase 02.1: Renderer boundary extraction (INSERTED)

**Goal**: Extract a clean, Qt-GL-free renderer/surface boundary so the Phase 5 pipeline modernization is a contained ~2-file backend swap behind a flag, not a re-touch of scene.C. Pure refactor — no behaviour change.
**Depends on**: Phase 2 (the QOpenGLWidget port must land first so the extracted `RenderSurface` wraps the real post-port widget)
**Blocks**: Phase 5 (incl. its folded-in renderer backend spike) — the contained-swap boundary is its prerequisite
**Requirements**: ARCH-01, ARCH-02, ARCH-03, ARCH-04
**Success Criteria** (what must be TRUE):
  1. A `RenderSurface` interface owns the context-lifecycle verbs (`beginFrame`/`endFrame`); `RenderSetup::makeCurrent()`'s GL-specific body moves behind it
  2. A `RendererFactory` constructs renderers and surfaces by enum; `scene.C` contains zero `new GLRenderWindow` and zero `dynamic_cast<GLRenderWindow>`/`dynamic_cast<GLRenderer>` sites
  3. The `Renderer` interface gains a batched `renderRepresentations_()` + `capabilities()` entry point; existing immediate-mode renderers are untouched (default fan-out preserves behaviour)
  4. BALLView builds and renders identically to post-Phase-2 (pure refactor — same pixels, verified by the Phase 02.2 smoke check)
**UI hint**: no
**Reference**: `.planning/RENDERER-INTERFACE-BOUNDARY.md` (full design)
**Plans**: 3 plans
- [x] 02.1-01-PLAN.md — Extract the `RenderSurface` interface; move `RenderSetup::makeCurrent()`'s GL body behind `beginFrame()/endFrame()` (ARCH-01)
- [x] 02.1-02-PLAN.md — Add the additive batched `Renderer::renderRepresentations_()` + `capabilities()` entry point with a behaviour-preserving default fan-out (ARCH-03)
- [x] 02.1-03-PLAN.md — Add `RendererFactory`; route `Scene` through it and remove the `new GLRenderWindow` / `dynamic_cast<GLRenderWindow>` / `dynamic_cast<GLRenderer>` sites; human-verify identical render (ARCH-02, ARCH-04)

### Phase 02.2: CI and build-smoke matrix (INSERTED)

**Goal**: Stand up an early automated regression net — a 3-platform build matrix plus a render smoke check — so every later phase (C++17, deps, Qt 6, packaging) lands against a gate instead of a promise. Pulled forward from the old Phase 9 per the Codex adversarial review (its #1 finding: "CI is catastrophically late").
**Depends on**: Phase 2 (a building, rendering BALLView to gate on). Independent of Phases 02.1/3/4 — runs in parallel.
**Requirements**: CI-01, DIAG-01
**Success Criteria** (what must be TRUE):
  1. A GitHub Actions matrix configures and builds BALL/VIEW/BALLView on macOS-arm64, Ubuntu, and Windows on every push
  2. A scripted render smoke check launches BALLView headless with a known molecule, captures the framebuffer, and asserts non-blank pixels at the expected viewport size
  3. BALLView logs a startup GL-capability diagnostic (GL vendor/version/profile, `QSurfaceFormat`, device-pixel ratio, default FBO size, selected renderer backend) — both a debugging aid and the smoke check's oracle
  4. The "no legacy Qt GL symbols" grep gate (`check-no-legacy-gl-symbols.sh`) runs as a CI lint
**Note**: On macOS the build matrix uses Homebrew deps; Linux uses system packages; Windows is gated until Phase 4 lands the vcpkg manifest (Windows row starts as `continue-on-error` and becomes required after Phase 4).
**UI hint**: no
**Plans**: 2 plans
- [x] 02.2-01-PLAN.md — DIAG-01 startup GL-capability diagnostic line in `glRenderWindow.C` + the local render smoke-check script
- [x] 02.2-02-PLAN.md — The GitHub Actions 3-OS build matrix: Homebrew/apt deps + caching + ccache, render smoke check wired on macOS/Linux, legacy-GL grep lint job

### Phase 3: Language Modernization
**Goal**: The whole project compiles under C++17 with the standard set the modern CMake way, removing the load-bearing C++14 bridge.
**Depends on**: Phase 1 (build baseline). Independent of Phase 2.
**Requirements**: LANG-01, LANG-02, LANG-03
**Success Criteria** (what must be TRUE):
  1. The codebase compiles cleanly with C++17 and the C++14 bridge flag is gone
  2. No occurrences of `std::unary_function`, `binary_function`, `bind2nd`, `bind1st`, `ptr_fun`, `auto_ptr`, or `mem_fun` remain across the 7 known files
  3. The C++ standard is configured via `CMAKE_CXX_STANDARD`/`target_compile_features`, with no raw `-std=` flags
**Plans**: 3 plans
- [x] 03-01-PLAN.md — Rewrite the 7 C++17-removed STL construct sites as lambdas / dropped base classes (LANG-02)
- [x] 03-02-PLAN.md — Bump CMAKE_CXX_STANDARD to 17, delete raw -std= flags and the blanket -Wno-deprecated-declarations (LANG-03)
- [x] 03-03-PLAN.md — C++17 build-and-fix loop + drive all 3 CI platforms green on the C++17 commit (LANG-01)

### Phase 4: Dependency System Overhaul
**Goal**: All dependencies come from current, supported sources (Homebrew/system on macOS/Linux, vcpkg on Windows) with `ball_contrib` fully removed from the build path, stable per-platform CMake presets, and an explicit feature matrix.
**Depends on**: Phase 1 (build baseline). Benefits from Phase 3 (C++17) since modern dependency headers may require it.
**Requirements**: DEPS-01, DEPS-02, DEPS-03, DEPS-04, DEPS-05, FEAT-01
**Success Criteria** (what must be TRUE):
  1. CMake resolves every dependency via system/Homebrew packages with `ball_contrib` no longer on the build path
  2. A `vcpkg.json` manifest provides the complete Windows dependency set
  3. Stale bundled `Find*.cmake` modules (Boost, TBB, Eigen3, OpenBabel) are replaced with config-mode `find_package` wherever upstream ships a config
  4. Minimum dependency versions are pinned in CMake and documented
  5. A `CMakePresets.json` provides stable configure presets for macOS-Homebrew, Linux-system, Windows-vcpkg, and CI — no more bespoke per-platform configure invocations
  6. A feature matrix (REQUIREMENTS.md) classifies every optional dependency (OpenBabel, TBB, LPSolve, WebEngine, …) as required / optional / removed / deferred, and states what each one's absence disables
**Plans**: 4 plans, 2 waves
- [x] 04-01-PLAN.md — Wave 1: config-mode finder migration (Eigen3/TBB/OpenBabel), ball_contrib de-referencing, min-version pinning (DEPS-01/03/04)
- [x] 04-02-PLAN.md — Wave 1: CMakePresets.json (4 presets) + CI/BUILD-macos.md preset migration + FEAT-01 feature matrix annotation (DEPS-05/FEAT-01)
- [x] 04-03-PLAN.md — Wave 2: OpenBabel 2.x→3.x API port across the 5-file surface + CI exercises it + chemistry smoke check (D-05)
- [x] 04-04-PLAN.md — Wave 2: vcpkg.json manifest + Windows CI provisioning to required-green (DEPS-02)

### Phase 4.1: Config Color-Defaults Fix (PROMOTED from backlog 999.4)

**Goal**: The persisted `~/.BALLView` config no longer silently shadows the compiled-in element/residue color defaults. Users see compiled defaults for any color they have not explicitly customized, and a stale or partial saved color block falls back to compiled defaults instead of being applied blindly.
**Depends on**: Phase 1 (build baseline). Independent of Phase 4 — sequenced here as small, contained polish; the only workaround today is deleting `~/.BALLView`.
**Requirements**: CONFIG-01
**The problem (found 2026-05-14)**: BALLView persists the full element/residue color tables in `~/.BALLView` (`Elements=` / `ResidueNameColors=` lines). On startup the saved map is applied over the compiled `ElementColorProcessor`/`ResidueNameColorProcessor` defaults — so editing the default tables in source and rebuilding has no visible effect if any config exists. The saved map can also be stale or partial: a real case had indices 0-20 (incl. C/N/O/P/S) all `ffffffff` white.
**Success Criteria** (what must be TRUE):
  1. Editing the compiled `ElementColorProcessor`/`ResidueNameColorProcessor` default tables and rebuilding produces visibly updated colors even when a `~/.BALLView` config already exists
  2. A stale or partial saved color block (version/schema mismatch, or implausible all-white indices) is detected and compiled defaults are used instead of the stale map
  3. The user can reset element/residue colors to compiled defaults from Preferences without deleting `~/.BALLView`
**Fix options** (decide during planning): version/checksum the color block with fallback-on-mismatch; a "reset colors to defaults" Preferences action; or persist only user-customized colors as a diff/override set so new compiled defaults show through.
**UI hint**: yes
**Plans**: 2 plans, 2 waves

Plans:
- [x] 04.1-01-PLAN.md — Wave 1: writePreferenceEntries diff/override write + readPreferenceEntries legacy-Elements discard + ElementColorOverrides parse + all-white sanity (D-01, D-03, D-05, D-06)
- [x] 04.1-02-PLAN.md — Wave 2: Reset element colors button (.ui + header slot + slot impl) + residue follow-up backlog stub (D-02, D-04)

### Phase 5: Qt 6 Migration (4b) + Renderer Backend Decision Spike
**Goal**: BALLView builds and runs on Qt 6 with deprecated VIEW APIs removed and the compatibility-profile fixed-function GL path kept working as the known-good backend. Once the Qt 6 build is stable, a time-boxed spike prototypes the renderer backend behind the Phase 02.1 `Renderer`/`RenderSurface` boundary and produces a *recorded decision* (GL-core vs QRhi) — de-risking and scoping the v2 `PIPE-01` rewrite, which stays out of scope. (The spike was formerly standalone Phase 05.1; folded in here 2026-05-14 because it must prototype against Qt 6. Phase 5 itself was split from the old oversized "Qt 6 + Pipeline" per the Codex review.)
**Depends on**: Phase 02.1 (the renderer boundary), Phase 2 (the `QOpenGLWidget` port), Phase 4 (Qt 6 from the modern dependency system)
**Requirements**: QT6-01, QT6-02, SPIKE-01, SPIKE-02
**Success Criteria** (what must be TRUE):
  1. BALLView builds and launches against Qt 6 with all `QGLWidget`-era APIs removed (including `glDisplayList.h`'s `QtOpenGL/qgl.h`)
  2. The user-facing GUI behaves correctly with `QRegExp` and `QDesktopWidget` replaced by Qt 6 equivalents
  3. The 3D scene still renders correctly under Qt 6 via the compatibility-profile fixed-function path (no pixel regression vs. Phase 2, gated by the Phase 02.2 smoke check)
  4. A throwaway prototype renders the demo molecule through at least the leading backend candidate (GL-core and/or QRhi) behind the `RendererFactory`, demonstrating picking and a text overlay; macOS (Apple Silicon) and Windows driver behaviour are checked and recorded
  5. A decision record (`.planning/` doc) names the chosen backend, the rationale, per-platform (macOS/Windows) acceptance criteria, and a scoped task list for the `PIPE-01` full rewrite
**Sequencing note**: the Qt 6 migration (criteria 1-3) lands first; the spike (criteria 4-5) runs against the working Qt 6 build. Split into separate plan waves accordingly.
**UI hint**: yes
**Plans**: 8 plans, 6 waves

Plans:
**Wave 1**
- [x] 05-01-cmake-qt6-bringup-PLAN.md — Wave 1: Rename CMake to Qt 6.5 + add Qt6::OpenGLWidgets; macOS preset + vcpkg manifest port (QT6-01) *(complete 2026-05-15; configure green on macOS-arm64 against Qt 6.11)*

**Wave 2** *(blocked on Wave 1 completion)*
- [x] 05-02-source-api-renames-PLAN.md — Wave 2: Inline QRegExp → QRegularExpression + QDesktopWidget → QScreen + Qt::SkipEmptyParts (D-05 clean break) (QT6-02) *(complete 2026-05-15; 6 files ported, BLOCKER-A/B/C deferred to follow-on plan)*
- [x] 05-03-qsurfaceformat-compat-profile-PLAN.md — Wave 2: QSurfaceFormat::DeprecatedFunctions + QtOpenGLWidgets include (D-06/D-07/D-08/D-09) (QT6-01) *(complete 2026-05-15; 2 files; BLOCKER-C resolved, BLOCKER-A/B/D deferred)*

**Wave 3** *(blocked on Wave 2 completion)*
- [x] 05-04-ci-matrix-and-lint-PLAN.md — Wave 3: ubuntu-24.04 + jurplel/install-qt-action + GL profile assertion + new Qt5-holdout lint (QT6-01/QT6-02) *(complete 2026-05-15; ci.yml + scripts/check-no-legacy-qt6-symbols.sh; static gates green; runtime D-08 oracle wired and ready — exercises itself once BLOCKER-A/B/D land in the implicit BALL bring-up plan)*

**Wave 4** *(blocked on Wave 3 completion)*
- [x] 05-05-glcore-spike-PLAN.md — Wave 4: CoreGLRenderer + RendererFactory::Kind::OpenGL_Core + color-buffer FBO picking + spike CI variant (SPIKE-01) *(complete 2026-05-15; first Qt 6 link-green BALLView build via BLOCKER-A/B/D resolution + Qt 6 API sweep)*

**Wave 5** *(blocked on Wave 4 completion)*
- [x] 05-06-qrhi-spike-PLAN.md — Wave 5: QRhiRenderer + QtRhiSurface + RendererFactory::Kind::QRhi (gated on Qt >= 6.7) (SPIKE-01) *(complete 2026-05-15; Qt 6.11 / Metal backend on macOS-arm64; build green via Qt6::GuiPrivate; downstream-virtual caveat identical to Plan 05-05)*

**Wave 6** *(blocked on Wave 5 completion)*
- [x] 05-07-driver-behaviour-record-PLAN.md — Wave 6: 05-DRIVER-BEHAVIOUR.md — per-platform DIAG + picking/overlay records (SPIKE-01)
- [x] 05-08-spike-decision-record-PLAN.md — Wave 6: 05-SPIKE-DECISION.md — chosen backend + rationale + per-platform criteria + scoped PIPE-01 task list (SPIKE-02) *(complete 2026-05-15; split-pattern decision: GL-Core for v1.6.x → QRhi for v2; 342 lines, 22 data citations; reasonable-call autonomous override per the same pattern as Plan 05-07. Phase 5 COMPLETE pending orchestrator phase-close step. PIPE-01 backlog 999.6 unblocked — dormant gate satisfied.)*

### Phase 5.1: Build Warnings & Latent Bug Cleanup (INSERTED)
**Goal**: Remove the latent bugs and tame the warning surface surfaced by Phase 4's CI matrix once it reached green on all three OSes (run [25899905204](https://github.com/BALL-Project/ball/actions/runs/25899905204), 2026-05-15). Tier A real bugs first (C4717 `getline` infinite recursion, C4311 pointer truncation with end-to-end audit, `-Wself-assign-field`, `-Wtautological-constant-out-of-range-compare`, `-Wformat-overflow` in the CIF parser, `-Wstringop-truncation` audit). Tier B Windows DLL hygiene (C4910 `extern template class BALL_EXPORT` mismatch in `vector3.h` + `atom.h`, then broaden the C4251 pragma after re-measurement, plus C4996 GeneticIndividual and C4834 cleanup). Tier D build configuration (Qt5LinguistTools / Node 20 actions / Linux ccache tar / Windows release `--config Release` / BALLView.app CFBundleIdentifier). Tier C style cleanup deferred until Phase 5 (Qt 6) lands and strips `-Wdeprecated-declarations`.
**Depends on**: Phase 4 (the Windows CI green run that surfaced the warnings), Phase 5 (resolves the bulk of Tier C deprecation noise)
**Requirements**: None — this phase has no formal REQ-IDs. Plans cite BACKLOG.md task IDs (A1-A6, B1-B5, D1-D5).
**Reference**: [`.planning/phases/05.1-build-warnings-and-latent-bugs/05.1-BACKLOG.md`](phases/05.1-build-warnings-and-latent-bugs/05.1-BACKLOG.md) — full warning census, per-task fix proposals, Codex CLI cross-check verdicts
**UI hint**: no
**Plans**: 14 plans, 2 waves

**Wave 1** *(parallel — A1-A6, B1-B2, B4-B5, D1-D5 minus B3)*
- [x] 05.1-01-PLAN.md — A1: C4717 getline infinite recursion in string.iC (drop std::move on rvalue overloads)
- [x] 05.1-02-PLAN.md — A2: C4311 pointer truncation T*→long — end-to-end audit (hash.C + MMFF94 + triangulatedSurface) with downstream storage widened to BALL::PointerSizeUInt *(complete 2026-05-15; 3 source files; chose `BALL::PointerSizeUInt` (global.h:207) over `std::uintptr_t` for codebase consistency — already pervasive in hash.h, persistenceManager.h ×6, timer.h/C, molecularSurfaceGrid.C ~30 sites; no new `<cstdint>` includes needed (transitive via common.h); Codex D-08 audit confirmed only downstream long storage was the function-local HashMap<long,Position> inside MMFF94StretchBend.C::setupStretchBends_() — migrated to HashMap<BALL::PointerSizeUInt,Position>; headers in plan files_modified (MMFF94StretchBend.h + triangulatedSurface.h) audited and confirmed correctly typed — atoms stored as Atom*, indices as Position/Index, no pointer-derived long storage; NOT edited (not a deviation — the Codex-feared header surface didn't exist); out-of-scope discovery serverWidget.h:124 HashMap<unsigned long, Composite*> logged for future Phase 5.1 follow-up; local build green via `cmake --build build/ci-macos --target BALL -j 8` with zero warnings on the 3 modified files; tri-OS CI verification follows on push)*
- [x] 05.1-03-PLAN.md — A3: -Wself-assign-field at pairExpInteractionEnergyProcessor.C:124 (propagate from proc.rdf_parameter_) *(complete 2026-05-15; 1-line surgical fix at line 124; libBALL build green on macos-arm64 / Qt 6.11; commit 066195c)*
- [x] 05.1-04-PLAN.md — A4: -Wtautological-constant-out-of-range-compare in representation.C (asymmetric — preserve DrawingPrecision INVALID=-1 sentinel, drop 3 tautological halves) *(complete 2026-05-15; chose Option A drop; Rule 1 deviation — plan named the `< 0` halves but actual clang warnings were on `> MAXIMAL_*` upper bounds because both macros = 4 while enum max = 3; final conditional `if (drawing_precision_ < 0 || transparency_ > 255)`; VIEW build green on macos-arm64 / Qt 6.11; commit 932bd27)*
- [x] 05.1-05-PLAN.md — A5: -Wformat-overflow in CIFParserParser.y (sprintf → snprintf, 20 instances — BACKLOG said 19, actual was 20) *(complete 2026-05-15; all 20 `sprintf($$, ...)` → `snprintf($$, sizeof($$), ...)` in the Bison grammar; `sizeof($$)` yields CIFPARSER_LINE_LENGTH=2550 via the %union; bison regenerates the .C cleanly; libBALL build green on macos-arm64 / Qt 6.11; zero -Wformat-overflow on the regenerated CIFParserParser.C; commit d46b942)*
- [x] 05.1-06-PLAN.md — A6: -Wstringop-truncation audit of strncpy sites (10 instances, case-by-case) *(complete 2026-05-15; all 10 gcc-flagged sites from CI run 25899905204 audited and remediated via memcpy + explicit nul-termination — 4 PDB writer sites (PDBFileGeneral.C:381 format-string assembly, PDBFileDetails.C:1460/1553 residue_name writers, PDBFileDetails.C:1785 SEQRES residue-name slot) + 6 CIF-lexer sites (CIFParserLexer.l:90/117/135/200/207/223); chose Remediation B (memcpy + explicit nul) over Remediation A (std::string) because every destination is a fixed-width PDB record field or Bison %union char[N] consumed by C-string APIs where std::string conversion would force boundary churn, and rejected Remediation C (#pragma suppress) because the warnings flag a real nul-termination concern; Rule 2 extended Remediation B to 7 unflagged structurally-identical PDBFileDetails.C neighbour sites (1473 "UNK", 1777 nucleotide-replace, 1783 res_name fallback, 1802 helix_ID, 1842/1847 sheet_ID, 1867 turn_ID) and to all 17 strncpy sites in CIFParserLexer.l (introduced static inline `CIF_copy_text()` helper in the .l prelude — single point of truncation policy for all lexer rules); added `#include <cstring>` to both PDB .C files for defence against include-order shuffling; 13 residual unflagged strncpy sites across BALL (DATATYPE/string.C n-1 idiom, COMMON/logStream.C / FORMAT/lineBasedFile.C runtime-derived bounds, CONCEPT/classTest.h .size() bound, FORMAT/PDBdefs.h ×8 short-literal sources) deliberately left untouched — gcc didn't flag them and they're out of A6's scope, documented in SUMMARY for future audits; libBALL build green via `cmake --build build/macos-homebrew --target BALL -j 8`; zero -Wstringop-truncation on the 3 edited files; tri-OS CI verification follows on push)*
- [x] 05.1-07-PLAN.md — B1+B2: C4910 extern template / BALL_EXPORT migration in vector3 + atom (sequenced one plan) *(complete 2026-05-15; moved `BALL_EXPORT` from `extern template class` declarations in vector3.h:1145,1148 + atom.h:1010 to the single-TU `template class` definitions in vector3.C:10,13 + atom.C:680; canonical MSVC DLL-export pattern eliminates C4910-on-vector3.h and per-TU duplicate-emit of std::vector<Atom*>; B1+B2 bundled per CONTEXT.md D-02 since they share the fix pattern; libBALL build green on macos-arm64 / Qt 6.11; tri-OS CI run 25922117642 supervised in background; commit d0f7261)*
- [x] 05.1-09-PLAN.md — B4+B5: C4996 GeneticIndividual deprecation + C4834 regressionModel.C:258 (Tier-B cosmetic) *(complete 2026-05-15; B5: deleted dead `descriptor_IDs_.begin();` call at regressionModel.C:258 — `.begin()` on multiset is a pure observer, real iterator obtained 3 lines below; B4: removed incorrect class-level `BALL_DEPRECATED` from `BALL::GeneticIndividual` (geneticIndividual.h:15) — Remediation A un-deprecate chosen because the class is still load-bearing in `GeneticAlgorithm::pools_`/`template_individual_` and `EvolutionaryDocking` with no replacement class; 11-line documentary comment block above the class records the rationale; B4+B5 bundled per CONTEXT.md D-02; libBALL build green on macos-arm64 / Qt 6.11; tri-OS CI run 25923452178 supervised in background; commit d30feca)*
- [x] 05.1-10-PLAN.md — D1: Qt5LinguistTools / qttools missing on Windows vcpkg *(complete 2026-05-15; combined option (a) + (b) — `qttools` already in `vcpkg.json:8` from D6 root-cause fix, added `QUIET` to `FIND_PACKAGE(Qt6LinguistTools ...)` at `CMakeLists.txt:351` + downgraded `MESSAGE(WARNING)` → `MESSAGE(STATUS)` with clearer remediation pointer; suppresses CMake's built-in "By not providing FindQt6LinguistTools.cmake" warning on Windows Configure even in non-vcpkg edge cases; translation pipeline behaviour unchanged on happy path; `BALLViewTranslations.cmake` already early-RETURNs when tools absent; macOS/Linux unaffected; Rule 2 deviation — applied option (b) on top of already-landed (a) for defence-in-depth)*
- [x] 05.1-11-PLAN.md — D2: Node.js 20 GitHub Actions deprecation (bump checkout/cache pins or set FORCE_JAVASCRIPT_ACTIONS_TO_NODE24) *(complete 2026-05-15; Option A pin bump v4→v5 across both workflow files — 10 line edits total: actions/checkout@v4→v5 ×3 [ci.yml build + ci.yml lint + release.yml], actions/cache@v4→v5 ×3 [ccache + Homebrew + apt], actions/cache/restore@v4→v5 ×2 [ci.yml + release.yml vcpkg restore], actions/cache/save@v4→v5 ×2 [ci.yml + release.yml vcpkg save]; pre-flight `gh api` confirmed v5 GA — checkout latest=v6.0.2, cache latest=v5.0.5; chose conservative v5 over v6-for-checkout to keep delta minimal on the v1.6 release path; deferred upload-artifact@v4→v6 + download-artifact@v4→v7 to a follow-up plan — currently NOT in deprecation annotation surface, and the Node-24 jumps cross breaking-change majors)*
- [x] 05.1-12-PLAN.md — D3: Linux ccache cache-save tar failure (diagnosis-first; fix or no-fix-transient) *(complete 2026-05-15; diagnosis corrected the BACKLOG D3 attribution — the recurring `/usr/bin/tar exit code 2` warning is on the **apt-archives** cache step (`/var/cache/apt/archives`), not the **ccache** cache step which has been saving cleanly all along; recurrence rate = 7/7 (100%) on recent successful Linux runs that reached the post-job cache phase — chronic, not transient; root cause = unprivileged runner cannot read APT's root-owned `lock` (0640) + `partial/` (0700) inside the cached path, tar exits with code 2, cache-action fallback then succeeds (`Cache saved with key` follows the warning so the cache IS still saved — definitionally cosmetic); fix = narrow `path: /var/cache/apt/archives` → `path: /var/cache/apt/archives/*.deb` (1 substantive line + 17 doc-comment lines in `.github/workflows/ci.yml`), addresses the root cause by construction — the `.deb` archives are the only content `apt-get install` downloads and the only content the cache restore needs, lockfile + partial/ are runtime APT state that doesn't belong in a cache; chose `narrow-cached-path` (a 5th option) over the four plan-listed options autonomously per no-clarifying-questions mode because none of split-cache-key / pre-save-cleanup / permissions-fix / no-fix-transient mapped cleanly to apt-cache vs. ccache; YAML parses clean (python3 yaml.safe_load); all plan-11 v5 action pins preserved; commit 1321336)*
- [x] 05.1-13-PLAN.md — D4: Windows release LNK1104 tbb12_debug.lib — add `--config Release` in release.yml + ci.yml *(complete 2026-05-15; **no-op verification plan** — both `--config Release` insertions were already in place in prior commits: release.yml fixed by `381c129` (direct D4 fix on v1.6-modernization) and synced to master via `8b2667a` + `afa00c2`; ci.yml had `--config Release` since `e63f061` (Phase 04-04 Windows wire-up, 2026-05-14) which predates the 2026-05-15 release run that surfaced the gap in release.yml — BACKLOG D4's claim of a parallel ci.yml gap was inaccurate at write-time; closed out per the plan's decision-default policy `"If both files already contain --config Release ... document that and close out with a no-op commit + SUMMARY"`; all success criteria verified — grep matches at release.yml:193 + ci.yml:315, zero Debug remnants, YAML parses clean, plan-11 v5 action pins preserved, plan-12 apt-cache `*.deb` narrowing preserved; downstream verification already validated by `afa00c2` v1.6.0 consolidation — Windows installer shipped, ci.yml Windows job has been continuously green/blocking since `f3802ee`)*
- [x] 05.1-14-PLAN.md — D5: BALLView.app CFBundleIdentifier + bundle-identity fields (Phase 8 notarization prerequisite; choose canonical identifier) *(complete 2026-05-15; Option A `SET_TARGET_PROPERTIES(BALLView ... MACOSX_BUNDLE_*)` block in `source/APPLICATIONS/BALLVIEW/CMakeLists.txt` under `IF(APPLE)` after `TARGET_LINK_LIBRARIES`; chose `de.uni-tuebingen.ball.ballview` per CONTEXT.md D-10 (institutional reverse-DNS — BALL originated at Universität Tübingen — strongest provenance for a Developer ID Application certificate registered to the institution in Phase 8); populates all seven previously-empty bundle-identity fields (CFBundleIdentifier, CFBundleName, CFBundleVersion, CFBundleShortVersionString, CFBundleLongVersionString, CFBundleGetInfoString, NSHumanReadableCopyright) via the pre-existing `cmake/MacOSXBundleInfo.plist.in` template's standard `${MACOSX_BUNDLE_*}` placeholders — template needed no edit, the missing variables were the root cause; CMake configure smoke-tested clean via `cmake -S . -B build/d5-test -DBALL_HAS_VIEW=ON -DBALL_PYTHON_SUPPORT=OFF`; Phase 8 notarization prerequisite cleared — notarytool no longer rejects on empty CFBundleIdentifier and Developer ID code-signing can validate the identifier against the cert's authorized bundle IDs; commit 3c028a8)*

**Wave 2** *(blocked on 05.1-07 measurement)*
- [x] 05.1-08-PLAN.md — B3: C4251 STL-members-of-DLL-exported-classes pragma scope decision (re-measure after B1+B2 land; full / narrow / skip) *(complete 2026-05-15; chose D-06 default `full-pragma` per the plan's explicit fallback policy ("If Build never started... apply the default decision — the pragma is safe-by-default and the measurement can be retroactively validated when CI stabilizes") — two consecutive post-B1+B2 Windows CI runs were cancelled mid-build by the unrelated Linux `aqtinstall` Qt 6.5.3 failure (out of scope per plan context), preventing reliable C4251 re-measurement; added `#pragma warning(disable: 4251)` inside the existing `#ifdef BALL_COMPILER_MSVC` block at `include/BALL/COMMON/global.h:54` with a 14-line rationale comment documenting the Phase 4 vcpkg-pin precondition (single MSVC toolchain + STL ABI across libBALL + every client TU) that makes the disable cosmetic-only; the narrow push/pop window at `exception.h:16-19` becomes redundant on Windows but is kept as belt-and-suspenders; local BALL build green via `cmake --build build/ci-macos --target BALL -j 8`; retroactive validation expected on next clean tri-OS CI run; commit 9c34b24; Phase 5.1 now 14/14 complete)*


### Phase 6: Python Bindings — autowrap+Cython vs nanobind bake-off (TARGETED FOR v2.1 — FIRST STEP)

> **Implementation-ready scope:** [`PYBALLV2.md`](PYBALLV2.md) §6 — the bake-off plan. Read PYBALLV2.md before promoting this phase; the §6 task breakdown (P6-01 SIP behavior inventory + P6-02 autowrap track + P6-03 nanobind track + P6-04 bake-off decision) and the hard stop/pivot gates in PYBALLV2.md §10 are the authoritative scope.

**Goal**: Empirically decide BALL's Python-bindings tool (autowrap+Cython vs nanobind) by running both against a real cross-platform vertical slice of seven BALL classes. **The tool decision is the deliverable**, not a premise. If the chosen tool passes the hard stop/pivot gates, the v2.1 bulk wrap fires as [Phase 999.15](#phase-99915-pyball-wrapping-rewrite-backlog--targeted-for-v21--conditional-on-phase-6-bake-off).

**Depends on**: Phase 3 (C++17 codebase), Phase 4 (modern dependency system), Phase 5 (Qt 6 baseline), Phase 5.1 (build-warning cleanup).
**Requirements**: PY-01, PY-02 (per PYBALLV2.md §6 — see acceptance criteria there).

**Success criteria** (per PYBALLV2.md §6 + §10):
  1. The seven slice cases (Vector3 template, Atom Composite+iterators+ownership, PDBFile exceptions+I/O, HBondProcessor Composite/apply Python-override, RegularData3D NumPy/buffer interop, OWNED-pointer Composite case, StringHashMap mapped container) build green and pass behavior tests on **macOS-arm64 + linux-x64 + windows-x64**, under both candidate tools.
  2. Quantitative comparison table (compile time, wheel size, runtime overhead, lines-of-binding-code, API-parity gap, Windows pain points) recorded in `.planning/phases/06-python-bindings/04-BAKE-OFF-DECISION.md`.
  3. The chosen tool passes ALL PYBALLV2.md §10 hard stop/pivot gates (tri-OS green; behavior spec parity; exception/iterator/ownership parity; perf within 2× of SIP 4 baseline; wheel import ≤ 500 ms).

**Estimated effort:** 7.5 weeks per PYBALLV2.md §2 (P6-01 1.5w + P6-02 3w + P6-03 2.5w + P6-04 0.5w). Possible to land within a single v1.6.x cycle if 2 engineers parallelize the autowrap and nanobind tracks.

**Plans**: TBD (mirror PYBALLV2.md §6's P6-01..P6-04 task breakdown when promoted).

### Phase 7: Networking Rework — DEFERRED TO BACKLOG 999.3
**Status**: Removed from the v1.6 active roadmap per the Codex review. Networking is not core value, and the Boost.Asio code already *compiles* (the API breakage was fixed in Phase 1). The proper `TCPServer` rework + unit test is 1.6.x polish, tracked as backlog **999.3**. `NET-01` moved to REQUIREMENTS.md "Deferred (1.6.x)".
**Plans**: TBD

### Phase 8: Packaging & Distribution (TARGETED FOR v1.7)
**Goal**: BALLView is shippable: a **signed and notarized** macOS bundle plus a **signed** Windows installer, plus an honest, documented build-from-source story for Linux, plus a license/distribution review. v1.6.0 already ships *unsigned* best-effort installers via `release.yml` (commit `a186fb5`) — Phase 8 replaces that with the signed/notarized pipeline so users stop seeing Gatekeeper / SmartScreen warnings. (Original Codex-reviewed scope was macOS bundle + Linux/Windows from-source; expanded 2026-05-15 to include Windows signed installer based on the v1.6.0 release feedback.)
**Depends on**: Phase 5 (Qt 6 build is what gets packaged)
**Requirements**: PKG-01, PKG-02, PKG-03, PKG-04 (Windows signed installer — added 2026-05-15)
**Success Criteria** (what must be TRUE):
  1. `BALLView.app` launches by double-click with no environment variables set, finding its `data/` in `Contents/Resources`
  2. The macOS build produces a `macdeployqt`-processed, **code-signed (Developer ID Application) + Apple-notarized + stapled** universal (arm64 + x86_64) bundle packaged as a **`.dmg`** (via `create-dmg`) with a drag-to-Applications hint and BALL branding — that passes `spctl --assess` and opens without Gatekeeper warnings on a fresh macOS install
  3. The Windows build produces a **code-signed `.exe` installer** (via CPack NSIS — modernizing the existing `cmake/BALLPackageConfig.cmake` scaffolding) that passes `signtool verify /pa`, registers in Add/Remove Programs, places Start Menu shortcuts, supports clean uninstall, and minimizes SmartScreen friction (full SmartScreen-bypass is no longer available post-2024; reputation-building is expected on early releases regardless)
  4. `BUILD-macos.md` is joined by `BUILD-linux.md` and `BUILD-windows.md` documenting the from-source build on each platform
  5. A license/distribution review covers the FFTW GPL path, OpenBabel, Qt deployment mode, bundled `data/`, and the **code-signing chain of trust** (cert provenance, timestamping authority, renewal cadence) — recorded so notarization/distribution is unambiguous
**Decisions locked (2026-05-15)**:
  - **Windows signing provider: SignPath Foundation (Path A)** — free for OSS, BALL's LGPL-2.1 qualifies. Fallback order if rejected: Path B (Azure Artifact Signing, ~$120/yr) → Path C (commercial OV + cloud HSM, $200-500/yr). Avoid commercial EV (over-spec, no SmartScreen bypass post-2024).
  - **macOS signing path: Apple Developer ID Application + notarytool + stapler** — the only path for non-App-Store distribution, $99/yr Apple Developer Program.
**References**:
- [`.planning/phases/08-packaging-and-distribution/08-SIGNING-RESEARCH.md`](phases/08-packaging-and-distribution/08-SIGNING-RESEARCH.md) — full signing & notarization research (macOS Developer ID + notarytool flow, Windows path comparison with decision rationale, CA/B Forum 2026 changes, BALL's LGPL-2.1 SignPath Foundation eligibility, GitHub Actions integration patterns)
- [`.planning/phases/08-packaging-and-distribution/08-INSTALLER-FORMATS-RESEARCH.md`](phases/08-packaging-and-distribution/08-INSTALLER-FORMATS-RESEARCH.md) — installer format research: macOS `.dmg` via `create-dmg` (recommended over hdiutil / dmgbuild), Windows `.exe` installer via CPack NSIS (leverages BALL's existing `cmake/BALLPackageConfig.cmake` scaffolding; WiX/MSI noted as future enterprise upgrade), CMake/CPack integration sketch, coupling with signing flow
**Plans**: TBD

### Phase 9: Test Suite Triage
**Goal**: The existing `test/` tree is wired into `ctest` and its failures are triaged. (The 3-platform *build* matrix moved to Phase 02.2 per the Codex review — this phase is the remaining test-suite work.)
**Depends on**: Phase 02.2 (the CI matrix this extends), Phase 3, Phase 4 (the test tree builds against the modernized toolchain/deps)
**Requirements**: CI-02 (legacy v1 — partially met by the landed CI wiring), `TEST-CLOSE-01` + `TEST-CLOSE-02` (v1.6.2 — the remaining failure-triage + gatekeeper-flip work; see `REQUIREMENTS.md` v1.6.2 section).
**Success Criteria** (what must be TRUE):
  1. The `test/` tree (currently `EXCLUDE_FROM_ALL`) is wired into the build and `ctest` runs in CI on all three platforms
  2. Test failures are triaged — each is fixed, quarantined with a tracking note, or documented as a known modernization casualty
  3. The gatekeeper flips from `continue-on-error: true` to blocking on macOS + Linux once the triaged green-list is stable
**Progress (partial, 2026-05-15)**:
- macOS + Linux test gatekeeper wired into `ci.yml` AND `release.yml` (mirrors). Non-blocking initially (`continue-on-error: true`); flips to blocking once the triaged green-list is stable. Test results uploaded as workflow artifacts (`ball-tests-{macos,linux}.xml` JUnit XML) for every CI run.
- **Linux coverage job** added to `ci.yml` using BALL's existing `ENABLE_COVERAGE_TESTING` option (gcovr HTML + Cobertura XML; 90-day artifact retention).
- **PR test-results check run** via `EnricoMi/publish-unit-test-result-action@v2` — surfaces JUnit XML as an inline check on PRs/pushes.
- **v1.6.0 baseline captured** locally on macOS arm64 (see [`.planning/phases/09-test-suite-triage/PHASE-9-BASELINE.md`](phases/09-test-suite-triage/PHASE-9-BASELINE.md)): **291/294 pass = 99.0%** when `BALL_DATA_PATH` is set. Three real failures need triage as Phase 9 main work: `Directory_test` (macOS path), `AmberFF_test` (2.6% energy off — FP precision suspect), `AssignBondOrderProcessor_test2` (one fine-penalty assertion).
- Windows tests deferred to Phase 9 main work (vcpkg + MSVC test integration is non-trivial).
**Plans**: TBD (Phase 9 main work: triage the failing tests, fix/quarantine/document each, flip gatekeeper to blocking)

## Progress

| Phase | Plans Complete | Status | Completed |
|-------|----------------|--------|-----------|
| 1. Build Baseline | 1/1 | Complete | 2026-05-14 |
| 2. Rendering Port (4a) | 4/4 | Complete — human-verified on macOS (RENDER-08 Linux/Windows carry-forward) | 2026-05-14 |
| 02.1 Renderer boundary extraction | 3/3 | Complete    | 2026-05-14 |
| 02.2 CI and build-smoke matrix | 2/2 | Complete    | 2026-05-14 |
| 3. Language Modernization | 3/3 | Complete   | 2026-05-14 |
| 4. Dependency System Overhaul | 4/4 | Complete — CI green on all 4 jobs (incl. Windows blocking) on run 25899905204 | 2026-05-15 |
| 4.1 Config Color-Defaults Fix | 5/5 | Complete   | 2026-05-15 |
| 5. Qt 6 Migration (4b) + Renderer Backend Spike | 8/8 | Complete — Plans 01-08 complete (CMake bring-up, source renames, QSurfaceFormat compat, CI matrix + Qt5 lint, GL-core spike, QRhi spike + Qt 6 link bring-up, driver-behaviour record, SPIKE-02 decision: GL-Core for v1.6.x → QRhi for v2) | 2026-05-15 |
| 5.1 Build Warnings & Latent Bug Cleanup | 14/14 | Complete — Tier A: C4717 getline + C4311 pointer-trunc audit + -Wself-assign-field + -Wtautological + -Wformat-overflow CIF + -Wstringop-truncation; Tier B: C4910 BALL_EXPORT vector3/atom + C4834/C4996 GeneticIndividual+regressionModel + B3 C4251 pragma; Tier D: D1 Qt5LinguistTools + D2 Node-20 pin bump + D3 apt-cache narrowing + D4 Windows --config Release + D5 BALLView.app CFBundleIdentifier. **Carry-forward (RESOLVED 2026-05-16):** B3 baseline measured at 3495 → 0 on first clean tri-OS green run [25953405453](https://github.com/BALL-Project/ball/actions/runs/25953405453); see [05.1-08-SUMMARY.md](phases/05.1-build-warnings-and-latent-bugs/05.1-08-SUMMARY.md) + [05.1-UAT.md](phases/05.1-build-warnings-and-latent-bugs/05.1-UAT.md). | 2026-05-15 |
| **999.2 Ninja build generator switch** | 6/6 | **Complete** (v1.6.1, promoted + landed 2026-05-16) — Windows Build 4818s cold → **55s warm** (87× speedup, ~98.9% effective ccache hit). Windows total job ~4.7min, under the 10min standing-disable threshold. CI verified on run [25953405453](https://github.com/BALL-Project/ball/actions/runs/25953405453) (attempts 1+2). Zero source impact. See [999.2-SUMMARY.md](phases/999.2-ninja-generator-switch/999.2-SUMMARY.md). | 2026-05-16 |
| **v1.6.2 omnibus** (Qt 6.8 LTS bump, ARM Linux baseline, warning-execution + GL silence, latent bug flush) | — | **Complete** (v1.6.2, 2026-05-16) — 43 commits since v1.6.1 across two parallel work streams. Headline metrics: Qt floor `6.5 → 6.8 LTS` globally (`95a1e9f`); `linux-arm64` (`ubuntu-24.04-arm`) added to CI matrix and flipped to blocking after 2 consecutive greens (`d10da9a`); warning surface cut from ~4 000 (linux-arm64) / ~2 400 (linux-x64) / ~1 800 (macos-arm64) / ~700 (windows-x64) to **<10 actionable warnings on every platform**; coverage Linux job green for the first time (Phase 999.27); real `va_start` UB fix in `PDBFile::writeRecord_` (`c6ecd48`); 6× discarded-bool latent silent-failure fixes in QFile::open() download sites (`82dba3c` + `24c044f`); macOS GL deprecation noise silenced via `GL_SILENCE_DEPRECATION` (`d730518`). Codex CLI adversarial review applied to the immediate-fix shortlist before execution. Final pre-tag verifying run: 25970862407. See [RELEASE-NOTES-v1.6.2.md](../RELEASE-NOTES-v1.6.2.md). | 2026-05-16 |
| 6. Python Bindings (bake-off, v2.1 first step) | 0/4 | Not started — autowrap+Cython vs nanobind bake-off per [PYBALLV2.md](PYBALLV2.md) §6; 7-case slice, tri-OS, 7.5 wk. Moved from v1.6.x → v2.1 per 2026-05-16 user direction ("PyBALL changes should move to 2.x"); runs as FIRST step of v2.1, before 999.15 bulk wrap. | - |
| 7. Networking Rework | — | Deferred to backlog 999.3 | - |
| 8. Packaging & Distribution | 0/0 | Not started | - |
| 9. Test Suite Triage | 1/1 | Complete   | 2026-05-16 |

---

## Backlog

### Phase 999.1: BALLView UI maintainer open-questions (RESOLVED · 2026-05-17)

**Status:** **RESOLVED** — maintainer answers received 2026-05-17 (in-session structured-question flow; no GitHub issue published).

**Answers:**

| # | Question | Answer | Vs Handover rec |
|---|---|---|---|
| Q1 | macOS menu bar | **Keep inline menubar** (cross-platform consistency); `QAction::setMenuRole(...Standard)` for About/Prefs/Quit is fine (per-action standard-role plumbing, not full global-menubar mode) | DEPARTS (rec: native global) |
| Q2 | Classic 5-dock workspace | **Retire after one release** — Phase 999.45 ships both with picker; **Phase 999.49 deletes Classic** post-flag-removal in same v1.7 RC cycle (re-pinned 2026-05-17 audit pull-in from v1.9 → v1.7) | matches rec |
| Q3 | Theme picker scope | **Single neutral theme only** (no Light/Dark/Follow-System) | DEPARTS (rec: Follow-System) |
| Q4 | Translation churn | **Accept; community round during v1.7 cycle** (re-pinned 2026-05-17 audit pull-in from v1.8 → v1.7) | matches rec |

**Impacts (propagated to ROADMAP Phases 999.42/.43/.44/.45/.46/.47/.48/.49 + v1.7-PLAN.md Wave 4):**

- **Q1 → Phase 999.46 (menu reorg + command palette, v1.7 Wave 4):** skip full QMenuBar global-menubar mode. DO `QAction::setMenuRole(QAction::AboutRole / PreferencesRole / QuitRole)` for the 3 standard items on macOS (this is per-action standard-role plumbing — fully compatible with the inline-menubar Q1 answer).
- **Q2 → Phases 999.44 + 999.45 + 999.49 (Inspector + Workspace + Classic-delete, all v1.7 Wave 4):** 999.44 design accommodates Classic-coexistence period; 999.45 ships Default/Classic/Focused picker; **999.49 deletes Classic** in the same v1.7 RC cycle post-999.48 flag removal.
- **Q3 → Phases 999.42 + 999.43 + 999.48 (all v1.7 Wave 4):**
  - 999.42 + 999.43 ship — value reframes from "unlock dark mode" to "design-system consistency + OS palette inheritance" (palette removal still useful even single-theme; Qt applies system widget styling natively).
  - 999.48 sub-deliverable "dark-mode finalize" drops AND §8.6 theme-picker UI drops (no dark mode to finalize, no Light/Dark/Follow-System picker).
  - 999.40 ThemeManager built single-theme — `QStyleHints::colorScheme()` reactor wired but inactive (no harm; future-proofs if maintainer reverses Q3).
  - **999.42 ThemedIconEngine state-tints (Normal/Active/Selected/Disabled) are NOT theme-tints and Q3 does NOT block them.**
- **Q4 → Phase 999.46 (v1.7 Wave 4) timing:** v1.7 ships English + best-effort de_DE; community translation round during v1.7 cycle; v1.7.x point releases pick up completed translations. No v1.7 ship-date blocker. `[i18n]` commit-prefix discipline.

**Original 4 questions (preserved for history):**
  1. macOS menu bar — keep inline, or use the native global menubar via `QAction::setMenuRole`?
  2. Legacy 5-dock "Classic" workspace — keep as a long-term opt-in preset, or retire after one release?
  3. Theme picker — ship one neutral theme, or Light/Dark/Follow-System? (handover recommends Follow-System)
  4. Translation churn — the menu re-org invalidates ~40% of `BALLView-de_DE.ts`; plan a community translation round.

**Requirements:** N/A (process phase, no REQ-IDs).
**Reference:** `.planning/MAINTAINER-QUESTIONS-999.1.md` (full answer record), `.planning/DESIGN-HANDOVER-INTEGRATION.md`, `.planning/seeds/SEED-001-ballview-refresh-ui-milestone.md`, `.planning/v1.7-PLAN.md` Wave 4

Plans:
- [x] N/A — no plan needed; questions were a decision-gathering activity, completed when answers received

### Phase 999.2: Ninja build generator switch (COMPLETE · v1.6.1 · 2026-05-16)

**Status:** Promoted from backlog v2.0 → v1.6.1 active on **2026-05-16**.
Trigger: with Phase 4's Windows `blocking: true` flip, every CI cycle eats
~80 min of Windows wall-clock under MSBuild. Switching to Ninja + ccache
(prep commits already landed: `d5f5566` Windows `--parallel`, `9c932eb`
choco install ccache) pays back its own implementation cost on the first
re-run and makes the standing "Windows >2× macOS/Linux disable" policy
tractable. Pure build-tooling change — zero source changes.

**Goal:** Switch the CI CMake generator from per-platform Make/MSBuild to
**Ninja (single-config)** across all three `ci-*` presets so Windows stops
paying the MSBuild tax and `CMAKE_<LANG>_COMPILER_LAUNCHER=ccache`
(already wired in the `ci-windows` preset since Phase 4) actually takes
effect — MSBuild silently ignores `COMPILER_LAUNCHER`; Ninja honors it.

**Cross-platform impact sketch:**
  - **macOS:** `brew install ninja`; `"generator": "Ninja"` in
    `ci-macos`. Already ccache-warm in ~3 min; Ninja-vs-Make is a wash,
    marginal cold-cache improvement. Low risk.
  - **Linux:** `apt install ninja-build`; `"generator": "Ninja"` in
    `ci-linux`. Same shape as macOS. Low risk.
  - **Windows:** Biggest win — replaces MSBuild (`cmake -G Ninja` after
    `ilammy/msvc-dev-cmd@v1` sets up `vcvars64.bat`). Unlocks
    ccache-on-MSVC (the prep commit `9c932eb` already preinstalls
    ccache; the launcher in the `ci-windows` preset becomes load-bearing
    once Ninja is the generator). Baseline (run 25899905204): 4625s.
  - **CI:** the three `ci-*` presets in `CMakePresets.json` are the
    sole edit surface; local-developer presets unchanged.
  - **Risk:** Low. CMake fully supports Ninja; BALL's BISON/FLEX
    custom commands use the generator-agnostic `BISON_TARGET` /
    `FLEX_TARGET` macros (see [`source/FORMAT/sources.cmake`](../../source/FORMAT/sources.cmake)).
    Best sequenced AFTER Phase 4 (DONE), Phase 5 (DONE), Phase 5.1
    (DONE) — done.

**Milestone target: v1.6.1.** Carrying out a pure-tooling,
zero-source-impact change inside a strict-corrective patch milestone is
acceptable per the v1.6.1 scope discipline
([`MILESTONE-CONTEXT.md` §"Scope discipline"](../MILESTONE-CONTEXT.md)):
risk surface is CI YAML + `CMakePresets.json`, NOT BALL/VIEW source.
Removes the rationale for keeping 999.2 in the v2.0 substrate bundle.

**Requirements:** none (build-tooling change; no REQ delta)
**Plans:** 1/1 plans complete
a multi-PLAN code phase — task IDs 999.2-01..06)

Plans:
- [x] [PLAN.md](phases/999.2-ninja-generator-switch/PLAN.md) — single-file
  phase plan with tasks 999.2-01 (planning retarget) through 999.2-06
  (phase close). Result: [999.2-SUMMARY.md](phases/999.2-ninja-generator-switch/999.2-SUMMARY.md)
  — Windows Build dropped from 4818s cold → **55s warm**
  (87× speedup, ~98.9% effective ccache hit rate);
  Windows total job ~4.7 min, clearing the 10-min
  standing-disable threshold; macOS/Linux numbers steady
  (already ccache-warm pre-switch). CI verified on
  [25953405453](https://github.com/BALL-Project/ball/actions/runs/25953405453)
  attempts 1 (cold-cache green) and 2 (warm-cache 55s Windows).

### Phase 999.3: Networking rework (BACKLOG · LIKELY SUPERSEDED BY 999.10)

**Goal:** Rework `TCPServer`/`TCPServerThread` onto the modern Boost.Asio acceptor/socket model and cover it with a unit test.
**Why backlog, not v1.6:** Networking is not the milestone's core value, and the Boost.Asio API breakage was already fixed in Phase 1 — the code compiles and links. A proper rework + test is genuine polish but does not gate "build and render on 3 OSes". Deferred to a 1.6.x release. (Moved out of the active roadmap per the Codex adversarial review.)
**Likely superseded:** [Phase 999.10](#phase-99910-replace-socket-remote-control-with-rest-api-backlog--targeted-for-v20) (v2.0) replaces the entire TCP-socket remote-control interface with a REST API and deletes `TCPServer`/`TCPServerThread` outright. If 999.10 promotes to active, this phase becomes moot — close as superseded. The only path where 999.3 survives is if 999.10 is itself deferred past v2.0 and BALL needs the TCP server in the interim.
**Requirements:** NET-01
**Plans:** 0 plans

Plans:
- [ ] TBD (promote with /gsd-review-backlog when ready)

### Phase 999.4: Config shadows compiled color defaults — COMPLETED via Phase 4.1 (2026-05-15)

**Status:** Promoted to active **Phase 4.1: Config Color-Defaults Fix** on 2026-05-14; landed and verified 2026-05-15 (5 plans, 8/8 must-haves verified, CONFIG-01 satisfied at runtime). See the Phase 4.1 detail section above and the Progress table.

### Phase 999.4b: Residue color persistence — apply Phase 4.1 fix pattern to ResidueNameColorProcessor (BACKLOG)

**Goal:** Apply the Phase 4.1 D-01/D-03/D-05/D-06 fix pattern (diff/override
only, legacy-key discard, key-name + defensive sanity, diff at write time)
to the residue color persistence path so editing the compiled
`ResidueNameColorProcessor` default table at
[`source/VIEW/MODELS/standardColorProcessor.C:196`](../../../source/VIEW/MODELS/standardColorProcessor.C:196)
produces visibly updated colors even when a pre-existing `~/.BALLView`
contains the legacy `ResidueNames=`/`ResidueNameColors=` keys.
**Why backlog, not v1.6 Phase 4.1:** same bug class, same fix pattern, but
Phase 4.1 was scoped to element-only per D-04 to keep the change surface small
and the migration risk concentrated. Promote when a contributor has time;
this is a pure persistence-format change, no UX implications.
**Scope:**
  - Mirror Phase 4.1 Plan 01 against the residue-name section of
    [`source/VIEW/DIALOGS/coloringSettingsDialog.C`](../../../source/VIEW/DIALOGS/coloringSettingsDialog.C)
    — replace the unconditional `ResidueNames=`/`ResidueNameColors=` write
    (lines 154-161) with a diff against `ResidueNameColorProcessor()` and a
    new key (e.g. `ResidueNameColorOverrides=`).
  - Mirror Phase 4.1 Plan 02 with a "Reset residue colors to defaults"
    button on the Residue_Name page of `coloringSettingsDialog.ui` (line 107
    of that file).
  - Defensive sanity check on residues is harder than the all-white check
    on elements — residue color collisions are less visually distinctive
    than C/N/O/P/S = white. Promote-time decision: which residue triples
    constitute "implausible" (probably default-name = `#ffffff` for
    GLY/ALA/VAL/LEU/ILE — though several real defaults are already pale).
**Requirements:** TBD (new sub-requirement CONFIG-02 on promotion)
**Reference:** `.planning/phases/04.1-config-color-defaults-fix/04.1-CONTEXT.md` /deferred/

Plans:
- [ ] TBD (promote with /gsd-review-backlog when ready)

### Phase 999.5: Open-PR triage — CLOSED as superseded by Phase 999.14 (2026-05-16)

**Status:** Superseded by [Phase 999.14 (GitHub issue + PR triage and cleanup)](#phase-99914-github-issue--pr-triage-and-cleanup-backlog--v16x-housekeeping) per ROADMAP-AUDIT-V1.6.2.md §B. 999.14 covers both issues AND PRs across 5 triage categories; this entry was PR-only and narrower. The named "5-PR legacy bundle" (#640, #600, #554, #550, #546) referenced in `MILESTONE-CONTEXT.md:270-279` is captured as a subtask inside 999.14.

**Original scope** (preserved for history): Review every open pull request on `BALL-Project/ball` in GitHub — triage each one against the current state of the codebase. The repo was frozen at a 2022 commit; open PRs predate the 1.6 modernization work and may be stale, superseded by the patches/phases already landed, or still valuable.

### Phase 999.6: PIPE-01 — Pipeline Rewrite (OpenGL fixed-function → modern backend) (BACKLOG · DORMANT)

**Goal:** Replace BALL's ~100 fixed-function OpenGL calls in `glRenderer.C` with a modern programmable-pipeline backend (QRhi via Metal/D3D11/Vulkan/OpenGL, or GL-core), eliminating the macOS-OpenGL-dead-end risk and matching Qt's strategic direction. **The transition itself, not its scope** — the scope (backend choice, task list, per-platform acceptance criteria) is **Phase 5 SPIKE-02's deliverable**, not this entry's.
**Why DORMANT, not active:**
  - Phase 5's spike (SPIKE-01) is what decides QRhi vs GL-core. Pre-scoping PIPE-01 before the spike locks in a backend assumption the spike would refute.
  - Phase 5 also closes the ~17 deferred stereo/multi-display sites in `scene.C` — without those, PIPE-01 cannot swap the backend cleanly through `RendererFactory`.
**Why this exists at all (1.6 release rationale):** macOS deprecated OpenGL in 10.14 (Mojave, 2018) — stuck at OpenGL 4.1, Apple Silicon support is not guaranteed long-term. Windows OpenGL drivers receive less vendor attention than D3D11/12 or Vulkan. v1.6 keeps fixed-function via the compat profile because that's enough for a shippable release; v2 is when the underlying pipeline catches up to current platform direction.
**Depends on:**
  - Phase 02.1 — Renderer boundary (DONE 2026-05-14): the `Renderer` / `RenderSurface` / `RendererFactory` seam PIPE-01 swaps behind
  - Phase 5 — Qt 6 Migration: must land first; QRhi backend needs Qt 6.7+ (QRhiWidget) or Qt 6's modern OpenGL bindings
  - Phase 5 SPIKE-01 — throwaway prototype against the boundary
  - Phase 5 SPIKE-02 — decision record + scoped task list (this fills in the dormant entry)
**Reference:** [`.planning/phases/999.6-pipe-01-pipeline-rewrite/PIPE-01-BACKLOG.md`](phases/999.6-pipe-01-pipeline-rewrite/PIPE-01-BACKLOG.md) — full known transition surface (4 documented leaks + 6 additional touch points), QRhi vs GL-core research, 8 open questions for the spike to resolve, research provenance.
**Requirements:** TBD (Phase 5 SPIKE-02 emits them)
**Plans:** 0 plans

Plans:
- [ ] TBD (do NOT promote before Phase 5 + SPIKE-02 complete)

### Phase 999.7: Qt 6 Linux + Windows Bring-Up and CI Fixup — CLOSED (2026-05-16, all scope resolved)

**Status:** All four original scope items are RESOLVED:
- (1) Linux aqtinstall — RESOLVED in commit `54da903` (Phase 5.1 D2/D3/D6) + run [25899905204](https://github.com/BALL-Project/ball/actions/runs/25899905204) Linux green.
- (2) Windows vcpkg baseline + build — RESOLVED via Phase 4 vcpkg + Phase 5.1 Tier-B DLL hygiene + Phase 999.2 Ninja switch (Windows Build 4818s → 55s; run [25953405453](https://github.com/BALL-Project/ball/actions/runs/25953405453) tri-OS green).
- (3) macOS `gl_profile=(compatibility|none)` relaxation — RESOLVED in commit `7b28685` (see [`05-HUMAN-UAT.md:23`](phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-HUMAN-UAT.md)).
- (4) Windows VM bring-up — RESOLVED 2026-05-16 (user-confirmed: BALLView tested on a Windows VM, working perfectly since v1.6). SEED-005-3 driver-capture-as-documentation is no longer blocking; the more useful artifact is the post-PIPE-01 capture (SEED-005-4) which falls naturally out of Phase 999.6's v2.0 work.

**Requirements:** QT6-01-LINUX (CLOSED by Phase 4 + 999.2), QT6-01-WINDOWS (CLOSED by Phase 5.1 + 999.2 + user-verified Windows VM run 2026-05-16).

**Carry-forward:** SEED-005-4 (post-PIPE-01 Windows driver-behaviour recapture) folds into Phase 999.6 v2.0 work — not a separate phase. Documented in 999.6 scope when promoted.

**Reference:** [`.planning/phases/999.7-qt6-linux-windows-bringup/QT6-LINUX-WINDOWS-BACKLOG.md`](phases/999.7-qt6-linux-windows-bringup/QT6-LINUX-WINDOWS-BACKLOG.md) — pre-closure scope doc; preserved for history.

Plans: none (phase closed).

### Phase 999.8: Auto-Update via Sparkle + WinSparkle (BACKLOG · TARGETED FOR v1.7 · sequenced after Phase 8)

**Goal:** BALLView checks for new releases on launch and prompts the user to install — Sparkle on macOS, WinSparkle on Windows. Eliminates the "manually re-download from GitHub Releases" friction for v1.x → v1.x+1 updates.
**Why v1.7, sequenced after Phase 8:** Phase 8 ships installers WITHOUT auto-update — captured as a deliberate Phase 8 deferral (Decision #5, 2026-05-15) because auto-update is non-trivial enough (appcast hosting, signed update manifests, key management, rollback handling) to be its own backlog phase. Phase 8 unblocks this — Sparkle + WinSparkle integrate INTO the installer pipeline. Both phases targeted v1.7 per 2026-05-16 user direction.
**Depends on:** Phase 5 (Qt 6 — Sparkle hooks into QApplication event loop, DONE), Phase 8 (Packaging — signing infrastructure must exist first; v1.7), project decision on appcast hosting (GitHub Pages vs ball-project.org vs CDN bucket)
**Reference:** [`.planning/phases/999.8-auto-update-sparkle-winsparkle/AUTO-UPDATE-BACKLOG.md`](phases/999.8-auto-update-sparkle-winsparkle/AUTO-UPDATE-BACKLOG.md) — full scope (Sparkle vs WinSparkle integration, appcast hosting options, key management, recommended 3-plan structure)
**Requirements:** TBD

Plans:
- [ ] TBD (do NOT promote before Phase 8 closes)

### Phase 999.9: Replace INIFile with YAML config (BACKLOG · TARGETED FOR v2.0)

**Goal:** Remove `BALL::INIFile` (the legacy in-tree INI parser/writer) and replace it project-wide with YAML. All 20 `.ini` files under `data/` get converted to `.yaml`; all ~49 source files that call `INIFile::read()` / `INIFile::write()` / `getValue()` / `setValue()` / section traversal migrate to the new YAML config API; `include/BALL/FORMAT/INIFile.h` + `source/FORMAT/INIFile.C` are deleted at the end of the migration.

**Why:** INI is a 1990s format with no schema, no nested structures, no type semantics — BALL's force-field parameter files (AMBER, MMFF94, CHARMM, GAFF) are deeply hierarchical and currently encoded via ad-hoc INI section conventions that the parser and every caller have to keep in sync by hand. YAML is the modern equivalent (nested maps + lists + typed scalars + comments + anchors for shared definitions), has well-maintained C++ libraries (yaml-cpp), and trivially round-trips through Python / web tooling for downstream BALLAXY / Jupyter / inspection workflows. The migration also removes a custom parser surface (~125 INIFile references across BALL's hot code paths) from the codebase, shrinking the maintenance footprint.

**Milestone target: v2.0.** Pairs naturally with the other v2.0 carry-forward (Phase 999.6 — PIPE-01 modern-pipeline rewrite) — v2.0's theme is "modernize the substrate after v1.6 stabilizes." The renderer transition + the config-format transition are both correctness-sensitive infrastructure changes that benefit from being grouped into a single major-version bump where users expect breaking-but-documented behavior changes (file-format churn, GL-vs-QRhi backend swap).

**Why BACKLOG, not active in v1.6.x or v1.7:** This is not core value (build + render on 3 OSes). INI files work today. The win is modernization + tooling + schema validation. v1.6.x is the modernization release that ships BALL/BALLView buildable; data-format churn during a release-cycle would invalidate the "same scientific output" guarantee. v1.7 is the BALLView UI Refresh (SEED-001) — a UI-layer concern, not a config-format concern. v2.0 is the natural home: substrate modernization (renderer + config) together, post-v1.7-UI.

**Scope (in scope):**
- New `BALL::YAMLConfig` (or similar) reader/writer class — likely thin wrapper over `yaml-cpp` (vcpkg / Homebrew / apt all package it) — that exposes the same caller-facing verbs INIFile does (`read`/`write`/`getValue`/`setValue`/section iteration) so the migration is a one-call-at-a-time edit per call site rather than a per-file rewrite
- Conversion of all 20 `.ini` files under `data/` to canonical YAML, preserving every value bit-exactly:
  - Force fields: `data/Amber/amber{91,91S,94,94gly,96,96-docking}.ini`, `data/CHARMM/param22.ini`, `data/CHARMM/EEF1/param19_eef1.ini`, `data/MMFF94/mmff94.ini`, `data/bond_lengths/bond_lengths_mmff94.ini`, `data/bondtyping/GAFFbondangles.ini`
  - Solvation: `data/solvation/{Claverie-AMBER,Ooi,RDF-AMBER}.ini`, `data/solvents/PCM-water.ini`
  - NMR: `data/NMR/{StandardSpectrum,SHIFTX,SHIFTX2,WilliamsonAsakura}.ini`
  - XRAY: `data/XRAY/spacegroups-details.ini`
- All ~49 source-file callers migrated to the new API (force-field engines in `MOLMEC/`, NMR/ in `NMR/`, VIEW dialog preference loaders, BALLView preferences, BALLAXY / Jupyter / PresentaBall extensions, Python bindings)
- `include/BALL/FORMAT/INIFile.h` + `source/FORMAT/INIFile.C` deleted at the END (after all callers migrated + new YAML config tested green)
- Old `.ini` files in `data/` deleted after corresponding `.yaml` is verified bit-exact

**Out of scope:**
- Schema validation (separate follow-up — YAML enables it but isn't required for parity)
- Changing the SEMANTICS of any force-field parameter file — this is a pure format conversion, scientific output stays bit-exact
- Persisted user config (`~/.BALLView`) — Phase 4.1 owns that; if 4.1 still uses INIFile, the 4.1 fix lands first, THEN this phase migrates that file too
- Python `.sip` binding regeneration for any new YAMLConfig types — that's Phase 6 territory

**Risk considerations (correctness-sensitive):**
- Force-field parameter files (`amber94.ini`, `mmff94.ini`, etc.) drive energy calculations. Bit-exact preservation is mandatory — a sign error, decimal-truncation, or rounding drift will silently change every MD simulation downstream. Mitigation: a per-file regression test that loads both old INI + new YAML through the relevant force-field, computes energies on a canonical molecule, and asserts equality to ≤1e-12.
- INIFile may have undocumented behaviors callers depend on (whitespace tolerance, comment styles, section-name case sensitivity). Audit the 49 callers for behaviors-not-just-values before deciding the YAMLConfig API.
- yaml-cpp's default parser is fairly strict — non-canonical YAML in the converted files will surface late. Pin the parser version and validate every converted file via a separate `yaml-cpp -> roundtrip -> diff` step.

**Dependencies:**
- Phase 4 (Dependency System Overhaul) — DONE; provides the vcpkg/Homebrew/apt path for adding `yaml-cpp` as a required dep
- Phase 3 (C++17) — DONE; yaml-cpp uses `std::optional` and `std::variant`-friendly APIs in newer versions

**Plan-shape sketch (when promoted):**
- Plan 1: pick yaml-cpp version, add as required dep in vcpkg.json + Homebrew + apt + `CMakeLists.txt`. Smoke-test the link green on all 3 OSes.
- Plan 2: build `BALL::YAMLConfig` (the wrapper). API mirrors INIFile verb shape. Unit test on a hand-written canonical YAML.
- Plan 3 (largest): convert all 20 `.ini` files in `data/` to `.yaml`. Bit-exact regression test per file (force-field energy, NMR shift, etc.) before each old `.ini` is deleted.
- Plan 4 (largest): migrate the ~49 callers. Done one file at a time; commit per caller; full BALL test suite green after each.
- Plan 5: delete `INIFile.h` + `INIFile.C` + all old `.ini` files. Update README / BUILD-*.md / any doc references. Final green CI on all 3 OSes.

Estimated effort: ~2 weeks if no force-field parameter regression appears; longer if bit-exact preservation forces a YAML representation tweak per force-field.

**Carry-along clean-up (do NOT skip during the migration):**
- All `*.opt` files referenced via INIFile in `data/options/` (if any — verify with `find data -name "*.opt"`)
- Any project-shipped sample / tutorial that hard-codes `*.ini` paths in expected user input
- BALL's Python tutorials that load `.ini` files via `BALL.INIFile()` — update them to `BALL.YAMLConfig()`

**Requirements:** TBD (emitted when promoted — likely `CONFIG-02: YAML config format` + per-file-format requirements)
**Plans:** 0 plans (5 sketched above)

Plans:
- [ ] TBD (promote with /gsd-review-backlog when v2.0 cycle opens; not before v1.6.x ships stable AND v1.7 UI work is mostly done — the YAML config migration touches force-field parameter loading which downstream-affects every chemistry calculation, so it lands after the modernization + UI churn settles)

### Phase 999.10: Deprecate remote-control from BALL proper; narrow REST API + PyBALL SDK (BACKLOG · TARGETED FOR v2.2 · sequenced after KERNEL v2.0 K0+K1)

**Goal:** **Remove all remote-control infrastructure from BALL the library** and replace it with a narrow REST API hosted **in the BALLView application** (not in libBALL), accompanied by a PyBALL class/SDK that wraps the REST calls for Python users. Remote control is no longer a BALL-library concern; it's a BALLView-application concern with a Python SDK.

**Maintainer directive (2026-05-15):** Remote control should be deprecated in BALL proper — not just modernized — and replaced by a narrow REST interface that comes with a PyBALL SDK class to remote control the viewer where necessary. The library should not ship a TCP server / client; that responsibility belongs to the application that needs remote control (BALLView) and to the Python user-facing layer.

**Why:** The current architecture leaks remote-control plumbing through BALL-the-library:
- [`BALL::SYSTEM::TCPIOStream/TCPServer/TCPServerThread`](include/BALL/SYSTEM/networking.h) lives in BALL proper but is only ever used by BALLView's server (and one CLI utility). libBALL users that just want molecular modelling carry the boost::asio + QThread dependency for code they never call.
- [`BALL::CONCEPT::Client`](include/BALL/CONCEPT/client.h) lives in BALL proper but is the client-side counterpart that only exists to talk to BALLView's server. Same misplacement.
- [`BALL::CONCEPT::ObjectCreator`](include/BALL/CONCEPT/objectCreator.h) is part of the persistence-over-network plumbing that only the Client/Server pair use.
- [`BALL::VIEW::ServerWidget`](include/BALL/VIEW/KERNEL/serverWidget.h) + `BALL::VIEW::BALLViewServer` — the server side, already marked `BALL_DEPRECATED` (line 59). The maintainer flagged the design as unfit for current use.
- The binary `Composite`-stream wire format is unscriptable from anything except BALL itself; no `curl` / Postman / `requests.post()` works. Browser/notebook clients (BALLAXY, Jupyter, PresentaBall) carry sidecar plumbing.

**Architectural shift:**

| Layer | Before (v1.x) | After (v2.0) |
|-------|---------------|--------------|
| libBALL | `TCPServer`/`Client`/`ObjectCreator` + `Persistence` wire format | (no remote-control code) |
| libVIEW | `ServerWidget` + `BALLViewServer` | (no remote-control code) |
| BALLView app | depends on libBALL+libVIEW for server | hosts narrow REST server directly |
| Python (PyBALL) | `client.sip` + `serverWidget.sip` thin wrappers | `pyball.viewer.RemoteViewer` class — SDK over REST |
| Standalone CLI | `BALLVIEWClient` utility (`source/APPLICATIONS/UTILITIES/BALLVIEWClient.C`) | deleted; users call PyBALL or `curl` |
| Wire format | binary `Composite` stream over raw TCP | HTTP + JSON (with `chemical/x-pdb` content-types for structures) |

**Milestone target: v2.2, sequenced after KERNEL v2.0 K0+K1 lands AND PyBALL v2.1 bulk wrap ships** (2026-05-16 user direction — v2.x reshape: v2.0=KERNEL, v2.1=PyBALL wrap, v2.2=this entry, v2.3=RTD docs). Rationale: KERNEL v2.0 (Phase [999.24](#phase-99924-kernel-redesign--moleculestore-soa-parallel-model-migration-backlog--targeted-for-v20--lead-v20-phase)) introduces `MoleculeStore` as the canonical hot-path data structure; PyBALL v2.1 ([999.15](#phase-99915-pyball-wrapping-rewrite-backlog--targeted-for-v21--conditional-on-phase-6-bake-off)) wraps the v2.0 surface. The REST API surface 999.10 exposes targets the post-MoleculeStore facade with PyBALL SDK classes built on top — both upstreams must be stable before REST endpoints are designed.

**v2.x release sequence (per 2026-05-16 user direction "2.0 should start with the kernel"):**
- **v2.0** = [Phase 999.24](#phase-99924-kernel-redesign--moleculestore-soa-parallel-model-migration-backlog--targeted-for-v20--lead-v20-phase) KERNEL redesign (LEAD) + 4 substrate phases (999.6 PIPE-01 + 999.9 YAML + 999.11 gemmi + 999.12 deprecated removal).
- **v2.1** = [Phase 999.15](#phase-99915-pyball-wrapping-rewrite-backlog--targeted-for-v21--conditional-on-phase-6-bake-off) PyBALL bulk wrap targeting v2.0 MoleculeStore facade.
- **v2.2** = this entry (REST API + PyBALL SDK over MoleculeStore).
- **v2.3** = [Phase 999.13](#phase-99913-convert-doxygen-docs-to-read-the-docs--swagger-ui-for-rest-api-backlog--targeted-for-v23--last-v2x-phase) Read the Docs consuming the v2.2 REST OpenAPI spec.

**Supersedes backlog 999.3** (Networking rework — modernize existing `TCPServer` onto modern Boost.Asio acceptor/socket model + unit test). 999.3's goal was to *modernize* the BALL-proper networking code. 999.10's goal is to *remove* it. If 999.10 promotes, 999.3 is moot — close as superseded. The only path 999.3 survives is if 999.10 is itself deferred past v2.2 AND BALL still needs the TCP server for legacy clients in the interim (unlikely — Phase 6 v1.6.x bake-off + 999.15 v2.1 bulk wrap will take the PyBALL SDK along and obviate the need).

**Why BACKLOG, not active in v1.6.x or v1.7:** Not core value (build + render on 3 OSes). The TCP server already compiles (the `boost::asio` API breakage was fixed in Phase 1) and `BALL::VIEW::ServerWidget` is `BALL_DEPRECATED` but not removed, so external clients that still use it continue to work in v1.6.x. v1.7 is the UI refresh, not the wire-protocol/library-API refresh. v2.0/v2.1 establish the data layout + Python surface; v2.2 designs REST endpoints over the stable substrate.

**Scope (in scope):**

*Removal from libBALL + libVIEW (the deprecation half):*
- Delete `include/BALL/SYSTEM/networking.{h}` + `source/SYSTEM/networking.C` (`TCPIOStream`, `TCPServer`, `TCPServerThread`)
- Delete `include/BALL/CONCEPT/client.{h}` + `source/CONCEPT/client.C` (`BALL::Client`)
- Delete `include/BALL/CONCEPT/objectCreator.{h}` + `source/CONCEPT/objectCreator.C` (only used by Client; verify before deleting)
- Delete `include/BALL/VIEW/KERNEL/serverWidget.{h,iC}` + `source/VIEW/KERNEL/serverWidget.C` (`ServerWidget`, `BALLViewServer`)
- Delete `source/APPLICATIONS/UTILITIES/BALLVIEWClient.C` (the standalone CLI client utility — users go through PyBALL or `curl`)
- Delete `source/PYTHON/EXTENSIONS/BALL/client.sip` + `source/PYTHON/EXTENSIONS/VIEW/serverWidget.sip` (replaced by PyBALL SDK)
- Remove the resulting orphaned `#include` lines + dead UI bits ("Server" menu/dialog entries in BALLView)

*Replacement in BALLView app (the narrow REST server):*
- New `source/APPLICATIONS/BALLVIEW/rest_server.{h,C}` — hosted inside BALLView, not libBALL/libVIEW. Owns the HTTP listening socket and routes to BALLView's existing internal commands.
- Library choice (decide during promotion plan):
  - **cpp-httplib** — header-only, MIT, no extra runtime deps; recommended default
  - **Boost.Beast** — already-have-boost; bigger learning curve
- **Narrow endpoint surface** (locked at "what's needed to remote-control the viewer," not "expose BALLView's command catalog"):
  - `POST /molecule` — body: PDB/SDF/SMILES content; loads molecule into the scene. Response: scene-graph ID.
  - `DELETE /molecule/{id}` — remove from scene.
  - `GET /molecules` — list loaded IDs + names.
  - `POST /view` — body: `{camera?, representation?, selection?}`; adjusts view. Narrow JSON shape, not a generic command bus.
  - `GET /screenshot` — returns PNG of current scene.
  - `GET /version` — server version + capabilities (for SDK feature-detection).
- **Loopback-only by default** (`bind: 127.0.0.1:8642`). LAN exposure deferred to a future phase; this iteration is local-scripting only. Single-user model — no auth needed when loopback-only.
- OpenAPI 3.x spec at `doc/REST-API.yaml`. Authored alongside the code, not retro-fitted. **Phase 999.13** (Read the Docs site) consumes this file to render an interactive Swagger UI page on the docs portal.
- **No WebSocket** in v2.0 — push notifications deferred to a follow-up phase. The initial SDK uses polling (`GET /scene/version` returns a monotonic counter); WebSocket lands when there's a real demand.

*Replacement in PyBALL (the SDK half):*
- New `pyball.viewer.RemoteViewer` class — Python SDK that wraps the REST endpoints. Example:
  ```python
  from pyball.viewer import RemoteViewer
  rv = RemoteViewer()  # connects to localhost:8642
  mol_id = rv.load_molecule('1ake.pdb')
  rv.set_view(representation='cartoon', selection='resname ALA')
  rv.screenshot('output.png')
  ```
- The HTTP wire format is an implementation detail; users code against the Python class. SDK signature stable across REST endpoint refactors.
- Depends on Phase 6 (Python Bindings) having picked a generator — PyBALL exists by v2.0 either way.

**Out of scope (defer to later v2.x):**
- LAN-exposed mode (`--listen` flag + `X-BALL-Token`)
- WebSocket / push events
- OAuth / SSO / multi-user
- Remote rendering / scene streaming
- Backwards-compatibility shim for the old TCP protocol (users migrate at v2.0; the old code is `BALL_DEPRECATED` through v1.7)
- Non-HTTP transports (gRPC, MessagePack-RPC)
- Exposing the full BALLView command catalog over REST — narrow surface only

**Risk considerations:**
- **External clients depending on `BALL::Client` / `ServerWidget` break at v2.0.** Mitigation: v1.7 release notes include a deprecation banner pointing at `pyball.viewer.RemoteViewer`. v2.0 release notes carry a migration guide.
- **`BALL::CONCEPT::ObjectCreator` may have other callers** beyond `Client`. Pre-flight audit during Plan 1 — `grep -rln "ObjectCreator" include/ source/`. If non-Client callers exist, keep `ObjectCreator` (move to a different namespace) or replace those callers' usage too.
- **PyBALL SDK gates on Phase 6's binding choice.** If Phase 6 picks pybind11/nanobind (likely), PyBALL doesn't need SIP for the SDK and the SDK can be pure Python wrapping `requests`. If Phase 6 picks SIP 6 (unlikely), the SDK is still pure Python — REST is transport-agnostic.
- **Library choice (cpp-httplib vs Beast) locks in some maintenance shape.** Mitigation: thin wrapper inside `rest_server.{h,C}` so swap is contained. cpp-httplib's header-only model is the safer default for an embedded server in a desktop app.

**Dependencies:**
- Phase 4 (Dependency System Overhaul) — DONE; provides vcpkg/Homebrew/apt path for the HTTP library.
- Phase 5 (Qt 6) — DONE; the BALLView app is on Qt 6 and can host an embedded HTTP server alongside its event loop.
- Phase 6 (Python Bindings) — TARGETED FOR v1.7-track or v2.0; PyBALL must exist before the SDK side of 999.10 ships. If Phase 6 lands in v1.7, 999.10's PyBALL SDK plan slots in naturally for v2.0.
- v1.7 (BALLView UI Refresh) — useful, not blocking; the new UI can include a "REST API status" indicator if desired.

**Plan-shape sketch (when promoted):**
- Plan 1: **HTTP library decision + ObjectCreator audit.** 1-page comparison cpp-httplib vs Beast; pick one. Audit `ObjectCreator` callers — confirm it's only Client-coupled.
- Plan 2: **Add chosen HTTP library** across vcpkg.json + Homebrew + apt + BALLView's CMakeLists.txt. Smoke-test link green on all 3 OSes.
- Plan 3: **REST server skeleton in BALLView.** `source/APPLICATIONS/BALLVIEW/rest_server.{h,C}` + `GET /version` + `GET /molecules` + `POST /molecule` + loopback binding. Unit tests via `curl` in CI.
- Plan 4: **Remaining narrow endpoints** (`DELETE /molecule/{id}`, `POST /view`, `GET /screenshot`). OpenAPI 3.x spec at `doc/REST-API.yaml`.
- Plan 5: **PyBALL `RemoteViewer` SDK class.** Pure-Python wrapper over `requests`. Documented examples. Pip-installable alongside PyBALL.
- Plan 6: **Deletion sweep** — remove `networking.{h,C}`, `client.{h,C}`, `objectCreator.{h,C}` (if Plan 1 audit cleared it), `serverWidget.{h,iC,C}`, `BALLVIEWClient.C`, `client.sip`, `serverWidget.sip`. Strip BALLView "Server" UI elements. Update README + migration guide. Final tri-OS CI green.

Estimated effort: ~3–4 weeks. Plan 6 (the deletion sweep) is the longest because every caller of `BALL::Client` needs to be replaced or removed; the actual REST server + SDK is ~1 week of well-trodden territory.

**Requirements:** TBD (emitted when promoted — likely `REMOTE-01: REST API for BALLView control` + `REMOTE-02: PyBALL RemoteViewer SDK class` + `REMOTE-03: deprecation/removal of TCP-Composite protocol from libBALL + libVIEW`)
**Plans:** 0 plans (6 sketched above)

Plans:
- [ ] TBD (promote with /gsd-review-backlog when v2.0 cycle opens; do NOT promote before v1.6.x ships stable AND Phase 6 picks a Python binding generator AND v1.7 UI work is mostly done. If 999.3 is still in the backlog at v2.0 promotion time, mark 999.3 as superseded.)

### Phase 999.11: Replace in-tree mmCIF parser with gemmi (BACKLOG · TARGETED FOR v2.0)

**Goal:** Replace BALL's in-tree Flex/Bison-based mmCIF parser with [`gemmi`](https://gemmi.readthedocs.io/) — a modern, well-maintained C++ library for structural biology that includes a fast and conformant mmCIF reader/writer. Reimplements [`BALL::CIFFile`](include/BALL/FORMAT/CIFFile.h) on top of gemmi, deletes the Flex/Bison grammar sources, and (optionally) routes other crystallography formats through gemmi where it adds value.

**Why:** BALL's current mmCIF parser is the Flex/Bison pair `source/FORMAT/CIFParserLexer.l` + `source/FORMAT/CIFParserParser.y`, used by `BALL::CIFFile` (and transitively by `BALL::NMRStarFile`). It is a long-standing maintenance liability:
- **Custom grammar that nobody else uses** — every CIF dialect edge case (multi-line text fields, save-frames, escape conventions) has to be handled by hand. The PDB mmCIF format has evolved since the grammar was written, and corner cases surface in real PDB entries.
- **Surfaces of fragility caught by Phase 5.1:** Plan 05.1-05 fixed 20 `sprintf → snprintf` calls in the grammar; Plan 05.1-06 fixed 17 `strncpy` sites in the lexer with `-Wstringop-truncation`; Phase 5.1 carry-forward fixed a `yyerror(char*)` vs MSVC-`const char*` regression. The grammar regenerates inconsistencies between toolchains (`-Wconflicts-sr` ×3 grammar warnings remain unresolved in the lexer — see Phase 5.1 BACKLOG).
- **gemmi is the de-facto modern alternative** — used by CCP4, PDB-tools, Phenix, RDKit-CIF integration, the Coot model viewer, and the PDB itself. MIT-licensed, packaged in vcpkg + Homebrew + apt (Debian: `libgemmi-dev`). Native Python bindings (`pip install gemmi`) and a CLI (`gemmi convert`) — useful for the eventual PyBALL SDK.
- **Substantial code reduction:** 7 files (CIFFile.{h,C} + Lexer.l + Parser.y + Bison/Flex CMake glue) → ~1 file (CIFFile.{h,C} as a thin gemmi adapter). Removes the project's only Flex+Bison dependency for this format (PDB/MOL2/etc. still use Bison; Bison stays in the build for those).
- **mmJSON + mmCIF + binary CIF (BinCIF) parity** — gemmi reads all three with one API; BALL today only reads CIF. Even if BALL doesn't expose all formats publicly, the floor is higher.

**Milestone target: v2.0.** Joins the (post-2026-05-16 reshape) v2.0 bundle:
- **999.24** — KERNEL redesign (LEAD v2.0 phase per user direction "2.0 should start with the kernel"); MoleculeStore foundation that the other v2.0 substrate phases can build on
- **999.6** — PIPE-01 renderer pipeline rewrite (GL → QRhi)
- **999.9** — INIFile → YAML config-format migration
- **999.11** — In-tree mmCIF parser → gemmi adoption
- **999.12** — Remove deprecated code from core library

All five are correctness-sensitive substrate changes. KERNEL leads (K0+K1 = MoleculeStore foundation); the other four can run in parallel or after K0+K1. v2.0 ships when all five complete. *(999.2 Ninja generator was originally in this bundle; promoted out to v1.6.1 on 2026-05-16 — pure CI-side tooling change with zero source impact.)* *(999.10 REST API + 999.13 RTD docs + 999.15 PyBALL wrap were considered for v2.0 in earlier drafts; per 2026-05-16 user direction they shifted to v2.1/v2.2/v2.3 to sequence after KERNEL v2.0 establishes the stable MoleculeStore data layout.)*

**Scope (in scope):**

*Adoption:*
- Add `gemmi` as a required dependency: `vcpkg.json`, Homebrew (`brew install gemmi`), apt (`libgemmi-dev` on Ubuntu 24.04+; built-from-source fallback if older). Min version: TBD at promotion (pick a Q4-2025 or 2026 release).
- Add CMake `find_package(gemmi CONFIG REQUIRED)` — gemmi ships a CMake config package.

*Reimplementation:*
- Rewrite `source/FORMAT/CIFFile.C` to use `gemmi::cif::Document` / `gemmi::cif::Block` for parsing + writing. The `BALL::CIFFile` public API (Datablock / Datacontent / Item / SaveFrame iteration) is preserved so callers (currently only `NMRStarFile.C`) don't change.
- Decision at plan-time: do we preserve `BALL::CIFFile`'s API shape as-is, or switch to a cleaner shape and migrate callers? With only `NMRStarFile.C` as the in-tree caller, the migration cost is small either way.

*Deletion:*
- `source/FORMAT/CIFParserLexer.l` — DELETE (Flex grammar)
- `source/FORMAT/CIFParserParser.y` — DELETE (Bison grammar)
- The generated `CIFParserLexer.C` and `CIFParserParser.C/h` go with them
- BISON/FLEX CMake glue for these files in `source/FORMAT/sources.cmake` (or wherever it lives) — DELETE. Other parsers (PDB, MOL2, etc.) keep using Bison/Flex.

*Regression coverage (mandatory before deletion):*
- Per-format regression test: load a representative set of PDB CIF entries through both the old parser (snapshot at the point of replacement) and gemmi-based BALL::CIFFile, assert identical molecule structures (atom count, residue map, chain layout, header metadata). PDB IDs to include in the test set: a small, a typical, a large, a multi-NMR-model, a save-frame-heavy NMR-Star example.
- Add the same set to BALL's test suite (Phase 9) so it stays a permanent gate.

**Optional extension (decide during planning):**
- gemmi also reads PDB, mmJSON, MTZ (X-ray reflection files). If BALL's existing PDB/MOL2 parsers also show maintenance issues at v2.0 time, consider routing more formats through gemmi. Default: don't expand scope — replace the CIF parser cleanly, leave other formats alone. PDB-via-gemmi can be its own Phase 999.X later.

**Out of scope:**
- mmJSON / BinCIF / MTZ support — gemmi can read them, but exposing them through `BALL::CIFFile` is a separate API decision
- Crystallographic structure-factor manipulation (gemmi has rich MTZ + SF tools) — out of BALL's molecular-modelling scope
- Writing CIF files in a different dialect than the current parser produces — preserve write-path output bit-for-bit unless the regression test forces a change
- Replacing the lexer/parser pattern for PDB, MOL2, HIN, KCF, etc. — those are not gemmi-supported in the same way

**Risk considerations:**
- **gemmi may parse some malformed-but-historically-accepted CIF entries differently** than BALL's hand-rolled parser. The regression test (load + compare structures) is the gate; deviations get documented case-by-case (and the gemmi behavior is almost certainly more correct per the IUCr CIF spec).
- **NMR-Star files** are CIF-derived but with a non-standard save-frame syntax. gemmi handles them ([gemmi NMR-Star docs](https://gemmi.readthedocs.io/en/latest/cif.html#nmr-star)). Verify the BMRB-format test entries parse with gemmi before deleting `NMRStarFile`'s parser path.
- **gemmi version pinning** — pin a specific gemmi version in vcpkg + Homebrew + apt so cross-platform CI sees the same parser. Mitigates "works on Linux fails on Windows" via library-version drift.
- **gemmi is C++17+** — BALL is C++17 since Phase 3; matches.

**Dependencies:**
- Phase 4 (Dependency System Overhaul) — DONE; provides the vcpkg/Homebrew/apt path for adding gemmi
- Phase 3 (C++17) — DONE; gemmi uses C++17

**Plan-shape sketch (when promoted):**
- Plan 1: Pin gemmi version, add to vcpkg.json + Homebrew + apt + CMakeLists.txt. Smoke-test link green on all 3 OSes.
- Plan 2: Author the BALL::CIFFile gemmi adapter behind the existing public API (parallel to the current Flex/Bison implementation, gated by `-DBALL_USE_GEMMI_CIF=ON` for safe A/B testing).
- Plan 3: Regression test corpus — set of PDB IDs + NMR-Star entries, compare old vs new parsing outputs. Add to BALL test suite.
- Plan 4: Flip the default to gemmi (`-DBALL_USE_GEMMI_CIF=ON` becomes the default; old path stays optionally compilable for one release as escape hatch).
- Plan 5: Delete the Flex/Bison sources + generated files + CMake glue. Final tri-OS CI green confirms no orphaned references.
- Plan 6 (optional): NMRStarFile.C cleanup — if gemmi handles NMR-Star natively in `gemmi::cif`, simplify NMRStarFile.C similarly.

Estimated effort: ~2-3 weeks. Plan 3 (regression corpus) is the most time-consuming; Plans 1+2 are mechanical.

**Requirements:** TBD (emitted when promoted — likely `FORMAT-01: gemmi-based mmCIF parser` + `FORMAT-02: deprecation/removal of Flex/Bison CIF grammar` + `FORMAT-03: PDB-entry regression corpus`)
**Plans:** 0 plans (6 sketched above)

Plans:
- [ ] TBD (promote with /gsd-review-backlog when v2.0 cycle opens; not before v1.6.x ships stable AND v1.7 lands. Sequencing within v2.0: can run in parallel with 999.9 (YAML config) and 999.10 (REST API) — independent files.)

### Phase 999.12: Remove deprecated code from core library and beyond (BACKLOG · TARGETED FOR v2.0)

**Goal:** Audit every `BALL_DEPRECATED` / `BALL_VIEW_DEPRECATED` / `[[deprecated]]` marker in libBALL + libVIEW + applications + bindings, and for each: **delete** the code (+ in-tree callers) OR **un-deprecate** it (if no replacement exists and the class is still load-bearing, per the Phase 5.1 Plan-09 precedent). The major-version break is the right time to ship this cleanup; v1.x stays binary-compatible with the deprecation warnings, v2.0 ships the breaking removal.

**Why:** ~30 files across 7 modules carry deprecation markers as of v1.6 (Phase 5.1 census 2026-05-15). The deprecation has been signaling "this API will go away" to clients for years without follow-through; v1.x's binary compat constraint blocked actually removing it. v2.0 IS the follow-through. Leaving deprecated code in indefinitely creates two problems:

- **Maintenance drag.** Every Phase-5.1-style warning sweep also has to handle the deprecated paths (Plan 05.1-09 fixed a C4996 in `GeneticIndividual` that wouldn't exist if the deprecated class were removed). The Phase 5.1 LEARNINGS already records this pattern.
- **API surface confusion.** Users coding against BALL don't know which API is the "right" one to call; the deprecation marker says "don't use this" without saying "use that instead." Removing deprecated code forces the team to either provide a clear replacement OR keep the API as the live one.

**Plan-05.1-09 precedent (load-bearing — don't ignore):** Plan 05.1-09 ran into `BALL_DEPRECATED` on `BALL::GeneticIndividual` and discovered the class is still load-bearing for `GeneticAlgorithm`, `EvolutionaryDocking`, and similar — with NO replacement class in the tree. The plan **un-deprecated** the class (1-token edit + 11-line rationale comment in `geneticIndividual.h:15`) rather than removing it. **Phase 999.12 must apply the same audit to every deprecated class** — if there's no replacement, the deprecation was wrong and the marker should be removed instead of the class.

**Milestone target: v2.0.** Joins the v2.0 substrate-modernization theme; the major-version break is the natural slot for breaking-API removal. Final v2.0 phase by sequencing — runs AFTER the other v2.0 substrate transitions (999.6, 999.9, 999.10, 999.11) because those each remove a slice of the deprecated surface as a side-effect, and 999.12 mops up what's left.

**Slices already handled by other v2.0 phases:**

| Module | Files | Handler phase |
|--------|-------|---------------|
| `BALL::VIEW::ServerWidget` + `BALLViewServer` | `include/BALL/VIEW/KERNEL/serverWidget.h` + `.iC` + `source/.../serverWidget.C` | **999.10** (deletes as part of REST API replacement) |
| `BALL::TCPServer` / `TCPServerThread` / `TCPIOStream` | `include/BALL/SYSTEM/networking.h` + `source/SYSTEM/networking.C` | **999.10** (deletes as part of REST API replacement) |
| `BALL::Client` / `ObjectCreator` | `include/BALL/CONCEPT/client.h` / `objectCreator.h` + source | **999.10** (deletes as part of REST API replacement) |
| `BALL::VIEW::PythonHighlighter` / `PythonValidator` / `RunPythonThread` / `PyWidget` / `PythonSettings` | `include/BALL/VIEW/WIDGETS/pyWidget.h`, `include/BALL/VIEW/DIALOGS/pythonSettings.h` + sources | **Phase 6** (Python bindings decision; SIP-era embedding goes away if pybind11/nanobind picked) |
| `BALL::PYTHON::pyKernel` / `pyInterpreter` / `pyCAPIKernel` | `include/BALL/PYTHON/*.h` + sources | **Phase 6** (same reason) |
| `BALL::VIEW::CUDAVolumeRenderer` | `include/BALL/VIEW/RENDERING/RENDERERS/cudaVolumeRenderer.h` | **999.6 PIPE-01** (renderer pipeline rewrite — likely subsumes or explicitly drops the CUDA path) |

**Scope of 999.12 — what's left after the slices above:**

The remaining deprecated surface is concentrated in three areas:

*1. DOCKING/GENETICDOCK subsystem — **SLICE HANDLED BY [Phase 999.29](#phase-99929-remove-dockinggeneticdock-dead-subsystem-backlog--targeted-for-v162--pull-forward-from-99912-v20-slice) (v1.6.2 pull-forward, 2026-05-16):*** audit completed against [run 25970222837](https://github.com/BALL-Project/ball/actions/runs/25970222837); zero external references / UI / tests / docs — disposition confirmed **delete**. Scope removed from 999.12: ~11 classes (`GeneticAlgorithm`, `EvolutionaryDocking`, `DockProblem`, `GenericGene/DoubleGene/QuaternionGene`, `GenericParameter/DoubleParameter/QuaternionParameter`, `RotateBonds/RotateBond`, `GeneticIndividual`). 18 files / 2722 LOC delete + 2 sources.cmake line removals lands in 999.29; 999.12's remaining v2.0 scope is the VIEW/WIDGETS + VIEW/DIALOGS slices below.

*2. VIEW/WIDGETS legacy UI (non-Python — 4 classes):*
- `BALL::VIEW::MyTextBrowser` + `HelpViewer` ([helpViewer.h:24,62](include/BALL/VIEW/WIDGETS/helpViewer.h#L24)) — likely Qt 5 → Qt 6 unported help-system widgets
- `BALL::VIEW::ComboBoxDelegate` + `HotkeyTable` ([hotkeyTable.h:23,47](include/BALL/VIEW/WIDGETS/hotkeyTable.h#L23)) — legacy hotkey UI

**Disposition:** if v1.7 BALLView UI Refresh provides replacement widgets, delete these. If v1.7 keeps them as-is, un-deprecate (or migrate to Qt 6 properly). Maintainer call after v1.7 lands.

*3. VIEW/DIALOGS / VIEW/RENDERING residual:*
- `BALL::VIEW::ExportGeometryDialog` ([exportGeometryDialog.h](include/BALL/VIEW/DIALOGS/exportGeometryDialog.h)) — geometry export UI; check if any users still call it
- `BALL::VIEW::Preferences` deprecation markers — likely individual deprecated members within an otherwise-live class; targeted member removal, not class deletion
- 2 other RENDERING/RENDERERS deprecations besides cudaVolumeRenderer — audit

**Out of scope:**
- Macro DEFINITIONS in `include/BALL/COMMON/macros.h` (49 `BALL_DEPRECATED` occurrences) — these are the deprecation macro infrastructure itself, not call sites. Keep.
- `BALL_DEPRECATED` macro definitions in `include/BALL/COMMON/global.h` — keep.
- The deprecation marker in `include/BALL/COMMON/exception.h:18` — that's the C4251 push/pop narrow window comment, not a class deprecation. Keep.
- Any deprecation marker that, on audit, turns out to be load-bearing without a replacement: apply Plan-05.1-09 precedent (un-deprecate, document rationale, keep code).

**Risk considerations:**
- **External clients break.** Some downstream users may be calling `BALL::EvolutionaryDocking` or `BALL::HelpViewer` directly. v2.0 release notes carry the breaking-change list; v1.7 release notes can preview it.
- **Audit-vs-delete asymmetry.** "Audit first" is the rule, "delete" is the action. Every deprecated class gets a one-paragraph note in the v2.0 RELEASE-NOTES.md ("removed: X — replaced by Y" / "removed: X — no replacement, was dead code" / "kept: X — was wrongly deprecated").
- **Test suite coverage.** If a deprecated class has tests under `test/`, those tests get deleted with the class. Phase 9 (test suite triage) work landed before v2.0 means we have a clearer signal on which tests cover live code.
- **Plan-05.1-09 false-positive risk.** Some classes may LOOK load-bearing (have callers in the tree) but those callers are themselves deprecated. Audit the dependency closure, not just first-order callers.

**Dependencies:**
- 999.10 (REST API + PyBALL SDK) — DONE before 999.12; takes the TCP-server slice off the deprecation list
- Phase 6 (Python bindings) — DONE before 999.12; takes the SIP-era Python embedding slice off the deprecation list
- 999.6 (PIPE-01 renderer rewrite) — DONE before 999.12; takes the CUDA volume renderer slice off the deprecation list
- v1.7 (BALLView UI Refresh) — DONE before 999.12; informs the VIEW/WIDGETS UI cleanup audit

**Plan-shape sketch (when promoted):**
- Plan 1: **Census + audit.** `grep -rln "BALL_DEPRECATED\|BALL_VIEW_DEPRECATED" include/ source/` at promotion time. Per-class entry: file path + linked classes/callers + disposition recommendation (`delete` / `un-deprecate` / `defer`). Output: an audit table.
- Plan 2: **DOCKING/GENETICDOCK disposition.** Either bulk-delete the directory (if dead) OR un-deprecate the whole subsystem (Plan-05.1-09 pattern at scale).
- Plan 3: **VIEW deprecation cleanup.** Per-class delete or un-deprecate per audit.
- Plan 4: **Per-removed-class release-notes entry.** RELEASE-NOTES.md migration guide for v1.x → v2.0 users.
- Plan 5: **Final sweep.** `grep -rln "BALL_DEPRECATED\|BALL_VIEW_DEPRECATED"` should match only macro DEFINITIONS in macros.h + global.h (no class-level uses remain). Tri-OS CI green confirms no orphaned references.

Estimated effort: ~2-3 weeks. Plan 1 (audit) is the most contentious — each class needs a real disposition call, not a blind delete. Plan 4 (release notes) is the most labour-intensive but it's the doc work that makes the v2.0 break professional.

**Requirements:** TBD (emitted when promoted — likely `LEGACY-01: deprecated-code removal audit` + `LEGACY-02: v2.0 RELEASE-NOTES.md migration guide`)
**Plans:** 0 plans (5 sketched above)

Plans:
- [ ] TBD (promote with /gsd-review-backlog when v2.0 cycle opens; do NOT promote before 999.6, 999.10, Phase 6, and v1.7 have landed their slices. This phase is the v2.0 "mop up" — it inherits the cleaner deprecation list after the other transitions.)

### Phase 999.13: Convert Doxygen docs to Read the Docs (+ Swagger UI for REST API) (BACKLOG · TARGETED FOR v2.3 · LAST v2.x PHASE)

**Goal:** Convert BALL's existing Doxygen-only C++ API documentation (659 header files carrying `/** ... */` comments across 25 modules — STRUCTURE/QSAR/VIEW/KERNEL/FORMAT/etc.) into a hosted [Read the Docs](https://about.readthedocs.com/) site at `ball-project.readthedocs.io`. The Doxygen comments stay in the source (single source-of-truth, no per-class rewrite), but the OUTPUT format flips from "locally-built HTML you only see if you run `cmake --build --target doc`" to "online portal with stable URL + search + cross-references + version selector." The same site also publishes a Swagger UI page rendering Phase 999.10's REST API spec ([`doc/REST-API.yaml`](doc/REST-API.yaml)) — interactive endpoint browser without rebuilding BALL.

**Two distinct deliverables, one phase:**
1. **Convert the Doxygen-generated C++ API reference to a Read the Docs-hosted online reference** — the bulk of the work, since BALL's API surface is large (659 files with Doxygen comments). Mechanism: keep Doxygen as the comment extractor; add Breathe + Exhale as the Doxygen-XML → Sphinx bridge; publish via RTD.
2. **Add a Swagger UI page consuming `doc/REST-API.yaml`** — small mechanical work using `sphinxcontrib-openapi` once the Sphinx site exists.

The Doxygen-to-RTD conversion is the primary goal; the Swagger UI is a natural addition because both live in the same Sphinx project.

**Why:** BALL has no central docs portal today. Users find:
- A 1990s-era LaTeX tutorial in [`doc/TUTORIAL/`](doc/TUTORIAL/) — not online
- Doxygen-generated C++ API HTML — only available if you `cmake --build --target doc` locally
- README on GitHub
- No Python (PyBALL) docs
- No REST API docs (Phase 999.10 authors the OpenAPI spec but doesn't publish it)

For a v2.0 release shipping signed installers (Phase 8) + a Python SDK (PyBALL via Phase 6) + a REST API (Phase 999.10), having no docs portal is a credibility gap. Read the Docs is the de-facto C++/Python scientific-software docs solution (used by NumPy, SciPy, RDKit, OpenBabel, gemmi, scikit-learn, …). It's free for OSS, integrates with GitHub, builds Sphinx docs automatically on every push, hosts at a stable URL.

**Milestone target: v2.3** (per 2026-05-16 v2.x reshape — was v2.0, then v2.1, then v2.3 as the cascade settled). LAST v2.x phase. Sequencing chain: KERNEL v2.0 (999.24) → PyBALL v2.1 wrap (999.15) → REST API v2.2 (999.10) authors `doc/REST-API.yaml` → v2.3 RTD consumes it via Swagger UI. The docs portal needs all three upstream phases shipped before it has meaningful content to render. Once BALL ships installable (Phase 8 v1.7) + scriptable (999.15 v2.1) + remotely controllable (999.10 v2.2), the docs portal makes the v2.x surface discoverable.

**Scope (in scope):**

*Infrastructure:*
- `.readthedocs.yaml` at repo root — RTD build config (Python version, Sphinx target, theme).
- `docs/` directory (new — separate from the existing `doc/` which has Doxygen + LaTeX tutorial). Sphinx project layout:
  - `docs/conf.py` — Sphinx config; theme: `furo` or `sphinx-rtd-theme` (industry standard, well-supported on RTD)
  - `docs/index.rst` — landing page with cards to sub-sections
  - `docs/getting-started/` — install instructions per platform (links to `BUILD-*.md`)
  - `docs/tutorial/` — port relevant pieces from `doc/TUTORIAL/*.tex` to reStructuredText / MyST markdown
  - `docs/api/` — **C++ API reference, the deliverable that subsumes today's local-only Doxygen HTML.** Built via [Breathe](https://breathe.readthedocs.io/) + [Exhale](https://exhale.readthedocs.io/), which consume the existing Doxygen XML output and render it into the Sphinx site. Doxygen comments in the source stay as the source-of-truth (no per-class rewrite — 659 files would be infeasible); only the OUTPUT format moves online. CMake's `doc` target (today's `cmake --build --target doc`) becomes a developer-convenience-only step; the user-facing path is `https://ball-project.readthedocs.io/api/`.
  - `docs/python/` — PyBALL SDK reference, auto-generated from docstrings via `sphinx.ext.autodoc`. Requires Phase 6 to have landed.
  - `docs/rest-api/` — **Swagger UI page consuming `doc/REST-API.yaml`** via [`sphinxcontrib-openapi`](https://github.com/sphinx-contrib/openapi) or the [`sphinx_swagger_ui`](https://github.com/timothycrosley/sphinx-swagger-ui) plugin. Interactive endpoint browser, try-it-out forms (loopback-pointing for local dev), schema visualization.

*Read the Docs project setup:*
- Connect the GitHub repo → Read the Docs (one-time admin step, GUI on RTD's site)
- Configure builds: `pip install -r docs/requirements.txt && sphinx-build -b html docs/ _build/html/`
- Set the public URL: `https://ball-project.readthedocs.io/` (or alternative if naming collides)
- Version mapping: stable (latest tagged release), latest (v1.6-modernization branch), v1.6 / v1.7 / v2.0 (each release as a permalink)

*Swagger UI page (the user's explicit ask):*
- Single `docs/rest-api/index.rst` page that includes the rendered spec
- Use `sphinxcontrib-openapi`'s `.. openapi::` directive (or equivalent) pointing at `../../doc/REST-API.yaml`
- Renders endpoint summaries + request/response schemas + try-it-out forms (where applicable)
- Cross-link from `docs/python/RemoteViewer.rst` (PyBALL SDK page from Phase 999.10) → REST endpoint that backs each method

*Migration of existing content (decide at plan-time):*
- LaTeX tutorial in `doc/TUTORIAL/` — either port to `docs/tutorial/` as MyST/reST OR keep as `.pdf` artifact downloadable from the new site
- Doxygen HTML output — superseded by Breathe-rendered API ref in `docs/api/`; the old `cmake --build --target doc` can stay as a developer convenience but isn't the user-facing path anymore

**Out of scope:**
- Russian / German / other translations of the docs (i18n is a separate concern; ship English first)
- Versioned docs migration of pre-1.6 BALL versions (unhelpful — the API has changed too much)
- Hosting on a custom domain (`docs.ball-project.org` or similar) — readthedocs.io subdomain is fine for v2.0
- Auto-generating REST endpoint examples from the OpenAPI spec — Swagger UI already does this interactively

**Risk considerations:**
- **Breathe / Exhale can be slow** on large Doxygen trees. BALL's API surface is large. Mitigation: cache the Doxygen XML in CI; build incrementally.
- **`sphinxcontrib-openapi` may not render every OpenAPI 3.x feature** BALL's spec uses. Mitigation: choose the plugin AFTER the spec is stable (999.10 done), test rendering on the actual spec.
- **PyBALL docstring coverage** drives the auto-doc quality. Phase 6 + the SDK plan in 999.10 should include "write docstrings" as part of acceptance criteria, not as an afterthought.
- **RTD build limits** — free tier has time/memory caps. If BALL's Doxygen tree pushes past them, fall back to RTD Business (paid) or self-host Sphinx output.

**Dependencies:**
- **Phase 999.10** — DONE before 999.13 (it authors `doc/REST-API.yaml`, the input to the Swagger UI page)
- **Phase 6** (Python bindings) — DONE before 999.13 (PyBALL must exist for the Python-SDK docs section)
- **Phase 8** (Packaging & Distribution) — DONE before 999.13 (provides install instructions for `docs/getting-started/`)
- v1.7 (BALLView UI Refresh) — helpful: any new UI features get user-facing screenshots in the docs

**Plan-shape sketch (when promoted):**
- Plan 1: **Sphinx scaffolding.** `docs/` directory, `conf.py`, `index.rst`, `.readthedocs.yaml`, `docs/requirements.txt`. Local `sphinx-build` green on a placeholder page.
- Plan 2: **Doxygen ↔ Sphinx bridge via Breathe.** `docs/api/` consumes existing Doxygen XML output. Sample a few classes (Atom, Molecule, Composite) to confirm rendering quality.
- Plan 3: **Swagger UI page for the REST API.** `docs/rest-api/index.rst` uses `sphinxcontrib-openapi` (or chosen plugin) to render `doc/REST-API.yaml`. Cross-link with the PyBALL SDK page.
- Plan 4: **PyBALL SDK reference.** `docs/python/` via `sphinx.ext.autodoc` reading PyBALL docstrings. Manual narrative pages for cookbook examples (`RemoteViewer` walkthrough, common scripting patterns).
- Plan 5: **Getting Started + Tutorial migration.** `docs/getting-started/` links to `BUILD-*.md`; `docs/tutorial/` ports relevant LaTeX-tutorial content to MyST. Old `doc/TUTORIAL/*.tex` either deleted or kept as legacy PDF download.
- Plan 6: **Read the Docs activation.** Connect GitHub → RTD; configure versioning; publish v2.0 docs at the public URL. Add the URL to README + the project's GitHub repo description.

Estimated effort: ~2-3 weeks. Plan 2 (Breathe integration) is the trickiest because BALL's API surface is large and Doxygen quality varies; Plan 3 (Swagger UI) is mechanical once the OpenAPI spec is stable.

**Requirements:** TBD (emitted when promoted — likely `DOC-01: Read the Docs site live at v2.0 release` + `DOC-02: Swagger UI for REST API published` + `DOC-03: PyBALL SDK docs auto-generated from docstrings` + `DOC-04: C++ API reference via Breathe`)
**Plans:** 0 plans (6 sketched above)

Plans:
- [ ] TBD (promote with /gsd-review-backlog when 999.10 + Phase 6 + Phase 8 + 999.15 + KERNEL v2.0 (999.24) have all landed; do NOT promote earlier — the docs site needs real content to ship, and that content comes from those upstream phases. Note: per 2026-05-16 v2.x reshape, 999.13 is the **last v2.x phase** (v2.3), shipping after KERNEL v2.0 + PyBALL v2.1 + REST v2.2 all land.)

### Phase 999.14: GitHub issue + PR triage and cleanup (BACKLOG · v1.6.x HOUSEKEEPING)

**Goal:** Triage every open issue and pull request on `BALL-Project/ball` and close everything that is already resolved, obsolete, or stale, leaving a clean backlog that reflects genuinely actionable work. Document the decision per item so the audit trail exists.

**Why now (v1.6.x slot, not v2.0):** The repo was frozen at a 2022 commit for years. The current v1.6 modernization (Phases 1, 2, 02.1, 02.2, 3, 4, 4.1, 5, 5.1) silently fixes a substantial chunk of what's in flight — issues that were filed against the dead build, against pre-Qt6 APIs, against the dropped `ball_contrib` dependency, against pre-C++17 source — those need to be closed *with cross-references to the commits/PRs that fixed them*, not left to rot. Doing this in v1.6.x (not v2.0) means:
- Users hitting old issues on Google land on a closed-with-context page, not a year-old open thread.
- The remaining open issue/PR set after triage is a faithful representation of v2.0's actual scope, not noise.
- Pre-v2.0 stale PRs are easier to close before they conflict with the v2.0 substrate-modernization wave (PIPE-01 renderer rewrite, INIFile→YAML, gemmi mmCIF, REST API rewrite, Doxygen→RTD docs).

**Supersedes:** [Phase 999.5 (Open-PR triage)](#phase-9995-open-pr-triage-backlog) — that phase was PR-only and narrower; this entry covers both issues and PRs and is the work that should actually run. When 999.14 is promoted, mark 999.5 as superseded with a back-reference.

**Scope:**

*Triage categories (each open issue/PR gets exactly one):*
1. **close-as-fixed** — Resolved by a specific landed commit or PR in the v1.6-modernization branch. Requires cross-referencing the fix-commit before closing. Examples: any issue about the build failing on macOS Apple Silicon (fixed by Phase 1), any PR against `QGLWidget` rendering (fixed by Phase 2), anything mentioning `ball_contrib` (made moot by Phase 4).
2. **close-as-obsolete** — Targets an abandoned subsystem or a pre-Qt6 / pre-C++17 code path that no longer exists in current `main` (or `v1.6-modernization`). Examples: changes to deleted dead code, SIP Python bindings (Phase 6 will pick a new generator), pre-Qt 6 `QGLWidget` paths, the dead `ball_contrib` tree.
3. **close-as-stale** — No activity in ≥ 3 years (suggested threshold; tunable on promotion) AND not on the v1.6/v2.0 critical path. Templated close with an "if this is still relevant, reopen with a current-codebase reproducer" note so legitimately-affected users can resurface.
4. **keep** — Still actionable against the current codebase; either v1.6.x scope or v2.0 substrate-modernization scope. Add a label (`v1.6.1`, `v1.7`, `v2.0`, etc.) reflecting target milestone.
5. **needs-investigation** — Cannot confidently categorize without a deeper read (reproducer required, fix-commit search inconclusive, depends on a v2.0 phase decision not yet locked). Park as a follow-up triage round.

*Tasks (when promoted):*
1. **Enumerate.** `gh issue list --state open --limit 500 --json number,title,labels,createdAt,updatedAt,author` + `gh pr list --state open --limit 500 --json number,title,labels,createdAt,updatedAt,author,headRefName`. Snapshot both lists to `.planning/triage-999.14/{issues,prs}-baseline.json` so the audit trail captures the pre-triage state.
2. **Categorize.** Per item: read body + comments, grep the v1.6-modernization branch for related fix commits (`git log --grep`, `git log --diff-filter` on affected files), assign one of the 5 categories. For close-as-fixed: record the exact commit hash that fixed it. For close-as-stale: record last-activity date.
3. **Cross-reference (close-as-fixed only).** Before closing, verify the cited fix commit is actually on the v1.6-modernization branch and that the fix matches the issue's symptom. Inadequate cross-reference is a red flag — re-categorize as needs-investigation.
4. **Bulk-close with templated comments.** Separate template per category — see "Templates" below. Comments cite Phase numbers and commit hashes; obsolete close cites the API/subsystem deletion; stale close invites reopening with a current reproducer. Use `gh issue close --comment` and `gh pr close --comment`.
5. **Document the decisions.** Write `.planning/triage-999.14/decisions.md` with one row per item: number, title, category, fix-commit (if applicable), close-comment-snippet, decided-by (Claude or maintainer), date. This is the audit trail.

*Templates (close comments):*
- **fixed**: `"This was resolved by the v1.6 modernization work. Specifically: {commit-hash} ({short-message}) in Phase {N}. Closing as fixed — please reopen with a reproducer against `v1.6-modernization` if the issue persists."`
- **obsolete**: `"This targets {subsystem}, which was {removed | replaced} during Phase {N}. {subsystem} is no longer part of the build. Closing as obsolete. If similar functionality exists elsewhere in BALL today, please file a new issue against the current code."`
- **stale**: `"Closing as stale (no activity since {date}, more than 3 years). If this is still relevant against current `v1.6-modernization`, please reopen with a fresh reproducer and we'll triage promptly."`

**Out of scope (DO NOT do during this triage):**
- Reviewing/merging individual PRs that fall in the "keep" category — that is per-PR work, handled by whoever owns the affected subsystem.
- Re-categorizing already-closed issues (only open items).
- Closing issues that have active comments in the last 90 days regardless of category (user is engaged; needs human judgment, not automated triage).

**Requirements:** `TRIAGE-01` (open-issue+PR categorization+bulk-close), `TRIAGE-02` (stale-doc audit bundled task), `TRIAGE-03` (5-PR legacy bundle merge) — see `REQUIREMENTS.md` v1.6.2 section.
**Plans:** 0 plans (5 tasks sketched above; would run as a single PLAN.md when promoted).

**Estimated effort:** 1-3 days for the issue+PR triage itself; +0.5 day for the stale-doc audit task below; +2-5 days for the named 5-PR legacy bundle subtask (depends on triage outcomes per the table below). **Total: 3-9 days.**

**Promotion trigger:** v1.6.1 has tagged + shipped. Then promote with `/gsd-review-backlog 999.14`. Do NOT promote concurrently with active Phase work — triage benefits from a stable HEAD so fix-commit references don't shift mid-pass. **Target milestone: v1.6.2** (per ROADMAP-AUDIT-V1.6.2.md §B; supersedes the former Phase 999.5 Open-PR-triage entry).

**Bundled doc-hygiene task: VERIFICATION.md ↔ HUMAN-UAT.md reconciliation (per ROADMAP-AUDIT-V1.6.2.md §E):**

Scan all `.planning/phases/**/*VERIFICATION.md` files for stale findings whose resolution lives in a sibling `*HUMAN-UAT.md`. ROADMAP-AUDIT v1+v2 were both misled by reading `05-VERIFICATION.md` as current-state when its CR-01/02/03 + gl_profile rows were actually resolved in the HUMAN-UAT rolling log. Same pattern likely exists in other phases.

Per-VERIFICATION.md check (~10 files in tree): does a sibling `*HUMAN-UAT.md` exist? If yes, do the resolution dates in HUMAN-UAT post-date VERIFICATION rows marked FAIL/LATENT/UNRESOLVED? If yes, add the same "⚠ STALE FINDINGS — Resolution log in [HUMAN-UAT.md]" banner pattern that landed on `05-VERIFICATION.md` in commit `f176b8b`. Add a `resolution_log:` frontmatter field to each affected file. ~0.5 day total.

**Named subtask: "5-PR legacy bundle" (per [`MILESTONE-CONTEXT.md:270-279`](MILESTONE-CONTEXT.md) Phase 5.2 carve-out, folded into 999.14 per ROADMAP-AUDIT-V1.6.2.md §B):**

Five specific 2015-2017 PRs deserve named tracking inside 999.14's categorization pass — they were explicitly cited by codex synthesis review of v1.6.1 as v1.6.2 deferrals:

| PR | Year | Plan | Likely category | Notes |
|----|------|------|-----------------|-------|
| [#640 FindXDR](https://github.com/BALL-Project/ball/pull/640) | 2017 | A | close-as-obsolete OR rebase-and-merge | Quick check during triage: `grep -rn "XDR\|FindXDR" CMakeLists.txt include/ source/` — if XDR is gone, close as obsolete; if still in use, rebase. |
| [#600 Travis-CI](https://github.com/BALL-Project/ball/pull/600) | 2016 | B | close-as-obsolete (`.travis.yml` is dead post-GH-Actions) + cherry-pick orthogonal changes | Phase 02.2 + 5.1 made GitHub Actions the CI; Travis is irrelevant. |
| [#554 Omega torsion angles](https://github.com/BALL-Project/ball/pull/554) | 2015 | C | rebase-and-merge (C++ slice); SIP portion defers to 999.15 | Touches `residue.C`; bundle with #546 (same file). |
| [#550 Hydroxyproline (HYP) residue](https://github.com/BALL-Project/ball/pull/550) | 2015 | D | rebase-and-merge | Touches `residue.C`; sequence after C or bundle. |
| [#546 Residue with insertion code](https://github.com/BALL-Project/ball/pull/546) | 2015 | C | rebase-and-merge (C++ slice); SIP portion defers to 999.15 | Touches `residue.C`; bundle with #554 (same file). |

**Bundle escape clause:** if triage finds the rebase work exceeds ~3 days actual effort (per Codex review: don't pre-split phases for speculative work), spin out the merge work as Phase 5.2 at that point. Default: keep inside 999.14 as the codex-recommended path.

Plans:
- [x] [999.14-01-PLAN.md](phases/999.14-github-issue-pr-triage/999.14-01-PLAN.md) — TRIAGE-01 + TRIAGE-02 + TRIAGE-03 bundled execution (complete 2026-05-16)

### Phase 999.15: PyBALL wrapping rewrite (BACKLOG · TARGETED FOR v2.1 · CONDITIONAL on Phase 6 bake-off)

> **Implementation-ready plan:** [`PYBALLV2.md`](PYBALLV2.md) supersedes the outline below. The outline below is preserved as the high-level summary; PYBALLV2.md has the 4-round CODEX-reviewed detail, working autowrap POC at [`pyball-poc/poc1-simple/`](pyball-poc/poc1-simple/), and the 26-40 engineer-week planning envelope. Read PYBALLV2.md before promoting this phase.

> **Important restructuring (per PYBALLV2.md):** the autowrap+Cython recommendation is a **candidate pending Phase 6 bake-off**, NOT a pre-committed decision. Phase 6 (in the active phase list, "Python Bindings") becomes the cross-platform bake-off comparing autowrap+Cython vs nanobind on a real seven-case BALL slice. Phase 999.15 only fires if the chosen tool passes hard stop/pivot gates (see PYBALLV2.md §10). If autowrap wins → execute the autowrap plan in PYBALLV2.md. If nanobind wins → execute the variant plan in PYBALLV2.md §11 (29-40 weeks instead of 25.5-32.5).

**Goal:** Replace BALL's current SIP-4.9-based Python bindings with a modern, Python-3-only binding layer that (a) covers a substantially broader API surface than today's hand-curated subset, (b) keeps in sync with the C++ headers (codegen-first if autowrap wins the bake-off; otherwise hand-written from PYBALLV2.md §11 templates), and (c) eliminates the SIP 4.x deprecation liability before the SIP toolchain stops working on new Python releases.

**Why now (v2.1, not v1.6.x or v2.0):**
- The current 237 hand-written `.sip` files under [`source/PYTHON/EXTENSIONS/{BALL,VIEW}/`](source/PYTHON/EXTENSIONS/) are SIP 4.9 (BALL's `CMakeLists.txt` declares `FIND_PACKAGE(SIP 4.9 REQUIRED)`). SIP 4 is **deprecated**; SIP 6 is the current line. The PyQt project moved to SIP 6 years ago, and the top-level `sip` module Python import that SIP 4 used is itself deprecated ([openSUSE PyQt packaging notes](https://en.opensuse.org/openSUSE:Packaging_PyQt_and_SIP)).
- BALL's Python bindings are **currently disabled in CI** ([`CMakePresets.json`](CMakePresets.json) → `"BALL_PYTHON_SUPPORT": "OFF"` in all three `ci-*` presets) precisely because SIP 4.9 + Python 3.10+ + Qt 6 is a fragile build matrix.
- v2.0 substrate-modernization phases (999.6 PIPE-01, 999.9 INIFile→YAML, 999.10 REST API + PyBALL SDK, 999.11 gemmi, 999.12 deprecated removal) are heavy and *don't* depend on the wrapping rewrite — running the bindings work in parallel would conflict. v2.1 is the natural slot once v2.0 has landed and the wrapped API surface is stable.
- The existing **Phase 6 (Python Bindings)** in the active phase list is scoped as "decide the binding generator via a vertical slice (5-10 core classes), then commit." This 999.15 entry **supersedes Phase 6**: the generator decision is made (autowrap + Cython, see "Recommended path" below) and the work moves to v2.1 as a fuller rewrite. When 999.15 is promoted, mark Phase 6 as superseded with a back-reference. *(If a maintainer prefers the original vertical-slice approach, this entry can be downgraded back to a Phase 6 scope; see "Open questions" #1.)*

**Confirmation of "Python 2 era" claim (audited 2026-05-16):** the C-side glue ([`source/PYTHON/pyCAPIKernel.C`](source/PYTHON/pyCAPIKernel.C)) actually uses **Python 3 C API** idioms (`PyBytes_AsString`, `PyUnicode_AsUTF8`, `PyDict_*`), and `CMakeLists.txt` calls `FIND_PACKAGE(Python3 COMPONENTS Interpreter Development)`. So the Python 3 transition is **partially done at the C level**. The legacy piece is the **wrapping generator (SIP 4.9)** + the 237 `.sip` files that were originally authored for SIP 4 and target a pre-modern wrapping idiom. This phase therefore is "lift the *wrapping toolchain* to Python 3 era," not "lift the C API to Python 3" — the latter is mostly already done.

---

#### 1. State of the art (C++ → Python wrapping, 2026)

Six tools considered. Each in one paragraph with how-it-generates / C++ feature support / build integration / ecosystem traction / maintenance status:

- **[SWIG](https://eathealthy365.com/swig-explained-bridging-c-with-python-java-more/)** — 25+ years, multi-language (Python/Java/C#/Ruby/Go), parses C++ headers via `.i` interface files. Can wrap **unmodified** code, which is its killer feature for legacy libraries. Generates large binaries with significant runtime overhead. Template support is partial — heavy template metaprogramming routinely breaks the SWIG parser. CMake integration via `FindSWIG`. Used by gem5, but gem5 [migrated to pybind11 in 2017](https://m5-dev.m5sim.narkive.com/HvO7oauT/ge-change-in-public-gem5-master-python-use-pybind11-instead-of-swig-for-python-wrappers) citing maintenance burden. Still actively maintained but not the modern recommendation for Python-only targets.

- **[Boost.Python](https://www.boost.org/doc/libs/release/libs/python/doc/html/index.html)** — Legacy, manual binding code. Compared to nanobind, [~11× larger binaries](https://nanobind.readthedocs.io/en/latest/benchmark.html). Heavy Boost dependency. Largely superseded by pybind11 (which started life as a Boost.Python rewrite). Not recommended for new projects.

- **[SIP 6](https://python-sip.readthedocs.io/en/stable/introduction.html)** — Current SIP line, by Riverbank Computing (PyQt). Requires `.sip` DSL files (a Python-ish syntax describing C++ classes to wrap). Mature; powers PyQt5/PyQt6. Adding `%MinimumABIVersion` is now required (deprecation warning otherwise; ["The use of the %MinimumABIVersion directive will be required in SIP v7"](https://python-sip.readthedocs.io/en/stable/releases.html)). For BALL: SIP 4 → SIP 6 migration is feasible but **keeps the manual `.sip` file maintenance burden** — the very thing this phase aims to eliminate.

- **[pybind11](https://pybind11.readthedocs.io)** — Header-only, modern C++17, no codegen, just `PYBIND11_MODULE` macros that you write by hand for each class. Battle-tested at scale (CERN, scipy, PyTorch's C++ glue, GTSAM). Mature, stable, large community. **Downsides**: slow compile (~4× slower than nanobind), large binaries (~3-5× bigger than nanobind), runtime overhead (~10× higher than nanobind for class passing). For BALL: would require **hand-writing bindings for every class** — same maintenance shape as SIP, different syntax. Per the [Rubin Observatory's LSST migration notes](https://community.lsst.org/t/using-pybind11-instead-of-swig-to-wrap-c-code/1096), pybind11 is the modern default *for manual wrapping*.

- **[nanobind](https://nanobind.readthedocs.io)** — pybind11's spiritual successor by the same author (Wenzel Jakob), released 2022, actively maintained ([changelog through 2026](https://nanobind.readthedocs.io/en/latest/changelog.html)). C++17 minimum. Targets a smaller C++ subset on purpose. Benchmarks: [~4× faster compile, ~5× smaller binaries, ~10× lower runtime overhead than pybind11](https://nanobind.readthedocs.io/en/latest/benchmark.html); supports Python's Stable ABI (one binary for many Python versions). Virtual functions via trampoline classes (`NB_TRAMPOLINE` macro), multiple inheritance supported but with documented gotchas in deep hierarchies. **Same manual-wrapping shape as pybind11** — still no codegen, you write the bindings yourself.

- **[autowrap (OpenMS)](https://github.com/OpenMS/autowrap) + [Cython](https://cython.org)** — Codegen tool that consumes annotated `.pxd` files (Cython's C++ declaration syntax) and emits Cython `.pyx` wrapper code, which Cython then compiles to a Python extension. Developed for and battle-tested by [pyOpenMS](https://pyopenms.readthedocs.io/en/release_2.5.0/wrap_classes.html) — has wrapped **>4100 C++ method calls** in a production scientific-software setting. **Python 3 only** (requires Python ≥ 3.9), Cython ≥ 3.0 (audited 2026-05-16 via `GET /OpenMS/autowrap` README — release 0.27.0 dated 2026-01-14, active CI, 879 commits). Supports template classes, enums, free functions, static methods, STL container converters, automatic C++ exception → Python exception mapping, correct reference counting. Author's stated approach: "code generator handles 95% of all use cases; the remaining 5% are still wrapped manually." Build integration via `setup.py`/`pip install -e`; can also be invoked from CMake. **This is the codegen+wrapping combination the user requested.**

- **[Cython directly (no autowrap)](https://cython.org)** — manual `.pyx`/`.pxd` files. Without a generator, hand-writing 700+ class wrappers is comparable maintenance burden to the SIP status quo. Only useful as a fallback for the 5% of classes autowrap can't handle.

- **[cppyy](https://cppyy.readthedocs.io)** (mentioned for completeness) — runtime introspection via Cling JIT. Powerful (handles templates, virtual functions, multiple inheritance with near-zero binding code). But requires Cling at runtime (heavy dependency), and ROOT-derived ecosystem skews toward HEP. Not recommended for BALL: the runtime dependency is too heavy for a downstream-friendly scientific library.

##### Recommended path: **autowrap + Cython**, with **nanobind as an optional escape hatch** for hot paths

**Why autowrap + Cython wins for BALL specifically:**

1. **It is the only option in the comparison that is codegen-first.** The user's explicit goal is "kept in sync via codegen rather than manual edits." pybind11, nanobind, SIP (4 or 6) all require hand-written bindings per class — the maintenance burden the user wants to eliminate. SWIG is codegen-ish but generates from `.i` files that you still write per class. Only autowrap consumes existing C++ declarations (via Cython's `.pxd` syntax, which can be partially auto-generated from headers) and emits bindings, with the explicit 95/5 split stated by its author.
2. **It is proven at BALL-comparable scale.** pyOpenMS wrapped >4100 C++ method calls. BALL's surface (the relevant subset; see Impact Analysis §2 below) is in the same order of magnitude. OpenMS and BALL are both C++ scientific-software libraries with templates, processors, factories, and Composite hierarchies; the pyOpenMS precedent is direct evidence the approach scales to BALL.
3. **Python 3 only** is exactly what we want — no Python 2 retention burden. The SIP 4.9 → SIP 6 migration would re-open Python 2/3 compatibility questions in places.
4. **STL converters out of the box.** BALL's API surface is full of `std::vector<Atom*>`, `std::map<String, ...>`, `HashMap<Position, ColorRGBA>` etc. — autowrap converts these to Python lists/dicts/etc. automatically.
5. **Cython → C exception mapping is automatic.** BALL throws `BALL::Exception::*` (see [`include/BALL/COMMON/exception.h`](include/BALL/COMMON/exception.h)). Cython's `except +` clause turns these into Python exceptions without per-method work.
6. **Active maintenance (release 0.27.0 on 2026-01-14).** Not abandonware.

**Why nanobind is the optional escape hatch:** if profiling after the autowrap rollout shows a few BALL hot paths (probably KERNEL iterator-loop trampolines, `Composite::apply` traversals, or QSAR matrix accessors) bottleneck on wrapping overhead, those *specific* classes can be re-wrapped with nanobind without disturbing the autowrap-generated bulk. nanobind's runtime overhead floor is the lowest of any tool surveyed. This is a v2.2 follow-up, not v2.1 in-scope.

**Rejected options + why:**
- SIP 6 → keeps the manual-`.sip`-file burden; doesn't deliver the user's autogen goal. Migration cost ≈ rewrite cost; no payoff.
- pybind11/nanobind alone → manual wrapping per class; not codegen-first. nanobind would be better for the 237 classes but still 237 hand-written wrappers.
- SWIG → multi-language is not a BALL goal (no Java/C# users on the roadmap), and SWIG's template-parsing limitations are exactly the wrong fit for BALL's heavy template usage in QSAR/STRUCTURE/MATHS.
- cppyy → runtime Cling dependency is too heavy for a downstream-friendly conda-forge BALL package.

---

#### 2. Impact analysis on BALL

*(BALL surface numbers per repo grep on `v1.6-modernization` branch as of 2026-05-16: **762 `.C` TUs** under `source/`, **683 `.h` files** under `include/`, **237 `.sip` files** under `source/PYTHON/EXTENSIONS/{BALL,VIEW}/`.)*

**Subset of public API to wrap (target surface):**
- **`include/BALL/KERNEL/*.h`** — molecular hierarchy (`System`, `Molecule`, `Residue`, `Chain`, `Atom`, `Bond`, `PDBAtom`, `PTE_`). MUST wrap; the core of any scientific use.
- **`include/BALL/FORMAT/*.h`** — file I/O (`PDBFile`, `MOL2File`, `HINFile`, `KCFFile`, `NMRStarFile`, `DCDFile`, `INIFile`). MUST wrap; users want to read/write structures from Python.
- **`include/BALL/STRUCTURE/*.h`** — structural algorithms (`HBondProcessor`, `SmartsParser`, `BindingPocketProcessor`, `FragmentDB`, `RotamerLibrary`). HIGH-PRIORITY; the analysis-script user surface.
- **`include/BALL/COMMON/*.h`** — `Exception::*`, `Position`, `Size`, `Index`. Wraps automatically (Cython converters).
- **`include/BALL/DATATYPE/*.h`** — `String`, `HashMap`, `List`, `BitVector`, `Quaternion`, `Vector3`. Lower priority since most map to Python natives via Cython STL converters.
- **`include/BALL/MATHS/*.h`** — `Matrix3x3`, `Vector3`, `Vector4`, geometric primitives. MEDIUM priority.
- **`include/BALL/MOLMEC/*.h`** — molecular mechanics (AMBER, MMFF94, force-field interfaces). HIGH priority for the scripting user.
- **`include/BALL/ENERGY/*.h`**, **`include/BALL/SOLVATION/*.h`**, **`include/BALL/NMR/*.h`** — domain modules. MEDIUM priority; wrap based on existing `.sip` precedent (whatever the SIP wrappers covered, the new wrappers should at minimum match).
- **`include/BALL/QSAR/*.h`**, **`include/BALL/DOCKING/*.h`**, **`include/BALL/SCORING/*.h`** — large modules with heavy template usage. CAUTION: template instantiation depth here is where autowrap will hit the 5% manual-wrap edge cases.
- **`include/BALL/VIEW/*.h`** — DELIBERATELY OUT OF SCOPE for v2.1. Wrapping VIEW (GUI classes, OpenGL renderers, Qt-derived widgets) requires careful Qt-binding integration (PyQt6 / Qt-for-Python) and re-opens the Qt 6 transition story. Defer to v2.2 or later. Keep VIEW unwrapped in v2.1; the BALLView GUI is the canonical "use VIEW from C++" surface and Python scripting users want core BALL, not VIEW.

**Hardest BALL idioms (where the 5% manual-wrap work concentrates):**
- **Composite hierarchy + `apply(UnaryProcessor<T>&)`** — pyOpenMS hit similar issues with template-based visitors. autowrap can wrap the specific instantiations (`UnaryProcessor<Atom>`, `UnaryProcessor<Residue>`, `UnaryProcessor<Bond>`, etc.) but a Python-side override (Python class inheriting from a processor and overriding `operator()`) requires a trampoline equivalent. **Likely the largest manual-wrap pocket**; budget ~1 week.
- **Heavy template instantiation (`HashMap<K,V>`, `List<T>`, `Vector3<T>`, force-field templates)** — autowrap handles instantiated templates but the `.pxd` declarations need each instantiation enumerated. Workable but verbose; budget another week.
- **Smart pointers / ownership** — BALL uses raw pointers in most places, occasional `boost::shared_ptr`. Cython needs explicit ownership annotations (`ptr_owned`, `ptr_unowned`). Per-class decision.
- **Factories returning base-class pointers** (`ModelProcessor`, `Renderer`, `Representation`) — autowrap can wrap, but downcasting from Python requires extra glue. Document the pattern once, replicate.
- **Iterators** — BALL's iterator classes (`AtomIterator`, `ResidueIterator`, etc.) need Python iterator protocol mapping. Cython provides this via `__iter__`/`__next__` patterns; not hard but per-class boilerplate. autowrap handles via annotations.
- **Operator overloading** (`operator+`, `operator*` on `Vector3`) — Cython supports; per-class declaration in `.pxd`.

**Existing PyBALL test / example surface to keep working:**
- The single `.py` script in tree is [`source/PYTHON/EXTENSIONS/BALL.py`](source/PYTHON/EXTENSIONS/BALL.py) — a 2-line wrapper that imports both `BALLCore` and `VIEW` modules. v2.1 must keep this import path stable (`from BALLCore import *` should still work post-rewrite). The `VIEW` import becomes optional (out of scope for v2.1).
- The 237 `.sip` files transitively name the API surface that *was* wrapped historically — they are the **specification of v2.1's minimum coverage target**. Audit task (999.15-01): enumerate every class+method in the existing `.sip` files; the new autowrap output MUST cover at least the same surface plus the broader-API expansion.
- The BALL test tree (`source/*/test/`) is C++-only; no PyBALL tests exist today. v2.1 should add Python-side smoke tests for the wrapped API.

**Downstream user migration path:**
- Existing PyBALL users: there are very few (the SIP build has been broken/disabled for a while). New users encounter `pip install pyball` → autowrap-built wheel; no learning curve since both old SIP API and new Cython API expose the same C++ class hierarchy with the same method names.
- Anaconda / conda-forge: the package recipe needs updating from SIP 4 → autowrap+Cython. Modest packaging change. Coordinate with conda-forge BALL feedstock maintainer.

---

#### 3. What this phase makes obsolete

Each item below was verified against the current repo before claiming. **Confidence levels: VERIFIED (grep-confirmed), LIKELY (reasoned from architecture), DEPENDS (conditional on a follow-up decision).**

- **VERIFIED — `source/PYTHON/EXTENSIONS/BALL/*.sip`** (104 files per `find source/PYTHON/EXTENSIONS/BALL -name '*.sip' | wc -l`) — the entire SIP-format C++→Python wrapping spec for libBALL. Replaced by autowrap-generated wrappers from annotated `.pxd` files.
- **VERIFIED — `source/PYTHON/EXTENSIONS/VIEW/*.sip`** (133 files in the VIEW subset). Most can be deleted because VIEW wrapping is deferred; the few that document the BALL-side of VIEW interactions move to v2.2 when VIEW gets wrapped.
- **VERIFIED — `source/PYTHON/EXTENSIONS/BALL/BALLPyMacros.h` + `pyBALLSipHelper.h/C`** — SIP-specific helper macros and bridge code. Become unused once SIP is gone.
- **VERIFIED — `include/BALL/PYTHON/EXTENSIONS/pyBALLSipHelper.h` + helpers** — same shape, SIP-specific.
- **VERIFIED — `cmake/FindSIP.cmake` + `cmake/FindSIP.py`** — CMake module to locate SIP. Becomes dead code; delete.
- **VERIFIED — `FIND_PACKAGE(SIP 4.9 REQUIRED)` in `CMakeLists.txt`** plus the surrounding `BALL_PYTHON_SUPPORT` block (~10 lines) — replaced with `find_package(Python3 ...)` + `find_package(Cython ...)` + autowrap invocation as a custom CMake target.
- **LIKELY — `source/EXTENSIONS/JUPYTER/CMakeLists.txt`** — the BALL Jupyter integration probably depends on PyBALL via SIP; needs audit at 999.15-01 time to confirm whether (a) it's already broken in tree, (b) it would just work against the new bindings (same Python module names), or (c) requires its own port. **Don't delete blindly.**
- **DEPENDS — `BALLAXY / PresentaBall / Galaxy` integrations** — historical PyBALL consumers. If any external project still depends on the SIP-generated `BALLCore.so` ABI specifically (not just the Python module API), the v2.1 rewrite breaks them. Audit task 999.15-02 must check whether these projects are still alive and, if so, what their integration shape is.
- **DEPENDS — Python 2 build/CI path** — VERIFIED **not present** in current tree (`CMakeLists.txt` only references `Python3`; no `Python2` find). So there is no Python 2 path to delete; the user's premise of "Python 2 build/CI path needs removal" doesn't apply to the current state. This is good news — one less migration.
- **VERIFIED — `BALL_PYTHON_SUPPORT: OFF` in `CMakePresets.json`** — currently disabled in CI for all three platforms. The v2.1 work flips this to `ON` after the bindings are green on a test matrix.

**Items NOT made obsolete (preserved):**
- `source/PYTHON/pyCAPIKernel.{h,C}` + `pyInterpreter.{h,C}` + `pyKernel.{h,C}` + `pyServer.{h,C}` — these are the **embedded Python interpreter** in BALLView (lets users script BALL from inside the running application). Independent of the C++→Python wrapping question; stays. Uses Python 3 C API already (audit 2026-05-16); minor cleanup possible but not required by this phase.
- All C++ source under `source/` and `include/BALL/` — zero changes to BALL itself; this is a wrapping-layer rewrite, not a library rewrite.

---

#### 4. Phase outline (numbered tasks, when promoted to active)

Each task has a one-line goal and explicit, verifiable success criteria. Tasks numbered `999.15-NN`.

**999.15-01: Audit current PyBALL surface + dependents.**
- *Action:* Enumerate every class and method declared in the 237 `.sip` files. Output: `.planning/phases/999.15-pyball-rewrite/01-SURFACE-AUDIT.md` with a table of (class, methods, header, used-by-`.sip-file`, downstream-dependent-known). Also audit `source/EXTENSIONS/JUPYTER/` and reach out to BALLAXY/PresentaBall maintainers about ABI vs API dependence.
- *Success criteria:* (a) Table has ≥1 row per `.sip` file (count = 237 baseline). (b) Audit document lists every external consumer of `BALLCore.so` known to the maintainers. (c) The "v2.1 minimum coverage" set is locked: an explicit subset of the 237 that the new wrappers MUST cover by ABI-compatible API.

**999.15-02: POC — wrap a representative BALL subset with autowrap.**
- *Action:* Pick 5 classes spanning the difficulty range: `PTE_` (trivial enum-like), `Vector3` (operator-heavy template), `Atom` (KERNEL composite member with iterators), `PDBFile` (FORMAT class with file I/O exceptions), `HBondProcessor` (STRUCTURE processor with `apply` semantics). Write `.pxd` files, run autowrap, build the resulting Cython module, write a Python smoke test that exercises each.
- *Success criteria:* (a) `pip install -e .` builds the POC module on macOS-arm64. (b) Smoke test passes: read a PDB file, iterate atoms, compute H-bonds, get vector arithmetic results. (c) Document the 5% manual-wrap edge cases hit in `.planning/phases/999.15-pyball-rewrite/02-POC-FINDINGS.md` (almost certainly: the `apply(UnaryProcessor<Atom>&)` pattern + override-in-Python).

**999.15-03: Establish autowrap CMake integration + tri-OS build matrix.**
- *Action:* Add a `cmake/FindAutowrap.cmake` (or inline `find_program`) + an `add_custom_command` that runs autowrap on the input `.pxd` files at build time. Update the three `ci-*` CMake presets in [`CMakePresets.json`](CMakePresets.json) to flip `BALL_PYTHON_SUPPORT: ON`. Update `.github/workflows/ci.yml` to install autowrap + Cython + Python dev headers on all three runners (Homebrew on macOS, apt on Linux, pip in a Windows venv).
- *Success criteria:* (a) `cmake --build` on all three platforms produces the POC Cython module green. (b) The POC smoke test runs and passes on all three platforms in CI. (c) The Windows path is documented (vcpkg / pip / msys2 interplay; Cython on Windows historically has rough edges).

**999.15-04: Full surface wrap — KERNEL + FORMAT + COMMON + DATATYPE + MATHS.**
- *Action:* Generate `.pxd` files for these five modules (the "must-wrap" core from §2). Use the POC's edge-case findings to handle templates, iterators, operators. Build, test, fix.
- *Success criteria:* (a) All public classes in these five modules are accessible from Python. (b) Smoke tests cover: opening every supported file format, iterating the Composite hierarchy, vector/matrix math, exception propagation. (c) The previously SIP-wrapped KERNEL+FORMAT API is at minimum reachable from Python (audit cross-check against 999.15-01's table).

**999.15-05: STRUCTURE + MOLMEC + ENERGY + SOLVATION + NMR wrap.**
- *Action:* Same shape as 999.15-04, second tier of modules. STRUCTURE has the `apply(processor)` patterns that 999.15-02 derisked; MOLMEC has the heavy force-field templates.
- *Success criteria:* (a) Force-field minimization (AMBER, MMFF94) runs end-to-end from Python on a real input PDB. (b) H-bond detection, secondary structure assignment via Python. (c) NMR shift prediction (NMRStarFile + NMRDescriptors) reachable from Python.

**999.15-06: QSAR + DOCKING + SCORING wrap (heavy templates — schedule last).**
- *Action:* These modules have the deepest template usage. Expect to hit autowrap's 5% manual-wrap edge cases here. Possibly nanobind the worst offenders if compile time blows up.
- *Success criteria:* (a) At least 80% of the previously SIP-wrapped QSAR/DOCKING/SCORING surface is reachable from Python (the 20% manual-only). (b) The 20% gap is documented with rationale per class. (c) Compile time for the full PyBALL extension on a 4-core runner is ≤ 15 min.

**999.15-07: Deprecate + remove SIP layer.**
- *Action:* Delete the 237 `.sip` files, `FindSIP.cmake`, `BALLPyMacros.h`, `pyBALLSipHelper.{h,C}`. Remove `FIND_PACKAGE(SIP 4.9 REQUIRED)` from `CMakeLists.txt`. Update `BALL.py` if the import path needs adjustment.
- *Success criteria:* (a) No `.sip` files remain under `source/PYTHON/EXTENSIONS/`. (b) `grep -r SIP source/PYTHON include/BALL/PYTHON CMakeLists.txt cmake/` returns nothing meaningful (only natural-language doc references). (c) Build is green; Python smoke tests still pass.

**999.15-08: Documentation + downstream migration guide.**
- *Action:* Write `docs/python/migration-from-sip.md` (or `BUILD-pyball.md` if no Sphinx site yet — depends on whether 999.13 has shipped). Document: per-class API mapping (old SIP → new Cython), known behavior differences, how to rebuild downstream packages. Coordinate with conda-forge BALL feedstock for the recipe change.
- *Success criteria:* (a) Migration doc exists in the docs site (or repo if pre-999.13). (b) conda-forge feedstock PR opened (separate repo; trackable). (c) A canonical example script (e.g., "load PDB, find H-bonds, write MOL2") works on the new bindings and is added to `docs/python/cookbook.md`.

**999.15-09 (OPTIONAL): nanobind escape hatch for hot paths.**
- *Action:* Profile the autowrap-generated bindings on representative workloads (load 1000-residue protein, iterate atoms, compute pairwise distances, run force-field single-point energy). Identify the 3-5 hottest wrapping-overhead bottlenecks. Re-wrap those *specific* classes with nanobind.
- *Success criteria:* (a) Profiling report identifies bottlenecks with numbers. (b) Re-wrapped hot paths show ≥3× speedup vs autowrap baseline. (c) Mixed nanobind+autowrap build is green on tri-OS CI.
- *Skip if:* profiling shows no bottlenecks that justify a second binding tool's complexity.

**Estimated effort:** 999.15-01..04 = ~4-6 weeks (core wrap + POC + CMake). 999.15-05..06 = ~3-4 weeks (rest of API). 999.15-07..08 = ~1-2 weeks (cleanup + docs). 999.15-09 optional. **Total realistic budget: 2-3 months** for one engineer; faster with two if 999.15-05 and 999.15-06 parallelize cleanly (they don't share files but they share infrastructure).

---

#### 5. Risks & open questions

1. **Qt / PyQt6 interaction with BALLView Python scripting.** BALLView embeds a Python interpreter (`pyInterpreter.h/C`). When VIEW eventually gets wrapped (v2.2), the Python side of VIEW will need to coexist with PyQt6 in the same process. Autowrap-generated Cython modules and PyQt6's SIP6-generated modules can coexist (they don't share ABI), but signal/slot threading and Qt event-loop integration need design work. **For v2.1 this is deferred** (VIEW out of scope), but flag for v2.2 planning.
2. **Performance regressions vs raw C++.** Most PyBALL users use it for scripting (load file, transform, save), not numerical inner loops, so wrapping overhead is acceptable. But the QSAR module sometimes feeds Python ML pipelines (descriptor matrices → scikit-learn). If autowrap's per-call overhead is high vs SIP 4 on the descriptor-matrix accessors, that's a regression. **Mitigation:** 999.15-09 (nanobind for hot paths) addresses this.
3. **ABI compatibility with conda-forge BALL.** The conda-forge `ball` package historically shipped PyBALL as part of the install. The new autowrap-generated module name should match (`BALLCore`, `VIEW`) so import paths in downstream scripts don't break. **Action:** coordinate with conda-forge feedstock maintainer before 999.15-07 lands.
4. **Cython on Windows.** Cython compilation on Windows historically requires either MSVC or MinGW + the right Python headers in the right place. vcpkg doesn't provide Cython (Cython is pip-installed). The Windows CI matrix in 999.15-03 needs a hardened path. **Mitigation:** the recently-landed Windows Ninja + ccache work (Phase 999.2) helps; ccache-on-MSVC works for both BALL and the generated Cython modules.
5. **Maintenance commitment.** Autowrap had a 2026-01-14 release (active) but only 78 GitHub stars and a small (≤20) contributor base. If autowrap stagnates between v2.1 and v3.0, we'd need either to fork it or to migrate again. **Mitigation:** the `.pxd` files are standard Cython syntax; they'd survive a switch to a different `.pxd`→`.pyx` codegen tool with limited refactoring (or be hand-edited as plain Cython if autowrap dies — same situation as today's manual `.sip` files, except the input format is industry-standard Cython instead of vendor-specific SIP).
6. **OPEN: do we keep the embedded interpreter (`pyInterpreter.{h,C}`)?** Phase 999.10 (REST API + PyBALL SDK) reshapes how Python interacts with BALL: instead of an in-process embedded Python, users would run a separate Python process talking REST to BALLView. If 999.10 lands first and we decide the embedded interpreter is redundant, that's another ~4 files to delete in 999.15-07's scope. **Decision deferred** until 999.10 has shipped.
7. **OPEN: target Python version floor.** Python 3.9 is autowrap's minimum (audited 2026-05-16). Cython 3 is the floor too. The conda-forge `ball` feedstock currently builds for Python 3.10 / 3.11 / 3.12 / 3.13. Should 999.15 commit to a Python 3.10+ floor (matches conda-forge), or 3.9+ (matches autowrap's minimum)? **Recommend 3.10+** to match the wider scientific-Python ecosystem; 3.9 is in security-only mode.
8. **OPEN: Phase 6 (active list) vs Phase 999.15 (this entry).** Phase 6 is on the active phase list as a v1.6 "decide-the-generator vertical slice." This entry supersedes that scope but targets v2.1. **Decision needed before either is promoted:** delete Phase 6 from the active list entirely? Downgrade it to "deferred to 999.15"? Or keep it as a v1.6 micro-version vertical slice (wrap 5-10 classes with autowrap, prove the chain, defer the full rewrite to v2.1)? The vertical-slice option is appealing as a de-risk for 999.15 — it'd cover 999.15-01/02/03 inside v1.6 and leave 04..09 for v2.1.

**Requirements:** TBD (likely a single `PYTHON-01: PyBALL wrapping ported off SIP 4.9 to autowrap+Cython, ≥80% API coverage parity` on promotion).
**Plans:** 0 plans (9 tasks sketched above; would unfold as PLAN.md files numbered 999.15-01-PLAN.md through 999.15-09-PLAN.md when promoted to active).

**Reference:** [pyOpenMS wrapping workflow docs](https://pyopenms.readthedocs.io/en/release_2.5.0/wrap_classes.html) — the direct template for the autowrap approach. [nanobind benchmarks](https://nanobind.readthedocs.io/en/latest/benchmark.html) for the optional escape-hatch comparison numbers. [SIP release notes](https://python-sip.readthedocs.io/en/stable/releases.html) for the SIP 4 → SIP 6 deprecation timeline that makes this phase necessary.

Plans:
- [ ] TBD (promote with /gsd-review-backlog after v2.0 ships; do NOT promote earlier — v2.0's 999.10 REST API decision affects whether `pyInterpreter.{h,C}` survives, and 999.10's ABI changes would invalidate any wrapping work done before it lands.)

### Phase 999.16: Build acceleration — precompiled headers for BALL + VIEW (BACKLOG · TARGETED FOR v1.6.2)

**Goal:** Add CMake-native precompiled headers (`target_precompile_headers`, CMake 3.16+) for the `BALL` and `VIEW` library targets, pre-compiling the heavy include set (Qt, Boost, Eigen, `BALL/CONCEPT/*`, `BALL/COMMON/*`) once per target instead of N times per TU. Cross-platform — affects all three CI runners.

**Why now (v1.6.2, not v2.0):** With Phase 999.2 landed, the cold-cache Windows Build step is the only remaining build-time pain point on `v1.6-modernization`: warm cache is 55s but cold cache (first push of a feature branch, after eviction, after high-fanout header change) is ~80min. PCH typically delivers 20-40% cold-build speedup on template-heavy C++ — BALL's heavy Boost/Eigen/Qt include surface is exactly the workload PCH is designed for. The change is mechanical (handful of lines in `CMakeLists.txt`), per-target opt-in, and trivially reversible — fits v1.6.2's "small wins, no substrate change" envelope. Deferring to v2.0 would mean v2.0's substrate-modernization phases (PIPE-01 renderer rewrite, INIFile→YAML, gemmi mmCIF, REST API rewrite) pay the cold-cache tax on every CI iteration of every PR. Cheap to land now, compounds across every future build.

**Scope:**
1. **Pick the header set per target.** Audit `source/{KERNEL,CONCEPT,COMMON,DATATYPE,MATHS,SYSTEM}/*.C` for the most-included headers via `grep -h '#include' | sort | uniq -c | sort -rn`; the top ~10-20 are the PCH candidates. Same exercise for `source/VIEW/*.C` (Qt + GL headers dominate). Document the chosen set inline in `CMakeLists.txt` so future drift is visible.
2. **Wire `target_precompile_headers(BALL PRIVATE ...)` and `target_precompile_headers(VIEW PRIVATE ...)`** with the chosen header lists. `PRIVATE` (not `PUBLIC`) so downstream consumers aren't forced to share PCH state. Skip BALLView (the exe target) — its TU count is small enough that PCH overhead isn't worth it.
3. **Verify ccache compatibility.** ccache 4.x has known PCH integration quirks (`sloppiness = pch_defines,time_macros,include_file_mtime` is typically required). Add the `sloppiness` setting to the ccache config step in `.github/workflows/ci.yml` for all three platforms. Without this, PCH-enabled builds can silently bust ccache hits.
4. **Measure delta on cold + warm CI.** Trigger one cold-cache run (push that invalidates the ccache key entirely) and one warm-cache run (rerun on same SHA). Report Build-step times per platform pre/post. Acceptance: ≥15% cold-cache Build-step reduction on Windows, ≥10% on Linux/macOS. If smaller — investigate header set; if negative — revert and document why.
5. **Document in `cmake/PCH.md`** which targets have PCH, which headers, and the ccache sloppiness requirement. Future drift (someone adds a heavy header to one of the PCH'd targets) needs a documented home for "did this get audited?"

**Out of scope (DO NOT do during this phase):**
- PCH on test targets (test build is non-blocking + Phase 9 territory).
- PCH on `BALLView` exe (small TU count, not worth complexity).
- Header hygiene pass (forward-decls instead of full `#include`s in public headers) — separate, slow-burn refactor; not coupled to PCH wins.
- Switching the compiler-launcher integration (clang-cl, distcc, sccache cloud backend) — separate phases if pursued.

**Requirements:** `BUILD-ACCEL-01` — see `REQUIREMENTS.md` v1.6.2 section.
**Plans:** 1/1 plans complete

**Estimated effort:** 0.5-1 day. Wiring is ~20 lines of CMake; the time is in picking the right header set + measuring delta cleanly under the concurrency-group churn that bit the Phase 999.2 measurements.

**Promotion trigger:** Anytime in v1.6.2 cycle; no upstream dependencies. Promote with `/gsd-review-backlog 999.16`.

Plans:
- [ ] TBD (promote with /gsd-review-backlog when v1.6.2 milestone opens)

### Phase 999.17: Build acceleration — cache Windows CMake build tree across CI runs (COMPLETE · v1.6.2 · 2026-05-16)

**Goal:** Cache the Windows CMake build tree (`build/ci-windows/CMakeCache.txt`, `build.ninja`, `CMakeFiles/`, `vcpkg_installed/`) across CI runs via `actions/cache`, keyed on the hash of structural files (`CMakeLists.txt`, `cmake/**`, `vcpkg.json`, `CMakePresets.json`). Collapse the now-dominant warm-cache `Configure (Windows)` step (~2m 32s post-Phase 999.2) to seconds when nothing structural changed.

**Outcome (2026-05-16):** BUILD-ACCEL-02 complete. Warm-cache Configure (Windows) reduced from 149s to 63s (−57.7%). Full scope retained (≥50% threshold). Restore-key prefix fallback provides ~49% speedup on structural-change runs (76s). All 3 platforms green. SUMMARY: `.planning/phases/999.17-windows-build-tree-cache/999.17-01-SUMMARY.md`.

**Why now (v1.6.2, not v2.0):** Post-Phase 999.2, the warm-cache Windows job is `4m 49s` total — of which `Configure (Windows)` accounts for `2m 32s` (vcpkg toolchain restore + FIND_PACKAGE chains + CMake regenerate). Build step is now `55s`. So `Configure` is now the largest single contributor to warm Windows job time; cutting it to seconds takes typical warm Windows from ~5min to ~2min total. Linux/macOS `Configure` is already 5-15s; not worth caching those. The change is contained to `.github/workflows/ci.yml` — no source impact, fully reversible.

**Scope:**
1. **Identify the safe-to-cache subset of `build/ci-windows/`.** `CMakeCache.txt`, `CMakeFiles/`, `build.ninja`, `vcpkg_installed/` are the high-value entries. `Win32/` (object files) is OUT — that's ccache's job, not build-tree cache's. Document the include/exclude list in the workflow comment.
2. **Add an `actions/cache` step before `Configure (Windows)`** keyed on `windows-buildtree-${{ hashFiles('CMakeLists.txt', 'cmake/**', 'vcpkg.json', 'CMakePresets.json', 'source/**/CMakeLists.txt') }}` with restore-key `windows-buildtree-`. On hit: skip configure (or run a fast `cmake --build --preset ci-windows -- regenerate.phony` no-op). On miss: full Configure, then save.
3. **Verify Ninja's incremental-build robustness across cache restores.** Ninja's `.ninja_deps` and `.ninja_log` track per-TU dependency timestamps; restoring them from a different runner's filesystem can confuse Ninja if mtimes drift. Mitigation: use `cmake --build --regenerate-no-test` or set `CMAKE_NINJA_OUTPUT_PATH_PREFIX` consistently. Document the chosen approach.
4. **Confirm vcpkg_installed restoration works alongside the vcpkg binary cache.** The two are different — `vcpkg_installed/` is the resolved per-build artifact set; the binary archives cache (already in place at [ci.yml:335](.github/workflows/ci.yml#L335)) is the source of those artifacts. Caching `vcpkg_installed/` is what removes the multi-minute "install vcpkg packages" portion of Configure.
5. **Measure delta on warm CI.** Trigger one warm-cache rerun (same SHA, same structural-file hash). Acceptance: ≥80% reduction in `Configure (Windows)` step time. If smaller — investigate which sub-step survives the cache; if Ninja chooses to re-run all CMake configures despite the restored cache, the chosen safe-subset is wrong.

**Out of scope (DO NOT do during this phase):**
- Caching the Linux/macOS build tree — Configure on those platforms is small (5-15s); cache save/restore overhead would dominate.
- Caching `Win32/` object files — that's ccache's job and ccache is already wired.
- Reorganizing the vcpkg binary cache step (already working).
- Changing the CMake generator (Ninja stays).

**Requirements:** `BUILD-ACCEL-02` — see `REQUIREMENTS.md` v1.6.2 section.
**Plans:** 0 plans (5 tasks sketched above; would run as a single PLAN.md when promoted).

**Estimated effort:** 0.5-1 day. ~20 lines of workflow YAML + one measurement cycle. Most of the time is debugging Ninja's incremental-build behavior across restored caches (task 3); if Ninja silently re-runs all configures regardless of the restored tree, scope drops to "cache `vcpkg_installed/` only" which is a 3× simpler change with most of the win.

**Dependencies:** None. Layers cleanly with 999.16 (PCH) — 999.16 attacks cold-cache Build, 999.17 attacks warm-cache Configure. Land in either order.

**Promotion trigger:** Anytime in v1.6.2 cycle. Promote with `/gsd-review-backlog 999.17`.

Plans:
- [x] 999.17-01 — Add actions/cache@v5 step + measure cold/warm/structural-change CI cycles (COMPLETE, commit 58a0178+77cde3e+00d2c6f)

### Phase 999.18: CI hygiene — path-aware triggers + concurrency groups (BACKLOG · TARGETED FOR v1.6.2)

**Goal:** Stop docs-only / planning-only pushes from cancelling in-progress code-build CI runs. Today, every push to `v1.6-modernization` triggers the full CI matrix and the `cancel-in-progress: true` concurrency policy ([ci.yml:19-21](.github/workflows/ci.yml#L19)) cancels any running build — including 60-80-minute cold-cache Windows builds — even when the new push only touches `.planning/**` or `*.md` files. Measured cancellation rate on the 2026-05-16 development burst: **8 of last 10 runs cancelled (80%)**, ~80 wasted Windows runner-minutes in a single hour. Lost 5 cold-cache Windows measurements during Phase 999.2 development because of this.

**Why now (v1.6.2, not later):** The pain is observed, measured, and ongoing — every multi-session day during active development on `v1.6-modernization` will hit it. Each cancelled cold-cache Windows run wastes ~30-80min of runner compute that produced no signal. The fix is a workflow-config-only change (no source impact, fully reversible), so v1.6.x scope-discipline is satisfied. Deferring to v2.0 means paying the cancellation tax through every v2.0 substrate phase too.

**Scope:**
1. **Split workflow triggers by path.** Two options to evaluate during planning:
   - *Option A — `paths-ignore` on main CI:* Add `paths-ignore: ['.planning/**', '**/*.md', 'docs/**']` to `ci.yml`'s `push:` + `pull_request:` triggers. Docs-only pushes simply don't trigger CI at all. Cleanest if no docs validation is needed.
   - *Option B — separate workflow for docs:* Keep `ci.yml` for code+CI changes; add `docs.yml` for `.planning/**` + `*.md` paths that runs only lint/link-check. Each has its own concurrency group. Better if you want some basic validation on docs.
2. **Pick option, document the decision** in the workflow comment block (the existing comment at [ci.yml:3-6](.github/workflows/ci.yml#L3) is the natural place).
3. **Verify cancellation behavior** by running a controlled experiment: push a docs-only change while a Windows build is running; confirm the Windows build completes uncancelled.
4. **Update `.planning/MILESTONE-CONTEXT.md`** with a note about the trigger policy so future contributors understand why docs pushes don't show CI status.

**Out of scope (DO NOT do during this phase):**
- Per-job concurrency groups (e.g. "cancel macOS but let Windows finish") — that's a future optimization; the path-aware fix removes 80% of the pain cheaply.
- Removing `cancel-in-progress` entirely — code-change pushes legitimately should cancel stale runs to keep the queue moving.
- Auto-rebase-on-main hooks or merge-queue setup — separate scope.

**Requirements:** `BUILD-ACCEL-03` — see `REQUIREMENTS.md` v1.6.2 section.
**Plans:** 0 plans (4 tasks sketched above; would run as a single PLAN.md when promoted).

**Estimated effort:** 0.5 day. ~10 lines of workflow YAML + one controlled experiment.

**Promotion trigger:** Anytime in v1.6.2 cycle; no dependencies. Promote with `/gsd-review-backlog 999.18`.

**Status (2026-05-16): COMPLETE.** Landed in two commits: `d10da9a` (parallel session — paths-ignore for `.planning/**`, `**.md`, `LICENSE*` on push+pull_request, + bonus linux-arm64 blocking flip) and follow-on `+doc/**` top-up (this plan close). Task 2 (controlled experiment) satisfied empirically: docs-only commit `de94e86` did NOT trigger CI in the GH Actions run list — paths-ignore works as designed. See [999.18-01-SUMMARY.md](phases/999.18-path-aware-ci-triggers/999.18-01-SUMMARY.md).

Plans:
- [x] [999.18-01-PLAN.md](phases/999.18-path-aware-ci-triggers/999.18-01-PLAN.md) — paths-ignore + controlled-experiment verification — COMPLETE 2026-05-16

### Phase 999.19: Per-TU build profiling artifact (COMPLETE · 2026-05-16)

**Goal:** Upload Ninja's per-TU build timing log (`build/ci-{platform}/.ninja_log`) as a CI artifact on every run, and generate a top-N-slowest-TUs summary in the Build step output. Today `bbac526` instruments ccache hit/miss counts, but there's no per-TU duration data — so we don't know *which* TUs dominate cold-build time, which makes 999.16's PCH header set selection a guessing game.

**Why now (v1.6.2, not later):** 999.16 (PCH) needs this data to pick the right headers to precompile. Without it, 999.16 either picks headers by intuition (BALL/CONCEPT/*, Boost, Qt — probably right but unverified) or has to run its own one-off profiling pass. Folding the profiling into the standing CI gives 999.16 the data for free *and* surfaces the same data for every future phase that touches build performance (999.12 dead code, 999.21 unity builds if pursued, header hygiene refactors). The instrumentation is also a prerequisite for any future "Windows Build step exceeded 2× recent baseline" regression alert. Self-justifying compounding tool, fits the v1.6.2 "small wins" envelope.

**Scope:**
1. **Upload `.ninja_log` per platform.** Add an `actions/upload-artifact` step after each Build step that uploads `build/ci-${platform}/.ninja_log` keyed on `ninja-log-${platform}-${github.sha}`. Retention: 30 days (matches the existing test-result artifact convention at [ci.yml:300-302](.github/workflows/ci.yml#L300)).
2. **In-step top-N summary.** Add a `awk` or Python one-liner to the Build step that parses `.ninja_log` (4-column TSV: `start-ms`, `end-ms`, `mtime`, `path`) and prints the 20 slowest TUs to the job output. ~10 lines, no extra dependencies.
3. **(Optional, behind a step flag) `clang -ftime-trace` integration.** On Linux clang builds (none today; this is an opt-in for future PRs that want per-TU per-pass breakdown), the `-ftime-trace` flag emits Chrome-trace JSON. Add the build flag + upload step gated on a workflow_dispatch input. Skip if effort >1h — the `.ninja_log` summary alone covers 80% of the value.
4. **Document the artifact in `.planning/codebase/STACK.md`** or a new `.planning/codebase/CI-ARTIFACTS.md` so future contributors know where to look for per-TU timing data when investigating slow builds.

**Out of scope (DO NOT do during this phase):**
- Storing build-time history in a database / dashboard. That's a v2.0+ infrastructure question; the per-run artifact is enough for ad-hoc analysis.
- Comparing runs / regression alerting (separate future phase; 999.19 only provides the data).
- MSVC equivalent of `-ftime-trace` (`/d2cgsummary`) — Windows + MSVC is the slowest platform and would benefit most, but `/d2cgsummary` output is less analysis-friendly than Clang's. Defer to follow-up if Windows-specific drill-down becomes needed.

**Requirements:** `BUILD-ACCEL-04` — see `REQUIREMENTS.md` v1.6.2 section.
**Plans:** 0 plans (3-4 tasks sketched above; would run as a single PLAN.md when promoted).

**Estimated effort:** 0.5 day. Pure CI instrumentation; no source impact.

**Dependencies:** None to land, but blocks the *full* value of 999.16 (PCH header set selection) — 999.19 should land first so 999.16 can use real data instead of intuition.

**Promotion trigger:** Anytime in v1.6.2 cycle; recommended before 999.16. Promote with `/gsd-review-backlog 999.19`.

Plans:
- [x] [999.19-01-PLAN.md](phases/999.19-per-tu-build-profiling/999.19-01-PLAN.md) — ninja-log artifact upload + top-20 TU print per platform — COMPLETE 2026-05-16

### Phase 999.20: Bump action artifact pins to v6/v7 (**COMPLETE** · v1.6.2 · 2026-05-16)

**Goal:** Update GitHub Actions artifact action pins from `actions/upload-artifact@v4` → v6 and `actions/download-artifact@v4` → v7 across all three workflows (`ci.yml`, `release.yml`, and 999.19's new uploads when it lands). Already noted as carried-over deferred work from Phase 5.1 in [MILESTONE-CONTEXT.md:229](.planning/MILESTONE-CONTEXT.md#L229).

**Why now (v1.6.2):** Already tagged for v1.6.2 in the milestone deferred list — this entry promotes it from a deferral-table line item to an actual backlog phase so it gets scheduled and tracked rather than rotting. The v4→v6 / v4→v7 jumps cross breaking-change majors (auto-merging removed, deprecation of older Node runtime), so the bump needs validation, not blind acceptance. Pure CI hygiene; no source impact.

**Scope:**
1. **Audit all `actions/upload-artifact` and `actions/download-artifact` invocations.** Currently `ci.yml:297-302`, `ci.yml:493-505`, and `release.yml` (TBD locations). Capture the current invocation + arguments per call site.
2. **Read v4→v6 and v4→v7 changelogs.** Surface every breaking change in scope: artifact name collision behavior, retention defaults, compression options, immutability defaults.
3. **Bump pins one at a time, verify CI green per bump.** Don't bundle both bumps in one commit — bisect-friendliness matters if a downstream check breaks. Order: `upload@v4 → v6` first (more touch points), then `download@v4 → v7`.
4. **Update [MILESTONE-CONTEXT.md:229](.planning/MILESTONE-CONTEXT.md#L229)** to mark this deferral resolved.

**Out of scope (DO NOT do during this phase):**
- Switching to non-GitHub artifact stores (S3, Azure Blob) — that's a v2.0 infra question.
- Bumping other action pins (`actions/checkout`, `actions/cache`) — those are independent, not part of the carry-over deferred set.
- Adding new artifact uploads (999.19 territory).

**Requirements:** `BUILD-ACCEL-05` — see `REQUIREMENTS.md` v1.6.2 section. **COMPLETE.**
**Plans:** 1 plan executed (999.20-01-PLAN.md; 3 tasks).

**Actual effort:** ~45 min (dominated by CI watch + parallel-session concurrency cancellations).

**Result:** upload-artifact@v6 (10 sites ci.yml + 2 release.yml), download-artifact@v7 (1 ci.yml + 1 release.yml). No artifact-name collisions. CI run 25970222837 green tri-OS+linux-arm64+coverage. Closes BUILD-ACCEL-05 and the 5-phase build-accel cluster.

Plans:
- [x] 999.20-01 — action artifact pin sweep v4→v6/v7 (commit 1d3da02 upload, 3cb03c8 download)

### Phase 999.21: DockResultFile QtXml dead-code cleanup (BACKLOG · TARGETED FOR v1.6.2)

**Goal:** Remove the stubbed Qt 5 SAX overload `DockResultFile::attributesToHashMap(const QXmlAttributes&)` that has been dead code since Phase 5 Qt 6 migration (commit `3691232` stubbed it under `#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)`).

**Why now (v1.6.2):** Surfaced by ROADMAP-AUDIT-V1.6.2.md §A3. Originally deferred from Phase 5.1 as BLOCKER-A2 ([`.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/deferred-items.md:44`](phases/05-qt-6-migration-4b-renderer-backend-decision-spike/deferred-items.md)); the suggested home was "Phase 5.1 build-warnings/latent-bugs cleanup" but it never landed there. All current BALL readers use the modern `QXmlStreamReader` path, so the stubbed overload is purely dead code — a faint cognitive load on future readers + a small surface for future Qt-version-bump confusion. Tiny effort to fix; fits patch release.

**Scope:**
1. Drop the dead `QXmlAttributes` parameter overload from [`include/BALL/FORMAT/dockResultFile.h`](../include/BALL/FORMAT/dockResultFile.h) (signature only; no callers exist).
2. Remove the stubbed implementation body from [`source/FORMAT/dockResultFile.C`](../source/FORMAT/dockResultFile.C).
3. Clean up the `#if QT_VERSION` guard (no longer needed).
4. Clean up the `#include <QtXml/...>` header reference if it becomes orphaned.

**Out of scope:** Any work on the live `QXmlStreamReader` path. Any other QtXml usages elsewhere in BALL (audit during this phase, file a separate stub if found).

**Estimated effort:** 1 hour. Single-file (header + impl); single-commit change; no behaviour delta on any code path BALL actually exercises.

**Requirements:** `DEADCODE-01` — see `REQUIREMENTS.md` v1.6.2 section.

**Status (2026-05-16): COMPLETE.** Done inline by Claude (no executor spawn); 30 lines removed across header + impl in commit `914622d8f`. Phase 5 BLOCKER-A closed. Local macOS BALL build green; tri-OS CI verification on the commit's run. See [999.21-01-SUMMARY.md](phases/999.21-dockresultfile-dead-code/999.21-01-SUMMARY.md).

Plans:
- [x] [999.21-01-SUMMARY.md](phases/999.21-dockresultfile-dead-code/999.21-01-SUMMARY.md) — inline dead-code removal — COMPLETE 2026-05-16

### Phase 999.22: Warning census (CENSUS-ONLY — Tier-C + residual C4910/C4834) (BACKLOG · TARGETED FOR v1.6.2)

**Goal:** Produce a structured census of the deferred Tier-C warning surface (~3700 instances of `-Wdeprecated-copy`, `-Wunqualified-std-cast-call`, `-Wcatch-value` on macOS/Linux + residual 307×C4910 + 9×C4834 on Windows). **Census-only scope per 2026-05-16 Open Q3 resolution — no warning execution in v1.6.2.** The census + deferral split-list IS the deliverable; actual warning fixes land later per category.

**Why census-only (per 2026-05-16 Open Q3 resolution):** earlier audit draft proposed census + execute the mechanical (a)-subset. User direction: do census-only, file 3 deferral backlog stubs, ship v1.6.2 with no actual warning reduction. Cheapest scope; preserves the patch-release shape; defers the real cleanup decisions to the right milestone targets.

**Scope (CENSUS-ONLY):**

1. **Generate the census** at `.planning/phases/999.22-warning-census/CENSUS.md`: full breakdown per warning category × file path × likely-cause classification. Each warning entry tagged with one of:
   - (a) **Mechanical-fix** — e.g., `-Wdeprecated-copy` for copy constructors that should be `= default`; simple `-Wcatch-value` lifts. Defer to **v1.7** OR a small follow-on v1.6.3 cycle.
   - (b) **Defer-to-v2.0** — fixed-function OpenGL deprecation noise that Phase 999.6 PIPE-01 renderer rewrite dissolves. Cleaning these in v1.6.x is wasted work.
   - (c) **Defer-to-v1.7** — needs real per-site code review (correctness/intent questions).
2. **Include residual Windows warnings:** 307×C4910 + 9×C4834 documented in [`05.1-UAT.md:65`](phases/05.1-build-warnings-and-latent-bugs/05.1-UAT.md). Note: `atom.h` documents an intentional warning tradeoff per Phase 5.1 — flag in census but preserve in code.
3. **File 3 deferral backlog stubs:**
   - `999.22a` — execute (a) mechanical-fix subset (target: v1.7 OR a v1.6.3 cycle if one materializes)
   - `999.22b` — (b) subset, blocked-by-999.6 PIPE-01 renderer rewrite (v2.0 dependent)
   - `999.22c` — (c) per-site code-review subset (v1.7 alongside the UI refresh)

**Out of scope (per 2026-05-16 Open Q3 resolution):**
- **Any warning execution** in v1.6.2 — that's what the 3 backlog stubs are for.
- Anything reachable by the planned Phase 999.6 (PIPE-01) renderer rewrite.
- Tier-A / Tier-B / Tier-D warnings — those were Phase 5.1 scope and are closed.
- Bison `-Wconflicts-sr` grammar conflicts — separate Phase 999.23.

**Estimated effort:** **1-2 days** (was 3-5 days when execution was in scope). Single-pass tri-OS build, capture warnings, categorize, write CENSUS.md + 3 backlog stubs. Done.

**Requirements:** `WARN-CENSUS-01` (CENSUS-ONLY) — see `REQUIREMENTS.md` v1.6.2 section.
**Plans:** 1 plan executed (census complete 2026-05-16).

**Promotion trigger:** anytime in v1.6.2 cycle; no upstream dependencies.

Plans:
- [x] [999.22-01-PLAN.md](phases/999.22-warning-census/999.22-01-PLAN.md) - Generate CENSUS.md + file 999.22a/b/c backlog stubs

### Phase 999.22a: Mechanical Tier-C warning fixes (BACKLOG · TARGETED FOR v1.7)

**Goal:** Execute the (a) mechanical-fix subset of the 999.22 Tier-C warning census — `-Wdeprecated-copy` `= default`-ing on MMFF94 parameter structs (create.h, cosineTorsion.h, scattered MOLMEC/FORMAT/NMR/QSAR), Qt 6 API migrations (`QMouseEvent::x()/.y()` → `position()`, deprecated dialog APIs, Eigen SVD API), sprintf → snprintf on KCFFile.C/logStream.C/GAMESSDatFile.C residuals. Per CENSUS.md, **~161 `-Wdeprecated-copy`** + **~65 `-Wdeprecated-declarations`** = **~226 warnings** (~73% of the remaining Tier-C surface).

**Why now (v1.7):** Mechanical formula-driven; CI-bot safe. Bundled with v1.7's UI refresh so the noise floor drops before QSAR/etc warnings get audited per-site (999.22c). Post-parallel-session the surface is small enough (~226 warnings) that execution is a 1-2 day sweep, not the original 3-5 day estimate.

**Scope:** Execute the (a)-tagged rows from `.planning/phases/999.22-warning-census/CENSUS.md` per-file. Re-measure delta vs the census baseline (CI run 25970862407 on commit 0a75edede). **Excludes:** atom.h (`INTENTIONAL — PRESERVE` flag in census; see 05.1-07/08-SUMMARY.md rationale). Excludes (b) and (c) categories. Excludes renderTarget.h/renderWindow.C PixelFormat cluster (categorized as (b) — Phase 999.6 dissolves them).

**Requirements:** carries no new REQ ID; satisfies the v1.7 follow-on for WARN-CENSUS-01.
**Estimated effort:** 1-2 days (per category × per file mechanical pass + tri-OS green CI re-verify). Original 3-5 day estimate was pre-parallel-session; actual surface is ~6× smaller.
**Plans:** 0 (single-task PLAN when promoted; uses CENSUS.md as the work list).
**Promotion trigger:** anytime in v1.7 cycle.

Plans:
- [ ] TBD (promote with /gsd-review-backlog in v1.7 cycle)

### Phase 999.22b: Fixed-function OpenGL deprecation warning cleanup (BACKLOG · v2.0 · BLOCKED-BY-999.6)

**Goal:** Execute the (b) defer-to-v2.0 subset of the 999.22 census — fixed-function OpenGL deprecation warnings (**~125 instances** per CENSUS.md) emitted from `include/BALL/VIEW/RENDERING/renderTarget.h` (PixelFormat, 123 instances across 41 TUs) and `source/VIEW/RENDERING/renderWindow.C` (2 instances). Phase 999.6 PIPE-01 renderer rewrite will delete these call sites.

**Why blocked-by-999.6:** Cleaning these in v1.6.x or v1.7 is wasted work — the `PixelFormat` struct in renderTarget.h and the entire fixed-function rendering path are targeted for deletion/replacement in the 999.6 PIPE-01 rewrite. Either the 999.6 rewrite naturally zeroes them out (best case) or a 1-day cleanup sweep handles the residual after 999.6 lands.

**Scope:** Run as a follow-up to 999.6 PIPE-01 (NOT before). Re-measure (b)-tagged rows from CENSUS.md post-999.6 — most should be gone (renderTarget.h itself may be deleted or rewritten). Any residual: clean per the mechanical formula in 999.22a (add `= default` copy-ctor to `PixelFormat` or its successor class).

**Requirements:** carries no new REQ ID; satisfies the v2.0 follow-on for WARN-CENSUS-01.
**Estimated effort:** 0-1 days post-999.6 (mostly verification that 999.6 did the work; small cleanup of residuals if any).
**Plans:** 0 (single-task PLAN when promoted).
**Promotion trigger:** AFTER Phase 999.6 (PIPE-01) lands. Do NOT promote earlier.

Plans:
- [ ] TBD (promote AFTER Phase 999.6 PIPE-01 lands)

### Phase 999.22c: Per-site code review for non-mechanical Tier-C warnings (BACKLOG · TARGETED FOR v1.7)

**Goal:** Execute the (c) defer-to-v1.7 per-site review subset of the 999.22 census — **~14 `-Wdeprecated-copy`** + **~28 `-Wdeprecated-declarations`** + **~69 `-Wunused-comparison`** = **~111 warnings** where intent or correctness questions block a mechanical fix. Includes:
- `source/VIEW/DIALOGS/dockDialog.C` — `RadiusRuleProcessor`/`ChargeRuleProcessor` copy-assigned in dialog code; copy semantics may be unintentional
- `include/BALL/MOLMEC/COMMON/forceFieldComponent.h` — ForceFieldComponent copy semantics affect all MOLMEC subclasses; non-trivially safe to default
- `include/BALL/VIEW/KERNEL/message.h` — Message base class copy in the COMPOSITE event pipeline; may have load-bearing semantics
- `include/BALL/DOCKING/GENETICDOCK/geneticIndividual.h` + `.C` — `GenericGene` deprecated attribute; callers need per-site migration verification
- `test/EnumeratorIndex_test.C` + `Substring_test.C` + `ConstRandomAccessIterator_test.C` + `Bit_test.C` — `-Wunused-comparison` in test code; could be intentional test idioms

**Why now (v1.7):** Each site needs a real review — copy-semantics intentional or accidental, deprecated class migration path available or blocked, comparison idiom in tests intentional or bug. Bundles cleanly with v1.7's UI refresh because the audit cadence (slow, deliberate) matches.

**Scope:** Per-site review of (c)-tagged rows from CENSUS.md. Per row: keep-as-is + annotate (intentional), fix mechanically (recategorize as 999.22a-residual), or fix per-site with rationale comment.

**Requirements:** carries no new REQ ID; satisfies the v1.7 follow-on for WARN-CENSUS-01.
**Estimated effort:** 1-2 days (slower than 999.22a because it's per-site reasoning, not formula).
**Plans:** 0 (single-task PLAN when promoted).
**Promotion trigger:** v1.7 cycle. Can run in parallel with 999.22a; reviews different files.

Plans:
- [ ] TBD (promote with /gsd-review-backlog in v1.7 cycle)

### Phase 999.26: Suppress residual C4910 on atom.h (BACKLOG · TARGETED FOR v1.6.2)

**Goal:** Silence the 307 residual `warning C4910` instances on [`include/BALL/KERNEL/atom.h:1035`](../include/BALL/KERNEL/atom.h#L1035) with a single-line `#pragma warning(suppress: 4910)` directive. Cosmetic-only fix; zero behavior change. Closes the documented Phase 5.1 trade-off recorded in [05.1-07-SUMMARY.md](phases/05.1-build-warnings-and-latent-bugs/05.1-07-SUMMARY.md) + [05.1-UAT.md test 4](phases/05.1-build-warnings-and-latent-bugs/05.1-UAT.md) (PARTIAL-PASS).

**Why now (v1.6.2, not v2.0):** v1.6.1 acceptance recorded C4910 as the only outright "WARNING UNCLEAN" on the post-Phase-5.1 Windows MSVC build (down from 722 pre-fix, but still 307 from the atom.h `extern template class BALL_EXPORT std::vector<Atom*>;` line — see census in [05.1-UAT.md test 4](phases/05.1-build-warnings-and-latent-bugs/05.1-UAT.md)). The fix is a one-line `#pragma warning(suppress: 4910)`, fits the v1.6.2 "small wins, no substrate change" envelope. Census-only Phase 999.22 keeps the broader Tier-C cleanup deferred; this surgical fix takes one specific warning category to zero without violating 999.22's CENSUS-ONLY policy because it's not a "warning execution" pass — it's a single suppression matching BALL's existing per-warning idiom at [exception.h:18](../include/BALL/COMMON/exception.h#L18).

**Root cause (recorded for future maintainers):**
- `BALL::Atom` is `BALL_EXPORT`. Multiple BALL_EXPORT classes (e.g. `Composite` tree fragments) contain `std::vector<Atom*>` members.
- When libBALL is compiled, the `BALL_EXPORT` on those classes propagates `__declspec(dllexport)` to their inline destructor/operator= which implicit-instantiate `std::vector<Atom*>`. libBALL.dll exports `std::vector<Atom*>::~vector` and `operator=` as strong symbols.
- libVIEW's `cartoonModel.obj` + `editMode.obj` transitively include atom.h. Pointer-element `std::vector<T*>` (under MSVC) shares representation across all `T*` instantiations and `extern template class` doesn't reliably suppress implicit instantiation in client TUs — without a dllimport hint, libVIEW emits its own strong symbols → LNK2005 at link time.
- atom.h restores the pre-Plan-05.1-07 pattern (`extern template class BALL_EXPORT std::vector<Atom*>;`) so client TUs see the BALL_EXPORT → dllimport mapping and don't implicit-instantiate. This works but trips C4910 in libBALL's own TUs (where BALL_EXPORT → dllexport, contradicting `extern`).
- The warning is harmless — MSVC honors the dllexport (libBALL DOES emit the symbols) and the `extern` is treated as "this is also imported by clients elsewhere." Build succeeds; symbols are exported correctly; C4910 is just noise.

**Scope:**

Single-line edit in [`include/BALL/KERNEL/atom.h:1033-1035`](../include/BALL/KERNEL/atom.h#L1033):

```cpp
 #ifdef BALL_COMPILER_MSVC
 #include <vector>
+#  pragma warning(suppress: 4910)
 extern template class BALL_EXPORT std::vector<Atom*>;
 #endif
```

`#pragma warning(suppress: ...)` is MSVC-specific syntax that suppresses for the *next statement only* — narrower than push/pop. Doesn't affect any other compiler (the surrounding `#ifdef BALL_COMPILER_MSVC` already gates the whole block).

**Verification:** Post-fix CI run's Windows MSVC log should show **0 × C4910 on atom.h:1035** (down from 307). Other C4910 sites (if any future code accidentally writes the same contradictory pattern) remain unsuppressed — the narrow `suppress` directive doesn't leak.

**Out of scope:**
- The 9 residual C4834 warnings — those are pre-existing `[[nodiscard]]` discard sites outside Plan 05.1-09's scope. Census-only via Phase 999.22; execution deferred to v1.7-track per the same policy.
- Architectural fix (Option D in the C4910 research: move the extern decl to a client-only header). That eliminates the warning at source rather than suppressing — but requires creating a new header, updating ~5 VIEW TU includes, and re-verifying tri-OS green. Not patch-release shaped. Capture as a possible v2.0 cleanup if anyone wants a fully clean Windows build.
- Other pointer-element-vector extern declarations elsewhere in the tree (none surveyed yet; atom.h is the only documented one). If 999.22's census surfaces more, fold into this phase or its v1.7 sibling.

**Estimated effort:** ~15 minutes (edit + commit + CI verify). Single-plan phase.

**Requirements:** `WARN-MSVC-C4910-01` — suppress C4910 on atom.h:1035 specifically; verify post-fix Windows MSVC log shows 0 × C4910 on this site. To add to `REQUIREMENTS.md` v1.6.2 section when promoted.

**Plans:** 0 (single plan when promoted).

**Promotion trigger:** anytime in v1.6.2 cycle; no upstream dependencies; trivially co-landable with Phase 999.22 census pass (would shift census's "C4910 ×307 on atom.h" entry to "C4910 ×0 — suppressed in 999.26").

Plans:
- [ ] TBD (promote with /gsd-review-backlog when v1.6.2 milestone opens)

### Phase 999.28: v1.6.2 warning-execution omnibus + Apple-GL silence (COMPLETE · 2026-05-16)

**Goal:** Drive every blocking-platform warning count from the post-v1.6.1 baseline (~4 000 / 2 400 / 1 800 / 700 across linux-arm64, linux-x64, macos-arm64, windows-x64) to under ~10 actionable warnings per platform, without touching architectural patterns and without risking regression to the v1.6.1 binary surface. Real latent bug fixes that fell out of the cleanup are part of the deliverable, not a side-effect.

**Why now (v1.6.2, not v1.7):** The Phase 999.22 census-only policy held the line through v1.6.1 to keep scope tight, but post-Qt-6.8-bump (`95a1e9f`) the warning surface noise had grown large enough to bury real signal — the `va_start` UB in `PDBFile::writeRecord_` and the 6 silent `QFile::open()` failure sites are both real latent bugs only caught by reading the warning logs. Executing the immediate-fix shortlist now closes the signal-to-noise gap before further phase work.

**Approach:** Two parallel work streams, both folded in:

**Stream A — file-by-file warning category execution** (parallel agent work; commits `5ef5509`, `d77aab3`, `94fcd73`, `85d991d`, `3519c34`, `cb392cc`, `c947b2d`):
- `-Wcatch-value` → catch polymorphic `BALL::Exception::*` by `const&` (35 sites)
- Latin-1 source bytes → ASCII in TODO comments (fixes MSVC C4828)
- `sprintf` / `vsprintf` → `snprintf` / `vsnprintf` (all platforms; fixes Apple SDK deprecation advisory + general security improvement)
- `-Woverloaded-virtual` sweep on QSAR `simpleBase.h` subclasses (~40 `using SimpleBase::operator=;` decls in `simpleDescriptors.h` siblings; kills the largest single warning category — ~2 800 hits on linux-arm64)
- Init-list reorder + dead-write removal + empty-body-loop clarification
- GCC `-Wmaybe-uninitialized` false-positive suppression on system headers

**Stream B — Codex-CLI-reviewed shortlist execution** (commits `c6ecd48`, `82dba3c`, `24c044f`, `d730518`, `0a75ede`):
- **P0 real UB:** `PDBFile::writeRecord_` `va_start` on default-promotable enum parameter — signature changed from `(PDB::RecordType, ...)` → `(int record_type, ...)` with internal cast-back. Latent since C++17 adoption.
- **P1 broad warning kills:** defaulted copy-assign on `(Const)BidirectionalIterator` + `(Const)RandomAccessIterator` (~740 -Wdeprecated-copy hits).
- **P1 Qt 6 mechanical migrations:** `QMouseEvent::globalPos()` → `globalPosition().toPoint()` (6 sites); `QByteArray::count()` → `size()` (6 sites); `QDomDocument::setContent` 5-arg → `ParseOptions` overload (2 sites); `QMenu::addAction` arg-order swap (4 sites).
- **P1 small cleanups:** `kekulizer.h` `AtomInfo` member init; `glRenderer` ctor mem-init reorder; drop dead `pm.createAlphaMask()` × 2; fold `QTranslator::load()` result; `Hash_test.C` `char* → const char*`.
- **Windows MSVC trio:** C4834 in `simpleDownloader.C:134` (real latent silent-failure bug); cross-platform `-DBOOST_BIND_GLOBAL_PLACEHOLDERS`; `-D_WIN32_WINNT=0x0A00` on Windows.
- **C4834 sibling sweep:** 5 sites the original census missed — `dockResultFile.C:586,594,602`, `downloadPDBFile.C:125`, `downloadElectronDensity.C:167`. All real latent silent-failure bugs.
- **macOS GL silence:** `GL_SILENCE_DEPRECATION` on the `VIEW` target via Apple's documented escape hatch — eliminates ~1 500 deprecation warnings on macOS until v2.0 Metal/MoltenVK migration.
- **C4267 narrowing mop-up:** 3 sites where `size_t` was implicitly narrowing to `BALL::Index` (32-bit on Windows LLP64) — explicit `static_cast<Index>()` at `CIFFile.C:447`, `binaryFingerprintMethods.C:1618`, `conformationSet.C:46`.

**Codex CLI adversarial review:** Before executing the Stream B shortlist, the censused fix list was fed through `codex exec --sandbox read-only` for an independent senior-C++/Qt6 reviewer pass. Codex caught two census errors before they shipped: A1 named the wrong subclass set (real sites were 40 classes in `simpleDescriptors.h`, not 8); B4 was a misread (the cited site returned by reference, not by value — no dangling). Result file: `/tmp/ci-warnings/CODEX-REVIEW.md`.

**Latent bug fixes flushed out by the cleanup process:**

| File | Bug | Severity |
|---|---|---|
| `PDBFileGeneral.C:1602` | `va_start` UB on default-promoted enum parameter | **Real UB; latent since C++17 adoption** |
| `simpleDownloader.C:134` | `QFile::open()` failure produced zero-byte download with no diagnostic | Real latent silent failure |
| `dockResultFile.C:586,594,602` | Same pattern × 3 — truncated docking-file output on open failure | Real latent silent failure |
| `downloadPDBFile.C:125` | Same pattern → cryptic "no atoms found" parser failure | Real latent UX bug |
| `downloadElectronDensity.C:167` | Same pattern → DSN6 parser failure on zero-byte map file | Real latent UX bug |
| `hashGrid.C` neighbour table | ARM gcc unsigned-char default narrowed `-1` literals (build failure on ARM) | Build-blocking on ARM |

**Verifying run:** [25970862407](https://github.com/BALL-Project/ball/actions/runs/25970862407) — all blocking jobs green; coverage Linux green for the second consecutive run (Phase 999.27 stable).

**Out of scope (preserved for v1.7 / v2.0):**
- Phase 999.22 (b) PIPE-01-dependent warning categories — still v2.0.
- Phase 999.22 (c) per-site code-review subset — still v1.7 alongside UI refresh.
- GENETICDOCK self-deprecation chain (`GenericGene`, `GeneticAlgorithm`, etc., ~120 `[[deprecated]]` markers) — architectural decision, not a 1.6.x patch.
- DCDFile.C:1 C4828 reported by run 25968675219 — file is clean ASCII on HEAD; not reproducible locally. Re-check next CI run.

**Requirements (delivered):**
- `WARN-EXEC-OMNIBUS-01` ✓ — all blocking platforms under 10 actionable warnings on tag commit.
- `WARN-EXEC-OMNIBUS-02` ✓ — no behavior regression vs v1.6.1 (Phase 9 gatekeeper green on tag commit).
- `LATENT-PDB-VA-START-01` ✓ — `PDBFile::writeRecord_` no longer UB.
- `LATENT-QFILE-OPEN-01` ✓ — 6 discarded-bool sites now log + bail on failure.
- `MACOS-GL-SILENCE-01` ✓ — `GL_SILENCE_DEPRECATION` defined for VIEW target; Apple-GL warnings = 0 on macOS.

Plans:
- [x] Stream A executed inline across 7 commits — COMPLETE 2026-05-16
- [x] Stream B executed inline across 5 commits — COMPLETE 2026-05-16

### Phase 999.27: Coverage-build Linux compile-clean trio (COMPLETE · 2026-05-16)

**Goal:** Make the non-blocking coverage CI job compile-clean on Linux Qt 6.8 by fixing three concrete defects surfaced when the coverage build (`-DCMAKE_BUILD_TYPE=Debug -fprofile-arcs -ftest-coverage`) stops masking pre-existing source/include hygiene issues. The `coverage (Linux, gcov, non-blocking)` job had been failing at the **Build BALL + tests** step on every push to v1.6-modernization; all three platforms' release builds stayed green. Fixed without changing any release-build behavior.

**Root cause (corrected from original BACKLOG-era PCH theory):** The original entry guessed PCH ordering / `-O3 -DNDEBUG` invalidation. The actual mechanism is simpler: [`cmake/BALLConfiguration.cmake:12-14`](../cmake/BALLConfiguration.cmake) gates `BALL_VIEW_DEBUG=TRUE` only when `CMAKE_BUILD_TYPE STREQUAL "Debug"`. The coverage job passes `-DCMAKE_BUILD_TYPE=Debug`; the regular Linux build job does not. The offending unqualified `endl` lives inside a `#ifdef BALL_VIEW_DEBUG` block in `~NetworkPreferences`, so the regular Linux build never compiles it and never sees the ambiguity (Qt's `qtextstream.h` introducing `Qt::endl` into scope via `QColorDialog → qaction.h → qdebug.h`). PCH ordering is incidental — once the offending TU is actually compiled, ANY include chain pulling in Qt's text-stream header trips it. Fixed at the source site (`std::endl`).

**Why now (v1.6.2, not v2.0):** Coverage failures hide real coverage regressions — every run shows ❌ on the coverage tile, so future genuine gcovr/test-suite regressions become indistinguishable from this carry-forward noise. All three fixes are one-liners or short edits, fit cleanly in the v1.6.2 "small wins, no substrate change" envelope, and don't touch the renderer/threading hotspots. Surfaced by CI run [25963533636](https://github.com/BALL-Project/ball/actions/runs/25963533636) (post-Qt-6.8-bump + ARM Linux baseline, 2026-05-16); reproduced and diagnosed against failing CI run [25968675219](https://github.com/BALL-Project/ball/actions/runs/25968675219) on commit `061a823`.

**Defects fixed:**

1. **`source/VIEW/DIALOGS/networkPreferences.C:44`** — unqualified `endl` in `~NetworkPreferences` `BALL_VIEW_DEBUG` log statement. Qt's `<QtCore/qtextstream.h>` (pulled in via the VIEW PCH chain `QColorDialog → qaction.h → qdebug.h`) introduces `Qt::endl` into scope, leaving the unqualified token undeclared once the ifdef'd block is compiled (i.e. when `BALL_VIEW_DEBUG=TRUE`, which only happens under Debug build). Sibling call at line 76 already used `std::endl`. **Fix:** qualify as `std::endl`. Commit `78ecda5`. **THE actual build-blocking error** — the only `error:` in the failing log; the other two are warnings.

2. **`source/VIEW/DIALOGS/modifyRepresentationDialog.C:106-107`** — Qt 6 `-Wdeprecated-declarations` on `QMessageBox::critical(QWidget*, QString, QString, int, int, int)` (six-int legacy overload). **Fix:** switch to the `StandardButtons` overload: `QMessageBox::critical(this, tr("BALLView"), msg, QMessageBox::Ok)` — drops the trailing `Qt::NoButton` default-button slot (no `StandardButtons` analog; `Ok` is single-button default). Warning, not error; folded in opportunistically. Commit `2497369`. (Note: `dockDialog.C`, `dockResultDialog.C` carry the same deprecated **constructor** overload — different signature, separately tracked in Phase 999.22 census.)

3. **`include/BALL/MOLMEC/MMFF94/MMFF94Parameters.h:52`** — `-Wdeprecated-copy` on `MMFF94ESParameters` / `MMFF94VDWParameters` / `MMFF94PlaneParameters` copy ctors because base class `MMFF94ParametersBase` has user-provided `operator=` but no user-provided copy ctor (rule-of-three violation). **Fix:** add `MMFF94ParametersBase(const MMFF94ParametersBase&) = default;` to the base — pairs at the inheritance root so all derived classes inherit the explicit pairing. Warning, not error; folded in opportunistically. Commit `b98b63a`.

**Verifying run:** [25968988067](https://github.com/BALL-Project/ball/actions/runs/25968988067) on commit `c0d439c` — all 8 jobs green, including `coverage (Linux, gcov, non-blocking)` for the first time in the v1.6-modernization branch's recorded history (✓ Build BALL + tests, ✓ Run test suite under coverage, ✓ Generate coverage report).

**Out of scope (preserved for v2.0 / Phase 999.22):**
- Whole-tree Tier-C warning cleanup — remains Phase 999.22 (census-only).
- Full rule-of-five conversion of `MMFF94ParametersBase` — v2.0 substrate work, not patch-release shaped.
- PCH ordering changes — defect 1 was fixed at the source site, not by reshuffling PCH includes.
- The other `QMessageBox` constructor-form deprecations in `dockDialog.C` / `dockResultDialog.C` (different overload, different surface).

**Requirements (delivered):**
- `COV-CLEAN-01` ✓ — coverage Linux build compiles end-to-end (Build BALL + tests ✓ on `c0d439c`).
- `WARN-VIEW-ENDL-01` ✓ — `networkPreferences.C` uses `std::endl`; no unqualified `endl` in file.
- `WARN-VIEW-QMSGBOX-01` ✓ — `modifyRepresentationDialog.C` uses `StandardButtons` overload.
- `WARN-MMFF94-COPY-01` ✓ — `MMFF94ParametersBase` declares explicit `= default` copy ctor; `-Wdeprecated-copy` no longer fires on `MMFF94ESParameters` / siblings.

Plans:
- [x] Fix-and-land trio executed inline (3 commits: `78ecda5`, `2497369`, `b98b63a`) — COMPLETE 2026-05-16

### Phase 999.29: Remove DOCKING/GENETICDOCK dead subsystem (COMPLETE · v1.7 · 2026-05-17)

**Goal:** Delete the entire `include/BALL/DOCKING/GENETICDOCK/` + `source/DOCKING/GENETICDOCK/` directories and the two `INCLUDE(...)` lines that wire them into the build. 18 files, 2722 LOC removed. Eliminates ~150 deprecation warnings from every platform build (`-Wdeprecated-declarations` on `GeneticAlgorithm`, `EvolutionaryDocking`, `DockProblem`, `GenericGene/DoubleGene/QuaternionGene`, `GenericParameter/DoubleParameter/QuaternionParameter`, `RotateBonds/RotateBond`, `GeneticIndividual`).

**Why now (v1.6.2, not v2.0):** Phase 999.12 (BACKLOG · v2.0) catalogued the GENETICDOCK slice with the explicit decision branch "**delete** the entire directory OR **un-deprecate**" pending audit. Audit completed against [run 25970222837](https://github.com/BALL-Project/ball/actions/runs/25970222837) (commit `cb392cc`, 2026-05-16): **zero external references** across `include/`, `source/`, `APPLICATIONS/`, `test/`, `source/PYTHON/`; **zero UI exposure** — `DockingController` only registers `Geometric Fit`, no `GENETIC_*` enum, no dialog/UI string references; **zero test coverage**; **zero documentation references** in `doc/` or any `*.md`. The "user — is this load-bearing?" question has a definitive answer: it isn't. Pulling forward to v1.6.2 takes ~150 warnings to zero today instead of carrying them through every CI run until v2.0, and shrinks Phase 999.12's scope by its single largest slice. Mechanical, isolated, reversible — fits the v1.6.2 "small wins" envelope despite the LOC count, because no external API or behavior changes.

**Scope:**
- `rm -rf include/BALL/DOCKING/GENETICDOCK/` (9 headers + `sources.cmake`, 968 LOC)
- `rm -rf source/DOCKING/GENETICDOCK/` (9 sources + `sources.cmake`, 1754 LOC)
- Delete `INCLUDE(source/DOCKING/GENETICDOCK/sources.cmake)` from `source/DOCKING/sources.cmake:5`
- Delete `INCLUDE(include/BALL/DOCKING/GENETICDOCK/sources.cmake)` from `include/BALL/DOCKING/sources.cmake:5`
- Update Phase 999.12 entry to note GENETICDOCK slice handled by 999.29 (scope shrinks to VIEW/WIDGETS legacy UI + VIEW/DIALOGS/RENDERING residual)
- One-line `RELEASE-NOTES-v1.6.2.md` entry: "Removed dead GENETICDOCK subsystem — was deprecated since pre-v1.6, never exposed in UI, no callers"

**Out of scope:**
- The other Phase 999.12 deprecation slices (VIEW/WIDGETS HelpViewer/MyTextBrowser/ComboBoxDelegate/HotkeyTable, VIEW/DIALOGS ExportGeometryDialog, RENDERING residuals) — those have potential UI/feature implications and stay v2.0.
- Any audit of whether the GENETICDOCK *algorithm* (genetic-algorithm-based docking) should be re-implemented under a different surface in the future — separate question, separate phase if ever.

**Estimated effort:** ~30 minutes total (2 directory deletes + 2 `sources.cmake` line edits + CI verify all platforms compile-clean + ~150-warning drop on every platform).

**Requirements:**
- `DEADCODE-02` — `find include source -path '*GENETICDOCK*'` returns no results post-fix.
- `DEADCODE-03` — `grep -rn "GeneticAlgorithm\|EvolutionaryDocking\|GenericGene\|GeneticIndividual\|DockProblem\|RotateBonds" include/ source/ test/` matches only `CommandlineParser::registerOptional/MandatoryDoubleParameter` (unrelated, in BALL::FORMAT).
- `DEADCODE-04` — every platform's `-Wdeprecated-declarations` count drops by ≥150 vs. pre-fix baseline.

To add to `REQUIREMENTS.md` v1.6.2 section when promoted.

**Promotion trigger:** anytime in v1.6.2 cycle; no upstream dependencies; trivially co-landable with Phase 999.30 (PixelFormat) and 999.31 (QSAR) as a v1.6.2 "warning-cleanup omnibus 2" bundle.

Plans:
- [ ] TBD (promote with /gsd-review-backlog when v1.6.2 milestone opens)

### Phase 999.30: VIEW::PixelFormat rule-of-three (BACKLOG · TARGETED FOR v1.6.2 · 999.27 follow-on)

**Goal:** Add explicit `PixelFormat(const PixelFormat&) = default;` to [`include/BALL/VIEW/RENDERING/pixelFormat.h:162`](../include/BALL/VIEW/RENDERING/pixelFormat.h#L162) (adjacent to the existing user-provided `operator=`). One-line edit; eliminates **125 instances** of `-Wdeprecated-copy` from every Linux build — the single largest warning cluster on Linux x64 / ARM64 / coverage. Same defect class as Phase 999.27 defect 3 (`MMFF94ParametersBase`); same fix.

**Why now (v1.6.2):** 999.27 demonstrated the pattern (1-line `= default` on the base class kills the whole derived-class warning cascade); `PixelFormat` is the same shape and the largest remaining instance (125 vs. MMFF94ParametersBase's ~6). Mechanical, no behavior change (the synthesized copy already does the right thing — this just makes the pairing explicit so `-Wdeprecated-copy` stops firing). Sits exactly inside the v1.6.2 "small wins" envelope.

**Defect:** Per [run 25970222837](https://github.com/BALL-Project/ball/actions/runs/25970222837) (cb392cc, 2026-05-16): `include/BALL/VIEW/RENDERING/renderTarget.h` triggers 123 of the 125 hits (every `FrameBufferFormat` ctor and every place a `PixelFormat&` member is copy-initialized); 2 more from `source/VIEW/RENDERING/renderWindow.C`.

**Out of scope:** Same as 999.27 — no rule-of-five conversion, no PCH changes, no other rule-of-three sites (those are tracked in 999.22 census or get their own follow-on phases as the pattern repeats).

**Estimated effort:** ~5 minutes (1-line edit + CI verify).

**Requirements:**
- `WARN-VIEW-PIXELFMT-01` — `PixelFormat` declares explicit `= default` copy ctor; `-Wdeprecated-copy` does not fire on `FrameBufferFormat` / `renderTarget.h` / `renderWindow.C`.

To add to `REQUIREMENTS.md` v1.6.2 section when promoted.

**Promotion trigger:** anytime in v1.6.2 cycle; bundle with 999.29 / 999.31 as warning-cleanup omnibus 2.

Plans:
- [ ] TBD (promote with /gsd-review-backlog when v1.6.2 milestone opens)

### Phase 999.31: QSAR virtual `operator=` overloaded-virtual silence (BACKLOG · TARGETED FOR v1.6.2)

**Goal:** Silence the 22 `-Woverloaded-virtual` warnings emitted by GCC across the QSAR Model inheritance chain (`Model.h` → `regressionModel.h` → `linearModel.h` → `mlrModel.h` → `rrModel.h`). The implicitly-synthesized `RRModel& RRModel::operator=(const RRModel&)` hides the virtual `operator=` chain even though the `using <Base>::operator=` declarations are present at every intermediate level — GCC's `-Woverloaded-virtual` heuristic still fires because the synthesized signature differs from the virtual one (return type `RRModel&` vs. `void`).

**Why now (v1.6.2):** Whole cluster lives in 3 headers and one ancestral declaration. The using-chain is *almost* right — Plan-05.1-style audit revealed: `Model::operator=(const Model&)` is `virtual void`; every derived class provides `using Base::operator=` plus its own `virtual void operator=(const Derived&)`. The missing link is at the leaves (`MLRModel`, `RRModel`) which only chain *one level up*. Fix: add `RRModel& operator=(const RRModel&) = default;` + extend the `using` chain at the leaf classes, OR remove `virtual` from the Model::operator= declaration (cleaner but API-shape change — defer to v2.0). v1.6.2 takes the patch-shape fix.

**Defect (3 sites):**
- [`include/BALL/QSAR/Model.h:48`](../include/BALL/QSAR/Model.h#L48) — `virtual void operator=(const Model& m);` (virtual ROOT of the chain)
- [`include/BALL/QSAR/regressionModel.h:35`](../include/BALL/QSAR/regressionModel.h#L35) — `virtual void operator=(const RegressionModel& m);` (hidden by RRModel synthesized)
- [`include/BALL/QSAR/linearModel.h:31`](../include/BALL/QSAR/linearModel.h#L31) — `virtual void operator=(const LinearModel& m);` (same)

**Fix:** Add explicit `= default` copy assignments at `MLRModel` + `RRModel` (and any other leaf derived class that lacks one), and ensure the `using` chain is complete. **Adversarial check:** confirm no code path relies on virtual dispatch through `operator=` (polymorphic assignment is generally a code smell; if any caller does `Model* m = ...; *m = other_model;` and expects polymorphic behavior, the v1.6.2 fix is insufficient and must defer to v2.0). Grep the tree for such patterns before applying.

**Out of scope:**
- Redesigning QSAR's polymorphic assignment story (v2.0 territory; pairs with `Phase 999.24` KERNEL redesign).
- Other `-Woverloaded-virtual` sites outside QSAR (the renderer hierarchy `Renderer::renderRepresentations_(const RepresentationList&)` vs. `GLRenderer::renderRepresentations_(BufferMode)` is its own story — separate hit count, separate phase if pursued).

**Estimated effort:** ~30 minutes (3-5 line edits + adversarial-grep audit + CI verify 22-warning drop).

**Requirements:**
- `WARN-QSAR-OVERVIRT-01` — `-Woverloaded-virtual` count on `include/BALL/QSAR/{Model,regressionModel,linearModel}.h` drops to zero on Linux GCC.

To add to `REQUIREMENTS.md` v1.6.2 section when promoted.

**Promotion trigger:** anytime in v1.6.2 cycle; bundle with 999.29 / 999.30.

Plans:
- [ ] TBD (promote with /gsd-review-backlog when v1.6.2 milestone opens)

### Phase 999.32: test-suite -Wunused-comparison cleanup (BACKLOG · TARGETED FOR v1.6.2)

**Goal:** Silence the 69 `-Wunused-comparison` warnings emitted by clang on macOS across `test/Bit_test.C` and `test/Substring_test.C`. The pattern is `TEST_EXCEPTION(Exception::NullPointer, b0 == true)` — the comparison expression's *side-effect* (throwing from `Bit::operator==` when null) is intentional, but clang's static analyzer sees the bool result as discarded.

**Why now (v1.6.2):** Largest single warning cluster on macOS (69 of 175 total). Trivially localizable — only 2 test files. Fix is mechanical: wrap the inner expression in a `(void)` cast, OR (cleaner) update the `TEST_EXCEPTION` macro body in [`include/BALL/CONCEPT/classTest.h`](../include/BALL/CONCEPT/classTest.h) to `(void)(_expr_)` once. The macro-level fix is preferred — single edit, fixes all current + future sites — but requires care: `cb392cc` recently touched `classTest.h` (TEST_REAL_EQUAL bracing), so the change should pair-review against that commit's intent. Adversarial check: confirm `(void)` cast doesn't suppress legitimate compile errors in the test bodies.

**Defect (sample sites):**
- [`test/Bit_test.C:140,167,194`](../test/Bit_test.C#L140) — `b0 == true`, `b0 != true` inside `TEST_EXCEPTION`
- [`test/Substring_test.C:282-283`](../test/Substring_test.C#L282) — same pattern

**Fix options (pick at promotion):**
1. **Macro-level:** update `TEST_EXCEPTION` to internally `(void)(_expr_)`. Single edit; covers all test files; aligns with `cb392cc`'s precedent for cleaning up `classTest.h`.
2. **Per-site:** wrap each comparison in `(void)(b0 == true)`. Touches only the 2 test files; safer against macro-level regressions but more LOC.

**Out of scope:**
- The 2 `-Wunused-value` + 1 `-Wliteral-conversion` + 2 `-Wliteral-range` on macOS (separate, fewer hits, lower priority).
- `-Wunused-parameter` cleanup (26 instances on Linux; deliberate API-shape preservation; tracked in 999.22 census).

**Estimated effort:** ~20 minutes (decide macro vs. per-site; apply; CI verify 69-warning drop on macOS).

**Requirements:**
- `WARN-TEST-UNUSEDCMP-01` — `-Wunused-comparison` count on `test/*.C` drops to zero on macOS clang.

To add to `REQUIREMENTS.md` v1.6.2 section when promoted.

**Promotion trigger:** anytime in v1.6.2 cycle; cleanly co-landable with 999.29/30/31.

Plans:
- [ ] TBD (promote with /gsd-review-backlog when v1.6.2 milestone opens)

### Phase 999.33: PoseClustering Boost.Graph -Wmaybe-uninitialized suppression (BACKLOG · TARGETED FOR v1.6.2)

**Goal:** Suppress the 5 `-Wmaybe-uninitialized` warnings emitted by GCC 13 on Linux Release builds — all 5 are inside `/usr/include/boost/graph/detail/` (Boost.Graph 1.83+) instantiated through `BALL::PoseClustering::ClusterProperties`'s `boost::adjacency_list<vecS, vecS, directedS, ClusterProperties, ...>`. These are **GCC false-positives** from Boost.Graph + Boost.Optional interaction on edge iterators — Boost upstream issue, not a BALL bug. Confirmed by inspection (the `boost::optional` `m_src` / `_M_current` fields are initialized by `optional`'s construction path, but GCC's flow analysis loses the connection across template/lambda boundaries).

**Why now (v1.6.2):** Only 5 warnings, but they live in the highest-severity category (potential UB) and Linux release CI surfaces them on every push — they dilute the "high-severity" signal that `-Wmaybe-uninitialized` is meant to give for *real* uninitialized reads. `cb392cc` already established the surgical-suppression precedent (GCC `-Wdangling-reference` false-positive in `coloringSettingsDialog.C` solved by source-site refactor rather than blanket `-Wno-`). Same approach here: wrap the two `boost/graph/adjacency_list.hpp` includes in [`include/BALL/DOCKING/COMMON/poseClustering.h:50,53`](../include/BALL/DOCKING/COMMON/poseClustering.h#L50) with `#pragma GCC diagnostic push / ignored "-Wmaybe-uninitialized" / pop`.

**Defect:** All 5 warnings flow from instantiation of `boost::detail::out_edge_iter` / `boost::detail::adj_list_edge_iterator` / `boost::optional` chain for `PoseClustering::ClusterProperties`. Backtrace shows the warnings firing in:
- `/usr/include/c++/13/bits/stl_algobase.h:262`
- `/usr/include/boost/graph/detail/adj_list_edge_iterator.hpp:80`
- `/usr/include/boost/graph/detail/adjacency_list.hpp:148`

**Fix:** Wrap the two `boost/graph/adjacency_list.hpp` includes in `poseClustering.h` with `#pragma GCC diagnostic push` / `#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"` / `#pragma GCC diagnostic pop`. Surgical — affects only PoseClustering's include site, doesn't touch any other Boost.Graph user. **Do not** use blanket `-Wno-maybe-uninitialized` at the build-flag level — it would hide real bugs elsewhere.

**Adversarial check:** Confirm the warnings are clang-clean already (they don't fire on macOS clang in the same workload) — verified against [run 25970222837](https://github.com/BALL-Project/ball/actions/runs/25970222837) macos-arm64 log: 0 `-Wmaybe-uninitialized` hits. This confirms the GCC-specific false-positive shape; `#pragma GCC diagnostic` is exactly the right granularity.

**Out of scope:**
- Bug-reporting upstream to Boost.Graph (separate, not blocking).
- The 1 `-Warray-bounds` warning on Linux (separate cluster; pair with 999.22 census).

**Estimated effort:** ~10 minutes (wrap 2 includes + CI verify 5-warning drop on linux-x64/arm64).

**Requirements:**
- `WARN-POSECLUSTER-MAYBEUNINIT-01` — `-Wmaybe-uninitialized` count on linux-x64 GCC Release build drops to zero (or to whatever real-bug subset remains after PoseClustering is suppressed).

To add to `REQUIREMENTS.md` v1.6.2 section when promoted.

**Promotion trigger:** anytime in v1.6.2 cycle; trivially co-landable with 999.29/30/31/32 as warning-cleanup omnibus 2.

Plans:
- [ ] TBD (promote with /gsd-review-backlog when v1.6.2 milestone opens)

### Phase 999.34: Windows test gatekeeper (COMPLETE · v1.7 · 2026-05-17)

**Goal:** Bring up the Windows test gatekeeper to parity with macOS + Linux — wire `ctest` into the Windows CI job with `BALL_DATA_PATH` export + `WILL_FAIL TRUE` quarantines mirrored from macOS/Linux + flip to blocking. Today the Windows CI job builds but does not run tests; the gatekeeper flip in TEST-CLOSE-02 covered macOS + Linux only.

**Why deferred (per [PHASE-9-BASELINE.md:84](phases/09-test-suite-triage/PHASE-9-BASELINE.md)):** vcpkg + MSVC test integration is non-trivial — the test binaries link against BALL.dll which lives in `build/ci-windows/bin/` and needs `PATH` adjustment for the test runner to find it. Worth a real investigation slot, not bundled into TEST-CLOSE-02's gatekeeper flip.

**Delivered (Phase 999.34, 2026-05-17):**
1. Added "Build BALL test suite (Windows)" + "Run BALL test suite (Windows)" + "Upload test results (Windows)" steps to `.github/workflows/ci.yml`'s windows-x64 branch (commit 5f0c9462b0).
2. Mirrored the same three steps into `.github/workflows/release.yml` so tag-triggered release builds also gate on the Windows test suite (commit 92a85ad821).
3. PATH discovery: both steps extend `$env:PATH` with `build\ci-windows\bin` (BALL.dll + VIEW.dll) AND `build\ci-windows\vcpkg_installed\x64-windows-release\bin` (Qt6 + Boost DLLs) so test EXEs in `bin\TEST\` resolve every DLL they link against.
4. `BALL_DATA_PATH` exported to `${{ github.workspace }}\data` (same convention as macOS/Linux).
5. JUnit XML uploaded under `ball-tests-windows-ui_v2_*.xml` (ci.yml) / `ball-tests-windows-release.xml` (release.yml); the existing test-report job's `ball-tests-*` glob picks up the Windows artifacts automatically and merges them into the unified Check Run summary.
6. Quarantine inheritance verified — no Windows-specific WILL_FAIL gates needed:
   - `AssignBondOrderProcessor_test2` stays Apple-arm64-only (MSVC x64 = Intel-compatible FP path, passes like Linux).
   - `PeptideCapProcessor_test` / `Peptides_test` / `RotamerLibrary_test` are already unconditional WILL_FAIL TRUE; expected-fail on Windows the same way they are on macOS/Linux.
   - `AmberFF_test` runs the loosened-ARM-tolerance assertions but the strict path still passes on Intel/MSVC.
7. Both `# Windows test gatekeeper deferred to Phase 999.34.` comments at the macOS + Linux gatekeeper preambles flipped to "landed in Phase 999.34" pointers.
8. `continue-on-error: false` on both steps — Windows test failure now blocks the Windows job (and therefore the whole matrix on `needs:` aggregation).

**Out of scope (preserved from BACKLOG framing):** changes to BALL test sources (mirror existing decisions, don't re-triage). The AssignBondOrderProcessor fine-penalty fix mentioned as a promotion-trigger note is NOT bundled here — that remains a future v1.7/v2.0 algorithmic task tracked under the same backlog row.

**Requirements:** carries no new REQ ID; satisfies a v1.7 follow-on for the deferred-Windows-test concern in TEST-CLOSE-01/02 closure notes.
**Estimated effort (actual):** ~45min implementation + first-CI-run baseline. Vs. the 1-2 day estimate the path-debugging risk was lower than feared (the macOS/Linux gatekeepers already worked out the BALL_DATA_PATH + ctest invocation shape; only the Windows PATH-extension for DLL discovery was new).
**Plans:** 1 ([999.34-01-PLAN.md](phases/999.34-windows-test-gatekeeper/999.34-01-PLAN.md) / [999.34-01-SUMMARY.md](phases/999.34-windows-test-gatekeeper/999.34-01-SUMMARY.md)).

Plans:
- [x] [999.34-01-PLAN.md](phases/999.34-windows-test-gatekeeper/999.34-01-PLAN.md) — Windows test gatekeeper bring-up — 2026-05-17

### Phase 999.35: OOS-PR-merge regression fix — PeptideCapProcessor/Peptides/RotamerLibrary (BACKLOG · v1.6.2 OR v1.7)

**Goal:** Fix the 3 test regressions introduced by the parallel-session PR merge (commit 45dce6971, Phase 999.14, feat: merge C++ slice of PRs #546 #554 #550):
1. `PeptideCapProcessor_test` — `prot->countAtoms()` returns 171 vs expected 169. PRO.db added 1OB/1OH atoms (HYP variant) but bonds 40/41 reference 2OB/2OH which don't exist in the standard PRO residue, causing 2 extra atoms.
2. `Peptides_test` — `ThreeLetterCode('b')` returns "ASX" vs expected "UNK"; `IsOneLetterCode('b')` returns true vs expected false. The PR extended `one_letter_codes` to include 'B'=ASX/'Z'=GLX for ambiguity codes; the tests expected the old behavior.
3. `RotamerLibrary_test` — `rc.getStatus()` returns false (0) vs expected 1 in multiple assertions. Related to the broken 2OB/2OH bond references in PRO.db that cause FragmentDB to fail, cascading into RotamerLibrary status checks.

**Root cause:** `data/fragments/PRO.db` change in 45dce6971 adds bonds 40/41 to 2OB/2OH atoms that only exist in the HYP variant — not in the Default/PRO variant. The bonds should only be in the HYP-specific Delete/Add section, not in the Default bonds list. `source/STRUCTURE/peptides.C` added ambiguity one-letter codes ('B', 'Z') without updating the test expectations.

**Fix options:**
1. **Preferred:** Correct `data/fragments/PRO.db` so bonds 40/41 are HYP-only (not in Default PRO). Then update `Peptides_test` to assert 'b' → "ASX" (the chemically correct behavior) and 'B' as a valid one-letter code.
2. **Alternative:** Revert the PRO.db + peptides.C portion of 45dce6971 (keep PR #546 and PR #554 changes, revert PR #550 hydroxyproline data changes that are incorrectly structured).

**Quarantine applied (Phase 9, TEST-CLOSE-01 triage deviation):** `PeptideCapProcessor_test`, `Peptides_test`, `RotamerLibrary_test` marked `WILL_FAIL TRUE` in `test/CMakeLists.txt`. See 09-TRIAGE.md deviation notes.

**Estimated effort:** 2-4 hours (fragment DB format investigation + test update).
**Plans:** 0.
**Promotion trigger:** v1.6.2 or v1.7, after verifying the PRO.db HYP-section structure.

Plans:
- [ ] TBD (promote when capacity allows; unblock by reverting PRO.db bonds or fixing HYP section)

### Phase 999.36: Source-aware Windows cmake-tree cache key (BACKLOG · v1.6.3 OR v1.7)

**Goal:** Extend the Phase 999.17 (BUILD-ACCEL-02) Windows cmake-tree cache key to include hashes of source files that participate in MSVC DLL-export contracts — currently the key is `hashFiles('CMakeLists.txt', 'cmake/**', 'vcpkg.json', 'CMakePresets.json')` which misses any source edit that changes export semantics without changing build configuration.

**Why now (filed 2026-05-17 as direct follow-up to the cb392cc4 / 9042af07 incident):** During the v1.6.2 release-tag prep window the parallel session landed `cb392cc4` (drop redundant BALL_EXPORT on vector3.C TVector3<float> instantiation, citing GCC -Wattributes). That source edit silently broke BALL.dll's MSVC export of TVector3<float> symbols. ci.yml masked the defect for ~12 hours because the 999.17 cache key did not include `source/MATHS/vector3.C`'s hash: warm cache restored a pre-regression CMakeFiles/ with a stale BALL.lib import library, VIEW.dll linked from the stale import, and the job greenlit. release.yml caught it (different cache scope, cold cache on tag-trigger) but ate 3 runs (25977139490, 25974049283, 25971072146) before the source-defect was traced. Fixed in 9042af07 + cache key bumped v1 → v2 in c9d8de383 to force one cold rebuild — that's a stopgap, not a structural fix.

**Scope:**
1. **Identify the export-contract surface.** Scan tree for files containing `template class BALL_EXPORT` (canonical Phase 5.1 Plan 07 pattern). Today: `source/MATHS/vector3.C`, `source/KERNEL/atom.C` (per Phase 5.1 Plan 07 SUMMARY), possibly `source/MATHS/vector4.C` and other `.C` files in MATHS/. Lock the actual set via `grep -rn "template class BALL_EXPORT" source/`.
2. **Extend the cache key.** Add a `hashFiles(...)` arm covering those files. Resulting key:
   ```yaml
   key: ci-windows-cmake-tree-v3-${{ hashFiles('CMakeLists.txt', 'cmake/**', 'vcpkg.json', 'CMakePresets.json') }}-${{ hashFiles('source/**/*BALL_EXPORT*') }}
   ```
   (Or use `hashFiles('source/MATHS/vector3.C', 'source/KERNEL/atom.C', ...)` with the explicit list.)
3. **Validate via controlled experiment.** Make a no-op edit to one of the listed files; confirm cache MISSES on the next CI run. Make an edit to an unlisted source file; confirm cache HITS (the warm-cache speedup is preserved for routine source-only changes).
4. **Document in ci.yml comment + cmake/PCH.md sibling note.** Explain why this expanded key matters and how to add a new file to the list when introducing a new `template class BALL_EXPORT`.

**Out of scope (DO NOT do during this phase):**
- Switching to per-file ccache invalidation (orthogonal; ccache already does this for compile artifacts, what we need is build-tree cache invalidation).
- Changing the Phase 999.17 cache PATH (`build/ci-windows/{CMakeCache.txt, CMakeFiles/, build.ninja, vcpkg_installed/}`) — still the right scope.
- Backporting to release.yml — release.yml doesn't use this cache (tag-trigger ≠ branch-trigger), so the same masking class can't happen there.

**Acceptance:** a controlled experiment shows the new cache key invalidates on edits to ANY file in the export-contract surface, while preserving warm-cache speedup for unrelated source edits. The cb392cc4-class of masking bug is structurally prevented.

**Estimated effort:** 1-2 hours (one ci.yml edit + 2-3 controlled-experiment runs).
**Plans:** 0 (single-task PLAN when promoted).

**Promotion trigger:** v1.6.3 if one materializes, OR v1.7 cycle. No new REQ ID — satisfies a follow-on for BUILD-ACCEL-02 (Phase 999.17).

Plans:
- [ ] TBD (promote when capacity allows; trivial fit anywhere post-v1.6.2)

### Phase 999.37: VRMLRenderer removal + STL-export rename (COMPLETE · v1.7 · 2026-05-17)

**Goal:** Remove the `BALL_DEPRECATED` `VRMLRenderer` class entirely from BALL/VIEW, plus the misnamed STL-export UI surface that historically lived under "VRML" identifiers (despite always writing STL via `STLRenderer`).

**Why now (v1.7, executed inline 2026-05-17):** v1.6.2 macOS warning census (see [`v1.7-WARNING-CENSUS-PRESEED.md`](v1.7-WARNING-CENSUS-PRESEED.md)) flagged the `[[deprecated]]` `VRMLRenderer` instantiation at `rendererFactory.C:103`. User direction "remove the deprecated VRML renderer entirely as part of 1.7 work" promoted this from the original 999.12 (v2.0 deprecated-code removal sweep) into v1.7 Wave 1. Investigation revealed the surface was larger than just the class: the BALLView UI menu "Export VRML" actually invoked `STLRenderer`, with `Scene::showExportVRMLDialog`, `Scene::vrml_nr_`, `Scene::animation_export_VRML_action_`, INI key `EXPORT/VRMLNR`, and shortcut-file entries all historically misnamed. Cleaned up coherently in one phase since splitting class-deletion from name-cleanup would leave the codebase in a confusing intermediate state.

**Scope (delivered):**

A. **VRMLRenderer class deleted:**
- `include/BALL/VIEW/RENDERING/RENDERERS/VRMLRenderer.h` (146 lines) — deleted
- `source/VIEW/RENDERING/RENDERERS/VRMLRenderer.C` (487 lines) — deleted
- `source/VIEW/RENDERING/RENDERERS/sources.cmake` — entry removed

B. **Factory and enum references removed:**
- `source/VIEW/RENDERING/rendererFactory.C` — `#include`, `case Kind::VRML` branches (×2) removed
- `include/BALL/VIEW/RENDERING/rendererFactory.h` — `Kind::VRML` enum value removed
- `include/BALL/VIEW/RENDERING/renderSetup.h` — `VRML_RENDERER` enum value removed (no callers; downstream switches don't break)

C. **Dead stub methods deleted:**
- `include/BALL/VIEW/DIALOGS/exportGeometryDialog.h` — `BALL_DEPRECATED bool export_vrml()` (returned always `false`, 0 callers) + `BALL_DEPRECATED bool export_stl()` (returned always `true`, 0 callers) both removed

D. **STL export UI surface rename (the misnomer cleanup):**
- `include/BALL/VIEW/WIDGETS/scene.h`: `setVRMLNumber` → `setSTLNumber`, `vrml_nr_` → `stl_nr_`, `showExportVRMLDialog` → `showExportSTLDialog`, `animation_export_VRML_action_` → `animation_export_STL_action_`
- `source/VIEW/WIDGETS/scene.C` (9 sites): all `vrml_nr_` references renamed; menu description `"Shortcut|File|Export|VRML"` → `"Shortcut|File|Export|STL"`; slot `showExportVRMLDialog` → `showExportSTLDialog`; INI key `"EXPORT/VRMLNR"` → `"EXPORT/STLNR"` with backward-compat migration (reader prefers new key, falls back to legacy `VRMLNR` once for pre-v1.7 configs)
- `data/BALLView/shortcuts_12.txt`, `data/BALLView/shortcuts_15.txt`: `Shortcut|File|Export|VRML` → `Shortcut|File|Export|STL`

E. **Out of scope (intentionally deferred):**
- `.ts` Qt translation files (`BALLView-zh_TW.ts`, `BALLView-de_DE.ts`) contain "VRML" strings — Qt's `lupdate` toolchain regenerates these from source; manual editing collides with translator workflow. Orphan strings are harmless at runtime (no UI element bound to them).

**Verification:**
- Local macOS clean build (`cmake --build . --target BALL VIEW BALLView -j 8`): **0 errors, 0 stale VRML references**.
- Backward-compat INI migration tested by reading pre-v1.7 config with `VRMLNR=N`: new code reads `N` into `stl_nr_`, subsequent save emits `STLNR=N` (legacy key naturally fades).
- Stage 1 CI (macOS-arm64) will validate on the next branch push.

**Why combined into one phase (rather than two — class delete + rename split):** the misnomer correction is part of "remove the deprecated VRML renderer entirely" because the misnomer is the *only reason* the VRML name persists in the UI. Splitting would leave a confusing intermediate where the renderer class is gone but the UI still labels its action "VRML" pointing at a "VRML dialog" method that writes STL. Atomic landing avoids that intermediate state.

**Requirements (delivered):**
- `VRML-REMOVE-01` ✓ — `VRMLRenderer` class deleted; no references in `include/`, `source/`, `data/` (Qt `.ts` deferred per rationale above).
- `VRML-REMOVE-02` ✓ — `RendererFactory::Kind::VRML` and `RenderSetup::VRML_RENDERER` enum values removed.
- `VRML-REMOVE-03` ✓ — Dead `export_vrml()` / `export_stl()` stubs removed from `ExportGeometryDialog`.
- `STL-RENAME-01` ✓ — Misnamed STL export surface renamed (function, member var, INI key, menu description, shortcut data files).
- `STL-RENAME-02` ✓ — INI key backward-compat migration (`VRMLNR` → `STLNR` graceful upgrade).

**Plans:** 0 (executed inline, single commit).

### Phase 999.23: CIF Bison grammar shift-reduce audit (BACKLOG · v1.6.2 OR v1.7)

**Goal:** Audit and resolve (or document-as-benign) the shift-reduce conflicts emitted by Bison on [`source/FORMAT/CIFParserParser.y`](../source/FORMAT/CIFParserParser.y). Count needs reconciliation: [`05.1-BACKLOG.md:196`](phases/05.1-build-warnings-and-latent-bugs/05.1-BACKLOG.md) says 3 conflicts, [`05.1-05-SUMMARY.md:85`](phases/05.1-build-warnings-and-latent-bugs/05.1-05-SUMMARY.md) says 5 — first task is to lock the actual current count.

**Why now (v1.6.2 OR v1.7):** Originally deferred from Phase 5.1 as a "separate investigation" ([`05.1-CONTEXT.md:61`](phases/05.1-build-warnings-and-latent-bugs/05.1-CONTEXT.md)). Documented as "real ambiguities worth investigating" in [`05.1-BACKLOG.md:196`](phases/05.1-build-warnings-and-latent-bugs/05.1-BACKLOG.md). Per-conflict triage: confirm benign (default-precedence-resolves-correctly) or fix grammar rule. Isolated grammar work; one parser; no architectural dependency. Cheap to dispatch.

**Scope:**
1. Reconcile the conflict count — run `bison -W CIFParserParser.y` cleanly and capture the verbatim conflict report. (Current latest CI run [25970222837](https://github.com/BALL-Project/ball/actions/runs/25970222837) reports **5 shift/reduce conflicts** for CIFParserParser.y on Bison 3.8.2 — locks the count for the audit.)
2. For each conflict: examine the grammar rule + Bison's default action; confirm-benign or fix.
3. **Fold-in: address the 19 `-Wformat-truncation=` warnings emitted by Bison-generated `CIFParserParser.C` on Linux GCC** (rules 198-243 in `CIFParserParser.y` use `snprintf` into 2550-byte buffers with `%s`-formatted parts up to 5099 bytes — every one a "could truncate" diagnostic). Audit: either raise the buffer size, switch to `std::string`/`fmt::format`-style growth, or accept truncation and silence with `[[maybe_unused]]` / explicit `snprintf` return check. Largest single warning category on Linux (19 hits) outside the `-Wdeprecated-*` clusters.
4. Document outcome in `.planning/phases/999.23-cif-grammar-audit/AUDIT.md` (per-conflict table + format-truncation disposition).
5. **Out of scope:** The other 7 Bison grammars in tree (parsedFunction, GAMESSDatParser, GAMESSLogParser, smartsParser, smilesParser, expressionParser, GAFFCESParser) — no documented audit evidence yet. Spin out a separate v1.7 backlog stub if evidence of conflicts in any of them surfaces during this phase.

**Estimated effort:** 1-2 days depending on whether any conflict needs a real grammar fix (+ format-truncation cleanup adds ~30 min). Defer to v1.7 if v1.6.2 fills up — not patch-release-critical.

**Requirements:** `GRAMMAR-01` (STRETCH) — see `REQUIREMENTS.md` v1.6.2 section.
**Plans:** 0 (single-task PLAN when promoted).

Plans:
- [ ] TBD (promote with /gsd-review-backlog when v1.6.2 milestone opens, OR defer to v1.7)

### Phase 999.24: KERNEL redesign — MoleculeStore (SoA) parallel-model migration (BACKLOG · TARGETED FOR v2.0 · LEAD v2.0 PHASE)

> **Implementation-ready scope:** [`KERNELV2.md`](KERNELV2.md) — 554-line plan, post-Codex peer-reviewed v3. Read it before promoting this phase. Mirrors the [PYBALLV2.md](PYBALLV2.md) pattern for substantive multi-month design docs.

**Goal:** Modernize BALL's KERNEL subsystem hot paths from Composite-pattern object graph traversal to a **MoleculeStore** (SoA columns + handle facade) parallel-model migration — without breaking the Composite-based object model that 2,463 sites + 207k LOC + persisted file formats + the entire PYTHON binding surface depend on.

**Why parallel-model, not clean-room replacement:** Two rounds of Codex peer review confirmed the Composite layout is structurally welded to file-format persistence, the binary `Atom`/`Bond` layout, RTTI selection, and 111 `BALL_FOREACH_*` macro sites. A clean-room replacement would force a binary ABI break, file-format break, and break every downstream user. Instead the plan introduces `MoleculeStore` *alongside* Composite — hot paths route through the store; legacy code continues using Composite — with sync state in an external registry keyed on `System*` so no Atom/Composite binary layout changes are needed.

**Scope (KERNELV2.md §K0-K4):**
- **K0 (foundation):** MoleculeStore data structure + external sync registry.
- **K1 (nonbonded SoA):** Force-field nonbonded inner loops (MOLMEC AMBER/CHARMM/MMFF94) route through MoleculeStore columns.
- **K2 (compiled selection):** Selection/Expression queries compiled and cached (no more per-atom AST re-walk).
- **K3 (FORMAT + MOLMEC bonded + SCORING + DOCKING + QSAR + NMR + XRAY + STRUCTURE):** subsystems incrementally moved to MoleculeStore as the canonical hot-path source.
- **K4 (Python):** PyBALL exposes MoleculeStore handle facade alongside (not replacing) Composite bindings.

**Release milestones (per KERNELV2.md §1; rebranded v2.1 → v2.0 per 2026-05-16 user direction):**
- **v2.0-preview:** K0 + K1 + K2 — 8-14 months
- **v2.0.0:** + K3 minimum (FORMAT + MOLMEC bonded + SCORING) + K4 Python-binding bridge — 16-24 months total
- **v2.0.1+:** Remaining K3 subsystems — +6-10 months
- **v2.0 final:** All K0-K4 — 22-34 months total

**Compatibility promise (locked in KERNELV2.md):** Source-compatible across v1.7 → v2.0 (existing client code recompiles unchanged); ABI-compatible for all KERNEL+CONCEPT exported types; file formats unchanged in v2.0; `BALL_FOREACH_*` macros preserved (deprecated for new internal code only).

**Memory budget:** Hot-path additional footprint ≤150 B/atom (SoA columns + adjacency + selection). Composite shadow unchanged; total memory does not decrease in v2.0.

**Sequencing within v2.0 (per 2026-05-16 user direction "2.0 should start with the kernel"):**
- KERNEL v2.0 K0+K1 lands FIRST in v2.0 — establishes MoleculeStore as the canonical hot-path data structure.
- v2.0 substrate phases (999.6 PIPE-01, 999.9 YAML config, 999.11 gemmi, 999.12 deprecated removal) land in parallel or after K0+K1.
- v2.0 ships when KERNEL v2.0.0 (K0-K3-minimum + K4) AND the 4 substrate phases are all done.

**Cross-version sequencing:**
- v2.0 → KERNEL + 4 substrate phases (this entry + 999.6 + 999.9 + 999.11 + 999.12).
- v2.1 → [Phase 999.15](#phase-99915-pyball-wrapping-rewrite-backlog--targeted-for-v21--conditional-on-phase-6-bake-off) PyBALL bulk wrap targeting the v2.0 MoleculeStore handle facade.
- v2.2 → [Phase 999.10](#phase-99910-deprecate-remote-control-from-ball-proper-narrow-rest-api--pyball-sdk-backlog--targeted-for-v22--sequenced-after-kernel-v20-k0k1-) REST API + PyBALL SDK over MoleculeStore.
- v2.3 → [Phase 999.13](#phase-99913-convert-doxygen-docs-to-read-the-docs--swagger-ui-for-rest-api-backlog--targeted-for-v23) Read the Docs portal consuming the REST OpenAPI spec.

**Reference:** [`KERNELV2.md`](KERNELV2.md) for the detailed 554-line plan. Adopted post-Codex Round 3 review (v3 baseline). The doc still uses "v2.1" terminology in places — the rebrand to v2.0 is captured here and in KERNELV2.md frontmatter; remaining v2.1 mentions in KERNELV2.md body text should be read as v2.0 until the doc gets a full pass during phase promotion.

**Requirements:** TBD (likely `KERNEL-V2-01..04` mapped to K0..K3 + `KERNEL-V2-05` for K4 Python).
**Plans:** 0 (will mirror KERNELV2.md §K0-K4 task breakdown when promoted; K0 is the natural first PLAN).

Plans:
- [ ] TBD (promote first thing when v1.7 tags — KERNEL is the v2.0 lead phase. Co-sequenced with the 4 v2.0 substrate phases [999.6/9/11/12], which can run in parallel with K2+K3 after K0+K1 lands.)

### Phase 999.25: v1.7 closure documentation pass + **v1.4 → v1.7** retrospective + audit-procedure capture (BACKLOG · TARGETED FOR v1.7 CLOSURE)

**Goal:** At v1.7 close, do a deliberate documentation pass capturing three things: (1) audit-procedure lessons learned from the 2026-05 audit cycle (so future audits don't trip on the same stale-doc traps), (2) a **v1.4 → v1.7 retrospective** comparing BALL's last properly-documented release era (~2002-2010 CVS-era state, with `doc/TUTORIAL/*` carrying `$Id$` strings dating to 2002-2004) against v1.7 delivered (UI refresh + packaging + all the v1.6.x modernization in between), (3) user-facing docs cleanup — the tutorials/READMEs/build instructions that have NOT been touched since the v1.4 era.

**Why v1.4 (not v1.6) as the retrospective baseline:** The v1.6 modernization treated the 2022 git-history baseline (`d85d2dd`, "[CMake] Disable MPI support by default") as its starting point — but BALL's user-facing docs are far older. `doc/TUTORIAL/` files carry CVS `$Id$` keywords from 2002-2004 (e.g., `$Id: tutorial2.C,v 1.4 2002/02/27 12:25:08 sturm Exp $`); README, BUILD-* instructions reference deprecated workflows (XDR, `ball_contrib`, SIP 4, Qt 4/5 paths). v1.4 was the last "BALL is freshly documented" era; v1.5, v1.6, the long freeze, and v1.6.x modernization all happened without a corresponding docs refresh. v1.7 closure is the right moment to acknowledge the actual ~20-year arc and update the docs against the modernized reality.

**Why v1.7 closure (not v1.6.2 or v2.0):** v1.6.x is patch-shape; adding a 20-year-arc retrospective there muddies the strict-corrective scope. v2.0 is the substrate-modernization bundle — too early for a v1.x retrospective. v1.7 close is the natural moment: v1.4 → v1.7 modernization arc is conceptually complete; v2.0 substrate rewrite is the next big break, and the v1.x docs should be coherent before that break lands.

**Scope:**

1. **Audit-procedure doc** (`docs/audit-procedure.md` or `.planning/audit-procedure.md`) — capture the lessons from the 2026-05 audit cycle:
   - Verify implementation against source, not planning docs (ContourSurface false-positive lesson)
   - Cross-check VERIFICATION.md against sibling HUMAN-UAT.md before treating findings as current (CR-01/02/03 false-positive lesson)
   - Resolution-log frontmatter pattern (added to 05-VERIFICATION.md in commit `f176b8b`) — apply consistently for new phase verifications
   - When in doubt, grep + read code; don't trust 30-day-old docs

2. **v1.4 → v1.7 retrospective** (`.planning/retrospective-v1.4-v1.7.md`) — capture the full ~20-year modernization arc:
   - **Era baselines:** v1.4 (~2002-2010 CVS-era, the last "freshly documented" release; build system was CVS + Autotools; Qt 3; Python 2 SIP 4; ball_contrib in tree; XDR-based persistence). v1.7 (2026; CMake + Homebrew/vcpkg; Qt 6.5; Python 3.12+; no ball_contrib; tri-OS CI; signed installers).
   - **Per-subsystem deltas:** build system (CVS+Autotools → CMake+vcpkg), language (C++98/03 → C++17), GUI (Qt 3 → Qt 6.5), Python (SIP 4 → autowrap/Cython or nanobind — depending on Phase 6 bake-off), GL (fixed-function → planned QRhi), packaging (manual builds → notarized macOS + signed Windows installers), CI (none → tri-OS GH Actions matrix + coverage + JUnit), tests (manual → ctest gatekeeper).
   - **Milestone summary** for v1.6 line (10 phases, 6 patches) + v1.7 line (UI refresh + packaging).
   - **What got deferred to v2.0** — KERNEL redesign (the substrate that v1.x preserved); REST API rewrite; deprecated-code removal; YAML config; gemmi mmCIF.
   - **Lessons learned per release** — the v1.6.1 strict-corrective approach worked; the v1.6.2 build-acceleration cluster bought 87× Windows CI speedup; the audit-doc trap (stale docs misleading audits) recurred twice in the 2026-05 cycle.
   - Patterned on standard project retrospectives. ~5-8 pages.

3. **User-facing docs audit** — focus on docs that haven't been touched since the v1.4 era. Signals to grep for:
   - **CVS `$Id$` keywords** in `doc/TUTORIAL/`, `doc/examples/` — anything with a year < 2018 is suspect; verify before declaring obsolete.
   - **Deprecated-workflow references:** "make sure XDR is installed", `ball_contrib`, SIP 4 / pyqt4 / pyqt5, Qt 4 / Qt 5 build paths, `qmake`, fixed-function GL tutorials.
   - **Tutorial code samples** in `doc/TUTORIAL/*.tex` + `doc/examples/TUTORIAL/*.C` — do they still compile against the v1.7 API? Probably not. Either update samples, mark as historical, or delete.
   - **README.md + BUILD-*.md** — these are the user's first contact. Verify build commands work against v1.7 reality on all 3 OSes.
   - **`data/` references** — file format docs need a check against current support.

**Estimated effort:** 4-7 days (was 2-4 for v1.6→v1.7 scope; expanded for the longer arc). Mostly writing + extensive grep-then-read passes against `doc/TUTORIAL/`. Could split into a writing-heavy first 2 days (audit-procedure + retrospective) + 3-5 days of user-facing docs audit + targeted updates.

**Why this matters:** The 2026-05 audit cycle exposed two false-positive backlog entries (ContourSurface, CR-01/02/03) both rooted in stale planning docs that disagreed with source. Capturing the procedure now reduces the cost of future audits. The v1.4 → v1.7 retrospective is project-management hygiene; the user-facing docs audit catches ~20 years of doc-rot that the v1.6 modernization deliberately scoped out (the modernization was code-focused; docs were carry-forward).

**Requirements:** TBD (likely `DOC-V17-01: closure docs published`).
**Plans:** 0 (single PLAN with three tasks when promoted).

**Promotion trigger:** v1.7 has tagged. Run as the final v1.7 closure item before v2.0 kicks off.

Plans:
- [ ] TBD (promote with /gsd-review-backlog after v1.7 tags; do NOT promote during v1.7 active work — retrospective needs a stable v1.7 state to compare against)

### Phase 999.40: BALLView Refresh — Design system & QSS foundation (Handover Phase 0) (BACKLOG · TARGETED FOR v1.7 WAVE 4)

**Goal:** Land the design-token vocabulary, base QSS stylesheet, and `BALL_UI_V2` build-flag plumbing that every later BALLView Refresh phase depends on. **No user-visible change** — `BALL_UI_V2` defaults OFF in this phase; the foundation is dormant until 999.41/.42/.43/.44/.45/.46/.47 opt in (per audit pull-in: all in v1.7 Wave 4); 999.48 flips the default ON and then REMOVES the flag entirely.

**Why now (v1.7 Wave 4):** User direction 2026-05-17 — "roll the BALLView refresh into the tail-end of v1.7" + audit pull-in "keep everything in 1.7." Foundation is maintainer-decision-independent (Q1-Q4 don't apply to 999.40) so it ships unblocked. Lands the substrate during v1.7 packaging/auto-update lead time; subsequent phases consume it; 999.48 flips ON-default + removes the flag.

**Source:** `/Users/kohlbach/Claude/BALL/Claude Design Handover/revitalization/00-principles.md` — the Handover's Phase 0 doc has the full scope, design steps, file list, and acceptance criteria. Drops in with light adaptation.

**Scope (in):**
- `include/BALL/VIEW/KERNEL/theme/tokens.h` — design tokens as a single C++ header (color, spacing, type scale)
- `source/VIEW/KERNEL/theme/theme.qrc` — Qt resource bundling the single neutral `.qss` stylesheet (per Q3)
- `source/VIEW/KERNEL/theme/ThemeManager.{h,C}` — singleton that loads the stylesheet at startup; `QStyleHints::colorScheme()` reactor wired but inactive per Q3 single-neutral-theme (future-proofs if Q3 reverses)
- `BALL_UI_V2` CMake option, default OFF (flipped ON in 999.48, then removed in 999.48 §8.7)
- Behind-flag wiring: when `BALL_UI_V2=ON`, ThemeManager init runs in BALLView main; when OFF, classic palette stays

**Scope (out):**
- Any `.ui` file edits (Handover Phase 1 = Phase 999.42)
- Icon replacement (Handover Phase 2 = Phase 999.41 / carry-overs in 999.42)
- Any visible change in the OFF default

**Hard prerequisite:** Qt 6.5+ for `QStyleHints::colorScheme()` — **satisfied** since v1.6.2 (Qt 6.8 LTS floor).

**Requirements:** TBD (likely `UIV2-FND-01: ThemeManager + tokens.h + theme.qrc + BALL_UI_V2 flag`)

**Estimated effort:** ~1 week.

**Plans:** 0 (single PLAN with 4 tasks when promoted: tokens header, theme.qrc, ThemeManager, BALL_UI_V2 build-flag wiring).

Plans:
- [ ] TBD (promote with /gsd-plan-phase during v1.7 tail-end)

### Phase 999.41: BALLView Refresh — SVG icons + HiDPI pipeline (Handover Phase 2) — COMPLETED 2026-05-17 (v1.7 Wave 4)

**Goal:** Replace the XPM bitmap icon system with an SVG-based, theme-aware, HiDPI-correct icon pipeline. Pure mechanical asset work; no UX decisions; no maintainer dependencies.

**Status (2026-05-17):** COMPLETED — single PLAN, 6 implementation commits + 1 docs amendment + 1 final docs commit, ON + OFF macOS builds green. v1.7 subset only: build-flag-gated SVG-preferred wedge inside `IconLoader::loadIcon_` rather than replacing all 28 call sites. 14 Lucide v0.460.0 SVGs vendored under `source/VIEW/KERNEL/theme/icons/lucide/` and aliased onto BALLView's existing icon-key namespace via the 999.40 `theme.qrc`. Toolbar size 22→20 logical px under `BALL_UI_V2`. **Carry-overs (per 2026-05-17 audit) all forwarded to 999.42, not v1.8:** ThemedIconEngine with state-tint cache (Normal/Active/Selected/Disabled — NOT theme tints; Q3 does NOT block this), 28 `IconLoader::getIcon` call-site rewrites, XPM array deletion (`bucky_64x64_xpm` + 2 others), `.icns`/`.ico` regen. See [999.41-01-SUMMARY.md](phases/999.41-svg-icons-hidpi/999.41-01-SUMMARY.md).

**Source:** `/Users/kohlbach/Claude/BALL/Claude Design Handover/revitalization/02-phase-icons.md`.

**Depends on:** 999.40 (uses `theme.qrc` for resource bundling; `tokens.h` for the 20-px icon size constant).

**Scope (delivered):**
- Lucide v0.460.0 14-SVG subset vendored (ISC license)
- `theme.qrc` extended with `<qresource prefix="/icons">` aliases mapping BALLView keys → Lucide files
- Qt6::Svg linked into VIEW (unconditional, link surface uniform across flag values)
- `IconLoader::loadIcon_` SVG-preferred branch under `#ifdef BALL_UI_V2` (zero call-site churn; PNG fallback intact)
- Toolbar size 22→20 logical px at `mainframe.C:295` under `#ifdef BALL_UI_V2`
- TODO breadcrumbs at `icons.h`, `iconLoader.h`, `iconLoader.C` for 999.42 (carry-overs absorbed there per audit) + Phase 8 packaging

**Scope (forwarded):**
- ThemedIconEngine (per-DPR tint cache, state-aware tints — Normal/Active/Selected/Disabled, NOT theme tints) → **999.42 carry-over** (re-pinned from v1.8 per 2026-05-17 audit; Q3 does NOT block state tints since they're interaction-state not theme)
- Replacing remaining XPM uses (`bucky_64x64_xpm`, `mini_ray_xpm_`, `simulation_running_xpm_`) → 999.42 + Phase 8
- `.icns`/`.ico` regeneration + app icon refresh → Phase 8 packaging
- Linux/Windows smoke builds → next Wave-4 phase (this phase was macOS-only per orchestrator scope)

**Plans:** 1 plan
- [x] 999.41-01-PLAN.md — vendor 14 SVGs, extend theme.qrc, link Qt6::Svg, add SVG-preferred branch to IconLoader, bump toolbar size, leave breadcrumbs (6 commits + 2 docs commits)

### Phase 999.42: BALLView Refresh — QSS theming + palette removal + 999.41 icon carry-overs (Handover Phase 1 + Phase 2 tail) — COMPLETED 2026-05-17 (v1.7 Wave 4)

**Goal:** Remove every `<palette>` block from every `.ui` file in the tree so dialogs inherit the OS / theme palette set by 999.40's ThemeManager — AND close out the 999.41 SVG-icon carry-overs (ThemedIconEngine for state tints, call-site rewrites, XPM array deletion, packaging icon regen). Even without dark-mode/Follow-System, palette removal is hygiene — Qt then applies system widget styling natively (lighter chrome on macOS, native on Windows).

**Status (2026-05-17):** COMPLETED — single PLAN, 17 commits (1 chore + 7 feat + 8 refactor + 5 docs), macOS-arm64 ON + OFF builds green for every per-task commit. Reality vs Handover counts: 7 `.ui` files with palette (not 38), 22 IconLoader call sites (not 28), 3 XPM arrays (matches). ThemedIconEngine + IconRegistry façade landed; 22 call sites migrated to `Icons::get` (per-file atomic commits); all 3 XPM arrays + `icons.{h,C}` deleted; 3 new Lucide SVGs vendored. **Deferrals (documented breadcrumbs in SUMMARY):** lupdate refresh (10k-line drift unrelated to palette; → `chore(i18n)`); .icns/.ico regen (toolchain not universal on CI runners + bespoke artwork also Phase 8; → Phase 8 packaging). See [999.42-01-SUMMARY.md](phases/999.42-theming-palette-removal/999.42-01-SUMMARY.md).

**Source:** `/Users/kohlbach/Claude/BALL/Claude Design Handover/revitalization/01-phase-theming.md` + `02-phase-icons.md` §2.2 + §2.3 + §2.4 + §2.6.

**Depends on:** 999.40 (consumes ThemeManager); 999.41 (the SVG-preferred loader wedge already exists; this phase closes out the rest).

**Cross-cutting CI prerequisite:** ✅ DONE 2026-05-17 — `.github/workflows/ci.yml` matrix now carries the `ui_v2: [ON, OFF]` axis (per Handover §09-cross-platform.md §"CI matrix" + §10 CI gate 6). Implementation: `detect-platforms` job emits a `ui_v2` field on every matrix entry; doubled to `[ON, OFF]` on UI_V2-relevant paths (theme/, mainframe.C, CMakeLists.txt, etc.) and on schedule/dispatch/maintenance-branch triggers; single `ui_v2: OFF` per OS otherwise. Each Configure step threads `-DBALL_UI_V2=${{ matrix.ui_v2 }}`. Drop the axis post-999.48 §8.7 flag-REMOVAL (drop steps documented in `ci.yml` `WHEN TO DROP` comment block).

**Scope (palette-removal half — Handover Phase 1):**
- Strip `<palette>` blocks from **~30 `.ui` files** (~52 in tree, ~38 with palette per Handover §01). Inventory step writes `revitalization/inventory/phase-1-ui-files.txt` + `phase-1-with-palette.txt`.
- Ship `revitalization/scripts/strip-palettes.py` (per Handover §01-phase-theming.md §1.1).
- Restore intentional semantic colors via code (NOT `.ui`) in **5 named files**: `coloringSettingsDialog` (residue color swatches), `gridColorWidget` (gradient stops), `labelDialog` (foreground color preview), `materialSettings` (RGB sliders preview swatch), `assignBondOrderResultsDialog` (result-status badges). Use `Theme::accent()`/`Theme::warn()` from 999.40 tokens, NOT re-add hardcoded RGB.
- Per-dialog QSS `objectName` sweep (Handover §01-phase-theming.md §1.4) — ensure every dialog's root has a stable `objectName` so QSS can target it.
- Run `lupdate-qt5` and commit inventory artifacts (`phase-1-ui-files.txt`, `phase-1-with-palette.txt`).

**Scope (999.41 carry-overs — Handover Phase 2 tail):**
- **`ThemedIconEngine`** (Handover §02-phase-icons.md §2.2) — per-DPR tint cache with **state-aware tints (Normal / Active / Selected / Disabled)**. NOT theme tints (light/dark/follow-system) — Q3 doesn't block this; the state tints are about interaction state, not theme. New files `include/BALL/VIEW/KERNEL/theme/themedIconEngine.h` + `source/VIEW/KERNEL/theme/themedIconEngine.C`. Engine registered via `IconRegistry` exposing `BALL::VIEW::Icons::get(const char* name) → QIcon`.
- **28 `IconLoader::getIcon` call-site rewrites → `Icons::get`** (Handover §2.3). Mechanical replacement using the icon-inventory CSV. Keep `IconLoader` as a thin shim for one release (mark `BALL_DEPRECATED` for v2.0).
- **Delete `icons.{h,C}`** under `source/APPLICATIONS/BALLVIEW/` + the embedded **XPM arrays**: `bucky_64x64_xpm` (`mainframe.C:71`), `mini_ray_xpm_` (`serverWidget.C:125`), `simulation_running_xpm_` (`mainControl.C:296`). Replace `setWindowIcon(QPixmap(bucky_64x64_xpm))` with `setWindowIcon(Icons::get("ball-app"))`.
- **`.icns` / `.ico` regen** via `BALLViewInstallFix.cmake.in` (Handover §02-phase-icons.md §2.6) — multi-resolution `.ico` (16/24/32/48/64/256) for Windows; macOS `.icns` with right padding for the Big-Sur+ soft-square template. Source from `data/BALLView/icons/ball-app.svg` (new).

**Acceptance criteria:**
- `grep -r "<palette>" source/**/*.ui` returns zero matches.
- `grep -rn "xpm\|XPM" source/APPLICATIONS/BALLVIEW source/VIEW` returns no matches in production code (test fixtures excepted).
- Toolbar icons render crisply on 2× display; selected-state icons use `--accent`; disabled actions render with `--ink-muted` automatically.
- Walk-through of the ~25 user-visible dialogs in `BALL_UI_V2=ON` mode shows them adopting the new theme; `BALL_UI_V2=OFF` walk-through shows native system palette (old hardcoded beige gone everywhere).
- `coloringSettingsDialog` residue swatches visually unchanged (semantic colors preserved via code path).

**Estimated effort:** ~1.5-2 weeks (was ~1 week — palette half + carry-overs). **Actual: ~75 minutes** (much smaller real-world surface than the Handover doc projected, plus per-task local smoke-builds rather than waiting on CI for each gate).

**Plans:** 1 plan
- [x] 999.42-01-PLAN.md — palette strip + ThemedIconEngine + IconRegistry + 22 call-site migration + 3 XPM array deletion (17 commits)

### Phase 999.43: BALLView Refresh — Simple-dialog cleanup + shared widget set (Handover Phase 3) (COMPLETE 2026-05-17)

**Goal:** Apply the new design system + idiom to the small, low-risk dialogs first AND ship the shared widget set that Phase 999.44 Inspector hard-depends on. Build muscle memory before the Inspector overhaul.

**Status (revised 2026-05-17 audit pull-in):** UNCONDITIONAL in v1.7 Wave 4 — Phase 999.1 maintainer-Q3 resolved (single neutral theme). Value reframes from "themed dark/light dialogs" to "design-system consistency across simple dialogs." Phase still ships in v1.7. **Scope expanded per audit:** shared-widget set is explicitly load-bearing for 999.44 — separated out so the Inspector phase can author against a stable widget vocabulary.

**Source:** `/Users/kohlbach/Claude/BALL/Claude Design Handover/revitalization/03-phase-simple-dialogs.md`.

**Depends on:** 999.42 (palette-free substrate); 999.41 + 999.42 carry-overs (SVG icons + `Icons::get` available).

**Scope (shared widgets — load-bearing for 999.44 Inspector):** under `source/VIEW/WIDGETS/` add **4 widgets**, each ~50 LOC, no business logic:
- **`SectionHeader`** (`sectionHeader.{h,C}`) — labeled section divider used inside dialogs and the Inspector; 11px uppercase-tracked label with a hairline rule.
- **`FormRow`** (`formRow.{h,C}`) — row helper: label left, control right, optional inline help text below. Replaces ad-hoc `QHBoxLayout` patterns.
- **`LabeledSlider`** (`labeledSlider.{h,C}`) — slider + numeric readout + reset button; used in dozens of places.
- **`SwatchButton`** (`swatchButton.{h,C}`) — replacement for `ColorButton` that respects theme contrast.

**Scope (About dialog rewrite — hand-coded, no `.ui`):**
- **Delete `aboutDialog.ui`.** Implement `aboutDialog.C` from scratch (~120 LOC). Fixed width 480 px, height grows with content. App icon (`Icons::get("ball-app")`, 64 px) + product name in 22 pt display + version in 12 pt monospace. "Built with" section listing Qt / Python / OpenGL renderer (one `FormRow` each). "Authors" section sourced from new `data/BALLView/about/authors.json` (stops being hardcoded). Footer: rich-text links to website / issue tracker / license with `setOpenExternalLinks(true)`. Single right-aligned "Close" button.

**Scope (10 named dialogs restyled):**
1. `preferences.ui` — keep tree-on-left/stack-on-right pattern; restyle each page as `QWidget` subclass using `SectionHeader` + `FormRow`; no more groupbox nesting.
2. `shortcutDialog.ui` — single `QTreeView` over category-grouped model; search field at top (live filter); inline edit (click cell → recorder lights up, press combo, Enter to confirm, Esc to cancel); per-row "Reset to default" + bottom "Restore all".
3. `editSingleShortcut.ui`
4. `pluginDialog.ui`
5. `networkPreferences.ui`
6. `openSavePreferences.ui`
7. `pythonSettings.ui`
8. `mainControlPreferences.ui`
9. `setCamera.ui` (restyle only; conversion to Inspector panel happens in 999.44)
10. `setClippingPlane.ui` (restyle only; conversion to Inspector panel happens in 999.44)

**Scope (icon-browser dev tool — Handover §02-phase-icons.md §2.7):**
- Optional debug dialog `Tools › Icon Browser` hidden behind `BALL_UI_V2 && BALL_DEBUG`; shows the whole `IconRegistry` as a grid. Catches missing icons in CI by name lookup.

**Acceptance criteria:**
- About dialog matches the Handover `mockups.html` "About" board.
- All Preferences pages use `SectionHeader` + `FormRow`; no nested `QGroupBox` beyond 1 level.
- Shortcut dialog supports search + inline recording (the highest-value UX improvement in this phase).
- No regression in `Tab` order (verified with keyboard-only navigation).
- All `.ts` translations re-validated; missing strings re-added with English fallback.

**Estimated effort:** ~1.5 weeks (was ~1-1.5; the shared-widget set adds ~2-3 days but pays back fully into 999.44 sequencing). **Actual:** ~35 minutes (much smaller real-world surface — shared widgets are ~50-90 LOC each per spec, AboutDialog rewrite ~180 LOC including the authors.json loader, dialog QSS hook was a one-line dynamic-property add per file, the shortcutDialog search field already existed end-to-end so only polish was needed, IconBrowser shipped minimal per the spec's "optional debug dialog" framing).

**Plans:** 1 plan
- [x] 999.43-01-PLAN.md — 4 shared widgets + AboutDialog rewrite + 10-dialog QSS hookup + shortcutDialog polish + IconBrowser dev tool (12 commits)

**Scope-trims forwarded (with breadcrumbs in 999.43-01-SUMMARY.md):**
- Deep page-rewrite of each Preferences page as a hand-coded QWidget subclass using SectionHeader + FormRow → forwarded to **Phase 999.46** (Tools › Legacy Settings retirement — natural home for the deep rewrites since it owns the legacy migration).
- `setCamera` / `setClippingPlane` → Inspector panel conversion → **Phase 999.44** (already explicit per ROADMAP lines 2018-2019).
- IconBrowser theme.qrc auto-scan + CI missing-icon gate → **Phase 999.46** (build-system change better landed with the dev-tools polish pass).
- `_test.C` unit tests for 4 shared widgets → **Phase 999.46** (BALL test framework needs QApplication+display setup that's better landed with proper widget-test infrastructure work).

### Phase 999.44: BALLView Refresh — Unified Inspector (Handover Phase 4) (COMPLETE · v1.7 WAVE 4 · 2026-05-17)

**Goal:** Collapse the modal-dialog soup (Display, Model, Material, Light, Stage, Stereo, Clipping, Coloring, Label, Modify Representation — 10+ dialogs) into a single **right-rail Inspector dock** with collapsible sections, live preview, and tabs organized by *subject* (Selection · Representation · Scene).

**Status (revised 2026-05-17 Plans 02-06 landed):** **COMPLETE except pixel-diff regression test (Plan 07 deferred to v1.7 RC patch).** Plan 01 shipped the architecture + Controller-extraction pattern + StageSection exemplar + LegacySettingsHelper static data. Continuation session landed Plans 02 (mainframe wiring), 03 (Selection tab), 04 (Representation tab — 5 Controllers + 4 sections, Label + Clip sections deferred to 999.44-04b RC patch), 05 (Scene tab remainder — 3 Controllers + 4 sections), 06 (StageController apply() cut-over). All 9 Controllers landed; 1 cut-over complete + 8 read-only mirrors awaiting individual cut-over plans in RC patch cycle. **Plan 07 (pixel-diff regression test infrastructure) deferred to v1.7 RC patch** per operating-contract scope-trim guidance — see `999.44-07-PLAN.md` for pickup breadcrumb. See per-plan SUMMARYs in `.planning/phases/999.44-unified-inspector/`.

**Original status (revised 2026-05-17 audit pull-in):** Re-pinned from v1.8 → v1.7 Wave 4 per user direction "keep everything in 1.7." Biggest single architectural piece in the entire Refresh — replaces 10+ modal dialogs with one dock + tabs + live preview. Maintainer-Q2 RESOLVED (Classic-coexistence design); sequencing accommodates running 999.44 ‖ 999.45 in parallel (different file trees).

**Source:** `/Users/kohlbach/Claude/BALL/Claude Design Handover/revitalization/04-phase-inspector.md`.

**Depends on:** 999.40 (theme foundation), 999.41 + 999.42 carry-overs (SVG icons + `Icons::get`), 999.42 (palette removal), 999.43 (4 shared widgets — `SectionHeader` / `FormRow` / `LabeledSlider` / `SwatchButton`), maintainer-Q2 RESOLVED.

**Renderer-interface boundary (SEED-001 step 5):** bind to the **post-Phase-02.1 renderer interface** (`Renderer` / `RenderSurface`), NOT today's GL types — several replaced dialogs touch renderer/scene settings. See `.planning/RENDERER-INTERFACE-BOUNDARY.md` for the contract.

**Scope (6 named sub-PRs per Handover §04-phase-inspector.md §"Sub-PR breakdown"):**
| Sub-PR | Title | LOC est. | Behind `BALL_UI_V2` |
|--------|-------|----------|---------------------|
| **4.1** | Inspector shell + tabs + dock plumbing | ~600 | yes |
| **4.2** | Selection tab + selection bus | ~800 | yes |
| **4.3** | Representation tab: Model + Coloring | ~1200 | yes |
| **4.4** | Representation tab: Material + Label + Clip | ~900 | yes |
| **4.5** | Scene tab: Camera + Lights + Stage + Stereo | ~1100 | yes |
| **4.6** | Legacy dialog deprecation + retire flag | ~200 | merged once 4.1-4.5 stable |

**Scope (architecture):**
- **`InspectorDock`** (`QDockWidget`, right area) → **`InspectorView`** (root) → **`InspectorTabs`** (`QTabBar`, 3 tabs) + **`InspectorBody`** (`QStackedWidget`).
- **3 tabs with ~13 named sections replacing 10+ legacy dialogs:**
  - **Selection** — `SelectionSummarySection` + `PropertiesSection` + `QuickActionsSection`.
  - **Representation** — `RepHeaderSection` + `ModelSection` (← `ModelSettingsDialog`) + `ColoringSection` (← `ColoringSettings`) + `MaterialSection` (← `MaterialSettings`) + `LabelSection` (← `LabelDialog`) + `ClipSection` (← `ClippingDialog`).
  - **Scene** — `CameraSection` (← `SetCamera`) + `LightsSection` (← `LightSettings`) + `StageSection` (← `StageSettings`) + `StereoSection` (← `StereoSettings`) + `BackgroundSection`.

**Scope (`*Controller` extraction — PRIMARY RISK MITIGATION per Handover §Risks):**
- **Pre-refactor commit:** extract business logic from each legacy dialog `.C` into a `*Controller` class. Both surfaces (legacy dialog + new Inspector section) use the same controller — consistency between the two during the coexistence period is GUARANTEED by construction, not by message-bus discipline. This is the single biggest risk mitigation in the Inspector phase.

**Scope (live-preview + persistence + UX):**
- **Live 100ms-debounced commits** — every control commits on change with a 100 ms debounce; no "Apply" buttons.
- **Section state persistence** in `~/.BALLView` under `[Inspector]` INI schema:
  ```ini
  [Inspector]
  visible=true
  tab=Representation
  Selection.Properties=expanded
  Representation.Model=expanded
  Representation.Coloring=expanded
  Representation.Material=collapsed
  ```
  Read on construction, write on every change.
- **Collapse animation** — `QPropertyAnimation` on `maximumHeight`, duration 160 ms, easing `OutCubic`. Section header is a 36px row: 12px chevron, 14pt semibold title, spacer, optional mini-icons (reset, help).
- **Empty states** — when no selection / no representation / no scene state, the relevant tab shows an illustrated empty state (see Handover mockup).
- **`Tools › Legacy Settings` interim home** — legacy dialogs remain reachable from `Tools › Legacy Settings › …` for one release WITH a one-time first-launch migration notice ("Display, Model, Material, Light, and other settings are now in the Inspector on the right. Old dialogs are still accessible under Tools › Legacy Settings."). **Flag: 999.48 §8.8 REMOVES `Tools › Legacy Settings` submenu entirely.**

**Acceptance criteria:**
- Right-rail Inspector visible on launch with `BALL_UI_V2=ON`.
- Loading a PDB and selecting a residue updates Selection tab live.
- Switching representation in the Project dock (999.45) updates Representation tab live.
- All controls debounce-commit at 100 ms; no flicker / no excessive re-renders.
- Section collapse state persists across restart (`[Inspector]` schema written + read correctly).
- Inspector hideable via `View › Hide Inspector` and `⌘\` / `Ctrl+\`.
- Legacy dialogs still reachable from `Tools › Legacy Settings` and produce identical scene state changes (controller-shared invariant).
- Pixel-diff regression against Handover mockups < 5% per tab.

**Estimated effort:** ~3-4 weeks (longest pole in Wave 4 — run parallel with 999.45 which touches different file trees).

**Plans:** 6 of 7 COMPLETE (architecture + mainframe + 3 tabs + cut-over); 1 DEFERRED (pixel-diff regression test → v1.7 RC patch).

Plans:
- [x] 999.44-01 COMPLETE — Inspector architecture (Dock/View/Tabs/Body/Section/EmptyState) + StageController exemplar + PATTERN.md + StageSection + LegacySettingsHelper static data. 6 commits.
- [x] 999.44-02 COMPLETE — mainframe wiring: InspectorDock right-rail registration + `Display ▸ Hide Inspector` (Ctrl+\\) + `Tools ▸ Legacy Settings ▸ <12>` submenu + first-run migration QMessageBox. Commit `9f7ca02ad4`.
- [x] 999.44-03 COMPLETE — Selection tab: SelectionSummarySection + PropertiesSection + QuickActionsSection + SelectionInspectorAdapter (ConnectionObject listening for ControlSelectionMessage / NewSelectionMessage). Commit `a9bca8a69c`.
- [x] 999.44-04 COMPLETE — Representation tab: 5 Controllers (Model/Coloring/Material/Label/Clipping, read-only mirror) + 4 sections (RepHeader/Model/Coloring/Material) + RepresentationInspectorAdapter. Label + Clip sections deferred to 999.44-04b RC patch (Controllers landed). Commit `a27b812b48`.
- [x] 999.44-05 COMPLETE — Scene tab remainder: 3 Controllers (Camera/Light/Stereo, read-only mirror) + 4 sections (Camera/Lights/Stereo/Background). All 5 Scene-tab sections now live. Commit `0b3934b627`.
- [x] 999.44-06 COMPLETE — StageController apply() cut-over: read-only stub deleted; apply() now mutates Stage. Consistency-by-construction invariant achieved for migrated fields. Commit `062d3fc901`.
- [ ] 999.44-07 DEFERRED-TO-v1.7-RC-PATCH — Pixel-diff regression test infrastructure (<5% per tab against Handover mockups). Pickup breadcrumb: `.planning/phases/999.44-unified-inspector/999.44-07-PLAN.md`.

**Remaining for v1.7 RC patch cycle:**
- 8 Controller cut-over plans (one per remaining read-only mirror Controller). Mechanical per StageController exemplar in 999.44 Plan 06.
- LabelSection + ClippingSection (Plan 999.44-04b).
- Renderer-interface-boundary binding for Camera/Lights (SEED-001 step 5) — folds into the Camera/Light cut-over plans.
- Plan 999.44-07 pixel-diff tests.

### Phase 999.45: BALLView Refresh — Workspace consolidation (Handover Phase 5) (COMPLETE · v1.7 Wave 4 · 2026-05-17)

**Goal:** Replace the five-docks-on-startup layout with a sane default — left "Project" dock, right Inspector dock (999.44), bottom collapsed drawer for Logs/FileObserver. Expose a "Workspace" picker (Default / Classic / Focused) so power users keep their old layout if they want.

**Status:** **COMPLETE 2026-05-17.** 8/8 sub-items shipped: `ProjectDock` (left-rail tabify helper), `BottomDrawer` (24px↔240px QPropertyAnimation), `WorkspaceManager` (3-preset singleton with JSON descriptors), 3 `.layout` files, first-run QMessageBox prompt, `~/.BALLView.pre-v2.bak` auto-backup, status-bar clickable `WorkspaceStatusLabel`, `ConfigMigration` (legacy `~/.BALLView` → XDG/AppSupport/AppData via `QStandardPaths::AppConfigLocation`). Local macOS-arm64 smoke build: BALL_UI_V2=ON → BALLView app links clean; OFF cell blocked by pre-existing 999.44 Inspector vtable issue (logged for 999.44 follow-up; 999.45 sources compile clean in both cells). See [999.45-01-SUMMARY.md](phases/999.45-workspace-consolidation/999.45-01-SUMMARY.md) for full task table, decisions, and deferred items.

**Source:** `/Users/kohlbach/Claude/BALL/Claude Design Handover/revitalization/05-phase-workspace.md`.

**Depends on:** 999.43 (shared widgets), 999.44 dock-area conventions (the right rail dock area is registered by 999.44); maintainer-Q2 RESOLVED.

**Scope (ProjectDock — Handover §5.1):**
- **`ProjectDock`** (`source/VIEW/WIDGETS/projectDock.{h,C}`) on the LEFT with **3 tabs at top (28px tab bar with accent underline)**: **Structures** (reuses `MolecularControl`) / **Representations** (reuses `GeometricControl`) / **Datasets** (reuses `DatasetControl`). Dock title bar hidden — tab bar serves as identity.

**Scope (BottomDrawer — Handover §5.2):**
- **`BottomDrawer`** (`source/VIEW/WIDGETS/bottomDrawer.{h,C}`) — `QDockWidget` whose collapsed state is a **24px strip** with chevron + label ("Logs · 12 new"). Click → expands to **240px**. Drag → resizes. Chevron rotates with `QPropertyAnimation`. Contents: `QTabBar` switching between `LogView` and `FileObserver`.

**Scope (WorkspaceManager — Handover §5.3):**
- **`WorkspaceManager`** (`source/VIEW/KERNEL/workspaceManager.{h,C}`) with **3 presets (NOT 2 — Focused is the third)**: `Default` / `Classic` / `Focused` / `UserDefined`. `apply(Preset)` / `save(name)` / `load(name)` / `userPresets()` API.
- **Shipped `.layout` files** in `data/BALLView/workspaces/`: `default.layout`, `classic.layout`, `focused.layout` — each a binary `QMainWindow::saveState()` blob plus a small JSON sidecar with the dock visibility map. User-saved layouts go under `~/.BALLView/workspaces/`.

**Scope (first-run migration — Handover §5.4):**
- **First-run migration prompt** on first launch *after* upgrading to a Phase-5-enabled build: detect previous (5-dock) layout in `~/.BALLView` and prompt:
  > *BALLView has a refreshed default workspace.* Switch to it, or keep your current layout (you can switch later from View → Workspace).
  - **Try new workspace** (default)
  - **Keep my layout** (preserved as **"User Default"**)
- **`~/.BALLView.pre-v2.bak` auto-backup** of the pre-upgrade layout file (mitigates the "users with custom layouts lose them on upgrade" risk).

**Scope (status-bar workspace label — Handover §5.5):**
- Right end of status bar shows active workspace name as a clickable label. Clicking opens popup with preset list + "Save current as…".

**Scope (configMigration.C — Handover §09-cross-platform.md §"File system paths"):**
- **`source/VIEW/KERNEL/configMigration.C`** — migrate legacy `~/.BALLView` file into the new platform-correct paths on first launch with `BALL_UI_V2`:
  - Linux: `~/.config/BALLView/` + `~/.cache/BALLView/`
  - macOS: `~/Library/Preferences/BALLView/` + `~/Library/Caches/BALLView/`
  - Windows: `%APPDATA%/BALLView/` + `%LOCALAPPDATA%/BALLView/Cache/`
  - Workspaces also migrate: `~/.config/BALLView/workspaces/` etc.
- Required because the `[Inspector]` INI schema added by 999.44 + `[Appearance]` schema added by 999.48 change the on-disk format; the migration step is the right place to bridge.

**Acceptance criteria:**
- Fresh launch (no `.BALLView` file) → Default workspace: Project dock left, Inspector right, scene center, Drawer collapsed.
- `View › Workspace › Classic` restores the pre-revitalization 5-dock layout.
- Workspace state persists across restart even after switching presets multiple times.
- Bottom drawer expand/collapse animated at 200 ms with no content reflow flicker.
- Status-bar workspace label updates immediately on preset change.
- Upgrade path preserves the user's old layout as "User Default" if they choose "Keep my layout."
- `configMigration.C` migrates legacy `~/.BALLView` to platform-correct paths on first launch with `BALL_UI_V2`.

**Estimated effort:** ~1.5-2 weeks (was ~1-2; `configMigration.C` adds ~3 days, picker-with-3-presets unchanged from 2-preset estimate).

**Plans:** 1/1 plan complete.

Plans:
- [x] [999.45-01-PLAN.md](phases/999.45-workspace-consolidation/999.45-01-PLAN.md) — 11-task plan. Result: [999.45-01-SUMMARY.md](phases/999.45-workspace-consolidation/999.45-01-SUMMARY.md). 7 source commits (PLAN + 4 feat + 1 feat + 1 docs final) under the `(999.45/NN)` scope prefix. Note: deferred items in [deferred-items.md](phases/999.45-workspace-consolidation/deferred-items.md) flag two 999.44-owned issues surfaced during 999.45 execution (cmake macro — already resolved by concurrent 999.44 agent; OFF-cell Q_OBJECT vtable ifdef — pending 999.44 fix).

### Phase 999.46: BALLView Refresh — Menus + command palette (Handover Phase 6) (COMPLETE · v1.7 WAVE 4 · 2026-05-17)

**Goal:** Re-author the top menu bar around **tasks** rather than C++ namespaces, and introduce `Cmd/Ctrl+K` **command palette** that exposes every action in the app by name.

**Status (revised 2026-05-17 audit pull-in):** Re-pinned from v1.8 → v1.7 Wave 4 per "keep everything in 1.7." Menu reorg touches every `.ui` menu file + invalidates ~40% of `BALLView-de_DE.ts` translations. Maintainer gates RESOLVED.

**Maintainer-Q1 + Q4 (RESOLVED 2026-05-17, Q4 re-pinned per audit):**
- **Q1 = "Keep inline menubar"** → SKIP full QMenuBar global-menu mode. **DO** `QAction::setMenuRole(QAction::AboutRole / PreferencesRole / QuitRole)` for the 3 standard items on macOS — this is the auto-relocation Qt does at the per-action level, fully compatible with the inline-menubar Q1 answer (the Q1 SKIP is about the global-menubar mode, not standard-role plumbing).
- **Q4 = "Accept; community round during v1.7 cycle (re-pinned from v1.8 per audit)"** → v1.7 ships English + best-effort de_DE; community translators called for in v1.7 RC notes; v1.7.x point releases pick up completed translations. NOT a v1.7 ship-date blocker.

**Source:** `/Users/kohlbach/Claude/BALL/Claude Design Handover/revitalization/06-phase-menus.md`.

**Depends on:** 999.44 + 999.45 (new layout context). Maintainer gates RESOLVED.

**Scope (CommandRegistry — Handover §6.1):**
- **`source/VIEW/KERNEL/commandRegistry.{h,C}`** — singleton owning the canonical list of all `Command` records (`id` / `title` / `category` / `description` / `shortcut` / `icon` / `trigger` / `isEnabled` fn ptrs).
- **Auto-registration in `MainControl::insertMenuEntry`** — every `insertMenuEntry` call gains an internal `registerCommand()` call so registration is automatic.
- **Plugin `setCategory()` hook** (Handover §Risks) — plugin API gets a `setCategory()` hook; default category is `Scripts` (mitigates "plugin-defined menu entries land in unexpected places" risk).

**Scope (menu remap — Handover §6.2):**
- **18-row mapping table → 8 top-level menus**: **File · Edit · Select · View · Compute · Scripts · Window · Help** (all single-word, all task-coded, matches PyMOL / ChimeraX / Blender patterns).
- Full mapping per Handover §06-phase-menus.md §"New menu bar mapping" — moves Edit › Invert/Clear/By-Expression → new **Select** menu; Build › * → Edit › Structure ›; Display › * → View › *; Molecular Mechanics › * → **Compute** ›; Tools › * → **Scripts** ›; Macro › * → Scripts › Macros ›; Windows → Window.
- **Every existing accelerator stays mapped to the same action** (Alt+X Fullscreen, Alt+C Abort, Ctrl+O Open, etc.). The `Shortcut|...` description strings in `mainframe.C` are the contract.
- **`source/VIEW/test/menuMapping_test.C`** — unit test asserts every old shortcut still resolves to a command with the expected title (mechanical change-detector).

**Scope (commandPalette widget — Handover §6.3):**
- **`source/VIEW/WIDGETS/commandPalette.{h,C}`** — floating sheet centered on main window, **560 px wide**, activated by `Cmd+K` (macOS) / `Ctrl+K` (Linux/Windows).
- `QLineEdit` at top with placeholder "Type a command, or ?". Results below: **fuzzy-matched** commands with category chip + shortcut + icon, up to 8 visible, scroll for more. **Recent commands at top when query empty.**
- `Enter` triggers, `Esc` closes, `Tab` accepts current. Implementation: `QListView` over `QSortFilterProxyModel` with custom delegate.
- **`?` / `>` / `:` modes** (Handover §6.4):
  - `>` prefix: only file actions (jump to Open Recent).
  - `?` prefix: command-help mode; result rows show description + shortcut prominently.
  - `:` prefix: jump to inspector section.
  - *(plain text)*: fuzzy search all commands.

**Scope (i18n / translation workflow — Handover §10-migration-playbook.md §"Translation workflow", re-pinned per audit #10):**
- Run `lupdate-qt5 source -ts data/BALLView/translations/*.ts` after the menu reorg.
- **`[i18n]` commit-prefix discipline** for translation-update commits.
- **Translator mailing-list announce** with a diff of new strings — included in v1.7 RC notes.
- **Follow-up translation PRs land in v1.7.x patch cycle** (Q4 re-pinned from v1.8 cycle per audit pull-in).
- DON'T block the phase PR on translations.

**Acceptance criteria:**
- New menu layout matches mapping table; 8 top-level menus.
- `Cmd/Ctrl+K` opens palette; typing "ene" matches "Minimize Energy" and "Compute Energy…".
- Every shortcut in the v1 audit still works (`menuMapping_test.C` green).
- About / Preferences / Quit auto-relocate to the macOS application menu via `setMenuRole(...Standard)`.
- Translation regeneration produces a known set of new untranslated strings; no string deleted accidentally.

**Estimated effort:** ~2 weeks (menu work; reduced from 2-3 weeks because no macOS global-menubar mode work per Q1). Community translation round (parallel, time-elapsed-only, not engineering effort).

**Actual effort:** ~13 min execution wall-clock (9 commits, ~2300 LOC added net of translation refresh). CommandPalette + CommandRegistry singletons + 18-row menu remap + macOS standard-role plumbing + minimal test + i18n workflow doc + lupdate refresh all landed in one execution session.

**Plans:** 1/1 plans complete.

Plans:
- [x] [PLAN.md](phases/999.46-menus-command-palette/PLAN.md) — single-plan
  phase covering all 8 tasks (CommandRegistry / auto-register hook / menu
  remap / setMenuRole / CommandPalette / prefix-modes / menuMapping_test /
  i18n refresh). Result:
  [999.46-SUMMARY.md](phases/999.46-menus-command-palette/999.46-SUMMARY.md)
  — CI run [25999533400](https://github.com/BALL-Project/ball/actions/runs/25999533400)
  dispatched on `4036b41555`; ui_v2 × OS matrix in flight at phase-close
  time. Both macOS-arm64 cells (ON + OFF) verified locally: BALLView.app
  links green, `menuMapping_test` PASSED.

### Phase 999.47: BALLView Refresh — Onboarding (Handover Phase 7) (COMPLETE · v1.7 WAVE 4)

**Goal:** Replace the standalone *Demo / Tutorial* dialog + `Welcome.rtf` with an in-product welcome screen on first launch. Refresh the Help viewer to use Markdown (drop the QtWebEngine dependency where avoidable — already removed from build per existing Phase 5 work).

**Status (2026-05-17, COMPLETE):** 7 commits landed on `v1.7-modernization` (4fcaebfc5f WelcomeScreen widget · 403789ea5e samples+thumbnails · 4bc14349b2 mainframe wiring · c829e95402 MarkdownHelpViewer · d9ba1884c6 ballview:// scheme registration · 543658f72f tutorial Markdown migration · c6472c8376 Welcome.rtf retirement). All 8 sub-deliverables shipped. CI run 26001813912 dispatched. Scope-trims documented in 999.47-SUMMARY.md: 3 sample protein slots ship as BPTI-derived placeholders pending 999.48 asset pipeline; thumbnails are single-density 320×200 (the @2× pipeline is 999.48 scope); demoTutorialDialog.* stays in tree per 999.48's existing legacy-file deletion roster. 999.48 dispatch unblocked.

**Source:** `/Users/kohlbach/Claude/BALL/Claude Design Handover/revitalization/07-phase-onboarding.md`.

**Depends on:** 999.40-999.46 (the new look the welcome screen showcases).

**Scope (WelcomeScreen widget — Handover §7.1):**
- **`source/VIEW/WIDGETS/welcomeScreen.{h,C}`** — `QWidget` placed inside the central area when `composite_manager_.getNumberOfComposites() == 0` and `BALL_UI_V2`. **NOT a modal dialog** — a panel rendered in place of the scene placeholder.
- 720 px max-width content, vertically centered, comfortable padding.
- Header: app icon (96 px) + product name + tagline + version.
- **3 big action buttons in a row**: **Open file…** / **Open from PDB / PubChem…** / **Recent…**.
- "Recent" section: list of up to 5 recently opened files (filename, muted path, last-modified date).
- "Sample structures" strip: **6 image cards (160×100)** with caption + action.
- Footer: "What's new in 1.7" link if user just upgraded; "Skip this screen on startup" toggle.

**Scope (6 curated sample structures — Handover §7.2; NEEDS BUNDLING DECISION):**
| Sample | File | Why |
|---|---|---|
| DNA double helix | `1bna.pdb` | Classic, fast load, looks good |
| Small protein | `1ubq.pdb` (ubiquitin) | Pedagogical |
| Enzyme + ligand | `1stp.pdb` (streptavidin–biotin) | Shows ligand handling |
| Membrane | `1ymg.pdb` | Surface + transparency demo |
| Small molecule | `caffeine.mol2` | Small-molecule path |
| MD trajectory | `dialanine_md.dcd` + `dialanine.pdb` | Trajectory player demo |

- **OPEN: data path + install rules** for the bundled samples — likely `data/BALLView/samples/` with a sources.cmake-equivalent install hook; check if BALL's existing `BALL_DATA_PATH` install logic covers this, else add. Each sample needs a citation in `samples/README.md` (all 6 are from the public PDB — copyright fine).
- **Thumbnails**: pre-bundled PNGs rendered at **320×200 @ 2× DPR** (so 640×400 physical). Lazy-decode on visible to keep first-run cold-start fast.

**Scope ("What's new" card — Handover §7.6):**
- When app version differs from last-seen version in `~/.BALLView`:
  - Welcome screen shows "New in 1.7" card with 3 bullets.
  - Dismissible chip in status bar links to longer changelog.
  - **Auto-dismiss after 2 minutes of use without dismiss.**
- Source: curated `data/BALLView/help/whatsnew/1.7.md`.

**Scope (HelpViewer rewrite — Handover §7.3):**
- **`source/VIEW/WIDGETS/helpViewer.{h,C}`** rewritten using **`QTextBrowser::setMarkdown(...)`** (Qt 5.14+; Qt 6 has it improved). Drop `QWebEngineView` dependency for built-in help — already removed from the BALL build per existing Phase 5 work. (Keep WebEngine ONLY for genuine PubChem / RCSB browsers; built-in help no longer uses it.)
- **Custom `ballview://` URL scheme handler** — intercepted by `helpViewer::setSource` override and dispatched to the `CommandRegistry` (999.46). Lets help docs include "Try this →" links that actually run a command. Example: `ballview://command/file.openPdb`.

**Scope (tutorial migration — Handover §7.4):**
- The existing `demoTutorialDialog.C` content (a sequence of `QLabel`+`QPushButton` slides) converts to **6 Markdown files with frontmatter** under `data/BALLView/help/tutorial/`:
  - `01-welcome.md`, `02-loading.md`, `03-models.md`, `04-selection.md`, `05-coloring.md`, `06-saving.md`.
  - Frontmatter schema: `title:` + `order:` + `prev:` + `next:`.
- Tutorial navigation lives inside the HelpViewer with prev/next chrome.

**Scope (deletions):**
- **Delete `Welcome.rtf`** (`source/APPLICATIONS/BALLVIEW/Welcome.rtf`).
- **Delete `demoTutorialDialog.{C,h,ui}`** (`source/APPLICATIONS/BALLVIEW/demoTutorialDialog.*`).
- Update sources.cmake.

**Acceptance criteria:**
- Fresh launch shows WelcomeScreen.
- Clicking "Open file…" routes to existing `MolecularFileDialog`.
- Sample structures load on click; thumbnails crisp at 2×.
- Markdown tutorial files render in new HelpViewer; in-doc action links work.
- No `QWebEngineView` instantiated in built-in help paths.
- "What's new" card appears after a simulated version bump and disappears on dismiss (auto-dismiss after 2 min).

**Estimated effort:** ~1.5 weeks.

**Plans:** 0.

Plans:
- [ ] TBD (promote with /gsd-plan-phase after 999.46 lands)

### Phase 999.48: BALLView Refresh — Accessibility + `BALL_UI_V2` default flip + flag REMOVAL + legacy file DELETION (Handover Phase 8, scope-reduced) (COMPLETE · v1.7 WAVE 4 · 2026-05-17 · LAST UI PHASE BEFORE 999.49)

**Goal:** Close out the revitalization — harden accessibility, add per-user font scaling, audit tab order, ship the remaining quality-of-life details. **Three distinct in-phase steps:** (a) flip `BALL_UI_V2=ON` as default; (b) per Handover §8.7 REMOVE the flag entirely (delete all `#ifdef BALL_UI_V2` pairs); (c) per Handover §8.8 DELETE the 9 named legacy `.{C,h,ui}` files that the Inspector replaced.

**Status (2026-05-17 COMPLETE):** 13/15 sub-deliverables landed. A11y pass (QAccessible names × 4 widgets + tabOrder_test smoke + ThemeManager::TextScale API + contrast-check.py + tooltip-lint CI gate) + flag DEFAULT FLIP OFF→ON + flag REMOVAL across 76 files (110 #ifdef blocks + 12 #else branches, 265 net lines deleted) + CMakeLists.txt OPTION block + ci.yml ui_v2 matrix axis (~160-line reduction, single cell/OS). **Step (c) DEFERRED to v1.7 RC patch with breadcrumb** in [deferred-items.md](phases/999.48-a11y-flag-flip-removal/deferred-items.md) — displayProperties.C is the lynchpin still on the production rendering path (lines 451+487, createModelProcessor/createColorProcessor factories); requires landing 8 Controller cut-over plans first. 11 commits + 2 docs commits, ~22min wall-clock. 999.49 (Classic dock delete) dispatch unblocked. See [999.48-SUMMARY.md](phases/999.48-a11y-flag-flip-removal/999.48-SUMMARY.md).

**Source:** `/Users/kohlbach/Claude/BALL/Claude Design Handover/revitalization/08-phase-a11y.md` (with §8.6 theme picker DROPPED per Q3; dark-mode finalize sub-section also DROPPED per Q3).

**Depends on:** every other UI Refresh phase (999.40-999.47).

**Scope (a11y core — Handover §8.1, §8.2):**
- **Tab-order audit** through every dialog + main window. `QWidget::setTabOrder` deterministic order: section header (focusable for Space-to-collapse) → body controls L-R T-B → footer actions.
- **`source/VIEW/test/tabOrder_test.C`** — walks the tab chain via `focusNextChild()` and asserts the order matches a per-dialog manifest.
- **`QAccessible` roles on 4 named custom widgets:** `SwatchButton` → `QAccessible::Button` (name "Color: <hex>"); `LabeledSlider` → `QAccessible::Slider` (name from `label`); `SectionHeader` → `QAccessible::Heading` (level 2); `InspectorSection` → `QAccessible::Section` (name from title). (WelcomeScreen cards: `QAccessible::ListItem`.)

**Scope (text-size pref — Handover §8.3):**
- Add to `Preferences › Appearance`: **Radio: Normal (100%) / Large (115%) / Extra Large (130%)**.
- Applies via `qApp->setFont(...)` on the base font. **Persistence schema:** `~/.BALLView` `[Appearance] textScale=` (integer percent or enum).
- Test at 130% that: 3D scene unaffected; Inspector + Project dock fit at min window size (1280×800); Welcome screen reflows.

**Scope (contrast check — Handover §8.4):**
- **`revitalization/scripts/contrast-check.py`** — reads token pairs (foreground/background combinations from `data/BALLView/theme/theme.qss`) and asserts each pair meets **WCAG 2.1 AA: 4.5:1 for body, 3:1 for large text and UI components.**
- Note: Q3-driven single-theme scope means we run the check against the single neutral theme, not theme-light + theme-dark.
- Record any token adjustments in `revitalization/inventory/contrast-changes.md`.

**Scope (tooltip audit + CI lint — Handover §8.5):**
- For every `QAction` + every tool button: `setToolTip(QString("%1 (%2)").arg(text, shortcut.toString(QKeySequence::NativeText)))` + `setStatusTip(<longer one-line hint>)`.
- **CI tooltip-lint script** — walks the source tree counting `addAction(...)` calls without paired `setToolTip` and fails CI if any new ones land.

**Scope (THE FLIP — Handover §8.7, step (a)):**
- **`BALL_UI_V2=ON` becomes the CMake default.** Edit `CMakeLists.txt` so `option(BALL_UI_V2 ... ON)`.
- Release-note item: *"BALLView 1.7 ships with the revitalized UI as default. The legacy UI flag is being removed in this same release."*

**Scope (THE REMOVAL — Handover §8.7, step (b)) — explicitly distinct from the FLIP:**
- **Delete all `#ifdef BALL_UI_V2` / `#endif` pairs** in every source file: replace each with the new-UI branch (delete the else). Sources: every file the 999.40-999.47 phases touched while behind the flag.
- **Delete the `option(BALL_UI_V2 ...)` block in `CMakeLists.txt`.**
- Grep-asserted: `grep -r "BALL_UI_V2" source/ include/ CMakeLists.txt` returns 0 matches (test fixtures excepted).
- Drop the `ui_v2: [ON, OFF]` axis from `.github/workflows/ci.yml` (the dynamically-computed matrix; the OFF path no longer exists).

**Scope (THE DELETION — Handover §8.8) — 9 named legacy files:**
- Delete the following `.{C,h,ui}` triples (and their `sources.cmake` entries):
  1. `displayProperties.{C,h,ui}`
  2. `modelSettingsDialog.{C,h,ui}`
  3. `materialSettings.{C,h,ui}`
  4. `lightSettings.{C,h,ui}`
  5. `stageSettings.{C,h,ui}`
  6. `stereoSettingsDialog.{C,h,ui}`
  7. `clippingDialog.{C,h,ui}`
  8. `coloringSettingsDialog.{C,h,ui}`
  9. `labelDialog.{C,h,ui}` (functionality folded into Inspector LabelSection)
- **Delete `Tools › Legacy Settings` submenu entries + the submenu itself** (the interim home added by 999.44 sub-PR 4.6).
- **Shared controller code extracted in 999.44 STAYS** — only the view layer is deleted.

**Scope (DROPPED per Q3):**
- **§8.6 Theme picker UI EXPLICITLY DROPPED** per Q3 single-neutral-theme. Re-affirming here so a future planner doesn't add it back from the Handover doc.

**Acceptance criteria:**
- WCAG 2.1 AA contrast pass on the single neutral theme.
- Keyboard-only walkthrough: load PDB → change representation → apply coloring → save image — all without mouse.
- All custom widgets expose appropriate `QAccessible` roles (verified via `QAccessibleWidget` introspection in `tabOrder_test.C`).
- Text-size pref scales UI without clipping at 130% on a 1280×800 window.
- **No `BALL_UI_V2` references remain anywhere in the tree** (flag-REMOVAL acceptance).
- Legacy settings dialog files removed; `grep -r "ModelSettingsDialog" source/` matches only the new Inspector section's symbol references.
- All three CI platforms green; CI tooltip-lint passing; `ui_v2` axis dropped from ci.yml.

**Estimated effort:** ~1.5 weeks (was ~1; dark-mode finalize + theme picker drops save time, but the 3-step a/b/c sequence + 9-file deletion + ci.yml axis drop add it back).

**Plans:** 1/1 plans complete.

Plans:
- [x] [999.48-PLAN.md](phases/999.48-a11y-flag-flip-removal/999.48-PLAN.md) → [999.48-SUMMARY.md](phases/999.48-a11y-flag-flip-removal/999.48-SUMMARY.md) — single-plan phase landing 13/15 sub-deliverables (a11y pass + default flip + flag REMOVAL across 76 files + ci.yml ui_v2 axis drop); 9-file legacy DELETION + Tools › Legacy Settings retirement deferred to v1.7 RC patch with breadcrumb (cluster lynchpin displayProperties.C still on production rendering path).

### Phase 999.49: Delete Classic 5-dock workspace (Q2 commitment) (COMPLETE · v1.7 WAVE 4 · 2026-05-17 · LAST WAVE-4 PHASE — v1.7-RC1 UNBLOCKED)

**Goal:** Honor the **Q2 = "Retire Classic after one release"** commitment by deleting the Classic 5-dock workspace artifacts. The picker (added in 999.45) becomes **Default / Focused only**.

**Status (2026-05-17 COMPLETE):** 8/8 sub-deliverables landed in 7 commits across ~30 min wall-clock. WorkspaceManager::Classic enum value deleted (numeric value 1 permanently retired); classic.layout file deleted; Window+View > Workspace menus stripped of Classic action; Phase 999.45 first-run prompt simplified from two-button "Try new / Keep my layout (Classic)" to a one-button informational notice (the legacy 5-dock layout is no longer restorable post-deletion since Mainframe::show() applies Default before the prompt opens); auto-migration of `[Workspace]/currentPreset=Classic` → `Default` on first launch with v1.7 protects upgrade-from-v1.6.x + upgrade-from-v1.7-pre users; RELEASE-NOTES-v1.7 draft populated. 1 Rule-3 hotfix (`test/CMakeLists.txt` SET_TESTS_PROPERTIES QT_QPA_PLATFORM=offscreen for all VIEW tests) absorbed a linux-x64 tabOrder_test display-server failure (xcb-cursor0 missing) surfaced by CI run 26002675865. 2 OOS maintainer commits observed mid-execution (f846562eea — 999.48 BALL_UI_V2 #ifdef scrub; 8485636bc7 — concurrent in-test offscreen-platform fix); both kept, surfaced. CI run 26002905171 on HEAD 7f965a0cf3 = GREEN (macos-arm64 + Phase-9 BALL test suite + lint + coverage all success); linux-x64 + windows-x64 + linux-arm64 NOT re-validated post-hotfix (detect-platforms narrowed to macOS-only — flagged as action item to extend `^CMakeLists\.txt` regex to also match `^test/CMakeLists\.txt` or land a full-matrix-triggering commit at RC1 tag time). **Wave 4 of v1.7 complete; v1.7-RC1 ready to tag** modulo one cross-platform CI validation pass + pre-existing 999.48 deferred-items (9-file legacy DELETION) + v1.6.2 pre-tag regression follow-ups (not v1.7-blocking). See [999.49-SUMMARY.md](phases/999.49-classic-dock-delete/999.49-SUMMARY.md).

**Reference:** [SEED-001](seeds/SEED-001-ballview-refresh-ui-milestone.md) + [MAINTAINER-QUESTIONS-999.1.md Q2](MAINTAINER-QUESTIONS-999.1.md).

**Trigger:** 999.48 lands (flag REMOVAL complete; legacy files DELETED; one stable release cycle of the new default workspace has passed in the v1.7 RC stream).

**Depends on:** 999.48 (legacy file deletion + flag removal — the Classic-delete is the natural follow-up).

**Scope:**
- **Delete the `Classic` preset** from `WorkspaceManager` (`source/VIEW/KERNEL/workspaceManager.{h,C}`) — remove the enum value, the `apply(Classic)` branch, and the `classic.layout` enum-to-file mapping.
- **Delete the `.layout` file**: `data/BALLView/workspaces/classic.layout`.
- **Delete the first-run migration prompt** (added in 999.45 §5.4) — the "Try new workspace" vs "Keep my layout" prompt's "Keep my layout" path preserved the 5-dock layout as "User Default". After 999.49, users who chose "Keep my layout" still have their layout preserved as a `UserDefined` preset (no data loss) — just the named "Classic" preset goes away.
- **Picker becomes Default / Focused only** (plus any `UserDefined` presets the user has saved). Status-bar workspace label updates accordingly.
- Update `View › Workspace › Classic` menu entry → remove.
- Release-note item: *"The Classic 5-dock workspace preset has been removed in v1.7.x. User-saved layouts and the Default/Focused presets are unaffected."*

**Acceptance criteria:**
- `grep -rn "Classic\b" source/VIEW/KERNEL/workspaceManager.*` returns 0 matches.
- `data/BALLView/workspaces/classic.layout` does not exist.
- `View › Workspace` menu lists Default + Focused + any UserDefined presets, no Classic.
- Users who previously had Classic active fall back to Default (or to a `UserDefined` preset if they had saved one).
- No regression on Default / Focused / UserDefined preset application.

**Estimated effort:** ~3 days.

**Plans:** 1.

Plans:
- [x] [999.49-PLAN.md](phases/999.49-classic-dock-delete/999.49-PLAN.md) → [999.49-SUMMARY.md](phases/999.49-classic-dock-delete/999.49-SUMMARY.md) — single-plan phase landing 8/8 sub-deliverables (enum + apply() branch + .layout file + 2 menu wirings + first-run prompt + auto-migration + RELEASE-NOTES) + 1 Rule-3 hotfix for VIEW test offscreen-platform. 7 commits + ~30 min wall-clock. Migration-before-deletion ordering preserves users transitioning through v1.6.x → v1.7-RC1 from carrying an unrecognized preset name in their INI.

### Phase 999.50: v1.7.2 build acceleration — Ward serializer de-boost (eliminate boost::serialization · build-accel cluster continuation)

**Goal:** Drop `boost::serialization` (the single heaviest Boost component to compile) from the PoseClustering Ward-tree serializer. v1.7.1 already *quarantined* the cost into its own TU (`poseClusteringSerialization.C`, commit `5e10b27797`) so parallelism + ccache isolate it; v1.7.2 goes for *elimination* — a hand-rolled compact reader/writer drops that TU's compile from tens of minutes to seconds. `poseClustering.C` alone dominated the Windows release build (~57 min, per the 999.19 per-TU profiling data), and boost archives are the bulk of it.

**Canonical plan:** [`v1.7.2-BUILD-ACCEL-PLAN.md`](v1.7.2-BUILD-ACCEL-PLAN.md) — the maintainer-authored spec for this phase (lead item ①, release.yml ccache ②, MSVC de-opt fallback ③). Downstream planner + executor MUST treat it as the source of truth.

**Scope:**
1. **① LEAD — hand-roll a compact cluster-tree serializer (BUILD-ACCEL-06).** In `source/DOCKING/COMMON/poseClusteringSerialization.C`, replace `boost::archive::{binary,text}_{i,o}archive` + `adj_list_serialize.hpp` + `ClusterProperties::serialize` with a hand-rolled reader/writer. Keep the public API (`serializeWardClusterTree(std::ostream&, bool binary)` / `deserializeWardClusterTree(std::istream&, bool binary)`) and the `bool binary` flag (selects binary vs text variant of the NEW format). Serialize exactly today's payload: graph order + root vertex index (`cluster_tree_[boost::graph_bundle]`), per-vertex `std::set<Index> poses` / `Size size` / `float merged_at`, and the directed edge list (parent→child); rebuild via `add_vertex`/`add_edge` then set the graph-bundle root. Prepend a magic + format-version header (e.g. `"BALLWARD\x01"`) so a wrong/old file fails loudly.
2. **Format-break follow-through.** Old boost-serialized `.ward`/`.dat` trees will NOT load (acceptable — niche docking research format). Regenerate the committed fixture `test/data/PoseClustering_wardtree.dat` deterministically from existing test inputs in the new format so `PoseClustering_test3.C:360/366` read-back assertions (`extractClustersForThreshold(0.5)` → 6 clusters) still hold. Keep callers working: `source/APPLICATIONS/TOOLS/DockPoseClustering.C` (writer) + `source/APPLICATIONS/TOOLS/ExtractClustersFromWardTree.C:94` (reader). Note the break in RELEASE-NOTES v1.7.2 "Breaking changes".
3. **② release.yml ccache reuse (BUILD-ACCEL-07).** `release.yml` installs ccache but never restores/saves a ccache build cache (only the vcpkg cache) — every tag builds cold. Add `actions/cache` for the ccache dir (key aligned with `ci.yml` where possible) so a release tag of an already-CI-built commit hits warm cache.
4. **③ (fallback) MSVC `/O1` de-opt.** Only if anything heavy remains after ①: `set_source_files_properties(... COMPILE_OPTIONS "/O1")` for the specific slow TU on MSVC. Largely moot once ① lands.

**Sequencing:** ① first (the real Windows-build-time killer), then ②. Land at the very start of v1.7.2.

**Validation:** `PoseClustering_test3` (round-trip 290–449 + fixture read 359) and `PoseClustering_test1` (graphviz) pass; `poseClusteringSerialization.C` compiles in seconds; no `boost/archive` or `adj_list_serialize` includes remain in the tree. Tri-OS CI green.

**Requirements:** `BUILD-ACCEL-06`, `BUILD-ACCEL-07` — see [`REQUIREMENTS.md`](REQUIREMENTS.md) build-accel cluster.

**Estimated effort:** ~40–80 LOC + fixture regen + 1 CI yaml change. Low-medium. Risk: format break (handled via regen + magic header).

**Plans:** 2/2 plans complete

Plans:
- [x] 999.50-01-PLAN.md — BUILD-ACCEL-06: hand-rolled boost-free Ward-tree serializer (magic+version header) + new-format fixture regen + RELEASE-NOTES break note; callers + PoseClustering_test3/test1 green
- [x] 999.50-02-PLAN.md — BUILD-ACCEL-07: release.yml ccache restore/save (key aligned with ci.yml) + optional disabled MSVC /O1 fallback (item ③)

### Phase 999.51: Inspector controller cut-overs — Clipping + Label (TARGETED FOR v1.7.x)

**Goal:** Wire the two latent Inspector controllers that ship reachable-via-API but UI-dead in v1.7.x. Add a `ClippingSection` and a `LabelSection` to `inspectorView.C`'s Representation-tab section list (mirroring the shipped `Model`/`Coloring`/`Material` section pattern), and cut `ClippingController::apply()` + `LabelController::apply()` over from log-only stubs to functional apply — reusing the existing legacy Clipping/Label dialogs' parameter parsing (the `ModelController` → `DisplayProperties::getModelSettingsDialog()` pattern). Remove the `STUB` log markers + deferral comments.

**Canonical source:** [`v1.7.x-PATCH-QUEUE.md`](v1.7.x-PATCH-QUEUE.md) items v1.7.x-01, v1.7.x-02.
**Scope:** `clippingController.{h,C}`, `labelController.{h,C}`, `inspectorView.C`, new `sections/clippingSection.{h,C}` + `sections/labelSection.{h,C}`. **Important:** these REUSE the legacy Clipping/Label dialogs — those dialogs must NOT be deleted by Phase 999.53 until this lands.
**Requirements:** `VIEW-INSP-01`, `VIEW-INSP-02`.
**Depends on:** none. **Effort:** ~200 LOC per controller.
**Plans:** 2/2 plans complete
- [~] 999.51-01-PLAN.md — Clipping cut-over: ClippingSection + functional ClippingController::apply() (VIEW-INSP-01). Code+build COMPLETE (3 task commits, VIEW build green BALL_UI_V2=ON); PAUSED at blocking GUI human-verify (Task 4 — confirm section renders + visibly clips in running BALLView). Legacy ClippingDialog untouched.
- [~] 999.51-02-PLAN.md — Label cut-over: LabelSection + functional LabelController::apply() (VIEW-INSP-02). Code+build COMPLETE (3 task commits `3d2102a5b2`/`8ad9fc6076`/`590d7d6b9e`, VIEW build green BALL_UI_V2=ON); PAUSED at blocking GUI human-verify (Task 4 — confirm section renders + a label visibly appears in running BALLView). Legacy LabelDialog (Display > Create > Label) untouched.

### Phase 999.52: Inspector material transparency control (#527) (TARGETED FOR v1.7.x)

**Goal:** Close GitHub [#527](https://github.com/BALL-Project/ball/issues/527). The Refresh's Inspector `materialSection` ships ambient/diffuse/specular/shininess sliders but **no transparency control**, while the legacy `MaterialSettings` slider (the one #527 reports broken) is still compiled and reachable. Add a working transparency control to the Inspector `materialSection` so the broken legacy path is superseded (and can later be deleted in 999.53).

**Canonical source:** [`v1.7.x-PATCH-QUEUE.md`](v1.7.x-PATCH-QUEUE.md) item v1.7.x-30; GitHub #527.
**Scope:** `source/VIEW/WIDGETS/inspector/sections/materialSection.{h,C}` (+ the material apply path).
**Requirements:** `VIEW-INSP-03`.
**Depends on:** none (additive to materialSection). **Effort:** small–medium.
**Plans:** 1/1 plans complete

Plans:
- [~] 999.52-01-PLAN.md — Add a working 0-255 Transparency control to the Inspector materialSection, driving **Representation::setTransparency() + Representation::update()** (the per-vertex alpha path the interactive GLRenderer actually honors — NOT the dead Stage::Material.transparency field, which was the #527 bug), sharing Representation transparency state with the existing Model-section slider. Legacy dialog untouched. **Code + build complete (2 task commits `ce4061a93d`, `7374d42ae8`; libVIEW + BALLView.app green under BALL_UI_V2=ON); PAUSED at the BLOCKING GUI human-verify (Task 3) — drag the Material-tab slider, confirm visible transparency + Model-section cross-check + reset.** VIEW-INSP-03 implementation-complete / awaiting GUI verify; closes #527 once confirmed.

### Phase 999.53: Legacy dialog deletion (Wave-4 cleanup tail) (TARGETED FOR v1.7.x)

**Goal:** Delete the 9 legacy VIEW dialog files whose Inspector replacements have shipped (deferred from Wave 4 Phase 999.48 §10, breadcrumb commit `7f56f80d89`), one atomic commit per file, keeping the tri-OS CI matrix green between deletions.

**Canonical source:** [`v1.7.x-PATCH-QUEUE.md`](v1.7.x-PATCH-QUEUE.md) item v1.7.x-08.
**Scope:** the 9-file list from commit `7f56f80d89` — but each file must be confirmed UNREFERENCED before deletion. **Critical reconciliation:** Phases 999.51 (Clipping/Label cut-overs reuse the legacy Clipping/Label dialogs) and 999.52 (transparency supersedes the legacy Material dialog) change what is still in use, so this phase must run AFTER them and re-verify the reuse state per file.
**Requirements:** `VIEW-CLEAN-01`.
**Depends on:** Phase 999.51, Phase 999.52. **Effort:** small (deletions + CI validation). **Plan just-in-time** after 51/52 land so the reuse audit is accurate.
**Plans:** 0.

### Phase 999.54: BALLView startup warning when no renderer plugins found (#501) (TARGETED FOR v1.7.x)

**Goal:** Close GitHub [#501](https://github.com/BALL-Project/ball/issues/501). Today BALLView gives no feedback when renderer-plugin discovery returns empty. Add a startup check that emits a `Log`/status-bar warning (and ideally surfaces via the onboarding/notice path) when zero renderer plugins are discovered. Aligns with the Refresh's first-run/accessibility passes.

**Canonical source:** [`v1.7.x-PATCH-QUEUE.md`](v1.7.x-PATCH-QUEUE.md) item v1.7.x-31; GitHub #501.
**Requirements:** `VIEW-UX-01`.
**Depends on:** none. **Effort:** small (~20–40 LOC).
**Plans:** 1/1 plans complete
- [x] 999.54-01-PLAN.md — non-fatal zero-renderer startup guard (Log.warn + status bar) closing #501.

### Phase 999.55: Build acceleration — precompiled headers across BALL + VIEW (TARGETED FOR v1.7.x)

**Goal:** Extend PCH to the test-suite build — the remaining live half of patch item v1.7.x-09. The **library-target** halves (`target_precompile_headers(BALL PRIVATE …)` + `target_precompile_headers(VIEW PRIVATE …)`, the ccache `sloppiness` contract, and `cmake/PCH.md`) already shipped in Phase 999.16 (BUILD-ACCEL-01: Windows −35.6% cold, Linux −15% warm, macOS excluded for AppleClang). Because that PCH is `PRIVATE`, the 298 `*_test.C` executables do not inherit it; 999.55 wires `target_precompile_headers(<test> REUSE_FROM BALL)` / `REUSE_FROM VIEW` in `test/CMakeLists.txt` (PIC-matched to the SHARED-lib PCH, AppleClang-guarded), reusing the 999.16 audited header sets verbatim — measurably reducing the per-TU header-parse cost on the test build (target ≥15%; PCH does not help the template-metaprogramming TUs, which v1.7.2/999.50 already handled). The "drop debug builds" half of v1.7.x-09 proved a no-op (already Release).

**Canonical source:** [`v1.7.x-PATCH-QUEUE.md`](v1.7.x-PATCH-QUEUE.md) item v1.7.x-09 (PCH half); original [Phase 999.16](#) scope.
**Requirements:** `BUILD-ACCEL-08`.
**Depends on:** none. **Effort:** ~1 day. Pure build-config; per-TU profiling data already exists (999.19).
**Plans:** 1/1 plans complete

### Phase 999.56: Refresh the PDF tutorial for the modernized stack (#560) (TARGETED FOR v1.7.x)

**Goal:** Close GitHub [#560](https://github.com/BALL-Project/ball/issues/560). The PDF tutorial is outdated on build system, screenshots, URLs, and developer list — exactly what the v1.6/v1.7 modernization changed. Refresh content + screenshots against v1.7.x BALLView; update build instructions to the documented Homebrew/vcpkg flow and the GitHub Releases download path. Docs deliverable.

**Canonical source:** [`v1.7.x-PATCH-QUEUE.md`](v1.7.x-PATCH-QUEUE.md) item v1.7.x-32; GitHub #560.
**Requirements:** `DOCS-01`.
**Depends on:** none (best after the UI phases so screenshots reflect final state). **Effort:** medium–large (content, not code).
**Plans:** 0.

### Phase 999.57: Complete Inspector controller cut-over — unwire displayProperties.C (TARGETED FOR v1.7.x · PREREQUISITE FOR 999.53)

**Goal:** Detach the production rendering pipeline from the legacy settings dialogs so Phase 999.53 can delete the 9-file cluster. The lynchpin is `source/VIEW/DIALOGS/displayProperties.C`: line 451 (`rep.setModelProcessor(model_settings_->createModelProcessor(...))`) and line 487 (`rep.setColorProcessor(coloring_settings_->createColorProcessor(...))`) are still on the active render path, and `modelController.C` + `coloringController.C` still call the legacy factories. Relocate the `createModelProcessor` / `createColorProcessor` factory logic out of `modelSettingsDialog` / `coloringSettingsDialog` into the Inspector Model/Coloring sections via the Controller layer (mirror the StageController cut-over recipe from 999.44-06), and resolve the remaining `displayProperties` / `MaterialSettings` / `LightSettings` / `StageSettings` references (`scene.C:141/142/161`, `mainControl.C`, `representationManager.C:336`, `molecularControl.C`, `geometricControl.C`, `mainframe.C`, `pluginDialog.C`, `demoTutorialDialog.C`) — adding Stage/Light Inspector coverage where the sections don't yet exist.

**Canonical source:** [`.planning/phases/999.48-a11y-flag-flip-removal/deferred-items.md`](phases/999.48-a11y-flag-flip-removal/deferred-items.md) (per-file dependency table + recommended sequence + acceptance criteria); the 999.44 cut-over plans/summaries (the deferred 04a..04h work + the StageController cut-over exemplar).
**Requirements:** `VIEW-CLEAN-02`.
**Risk:** HIGHEST — touches the threaded rendering pipeline (CLAUDE.md). Must be incremental + verifiable against a running GUI; a prior naive deletion broke the build (commit `7f56f80d89`). Likely multi-plan with GUI UAT per domain (Model/Coloring/Stage/Light render end-to-end).
**Depends on:** none (builds on 999.51/52 Inspector sections). **Blocks:** 999.53.
**Plans:** 3/3 plans complete

Plans:
- [x] 999.57-01-PLAN.md — Relocate `createModelProcessor` → headless `ModelProcessorFactory` (param struct); delegate the legacy dialog to it (Wave 1)
- [x] 999.57-02-PLAN.md — Relocate `createColorProcessor` → headless `ColorProcessorFactory` (overrides struct, compiled-defaults invariant); delegate the legacy dialog (Wave 1)
- [~] 999.57-03-PLAN.md — Repoint `displayProperties.C:451/487` + `modelController`/`coloringController` onto the factories; blocking running-BALLView render UAT (default-rep + Model + Coloring sections); CONSUMER-AUDIT classifying every remaining reference for 999.53 (Wave 2). **Code+build+audit DONE** (commits 2b87c689ea, 29d661fa8f, a44eef0ed6; BALLView.app links clean; CONSUMER-AUDIT.md landed; render path grep-proven detached) — **BLOCKING render UAT PENDING** (default-rep + Inspector Model/Coloring must be GUI-verified on a running BALLView before this plan + VIEW-CLEAN-02 close).

> **SCOPE SPLIT (planner, 2026-05-21):** This phase delivers Stream A only — relocating the two render-path factories (`createModelProcessor`/`createColorProcessor`) off `displayProperties.C`, which unblocks 999.53's deletion of the **modelSettingsDialog + coloringSettingsDialog + displayProperties** triples. The `scene.C` `MaterialSettings`/`LightSettings`/`StageSettings` constructions (orchestrator Stream B) are NOT dead code — they are load-bearing for the Preferences-dialog stack (`scene.C:1104-1130`), `applyPreferences()` (`1142-1178`), and the entire **stereo rendering path** (screen geometry / renderer types / stereo modes at `scene.C:2664-3165`). Relocating those out of the dialogs (into Inspector Stage/Light coverage + a Scene-owned stereo-screen config) is an independent high-risk threaded-renderer migration comparable in size to this whole phase, with its own GUI UAT (lights / materials / stereo must still drive the scene). Recommended as a sibling phase **999.58**, gated BEFORE 999.53 deletes the 3 scene.C dialog triples. The orchestrator's `representationManager.C:336` / `molecularControl.C:446,460` references are **false positives** (`modelMuteableByDisplayProperties`/`coloringMuteableByDisplayProperties` are `ModelInformation` methods, a name collision — not the dialog). See `999.57-03-PLAN.md` Task 4 / CONSUMER-AUDIT.md.

### Phase 999.58: scene.C settings-dialog cut-over — Material / Light / Stage incl. the stereo render path (TARGETED FOR v1.7.x · 2nd PREREQUISITE FOR 999.53)

**Goal:** Detach `source/VIEW/WIDGETS/scene.C` from the legacy `MaterialSettings` / `LightSettings` / `StageSettings` dialogs (constructed at `scene.C:141/142/161`) so Phase 999.53 can delete those three dialog triples. Per the 999.57 planner's grounding, these are NOT dead — they are load-bearing across ~15 sites: the Preferences-dialog stack (`scene.C:1104-1130`), `applyPreferences()` (`scene.C:1142-1178`), and the **entire stereo rendering path** (screen geometry / renderer types / stereo modes at `scene.C:2664-3165`). Relocate that configuration out of the dialogs into Inspector Stage/Light/Material coverage + a Scene-owned stereo-screen config, so lights, materials, and stereo still drive the rendered scene without the dialogs.

**Canonical source:** [`.planning/phases/999.48-a11y-flag-flip-removal/deferred-items.md`](phases/999.48-a11y-flag-flip-removal/deferred-items.md); the 999.57 SCOPE-SPLIT note + `999.57-CONSUMER-AUDIT.md`.
**Requirements:** `VIEW-CLEAN-03`.
**Risk:** HIGHEST — independent threaded-renderer migration comparable in size to 999.57, touching the stereo path. Incremental + verifiable against a running GUI (lights/materials/stereo render end-to-end); own GUI UAT.
**Depends on:** 999.57 (builds on the detached render path). **Blocks:** 999.53.
**Plans:** 3/3 plans complete
Plans:
- [~] 999.58-01-PLAN.md — Material + Light config relocation onto the Inspector MaterialController/LightController; route applyPreferences material+light + the light-refresh sites off the dialogs (wave 1). **Code+build COMPLETE 2026-05-22** (MaterialController owns the full default-material field set incl. the 3 colors + `applyDefaultMaterial()`; LightController owns the full per-light list + `syncFromStage()`; scene.C `applyPreferences` + all 5 light-refresh sites routed through the controllers; no live MaterialSettings/LightSettings reach-throughs remain on Scene's render path; BALLView.app links clean BALL_UI_V2=ON). **BLOCKING running-BALLView render UAT PENDING** (lights + materials must still visibly drive the scene) — VIEW-CLEAN-03 stays incomplete (phase-spanning).
- [~] 999.58-02-PLAN.md — Full non-stereo Stage render config relocation onto StageController (vertex buffers, downsampling, projection, FPS, preview, offscreen, renderer switch, etc.); route applyPreferences stage branch + getGLSettings off stage_settings_ (wave 2). **Code+build COMPLETE 2026-05-22** (StageController grew the full non-stereo render-config field set — projection / show-lights / animation-smoothness / offscreen+factor / capping / FPS / preview / vertex-buffers incl. rep-delete side effect / smooth-lines / downsampling / renderer-switch / mouse+wheel-sensitivity — applied through the same Scene/Stage/Camera/GLRenderer/ClippingPlane backend behind the re-entrancy shield + a new MainControl::isBusy() defer-guard; scene.C `applyPreferences` routed through a transient StageController; non-load-bearing `getGLSettings()` init readback removed; swap-side-by-side left to StereoController; BALLView.app links clean BALL_UI_V2=ON). **BLOCKING running-BALLView render UAT PENDING** (stage controls must still visibly drive the scene; no regression to background/fog/eye/focal) — VIEW-CLEAN-03 stays incomplete (phase-spanning).
- [~] 999.58-03-PLAN.md — Remove the 3 dialog constructions (scene.C:142/143/162) + Preferences-stack rewire; Scene-owned stereo-screen config for the deferred stereo bodies; in-Scene applyStereoDefaults; demoTutorial repoint; grep proof scene.C is dialog-free (wave 3). **Code+build COMPLETE 2026-05-22** (scene.C constructs/registers none of LightSettings/MaterialSettings/StageSettings; members/forward-decls/getStageSettings()/getMaterialSettings() accessors removed from scene.h; the four guard-and-deferred stereo bodies read a Scene-owned `StereoScreenConfig` struct instead of `stage_settings_->get*` — guard-and-return preserved, stereo NOT re-enabled; `applyStereoDefaults` inlines the geometry math in-Scene; demoTutorialDialog repointed off all 6 dialog-accessor reach-throughs with the dormant RTfact Stage-setup logic preserved; unused dialog includes dropped from both consumers; BALLView.app links clean BALL_UI_V2=ON). VIEW-CLEAN-03 construction grep proof PASSES — **999.53 unblocked to delete the 3 dialog triples**. **BLOCKING running-BALLView render UAT PENDING** (materials/lights/stage must still visibly drive the scene; stereo menu must defer cleanly without crash) — VIEW-CLEAN-03 stays incomplete (phase-spanning).

> **Deferred / blocked v1.7.x items (NOT promoted to phases):** v1.7.x-03 (Stereo checkbox) is **blocked** on the SEED-001 step-5 Renderer/RenderSurface stereo-mode boundary; v1.7.x-04 (Geometric/Dataset highlight) and v1.7.x-05 (macOS QSS overlay) are **dormant** pending a user re-file / concrete trigger. They stay in [`v1.7.x-PATCH-QUEUE.md`](v1.7.x-PATCH-QUEUE.md) with their gating conditions. v1.7.x-34 (www.ball-project.org outage) is external/Tübingen-side infra, not a code phase.

---

## v1.7.4 milestone — Inspector depth + the contract

> **Ingested from the v1.7.4 design handover (2026-05-22).** Six GSD-shaped phases in two waves. **Sequences AFTER v1.7.2 + v1.7.3** — v1.7.4 is cut from `v1.7-modernization` post-`v1.7.3` tag. None complete.
>
> **Renumber provenance.** The handover numbered these `999.58–999.63`, colliding with the in-flight `999.58-scene-settings-stereo-cutover`. The entire set was renumbered **+1** at ingest → `999.59–999.64` (contiguous + ordered). The two new patch-queue IDs the handover minted (`v1.7.x-30/-31`) collided with this repo's existing -30 (#527) / -31 (#501), so they were re-minted to **v1.7.x-35 / v1.7.x-36**.
>
> **Wave A · the contract (architecture, gating):** 999.59 (apply() cut-over), 999.60 (contract tests), 999.61 (Qt-lifetime), 999.62 (opaque-paint). **Wave B · Inspector depth (visible):** 999.63 (value-range histogram), 999.64 (per-section reset). Wave A gates Wave B (999.63/999.64 consume the command-shaped `apply()` from 999.59).
>
> **Supporting docs (ingested into `.planning/`):** [`v1.7.4-ARCHITECTURE-CONTRACT.md`](v1.7.4-ARCHITECTURE-CONTRACT.md) (the gating contract — read end-to-end), [`v1.7.4-ACCEPTANCE.md`](v1.7.4-ACCEPTANCE.md) (cross-phase release gates), [`RELEASE-NOTES-v1.7.4.md`](RELEASE-NOTES-v1.7.4.md) (draft). REQs: `ARCH-CONTRACT-01..04`, `TEST-CONTRACT-01`, `LIFETIME-DOC-01`, `OPAQUE-PAINT-01`, `INSP-COLORING-VALUE-RANGE-01`, `INSP-RESET-01` (see [`REQUIREMENTS.md`](REQUIREMENTS.md) §v1.7.4).

### Phase 999.59: Controller cut-over · command-shaped `apply()` (v1.7.4 · Wave A · GATING)

**Goal:** Land the command-shaped `Controller::apply()` contract on every mutating Controller in the VIEW layer — the §2 shape from the architecture contract (preconditions → re-entrancy guard → capture-for-reversibility → mutate the single owner → emit ONE typed event → soft render-invalidate → record reversible intent). Ship the `ApplyGuard` RAII + `ApplyPayload` base types, migrate the 3 already-mutating Controllers (Model/Coloring/Material), cut over the 5 read-only mirrors (Camera/Light/Stereo + Clipping/Label tails), and narrow `Stage`/`Representation` setters to Controller-`friend` scope so external mutation does not compile. The gating phase — 999.60/999.63/999.64 all consume the contract.

**Plan doc:** [`phases/999.59-controller-cutover-command-shape/999.59-01-PLAN.md`](phases/999.59-controller-cutover-command-shape/999.59-01-PLAN.md) (carries the 5-plan internal breakdown 999.59-01..05; the planner/executor may decompose). **Architecture contract:** [`v1.7.4-ARCHITECTURE-CONTRACT.md`](v1.7.4-ARCHITECTURE-CONTRACT.md) §2 / §13.
**v1.7.x mapping:** v1.7.x-24 (FOUNDATIONAL apply() cut-over). **Wave:** A (gating). **Requirements:** `ARCH-CONTRACT-01`.
**Depends on:** 999.57 (the displayProperties cut-over the command-shape builds on). **Blocks:** 999.60, 999.63, 999.64. **Status:** not started.

### Phase 999.60: Contract tests · cross-surface parity (v1.7.4 · Wave A)

**Goal:** Stand up the contract tests that enforce the apply() contract by behavior, not grep. A `ContractTestHarness` boots BALLView headlessly and runs the same operation through all four surfaces (Inspector / toolbar / menu / Python), asserting bit-equal owner postconditions; per-Controller parity tests for the 8 mutating Controllers; the four-way selection-consumer agreement test (tree / scene picker / mainControl / action enable-gate); a re-entrancy test (second apply during own emission is dropped, not stacked); and owner-narrowing compile-fail tests. New blocking Linux-only `contract-tests` CI job.

**Plan doc:** [`phases/999.60-contract-tests/999.60-01-PLAN.md`](phases/999.60-contract-tests/999.60-01-PLAN.md). **Architecture contract:** [`v1.7.4-ARCHITECTURE-CONTRACT.md`](v1.7.4-ARCHITECTURE-CONTRACT.md) §11 / §7.
**v1.7.x mapping:** v1.7.x-25 (contract tests — the real enforcement). **Wave:** A. **Requirements:** `ARCH-CONTRACT-02`, `TEST-CONTRACT-01`.
**Depends on:** 999.59 (tests need a Controller that already carries the contract). **Status:** not started.

### Phase 999.61: Qt-lifetime audit + conventions doc (v1.7.4 · Wave A)

**Goal:** Codify the QObject-lifetime conventions that emerged from the UFG-30/32/18/26 cycle into a canonical `.planning/codebase/QT-LIFETIME.md` (5 rules + 4 case studies + cookbook), audit the 6 known leak-class sites, adopt `QPointer<T>` for cross-owner references (~12 sites), and ship a small blocking `lifetime-discipline` CI lint that rejects `deleteLater()` inside QObject destructors. Write-once discipline artifact; no behavior change intended.

**Plan doc:** [`phases/999.61-qt-lifetime-audit/999.61-01-PLAN.md`](phases/999.61-qt-lifetime-audit/999.61-01-PLAN.md). **Architecture contract:** [`v1.7.4-ARCHITECTURE-CONTRACT.md`](v1.7.4-ARCHITECTURE-CONTRACT.md) §9.
**v1.7.x mapping:** v1.7.x-26 (Qt-lifetime ownership audit + conventions). **Wave:** A. **Requirements:** `ARCH-CONTRACT-03`, `LIFETIME-DOC-01`.
**Depends on:** none (independent audit; runs in parallel with 999.60 once 999.59 clears the Controller dtors). **Status:** not started.

### Phase 999.62: Opaque-paint contract codification (v1.7.4 · Wave A)

**Goal:** Convert the UFG-05/09/10 opaque-paint lessons from per-site fixes into a structural contract: every container that hosts an animated child (a `QPropertyAnimation` target) carries `WA_OpaquePaintEvent` + `setAutoFillBackground(true)` within 3 ancestor hops, factored into the `InspectorSection` base + a shared mixin; a centralized theme token in `theme-neutral.qss`; a new design-system doc `data/BALLView/theme/THEME.md`; and a CI `opaque-paint-lint` job (STATUS → BLOCKING after a clean week). Prerequisite for dark mode (v1.8).

**Plan doc:** [`phases/999.62-opaque-paint-contract/999.62-01-PLAN.md`](phases/999.62-opaque-paint-contract/999.62-01-PLAN.md). **Architecture contract:** [`v1.7.4-ARCHITECTURE-CONTRACT.md`](v1.7.4-ARCHITECTURE-CONTRACT.md) §10.
**v1.7.x mapping:** v1.7.x-35 (new — handover minted -30, re-minted at ingest because -30 was taken by #527/Phase 999.52). **Wave:** A. **Requirements:** `ARCH-CONTRACT-04`, `OPAQUE-PAINT-01`.
**Depends on:** 999.43 (shared widgets, already shipped). **Status:** not started.

### Phase 999.63: Coloring · value-range histogram (v1.7.4 · Wave B)

**Goal:** The first visible Inspector-depth win on the v1.7.4 contract. Replace the basic min/max text inputs that shipped in v1.7.x-17 with a `ValueRangeWidget` rendering a histogram of the active selection's scalar value (B-factor / Occupancy / Charge / Distance / Custom) for value-based coloring methods, two draggable handles (live preview during drag, commit on release), and four presets (Auto-fit / Full range / Robust 5–95% / Reset). The Controller owns the binning via a cached `ColoringController::distribution()` read-model; the widget never walks the Composite tree. Commit goes through `ColoringController::setRange(min,max)` → the command-shaped `apply()`. Perf: 50k-atom selection re-bin <50 ms.

**Plan doc:** [`phases/999.63-coloring-value-range-histogram/999.63-01-PLAN.md`](phases/999.63-coloring-value-range-histogram/999.63-01-PLAN.md). **Architecture contract:** [`v1.7.4-ARCHITECTURE-CONTRACT.md`](v1.7.4-ARCHITECTURE-CONTRACT.md) §5 (view = pure presentation). Visual target: `revitalization/mockups.html` board 05.
**v1.7.x mapping:** v1.7.x-17b (upgrade of the v1.7.x-17 value-range slice). **Wave:** B. **Requirements:** `INSP-COLORING-VALUE-RANGE-01`.
**Depends on:** 999.59 (ColoringController carrying the apply() contract) + 999.60 tests. **Status:** not started.

### Phase 999.64: Per-section "Reset section" affordance (v1.7.4 · Wave B)

**Goal:** Add one section-level Reset affordance to every Inspector section header (a Reset glyph on hover, tooltip "Reset section to defaults"). Clicking invokes the bound Controller's `reset()`, which captures-current → invokes method-defined defaults → emits exactly ONE event through the §2 contract (one `ApplyPayload`, undoable in v2.0). Every Controller implements `reset()` (read-only sections as a documented no-op). Codifies a missing affordance across all 14 sections.

**Plan doc:** [`phases/999.64-per-section-reset/999.64-01-PLAN.md`](phases/999.64-per-section-reset/999.64-01-PLAN.md). **Architecture contract:** [`v1.7.4-ARCHITECTURE-CONTRACT.md`](v1.7.4-ARCHITECTURE-CONTRACT.md) §2.
**v1.7.x mapping:** v1.7.x-36 (new — handover minted -31, re-minted at ingest because -31 was taken by #501/Phase 999.54). **Wave:** B. **Requirements:** `INSP-RESET-01`.
**Depends on:** 999.59 (`Controller::reset()` in the base). **Status:** not started.

---
*Roadmap created: 2026-05-14*
*Mirrors `/Users/kohlbach/Claude/BALL/ROADMAP-1.6.md` (phases 1, 2, 3, 4a, 4b, 5, 6, 7, 8). Revised 2026-05-14 after Codex adversarial review — cheap fixes applied; structural changes (early CI phase, Phase 5 split, diagnostics requirement, feature matrix) pending a deliberate roadmap revision.*
*Revised 2026-05-14 (consolidation): backlog 999.4 promoted to active Phase 4.1 (Config Color-Defaults Fix, CONFIG-01); former standalone Phase 05.1 (renderer backend spike) folded into Phase 5 since it must prototype against Qt 6.*
*Revised 2026-05-17 (BALLView Refresh promotion): SEED-001 promoted-subset → 9 new phases 999.40-999.48 mapping Handover Phases 0-8; v1.7 takes the foundation + maintainer-independent subset (999.40 + 999.41 + conditional 999.42/.43), v1.8 takes the architecturally-heavier phases (999.44 Inspector, 999.45 Workspace, 999.46 Menus, 999.47 Onboarding, 999.48 a11y) as a dedicated UI marketing release with BALL_UI_V2=ON default flip. Phase 999.1 maintainer questions status: publish-ready, now gates concrete v1.7 + v1.8 work.*
*Revised 2026-05-17 (BALLView Refresh audit pull-in): per user direction "keep everything in 1.7" — phases 999.44-999.48 re-pinned from v1.8 → v1.7 Wave 4; new Phase 999.49 (Classic-delete, Q2 commitment) appended; SEED-001 status promoted-subset → promoted-full; v1.7 final ships with BALL_UI_V2=ON default; 999.42 absorbs 999.41 carry-overs (ThemedIconEngine state-tints, call-site rewrites, XPM array deletion, .icns/.ico regen); 999.48 distinguishes flag-default-FLIP from flag-REMOVAL and absorbs Handover §8.8 9-file DELETION; Q4 translation round re-pinned from v1.8 → v1.7 cycle. v1.8 milestone scope wholly TBD as of this audit.*
