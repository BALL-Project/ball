---
phase: 03-language-modernization
plan: 03
subsystem: build-system
tags: [cpp17, build-and-fix, ci-green, lang-modernization]
dependency_graph:
  requires: [03-01, 03-02]
  provides: [LANG-01-done, D-08-done, cpp17-clean-build]
  affects:
    - source/COMMON/exception.C
    - source/FORMAT/SDFile.C
    - source/KERNEL/expressionParserParser.y
tech_stack:
  added: []
  patterns: [noexcept-over-throw-spec, std-qualified-names, removed-dynamic-exception-specs]
key_files:
  created: []
  modified:
    - source/COMMON/exception.C
    - source/FORMAT/SDFile.C
    - source/KERNEL/expressionParserParser.y
decisions:
  - "D-07 applied: fixed all 3 additional C++17 breakages surfaced by the build — std::set_unexpected removed, unscoped streampos qualified, dynamic exception spec removed from parser"
  - "D-08 achieved: all 3 CI platforms green on commit 446cf10; render smoke passed on macOS and Linux; Windows also green"
  - "D-06 no action needed: no narrowed Qt deprecation suppression added — the build produced only warnings, no Qt-noise flood requiring intervention"
metrics:
  duration: "~20min"
  completed: "2026-05-14"
  tasks_completed: 2
  files_modified: 3
---

# Phase 03 Plan 03: C++17 Build-and-Fix Loop Summary

**One-liner:** Fixed 3 additional C++17 breakages surfaced by the local build (set_unexpected removal, unscoped streampos, dynamic exception spec in parser), then confirmed all 3 CI platforms (macOS, Linux, Windows) green on commit 446cf10 with render smoke passing on both macOS and Linux.

## Tasks Completed

| Task | Name | Commit | Files |
|------|------|--------|-------|
| 1 | Local C++17 build-and-fix loop — compile cleanly on macOS | 446cf10 | source/COMMON/exception.C, source/FORMAT/SDFile.C, source/KERNEL/expressionParserParser.y |
| 2 | Push C++17 commit and confirm 3-platform CI green (D-08) | (push of 446cf10) | CI run 25862456769 |

## What Was Done

### Task 1: Local C++17 Build-and-Fix Loop

Ran the full local build per `BUILD-macos.md` after Plans 01 and 02 landed. The C++17 build surfaced exactly 3 errors beyond the 7 known Plan 01 files:

#### Fix 1: `std::set_unexpected` removed in C++17

**File:** `source/COMMON/exception.C` line 333  
**Construct:** `std::set_unexpected(terminate);` in `GlobalExceptionHandler::GlobalExceptionHandler()`  
**Error:** `error: no type named 'set_unexpected' in namespace 'std'`  
**Root cause:** `std::set_unexpected` was removed in C++17 as part of removing dynamic exception specifications (`throw()` on function declarations). Since no code in the codebase uses dynamic exception specs anymore (also removed in C++17), `std::unexpected` is never invoked and there is nothing to set.  
**Fix:** Removed the `std::set_unexpected(terminate)` call. The `std::set_terminate(terminate)` and `std::set_new_handler(newHandler)` calls remain — they are still valid in C++17.  
**Behaviour:** Identical. `std::unexpected` was never invoked in practice since dynamic exception specs were already obsolete.

#### Fix 2: Unscoped `streampos` not available in C++17

**File:** `source/FORMAT/SDFile.C` line 161  
**Construct:** `streampos current_pos = tellg();`  
**Error:** `error: unknown type name 'streampos'; did you mean 'boost_swap_impl::streampos'?`  
**Root cause:** `streampos` without `std::` qualification relied on implicit namespace injection that was tightened in C++17. The type exists in `<ios>` as `std::streampos`.  
**Fix:** Qualified as `std::streampos current_pos = tellg();`  
**Behaviour:** Identical.

#### Fix 3: Dynamic exception specification in parser source

**File:** `source/KERNEL/expressionParserParser.y` lines 11 and 126  
**Construct:** `throw(Exception::ParseError)` on both the declaration and definition of `ExpressionParsererror`  
**Error:** `error: ISO C++17 does not allow dynamic exception specifications [-Wdynamic-exception-spec]`  
**Root cause:** Dynamic exception specifications (`throw(T)` on function declarations) were removed in C++17. The function body immediately throws anyway, making the spec redundant even in C++14.  
**Fix:** Removed the `throw(Exception::ParseError)` specification from both the `extern` declaration and the function definition. Function signature is unchanged in all other respects.  
**Behaviour:** Identical. The function still throws `Exception::ParseError` — the spec removal doesn't suppress the throw.

### Task 2: Push and CI Verification (D-08)

Pushed commit `446cf10` to `v1.6-modernization`. CI run `25862456769` ran to completion.

**CI Run URL:** https://github.com/BALL-Project/ball/actions/runs/25862456769

**Per-platform status:**

| Platform | Job | Status | Notes |
|----------|-----|--------|-------|
| macOS-arm64 | build (macos-arm64) | GREEN | 7m7s |
| Linux | build (linux-x64) | GREEN | 12m9s |
| Windows | build (windows-x64) | GREEN | 1m0s |
| All | lint (legacy-GL grep gate) | GREEN | 14s |
| macOS | Render smoke check (macOS — native) | PASSED | Non-blank PNG confirmed |
| Linux | Render smoke check (Linux — xvfb + software Mesa) | PASSED | Non-blank PNG confirmed |

All 4 jobs green. Windows is non-blocking per Phase 02.2 design (flips to required in Phase 4) but went green too.

## C++17 Fixes Beyond Plan 01 Set — Complete Record

| File | Construct | C++17 Change | Fix Applied |
|------|-----------|-------------|-------------|
| source/COMMON/exception.C | `std::set_unexpected(terminate)` | Removed in C++17 (dynamic exception spec elimination) | Removed the call |
| source/FORMAT/SDFile.C | `streampos current_pos` (unqualified) | Stricter namespace injection under C++17 | Qualified as `std::streampos` |
| source/KERNEL/expressionParserParser.y | `throw(Exception::ParseError)` on function decl/def | ISO C++17 forbids dynamic exception specs | Removed the spec |

The 7-file Plan 01 set was the complete STL-adapter removal. This plan adds 3 more C++17 removals to the evidence trail. Total: 10 files touched across Phase 3 Plans 01 and 03.

## Deprecation-Noise Decision (D-06 follow-up)

The build ran without the blanket `-Wno-deprecated-declarations`. The warnings produced were:
- `sprintf` deprecated on macOS (system stdlib warning — not BALL code, OS-SDK-level)
- `unqualified-std-cast-call` for `move()` in `string.iC` (legitimate style warning, not a C++17 removal)

No Qt 5.15 deprecation noise flood occurred. No narrowed suppression was added. The blanket removal (D-06) stands as the correct decision.

## Verification Results

All acceptance criteria met:

- `grep -rn 'unary_function|binary_function|bind2nd|bind1st|ptr_fun|auto_ptr|mem_fun' include/ source/` → exit 1 (zero matches)
- `grep -rn 'std=c++' cmake/` → exit 1 (no raw `-std=` flags)
- `grep -n 'CMAKE_CXX_STANDARD 17' CMakeLists.txt` → line 33 matches
- `grep -rn 'Wno-deprecated-declarations' cmake/` → exit 1 (no suppression)
- BALL/VIEW/BALLView build: zero errors under C++17 on macOS-arm64
- CI run 25862456769: all 4 jobs green, render smoke passed on macOS + Linux

## Deviations from Plan

### Auto-fixed Issues

**1. [Rule 1 - Bug] `std::set_unexpected` removed in C++17**
- **Found during:** Task 1 (first build attempt)
- **Issue:** `GlobalExceptionHandler::GlobalExceptionHandler()` called `std::set_unexpected` which was removed in C++17
- **Fix:** Removed the call — no C++17 equivalent (dynamic exception specs were eliminated entirely)
- **Files modified:** `source/COMMON/exception.C`
- **Commit:** 446cf10

**2. [Rule 1 - Bug] Unscoped `streampos` not resolved under C++17**
- **Found during:** Task 1 (second build iteration)
- **Issue:** `streampos current_pos` relied on namespace injection that C++17 tightened
- **Fix:** Qualified as `std::streampos`
- **Files modified:** `source/FORMAT/SDFile.C`
- **Commit:** 446cf10

**3. [Rule 1 - Bug] Dynamic exception specification in parser source**
- **Found during:** Task 1 (third build iteration)
- **Issue:** `ExpressionParsererror` had `throw(Exception::ParseError)` on its declaration and definition
- **Fix:** Removed the dynamic exception spec from both sites
- **Files modified:** `source/KERNEL/expressionParserParser.y`
- **Commit:** 446cf10

## Known Stubs

None.

## Threat Flags

None — no new network endpoints, auth paths, file access patterns, or schema changes. Pure C++17 compliance fixes with identical runtime behaviour.

## Self-Check: PASSED

- source/COMMON/exception.C — set_unexpected call removed, file exists
- source/FORMAT/SDFile.C — std::streampos qualified, file exists
- source/KERNEL/expressionParserParser.y — throw() spec removed, file exists
- Commit 446cf10 — present in git log
- CI run 25862456769 — all 4 jobs green (verified via gh run view)
- Render smoke: macOS PASSED, Linux PASSED
- Grep gates: all exit 1 (zero matches)
