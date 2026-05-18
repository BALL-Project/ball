# K0 Adversarial Review — Codex CLI Round 2 (2026-05-18)

**Status:** Complete. **Action:** All 9 findings cross-verified;
verified findings drive amendments to `KERNEL-V2-DECISIONS.md` adding
**K0.3c.1-6** between K0.3b.8 and K0.4.

**Reviewer:** Codex CLI 0.128.0 (`codex exec`).
**Subject:** The 8-commit K0.3b.* dual-write migration plus K0.2b CSR /
K0.2c lease enforcement that landed on `v2.0` after the Round-1 review.

**Scope of review:** prompted Codex to attack:
1. Orphan store memory growth + reclamation
2. Bond shadow consistency under delete/rebond
3. Atom::swap with different stores
4. Path-2 drift residue (mutable-reference writes)
5. Lease pattern weakness (opt-in only)
6. Memory budget D13 interim cost
7. K0.4 System.adopt risks
8. Persistence interaction with dual-write

---

## Findings (Codex's verbatim summary)

| # | Sev | Area | Finding |
|---|---|---|---|
| 1 | BLOCKER | Bond shadow | Store-side bonds are append-only. `Bond::~Bond` / `Bond::clear` / `Bond::destroy` / `Atom::destroyBond` / `Atom::destroyBonds` only mutate v1 arrays. Store CSR reports dead bonds as live. |
| 2 | BLOCKER | Orphan store | Every Atom ctor allocates a permanent orphan slot + back_ptr; destruction never releases. Dead atoms leak rows, string-pool entries, bond records for process lifetime. |
| 3 | HIGH | Path-2 drift | `a.getPosition() = v`, `a.getVelocity() = v`, `a.getForce() = v` mutate v1 fields directly via the non-const ref-returning getter; never touch store columns. |
| 4 | HIGH | Atom::swap | Swap moves `bond_[]` and `number_of_bonds_`, but mirror_all only handles scalar/vector/name fields. Store connectivity stays attached to pre-swap slots. |
| 5 | HIGH | Bond metadata | `Bond::setOrder` / `Bond::setType` don't update `BondRecord`. Store bond metadata is commonly stale. |
| 6 | HIGH | Lease contract | `BorrowedColumnRef<T>` is opt-in only. `Atom::getPosition()` returns a naked `Vector3&` — doesn't increment the lease counter — so K0.2c's safety can't detect post-K0.4 getter-flip drift either. |
| 7 | MEDIUM | Cross-store bonds | Silently skipped when atoms in different stores. Tolerable now; dangerous during partial `System::adopt`. |
| 8 | MEDIUM | Persistence | `Atom::persistentRead` restores `bond_[]` but doesn't populate store bonds. Reading v1 persistence yields atom columns + an empty/stale store bond graph. |
| 9 | LOW | Swap back_ptr | (Codex retracted Round-1's concern; back_ptr is not stale after swap because slots represent handle identity.) |

---

## Cross-verification (Claude)

All 8 substantive findings verified. Codex provided concrete proof
patterns; the assertions they predict to fail are accurate against the
current tree.

**Proof patterns from Codex** (these are the regression tests K0.3c
will add):

```cpp
// Finding 1 (Bond shadow append-only)
Atom a, b;
Bond* bond = a.createBond(b);
auto* store = a.getStore();
auto ai = a.getStoreIndex();
TEST_EQUAL(store->bond_degree(ai), 1)
a.destroyBonds();
TEST_EQUAL(a.countBonds(), 0)
TEST_EQUAL(store->bond_degree(ai), 0)  // currently FAILS: still 1

// Finding 3 (mutable ref drift)
Atom a;
Vector3& p = a.getPosition();
p = Vector3(7, 8, 9);
TEST_EQUAL(a.getStore()->position(a.getStoreIndex()), Vector3(7, 8, 9))
// currently FAILS: store has the default 0/0/0

// Finding 5 (bond metadata drift)
Atom a, b;
Bond* bond = a.createBond(b);
bond->setOrder(Bond::ORDER__DOUBLE);
auto rec = a.getStore()->bond(a.getStore()->bonds_of(a.getStoreIndex())[0]);
TEST_EQUAL(rec.order, Bond::ORDER__DOUBLE)
// currently FAILS: stored as initial order (1 from createBond default)
```

**Leak quantification** (Codex): ~76 B per orphan atom + ~20 B per bond.
Within a single long-running process this grows linearly. The 36-test
suite passes only because each test binary's exit kills the orphan
store; BALLView, REST servers, and Jupyter sessions don't have that
escape hatch.

---

## Disposition — net amendments

All findings drive K0 phasing changes. **K0.3c.1-6 inserted between
K0.3b.8 (current HEAD) and K0.4 (System.adopt).** K0.4 is no longer
the next step; the bond/destruction/swap drift must close first.

### New K0.3c subphases

| Step | Scope | Estimate | Gate |
|---|---|---|---|
| K0.3c.1 | `MoleculeStore::release_atom(Index, Atom*)` — free-list / tombstone semantics. `Atom::~Atom` clears back_ptr + releases slot + removes incident bonds | ~3 days | New `OrphanStoreReclaim_test`: 1000 default-constructed atoms then deleted → store size returns to baseline ±10 atoms |
| K0.3c.2 | `MoleculeStore::remove_bond(uint32_t)` + tombstones + CSR rebuild ignoring dead bonds. Wired into `Bond::~Bond` / `Bond::clear` / `Bond::destroy` / `Atom::destroyBond` / `Atom::destroyBonds` | ~3 days | New `BondShadowConsistency_test`: bond → destroy → store->bond_degree(a) == 0 |
| K0.3c.3 | Bond store-aware: `Bond` holds `bond_record_idx_`; `Bond::setOrder` / `setType` / endpoint changes mirror into BondRecord | ~5 days | New `BondMetadataDrift_test` covering setOrder/setType/swap |
| K0.3c.4 | `Atom::swap` mirrors bond connectivity (swap store CSR rows for the two atoms) | ~2 days | Extend Atom_test1's swap check to verify store CSR rows swap too |
| K0.3c.5 | `Atom::persistentRead` populates store bonds after v1.x bond_[] fixup | ~3 days | Existing persistence test extended: after read, store_->bond_degree(...) matches countBonds() |
| K0.3c.6 | Mutable-getter drift: either return a `PositionRef` proxy that writes through to store + v1 field, or explicitly document the gap with a `MutableRefDriftAcceptable_test` that locks the v1-only semantics until K0.3b.LATER's getter flip | ~5 days | One of: (a) proxy lands and write-through verified; or (b) explicit gap-acceptance test green + release note line |

**K0.3c parallel-assignable:** these six steps touch different files and
different test patterns. K0.3c.1 (atom destruction) and K0.3c.2 (bond
removal) share `MoleculeStore`'s mutation surface and should serialise
to avoid merge churn. K0.3c.3/4/5/6 can land in any order after the
first two. With 2 FTE, K0.3c lands in 1-2 weeks total instead of
3-4 weeks sequential.

### Decision amendments

- **D12 amended:** persistence work now has an explicit dependency on
  K0.3c.5 (bond mirror from persistence) before K0.6a/b can be locked.
- **D7 strengthened:** ref-return semantics are explicitly documented as
  v1-only-until-K0.3b.LATER. Mutable references that bypass the
  setter are a **known gap**, tracked by K0.3c.6's test, closed when
  the getter flip lands.
- **Cross-store bonds (Finding 7):** add an explicit `noexcept` rejection
  to `Bond::createBond` when atoms are in different stores. Currently it
  silently no-ops the store mirror; that's tolerable now but lethal
  during partial adoption. Land this in K0.3c.3.

### Memory budget interim

Codex's quantification confirms the K0.3b interim costs **~558 B/atom**
(v1.x ~460 + store columns 74 + handle 24). This is **3.5× over the
160 B/atom HARD cap for end state**, but the cap is for END STATE
(post K0.3b.LATER + Composite/PropertyManager/Selectable stub
shrinkage). No amendment to D13 needed; the interim cost was already
acknowledged in K0.3b.7's commit message.

**Worth tracking:** if K0.3c lands without freeing the orphan store
leak, the test suite RAM consumption could grow significantly. K0.3c.1
addresses this; verify with `/usr/bin/time -lp` on the full sweep
after that step.

---

## Why Round 1 missed these

Round 1 didn't catch the bond-destroy / mutable-getter / Bond-metadata
issues because the prompt focused on the Atom data path (position,
charge, etc.) and the parallel-vs-replacement architecture choice.
Round 2 prompted explicitly for graph mutation and ref-leak attacks
— hence the new findings.

This pattern (one review surfaces architecture, second review surfaces
graph mutation) suggests a **Round 3 prompt** focused on selection /
visitor / persistence might surface another batch. Schedule Round 3
after K0.5 (compiled selection) lands.

---

## Net status

**K0.4 System.adopt is now blocked** until K0.3c.1-6 closes the graph
mutation drift. K0.4 design itself doesn't change — the per-System
store + adoption transaction still applies — but its preconditions
just expanded by 6 commits.

**v2.0 timeline impact:** +1-2 weeks (parallel-assignable). New total:
**16-23 weeks remaining** with aggressive parallelism (was 15-21).

**Next concrete action:** kick off K0.3c.1 (orphan store reclaim) +
K0.3c.2 (bond removal) sequentially; K0.3c.3/4/5/6 can run in
parallel after both land.

---

*End of K0-CODEX-REVIEW-ROUND2.md.*
