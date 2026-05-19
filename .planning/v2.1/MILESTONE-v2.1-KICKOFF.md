# BALL 2.1 — Milestone Kickoff

**Status:** PLANNING (P0).
**Authored:** 2026-05-19 (immediately post-v2.0.0 tag).
**Predecessor:** v2.0.0 (commit `7a8dc7caf`, tagged 2026-05-19).
**Branch:** `v2.1` (forked from `v2.0` at v2.0.0 tag).

This document is the kickoff brief for the v2.1 milestone — what's
in scope, what's out, sequencing, and the design questions that
need to land before phase work starts.

---

## Why v2.1 exists

v2.0 shipped the KERNEL substrate (SoA `MoleculeStore` + handle
layer) and re-enabled the full module surface, but two of v2.0's
own design targets landed as **partials** that v2.1 closes:

1. **D13 memory budget — full-handle workloads are 3.1× over.**
   `sizeof(Atom) = 360 B`, `sizeof(Bond) = 288 B`, dominated by the
   D2/D3/D4 thin-stub Composite + PropertyManager + Selectable bases
   that v2.0 deliberately kept as forwarders rather than deleting.
   Interactive / GUI / v1.x-compat code paths miss the ≤160 B/atom
   budget because every visible atom needs an `Atom*`.

2. **JSON load is 15× slower than save** (3.8 s for 100k atoms vs
   233 ms save). Per-atom heap-alloc + adoption + Composite-tree
   insertion is the bottleneck.

Plus several smaller v2.0 deferrals (Bond PropertyManager JSON,
custom Element round-trip, CI-calibrated perf gates).

---

## Scope (in)

| ID | Item | Source | Sized |
|---|---|---|---|
| **V21-ATOM-THIN-HANDLE** | `sizeof(Atom) 360 B → ~24 B` by deleting D2/D3/D4 base-class inheritance | R12 K1, R9 #1 | L (deep, touches every consumer of Atom-as-Composite) |
| **V21-BOND-THIN-HANDLE** | `sizeof(Bond) 288 B → ~16 B` (same surgery as ATOM) | R12 K2 | L |
| **V21-COMPOSITE-SIDE-TABLE** | Move `Composite` parent/child links off Atom into a per-store side table | R12 D2 | L (prerequisite for ATOM-THIN-HANDLE) |
| **V21-SPARSE-PROPERTY-MAP** | Move `PropertyManager` bag off Atom into sparse `HashMap<atom_idx, PropertyMap>` | R12 D3 | M |
| **V21-SELECTABLE-SIDE-BITMAP** | Move `Selectable::selected_` bit off Atom into a per-store bitmap | R12 D4 | S |
| **V21-STORE-ITER-API** | Public store-native iterator (skip Atom* materialisation entirely for hot loops) | R12 D5 | M |
| **V21-HANDLE-GENERATION-GUARD** | Compile-time + runtime stale-handle detection (RAII `BorrowedAtom` wrapper) | D8, R9 | S |
| **V21-BOND-PROPERTY-JSON** | Serialise Bond handle's PropertyManager bag in K0.6 JSON (MMFF94SBMB, MMFF94RBL, VIRTUAL__BOND, HBond annotations) | R16 C-B7 | S |
| **V21-LOAD-BATCH** | Close JSON save/load 15× gap via batched insert + deferred adopt | R7+R8+R9 | M |
| **V21-ELEMENT-INSTANCE-ID** | Custom-Element round-trip via instance-id table (current K0.6 path loses pointer identity) | R8 | S |
| **V21-CI-PERF-GATES** | Calibrate gates to ~2× regression detection (today's gates are 100×-loose tripwires) | R9 #4 | S |
| **V21-MEDIAN-OF-N-BENCH** | Reproducible release-claim perf numbers (median-of-5 runs, configurable) | R9 #6 | S |

---

## Out of scope (explicit)

- **Track B Wave 4-7 module work** — the remaining trim-list closures (full DOCKING test surface, PyBALL, BALLView refresh) ride in v2.0.x patches or run in parallel. v2.1 is purely the kernel thin-handle refactor + perf gap.
- **Renderer / VIEW changes** — Phase 4b (Qt 6) and Phase 5 (backend swap) are BALLView 1.6 milestones, orthogonal to v2.x kernel.
- **New file formats / new force-fields / new science** — v2.1 is infrastructure-only.
- **K0.6 binary fast-path** — JSON is the format; the binary path remains a "if profiling shows we need it" item, not v2.1 scope.

---

## Phasing

| Phase | Name | Gate |
|---|---|---|
| **v2.1-P0** | Design lock | All design questions below answered; ADR-style write-up per decision; Codex R17 reviews the design before P1 starts. |
| **v2.1-P1** | Side-table infra | COMPOSITE-SIDE-TABLE + SELECTABLE-SIDE-BITMAP + SPARSE-PROPERTY-MAP land as parallel side tables. Atom *still* inherits D2/D3/D4 — no surgery yet. All existing tests pass; perf parity. |
| **v2.1-P2** | Thin-handle flip | Delete D2/D3/D4 inheritance from Atom. `sizeof(Atom)` drops to target. Every consumer of Atom-as-Composite redirected through side tables. Codex R18 reviews. |
| **v2.1-P3** | Bond thin-handle | Same surgery on Bond. `sizeof(Bond)` drops to target. |
| **v2.1-P4** | JSON closures | V21-LOAD-BATCH + V21-BOND-PROPERTY-JSON + V21-ELEMENT-INSTANCE-ID. K0.6 schema MINOR bump. |
| **v2.1-P5** | Perf & benchmarks | V21-CI-PERF-GATES + V21-MEDIAN-OF-N-BENCH + V21-STORE-ITER-API + V21-HANDLE-GENERATION-GUARD. Final D13 verification. |
| **v2.1-P6** | Release notes + tag | RELEASE-NOTES-v2.1.md, MILESTONE-CONTEXT-v2.1.md, v2.1.0-rc1 through v2.1.0 final. |

P1 is the highest-risk phase — it's the wedge between the v2.0 world (Atom-as-Composite) and the v2.1 world (Atom-as-handle). The side tables must be correct AND functionally equivalent before P2's flip, or the cascade of consumer breakage will be unmanageable.

---

## Design questions to lock in P0

These are the "load-bearing" decisions; everything downstream depends on them.

### Q1. Composite side-table — how is the tree stored?

Options:
- **A.** Per-store `vector<CompositeNode>` indexed by atom_idx. Cheap, but Atom isn't the only Composite — Molecule, Chain, Residue, System all are. Need a unified node table or per-class tables.
- **B.** Heap-allocated side `Composite*` per Atom, owned by store. Halves the gain — back to ~96 B/atom for the side pointer + alloc overhead.
- **C.** Don't store the Composite tree at all for Atom — synthesize on demand from store's molecule_id_ + residue_id_ columns. Works only if the tree is read-only-derived from store columns; breaks Atom::insert(*child) patterns.

**Recommendation to discuss:** A with a unified node table; Atom's "composite-ness" becomes a (idx, kind) tuple looked up in `MoleculeStore::composite_nodes_`.

### Q2. PropertyManager side-map — dense or sparse?

`PropertyManager` is rarely populated on Atom (most atoms have 0 properties). Options:
- **A.** Dense `vector<PropertyBag>` per store, sized to store capacity. Wastes memory if usage is sparse.
- **B.** Sparse `HashMap<atom_idx, PropertyBag>`. Lookup is O(1) avg but slower than [].
- **C.** Hybrid: small fixed-size inline (3-4 NamedProperty values) + spill to heap. Matches v1.x lazy heap-alloc behaviour.

**Recommendation:** B; benchmark vs A in P1.

### Q3. Selectable bitmap — atomic or single-threaded contract?

`Selectable::selected_` is read in tight loops (rendering, selection queries) and written from GUI events. Options:
- **A.** Single-thread per-System contract (matches D16); plain `vector<bool>` or `vector<uint8_t>`.
- **B.** Atomic per-bit access. Slows hot reads.

**Recommendation:** A, consistent with D16.

### Q4. Generation-guard surface — opt-in or always-on?

Today's `BorrowedColumnRef` is opt-in. For v2.1 we have a choice:
- **A.** Always-on at handle dereference. Every Atom getter/setter does a generation check. Cost: one compare per access.
- **B.** Debug-only (`BALL_DEBUG`). Free in release.
- **C.** Opt-in via `BorrowedAtom` RAII handle; raw `Atom*` is "you said you knew what you were doing".

**Recommendation:** C, with a debug-mode trap on `Atom*` use after store invalidation.

### Q5. v2.0 → v2.1 API break — yes, no, or how much?

Atom-as-Composite is in the public API. Deleting D2/D3/D4 inheritance breaks:
- `Atom*` ⇒ `Composite*` upcast (used by traversal code)
- `dynamic_cast<Atom*>(Composite*)` (used by visitor pattern)
- `atom->getParent()`, `atom->getChild(n)`, `atom->countDescendants()`
- `atom->setProperty(...)` etc. (PropertyManager API)
- `atom->select()`, `atom->isSelected()` (Selectable API)

The Track B clusters used these APIs extensively. We can either:
- **A.** Keep the surface, redirect to side tables (zero source break, some perf cost from indirection)
- **B.** Break the surface, force consumers to call new `store->getParentOf(atom_idx)` style APIs
- **C.** Hybrid: keep the surface with a clear deprecation in v2.1, plan removal for v2.2

**Recommendation:** A for v2.1, deprecation warnings for v2.2. The whole point of D2/D3/D4 thin-stubs was to preserve the API surface; v2.1 keeps that promise.

---

## Success criteria

- `sizeof(Atom) ≤ 32 B`, `sizeof(Bond) ≤ 24 B` (pinned via `Sizeof_test`).
- D13 fully met: full-handle workload **≤160 B/atom**.
- JSON load 100k atoms: **≤500 ms** (8× improvement over v2.0's 3.8 s; goes from 16× slower than save to 2× slower).
- Bond PropertyManager round-trips through K0.6 JSON.
- All 282 v2.0 CORE_ONLY tests still PASS (zero regression).
- Selector speedup still ≥10× median (the K0.5 gate must hold).
- Codex adversarial review at every phase boundary; zero [BUG]s shipped in v2.1.0.

---

## Open questions (defer to user)

1. **Should v2.1 land before v2.0.x patches close Track B Wave 4-7?** The v2.0 trim list (`MOL2File`, `DCDFile`, rotamer libraries, etc.) is independent of the thin-handle refactor. They could ship as v2.0.1 / v2.0.2 / v2.0.3 in parallel.
2. **Adversarial review cadence — Codex R17/R18 only, or sub-round per phase?** R10-R16 were heavy; v2.1 has fewer surface boundaries, so 2-3 rounds is probably right.
3. **Branch strategy — `v2.1` continues from `v2.0`, or rebase onto `master` first?** Master is still v1.x — no rebase opportunity. Stay on `v2.1` linear from v2.0.0 tag.
4. **Compatibility goal — bytes-identical JSON output between v2.0 and v2.1?** If yes, V21-LOAD-BATCH must preserve the column ordering. If no, schema MINOR bump.

---

*Authored 2026-05-19, immediately post v2.0.0 tag.
Next action: P0 design lock — answer Q1-Q5 and the four open questions above, then Codex R17 design review.*
