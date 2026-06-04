# V22 H3c design — STRUCTURE cluster consumer migration

**Predecessor cluster:** H3b (KERNEL extractors) CLOSED at `54b30d606`.
H3a.3b PDBAtom-origin (D-H3.6) IMPLEMENTED end-to-end. ctest 288/288.
HandleKeyLeakGate clean across H3b additions.

**This doc locks H3c.** Design-lock-before-code per the project's gated
discipline. Codex H3c-DR adversarial round follows immediately, with R2/R3
revision cycles as needed (same as H3a.3b which converged at DR4).

## H3c scope

`source/STRUCTURE/` — 73 files, ~53 K LOC. Two boundaries:

### Boundary A — Topology-mutating processors (Pass 1 + Pass 2 rewrite)

These call `createBond`, `destroyBond`, `Atom::insert`, mutate atom types /
bond orders, or otherwise change the v0 Composite tree:

| Processor | What it does | Migration tier |
|---|---|---|
| `addHydrogenProcessor` | Adds H atoms + bonds | T1 (must) |
| `assignBondOrderProcessor` | Mutates bond orders + adds H | T1 |
| `buildBondsProcessor` | Creates bonds by geometry | T1 |
| `disulfidBondProcessor` | Adds disulfide bonds | T1 |
| `DNAMutator` | Replaces residues (topology-heavy) | T1 |
| `HBondProcessor` | Records hydrogen bonds | T1 |
| `peptideCapProcessor` | Adds capping atoms + bonds | T1 |
| `peptideBuilder` | Builds peptides from scratch | T2 |
| `kekulizer` | Mutates aromatic bond orders | T2 |
| `atomTyper` | Mutates atom types | T2 |
| `fragmentDB` | Provides fragment templates | T2 (orthogonal) |

T1 = first pass; T2 = second pass after T1 close-review GO.

### Boundary B — Pointer-key containers (D-H3.8 leaks)

Files using `HashSet<Atom*>`/`HashSet<Bond*>` / similar as KEY storage.
These violate D-H3.8 once the file opts into handle headers:

```
analyticalSES, atomBijection, atomTyper, geometricProperties,
reconstructFragmentProcessor, molecularSimilarity, buildBondsProcessor,
fragmentDB, ringAnalyser, smartsParser, kekulizer, sdGenerator
```

Each needs `HashSet<StableId>` or `std::unordered_set<AtomHandle>` (via
the `std::hash<AtomHandle>` specialization shipped in `atomHandle.h`).

### Boundary C — `dynamic_cast<Atom*>` / `<Bond*>` sites

Files using v0 RTTI to walk the Composite tree:

```
disulfidBondProcessor, fragmentDB, peptideBuilder, residueChecker
```

Replace with `AtomHandle` / `BondHandle` via the handle-extractor surface
already shipped (H3b.1-6).

## D-H3c.1 — Two-pass rewrite pattern (per H3-DR2 Q2=(b))

Every topology-mutating processor rewrites as:

```cpp
// PASS 1: handle-yielding walk, accumulate decisions (NO mutation).
//   Decision set is StableId-keyed (D-H3.8) so it survives reparent /
//   reallocation between Pass 1 and Pass 2.
struct Decision { ... StableId-typed fields only ... };
std::vector<Decision> decisions;
for (AtomHandle a : atomHandles(root_container))
{
    if (predicate(a)) decisions.push_back({a.getStableId(), ...});
}

// PASS 2: apply mutations using AtomHandle -> v0 bridge.
//   Resolve StableId -> AtomHandle each iteration (StableId is durable
//   across recycles; raw AtomHandle stored across the gap may go stale
//   if Pass 1 captured one BEFORE a mutation invalidated it).
for (const Decision& d : decisions)
{
    AtomHandle a = store.atomHandleByStableId(d.atom_sid);  // re-resolve
    if (!a.isValid()) continue;                              // skipped
    Atom* v0_atom = a.getAtom();                             // bridge
    if (v0_atom) v0_atom->createBond(...);                   // mutate via v0
}
```

**Key invariant:** Pass 1 walks must NEVER store raw `Atom*` / `Bond*` keys
in the decision set. Pass 2 mutations bridge through `getAtom()` /
`getBond()` ONLY at the apply moment (no stored pointer-identity).

This pattern keeps the migration HandleKeyLeakGate-clean per the D-H3.8
contract.

## D-H3c.2 — Decision-set helpers (additive on AtomHandle / BondHandle)

Where the existing handle surface is insufficient for two-pass rewrites,
add new helpers without touching v0:

```cpp
// MoleculeStore: re-resolve a StableId -> AtomHandle (or null handle).
AtomHandle MoleculeStore::atomHandleByStableId(StableId sid) const;
BondHandle MoleculeStore::bondHandleByStableId(StableId sid) const;

// Already exists: AtomHandle::getStableId(), BondHandle::getStableId().
// Already exists: HandleKeyLeakGate enforces D-H3.8 per opt-in file.
```

The two functions above are the missing piece for the two-pass pattern's
"re-resolve at apply time" step. Cost: O(1) via the existing
`stable_id_to_idx_` map (or O(log N) with a scan-fallback if no map).

## D-H3c.3 — Implementation order (commit sequence)

After H3c-DR GO:

1. **D-H3c.2 helpers**: `atomHandleByStableId` + `bondHandleByStableId` on
   MoleculeStore. ZERO callers in this commit (infrastructure).
2. **HandleKeyLeakGate activation**: enable the gate for the first migrated
   STRUCTURE file as proof-of-concept.
3. **Tier 1, processor 1** (`addHydrogenProcessor`): two-pass rewrite +
   close-review.
4. **Tier 1, processors 2-7**: same pattern.
5. **Tier 1 close-review** (Codex CR-T1) covering the cluster pattern.
6. **Tier 2 processors** (peptideBuilder, kekulizer, atomTyper, fragmentDB).
7. **Pointer-key leaks**: convert `HashSet<Atom*>` → `HashSet<StableId>`
   per file (Boundary B; can be batched with the consuming processor).
8. **dynamic_cast<Atom*>** → handle predicates (Boundary C; small).
9. **Tier 2 close-review** (Codex CR-T2).
10. **H3c overall close-review** (Codex CR-H3c).

Estimated atomic commits: ~25-30. Estimated DR rounds: 2-3 (cf. H3a.3b's 4).

## D-H3c.4 — Out of scope for H3c

- **FORMAT cluster** (PDBFile / Mol2File / SMILES / SDF / etc.) — H3d.
- **Persistence schema changes** — H3a.3b shipped origin_flags column;
  no further schema work in H3c.
- **PDBAtom attribute migration** (occupancy / temperature_factor →
  store columns) — HCP-3.
- **STRUCTURE/RDF / SAS / SES / Connolly** (read-only geometric processors) —
  no mutation, no D-H3.8 leak; defer to later if any cleanup needed.

## D-H3c.5 — Risk assessment

- **HIGH**: topology mutation across the dual-existence boundary (v0 tree
  + store mirror). Bond mutation during iteration is the classic
  iterator-invalidation hazard. The two-pass pattern mitigates by
  separating decision from mutation.
- **MEDIUM**: bond CSR rebuild cost. Each Pass 2 call to `createBond` /
  `destroyBond` invalidates the CSR. Batching mutations within Pass 2
  amortizes the rebuild.
- **LOW**: HandleKeyLeakGate false positives. Already tested on H3b files;
  the regex is conservative.

## D-H3c.6 — Open questions for Codex (H3c-DR review)

Q1: Two-pass pattern — is it sufficient for topology-mutating processors
that BOTH add AND remove atoms in one walk (e.g., DNAMutator replaces a
residue)? Or do we need a three-pass: (a) walk + decide, (b) remove old,
(c) add new?

Q2: The decision-set helper `atomHandleByStableId(sid)` is O(?) on the
current MoleculeStore. Is a stable_id → idx map already present? If not,
adding one is part of H3c.

Q3: HandleKeyLeakGate is currently a CMake-script gate. Should it run
PER MIGRATED FILE (i.e., the gate scans the file only when its name
appears in a `H3C_MIGRATED_FILES` list) or globally (scanning all files
that include atomHandle.h)?

Q4: For the Pass 1 / Pass 2 boundary, what's the right exception-safety
contract? If Pass 1 succeeds but Pass 2 throws midway, the store is in
a partial state. Should the design require Pass 2 to be transactional
(roll back to pre-Pass 2 state on throw)?

Q5: kekulizer is in Tier 2. Its mutations are bond-order-only (no
atom add/remove). Does the two-pass pattern actually buy anything for a
pure-order-mutation processor, or should it stay one-pass with handle
adoption only at the leaf?

Q6: fragmentDB — provides Atom/Bond templates by name. Is the migration
to handle-side just `getAtom()`-bridging the existing v0 interface, or
does it require a deeper redesign of the fragment template format?

Q7: `atomTyper` mutates atom_type column. Single-pass with handle-side
walk, mutating store column directly via AtomHandle? Or does it need
two-pass for any reason?

## H3c-DR — Codex adversarial round (next)

After this design lands at HEAD, run the Codex adversarial review prompt
against this doc + the H3 cluster context (HandleKeyLeakGate, D-H3.8
contract, the H3a.3b implementation as a reference for what's already
done). Iterate to GO / GO-WITH-FIXES, then implement per D-H3c.3.

## H3c-DR review (Codex)

Verdict: **GO-WITH-FIXES**. 3 BLOCKERs + 8 FLAWs + 1 NIT.

- **BLOCKER 1**: D-H3c.2 assumed an existing `stable_id_to_idx_` map.
  None exists -- `MoleculeStore` has parallel `stable_ids_` /
  `bond_stable_ids_` vectors only. Phase-0 work: add reverse maps with
  full lifecycle spec.
- **BLOCKER 2**: Pass 2 "if (!a.isValid()) continue" rule too weak. Some
  processors (DNAMutator, HBondProcessor) throw on missing structural
  preconditions. Need per-processor skip-vs-error classification.
- **BLOCKER 3**: Q4 exception safety not locked. Pick the contract.

FLAW 4-11: per-processor analysis (DNAMutator needs three-phase;
addHydrogen Pass 1 must record FULL H placements with positions+names;
fragmentDB needs explicit bridge boundaries; kekulizer stays one-pass
but heavy D-H3.8 cleanup needed; atomTyper one-pass; Pass 2 ordering
linearizable; concurrent UB inherit from store; scope undercount missed
molecularSimilarity / sdGenerator / rGroupAssembler / BONDORDERS).

NIT 12 (HandleKeyLeakGate): keep global-by-include. No change needed.

## H3c-R2 — Revised design (BLOCKER + FLAW fixes)

### D-H3c.0 — Phase-0: stable_id reverse maps (NEW)

`MoleculeStore` gets two private reverse maps + public lookup
functions (atomic per the existing orphan/bond mutexes):

```cpp
// moleculeStore.h private members
std::unordered_map<StableId, Index>          atom_sid_to_idx_;
std::unordered_map<StableId, std::uint32_t>  bond_sid_to_idx_;

// public reader API (PHASE-0):
AtomHandle atomHandleByStableId(StableId sid) const;   // null handle if unknown
BondHandle bondHandleByStableId(StableId sid) const;
```

Lifecycle:
- `allocate_atom` (3 overloads): insert `(new_sid, idx)` into
  `atom_sid_to_idx_` BEFORE returning.
- `release_atom`: erase `(stable_ids_[i], i)` from the map BEFORE
  zeroing the slot. (Idempotent: erase a missing key is a no-op.)
- `clear()`: clear the map.
- `loadStoreJSON` after `restore_stable_ids_for_load_`: rebuild map
  from `stable_ids_` parallel column.
- `add_bond` / `remove_bond`: same pattern for `bond_sid_to_idx_`.

Cost: O(1) lookup; O(1) insert/erase; ~16 B per live atom + ~16 B per
live bond (typical std::unordered_map). For a 10K-atom system: ~160KB
extra. Acceptable for the H3c-blocking infra.

Phase-0 ships as commit 0 of H3c (before any processor migration). Test:
extend AtomHandle_test with explicit StableId → atomHandleByStableId
round-trip incl. ABA-recycle case (the new map MUST evict the old
binding before the recycled slot gets re-inserted).

### D-H3c.1-R2 — Mutation-pattern classification (per Codex per-processor analysis)

Replace the prior Tier 1/Tier 2 split with mutation-pattern-driven
classification:

| Pattern | Processors |
|---|---|
| **A. Single-pass scalar mutation** (no topology change; just sets atom_type, bond_order, properties) | atomTyper, kekulizer (after Codex correction), residueChecker, hybridisationProcessor |
| **B. Two-pass topology mutation** (decide-then-add or decide-then-remove; no decision dependencies between Pass 2 mutations) | addHydrogenProcessor, assignBondOrderProcessor, buildBondsProcessor, disulfidBondProcessor, HBondProcessor, peptideCapProcessor |
| **C. Three-phase compound mutation** (decide + remove + insert; Pass 2 phases ordered) | DNAMutator |
| **D. Builder pattern** (no v0 input tree to mutate; constructs from scratch) | peptideBuilder, fragmentDB-as-template-source, sdGenerator |
| **E. Pure pointer-key cleanup** (D-H3.8 leaks only; no behavior change) | analyticalSES, atomBijection, geometricProperties, reconstructFragmentProcessor, molecularSimilarity, ringAnalyser, smartsParser, rGroupAssembler |

### D-H3c.1-R2 detail — Pattern A: single-pass

```cpp
// Walk handles, mutate directly via AtomHandle / BondHandle scalar
// setters. No decision set, no re-resolve. Existing v0 mutator
// calls migrate to handle-side variants. HandleKeyLeakGate compliance
// via StableId-keyed containers when storing intermediate state.
```

### D-H3c.1-R2 detail — Pattern B: two-pass

```cpp
struct AddBondDecision {
    StableId  from_sid;   // captured in Pass 1
    StableId  to_sid;
    Bond::Order order;
    // ... other StableId-typed parameters ...
};
std::vector<AddBondDecision> decisions;

// Pass 1 (NO mutation): walk + accumulate
for (AtomHandle a : atomHandles(root_container))
    if (predicate(a)) decisions.push_back({...});

// Pass 1.5 PREVALIDATION (NEW per BLOCKER 2/3): cheap check that all
// captured StableIds still resolve. Hard-fail (throw) here if any
// missing, BEFORE the first mutation. This makes Pass 2 abandonment
// a clean failure rather than a partial-mutation mess.
for (const auto& d : decisions)
{
    if (!store.atomHandleByStableId(d.from_sid).isValid() ||
        !store.atomHandleByStableId(d.to_sid).isValid())
        throw Exception::InvalidIterator(__FILE__, __LINE__,
            "Pass 2 prevalidation: stale decision-set entry");
}

// Pass 2 mutations are now atomic-best-effort: any throw past this
// point leaves the v0 tree in a partial state matching pre-H3c v0
// behavior. Document this as the locked contract.
for (const auto& d : decisions)
{
    AtomHandle a = store.atomHandleByStableId(d.from_sid);
    AtomHandle b = store.atomHandleByStableId(d.to_sid);
    Atom* va = a.getAtom();   // bridge at mutation moment
    Atom* vb = b.getAtom();
    Bond* bond = va->createBond(*vb);
    bond->setOrder(d.order);
}
```

Skip-vs-throw rule (BLOCKER 2 fix): the prevalidation phase makes
"stale decision" a hard error. Pass 2's per-iteration check is just
defense-in-depth against a Pass 2 mutation that invalidated a later
decision (e.g., createBond changes valence so a candidate H site is no
longer needed); those CAN silently skip because they are by-design
benign.

### D-H3c.1-R2 detail — Pattern C: three-phase (DNAMutator)

```cpp
// Phase 1 (decide): identify the residue replacement + all needed
// connections. StableId-keyed decision struct, NO mutation.
// Phase 1.5 (prevalidate): every captured StableId resolves.
// Phase 2 (remove): destroy the old residue's bonds + atoms.
// Phase 3 (insert): materialise the new residue + attach to chain.
//   Phase 3 captures the NEW atoms' StableIds for the bond-attachment
//   step that follows; no forward reference to phase-1 captures whose
//   slots phase 2 just freed.
```

### D-H3c.1-R2 detail — Pattern D: builder

```cpp
// No input tree; the builder creates atoms + bonds from scratch.
// Single-pass; each atom is allocated via `new Atom` (or `new
// PDBAtom`), inserted into the builder's container, then bonded.
// The migration is mostly cosmetic: the v0 builder API works as-is;
// HandleKeyLeakGate would NOT trigger unless the builder includes
// atomHandle.h. Don't opt-in -- keep these as v0-style code.
```

### D-H3c.1-R2 detail — Pattern E: pointer-key cleanup

```cpp
// No mutation pattern change. Just replace HashSet<Atom*> with
// HashSet<MoleculeStore::StableId> or std::unordered_set<AtomHandle>.
// AtomHandle has std::hash<> specialization (H3a.5 / H3a-CR).
// Existing semantic preserved.
```

### D-H3c.4-R2 — Out of scope (revised)

Same as R1 + add explicit note:
- **STRUCTURE/RDF / SAS / SES / Connolly / RS\*** — pure geometric
  read-only, no v0 mutation, no D-H3.8 pointer-key leak. Do NOT opt
  these into atomHandle.h; they stay v0 until H4. Marked "Pattern Z:
  not-migrated" in the inventory.

### D-H3c.5-R2 — Exception + concurrency contract (BLOCKER 3 fix)

- **Pass 2 mutations are NOT transactional.** Any throw past Pass 1.5
  prevalidation leaves the v0 tree in a partial state. This matches
  pre-H3c v0 behavior. Documented at the processor surface as
  "@throws if mutation midway fails; tree may be in partial state".
- **Concurrent processors on the same store: UB.** Inherits the
  existing per-System single-threaded-mutator contract from
  `moleculeStore.h:328` ("bond mutation on the same per-System store
  from multiple threads is undefined behavior").

### D-H3c.3-R2 — Updated implementation order

After R2 design lock:

0. **Phase 0** (D-H3c.0): stable_id reverse maps + atomHandleByStableId
   / bondHandleByStableId on MoleculeStore. Includes test for ABA
   recycle correctness. Zero callers in this commit.

Pattern A (single-pass scalar):
1. atomTyper (D-H3.8 pointer-key cleanup + handle adoption)
2. kekulizer (D-H3.8 cleanup + BondHandle::setOrder)
3. residueChecker (read-only handle adoption)
4. hybridisationProcessor

Pattern B (two-pass topology):
5. addHydrogenProcessor (T1 prototype)
6. assignBondOrderProcessor
7. buildBondsProcessor
8. disulfidBondProcessor
9. HBondProcessor
10. peptideCapProcessor

Pattern C (three-phase):
11. DNAMutator

Pattern E (pointer-key cleanup, batched by file):
12-18. analyticalSES, atomBijection, geometricProperties,
       reconstructFragmentProcessor, molecularSimilarity,
       ringAnalyser, smartsParser, rGroupAssembler

Close-reviews:
- After step 4 (Pattern A complete): mini close-review.
- After step 11 (Pattern A+B+C complete): cluster close-review.
- After step 18 (all migrated): final H3c close-review.

Updated commit estimate: 35-45 atomic commits, pessimistic upper bound 55.

### D-H3c.6-R2 — Closed Q1-Q7

Q1 (DNAMutator): three-phase per Pattern C above.
Q2 (atomHandleByStableId cost): no map exists; D-H3c.0 adds one.
   Phase-0 commit. Test ABA recycle.
Q3 (HandleKeyLeakGate): keep global-by-include (NIT 12 confirms).
Q4 (Pass 2 exception safety): best-effort, no rollback,
   prevalidation in Pass 1.5 makes stale-decision a hard error
   BEFORE first mutation.
Q5 (kekulizer): one-pass with D-H3.8 cleanup (Pattern A).
Q6 (fragmentDB): keep v0 template format; define handle bridge at
   template materialization + bond-creation boundaries.
Q7 (atomTyper): one-pass (Pattern A).

### NEW open question for H3c-DR2

Q8: Pattern D builder (peptideBuilder / sdGenerator / fragmentDB as
template source) -- the design says "don't opt-in; stay v0". Is that
sustainable through H4 when v0 retires entirely? Or do these need a
forward-stable "handle-side builder" surface eventually? Answer for
H3c: not-migrated-in-H3c; revisit when H4 plan crystallizes.

