# Phase 3: Language Modernization - Context

**Gathered:** 2026-05-14
**Status:** Ready for planning

<domain>
## Phase Boundary

Move the whole BALL/VIEW/BALLView codebase to C++17: remove the load-bearing C++14
bridge, rewrite the C++17-removed STL constructs, and set the C++ standard the modern
CMake way.

**In scope:** bumping the C++ standard to 17, removing every C++17-removed construct
the build surfaces (the 7 known files are the confirmed starting point), and cleaning
up the CMake standard configuration.
**Out of scope:** Qt 6 migration (Phase 5), dependency-system overhaul (Phase 4),
wiring `test/` into ctest (Phase 9). Qt 5.15's own deprecated-API warnings are Phase 5's
concern, not Phase 3's.

Requirements: LANG-01, LANG-02, LANG-03.
</domain>

<decisions>
## Implementation Decisions

### Replacement idiom (removed C++17 constructs)
- **D-01:** Drop the `unary_function`/`binary_function` base-class inheritance
  **entirely** in `predicate.h`, `parsedFunction.h`, `standardPredicates.h`, and
  `residueRotamerSet.h`. Remove the `using std::unary_function;` / `using
  std::binary_function;` lines outright. Do **not** hand-roll `argument_type` /
  `result_type` typedefs to preserve them — they are almost never consumed in modern
  code, and the C++17 build will immediately flag any downstream `::argument_type`
  user if one exists.
- **D-02:** Rewrite the `bind2nd` / `mem_fun` / `mem_fun_ref` call sites
  (`enumerator.C:133`, `DNAMutator.C:247,258`, `hashGrid.h:1351`) as **lambdas** — not
  `std::mem_fn` / `std::bind`. The modern, readable idiom; no `<functional>` adapter
  cruft.

### CMake standard mechanism
- **D-03:** Bump the **global `CMAKE_CXX_STANDARD` 14 → 17** in `CMakeLists.txt:37`
  (`CMAKE_CXX_STANDARD_REQUIRED` is already `ON` at line 38). Use the global variable —
  it is set before any target is defined, so it applies project-wide. Do not switch to
  per-target `target_compile_features` — larger diff, no behavioural gain here.
- **D-04:** **Delete both raw `-std=` lines** from `cmake/BALLCompilerSpecific.cmake`
  (`-std=c++0x` at line 48, `-std=c++14` at line 132) — LANG-03 requires no raw `-std=`
  flags. Also remove/update the now-stale legacy comment block above line 132 that
  explains the C++14 bridge.
- **D-05:** Set **`CMAKE_CXX_EXTENSIONS OFF`** — force strict `-std=c++17` (not
  `gnu++17`). Maximizes cross-compiler consistency, which is the point of this
  milestone; surfaces any GNU-extension reliance now rather than on the Windows/MSVC
  build.

### Deprecation-warning policy
- **D-06:** **Remove the blanket `-Wno-deprecated-declarations`** from
  `BALLCompilerSpecific.cmake` — its original justification (old Eigen3's `binder2nd`
  spam) is obsolete now that the project builds against Eigen 5. Removing it surfaces
  C++17-relevant deprecation warnings (e.g. `std::not1` in `hashGrid.h:1351`).
  **Planner discretion:** if removing it floods the Phase 3 build log with Qt 5.15
  deprecation noise (genuinely Phase 5's scope), the planner may fall back to a
  narrower form — keep the suppression scoped to Qt headers only, or accept the noise.

### Sweep scope & done bar
- **D-07:** Phase 3 scope = **"the codebase compiles cleanly under C++17", full stop.**
  The 7 known files (`hashGrid.h`, `predicate.h`, `parsedFunction.h`,
  `residueRotamerSet.h`, `standardPredicates.h`, `enumerator.C`, `DNAMutator.C`) are
  the confirmed starting point — but anything else the C++17 build flags (e.g.
  `std::not1`, removed dynamic exception specifications `throw()`, `std::iterator`
  deprecation, `std::result_of`) gets fixed **in-phase**. LANG-01 is the real success
  bar.
- **D-08:** Done bar = **all 3 CI platforms green** on the C++17 commit. The Phase 02.2
  CI matrix (macOS-arm64 + Linux + Windows build, render smoke check, legacy-GL grep
  lint) is the regression net built for exactly this work — use it as the verification
  gate. Windows is still non-blocking (flips to required in Phase 4) but should be
  observed, not ignored.

### Claude's Discretion
- The exact lambda formulation at each rewritten call site (capture lists, parameter
  naming) — D-02 fixes the idiom, not the spelling.
- Whether to narrow vs. remove `-Wno-deprecated-declarations` if Qt noise floods (D-06).
- How to handle any *additional* C++17 breakage beyond the 7 files (D-07) — fix it
  in-phase using the same idioms (lambdas, modern STL); the planner sequences it.
</decisions>

<canonical_refs>
## Canonical References

**Downstream agents MUST read these before planning or implementing.**

### Phase definition
- `.planning/ROADMAP.md` §"Phase 3: Language Modernization" — goal, success criteria
- `.planning/REQUIREMENTS.md` §"Language Modernization" — LANG-01, LANG-02, LANG-03 wording

### Files to modify (the 7 known C++17-removed-construct sites)
- `include/BALL/CONCEPT/predicate.h:15-44` — `using std::unary_function/binary_function`; `UnaryPredicate`/`BinaryPredicate` base classes (D-01)
- `include/BALL/MATHS/parsedFunction.h:14,38` — `using std::unary_function`; base class (D-01)
- `include/BALL/KERNEL/standardPredicates.h:642` — `binary_function` base class (D-01)
- `include/BALL/STRUCTURE/residueRotamerSet.h:258` — `std::binary_function` base class (D-01)
- `include/BALL/DATATYPE/hashGrid.h:1351` — `std::mem_fun_ref` (removed in C++17) + `std::not1` (deprecated in C++17) (D-02)
- `source/CONCEPT/enumerator.C:133` — `std::bind2nd` (D-02)
- `source/STRUCTURE/DNAMutator.C:247,258` — `std::mem_fun` (D-02)

### CMake configuration
- `CMakeLists.txt:37-38` — `CMAKE_CXX_STANDARD 14` / `CMAKE_CXX_STANDARD_REQUIRED ON` — bump to 17, add `CMAKE_CXX_EXTENSIONS OFF` (D-03, D-05)
- `cmake/BALLCompilerSpecific.cmake:48,132` — the two raw `-std=` lines to delete; line ~127-132 stale C++14-bridge comment; line ~52 `-Wno-deprecated-declarations` to remove (D-04, D-06)

### Regression net
- `.planning/phases/02.2-ci-and-build-smoke-matrix/02.2-CONTEXT.md` — what the CI matrix does (the Phase 3 done-bar gate, D-08)
- `.github/workflows/ci.yml` — the actual CI matrix definition

No external specs or ADRs — requirements fully captured in the decisions above.
</canonical_refs>

<code_context>
## Existing Code Insights

### Reusable Assets
- Phase 02.2 CI matrix (`.github/workflows/ci.yml`): the ready-made 3-platform
  build + render-smoke + legacy-GL-lint gate — Phase 3's verification bar (D-08).
- `CMAKE_CXX_STANDARD` is already wired into `CMakeLists.txt` (a Phase 02.2 CI
  down-payment) — Phase 3 changes a value, not the mechanism.

### Established Patterns
- BALL uses per-directory `sources.cmake` files, not per-target `CMakeLists.txt`;
  headers are implicit, not listed (per STATE.md). Relevant if the planner considers
  per-target compile-feature wiring — D-03 deliberately avoids that.
- The C++14 bridge is explicitly documented (PROJECT.md, STATE.md, the
  `BALLCompilerSpecific.cmake` comment) as load-bearing and temporary — Phase 3 is
  the planned removal point.

### Integration Points
- Modified: 7 source/header files (the construct rewrites), `CMakeLists.txt`,
  `cmake/BALLCompilerSpecific.cmake`. No new files expected.
- The grep confirmed exactly 7 files match the LANG-02 construct list — no
  `bind1st`, `ptr_fun`, or `auto_ptr` occurrences exist in `include/` or `source/`.

### Constraints from prior phases
- gsd-tools has a recurring decimal-phase bug (STATE.md) — irrelevant to Phase 3
  (integer phase) but worth knowing if the planner runs `phase` ops.
- `CMAKE_CXX_STANDARD` must be set *early* in `CMakeLists.txt` — Phase 02.2's CI
  bring-up hit AppleClang running sub-C++14 feature-detection when the standard was
  set late. The standard is already early; keep it that way.
</code_context>

<specifics>
## Specific Ideas

- `hashGrid.h:1351` is the one site with two issues on one line: `std::mem_fun_ref`
  (hard removal in C++17 — must fix) and `std::not1` (deprecation only). Both should
  be folded into the lambda rewrite per D-02 rather than leaving `not1` behind.
- The stale C++14-bridge comment in `BALLCompilerSpecific.cmake` (~lines 127-131)
  should be removed, not just the flag — it would otherwise misdescribe the build.
</specifics>

<deferred>
## Deferred Ideas

None — discussion stayed within the Phase 3 boundary. (Qt 5.15 deprecated-API
cleanup is already a roadmap boundary: Phase 5, not deferred *here*.)
</deferred>

---

*Phase: 03-language-modernization*
*Context gathered: 2026-05-14*
