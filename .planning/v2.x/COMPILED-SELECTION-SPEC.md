# COMPILED-SELECTION-SPEC — K0.5 design contract

**Status:** DRAFT. **Authored:** 2026-05-18. **Phase:** K0.5 (compiled
selection). **Effort estimate:** 3-4 weeks (per `V2.0-ROADMAP.md`).
**Authoritative for K0.5.**

**Companion docs:**
- [`KERNEL-V2-DECISIONS.md`](KERNEL-V2-DECISIONS.md) — D11 (compiled
  selection in K0; variant AST; ≥10× Selector_test speedup).
- [`V2.0-ROADMAP.md`](V2.0-ROADMAP.md) — phase placement after K0.4.
- `include/BALL/KERNEL/moleculeStore.h` — the SoA columns we evaluate
  over.

This document specifies the v2.0 replacement for the
`Expression` / `ExpressionTree` / `ExpressionParser` / `Selector`
quartet. It is the input to `/gsd-plan-phase` for K0.5.

When this document and any other document disagree on a K0.5 contract,
this document wins (subordinate to `KERNEL-V2-DECISIONS.md`).

---

## 1. Goal restated

Per D11:

> **D11 — Compiled selection.** In K0. Variant-based cached AST;
> Selector + Expression rewritten once.

Gate per K0.5 row of `V2.0-ROADMAP.md`:

> `Expression_test` + `ExpressionTree_test` + `ExpressionParser_test` +
> `ExpressionPredicate_test` + `KernelPredicate_test` + `Selector_test` +
> `StandardPredicates_test1..4` + `GlobalKernel_test` green;
> **Selector_test ≥10× faster than baseline.**

### 1.1 Baseline numbers (`baseline/tests.txt`, 2026-05-17)

```
Expression_test           246 ms
ExpressionParser_test     255 ms
ExpressionPredicate_test  243 ms
ExpressionTree_test       316 ms
Selector_test             239 ms
```

The K0.5 brief cited "Selector_test ~3 s currently"; the recorded
baseline says 239 ms. The brief figure likely refers to a pre-K0
measurement or to a corpus that has not been committed. Resolution:

- The **gate** (≥10× faster than baseline) stays.
- The **measurement** is taken against `baseline/tests.txt` at the
  K0.5 branch point. With the current 239 ms number, the gate is
  **< 24 ms**. If the K0.5 worker sees a different baseline, they
  re-record and apply 10× to that.
- K0.5 also adds a `benchmarks/selector_bench.C` micro-benchmark
  (≥1 M evaluations of the existing corpus over a 100k-atom synthetic
  System) so the gate is stable and isolatable.

### 1.2 Non-goals — see §8.

---

## 2. AST representation — variant-based

### 2.1 Node taxonomy

The variant AST collapses 27 polymorphic `ExpressionPredicate`
subclasses (see `source/KERNEL/expression.C:209-239`) into a small,
closed set of node types. Each leaf node carries **pre-compiled
constants** (element index, normalised name offset, charge bound,
etc.) so the inner evaluation loop is just integer/float comparison
on a column.

**Vectorisable leaves** (single integer / float compare per atom):
`TrueLeaf`, `FalseLeaf`, `SelectedLeaf`, `SolventLeaf`, `BackboneLeaf`,
`ElementPred{uint8_t element_idx}`,
`AtomNamePred{uint32_t name_offset}` (offset into the store string
pool — see §4.4),
`AtomTypePred{uint32_t type_name_offset}`,
`ResidueNamePred / ChainPred / ProteinPred / MoleculePred`
(`{uint32_t name_offset}` each, one indirect lookup through the K0.4
atom→residue/chain/etc. CSR),
`ResidueIDPred{int32_t id; bool string_match; std::string raw}`,
`ChargeRange{CmpOp op; float value}`,
`HybridLeaf{uint8_t kind}` (1=sp, 2=sp2, 3=sp3),
`NumberOfBondsPred{CmpOp op; uint8_t n; uint8_t order_filter}` (partly
vectorisable — single integer compare on the K0.2b CSR `bond_degree`
column).

**Slow-path leaves** (`OwnedPred` — heap-held v1.x predicate instance,
per-atom virtual dispatch through `Atom*` back-pointer):
`ConnectedToPred` (owned `ConnectedToPredicate`),
`InRingPred` (owned `InRingPredicate`, ring DFS),
`AxialLeaf`, `Conf4C1Leaf`,
`SmartsPred` (non-core-only),
**any user predicate** registered via
`Expression::registerPredicate()`.

**Inner nodes:** `AndNode{vector<PredNode> children}` (bitmap AND),
`OrNode{vector<PredNode> children}` (bitmap OR),
`NotNode{PredNode child}` (bitmap NOT).

We keep the v1.x predicate classes alive for the slow paths; we do
not rewrite ring/bond/SMARTS algorithms in K0.5.

### 2.2 Variant sketch

```cpp
// include/BALL/KERNEL/compiledExpression.h (new)
namespace BALL {

struct TrueLeaf      {};
struct FalseLeaf     {};
struct SelectedLeaf  {};
struct SolventLeaf   {};
struct BackboneLeaf  {};

struct ElementPred       { std::uint8_t element_idx; };
struct AtomNamePred      { std::uint32_t name_offset; };
struct AtomTypePred      { std::uint32_t type_name_offset; };
struct ResidueNamePred   { std::uint32_t name_offset; };
struct ResidueIDPred     { std::int32_t  id;  bool string_match; std::string raw; };
struct ChainPred         { std::uint32_t name_offset; };
struct ProteinPred       { std::uint32_t name_offset; };
struct MoleculePred      { std::uint32_t name_offset; };

enum class CmpOp : std::uint8_t { LT, LE, EQ, GE, GT };
struct ChargeRange       { CmpOp op; float value; };
struct HybridLeaf        { std::uint8_t kind; };  // 1=sp, 2=sp2, 3=sp3
struct NumberOfBondsPred { CmpOp op; std::uint8_t n; std::uint8_t order_filter; };

// Slow-path: owned non-vectorisable predicate instance.
struct OwnedPred {
    std::unique_ptr<ExpressionPredicate> impl;
    std::string                          name;       // for hashing / equality
    std::string                          argument;   // for hashing / equality
};

// Forward declarations for boxed inner nodes (variant needs complete types
// only at instantiation, but cleanest with forward + unique_ptr).
struct AndNode;
struct OrNode;
struct NotNode;

using PredNode = std::variant<
    TrueLeaf, FalseLeaf, SelectedLeaf, SolventLeaf, BackboneLeaf,
    ElementPred, AtomNamePred, AtomTypePred,
    ResidueNamePred, ResidueIDPred, ChainPred, ProteinPred, MoleculePred,
    ChargeRange, HybridLeaf, NumberOfBondsPred,
    OwnedPred,
    std::unique_ptr<AndNode>,
    std::unique_ptr<OrNode>,
    std::unique_ptr<NotNode>
>;

struct AndNode { std::vector<PredNode> children; };
struct OrNode  { std::vector<PredNode> children; };
struct NotNode { PredNode child; };

} // namespace BALL
```

**Why `unique_ptr<AndNode>` etc.** `std::variant` needs complete
alternatives; boxing the recursive cases is the standard workaround.
The pointer hop costs cache locality but keeps the variant size
bounded (≤16 B for vectorisable leaves; one pointer + 2 strings for
the worst `OwnedPred`).

**Flat-AST alternative considered** (single `std::vector<PredNode>`
with child-index ints). Rejected for K0.5: (a) `OwnedPred` already
holds a `unique_ptr` so the variant is move-only regardless; (b) test-
corpus ASTs are tiny (≤6 nodes) so locality wins are bounded. K0.7
may revisit.

`PredNode` is move-only. `CompiledExpression` is move-only at the
public level; the cache (§3.3) holds
`std::shared_ptr<const CompiledExpression>`.

---

## 3. CompiledExpression and the cache

### 3.1 Class shape

```cpp
class BALL_EXPORT CompiledExpression {
  public:
    explicit CompiledExpression(PredNode root,
                                std::string source,
                                std::size_t pred_set_hash);
    ~CompiledExpression();

    CompiledExpression(const CompiledExpression&)            = delete;
    CompiledExpression& operator=(const CompiledExpression&) = delete;
    CompiledExpression(CompiledExpression&&)            noexcept = default;
    CompiledExpression& operator=(CompiledExpression&&) noexcept = default;

    // Bitmap evaluation (vectorised inner loop).
    // Output: bitmap[i] = (root evaluates true for store atom i).
    // bitmap.size() == store.size() (includes freed slots; freed
    // slots get bitmap[i] = 0 unconditionally).
    void evaluate(const MoleculeStore& store,
                  std::vector<std::uint8_t>& out_bitmap) const;

    // Per-atom evaluation fallback. Required for the existing
    // Expression::operator()(const Atom&) API surface.
    bool evaluate_one(const Atom& atom) const;

    const std::string& source() const  { return source_; }
    std::size_t pred_set_hash() const  { return pred_set_hash_; }
    const PredNode& root() const       { return root_; }

  private:
    PredNode    root_;
    std::string source_;          // original expression string
    std::size_t pred_set_hash_;   // hash of registered-predicate name set
};
```

### 3.2 Compilation entry-point

`Expression::compile_()` is the K0.5 internal function called by
`setExpression()` and (lazily) by the first `operator()()` call after
construction:

1. **Lex + parse.** Drive a re-entrant parser (§5) over the expression
   string. Yields a `SyntaxTree` (unchanged shape from
   `ExpressionParser::SyntaxTree`).
2. **Lower.** Walk the `SyntaxTree`. For each leaf, dispatch on the
   predicate name:
   - If the name resolves to a known vectorisable predicate
     (`element`, `name`, `residue`, …), construct the corresponding
     leaf node with pre-compiled constants (parse the argument into
     `uint8_t element_idx`, intern the string into the store's pool to
     get a `name_offset`, etc.). Argument parse errors throw
     `Exception::ParseError`.
   - If the name is in `Expression::create_methods_` but not a
     built-in (i.e. registered via `registerPredicate`), construct
     `OwnedPred` with the user-supplied `ExpressionPredicate` instance.
   - Otherwise throw `Exception::ParseError` (same behaviour as v1.7).
3. **Cache.** Look up the global cache (§3.3) under
   `(source_string, pred_set_hash)`. If present, drop the freshly built
   tree and adopt the cached `shared_ptr`. If absent, install the new
   tree.

### 3.3 Cache

A per-process LRU map:

```cpp
// source/KERNEL/compiledExpression.C (translation-unit-local)
struct CacheKey {
    std::string expression_source;
    std::size_t pred_set_hash;
    bool operator==(const CacheKey&) const = default;
};
struct CacheKeyHash { std::size_t operator()(const CacheKey&) const; };

class CompiledExpressionCache {
  public:
    static CompiledExpressionCache& instance();  // Meyers singleton
    std::shared_ptr<const CompiledExpression>
        get_or_compile(CacheKey key, std::function<PredNode()> builder);
    void invalidate(const CacheKey& key);
    void clear();
    std::size_t size() const;
    void set_capacity(std::size_t n);  // default: 1024 entries
  private:
    mutable std::mutex                                              mu_;
    std::unordered_map<CacheKey, std::shared_ptr<const CompiledExpression>, CacheKeyHash> map_;
    std::list<CacheKey>                                             lru_;
    std::size_t                                                     cap_ = 1024;
};
```

**Cache key constituents:**

- `expression_source` — the raw string passed to
  `Expression::setExpression()`.
- `pred_set_hash` — `Expression::registerPredicate()` may install
  user-defined predicates. Two `Expression` instances with the same
  source string but different registered predicates must compile to
  different ASTs (and the user-predicate identity is captured via the
  pointer-to-creation-method, which lives in the `create_methods_`
  map). Hash = `xor` of `(name, creation_method_pointer)` pairs from
  `create_methods_`.

**Invalidation:**
- Explicit only — `Expression::recompile()` (new method) drops the
  cached entry and rebuilds.
- Implicit on `registerPredicate()` call: the next `operator()()`
  notices the `pred_set_hash` has changed and re-compiles. The old
  cache entry stays in the LRU until evicted.

**LRU bound:** 1024 entries by default (configurable via
`CompiledExpressionCache::instance().set_capacity(n)`). Each entry
carries one AST (typically ≤ few KB), so cache memory is bounded to
~few MB worst-case.

**Thread safety:** Single mutex around the map + LRU list. Lookup is
O(1) hash + O(1) LRU bookkeeping. Contention should be negligible
because evaluation does not touch the cache and re-compilation is
rare. If contention bites, K0.7 may shard the cache.

---

## 4. Evaluation — bitmap inner loop

### 4.1 Algorithm

`CompiledExpression::evaluate(store, out_bitmap)`:

```cpp
void CompiledExpression::evaluate(const MoleculeStore& s,
                                  std::vector<std::uint8_t>& out) const
{
    out.assign(s.size(), 0);
    eval_node_(root_, s, out);
    // freed slots zeroed automatically: leaf evaluators look at
    // selection_/element_indices_/etc., but the Selector-integration
    // step (§5) masks out s.is_freed(i) before reporting.
}
```

Each node type lowers to a tight loop:

```cpp
// Sketch — ElementPred.
void eval_element_(const ElementPred& p, const MoleculeStore& s,
                   std::vector<std::uint8_t>& out)
{
    const auto* col = s.element_indices_data();  // const uint8_t*
    const std::size_t n = s.size();
    for (std::size_t i = 0; i < n; ++i) {
        out[i] = (col[i] == p.element_idx) ? 1 : 0;
    }
}

// Sketch — AndNode (2-arg case).
void eval_and_(const AndNode& a, const MoleculeStore& s,
               std::vector<std::uint8_t>& out)
{
    // Evaluate first child into out (overwrites).
    eval_node_(a.children[0], s, out);
    // For each subsequent child, evaluate into scratch and AND-merge.
    std::vector<std::uint8_t> scratch(s.size());
    for (std::size_t k = 1; k < a.children.size(); ++k) {
        eval_node_(a.children[k], s, scratch);
        for (std::size_t i = 0; i < s.size(); ++i) {
            out[i] &= scratch[i];
        }
    }
}
```

OR is the same with `|=`. NOT flips bits with a final
`out[i] = out[i] ? 0 : 1` pass.

### 4.2 New MoleculeStore accessors required

Bitmap evaluation reads columns directly; we add `*_data()` const
accessors to `MoleculeStore` for the columns the leaves need:

```cpp
const std::uint8_t* element_indices_data() const { return element_indices_.data(); }
const float*        charges_data()          const { return charges_.data(); }
const std::uint32_t* name_offsets_data()    const { return name_offsets_.data(); }
const std::uint32_t* type_name_offsets_data() const { return type_name_offsets_.data(); }
const std::uint8_t* selection_data()        const { return selection_.data(); }
// + a residue_index column (K0.4 should already expose this), used by
//   ResidueNamePred / ResidueIDPred / ChainPred / ProteinPred etc.
```

`ResidueNamePred` and friends evaluate by dereferencing a per-atom
`residue_offset` (atom → residue index from the K0.4 CSR), looking up
the residue's `name_offset` in a parallel residue-name column, and
comparing to the cached `name_offset` in the predicate. **One indirect
lookup per atom**; still a tight loop with no virtual dispatch.

### 4.3 Slow-path (OwnedPred)

`OwnedPred` cannot be vectorised. Its leaf evaluator falls back to:

```cpp
void eval_owned_(const OwnedPred& p, const MoleculeStore& s,
                 std::vector<std::uint8_t>& out)
{
    const std::size_t n = s.size();
    for (std::size_t i = 0; i < n; ++i) {
        if (s.is_freed(i)) { out[i] = 0; continue; }
        // Build a transient Atom handle and call the predicate.
        Atom* a = s.back_ptr(i);  // K0.3c.x guarantees back_ptr is set
        out[i] = (*p.impl)(*a) ? 1 : 0;
    }
}
```

This is the only path that walks through the heap-allocated `Atom`
handle. For user-defined predicates registered via
`registerPredicate()`, plus the legacy ring/bond/SMARTS predicates
listed in §2.1, this is the only path.

### 4.4 Name-comparison detail

Atom-name / residue-name / chain-name etc. comparisons need a fast
string-equality test. We use the store's string pool plus an interner:

- When lowering an `AtomNamePred("CA")`, we call
  `store.intern_name("CA")` which returns a stable `uint32_t` offset
  into the string pool (registering the string if it was not present).
- At eval time we compare `name_offsets_[i] == p.name_offset`. This is
  a single 32-bit compare in the inner loop — no string compare, no
  hash.
- The intern table is part of MoleculeStore (added as a K0.5 sub-task:
  `MoleculeStore::intern_name` + `intern_type_name`). The store
  already owns the pool (`string_pool_`) — see `moleculeStore.h:286`.
  We add a hash map from string view to offset alongside.

**Open question (K0.5 V1):** when does the interner get pruned? If the
store grows for a long time, the pool can balloon. We propose:
intern-table entries are append-only for the life of the store;
`compact()` may rebuild it. This matches the live-reference contract
on column data.

---

## 5. Selector / Expression API integration

### 5.1 Public API preservation (D10)

Every public symbol of `Expression`, `ExpressionTree`,
`ExpressionParser`, `ExpressionPredicate`, `Selector`,
`StandardPredicates` stays. Tests pull these headers verbatim; per D10
no test sources may be edited.

Concretely, the following symbols are **preserved** with the same
signatures (we list the load-bearing ones; the full surface is
`include/BALL/KERNEL/expression.h:79-217`, `expressionTree.h:25-182`,
`selector.h:30-127`):

- `Expression::Expression(const String&)`,
- `Expression::operator()(const Atom&) const`,
- `Expression::setExpression(const String&)`,
- `Expression::registerPredicate(const String&, CreationMethod)`,
- `Expression::getExpressionTree() const` — **returns a synthesised
  `ExpressionTree*`** built lazily for tests that inspect the tree
  structure (see `test/ExpressionTree_test.C`). The synthesised tree
  is built from the variant AST on demand; it shares lifetime with
  the `Expression`.
- `Selector::operator()(Composite&)`,
- `Selector::start()`,
- `Selector::getNumberOfSelectedAtoms()`,
- `Selector::getSelectedAtoms()`.

**`ExpressionTree` retained as a thin facade.** The class stays in
`expressionTree.h` because tests use `ExpressionTree::Type`, the
`getType()` / `getChildren()` / `getPredicate()` accessors etc. We
keep `ExpressionTree` as a synthesisable view over the compiled AST.
The compiled AST is the source of truth; `getExpressionTree()`
materialises a tree of `ExpressionTree` nodes that mirrors the variant
shape, lazily, with a `mutable std::unique_ptr<ExpressionTree>` cache
member on `Expression`.

### 5.2 Selector::apply path

`Selector::operator()(Composite&)` currently re-evaluates
`expression_(atom)` per atom (`source/KERNEL/selector.C:84-99`). Post
K0.4, every `Composite` is a handle that resolves to a
`MoleculeStore`. The new path:

```cpp
// Replaces selector.C:84-99
Processor::Result Selector::operator()(Composite& c)
{
    // Fast path: System or any container handle with a MoleculeStore.
    if (auto* store = c.store_or_null()) {
        if (current_store_ != store) {
            // First atom under a new store — evaluate the compiled
            // expression in one shot.
            const auto& ce = expression_.compiled();
            ce.evaluate(*store, bitmap_scratch_);
            current_store_ = store;
            current_store_gen_ = store->generation();
        }
        // Same store — re-use bitmap_scratch_. Index from handle.
        if (RTTI::isKindOf<Atom>(&c)) {
            Atom& a = dynamic_cast<Atom&>(c);
            const auto idx = a.store_index();
            if (bitmap_scratch_[idx]) {
                a.select();
                selected_atoms_.push_back(&a);
            }
        }
        return Processor::CONTINUE;
    }

    // Legacy fallback (defensive, post-K0.4 should not trigger): per-atom.
    if (RTTI::isKindOf<Atom>(&c)) {
        Atom& a = dynamic_cast<Atom&>(c);
        if (expression_(a)) { a.select(); selected_atoms_.push_back(&a); }
    }
    return Processor::CONTINUE;
}

// New Selector::start(): clear bitmap cache, clear results.
bool Selector::start() {
    selected_atoms_.clear();
    current_store_ = nullptr;
    bitmap_scratch_.clear();
    return true;
}
```

**Key shift:** with the bitmap cached, the processor's per-atom call
is a single bitmap read. The expensive walk-the-tree-per-atom logic
collapses to one evaluation per (store, expression) pair, then O(1)
per-atom decisions.

`current_store_gen_` lets us detect store mutation between the bitmap
build and a subsequent processor visit; if it advances we re-evaluate
(rare path — only happens if the System mutates while a Processor walk
is in flight, which v1.x already documents as UB).

### 5.3 Expression::operator()(const Atom&)

Kept for tests + callers that hold a bare `Atom&`. Routes to
`CompiledExpression::evaluate_one(atom)` which builds the bitmap-free
slow path:

```cpp
bool CompiledExpression::evaluate_one(const Atom& a) const {
    return eval_node_one_(root_, a);
}
```

Each node type has both a vectorised `eval_node_` and a scalar
`eval_node_one_`. The scalar variant is what powers the per-Atom path
and the test-suite's atom-by-atom assertions.

---

## 6. Parser re-entrancy fix

### 6.1 The bug

`source/KERNEL/expressionParser.C:146-169` uses a class-static `state`
field:

```cpp
state.current_parser = this;
state.buffer = s.c_str();
state.char_count = 0;
state.tree = 0;
...
ExpressionParser_initBuffer(state.buffer);
ExpressionParserparse();
```

`ExpressionParser::state` is a single `struct State` (declared at
`expressionParser.h:172-180`) shared across **every** call into the
parser, on every thread, in every translation unit. Two concurrent
`parse()` calls corrupt each other's `tree`/`buffer`/`char_count`
fields.

This was tolerable in v1.7 because Expression compilation was rare and
serial. K0.5 introduces a global compilation cache (§3.3) that may be
hit concurrently; combined with future thread-pool worker selection
this is a TSAN landmine.

### 6.2 The fix

Replace the static `state` with a `ParseContext` struct passed by
pointer through the parser:

```cpp
// expressionParser.h (revised)
struct ParseContext {
    Size                            char_count = 0;
    ExpressionParser*               current_parser = nullptr;
    const char*                     buffer = nullptr;
    ExpressionParser::SyntaxTree*   tree = nullptr;
};
```

The bison/flex glue is regenerated (or the C-style entry-points are
wrapped) so that each call to `parse(s)` allocates a stack-local
`ParseContext`, threads its address through `yyparse`, and uses
**reentrant** flex (`%option reentrant`) + bison (`%define api.pure`).
Flex reentrancy gives us per-scanner state; bison's pure API gives us
a `yyparse(void* user)` signature where `user` is our `ParseContext*`.

### 6.3 Legacy static-state fallback

We keep the static `ExpressionParser::state` field as a deprecated
shim **only if** non-KERNEL callers reach into it. Audit step: grep
for `ExpressionParser::state` across `source/`, `test/`, every
included module. If only KERNEL touches it, delete the field outright
(and update the K0.5 surface-deletion table in
`KERNEL-V2-DECISIONS.md` §3 with the symbol). Initial grep on the
v2.0 branch shows the state struct is referenced only in
`expressionParser.{C,h,l,y}` and the generated lexer/parser. Safe to
remove.

A global `std::mutex` guards the static-state shim if we keep it.
Modern callers use the `ParseContext` overload.

### 6.4 TSAN test

New test `test/CompiledExpressionRe-entrancy_test.C`:

```cpp
// Spawn N threads, each compiling and evaluating M expressions
// concurrently. Run under -fsanitize=thread. No races; no crashes;
// bitmap results match a single-threaded reference.
```

The build matrix grows by one TSAN-enabled job (`BALL_TSAN=ON`) which
exercises this test.

---

## 7. Tests

### 7.1 Existing tests — all must stay green unchanged

Per D10, **no test source edits.** The K0 green-bar (48 tests, see
`KERNEL-V2-DECISIONS.md` §6) includes:

- `Expression_test`
- `ExpressionTree_test`
- `ExpressionParser_test`
- `ExpressionPredicate_test`
- `KernelPredicate_test`
- `Selector_test`
- `StandardPredicates_test1` … `StandardPredicates_test4`
- `GlobalKernel_test`

All nine remain green-bar gates for K0.5.

### 7.2 New tests

**`CompiledExpression_test.C`** (new) — covers:

- **Correctness corpus.** A table-driven suite that:
  1. Takes ≥100 expression strings drawn from the union of
     `Selector_test`, `Expression_test`, `ExpressionPredicate_test`,
     and `StandardPredicates_test{1..4}` (extract them as a header-
     only `corpus.h` so both tests share them — no test source edits).
  2. Builds a 100k-atom synthetic `System` via a fixture.
  3. For each expression, evaluates **twice**: once via the legacy
     per-atom `Expression::operator()` path (forced by setting a
     `BALL_COMPILED_SELECTION=0` env var or via a debug flag), once
     via the bitmap path.
  4. Asserts the two `std::vector<uint8_t>` results are byte-identical.

  This is the v1.x-equivalence gate. Without it we have no signal
  whether the compiled tree changed semantics.

- **AST cache hit.** Compile the same expression twice on the same
  `Expression`; assert the cache returns the same `shared_ptr`.
- **AST cache invalidation.** Register a new user predicate via
  `Expression::registerPredicate()`; assert recompilation is forced.
- **`recompile()`.** Explicit invalidation drops the cached entry.
- **Variant constant pre-compilation.** `Expression("element(H)")`
  produces an `ElementPred{idx=1}` (not a string compare).

**`CompiledExpressionRe-entrancy_test.C`** (new) — §6.4.

**`benchmarks/selector_bench.C`** (new) — 100k-atom System, ≥1 M
evaluations across the corpus, prints ms/eval. **Gate: < 24 ms** total
for the existing Selector_test corpus measured at the same baseline
revision. This benchmark is the K0.5 perf gate per
`V2.0-ROADMAP.md`.

### 7.3 Bench wiring

`baseline/benchmarks.txt` (already exists) records a row for
`selector_bench` pre-K0.5; the K0.5 PR updates it with the new value
and lists the 10× ratio.

---

## 8. What is OUT of scope for K0.5

Echoed for the planning phase (these are non-goals; do not let them
creep in):

- JIT codegen (LLVM, AsmJit). Interpretation only.
- GPU evaluation kernels.
- Distributed / multi-process AST sharing.
- Persistence of compiled ASTs across processes.
- New expression syntax (grammar frozen).
- Vectorisation of ring / bond / SMARTS / connectedTo predicates.
- Selection bidirectional sync with VIEW (post-K0 anyway).
- Negation push-down / boolean simplification (e.g. De Morgan, double
  negation, constant folding). The K0.5 lowerer keeps the parsed AST
  shape; a simplification pass is a v2.1 perf polish.

---

## 9. Risks

### R1 — User-predicate (`registerPredicate`) coverage

`OwnedPred` cannot be vectorised. If the existing test corpus is
dominated by user-defined predicates, the 10× speedup gate fails.

**Audit (2026-05-18):** grep across `test/`:
- `Expression_test.C:85,184,197` — registers `MickeyPredicate` (a
  test fixture).
- No other test in the K0 green-bar registers user predicates.

Selector_test's corpus (`test/Selector_test.C:67-77, 114-125`) uses
only built-in predicates (`true()`, `connectedTo`, `element`, `name`,
`chain`). Of these, `connectedTo` falls into the slow path (kept as
`OwnedPred`), but `element`, `name`, `chain` are vectorisable. Bitmap
AND/OR/NOT short-circuiting plus per-atom bitmap reads should easily
clear 10×.

**Quantified:** if `connectedTo((-H))` is ~30% of Selector_test's
wall time, the compiled path can speed up the *other* 70% by ~50×
(per-atom-virtual-call elimination + tight inner loop) and the bond
predicate stays slow. Net speedup ≈ 1 / (0.3 + 0.7/50) ≈ 3.2×. **This
would miss the 10× gate.**

**Mitigation if hit:** add a fast `connectedTo` path that evaluates
the CTPNode tree against the bond CSR directly, bypassing per-Atom
`Bond*` traversal. This is one focused optimisation, scoped as a K0.5
follow-on if the gate falls short. We document it now so the K0.5
plan-phase doesn't have to re-discover it.

### R2 — Variant boxing / cache locality

`unique_ptr<AndNode>` etc. is a heap hop per inner node. For tiny
ASTs (≤6 nodes — the BALL test corpus average) the hop is cheap; for
deeply nested user expressions (rare) it could become a hotspot.

**Mitigation:** the K0.7 perf phase may convert to a flat-AST
representation if profiling shows the AST walk dominates. Until then,
keep the variant.

### R3 — Re-entrancy fix breaks legacy callers

Audit of `ExpressionParser::state` shows no external readers (see
§6.3). If a downstream module (post-K0 re-enablement) turns out to
poke into the static state, this is a build break. Acceptable given
v2.0's tests-only source-compat scope.

### R4 — `ExpressionTree` synthesised facade staleness

Tests call `Expression::getExpressionTree()` and walk the legacy
tree (`test/ExpressionTree_test.C`). Our facade is lazily synthesised
from the variant AST. If the synthesis differs in **structure** from
the v1.7 tree (e.g. a different node-merge policy), tests may break
with no implementation bug — just a representational divergence.

**Mitigation:** drive the synthesis directly from the parser
`SyntaxTree` (same shape as v1.7) instead of from the variant AST.
The variant is for evaluation; the `ExpressionTree` facade is for
introspection compatibility. Two views, one source.

### R5 — `bitmap_scratch_` lifetime / Selector copy semantics

`Selector` is copyable (`selector.h:48`). The bitmap_scratch_ member
adds a `std::vector<uint8_t>` to the copy footprint. Cost: negligible
(small vector for empty Selectors; zero for default-constructed
ones). No semantic change.

### R6 — Cache size unbounded across long-running processes

A long-running daemon (e.g. PyBALL with many one-shot Expressions)
could fill the 1024-entry cache, evicting hot entries. LRU plus a
configurable capacity covers the common case; pathological workloads
get a knob. Acceptable.

### R7 — String-pool interner growth

Interning every name a user ever queries causes monotonic pool
growth. For long-running processes this is unbounded. Mitigation:
`compact()` rebuilds the pool, dropping unreferenced entries. K0.5
ships the basic interner; pool compaction is added with the broader
`MoleculeStore::compact()` work (already on the K0.2 roadmap).

---

## 10. Acceptance criteria — K0.5 done

1. All 9 expression/selector tests of the K0 green-bar pass with
   **zero** edits to test sources.
2. `CompiledExpression_test.C` is added and passes: ≥100 expressions,
   byte-identical bitmap equality vs the legacy per-Atom path.
3. `CompiledExpressionRe-entrancy_test.C` is added; passes under
   `-fsanitize=thread`.
4. `benchmarks/selector_bench.C` is added; recorded in
   `baseline/benchmarks.txt`; shows ≥10× speedup vs the recorded
   pre-K0.5 baseline for the same corpus.
5. `Selector_test` wall time satisfies the
   `KERNEL-V2-DECISIONS.md` D11 / `V2.0-ROADMAP.md` K0.5 gate
   (≥10× over its current baseline number).
6. `ExpressionParser::state` static field is **deleted** (or, if a
   downstream caller is found in audit, a mutex-guarded shim ships
   with a deprecation comment).
7. Public surface of `Expression`, `ExpressionTree`,
   `ExpressionParser`, `Selector`, `StandardPredicates` unchanged
   (verified by `nm` diff or by header diff against v1.7).
8. New surfaces `CompiledExpression`, `CompiledExpressionCache`,
   `MoleculeStore::intern_name`, `*_data()` column accessors added
   per §3, §4.2.

---

## 11. Provenance

- D11 — locked 2026-05-17, `KERNEL-V2-DECISIONS.md` §2.
- Baseline numbers — `baseline/tests.txt` 2026-05-17T14:14:54Z.
- Parser-re-entrancy bug — observed in
  `source/KERNEL/expressionParser.C:146-169` +
  `include/BALL/KERNEL/expressionParser.h:172-180`.
- Predicate inventory — from
  `source/KERNEL/expression.C:209-239`
  (`registerStandardPredicates_`) and
  `include/BALL/KERNEL/standardPredicates.h:56-932`.
- Existing Selector loop — `source/KERNEL/selector.C:84-99`.
- MoleculeStore column accessors target — `moleculeStore.h:89-131,
  267-312`.
- K0.5 phase gate — `V2.0-ROADMAP.md` K0.5 row;
  `KERNEL-V2-DECISIONS.md` §5 K0.5 row.

*End of COMPILED-SELECTION-SPEC.md. This is the K0.5 contract.*
