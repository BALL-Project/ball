# V22-H4-PDB-CONSUMER-AUDIT — PDB-symbol migration ledger

**Status:** Audit ledger for H4 commit 5a (per V22-H4-DESIGN.md R7,
D-H4.14 R5).
**Authored:** 2026-06-04.
**Scope:** non-VIEW, non-PYTHON production + test code per the R5
exclusion. VIEW = H7 / D37 boundary; PYTHON = pyBALL v2 rebaseline
per V2X-ROADMAP §4a.

## Audit predicate

Per D-H4.14 R5:

```
rg -tcpp '\bPDBAtom\b|\bPDBAtomIterator\b|\bPDBAtomList\b' include source
```

with `source/VIEW` and `source/PYTHON` EXCLUDED.

**Total raw hits at HEAD `a4aae1ad4`: 654** (across all
non-VIEW non-PYTHON code).

## Five-category breakdown

| Category | Count | Lands at | Notes |
|---|---|---|---|
| (I)   FORMAT construction (PDBFile + HINFile + dockResultFile) | 16 | commit 5b | `new PDBAtom` sites in PDBFileDetails.C:366, HINFile.C:461; `writePDBAtom(PDBAtom*)` overloads in dockResultFile.C:848,853,864 |
| (II)  KERNEL public API surface | 101 | commit 5c | residue.h, chain.h, protein.h, secondaryStructure.h public method declarations + extractors.h type declarations + source/KERNEL/*.C implementations |
| (III) Iterator-traits + extractor type definitions | 24 | commit 5d | PDBAtomIterator.h + extractors.h + extractors.C dynamic_cast filter + iterator typedef definitions |
| (IV)  STRUCTURE/SOLVATION non-FORMAT production | 34 | commit 5e | peptideBuilder.C, sideChainPlacementProcessor.C, disulfidBondProcessor.C, poissonBoltzmann.C, plus other STRUCTURE consumers |
| (V)   APPLICATIONS code | 17 | commit 5e | clip_protein_around_ligand.C, AMBER/files.C, plus other tooling |
| Tests | included in above categories | commits 5b/5c/5d/5e | test/PDBAtom_test.C, test/PDBAtomIterator_test.C, etc. |
| Total | **654** | commits 5a-5e | |

## Commit-5 plan refresh

- **5a (this commit)** — ledger only; no source change.
- **5b** — FORMAT construction (16 sites). PDBFile loader stops
  producing `PDBAtom*` and stamps PDB-origin payload onto atom
  rows via store-slot factories. HINFile + dockResultFile follow
  the same pattern.
- **5c** — KERNEL public API migration (101 sites). Public
  PDBAtom-accessor methods on Residue/Chain/Protein/
  SecondaryStructure become `StructureQuery::` free functions
  filtering on `Atom::hasPDBOrigin()`. Deprecated reverse-aliases
  keep spelling-only consumers compiling through v2.3.
- **5d** — iterator-traits + extractor collapse (24 sites).
  `PDBAtomIterator` becomes `using = FilteredHandleIterator<Atom,
  hasPDBOrigin>`. `PDBAtomList` becomes
  `using = std::vector<Atom>` (hard break enumerated in
  V22-API-BREAK-LEDGER.md).
- **5e** — STRUCTURE/SOLVATION + APPLICATIONS production
  consumers (34 + 17 = 51 sites). Per-site rewrites following the
  cat (II)+(III) replacement patterns.

`PDBAtom` itself is DELETED in commit 8 alongside `Atom : Composite`
per D-H4.14 R5; PDB-origin payload lives on atom-row columns from
HCP-1.

## D-H4.14 R5 patterns

(repeated here so per-site reviewers don't have to cross-reference
the design)

**Pattern A** — `dynamic_cast<PDBAtom*>(&atom)` → predicate:

```cpp
// Before H4:
if (auto* p = dynamic_cast<PDBAtom*>(&atom)) {
  do_pdb_thing(p);
}

// After H4 (commit 5c/5e):
if (atom.hasPDBOrigin()) {
  do_pdb_thing(atom);   // accessors live on the Atom handle directly
}
```

**Pattern B** — `for (auto* p : extractors::PDBAtoms(c))` → by-value:

```cpp
// Before H4:
PDBAtomList list = extractors::PDBAtoms(container);
for (PDBAtom* p : list) p->setOccupancy(0.5);

// After H4 (commit 5d):
auto list = StructureQuery::pdbAtoms(container);   // vector<Atom>
for (Atom a : list) a.setOccupancy(0.5);
```

**Pattern C** — `new PDBAtom()` construction → store-slot factory:

```cpp
// Before H4:
PDBAtom* pa = new PDBAtom;
pa->setSerial(serial);
container.insert(*pa);

// After H4 (commit 5b/5e):
Atom a = container.appendAtom();
a.setPDBSerial(serial);   // accessor exists from HCP-1
```

**Pattern D** — `PDBAtomIterator` callers — unchanged at the type
spelling because the `using` alias keeps it; only `*it` semantics
shift from `Atom&` to `Atom` per the D-H4.12 R3 call-site
migration. Cross-listed in V22-H4-ITERATOR-CONSUMER-AUDIT.md.

## Reproduction

Run the predicate above at HEAD; the ledger refreshes against
that snapshot. If the per-category counts drift by more than ±20%
from the recorded baseline, re-run before commit 5b lands.

---

*Authored 2026-06-04 as the H4 commit 5a deliverable per D-H4.14 R5.*
