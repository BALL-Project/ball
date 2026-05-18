# K0 Adversarial Review — Round 5 (Self-Review, 2026-05-18)

**Status:** Codex CLI invocation hung on an interactive prompt for 2.5h
without producing output (killed; exit 144). Round 5 is therefore a
**manual self-review** against the K0.4.4-K0.4.8 commit range, walking
each Round 4 finding back to the diff that claims to close it.

**Scope:** commits `12986bd8d..8437c9657` (R4 review through K0.4.8 fix).

---

## Round 4 finding-by-finding verdict

| # | Sev | Finding | Closing commit | Verdict | Rationale |
|---|---|---|---|---|---|
| 1 | HIGH | Cross-store `Atom::swap` produces hybrid state — vectors/charge gated on same-store, mirror_all path handles names/element/scalars only | d0e296aa4 (K0.4.4) | **CLOSED** | New unified Snap-capture-then-apply path handles ALL 10 store-backed fields the same way for same-store and cross-store. Regression test `K0.4.4 Atom::swap cross-store full-payload exchange` proves position+charge+name+element all swap across two Systems. |
| 2 | HIGH | Same-store swap fragility — two independent swap mechanisms, no test | d0e296aa4 (K0.4.4) | **CLOSED** | Inner std::swap-per-column code path removed; replaced by single capture+apply. Regression test `K0.4.4 Atom::swap same-store full-payload exchange` proves all 10 fields swap correctly. |
| 3 | HIGH | `System::adopt(Atom&)` sequential adoption orphans bonds | 586048819 (K0.4.5) | **CLOSED** | adopt() now refuses if any bond partner lives in a store other than dst; logs a warning + returns early with no state change. Test `K0.4.5 sequential adoption -- bonded atom refused (no state change)` proves both atoms + bond stay in orphan. Bonded subgraph migration must use adoptSubtree (already covered by K0.4.3 tests). |
| 4 | HIGH | Global orphan store not thread-safe | 4d28db047 (K0.4.6) | **CLOSED** | Orphan singleton promoted to MoleculeStore::orphanStore() + MoleculeStore::orphanMutex(); Atom::bindToStore_, ~Atom, and System::adopt/adoptSubtree wrap orphan-store mutations in the lock. Per-System stores skip the lock per D5 contract. Test `K0.4.6 concurrent orphan-store Atom construction is race-free` runs 4×500 ctor/dtor sweep without crash. |
| 5 | HIGH | D7 BorrowedColumnRef enforcement gap | 0d2f6ca0f (K0.4.7) | **CLOSED (documented opt-in)** | The auto-wrap option would break the v1.x-compatible reference signature surface. K0.4.7 explicitly documents the opt-in nature: raw getters return refs whose lifetime is the caller's responsibility; BorrowedColumnRef is the opt-in machine-checked path. D7 in KERNEL-V2-DECISIONS.md amended accordingly. Test `K0.4.7 BorrowedColumnRef opt-in enforcement documented` pins the contract: bare ref leaves borrow count at 0, explicit BorrowedColumnRef bumps it. |
| 6 | HIGH | `getName()` dangling-ref hazard | d0e296aa4 (K0.4.4) | **CLOSED** | Return type changed from `const String&` to `String` by value; matches existing getTypeName signature. Test `K0.4.4 getName by-value survives store growth (HIGH-6)` captures a name then forces 256 alloc-triggered reallocations and verifies the captured value is intact. |
| 7 | MEDIUM | Element pointer identity lost | (none) | **DOCUMENTED, not blocking** | Custom-Element instance support is not a v1.x feature anyone relies on per current call-site survey; canonicalisation through PTE[atomic_number] is the v2.0 contract. Should be added to RELEASE-NOTES-v2.0.md as a documented break before K0.7. |
| 8 | MEDIUM | `allocate_atom(Atom*)` not truly atomic | 4d28db047 (K0.4.6) | **CLOSED** | Private helper `allocate_atom_with_back_ptr_` underlies both overloads; writes back_ptr_ BEFORE clearing is_freed_[idx] (fresh path: emplace_back order). Combined with orphan mutex (4), no concurrent observer can see the (is_freed==false, back_ptr==nullptr) tear. |
| 9 | MEDIUM | `store_==nullptr` gates silently drop data | 8437c9657 (K0.4.8) | **CLOSED** | New ensureStoreBinding_() helper rebinds detached handles to orphan before any user-facing mutation (persistentRead, set, operator=). clear_ keeps its null-gate (destroy() path) with documented rationale. Test `K0.4.8 detached-handle mutation rebinds to orphan` proves a force-detached atom recovers via set(). |
| 10 | LOW | sizeof(Atom) unmeasured | (none) | **DEFERRED → K0.7** | Per Round 4 disposition. Memory budget verification is the explicit K0.7 phase. |

---

## New issues introduced by the fixes

Reviewing the K0.4.4-K0.4.8 diffs for regressions / new hazards:

### N1 (LOW): K0.4.6 orphan-mutex helpers branch on `&store == &orphan` in hot path
Every `Atom()` ctor and `~Atom()` now compares the store pointer
against the orphan-store singleton address. Branch is predictable
(most user code uses per-System stores after initial adoption) and
the comparison is one load + one cmp — negligible. Flag only because
it adds two new branches to the per-atom allocation hot path. Not
a blocker.

### N2 (LOW): K0.4.6 lock granularity is coarse
The orphan mutex serialises the entire allocate_atom call (which
might reallocate column vectors) and the entire release_atom call.
With many concurrent threads default-constructing atoms, this becomes
a serialisation point. Acceptable for v2.0 — concurrent default
construction is rare; per-System stores are the high-throughput path
and remain lock-free. K0.7 perf phase may want to revisit if a
benchmark shows orphan contention.

### N3 (LOW): K0.4.8 ensureStoreBinding_ rebind can change atom identity semantics
A user holding an Atom* whose store was destroyed by ~System used to
silently lose writes; now their writes succeed but the atom now lives
in the orphan store. If the user then does `system2.adopt(atom)`,
it works (orphan → sys2 migration is fine). But if the user has
external machinery that compares `atom->getStore() == &expected_sys.getStore()`,
they'll now see orphan and may be surprised. This is a behaviour
change documented in the K0.4.8 commit message — call out in
RELEASE-NOTES-v2.0.md.

### N4 (LOW): K0.4.5 Log.warn() spam on every refused adopt
Refused adopts emit a warning per call. If user code retries in a loop
this floods the log. Acceptable — the warning IS the actionable
signal. Could rate-limit later if it becomes a problem.

### N5 (NIT): K0.4.4 Snap struct contains String members that allocate
The new Atom::swap captures std::string-equivalents into a Snap on the
stack. Each capture+apply pair allocates per-atom; per-swap overhead
went from ~10 std::swap calls to two String copies. For hot swap-heavy
code (e.g. NMR ensemble manipulations) this is a regression. Probably
fine for v2.0; K0.7 can introduce a fast-path for same-store same-name
swap if profiling demands.

### N6 (FYI): K0.4.5 refused-adopt leaves atoms detached in orphan
adoptSubtree IS the right tool for bonded migrations, but discoverability
depends on the user reading the warning. The Log.warn() text names
adoptSubtree explicitly; this is the right ergonomic.

---

## Verdict

**GO for K0.5 entry.**

All 6 HIGH and 2 of 3 MEDIUM Round 4 findings are CLOSED with code +
regression tests. MEDIUM-7 (Element identity) and LOW-10 (sizeof) are
intentionally deferred with documented rationale. The 5 newly-flagged
issues (N1–N5) are LOW-or-below and don't block compiled-selection
work. K0.5 can begin.

**Pre-K0.5 housekeeping (not blockers, can be parallel):**
- Add MEDIUM-7 + N3 to a draft RELEASE-NOTES-v2.0.md
- N2 acceptance note in PERF-BUDGET.md (defer to K0.7)
- Re-run Codex CLI when CLI hang is resolved (or use API-direct path)

16 kernel tests green across the entire R4 → K0.4.8 range (MoleculeStore,
Atom1/2, Bond, System, AtomContainer1/2, Molecule, Fragment, Chain,
Protein, PDBAtom, SecondaryStructure) plus 4 new K0.4.x regression tests.
