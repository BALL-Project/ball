# BALL v2.2 — H3 Design: consumer migration (clustered)

**Status:** ✅ DESIGN LOCKED (R3) — Codex H3-DR3 GO (2026-05-25). H3-DR + H3-DR2 (NEEDS-REVISION) resolved via R2 (stable-id keys; PDBAtom subtype eliminated) + R3 (bond stable-ids 1a; topology-mutating processors two-pass in H3c 2b). The "## H3 revision R3" section supersedes all prior conflicting decisions; H3-DR3 GO at end. Implementing H3a.
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

---

## H3 revision R3 — H3-DR2 resolutions + maintainer decisions (2026-05-25)

Closes the two H3-DR2 FLAWs + the WEAKs. Maintainer decisions: **(1a) add bond
stable-ids**; **(2b) two-pass-rewrite the topology-mutating processors in H3**
(NOT deferred to H4). SUPERSEDES the same-numbered items where in conflict.

### D-H3.8 (REVISED R3) — identity = atom AND bond stable-ids [closes H3-DR2 FLAW item 1/8]
- **Bond stable-ids added (H3a):** mirror the atom `StableId` machinery on the
  bond table — a `bond_stable_ids_` column, a fresh id drawn on every bond
  creation (incl. free-list slot reuse, so a recycled bond slot gets a NEW id =
  ABA-safe), preserved across `compact()`, uniqueness validated on JSON restore.
  `BondHandle` validity/identity use bond `StableId` (the H2c `AtomHandle`
  pattern). This is foundational H3a store work, symmetric to atoms.
- `Atom*`/`Bond*`-identity containers (`Selector::selected_atoms_`,
  `MolecularGraph`, `MolecularInteractions`, SMARTS/ring/kekulizer/`sdGenerator`/
  `atomBijection`) re-key on `StableId`. `AtomHandle`/`BondHandle` get
  `std::hash` + `operator<` on `{store, stable_id}` so they are drop-in keys; an
  algorithm that used pointer order only as an arbitrary tie-break gets the same
  determinism from stable-id order.
- **Check gate (CI, D31b-style):** a grep gate REJECTS a stored `getAtom()` /
  `getBond()` result used as a map/set/vector key (transition shim only, never a
  stored key). Added with the H3a infra; enforced per cluster.

### D-H3.4 (REVISED R3) — handle apply forbids mid-walk topology mutation; mutating processors are two-pass-rewritten in H3c [closes H3-DR2 FLAW item 2]
- The handle `apply` (D-H3.4 R2 contract: `Processor::Result` CONTINUE/BREAK/
  ABORT, `bool` return, `start()/finish()`, pre/postorder + child/level variants)
  **forbids topology mutation during the walk** — correct for the read-only
  majority.
- The bounded set of processors that mutate topology *during* application —
  STRUCTURE: `addHydrogenProcessor`, `buildBondsProcessor`,
  `assignBondOrderProcessor`, `HBondProcessor`, `disulfidBondProcessor`,
  `peptideCapProcessor`, `secondaryStructureProcessor`,
  `sideChainPlacementProcessor`, `fragmentDB`, `BONDORDERS/FPTBondOrderStrategy`
  (the H3c cluster; confirmed by the D-H3.2 grep gate) — are **two-pass
  rewritten**: pass 1 walks read-only collecting the `AtomHandle`/`BondHandle`
  work set (+ stable-id keys); pass 2 mutates via the store/container mutators
  (`createBond`/`destroyBond`, container insert/remove) AFTER the walk completes.
  This is part of each one's H3c migration, not a blanket claim. Where a
  processor's mutation is inherently interleaved (must read freshly-created atoms
  mid-walk), it stays on the v0 `Composite::apply` path with an explicit ledger
  note and migrates at H4 — but the default + expectation is two-pass.

### D-H3.6 (REVISED R3) — PDBAtom origin predicate replaces the type test [closes H3-DR2 WEAK item 3]
PDBAtom-elimination (R2) stands. To replace `PDBAtom` *type identity*
(`dynamic_cast<const PDBAtom*>`, `getPDBAtom`, PDB iterators, extractors): add an
explicit **origin attribute** on the atom (e.g. `AtomHandle::isFromPDB()` backed
by an origin enum/flag column, set when a PDB source materialises the atom),
since the PDBAtom fields (branch designator, remoteness indicator, alt-location,
occupancy, B-factor, `PROPERTY__HETATM`) are per-atom scalars and do not
themselves prove PDB origin. Type-identity consumers re-express as
origin-filtered `AtomHandle` queries; any residual pure type-test that the origin
predicate cannot reproduce is **ledgered** as an intentional break.

### D-H3.2 (R3) — consumed-surface grep gate [closes H3-DR2 WEAK item 4]
Each cluster begins with a mandated grep enumerating its `Atom*`/`Bond*`/
`Composite*`/`dynamic_cast`/`BALL_FOREACH_BOND`/`getBond(Atom&)`/`Bond::getFirst
Atom/getSecondAtom/getPartner`/`AtomBondIterator`/pointer-list-returning sites;
each site is converted to the handle/stable-id surface or ledgered. The gate is
the cluster's migration checklist + its commit's verification evidence.

### D-H3.7 (R3) — per-cluster mirror-freshness enumeration [closes H3-DR2 WEAK item 5]
The sequencing invariant (R2) stands; additionally each cluster's commit
**enumerates the v0 mutators it still invokes** and includes a read-test proving
synchronous mirror freshness for them. Known bridge reads
(`container_selection_count_`, `back_ptr`, `bond_back_ptr`) are documented and
converted to store reads as their cluster migrates.

### Build gate (R3, confirmed SOUND in H3-DR2)
Full non-VIEW `libBALL` (`BALL_HAS_VIEW=OFF`) + all `bin/TEST/` + full `ctest`
(284/284 baseline), per cluster. VIEW deferred (Class K).

**Re-review:** Codex **H3-DR3** verifies R3 closes both FLAWs; on GO the design is
locked and H3a (atom+bond stable-id-keyed handle shim + handle-processor contract
+ check gate) starts.

## H3-DR3 re-review

Verdict: **GO**. R3 closes both H3-DR2 blocking flaws; design locked, H3a starts.

1. **SOUND** — D-H3.8 is feasible and ABA-safe: the current bond table has one allocation choke point (`add_bond`), tombstone/free-list reuse, lazy CSR row-index rebuild, and no bond-row movement in `compact()`, so a parallel `bond_stable_ids_` column can allocate fresh ids on every create/reuse, preserve ids across `compact()`, validate restore uniqueness, and survive clone/new-bond paths with intentionally new ids.
2. **SOUND** — D-H3.4 is correct for the named mutating processors: `addHydrogenProcessor` can snapshot original targets and add per-target hydrogens sequentially in pass 2, `buildBondsProcessor` already separates candidate discovery from bond/order mutation, and the other STRUCTURE mutators are collect-delete/create/update cases rather than inherently traversal-interleaved; the v0+ledger escape is scoped to a proven exception, not a default hole.
3. **SOUND** — D-H3.6's origin flag plus PDB attribute columns faithfully replaces `PDBAtom` type identity for `getPDBAtom`, PDB iterators, extractors, and FORMAT dispatch; remaining `PDBAtom*` APIs become origin-filtered `AtomHandle` queries or ledgered API breaks, not a semantic blocker.
4. **SOUND** — D-H3.2 and D-H3.7 are sufficient per-cluster process gates: grep cannot prove arbitrary C++ alias/dataflow, but the mandated enumeration of pointer/bond/type-test surfaces plus per-cluster mirror-mutator freshness tests is realistic and enforceable as migration evidence.
5. **SOUND** — No residual H3a blocker remains; the remaining risks are implementation checklist items for H3a/H3c, not design flaws requiring another revision.

## H3a-CR close review

Verdict: **GO-WITH-FIXES**. H3a is mostly additive and the stable-id/processor/nav/property foundations are sound, but close-out should not be marked GO until the two real shim bugs below are fixed: `AtomHandle::select()/deselect()` must preserve the dual-existence selection contract, and `BondHandle::getPartner()` must reject invalid/cross-store atom handles. Recommended follow-up: use `std::less<MoleculeStore*>` / `std::less<const void*>` for cross-store `operator<` comparators instead of raw pointer `<`.

1. **SOUND** — Bond stable-ids cover the only bond row creation path (`add_bond`) in both append and free-list reuse, clear the column/reset the allocator in `clear()`, and are unaffected by `compact()` because bond rows are not moved. The `UINT64_MAX` guard mirrors atom IDs and is called under `bond_mutex_`; construction-time exhaustion is not practically testable without private-state injection. Bulk load currently creates fresh bond IDs via `add_bond`, which is acceptable for H3a unless persisted bond identity is later required.
2. **FLAW** — `BondHandle::isValid()` and `getBond()` are correctly gated, but `getPartner(const AtomHandle&)` only checks `!a` and compares the passed slot index. A stale handle, freed handle, or valid handle from a different store with the same index can be accepted as an endpoint and return an atom from the bond's store. Fix: require `a.isValid()` and `a.getStore() == store_` before comparing endpoints.
3. **FLAW** — Scalar store-column forwarding is fine, but selection is not equivalent to v0 during dual existence. `AtomHandle::select()/deselect()` write only `MoleculeStore::selection_`; v0 selection state and store `container_selection_count_()` still read through `back_ptr()->isSelected()`, and `selection_generation_` is not marked dirty. This can make `AtomHandle::isSelected()` disagree with v0/container selection observers. Fix selection through the v0 bridge during dual existence or add a store mutator that updates every current source-of-truth observer.
4. **WEAK** — `atom_parent_container_idx()` is encapsulated and bounds-guarded for `i >= back_ptr_.size()`, and `AtomHandle::getParent()` gates on `isValid()`, so live/freed/orphan/post-compact atom states are handled. The parent-role walks assume the container mirror is acyclic; that matches the existing mirror contract, but a defensive visited/step guard would avoid infinite loops if future mirror corruption introduces a parent cycle.
5. **SOUND** — The property shim is acceptable for dual existence: invalid handles no-op/read empty, and forwarding through `getAtom()` keeps HCP-1P store-backed property behavior until H4. H4 rewiring should be straightforward because the public surface is value/visitor-based. `eachProperty` is not mutation-stable if the visitor mutates properties; that is consistent with the normal single-thread/no-mutation-during-iteration contract and should be documented if exposed broadly.
6. **SOUND** — `applyAtomProcessor()` matches the v0 short-circuit shape: `start()` gates the walk, `ABORT` fails and skips `finish()`, `BREAK` is success early-stop and runs `finish()`, and topology mutation during the walk remains intentionally unenforced per D-H3.4's two-pass H3c plan.
7. **WEAK** — The leak gate catches the common direct forms (`HashSet`, `HashMap`, `StringHashMap`, STL set/map/unordered/vector with first template arg `(const )?(Atom|Bond)*`) in files that include handle headers. It intentionally misses aliases, nested pointer keys such as `std::set<std::pair<Atom*, Atom*>>`, boost containers, and containers introduced through included typedefs; acceptable as a guardrail, not as proof. False positives in H3b should be manageable because the gate only triggers on handle opt-in files.
8. **SOUND** — The reviewed handle/query headers do not include `_moleculeStoreInternal.h` or name private `ContainerRow`/`ContainerTable` types. `structureQuery.h` uses public `ContainerChildRef`; `atom_parent_container_idx()` delegates to the private table only in `moleculeStore.C`, preserving the D66a/D31b boundary.
9. **WEAK** — No ABI blocker found for the inline value types or `std::hash` specializations, and hash collisions are harmless under equality. The cross-store `operator<` implementations use raw pointer `<`; replace with `std::less<MoleculeStore*>()` or `std::less<const void*>()` to get the standard total-order comparator intended for unrelated object pointers.

## H3b-CR close review

5 sub-commits reviewed (7a65bd3be / 30788d13a / 9b5691722 / d39bad8cc / 31c761138).

Verdict: **GO-WITH-FIXES (applied)**. The handle extractor cluster (atomHandles
+ bondHandles + Expression + generic-callable + role-based residue/chain/SS +
kind-set molecule/fragment + ExpressionPredicate + CompiledExpression bitmap)
is sound; the HandleKeyLeakGate stays clean throughout. Four FLAWs landed in
one H3b-CR commit before continuing:

1. **FLAW 1 — root self-inclusion (v0 parity).** v0 `extractors.h::molecules(c)`
   /`residues(c)`/`chains(c)`/`secondaryStructures(c)` iterate
   `beginAtomContainer()` which visits the root first; the H3b cuts walked
   descendants only, so e.g. `moleculeHandles(prot)` was `0` where v0
   `molecules(prot)` would include `prot` itself. Fixed in `extractorsHandle.h`
   for all five typed extractors: emit `root` as the first element if it
   matches the role/kind, then iterate descendants. Test fixture added:
   `moleculeHandles(prot)` returns 1 (the PROTEIN itself);
   `chainHandles(ch)` returns 1; `residueHandles(r)` returns 1.

2. **FLAW 2 — HCP-2 enum-shrink forward-stability documentation.** The
   predicate group sets `isMoleculeKind_` / `isFragmentKind_` directly name
   enum constants (`PROTEIN`, `NUCLEIC_ACID`, `CHAIN`, `RESIDUE`,
   `SECONDARY_STRUCTURE`, `NUCLEOTIDE`) that disappear at the HCP-2 shrink.
   The earlier comment claimed "works unchanged post-shrink" which is false:
   the file stops compiling at that commit. Logged as **Class L** in
   V22-API-BREAK-LEDGER.md with the full inventory of sites that must be
   hand-edited at the shrink commit (containerHandle.h legacy-handle macro,
   extractorsHandle.h predicates, system.C kindOfContainer_). Comment in
   extractorsHandle.h refreshed to "intentional hard-fail at shrink commit".

3. **FLAW 3 — `bondHandles(c, selected_only=true)` v0 parity.** v0
   extractors.C:108-130 iterates the SELECTED atoms and inserts ALL their
   bonds; the bond participates iff at least one endpoint is selected. The
   H3b.1 cut required BOTH endpoints to be selected -- a silent
   behavior-change for consumers migrating off v0 `bonds(c, true)`. Restored
   v0 semantics; the `BondHandle f/s` endpoint cross-check is gone. Test
   fixture added: `a-b-c` with only `a` selected -> 1 bond (`a-b`).

4. **FLAW 4 — `atomHandlesBy` single-store assertion.** The bitmap path
   resolves the store from `all[0].getStore()` and trusts every other handle
   to share it. Today asContainerHandle + atom traversal can only yield one
   store per subtree, but defensive `getStore() == store` checks across the
   handles now throw `Exception::InvalidArgument` instead of silently
   bitmap-missing -- so any future mirror corruption produces a loud failure
   in the migration bridge.

NITs addressed in the same commit:

- **NIT 5** -- `asContainerHandle` no longer uses `const_cast`. The Composite
  / AtomContainer accessors are const-qualified; the const_cast was
  spurious and confusing to readers.
- **NIT 7** -- `StructureQuery::containersByKind` was per-node-allocating
  vectors during recursion; rewritten to a `detail::visitContainersByKind_`
  visitor that writes into a single output vector. (extractorsHandle.h's
  `visitContainersIf_` already had this shape.)
- **NIT 8** -- new test: interleaved sibling order (atom, container, atom
  under one parent) verifies the H2c ordered-ChildRef preorder claim against
  v0 `AtomIterator`.
- **NIT 9** -- new test: atomHandlesBy with intervening freed atom slots in
  the store, so the CompiledExpression bitmap has interior zeros. The
  intersection-by-store-index path stays correct.

NIT 6 (silent-skip on `getAtom() == nullptr`) is deferred: under H3 dual
existence v0 backing is always present; once it disappears the silent-skip
behavior becomes meaningful and the overload's contract gets revisited then.

**Outcome:** ctest 287/287 PASS post-fixes. HandleKeyLeakGate clean. H3b
cluster CLOSED at H3b-CR. Move on to next H3 cluster after H3b's deferred
piece (PDBAtom-handle extractor over D-H3.6 origin predicate, needs H3a.3b
deferred work).

## H3a.3b design — PDBAtom origin (D-H3.6 detail)

**Goal:** D-H3.6 eliminates the `PDBAtom` subtype forward; pre-H4 the v0
`PDBAtom` class stays, and we add a forward-stable **origin marker** on the
atom store that says "this slot was constructed as a PDBAtom". The marker
becomes the SOLE truth post-H4 when the subtype is removed; pre-H4 it MUST
agree with `RTTI::isKindOf<PDBAtom>(back_ptr)` (dual-existence parity, D60).

### D-H3.10 — `pdb_origin_bits_` SoA column on `MoleculeStore`

```cpp
// include/BALL/KERNEL/moleculeStore.h, alongside selection_/is_freed_
std::vector<std::uint8_t> pdb_origin_bits_;   // K0.5.5 / H3a.3b origin flag
```

- One byte per slot (matches `selection_`, `is_freed_` shape). Bit-packing
  is deferred — atom-slot scalar columns are byte-per-slot today; staying
  byte-per-slot keeps the access pattern uniform.
- Forward-extensible: at HCP-3+ when other class-of-origin tags ship (e.g.
  NMR atoms), we can either add a parallel column or pack into one
  `origin_flags_` column. The decision can wait — adding the column is
  cheap relative to changing the schema after the fact, and naming it
  `pdb_origin_bits_` makes the intent explicit at the call site.
- Read accessor: `bool is_pdb_origin(std::uint32_t idx) const` (slot-bound
  query, no validity assertion — the AtomHandle layer does that).
- Write accessor: `void set_pdb_origin(std::uint32_t idx, bool on)`.

### D-H3.11 — Lifecycle invariants

| Event | Bit transition | Justification |
|---|---|---|
| `add_atom()` (fresh slot or recycled) | initialised to **0** | Birth-default: no atom is PDB-origin until explicitly marked. Equivalent to today's `back_ptr_[i] = nullptr` initialisation. |
| `release_atom(i)` | set to **0** | Reset on release, NOT on the next `add_atom`. Matches `selection_[i] = 0` policy. Prevents a recycled slot from inheriting the previous occupant's PDB-origin. |
| `PDBAtom::PDBAtom(...)` ctor body | set to **1** via `store->set_pdb_origin(getStoreIndex(), true)` | Atom base ctor allocates the slot; PDBAtom body marks it. The window where the slot exists but bit=0 is intra-ctor (no observer sees it). |
| `compact()` | column compacted parallel to all other slot-bound columns | Slot index changes — bit travels with the slot, via the same swap-down pattern used for `selection_`, `is_freed_`, `back_ptr_`. |
| `clear()` | column resized to 0 alongside other columns | The whole store goes away; trivially preserved. |
| `PDBAtom::operator= (deep)` and `set()`  | bit UNCHANGED | The slot's class-of-origin is a stable property; assigning attribute values into a PDBAtom does not change its origin. |
| `PDBAtom::clear()` (v0 attribute reset, NOT slot release) | bit UNCHANGED | Same rationale — attribute clear ≠ class-of-origin change. |

### D-H3.12 — Handle-level API

```cpp
// include/BALL/KERNEL/atomHandle.h (additive)
bool isPDBOrigin() const
{
    return isValid() && store_->is_pdb_origin(idx_);
}
```

```cpp
// include/BALL/KERNEL/extractorsHandle.h (additive)
inline std::vector<AtomHandle> pdbAtomHandles(const AtomContainer& fragment)
{
    return atomHandlesIf(fragment,
        [](const AtomHandle& h) { return h.isPDBOrigin(); });
}

inline std::vector<AtomHandle> pdbAtomHandles(const AtomContainer& fragment,
                                              const String& expression)
{
    std::vector<AtomHandle> all = atomHandles(fragment, expression);
    std::vector<AtomHandle> out;
    for (Size i = 0; i < all.size(); ++i)
        if (all[i].isPDBOrigin()) out.push_back(all[i]);
    return out;
}
```

This is the forward-stable replacement of v0
`extractors.h::PDBAtoms(c)` / `PDBAtoms(c, expr)`. Returns AtomHandles
(not PDBAtom*); consumers fetch PDB attributes through either the
v0 bridge (`h.getAtom()` → `dynamic_cast<PDBAtom*>` pre-H4) or future
`AtomHandle::getOccupancy()/getTemperatureFactor()` if/when those
columns migrate (post-HCP-3).

### D-H3.13 — Dual-existence parity test

Pre-H4 the v0 RTTI must agree with the bit:

```
for every live slot i:
    isKindOf<PDBAtom>(back_ptr_[i]) == is_pdb_origin(i)
```

Add this as a randomised parity sweep in `HierarchyParity_test` (touch the
PDBAtom-creating fixtures) or as a standalone gate inside a new
`PDBAtomOriginParity_test`. The cluster review (H3a.3b-CR) checks coverage
of release/recycle/compact paths.

### D-H3.14 — Out of scope for H3a.3b (deferred)

- **PDB attribute columns** (`branch_designator_`, `remoteness_indicator_`,
  `alternate_location_indicator_`, `occupancy_`, `temperature_factor_`)
  STAY on v0 `PDBAtom`. Their migration to store columns is HCP-3 work
  and orthogonal to the origin flag.
- **PDBAtom property bit** (`PROPERTY__HETATM`) STAYS on the v0 property
  bag (read/written via the HCP-1P PropertyColumnRegistry shim). No
  migration here.
- **PDB Iterator / FORMAT/PDBFile dispatch** STAYS on v0 dynamic_cast
  until H4 (only the *handle-side* consumers migrate now, via
  `pdbAtomHandles`).

### Open questions for Codex (H3a.3b-DR review prompt)

Q1: Is the slot-recycled invariant (bit reset on `release_atom`, NOT on
`add_atom`) correct? Could there be a code path that bypasses `release_atom`
and reuses a slot directly?

Q2: PDBAtom's `operator=` / `set(deep=true)` keeps the bit unchanged. Is
that genuinely correct, or does the v0 semantic of "deep assignment from
a non-PDBAtom Atom" exist somewhere and create a slot that's PDB-origin in
the bit but not in the v0 class? (Likely no — `Atom::operator=(Atom&)` is
distinct from `PDBAtom::operator=(PDBAtom&)`, but worth verifying.)

Q3: Should `pdb_origin_bits_` be made `friend` to a narrow lifecycle helper,
or kept on the `MoleculeStore` accessor surface and called by `PDBAtom`'s
ctor through a public store getter? (Symmetric to how `back_ptr_` is set.)

Q4: Bond-aware extractors -- no change needed (bonds don't have a
class-of-origin), correct?

Q5: For the dual-existence parity test, where in the test surface is the
right home? `HierarchyParity_test` already has a randomised sweep; a small
`PDBAtomOriginParity_test` keeps the gate focused on this single
invariant. Pick one.

## H3a.3b-DR review (Codex)

Verdict: **NOT-GO** — 3 BLOCKERs + 6 FLAWs + 2 NITs. Adoption misses the new
column (System::adopt + adoptSubtreeInto_ snapshot atom payload but not
origin), detached rebinding via ensureStoreBinding_ silently allocates an
unmarked slot post-System-teardown, post-H4 has no writer once PDBAtom is
gone. Also: compact() does NOT swap-down today (FLAW 4 mis-statement),
operator= goes through Atom::operator= NOT set() (FLAW 6 mis-statement),
persistence-scope was un-specified, parity invariant must scope-guard
store-only loads, test surface under-specified.

## H3a.3b-R2 — Revised design (addressing the 3 BLOCKERs + 6 FLAWs)

### D-H3.10-R2 — Column rename + allocation-time origin hint

Rename `pdb_origin_bits_` -> **`origin_flags_`** (`std::vector<std::uint8_t>`,
bit 0 = PDB origin; bits 1..7 reserved for future origin classes per NIT 10).

The construction protocol becomes an **allocation-time hint** rather than a
post-bind write. This is the central change vs the original draft and
collapses BLOCKERs 1, 3, 5 + FLAW 5 into a single atomic operation under
the existing orphan-mutex lock:

```cpp
// moleculeStore.h (signature change):
Index allocate_atom();                                 // bit 0 = 0
Index allocate_atom(Atom* back_ptr);                   // bit 0 = 0
Index allocate_atom(Atom* back_ptr, std::uint8_t origin_flags);  // NEW

// PDBAtom.C ctor (orphan path):
store_idx_ = store.allocate_atom(this, /*origin_flags=*/0x01);
```

Why allocation-time:
- **Atomic with allocation under orphan_mutex_** — no post-bind write
  needs its own locking story; the bit lands in the SAME critical
  section that allocates and sets back_ptr (atom.C:32).
- **No ctor-window observability gap** — the bit is set at the same
  moment the slot becomes visible to any observer.
- **Forward-stable post-H4** — once PDBAtom subtype is gone, the PDB
  format loader / handle-level factory passes the same flag through
  `allocate_atom(nullptr, 0x01)`. No subtype constructor required.

### D-H3.11-R2 — Lifecycle invariants (revised)

| Event | bit transition | Justification / verification |
|---|---|---|
| `allocate_atom(back_ptr, flags)` — fresh slot or recycled | bit = (flags >> 0) & 1 | Atomic with slot bind. Recycled slots are zeroed at `release_atom`, so the hint always wins. |
| `release_atom(i)` | bit = 0 | Reset on release. Mirror of `selection_[i] = 0` policy (moleculeStore.C). |
| `add_atom()` / `allocate_atom(back_ptr)` no-flag overloads | bit = 0 | Birth-default zero for the non-PDB code paths. |
| `PDBAtom::PDBAtom(...)` ctor | bit set via allocate_atom hint | NOT a post-bind write. |
| `PDBAtom::~PDBAtom` → release_atom | bit = 0 (above) | |
| `PDBAtom::operator=(PDBAtom&)` (which delegates to `Atom::operator=`, per FLAW 6 correction) | bit UNCHANGED | The slot stays the same slot; assigning v0 attribute values does NOT change origin. |
| `PDBAtom::set(deep=true)` (calls Atom::set → ensureStoreBinding_ if unbound) | bit copied from `pdb_atom` source | **BLOCKER 2 fix**: `PDBAtom::set` is the rebind reentry point; override it so the *new* slot allocation passes origin_flags=0x01 (or pass it down through Atom::ensureStoreBinding_ + an Atom::origin_hint_ field set by PDBAtom). |
| `Atom::ensureStoreBinding_()` (unbound atom needs an orphan slot at first non-ctor mutation) | reads atom's `origin_hint_` field; passes to allocate_atom | A new private `Atom::origin_hint_: std::uint8_t = 0`. PDBAtom ctor sets `origin_hint_ = 0x01`. ensureStoreBinding_ passes it through. (FLAW 6 / BLOCKER 2 fix.) |
| `System::adopt(Atom&)` | copy src bit -> dst bit alongside other columns | **BLOCKER 1 fix**: extend the snapshot block in system.C:120-150 with `std::uint8_t origin = src->origin_flags(src_idx)`, then `dst->set_origin_flags(dst_idx, origin)` after allocate_atom. Same for `adoptSubtreeInto_` (system.C:340-355). |
| `compact()` | column compacted parallel to other slot-bound columns if/when swap-down lands | **FLAW 4 fix**: today `compact()` only `shrink_to_fit()` + rebuilds string pools (moleculeStore.C:1172) — there is no slot-index motion to track. The column lives in the same vector + `shrink_to_fit` set as other atom columns. The FUTURE swap-down work (separate milestone) lands a full slot-move algorithm covering ALL atom columns at once; origin_flags participates symmetrically with selection_/back_ptr_/stable_ids_ at that time. |
| `clear()` | column resized to 0 alongside other columns | Trivial — store goes away. |
| `loadStoreJSON` (persistence read path) | bit = 0 for every slot (current minor version) | **FLAW 7 fix**: H3a.3b explicitly DEFERS persistence of origin_flags. Store/System JSON minor-version stays put. The post-load slot is bit=0; a future minor-version bump adds the column with `origin_flags == 0` interpreted as "unknown origin" for backward compatibility. |

### D-H3.12-R2 — Handle-level API (unchanged from R1)

```cpp
// AtomHandle
bool isPDBOrigin() const
{ return isValid() && (store_->origin_flags(idx_) & 0x01u) != 0; }

// extractorsHandle.h
inline std::vector<AtomHandle> pdbAtomHandles(const AtomContainer& fragment);
inline std::vector<AtomHandle> pdbAtomHandles(const AtomContainer&, const String& expr);
```

`AtomHandle` writer API for post-H4 is `markPDBOrigin(bool)` (new), set as
a *creation hint* not a post-construction mutation — i.e. the v0 PDBAtom
ctor path eventually disappears and is replaced by callers building atoms
via a factory function `AtomHandle createPDBAtom(MoleculeStore&, ...)` that
funnels through `allocate_atom(..., 0x01)`. Post-H4 detail; not coded now.

### D-H3.13-R2 — Parity invariant (scope-guarded for store-only state)

The dual-existence parity invariant becomes (**FLAW 8 fix**):

```
for every slot i where back_ptr_[i] != nullptr:
    RTTI::isKindOf<PDBAtom>(back_ptr_[i]) == ((origin_flags_[i] & 0x01) != 0)
```

i.e., the invariant is over the dual-existence range only. Store-only
slots (loaded via JSON; back_ptr is nullptr) carry the bit by itself --
the bit IS the truth, with no v0 to compare. The same pattern v2.1
adopted for `selection_` columns vs `Atom::isSelected()` (D60).

### D-H3.14-R2 — Out of scope for H3a.3b (revised)

Unchanged + add:
- **JSON persistence of origin_flags** (FLAW 7 deferral)
- **Typed v0 APIs that stay**: `AtomContainer::getPDBAtom`, `countPDBAtoms`
  in Protein/Chain/Residue/SecondaryStructure (NIT 12). These keep
  `dynamic_cast<PDBAtom*>(&*it)` until H4. Handle-side replacement is
  `pdbAtomHandles(c)`.
- **Future bond-origin / origin-class extensions** (Q4 was: bonds don't
  need this — confirmed).

### D-H3.15 — Test surface (FLAW 9 fix — explicit coverage)

A new focused `PDBAtomOriginParity_test` with these CHECK blocks:

1. **Orphan ctor**: `PDBAtom a;` → orphan store slot has bit=1.
2. **Plain Atom ctor**: `Atom a;` → orphan store slot has bit=0.
3. **System adoption (single atom)**: orphan PDBAtom → System::insert →
   System's store has bit=1 at the dst slot, src orphan slot bit=0
   after release.
4. **System adoption (subtree)**: same with a Protein/Chain/Residue tree
   containing PDBAtoms; bits preserved across adoptSubtreeInto_.
5. **Release + recycle into non-PDB Atom**: free a PDBAtom slot, allocate
   an Atom at the same idx, bit=0.
6. **Detached rebinding after System teardown**: PDBAtom alive after
   ~System, mutate to trigger ensureStoreBinding_, new orphan slot is bit=1.
7. **Cross-store move**: PDBAtom in one System adopted by another System,
   bit=1 in dst store.
8. **Swap** (PDBAtom::swap with another PDBAtom): bits stay correct.
9. **Deep copy** (`PDBAtom b(a, true)`): new slot has bit=1.
10. **JSON load (FLAW 8 scope)**: store-only state with bit=0; isPDBOrigin
    returns false; no parity violation (back_ptr is nullptr → invariant
    range excludes the slot).

Also extend `HierarchyParity_test` ATTACH/REMOVE/SWAP randomized sweep
(test/HierarchyParity_test.C:1270+) with a parallel "for each live slot
where back_ptr != nullptr, assert isKindOf<PDBAtom>(back_ptr) ==
origin_flags & 0x01" pass post-each-op. Catches mirror drift in the
randomized fixture.

### Codex Q1-Q5 (answered in revised design)

Q1: Resolved — `release_atom` reset is correct AND `allocate_atom` hint
    is the new write path. compact() needs no special handling today;
    future swap-down handles all atom columns uniformly.

Q2: Resolved — `PDBAtom::operator=` (which calls `Atom::operator=` per
    FLAW 6 correction) preserves the bit. The detached-rebind case goes
    through `Atom::ensureStoreBinding_` which reads `Atom::origin_hint_`
    set by the PDBAtom ctor (D-H3.11-R2 row).

Q3: Resolved — `allocate_atom(back_ptr, origin_flags)` is the narrow
    lifecycle helper. PDBAtom ctor uses it; adoption snapshot+copy uses
    a `set_origin_flags` accessor (not a casual setter — it's the
    adoption migration path).

Q4: Confirmed — bonds have no origin concept.

Q5: Both — focused `PDBAtomOriginParity_test` (10 CHECK blocks) PLUS
    randomized parity assertion in `HierarchyParity_test`.


## H3a.3b-DR2 review (Codex)

Verdict: **NOT-GO** (again). 3 BLOCKERs persist + 4 new/refined FLAWs:

- **BLOCKER 1** — PDBAtom ctor body runs AFTER Atom base ctor's
  `bindToStore_` allocates the slot, so a derived-side `origin_hint_`
  assignment is too late for the FIRST allocation. The R2 protocol
  silently relied on dispatch that does not exist.
- **BLOCKER 2** — Adoption R2 plan: `allocate_atom(&atom)` + post-write
  `set_origin_flags`. That creates a parity-violation window: `back_ptr_
  [dst]` is observable as a PDBAtom while `origin_flags_[dst]` is still 0.
  Atomicity requires passing the flag INTO the allocation.
- **BLOCKER 3** — JSON round-trip deferral is a real observable
  fidelity loss: save+load drops PDB origin. Must add the column to JSON
  now OR document + test the loss.
- **FLAW 4** — Atom layout: a new `uint8_t origin_hint_` placed at the
  end of Atom inflates sizeof(Atom). Pack near `number_of_bonds_`
  (atom.h:981).
- **FLAW 5** — `markPDBOrigin()` handle writer contradicts the "creation
  hint only" rule. Remove from the public handle surface.
- **FLAW 6** — Test surface must cover `Atom::swap(Atom&)` between a
  PDBAtom and a plain Atom (payload swap; slot/back_ptr stays; origin
  travels with the SLOT, not the payload).
- **FLAW 7** — Test surface must cover
  `static_cast<Atom&>(pdb_atom) = plain_atom;` (base-assignment from a
  non-PDB Atom into a PDBAtom subobject — bit must stay 1).

## H3a.3b-R3 — Revised design (BLOCKER fixes done at ALLOCATION SITE)

### D-H3.10-R3 — `allocate_atom` takes the origin hint AS a parameter

Replace the post-bind-write idea entirely. The hint travels through
construction:

```cpp
// moleculeStore.h public surface (extension):
Index allocate_atom();                                      // hint = 0
Index allocate_atom(Atom* back_ptr);                        // hint = 0
Index allocate_atom(Atom* back_ptr, std::uint8_t origin_flags);   // NEW
// No "set_origin_flags" public mutator. The bit is settable ONLY at
// allocate-time; reads via `origin_flags(idx)`.

// atom.h constructor surface (additive):
Atom(std::uint8_t origin_hint = 0);
Atom(const Atom& atom, bool deep = true, std::uint8_t origin_hint = 0);
Atom(Element& element, const String& name, ...,
     std::uint8_t origin_hint = 0);    // detailed ctor adds tail param
// bindToStore_ takes the hint:
void bindToStore_(MoleculeStore& store, std::uint8_t origin_hint = 0);

// Atom private member (BLOCKER 1 anchor):
std::uint8_t origin_hint_;   // ctor-set; consulted by ensureStoreBinding_

// PDBAtom.C ctors (forward the hint through Atom):
PDBAtom::PDBAtom() : Atom(/*origin_hint=*/0x01), branch_designator_(...), ... {}
PDBAtom::PDBAtom(const PDBAtom& o, bool deep)
   : Atom(o, deep, /*origin_hint=*/0x01), ... {}
PDBAtom::PDBAtom(const String& name) : Atom(/*origin_hint=*/0x01)
   { Atom::setName(name); ... }
PDBAtom::PDBAtom(Element& e, ..., float t) : Atom(e, name, ..., 0x01) { ... }
```

This atomically lands the bit at allocation:
- `Atom::Atom(hint)` -> sets `origin_hint_ = hint` BEFORE the body.
- `bindToStore_(store, origin_hint_)` -> `store.allocate_atom(this, origin_hint_)`.
- `allocate_atom(Atom*, uint8_t)` -> sets `back_ptr_[idx] = atom` AND
  `origin_flags_[idx] = origin_flags` under the same orphan_mutex lock
  before returning. Slot becomes visible to other threads as a
  CONSISTENT pair, never as a (back_ptr=PDBAtom, origin_flags=0) state.

**This fully closes BLOCKER 1** (construction-order atomicity).

### D-H3.11-R3 — Adoption uses the flagged overload too

System::adopt + adoptSubtreeInto_ change their snapshot block:

```cpp
// system.C:120-150 snapshot, BEFORE allocate_atom call:
const std::uint8_t origin = src->origin_flags(src_idx);
...
const std::uint32_t dst_idx = dst->allocate_atom(&atom, origin);
// No post-write needed. Atomicity = allocation site.
```

Same change in adoptSubtreeInto_ at system.C:340-355.

**This closes BLOCKER 2.**

### D-H3.12-R3 — Detached rebind via Atom::ensureStoreBinding_

ensureStoreBinding_ reads `this->origin_hint_` and forwards:

```cpp
// atom.C ensureStoreBinding_:
if (store_ == nullptr) {
    bindToStore_(MoleculeStore::orphanStore(), origin_hint_);
}
```

`origin_hint_` was set in the Atom ctor (D-H3.10-R3 above), so a PDBAtom
whose first allocation gets detached and later rebinds via
ensureStoreBinding_ reuses the stored hint -- the new orphan slot is
correctly marked.

### D-H3.12b-R3 — Drop `AtomHandle::markPDBOrigin()` from the public surface

The handle gets ONLY a reader: `isPDBOrigin() const`. There is no
post-construction writer. Post-H4, the PDB-file format loader calls
`store.allocate_atom(nullptr, 0x01)` directly via a factory function
in PDBFile (or via a public `MoleculeStore::createPDBAtomSlot()` helper).
The R2 sketch of `markPDBOrigin(bool)` is **deleted** from the design.

**Closes FLAW 5.**

### D-H3.13-R3 — Atom layout: pack `origin_hint_` near `number_of_bonds_`

Atom currently has `unsigned char number_of_bonds_;` followed by
`Bond* bond_[MAX_NUMBER_OF_BONDS];` at atom.h:981+. Insert
`std::uint8_t origin_hint_;` IMMEDIATELY after `number_of_bonds_`. Both
byte-scale fields pack into the same alignment slot the previous
single-byte field occupied (or into the natural padding before the
`Bond*` pointer array).

Verification gate:
```cpp
static_assert(sizeof(Atom) <= BALL_ATOM_SIZE_HINT,
    "Atom inflated; origin_hint_ placement violated layout budget");
```
We'll set `BALL_ATOM_SIZE_HINT = sizeof(Atom)-at-current-HEAD` during
implementation and assert no growth.

**Closes FLAW 4.**

### D-H3.14-R3 — JSON persistence: round-trip the column NOW

Reject the FLAW 7 deferral. Add `origin_flags` to:
- `moleculeStoreJson.C` writer (atoms object, alongside "selection") --
  base64 or raw byte array column same length as `selection`.
- `moleculeStoreJson.C` reader: parse "origin_flags" if present;
  zero-fill if absent (backward compat with pre-v2.2.X JSON).
- Bump JSON minor version (the format already supports minor-version
  forward-compat per existing reader).

Test: PDBAtom-bearing System -> save -> load -> `pdbAtomHandles(loaded)`
matches the original.

**Closes BLOCKER 3.**

### D-H3.15-R3 — Test surface (expanded per FLAW 6 + 7)

Replace the D-H3.15 list with these 13 CHECK blocks:

 1. Orphan ctor: `PDBAtom a;` -> bit=1.
 2. Plain Atom ctor: `Atom a;` -> bit=0.
 3. System adoption (single atom).
 4. System adoption (subtree).
 5. Release + recycle: free PDBAtom slot, allocate Atom at same idx -> bit=0.
 6. Detached rebind after System teardown (via ensureStoreBinding_).
 7. Cross-store move (System -> System).
 8. PDBAtom::swap(PDBAtom&) -- bits stay correct (both 1).
 9. **NEW (FLAW 6):** Plain `Atom::swap(Atom&)` between a PDBAtom and a
    plain Atom -- payload swaps but **origin bit travels with the SLOT
    identity, not the payload**. Verify: after swap, the PDBAtom object's
    slot bit stays 1; the plain Atom's slot bit stays 0.
10. Deep copy: `PDBAtom b(a, true);` -> new slot has bit=1.
11. **NEW (FLAW 7):** Base-assignment from non-PDB:
    `static_cast<Atom&>(pdb_atom) = plain_atom;` -- PDBAtom's slot bit
    stays 1 (the SLOT is PDB-origin; assigning a non-PDB payload does
    NOT change origin).
12. Save+Load (JSON round-trip): origin bit preserved (D-H3.14-R3).
13. HierarchyParity_test extension: randomised sweep at .C:1270+
    asserts parity invariant after each op (`back_ptr != nullptr =>
    isKindOf<PDBAtom>(back_ptr) == (origin_flags & 0x01)`).

### Summary of R3 vs R2

| Issue | R2 plan | R3 fix |
|---|---|---|
| BLOCKER 1 (ctor order) | post-bind write | hint in `allocate_atom(Atom*, uint8_t)` -- atomic with bind; PDBAtom passes via Atom ctor overload |
| BLOCKER 2 (adoption window) | snapshot + post-write | snapshot + flagged allocate; atomic |
| BLOCKER 3 (JSON loss) | deferred | add column NOW + minor-version bump |
| FLAW 4 (Atom inflation) | un-specified | pack near number_of_bonds_ + static_assert |
| FLAW 5 (markPDBOrigin) | mentioned briefly | dropped from public handle API |
| FLAW 6 (swap test) | only PDBAtom::swap | + Atom::swap(Atom&) mixed-type |
| FLAW 7 (base assign) | un-tested | base-assign test added |

## H3a.3b-DR3 review (Codex)

Verdict: **NOT-GO**, but the verdict is partly a category-error:
findings 1-7 flag "R3 not implemented at branch tip" -- correct as a
fact, but R3 is DESIGN TEXT, not landed code. Per the project's
design-lock-before-code discipline (used at H3-DR/DR2/DR3 and H2-0
prior), implementation lands AFTER design GO. So findings 1-7 collapse
to a single implementation-todo list, not design defects.

The one **genuine new finding** is:

- **FLAW 8 (NEW, GENUINE):** Changing `Atom()` to `Atom(std::uint8_t=0)`
  emits a different ctor symbol -> ABI break. Keeping both produces
  source-level ambiguity for `Atom()` calls (both viable -- the new one
  via default arg, the old one direct). R4 must preserve the existing
  public ctor signatures and add the hint via a non-defaulted overload.

## H3a.3b-R4 — Final tweak (FLAW 8 fix)

### D-H3.10-R4 — Atom ctors stay; add a TAGGED hint overload

Don't modify the existing three `Atom` ctor signatures. Instead, add
three NEW overloads taking a NON-DEFAULTED tag-typed origin parameter:

```cpp
// atom.h additions (no signature change to existing ctors):
namespace AtomCtor
{
    /// Non-defaulted tag carrying an origin-flag hint. Existing
    /// `Atom()` / `Atom(const Atom&, bool)` / detailed ctor remain
    /// unchanged; the four taking AtomCtor::Origin land alongside.
    struct Origin { std::uint8_t bits; };
}

Atom(AtomCtor::Origin origin);
Atom(const Atom& atom, bool deep, AtomCtor::Origin origin);
Atom(Element& element, const String& name, ...,
     AtomCtor::Origin origin);     // detailed ctor, origin appended
```

**ABI compat:** Existing `Atom()` symbol unchanged. New symbols are
strictly additive. **Source disambiguation:** `Atom()` is unambiguous
(the new overloads require an `Origin` argument; no default).

PDBAtom ctors call the tagged overloads explicitly:

```cpp
PDBAtom::PDBAtom() : Atom(AtomCtor::Origin{0x01}), branch_designator_(...), ... {}
PDBAtom::PDBAtom(const PDBAtom& o, bool deep)
   : Atom(o, deep, AtomCtor::Origin{0x01}), ... {}
PDBAtom::PDBAtom(Element& e, ..., float t)
   : Atom(e, name, ..., AtomCtor::Origin{0x01}), ... {}
PDBAtom::PDBAtom(const String& name) : Atom(AtomCtor::Origin{0x01})
   { Atom::setName(name); ... }
```

`AtomCtor::Origin` is a tag struct (a typed wrapper around `uint8_t`)
specifically to keep these overloads from being silently chosen for
non-PDB callers passing a stray uint8_t.

### D-H3.16-R4 — Implementation TODO list (the items DR3's findings 1-7
        identified as not-yet-implemented)

These are the implementation steps that follow design GO. NONE of them
land in the design doc -- only as actual commits after R4 GO:

1. Add `std::vector<std::uint8_t> origin_flags_` column to MoleculeStore,
   alongside `selection_` / `is_freed_`.
2. Add `Index allocate_atom(Atom*, std::uint8_t origin_flags)` overload;
   `allocate_atom_with_back_ptr_` internal helper takes the new flags
   param and writes both columns under the same lock.
3. Add `bool origin_flags(Index)` accessor (slot-bound, no validity check).
4. Add `bindToStore_(MoleculeStore&, std::uint8_t)` overload, forward to
   `allocate_atom(this, hint)`.
5. Add the three new tagged Atom ctors (D-H3.10-R4); existing 3 unchanged.
6. PDBAtom: 4 ctors call tagged Atom variants.
7. Atom layout: add `std::uint8_t origin_hint_;` immediately after
   `number_of_bonds_` (atom.h:981); `static_assert(sizeof(Atom) ==
   BALL_ATOM_SIZE_HINT)` where BALL_ATOM_SIZE_HINT = current value at
   implementation time.
8. `Atom::ensureStoreBinding_()` reads `this->origin_hint_`, passes
   to bindToStore_.
9. System::adopt + adoptSubtreeInto_: snapshot `src->origin_flags(idx)`,
   pass into the new flagged `allocate_atom(&atom, origin)`.
10. JSON writer (`moleculeStoreJson.C:127`): add origin_flags column
    alongside selection. Reader (line 301): accept missing column as
    all-zeros (backward compat). Bump minor version.
11. JSON System load (`systemJson.C:408`): use the flagged
    `allocate_atom` overload with the column value.
12. `AtomHandle::isPDBOrigin()` accessor in atomHandle.h (READ ONLY;
    no `markPDBOrigin` writer per D-H3.12b-R3).
13. extractorsHandle.h `pdbAtomHandles(c)` and
    `pdbAtomHandles(c, expression)`.
14. New `PDBAtomOriginParity_test` with the 13 CHECK blocks (D-H3.15-R3).
15. HierarchyParity_test sweep extension.

## H3a.3b-DR4 review (Codex)

Verdict: **GO-WITH-FIXES**. Three small actionable items + one optional.

## H3a.3b-R5 — DR4 fixes applied (DESIGN LOCKED)

### Fix 1 — Detailed tagged Atom ctor: fully non-defaulted signature

The existing detailed `Atom(Element&, const String& name, const String&
type_name = ..., ...)` ctor has DEFAULTED params from `type_name` on. A
tagged variant cannot just append `AtomCtor::Origin` (non-defaulted after
defaulted = ill-formed). Specify the tagged detailed overload with
EVERY param non-defaulted:

```cpp
Atom(Element& element,
     const String& name,
     const String& type_name,
     Type atom_type,
     const Vector3& position,
     const Vector3& velocity,
     const Vector3& force,
     float charge,
     float radius,
     AtomCtor::Origin origin);   // non-defaulted tail
```

PDBAtom's detailed ctor passes every value explicitly (no defaults) to
this overload.

### Fix 2 — origin_hint_ initialization explicit per ctor

All 6 Atom ctors (3 existing + 3 tagged) initialize `origin_hint_`:
- Existing 3 ctors: `origin_hint_(0)` in member-initializer list BEFORE
  `bindToStore_()` runs.
- Tagged 3 ctors: `origin_hint_(origin.bits)` in member-initializer list
  BEFORE `bindToStore_(store, origin.bits)`.

This is explicit (not implied) because R4 changed the surface; restate.

### Fix 3 — D-H3.16-R5 implementation step reorder

Reorder so every intermediate commit builds + tests green:

1. **MoleculeStore**: add `origin_flags_` column + `origin_flags(Index)`
   accessor + `allocate_atom(Atom*, std::uint8_t)` new overload (atomic
   write under orphan_mutex_; existing 2-arg overloads forward with
   flags=0) + `release_atom` zeroes column. NO callers in this commit.
2. **Atom layout**: add `std::uint8_t origin_hint_;` immediately after
   `number_of_bonds_` (atom.h:981) + initialize to 0 in EACH existing
   ctor + `static_assert(sizeof(Atom) <= 512)` retained (Sizeof_test
   guard).
3. **Atom::bindToStore_** new overload taking hint; existing 1-arg path
   forwards with hint = origin_hint_. **Atom::ensureStoreBinding_** reads
   `origin_hint_` and forwards to bindToStore_.
4. **AtomCtor::Origin** tag struct + 3 NEW tagged Atom ctor overloads.
   Each initializes `origin_hint_(origin.bits)`.
5. **PDBAtom**: 4 ctors retargeted to the tagged Atom variants.
6. **System::adopt** + **adoptSubtreeInto_**: snapshot
   `src->origin_flags(src_idx)` + pass to `dst->allocate_atom(&atom,
   origin)`.
7. **JSON writer** (moleculeStoreJson.C:127): add origin_flags column.
   **JSON reader** (line 301): treat missing column as all-zeros (no
   minor-version bump needed — backward compat by absence is cleaner).
8. **systemJson.C** (line 408): pass origin_flags via flagged
   allocate_atom on load.
9. **AtomHandle::isPDBOrigin()** read-only accessor.
10. **extractorsHandle.h pdbAtomHandles(c)** + Expression overload.
11. **PDBAtomOriginParity_test** (13 CHECKs).
12. **HierarchyParity_test** sweep extension.

### Fix 4 (non-blocking) — Rollback semantics

Rollback is additive-only and trivial: revert PDBAtom ctor retargeting,
remove tagged Atom overloads + AtomCtor::Origin, remove `origin_hint_`
member, remove `origin_flags_` column + accessors + JSON column +
isPDBOrigin + pdbAtomHandles + tests. Each step is a clean
`git revert -n <sha>` of the corresponding commit. No data loss because
origin_flags is never read by anything outside the H3a.3b surface.

**DESIGN LOCKED at R5.** Implementation per D-H3.16-R5 begins next.
