# Track A v2.1 Cluster R20c — Codex CLI Round 20c (2026-05-19)

**Status:** Complete
**Verdict:** NEEDS-FIXES
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.1 P2 plan, third re-review
**Predecessors:** R20 NO-GO, R20b NEEDS-FIXES

## R20b finding closure status

| R20b finding | Severity | Resolution | Status |
|---|---|---|---|
| R20b-1 free-standing subtree backfill | BLOCKER | P2.4 adds adoption-time DFS backfill for free-standing Composite subtrees. | PARTIAL |
| R20b-2 dynamic_cast in getCompositeStore_ helper | NEEDS-FIX | P2.1 now uses a virtual store-reach hook with default nullptr and Atom/System overrides. | CLOSED |
| R20b-3 bit-property mutators | NEEDS-FIX | P2.2 names `setProperty(Property)`, `clearProperty(Property)`, and `toggleProperty(Property)` as in scope. | PARTIAL |
| R20b-4 migration failure semantics | NEEDS-FIX | P2.4 states snapshot -> allocate -> write -> release and registry-cap fallback. | PARTIAL |

## Probe answers

| # | Probe | Verdict | Notes |
|---|---|---|---|
| P20c-1 | Virtual store-reach hook | OK WITH ABI NOTE | `Composite` already has a vptr via `PersistentObject` and virtual methods/destructor, so adding `getCompositeStore_()` does not add per-object storage or move data members. It does add one vtable entry and is therefore an ABI-visible change for prebuilt clients; acceptable if v2.1 only promises source compatibility/rebuilds, but not literally ABI-free. |
| P20c-2 | Backfill ordering | NEEDS-FIX | For `Molecule -> Atom A, Atom B`, the destination Molecule node must be allocated first or at least before commit; A/B destination composite handles must be allocated before Molecule `first/last` and sibling links are written; A/B property columns and sparse entries are written after destination atom rows exist and before source orphan rows are released. The plan says this directionally, but does not define a staged all-node allocation/write/commit boundary, so partial destination rows could be left after an allocation/column failure. |
| P20c-3 | Bit-property mutator decision deferred to P2.2.1 | NEEDS-FIX | `P2.2.1` is referenced but not actually defined as a substep/gate. If packed bool columns are used, the plan must name the column key scheme for unnamed `Property` bits and handle enum collisions/offsets across Atom/PDBAtom/Molecule/Residue/Bond/etc. If sparse fallback is used, parity must cover representative inherited and external bit ranges, not only one Atom bit. |
| P20c-4 | 4-stream transactional migration | NEEDS-FIX | The snapshot is not concretely scoped. Per-atom migration can snapshot one atom's composite handle, property column/sparse entries, and selected bit. Backfill/adoptSubtree needs a subtree-level transaction: collect all source pointers/handles and all v0 links, allocate every destination composite/atom side row, write all links/properties/selection, then publish/release. Per-node transactions can expose or strand a partial subtree. |
| P20c-5 | Parity test scope creep | OK WITH SPLIT | One monolithic `SideTableParity_test` will become hard to review. Split by behavior (`CompositeSideTableParity`, `PropertySideTableParity`, `SelectionSideTableParity`, `AdoptionSideTableParity`, `CompactSideTableParity`), while keeping shared helpers. This is not a blocker if coverage is equivalent. |
| P20c-6 | R20b leftover P20-8 free-standing non-Atom Composite trees | PARTIAL | Free-standing `sys.insert(mol)` is addressed directionally. The gap remains for already-side-tabled non-Atom Composite subtrees transferred between Systems: Molecule/Chain/Residue composite nodes must be migrated and source nodes released transactionally, not merely backfilled as if no source side table existed. |
| P20c-7 | Sub-phase atomicity | OK WITH STRONGER GATES | P2.1 can land safely because v0 remains source of truth, but each subphase must include local parity assertions for the paths it wires. The parity suite is meaningful before P2.5 if each commit's new dual-write surface has tests; it should not wait for a final mega-test. |
| P20c-8 | Overall readiness | NEEDS-FIXES | The pivot is still sound and there are no renewed inheritance-flip blockers. P2 is not quite executable until P2.4 specifies a subtree transaction algorithm and P2.2 resolves the bit-property representation/gates. |

## New findings (if any)

**R20c-1 — P2.4 still lacks a concrete subtree transaction boundary.**  
The revised plan says "snapshot -> allocate -> write -> release," which is the right invariant, but it does not say whether this is per atom, per Composite node, or per adopted subtree. The free-standing backfill case cannot be safely implemented as independent per-node commits: parent/child/sibling links require all destination handles to exist before link write, and failure after some destination rows are allocated must not publish or strand a partial mirror.

Required fix: define P2.4 as a subtree transaction for `adoptSubtree`/Composite adoption:
collect v0 nodes and source side-table handles first; allocate all destination composite nodes/atom rows/property destinations; write all topology/property/selection data; then commit handles and release source rows. On failure, destination allocations are rolled back or left unreachable on a free list, and source side-table state remains untouched.

**R20c-2 — `P2.2.1` is a phantom decision point for bit properties.**  
P2.2 closes the R20b wording gap by saying bit mutators are in scope, but it defers the representation to `P2.2.1` without defining that sub-substep. That is too easy to miss during execution, and unnamed bit properties are not a single property name. The codebase has inherited enum ranges (`AtomContainer`, `Molecule`, `Residue`, `PDBAtom`) plus non-kernel/external ranges such as `SDGenerator`.

Required fix: add an explicit P2.2.1 row or make the decision now. Either specify the packed-bool key scheme and enum-domain handling, or keep bit properties in sparse fallback for P2 with explicit parity tests for set/clear/toggle across inherited bit values.

**R20c-3 — P2.2's "Every PropertyManager mutation" scope is broader than the side-table schema shown.**  
`PropertyManager` is not Atom-only: `Atom`, `AtomContainer`/`Molecule`/`Residue`/`System`, and `Bond` all inherit it. The P1 side-table property storage is keyed by `atom_idx`, and P2.2 says `source/CONCEPT/property.C` writes every mutation to `property_columns_`/`sparse_bag_`. As written, the plan does not define how a base `PropertyManager` mutation discovers whether it belongs to an Atom, non-Atom Composite, or Bond, nor where non-Atom/Bond property rows live.

Required fix: narrow P2.2 explicitly to Atom-local `PropertyManager` mirroring for v2.1, with non-Atom/Bond properties remaining v0-only until a later phase; or define side-table identity/storage for non-Atom Composite and Bond property managers. The success criteria and tests must match that chosen scope.

## Overall verdict

**NEEDS-FIXES for P2 execution.**

The R20b fixes are materially improved: the RTTI helper is gone, free-standing subtree backfill is acknowledged, bit mutators are no longer omitted, and migration now has the right high-level transaction invariant.

However, P2.4 and P2.2 still leave execution-critical choices implicit. Before coding starts, update the plan with a precise subtree transaction algorithm, define the bit-property representation/gate, and reconcile the `PropertyManager` scope with the atom-indexed side-table schema. After those edits, this should be close to a GO rather than another broad redesign round.
