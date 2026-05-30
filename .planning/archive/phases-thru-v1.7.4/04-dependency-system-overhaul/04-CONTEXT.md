# Phase 4: Dependency System Overhaul - Context

**Gathered:** 2026-05-14
**Status:** Ready for planning

<domain>
## Phase Boundary

Every dependency resolves from a current, supported source — Homebrew/system on
macOS/Linux, vcpkg on Windows — with `ball_contrib` removed from the build path,
config-mode `find_package` replacing stale bundled `Find*.cmake` modules, a
`CMakePresets.json` for stable per-platform configures, pinned minimum dependency
versions, and an explicit feature matrix.

**In scope:** `vcpkg.json` manifest + toolchain wiring, driving the Windows CI job
to green, replacing/fixing the bundled finder modules, `CMakePresets.json`, min-version
pinning, de-referencing `ball_contrib` from CMake, the FEAT-01 feature matrix, and the
OpenBabel 2.4→3.x integration port + TBB (oneTBB) enablement.
**Out of scope:** Qt 6 migration (Phase 5), wiring `test/` into ctest (Phase 9),
networking-dependency rework (backlog 999.3), Python/SIP (Phase 6), Ninja generator
(backlog 999.2).

Requirements: DEPS-01, DEPS-02, DEPS-03, DEPS-04, DEPS-05, FEAT-01.
</domain>

<decisions>
## Implementation Decisions

### vcpkg & Windows integration
- **D-01:** Phase 4 authors `vcpkg.json` (manifest mode) + wires the vcpkg toolchain
  file, **and drives the Windows CI job to green** — then flips it from non-blocking
  (`continue-on-error`) to a required check. DEPS-02's "complete Windows dependency
  set" means complete enough that Windows actually builds, not just a manifest file
  on disk. This is the designated phase for it (per Phase 02.2 D-03).
- **D-02:** vcpkg is the Windows path only; macOS/Linux stay on Homebrew/system
  packages. Do **not** adopt vcpkg as a cross-platform fallback — CI must keep testing
  the exact path contributors follow (`BUILD-macos.md`).

### Find*.cmake migration
- **D-03:** Use config-mode `find_package` wherever upstream ships a CMake config.
  Keep thin finder shims **only** for deps with no upstream config (lp_solve, libSVM).
- **D-04:** Fix the 3 finders CONCERNS.md flags as broken, **in-phase**:
  `cmake/FindOpenBabel2.cmake` (detect OpenBabel 3.x), `cmake/FindTBB.cmake` (oneTBB —
  `find_package(TBB CONFIG)`, drop the `<2017` version gate), `cmake/FindEigen3.cmake`
  (read the version from `Eigen/Version` for Eigen 5+).

### Optional-dependency policy & feature matrix (FEAT-01)
- **D-05:** **OpenBabel and TBB are both enabled in 1.6.** TBB: fix the finder, turn
  on (oneTBB). OpenBabel: port BALL's integration to the OpenBabel **3.x** API and
  enable it. They remain classified **Optional** (auto-detected, enabled when found;
  absence disables only their own features) — "ON" means enabled-by-default-when-present,
  not promoted to Required.
- **D-06:** FEAT-01 feature matrix classifies every optional dependency as
  required / optional / removed / deferred with the user-visible impact of absence.
  A missing optional dep = auto-disable with a clear configure-time message, **not** a
  hard error. Confirm/update the matrix already drafted in `REQUIREMENTS.md`.

### CMakePresets.json & ball_contrib removal
- **D-07:** Ship 4 named presets: `macos-homebrew`, `linux-system`, `windows-vcpkg`,
  `ci`. Release-focused; structure the JSON so Debug variants can be added later
  without a rewrite (mirrors Phase 02.2's Release-only CI choice).
- **D-08:** Pin minimum dependency versions in CMake (`CMakeLists.txt` / `find_package`
  calls), documented alongside the presets.
- **D-09:** Remove every `ball_contrib` reference from `ball/`'s CMake — search paths,
  finder hints, contrib-build glue — so it is fully off the build path. (`ball_contrib`
  is a separate sibling repo; this phase de-references it, it does not delete it.)

### Claude's Discretion
- Exact vcpkg triplet(s) for Windows, preset cache-variable layout, finder shim style.
- Whether min-version pins live as `find_package(... X.Y)` calls vs a central version
  block — planner decides.
- How to sequence the OpenBabel 3.x port within the phase (likely its own plan/wave —
  it is the one source-touching item in an otherwise CMake-centric phase).
</decisions>

<canonical_refs>
## Canonical References

**Downstream agents MUST read these before planning or implementing.**

### Phase definition
- `.planning/ROADMAP.md` §"Phase 4: Dependency System Overhaul" — goal, success criteria
- `.planning/REQUIREMENTS.md` §"Dependency System" (DEPS-01…05, FEAT-01) + §"Feature Matrix"

### Codebase intel
- `.planning/codebase/CONCERNS.md` §"Dependency Issues" — the 3 broken finders
  (OpenBabel 2.4, TBB pre-oneTBB, Eigen 5 version header) + §"Known Limitations"
- `.planning/codebase/STACK.md`, `.planning/codebase/INTEGRATIONS.md` — full dependency inventory
- `BUILD-macos.md` — the Homebrew dependency list CI and the presets must mirror

### Files to modify
- `cmake/FindOpenBabel2.cmake`, `cmake/FindTBB.cmake`, `cmake/FindEigen3.cmake` — the 3 finders to fix (D-04)
- `cmake/BALLConfigBoost.cmake` — Boost config-mode (reference for the config-mode pattern)
- `CMakeLists.txt` — `find_package` calls, min-version pinning (D-08), `ball_contrib` de-referencing (D-09)
- `.github/workflows/ci.yml` — flip the Windows job from non-blocking to required (D-01)
- BALL OpenBabel-integration source — the 2.4→3.x API port (D-05)

### Prior-phase context
- `.planning/phases/02.2-ci-and-build-smoke-matrix/02.2-CONTEXT.md` — D-03 (Windows
  non-blocking → required after Phase 4), D-06 (native package managers), D-11 (extensible matrix)
- `.planning/phases/03-language-modernization/03-CONTEXT.md` — C++17, `CMAKE_CXX_EXTENSIONS OFF`

No external specs or ADRs — requirements fully captured in the decisions above.
</canonical_refs>

<code_context>
## Existing Code Insights

### Reusable Assets
- Phase 02.2 CI matrix (`.github/workflows/ci.yml`) — extend it; flip the Windows job
  to required (D-01). Already structured for extra axes (02.2 D-11).
- `cmake/BALLConfigBoost.cmake` — Boost is already consumed config-mode; the template
  for the D-03 config-mode migration.

### Established Patterns
- BALL uses per-directory `sources.cmake` files, not per-target `CMakeLists.txt`.
- `BALL_HAS_*` compile-time feature flags gate optional deps (GLEW, TBB, FFTW,
  OPENBABEL, LPSOLVE, LIBSVM, …) — D-05/D-06 work within this pattern.
- Build config is generated into `include/BALL/CONFIG/config.h` from `cmake/config.h.in`.

### Integration Points
- New files: `vcpkg.json`, `CMakePresets.json`.
- Modified: `cmake/Find*.cmake` (the 3 broken finders), `CMakeLists.txt`,
  `.github/workflows/ci.yml`, plus BALL's OpenBabel-integration source for the 3.x port.

### Constraints
- `CONCERNS.md` was written using the old `ROADMAP-1.6.md` numbering — it labels the
  dependency fixes "Phase 3". The canonical GSD phase is **4**; ignore that label drift.
- CI provisions deps via native package managers (Homebrew/apt) — presets and the
  vcpkg manifest must keep CI testing the contributor path, not an `install-qt-action`
  divergence (02.2 D-06).
</code_context>

<specifics>
## Specific Ideas

- The OpenBabel 2.4→3.x API port is the one source-touching item in an otherwise
  CMake-centric phase — expect it as its own plan/wave, sequenced so a failed port
  can't block the rest of the dependency work.
- Reaching a **green Windows build** is the gating success criterion (DEPS-02) and the
  trigger to flip the CI job to required — treat it as the phase's done-bar alongside
  macOS/Linux staying green.
</specifics>

<deferred>
## Deferred Ideas

None — discussion stayed within the Phase 4 boundary. (Networking-dependency
fragility = backlog 999.3; Python/SIP generator = Phase 6; Ninja generator =
backlog 999.2 — these are pre-existing roadmap boundaries, not deferred here.)
</deferred>

---

*Phase: 04-dependency-system-overhaul*
*Context gathered: 2026-05-14*
