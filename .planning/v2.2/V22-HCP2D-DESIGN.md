# V22 HCP-2d — SecondaryStructure-as-annotation (D-HC4): design investigation + scoping fork

**Status:** ⏸ **DEFERRED TO H4** (user decision 2026-05-24 + Codex HCP-2d-R0, commit
1ffaa01ed). The SS owning→annotation flip is NOT forced by the collapse before the H4
source-of-truth flip; it folds into H4's consumer migration (when v0 classes are deleted +
VIEW is migrated). Do NOT do the ~58-file v0+VIEW change during dual existence. This doc
remains the investigation of record; re-open at H4. Next milestone: PR-removal (task #61).

HCP-2d is the FINAL HCP-2 sub-step and, per the collapse plan, "the heaviest structural item;
may split into its own sub-phase." This doc captures the codebase investigation + the strategic
scoping fork, because HCP-2d differs in KIND from 2a/2b/2c (kernel-internal, dual-existence-safe,
well-tested store/handle work).

## Goal (D-HC4)
Convert `SecondaryStructure` from an OWNING hierarchy layer (Chain → SS → Residue) into a
NON-owning ANNOTATION belonging to its Chain (start/end residue refs + helix/sheet payload);
the collapsed canonical depth becomes System → Molecule → Fragment(CHAIN) → Fragment(RESIDUE)
→ Atom, with SS as an annotation OUTSIDE the ownership path.

## Investigation findings (codebase map)
- **SS today (secondaryStructure.h/.C):** `class SecondaryStructure : public AtomContainer`;
  only SS-specific field is `Type type_` (HELIX/COIL/STRAND/TURN/UNKNOWN). It OWNS its
  residues as Composite children; the span is IMPLICIT in the child list (no start/end
  fields). getResidue/countResidues/getNTerminal/getCTerminal operate over the children.
- **Ownership built at PDB read:** PDBFileDetails.C parses HELIX/SHEET/TURN records, then
  `Composite::insertParent(ss, initial, terminal, false)` REPARENTS the residue range under
  the SS (Chain→SS→Residue). Random-coil generation does the same.
- **Consumer surface: ~58 files.** 3 CRITICAL:
  1. **FORMAT/PDBFileDetails.C** — builds the nesting via insertParent + writes HELIX/SHEET/
     TURN from the SS objects (round-trip depends on SS owning its residues).
  2. **STRUCTURE/secondaryStructureProcessor.C** — creates SS, assigns residues, deletes SS.
  3. **VIEW/MODELS/backboneModel.C + cartoonModel.C + standardColorProcessor.C** — read a
     residue's enclosing SS (via ancestor walk) to drive cartoon/backbone GEOMETRY + coloring.
     **VIEW is the project's non-negotiable deliverable (CLAUDE.md) and is NOT in the current
     kernel-only build (it's Phase 4a).**
  Plus NMR (shift prediction reads SS type), iterators (SecondaryStructureIterator,
  ResidueIterator descends THROUGH SS), Chain/Protein/Residue accessors (getSecondaryStructure,
  countSecondaryStructures, residue.getSecondaryStructure ancestor-walk), standardPredicates,
  extractors, molecularControl tree widget, Python bindings, persistence.
- **Already collapse-ready (written ROLE-based to survive this):** the v2.2 mirror stores SS
  as a fragment-role row with `ss_type` payload; `StructureQuery::secondaryStructureOf` walks
  parents for a SECONDARY_STRUCTURE-role ancestor; `residues()` finds RESIDUE-role fragments
  at ANY depth (under chain OR under SS). These survive the ownership change unchanged.
- **No explicit start/end today** — span = the child list. An annotation needs a NEW span
  representation (start/end residue refs or an index range).
- **Persistence:** PDB + JSON StoreFormat + persistentRead/Write all serialize the SS→residue
  nesting (children). Changing ownership affects round-trip unless span is extracted/stored.

## The dual-existence tension (the crux)
Through dual existence the **v0 tree is the source of truth** and the store table is a verified
forward MIRROR (HierarchyParity asserts descTable(v0) == descV0). SS-as-annotation means
residues sit DIRECTLY under the chain with SS outside the ownership path. But:
- If **v0 SS stops owning residues** → a v0 BEHAVIOR CHANGE rippling through all ~58 consumers
  (incl. VIEW geometry + PDB round-trip + the iterators) — the heaviest, highest-blast-radius
  change in the whole collapse, touching the sacred VIEW layer that isn't even in the current
  build to test against.
- If **v0 keeps owning** (no consumer breakage) but the MIRROR represents SS as annotation
  (residues under chain) → the store DIVERGES from v0, breaking the parity invariant + the
  HierarchyParity machinery that has guarded every step.
So the "obvious" full conversion is NOT cleanly dual-existence-safe. This is why the plan
flagged it "may split into its own sub-phase."

## Scoping options (the fork to resolve before design-lock)

**Option A — Full conversion NOW (v0 ownership change).** Change v0 so residues are direct
chain children + SS is a non-owning annotation; rewrite all ~58 consumers (PDB build/write,
SS processor, VIEW geometry/coloring, iterators, predicates, NMR). HIGHEST risk: touches VIEW
(the non-negotiable deliverable, untestable in the kernel-only build) + format round-trip;
breaks the dual-existence "v0 is truth, store mirrors it" model mid-collapse.

**Option B — Additive annotation layer NOW (dual-existence-safe).** Keep v0 SS owning residues
(zero consumer breakage). ADD an annotation REPRESENTATION (start/end residue refs + a
role-based `secondaryStructureOf`/annotation query) over the handles/store ALONGSIDE the
existing owning mirror, so the H4 flip can DROP the owning layer cleanly. Establishes the
annotation API + span representation without a v0 change. Lower risk; but does not literally
"convert" — the ownership flip itself defers to H4.

**Option C — Defer HCP-2d entirely to the H4 flip.** At H4 the v0 classes are DELETED and the
store becomes the source of truth; at that point residues simply go directly under the chain
in the store and SS is an annotation — the ownership "change" is moot (no v0 to keep in sync,
no parity invariant to break, VIEW is migrated as part of the flip). Proceed now to the other
ready roadmap items (HCP-3/H3, the AGREED PR-removal milestone #61, the deprecated-iterator
alias HCP-2b.2, H2c handle iterators) and fold SS-as-annotation into H4's consumer migration.
StructureQuery + role-aware handles already make SS role-based, so nothing is blocked.

## Recommendation (pre-Codex)
Lean **Option C (defer to H4)** or **Option B (additive, dual-existence-safe)** over Option A.
Rationale: the collapse's dual-existence discipline (v0 = truth, store = verified mirror) is
what has made every step safe + reviewable; Option A abandons it for the single change that
also touches VIEW (the project's core deliverable, untestable here). The role-based handles +
StructureQuery were DESIGNED so SS ownership is invisible to consumers of the new API, so the
actual ownership flip is naturally an H4-flip concern, not a dual-existence concern.

[Codex HCP-2d-R0: pressure-test the 3 options' technical feasibility + the dual-existence
tension; is Option A's v0-change really forced, or is B/C cleanly correct? Recommend.]

## HCP-2d-R0 scoping review

**RECOMMENDED OPTION: C.** Defer the ownership flip to H4; optionally do a tightly bounded
Option-B precursor only if it is a derived/rebuilt annotation side table/API and explicitly not
the parity mirror. Option A is not forced by the collapse before H4.

1. **Dual-existence tension is real.** Live code confirms `SecondaryStructure` is an owning
`AtomContainer` with only `type_`; PDB read uses `Composite::insertParent(...)` to reparent
residue spans under SS; PDB write derives spans from `beginResidue()`/`rbeginResidue()`; parity
tests mirror the exact v0 child topology (`descTable(v0) == descV0`). A third framing that is
both genuinely dual-existence-safe and a real conversion does not exist unless the source of
truth changes: a collapsed "view/projection" over the faithful mirror is safe, but it is
projection/additive work, not the ownership conversion.
2. **Option A is an H4 concern, not a pre-H4 collapse requirement.** The collapse APIs already
route through roles: handles expose `FragmentRole::SECONDARY_STRUCTURE`, and
`StructureQuery::residues()` finds residues at either depth. Nothing about HCP-2/HCP-3 requires
v0 residues to move out from under SS before v0 deletion. Touching VIEW now is unjustified:
`backboneModel.C`/`cartoonModel.C` still use residue ancestor SS for geometry, and VIEW is not
covered by the current kernel-only build.
3. **Option B is safe only as separate derived annotation state.** Adding start/end residue refs
alongside the owning mirror does not break parity if the existing container table remains a
faithful `Chain->SS->Residue` mirror and the annotation representation is a non-authoritative
side table/query result. It can de-risk span encoding and lookup semantics, but it does not
perform the conversion and risks throwaway duplication unless bounded to H4 inputs.
4. **Option C does not block HCP-2/HCP-3.** The live `StructureQuery` and role-aware handles were
written depth-agnostic for exactly this interval; `StructureQuery_test` already pins residues at
both direct-chain and SS-owned depths. Consumer migration to handles can proceed while v0 keeps
owning residues, because the new API hides that ownership shape.
5. **Format/round-trip/parity reality check.** With B or C, PDB, JSON/store persistence, and
`HierarchyParity_test` stay unchanged because v0 remains truth and the mirror remains faithful.
With A, PDB read/write loses its current implicit span source unless new span fields and all
writers/readers are rewritten; residue ancestor queries, iterators, parity descriptors, and VIEW
geometry/coloring all change at once.
6. **Recommendation against A.** C best serves the end state, dual-existence discipline, the
VIEW-renders-molecules deliverable, and minimal redo. B is acceptable only as a small H4-enabling
annotation prototype derived from v0/store state; it should not replace the mirror. The collapse
does not force the v0 SS ownership change before H4; the forcing event is the H4 source-of-truth
flip/deletion of v0 classes, when parity no longer requires the store topology to match v0.
