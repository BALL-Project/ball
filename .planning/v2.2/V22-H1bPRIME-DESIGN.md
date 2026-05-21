# BALL v2.2 — `H1b′` Hierarchy-Collapse Design-Lock (HCP-0)

**Status:** DRAFT — recommended answers for maintainer review; pre-Codex
(H1bP-review pending).
**Authored:** 2026-05-21.
**This IS the `H1b′` gate** (`V2X-HIERARCHY-COLLAPSE-PLAN.md` HCP-0). It must
LOCK the collapse design concretely enough to implement (HCP-1/HCP-2) and
migrate consumers (HCP-3) WITHOUT discovery during H3. If these do not lock
cleanly, **stage the collapse out of v2.2** (plan §5).
**Parents:** `V2X-HIERARCHY-COLLAPSE.md` (§2a LOCKED *what*),
`V2X-HIERARCHY-COLLAPSE-PLAN.md` (the *how/order*, HCP1b AGREE),
`V22-DECISIONS.md` (D45–D74).

> Each section ends with a **RECOMMENDATION** (my proposed lock) + the
> decision id (D-HC1…D-HC7). The maintainer accepts/edits; Codex then
> adversarially reviews the locked set.

---

## 0. Current model (what we are collapsing)

- **`ContainerKind`** (`containerKind.h`): `NONE, MOLECULE, PROTEIN, CHAIN,
  RESIDUE, SECONDARY_STRUCTURE, NUCLEOTIDE, NUCLEIC_ACID, FRAGMENT` (9).
- **8 typed handles** (`containerHandle.h`, all 24 B, no data members):
  `MoleculeHandle`/`ChainHandle`/`FragmentHandle` (name only),
  `ProteinHandle`/`NucleicAcidHandle` (+`getID`),
  `ResidueHandle`/`NucleotideHandle` (+`getID`+`getInsertionCode`),
  `SecondaryStructureHandle` (+`getTypeCode`).
- **Role-bearing v0 state:**
  - `Residue::Property` bits: `NON_STANDARD, AMINO_ACID, WATER, HAS_SSBOND,
    C_TERMINAL, N_TERMINAL, CYCLIC`; `Residue::id_` (String),
    `insertion_code_` (char).
  - `SecondaryStructure::Type`: `HELIX, COIL, STRAND, TURN, UNKNOWN`.
  - `PDBAtom`: `branch_designator_`, `remoteness_indicator_`,
    `alternate_location_indicator_` (char), `occupancy_`,
    `temperature_factor_` (float), + a redundant `PROPERTY__HETATM` bit. (PDB
    serial is a transient parse key; HETATM is authoritatively a *residue*
    fact; segId has no field — see §2.)

The store container table (D58) already keys rows on `ContainerKind` + a
per-kind payload; H1a/H2 edge/mirror machinery is **kind-agnostic** and
survives the collapse (HC1b §6). Residue/Nucleotide flag bits are already
container *properties*, not payload fields.

---

## 1. Node kinds + role taxonomy (D-HC1)

**Kinds (final):** `System` (root row), `Molecule`, `Fragment`, `Atom`
(+ `Bond` edge). `ContainerKind` shrinks to **`{ NONE=0, MOLECULE=1,
FRAGMENT=2 }`**; an orthogonal **`role`** column carries identity.

Mapping from the 9 old kinds:
| old kind | new kind | role |
|---|---|---|
| MOLECULE | MOLECULE | MoleculeRole::UNKNOWN/SMALL_MOLECULE |
| PROTEIN | MOLECULE | MoleculeRole::PROTEIN |
| NUCLEIC_ACID | MOLECULE | MoleculeRole::NUCLEIC_ACID |
| CHAIN | FRAGMENT | FragmentRole::CHAIN |
| RESIDUE | FRAGMENT | FragmentRole::RESIDUE (+ ResidueKind) |
| NUCLEOTIDE | FRAGMENT | FragmentRole::RESIDUE + ResidueKind::NUCLEOTIDE |
| SECONDARY_STRUCTURE | FRAGMENT | FragmentRole::SECONDARY_STRUCTURE (+ SSKind) |
| FRAGMENT | FRAGMENT | FragmentRole::ARBITRARY |

**Role enums (typed, `std::uint8_t`, `UNKNOWN = 0` default):**
```
enum class MoleculeRole : u8 { UNKNOWN, PROTEIN, NUCLEIC_ACID, SMALL_MOLECULE,
                               POLYMER, LIGAND, SOLVENT, WATER, ION };
enum class FragmentRole : u8 { UNKNOWN, CHAIN, RESIDUE, SECONDARY_STRUCTURE,
                               DOMAIN, FUNCTIONAL_GROUP, ARBITRARY };
enum class ResidueKind  : u8 { UNKNOWN, AMINO_ACID, NUCLEOTIDE, WATER, LIGAND,
                               ION, NONSTANDARD };   // when FragmentRole==RESIDUE
enum class SSKind       : u8 { UNKNOWN, HELIX, STRAND, TURN, COIL };
```
- `ResidueKind` **absorbs** the `Residue::Property` identity bits
  `AMINO_ACID`/`WATER`/`NON_STANDARD` (they become the kind, one typed field,
  not 3 sparse bools).
- `HAS_SSBOND`, `CYCLIC` stay **sparse boolean container properties** (rare;
  set by processors).
- `C_TERMINAL`/`N_TERMINAL` are **derived, not stored** — a residue is
  N/C-terminal iff it is the **first/last POLYMER residue** of its chain, i.e.
  first/last role=RESIDUE child with `ResidueKind ∈ {AMINO_ACID, NUCLEOTIDE}`
  (H1bP-5). This matches v0, whose free `getNTerminal`/`getCTerminal`
  (`residue.h:461,476`) key on `PROPERTY__AMINO_ACID` — NOT merely the
  first/last residue child (which, now that `ResidueKind` includes
  WATER/LIGAND/ION/NONSTANDARD as residue-role fragments, would wrongly pick a
  trailing solvent/ligand). (If a processor needs to *force* a terminal flag
  independent of position, it remains a sparse property.)
- `SSKind` maps 1:1 from `SecondaryStructure::Type` (COIL/UNKNOWN preserved).

Storage in the container payload (D58 extension): `mol_role:u8`,
`frag_role:u8`, `residue_kind:u8`, `ss_kind:u8` (overlay/union by kind — a row
is MOLECULE xor FRAGMENT, so 2 role bytes + `id`/`insertion`/SS payload as
today). No size regression vs the current per-kind payload.

**RECOMMENDATION (D-HC1):** kinds `{MOLECULE, FRAGMENT}` + the four role enums
above; identity bits fold into `ResidueKind`; terminal = derived; SSBOND/CYCLIC
= sparse properties.

---

## 2. Atom PDB-field storage (D-HC2)

Former `PDBAtom` fields, split typed-column vs sparse-property by access
frequency in PDB I/O (the hot path) + round-trip necessity:

> **Verified storage facts (H1bP-1/2):**
> - `PDBAtom::PROPERTY__HETATM` **does exist** (`PDBAtom.h:79`) and is set at
>   parse (`PDBFileDetails.C:731`) — BUT the writer **ignores it**: it derives
>   HETATM from the *residue* (`isHeteroAtom_` → `!Residue::PROPERTY__AMINO_ACID`,
>   `PDBFileDetails.C:1264-1273`). So het is *already* a residue-level fact;
>   the atom bit is redundant.
> - PDB **serial is a transient parse-time key**, not stored: the reader maps
>   `record.serial_number → atom` (`PDB_atom_map_`, `:368`) and turns CONECT
>   records into **bonds** (`createBond`, `:543`); the writer **renumbers** and
>   regenerates CONECT from the bond graph. Connectivity lives in the bond
>   graph, not in preserved serials.
> - **No segment-id accessor exists** on `PDBAtom`/`Atom` (HC1b §2a listed it
>   speculatively — there is nothing to migrate).

| field | storage | why |
|---|---|---|
| occupancy (float) | **typed atom column** | every ATOM/HETATM; FORMAT round-trip + VIEW coloring |
| temperature factor (float) | **typed atom column** | every ATOM/HETATM |
| altLoc (char) | **typed atom column** | selection-affecting; FORMAT round-trip |
| ATOM vs HETATM | **`ResidueKind`** (§1) — atom `PROPERTY__HETATM` bit **intentionally normalized away** | the writer already derives het from the residue; the atom bit is redundant. Deliberate normalization (consistent with structural-equivalence, not byte-faithful) |
| PDB serial | **NOT stored** — transient parse-time key; connectivity via the **bond graph** | CONECT round-trips through bonds; writer renumbers (serials are not preserved byte-faithfully — they never were) |
| segment id | **n/a** — no field exists today | nothing to migrate; if SEGID round-trip is later wanted → sparse property |
| formal PDB charge string | **sparse property** | rare |
| branch designator / remoteness | **DROP** | zero non-test consumers (parse-time population only) |

Consequence: **no `dynamic_cast<PDBAtom*>` anywhere** — PDB-ness becomes
"the atom has these columns" (always present, default-valued for non-PDB
atoms). The reader's existing behaviour (defaults for non-PDB atoms, dropping
non-selected altLocs, regenerating records + serials from parsed structure) is
preserved — fidelity is **normalized structural equivalence**, not
byte-faithful (HC1b §2a).

**RECOMMENDATION (D-HC2):** occupancy/B-factor/altLoc = the only typed atom
columns; het → `ResidueKind` (the redundant atom `PROPERTY__HETATM` bit is
dropped, a deliberate normalization); serial NOT stored (CONECT via the bond
graph, writer renumbers); no segment-id to migrate; formal charge sparse; drop
branch/remoteness.

---

## 3. Collapsed handle API + facade policy (D-HC3, D-HC5)

**Handles (value, read the role columns):**
- `Molecule` — `getName`, `getMoleculeRole`/`setMoleculeRole`, container API.
- `Fragment` — `getName`, `getFragmentRole`/`set…`; **role-aware** accessors
  that assert role in debug/Python (`BALL_CONTAINER_HANDLE_CHECKS`):
  - `getID`/`setID` (role ∈ {CHAIN?,RESIDUE}); `getInsertionCode`/`set…`
    (role==RESIDUE); `getResidueKind`/`set…` (role==RESIDUE);
    `getSSKind`/`set…` (role==SECONDARY_STRUCTURE).
- `Atom` — existing value handle + the new PDB columns (§2).

**Navigation = `BALL::StructureQuery` free functions** (not member methods),
operating on `Molecule`/`Fragment`/`System`:
`residues(x)`, `chains(x)`, `residueByID(chain,id,ins)`, `nTerminal(chain)`,
`cTerminal(chain)`, `secondaryStructures(mol)`, `fragmentsByRole(x, role)`.
Rationale: keeps the handle types thin; navigation is cross-cutting and
role-filtered, not per-instance state.

**Facade policy (D-HC5):** **hard break for the TYPES** (`Residue`/`Chain`/
`Protein`/… removed; `using Residue = Fragment` can't carry role
preconditions or methods — HC1-4) **+ migration guide**. BUT the census shows
one shim earns its keep: the **typed iterators span 40 files** (`ResidueIterator`
alone). Recommend shipping a **`[[deprecated]]` role-filtered-view alias** so
those sites compile + migrate gradually, removed at H4.

**Iterator-view contract (H1bP-3) — what the alias guarantees + where it does
NOT:**
- A *container-scoped* role view (`residues(system/molecule/chain)` ≡
  `ResidueIterator` over that container) is **behaviour-compatible** for the
  common cases: **preorder** traversal, **canonical-depth** descent
  (System→Molecule→Chain→Residue), yielding exactly the role=RESIDUE
  fragments — equivalent to the old `CompositeIterator`-based residue
  iteration once SS is no longer an owning layer (D-HC4 actually *simplifies*
  this: no Chain→SS→Residue indirection).
- **Mutation/erasure during iteration** keeps the v0 contract: iterators are
  invalidated by structural mutation of the iterated subtree (documented;
  not made stronger).
- **NOT a plain alias:** `SecondaryStructure::beginResidue()` /
  `getResidue(i)` (residues *of an SS*) — under D-HC4 an SS owns no residues,
  so this becomes a **span iteration** over the SS annotation's
  `[first_residue_ref, last_residue_ref]` (a `StructureQuery::residuesInSpan`
  view), reimplemented, not aliased. The 2 known consumers (HELIX/SHEET write
  paths) migrate to the span view explicitly.
This is an *iteration shim*, not a type facade — no false type guarantees.
Type-facade wrapper classes (`[[deprecated]] class Residue { Fragment f_; }`)
remain *optional* and maintainer's call.

**RECOMMENDATION (D-HC3/D-HC5):** `Molecule`/`Fragment`/`Atom` +
`StructureQuery`; hard break on the types; **ship the deprecated
container-scoped role-iterator alias** (40-file surface) with the contract
above; **SS-scoped residue iteration reimplemented over the span**; type-facade
wrappers optional.

---

## 4. Audited consumer matrix (the gate's load-bearing deliverable)

> Evidence-driven (HCP1-2): the role API + relocation map (§5) are designed
> AGAINST these real call-sites, not guessed. A method that does not relocate
> cleanly here is a GATE finding, not an H3 surprise.

Census of `source/**/*.C` + `include/BALL/**/*.{h,iC}` (tests + the owning
class excluded). Ranked by relocation effort:

| # | Surface | Consumer files | Modules (heaviest) | Relocation |
|---|---|---|---|---|
| 1 | **Typed iterators** `ResidueIterator`/`residueBegin`/`BALL_FOREACH_RESIDUE` (40) + `ChainIterator` (11) + `AtomContainerIterator` (6) + `Protein`/`SS`Iterator (5 ea) | **40** (residue) | APPLICATIONS 10, NMR 7, STRUCTURE 5, VIEW 4, FORMAT 3, MOLMEC/DOCKING/KERNEL 2 | **HIGH (broad, mechanical)** → role-filtered view + **deprecated alias shim** so the 40 sites migrate gradually |
| 2 | **RTTI dispatch** `dynamic_cast`/`isKindOf` on the doomed types | Residue 13/15, Protein 8/15, AtomContainer 13/10, PDBAtom 8/0, SS 4/7, Chain 1/14, NA/Nucleotide 3/3 | VIEW (AtomContainer 13), KERNEL, STRUCTURE, FORMAT | **MOD–HIGH** → role test (`getRole()==`); `molecularInformation.C:55-71` + `system.C:275-289` are the type-switch templates. AtomContainer casts: the VIEW ones are largely "is-a-container" (→ non-Atom check), **BUT non-VIEW sites (`molecularInformation.C`, `system.C`, `sideChainPlacementProcessor.C`, `MOL2File.C`) are NOT presentation-only and need explicit role-test conversion (H1bP-6) — do NOT defer the whole AtomContainer set to VIEW/H7; each site is audited at H3a** |
| 3 | **`Residue::Property` enum** | **12** | STRUCTURE 4 (`fragmentDB`,`disulfidBondProcessor`,`defaultProcessors`,`peptide*`), FORMAT 3, MOLMEC 2, NMR/VIEW/APP 1 | **MOD** → `AMINO_ACID`/`WATER`/`NON_STANDARD` **become `ResidueKind`**; `HAS_SSBOND`/`CYCLIC` stay sparse bits; `C/N_TERMINAL` derived. `fragmentDB.C:482-502` is the enum↔string table |
| 4 | **Typed-tree construction** `new Residue`(13)/`new PDBAtom`(11)/`new Protein`(7)/`new Chain`(6)/`new SS`(2); `insertParent`(**1**) | ~13 (mostly 4 readers) | FORMAT (`PDBFileDetails`,`dockResultFile`,`HINFile`,`MOL2File`), STRUCTURE builders | **TRACTABLE, localized** → `new Fragment`+role. **`insertParent` (SS-grouping) is PDBFileDetails.C ONLY**. `new NucleicAcid`/`Nucleotide` never built outside tests |
| 5 | **Terminal predicates** `isNTerminal`/`isCTerminal`/`isTerminal` | 8 | STRUCTURE 4, MOLMEC/NMR/VIEW 1, APP 2 | **MOD (structural)** → role-filtered first/last query; `getNTerminal`/`getCTerminal` **already free functions** in `residue.h:461,476` (head start) |
| 6 | **SS `Type`/`getType`/`setType`** | 11 | VIEW 3 (coloring) + `colorProcessor.h`, FORMAT 1, STRUCTURE 1 (the DSSP assigner), NMR/CONCEPT/APP 1 | **EASY** → `Fragment::getSSKind`; direct field, no tree-walk |
| 7 | **Navigation** `countResidues`(9)/`getResidueByID`(1)/`getResidue(idx)`(~6)/`getChain(idx)`(3)/`countChains`(3)/Residue`getSecondaryStructure()`(2) | ~9 | FORMAT, STRUCTURE, VIEW, APP, NMR | **EASY** → `StructureQuery` role-filtered count/index/byID |
| 8 | **PDBAtom fields** occupancy(5)/tempFactor(5)/altLoc(**1**); branch/remoteness(**0**) | 5–8 | FORMAT (round-trip), VIEW (B-factor coloring), DOCKING, APP | **EASY + PRUNE** → occupancy/B-factor/altLoc → Atom columns; **drop branch/remoteness (zero consumers)** |
| 9 | **Backbone torsions** `getTorsionPhi/Psi/Omega` | 6 | NMR, STRUCTURE, VIEW, APP | **EASY** → free fn over backbone atoms (pure geometry) |

**Census disambiguations that change the story (load-bearing):**
- `Atom::getResidue()`/`getChain()` (no-arg, atom→parent navigators, ~25 files)
  are **NOT** in scope — only the indexed/by-id container accessors are.
  `Atom::getFullName` (84 hits, MOLMEC/SCORING) is **NOT** in scope; only
  `Residue::getFullName` (~7 logging sites) is.
- **HETATM/ATOM is NOT a PDBAtom field** — modeled via `Residue`
  `PROPERTY__WATER`/`__NON_STANDARD` → it maps to **`ResidueKind`**, not an
  atom column. **No `getRecordType`/segment-id atom accessors exist.**
- `getBranchDesignator`/`getRemotenessIndicator`: **zero non-test consumers**
  → delete, do not even keep as sparse properties.
- `getNTerminal`/`getCTerminal` already free functions (terminal = derived ✓).

**Gate finding:** every relocated method (§5) has a clean new home — no
method requires keeping a subtype. The two non-trivial structural items
(SS-as-annotation §7; the 40-file iterator surface) are quantified and have
mitigations (shim). **No blocker; the collapse API serves real usage.**

---

## 5. Method-relocation map

Each entry traces to a §4 consumer cluster.

| v0 surface | new home | notes (from §4 evidence) |
|---|---|---|
| typed iterators (`ResidueIterator`/`residueBegin`/`BALL_FOREACH_RESIDUE`, …) | `StructureQuery::fragments(x, role)` role-filtered view + **`[[deprecated]]` alias** `ResidueIterator` = view-over-role | 40 files (residue) — ship the alias so sites migrate gradually, not in one commit (D-HC5) |
| `dynamic_cast<Residue*>`/`isKindOf<Protein>` etc. | `fragment.getRole()==` / `molecule.getMoleculeRole()==` via a central role-test helper | `molecularInformation.C:55-71` + `system.C:275-289` are the templates; **AtomContainer "is-a-container" casts may survive** as a non-Atom check (audit each at H3a) |
| `Residue::Property` `AMINO_ACID`/`WATER`/`NON_STANDARD` | `ResidueKind` (the role) | `fragmentDB.C:482-502` enum↔string table re-pointed at `ResidueKind` |
| `Residue::Property` `HAS_SSBOND`/`CYCLIC` | sparse boolean Fragment properties | set by `disulfidBondProcessor`/build-bonds |
| `isNTerminal`/`isCTerminal` | `StructureQuery::isNTerminal(fragment)` (position-derived) | free fns `getNTerminal/getCTerminal` already exist in `residue.h:461,476` |
| `getResidueByID`/`getResidue(idx)`/`countResidues`/`getChain(idx)`/`countChains` | `StructureQuery` role-filtered byID/index/count | low volume (1–9 each) |
| `Residue::getSecondaryStructure()` (residue→containing SS) | `StructureQuery::secondaryStructureOf(residue)` (reverse annotation lookup, §7) | 2 consumers |
| `SecondaryStructure::getType/setType` | `Fragment::getSSKind/setSSKind` | 11 consumers, mostly VIEW coloring + FORMAT |
| `Residue::getTorsionPhi/Psi/Omega` | `StructureQuery` free fn over backbone atoms | 6 consumers, pure geometry |
| `Residue::getFullName` | `StructureQuery::fullName(fragment)` | ~7 logging sites (NOT `Atom::getFullName`) |
| `PDBAtom` occupancy/B-factor/altLoc | `Atom` columns (§2) | FORMAT + VIEW |
| typed-tree `new Residue/Chain/…` + `insertParent` | `new Fragment`+role; SS-grouping reparent | construction localized to 4 FORMAT readers; `insertParent` = `PDBFileDetails.C` only |

---

## 6. Canonical depth + role-filtered iteration (contract)

- **Canonical PDB depth:** `System → Molecule(role=PROTEIN/NA) →
  Fragment(role=CHAIN) → Fragment(role=RESIDUE) → Atom`. Free nesting allowed
  for non-PDB; **PDB I/O normalizes to this depth**; role queries return the
  canonical sequence regardless of extra nesting.
- **Iteration contract:** typed iterators → `StructureQuery::fragments(x,
  role)` + predicate-filtered `apply`. `residues(x)` ≡ `fragments(x,
  RESIDUE)` honouring canonical depth.
- **Invariant asserts (debug/Python):** a role=RESIDUE fragment's parent
  chain has role=CHAIN; a role=CHAIN fragment's parent is a Molecule. Wrong
  parentage = debug assert (HCP-5 wrong-role test).

---

## 7. Secondary-structure span model (D-HC4)

> **⚠ This is the heaviest single structural change in the migration —
> named here so it is not discovered at H3.** In v0 the hierarchy is
> `Protein → Chain → SecondaryStructure → Residue → Atom`: **SS OWNS the
> residues** (`SecondaryStructure::getResidue(pos)`/`getNTerminal`/
> `getCTerminal` — residues are SS's *children*). The locked collapse
> (HC1b §2a) makes residues **direct children of the chain** and demotes SS
> to a **non-owning annotation sibling**, changing the canonical depth to
> `System → Molecule → Fragment(CHAIN) → Fragment(RESIDUE) → Atom` (no SS
> layer). Every `chain → SS → residue` traversal and the PDB reader's
> SS-nesting + `SecondaryStructureProcessor` must be rewritten (H3a STRUCTURE
> + H3c FORMAT). The §4 consumer matrix MUST quantify this (SS file count).

SS is a `Fragment(role=SECONDARY_STRUCTURE)` that **annotates** a residue
range — it does NOT own the residues. **Maintainer-confirmed model
(2026-05-21):** *"SS are annotations only and need to reference back to the
chain they belong to and reference the residues they start/end at."* So:

- **LOCKED (D-HC4):**
  - Residues are now **direct children of the chain** (`Fragment(CHAIN) →
    Fragment(RESIDUE) → Atom`); the v0 SS-owns-residues layer is gone.
  - An SS annotation is a `Fragment(role=SECONDARY_STRUCTURE)` that **belongs
    to its chain** — its **parent edge is that chain** (≡ "reference back to
    the chain"), living as a sibling of the chain's residue children
    (role-filtered iteration separates them: `residues(chain)` returns only
    role=RESIDUE children).
  - It carries **`{first_residue_ref, last_residue_ref}`** + the **SS payload
    (locked here):** `SSKind:u8`, `helix_class:u8` (PDB HELIX col 39-40),
    `sheet_id:u16` (interned), `strand_number:u8`, `sense:i8` (-1/0/+1 per PDB
    SHEET) — "the residues they start/end at" + the HELIX/SHEET/TURN record
    fields. Refs are container-row handles into the same chain.
  - **Reverse lookup overlap semantics (H1bP-4) — locked:** v0
    `Residue::getSecondaryStructure()` returns a *single* SS, so the
    compatibility shim **`StructureQuery::secondaryStructureOf(residue)`
    returns the FIRST covering annotation in the chain's SS-child order**
    (deterministic, matches v0's single-parent behaviour). The general
    overlap-aware API is **`secondaryStructuresOf(residue)` → list**. Overlap
    is allowed (annotations may overlap; ownership never does).
  - **Discontiguous / multi-strand cases:** a β-sheet with N strands = N SS
    annotation fragments (one span each, shared sheet-id); SS spanning chains
    is disallowed (PDB HELIX/SHEET are single-chain). A non-contiguous single
    annotation may use an explicit residue-ref list instead of first/last.
- Rejected: SS owning the residues as children (breaks single-parent
  ownership + the canonical chain→residue→atom depth). Round-trip = read/write
  the start/end residue refs against PDB HELIX/SHEET/TURN records.

---

## 8. Migration-guide skeleton (seed into `V22-API-BREAK-LEDGER.md`)

Old → new, one row per removed surface: `Residue`→`Fragment`(role=RESIDUE) +
`StructureQuery`; `Chain`/`Protein`/`NucleicAcid`/`Nucleotide`/
`SecondaryStructure`/`AtomContainer`→`Fragment`/`Molecule`+role; `PDBAtom`→
`Atom`+columns; typed iterators→`fragments(role=…)`; torsion/terminal→
`StructureQuery`; `dynamic_cast<Residue*>` etc.→role checks. Each row: a
before/after snippet. Built incrementally per HCP-3 cluster.

---

## 9. PDB golden-corpus spec (for HCP-3c smoke + HCP-5 gate)

Cases (parse→write→parse structural equivalence on structural fields): a
multi-chain protein; a nucleic acid; a ligand+water+ion complex; insertion
codes; altLoc; HETATM; TER; CONECT (**connectivity preserved via the bond
graph; output serials renumbered, NOT byte-stable**); SSBOND; HELIX/SHEET/
TURN; CRYST1; and a file with records BALL skips (carried through). Equivalence
compares: chain ids, residue id+insertion+kind, het flag (residue-level),
altLoc/occupancy/B-factor, SS records (incl. helix-class/sheet-id/strand/sense/
endpoints), TER, **CONECT bond-connectivity (not serial values)**,
element+formal-charge policy. NOT byte-faithful. HCP-3c runs a
representative subset; HCP-5 (H8) runs the full corpus + the
no-`dynamic_cast<PDBAtom*>` regression grep + the wrong-role assertion test.

---

## 10. Build-narrow flag + sub-phase numbering (D-HC7, D-HC6)

- **D-HC7 (narrow flag):** **reuse `BALL_CORE_ONLY`** — restore the
  `IF(NOT BALL_CORE_ONLY)` guards around the extension-module includes in
  `cmake/BALLIncludes.cmake:31–143` (they were removed during v2.0 Track-B;
  only PYTHON stays gated at :145). One flag, matches the Track-B mental
  model, and "core only" is exactly the semantics we want. (Alternative: a
  fresh `BALL_COLLAPSE_KERNEL_ONLY` to avoid re-overloading the v2.0 flag —
  rejected as redundant; v2.0 is shipped, the v2.2 branch may redefine the
  flag's gating.) **WARNING (H1bP-7):** this *redefines* a flag whose current
  behaviour is "everything except Python", so HCP-1 task 0 must (a) **assert
  in the build the exact module set excluded** (a config-time message + a
  CI check listing the gated modules), and (b) **update the user-facing CMake
  option help text / docs** so `BALL_CORE_ONLY=ON` no longer silently means
  "+Python-off". HCP-1 task 0 restores + baselines + documents this.
- **D-HC6 (numbering):** HCP-1/HCP-2 land as an **explicit revision of the
  H1b deliverables** (the roadmap's `H1b′`), same milestone-internal — not a
  new top-level phase. The collapsed `containerKind.h`/`containerHandle.h`
  supersede the H1b versions in place.

---

## 11. Open items the gate must NOT leave for H3

- The §4 consumer matrix must show every relocated method (§5) has a clean
  new home; any that doesn't is a gate blocker.
- HCP-1 delivers the **scalar+role mutation mirror** (blocks H2d) — name/id/
  insertion/SS-kind/roles + the new atom PDB columns (plan §2 HCP-1).
- The JSON `kind`+`role` schema is sketched here but **frozen at H6b** (not
  this gate) — must stay forward-compatible with the role model.

## 12. Status — CONVERGED (Codex H1bP→H1bPb AGREE); awaiting maintainer sign-off

**H1bP = NEEDS-REVISION → revised → H1bPb = AGREE (2026-05-21).** The design is
Codex-converged; the `H1b′` gate closes on **maintainer sign-off** of
D-HC1…D-HC7 (below). On sign-off, HCP-1 implementation begins; until then this
is a locked-pending-approval design. (D-HC4 already maintainer-confirmed.)

**H1bP applied (NEEDS-REVISION → revised, 2026-05-21).** Fixes: (1) het bit
exists but is normalized away to `ResidueKind` (was wrongly "no HETATM
field"); (2) PDB serial is NOT stored — CONECT round-trips via the bond graph,
writer renumbers; segId has no field; (3) iterator-shim contract scoped
(container-scoped role view = compatible; SS-scoped residue iteration
reimplemented over the span); (4) SS payload (sheet-id/strand/sense/helix-class)
+ overlap reverse-lookup semantics locked; (5) terminal = first/last POLYMER
residue; (6) non-VIEW AtomContainer casts not deferred to H7; (7) D-HC7 reuse
warning + HCP-1 module-exclusion assertion. Review:
`V22-H1bPRIME-DESIGN-H1bP.md`. Next: re-review (**H1bPb**); on AGREE +
maintainer sign-off this is the `H1b′` lock.

Recommended decisions for sign-off: **D-HC1** kinds={MOLECULE,FRAGMENT}+4 role
enums; **D-HC2** occ/B-factor/altLoc = the only atom columns, het→ResidueKind
(atom bit dropped), serial NOT stored (CONECT via bonds), drop
branch/remoteness; **D-HC3** Molecule/Fragment/Atom + StructureQuery;
**D-HC4** SS = non-owning annotation parented under its chain, carrying
start/end residue refs (**maintainer-confirmed**; changes v0 Chain→SS→Residue
ownership → residues now direct children of the chain); **D-HC5** hard type
break + deprecated role-iterator alias;
**D-HC6** land as the H1b′ in-place revision; **D-HC7** reuse `BALL_CORE_ONLY`
(restore guards).
