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

**P17b-N3 BUG: dynamic property names unbounded.**

Add cap: `MoleculeStore::setMaxDynamicPropertyNames(size_t)`,
default **65536 distinct dynamic names per store**. On reaching the
cap, further `registerColumn(new_name)` and sparse-bag entries
with new names throw `Exception::InvalidArgument("dynamic property
name cap exceeded")`. Existing names continue to work. Well-known
force-field columns are NOT counted against the cap.

**Wording note (R17c P17c-3):** sparse-bag entries themselves are
NOT separately byte-capped. They remain proportional to
`(live_atoms × capped_dynamic_names)` worth of explicit
`setProperty(name, ...)` calls. The cap bounds *name growth*; entry
count is bounded by explicit assignments. Imprecise phrasing
"bounded sparse bag" replaced with "bounded dynamic property
names" throughout.

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

Total Codex review invocations: **14** (P0 cluster 3 + P1-P5 ×2 + P6 close 1). Numeric ID slots: 12 (R17 + R18-R28), with R17b/R17c as suffixed repeats of the P0 design-lock review. The doc previously said "12 rounds" — corrected per R17c P17c-8 to distinguish actual invocations from ID slots.

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

---

# Third revision post-R19 (2026-05-19 P1 close)

R19 verdict: NEEDS-FIXES. Three findings: R19-F1 CI guardrails
unwired, R19-F2 iterator-stability test coverage incomplete,
R19-F3 public underscore accessors need API classification.

R19-F1 splits into two parts:
- D31b grep gate: actionable in P1 close. **Closed** in the same
  commit that addresses R19 (added to `.github/workflows/ci-v2.yml`,
  blocks PRs that leak CompositeNode* / composite_nodes_ into
  iterator-adjacent public headers).
- D34b Windows MSVC job: NOT actionable in P1 close. The codebase
  has never been built on Windows; vcpkg setup is part of the
  separate Phase 4 (BALLView 1.6 modernization) roadmap. Forcing
  this gate at P1 close would block v2.1 on a parallel project
  whose ETA is not v2.1's to set. **D34c (below) defers this.**

R19-F2 partial closure: added splice + reverse-wrapper coverage
to CompositeIteratorStability_test. clear/destroyChildren mid-
iteration coverage is deferred to P2 (where the destroy paths
are touched anyway for the inheritance flip; adding tests in
isolation now would be redundant work).

R19-F3 closed: D35 (below) classifies the public underscore
accessors.

## D34c. MSVC CI gate moves to v2.1.0-rc1 (R19-F1 revised)

**Revises D34b.**

D34b promised the Windows MSVC GHA job at P1 close. R19-F1 correctly
flagged it as missing. Practical reality: the BALL codebase has never
been built on Windows. The CLAUDE.md project brief notes "vcpkg for
Windows is pending (Phase 4)" — Phase 4 is the BALLView 1.6
modernization wave, NOT v2.1's kernel work. Adding Windows CI
requires:
1. vcpkg manifest for Qt5 + Boost + Eigen3 + flex + bison + cmake
2. CMakeLists adjustments for MSVC-specific link flags (`/EHsc`,
   `_USE_MATH_DEFINES`, etc.)
3. Resolution of any MSVC-specific source incompatibilities
   (currently unknown — never been compiled)

That is a 1-2 day project of its own, blocked on Phase 4 sequencing.

**Revised timing:**
- **D34c.1 (NEW):** at v2.1.0-rc1 tag preparation (post-P5, pre-
  release), Windows MSVC CI MUST be wired and green for the v2.1
  kernel CORE_ONLY surface (with v2.1 sizeof pins active).
- **D34c.2 (NEW):** P2 ships the inheritance flip and asserts
  `sizeof(Atom) ≤ 32 B` on Apple Clang via the existing
  `Sizeof_test`. Apple Clang and GCC apply EBO for distinct empty
  bases; the v2.1 target sizes hold on both. MSVC verification
  happens at rc1.
- **D34c.3 (NEW):** Between P2 close and v2.1.0-rc1, a parallel
  sub-track (V21-MSVC-CI) wires vcpkg + Windows builds. This is a
  v2.1 backlog item that can run concurrently with P3-P5.
- **D34c.4 (NEW):** If MSVC at rc1 reveals `sizeof(Atom) > 32 B`,
  the fix is to add `BALL_EMPTY_BASES` to the affected classes
  (the macro is already in place from P1.1). This is a
  per-class, mechanical fix — not a redesign.

Risk acknowledged: if MSVC EBO behavior differs more dramatically
than `__declspec(empty_bases)` can handle, v2.1.0 may need a
late-stage size-pin relaxation on Windows. This is documented as
a known v2.1 release risk in MILESTONE-v2.1-KICKOFF.md.

## D35. Public underscore accessors are internal/experimental, NOT v2.1 stable API (R19-F3 closure)

`Composite::getCompositeHandle_`, `setCompositeHandle_`, and
`getNode_` are public on the v2.1 P1.3 commit (39fb54415). The
incomplete-return-type gate prevents accidental use without
`_moleculeStoreInternal.h`, but they ARE in the public ABI.

**Decision:** these accessors are **internal/experimental** and
explicitly excluded from v2.1 stable API promises.

- Documented in `RELEASE-NOTES-v2.1.md` (drafted at P6) under a
  "Not stable for v2.2" section.
- Trailing-underscore naming convention is the v2.1 marker for
  this status.
- v2.2 candidates: move to `protected` with a friend grant for
  `MoleculeStoreSideTables`, or wrap behind a free-function API
  in the internal header.

This avoids API-freeze paralysis at v2.1.0 over still-evolving
internal storage details.

---

## Revised cross-decision summary (post-R19)

| # | Decision | Status |
|---|---|---|
| D22b CompositeNode 5-link | CLOSED |
| D23b PropertyManager column registry | CLOSED |
| D24b selected_bits_ atomic-array | CLOSED |
| D26a EBO macro | CLOSED (macro defined; application at P2) |
| D30a backward-read JSON demux | CLOSED |
| D31b CompositeNode encapsulation + grep gate | CLOSED at P1 close (CI gate wired) |
| D32b backport policy | CLOSED |
| D33b review cadence | CLOSED |
| D34c MSVC CI defers to rc1 (revised D34b) | CLOSED with rc1 gate |
| D35 internal/experimental accessor classification | CLOSED |

**P2 entry gate (R19 checklist):**
1. ✅ D31b grep gate wired in CI
2. ✅ CompositeHandle is_trivially_copyable + alignment static_asserts
3. ✅ Iterator stability test expanded (splice + reverse wrapper)
4. ✅ D35 API classification recorded
5. ⏭ D34c MSVC CI deferred to rc1 (formal revision)
6. ⏭ clear/destroyChildren iterator coverage deferred to P2

*Third revision 2026-05-19 post-R19 P1 close. Next: R19 fix
commit, then P2 (thin-handle flip).*

---

# Fourth revision post-R20 (2026-05-19 P2 plan NO-GO + pivot)

R20 verdict on P2 plan: **NO-GO** (8 BLOCKERs). The plan
underestimated how entrenched Atom's bases are. See
`V21-CODEX-REVIEW-ROUND20.md`.

**Maintainer decision (2026-05-19):** "The RTTI thing can
disappear, resolving the composite model. Instead we go with
option B."

Option B = two-phase flip: v2.1 ships side-table state in
parallel with v0 inline state; v2.2 deletes the v0 inline state
and flips inheritance.

## D36. Two-phase thin-handle flip across v2.1 + v2.2

**Revises and supersedes D26a/D26 for the v2.1 milestone scope.**

**v2.1 scope (replaces original P2-P6 plan):**

- Side-table state is **maintained in parallel** with v0 inline
  Composite / PropertyManager / Selectable state. Every mutation
  through the v0 API also writes to the matching side table.
- `Atom` continues to inherit `Composite + PropertyManager +
  Selectable` exactly as in v2.0. **No inheritance change in v2.1.**
- `sizeof(Atom)` stays at 360 B in v2.1. The D13 budget miss
  documented in v2.0 remains documented; v2.1 does NOT close
  that gap.
- v2.1's headline shifts from "thin handles deliver D13 budget"
  to "side-table infrastructure + JSON improvements + column
  properties + RTTI cleanup, paving the way for v2.2's thin
  handles."

**v2.2 scope (NEW, separate milestone):**

- After all consumers have moved off Atom-RTTI surface (D37 below),
  delete `Composite + PropertyManager + Selectable` from Atom's
  inheritance chain.
- Replace with `D17Composite / D17PropertyManager / D17Selectable`
  empty shim bases (with `BALL_EMPTY_BASES`).
- Delete v0 inline state from `Composite`; reads come exclusively
  from side tables.
- `sizeof(Atom)` drops to ≤32 B; D13 budget closed.

**Why this is safer than original P2:**

- v2.1 has 100% source compatibility. Track B consumers don't
  change.
- v2.1 bake time on real workloads validates the side-table
  invariants BEFORE the inheritance flip.
- v2.2's flip is then mechanical (consumer code already updated;
  inheritance change is the last step).
- Two adversarial-review cycles (v2.1 close + v2.2 plan) instead
  of trying to ship the flip atomically.

**Drawback acknowledged:** v2.1 does not deliver the D13 win.
Documented in `RELEASE-NOTES-v2.1.md` (drafted at P6).

## D37. Atom-RTTI surface removal (v2.1 prerequisite for v2.2 flip)

**Decision:** v2.1 removes Atom-specific RTTI usage from BALL
kernel code (24 sites surveyed). VIEW + APPLICATIONS keep theirs
for now (54 sites; addressed in Phase 4 / BALLView 1.6 roadmap).

**Scope:**

- `dynamic_cast<Atom*>(composite_ptr)` → replaced with explicit
  store-handle check, or refactored to take `Atom*` directly.
- `RTTI::isKindOf<Atom>(&composite)` → same.
- All v2.1 kernel sites (24) closed.
- VIEW + APPLICATIONS (54) tracked as V21-VIEW-RTTI follow-up,
  closed by v2.2 inheritance flip prep.

**Audit (2026-05-19):**
- KERNEL/STRUCTURE/FORMAT/QSAR/MOLMEC/SCORING/SOLVATION/DOCKING/
  ENERGY/NMR/CONCEPT: 24 sites.
- VIEW + APPLICATIONS: 54 sites.

**Implementation phase:** P3 (renamed from P2 in the original
plan; v2.1's P2 becomes "parallel side-table maintenance," P3
becomes "RTTI removal," P4-P6 as originally planned).

## Revised v2.1 phase plan (supersedes original P0-P6)

| Phase | Subject | Status |
|---|---|---|
| P0 | Design lock | ✅ DONE (R17/R17b/R17c GO) |
| P1 | Side-table infrastructure storage | ✅ DONE (commits 173374553..ac3e142df) |
| **P2** | **Parallel side-table maintenance** (renamed) — wire Composite/PropertyManager/Selectable mutations to dual-write side tables alongside v0 inline state. NO inheritance change. | NEXT |
| **P3** | **Atom-RTTI removal** (NEW) — 24 kernel sites cleaned up. VIEW/APPLICATIONS deferred. | After P2 |
| P4 | JSON closures — V21-LOAD-BATCH, V21-BOND-PROPERTY-JSON, V21-ELEMENT-INSTANCE-ID | After P3 |
| P5 | Perf + benchmarks — V21-STORE-ITER-API, V21-CI-PERF-GATES, V21-MEDIAN-OF-N-BENCH | After P4 |
| P6 | Release — v2.1.0-rc1 → v2.1.0 | After P5 |

D33b cadence applies: each phase gets planning (Rxx) + close (Rxx+1) Codex reviews.

## D38. v2.2 milestone scope (formalised)

**v2.2 scope (placeholder; detailed plan deferred to v2.2-P0):**
- Delete Composite + PropertyManager + Selectable from Atom's
  inheritance chain.
- Apply BALL_EMPTY_BASES to D17* shim bases.
- Delete v0 inline state from Composite.
- Migrate VIEW + APPLICATIONS off Atom-RTTI (parallel with Phase 4
  BALLView refresh).
- sizeof(Atom) ≤ 32 B; D13 budget closed.
- v2.2 ABI break is intentional and announced.

v2.2 is NOT a separate branch yet — it's a milestone marker. v2.1
finishes first.

---

## Revised cross-decision summary (post-R20 pivot)

| # | Decision | Status |
|---|---|---|
| D22b CompositeNode 5-link | CLOSED |
| D23b PropertyManager column registry | CLOSED |
| D24b selected_bits_ atomic-array | CLOSED |
| D26a EBO macro | CLOSED (macro defined; application deferred to v2.2 by D36) |
| D30a backward-read JSON demux | CLOSED |
| D31b CompositeNode encapsulation + grep gate | CLOSED |
| D32b backport policy | CLOSED |
| D33b review cadence | CLOSED |
| D34c MSVC CI defers to v2.1.0-rc1 | CLOSED |
| D35 internal/experimental accessor classification | CLOSED |
| **D36 two-phase flip across v2.1+v2.2** | **NEW, closes R20 NO-GO** |
| **D37 Atom-RTTI removal (24 kernel sites)** | **NEW** |
| **D38 v2.2 milestone scope placeholder** | **NEW** |

**P2 entry gate (revised):** P2 is now "parallel side-table
maintenance." No inheritance change. Lower risk than the original
P2; should pass R20b cleanly.

*Fourth revision 2026-05-19 post-R20. Next: rewrite P2-PLAN.md
to match revised scope; R20b on the rewrite; then P2 execution.*

---

# Fifth revision post-R21 (2026-05-19 v2.1 roadmap pivot)

P2.1.1 attempted mutation wiring corrupted the heap via cascading
~System destruction. D39 + D40 (below) defer mutation wiring to
v2.2; v2.1 ships scaffolding + RTTI + JSON + perf.

R21 verdict on the resulting roadmap: NEEDS-FIXES with 7 narrow
revisions and 4 GO probes. All R21 NEEDS-FIXES revisions captured
in V21-ROADMAP.md's second pass. D34c is revised here:

## D34d. MSVC CI moves to v2.2 (revises D34c)

**Revises D34c** (which moved Windows MSVC CI from "P1 close" to
"v2.1.0-rc1 prep").

**Decision per R21 P21-11 / R21-F3:** Since D40 explicitly removes
the `sizeof(Atom) ≤ 32 B` target from v2.1, the MSVC EBO
verification that motivated D34c is no longer a v2.1 release
gate. Windows MSVC CI moves to **v2.2 P0/P2** where the sizeof
target actually matters.

**For v2.1:**
- Apple Clang macOS arm64 remains the only enforced CI target
- Windows portability is a non-blocking v2.1 backlog item
  (filed as V21-MSVC-CI-PORTABILITY) — if the codebase builds on
  Windows with vcpkg, great; if it doesn't, that's a v2.2 gate
  not a v2.1 gate
- BALL_EMPTY_BASES macro shipped in P1.1 still works on MSVC
  when v2.2 needs it; it's a no-op on Clang/GCC currently

**For v2.2:**
- Windows MSVC GHA job lands in v2.2 P0 (before the inheritance
  flip's sizeof claim is made)
- Phase 4 (BALLView 1.6 vcpkg setup) becomes a v2.2 dependency

## D41. New v2.1-internal decisions from R21 follow-ups

Three small decisions promoted from R21 review:

**D41.1 (R21 P21-4 / R21-F2):** v2.1 does NOT add
`Composite::isAtom_()` virtual. Per-site refactor for the 24
RTTI removal sites instead. v2.2 inheritance flip would make
the virtual invalid anyway.

**D41.2 (R21 P21-5 / R21-F1):** P4.1 V21-LOAD-BATCH is gated
on a P4.0 profiling sub-step. The specific perf target ("8×
load speedup") is conditional on the dominant bucket; the
roadmap can't commit to a number without profile data.

**D41.3 (R21 P21-8):** P5.2 perf gate calibration uses the
`max(2 × median, median + 6 × MAD)` rule over 5 baseline runs.
Benchmarks with CoV > 30% are advisory until variance settles.

---

## Revised cross-decision summary (post-R21 roadmap pivot)

| # | Decision | Status |
|---|---|---|
| D22b CompositeNode 5-link | CLOSED |
| D23b PropertyManager column registry | CLOSED |
| D24b selected_bits_ atomic-array | CLOSED |
| D25 generation guard | CLOSED (P5.4 verifies #ifdef proof) |
| D26a EBO macro | CLOSED |
| D27 Sequencing | CLOSED |
| D29 Branch strategy | CLOSED (P6.6 explicit post-tag action) |
| D30a backward-read JSON demux | CLOSED |
| D31b CompositeNode encapsulation | CLOSED |
| D32b backport policy | CLOSED |
| D33b review cadence | revised → 6 rounds remaining (was 8) |
| D34c → **D34d** MSVC CI moves to v2.2 | CLOSED |
| D35 internal/experimental accessor classification | CLOSED |
| D36 two-phase flip | CLOSED |
| D37 Atom-RTTI removal scope | CLOSED |
| D38 v2.2 milestone placeholder | CLOSED |
| **D39 mutation wiring deferred to v2.2 (NEW from R21)** | CLOSED |
| **D40 v2.1 deliverable summary + honest framing (NEW)** | CLOSED |
| **D41.1 no `Composite::isAtom_()` (NEW)** | CLOSED |
| **D41.2 P4.1 profile-driven target (NEW)** | CLOSED |
| **D41.3 P5.2 calibration rule (NEW)** | CLOSED |

*Fifth revision 2026-05-19 post-R21. Next: R21b on the revised
roadmap.*

---

# Sixth revision post-P4.2 (2026-05-20)

## D42. V21-BOND-PROPERTY-JSON deferred to v2.2 (P4.2 findings) — **SUPERSEDED by D42-REV**

> **D42-REV (2026-05-20, maintainer reversal):** D42's deferral was
> reversed. **V21-BOND-PROPERTY-JSON was IMPLEMENTED in v2.1 P4.2**
> via option A from `P4.2-FINDINGS.md`: the System loader
> reconstructs the Atom-side `Bond*` graph with `createBond` and
> round-trips the Bond `PropertyManager` bag (commit `45df3714d`;
> R25-hardened against the leak + duplicate-pair boundary cases in
> `67827ad7c`). This ALSO closed D43 (the `atom.countBonds()==0`
> fidelity gap) and the R16 C-B7 v2.0 known gap. Schema minor
> bumped (store 1→2, system 0→1). The text below is retained as
> the original deferral rationale (now historical); read the
> "Impact" bullets as superseded.

### Original D42 (historical — deferral, reversed by D42-REV)


**Decision (maintainer, 2026-05-20):** Option C from
`P4.2-FINDINGS.md`. V21-BOND-PROPERTY-JSON moves out of v2.1 P4 to
the v2.2 milestone, bundled with V21-BOND-THIN-HANDLE.

**Rationale:** P4.2's "clean win" premise (R16 C-B7 / R24 P24-6 —
"read Bond's v0 PropertyManager directly") is correct on save but
impossible on load. `loadSystemJSON` only restores store
BondRecords; it never reconstructs the `Bond*` heap objects that
own the PropertyManager bags (`atom.countBonds() == 0` after a
System round-trip). Closing the round-trip requires reconstructing
the Atom-side `Bond*` graph — materially bigger than "serialize one
more bag," and entangled with the v2.2 bond-thin-handle redesign +
mutation wiring (D39, already v2.2). Building Atom-side `bond_[]`
wiring (Option A) or standalone bond-property columns (Option B)
now is scaffolding v2.2 would rework.

**Impact:**
- v2.1 P4 scope is now: P4.0 (done) + P4.1 V21-LOAD-BATCH (done,
  16× load) + P4.3 V21-ELEMENT-INSTANCE-ID + P4.4 schema bump.
- The R16 C-B7 known gap (Bond PropertyManager not in K0.6 JSON)
  carries forward to v2.2. Documented in RELEASE-NOTES-v2.1.md
  with `P4.2-FINDINGS.md` as the rationale.
- `V21-BOND-PROPERTY-JSON` reclassified from v2.1 P4 to v2.2 in
  `.planning/v2.1/BACKLOG.md`.

## D43. Latent bond round-trip-fidelity gap noted — **CLOSED in P4.2 (see D42-REV)**

> **CLOSED 2026-05-20:** D43's `atom.countBonds()==0` gap was fixed
> in P4.2 (the loader now reconstructs the `Bond*` graph via
> `createBond`). The text below is the original observation.

### Original D43 (historical — closed by P4.2)

**Observation (not a decision to act in v2.1):** the System JSON
round-trip does not reconstruct the Atom-side `Bond*` graph, so
`atom.countBonds()` returns 0 after `loadSystemJSON` even though
the store has the bonds. This is a pre-existing v2.0 fidelity gap,
independent of bond *properties*. Filed as
`V21-BOND-GRAPH-RECONSTRUCT` in the backlog; its natural home is
the same v2.2 bond-thin-handle work (D42). Not patched in v2.1
because the v2.2 redesign resolves the bond representation
question wholesale rather than bolting Atom-side wiring onto the
current dual representation.

*Sixth revision 2026-05-20 post-P4.2. Next: P4.3
V21-ELEMENT-INSTANCE-ID.*

---

# Seventh revision post-R26 (2026-05-20)

## D44. P5 trimmed to median-of-N benchmark reporting only

**Decision (R26 P5 planning review + maintainer "potentially
revise, then execute"):** P5 is trimmed to P5.3
(V21-MEDIAN-OF-N-BENCH) alone. P5.1, the heavy part of P5.2, and
P5.4 are deferred to v2.2.

**Rationale (R26, code-grounded):**
- **P5.1 STORE-ITER-API → v2.2.** "Skip Atom* materialisation" is
  not real in v2.1 — atoms are heap `Atom*` objects with a store
  back_ptr; the objects already exist, so an iterator yielding
  `Atom*` skips nothing. The win arrives only after the v2.2
  thin-handle flip. Shipping the public API now would lock a
  value-category/invalidation contract before the thin-handle
  shape is known.
- **P5.2 pinned-baseline CI gate → v2.2/v2.1.x.** The ClassTest
  harnesses can't read a baseline file + compute
  `max(2×median, median+6×MAD)` + classify CoV without new infra.
  Over-built for single-platform CI; P4 already tightened the
  gates that catch the known regression. v2.1 keeps fixed ctest
  thresholds.
- **P5.4 generation-guard → v2.2.** `Atom::store_generation_`
  exists, but a per-deref equality check against
  `store_->generation()` would FALSE-TRIP on ordinary store
  growth/reserve/compact (the generation counter tracks
  column-storage events, not handle staleness; a handle stays
  valid if its slot exists). Correct detection needs v2.2
  slot-generation semantics. Implementing the naive D25 check now
  would be wrong. Tracked as `V21-GENERATION-GUARD`.
- **P5.2 vs P5.3 ordering** was backwards (gates depend on the
  median harness). With the gate deferred, only P5.3 remains.

**Impact:**
- v2.1 P5 scope = P5.3 only: a median-of-N benchmark-reporting
  helper applied to JsonBench + SelectorBench, keeping existing
  fixed thresholds. R27 close review.
- D25 (generation guard, debug-only) is **superseded for v2.1**
  by D44 — it moves to v2.2 with the corrected slot-generation
  framing.
- v2.1 still ships honestly per D40: JSON load 16× + bond
  round-trip + RTTI cleanup + reproducible bench numbers, plus
  v2.2 prep. No new public API, no fragile CI perf system.

*Seventh revision 2026-05-20 post-R26. Next: execute P5.3.*
