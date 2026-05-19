# BALL 2.1 — Decision Log

**Status:** P0 design lock (in progress).
**Authored:** 2026-05-19.
**Companion:** `MILESTONE-v2.1-KICKOFF.md`.

D-decisions continue the v2.0 series (`KERNEL-V2-DECISIONS.md` ends
at D21). v2.1 starts at **D22**.

Each decision: question → options considered → chosen path →
rationale → impact on other decisions / phases.

---

## D22. Composite side-table: unified per-store node table

**Question:** Where does the Composite tree live once Atom no longer
inherits from Composite?

**Chosen:** **A. Unified per-store `vector<CompositeNode>` indexed by
`(kind, idx)`.**

**Rationale:**
- Atom is the high-volume case (100k+ per System), but Molecule /
  Chain / Residue / System are also Composites. A unified table
  amortises the side-storage infra across all five kinds.
- Compact layout: one `CompositeNode { parent_handle, first_child,
  next_sibling, kind: u8 }` per composite member.
- Lookup by handle is O(1) — `composite_nodes_[handle.idx]`.
- Avoids the half-gain trap of per-Atom heap `Composite*` (Option B
  would cost ~96 B/atom and lose most of the v2.1 win).
- Avoids the read-only-derived trap of Option C (we need
  `Atom::insert(*child)` patterns to keep working for v1.x compat).

**Impact:**
- `MoleculeStore` grows a `composite_nodes_` SoA column.
- Every Composite-derived class gets a `composite_handle_` lookup
  key instead of inline `parent_/child_/sibling_` pointers.
- Each Composite kind registers a tag (`COMPOSITE_KIND_ATOM`, ...)
  for the `(kind, idx)` discriminator.
- Tree mutation (`appendChild`, `removeChild`) becomes a store
  operation under the existing per-store mutex contract.

**Sizing:** `CompositeNode` ≈ 24 B (3× u64 handle + 1 byte kind +
padding). At 100k atoms that's 2.4 MB additional side storage —
well under the 36 MB we save from dropping the inline Composite
state on Atom (360 B × 100k = 36 MB).

---

## D23. PropertyManager → pre-allocated per-store columns (NOT sparse)

**Question:** Dense vector, sparse HashMap, or hybrid inline-spill?

**Chosen:** **A, refined: per-property typed columns in the store,
pre-allocated to store capacity.**

**Rationale (maintainer correction over the kickoff doc's lean):**

The kickoff doc assumed properties are sparse (most atoms have 0
properties) and recommended `HashMap<atom_idx, PropertyBag>`. The
maintainer's correction: **in BALL's actual usage pattern,
properties are assigned in bulk across all atoms at the same time**
(atom-typing pass writes MMFF94_TYPE on every atom; charge-assign
pass writes PARTIAL_CHARGE on every atom; etc.). The sparse-map
assumption is wrong.

So we move from "bag-per-atom" to **"column-per-property, indexed
by atom_idx"**:

```cpp
// In MoleculeStore:
class PropertyColumnRegistry {
    HashMap<String, PropertyColumnHandle> by_name_;
    std::vector<std::unique_ptr<PropertyColumnBase>> columns_;
public:
    template<typename T>
    PropertyColumnHandle registerColumn(const String& name);

    PropertyColumnHandle find(const String& name) const;
    PropertyColumnBase& column(PropertyColumnHandle h);
};

class PropertyColumn<T> : public PropertyColumnBase {
    std::vector<T> data_;           // sized to store capacity
    BitVector      present_;        // 1 bit per atom — is this prop set?
public:
    void set(atom_idx i, const T& v);
    bool isSet(atom_idx i) const;
    const T& get(atom_idx i) const;
    void clear(atom_idx i);
};
```

**Storage cost (100k atoms, 5 typical force-field props):**
- 5 × 4 B float column = 2 MB
- 5 × 12.5 KB BitVector = 62.5 KB
- Registry overhead: ~200 B

Vs the v2.0 per-atom PropertyManager bag, which is ~88 B inline +
heap-allocated value entries. At 100k atoms with 5 props each,
v2.0 uses ~8.8 MB of inline + heap allocations + map-rebalance
churn during atom-typing. v2.1 column approach is **~4× smaller**
and avoids the realloc cascade.

**Trade-off — what we lose:**
- Properties with very low fill rate (e.g. one atom out of 100k has
  a debug marker) waste a column allocation. Mitigated by:
  (a) the `present_` BitVector means unset slots are 0 cost beyond
  the 1-bit flag, and
  (b) typed columns can use sentinel values (NaN for float, empty
  string for String) to skip the BitVector for fully-dense props.
- Type punning: `PropertyManager` today is type-erased. We need to
  preserve that surface — `setProperty(name, NamedProperty)` looks
  up the registry; if the type doesn't match the registered column
  type, it throws `InvalidArgument`. Same observable behaviour as
  v1.x's mismatched-type assertion.

**Impact:**
- v2.0's `PropertyManager` is preserved as a thin facade that
  forwards every operation to the per-store column registry.
- Bond properties: same column infra in the bond table. Closes
  V21-BOND-PROPERTY-JSON for free — columns serialise as-is to
  K0.6 JSON.
- The Bond properties used by force fields (MMFF94SBMB,
  MMFF94RBL, VIRTUAL__BOND) become known column types, registered
  at first use.

**Open sub-question (deferred to P1):** Do we hoist a fixed schema
of "well-known" columns (PARTIAL_CHARGE, FORMAL_CHARGE, MMFF94_TYPE,
AMBER_TYPE, etc.) into typed accessors on Atom directly, bypassing
the registry? Probably yes for the 5-10 most-touched ones — cuts a
HashMap lookup off the hot path. Defer the list to a P1 benchmark.

---

## D24. Selectable: packed BitVector, not byte-vector

**Question:** Atomic per-bit, or single-threaded contract?
**Refined by user:** "store this as a bitvector."

**Chosen:** **Single-thread per-System contract (per D16), backed by
a packed BitVector (8× smaller than the kickoff's `vector<uint8_t>`
suggestion).**

**Rationale:**
- D16 already commits us to single-thread per-System mutation. No
  reason to pay atomic cost for selection bits.
- BitVector packing: 100k atoms = 12.5 KB instead of 100 KB. Fits
  comfortably in L2 cache on modern CPUs; full-store scan for
  "iterate all selected atoms" stays in cache.
- BALL already has `BALL::BitVector` in `DATATYPE` — reuse, not
  reinvent. Use `BALL::BitVector` (or `std::vector<uint64_t>` with
  inline bit ops if the BitVector API gets in the way of hot
  loops).

**Impact:**
- `MoleculeStore` gets a `selected_` BitVector column, sized to
  store capacity.
- `Atom::select()` / `isSelected()` / `deselect()` redirect to
  store. Composite-tree downward propagation (`select()` on a
  Molecule selects all its atoms) becomes a range-set on the
  BitVector — much faster than v1.x's per-atom recursion.
- Selection-aware queries (`Selector` "is selected" predicate)
  benefit: scan one BitVector instead of dereferencing every
  Atom*.

---

## D25. Stale-handle generation guard: debug-only

**Question:** Always-on, debug-only, or opt-in RAII?
**Chosen:** **B. Debug-only via `BALL_DEBUG`.**

**Rationale:**
- The kickoff lean was C (opt-in RAII). The maintainer's call:
  release builds pay zero for handle validation; debug builds
  trap on misuse.
- Matches the v2.0 contract: stale-handle detection in v2.0 is
  generation-counter-based and per-evaluate (CompiledExpression
  D17). Atom handles in release builds today have no per-deref
  check either — v2.1 doesn't regress that.
- Production workloads (renderer hot loops, batch processors) get
  full speed. Bugs surface in `BALL_DEBUG` builds and CI debug
  runs.
- Opt-in RAII (C) adds an API surface (`BorrowedAtom`) consumers
  would need to migrate to. Debug-only is invisible to consumer
  code.

**Impact:**
- Every Atom getter/setter in release mode: zero cost change vs
  v2.0.
- In `BALL_DEBUG`: each handle deref asserts
  `handle.generation == store->generation_`. On mismatch, throw
  `InvalidArgument` with file/line of the deref site.
- CI gains a "debug-suite run" job to catch handle-staleness
  regressions across the full test surface (cost: ~30% slower
  than release ctest).

---

## D26. v2.0 → v2.1 public API: preserve the surface

**Question:** Break the Atom-as-Composite surface, keep it, or
deprecate-then-remove?

**Chosen:** **A. Preserve the surface; redirect to side tables.**

**Rationale:**
- The whole point of v2.0's D2/D3/D4 thin-stub compromise was to
  preserve the public API surface while moving the storage. v2.1
  finishes the storage move; consumers shouldn't have to rewrite
  to benefit.
- Track B clusters (FORMAT, STRUCTURE, QSAR, DOCKING, MOLMEC) use
  the Composite + PropertyManager + Selectable surfaces
  extensively. Breaking them would propagate fix-up work across
  the entire downstream ecosystem.
- Performance cost of preserving the surface: one extra indirect
  call per Composite-tree access. Hot loops should use the new
  `MoleculeStore::iterAtoms()` API (V21-STORE-ITER-API) directly
  anyway.
- Deprecation pass (C) deferred to v2.2 if at all — gives consumers
  a release cycle to migrate hot paths to store-native APIs
  voluntarily.

**Impact:**
- Atom inherits from `D17Composite`, `D17PropertyManager`,
  `D17Selectable` shim classes that forward to the side tables.
- Method signatures unchanged. `atom->getParent()`, `atom->select()`,
  `atom->setProperty(...)` — identical to v2.0.
- The shim classes have no per-instance state (empty bases, EBO
  collapses them) — sizeof contribution is 0 B.
- Tests in Track B continue to pass without source changes.

---

## D27. Sequencing: v2.1 ships before v2.0.x patches close Wave 4-7

**Question:** Block on v2.0.x patches, or ship in parallel?
**Chosen:** **v2.1 ships first.**

**Rationale:**
- The thin-handle refactor is foundational; Wave 4-7 module
  closures are independent and can ride on either v2.0.x or v2.1.
- Doing v2.0.x first delays v2.1 by 4-8 weeks for work that doesn't
  unblock anything in v2.1's critical path.
- If Wave 4-7 closures uncover kernel bugs, they can land as v2.1
  patches (v2.1.1, v2.1.2) — same upstream branch.

**Impact:**
- v2.0.x maintenance line: only critical fixes (security, kernel
  correctness regressions found in production). No feature work.
- Wave 4-7 closures retarget to v2.1.x patch releases.

---

## D28. Adversarial review cadence: per-phase, design + post-implementation

**Question:** R17/R18 only, or sub-round per phase?
**Chosen:** **Per-phase, with Codex review at BOTH the planning gate
AND the implementation close of each phase.**

**Rationale (user direction):**
> "make sure you do an adversarial review in the planning and at the
> end of a phase for review and fix/gap closing."

This is stricter than the kickoff's "R17/R18 only" lean. The v2.0
pattern showed Codex reviews caught bugs the implementation didn't
self-detect (R11's 5 bugs, R12's 5 bugs, R13's 2 bugs, R14's 2
bugs, R16's 3 bugs). v2.1's surgery on Atom inheritance is higher-
risk than v2.0's K0.5 / K0.6 phases; per-phase guardrails are
proportional to the risk.

**Schedule (review IDs continue from R16):**

| Phase | Planning review | Close review |
|---|---|---|
| **P0 design lock** | — (covered by P1 planning review) | **R17** — design lock sanity |
| **P1 side-table infra** | **R18** — P1 plan review | **R19** — P1 close review |
| **P2 thin-handle flip** | **R20** — P2 plan review | **R21** — P2 close review |
| **P3 bond thin-handle** | **R22** — P3 plan review | **R23** — P3 close review |
| **P4 JSON closures** | **R24** — P4 plan review | **R25** — P4 close review |
| **P5 perf & benchmarks** | **R26** — P5 plan review | **R27** — P5 close review |
| **P6 release** | — | **R28** — pre-tag review |

**Impact:** 12 Codex review rounds across v2.1 (vs 7 in v2.0).
Budget: ~1 day per round for the review + fix cycle.

---

## D29. Branch strategy: v2.1 linear from v2.0

**Question:** Linear from v2.0, or rebase / merge cadence?
**Chosen:** **Linear `v2.1` branch from the v2.0.0 tag.**

**Rationale:**
- Master is still v1.x — no rebase target exists.
- v2.0 branch is now in maintenance mode (D27); the v2.1 work
  doesn't need to track v2.0 commits.
- Any v2.0.x critical fix that's relevant to v2.1 (kernel
  correctness) can be cherry-picked onto v2.1.

**Impact:**
- `origin/v2.1` is the v2.1 development branch.
- v2.1 PRs target `v2.1`, not `master`.
- After v2.1.0 final tag, merge v2.1 → master (or rebase master to
  fast-forward, depending on what's accumulated on master).

---

## D30. JSON forward/backward compatibility: not required

**Question:** Keep K0.6 schema byte-compatible, or MINOR bump?
**Chosen:** **MINOR bump. No forward-compat requirement.**

**Rationale (user direction):** "not required."

- v2.1 can add fields (e.g., per-property-column type tags, bond
  PropertyManager columns for V21-BOND-PROPERTY-JSON) without
  backward-compat reader logic.
- K0.6 MAJOR stays at 1; MINOR bumps to 1.1 for v2.1.
- v2.0 readers loading v2.1 JSON: fail cleanly with
  `Exception::ParseError("schema MINOR 1.1 > reader 1.0")`.
- v2.1 readers loading v2.0 JSON: supported (backward read). v2.1
  reader recognises missing MINOR-1.1 fields and substitutes
  defaults.

**Impact:**
- V21-LOAD-BATCH free to reorganise the JSON layout (e.g., bulk-
  column arrays instead of per-atom records) since byte-compat
  isn't required.
- V21-BOND-PROPERTY-JSON adds new top-level `bond_property_columns`
  section without breaking the writer.
- Documented in RELEASE-NOTES-v2.1.md as "v2.0 JSON loads in v2.1;
  v2.1 JSON does not load in v2.0".

---

## Cross-decision summary

| Decision | Choice (1-word) |
|---|---|
| D22 Composite side-table | unified-node-table |
| D23 PropertyManager | per-store-columns |
| D24 Selectable | packed-BitVector |
| D25 Generation guard | debug-only |
| D26 API surface | preserve |
| D27 Sequencing | v2.1-first |
| D28 Review cadence | per-phase-x2 |
| D29 Branch strategy | linear-from-v2.0 |
| D30 JSON compat | MINOR-bump, no fwd-compat |

---

## Implementation consequences (P1 prep)

D22 (unified node table) + D23 (column-per-property) + D24
(BitVector) all push storage into `MoleculeStore`. **P1's
deliverable: extend `MoleculeStore` with three new SoA segments —
`composite_nodes_`, `property_columns_`, `selected_bits_` — fully
populated and tested, with Atom/Composite/PropertyManager/Selectable
still inheriting as in v2.0.** P2 then flips the inheritance.

D25 (debug-only guard) + D26 (preserve API) mean the P1 → P2 flip
is internal-only. Consumer code is unaffected.

D28 (per-phase reviews ×2) means R17 lands immediately after this
decisions doc commits — review the P0 design before any P1 code is
written.

---

*Authored 2026-05-19. Next: R17 Codex adversarial review of D22-D30.*

---

# Revisions post-R17 (2026-05-19)

Codex R17 verdict: **NO-GO** (3 BLOCKERs + 4 BUGs + 3 DEBTs). See
`V21-CODEX-REVIEW-ROUND17.md`. The direction is sound; the specifics
needed correction. Revised D-decisions below preserve the original
direction and supplement with the corrected detail.

User direction on the corrections (2026-05-19): "plan the next phase
and go ahead in auto mode" — go with the maintainer's leans on N1-N5
+ apply N6-N9 pure corrections.

## D22a. CompositeNode — five-link, ~48 B (R17 P17-1 BLOCKER fix)

The original D22 specified a 3-handle `{ parent, first_child,
next_sibling, kind }` 24 B node. R17 P17-1 flagged this as
BLOCKER: current `Composite` carries **five** topology pointers
(`parent_`, `previous_`, `next_`, `first_child_`, `last_child_`),
all of which are API-visible — `getLastChild()` is public,
`hasPreviousSibling()` is public, and reverse-iterator semantics
require both `last_child_` and `previous_`.

**Revised CompositeNode:**

```cpp
struct CompositeNode {
    CompositeHandle parent;           // 8 B
    CompositeHandle first_child;      // 8 B
    CompositeHandle last_child;       // 8 B  (NEW; was missing)
    CompositeHandle next_sibling;     // 8 B
    CompositeHandle prev_sibling;     // 8 B  (NEW; was missing)
    uint32_t        child_count;      // 4 B  (was implicit in v2.0)
    uint8_t         kind;             // 1 B
    // 3 B padding
};
// total: 48 B per node
```

**Memory math at 100k atoms:** 48 B × 100k = **4.8 MB side
table** (was 2.4 MB claim). Still saves ~31 MB vs the inline
`Composite` state in v2.0 (which is 5×8 B pointers + Size + TimeStamp
+ counters ≈ 60-80 B per atom inline). Net gain remains overwhelming.

**Why keep 5-link instead of deprecating `getLastChild()`:**
Track B consumers (FORMAT, STRUCTURE, NMR, etc.) use the reverse-
iterator and `getLastChild` paths. Per D26 "preserve API surface,"
v2.1 doesn't break these. Future v2.2 may deprecate; not in v2.1
scope.

## D23a. Property registry — predeclared + sparse + promote (R17 P17-2/P17-3 BUG fix)

R17 P17-2 flagged unbounded growth (1M distinct names × 100k atoms
× 4 B = 400 GB memory bomb). R17 P17-3 flagged the storage math
ignored STRING/OBJECT columns.

**Revised design:**

1. **Well-known force-field columns are predeclared at store
   construction.** The fixed schema covers the ~10 columns BALL's
   force-field code touches per-atom (PARTIAL_CHARGE, FORMAL_CHARGE,
   MMFF94_TYPE, AMBER_TYPE, ATOM_TYPE_NAME, RADIUS, EPSILON,
   STEREO_DESCRIPTOR, HYBRIDIZATION, IS_AROMATIC). Static dispatch
   on Atom; zero registry lookup overhead.

2. **Dynamic property names start in a sparse fallback bag**
   (`HashMap<atom_idx, PropertyBag>`), one bag per atom that has
   any dynamic property set.

3. **Promotion to dense column** triggered by fill-rate ≥10% over
   the live atom count, sampled at `compact()` time. The promoted
   column type is fixed at promotion based on the dominant type
   in the sparse bag.

4. **Registry cap: 256 dynamic columns** (after well-known are
   excluded). Reaching the cap means further dynamic registrations
   stay sparse forever (no promotion). Configurable via
   `MoleculeStore::setMaxDynamicColumns()`.

5. **Property name interning**: every property name lives once
   in a per-store `name_pool_` (reuses the existing string pool
   infrastructure). Hash keys are `name_id_` (u32), not `String`.

**String columns:** dense `std::vector<uint32_t>` of intern-pool
offsets into a per-column intern pool, not `std::vector<String>`.
At 100k rows × 4 B = 400 KB per dense string column, plus the
intern pool itself (sized to distinct values).

**OBJECT/SMART_OBJECT properties:** stay in the sparse bag.
Identical placeholder semantics to v2.0 JSON.

**Mismatched-type on `setProperty(name, T)` when column is
registered as type U:** stays in the sparse bag for that atom
(does NOT throw). Matches v1.x's "per-atom independent
PropertyManager" semantics. This also closes R17 P17-10 backward-
read: v2.0 JSON files with name-type conflicts demux cleanly into
the sparse bag.

## D24a. Selectable — `std::vector<std::atomic<uint64_t>>` with relaxed atomics (R17 P17-4 BLOCKER fix)

R17 P17-4 flagged renderer-read / GUI-write race on packed
BitVector words as UB without sync.

**Revised:** `selected_bits_` is `std::vector<std::atomic<uint64_t>>`,
one word per 64 atoms. All reads use
`memory_order_relaxed`; all writes use atomic
`fetch_or` / `fetch_and` / CAS with
`memory_order_relaxed`.

**Rationale:**
- `memory_order_relaxed` on modern CPUs is free for aligned word-
  size loads/stores (x86-64, arm64, ARM-MTE all guarantee atomic
  word access). Cost vs. plain BitVector: zero on hot read paths.
- Renderer sees a torn-but-consistent view at word granularity
  (acceptable — selection is a visual indicator; one-frame stale
  is fine).
- GUI writes individual bits via `fetch_or` (set) / `fetch_and`
  (clear). No reader sees half-written bits.
- Eliminates the data race. C++ memory model compliant.

**API:** `bool is_selected(atom_idx) const` and
`set_selected(atom_idx, bool)` on `MoleculeStore`. Range-set ops
(`set_range_selected(begin, end)`) batch via word-level OR.

**Memory at 100k atoms:** 100000 / 64 = 1563 words × 8 B = **12.5 KB**.
Same footprint as the original BitVector claim — atomic adds no
size overhead (atomic<u64> is layout-compatible with u64 on all
supported platforms).

## D26a. EBO — `BALL_EMPTY_BASES` macro for MSVC portability (R17 P17-5 BUG fix)

R17 P17-5 flagged that default MSVC multiple inheritance does
NOT apply EBO and adds 1 B padding per empty base, breaking the
`sizeof(Atom) ≤ 32 B` target.

**Revised:** Add to `include/BALL/COMMON/macros.h`:

```cpp
#if defined(_MSC_VER)
  #define BALL_EMPTY_BASES __declspec(empty_bases)
#else
  #define BALL_EMPTY_BASES
#endif
```

Apply to every concrete handle class that inherits the v2.1 shim
bases:

```cpp
class BALL_EXPORT BALL_EMPTY_BASES Atom
    : public D17Composite, public D17PropertyManager, public D17Selectable {
    ...
};
```

**Verification:** `Sizeof_test.C` adds explicit pins for the
v2.1 target sizes (`sizeof(Atom) == 32` etc.) under all three
toolchains (Clang/GCC/MSVC). CI must build under MSVC before
v2.1.0 tag — the v2.0 CI runs Apple Clang only, so a Windows CI
job is a v2.1 prerequisite (filed as V21-CI-MSVC).

## D30a. v2.0 JSON backward read — sparse-bag fallback (R17 P17-10 BUG fix)

R17 P17-10 flagged that v2.0 allows the same property name with
mismatched types across atoms. D23a's sparse-bag fallback (above)
handles this cleanly:

- v2.1 reader processes v2.0 `atoms.properties[]` per-atom.
- Each named property: if name is a well-known force-field column
  AND value type matches, write to dense column.
- Otherwise: register dynamic column on first occurrence; subsequent
  same-name-different-type values go to sparse bag.
- Reading via `getProperty(name)` returns column value if present,
  else sparse bag value.

**Edge case docs:** `RELEASE-NOTES-v2.1.md` notes "v2.0 JSON files
with mixed-type same-name properties load successfully into v2.1;
the affected atoms keep their properties in the sparse fallback bag
and `getProperty()` reads work unchanged."

---

# New decisions D31-D34 (R17 process + remaining concerns)

## D31. Composite iterator invariant — pointer-only, no node-table aliasing (R17 P17-7 DEBT)

**Decision:** Composite iterators (`ChildCompositeIterator`,
`CompositeIterator`, all variants) may cache `Composite*` only.
They must NEVER cache:
- `CompositeNode*` (raw pointer into the side-table vector — invalid
  on vector growth)
- `composite_nodes_` index (handle.idx — valid across growth, but
  not across handle invalidation / re-use during compact())

**Resolution path:** iterator step methods (`forward`, `backward`,
`toBegin`, etc.) look up topology through `Composite*->getNode_()`
on each access. `Composite::getNode_()` returns
`store->composite_nodes_[handle.idx]` reference.

**Verification (P1 test plan):** new test
`CompositeIteratorStability_test.C` exercises
insertion-during-iteration scenarios that would invalidate any
node-table-caching iterator. Existing
`CompositeChildcompositeIteratorTraits_test` covers basic
forward/backward semantics.

## D32. v2.1-thin-only bug backport policy (R17 P17-8 DEBT)

**Decision:**

- **Kernel correctness bugs that exist in BOTH v2.0 and v2.1**: fix
  on v2.1.x AND cherry-pick to v2.0.x patch.
- **v2.1-thin-handle-only bugs (regression introduced by v2.1
  inheritance flip)**: fix on v2.1.x only. v2.0.x users are not
  affected by definition.
- **Wave 4-7 module closures discover a v2.1-thin-only bug**: file
  separately; v2.0.x users continue with the fat-handle workaround.
- **Wave 4-7 closure branches** must declare in PR description:
  v2.0-fat-compatible / v2.1-thin-only / shared.

This is documented in `MILESTONE-v2.1-KICKOFF.md` and
`RELEASE-NOTES-v2.1.md`.

## D33. Review cadence downgrade — P5/P6 close-only (R17 P17-9 DEBT)

**Decision (revises D28):**

| Phase | Planning review | Close review |
|---|---|---|
| P0 design lock | — | **R17 / R17b** ✅ |
| P1 side-table infra | **R18** | **R19** |
| P2 thin-handle flip | **R20** | **R21** |
| P3 bond thin-handle | **R22** | **R23** |
| P4 JSON closures | **R24** | **R25** |
| P5 perf & benchmarks | ~~R26~~ — *(downgrade: maintainer checklist)* | **R26** (renumbered close) |
| P6 release | ~~R28~~ — *(no planning needed)* | **R27** (renumbered pre-tag) |

P5/P6 are execution/verification phases against the locked design;
planning gate adds process cost without design-risk reduction.
Total v2.1 Codex rounds: **10** (R17 + R17b + R18-R25 + R26 + R27).

## D34. v2.1 CI must include MSVC build (R17 P17-5 implication)

**Decision:** The v2.0 CI runs Apple Clang on macOS arm64 only.
v2.1's `sizeof(Atom) ≤ 32 B` target depends on MSVC EBO behavior
that is NOT verified by Apple Clang. Add a Windows MSVC build job
to CI before v2.1.0 tag.

**Filed as V21-CI-MSVC backlog item.** Implementation:
- GitHub Actions Windows runner with VS 2022.
- Build with `cmake -G "Visual Studio 17 2022"`.
- Run `Sizeof_test` to verify the v2.1 sizeof pins under MSVC.

**Scope clarification:** the MSVC CI job is BALL-only (kernel +
core libraries), not BALLView (Qt+OpenGL adds dependency
complexity outside v2.1 scope — that's still a v1.8 / Phase 5
concern).

---

## Revised cross-decision summary

| # | Decision | Choice | R17 status |
|---|---|---|---|
| D22a | Composite side-table | unified-node-table, 5-link (~48 B) | BLOCKER → fixed |
| D23a | PropertyManager | predeclared + sparse + promote, 256-col cap | BUG → fixed |
| D24a | Selectable | atomic<u64> word vector, relaxed memory order | BLOCKER → fixed |
| D25 | Generation guard | debug-only | OK |
| D26a | API surface | preserve, with BALL_EMPTY_BASES on MSVC | BUG → fixed |
| D27 | Sequencing | v2.1-first | OK |
| D28 → D33 | Review cadence | per-phase planning+close P1-P4, close-only P5/P6 | DEBT → fixed |
| D29 | Branch strategy | linear-from-v2.0 | OK |
| D30a | JSON compat | MINOR-bump; backward read via sparse bag | BUG → fixed |
| D31 | Iterator invariants | Composite* only, no node-table cache | DEBT → fixed |
| D32 | Backport policy | thin-only stays v2.1.x; shared cherry-picks both | DEBT → fixed |
| D34 | MSVC CI | Windows job pre-v2.1.0 | BUG implication → fixed |

**R17 finding closure:** 7 of 7 BLOCKERs+BUGs addressed. 3 of 3 DEBTs
addressed. **Re-review gate: R17b (Codex CLI on revised D22a-D34).**

---

*Revised 2026-05-19 post-R17. Next: R17b re-review, then P1 plan.*

---

# Second revision post-R17b (2026-05-19)

Codex R17b verdict: **still NO-GO**. 2 new BLOCKERs (P17b-N1
mismatched-type sparse override, P17b-N2 atomic vector
non-Cpp17MoveInsertable) + 3 BUGs + 2 DEBTs introduced or
incompletely closed by the first revision. See
`V21-CODEX-REVIEW-ROUND17B.md`.

User direction: continue auto-mode per the prior turn. Applying
all R17b required fixes as a single revision.

## D22b. CompositeNode memory math correction (R17b P17b-1 BUG fix)

**Original claim (D22a):** "still saves ~31 MB vs the inline
`Composite` state in v2.0."

**Corrected:** the comparison is whole-handle shrink, not just
the displaced Composite topology. With `sizeof(Atom)` going from
360 B (v2.0) to ≤32 B (v2.1 target), gross savings are 32.8 MB
per 100k atoms; net after the 4.8 MB `composite_nodes_` side
table is **~28.0 MB saved per 100k atoms**, not 31.

D13 budget accounting: side table is 48 B/atom (1/3 of the 160
B/atom budget). The v2.1 target is full-handle ≤160 B/atom:

```
v2.0 inline state:    360 B/atom Atom + ~88 B PropertyManager bag
v2.1 side state:       48 B composite_nodes + ~20 B prop columns + 0.125 B sel_bits
                    = ~68 B/atom side
v2.1 thin handle:     ≤32 B Atom (post-P2, with empty bases)
v2.1 total:          ≤100 B/atom (well under 160 B budget)
```

Documented in `MILESTONE-v2.1-KICKOFF.md` success criteria.

## D23b. PropertyManager — sparse-first lookup + bounded sparse bag (R17b P17b-N1/N3 BLOCKER/BUG fix)

**P17b-N1 BLOCKER: setProperty replacement semantics.**

v2.0 `setProperty(name, T)` erases any prior same-name property
regardless of old type, then appends the new one
(`source/CONCEPT/property.C:193`). D23a's "mismatched type stays
in sparse bag" combined with D30a's "getProperty checks column
first" returns stale dense values after a type-changing setProperty.

**Revised lookup precedence (replaces D30a's column-first rule):**

```cpp
const NamedProperty* getProperty(const String& name, atom_idx i) const {
    // 1. Sparse override takes precedence (D23b)
    if (auto sparse = sparse_bag_.find(i); sparse != end()) {
        if (auto prop = sparse->second.find(name); prop) return prop;
    }
    // 2. Then dense column
    if (auto col = registry_.find_column(name); col) {
        if (col->isSet(i)) return col->as_named_property(i);
    }
    return nullptr;
}
```

**Revised setProperty:**

```cpp
void setProperty(const String& name, atom_idx i, T value) {
    auto col = registry_.find_column(name);
    if (col && col->type() == typeid(T)) {
        // matching dense column — write to column, clear sparse
        col->set(i, value);
        sparse_bag_.clearProperty(i, name);  // tombstone effect
    } else {
        // mismatched type OR no dense column — write to sparse
        // (this overrides any dense column read via precedence)
        sparse_bag_.setProperty(i, name, value);
        // do NOT clear col (other atoms may still use the dense column)
    }
}
```

This preserves v2.0 replacement semantics: a mismatched-type
setProperty makes the sparse entry win on subsequent getProperty.
The dense column row for that atom becomes effectively shadowed
(getProperty returns sparse). Dense column slot stays allocated
but unread for that atom — wastes one slot but avoids the
expensive "clear dense row" operation.

**P17b-N3 BUG: Sparse bag unbounded.**

Add cap: `MoleculeStore::setMaxDynamicPropertyNames(size_t)`,
default **65536 distinct dynamic names per store**. On reaching the
cap, further `registerColumn(new_name)` and sparse-bag entries
with new names throw `Exception::InvalidArgument("dynamic property
name cap exceeded")`. Existing names continue to work. Well-known
force-field columns are NOT counted against the cap.

**P17b-3 DEBT: Promotion hysteresis.**

Add policy: **promote once, never demote.** A promoted column
stays dense for the lifetime of the store. Demotion would require
moving column data back to sparse, which is expensive and creates
flap behavior. Workload that sets-then-unsets a property keeps the
column. Acceptable trade-off; documented.

## D24b. selected_bits_ — `unique_ptr<atomic[]> + size_t` (R17b P17b-N2 BLOCKER fix)

**P17b-N2 BLOCKER:** `std::vector<std::atomic<uint64_t>>` fails
libc++ Cpp17MoveInsertable static assertion on `shrink_to_fit()`.

**Revised:** owned atomic array rebuilt under exclusive lock:

```cpp
class MoleculeStore {
    // ...
    std::unique_ptr<std::atomic<uint64_t>[]> selected_bits_;
    size_t                                    selected_bits_word_capacity_;
    // ...

    void resize_selected_bits_(size_t new_atom_capacity) {
        // Caller holds the store's exclusive write lock.
        size_t new_word_capacity = (new_atom_capacity + 63) / 64;
        if (new_word_capacity == selected_bits_word_capacity_) return;

        auto new_bits = std::make_unique<std::atomic<uint64_t>[]>(new_word_capacity);
        // Copy existing words via relaxed load/store.
        size_t copy_words = std::min(selected_bits_word_capacity_, new_word_capacity);
        for (size_t w = 0; w < copy_words; ++w) {
            new_bits[w].store(
                selected_bits_[w].load(std::memory_order_relaxed),
                std::memory_order_relaxed
            );
        }
        // Zero new words.
        for (size_t w = copy_words; w < new_word_capacity; ++w) {
            new_bits[w].store(0, std::memory_order_relaxed);
        }
        selected_bits_ = std::move(new_bits);
        selected_bits_word_capacity_ = new_word_capacity;
    }
};
```

`compact()` calls `resize_selected_bits_(live_atom_count + slack)`
under its existing exclusive contract. `reserve_atoms()` calls
it similarly. The relaxed atomic load/store during resize is safe
because the caller holds exclusive write — no concurrent readers
during resize.

Steady-state read path (`is_selected(i)`) and write path
(`set_selected(i, b)`) operate on the owned array using relaxed
atomics. Zero extra cost vs. plain BitVector on aligned word
access.

API surface unchanged: `is_selected`, `set_selected`,
`set_range_selected`.

## D31b. CompositeNode hard encapsulation (R17b P17b-N5 / P17-7 DEBT fix)

**Original (D31):** documentation + future test.

**Revised:** structural enforcement.

- `CompositeNode` and `composite_nodes_` are declared in a
  private internal header `include/BALL/KERNEL/_moleculeStoreInternal.h`,
  consumed only by `moleculeStore.C`, `composite.C`, and the
  P1.3 wiring code.
- `Composite::getNode_()` returns topology by value:
  `CompositeTopologyView { Composite* parent, *first_child,
  *last_child, *next, *prev; Size child_count; }` — no
  `CompositeNode*` escapes.
- CI grep gate (`.github/workflows/ci-v2.yml`):

```bash
if grep -rn "CompositeNode\*\|composite_nodes_" \
      include/BALL/CONCEPT/composite.h \
      include/BALL/CONCEPT/composite.iC \
      $(find include -name "*Iterator*.h"); then
    echo "ERROR: CompositeNode leaks into iterator/public header"
    exit 1
fi
```

Iterator implementations cannot name `CompositeNode` because the
header isn't visible to them.

## D32b. Backport policy — v2.0.x-only regression case (R17b P17b-8 DEBT fix)

**Added clause to D32:**

> **v2.0.x-only regression that v2.1's thin handle would have
> prevented:** fix on v2.0.x with a regression test. If the same
> public-API behavior exists in v2.1 (it should — D26 preserves
> the surface), add the same test to v2.1 even if no code change
> is needed there. This ensures the v2.1 surface stays
> demonstrably correct on the case.

Wave 4-7 closure branches must explicitly state in PR description:
v2.0-fat-compatible / v2.1-thin-only / shared. The v2.0.x-only-
regression case is now the fourth bucket.

## D33b. Restore P5 planning review (R17b P17b-N4 BUG fix)

**Original (D33):** P5/P6 planning reviews downgraded.

**Revised:** P5 ships `V21-STORE-ITER-API` (public API surface),
generation-guard finalization (consumer-visible RAII type), and
perf gates (CI policy). All three warrant a planning review.

**Final cadence (re-tabulated):**

| Phase | Planning | Close |
|---|---|---|
| P0 design lock | — | R17 / R17b / **R17c** |
| P1 side-table infra | R18 | R19 |
| P2 thin-handle flip | R20 | R21 |
| P3 bond thin-handle | R22 | R23 |
| P4 JSON closures | R24 | R25 |
| P5 perf & benchmarks | **R26** (restored) | R27 |
| P6 release | — | R28 |

P6 stays close-only; P6 is release-notes + tag + no new design.

Total Codex rounds: **12** (R17 + R17b + R17c + R18-R25 + R26 + R27 + R28 = 13 ID slots; counting R17/R17b/R17c as the P0 cluster gives 11 phase reviews).

## D34b. MSVC CI gate moves to P2 close (R17b P17b-10 DEBT fix)

**Original (D34):** MSVC CI before v2.1.0 final tag.

**Revised:** MSVC CI must run successfully before merging P2 (the
thin-handle flip) to `v2.1`. P2 is where `sizeof(Atom) ≤ 32 B`
is actually claimed; verifying it under MSVC at that moment is
the right gate. Waiting until v2.1.0 final would mean discovering
EBO failures after extensive consumer code already depends on the
flip.

Implementation: add the Windows GHA job in P1 close commit so it's
running on a green-bar v2.0-equivalent baseline through P1, then
fails P2 PR if MSVC `Sizeof_test` pins don't hold.

---

## Revised cross-decision summary (post-R17b)

| # | Decision | Choice | Status |
|---|---|---|---|
| D22b | Composite side-table | 5-link 48 B node; ~28 MB net savings | CLOSED (math corrected) |
| D23b | PropertyManager | predeclared + sparse-first lookup + 65k-name cap + promote-once | CLOSED (N1/N3 + promotion) |
| D24b | Selectable | unique_ptr<atomic[]> + word_capacity + rebuild-under-lock | CLOSED (N2) |
| D25 | Generation guard | debug-only | OK |
| D26a | API surface | preserve, BALL_EMPTY_BASES on MSVC | OK |
| D27 | Sequencing | v2.1-first | OK |
| D29 | Branch strategy | linear-from-v2.0 | OK |
| D30a | JSON compat | MINOR-bump, backward read via sparse override | refined by D23b precedence |
| D31b | Iterator invariants | CompositeNode private to internal header + CI grep | CLOSED (N5) |
| D32b | Backport policy | + v2.0.x-only regression bucket | CLOSED |
| D33b | Review cadence | P5 planning restored | CLOSED (N4) |
| D34b | MSVC CI | gate moves to P2 close | CLOSED |

**R17b closure status:** 5/5 BUGs+BLOCKERs addressed. 2/2 DEBTs
addressed. P17b-6 (bond identity) remains deferred to P3/P4 per
out-of-scope instruction.

**Re-review gate: R17c.**

---

*Second revision 2026-05-19 post-R17b. Next: R17c re-review, then
P1 execute.*
