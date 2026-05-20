# Track A v2.2 Cluster R31 — Codex CLI Round 31 (2026-05-20)
**Status:** Complete
**Verdict:** GO -- start H1a
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.2 H0 addendum re-review (D56-D62 close R30's 7 contracts)

## Probe answers

**P31-1. D56 container orphan store is viable.** The existing code already has the right migration shape for atoms: `System::adoptSubtree(AtomContainer&)` collects every descendant atom, snapshots source payload, allocates destination slots, migrates bonds, then releases source slots (`source/KERNEL/system.C` lines 217-320). `AtomContainer` calls it after inserting a detached container under a rooted System (`source/KERNEL/atomContainer.C` lines 426-455). Extending this to container rows is mechanically plausible: add a container-entry pass before/alongside atom allocation, create an old-container-index to new-container-index map, copy payload/properties/selection, rewrite container parent links, rewrite atom `parent_container_idx`, rebuild/copy `ChildRef` edges through the map, then release orphan rows.

The lifecycle contract is also adequate for H1a: D56 says detached handles point at the orphan store, migrated handles are rebound to the System store, and the orphan mutex pattern is reused. One implementation detail must be made explicit when H1b exposes freely copyable container handles: aliases copied before migration cannot all be silently rebound. The likely policy is "the handle passed to insert is rebound; stale aliases either remain orphan-bound and fail generation checks after release or become null/stale handles." That is not a blocker for H1a's store-side prototype.

**P31-2. D57 ordered `ChildRef` edges close the traversal/order hole.** Current `Composite` semantics are a doubly linked child list (`first_child_`, `last_child_`, `previous_`, `next_`) with operations like `insertBefore`, `insertAfter`, `spliceBefore`, `spliceAfter`, and `swap` rewriting sibling and parent pointers (`source/CONCEPT/composite.C` lines 965-1206 and 1511-1589). A per-container `vector<ChildRef{kind,idx}>` is the direct indexed equivalent. It can represent mixed atom/container child order, preorder/postorder, insertion before/after a particular child, bulk splice of another container's child vector, and swap by exchanging child lists/payload plus fixing reverse parents.

The CSR split is coherent: `ChildRef` vectors are the source of truth for order, while CSR is a derived/cache layer for fast atom grouping. This avoids R30's failure mode where free-list/store index order accidentally becomes traversal order.

**P31-3. D58 schema is complete enough against the checked headers.** The common row covers `AtomContainer::name_` and tree/properties/selection. Payloads cover the scalar state visible in the container headers: `Protein::id_`, `NucleicAcid::id_`, `Residue::id_` plus `insertion_code_`, `Nucleotide::id_` plus `insertion_code_`, and `SecondaryStructure::type_`. Fragment and Molecule have no additional private scalar payload in the checked headers beyond name/properties. Residue and Nucleotide enum-style flags are better represented as properties/sparse property bits than as large bespoke fields, and D58/D59 allow that.

Minor note: `Chain` currently appears to have a name but no separate `id_` field. D58's `Chain : { id : string }` is harmless if it is the chain identifier/name mapped into payload, but H1a should avoid inventing two chain strings unless code review finds a real second field.

**P31-4. D60 dual-existence strategy is sound enough to start.** R30 needed a source-of-truth rule; D60 now provides one: v0 object tree wins through H1-H3, store tables are a verified mirror, and H4 flips atomically. That is the right incremental strategy because existing mutation paths can remain authoritative while H1a/H1b/H2 build mirror tables and parity.

Forward-only mirroring is sufficient if interpreted exactly as written: mirror all forward mutations, never mirror from destructors, and free store rows wholesale during teardown. The guardrail is not hand-wavy if `HierarchyParity_test` is implemented as an executable traversal comparator after mutation-heavy scenarios: structure, ordered child sequence, parent links, payload, properties, and selection counters must match the v0 tree. D60 also correctly defers enumerating the full mutation set to H2, while H1a only needs the parity skeleton.

**P31-5. D61 pointer-return break policy is adequate, with expected migration pain.** The common idiom can migrate cleanly from:

```cpp
if (Residue* r = atom.getResidue()) r->getName();
```

to:

```cpp
if (Residue r = atom.getResidue()) r->getName();
```

provided handles are bool-testable and expose pointer-like `operator->`. The ledger must still capture harder breaks: explicit pointer variables and member fields, storing `Atom*`/`Residue*` in STL containers, pointer identity comparison/order, `nullptr` overload resolution, functions taking `Composite*`/`AtomContainer*`, `dynamic_cast`/RTTI flows, address-taking of iterator results, and any code that assumes object lifetime or deletes returned pointers. D61 names the right break classes and mandates ledger/sign-off, so this is no longer under-specified for H1a.

**P31-6. D62 persistence bump is acceptable and captures the topology.** A schema major bump is reasonable for A2 because v2.1 readers cannot reconstruct a handle-only container topology from the old molecules/atom_indices encoding. The proposed `containers` section includes the necessary round-trip data: kind, parent, name, typed payload, ordered children, and properties, with atom `parent_container_idx` rebuilt and validation for cycles/orphans/parent resolution. That is sufficient as a design contract; H6 can refine exact JSON field names.

**P31-7. H1a is now bounded and buildable.** H1a has a clear non-invasive scope: add container row/table storage, ordered `ChildRef` vectors, orphan container rows, migration plumbing, and a parity-test skeleton while leaving consumers on the v0 tree. A developer can start from D56-D60 without changing public traversal or pointer-return APIs. The main H1a acceptance criterion should be a store mirror that can be built/updated from representative v0 insert/append/prepend/insertBefore/insertAfter cases and compared by parity.

**P31-8. Overall: GO.** The addendum closes the seven R30 contracts to the level needed to start H1a. The remaining risks are implementation risks, not missing architectural contracts.

## Remaining gaps (if any)

No blocking design gaps for H1a.

Non-blocking follow-ups to track during implementation:

- Define stale/alias behavior for copied detached container handles before H1b value handles become a public surface.
- Resolve whether `Chain` has a separate payload `id` or uses the common `name` as its identifier.
- Make `HierarchyParity_test` concrete early: compare ordered `ChildRef` traversal, reverse parent links, payload, properties, and selection counts after each covered mutation.
- Seed `V22-API-BREAK-LEDGER.md` with pointer-container, pointer-comparison, `nullptr`, `Composite*`, and RTTI migration cases, not only simple return-type examples.

## Overall verdict

**GO -- start H1a.** D56-D62 adequately lock the detached-container lifecycle, deterministic child ordering, typed container payloads, selection/property/timestamp storage, dual-existence source-of-truth rule, pointer-return break policy, and JSON topology persistence. H1a is now a bounded store-side prototype with parity scaffolding and no required consumer migration.
