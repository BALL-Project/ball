# V22 — Atom-as-pure-handle + GROMACS-SoA: reconciled design (decision of record)

**Status:** Authoritative synthesis of the dual deep-research pass
(Codex + Vibe) + independent GROMACS source study. Supersedes the
speculative parts of `H4-ATOM-AS-PURE-HANDLE-GROMACS-SOA-DESIGN.md`
(Vibe draft, commit `acae66147`) where they conflict.
**Authored:** 2026-06-12. HEAD `acae66147`. No code change — design only.

> **DECISION LOCKED (2026-06-12): Option A — GROMACS-faithful.** Keep
> AoS `Vector3` columns; repack SoA transiently inside hot kernels only;
> add interaction-term bonded arrays + derived `AtomRange` + (later)
> molblock-style instancing — **all API-additive, NO `Vector3&`
> retirement.** Option B (primary `Vec3SoA` + killing the mutable
> `Vector3&` API) is **explicitly deferred** to a profile-gated v2.3+
> decision and is NOT a prerequisite for GROMACS-class layout. See §3.
> Sequencing: §4 — everything follows the H4 flip; pre-H4 work is
> additive prep only.

Two independent researchers (Codex checked out the live GROMACS tree at
`/tmp/gromacs-src`; Vibe + my own web study cross-checked) converged.
This doc records the convergence, corrects the draft's two errors, and
frames the one real decision.

## 1. Convergent findings (both researchers + GROMACS source agree)

1. **The #1 win is removing the 376-byte heap `Atom` — which IS the H4
   flip.** Both rank it first: **1.5–4× on creation / load / clone**
   plus a large memory/cache drop. `KernelCreation_bench` is pure
   `new Atom` cost; GROMACS has *no* per-atom object in its state at all.
   **This is already the roadmap (H4 commit 8).** The research validates
   the existing plan rather than proposing a new one.

2. **GROMACS stores coordinates as AoS, not SoA.** `gmx::RVec =
   BasicVector<real>` (`real[3]`), `t_state.x/v` are `PaddedHostVector<RVec>`
   (`vectypes.h:47/351`, `paddedvector.h:173`) — contiguous AoS with
   *SIMD-padding tail*, NOT primary x/y/z SoA. SoA appears **only as
   kernel-local repacking** in the nonbonded path: `nbnxn_atomdata_t`
   copies `rvec*` into cluster `x`/`xq` buffers (`atomdata.h:90/396`,
   `nbnxm.cpp:155`). **Persistent global x/y/z SoA is MORE aggressive
   than GROMACS does.**

3. **Topology is separate from state and never an object graph.**
   `gmx_mtop_t` = `gmx_molblock_t` (a *block of identical molecules* —
   instancing) + `gmx_moltype_t` (`topology.h:72`). Bonded terms are
   **flat integer interaction lists** `InteractionList::iatoms[]`
   (`idef.h:291`) indexed by atom number — never a per-atom `Bond*`
   array. Identity across DD reorder is the global atom number +
   `gatindex[]` map (BALL's stable-id + generation analog).

4. **Bonds → store records + CSR + interaction-term arrays.** Drop
   `Atom::bond_[12]` (96 B) + `number_of_bonds_`. The substrate already
   exists: `BondRecord {a,b,order,type,flags}` (moleculeStore.h:66),
   CSR `bond_csr_off_/idx_` (moleculeStore.h:724), and `for_each_bond_of`
   (moleculeStore.h:425) already proves object-free access. Force fields
   precompute GROMACS-style term arrays at setup:
   ```cpp
   struct BondTerm    { uint32_t i,j;     uint16_t param; };
   struct AngleTerm   { uint32_t i,j,k;   uint16_t param; };
   struct TorsionTerm { uint32_t i,j,k,l; uint16_t param; };
   ```
   then iterate term arrays + SoA spans, not `atom->getBond(i)`.

5. **Tree stays scattered, ranges are derived.** Keep `ContainerRow` +
   ordered `ChildRef` (`_moleculeStoreInternal.h:443/467`) as the
   primary, editor-friendly representation. A strict "every residue
   always contiguous" invariant would force compaction on every
   interactive insert/delete. Instead derive a cached
   `AtomRange{begin,end,epoch}` for containers that *happen* to be
   contiguous in preorder (the `gmx_molblock_t` analog) and fall back to
   scattered iteration otherwise. Processors survive by yielding handles
   (`UnaryProcessor<AtomHandle>`, already built in H2c), not by keeping
   `Atom : Composite`.

6. **GC / mutation = deferred + lease-protected compaction.** Substrate
   exists: `free_list_` (moleculeStore.h:276), tombstones, `compact()`
   documented exclusive (moleculeStore.h:316), generation counter.
   Policy: insert/remove/modify **never compact**; compact only at
   explicit save/optimize/sim-start or idle past a threshold
   (`freed_slot_count > max(4096, live/8)`); **never** during a drag,
   renderer frame lease, undo transaction, or active traversal; long-lived
   UI state holds **stable-IDs, not indices or `Atom*`**.

## 2. Corrections to the Vibe draft

- **C1 — no persistent SoA x/y/z; do NOT dual-store position.** The
  draft's Part B adds `positions_x/y/z_` *alongside* `positions_` and its
  `setPosition` writes BOTH (1.5× memory + two writes per move). That
  contradicts the draft's own Part A (GROMACS keeps AoS) and is strictly
  worse. **Decision: one representation.** Default = keep AoS `Vector3`
  columns (preserves the reference API, §3 Option A); SoA is a separate,
  later, *replace-not-augment* migration (Option B). Never both.
- **C2 — call-site count.** Use Codex's measured scope: **783
  `getPosition()` occurrences across 164 files, 347 in
  MOLMEC/ENERGY/STRUCTURE hot paths**, plus 71 `getForce()` in hot areas
  (e.g. `bendComponent.C:160`, `amberNonBonded.C:1068`,
  `charmmNonBonded.C:1004`). Vibe's "3,106" over-counts (includes the
  `BondIterator::getPosition` inner-class + comments). The break is real
  but ~4× smaller than the draft states.

## 3. The one real decision: position storage

Both options keep Atom-as-handle, CSR bonds, separated tree, deferred GC.
They differ ONLY in the position/velocity/force column layout.

### Option A — GROMACS-faithful  ✅ SELECTED (2026-06-12)
Keep **AoS `Vector3` columns** (already in `moleculeStore.h:668`). Repack
to SoA **transiently inside the few vectorizable hot kernels only** (the
nbnxn pattern). Add interaction-term arrays (§1.4) + derived ranges (§1.5).
- **Preserves `getPosition()`/`position(i)` `Vector3&`** — zero of the
  783-site break. The reference into a `Vector3` column stays valid.
- This is *literally what GROMACS does*: AoS state, SoA only in kernels.
- Risk: LOW. Win: the bonded-term + range + handle wins (1.2–2.5×
  bonded, 1.1–2× scans) without an API break.

### Option B — primary SoA x/y/z  ⏸ DEFERRED (profile-gated, v2.3+)
Replace AoS columns with `Vec3SoA{x_,y_,z_ padded aligned}`. **Requires
killing the mutable `Vector3&` API** — kernels take spans
(`PositionSoA`/`ForceSoA`/`AtomRange`), scattered writes use value setters
(`setPosition(idx, v)`, `translateAtoms(indices, delta)`, `addForce(idx, v)`).
- Ceiling: **1.2–3× where loops actually vectorize** — and **zero gain
  where call sites stay scalar/object-based** (both reviewers flag this).
- Risk: HIGH (783-site break; a `Vector3&` proxy would preserve source
  compat but "defeats the point" by killing the aliasing/vectorization
  benefit — Codex).

**Recommendation:** Do **A** as the default end-state. Treat **B** as a
*measured* v2.3+ decision: implement it only if, after H4, a profile of
the actual energy/MD inner loops shows them vectorization-bound on
position loads. Do not pay an 783-site break on an unprofiled hypothesis.

## 4. Sequencing (both reviewers agree)

1. **Finish H4** (commit 8 deletes the 376 B `Atom`) — this is the #1
   win and is already in flight. Everything else is secondary.
2. **Pre-H4, only do API-additive prep:** span-based kernel APIs
   (`PositionSoA`/`ForceSoA`/`AtomRange`) + a compile gate that flags new
   mutable-`Vector3&` position dependencies. No storage change, no break.
3. **Post-H4 (v2.2 tail / v2.3):** interaction-term bonded arrays
   (§1.4), derived `AtomRange` fast path (§1.5) — both API-additive,
   measurable on `AmberFF_bench`/`CharmmFF_bench`/`KernelIteration_bench`.
4. **v2.3+ only, profile-gated:** Option B (primary SoA + `Vector3&`
   retirement), and separately spatial binning / cluster pairlists for
   nonbonded (a standalone project, "not unlocked automatically").

> Codex's blunt sequencing verdict: *"This should follow H4, not precede
> it… splitting x/y/z before `Atom::getPosition()` dies means fighting
> both the old object tree and the new storage model at once."*

## 5. Ranked speedups (tempered — engineering estimates, NOT profiles)

| # | Win | Magnitude | Workload | GROMACS precedent | Risk |
|---|-----|-----------|----------|-------------------|------|
| 1 | Remove 376 B heap `Atom` (= H4 flip) | 1.5–4× create/load/clone + memory | KernelCreation/Clone | no per-atom object in state | already roadmap |
| 2 | Interaction-list bonded loops | 1.2–2.5× bonded | AmberFF/CharmmFF | `InteractionList::iatoms` | setup correctness |
| 3 | Derived contiguous ranges | 1.1–2× scans | KernelIteration | `gmx_molblock_t` | mutation dirties ranges |
| 4 | SoA x/y/z + SIMD (Option B) | 1.2–3× **where loops vectorize, else 0** | energy/MD inner loops | nbnxm packing | HIGH — `Vector3&` kill |
| 5 | Spatial binning / cluster pairlists | 2–10× nonbonded | nonbonded AMBER/CHARMM | nbnxm grid/pairlists | separate project |
| — | **molblock instancing** (new) | memory: identical solvent/molecules share one moltype | load/memory of large solvated systems | `gmx_molblock_t.nmol` | needs an instancing layer BALL lacks |

All magnitudes need validation on the actual BALL benches before any
commit — the prior perf round already showed an "obvious" ABO win that
crashed under the real workload.

## 6. The five adversarial risks (both researchers)

1. **`Vector3&` retirement is the blast radius** — 783 sites, 347 hot. A
   proxy preserves source-compat but kills the SIMD benefit. → Option A
   sidesteps it entirely; Option B is profile-gated.
2. **Interactive GC can make BALLView *worse*** — GROMACS never mutates
   topology mid-run; BALL edits live. Compaction must be deferred +
   lease-protected or selection/undo/render break. → §1.6 policy; the
   GUI MUST hold stable-IDs, not indices.
3. **`Composite` decoupling isn't free** — `Atom : Composite`
   (atom.h:112, composite.h:1589); processors/RTTI traversal need a real
   handle adapter, not a typedef. → H2c handle iterators are the adapter;
   this is H4 work already.
4. **Contiguous-range vs scattered-edge** — strict ranges fight editing;
   scattered fights kernels. → ranges are *derived/cached*, edges are
   primary (§1.5).
5. **Do it after H4, not before** — mixing the storage migration with the
   facade flip fights two disruptive migrations at once. → §4 sequencing.

## 7. Net

The deep research **confirms the architecture is already pointed the
right way** and the biggest win is simply finishing H4. The GROMACS
study produces a counterintuitive, valuable result: *being faithful to
GROMACS argues for KEEPING AoS `Vector3` storage* (Option A) and thereby
**dodging the `Vector3&` retirement that every prior perf review flagged
as the blocker** — SoA is a kernel-local technique there, not a storage
layout. The aggressive primary-SoA split (Option B) is a real but
optional, profile-gated, post-H4, higher-risk lever — not a prerequisite
for GROMACS-class layout.

**Actionable now (all API-additive, no break, post-H4):** interaction-term
bonded arrays, derived `AtomRange`, span kernel APIs, molblock-style
instancing for solvent. **Deferred + profile-gated:** Option B + spatial
binning.

## 8. Option-A execution plan (LOCKED)

Concrete work items, all consistent with "keep AoS, no `Vector3&`
break." Ordered by dependency; none precede the H4 flip except the
additive prep in P0.

**P0 — pre-H4, additive prep (safe to start now, no break):**
- P0.1 Span/range kernel API surface on `MoleculeStore`:
  `ColumnSpan<Vector3> positions()/forces()` (whole-column + `AtomRange`
  overloads), `AtomRange` type, `ColumnSpan<T>` view. AoS-backed today.
  Purely additive, no call-site change. **Survival precision:** the
  `AtomRange` type + range-based *access pattern* survive an Option-B
  SoA swap unchanged; the `ColumnSpan<Vector3>` *signature* does not (no
  contiguous `Vector3*` under SoA) — it would become component spans.
  Under the LOCKED Option A (AoS stays) this is moot. **DONE** (commit
  pending; `MoleculeStore_test` "Option-A P0.1" CHECK).
- P0.2 Gate flagging *new* mutable-`Vector3&` position/velocity/force
  accessor declarations (mirror of HandleKeyLeakGate) so the Option-B
  retirement surface stops growing while H4 lands. `PositionRefGate`
  ctest, baseline 6 (3 durable `MoleculeStore` + 3 `Atom` facade, the
  latter dropping at H4 commit 8). **DONE.** Lower the baseline to 3
  when H4 commit 8 deletes the `Atom` accessors (the gate is a ceiling,
  not an auto-ratchet — Codex P0-review LOW).

**P0 STATUS: COMPLETE** (commit pending). Both items landed additive,
adversarially reviewed (Codex + Vibe), 291/291 ctest GREEN, benchmarks
flat (ABO within ±1% of baseline). Review fixes applied: empty-span
null-safety, mutable→const span widening, broadened gate regex
(`Vector3 &` + qualifiers, const-filtered), empty-store test coverage.
Honest-framing correction: the `ColumnSpan<Vector3>` *signature* does
not survive an Option-B SoA swap; the `AtomRange` + range *pattern* do.

**P1 — post-H4 (commit 8+), additive, measurable:**
- P1.1 GROMACS-style interaction-term arrays. Force fields precompute
  `BondTerm/AngleTerm/TorsionTerm` at setup; CHARMM/AMBER/MMFF bonded
  loops iterate term arrays + `for_each_bond_of`/CSR instead of
  `atom->getBond(i)`. Gate: `AmberFF_bench` / `CharmmFF_bench` (target
  1.2–2.5×, validate before commit).
- P1.2 Derived `AtomRange{begin,end,epoch}` cache for preorder-contiguous
  containers; kernels take the range fast-path, fall back to scattered
  `ChildRef`. Gate: `KernelIteration_bench`.
- P1.3 Transient SoA repack helper for the few vectorizable inner loops
  (the nbnxn pattern) — local to the kernel, AoS stays canonical.

**P2 — later, additive:**
- P2.1 molblock-style instancing for identical solvent/molecules (one
  moltype shared by N copies) — memory win for large solvated systems;
  needs a new instancing layer BALL lacks today.

**Deferred (NOT scheduled): Option B + spatial binning** — revisit only
with a post-H4 profile showing the energy/MD inner loops vectorization-
bound on position loads.

**Each P-item follows the standard gate:** build + 290/290 ctest +
HandleKeyLeakGate + `AssignBondOrder_bench` ≤5%, and must show its
claimed bench win *before* commit (the ABO precompile episode is the
cautionary precedent — an "obvious" win that crashed the real workload).

---
*Source docs: `H4-ATOM-AS-PURE-HANDLE-GROMACS-SOA-DESIGN.md` (Vibe full
draft), `/tmp/codex-atom-research.out` (Codex + live GROMACS source),
GROMACS manual nbnxm/topology. This reconcile doc is the decision of
record where they differ.*
