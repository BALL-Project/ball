# V22 — Full Kernel Persistence Removal (design-lock)

**Status:** DESIGN-LOCK (pre-execution). Maintainer-directed milestone.
**Decision (locked, revised after the dependency sweep):** remove the **legacy
`PersistenceManager` framework** (Text/XDR + `PersistentObject` base + the
`persistentRead/Write` virtuals) and **KEEP the v2.x JSON `StoreFormat`** as the
kernel's ONE serialization path. **Re-point the live consumers (VIEW project
save/load, MPI broadcast) onto the JSON StoreFormat** instead of `TextPersistence
Manager`/`XDRPersistenceManager`. The FORMAT-layer chemistry file I/O (PDB/MOL2/
HIN/SD/MOL/KCF/Antechamber/…) **stays** — it is molecular file I/O, not
object-graph persistence. *(Initial directive was "full persistence out incl.
JSON"; revised to keep JSON once the sweep showed VIEW project save/load + MPI
broadcast are live consumers that need a serializer — JSON is the natural target.)*
**Companion:** scope/dependency sweep (Explore agent, this session);
`V22-API-BREAK-LEDGER.md`; recommended **ahead of H4** (it strips the
persistence stream framework the flip's base-shedding builds on).

---

## 0. R1 SCOPE CORRECTION (Codex PR-REMOVAL-R1 = NEEDS-REVISION) — supersedes below

Codex R1 found a real flaw: **`PersistentObject` is a live TYPE contract, not just
a persistence-stream base.** It is the base type for:
- **object-valued `NamedProperty` values** (`PropertyManager` object properties /
  `setProperty(name, PersistentObject*)`) — this is **D49 / H6a property-break
  territory**;
- **VIEW `Stage::Material`** (representation material override is a
  `shared_ptr<PersistentObject>`);
- **PDB/crystal metadata** carried as `PersistentObject` smart-properties
  (`PDBInfo`, `crystalInfo`);
- the **Python** property bindings (`property.sip`);
- `expression.C` uses `PersistenceManager::CreateMethod` as a creation typedef;
  `Options::read/write(PersistenceManager&)` (used by `MPISupport`) — StoreFormat
  does not cover `Options`.

**Corrected scope (this milestone = the achievable kernel cleanup):**
- **REMOVE** the persistence **stream framework**: `PersistenceManager`,
  `TextPersistenceManager`, `XDRPersistenceManager`, the
  `persistentRead/Write(PersistenceManager&)` **virtuals** (76 decls), the
  `operator>>/<<(PersistenceManager&)` stream ops, the persistent-class
  registration, and `v1ToV2JsonConverter`.
- **RETAIN `PersistentObject`** as a now-minimal base type for object-valued
  properties + VIEW `Material` + PDB/crystal metadata (after the stream virtuals
  are gone it is an (near-)empty base). Provide a plain `CreateMethod`-equivalent
  typedef for `expression.C`. Keep `Options` (re-home its serialization, or keep a
  minimal value (de)serialization for MPI — see §4).
- **DEFER** the full `PersistentObject`-base ERASURE to coordinate with **H6a /
  D49** (the object-property mutable-reference break) **+ VIEW (Phase 4)** — it
  cannot be done as a standalone kernel cleanup without redesigning the
  object-property + VIEW-material contracts.
- **KEEP** the JSON `StoreFormat`; **PORT** VIEW project save/load + MPI to it.

Net: this milestone deletes the **Text/XDR object-graph streaming** (the actual
"persistence we no longer need") and its 76 virtuals, ports VIEW/MPI to JSON, and
leaves `PersistentObject`-as-a-type for the property/material/Python contracts that
H6a + VIEW will redesign. §1–§7 below are the original (pre-R1) analysis; where they
say "delete PersistentObject" / "remove the Composite base", read §0 instead.
Full R1 findings: the "## PR-REMOVAL-R1 design review" section at the end.

**R1b ADDENDUM — EXHAUSTIVE consumer enumeration (folds R1b gaps; breaks the
incremental-discovery cycle).** A full sweep finds **65 files** referencing
`PersistenceManager` (excl. the 4 framework files). The complete stream-removal
surface beyond the 76 virtuals:
- **`ObjectCreator` / `MoleculeObjectCreator`** (`source/CONCEPT/{objectCreator,
  moleculeObjectCreator}.C` + headers + `sources.cmake`) — the object-factory used
  by `readObject`. Consumers: **`source/VIEW/KERNEL/serverWidget.C`** (VIEW server)
  + **`source/APPLICATIONS/BALLVIEW/mainframe.C`**. Remove the factory; the VIEW
  server/client object exchange goes with the VIEW port (or stub-with-ledger).
- **Non-virtual `read/write(PersistenceManager&)`** (distinct from the 76 virtuals):
  `PropertyManager` (`property.h:619,622`), `Selectable` (`selectable.h:159,168`),
  `Options` (`options.C:433,456` / `options.h:353,361`), `TimeStamp`/`PreciseTime`,
  **`BitVector` (`bitVector.h`/`.C`) + `TVector3` (`vector3.h`) + `TVector2`
  (`vector2.h`)** (R1c). Remove/re-home + their tests (`BitVector_test`,
  `Vector3_test`, `Options_test`, `PropertyManager_test`, `Selectable_test` lose
  their persistence CHECKs).
- **`Options`** — `MPISupport` broadcasts it; StoreFormat doesn't cover Options →
  give Options a minimal value (de)serialise for the MPI port (or stub MPI).
- **2 `operator>>/<<(PersistenceManager&)`** stream ops; persistence registration is
  self-contained in `persistenceManager.{h,C}`.
This is now the COMPLETE surface (65 files): framework(4) + ObjectCreator(2) +
virtuals(76 decls across the layers in §1) + the 4 non-virtual read/write APIs +
2 stream ops + VIEW serverWidget/mainframe + the consumer ports (VIEW mainControl,
MPISupport) + the non-virtual `read/write(PM&)` set (PropertyManager/Selectable/
Options/TimeStamp/BitVector/TVector3/TVector2).

**DESIGN LOCKED (R1 → R1c).** R1c verdict: AGREE conditional on folding
`BitVector`/`TVector3`/`TVector2` `read/write(PM&)` into the list — **done above**, so
the stated AGREE condition is met. ObjectCreator scope, `PersistentObject`-as-type
retention, and the VIEW handling were confirmed SOUND. The design is ready for
**phased execution** (best as a focused fresh session — ~65-file surface, base-class-
adjacent care, VIEW/MPI ports). Execution order = §5 (with §0 overriding the PersistentObject-
erasure parts). (Full R1/R1b/R1c trail at the end of this doc.)

---

## 1. Scope — what is removed

**(A) Legacy PersistenceManager framework** (CONCEPT):
- `include/BALL/CONCEPT/persistenceManager.{h,iC}`, `persistentObject.h`,
  `textPersistenceManager.h`, `XDRPersistenceManager.h`
- `source/CONCEPT/persistenceManager.C`, `textPersistenceManager.C`,
  `XDRPersistenceManager.C` (+ `source/CONCEPT/sources.cmake` entries)
- the per-class `virtual void persistentRead(PersistenceManager&)` /
  `persistentWrite(PersistenceManager&) const` methods — **76 declarations**
  across CONCEPT(11) / KERNEL(26) / DATATYPE(4) / MATHS(4) / FORMAT(5) /
  VIEW(12) / XRAY(2) / SYSTEM(1).

**(B) `v1ToV2JsonConverter` only** (KERNEL): delete `v1ToV2JsonConverter.{h,C}` +
  `V1ToV2JsonConverter_test` — its INPUT is the legacy `.BAL` text persistence
  we're removing, so the converter cannot function once `TextPersistenceManager`
  is gone (v1→v2 migration would require the old BALL). The rest of the JSON
  StoreFormat **stays** (see §2).

> SCOPE REVISION: the JSON `StoreFormat` (`moleculeStoreJson`/`systemJson`/
> `propertyJson`) is **NOT removed** — it becomes the kernel's one serialization
> and the target VIEW/MPI are ported onto.

## 2. What STAYS (must keep working)
- **JSON StoreFormat** — `moleculeStoreJson.{h,C}` (`saveStoreJSON`/`loadStoreJSON`),
  `systemJson.{h,C}`, `propertyJson.{h,C}`. The kernel's sole (de)serialization;
  VIEW project save/load + MPI broadcast are re-pointed here. Tests
  `MoleculeStoreJson_test` + `SystemJson_test` **stay**.
- **FORMAT chemistry readers/writers** (PDBFile/MOL2File/HINFile/SDFile/…): they
  do **NOT** call `persistentRead/Write` or `PersistenceManager`. SAFE. (Only
  `PDBInfo`/`PDBRecords` *define* the virtuals; `PDBFileGeneral` doesn't use
  them — those two get the virtuals stripped, no behavior change.)
- **Clone / copy** (`BALL_CREATE_DEEP`/`BALL_CREATE`, `include/BALL/COMMON/create.h`):
  **independent of persistence** — they define `create()`/`createDefault()` with
  zero coupling to `persistentRead/Write`. Cloning will NOT break. (Verified.)

## 3. THE LANDMINE — `PersistentObject` is a base class
- `Composite : public PersistentObject, public Selectable` (`composite.h:86`) →
  ripples to **~35 KERNEL classes** (Atom/Bond/AtomContainer/Molecule/Chain/
  Residue/Protein/System/Fragment/Nucleotide/NucleicAcid/SecondaryStructure…).
- Also `MATHS/vector2,vector3` and `DATATYPE/bitVector,options` derive from
  `PersistentObject`.
- Removing `: public PersistentObject` is compile-safe (descendants just lose an
  inherited no-longer-referenced interface) **but** is the single largest
  coupling point. **This directly serves the H4 flip** (Atom/Bond must shed
  their bases) — which is why this milestone runs *ahead* of H4.

## 4. ⚠️ ACTIVE CONSUMERS — the real blockers (USER DECISION REQUIRED)
Removing persistence **breaks live features** that currently depend on it:

| Consumer | Use | Breakage |
|---|---|---|
| **VIEW `mainControl.C`** (~1300-1330) | BALLView **project save/load** (`System >> TextPersistenceManager`, `pm.readObject()`) — the `.bvp` scene file | BALLView can no longer save/load projects |
| **SYSTEM `MPISupport.C`** (~80-120) | `XDRPersistenceManager` broadcasts `System`+`Options` across MPI ranks | distributed/parallel MD broadcast breaks |
| **`v1ToV2JsonConverter`** | reads legacy `.BAL` (text persistence) → v2 JSON | v1.x→v2 project migration impossible |

**RESOLVED (maintainer): port these consumers onto the JSON `StoreFormat`** (which
is retained), instead of stubbing or deleting:
- **VIEW `mainControl`** save/load → replace the `System >> TextPersistenceManager`
  / `pm.readObject()` calls with `systemJson` save/load (`saveStoreJSON`/
  `loadStoreJSON` via the System's store). The `.bvp`/project format moves to JSON
  (a one-time format change to flag in release notes; BALLView project I/O is
  PRESERVED, not lost). NOTE: VIEW also persists per-Representation/Stage/Material
  *view* state via `persistentRead/Write` (12 VIEW decls) — those need either a
  small JSON view-state writer or to be carried as a separate VIEW concern; scope
  this in execution (it may extend into the VIEW/Phase-4 surface).
- **SYSTEM `MPISupport`** broadcast → serialize `System`+`Options` to a JSON
  (or the StoreFormat byte buffer) and broadcast that, instead of XDR. (If a VIEW/
  MPI port proves heavier than a kernel-cleanup milestone should carry, fall back
  to stub+ledger for THAT consumer only, flagged for its own milestone.)
- **`v1ToV2JsonConverter`** → DELETE (its `.BAL` text-persistence input is being
  removed; legacy migration needs old BALL).

> Rendering is unaffected; with the JSON port, BALLView **project save/load is
> preserved** (format changes to JSON). The residual risk is the VIEW *view-state*
> persistence (Representation/Stage/Material) — sized in execution.

## 5. Removal order (dependency-safe, refined from the sweep)
1. **Tests** — delete the 4 persistence-only tests + drop the ~18 incidental
   `CHECK(persistentRead/Write)` lines + their `#include`s; update test CMake.
2. **Delete `v1ToV2JsonConverter`** only (+ its test) — its `.BAL` text input is
   being removed. The rest of the JSON StoreFormat STAYS.
3. **Leaf virtuals** — strip `persistentRead/Write` from KERNEL(26)/DATATYPE/
   MATHS/XRAY/FORMAT(PDBInfo,PDBRecords) declarations + definitions.
4. **Port active consumers to JSON** (per §4) — VIEW `mainControl` save/load →
   `systemJson`; SYSTEM `MPISupport` broadcast → JSON/StoreFormat buffer. (Land
   the ports BEFORE deleting the framework so the features never go dark; if VIEW
   view-state persistence balloons, stub+ledger that slice only.)
5. **Composite base** — remove `: public PersistentObject` from `composite.h`
   + its `persistentRead/Write`; same for `property.h`/`selectable` if they
   carry it. Full rebuild + full ctest (rc-gated, all test binaries).
6. **Core framework** — delete the 4 CONCEPT headers + 3 sources + `.iC` +
   CMake. Grep for any stray reference (stream operators, RTTI registration).
7. **Verify** — rc-gated full rebuild + 286-N ctest green; BALLView still builds
   + renders (the core value); ledger the breaks (new Class K).

## 6. Open decisions for the gate
- **D-PR1 — RESOLVED (maintainer): keep JSON StoreFormat + port VIEW/MPI to it**
  (not stub/delete). VIEW `mainControl` save/load → `systemJson`; SYSTEM
  `MPISupport` broadcast → JSON/StoreFormat buffer; `v1ToV2JsonConverter`
  deleted. Residual to size in execution: the 12 VIEW *view-state*
  `persistentRead/Write` decls (Representation/Stage/Material) — JSON view-state
  writer vs stub-that-slice; may reach into the VIEW/Phase-4 surface.
- **D-PR2:** does the FORMAT `PDBInfo`/`PDBRecords` virtual-strip need any
  behavior preserved (they only DEFINE the virtuals; PDBFile doesn't call them)?
  → strip the virtuals only; verify PDB round-trip.
- **D-PR3:** narrowed (`BALL_COLLAPSE_KERNEL_ONLY`) build first, then full?

## 7. Gate
Codex adversarial review of THIS design (AGREE) **+** maintainer answer to D-PR1
before any code. Then phased execution (§5), each phase: rc-gated full rebuild
(all `bin/TEST/` targets) + full ctest + atomic commit + Codex step-review.
Given the size (~97 files, base-class ripple, VIEW/MPI), execution is best run
as its own focused session with this doc as the contract.

## PR-REMOVAL-R1 design review

**Verdict: NEEDS-REVISION.** The direction is right only if split into two layers:
remove the Text/XDR `PersistenceManager` stream framework and `persistentRead/Write`
methods, but do not delete/erase the `PersistentObject` type contract until the
`NamedProperty` object-value API, VIEW materials, PDB/crystal metadata, and deprecated
VIEW client/server path are explicitly redesigned. Current design under-scopes that
type-contract dependency and overstates VIEW/MPI JSON port coverage.

1. **FLAW** — Scope is not coherent as written: `systemJson`/`moleculeStoreJson`
   avoid `PersistenceManager`, but `propertyJson` includes `CONCEPT/property.h`,
   where `NamedProperty : PersistentObject` and OBJECT/SMART_OBJECT properties use
   `PersistentObject*`/`boost::shared_ptr<PersistentObject>`; deleting
   `persistentObject.h` breaks the kept JSON path unless the property object API is
   refactored or a non-persistence base is retained.
2. **FLAW** — The `Composite : PersistentObject` removal is not just “unused
   inherited interface”: `Client::insert` tests `RTTI::isKindOf<PersistentObject>`
   and streams composites, `ObjectCreator`/`MoleculeObjectCreator` read
   `PersistentObject*`, and property users rely on `PersistentObject` smart pointers
   for `Stage::Material`, `PDBInfo`, and `CrystalInfo`; also `Options` is not
   actually a `PersistentObject` subclass here, it only has `write/read(PersistenceManager&)`.
3. **FLAW** — VIEW project save/load cannot be declared a clean `systemJson` swap:
   `systemJson` documents state `System -> Molecules -> Atoms` only and explicitly
   exclude Chain/Residue/Fragment/SecondaryStructure; `.bvp` also stores multiple
   systems plus representation settings, and `Representation::getSettings` /
   `DisplayProperties::getSettingsFromString` still serialize `NamedProperty`
   material state via `TextPersistenceManager`, so deferring VIEW state blocks
   framework deletion unless that state is ported or deliberately removed.
4. **WEAK** — MPI `System` broadcast can plausibly use JSON bytes, subject to the
   same hierarchy/property-loss caveats as `systemJson`; MPI `Options` cannot use
   StoreFormat because Options is not store-backed and no JSON helper exists, so
   execution must add an Options JSON/text-buffer codec or explicitly stub and ledger
   MPI as a removed/deferred feature.
5. **FLAW** — “Port consumers before deleting framework” is the right order, but
   the consumer list is incomplete: in addition to mainControl/MPISupport/converter/
   VIEW view-state, there are live or build-visible dependencies in VIEW client/server,
   CONCEPT object creators, `NamedProperty` object persistence, PDB/crystal metadata
   properties, Python SIP property bindings, and `Expression`’s use of
   `PersistenceManager::CreateMethod` as a factory typedef.
6. **SOUND** — `BALL_CREATE`/`BALL_CREATE_DEEP` in `include/BALL/COMMON/create.h`
   are plain virtual construction/copy macros using constructors and `createDefault`;
   they do not route through persistence.
7. **WEAK** — Running ahead of H4 is still desirable for the Atom/Bond base cleanup,
   but only after the milestone is narrowed: remove stream persistence first, retain
   or rename the object-property base until H4/property work can replace it, and make
   VIEW project-state/MPI Options decisions explicit.

Missed consumers / landmines found by grep:
- `source/CONCEPT/client.C`, `include/BALL/CONCEPT/client.h`: deprecated client
  sends `Composite` through `TextPersistenceManager` and requires
  `PersistentObject`.
- `source/VIEW/KERNEL/serverWidget.C`, `include/BALL/VIEW/KERNEL/serverWidget.h`,
  `source/CONCEPT/objectCreator.C`, `source/CONCEPT/moleculeObjectCreator.C`:
  VIEW server receives `PersistentObject` streams and converts them to `Composite`.
- `include/BALL/CONCEPT/property.h`, `include/BALL/CONCEPT/property.iC`,
  `source/CONCEPT/property.C`: `NamedProperty` derives from `PersistentObject` and
  stores raw/smart `PersistentObject` object values.
- `source/VIEW/KERNEL/representation.C` and `source/VIEW/DIALOGS/displayProperties.C`:
  representation settings encode/decode `NamedProperty` blobs with
  `TextPersistenceManager`.
- `source/VIEW/WIDGETS/scene.C`, `source/VIEW/DIALOGS/materialSettings.C`,
  `source/VIEW/RENDERING/RENDERERS/{glRenderer,XML3DRenderer,rtfactRenderer}.C`:
  representation material override is a `boost::shared_ptr<PersistentObject>` to
  `Stage::Material`.
- `include/BALL/VIEW/KERNEL/stage.h`, `source/VIEW/KERNEL/stage.C`:
  `Stage::Material` remains a `PersistentObject` for that material property path.
- `include/BALL/FORMAT/PDBInfo.h`, `include/BALL/FORMAT/PDBRecords.h`,
  `include/BALL/XRAY/crystalInfo.h`, `source/FORMAT/PDBFileDetails.C`,
  `source/XRAY/crystalGenerator.C`, VIEW property editor/generate-crystal dialogs:
  PDB/crystal metadata is carried as `PersistentObject` smart properties even if
  chemistry file I/O does not call `PersistenceManager`.
- `source/SYSTEM/MPISupport.C`: `Options` currently uses
  `Options::write/read(PersistenceManager&)`; StoreFormat does not cover it.
- `source/KERNEL/expression.C`: uses `PersistenceManager::CreateMethod` as a
  creation-function typedef, independent of stream persistence but broken by deleting
  the class without replacement.
- `source/PYTHON/EXTENSIONS/BALL/property.sip`: Python property bindings expose
  `PersistentObject` object properties and `PropertyManager::setProperty`.

## PR-REMOVAL-R1b confirmation

**Verdict: NEEDS-REVISION.** The R1 type-contract blocker is resolved, but the
corrected execution scope still needs an explicit dangling-stream-site cleanup list
before implementation.

1. **ADDRESSED** — Retaining `PersistentObject` as a minimal `Object`-derived base
keeps object-valued `NamedProperty`, VIEW `Stage::Material`, PDB/crystal metadata,
and Python property bindings on a valid non-abstract base once the stream virtuals
and `operator>>(PersistenceManager&)` are removed.
2. **ADDRESSED** — Deferring full `PersistentObject` erasure to H6a/D49 + VIEW is
the right coupling; D49/H6 is the planned object-property mutable-reference/API
break, and VIEW owns the material/server project-state contract.
3. **PARTIAL** — `CreateMethod` and `Options` are called out, but the dangling-site
coverage is still incomplete: `ObjectCreator`/`MoleculeObjectCreator` and the VIEW
server/client path are hard `TextPersistenceManager` consumers, and non-virtual
`read/write(PersistenceManager&)` APIs on `PropertyManager`, `Selectable`,
`PreciseTime`/`TimeStamp`, plus their tests, must be removed, re-homed, or explicitly
stubbed along with `Options`.
4. **NOT** — Remaining blocker: fold those missed non-virtual and client/server
stream consumers into the phase order; after that, the revised “remove stream
framework, retain `PersistentObject` type, keep JSON StoreFormat” design can proceed.

## PR-REMOVAL-R1c confirmation

**Verdict: NEEDS-REVISION.** The corrected architecture is coherent, but the
claimed exhaustive stream-removal surface still misses two live
`PersistenceManager` read/write API families; deleting the framework after only
the listed work would leave dangling references.

1. **PARTIAL** — `ObjectCreator`/`MoleculeObjectCreator` now appear fully scoped
to `serverWidget` + `BALLVIEW mainframe`, and the named
`PropertyManager`/`Selectable` callers are test-only; however the non-virtual
surface is still not complete because `BitVector::read/write(PersistenceManager&)`
and `TVector3::read/write(PersistenceManager&)` remain live declarations,
definitions, and tests (`BitVector_test2.C`, `Vector3_test.C`).
2. **ADDRESSED** — Retaining `PersistentObject` as a minimal type while removing
`PropertyManager`/`Selectable` PM read/write is coherent: those classes can keep
the base/value-type contract for object properties while dropping only the stream
serialization methods.
3. **ADDRESSED** — VIEW handling is sound in shape: `mainControl`/MPI must be
ported to JSON/value serialization, while `serverWidget` object exchange and VIEW
view-state stream virtuals are either ported or stubbed with a ledger as a VIEW
slice, so the kernel stream framework is not intrinsically blocked by VIEW once
that slice is explicit.

Remaining blocker: fold `DATATYPE/bitVector` and `MATHS/vector3` PM
`read/write` APIs and tests into the exhaustive removal list/phase plan. With that
addition, this would move to AGREE.

## PR3a-CR code review

**Verdict: GO.** The phase-3a kernel leaf virtual strip is clean: no leaf
`persistentRead`/`persistentWrite` declaration/definition mismatch remains, no brace
imbalance was introduced in the touched KERNEL sources, `Bond::finalize()` and its
store-mirror helper remain intact, and the retained phase-3b/3c/5/6 persistence
surfaces still exist.

1. **SOUND** — Header/source removal took the intended leaf persistence virtuals:
the 13 KERNEL leaf/header pairs plus `Bond` no longer declare or define
`persistentRead`/`persistentWrite`; no adjacent non-persistence functions were
removed and `git diff --check` is clean.
2. **SOUND** — `Bond::finalize()` is still declared in `bond.h`, defined inline in
`bond.iC`, and still calls `finalize_storeMirror_()`; no remaining source/test call
to `Bond::persistentRead` was found.
3. **SOUND** — `Composite`/`PropertyManager`/`Selectable`/`PersistentObject`, FORMAT
`PDBInfo`/`PDBRecords`, XRAY `CrystalInfo`, MATHS `TVector2`, and the
`read/write(PersistenceManager&)` family are untouched; `PDBInfo.C` still calls the
kept `PDBRecords::persistentRead/Write` pair.
4. **WEAK** — The stripped KERNEL test sections are structurally clean
(`START_TEST`/`END_TEST` and `CHECK`/`RESULT` counts balance, no dangling persistence
manager references remain), but `test/NamedProperty_test.C` is also changed even
though it is outside the stated 13 kernel-test list. This is non-blocking because the
rebuilt test passes and `PropertyManager_test.C` still covers the retained
NamedProperty/PropertyManager stream path, but the phase note should explicitly
account for it.
5. **SOUND** — Compile behavior is safe for this phase: the leaf classes now inherit
the still-present base virtuals until phase 5/6, so there is no ODR/vtable hole from
removing the overrides. Verified `cmake --build build-core -j2` and rebuilt the
affected `build-core` test binaries.
6. **SOUND** — No new dangling include/use site was found in the touched KERNEL
files or stripped tests; rebuilt affected `build-core` tests
`NamedProperty_test`, `Atom_test2`, `Bond_test`, `AtomContainer_test1`,
`Fragment_test`, `Chain_test`, `Molecule_test`, `SecondaryStructure_test`,
`System_test`, `Protein_test`, `PDBAtom_test`, `NucleicAcid_test`, and
`Nucleotide_test` all pass; full-build `Residue_test2` also passes.

## PR3b-CR code review

**Verdict: GO.** The phase-3b FORMAT/XRAY/MATHS leaf persistence strip is clean:
the intended declarations and definitions are removed as pairs, no remaining caller
targets the stripped leaf overrides, and PDB read/write still uses the normal PDBFile
record path rather than these persistence virtuals.

1. **SOUND** — Header removal is limited to the `Storable Interface`/`Persistence`
blocks, including the commented-out `read/write(PersistenceManager&)` lines; the
source removal is exactly the two `persistentRead`/`persistentWrite` definitions
each for `PDBInfo`, `PDBRecords`, and `CrystalInfo`, with no brace bleed and
`git diff --check` clean.
2. **SOUND** — The `PDBInfo` to `PDBRecords` pair was removed consistently:
`PDBInfo::persistentRead/Write` and `PDBRecords::persistentRead/Write` are both
gone, so the former no longer contains calls to removed member methods; no other
`PDBInfo`/`PDBRecords` persistence caller was found.
3. **SOUND** — `TVector2` has no orphaned template line or dangling out-of-class
definition after the inline persistence methods were removed; inheriting the still
present `PersistentObject` base virtuals is ABI/ODR-coherent for this phase.
4. **SOUND** — PDB round-trip behavior should not change: `PDBFile` accesses
`PDBInfo`/`PDBRecords` through their normal record/container APIs, not through
`persistentRead/Write`, and the stripped methods were only persistence-stream
virtual implementations.
5. **SOUND** — No stray `persistentRead/Write` references remain in the touched
headers/sources or `Vector2_test.C`; the only leftovers are now-unused
`#include <BALL/CONCEPT/persistenceManager.h>` lines in `PDBInfo.h`,
`PDBRecords.h`, `crystalInfo.h`, and `vector2.h`, which are harmless because they
still provide `PersistentObject` transitively but can be replaced with
`persistentObject.h` in a cleanup.
6. **SOUND** — No new issue found. The uncommitted `Vector2_test.C` change is
related and necessary: it removes the persistence-stream test for the stripped
`TVector2` overrides while preserving the later temporary filename used by the
normal iostream test.
