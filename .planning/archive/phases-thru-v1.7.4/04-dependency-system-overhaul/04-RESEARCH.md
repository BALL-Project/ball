# Phase 4: Dependency System Overhaul - Research

**Researched:** 2026-05-14
**Domain:** CMake build-system modernization (config-mode `find_package`, `CMakePresets.json`, vcpkg manifest), OpenBabel 2.x→3.x C++ API port
**Confidence:** HIGH on codebase state and config-mode migration; MEDIUM-HIGH on vcpkg (port availability verified, full Windows green build is inherently iterative); HIGH on OpenBabel port surface (grepped exhaustively)

## Summary

Phase 4 is a CMake-centric phase with one source-touching island (the OpenBabel 3.x API port). The codebase is in good shape for it: Boost is already config-mode (`cmake/BALLConfigBoost.cmake` is the reference pattern), C++17 is set project-wide, and the Phase 02.2 CI matrix already builds all 3 OSes green (Windows even builds clean today, non-blocking). The work splits cleanly into independent streams: (1) the 3 broken finders, (2) `ball_contrib` de-referencing, (3) `CMakePresets.json` + min-version pins, (4) the `vcpkg.json` manifest + driving Windows CI to a *required* green, and (5) the OpenBabel 3.x port.

The two highest-risk items are well-bounded. **vcpkg:** the verified blocker is that `openbabel`, `lp_solve`, `flex`, and `bison` are **not** vcpkg ports — Qt5, Boost, Eigen3, FFTW3, GLEW, TBB, libSVM all are. The realistic Windows path is a `vcpkg.json` covering the available set, plus a non-vcpkg provisioning step for flex/bison (`choco install winflexbison3`) and a decision to leave OpenBabel/lp_solve OFF on Windows (both are Optional per the feature matrix — D-06 allows this). **OpenBabel:** the API port surface is exactly 5 files (`molecularSimilarity.{h,C}` is the core; 4 TOOLS files consume it). `hybridisationProcessor.C` only mentions OpenBabel in comments — no actual API usage. The 2.x→3.x breaking changes that hit this code are narrow and mechanical: `BeginModify`/`EndModify`, the element/valence method renames, implicit-hydrogen handling, and aromaticity-perception flag changes.

**Primary recommendation:** Plan 5 waves. Wave 0: the 3 finder fixes + `ball_contrib` de-referencing + `CMakePresets.json` + min-version pins (all CMake, mutually independent, verifiable on macOS/Linux immediately via existing CI). Wave 1: the OpenBabel 3.x port (its own wave — source-touching, sequenced so a failed port can't block the rest; OpenBabel stays auto-detected/Optional). Wave 2: `vcpkg.json` + Windows CI bring-up to green, then flip `blocking: true`. FEAT-01 feature-matrix confirmation is a doc task foldable into Wave 0.

## User Constraints (from CONTEXT.md)

### Locked Decisions

- **D-01:** Phase 4 authors `vcpkg.json` (manifest mode) + wires the vcpkg toolchain file, **and drives the Windows CI job to green** — then flips it from non-blocking (`continue-on-error`) to a required check. DEPS-02's "complete Windows dependency set" means complete enough that Windows actually builds, not just a manifest file on disk. This is the designated phase for it (per Phase 02.2 D-03).
- **D-02:** vcpkg is the Windows path only; macOS/Linux stay on Homebrew/system packages. Do **not** adopt vcpkg as a cross-platform fallback — CI must keep testing the exact path contributors follow (`BUILD-macos.md`).
- **D-03:** Use config-mode `find_package` wherever upstream ships a CMake config. Keep thin finder shims **only** for deps with no upstream config (lp_solve, libSVM).
- **D-04:** Fix the 3 finders CONCERNS.md flags as broken, **in-phase**: `cmake/FindOpenBabel2.cmake` (detect OpenBabel 3.x), `cmake/FindTBB.cmake` (oneTBB — `find_package(TBB CONFIG)`, drop the `<2017` version gate), `cmake/FindEigen3.cmake` (read the version from `Eigen/Version` for Eigen 5+).
- **D-05:** **OpenBabel and TBB are both enabled in 1.6.** TBB: fix the finder, turn on (oneTBB). OpenBabel: port BALL's integration to the OpenBabel **3.x** API and enable it. They remain classified **Optional** (auto-detected, enabled when found; absence disables only their own features) — "ON" means enabled-by-default-when-present, not promoted to Required.
- **D-06:** FEAT-01 feature matrix classifies every optional dependency as required / optional / removed / deferred with the user-visible impact of absence. A missing optional dep = auto-disable with a clear configure-time message, **not** a hard error. Confirm/update the matrix already drafted in `REQUIREMENTS.md`.
- **D-07:** Ship 4 named presets: `macos-homebrew`, `linux-system`, `windows-vcpkg`, `ci`. Release-focused; structure the JSON so Debug variants can be added later without a rewrite (mirrors Phase 02.2's Release-only CI choice).
- **D-08:** Pin minimum dependency versions in CMake (`CMakeLists.txt` / `find_package` calls), documented alongside the presets.
- **D-09:** Remove every `ball_contrib` reference from `ball/`'s CMake — search paths, finder hints, contrib-build glue — so it is fully off the build path. (`ball_contrib` is a separate sibling repo; this phase de-references it, it does not delete it.)

### Claude's Discretion

- Exact vcpkg triplet(s) for Windows, preset cache-variable layout, finder shim style.
- Whether min-version pins live as `find_package(... X.Y)` calls vs a central version block — planner decides.
- How to sequence the OpenBabel 3.x port within the phase (likely its own plan/wave — it is the one source-touching item in an otherwise CMake-centric phase).

### Deferred Ideas (OUT OF SCOPE)

None deferred in this phase's discussion. Pre-existing roadmap boundaries (NOT Phase 4 scope): Qt 6 migration (Phase 5), wiring `test/` into ctest (Phase 9), networking-dependency rework (backlog 999.3), Python/SIP (Phase 6), Ninja generator (backlog 999.2).

## Phase Requirements

| ID | Description | Research Support |
|----|-------------|------------------|
| DEPS-01 | `ball_contrib` removed from build path; CMake finds deps via system/Homebrew | "ball_contrib De-Referencing" section — full removal surface enumerated (6 sites across 3 cmake files + `CMakeLists.txt`) |
| DEPS-02 | `vcpkg.json` manifest provides the Windows dependency set | "vcpkg / Windows" section — verified port availability, triplet recommendation, flex/bison + OpenBabel/lp_solve workarounds, CI bring-up path |
| DEPS-03 | Stale bundled `Find*.cmake` replaced with config-mode `find_package` where upstream provides it | "Config-Mode Finder Migration" section — config availability confirmed for TBB/Eigen3/OpenBabel; lp_solve/libSVM keep shims |
| DEPS-04 | Minimum dependency versions pinned and documented | "Minimum Version Pinning" section — recommended pins per dep with rationale |
| DEPS-05 | `CMakePresets.json` provides stable configure presets for the 4 named environments | "CMakePresets.json" section — preset structure, cache-var layout, Debug-extensibility pattern |
| FEAT-01 | Feature matrix classifies every optional dependency + states absence impact | "Feature Matrix (FEAT-01)" section — confirms/updates the REQUIREMENTS.md draft |

## Current Build-System State (verified by code inspection)

### Dependency wiring in `CMakeLists.txt`

| Dep | Current mechanism | Lines | Config-mode available upstream? |
|-----|-------------------|-------|----------------------------------|
| Boost | **config-mode already** via `cmake/BALLConfigBoost.cmake` (`FIND_PACKAGE(Boost 1.55 REQUIRED COMPONENTS ...)`, `CMP0167 NEW`) | 153 | Yes — already done. Reference pattern for D-03. |
| Eigen3 | bundled `cmake/FindEigen3.cmake` (module-mode) | 359-361 | **Yes** — Eigen 3.3+ ships `Eigen3Config.cmake` → `Eigen3::Eigen` target |
| TBB | bundled `cmake/FindTBB.cmake` (pre-oneTBB, broken) | 226-256 | **Yes** — oneTBB ships `TBBConfig.cmake` → `TBB::tbb` target |
| OpenBabel | bundled `cmake/FindOpenBabel2.cmake` (pkg-config `openbabel-2.0`, broken) | 186-193 | **Yes** — OpenBabel 3.x ships `OpenBabel3Config.cmake` → `openbabel` target |
| FFTW | bundled `cmake/FindFFTW.cmake` (GPL builds only) | 172-176 | Partial — upstream FFTW ships `FFTW3Config.cmake` but naming is inconsistent; keep the bundled finder (out of D-04's 3, low priority) |
| GLEW | CMake's **own** `FindGLEW` (module) → already prefers `GLEW::GLEW` target (line 580) | 378-384 | CMake ships `FindGLEW` that produces the imported target; no change needed |
| lp_solve | bundled `cmake/FindLPSolve.cmake` | 201-205 | **No upstream config** — keep thin shim (D-03) |
| libSVM | bundled `cmake/FindlibSVM.cmake` | 210-216 | **No upstream config** — keep thin shim (D-03) |
| Qt5 | CMake config-mode already (`FIND_PACKAGE(Qt5 ...)`) | 300-344 | Yes — already config-mode; Phase 5 owns the Qt6 move |
| flex/bison | CMake's own `FindFLEX`/`FindBISON` (+ `cmake/FindBISON.cmake` override, `cmake/FindFLEX.cmake`) | 121-146 | Build tools, not link deps — keep as-is |

**Key insight:** Only 3 finders are in D-04's scope (Eigen3, TBB, OpenBabel2). Boost is the proof the pattern works. GLEW/FFTW/flex/bison are out of scope. lp_solve/libSVM explicitly keep shims.

### `ball_contrib` reference inventory (D-09 removal surface)

`grep` across `*.cmake`, `CMakeLists.txt`, `*.in` found these **active** references (planning docs don't count):

| File | Line(s) | What it does |
|------|---------|--------------|
| `CMakeLists.txt` | 57-73 | `SET(BALL_CONTRIB_PATH ...)` cache var + the whole `IF(BALL_CONTRIB_PATH)` block (adds `include/`, prepends `CMAKE_PREFIX_PATH`, sets `CMAKE_REQUIRED_INCLUDES`) |
| `CMakeLists.txt` | 414-421 | `IF(BALL_CONTRIB_PATH)` Python `PYTHONPATH` injection inside `BALL_PYTHON_SUPPORT` block |
| `CMakeLists.txt` | 440 | `FIND_PROGRAM(MACDEPLOYQT_EXECUTABLE macdeployqt PATHS ${BALL_CONTRIB_PATH}/bin)` |
| `CMakeLists.txt` | 448 | `FIND_PROGRAM(WINDEPLOYQT_EXECUTABLE windeployqt.exe PATHS ${BALL_CONTRIB_PATH}/bin)` |
| `cmake/BALLInstallScripts.cmake.in` | 23 | `SET(DEPENDENCY_PATHS ${BALL_CONTRIB_PATH}/dlls ${QT_BINARY_DIR})` — Windows install-script glue |
| `cmake/FindSIP.cmake` | 29 | Comment only — but the file is SIP/Python (Phase 6 scope); leave the comment or trim it |
| `cmake/BALLFinishedMessage.cmake` | 29 | Echo string "Make sure to add the BALL and contrib lib/ path" — stale user message, update text |

`grep` for `BALL_CONTRIB` confirms `BALL_CONTRIB_PATH` is the only variable. **Removal approach:** delete the cache var and all `IF(BALL_CONTRIB_PATH)` blocks; `macdeployqt`/`windeployqt` `FIND_PROGRAM` calls keep their default search (drop the `PATHS ${BALL_CONTRIB_PATH}/bin` arg — vcpkg/Homebrew put these on PATH); fix `BALLInstallScripts.cmake.in` (`DEPENDENCY_PATHS` drops the contrib entry — vcpkg `applocal`/`windeployqt` handles DLLs); update the stale message string. The `BUILD-macos.md` and `CLAUDE.md` mentions are documentation (already say "NOT used") — leave or tidy.

### CI state (`.github/workflows/ci.yml`)

Single `build` job, `strategy.matrix.include` per OS. Windows entry today: `blocking: false`, `smoke: false`, a "best-effort" `cmake ... || true` step with **no dependency provisioning** (expected to fail/no-op). D-01's flip is a one-line matrix change (`blocking: true`) **plus** real provisioning steps. macOS/Linux jobs mirror `BUILD-macos.md` verbatim — presets must not diverge from that path (D-02).

## Standard Stack

This phase doesn't add libraries — it modernizes how existing ones are *found*. The "stack" here is the build-system tooling and the verified dependency versions.

### Build tooling

| Tool | Version | Purpose | Why standard |
|------|---------|---------|--------------|
| CMake | 3.21+ recommended (repo min is 3.5; presets v3+ need 3.21) | `CMakePresets.json` schema v3+, config-mode `find_package` | [CITED: cmake.org/cmake/help/latest/manual/cmake-presets.7.html] Presets v3 (CMake 3.21) added `toolchainFile` support — needed for the vcpkg toolchain. The repo's `CMAKE_MINIMUM_REQUIRED(VERSION 3.5...3.31)` allows 3.21+. |
| vcpkg | manifest mode, latest (2026.04.27 release; 2807 ports) | Windows dependency provisioning | [VERIFIED: github.com/microsoft/vcpkg releases] Manifest mode (`vcpkg.json` + `CMAKE_TOOLCHAIN_FILE=.../vcpkg.cmake`) is the current standard. |
| ccache | already in CI | Compiler cache | Already wired in Phase 02.2; presets should keep `CMAKE_CXX_COMPILER_LAUNCHER` settable. |

### Verified dependency versions (for D-04 / D-08)

| Dep | Homebrew (macOS) | vcpkg port | Ships CMake config? | Notes |
|-----|------------------|------------|---------------------|-------|
| Qt5 | `qt@5` → 5.15.x | `qt5-base` **5.15.18** [VERIFIED: vcpkg registry] | Yes | Qt6 is Phase 5 |
| Boost | `boost` → 1.8x/1.9x | `boost` (umbrella) **available** [VERIFIED] | Yes (`BoostConfig.cmake`) | Already config-mode |
| Eigen3 | `eigen` → 3.4 / 5.x | `eigen3` **5.0.1** [VERIFIED: vcpkg registry] | Yes (`Eigen3Config.cmake`, target `Eigen3::Eigen`) | Eigen 5.0 moved version macros to `Eigen/Version` |
| FFTW | `fftw` → 3.3.10 | `fftw3` **3.3.10** [VERIFIED: vcpkg registry] | Yes-ish | GPL builds only; bundled finder kept |
| GLEW | `glew` → 2.x | `glew` **2.3.1** [VERIFIED: vcpkg registry] | Via CMake's `FindGLEW` | No change |
| OpenBabel | `open-babel` → **3.1.1** | **NOT a vcpkg port** [VERIFIED: HTTP 404 on `openbabel`/`open-babel`] | Yes (`OpenBabel3Config.cmake`) on Homebrew | Windows: OpenBabel OFF (Optional) |
| TBB (oneTBB) | `tbb` → 2021.x/2022.x | `tbb` **2022.3.0** (oneTBB) [VERIFIED: vcpkg registry] | Yes (`TBBConfig.cmake`, target `TBB::tbb`) | Drop the `<2017` gate |
| lp_solve | `lp_solve` → 5.5.x | **NOT a vcpkg port** [VERIFIED: HTTP 404 on `lp-solve`/`lpsolve`/`coin-or-lpsolve`] | No | Windows: lp_solve OFF (Optional). Linux already OFF (non-PIC `.a`). |
| libSVM | `libsvm` → 3.x | `libsvm` **3.35** [VERIFIED: vcpkg registry] | No upstream config — keep `cmake/FindlibSVM.cmake` shim | |
| flex | `flex` (keg-only) | **NOT a vcpkg port** [VERIFIED: HTTP 404 on `flex`/`winflexbison`/`winflexbison3`] | Build tool | Windows: provision via `choco install winflexbison3` |
| bison | `bison` (keg-only) | **NOT a vcpkg port** [VERIFIED] | Build tool | Windows: `choco install winflexbison3` (provides `win_flex`/`win_bison`) |

## Architecture Patterns

### Pattern 1: Config-mode `find_package` migration (D-03/D-04)

**What:** Replace a bundled `cmake/FindXxx.cmake` module with a call to upstream's own `XxxConfig.cmake`, consumed as an imported target.
**Reference (already in-repo):** `cmake/BALLConfigBoost.cmake` — `FIND_PACKAGE(Boost 1.55 REQUIRED COMPONENTS ...)` with `CMP0167 NEW`, then exposes `Boost::*` targets via `Boost_LIBRARIES`.

**Eigen3 migration:**
```cmake
# CMakeLists.txt — replace lines 359-361
# Old: SET(EIGEN3_VERSION_OK "3.0.0"); FIND_PACKAGE(Eigen3 REQUIRED); INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})
FIND_PACKAGE(Eigen3 3.4 REQUIRED NO_MODULE)   # NO_MODULE / CONFIG forces upstream Eigen3Config.cmake
# then link the imported target instead of INCLUDE_DIRECTORIES:
#   TARGET_LINK_LIBRARIES(BALL ... Eigen3::Eigen)
```
This deletes `cmake/FindEigen3.cmake` entirely (the `Eigen/Version` reading is then upstream's problem — Eigen3Config.cmake handles 3.4 and 5.x). [CITED: eigen.tuxfamily.org docs — Eigen ships `Eigen3Config.cmake` since 3.3]

**TBB migration:**
```cmake
# CMakeLists.txt — replace lines 226-256
FIND_PACKAGE(TBB CONFIG)        # oneTBB ships TBBConfig.cmake
IF(TBB_FOUND)
  SET(BALL_HAS_TBB TRUE)
  # link TBB::tbb on the BALL target — NO more INCLUDE_DIRECTORIES / version<2017 gate
ENDIF()
```
The `<2017` version gate (line 244) and the entire `cmake/FindTBB.cmake` are deleted. [CITED: github.com/oneapi-src/oneTBB — oneTBB ships `TBBConfig.cmake` exporting `TBB::tbb`]

**OpenBabel migration:**
```cmake
# CMakeLists.txt — replace lines 186-193 (and move out of the GPL-only block — see Pitfall 4)
FIND_PACKAGE(OpenBabel3 CONFIG)   # Homebrew open-babel 3.1.1 ships OpenBabel3Config.cmake; target: openbabel
IF(OpenBabel3_FOUND)
  SET(BALL_HAS_OPENBABEL TRUE)
ENDIF()
```
`cmake/FindOpenBabel2.cmake` is deleted. Note the package name is `OpenBabel3` and the imported target is `openbabel` (lowercase). [CITED: openbabel.org/api/3.0 — OpenBabel 3.x build installs `OpenBabel3Config.cmake`]

**Anti-pattern to avoid:** Keeping `INCLUDE_DIRECTORIES(${XXX_INCLUDE_DIRS})` + raw `${XXX_LIBRARIES}` lists after migrating. Config-mode's whole point is the imported target carries includes/flags transitively — use `TARGET_LINK_LIBRARIES(BALL Eigen3::Eigen)` etc. The repo's `BALL_DEP_LIBRARIES` list pattern still works (append the target name), but prefer the target over the legacy `_INCLUDE_DIRECTORIES` calls.

### Pattern 2: Thin finder shim (lp_solve, libSVM — D-03)

These have **no upstream CMake config**. Keep `cmake/FindLPSolve.cmake` and `cmake/FindlibSVM.cmake` but they should be *thin* — header find + library find + `FindPackageHandleStandardArgs`. The existing `FindlibSVM.cmake` is already roughly this shape (it's a clean BSD-licensed third-party finder). `FindLPSolve.cmake` is more elaborate (link-test logic for the `dl`/`colamd` extra libs) — that complexity is load-bearing on Linux, keep it. No migration needed; just confirm they still work and document that they are the deliberate exceptions to D-03.

### Pattern 3: `CMakePresets.json` with inheritance (D-07)

**What:** A base hidden preset holds common cache vars; the 4 named presets inherit and add platform specifics. A `ci` preset inherits from the platform presets. Structure for later Debug variants via `configurePreset` + separate `buildPreset` with `configuration`, OR a `*-debug` configure preset that `inherits` the release one and overrides `CMAKE_BUILD_TYPE`.

```jsonc
{
  "version": 3,                       // v3 = CMake 3.21+, needed for toolchainFile
  "cmakePresets": [
    {
      "name": "base", "hidden": true,
      "binaryDir": "${sourceDir}/build/${presetName}",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release",
        "CMAKE_POLICY_VERSION_MINIMUM": "3.5",
        "BALL_PYTHON_SUPPORT": "OFF",
        "USE_RTFACT": "OFF",
        "BALL_HAS_VIEW": "ON"
      }
    },
    {
      "name": "macos-homebrew", "inherits": "base",
      "cacheVariables": {
        "CMAKE_PREFIX_PATH": "/opt/homebrew/opt/qt@5;/opt/homebrew",
        "BISON_EXECUTABLE": "/opt/homebrew/opt/bison/bin/bison",
        "FLEX_EXECUTABLE": "/opt/homebrew/opt/flex/bin/flex"
      }
    },
    {
      "name": "linux-system", "inherits": "base",
      "cacheVariables": { "USE_LPSOLVE": "OFF" }   // non-PIC liblpsolve55.a on Ubuntu
    },
    {
      "name": "windows-vcpkg", "inherits": "base",
      "toolchainFile": "$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake",
      "cacheVariables": {
        "VCPKG_TARGET_TRIPLET": "x64-windows",
        "USE_LPSOLVE": "OFF", "BALL_HAS_OPENBABEL": "OFF"
      }
    },
    { "name": "ci", "inherits": "macos-homebrew", "cacheVariables": {
        "CMAKE_C_COMPILER_LAUNCHER": "ccache", "CMAKE_CXX_COMPILER_LAUNCHER": "ccache" } }
  ]
}
```
**Note** the `ci` preset can't cleanly inherit *all three* platforms at once — discretion item. Two viable shapes: (a) `ci` inherits `base` and CI passes `--preset ci` then layers platform cache-vars per-runner, or (b) three CI presets `ci-macos`/`ci-linux`/`ci-windows` each inheriting their platform preset + adding ccache. Option (b) is cleaner and matches the matrix. The planner decides; both keep Debug-extensibility (add `*-debug` presets inheriting the release ones, overriding `CMAKE_BUILD_TYPE`).

### Pattern 4: vcpkg manifest mode (D-01/D-02)

**What:** A `vcpkg.json` at repo root declares dependencies; CMake's vcpkg toolchain file auto-installs them at configure time. Windows-only — macOS/Linux ignore it (they don't pass the toolchain file).

```jsonc
{
  "$schema": "https://raw.githubusercontent.com/microsoft/vcpkg-tool/main/docs/vcpkg.schema.json",
  "name": "ball",
  "version-string": "1.6.0-dev",
  "builtin-baseline": "<pinned vcpkg commit SHA>",
  "dependencies": [
    { "name": "qt5-base" },
    { "name": "qt5-tools" },
    "boost-chrono", "boost-date-time", "boost-iostreams",
    "boost-regex", "boost-serialization", "boost-thread", "boost-asio",
    "eigen3", "fftw3", "glew", "tbb", "libsvm"
  ]
}
```
**NOT in the manifest** (verified absent from vcpkg): `openbabel`, `lp-solve`, `flex`, `bison`. Handle out-of-band:
- **flex/bison:** GitHub Actions step `choco install winflexbison3` → provides `win_flex.exe`/`win_bison.exe`; pass `-DFLEX_EXECUTABLE=.../win_flex.exe -DBISON_EXECUTABLE=.../win_bison.exe`. (The repo already has Windows bison handling at `CMakeLists.txt:118-129` via a `BALL-bison.bat` — the planner should confirm whether winflexbison3's `win_bison` works directly or needs that wrapper.)
- **OpenBabel + lp_solve:** leave OFF on Windows via the `windows-vcpkg` preset cache vars. Both are **Optional** in the feature matrix (D-06) — a Windows build without them is a valid green build. This is the pragmatic path to "Windows actually builds" (D-01).

**Triplet recommendation:** `x64-windows` (dynamic linkage). Rationale: BALL builds `libBALL`/`libVIEW` as **shared** libraries (`BUILD_SHARED_LIBS TRUE`, `CMakeLists.txt:489`) and Qt5 is dynamic — `x64-windows` (DLLs) matches. `x64-windows-static` would fight the shared-lib model. Discretion item per CONTEXT.md, but `x64-windows` is the low-friction choice.

## Don't Hand-Roll

| Problem | Don't build | Use instead | Why |
|---------|-------------|-------------|-----|
| Finding Eigen3/TBB/OpenBabel | A new bespoke `Find*.cmake` module | Upstream `*Config.cmake` via `find_package(... CONFIG)` | Upstream configs track their own version/layout changes — the whole reason the 3 bundled finders broke (Eigen moved its version header, oneTBB restructured, OpenBabel bumped major). D-03/D-04 mandate this. |
| Windows dependency provisioning | Hand-curated DLL bundles, reviving `ball_contrib` | vcpkg manifest mode | `ball_contrib`'s ~2016 tarballs don't build on modern toolchains (the entire premise of this milestone). vcpkg is the maintained path. |
| Per-platform configure invocations | Shell scripts with long `cmake` arg lists | `CMakePresets.json` | DEPS-05 exists precisely to kill the "bespoke per-platform configure invocation" — `BUILD-macos.md`'s 8-line cmake command becomes `cmake --preset macos-homebrew`. |
| OpenBabel molecule round-tripping | Reimplementing SMILES/format conversion | OpenBabel 3.x `OBConversion` | BALL's `MolecularSimilarity` already wraps it correctly — the port is an API-rename exercise, not a rewrite. |

**Key insight:** Every broken finder in this phase broke because it hand-rolled detection logic that upstream's own config file would have gotten right. The phase's central move is "stop hand-rolling detection."

## OpenBabel 2.x → 3.x API Port (D-05)

### Port surface — exactly 5 files (grep-verified)

| File | OpenBabel API used | Port effort |
|------|--------------------|-------------|
| `include/BALL/STRUCTURE/molecularSimilarity.h` | `#include <openbabel/mol.h>`; `OpenBabel::OBMol` in 3 signatures | Trivial — include + type names unchanged |
| `source/STRUCTURE/molecularSimilarity.C` | **The core.** `OBMol`, `OBAtom`, `OBBond`, `OBConversion`, `OBSmartsPattern`, `OBAtomIterator`, `OBBondIterator`; methods: `NewAtom/NewBond`, `SetAtomicNum/SetVector/SetFormalCharge/SetBondOrder`, `SetBegin/SetEnd`, `AddBond`, `SetAromaticPerceived`, `GetAtomicNum/GetPartialCharge/GetFormalCharge/GetX/GetY/GetZ`, `GetBeginAtom/GetEndAtom/GetBondOrder`, `BeginAtoms/EndAtoms/BeginBonds/EndBonds`; includes `obconversion.h parsmart.h atom.h` | **Bulk of the work** — see breaking-changes list below |
| `source/APPLICATIONS/TOOLS/MolDepict.C` | `OBConversion`, `MolecularSimilarity::createOBMol` | Small — `OBConversion` API stable |
| `source/APPLICATIONS/TOOLS/ProteinProtonator.C` | `OBMol`, `OBConversion`, iterates atoms | Small |
| `source/APPLICATIONS/TOOLS/Ligand3DGenerator.C` | `OBMol`, `OBBuilder`, `OBForceField`, `OBPlugin`, `OBFFConstraints`, `OBAtomIterator`; `AddHydrogens`, `SetDimension`, `GetIdx`, `GetVector`, `pFF->Setup` | Medium — `OBPlugin`/`OBForceField`/`OBBuilder` API |

**`source/STRUCTURE/hybridisationProcessor.C`:** mentions OpenBabel only in **comments** ("reimplementation of the openbabel code") — **no actual API usage**. Not a port target. The `sources.cmake` files in `STRUCTURE/` and `TOOLS/` have `BALL_HAS_OPENBABEL` guards — confirm those still gate compilation correctly.

### OpenBabel 2.x→3.x breaking changes that hit this code

[CITED: open-babel.readthedocs.io/en/latest/UseTheLibrary/migration.html] — only the changes relevant to the symbols above:

| Change | 2.x | 3.x | Where it bites |
|--------|-----|-----|----------------|
| **Bond order accessors** | `GetBO()` / `SetBO()` | `GetBondOrder()` / `SetBondOrder()` | `molecularSimilarity.C` already uses `SetBondOrder`/`GetBondOrder` — **may already be 3.x-clean here**; verify no `GetBO` slipped in elsewhere |
| **Valence/degree renames** | `GetValence()`, `BOSum()`, `GetHvyValence()` | `GetExplicitDegree()`, `GetExplicitValence()`, `GetHvyDegree()` | grep the 5 files for these — none seen in the grep sample, but verify |
| **Element table** | `OBElementTable`, `etab.GetSymbol()`, `IsCarbon()` etc. | `OBElements::GetSymbol()` via `#include <openbabel/elements.h>`; use `GetAtomicNum()` comparisons | grep for `OBElementTable`/`etab`/`IsCarbon` — appears unused in the sampled code but must be confirmed |
| **Implicit hydrogens** | implicit H count derived | stored as atom property; `SetImplicitHCount()`/`GetImplicitHCount()`; must adjust manually when adding/removing bonds | `createOBMol` builds molecules bond-by-bond — **this is the subtle one**. After `NewBond`/`AddBond`, implicit-H counts may need explicit management or an `obmol->AddHydrogens()` call. `Ligand3DGenerator.C` already calls `AddHydrogens(false,true,pH)`. |
| **Aromaticity perception** | molecule modification cleared aromaticity flags | modification no longer auto-clears; call `SetAromaticPerceived(false)` to force reperception | `molecularSimilarity.C:238` already calls `SetAromaticPerceived(...)` — confirm the polarity (it sets perceived *true* after manually setting bond orders; 3.x semantics differ) |
| **`BeginModify()`/`EndModify()`** | wrapped batch edits | still exist but perception behavior changed | `createOBMol` builds atoms+bonds — check whether it brackets with `BeginModify/EndModify`; 3.x may need explicit `obmol->PerceiveBondOrders()` or careful flag handling |
| **Header trims** | `mol.h` pulled in many headers transitively | misc headers removed from `mol.h`/`atom.h`/`bond.h` | may need to add explicit `#include <openbabel/obiter.h>`, `#include <openbabel/elements.h>`, `#include <openbabel/bond.h>` etc. — compiler will flag these |
| **`babel` → `obabel`** | `babel` CLI | `obabel` CLI | Not a code change — only relevant if any script/test shells out to `babel` |

**Confidence:** HIGH on the port *surface* (exhaustive grep). MEDIUM on exact per-line edits — the implicit-hydrogen and aromaticity-perception semantics are the items most likely to need a build-and-test loop. This is why CONTEXT.md D-05 + Specifics sequence the port as **its own wave** that can't block the rest.

### Verification of the OpenBabel port

The port is verified by: (1) `BALL_HAS_OPENBABEL=ON` configures and `libBALL`/`libVIEW` + the 4 TOOLS compile and link against Homebrew `open-babel` 3.1.1 on macOS CI; (2) ideally a smoke check that `MolecularSimilarity::generateCanSmile` produces a non-empty canonical SMILES for a known molecule (round-trip correctness — implicit-H bugs would corrupt this). The repo's `test/` tree has a `MolecularSimilarity_test.C` candidate but `test/` is `EXCLUDE_FROM_ALL` until Phase 9 — so the realistic Phase 4 gate is **compile + link green on all platforms that have OpenBabel** (macOS + Linux; Windows has it OFF).

## Common Pitfalls

### Pitfall 1: OpenBabel trapped inside the GPL-only block
**What goes wrong:** `FIND_PACKAGE(OpenBabel2)` currently sits **inside** `IF(${BALL_LICENSE_GPL})` (`CMakeLists.txt:164-195`). With the default `BALL_LICENSE=LGPL`, OpenBabel is *never even searched for*. D-05 says "OpenBabel enabled in 1.6" — that's impossible while it's GPL-gated.
**How to avoid:** Move the OpenBabel `find_package` block **out** of the `IF(BALL_LICENSE_GPL)` block. OpenBabel itself is GPL-licensed, so there may be a deliberate licensing reason it was gated — the planner/discuss should confirm whether OpenBabel-as-an-optional-runtime-dep is acceptable under BALL's LGPL build (it's a separate shared library, dynamically linked, behind `BALL_HAS_OPENBABEL`). [ASSUMED] The intent of D-05 ("enabled in 1.6") implies this gating is to be removed — but the LGPL/GPL licensing interaction is a real question, flagged in the Assumptions Log.
**Warning signs:** OpenBabel "not found" in configure output even though `brew list open-babel` shows it installed.

### Pitfall 2: Windows CI bring-up is iterative — budget for it
**What goes wrong:** Phase 02.2's CI bring-up took 2 iterations *after the workflow landed*. The Windows job has **never** had real dependency provisioning — first-contact issues (Qt5 deployment, MSVC flags, the `BALL-bison.bat` wrapper, `/NODEFAULTLIB:libcmt` link flags at `CMakeLists.txt:574`, `BALL_BUILD_DLL` export macros) are likely.
**How to avoid:** Sequence vcpkg/Windows as the **last wave** so it can't block finder/preset work. Expect a `gh run watch` → diagnose → fix → re-push loop (the user owns this per their memory note). The done-bar is "Windows green" — treat it as iterative, not a one-shot.
**Warning signs:** vcpkg install timeouts (Qt5 from source is slow — caching is essential), MSVC `C2xxx` errors from header-trim differences, link errors from triplet mismatch.

### Pitfall 3: Config-mode target vs legacy variable confusion
**What goes wrong:** After `find_package(Eigen3 CONFIG)`, the old `${EIGEN3_INCLUDE_DIR}` variable is empty — code still doing `INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})` silently gets nothing, and the build fails to find `Eigen/Dense`.
**How to avoid:** When migrating each finder, grep for **every** use of the old `_INCLUDE_DIR(S)` / `_LIBRARIES` variable and replace with the imported target. For Eigen3: `CMakeLists.txt:361` `INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})` must become a `TARGET_LINK_LIBRARIES(BALL ... Eigen3::Eigen)`. For TBB: lines 247-249, 553-555. For OpenBabel: lines 189-190.
**Warning signs:** "No such file or directory: Eigen/Dense" / "tbb/parallel_for.h not found" after the finder swap.

### Pitfall 4: vcpkg `builtin-baseline` drift
**What goes wrong:** Omitting `builtin-baseline` (or letting it float) makes Windows builds non-reproducible — a vcpkg update changes a port version and CI goes red for unrelated reasons.
**How to avoid:** Pin `builtin-baseline` to a specific vcpkg commit SHA in `vcpkg.json`. This is the vcpkg analogue of D-08's min-version pinning.
**Warning signs:** Windows CI red on a run with no Windows-relevant code change.

### Pitfall 5: Presets must mirror `BUILD-macos.md`, not replace its semantics
**What goes wrong:** D-02 is explicit — CI must test the contributor path. If the `macos-homebrew` preset quietly differs from `BUILD-macos.md` (different `CMAKE_PREFIX_PATH`, missing `CMAKE_POLICY_VERSION_MINIMUM=3.5`), CI tests a fiction.
**How to avoid:** Derive the `macos-homebrew`/`linux-system` presets *directly* from the current `ci.yml` configure steps (which already mirror `BUILD-macos.md` verbatim). Then **update `BUILD-macos.md` and `ci.yml`** to invoke `cmake --preset ...` so all three (doc, CI, presets) are one source of truth. Otherwise DEPS-05 just adds a 4th divergent invocation.
**Warning signs:** `BUILD-macos.md` still shows the long bespoke `cmake ..` line after Phase 4.

## Runtime State Inventory

Phase 4 is a build-system + source-port phase, not a rename/migration. No stored data, live services, or OS-registered state is renamed. The one near-miss:

| Category | Items Found | Action Required |
|----------|-------------|-----------------|
| Stored data | None — no datastores keyed on any renamed string | None — verified, no rename in this phase |
| Live service config | GitHub Actions CI config (`.github/workflows/ci.yml`) — Windows job `blocking` flag flips; not "state" but a config edit | Code edit (D-01: `blocking: true` + provisioning steps) |
| OS-registered state | None | None |
| Secrets/env vars | `BALL_CONTRIB_PATH` cache var removed; `VCPKG_ROOT` env var newly *consumed* by `windows-vcpkg` preset | Document `VCPKG_ROOT` requirement in `BUILD-windows.md` (Phase 8) / preset comments |
| Build artifacts | A stale `build/` tree configured with old finders will have cached `EIGEN3_INCLUDE_DIR` etc. — config-mode migration requires a **clean reconfigure** | Document: delete `build/CMakeCache.txt` after the finder migration; CI is clean per-run so unaffected |

## Feature Matrix (FEAT-01)

The draft in `REQUIREMENTS.md` §"Feature Matrix" is **mostly accurate**. Confirmed/updated per this research:

| Component | Draft status | Confirmed? | Update |
|-----------|--------------|-----------|--------|
| Qt 5.15 | Required | ✓ | No change. Qt6 = Phase 5. |
| Boost, Eigen, FFTW, GLEW | Required | ✓ (with nuance) | **FFTW is GPL-build-only** — in the default LGPL build it's effectively absent. Strictly it's "Required for GPL builds, absent otherwise." GLEW is `USE_GLEW=ON` optional but always found. Suggest matrix note: "Boost/Eigen Required; GLEW Required-when-VIEW; FFTW Required-for-GPL-builds-only." |
| OpenBabel | Optional (3.x; was OFF) | ✓ | Confirmed Optional. Add: "Windows: OFF (not a vcpkg port)." If absent → `MolecularSimilarity` + 4 TOOLS (`MolDepict`, `ProteinProtonator`, `Ligand3DGenerator`, `MolFilter`) disabled. |
| TBB | Optional (oneTBB; was OFF) | ✓ | Confirmed Optional, enabled-when-found. Absence → no parallel speedups, no functional loss. |
| LPSolve | Optional | ✓ | Confirmed. Add: "Linux CI: OFF (Ubuntu's `liblpsolve55.a` is non-PIC). Windows: OFF (not a vcpkg port). macOS: ON." |
| libSVM | Optional | ✓ | Confirmed Optional. **Is** a vcpkg port (3.35) so available on all 3 platforms. Keeps a thin finder shim (no upstream config). |
| QtWebEngine | Optional/deferred | ✓ | No change — `qt@5` Homebrew has no WebEngine; stays disabled. |
| RTfact | Removed | ✓ | No change. |
| Python bindings (SIP) | Deferred to Phase 6 | ✓ | No change. |
| VRPN/SpaceNavigator | Removed | ✓ | No change. |

**FEAT-01 deliverable:** a small doc update to `REQUIREMENTS.md` adding the per-platform availability notes above. Foldable into Wave 0 as a documentation task. The matrix is sound; it needs *annotation*, not restructuring.

## Minimum Version Pinning (DEPS-04 / D-08)

Recommended pins, with rationale. Whether these live as `find_package(X.Y)` args or a central version block is the planner's call (CONTEXT.md discretion).

| Dep | Recommended min | Rationale |
|-----|-----------------|-----------|
| CMake | `3.21` | `CMakePresets.json` v3 + `toolchainFile`. Repo `CMAKE_MINIMUM_REQUIRED` is `3.5...3.31` — bump the floor to 3.21, keep the `...3.31` ceiling. |
| Qt5 | `5.15` | Current repo `QT_MIN_VERSION` is `5.5` — bump to `5.15` (the only Qt5 line still maintained; Homebrew + vcpkg both ship 5.15.x). |
| Boost | `1.70` | Config-mode `BoostConfig.cmake` is reliable from 1.70+; current pin is `1.55`. `cmake/BALLConfigBoost.cmake` comment already notes 1.70 as the config-mode threshold. |
| Eigen3 | `3.4` | First Eigen with stable `Eigen3Config.cmake` semantics that also covers the 5.x line. Current effective pin is `3.0`. |
| TBB (oneTBB) | `2021.0` | First oneTBB release with `TBBConfig.cmake` exporting `TBB::tbb`. The old `<2017` gate is deleted entirely (D-04). |
| OpenBabel | `3.0` (target `OpenBabel3`) | The port targets the 3.x API; 3.1.1 is current Homebrew. |
| FFTW | `3.3` | Stable; bundled finder kept. |
| GLEW | `2.0` | `GLEW::GLEW` imported target era. |
| libSVM | `3.0` | Homebrew/vcpkg both ship 3.x; the wrapper hack in `libsvmModel.h` should be checked against 3.35 (CONCERNS.md flags it). |

## Validation Architecture

> `workflow.nyquist_validation` is `true` in `.planning/config.json` — section included.

### Test Framework
| Property | Value |
|----------|-------|
| Framework | None wired for Phase 4. The `test/` tree exists but is `ADD_SUBDIRECTORY(test EXCLUDE_FROM_ALL)` (`CMakeLists.txt:795`) — wiring it into ctest is **Phase 9**, explicitly out of scope. |
| Config file | `.github/workflows/ci.yml` — the 3-OS build matrix + render smoke check **is** the validation harness for this phase |
| Quick run command | `cmake --preset <platform>` then `cmake --build build/<platform> --target BALL VIEW BALLView` (local per-platform build) |
| Full suite command | Push to `v1.6-modernization` → CI runs all 4 jobs (build macos/linux/windows + lint) + render smoke on macOS/Linux |

### Phase Requirements → Test Map
| Req ID | Behavior | Test Type | Automated Command | File Exists? |
|--------|----------|-----------|-------------------|--------------|
| DEPS-01 | `ball_contrib` off the build path; deps resolve from system/Homebrew | build | macOS+Linux CI jobs configure+build green with no `BALL_CONTRIB_PATH` set | ✅ `ci.yml` |
| DEPS-02 | `vcpkg.json` provides the Windows dep set; Windows builds | build | Windows CI job configures+builds green with the vcpkg toolchain; `blocking: true` | ✅ `ci.yml` (job exists; provisioning is Wave 2 work) |
| DEPS-03 | Config-mode `find_package` for Eigen3/TBB/OpenBabel | build | All 3 CI platforms configure with the bundled finders deleted; configure log shows config-mode resolution | ✅ `ci.yml` |
| DEPS-04 | Min versions pinned + documented | configure | CI configure step succeeds with the pins; a `cmake` configure on a deliberately-old dep fails clearly (manual spot-check) | ✅ `ci.yml` (configure step) |
| DEPS-05 | `CMakePresets.json` for the 4 environments | build | `cmake --preset macos-homebrew` / `linux-system` / `windows-vcpkg` / `ci` each configure; CI is migrated to invoke presets | ❌ Wave 0 — `CMakePresets.json` is new |
| FEAT-01 | Feature matrix classifies optional deps + absence impact | doc review | `REQUIREMENTS.md` §"Feature Matrix" updated with per-platform notes; reviewed | ✅ doc — no automated test |
| (D-05) | OpenBabel 3.x port compiles + links with `BALL_HAS_OPENBABEL=ON` | build | macOS+Linux CI build green with OpenBabel enabled (the 5 files compile + link against open-babel 3.1.1) | ✅ `ci.yml` (needs `BALL_HAS_OPENBABEL` ON in the preset) |

### Sampling Rate
- **Per task commit:** local `cmake --preset <platform>` + build the 3 targets on the dev machine (macOS).
- **Per wave merge:** push to `v1.6-modernization` → full CI matrix (the user `gh run watch`-es per their memory note).
- **Phase gate:** all 4 CI jobs green **including a now-required (`blocking: true`) Windows job** + render smoke green on macOS/Linux.

### Wave 0 Gaps
- [ ] `CMakePresets.json` — new file (DEPS-05); also migrate `ci.yml` + `BUILD-macos.md` to invoke it.
- [ ] CI preset must set `BALL_HAS_OPENBABEL=ON` (or equivalent) so the OpenBabel port is actually exercised by CI on macOS/Linux — today `ci.yml` doesn't enable it. Without this, the D-05 port lands untested.
- [ ] Windows CI: real dependency-provisioning steps (vcpkg bootstrap + `choco install winflexbison3`) — currently the Windows job is a `|| true` no-op.
- [ ] No framework *install* needed — the existing CI matrix is the harness. `test/` → ctest is deliberately Phase 9.

*Note: there is no unit-test gap to fill in-phase — Phase 4's verification is "configures + builds + links + render-smoke green on all 3 OSes," which the Phase 02.2 CI already provides. The one real gap is that CI must be made to actually enable OpenBabel/TBB so D-05 is exercised.*

## Environment Availability

| Dependency | Required By | Available (macOS dev) | Version | Fallback |
|------------|------------|----------------------|---------|----------|
| Homebrew `qt@5 boost eigen fftw tbb glew open-babel lp_solve libsvm flex bison` | macOS/Linux build | ✓ (per `BUILD-macos.md`, CI green) | qt 5.15.x, open-babel 3.1.1, oneTBB 2021/2022 | — |
| CMake ≥ 3.21 | `CMakePresets.json` v3 | ✓ (repo tested to 3.31) | 3.31 | — |
| vcpkg | Windows dependency provisioning | ✗ on dev machine (Windows-only path; not needed on macOS) | — | Windows CI runner bootstraps vcpkg; no macOS fallback needed |
| `choco` (Chocolatey) | flex/bison on Windows CI | ✗ on dev machine | — | Pre-installed on GitHub `windows-2022` runners; `choco install winflexbison3` |
| GitHub Actions `windows-2022` runner | Verifying the Windows green build (DEPS-02) | ✓ (CI matrix already has the job) | windows-2022 | None — "Windows green" is **only** verifiable via CI (per CONTEXT.md Specifics) |

**Missing dependencies with no fallback:** None blocking. The Windows-specific tooling (vcpkg, choco) is provisioned *on the CI runner*, not the dev machine — that's by design (D-02: macOS/Linux dev stays Homebrew/system).

**Missing dependencies with fallback:** OpenBabel + lp_solve on Windows → Optional, left OFF (feature matrix permits). flex/bison on Windows → `winflexbison3` via choco.

## State of the Art

| Old approach | Current approach | When changed | Impact |
|--------------|------------------|--------------|--------|
| Bundled `Find*.cmake` modules | Upstream `*Config.cmake` via `find_package(CONFIG)` | Eigen 3.3 (2016), oneTBB 2021, OpenBabel 3.0 (2019) all started shipping configs | The 3 broken finders are broken *because* they predate this — D-03/D-04 catch up |
| `ball_contrib` bundled dependency tarballs | System/Homebrew + vcpkg manifest | This milestone (1.6) | DEPS-01/DEPS-02 — `ball_contrib`'s ~2016 tarballs don't build on modern toolchains |
| Bespoke per-platform `cmake ..` command lines | `CMakePresets.json` (CMake 3.19+, v3 schema 3.21+) | CMake 3.19 (2020), v3 in 3.21 | DEPS-05 — one preset invocation replaces the 8-line `BUILD-macos.md` command |
| OpenBabel 2.x C++ API | OpenBabel 3.x API | OpenBabel 3.0 (Oct 2019), 3.1.1 current | D-05 — element table, valence renames, implicit-H model, aromaticity perception all changed |
| `OBElementTable` / `etab` | `OBElements` namespace + `<openbabel/elements.h>` | OpenBabel 3.0 | Per-symbol element lookups changed for performance |

**Deprecated/outdated in this codebase:**
- `cmake/FindOpenBabel2.cmake` — looks for `openbabel-2.0` pkg-config; OpenBabel 3.x is `openbabel-3`. Delete (D-04).
- `cmake/FindTBB.cmake` — pre-oneTBB, `task_scheduler_init.h` (removed in oneTBB), `<2017` version gate. Delete (D-04).
- `cmake/FindEigen3.cmake` — already partially patched (reads `Eigen/Version`) but still module-mode; replace with config-mode (D-04).
- `BALL_CONTRIB_PATH` cache var + all its `IF()` blocks — obsolete (D-09).

## Assumptions Log

| # | Claim | Section | Risk if Wrong |
|---|-------|---------|---------------|
| A1 | Moving the OpenBabel `find_package` out of the `IF(BALL_LICENSE_GPL)` block is acceptable under BALL's LGPL build (OpenBabel is GPL, dynamically linked, behind `BALL_HAS_OPENBABEL`). D-05 says "enabled in 1.6" which implies this. | Pitfall 1 | If the GPL gating was deliberate licensing policy, enabling OpenBabel in the default LGPL build could be a license violation. **Discuss-phase or planner should confirm with the maintainer** whether OpenBabel-as-optional-dynamic-dep is OK in an LGPL BALL build, or whether "enabled in 1.6" means "enabled in GPL builds only." This is the single most important open question. |
| A2 | `x64-windows` (dynamic) is the right vcpkg triplet — BALL builds shared libs + Qt5 is dynamic. | Pattern 4 | If something forces static linkage on Windows, `x64-windows-static` would be needed and the Qt5 deployment story changes. CONTEXT.md explicitly leaves triplet to discretion — low risk, just means a Wave 2 adjustment. |
| A3 | Leaving OpenBabel + lp_solve OFF on Windows satisfies DEPS-02's "complete Windows dependency set." D-06 classifies both as Optional and D-01 defines "complete" as "Windows actually builds." | Pattern 4, Feature Matrix | If a stakeholder reads "complete dependency set" literally, they may expect OpenBabel on Windows — requiring building it from source in CI (slow, fragile) or an overlay port. The CONTEXT.md D-01 wording ("complete enough that Windows actually builds") supports the OFF approach, but worth surfacing. |
| A4 | `winflexbison3`'s `win_bison.exe` works with BALL's bison invocation without the existing `BALL-bison.bat` wrapper (`CMakeLists.txt:118-129`). | Pattern 4 | If `win_bison` needs the `.bat` wrapper or has incompatible flags, the Windows parser-generation step fails — a Wave 2 bring-up issue to debug, not a blocker. |
| A5 | `molecularSimilarity.C` already using `SetBondOrder`/`GetBondOrder` (not `SetBO`/`GetBO`) means that part is already 3.x-clean. | OpenBabel port table | Low risk — even if some `GetBO` slipped in elsewhere, the compiler flags it immediately. The grep was a sample, not exhaustive per-line. |
| A6 | The OpenBabel port can be verified by "compiles + links on macOS/Linux CI" without a behavioral test, because `test/` is `EXCLUDE_FROM_ALL` until Phase 9. | Validation Architecture | If the implicit-hydrogen or aromaticity-perception semantics change silently corrupts `generateCanSmile` output, a compile-only gate won't catch it. Mitigation: planner may add a tiny standalone smoke (one molecule → non-empty canonical SMILES) even though the full `test/` tree stays deferred. |

## Open Questions

1. **OpenBabel licensing in the LGPL build (see A1)**
   - What we know: OpenBabel's `find_package` is currently inside `IF(BALL_LICENSE_GPL)`. D-05 says enable OpenBabel in 1.6. The default build is LGPL.
   - What's unclear: whether "enable in 1.6" means "remove the GPL gating entirely" or "enable, but still only in GPL builds." OpenBabel is GPL-licensed software.
   - Recommendation: **Surface to discuss-phase or the maintainer before the OpenBabel wave.** If it stays GPL-gated, the default LGPL CI build can't exercise the port — CI would need a GPL-build job, or the OpenBabel wave verifies via a `-DBALL_LICENSE=GPL` configure. This materially affects how D-05 is verified.

2. **`ci` preset shape — one preset or three?**
   - What we know: a single `ci` preset can't inherit all 3 platform presets simultaneously.
   - What's unclear: whether to do `ci-macos`/`ci-linux`/`ci-windows` (clean, matches the matrix) or a base `ci` + per-runner cache-var layering.
   - Recommendation: planner's call (CONTEXT.md discretion). `ci-<platform>` triplet is the lower-friction option and keeps the `ci.yml` matrix simple.

3. **Does CI currently build with OpenBabel/TBB at all?**
   - What we know: `ci.yml` installs `open-babel` and `tbb` via Homebrew/apt but the configure step doesn't pass `BALL_HAS_OPENBABEL=ON` and OpenBabel is GPL-gated anyway. TBB is `USE_TBB=ON` by default but the broken finder means `TBB_FOUND` is false.
   - What's unclear: confirm by reading a recent CI configure log whether TBB/OpenBabel are actually `FOUND`.
   - Recommendation: Wave 0/Wave 1 must ensure the CI preset enables these so D-05 is genuinely tested — otherwise the port "passes CI" without being compiled.

## Sources

### Primary (HIGH confidence)
- Direct inspection of repo files: `CMakeLists.txt`, `cmake/BALLConfigBoost.cmake`, `cmake/FindEigen3.cmake`, `cmake/FindTBB.cmake`, `cmake/FindOpenBabel2.cmake`, `cmake/FindLPSolve.cmake`, `cmake/FindlibSVM.cmake`, `.github/workflows/ci.yml`, `BUILD-macos.md` — current build-system state
- `grep` across `source/` + `include/` — exhaustive OpenBabel API usage surface (5 files; `hybridisationProcessor.C` is comments-only)
- `grep` across `*.cmake`/`CMakeLists.txt`/`*.in` — full `ball_contrib` reference inventory
- vcpkg registry (`raw.githubusercontent.com/microsoft/vcpkg/master/ports/*/vcpkg.json`) — VERIFIED port availability + versions: `qt5-base` 5.15.18, `eigen3` 5.0.1, `tbb` 2022.3.0, `fftw3` 3.3.10, `glew` 2.3.1, `libsvm` 3.35, `boost` present; `openbabel`/`lp-solve`/`flex`/`bison` all HTTP 404 (not ports)
- [open-babel.readthedocs.io/en/latest/UseTheLibrary/migration.html](https://open-babel.readthedocs.io/en/latest/UseTheLibrary/migration.html) — OpenBabel 2.x→3.0 API breaking changes
- `.planning/` docs: `04-CONTEXT.md`, `ROADMAP.md`, `REQUIREMENTS.md`, `STATE.md`, `codebase/CONCERNS.md`, `codebase/STACK.md`, `codebase/INTEGRATIONS.md`, `02.2-CONTEXT.md`, `03-CONTEXT.md`

### Secondary (MEDIUM confidence)
- [github.com/microsoft/vcpkg releases](https://github.com/microsoft/vcpkg/releases) — vcpkg 2026.04.27, 2807 ports, manifest mode is current standard
- [learn.microsoft.com/en-us/vcpkg/concepts/manifest-mode](https://vcpkg.io/en/docs/maintainers/manifest-files.html) — `vcpkg.json` schema
- [cmake.org cmake-presets docs](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html) — presets v3 / `toolchainFile` (CMake 3.21)
- [open-babel.readthedocs.io ob300 release notes](https://open-babel.readthedocs.io/en/latest/ReleaseNotes/ob300.html) — OpenBabel 3.0 changes

### Tertiary (LOW confidence — flagged for validation)
- `winflexbison3` as the Windows flex/bison provisioning path — common practice, not verified against BALL's specific bison invocation (A4)
- Exact OpenBabel per-line edits — port *surface* is HIGH confidence; the implicit-H/aromaticity semantics need a build-and-test loop to pin down (A5, A6)

## Metadata

**Confidence breakdown:**
- Codebase state (finders, `ball_contrib` inventory, OpenBabel surface): **HIGH** — direct inspection + exhaustive grep
- Config-mode finder migration: **HIGH** — Boost is the working in-repo proof; upstream configs confirmed to exist
- vcpkg port availability: **HIGH** — verified against the live registry (HTTP status per port)
- vcpkg Windows green build: **MEDIUM** — port set is known, but first-ever real Windows provisioning is inherently iterative (Phase 02.2 precedent: 2 bring-up rounds)
- OpenBabel 3.x port: **HIGH** on surface/files, **MEDIUM** on exact edits — implicit-H + aromaticity semantics are the soft spots
- Feature matrix: **HIGH** — the REQUIREMENTS.md draft is sound, needs per-platform annotation only

**Research date:** 2026-05-14
**Valid until:** ~2026-06-14 (30 days — stable build tooling; vcpkg port set could shift but the *absence* of openbabel/lp_solve/flex/bison is long-standing)
