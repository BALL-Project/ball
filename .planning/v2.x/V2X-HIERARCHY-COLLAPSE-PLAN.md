# BALL 2.x — Hierarchy-Collapse Implementation Phase Plan (HCP)

**Status:** DRAFT — pre-adversarial-review (Codex HCP1 pending).
**Authored:** 2026-05-21, mid-v2.2-H2 (H2b topology mirror landed, 286/286).
**Parents:** `V2X-HIERARCHY-COLLAPSE.md` (the converged assessment, HC1b
AGREE), `V2X-ROADMAP.md` (LOCKED kernel roadmap; the `H1b′` gate note in §2),
`V22-DECISIONS.md` (D45–D74), `V22-ARCH-HANDLE-MODEL.md`.
**Purpose:** turn the agreed collapse (4 node kinds: System/Molecule/
Fragment/Atom + roles) into a concrete, sequenced, **KERNEL-first /
local-build-first** implementation phase, sliced for atomic execution and
adversarial review, and slotted into the roadmap at the `H1b′`-before-H3
position.

---

## 0. What is already decided (do not relitigate)

From `V2X-HIERARCHY-COLLAPSE.md` §2a (LOCKED, HC1b AGREE):
- **4 node kinds:** `System` (root) · `Molecule` (distinct, NOT a Fragment
  role) · `Fragment` (recursive) · `Atom`. `Bond` = edge.
- **Role taxonomy** (typed enums): `MoleculeRole`, `FragmentRole`,
  `ResidueKind`, `SecondaryStructureKind`, + PDB atom fields as atom
  columns/sparse properties.
- **Canonical PDB depth:** System → Molecule → chain-Fragment →
  residue-Fragment → Atom (free nesting allowed for non-PDB; PDB I/O
  normalizes to this depth).
- **SS = role=secondary_structure Fragment spanning a residue range**
  (annotation, references residue fragments; not an owning atom container).
- **PDB fidelity promise = normalized structural equivalence** (parse→write→
  parse on structural fields), NOT byte-faithful.
- **Slotting = fold into v2.2 via the `H1b′` design-lock gate that closes
  BEFORE H3; stage to v2.3 if the gate slips.**

This plan does NOT change those. It specifies the *how* and the *order*.

---

## 1. The organizing principle: KERNEL-first, local-build-first

> Maintainer directive: *"it'll break a lot and might deserve focusing on the
> KERNEL classes and local builds first before opening up for the rest of the
> library."*

We reuse the **v2.0 Track-B narrowing PATTERN** — but the `BALL_CORE_ONLY`
flag as it stands today does NOT give us the partition we need, and the plan
must re-establish it first.

> **CORRECTION (HCP1-1): `BALL_CORE_ONLY=ON` no longer narrows to KERNEL.**
> During the v2.0 Track-B module re-enable, FORMAT / STRUCTURE / XRAY / NMR /
> ENERGY / SCORING / MOLMEC / QSAR / SOLVATION / DOCKING were all re-enabled
> **unconditionally** in `cmake/BALLIncludes.cmake` (lines 31–143). The only
> surviving `IF(NOT BALL_CORE_ONLY)` guard is lines 145–148 and it gates
> **PYTHON only**. `ADD_BALL_SOURCES` (`cmake/BALLMacros.cmake:44–55`) has no
> per-module filter hook. So today `BALL_CORE_ONLY=ON` ⇒ "everything except
> the (already-off) Python bindings". The build cache having `CORE_ONLY=ON`
> therefore buys essentially nothing for module narrowing.

**Consequence for the plan:** the very first task of **HCP-1** is to
*re-establish a true KERNEL-only build partition* (call it the
**collapse-narrowed build**). Concretely: wrap the extension-module `INCLUDE`s
in `BALLIncludes.cmake:31–143` back under `IF(NOT BALL_CORE_ONLY)` (or
introduce a dedicated `BALL_COLLAPSE_KERNEL_ONLY` flag if reusing
`BALL_CORE_ONLY` risks confusing the v2.0 semantics — decided at HCP-0/D-HC7),
**verify a clean KERNEL-only build + KERNEL ctest subset on *current* HEAD as
a baseline** (so any later breakage is attributable to collapse edits, not to
CORE_ONLY bit-rot), and only then begin the collapse. The cluster re-open
(**HCP-3**) removes those guards again in the SAME dependency order Track-B
used (FORMAT → STRUCTURE → … → DOCKING), which is why that order is already
encoded in `BALLIncludes.cmake` comments.

KERNEL genuinely does not depend on the extension modules (it is the
foundation layer they build on), so gating them off is safe; this is a CMake
partition restore, not a code change to KERNEL.

**Why this is the right spine for the collapse (not for the earlier H-phases):**
- The collapse's blast radius is huge (AtomContainer 213, Residue 141,
  Protein 101, Chain 94, PDBAtom 54, SS 49, NA/Nucleotide 24 files) but
  almost all of it is in the *extension modules + VIEW*, NOT KERNEL. The
  **role model, the collapsed handle surface, and the store role columns all
  live in KERNEL.** So once the collapse-narrowed build is restored, we can
  design + land + prove the new model against a KERNEL-only build and KERNEL
  tests, with the 200-file consumer churn switched OFF, before re-enabling
  modules one cluster at a time.
- This converts one un-bisectable 200-file mega-break into: (a) a KERNEL
  core that is green in isolation, then (b) a sequence of per-cluster
  re-enable commits, each independently green + reviewable — exactly the
  Track-B cadence that worked for the v2.0 module re-enable.
- The v0 typed classes (Protein/Chain/Residue/…/PDBAtom/AtomContainer) ARE
  KERNEL classes; dual existence (D60) keeps them alive as the source of
  truth through this whole plan, so KERNEL-first does NOT mean deleting them
  early — it means landing the *new* role API alongside them in a narrowed
  build first.

**Local-build-first** = once the collapse-narrowed build is restored (HCP-1
task 0), the inner loop for HCP-1/HCP-2 is `cmake -D<narrow-flag>=ON … &&
ninja libBALL + KERNEL tests && ctest -R '<KERNEL subset>'` — fast, no
module/VIEW rebuild, no MSVC/CI dependency. CI's full matrix re-engages at
HCP-3 (module re-open) per cluster.

---

## 2. Sub-phase breakdown (maps onto the roadmap H-phases)

The collapse is NOT a new top-level milestone; it threads through the
existing v2.2 H-phases. The plan slices it into HCP-0 … HCP-5:

| HCP | Roadmap slot | Build mode | Gate | Status |
|---|---|---|---|---|
| **HCP-0** design-lock | **= `H1b′`** (revises landed H1b) | n/a (docs) | role/depth/SS/PDB-field/API + consumer matrix (HCP1-2) LOCKED + Codex AGREE → else stage (§5) | ✅ H1bP→H1bPb AGREE + maintainer sign-off |
| **HCP-1** store role model | within H1b′/H2 | restore + use narrowed build | task 0: restore KERNEL-only gating + baseline-green; then role enums/columns/payload + scalar+role mirror; **blocks H2d** (HCP1-3); KERNEL tests green | ✅ KR1 + HCP1R→HCP1Rb GO; 132/132 + 286/286 |
| **HCP-2** collapsed handle API | within H1b′ | narrowed build | 8 typed handles → `Molecule`/`Fragment`(+`Atom`) role-aware; `StructureQuery` free fns; KERNEL tests green | ⏭ NEXT (after H2d) — see §6a |
| **HCP-3** module re-open (= **H3a→H3b→H3c**) | H3 | remove guards cluster-by-cluster | each cluster migrated to role API, own commit + Codex review; **H3c (FORMAT) gate includes the PDB golden-corpus parse→write→parse smoke** (HCP1-4) | ⏳ |
| **HCP-4** the flip (= **H4**) | H4 | full | delete v0 typed classes; store is sole truth; `sizeof(Atom)` drops | ⏳ |
| **HCP-5** fidelity gate (= **H8** addition) | H8 | full + MSVC | full PDB/structural round-trip golden-corpus gate alongside D13 | ⏳ |

JSON `kind`+`role` model freezes at **H6b** (unchanged dependency).

### 6a. Course from HCP-1 → HCP-2 (next milestone)

**Immediate — H2d (close the H2 mirror).** Randomized full-surface parity
sweep: random sequences of insert/remove/reparent/splice/swap/clear/replace +
select/deselect + post-root scalar setters, asserting `descTable == descV0`
(name-bearing) + per-row derived selection parity after each step. SCOPE:
exclude rooted-object `set()`/`operator=`/`persistentRead` (the documented
full-replacement carry-over) until that mirror lands in HCP-2. Then a short
Codex H2d close-review → H2 DONE.

**HCP-1P — KERNEL object-creation fast path (perf sibling of HCP-1).**
KERNEL-only, collapse-narrowed build, no handle-API change; closes the v2.0→2.x
**create/clone regression** (System ≈2.6 µs/op, Atom ≈780 ns/op — sample-profiled
to redundant ctor re-init + default-name interning + per-System well-known column
predeclare). Runs next, in parallel with / ahead of HCP-2 (independent surface).
Each step atomic + green ctest + benchmark re-measure:
- **HCP-1P.A — born-default atom slot.** Align `MoleculeStore::allocate_atom`'s
  fresh-slot defaults to `BALL_ATOM_DEFAULT_*`, then **delete** the redundant
  `Atom::Atom()` (default ctor only) `BALL_ATOM_ORPHAN_INITIAL_WRITES_LOCK_` block.
  Biggest single win — every atom hits the ctor.
  **Verified design (pre-implementation):**
  - Only **2 columns** mismatch the born-default zeros: `atom_types_` (born `0`,
    must be `Atom::UNKNOWN_TYPE = -1`) and type_name (born `""`, must be `"?"`).
    Element already matches — `BALL_ATOM_DEFAULT_ELEMENT = &Element::UNKNOWN` →
    atomic number 0 = the born `element_indices_` default. name/charge/pos/vel/
    force/radius/formal_charge already match.
  - **Concurrency: deleting the default-ctor block is a net WIN, not just perf.**
    `bindToStore_` already holds `orphanMutex` across `allocate_atom` (atom.C:43);
    the `INITIAL_WRITES_LOCK_` block exists *only* because the post-bind default
    writes re-evaluate `store_->position(idx)` against a base pointer a concurrent
    reallocation could stale (the R11 race, atom.C:192-200). Born-default writes
    happen *inside* the mutex-protected `allocate_atom`, so the default ctor needs
    **zero** post-bind writes → the R11 stale-base window is *eliminated* for
    default construction (it was only guarded before). The copy ctor + the 3rd
    ctor keep their blocks (they write source values, not defaults).
  - **The `"?"` offset subtlety:** type_name has two representations — the live
    `type_name_strings_` String column (read-truth for `getTypeName`) AND the
    `type_name_offsets_`→`string_pool_` path (persistence-truth, StoreFormat).
    Born-default must set BOTH consistently. Naively calling `set_type_name(idx,"?")`
    per atom re-does a hash probe (the cost .D attacks). Instead: cache
    `default_type_name_offset_` (lazily interned on first `allocate_atom` via
    `intern_name("?")`; `0` sentinel = not-yet-computed since `"?"`≠empty → offset
    ≥1), set `type_name_offsets_[idx] = default_type_name_offset_` +
    `type_name_strings_[idx] = "?"` — BOTH set in the SAME `allocate_atom` write so
    no serialization-observable point sees string/offset drift (a freshly created-
    then-serialized atom with no explicit `setTypeName` must round-trip `"?"` through
    the offset→pool path, not `""`). **Lazy, not eager in the ctor** — eager would
    re-add a per-System cost, the very thing .C removes.
  - **`default_type_name_offset_` MUST reset to `0` on EVERY pool-reset path**
    (R1 FLAW #3): `compact()`/`rebuild_string_pool_` (which re-intern every atom's
    `type_name_strings_`) AND **`MoleculeStore::clear()`** — `clear()` drops
    `string_pool_`+`string_intern_` (moleculeStore.C ~1048-1050), so a stale cached
    offset would otherwise be written into a fresh pool by the next `allocate_atom`.
    Recompute lazily on next use (avoids the R13.2-class stale-offset trap).
    Per-System stores are single-threaded (D7); the orphan store serialises the lazy
    init via orphanMutex.
  - Touch points: `allocate_atom` (both fresh + free-list-reuse paths),
    `rebuild_string_pool_` + `compact()` + **`clear()`** (reset cache), one new private
    member, and delete the `atom.C` default-ctor block.
  - Verify: full ctest + 100-run MoleculeStore stress (orphan path) + benchmark +
    Codex review. New regression tests (R1 nice-to-haves): (1) default Atom create →
    serialize without `setTypeName`, assert `"?"` round-trips via the offset path;
    (2) free-list reuse — release then reallocate, assert type/type-name; (3) compact
    AND clear — default atom → compact-or-clear → reallocate, assert
    `type_name_offsets_` point into the CURRENT pool.
  - **Land .A and .C as SEPARATE atomic patches** (R1) so the benchmark delta shows
    which hotspot moved. **Code lands via the GSD execution flow (not direct edit).**
- **HCP-1P.B — kill the `std::string(s.c_str())` round-trip.** `Atom::writeStoreName_`/
  `writeStoreTypeName_` (and the 3 other call sites in `atom.C`) build a throwaway
  `std::string` from `String::c_str()` (forces strlen + alloc + copy). `BALL::String`
  **encapsulates** a `std::string` and exposes `operator const std::string&() const`
  (string.h:255) → pass the `String` straight into `set_name(idx, const std::string&)`
  for a **zero-copy reference bind**; `intern_name` then allocates only when the name
  is not already pooled. This is the **down-payment slice of v2.3** (`BALL::String`→
  `std::string`, roadmap §4) taken in the hot path now, no signature change.
  **(R1 WEAK #4) Scope must ALSO reach inside the store:** `set_name`/`set_type_name`
  themselves still do `name_strings_[i] = String(s.c_str())` (moleculeStore.C:1189,
  1208) — a *second* redundant reconstruction. .B removes that too (assign from `s`
  directly). Note: once .A deletes the default-name writes, the remaining hot caller
  is the PDB/parser `setName` path — so .B's payoff is mostly there, not in the ctor.
- **HCP-1P.C — lazy well-known property columns.** `PropertyColumnRegistry`'s ctor
  eagerly `predeclareWellKnown_()` (10 heap-allocated columns × 2 registries per
  store) — the dominant **System-creation** cost with no atoms present. Predeclare
  lazily (materialise the column object on first write to a well-known name) and/or
  share ONE immutable well-known *schema* across stores (schema shared, per-store row
  data NOT shared), so an empty System pays ~nothing.
  **(R1 FLAW #5) Lazy changes observable registry behaviour — lock the contract:**
  today `columnCount()==10` and `findColumn("PARTIAL_CHARGE")!=nullptr` immediately
  after construction; lazy makes those reflect only materialised columns. Define the
  lazy semantics precisely: well-known names stay **exempt from the dynamic-cap
  accounting** (`max_dynamic_`), a write to a well-known name materialises it AS
  well-known (not as a dynamic column), and any changed `columnCount`/`findColumn`
  behaviour is deliberate **with the affected tests updated**. Verify property parity
  + JSON column-set round-trip determinism.
- **HCP-1P.D — predefined canonical-name pre-intern (CONDITIONAL / measure-first).**
  The string pool already **dedups** repeated names (offset reuse, D20), so storage is
  solved; the residual cost is the per-name **hash+probe** in `intern_name`. **(R1 WEAK
  #7) DEMOTED to measure-first:** with .A's default-name writes gone and dedup already
  installed, .D only saves the hash on *real* import names — run AFTER .A+.B and only
  if a profile of the PDB/import path shows `intern_name` hashing is still a hot spot.
  **(R1 FLAW #6) Layering — KERNEL must NOT know FragmentDB.** FragmentDB is STRUCTURE;
  `MoleculeStore` is KERNEL and cannot include/depend on it. So the seeding is **driven
  from the STRUCTURE/FORMAT import path** (which already loads FragmentDB) calling the
  generic KERNEL API `store.intern_name(name)` for the canonical residue/atom names —
  the store stays ignorant of FragmentDB. (Optionally a tiny KERNEL-side
  direct/perfect-hash table keyed on a STRUCTURE-supplied name list; still no
  STRUCTURE include in KERNEL.) Note: pre-interned offsets are NOT permanent — `compact()`
  rebuilds from live columns, so unused predefined names don't survive; the win is
  per-import-session hash avoidance, not fixed offsets. Verify: PDB golden-corpus parse
  offsets stable + intern determinism (I11).
- **HCP-1P gate.** Re-run the v2.0 baseline benchmarks (CompositeCreation/Clone,
  KernelCreation/Clone/Iteration). **(R1 WEAK #9) The BALLStones coarse 0.01s timer at
  N=40000 is too blunt for sub-µs/op claims** — strengthen to **repeated runs (report
  median + spread) with larger N and/or a higher-resolution timer**, and treat the
  pulled-forward V21-CI-PERF-GATES (@H8) comparator as an **interim measurement-only
  sanity gate, NOT a sole definitive GO** (guard against a false GO on timer noise).
  Confirm create/clone return to ≤ v2.0 baseline and iteration is unregressed. Codex
  close-review → GO.

**Next milestone — HCP-2 (the collapsed handle API).** The user-visible heart
of the collapse, still dual-existence (handles read the HCP-1 role columns;
v0 classes remain until H4). Sub-steps, each green + committed:
- **HCP-2a — role-aware value handles.** Add `Molecule` + `Fragment` (+ reuse
  `Atom`) handles over `ContainerHandleBase`: `getMoleculeRole`/`getFragmentRole`
  /`getResidueKind`/`getSSKind` + role-asserting `getID`/`getInsertionCode`
  (debug/Python `BALL_CONTAINER_HANDLE_CHECKS`). Keep the 8 typed `*Handle`
  as `[[deprecated]]` aliases for now.
- **HCP-2b — `BALL::StructureQuery` namespace.** `residues`/`chains`/
  `residueByID`/`nTerminal`/`cTerminal` (polymer-residue, H1bP-5)/
  `secondaryStructureOf`/`secondaryStructuresOf`/`fragmentsByRole` +
  canonical-depth-honouring iteration; ship the `[[deprecated]]`
  role-filtered iterator alias (the 40-file `ResidueIterator` surface, D-HC5).
- **HCP-2c — land the deferred mirrors (the role/property rework point).**
  (i) container `setProperty`/`clearProperty` mirror incl. property-driven
  `ResidueKind`/`IS_SOLVENT` → role refinement (so MoleculeRole stops
  returning UNKNOWN for solvent/ligand/ion); (ii) full-subtree-replacement-on-
  rooted mirror (`set`/`operator=`/`persistentRead` + cross-store move (KR1
  HIGH-1) + insertParent/`replace` materialise-new-member) — re-materialise the
  affected subtree. Lift the H2d scope restriction once landed.
- **HCP-2d — SS-as-annotation (D-HC4).** Convert SecondaryStructure from an
  owning Chain→SS→Residue layer to a non-owning annotation belonging to its
  chain (start/end residue refs + sheet payload); rewrite the chain→SS→residue
  traversals. (Heaviest structural item; may split into its own sub-phase.)
- **HCP-2 close-review** (Codex) → GO.

NOTE: `ContainerKind` shrink to `{MOLECULE,FRAGMENT}` is deferred to the H4
flip (HCP-4) — during dual existence the v0 kind still drives role derivation
(HCP-1a), so shrinking early would force premature consumer changes.

**Then:** HCP-3 (= H3, module re-open + consumer migration onto the role API,
FORMAT/PDB last with the corpus smoke) → HCP-4 (= H4 flip: delete v0 typed
classes, `sizeof(Atom)` drops, retire dual existence) → H5 (Bond unify) →
H6/H7 (JSON v2 + VIEW/MSVC) → H8 (= HCP-5, D13 + PDB-fidelity gate) →
**v2.2.0**.

### HCP-0 — design-lock (this IS `H1b′`)
Deliverable: a design doc (`V22-H1bPRIME-DESIGN.md`) + decisions appended to
`V22-DECISIONS.md` that LOCK, concretely enough to implement without
discovery during H3:
1. **`ContainerKind` final set** + the `Role` enums (exact enumerators,
   storage width, default/unknown values). Decide typed-column vs sparse-
   property per field (HC1: core identity = typed columns; rare = sparse).
2. **Atom PDB-field storage:** which become atom columns (serial, altLoc,
   occupancy, B-factor, ATOM/HETATM record kind, segment id) vs sparse
   properties; the no-`dynamic_cast<PDBAtom*>` consequence.
3. **Collapsed handle API surface:** `Molecule`/`Fragment`/`Atom` value
   handles; which getters are role-aware Fragment methods vs `StructureQuery`
   free functions; the deprecated-facade policy (HC1-4: aliases buy little —
   facade wrappers only as migration shims, not the model).
4. **Audited consumer matrix (HCP1-2) — the gate's load-bearing deliverable.**
   NOT just "grep the 200 files": an actual call-site audit that names the
   consumer clusters and, for each, the methods/types they touch + the
   required compile/test smoke that proves the role API serves them. Minimum
   clusters to audit:
   - **FORMAT/PDB** (PDBFile/PDBFileDetails `insertParent`/typed-tree build,
     PDBAtom field access, naming) — the heaviest + highest fidelity risk;
   - **STRUCTURE** selectors/predicates + FragmentDB/ResidueChecker/
     Peptides/NormalizeNames/SecondaryStructureProcessor;
   - **KERNEL** extractors/iterators (`residueBegin`/`chainBegin`/… typed
     iterators; `forEach*` predicates) and the legacy typed-class methods
     that stay live until H4;
   - **MOLMEC/QSAR/SCORING/DOCKING** residue/atom-type access;
   - **VIEW** (deferred to H7 but its typed-hierarchy assumptions noted).
   The role API + the method-relocation map (below) are designed AGAINST this
   evidence. If the audit reveals a method that does not relocate cleanly to a
   role-aware Fragment method or `StructureQuery` free function, that is a
   gate finding, not an H3 surprise.
5. **Method-relocation map:** every type-specific method (Residue torsions
   φ/ψ/ω, `getNTerminal`/`getCTerminal`, `Protein::getResidueByID`, Chain
   residue/SS access, `SecondaryStructure::getType`, `Residue::getFullName`/
   PDB naming) → its new home (role-aware Fragment method or `StructureQuery`),
   each entry traceable to a consumer-matrix cluster from (4).
6. **SS span model:** how a role=SS Fragment references its residue range
   (child edges vs an index span); overlap semantics.
7. **Canonical-depth invariants + role-filtered iteration contract**
   (`fragments(role=RESIDUE)` etc.) replacing typed iterators.
8. **Migration-guide skeleton** (Residue/Chain/PDBAtom/SS/terminal/torsion/
   typed-iterators → role API), seeded into `V22-API-BREAK-LEDGER.md`.
9. **PDB golden-corpus spec** for HCP-5 (the cases to cover) — see HCP-5.
10. **D-HC7: the narrow-flag decision** — reuse `BALL_CORE_ONLY` (and re-add
    the extension-module `IF(NOT …)` guards) vs introduce a dedicated
    `BALL_COLLAPSE_KERNEL_ONLY` flag (avoids re-overloading the v2.0 flag).

**Gate verdict (the H1b′ gate):** Codex review of HCP-0. If the above do not
LOCK cleanly + quickly → **STAGE the collapse to v2.3** (keep v2.2's landed
typed handles; accept the double consumer migration). An unstable v2.2 public
surface is worse than a double migration (HC1 §5).

### HCP-1 — store role model (collapse-narrowed build)
- **Task 0 (prerequisite):** restore the KERNEL-only build partition (§1
  correction) per D-HC7 and confirm a clean narrowed build + KERNEL ctest
  subset on current HEAD (the attributable baseline).
- Shrink `ContainerKind` to `{MOLECULE, FRAGMENT}` (System = root row, not a
  kind) — or keep a thin kind tag + a `role` column (decided at HCP-0).
- Add `role` columns (Molecule/Fragment role) + `ResidueKind`/SS-kind payload
  + atom PDB-field columns to the store (extends D58 payload).
- Update `materialiseContainer_` to populate roles from the v0 typed class
  (the existing RTTI dispatch already distinguishes Protein/Residue/SS — it
  now writes a *role*, not just a kind).
- **Deliver the deferred scalar+role mutation mirror (HCP1-3 — NOT
  re-deferred).** H2b mirrors topology only and parked scalar mirroring so it
  is built ONCE here against the role-payload columns. HCP-1 MUST mirror, on
  mutation (clear/swap/post-root setters), the full field set:
  - container **name**, **id**, **insertion-code**, **SS-type**;
  - **Molecule role**, **Fragment role**;
  - **ResidueKind** + SS payload (helix-class/sheet-id/strand/sense);
  - former-`PDBAtom` scalar fields **that become atom columns** (per HCP-0 #2:
    serial, altLoc, occupancy, B-factor, record kind, segment id).
- **HCP-1 BLOCKS H2d (hard prerequisite).** H2d is the randomized full-surface
  parity sweep; it stays RED for scalar-mutating ops until this mirror lands.
  Therefore the v2.2 sequence is: H2b (topology, DONE) → H2c (iterators) →
  **HCP-1 scalar+role mirror** → H2d (randomized sweep, now green) → H3. H2d
  must NOT be scheduled before HCP-1, or it cannot pass.
- Green: narrowed build + `HierarchyParity_test` (role + scalar columns
  mirrored after clear/swap/setters) + `MoleculeStore`/handle tests.

### HCP-2 — collapsed handle API (collapse-narrowed build)
- Collapse the 8 typed `*Handle` (H1b) → `Molecule`, `Fragment` (+ `Atom`)
  role-aware handles reading the role columns; per-kind getters become
  role-aware Fragment methods.
- Add the `BALL::StructureQuery` free-function namespace (navigation:
  residues-of, chain-of, N/C-terminal, residue-by-id; role-filtered
  iteration).
- v0 typed classes stay (dual existence). New KERNEL handle tests
  (`RoleHandle_test`, `StructureQuery_test`) green under the narrowed build.
- **End-of-HCP-2 = the H1b′ gate is fully satisfied in code for KERNEL.**

### HCP-3 — progressive module re-open (this IS H3a/H3b/H3c)
Remove the extension-module guards (restored in HCP-1 task 0) in dependency
clusters (mirror the v2.0 Track-B order), migrating each cluster's consumers
to the role API. The consumer-matrix clusters from HCP-0 #4 drive what each
step must touch:
- **H3a:** KERNEL-facing + STRUCTURE (Selector/Expression are KERNEL-facing;
  typed-iterator → role-filtered conversions start here).
- **H3b:** MOLMEC / QSAR / SCORING / DOCKING.
- **H3c:** FORMAT (PDB/PDBFileDetails/naming/residue — the heaviest, builds
  Molecule/Fragment(role)/Atom instead of typed trees; PDBAtom fields →
  atom columns) + NMR / ENERGY / SOLVATION.
Each cluster: its own commit + Codex review + green ctest for that cluster.
**H3c acceptance (HCP1-4) requires the PDB golden-corpus parse→write→parse
structural-equivalence SMOKE** (a representative subset of the HCP-5 corpus) —
FORMAT cannot be marked green on a typed-tree-removed reader/writer without
it; the corpus regressions surface at the cluster, not at the H8 release gate.
VIEW migration is **H7** (already scheduled); pyBALL rebaseline already
anchored ≥ H4.

### HCP-4 — the flip (this IS H4)
- Delete the v0 typed classes (Protein/NucleicAcid/Chain/Residue/Nucleotide/
  SecondaryStructure/AtomContainer/PDBAtom) + their inline tree state; the
  store role model is the sole source of truth; retire dual existence + the
  parity test. **Far fewer classes to delete** than the pre-collapse plan
  (only System/Molecule/Fragment/Atom remain).
- `sizeof(Atom)` drops (the D13 path) — unchanged from the roadmap.

### HCP-5 — PDB/structural fidelity gate (added to H8)
- Golden PDB corpus (protein/NA/ligand/water/ion/insertion-codes/altLoc/
  HETATM/TER/CONECT/SSBOND/HELIX/SHEET/TURN/CRYST1/skipped records).
- parse→write→parse **structural equivalence** assertion (the HC1 promise).
- no-`dynamic_cast<PDBAtom*>` regression grep gate; debug/Python wrong-role
  assertion test. Lands alongside D13 at H8.

---

## 3. v0-class disposition under dual existence (the timeline)

| Stage | v0 typed classes (Protein/Chain/Residue/PDBAtom/…) | role model |
|---|---|---|
| now (H2b) | source of truth; bound to store rows by `container_row_idx_` | kind tag + payload (typed) in store; mirror = topology |
| HCP-1/HCP-2 (narrowed build) | **still source of truth**; unchanged externally | role columns + collapsed handles land ALONGSIDE; read-only mirror |
| HCP-3 (module re-open) | still source of truth; consumers migrate to role API one cluster at a time | role API is the consumer surface |
| HCP-4 (flip) | **deleted** | store role model = sole source of truth |

Key invariant (D60, unchanged): the store table is a *verified mirror* until
H4; the flip inverts it. The collapse does NOT move the flip earlier — it
only changes *what* the handles/columns look like before and after.

---

## 4. Risks specific to this sequencing (+ mitigations)

1. **The narrowed build hides downstream breakage.** The role API may compile
   + pass KERNEL tests but be awkward/insufficient for FORMAT/STRUCTURE, only
   discovered at HCP-3. → *Mitigation:* HCP-0's **audited consumer matrix**
   (#4) + method-relocation map are derived from the ACTUAL consumer
   call-sites with a required compile/test smoke per cluster, so the API is
   designed against real usage, not guessed. Treat HCP-0 as consumer-driven,
   not KERNEL-driven. A method that does not relocate cleanly is a GATE
   finding (blocks the H1b′ verdict), not an H3 surprise.
2. **The H1b′ gate is on the critical path** (blocks H3). If HCP-0 can't lock
   fast → stage the collapse out of v2.2 (§5) rather than letting v2.2's
   surface churn. Decision owner: maintainer, at the gate.
3. **PDB fidelity (the #1 correctness risk).** Regressions compile clean and
   corrupt science. → HCP-5 golden-corpus gate is mandatory; additionally run
   it as a *smoke test during HCP-3 H3c* (not only at H8) so FORMAT breakage
   surfaces at the cluster, not at release.
4. **Loss of compile-time typing** (`Residue r` → `Fragment`+role). →
   role-aware accessors validate role in debug/Python builds; wrong-role
   assertion test (HCP-5).
5. **Double-handling the scalar mirror.** H2b deferred typed name/id
   mirroring precisely so it is built once here against role columns — this
   plan must DELIVER that scalar/role mirror in HCP-1 (not re-defer it), or
   the H2d randomized parity sweep stays red.
6. **CORE_ONLY drift.** `BALL_CORE_ONLY=ON` is exercised by a smoke build
   (BALLIncludes.cmake:30) but the extension-module guards were since removed
   (§1 correction) — the partition must be RESTORED, not merely re-toggled. →
   HCP-1 task 0 restores the KERNEL-only guards (D-HC7) and confirms a clean
   narrowed build + KERNEL ctest subset of *current* HEAD before any collapse
   edits (a baseline, so later breakage is attributable).

---

## 5. Fallback coherence — staging the collapse out of v2.2 (HCP1-5)

If the H1b′ gate slips, the collapse stages to a LATER minor WITHOUT
disturbing the locked version plan. **The locked roadmap (§4a) already
assigns v2.3.0 to `BALL::String`→`std::string`**, so the fallback must NOT
claim v2.3 carries both the collapse AND String as "one break wave". Pick a
priority (maintainer decides at the gate):

- **Default fallback — String keeps v2.3, collapse → v2.4 (then repo split →
  v2.5).** Honours the locked §4a assignment; each minor stays one
  clearly-documented break (String in v2.3; collapse in v2.4; package/repo
  split shifts to v2.5). Cleanest w.r.t. the locked plan.
- **Alternative — collapse takes v2.3, String → v2.4.** Only if the
  maintainer judges the collapse more urgent than String; this REQUIRES
  re-locking §4a (swap the v2.3/v2.4 contents) — not a silent move.
- In **either** case: v2.2 ships with the **landed typed handles** (H1b) +
  the flip (H4) over the typed model — a complete, releasable milestone — and
  the cost is the double consumer migration (HC1 §5, rejected as default but
  acceptable as fallback). Version band stays 2.x (D53).

Coherent because the roadmap's staged minors already treat each minor as an
independent break wave; staging the collapse is a *scheduling* move. What is
NOT permitted without a roadmap re-lock is co-locating two semver-major-sized
breaks (String + collapse) in a single minor.

---

## 6. Open decisions for the gate (HCP-0 must answer)

- D-HC1: `ContainerKind` final set — `{MOLECULE, FRAGMENT}` only, or keep a
  small kind tag + orthogonal `role`? (Affects the H1a table + JSON schema.)
- D-HC2: atom PDB-field storage split (typed columns vs sparse) — exact list.
- D-HC3: `StructureQuery` namespace vs Fragment methods — the boundary.
- D-HC4: SS span representation (child edges vs index range) + overlap rules.
- D-HC5: deprecated-facade policy — ship `Residue`-shaped shims in v2.2 or
  break hard? (HC1-4: shims buy little; default = break hard + migration
  guide; maintainer may opt for shims to soften downstream.)
- D-HC6: whether HCP-1/HCP-2 land as a *revision of H1b* (same milestone
  internal) or as explicitly-numbered new sub-phases in the roadmap table.
- D-HC7: the narrow-flag — restore the `IF(NOT BALL_CORE_ONLY)` guards around
  the extension-module includes (reuse `BALL_CORE_ONLY`) vs introduce a
  dedicated `BALL_COLLAPSE_KERNEL_ONLY` flag (avoids re-overloading the v2.0
  semantics). Either way HCP-1 task 0 restores a real KERNEL-only partition.

---

## 7. Status — CONVERGED

**HCP1 = NEEDS-REVISION → revised → HCP1b = AGREE (2026-05-21).** This plan is
the canonical sequencing for the hierarchy collapse; its HCP-0…HCP-5 mapping
is folded into `V2X-ROADMAP.md` §2 (the `H1b′` slot). Reviews:
`V2X-HIERARCHY-COLLAPSE-PLAN-HCP1.md` (+ HCP1b confirmation).

The five HCP1 items:
(1) corrected the false `BALL_CORE_ONLY` premise — it only gates PYTHON today;
HCP-1 task 0 restores a real KERNEL-only partition (§1, HCP-1, D-HC7);
(2) HCP-0 strengthened from "grep" to an audited consumer matrix (§2 HCP-0 #4);
(3) HCP-1 scalar+role mirror made an explicit H2d-blocking deliverable with
the exact field list (§2 HCP-1); (4) PDB golden-corpus smoke promoted into the
HCP-3/H3c acceptance gate (§2 HCP-3 + table); (5) v2.3 fallback de-conflicted
with the locked String-in-v2.3 assignment (§5). Next: re-review (**HCP1b**);
on AGREE, fold the HCP-0…HCP-5 mapping into `V2X-ROADMAP.md` §2 (replacing the
prose `H1b′` note). Reviews: `V2X-HIERARCHY-COLLAPSE-PLAN-HCP1.md`.
