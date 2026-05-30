---
phase: 03-language-modernization
plan: "02"
subsystem: build-system
tags: [cmake, c++17, compiler-flags, lang-modernization]
dependency_graph:
  requires: [03-01]
  provides: [cmake-cxx17-standard, no-raw-std-flags]
  affects: [all BALL/VIEW/BALLView compilation targets]
tech_stack:
  added: []
  patterns: [CMAKE_CXX_STANDARD global variable, strict no-extensions mode]
key_files:
  created: []
  modified:
    - CMakeLists.txt
    - cmake/BALLCompilerSpecific.cmake
decisions:
  - "D-03: CMAKE_CXX_STANDARD 17 global (before any target), not per-target target_compile_features"
  - "D-04: Both raw -std= lines deleted, stale C++14-bridge comment block removed"
  - "D-05: CMAKE_CXX_EXTENSIONS OFF retained — strict -std=c++17 not gnu++17"
  - "D-06: Blanket -Wno-deprecated-declarations removed (binder2nd Eigen3 justification is obsolete)"
metrics:
  duration: "~5min"
  completed: "2026-05-14"
  tasks_completed: 3
  files_modified: 2
---

# Phase 03 Plan 02: CMake C++17 Standard Configuration Summary

CMake C++17 standard set via CMAKE_CXX_STANDARD 17 (global, strict, no-extensions); all raw -std= flags and obsolete suppression removed from cmake/.

## What Was Built

Bumped the project-wide C++ standard mechanism to C++17 the modern CMake way and stripped legacy raw `-std=` flags and obsolete deprecation-warning suppression from `cmake/BALLCompilerSpecific.cmake`.

### Changes Made

**CMakeLists.txt**
- `CMAKE_CXX_STANDARD` bumped from 14 to 17
- Stale bridge comment ("C++14 is the current bridge standard; Phase 3 bumps this to 17") replaced with an accurate description of the current state
- `CMAKE_CXX_STANDARD_REQUIRED ON` and `CMAKE_CXX_EXTENSIONS OFF` untouched (already correct)

**cmake/BALLCompilerSpecific.cmake**
- GCC branch: deleted `SET(BALL_PROJECT_COMPILE_FLAGS "${BALL_PROJECT_COMPILE_FLAGS} -std=c++0x")`
- Clang/AppleClang branch: deleted the 3-line stale comment block ("BALL's legacy code still uses std::unary_function...") and `SET(BALL_PROJECT_COMPILE_FLAGS "${BALL_PROJECT_COMPILE_FLAGS} -std=c++14")`
- GCC branch: deleted `SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")` and its obsolete justification comment (Eigen3 binder2nd spam — no longer relevant with Eigen 5)

## Tasks Completed

| Task | Name | Commit | Files |
|------|------|--------|-------|
| 1 | Bump CMAKE_CXX_STANDARD 14->17, strict no-extensions (D-03, D-05) | 3e82b61 | CMakeLists.txt |
| 2 | Delete both raw -std= lines and stale C++14-bridge comment (D-04) | 792a7fd | cmake/BALLCompilerSpecific.cmake |
| 3 | Remove blanket -Wno-deprecated-declarations and binder2nd comment (D-06) | a2f9f0f | cmake/BALLCompilerSpecific.cmake |

## Verification Results

All plan verification checks pass:
- `grep -rn 'std=c++' cmake/` returns nothing (exit 1)
- `grep -n 'CMAKE_CXX_STANDARD 17' CMakeLists.txt` matches line 33
- `grep -rn 'Wno-deprecated-declarations' cmake/` returns nothing (exit 1)
- `grep -n 'C++14\|binder2nd' CMakeLists.txt cmake/BALLCompilerSpecific.cmake` returns nothing

## Deviations from Plan

None — plan executed exactly as written.

## Known Stubs

None — this plan is build-configuration only (no UI, no data wiring).

## Threat Flags

None — no new network endpoints, auth paths, file access patterns, or schema changes. CMake build-config change only.

## Self-Check: PASSED

- CMakeLists.txt: CMAKE_CXX_STANDARD 17 confirmed at line 33
- cmake/BALLCompilerSpecific.cmake: no raw -std= flags, no -Wno-deprecated-declarations, no stale comments
- Commits: 3e82b61, 792a7fd, a2f9f0f all present in git log
