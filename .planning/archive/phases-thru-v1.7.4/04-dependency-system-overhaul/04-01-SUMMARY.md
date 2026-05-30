---
phase: 04-dependency-system-overhaul
plan: "01"
subsystem: infra
tags: [cmake, eigen3, tbb, openbabel, ball_contrib, find_package, config-mode]

# Dependency graph
requires:
  - phase: 03-language-modernization
    provides: C++17 standard set; CI green on all 4 jobs

provides:
  - Config-mode find_package for Eigen3 (3.4), TBB (2021.0), OpenBabel3 (3.0)
  - Deletion of cmake/FindEigen3.cmake, cmake/FindTBB.cmake, cmake/FindOpenBabel2.cmake
  - BALL_CONTRIB_PATH fully removed from the build path
  - Minimum version pins: CMake 3.21, Qt 5.15, Boost 1.70, Eigen 3.4, TBB 2021.0, OpenBabel 3.0
  - OpenBabel GPL-gate decision recorded; Plan 04-03 uses -DBALL_LICENSE=GPL configure
affects: [04-02, 04-03, 04-04, 05-qt6-migration, 08-packaging]

# Tech tracking
tech-stack:
  added: []
  patterns:
    - "Config-mode find_package with version floor at every find site (no bundled finders)"
    - "GPL-gated optional deps: FFTW and OpenBabel inside IF(BALL_LICENSE_GPL)"
    - "TBB::tbb imported target replaces TBB_LIBRARIES + TBB_INCLUDE_DIRS"
    - "Eigen3::Eigen imported target replaces EIGEN3_INCLUDE_DIR"
    - "QT_NO_KEYWORDS enabled to prevent Qt macros clashing with TBB oneAPI event::emit()"

key-files:
  created: []
  modified:
    - CMakeLists.txt
    - cmake/BALLConfigBoost.cmake
    - cmake/BALLInstallScripts.cmake.in
    - cmake/BALLFinishedMessage.cmake
  deleted:
    - cmake/FindEigen3.cmake
    - cmake/FindTBB.cmake
    - cmake/FindOpenBabel2.cmake

key-decisions:
  - "Eigen3 version range 3.4...6 used (not plain 3.4) because Homebrew ships Eigen 5.0.1 which plain 3.4 would not match"
  - "QT_NO_KEYWORDS enabled (was commented out) to prevent Qt emit/signals macros clashing with TBB oneAPI tbb::detail::d1::event::emit()"
  - "OpenBabel stays GPL-gated: FIND_PACKAGE(OpenBabel3) remains inside IF(BALL_LICENSE_GPL); LGPL builds never search for it"
  - "BALL_HAS_OPENBABEL activation deferred to Plan 04-03 — config-mode finder is in place but enabling compilation requires the 2.x→3.x API port first"
  - "Plan 04-03 must verify the OpenBabel port via a -DBALL_LICENSE=GPL configure, not the default LGPL build"

patterns-established:
  - "Pattern 1: Each find_package carries an inline version floor; rationale comment block above the cluster documents all floors"
  - "Pattern 2: Imported targets (Eigen3::Eigen, TBB::tbb, openbabel) used throughout; no legacy _INCLUDE_DIRS/_LIBRARIES variables"

requirements-completed: [DEPS-01, DEPS-03, DEPS-04]

# Metrics
duration: ~2h (split across two sessions with checkpoint pause)
completed: 2026-05-14
---

# Phase 4 Plan 01: CMake Dependency Foundation Summary

**Config-mode find_package for Eigen3/TBB/OpenBabel3 replaces three bundled finders; BALL_CONTRIB_PATH deleted; version floors pinned at CMake 3.21, Qt 5.15, Boost 1.70, Eigen 3.4, TBB 2021.0, OpenBabel 3.0; OpenBabel confirmed GPL-gated by maintainer decision.**

## Performance

- **Duration:** ~2h (across two sessions with a decision checkpoint pause)
- **Started:** 2026-05-14T15:21:00Z (Task 1)
- **Completed:** 2026-05-14T17:27:15Z (Task 3)
- **Tasks:** 3 (2 auto + 1 checkpoint:decision)
- **Files modified:** 4 modified, 3 deleted

## Accomplishments

- Deleted three bundled finder modules (`cmake/FindEigen3.cmake`, `cmake/FindTBB.cmake`, `cmake/FindOpenBabel2.cmake`) and replaced with upstream config-mode `find_package` calls with explicit version floors
- Removed all `BALL_CONTRIB_PATH` references from the active build path (CMakeLists.txt, BALLInstallScripts.cmake.in, BALLFinishedMessage.cmake); only the allowed comment in cmake/FindSIP.cmake remains
- Pinned minimum versions at each find site with a rationale comment block: CMake 3.21, Qt 5.15, Boost 1.70, Eigen 3.4 (range to 6), TBB 2021.0, OpenBabel 3.0
- Recorded maintainer decision: OpenBabel stays GPL-gated; Plan 04-03 must verify via `-DBALL_LICENSE=GPL` configure

## Task Commits

Each task was committed atomically:

1. **Task 1: Migrate Eigen3, TBB, OpenBabel finders to config-mode find_package** - `d525d48` (feat)
2. **Task 2: De-reference ball_contrib from the CMake build path and pin minimum versions** - `fb854de` (feat)
3. **Task 3: OpenBabel GPL-gate decision — move find_package back inside BALL_LICENSE_GPL block** - `f8768a1` (fix)

**Checkpoint state docs:** `218cb8b` (docs: record decisions and checkpoint state for plan 04-01)

## Files Created/Modified

- `CMakeLists.txt` — Three config-mode find_package calls, BALL_CONTRIB_PATH removed, version floors pinned, QT_NO_KEYWORDS enabled, Eigen3::Eigen + TBB::tbb imported targets, OpenBabel GPL-gated
- `cmake/BALLConfigBoost.cmake` — Boost floor bumped from 1.55 to 1.70
- `cmake/BALLInstallScripts.cmake.in` — DEPENDENCY_PATHS no longer includes ${BALL_CONTRIB_PATH}/dlls
- `cmake/BALLFinishedMessage.cmake` — echo updated to remove "contrib lib/ path" reference
- `cmake/FindEigen3.cmake` — DELETED (replaced by upstream Eigen3Config.cmake)
- `cmake/FindTBB.cmake` — DELETED (replaced by upstream TBBConfig.cmake)
- `cmake/FindOpenBabel2.cmake` — DELETED (replaced by upstream OpenBabel3Config.cmake)

## Decisions Made

**1. Eigen3 version range 3.4...6**
Homebrew ships Eigen 5.0.1. A plain `FIND_PACKAGE(Eigen3 3.4)` with the strict default version compatibility schema would not match 5.x (the version compatibility schema in Eigen3's CMake config uses a 3.x compatibility block). Using range `3.4...6` makes the requirement explicit and passes correctly.

**2. QT_NO_KEYWORDS enabled**
Qt's `emit`/`signals` macros were commented out (`#define QT_NO_KEYWORDS`) in the existing code. Enabling this was required for TBB config-mode to build cleanly: TBB's oneAPI headers include `tbb::detail::d1::event::emit()`, which clashes with Qt's `emit` keyword macro. Enabling QT_NO_KEYWORDS resolves the ambiguity with no functional impact (BALL already uses Q_EMIT).

**3. OpenBabel GPL-gated (maintainer decision, 2026-05-14)**
The prior executor (Task 1) moved `FIND_PACKAGE(OpenBabel3)` out of the `IF(BALL_LICENSE_GPL)` block per plan instructions. At the Task 3 checkpoint the maintainer chose option `gpl-gated`: OpenBabel must only be searched in GPL-licensed BALL builds. The find_package block was moved back inside `IF(BALL_LICENSE_GPL)`.

Consequence for Plan 04-03: the 2.x→3.x API port must be configured and verified with `-DBALL_LICENSE=GPL`, not the default LGPL build.

**4. BALL_HAS_OPENBABEL deferred to Plan 04-03**
Even in GPL builds, `BALL_HAS_OPENBABEL` is not activated yet. The config-mode finder is in place and detects OpenBabel 3.1.0 on the dev machine. Enabling the compile flag requires the C++ 2.x→3.x API port, which is Plan 04-03's scope.

## Deviations from Plan

### Auto-fixed Issues

**1. [Rule 1 - Bug] Eigen3 version range 3.4...6 instead of plain 3.4**
- **Found during:** Task 1 (Eigen3 config-mode migration)
- **Issue:** Homebrew provides Eigen 5.0.1. The plan specified `FIND_PACKAGE(Eigen3 3.4 REQUIRED NO_MODULE)` but the version compatibility mode in Eigen3's CMake config does not match 5.x with a plain floor of 3.4 under `NO_MODULE` strict resolution.
- **Fix:** Used version range `FIND_PACKAGE(Eigen3 3.4...6 REQUIRED NO_MODULE)` to explicitly allow the 3.x–5.x series.
- **Files modified:** CMakeLists.txt
- **Committed in:** d525d48 (Task 1 commit)

**2. [Rule 2 - Missing Critical] QT_NO_KEYWORDS enabled**
- **Found during:** Task 1 (TBB config-mode migration verify step)
- **Issue:** Configure failed with TBB config-mode: TBB oneAPI headers define `event::emit()`, which clashed with Qt's `emit` macro. The plan did not address this.
- **Fix:** Enabled `QT_NO_KEYWORDS` in CMakeLists.txt (it was already commented out as `#ADD_DEFINITIONS(-DQT_NO_KEYWORDS)` — uncommented and made active). BALL code uses `Q_EMIT` throughout, so this is a no-op for BALL code.
- **Files modified:** CMakeLists.txt
- **Committed in:** d525d48 (Task 1 commit)

---

**Total deviations:** 2 auto-fixed (1 bug fix — version range; 1 missing critical — QT_NO_KEYWORDS)
**Impact on plan:** Both fixes were required for the configure/build to succeed. No scope creep.

## Issues Encountered

- TBB config-mode import collided with Qt's `emit` keyword macro — resolved by enabling QT_NO_KEYWORDS (see Deviations above).
- Eigen3 version matching quirk between Homebrew's 5.x and the 3.x-series CMake version spec — resolved with version range.

## User Setup Required

None - no external service configuration required. Dependencies are Homebrew packages already installed on the dev machine.

## Next Phase Readiness

- Plan 04-02 (CMakePresets.json + feature matrix) can proceed immediately — it builds directly on this plan's config-mode foundation.
- Plan 04-03 (OpenBabel 2.x→3.x port) must use `-DBALL_LICENSE=GPL` for its verification configure; the default LGPL build will not exercise OpenBabel.
- Plan 04-04 (vcpkg manifest for Windows) has the version pins it needs from this plan.
- `BALL_CONTRIB_PATH` is gone — no other plan should reference it.

## Known Stubs

- `BALL_HAS_OPENBABEL` remains `FALSE` in all builds (default LGPL and GPL alike) until Plan 04-03 completes the C++ API port. The `FIND_PACKAGE(OpenBabel3 3.0 CONFIG)` call inside the GPL block is a detection-only stub: it reports "found" but does not enable any BALL code.

## Threat Flags

None — all STRIDE mitigations from the plan's threat model are in place:
- T-04-01: version pins at all find_package sites (Eigen3 3.4, TBB 2021.0, OpenBabel 3.0, Boost 1.70, Qt 5.15)
- T-04-02: BALL_CONTRIB_PATH deleted entirely

## Self-Check

Files verified:

- CMakeLists.txt exists and contains config-mode calls
- cmake/FindEigen3.cmake: DELETED (confirmed)
- cmake/FindTBB.cmake: DELETED (confirmed)
- cmake/FindOpenBabel2.cmake: DELETED (confirmed)

Commits verified:

- d525d48: Task 1 (Eigen3/TBB/OpenBabel config-mode migration)
- fb854de: Task 2 (ball_contrib removal + version pins)
- f8768a1: Task 3 (OpenBabel GPL-gate revert)
- 218cb8b: Checkpoint docs

## Self-Check: PASSED

---
*Phase: 04-dependency-system-overhaul*
*Completed: 2026-05-14*
