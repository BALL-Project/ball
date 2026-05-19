# Track A v2.1 Cluster R17c — Codex CLI Round 17c (2026-05-19)

**Status:** Complete
**Verdict:** GO
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.1 P0 design lock — second revision D22b/D23b/D24b/D31b/D32b/D33b/D34b
**Predecessors:** R17 NO-GO, R17b NO-GO

## R17b finding closure status

| R17b finding | Severity | Second-revision fix | Status |
|---|---|---|---|
| P17b-1 memory math | BUG | D22b corrects the comparison to whole Atom handle shrink and states ~28.0 MB net savings per 100k atoms. | CLOSED |
| P17b-2 sparse/name growth | BUG | D23b adds a default 65,536 dynamic-property-name cap and rejects new names after the cap. | CLOSED for the R17b unbounded-name attack; see P17c-3 note on entry count. |
| P17b-3 promotion hysteresis | DEBT | D23b documents promote-once, never-demote. | CLOSED |
| P17b-N1 mismatched-type replacement semantics | BLOCKER | D23b changes lookup to sparse-first and clears sparse overrides on matching dense writes. | CLOSED |
| P17b-N2 `vector<atomic>` resize/compact invalid | BLOCKER | D24b replaces it with `unique_ptr<atomic<uint64_t>[]>` rebuilt under exclusive store access. | CLOSED |
| P17b-N3 sparse bag unbounded | BUG | D23b caps dynamic property names at 65,536 per store. | CLOSED for distinct-name growth; see P17c-3 note. |
| P17b-N4 P5 review downgrade | BUG | D33b restores P5 planning review. | CLOSED |
| P17b-N5 D31 unenforced iterator invariant | DEBT | D31b moves `CompositeNode` to a private internal header and adds a grep gate against public/iterator leakage. | CLOSED |
| P17b-8 backport policy incomplete | DEBT | D32b adds the v2.0.x-only regression bucket. | CLOSED |
| P17b-10 MSVC CI timing | DEBT | D34b moves the gate to P2 close and starts the Windows job at P1 close. | CLOSED |

## Probe answers

| # | Probe | Verdict | Notes |
|---|---|---|---|
| P17c-1 | D22b memory accounting | OK | The accounting is directionally correct: 48 B `composite_nodes_` + ~20 B property columns + 0.125 B selection bits + <=32 B thin Atom = ~100.125 B/atom. The text says `<=100 B/atom`; that should be read as rounded shorthand, or changed to `~100 B/atom`, but it remains far under D13's 160 B/atom budget. Bond side state remains deferred/out of scope. Per-store registry/vector overhead is amortized at 100k atoms and does not threaten the budget. |
| P17c-2 | Sparse-first replacement sequence | OK | `setProperty(foo, INT, 5)` writes dense if `foo` has an INT column. `setProperty(foo, STRING, "bar")` writes a sparse override for atom 0. `getProperty(foo, atom0)` checks sparse first and returns `"bar"`. Atom 1 has no sparse override from atom 0's write, so it still reads its own sparse or dense value. |
| P17c-3 | 65k dynamic-name cap scope | OK with documentation note | Atom name/type strings use the existing atom string pool and are not dynamic property names; D23b does not cap them. Sparse entry count is not independently capped: it is bounded by explicit writes over `(live atoms x capped dynamic names)`, not by a byte/entry budget. That closes the R17b one-atom/1M-name attack, but the phrase "bounded sparse bag" is imprecise. Suggested wording: "bounded dynamic property names; sparse entries remain proportional to explicit property assignments." |
| P17c-4 | Promote-once memory waste | OK | The doc is honest enough: promoted columns stay dense for the store lifetime. A cleared INT/FLOAT column costs about 100k x 4 B = 400 KB plus presence bits. N oscillating properties cost roughly N x 400 KB. This is an accepted anti-flapping trade-off, not a hidden blocker. |
| P17c-5 | `unique_ptr<atomic[]>` resize | OK | Relaxed load/store copying is correct when the caller has exclusive store access. Existing `compact()` already requires exclusive access and is not safe against concurrent readers/evaluate calls; D24b aligns with that contract. There should be no concurrent readers during resize/compact. |
| P17c-6 | D31b encapsulation boundary | OK with clarification | Including `_moleculeStoreInternal.h` from `composite.C` does not expose `CompositeNode*` to public headers. "P1.3 wiring code" should be named as concrete `.C` implementation files in the P1 plan. The important boundary is preserved: no `CompositeNode*` or `composite_nodes_` in `composite.h`, `composite.iC`, or iterator headers. |
| P17c-7 | D32b regression bucket | OK | The added v2.0.x-only regression bucket is clear. A "v2.1-only feature accidentally backported to v2.0" is a branch-management error, not a missing bug bucket; v2.1-only features are not supposed to exist on v2.0.x by definition. |
| P17c-8 | D33b cadence/R-numbering | DOC FIX | The arithmetic is wrong. Actual review invocations are P0 cluster 3 (R17/R17b/R17c) + P1-P5 10 + P6 close 1 = 14. Numeric ID slots are R17 plus R18-R28 = 12, with R17b/R17c as suffixed repeats. The doc's "Total Codex rounds: 12" is only true if counting numeric slots, not actual review rounds. |
| P17c-9 | D34b MSVC CI P1 scope | OK with clarification | Adding Windows CI at P1 close is acceptable if it runs the normal BALL/core build and P1 side-table parity tests on MSVC. The `sizeof(Atom) <= 32 B` pins must become required at P2 close, not during P1 while Atom is still v2.0-compatible. D34b implies this; the P1 plan should spell out the exact Windows job matrix. |
| P17c-10 | Cross-revision interactions | OK | D23b sparse-first lookup is compatible with D30a v2.0 JSON demux: demuxed sparse entries correctly override dense rows for affected atoms. D24b rebuild-under-exclusive-lock does not need a separate generation bump for selection bits because no borrowed selection pointer is exposed; `compact()` already bumps generation. If future code exposes selected word pointers, that must change. `CompositeTopologyView` by-value is fine as an internal/private implementation view; do not make it a supported public API type. |

## New findings (if any)

No new BLOCKERs or BUGs were found in the second revision.

Non-blocking documentation corrections before or during P1 planning:

1. Fix D33b's review-count wording: distinguish 14 actual review invocations from 12 numeric ID slots.
2. Tighten D23b wording from "bounded sparse bag" to "bounded dynamic property names"; sparse entries remain proportional to explicit assignments and are not separately byte-capped.
3. In the P1 plan, name the concrete files covered by "P1.3 wiring code" and define the Windows CI job as BALL/core build + P1 parity tests, with v2.1 size pins becoming mandatory at P2 close.

## Overall verdict

**GO for P1.** The second revision closes the R17b P1 blockers: mismatched-type property replacement now preserves v2.0 semantics, selected-bit storage is implementable under the store lifecycle, sparse distinct-name growth is capped, P5 planning review is restored, and CompositeNode leakage has a concrete enforcement boundary.

P17-6 bond identity remains deferred and out of scope. Start P1 with the three documentation cleanups above folded into the P1 planning artifact.
