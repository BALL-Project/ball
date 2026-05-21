# BALL 2.x — Composite-hierarchy collapse HC1 adversarial review

**Status:** COMPLETE — adversarial review  
**Verdict:** NEEDS-REVISION  
**Reviewer:** Codex CLI 0.128.0  
**id:** HC1

## HC1-1. SOUNDNESS

The data-not-types thesis is mostly viable, but the assessment is too casual about the behavior being relocated.

No reviewed method proves that `Protein`, `Chain`, `Residue`, `Nucleotide`, `SecondaryStructure`, or `PDBAtom` must remain a C++ subtype. The class surfaces are mostly:

- scalar metadata: `Protein::getID` / `setID` in `protein.h:229-237`, `Residue::getID` / insertion-code access in `residue.h:271-302`, `Nucleotide::getID` / insertion-code access in `nucleotide.h:161-192`, `SecondaryStructure::getType` / `setType` in `secondaryStructure.h:149-153`, and PDB atom fields in `PDBAtom.h:229-277`;
- role predicates and flags: residue amino-acid/water/terminal/cyclic flags in `residue.h:48-70` and predicates in `residue.h:361-386`;
- navigation/query conveniences: `Protein::getResidueByID`, terminal lookups, child accessors, and counts in `protein.h:131-257`, `chain.h:142-227`, `secondaryStructure.h:179-219`;
- geometry over named atoms plus ordered neighboring residues: residue torsions in `residue.h:191-219`.

These all relocate to role-aware `Fragment` methods or free functions. The torsion methods do not need a subtype; they need a fragment with role `residue`, residue-kind `amino_acid`, named atoms, and previous/next amino-acid residue lookup. `getFullName()` is likewise string formatting over name, residue id, insertion code, and flags (`residue.h:177-189`).

The hard part is not subtype replacement. It is preserving invariants that the subtype currently implied by construction: a residue-like fragment must have valid residue identity, terminal predicates must only run against a chain-ordered amino-acid sequence, and PDB atoms need PDB-field presence/default semantics. Role-aware accessors must validate role and fail loudly in debug/Python, otherwise the change silently trades compile-time wrong-type errors for runtime wrong-role bugs.

One type-like behavior needs explicit design: `SecondaryStructure` is currently a tree container inserted around residue ranges (`PDBFileDetails.C:166`, `:261`, `:307`). If secondary structure remains an owning `Fragment`, overlapping or annotation-style ranges are awkward. PDB HELIX/SHEET/TURN are better modeled as role `secondary_structure` fragments with span/child references, or as non-owning annotations, not just arbitrary free-nesting. The assessment must decide this before claiming the collapse is mechanically sound.

## HC1-2. FIELD SURVEY ACCURACY

The survey direction is broadly right, but it overclaims.

Corrections:

- RDKit: accurate in spirit. RDKit stores PDB residue data on atoms via `RDKit::AtomPDBResidueInfo`, including atom name, serial, altLoc, residueName, residueNumber, chainId, insertionCode, occupancy, tempFactor, hetero flag, secondaryStructure, and segmentNumber. API: `RDKit::AtomPDBResidueInfo` (https://www.rdkit.org/docs/cppapi/classRDKit_1_1AtomPDBResidueInfo.html).
- OpenMM: accurate. `Topology` is fixed-depth: each `Chain` contains `Residue`s, and each `Residue` contains `Atom`s; types are names/ids, not subclasses. APIs: `openmm.app.Topology`, `Chain`, `Residue` (https://docs.openmm.org/latest/api-python/generated/openmm.app.topology.Chain.html; https://docs.openmm.org/8.0.0/api-python/generated/openmm.app.topology.Topology.html).
- MDAnalysis: “index groups + attribute arrays” is directionally right, but saying “Residue/Segment = grouping + attrs” should not imply there are no residue/segment objects. MDAnalysis has `Residue`, `ResidueGroup`, `Segment`, and `SegmentGroup` topology objects; attributes such as `resids`, `resnames`, and `segids` are topology attributes. API/user guide: https://userguide.mdanalysis.org/1.1.1/advanced_topology.html.
- MDTraj: accurate enough. Fixed topology with `Chain`/`Residue`/`Atom`; residue type helpers such as `is_protein` and `is_water` are computed from names. API: `mdtraj.Topology` / `mdtraj.topology.Residue` (https://mdtraj.org/1.6.2/api/generated/mdtraj.Topology.html).
- BioPython: accurate. Bio.PDB uses SMCRA fixed depth and residue id `(hetfield, resseq, icode)`; `is_aa(residue)` is a function, not a residue subclass. API/tutorial: https://biopython.org/docs/latest/Tutorial/chapter_pdb.html.
- Gemmi: mostly accurate, but “entity enum” understates the model. Gemmi has explicit `Structure`/`Model`/`Chain`/`Residue`/`Atom` objects and also `EntityType` values such as `Polymer`, `NonPolymer`, and `Water`. API/docs: https://gemmi.readthedocs.io/en/stable/mol.html.
- OpenBabel: needs correction. `OBResidue` is not merely metadata; it is a residue side object stored in `OBAtom` and `OBMol`, with atom lists, chain, residue number/name, per-atom HETATM and atom ids. It is still not a protein/residue subclass hierarchy. API: `OpenBabel::OBResidue` (https://openbabel.org/api/2.0.2/classOpenBabel_1_1OBResidue.shtml).
- CDK is a counterexample to the blanket “no mainstream package uses subclassing” claim. CDK has `IAtomContainer` and subinterfaces/classes including `IAminoAcid`, `IBioPolymer`, `IPDBMonomer`, `IPDBPolymer`, `PDBMonomer`, `PDBPolymer`, `Residue`, `Strand`, and `AminoAcid`. API: `IAtomContainer` / `AtomContainer` (https://cdk.github.io/cdk/latest/docs/api/org/openscience/cdk/interfaces/IAtomContainer.html; https://cdk.github.io/cdk/2.3/docs/api/org/openscience/cdk/AtomContainer.html).

Conclusion: the survey supports “BALL is unusually deep for C++ subtype modeling among structural-biology toolkits,” not “no mainstream package uses subclassing.” The stronger field lesson is fixed-depth or side-object residue models, not arbitrary free-nesting.

## HC1-3. MINIMAL NODE MODEL

Keep four node kinds: `System`, `Molecule`, `Fragment`, `Atom` plus `Bond` as an edge.

A three-kind model (`System` + recursive `Fragment` + `Atom`) is tempting, but `Molecule` earns a distinct kind because BALL already has System-level molecule iteration, orphan construction, top-level import/export grouping, solvent/molecule classification, and future pyBALL ergonomics around “molecule” as a first-class object (`molecule.h:120-129`, `:245-247`). Making top-level molecules merely fragments with `role=molecule` makes every System-child query role-dependent and weakens a useful invariant.

Required taxonomy:

- `MoleculeRole`: `protein`, `nucleic_acid`, `polymer`, `small_molecule`, `ligand`, `solvent`, `water`, `ion`, `unknown`.
- `FragmentRole`: `chain`, `residue`, `secondary_structure`, `domain`, `arbitrary_fragment`, `functional_group`, `unknown`.
- `ResidueKind` for `role=residue`: `amino_acid`, `nucleotide`, `water`, `ligand`, `ion`, `nonstandard`, `unknown`.
- `SecondaryStructureKind`: `helix`, `strand`, `turn`, `coil`, `unknown`, plus properties such as helix class, sheet id, strand number, sense.
- `PdbAtomFields` on atoms: serial/preserved serial if needed, altLoc, occupancy, temperature factor, branch/remoteness if retained, record kind ATOM/HETATM, segment id, formal charge text or parsed charge policy.

Depth convention is required for PDB/mmCIF fidelity. The canonical PDB containment should be:

`System -> Molecule -> chain Fragment -> residue Fragment -> Atom`

Secondary structure should either be an annotation/span over residue fragments or a secondary fragment layer whose API still exposes the canonical chain/residue sequence. Free nesting can exist for generic fragments, but PDB import/export must normalize to the canonical depth or provide equivalent role-query invariants.

## HC1-4. SLOTTING

The recommendation to fold into v2.2 is plausible, but only after revision and a go/no-go gate. It is not ready as a blanket roadmap change.

The locked roadmap says v2.2 is already in progress: H1a container table is landed, H1b eight typed handles are landed, and H2 mutation mirror is in progress (`V2X-ROADMAP.md:62-85`). H3 is explicitly the consumer migration, with FORMAT/PDB/naming/residue as the heavy H3c cluster (`V2X-ROADMAP.md:78-80`). That means the best point to avoid double migration is indeed before H3.

But the proposed collapse adds design work that is not just “shrink handles”: role taxonomy, PDB atom field storage, PDB depth convention, secondary-structure representation, migration guide, and field-level round-trip tests. If those are not locked before H3 starts, v2.2 becomes an unbounded architecture change.

Recommendation: fold into v2.2 only as `H1b' / H2b'` with an explicit short design-lock gate. If the gate does not close quickly, stage to v2.3 despite the double migration. A double migration is expensive; an unstable v2.2 public surface is worse.

Compatibility aliases buy little. `using Residue = Fragment` cannot carry role preconditions, overload separation, or method behavior without making `Fragment` absorb every legacy method. H1b already documents that even `using Residue = ResidueHandle` is not source-transparent because of forward declarations, friends, overloads, and specializations (`V22-H1b-DESIGN.md:52-76`). Facades can soften source migration if they are real classes/wrappers with role asserts, but then they preserve most of the handle surface and are not the clean collapse. Use them only as deprecated migration shims, not as the architectural model.

## HC1-5. BLAST RADIUS vs PDB FIDELITY

PDB fidelity is the dominant correctness risk. Blast radius is larger and noisier, but it is detectable by compile failures and clustered migration. PDB regressions can compile cleanly and corrupt scientific data.

Must preserve at least:

- chain id/name and chain ordering (`PDBFileDetails.C:321-330`, writer chain id at `:1578-1580`);
- residue name, sequence id, insertion code, and residue identity map key `(name, chain, sequence, insertion)` (`PDBFileDetails.C:341-363`, `include/BALL/FORMAT/PDBFile.h:635-637`);
- ATOM vs HETATM, residue nonstandard/amino-acid/water classification (`PDBFileDetails.C:721-737`, writer hetero decision at `:1264-1274`);
- atom serial mapping for CONECT (`PDBFileDetails.C:366-368`, `:506-545`, writer atom numbering/map at `:1583-1589`);
- altLoc, occupancy, B-factor/temperature factor (`PDBFileDetails.C:332-338`, `:373-375`, writer dynamic-cast recovery at `:1518-1532`);
- element and formal charge parsing/writing policy (`PDBFileDetails.C:388-404`, `:1534-1546`);
- TER boundaries (`PDBFileDetails.C:1591-1599`);
- HELIX/SHEET/TURN/SSBOND records, including helix class, sheet id/strand number/sense, residue endpoints and insertion codes (`PDBFileDetails.C:687-718`, `:981-997`, `:1824-1947`, `:2128-2177`);
- skipped/raw records currently carried through `PDBInfo` where applicable (`PDBFileGeneral.C:311-317`, writer add-back sections at `PDBFileDetails.C:2017-2029`, `:2043-2095`).

The current implementation is already not byte-faithful for all PDB input. It discards non-selected alternate locations (`PDBFileDetails.C:332-338`), computes defaults for non-PDB atoms (`:1518-1523`), and rewrites many records from parsed structure rather than preserving original columns. So the assessment must stop promising “byte-faithful” unless the new model stores raw record fields and all alternates. The realistic gate is:

1. Golden PDB corpus covering protein, nucleic acid, ligand, water, ions, insertion codes, altLoc, HETATM, TER, CONECT, SSBOND, HELIX/SHEET/TURN, CRYST1, and skipped records.
2. Parse -> write -> parse equivalence on normalized structural fields.
3. Exact-line round-trip only for records BALL explicitly promises to preserve raw.
4. Dedicated regression that PDB atom fields survive without `dynamic_cast<PDBAtom*>`.
5. A negative test that wrong-role fragments fail role-aware residue/PDB accessors in debug/Python.

## HC1-6. ALREADY-LANDED REWORK

What survives:

- H1a’s ordered child-edge model survives. `ChildRef` already discriminates atom vs container and stores ordered children (`_moleculeStoreInternal.h:454-471`); this is exactly what a collapsed container model needs.
- H1a’s common row fields survive: parent, name, children, selection count, generation (`_moleculeStoreInternal.h:493-510`).
- D56 orphan-store concept survives (`V22-H0-ADDENDUM.md:15-40`).
- D57 deterministic ordered edges survive (`V22-H0-ADDENDUM.md:42-69`).
- D59 container property/selection storage survives (`V22-H0-ADDENDUM.md:109-133`).
- H2 mirror architecture is mostly kind-agnostic: mutation ops are append/prepend/insert/remove/splice/swap/clear/property/select over child refs and rows (`V22-H2-DESIGN.md:156-194`). The traversal layer filters by `ContainerKind` today (`V22-H2-DESIGN.md:196-216`), but the machinery can filter by role instead.

What is redone:

- `ContainerKind` must shrink and/or be redefined. The landed enum has eight container kinds (`containerKind.h:40-51`).
- H1b typed handles are substantially redone. The current handle layer declares eight typed handles and kind-specific getters (`containerHandle.h:232-259`), exactly the surface the collapse removes.
- D58 payload must change. Current payload is generic but still keyed by kind-specific meaning: id, insertion code, and ss type (`_moleculeStoreInternal.h:473-491`). Collapse needs role plus role-payload semantics and likely atom PDB columns.
- H2c typed container iterators must become role-filtered iterators.

What is transitional:

- H2’s mirror binding lives on v0 `AtomContainer` (`V22-H2-DESIGN.md:62-94`; `atomContainer.h:401-420`). That is fine during dual existence because the v0 hierarchy still exists through H3. It should not be mistaken for an enduring `AtomContainer` concept.

## HC1-7. HONEST DOWNSIDES

The strongest case against doing this now:

- It removes compile-time domain types and replaces them with runtime role discipline. That is a real regression unless the role API is strict and tested.
- It makes the public break louder than the handle flip. Existing downstream code can reason about `Residue*`, overloads, iterators, and documentation. `Fragment(role=residue)` is more abstract and easier to misuse.
- The field survey actually argues for fixed depth more than arbitrary free-nesting. OpenMM, MDTraj, BioPython, and Gemmi all keep named chain/residue/atom levels. BALL should not adopt a “generic graph of fragments” without canonical structural conventions.
- PDB/mmCIF import/export is science-critical and already complex. Rewriting it while H2 dual-existence and H3 migration are active compounds risk.
- The current roadmap has a locked v2.2 success criterion: delete Composite inheritance and hit D13 memory gates (`V2X-ROADMAP.md:83-85`, `:209-210`). A hierarchy collapse is attractive, but it is not required for D13 if eight store-backed handle kinds remain.
- Compatibility facades either do not help enough (`using` aliases) or preserve the old surface enough to reduce the benefit.
- Maintainers may reasonably choose typed handles as the safer 2.x landing point, then revisit role collapse after v2.2 proves the flat store and JSON schema.

## HC1-8. VERDICT

The assessment is directionally sound but not ready to fold into the locked plan.

Recommended decision:

- **Collapse:** yes, if the maintainer accepts a larger public break and locks the missing role/depth/PDB design immediately.
- **Minimal model:** four node kinds: `System`, `Molecule`, `Fragment`, `Atom`; `Bond` stays an edge. Keep `Molecule` distinct.
- **Roles:** add explicit molecule, fragment, residue-kind, secondary-structure-kind, and PDB atom field taxonomy. Do not rely on ad hoc string properties for core PDB identity.
- **Depth:** require canonical PDB depth `Molecule -> chain Fragment -> residue Fragment -> Atom`; model secondary structure as role/span or guarantee role queries see through any SS nesting.
- **Slotting:** fold into v2.2 only with a short `H1b'` design-lock gate before H3. If role/depth/PDB-field decisions slip, stage to v2.3.
- **Test gate:** PDB structural round-trip corpus plus debug/Python wrong-role assertions and JSON schema validation for `kind + role + role_payload`.

## Punch List Before Lock

1. Revise the field survey to remove the “no mainstream package uses subclassing” overclaim; add CDK as a real counterexample and correct OpenBabel/MDAnalysis wording.
2. Replace “byte-faithful PDB round-trip” with a precise promise: normalized structural equivalence plus exact preservation only for explicitly raw-stored records, unless raw PDB lines/all alternates become part of the model.
3. Lock the role taxonomy and decide which fields are typed columns versus sparse properties.
4. Decide secondary-structure representation: owning fragment layer versus non-owning span/annotation. Do not leave this to FORMAT migration.
5. Keep `Molecule` as a distinct kind unless the roadmap explicitly accepts losing System-child molecule invariants.
6. Amend `V2X-ROADMAP.md` with an `H1b'` collapse design gate before H3, not a silent rewrite of H3.
7. Amend `V22-H1b-DESIGN.md`: eight typed handles are superseded by `Molecule`/`Fragment` role-aware handles if the gate passes.
8. Amend `V22-H2-DESIGN.md`: traversal filters by role; mutation mirror remains row/edge based; v0 `AtomContainer` binding is transitional only.
9. Add a PDB/structural fidelity gate to H8 alongside D13, with the corpus listed in HC1-5.
10. Add migration-guide examples for `Residue`, `Chain`, `PDBAtom`, `SecondaryStructure`, terminal/torsion queries, and typed iterators.

## HC1b confirmation

**Verdict: AGREE.** The collapse plan is sound and ready to fold into the
2.x plan with the `H1b'` gate. This is planning convergence, not code.

1. **ADDRESSED** — field survey overclaim removed; CDK added as a real
   counterexample; OpenBabel/MDAnalysis wording corrected
   (`V2X-HIERARCHY-COLLAPSE.md` §2).
2. **ADDRESSED** — PDB fidelity promise is normalized structural equivalence
   plus exact raw preservation only where BALL explicitly stores raw records
   (`V2X-HIERARCHY-COLLAPSE.md` §4, §6).
3. **ADDRESSED** — role taxonomy is locked, including Molecule/Fragment/
   Residue/SecondaryStructure roles and PDB atom fields, with typed-column vs
   sparse-property guidance (`V2X-HIERARCHY-COLLAPSE.md` §2a).
4. **ADDRESSED** — secondary structure is decided as a span/annotation
   `Fragment`, not deferred to FORMAT migration
   (`V2X-HIERARCHY-COLLAPSE.md` §2a, §6).
5. **ADDRESSED** — `Molecule` remains a distinct kind with System-child and
   import/export invariants preserved (`V2X-HIERARCHY-COLLAPSE.md` §2a, §4).
6. **ADDRESSED** — roadmap has an explicit pre-H3 `H1b'` collapse design
   gate, with v2.3 staging if the gate slips (`V2X-ROADMAP.md` §2;
   `V2X-HIERARCHY-COLLAPSE.md` §5).
7. **ADDRESSED** — H1b design now notes the 8 typed handles are superseded
   by `Molecule`/`Fragment` role-aware handles if `H1b'` passes
   (`V22-H1b-DESIGN.md` supersession notice).
8. **ADDRESSED** — H2 design now states traversal filters by role, mirror
   machinery remains row/edge-based, and the v0 `AtomContainer` binding is
   transitional (`V22-H2-DESIGN.md` HC1 note).
9. **ADDRESSED** — H8 has a PDB/structural fidelity gate alongside D13, with
   the corpus and checks called out (`V2X-ROADMAP.md` §2;
   `V2X-HIERARCHY-COLLAPSE.md` §6).
10. **ADDRESSED** — migration-guide coverage is called for across the
    affected legacy surfaces: `Residue`, `Chain`, `PDBAtom`,
    `SecondaryStructure`, terminal/torsion queries, and typed iterators
    (`V2X-HIERARCHY-COLLAPSE.md` §4, §5; `V2X-ROADMAP.md` §2 H8).

**New issue:** none. No new internal contradiction found between the
assessment, the roadmap `H1b'` gate, and the H1b/H2 design-doc notices. The
fallback to stage to v2.3 if the gate slips is coherent with the locked
v2.2/v2.3/v2.4 plan, and the revision does not overstate what H1a/H1b/H2a
can keep: it preserves H1a/H2 machinery while explicitly redoing
`ContainerKind`, the typed-handle surface, D58 role payload, and H2c
typed iterators.
