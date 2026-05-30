---
phase: 04-dependency-system-overhaul
plan: "04"
subsystem: ci/build-system
tags: [vcpkg, windows, msvc, ci, manifest-mode, boost-regex, glu, blocking-flip]

# Dependency graph
requires:
  - phase: 04-dependency-system-overhaul
    plan: "01"
    provides: config-mode find_package + min-version pins (Eigen3, TBB) — Windows build resolves through them
  - phase: 04-dependency-system-overhaul
    plan: "02"
    provides: CMakePresets.json with the `windows-vcpkg` / `ci-windows` presets carrying VCPKG toolchainFile and CMAKE_BUILD_TYPE=Release

provides:
  - vcpkg.json manifest with pinned `builtin-baseline` declaring the complete Windows-buildable dependency set (qt5-base/qt5-tools, boost-*, eigen3, tbb, glew, fftw3, lpsolve, libxml2, openssl, zlib, ...)
  - cmake/vcpkg-triplets/x64-windows-release.cmake — release-only overlay triplet that skips Debug Boost/Qt/etc. (~50% build-time cut on CI)
  - .github/workflows/ci.yml — Windows job rewired with real provisioning (vcpkg manifest install + choco winflexbison3) configuring through ci-windows preset, building BALL.dll / VIEW.dll / BALLView.exe
  - actions/cache@v4 x-gha binary cache keyed on vcpkg.json hash for cold-cache mitigation
  - VCPKG_ROOT bridge from runner-shipped VCPKG_INSTALLATION_ROOT (windows-2022 ships vcpkg but exposes it under a different env var than the preset expects)
  - QT_NO_KEYWORDS reaching the MSVC compiler via add_compile_definitions (was silently dropped on Windows)
  - Explicit Boost component target linking (Boost::regex / Boost::date_time / Boost::thread / Boost::iostreams / Boost::system / Boost::serialization) — implicit linking broke on the vcpkg layout
  - source/COMMON/regularExpression.C — BALL::RegularExpression ported from the deprecated Boost.Regex POSIX C API (`regex_t` / `regcomp` / `regexec`) to `boost::regex` / `boost::sregex_iterator` (header-only on the new vcpkg port)
  - include/BALL/VIEW/RENDERING/RENDERERS/glRenderer.h — explicit `#include <GL/glu.h>` (Linux/MSVC) / `<OpenGL/glu.h>` (Darwin) in the `BALL_HAS_GLEW` branch (MSVC + vcpkg's OpenGL port doesn't transitively expose GLUquadricObj like the system Qt/GLU paths do)
  - cmake/BALLViewTranslations.cmake — `return()` argument removed (MSVC's CMake module path doesn't tolerate the older-Bison-style `RETURN(...)` extension)
  - Windows job flipped from `blocking: false` (continue-on-error) to `blocking: true` (required); macOS/Linux/lint remain blocking
affects: [05-qt6-migration, 05.1-build-warnings-and-latent-bugs, 08-packaging]

# Tech tracking
tech-stack:
  added: [vcpkg-manifest-mode]
  patterns:
    - "vcpkg.json manifest mode with pinned `builtin-baseline` (40-char SHA) for reproducible Windows builds — supply-chain hardening (T-04-13)"
    - "Custom triplet `x64-windows-release.cmake` overlays the default x64-windows to skip Debug binaries (~50% CI build-time saving on cold cache; mandatory for actions/cache@v4 size limits)"
    - "actions/cache@v4 with `x-gha` binary cache provider, keyed on `hashFiles('vcpkg.json', 'cmake/vcpkg-triplets/x64-windows-release.cmake')` — warm cache restores in seconds"
    - "VCPKG_ROOT bridged from VCPKG_INSTALLATION_ROOT inside pwsh (`$env:VCPKG_ROOT = $env:VCPKG_INSTALLATION_ROOT`) — windows-2022 runner ships vcpkg but exposes it under the wrong env var for cmake-presets toolchainFile resolution"
    - "Explicit Boost::component targets used in target_link_libraries — implicit Boost linking via Boost_LIBRARIES legacy variable broke against the vcpkg-config Boost ports"
    - "BALL::RegularExpression now wraps boost::regex (C++ API) rather than POSIX C regcomp/regexec — Boost.Regex deprecated the POSIX C wrapper, vcpkg's boost-regex no longer ships it"
    - "Explicit GLU header include at the glRenderer.h GLUquadricObj member declaration site — MSVC + vcpkg's OpenGL port doesn't transitively pull GLU like system Qt/GLU on macOS+Linux"
    - "Matrix-driven `continue-on-error: ${{ !matrix.blocking }}` is the gate-flip mechanism — flipping `blocking: false` → `blocking: true` on the Windows entry promotes it to a required check without restructuring the workflow"

key-files:
  created:
    - vcpkg.json
    - cmake/vcpkg-triplets/x64-windows-release.cmake
  modified:
    - .github/workflows/ci.yml
    - source/COMMON/regularExpression.C
    - include/BALL/COMMON/regularExpression.h
    - include/BALL/VIEW/RENDERING/RENDERERS/glRenderer.h
    - cmake/BALLViewTranslations.cmake
    - CMakeLists.txt

key-decisions:
  - "Defer Windows GLU/MSVC fix vs land it: we held option B (defer) ready but the GLU include fix shipped, Windows went green, and the deferred-Windows phase scaffolding was unnecessary — Phase 5.1 instead captures the broader latent-bug + warning backlog surfaced by the green build"
  - "Boost.Regex POSIX C → C++ port (`regex_t` → `boost::regex`) is a real source change to BALL, not a wrapper hack; the Boost C ABI for POSIX regex is gone in modern Boost.Regex and the port is the canonical migration path"
  - "x64-windows-release overlay triplet kept as a Phase 4 artifact (not folded into vcpkg defaults) — the choice between Debug+Release and Release-only is environment-dependent (CI vs developer workflow)"
  - "Boost component linking made explicit and load-bearing: the implicit `Boost_LIBRARIES` legacy variable is no longer used; every target_link_libraries call names the components it needs"
  - "QT_NO_KEYWORDS added via add_compile_definitions rather than per-target — already required on macOS/Linux for Qt5 compat, but was silently dropping on Windows before this fix; centralizing it avoided per-target audit"
  - "An exploratory warning-cleanup commit (d7b2de6) was REVERTED (656013b) before it landed — out of Plan 04-04 scope; the work is captured as Phase 5.1 backlog with Codex CLI cross-checked fix proposals"

requirements-completed: [DEPS-02]

# Metrics
duration: ~6h (Task 1 ~1h + Task 2 ~3h initial wiring + Task 3 ~2h CI bring-up iteration over ~14 attempts before green)
attempts_before_green: 13 (CI runs since e63f061 first wired the job; final green on `6056f74` was run 25899905204)
completed: 2026-05-15
---

# Plan 04-04 Summary — Windows CI on vcpkg manifest + Boost.Regex port + GLU include + blocking flip

## Performance

| Subtask | Duration | Notes |
|---|---|---|
| Task 1 — vcpkg.json author | ~1h | Single commit `3ae77aa`; manifest declares qt5-base/tools, boost components, eigen3, tbb, glew, fftw3, lpsolve, libxml2, openssl, zlib + pinned baseline |
| Task 2 — CI wiring | ~3h | Commit `e63f061` + 11 follow-up fixes (VCPKG_ROOT bridge, x-gha cache, missing Boost ports, QT_NO_KEYWORDS reach, RETURN() arg, Boost.Regex port, GLU include) |
| Task 3 — bring-up + blocking flip | ~2h iter + ~30min wait | ~14 CI runs total; first green at run 25899905204 on `6056f74`; blocking flip in this wrap-up commit batch |
| Task 4 — SUMMARY + phase-complete | ~30min | This document |

Cold-cache vcpkg Windows build: ~75min (run-25873070779 era). Warm-cache (x-gha hit): ~30min total (run-25899905204).

## Accomplishments

1. **vcpkg.json manifest authored** — declares the full Windows-buildable dependency set (qt5-base, qt5-tools, boost-{regex,date-time,thread,iostreams,system,serialization,chrono,filesystem,...}, eigen3, tbb, glew, fftw3, lpsolve, libxml2, openssl, zlib) with a pinned `builtin-baseline` for reproducibility. Supply-chain hardening per threat T-04-13.
2. **Windows CI job rewired** — no longer a `|| true` no-op; provisions via `vcpkg install --triplet x64-windows-release` + `choco install winflexbison3`, configures through the `ci-windows` preset, builds `BALL.dll` / `VIEW.dll` / `BALLView.exe` as a real pass/fail step.
3. **Custom release-only triplet** — `cmake/vcpkg-triplets/x64-windows-release.cmake` skips Debug binaries, halving cold-cache build time and fitting under actions/cache@v4 size limits.
4. **x-gha binary cache** — keyed on `vcpkg.json` hash; warm-cache restores in seconds, making the Windows job iterable.
5. **Boost.Regex POSIX C → C++ port** — `BALL::RegularExpression` rewritten off `regex_t` / `regcomp` / `regexec` (Boost 1.x deprecated, vcpkg's boost-regex no longer ships the C wrapper) onto `boost::regex` / `boost::sregex_iterator`. Header-only on the new port. Source change in `source/COMMON/regularExpression.C` + matching `include/BALL/COMMON/regularExpression.h`.
6. **Explicit GLU include** — `#include <GL/glu.h>` (Linux/MSVC) / `<OpenGL/glu.h>` (Darwin) in `include/BALL/VIEW/RENDERING/RENDERERS/glRenderer.h:32-46`, gated on `BALL_HAS_GLEW`. macOS/Linux pulled GLU transitively through system Qt/GLU; MSVC + vcpkg doesn't, so `GLUquadricObj` was unknown at the member declaration site.
7. **MSVC-side fixes** — `QT_NO_KEYWORDS` reaches the Windows compiler (was silently dropped); `RETURN()` argument removed from `BALLViewTranslations.cmake`; Boost component target linking made explicit.
8. **Windows job flipped to blocking** — `continue-on-error: ${{ !matrix.blocking }}` evaluates to `false` for Windows now; it's a required check. macOS/Linux/lint remain blocking. If Windows regresses, the build fails — no silent fallback.

## Task commits

| Commit | Scope | Notes |
|---|---|---|
| `3ae77aa` | Task 1 | feat(04-04): author vcpkg.json with pinned Windows dependency set |
| `e63f061` | Task 2 | feat(04-04): wire Windows CI job with real vcpkg + choco provisioning |
| `13e182c` | Task 2 | fix: bridge VCPKG_ROOT from VCPKG_INSTALLATION_ROOT |
| `5165b87` | Task 2 | fix: set VCPKG_ROOT inside pwsh |
| `8bd5294` | Task 2 | fix: drop unresolvable vcpkg builtin-baseline (then re-pinned) |
| `bda97ba` | Task 2 | perf: x-gha cache + release triplet + trimmed deps |
| `397bcaf` | Task 2 | fix: RETURN() takes no arg in BALLViewTranslations.cmake |
| `cf41403` | Task 2 | fix: add missing Boost ports; revert x-gha → actions/cache |
| `806b295` | Task 2 | fix: QT_NO_KEYWORDS reach Windows; fix vcpkg cache |
| `26b9e60` | Task 2 | fix: always link explicit Boost component targets |
| `44b46c4` | Task 2 | fix: port BALL::RegularExpression off Boost.Regex POSIX C API |
| `6056f74` | Task 2 | fix: include <GL/glu.h> explicitly when GLEW is enabled (final fix that turned Windows green on run 25899905204) |
| `d7b2de6` → `656013b` | (reverted) | exploratory warning cleanup landed and reverted same-day — out of Plan 04-04 scope; captured as Phase 5.1 backlog |
| _this batch_ | Task 3 + Task 4 | hashGrid.h Phase 03-01 follow-up; Windows blocking flip; this SUMMARY; Phase 4 closed in ROADMAP |

## Files created

- `vcpkg.json` — Windows dependency manifest
- `cmake/vcpkg-triplets/x64-windows-release.cmake` — release-only overlay triplet
- `.planning/phases/05.1-build-warnings-and-latent-bugs/05.1-BACKLOG.md` — Phase 5.1 follow-up scope from the green build's warning surface

## Files modified

- `.github/workflows/ci.yml` — Windows job (provisioning, configure, build, blocking flip)
- `source/COMMON/regularExpression.C` + `include/BALL/COMMON/regularExpression.h` — Boost.Regex POSIX C → C++ port
- `include/BALL/VIEW/RENDERING/RENDERERS/glRenderer.h` — explicit GLU header include
- `cmake/BALLViewTranslations.cmake` — `RETURN()` argument removed
- `CMakeLists.txt` — Boost component linking made explicit; QT_NO_KEYWORDS centralized
- `include/BALL/DATATYPE/hashGrid.h` — Phase 03-01 lambda rewrite follow-up: drop stray `*item` deref + `processor->finish()` arrow (iterator/processor are values, not pointers, after the lambda port)

## Decisions made

1. **Defer-Windows option built then unused.** During Task 3 bring-up we scaffolded a "defer Windows to a later phase" plan as an escape hatch, but the GLU include fix (`6056f74`) cleared the actual blocker. The deferred-Windows phase was never opened; Phase 5.1 instead captures the broader warning + latent-bug backlog surfaced *by* the green build.
2. **Boost.Regex port is real, not a shim.** The Boost C-API for POSIX regex is gone in modern Boost.Regex; the migration to `boost::regex` is the canonical path and matches upstream guidance. Cleaner than vendoring the old wrapper.
3. **Release-only triplet kept as a Phase 4 artifact, not upstreamed.** Whether to ship Debug+Release binaries is a CI-vs-developer tradeoff; centralizing the choice in a per-project triplet rather than mutating vcpkg defaults preserves both options.
4. **Warning cleanup deferred to Phase 5.1.** The same-day d7b2de6/656013b commit pair shows a 5082-warning cleanup was attempted then reverted as out-of-scope. The work is the right work — but it belongs in its own phase (5.1), Codex-cross-checked, not folded into a Windows-bring-up task.

## Deviations from plan

- **Task 2 grew from ~3 sub-fixes to 11.** Anticipated in the plan's Pitfall 2 ("Windows CI bring-up is iterative — budget for it"); the spec specifically called this out and the iteration count was within the expected envelope.
- **Boost.Regex port was not in the plan.** Surfaced during Task 2 when the build hit `regex_t` link errors. Treated as a Task-2 follow-up rather than spinning a new sub-plan; the change is well-scoped (one class) and tightly coupled to the vcpkg Boost ports landing.
- **GLU include fix was not in the plan.** Same as above — surfaced during the final bring-up rounds; sub-9-line fix in a single header.
- **Phase 5.1 spun out.** The green build emitted ~10k warning lines (mostly noise but ~6 latent bug categories). User decision to capture them as Phase 5.1 rather than fold into Phase 4 wrap-up.

## CI coverage

- macOS arm64 (Homebrew Qt5) — green throughout
- Linux x64 (apt Qt5) — green throughout
- Windows x64 (vcpkg Qt5 + choco flex/bison) — green from run 25899905204 (HEAD `6056f74`). 14 CI runs total between `e63f061` (first real-provisioning attempt) and `6056f74` (first green Windows + green everywhere else).
- Lint (legacy-GL grep gate) — green throughout; 0 legacy Qt GL symbol references in code.

## Known stubs / follow-ups

- **Phase 5.1: Build Warnings & Latent Bug Cleanup** — scoped in [`.planning/phases/05.1-build-warnings-and-latent-bugs/05.1-BACKLOG.md`](../../phases/05.1-build-warnings-and-latent-bugs/05.1-BACKLOG.md). Contains the Codex-cross-checked fix proposals for C4717 (`getline` recursion), C4311 (pointer truncation on 64-bit Windows in MMFF94 code), C4910 (extern + dllexport mismatch in vector3.h), `-Wself-assign-field`, `-Wformat-overflow`, `-Wtautological-constant-out-of-range-compare`. Tier B Windows DLL hygiene + Tier C style cleanup also scoped there.
- **Qt5LinguistTools missing on Windows vcpkg** — translation pipeline (lupdate/lrelease) silently skipped. Captured as Phase 5.1 Task D1.
- **Phase 4.1 (Config Color-Defaults Fix, promoted from backlog 999.4)** — still open; orthogonal to 04-04 scope, no dependency.
