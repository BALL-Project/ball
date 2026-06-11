# V22-H4-R3-REMEDIATION-ROADMAP — fixing the R2 adversarial findings

**Status:** DRAFT for adversarial review (Codex + Vibe) before implementation.
**Authored:** 2026-06-11. HEAD at draft: `c66d45cef`.
**Scope:** the 12 open/partial findings (N1–N12) from the H4 R2 dual-reviewer pass.

The R2 pass verified all 5 R1 HIGH fixes CLOSED-at-core. The residue is
7 PARTIAL/OPEN tails + 5 NEW findings. This roadmap groups them into
bounded commits, each with the standard D-H4.9 per-commit gate (build +
290/290 ctest + HandleKeyLeakGate + AssignBondOrder_bench ≤5%).

## Finding inventory (from R2 synthesis)

| ID | Sev | Status | One-line |
|----|-----|--------|----------|
| N1 | HIGH | PARTIAL | bridge has no explicit thread-safety contract documented |
| N2 | HIGH | OPEN | gate misses `deque<Atom*>` / `list<Atom*>` (DNAMutator.h) |
| N3 | HIGH | PARTIAL | D-H4.11.A System-strip ledger not executable (missing methods, fake clear_bonds) |
| N4 | HIGH | NEW | orphan-atom JSON (`system_atom_indices`) contradicts "no bare atoms on System" |
| N5 | HIGH | OPEN | 7b.8 insertBefore/After still uncovered |
| N6 | HIGH | OPEN | unbound-v0 JSON parity case untested |
| N7 | HIGH | OPEN | bit-property test reads v0, not handle |
| N8 | MED | NEW | `MoleculeStore::clear()` leaves stale `container_back_ptr_` tail |
| N9 | MED | NEW | `System::insert(Molecule&)` still delegates through AtomContainer |
| N10 | MED | NEW | selection handle surface incomplete (no countSelectedAtoms) |
| N11 | MED | PARTIAL | post-flip insert sentinel undefined + `[[deprecated]]` not landed |
| N12 | MED | NEW | `clear_freed_back_ptrs_` loop misses tail `[t.size, back_ptr.size)` |

## Design decision up front: N1 (concurrency)

**Resolution: DOCUMENT, do not mutex.** The existing codebase has a
documented **per-System single-threaded-mutator contract**
(`V22-H3c-DESIGN.md:374`, `V22-CODEX-REVIEW-ROUND34.md:36`): all
container-table + side-table mutation on a given System runs on one
thread. The `bond_mutex_` / `orphan_mutex_` exist ONLY for the
process-global orphan store (default-ctor'd atoms from arbitrary
threads). The `container_back_ptr_` bridge mutates in lock-step with
`container_table_`, which is already under the single-thread contract.

Adding a mutex to the bridge ONLY would be inconsistent (the table
itself has none) and would imply a thread-safety guarantee the rest of
the store does not make. The correct fix is to state the contract
explicitly in the bridge accessors' doc-comments + a one-line note in
the design. If true multi-thread mutation is ever required, it is a
store-wide change (table + bridge + columns together), out of H4 scope.

This is the ONE finding where I disagree with the reviewer's
recommended fix; the roadmap records the rationale so the re-review can
challenge it directly.

## Commit plan

### R3.1 (code, MED) — bridge clear()/tail hygiene [N8 + N12]
- `MoleculeStore::clear()`: add `container_back_ptr_.clear()` (or resize
  to 1 + null) wherever side tables are reset, so a cleared store has no
  stale bridge entries.
- `container_back_ptr(idx)`: return `nullptr` when
  `idx >= container_table_size_()` (not just `>= vector.size()`), so a
  shrunk/cleared table can never hand back a stale tail pointer.
- `clear_freed_back_ptrs_`: loop to `container_back_ptr_.size()` (the
  table `is_freed` accessor already bounds-checks; out-of-table indices
  are treated as freed → nulled).
- Test: extend H4TableTopologyInvariant_test — build store, populate
  containers, `clear()`, assert every `container_back_ptr(i) == nullptr`
  for the old index range.
- Gate: full ctest.

### R3.2 (code, HIGH) — handle bit-property read + test coverage tails [N5 + N6 + N7]
- Add `ContainerHandleBase::hasProperty(BALL::Property)` bridge overload
  (mirrors the existing `setProperty/clearProperty(BALL::Property)` at
  containerHandle.h:595). This makes N7 testable through the handle.
- Extend the 7b.8 CHECK: actually call `h_mol.insertBefore(*a_new, *a2)`
  and `insertAfter`, assert order via `h_mol.getAtom(position)` +
  `countAtoms`. [N5]
- Extend the JSON-parity CHECK: add an UNBOUND `Molecule m` (not inserted
  into any System) with properties; assert the handle path either (a)
  matches the PropertyManager& path, or (b) the documented unbound
  contract `{}` — pick (b) and assert it, since unbound handles have no
  store row. Document the contract in propertyJson.h. [N6]
- Rewrite the bit-property CHECK to verify through
  `h_mol.hasProperty(TEST_BIT)`, not `mol->hasProperty`. [N7]
- Gate: full ctest + the 3 new CHECKs must PASS.

### R3.3 (code, HIGH) — gate widening: deque/list + DNAMutator [N2]
- `HandleKeyLeakGate.cmake`: add `deque|list|forward_list|multiset|multimap`
  to BOTH the qualified and unqualified container alternations.
- Re-run; surface any newly-caught sites; add each to GATE_ALLOWLIST
  with its retiring follow-on commit (DNAMutator.h confirmed; sweep for
  others).
- Gate: full ctest (HandleKeyLeakGate must still PASS via allowlist).

### R3.4 (design, HIGH) — System-strip ledger + orphan-atom + concurrency note [N1 + N3 + N4 + N9]
- **N1:** add a "Bridge concurrency contract" note to D-H4.16 + doc-
  comments on `container_back_ptr` / `set_container_back_ptr` /
  `clear_freed_back_ptrs_` stating the per-System single-thread
  contract.
- **N3:** replace D-H4.11.A's hand-written 14-row table with a
  method-by-method ledger DERIVED from system.h. Each row: method,
  impl source, post-strip owner, exact replacement. Remove the fake
  `getStore().clear_bonds()` reference (no such API); specify the real
  bond-clear path.
- **N4:** decide the orphan-atom JSON contract. Options: (a) keep a
  System-root orphan path post-flip (System owns a hidden orphan
  Molecule); (b) declare `system_atom_indices` a schema break with a
  loader migration. RECOMMEND (a) — orphan atoms are a real BALL
  use-case (HIN/PDB loaders produce them); document the post-flip
  storage (System's store holds the atom rows with parent_container_idx
  == CONTAINER_NONE, already the case today). Add a round-trip test to
  the ledger's verification list.
- **N9:** add a "System molecule-insertion rewrite" sub-commit (11.B')
  to the ledger: rewrite `System::insert(Molecule&)` etc. against the
  store/table API BEFORE the base strip, so commit 11.D has no
  AtomContainer dependency.
- Gate: design-doc only; no build.

### R3.5 (code, MED) — post-flip sentinel + deprecation marker [N11]
- Define `AtomHandle()` (default/invalid) as the "append at end"
  sentinel for the frozen `insertBefore/After(AtomHandle, AtomHandle)`
  signatures; cross-store / wrong-kind markers are a no-op (documented).
  Add to D-H4.11.B.
- Land `[[deprecated("H4 commit 8: use AtomHandle/ContainerHandleBase positional insert")]]`
  on the current `Composite&`-taking overloads in containerHandle.h
  (the 4 overloads at 559-578). Verify it does not break the existing
  ContainerHandle_test (which calls them) by either marking the test's
  usage with a pragma push/ignore, or routing the test through the
  new surface.
- Gate: full ctest; deprecation warnings must not become errors.

### R3.6 (design, MED) — selection handle surface [N10]
- Design-only: add `ContainerHandleBase::countSelectedAtoms()` /
  `countSelectedAtomContainers()` to the planned handle surface, wired
  to `selected_bits_`, scheduled under a named commit (7b.15). Record
  in V22-H4-DESIGN.md's pending list. DO NOT implement now — it depends
  on the selected_bits_ rollup that H4 commit-cluster owns; landing a
  v0-bridge version would just be churn deleted at commit 12.
- Gate: design-doc only.

## Ordering rationale

1. R3.1 first — smallest, closes the two NEW MED store-hygiene bugs that
   could mask future bridge issues.
2. R3.2 + R3.3 — the HIGH code tails; independent, can land in either
   order.
3. R3.4 — the HIGH design cluster; unblocks the commit-11 plan.
4. R3.5 + R3.6 — MED tails; R3.5 has a code-half, R3.6 is design-only.

R3.1, R3.2, R3.3, R3.5 are code (gated on ctest). R3.4, R3.6 are design.

## Out of scope / deferred

- True multi-thread store mutation (N1's "real" fix) — store-wide,
  not H4.
- N10 implementation — deferred to commit 7b.15 (needs selected_bits_
  rollup).
- The actual System base strip (commit 11.A–11.E) — this roadmap only
  makes its ledger executable, it does not perform the strip.

---

## ROADMAP REVIEW CORRECTIONS (Codex GO-WITH-FIXES + Vibe NO-GO, 2026-06-11)

Dual-reviewer pass on this roadmap. Convergent verdict: the *intent* is
right but R3.1 as drafted is **wrong in three concrete ways**, plus one
new HIGH lifetime finding. Corrections folded below; implementation
follows the corrected spec.

### Verified-DEFENSIBLE (no change)
- **N1 document-don't-mutex** — DEFENSIBLE. `moleculeStore.h:357` +
  `V22-H3c-DESIGN.md:373` document per-System single-mutator UB; the
  bridge mutates in lock-step with the table under that contract. BUT
  Codex adds: R3.4 must explicitly state H2/D73 (free-standing
  containers have no store, materialized at adoption) SUPERSEDES D56's
  orphan-container-store model — else the bridge would need orphan-store
  locking. Folded into R3.4.
- **N6 unbound contract (b)** — DEFENSIBLE. No production caller
  serializes an unbound container through `ContainerHandleBase` (systemJson
  uses `PropertyManager&`; moleculeStoreJson uses Atom/BondHandle; the
  only container-handle caller is the test). Choice (b) = document `{}`.
  Also narrow `propertyJson.h:64` "byte-identical" claim to "valid bound
  handles only".
- **R3.5 [[deprecated]] build-safety** — DEFENSIBLE. No `-Werror`
  (`cmake/BALLCompilerSpecific.cmake` uses `-Wall -Wextra` only); the 4
  Composite&-overloads have ZERO production callers (the v0
  `insertBefore` hits are AtomContainer, not the handle). FIX the
  roadmap's false claim: the test does NOT currently call them; R3.2
  introduces the first call.

### MUST-FIX before implementation

**C1 (was R3.1, HIGH) — out-of-range is_freed false premise.**
`ContainerTable::is_freed()` returns *false* for `idx >= rows_.size()`
([moleculeStore.C:377](source/KERNEL/moleculeStore.C:377)). So looping
`clear_freed_back_ptrs_` to `back_ptr_.size()` and calling `is_freed(i)`
will NOT null tail entries `[t.size(), back_ptr_.size())`. **Corrected
spec:** null slot `i` when `i >= t.size() || t.is_freed(i)`.

**C2 (was R3.1, HIGH) — accessor must check BOTH bounds.**
Tests allocate rows directly through `ContainerTable`
([ContainerHandle_test.C:81](test/ContainerHandle_test.C:81)), bypassing
the bridge resize, so `table_size > back_ptr.size()` is a REAL state.
**Corrected spec:** `container_back_ptr(idx)` returns null when
`idx == 0 || idx >= container_table_size_() || idx >= container_back_ptr_.size()`
(intersection of both bounds). Vibe's "keep back_ptr.size only" and
Codex's "add table_size" are both satisfied by the AND-of-bounds.

**C3 (Codex R3-N3, MED) — bound set_container_back_ptr.**
`set_container_back_ptr` resizes from `idx` alone, manufacturing bridge
tails for non-existent rows ([moleculeStore.C:881](source/KERNEL/moleculeStore.C:881)).
**Corrected spec:** no-op when `idx == 0 || idx >= container_table_size_()`;
only `container_create_` grows the bridge.

**C4 (Codex R3-N2, HIGH/NEW) — surviving container UAF.**
`~System` runs `destroy()` while `store_` is alive (member dies after
body) — so the COMMON case (auto-deletable children) is safe. But
`Composite::clear()` detaches NON-auto-deletable children without
unbinding ([composite.C:1598](source/CONCEPT/composite.C:1598)); such a
survivor keeps `container_row_store_` pointing at the soon-freed store,
and its later `~AtomContainer` derefs it via the F1-added
`setContainerRowBinding_(0,0)` unbind → UAF. **Corrected spec:** in
`~System`, BEFORE `destroy()`, recursively unbind all container rows
(reuse the `unbindContainerRowsBelow_` pattern) while the store is alive.
Then auto children unbind as no-ops, non-auto survivors read as unbound.

**C5 (N4, HIGH) — orphan-atom post-flip storage must be concrete.**
`systemJson.C:481` restores orphans via `sys.AtomContainer::insert(...)`
which is deleted post-flip. **Corrected spec for R3.4:** orphan atoms
stay as store rows with `parent_container_idx == CONTAINER_NONE` (already
the runtime state); the loader's post-flip replacement is
`store.adopt_atom_` + leave parent unset, NOT a hidden Molecule (which
would be a schema hazard if ever serialized). Keep the
`system_atom_indices` schema. Add to the D-H4.11.A ledger with a
round-trip test in the verification list.

### Ordering correction
Land **R3.3 (gate widening) BEFORE R3.2 (test extension)** so any new
test code is checked against the widened gate (Vibe R3-N4). Verified the
current test file is clean, so this is belt-and-suspenders, but free.

### Non-issues confirmed
- MoleculeStore move/copy/swap: copy/assign `= delete`
  ([moleculeStore.h:135](include/BALL/KERNEL/moleculeStore.h:135));
  declaring copy suppresses implicit move; no swap API. Add explicit
  `= delete` moves as a one-line guard (free).

### Corrected commit order
1. **R3.1′** — bridge hygiene: C1 (loop) + C2 (accessor) + C3
   (set_ guard) + C4 (~System sweep) + `clear()` nulling + explicit
   move-delete. One commit, with tests (clear-nulls-bridge +
   non-auto-survivor no-UAF).
2. **R3.3** — gate widening (deque/list/...) + allowlist sweep.
3. **R3.2** — handle `hasProperty(Property)` + test tails (insert,
   unbound-contract, handle-side bit read).
4. **R3.4** — design: System ledger refresh + orphan-atom concrete
   path (C5) + D56-vs-D73 supersede note (N1) + selection surface (N10).
5. **R3.5** — sentinel definition + `[[deprecated]]` markers.

*Corrected spec locked. Implementation proceeds against this section,
not the original draft.*
