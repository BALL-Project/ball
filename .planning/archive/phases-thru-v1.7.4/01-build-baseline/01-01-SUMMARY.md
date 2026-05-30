---
phase: 01-build-baseline
plan: 01
subsystem: infra
tags: [cmake, boost, eigen, asio, homebrew, macos, build-system]

# Dependency graph
requires: []
provides:
  - 8 modern-toolchain patches committed to git history in 3 logical groups
  - Project version bumped to 1.6.0 with -dev pre-release marker
  - BUILD-macos.md documenting the verified Homebrew build/run flow
  - Verified reproducible reconfigure + build of the BALL library
affects: [02-rendering-port, 03-language-modernization, 04-dependency-system, networking-rework]

# Tech tracking
tech-stack:
  added: []
  patterns:
    - "Build against Homebrew/system packages; ball_contrib abandoned"
    - "CMake config-mode package discovery (Boost); modern Find module version reads (Eigen)"
    - "C++14 bridge for legacy C++17-removed constructs (std::unary_function/bind2nd)"

key-files:
  created:
    - BUILD-macos.md
  modified:
    - CMakeLists.txt
    - cmake/BALLCompilerSpecific.cmake
    - cmake/BALLConfigBoost.cmake
    - cmake/BALLViewTranslations.cmake
    - cmake/FindEigen3.cmake
    - include/BALL/DATATYPE/contourSurface.h
    - include/BALL/SYSTEM/networking.h
    - source/SYSTEM/networking.C

key-decisions:
  - "CMake VERSION field kept numeric (1.6.0); -dev pre-release marker carried as an inline comment"
  - "Patches grouped into 3 commits by concern: CMake modernization, Boost.Asio networking port, latent bug fix"

patterns-established:
  - "Stage source files by explicit path only — never git add -A/. — to keep .planning/ out of source history"
  - "macOS build documented as the reference flow against Homebrew, with keg-only bison/flex on PATH"

requirements-completed: [BUILD-01, BUILD-02, BUILD-03, BUILD-04]

# Metrics
duration: 3min
completed: 2026-05-14
---

# Phase 01 Plan 01: Build Baseline Summary

**Modern-toolchain build baseline landed: 8 toolchain patches committed in 3 logical groups, project bumped to 1.6.0-dev, BUILD-macos.md added, and a clean reconfigure + `make BALL` verified.**

## Performance

- **Duration:** 3 min
- **Started:** 2026-05-14T07:03:44Z
- **Completed:** 2026-05-14T07:06:06Z
- **Tasks:** 3
- **Files modified:** 8 source/build files + 1 new doc

## Accomplishments
- Committed the 8 previously-uncommitted modern-toolchain patches (at risk in a shallow clone) into git history with why-focused messages, grouped into 3 logical commits.
- Bumped the CMake PROJECT declaration to `VERSION 1.6.0` with a `# 1.6.0-dev (pre-release)` marker — confirmed live as `CMAKE_PROJECT_VERSION=1.6.0` in CMakeCache.txt.
- Wrote BUILD-macos.md (66 lines) documenting the verified Homebrew dependency install, CMake configure, build, and run flow for macOS Apple Silicon.
- Verified the version bump broke nothing: clean reconfigure (exit 0, "successfully configured BALL") and `make BALL -j8` (exit 0, "Built target BALL").

## Task Commits

Each task was committed atomically:

1. **Task 1: Commit the 8 toolchain patches in logical groups** — 3 commits:
   - `237881c` cmake: modernize build for CMake 3.31 and modern Find modules
   - `c32bca9` system: port Boost.Asio networking to the Boost 1.90 API
   - `5212aa8` datatype: fix latent contourSurface operator== bug
2. **Task 2: Bump project version + write BUILD-macos.md** — version bump committed in Task 3 (per plan); doc creation verified.
3. **Task 3: Verify clean reconfigure + build, then commit version bump and docs** — 2 commits:
   - `f6711e6` build: bump project version to 1.6.0-dev
   - `a116a9f` docs: add BUILD-macos.md for the Homebrew build flow

## Files Created/Modified
- `CMakeLists.txt` — CMake policy/version modernization (CMP0042/CMP0167, Dart→CTest, GLEW imported target) + project version 1.6.0-dev
- `cmake/BALLConfigBoost.cmake` — Boost config-mode package, dropped header-only `system` component
- `cmake/FindEigen3.cmake` — read version macros from Eigen/Version (Eigen >= 5.0)
- `cmake/BALLCompilerSpecific.cmake` — Clang standard c++11 → c++14 bridge
- `cmake/BALLViewTranslations.cmake` — explicit POST_BUILD on ADD_CUSTOM_COMMAND
- `include/BALL/SYSTEM/networking.h` — io_service → io_context; resolver string_view ctor changes
- `source/SYSTEM/networking.C` — io_service::post → boost::asio::post; reset() → restart(); accept(stream.socket())
- `include/BALL/DATATYPE/contourSurface.h` — latent operator== bug fixed (data.data_ → Surface::operator==)
- `BUILD-macos.md` — new: macOS Apple Silicon Homebrew build/run documentation

## Decisions Made
- CMake VERSION fields must be numeric, so the version is `1.6.0` with the `-dev` pre-release status carried in an inline comment rather than the version literal.
- The 8 patches were grouped into 3 commits by concern (build-system modernization / networking API port / latent bug fix) so each commit message can explain a single coherent rationale.

## Deviations from Plan

None - plan executed exactly as written.

## Issues Encountered
None. The cmake configure exit code was masked by a `${PIPESTATUS}` reference inside an echo subshell, but the "successfully configured BALL" banner and `CMAKE_PROJECT_VERSION:STATIC=1.6.0` in CMakeCache.txt confirmed success; `make BALL` was then run separately and reported exit 0.

## User Setup Required
None - no external service configuration required. Local Homebrew dependencies are documented in BUILD-macos.md.

## Next Phase Readiness
- The build baseline is committed, versioned, documented, and verified reproducible — Phase 2 (Rendering Port / 4a) can proceed against a stable working build.
- No blockers. The known blank-3D-scene issue is the deliberate target of Phase 2, not an obstacle here.

## Self-Check: PASSED

- FOUND: BUILD-macos.md
- FOUND: commit 237881c, c32bca9, 5212aa8, f6711e6, a116a9f

---
*Phase: 01-build-baseline*
*Completed: 2026-05-14*
