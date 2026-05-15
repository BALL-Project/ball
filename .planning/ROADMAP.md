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
- [ ] **Phase 4.1: Config Color-Defaults Fix** - Stop persisted `~/.BALLView` config from silently shadowing compiled element/residue color defaults *(promoted from backlog 999.4 — real user-facing bug)*
- [x] **Phase 5: Qt 6 Migration + Renderer Backend Spike** - Build against Qt 6 and replace deprecated VIEW APIs (keep the compat-profile GL path working), then a time-boxed renderer-backend decision spike behind the Phase 02.1 boundary *(former Phase 05.1 folded in — it must prototype against Qt 6)* *(complete 2026-05-15; 8 plans 05-01..05-08; SPIKE-01 delivered with documented caveats per the PIPE-01 downstream-init blocker; SPIKE-02 decision: split-pattern — GL-Core for v1.6.x → QRhi for v2)*
- [x] **Phase 5.1: Build Warnings & Latent Bug Cleanup** - Fix latent bugs and tame the warning surface surfaced by Phase 4's tri-OS CI (C4717 `getline` recursion, C4311 pointer truncation on Windows, C4910 dll-export mismatch, `-Wself-assign-field`, `-Wformat-overflow`); Codex CLI cross-checked. *(inserted 2026-05-15 — captures Phase 4 follow-ups; runs after Phase 5 so Qt 6 deprecation noise clears first; complete 2026-05-15 with 14/14 plans landed — full Tier A bug fixes + Tier B Windows DLL hygiene + Tier D build configuration; retroactive Windows CI validation expected on next clean tri-OS run once the Linux `aqtinstall` Qt 6.5.3 cancellation cascade is fixed separately)*
- [ ] **Phase 6: Python Bindings** - Decide the binding generator via a vertical slice (5-10 core classes), then commit *(restructured per Codex review — was a single under-scoped criterion)*
- [ ] ~~**Phase 7: Networking Rework**~~ - **Deferred to backlog 999.3** — not core value, the Asio code already compiles (Phase 1); the proper rework + test is 1.6.x polish
- [ ] **Phase 8: Packaging & Distribution** - Notarizable macOS bundle (`data/` embedded, `macdeployqt`); documented build-from-source for Linux/Windows; license/distribution review
- [ ] **Phase 9: Test Suite Triage** - Wire the `test/` tree into `ctest` and triage failures *(the build matrix moved to Phase 02.2)*

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
- [ ] 04.1-01-PLAN.md — Wave 1: writePreferenceEntries diff/override write + readPreferenceEntries legacy-Elements discard + ElementColorOverrides parse + all-white sanity (D-01, D-03, D-05, D-06)
- [ ] 04.1-02-PLAN.md — Wave 2: Reset element colors button (.ui + header slot + slot impl) + residue follow-up backlog stub (D-02, D-04)

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


### Phase 6: Python Bindings
**Goal**: Re-establish BALL's Python bindings on a supported Python (3.12+) and a maintained binding generator — via a decision-first vertical slice, not a blind full migration of all 237 `.sip` files. Restructured per the Codex review (SIP 6 migration vs a pybind11/nanobind rewrite are different projects; the generator must be *chosen* on evidence).
**Depends on**: Phase 3 (C++17 codebase), Phase 4 (modern dependency system)
**Requirements**: PY-01, PY-02
**Success Criteria** (what must be TRUE):
  1. A vertical slice binds and imports 5-10 representative core BALL classes, proving ownership/lifetime, exception translation, STL-container handling, and build packaging — for the candidate generator(s)
  2. A decision record names the chosen generator (SIP 6 or pybind11/nanobind) with rationale and a scoped plan for the remaining bindings
  3. The chosen generator builds against Python 3.12+ and the slice module imports and exercises core BALL classes from a Python interpreter
**Plans**: TBD

### Phase 7: Networking Rework — DEFERRED TO BACKLOG 999.3
**Status**: Removed from the v1.6 active roadmap per the Codex review. Networking is not core value, and the Boost.Asio code already *compiles* (the API breakage was fixed in Phase 1). The proper `TCPServer` rework + unit test is 1.6.x polish, tracked as backlog **999.3**. `NET-01` moved to REQUIREMENTS.md "Deferred (1.6.x)".
**Plans**: TBD

### Phase 8: Packaging & Distribution
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
**Requirements**: CI-02
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
| 4.1 Config Color-Defaults Fix | 0/2 | Not started (promoted from backlog 999.4; 2 plans, 2 waves) | - |
| 5. Qt 6 Migration (4b) + Renderer Backend Spike | 8/8 | Complete — Plans 01-08 complete (CMake bring-up, source renames, QSurfaceFormat compat, CI matrix + Qt5 lint, GL-core spike, QRhi spike + Qt 6 link bring-up, driver-behaviour record, SPIKE-02 decision: GL-Core for v1.6.x → QRhi for v2) | 2026-05-15 |
| 5.1 Build Warnings & Latent Bug Cleanup | 14/14 | Complete — Tier A: C4717 getline + C4311 pointer-trunc audit + -Wself-assign-field + -Wtautological + -Wformat-overflow CIF + -Wstringop-truncation; Tier B: C4910 BALL_EXPORT vector3/atom + C4834/C4996 GeneticIndividual+regressionModel + B3 C4251 pragma; Tier D: D1 Qt5LinguistTools + D2 Node-20 pin bump + D3 apt-cache narrowing + D4 Windows --config Release + D5 BALLView.app CFBundleIdentifier. Carry-forward: B3 baseline measurement on next clean tri-OS CI run. | 2026-05-15 |
| 6. Python Bindings | 0/0 | Not started | - |
| 7. Networking Rework | — | Deferred to backlog 999.3 | - |
| 8. Packaging & Distribution | 0/0 | Not started | - |
| 9. Test Suite Triage | 0/0 | Not started | - |

---

## Backlog

### Phase 999.1: BALLView UI maintainer open-questions (BACKLOG)

**Goal:** Get BALL maintainer decisions on 4 UI questions from the Claude Design Handover package — long lead time, raise before Milestone 2 ("BALLView Refresh", SEED-001) reaches its Inspector phase.
**Questions:**
  1. macOS menu bar — keep inline, or use the native global menubar via `QAction::setMenuRole`?
  2. Legacy 5-dock "Classic" workspace — keep as a long-term opt-in preset, or retire after one release?
  3. Theme picker — ship one neutral theme, or Light/Dark/Follow-System? (handover recommends Follow-System)
  4. Translation churn — the menu re-org invalidates ~40% of `BALLView-de_DE.ts`; plan a community translation round.
**Status:** Questions written up issue-ready in `.planning/MAINTAINER-QUESTIONS-999.1.md` (2026-05-14) — awaiting publication to maintainers.
**Requirements:** TBD
**Reference:** `.planning/MAINTAINER-QUESTIONS-999.1.md`, `.planning/DESIGN-HANDOVER-INTEGRATION.md`, `.planning/seeds/SEED-001-ballview-refresh-ui-milestone.md`

Plans:
- [ ] TBD (promote with /gsd-review-backlog when ready)

### Phase 999.2: Ninja build generator (BACKLOG)

**Goal:** Switch the default CMake generator from Make/MSBuild to **Ninja** across all three platforms.
**Why:** Faster incremental builds and consistent parallelism everywhere; on Windows it replaces slow MSBuild and makes the legacy `ball_contrib` "do not use -j" hazard moot (contrib is already obsolete on macOS/Linux). Pure build-tooling change — **zero source changes**, just `cmake -G Ninja`.
**Cross-platform impact sketch:**
  - **macOS:** `brew install ninja`; `cmake -G Ninja`. Faster rebuilds, better core utilization than Make. Update `BUILD-macos.md`. Low risk.
  - **Linux:** `apt/dnf install ninja-build`; `cmake -G Ninja`. Same benefits. Low risk.
  - **Windows:** Biggest win — replaces MSBuild (`cmake -G Ninja` in a VS dev shell, or `Ninja Multi-Config`). Eliminates the `ball_contrib` `/maxcpucount`/`-j` undefined-behaviour warning entirely. Slightly more setup (Ninja must be on PATH).
  - **CI (Phase 9):** the GH Actions matrix should standardize on `-G Ninja` on all three runners — simpler, faster, uniform.
  - **Risk:** Low. CMake fully supports Ninja; the only watch-items are non-standard custom commands / `add_custom_command` ordering and any code that shells out assuming Makefile targets. BALL's CMake is fairly standard. Best sequenced *after* Phase 4 (dependency overhaul) and *with* Phase 9 (CI) so it lands once, matrix-wide.
**Requirements:** TBD
**Plans:** 0 plans

Plans:
- [ ] TBD (promote with /gsd-review-backlog when ready)

### Phase 999.3: Networking rework (BACKLOG)

**Goal:** Rework `TCPServer`/`TCPServerThread` onto the modern Boost.Asio acceptor/socket model and cover it with a unit test.
**Why backlog, not v1.6:** Networking is not the milestone's core value, and the Boost.Asio API breakage was already fixed in Phase 1 — the code compiles and links. A proper rework + test is genuine polish but does not gate "build and render on 3 OSes". Deferred to a 1.6.x release. (Moved out of the active roadmap per the Codex adversarial review.)
**Requirements:** NET-01
**Plans:** 0 plans

Plans:
- [ ] TBD (promote with /gsd-review-backlog when ready)

### Phase 999.4: Config shadows compiled color defaults — PROMOTED to Phase 4.1

**Status:** Promoted to active **Phase 4.1: Config Color-Defaults Fix** on 2026-05-14 (real user-facing bug; only workaround is deleting `~/.BALLView`). See the Phase 4.1 detail section above.

### Phase 999.5: Open-PR triage (BACKLOG)

**Goal:** Review every open pull request on `BALL-Project/ball` in GitHub — triage each one against the current state of the codebase.
**Why:** The repo was frozen at a 2022 commit; open PRs predate the 1.6 modernization work and may be stale, superseded by the patches/phases already landed, or still valuable. They need a deliberate merge / close / defer decision rather than being left to rot.
**Scope sketch:**
  - Enumerate all open PRs (`gh pr list`).
  - For each: assess relevance to v1.6 modernization (or a later milestone), check for conflicts with work already done (Phases 1–02.2), decide merge / rebase-then-merge / close / defer-to-milestone, and record the rationale.
  - Output: a triage table (PR #, title, decision, rationale).
**Requirements:** TBD
**Plans:** 0 plans

Plans:
- [ ] TBD (promote with /gsd-review-backlog when ready)

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

### Phase 999.7: Qt 6 Linux + Windows Bring-Up and CI Fixup (BACKLOG · DORMANT)

**Goal:** Bring Linux and Windows runtime to parity with the Phase 5 macOS Qt 6 baseline. Phase 5 delivered the migration end-to-end on macOS-arm64 (configure + link + smoke green); Linux and Windows were deferred because three CI/tooling carry-forwards (aqtinstall module syntax, vcpkg baseline drift, Windows hardware) blocked the matrix.
**Why DORMANT, not active:** Phase 5.1 is the next active phase (warnings + latent bugs from Phase 4's tri-OS CI). Folding Linux/Windows Qt 6 bring-up into 5.1 would blur its scope. Promote when Phase 5.1 closes AND a contributor has Windows access (or the project decides to ship v1.6.0 macOS-only with a "build from source, unverified" footnote for Linux/Windows).
**Scope:** (1) Resolve SEED-005-1 (Linux aqtinstall) + green Linux build + smoke. (2) Resolve SEED-005-2 (Windows vcpkg baseline) + SEED-005-3 (Windows VM driver capture) + green Windows build + smoke. (3) One-line macOS CI assertion relaxation (`gl_profile=(compatibility|none)`) — Apple GL 2.1 emits `none`, not `compatibility`. (4) Optional: SEED-005-4 post-PIPE-01 spike re-capture once full render works.
**Depends on:** Phase 5 (DONE), Phase 5.1 (NOT YET DONE — sequencing only, not technical)
**Reference:** [`.planning/phases/999.7-qt6-linux-windows-bringup/QT6-LINUX-WINDOWS-BACKLOG.md`](phases/999.7-qt6-linux-windows-bringup/QT6-LINUX-WINDOWS-BACKLOG.md) — consolidated scope + per-seed effort
**Requirements:** QT6-01-LINUX, QT6-01-WINDOWS (new, TBD on promotion); the existing QT6-01 stays "macOS verified" via Phase 5

Plans:
- [ ] TBD (do NOT promote before Phase 5.1 closes; needs Windows access)

### Phase 999.8: Auto-Update via Sparkle + WinSparkle (BACKLOG · DORMANT)

**Goal:** BALLView checks for new releases on launch and prompts the user to install — Sparkle on macOS, WinSparkle on Windows. Eliminates the "manually re-download from GitHub Releases" friction for v1.x → v1.x+1 updates.
**Why DORMANT, not active:** Phase 8 ships installers WITHOUT auto-update — captured as a deliberate Phase 8 deferral (Decision #5, 2026-05-15) because auto-update is non-trivial enough (appcast hosting, signed update manifests, key management, rollback handling) to be its own backlog phase. Phase 8 unblocks this — Sparkle + WinSparkle integrate INTO the installer pipeline.
**Depends on:** Phase 5 (Qt 6 — Sparkle hooks into QApplication event loop), Phase 8 (Packaging — signing infrastructure must exist first), project decision on appcast hosting (GitHub Pages vs ball-project.org vs CDN bucket)
**Reference:** [`.planning/phases/999.8-auto-update-sparkle-winsparkle/AUTO-UPDATE-BACKLOG.md`](phases/999.8-auto-update-sparkle-winsparkle/AUTO-UPDATE-BACKLOG.md) — full scope (Sparkle vs WinSparkle integration, appcast hosting options, key management, recommended 3-plan structure)
**Requirements:** TBD

Plans:
- [ ] TBD (do NOT promote before Phase 8 closes)

### Phase 999.9: Replace INIFile with YAML config (BACKLOG)

**Goal:** Remove `BALL::INIFile` (the legacy in-tree INI parser/writer) and replace it project-wide with YAML. All 20 `.ini` files under `data/` get converted to `.yaml`; all ~49 source files that call `INIFile::read()` / `INIFile::write()` / `getValue()` / `setValue()` / section traversal migrate to the new YAML config API; `include/BALL/FORMAT/INIFile.h` + `source/FORMAT/INIFile.C` are deleted at the end of the migration.

**Why:** INI is a 1990s format with no schema, no nested structures, no type semantics — BALL's force-field parameter files (AMBER, MMFF94, CHARMM, GAFF) are deeply hierarchical and currently encoded via ad-hoc INI section conventions that the parser and every caller have to keep in sync by hand. YAML is the modern equivalent (nested maps + lists + typed scalars + comments + anchors for shared definitions), has well-maintained C++ libraries (yaml-cpp), and trivially round-trips through Python / web tooling for downstream BALLAXY / Jupyter / inspection workflows. The migration also removes a custom parser surface (~125 INIFile references across BALL's hot code paths) from the codebase, shrinking the maintenance footprint.

**Why BACKLOG, not active:** This is not core value (build + render on 3 OSes). INI files work today. The win is modernization + tooling + schema validation. Should land in a v1.7-track or v1.8 milestone, NOT in v1.6.x — v1.6.x is the modernization release that ships BALL/BALLView buildable; data-format churn during a release-cycle would invalidate the "same scientific output" guarantee.

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
- [ ] TBD (promote with /gsd-review-backlog when ready; not before v1.6.x ships stable — needs the modernization milestone closed first so the migration doesn't compound the v1.6 release risk)

---
*Roadmap created: 2026-05-14*
*Mirrors `/Users/kohlbach/Claude/BALL/ROADMAP-1.6.md` (phases 1, 2, 3, 4a, 4b, 5, 6, 7, 8). Revised 2026-05-14 after Codex adversarial review — cheap fixes applied; structural changes (early CI phase, Phase 5 split, diagnostics requirement, feature matrix) pending a deliberate roadmap revision.*
*Revised 2026-05-14 (consolidation): backlog 999.4 promoted to active Phase 4.1 (Config Color-Defaults Fix, CONFIG-01); former standalone Phase 05.1 (renderer backend spike) folded into Phase 5 since it must prototype against Qt 6.*
