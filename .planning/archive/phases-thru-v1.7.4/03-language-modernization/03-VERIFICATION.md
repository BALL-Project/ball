---
phase: 03-language-modernization
verified: 2026-05-14T00:00:00Z
status: passed
score: 3/3 must-haves verified
overrides_applied: 0
re_verification: false
---

# Phase 3: Language Modernization — Verification Report

**Phase Goal:** The whole project compiles under C++17 with the standard set the modern CMake way, removing the load-bearing C++14 bridge.
**Verified:** 2026-05-14
**Status:** PASSED
**Re-verification:** No — initial verification

## Goal Achievement

### Observable Truths

| # | Truth | Status | Evidence |
|---|-------|--------|----------|
| 1 | The codebase compiles cleanly with C++17 and the C++14 bridge flag is gone | VERIFIED | CI run 25862456769 on commit 446cf10 — all 4 jobs green (macOS-arm64, linux-x64, windows-x64, lint); render smoke passed on macOS and Linux. Commit 446cf10 is on the current branch `v1.6-modernization`. `CMAKE_CXX_STANDARD 14` absent from CMakeLists.txt; no raw `-std=c++14` in cmake/. |
| 2 | No occurrences of `std::unary_function`, `binary_function`, `bind2nd`, `bind1st`, `ptr_fun`, `auto_ptr`, or `mem_fun` remain across the 7 known files | VERIFIED | `grep -rn 'unary_function\|binary_function\|bind2nd\|bind1st\|ptr_fun\|auto_ptr\|mem_fun' include/ source/` exits 1 (zero matches). Full-tree grep confirmed clean. |
| 3 | The C++ standard is configured via `CMAKE_CXX_STANDARD`/`target_compile_features`, with no raw `-std=` flags | VERIFIED | `grep -n 'CMAKE_CXX_STANDARD 17' CMakeLists.txt` matches line 33. `grep -rn 'std=c++' cmake/` exits 1 (zero matches). No `-Wno-deprecated-declarations` remains (exits 1). No stale C++14 or binder2nd comments remain. |

**Score:** 3/3 truths verified

### Required Artifacts

| Artifact | Expected | Status | Details |
|----------|----------|--------|---------|
| `include/BALL/CONCEPT/predicate.h` | UnaryPredicate/BinaryPredicate with no functional base class | VERIFIED | 68 lines, no `unary_function`/`binary_function`; classes declare with no `: public` base clause |
| `include/BALL/MATHS/parsedFunction.h` | ParsedFunction with no unary_function base | VERIFIED | 154 lines, no `unary_function` reference |
| `include/BALL/KERNEL/standardPredicates.h` | struct greater_ with no binary_function base | VERIFIED | 934 lines, no `binary_function` reference |
| `include/BALL/STRUCTURE/residueRotamerSet.h` | RotamerProbabilityGreaterThan_ with no binary_function base | VERIFIED | 311 lines, no `binary_function` reference |
| `include/BALL/DATATYPE/hashGrid.h` | countNonEmptyBoxes using lambda count_if predicate | VERIFIED | `count_if(box_.begin(), box_.end(), [](const HashGridBox3<Item>& box) { return !box.isEmpty(); })` confirmed at line 1350–1353 |
| `source/CONCEPT/enumerator.C` | transform call using lambda multiplier | VERIFIED | `std::transform(..., [modulus](Position m) { return m * modulus; })` confirmed at line 131–133 |
| `source/STRUCTURE/DNAMutator.C` | for_each select/deselect using lambdas | VERIFIED | Two `for_each` with `[](Atom* atom) { atom->select(); }` and `[](Atom* atom) { atom->deselect(); }` at lines 247 and 258 |
| `CMakeLists.txt` | CMAKE_CXX_STANDARD 17 global, strict, no-extensions | VERIFIED | Lines 33–35: `SET(CMAKE_CXX_STANDARD 17)`, `SET(CMAKE_CXX_STANDARD_REQUIRED ON)`, `SET(CMAKE_CXX_EXTENSIONS OFF)` — set early, before any target |
| `cmake/BALLCompilerSpecific.cmake` | No raw -std= flags, no stale C++14-bridge comment, no -Wno-deprecated-declarations | VERIFIED | All three grep gates exit 1 (zero matches). Only unrelated `-Wno-inconsistent-missing-override` remains (different concern) |
| `source/COMMON/exception.C` | std::set_unexpected call removed (C++17 additional fix) | VERIFIED | Line 333 is a comment explaining the removal; the call itself is absent |
| `source/FORMAT/SDFile.C` | std::streampos qualified (C++17 additional fix) | VERIFIED | `std::streampos current_pos = tellg();` at line 161 |
| `source/KERNEL/expressionParserParser.y` | throw(Exception::ParseError) dynamic spec removed | VERIFIED | `grep -n 'throw(' exits 1 (zero matches) |
| `.planning/phases/03-language-modernization/03-03-SUMMARY.md` | CI run URL and per-platform status recorded | VERIFIED | CI run https://github.com/BALL-Project/ball/actions/runs/25862456769 recorded with per-platform table; all 4 jobs green |

### Key Link Verification

| From | To | Via | Status | Details |
|------|----|-----|--------|---------|
| `CMakeLists.txt` (CMAKE_CXX_STANDARD 17) | every BALL/VIEW/BALLView target | Set before any target is defined — applies project-wide | VERIFIED | Standard declared at line 33, early in CMakeLists.txt before any `add_subdirectory` or target definition |
| C++17 commit (446cf10) | CI matrix 3-OS build + render smoke + lint | git push to v1.6-modernization | VERIFIED | CI run 25862456769 confirmed; commit present on `v1.6-modernization` branch |

### Data-Flow Trace (Level 4)

Not applicable. This phase modifies build configuration and C++ source constructs — no data-rendering components introduced. No dynamic data flow to trace.

### Behavioral Spot-Checks

| Behavior | Command | Result | Status |
|----------|---------|--------|--------|
| No removed STL constructs in source tree | `grep -rn 'unary_function\|...\|mem_fun' include/ source/` | exit 1, zero matches | PASS |
| CMAKE_CXX_STANDARD 17 present in CMakeLists.txt | `grep -n 'CMAKE_CXX_STANDARD 17' CMakeLists.txt` | matches line 33 | PASS |
| No raw -std= flags in cmake/ | `grep -rn 'std=c++' cmake/` | exit 1, zero matches | PASS |
| No -Wno-deprecated-declarations in cmake/ | `grep -rn 'Wno-deprecated-declarations' cmake/` | exit 1, zero matches | PASS |
| No stale C++14 or binder2nd comments | `grep -n 'C++14\|binder2nd' CMakeLists.txt cmake/BALLCompilerSpecific.cmake` | exit 1, zero matches | PASS |
| 3-platform CI green on C++17 commit | CI run 25862456769 (authoritative — all 4 jobs green per prompt) | macOS-arm64 GREEN, linux-x64 GREEN, windows-x64 GREEN, lint GREEN | PASS |
| Render smoke non-blank | CI run 25862456769 | macOS PASSED, Linux PASSED | PASS |

### Requirements Coverage

| Requirement | Source Plan | Description | Status | Evidence |
|-------------|------------|-------------|--------|----------|
| LANG-01 | 03-03-PLAN.md | Codebase compiles under C++17 (C++14 bridge flag removed) | SATISFIED | CI run 25862456769 all 4 jobs green on commit 446cf10; `CMAKE_CXX_STANDARD 14` absent; 3 additional C++17 breakages fixed in-phase |
| LANG-02 | 03-01-PLAN.md | All uses of C++17-removed constructs replaced across 7 known files | SATISFIED | Full-tree grep returns zero matches; all 7 files verified with lambda/base-class-drop rewrites |
| LANG-03 | 03-02-PLAN.md | C++ standard set via CMAKE_CXX_STANDARD, not raw -std= flags | SATISFIED | CMAKE_CXX_STANDARD 17 at CMakeLists.txt:33; no raw -std= anywhere under cmake/ |

All three requirement IDs from PLAN frontmatter (LANG-01, LANG-02, LANG-03) are accounted for and satisfied.

### Anti-Patterns Found

| File | Line | Pattern | Severity | Impact |
|------|------|---------|----------|--------|
| `source/COMMON/exception.C` | 333 | Comment explaining removal of `std::set_unexpected` (not a code stub) | Info | None — explanatory comment is correct and appropriate |

No functional stubs, placeholder implementations, hardcoded empty returns, or TODO/FIXME blockers found in phase-modified files.

### Human Verification Required

None. All success criteria are programmatically verifiable via grep gates and authoritative CI evidence. The CI run 25862456769 on commit 446cf10 is confirmed authoritative for criterion 1 (clean build + render smoke).

### Gaps Summary

No gaps. All three roadmap success criteria are fully satisfied:

1. **C++17 clean build + C++14 bridge gone** — confirmed by CI run 25862456769 (4 jobs green) on commit 446cf10, which is on the active branch. CMakeLists.txt has `CMAKE_CXX_STANDARD 17` and no `14` reference.

2. **Zero removed-construct occurrences** — confirmed by full-tree grep exiting 1. All 7 known files rewritten (4 with base-class drops, 3 with lambda rewrites), plus 3 additional C++17 breakages fixed in Plan 03 (set_unexpected removal, streampos qualification, dynamic exception spec removal).

3. **CMake-standard configuration, no raw flags** — `CMAKE_CXX_STANDARD 17` present at CMakeLists.txt:33 with REQUIRED and EXTENSIONS OFF; `grep -rn 'std=c++' cmake/` exits 1; `grep -rn 'Wno-deprecated-declarations' cmake/` exits 1.

---

_Verified: 2026-05-14_
_Verifier: Claude (gsd-verifier)_
