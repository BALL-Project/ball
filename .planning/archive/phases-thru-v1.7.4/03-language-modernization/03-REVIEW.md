---
phase: 03-language-modernization
reviewed: 2026-05-14T00:00:00Z
depth: standard
files_reviewed: 12
files_reviewed_list:
  - CMakeLists.txt
  - cmake/BALLCompilerSpecific.cmake
  - include/BALL/CONCEPT/predicate.h
  - include/BALL/DATATYPE/hashGrid.h
  - include/BALL/KERNEL/standardPredicates.h
  - include/BALL/MATHS/parsedFunction.h
  - include/BALL/STRUCTURE/residueRotamerSet.h
  - source/COMMON/exception.C
  - source/CONCEPT/enumerator.C
  - source/FORMAT/SDFile.C
  - source/KERNEL/expressionParserParser.y
  - source/STRUCTURE/DNAMutator.C
findings:
  critical: 0
  warning: 1
  info: 4
  total: 5
status: issues_found
---

# Phase 3: Code Review Report

**Reviewed:** 2026-05-14
**Depth:** standard
**Files Reviewed:** 12
**Status:** issues_found

## Summary

This phase performs a focused C++14 to C++17 migration: dropping the removed
`std::unary_function` / `std::binary_function` base classes, rewriting
`bind2nd` / `mem_fun` / `mem_fun_ref` / `not1` call sites as lambdas, removing
the dynamic exception specification on `ExpressionParsererror`, dropping the
removed `std::set_unexpected` call, qualifying `std::streampos`, and bumping
`CMAKE_CXX_STANDARD` to 17 while deleting the now-redundant raw `-std=` flags.

The migration is well-scoped and the diff is small. I reviewed every lambda
rewrite for behavioural equivalence against the construct it replaced — all
four are faithful translations (see verification notes below). No critical
issues found. One warning concerns a latent functional-correctness bug in
`HashGrid3::apply` that the diff passes through unchanged but which sits
directly in code touched by this phase; the remaining items are informational
(redundant includes, residual deprecated constructs out of this phase's scope).

### Behavioural-equivalence verification (lambda rewrites)

- `hashGrid.h:1350-1352` — `std::not1(std::mem_fun_ref(&HashGridBox3::isEmpty))`
  became `[](const HashGridBox3<Item>& box) { return !box.isEmpty(); }`.
  `mem_fun_ref` invokes the member on a reference; `count_if` passes elements
  of `box_` (a `vector<HashGridBox3<Item>>`) by reference. `not1` negates the
  bool result. The lambda is exactly equivalent. **OK.**
- `enumerator.C:133` — `std::bind2nd(std::multiplies<Position>(), modulus)`
  became `[modulus](Position m) { return m * modulus; }`. `bind2nd` fixes the
  second argument, so the call was `m * modulus`. `modulus` is captured by
  value (a `Size`), matching `bind2nd`'s copy semantics. **OK.**
- `DNAMutator.C:247,258` — `std::mem_fun(&Atom::select/&Atom::deselect)` became
  `[](Atom* atom) { atom->select(); }` / `{ atom->deselect(); }`. `mem_fun`
  (pointer form) expects a pointer argument; `to_optimize_` is a container of
  `Atom*`, so the element type matches. Both `select()` and `deselect()` return
  `void`, and `for_each` discards the result. **OK.**
- `expressionParserParser.y:9-10,124` — removing
  `throw(Exception::ParseError)` from the declaration and definition of
  `ExpressionParsererror`. Dynamic exception specifications were removed in
  C++17; the function body still throws `Exception::ParseError` unconditionally,
  so observable behaviour is unchanged (the spec only ever affected
  `std::unexpected` dispatch, which no longer exists). **OK.**

### C++17 correctness verification

- `exception.C:332-335` — `std::set_unexpected(terminate)` removed. `std::unexpected`
  / `set_unexpected` were removed in C++17 alongside dynamic exception specs and
  are never invoked at runtime in C++17, so dropping the call is correct and
  loses no behaviour. `std::set_terminate` and `std::set_new_handler` are
  retained. **OK.**
- Residual `throw()` (empty) and `throw(SomeException(...))` *statements*
  elsewhere in the tree (e.g. `file.h`, `file.iC`, `exception.h`) were checked:
  empty `throw()` is still valid (deprecated) in C++17 and only removed in
  C++20, and `throw(expr)` in `file.iC` is a throw *statement* with redundant
  parentheses, not a dynamic exception spec. Neither blocks a C++17 build, so
  they are correctly left out of this phase's scope.

## Warnings

### WR-01: Pre-existing use-after-typo bug in `HashGrid3::apply` sits in modernized file

**File:** `include/BALL/DATATYPE/hashGrid.h:1684`
**Issue:** `HashGrid3<Item>::apply(UnaryProcessor<Item>&)` declares the inner
loop as `for (typename HashGridBox3<Item>::DataIterator *item = box->beginData(); +item; ++item)`.
`beginData()` returns a `DataIterator` by value, not a pointer; binding it to a
`DataIterator*` and then dereferencing via `processor(*item)` is not
behaviourally equivalent to iterating the data items and will not compile as
written for any instantiated `Item`. The sibling overload at line 1711 and the
`HashGridBox3::apply` implementations (lines 685-705) use the correct
by-value-iterator form. This is a pre-existing defect, not introduced by Phase
3, but it lives in `hashGrid.h` — a file this phase modified (line 1350-1352) —
and the function is otherwise dead-by-non-instantiation, which is why it has
survived. Flagging because the phase's stated goal is behavioural equivalence
of the modernized file, and a reviewer/maintainer touching this file should be
aware the file does not fully compile if `HashGrid3::apply(UnaryProcessor<Item>&)`
is ever instantiated.
**Fix:** Outside this phase's scope to fix, but if addressed:
```cpp
for (typename HashGridBox3<Item>::DataIterator item = box->beginData(); +item; ++item)
{
    result = processor(*item);
    ...
}
return processor.finish();   // also: line 1695 uses processor->finish() on a reference
```
Recommend filing a separate issue rather than expanding Phase 3's scope.

## Info

### IN-01: `<functional>` include now likely redundant in `predicate.h`

**File:** `include/BALL/CONCEPT/predicate.h:12`
**Issue:** With `std::unary_function` / `std::binary_function` and the
`using std::unary_function;` / `using std::binary_function;` declarations
removed, `predicate.h` no longer references anything from `<functional>`. The
`#include <functional>` is now dead unless retained intentionally as a
transitive convenience for downstream includers.
**Fix:** Consider removing the include, or leave it if downstream files were
relying on the transitive include (low risk either way). Not required for
correctness.

### IN-02: `<numeric>` include likely redundant in `parsedFunction.h`

**File:** `include/BALL/MATHS/parsedFunction.h:12`
**Issue:** `parsedFunction.h` includes `<numeric>` but the file uses no
`<numeric>` facilities (`accumulate`, `iota`, etc.) — only `<cmath>` functions
via the `(double(*)(double))&sin` casts, which come in transitively. This
include predates the phase, but the removal of the `using std::unary_function;`
line on the adjacent lines makes it a natural co-cleanup.
**Fix:** Consider dropping `#include <numeric>` (and possibly adding an explicit
`#include <cmath>`). Cosmetic; no behavioural impact.

### IN-03: Removed namespace-scope `using` declarations may have leaked to downstream code

**File:** `include/BALL/CONCEPT/predicate.h:14-17`, `include/BALL/MATHS/parsedFunction.h:11`
**Issue:** The old `predicate.h` exported `using std::unary_function;` /
`using std::binary_function;` into namespace `BALL`, and `parsedFunction.h`
exported `using std::unary_function;` at global scope. Any other translation
unit that included these headers and relied on the unqualified name leaking
through will now fail to compile. A tree-wide grep found no remaining
`unary_function` / `binary_function` references in `include/` or `source/`, so
the BALL/VIEW tree itself is clean — but out-of-tree consumers (plugins,
`ball_contrib`-era code, user code) could be affected.
**Fix:** No action needed within this repo. Worth a one-line note in the phase
summary / release notes that these namespace-polluting `using` declarations
were removed, since it is a (minor) public-header API change.

### IN-04: `using namespace std;` retained in `expressionParserParser.y`

**File:** `source/KERNEL/expressionParserParser.y:7`
**Issue:** The file still carries a file-scope `using namespace std;`. This is
pre-existing and not introduced by the phase, and the phase's only change here
(removing the dynamic exception spec) is correct. Noting it only because the
generated parser is now C++17 and `using namespace std;` at file scope is a
known footgun (e.g. ADL collisions with `std::` names); a future cleanup pass
could qualify the two `std` uses explicitly and drop the directive.
**Fix:** Defer; out of scope for Phase 3.

---

_Reviewed: 2026-05-14_
_Reviewer: Claude (gsd-code-reviewer)_
_Depth: standard_
