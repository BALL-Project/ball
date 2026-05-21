# BALL 2.x Unified Roadmap -- RR1b Re-review

**Status:** COMPLETE  
**Verdict:** AGREE-LOCKABLE  
**Reviewer:** Codex CLI 0.128.0  
**Review id:** RR1b  
**Subject:** `.planning/v2.x/V2X-ROADMAP.md` after RR1 revision

## RR1 Required Changes

1. **ADDRESSED** -- §3 now marks `JSON load batching`, `AndNode tmp-bitmap reuse`, and `K0.6.4 v1->v2 JSON converter` as done/dropped; declines `Element-instance-id-table` per P4.3; and adds `V21-COMPOSITEASATOM-REMOVAL` in H4/H7. Doc: `V2X-ROADMAP.md` §3.
2. **ADDRESSED** -- §2 splits H6 into H6a and H6b; H6b depends on H5 final bond representation; H7 owns the VIEW redraw/stamp contract and feeds it before H6b schema freeze. §5 diagrams the same H5/H6a/H7 -> H6b edges. Doc: `V2X-ROADMAP.md` §§2,5.
3. **ADDRESSED** -- v2.4 is split into v2.4a installed-package contract before v2.4b physical split, with exported targets, `find_package(BALL)`, install-tree tests, ABI/version policy, compatibility matrix, release artifacts, and CI/versioning. Doc: `V2X-ROADMAP.md` §§4,5.
4. **ADDRESSED** -- pyBALL is explicitly marked REBASELINE required; the Phase-6 bake-off is called Composite-era and must be re-cut against the handle surface. SIP stays off through v2.2; wrappers return after handle API stabilization with `BALL_PYTHON_WRAPPER`. Doc: `V2X-ROADMAP.md` §§2,4,4a.
5. **ADDRESSED** -- the D31b/D66a gate is a v2.2 standing invariant enforced by a maintained script with comment/prose false-positive handling, not ad hoc grep, and is part of H2-H4/H8 acceptance. Doc: `V2X-ROADMAP.md` §§2,6.
6. **ADDRESSED** -- API-break ledger closure, migration notes, downstream migration guide, and H8 release/migration-guide closure are explicitly required. Doc: `V2X-ROADMAP.md` §§2,4a,5,6.
7. **ADDRESSED** -- the H2-H3 mutation/import perf budget is a standing invariant with PDB load / JSON / build benchmarks during dual existence. Doc: `V2X-ROADMAP.md` §§2,6.
8. **ADDRESSED** -- D13 acceptance now separates `sizeof(BALL::Atom) <= 32 B` and `sizeof(BALL::Bond) <= 32 B` on every supported compiler including MSVC from the total live store footprint `<= 160 B/atom`. Doc: `V2X-ROADMAP.md` §§2,5,6.
9. **ADDRESSED** -- §4a states the staged minor strategy: v2.2 handle flip, v2.3 String removal unless ready before v2.2-rc1, v2.4 package/split; it also states D53 keeps the work in the 2.x band. Doc: `V2X-ROADMAP.md` §§4,4a,6.
10. **ADDRESSED** -- the stale source-doc "v3.0" label is called out as historical and superseded by D53; the roadmap is authoritative that the milestone is v2.2. Doc: `V2X-ROADMAP.md` §6.

## New Findings

No new lock-blocking inconsistencies found.

- §2 phase table, §3 backlog table, §4/§4a version plan, and §5 critical-path diagram are internally aligned: H2->H3->H4 precedes H5/H6a/H7; H6b waits for H5 plus H7 stamp/schema input; H8 closes v2.2; v2.3/v2.4 follow.
- The H6a/H6b split is acceptable: H6a owns the property break plus container/property JSON work after H4, while H6b is the final schema freeze/converter after H5 and the H7 stamp decision. The schema freeze remains centralized in H6b.
- "MSVC bring-up may start after H2" is safe as written because it is scoped as parallel bring-up once the build shape is stable; H7 remains the acceptance gate and H8 remains the cross-compiler `sizeof`/D13 verification gate. No edit to "after H4" is required.
- The roadmap no longer conflates BALLView 1.6.x with the kernel 2.x track: §0/§1 frame BALLView 1.6.x as parallel and independent, with intersections only at VIEW-RTTI, MSVC CI, and repo split.

Minor non-blocking note: §4's lead sentence says the post-v2.2 milestones are sequenced after v2.2, while §4a permits `BALL::String` to fold into v2.2 only if it lands before rc1 with no schedule risk. §4a is clear enough as the explicit exception; no lock-blocking edit needed.

## Overall Verdict

**AGREE-LOCKABLE.** The RR1 required-change punch list is addressed, and the revised roadmap is sound enough to lock as the canonical BALL 2.x kernel plan. No remaining required edits.
