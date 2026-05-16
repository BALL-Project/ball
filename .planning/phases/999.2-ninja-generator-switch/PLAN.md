---
phase: 999.2-ninja-generator-switch
type: phase-plan
milestone: v1.6.1
status: in-flight
created: 2026-05-16
predecessor_prep_commits:
  - d5f5566  # ci(windows): build with --parallel $env:NUMBER_OF_PROCESSORS
  - 9c932eb  # ci(windows): preinstall ccache (no-op until Ninja switch)
  - a2c0851  # docs(roadmap): retarget Phase 999.2 v2.0 → v1.6.1 (ROADMAP-only)
  - fbc7a12  # ci(windows): consolidate winflexbison3 + ccache into composite action
---

# Phase 999.2 — Ninja Build Generator Switch (macOS / Linux / Windows)

## Goal

Switch the CMake generator used by CI from per-platform `Make` / `MSBuild`
to **`Ninja` (single-config)** across all three platforms (`ci-macos`,
`ci-linux`, `ci-windows`) so:

1. Windows CI stops paying the MSBuild + `cl.exe`-via-vcxproj tax that
   makes the Windows job a ~77-minute outlier (last green: run
   [25899905204](https://github.com/BALL-Project/ball/actions/runs/25899905204),
   Build (Windows) = 4625s).
2. `CMAKE_<LANG>_COMPILER_LAUNCHER=ccache` (already set in the `ci-windows`
   preset since Phase 4 — [CMakePresets.json:71-72](../../../CMakePresets.json#L71))
   actually takes effect. MSBuild silently ignores `COMPILER_LAUNCHER`;
   Ninja honors it. This is the entire point of the prep commit
   [`9c932eb`](https://github.com/BALL-Project/ball/commit/9c932eb)
   that preinstalls ccache on the Windows runner.
3. macOS / Linux gain the marginal Ninja-vs-Make speedup (small in
   absolute terms — both jobs are already ccache-warm and finish in
   ~3 minutes — but worth the uniformity).

## Why now (milestone retarget v2.0 → v1.6.1)

Originally backlogged for v2.0 substrate-modernization. **Promoted into
v1.6.1** on 2026-05-16: with the Windows job promoted to `blocking: true`
(Phase 4 / Plan 04-04), every Phase 5.1 / 04.1 / future-fix CI cycle costs
~80 minutes of Windows wall-clock. Cutting that to the Ninja+ccache regime
pays back its own implementation cost on the first re-run. It also makes
the standing conditional disable policy (re-enable continue-on-error if
Windows >2× macOS/Linux total job time) tractable — the gap shrinks
dramatically once MSBuild is out of the loop.

The trigger phrase from `.planning/ROADMAP.md` Phase 999.2 stays valid:
*"Pure build-tooling change — zero source changes, just `cmake -G Ninja`."*
Carrying out a pure-tooling, zero-source-impact change inside a
strict-corrective patch milestone is acceptable per the v1.6.1 scope
discipline (`MILESTONE-CONTEXT.md` §"Scope discipline (post-review)"):
risk is contained to CI YAML + `CMakePresets.json`, not to BALL/VIEW source.

## Scope

**In scope:**
- `CMakePresets.json` — add `"generator": "Ninja"` to all three `ci-*`
  presets. Plain `Ninja` (single-config), not `Ninja Multi-Config`:
  current behaviour is single-config Release, no benefit from MC.
- `.github/workflows/ci.yml` — provision `ninja` on each runner:
  - Linux: `ninja-build` via apt (no-op if already present on
    `ubuntu-24.04` image; harmless to declare explicitly).
  - macOS: `brew install ninja` (sibling to existing `brew install ccache`).
  - Windows: `choco install ninja -y --no-progress` (sibling to existing
    `choco install ccache`).
- Windows MSVC environment for direct `cl.exe` invocation under Ninja:
  add `ilammy/msvc-dev-cmd@v1` before the Configure step. MSBuild had
  this implicit; Ninja calls `cl.exe` directly and needs the
  `vcvars64.bat` env populated.
- `.planning/ROADMAP.md` — flip Phase 999.2 from
  `BACKLOG · TARGETED FOR v2.0` to v1.6.1 active, with the predecessor
  prep-commits and this PLAN.md linked.
- `.planning/PROJECT.md` — drop 999.2 from the v2.0 substrate bundle
  and from the backlog one-liner.

**Out of scope:**
- BALL / VIEW / BALLView source files. No CMake target rules change.
  No custom-command rewrites — BALL's existing CMakeLists.txt is
  generator-agnostic (no MSBuild-only or Make-only escape hatches in
  use, per `ROADMAP.md` 999.2 risk paragraph).
- Local developer presets (`macos-homebrew`, `linux-system`,
  `windows-vcpkg`). Keep using the platform's default generator locally
  to match existing `BUILD-macos.md` instructions; only CI flips.
- `release.yml`. Release builds are infrequent and ccache-warmth is a
  non-goal there. Out of scope for this phase; revisit in v1.6.2 if
  Windows release artifact time becomes a pain point.
- Local Windows / Linux build instructions (`BUILD-*.md` if/when added).
  Touched separately when Phase 8 (Packaging & Distribution) does the
  per-platform build-from-source docs.

## Success criteria (what must be TRUE)

1. All three `ci-*` presets in `CMakePresets.json` declare
   `"generator": "Ninja"`.
2. CI run on `v1.6-modernization` HEAD is green on **macOS-arm64,
   Linux-x64, AND Windows-x64** with the new generator.
3. Windows `Build` step duration drops materially vs. the
   pre-Phase-999.2 baseline of 4625s on cold-cache, and produces a
   ccache hit rate >90% on a second build of the same SHA (the entire
   ccache premise).
4. macOS / Linux `Build` step durations do not regress (small
   improvements are bonus; the floor is "no worse than current").
5. All three platforms still pass their existing smoke checks
   (render smoke + GL profile assertion on macOS/Linux; OpenBabel smoke
   on macOS/Linux). Windows has no smoke check today — that's unchanged.
6. The `BALL VIEW BALLView` target set builds green on all three
   platforms — no regression in linkage / DLL hygiene / generator-
   specific quirks.

## Task list

**999.2-01 — Retarget planning docs.** *(Partially landed in `a2c0851`
by a parallel milestone-tracking session; this session lands the
remainder.)*
- ✅ Already landed by `a2c0851`: remove the empty placeholder dir
  `.planning/phases/999.2-ninja-build-generator/.gitkeep`; retarget
  `.planning/ROADMAP.md` Phase 999.2 entry from
  "BACKLOG · TARGETED FOR v2.0" → v1.6.1 active; v2.0 substrate-bundle
  paragraph updated.
- This session: drop 999.2 from the v2.0 substrate bundle + the backlog
  one-liner in `.planning/PROJECT.md` (BALL 2's commit didn't touch
  PROJECT.md).
- This session: add 999.2 to the `MILESTONE-CONTEXT.md` v1.6.1 in-scope
  list (mirrors the 999.7-Linux "Promoted from backlog" pattern).
- This session: land this PLAN.md.

**999.2-02 — `CMakePresets.json`: switch ci-* generators to Ninja.**
- Add `"generator": "Ninja"` to the `ci-macos`, `ci-linux`,
  `ci-windows` configure presets. Keep `CMAKE_C_COMPILER_LAUNCHER` /
  `CMAKE_CXX_COMPILER_LAUNCHER=ccache` — now honored on Windows too.
- Local-developer presets (`macos-homebrew`, `linux-system`,
  `windows-vcpkg`) stay generator-unspecified (CMake default applies).

**999.2-03 — Provision ninja per platform + Windows MSVC env for Ninja.**
- macOS: append `ninja` to the existing `brew install` line in
  "Install dependencies (macOS / Homebrew)".
- Linux: add `ninja-build` to the apt install list (harmless if
  already present on `ubuntu-24.04` image).
- Windows: fold `ninja` into the
  `.github/actions/setup-windows-deps/action.yml` composite action's
  `choco install` line — sibling to `winflexbison3` and `ccache`. This
  is the documented pattern per the composite's docstring (was
  pre-staged by BALL 2 in `fbc7a12`: *"`ninja` for Phase 999.2's
  generator switch should be folded into the same choco call here"*).
  Add `ilammy/msvc-dev-cmd@v1` as a separate step before Configure
  (Windows) so Ninja can invoke `cl.exe` directly with full
  `vcvars64.bat` env (MSBuild had this implicit; Ninja does not).
  `msvc-dev-cmd` is an action-using step, not a shell step, so it
  stays out of the composite.
- Keep `--parallel N` flags on the build commands. With Ninja they are
  optional (Ninja's default is all cores) but the explicit form keeps
  the YAML self-documenting.
- `--config Release` on the Windows build line is silently no-op under
  single-config Ninja (CMake docs). Leave it for transparency / future
  Multi-Config compatibility.

**999.2-04 — Verify via CI (push, watch, iterate).**
- Push commits 02 + 03. Wait for the CI run.
- If all three platforms go green: record per-platform `Build` step
  duration before/after.
- If Windows fails: most likely candidates and their
  minimal-workaround fixes:
  - **`cl.exe` not found** — `msvc-dev-cmd` step missing/misordered.
  - **PDB sharing / `/Zi` ccache miss** — switch Windows preset
    to `CMAKE_MSVC_DEBUG_INFORMATION_FORMAT=Embedded` (CMake 3.25+,
    requires `cmake_policy(SET CMP0141 NEW)`). For `CMAKE_BUILD_TYPE=Release`
    this is usually moot (no PDB), but if MSVC still emits one, this is
    the documented workaround.
  - **Vcpkg toolchain not picked up after env reset** — make sure the
    `$env:VCPKG_ROOT = $env:VCPKG_INSTALLATION_ROOT` bridge still runs
    AFTER `msvc-dev-cmd` (or move it before — vcvars doesn't clear
    user env vars, only PATH augmentations).
  - **ccache cache miss on first Ninja build** — expected and fine;
    the cache is cold for MSVC under the new launcher. Second build
    is the real test.

**999.2-05 — Sanity test ccache hit on Windows.**
- Trigger a no-op second CI run on the same SHA (push an empty commit
  to a throwaway branch, or use `gh workflow run`). Confirm Windows
  Build step time drops dramatically AND `ccache -s` shows >90% hit
  rate. Capture both numbers for the SUMMARY.

**999.2-06 — Close phase.**
- Write `999.2-SUMMARY.md` with the before/after duration table,
  ccache hit rate, all commit SHAs, and links to the green CI runs.
- Update `.planning/ROADMAP.md` Phase 999.2 to `[x] Complete` with
  the green-CI provenance.
- Update `.planning/MILESTONE-CONTEXT.md` v1.6.1 progress / Release
  criteria where 999.2 is referenced.
- Update `.planning/STATE.md` cursor if appropriate.

## Risks and mitigations

| Risk | Likelihood | Mitigation |
|------|------------|------------|
| Windows fails after generator swap (cl.exe / PDB / vcpkg-env issue) | Medium | `msvc-dev-cmd` action handles cl.exe + vcvars. Release build skips PDB by default. Vcpkg bridge runs in-shell, unaffected by vcvars. Documented workarounds listed under task 04. |
| macOS or Linux build regresses | Low | Both are already cache-warm; Ninja vs Make is a wash, marginally faster on cold caches. If it regresses, revert that platform's preset entry — independent across platforms. |
| ccache cache key churn invalidates Linux/macOS warm cache | Low (one-time) | First run after switch loses cache. Second run rewarms. Cost is one extra ~80s build per platform; absorbed in the verification cycle. |
| `BISON`/`FLEX` custom commands behave differently under Ninja | Very Low | CMake's `BISON_TARGET` / `FLEX_TARGET` emit generator-agnostic `add_custom_command`s. Verified by inspecting `cmake/CMakeLists.txt` (not modified by this phase). |
| The `ball_contrib` `-j` undefined-behaviour hazard mentioned in the original ROADMAP entry | N/A | `ball_contrib` was deleted in Phase 4. Hazard does not apply. |

## Stop conditions

- A platform regresses (build breaks or wall-clock significantly worse than
  baseline): stop, do NOT push through, fix root cause first.
- Discovery of a hidden gotcha (e.g. PDB-sharing on `/Zi`) requiring more
  than the documented minimal workaround: apply minimal fix + document in
  SUMMARY, do NOT scope-creep into a broader MSVC-flags audit.

## References

- [`CMakePresets.json`](../../../CMakePresets.json) — current ci-* presets
- [`.github/workflows/ci.yml`](../../../.github/workflows/ci.yml) — current
  per-platform install + configure + build steps
- [`.planning/ROADMAP.md`](../../ROADMAP.md#L285) — Phase 999.2 entry
  (being retargeted by 999.2-01)
- [`.planning/MILESTONE-CONTEXT.md`](../../MILESTONE-CONTEXT.md) — v1.6.1
  scope discipline
- CMake docs: [`CMAKE_GENERATOR`](https://cmake.org/cmake/help/latest/variable/CMAKE_GENERATOR.html),
  [`CMAKE_<LANG>_COMPILER_LAUNCHER`](https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_LAUNCHER.html)
- [ilammy/msvc-dev-cmd](https://github.com/ilammy/msvc-dev-cmd) —
  vcvars64 env for non-MSBuild invocations
- Baseline: CI run [25899905204](https://github.com/BALL-Project/ball/actions/runs/25899905204)
  (last green before phase) — Windows Build = 4625s, macOS Build = 67s,
  Linux Build = 99s.
