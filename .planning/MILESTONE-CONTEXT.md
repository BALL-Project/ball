---
milestone: v1.6.1
milestone_name: "Patch — re-ship v1.6 with the cleanup landed"
status: draft (awaiting /gsd-new-milestone)
drafted: 2026-05-15
predecessor: v1.6 (which shipped as v1.6.0 with broken Windows installer + empty macOS Info.plist version strings)
---

# Milestone v1.6.1 — Context (Pre-Workflow Draft)

> This file is a lightweight scope doc for the v1.6.1 milestone, intended to be
> picked up by `/gsd-new-milestone v1.6.1` (which scans for
> `.planning/MILESTONE-CONTEXT.md`). It captures the scope, in-flight work, and
> release criteria so the formal workflow can short-circuit its questioning step.

## Goal

**Ship what v1.6.0 should have shipped.** v1.6.0 tagged on 2026-05-15 but the
Windows installer was skipped (LNK1104 `tbb12_debug.lib`), the macOS bundle's
Info.plist had empty version strings, and tri-OS CI was never green on the
release branch. v1.6.1 is the corrective re-release: same modernization payload,
but actually buildable, actually installable, actually verifiable on all three
OSes.

Phase 5.1 already landed the source-level fixes; v1.6.1 is mostly verification +
re-tag + a small user-facing bug fix (Phase 4.1).

## Goal one-liner

> *"v1.6.1: deliver what v1.6.0 promised — clean tri-OS installers, populated
> macOS bundle metadata, and the persisted-config color-shadow bug fixed."*

## In scope

### 1. Phase 5.1 carry-forward (LANDED)

Source-level work complete in 14 plans on `v1.6-modernization`:

- **A1–A6** Tier-A correctness fixes (getline recursion, pointer truncation,
  self-assign, tautological compare, format-overflow, stringop-truncation)
- **B1+B2** Tier-B Windows DLL hygiene (`extern template class` / `BALL_EXPORT`
  migration in `vector3.{h,C}` + `atom.{h,C}`)
- **B3** project-wide `#pragma warning(disable: 4251)` in `global.h`
- **B4+B5** Tier-B cosmetic Windows fixes
- **D1+D6** Qt6LinguistTools `QUIET` + vcpkg `qttools`
- **D2** GitHub Actions Node 24 action pins (`@v5`)
- **D3** Linux apt-archives cache narrowed (was misclassified in BACKLOG as ccache)
- **D4** Windows release `--config Release` (Windows installer unblock)
- **D5** macOS `CFBundleIdentifier` (`de.uni-tuebingen.ball.ballview`) +
  `CFBundleVersion` (via `${CMAKE_PROJECT_VERSION}` fix in commit `61aff67`)

Plus two in-cycle gap fixes:
- vcpkg `builtin-baseline` regression repair (commit `08ec9c5`)
- Linux `aqtinstall` Qt 6.5+ module-list fix (commit `54da903`)

**Verification gate:** tri-OS CI run on commit `54da903` (or successor) must be
green, with the warning-census drops visible per UAT tests 3–11 in
[`05.1-UAT.md`](.planning/phases/05.1-build-warnings-and-latent-bugs/05.1-UAT.md).

### 2. Phase 4.1 — Config Color-Defaults Fix (IN PARALLEL JOB — user-driven)

Persisted `~/.BALLView` config was silently shadowing compiled element / residue
color defaults. Real user-facing bug. Promoted from backlog 999.4. User is
running this in a separate job in parallel.

**Verification gate:** Phase 4.1's own SUMMARY + UAT. Once it lands on
`v1.6-modernization` and merges with Phase 5.1, the v1.6.1 milestone has the
patch payload it needs.

### 3. Re-tag and re-release (PENDING — gated on 1 + 2)

- Tag `v1.6.1` on the green HEAD of `v1.6-modernization` after CI verification
- `gh workflow run release.yml -f tag=v1.6.1`
- Confirm both installers ship:
  - `BALLView-v1.6.1-macos-arm64.zip` (with populated Info.plist + ad-hoc resign
    from D4-mac already in place)
  - `BALLView-v1.6.1-windows-x64.zip` (the missing v1.6.0 installer)
- Attach to the GitHub Release; supersede v1.6.0's incomplete asset list

### 4. Optional — Phase 8 minimal macOS notarization slice (CONDITIONAL)

Only if a Developer ID Application certificate is available to the maintainer.
The Phase 5.1 prereqs are satisfied (`CFBundleIdentifier` correct,
`CFBundleVersion` non-empty, D4-mac ad-hoc resign in place).

- `codesign --sign "Developer ID Application: ..." --options runtime BALLView.app`
- `notarytool submit BALLView-v1.6.1-macos-arm64.zip --apple-id ... --wait`
- `xcrun stapler staple BALLView.app`

If no certificate is available, **defer to Phase 8 proper** in a later milestone.
v1.6.1 still ships with the existing ad-hoc-resign macOS bundle which loads on
macOS 26+ but requires the right-click→Open dance.

## Out of scope (defer to v1.7 / v1.6.2 / later)

| Item | Why deferred | Target |
|------|--------------|--------|
| **Phase 6** (Python Bindings) | Vertical-slice decision spike (SIP 6 vs pybind11 vs nanobind) is its own milestone-shape; doesn't fit a patch release | v1.7-track |
| **Phase 8 full** (BUILD-linux.md, BUILD-windows.md, license/distribution review, full notarization wiring) | Bigger scope than a patch; the macOS notarization slice can land in v1.6.1 conditionally | v1.6.2 or v1.7 |
| **Phase 9** (Test Suite Triage) | Already in flight via commits `b2bb718` + `61bf5a7`; may finish before v1.6.1 ships, in which case it folds in. Otherwise its own milestone. | Possibly v1.6.1 if it finishes; else v1.6.2 |
| **Action artifact pins** (`upload-artifact@v4` → v6, `download-artifact@v4` → v7) | Deferred from Phase 5.1 Plan 11; cross breaking-change majors | v1.6.2 |
| **Tier-C warning bulk cleanup** (~3700 warnings of `-Wdeprecated-copy`, `-Wunqualified-std-cast-call`, `-Wcatch-value`, etc.) | Explicitly out of Phase 5.1 scope per CONTEXT.md D-04 | v1.6.2 or v1.7 |
| **BALLView UI refresh** (SEED-001) | The v1.7 milestone proper | v1.7 |
| **B3 measurement retroactive validation** | Plan 08 applied the C4251 pragma by default without the measurement (CI cancellations); next clean tri-OS run records the actual count. Non-blocking. | v1.6.1 verification side-band |

## Release criteria

A v1.6.1 release is shippable when **all five** are true:

1. **Tri-OS CI green** on the target HEAD — macOS arm64, Linux x64, Windows x64
   all reach `[100%] Built target BALL` + their respective smoke checks
2. **Phase 5.1 UAT tests 3–11 PASS** — warning census on each toolchain shows
   the targeted categories at zero on the matched files
3. **Phase 4.1 merged + UAT-confirmed** — color defaults bug user-verified fixed
4. **Both installers attached to the GitHub Release** — Windows zip (new) +
   macOS arm64 zip (re-shipped with populated Info.plist + functional bundle
   identity)
5. **No new C4717 / C4311 / C4910 / C4834 regressions** in the post-tag CI run
   — i.e. v1.6.1's source-level cleanup is not undone by any commit between
   Phase 5.1 close-out and the tag

## Carry-over from v1.6 STATE

The v1.6 milestone's accumulated context (decisions, blockers, todos) survives
the milestone switch — symmetric with `gsd-sdk query state.milestone-switch`.
v1.6.1 builds on:

- Phase 1–5 modernization decisions (all locked)
- Phase 02.1 renderer boundary (foundation for Phase 5 backend swap)
- Phase 02.2 CI matrix (the verification oracle)
- Phase 5 Qt 6 + SPIKE-02 decision (GL-Core for v1.6.x → QRhi for v2)
- Phase 5.1 LEARNINGS (BACKLOG misclassification patterns, Codex cross-check
  value, vcpkg baseline trap, nested-PROJECT shadow)

## Roadmap delta from v1.6

If `/gsd-new-milestone v1.6.1` is run, the resulting ROADMAP.md changes are
minimal:

- **No new phases.** v1.6.1's scope is verification + re-tag + Phase 4.1
  (already in ROADMAP.md as an open phase).
- **Optional: insert "Phase 8.0 — macOS notarization slice"** if Developer ID
  is available. Otherwise Phase 8 stays whole and defers.
- **STATE.md frontmatter** flips `milestone: v1.6` → `milestone: v1.6.1`;
  status resets to `planning`.

## Open questions

1. **Developer ID Application certificate available?** Drives whether the
   optional Phase 8 slice lands in v1.6.1 or defers.
2. **Phase 9 timing?** If the in-flight test-suite work (`b2bb718`, `61bf5a7`)
   reaches a stable point before v1.6.1 tags, it can fold into the patch
   release as a bonus.
3. **v1.6.0 retraction strategy?** v1.6.0 is already tagged + a partial release
   exists (macOS only). Options: (a) leave v1.6.0 as-is and just ship v1.6.1
   normally, (b) edit the v1.6.0 release notes to point at v1.6.1, (c) delete
   the v1.6.0 release. (a) is simplest; recommend (a) + (b) for clarity.

## Next action

Run `/gsd-new-milestone v1.6.1` once tri-OS CI green is confirmed on commit
`54da903`. The workflow will pick this file up and short-circuit its
requirements-gathering step.
