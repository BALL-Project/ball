# v2.1-P2 — Thin-Handle Flip: PLAN

**Phase:** P2 (the inheritance flip; highest-risk phase in v2.1).
**Status:** Planning.
**Authored:** 2026-05-19 (immediately post-R19 P1 close).
**Decisions consumed:** D22b, D23b, D24b, D25, D26a, D31b, D32b, D34c, D35.

## Goal

Delete `Composite + PropertyManager + Selectable` base classes from
`Atom` and `Bond` and replace with three **empty shim base classes**
(`D17Composite`, `D17PropertyManager`, `D17Selectable`) that
**forward all API calls** to `MoleculeStore`'s side tables (the
P1-shipped infrastructure).

**Phase delivers:**
- `sizeof(Atom)` drops from 360 B (v2.0) to ≤ 32 B (v2.1 target).
- `sizeof(Bond)` deferred to P3.
- All v2.0 public API on Atom remains source-compatible:
  `atom->getParent()`, `atom->getDegree()`, `atom->setProperty(...)`,
  `atom->isSelected()`, etc. — every call works unchanged because
  the shim forwards transparently.
- The side-table mutation wiring P1 deferred lands here: every
  Composite mutation on an Atom updates `composite_nodes_`; every
  PropertyManager mutation updates `property_columns_` or
  `sparse_bag_`; every Selectable mutation updates `selected_bits_`.

## Out of scope for P2

- `Bond` thin-handle (deferred to P3).
- Removing `D2`/`D3`/`D4` from non-Atom Composites (Molecule,
  Chain, Residue, etc. — they keep v0 inheritance until v2.2;
  the side-table flip only applies to Atom in P2 since Atom is
  the high-volume case).
- JSON closures (P4).
- Perf gates (P5).
- Generation-guard debug-mode trap (P5).

## Phase gate (success criteria)

1. `sizeof(Atom) ≤ 32 B` on Apple Clang arm64 (`Sizeof_test` pin
   updated; current 360 B pin REPLACED with new value). Failure
   blocks the commit.
2. `Atom` inherits ONLY from `D17Composite`, `D17PropertyManager`,
   `D17Selectable` (each marked `BALL_EMPTY_BASES`-friendly), plus
   any v2.0-specific bases that remain (e.g. `BALL_EXPORT` is a
   visibility tag, not a base).
3. Every v2.0 Atom-API call returns the SAME value as the v2.0
   inline-state implementation would have. Existing tests are the
   regression suite (especially Atom_test1/2, AtomContainer_test1/2,
   AtomIterator_test, Selector_test, all 282 v2.0 tests).
4. `SideTableParity_test` extended with end-to-end parity assertions:
   `atom.setProperty("PARTIAL_CHARGE", 0.5f)` immediately reflects
   in `store.sideTables_().property_columns_.findColumn(...)`; same
   for tree mutations and selection toggles.
5. CORE_ONLY ctest: 284/284 PASS preserved.
6. `MoleculeStore_test` 100-run stress: 0/100 flakes.
7. Codex R21 close review.

## Risk register

- **R-P2.A (BLOCKER risk):** `Atom` is the single most-used type
  in BALL. Any incomplete forwarding in D17Composite/PM/Selectable
  causes a flood of compile errors across the full Track B surface.
  *Mitigation:* land P2 in many small commits, each preserving
  source compatibility. Use a "shim emits same vtable / same
  observable behavior" rule. Verify each commit builds the full
  ctest, even if a forwarding method is `assert(false); abort();`
  as a placeholder.
- **R-P2.B (MEDIUM risk):** v2.0 ABI consumers (e.g. PyBALL) that
  derive from `Atom` may break if the inheritance graph changes
  shape. *Mitigation:* document Atom's new public bases in
  RELEASE-NOTES; v2.1 already breaks ABI by definition, so this
  is documented + accepted.
- **R-P2.C (MEDIUM risk):** Composite-tree operations on Atom
  intermix with operations on non-Atom Composites (Molecule's
  `appendChild(Atom&)`). The shim's `appendChild` must call the
  non-Atom parent's v0 inline appendChild AND maintain the Atom-
  side `composite_nodes_` entry. Cross-class state stays
  consistent.
   *Mitigation:* the inserting Composite (parent) does v0 inline
   work; the Atom's shim does ONLY its own `composite_nodes_`
   maintenance. No cross-class mutation.
- **R-P2.D (LOW risk):** Free-store memory accounting changes —
  Atom-only `delete` no longer destroys the parent_/sibling_/etc.
  pointers (because they don't exist on Atom anymore). v0 Atom
  destructor needs explicit side-table `release_composite_node_`.
  *Mitigation:* `~D17Composite` calls release; tested via
  destructor unit test.

## Sub-phases

P2 splits into **9 atomic commits**:

| # | Subject | Deliverable | Gate |
|---|---|---|---|
| **P2.1** | Define `D17Composite` empty shim class with API mirror of `Composite` | new file `include/BALL/CONCEPT/d17Composite.h` + `.iC` + `.C`. NO subclass yet uses it. Forwarding implementations are placeholders that delegate to a friend-provided `reach_store_()` helper (returns nullptr if no store reachable). | builds; new test `D17Composite_test` constructs + queries an instance |
| **P2.2** | Define `D17PropertyManager` empty shim class | new file `include/BALL/CONCEPT/d17PropertyManager.h` + `.iC` + `.C`. API mirror of `PropertyManager`. | builds; `D17PropertyManager_test` round-trips setProperty/getProperty via the column registry |
| **P2.3** | Define `D17Selectable` empty shim class | new file `include/BALL/CONCEPT/d17Selectable.h` + `.iC` + `.C`. API mirror of `Selectable`. | builds; `D17Selectable_test` round-trips select/deselect via `selected_bits_` |
| **P2.4** | Swap Atom's inheritance | `Atom : public BALL_EMPTY_BASES D17Composite, public D17PropertyManager, public D17Selectable`. Existing inline state on Composite (parent_, etc.) ... wait, Composite still has its state. Approach: keep v0 Composite as-is; Atom inheritance swap means Atom no longer inherits Composite. Internally Atom needs a Composite shim that returns a pointer to a dynamically-allocated v0 Composite? No — the side table replaces this. | new `Sizeof_test` pin: `sizeof(Atom) ≤ 32 B` |
| **P2.5** | Wire `D17Composite` to read from `composite_nodes_` | implement getParent / getFirstChild / getLastChild / getDegree / getChild / countDescendants / etc. via `store.sideTables_().node_(handle)`. | all Composite-on-Atom reads return same values as v2.0 |
| **P2.6** | Wire `D17Composite` to write to `composite_nodes_` | appendChild / removeChild / insertBefore / insertAfter / splice* / destroyChildren_ / destroy: maintain side-table topology. THIS IS THE DEFERRED P1.3/P1.6/P1.8 WIRING. | all Composite mutations on Atom reflect in `composite_nodes_` |
| **P2.7** | Wire `D17PropertyManager` to property_columns_ + sparse_bag_ | setProperty: well-known column fast-path; type-mismatch sparse fallback; sparse-first lookup precedence per D23b. clearProperty: tombstones dense + erases sparse. | atom.setProperty/getProperty roundtrip; mismatched-type preserved per D23b |
| **P2.8** | Wire `D17Selectable` to selected_bits_ | select / deselect / setSelected / isSelected: atomic word ops. Tree-propagation `contains_selection_` book-keeping deferred to v2.2 (out-of-scope per P2). | atom.select() / isSelected() roundtrip via `is_selected_` |
| **P2.9** | Bond's per-bond Composite stays inline (P3 territory). Final regression + parity verification + commit. | 284/284 ctest still PASS; sizeof pins hold |

**Estimated effort:** 2-3 days of execution + 1 R20 (planning)
review + 1 R21 (close) review.

## Codex review gates

Per D33b:
- **R20:** P2 planning review on this document, BEFORE any P2 code.
- **R21:** P2 close review after all 9 sub-phases land.

## Next action

After this plan commits: spawn R20 (Codex CLI). Wait for GO.
Then execute P2.1 → P2.9 sequentially.

P2 cannot start until R20 returns GO. The risk profile (R-P2.A
especially) makes a planning review especially valuable here.
