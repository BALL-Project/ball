# Track A v2.1 Cluster R17b — Codex CLI Round 17b (2026-05-19)

**Status:** NEEDS-FIXES
**Verdict:** NO-GO
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.1 P0 design lock revised — D22a-D34
**Predecessor:** R17 (NO-GO; 3 BLOCKERs + 4 BUGs + 3 DEBTs)

## R17 finding closure status

| R17 finding | Severity | Revision | Status |
|---|---|---|---|
| P17-1 CompositeNode underspecified | BLOCKER | D22a | PARTIAL. Five topology links are present, but the sizing/savings claim is not correct enough for the memory budget. |
| P17-2 Property registry unbounded | BUG | D23a | PARTIAL. Dense-column bomb is capped, but sparse fallback/name pool remain unbounded. |
| P17-3 String column sizing | BUG | D23a | CLOSED. D23a uses `uint32_t` intern offsets, not dense `String` objects. |
| P17-4 BitVector race | BLOCKER | D24a | PARTIAL/REGRESSED. Relaxed atomic word access is data-race-free, but `vector<atomic<u64>>` is not compatible with normal vector reallocation/shrink operations. |
| P17-5 MSVC EBO | BUG | D26a/D34 | CLOSED. Macro shape is correct; CI gate timing should move earlier than final tag. |
| P17-6 Bond identity | BUG | Deferred | OUT OF SCOPE for R17b per instruction. |
| P17-7 Iterator invariants | DEBT | D31 | PARTIAL. Documented and test-planned, but not enforced. |
| P17-8 Backport policy | DEBT | D32 | PARTIAL. Thin-handle-only cases are covered; v2.0.x-regression cases are not. |
| P17-9 Review cadence | DEBT | D33 | PARTIAL. P6 close-only is fine; P5 should keep planning review because it adds public store iteration API/perf gates. |
| P17-10 v2.0 JSON demux | BUG | D30a | MOSTLY CLOSED. The mixed-file read path works as written if dense columns have a presence bitmap and sparse override semantics are fixed. |

## Probe answers

| # | Probe | Verdict | Notes |
|---|---|---|---|
| P17b-1 | D22a 5-link node math | BUG | 48 B/node is internally consistent: 5 handles = 40 B, `uint32_t child_count` + `uint8_t kind` + padding = 48 B ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:400)). 48 B x 100k = 4.8 MB. The "saves ~31 MB" claim is not supported as written ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:414)): current `Composite` private state includes 5 pointers, 3 `Size` counters, 2 bool/byte flags, and 2 `TimeStamp`s ([composite.h](/Users/kohlbach/Claude/BALL2/include/BALL/CONCEPT/composite.h:1585)); `TimeStamp` contains a `PreciseTime` object ([timeStamp.h](/Users/kohlbach/Claude/BALL2/include/BALL/CONCEPT/timeStamp.h:270)), and `PreciseTime` stores two longs plus a vptr-bearing class body ([timeStamp.h](/Users/kohlbach/Claude/BALL2/include/BALL/CONCEPT/timeStamp.h:37), [timeStamp.h](/Users/kohlbach/Claude/BALL2/include/BALL/CONCEPT/timeStamp.h:157)). If the comparison is whole Atom handle shrink, current `sizeof(Atom)=360` and target is 32, so gross savings are 32.8 MB/100k and net after the side table is ~28.0 MB, not ~31 MB ([Sizeof_test.C](/Users/kohlbach/Claude/BALL2/test/Sizeof_test.C:38), [MILESTONE-v2.1-KICKOFF.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/MILESTONE-v2.1-KICKOFF.md:139)). If the comparison is only displaced Composite topology/state, the doc must itemize the exact bytes moved and where stamps/selection counters live. |
| P17b-2 | D23a registry bomb threat model | BUG | The 400 GB dense-column bomb is prevented by 256 dynamic dense columns plus sparse fallback ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:449)). But malicious `setProperty("name_"+i, float)` for 1M names on one atom still interns 1M names and stores 1M sparse properties because the sparse bag has no cap ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:440), [V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:454)). Growth is no longer `1M x 100k x 4 B`; it is roughly O(distinct names + sparse entries), still unbounded and attacker-controlled. Add max dynamic property names/entries and a sparse-bag memory budget or rejection policy. |
| P17b-3 | D23a promotion path/oscillation | DEBT | Promotion is specified as sampled at `compact()` time, not synchronous/background ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:444)). That avoids surprise work on `setProperty`, but no demotion/hysteresis/tombstone policy is written for properties oscillating around 10%. Add "promote once, never demote" or explicit high/low thresholds and define unset accounting. |
| P17b-4 | D24a relaxed atomics data-race freedom | OK | Yes for the narrow memory-model question: an atomic `fetch_or(relaxed)` racing with an atomic `load(relaxed)` on the same `atomic<uint64_t>` is data-race-free. cppreference summarizes the C++ rule: a data race requires at least one conflicting evaluation to be non-atomic, and relaxed atomics guarantee atomicity without synchronization/order constraints. Source: https://en.cppreference.com/w/cpp/atomic/memory_order. This does not close P17b-11's container/reallocation problem. |
| P17b-5 | D26a EBO macro | OK | `#define BALL_EMPTY_BASES __declspec(empty_bases)` under `_MSC_VER` and empty elsewhere is the right portability shape ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:510)). `class BALL_EXPORT BALL_EMPTY_BASES Atom ...` becomes multiple `__declspec(...)` annotations on MSVC and an empty macro on Clang/GCC ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:524)). Put the v2.1 pins in [Sizeof_test.C](/Users/kohlbach/Claude/BALL2/test/Sizeof_test.C:26), next to the existing Atom/Bond checks, and keep it in the already listed `Sizeof_test` target ([test/CMakeLists.txt](/Users/kohlbach/Claude/BALL2/test/CMakeLists.txt:56)). |
| P17b-6 | D30a mixed-type backward read | OK | For v2.0 JSON where atom 0 has `foo: INT=5` and atom 1 has `foo: STRING="bar"`, D30a registers `foo` as INT on first occurrence, writes atom 0 to the dense column, then stores atom 1's mismatched STRING in sparse fallback ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:542)). `getProperty("foo")` then returns the column value if present, else sparse value ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:547)). This yields 5 for atom 0 and "bar" for atom 1, assuming dense dynamic columns have per-row presence bits. v2.0 writes per-atom property bags today ([moleculeStoreJson.C](/Users/kohlbach/Claude/BALL2/source/KERNEL/moleculeStoreJson.C:109)). |
| P17b-7 | D31 iterator invariant | DEBT | D31 is documentation plus a future test plan, not enforcement ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:561), [V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:574)). Current iterators store `Composite*` positions ([composite.h](/Users/kohlbach/Claude/BALL2/include/BALL/CONCEPT/composite.h:1115), [composite.h](/Users/kohlbach/Claude/BALL2/include/BALL/CONCEPT/composite.h:1275)). Enforcement: keep `CompositeNode` incomplete/private outside store internals, do not expose `CompositeNode*` from `Composite::getNode_()`, return topology scalars/references only inside step functions, and add a compile-time grep/static-analysis check or a narrow friend boundary that prevents iterator classes from naming `CompositeNode`. |
| P17b-8 | D32 backport policy completeness | DEBT | D32 covers shared bugs, v2.1-thin-only bugs, and Wave 4-7 closures that discover thin-only bugs ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:585)). It does not cover a v2.0.x patch regression that v2.1's thin handle would have prevented or made impossible. Add policy: v2.0.x-only regressions are fixed on v2.0.x with a v2.0 regression test; if the public behavior exists in v2.1, add the same test there even if no code change is needed. |
| P17b-9 | D33 review cadence/P5 guardrail | BUG | P6 planning can stay downgraded. P5 should keep planning review because it introduces `V21-STORE-ITER-API`, perf gates, benchmark methodology, and generation-guard finalization ([MILESTONE-v2.1-KICKOFF.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/MILESTONE-v2.1-KICKOFF.md:73)). D33 calls P5 "execution/verification" only ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:612)), but store iteration is public API surface. Restore a P5 planning review or split `STORE-ITER-API` into a reviewed earlier phase. |
| P17b-10 | D34 MSVC CI gate | DEBT | It is not required before P1 starts because P1 lands side tables while Atom still inherits the old bases ([MILESTONE-v2.1-KICKOFF.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/MILESTONE-v2.1-KICKOFF.md:69)). It is too late if gated only before v2.1.0 final ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:620)). Correct gate: before P2 close/merge of the thin-handle flip and before accepting `sizeof(Atom) <= 32 B` as achieved. |
| P17b-11 | New issues introduced by revisions | BLOCKER | D23a mismatched-type fallback breaks v2.0 replacement unless sparse overrides/tombstones clear old dense values. Current v2.0 `setProperty` erases any prior same-name property before pushing the new one ([property.C](/Users/kohlbach/Claude/BALL2/source/CONCEPT/property.C:193)); D23a says mismatched type stays sparse ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:466)), while D30a says `getProperty` checks column first ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:547)). Sequence: set `foo` INT on atom 0, then set `foo` STRING on atom 0; v2.1 would still return old dense INT if the sparse entry does not override or tombstone the column. D24a also has an implementation blocker: `std::vector<std::atomic<uint64_t>>` cannot be normally resized/reserved/shrunk on libc++ because `atomic` is not Cpp17MoveInsertable; current `MoleculeStore::compact()` shrinks every side vector ([moleculeStore.C](/Users/kohlbach/Claude/BALL2/source/KERNEL/moleculeStore.C:330)). Use a movable `AtomicWord` wrapper with explicit load/store move under exclusive resize, a fixed `unique_ptr<atomic[]>` rebuilt under exclusive access, or keep non-atomic storage plus snapshot/lock design. D22a's 4.8 MB table must be explicitly budgeted: P1 adds it before Atom shrink, and even after P2 the D13 success criterion is <=160 B/atom ([MILESTONE-v2.1-KICKOFF.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/MILESTONE-v2.1-KICKOFF.md:139)). |

## New findings

### P17b-N1: D23a mismatched-type sparse fallback violates `setProperty` replacement semantics — BLOCKER

v2.0 replacement is by name only: `PropertyManager::setProperty` erases an existing same-name property, regardless of old/new type, then appends the new property ([property.C](/Users/kohlbach/Claude/BALL2/source/CONCEPT/property.C:193)). D23a changes a mismatched type to "stays in the sparse bag for that atom" ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:466)), while D30a says `getProperty(name)` returns the column value before sparse fallback ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:547)).

That breaks a normal public API sequence:

```cpp
atom.setProperty(NamedProperty("foo", 5));      // foo promoted/registered as INT
atom.setProperty(NamedProperty("foo", "bar")); // mismatched STRING goes sparse
atom.getProperty("foo");                        // must return "bar", not stale 5
```

Required fix: sparse mismatch must either tombstone/clear the dense row for that atom, or lookup precedence must be "sparse override first, then dense column". The JSON demux path also needs this rule, otherwise D30a's compatibility behavior and live API behavior diverge.

### P17b-N2: `std::vector<std::atomic<uint64_t>>` is not compatible with reserve/compact/reallocation — BLOCKER

D24a chooses `std::vector<std::atomic<uint64_t>>` ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:478)). Atomic word operations are the right race-free primitive, but the exact container is not viable with the store's vector discipline. `std::atomic` is non-copyable/non-movable; a local C++17 compile on the repo's current Apple Clang/libc++ fails for:

```cpp
std::vector<std::atomic<std::uint64_t>> v;
v.resize(10);
v.reserve(20);
v.shrink_to_fit();
```

with a libc++ static assertion that the type is not Cpp17MoveInsertable. Current `MoleculeStore::compact()` calls `shrink_to_fit()` across side vectors and advances generation under an exclusive-access contract ([moleculeStore.C](/Users/kohlbach/Claude/BALL2/source/KERNEL/moleculeStore.C:330), [moleculeStore.h](/Users/kohlbach/Claude/BALL2/include/BALL/KERNEL/moleculeStore.h:249)).

Required fix: replace `vector<atomic<u64>>` with a storage shape that supports the store lifecycle, such as a custom movable wrapper whose move constructor uses relaxed load/store and is used only during exclusive resize, or an owned atomic array rebuilt under exclusive compact/reserve.

### P17b-N3: D23a sparse fallback is still unbounded — BUG

The 256 dynamic dense-column cap is useful, but only caps promoted columns ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:449)). The sparse fallback bag and name intern pool have no configured limit ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:440), [V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:454)). A 1M-name attack on one atom remains O(1M) allocations/entries. That is no longer a 400 GB dense allocation, but it is still an unbounded memory bomb through public API.

Required fix: add a max dynamic property-name count and/or max sparse entries/bytes per store, with documented behavior after the cap: reject with `InvalidArgument`, evict only if v2.0 semantics allow it, or require explicit opt-in for unbounded compatibility mode.

### P17b-N4: P5 planning review was downgraded despite adding public API — BUG

D33 downgrades P5 planning review because P5 is described as execution/verification ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:609)). The milestone says P5 includes `V21-STORE-ITER-API` and generation-guard finalization ([MILESTONE-v2.1-KICKOFF.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/MILESTONE-v2.1-KICKOFF.md:73)). Store iteration API is public API surface and should not land from a maintainer checklist alone.

Required fix: restore P5 planning review, or move `V21-STORE-ITER-API` into P1/P2/P4 with a planning gate.

### P17b-N5: D31 iterator invariant has no hard enforcement — DEBT

D31 states that iterators must never cache `CompositeNode*` or node indices ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:561)), but enforcement is a future test only ([V21-DECISIONS.md](/Users/kohlbach/Claude/BALL2/.planning/v2.1/V21-DECISIONS.md:574)). A test is useful but will not prevent a later implementation from exposing `CompositeNode` in iterator code.

Required fix: make `CompositeNode` private to `MoleculeStore` internals, avoid returning `CompositeNode*` from any `Composite` helper available to iterator classes, and add a source-level guard in CI that fails if iterator headers mention `CompositeNode` or `composite_nodes_`.

## Overall verdict

**NO-GO for P1.** D22a/D23a/D24a/D26a/D30a are materially better than R17, and P17-6 remains properly deferred. But the revised design still has P1-blocking issues:

1. Fix D23a replacement semantics for mismatched-type properties: sparse override/tombstone must prevent stale dense values from winning.
2. Replace `std::vector<std::atomic<uint64_t>>` with an atomic bit storage design that can be resized/reserved/compacted under `MoleculeStore` lifecycle rules.
3. Bound the sparse fallback/name intern growth, not just dense-column promotion.
4. Correct D22a memory accounting and explicitly budget the 48 B/node side table through P1/P2 and D13.

Recommended next step: revise D23a and D24a before starting P1, restore a P5 planning gate in D33, and move D34's MSVC size CI gate to P2 close rather than v2.1.0 final.
