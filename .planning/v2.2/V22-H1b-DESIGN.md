# BALL v2.2 — H1b Design: container value-handle types (dual existence)

**Status:** design lock (R33 review pending).
**Authored:** 2026-05-20, post-H1a (R32b GO).
**Companion:** `V22-H0-ADDENDUM.md` (D56–D62), `V22-ARCH-HANDLE-MODEL.md`,
`V22-API-BREAK-LEDGER.md`, `V22-DECISIONS.md`.
**Phase:** H1b — "Atom/Bond/container value-handle TYPES reading from the
store, alongside v0 objects (dual existence; v0 = source of truth per
D60)."

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
`NucleotideHandle`, `NucleicAcidHandle`, `FragmentHandle`. At **H4 (the
flip)** the v0 classes are deleted and each canonical name is restored as
an alias of its handle (`using Residue = ResidueHandle;`) — or the handle
is renamed and the alias retired. H3 consumer code written against
`ResidueHandle` keeps compiling through the alias; new/canonical code uses
`Residue`.

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

**Why suffix-then-alias is lowest risk:** H1b/H2 add NEW symbols (zero v0
churn, tree stays green trivially). H3 migrates consumers cluster-by-
cluster to `*Handle` names (each its own commit + review). H4's rename is
a single mechanical pass (delete v0 class; add `using` alias) per kind,
reviewable in isolation. The break ledger already anticipates the
`*`→handle return changes (Class A); D63 just fixes the transitional
spelling.

## D64. Handle shape: typed thin wrappers over a shared base

Each container handle is a **value type, `{MoleculeStore* store_;
std::uint32_t idx_; std::uint32_t generation_;}` = 16 B**, copyable, no
base classes (mirrors the `Atom` handle's `{store,idx,gen}` triple). To
avoid duplicating the common getters across 8 kinds:

- A non-virtual base `ContainerHandleBase` holds the triple + the
  kind-agnostic getters (`getKind`, `getName`, `getParent`, `isValid`,
  `operator bool`, `countChildren`, child access). It is **not**
  polymorphic — no vtable, EBO-friendly, 16 B.
- Eight typed handles (`ResidueHandle`, …) derive from it (or contain it;
  see below) and add only the kind-specific scalar getters
  (`ResidueHandle::getID`/`getInsertionCode`, `ProteinHandle::getID`,
  `SecondaryStructureHandle::getType`, `NucleotideHandle::getID`/
  `getInsertionCode`, `NucleicAcidHandle::getID`; `Molecule`/`Chain`/
  `Fragment` add none). A debug-only `assertKind_()` in each typed ctor/
  getter guards that the row's `kind` matches the handle type.

**Composition vs inheritance:** use **public inheritance from a
non-polymorphic `ContainerHandleBase`** (empty-base-optimisable, no
vtable). This gives `ResidueHandle` the base getters directly and lets a
typed handle slice to the base for kind-agnostic code, without the
`Composite` machinery. (If MSVC EBO/sizeof proves troublesome at H7,
fall back to composition + forwarding; the 16 B target is the gate.)

- Construction in H1b: `ResidueHandle(MoleculeStore& s, std::uint32_t
  idx)` captures the row's current generation. A default-constructed
  handle is the **null handle** (`store_ == nullptr`), `bool`-false.
- No setters in H1b (read-only; D60 forbids handle→store writes while v0
  is the source of truth). Mutation mirroring is H2.

## D65. Handle validity / generation (D54 + R31 follow-up #1)

Per **D54**, validity is checked in `BALL_DEBUG` builds AND the
`BALL_PYTHON_WRAPPER` layer; **zero cost in release C++**.

- **Per-slot generation.** `ContainerRow::generation` (already present
  from H1a) becomes a **monotonic per-slot counter**: `ContainerTable::
  release()` **bumps** it (currently it resets to 0 — H1b fixes this so a
  recycled slot invalidates stale handles), and `allocate()` **preserves**
  the slot's generation on free-list reuse (fresh appended slots start at
  0). Lifecycle: fresh=0 → release→1 (freed) → reuse→1 (live) → release→2…
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

- `ContainerHandleBase`: `getKind()` (→ `ContainerKind`), `getName()`
  (→ String/std::string from the table pool), `getParent()` (→ a base
  handle for the parent row, null at root), `getParentIndex()`,
  `countChildren()` (size of the `ChildRef` vector), `getChild(i)`
  (→ a small `ChildRef`-shaped result the caller can resolve),
  `getSelectionCount()`, `isValid()`, `operator bool`, `operator==`/`!=`
  (compare store+idx+gen).
- Typed getters as in D64.
- **Not in H1b:** `atoms()`, `residues()`, `AtomIterator`-style yields,
  `apply<T>`, any mutator. Those are H2.

The getters are thin: each loads `table.row(idx_)` and reads a field /
interns-back a string. O(1) except `getChild`/`countChildren` which are
O(1)/O(deg). No allocation on the scalar path.

## H1b deliverables + tests

1. `include/BALL/KERNEL/containerHandle.h` (+ `.C` if needed): the base +
   8 typed handles, read-only getters, validity (debug/Python-gated).
2. `ContainerTable` generation fix: bump-on-release, preserve-on-reuse;
   add `generation(idx)` accessor for `isValid`.
3. `ContainerHandle_test`: build a table (reuse the H1a mirror helper
   pattern), wrap rows in typed handles, assert every getter reads the
   row correctly; assert null-handle semantics; assert generation-based
   staleness after release/reuse and after a migrate (the R31 alias
   policy); assert `assertKind_` fires in debug on a wrong-kind wrap.
4. Non-invasive: no v0 header touched except (if needed) a forward decl;
   no `MoleculeStore`/`Atom` ABI change; CI grep gates unaffected (the
   handle header does NOT include `_moleculeStoreInternal.h` publicly —
   it needs container-table reads, so it either (a) routes through a thin
   `MoleculeStore` accessor that returns scalar values, or (b) is itself
   a documented internal-header consumer like the parity test). **Open
   sub-question for R33:** which of (a)/(b) keeps the D31b encapsulation
   gate intact — a public handle header must NOT leak the internal types.

## Decisions added

| # | Contract |
|---|---|
| D63 | Dual-existence naming: `*Handle` suffix in `BALL::`, aliased to canonical at H4 |
| D64 | Handle shape: 16 B `{store,idx,gen}` typed thin wrappers over a non-polymorphic base |
| D65 | Per-slot generation (bump-on-release) + debug/Python validity; detached-alias staleness is detectable |
| D66 | H1b read-only getter surface; traversal/iterators deferred to H2 |

## Key open question for R33

The **encapsulation boundary** (D66 deliverable #4): a *public*
`containerHandle.h` must give handles read access to `ContainerTable`
rows WITHOUT leaking `ContainerRow`/`ChildRef`/`ContainerTable` into a
public header (the D31b CI gate forbids the internal types in public/
iterator headers). Options: (a) add scalar-returning accessors on
`MoleculeStore` (e.g. `container_kind(idx)`, `container_name(idx)`,
`container_parent(idx)`, `container_child(idx,i)`) that the handle calls —
keeps the handle header free of internal types; (b) make the handle types
themselves internal-header consumers (not public) until H3. R33 should
confirm (a) is the right call and that the scalar-accessor surface on
`MoleculeStore` is acceptable (it is the same pattern as the existing
atom-column accessors).

## Next action

Codex **R33** reviews this H1b design (is the dual-existence naming sound,
is the handle shape/validity correct, is the encapsulation boundary
resolved, is H1b a bounded non-invasive step?). On GO, implement H1b.
