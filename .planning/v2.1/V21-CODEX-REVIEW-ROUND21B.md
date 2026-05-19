# Track A v2.1 Cluster R21b — Codex CLI Round 21b (2026-05-19)

**Status:** Complete
**Verdict:** NEEDS-FIXES
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.1 roadmap, second pass

## R21 finding closure status

| R21 finding | Resolution | Status |
|---|---|---|
| P21-2 D40 framing | D40 honest framing section added: JSON + API hygiene + v2.2 prep, not D13/thin-handle closure. P6.1 release notes also repeats this. | CLOSED |
| P21-4 P3 `isAtom_()` | D41.1 rejects `Composite::isAtom_()`; P3 now requires per-site classification and a CI grep gate. | CLOSED WITH FOLLOW-UP |
| P21-5 / R21-F1 P4 load batch | D41.2 + P4.0 profile sub-step added before P4.1 design lock. | CLOSED |
| P21-8 P5 perf gates | D41.3 adds `max(2 x median, median + 6 x MAD)` over 5 baseline runs. | CLOSED WITH FOLLOW-UP |
| P21-9 P5 generation guard | P5.4 now requires `#ifdef BALL_DEBUG`, release disassembly or `nm`/`strings` proof, and debug stale-handle test. | PARTIAL |
| P21-10 / R21-F4 P6 branch | P6.6 now states tag from `v2.1`, merge `v2.1` to `master`, do not merge to `v2.0`. | CLOSED |
| P21-11 / R21-F3 D34c MSVC | D34d moves MSVC to v2.2 and makes v2.1 Windows portability non-blocking. | PARTIAL |
| P21-12 R26 cadence | R26 P5 planning downgraded to checklist; total cadence shortened. | PARTIAL |

## Probe answers

**P21b-1. D40 framing**

The RELEASE-NOTES guidance is now honest. P6.1 explicitly says the release notes must use "JSON improvements + API + v2.2 prep" framing and include a "Not in v2.1" section for sizeof, wiring, and D13.

I do not think P6.1 needs to mandate that exact framing in the commit message. The enforceable artifact is `RELEASE-NOTES-v2.1.md`, and R27 pre-tag review can reject the release if the notes drift back to thin-handle/D13 language.

**P21b-2. D41.1 per-site classification**

The categories are the right categories:

- (T) typed-call-path-available
- (S) store-backed-identity
- (V) visitor-needed

Together they cover the 24 sites without requiring a default replacement. If a site cannot be made typed and cannot prove identity through store/back-ptr, it falls into (V) and must justify a visitor or equivalent runtime dispatch.

The deliverable form is too weak. "Per-site spreadsheet in P3.1 commit message" is easy to lose and hard to review in R22/R23. Make it a checked-in artifact, preferably `.planning/v2.1/P3-CLASSIFICATION.md`, with all 24 sites listed, category, replacement plan, and test/grep coverage. The commit message can summarize it, but should not be the source of record.

**P21b-3. P4.0 profile sub-step**

Yes, `P4-PROFILE.md` should be committed before P4.1 designs or implements the batch path. The roadmap already says "before P4.1 design lock," which is the right gate.

The gate should be explicit: P4.1 cannot start until `P4-PROFILE.md` records bucket timings for the representative 100k-atom load, identifies the dominant bucket, and states the chosen optimization target plus the resulting numeric performance claim.

**P21b-4. D41.3 calibration cost**

The math is real: 5 baseline workflow runs with N=5 internal iterations is 25 bench invocations per metric. With a ~30s JsonBench, one metric costs roughly 12-15 minutes of baseline collection on GHA macos-15.

That is acceptable as a one-time calibration PR, not as routine CI. The roadmap should say baselines are collected once on clean `v2.1` baseline, pinned in a checked-in baseline file, and only refreshed intentionally after benchmark or runner drift review. Otherwise P5.2 risks making normal CI too expensive and flaky.

**P21b-5. P5.4 generation-guard verification**

P5.4 now asks for release disassembly or `nm`/`strings` proof, but it does not specify an enforceable CI command. As written, this is still manual close-review evidence, not a CI gate.

Fix: add a concrete verification target or script, for example `ctest -R GenerationGuardDebug` for stale-handle behavior plus a release-build `objdump`/`nm` check target that fails if inline getter objects contain a `generation_` reference or compare. If fully reliable disassembly grep is too toolchain-specific, the roadmap should explicitly classify the proof as manual release-review evidence instead of calling it CI enforcement.

**P21b-6. P6.6 branch strategy**

No conflict. Because `v2.1` forked from `v2.0.0`, merging `v2.1` to `master` after the final tag intentionally lands v2.0 plus v2.1 onto master in one step. That matches D29.

The wording is now sufficient. It also correctly preserves `v2.0` as a maintenance branch and avoids merging feature work back into `v2.0`.

**P21b-7. D34d MSVC backlog**

`V21-MSVC-CI-PORTABILITY` is not actually tracked anywhere I could find beyond the sentence in D34d. `rg` finds the new name only in `V21-DECISIONS.md`; there is no backlog file or issue-style artifact.

So "filed" is currently wishful. Either create a tracked backlog artifact, or change the wording to "to be filed before v2.1.0 final." Also fix stale roadmap text that still says D34c may slip from rc1 to v2.2.

**P21b-8. R26 downgrade / STORE-ITER-API**

A commit-message-only design memo is not enough for new public API. `MoleculeStore::iterAtoms()` / `iterBonds()` is small, but it still creates a consumer-facing contract that must survive v2.2.

Minimum fix: make P5.1 produce a checked-in `P5-STORE-ITER-API.md` or restore a short R26 API-only planning review. The current roadmap is internally inconsistent: it says R26 planning is downgraded, but risk R-V21.D still says "R26 planning review explicitly evaluates" v2.2 compatibility.

**P21b-9. New issues introduced by revisions**

1. Stale MSVC text remains in `V21-ROADMAP.md`: the "What v2.1 does NOT ship" section still says "D34c -> moved to v2.1.0-rc1 prep, may slip to v2.2," and risk R-V21.E repeats the rc1-slip framing. This now contradicts D34d.

2. R-V21.D still references an R26 planning review even though R26 planning was downgraded. Either restore the API-specific planning gate or update the risk mitigation.

3. The review-count wording is muddy: "Total revised: 6 more Codex rounds" lists R21 and R21b along with R22-R27. After R21b, the remaining rounds are R22-R27. If the intended total includes R21/R21b, the count is not six.

4. P3.1 and P5.1 currently rely on commit-message design artifacts. That is too ephemeral for roadmap gates that later reviewers must audit.

**P21b-10. Overall**

NEEDS-FIXES. The R21 direction is mostly corrected, and P3 can start after a narrow cleanup pass. The remaining items are documentation/process fixes, not architecture blockers.

## Overall verdict

**NEEDS-FIXES.** Required fixes before GO:

1. Replace P3.1's commit-message spreadsheet with a checked-in `P3-CLASSIFICATION.md` or equivalent reviewed artifact covering all 24 sites.
2. Clarify P5.2 baselines as one-time collected and pinned, not re-collected on normal CI.
3. Add a concrete P5.4 verification command/target, or explicitly mark release disassembly proof as manual close-review evidence.
4. Track `V21-MSVC-CI-PORTABILITY` in a real backlog artifact or stop saying it is filed; remove stale D34c/rc1 MSVC text from the roadmap.
5. Give STORE-ITER-API a checked-in design memo or restore a short API-only planning review; fix the R-V21.D / R26 contradiction.
6. Clean up the Codex round-count wording.
