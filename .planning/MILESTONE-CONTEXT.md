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

## In scope (8 phases)

Drawn from the post-2026-05-16 ROADMAP-AUDIT-V1.6.2.md + roadmap
discussion outcome. Effort estimates per ROADMAP entries.

### Build-acceleration cluster (5 phases, ~3-5 days total)

Layered on top of [Phase 999.2 Ninja switch](ROADMAP.md) (already
landed in v1.6.1 — Windows Build 4818s → 55s warm, 87× speedup).
These five close the remaining marginal wins:

1. **[Phase 999.16](ROADMAP.md) — PCH for BALL + VIEW** — `target_precompile_headers` on the heavy Qt + Boost + Eigen + BALL/CONCEPT/COMMON header set. Targets 20-40% cold-cache speedup on template-heavy TUs. ~0.5-1 day.
2. **[Phase 999.17](ROADMAP.md) — Windows CMake build-tree cache** — `actions/cache` keyed on hash of CMakeLists/cmake/vcpkg.json/CMakePresets.json. Collapses warm-cache `Configure (Windows)` from ~2.5 min to seconds. ~0.5-1 day.
3. **[Phase 999.18](ROADMAP.md) — Path-aware CI triggers + concurrency groups** — skip CI runs on doc-only / planning-only changes. Concurrency groups cancel obsolete runs on rapid pushes. ~0.5 day.
4. **[Phase 999.19](ROADMAP.md) — Per-TU build profiling artifact** — ninja `.compile_commands.json` + timing breakdown uploaded as a CI artifact. Future-proof for identifying compile-time outliers. ~0.5 day.
5. **[Phase 999.20](ROADMAP.md) — Bump action artifact pins to v6/v7** — `upload-artifact@v4 → v6`, `download-artifact@v4 → v7`. Was Phase 5.1 D-deferred (cross-breaking-change majors). ~0.5 day.

### Source-level + cleanup (3 phases, ~5-9 days total)

6. **[Phase 6](ROADMAP.md) — Python Bindings (autowrap+Cython vs nanobind bake-off)** per [PYBALLV2.md](PYBALLV2.md) §6 — 7-case cross-platform vertical slice; tool decision is the deliverable. **7.5 weeks** if both tracks run sequentially; **~4 weeks** if 2 engineers parallelize the autowrap and nanobind tracks. Could slip to v1.6.3 if v1.6.2 fills up — the v1.6.x label is forgiving.
7. **[Phase 9](ROADMAP.md) — Test Suite Triage (close)** — CI wiring already landed in `b2bb718` + `61bf5a7`; baseline at 99.0% (291/294) on macOS with `BALL_DATA_PATH` set. Remaining: triage 3 failures (`Directory_test`, `AmberFF_test`, `AssignBondOrderProcessor_test2`) + flip CI gatekeeper from `continue-on-error: true` to blocking. ~2-3 days.
8. **[Phase 999.14](ROADMAP.md) — GitHub issue + PR triage + bundled tasks** — (a) 5-category triage of open issues + open PRs (1-3 days); (b) **stale-doc audit** scanning all `*VERIFICATION.md` files for the HUMAN-UAT-disagreement pattern that misled the 2026-05 ROADMAP-AUDIT (0.5 day); (c) **5-PR legacy bundle** (#640 FindXDR, #600 Travis-CI, #554 Omega torsion, #550 hydroxyproline, #546 residue insertion code) — categorize and merge/close per audit-decided plans (2-5 days depending on rebase complexity). Total 3-9 days.

### Tiny dead-code + grammar cleanups (2 phases, ~1.5-3 days total)

9. **[Phase 999.21](ROADMAP.md) — DockResultFile QtXml dead-code cleanup** — drop the dead `QXmlAttributes` overload that was stubbed under `#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)`. Was Phase 5.1 BLOCKER-A2, never picked up. ~1 hour.
10. **[Phase 999.22](ROADMAP.md) — Warning census + (a)-subset cleanup** — census of deferred Tier-C warnings (~3700) + residual Windows C4910/C4834. **NOT a wholesale cleanup**; produces a v1.7 split-list + executes only the (a) mechanical-fix-now subset. ~3-5 days. If (a) is empty, phase closes with just the census doc + split-stub backlog entries.

### Stretch (defer to v1.7 if v1.6.2 fills up)

11. **[Phase 999.23](ROADMAP.md) — CIF Bison grammar audit** — 3-5 shift-reduce conflicts in the CIF parser (count needs reconciliation). 1-2 days. Defer to v1.7 if v1.6.2 budget tight.

**Total v1.6.2 effort estimate:** 11-21 days if Phase 6 runs sequentially in-cycle; substantially less if Phase 6 slips to v1.6.3 (Phase 6 dominates the budget at 7.5 weeks single-engineer / ~4 weeks dual-engineer).

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

A v1.6.2 release is shippable when **all six** are true:

1. **All 5 build-acceleration phases (999.16-999.20) landed** AND CI green on a clean tri-OS run.
2. **Phase 9 close: CI gatekeeper flipped to blocking** on macOS + Linux; remaining 3 baseline failures either fixed, quarantined, or documented as known-modernization-casualty.
3. **Phase 999.14 triage complete:** open-issue + open-PR counts reduced to genuinely-actionable set; 5-PR legacy bundle resolved (merge or close per category); stale-doc audit closed.
4. **Phase 999.21 + 999.22 landed:** DockResultFile dead code removed; Tier-C warning census published + (a) subset cleaned.
5. **Phase 6 bake-off decision recorded** OR explicitly deferred to v1.6.3 (the tool-choice doc is the deliverable; not all bulk wrap needs to land).
6. **Both installers attached to the GitHub Release** — Windows zip + macOS arm64 zip — and the GitHub Release is **published**, not draft. (Same criterion as v1.6.1 #6.) Per the v1.6.2 corrective shape, no signing/notarization gate (that's v1.7's Phase 8).

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
- **Promote Phase 6** from active-not-started to active-in-progress (with the bake-off scope per PYBALLV2.md §6).
- **Promote Phase 9** from "Not started" to "In Progress" (Phase 9 detail section already records the partial-progress state).
- **Promote 999.14** from BACKLOG to active (with all 3 bundled subtasks).
- **Promote 999.23** to active OR leave in backlog targeting v1.7 (depending on cycle capacity at promotion time).
- **STATE.md frontmatter:** `milestone: v1.6.1 → v1.6.2`; `status: planning`.

## Open questions

1. **Phase 6 timing in v1.6.2 — sequential or parallel tracks?** PYBALLV2.md §6 estimates 7.5 weeks sequential. If you want v1.6.2 to ship in a reasonable window, either (a) 2 engineers parallelize autowrap and nanobind tracks (~4 weeks), or (b) Phase 6 slips to v1.6.3 and v1.6.2 ships without the bake-off result. Recommend (a) if a second engineer is available; otherwise (b).
2. **Phase 999.14 promotion order — triage first, or 5-PR bundle first?** Triage benefits from a stable HEAD; 5-PR bundle is independent rebase work. Recommend triage first (fast, clears noise), then 5-PR bundle as a follow-on.
3. **Phase 999.22 warning census — wholesale audit or census-only?** Per audit §A1, the explicit scope is census + (a)-subset only (NOT wholesale ~3700 cleanup). Confirm that's still the call before promotion.
4. **v1.6.2 release notes shape** — does v1.6.2 get its own RELEASE-NOTES-v1.6.2.md (mirror the v1.6.1 pattern), or fold into the existing release-notes infrastructure?

## Next action

Run `/gsd-new-milestone v1.6.2` once you're ready to commit. The workflow
picks this file up and short-circuits its requirements-gathering step.
The eight (+1 stretch) phases above are ready to promote with
`/gsd-review-backlog 999.NN` per phase as needed.
