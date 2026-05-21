# V22 Kernel Review KR1

Focused adversarial review of the v2.2 dual-existence molecular-kernel data structures.

## Findings

### HIGH - Cross-store container moves leave live stale rows and valid stale handles in the source store

**Where:** `source/KERNEL/system.C:310`, `source/KERNEL/system.C:392`, `source/KERNEL/system.C:416`, `source/KERNEL/system.C:427`; `source/KERNEL/system.C:257`, `source/KERNEL/system.C:266`, `source/KERNEL/system.C:271`

**Problem:** `System::adoptSubtree()` migrates atom slots out of the source store and then materialises a new container subtree in the destination store, but it never releases the moved container rows from the source store. This is benign only for detached containers that had no source rows. It is wrong for moving an already materialised `AtomContainer` subtree from one `System` to another:

- the v0 tree removes the subtree from the old parent before insertion;
- pass 3 releases source atom slots (`system.C:392-414`);
- pass 4 calls `materialiseContainer_()` (`system.C:416-427`);
- `materialiseContainer_()` creates fresh rows when the existing row binding is for a different store (`system.C:266-272`);
- no code releases the old source container subtree or bumps those rows' generations.

The old `MoleculeStore` can therefore retain live `ContainerRow`s whose atoms were released and whose v0 objects are now bound to a different store. Any previously-created `ContainerHandleBase(old_store, old_row)` remains `isValid()` because the source row was not freed and its generation did not change. The stale source table can also retain `atom_parent_` entries for released atom indices, which can become actively misleading after atom-slot reuse.

**Suggested fix:** Add a production migration path for `container.getContainerRowStore_() != nullptr && != dst` in `System::adoptSubtree()`. Either use and extend `ContainerTable::migrate_subtree_from()` so it returns an old-row to new-row remap and updates every descendant `AtomContainer` row binding, or explicitly release the source container subtree after destination materialisation. In both cases, source rows must be freed via `ContainerTable::release()` so old handles fail generation/freed checks, and source `atom_parent_` entries for migrated atoms must be cleared.

### HIGH - `MoleculeStore::clear()` resets atoms and bonds but leaves the container mirror intact

**Where:** `source/KERNEL/moleculeStore.C:941`, `source/KERNEL/moleculeStore.C:950`, `source/KERNEL/moleculeStore.C:970`, `source/KERNEL/moleculeStore.C:977`; `include/BALL/KERNEL/moleculeStore.h:637`

**Problem:** `MoleculeStore::clear()` is documented as a full reset, and it clears atom columns, bond tables, string pools, and free lists. It does not reset `side_tables_`, so `container_table_` rows, `atom_parent_`, container string pools, composite nodes, and side-table property columns survive the store reset.

For the v2.2 container mirror this is a topology desync: after `clear()`, the v0/store atom state is empty, but `container_table_size_()`, container handles, child vectors, and reverse atom-parent entries can still describe the previous molecule. Because rows are not released, existing container handles also remain valid across a full store clear.

**Suggested fix:** Reset side tables as part of `MoleculeStore::clear()`. The simplest safe implementation is `side_tables_ = std::make_unique<MoleculeStoreSideTables>();` after clearing primary columns. If preserving property-column registry configuration is required, add explicit `clear()` methods on `MoleculeStoreSideTables` / `ContainerTable` that rebuild sentinel slot 0 and bump/free all container row generations before dropping rows.

### HIGH - Container selection counters are exposed but not maintained by v0 selection mutations

**Where:** `include/BALL/KERNEL/_moleculeStoreInternal.h:503`, `include/BALL/KERNEL/_moleculeStoreInternal.h:599`, `include/BALL/KERNEL/containerHandle.h:144`; `source/CONCEPT/composite.C:620`, `source/CONCEPT/composite.C:672`, `source/CONCEPT/composite.C:744`

**Problem:** `ContainerRow::selection_count` is part of the mirror and is exposed through `ContainerHandleBase::getSelectionCount()`, but the v0 selection paths never update it. `Composite::select()`, `Composite::deselect()`, and `Composite::updateSelection_()` mutate v0 selection state and parent counters only; there is no call into `ContainerTable::bump_selection_up()` from production code. A grep found `bump_selection_up()` used only by tests.

This means any handle reading `getSelectionCount()` can return zero or stale values after ordinary selection operations, while v0 `containsSelection()` / selected-child counters say otherwise. The risk is not scalar-field deferral; this is a separately advertised D59 counter and a public H1b getter.

**Suggested fix:** Wire selection mirroring at the atom/container selection boundary. For atom selection, bump from the atom's immediate container row via `atom_parent_`; for container subtree selection, either recompute affected container `selection_count`s from v0 after the operation or apply deltas bottom-up with clear ownership of whether the count means selected atoms only or selected immediate children/subtree nodes. Add parity tests for atom select/deselect, residue select/deselect, and subtree deselect after partial selection.

### MEDIUM - `Composite::insertParent()` changes topology without re-deriving affected container rows

**Where:** `source/CONCEPT/composite.C:970`, `source/CONCEPT/composite.C:986`, `source/CONCEPT/composite.C:990`, `source/CONCEPT/composite.C:1030`, `source/CONCEPT/composite.C:1047`

**Problem:** `insertParent()` rewires a contiguous sibling range under a new parent in the v0 tree, but it has no container-table mirror call. It can remove several children from `parent_ptr`, attach them under `parent`, and replace the sibling range with `parent`; then it returns without re-deriving either affected row. If the involved composites are materialised containers, the table keeps the old child vectors and reverse parent edges.

This is a real topology desync path. The other bulk topology methods now call `mirrorRederiveOwnRow_()` on affected rows after splice/swap/clear/replace, but `insertParent()` does not.

**Suggested fix:** After the pointer surgery succeeds, call `parent.mirrorRederiveOwnRow_()` and `parent_ptr->mirrorRederiveOwnRow_()` when they have rows. If the inserted parent was previously materialised elsewhere, ensure its previous parent was already removed/mirrored by `parent.destroy(destroy_parent)`, or explicitly re-derive that prior parent too.

### LOW - `ContainerRow` likely carries avoidable padding

**Where:** `include/BALL/KERNEL/_moleculeStoreInternal.h:495`, `include/BALL/KERNEL/_moleculeStoreInternal.h:499`, `include/BALL/KERNEL/_moleculeStoreInternal.h:502`, `include/BALL/KERNEL/_moleculeStoreInternal.h:508`

**Problem:** The field order places a `uint8` enum before `uint32`s and a `std::vector`, then places a `uint32` before a `uint64`. On normal 64-bit ABIs this makes `ContainerRow` about 64 B, with padding before the vector and before `generation`. Reordering to put the 8-byte-aligned members first should make the row about 56 B without changing semantics:

`children`, `generation`, `parent_container_idx`, `name_offset`, `selection_count`, `payload`, `kind`.

This is not per-atom hot storage, so it is a low-severity compactness issue, but it is still easy bloat in a table that exists once per molecular container row.

**Suggested fix:** Reorder `ContainerRow` fields and pin `static_assert(sizeof(ContainerRow) <= 56)` if that ABI target is acceptable. Also consider pinning `sizeof(ChildRef) == 8`, `sizeof(ContainerPayload) == 8`, and `sizeof(ContainerChildRef) == 8`; these are currently small and should stay that way.

## Confirmed Sound

- `ChildRef` and `ContainerChildRef` are compact 1-byte discriminator + 32-bit index shapes, with no pointer payload per edge (`_moleculeStoreInternal.h:459-471`, `containerKind.h:65-76`). A 4-byte packed representation is possible only by stealing index bits; the current 8-byte shape is a defensible safety tradeoff.
- `ContainerPayload` contains only kind-specific scalars, while common name/parent/children/selection/generation live in `ContainerRow` (`_moleculeStoreInternal.h:481-510`). I did not find duplicate storage of id/name in both row and payload for the same semantic field, except the accepted scalar-mutation deferral.
- Public handles do not leak private `ContainerRow`, internal `ChildRef`, or `ContainerTable` types. `containerHandle.h` includes only public headers and forwards reads through scalar `MoleculeStore` accessors (`containerHandle.h:15-19`, `moleculeStore.h:423-452`).
- Typed container handles add no data members and are pinned to the 24 B base size (`containerHandle.h:187-193`, `containerHandle.h:214-230`). The 64-bit generation is justified for stale-row safety; shrinking it would be a policy change, not an obvious bug.
- Same-store append/reparent paths are mostly covered: `Composite::appendChild()` removes from an old parent first (`composite.C:921-925`), remove mirrors the old edge (`composite.C:1482-1491`), and append mirrors the new edge (`atomContainer.C:336-347`, `atomContainer.C:468-472`). The `append_child()` no-op for "already current last child" closes the duplicate-edge case for v0 append no-ops (`moleculeStore.C:386-397`).
- The vector-only `clear_children()` behavior is intentional and correct for order-independent re-derive of multiple affected parents; it avoids one parent re-derive clearing another parent's freshly repaired reverse edge (`moleculeStore.C:440-456`).
- The `swap()` snapshot relink avoids the sibling self-loop class and re-derives the two swapped rows plus their final parents (`composite.C:1650-1700`, `composite.C:1739-1750`). I did not find a remaining sibling/adjacent swap mirror inconsistency in that path.
- The `being_destroyed_` guard is placed on the mirror helpers, and destructors set it before teardown in `AtomContainer` and `System` (`atomContainer.C:79-83`, `system.C:444-447`). That prevents destructor-cascade remove/clear from mutating a half-destroyed parent's mirror.

## Axes Summary

- **Compactness:** no critical per-atom bloat in the reviewed container structures; one low row-padding issue.
- **Division of labor:** public/private boundary is mostly clean. The main layering smell is `System::materialiseContainer_()` doing RTTI/dynamic-cast payload extraction and container table construction in one place, but it is acceptable transitional glue under dual existence.
- **Data-structure consistency:** real issues found in cross-store container moves, store clear, selection counters, and `insertParent()`. These can desync the mirror from the v0 source of truth beyond the accepted scalar-field deferral.

---

## Dispositions (applied 2026-05-21)

**FIXED now (obvious / contained):**
- **HIGH-2 (`MoleculeStore::clear()` left the container mirror intact)** —
  FIXED. `clear()` now `side_tables_ = std::make_unique<MoleculeStoreSideTables>()`
  after wiping primary columns, so the container table / atom_parent_ / props
  reset with the store and stale container handles fail their checks
  (`moleculeStore.C`).
- **MEDIUM (`insertParent` no mirror)** — FIXED for the already-materialised
  case (`parent.mirrorRederiveOwnRow_()` + `parent_ptr->mirrorRederiveOwnRow_()`,
  gated on `parent` having a row). The new-unmaterialised-parent case is the
  deferred materialise-new-member carry-over; documented inline that
  insertParent's sole production caller (PDB SS-grouping) operates pre-adoption
  (captured by materialise-at-adoption) AND is removed by the collapse (D-HC4
  SS=annotation), so not over-invested (`composite.C`).
- **LOW (`ContainerRow` padding)** — FIXED. Reordered fields 8-byte-aligned
  first (children, generation, indices, payload, kind) → packs 64→56 B; added
  `static_assert` pins for `ChildRef`==8, `ContainerPayload`==8, `ContainerRow`
  <=56 (`_moleculeStoreInternal.h`).

**DEFERRED → HCP-1 (latent; collapse reworks them — build once):**
- **HIGH-3 (selection counters never wired)** — `ContainerRow::selection_count`
  is never updated by v0 `select()`/`deselect()`; `getSelectionCount()` returns
  0. NO production consumer reads it yet (dual existence — verified zero
  callers). The selection mirror is built once at HCP-1 against the collapsed
  role model (alongside the deferred scalar/role mirror) and **blocks H2d**.
  Added a `\warning` to `getSelectionCount()` so it cannot silently mislead.
- **HIGH-1 (cross-store container move leaves stale source rows + valid
  handles)** — `adoptSubtree` does not release the source container subtree /
  clear source `atom_parent_` on a cross-`System` move of a materialised
  subtree. Latent (no production container-handle consumer; cross-store
  container moves are rare). Folds into the materialise-new-member path at
  HCP-1.

Both deferrals follow the same rule as the scalar mirror: the collapse (HC1b)
reworks selection counters + the container-creation path into the role model,
so the mirror is built ONCE against the collapsed model rather than twice.
Recorded in `V2X-ROADMAP.md` H2b carry-overs.
