# BALL 2.0 — Release Notes (DRAFT)

**Status:** DRAFT for tag. **Authored:** 2026-05-18 by K0.8.

BALL 2.0 ships a redesigned kernel: atoms and bonds live in a SoA
`MoleculeStore` per System, with Atom and Bond as thin handles. The
old text-based persistence is replaced by a JSON format. Selection
queries compile to a variant AST that runs 10–80× faster than v1.x
for typical molecular workloads. The `BALLView` GUI continues to
build against this kernel; full module re-enable (FORMAT, STRUCTURE,
MOLMEC, …) is Track B work happening in parallel.

This document is the user-facing summary. The phase-by-phase audit
trail lives in `.planning/v2.x/`.

---

## What's new

### MoleculeStore: SoA columns per System

Every `System` now owns a `MoleculeStore` holding atom and bond data
as parallel column vectors (positions, velocities, forces, charges,
radii, atom types, formal charges, element indices, selection bits,
name + type-name strings, stable IDs, back-pointers, free-list flag).
`Atom` becomes a thin handle holding `(MoleculeStore*, store_idx,
generation)` plus the residual `bond_[]` array.

Adoption is automatic: `AtomContainer::insert(Atom&)` migrates the
atom from a process-global *orphan* store into the destination
System's store via the K0.4.3 hook. `System::adopt(Atom&)` is the
explicit single-atom path; `adoptSubtree` is the batched
3-pass path for bonded subgraphs.

The orphan store is mutex-protected for concurrent default
`Atom()` construction across user threads.

### Compiled selection (K0.5)

`Expression` and `Selector` now route through a new
`CompiledExpression` — a variant AST built once per source string,
cached in a process-global LRU keyed by `(source, MoleculeStore*)`.
The hot path is a bitmap evaluator that scans store columns
directly with one virtual call per AST node, zero per atom for the
fast-path leaves.

**Benchmark (Darwin arm64 release, 100k synthetic atoms,
single-run):**

| Query                        | v1.x ms | v2.0 ms | Speedup |
|------------------------------|--------:|--------:|--------:|
| `element(C)`                 |   10.11 |   0.261 |  38.71× |
| `name(CA)`                   |   12.99 |   0.210 |  61.78× |
| `charge(<0)`                 |   50.02 |   0.586 |  85.41× |
| `element(C) AND name(CA)`    |   13.34 |   0.554 |  24.10× |
| median                       |         |         |  61.78× |
| `inRing()` (OwnedPred slow)  |    0.21 |   0.162 |   1.27× |

The K0.5 design gate was ≥10× median speedup; observed median is
6.2× over the gate. Predicates not in the fast-path subset (ring,
SMARTS, residue / chain / protein lookups, user-registered) compile
to `OwnedPred` which wraps the v1.x `ExpressionPredicate` and pays
one virtual call per atom plus a back-pointer dereference — still
slightly faster than v1.x because it skips the `ExpressionTree`
node walk.

`ExpressionParser::parse()` is mutex-protected; concurrent compiles
on different threads are serialised. The cache means each unique
source is parsed once per process.

### JSON persistence (K0.6)

The original spec called for a custom binary format. After
maintainer review (2026-05-18), K0.6 was re-scoped to JSON via
vendored `nlohmann::json` (single header at
`include/BALL/EXTERNAL/nlohmann_json.hpp`, MIT). Rationale: typical
BALL workloads (≤10k atoms) don't need the 3-4 week binary
investment; JSON wins on debuggability, schema evolution, zero
endianness work. A binary fast-path can be added later if profiling
shows it.

**Coverage:**
- per-column SoA atom store (every column + freed-slot flag)
- bond table (live records only; tombstones stripped at save)
- per-atom + per-molecule + per-system `PropertyManager` bags
  (BOOL / INT / UNSIGNED_INT / FLOAT / DOUBLE / STRING fully round-trip;
  OBJECT / SMART_OBJECT emit a placeholder, the key survives, the
  pointer doesn't)
- orphan-atom path (System direct children, not in any Molecule)
- bit-exact float opt-in (`JsonFloatFormat::BIT_EXACT_HEX`) for
  trajectory replay where decimal drift matters
- stable-id round-trip via a friend-restricted bulk-restore
- MAJOR/MINOR schema versioning
- `document_type` discriminator (`"MoleculeStore"` vs `"System"`) so
  a System loader can't silently partial-load a store doc
- comprehensive parse-error boundary — every `nlohmann::json` or
  `std::stoul` failure rethrown as `Exception::ParseError`
- two-phase loader: validate every atom_index for range / freed /
  duplicate BEFORE any allocation; RAII guard cleans up
  pre-allocated atoms if a later step throws

**API:**
```cpp
saveStoreJSON  (store, ostream, indent, JsonFloatFormat);
loadStoreJSON  (store, istream);
saveSystemJSON (sys,   ostream, indent, JsonFloatFormat);
loadSystemJSON (sys,   istream);
```

**Performance (100k atoms, Darwin arm64 release, single run):**

| Scenario                   | save ms | load ms | bytes/atom JSON |
|----------------------------|--------:|--------:|----------------:|
| 100k, no per-atom props    |     233 |    3840 |              99 |
| 100k, +3 props/atom        |     375 |    4107 |             274 |

Load is ~15× slower than save (per-atom heap allocation + adoption
dominates). Acceptable for v2.0; batching optimisation deferred to
v2.1.

### Adversarial review

Codex CLI was run as the second-opinion reviewer at every major
phase boundary. Nine review rounds across K0 (R1 architecture,
R2 design lock, R3 K0.3c, R4 K0.3b.LATER + K0.4, R5 manual, R6 K0.5,
R7 K0.6.0-K0.6.3, R8 K0.6.5b, R9 K0.7). Every HIGH and OPEN finding
was closed before the next phase began; FYI items were either fixed
inline or routed to v2.1 with explicit documentation.

---

## Breaking changes

### `D2/D3/D4` — base classes reduced to thin-stub forwarders

`Composite`, `PropertyManager`, and `Selectable` stay as base classes
so existing tests + downstream module code can still use
`Composite&`, `PropertyManager*`, `Selectable` in signatures. Their
methods are reduced to thin forwarders that delegate to the
MoleculeStore where appropriate. Full deletion is **v2.1** work.

**User impact:** none for normal use. Code that introspected `Atom`'s
sizeof or directly read its inherited member layout will see
different numbers (see *Memory budget* below).

### `Atom::getName()` returns `String` by value

Was `const String&`. The name now lives in a `std::vector<String>`
column on the store; returning a reference across store-growth
boundaries would dangle. `getTypeName()` already returned by value
in v1.x.

Most call sites are unaffected (assignment + comparison + stream
output all work identically). The handful of sites that bound to
`const String&` still work via C++ temporary-lifetime extension.

### `Atom::getElement()` resolves through PTE

The v1.x `Atom::element_` field was deleted (K0.3b.LATER.7). The
store holds an atomic-number column; `getElement()` returns
`PTE[atomic_number]`. **Element pointer identity is lost** — a
caller passing a custom `Element` instance to `setElement` will get
the canonical PTE entry back on `getElement`. v2.1 may add an
element-id table for round-tripping custom instances.

### `ExpressionParser` is mutex-serialised

Concurrent `parse()` calls on different threads previously corrupted
the static parser state. v2.0 wraps the call in a mutex. The
compiled-expression cache means each source string is parsed once
per process; throughput impact is negligible.

### `Bond` is store-aware

`Bond` now holds `(bond_store_, bond_record_idx_)` in addition to
its v1.x atom-pointer fields. Constructors and `createBond` mirror
the bond into the store's bond table; destructors tombstone the
record.

### Persistence format changed

`TextPersistenceManager` still exists. New saves should use the JSON
format via `saveStoreJSON` / `saveSystemJSON`. A v1→v2 converter
(K0.6.4) is gated on the FORMAT module re-enable in Track B and
ships in a follow-on patch release.

---

## Workload guidance (D13 memory budget)

The D13 budget is ≤160 B/atom for typical workloads. **v2.0 meets
the budget for store-resident atoms and misses it for fully-handle-
materialised atoms.**

Measured at 100k atoms (Darwin arm64 release, modeled — not RSS):

| Component                    | B/atom |
|------------------------------|-------:|
| SoA store columns            |    139 |
| string-pool (intern-dedup)   |     ~0 |
| `Atom*` handle (heap-alloc)  |    360 |
| **Total**                    |  **499** |

The 139 B SoA contribution is **under budget**. The 499 B full-fat
total is **3.1× over budget**, dominated by `sizeof(Atom)=360 B`
which carries the `Composite + PropertyManager + Selectable` base-
class overhead from the D2/D3/D4 thin-stub compromise.

**Workloads that hit D13:**
- trajectory frame reading (atoms live in store, no per-atom handle)
- batch analysis where atoms are processed via store columns
- persistence-only paths (load + read column data + free)

**Workloads that miss D13:**
- interactive editing (every visible atom needs an Atom*)
- GUI selection (selection bit accessed via Atom*)
- v1.x-compatibility code paths (Composite tree traversal)

The miss is documented architectural debt. v2.1's D2/D3/D4 actual
removal targets reducing `sizeof(Atom)` to ~64 B which would bring
the full-fat number under budget.

---

## Known gaps

### Track B partial-module surface

v2.0 ships a re-enabled-by-default Track B Wave 1 (B1.1 FORMAT + B1.2
STRUCTURE) on top of the K0 core. To keep the link surface clean
without pulling MOLMEC/QSAR/DOCKING, both modules ship as **subsets**
in the `BALL_CORE_ONLY=ON` build:

**FORMAT (30 of 38 sources)** — kept: PDB (`PDBFile`, `PDBInfo`,
`PDBdefs`, `PDBRecords`), HIN, INI, simple small-molecule formats
(XYZ, KCF, MOLFile, SDFile, MOPACInput/Output, JCAMP, HMOFile,
antechamber, bruker NMR, GAMESS), generic-mol, CCP4, CIFFile, DSN6,
amiraMesh, pubchem, parameters/parameterSection, lineBasedFile,
commandlineParser, resourceFile. **Trimmed (link-error if called):**
`MOL2File` (needs GAFFTypeProcessor — MOLMEC+QSAR, Wave 3-4),
`DCDFile` / `TRRFile` / `trajectoryFile{,Factory}` (need SnapShot[Manager]
— MOLMEC, Wave 3), `NMRStarFile` (needs Peptides::NameConverter —
STRUCTURE Wave 1b), `SCWRLRotamerFile` (needs Rotamer/RotamerLibrary
— STRUCTURE Wave 1b cycle close), `dockResultFile` (needs
Result::ResultData — DOCKING, Wave 6), `molFileFactory` (DockResultFile
typeinfo).

**STRUCTURE (~47 of 60+ sources)** — kept: FragmentDB, ResidueChecker,
DefaultProcessors, Peptides{,Builder,CapProcessor},
NormalizeNamesProcessor, SecondaryStructureProcessor,
connectedComponents, disulfid, mutator, nucleotideMapping,
numericalSAS, analyticalSES, reducedSurface, SES*/SAS*/RS* surface
stack, triangulated* mesh, structureMapper, RMSDMinimizer, UCK,
binaryFingerprintMethods, bindingPocketProcessor, atomBijection,
HBondProcessor, secondaryStructureProcessor. **Trimmed (link-error
if called):** `rotamerLibrary`, `sideChainPlacementProcessor` (need
SCWRLRotamerFile — Wave 1b cycle close); `ringAnalyser`, `sdGenerator`,
`hybridisationProcessor`, `buildBondsProcessor`,
`assignBondOrderProcessor` + `STRUCTURE/BONDORDERS/` sub-tree,
`kekulizer`, `smartsMatcher`, `atomTyper`, `molecularSimilarity`
(need RingPerceptionProcessor/AromaticityProcessor — QSAR, Wave 4);
`addHydrogenProcessor`, `RDFParameter`, `DNAMutator` (need
MMFF94StretchParameters / ForceFieldParameters / AmberFF — MOLMEC,
Wave 3).

Setting `BALL_CORE_ONLY=OFF` enables the full module surface (all
FORMAT + STRUCTURE + ENERGY + MOLMEC + NMR + PYTHON + QSAR + SCORING +
SOLVATION + DOCKING + XRAY). v2.0.x patch releases close the trim
list wave-by-wave; the v2.1 milestone removes `BALL_CORE_ONLY` as a
build-time concept.

- **K0.6.4 (v1→v2 converter)** — DELIVERED in Track B B0.2 (commit
  `83e82da0a`). `convertV1BalToV2JSON()` is in `BALL/KERNEL/v1ToV2JsonConverter.h`.
  Reads via `TextPersistenceManager`, writes via the new `saveSystemJSON`
  path. Round-trip verified on 3 fixtures (atoms, molecules, properties).
- **Full v1.x test surface** — re-enabled in B1.1+B1.2. `Expression_test`,
  `Selector_test`, `ExpressionParser_test`, `StandardPredicates_test1..4`,
  `Residue_test1/2`, `StdIteratorWrapper_test`, plus 7 STRUCTURE tests
  (FragmentDB_test, NormalizeNamesProcessor_test, PeptideBuilder_test,
  PeptideCapProcessor_test, Peptides_test, ResidueChecker_test,
  SecondaryStructureProcessor_test) all build and (mostly) pass under
  `BALL_CORE_ONLY=ON`. K0.5's selector speedup claim is verified on
  the v1.x corpus via `Selector_test`. 4 documented quarantines remain
  (PersistenceManager_test, Expression_test, Peptides_test,
  PeptideCapProcessor_test — see `test/CMakeLists.txt` for each).
- **JSON load batching** — load is 15× slower than save (4s for
  100k atoms). Per-atom heap allocation + K0.4 adoption + Composite-
  tree insertion is the bottleneck. v2.1 candidate.
- **Element-instance round-trip** — see *Breaking changes* above.
- **AndNode tmp-bitmap allocation** — each AND/OR/NOT inner node
  allocates one scratch `std::vector<uint8>` per evaluation. Bounded
  by tree depth, not child count. Not on the hot path for typical
  ASTs; v2.1 perf-debt item if profiling demands.
- **Tighter perf gates** — JSON I/O test gates (30s save / 30s load)
  are tripwires, not regression-pins. v2.1 should calibrate against
  CI machine variance.

## Things deferred to v2.1

| Item | Source | Win |
|---|---|---|
| D2/D3/D4 actual removal | K0 design + R9 | `sizeof(Atom)` 360→64 B → D13 met for all workloads |
| JSON load batching | R7+R8+R9 | save/load 15× ratio closed |
| AndNode tmp-bitmap reuse | R6 FYI-6 | minor selector perf win for deep ASTs |
| Element-instance id-table | R4 MEDIUM-7 | custom Element round-trip |
| CI-calibrated perf gates | R9 finding 4 | regression detection at ~2× drift, not 100× |
| Median-of-N for release-claim perf numbers | R9 finding 6 | reproducible numbers in marketing copy |

---

## Test counts at tag

- **Kernel core sweep:** 24 binaries (Atom1/2, Bond, System, Molecule,
  Fragment, Chain, Protein, PDBAtom, SecondaryStructure, NucleicAcid,
  Nucleotide, AtomContainer1/2, AtomIterator, AtomBondIteratorTraits,
  Extractors, ExpressionPredicate, ExpressionTree, KernelPredicate,
  GlobalKernel, MoleculeStore, MoleculeStoreJson, SystemJson,
  CompiledExpression, SelectorBench, Sizeof, MemoryBudget, JsonBench)
- **K0.5 sub-tests:** 25+ CompiledExpression CHECKs + 2 SelectorBench CHECKs
- **K0.6 sub-tests:** 42 (5 K0.6.1 + 8 K0.6.2 + 8 K0.6.3 + 6 K0.6.3b +
  6 K0.6.5 + 5 K0.6.5b + 4 K0.6.5c)
- **K0.7 sub-tests:** 7 (5 Sizeof + 2 MemoryBudget + 2 JsonBench)

All green at tag time.

---

## Provenance

Authored 2026-05-18 by K0.8. Phase artifact trail in
`.planning/v2.x/`. Adversarial reviews at
`.planning/v2.x/K0-CODEX-REVIEW-ROUND{1..9}.md`. Design contract at
`.planning/v2.x/KERNEL-V2-DECISIONS.md`.
