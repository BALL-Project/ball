# V22-H4-ITERATOR-CONSUMER-AUDIT — Iterator-consumer migration ledger

**Status:** Audit ledger for H4 commit 4a (per V22-H4-DESIGN.md R7,
D-H4.12 R3).
**Authored:** 2026-06-04.
**Scope:** non-VIEW, non-PYTHON production + test code.

D-H4.12 R3 mandates call-site migration (NOT compatibility proxy)
for the molecular-Composite-backed iterators. This ledger
enumerates the current surface so commit 4b's per-site edits land
against a frozen target.

## Audit predicate scope (run at HEAD `abcf70082`)

Five greps over `source/` + `test/` + `include/`, excluding
`VIEW/` and `PYTHON/`:

### (A) Deref-to-reference (the breaking semantic change)

```
rg -tcpp '(Atom|Bond|Residue|Chain|Molecule|Protein|Fragment|System)\s*[&*]\s*[a-zA-Z_]+\s*=\s*\*\s*[a-zA-Z_]+' source test
```

**Hits: 61**

These compile against the v0 reference-yielding iterator
(`*it → Atom&`), but post-H4 the iterator yields a 24 B handle by
VALUE (`*it → Atom`). Each site is a call-site edit:

```cpp
// Before H4
Atom& a = *it; a.setCharge(1);

// After H4
Atom a = *it;  a.setCharge(1);   // Atom is now a handle; copy is cheap
```

The handle's `setCharge` mutates the underlying store row through
its bound `{store, idx, generation}`. Semantics survive because
the handle is reference-by-bind, not by-storage. The 61 sites are
mostly in STRUCTURE / FORMAT / MOLMEC tests + production
processors.

### (B) Address-of-iterator (`&*it`)

```
rg -tcpp '&\*\s*[a-zA-Z_]+_?it\b|&\*\s*it\b' source test
```

**Hits: 466**

`&*it` taking the address of a handle-by-value yields a dangling
pointer post-H4. Each site is one of:

  (B.1) **Migration to handle-by-value**: re-form the consumer to
        carry the handle (24 B copy) instead of an Atom*.
  (B.2) **Use sid as the identity token** (Atom -> StableId for
        the handle case via `.getStoreIndex()` + `store.stable_id()`
        when the consumer needs a persistent reference key).
  (B.3) **Use bond-CSR bond_idx + `getBond(i)` -> BondHandle** for
        bond-from-atom resolution patterns.

Per the per-cluster strategy of D-H4.12 R3, commit 4b sub-divides
the 466 sites by category and applies the per-pattern rewrite.

### (C) Iterator API surface (raw symbol count)

```
rg -tcpp 'AtomIterator|MoleculeIterator|AtomContainerIterator|PDBAtomIterator|ResidueIterator|ChainIterator|FragmentIterator|ProteinIterator|SecondaryStructureIterator|NucleotideIterator|NucleicAcidIterator' source test include
```

**Hits: 1164 / 386 files.**

The iterator type symbols survive as `using`-aliases for the
filtered handle iterators post-H4 (per D-H4.12 R3). The 1164
symbol references do NOT each need an edit — only the (A) + (B)
patterns require call-site edits. Type-name appearances in
function signatures stay because of the `using` alias contract.

## Commit-4b migration plan

Per-cluster sub-commits within commit 4b (D-H4.12 R3's "may sub-
divide if scope demands"):

  4b.1 KERNEL — iterator declarations + alias headers
        (atomIterator.h, moleculeIterator.h, etc.) become
        `using` for the new filtered-handle iterators
  4b.2 STRUCTURE — 61 (A) + ~200 (B) site rewrites
  4b.3 FORMAT — ~150 (B) sites; aligns with commit 5b's
        FORMAT construction rewrite
  4b.4 MOLMEC + QSAR + SCORING + DOCKING + SOLVATION + NMR + ENERGY
        — remaining (A) + (B) sites
  4b.5 KERNEL + test surface — final cleanup

Each 4b.* commit follows the D-H4.9 per-commit gate (build + ctest
+ HandleKeyLeakGate + AssignBondOrder_bench within 5%).

## Tolerance band (per CR-R5 Q4 answer)

The (A) count = 61 today; CR-R5 noted Codex's quick grep gave 159
and asked whether a ±20% tolerance band should apply or strict
count match. R5's answer: **regenerate at commit-prep time**; the
absolute count drift between today (61) and commit-4b-prep is
governed by the predicate, not the ledger.

The drop from Codex's 159 to today's 61 is explained by H3d's
sweep landing the bulk of `Atom& a = *it` → handle-friendly
forms (the v2.2 H3d closing-CR clusters already moved STRUCTURE
processors off pointer-identity at the storage level). The
remaining 61 are local-variable copies that need the
type-spelling edit only.

## Reproduction

Run the predicates above at HEAD; the ledger refreshes against
that snapshot. If the counts drift by more than ±30% from the
recorded baseline, re-run the audit before commit 4b.

---

*Authored 2026-06-04 as the H4 commit 4a deliverable per D-H4.12.*
