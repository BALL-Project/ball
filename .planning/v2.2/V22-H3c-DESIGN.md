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
