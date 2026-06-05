# V22-H4-ATOMCONTAINER-AUDIT — AtomContainer collapse inventory

**Status:** Audit ledger for H4 commit 7a (per V22-H4-DESIGN.md R7,
D-H4.15 R3).
**Authored:** 2026-06-04.
**Scope:** non-VIEW, non-PYTHON production + test code.

D-H4.15 R3 mandates the public `AtomContainer` API moves to
role-handle members + `StructureQuery::` free functions before
the heavy base class disappears. This ledger inventories the
public surface + its consumers so commit 7b's per-method-cluster
migration lands against a frozen target.

## Audit predicate scope (run at HEAD `7d35f0c35`)

### (A) Public method surface on AtomContainer

```
rg -n '^\s+(virtual\s+|static\s+)?[A-Za-z]' include/BALL/KERNEL/atomContainer.h
| filter to public method declarations (exclude // comments,
                                        private:, protected:)
```

**Method declarations: 59.**

### (B) Total AtomContainer references in code

```
rg -tcpp 'AtomContainer\b' source test include
| exclude source/VIEW, source/PYTHON
```

**Total references: 1439 across 386 files.**

### (C) Hits by directory (top 10)

| Directory | Files mentioning AtomContainer |
|---|---|
| `test/` | 31 |
| `source/STRUCTURE/` | 16 |
| `include/BALL/KERNEL/` | 16 |
| `include/BALL/STRUCTURE/` | 14 |
| `source/QSAR/` | 11 |
| `include/BALL/QSAR/` | 11 |
| `source/SCORING/FUNCTIONS/` | 9 |
| `include/BALL/SCORING/FUNCTIONS/` | 9 |
| `source/KERNEL/` | 8 |
| `source/FORMAT/` | 8 |

## Public-method classification (the commit 7b destinations)

Per D-H4.15 R3 commit 7b sub-divides the 59 public methods into
9 dependency-ordered clusters. The classification by destination:

| Method cluster | Approx count | Destination | Lands at |
|---|---|---|---|
| **Construction/copy** | 6 | persisted on `System` only (since System is the only surviving owner-class per D-H4.11); per-role wrappers redirect to `StructureQuery::` factories | commit 11 (System base strip) |
| **Parent/child access** | 4 | replaced by `ContainerHandleBase::parent()` + `ContainerHandleBase::children()` already in containerHandle.h | commit 7b.1 |
| **Atom lookup** (`getAtom`, `findAtom`, etc.) | 5 | StructureQuery free functions on role handles | commit 7b.2 |
| **Counts** (`countAtoms`, `countContainers`, etc.) | 7 | members on `ContainerHandleBase` + role wrappers in the handle headers (mostly already shipped in H1b/HCP-2c) | commit 7b.3 |
| **Mutation** (`insert`, `append`, `prepend`, etc.) | 9 | StructureQuery free functions; underlying MoleculeStore mutators already exist (H2b/H2c) | commit 7b.4 |
| **Splice/remove** (`spliceBefore`, `spliceAfter`, `remove`, etc.) | 6 | StructureQuery free functions; underlying mutators in H2b | commit 7b.5 |
| **Predicates** (`hasAtom`, `isProperSubsetOf`, etc.) | 5 | free functions on role handles in StructureQuery::; predicate body operates on handle equality + container_idx walks | commit 7b.6 |
| **Bond application** (`applyToBonds`, etc.) | 4 | bond-CSR walk via the existing handle bond accessors; StructureQuery wrappers | commit 7b.7 |
| **Iterators** (`beginAtom`, `endAtom`, `beginContainer`, etc.) | 8 | role-filtered handle iterators per D-H4.12 R3 (CROSS-LISTED in V22-H4-ITERATOR-CONSUMER-AUDIT.md); the per-method spelling stays via `using` alias | commit 4b.1 (shipped earlier in the commit-4 cluster) |
| **Property behavior** | 3 | inherited from PropertyManager base; the base disappears per D-H4.6 R2 visitor pattern | commit 6 (property persistence) |
| **Mirror hooks** (`mirrorAppendEdge_` etc.) | 2 | private mirror surface; survives until H4 commit 12 (bridge removal) | commit 12 |

**Total: 59 methods.** Commits 7b.1–7b.7 cover 41 methods
(parent/child + lookup + counts + mutation + splice + predicates +
bond application). The remaining 18 are handled by adjacent
commits (4b iterators, 6 property, 11 construction/copy, 12 mirror
hooks).

## Caller-cluster summary (the commit 7b boundary surface)

Per-cluster sub-commits within commit 7b:

  7b.1 KERNEL — parent/child access migration in include/BALL/KERNEL/
  7b.2 STRUCTURE + tests — atom lookup migration
  7b.3 KERNEL + STRUCTURE + QSAR — counts (mostly already shipped)
  7b.4 KERNEL + STRUCTURE + FORMAT — mutation (the largest cluster)
  7b.5 KERNEL + STRUCTURE — splice/remove
  7b.6 KERNEL + STRUCTURE — predicates
  7b.7 STRUCTURE + MOLMEC + SCORING — bond application

Each 7b.* commit follows the D-H4.9 per-commit gate (build + ctest
+ HandleKeyLeakGate + AssignBondOrder_bench within 5%).

## AtomContainer base-class deletion

After commit 7b.* finishes, the heavy AtomContainer base class
disappears in the v0-class-deletion sequence:

  - commit 10a: Fragment/SecondaryStructure drop `: public AtomContainer`
  - commit 10b: Residue/Nucleotide drop the base
  - commit 10c: Chain/Protein/NucleicAcid/Molecule drop the base
  - commit 11: System strips `: public AtomContainer, public Composite`

At this point the `AtomContainer` symbol itself can be deleted as
a class. If callers used `AtomContainer&` in a function signature
they will need to update — that count is rolled into commit 7b's
ledger.

## Reproduction

Run the predicates above at HEAD; the ledger refreshes against that
snapshot. If counts drift by more than ±20% from the recorded
baseline (1439 / 59 / 31-16-16-14...), re-run before commit 7b
lands.

---

*Authored 2026-06-04 as the H4 commit 7a deliverable per D-H4.15 R3.*
