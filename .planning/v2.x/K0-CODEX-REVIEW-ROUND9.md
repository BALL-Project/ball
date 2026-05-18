# K0.7 Adversarial Review — Codex CLI Round 9 (2026-05-18) — perf gate

**Status:** Complete. Verdict: **go for K0.8** with one required
documentation fix first (D13 wording must not claim it's met).

**Reviewer:** Codex CLI 0.128.0 (`codex exec`).
**Subject:** K0.7.0 - K0.7.4 (commits `486573a40..7fa7c32d2`).

---

## Verdict (Codex, verbatim)

> Go for K0.8, with one required documentation fix first: do not
> describe D13 as met. Say explicitly that v2.0 meets the SoA column
> budget but misses the full live-handle D13 budget by 3.1x.
>
> I would not spend RC time partially shrinking Atom unless you have
> a low-risk patch already sitting there. The 360 B handle is
> architectural debt from the D2/D3/D4 compromise, not a perf surprise
> that threatens the demonstrated v2.0 fast path.

## Probe answers (summary)

| # | Probe | Verdict |
|---|---|---|
| 1 | D13 acceptable for RC? | **Acceptable IF documented as a miss, not a pass.** Don't partial-reduce now — touches semantics. |
| 2 | K0.7.5 deferral OK? | **Agree.** Keep on v2.1 perf-debt list. |
| 3 | JSON load 15× slower than save | **Acceptable.** Batching is the right opt but invasive — defer to v2.1, mention in release notes. |
| 4 | Soft gates 30s/60s too loose? | **Too loose as regressions, fine as tripwires.** Tighten in K0.8 with CI-calibrated values. |
| 5 | Per-atom property emit cost | **Acceptable.** +142ms / +175 B/atom for 3 props. |
| 6 | Single-run measurements | **Label all numbers "single run, Darwin arm64 release"; use median-of-N for any release claim.** |
| 7 | No-molecule memory benchmark caveat | **Conclusion right; rename "measures" → "models", omit allocator overhead disclosure.** |
| 8 | Ready for K0.8? | **Yes, after correcting D13 wording.** |

## Adversarial findings (all documentation / framing)

1. MemoryBudget_test "measures" but actually MODELS — relabel.
2. D13 was "typical ≤160 B/atom"; full-fat workload misses by 3.1×. Don't soften into "SoA passes" without caveat.
3. `sizeof(Atom) <= 512` is a weak pin — prevents disaster, not drift.
4. JSON gates (30s/60s) miss 2-10× regressions.
5. Single-run numbers shouldn't become marketing/release claims without median labels.
6. JSON load batching is real but too invasive for RC.
7. `countAtoms()` over orphan atoms is not a strong D14 proxy for molecule traversal.
8. Memory budget excludes allocator overhead — 499 B/atom is optimistic.
9. K0.7-SUBPHASES.md's K0.7.3 row says "D13 met" — fix this planning line.
10. Release notes must distinguish "store-native / handle-sparse" from "all atoms materialized as Atom*".

---

## Remediation (K0.7.7 doc fixes + plan correction)

| Item | Fix | Effort |
|---|---|---|
| Finding 1+9 | Rename `MemoryBudget_test` "measures" comments to "models"; K0.7-SUBPHASES.md K0.7.3 row updated to "D13 partially met (SoA only)" | 10 min |
| Finding 2 | KERNEL-V2-DECISIONS.md note the D13 split (column vs full-handle) explicitly | 10 min |
| Finding 3,4,5 | All bundled into K0.8 release-notes drafting (regression-pin tightening + median-of-N + D2/D3/D4 v2.1 deferral list) | K0.8 |

---

## What ships in v2.0 (per R9)

- SoA-only data path: D13 met (139 B/atom).
- Full live-handle workload: D13 missed by 3.1× (499 B/atom).
- Selector speedup: 61.8× median, well over the ≥10× design gate.
- JSON I/O: save 232ms / load 3.8s for 100k atoms. Acceptable but
  documented.

## What slips to v2.1

- D2/D3/D4 base-class actual removal (Composite, PropertyManager,
  Selectable) → would reduce sizeof(Atom) from 360 B to ~64 B.
- JSON load batching: pre-allocate slots + bulk-fill instead of
  per-atom new Atom + adopt — would close most of the 15× save/load
  ratio.
- AndNode tmp-bitmap reuse via arena/scratch buffer.
- CI-calibrated tight perf gates.
