# BALL 2.x — Unified Roadmap (kernel modernization track)

**Status:** DRAFT for adversarial review (RR1 pending).
**Authored:** 2026-05-21, mid-v2.2-H2.
**Scope:** the libBALL kernel modernization line (v2.0 → v2.x). Integrates
the v2.2 handle redesign, the v2.1 backlog, the maintainer's new
directives (3-way repo split; `BALL::String` removal), pyBALL v2, and the
VIEW/MSVC cross-cutting items. Companion: `V22-ARCH-HANDLE-MODEL.md`
(handle phasing), `V22-DECISIONS.md` (D45–D74), `v2.1/BACKLOG.md`,
`JOINT-ROADMAP.md` (BALLView 1.6.x track + the two new additions),
`PYBALLV2.md`.

This is the **kernel track**. The **BALLView 1.6.x track** (`ROADMAP.md`,
Phases 1–9 + 999.x) runs in parallel and ships independently; the two
intersect at VIEW-RTTI, MSVC CI, and the eventual repo split.

---

## 0. The throughline (why this order)

The 2.x line is the 1.x→modern-SoA modernization band. One technical
spine drives the sequencing:

> **The single source of truth for the molecular hierarchy moves from a
> C++ `Composite` object tree to flat store-backed SoA data + value
> handles.** Everything else (memory budget D13, the API breaks, the repo
> split, the `String` removal, pyBALL v2) is gated on *where in that
> migration the public libBALL surface stabilises.*

So: finish the handle flip (v2.2) → stabilise the surface → then split
repos / remove `String` / rebuild pyBALL against the stable surface.

---

## 1. Shipped

### v2.0 (tagged) — SoA kernel foundation
- `MoleculeStore` per-System struct-of-arrays (positions/charges/element/
  selection/names + bond table + CSR adjacency).
- `Atom`/`Bond` as `{store, idx, generation}` handles **that still inherit
  `Composite`/`PropertyManager`/`Selectable`** (hybrid — the bloat source).
- Compiled selection (Expression→bitmap), **median 61.8× speedup**.
- JSON persistence (K0.6) replacing the binary-spec plan.
- Full domain-module re-enable (Track B: FORMAT/STRUCTURE/MOLMEC/QSAR/
  SCORING/DOCKING/SOLVATION/NMR/ENERGY). 282/282 tests.

### v2.1 (tagged) — side-table infra + perf + RTTI consolidation
- Side tables on the store (`composite_nodes_`, typed property columns +
  sparse bag, atomic `selected_bits_`) — scaffolding for the flip.
- 24 CORE_ONLY Atom-RTTI sites consolidated → `detail::compositeAsAtom_`
  (the single dynamic_cast site); D41.1 CI grep gate.
- JSON **load 7–14× faster** (O(n²) CSR-rebuild fix); bond-property +
  `Bond*` graph round-trip; median-of-N benchmark harness. 284/284 tests.
- **D13 = PARTIAL**: SoA columns 139 B/atom (under the ≤160 B budget) but
  the *full* `Atom` object is 499 B (over) because D2/D3/D4 thin-stub
  base classes remain. **D13 closes only when the inheritance is deleted
  (v2.2 H4) + verified (H8).**

---

## 2. v2.2 — the handle redesign (IN PROGRESS)

**Decision D55 = A2:** the *entire* molecular hierarchy
(Molecule/Chain/Residue/Protein/SecondaryStructure/Nucleotide/
NucleicAcid/Fragment) becomes store-backed value handles over a flat
**container metadata table**; there is no molecular `Composite` object
tree after the flip. **Version stays 2.2** (D53 — 2.x is the
breaking-modernization band; release notes loud about breaks). Dual
existence (D60): through H1–H3 the v0 object tree is the source of truth
and the store table is a *verified mirror*; the flip (H4) inverts that.

| Phase | Status | Features |
|---|---|---|
| **H0** design lock | ✅ R29→R36d | D45–D74; arch model; API-break ledger seeded; container schema (D58), ChildRef edges (D57), orphan store (D56), dual-existence rule (D60), pointer→handle break policy (D61), JSON `containers` schema bump (D62), naming (D63), handle shape (D64), per-slot generation (D65), encapsulation boundary (D66a), mutation-mirror + destruction guard (D67–D70/D73/D74). |
| **H1a** container table | ✅ R32b GO | `ContainerTable` (rows + per-kind payload + `ChildRef` ordered edges + selection counters), orphan-store container rows + subtree migration, `HierarchyParity_test` skeleton. Store-side only. |
| **H1b** handle types | ✅ R34 GO | public `containerKind.h` (`ContainerKind` + `ContainerChildRef`); `MoleculeStore` scalar container accessors; per-slot u64 generation; `containerHandle.h` = 24 B `ContainerHandleBase` + 8 typed `*Handle` + `as<>()` narrow + debug/Python validity. Reads the table; dual existence. |
| **H2** mutation-mirror + traversal | 🚧 in progress | **H2a ✅ R36d GO**: forward-only v0→table mirror for insert (adoption materialisation) + remove, with the `being_destroyed_` destruction guard (defuses the P2.1.1 heap-corruption trap). **H2b 🚧**: splice/swap/clear/replace mirrored (done), `insertParent` + container `setProperty` + atom `select` + container-property migration (remaining). **H2c**: handle-yielding `AtomIterator`/container iterators/`apply` over `ChildRef`. **H2d**: randomized full-surface parity sweep + H2 close review. |
| **H3** consumer migration | ⏳ | Move modules to the handle API in dependency clusters: **H3a** KERNEL+STRUCTURE → **H3b** MOLMEC/QSAR/SCORING/DOCKING → **H3c** FORMAT (PDB/naming/residue — heaviest) + NMR/ENERGY/SOLVATION. Each cluster its own commit + review. Processors take `Atom` by value (the single largest consumer break). |
| **H4** the flip | ⏳ | Delete `Atom : Composite` + the molecular container objects' inline tree state; store table becomes sole source of truth; retire the parity test + dual existence; **`sizeof(Atom)` drops** (D13 path). Per-kind canonical-name migration (delete v0 class + fwd-decls/friends, rename `*Handle`→canonical, reverse-alias) per D63. |
| **H5** Bond unify | ⏳ | `Bond` fully a handle; remove `Atom::bond_[]`; bond properties → store columns; bond iterators redesigned (D51). |
| **H6** property break + guards + JSON v2 | ⏳ | Property mutable-reference surface break (`NamedProperty&`/`BitVector&` → visitor/value accessors, D49); slot-generation stale-handle detection in `BALL_DEBUG`+`BALL_PYTHON_WRAPPER` (D54/D65); JSON schema **MAJOR bump to 2** (`containers` section, D62) + validation (no cycles/orphans). |
| **H7** VIEW + MSVC | ⏳ | The **54 VIEW/APPLICATIONS Atom-RTTI sites** (V21-VIEW-RTTI/D37) → handle kind tag; **MSVC + vcpkg CI** (V21-MSVC-CI) for EBO/`sizeof` verification on Windows. |
| **H8** D13 verify + release | ⏳ | Assert `sizeof(Atom)` ≤ target (≤32 B handle goal; ≤160 B/atom total budget); pinned-baseline perf gates (V21-CI-PERF-GATES); v2.2.0-rc1 → v2.2.0. |

**H2b carry-overs surfaced this session (place in H2b/H2d):** `insertParent`
+ free-standing `replace` need a *materialise-the-new-member* path (like
`adoptSubtree`), distinct from re-derive; the container-property migration
in `materialiseContainer_` (R32 carry-over).

---

## 3. v2.1 backlog → placement

| Backlog item | Placed in |
|---|---|
| V21-MSVC-CI-PORTABILITY | **H7** (also unblocks the Windows leg of the repo split) |
| V21-VIEW-RTTI (54 sites) | **H7** |
| V21-MUTATION-WIRING (D39) | **H2** (now active — H2a/H2b) |
| V21-STORE-ITER-API (public `iterAtoms/iterBonds`) | **H2c** (handle iterators) / **H3** |
| V21-GENERATION-GUARD (atom handle staleness) | **H6** (containers already done in H1b/D65) |
| V21-BIT-PROPERTY-COLUMN (packed-bool cols) | **H6** *if* profiling shows the sparse path is hot; else backlog |
| V21-CI-PERF-GATES (pinned-baseline comparator) | **H8** |
| JSON load batching (close save/load ~15× gap) | **H6** (alongside the JSON v2 schema work) or v2.2.x |
| AndNode tmp-bitmap reuse (selection perf) | v2.2.x perf backlog (small, isolated) |
| Element-instance-id-table (custom-Element JSON round-trip) | **H6** (JSON v2) or v2.2.x |
| Track B FORMAT/STRUCTURE full `Expression_test`/`Selector_test` surface | audit in **H3c** (FORMAT cluster) |
| K0.6.4 v1→v2 JSON converter | **H6** (the schema MAJOR bump makes a converter a release item) |

---

## 4. Post-v2.2 milestones (the new directives + pyBALL)

Sequenced AFTER v2.2 because each consumes the *stabilised* libBALL handle
surface; doing them earlier would churn against the in-flight flip.

### v2.3 — `BALL::String` → `std::string` (kernel API modernization)
- Delete the bespoke `BALL::String`; use `std::string` + a small
  `BALL::StringUtils` free-function namespace for the conveniences
  (tokenise/getField, trim, case, parse/format, substitute).
- Wide signature break (name/id getters, FORMAT parsers, persistence) →
  its own API-break ledger; fold into the H4/H6 break wave if scheduled
  close, else a clean v2.3.
- **Gap/decision:** maintainer said "2.0" — interpret as the 2.x line;
  confirm whether this rides the v2.2 break wave or is a separate v2.3.

### v2.4 — repo / build / test separation (3-way)
- Split into independent repos + CMake builds + CI + versioning:
  **libBALL** (kernel, no Qt/Python), **libVIEW + BALLView** (consumes
  installed libBALL via `find_package`), **pyBALL** (consumes libBALL /
  libVIEW). Compatibility matrix; no in-tree source coupling.
- **Hard dependency:** must follow v2.2 (handle API stable) AND ideally
  v2.3 (no `String` to marshal). Needs VIEW off the molecular `Composite`
  tree first (H4/H7) — so VIEW can build against handle-libBALL.

### pyBALL v2 (binding rewrite) — cross-cuts v2.x
- Phase 6 bake-off (autowrap+Cython vs nanobind vs SIP6) on a hard-pattern
  BALL slice → tool decision; then 999.15 bulk wrap. Detailed in
  `PYBALLV2.md`. **Gap:** currently parked on the BALLView track but
  depends on the libBALL handle surface — should be re-anchored to **after
  H4** (handle API final) and ideally the repo split (v2.4), since the
  bindings wrap the handle types, not the v0 `Composite` API.

---

## 5. Cross-cutting dependency chain (the critical path)

```
v2.2: H2 (mirror) → H3 (consumers) → H4 (FLIP: delete Composite tree, sizeof drops)
                                        │
                 ┌──────────────────────┼───────────────────────────┐
                 ▼                      ▼                            ▼
            H5 Bond unify        H6 property break + JSON v2     H7 VIEW-RTTI + MSVC CI
                 └──────────────┬───────┘                            │
                                ▼                                    │
                          H8 D13 verify + v2.2.0 ◄───────────────────┘
                                │
                                ▼
              v2.3 String→std::string  ──►  v2.4 repo split  ◄── pyBALL v2 (after H4, ideally v2.4)
```

D13 (the original memory goal) is **MET at H4 and CONFIRMED at H8** — this
is the milestone's headline success gate.

---

## 6. Gaps / open questions for the review (RR1)

1. **D13 closure is implicit, not a phase.** The roadmap must name H4
   (flip removes inheritance → sizeof drops) + H8 (assert) as the explicit
   D13 success gate, and state the numeric target (≤32 B handle? ≤160 B
   total?) — D40 dropped the ≤32 B v2.1 target; re-affirm for v2.2.
2. **Version strategy for the breaks.** v2.2 carries the handle break; v2.3
   carries `String`. Are these one break wave (v2.2.0) or staged minor
   versions? A single loud break is easier for downstreams than two.
3. **pyBALL anchoring.** PyBALL v2 currently sits on the BALLView track but
   semantically belongs after H4 (it wraps the handle API). Re-anchor.
4. **VIEW timing.** VIEW must move off the `Composite` tree (H7) before the
   repo split (v2.4) can give libVIEW an independent build against
   handle-libBALL. Confirm H7 ⟶ v2.4 ordering.
5. **MSVC CI is needed twice:** for H8 EBO/sizeof verification AND for the
   repo split's Windows leg. Single H7 deliverable, consumed by both.
6. **Is "stay v2.2" still right** given the flip + String + split are all
   breaking? Or should the flip be v3.0 with 2.x as the pre-flip band?
   (D53 said stay; revisit now that the full post-flip scope is visible.)
7. **Track-B test surface gap** (Expression/Selector) — is it a real
   regression risk for H3c, or already covered?

---

## 7. Next action

Codex **RR1** adversarial review of this unified roadmap: integration
coherence, the critical-path ordering, backlog placement, the 7 gaps, and
the version strategy. Iterate to agreement, then lock as the canonical 2.x
roadmap.
