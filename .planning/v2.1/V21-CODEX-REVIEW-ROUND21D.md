# Track A v2.1 Cluster R21d — Codex CLI Round 21d (2026-05-19)

**Status:** Complete
**Verdict:** GO
**Reviewer:** Codex CLI Round 21d
**Subject:** v2.1 roadmap, final pass after R21c fixes

## R21c finding closure status

| R21c item | Expected fix | Verification | Status |
|---|---|---|---|
| P3.1 false artifact claim | Roadmap must say P3.1 creates `.planning/v2.1/P3-CLASSIFICATION.md` as the first P3 deliverable, not that the file already exists. | `V21-ROADMAP.md` P3.1 now says "**P3.1 creates** `.planning/v2.1/P3-CLASSIFICATION.md` as its first deliverable." | **CLOSED** |
| P5.1 false artifact claim | Roadmap must say P5.1 creates `.planning/v2.1/P5-STORE-ITER-API.md` as the first P5.1 deliverable. | P5.1 now says "**P5.1 creates** `.planning/v2.1/P5-STORE-ITER-API.md` as its first deliverable." | **CLOSED** |
| P5.2 false artifact claim | Roadmap must say P5.2 creates `.planning/v2.1/PERF-BASELINES.json` during calibration, not that it is pre-existing. | P5.2 now says "**P5.2 creates** `.planning/v2.1/PERF-BASELINES.json` as the pinned baseline file." | **CLOSED** |
| P5.4 weak `nm` proof | Remove the false-negative-prone `nm -gU ... | grep generation_` release proof, or replace it with meaningful evidence. | P5.4 now has two verification levels: CI-enforced `ctest -R "GenerationGuard"` for the debug trap, plus R26 manual close-review of release-build `objdump -d` for representative inline getters. The roadmap explicitly rejects the `nm` grep. | **CLOSED** |

## Probe answers

**P21d-1. Does the "P3.1 creates X.md as first deliverable" wording adequately resolve R21c's false-claim concern?**

Yes. The roadmap no longer implies `.planning/v2.1/P3-CLASSIFICATION.md` already exists. It now defines it as the first P3.1 output, with the right contents: all 24 sites, category, replacement plan, and test/grep coverage. That is executable for P3 kickoff.

The same fix pattern is applied to the P5.1 design memo and P5.2 perf baseline pin. Those files are no longer falsely represented as current tree artifacts.

**P21d-2. Is the P5.4 two-level verification adequate, or does the manual-only release proof create release-quality risk?**

Adequate for v2.1. The debug behavior is CI-enforced by a named `GenerationGuard_test` under `ctest -R "GenerationGuard"`, which covers the correctness property that stale handles trap in `BALL_DEBUG`.

The release-build zero-cost property is not CI-enforced, but manual `objdump` inspection during R26 is a meaningful proof path for the representative hot getters named in the roadmap. This is acceptable because the zero-cost release check is about absence of debug instrumentation in optimized getters, not a runtime user-visible behavior. The roadmap also correctly avoids the earlier `nm` grep, which would have created false confidence.

No must-fix-now item remains here. A future automated release assembly check would be useful, but the roadmap already scopes that as a v2.1.x backlog possibility if a sound check is found.

**P21d-3. Any remaining contradictions in the roadmap as-of-now?**

No must-fix contradictions remain.

The active execution model is coherent:

- v2.1 is framed as JSON improvements + API hygiene + v2.2 preparation, not thin-handle/D13 closure.
- Mutation wiring, inheritance flip, `sizeof(Atom) <= 32 B`, and D13 budget closure are consistently deferred to v2.2.
- P3 avoids the rejected `Composite::isAtom_()` universal replacement and requires per-site classification first.
- P4 is profile-driven before load-batch design.
- P5 creates its planning artifacts during the relevant sub-phases and reviews the public API at P5 close.
- P6 branch strategy keeps `v2.0` as maintenance and merges `v2.1` to `master`.

There is minor stale process wording in the review-cadence section that still says "R21b" in a few places even though this is R21d. That is housekeeping, not an execution contradiction: the remaining phase gates are still R22-R27, and P3 can start after this GO.

**P21d-4. Overall**

GO.

## Overall verdict

**GO.**

No must-fix-now items remain before P3 kickoff.
