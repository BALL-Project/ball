# Track A v2.1 Cluster R19 — Codex CLI Round 19 (2026-05-19)

**Status:** Complete
**Verdict:** NEEDS-FIXES
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.1 P1 close review — side-table infrastructure

## Commits reviewed

- `173374553` — P1.1 `BALL_EMPTY_BASES` macro
- `14f9fdb1d` — P1.2 PImpl side-table storage
- `39fb54415` — P1.3 Composite handle infrastructure
- `155a3e057` — P1.4/P1.5/P1.7/P1.9 property columns, sparse bag, selected bits, iterator stability

## Phase gate verification

| P1 gate criterion | Status |
|---|---|
| P1 branch is linear from `v2.0.0` and contains the stated P1 commits | PASS |
| `MoleculeStoreSideTables` PImpl exists and owns `composite_nodes_`, property columns, sparse bag, and selected bits | PASS |
| `CompositeHandle` and `CompositeNode` size pins hold on Apple Clang arm64 | PASS: `sizeof(CompositeHandle) == 8`, `sizeof(CompositeNode) == 48` at `_moleculeStoreInternal.h:77` and `_moleculeStoreInternal.h:103`; also tested in `SideTableParity_test.C:388` |
| P1 defers mutation wiring to P2 | PASS: `Composite::getNode_()` still reads v2.0 inline topology at `source/CONCEPT/composite.C:62` |
| Targeted regression tests pass locally | PASS: `MoleculeStore_test`, `MoleculeStoreJson_test`, `SideTableParity_test`, `CompositeIteratorStability_test` passed locally |
| D31b CI grep gate is wired | FAIL: D31b requires a grep gate at `V21-DECISIONS.md:838`, but `.github/workflows/ci-v2.yml` has no such step |
| D34b Windows/MSVC P1-close job is wired | FAIL: D34b says add Windows GHA in the P1 close commit at `V21-DECISIONS.md:903`; current `ci-v2.yml` is still macOS-only and targets `v2.0` at `.github/workflows/ci-v2.yml:24` and `.github/workflows/ci-v2.yml:31` |

## Probe answers

| # | Probe | Verdict | Notes |
|---|---|---|---|
| P19-1 | `CompositeHandle` opaque-slot memcpy | OK with small hardening | `std::memcpy` between trivially copyable object representations is strict-aliasing safe; no typed aliasing occurs at `source/CONCEPT/composite.C:40`. Actual layout is compatible: `CompositeHandle` is 8 B at `_moleculeStoreInternal.h:77`; `std::uint64_t composite_handle_packed_` is the storage slot at `composite.h:1625`. Add `static_assert(std::is_trivially_copyable<CompositeHandle>::value)` and `static_assert(alignof(CompositeHandle) <= alignof(std::uint64_t))`; current code only asserts size at `composite.C:37`. |
| P19-2 | D31b grep gate not wired | NEEDS-FIXES | This should ship at P1 close, not P2. D31b explicitly claims structural enforcement by CI at `V21-DECISIONS.md:838`; P2 is exactly when iterator-adjacent drift becomes dangerous. Current workflow is still named v2.0 and lacks the gate. |
| P19-3 | `PropertyColumn<T>::set()` resize ordering | OK | For `set(100)` on a fresh column: `ensureCapacity_(101)` sets base `size_ = 101` and grows `present_` to 13 bytes, i.e. capacity for 104 bits (`_moleculeStoreInternal.h:190`). Then `data_.resize(101)` runs at `_moleculeStoreInternal.h:219`; `setPresence_(100)` sees capacity already sufficient and sets bit 100 at `_moleculeStoreInternal.h:169`. Final state: base size 101, data size 101, presence bitmap 13 bytes/104 bits. |
| P19-4 | `StringPropertyColumn` pool reallocation | OK now, document future hazard | Current `get()` returns a fresh `std::string` copy at `_moleculeStoreInternal.h:268`; no current API exposes `pool_.c_str() + offset`. Offsets stay valid across `pool_.append()` at `_moleculeStoreInternal.h:295`. Add a comment if a future raw-view accessor is introduced: any `const char*` view is invalidated by subsequent `set()`/intern growth. |
| P19-5 | `promote_sparse_` erase iteration | OK | A two-name same-atom bucket behaves correctly. Example `[A, B]`: promoting A erases only A in the inner loop at `moleculeStore.C:225`, leaving `[B]`; cleanup at `moleculeStore.C:230` keeps the non-empty bucket. Promoting B later sees `[B]`, erases it, then cleanup removes the empty atom bucket. No half-processed state confuses sequential promotion. |
| P19-6 | `selected_bits_` resize race enforcement | DEBT for P2 | The exclusive resize contract is documented at `_moleculeStoreInternal.h:542` and `moleculeStore.C:238`, but not mechanically enforced. P1 tests call `resize_selected_bits_()` without a lock at `SideTableParity_test.C:355`. Before P2 wiring, route all resize calls through store-owned exclusive paths or add a debug owner/token assertion; `set_selected_()` itself asserts capacity only at `moleculeStore.C:270`. |
| P19-7 | D31 iterator test coverage | PARTIAL | The test covers insert during child iteration, manual backward child iteration, non-current removal, and basic preorder at `CompositeIteratorStability_test.C:37`, `:70`, `:91`, and `:117`. It misses `splice`/`spliceBefore`/`spliceAfter` during iteration, `clear`/`destroyChildren_` invalidation behavior, and actual `std::reverse_iterator` wrappers (`rbeginChildComposite()` / `CompositeReverseIterator`) declared at `composite.h:1217` and `composite.h:1501`. Add those before or during P2. |
| P19-8 | Zero-atom memory accounting | OK, clarify fixed cost | D22b's 48 B/atom node math is the amortized node-table term, not the fixed per-store cost. On Apple Clang/libc++ arm64, a diagnostic allocation probe measured `new MoleculeStoreSideTables()` at 37 allocations, 2,456 requested heap bytes, 2,592 malloc-rounded bytes. Add the 8 B `MoleculeStore::side_tables_` pointer at `moleculeStore.h:563`: fixed P1 overhead is about 2,464 requested bytes, about 2,600 allocator-rounded bytes per zero-atom store. |
| P19-9 | MSVC compile/layout risk | NEEDS-FIXES gate | `BALL_EMPTY_BASES` is defined at `macros.h:404` but not yet applied to `Atom` or `Bond` (`atom.h:96`, `bond.h:54`), which is acceptable for P1 only if the P1-close Windows job exists. It does not. The new 8 B `Composite` slot at `composite.h:1625` is transient but can still shift MSVC layout differently; D34b says to catch that before P2 via Windows CI. |
| P19-10 | Public API impact | NEEDS CLASSIFICATION | `getCompositeHandle_`, `setCompositeHandle_`, and `getNode_` are public at `composite.h:1635`. The incomplete-type gate reduces accidental use, but any external caller can include `_moleculeStoreInternal.h`. Treat these as internal/experimental and explicitly exclude them from v2.1 stable API promises, or move to friend/narrower access before release. |

## New findings

### R19-F1 — NEEDS-FIXES: P1-close CI guardrails are not wired

D31b and D34b are P1-close guardrails for the P2 thin-handle flip, but the current workflow is still the v2.0 macOS-only workflow. It targets `v2.0` pushes/PRs at `.github/workflows/ci-v2.yml:24` and `.github/workflows/ci-v2.yml:31`, has no D31b grep gate, and has no Windows/MSVC job. This conflicts with D31b's required CI grep gate at `V21-DECISIONS.md:838` and D34b's P1-close Windows job requirement at `V21-DECISIONS.md:903`.

P2 should not start until `ci-v2.yml` is updated for `v2.1`, adds the D31b leak scan, and adds the Windows VS 2022 CORE_ONLY job with `SideTableParity_test` and `Sizeof_test`.

### R19-F2 — DEBT: Iterator-stability coverage is not broad enough for the P2 flip

`CompositeIteratorStability_test` is useful but incomplete. It does not exercise splice mutations (`source/CONCEPT/composite.C:946`, `:1003`, `:1060`), subtree clear/destroy behavior (`source/CONCEPT/composite.C:498`), or the `std::reverse_iterator` wrappers declared at `include/BALL/CONCEPT/composite.h:1217` and `:1501`.

This does not invalidate P1 infrastructure, but P2 should expand this test before relying on side-table topology reads in iterator code.

### R19-F3 — DEBT: Internal public accessors need an explicit API classification

`Composite::getCompositeHandle_`, `setCompositeHandle_`, and `getNode_` are public at `include/BALL/CONCEPT/composite.h:1635`. The underscore names and incomplete-type requirement are useful friction, but this is still exported public surface. The v2.1 release notes or API docs should mark these as internal/experimental and not stable for v2.2, or the implementation should narrow access before the v2.1.0 API freeze.

## Overall verdict

**NEEDS-FIXES before P2.** The side-table primitives are broadly sound for P1: handle packing is safe with minor static-assert hardening, property-column sizing is synchronized, sparse promotion is correct, and targeted tests pass. The blocker is the missing P1-close enforcement layer: D31b grep and D34b Windows/MSVC CI are promised specifically to protect the P2 flip.

Recommended P2 entry checklist:

- Update `.github/workflows/ci-v2.yml` to target `v2.1`.
- Add the D31b grep/static gate for `CompositeNode*` / `composite_nodes_` leakage into public iterator headers.
- Add the Windows VS 2022 CORE_ONLY job required by D34b.
- Add `CompositeHandle` trivially-copyable and alignment static asserts.
- Expand iterator-stability tests for splice, clear/destroy, and reverse-wrapper paths.
- Classify the new public underscore accessors as internal/experimental before the release API freeze.
