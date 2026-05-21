# BALL v2.2 — H1b Design: container value-handle types (dual existence)

**Status:** design lock (R33 review pending).
**Authored:** 2026-05-20, post-H1a (R32b GO).
**Companion:** `V22-H0-ADDENDUM.md` (D56–D62), `V22-ARCH-HANDLE-MODEL.md`,
`V22-API-BREAK-LEDGER.md`, `V22-DECISIONS.md`.
**Phase:** H1b — "Atom/Bond/container value-handle TYPES reading from the
store, alongside v0 objects (dual existence; v0 = source of truth per
D60)."

> **SUPERSESSION NOTICE (2026-05-21, HC1):** the maintainer's
> hierarchy-collapse directive (`V2X-HIERARCHY-COLLAPSE.md`) — if its
> `H1b′` design-lock gate passes — **supersedes the 8 typed handles below**
> with two role-aware handles (`Molecule`, `Fragment`) + `Atom`, and
> shrinks `ContainerKind` + the D58 payload to a role + role-payload model.
> The handle SHAPE / validity / encapsulation contracts (D64/D65/D66a)
> still hold; only the *number* of typed handles and the per-kind getters
> change. Treat the per-kind handles below as the pre-collapse design.

H1a delivered the store-side `ContainerTable` (rows + `ChildRef` edges +
orphan migration + parity skeleton), exercised only by tests; nothing in
the v0 path reads it. H1b introduces the **value-handle TYPES** for the
molecular-hierarchy containers that READ from that table, locking the
handle shape + read path before the H2 traversal API and H3 consumer
migration. It stays non-invasive: v0 objects remain the source of truth;
the handles are exercised by tests, not yet wired into the v0 API.

This doc locks four things: (1) the dual-existence naming/coexistence
strategy, (2) the handle type shape, (3) handle validity/generation
(R31 follow-up #1), (4) the H1b read-only getter surface.

---

## D63. Dual-existence naming: `*Handle` suffix in `BALL::`, aliased at H4

**The problem.** Through H1b–H3 BOTH must exist: the v0 C++ classes
`BALL::Molecule`/`Chain`/`Residue`/`Protein`/`SecondaryStructure`/
`Nucleotide`/`NucleicAcid`/`Fragment` (heap objects, `Composite`-derived,
the source of truth) AND the new value-handle types reading the container
table. They cannot share a name while both compile, and a v0 class cannot
simultaneously be a value handle (it is a heavyweight `Composite`).

**Decision.** During dual existence the handle types live in `BALL::`
with a **`Handle` suffix**: `MoleculeHandle`, `ChainHandle`,
`ResidueHandle`, `ProteinHandle`, `SecondaryStructureHandle`,
`NucleotideHandle`, `NucleicAcidHandle`, `FragmentHandle`.

- `Atom` / `Bond` keep their canonical names (already value handles since
  v2.0; no v0 object collision). Only the **container** kinds take the
  transitional suffix, because only their names are still occupied by a v0
  class. The suffix is explicitly transitional and removed at H4.
- **Rejected — `BALL::v2::Residue` namespace:** more uniform, but it puts
  Atom (canonical, in `BALL::`) and `v2::Residue` in different namespaces
  anyway (mixed regardless), and the H4 cleanup is a whole-namespace move
  rather than a per-name alias — higher risk, harder to grep mid-flight.
- **Rejected — rename v0 classes to `ResidueV0`/`detail::ResidueObject`
  now:** the v0 classes are the live source of truth used pervasively;
  renaming them in H1b is the opposite of non-invasive and churns the
  whole tree before any handle is even consumed.

**The H4 flip is a canonical-name MIGRATION AUDIT, not a one-line alias
(R33 HIGH-1).** A naive `using Residue = ResidueHandle;` is NOT
source-transparent: it collides with existing `class Residue;` forward
declarations (`include/BALL/KERNEL/atom.h:44-54`,
`include/BALL/KERNEL/predicate.h:28`), `friend class Residue;`
declarations, `Residue*`-vs-`ResidueHandle` overload sets that would
collapse, and template/helpers keyed on `Residue`
(`include/BALL/KERNEL/residue.h`). So at H4, per kind:

1. Delete the v0 `Residue` class **and** its forward declarations + friend
   declarations + any v0-only specializations.
2. **Rename `ResidueHandle` → `Residue`** so the canonical name is a REAL
   class again (not an alias). This is what makes forward declarations,
   `friend class Residue;`, and specializations legal again.
3. Add the compatibility alias in the SAFE direction —
   `using ResidueHandle = Residue;` — so any H3-migrated code still
   spelling `ResidueHandle` keeps compiling. (This direction has no
   forward-decl/friend hazard because nothing forward-declares or
   friend-declares `ResidueHandle`.)
4. Audit + reconcile overload sets and template specializations that
   previously distinguished `Residue` from `ResidueHandle`.

Each kind's H4 migration is its own reviewed commit. D63's earlier
"single mechanical pass / add alias" framing was too optimistic; the work
is bounded and per-kind but it is a real audit.

**Why suffix-now is still lowest risk:** H1b/H2 add NEW symbols (zero v0
churn, tree stays green trivially). H3 migrates consumers cluster-by-
cluster to `*Handle` names (each its own commit + review). The H4 audit
above is per-kind and reviewable in isolation. The break ledger already
anticipates the `*`→handle return changes (Class A); D63 fixes the
transitional spelling and the (non-trivial) H4 procedure.

## D64. Handle shape: typed thin wrappers over a shared base

Each container handle is a **value type holding `{MoleculeStore* store_;
std::uint32_t idx_; std::uint64_t generation_;}`** — 8 + 4 (+4 pad) + 8 =
**24 B**, copyable, non-polymorphic. **Container handles are NOT held to
the 16 B target** — that target is for `Atom`/`Bond` (the O(100k+)
handles that are the entire D13 memory case); the molecular containers
are O(thousands) so their handle size is irrelevant (per
`V22-ARCH-HANDLE-MODEL.md`). The wider `u64 generation_` is deliberate —
it removes the per-slot wrap concern entirely (R33 LOW; see D65).

To avoid duplicating the common getters across 8 kinds:

- A **non-virtual, non-polymorphic** base `ContainerHandleBase` holds the
  triple + the kind-agnostic getters (`getKind`, `getName`, `getParent`,
  `isValid`, `operator bool`, `countChildren`, child access). No vtable.
- Eight typed handles (`ResidueHandle`, …) **publicly inherit** it and add
  ONLY kind-specific scalar getters (`ResidueHandle::getID`/
  `getInsertionCode`, `ProteinHandle::getID`,
  `SecondaryStructureHandle::getType`, `NucleotideHandle::getID`/
  `getInsertionCode`, `NucleicAcidHandle::getID`; `MoleculeHandle`/
  `ChainHandle`/`FragmentHandle` add none). They add **no data members**,
  so each stays 24 B.

**Inheritance rationale (R33 MEDIUM-1 correction):** the base is NOT
empty (it carries the 24 B triple), so this is **not** an empty-base
optimisation — EBO is irrelevant here. Inheritance is used purely for
**getter reuse**; correctness depends only on the typed handles adding no
members. This is pinned by `static_assert(sizeof(ResidueHandle) ==
sizeof(ContainerHandleBase), ...)` for every kind, plus a
`sizeof(ContainerHandleBase) == 24` pin, in the header and re-asserted in
the test. (If MSVC layout ever diverges at H7, fall back to composition +
forwarding; the static_asserts are the gate.)

**Wrong-kind construction boundary (R33 MEDIUM-2).** A raw typed ctor on
a row of the wrong kind must never silently read mismatched payload. The
locked rule:
- The public typed ctor `ResidueHandle(MoleculeStore& s, std::uint32_t
  idx)` `assert()`s (debug) that `s.container_kind_(idx) ==
  ContainerKind::RESIDUE`; in release it is UB on misuse (documented, same
  class as the existing `Atom` reference contract).
- The **safe, release-checked** path is a base→typed conversion:
  `ContainerHandleBase::as<ResidueHandle>()` (or `asResidue()`) returns a
  **null typed handle** when the row kind ≠ RESIDUE — no RTTI, just a
  `kind` tag compare. `getParent()` returns a base handle; callers narrow
  via `as<>` and get a null on mismatch. This is how downcasting works
  with zero RTTI.
- Construction in H1b: ctor captures the row's current generation; a
  default-constructed handle is the **null handle** (`store_ == nullptr`),
  `bool`-false.
- No setters in H1b (read-only; D60 forbids handle→store writes while v0
  is the source of truth). Mutation mirroring is H2.

## D65. Handle validity / generation (D54 + R31 follow-up #1)

Per **D54**, validity is checked in `BALL_DEBUG` builds AND the
`BALL_PYTHON_WRAPPER` layer; **zero cost in release C++**.

- **Per-slot generation.** `ContainerRow::generation` (present from H1a)
  is **widened to `std::uint64_t`** and becomes a **monotonic per-slot
  counter**: `ContainerTable::release()` **bumps** it (currently it resets
  to 0 — H1b fixes this so a recycled slot invalidates stale handles), and
  `allocate()` **preserves** the slot's generation on free-list reuse
  (fresh appended slots start at 0). Lifecycle: fresh=0 → release→1
  (freed) → reuse→1 (live) → release→2… The `u64` width makes wraparound a
  non-issue (R33 LOW): a single slot would need 2^64 alloc/release cycles;
  container memory is irrelevant so the wider counter costs nothing that
  matters. This is a per-slot counter, **distinct** from the `Atom`
  handle's coarse whole-store `store_generation_` (D54) — they do not
  interact; a container handle validates against its row's slot
  generation only.
- **`isValid()`** = `store_ != nullptr && idx_ < table.size() &&
  !table.is_freed(idx_) && table.row(idx_).generation == generation_`.
  In debug/Python every getter calls `assertValid_()` first → clean
  exception (`Exception::InvalidatedHandle` or existing equivalent), never
  a segfault. Release C++ elides the check (documented UB on misuse, like
  the existing `Atom` reference contract).
- **Stale/alias policy for detached containers (R31 follow-up #1):** when
  `migrate_subtree_from` releases the orphan source rows (H1a true-move),
  each released slot's generation is **bumped**. Any handle alias still
  holding `{orphan_store, old_idx, old_gen}` therefore fails the
  generation check → reported stale (debug/Python) rather than silently
  reading a recycled row. The handle that the caller passed to
  `System::insert` is **rebound** to the destination store/idx/gen by the
  insert path (H2 wiring; in H1b the test rebinds explicitly). This makes
  the "aliases can't all be rebound" hazard a detectable stale-handle
  condition, not corruption.

## D66. H1b read-only getter surface (no traversal API yet)

H1b locks the handle shape + scalar read path; the handle-yielding
iterators / `apply` / `atoms()` traversal are **H2**. The H1b surface:

- `ContainerHandleBase`: `getKind()` (→ **public** `ContainerKind`),
  `getName()` (→ `String`/`std::string`), `getParent()` (→ a base handle
  for the parent row, null at root), `getParentIndex()`, `countChildren()`,
  `getChild(i)` (→ **public** `ContainerChildRef`), `getSelectionCount()`,
  `isValid()`, `operator bool`, `operator==`/`!=` (compare store+idx+gen),
  `as<TypedHandle>()` (release-checked narrow, null on kind mismatch).
- Typed getters as in D64.
- **Not in H1b:** `atoms()`, `residues()`, `AtomIterator`-style yields,
  `apply<T>`, any mutator. Those are H2.

The getters are thin: each calls a scalar `MoleculeStore` accessor (D66a)
that loads `table.row(idx_)` and returns a public-typed value. O(1) except
`countChildren`/`getChild` which are O(1). No allocation on the scalar
path beyond the returned `std::string` for names/ids.

### D66a. Encapsulation boundary — public types + scalar accessors (R33 HIGH-2)

A *public* `containerHandle.h` must give handles read access to the
container table **without** including `_moleculeStoreInternal.h` or naming
`ContainerRow`/`ChildRef`/`ContainerTable` (the D31b CI gate forbids the
internal types in public/iterator headers). Locked solution:

1. **Move `ContainerKind` to a public header** — new
   `include/BALL/KERNEL/containerKind.h`. `_moleculeStoreInternal.h`
   `#include`s it and **stops defining its own** copy (single definition,
   no ODR/name conflict). The enum is now part of the public handle API.
2. **Introduce a public child-reference value type** `ContainerChildRef`
   (in `containerKind.h` or `containerHandle.h`): `{ bool is_atom;
   std::uint32_t idx; }` — `idx` is an atom store index when `is_atom`,
   else a container row index. The internal `ChildRef` stays internal;
   the scalar accessor converts internal→public at the boundary.
3. **Public scalar accessors on `MoleculeStore`** (declared in
   `moleculeStore.h`, which already forward-declares the side tables;
   **defined out-of-line in `moleculeStore.C`** which already includes
   `_moleculeStoreInternal.h`). Underscore-suffixed (in-tree handle
   plumbing), returning only public types:
   - `ContainerKind   container_kind_(std::uint32_t idx) const`
   - `std::string     container_name_(std::uint32_t idx) const`
   - `std::string     container_id_(std::uint32_t idx) const`
   - `char            container_insertion_code_(std::uint32_t idx) const`
   - `std::uint8_t    container_ss_type_(std::uint32_t idx) const`
   - `std::uint32_t   container_parent_(std::uint32_t idx) const`
   - `std::size_t     container_child_count_(std::uint32_t idx) const`
   - `ContainerChildRef container_child_(std::uint32_t idx, std::size_t i) const`
   - `std::uint32_t   container_selection_count_(std::uint32_t idx) const`
   - `std::uint64_t   container_generation_(std::uint32_t idx) const`
   - `bool            container_is_freed_(std::uint32_t idx) const`
   - `std::size_t     container_table_size_() const`
4. `containerHandle.h` includes `moleculeStore.h` + `containerKind.h`
   **only** — never `_moleculeStoreInternal.h`. D31b gate stays green;
   the handle header carries no internal type. The handle getters are
   one-line forwards to these accessors.

This is the same pattern as the existing atom-column accessors on
`MoleculeStore` (e.g. `position(i)`, `name(i)`), just for container rows.

## H1b deliverables + tests

1. `include/BALL/KERNEL/containerKind.h` (NEW, public): the
   `ContainerKind` enum + the `ContainerChildRef` value type.
   `_moleculeStoreInternal.h` includes it and deletes its private
   `ContainerKind`/`ChildRef`-public-surface duplication (the internal
   `ChildRef` stays internal; `ContainerKind` becomes the shared public
   definition).
2. `MoleculeStore` scalar container accessors (D66a): declared in
   `moleculeStore.h`, defined out-of-line in `moleculeStore.C`. Return
   only public types. Plus the `ContainerTable` generation fix: widen
   `ContainerRow::generation` to `u64`, bump-on-release, preserve-on-reuse.
3. `include/BALL/KERNEL/containerHandle.h` (+ `.C` if needed): the
   non-polymorphic base + 8 typed handles, read-only getters forwarding to
   the scalar accessors, `as<>()` narrow, validity (debug/Python-gated),
   `static_assert` size pins. Includes only `moleculeStore.h` +
   `containerKind.h`.
4. `ContainerHandle_test`: build a table (reuse the H1a mirror helper
   pattern), wrap rows in typed handles, assert every getter reads the row
   correctly; assert null-handle semantics; generation-based staleness
   after release/reuse and after a migrate (the R31 alias policy);
   `as<>()` narrowing (correct kind → valid, wrong kind → null);
   parent/child relationships; payload-specific getters.
5. Non-invasive: no v0 header touched; no `MoleculeStore`/`Atom` ABI
   change (the scalar accessors are additive methods); CI grep gates
   intact — `containerHandle.h` never includes `_moleculeStoreInternal.h`
   (resolved in D66a). +1 test (ContainerHandle_test) → 286 ctest.

## Decisions added

| # | Contract |
|---|---|
| D63 | Dual-existence naming: `*Handle` suffix in `BALL::`; H4 is a per-kind canonical-name migration audit (delete v0 class+fwd-decls+friends, rename `*Handle`→canonical, add reverse alias), not a one-line `using` |
| D64 | Handle shape: 24 B `{store,u32 idx,u64 gen}` typed thin wrappers over a non-polymorphic base (NOT EBO — getter reuse; size pinned by static_assert); wrong-kind narrow via release-checked `as<>()`, debug-assert in raw ctor |
| D65 | Per-slot `u64` generation (bump-on-release, preserve-on-reuse) + debug/Python validity; detached-alias staleness is detectable; no wrap concern |
| D66 | H1b read-only getter surface; traversal/iterators deferred to H2 |
| D66a | Encapsulation: public `ContainerKind` header + public `ContainerChildRef` value type + scalar `MoleculeStore` accessors (defined out-of-line); `containerHandle.h` never includes `_moleculeStoreInternal.h` — D31b gate intact |

## Resolved open question (was: encapsulation boundary)

**Resolved in D66a (R33 HIGH-2):** option (a) — public `ContainerKind`
header + public `ContainerChildRef` type + scalar `MoleculeStore`
accessors defined out-of-line in `moleculeStore.C`. `containerHandle.h`
includes only `moleculeStore.h` + `containerKind.h`, never the internal
header. `ContainerKind` moves to the public header and the internal header
includes (no longer redefines) it — single definition, no ODR conflict.

## Next action

R33 returned NEEDS-FIXES (D63 H4-alias transparency + D66 boundary); this
revision adds the H4 migration-audit procedure (D63), the public-type +
scalar-accessor boundary (D66a), the 24 B/`u64`-generation + wrong-kind
narrow + static_assert pins (D64/D65), and the generation-wrap resolution
(D65). Codex **R33b** re-reviews; on GO, implement H1b.
