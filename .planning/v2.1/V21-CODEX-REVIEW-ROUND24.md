# Track A v2.1 Cluster R24 — Codex CLI Round 24 (2026-05-20)

**Status:** Complete
**Verdict:** GO
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.1 P4 planning review (V21-LOAD-BATCH)

## Probe answers

**P24-1. Root-cause validation:** Confirmed. `loadSystemJSON` creates all live atoms with `new Atom` before insertion (`source/KERNEL/systemJson.C:399-403`). It then inserts each molecule atom individually with `m->insert(*atom_by_save_idx[save_idx])` (`systemJson.C:420-424`). `Molecule::insert(Atom&)` delegates to `AtomContainer::insert/append`, and `AtomContainer::append(Atom&)` calls `sys->adopt(atom)` after `Composite::appendChild(atom)` (`source/KERNEL/molecule.C:101-104`, `source/KERNEL/atomContainer.C:307-317`). `System::adopt(Atom&)` calls `src->for_each_bond_of(src_idx, ...)` on the source store (`source/KERNEL/system.C:143`), which currently calls `ensure_csr_()` unconditionally (`include/BALL/KERNEL/moleculeStore.h:376-380`). `ensure_csr_()` rebuilds offsets sized to `positions_.size() + 1` and scans `bonds_` (`source/KERNEL/moleculeStore.C:815-849`), so the empty-bond case is still O(n_atoms). `allocate_atom` marks `csr_dirty_ = true` on both fresh allocation and free-list reuse (`moleculeStore.C:378`, `moleculeStore.C:417`), and `release_atom` also marks it dirty (`moleculeStore.C:456`). Therefore every per-atom adopt in the loader dirties the orphan store again before the next `for_each_bond_of`. For the profiled loader path, where bonds are restored only after the insert phase into the destination store (`systemJson.C:552-575`), this is the claimed O(n_atoms * n_adopts) empty CSR rebuild.

**P24-2. `bonds_.empty()` correctness:** Semantically safe for valid atom indices. `bonds_` is the owning table of all bond records; if it is empty, no atom in that store can have an incident store-level bond record. A non-empty table may contain only tombstoned entries, so `empty()` can miss an optimization, but that is conservative and correct. One small API caveat: returning from `bond_degree(i)` / `bonds_of(i)` before CSR construction changes invalid-index behavior in an empty-bond store from likely out-of-bounds access to `0` / empty vector. That is not a real semantic contract, but tests should use valid indices.

**P24-3. Predicate choice:** `bonds_.empty()` is enough for the P4.1 benchmark and for freshly created bondless atoms. There are workloads where it is insufficient: a long-lived store can have only dead bond records, or records whose endpoints are freed and filtered by CSR, and `bonds_.empty()` will stay false. The existing `live_bond_count()` is not a perfect substitute for "queryable incident bonds" because bonds touching freed atom slots are not necessarily tombstoned by `release_atom`; CSR filters them during rebuild. A stronger O(1) predicate would need a maintained count for queryable/live endpoint pairs, not just `bonds_.size() - bond_free_list_.size()`. For v2.1, the simple `empty()` predicate is the right low-risk fix for the measured load regression.

**P24-4. Guard location:** Prefer putting the fast path in `ensure_csr_()` itself:

```cpp
if (!csr_dirty_) return;
std::lock_guard<std::mutex> lk(bond_mutex_);
if (!csr_dirty_) return;
if (bonds_.empty()) {
  bond_csr_off_.assign(positions_.size() + 1, 0);
  bond_csr_idx_.clear();
  csr_dirty_ = false;
  return;
}
```

This centralizes the invariant and covers `for_each_bond_of`, `bond_degree`, `bonds_of`, and any future caller. Clearing dirty is correct because `add_bond()` dirties CSR on both push and tombstone-slot reuse (`moleculeStore.C:724`, `moleculeStore.C:736`). Guarding the three public callers is also correct, but it leaves the empty-store CSR permanently dirty and duplicates the policy. If caller guards are added anyway, they should be treated as a secondary micro-optimization, not the primary correctness location.

**P24-5. Thread-safety:** The unlocked `bonds_.empty()` read would be UB if it raced with `add_bond()` / `remove_bond()` / vector mutation. The documented store contract already forbids concurrent bond mutation while reading bond adjacency: after `ensure_csr_()`, subsequent CSR and `bonds_` reads are explicitly not mutex-protected and callers must avoid concurrent bond mutators (`include/BALL/KERNEL/moleculeStore.h:517-528`). Under that contract, the unlocked `empty()` fast path is no worse. If the guard is moved into `ensure_csr_()` after acquiring `bond_mutex_`, this specific new unlocked vector read disappears. Note: the existing `ensure_csr_()` already reads `csr_dirty_` before locking (`moleculeStore.C:811`), so this subsystem is not designed for arbitrary concurrent mutator/query races.

**P24-6. `adoptSubtree` vs `adopt`:** The loader really uses single-atom adoption for molecule contents. `sys.insert(*m)` happens before atoms are inserted into `m`, so that initial molecule-level `adoptSubtree` sees an empty molecule (`systemJson.C:409-415`, `source/KERNEL/system.C:527-540`). The subsequent `m->insert(*atom)` calls use the per-atom path. Reworking the loader to build molecules off-tree and insert each completed molecule once would route through `adoptSubtree`, and that is a valid complementary design. It is not necessary to close this benchmark regression if Layer 1 removes the empty CSR rebuild. The larger loader rewrite has more rollback/ownership surface than the CSR fast path.

**P24-7. Layer 2 reserve interaction:** `MoleculeStore::reserve` reserves all atom columns plus handle/freed side vectors: positions, velocities, forces, charges, radii, atom types, formal charges, element indices, selection, name offsets, type-name offsets, name strings, type-name strings, stable ids, back pointers, and freed flags (`source/KERNEL/moleculeStore.C:459-480`). It does not reserve the string pool or bond table, but that is unrelated to per-atom `allocate_atom` column reallocation. Calling `sys.getStore().reserve(n + 64)` after `sys.getStore().clear()` is correct and should prevent the destination store's repeated vector growth during the adopt storm.

**P24-8. Broader adopt O(n²):** The empty-bond guard fixes the measured case only. Atom-by-atom adoption from a source store with actual bond records can still rebuild CSR repeatedly because `dst->allocate_atom()` and `src->release_atom()` dirty their respective stores inside each single-atom adopt. For bonded multi-atom moves, the intended answer is already `adoptSubtree`, and `canAdopt()` rejects unsafe single-atom bonded adoption. However, there remains a latent performance issue for loops that adopt many atoms individually from a source with non-empty bond history, especially if records are dead or filtered. Track as follow-up; do not block P4.1.

**P24-9. Test adequacy:** The proposed micro-test plus `JsonBench_test` gate plus full CORE_ONLY ctest is mostly sufficient, but the micro-test should be made observable without relying on private CSR state if possible. Recommended coverage:

- Valid-index behavior: empty store with allocated atoms returns degree 0, empty `bonds_of`, and no `for_each_bond_of` callbacks.
- Dirty-after-atom-mutation behavior: allocate/release atoms in a bondless store, then repeated adjacency queries remain fast and correct.
- Non-empty correctness parity: existing `MoleculeStore_test` bond-degree, tombstone, mutation-safe iteration, and adoptSubtree tests continue to pass.
- Benchmark gate: tighten `JsonBench_test` only after measuring post-fix numbers with headroom.

**P24-10. Overall:** GO to implement Layer 1 + Layer 2. I recommend implementing Layer 1 as an `ensure_csr_()` empty-table fast path, with optional caller-level early returns only if profiling shows the mutex/double-check still matters.

## New findings

1. The design's root cause is real, but its wording "orphan store has ZERO bonds during load" is only guaranteed for the profiled fresh benchmark shape. `orphanStore()` is process-global. A long-running process may have accumulated orphan bond records or tombstones from unrelated prior activity, in which case `bonds_.empty()` will not fire and the O(n²) path can return.

2. `live_bond_count()` is not a drop-in better predicate. Because `release_atom()` does not tombstone incident bonds, a store can have non-dead bond records that CSR will filter out due to freed endpoints. A correct cached predicate would need to match CSR's live-bond definition, not just subtract `bond_free_list_`.

3. Guarding only `for_each_bond_of`, `bond_degree`, and `bonds_of` duplicates cache policy and leaves an empty store dirty forever. That is correct but untidy; centralizing in `ensure_csr_()` is less fragile.

4. The proposed micro-test phrase "without dirtying" is imprecise. `allocate_atom` and `release_atom` intentionally dirty CSR. The test should assert that empty-bond adjacency queries do not perform the expensive rebuild and still return correct empty adjacency, not that atom mutations avoid dirtying.

5. Loader batching via completed-molecule insertion remains a valid future improvement, especially for bonded or non-empty-source cases, but it is a higher-risk loader ownership change and should not displace the small kernel fast path for P4.1.

## Overall verdict

GO. The P4.1 design identifies the actual hot path and the proposed empty-bond short-circuit is semantically safe for the measured System JSON load. Implement the fast path centrally in `ensure_csr_()`, add destination-store reserve after `clear()`, measure `BALL_JSON_LOAD_PROFILE=1`, then set the `JsonBench_test` load gate from observed results with CI headroom. Track broader non-empty-source adopt batching as follow-up rather than expanding P4.1.
