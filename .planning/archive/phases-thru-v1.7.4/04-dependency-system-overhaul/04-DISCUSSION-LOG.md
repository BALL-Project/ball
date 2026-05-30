# Phase 4: Dependency System Overhaul - Discussion Log

> **Audit trail only.** Do not use as input to planning, research, or execution agents.
> Decisions are captured in CONTEXT.md — this log preserves the alternatives considered.

**Date:** 2026-05-14
**Phase:** 04-dependency-system-overhaul
**Areas discussed:** vcpkg & Windows, Find*.cmake migration, Optional-dep policy / feature matrix, Presets & ball_contrib removal

---

## vcpkg & Windows

| Option | Description | Selected |
|--------|-------------|----------|
| Manifest + drive Windows green | Author vcpkg.json, wire toolchain, iterate Windows CI to green, flip job to required | ✓ |
| Manifest authored, CI stays non-blocking | vcpkg.json + presets so Windows *can* build; later phase verifies green | |
| Best-effort manifest only | Ship vcpkg.json as a starting point; Windows-green is Phase 8's problem | |

**User's choice:** Manifest + drive Windows green
**Notes:** Aligns with DEPS-02 ("complete Windows dependency set") and Phase 02.2 D-03 (Windows CI job flips to required after Phase 4 lands the manifest).

---

## Find*.cmake migration

| Option | Description | Selected |
|--------|-------------|----------|
| Config-mode + fix the 3 broken finders | config-mode find_package where upstream ships a config; thin shims for lp_solve/libSVM; fix FindOpenBabel2, FindTBB, FindEigen3 | ✓ |
| Config-mode where available only | Replace finders only where a config exists; leave broken legacy finders alone | |
| Rewrite all finders uniformly | Modernize every Find*.cmake regardless of config availability | |

**User's choice:** Config-mode + fix the 3 broken finders
**Notes:** The 3 broken finders (OpenBabel 2.4, TBB pre-oneTBB, Eigen 5 version header) are flagged in `.planning/codebase/CONCERNS.md` and block real functionality.

---

## Optional-dep policy / feature matrix

| Option | Description | Selected |
|--------|-------------|----------|
| TBB ON, OpenBabel OFF but finder fixed | Enable oneTBB; fix OpenBabel finder but keep it OFF — defer the 3.x port | |
| Both ON in 1.6 | Enable TBB *and* port BALL's OpenBabel integration to the 3.x API | ✓ |
| Both stay OFF, finders fixed only | Fix finders so both *can* be enabled later; ship with both OFF | |

**User's choice:** Both ON in 1.6
**Notes:** Interpreted as enabled-by-default-when-found — both remain *Optional* in the feature matrix, not promoted to Required. The OpenBabel 2.4→3.x API port is the one source-touching item in the phase; sequence it as its own plan/wave.

---

## Presets & ball_contrib removal

| Option | Description | Selected |
|--------|-------------|----------|
| 4 presets, Release-focused, extensible | macos-homebrew / linux-system / windows-vcpkg / ci; Debug variants addable later; min-versions pinned in CMakeLists | ✓ |
| 4 presets + Debug variants (8 total) | Same four platforms plus explicit Debug variant of each | |
| Minimal: one preset per platform | Three thin platform presets, build type left to the user | |

**User's choice:** 4 presets, Release-focused, extensible
**Notes:** Mirrors Phase 02.2's Release-only CI choice. `ball_contrib` is a separate sibling repo — Phase 4 de-references it from `ball/`'s CMake, it does not delete the directory.

## Claude's Discretion

- Exact vcpkg triplet(s), preset cache-variable layout, finder shim style.
- Whether min-version pins live as `find_package(... X.Y)` calls vs a central version block.
- Internal sequencing of the OpenBabel 3.x port within the phase.

## Deferred Ideas

None — discussion stayed within the Phase 4 boundary.
