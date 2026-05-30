---
phase: 04-dependency-system-overhaul
plan: "02"
subsystem: infra
tags: [cmake, presets, ci, feature-matrix, deps-05, feat-01]

# Dependency graph
requires:
  - phase: 04-dependency-system-overhaul
    plan: "01"
    provides: Config-mode find_package for Eigen3/TBB/OpenBabel3; version floors; ball_contrib removed

provides:
  - CMakePresets.json with 4 D-07 named presets (macos-homebrew, linux-system, windows-vcpkg, ci-* family)
  - BALL_HAS_OPENBABEL=ON in ci-macos/ci-linux presets (single source of truth for Plan 03)
  - BUILD-macos.md migrated to cmake --preset macos-homebrew
  - .github/workflows/ci.yml macOS+Linux steps migrated to cmake --preset ci-macos / ci-linux
  - FEAT-01 feature matrix annotated with per-platform availability + D-06 auto-disable policy
affects: [04-03, 04-04, 08-packaging]

# Tech tracking
tech-stack:
  added:
    - "CMakePresets.json (version 3, CMake 3.21+)"
  patterns:
    - "cmake --preset <name> as the single configure invocation for contributors and CI"
    - "ci-* preset family: inherits platform preset + adds ccache + BALL_HAS_OPENBABEL=ON"
    - "FIND_PACKAGE(Eigen3 REQUIRED NO_MODULE) + manual IF(VERSION_LESS) floor check (avoids version-range incompatibility with Ubuntu's old Eigen3ConfigVersion.cmake)"

key-files:
  created:
    - CMakePresets.json
  modified:
    - BUILD-macos.md
    - .github/workflows/ci.yml
    - CMakeLists.txt
    - .planning/REQUIREMENTS.md

key-decisions:
  - "ci-* family (ci-macos/ci-linux/ci-windows) satisfies D-07's fourth named preset: a single ci preset cannot carry per-platform cache vars, so it is split per-platform (RESEARCH Open Question 2)"
  - "BALL_HAS_OPENBABEL=ON in ci-macos and ci-linux presets (not in ci.yml); Plan 03 adds the smoke step only, not the cache variable"
  - "macos-homebrew preset does not set BALL_HAS_OPENBABEL (auto-detect for local builds); windows-vcpkg keeps it OFF (not a vcpkg port)"
  - "Eigen3 find_package changed from 3.4...6 range to plain REQUIRED NO_MODULE + manual version check: Ubuntu 22.04 Eigen 3.4.0 ships old Eigen3ConfigVersion.cmake that does not support range syntax"

requirements-completed: [DEPS-05, FEAT-01]

# Metrics
duration: ~26min
completed: 2026-05-14
---

# Phase 4 Plan 02: CMakePresets.json + Feature Matrix Summary

**CMakePresets.json with 4 D-07 presets (macos-homebrew, linux-system, windows-vcpkg, ci-* family) replaces bespoke per-platform cmake invocations in BUILD-macos.md and CI; FEAT-01 feature matrix annotated with per-platform availability and D-06 auto-disable policy; CI green on all 4 jobs via preset path.**

## Performance

- **Duration:** ~26min
- **Started:** 2026-05-14T15:30:05Z
- **Completed:** 2026-05-14T15:56:16Z
- **Tasks:** 3 auto (+ 1 auto-fix deviation)
- **Files modified:** 4 modified, 1 created

## Accomplishments

- Authored `CMakePresets.json` (version 3, CMake 3.21+) with `base` hidden preset, 4 named configure presets, and 6 build presets. The `ci-macos`/`ci-linux` presets carry `BALL_HAS_OPENBABEL=ON` as the single source of truth for Plan 03's CI enablement.
- Migrated `BUILD-macos.md` from a bespoke 8-line cmake command to `cmake --preset macos-homebrew` / `cmake --build --preset macos-homebrew`. Added "Build presets" subsection with D-07 → `ci-*` mapping note.
- Migrated `.github/workflows/ci.yml` macOS Configure+Build and Linux Configure+Build steps to `cmake --preset ci-macos` / `cmake --preset ci-linux`. Smoke check steps updated to pass explicit binary paths (`build/ci-{os}/`) since the preset binaryDir differs from the old `build/` assumed by the auto-detect.
- Annotated the FEAT-01 feature matrix in `REQUIREMENTS.md` with per-platform availability (OpenBabel ON/OFF, LPSolve ON/OFF, libSVM all-platforms-via-vcpkg, FFTW GPL-builds-only, GLEW Required-when-VIEW). Added D-06 auto-disable policy sentence.
- CI run 25869677637 green on all 4 jobs: build (macos-arm64), build (linux-x64), build (windows-x64, non-blocking), lint.

## Task Commits

1. **Task 1: Author CMakePresets.json** — `d7356ff` (feat)
2. **Task 2: Migrate BUILD-macos.md and CI steps** — `a55f704` (feat)
3. **Task 3: Annotate FEAT-01 feature matrix** — `76ed477` (feat)
4. **[Rule 1 - Bug] Fix Eigen3 version range for Linux** — `d5fe7b6` (fix)

## Files Created/Modified

- `CMakePresets.json` — New. 7 configure presets (base hidden + 6 named) + 6 build presets. Version 3 (CMake 3.21+). `ci-macos`/`ci-linux` set `BALL_HAS_OPENBABEL=ON`; `windows-vcpkg` keeps it `OFF`.
- `BUILD-macos.md` — Replaced bespoke cmake command with `cmake --preset macos-homebrew`. Added "Build presets" subsection with D-07 → `ci-*` traceability note.
- `.github/workflows/ci.yml` — macOS Configure: `cmake --preset ci-macos`. macOS Build: `cmake --build --preset ci-macos`. Linux Configure: `cmake --preset ci-linux`. Linux Build: `cmake --build --preset ci-linux`. Smoke check steps: pass explicit binary paths (`build/ci-macos/...`, `build/ci-linux/...`). Windows Best-effort step and lint job: untouched.
- `CMakeLists.txt` — Eigen3 find_package changed from `FIND_PACKAGE(Eigen3 3.4...6 REQUIRED NO_MODULE)` to `FIND_PACKAGE(Eigen3 REQUIRED NO_MODULE)` + `IF(Eigen3_VERSION VERSION_LESS "3.4")` floor check (see Deviations).
- `.planning/REQUIREMENTS.md` — Feature Matrix annotated with Platform availability column, per-dep notes, D-06 policy sentence. Classification unchanged for all 10 components.

## Decisions Made

**1. ci-* family satisfies D-07's "ci" preset**
D-07 specifies 4 named presets including one literally called "ci". A single `ci` preset cannot carry per-platform cache vars (different CMAKE_PREFIX_PATH, toolchain files, BISON/FLEX paths) — so the plan ships `ci-macos` / `ci-linux` / `ci-windows` as a family. The D-07 → `ci-*` mapping is documented in both BUILD-macos.md (the maintainer-visible note) and in the plan objective. Sanctioned by RESEARCH Open Question 2.

**2. BALL_HAS_OPENBABEL=ON in ci-* presets, not in ci.yml**
Plan 03 enables OpenBabel in CI. To keep `CMakePresets.json` the single source of truth, `BALL_HAS_OPENBABEL=ON` is authored into `ci-macos` and `ci-linux` here. Plan 03 adds the smoke step only — it does NOT pass a `-DBALL_HAS_OPENBABEL=ON` CLI arg in ci.yml.

## Deviations from Plan

### Auto-fixed Issues

**1. [Rule 1 - Bug] Drop Eigen3 version range — Ubuntu 22.04 Eigen3ConfigVersion.cmake doesn't support range syntax**
- **Found during:** CI run 25869137308 (after Task 2 commit) — Linux Configure step failed
- **Issue:** `FIND_PACKAGE(Eigen3 3.4...6 REQUIRED NO_MODULE)` was introduced in Plan 04-01 to handle Homebrew's Eigen 5.x on macOS. The version range requires `Eigen3ConfigVersion.cmake` to implement the range-check block. Ubuntu 22.04's system `libeigen3-dev` (3.4.0) ships an old-style `Eigen3ConfigVersion.cmake` using `COMPATIBILITY AnyNewerVersion` which interprets the `...6` upper bound as "is 3.4.0 >= 6?" → NO → package rejected. The previous Linux CI was green because the bespoke `cmake ..` invocation happened to restore a ccache'd configure (same `build/` directory) that masked the issue. The preset migration changed the build directory to `build/ci-linux/` (fresh configure), exposing the latent bug.
- **Fix:** Changed to `FIND_PACKAGE(Eigen3 REQUIRED NO_MODULE)` (no version constraint) followed by `IF(Eigen3_VERSION VERSION_LESS "3.4") MESSAGE(FATAL_ERROR ...)`. This works on both macOS (Eigen 5.0.1 with new ConfigVersion script) and Linux (Eigen 3.4.0 with old script).
- **Files modified:** CMakeLists.txt
- **Commit:** d5fe7b6
- **CI verification:** Run 25869677637 — all 4 jobs green including Linux configure + build + render smoke.

---

**Total deviations:** 1 auto-fixed bug (Eigen3 version range)
**Impact on plan:** Required one additional commit and a second CI run. No scope creep.

## CI Results

- **Run 25869137308** (after Tasks 1-3): macOS green, Linux red (Eigen3 version range), Windows non-blocking, lint green
- **Run 25869677637** (after Eigen fix): ALL 4 jobs green — build macos-arm64, build linux-x64 (with render smoke PASS), build windows-x64 (non-blocking), lint

## Known Stubs

None — `CMakePresets.json` is fully wired to CI and BUILD-macos.md. `BALL_HAS_OPENBABEL=ON` is set in `ci-macos`/`ci-linux` presets but the actual OpenBabel source API port is Plan 04-03's scope.

## Threat Flags

None — T-04-05 through T-04-08 mitigations verified:
- T-04-05: `CMakePresets.json` is version-controlled; CI runs only on milestone branches (`on: push: branches: [master, v1.6-modernization]`)
- T-04-06: `VCPKG_ROOT` for `windows-vcpkg` toolchainFile — Plan 04 pins vcpkg baseline; documented in BUILD-macos.md presets note
- T-04-07: Homebrew absolute paths in presets are public standard locations, not secrets
- T-04-08: `cmake --preset ci-macos -N` dry-run prints resolved cache vars for auditability

## Self-Check

Files verified:

- CMakePresets.json: FOUND
- BUILD-macos.md: FOUND
- .github/workflows/ci.yml: FOUND
- CMakeLists.txt: FOUND (Eigen3 range fix)
- .planning/REQUIREMENTS.md: FOUND

Commits verified:

- d7356ff: Task 1 (CMakePresets.json)
- a55f704: Task 2 (BUILD-macos.md + ci.yml preset migration)
- 76ed477: Task 3 (REQUIREMENTS.md feature matrix)
- d5fe7b6: Deviation fix (Eigen3 version range)

## Self-Check: PASSED

---
*Phase: 04-dependency-system-overhaul*
*Completed: 2026-05-14*
