---
milestone: v2.2
milestone_name: "Handle redesign — store-backed value handles + collapsed molecular hierarchy"
status: H3d closed; H4 (the flip) remaining before rc1
authored: 2026-06-04
predecessor: v2.1.0 (tagged 2026-05-20, commit `fdb871bb2`)
predecessor_context: MILESTONE-CONTEXT-v2.1.md (preserved)
---

# BALL 2.2 — Milestone Context

Project-level handover for v2.2 covering H0–H3d as landed and the
remaining H4–H8 critical path. Companion docs: `V2X-ROADMAP.md`
(authoritative kernel roadmap), `v2.2/V22-DECISIONS.md` (decision
log D45–D-H3d.CLOSE), `v2.2/V22-API-BREAK-LEDGER.md` (every break +
migration note), `v2.2/V22-H3d-DESIGN.md` (the cluster that just
closed).

## Scope of v2.2

v2.2 implements the handle redesign that v2.1 deferred — the
controlled break from `Atom`/`Bond`/molecular containers as
`Composite` subclasses to store-backed value handles over a flat
container metadata table. **Version stays 2.2** (D53; 2.x is the
breaking-modernization band, release notes loud about breaks).

Dual existence (D60) is the structural invariant of the v2.2 cycle:
through H1–H3 the v0 object tree is the source of truth and the
store table is a *verified mirror*; H4 inverts that (the store
table becomes sole source, v0 is deleted).

## Status snapshot (2026-06-04)

| Phase | Status | Closed at |
|---|---|---|
| **H0** design lock | ✅ CLOSED | R29–R36d (D45–D74) |
| **H1a** container table | ✅ CLOSED | R32b GO |
| **H1b** handle types | ✅ CLOSED | R34 GO |
| **H1b′** hierarchy collapse design | ✅ CLOSED | HC1→HC1b + HCP1→HCP1b + H1bP→H1bPb AGREE |
| **HCP-1** role columns + scalar mirror + selection mirror | ✅ CLOSED | KR1 + HCP1R reviews applied |
| **HCP-1P** KERNEL object-creation perf | ✅ CLOSED | A/B/C landed; D measured = NOT-JUSTIFIED |
| **HCP-2** collapsed handle API | ✅ CLOSED | HCP-2c carry-overs + HCP-2d secondary-structure-as-annotation |
| **H2** mutation-mirror + traversal | ✅ CLOSED | H2d-R1 GO, 286/286 + 100/100 stress |
| **H3a** handle migration shim + infra | ✅ CLOSED | gate + handle-key-leak regex |
| **H3b** KERNEL cluster consumer migration | ✅ CLOSED | Codex H3b close-review GO |
| **H3c** STRUCTURE cluster consumer migration | ✅ CLOSED | Codex H3c close-review GO |
| **H3d** STRUCTURE public-API cluster + remaining | ✅ **CLOSED 2026-06-04** | Closing-CR round 4 GO at `4e71e63f4` |
| **H4** the flip | ⏳ NEXT | Design lock + per-class deletion + parity-test retirement |
| **H5** Bond unify | ⏳ | Bond fully a handle, `Atom::bond_[]` removed |
| **H6a** property break + JSON containers | ⏳ | Runs after H4 |
| **H6b** final JSON schema v2 + converter | ⏳ | Depends on H5 |
| **H7** VIEW + MSVC | ⏳ | 54 VIEW Atom-RTTI sites + V21-MSVC-CI bring-up |
| **H8** D13 verify + release | ⏳ | `sizeof(Atom)/Bond) ≤ 32 B` + 160 B/atom store footprint + rc1 → v2.2.0 |

Where H3d landed (this milestone's most recent shippable boundary):
- `origin/v2.2` HEAD `4e71e63f4`.
- 289/289 ctest GREEN; HandleKeyLeakGate clean.
- AssignBondOrder benchmark within ±0.3% of D-H3d.D.BASELINE
  (AStar 6.54 s vs 6.55 s baseline, FPT 5.93 s vs 5.91 s baseline).
- Public API state: all v0 entry points preserved (A1 overload-add
  strategy applied across SmartsMatcher, analyticalSES, AtomTyper
  setters). Internal state is sid-keyed in the migrated consumers.

## What H3d delivered (since v2.1.0)

Per `v2.2/V22-DECISIONS.md` D-H3d.CLOSE:

- **h3d.E** DNAMutator Pattern C rewrite (`80e276c57`) — per-sid
  validity table threads through the 5-phase mutate.
- **atomBijection extract** (`f48b9485a`) — HAS-A composition over
  std::vector base, preserves `operator[]`/iterator API.
- **h3d.A SmartsMatcher cluster** (`a12608196` → `029d978f8`) —
  overload-add A1 across SmartsMatcher + atomTyper + MMFF94
  cascade + kekulizer + SMARTSPredicate, with 2 Codex CR rounds
  inline (CR1 = 5 fixes, CR2 = 2 fixes) and round 3 GO.
- **h3d.B Public-Atom*-vector cluster** (`4041026c5` →
  `efebe25be`) — atomBijection sid migration +
  reconstructFragmentProcessor + analyticalSES sid overload +
  ringAnalyser B.4.b parallel sid accessor.
- **h3d.D ILP cluster** (`1de7f2b16` → `45b4c968b`) —
  AssignBondOrder benchmark + ABO gate opt-in + BONDORDERS
  strategies opt-in; transient scratch carve-outs documented;
  ILP path coverage gap surfaced in-source.
- **h3d.C builder skip** — no commits per design (peptideBuilder /
  sdGenerator / molecularSimilarity / rGroupAssembler /
  sideChainPlacementProcessor / smilesParser-as-builder stay v0).
- **H3d closing-CR** (`595a80ddc` → `4e71e63f4`) — 4 rounds, 10
  findings applied inline, round 4 GO.

Known residual hazards (documented in-source, deferred to H4):

1. `bondOrderAssignment` maps survive in `ABO::solutions_`
   between apply() calls; caller contract is "valid until next
   MoleculeStore mutation OR next apply()."
2. `ILPBondOrderStrategy` is compiled-out on default
   Homebrew/Apple Silicon (no `BALL_HAS_LPSOLVE`); the H3d.D
   benchmark baseline omits the ILP path. Any future ILP-touching
   commit MUST re-run `AssignBondOrder_bench` on a configured
   build.
3. `MMFF94AtomTyper::assignTo` now THROWS
   `Exception::InvalidArgument` on multi-store System input. Hard
   v2.1→v2.2 break. Internal callers unaffected; external
   consumers feeding multi-store Systems must catch + pre-split.

## What's left in v2.2

The remaining work is summarised in V2X-ROADMAP.md §2; the headline
gate is D13: `sizeof(BALL::Atom) ≤ 32 B` + `sizeof(BALL::Bond) ≤
32 B` + total live store footprint ≤ 160 B/atom, verified on every
supported compiler including MSVC.

H4–H8 are each their own design-locked phase per the standing v2.2
pattern (KERNEL-V2-DECISIONS / V22-DECISIONS log; Codex DR before
implementation). They are NOT bundled into one closure commit.

H4 is the next phase to design-lock and execute. H5 follows H4
(Bond unify). H6a/H6b run after H5 (property break + JSON schema
freeze). H7 runs in parallel with H6 once the build shape is
stable (VIEW + MSVC CI bring-up). H8 is the closing release gate
(D13 verification + rc1 → v2.2.0).

## v2.1 backlog placement

Per V2X-ROADMAP.md §3, the v2.1 backlog items map to v2.2 phases:

| v2.1 backlog item | v2.2 placement |
|---|---|
| V21-MSVC-CI-PORTABILITY | H7 gate (bring-up may start in parallel after H2) |
| V21-VIEW-RTTI (54 sites) | H7 |
| V21-COMPOSITEASATOM-REMOVAL | H4 (the flip dissolves the helper's premise) + H7 (VIEW sites) |
| V21-MUTATION-WIRING (D39) | ✅ ADDRESSED by H2 |
| V21-STORE-ITER-API | ✅ ADDRESSED by H2c (handle iterators) / H3 |
| V21-GENERATION-GUARD | H6a (Atom/Bond + Python-wrapper guard completion) |
| V21-BIT-PROPERTY-COLUMN | H6a *if* profiling shows the sparse path is hot |
| V21-CI-PERF-GATES | H8 (full CI gate); measurement-only pulled forward to HCP-1P (done) |
| Track B FORMAT/STRUCTURE Expression/Selector | H3a + H3c ✅ |

## Standing invariants for the rest of v2.2

(per V2X-ROADMAP.md §2)

(a) **SIP/Python stays OFF** and CI proves it isn't built (D52.3-7
    / D54). pyBALL v2 wrappers return only AFTER the handle API
    stabilises post-H4.
(b) **D31b/D66a encapsulation gate** (`_moleculeStoreInternal.h` +
    `composite_nodes_`/`CompositeNode*` out of public/iterator
    headers; `containerHandle.h` off the internal header) is
    enforced by a maintained script with comment/prose
    false-positive handling.
(c) **Every landed break is enumerated in
    `V22-API-BREAK-LEDGER.md`** with a migration note + Codex
    sign-off.
(d) **Mutation/import perf budget** holds through the H2–H3
    dual-existence window — the forward-only mirror never makes
    import paths unusable. Tracked via PDB load / JSON / build
    benchmarks; AssignBondOrder_bench is now the canonical
    bond-order regression gate (5% threshold).

## Navigating the v2.2 planning tree

- `V2X-ROADMAP.md` — authoritative kernel roadmap (v2.0 through
  v2.4)
- `v2.2/V22-DECISIONS.md` — decision log D45 → D-H3d.CLOSE
- `v2.2/V22-API-BREAK-LEDGER.md` — every break + migration note
- `v2.2/V22-ARCH-HANDLE-MODEL.md` — handle architecture spec
- `v2.2/V22-H{0,1a,1b,1bPRIME,2,3,3c,3d,HCP1,HCP2}-DESIGN.md` —
  per-phase design locks
- `v2.2/V22-CODEX-REVIEW-ROUND{29..36}.md` — DR sign-offs

## Release criteria for v2.2.0

(per V2X-ROADMAP.md §2 H8 table row)

1. At H4 `Atom`/`Bond` are non-polymorphic value handles with no
   `Composite`/`PropertyManager`/`Selectable` base and no inline
   tree/property/bond state.
2. CI asserts `sizeof(BALL::Atom) ≤ 32 B` AND `sizeof(BALL::Bond)
   ≤ 32 B` on every supported compiler including MSVC.
3. A 100k-atom memory-accounting test asserts total live store
   footprint ≤ 160 B/atom (mandatory columns + parent/container
   indices + selection bits + bond-CSR baseline + side-table
   overhead; excluding JSON text + optional user properties).
4. Release notes state handle size AND per-atom budget
   **separately** (neither is a proxy for the other).
5. Pinned-baseline perf gates (V21-CI-PERF-GATES) pass.
6. AssignBondOrder_bench within 5% of D-H3d.D.BASELINE on a
   configured ILP-enabled build (closes the H3d.D coverage gap).
7. v2.2.0-rc1 → v2.2.0 + downstream migration guide.

---

*Authored 2026-06-04 at H3d closure.*
