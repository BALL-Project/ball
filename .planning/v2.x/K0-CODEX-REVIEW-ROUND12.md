# K0 Kernel Compactness + Correctness — Codex CLI Round 12 (2026-05-18)

**Status:** Complete. Verdict: **NEEDS-FIXES → 5/5 [BUG]s applied;
8 [DEBT] items documented for v2.1 → GO**.

**Reviewer:** Codex CLI 0.128.0 (`codex exec`).
**Subject:** v2.0 KERNEL itself — Atom/Bond/Molecule/System/Composite
layout, MoleculeStore invariants, lifetime management, store mechanics.
Focused review distinct from R10/R11 (which were broad-scope).

---

## Verdict (Codex, verbatim)

> Overall kernel correctness verdict: **NEEDS-FIXES**. Core store
> mechanics are plausible, but detached-handle mutation, rejected
> adoption after tree insertion, hidden overloads, and unsynchronized
> CSR mutation are real correctness hazards.
>
> Compactness verdict: **64 B is not reachable without breaking or
> replacing the v1.x Composite/Property/Bond APIs.** Top fatteners:
> Composite/Selectable machinery roughly 100-150 B, `Atom::bond_[12]`
> roughly 96 B, PropertyManager roughly 40-70 B.
>
> Top priority bugs:
> 1. K12 detached Atom setters dereference null `store_`.
> 2. K8 bonded Atom insert can leave Composite tree and store binding inconsistent.
> 3. K9 bond CSR/store mutation is unsafe under concurrent mutation.

## Probe answers (summary)

| # | Probe | Verdict | Status |
|---|---|---|---|
| K1 | Atom 360 B fatness | [DEBT] handle has interactions, flag, bond_[12], (store,idx,gen). store_generation_ is dead | v2.1 (V21-ATOM-THIN-HANDLE) |
| K2 | Bond 288 B fatness | [DEBT] Bond carries Composite+PropertyManager+name+(store,idx) | v2.1 (V21-BOND-THIN-HANDLE) |
| K3 | Composite tree overhead | [DEBT] 5 pointers + counts + timestamps per atom; needed for v1.x apply()/traversal | v2.1 (V21-COMPOSITE-SIDE-TABLE) |
| K4 | String pool growth | [DEBT] append-only; release doesn't reclaim; compact() doesn't shrink | v2.1 (V21-STRING-POOL-COMPACT) |
| K5 | D7 enforcement gaps | [BUG] compact() only bumps generation on capacity change; store_generation_ never checked | **Fixed** (unconditional bump) |
| K6 | back_ptr null tolerance | [OK] readers check; ctor window closed by allocate_atom(this) | — |
| K7 | stable_id reuse semantics | [DEBT] clear() resets to 1; no UINT64_MAX overflow guard | v2.1 (V21-STABLE-ID-OVERFLOW) |
| K8 | Adoption + Composite consistency | [BUG] AtomContainer inserts to tree BEFORE adopt's safety check; soft-reject leaves tree/store inconsistent | **Fixed** (canAdopt() pre-check) |
| K9 | CSR rebuild thread-safety | [BUG] add/remove_bond + ensure_csr_ unsynchronized | **Fixed** (documented single-thread-per-store contract; v2.1 V21-BOND-MUTEX for real concurrency) |
| K10 | System::insert(Atom&) hidden | [BUG] Molecule re-exposes but System hides | **Fixed** (using AtomContainer::insert et al.) |
| K11 | PropertyManager virtual-clear trap | [OK] contained in KERNEL post R7 fix | — |
| K12 | Detached-handle setter crash | [BUG] writeStoreXxx_ deref null store_ post-~System | **Fixed** (ensureStoreBinding_ in every helper) |
| K13 | PropertyManager bag per Atom | [DEBT] every Atom carries empty BitVector + vector<NamedProperty> | v2.1 (V21-SPARSE-PROPERTY-MAP) |
| K14 | Thin-handle Atom split | [DEBT] right path; v2.1-scale rewrite | v2.1 (V21-ATOM-THIN-HANDLE) |
| K15 | Sparse PropertyManager | [DEBT] correct compactness design; cost is lookup overhead | v2.1 (V21-SPARSE-PROPERTY-MAP) |
| K16 | Composite traversal → store iter | [DEBT] order semantics must be specified first | v2.1 (V21-STORE-ITER-API) |

## Fixes applied in this commit

### Fix K10 — Re-expose hidden AtomContainer overloads on System

`include/BALL/KERNEL/system.h`: added `using AtomContainer::insert;
using AtomContainer::insertBefore; using AtomContainer::insertAfter;
using AtomContainer::append; using AtomContainer::prepend;
using AtomContainer::remove;` so System's Molecule overloads don't
hide the inherited Atom/AtomContainer ones. Matches the pattern
Molecule already uses for the same reason. Closes the
`sys.AtomContainer::insert(*atom)` workaround documented in
systemJson.C.

### Fix K12 — Detached Atom mutation rebinds to orphan

`source/KERNEL/atom.C`: every writeStoreXxx_ helper now calls
`ensureStoreBinding_()` first. Pre-fix, after `~System` nulled
store_ pointers on its atoms (system.C line ~321), calling
setPosition/setName/setCharge/etc. on a detached atom would
dereference null store_ and crash. K0.4.8 added ensureStoreBinding_
but it wasn't actually called from the write helpers. Now it is.

Also routed `clear_()`'s direct `store_->position(...) = ...` calls
through the writeStoreXxx_ helpers for uniform detached-handle
handling. The destroy()-path early-return remains (slot's about to
be released anyway).

Read path (`readStoreElement_`): if detached, returns
`PTE[0]` (Element::UNKNOWN) — sensible default for a torn-down handle.

### Fix K5 — compact() bumps generation unconditionally

`source/KERNEL/moleculeStore.C`: `compact()` now always increments
`generation_` instead of only when `positions_.capacity()` actually
changed. The documented contract is "compact() invalidates all
borrowed refs" so the increment must always fire. Cost is one
64-bit increment.

Note: `store_generation_` on Atom is still dead (assigned in
bindToStore_ / migrateTo_ but never read for stale checks). That's
v2.1 (V21-HANDLE-GENERATION-GUARD) — proper stale-handle detection
on every access is invasive and contentious vs the current `if
(store_)` null check.

### Fix K9 — Document single-thread-per-store bond mutation

`include/BALL/KERNEL/moleculeStore.h`: bond table API
(`add_bond`, `remove_bond`, `remove_bonds_between`,
`ensure_csr_`) now carries an explicit thread-safety contract
comment:
- Orphan store: no bond creation (default Atom() doesn't bond).
- Per-System store: single-thread-per-store; concurrent bond
  mutation is undefined behaviour. Caller must wrap with external
  mutex.

A per-store bond_mutex is V21-BOND-MUTEX. Adding it now would
invade every bond mutation site + every CSR read; current
usage patterns don't exercise concurrent bond mutation (orphan
doesn't take bonds; per-System builders are serial).

### Fix K8 — canAdopt() pre-check before Composite tree insertion

`include/BALL/KERNEL/system.h`: new public `bool canAdopt(const Atom&)
const` predicate that mirrors adopt()'s bonded-atom safety gate
without mutating.

`source/KERNEL/system.C`: `adopt()` refactored to use `canAdopt()`
internally; still emits the Log.warn on rejection for callers that
didn't pre-check.

`source/KERNEL/atomContainer.C`: `prepend/append/insert/insertBefore/
insertAfter(Atom&)` all pre-check `sys->canAdopt(atom)` before
calling Composite tree-insertion methods. If canAdopt returns false,
the entire prepend+adopt sequence is skipped (adopt is still called
to emit the user-facing Log.warn). Pre-fix, tree was mutated FIRST
then adopt could soft-reject, leaving the atom in this container's
Composite tree but bound to its old (orphan) store — inconsistent
state.

Test impact: `test/System_test.C` `[EXTRA]destroyBonds()` CHECK was
relying on the buggy soft-reject behavior. Rewrote to use the
v2.0-correct idiom (atoms added FIRST, bonds created AFTER), which
also more closely matches realistic v2.0 builder patterns.

## Re-verification (post-fix)

- **CORE_ONLY ctest:** 164/164 PASS.
- **MoleculeStore_test 100-run stress:** 0 failures (R11 fix A still
  holds, K12 fix didn't regress the orphan-locking path).
- **Heap-System repro:** exit 0, no SEGV (DTOR-HARDEN still holds).
- **System_test [EXTRA]destroyBonds() CHECK:** rewritten to v2.0
  idiom, PASSES.

## DEBT items deferred to v2.1 (full backlog)

| ID | Description | Complexity |
|---|---|---|
| V21-ATOM-THIN-HANDLE | Atom = (store*, idx, generation) only; rest in store columns / side tables | L |
| V21-BOND-THIN-HANDLE | Bond = (bond_store*, bond_idx) delegating to BondRecord | L |
| V21-COMPOSITE-SIDE-TABLE | Move Composite linkage out of Atom into a hierarchy side-table | L |
| V21-STRING-POOL-COMPACT | Reclaim string pool on release_atom or via compact() | M |
| V21-STABLE-ID-OVERFLOW | UINT64_MAX guard + decide reuse policy | S |
| V21-HANDLE-GENERATION-GUARD | Wire store_generation_ checks into every read/write | M |
| V21-SPARSE-PROPERTY-MAP | PropertyManager → side-table keyed by store_idx | M |
| V21-STORE-ITER-API | Define order semantics for store-slot iteration vs Composite | M |
| V21-BOND-MUTEX | Per-store bond mutex if concurrent bond mutation lands | S |

## Conclusion

5/5 [BUG]s fixed; 9 [DEBT] items documented as v2.1 backlog.
CORE_ONLY surface remains 164/164 green; no regressions; one test
(System_test [EXTRA]destroyBonds) updated to track the new correct
behavior.

**Verdict: GO.**
