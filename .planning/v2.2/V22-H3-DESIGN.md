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
