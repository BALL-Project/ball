# Track A v2.1 Cluster R21c — Codex CLI Round 21c (2026-05-19)

**Status:** Complete
**Verdict:** NEEDS-FIXES
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.1 roadmap, third pass

## R21b finding closure status

| R21b finding | Expected fix | Verification | Status |
|---|---|---|---|
| P21b-2 P3 classification artifact | Checked-in `.planning/v2.1/P3-CLASSIFICATION.md` | `find . -name P3-CLASSIFICATION.md` returns nothing; `git show --name-only 7f8a469f0` does not include it. | **OPEN** |
| P21b-4 P5.2 baseline pinning | One-time collected + pinned `.planning/v2.1/PERF-BASELINES.json` or equivalent | Roadmap text now says one-time collected and pinned, but no baseline file exists in tree or in commit `7f8a469f0`. | **PARTIAL** |
| P21b-5 P5.4 verification | Concrete debug test + release proof path | Roadmap now has three levels: `ctest -R "GenerationGuard"`, `nm -gU` grep, and manual `objdump`. The grep level is not meaningful as written; see P21c-5. | **PARTIAL** |
| P21b-7 MSVC backlog | Real backlog artifact | `.planning/v2.1/BACKLOG.md` exists and tracks `V21-MSVC-CI-PORTABILITY`. | **CLOSED** |
| P21b-8 STORE-ITER-API memo | Checked-in `.planning/v2.1/P5-STORE-ITER-API.md` | `find . -name P5-STORE-ITER-API.md` returns nothing; `git show --name-only 7f8a469f0` does not include it. | **OPEN** |
| P21b-10 round count cleanup | Clear remaining-round wording | Roadmap now says R22-R27 remain after R21b, 6 rounds. | **CLOSED** |

The main problem is that commit `7f8a469f0` claims to add the P3 classification artifact, perf baseline pin, and STORE-ITER-API memo, but the tree only contains `BACKLOG.md`, `V21-CODEX-REVIEW-ROUND21B.md`, and edits to `V21-ROADMAP.md`.

## Probe answers

**P21c-1. R21b finding closure**

Not closed. Three claimed checked-in artifacts are absent:

- `.planning/v2.1/P3-CLASSIFICATION.md`
- `.planning/v2.1/PERF-BASELINES.json`
- `.planning/v2.1/P5-STORE-ITER-API.md`

This is a must-fix before P3 kickoff for `P3-CLASSIFICATION.md`. The P5 files can technically be produced during P5, but the roadmap currently represents them as checked-in/available design artifacts, so either add them now or change the wording to "will be produced in P5.1/P5.2."

**P21c-2. Is the roadmap executable as written?**

Mostly, but not yet as represented. A fresh maintainer can start P3.1 from `V21-ROADMAP.md` + `V21-DECISIONS.md`: the categories, anti-pattern, grep gate, and close-review expectations are clear.

However, they cannot "pick up" the P3 classification from the promised artifact because it does not exist. If the intent is that P3.1 is still future work, the roadmap should say P3.1 will create `P3-CLASSIFICATION.md`. If the intent is that R21b already created it, add the file before GO.

**P21c-3. Remaining contradictions or stale references**

There are no new architectural contradictions in the revised roadmap itself: D39/D40/D34d framing is coherent, P6 branch strategy is clean, and R26 planning downgrade is reconciled by the STORE-ITER memo plus R26 close review.

Residual stale context remains in older planning docs:

- `MILESTONE-v2.1-KICKOFF.md` still describes P5 as including "Final D13 verification" and the original thin-handle/sizeof posture.
- `P1-PLAN.md` still discusses MSVC verification in the old P1/D34 frame.
- Earlier parts of `V21-DECISIONS.md` preserve superseded D28/D33/D34/D34c text before D34d/D39-D41 revise it.

Those are acceptable as historical docs if the roadmap is treated as the active source of truth, but the missing artifacts are not acceptable.

**P21c-4. BACKLOG.md sufficiency**

`BACKLOG.md` is sufficient for `V21-MSVC-CI-PORTABILITY`. It has an ID, status, source, rationale, v2.1/v2.2 blocking relationship, target milestone, and implementation scope. It does not need more process structure before P3.

**P21c-5. P5.4 `nm -gU ... | grep generation_` check**

As written, this is weak and likely produces no useful signal. `nm -gU build/lib/libBALL.dylib` lists external/global symbols. A private member name or local inline dereference of `store->generation_` normally will not appear as a `generation_` symbol in a release dylib, especially after optimization and stripping. So "grep does not find generation_" is mostly a false-negative-prone check, not proof that release getters avoided the generation compare.

Keep the debug stale-handle test. For release zero-cost proof, rely on targeted disassembly or compile an explicit no-debug object/assembly check for representative getters. This can be fixed before P5.4; it is not a blocker for starting P3.

**P21c-6. Overall**

NEEDS-FIXES. The roadmap direction has converged, but the R21b closure claim is materially false in the current tree.

## Overall verdict

**NEEDS-FIXES.**

Must-fix before P3 kickoff:

1. Add `.planning/v2.1/P3-CLASSIFICATION.md` now, or revise the roadmap to state that P3.1 creates it as the first P3 deliverable.

Can-fix during execution, but should be corrected before the relevant phase:

1. Add or defer-word `.planning/v2.1/PERF-BASELINES.json`.
2. Add `.planning/v2.1/P5-STORE-ITER-API.md` before P5.1/R26, or restore an explicit API planning gate.
3. Replace the P5.4 `nm -gU ... | grep generation_` CI claim with a meaningful release-build verification method, or mark that level as manual close-review evidence.

Once the missing P3 artifact issue is resolved, P3 can kick off. The remaining P5 verification/API issues do not need to block P3.
