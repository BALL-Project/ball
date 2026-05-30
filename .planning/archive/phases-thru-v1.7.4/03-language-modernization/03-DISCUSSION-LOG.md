# Phase 3: Language Modernization - Discussion Log

> **Audit trail only.** Do not use as input to planning, research, or execution agents.
> Decisions are captured in CONTEXT.md — this log preserves the alternatives considered.

**Date:** 2026-05-14
**Phase:** 3-language-modernization
**Areas discussed:** Replacement idiom, CMake standard mechanism, Deprecation-warning policy, Sweep scope & done bar

---

## Replacement idiom

### Base-class inheritance replacement

| Option | Description | Selected |
|--------|-------------|----------|
| Drop base class entirely | Remove inheritance + `using std::unary_function/binary_function` lines outright; C++17 build catches any downstream `::argument_type` consumer | ✓ |
| Hand-roll typedefs inline | Replace inheritance with explicit `using argument_type = T;` aliases to preserve exact public API | |
| Investigate first, then decide | Grep for actual `::argument_type`/`::result_type` consumers before deciding per-file | |

**User's choice:** Drop base class entirely

### Call-site idiom

| Option | Description | Selected |
|--------|-------------|----------|
| Lambdas | Rewrite each `bind2nd`/`mem_fun`/`mem_fun_ref` call site as a lambda | ✓ |
| std::mem_fn / std::bind | Minimal-diff swap to the modern functional adapters | |

**User's choice:** Lambdas

---

## CMake standard mechanism

### Application across targets

| Option | Description | Selected |
|--------|-------------|----------|
| Global bump + delete raw flags | Bump `CMAKE_CXX_STANDARD` 14→17, delete both raw `-std=` lines from `BALLCompilerSpecific.cmake` | ✓ |
| Per-target compile_features | `target_compile_features(... cxx_std_17)` per library target, remove the global var | |

**User's choice:** Global bump + delete raw flags

### Compiler extensions

| Option | Description | Selected |
|--------|-------------|----------|
| Set CMAKE_CXX_EXTENSIONS OFF | Force strict `-std=c++17` (not `gnu++17`) for cross-compiler consistency | ✓ |
| Leave extensions ON (default) | CMake default `gnu++17`; defers GNU-extension discovery to the Windows/MSVC build | |

**User's choice:** Set CMAKE_CXX_EXTENSIONS OFF

---

## Deprecation-warning policy

| Option | Description | Selected |
|--------|-------------|----------|
| Remove it — Eigen justification is obsolete | Delete `-Wno-deprecated-declarations`; surfaces C++17 deprecation warnings; Qt 5.15 noise is a known caveat | |
| Remove it, but let planner scope if Qt noise floods | Remove the blanket suppression; planner may fall back to a narrower form if the build log becomes unusable | ✓ |
| Keep it as-is | Leave the suppression; lowest noise but hides relevant deprecations | |

**User's choice:** Remove it, but let planner scope if Qt noise floods

---

## Sweep scope & done bar

### Scope

| Option | Description | Selected |
|--------|-------------|----------|
| Fix whatever the C++17 build surfaces | Phase 3 = "compiles cleanly under C++17"; 7 files are the start, anything else gets fixed in-phase | ✓ |
| Strictly the 7 files + bump | Bound to exactly the 7 named files + the CMake change; other breakage becomes a follow-up | |
| 7 files + errors only, warnings deferred | Fix the 7 + hard errors; deprecation warnings logged as todos for later | |

**User's choice:** Fix whatever the C++17 build surfaces

### Done bar

| Option | Description | Selected |
|--------|-------------|----------|
| All 3 CI platforms green | Phase 02.2 CI matrix green on the C++17 commit (build + render smoke + lint, all 3 OSes) | ✓ |
| Clean macOS compile + CI follow-up | Done on a clean local macOS C++17 build; CI catches Linux/Windows fallout asynchronously | |

**User's choice:** All 3 CI platforms green

---

## Claude's Discretion

- Exact lambda formulation at each rewritten call site (capture lists, parameter naming).
- Whether to narrow vs. fully remove `-Wno-deprecated-declarations` if Qt deprecation noise floods the Phase 3 build log.
- How to sequence and fix any C++17 breakage beyond the 7 known files.

## Deferred Ideas

None — discussion stayed within the Phase 3 boundary.
