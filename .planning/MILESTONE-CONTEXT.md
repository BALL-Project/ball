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

### 5. Upstream PR triage and integration (NEW — Phase 5.2 candidate)

Five PRs sit open on `master` from 2015–2017 — pre-modernization community
contributions never reviewed by the current maintainer. Most don't fit the
post-Phase-4 architecture directly; this slot triages each, integrates what's
salvageable into `v1.6-modernization`, and closes the rest.

Suggested new phase number: **Phase 5.2** (inserted after 5.1; pure
maintenance + integration, no architectural decisions). Plan-shape: one plan
per disposition group.

| PR | Author / Year | Files | Disposition | Plan |
|---|---|---|---|---|
| [#640](https://github.com/BALL-Project/ball/pull/640) — Refactored FindXDR.cmake | philthiel / 2017 | `cmake/FindXDR.cmake`, `cmake/BALLConfiguration.cmake` | **Investigate-then-close-or-rebase**: Phase 4 moved to config-mode `find_package` for the deps that had upstream configs; XDR has no upstream config so `FindXDR.cmake` still exists in tree (verified). If XDR is still a build dep on any OS, rebase this PR against the post-Phase-4 CMake; if XDR has been replaced/dropped, close as obsolete with explanation. | Plan A |
| [#600](https://github.com/BALL-Project/ball/pull/600) — Travis-CI integration | dannyedel / 2016 | `.travis.yml`, `cmake/BALLDoc.cmake`, `doc/tools/makedoc`, `include/BALL/CONCEPT/classTest.h`, `source/TEST/data/PoseClustering_wardtree.dat` | **Split-disposition**: `.travis.yml` is fully obsolete (Travis-CI .com sunset 2021; GitHub Actions matrix is the CI of record via Phase 02.2 — close that part). The other 4 files (`BALLDoc.cmake`, `makedoc`, `classTest.h`, `PoseClustering_wardtree.dat`) may carry orthogonal small improvements; review individually and cherry-pick what's still valid. | Plan B |
| [#554](https://github.com/BALL-Project/ball/pull/554) — Omega torsion angles | smoe / 2015 | `residueRotamerSet.{h,C}`, `residue.C`, `residue.sip`, `residueRotamerSet.sip` | **Split-disposition**: Extract the C++ slice (`residue.C` + `residueRotamerSet.{h,C}`) — pure C++ addition that gives BALL omega-dihedral handling alongside psi/phi. Defer the `.sip` Python additions to Phase 6 (binding generator decision pending). | Plan C |
| [#550](https://github.com/BALL-Project/ball/pull/550) — Hydroxyproline (HYP) as standard AA | smoe / 2015 | `PRO.db`, `peptideBuilder.{h,C}`, `residue.C`, `peptides.C` | **Rebase + merge** — pure C++ + data addition; no Python parts; closes a real gap (HYP appears in 157 PDB entries, currently surfaces as `unknown/?` with no torsion angles). The smallest single PR-merge win in the set; ships in v1.6.1. | Plan D |
| [#546](https://github.com/BALL-Project/ball/pull/546) — Print residue with insertion code | smoe / 2015 | `residue.{h,C}`, `residue.sip` | **Split-disposition**: Extract the C++ slice (`residue.h` + `residue.C`) — extends `getFullName()` to include insertion code (PDB residues like `100A`) + extends the `FullnameType` enum. Defer the `.sip` part to Phase 6. | Plan C (bundles with #554) |

**Total v1.6.1 PR-integration payload** when all dispositions complete: ~2 PRs
merged (the C++ slices of #550 and the C++ slices of #554 + #546 bundled),
~2 PRs closed as obsolete (#600 `.travis.yml` part, #640 if XDR is gone),
~3 PRs deferred (the `.sip` portions of #554 and #546, plus any unresolved
#600 cherry-pick decisions and the #640 verdict).

**Caveats:**
- Each PR is ~9–10 years old; rebasing may surface conflicts with Phase 1–5
  modernization (C++17 changes in Phase 3, header reorganization, etc.).
- Author engagement is unlikely (the smoe PRs are from 2015). Maintainer
  cherry-picks the diffs rather than waiting for rebases.
- HYP support (#550) and the omega-torsion C++ slice (#554) may interact —
  smoe authored both and #554's body mentions hydroxyproline rich-ness in
  collagen as motivation. Land #550 first; rebase #554's C++ slice on top.

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

A v1.6.1 release is shippable when **all six** are true:

1. **Tri-OS CI green** on the target HEAD — macOS arm64, Linux x64, Windows x64
   all reach `[100%] Built target BALL` + their respective smoke checks
2. **Phase 5.1 UAT tests 3–11 PASS** — warning census on each toolchain shows
   the targeted categories at zero on the matched files
3. **Phase 4.1 merged + UAT-confirmed** — color defaults bug user-verified fixed
4. **Phase 5.2 (PR triage) complete** — each of the 5 open PRs has a
   recorded disposition (merged / cherry-picked / closed-with-explanation /
   deferred-to-Phase-6); the GitHub PR list shows 0 PRs in undecided state
5. **Both installers attached to the GitHub Release** — Windows zip (new) +
   macOS arm64 zip (re-shipped with populated Info.plist + functional bundle
   identity)
6. **No new C4717 / C4311 / C4910 / C4834 regressions** in the post-tag CI run
   — i.e. v1.6.1's source-level cleanup is not undone by any commit between
   Phase 5.1 close-out and the tag (relevant for #550 HYP / #554 omega / #546
   insertion-code rebases, which touch `residue.C` and may introduce new
   warnings on Windows MSVC if not careful)

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

If `/gsd-new-milestone v1.6.1` is run, the resulting ROADMAP.md changes are:

- **Insert Phase 5.2 — Upstream PR Triage and Integration.** Sequenced after
  Phase 5.1; depends on no prior phase technically (the PRs target `master`),
  but rebases against `v1.6-modernization` which assumes Phase 1–5 is in.
  Suggested plan breakdown:
  - Plan A — #640 FindXDR investigate-then-close-or-rebase
  - Plan B — #600 .travis.yml close + cherry-pick of orthogonal changes
  - Plan C — #554 + #546 C++ slice extraction (bundled — both touch `residue.C`,
    both have `.sip` parts that defer)
  - Plan D — #550 HYP residue rebase + merge (touches `residue.C` too;
    sequence after Plan C OR merge with Plan C if conflict surface is small)
- **Optional: insert "Phase 8.0 — macOS notarization slice"** if Developer ID
  is available. Otherwise Phase 8 stays whole and defers.
- **Phase 4.1** stays in ROADMAP.md as already-open.
- **STATE.md frontmatter** flips `milestone: v1.6` → `milestone: v1.6.1`;
  status resets to `planning`.

## Open questions

1. **Developer ID Application certificate available?** Drives whether the
   optional Phase 8 slice lands in v1.6.1 or defers.
2. **Phase 9 timing?** If the in-flight test-suite work (`b2bb718`, `61bf5a7`)
   reaches a stable point before v1.6.1 tags, it can fold into the patch
   release as a bonus.
3. **XDR still a BALL dependency?** Drives #640's disposition: if XDR is gone
   (replaced or no longer used by any active code path), the PR closes as
   obsolete; if it's still in use, the PR rebases. Quick check during Plan A:
   `grep -rn "XDR\|FindXDR" CMakeLists.txt include/ source/ | grep -v ball_contrib`.
4. **Maintainer policy on auto-closing PRs from departed contributors?** The
   2015-era PRs from `smoe` (#554, #550, #546) are technical wins, but the
   author may not be available to rebase. Phase 5.2 should default to
   maintainer-driven cherry-picks rather than waiting for the author. Confirm
   this is OK for the institutional record.
5. **v1.6.0 retraction strategy?** v1.6.0 is already tagged + a partial release
   exists (macOS only). Options: (a) leave v1.6.0 as-is and just ship v1.6.1
   normally, (b) edit the v1.6.0 release notes to point at v1.6.1, (c) delete
   the v1.6.0 release. (a) is simplest; recommend (a) + (b) for clarity.

## Next action

Run `/gsd-new-milestone v1.6.1` once tri-OS CI green is confirmed on commit
`54da903`. The workflow will pick this file up and short-circuit its
requirements-gathering step.
