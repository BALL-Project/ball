# H4 Design Summary: Atom-as-Pure-Handle + GROMACS-Style SoA

**Full document:** [H4-ATOM-AS-PURE-HANDLE-GROMACS-SOA-DESIGN.md](H4-ATOM-AS-PURE-HANDLE-GROMACS-SOA-DESIGN.md)

---

## GROMACS Layout (Part A)

### Key Findings

1. **GROMACS uses AoS `rvec[3]` for storage** (`typedef real rvec[3]` in vec.h:42)
   - State: `t_state` with `x[]`, `v[]`, `f[]` as `rvec*` arrays
   - **NOT SoA x/y/z at storage level**

2. **SoA appears ONLY in nbnxn kernels** as **temporary repacking**
   - Input: AoS `rvec x[]` from `t_state`
   - Repack: SoA `float x[N]`, `y[N]`, `z[N]` for j-clusters (Q-padded)
   - Why: SIMD efficiency for 4×4 atom cluster processing

3. **Topology vs State separation**
   - `gmx_mtop_t` = topology (moltype, molblock, atoms, idef)
   - `t_state` = state (coordinates, velocities, forces)
   - **Independent objects**, linked only by atom indices

4. **Bonds as interaction lists**
   - `t_idef.ilists[F_BONDS].iatoms[]` = flat array `[a1, b1, a2, b2, ...]`
   - **No per-atom bond arrays** (unlike BALL's `bond_[12]`)

5. **Atom identity**
   - Global atom number (0..N-1) is primary identity
   - DD uses `gatindex[]` for global↔local mapping
   - **No handle invalidation** because global indices never change

---

## Target BALL Design (Part B)

### Atom Becomes a 24B Handle

```cpp
// atom.h (post-H4)
class BALL_EXPORT Atom final
{
    MoleculeStore* store_           = nullptr;   // 8 B
    std::uint32_t  store_idx_       = 0;         // 4 B  
    std::uint64_t  store_generation_ = 0;         // 8 B
    // Total: 24 B (no vtable - final class)
};
```

**Identity:** StableId (8B) + Generation (8B) stored in MoleculeStore
- StableId: Never reused (ABA-safe)
- Generation: Store-wide counter, bumps on reallocation

### SoA Column Set

**Current MoleculeStore columns (moleculeStore.h:668-688):**
- `positions_`, `velocities_`, `forces_`: `vector<Vector3>` (12B/atom each)
- `charges_`, `radii_`: `vector<float>` (4B/atom each)
- `atom_types_`, `formal_charges_`: `vector<short>` (2B/atom each)
- `element_indices_`, `selection_`, `origin_flags_`, `is_freed_`: `vector<uint8_t>` (1B/atom each)
- `stable_ids_`: `vector<StableId>` (8B/atom)
- **Total: ~64B/atom fixed**

**Enhancement:** Add SoA x/y/z split
```cpp
std::vector<float> positions_x_;  // 4 B/atom
std::vector<float> positions_y_;  // 4 B/atom  
std::vector<float> positions_z_;  // 4 B/atom
// Total: 12 B/atom (no padding)
```

### Vector3& getPosition() Retirement

**The Problem:** 3,106 call sites use reference-returning `getPosition()`

**Migration Strategy:**

| Category | Count | Strategy | Risk |
|----------|-------|----------|------|
| Read | ~1,500 | `atom.getPosition()` → `store->position(idx)` | Low |
| Write | ~500 | `atom.getPosition() = v` → `store->setPosition(idx, v)` | Low |
| Modify in-place | ~1,100 | `a.getPosition() += dx` → `a.setPosition(a.getPosition() + dx)` | Medium |
| Reference capture | ~50 | `Vector3& p = a.getPosition()` | **HIGH - BREAKS** |

**Solution:** Remove non-const `getPosition()`, force value semantics
```cpp
const Vector3& getPosition() const { return store_->position(store_idx_); }
Vector3 getPosition() { return store_->position(store_idx_); }  // By value
void setPosition(const Vector3& v) { store_->setPosition(store_idx_, v); }
```

### Bonds: CSR + BondRecord

**DELETE from Atom (atom.h:1022, 1012):**
```cpp
Bond* bond_[MAX_NUMBER_OF_BONDS];  // 96 B - GONE
unsigned char number_of_bonds_;    // GONE
```

**Bond as 24B handle:**
```cpp
class BALL_EXPORT Bond final
{
    MoleculeStore* store_ = nullptr;       // 8 B
    std::uint32_t  bond_idx_ = 0;          // 4 B
    std::uint64_t  bond_generation_ = 0;    // 8 B
    std::uint8_t   padding_[4] = {};        // 4 B
};
```

**Bonded-term loops (GROMACS-style):**
```cpp
// BondRecord iteration (O(bonds))
for (std::uint32_t i = 0; i < store->bond_count(); ++i) {
    const BondRecord& br = store->bond(i);
    if (br.flags & FLAG_BOND_DEAD) continue;
    Index a1 = br.a, a2 = br.b;
    // Compute energy
}

// CSR adjacency (per-atom, O(degree))
store->for_each_bond_of(atom_idx, [&](std::uint32_t bond_idx) {
    const BondRecord& br = store->bond(bond_idx);
    Index partner = (br.a == atom_idx) ? br.b : br.a;
    // Compute energy
});
```

---

## Separated Tree (Part C)

### Current State

**Container table already exists** (v2.2 H1b, moleculeStore.h:497-527):
- `container_kind_`, `container_parent_`, `container_child_`
- `atom_parent_container_idx_`
- **Separated from numeric SoA columns** ✓

### Decision: Hybrid Contiguous/Scattered

| Aspect | GROMACS | BALL Target |
|--------|---------|--------------|
| Storage | Contiguous ranges (`t_molblock.atom_start/end`) | **ChildRef scattered indices** |
| Kernel API | Direct array access | **Lazy contiguous view** |
| Mutation | Never (after grompp) | Frequent (editor) |

**Why hybrid:**
- **Mutation requires scattered indices** (insert mid-residue)
- **Kernels want contiguous ranges** (SIMD, cache)
- **Solution:** Keep scattered storage, provide contiguous view for kernels

```cpp
// Storage: scattered
container_append_atom_(container_idx, atom_idx);

// Kernel: contiguous view (lazily copies if needed)
ContainerAtomView view = container.getContiguousAtomView();
PositionSpan positions = view.positions();
```

### Processor Survival

**Problem:** `Composite::apply()` cannot yield Atoms post-H4

**Solution:** Handle-yielding iterators (already built in H2c)
```cpp
// BEFORE
system.apply(processor);  // UnaryProcessor<Atom>

// AFTER  
system.applyAtoms(processor);  // UnaryProcessor<AtomHandle>
// OR
for (AtomHandle ah : system.atoms()) { ... }
```

---

## GC/Mutation Policy (Part D)

### The Critical Tension

| Aspect | GROMACS | BALLView |
|--------|---------|----------|
| Topology mutation | Never | **Frequent** |
| Compaction | Irrelevant | **Critical** |
| Common case | MD steps | Drag operations |

### Proposed Policy: Deferred Compaction

```cpp
enum CompactionPolicy {
    NEVER,        // Never compact
    ON_IDLE,      // Compact when GUI is idle (DEFAULT)
    THRESHOLD,    // Compact when free_list > 25% of live
    EXPLICIT      // Only on explicit compact()
};
```

**Guarantees:**
- `setPosition()` does NOT reallocate
- `compact()` is NOT called during drag
- Handles remain valid across mutations

**Implementation:**
```cpp
void MoleculeStore::setPosition(Index i, const Vector3& v)
{
    // Direct write - no reallocation
    positions_[i] = v;
    positions_x_[i] = v.x;
    positions_y_[i] = v.y;
    positions_z_[i] = v.z;
    has_mutation_ = true;  // Defer compaction
}

// Called from GUI idle handler
void compactIfNeeded()
{
    if (policy_ == ON_IDLE && has_mutation_) {
        compact();
        has_mutation_ = false;
    }
}
```

### Self-Attack Resolution

**Argument AGAINST deferred compaction:**
- Memory bloat: 50% fragmentation possible
- Cache inefficiency: holes in arrays

**Counter:**
- Memory is cheap (100K atoms × 376B = 37MB; 50% fragmentation = 18MB extra)
- Hybrid contiguous view mitigates cache issues
- **No better solution exists** for interactive editor

**Verdict:** ✓ **Acceptable with ON_IDLE default**

---

## Kernel Speedups (Part E)

| # | Speedup | Magnitude | Workload | GROMACS Precedent | Risk | Priority |
|---|---------|-----------|----------|-------------------|------|----------|
| 1 | SoA x/y/z + SIMD | **1.5-3×** | Energy inner loops | nbnxn SoA repack | Medium | H4.1 |
| 2 | No per-atom heap | **2-4×** | Load/clone | `t_state` | Low | H4.0 |
| 3 | Contiguous ranges | **1.2-2×** | Residue ops | `t_molblock` | Medium | H4.2 |
| 4 | CSR bonds | **1.3-2×** | Bonded terms | `t_idef` | Low | H4.1 |
| 5 | Spatial binning | **5-10×** | Nonbonded | nbnxn | High | v2.3 |

### Detailed Estimates

**E1. SoA x/y/z + SIMD:**
- Distance calculation: 4× atoms per SSE vector → **4× throughput**
- Cache: Contiguous loads → **1.5-2× better**
- **Overall: 1.5-3×** for position-heavy loops

**E2. No Per-Atom Heap:**
- 100K atoms: 376B → 24B per atom
- 10K allocations → few large allocations
- **Load time: 2-4× faster**

**E3. Contiguous Ranges:**
- Residue iteration: scattered → contiguous
- **Cache: 1.2-2× better**

**E4. CSR Bonds:**
- Bond iteration: pointer chase → contiguous array
- **Cache: 1.3-2× better**

**E5. Spatial Binning:**
- O(N²) → O(N × avg_neighbors) where avg_neighbors ~ 10-50
- **5-10× speedup** (GROMACS-proven)

---

## Adversarial Review (Part F)

### Top 5 Risks

| # | Risk | Verdict | Mitigation |
|---|------|---------|------------|
| 1 | Vector3& retirement (3,106 call sites) | **HIGH RISK** | Phased migration (H4.3) |
| 2 | Interactive mutation/GC cost | **MEDIUM** | Deferred compaction + ON_IDLE default |
| 3 | Composite/processor decoupling | **MEDIUM-HIGH** | Handle iterators (already in H2c) |
| 4 | Contiguous vs scattered tree | **MEDIUM** | Hybrid approach |
| 5 | Timing (before vs after H4) | **MEDIUM** | Staged rollout (H4, H4.1, H4.2, H4.3) |

### Detailed Attack/Defense

**Risk 1: Vector3& retirement scope**
- **Attack:** 3,106 call sites, many in templates/external code
- **Defense:** GROMACS has no per-atom references; SoA fundamentally requires value semantics
- **Resolution:** Phase 1: Deprecate non-const getPosition(). Phase 2: Migrate. Phase 3: Remove.

**Risk 2: Interactive mutation cost**
- **Attack:** Free list with 10K holes → 50% cache inefficiency
- **Defense:** Memory is cheap; deferred compaction matches user expectations
- **Resolution:** ON_IDLE policy, threshold-based auto-compaction

**Risk 3: Processor decoupling**
- **Attack:** 500+ UnaryProcessor<Composite> sites
- **Defense:** Handle iterators already exist (H2c); dual existence during transition
- **Resolution:** Migrate processors to AtomHandle; D17Composite for backward compat

**Risk 4: Contiguous vs scattered**
- **Attack:** Mid-residue insertion breaks contiguity
- **Defense:** GROMACS doesn't have this problem (immutable topology)
- **Resolution:** Hybrid: scattered storage + contiguous view for kernels

**Risk 5: Timing**
- **Attack:** H4 is already massive; adding SoA increases risk
- **Defense:** SoA columns already exist; x/y/z split is incremental
- **Resolution:** H4 (flip) → H4.1 (columns) → H4.2 (kernels) → H4.3 (retirement)

---

## Implementation Roadmap

### Phase H4 (Current - Atom-as-Handle Flip)
- [ ] Delete Composite/PropertyManager/Selectable inheritance from Atom
- [ ] MoleculeStore as sole source of truth
- [ ] Side tables for all former inline state
- [x] **Do NOT add SoA x/y/z split yet**

### Phase H4.1 (Immediate Post-H4 - SoA Preparation)
- [ ] Add `positions_x_`, `positions_y_`, `positions_z_` columns
- [ ] Add CSR bond iteration as primary path
- [ ] Keep `positions_` for backward compatibility
- [ ] Add bulk span accessors

### Phase H4.2 (Kernel Migration)
- [ ] Update ENERGY/ kernels to use SoA spans
- [ ] Update MOLMEC/ kernels
- [ ] Update STRUCTURE/ and GEOMETRY/
- [ ] **Profile before/after**

### Phase H4.3 (Breaking Change - Vector3& Retirement)
- [ ] Deprecate non-const `getPosition()`
- [ ] Migrate all 3,106 call sites
- [ ] Remove non-const `getPosition()` (ABI break)
- [ ] Requires v2.3 or later

### Phase v2.3 (Optional - Spatial Binning)
- [ ] nbnxn-style pairlists
- [ ] Cell lists for nonbonded
- [ ] High risk, high reward

---

## Conclusion

**Verdict: PROCEED with staged implementation.**

✓ **GROMACS precedent validates** the SoA + separated topology + pure handles approach
✓ **BALL infrastructure is 80% ready** (MoleculeStore, AtomHandle, container table, CSR)
✓ **Critical tension (interactive mutation) is resolvable** with deferred compaction
✓ **Performance wins justify the cost** (2-10× for various workloads)
⚠ **Biggest risk:** Vector3& retirement touches 3,106 sites — must be phased post-H4

**The design is concrete, defensible, and ready for implementation.**
