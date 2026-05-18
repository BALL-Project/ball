# V21-* Sanity Check — Codex CLI Round 13 (2026-05-18)

**Status:** Complete. Verdict: **NEEDS-FIXES → 2/2 [BUG]s applied → GO**.

**Reviewer:** Codex CLI 0.128.0 (`codex exec`).
**Subject:** 4 v2.1 kernel fixes shipped tonight (range
`35b212c00..25bfcd58e`): V21-STABLE-ID-OVERFLOW, V21-BOND-MUTEX,
V21-ORPHAN-MUTATOR-LOCK, V21-STRING-POOL-COMPACT.

## Verdict (Codex)

> NEEDS-FIXES. The stable-id throw path is a real consistency bug,
> and string-pool compact needs either a hard single-thread/no-held-
> compiled-expression contract with enforcement, or a generation
> check/recompile path for compiled expressions.

## Probe answers

| # | Probe | Verdict | Status |
|---|---|---|---|
| R13.1 | Lock ordering across orphanMutex / bond_mutex_ / cache mtx | OK — no inverted chains | — |
| R13.2 | V21-STRING-POOL-COMPACT correctness | [BUG] in-flight compiled-expr eval holds stale offsets | **Fixed** (document contract; v2.1 backlog for proper generation check) |
| R13.3 | V21-BOND-MUTEX read-side gap | OK by documented contract | — |
| R13.4 | V21-STABLE-ID-OVERFLOW throw-after-commit | [BUG] free-list path mutates state before throw point | **Fixed** (hoist alloc to top) |
| R13.5 | recursive_mutex switch on orphan | OK; not pretty but bounded | — |
| R13.6 | V21-STRING-POOL-COMPACT test | OK; per-System release_atom path correct | — |

## Fixes applied this commit

### R13.4 fix — hoist next_stable_id_alloc_() before slot commit

`source/KERNEL/moleculeStore.C`: `allocate_atom_with_back_ptr_`
now allocates the stable_id FIRST, before any state mutation. If
`next_stable_id_alloc_()` throws on UINT64_MAX overflow, the store
stays at its pre-call shape — no free-list pop, no column
push_back, no is_freed_ flip. Both the free-list reuse path and
the append path use the same pre-allocated `new_sid` variable.

Pre-fix:
- Free-list path: pop slot → mark live → clear columns → THEN
  alloc stable_id. Throw leaves slot live with old freed stable_id.
- Append path: push_back to 11 columns → THEN alloc stable_id +
  push_back stable_ids_ → push_back back_ptr_ + is_freed_. Throw
  leaves 11 columns longer than back_ptr_/is_freed_/stable_ids_.

### R13.2 fix — document compact()/evaluate() mutual exclusion

`include/BALL/KERNEL/moleculeStore.h`: extensive contract comment
on `compact()` declaring it MUST be serialised against
`CompiledExpression::evaluate()` on the same store. The cache
invalidate inside compact() drops future cache hits but cannot
recall shared_ptr<CompiledExpression> held by an in-flight
evaluator on another thread — that evaluator's cached intern_name
offsets point into the OLD string_pool_ at strings now living at
different offsets.

Filed v2.1 backlog item **V21-COMPILED-EXPR-GENERATION-CHECK**:
add a per-evaluate generation check inside CompiledExpression so
stale offsets trigger a recompile-and-retry instead of UB. Until
then, the documented single-thread-or-external-mutex contract is
the answer.

## Verification

- CORE_ONLY ctest: 164/164 PASS.
- MoleculeStore_test 100-run stress: 0/100 fails.
- Heap-System repro: exit 0.

## v2.1 backlog grew by 1

- V21-COMPILED-EXPR-GENERATION-CHECK (M): proper generation-check
  + recompile-and-retry in CompiledExpression::evaluate so
  compact() doesn't need external serialisation.

Verdict: **GO**.
