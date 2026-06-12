# V22-PERF-REVIEW-FINDINGS — kernel speedup adversarial review

**Status:** Findings + backlog. Dual-reviewer (Codex + Vibe) adversarial
performance review at HEAD `64759036c`, 2026-06-12.
**Outcome:** one win attempted + reverted (unsafe under repeated apply);
the genuinely-actionable wins are H4-gated or belong in the ABO
STRUCTURE migration. No layout change landed — the green tree + working
bench are preserved.

## Measured baseline (Apple Silicon, release, median of 3)

| Bench | Current | Pre-2.0 (v1.x) |
|---|---|---|
| CompositeCreation | 0.53 s | 0.59 s (−10%) |
| CompositeClone | 0.30 s | 0.32 s (−6%) |
| KernelCreation | ~0.08–0.11 s | 0.080 s |
| KernelClone | 0.37 s | — |
| KernelIteration | 0.064 s | — |
| AssignBondOrder | AStar 6.57 s / FPT 5.89 s (total 12.14 s) | n/a (mid-v2.2) |

`sizeof(Atom) = 376 B` (Composite base 168 B, `bond_[12]` 96 B,
store-binding 20 B); `Vector3 = 12 B` (float); `String = 32 B`.

## Key diagnosis (where the time actually is)

The two reviewers converged on a non-obvious conclusion: **the store
column layout is NOT the bottleneck.** The benches that store-layout
tweaks would touch (KernelCreation 0.08 s, KernelIteration 0.064 s) are
already at/under pre-2.0. The heavy bench (ABO, 12 s) is dominated by
the **ABO algorithm** — pointer-keyed `HashMap` lookups and per-atom
`Expression` reconstruction — not by cache behavior of the SoA columns.
And the single biggest durable win is **the H4 flip itself**: deleting
the 376 B `Atom` facade (Composite 168 B + bond_[12] 96 B + 3 base
classes) in favour of 24 B value handles + store rows removes the
per-atom heap object entirely.

## Findings, ranked

### Actionable — but scheduled into existing migration work

| ID | Win | Where | Status |
|----|-----|-------|--------|
| **A1** | ABO penalty `HashMap<Bond*,...>` / `std::map<Bond*,short>` → integer-indexed arrays keyed by store/bond index | `partialBondOrderAssignment.C:231/525/663`, `assignBondOrderProcessor.h:676` | **15–40% on AStar** (Codex). Medium algorithm risk. **Fold into the ABO STRUCTURE migration (7b.9–7b.16)** — the post-flip version keys directly by store index, which also fixes the A2 hazard below. |
| **A2** | Precompile ABO block penalty `Expression`s once instead of `Expression exp(rule)` per `(atom × rule)` | `assignBondOrderProcessor.C:898/978` | **ATTEMPTED + REVERTED.** Functionally correct under `AssignBondOrderProcessor_test1/2` (via ctest) but **SIGSEGV in `AssignBondOrder_bench`** under the repeated-apply-across-5-fixtures pattern. Root cause: a long-lived reused `Expression` object routes `operator()` through the global `CompiledExpressionCache` keyed by `(store, expression)` with a generation-staleness contract (R13.2/R14.1); reusing one Expression across created/destroyed stores collides with that contract. NOT a safe bounded change — must be done as part of A1's by-store-index rekey, where the Expression object is no longer reused across stores. |

### Durable, but no measurable win pre-H4 (do post-flip)

| ID | Win | Note |
|----|-----|------|
| **A3** | Zero-copy CSR bond range/span on `MoleculeStore::bonds_of()` (today allocates a `std::vector<uint32_t>` per call) | 2–10× for handle-native bond iteration; **negligible for the v0 ABO path today** — compounds only after the handle migration. Land with the bond-iteration handle work. |
| **A4** | Handle-native construction/clone — skip the per-atom heap `Atom` | 20–40% on KernelCreation/Clone **after H4** (the flip removes the 376 B object). Do NOT build a v0 Atom arena/pool now — it is deleted at commit 8. |
| **A5** | Drop the redundant live `String` name columns (`name_strings_`/`type_name_strings_`, 64 B/atom) — `name_offsets_`/`string_pool_` already exist | ~45% of the raw store row, mostly memory/cache. Blocked on v0 `getName()` by-value semantics; do post-flip when the handle API returns string-view/id. |
| **A6** | Lazily materialize cold `velocities_`/`forces_` columns (24 B/atom, default-written even for non-MD) | 3–8% on construction/clone memory bandwidth. Medium risk: `getVelocity()` returns `Vector3&` into the column, so lazy materialization must keep the ref API working (same obstacle as SoA). |

### Rejected (both reviewers or verified)

| ID | Proposal | Why rejected |
|----|----------|--------------|
| **R1** | Split `positions_` into SoA `x/y/z` float arrays | **Vibe proposed, Codex + verification reject.** `getPosition()` / `MoleculeStore::position(i)` return `Vector3&` INTO the column (atom.h:476, atomHandle.h:132, moleculeStore.h:150) and `setPosition` mutates through it — a split layout cannot return `Vector3&`, breaking the reference API across the whole codebase. ABO also accesses x/y/z together per bonded pair (AoS-favourable); Codex flags a possible regression. |
| **R2** | Bit-pack `is_freed_` (uint8 → bit) | Not a hot-loop column (alloc/release metadata only); saves <1 B/atom and adds mask/extract. |
| **R3** | Composite modification-timestamp batching | Composite deleted at commit 8; API/Observer risk. |
| **R4** | Lazy orphan-store binding in `Atom::Atom()` | v0 Atom deleted at commit 8; the flip removes the cost entirely (A4). |
| **R5** | Generic "trusted handle" fast-path (skip generation check) | `AtomHandle` scalar getters already don't call `isValid()` per read; the only heavy checks fold into A3. |

## Net

The architecture is already in good shape for performance: creation/clone
beat pre-2.0, and the bridge/handle indirection is off the ABO hot path
(re-confirmed). The real remaining wins are (a) the H4 flip itself
(removes the 376 B facade) and (b) the ABO algorithm's pointer-keyed
maps — both already on the roadmap. The layout tweaks a naive review
reaches for (SoA, bitset, lazy cold columns) either break the
load-bearing `Vector3&` reference API or have no measurable effect on
the benches that matter. No speculative layout change was landed.

**Scheduled:** A1+A2 → ABO STRUCTURE migration (commits 7b.9–7b.16);
A3/A4/A5/A6 → post-H4 (after the facade deletion at commit 8).
