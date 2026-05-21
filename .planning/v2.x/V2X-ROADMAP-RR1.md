# BALL 2.x Unified Roadmap — RR1 Adversarial Review

**Status:** COMPLETE  
**Verdict:** NEEDS-REVISION  
**Reviewer:** Codex CLI 0.128.0  
**Review id:** RR1  
**Subject:** `.planning/v2.x/V2X-ROADMAP.md` as canonical BALL 2.x kernel roadmap

## RR1-1. Integration

The draft mostly captures the intended spine: v2.2 H0-H8, D53 "stay v2.2",
D55 A2 flat hierarchy, D60 dual existence, D62 JSON `containers`, D63 H4
canonical-name migration, the two maintainer directives, and the pyBALL v2
existence are all present (`V2X-ROADMAP.md` §§2,4; `V22-H0-ADDENDUM.md`
§§D55-D62; `V22-H1b-DESIGN.md` §D63).

It is not a clean absorption yet:

- **Dropped / under-specified:** SIP/Python-off policy is not promoted into
  a v2.2 invariant. Source docs say Python/SIP stays off for the handle
  phases and CI proves it is not built; wrappers return only with pyBALL v2
  against handles (`V22-ARCH-HANDLE-MODEL.md` §7; `V22-DECISIONS.md`
  §D52.3-7; §D54). The draft only says pyBALL is "after H4".
- **Dropped:** D31b internal-header gate maintenance is missing. H1b D66a
  explicitly protects `containerHandle.h` from `_moleculeStoreInternal.h`;
  D31b originally created the public/iterator-header grep gate
  (`V22-H1b-DESIGN.md` §D66a; `V21-DECISIONS.md` §D31b). This matters during
  H2-H4 cleanup because the old `composite_nodes_` path is left inert, not
  immediately deleted (`V22-H2-DESIGN.md` §D67).
- **Mis-attributed:** `AndNode tmp-bitmap reuse` is listed as a v2.2.x perf
  backlog item, but it was delivered in v2.0 (`RELEASE-NOTES-v2.0.md`
  "Deferred / follow-up" list).
- **Mis-attributed:** `K0.6.4 v1->v2 JSON converter` is listed under H6, but
  v2.0 notes say it was delivered in Track B B0.2
  (`RELEASE-NOTES-v2.0.md` "Deferred / follow-up" list).
- **Misplaced / stale:** `Element-instance-id-table` is placed in H6 or
  v2.2.x as JSON work. P4.3 found the issue is not JSON; it is an intentional
  store atomic-number-only model, and recommended dropping it unless the
  kernel consciously adds an element registry (`P4.3-FINDINGS.md`
  "Recommendation").
- **Double-counted:** `JSON load batching` is already the headline v2.1
  shipped item, 7-14x faster, with save/load now near parity
  (`RELEASE-NOTES-v2.1.md` "JSON load is 7-14x faster"). It should not sit
  in the v2.2 backlog as "close save/load ~15x gap".
- **Missing:** `V21-COMPOSITEASATOM-REMOVAL` was identified in P3 as a
  v2.2-removable name, but it is not placed in the unified roadmap
  (`P3-CLASSIFICATION.md` risk R-P3.A). It belongs in H4/H7 with the
  helper/traversal replacement.
- **pyBALL mismatch:** the draft re-anchors pyBALL after H4, which is
  directionally right, but `PYBALLV2.md` still targets old v2.1/Composite
  slice cases (`Atom (Composite+iterators+ownership)`, `HBondProcessor
  Composite/apply`). The roadmap must say the pyBALL plan is to be
  **rebaselined**, not merely moved (`PYBALLV2.md` §§1,6,7).

## RR1-2. Critical Path / Sequencing

The broad chain H2 -> H3 -> H4 -> H8 is right (`V22-H0-ADDENDUM.md` §D60;
`V22-H2-DESIGN.md` §§D71-D72). H4 must not happen until H3 has no consumer
requiring the v0 object tree, and H8 is the release verification gate.

The draft's post-H4 parallelism is too optimistic:

- **H5 and H6 are not independent as drawn.** H5 moves bond properties into
  store-side bond columns and removes the `Bond*`/`Atom::bond_[]` duality
  (`V22-DECISIONS.md` §D51; `V22-ARCH-HANDLE-MODEL.md` §6). H6 owns JSON v2
  and property-surface break (`V22-H0-ADDENDUM.md` §D62;
  `V22-ARCH-HANDLE-MODEL.md` §5). A final JSON v2 schema should not ship
  before the final bond representation is known. Either make H6 depend on
  H5, or split H6 into H6a topology/container/property JSON after H4 and
  H6b final bond JSON after H5.
- **H7 is not purely after H4 and independent.** VIEW work may need the D59
  timestamp/selection decision: D59 explicitly says an optional
  per-container `selection_stamp` column may be added in H7 if VIEW needs it
  (`V22-H0-ADDENDUM.md` §D59). That feeds back into store schema, JSON, and
  migration docs. H7 should include an explicit "VIEW redraw/stamp contract"
  subtask before H6 JSON schema is frozen.
- **MSVC CI should not wait for H7 as a lump.** The Windows build leg is a
  prerequisite for H8 size assertions and later repo split, but it is
  infrastructure and can start earlier once the v2.2 branch has a stable
  build shape (`v2.1/BACKLOG.md` §V21-MSVC-CI-PORTABILITY). Keep the H7
  acceptance gate, but add an early "bring-up may run in parallel after H2"
  note.
- **H3 compatibility shim lifetime needs to be explicit.** The architecture
  doc says the dual `Composite&`/handle API exists through H2-H3 and is
  deleted at H4 (`V22-ARCH-HANDLE-MODEL.md` "Open sub-questions"). The draft
  mentions "Processors take Atom by value" but not the deletion decision and
  migration-shim acceptance rule.
- **Repo split needs more than VIEW off Composite.** The draft correctly
  orders H7 before v2.4, but `JOINT-ROADMAP.md` requires stable installed
  interfaces, exported CMake targets, independent CI, versioning, and a
  compatibility matrix. Those are not implementation details; they are a
  v2.4 work package. Add "libBALL installed package/exported targets/version
  contract" as an explicit prerequisite before any physical repo split
  (`JOINT-ROADMAP.md` "2.x roadmap addition — separation").

## RR1-3. Gaps

The seven open questions in draft §6 are real, but incomplete.

Missing gaps that should be added:

1. **SIP/Python-off invariant and re-enable policy.** v2.2 must keep SIP off
   and prove it; pyBALL v2 re-enables wrappers with
   `BALL_PYTHON_WRAPPER` validity checks after handle API stabilization
   (`V22-DECISIONS.md` §D54; §D52.3-7).
2. **D31b/D66a encapsulation gate upkeep.** The roadmap needs a named guard
   for `_moleculeStoreInternal.h`, public handle headers, and iterator
   headers (`V21-DECISIONS.md` §D31b; `V22-H1b-DESIGN.md` §D66a).
3. **Pre-existing D31b grep false-positive/comment policy.** The gate must
   be precise enough not to fail on comments/prose while still catching type
   leaks. The draft should require a maintained script, not ad hoc grep in a
   CI YAML block.
4. **Downstream migration guide at H4/H6.** D50/D61 require deliberate break
   documentation and ledger feed-through (`V22-DECISIONS.md` §D50;
   `V22-H0-ADDENDUM.md` §D61), but the unified roadmap lacks a release
   migration-guide deliverable.
5. **H2-H3 mirror performance budget.** Dual existence makes every forward
   mutation update v0 first and mirror to the table (`V22-H2-DESIGN.md`
   §D67). The draft needs a perf/regression budget for mutation-heavy
   import/build paths before H3/H4.
6. **v2.4 CMake/package/versioning/CI as first-class work.** The split is
   not just moving files; it requires exported targets, install-tree tests,
   ABI/version policy, compatibility matrix, and release automation
   (`JOINT-ROADMAP.md` split requirements).
7. **H3 processor-by-value shim policy.** Decide whether compatibility
   adapters exist, how long, and whether H4 requires zero remaining
   `UnaryProcessor<Composite>` atom consumers.
8. **JSON v1/v2 converter scope after D62.** The draft says converter in H6,
   but it should specifically mean v2.1 System JSON -> v2.2 container JSON,
   not the already-delivered K0.6.4 v1->v2 converter.
9. **Stale source-doc labels.** `V22-ARCH-HANDLE-MODEL.md` still titles
   itself "v3.0 (on the v2.2 branch)" even though D53 overrides the rename.
   The roadmap can tolerate historical superseded text, but the canonical
   plan should call it out to avoid reopening version debate accidentally
   (`V22-DECISIONS.md` §§D52.1,D53).

## RR1-4. Backlog Placement

Section 3 needs a cleanup pass before lock:

- `V21-MSVC-CI-PORTABILITY` -> H7/H8 is acceptable, but add earlier parallel
  bring-up and reuse for v2.4.
- `V21-VIEW-RTTI` -> H7 is correct.
- `V21-MUTATION-WIRING` -> H2 is correct.
- `V21-STORE-ITER-API` -> H2c/H3 is correct, because H2 supplies
  handle-yielding traversal and H3 migrates consumers (`V22-H2-DESIGN.md`
  §D71).
- `V21-GENERATION-GUARD` -> H6 is mostly correct for Atom stale-handle
  detection, but container generation was already H1b D65; say "Atom/Bond
  and Python wrapper guard completion" to avoid double-counting
  (`V22-H1b-DESIGN.md` §D65).
- `V21-BIT-PROPERTY-COLUMN` -> H6/profile-gated backlog is acceptable.
- `V21-CI-PERF-GATES` -> H8 is correct.
- `JSON load batching` should be removed or reframed as "non-empty
  adoption/import batching if profiling shows a new H2/H3 mirror regression."
  The v2.0 save/load gap was closed in v2.1 (`RELEASE-NOTES-v2.1.md`
  "JSON load is 7-14x faster").
- `AndNode tmp-bitmap reuse` should be removed; delivered in v2.0.
- `Element-instance-id-table` should be removed or reframed as an explicit
  kernel element-registry decision, probably backlog/declined, not H6 JSON
  (`P4.3-FINDINGS.md`).
- `Track B Expression/Selector surface` is a real risk, but "audit in H3c"
  is too narrow. FORMAT-heavy H3c should audit PDB/naming/residue consumers;
  selector/expression parity should also be part of H3a/H3c because
  `Selector` is KERNEL-facing while FORMAT supplies corpus coverage
  (`P3-CLASSIFICATION.md` strategy; `RELEASE-NOTES-v2.0.md` test-surface
  note).
- `K0.6.4 v1->v2 JSON converter` should be removed as delivered. Add a new
  item: "v2.1 JSON -> v2.2 JSON v2 topology converter" in H6 if needed.

No backlog item obviously needs a whole new top-level phase, but pyBALL does:
it should be a post-H4/post-v2.2 milestone with a rewritten Phase 6 bake-off
input, not a small cross-cutting note.

## RR1-5. Version Strategy

Recommendation: **keep the handle flip as v2.2, but do not fold String
removal and repo split into the same release unless they are already ready
before rc1.**

Rationale:

- D53 is explicit: the maintainer chose "2.x is the modernization band" and
  declined v3.0 despite the semver argument (`V22-DECISIONS.md` §D53). The
  unified roadmap should not relitigate that as an open decision unless the
  maintainer asks.
- Downstream ergonomics argue for one clearly documented v2.2 handle break
  wave, not an unbounded "everything modern" release. H4/H6 already breaks
  pointer returns, processors, property mutable references, JSON topology,
  and possibly VIEW assumptions (`V22-H0-ADDENDUM.md` §§D61-D62).
- `BALL::String` removal is wide and mechanical, touching signatures,
  FORMAT parsers, persistence, and binary/persistent IO (`JOINT-ROADMAP.md`
  String addition). Folding it into v2.2 is attractive only if it lands
  before downstreams migrate to the handle API. If not ready, ship it as
  **v2.3** with its own ledger and migration guide.
- Repo split is not an API break so much as distribution/build-system
  breakage. It belongs after a stable libBALL install package exists, as
  **v2.4**, not in the same tag as the H4/H6 source break.
- Do not rename v2.2 to v3.0 in the roadmap. Instead, say plainly:
  "semver-major-sized breaks inside the 2.x modernization band."

Concrete version plan:

- **v2.2.0:** handle hierarchy flip, H5/H6/H7 close, JSON schema v2, D13
  verified, migration guide.
- **v2.3.0:** `BALL::String` -> `std::string`, unless completed before
  v2.2 rc1 with no schedule risk.
- **v2.4.0:** installed libBALL package contract + repo split.
- **pyBALL v2:** after H4 at minimum; preferably after v2.2.0 and aligned
  with v2.4 packaging if it consumes installed libBALL.

## RR1-6. D13 Closure

Making H4+H8 the D13 success gate is correct, but the target statement is
currently fuzzy.

Source facts:

- v2.1 says D13 is not closed; SoA columns are ~139 B/atom under the
  <=160 B budget, but full `Atom` remains over budget because inheritance
  remains (`RELEASE-NOTES-v2.1.md` "What's NOT" and "Known limitations").
- D40 dropped the v2.1 `sizeof(Atom) <= 32 B` target; v2.2 reintroduces the
  handle goal when inheritance is actually deleted (`v2.1/BACKLOG.md`
  §V21-MSVC-CI-PORTABILITY; `V2X-ROADMAP.md` §2 H8).
- H1b pins container handles at 24 B but explicitly says container handle
  size is not the D13 memory case (`V22-H1b-DESIGN.md` §D64).

Recommended D13 acceptance assertion:

1. At H4, `Atom` is a non-polymorphic value handle with no `Composite`,
   `PropertyManager`, or `Selectable` base and no inline tree/property/bond
   state.
2. At H8, CI asserts on every supported compiler, including MSVC:
   `sizeof(BALL::Atom) <= 32` and `sizeof(BALL::Bond) <= 32` unless the
   implementation intentionally documents a different fixed handle shape.
3. At H8, a representative 100k-atom System memory accounting test asserts
   total live per-atom store footprint remains `<= 160 B/atom` for the
   D13 baseline scenario, excluding JSON text and optional user property
   payloads but including mandatory store columns, parent/container indices,
   selection bits, bond CSR baseline, and side-table overhead.
4. H8 release notes state both numbers separately: handle object size and
   total per-atom store budget. Do not treat one as a proxy for the other.

## RR1-7. Lockability

Not lockable yet. The roadmap is close in structure, but it is not safe as
the canonical 2.x plan until the factual backlog mistakes and dependency
edges are corrected.

## Required Changes Before Lock

1. **Clean §3 backlog placement.** Remove delivered items (`AndNode`,
   K0.6.4 converter, JSON load batching as originally framed), drop/reframe
   Element-instance-id per P4.3, and add `V21-COMPOSITEASATOM-REMOVAL`.
2. **Replace the H5/H6/H7 parallel diagram.** Show H6 final JSON v2 depends
   on H5 bond representation, and H7 VIEW stamp/selection findings can feed
   the final store/JSON contract before H8.
3. **Add a v2.4 installed-package workstream.** Explicit deliverables:
   exported CMake targets, `find_package(BALL)` install-tree tests,
   version/ABI policy, compatibility matrix, independent CI, and release
   artifact flow before physical repo split.
4. **Rebaseline pyBALL.** State that `PYBALLV2.md` must be revised because
   its Phase 6 slice is Composite-era; pyBALL stays off during v2.2 and
   returns after handle API stabilization with `BALL_PYTHON_WRAPPER`
   validity checks.
5. **Add D31b/D66a gate maintenance.** Convert the internal-header/iterator
   grep into an explicit maintained CI script with comment/prose
   false-positive handling, and list it in H2-H4/H8 acceptance.
6. **Add H4/H6 migration documentation.** Require an API-break ledger close,
   downstream migration guide, and release-note break matrix for pointer to
   handle returns, processor signatures, property mutable-reference removal,
   JSON v2, and canonical `*Handle` renames.
7. **Add H2-H3 mirror perf gates.** Establish mutation/import benchmarks
   while v0+table dual existence is active so H2/H3 do not silently make
   PDB/JSON/import paths unusable.
8. **Clarify D13 numbers.** State separate H8 assertions for
   `sizeof(Atom/Bond) <= 32 B` and total D13 store footprint
   `<= 160 B/atom`; mention MSVC as a required leg.
9. **Tighten version strategy.** Adopt staged minors by default:
   v2.2 handle flip, v2.3 String, v2.4 split. Only fold String into v2.2 if
   ready before rc1 without destabilizing H4/H6/H8.
10. **Call out stale source-doc labels.** Note that D53 overrides the
    older v3.0 recommendation/title in `V22-ARCH-HANDLE-MODEL.md`, so the
    canonical roadmap remains v2.2 for the handle milestone.

## Overall Verdict

**NEEDS-REVISION.** The roadmap has the correct central thesis and the right
H2->H3->H4->H8 backbone, but it is not yet canonical. It currently carries
stale backlog items, underspecifies the split/build infrastructure, treats
post-H4 work as more parallel than the source docs support, and underplays
the pyBALL/SIP rebaseline. Apply the required edits above, then the plan can
be reviewed again for lock.
