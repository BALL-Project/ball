# V22 H3d design — STRUCTURE public-API cluster + remaining migrations

**Predecessor cluster:** H3c CLOSED at `f9f474558` (cluster CR GO-WITH-FIXES applied at `f9f474558`, advisory helper at `e6c7295b5`, follow-up opt-ins at `3e0155f41`/`f82754050`). 13 STRUCTURE files gate-clean. ctest 289/289.

**This doc scopes H3d.** Unlike H3c, H3d is heterogeneous — it bundles processors that H3c deferred for different reasons (public-API leaks; SmartsMatcher coupling; ILP solver state; design-debt). Each has its own gating consideration.

## H3d scope (5 sub-clusters)

| Sub | Processors | Why blocked from H3c | Migration target |
|---|---|---|---|
| **h3d.A** SmartsMatcher cluster | atomTyper, kekulizer, smartsParser, smilesParser, SmartsMatcher | Public API takes `vector<HashSet<Atom*>>` (SSSR input). Migrating atomTyper alone forces SmartsMatcher rewrite | StableId-keyed SSSR API across the cluster |
| **h3d.B** Public-Atom*-vector cluster | analyticalSES, atomBijection, reconstructFragmentProcessor, ringAnalyser | Public API surfaces use `HashMap<const Atom*, float>`, `vector<pair<Atom*, Atom*>>`, etc. | Migrate each per-file with StableId-keyed public API |
| **h3d.C** Builder cluster | peptideBuilder, sdGenerator, molecularSimilarity::createMolecule, rGroupAssembler, sideChainPlacementProcessor | Pattern D builders kept v0 per D-H3c.4 | Decision: opt-in for gate coverage OR retain v0 explicit-skip |
| **h3d.D** ILP solver | assignBondOrderProcessor | 8 header leaks in public ILP solver state | Refactor ILP solver to keyed-state API |
| **h3d.E** DNAMutator Pattern C | DNAMutator | 0 gate triggers but design-debt: three-phase decide/prevalidate/remove/insert per H3c-DR FLAW 4 | Three-phase rewrite per H3c R3 spec |

## D-H3d.1 — Sub-cluster ordering + dependencies

```
h3d.A (SmartsMatcher) ──┐
                         ├─→ h3d.D (ILP, uses atomTyper)
h3d.B (Public-vector) ───┤
                         └─→ H3d closing CR (after A+B+D land)
h3d.C (Builders) ──── independent, can run in parallel
h3d.E (DNAMutator) ── independent, design-debt cleanup
```

Recommended order: **h3d.E → h3d.B → h3d.A → h3d.D → h3d.C**

Rationale:
- h3d.E (DNAMutator) is smallest + isolated; good first Pattern C precedent for H3d.
- h3d.B (Public-vector files) is mostly independent file-by-file; each migration validates the boundary-translation idiom inherited from H3c.
- h3d.A (SmartsMatcher) is the biggest blocker because atomTyper / kekulizer can't migrate until SmartsMatcher API supports StableId-keyed inputs.
- h3d.D (ILP) depends on h3d.A (assignBondOrderProcessor uses atomTyper).
- h3d.C (Builders) is independent; can run anytime.

## D-H3d.2 — Pattern conformance

All sub-clusters inherit the H3c patterns (Codex-validated):

1. **Store-captured-at-first-push + reject-foreign-store** — for any processor that maintains state across multiple input atoms.
2. **Resolve-sid-to-Atom* at consumer site** (HBondProcessor's `resolve_` lambda) — for any processor with many access sites.
3. **Live-position resolution at apply time** (peptideCapProcessor's `computeDistance(store, vec<sid>)`) — for any geometric iteration that mutates positions between calls.
4. **Single-store member discipline** (e.g., `*_store_` member) — for any processor that stores sid sets between method calls.
5. **`MoleculeStore::bond_sid_of(const Bond*)`** — for any code holding a `Bond*` that needs the sid (already shipped at `e6c7295b5`).
6. **HandleKeyLeakGate opt-in per file** + reject the qualified-only regex (per D-H3c CR finding 3 deferred future-tightening).

## D-H3d.3 — Sub-cluster scoping details

### h3d.E — DNAMutator Pattern C three-phase (smallest; first)

Per H3c-DR FLAW 4. The existing `mutateSingleBase_` runs:
1. Identify `res_at` / `res_connection_at` / `frag_at` / `frag_connection_at` (PHASE 1).
2. `res_at->destroyBond(*res_connection_at)` (mutation in flight).
3. Geometry on frag_at.
4. `frag->removeNotHavingProperty(prop_)` (mutation in flight).
5. `res->removeHavingProperty(prop_)` (mutation in flight).
6. `setName`, `splice(*frag)` (mutation in flight).
7. `delete frag`.
8. `frag_at->createBond(*res_connection_at)` (uses captured raw pointer from step 1 AFTER 4 mutations).

Pattern C rewrite:
- **Phase 1 (identify)**: capture sids for `res_at_sid`, `res_connection_at_sid`, `frag_at_sid`, `frag_connection_at_sid`. Compute `res_connection` and `frag_connection` geometry. No mutation.
- **Phase 1.5 (prevalidate)**: all 4 sids must resolve. Throw `InvalidIterator` if any missing (per H3c R3 Pass 1.5 spec).
- **Phase 2 (remove old)**: destroyBond(res_at, res_connection_at); destroyBond(frag_at, frag_connection_at). Resolve sids to Atom* at each call. After remove, the 4 sids are STILL valid (atoms not deleted yet, just unbonded).
- **Phase 3 (apply geometric transform)**: rotate / align (operates on frag, uses captured Vector3 connection vectors — no sid resolution needed).
- **Phase 4 (insert new)**: `res->removeHavingProperty(prop_)` removes the base atoms (some sids may now be invalid — the prop_-marked subset). Splice frag into res. `setName`. Re-resolve `frag_at_sid` AFTER splice (Phase 4 reparented it) and `res_connection_at_sid` (not removed) — both should still resolve. createBond.
- **Phase 5 (cleanup)**: `delete frag`.

Estimate: ~120-line rewrite + close-review. 1 commit.

### h3d.B — Public-Atom*-vector cluster (4 files)

| File | Public surface leak | Migration approach |
|---|---|---|
| `analyticalSES` | `HashMap<const Atom*, float>& atom_areas` output param | Change to `HashMap<StableId, float>&` + add ovld w/ store; deprecate v0 form |
| `atomBijection` | `std::vector<std::pair<Atom*, Atom*>>` PUBLIC BASE CLASS | Change to `vector<pair<StableId, StableId>>` + extract base; ripples through 5-8 callers |
| `reconstructFragmentProcessor` | `HashSet<const Atom*>& allowed` param | Change to `HashSet<StableId>& allowed` + store ovld |
| `ringAnalyser` | `std::vector<Atom*>` in public `Ring` class | Change Ring's atoms member to `vector<StableId>` + accessor with store resolution |

Each file is 1 atomic commit. Estimate: 4 commits + per-file CR or 1 batched CR.

### h3d.A — SmartsMatcher cluster (biggest)

The SmartsMatcher API currently:
- `setSSSR(const vector<vector<Atom*>>&)` (input ring set)
- `match(vector<set<const Atom*>>& result, AtomContainer& mol, String expr, set<const Atom*>& atoms_to_match)` (in/out atom sets)

Migration options:
- (A1) **Overload-add**: keep v0 API; add `setSSSR(vector<vector<StableId>>&, MoleculeStore&)` + `match(...StableId...)` overloads. Callers migrate per-file. Risk: API divergence.
- (A2) **Replace + adapter**: change SSSR/match to sid-keyed; add inline adapter that takes the v0 form, translates at boundary. Callers don't change. Risk: hidden translation cost.
- (A3) **Deprecate + force-migrate**: change API, force every caller to migrate in one cluster commit. Risk: blast radius (atomTyper/kekulizer/smartsParser/smilesParser/RingPerception in one go).

Recommended: A2 (replace + adapter). The translation cost is amortized by reverse-map lookup (~O(1)); callers gain forward stability; API stays single-surface.

Estimate: SmartsMatcher + atomTyper + kekulizer + smartsParser + smilesParser. ~6 commits + CR.

### h3d.D — assignBondOrderProcessor ILP

8 header leaks in protected ILP solver state:
- `HashMap<Atom*, int>::Iterator it = solutions_[i].number_of_virtual_hydrogens.begin();`
- `std::map<Bond*, short> bond_fixed_;`
- `std::vector<Bond*> free_bonds_;`
- `HashMap<Bond*, Index> bond_to_index_;`
- `std::vector<Bond*> index_to_bond_;`
- `HashMap<Atom*, int> number_of_virtual_hydrogens_;`
- `HashMap<Atom*, int> atom_to_virtual_bond_index_;`
- `HashMap<Bond*, vector<float>> bond_lengths_penalties_;`

All keyed on Atom*/Bond*. ILP solver state. Migration: replace each with StableId-keyed. Atom*/Bond* → StableId resolution at every read site.

Risk: this is performance-sensitive (ILP solver iterates these heavily). The unordered_map<StableId, ...> lookup is O(1) but the hash is uint64 vs pointer (different cost profile). Need benchmark verification.

Estimate: 1 large commit + benchmark + CR.

### h3d.C — Builder cluster (decision required)

Per D-H3c.4: builders KEPT v0 because they construct from scratch (no v0-tree input to mutate). The decision is whether to opt these into the gate for forward coverage.

Options:
- (C1) **Skip entirely** — builders stay non-opt-in. Pro: zero churn. Con: no gate enforcement on future builder additions.
- (C2) **Opt-in only the ones with 0 leaks** — peptideBuilder, rGroupAssembler, sdGenerator if they're clean. Pro: gate forward coverage. Con: design said "stay v0".
- (C3) **Opt-in all + migrate any leaks** — full coverage. Con: contradicts D-H3c.4 explicit "stay v0".

Recommended: C1 (skip). The H3c design explicitly chose this; reversing it requires its own design decision.

Estimate: 0 commits.

## D-H3d.4 — Out of scope (carry-over from H3c)

- **STRUCTURE/RDF / SAS / SES / Connolly / RS*** (Pattern Z) — stay v0 (D-H3c.4 carry-over).
- **HCP-2d SecondaryStructure-as-annotation** — H4.
- **PDBAtom attribute migration** (occupancy / temperature_factor → store columns) — HCP-3.
- **FORMAT cluster** (PDBFile / Mol2File / SMILES / SDF) — H3e (own milestone) or H4.

## D-H3d.5 — Risk

- **HIGH**: h3d.D ILP performance regression. Need benchmark gate.
- **HIGH**: h3d.A blast radius (5+ files in one cluster). Codex DR specifically for SmartsMatcher API redesign.
- **MEDIUM**: h3d.B public-API ripples — each file has 5-8 callers across STRUCTURE / MOLMEC. Each commit's "caller migration" sub-commit needs its own gate.
- **LOW**: h3d.E DNAMutator three-phase — well-specified per H3c R3.
- **LOW**: h3d.C builder skip — explicit design decision; no risk.

## D-H3d.6 — Estimated effort

- h3d.E: 1 commit
- h3d.B: 4 commits + close-review
- h3d.A: 6 commits + design DR + close-review
- h3d.D: 1 large commit + benchmark + close-review
- h3d.C: 0 commits
- H3d closing CR: 1

Total: ~12-15 commits + 2-3 Codex DR + 3-4 close-reviews. Comparable to H3c (17 commits).

## D-H3d.7 — Open questions for Codex (H3d-DR)

Q1: Is the recommended ordering (E → B → A → D → C-skip) correct? Or should h3d.A go first because it unblocks the most consumers?

Q2: h3d.A migration option A2 (replace + adapter) vs A1 (overload-add) -- which has better cost/risk?

Q3: h3d.D ILP performance — should the migration be gated on a benchmark that asserts ≤ 5% regression? Or is a one-time measurement enough?

Q4: h3d.E DNAMutator Pattern C — is the 5-phase split (identify / prevalidate / remove / transform / insert+cleanup) per the design above correct, or should it follow a different decomposition?

Q5: h3d.C builder skip — should we instead add a NARROW gate exception that whitelists builders explicitly? Pro: documents intent; con: adds whitelisting complexity.

Q6: h3d.B `atomBijection` base class change — the v0 `class AtomBijection : public std::vector<std::pair<Atom*, Atom*>>` is sketchy from a modern-C++ perspective (inheriting from STL container). Should the migration take the opportunity to EXTRACT the vector into a private member?

Q7: Cross-cluster: with H3d expected to land ~15 more commits, is there value in adding a dedicated `MoleculeStore::handle_pair_of(const Atom*, const Atom*)` helper that returns `(StableId, StableId)` in one call, to reduce the boilerplate for h3d.D's pair-key state?

## H3d-DR — Codex adversarial round (next)

After this design lands at HEAD, run the Codex adversarial review prompt against this doc + the H3c lessons + the cited file paths. Iterate to GO / GO-WITH-FIXES, then implement per D-H3d.1 ordering.

## H3d-DR review (Codex)

Verdict: **GO-WITH-FIXES**. 2 BLOCKERs + 8 FLAWs + 1 NIT + 1 PRAISE.

- **BLOCKER 1**: ringAnalyser/sdGenerator coupling not addressed.
  `RingAnalyser::Ring::atoms` is public `vector<Atom*>` consumed
  directly by sdGenerator at 4+ sites. h3d.B can't migrate ringAnalyser
  without also touching sdGenerator (which h3d.C marked "skip").
- **BLOCKER 2**: h3d.D benchmark gate references `KernelCreation_bench.C`
  which measures atom creation/destruction, NOT ILP bond-order
  assignment. Real bond-order benchmark with baseline + <=5% threshold
  + revert criterion needed.
- **FLAW 3**: h3d.D does NOT depend on h3d.A (no atomTyper dep in
  assignBondOrderProcessor.C). My dependency claim was wrong.
- **FLAW 4**: A2 adapter cost is per-rule (atomTyper's match() loop),
  not amortized. A1 (overload-add) preferred.
- **FLAW 5**: h3d.A scope misses KERNEL predicates, QSAR, applications,
  tests as SmartsMatcher callers.
- **FLAW 6**: atomBijection base-class extraction should be its OWN
  preparatory commit before StableId migration.
- **FLAW 7**: h3d.D inventory missed BONDORDERS/* strategy files
  (ILPBondOrderStrategy has its own Bond* maps).
- **FLAW 8**: DNAMutator phase text imprecise about which sids
  invalidate. Spec: res_at_sid CAN invalidate (it's a base atom
  removed by removeHavingProperty); res_connection_at_sid stays valid
  (C1 connection); frag_at_sid stays valid across splice.
- **FLAW 9**: analyticalSES downstream caller migration (Solvation,
  QSAR consumers) broader than stated.
- **FLAW 10 (NIT)**: D-H3d.2 pattern inheritance overbroad. Public
  output maps + public value objects need different treatment.
- **FLAW 11**: Estimates low. Revised: **20-28 commits, 4-6 reviews,
  dedicated D benchmark review**.
- **PRAISE 12**: DNAMutator-first ordering correct (low blast radius,
  already opted in, current code holds raw pointers across mutations).

## H3d-R2 — Revised scoping (DR fixes applied)

### D-H3d.1-R2 — Revised sub-cluster ordering

**E → atomBijection-extraction → A-API-decision → B-with-ringAnalyser/sdGenerator-decision → D-with-benchmark → C-explicit-skip**

Key changes vs R1:
- Insert **atomBijection-extraction** as a prep step BEFORE B (FLAW 6).
- A goes BEFORE B because the SmartsMatcher API decision unblocks the
  rest of B's plan (atomTyper / kekulizer cleanup gates on A landing).
- B's ringAnalyser entry becomes a coupled `ringAnalyser + sdGenerator
  + downstream callers` cluster (BLOCKER 1).
- D is decoupled from A (no longer "h3d.A blocks h3d.D" -- FLAW 3 fix).
- D requires its own benchmark commit BEFORE migration (BLOCKER 2 fix).
- C disposition is explicit "skip + document": no opt-in for builders.

### D-H3d.3-R2 — Revised sub-cluster details

#### h3d.E DNAMutator (1 commit, first)

Per-sid validity table (FLAW 8 fix):
| Sid | Phase 1 | Phase 2 destroyBond | Phase 4a removeHavingProperty(prop_) | Phase 4b splice | Phase 4c createBond |
|---|---|---|---|---|---|
| res_at_sid | valid | valid (unbonded) | **invalid** (base atom removed) | n/a | n/a |
| res_connection_at_sid | valid | valid | valid (C1 NOT marked) | valid | valid |
| frag_at_sid | valid | valid | n/a | valid (reparented to res) | valid |
| frag_connection_at_sid | valid | valid (unbonded) | valid (frag retains) | **invalid** (delete frag) | n/a |

Phase 1.5 prevalidate: all 4 sids valid.
Phase 2: destroyBond ×2. After this: all sids still valid.
Phase 3: geometric transforms (no sid resolution).
Phase 4a: res->removeHavingProperty(prop_). res_at_sid now invalid.
Phase 4b: splice frag into res. frag_at_sid REMAINS valid (atom
moved but slot intact).
Phase 4c: re-resolve res_connection_at_sid + frag_at_sid; createBond.
Phase 5: delete frag. frag_connection_at_sid now invalid (slot freed
by frag dtor).

Estimate: 1 commit + close-review.

#### atomBijection-extraction (NEW prep commit, before B)

Per FLAW 6. Extract the `std::vector<std::pair<Atom*, Atom*>>` base
into a private member; preserve the index-access semantics via
operator[] + size() + push_back forwarders. NO StableId migration
in this commit -- just the inheritance break.

Estimate: 1 commit + close-review (touches RMSD/StructureMapper/
sideChainPlacement that use `ab[i].first/second`).

#### h3d.A SmartsMatcher cluster (revised)

Per FLAW 4 + 5. Migration uses **A1 overload-add** (NOT A2 adapter)
to avoid per-rule translation cost:

```cpp
// smartsMatcher.h public surface (additive):
typedef std::vector<std::set<MoleculeStore::StableId>> MatchSidSet;
void setSSSR(const std::vector<std::vector<MoleculeStore::StableId>>& rings, MoleculeStore&);
bool match(MatchSidSet& result, const AtomContainer&, const String& expr,
           const std::set<MoleculeStore::StableId>& atoms_to_match, MoleculeStore&);
// Existing v0 setSSSR / match REMAIN, route into the sid form via
// boundary-translation HELPERS inside smartsMatcher.C. This means v0
// callers still work; new callers (atomTyper migrated, kekulizer
// migrated) use the sid form directly.
```

Callers enumerated (FLAW 5):
- source/STRUCTURE/{atomTyper, kekulizer, smartsParser, smilesParser}
- source/KERNEL/standardPredicates.C (SMARTS predicate)
- source/QSAR/* (descriptor matchers)
- source/APPLICATIONS/* (CLI tools)
- test/{Smarts,Smiles,AtomTyper,Kekulizer}_test.C

Migration order:
1. smartsMatcher.h+C add sid-keyed overloads (boundary-translated to v0
   inside)
2. atomTyper migrate to sid form
3. kekulizer migrate to sid form
4. smartsParser/smilesParser migrate
5. standardPredicates SMARTS predicate migrate (kernel-level)
6. QSAR + APPLICATIONS migrate
7. h3d.A close-review

Estimate: 7-8 commits + 1 design DR (API decision) + 1 close-review.

#### h3d.B Public-Atom*-vector cluster (revised)

Per BLOCKER 1 + FLAW 9. atomBijection-extraction lands first (above).
Then per-file:

1. **analyticalSES** + downstream Solvation/QSAR/Pierotti consumers
   (FLAW 9 expanded scope) — 2-3 commits.
2. **atomBijection** StableId migration AFTER extraction —
   1 commit (touches RMSD/StructureMapper/sideChainPlacement).
3. **reconstructFragmentProcessor** — 1 commit.
4. **ringAnalyser** + sdGenerator coupling (BLOCKER 1):
   - Option B.4.a: migrate Ring::atoms to sids; sdGenerator becomes
     part of B (NOT skipped). Pro: clean ringAnalyser; con: sdGenerator
     loses Pattern D skip.
   - Option B.4.b: keep Ring::atoms as v0 vector<Atom*>; add a parallel
     `vector<StableId>` accessor. sdGenerator stays v0.
     Pro: preserves Pattern D skip; con: dual storage.
   - **Recommendation**: B.4.b for H3d (forward-stable accessor; v0
     surface preserved). Re-evaluate at H4 when builders retire.

Estimate: 5-6 commits + close-review.

#### h3d.D ILP solver (revised)

Per BLOCKER 2 + FLAW 7. Scope expanded to BONDORDERS strategy files
(ILPBondOrderStrategy + others).

Phase 0 (NEW prep commit): bond-order benchmark.
- Add `source/BENCHMARKS/AssignBondOrder_bench.C` modeled on
  KernelCreation_bench, exercising assignBondOrderProcessor on a
  representative molecule set (e.g., the test/data MOL2 fixtures).
- Capture baseline numbers in V22-DECISIONS.md.
- Required: ALL D commits gate on <=5% regression vs baseline.

Then per-file:
1. assignBondOrderProcessor.h leak elimination (8 sites) + boundary-
   translation in `.C`
2. BONDORDERS/ILPBondOrderStrategy.C + .h leak elimination
3. BONDORDERS/AStarBondOrderStrategy.C (similar pattern; verify)
4. Other BONDORDERS/* files surveyed
5. h3d.D close-review including benchmark verification

Estimate: 5-6 commits + benchmark prep + benchmark close-review.

#### h3d.C explicit skip (no commits)

Per Q5. Builders stay v0. NO opt-in. NO migration. Document in
D-H3c.4-R2 carry-over.

The H3c follow-up opt-ins (residueChecker, hybridisationProcessor,
defaultProcessors, UCK, nucleotideMapping) shipped at `3e0155f41` +
`f82754050` are NOT builders -- those were Pattern A clean.

Builders (peptideBuilder, sdGenerator, molecularSimilarity,
rGroupAssembler, sideChainPlacementProcessor, smilesParser-as-builder)
remain non-opt-in.

### D-H3d.2-R2 — Pattern conformance (NIT 10 fix)

The H3c patterns apply to SINGLE-INPUT processor STATE (where the
processor consumes atoms from one System). For:

- **Public output maps / vectors** (h3d.B): caller provides the
  container; processor populates. Cross-store risk is mitigated only
  by caller-side discipline. The h3d.B migration must specify whether
  output maps are caller-cleared (analyticalSES is) or caller-owned.
- **Public value objects** (atomBijection): need a "Bijection between
  TWO systems" semantic. The container holds pairs from (sys_a,
  sys_b); the sids are scoped to their respective stores. Migration
  must specify the 2-store invariant.

### D-H3d.6-R2 — Revised estimate

Per FLAW 11. Total: **20-28 commits + 4-6 design/close reviews + 1
benchmark review**. Sub-breakdown:
- h3d.E DNAMutator: 1 commit + 1 CR
- atomBijection extraction: 1 commit + 1 CR
- h3d.A SmartsMatcher: 7-8 commits + 1 design DR + 1 CR
- h3d.B remaining: 5-6 commits + 1 CR
- h3d.D ILP: 1 benchmark commit + 5-6 migration commits + 1 benchmark CR
- h3d.C skip: 0 commits
- H3d closing CR: 1

### D-H3d.7-R2 — Answered Q1-Q7

Q1: Order is `E → atomBijection-extraction → A → B → D-benchmark
    → D-migrate → C-skip` (revised per BLOCKER 1 + FLAW 6).
Q2: A1 overload-add (FLAW 4 fix).
Q3: Hard benchmark gate <=5% (BLOCKER 2 fix).
Q4: 5-phase split per the validity table above (FLAW 8 fix).
Q5: Builders skip + explicit document. No gate exception complexity.
Q6: Extract first (FLAW 6 fix).
Q7: handle_pair_of helper deferred; bond/atom sid helpers come up
    as needed inside D's strategy migrations.

### DESIGN LOCKED at R2.

Per the project's gated discipline, R3 would normally need DR2. The
R2 fixes are surface corrections (taxonomy + sub-cluster details +
order) that don't change the structural approach. Implementation
begins per the revised D-H3d.1-R2 order; each sub-cluster gets its
own design DR (h3d.A specifically) and close-review.

