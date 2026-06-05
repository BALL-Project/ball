# V22-H4-APPLY-AUDIT — Composite::apply caller inventory

**Status:** Audit ledger for H4 commit 3 (per V22-H4-DESIGN.md R7,
D-H4.12 R3).
**Authored:** 2026-06-04.
**Scope:** non-VIEW, non-PYTHON production + test code.

## Audit predicate

Two grep predicates run over `source/` + `test/` + `include/`,
excluding `VIEW/` and `PYTHON/` directories:

(A) **Explicit type-template form** (`.apply<T>(`):
```
rg -tcpp '\.apply<[A-Z][a-zA-Z]*>\(' source test include
```
→ **0 hits in production code.**

(B) **Runtime-dispatched form** (`.apply(processor)` /
`->apply(processor)`):
```
rg -tcpp '->apply\(|\.apply\(' source test
```
→ **849 hits.**

## Category I — Explicit `apply<T>` template form (target of D-H4.12)

| Category | Count |
|---|---|
| Production code | 0 |
| Tests | 0 |
| Headers (definitions) | 5 (`composite.h:1758,1784,1810,1816,1822,1844`) |

D-H4.12 R3 names `Composite::apply<T>()` as the free template
entry point that "dies when the molecular classes drop their
Composite base." The audit confirms there are NO production
callers of the type-template form; the entry point only exists as
overloaded method declarations on `Composite` itself.

**Resolution:** the templates are DELETED at H4 commit 8 alongside
the `Atom : Composite` deletion. No separate commit-3 edit needed
for the call-site migration because there are no call sites.

Commit 3's substantive value is THIS audit ledger landing the
finding and allowing commits 4-13 to proceed without holding a
commit-3 placeholder.

## Category II — Runtime-dispatched `.apply(processor)` form

849 call sites. These do NOT use the type-template entry point —
they use the runtime virtual dispatch on `Composite::apply` which
returns true/false based on the processor's traversal. The migration
target is the handle-aware `StructureQuery::apply` already shipped
in H2c + HCP-2.

**The migration of these 849 sites is the substance of commits
4-11** (handle-yielding iteration commits + v0 class deletion +
AtomContainer collapse). Each commit migrates the call sites in
its scope; the audit ledger here documents the count for the
benefit of the next implementor.

Spot check of where the 849 sites live (top 10 directories by
hit count):

| Directory | Approx hit count |
|---|---|
| `source/STRUCTURE/` | ~200 |
| `source/FORMAT/` | ~150 |
| `source/MOLMEC/` | ~100 |
| `source/SOLVATION/` | ~50 |
| `source/QSAR/` | ~50 |
| `source/SCORING/` | ~50 |
| `source/DOCKING/` | ~50 |
| `source/KERNEL/` | ~40 |
| `test/` | ~200 |
| other | ~10 |

These don't break at H4 because they dispatch to the same v0
processor; they only need migration when the underlying virtual
machinery retires. Each is handled within its cluster's H4
commit (commits 4b, 5b/5c/5d/5e, 6, 7b).

## Verification

The audit is reproducible from the repo at HEAD using the two `rg`
commands above. The H4-DR-cycle audit predicates (per V22-H4-DESIGN.md
R7) are documented for the bridge audit (commit 11.5), PDB audit
(commit 5a, 5 categories), iterator audit (commit 4a), AtomContainer
audit (commit 7a), and now apply audit (this commit 3).

---

*Authored 2026-06-04 as the H4 commit 3 deliverable per D-H4.12.*
