# BALL 2.x — Composite-hierarchy collapse assessment

**Status:** HC1 NEEDS-REVISION applied (field-survey corrected, role/depth/
SS/PDB locked, slotting = explicit pre-H3 gate); HC1b pending.
**Verdict so far:** COLLAPSE = YES (4 kinds: System/Molecule/Fragment/Atom
+ Bond), conditional on the `H1b′` design-lock gate closing before H3.
**Authored:** 2026-05-21 (maintainer directive, during v2.2 H2).
**Question:** collapse the molecular class hierarchy to fundamentally
**System / Molecule / Fragment / Atom** (+ Bond), capturing residue /
nucleotide / chain / secondary-structure / protein / nucleic-acid /
PDBAtom distinctions as **roles/properties** instead of C++ subclasses;
drop the `AtomContainer` concept. Assess impact, slot into the 2.x plan.

---

## 1. Today's hierarchy vs the proposal

**Today (v0):** `Composite` → `AtomContainer` → { `Molecule` → {`Protein`,
`NucleicAcid`}, `Chain`, `Fragment` → {`Residue`, `Nucleotide`},
`SecondaryStructure` }; `Atom`/`PDBAtom` (leaves); `Bond`; `System` (root).
~11 hierarchy classes + 2 atom classes.

**Proposed:** four node kinds —
- **System** — the root container (owns the store).
- **Molecule** — a top-level molecule. Protein / nucleic-acid distinction
  → a `role`/properties on the Molecule.
- **Fragment** — a *recursively nestable* sub-container. Chain, Residue,
  Nucleotide, SecondaryStructure all become a Fragment carrying a `role`
  (chain / residue:amino-acid / residue:nucleotide / ss:helix / …) plus
  the role-specific data (id, insertion-code, SS-type, flags) as fields/
  properties.
- **Atom** — the leaf. PDBAtom's extra fields → atom properties.
- **Bond** — unchanged (an edge, not a hierarchy node).

Semantic type becomes **data on the node**, not a C++ type. This is the
natural endpoint of A2 (which already stores the hierarchy as rows with a
`kind` tag + per-kind payload).

---

## 2. How the field represents molecular hierarchy (survey)

| Package | Hierarchy | Residue/chain typing | Per-particle subclasses? |
|---|---|---|---|
| **RDKit** | `Mol` → `Atom`/`Bond` (+ Conformer). No residue/chain *objects*. | `AtomPDBResidueInfo` **per-atom metadata** (resname, chain, icode, isHetero). | No. Fragments are *computed*, not objects. |
| **OpenMM** | `Topology` → `Chain` → `Residue` → `Atom` (fixed 4-level) | `Residue.name` (string), `Chain.id` (string). | **No subclasses** — levels are fixed, type is a string. |
| **MDAnalysis** | `Universe`; `AtomGroup`/`ResidueGroup`/`SegmentGroup` are **index groupings + attribute arrays** (SoA) | `resnames`, `segids`, `resids` are arrays. | No. Residue/Segment = grouping + attrs. |
| **MDTraj** | `Topology` → `Chain` → `Residue` → `Atom` | `Residue.name`; `.is_protein`/`.is_water` computed. | No subclasses; type is name/computed. |
| **BioPython** | `Structure`→`Model`→`Chain`→`Residue`→`Atom` (SMCRA, fixed 5-level) | `Residue.resname` + id `(hetflag, resseq, icode)`; `is_aa()` is a free function. | No subclasses; type is data. |
| **Gemmi / CCP4** | `Structure`→`Model`→`Chain`→`Residue`→`Atom` | `Residue.het_flag`, entity type enum (polymer/non-polymer/water). | No subclasses; type is enum/data. |
| **OpenBabel** | `OBMol`→`OBAtom`/`OBBond`; **`OBResidue` is a real side object** (atom lists, chain, resnum/resname, per-atom HETATM/atom-id) stored in `OBAtom`/`OBMol` — not a tree node, but more than metadata. | residue data on the side object. | No protein/residue subclass tree. |
| **CDK (Java)** | `IAtomContainer` + subinterfaces. | — | **YES — CDK is a real counterexample:** `IAminoAcid`, `IBioPolymer`, `IPDBMonomer`/`IPDBPolymer`, `Residue`, `Strand`, `AminoAcid` are subclasses/subinterfaces of `IAtomContainer`. (And CDK keeps the `AtomContainer` concept BALL proposes to drop.) |
| **BALL (today)** | `Composite`→`AtomContainer`→Protein/NucleicAcid/Chain/Residue/Nucleotide/SS/Fragment; Atom/PDBAtom | **C++ subclasses + type-specific methods** | **Yes — among the deepest.** |

**Takeaway (corrected):** BALL is **unusually deep** for C++ subtype
modeling among structural-biology toolkits, but the claim "*no* mainstream
package uses subclassing" is FALSE — **CDK does** (`IAminoAcid`,
`IBioPolymer`, `Residue`, `Strand`, …). The accurate field lesson is that
the C/C++/Python *workhorse* toolkits favor:
- **(a) fixed-name levels** with the type as a `name`/`enum`/flag
  (OpenMM, MDTraj, BioPython, Gemmi — a fixed Chain→Residue depth), or
- **(b) per-atom metadata + index groupings** with attribute arrays
  (RDKit, MDAnalysis — though MDAnalysis *does* have `Residue`/`Segment`
  objects + group classes; the data lives in topology-attribute arrays).

The stronger field lesson is **fixed-depth or side-object residue models,
NOT arbitrary free-nesting.** BALL's A2 store is literally pattern (b)
(index groupings = `ChildRef` edges; attribute arrays = container columns)
with a thin handle veneer (pattern (a)) — so the proposal lands BALL in
the field's mainstream, *provided it keeps a canonical PDB depth* rather
than a generic graph of fragments.

## 2a. LOCKED design decisions (post-HC1)

- **Node kinds (4):** `System` (root), `Molecule` (distinct kind — keeps
  System-child molecule iteration / orphan construction / import-export
  grouping / solvent classification invariants; *not* demoted to a
  Fragment role), `Fragment` (recursive sub-container), `Atom`. `Bond` =
  edge.
- **Role taxonomy** (typed enums, not ad-hoc strings, for core identity):
  - `MoleculeRole`: protein / nucleic_acid / polymer / small_molecule /
    ligand / solvent / water / ion / unknown.
  - `FragmentRole`: chain / residue / secondary_structure / domain /
    functional_group / arbitrary_fragment / unknown.
  - `ResidueKind` (when role=residue): amino_acid / nucleotide / water /
    ligand / ion / nonstandard / unknown.
  - `SecondaryStructureKind`: helix / strand / turn / coil / unknown
    (+ helix-class / sheet-id / strand-number / sense as properties).
  - **PDB atom fields** (former `PDBAtom`) → atom columns/properties:
    serial, altLoc, occupancy, temperature factor, record kind
    (ATOM/HETATM), segment id, formal-charge policy, branch/remoteness if
    retained. Core identity fields are **typed columns**; rare/optional
    ones are sparse properties.
- **Canonical depth (required for PDB/mmCIF fidelity):**
  `System → Molecule → chain-Fragment → residue-Fragment → Atom`. Generic
  free-nested fragments are allowed for non-PDB use, but PDB import/export
  **normalizes to this depth**, and role queries (residues-of, chain-of,
  N/C-terminal) must return the canonical sequence regardless of any extra
  nesting.
- **Secondary structure:** modeled as a **role=secondary_structure
  Fragment that spans/annotates a residue range** (references residue
  fragments) rather than an owning atom-container inserted into the chain.
  This keeps the canonical chain→residue→atom depth intact, allows
  overlapping/annotation SS, and matches the PDB HELIX/SHEET/TURN record
  model. (Decision per HC1-1: do not leave SS to FORMAT migration.)

**Design implication from the survey:** the field is split on **fixed
depth vs free nesting**. (a)-packages fix Chain→Residue→Atom; the BALL
proposal's "recursively nestable Fragment" is *more* general than any of
them. That generality is a feature (handles arbitrary fragmentation: rings,
SS spanning residues, coarse-grained beads) but loses the "a Chain
contains Residues" structural invariant some code/round-trips rely on. A
**role taxonomy + optional depth conventions** (Molecule ⊃ chain-Fragment
⊃ residue-Fragment ⊃ Atom for PDB) is needed to keep PDB semantics.

---

## 3. Synergy with the v2.2 A2 redesign (this is the key point)

A2 **already** did most of the conceptual collapse:
- The hierarchy is store rows with a `ContainerKind` tag + per-kind payload
  (`V22-H0-ADDENDUM.md` D58) — *not* a C++ object tree (post-flip).
- H1b introduced typed value handles (`MoleculeHandle`/`ChainHandle`/
  `ResidueHandle`/…) but they are **thin wrappers over one row type**;
  the only per-kind differences are a few scalar getters (D64).
- Residue/Nucleotide flag bits are *already* container properties, not
  payload fields (D58/D59, confirmed vs `residue.h enum Property`).

So collapsing means:
- **`ContainerKind`** shrinks from 8 kinds → essentially `{MOLECULE,
  FRAGMENT}` (System = root, not a row), with a richer free-form **`role`**
  (amino-acid / nucleotide / chain / helix / strand / water / ligand / …)
  carried as a small enum + properties.
- The **8 typed handles → ~2** (`Molecule`, `Fragment`) + `Atom`. Type-
  specific getters (`getID`, `getInsertionCode`, SS-type) become Fragment
  methods reading the role payload; navigation (`getResidue`, `getChain`,
  `getNTerminal`, `getResidueByID`) becomes **role-filtered** Fragment/
  free-function queries.
- **`AtomContainer`** as a base concept disappears — System/Molecule/
  Fragment all expose the same container API via the handle base
  (`ContainerHandleBase` already is exactly this).
- The A2 ChildRef edges + grouping CSR + mutation mirror are **kind-
  agnostic** → H1a/H2 infrastructure survives essentially unchanged; only
  the kind enum + handle types shrink.

**Crucially: the collapse REDUCES not-yet-done work.** H1b's typed handles
shrink; H3 (consumer migration — not started) migrates consumers to the
*collapsed* API once, instead of to 8 typed handles now and a collapsed
API later. **Doing the collapse as a separate post-v2.2 step would force a
double consumer migration** of 100–200 files.

---

## 4. Impact assessment (honest)

### What gets simpler / better
- 4 node kinds instead of ~11; no deep inheritance; no RTTI (already a
  v2.2 goal). Matches the entire field.
- Smaller handle layer; far simpler pyBALL bindings (fewer types to wrap).
- One uniform container API; `AtomContainer` (213 files) concept dissolves.
- Eliminates a whole class of "is this Composite a Residue?" branching.

### What breaks (large — but mostly inside the v2.2 H3 break already)
- **Public types removed:** `Protein`, `NucleicAcid`, `Chain`, `Residue`,
  `Nucleotide`, `SecondaryStructure`, `AtomContainer`, `PDBAtom`. Usage
  breadth: AtomContainer 213, Residue 141, Protein 101, Chain 94, PDBAtom
  54, SS 49, NucleicAcid/Nucleotide 24 files. This is a **bigger** break
  than the handle flip alone — but it largely *coincides* with the H3
  consumer migration (every one of those files is already being touched to
  move off `Composite&`/typed pointers).
- **Type-specific methods need a home:** Residue torsions (φ/ψ/ω), terminal
  detection (`getNTerminal`/`getCTerminal`), `Protein::getResidueByID`,
  `Chain` residue/SS access, `SecondaryStructure::getType`,
  `Residue::getFullName`/PDB naming → role-aware `Fragment` methods or free
  functions in a `BALL::StructureQuery` namespace. Mostly mechanical;
  torsion/terminal logic is geometry over atoms (unchanged), just relocated.
- **Typed iterators** (`residueBegin`, `chainBegin`, `proteinBegin`, …) →
  **role-filtered** iteration (`fragments(role=RESIDUE)`). Pervasive in
  STRUCTURE/FORMAT/QSAR.
- **PDBAtom** removed: its PDB-specific fields (alt-loc, occupancy,
  temperature factor, record type, formal charge string, etc.) → atom
  columns/properties. Touches the PDB reader/writer + 54 files. **Must
  preserve PDB round-trip exactly.**
- **FORMAT (PDB/PDBFileDetails, etc.):** the reader currently builds typed
  Protein/Chain/Residue/PDBAtom trees with `insertParent`/typed inserts.
  Rewritten to build Molecule/Fragment(role)/Atom with role+payload. This
  is the heaviest cluster (H3c) and the highest round-trip-fidelity risk.
- **Persistence/JSON:** the JSON `containers` schema (D62) already keys on
  `kind`; collapsing changes the kind set + adds `role` — fold into the
  H6b schema freeze (one schema, not two).
- **Tests:** `Protein_test`, `Chain_test`, `Residue_test{1,2}`,
  `NucleicAcid_test`, `Nucleotide_test`, `SecondaryStructure_test` →
  rewritten as role-based `Fragment` tests.
- **BALLView + pyBALL:** both assume the typed hierarchy; folds into the
  H7 (VIEW) + pyBALL-rebaseline work the roadmap already schedules.

### Real risks / open issues
1. **PDB/structural round-trip fidelity (the #1 correctness risk).** Chain
   id, residue id + insertion code, het flag (ATOM/HETATM), alt-loc,
   occupancy/B-factor, SS records (HELIX/SHEET/TURN/SSBOND incl. helix
   class / sheet id / strand number / sense / endpoints), TER, CONECT atom
   serials, element + formal-charge policy, and carried-through skipped
   records must survive as Fragment/Atom role+data. **The promise is
   "normalized structural equivalence (parse→write→parse on structural
   fields)", NOT byte-for-byte** — today's reader is already not
   byte-faithful (it drops non-selected alt-locs, computes defaults for
   non-PDB atoms, rewrites records from parsed structure). Exact-line
   preservation applies only to records BALL explicitly stores raw. (Gemmi/
   OpenMM prove data-not-types is sufficient; BALL must match.)
2. **Loss of compile-time typing.** `Residue r` today is type-checked;
   `Fragment` + a `role` enum is checked at runtime. Mis-set roles become
   runtime bugs, not compile errors. Mitigate with a strict role taxonomy +
   debug asserts + role-aware accessors that validate.
3. **Free nesting vs PDB's fixed depth.** Must define role/depth
   conventions (a residue-Fragment's parent is a chain-Fragment is a
   Molecule) so structural code + round-trips have invariants to rely on,
   without hard-coding a fixed class depth.
4. **Scope/timeline of v2.2.** This expands the design surface (role
   taxonomy, relocated methods, FORMAT rewrite) even though it shrinks the
   handle layer. Net: v2.2 gets riskier but avoids a v2.3 double-migration.
5. **Downstream shock.** Removing `Residue`/`Chain`/etc. is a far louder
   break than the handle flip. Needs a prominent migration guide +
   possibly thin deprecated type-aliases (`using Residue = Fragment;` won't
   carry type-specific methods — so aliases are limited).
6. **`Molecule` distinct kind — DECIDED** (§2a): `Molecule` stays a
   distinct node kind (not a Fragment role), to preserve System-child
   molecule invariants (iteration, orphan construction, import grouping,
   solvent classification). Per HC1-3.

---

## 5. Where it slots in the 2.x plan — an explicit pre-H3 design gate

**Recommendation (post-HC1): fold the collapse into v2.2 ONLY via an
explicit short `H1b′` design-lock gate that closes BEFORE H3 starts. If
the role/depth/PDB-field/SS decisions do not lock quickly, STAGE the
collapse to v2.3 and accept the double consumer migration** — an unstable
v2.2 public surface is worse than a double migration.

Rationale: the collapse is the natural completion of A2 and the expensive
consumer migration (H3) hasn't started, so doing it once against the
collapsed API is far cheaper than typed-handles-then-collapse — BUT the
collapse adds real design work (role taxonomy, PDB-field storage, canonical
depth, SS representation, migration guide, round-trip test corpus) that is
NOT just "shrink handles". That design must be locked, not discovered
during H3, or v2.2 becomes an unbounded architecture change.

**Compatibility aliases buy little** (HC1-4): `using Residue = Fragment`
cannot carry role preconditions, overload separation, or the type-specific
methods (H1b already documents that even `using Residue = ResidueHandle`
isn't source-transparent). Real facade *wrapper classes* with role asserts
can soften source migration but then preserve most of the surface — use
them only as deprecated migration shims, not the architectural model.

Concrete slotting (revises the locked roadmap):
- **H1b′ (revise the just-landed H1b):** collapse `ContainerKind` → kinds
  + a `role` enum/property; reduce the 8 typed handles to `Molecule` +
  `Fragment` (+ `Atom`); move per-kind getters to role-aware Fragment
  methods. (H1a table + H2 mirror are kind-agnostic — keep.)
- **H2 (in progress):** unaffected in machinery; the mutation mirror is
  kind-agnostic. The `setProperty`/role payload mirror covers roles.
- **H3:** consumer migration now targets the collapsed API; STRUCTURE/
  FORMAT clusters do the role-method relocation + typed-iterator → role-
  filtered conversion. PDBAtom→Atom-properties lands in H3c (FORMAT).
- **H4:** the flip deletes the v0 classes — now there are far fewer to
  delete (System/Molecule/Fragment/Atom remain).
- **H6b:** the JSON schema freeze includes the final `kind`+`role` model.
- **H8:** D13 + a dedicated **PDB/structural round-trip fidelity gate**.

**Alternative (rejected):** do A2-with-typed-handles in v2.2, collapse in
v2.3. Rejected because it double-migrates 100–200 consumer files and keeps
RTTI/typed-iterators alive one extra milestone.

**Caveat:** if the maintainer wants to *de-risk* v2.2's timeline, the
collapse can be staged: keep the role model + Fragment as the storage/
handle truth in v2.2, but retain thin `Residue`/`Chain` compatibility
*type-aliases or facades* through v2.2 and remove them in v2.3. This
trades a cleaner v2.2 break for a softer downstream transition. (Decision
for the maintainer.)

---

## 6. Open questions — resolved by HC1

1. **Slotting** → fold into v2.2 via an explicit `H1b′` pre-H3 design-lock
   gate; stage to v2.3 if the gate slips (§5).
2. **Node model** → 4 kinds; `Molecule` stays distinct (§2a).
3. **PDB round-trip** → canonical depth required; "normalized structural
   equivalence", not byte-faithful (§2a, §4). Corpus + gate at H8 (below).
4. **Type-specific methods** → all relocatable (none need a subtype);
   torsions/terminal = geometry relocated; navigation = role-filtered;
   the one design item is **SS = span/annotation Fragment** (§2a).
5. **Aliases** → buy little; deprecated facade wrappers only (§5).
6. **Already-landed rework** → H1a table + H2 mirror machinery + D56/D57/
   D59 SURVIVE (kind-agnostic, role-ready); only the `ContainerKind` enum,
   the H1b typed-handle surface, the D58 payload (→ role+role-payload +
   atom PDB columns), and H2c typed-iterators (→ role-filtered) are redone.
   The H2 mirror's v0 `AtomContainer` binding is transitional (dies at H4).
7. **Risk ranking** → PDB fidelity is the dominant *correctness* risk
   (regressions compile clean, corrupt science); blast radius is larger but
   compile-detected. Gate = a golden PDB corpus (protein/nucleic-acid/
   ligand/water/ion/insertion-codes/altLoc/HETATM/TER/CONECT/SSBOND/HELIX/
   SHEET/TURN/CRYST1/skipped records) + parse→write→parse structural
   equivalence + a no-`dynamic_cast<PDBAtom*>` regression + a debug/Python
   wrong-role assertion test. Lands at **H8** alongside D13.

## 7. Status

HC1 = NEEDS-REVISION → this revision applies the 10-item punch list (field
survey corrected incl. CDK counterexample; PDB promise downgraded to
structural-equivalence; role taxonomy + canonical depth + SS-as-span +
Molecule-distinct locked; slotting = explicit `H1b′` gate; rework scope
itemized). Codex **HC1b** confirms; on AGREE the model folds into
`V2X-ROADMAP.md` (H1b′ gate + H8 fidelity gate) and the v2.2 design docs.
