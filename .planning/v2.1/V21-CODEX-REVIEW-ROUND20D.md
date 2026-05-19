# Track A v2.1 Cluster R20d — Codex CLI Round 20d (2026-05-19)

**Status:** Complete
**Verdict:** NEEDS-FIXES
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.1 P2 plan, fourth re-review (final pass)
**Predecessors:** R20 NO-GO, R20b NEEDS-FIX, R20c NEEDS-FIX

## R20c finding closure status

| R20c finding | Resolution | Status |
|---|---|---|
| R20c-1 P2.4 subtree transaction boundary | P2.4 now specifies a subtree-level Scan / Allocate / Write / Commit algorithm, with no destination writes during scan, all destination allocation before link writes, rollback of partial destination allocations on failure, source state left untouched until commit, and free-standing subtree backfill through the same path. | CLOSED |
| R20c-2 bit-property representation | P2.2 makes the representation decision: bit mutators are in scope and use `sparse_bag_` with synthetic names such as `"_bit_<n>"`; packed bool columns are deferred to v2.2. | PARTIAL |
| R20c-3 PropertyManager scope vs atom-indexed storage | P2.2 narrows v2.1 mirroring to Atom-instance `PropertyManager` state only. Non-Atom `PropertyManager` state remains v0-only, which matches the atom-indexed side-table schema. | CLOSED |
| R20c-5 parity test split | P2 now names five separate parity test files: base side-table primitives, Composite, Property, Selection, and Adoption. | CLOSED |

## Probe answers

**P20d-1. Subtree transaction.**  
For `sys.insert(mol)` with `mol -> residue -> 3 atoms`, the revised P2.4 shape is executable: scan the v0 Composite pointer tree first, snapshot topology for Molecule/Residue/Atom nodes, and separately snapshot Atom-local side-table state for the three Atom nodes. The scan does not need RTTI to walk parent/first/next links; implementation can identify Atom nodes either through the existing atom traversal surface or an Atom-only virtual/store-index hook rather than by class-name tests.

Allocation should be treated as "all destination identities before any topology write." Preorder allocation (`mol`, then `residue`, then atoms) is simplest because it naturally builds the old-node to new-handle map, but correctness does not depend on that order as long as every destination composite handle and atom row exists before Write phase emits parent/child/sibling links.

Failure signaling is acceptable if implemented as an RAII transaction helper: allocation helpers may throw (`allocate_atom`, `registerColumn`, composite-node allocation), the helper rolls back the destination rows/free-list entries, and then either rethrows or returns a failure status consistently. The plan's semantic contract is what matters: no source side-table mutation before Commit, and no leaked destination mirror on failure.

**P20d-2. `PropertyManager::getAtomIdx_()` virtual.**  
Atom already has the required identity as `Atom::store_` plus `Atom::store_idx_`; the new virtual can expose exactly that pair for Atom instances. The important implementation detail is that `PropertyManager` itself should provide the default `nullptr` implementation. That covers standard non-Atom classes and also existing non-standard subclasses found in the tree, including NMR/STRUCTURE/FORMAT/VIEW-derived `PropertyManager` users. Those classes must remain v0-only in v2.1 unless they explicitly override the hook later.

**P20d-3. Synthetic bit name collision.**  
This is the last blocker. P2.2 stores unnamed bit property `<n>` as a sparse named property like `"_bit_<n>"`. A user can also legally call `setProperty("_bit_42", "some string")`; both entries would address the same sparse-bag key for the same atom. With D23b sparse-first lookup and v2.1 parity tests comparing side-table state to v0 inline state, this is a lossy representation: a named property can overwrite a bit mirror, or a bit mutator can overwrite a user named property mirror.

Required fix before P2.2 code lands: reserve and reject the chosen prefix for user named properties, or use a non-user namespace/key type for bit mirrors. The cleaner fix is to make sparse-bag keys typed, e.g. `{kind: named, name_id}` vs `{kind: bit, bit_index}`, so v0 named properties and v0 bit properties cannot collide. A reserved-prefix rule is smaller, but it is a new user-visible restriction and must be documented and tested.

**P20d-4. Composite tree traversal stability.**  
The scan assumes the source subtree is stable while adoption runs. That is consistent with D16's single-thread per-System mutation contract and the existing `compact()` exclusive-access style. This should be documented on the P2.4 helper/API comments, but it is not a planning blocker.

**P20d-5. Generation bump in Commit.**  
Destination generation bump is required because destination rows and side-table structures become newly visible. Source generation does not automatically need to bump merely because source rows are released; existing `release_atom()` does not bump `generation_`, and compiled expression cache invalidation is currently tied to operations that invalidate cached intern offsets or borrowed column references, such as `compact()`. During implementation, if P2.4 source-side release changes any structure with stronger invalidation semantics than current atom release, call `CompiledExpressionCache::invalidate_store(source)` or bump source generation there. As planned, this is a can-fix-during-execution detail, not a blocker.

**P20d-6. Overall.**  
P2 is close enough structurally to execute once the bit-key namespace is fixed. The subtree transaction, Atom-only property scope, and test split no longer require another redesign round.

## Overall verdict

**NEEDS-FIXES, with one remaining blocker.**

Must-fix before any P2.2 property-mirroring code lands: define a collision-free representation for bit-property mirrors in `sparse_bag_`. The current `"_bit_<n>"` synthetic name can collide with legal user named properties and makes the side-table mirror ambiguous.

Can-fix during execution: document adoption-source stability under the D16 single-thread mutation contract, implement `PropertyManager::getAtomIdx_()` as a base default returning `nullptr` so external derived classes remain v0-only, and choose consistent exception/return semantics for the P2.4 rollback helper.
