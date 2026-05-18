# K0.5 Adversarial Review — Codex CLI Round 6 (2026-05-18)

**Status:** Complete. Codex verdict: **no-go for K0.6 baseline** until
3 OPEN items are fixed. Remediation tracked in K0.5.8.

**Reviewer:** Codex CLI 0.128.0 (`codex exec`).
**Subject:** K0.5.0–K0.5.6 (commits `5356fe188..8f318d96a`).

---

## Findings

| # | Sev | Probe | Verdict | Rationale |
|---|---|---|---|---|
| 1 | OPEN | Cache key correctness | NOT SAFE | `~MoleculeStore` invalidates by pointer (closes simple stale reuse) but `CompiledExpressionCache` has NO mutex — destructor invalidation can race `get_or_compile` on another thread. Key is `(source, store*)` only, ignores predicate registry identity — two Expressions with the same source but different registered predicate factories share the wrong `OwnedPred`. |
| 2 | OPEN | OwnedPred lifetime | UNSAFE | One `ExpressionPredicate` instance lives in the cached AST and is reused across every `evaluate()` / `evaluate_one()`. Stateful v1.x predicates (anything that mutates between calls — e.g. SMARTS state, ring detection caches) corrupt under concurrent or repeated use. Cache sharing makes the sharing broader than v1.x per-ExpressionTree ownership. |
| 3 | OPEN | Parser mutex granularity | LEAK | `ExpressionParser::parse()` is mutex-guarded, but `~ExpressionParser` calls `ExpressionParser_destroy()` (generated lexer cleanup) WITHOUT the lock — touches the same Flex/Bison globals. Race window on parser teardown vs concurrent parse on another thread. |
| 4 | FYI | `getCompiled` shared_ptr semantics | DOCUMENT | No direct dangling `MoleculeStore*` is stored inside CompiledExpression, so caller-outlives-store doesn't poison the object directly. But the object is store-specific via interned offsets — using it against a different store is silent UB. API/docs should state more strongly or bind/check store identity. |
| 5 | OPEN | Benchmark validity | INSUFFICIENT | Narrow synthetic 50k corpus, 4 fast-path queries, soft `>=5x` test gate. Codex says the K0.7 deferment is too aggressive — the K0.5 claim needs broader corpus + the stated 10x gate enforced. |
| 6 | FYI | AndNode tmp-bitmap churn | OPTIMISATION | O(depth × n) temporary bitmap allocations on deep nested ASTs. Bounded per node, not correctness-blocking; arena/scratch-buffer work later. |

---

## Remediation Plan (K0.5.8)

| Item | Fix | Effort |
|---|---|---|
| #1 cache | Wrap `CompiledExpressionCache` mutators in a `std::mutex`. Add a registry-fingerprint hash to the cache key so registries with different factory bindings don't collide. | 0.5 day |
| #2 OwnedPred | Make `OwnedPred::impl` create a fresh predicate instance per evaluate via the registered factory (store factory pointer + arg, not the instance). For per-atom path use a thread-local instance if reuse benefit is meaningful. | 0.5 day |
| #3 parser dtor | Move the mutex lock to wrap `~ExpressionParser` body (covering `ExpressionParser_destroy()`). Or move `ExpressionParser_destroy()` into `parse()`'s finally block so it's already inside the lock. | 0.5 day |
| #5 benchmark | Tighten test gate to `>=10x` (current ratio is ~50x so the gate has headroom), expand corpus query set to include negation, deep AND/OR nesting, and OwnedPred mixed with fast-path. | 0.5 day |
| #4, #6 | Documentation update on getCompiled API + tracked FYI for K0.7 perf phase. | inline with #1-#3 |

Total: ~2 days. After K0.5.8 green sweep, gate-check K0.6 (persistence v2) entry.

---

## Verdict (Codex, verbatim)

> No-go as a clean K0.6 baseline yet. Fix the cache mutex/key
> semantics and parser destructor locking first; the benchmark and
> tmp-bitmap issues can ride as tracked FYIs, but the cache/OwnedPred
> correctness risks are real.

---

## Operational notes

Codex CLI Round 5 (K0.4 review) hung for 2.5h on what turned out to
be an interactive prompt waiting on stdin and was killed. Round 6
ran to completion in ~2 min by feeding the prompt purely via the
command-line argument and using `bmad-review-adversarial-general`
internal skill rather than the bare `exec` path. Worth recording for
future automation.
