# P4.0 — JSON Load Profile (100k atoms)

**Phase:** P4.0 (gate before P4.1 V21-LOAD-BATCH design lock).
**Status:** Complete.
**Date:** 2026-05-20.
**Source:** D41.2 (R21 P21-5 / R21-F1) — profile-driven target.
**Instrumentation:** `BALL_JSON_LOAD_PROFILE=1` env-gated profiler
in `source/KERNEL/systemJson.C` (`LoadProfile` struct).

## Method

Ran `JsonBench_test` (existing 100k-atom / 100-molecule harness)
with `BALL_JSON_LOAD_PROFILE=1` on Apple Clang arm64 Release
CORE_ONLY. The profiler buckets `loadSystemJSON` into the 6 P4.0
roadmap categories + validation + stable-id.

## Results

### 100k atoms, NO per-atom properties

```
parse    =   99.5 ms   ( 2.8%)
validate =    5.6 ms   ( 0.2%)
alloc    =   13.0 ms   ( 0.4%)
insert   = 3414.6 ms   (95.2%)   <-- DOMINANT
columns  =   50.9 ms   ( 1.4%)   (of which props = 5.6)
stableid =    4.5 ms   ( 0.1%)
bonds    =    0.0 ms   ( 0.0%)
-----------------------------------
total    = 3588.0 ms
```

### 100k atoms, +3 properties/atom

```
parse    =  201.6 ms   ( 5.4%)
validate =    5.6 ms
alloc    =   11.0 ms
insert   = 3426.0 ms   (91.4%)   <-- DOMINANT
columns  =  101.6 ms   ( 2.7%)   (of which props = 55.5)
stableid =    4.4 ms
bonds    =    0.0 ms
-----------------------------------
total    = 3750.3 ms
```

## Analysis — the dominant bucket is `insert`

**95% of load time** (no-props) is the **Composite-tree insertion
+ per-atom orphan→System adoption** phase:

```cpp
// systemJson.C Phase 2:
for (mol_obj) {
    Molecule* m = new Molecule;
    sys.insert(*m);
    for (save_idx in mol_obj["atom_indices"]) {
        m->insert(*atom_by_save_idx[save_idx]);   // <-- 3.4 s total
    }
}
```

Each `m->insert(*atom)` routes through
`AtomContainer::append(Atom&)`:

```cpp
void AtomContainer::append(Atom& atom) {
    Composite::appendChild(atom);                 // O(1) pointer ops — cheap
    if (System* sys = findRootSystem_(this))
        sys->adopt(atom);                          // <-- the expensive part
}
```

The atoms were created with `new Atom` (line 352), which binds them
to the **process-global orphan store**. Each `sys->adopt(atom)`
**migrates one atom from the orphan store to sys's store**:
allocate a fresh slot in the destination store, copy all SoA
columns, then release the orphan slot. Doing this **100,000 times
individually** is the 3.4-second cost.

Confirmation that the per-atom migration dominates:
- `alloc` (the 100k `new Atom`) is only 13 ms — Atom construction
  itself is cheap.
- `columns` (writing positions/charges/etc. via the slot map) is
  only 51 ms — bulk column writes are fast.
- The 3.4 s is purely the `insert`-path adoption overhead.

## V21-LOAD-BATCH design target (P4.1)

The roadmap's conditional (D41.2) resolves to: **the (c)
Composite-insert/adopt bucket dominates → batch the insertion,
defer/eliminate the per-atom adoption.**

Candidate strategies for P4.1 (to be designed + R24-reviewed):

1. **Allocate atoms directly in the destination store** instead
   of the orphan store. If the loader creates atoms already bound
   to `sys.getStore()`, the orphan→sys migration disappears
   entirely. The Composite-tree insert becomes O(1) pointer ops
   only (the cheap part). Requires a loader-only path to
   construct an Atom pre-bound to a specific store + index.

2. **Defer adoption until all atoms inserted, then one batched
   adoptSubtree.** Insert all atoms into molecules first (orphan-
   bound, Composite-tree only), then a single `sys.adoptSubtree`
   pass migrates the whole tree. The batched migration can
   pre-reserve the destination store (avoiding per-atom realloc)
   and copy columns in bulk.

3. **Bulk slot pre-allocation + direct column write.** Reserve
   `n` slots in sys's store up front; assign each loaded atom a
   pre-determined fresh index; write columns directly; build the
   Composite tree referencing those indices without adoption.

Strategy 1 or 3 is likely the biggest win because it eliminates
the orphan-store round-trip entirely. The store already has a
`reserve()` API (JsonBench's `build_` uses
`sys.getStore().reserve(N + 64)`). The K0.4 adoption machinery
exists for the interactive-edit path; the JSON loader is a bulk
path that can bypass it.

### Realistic target

If `insert` (3414 ms) collapses to roughly the `alloc` + `columns`
cost (~64 ms) plus a bounded Composite-tree-link pass (~100-200 ms
estimated for 100k O(1) appendChild calls), total load drops from
~3588 ms to roughly **~400-500 ms** — an **~7-9× load speedup**,
landing near the roadmap's "≤2× save:load ratio" aspiration
(save is ~230 ms; 2× = ~460 ms).

**P4.1 commits to a measured target** after the chosen strategy's
prototype, not before. The profile justifies aiming at the insert
bucket; the exact multiple depends on how cleanly the orphan
round-trip can be eliminated.

## Out-of-scope confirmations

- **parse** (100-200 ms): nlohmann parse cost. Not worth
  optimizing in v2.1 — it's <6% of load and swapping the JSON
  parser is a larger project (the K0.6 binary-fast-path idea,
  explicitly deferred).
- **columns / props**: already fast (51-102 ms). The property
  restore at 55 ms for 300k property entries is acceptable.
- **stableid / bonds**: negligible.

## Next action

P4.1 design lock: choose strategy (1/2/3 above), prototype,
measure, commit with the achieved number. R24 planning review
gates the design before the batch-loader implementation lands.
