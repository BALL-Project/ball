# BALL 2.2 — Milestone Kickoff

**Status:** PLANNING (W0 design lock pending).
**Authored:** 2026-05-20.
**Predecessor:** v2.1.0 (commit `fdb871bb2`, tagged 2026-05-20).
**Branch:** `v2.2` (forked from the `v2.1.0` tag per D29-REV / option B).
**Decision log:** continues the D-series from v2.1 (ends at D44) →
v2.2 starts at **D45**.

This is **the thin-handle flip** — the memory release that v2.0 and
v2.1 deliberately set up. It closes the D13 budget by deleting the
D2/D3/D4 base-class inheritance from Atom and Bond.

---

## Why v2.2 exists

v2.0 shipped the SoA `MoleculeStore` but kept `Atom`/`Bond`
inheriting `Composite` + `PropertyManager` + `Selectable` as
thin-stub bases — so `sizeof(Atom)` is **368 B** and the full
per-atom footprint is **~507 B/atom, ~3.2× over the D13 budget
(160 B/atom)**. The SoA columns alone are 139 B/atom (under
budget); the entire miss is the fat handle.

v2.1 built the substrate:

- **Side tables exist** (`_moleculeStoreInternal.h`):
  `composite_nodes_` (5-link tree), `property_columns_` +
  `sparse_bag_` (typed property columns), `selected_bits_` (atomic
  word array). Populated only by direct-API tests so far.
- **RTTI is consolidated** — all CORE_ONLY Atom-RTTI routes through
  `BALL::detail::compositeAsAtom_`, so the flip replaces **one**
  helper, not 29 sites.
- **`BALL_EMPTY_BASES`** macro is in place for MSVC EBO.

v2.2 does the actual flip: wire mutations to the side tables, then
delete the inheritance so Atom/Bond become true thin handles.

---

## Scope (in)

| ID | Item | Source | Sized |
|---|---|---|---|
| **V22-MUTATION-WIRING** | Wire every `Composite`/`PropertyManager`/`Selectable` mutation to maintain the side tables. **Prerequisite for the flip.** | D39 | L |
| **V22-ATOM-THIN-HANDLE** | Delete `Composite`/`PropertyManager`/`Selectable` inheritance from `Atom`; `Atom : D17Composite + D17PropertyManager + D17Selectable` empty shims forwarding to side tables. `sizeof(Atom)` 368 → ~32 B. | D26a, D40 | L |
| **V22-BOND-THIN-HANDLE** | Same surgery on `Bond`. 296 → ~16 B. | D40 | M |
| **V22-GENERATION-GUARD** | Debug-mode stale-handle detection with **slot-generation** semantics (the naive store-generation compare was rejected in v2.1, D44). | D44 (supersedes D25) | M |
| **V22-STORE-ITER-API** | Public `MoleculeStore::iterAtoms()`/`iterBonds()` — now genuinely skips Atom* materialisation. | D44 | M |
| **V22-MSVC-CI** | Windows MSVC + vcpkg CI job; verify EBO collapses the empty bases (the ≤32 B claim is MSVC-dependent). | D34d | M |
| **V22-VIEW-RTTI** | The 54 Atom-RTTI sites in VIEW + APPLICATIONS. | D37 | M |
| **V22-CI-PERF-GATES** | Pinned-baseline median+MAD CI comparator (deferred from v2.1 P5). | D44 | S |
| **V22-D13-CLOSURE** | Verify full-fat ≤160 B/atom — the milestone success gate. | D13 | — |

---

## Scope (out)

- **New science / formats / force-fields** — v2.2 is infrastructure.
- **PyBALL v2** — Phase 6, gated on the binding-tool bake-off.
- **BALLView Qt6 / renderer swap** — v1.8 / Phase 4-5, orthogonal.
- **`master` trunk promotion** — per D29-REV (option B), the v2.x
  line stays on its branches; promoting v2.x to `master` is a
  separate future decision, not v2.2.

---

## The crux risk

The inheritance flip is the **highest-risk surgery of the entire
v2.x line**. It touches, simultaneously:

1. **Tree-node identity** — `Composite*` is the iterator payload
   type; `AtomIterator` filters `CompositeIterator`.
2. **RTTI / dynamic_cast surface** — `dynamic_cast<Atom*>(composite)`
   stops compiling once Atom isn't a Composite (mitigated: 1 helper).
3. **Selection tree propagation** — `Composite::deselect()` walks
   parent-side counters that go stale if Atom selection writes only
   `selected_bits_`.
4. **Adoption boundary** — `System::adopt`/`adoptSubtree` migrate
   only core columns today; must migrate side-table state too.
5. **PropertyManager full surface** — bit properties, named-property
   iteration, persistence, `set/swap/operator=`.
6. **Destruction order** — the P2.1.1 trap: mutation mirroring during
   `~System` cascade corrupts the heap. D39's bet is that the new
   D17* hierarchy gives clean destructor-order control.
7. **SIP Python bindings** — `appendChild(Composite&)` etc. (deferred
   if PyBALL stays off).

R20 found 8 BLOCKERs analysing this for v2.1. v2.2 must have a
genuine design answer for each before any flip code lands.

---

## Phasing (proposed)

| Phase | Name | Gate |
|---|---|---|
| **W0** | Design lock | Answer the load-bearing design questions (below); ADR-style D45+ decisions; **Codex review (R29)** before W1. |
| **W1** | Mutation wiring | Wire Composite/PM/Selectable mutations to the side tables, *with Atom still inheriting v0 bases* (dual-write). Solve the destruction-order trap here. Side-table parity tests assert the tables now track production mutations. |
| **W2** | Atom thin-handle flip | Delete D2/D3/D4 from Atom; reads + writes go to side tables; v0 inline state deleted. `sizeof(Atom)` drops. The big one. |
| **W3** | Bond thin-handle flip | Same for Bond. |
| **W4** | Generation guard + slot semantics | Stale-handle detection that doesn't false-trip. |
| **W5** | STORE-ITER-API | Public store-native iteration. |
| **W6** | VIEW/APPLICATIONS RTTI + MSVC CI | Broader RTTI cleanup + Windows EBO verification. |
| **W7** | Perf gates + D13 verification + release | Confirm ≤160 B/atom; pinned-baseline gates; v2.2.0-rc1 → v2.2.0. |

Per-phase Codex review at planning + close (the cadence that caught
~22 issues across v2.1).

---

## Load-bearing design questions (W0 — for the maintainer)

These shape everything downstream. The recommendations are mine;
the calls are the maintainer's.

### Q1. Destruction order — how is the P2.1.1 trap actually solved?

The bet (D39) is that flipping to `D17Composite` gives clean
destructor control. But concretely: when `~System` cascades into
child destruction, the order in which an Atom releases its
side-table rows (composite node, property columns, selection bit)
vs the store's own teardown must be deterministic and
non-interleaved.

**Options:** (A) `~D17Composite` releases the handle, ordered before
the store frees columns; (B) the store owns all side-table rows and
Atom destruction is a pure index-release (no back-mutation into a
half-destroyed store); (C) defer all releases to a single
`store.compact()`/teardown pass.

*My lean: B* — the store owns the rows; Atom dtor just marks its
slot freed. No Atom→store back-mutation during teardown.

### Q2. Selection tree propagation — counters or recompute?

`Composite::deselect()` relies on parent-side counters
(`number_of_selected_children_`). Post-flip, Atom selection lives in
`selected_bits_`. Either the counters stay (and the bitmap writes
also bump parent counters — keeps O(1) queries but couples Atom
selection to the tree) or selection-state queries recompute from the
bitmap on demand.

*My lean: keep the counters* — molecule-level "is anything selected"
queries are common; recompute would be O(atoms) per query.

### Q3. AtomIterator — how does it work when Atom isn't a Composite?

`AtomIterator` is `CompositeIterator` filtered by
`KernelPredicate<Atom>`. If Atom isn't a Composite, the
CompositeIterator never yields it. Either (A) Atom *stays* in the
composite tree via the `composite_nodes_` side table (the tree node
exists; only the inline pointers move to the store), so iteration is
unchanged; or (B) AtomIterator is reimplemented over store rows.

*My lean: A* — the side table preserves the tree-node identity;
`D17Composite` forwards `getParent()`/iteration through it. Atom is
still "in the tree," just with its tree links in the side table.
This keeps AtomIterator + `apply()` working.

### Q4. Adoption — migrate side-table state in adopt/adoptSubtree.

`System::adopt` must migrate composite-node links + property columns
+ selection bit alongside the core SoA columns. This is mechanical
but must be complete (R20-3 flagged data-loss risk).

*Confirm: in scope for W1/W2; no real alternative.*

### Q5. PropertyManager surface — full forwarding or narrowed?

v2.0's `PropertyManager` exposes bit properties, named-property
iteration (`beginNamedProperty`), persistence, index access. The
`D17PropertyManager` shim must forward all of it to the column +
sparse-bag store, OR v2.2 narrows the surface (a breaking change).

*My lean: full forwarding* (preserve the surface; v2.1 D26 promised
it). Named-property iteration over columns+sparse needs a
materialising adapter.

### Q6. Public API break tolerance — same as v2.1 (preserve)?

D26 (v2.1) committed to preserving the `Atom`-as-`Composite` API
surface. v2.2 deletes the inheritance but can keep the *methods*
(`atom->getParent()`, `atom->setProperty()`, `atom->select()`)
forwarding to side tables. Confirm v2.2 keeps source-compat (no
consumer rewrites), accepting the indirection cost — or allows a
controlled break.

*My lean: preserve source-compat* (forward through shims), deprecate
nothing in v2.2; revisit in v3.0.

### Q7. Bond representation — does W3 also reconcile the dual graph?

v2.1 P4.2 reconstructs the Atom-side `Bond*` graph on JSON load, but
bonds are still dual-represented (store BondRecords + Atom `bond_[]`
+ heap `Bond*`). Does the Bond thin-handle flip (W3) unify these
(bonds become pure store rows, `Bond*` becomes a handle), or just
shrink the Bond object?

*My lean: unify* — make Bond a store-backed handle like Atom; the
`bond_[]` array becomes a store-CSR query. Bigger, but it's the
coherent end state and resolves V21-BOND-GRAPH-RECONSTRUCT's root.

---

## Success criteria

- `sizeof(Atom) ≤ 32 B`, `sizeof(Bond) ≤ 24 B` (pinned in Sizeof_test
  under Clang/GCC/**MSVC**).
- **D13 met**: full-fat ≤160 B/atom at 100k atoms (MemoryBudget_test
  gate flips from "documented over" to "asserted under").
- All v2.1 tests still pass (zero regression); selection speedup
  still ≥10× median; JSON round-trip unchanged.
- Side tables are the production source of truth (not just test-exercised).
- MSVC CI green.
- Per-phase Codex review; zero BLOCKERs shipped.

---

## Estimated shape

This is a multi-phase, multi-week milestone — the largest of the
v2.x line. W1 (mutation wiring) + W2 (Atom flip) are the long poles
and the highest risk. Realistic: W0 design lock first (with the
maintainer answering Q1-Q7), then iterate phase by phase with the
adversarial-review cadence.

## Next action

Maintainer answers Q1-Q7 (or "go with your leans"). Then W0 writes
the D45+ decisions, Codex R29 reviews the design lock, and W1
(mutation wiring) begins.
