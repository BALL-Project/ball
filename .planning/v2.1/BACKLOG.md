# v2.1 Backlog

Items tracked for v2.1 milestone but **not blocking** v2.1.0 release. Each
gets a tracking ID, current status, and target milestone.

## Active backlog (closed by v2.1.0 if possible; otherwise documented as
v2.1.x or v2.2 follow-up)

### V21-MSVC-CI-PORTABILITY

**Status:** Open. Non-blocking v2.1.0 per D34d.
**Source:** R21 P21-11 / R21-F3, formalised in R21b P21b-7.

Add a Windows MSVC + vcpkg GHA job to `.github/workflows/ci-v2.yml`
that runs the CORE_ONLY build + ctest sweep. Currently the codebase
has never been built on Windows; vcpkg setup is part of Phase 4
(BALLView 1.6 modernization), not v2.1's kernel work.

D34d demoted this from v2.1.0-rc1 prep (D34c original) to v2.2 P0/P2
because D40 dropped the `sizeof(Atom) ≤ 32 B` target from v2.1.
Without that target, MSVC EBO verification is no longer a release
gate.

**v2.1 status:** non-blocking. Apple Clang macOS arm64 remains the
sole enforced CI target.
**v2.2 status:** required before v2.2's inheritance flip claims
`sizeof(Atom) ≤ 32 B` (BALL_EMPTY_BASES needs MSVC verification then).

Scope when implemented:
1. vcpkg manifest for Qt5 + Boost + Eigen3 + flex + bison + cmake
2. CMakeLists adjustments for MSVC link flags (`/EHsc`,
   `_USE_MATH_DEFINES`)
3. Resolution of any MSVC source incompatibilities
4. CI matrix expansion to include `windows-2022`

---

## Documentation-only backlog items

(These exist as design notes / commitments that don't affect code
state, but tracked here for visibility.)

- **V21-VIEW-RTTI** (D37): 54 Atom-RTTI sites in VIEW + APPLICATIONS.
  v2.1 only removes the 24 sites in CORE_ONLY modules. VIEW removal
  is part of Phase 4 (BALLView 1.6 modernization) prep.
- **V21-MUTATION-WIRING** (D39): The `Composite` / `PropertyManager`
  / `Selectable` mutation-path wiring that fills side tables in
  parallel with v0 inline state is **deferred to v2.2** alongside
  the inheritance flip. v2.1 ships side-table scaffolding only.
- **V21-BIT-PROPERTY-COLUMN** (R20c-2): bit-property mirrors live in
  the sparse_bag with a typed `SparseKey { kind, name_id/bit_idx }`.
  Packed-bool columns are a v2.2 candidate if profiling shows the
  sparse path is a bottleneck.
- **V21-BOND-PROPERTY-JSON** (D42 → D42-REV): **CLOSED in v2.1
  P4.2** (commit 45df3714d; R25-hardened 67827ad7c). Initially
  deferred per D42, then maintainer-reversed and implemented via
  option A — the System loader reconstructs the `Bond*` graph with
  `createBond` and round-trips the Bond `PropertyManager` bag.
  Closes the R16 C-B7 v2.0 known gap. (Listed for history; no
  longer outstanding.)
- **V21-BOND-GRAPH-RECONSTRUCT** (D43): CLOSED in v2.1 P4.2 — the
  System JSON loader now reconstructs the Atom-side `Bond*` graph
  via `createBond` (commit 45df3714d / R25-hardened 67827ad7c).
  `atom.countBonds()` is correct post-load. (Listed here for
  history; no longer outstanding.)
- **V21-STORE-ITER-API** (D44, was v2.1 P5.1 → now v2.2): public
  `MoleculeStore::iterAtoms()/iterBonds()`. Deferred because the
  "skip Atom* materialisation" win only exists after the v2.2
  thin-handle flip; designing the public iterator contract now
  risks v2.2 reshaping it.
- **V21-CI-PERF-GATES** (D44, was v2.1 P5.2 → now v2.2/v2.1.x):
  pinned-baseline CI comparator (`max(2×median, median+6×MAD)`,
  CoV classification). Needs new harness infra; over-built for
  single-platform CI. v2.1 keeps fixed ctest thresholds.
- **V21-GENERATION-GUARD** (D44, was v2.1 P5.4 → now v2.2): the
  `BALL_DEBUG` per-deref Atom-handle staleness check. The naive
  `store_generation_ == store_->generation()` compare false-trips
  on ordinary store growth/reserve/compact; correct detection
  needs v2.2 slot-generation semantics + handle-refresh
  machinery. Supersedes D25 for v2.1.

---

## Closed (delivered in v2.1 milestone)

(none yet — v2.1 in progress)
