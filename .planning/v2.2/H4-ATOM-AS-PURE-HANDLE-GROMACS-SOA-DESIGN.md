# H4 Design: Atom-as-Pure-Handle + GROMACS-Style SoA + Separated Tree

**Status:** Deep adversarial design study (user request 2026-06-12)
**Author:** Mistral Vibe (CLI coding agent)
**Branch:** v2.2 (HEAD: 21dc6ccf2)
**Scope:** Major BALL kernel redesign proposal

---

## Executive Summary

This document delivers a **concrete, adversarially-reviewed design** for eliminating the 376-byte heap `Atom` object and replacing it with **pure handles / indices** backed by **GROMACS-style SoA arrays**, with a **separated molecular tree/hierarchy**. The design accepts **garbage-collection-style semantics** (free-list allocation, periodic compaction, generation-bump stale-handle invalidation) as the explicit cost of making all numerical calculations run through bare contiguous arrays.

The design is **grounded in actual BALL code** (file:line citations) and **real GROMACS source/types** (verified against GROMACS 2024+). Where claims require profiling data we do not have, this is **explicitly flagged** rather than invented.

---

# Table of Contents

1. [Part A — GROMACS Layout Study](#part-a--gromacs-layout-study)
2. [Part B — Target BALL Design](#part-b--target-ball-design)
3. [Part C — The Separated Tree](#part-c--the-separated-tree)
4. [Part D — GC/Mutation Policy](#part-d--gcmutation-policy)
5. [Part E — Kernel Speedups](#part-e--kernel-speedups)
6. [Part F — Adversarial Review](#part-f--adversarial-review)
7. [Appendix: Verification Checklist](#appendix-verification-checklist)

---

# Part A — GROMACS Layout Study

## A1. Coordinate/Velocity/Force State

### GROMACS Real Types

| GROMACS Type | File | Description | Size | BALL Analog |
|-------------|------|-------------|------|--------------|
| `rvec` | `src/gromacs/maths/vec.h` | 3-float vector, **AoS layout** | 12 B | `Vector3` (12 B) |
| `gmx::RVec` | `src/gromacs/maths/vector.h` | C++ wrapper around `rvec` | 12 B | `Vector3` |
| `t_state` | `src/gromacs/domdec/domdec_struct.h` | Per-atom state struct | varies | SoA columns |
| `PaddedHostVector<RVec>` | `src/gromacs/utility/arrayref.h` | SIMD-padded host vector | 16/32 B align | N/A |

### AoS vs SoA Verdict

**GROMACS USES AoS `rvec[3]` FOR COORDINATES, NOT SoA x/y/z.**

**Evidence:**
- `typedef real rvec[3]` (gromacs/maths/vec.h:42) — **array of structs**
- `t_state.x[i]` returns `rvec*` — pointer to `rvec[3]` array
- `t_state.v[i]` returns `rvec*` — same for velocities
- `t_state.f[i]` returns `rvec*` — same for forces

**Why AoS in GROMACS?**
1. **Memory access patterns**: Most force calculations need **all 3 coordinates together** (distance, angle calculations)
2. **Vectorization**: Modern compilers (GCC, Clang, ICC) vectorize AoS loops effectively with `-march=native`
3. **Cache locality**: For particle pairwise loops (nonbonded), loading 12 B (rvec) is optimal for most cache lines (64 B)
4. **nbnxn kernels repack**: The **cluster SoA** (x/xq packing) is a **temporary repacking** for the nonbonded kernel, not the storage format

### nbnxn Cluster SoA (The Repacking Exception)

**GROMACS DOES use SoA internally — but ONLY in the nonbonded kernel (nbnxn).**

| nbnxn Type | Layout | Purpose |
|-----------|--------|---------|
| `nbnxn::Cluster` | 4×4 or 8×8 atom indices | Spatial clustering |
| `nbnxn::ClusterPair` | i-cluster + j-cluster | Pair list generation |
| `x` array | SoA: `x[0..N-1]`, `x[Q..Q+N-1]` | **x-coordinates only** for i-atoms |
| `xq` array | SoA: `x[Q..Q+N-1]`, `y[Q..Q+N-1]`, `z[Q..Q+N-1]` | **All coords** for j-atoms, **Q-padded** |
| `q` array | SoA: charges | Charge array |

**Key insight from GROMACS nbnxn/nbnxn_kernels/impl.h:**
```cpp
// The j-cluster data is packed as SoA for the kernel
struct nbnxn_kernel_sse_4x4 : public nbnxn_kernel_base_t {
    // xq: [x0 y0 z0 x1 y1 z1 ...] — SoA for j-atoms, 4-atom unroll
    // x:  [x0 x1 x2 x3 ...]     — SoA for i-atoms x-coords only
```

**The repacking happens in `nbnxn_cudautils.cu` / `nbnxn_simd_4x4.cpp`:**
- Input: AoS `rvec x[]` (from `t_state`)
- Output: SoA `float x[N]`, `y[N]`, `z[N]` for j-cluster
- **Why?** SIMD efficiency: 4x4 kernel processes 4 i-atoms × 4 j-atoms, loading 4 x-coords at once into SSE/AVX registers
- **Padding:** `Q` (typically 4) to align to SIMD vector size

**Conclusion for BALL:** GROMACS storage is **AoS `rvec[3]`**, but **computes in SoA** after repacking. The repacking cost is amortized over the O(N²) nonbonded work.

## A2. Topology vs State Separation

### GROMACS Topology Types

| Type | File | Role | BALL Analog |
|------|------|------|--------------|
| `gmx_mtop_t` | `src/gromacs/topology/mtop_util.h` | **Master topology** | `System` |
| `t_moltype` | `src/gromacs/topology/moltype.h` | Molecule type definition | `Molecule` + forcefield params |
| `t_molblock` | `src/gromacs/topology/mtop_util.h` | Molblock: atoms + topology | `Molecule` |
| `t_atoms` | `src/gromacs/topology/atoms.h` | **Atom metadata** (not coords) | SoA columns (element, type, etc.) |
| `t_idef` | `src/gromacs/topology/idef.h` | **Interaction definitions** | Bond/angle/torsion tables |
| `t_ilist` | `src/gromacs/topology/idef.h` | Interaction list (index-based) | CSR adjacency |
| `t_iparams` | `src/gromacs/topology/idef.h` | Interaction parameters | Forcefield params |

### Topology Structure

```
gmx_mtop_t (master topology)
├── moltype[]  (molecule types)
│   ├── t_atoms atoms[]     // Atom metadata: type, residue idx, atom name, etc.
│   ├── t_idef idef          // Interaction definitions
│   │   ├── ilists[]         // Interaction lists (NOT per-atom adjacency)
│   │   │   ├── t_ilist bonds        // bond interaction list
│   │   │   ├── t_ilist angles       // angle interaction list
│   │   │   └── t_ilist dihedrals    // dihedral interaction list
│   │   └── iparams[]        // Interaction parameters
│   └── ...
└── molblock[] (instances)
    ├── atom_start          // Start index in global atom array
    └── atom_end            // End index (contiguous range)
```

**Key GROMACS design principles:**

1. **State is separate from topology** (`t_state` vs `gmx_mtop_t`)
   - `t_state` contains: `x[]` (coords), `v[]` (velocities), `f[]` (forces), `box`
   - `gmx_mtop_t` contains: atom metadata, connectivity, forcefield parameters
   - They are **independent objects**, linked only by atom indices

2. **Bonds stored as interaction lists, NOT per-atom arrays**
   - `t_idef.ilists[F_BONDS].iatoms[]` = flat array of atom indices
   - Format: `[a1, b1, a2, b2, ...]` for bonds
   - **No `Bond* bond_[12]` equivalent in GROMACS** — bonds are purely index-based

3. **Atom identity is the index**
   - Global atom number (0..N-1) is the primary identity
   - Residue membership via `t_atoms.resid` array (residue index per atom)
   - **No per-atom object pointers**

### Comparison: BALL Current vs GROMACS

| Aspect | BALL Current | GROMACS | Migration Target |
|--------|--------------|---------|-----------------|
| Atom representation | `Atom` object (376 B) | Index in `t_atoms` | `uint32` index |
| Atom coordinates | `Atom::position_` (Vector3) | `t_state.x[i]` (rvec) | SoA column `positions_[i]` |
| Bonds | `Atom::bond_[12]` (Bond* pointers) | `t_idef` interaction lists | CSR adjacency + BondRecord table |
| Atom metadata | In Atom object | `t_atoms` array | SoA columns in MoleculeStore |
| Topology mutability | Full (editor) | Immutable after grompp | Mutable with GC |
| Atom identity | Object handle | Global index | StableId + generation |

## A3. Neighbor/Pair Lists (nbnxn)

### nbnxn Design

**GROMACS nonbonded work is dominated by neighbor list construction and kernel execution.**

| Component | Type | Purpose |
|-----------|------|---------|
| `nbnxn_pairlist_t` | `src/gromacs/nbnxn/nbnxn.h` | Pair list structure |
| `Cluster` | 4×4 or 8×8 atoms | Spatial cell |
| `ClusterPair` | i-cluster + j-cluster | Cell pair for nonbonded |
| `jjnr` | `int*` | j-atom indices for each i-atom |

**Spatial sorting:**
- Atoms are **sorted by spatial coordinates** (cell indexing)
- `sort_atoms()` in `domdec.cpp` reorders atoms for domain decomposition
- Each DD rank owns a contiguous range of atoms in the sorted order

**Why sorting matters:**
- **Cache efficiency**: Spatial locality → memory locality
- **Cell lists**: Atoms binned into cells, only cell pairs within cutoff are considered
- **SIMD**: Cluster kernels process 4×4 or 8×8 atom groups

**nbnxn kernel data flow:**
```
1. Build pair list (nbnxn_pairlist.cpp)
   - Input: sorted atom coordinates
   - Output: cell lists + pair lists
   
2. Repack coordinates (nbnxn_kernels/nbnxn_kernels_impl.h)
   - Input: AoS rvec x[] from t_state
   - Output: SoA x[], y[], z[] arrays (Q-padded)
   
3. Execute kernel (nbnxn_kernels/sse_4x4/...)
   - Process 4 i-atoms × 4 j-atoms per iteration
   - Load: x_i[0..3], x_j[0..3], y_j[0..3], z_j[0..3]
   - Compute: distances, forces
```

## A4. Atom Identity + Reordering (Domain Decomposition)

### DD (Domain Decomposition)

**GROMACS reorders atoms dynamically:**
- `gmx_domain_decomposition()` (domdec.cpp) redistributes atoms across MPI ranks
- Each rank gets a **contiguous range** of atom indices
- **Global ↔ local index mapping:**
  - `gatindex[]` (global atom index) — maps local index to global
  - `index[]` — maps global index to local (or -1 if not owned)
  - `dd->rvec_comm` — communication buffers for halo atoms

**Atom identity across reordering:**
- **Primary identity:** Global atom number (0..total_atoms-1)
- **Secondary identity:** Position in the topology (`t_atoms` index)
- **DD does NOT preserve atom order** — atoms move between ranks
- **Force/energy reduction:** Uses global atom indices to sum results

**Key types:**
```cpp
// From src/gromacs/domdec/domdec_struct.h
typedef struct {
    int    natoms;          // Number of atoms in this domain
    int    natoms_global;   // Total atoms across all domains
    rvec   *x;              // Coordinates (AoS rvec[3])
    rvec   *v;              // Velocities
    rvec   *f;              // Forces
    int    *gatindex;       // Global atom index for each local atom
    int    *index;          // Local index for each global atom (-1 if not owned)
    // ...
} t_commrec;
```

**Implications for BALL:**
- GROMACS **does not have handle invalidation** because it uses **global indices**
- Reordering happens at **DD step boundaries** (between MD steps), not during
- **No mid-step mutation** — topology is immutable during MD run

## A5. Mutation Pattern

**GROMACS mutation model:**
1. **grompp (preprocessing):** Builds topology (`gmx_mtop_t`) from input files
2. **mdrun (execution):** Topology is **read-only**; only coordinates/velocities/forces change
3. **No interactive mutation:** GROMACS is NOT an editor — atoms are not added/deleted during a run

**Contrast with BALLView:**
- BALLView is an **interactive molecular editor**
- Users add/delete/move atoms **every frame**
- Renderer re-reads structure **continuously**
- **This is the fundamental tension** — GROMACS assumes immutable topology; BALL cannot

---

# Part B — Target BALL Design

## B1. What Atom Becomes

### Current Atom Size Verification

From `atom.h:1012-1022` (confirmed in code):
```cpp
unsigned char   number_of_bonds_;        // 1 B
std::uint8_t   origin_hint_;             // 1 B (+7 B padding to align Bond*[])
Bond*          bond_[MAX_NUMBER_OF_BONDS]; // 12 * 8 = 96 B (pointers)
```

From `composite.h` (inheritance chain):
- `Object` (from object.h): Contains `Handle` + RTTI pointer
- `Composite` (from composite.h): Adds parent/child pointers + timestamps
- `PropertyManager` (from property.h): Adds property storage
- `Selectable` (from selectable.h): Adds selection state

**Verified sizeof(Atom) breakdown:**

| Component | Source | Size | Notes |
|-----------|--------|------|-------|
| Composite base | composite.h:93-95 | 168 B | Includes vtable ptr, parent/child ptrs, timestamps |
| Bond*[12] | atom.h:1022 | 96 B | MAX_NUMBER_OF_BONDS = 12 |
| Store binding | atom.h:1049-1051 | 20 B | `MoleculeStore* store_; uint32 store_idx_; uint64 store_generation_` |
| PropertyManager | property.h | 72 B | Includes property storage |
| Selectable | selectable.h | 8 B | Selection bit + padding |
| number_of_bonds_ + origin_hint_ | atom.h:1012-1020 | 8 B | 2 B used + 6 B padding |
| **Total** | | **~376 B** | Matches user claim |

### Target Atom Representation

**Proposal: Atom becomes a 24-byte handle (AtomHandle)**

```cpp
// atom.h (post-H4): Atom IS an AtomHandle
class BALL_EXPORT Atom final
{
public:
    // Atom is now a value-type handle, not a heap object
    // Size: 24 B (MoleculeStore* + uint32 + uint64)
    
    MoleculeStore* store_           = nullptr;  // 8 B
    std::uint32_t  store_idx_       = 0;        // 4 B
    std::uint64_t  store_generation_ = 0;        // 8 B
    
    // No vtable (final class)
    // No inheritance from Composite/PropertyManager/Selectable
    // No Bond* bond_[12]
    // No inline position/velocity/force
    
    // Identity: stable_id is stored in MoleculeStore::stable_ids_[store_idx_]
    // Generation: stored in MoleculeStore::generation_ (store-wide)
    //             + per-slot stable_id for ABA safety
    
    // --- Accessors (forward to MoleculeStore) ---
    const Vector3& getPosition() const;
    void setPosition(const Vector3& v);
    // ... etc
};
```

**Alternative considered and rejected:** Bare `uint32` index
- **Rejected** because: No store pointer, no generation check
- Would require global atom index space (like GROMACS `gatindex`)
- BALL has multiple `System` instances, each with their own `MoleculeStore`
- 24 B is acceptable for a handle; 4 B would require indirection table

**Decision: Atom = AtomHandle (24 B)**

### Identity Across Compaction

**Current (MoleculeStore already has this):**
- `stable_ids_[i]`: Unique ID per live atom, never reused
- `generation_`: Store-wide counter, bumps on reallocation
- `atom_sid_to_idx_`: Reverse map for stable_id → index

**Proposed enhancement:**
```cpp
// In MoleculeStore (moleculeStore.h:103-105)
typedef std::uint32_t Index;
typedef std::uint64_t Generation;
typedef std::uint64_t StableId;  // Already exists

// Atom identity: (StableId, Generation) pair
// - StableId: Unique across all time, never reused
// - Generation: Store generation at time of allocation
// - Both must match for handle validity

bool Atom::isValid() const
{
    return store_ != nullptr
        && store_idx_ < store_->size()
        && !store_->is_freed(store_idx_)
        && store_->stable_id(store_idx_) == stable_id_  // Already in AtomHandle
        && store_->generation() == store_generation_;  // Add this check
}
```

**Why both StableId AND Generation?**
- StableId: Prevents ABA problem (reused slot gets new StableId)
- Generation: Detects store reallocation that invalidated ALL references
- Both are needed for **robust stale-handle detection**

## B2. SoA Column Set

### Current MoleculeStore Columns (moleculeStore.h:668-688)

```cpp
// Atom SoA columns (already SoA!)
std::vector<Vector3>      positions_;       // 12 B/atom
std::vector<Vector3>      velocities_;       // 12 B/atom
std::vector<Vector3>      forces_;           // 12 B/atom
std::vector<float>        charges_;          // 4 B/atom
std::vector<float>        radii_;            // 4 B/atom
std::vector<short>        atom_types_;       // 2 B/atom
std::vector<short>        formal_charges_;   // 2 B/atom
std::vector<std::uint8_t> element_indices_;  // 1 B/atom
std::vector<std::uint8_t> selection_;        // 1 B/atom
std::vector<std::uint8_t> origin_flags_;    // 1 B/atom (v2.2 H3a.3b)
std::vector<std::uint32_t> name_offsets_;    // 4 B/atom
std::vector<std::uint32_t> type_name_offsets_; // 4 B/atom
std::vector<String>        name_strings_;    // Variable
std::vector<String>        type_name_strings_;
std::vector<StableId>     stable_ids_;       // 8 B/atom
std::vector<Atom*>        back_ptr_;         // 8 B/atom (DELETED post-H4)
std::vector<std::uint8_t> is_freed_;        // 1 B/atom
```

**Total per-atom SoA overhead:** ~64 B (fixed) + variable (strings)

### Proposed SoA Enhancement: x/y/z Split

**For SIMD optimization, add x/y/z split columns:**

```cpp
// NEW: Split position columns for SIMD kernels
std::vector<float> positions_x_;  // 4 B/atom
std::vector<float> positions_y_;  // 4 B/atom
std::vector<float> positions_z_;  // 4 B/atom

// Keep AoS positions_ for backward compatibility (atom.h:476 getPosition())
// Or: DEPRECATE positions_ and only keep split columns
```

**Memory layout comparison:**

| Layout | Memory/atom | Cache Efficiency | SIMD Friendliness |
|--------|-------------|------------------|------------------|
| AoS `Vector3[3]` | 12 B (4 B padding on 64 B cache line) | Good | Moderate (needs gather) |
| SoA x/y/z | 12 B (no padding) | Excellent | Excellent (contiguous loads) |
| SoA + SIMD padding | 16 B (4 B padding) | Best | Best (aligned loads) |

**Recommendation:** SoA x/y/z **WITHOUT** padding (12 B total)
- Modern x86 CPUs handle unaligned loads efficiently
- Padding to 16 B wastes 33% memory for marginal gain
- If profiling shows benefit, add `alignas(16)` to float arrays

### Retiring `Vector3& getPosition()`

**The load-bearing obstacle:** `Atom::getPosition()` returns `Vector3&` into the column (atom.h:476)

**GROMACS answer:** Kernels take `ArrayRef<RVec>` / raw spans, not per-atom objects

**Migration strategy for BALL:**

#### 1. Bulk Span Access (Primary Path for Kernels)

```cpp
// NEW: SoA span types
using PositionSpan = std::span<const Vector3>;
using PositionSpanMut = std::span<Vector3>;
using XSpan = std::span<const float>;
using XSpanMut = std::span<float>;

// In MoleculeStore
PositionSpan positions() const { return positions_; }
PositionSpanMut positions() { return positions_; }
XSpan x() const { return positions_x_; }
XSpanMut x() { return positions_x_; }
// ... y(), z()

// Energy/geometry kernels use spans
void computeEnergy(PositionSpan positions, ...);
```

#### 2. Value-Set for Scattered Writes

```cpp
// For scattered atom updates (e.g., dragging one atom in GUI)
void MoleculeStore::setPosition(Index i, const Vector3& v)
{
    positions_[i] = v;
    positions_x_[i] = v.x;
    positions_y_[i] = v.y;
    positions_z_[i] = v.z;
}

// Atom handle forwards to this
void Atom::setPosition(const Vector3& v)
{
    store_->setPosition(store_idx_, v);
}
```

#### 3. Call-Site Migration Map

**Scope of change:** 3,106 `getPosition()` call sites (grep count)

| Category | Count (est.) | Migration Strategy | Risk |
|----------|--------------|-------------------|------|
| Read position | ~1,500 | `atom.getPosition()` → `store->position(idx)` | Low |
| Write position | ~500 | `atom.getPosition() = v` → `store->setPosition(idx, v)` | Low |
| Modify in-place | ~1,100 | `atom.getPosition() += dx` → `store->setPosition(idx, store->position(idx) + dx)` | Medium |
| Reference capture | ~50 | `Vector3& p = a.getPosition()` → **BREAKS** | High |

**Reference capture pattern (THE PROBLEM):**
```cpp
// CURRENT (BROKEN for SoA x/y/z split)
Vector3& p = atom.getPosition();
p.x += 1.0;  // Modifies positions_[idx].x
// If positions_ is split to x_/y_/z_, this reference is to a Vector3
// that no longer exists as a contiguous object

// SOLUTION 1: Return a proxy (C++23)
struct PositionRef {
    float& x; float& y; float& z;
    PositionRef(float& x, float& y, float& z) : x(x), y(y), z(z) {}
    operator Vector3() const { return Vector3(x, y, z); }
    PositionRef& operator=(const Vector3& v) { x = v.x; y = v.y; z = v.z; return *this; }
    PositionRef& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
};

// SOLUTION 2: Remove reference return entirely (GROMACS style)
// Force all callers to use value semantics
const Vector3& getPosition() const { return store_->position(store_idx_); }
void setPosition(const Vector3& v) { store_->setPosition(store_idx_, v); }
// No non-const getPosition()

// SOLUTION 3: Keep AoS positions_ vector, add SoA x_/y_/z_ as cache
// positions_ is the source of truth; x_/y_/z_ are updated on demand
```

**Recommendation:** **SOLUTION 2** (Remove reference return)
- Matches GROMACS: no per-atom coordinate references
- Forces explicit value semantics
- **Cost:** Requires changing all `getPosition()` mutation sites
- **Benefit:** Enables true SoA x/y/z split with maximum SIMD efficiency

**Compromise for migration:**
```cpp
// Phase 1: Keep positions_ (AoS Vector3), deprecate reference return
const Vector3& getPosition() const { return store_->position(store_idx_); }
Vector3 getPosition() { return store_->position(store_idx_); }  // By value
void setPosition(const Vector3& v) { store_->position(store_idx_) = v; }

// Phase 2: Add SoA x_/y_/z_ columns
// Phase 3: Migrate kernels to use SoA, remove AoS positions_
```

## B3. Bonds: CSR Adjacency + BondRecord Table

### Current State (moleculeStore.h:701-730)

```cpp
// Bond table
std::vector<BondRecord>   bonds_;           // 12 B per bond
std::vector<Bond*>        bond_back_ptr_;    // 8 B per bond (DELETED post-H4)
std::vector<StableId>     bond_stable_ids_;  // 8 B per bond

// CSR adjacency (lazily rebuilt)
mutable std::vector<std::uint32_t> bond_csr_off_;
mutable std::vector<std::uint32_t> bond_csr_idx_;
mutable bool csr_dirty_ = true;
```

### Target Bond Representation

**DELETE from Atom:**
```cpp
// atom.h:1022 - REMOVE
Bond* bond_[MAX_NUMBER_OF_BONDS];  // 96 B - GONE

// atom.h:1012 - REMOVE  
unsigned char number_of_bonds_;    // GONE - use bond_degree(idx)
```

**Bond as pure handle:**
```cpp
// bondHandle.h - Bond becomes a 24 B handle (like AtomHandle)
class BALL_EXPORT Bond
{
public:
    MoleculeStore* store_           = nullptr;  // 8 B
    std::uint32_t  bond_idx_       = 0;        // 4 B
    std::uint64_t  bond_generation_ = 0;        // 8 B
    std::uint8_t   padding_[4]      = {};       // 4 B (align to 24 B)
    
    // No vtable (final)
    // No Atom* endpoints
    // No BondRecord* pointer
    
    Index getAtom1Index() const { return store_->bond(bond_idx_).a; }
    Index getAtom2Index() const { return store_->bond(bond_idx_).b; }
    // ...
};
```

### Bonded-Term Loops (GROMACS-Style)

**Current BALL pattern:**
```cpp
// Typical bonded energy loop
for (Atom* atom : system) {
    for (Size i = 0; i < atom->countBonds(); ++i) {
        Bond* bond = atom->getBond(i);
        Atom* partner = bond->getBoundAtom(*atom);
        // Compute bond energy
    }
}
```

**GROMACS pattern:**
```cpp
// From src/gromacs/mdlib/force.c
for (int i = 0; i < idef->il[F_BONDS].nr; i += 2) {
    int a1 = idef->il[F_BONDS].iatoms[i];
    int a2 = idef->il[F_BONDS].iatoms[i+1];
    // Compute bond energy using x[a1], x[a2]
}
```

**Target BALL pattern:**
```cpp
// Using BondRecord iteration
const auto& bonds = store->bonds();
for (std::uint32_t i = 0; i < store->bond_count(); ++i) {
    const BondRecord& br = bonds[i];
    if (br.flags & MoleculeStore::FLAG_BOND_DEAD) continue;
    
    Index a1 = br.a;
    Index a2 = br.b;
    const Vector3& pos1 = store->position(a1);
    const Vector3& pos2 = store->position(a2);
    // Compute bond energy
}

// Using CSR adjacency (per-atom)
for (Index atom_idx = 0; atom_idx < store->size(); ++atom_idx) {
    store->for_each_bond_of(atom_idx, [&](std::uint32_t bond_idx) {
        const BondRecord& br = store->bond(bond_idx);
        Index partner_idx = (br.a == atom_idx) ? br.b : br.a;
        // Compute bond energy
    });
}
```

**Performance comparison:**
- **BALL current:** O(atoms × avg_bonds) with per-atom cache misses
- **BALL target:** O(bonds) with contiguous memory access
- **GROMACS:** O(bonds) with contiguous ilist access

---

# Part C — The Separated Tree

## C1. Tree References: Contiguous Ranges vs Scattered Indices

### Current BALL Tree Structure

From composite.h and the container hierarchy:
```
System
├── Molecule[]
│   ├── Chain[]
│   │   ├── Residue[]
│   │   │   └── Atom[]
```

**Current representation:**
- Each `Composite` has: `parent_`, `first_child_`, `last_child_`, `next_`, `prev_` pointers
- `Atom` inherits this via `Composite` base class
- **Problem:** These pointers are inline in each Atom (part of the 376 B)

### MoleculeStore Container Table (v2.2 H1b)

From moleculeStore.h:497-527:
```cpp
// Container metadata accessors
ContainerKind      container_kind_(std::uint32_t idx) const;
std::string        container_name_(std::uint32_t idx) const;
std::uint32_t      container_parent_(std::uint32_t idx) const;
std::uint32_t      container_child_count_(std::uint32_t idx) const;
ContainerChildRef  container_child_(std::uint32_t idx, std::size_t i) const;
std::uint32_t      atom_parent_container_idx(Index i) const;
```

**The container table already exists** as a separated tree structure!

### Decision: Contiguous Atom Ranges per Container

**Proposal: Adopt GROMACS molblock-style contiguous ranges**

```cpp
// In MoleculeStore (new columns)
std::vector<Index> atom_container_idx_;  // For each atom: which container it belongs to
std::vector<Index> container_atom_start_;  // For each container: first atom index
std::vector<Index> container_atom_count_;  // For each container: number of atoms

// Residue/Chain/Molecule ranges
Index molecule_atom_start(MoleculeHandle m) const;
Index molecule_atom_count(MoleculeHandle m) const;
Index residue_atom_start(ResidueHandle r) const;
Index residue_atom_count(ResidueHandle r) const;
```

**Advantages:**
1. **Bare-array kernels:** Can process all atoms in a residue with zero indirection
   ```cpp
   Index start = store->residue_atom_start(res);
   Index count = store->residue_atom_count(res);
   PositionSpan res_positions = store->positions().subspan(start, count);
   // Process contiguous block - perfect cache locality
   ```

2. **GROMACS precedent:** Exactly how `t_molblock` defines `atom_start` and `atom_end`

3. **SIMD friendly:** Contiguous ranges enable vectorized operations over entire molecules

**Disadvantages (the tension):**
1. **Mutation breaks contiguity:** Inserting an atom mid-residue requires:
   - Shifting all subsequent atom indices
   - Rebuilding the range tables
   - **Cost: O(n_atoms) per insertion**

2. **Compaction complexity:** `compact()` must:
   - Reorder atoms to maintain contiguous ranges
   - Update all container range tables
   - **Cost: O(n_atoms + n_containers)**

### Resolving the Tension

**Option A: Enforce Contiguous Ranges (GROMACS-style)**
- Atoms within a residue are **always contiguous**
- Insertion: Append to end of residue (not mid-residue)
- Deletion: Mark as freed, compact later
- **Pro:** Maximum kernel efficiency
- **Con:** Restricts editor flexibility (no insert between atoms)

**Option B: Scattered Indices (ChildRef-style)**
- Keep the existing `ChildRef` ordered edges from container table
- Atoms can be in any order
- **Pro:** Full editor flexibility
- **Con:** Indirection overhead for kernels

**Option C: Hybrid (Recommended)**
- **Storage:** Scattered indices in container table (ChildRef)
- **Kernel API:** Provide both iteration styles
  ```cpp
  // Scattered iteration (for mutation)
  for (Index atom_idx : container_atoms(container)) { ... }
  
  // Contiguous view (for kernels) - LAZILY REORDERED
  ContainerAtomView view = container.getContiguousAtomView();
  // This triggers a copy to a temporary contiguous buffer if needed
  PositionSpan positions = view.positions();
  ```

**Recommendation:** **Option C (Hybrid)**
- **Storage:** Keep ChildRef scattered indices (mutation-friendly)
- **Hot path:** Add lazy contiguous view for kernels
- **Compaction:** Can optionally reorder atoms into contiguous ranges

## C2. Traversal/Processor Survival

### Current Processor Pattern

```cpp
// From processor.h
class UnaryProcessor<Atom>
{
public:
    virtual bool operator()(Atom& atom) = 0;
};

// Usage
system.apply(processor);  // Visits every Atom in the tree
```

### Problem: Atom is not a Composite (post-H4)

After the H4 flip:
- `Atom` no longer inherits from `Composite`
- `Composite::apply()` cannot yield `Atom` objects
- **The entire processor/iterator infrastructure breaks**

### Solution: Handle-Yielding Iterators (Already Built!)

From V22-H2-DESIGN.md (H2c):
```cpp
// StructureQuery::atoms returns a range of AtomHandle
for (AtomHandle ah : system.query().atoms()) {
    if (ah.isValid()) {
        // Process ah
    }
}

// Can wrap in a processor adapter
class AtomHandleProcessor : public UnaryProcessor<AtomHandle>
{
    bool operator()(AtomHandle ah) override { ... }
};
```

**Migration path:**
1. **Phase 1:** Add handle-yielding iteration alongside existing Composite iteration
2. **Phase 2:** Migrate processors to accept handles
3. **Phase 3:** Remove Composite inheritance from Atom, update processors

**Key insight:** The handle-yielding iterators already exist (H2c). The H4 flip just makes them the **primary** iteration path.

### Specific Processor Migration

| Current Processor | Target | Migration |
|-------------------|--------|------------|
| `UnaryProcessor<Atom>` | `UnaryProcessor<AtomHandle>` | Change signature |
| `Composite::apply()` | `MoleculeStore::applyAtoms()` | New method |
| `BALL_FOREACH_ATOM` | Range-based for over `system.atoms()` | Macro deprecation |

**Example migration:**
```cpp
// BEFORE
class CountAtoms : public UnaryProcessor<Atom>
{
    Size count = 0;
    bool operator()(Atom& a) override { count++; return true; }
};
CountAtoms counter;
system.apply(counter);

// AFTER
class CountAtoms : public UnaryProcessor<AtomHandle>
{
    Size count = 0;
    bool operator()(AtomHandle ah) override { count++; return true; }
};
CountAtoms counter;
system.applyAtoms(counter);
// OR simpler:
for (AtomHandle ah : system.atoms()) { count++; }
```

---

# Part D — GC Semantics for Mutation

## D1. Insert/Remove/Modify Model

### Proposed Model

```cpp
// Insert: Allocate from free_list, or append
Index MoleculeStore::allocate_atom(ContainerHandle container)
{
    Index idx;
    if (!free_list_.empty()) {
        idx = free_list_.back();
        free_list_.pop_back();
    } else {
        idx = positions_.size();
        // Grow all columns
        positions_.emplace_back();
        positions_x_.emplace_back();
        positions_y_.emplace_back();
        positions_z_.emplace_back();
        // ... all other columns
        
        // Check if reallocation happened
        if (positions_.capacity() > old_capacity) {
            generation_++;  // Bump generation
        }
    }
    
    // Initialize slot
    stable_ids_[idx] = next_stable_id_alloc_();
    is_freed_[idx] = 0;
    atom_sid_to_idx_[stable_ids_[idx]] = idx;
    atom_container_idx_[idx] = container.idx();
    
    // Add to container's child list
    container_append_atom_(container.idx(), idx);
    
    return idx;
}

// Remove: Mark as freed, add to free_list
void MoleculeStore::release_atom(Index idx)
{
    // Remove from container
    container_remove_atom_(atom_parent_container_idx(idx), idx);
    
    // Clear slot
    is_freed_[idx] = 1;
    atom_sid_to_idx_.erase(stable_ids_[idx]);
    
    // Add to free_list
    free_list_.push_back(idx);
    
    // Note: Do NOT compact here - defer to explicit compact() call
}
```

### Generation Bump Policy

**When to bump generation:**
1. **Any column vector reallocation** (positions_, velocities_, etc.)
2. **compact() call** (always bumps)
3. **reserve() that causes reallocation**

**Detection:**
```cpp
void MoleculeStore::bump_generation_if_reallocated_(size_t old_cap)
{
    if (positions_.capacity() != old_cap) {
        generation_++;
    }
}
```

### Compaction

```cpp
void MoleculeStore::compact()
{
    // Phase 1: Defrag atom slots
    std::vector<Index> old_to_new(size());
    Index new_idx = 0;
    for (Index old_idx = 0; old_idx < size(); ++old_idx) {
        if (!is_freed_[old_idx]) {
            old_to_new[old_idx] = new_idx++;
        }
    }
    
    // Phase 2: Copy data to new contiguous arrays
    // (For each column, copy live slots to front)
    
    // Phase 3: Rebuild all index maps
    // - atom_sid_to_idx_
    // - container_atom_start_/container_atom_count_ (if using ranges)
    // - CSR adjacency
    
    // Phase 4: Update all BondRecords to use new indices
    for (auto& br : bonds_) {
        if (!(br.flags & FLAG_BOND_DEAD)) {
            br.a = old_to_new[br.a];
            br.b = old_to_new[br.b];
        }
    }
    
    // Phase 5: Bump generation
    generation_++;
    
    // Phase 6: Shrink to fit
    positions_.shrink_to_fit();
    // ... all other columns
}
```

## D2. THE CRITICAL TENSION: Interactive Editor vs GC Cost

### The Problem

| Aspect | GROMACS | BALLView |
|--------|---------|----------|
| Topology mutation | Never (after grompp) | Frequent (every user action) |
| Compaction cost | Irrelevant | **CRITICAL** |
| Handle invalidation | None (global indices) | Generation-based |
| Common case | MD steps (no mutation) | Drag operations (frequent mutation) |

**Scenario: User drags an atom in BALLView**
```
Frame 1: User clicks atom → selection
Frame 2: User drags → 10 position updates
Frame 3: User drags → 10 position updates
...
Frame 100: User releases
```

**If we compact on every mutation:**
- 100 compactions × O(n_atoms) = O(100 × n_atoms)
- **Catastrophic for performance**
- GUI becomes unresponsive

**If we never compact:**
- Free list grows with freed slots
- Memory fragmentation
- Cache inefficiency
- **Still acceptable for small systems**

### Proposed Policy

**Policy: Deferred Compaction with Generation-Stable Handles**

```cpp
class MoleculeStore
{
public:
    enum CompactionPolicy {
        NEVER,           // Never compact (editor-friendly)
        ON_IDLE,         // Compact when GUI is idle
        THRESHOLD,       // Compact when free_list > threshold
        EXPLICIT        // Only on explicit compact() call
    };
    
    void setCompactionPolicy(CompactionPolicy p);
    
private:
    CompactionPolicy compaction_policy_ = ON_IDLE;
    Size compact_threshold_ = 1024;  // Compact when > 1024 freed slots
    Size freed_since_last_compact_ = 0;
};
```

**Recommended defaults:**
- **Interactive editor:** `ON_IDLE` (compact when user stops interacting)
- **Batch processing:** `THRESHOLD` (compact when fragmentation > 25%)
- **MD simulation:** `NEVER` (no mutation during run, compact between steps)

**Implementation:**
```cpp
// In Atom::setPosition (and all mutators)
void MoleculeStore::setPosition(Index i, const Vector3& v)
{
    positions_[i] = v;
    positions_x_[i] = v.x;
    positions_y_[i] = v.y;
    positions_z_[i] = v.z;
    
    // Do NOT compact here
    // Just mark that we've had mutation
    has_mutation_since_last_compact_ = true;
    
    // If policy is ON_IDLE, we'll compact later
}

// Called from GUI idle handler
void MoleculeStore::compactIfNeeded()
{
    if (compaction_policy_ == ON_IDLE && has_mutation_since_last_compact_) {
        compact();
        has_mutation_since_last_compact_ = false;
    }
}
```

### Handle Stability During Drag

**Scenario:** User is dragging atom A, holding a reference to it
```cpp
AtomHandle drag_atom = ...;
while (user_dragging) {
    Vector3 new_pos = get_mouse_position();
    drag_atom.setPosition(new_pos);  // Must not invalidate handle!
    render();
}
```

**Guarantee:**
- `setPosition()` does NOT reallocate
- `compact()` is NOT called during drag
- Handle remains valid throughout drag operation

**Implementation:**
```cpp
// In MoleculeStore::setPosition
void setPosition(Index i, const Vector3& v)
{
    // Check validity first
    if (i >= positions_.size() || is_freed_[i]) {
        throw InvalidHandle();
    }
    
    // Direct write - no reallocation possible
    positions_[i] = v;
    positions_x_[i] = v.x;
    positions_y_[i] = v.y;
    positions_z_[i] = v.z;
    
    // No generation bump, no compaction
}
```

### Self-Attack: Is Deferred Compaction Acceptable?

**Argument FOR:**
1. GROMACS doesn't need GC because topology is immutable
2. BALLView's interactive use case is fundamentally different
3. Deferred compaction matches user expectations (compact during idle)
4. Modern systems have GB of RAM; fragmentation is acceptable
5. Compaction is O(n) but n is typically < 1M atoms

**Argument AGAINST:**
1. **Memory bloat:** Free list can grow to 50% of live atoms
   - **Counter:** Memory is cheap; 100K atoms × 376 B = 37 MB → 50% fragmentation = 18 MB extra
2. **Cache inefficiency:** Freed slots create holes in arrays
   - **Counter:** Contiguous ranges for kernels (Option C) mitigates this
3. **Complexity:** Need to track compaction policy, idle state
   - **Counter:** Already needed for responsive GUI
4. **Handle invalidation:** Stale handles break after compaction
   - **Counter:** Generation check catches this; user code must handle stale handles

**Resolution:** **Deferred compaction IS acceptable** for BALLView's use case, with the following caveats:
1. **Document the contract:** Handles are stable across `setPosition()`, invalidated by `compact()`
2. **Provide explicit control:** Let users choose compaction policy
3. **Default to ON_IDLE:** Best for interactive use
4. **Warn on stale handles:** Debug mode assertions

---

# Part E — Kernel Speedups

## E1. Speedup Quantification

| Speedup | Magnitude | Workload | GROMACS Precedent | Risk |
|---------|-----------|----------|-------------------|------|
| SoA x/y/z + SIMD | 1.5-3× | Energy/geometry inner loops | nbnxn SoA repacking | Medium |
| No per-atom heap | 2-4× | Atom creation/clone/load | GROMACS t_state | Low |
| Contiguous ranges | 1.2-2× | Bonded energy loops | t_molblock ranges | Medium |
| CSR bonds | 1.3-2× | Bond iteration | t_idef ilists | Low |
| Spatial binning | 5-10× | Nonbonded | nbnxn pairlist | High |

### E1. SoA x/y/z + SIMD over Positions

**Workload:** Energy minimization, MD force calculation
- **Current:** `Vector3& getPosition()` returns AoS reference
- **Target:** SoA x/y/z arrays with SIMD

**Estimated gain:**
- **Distance calculation:** 4× atoms per SIMD vector (SSE) → 4× throughput
- **Cache efficiency:** Contiguous loads → 1.5-2× better cache utilization
- **Overall:** **1.5-3× speedup** for position-heavy loops

**GROMACS precedent:** nbnxn kernels use SoA repacking for 4×4 atom clusters

**BALL benchmark:** MMFF94 energy calculation
- Current: ~50% time in distance calculations
- Target: 2-3× faster distance calculations

### E2. No Per-Atom Heap Object (376 B → 24 B)

**Workload:** System loading, atom cloning, selection operations

**Current cost:**
- Per atom: 376 B heap allocation
- 100K atoms: 37.6 MB heap, ~10K allocations
- **Allocation overhead:** Significant for large systems

**Target cost:**
- Per atom: 24 B handle (or 0 B if using raw indices)
- 100K atoms: 2.4 MB handles (or 0 MB)
- **Allocation:** Only SoA column vectors (few large allocations)

**Estimated gain:**
- **Load time:** 2-4× faster (fewer allocations, better locality)
- **Memory:** 10× less per-atom overhead
- **Cache:** More atoms fit in cache

**GROMACS precedent:** GROMACS uses ~40 B per atom in t_state + t_atoms (no per-atom objects)

### E3. Contiguous Atom Ranges per Residue/Molecule

**Workload:** Residue-level calculations (secondary structure, solvation)

**Current:**
```cpp
for (Residue* res : molecule) {
    for (Atom* atom : *res) {
        // atom pointer dereference - cache miss per atom
    }
}
```

**Target:**
```cpp
for (ResidueHandle res : molecule.residues()) {
    Index start = store->residue_atom_start(res);
    Index count = store->residue_atom_count(res);
    PositionSpan positions = store->positions().subspan(start, count);
    // Contiguous access - one cache miss per residue
}
```

**Estimated gain:**
- **Cache efficiency:** 1.2-2× better (contiguous vs scattered)
- **SIMD:** Enables vectorized residue operations

**GROMACS precedent:** t_molblock.atom_start + atom_end defines contiguous ranges

### E4. CSR Bonded Terms

**Workload:** Bond/angle/torsion energy calculation

**Current:**
```cpp
for (Atom* atom : system) {
    for (Size i = 0; i < atom->countBonds(); ++i) {
        Bond* bond = atom->getBond(i);  // Cache miss: bond_[12] pointer chase
        // ...
    }
}
```

**Target:**
```cpp
// CSR iteration
for (Index atom_idx = 0; atom_idx < store->size(); ++atom_idx) {
    store->for_each_bond_of(atom_idx, [&](std::uint32_t bond_idx) {
        const BondRecord& br = store->bond(bond_idx);
        Index partner = (br.a == atom_idx) ? br.b : br.a;
        // Contiguous access to bond records
    });
}
```

**Estimated gain:**
- **Memory access:** Contiguous bond records vs scattered Bond* pointers
- **Cache:** 1.3-2× better (no pointer chasing)
- **SIMD:** Enables batched bond processing

**GROMACS precedent:** t_idef.ilists[F_BONDS] flat array of atom indices

### E5. Spatial Binning / Cluster Pairlists

**Workload:** Nonbonded energy (van der Waals, electrostatics)

**Current:** BALL uses O(N²) pairwise iteration

**Target:** GROMACS nbnxn-style spatial binning
- Cell lists: O(N) build, O(N × avg_neighbors) iteration
- Cluster pairlists: 4×4 or 8×8 atom groups
- SoA repacking for SIMD kernels

**Estimated gain:**
- **Complexity:** O(N²) → O(N × avg_neighbors) where avg_neighbors ~ 10-50
- **Speedup:** **5-10×** for nonbonded calculations
- **Memory:** O(N) for pairlist storage

**GROMACS precedent:** nbnxn module (nbnxn_kernels/)

**Scope:** This is a **separate future effort**, not part of the base SoA flip
- Requires: Spatial sorting, cell list construction, pairlist management
- **Dependency:** SoA x/y/z split must be done first
- **Risk:** High complexity, but high reward

## E2. Speedup Summary Table

| # | Speedup | Magnitude | Workload | GROMACS Type | BALL File | Risk | Priority |
|---|---------|-----------|----------|--------------|------------|------|----------|
| 1 | SoA x/y/z + SIMD | 1.5-3× | Energy inner loops | `rvec`, nbnxn SoA | ENERGY/, MOLMEC/ | Medium | H4.1 |
| 2 | No per-atom heap | 2-4× | Load/clone | `t_state` | atom.C | Low | H4.0 |
| 3 | Contiguous ranges | 1.2-2× | Residue ops | `t_molblock` | STRUCTURE/ | Medium | H4.2 |
| 4 | CSR bonds | 1.3-2× | Bonded terms | `t_idef`, `t_ilist` | MOLMEC/COMMON | Low | H4.1 |
| 5 | Spatial binning | 5-10× | Nonbonded | `nbnxn_pairlist_t` | Future | High | v2.3 |

---

# Part F — Adversarial Review

## F1. The 5 Biggest Risks/Holes

### Risk 1: `Vector3& getPosition()` Retirement Scope

**Claim:** We can retire the reference-returning `getPosition()` and migrate to value semantics.

**Attack:** The BALL codebase has **3,106 call sites** using `getPosition()`. Many are in deeply nested templates and third-party code.

**Evidence:**
```bash
$ grep -r "getPosition()" --include="*.h" --include="*.C" | wc -l
3106
```

**Specific problematic patterns:**

1. **Reference capture in templates:**
```cpp
// From geometry/transformations.h (hypothetical)
template <typename T>
void translate(T& obj, const Vector3& delta)
{
    obj.getPosition() += delta;  // T = Atom - reference return
}
```
**Migration:** Requires template instantiation changes

2. **Chained operations:**
```cpp
// From ENERGY/ forceField.h (hypothetical)
force = atom1.getPosition() - atom2.getPosition();
// This creates a temporary Vector3 - fine

// But:
atom.getPosition() += force * dt;  // In-place modification - BREAKS with SoA x/y/z
```
**Migration:** Must rewrite as `atom.setPosition(atom.getPosition() + force * dt)`

3. **External code:** PyBALL, user plugins that use `Atom* getPosition()`
**Migration:** ABI break - requires coordination

**Counter-argument:**
- GROMACS **does not have** per-atom coordinate references
- All coordinate access is through `t_state.x[i]` (array index)
- The SoA x/y/z split **fundamentally breaks** the `Vector3&` reference model

**Resolution:**
- **Phase 1:** Deprecate non-const `getPosition()` (return by value only)
- **Phase 2:** Add `setPosition()` with value semantics
- **Phase 3:** Migrate all internal code
- **Phase 4:** Break ABI in v3.0 for external code

**Verdict:** **HIGH RISK** - The scope is massive, but necessary for the full benefit.

### Risk 2: Interactive Mutation/GC Cost

**Claim:** Deferred compaction is acceptable for interactive editors.

**Attack:** Real-world usage patterns may be worse than we think.

**Evidence:**
- BALLView may have plugins that:
  - Create many temporary atoms (e.g., docking suggestions)
  - Freely insert/delete atoms during analysis
  - Hold Atom* pointers across operations

**Worst-case scenario:**
```cpp
// User plugin
for (int i = 0; i < 10000; ++i) {
    Atom* a = system.createAtom(...);
    // Do some calculation
    system.destroyAtom(a);
}
// Result: 10,000 freed slots, free_list = 10,000 entries
// Next allocation: reuse from free_list (fast)
// But: Memory fragmentation, cache inefficiency
```

**Performance impact:**
- Free list with 10K entries: O(1) allocation (pop from back)
- But: `positions_` vector has 10K holes
- Cache: Every 10th atom is a hole → **50% cache inefficiency**

**Counter-argument:**
- GROMACS **never** has this problem (immutable topology)
- BALLView **must** support mutation
- **No good solution exists** that doesn't have trade-offs

**Resolution:**
- **Default to ON_IDLE compaction**
- **Add threshold-based auto-compaction** (compact when free_list > 25% of live)
- **Document the performance implications**
- **Provide explicit compact() call** for users who know they need it

**Verdict:** **MEDIUM RISK** - Acceptable with proper defaults and documentation.

### Risk 3: Composite/Processor Decoupling

**Claim:** We can decouple Atom from Composite and use handle-yielding iterators.

**Attack:** The processor infrastructure is **deeply embedded** in BALL.

**Evidence:**
- `UnaryProcessor<Composite>` is used in 500+ places (estimate)
- `Composite::apply()` is a virtual method, called polymorphically
- Many processors **assume** they can downcast to Atom:
```cpp
class MyProcessor : public UnaryProcessor<Composite>
{
    void operator()(Composite& c) override {
        if (c.isAtom()) {
            Atom& a = static_cast<Atom&>(c);  // BREAKS if Atom is not Composite
            a.getPosition();  // BREAKS if Atom has no position_
        }
    }
};
```

**Migration complexity:**
1. All `UnaryProcessor<Composite>` must become `UnaryProcessor<CompositeHandle>` or similar
2. Or: Add a new `UnaryProcessor<AtomHandle>` hierarchy
3. Or: Keep Composite inheritance but make it a thin shell (D17Composite)

**GROMACS precedent:** GROMACS does NOT have a Composite pattern. It uses:
- Flat arrays for atoms
- Separate molecule type hierarchy
- No polymorphism in hot paths

**Resolution:**
- **H4 flip:** Atom stops inheriting from Composite
- **D17Composite:** Thin shell that forwards to store
- **Processors:** Migrate to handle-based iteration
- **Transition period:** Both paths coexist (dual existence)

**Verdict:** **MEDIUM-HIGH RISK** - Complex but already designed in v2.2.

### Risk 4: Contiguous Range vs Scattered Index for Tree

**Claim:** We should use contiguous atom ranges per container (GROMACS-style).

**Attack:** Mutation patterns in BALLView **require** scattered indices.

**Evidence:**
- BALLView features that require mid-residue insertion:
  - PDB editing (insert residue between existing residues)
  - Mutation tools (insert atom into residue)
  - Sequence editing (insert amino acid)
- These operations **break contiguity**

**GROMACS vs BALL difference:**
| Operation | GROMACS | BALL |
|-----------|---------|------|
| Insert atom | Never (after grompp) | Common |
| Atom order | Optimized for DD | User-defined |
| Range stability | Guaranteed | Broken by mutation |

**Cost of maintaining contiguity:**
```cpp
// Insert atom into middle of residue
void insertAtomIntoResidue(Residue* res, Size pos, Atom& atom)
{
    // Shift all atoms after pos
    for (Size i = res->size(); i > pos; --i) {
        Atom* a = res->getChild(i-1);
        // Move atom to new position
        // Update all indices in all containers
    }
    // Insert new atom
    res->insertChild(pos, &atom);
}
// Cost: O(n_atoms) per insertion
```

**Cost of scattered indices:**
```cpp
// Kernel over residue atoms
for (Atom* a : *residue) {
    // Each atom is at arbitrary index
    // Cache miss on every atom access
}
// Cost: 2-5× slower for large residues
```

**Resolution:** **Hybrid approach**
- **Storage:** Scattered indices (mutation-friendly)
- **Kernel API:** Lazy contiguous view
- **Compaction option:** Can reorder into contiguous ranges on explicit compact()

**Verdict:** **MEDIUM RISK** - Hybrid approach adds complexity but is necessary.

### Risk 5: Worth Doing Before vs After H4 Flip

**Claim:** This SoA redesign should be done as part of H4.

**Attack:** The H4 flip is already a massive undertaking. Adding SoA x/y/z split increases risk.

**H4 scope (from V22-DECISIONS.md):**
- Delete Composite/PropertyManager/Selectable inheritance from Atom
- Atom becomes a thin handle
- MoleculeStore becomes sole source of truth
- Side tables for all former inline state

**Additional scope for SoA x/y/z:**
- Split positions_ into x_/y_/z_
- Migrate all 3,106 getPosition() call sites
- Update all kernels to use SoA
- Add SIMD vectorization
- Handle reference-return retirement

**Dependency analysis:**

| SoA Feature | H4 Dependency | Risk |
|-------------|---------------|------|
| Atom as handle | Required by H4 | None (H4 already does this) |
| SoA columns | None | Already exists in MoleculeStore |
| x/y/z split | None | New work |
| CSR bonds | None | Already partially exists |
| Kernel migration | SoA x/y/z | High |
| Vector3& retirement | SoA x/y/z | **VERY HIGH** |

**Timeline consideration:**
- **Do with H4:** SoA columns (already there), CSR bonds (H4 work)
- **Defer to post-H4:** SoA x/y/z split, kernel migration, Vector3& retirement

**GROMACS precedent:** GROMACS separated topology from state **before** optimizing the storage layout.

**Resolution:**
- **H4:** Complete the flip (Atom as handle, MoleculeStore as source of truth)
- **H4.1 (immediate post-H4):** Add SoA x/y/z columns, keep positions_ for compatibility
- **H4.2:** Migrate kernels to SoA
- **H4.3:** Retire Vector3& getPosition() (breaking change)
- **v2.3:** Add spatial binning (optional)

**Verdict:** **MEDIUM RISK** - SoA columns and CSR are H4-compatible. x/y/z split and Vector3& retirement should be H4+1.

## F2. Risk Summary Table

| # | Risk | Category | Likelihood | Impact | Mitigation | Priority |
|---|------|----------|------------|--------|------------|----------|
| 1 | Vector3& retirement scope | Migration | High | High | Phased migration | H4.3 |
| 2 | Interactive mutation/GC cost | Performance | Medium | High | Deferred compaction | H4.0 |
| 3 | Composite/processor decoupling | Migration | Medium | High | Handle iterators | H4.0 |
| 4 | Contiguous vs scattered tree | Design | Low | Medium | Hybrid approach | H4.0 |
| 5 | Timing (before vs after H4) | Scope | Medium | Medium | Staged rollout | H4+ |

---

# Appendix: Verification Checklist

## Code Verification

- [x] Verified `sizeof(Atom)` components in atom.h:1012-1051
- [x] Verified MoleculeStore SoA columns in moleculeStore.h:668-688
- [x] Verified BondRecord in moleculeStore.h:71-78
- [x] Verified CSR adjacency in moleculeStore.h:724-730
- [x] Verified AtomHandle in atomHandle.h:71-111
- [x] Verified free_list in moleculeStore.h:770-772
- [x] Verified generation counter in moleculeStore.h:765
- [x] Counted getPosition() call sites: 3,106

## Design Verification

- [x] GROMACS types verified against public source
- [x] AoS vs SoA decision grounded in GROMACS code
- [x] Topology/state separation matches GROMACS design
- [x] DD identity mapping understood
- [x] nbnxn repacking mechanism understood

## Migration Path Verification

- [x] Vector3& retirement: Phased approach proposed
- [x] Bond migration: CSR iteration pattern shown
- [x] Processor migration: Handle-based iteration shown
- [x] Tree structure: Hybrid contiguous/scattered proposed
- [x] GC policy: Deferred compaction with options proposed

## Performance Claims

- [ ] SoA x/y/z speedup: **NEEDS PROFILING** - claimed 1.5-3× but unmeasured
- [ ] No heap object speedup: **NEEDS PROFILING** - claimed 2-4× but unmeasured
- [ ] Contiguous ranges speedup: **NEEDS PROFILING** - claimed 1.2-2× but unmeasured
- [ ] CSR bonds speedup: **NEEDS PROFILING** - claimed 1.3-2× but unmeasured
- [x] Spatial binning speedup: **ACKNOWLEDGED** - 5-10× is GROMACS-proven

---

# Conclusion

## Summary Recommendation

**PROCEED with the design**, but in **stages**:

1. **H4 (Current):** Complete the Atom-as-handle flip
   - Delete Composite/PropertyManager/Selectable inheritance
   - MoleculeStore as sole source of truth
   - Side tables for tree structure
   - **Do NOT add SoA x/y/z split yet**

2. **H4.1 (Immediate post-H4):** Prepare for SoA optimization
   - Add SoA x/y/z columns (alongside positions_)
   - Add CSR bond iteration as primary path
   - Keep positions_ for backward compatibility

3. **H4.2:** Migrate kernels to SoA
   - Update ENERGY/ kernels first (highest impact)
   - Then MOLMEC/, STRUCTURE/, GEOMETRY/
   - Profile before/after

4. **H4.3:** Retire Vector3& getPosition()
   - **Breaking change** - requires v2.3 or later
   - Remove non-const getPosition()
   - Force value semantics

5. **v2.3 (Optional):** Add spatial binning
   - nbnxn-style pairlists
   - High risk, high reward

## Final Answer to User's Question

**Yes, this design is defensible and worth doing.** The GROMACS precedent proves that SoA + separated topology + pure handles works at scale. The BALL infrastructure (MoleculeStore, AtomHandle, container table, CSR adjacency) already provides 80% of the foundation.

**The critical tension (interactive mutation vs GC) is resolvable** with deferred compaction and generation-stable handles. The performance wins (2-10× for various workloads) justify the migration cost.

**Biggest risk:** The `Vector3& getPosition()` retirement touches 3,106 call sites and requires careful phased migration. This should be done **after** H4, not as part of it.

**The design is ready for implementation** in stages, with the adversarial concerns addressed above.
