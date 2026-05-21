# BALL 2.x — Unified Roadmap (kernel modernization track)

**Status:** LOCKED — canonical BALL 2.x kernel roadmap (Codex RR1
NEEDS-REVISION → revised → RR1b AGREE-LOCKABLE, 2026-05-21).
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

> **Pending design gate `H1b′` (hierarchy collapse, HC1→HC1b):** the
> maintainer directed collapsing the molecular class hierarchy to four node
> kinds — **System / Molecule / Fragment / Atom** (+ Bond) — with
> Protein/NucleicAcid/Chain/Residue/Nucleotide/SecondaryStructure/PDBAtom/
> AtomContainer captured as **roles + properties**, not C++ subclasses.
> Codex HC1 = collapse-YES conditional on a short design-lock gate (role
> taxonomy + canonical PDB depth + SS-as-span + PDB round-trip test) that
> **closes BEFORE H3**; else stage to v2.3. See
> `V2X-HIERARCHY-COLLAPSE.md`. If the gate passes: `ContainerKind`
> shrinks + the 8 typed handles (H1b) collapse to `Molecule`/`Fragment`
> role-aware handles; H1a table + H2 mirror survive (kind-agnostic); H3
> migrates consumers to the collapsed API once (avoids a double migration);
> H8 adds a **PDB/structural round-trip fidelity gate** alongside D13.

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
| **H5** Bond unify | ⏳ | `Bond` fully a handle; remove `Atom::bond_[]`; bond properties → store columns; bond iterators redesigned (D51). Atom `sizeof` closure includes removing `number_of_bonds_`/`bond_[]` (R29 P29-6). |
| **H6a** property break + Atom/Bond guard + container/property JSON | ⏳ | Property mutable-reference break (`NamedProperty&`/`BitVector&` → visitor/value accessors, D49); Atom/Bond + Python-wrapper slot-generation stale-handle guard (D54/D65; containers already done H1b). JSON `containers`/topology/property sections. **Runs after H4.** |
| **H6b** final JSON schema v2 + converter | ⏳ | Freeze the JSON schema **MAJOR bump to 2** (D62) — **depends on H5** (the final bond representation must be known before the bond JSON is frozen) + validation (no cycles/orphans) + a **v2.1-JSON → v2.2-JSON-v2 topology converter** (NOT the already-shipped K0.6.4 v1→v2). |
| **H7** VIEW + MSVC | ⏳ | The **54 VIEW/APPLICATIONS Atom-RTTI sites** (V21-VIEW-RTTI/D37) + `compositeAsAtom_` removal in VIEW (V21-COMPOSITEASATOM-REMOVAL) → handle kind tag; the **VIEW redraw/stamp contract** (D59 per-container `selection_stamp` decision — feeds the store/JSON schema, so resolve BEFORE H6b freeze); **MSVC + vcpkg CI** (V21-MSVC-CI) for EBO/`sizeof` verification — **bring-up may start in parallel after H2** once the build shape is stable; the H7 gate is its acceptance. |
| **H8** D13 verify + release | ⏳ | **D13 acceptance (the headline gate):** (1) at H4 `Atom`/`Bond` are non-polymorphic value handles with no `Composite`/`PropertyManager`/`Selectable` base and no inline tree/property/bond state; (2) CI asserts `sizeof(BALL::Atom) ≤ 32 B` and `sizeof(BALL::Bond) ≤ 32 B` **on every supported compiler incl. MSVC**; (3) a 100k-atom memory-accounting test asserts total live store footprint ≤ 160 B/atom (mandatory columns + parent/container indices + selection bits + bond-CSR baseline + side-table overhead; excluding JSON text + optional user properties); (4) release notes state handle size AND per-atom budget **separately** (neither is a proxy for the other). Pinned-baseline perf gates (V21-CI-PERF-GATES); v2.2.0-rc1 → v2.2.0 + downstream migration guide.

**v2.2 standing invariants (every phase):** (a) SIP/Python stays OFF and CI proves it is not built (D52.3-7/D54) — wrappers return only with pyBALL v2 after the handle API stabilises; (b) the D31b/D66a encapsulation gate (`_moleculeStoreInternal.h` + `composite_nodes_`/`CompositeNode*` out of public/iterator headers; `containerHandle.h` off the internal header) is enforced by a **maintained script** with comment/prose false-positive handling (not ad-hoc grep) and is part of H2–H4/H8 acceptance; (c) every landed break is enumerated in `V22-API-BREAK-LEDGER.md` with a migration note + Codex sign-off (D50/D61); (d) a **mutation/import perf budget** (PDB load / JSON / build benchmarks) holds through the H2–H3 dual-existence window so the forward-only mirror never makes import paths unusable. |

**H2b carry-overs surfaced this session (place in H2b/H2d):** `insertParent`
+ free-standing `replace` need a *materialise-the-new-member* path (like
`adoptSubtree`), distinct from re-derive; the container-property migration
in `materialiseContainer_` (R32 carry-over).

---

## 3. v2.1 backlog → placement

| Backlog item | Placed in |
|---|---|
| V21-MSVC-CI-PORTABILITY | **H7** gate; bring-up may start in parallel after H2; reused by v2.4 |
| V21-VIEW-RTTI (54 sites) | **H7** |
| V21-COMPOSITEASATOM-REMOVAL (P3 R-P3.A) | **H4/H7** — the helper's premise dissolves at the flip; VIEW sites at H7 |
| V21-MUTATION-WIRING (D39) | **H2** (now active — H2a/H2b) |
| V21-STORE-ITER-API (public `iterAtoms/iterBonds`) | **H2c** (handle iterators) / **H3** |
| V21-GENERATION-GUARD | **H6a** — Atom/Bond + Python-wrapper guard completion (containers already done H1b/D65) |
| V21-BIT-PROPERTY-COLUMN (packed-bool cols) | **H6a** *if* profiling shows the sparse path is hot; else backlog |
| V21-CI-PERF-GATES (pinned-baseline comparator) | **H8** |
| Track B FORMAT/STRUCTURE `Expression_test`/`Selector_test` surface | **H3a** (Selector is KERNEL-facing) + **H3c** (FORMAT supplies the corpus) |
| ~~JSON load batching~~ | **DONE in v2.1** (load 7–14× faster, save/load near parity) — drop. Re-open only if an H2/H3 mirror perf regression appears. |
| ~~AndNode tmp-bitmap reuse~~ | **DONE in v2.0** (delivered) — drop. |
| ~~K0.6.4 v1→v2 JSON converter~~ | **DONE in v2.0** (Track B B0.2) — drop. (H6b adds a *new* v2.1→v2.2 topology converter.) |
| ~~Element-instance-id-table~~ | **Declined** (P4.3: the store's atomic-number-only model is intentional; not a JSON bug). Re-open only as a conscious kernel element-registry decision. |

---

## 4. Post-v2.2 milestones (the new directives + pyBALL)

Default sequencing is AFTER v2.2 because each consumes the *stabilised*
libBALL handle surface; doing them earlier would churn against the
in-flight flip. The one exception is `BALL::String` removal, which **may**
fold into the v2.2 break wave **iff** it lands before v2.2-rc1 with no
schedule risk to H4/H6/H8 (see §4a); otherwise it is v2.3.

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
Two work packages, in order:
1. **v2.4a — installed-package contract (in the monorepo first).** Before
   any physical split: exported CMake targets + `find_package(BALL)`
   config package, **install-tree** tests (build a consumer against the
   *installed* libBALL, not the source tree), an ABI/version policy +
   compatibility matrix, and release-artifact flow. This is real work, not
   a file move.
2. **v2.4b — physical split** into independent repos + CI + versioning:
   **libBALL** (kernel, no Qt/Python), **libVIEW + BALLView** (consumes
   installed libBALL), **pyBALL** (consumes libBALL/libVIEW). No in-tree
   source coupling.
- **Hard dependencies:** v2.2 (handle API stable) + VIEW off the molecular
  `Composite` tree (H7) + the MSVC/Linux/macOS CI legs (H7) reused per-repo;
  ideally v2.3 (no `BALL::String` to marshal across the boundary).

### pyBALL v2 (binding rewrite) — REBASELINE required, anchor after H4
- **Not just "moved" — must be REVISED.** `PYBALLV2.md`'s Phase 6 bake-off
  slice is **Composite-era** (`Atom (Composite+iterators+ownership)`,
  `HBondProcessor Composite/apply`); those cases change shape under the
  handle API. The bake-off slice must be re-cut against the **handle**
  surface (value `Atom`, `apply` over `ChildRef`, container value handles)
  before tool selection.
- SIP stays OFF through v2.2 (CI proves it). Wrappers return only after the
  handle API stabilises (≥ H4, preferably v2.2.0), compiled with
  `BALL_PYTHON_WRAPPER` so handle-validity checks are always on in scripting
  (D54). Best aligned with the v2.4 packaging (it consumes installed
  libBALL).

## 4a. Concrete version plan (RR1-5)

Staged minors by default — one clearly-documented break per release beats
an unbounded "everything modern" tag (D53: stay in the 2.x band; do **not**
rename the flip to v3.0 — these are "semver-major-sized breaks inside the
2.x modernization band").

| Release | Contents |
|---|---|
| **v2.2.0** | the handle hierarchy flip (H2→H8), JSON schema v2, D13 verified, API-break ledger closed + migration guide. |
| **v2.3.0** | `BALL::String` → `std::string` (+ `StringUtils`). Fold into v2.2 ONLY if it lands before v2.2-rc1 with no schedule risk to H4/H6/H8; otherwise its own ledger + migration guide. |
| **v2.4.0** | installed libBALL package contract (v2.4a) + 3-way repo split (v2.4b). |
| **pyBALL v2** | after H4 (min); preferably after v2.2.0, aligned with v2.4 packaging. Rebaselined bake-off → bulk wrap. |

---

## 5. Cross-cutting dependency chain (the critical path)

```
v2.2: H2 (mirror) → H3 (consumers) → H4 (FLIP: delete Composite tree, sizeof drops)
   │  (MSVC CI bring-up may start here, in parallel; H7 is its gate)
   │                                    │
   │              ┌─────────────────────┼───────────────┬───────────────┐
   │              ▼                     ▼               ▼               │
   │         H5 Bond unify        H6a property break   H7 VIEW-RTTI +    │
   │              │               + container/prop      VIEW stamp/      │
   │              │                 JSON                selection (D59)  │
   │              ▼                     │               + MSVC CI gate   │
   │         H6b final JSON v2 ◄────────┘◄──────────────┘ (stamp decision
   │         (needs final bond rep         feeds the schema BEFORE freeze)
   │          from H5 + VIEW stamp)
   │              │
   └──────────────┴────────────► H8 D13 verify (sizeof≤32B incl. MSVC; ≤160B/atom)
                                     + perf gates + migration guide → v2.2.0
                                     │
                                     ▼
   v2.3.0 String→std::string  ──►  v2.4a installed-package contract ──► v2.4b repo split
                                     ▲                                       ▲
                                     └──────── pyBALL v2 (rebaselined; after H4, ideally here) ┘
```
Key edges (RR1-2): **H6b (final JSON schema freeze) depends on H5** (bond
representation) **and the H7 VIEW stamp/selection decision** (D59) — both
must land before the schema is frozen. H6a (property break + container/
property JSON, no bond-final) can proceed right after H4. v2.4b (physical
split) depends on v2.4a (the installed-package contract), not just on H7.

D13 (the original memory goal) is **MET at H4 and CONFIRMED at H8** — this
is the milestone's headline success gate.

---

## 6. Gaps — dispositions (post-RR1)

Resolved into the plan above by the RR1 revision:
- **D13 closure** → explicit H8 acceptance (sizeof ≤32 B Atom/Bond incl.
  MSVC + ≤160 B/atom total store footprint, stated separately) (§2 H8).
- **Version strategy** → staged minors v2.2/v2.3/v2.4; stay in the 2.x band
  (D53), not v3.0 (§4a).
- **pyBALL anchoring** → REBASELINE the Phase-6 slice against the handle
  surface; anchor ≥ H4, align with v2.4 (§4).
- **VIEW timing / MSVC twice** → H7 (VIEW off Composite + stamp contract +
  MSVC) before v2.4; MSVC bring-up may start after H2; reused per-repo.
- **SIP/Python-off invariant** → v2.2 standing invariant (a) (§2).
- **D31b/D66a gate upkeep + the pre-existing prose-comment false-positive**
  → maintained-script invariant (b) (§2); supersedes the spawned
  ad-hoc-grep-fix task.
- **H4/H6 migration guide + break-ledger close** → invariant (c) + H8 (§2).
- **H2–H3 mirror perf budget** → invariant (d) (§2).
- **v2.4 installed-package work** → explicit v2.4a workstream (§4).
- **H3 processor-by-value shim lifetime** → the dual `Composite&`/handle API
  is deleted at H4; H4 requires zero consumers needing the v0 object form
  (`V22-ARCH-HANDLE-MODEL.md` open-sub-questions) (§2 H3/H4).
- **Track-B Expression/Selector surface** → H3a (Selector is KERNEL-facing)
  + H3c (FORMAT corpus) (§3).

Remaining notes (not blockers):
- **Stale source-doc label:** `V22-ARCH-HANDLE-MODEL.md` still titles itself
  "BALL v3.0 (on the v2.2 branch)"; **D53 overrides** — the canonical
  milestone is **v2.2**. Historical text left as-is; this roadmap is
  authoritative on versioning. Do not reopen the v3.0 debate.
- **"2.0" wording in the maintainer's `String` directive** = the 2.x line
  (2.0 is shipped); scheduled as v2.3 (§4) — confirm at scheduling.

---

## 7. Status — LOCKED

RR1 = NEEDS-REVISION → all 10 required changes applied → **RR1b =
AGREE-LOCKABLE**. This is the canonical BALL 2.x kernel roadmap. Reviews:
`V2X-ROADMAP-RR1.md`, `V2X-ROADMAP-RR1B.md`.
