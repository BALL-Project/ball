# BALL v2.2 H2 Planning Review - Round 35

**Status:** NEEDS-FIXES  
**Verdict:** H2 is not design-locked for H2a.  
**Reviewer:** Codex CLI 0.128.0  
**Subject:** H2 mutation mirror + destruction guard + handle-yielding traversal

## Probe Answers

### P35-1. D69 destruction guard

Partly correct, incomplete as written. Checking `this->being_destroyed_` in `removeChild` is the right parent-side predicate for the P2.1.1 trap: destructor child deletion re-enters `parent_->removeChild(*this)` from `Composite::~Composite()` (`source/CONCEPT/composite.C:257-265`) after `destroyChildren_()` deletes each child (`source/CONCEPT/composite.C:638-657`), and `removeChild` is the mutating detach site (`source/CONCEPT/composite.C:1298-1381`).

But setting the flag only at `~Composite` entry is too late for this codebase. Derived destructors call `destroy()` before base `~Composite` runs: `Atom::~Atom()` calls `destroy()` at `source/KERNEL/atom.C:289-327`, `AtomContainer::~AtomContainer()` calls `destroy()` at `source/KERNEL/atomContainer.C:52-68`, `Molecule::~Molecule()` calls `destroy()` at `source/KERNEL/molecule.C:32-35`, `Residue::~Residue()` calls `destroy()` at `source/KERNEL/residue.C:78-92`, `Chain::~Chain()` calls `destroy()` at `source/KERNEL/chain.C:27-30`, and `System::~System()` calls `destroy()` at `source/KERNEL/system.C:338-340`. Therefore the flag must be set at the start of both `Composite::destroy()` overloads before `clear()` / child teardown (`source/CONCEPT/composite.C:1483-1508`), not merely in `~Composite`.

`clear()` on a live container is also not just "per-child removeChild": auto-deletable children get deleted and re-enter `removeChild`, but non-auto-deletable children are detached by direct pointer writes (`source/CONCEPT/composite.C:1447-1461`). H2 must mirror the live clear explicitly for that branch, while skipping it when `being_destroyed_` is set.

### P35-2. Open risk #1: atom-index timing

The design premise is stale for the current code. `Atom::~Atom()` calls `destroy()` before `release_atom()` (`source/KERNEL/atom.C:289-312`), and `Atom::destroy()` calls `Composite::destroy()` (`source/KERNEL/atom.C:325-327`), whose parent detach happens before atom slot release (`source/CONCEPT/composite.C:1483-1490`). So during the normal delete-atom forward detach, `Atom::getStoreIndex()` is still readable and the slot has not yet been reused.

Slot reuse between release and mirrored remove is not possible on the current order. Capturing the atom index in `~Atom` or mirroring detach from `~Atom` is unnecessary for this exact path and could duplicate the `Composite::destroy()` detach unless carefully gated. The required H2 contract should instead pin the order: mirror live parent detach before `Atom::~Atom` releases the store slot.

### P35-3. D68 slot reuse

Production reads of `composite_nodes_` appear absent; the live production topology view still reads v0 inline pointers (`source/CONCEPT/composite.C:73-83`). Current `composite_nodes_` uses are allocator/helper code and tests (`source/KERNEL/moleculeStore.C:126-177`, `test/SideTableParity_test.C:45-127`, `test/SideTableParity_test.C:438-505`).

Reusing `composite_handle_packed_` is only safe if the old API is removed or made impossible to call. Leaving `mirrorToSideTable_()` callable while the same 8-byte slot stores a container row is a footgun: it will reinterpret a row index as `CompositeHandle` bytes and may allocate/write stale `composite_nodes_` state (`source/CONCEPT/composite.C:51-61`, `source/CONCEPT/composite.C:150-212`, `include/BALL/CONCEPT/composite.h:1636-1679`). `SideTableParity_test` directly depends on the old mirror and handle API (`test/SideTableParity_test.C:110-127`, `test/SideTableParity_test.C:438-497`), so H2a must update or delete those sections.

### P35-4. D70 mutation mapping fidelity

The insertBefore/insertAfter receiver semantics are correctly identified: `this` is the pivot and the inserted argument becomes a sibling under `parent_` (`source/CONCEPT/composite.C:965-1023`, `source/CONCEPT/composite.C:1025-1083`). The parent/pivot mapping is right.

The move mapping is not locked. `ContainerTable::reparent_child` always appends to the new parent (`source/KERNEL/moleculeStore.C:435-443`), so it cannot directly represent prepend or insert-before/after placement. For positional moves, H2 must either mirror the old-parent `removeChild` first and then use the positional attach op, or add positional reparent ops.

The mutation set is missing real topology mutators: `Composite::replace()` calls `insertBefore` then `removeChild` (`source/CONCEPT/composite.C:1085-1091`), and `Composite::insertParent()` rewrites a sibling range under a new parent (`include/BALL/CONCEPT/composite.h:496`, `source/CONCEPT/composite.C:885-963`). These are used outside tests (`source/FORMAT/PDBFileDetails.C:166`, `source/FORMAT/PDBFileDetails.C:261`, `source/FORMAT/PDBFileDetails.C:307`). Deep copy is mostly covered because `clone_()` builds children via `appendChild` (`source/CONCEPT/composite.C:480-496`, `source/CONCEPT/composite.C:1889-1908`), but `set`/`clone` still need explicit parity coverage when the destination root is already store-reachable.

`swap` is under-specified. v0 `swap` exchanges tree positions, child lists, selection counters, properties, and selectable state (`source/CONCEPT/composite.C:1511-1600`). The table plan says "exchange rows' child lists + payload", but atoms have no container row and `Composite::swap` can be called on arbitrary `Composite` siblings. H2 must define container-container, atom-atom, and atom-container behavior or explicitly exclude unsupported cases with tests.

### P35-5. Open risk #3: free-standing pre-adoption mutation

There is a gap. Free-standing containers have no reachable store: default `Composite::getCompositeStore_()` walks parents and returns null if no ancestor provides a store (`source/CONCEPT/composite.C:101-126`). But atoms are always bound to the orphan store at construction (`source/KERNEL/atom.C:212-220`).

So `new Molecule; m.insert(atom); m.insert(residue); sys.insert(m)` builds v0 children before any container row exists. `AtomContainer::append(AtomContainer&)` first mutates the v0 tree, then calls `System::adoptSubtree` only if the receiver is already rooted under a `System` (`source/KERNEL/atomContainer.C:426-438`). `System::adoptSubtree` migrates atom slots and bonds, but it currently does not materialize container rows from v0 (`source/KERNEL/system.C:217-267`). D56 `migrate_subtree_from` moves an existing source container row (`include/BALL/KERNEL/_moleculeStoreInternal.h:626-648`, `source/KERNEL/moleculeStore.C:546-558`); it cannot migrate a row that was never allocated.

H2 needs an adoption materialization path: either allocate orphan container rows during free-standing mutation, or recursively build the container-table subtree from the v0 object tree when the detached container is adopted.

### P35-6. Open risk #4: selection double-count

The double-count risk is real if H2 hooks propagated counter updates. v0 selection recursively selects descendants and then updates parent counters through `select_`, `deselect_`, and `updateSelection_` (`source/CONCEPT/composite.C:535-635`, `source/CONCEPT/composite.C:659-742`). The table primitive `bump_selection_up` already walks the parent-row chain from one starting container (`include/BALL/KERNEL/_moleculeStoreInternal.h:593-598`, `source/KERNEL/moleculeStore.C:445-463`).

The design should hook exactly the leaf atom selected-bit transition and call `bump_selection_up(parentRow, +/-1)` once. It must not also hook container-level `updateSelection_()` transitions.

### P35-7. D71 traversal + D72 phasing

A preorder walk over ordered `ChildRef` edges can match v0 child order: `ContainerTable::preorder` visits the root container, then recurses children in vector order and emits atom children in place (`include/BALL/KERNEL/_moleculeStoreInternal.h:606-624`), which corresponds to v0 `first_child_` / `next_` order. Deferring consumers to H3 is correct.

The sub-phases are only independently parity-testable if each test limits itself to wired operations. H2a can test append/prepend/insert/remove/delete/~System only. Full "parity after every mutation" is not achievable until H2d unless tests avoid splice/swap/clear/select/property before those hooks exist.

### P35-8. Overall

NEEDS-FIXES. H2a should not start until the destruction flag placement, slot-retirement mechanics, free-standing adoption materialization, positional reparent mapping, and missing mutator coverage are resolved in the design.

## New Findings

### BLOCKER: `~Composite` entry guard is too late for actual derived destructors

Derived destructors call `destroy()` before the base destructor can set a flag (`source/KERNEL/atom.C:289-327`, `source/KERNEL/atomContainer.C:52-68`, `source/KERNEL/system.C:338-340`). If H2 only sets `being_destroyed_` in `Composite::~Composite()` (`source/CONCEPT/composite.C:257-265`), child teardown already happened through `Composite::destroy()` / `clear()` (`source/CONCEPT/composite.C:1483-1508`, `source/CONCEPT/composite.C:1447-1481`). The flag must be set at the start of both `Composite::destroy()` overloads and remain set through child teardown.

### BLOCKER: detached subtree adoption has no source container rows to migrate

Free-standing container mutations have no reachable store (`source/CONCEPT/composite.C:101-126`), while D56 migration requires an existing source row (`source/KERNEL/moleculeStore.C:546-558`). `AtomContainer::append(AtomContainer&)` only adopts after insertion into a System-rooted tree (`source/KERNEL/atomContainer.C:426-438`), and `System::adoptSubtree` currently migrates atoms, not container rows (`source/KERNEL/system.C:217-267`). H2 must define recursive v0-to-table materialization on adoption or orphan-row allocation during free-standing mutation.

### HIGH: Slot reuse is unsafe if old side-table APIs remain callable

`getCompositeHandle_`, `setCompositeHandle_`, and `mirrorToSideTable_` still expose the old `CompositeHandle` interpretation of `composite_handle_packed_` (`include/BALL/CONCEPT/composite.h:1612-1679`, `source/CONCEPT/composite.C:51-61`, `source/CONCEPT/composite.C:150-212`). Reusing that slot for a container row while leaving those APIs live can corrupt or resurrect `composite_nodes_`. `SideTableParity_test` still exercises those APIs (`test/SideTableParity_test.C:110-127`, `test/SideTableParity_test.C:438-497`).

### HIGH: Positional reparent is not represented by `reparent_child`

`ContainerTable::reparent_child` removes from the old reverse-edge parent and appends to the new parent (`source/KERNEL/moleculeStore.C:435-443`). It cannot mirror `prependChild`, `insertBefore`, or `insertAfter` moves without losing the requested ordinal. The D70 "always reparent_child" row conflicts with exact v0 ordering.

### HIGH: Mutation surface omits `insertParent` and under-specifies `swap`

`insertParent` rewrites a sibling range into a new parent (`source/CONCEPT/composite.C:885-963`) and is used in PDB import paths (`source/FORMAT/PDBFileDetails.C:166`, `source/FORMAT/PDBFileDetails.C:261`, `source/FORMAT/PDBFileDetails.C:307`). `swap` exchanges far more than child vectors (`source/CONCEPT/composite.C:1511-1600`) and needs atom/container case handling.

### MEDIUM: Live `clear()` directly detaches non-auto-deletable children

`Composite::clear()` bypasses `removeChild` for non-auto-deletable children by zeroing their topology pointers and then clearing the child (`source/CONCEPT/composite.C:1457-1461`). A removeChild-only mirror misses this live detach.

## Overall Verdict

**NEEDS-FIXES.** Required design changes before H2a:

1. State that `being_destroyed_` is set at the start of `Composite::destroy()` and `Composite::destroy(bool)` before child teardown, not only in `~Composite`; add tests for derived-destructor cascades.
2. Define how live `clear()` mirrors non-auto-deletable direct detaches while destructor teardown skips them.
3. Replace "D56 migration covers free-standing mutation" with a concrete orphan-row allocation or v0-to-table materialization path on adoption.
4. Remove/rename/inert the old `CompositeHandle` / `mirrorToSideTable_` APIs and update `SideTableParity_test` before reusing `composite_handle_packed_`.
5. Fix the D70 mapping for positional reparent and define `insertParent`, `replace`, `set`/`clone`, and `swap` parity scope.
6. Pin atom delete ordering: live parent detach mirror happens before `Atom::~Atom` releases the atom store slot.

## R35b re-review (post-fix)

Verdict: NEEDS-FIXES before H2a. The R35 findings are closed as written, but the revised dedicated row binding introduces an unresolved lifecycle/stale-index blocker.

- R35 BLOCKER `~Composite` entry too late: CLOSED. D69 now requires `being_destroyed_` at the start of both `Composite::destroy()` overloads and `~Composite` as backstop (`.planning/v2.2/V22-H2-DESIGN.md:101`); this matches the actual derived destructors calling `destroy()` first (`source/KERNEL/atom.C:289`, `source/KERNEL/atomContainer.C:52`, `source/KERNEL/system.C:338`). The destructor-cascade window is closed.
- R35 BLOCKER free-standing adoption has no rows: CLOSED for pure free-standing adoption. D73 adds recursive v0-to-table materialization for every unbound container and builds atom `ChildRef`s after atom-slot migration (`.planning/v2.2/V22-H2-DESIGN.md:232`, `.planning/v2.2/V22-H2-DESIGN.md:249`), which covers `new Molecule; m.insert(atom); m.insert(residue); sys.insert(m)`.
- R35 HIGH slot reuse unsafe: CLOSED. D68 uses a dedicated `AtomContainer::container_row_idx_`, leaves `composite_handle_packed_` inert, and notes all molecular parents are `AtomContainer`s (`.planning/v2.2/V22-H2-DESIGN.md:67`, `.planning/v2.2/V22-H2-DESIGN.md:81`). The actual molecular insertion receivers are `AtomContainer`/`System` wrappers (`source/KERNEL/atomContainer.C:290`, `source/KERNEL/atomContainer.C:426`, `source/KERNEL/system.C:527`).
- R35 HIGH positional reparent: CLOSED. D70 now specifies remove from old parent plus the exact positional attach op, with append-only `reparent_child` reserved for append moves (`.planning/v2.2/V22-H2-DESIGN.md:165`). This matches `insertBefore`/`insertAfter` receiver semantics (`source/CONCEPT/composite.C:965`, `source/CONCEPT/composite.C:1025`).
- R35 HIGH insertParent/swap/replace/set/clone: CLOSED at design-scope level. D70 covers `replace`, `insertParent`, `set`/`clone`, and scopes `swap` to container-container plus atom-atom with atom-container rejected (`.planning/v2.2/V22-H2-DESIGN.md:157`, `.planning/v2.2/V22-H2-DESIGN.md:172`). `insertParent` is real and used by PDB import (`source/CONCEPT/composite.C:885`, `source/FORMAT/PDBFileDetails.C:166`); no molecular API exposes atom-container swap, only `Atom::swap(Atom&)` and `AtomContainer::swap(AtomContainer&)` (`include/BALL/KERNEL/atom.h:310`, `include/BALL/KERNEL/atomContainer.h:130`).
- R35 MEDIUM live `clear()` non-auto-deletable branch: CLOSED. D70 explicitly mirrors the direct-detach branch and skips only under `being_destroyed_` (`.planning/v2.2/V22-H2-DESIGN.md:159`), matching v0's direct pointer clear (`source/CONCEPT/composite.C:1457`).

NEW BLOCKER: `container_row_idx_` has no store owner or lifecycle contract, so stale row indices survive row release, cross-System migration, `System` destruction, and explicit destroy/re-adoption. D68 stores only a `uint32_t` row and allocates only when the slot is zero (`.planning/v2.2/V22-H2-DESIGN.md:67`, `.planning/v2.2/V22-H2-DESIGN.md:87`); D73 says store-to-store moves use `migrate_subtree_from` (`.planning/v2.2/V22-H2-DESIGN.md:245`), but that function releases the source rows and returns new row indices without any path to rebind each v0 `AtomContainer`'s slot (`source/KERNEL/moleculeStore.C:546`, `source/KERNEL/moleculeStore.C:557`). `ContainerTable::release` also recycles the numeric slot (`source/KERNEL/moleculeStore.C:331`), while `System::~System` invalidates atom handles only, not future container row slots (`source/KERNEL/system.C:346`). Fix before H2a: make the v0 binding store-aware or explicitly clear/rebind `container_row_idx_` for every container on row release, `migrate_subtree_from`, `System` teardown, and re-adoption; add tests for cross-System reparent and destroy/reinsert.

NEW HIGH: the `being_destroyed_` flag is specified as set at `destroy()` entry but not reset for explicit public `destroy()` calls on objects that remain alive (`.planning/v2.2/V22-H2-DESIGN.md:101`). `Composite::destroy()` is a public teardown/reuse method, not only a destructor path (`source/CONCEPT/composite.C:1483`, `source/KERNEL/atomContainer.C:65`). If the flag is a permanent latch, later live removes from the same object will silently skip mirrors; if it is scoped, the design must say so and rely on the `~Composite` backstop for actual destructor teardown.

Selection note: the leaf-only hook is directionally correct for recursive container selection because v0 `Composite::select_` recurses into descendants (`source/CONCEPT/composite.C:540`) and D70 hooks only atom leaf bit transitions (`.planning/v2.2/V22-H2-DESIGN.md:162`). Test it with `container.select()` and `container.deselect()` over nested residues to prove exactly one bump per descendant atom.
