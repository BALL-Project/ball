# K0 Adversarial Review — Codex CLI (2026-05-17)

**Status:** Complete. **Action:** All findings cross-verified by Claude
against the live tree; verified findings drove amendments to
[`KERNEL-V2-DECISIONS.md`](KERNEL-V2-DECISIONS.md) (see "amended"
markers on D2/D3/D4/D6/D7/D10/D12/D13 plus new D15/D16).

**Reviewer:** Codex CLI 0.128.0 (`codex exec`).
**Subject:** Locked K0 design — `KERNEL-V2-DECISIONS.md` + companion
research + `K0.3-FINDINGS.md` + just-landed K0.2 code in
`include/BALL/KERNEL/moleculeStore.h` + `source/KERNEL/moleculeStore.C`
+ the `Atom2` prototype in `experiments/moleculeStore/`.

**Prompt attack vectors (8):** D6 identity cost; D7 reference contract;
D10 test-compat surface; Path-2 drift; lazy per-Atom store; bond CSR
absence; persistence break; downstream PropertyManager users.

---

## Findings (as delivered by Codex)

| # | Severity | Area | Finding (verbatim summary) |
|---|---|---|---|
| 1 | BLOCKER | D10 green bar | `AtomContainer_test1.C` casts `AtomContainer*` to `Composite*` and uses `composite->create(true)`; also exercises `TextPersistenceManager` / `PersistentObject`. With full Composite + PersistentObject deletion the test can't compile. |
| 2 | BLOCKER | D13 / D6 | Memory budget omits `Atom* back_ptr_` + `Bond* bond_back_ptr_` + per-allocation heap overhead. For 100k atoms / 1M bonds (10 bonds/atom), bond_back_ptr_ alone = 80 B/atom; BondRecord adds another 120 B/atom — blows the ≤160 B/atom cap. |
| 3 | HIGH | D7 reference getters | Real BALL code stores `Vector3&` from `getPosition()` and mutates through it. Generation-check on the handle cannot detect an escaped `Vector3&` after a store resize. Cites `sdGenerator.C:347,563`, `atomBijection.C:33`, `MMFF94OutOfPlaneBend.C:219`. |
| 4 | HIGH | Path-2 drift | `Atom::operator=`, `Atom::swap`, `Atom::clear_`, `Bond::createBond` (via friendship) all write Atom fields directly. Phased getter migration that flips one getter at a time silently produces drift unless every direct writer for that field is migrated first. |
| 5 | HIGH | Lazy per-Atom store | Prototype's `Atom()` ctor allocates a full MoleculeStore (15 vectors + string pool + counters) per orphan atom. For 1M default-constructed atoms before adoption = 1M heap stores + allocator storm + release storm on adoption. |
| 6 | HIGH | Bond queries | K0.2 `bonds_of()` is linear scan. Force-field nonbonded loops and `BuildBondsProcessor` etc. depend on fast bond queries. Without CSR, whole-system queries become O(N²). |
| 7 | HIGH | Persistence | D12 promises new format + converter but no design subphase. Existing persistence has ordering invariants, pointer fixup, bond identity, hierarchy reproduction — none addressed in K0.6 estimate. |
| 8 | MEDIUM | Deleting bases | 327 files mention Composite/PropertyManager/Selectable. Non-KERNEL classes (`Ring`, `PDBFile`, `Peak`, `Representation`) inherit `PropertyManager`. Deletion fallout undercounted. |

---

## Claude's cross-verification

**Finding 1 (BLOCKER): CONFIRMED.** `test/AtomContainer_test1.C:210`
literally contains `Composite* composite = (Composite*)ac1;` followed
by `composite->create(true)`. Audit of all 23 K0 kernel hierarchy tests
shows EVERY one references Composite/PersistentObject/PropertyManager/
Selectable/TextPersistenceManager. The most damning case is
`AtomContainer::insertBefore(Atom&, Composite&)` —
`Composite&` is the *parameter type* in the AtomContainer public API
itself, and `AtomContainer_test2.C:177,190,223,236` calls it. There's
no way to delete `Composite` without either editing these tests
(violates D10) or breaking the API signature.
**Action:** D2/D3/D4 amended from "Full removal" to "Reduced to thin
stub" + §6.5 added to explain the stub surface.

**Finding 2 (BLOCKER): PARTIALLY CONFIRMED.** Math:
- Typical case (proteins, 3 bonds/atom, 100k atoms → 300k bonds):
  bond_back_ptr = 24 B/atom, BondRecord = 36 B/atom, total bond
  storage = 60 B/atom. Plus columns (74 B) + handle (24 B) ≈ **158
  B/atom — just inside the cap**.
- Codex's worst-case (10 bonds/atom, 1M bonds for 100k atoms):
  bond_back_ptr = 80 B/atom + BondRecord = 120 B/atom = 200 B/atom of
  bond storage alone. **Blows the cap**.

  Codex is right that the typical case is borderline (158 B/atom) and
  that worst case is over. D13 amended: typical case (3 bonds/atom)
  stays at ≤160 B/atom HARD; worst case documented as out-of-budget
  with rationale (compact-bond representation planned for K0.6+).

**Finding 3 (HIGH): CONFIRMED.** All four citation sites
(`sdGenerator.C:347,563`, `atomBijection.C:33`,
`MMFF94OutOfPlaneBend.C:219`) verified — they all hold `Vector3&` /
`const Vector3&` from `getPosition()`. Codex's "reference lease"
suggestion is the right shape. D7 amended: per-store
`borrowed_ref_count_` counter, RAII helper on getters, mutators
assert-zero in debug builds.

**Finding 4 (HIGH): CONFIRMED.** Cite-by-cite verification:
- `atom.C:166-184`: `Atom::set` writes Composite/PropertyManager bases
  and then direct fields.
- `atom.C:186-204`: `Atom::operator=` writes name_, type_name_,
  position_, charge_, velocity_, force_, etc. directly.
- `atom.C:204+`: `Atom::swap` swaps `bond_[i]` array directly.
- `bond.C:94-100`: `Bond::createBond` mutates `first.number_of_bonds_`,
  `first.bond_[...]`, `second.number_of_bonds_`, `second.bond_[...]`
  via friendship — bypasses any setter.

  Path-2 amendment in K0.3b: **per-field migration checklist** —
  no getter flip until every direct writer for that field is migrated.
  Specifically: before `getPosition` flips to read from store, ALL of
  `position_ = …` writes in `atom.C` `operator=`, `swap`, `clear_`,
  constructors must route through the new setter.

**Finding 5 (HIGH): CONFIRMED.** Prototype design works for handfuls
of atoms but won't scale. D16 added: drop lazy-per-Atom store; replace
with single process-global "orphan store" or require System reference
in `Atom()` ctor.

**Finding 6 (HIGH): CONFIRMED.** `moleculeStore.C:144` is the linear
scan. `bonds_of()` is called from every "give me this atom's bonds"
caller. K0.3a prototype masked the cost because tests use ≤5 bonds.
D15 added: CSR adjacency mandatory in K0.2b (a new step) before any
Atom/Bond facade perf claim.

**Finding 7 (HIGH): CONFIRMED.** Citations:
- `atom.C:105-128` shows v1 persistence writes payload fields + bond
  table.
- `bond.C:121-130` writes raw atom pointer linkage.
- `AtomContainer_test1.C:247-267` exercises `TextPersistenceManager`.

  D12 amended: K0.6 split into K0.6a (format design + invariants +
  corpus, 2 weeks) + K0.6b (implementation + converter, 3-4 weeks).

**Finding 8 (MEDIUM): CONFIRMED.** Inheritance grep:
- `include/BALL/STRUCTURE/ringAnalyser.h:51` — `Ring : PropertyManager`
- `include/BALL/FORMAT/PDBFile.h:55` — `PDBFile : PropertyManager`
- `include/BALL/NMR/peak.h:35` — `Peak<P> : PropertyManager`
- `include/BALL/VIEW/KERNEL/representation.h:57` —
  `Representation : PropertyManager`

  These are downstream — out of K0 scope per D10 — so they don't block
  K0 completion. But Codex is right that they undercount the v2.0
  cycle's total rewrite cost. D3 amended: PropertyManager stays as a
  proxy until v2.1 specifically to give downstream a migration window.

---

## Net amendments to KERNEL-V2-DECISIONS.md

- **D2/D3/D4:** "Full removal" → "Thin stub / proxy". Composite,
  PropertyManager, Selectable stay as base classes for K0; full
  deletion deferred to v2.1.
- **D6:** Atom handle size widened to 24-32 B (accounts for vtable +
  stub-base subobjects); back_ptr cost explicit.
- **D7:** Live-reference contract; debug enforcement; getPositionPtr is
  for callers that can't comply.
- **D10:** Marked as only satisfiable BECAUSE D2/D3/D4 are now stubs;
  full deletion would violate D10.
- **D12:** Split K0.6 into design (K0.6a) + implementation (K0.6b).
- **D13:** Split into typical-case ≤160 B/atom + worst-case
  documented-as-out-of-budget.
- **New D15:** CSR mandatory in K0.2b (NEW step).
- **New D16:** Process-global orphan store replaces lazy per-Atom store.
- **K0 phasing:** Inserted K0.2b (CSR) + K0.2c (lease enforcement)
  + K0.6a (format design). New total: 28-39 weeks (was 25-35).
- **§6.5 added:** Explains which test surfaces survive only because the
  stubs are kept.

---

## What this means for Path-2

Path-2 (phased migration with transient parallel-data) is still the
right shape. The amendments tighten it:

1. **K0.2b CSR** must land before K0.3b touches atom.h.
2. **K0.2c lease enforcement** must land before K0.3b flips any getter.
3. **K0.3b per-field checklist:** for each field (position, charge,
   element, etc.), identify ALL direct writers (operator=, swap,
   clear_, friend Bond::createBond, ctors, etc.) and migrate them
   ALL before flipping the getter. Document each migration as a
   separate small commit.
4. **K0.6a persistence design** happens before K0.6b — no
   implementation until invariants + corpus signed off.

This adds 3-4 weeks to K0 but eliminates the failure modes Codex
identified.

---

## Disposition

All 7 verified findings drove amendments. No findings dismissed.
Codex's overall assessment ("not implementation-ready as locked")
accepted. **Implementation-ready post-amendment.**

Next action: proceed with K0.2b (CSR) per the amended plan.
