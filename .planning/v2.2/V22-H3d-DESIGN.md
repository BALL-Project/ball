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
