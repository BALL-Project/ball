# K0 Adversarial Review — Codex CLI Round 3 (2026-05-18)

**Status:** Complete. **Action:** 3 of 4 HIGH findings closed by
K0.3c.7+8+10 (commit e0faf2b5c). HIGH-9 (string pool leak) deferred
as a follow-on. MEDIUM/LOW findings tracked for K0.7 perf phase.

**Reviewer:** Codex CLI 0.128.0 (`codex exec`).
**Subject:** K0.3c.1-6 (commits d83bfdbe9..6779766cc) plus K0.4.1
(commit 58a2daff5).

---

## Findings (Codex)

| # | Sev | Finding | Status |
|---|---|---|---|
| 7 | HIGH | String-pool leak open. release_atom doesn't reclaim string_pool_; every Atom() ctor appends "?\0" again | DEFERRED → K0.3c.9 |
| 8 | HIGH | Free-list reuse creates window where is_freed==false but back_ptr==null | CLOSED in K0.3c.8 (atomic allocate_atom(Atom*) overload) |
| 9 | HIGH | for_each_bond_of mutation-unsafe (callback can remove bond mid-iteration, corrupting outer CSR rebuild) | CLOSED in K0.3c.10 (snapshot before callback) |
| 10 | HIGH | Cross-store bond state not K0.4-ready (Bond's bond_record_idx_ has no owning store) | CLOSED in K0.3c.7 (Bond holds bond_store_) |
| 11 | MEDIUM | Bond::finalize() can create extra store records on endpoint rewrite | Will be addressed during K0.4 testing |
| 12 | MEDIUM | Memory budget not measured post-K0.3c | DEFERRED → K0.7 |
| 13 | LOW | Dead bond records don't have endpoints rewritten by swap_atom_connectivity | Acceptable (CSR skips dead; reuse overwrites on add_bond) |
| 14 | LOW | Mutable-ref drift only caught by K0.3c.6 lock test | Acceptable until K0.3b.LATER flips getters |

---

## Why K0.4.2 Is Now Ready

K0.3c.7+8+10 closed the three HIGH findings that gated K0.4.2:

- **K0.3c.7 (Bond owns store):** during K0.4 adoption, one endpoint
  atom migrates from orphan store → System's per-instance store.
  Without bond_store_, Bond::setOrder etc. would re-derive
  first_->getStore() and target the wrong store. With bond_store_
  pinned at createBond time, mirror operations always target the
  original store regardless of subsequent endpoint adoption.

- **K0.3c.8 (atomic atom binding):** allocate_atom(Atom* back) sets
  both is_freed_=0 and back_ptr_=back atomically. Closes the window
  where a freed slot is reused but back_ptr is still nullptr.

- **K0.3c.10 (safe for_each_bond_of):** snapshot the CSR slice
  before invoking callback. Adoption code can safely iterate
  incident bonds and remove/add bonds in the callback without
  corrupting iteration.

K0.4.2 design — System.adopt(Atom&) — can now:
1. Snapshot atom payload from orphan store (positions, charges, ...)
2. allocate_atom(&atom) in System's store (atomic bind via .8)
3. Copy payload to new slot
4. For each bond incident to atom in orphan store (via .10 safe iter):
   - If partner is also in System's store: re-add bond in System's
     store, update partner Bond's bond_store_ (or remove old bond_store_
     entry and create new)
   - If partner is still in orphan: leave bond as-is; bond_store_
     still points to orphan; when partner adopts later, K0.4 re-
     visits the bond
5. release_atom(orphan_idx) on orphan store
6. Update atom.store_ + atom.store_idx_ to System's store

The cross-store bond invariant (.7) means setOrder/setType on a
partially-adopted bond still works — it targets the orphan store's
record. When the partner is also adopted (step 4 above), the bond
migrates and bond_store_ updates.

---

## Open: K0.3c.9 string pool leak

Codex proof pattern:
```cpp
Atom probe;
auto* s = probe.getStore();
auto before = s->string_pool().size();
for (int i = 0; i < 10000; ++i) { Atom tmp; }
TEST_EQUAL(s->live_atom_count() <= 2, true)
TEST_EQUAL(s->string_pool().size(), before)   // currently FAILS
```

Three options:
1. **Append-only with intern table.** Add a hash from string → offset
   so set_name appends only the first time per unique string.
   Memory: small per-string overhead. Tradeoff: hash adds CPU on
   every setName.
2. **Reclaim on release_atom.** Mark string_pool regions as freed
   alongside the atom slot. Compact via background sweep. Complex.
3. **Bounded leak ledger.** Pre-allocate string_pool to a known cap;
   document the leak as O(unique_strings_observed). Simplest but
   not actually a fix.

Recommendation: Option 1 (intern table). Estimate: ~1-2 days.
File a K0.3c.9 commit before K0.7 perf benchmark.

---

## K0.4 Readiness — Codex Round 3 verdict (Claude follow-up)

Original Codex verdict: "Not ready." Two blockers identified:
- Non-atomic atom slot binding → CLOSED in K0.3c.8
- Lack of bond-record store ownership → CLOSED in K0.3c.7

Both are now closed. K0.4.2 (atom payload migration via System.adopt)
can proceed.

---

## Net amendments

`KERNEL-V2-DECISIONS.md` §5 phasing table will be amended to insert
K0.3c.7-10 (this commit and the deferred .9) between K0.3c.6 and
K0.4. K0 timeline +1 week (4 new mini-subphases at ~2 days each;
parallel-eligible).

`V2.0-ROADMAP.md` §3 K0.3c row gets the same insertion.

---

*End of K0-CODEX-REVIEW-ROUND3.md.*
