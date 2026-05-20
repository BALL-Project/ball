# BALL v2.2 - Codex Review Round 33 (H1b Planning Gate)

**Status:** NEEDS-FIXES  
**Verdict:** Do not implement H1b until D63/D66 are tightened.  
**Reviewer:** Codex CLI 0.128.0  
**Subject:** H1b container value-handle types over the H1a `ContainerTable`

## Probe Answers

### P33-1. D63 naming (`*Handle` suffix now, alias to canonical at H4)

Suffixing the H1b types is the right low-churn choice for dual existence: the canonical names are still occupied by v0 classes through H1-H3 (`V22-H1b-DESIGN.md:27-39`), and renaming the live v0 hierarchy now would churn the public API before the handle path is consumed (`V22-H1b-DESIGN.md:52-55`). A `BALL::v2::Residue` namespace would not actually make use sites uniform because `Atom` remains canonical in `BALL::` (`V22-H1b-DESIGN.md:44-50`).

But the H4 alias claim is overstated. `using Residue = ResidueHandle;` restores many value uses, return types, and parameter types, but it does not transparently restore all canonical-name patterns. Existing forward declarations such as `class Residue;` in public headers (`include/BALL/KERNEL/atom.h:44-54`, `include/BALL/KERNEL/predicate.h:28`) cannot coexist with an alias of the same name to a different class. Overloads or specializations that separately mention `Residue` and `ResidueHandle` collapse to the same type after H4. Friend declarations using `friend class Residue;` are declarations of a class, not an alias. Out-of-class constructor/destructor definitions are also not made source-equivalent by a typedef-style alias.

This does not invalidate H1b, but D63 needs to say H4 is not just "add alias"; it is a canonical-name migration pass: remove/replace forward declarations, audit overload sets and template specializations, then either keep `ResidueHandle` as a compatibility alias to a real canonical `Residue` handle class or accept that canonical `Residue` remains an alias with known source limits. The current "single mechanical pass" language (`V22-H1b-DESIGN.md:57-63`) is too optimistic.

The H3 mixed spelling (`Atom`/`Bond` canonical, `ResidueHandle` suffixed) is an inconsistency hazard, but it is tolerable if it is explicitly transitional and tests/examples consistently use suffixed container handles. The bigger risk is H4 source compatibility, not H3 aesthetics.

### P33-2. D64 handle shape and non-polymorphic inheritance

The value shape is coherent and matches the existing Atom direction: Atom carries `MoleculeStore*`, a store slot, and a store generation (`include/BALL/KERNEL/atom.h:1013-1023`), and `MoleculeStore` exposes the coarse generation used by that model (`include/BALL/KERNEL/moleculeStore.h:403-408`). Container handles using `{store*, container_idx, generation}` are the right analogue.

Non-polymorphic public inheritance is acceptable for values if the base is intentionally usable as a kind-agnostic handle and the derived classes add no data. There is no vtable and ordinary slicing to `ContainerHandleBase` is a feature, not accidental, if equality/validity are defined on the triple. However, the design's "EBO-friendly" wording is wrong: `ContainerHandleBase` is not empty; the size target relies on "derived adds no fields", not empty-base optimization (`V22-H1b-DESIGN.md:72-89`). Add explicit `static_assert(sizeof(ContainerHandleBase) == 16)` and one `static_assert` per typed handle. Do not defer this to H7/MSVC; H1b promises 16 B (`V22-H1b-DESIGN.md:67-69`).

The debug-only `assertKind_()` is acceptable only under the already-accepted D54 precedent that release C++ treats stale/wrong handles as misuse (`V22-H1b-DESIGN.md:97-113`). It is not a memory-safety guarantee: a wrong-kind `ResidueHandle` over a `Protein` row will read the shared fields correctly and may read meaningless payload in release. That needs to be documented as UB on forged/wrong-kind construction, with typed constructors either private/friend-only or explicitly marked low-level/test-only for H1b.

### P33-3. D65 validity/generation

The generation fix is necessary. Current `ContainerTable::release()` resets the row to a default `ContainerRow`, which resets `generation` to zero, and `allocate()` overwrites a reused slot with a default row (`source/KERNEL/moleculeStore.C:304-319`, `source/KERNEL/moleculeStore.C:326-332`). That cannot invalidate stale aliases if a slot is recycled. D65's bump-on-release and preserve-on-reuse contract (`V22-H1b-DESIGN.md:102-109`) closes the exact stale-alias hole from H1a migration.

The true-move migration path is consistent with D65: `migrate_subtree_from()` copies into the destination, sets the new root detached, then releases the source subtree (`source/KERNEL/moleculeStore.C:535-547`); source release walks children and frees rows post-order (`source/KERNEL/moleculeStore.C:522-532`). Once `release()` bumps generation, aliases still pointing at the orphan store fail validity rather than reading a freed/reused source row, which matches the plan (`V22-H1b-DESIGN.md:114-123`).

The per-slot container generation is distinct from Atom's coarse store generation: Atom stores `store_generation_` as a `uint64_t` snapshot of `MoleculeStore::generation()` (`include/BALL/KERNEL/atom.h:1020-1023`, `source/KERNEL/atom.C:46-68`), while container handles compare against `ContainerRow::generation` (`include/BALL/KERNEL/_moleculeStoreInternal.h:503-509`). There is no semantic conflict, but the design should explicitly name these `container_generation_` vs `store_generation_` to avoid D54 confusion.

`uint32_t` generation is probably acceptable for container slots because container churn is low, but wraparound must be acknowledged. A stale handle can become valid after 2^32 release/reuse cycles of the same slot. The plan should either document "accepted as impossible in practice for container rows" or use `uint64_t` for parity with Atom's coarse generation.

Debug/Python-only validity is acceptable because D54 already locks that policy (`V22-ARCH-HANDLE-MODEL.md:192-194`, `V22-H1b-DESIGN.md:99-113`). H1b should still make `isValid()` itself available in release; only implicit getter guards should be compiled out.

### P33-4. D66 read-only surface

Deferring traversal/iterators to H2 is the right phase cut. H1b can still be tested with `countChildren()` and indexed `getChild(i)` over hand-built rows, without committing the final iterator API (`V22-H1b-DESIGN.md:125-143`). That is enough to exercise scalar reads, parent links, child edge exposure, validity, and kind assertions.

`getParent()` returning `ContainerHandleBase` is coherent: the parent is kind-tagged data, and the architecture already says generic container resolution should use the row kind tag rather than RTTI (`V22-ARCH-HANDLE-MODEL.md:76-90`). H1b does not need downcast or RTTI if it also provides `getKind()` and, later, a visitor/factory in H2.

`getChild(i)` is underspecified. Returning "a small `ChildRef`-shaped result" (`V22-H1b-DESIGN.md:130-135`) is fine only if that result is a public API type, not the private `ChildRef` from `_moleculeStoreInternal.h`. H1b needs a public `ContainerChildRef`/`ContainerChild` value type with `{child_kind, idx}` or `{isAtom, Atom/container base handle}` semantics. Otherwise the public header either leaks private internals or cannot expose child access at all.

### P33-5. Encapsulation boundary

Option (a), scalar accessors on `MoleculeStore`, is the right direction, but it is not complete as written. The internal types currently live in `_moleculeStoreInternal.h`: `ContainerKind` (`include/BALL/KERNEL/_moleculeStoreInternal.h:442-456`), `ChildRef` (`include/BALL/KERNEL/_moleculeStoreInternal.h:458-475`), `ContainerRow` (`include/BALL/KERNEL/_moleculeStoreInternal.h:497-510`), and `ContainerTable` (`include/BALL/KERNEL/_moleculeStoreInternal.h:512-548`). The public handle header must not include that file; the internal header explicitly documents a narrow consumer allowlist and the CI boundary rationale (`include/BALL/KERNEL/_moleculeStoreInternal.h:4-18`), while the workflow enforces the existing public/iterator leak gate (`.github/workflows/ci-v2.yml:98-117`).

If public `getKind()` returns `ContainerKind`, that enum must move to a public header. That is not a problem; a stable public enum is part of the handle API. But the internal header must then stop defining a second `ContainerKind`, or it will create type mismatch and ODR/name conflicts. Likewise, `getChild(i)` requires a public child-reference type because exposing the private `ChildRef` would break the boundary. The scalar accessor set must be designed around public types only, for example:

- `ContainerKind MoleculeStore::container_kind_(uint32_t) const`
- `String/std::string MoleculeStore::container_name_(uint32_t) const`
- `uint32_t MoleculeStore::container_parent_(uint32_t) const`
- `uint32_t MoleculeStore::container_generation_(uint32_t) const`
- `bool MoleculeStore::container_is_freed_(uint32_t) const`
- `std::size_t MoleculeStore::container_child_count_(uint32_t) const`
- `ContainerChildRef MoleculeStore::container_child_(uint32_t, std::size_t) const`
- payload-specific scalar accessors for id, insertion code, and secondary-structure type

These should be declared in public headers and defined out-of-line in a `.C` file that includes `_moleculeStoreInternal.h`, preserving the PImpl boundary (`include/BALL/KERNEL/moleculeStore.h:571-587`). The current design leaves this as an open question (`V22-H1b-DESIGN.md:174-187`), so H1b is not locked.

### P33-6. Dual-existence coherence

In H1b, valid handles can only be obtained from manually built table rows or explicit test helper construction, because the v0 tree remains source of truth and automatic v0-to-table mirroring is H2 (`V22-H0-ADDENDUM.md:135-159`, `V22-H0-ADDENDUM.md:212-216`). That is meaningful enough for H1b: it tests the handle representation and read path against a table data structure, just as H1a tests manually mirrored rows (`test/HierarchyParity_test.C:6-17`).

It is self-referential only if tests merely assert "wrote X, read X" for one row. H1b tests should also cover wrong-kind construction, parent/child relationships, migration stale aliases, free-list reuse, null handles, and payload-specific getters. That makes the handle layer nontrivial without pretending to validate H2 mirror wiring.

There must be no public H1b path that constructs a handle from a v0 object, because many v0 objects still have no table row. Existing v0 APIs still return pointers such as `Atom::getResidue()` (`include/BALL/KERNEL/atom.h:380-389`) and `Protein::getResidue()` (`source/KERNEL/protein.C:123-151`). H1b constructors should accept `(MoleculeStore&, idx)` only, and any future v0-object-to-handle adapter belongs in H2 after mirror wiring exists. If a lookup has no row, return a null handle, not a forged `{store,0,0}` with accidental sentinel reads.

### P33-7. Scope/sequencing

The intended scope is bounded: new handle symbols, the generation fix, scalar read accessors, and tests. No v0 public API needs to change in H1b (`V22-H1b-DESIGN.md:145-163`). Traversal, mutation mirroring, `apply`, consumer migration, and property migration are correctly left to H2/H3 (`V22-H0-ADDENDUM.md:212-220`).

The hidden dependency is the unresolved public accessor/type boundary, not traversal. H1b is testable without H2 if D66 is narrowed to scalar/indexed reads over a manually built table. H1b is not implementable cleanly if the public header must choose ad hoc between leaking `_moleculeStoreInternal.h` and inventing public API types during coding.

### P33-8. Overall

NEEDS-FIXES. The H1b concept is sound, but the design is not locked because D63 overpromises H4 alias transparency and D66 leaves the public/private type boundary unresolved. Those are planning problems, not coding details.

## New Findings

### HIGH - H4 aliasing is not transparent for forward declarations, overloads, friends, and specializations

`V22-H1b-DESIGN.md:37-42` says canonical names are restored with aliases such as `using Residue = ResidueHandle;`. That works for many ordinary uses but fails or changes meaning for important C++ patterns already present in the tree. Public headers forward-declare v0 container classes (`include/BALL/KERNEL/atom.h:44-54`, `include/BALL/KERNEL/predicate.h:28`), APIs return `Residue*` today (`include/BALL/KERNEL/atom.h:380-389`), and templates/specialized container helpers are keyed on `Residue` (`include/BALL/KERNEL/residue.h:460-489`). After H4, `class Residue;` cannot coexist with `using Residue = ResidueHandle;`, `Residue` and `ResidueHandle` overloads collapse, and `friend class Residue;` is not an alias-friendly spelling.

Fix: revise D63 to require an H4 canonical-name migration audit. Either make the real handle class eventually be named `Residue` and keep `using ResidueHandle = Residue` as compatibility, or document that `Residue` as an alias has source-compatibility limits and requires removal/replacement of every forward declaration, friend class declaration, overload conflict, and duplicate specialization.

### HIGH - Public handle API cannot return private `ContainerKind` / `ChildRef` types

D66 exposes `getKind() -> ContainerKind` and `getChild(i)` as a `ChildRef`-shaped result (`V22-H1b-DESIGN.md:130-135`), but those types currently live in the private internal header (`include/BALL/KERNEL/_moleculeStoreInternal.h:442-475`). The design explicitly says the public handle header must not leak `ContainerRow`/`ChildRef`/`ContainerTable` and leaves the solution open (`V22-H1b-DESIGN.md:156-163`, `V22-H1b-DESIGN.md:174-187`). That is not a locked design.

Fix: move `ContainerKind` to a public API header, introduce a public child-reference value type, and declare only scalar/public-type `MoleculeStore` accessors in public headers. Define those accessors out-of-line in a `.C` that includes `_moleculeStoreInternal.h`.

### MEDIUM - D64's size rationale cites EBO for a non-empty base and defers required layout checks

`ContainerHandleBase` stores `{store, idx, generation}` and is therefore not empty (`V22-H1b-DESIGN.md:67-75`). Public inheritance can still keep derived handles at 16 B if derived classes add no fields, but that is not EBO. The 16 B target is a design gate (`V22-H1b-DESIGN.md:67-69`), not something to discover at H7 (`V22-H1b-DESIGN.md:84-89`).

Fix: require H1b `static_assert`s for base and all typed handle sizes, including MSVC when that CI is available. Correct the rationale from EBO to "derived adds no non-static data members".

### MEDIUM - Wrong-kind typed handles are UB in release but the construction boundary is not locked

Typed handles rely on debug-only `assertKind_()` (`V22-H1b-DESIGN.md:76-82`) while D54 elides getter validity checks in release (`V22-H1b-DESIGN.md:97-113`). A forged `ResidueHandle(store, protein_idx)` can read incorrect payload in release. That may be acceptable under the Atom precedent, but only if the design constrains construction.

Fix: mark raw `(store, idx)` typed constructors as internal/test-only or route construction through kind-checked factories in debug/Python. Document release C++ wrong-kind construction as UB.

### LOW - `uint32_t` per-slot generation wraparound is unaddressed

D65 uses `ContainerRow::generation` as `uint32_t` (`include/BALL/KERNEL/_moleculeStoreInternal.h:503-509`, `V22-H1b-DESIGN.md:102-109`). After 2^32 release/reuse cycles of the same slot, a stale handle can become apparently valid. This is probably not practical for low-volume container rows, but the design should state the accepted risk or use `uint64_t`.

Fix: document the accepted wrap risk for containers or widen the field before public handles depend on it.

## Overall Verdict

**NEEDS-FIXES.**

Precise fix list before H1b implementation:

1. Revise D63: H4 aliasing is not transparent; add an explicit canonical-name migration plan for forward declarations, friends, overloads, and template specializations.
2. Resolve D66 option (a) concretely: public `ContainerKind`, public child-ref/result type, and public scalar `MoleculeStore` accessors defined out-of-line without including `_moleculeStoreInternal.h` from `containerHandle.h`.
3. Add H1b layout requirements: `static_assert(sizeof(ContainerHandleBase) == 16)` and per typed handle.
4. Specify typed-constructor safety: raw `(store, idx)` construction is internal/test-only or factory-checked; wrong-kind release use is UB under D54.
5. Decide/document `uint32_t` generation wrap policy.

After those edits, the bounded H1b implementation can proceed without needing H2 traversal or v0 mirror wiring.
