# V22-H4-BRIDGE-CONSUMER-AUDIT — Dual-existence bridge migration ledger

**Status:** Audit ledger for H4 commit 11.5 (per V22-H4-DESIGN.md
R7, D-H4.4 R4).
**Authored:** 2026-06-04.
**Scope:** non-VIEW, non-PYTHON production + test code.

D-H4.4 R4 mandates the bridge audit be specified by symbol grep +
5-category classification, NOT a fixed numeric scope. This ledger
runs the audit and classifies the hits.

## Audit predicate

Per D-H4.4 R4:

```
rg -tcpp '\bback_ptr\b|\bbond_back_ptr\b|\bbond_idx_of\b|\bbond_sid_of\b' include source test
```

with `source/VIEW` and `source/PYTHON` EXCLUDED.

**Total hits at HEAD `5df46a461`: 121 lines across 35 files.**

## Five-category breakdown

| Category | Approx count | Files | Action at commit 11.5 |
|---|---|---|---|
| (a) **Handle accessor inline** — atomHandle.h / bondHandle.h reach into the bridge for lookup; can become inline table lookup in the same header | 2 hits | 2 files (atomHandle.h, bondHandle.h) | inline the table lookup; remove the bridge call |
| (b) **Production migration** — STRUCTURE / KERNEL production code resolves through the bridge; migrates to sid-keyed sites already added in H3a-d | 22 (STRUCTURE) + several (KERNEL) | 8 files in STRUCTURE; 4 files in KERNEL | per-site rewrite to sid + handle-API |
| (c) **Test-specific bridge use** — tests that verify bridge behavior; tests REWRITE for post-flip behavior | 28 hits | ~8 test files | rewrite tests to assert post-flip handle behavior |
| (d) **Definition site** — moleculeStore.h declarations + moleculeStore.C bodies | 39 hits | 2 files (moleculeStore.h, moleculeStore.C) | DELETED at commit 12 (bridge removal) |
| (e) **Comment / doc** — cosmetic mention; no edit needed | balance to 121 (~30) | several | no edit needed |

## Files referencing the bridge (full list at HEAD `5df46a461`)

35 files total. Definition sites (d) on top; consumer migration
sites (a/b) middle; test rewrite sites (c) lower; comment-only
(e) at the end.

The full file list refreshes at commit 11.5 prep time by
re-running the audit predicate; it is NOT enumerated here because
the symbol drift between today and commit-11.5-prep makes a
frozen list brittle.

## Commit 11.5 plan refresh

Commit 11.5 runs the predicate at commit-prep time, regenerates
this ledger, then applies edits:

  1. Category (e) comment-only: no edit, no commit.
  2. Category (c) test rewrites: rewrite each test to assert
     post-flip behavior using handles. Tests survive the bridge
     deletion at commit 12.
  3. Category (b) production migrations: per-site rewrite to
     sid + handle-API (most patterns already in H3a-d).
  4. Category (a) handle accessor inlining: atomHandle.h /
     bondHandle.h inline the table lookup that used to be
     bridge-mediated.
  5. Category (d) definition sites: LEFT FOR COMMIT 12; the
     bridge methods themselves get deleted then, after all
     consumers have migrated.

Each of steps 2-4 lands as its own sub-commit within 11.5,
following the D-H4.9 per-commit gate (build + ctest +
HandleKeyLeakGate + AssignBondOrder_bench within 5%).

## Drift baseline

Today's counts (HEAD `5df46a461`):
- Total hits: 121
- Files: 35
- Definitions: 39 (32% of total — these dominate)
- Handle headers: 2
- STRUCTURE consumers: 22
- Tests: 28
- Balance (KERNEL + other + comments): 30

If commit-11.5-prep counts drift by more than ±30% from this
baseline, re-run the categorization carefully — the audit
predicate may have caught new categories.

## Reproduction

Run the predicate above at HEAD; the ledger refreshes against
that snapshot.

---

*Authored 2026-06-04 as the H4 commit 11.5 audit-half deliverable
per D-H4.4 R4. The migration-half of commit 11.5 lands later in
the H4 cycle after all v0 classes have been deleted (commits 8-11).*
