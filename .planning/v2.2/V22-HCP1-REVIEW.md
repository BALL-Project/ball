Verdict: NEEDS-REVISION

# HCP1R -- BALL v2.2 HCP-1 Close Review

Scope reviewed: commits 480733b84, b2484ee7b, ef9ec0443, b7b0fcb9c; role layer, scalar mirror, selection mirror, materialisation, and `HierarchyParity_test.C`.

## Findings

### CRITICAL -- `selection_count` does not follow selected atoms across topology moves/removes

`Composite::mirrorAtomSelection_()` only bumps counts on atom `select_()` / `deselect_()` transitions (`source/CONCEPT/composite.C:303`, `source/CONCEPT/composite.C:670`, `source/CONCEPT/composite.C:725`). Topology mutation paths update the child edge mirror but never transfer the selected-atom contribution between old and new ancestor chains: `mirrorRemoveChild_()` removes the edge only (`source/CONCEPT/composite.C:216`, `source/CONCEPT/composite.C:234`, `source/CONCEPT/composite.C:241`), `mirrorAppendChild_()` appends the edge only (`source/CONCEPT/composite.C:249`, `source/CONCEPT/composite.C:257`, `source/CONCEPT/composite.C:262`), and `mirrorRederiveOwnRow_()` clears/rebuilds children only (`source/CONCEPT/composite.C:271`, `source/CONCEPT/composite.C:277`). The store-side child operations likewise only update edges (`source/KERNEL/moleculeStore.C:425`, `source/KERNEL/moleculeStore.C:452`, `source/KERNEL/moleculeStore.C:461`).

Concrete drift: select `a` under `r1`, then move `a` to `r2` via `r2.insert(a)`. `Composite::removeChild()` decrements v0 parent selection counters (`source/CONCEPT/composite.C:1519`) but the mirror only removes the edge (`source/CONCEPT/composite.C:1546`). `AtomContainer::append(Atom&)` then appends/rederives the new edge (`source/KERNEL/atomContainer.C:414`, `source/KERNEL/atomContainer.C:416`) without adding the selected atom to `r2`'s row. Result: `r1.selection_count` stays too high and `r2.selection_count` stays too low until a later select/deselect happens to mask it. Removing or clearing a selected child has the same problem.

Suggested fix: when a selected atom or a subtree containing selected atoms is detached, attached, reparented, replaced, spliced, or cleared, adjust `selection_count` by the subtree selected-atom cardinality on the affected old/new parent chains. The least fragile implementation is a helper that computes selected atom count from v0 for a child/subtree and is called by `mirrorRemoveChild_()`, `mirrorAppendChild_()`, and the rederive/clear paths. Add parity tests for selected atom reparent, selected residue/container reparent, selected subtree removal, clear of selected children, and splice/replace with selected descendants.

### HIGH -- Row scalars go stale through `set()` / assignment paths

HCP-1 wires explicit scalar setters and clear/swap, but copy assignment still writes mirrored scalar fields directly with no `mirrorResyncScalars_()` call. Examples:

- `AtomContainer::set()` copies `name_` at `source/KERNEL/atomContainer.C:187` and `source/KERNEL/atomContainer.C:194`.
- `Protein::set()` copies `id_` at `source/KERNEL/protein.C:65` and `source/KERNEL/protein.C:68`.
- `NucleicAcid::set()` copies `id_` at `source/KERNEL/nucleicAcid.C:65` and `source/KERNEL/nucleicAcid.C:68`.
- `Residue::set()` copies `id_` and `insertion_code_` at `source/KERNEL/residue.C:124` and `source/KERNEL/residue.C:128`.
- `Nucleotide::set()` copies `id_` and `insertion_code_` at `source/KERNEL/nucleotide.C:76` and `source/KERNEL/nucleotide.C:79`.

Those `set()` methods back the assignment operators (`source/KERNEL/atomContainer.C:206`, `source/KERNEL/protein.C:71`, `source/KERNEL/residue.C:132`, `source/KERNEL/nucleotide.C:83`). On an already materialised container, `r = other;` updates the v0 source of truth but leaves `container_name_`, `container_id_`, `container_insertion_code_`, and stored `ResidueKind` stale.

Suggested fix: call `mirrorResyncScalars_()` at the end of every scalar-copying `set()` method, after the most-derived fields are copied. For derived classes this should be the final statement after their own fields, matching the clear/swap pattern. Add `HierarchyParity_test` cases for post-root assignment of `Protein`, `Residue`, `Nucleotide`, and generic `AtomContainer`/`Chain`.

### HIGH -- Row scalars go stale through `persistentRead()`

The persistence read paths assign the same mirrored fields without re-sync:

- `AtomContainer::persistentRead()` reads `name_` at `source/KERNEL/atomContainer.C:176` and `source/KERNEL/atomContainer.C:184`.
- `Protein::persistentRead()` reads `id_` at `source/KERNEL/protein.C:57` and `source/KERNEL/protein.C:62`.
- `NucleicAcid::persistentRead()` reads `id_` at `source/KERNEL/nucleicAcid.C:57` and `source/KERNEL/nucleicAcid.C:62`.
- `Residue::persistentRead()` reads `id_` and `insertion_code_` at `source/KERNEL/residue.C:114` and `source/KERNEL/residue.C:120`.
- `Nucleotide::persistentRead()` reads `id_` and `insertion_code_` at `source/KERNEL/nucleotide.C:66` and `source/KERNEL/nucleotide.C:72`.
- `SecondaryStructure::persistentRead()` reads `type_` at `source/KERNEL/secondaryStructure.C:59` and `source/KERNEL/secondaryStructure.C:64`.

This is a real post-root mutation entry point, not just load-time construction: `persistentRead()` is a public virtual method and can be invoked on a row-bound object. After that, the v0 tree and row mirror disagree.

Suggested fix: call `mirrorResyncScalars_()` at the end of each persistent read after all subtype fields have been read. Add a post-root persistence-read parity test for at least `Residue` and `SecondaryStructure`.

### HIGH -- `Selectable::setSelected()` bypasses the selection mirror entirely

`Selectable::setSelected(bool)` writes `selected_` directly (`include/BALL/CONCEPT/selectable.h:123`) and `Composite` does not override it. That bypasses both v0 composite parent counters and HCP-1's mirror bump path (`source/CONCEPT/composite.C:644`, `source/CONCEPT/composite.C:703`). The API is public and still used in the tree-adjacent codebase, e.g. `test/AssignBondOrderProcessor_test2.C:960`.

If any atom in a rooted `System` is toggled through `setSelected(true/false)`, `ContainerRow::selection_count` remains unchanged. A later `deselect()` can saturate the row at zero via `bump_selection_up()` (`source/KERNEL/moleculeStore.C:474`, `source/KERNEL/moleculeStore.C:485`), hiding the earlier missed count rather than repairing it.

Suggested fix: override `Composite::setSelected(bool)` and route through `select()` / `deselect()` for Composite objects, or make `Selectable::setSelected()` non-virtual/internal and remove it from Composite-facing APIs. Add an atom-level test that uses `setSelected(true)` after materialisation and asserts row counts.

### MEDIUM -- `MoleculeRole` currently misclassifies solvent molecules as `SMALL_MOLECULE`

`MoleculeRole` includes `SOLVENT`, `WATER`, and `ION` (`include/BALL/KERNEL/containerRole.h:42`), and legacy solvent identity for a plain `Molecule` is represented by `Molecule::IS_SOLVENT` (`include/BALL/KERNEL/molecule.h:42`). Existing predicates treat that bit as the solvent truth (`source/KERNEL/standardPredicates.C:148`, `include/BALL/KERNEL/standardPredicates.h:242`). HCP-1's role accessor ignores it and maps every `ContainerKind::MOLECULE` to `MoleculeRole::SMALL_MOLECULE` (`source/KERNEL/moleculeStore.C:636`, `source/KERNEL/moleculeStore.C:642`).

That means a solvated system produced by code that marks solvent molecules, e.g. `source/MOLMEC/COMMON/periodicBoundary.C:458`, gets a row role that contradicts the v0 source of truth. This is separate from the accepted generic property-mirror deferral: this accessor exposes a role enum that already has solvent roles but cannot currently return them.

Suggested fix: either derive `ContainerKind::MOLECULE` role from the v0 `Molecule::IS_SOLVENT` bit during materialise/resync and store it as a role payload, or explicitly make `container_molecule_role_()` return `UNKNOWN` for plain molecules until HCP-2 can represent solvent/ligand/ion without lying. Add a solvent molecule role test.

### LOW -- `SecondaryStructure::set()` / assignment does not copy `type_`

This is not only a mirror issue, but it affects the HCP-1 scalar surface because SS kind derives from `type_`. `SecondaryStructure::set()` delegates to `AtomContainer::set()` and never copies `secondary_structure.type_` (`source/KERNEL/secondaryStructure.C:69`). Therefore assignment cannot update either v0 `type_` or the mirrored SS type. The HCP-1 setter `setType()` is wired (`include/BALL/KERNEL/secondaryStructure.h:153`), but assignment remains semantically broken.

Suggested fix: copy `type_` in `SecondaryStructure::set()` and call `mirrorResyncScalars_()` afterward. Update `swap()` too if SS type swap semantics are expected.

## Confirmed Sound

- `ContainerPayload` remains 8 B with `ResidueKind`, enforced by `static_assert(sizeof(ContainerPayload) == 8)` at `include/BALL/KERNEL/_moleculeStoreInternal.h:532`. `ContainerRow` stays capped at `<= 56 B` (`include/BALL/KERNEL/_moleculeStoreInternal.h:533`). HCP-1 adds one row binding pointer/index to `AtomContainer` (`include/BALL/KERNEL/atomContainer.h:423`) but no per-atom growth.
- The shared scalar write dispatch is correctly centralized for materialisation and setter resync: `detail::writeContainerScalars_()` is used both by `System::materialiseContainer_()` (`source/KERNEL/system.C:278`) and `AtomContainer::mirrorResyncScalars_()` (`source/KERNEL/atomContainer.C:72`, `source/KERNEL/atomContainer.C:76`).
- Explicit post-root scalar setters are wired for name/id/insertion-code/SS type: `AtomContainer::setName()` (`source/KERNEL/atomContainer.C:229`), `Protein::setID()` (`source/KERNEL/protein.C:199`), `NucleicAcid::setID()` (`source/KERNEL/nucleicAcid.C:129`), `Residue::setID()` / insertion setters (`source/KERNEL/residue.C:426`, `source/KERNEL/residue.C:437`, `source/KERNEL/residue.C:451`), `Nucleotide::setID()` / insertion setters (`source/KERNEL/nucleotide.C:128`, `source/KERNEL/nucleotide.C:139`, `source/KERNEL/nucleotide.C:153`), and `SecondaryStructure::setType()` (`include/BALL/KERNEL/secondaryStructure.h:153`).
- The clear/swap "most-derived call wins" reasoning is sound for the implemented fields. Base `AtomContainer::clear()` / `swap()` may write an intermediate row state (`source/KERNEL/atomContainer.C:146`, `source/KERNEL/atomContainer.C:217`), but derived `clear()` / `swap()` methods re-sync again after their own fields are reset/swapped, e.g. `Residue::clear()` / `swap()` (`source/KERNEL/residue.C:89`, `source/KERNEL/residue.C:143`) and `Protein::clear()` / `swap()` (`source/KERNEL/protein.C:36`, `source/KERNEL/protein.C:82`). The redundant writes are harmless as long as no observer reads between calls on the same thread.
- Selection leaf recursion avoids double counting for ordinary `select()` / `deselect()`: containers recurse to atoms, and only atom leaves bump (`source/CONCEPT/composite.C:648`, `source/CONCEPT/composite.C:670`, `source/CONCEPT/composite.C:709`, `source/CONCEPT/composite.C:725`). The existing HCP-1b.3 test covers partial select, container select, and subtree deselect (`test/HierarchyParity_test.C:1007`).
- Materialisation seeding of pre-root selected atoms is in the right place and does not double count on idempotent re-entry: selected atom children seed once in `materialiseContainer_()` (`source/KERNEL/system.C:280`, `source/KERNEL/system.C:290`), and an already-bound container returns early (`source/KERNEL/system.C:266`).
- `ResidueKind` derivation priority `WATER > AMINO_ACID > NON_STANDARD` is defensible for the current v0 ambiguity: water should dominate if legacy data accidentally carries both water and amino-acid/non-standard bits. Post-root mutation of those identity bits is not re-reported here because generic container `setProperty` / `clearProperty` mirroring is explicitly accepted as deferred.

## H2d Risk Summary

The upcoming randomized parity sweep will fail if it mutates row-bound objects through assignment/persistentRead, selection `setSelected()`, or topology operations involving already selected atoms/subtrees. The scalar setter path itself is mostly clean; the main blocker is that HCP-1 treats selection counts as an event log rather than a derived invariant maintained across both selection and topology changes.

---

## Dispositions (HCP1R applied 2026-05-21)

**FIXED:**
- **CRITICAL (selection_count drifts across topology moves) + HIGH (setSelected
  bypasses the mirror)** — both fixed by making `container_selection_count_`
  **compute-on-read**: it walks the row's subtree edges and reads the v0 atom
  selection via `back_ptr_(atom_idx)->isSelected()`. Correct-by-construction
  for EVERY mutation path (select/deselect, setSelected, topology moves,
  clear) regardless of how/where selection was set. Removed the fragile
  incremental machinery (Composite::mirrorAtomSelection_, the select_/
  deselect_ bumps, MoleculeStore::container_bump_selection_, the materialise
  seed). O(subtree) read; no production consumer reads it yet (dual existence)
  and the collapse reworks selection at HCP-2. (H1a's ContainerRow::
  selection_count + ContainerTable::bump_selection_up + their unit test are
  left intact but are no longer the getter's source.)
- **MEDIUM (MoleculeRole misclassifies solvent as SMALL_MOLECULE)** — a plain
  Molecule now returns `MoleculeRole::UNKNOWN` (the IS_SOLVENT bit is a
  container property whose mirror is deferred; returning UNKNOWN avoids
  contradicting the v0 truth). Precise SMALL_MOLECULE/SOLVENT/WATER/LIGAND/ION
  lands with the container-property mirror (HCP-2).
- **LOW (SecondaryStructure::set drops type_)** — pre-existing v0 assignment
  bug fixed (copies type_). (Row mirror for assignment-onto-rooted is the
  deferred full-replacement carry-over, below.)

**DEFERRED (carry-over, documented):**
- **HIGH (set()/operator= and persistentRead() leave row scalars stale on an
  already-rooted object)** — these are DEEP (deep=true) full-subtree-
  replacement operations: on a rooted object they replace BOTH children
  (topology) AND scalars, so a scalar-only resync would be a misleading
  half-fix. They join the **full-subtree-replacement-on-rooted** carry-over
  family with cross-store container move (KR1 HIGH-1) and insertParent/replace
  materialise-new-member. The COMMON usage -- build on an unrooted object,
  then root -- is correct (materialise captures the final state). H2d must not
  randomize assignment/persistentRead onto already-rooted objects until that
  carry-over lands (HCP-2 / a dedicated full-replacement-mirror step).

Confirmed-sound items from KR1R stand (payload 8 B, shared scalar dispatch,
explicit setters wired, clear/swap most-derived-wins, materialise idempotence,
ResidueKind priority).

## HCP1Rb confirmation

Verdict: GO — HCP-1 is ready for H2d + HCP-2.

- **ADDRESSED: CRITICAL selection drift across topology + HIGH setSelected bypass.** `container_selection_count_` now derives the selected-atom cardinality by walking the container-table subtree edges and reading v0 atom truth through `back_ptr_[atom_idx]->isSelected()`, which is the `Selectable::selected_` state written by atom `select()` and by `setSelected()`. The former incremental mirror hooks are gone (`Composite::mirrorAtomSelection_`, select_/deselect_ row bumps, `container_bump_selection_`, materialise selection seed), so the prior event-log drift no longer applies. Freed/root-invalid rows return 0, atom back-pointers are null/bounds guarded, and O(subtree) read cost is acceptable during dual existence.
- **ADDRESSED: MEDIUM MoleculeRole solvent.** Plain `Molecule` now returns `MoleculeRole::UNKNOWN`; this is acceptable as a non-lying placeholder until the HCP-2 property/role mirror can distinguish small molecule, solvent, water, ion, and ligand.
- **ADDRESSED: LOW SecondaryStructure::set type_.** `SecondaryStructure::set()` now copies `type_`, fixing the v0 assignment bug; rooted-row resync for assignment remains covered by the carry-over below.
- **ACCEPTED-DEFERRAL: HIGH set()/operator=/persistentRead on rooted objects.** Deferring these as full-subtree-replacement-on-rooted is sound for HCP-1: the operations are deep replacement surfaces, scalar-only resync would be partial, and the common build-then-root path is still correct because materialisation captures final state. H2d should keep rooted assignment/persistentRead out of randomized parity until the full replacement mirror lands.
- **ADDRESSED: HCP-1b.3 selection check.** The parity test now exercises derived selection counts across atom select/deselect and subtree select/deselect, and `HierarchyParity_test` passes in both `build-core` and `build`.
- **No new blocker found.** The only note is editorial: the derived-count comments/docs still mention the atom-selection bitmap in one sentence, but the implementation and nearby comments correctly use v0 `back_ptr_->isSelected()` as the current source of truth; this is not a behavioral issue.
