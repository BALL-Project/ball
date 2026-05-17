# Core-only MoleculeStore replacement — research & planning

**Branch:** `v2.0` (off `master` HEAD `70e23cdc7`).
**Scope contract (user-supplied, 2026-05-17):** v2.0 work happens on the
`v2.0` branch only. **First step is the redesign of the BALL kernel
data structures, restricted to the BALL core library.** "Core" = KERNEL
+ CONCEPT + their minimum support (CONFIG, COMMON, DATATYPE, MATHS,
SYSTEM). MOLMEC / STRUCTURE / FORMAT / VIEW / SCORING / DOCKING / QSAR /
NMR / SOLVATION / ENERGY / XRAY / PYTHON / PLUGIN are **out of scope**
for this first step; they are not built, not tested, and not migrated.
The replacement must be **minimally invasive** — i.e. expose a
**source-compatible facade** so downstream modules (when we later turn
them back on) recompile against the new core unchanged.

This document is a snapshot, not a commitment. It is the input to
`/gsd-plan-phase` for the K0 kernel-replacement phase.

---

## 1. How this differs from `KERNELV2.md`

`/Users/kohlbach/Claude/BALL2/.planning/KERNELV2.md` (pulled from
`origin/v2.x-planning-handover`, 560 lines) is the v2.0-LEAD plan
agreed after two Codex peer-review rounds. Its framing:

> "Parallel-model migration. Composite remains the source of truth for
> structure in v2.1. MoleculeStore is a snapshot." (§2 Goal 7)
> "Removing Composite as Atom's base class — v3.0 — requires file-format
> + ABI break." (§2 Non-Goal 1)
> "Phase K5 — Store-as-source-of-truth — DEFERRED to v3.0." (§4)

The user's directive on 2026-05-17 reframes the v2.0 first step:

> "Replace the core by the MoleculeStore … minimally invasive (i.e.
> with a compatible facade)."

These two framings are **not the same plan**:

| Dimension | KERNELV2.md (parallel-model) | User's directive (facade-over-store) |
|---|---|---|
| Source of truth for atom data | Composite (Atom owns position_, charge_, …) | MoleculeStore (Atom is a handle into store columns) |
| Composite tree | Unchanged, always live | Either (a) reconstructed lazily as a facade on top of the store, or (b) thin shell owning only structural pointers; column data lives in the store |
| `Atom*` validity | Stable pointers, owned by Composite tree | Stable pointers preserved by the facade; payload lookup goes through the store |
| ABI break | None (additive) | **Layout of `Atom`, `Bond`, `Composite` changes** — ABI break is intrinsic |
| File formats | Unchanged | Unchanged (FORMAT writes via the same `persistentWrite` virtuals; representation under the hood is store columns) |
| Memory budget vs v1.x | +150 B/atom (additional, on top of ~800 B/atom Composite cost) | Goal: net **reduction** (atoms shrink to handle-sized; columns are dense and shared) |
| Risk profile | Low — Composite is unchanged | High — every getter/setter in `atom.iC` / `bond.iC` is rewritten |
| Downstream module impact | Zero recompile cost (Atom layout unchanged) | Source-compat preserved (recompile needed; **link-compat broken**) |

Both framings target the same long-term endpoint (SoA columns as the
real data); they choose different points on the source-compat / ABI /
risk curve to land in v2.0.

**Resolution this document adopts:** treat the user's directive as the
**v2.0 first-step goal** and `KERNELV2.md`'s K0 as the **fallback plan
if the facade approach fails the source-compat regression** on
KERNEL/CONCEPT unit tests. The reframed plan stays inside the same
KERNELV2.md compatibility-promise envelope on the dimensions that
matter for downstream code (source compat, file-format stability,
behavior parity), and intentionally relaxes the dimension that matters
only for downstream binaries (ABI), since v2.0 is the agreed major-
version break already.

If the facade approach lands cleanly on KERNEL+CONCEPT tests, K0 of
KERNELV2.md is **superseded** by this plan; K1–K4 (hot-path SoA, compiled
selection, FORMAT migration, Python bindings) then become straight-line
work because the store is already the source of truth.

If the facade approach proves too invasive to land in a v2.0 envelope
(e.g. Composite's `friend class` semantics block clean separation, or
the inline `.iC` getter contract leaks through too many subsystems),
we fall back to KERNELV2.md's parallel-model K0 with no replanning
cost — that document remains the contingency.

---

## 2. What we inherited from the handover package

Five docs landed under `.planning/v2.x/` (from `origin/v2.x-planning-handover`):

- `HANDOVER.md` — entry point. Confirms v2.0 = substrate rewrite, KERNEL
  is LEAD.
- `ROADMAP-v2.md` — v2.0/v2.1/v2.2/v2.3 phase inventory. v2.0 phases:
  999.24 KERNEL (LEAD) + 999.6 PIPE-01 renderer + 999.9 INIFile→YAML +
  999.11 mmCIF→gemmi + 999.12 deprecated-code removal.
- `OPEN-QUESTIONS.md` — five pre-staged maintainer decisions. None gate
  the KERNEL phase itself. Q2 (PIPE-01 sequencing) confirms KERNEL K0
  runs **before** PIPE-01 (renderer reads from MoleculeStore positions).
- `SEEDS-FORWARD.md` — one active dormant seed (SEED-005-4, folds into
  PIPE-01); SEED-001 is the BALLView Refresh, scheduled to close in v1.8.
  No active seed blocks core kernel work.
- `GAPS-FROM-V1.7.md` — nine doc-tree gaps. None block core kernel work;
  Gap 5 (author `MILESTONE-CONTEXT-v2.0.md` at cycle open) is the only
  one we should action before this plan promotes to execution.
- `V1.8-WAVE.md` — five UI phases; not relevant to core kernel scope.

Also pulled to this branch: `.planning/KERNELV2.md` (560 lines,
post-Codex-Round-3 baseline) — the parallel-model fallback plan
described above.

`.planning/ROADMAP.md`, `PYBALLV2.md`, and the per-phase planning dirs
under `.planning/phases/` from the handover branch were **not pulled**
— they belong to v1.6/v1.7 phases or to v2.1+ work. This first step
needs only the KERNEL-relevant subset.

---

## 3. Existing core library — measured ground truth

All counts taken on `v2.0` HEAD (`70e23cdc7`) on 2026-05-17.

### 3.1 Core surface (KERNEL + CONCEPT)

| Area | Count |
|---|---|
| `include/BALL/KERNEL/` headers | 38 (incl. 3 `.iC` inline files) |
| `source/KERNEL/` impl | 24 `.C` files |
| `include/BALL/CONCEPT/` headers | 30 (incl. 5 `.iC` inline files) |
| `source/CONCEPT/` impl | 20 `.C` files |
| Combined LOC (headers + impl + inline) | ~34,800 |
| `friend class` declarations in KERNEL + CONCEPT | 9 |

Key files in the rewrite scope:

| File | LOC | Role |
|---|---|---|
| `include/BALL/KERNEL/atom.h` | 1,023 | Atom interface — 13 typed payload members (name_, type_name_, element_, radius_, type_, position_, velocity_, force_, charge_, formal_charge_, number_of_bonds_, bond_[12], interactions) |
| `include/BALL/KERNEL/atom.iC` | 208 | Inline getters/setters — hot-path accessors |
| `source/KERNEL/atom.C` | 679 | Non-inline impl: ctors, dtor, persistence, bond create/destroy, getFullName |
| `include/BALL/KERNEL/bond.h` | 473 | Bond interface — 5 typed payload members (first_, second_, name_, bond_order_, bond_type_) + `friend class Atom` |
| `include/BALL/KERNEL/bond.iC` | 260 | Inline getters/setters |
| `include/BALL/CONCEPT/composite.h` | 2,198 | Composite tree node — children/parent/sibling pointers, modification + selection stamp, traversal, `applyChild*` / `applyDescendantPreorder*` templates |
| `include/BALL/CONCEPT/composite.iC` | 234 | Inline iteration / traversal helpers |
| `source/CONCEPT/composite.C` | 1,762 | Composite impl — splice/insert/remove/replace/clone/persistence |
| `include/BALL/KERNEL/atomContainer.h` | 411 | Layered on Composite + PropertyManager — name_, insert/append/remove for Atom & AtomContainer, intra/inter bond counters |
| `include/BALL/KERNEL/system.h` | 279 | Top-level AtomContainer specialisation — knows Molecule/Protein/NucleicAcid children |

### 3.2 Composite hierarchy classes (Composite-derived in KERNEL)

`Atom`, `Bond`, `AtomContainer`, `Molecule`, `Fragment`, `Residue`,
`Chain`, `SecondaryStructure`, `Protein`, `NucleicAcid`, `Nucleotide`,
`PDBAtom`, `System`. Plus `Selector`, `Expression`, `ExpressionTree`,
`ExpressionParser`, `Element`, and ~22 predicate classes.

### 3.3 Atom binary layout (today)

From [atom.h:966–993](include/BALL/KERNEL/atom.h):

```
static AtomIndexList free_list_;   // class-static, not per-atom
String          name_;             // ~32 B (std::string SSO + ptr)
String          type_name_;        // ~32 B
const Element*  element_;          //  8 B
float           radius_;           //  4 B
Type            type_;             //  2 B (typedef short)
unsigned char   number_of_bonds_;  //  1 B
Bond*           bond_[12];         // 96 B  (MAX_NUMBER_OF_BONDS = 12)
Index           formal_charge_;    //  4 B (typedef int)
Vector3         position_;         // 12 B (3× float)
float           charge_;           //  4 B
Vector3         velocity_;         // 12 B
Vector3         force_;            // 12 B
MolecularInteractions* interactions;//  8 B (public!)
```

Plus the **Composite** base subobject: number_of_children_ + 5 ptr (parent_,
previous_, next_, first_child_, last_child_) + properties_ +
contains_selection_ + 2× Size counters + 2× TimeStamp = ~70 B.

Plus the **PropertyManager** base subobject: property bitset + named-properties
map (typically empty per atom).

Estimated total per `Atom`: ~280–320 B without bonds populated, plus 96 B
of fixed bond pointer array regardless of actual bond count. The
KERNELV2.md "~800 B/atom" figure includes the PropertyManager named map
worst case, plus alignment + per-allocation heap overhead.

### 3.4 Bond binary layout

From [bond.h:439–459](include/BALL/KERNEL/bond.h):

```
Atom*  first_;        //  8 B
Atom*  second_;       //  8 B
String name_;         // ~32 B (usually empty!)
Order  bond_order_;   //  2 B (typedef short)
Type   bond_type_;    //  2 B (typedef short)
```

Plus Composite (~70 B) + PropertyManager (~base).
`KERNELV2.md` `BondRecord` candidate is 12 B
(uint32_t a, uint32_t b, uint8_t order, uint8_t type, uint16_t flags).
Compression ratio is ~10× on the bond population alone.

### 3.5 Cross-module dependence on `Atom*` / `Atom&`

Raw `grep -E "\bAtom\s*[*&]"` count per module (file lines):

| Module | Lines mentioning `Atom*`/`Atom&` |
|---|---:|
| KERNEL | 297 |
| MOLMEC | 400 |
| STRUCTURE | 586 |
| VIEW | 167 |
| SCORING | 290 |
| QSAR | 118 |
| NMR | 114 |
| FORMAT | 80 |
| DOCKING | 51 |
| SOLVATION | 18 |
| CONCEPT | 8 |
| DATATYPE | 7 |
| ENERGY | 1 |
| XRAY | 0 |

**Implication:** the facade must keep `Atom*` and `Atom&` (and the
inline getter/setter contract from `atom.iC`) source-stable. Even one
breaking semantic change in `Atom::getPosition()` ripples to 2,000+
call sites across MOLMEC/STRUCTURE/SCORING.

KERNELV2.md's number was 2,463 `Atom*`/`Atom&` reference sites — the
grep counts above are line-counts (one line can contain multiple
references) so the two figures are consistent.

Other relevant counts: 111 `BALL_FOREACH_*` macro uses, 96
`dynamic_cast<…>` on KERNEL types (some include `Composite` casts
that aren't in KERNELV2.md's 53 — KERNELV2 grep was narrower).

### 3.6 Composite's stamp infrastructure — already in place

`Composite::stamp(StampType)` ([composite.h:445](include/BALL/CONCEPT/composite.h#L445))
propagates modification_stamp_ + selection_stamp_ to the root on every
structural mutation. KERNELV2.md §5.1 builds its Mutation Contract
Table around this hook. **For the facade approach the stamp matters
less** — if the store is the source of truth, mutations to columns
update store generation directly, and the stamp becomes a derived value
the Composite facade reads from the store on demand.

### 3.7 Test corpus

246 `_test.C` files under `test/`. Direct KERNEL/CONCEPT regression
targets (not exhaustive):

- `Atom_test1.C` (328 LOC), `Atom_test2.C`
- `AtomContainer_test1.C` (415 LOC), `AtomContainer_test2.C`
- `AtomIterator_test.C`, `AtomBondIteratorTraits_test.C`,
  `AtomVector_test.C`, `AtomBijection_test.C`
- `Bond_test.C`
- `Composite_test1.C` … `Composite_test6.C` (6 files)
- `CompositeIteratorTraits_test.C`,
  `CompositeAncestorIteratorTraits_test.C`,
  `CompositeChildcompositeIteratorTraits_test.C`,
  `CompositeCompositeIteratorTraits_test.C`
- `Molecule_test.C`, `Fragment_test.C`, `Chain_test.C`, `Protein_test.C`,
  `Residue_test.C`, `NucleicAcid_test.C`, `PDBAtom_test.C`, `System_test.C`
- `Expression_test.C`, `ExpressionTree_test.C`, `ExpressionParser_test.C`,
  `ExpressionPredicate_test.C`, `KernelPredicate_test.C`,
  `GlobalKernel_test.C`
- `Selector*_test.C`

These are the **green-bar regression target** for the facade — KERNEL +
CONCEPT must pass all of them with no source changes to the test
files themselves (test files use the public KERNEL API; they are part
of the facade-stability contract).

Test data fixtures (`test/data/`): PDB, HIN, mol2 files including
1BNA / 2ptc / 6_12-test. Useful for round-trip sync sanity.

### 3.8 Build wiring

`cmake/BALLIncludes.cmake` is the master include — `sources.cmake`
per module is added in a fixed order. Core-only build = enable
CONFIG + CONCEPT + COMMON + DATATYPE + KERNEL + MATHS + SYSTEM;
disable the other 13 modules. This is mechanical CMake surgery; the
inter-module dependency graph already runs core-first.

---

## 4. The replacement design

### 4.1 Target architecture

```
   ┌──────────────────────────────────────────────────────────────┐
   │  MoleculeStore  (SoA columns + CSR bond adjacency)           │
   │  Owns: positions, velocities, forces, charges, element,      │
   │        formal_charge, name_offset, residue_idx,              │
   │        bonds[a,b,order,type,flags], bond CSR, string_pool,   │
   │        selection_bitmap, generation, stamps                  │
   │  Identity: stable per-atom uint64 (assigned at insert)       │
   └──────────────────────┬─────────────────────────┬─────────────┘
                          │                         │
              ┌───────────▼──────────┐  ┌───────────▼─────────┐
              │ Atom  (handle facade)│  │ Bond (handle facade)│
              │  store_*  + idx_     │  │  store_* + bond_idx │
              │  generation_ stamp   │  │  generation_ stamp  │
              │  inherits Composite  │  │  inherits Composite │
              │  size: ≤32 B/atom    │  │  size: ≤32 B/bond   │
              └───────────┬──────────┘  └─────────────────────┘
                          │
                          │ as a Composite node:
                          ▼
            Composite tree nodes (AtomContainer, Molecule,
            Fragment, Residue, Chain, ..., System):
            unchanged hierarchy semantics; tree pointers stay
            in Composite base; payload reads/writes route to
            MoleculeStore via the Atom/Bond handle.
```

### 4.2 The facade contract — six invariants

The Atom / Bond / Composite / AtomContainer / System public API
(member signatures, return types, exception specs, observable side
effects) **does not change**. Specifically:

1. **Identity invariant.** `Atom* a; … *a …` continues to work for
   the lifetime of the atom. `a` is a pointer to the handle object;
   the handle holds a stable index into the store, not a column
   pointer. Pointer comparison (`a == b`, `std::less<Atom*>`),
   pointer hashing, and pointer arithmetic over `std::vector<Atom*>`
   continue to behave identically.
2. **Getter / setter return-type invariant.** `Atom::getPosition()`,
   `getCharge()`, `getName()`, etc. keep their exact return types
   (including reference return for the mutable overloads). The
   reference path is implemented as a reference into the store column;
   safe because the handle holds an index, not a column pointer, and
   the store guarantees the column reference is valid until the next
   mutation that resizes that column.
3. **Iteration invariant.** All KERNEL iterators (`AtomIterator`,
   `BondIterator`, `BALL_FOREACH_ATOM`, …) continue to yield `Atom&` /
   `Bond&` / `AtomContainer&` etc. Internally they may iterate
   over store indices instead of Composite-tree links — the yielded
   reference is to the facade handle, not to a column row.
4. **Persistence invariant.** `persistentWrite` / `persistentRead`
   continue to emit the same byte stream as v1.x. File formats are
   unchanged in v2.0 (per KERNELV2.md §2 Goal 5).
5. **Stamp + selection invariant.** `Composite::getModificationTime()`
   / `getSelectionTime()` continue to advance monotonically across
   mutations and select calls. Implementation reads from the store's
   generation counter; selection is a store-owned bitmap that the
   Composite tree projects.
6. **Property invariant.** `PropertyManager` API (named & indexed
   properties on Atom / Bond / AtomContainer) preserved. Properties
   with >5% atom coverage are promoted to typed store columns as an
   optimisation; below that threshold they live in the store's
   `custom_props` map keyed by stable id.

### 4.3 What changes vs v1.x at the source level

For client code restricted to the public KERNEL/CONCEPT API:
**nothing**. That is the facade contract.

For client code that reaches into Atom/Bond internals (via `friend
class` declarations, or via direct member access in cases where
KERNEL grants it): **breaks**. The 9 friend declarations in
KERNEL/CONCEPT are auditable; each either (a) becomes a store-mediated
accessor, or (b) is preserved as a friend of the new Atom facade
with the new field set.

### 4.4 Memory budget

Target: per-atom + per-bond storage **decreases** net vs v1.x on a
100k-atom benchmark, after accounting for the new SoA columns.

Rough per-atom estimate:
- Old: ~280 B Atom + ~70 B Composite + ~50 B PropertyManager amortised
  + 96 B bond_[12] fixed array (mostly empty) ≈ **~496 B/atom typical**.
- New: ~32 B handle (store* + idx + generation + Composite base
  pointers) + ~104 B/atom in store columns + ~16 B/atom in bond CSR
  ≈ **~152 B/atom typical**.

Net reduction: ~3× per atom on populated systems. Bond memory
compresses ~10× (12 B vs ~120 B today).

These are estimates. K0 of this plan includes a memory-budget benchmark
on the existing test corpus (1BNA, 2ptc, 6_12-test) — the budget is a
gate, not a hope.

### 4.5 ABI break

Intrinsic and accepted. v2.0 is the agreed major-version break;
KERNELV2.md §3 already calls out v1.x → v2.0 ABI as the natural
release-boundary slot for layout changes. The facade approach makes
the **layout** change unavoidable (Atom shrinks to handle size), but
keeps the **source API** stable.

CI gate: the multi-platform ABI-diff jobs from KERNELV2.md §6 still
fire, but instead of "block on incompatible change" they "report
incompatible changes for release-notes inclusion." Symbol allowlist
governs what's expected.

---

## 5. Phase shape — what K0 (core-only) looks like

This is a **research/planning sketch**, not a committed plan. The
committed plan comes from `/gsd-plan-phase` once this document is
reviewed and the open questions in §7 are resolved.

### K0.1 — Build matrix scaffolding (1–2 weeks)

- Add a `BALL_CORE_ONLY=ON` CMake option that disables MOLMEC, STRUCTURE,
  FORMAT, VIEW, SCORING, DOCKING, QSAR, NMR, SOLVATION, ENERGY, XRAY,
  PYTHON, PLUGIN, EXTENSIONS, BENCHMARKS, APPLICATIONS at configure time.
- Verify the v2.0 branch builds core-only on macOS-arm64 / Linux-x86_64
  + GCC / Linux-x86_64 + Clang.
- Verify the KERNEL+CONCEPT test subset (246 → ~40 core tests) runs
  green on the unmodified codebase. That's the **regression baseline**
  for the facade work.
- Add `BALL_CORE_ONLY=ON` CI job to `.github/workflows/ci.yml` (mirrors
  v1.7 path-aware-triggers pattern).

### K0.2 — MoleculeStore type + storage (3–5 weeks)

- New file pair: `include/BALL/KERNEL/moleculeStore.h` +
  `source/KERNEL/moleculeStore.C`.
- SoA columns per §4.1 of `KERNELV2.md` (positions, velocities, forces,
  charges, element, formal_charge, name_offset into string_pool,
  residue_idx, stable_id).
- Bond table + CSR adjacency.
- Generation counter; `markDirty(Channel)`.
- Insert / remove operations with stable-id assignment.
- No facade yet — store is exercised by direct API tests:
  `MoleculeStore_test.C`, `MoleculeStoreBond_test.C`.
- Memory benchmark on a fabricated 100k-atom population.

### K0.3 — Atom + Bond facade (4–6 weeks)

- Rewrite `include/BALL/KERNEL/atom.h` to declare Atom with payload =
  `{ MoleculeStore* store_; uint32_t idx_; uint64_t generation_; }` +
  inherited Composite + PropertyManager base subobjects.
- Rewrite `atom.iC` getters/setters to dispatch through the store
  using `idx_`.
- Rewrite `source/KERNEL/atom.C` ctors to acquire a store slot;
  dtor to release.
- Same for `bond.h` / `bond.iC` / `bond.C`.
- Run `Atom_test1.C`, `Atom_test2.C`, `Bond_test.C`,
  `AtomIterator_test.C`, `AtomBondIteratorTraits_test.C`,
  `AtomBijection_test.C`, `AtomVector_test.C`. **Green** is the gate.
- Persistence round-trip (`Atom::persistentWrite/Read`) byte-equal
  on the test fixtures.

### K0.4 — Composite + AtomContainer + System integration (3–5 weeks)

- Confirm Composite's `friend class Atom` semantics work with the
  reshaped Atom layout.
- Audit the 9 friend declarations in KERNEL+CONCEPT — for each,
  decide: keep as friend (preserves behaviour), promote to store
  accessor (cleaner), or refactor.
- Rewrite the inline iteration helpers in `composite.iC` only where
  the Atom/Bond payload move requires it; tree traversal stays
  pointer-walked.
- Run `Composite_test{1..6}.C`, `CompositeIteratorTraits_test.C`,
  `CompositeAncestorIteratorTraits_test.C`,
  `CompositeChildcompositeIteratorTraits_test.C`,
  `CompositeCompositeIteratorTraits_test.C`,
  `AtomContainer_test{1,2}.C`, `Fragment_test.C`, `Chain_test.C`,
  `Molecule_test.C`, `Protein_test.C`, `Residue_test.C`,
  `NucleicAcid_test.C`, `PDBAtom_test.C`, `System_test.C`. Green.

### K0.5 — Selection + Expression (2–3 weeks)

- `Selectable` / selection bitmap routes through the store (one source
  of truth for selection state).
- Expression / ExpressionTree / ExpressionParser unchanged at the
  source level; the leaf predicate that touches `Atom::getCharge()`
  etc. inherits store-routing through the facade.
- Run `Expression_test.C`, `ExpressionTree_test.C`,
  `ExpressionParser_test.C`, `ExpressionPredicate_test.C`,
  `KernelPredicate_test.C`, `Selector*_test.C`. Green.

### K0.6 — PropertyManager flattening (1–2 weeks)

- High-coverage properties (>5% of atoms) promoted to typed store
  columns; sub-5% live in `custom_props`. Existing `setProperty(name,
  value)` API delegates to the right backend per name.
- Property-tagged test corpus (whichever tests use `setProperty` on
  Atom/Bond/AtomContainer) — green.

### K0.7 — Memory + perf benchmark (1 week)

- Build a deterministic core-only benchmark: spin up 100k atoms,
  10× bond population per atom, run selection queries +
  iteration sweeps + getter sweeps.
- Compare memory (RSS + heap profile) and ops/sec to a `master` baseline
  measured in K0.1.
- Gate: memory ≥2× reduction on populated systems; iteration cost
  ≤1.5× legacy (handle indirection vs direct-member access — accepted
  on the understanding that hot paths will SoA-loop directly later).

### K0.8 — Compatibility audit + release-notes draft (1 week)

- Sweep the public KERNEL/CONCEPT API for any signature drift.
- Confirm `Atom*` pointer semantics are preserved (lifetime,
  comparability, hashability).
- Draft `RELEASE-NOTES-v2.0-KERNEL.md` documenting the ABI break
  and the source-compat contract.
- Author `MILESTONE-CONTEXT-v2.0.md` per Gap 5 in `GAPS-FROM-V1.7.md`.

**K0 total: ~15–25 weeks at 1.0 FTE.** That's shorter than
KERNELV2.md's K0 (10–14 weeks) + K1 (12–20 weeks) + K2 (4–6 weeks) =
26–40 weeks for the parallel-model preview, **because we are not
building two coexisting representations**. The store-as-truth approach
trades parallel-model coexistence cost for ABI-break cost; downstream
modules pay the recompile cost once at v2.0 instead of paying the
sync overhead forever.

---

## 6. Risks specific to the facade approach

| ID | Risk | Likelihood | Impact | Mitigation |
|---|---|---|---|---|
| F1 | Reference-return getters (e.g. `Vector3& Atom::getPosition()`) bind to store-column rows that move when the column resizes | High | Critical | Insert-with-reserve discipline in the store; reference is invalidated only at explicit `compact()`; document the contract; assert in debug; covered by the facade-contract invariant test |
| F2 | `Atom*` pointer-arithmetic patterns over `std::vector<Atom*>` break if handle objects move | Med | High | Handle objects are heap-allocated through the same `new Atom()` path; pointer stability preserved; this is the **whole point** of the handle indirection |
| F3 | Composite tree node identity for non-atom containers (Molecule, Residue, …) — these don't have a "store row" because they have no per-atom payload. Where does their position-of-children data live? | Med | Med | Containers stay structurally as Composite nodes; they own structural pointers but not atom-payload columns; the store knows which atoms belong to which container via `residue_csr_*` plus a separate `container_idx` lookup |
| F4 | `friend class` declarations in KERNEL+CONCEPT (9 sites) need rework because Atom's private members no longer exist (they live in the store) | Med | Med | K0.4 audit; each friend either re-friended on the new Atom facade with payload accessors, or promoted to a store-mediated public accessor |
| F5 | `AtomIndexList free_list_` (the static atom-id pool in atom.h:967) interacts with the store's stable-id allocator | Low | Low | Remove the free_list; store assigns stable ids; documented in release notes |
| F6 | `Atom::interactions` is a **public pointer member** (atom.h:623). Removing it from the Atom layout could break any client that reads/writes `a->interactions` directly | Med | Med | Audit callers (grep); preserve as a handle accessor `Atom::interactions()` returning the store-owned pointer, with a backwards-compat `public: MolecularInteractions* interactions;` shim that's a thin wrapper if needed |
| F7 | `MAX_NUMBER_OF_BONDS = 12` (atom.h:124) is part of the public API; some clients may iterate `for (int i = 0; i < 12; ++i) atom.getBond(i)`. The store has no fixed cap | Low | Low | Preserve the constant; the facade's `getBond(i)` returns nullptr for i ≥ countBonds(), same as v1.x behavior |
| F8 | Persistence: `Atom::persistentWrite` writes raw pointer-graph links per KERNELV2.md §1. Replacing the storage layout may change the persistence byte layout | High | High | Persistence virtuals stay; they read/write through the same Atom getters that route to the store; on read, the store is populated synchronously; byte format unchanged |
| F9 | The facade landing in core-only means the rest of BALL doesn't compile in this branch. Downstream modules carry latent breakage we won't see until we re-enable them | Med | High | Compile-fence the downstream modules off in K0.1; in K1 (out of scope of this first step), turn modules back on one at a time with a per-module compatibility audit |
| F10 | Performance regression in pointer-chasing-heavy code paths because of the handle indirection (every getter is now load-store-via-pointer + index lookup) | Med | Med | Inline the dispatch in `atom.iC`; for hot paths, the v2.1 K1-equivalent work exposes the SoA columns directly so the indirection disappears in the loop body |
| F11 | The `Composite::stamp(BOTH)` invariant that downstream tests rely on (e.g. "after I appendChild, getModificationTime() advances") needs the store to advance its generation in lockstep | Low | Med | `Composite::stamp` calls store->`markDirty(STRUCTURE)`; trivial wiring |

R1–R16 from KERNELV2.md still apply where the risk is independent of
the parallel-vs-replacement choice (e.g. R3 STRUCTURE bond perception
edge cases, R13 user-registered predicate semantics).

---

## 7. Open questions for maintainer (must resolve before promotion)

1. **Confirm the framing.** Does the user's directive "replace the core
   by the MoleculeStore … with a compatible facade" definitely mean
   store-as-source-of-truth (what this doc plans), or does it leave
   room for KERNELV2.md's parallel-model interpretation? The two are
   substantively different; pick one and commit before K0 plans land.

2. **ABI policy.** Accept the layout-level ABI break at v2.0 (this
   doc's assumption), OR insist on layout stability and revert to
   KERNELV2.md's parallel-model? The handover docs treat v2.0 as the
   major-version break window; this question is "are we using that
   window for the kernel break or saving it for something else?"

3. **Memory budget target.** This doc proposes ~152 B/atom (net
   reduction vs ~500 B/atom v1.x). KERNELV2.md sets ~150 B/atom as
   **additional** overhead on top of unchanged Composite, which implies
   no reduction. Which target is the gate?

4. **Reference-return getter contract.** `Atom::getPosition()` returns
   `Vector3&`. Three options:
   - (a) Preserve the reference return; reference is invalidated on
     column resize; document the contract.
   - (b) Change to value return; source-break the ~hundreds of call
     sites that hold the reference across a mutation.
   - (c) Add a separate `Atom::getPositionRef()` that's stable, mark
     `Atom::getPosition()` returning by-value and deprecate the old
     signature.
   This doc assumes (a). If maintainer wants (b) or (c) the K0.3
   work shape changes substantially.

5. **`Atom::interactions` public member.** Currently
   `public: MolecularInteractions* interactions;` on atom.h:623. Three
   options: preserve as public member shim, deprecate to accessor only,
   or remove cleanly at v2.0. This doc assumes "preserve as shim."

6. **K0 promotion criterion.** Is "all 40 core KERNEL+CONCEPT tests pass
   green and memory benchmark ≥2× reduction" enough to declare K0 done?
   OR do we additionally require a downstream-module smoke-build (e.g.
   re-enable MOLMEC just enough to compile a single force-field test)?
   This doc assumes core-only is the K0 exit. The user's directive
   ("focus on the BALL core library only") supports that reading;
   confirming closes the question.

7. **Scope of selection / Expression rework in K0.** This doc keeps
   selection routing through the store but defers KERNELV2.md's K2
   "compiled expression" optimization to later work. Is that the
   right cut, or should compiled selection land inside K0 so the
   facade exposes its perf benefit immediately?

---

## 8. What is explicitly NOT in this first step

- Any work on MOLMEC, STRUCTURE, FORMAT, VIEW, SCORING, DOCKING, QSAR,
  NMR, SOLVATION, ENERGY, XRAY, PYTHON, PLUGIN. These are disabled in
  the K0.1 CMake fence and re-enabled module-by-module in a later step
  (which is a separate plan).
- PIPE-01 renderer rewrite (Phase 999.6) — sequenced after KERNEL per
  Q2 in OPEN-QUESTIONS.md.
- INIFile → YAML (Phase 999.9), mmCIF → gemmi (Phase 999.11),
  deprecated-code removal (Phase 999.12) — independent v2.0 substrate
  phases, planned separately.
- PyBALL bindings (v2.1).
- REST API (v2.2).
- Read the Docs portal (v2.3).
- File-format v2 (KERNELV2.md §10).
- GPU-resident molecular data (KERNELV2.md §2 Non-Goal 5).
- BALLView selection bidirectional sync (KERNELV2.md K4 — needs VIEW,
  out of scope).

---

## 9. Next actions (recommended)

1. **Maintainer review of this document.** Answer the seven questions in
   §7. Confirm or reject the "store-as-source-of-truth" framing vs the
   `KERNELV2.md` "parallel-model" framing. Once §7-Q1 resolves, the
   rest of the plan collapses to either this document's K0.1–K0.8
   shape (replacement) or KERNELV2.md's K0–K2 shape (parallel).
2. **Author `MILESTONE-CONTEXT-v2.0.md`** (per Gap 5). One-time setup.
   ~1 day.
3. **Promote core-only KERNEL replacement to a phase** via
   `/gsd-plan-phase`. Phase number per ROADMAP convention: probably
   `999.24` (the KERNEL phase already reserved), with K0.1–K0.8 as
   per-plan files inside.
4. **K0.1 build-fence work** can start in parallel with maintainer
   review — it's purely additive (a new CMake option) and doesn't
   commit to either framing.

---

## 10. Provenance

- KERNELV2.md plan pulled from `origin/v2.x-planning-handover` commit
  `499c5f8714` ("docs(v2.x): create handover package for BALL 2.x
  planning"). 560 lines, post-Codex-Round-3.
- Handover docs (HANDOVER, ROADMAP-v2, OPEN-QUESTIONS, SEEDS-FORWARD,
  GAPS-FROM-V1.7, V1.8-WAVE) from same commit.
- Codebase measurements taken on `v2.0` branch HEAD `70e23cdc7`
  (master, "ci(release): sync release.yml to v1.6-modernization HEAD").
- Tools used: `git`, `grep -E`, `wc -l`, `find`. All measurements
  reproducible from those.

*End of CORE-MOLECULESTORE-RESEARCH.md.*
