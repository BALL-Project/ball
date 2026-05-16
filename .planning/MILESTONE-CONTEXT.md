---
milestone: v1.6.2
milestone_name: "Build acceleration + scope-cleanup patch release"
status: draft (post-v1.6.1-tag; awaiting /gsd-new-milestone v1.6.2)
drafted: 2026-05-16
predecessor: v1.6.1 (tagged 2026-05-16, commit `8e5495a`; release publish pending in parallel session)
predecessor_context: MILESTONE-CONTEXT-v1.6.1.md (preserved)
---

# Milestone v1.6.2 — Context (Pre-Workflow Draft)

> Pre-workflow scope doc for v1.6.2, intended to be picked up by
> `/gsd-new-milestone v1.6.2` (which scans for
> `.planning/MILESTONE-CONTEXT.md`). Captures the scope, in-flight work,
> and release criteria so the formal workflow can short-circuit its
> questioning step.

## Goal

**Bigger than v1.6.1, still corrective in shape.** v1.6.1 was the strict
corrective re-release of v1.6.0. v1.6.2 is a substantive patch release
that ships the build-acceleration cluster, closes the deferred-from-5.1
cleanup items, finishes Phase 9 (test suite triage), and runs the
GitHub issue + PR triage that's been queued since v1.6.1 tagged.

User direction (2026-05-16 roadmap discussion):
> "v1.6.2 will be a big chunk of work, but mostly corrective, so keep it
> in 1.6.2."

Not a minor release: nothing user-facing-new ships. Build cycle gets
faster. CI gets cleaner. Open issues/PRs get triaged. Tier-C warning
landscape gets censused (not all-cleaned).

## Goal one-liner

> *"v1.6.2: faster CI, cleaner tracker, deferred cleanups closed —
> patch shape, not feature shape."*

## In scope (7 phases + 1 stretch; revised post-2026-05-16 open-Q resolution)

Drawn from the post-2026-05-16 ROADMAP-AUDIT-V1.6.2.md + roadmap
discussion outcome + 2026-05-16 open-questions resolution (see §"Resolved questions" below). Effort estimates per ROADMAP entries.

**Open Q1 resolution:** Phase 6 (PyBALL bake-off) moves OUT of v1.6.x → **v2.1** as the first step of the PyBALL milestone (before [Phase 999.15](ROADMAP.md) bulk wrap). User direction: "PyBALL changes should move to 2.x." v1.6.2 effort drops from 11-21 days to **~7-13 days** with Phase 6 removed; v1.6.2 ships in a tight ~2-week window.

### Build-acceleration cluster (5 phases, ~3-5 days total)

Layered on top of [Phase 999.2 Ninja switch](ROADMAP.md) (already
landed in v1.6.1 — Windows Build 4818s → 55s warm, 87× speedup).
These five close the remaining marginal wins:

1. **[Phase 999.16](ROADMAP.md) — PCH for BALL + VIEW** — `target_precompile_headers` on the heavy Qt + Boost + Eigen + BALL/CONCEPT/COMMON header set. Targets 20-40% cold-cache speedup on template-heavy TUs. ~0.5-1 day.
2. **[Phase 999.17](ROADMAP.md) — Windows CMake build-tree cache** — `actions/cache` keyed on hash of CMakeLists/cmake/vcpkg.json/CMakePresets.json. Collapses warm-cache `Configure (Windows)` from ~2.5 min to seconds. ~0.5-1 day.
3. **[Phase 999.18](ROADMAP.md) — Path-aware CI triggers + concurrency groups** — skip CI runs on doc-only / planning-only changes. Concurrency groups cancel obsolete runs on rapid pushes. ~0.5 day.
4. **[Phase 999.19](ROADMAP.md) — Per-TU build profiling artifact** — ninja `.compile_commands.json` + timing breakdown uploaded as a CI artifact. Future-proof for identifying compile-time outliers. ~0.5 day.
5. **[Phase 999.20](ROADMAP.md) — Bump action artifact pins to v6/v7** — `upload-artifact@v4 → v6`, `download-artifact@v4 → v7`. Was Phase 5.1 D-deferred (cross-breaking-change majors). ~0.5 day.

### Source-level + cleanup (2 phases, ~5-12 days total)

6. **[Phase 9](ROADMAP.md) — Test Suite Triage (close)** — CI wiring already landed in `b2bb718` + `61bf5a7`; baseline at 99.0% (291/294) on macOS with `BALL_DATA_PATH` set. Remaining: triage 3 failures (`Directory_test`, `AmberFF_test`, `AssignBondOrderProcessor_test2`) + flip CI gatekeeper from `continue-on-error: true` to blocking. ~2-3 days.
7. **[Phase 999.14](ROADMAP.md) — GitHub issue + PR triage + bundled tasks** (per Open Q2 resolution: **triage first, then 5-PR bundle**) — (a) 5-category triage of open issues + open PRs (1-3 days, runs FIRST against the freshly-tagged v1.6.1 HEAD to clear noise); (b) **stale-doc audit** scanning all `*VERIFICATION.md` files for the HUMAN-UAT-disagreement pattern that misled the 2026-05 ROADMAP-AUDIT (0.5 day, runs alongside triage); (c) **5-PR legacy bundle** (#640 FindXDR, #600 Travis-CI, #554 Omega torsion, #550 hydroxyproline, #546 residue insertion code) — runs LAST after triage clears the categorization (2-5 days depending on rebase complexity). Total 3-9 days.

**Phase 6 (PyBALL bake-off) — moved to v2.1 per Open Q1.** No longer v1.6.x scope. Becomes the first step of the v2.1 PyBALL milestone, sequenced before [Phase 999.15](ROADMAP.md) bulk wrap. PYBALLV2.md §6 + §10 hard stop/pivot gates remain authoritative.

### Tiny dead-code + grammar cleanups (2 phases, ~1.5-3 days total)

8. **[Phase 999.21](ROADMAP.md) — DockResultFile QtXml dead-code cleanup** — drop the dead `QXmlAttributes` overload that was stubbed under `#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)`. Was Phase 5.1 BLOCKER-A2, never picked up. ~1 hour.
9. **[Phase 999.22](ROADMAP.md) — Warning census (CENSUS-ONLY per Open Q3)** — census of deferred Tier-C warnings (~3700) + residual Windows C4910/C4834. **Census-only — no execution.** Produces `.planning/phases/999.22-warning-census/CENSUS.md` + 3 backlog stubs: (a) mechanical-fix-now → deferred to v1.7 OR v2.0 depending on PIPE-01 dependency, (b) defer-to-v2.0-PIPE-01-dissolves, (c) defer-to-v1.7-needs-per-site-review. v1.6.2 ships with no actual warning reduction; the census IS the deliverable. ~1-2 days (was 3-5 with execution).

### Stretch (defer to v1.7 if v1.6.2 fills up)

10. **[Phase 999.23](ROADMAP.md) — CIF Bison grammar audit** — 3-5 shift-reduce conflicts in the CIF parser (count needs reconciliation). 1-2 days. Defer to v1.7 if v1.6.2 budget tight.

**Total v1.6.2 effort estimate: ~7-13 days** (build-accel 3-5d + Phase 9 close 2-3d + 999.14 triage 3-9d + 999.21 1h + 999.22 census 1-2d + 999.23 stretch 1-2d). Tight ~2-week patch window. With Phase 6 deferred to v2.x (per Open Q1) and 999.22 reduced to census-only (per Open Q3), the cycle ships substantially faster than the original 11-21 day estimate.

## Out of scope (defer to v1.7 / v2.x / later)

| Item | Why deferred | Target |
|------|--------------|--------|
| **Phase 8** (Packaging & Distribution) — full signed/notarized installers | Big-scope work; signing infrastructure requires Developer ID cert + SignPath registration. Per 2026-05-16 user direction "macos notarization and windows signing should sit in 1.7, not 1.6.2." | v1.7 |
| **Phase 999.8** (Auto-Update Sparkle + WinSparkle) | Depends on Phase 8 signing infra. | v1.7 (after Phase 8) |
| **Phase 999.1** (BALLView UI maintainer open-questions) | Long lead; ties to SEED-001 "BALLView Refresh" UI work. | v1.7 |
| **Phase 999.4b** (Residue color persistence mirror of Phase 4.1) | Small, fits anywhere but not patch-shape urgent. | v1.7 |
| **SEED-001 BALLView UI Refresh** | The headline v1.7 deliverable. | v1.7 |
| **Phase 999.25** (v1.4 → v1.7 retrospective + audit-procedure docs + user-facing docs audit) | Runs at v1.7 closure, after retrospective baseline is stable. | v1.7 closure |
| **Tier-C wholesale warning cleanup** (~3700) | 999.22 produces the split-list; the bulk (b)+(c) subsets defer. | v1.7 (c-subset) / v2.0 dependent on 999.6 PIPE-01 (b-subset) |
| **KERNEL v2.0 redesign** (999.24) | Lead v2.0 phase; 22-34 months. | v2.0 |
| **v2.0 substrate phases** (999.6 PIPE-01 / 999.9 YAML / 999.11 gemmi / 999.12 deprecated removal) | Substrate modernization bundle, co-shipping with KERNEL. | v2.0 |
| **PyBALL v2.1 bulk wrap** (999.15) | Conditional on Phase 6 bake-off outcome; targets v2.0 MoleculeStore. | v2.1 |
| **REST API + PyBALL SDK** (999.10) | Sequenced after KERNEL v2.0 K0+K1 + PyBALL v2.1. | v2.2 |
| **Read the Docs portal** (999.13) | Last v2.x phase; consumes v2.2 REST OpenAPI spec. | v2.3 |

## Release criteria (proposed)

A v1.6.2 release is shippable when **all five** are true (post-2026-05-16 open-Q resolution; was 6; criterion 5 removed because Phase 6 moved to v2.x):

1. **All 5 build-acceleration phases (999.16-999.20) landed** AND CI green on a clean tri-OS run.
2. **Phase 9 close: CI gatekeeper flipped to blocking** on macOS + Linux; remaining 3 baseline failures either fixed, quarantined, or documented as known-modernization-casualty.
3. **Phase 999.14 triage complete:** open-issue + open-PR counts reduced to genuinely-actionable set; 5-PR legacy bundle resolved (merge or close per category); stale-doc audit closed.
4. **Phase 999.21 + 999.22 landed:** DockResultFile dead code removed; Tier-C warning census published (census-only per Open Q3 — no warning fixes in v1.6.2; (a)/(b)/(c) deferral stubs filed).
5. **Both installers attached to the GitHub Release** — Windows zip + macOS arm64 zip — and the GitHub Release is **published**, not draft. (Same criterion as v1.6.1 #6.) Per the v1.6.2 corrective shape, no signing/notarization gate (that's v1.7's Phase 8).

**Release notes (per Open Q4):** `.planning/RELEASE-NOTES-v1.6.2.md` is **drafted at v1.6.2 tag time**, not pre-populated during the cycle. Same shape as v1.6.1's release notes, written from the actual landed work rather than the plan.

## Carry-over from v1.6.1 STATE

The v1.6.1 milestone's accumulated context survives the milestone switch:

- **All v1.6 line modernization decisions** (Phases 1-5.1 + 4.1 + 999.2) are locked.
- **Phase 02.1 renderer boundary** + **Phase 5 SPIKE-02 decision** (GL-Core for v1.6.x → QRhi for v2) carry forward.
- **B3 baseline measurement** (Phase 5.1 carry-forward) was RESOLVED on CI run [25953405453](https://github.com/BALL-Project/ball/actions/runs/25953405453) (3495 → 0); no v1.6.2 follow-up needed.
- **glwidget-port-misprojection debug session** was RESOLVED 2026-05-16 (user-verified on Windows VM; file moved to `.planning/debug/resolved/`).
- **CR-01/CR-02/CR-03 Qt 6 regressions** were fixed inline in commits `fb0f7ed`..`1271a2a` (verified by ROADMAP-AUDIT-V1.6.2.md §A1 v3 against current source).

Nothing else from v1.6.1 carries an open-action label into v1.6.2.

## Roadmap delta from v1.6.1

When `/gsd-new-milestone v1.6.2` is run, the resulting changes are:

- **Promote 999.16, 999.17, 999.18, 999.19, 999.20, 999.21, 999.22** from BACKLOG to active.
- **Phase 6 stays in active-list but retargets v2.1** (per Open Q1 — PyBALL changes move to 2.x; Phase 6 becomes the first step of v2.1, before [999.15](ROADMAP.md) bulk wrap).
- **Promote Phase 9** from "Not started" to "In Progress" (Phase 9 detail section already records the partial-progress state).
- **Promote 999.14** from BACKLOG to active (with all 3 bundled subtasks; **triage runs first** per Open Q2, then stale-doc audit alongside, then 5-PR bundle as a clean tail).
- **999.22 scope locked to census-only** per Open Q3 — produces split-list + 3 deferral backlog stubs; no warning execution.
- **Promote 999.23** to active OR leave in backlog targeting v1.7 (depending on cycle capacity at promotion time).
- **STATE.md frontmatter:** `milestone: v1.6.1 → v1.6.2`; `status: planning`.

## Resolved questions (2026-05-16 user decisions)

1. **Phase 6 timing → MOVED OUT of v1.6.x to v2.1** (user direction: "PyBALL changes should move to 2.x"). Becomes the first step of the v2.1 PyBALL milestone, before [999.15](ROADMAP.md) bulk wrap. v1.6.2 effort drops to ~7-13 days as a result.
2. **Phase 999.14 promotion order → triage first, then 5-PR bundle.** Stale-doc audit runs alongside triage. 5-PR bundle is the clean tail.
3. **Phase 999.22 scope → census-only, no execution.** Produces the census doc + 3 deferral backlog stubs ((a) mechanical defer to v1.7/v2.0, (b) defer-to-v2.0-PIPE-01-dissolves, (c) defer-to-v1.7-needs-per-site-review). v1.6.2 ships with no actual warning reduction.
4. **v1.6.2 release notes → mirror v1.6.1 pattern but draft at tag time, not now.** No `.planning/RELEASE-NOTES-v1.6.2.md` file pre-populated during the cycle; written from landed work when v1.6.2 is ready to tag.

## Next action

Run `/gsd-new-milestone v1.6.2` once you're ready to commit. The workflow
picks this file up and short-circuits its requirements-gathering step.
The seven (+1 stretch) phases above are ready to promote with
`/gsd-review-backlog 999.NN` per phase as needed.
