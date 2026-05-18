# K0 Adversarial Review — Codex CLI Round 4 (2026-05-18)

**Status:** Complete. Codex verdict: **no-go for K0.5/K0.6** until HIGH
findings are addressed. Triage and remediation tracked in
`KERNEL-V2-DECISIONS.md` under K0.4.4-K0.4.8 phases.

**Reviewer:** Codex CLI 0.128.0 (`codex exec`).
**Subject:** K0.3b.LATER.1-10 (commits `ec86c5d3d..b0f3576b7`) plus
K0.4.1-4.3 (commits `58a2daff5..b5fed89ee`). Atom is now a thin
store-backed handle; all v1.x payload fields deleted.

---

## Findings (Codex, verbatim summary)

| # | Sev | Finding | Status |
|---|---|---|---|
| 1 | HIGH | Cross-store `Atom::swap` is broken — vector/charge swaps only happen when both atoms share a store; mirror_all path handles names/element/scalars only. Cross-store produces hybrid state. | OPEN → K0.4.4 |
| 2 | HIGH | Same-store `Atom::swap` has TWO independent mechanisms (direct vector swap + snapshot mirror_all) — fragile, no dedicated test. | OPEN → K0.4.4 |
| 3 | HIGH | `System::adopt(Atom&)` sequential adoption orphans bonds; test `MoleculeStore_test.C:582-607` locks this in as expected. Contradicts comment in `system.C:43-45`. | OPEN → K0.4.5 (forbid bonded single-atom adopt OR forwarding state) |
| 4 | HIGH | Global orphan store not thread-safe — concurrent `Atom()` default ctor races on vector growth + free-list. | OPEN → K0.4.6 (mutex on orphan-only path) |
| 5 | HIGH | D7 `BorrowedColumnRef` enforcement not actually wired — `getPosition/Velocity/Force` return raw refs and never inc/dec the borrow counter. | OPEN → K0.4.7 (either remove claim or wire) |
| 6 | HIGH | New `getName()` returns `const String&` into `std::vector<String>` — dangling on store growth. `getTypeName()` returns `String` by value already (inconsistent). | OPEN → K0.4.4 (return by value) |
| 7 | MEDIUM | Element pointer identity lost — custom `Element` instances canonicalised through `PTE[atomic_number]` on readback. | Document as v2.0 regression in DECISIONS; not blocking. |
| 8 | MEDIUM | `allocate_atom(Atom*)` is not atomic — wraps `allocate_atom()` then sets back_ptr. Race window survives. | OPEN → K0.4.6 |
| 9 | MEDIUM | `store_ == nullptr` gates in `persistentRead/set/operator=/clear_` silently drop data. | OPEN → K0.4.8 (BALL_ASSERT) |
| 10 | LOW | `sizeof(Atom)` unmeasured after Composite/PropertyManager bases retained. | DEFERRED → K0.7 perf phase |

PTE[0] is safe (returns Element::UNKNOWN per `PTE.iC:218-222`).

---

## Remediation Plan (K0.4.4 — K0.4.8)

| Phase | Scope | Effort |
|---|---|---|
| **K0.4.4** | Atom::swap rewrite: single snapshot/write model for ALL store-backed fields; getName returns String by value; add Atom::swap regression tests (same-store, cross-store) | 0.5 day |
| **K0.4.5** | System::adopt(Atom) bonded-rejection guard or forwarding-state design; remove or document the misleading comment | 0.5 day |
| **K0.4.6** | Orphan-store mutex; fold back_ptr into atomic allocate_atom path (single critical section) | 0.5 day |
| **K0.4.7** | D7 enforcement audit: either remove the claim (downgrade to "documented contract") or migrate hot callers to BorrowedColumnRef proxy | 0.5-1 day |
| **K0.4.8** | Replace `store_ == nullptr` gates in atom.C with `BALL_ASSERT(store_)` (or rebind to orphan); audit System::~System ordering | 0.5 day |

Total: 2.5-3 days. After K0.4.8 green sweep, gate-check K0.5 (compiled
selection) and K0.6 (persistence v2) entry.

---

## Why "no-go" Right Now

Codex's argument is sound: K0.5 (compiled selection) and K0.6 (persistence
v2) both rely on the store being the single source of truth with
deterministic mutation semantics. Half-completed swap, hidden bond loss,
and racy default construction would propagate corruption into both
downstream phases. Path-2 staged migration requires the foundation be
correct *before* dependents land.
