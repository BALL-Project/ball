# BALL v2.2 — H3 Design: consumer migration (clustered)

**Status:** 🟡 DESIGN LOCK — revised (R2) after Codex **H3-DR** NEEDS-REVISION +
maintainer decisions (2026-05-25); re-review **H3-DR2** pending. See "## H3
revision R2" at the end (it supersedes the relevant decisions above).
**Phase:** H3 — move consumers off the v0 `Composite&`/`Atom&`/`dynamic_cast`
surface onto the handle API (H1b `ContainerHandle*` + H2c `AtomHandle` +
`StructureQuery`), in dependency clusters.
**Predecessors (all committed/green):** H1a table, H1b handles, H2a/b mutation
mirror, **H2c handle-yielding traversal** (`AtomHandle` + `StructureQuery::atoms
/apply`, commit 9a622b929), HCP-1P, HCP-2, persistence-stream removal.
**Successor:** H4 — the flip (delete the v0 `Composite` object tree; `Atom`/
`Bond`/containers *become* the handles; `sizeof(Atom)` drops).

> This is "the bulk of the work" (arch model §3): ~173 files touch atom
> traversal; `UnaryProcessor<Atom>` lives in STRUCTURE/QSAR/MOLMEC/CONCEPT;
> `dynamic_cast<Atom*>` is VIEW-concentrated. Unlike H1b–H2c (purely additive),
> H3 changes consumer code. It is executed **green-incremental**: the v0 object
> tree + v0 API remain the source of truth (D60) through H3; the v0 surface is
> deleted only at H4.

---

## The central question H3 answers

The arch model (§3) envisions `for (Atom a : c.atoms())` — `Atom` *by value*.
But during dual existence **`Atom` is still the heavy v0 `Composite`-derived
heap object** and cannot be a cheap/correct by-value handle. So the realistic
dual-existence migration target is the **`AtomHandle`** built in H2c (Option A),
not a shimmed-by-value v0 `Atom`. At H4 the v0 classes are deleted and
`AtomHandle`/`BondHandle`/`*Handle` reconcile to the canonical `Atom`/`Bond`/…
names (the per-kind D63 rename audit, NOT a one-line alias). H3 therefore
migrates consumers onto handles; H4 restores the canonical names over them.

This also forces the second decision: we **cannot** flip the global
`UnaryProcessor<Atom>::operator()(Atom&)→(Atom)` signature in one step (it would
break every processor at once → tree red until all ~30 are migrated). Instead H3
provides a **parallel handle-processor path** (the H2c `StructureQuery::apply`
already dispatches to `operator()(AtomHandle)` / `operator()(ContainerHandleBase)`)
and migrates processors cluster-by-cluster; the v0 `UnaryProcessor<Atom>` +
`Composite::apply<T>` stay until H4.

---

## Decisions

### D-H3.1 — Migration target = handles (`AtomHandle`/`BondHandle`/`*Handle`)
Consumers migrate from `Atom&`/`Atom*`/`Composite*` to the H2c/H1b handles
**by value**. `AtomHandle` is the atom target; H4 renames it to `Atom`
(D63 audit). No new "by-value v0 `Atom`" shim is introduced (impossible
pre-flip).

### D-H3.2 — `AtomHandle` grows the migration-shim surface (sub-phase H3a)
For consumer *bodies* to compile with minimal churn, `AtomHandle` replicates the
Atom consumer method **names**, forwarding to the store:
- **Scalar data** (already SoA columns since HCP-1P): `getPosition/setPosition`,
  `getCharge/setCharge`, `getElement/setElement`, `getRadius/setRadius`,
  `getForce/setForce`, `getVelocity/setVelocity`, `getType/setType`,
  `getTypeName/setTypeName`, `getFormalCharge/setFormalCharge`, `getName/setName`,
  `getFullName`, `getStore/getStoreIndex`.
- **Bonds** (D-H3.3): `countBonds`, `getBond(i)→BondHandle`, `hasBond`,
  `isBoundTo`, `getPartnerAtom/getBoundAtom→AtomHandle`, geminal/vicinal preds —
  resolved through the store bond CSR.
- **Hierarchy nav**: `getParent`, `getResidue/getChain/getMolecule/getFragment/
  getSecondaryStructure→*Handle` — container-row parent walk filtered by role
  (the `StructureQuery::*Of` pattern, already role-based ⇒ survives HCP-2d/H4).
- **Property surface** (D50/D52 controlled break): `getProperty/setProperty/
  hasProperty/clearProperty` (value semantics → property columns + sparse bag);
  the mutable-reference surface (`NamedProperty&`, `beginNamedProperty`,
  `BitVector&`) is **broken** and replaced by an `eachProperty(visitor)` +
  `propertyNames()` (ledgered). Selection: `select/deselect/isSelected`.
- **Validity**: `isValid` (stable_id, H2c); `operator bool`.
`mutable`-ref / write methods compile only in the dual-existence window where the
v0 atom still backs the slot (writes go to the shared store columns either way).

### D-H3.3 — `BondHandle` (arch §6)
A `{MoleculeStore*, bond_idx, …}` value handle over the bond CSR + bond columns
(`bond_back_ptr` is the dual-existence bridge). Yielded by `getBond(i)` and a
`bonds()` traversal. Built in H3a alongside the `AtomHandle` shim (the atom shim
needs it). Bond properties resolve to store-side bond columns.

### D-H3.4 — Processor / predicate / Expression / Selector migration
No global signature flip. Provided in H3a, consumed per cluster:
- **Handle processors:** consumers' `UnaryProcessor<Atom>` subclasses migrate to
  a handle-processor (`operator()(AtomHandle)`), invoked via
  `StructureQuery::apply` (H2c). v0 `UnaryProcessor<Atom>` + `Composite::apply<T>`
  remain until H4.
- **Predicates / `Expression` / `Selector`:** handle-taking variants
  (`UnaryPredicate<AtomHandle>` / an `Expression` evaluable against `AtomHandle`).
  The CompiledExpression store path (v2.1) already evaluates over store slots, so
  the handle predicate layer is thin.
- A consumer is "migrated" when it no longer names `Atom&`/`Composite*`/RTTI on
  atoms; un-migrated consumers keep working through the v0 path (dual existence).

### D-H3.5 — Cluster order, green-incremental, VIEW deferred
Each cluster = its own commit + Codex review; rc-gated full rebuild + full ctest
green at each. Order (dependency-driven, arch model §165):

| Sub | Cluster | Notes |
|---|---|---|
| **H3a** | **Shim + infra** | `AtomHandle` full surface + `BondHandle` + handle-processor/predicate infra + `bonds()`/nav. Additive; foundation for all clusters. |
| **H3b** | KERNEL | the few KERNEL atom-traversal consumers (extractors, selector, …). |
| **H3c** | STRUCTURE | the heaviest processor cluster (11 `UnaryProcessor<Atom>`). |
| **H3d** | MOLMEC + QSAR | force fields, descriptors. |
| **H3e** | SCORING + DOCKING | scoring components, pose handling. |
| **H3f** | FORMAT | file readers/writers over atoms. |
| **H3g** | NMR + ENERGY + SOLVATION | remaining processors. |
| **(deferred)** | **VIEW + APPLICATIONS** | out of the `BALL_CORE_ONLY` build (`BALL_HAS_VIEW=OFF`) and the bulk of the `dynamic_cast<Atom*>` sites. Migrated in the VIEW phase (Class K, with the persistence/Client VIEW work). NOT in H3's kernel scope. |

### D-H3.6 — RTTI / `dynamic_cast<Atom*>` sites
`dynamic_cast<Atom*>(composite)` / `RTTI::isKindOf<Atom>` → handle kind/`as<>()`
checks (`ContainerChildRef::is_atom`, `ContainerHandleBase::as<>()`). Kernel-side
sites migrate with their cluster; the VIEW-concentrated sites are deferred (D-H3.5).

### D-H3.7 — Dual-existence safety + per-cluster guardrail
The v0 object tree stays source of truth through H3 (D60). Migrated consumers
read handles off the **verified** container-table mirror (HierarchyParity_test is
green). Each cluster adds/extends a test asserting the migrated path yields the
same result as the v0 path it replaced (the H2c order-parity pattern generalized
to the cluster's operation). No `compact()`/free-list reordering contract changes.

---

## Out of scope / explicitly deferred
- **The flip** (delete v0 `Composite` tree; canonical-name restore; `sizeof(Atom)`
  drop) is **H4**, not H3.
- **VIEW / APPLICATIONS / Python** consumer migration — VIEW phase (Class K).
- **HCP-2d** SS-as-annotation reopens at H4, not H3.
- The general container-property bag (D-2c.4) — H4.

## Open sub-questions for the maintainer (design-lock asks)
1. **Shim scope** — full Atom surface on `AtomHandle` (D-H3.2, lower consumer
   churn, bigger handle) vs a **narrowed** surface (consumers adapt more bodies,
   leaner handle)? Recommend **full** (matches arch model "bodies compile
   unchanged").
2. **`BondHandle` timing** — H3a (recommended; the atom shim's `getBond` needs it)
   vs deferred until the first bond-touching cluster?
3. **Cluster commit granularity** — one commit per module cluster (H3b…H3g,
   recommended) vs finer (per-file) for the heavy STRUCTURE cluster?
4. **Property mutable-ref break** — land the `eachProperty` replacement in H3a
   (recommended) vs per-cluster as consumers hit it?

## Success criteria
- Every kernel cluster migrated off `Atom&`/`Composite*`/atom-RTTI onto handles;
  tree green (rc=0 build + full ctest) at each cluster commit; per-cluster Codex
  review with zero BLOCKERs shipped; API-break ledger updated per cluster.
- v0 surface still present + working (deleted only at H4); VIEW deferred.

## H3-DR design review

Verdict: **NEEDS-REVISION**. The migration target is correct, but this design
cannot be locked as written because it overstates the sufficiency of the current
handle `apply` path and under-enumerates pointer/subtype/property identity
consumers that cannot be solved by "same method names on `AtomHandle`".

Required fixes before H3a starts:
- Specify the handle processor contract as an H3a deliverable: `start()` /
  `finish()`, `Processor::Result` propagation, `BREAK` vs `ABORT` return
  semantics, root-vs-descendant traversal variants, and mutation-during-walk
  policy. Current `StructureQuery::apply` only recursively calls `proc(...)`;
  it is not equivalent to `Composite::apply<T>`.
- Add an explicit migration hazard bucket for consumers that store or expose
  `Atom*` / `Bond*` identity (`AtomList`, `BondList`, `Selector::selected_atoms_`,
  `MolecularGraph`, `MolecularInteractions`, SMARTS/ring/kekulizer/sdGenerator
  maps/sets/vectors). These must become handle/stable-id containers or remain
  temporarily bridged; forwarding methods on `AtomHandle` is not enough.
- Add a PDBAtom/subtype plan. KERNEL extractors and typed container APIs return
  `PDBAtom*` and use `dynamic_cast<const PDBAtom*>`; a generic `AtomHandle`
  needs a kind/subtype column, a `PDBAtomHandle`, or an explicit deferral/bridge.
- Expand the Atom/Bond shim inventory beyond scalar/bond/nav/property reads:
  `createBond`, `destroyBond(s)`, `applyBonds`, bond iterators/reverse iterators,
  `getDistance`, `isBound`, interactions, `dump`, `clear/destroy/swap/set/get`,
  and equality/order semantics either need handle equivalents or must be
  ledgered as intentional breaks.
- Define mirror freshness as a sequencing invariant, not just a parity test:
  every v0 mutator used by mixed clusters must update the store before any
  migrated reader can observe the object again, and known bridge reads such as
  selection/container counts that still read `back_ptr` must be documented.
- Clarify build scope. The text says VIEW/APPLICATIONS are out of
  `BALL_CORE_ONLY`, but the actual core-only switch excludes most H3 clusters;
  H3 should name the concrete CI/build matrix (`BALL_COLLAPSE_KERNEL_ONLY`,
  full `libBALL` without VIEW, full build with VIEW deferred, etc.).

Item verdicts:
1. **SOUND** — Targeting `AtomHandle` instead of a by-value v0 `Atom` is correct; the H4 rename is acceptable if D63 audits every public typedef/container and avoids exposing `AtomHandle` as a new stable API name.
2. **WEAK** — Store-backed forwarding is broadly sound for scalar/bond/nav reads, but the doc misses non-forwardable consumers: address-taking, pointer identity containers, `PDBAtom` subtype RTTI, bond/atom iterators, creation/destruction, interaction maps, and debug/lifecycle methods.
3. **FLAW** — "No global processor flip" is right, but the claimed parallel path is not sufficient yet: H2c `StructureQuery::apply` lacks `start()/finish()`, `Processor::Result` short-circuiting, return status, traversal variants, and mutation-during-iteration rules.
4. **WEAK** — The cluster order is plausible, but FORMAT likely belongs earlier or must be split because PDB/PDBAtom/container naming shape KERNEL/STRUCTURE tests; VIEW deferral is legitimate only if the full non-VIEW module build is the gate, not `BALL_CORE_ONLY`.
5. **WEAK** — `BondHandle` in H3a is correct and property visitor replacement belongs there, but RTTI conversion is only sound for atom-vs-container kind checks; `PDBAtom` and other subtype casts need an explicit handle-kind/subtype story.
6. **WEAK** — Dual existence is safe only if every v0 write path is synchronously mirrored and every migrated read is sequenced after that hook; "verified mirror" plus per-cluster parity does not catch transient stale reads or unmirrored legacy mutators.
7. **WEAK** — The four recommended answers are mostly right, but cluster granularity should allow STRUCTURE sub-commits, and the missing fifth load-bearing question is: what is the public replacement for pointer identity APIs (`Atom*`/`Bond*` lists, maps, sets, and subtype pointers) during H3 and after H4?
8. **FLAW** — New design risk: the proposal treats handle migration as mostly signature churn, but several core algorithms are graph algorithms keyed by object pointers; without a first-class stable-id/handle key policy, H3 will either leak `getAtom()` bridges everywhere or re-churn those clusters at H4.

---

## H3 revision R2 — H3-DR resolutions + maintainer decisions (2026-05-25)

Addresses every H3-DR finding. Maintainer decisions: **(1) stable-id keys**;
**(2) eliminate the `PDBAtom` subtype** (PDB fields become store-backed atom
attributes — no atom subtype, no `PDBAtomHandle`, no `dynamic_cast<PDBAtom*>`).
The items below SUPERSEDE the same-numbered decisions where they conflict.

### D-H3.8 (NEW) — Pointer/subtype identity policy = stable-id keys [resolves H3-DR FLAW item 8, WEAK items 2/7, the "5th question"]
BALL's graph algorithms key on `Atom*`/`Bond*` **identity** in `HashSet`/
`HashMap`/vectors (`Selector::selected_atoms_`, `MolecularGraph`,
`MolecularInteractions`, SMARTS matcher, ring perception, kekulizer,
`sdGenerator`, `atomBijection`, …). The durable public replacement is the store
**`StableId`** (u64, already unique-per-live-atom, ABA-safe, survives `compact()`
and the H4 flip):
- These identity containers re-key from `Atom*`/`Bond*` to `StableId` (or, where a
  full handle is needed, `AtomHandle`/`BondHandle` with `StableId`-based hashing).
  `AtomHandle`/`BondHandle` gain `std::hash` specializations + `operator<` on
  `{store, stable_id}` so they are drop-in keys.
- This is a **first-class key policy**, NOT a `getAtom()` bridge sprinkle — it is
  the explicit answer to "what replaces pointer-identity APIs during H3 and after
  H4" (the missing 5th question). `getAtom()` is allowed ONLY as a transition
  shim inside a not-yet-migrated body, never as a stored key.
- Migration-hazard bucket (per-cluster checklist): address-taking `&atom`, stored
  `Atom*`/`Bond*`, pointer comparisons, and `Atom*`-keyed containers are flagged
  and converted to `StableId`/handle keys as their cluster is migrated.

### D-H3.6 (REVISED) — `PDBAtom` subtype ELIMINATED; PDB fields = atom attributes [resolves H3-DR WEAK items 2/5, FLAW-adjacent subtype risk]
There is **no atom subtype** in the handle model. `PDBAtom`'s per-atom PDB fields
(occupancy, B-factor/temperature factor, alternate-location indicator, branch
designator, remoteness indicator, insertion-related, formal charge) become
**store-backed atom attributes** — new atom SoA columns for the hot/always-present
ones, the property bag for the rare ones — accessed through `AtomHandle`
getters/setters. Consequences:
- **No `PDBAtomHandle`, no kind/subtype column for atoms.** `dynamic_cast<(const)
  PDBAtom*>` sites collapse to direct `AtomHandle` attribute access (every atom
  can carry PDB attributes; default/empty when not from a PDB source).
- KERNEL extractors / typed APIs returning `PDBAtom*` re-express as `AtomHandle`
  (+ the PDB attribute accessors). The v0 `PDBAtom` class stays during dual
  existence (deleted at H4); its data is mirrored into the atom attributes so the
  handle path reads them.
- The PDB-attribute columns/accessors land in **H3a** (foundational); the
  `PDBAtom`-producing/consuming code (PDBFile etc.) migrates in the **FORMAT/PDB
  cluster** (see D-H3.5 R2).
- The generic `dynamic_cast<Atom*>(composite)` / `isKindOf<Atom>` (atom-vs-
  container discrimination) still maps to `ContainerChildRef::is_atom` /
  `ContainerHandleBase::as<>()` as before.

### D-H3.4 (REVISED) — Handle processor contract is a full H3a deliverable [resolves H3-DR FLAW item 3]
`StructureQuery::apply` is upgraded from a bare recursive call into a faithful
table-side `Composite::apply<T>` equivalent, built + tested in **H3a**:
- **`Processor::Result` propagation:** the handle processor returns
  `Processor::Result` (`CONTINUE`/`BREAK`/`ABORT`); `BREAK` stops the current
  subtree, `ABORT` unwinds the whole walk (return value surfaced to the caller).
- **`start()` / `finish()`** lifecycle hooks invoked once around the walk, matching
  `UnaryProcessor<T>`.
- **Traversal variants:** preorder (default) + the root-inclusion + descendant-only
  + kind-filtered (`AtomHandle`-only vs container-yielding) variants consumers use.
- **Mutation-during-walk policy:** the handle walk reads the ordered `ChildRef`
  edges; mutating topology mid-walk is **forbidden** (documented; matches the v0
  CONTINUE/BREAK contract which also forbids structural mutation mid-apply) — a
  processor that must mutate collects handles/stable-ids first, then mutates.
- Tested against the v0 `Composite::apply<T>` result + `Processor::Result` short-
  circuit parity (extends the H2c order-parity pattern).

### D-H3.2 (EXTENDED) — full shim inventory + value/identity semantics [resolves H3-DR WEAK item 2 tail]
Beyond the scalar/bond/nav/property reads already listed, `AtomHandle`/
`BondHandle` provide handle equivalents for the rest of the consumed surface, or
the gap is ledgered as an intentional break:
- `createBond`/`destroyBond(s)`/`applyBonds` → store bond-table mutators returning/
  taking `BondHandle`; `getDistance`, `isBound`, geminal/vicinal preds → store/CSR.
- bond iterators / reverse iterators → `bonds()` handle range.
- `MolecularInteractions` → keyed by `StableId` (D-H3.8).
- `dump`/`clear`/`destroy`/`swap`/`set`/`get` and `Atom`/`Bond` equality/order →
  `operator==`/`std::hash` on `{store, stable_id}` (D-H3.8); lifecycle methods
  (`destroy`/`clear`) are **not** on the read-handle — topology mutation goes
  through the container/store mutators, and any v0-only lifecycle method a consumer
  used is ledgered.

### D-H3.7 (REVISED) — mirror freshness as a SEQUENCING INVARIANT [resolves H3-DR WEAK item 6]
Not just a parity test. The invariant: **every v0 mutator reachable from a
mixed (partly-migrated) cluster mirrors into the store synchronously before
control returns**, so any subsequent migrated read observes a fresh mirror.
H2a/b already mirror synchronously inside the v0 mutators (append/remove/insert/
splice/swap/clear/setProperty/select); H3 adds NO async mirror. Known
`back_ptr`-bridge reads (selection counts, container counts that still consult the
v0 object) are documented per cluster and converted to store reads as their
cluster migrates. No interleaving of an un-mirrored v0 write with a migrated read
within a single logical operation.

### D-H3.5 (REVISED) — build/CI gate is the full non-VIEW `libBALL` [resolves H3-DR WEAK item 4]
The green gate at each cluster is the **current default build** — full `libBALL`
with `BALL_HAS_VIEW=OFF` (VIEW/APPLICATIONS excluded) + all `bin/TEST/` binaries +
full `ctest` — NOT `BALL_CORE_ONLY` (which excludes most H3 clusters and would
hide regressions). This is exactly the build/ctest gate used for the
persistence-removal + H2c commits (284/284). VIEW remains deferred (Class K).
FORMAT/PDB ordering: the **PDB-attribute columns/accessors** (D-H3.6) move into
**H3a** (foundational, needed before STRUCTURE/FORMAT read them); the PDBFile
producer/consumer migration stays in the **H3f FORMAT** cluster. No earlier
cluster depends on a later one (no back-edge).

### Open sub-questions — RESOLVED
1. Shim scope = **full** (D-H3.2 extended). 2. `BondHandle` in **H3a**.
3. Cluster granularity: per-module, with **STRUCTURE allowed sub-commits**
(H3c.1/.2) given its size. 4. Property mutable-ref break (`eachProperty`/
`propertyNames`) lands in **H3a**. 5. (the new one) pointer-identity replacement =
**stable-id keys** (D-H3.8).

**Re-review:** Codex **H3-DR2** verifies this revision resolves the two FLAWs +
WEAKs before H3a starts.

## H3-DR2 re-review

Verdict: **NEEDS-REVISION**. R2 fixes the direction of the prior review, but H3a
should not start yet: the stable-id key policy only exists for atoms, not bonds,
and the revised handle-`apply` contract incorrectly claims topology mutation
during v0 `apply` is already forbidden/equivalent.

1. **FLAW** — D-H3.8 is sound for `Atom*` identity only: `MoleculeStore`
   allocates fresh atom `StableId`s before slot reuse, preserves them through
   `compact()`, and validates uniqueness on restore; however `BondRecord` has no
   stable id and bond slots are tombstoned/reused through `bond_free_list_`, so
   `Bond*`-keyed containers cannot be safely re-keyed yet. Some algorithms also
   use pointer/order only as arbitrary tie-breaking, so stable-id ordering is
   acceptable there, but R2 needs a bond identity story and a grep/check gate
   that rejects stored `getAtom()`/`getBond()` bridge keys.
2. **FLAW** — D-H3.4 correctly names the missing lifecycle/result pieces, but
   its semantics are not faithful enough as written: v0 `Composite::apply<T>`
   returns `bool`, calls `start()`/`finish()`, treats `BREAK` as successful early
   stop and `ABORT` as failure, and supports child/ancestor/level/pre/postorder
   variants. More importantly, topology mutation during v0 apply is not merely
   forbidden: `AddHydrogenProcessor` appends atoms and creates bonds while being
   applied, and `BuildBondsProcessor` mutates bond topology inside its processor
   call. H3a needs an explicit compatibility plan for these processors, not a
   blanket "collect first" rule claimed as v0-equivalent.
3. **WEAK** — D-H3.6 is mostly sound for the actual `PDBAtom` data members:
   branch designator, remoteness indicator, alternate-location indicator,
   occupancy, temperature factor, plus the `PROPERTY__HETATM` bit are per-atom
   scalar attributes; parent protein/chain/residue accessors are hierarchy
   queries, not PDB-only storage. The unresolved risk is behavior that uses
   `PDBAtom` type identity as a class test (`getPDBAtom`, PDB iterators,
   extractors, `dynamic_cast<const PDBAtom*>` in FORMAT/VIEW/TOOLS). Plain
   default-valued attributes do not reproduce "this child is specifically a
   PDBAtom" unless R2 adds a replacement origin/type predicate or explicitly
   ledgers the semantic break.
4. **WEAK** — D-H3.2 is much more complete and covers the obvious bond,
   interaction, iterator, distance, equality/hash, property-visitor, and
   lifecycle gaps, but the inventory still needs a formal consumed-surface grep
   gate. Remaining high-risk surfaces include `Atom::BondIterator` macro
   semantics (`BALL_FOREACH_BOND` / interbond / intrabond), `getBond(Atom&)`,
   `Bond::getFirstAtom/getSecondAtom/getPartner`, ordered bond traversal, and
   public containers returning pointer lists/maps.
5. **WEAK** — D-H3.7 is the right invariant, and H2a/b contain many synchronous
   mirror hooks for insert/remove/splice/swap/clear/select and bond creation /
   deletion. It is still not proven as a blanket claim: bridge reads remain
   (`container_selection_count_`, `back_ptr`, `bond_back_ptr`), property mirroring
   is only safe through the public mutators, and mirror suppression during bulk
   clone is intentionally staged. H3 should require each cluster to enumerate
   the v0 mutators it still invokes and prove/read-test synchronous freshness.
6. **SOUND** — D-H3.5 names the right gate: full non-VIEW `libBALL` with
   `BALL_HAS_VIEW=OFF`, all `bin/TEST` binaries, and full `ctest`, not
   `BALL_CORE_ONLY`. Moving PDB attribute columns/accessors into H3a and leaving
   PDBFile migration for H3f has no obvious back-edge if the attributes are
   foundational and generic.
7. **WEAK** — The resolved open questions are directionally right: full shim,
   `BondHandle` in H3a, STRUCTURE sub-commits, `eachProperty` in H3a, and
   stable-id keys. They are not fully closed until `BondHandle` has ABA-safe
   identity and the shim/key inventory is enforced mechanically.
8. **FLAW** — Residual blocker for H3a: R2 still lacks a complete identity model
   for bonds and a processor migration model for topology-mutating processors.
   Fix D-H3.8 with bond stable ids (or an explicitly non-reused bond identity)
   and fix D-H3.4 to classify mutating processors into snapshot/collect-first/
   specialized traversal paths before declaring design lock.
