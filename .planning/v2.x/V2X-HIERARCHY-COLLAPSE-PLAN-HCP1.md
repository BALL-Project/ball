Verdict: NEEDS-REVISION

# BALL 2.x — Hierarchy-Collapse Phase Plan HCP1 Review

1. Fix the `BALL_CORE_ONLY` premise before relying on it for bisection. The
   plan says `BALL_CORE_ONLY=ON` builds only COMMON/CONCEPT/DATATYPE/MATHS/
   SYSTEM/KERNEL and excludes FORMAT/STRUCTURE/etc. (`V2X-HIERARCHY-COLLAPSE-PLAN.md`
   §1, lines 44-48), then uses that as the spine for HCP-1/HCP-2 and a later
   HCP-3 module re-open (§2, lines 122-156). Current CMake contradicts that:
   `cmake/BALLIncludes.cmake` unconditionally includes PLUGIN, FORMAT,
   STRUCTURE, XRAY, NMR, ENERGY, SCORING, MOLMEC, QSAR, SOLVATION, and DOCKING
   before the only `IF(NOT BALL_CORE_ONLY)` guard at lines 145-148, and
   `ADD_BALL_SOURCES` has no filtering hook (`cmake/BALLMacros.cmake` lines
   44-55). Required change: either restore true CORE_ONLY gating to match the
   roadmap premise, or rewrite HCP-1/HCP-2/HCP-3 around the modules that are
   actually compiled today. As written, "consumer churn switched OFF" (§1,
   lines 55-57) and "flip CORE_ONLY=OFF and re-enable in dependency clusters"
   (§2, lines 147-156) are not buildable claims.

2. Strengthen HCP-0 from "grep the 200 files" into an audited consumer
   matrix. The plan correctly identifies the risk that KERNEL-only success can
   hide FORMAT/STRUCTURE insufficiency (§4, lines 194-199), and HCP-0 requires
   a method-relocation map (§2, lines 105-108). That is necessary but not yet
   enough, especially if current CORE_ONLY already compiles substantial
   FORMAT/STRUCTURE surface. Required change: HCP-0 must name the audited
   call-site clusters and required compile/test smoke for each, including
   FORMAT/PDB, STRUCTURE selectors/predicates, KERNEL extractors/iterators, and
   the legacy typed class methods that remain live until H4. The mitigation
   should be consumer-driven evidence, not just a relocation table.

3. Make the scalar/role mirror a hard H2d prerequisite and define its exact
   parity fields. HCP-1 does not silently re-defer it: it says role columns,
   role payload, atom PDB-field columns, `materialiseContainer_`, and the
   deferred scalar-field mirror land in HCP-1 (§2, lines 122-134), and the risk
   section correctly says H2d stays red otherwise (§4, lines 210-213). Required
   change: make HCP-1 explicitly block H2d, and list the mirrored mutation
   fields: container name/id/insertion-code/SS-type, molecule/fragment role,
   residue/SS payload, and former `PDBAtom` scalar fields if those become atom
   columns. That closes the gap between "materialise from v0" and "randomized
   parity after scalar mutations".

4. Promote the PDB golden-corpus smoke from a risk note into the HCP-3c gate.
   The corpus spec is adequate for the HC1b promise: protein/NA/ligand/water/
   ion, insertion codes, altLoc, HETATM, TER, CONECT, SSBOND, HELIX/SHEET/TURN,
   CRYST1, and skipped records (§2, lines 168-173; HC1b confirmation lines
   186-188). The plan also correctly says the smoke should run during H3c, not
   only at H8 (§4, lines 203-206). Required change: add that smoke test to the
   HCP-3/H3c gate table and cluster acceptance (§2, lines 147-156), so FORMAT
   cannot be marked green without a parse-write-parse structural-equivalence
   check.

5. Fix the v2.3 fallback wording. Staging the collapse to v2.3 is coherent
   with HC1b and the roadmap gate (`V2X-HIERARCHY-COLLAPSE.md` §5;
   `V2X-ROADMAP.md` §2, lines 64-76), but the plan currently says v2.3 can do
   both `BALL::String` removal and the collapse while "one clearly-documented
   break per release" still holds (`V2X-HIERARCHY-COLLAPSE-PLAN.md` §5, lines
   223-230). The locked roadmap assigns v2.3.0 to `BALL::String` removal
   (`V2X-ROADMAP.md` §4a, lines 177-188). Required change: specify priority if
   the collapse slips: either collapse consumes v2.3 and String moves to a
   later minor, or String remains v2.3 and collapse moves later. Do not claim
   both are one break wave unless the roadmap explicitly re-locks that.

No new objection to the locked WHAT. The role taxonomy, canonical depth,
SS-as-span, normalized PDB promise, dual-existence timeline, and the claim
that H1a/H2 row/edge machinery mostly survives remain consistent with HC1b and
the roadmap once the sequencing issues above are corrected.

## HCP1b confirmation

Verdict: AGREE

1. ADDRESSED — `BALL_CORE_ONLY` is corrected as PYTHON-only today, with true
   KERNEL-only partition restore made HCP-1 task 0 (`V2X-HIERARCHY-COLLAPSE-PLAN.md`
   §1, §2/HCP-1; confirmed `BALLIncludes.cmake:145-148`).
2. ADDRESSED — HCP-0 now requires an audited consumer matrix with named
   clusters and compile/test smoke, including FORMAT/PDB, STRUCTURE,
   KERNEL extractors/iterators, legacy typed-class methods, and downstream
   consumers (`V2X-HIERARCHY-COLLAPSE-PLAN.md` §2/HCP-0 #4).
3. ADDRESSED — the scalar+role mirror is an explicit H2d-blocking HCP-1
   deliverable with the required mirrored field list and coherent
   H2b→H2c→HCP-1→H2d ordering (`V2X-HIERARCHY-COLLAPSE-PLAN.md` §2/HCP-1).
4. ADDRESSED — PDB golden-corpus parse→write→parse structural-equivalence
   smoke is now in the HCP-3/H3c acceptance gate, not only H8
   (`V2X-HIERARCHY-COLLAPSE-PLAN.md` §2/HCP-3).
5. ADDRESSED — the fallback no longer treats String + collapse as one v2.3
   break wave; it defines priority choices and requires roadmap re-lock before
   co-locating them (`V2X-HIERARCHY-COLLAPSE-PLAN.md` §5;
   `V2X-ROADMAP.md` §4a).

New issue: none. Planning convergence only; no code approval implied.
