# KERNEL v2.0 — Locked Design Decisions

**Status:** NORMATIVE. **Authored:** 2026-05-17. **Authoritative for K0.**
**Branch:** `v2.0` (on top of `origin/v1.7-modernization`).
**Companion docs:**
- [`CORE-MOLECULESTORE-RESEARCH.md`](CORE-MOLECULESTORE-RESEARCH.md) — research / context (historical).
- [`../KERNELV2.md`](../KERNELV2.md) — superseded for K0 by this doc (the parallel-model framing is no longer the plan; this doc commits to the store-as-source-of-truth replacement instead).

This document records the design contract for the BALL 2.0 KERNEL rewrite,
core library only (KERNEL + CONCEPT + minimum support: CONFIG, COMMON,
DATATYPE, MATHS, SYSTEM). It is the input to `/gsd-plan-phase` for the
K0 implementation phase.

When this document and any other document disagree on a K0 contract,
this document wins.

---

## 1. Framing — what we are building

**MoleculeStore is the ground truth.** Atom / Bond / AtomContainer /
Molecule / Residue / Chain / Fragment / SecondaryStructure / Protein /
Nucleotide / NucleicAcid / PDBAtom / System are **handles** layered on
top of a per-System MoleculeStore that owns all per-atom data in
struct-of-arrays columns.

The Composite class is **deleted entirely**. PropertyManager and
Selectable are **deleted entirely**. Their functionality is absorbed
into MoleculeStore (selection bitmap; named-properties map with
hot-promoted columns; modification + selection generation counters).

The on-wire persistence format **breaks at v2.0**. A new
MoleculeStore-native binary format ships; v1.x files cannot be loaded
directly; a one-shot `v1→v2` converter ships separately.

---

## 2. Locked decisions

| # | Decision | Locked value |
|---|---|---|
| D1 | Composite framing | Store-as-ground-truth (NOT parallel-model from KERNELV2.md) |
| D2 | Composite scope | **Full removal.** Class deleted. MoleculeStore owns hierarchy via CSR arrays |
| D3 | PropertyManager scope | **Full removal.** Named properties live in MoleculeStore (hot-promoted to columns) |
| D4 | Selectable scope | **Full removal.** Selection is a store-owned bitmap |
| D5 | Store ownership | **Per-System.** `System` holds `std::unique_ptr<MoleculeStore>` |
| D6 | `Atom*` identity | **Preserved.** Handles are heap-allocated objects (~16-32 B). Pointer comparison, hashing, `std::set<Atom*>` work unchanged |
| D7 | Reference-return getters | **Preserved.** `Vector3& getPosition()` returns `&store->positions[idx_]`. Invalidated only on store resize/compact (documented contract; debug-asserted via generation check) |
| D8 | Stable-handle escape hatch | **Required.** Add `Vector3* getPositionPtr()` (and equivalents) for callers that need rebind-on-resize semantics. Re-resolves the column row each access |
| D9 | `Atom::interactions` public field | **Removed.** Sparse store column; getter access only |
| D10 | Source-compat target | **Tests-only.** The post-deletion test subset (~49 tests, exact count from §6 verification) stays green with zero edits to test sources. Downstream modules (MOLMEC, STRUCTURE, etc.) are out of scope |
| D11 | Compiled selection | **In K0.** Variant-based cached AST; Selector + Expression rewritten once |
| D12 | Persistence format | **v2 break.** New MoleculeStore-native binary format; one-shot `v1→v2` converter ships separately as a CLI tool |
| D13 | Memory budget gate | **HARD ≤160 B/atom total** (handle + store columns + bond CSR amortised), measured on 40k-atom benchmark |
| D14 | Perf budget gate | **Iteration within 2× of baseline** (40k-atom System ≤0.13 s; baseline 0.067 s) |

---

## 3. Surface to be deleted

| Class | Header | Source | Tests |
|---|---|---|---|
| `Composite` | `include/BALL/CONCEPT/composite.h` (2198 LOC) + `.iC` (234 LOC) | `source/CONCEPT/composite.C` (1762 LOC) | `Composite_test1..6` |
| `PropertyManager` (named-properties + indexed bitset) | `include/BALL/CONCEPT/property.h` + `.iC` | `source/CONCEPT/property.C` | `PropertyManager_test`, `NamedProperty_test` |
| `Selectable` | `include/BALL/CONCEPT/selectable.h` + `.iC` | `source/CONCEPT/selectable.C` | `Selectable_test` |
| `PersistentObject` virtuals on Atom/Bond/Composite/AtomContainer/System | various | various | `PersistentObject_test`, `PersistenceManager_test`, `TextPersistenceManager_test` |
| Composite iterator-traits machinery | `include/BALL/KERNEL/atomContainerIterator.h` etc. | (header-only macros) | `Composite{,Composite,Childcomposite,Ancestor}IteratorTraits_test` |

Plus all `friend class` declarations in KERNEL+CONCEPT (9 sites) that
reach into the deleted classes.

---

## 4. Surface to be added

| Class | Header | Source | New tests |
|---|---|---|---|
| `MoleculeStore` | `include/BALL/KERNEL/moleculeStore.h` | `source/KERNEL/moleculeStore.C` | `MoleculeStore_test.C`, `MoleculeStoreBond_test.C` |
| `CompiledExpression` (variant AST + caching) | `include/BALL/KERNEL/compiledExpression.h` | `source/KERNEL/compiledExpression.C` | `CompiledExpression_test.C` |
| New persistence (binary v2 format) | `include/BALL/KERNEL/storeFormat.h` | `source/KERNEL/storeFormat.C` | `StoreFormat_test.C` |
| v1→v2 file converter (CLI) | `source/APPLICATIONS/TOOLS/ball-convert-v1-v2.C` (deferred to a K0.8 sub-task; ships with v2.0 but not gating K0 itself) | — | smoke test against a curated v1.x corpus |

Atom, Bond, AtomContainer, System and the rest of KERNEL keep their
header paths but their declarations are rewritten:

- `Atom` becomes a heap-allocated handle: `{ MoleculeStore* store_;
  uint32_t idx_; uint64_t generation_; }`. No more typed payload
  members; getters/setters in `atom.iC` dispatch through `store_`.
- `Bond` same shape, indexes into `store_->bonds`.
- `AtomContainer`, `Molecule`, `Residue`, `Chain`, `Fragment`,
  `SecondaryStructure`, `Protein`, `Nucleotide`, `NucleicAcid`,
  `PDBAtom`, `System` become container handles. Their hierarchy lives
  in store CSR arrays.

---

## 5. K0 phasing — what gets built when

This supersedes the K0.1–K0.8 sketch in `CORE-MOLECULESTORE-RESEARCH.md`
§5. The shape is similar but the scope (full removal + compiled
selection + v2 format) is larger.

| Step | Scope | Weeks (1.0 FTE) | Gate |
|---|---|---|---|
| K0.1 | Build-fence: `BALL_CORE_ONLY=ON` CMake option; CI matrix entry; verify baseline tests green on the as-is tree | 1-2 | Configure clean on macOS/Linux; 49-test baseline runs |
| K0.2 | `MoleculeStore` skeleton: columns + CSR + generation + selection bitmap + custom_props map. Standalone, no facade yet | 4-6 | `MoleculeStore_test.C` + `MoleculeStoreBond_test.C` green |
| K0.3 | Atom + Bond facade rewrite: payload shrinks to (store, idx, gen); `atom.iC` getters/setters dispatch through store; reference-return contract enforced | 6-8 | Atom_test1/2 + Bond_test + AtomVector_test + AtomBondIteratorTraits_test + AtomIterator_test green |
| K0.4 | Container facades + hierarchy: AtomContainer, Molecule, Residue, Chain, Fragment, SecondaryStructure, Protein, Nucleotide, NucleicAcid, PDBAtom, System rewritten on store CSR. New iterator templates (replace `BALL_KERNEL_DEFINE_ITERATOR_CREATORS` and `BALL_DECLARE_STD_ITERATOR_WRAPPER` with store-walking equivalents) | 6-8 | AtomContainer_test1/2 + Fragment_test + Chain_test + Molecule_test + System_test + Protein_test + Residue_test1/2 + PDBAtom_test + NucleicAcid_test + Nucleotide_test + SecondaryStructure_test green |
| K0.5 | Compiled selection: variant-based AST + cache + parser re-entrancy fix. Expression / ExpressionTree / ExpressionParser / Selector rewritten | 3-4 | Expression_test + ExpressionTree_test + ExpressionParser_test + ExpressionPredicate_test + KernelPredicate_test + Selector_test + StandardPredicates_test1..4 + GlobalKernel_test green; Selector_test ≥10× faster than baseline |
| K0.6 | Persistence v2: new binary format writer/reader; replace persistent virtuals on every handle | 3-4 | New `StoreFormat_test.C` green; round-trip identity on a curated corpus; v1→v2 converter sketch exists |
| K0.7 | Memory + perf benchmark + budget verification: instrument 40k-atom workload, prove ≤160 B/atom + iteration ≤2× baseline | 1 | Both gates met; results recorded in `RELEASE-NOTES-v2.0.md` draft |
| K0.8 | Cleanup + compat audit + release-notes draft: friend audit closure, public-API drift sweep, MILESTONE-CONTEXT-v2.0.md authored | 1-2 | All gates met; PR ready; planning artefacts complete |

**Total: 25-35 weeks at 1.0 FTE.** ~2× my earlier estimate (which
assumed parallel-model). Honest: at sub-1.0 FTE this stretches to
8-12 calendar months.

---

## 6. Test-deletion list — verified

Per D10 the K0 green-bar is "the 65 baseline tests minus those that
exercise deleted classes." Verified 2026-05-17 by inspecting each
candidate's actual `#include` set + transitive dependencies (item V1
of §7 is now closed; see §7 below).

**16 definite deletes** (test source `#include`s a header for a
deleted class):

| Test | Pulls | Reason |
|---|---|---|
| `Composite_test1`..`Composite_test6` | `composite.h` (all 6) | D2 |
| `CompositeIteratorTraits_test` | `composite.h` + `iterator.h` | D2 |
| `CompositeAncestorIteratorTraits_test` | `composite.h` + `iterator.h` | D2 |
| `CompositeChildcompositeIteratorTraits_test` | `composite.h` + `iterator.h` | D2 |
| `CompositeCompositeIteratorTraits_test` | `composite.h` | D2 |
| `PropertyManager_test` | `property.h` | D3 |
| `NamedProperty_test` | `property.h` | D3 |
| `Selectable_test` | `selectable.h` | D4 |
| `PersistentObject_test` | `persistentObject.h` + `persistenceManager.h` | D12 (no PersistentObject base; new v2 format owns its own test) |
| `PersistenceManager_test` | `persistenceManager.h` + `atom.h` (Atom persistence virtuals deleted) | D12 |
| `TextPersistenceManager_test` | `textPersistenceManager.h` + `composite.h` + `bond.h` | D2 + D12 |

**Definitely stay** (verified Composite-/Property-/Selectable-/
PersistentObject-independent by transitive include audit):

| Test | Why it stays |
|---|---|
| `BaseIterator_test` | Builds its own `VectorIteratorTraits_<DataType>` over `std::vector`. Tests the iterator-template API only. |
| `ConstForwardIterator_test`, `ForwardIterator_test` | Same fixture pattern as BaseIterator_test. |
| `ConstBidirectionalIterator_test`, `BidirectionalIterator_test` | Same pattern. |
| `ConstRandomAccessIterator_test`, `RandomAccessIterator_test` | Same pattern. |
| `Object_test`, `Factory_test`, `AutoDeletable_test`, `LogStream_test`, `Embeddable_test` | Each pulls only its own header; none reference Composite/Property/Selectable in implementation (the one Composite mention in `autoDeletable.h` is a doc-comment only). |

**Rewrite-or-defer (1 test): `DefaultProcessors_test`** — Uses
`Composite::apply(processor)` against `System`. With Composite removed
the visitor pattern needs a new dispatch (likely `MoleculeStore::apply()`
+ container handle delegation in K0.4). The test also pulls FORMAT
(`HINFile.h`) and STRUCTURE (`defaultProcessors.h`), so it cannot
build in core-only anyway. K0 disposition: **defer to when STRUCTURE
is re-enabled.** Removed from the K0 green-bar.

**K0 green-bar target: 48 tests** (65 − 16 confirmed deletes − 1 deferred).

This number is locked. Any further reductions or rewrites land as
explicit follow-on plans.

---

## 7. Outstanding verification work (before K0.1 plan promotes)

These items must close before `/gsd-plan-phase` is invoked on K0:

- **V1 — Iterator-trait test fixtures.** ✅ **CLOSED 2026-05-17.** All
  7 generic iterator-trait tests, all 5 specific Composite
  iterator-trait tests, `Embeddable_test`, `DefaultProcessors_test`,
  `Object_test`, `Factory_test`, `AutoDeletable_test`, `LogStream_test`
  audited. Result: see §6. K0 green-bar = 48 tests (16 deletes +
  1 deferred).

- **V2 — Friend-class audit list.** Grep KERNEL+CONCEPT for the 9
  `friend class` sites. For each: which class is the friend? What
  does it reach into? After Composite removal, does the friend
  declaration still mean anything? Output: per-site disposition table.

- **V3 — `dynamic_cast<Composite*>` callers in KERNEL+CONCEPT.** The
  research-doc grep found 96 dynamic_cast sites on KERNEL types; many
  cast through Composite. For each KERNEL+CONCEPT site: rewrite to
  the store-backed dispatch. Output: per-site rewrite table. (Sites
  in MOLMEC/STRUCTURE/etc. are out of scope.)

- **V4 — PropertyManager hot-promotion threshold.** D3 says
  "hot-promoted to columns" but doesn't specify the threshold. The
  research doc proposed >5% atom coverage. Verify against the BALL
  test corpus: which named properties actually appear, and at what
  coverage? Output: column allocation list + fallback policy.

These four are small (~1-2 days total) and run in parallel with K0.1.

---

## 8. What this contract does NOT decide

- **MOLMEC / STRUCTURE / FORMAT / VIEW etc. re-enablement plan.**
  Owned by post-K0 phases. Each module's rewrite cost is paid then,
  not now.
- **PyBALL bindings.** v2.1 work; depends on K0 surface being stable.
- **VIEW selection bidirectional sync.** K4 in KERNELV2.md; needs
  VIEW re-enabled; post-K0.
- **GPU-resident molecular data.** v3.0+ horizon.
- **Multi-trajectory MD storage.** v2.x or later; not K0.
- **REST API / Read the Docs portal.** v2.2 / v2.3 phases.

---

## 9. Provenance

User-locked decisions, 2026-05-17 interactive Q&A session in the v2.0
branch local development environment. Recorded answers:

- Round 1 (Composite scope / pointer identity / reference contract /
  source-compat scope): Q1=Full removal, Q2=Preserve Atom*, Q3=Preserve
  reference + stable-handle accessor, Q4=Tests-only.
- Round 2 (Test fate / adjacent bases / store ownership / perf budget):
  Q5=Delete affected tests, Q6=Remove PropertyManager + Selectable,
  Q7=Per-System store, Q8=Within 2× baseline.
- Round 3 (Memory budget / interactions field / Selector in K0 /
  persistence): Q9=≤160 B/atom hard, Q10=Remove field via store column,
  Q11=Compiled selection in K0, Q12=Break format.

Earlier opinion sources superseded by this document:
- `CORE-MOLECULESTORE-RESEARCH.md` §7 open-questions — now resolved.
- `KERNELV2.md` §2 "Composite remains the source of truth" — now
  superseded by D2.
- `KERNELV2.md` Phase K5 ("Store-as-source-of-truth — DEFERRED to
  v3.0") — now pulled into K0.
- `KERNELV2.md` §2 Goal 5 "no file-format break" — now superseded by
  D12.
- `KERNELV2.md` §5.1 Mutation Contract Table — still relevant but
  smaller: with Composite deleted, the table covers only `markDirty`
  surface from non-Atom-handle callers (much narrower than the
  parallel-model 95% target).

---

*End of KERNEL-V2-DECISIONS.md. This is the K0 contract.*
