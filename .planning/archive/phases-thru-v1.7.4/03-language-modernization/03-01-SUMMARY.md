---
phase: 03-language-modernization
plan: 01
subsystem: STL-compat
tags: [cpp17, stl-modernization, refactor]
dependency_graph:
  requires: []
  provides: [LANG-02-grep-gate]
  affects: [include/BALL/CONCEPT/predicate.h, include/BALL/MATHS/parsedFunction.h, include/BALL/KERNEL/standardPredicates.h, include/BALL/STRUCTURE/residueRotamerSet.h, include/BALL/DATATYPE/hashGrid.h, source/CONCEPT/enumerator.C, source/STRUCTURE/DNAMutator.C]
tech_stack:
  added: []
  patterns: [lambda-predicate, lambda-transform, lambda-for_each]
key_files:
  created: []
  modified:
    - include/BALL/CONCEPT/predicate.h
    - include/BALL/MATHS/parsedFunction.h
    - include/BALL/KERNEL/standardPredicates.h
    - include/BALL/STRUCTURE/residueRotamerSet.h
    - include/BALL/DATATYPE/hashGrid.h
    - source/CONCEPT/enumerator.C
    - source/STRUCTURE/DNAMutator.C
decisions:
  - "D-01: Dropped unary_function/binary_function base inheritance entirely — no typedef hand-rolling"
  - "D-02: Rewrote all adapter call sites as lambdas (not std::mem_fn/std::bind)"
metrics:
  duration: "83 seconds"
  completed: "2026-05-14"
  tasks_completed: 3
  files_modified: 7
---

# Phase 03 Plan 01: C++17 STL Construct Removal Summary

**One-liner:** Removed all six C++17-deleted STL adapters (`unary_function`, `binary_function`, `bind2nd`, `mem_fun`, `mem_fun_ref`, `not1`) from 7 files by dropping base-class inheritance and rewriting call sites as lambdas.

## Tasks Completed

| Task | Name | Commit | Files |
|------|------|--------|-------|
| 1 | Drop unary_function/binary_function base inheritance (D-01) | 518c1ff | predicate.h, parsedFunction.h, standardPredicates.h, residueRotamerSet.h |
| 2 | Rewrite bind2nd/mem_fun/mem_fun_ref/not1 call sites as lambdas (D-02) | 8dc6d73 | hashGrid.h, enumerator.C, DNAMutator.C |
| 3 | Full-tree grep gate — zero matches confirmed | (no commit, verification only) | — |

## What Was Done

### Task 1: Base-class inheritance removal

Four headers had classes/structs inheriting from `std::unary_function` or `std::binary_function` — both removed in C++17.

- **`predicate.h`**: Removed `using std::unary_function;` and `using std::binary_function;` from the `BALL` namespace. Removed `: public unary_function<T, bool>` from `UnaryPredicate` and `: public binary_function<T1, T2, bool>` from `BinaryPredicate`.
- **`parsedFunction.h`**: Removed file-scope `using std::unary_function;` (outside any namespace). Removed `: public unary_function<arg, double>` from `ParsedFunction`.
- **`standardPredicates.h`**: Removed `: public binary_function<const CTPNode*, const CTPNode*, bool>` from `struct greater_`.
- **`residueRotamerSet.h`**: Removed `: std::binary_function<Rotamer, Rotamer, bool>` from `struct RotamerProbabilityGreaterThan_`.

No `argument_type`/`result_type` typedefs were hand-rolled per D-01 decision.

### Task 2: Call-site lambda rewrites

Three files had call sites using C++17-removed/deprecated adapter factories.

- **`hashGrid.h`** (`countNonEmptyBoxes`): `std::count_if(..., std::not1(std::mem_fun_ref(&HashGridBox3<Item>::isEmpty)))` → `std::count_if(..., [](const HashGridBox3<Item>& box) { return !box.isEmpty(); })`. Both `mem_fun_ref` (hard removal) and `not1` (deprecated) folded into one lambda.
- **`enumerator.C`** (line ~133): `std::bind2nd(std::multiplies<Position>(), modulus)` → `[modulus](Position m) { return m * modulus; }`. Semantics preserved: each existing multiplier is multiplied by the new modulus.
- **`DNAMutator.C`** (lines ~247, ~258): Two `std::mem_fun(&Atom::select/deselect)` calls (pointer-element containers) → `[](Atom* atom) { atom->select(); }` and `[](Atom* atom) { atom->deselect(); }`.

### Task 3: Full-tree grep gate

`grep -rn 'unary_function|binary_function|bind2nd|bind1st|ptr_fun|auto_ptr|mem_fun' include/ source/` returned **zero matches** (exit code 1). No stragglers found beyond the 7 planned files.

## Decisions Made

- **D-01 applied**: Dropped base-class inheritance entirely; did not preserve `argument_type`/`result_type` typedefs. These were not consumed anywhere in the codebase.
- **D-02 applied**: Rewrote all adapter call sites as lambdas (not `std::mem_fn`/`std::bind`). Lambda formulations chosen for maximum readability and zero header dependencies.
- **`#include <functional>`** left in place in headers that had it — Plan 03's build loop will surface unused-include warnings if warranted (as planned).

## Deviations from Plan

None — plan executed exactly as written. The grep gate (Task 3) confirmed no stragglers beyond the 7 files.

## Known Stubs

None.

## Threat Flags

None — pure STL-adapter refactor with no new trust boundaries, network surface, or data-handling changes.

## Self-Check: PASSED

- `include/BALL/CONCEPT/predicate.h` — exists, no unary_function/binary_function
- `include/BALL/MATHS/parsedFunction.h` — exists, no unary_function
- `include/BALL/KERNEL/standardPredicates.h` — exists, no binary_function
- `include/BALL/STRUCTURE/residueRotamerSet.h` — exists, no binary_function
- `include/BALL/DATATYPE/hashGrid.h` — exists, count_if with lambda
- `source/CONCEPT/enumerator.C` — exists, std::transform with lambda
- `source/STRUCTURE/DNAMutator.C` — exists, two for_each with lambdas
- Commit 518c1ff — verified in git log
- Commit 8dc6d73 — verified in git log
- Full-tree grep gate: exit code 1 (zero matches)
