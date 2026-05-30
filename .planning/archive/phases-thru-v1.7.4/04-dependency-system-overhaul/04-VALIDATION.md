---
phase: 4
slug: dependency-system-overhaul
status: planned
nyquist_compliant: true
wave_0_complete: false
created: 2026-05-14
---

# Phase 4 — Validation Strategy

> Per-phase validation contract for feedback sampling during execution.

---

## Test Infrastructure

| Property | Value |
|----------|-------|
| **Framework** | CMake configure + build; GitHub Actions matrix (macOS-arm64 / Linux / Windows) + one standalone OpenBabel chemistry smoke driver |
| **Config file** | `CMakePresets.json` (new this phase), `vcpkg.json` (new this phase), `.github/workflows/ci.yml` |
| **Quick run command** | `cmake --preset macos-homebrew && cmake --build --preset macos-homebrew --target BALL VIEW BALLView` |
| **Full suite command** | CI matrix run across all jobs (build macos / linux / windows + lint), render smoke (macOS/Linux), OpenBabel smoke (macOS/Linux) |
| **Estimated runtime** | local configure+build minutes; CI matrix ~tens of minutes |

---

## Sampling Rate

- **After every task commit:** Re-run `cmake --preset <platform>` configure (and build the touched target) and confirm the touched dependency resolves
- **After every plan wave:** Full local build on the development platform (macOS) + push to trigger the CI matrix
- **Before `/gsd-verify-work`:** CI matrix green on all jobs — including the Windows job flipped to a required check (DEPS-02)
- **Max feedback latency:** local configure ~seconds; full CI ~tens of minutes

---

## Per-Task Verification Map

| Task ID | Plan | Wave | Requirement | Threat Ref | Secure Behavior | Test Type | Automated Command | File Exists | Status |
|---------|------|------|-------------|------------|-----------------|-----------|-------------------|-------------|--------|
| 04-01-T1 | 01 | 1 | DEPS-03 | T-04-01, T-04-03 | Config-mode finders carry pinned min versions; no untrusted finder substitution | build | `cmake -S . -B build/04-verify ... && cmake --build build/04-verify --target BALL`; configure log shows `Found TBB ... config mode` | ✅ CMakeLists.txt | ⬜ pending |
| 04-01-T2 | 01 | 1 | DEPS-01, DEPS-04 | T-04-02 | `BALL_CONTRIB_PATH` configure-time injection vector removed; min versions pinned | configure | `grep -rn BALL_CONTRIB_PATH CMakeLists.txt cmake/` clean (bar FindSIP comment) + `cmake -S . -B build/04-verify2` exits 0 | ✅ CMakeLists.txt | ⬜ pending |
| 04-01-T3 | 01 | 1 | DEPS-03 | — | OpenBabel GPL-gate decision recorded, not silently changed | checkpoint | maintainer selects lgpl-ok / gpl-gated | ✅ checkpoint | ⬜ pending |
| 04-02-T1 | 02 | 1 | DEPS-05 | T-04-05, T-04-06 | Presets are version-controlled; `windows-vcpkg` toolchainFile resolution documented; ci-* presets carry `BALL_HAS_OPENBABEL=ON` (single source of truth) | build | `python3 -c "..."` asserts preset names + `ci-macos`/`ci-linux` set `BALL_HAS_OPENBABEL=ON` + `cmake --preset macos-homebrew` exits 0 | ❌ → CMakePresets.json | ⬜ pending |
| 04-02-T2 | 02 | 1 | DEPS-05 | T-04-08 | CI invokes version-controlled presets; resolved flags auditable via `cmake --preset -N`; D-07 → ci-* mapping documented | build/CI | `grep "preset macos-homebrew" BUILD-macos.md` + `grep "preset ci-" ci.yml` + YAML parse | ✅ ci.yml | ⬜ pending |
| 04-02-T3 | 02 | 1 | FEAT-01 | — | N/A (doc) | doc review | `grep -nE "not a vcpkg port\|non-PIC\|auto-disable" REQUIREMENTS.md` | ✅ REQUIREMENTS.md | ⬜ pending |
| 04-03-T1 | 03 | 2 | DEPS-05 (D-05) | T-04-09, T-04-10 | OpenBabel 3.x port; pinned to >=3.0 so 2.x parser CVEs N/A | build | single `cmake --preset macos-homebrew -DBALL_HAS_OPENBABEL=ON` (captured to `/tmp/04-03-cfg.log`) + `cmake --build build/macos-homebrew --target BALL` exits 0; configure LOG (not CMakeCache.txt) greps `Found OpenBabel`; no 2.x symbols remain | ✅ molecularSimilarity.C | ⬜ pending |
| 04-03-T2 | 03 | 2 | DEPS-05 (D-05) | T-04-09 | TOOLS ported to 3.x API; no 2.x symbols | build | `cmake --build ... --target MolDepict ProteinProtonator Ligand3DGenerator` exits 0 | ✅ TOOLS *.C | ⬜ pending |
| 04-03-T3 | 03 | 2 | DEPS-05 (D-05) | T-04-10, T-04-11 | Smoke driver catches silent implicit-H/aromaticity corruption | smoke | `bash .planning/phases/04-.../scripts/openbabel-smoke.sh` prints `OPENBABEL_SMOKE_OK` | ❌ → openbabel-smoke.{C,sh} | ⬜ pending |
| 04-03-T4 | 03 | 2 | DEPS-05 (D-05) | T-04-11 | CI exercises the port via the ci-* presets (cache var lives in the preset, not a divergent CLI arg); smoke step version-controlled, read-only-token runner | CI | `grep -n "BALL_HAS_OPENBABEL" CMakePresets.json` (confirms ON for ci-macos/ci-linux) + `grep -n "openbabel-smoke.sh" ci.yml` + YAML parse + green CI; `ci.yml` has NO `-DBALL_HAS_OPENBABEL=ON` CLI arg | ✅ ci.yml | ⬜ pending |
| 04-04-T1 | 04 | 2 | DEPS-02 | T-04-13, T-04-15 | `vcpkg.json` baseline pinned to a 40-char SHA; no non-port deps | configure | `python3 -c "..."` asserts pinned baseline + verified port set | ❌ → vcpkg.json | ⬜ pending |
| 04-04-T2 | 04 | 2 | DEPS-02 | T-04-14, T-04-16, T-04-17 | choco pkg name pinned; vcpkg cached; ephemeral read-only runner; Windows job runs as a genuine pass/fail step (no `\|\| true` no-op) | build/CI | `grep -nE "winflexbison3\|preset ci-windows" ci.yml` + no `\|\| true` + valid YAML + cache@v4 step exists; push so the Windows job RUNS with real provisioning and reaches the configure/build step (build-to-green is 04-04-T3's job) | ✅ ci.yml | ⬜ pending |
| 04-04-T3 | 04 | 2 | DEPS-02 | — | Windows iterated to green on 2+ runs before flipped to required | checkpoint | maintainer drives `gh run watch` → fix loop to green, confirms Windows job green 2x + `blocking: true` committed + green | ✅ checkpoint | ⬜ pending |

*Status: ⬜ pending · ✅ green · ❌ red · ⚠️ flaky*

---

## Wave 0 Requirements

> "Wave 0" in 04-RESEARCH.md maps to **Wave 1** in the final plan numbering (Plans 01 + 02).

- [ ] `CMakePresets.json` (Plan 02 / Wave 1) — the configure presets that all subsequent verification depends on (D-07); the `ci-macos` / `ci-linux` presets carry `BALL_HAS_OPENBABEL=ON` so Plan 03's CI OpenBabel enablement stays preset-driven
- [ ] The 3 config-mode finder migrations + `ball_contrib` removal + version pins (Plan 01 / Wave 1) — the CMake foundation Plans 03 + 04 build on
- [ ] Confirm CI matrix from Phase 02.2 still green as the regression baseline before dependency changes land
- [ ] CI preset must enable `BALL_HAS_OPENBABEL=ON` on macOS/Linux (authored into the ci-* presets by Plan 02, smoke step added by Plan 03 / Wave 2) so the D-05 port is genuinely exercised — the one real verification gap the research flagged

*Build-system phase — verification is configure/build/CI success plus one OpenBabel chemistry smoke driver, not a unit-test framework install.*

---

## Manual-Only Verifications

| Behavior | Requirement | Why Manual | Test Instructions |
|----------|-------------|------------|-------------------|
| OpenBabel 3.x integration produces correct chemistry (implicit-H, aromaticity) | DEPS-05 (D-05) | Semantic correctness of the 2.4→3.x port can't be fully proven by "it compiles" — partly automated now by the Plan 03 Task 3 smoke driver (non-empty + deterministic + aromaticity-correct SMILES); a deeper reference-molecule comparison stays manual | Build with OpenBabel enabled, run `molecularSimilarity` against a reference molecule pair, compare similarity output to a pre-port baseline |
| BALLView still launches and renders after the dependency overhaul | DEPS-01 | GUI smoke — the dependency graph feeds the renderer; the Phase 02.2 render smoke check carries this on macOS/Linux but a human launch confirms no regression | Launch `BALLView`, load a molecule, confirm the 3D scene renders |
| Windows build is genuinely, repeatably green before being made required | DEPS-02 | "Windows green" is only verifiable via CI; a one-off green can be a fluke (Pitfall 2) — needs human judgement on repeatability before flipping `blocking: true`. The build-to-green `gh run watch` iteration loop is owned by Plan 04 Task 3 (the checkpoint), not Task 2 | Plan 04 Task 3 checkpoint: drive the Windows CI job to green, confirm green on 2+ consecutive runs, then flip `blocking: true` |

---

## Validation Sign-Off

- [x] All tasks have an automated verify (configure/build/CI/smoke) or are an explicit checkpoint
- [x] Sampling continuity: no 3 consecutive tasks without automated verify (the 2 checkpoints are bracketed by automated-verify tasks)
- [x] Wave 1 covers `CMakePresets.json` + the finder/contrib foundation (everything downstream configures through them)
- [x] No watch-mode flags
- [x] Feedback latency acceptable (local configure fast; CI matrix is the gate)
- [x] `nyquist_compliant: true` set in frontmatter

**Approval:** planned — ready for `/gsd-execute-phase 4`
</content>
