# V22-H4-DESIGN — The Flip (v0 retirement + canonical-name reconciliation)

**Status:** DRAFT pre-DR. Authored 2026-06-04 immediately post-H3d
closing-CR round 4 GO at `4e71e63f4`.
**Predecessor:** H3d (closed); see `V22-H3d-DESIGN.md` + D-H3d.CLOSE.
**Companion:** `V2X-ROADMAP.md` §2 H4 row.
**DR needed:** YES — Codex H4-DR before any H4 code lands.

---

## Why H4

Through H1a–H3d the v0 `Composite`-derived `Atom`/`Bond`/molecular
container objects have been the source of truth, with the
MoleculeStore table mirroring them under the dual-existence
invariant (D60). H4 inverts that:

- `Atom` is no longer `: public Composite, public PropertyManager,
  public Selectable`. The polymorphic heavy object becomes a
  trivial value handle.
- The molecular container objects' inline tree state is deleted;
  the container table (H1a + HCP-1 role columns) becomes the sole
  source of truth.
- The `HierarchyParity_test` is retired — there is no v0 side to
  cross-check.
- `sizeof(BALL::Atom) ≤ 32 B` and `sizeof(BALL::Bond) ≤ 32 B` are
  asserted on every supported compiler (D13 acceptance condition
  #2, scheduled for H8 verification but achievable here).
- Canonical names reconcile: `AtomHandle`/`BondHandle`/`*Handle`
  rename back to `Atom`/`Bond`/`...` per the D63 audit; reverse
  aliases let consumer code that still spelled the handle name
  continue to compile.

H4 is the breaking commit point of v2.2. Every consumer cluster
(KERNEL, STRUCTURE, MOLMEC, QSAR, SCORING, DOCKING, SOLVATION, NMR,
ENERGY) MUST already be migrated onto the handle API before H4 — H1a
through H3d together ensured that.

## Decisions

### D-H4.1 — Atomic-class deletion order (KERNEL-bottom-up)

The v0 classes that participate in `: public Composite` are deleted
bottom-up, smallest blast radius first, each as its own atomic commit
with full ctest gate:

1. **Atom** (the leaf). After H3a's StableId migration there is no
   stored `Atom*` outside the dual-existence bridge; deleting the
   class collapses every `Atom : Composite` site simultaneously.
2. **Bond**. After H5 (the dedicated Bond unify phase, scheduled
   after H4) but the v0 `Bond` class itself is deleted here in H4
   alongside `Atom`; H5 then designs the bond column layout and
   removes `Atom::bond_[]` / `number_of_bonds_`.
3. **Atomic containers** in dependency order: `Fragment`, `Residue`,
   `Chain`, `Protein`, `Nucleotide`, `NucleicAcid`,
   `SecondaryStructure`, `Molecule`, `System`. Each is replaced by
   its corresponding `*Handle` reverse-aliased to the canonical
   name.

Rationale: per-class atomic commits keep the tree green at every
step. A single mega-commit deleting all v0 classes at once is
strictly forbidden — there would be no way to bisect a regression.

### D-H4.2 — Reverse-alias strategy

For each v0 class deleted, the corresponding `*Handle` is renamed to
the canonical name (D63 audit), and the OLD `*Handle` name is kept
as a reverse `using` alias for one minor cycle. Example:

```cpp
// Before H4: kernel/atomHandle.h
class BALL_EXPORT AtomHandle { ... };

// After H4: kernel/atom.h
class BALL_EXPORT Atom { ... };          // was AtomHandle
using AtomHandle = Atom;                 // reverse alias, removed at v2.3
```

The reverse alias prevents source churn in downstream code that
adopted `AtomHandle` during H3a–H3d. The alias is REMOVED at the
v2.3 cycle open (V2X-ROADMAP §4a).

### D-H4.3 — HierarchyParity_test retirement

Once any v0 class is deleted, the parity test's premise dissolves:
there is no v0 side to compare against the store table. The test is
removed wholesale at the START of H4 (commit 1) BEFORE the first v0
class deletion, so the test is not left in a half-broken state
across the deletion sequence.

`HierarchyParity_test` is replaced with handle-only invariant tests
that already exist (`AtomHandle_test`, `BondHandle_test`,
`ContainerHandle_test`). No new test surface is added in H4 — H3a's
gate (`HandleKeyLeakGate_test`) continues to protect against
regressions where a migrated consumer would re-stash an Atom*.

### D-H4.4 — Dual-existence bridge removal

The dual-existence bridge methods (`MoleculeStore::back_ptr(i)`,
`bond_back_ptr(i)`, `bond_idx_of(const Bond*)`, `bond_sid_of(const
Bond*)`) survive H4 ONLY for the duration of the per-class
deletion sequence; they are dropped at the END of H4 (final commit)
when no v0 class remains. The bridge has no role post-H4 since
there is no v0 Atom*/Bond* anywhere in the public surface.

### D-H4.5 — Inline tree state deletion

For each container class:

- `Composite::child_`, `parent_`, `next_sibling_`, `previous_sibling_`
  inline pointer state is DELETED at the moment the class drops its
  `: public Composite` base.
- The container table's `ContainerRow` (parent_idx + ChildRef
  ordered edges) is sole source.
- Iteration moves to handle-yielding `AtomIterator` (already in H2c
  + HCP-2) over `ChildRef`; no remaining v0-tree iteration path.

### D-H4.6 — `PropertyManager` + `Selectable` base deletion

When `Atom`/`Bond`/container drop their `: public PropertyManager,
public Selectable`:

- Property storage moves wholesale to the store's `property_columns_`
  + sparse bag (already in v2.1 P1; consumers already migrated to
  the value-semantics `getProperty/setProperty/hasProperty/
  clearProperty` shim in H3a).
- Selection storage moves to the store's `selected_bits_` atomic
  bitmap (already in v2.1 P1; consumers already migrated to
  `select()/deselect()/isSelected()` on the handle).
- The mutable-reference `NamedProperty&` / `BitVector&` surface is
  removed per D49/D52 (controlled break; consumers have been
  migrated to the visitor `eachProperty(visitor)` + `propertyNames()`
  in H3a).

### D-H4.7 — `sizeof(Atom) ≤ 32 B` enforcement

Post-deletion, add a static assert in `include/BALL/KERNEL/atom.h`:

```cpp
static_assert(sizeof(BALL::Atom) <= 32,
  "D13: sizeof(BALL::Atom) must be ≤ 32 B post-H4 flip");
```

And in `include/BALL/KERNEL/bond.h`:

```cpp
static_assert(sizeof(BALL::Bond) <= 32,
  "D13: sizeof(BALL::Bond) must be ≤ 32 B post-H4 flip");
```

These compile-fail if the handle ever regresses past the budget.
H8 adds the cross-compiler verification + the 100k-atom store
footprint test; the per-class static_asserts here catch the local
regression at the file that owns the type.

### D-H4.8 — V21-COMPOSITEASATOM-REMOVAL closure

The `detail::compositeAsAtom_` single-dynamic-cast helper (v2.1
D41.1's RTTI consolidation point) becomes dead code at the moment
`Atom` no longer inherits from `Composite`. The CI grep gate
(`D41.1`-named) is removed alongside the helper. The 30 VIEW
RTTI sites that still call `compositeAsAtom_` are out of scope here
(they are H7's responsibility); CORE_ONLY closure is sufficient at
H4.

### D-H4.9 — Atomic commit cadence + tree-green gating

Each commit in H4 MUST:
1. Build clean on macOS + the CORE_ONLY target.
2. Run the full ctest (currently 289/289 GREEN at HEAD).
3. Run HandleKeyLeakGate_test (no new leak introductions).
4. Run AssignBondOrder_bench within 5% of D-H3d.D.BASELINE (no
   bond-order regression from the deletion).
5. Be reviewable as a self-contained class deletion (one v0 class
   per commit OR one tightly-coupled pair when an alias forces
   them).

### D-H4.10 — Codex DR before any code

H4 is the v2.2 cycle's most consequential refactor. Codex DR
required BEFORE the first deletion commit, NOT after. The DR
prompt MUST stress-test:
- Deletion-order correctness (D-H4.1)
- Reverse-alias hygiene (D-H4.2)
- HierarchyParity_test retirement timing (D-H4.3)
- Dual-existence bridge removal (D-H4.4)
- `sizeof` budget assertions (D-H4.7)
- Any consumer site that survived H3d still referencing a v0 spelling

## Commit plan

| # | Commit | Description |
|---|---|---|
| 0 | DR prep | Codex H4-DR + iterate to GO (≤3 rounds) |
| 1 | Retire parity | Drop `HierarchyParity_test` entry + skeleton |
| 2 | Delete Atom v0 | `Atom : Composite` → `using Atom = AtomHandle;`-style canonical rename; reverse alias `using AtomHandle = Atom` |
| 3 | Delete Bond v0 | same shape, `using BondHandle = Bond` reverse alias |
| 4 | Delete Fragment v0 | container deletion sequence step 1 |
| 5 | Delete Residue v0 | step 2 |
| 6 | Delete Chain v0 | step 3 |
| 7 | Delete Protein v0 | step 4 |
| 8 | Delete Nucleotide v0 | step 5 |
| 9 | Delete NucleicAcid v0 | step 6 |
| 10 | Delete SecondaryStructure v0 | step 7 |
| 11 | Delete Molecule v0 | step 8 |
| 12 | Delete System v0 | step 9 |
| 13 | Drop dual-existence bridge | `back_ptr`/`bond_back_ptr`/`bond_idx_of`/`bond_sid_of` removed |
| 14 | sizeof asserts | static_asserts in atom.h + bond.h |
| 15 | V21-COMPOSITEASATOM-REMOVAL | `compositeAsAtom_` + D41.1 gate retired |
| 16 | H4 close-CR | Codex H4-close-review; iterate to GO |

Estimate: 16 commits + 1 design DR + 1 closing CR = ~18 commits
across the H4 cycle.

## Out of scope for H4

- **VIEW Atom-RTTI sites (54)**: V21-VIEW-RTTI / D37 — H7's job.
- **Bond column layout / `Atom::bond_[]` removal**: H5 designs the
  bond column representation. H4 deletes the v0 `Bond` class but
  leaves the bond storage on AtomHandle's bond CSR view (the
  existing `bond_stable_ids_` / `bond_back_ptr_`) untouched.
- **JSON schema v2 freeze**: H6b's job. H4 keeps reading/writing the
  current schema; the v0 deletion doesn't change wire format yet.
- **MSVC + vcpkg CI**: H7's job. Bring-up may start in parallel
  once H4 commit 1 lands and the build shape stabilises.
- **`sizeof ≤ 32 B` cross-compiler verification**: H8's job. H4
  asserts the budget on the build compiler; H8 asserts it on every
  supported compiler including MSVC.
- **100k-atom store-footprint test**: H8's job. H4 closes the
  per-class sizeof; H8 closes the per-atom footprint.

## Open questions for Codex H4-DR

1. **Order of container deletions** (D-H4.1 step 3). Is the order
   "Fragment → Residue → Chain → ... → Molecule → System" the right
   one, or should it be inverted? The argument for bottom-up: each
   step's container parent class is still present so the children
   compile against an unchanged parent. The argument for top-down:
   System holds the orphan store + container table, so deleting it
   last means the store survives the deletion sequence.
2. **Reverse-alias lifetime** (D-H4.2). Is one minor cycle (until
   v2.3) right, or too short? Downstream code that adopted
   `AtomHandle` between H3a and v2.2.0 release will need to be
   updated by v2.3.
3. **HierarchyParity_test timing** (D-H4.3). Retire at commit 1 OR
   keep it until the last v0 class is gone? Argument for early
   retire: avoids a per-commit failure as v0 classes are deleted.
   Argument for late retire: catches accidental partial-deletion
   regressions during the sequence.
4. **Bridge-removal timing** (D-H4.4). The plan has bridge removal
   at commit 13 (after all v0 classes are gone). Is there a v0
   spelling that depends on the bridge surviving longer (e.g.
   external code that reads bonds via `bond_back_ptr` and that we
   want to keep working through the v2.2 cycle)?
5. **`sizeof` budget** (D-H4.7). Is 32 B the right number, or
   should the H4 budget be looser (say 40 B) and the 32 B target
   live only at H8 where cross-compiler verification happens?
6. **AssignBondOrder regression** (D-H4.9 gate 4). 5% threshold
   was set at H3d.D phase 0 (D-H3d.D.BASELINE). Should H4 tighten
   it (since H4 deletes overhead, perf should improve) or keep at
   5% (allow noise)?
7. **Atom v0 deletion atomicity** (D-H4.1 step 1). Can `Atom` be
   deleted in one commit, or does it need a multi-commit fan-out
   (deleting subset of bases in successive commits)?
8. **What about `AtomContainer`?** It's the base class of
   `Fragment`/`Molecule` etc., not in the deletion sequence above.
   Does it stay (as a handle reverse-alias) or does it disappear?

DR must return findings on each, severity-tagged.

---

*Authored 2026-06-04 as pre-DR draft. Codex H4-DR is the gate to
implementation.*
