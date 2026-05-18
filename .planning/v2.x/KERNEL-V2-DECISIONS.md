# KERNEL v2.0 — Locked Design Decisions

**Status:** NORMATIVE. **Authored:** 2026-05-17. **Last amended:** 2026-05-17 (post Codex adversarial review). **Authoritative for K0.**
**Branch:** `v2.0` (on top of `origin/v1.7-modernization`).
**Companion docs:**
- [`CORE-MOLECULESTORE-RESEARCH.md`](CORE-MOLECULESTORE-RESEARCH.md) — research / context (historical).
- [`K0.3-FINDINGS.md`](K0.3-FINDINGS.md) — Atom2 prototype validation + Path-1/Path-2 migration shape.
- [`K0-CODEX-REVIEW.md`](K0-CODEX-REVIEW.md) — adversarial review findings that forced the amendments below.
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

**Amended 2026-05-17 (Codex review):** Composite, PropertyManager, and
Selectable are NOT deleted entirely in K0 — they are reduced to **thin
stubs** that exist for type compatibility with the existing test API
surface (`AtomContainer::insertBefore(Atom&, Composite&)` etc. — see
§6.5). Their *payload* (children pointers, tree machinery, named-
property bag, selection bit, modification stamps) moves to
MoleculeStore. The classes themselves stay as marker types whose
operations are forwarded to the store. Full class deletion is
deferred to v2.1 when downstream modules can be rewritten to use the
new API. This is the only way to satisfy D10 (tests-only source-compat)
without editing test sources.

The on-wire persistence format **breaks at v2.0**. A new
MoleculeStore-native binary format ships; v1.x files cannot be loaded
directly; a one-shot `v1→v2` converter ships separately. K0.6 is split
into a design subphase (K0.6a, spec the format + invariants + corpus)
and an implementation subphase (K0.6b).

---

## 2. Locked decisions

| # | Decision | Locked value |
|---|---|---|
| D1 | Composite framing | Store-as-ground-truth (NOT parallel-model from KERNELV2.md) |
| D2 | Composite scope | **Reduced to thin stub** (was: "Full removal"). Composite remains as a marker base class that AtomContainer/Molecule/Residue/etc. inherit so existing test API surface compiles. Composite's *data* (children/parent/sibling pointers, modification + selection stamps, properties bitset) moves to MoleculeStore CSR + per-store metadata. Composite methods become thin forwarders to the store. Full class deletion deferred to v2.1. [Amended 2026-05-17 per Codex review BLOCKER-1] |
| D3 | PropertyManager scope | **Reduced to thin proxy** (was: "Full removal"). PropertyManager stays as a base class so AtomContainer/Bond/Atom can inherit it; methods forward to a per-store `properties_` table keyed by `(stable_id, name)`. Hot-promoted properties become typed store columns as an optimisation. Full deletion deferred to v2.1. [Amended 2026-05-17 per Codex review BLOCKER-1 + MEDIUM] |
| D4 | Selectable scope | **Reduced to thin proxy** (was: "Full removal"). Selectable stays as a base class so subclasses inherit `isSelected()`/`select()`/`deselect()`; methods forward to the store's selection bitmap. Full deletion deferred to v2.1. [Amended 2026-05-17 per Codex review BLOCKER-1] |
| D5 | Store ownership | **Per-System.** `System` holds `std::unique_ptr<MoleculeStore>` |
| D6 | `Atom*` identity | **Preserved.** Handles are heap-allocated objects (~24-32 B with vtable + inherited stub bases). Pointer comparison, hashing, `std::set<Atom*>` work unchanged. The store's `back_ptr_` column (8 B/atom) is part of the per-atom cost. [Amended 2026-05-17 per Codex review BLOCKER-2 — heap + back-pointer overhead now explicit] |
| D7 | Reference-return getters | **Preserved with a hard live-reference contract.** `Vector3& getPosition()` returns `&store->positions[idx_]`. Reference is valid only while no store-resizing operation (`allocate_atom` beyond capacity, `reserve` growing capacity, `compact`, `insert`/`remove`) is called on the same store. Generation check on the *handle* (D8) catches stale handles; it cannot catch already-escaped `Vector3&`. **Debug builds enforce a "no-mutation-while-reference-live" rule** via a per-store `borrowed_ref_count_` counter incremented by `getPosition()` / `getForce()` / `getVelocity()` calls (RAII helper) and asserted-zero by every mutator. Release builds elide the check; documented as undefined-behavior contract. [Amended 2026-05-17 per Codex review HIGH-D7] |
| D8 | Stable-handle escape hatch | **Required.** `Vector3* getPositionPtr()` re-resolves the column row each access. Survives store resize. For callers that can't comply with D7's live-reference contract. |
| D9 | `Atom::interactions` public field | **Removed.** Sparse store column; getter access only |
| D10 | Source-compat target | **Tests-only.** The post-deletion test subset stays green with zero edits to test sources. Downstream modules (MOLMEC, STRUCTURE, etc.) are out of scope. **D10 only satisfiable because D2/D3/D4 are now "thin stub" rather than "full removal" — full deletion would require editing every test that uses `Composite&` / `PropertyManager*` / `Selectable` in signatures.** [Amended 2026-05-17 per Codex review BLOCKER-1] |
| D11 | Compiled selection | **In K0.** Variant-based cached AST; Selector + Expression rewritten once |
| D12 | Persistence format | **v2 break.** New MoleculeStore-native binary format. K0.6 split: K0.6a = format design + invariants + corpus spec (~2 weeks); K0.6b = implementation + v1→v2 converter (~3-4 weeks). [Amended 2026-05-17 per Codex review HIGH-Persistence] |
| D13 | Memory budget gate | **Typical case ≤160 B/atom HARD** (handle + store columns + bond CSR amortised, at 3 bonds/atom — covers proteins, ligands, nucleic acids). **Worst case (≥10 bonds/atom, e.g. metal-organic frameworks, dense graphs) documented as out-of-budget** — bond storage alone exceeds 200 B/atom at that density. Worst-case workloads use a separate K0.6+ "compact-bond" representation that drops `bond_back_ptr_` and reconstructs handles lazily. [Amended 2026-05-17 per Codex review BLOCKER-2] |
| D14 | Perf budget gate | **Iteration within 2× of baseline** (40k-atom System ≤0.13 s; baseline 0.067 s) |
| D15 | Bond CSR adjacency | **MANDATORY in K0.2/K0.3, NOT optional polish.** Linear `bonds_of()` scan in the K0.2 skeleton becomes O(N²) for whole-system iteration; force-field loops are unusable without CSR. K0.3 work cannot ship without CSR in place. [Added 2026-05-17 per Codex review HIGH-Bond] |
| D16 | Orphan atom store | **Single process-global "orphan store" for default-constructed atoms.** Was: lazy per-Atom MoleculeStore (proven unscalable in the K0.3a prototype — million-orphan-atom scenario = million heap stores). New default `Atom()` ctor borrows from the orphan store; `System::adopt(Atom&)` migrates the atom into the System's per-System store. [Added 2026-05-17 per Codex review HIGH-Lazy-Store; supersedes prototype design in `experiments/moleculeStore/atom_prototype.h`] |

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
§5. The shape is similar but the scope is larger and the order is
revised post-Codex (CSR mandatory before facade; persistence split into
design + implementation subphases).

| Step | Scope | Weeks (1.0 FTE) | Gate | Status |
|---|---|---|---|---|
| K0.1 | Build-fence: `BALL_CORE_ONLY=ON` CMake option; CI matrix entry; verify baseline tests green | 1-2 | Configure clean on macOS/Linux; K0 baseline tests pass | ✅ DONE (d9d94f814) |
| K0.2 | `MoleculeStore` skeleton: columns + generation + selection bitmap + custom_props map | 4-6 | `MoleculeStore_test` green | ✅ DONE (6cee65e3d) |
| K0.2b | **CSR bond adjacency** (post-Codex: mandatory before facade work, not polish). Replace linear `bonds_of()` scan with CSR + incremental rebuild on bond add/remove | 1-2 | `MoleculeStore_test` extended with 100k-atom / 300k-bond bonds_of() perf check < 10 ms total | NEW (D15) |
| K0.2c | **Live-reference contract enforcement** (post-Codex D7): per-store `borrowed_ref_count_` counter; RAII helper for getPosition()/getForce()/getVelocity(); mutator assertions | 1 | `MoleculeStore_test` extended with abort-on-mutate-while-borrowed check | NEW (D7 amendment) |
| K0.3a | Atom2 prototype + findings | 1 | Prototype tests green; findings documented | ✅ DONE (19ab7e12d) |
| K0.3b.1-8 | Atom dual-write for all payload fields (in-tree). | 6-8 | 36/36 green at each per-field commit | ✅ DONE (d57918a20..fe1737ef2) |
| K0.3c.1-6 | **Graph mutation drift fix** (post-Codex-Round-2): orphan reclaim + bond remove + Bond store-awareness + swap CSR mirror + persistence bond population + mutable-ref gap test | 1-2 (parallel) | 36/36 green + 6 new K0.3c tests + orphan-store leak < 5 atoms after full sweep | ✅ DONE |
| K0.3c.7-10 | **Cross-store + atomic + safe-iter fixes** (post-Codex-Round-3): Bond holds explicit bond_store_ + atomic allocate_atom(Atom*) + for_each_bond_of snapshot + string-pool dedup | 0.5-1 week (3 done, 1 deferred) | 36/36 green + 3 new K0.3c tests | ✅ K0.3c.7+8+10 DONE (e0faf2b5c); K0.3c.9 deferred |
| K0.4 | Container facades + hierarchy: AtomContainer, Molecule, Residue, Chain, Fragment, SecondaryStructure, Protein, Nucleotide, NucleicAcid, PDBAtom, System rewritten on store CSR. New iterator templates (replace `BALL_KERNEL_DEFINE_ITERATOR_CREATORS` and `BALL_DECLARE_STD_ITERATOR_WRAPPER` with store-walking equivalents) | 6-8 | AtomContainer_test1/2 + Fragment_test + Chain_test + Molecule_test + System_test + Protein_test + Residue_test1/2 + PDBAtom_test + NucleicAcid_test + Nucleotide_test + SecondaryStructure_test green | |
| K0.5 | Compiled selection: variant-based AST + cache + parser re-entrancy fix. Expression / ExpressionTree / ExpressionParser / Selector rewritten | 3-4 | Expression_test + ExpressionTree_test + ExpressionParser_test + ExpressionPredicate_test + KernelPredicate_test + Selector_test + StandardPredicates_test1..4 + GlobalKernel_test green; Selector_test ≥10× faster than baseline | |
| K0.6a | **Persistence v2 design subphase** (post-Codex D12 amendment): spec the new binary format, identify invariants (atom ordering, bond endpoint stability, hierarchy reproduction, named-property roundtrip), build a curated v1.x corpus with edge cases (malformed-but-tolerated files, intermolecular bonds, dangling refs) | 2 | `STORE-FORMAT-SPEC.md` reviewed; corpus catalogued; invariant checklist signed off | NEW (D12 amendment) |
| K0.6b | Persistence v2 implementation: writer + reader + v1→v2 converter | 3-4 | New `StoreFormat_test` green on the K0.6a corpus; round-trip identity verified | |
| K0.7 | Memory + perf benchmark + budget verification: instrument 40k-atom (D14) + 100k-atom typical-bond (D13) workloads | 1 | Both gates met; worst-case-bond rationale documented; results in `RELEASE-NOTES-v2.0.md` draft | |
| K0.8 | Cleanup + compat audit + release-notes draft: stub-class audit (D2/D3/D4 forwarders correctly delegate), public-API drift sweep, `MILESTONE-CONTEXT-v2.0.md` authored | 1-2 | All gates met; PR ready; planning artefacts complete | |

**Total: 28-39 weeks at 1.0 FTE** (up from the earlier 25-35 — K0.2b CSR
+ K0.2c lease enforcement + K0.6a design split add ~3-4 weeks). Honest:
at sub-1.0 FTE this stretches to 9-14 calendar months.

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

### 6.5 Composite stub keeps the test surface compiling

**Post-Codex amendment 2026-05-17.** With D2 amended from "Full
removal" to "thin stub", these test surfaces (which CANNOT be edited
under D10) continue to type-check:

- `AtomContainer::insertBefore(Atom&, Composite&)` /
  `insertAfter(Atom&, Composite&)` — Composite stays as the parameter
  type. Stub's forwarder uses store CSR to position the new atom.
  See `test/AtomContainer_test2.C:177,190,223,236`.
- `(Composite*)ac1` cast in `test/AtomContainer_test1.C:210` —
  Composite stays as a base type that AtomContainer inherits.
- `composite->create(true)` — Composite stub keeps `create()` as a
  forwarder to AtomContainer::createDefault.
- `PersistentObject* ptr = pm.readObject()` —
  `test/AtomContainer_test1.C:267`. Persistence stays only enough
  for the test to type-check; the actual round-trip is K0.6b. Some
  persistence tests get **deferred** (not "deleted") and re-enabled
  when K0.6b lands.

So the K0 green-bar of 48 stays correct; what changes is the
**implementation strategy** (stubs instead of deletions) and the
**v2.1 horizon** (full deletion happens then, when test edits become
acceptable).

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
