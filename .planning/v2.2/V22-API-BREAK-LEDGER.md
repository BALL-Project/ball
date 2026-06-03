# BALL v2.2 — API Break Ledger

**Status:** seeded at H1a (per D61 + R31 follow-up #4).
**Authored:** 2026-05-20.
**Companion:** `V22-DECISIONS.md` (D61), `V22-H0-ADDENDUM.md`,
`V22-ARCH-HANDLE-MODEL.md`.

This is the single enumerated list of every public-API break the v2.2
handle redesign introduces. Per **D50/D61** every entry needs a Codex
sign-off at the W2/W3/H4 break commits. Entries are **seeded now** (no
break has landed yet — H1a is a non-invasive store-side prototype with
dual existence; nothing here breaks until H4 the flip). Each row will be
moved from PLANNED → LANDED with the commit that performs it.

The breaks are organised by R31's break classes (P31-5): not just the
simple return-type change, but every idiom that depends on raw-pointer
object identity or lifetime.

---

## Class A — pointer return types become value handles

The dominant break. Every accessor that returned a raw
`Atom*`/`Bond*`/`Molecule*`/`Chain*`/`Residue*`/`Protein*`/
`SecondaryStructure*`/`Nucleotide*`/`NucleicAcid*`/`Fragment*`/
`AtomContainer*`/`Composite*` to an object in the molecular hierarchy
returns the corresponding **value handle** instead. A `nullptr` result
becomes a **null handle** (`{store=nullptr}`, `bool`-testable via
`operator bool`, pointer-like member access via `operator->`).

| Symbol (representative) | Old return | New return | Phase | Status |
|---|---|---|---|---|
| `Atom::getResidue()` / `getChain()` / `getMolecule()` / `getProtein()` | `Residue*`/… | `Residue`/… value handle | H3 | PLANNED |
| `Composite::getParent()` (molecular) | `Composite*` | container value handle | H3 | PLANNED |
| `AtomContainer::getAtom(Position)` / `getAtom(name)` | `Atom*` | `Atom` | H3 | PLANNED |
| `AtomContainer::getAtomContainer` / `getSuperAtomContainer` | `AtomContainer*` | `AtomContainer` handle | H3 | PLANNED |
| `Protein::getResidueByID` / `Chain::getNTerminal` / `getCTerminal` | `Residue*` | `Residue` | H3 | PLANNED |
| `Residue::getPDBAtom(name)` | `PDBAtom*` | `Atom`/`PDBAtom` handle | H3 | PLANNED |
| `System::getMolecule(Position)` and the typed `getProtein`/… | `Molecule*`/… | value handle | H3 | PLANNED |

**Migration note:** `if (Residue* r = a.getResidue()) r->getName();`
→ `if (Residue r = a.getResidue()) r->getName();` compiles unchanged in
body because the handle is `bool`-testable and exposes `operator->`.

## Class B — pointer stored in containers / as members

Code that **stores** `Atom*`/`Residue*`/… in an `std::vector`,
`std::list`, `HashMap`, or as a class member field. These do not
auto-migrate: the stored type must change to the value handle, and any
code relying on the stored pointer's stability across store
reallocation must be re-checked against the handle generation contract.

| Pattern | Migration | Phase | Status |
|---|---|---|---|
| `std::vector<Atom*>` member / local | `std::vector<Atom>` (handles) | H3 | PLANNED |
| `HashMap<Atom*, T>` keyed by atom pointer | key by `Atom` handle or `stable_id` | H3 | PLANNED |
| `Residue*` cached in a class field | store `Residue` handle | H3 | PLANNED |

## Class C — pointer identity comparison / ordering

`a == b` / `a < b` / `std::set<Atom*>` / ordered map keyed on pointer
value. Handles compare by `{store, idx, generation}`; pointer **ordering**
(address order) has no handle equivalent and must move to `stable_id`
ordering or an explicit comparator.

| Pattern | Migration | Phase | Status |
|---|---|---|---|
| `atom_a == atom_b` (identity) | handle `operator==` (store+idx+gen) | H3 | PLANNED |
| `std::set<Atom*>` / `std::map<Atom*,…>` | order by `stable_id()` | H3 | PLANNED |
| pointer-difference / `<` ordering | `stable_id()` comparison | H3 | PLANNED |

## Class D — `nullptr` overload resolution / sentinel use

Call sites passing `0`/`nullptr` where a pointer was expected, or
overload sets distinguishing `T*` from other types, break when the
parameter becomes a value handle. Null is now the default-constructed
null handle.

| Pattern | Migration | Phase | Status |
|---|---|---|---|
| `f(Atom* = nullptr)` default arg | `f(Atom = Atom())` null handle | H3 | PLANNED |
| `x == nullptr` on a returned entity | `!x` (handle `operator bool`) | H3 | PLANNED |

## Class E — functions taking `Composite*` / `AtomContainer*` params

Signatures that **accept** a molecular `Composite*`/`AtomContainer*` —
including `insert(Composite&)`, processors, predicates, visitors. These
re-express on handles + the handle-yielding traversal layer (H2).

| Symbol class | Migration | Phase | Status |
|---|---|---|---|
| `UnaryProcessor<Atom>::operator()(Atom&)` | `operator()(Atom)` by value | H3 | PLANNED |
| `insertBefore(Composite&)` / `insertAfter` | handle-taking overloads | H3 | PLANNED |
| `UnaryPredicate<Atom>` / `Expression` / `Selector` | take `Atom` value | H3 | PLANNED |

## Class F — `dynamic_cast` / RTTI flows

`dynamic_cast<Atom*>(Composite*)` and the whole "is this Composite an
Atom?" question **dissolve** under A2 (no molecular Composite tree).
`detail::compositeAsAtom_(Composite*)` is **removed** (premise gone).
Kind discrimination reads the container row's `kind` tag (tagged-union /
visitor), not RTTI.

| Pattern | Migration | Phase | Status |
|---|---|---|---|
| `dynamic_cast<Atom*>(c)` | removed; `kind` tag on row | H4 | PLANNED |
| `detail::compositeAsAtom_` | removed | H4 | PLANNED |
| `RTTI::isKindOf<Residue>(c)` (molecular) | `row.kind == ContainerKind::RESIDUE` | H3 | PLANNED |
| 54 VIEW RTTI sites | handle kind tag | H7 | PLANNED |

## Class G — address-taking / lifetime / delete of returned entity

Code that takes the address of an iterator result, holds a reference
across a store mutation, or `delete`s a returned object pointer.

| Pattern | Migration | Phase | Status |
|---|---|---|---|
| `&(*atom_iterator)` → `Atom*` | hold the `Atom` handle by value | H3 | PLANNED |
| `delete returned_ptr` | no-op; lifetime owned by store | H4 | PLANNED |
| `Atom& a = *it;` held across mutation | re-fetch handle (generation guard) | H3 | PLANNED |

## Class H — property mutable-reference surface (D49)

A column store cannot hand out a live mutable reference into former
inline state.

| Symbol | Old | New | Phase | Status |
|---|---|---|---|---|
| `NamedProperty& getNamedProperty(Position)` | mutable ref | `eachProperty(visitor)` / value getters | H6 | PLANNED |
| `NamedPropertyIterator beginNamedProperty()` | vector iterator | `propertyNames()` + value getters | H6 | PLANNED |
| `BitVector& ...` | mutable ref | visitor / value accessor | H6 | PLANNED |

## Class I — removed inline state / timestamps

| Symbol | Disposition | Phase | Status |
|---|---|---|---|
| per-`Composite` `modification_stamp_` / `selection_stamp_` | per-System stamp; per-container `selection_stamp` column only if a consumer needs it (D59) | H6 | PLANNED |
| `Composite*` implicit upcast from `Atom`/container | gone (no inheritance) | H4 | PLANNED |
| `Element` pointer identity (already lost in v2.0) | value/index identity | — | LANDED (v2.0) |

## Class J — default-value change: atom type-name "?" → "" (HCP-1P.A)

A **behavioral / output** break (not structural), landed in the HCP-1P kernel
object-creation fast-path. `BALL_ATOM_DEFAULT_TYPE_NAME` changed `"?"` → `""`, so a
default/unassigned atom's type-name is now the empty string (= reserved string-pool
offset 0, no per-atom interning). Maintainer-directed (perf/storage + simpler
"unset = empty" semantics).

| Symbol | Old | New | Phase | Status |
|---|---|---|---|---|
| `BALL_ATOM_DEFAULT_TYPE_NAME` | `"?"` | `""` | HCP-1P.A | LANDED |
| `Atom().getTypeName()` (fresh/default atom) | `"?"` | `""` | HCP-1P.A | LANDED |
| text/persistence dump `type name:` field for untyped atoms | `?` | (empty) | HCP-1P.A | LANDED |
| Antechamber `.ac` type column (`%10s`) for untyped atoms | `         ?` | `          ` (10 spaces) | HCP-1P.A | LANDED |

**Consistency preserved:** the type-assignment sentinel checks
(`MOLMEC/COMMON/assignTypes.C`, `MOLMEC/PARAMETER/templates.C`,
`STRUCTURE/atomTyper.C`) compare against the `BALL_ATOM_DEFAULT_TYPE_NAME` *macro*,
not a literal `"?"`, so "is this atom untyped?" logic is unchanged. FORMAT readers
that historically used `"?"` have been audited; `HINFile` canonicalises the missing
`**` token to `BALL_ATOM_DEFAULT_TYPE_NAME` and still treats `""`/`"?"` identically
on the write-side unset path.

**Migration note:** downstream code/tooling that parsed BALL text/Antechamber output
expecting the literal `"?"` placeholder for untyped atoms must accept an empty field.
Reference test data updated: `AtomContainer_test.txt`, `Molecule_test.txt`,
`Nucleotide_test.txt`, `Residue_test.txt`, `Fragment_test.txt`, `AntechamberFile_test3.ac`.

---

## Class K — persistence STREAM framework removal (PR-removal milestone)

The legacy object-stream persistence framework is **removed**. The JSON
`StoreFormat` (`systemJson`/`propertyJson`) is **retained** and unaffected.
`PersistentObject` is **retained** as a near-empty base TYPE (ctor/dtor/`finalize()`);
`Composite : public PersistentObject` inheritance is **kept** (base-erasure deferred
to H6a/D49 + VIEW). This phase removed the framework files + all `persistentRead/Write`
virtuals + the `read/write(PersistenceManager&)` family + `operator>>/<<(PersistentObject&, PM&)`.

**Removed headers/sources (13):**
`CONCEPT/{persistenceManager.h,.iC, textPersistenceManager.h, XDRPersistenceManager.h,
objectCreator.h, moleculeObjectCreator.h}` + matching `.C`; plus **`CONCEPT/client.{h,C}`**
(see below).

| Symbol / API | Old | New | Status |
|---|---|---|---|
| `PersistenceManager`, `TextPersistenceManager`, `XDRPersistenceManager` | classes | **removed** | LANDED |
| `ObjectCreator`, `MoleculeObjectCreator` | classes | **removed** | LANDED |
| `*::persistentRead/persistentWrite(PersistenceManager&)` (76 virtuals) | virtual | **removed** | LANDED |
| `*::read/write(PersistenceManager&)` (BitVector/Options/TVector2/TVector3/TimeStamp/PreciseTime/PropertyManager/Selectable/NamedProperty/PDBInfo/PDBRecords/crystalInfo) | non-virtual | **removed** | LANDED |
| `operator>>/<<(PersistentObject&, PersistenceManager&)` | global | **removed** | LANDED |
| `PersistentObject` persistence virtuals | virtual | reduced to `finalize()` only | LANDED |

**Class K-CS — client-server-over-persistence-stream feature (collateral, deferred):**
`CONCEPT/Client` (libBALL) serialised a `Composite` through a `TextPersistenceManager`
member (`composite >> pm_`) onto a socket; it cannot survive stream removal. `Client`
had **no kernel-build consumers and no test**; its only users are
`VIEW/KERNEL/serverWidget.{h,C}` (the receiving `Server`, already out of the kernel-only
build) and the `APPLICATIONS/UTILITIES/BALLVIEWClient` utility. `client.{h,C}` are
therefore **removed from libBALL**; the "push molecule to a running BALLView over TCP"
feature is **dormant** and slated for rework over a current transport (JSON `StoreFormat`
or a new protocol) in the VIEW / networking phase (Phase 4a+).

**Out-of-kernel-build breaks deferred to the VIEW phase (do NOT compile in `BALL_CORE_ONLY`):**
`SYSTEM/MPISupport.C` (`BALL_HAS_MPI`; used XDR/Text PM + `Options::read/write(PM&)`),
`VIEW/KERNEL/serverWidget.C`, `VIEW/KERNEL/stage.C`/`mainControl.C`/`representation.C`,
`VIEW/DIALOGS/displayProperties.C`, `APPLICATIONS/BALLVIEW/mainframe.C`,
`APPLICATIONS/UTILITIES/BALLVIEWClient.C`, and the Python binding
`PYTHON/EXTENSIONS/BALL/client.sip` (wraps the removed `Client` class; gated off by
`BALL_PYTHON_SUPPORT`, which is disabled in this build) — all reference removed
PM/ObjectCreator/Client symbols and must be reworked or excised when VIEW / the Python
bindings are reactivated.

**Include-hygiene fallout (self-containment, no API change):** `persistenceManager.h` was
a transitive-include hub that silently provided `<iomanip>`, `<fstream>`, `String`,
`HashMap`, `HashSet`, `StringHashMap`, `<BALL/COMMON/global.h>` (`BALL_EXPORT`) and
`PersistentObject` to a large swath of CONCEPT/MATHS/FORMAT/STRUCTURE/DOCKING/QSAR
headers. Its removal exposed ~40 latent missing-include bugs (incl. a pre-existing
wrong-macro guard on `STRUCTURE/reducedSurface.h`'s `hashMap.h` include); all fixed by
adding the genuinely-used includes directly. No public symbol changed.

---

## Class L — HCP-2 `ContainerKind` enum shrink (planned, not yet landed)

At the HCP-2 commit that collapses the typed kernel hierarchy, `enum class
ContainerKind` (in `include/BALL/KERNEL/containerKind.h`) drops to two
values: `{NONE, MOLECULE, FRAGMENT}`. The obsolete arms (`PROTEIN`,
`NUCLEIC_ACID`, `CHAIN`, `RESIDUE`, `SECONDARY_STRUCTURE`, `NUCLEOTIDE`)
disappear from the enum.

Sites that name these enum constants STOP COMPILING and must be
hand-edited in the same commit. Inventory (search: `ContainerKind::(PROTEIN|NUCLEIC_ACID|CHAIN|RESIDUE|SECONDARY_STRUCTURE|NUCLEOTIDE)`):

- `include/BALL/KERNEL/containerHandle.h` — `BALL_DECLARE_LEGACY_HANDLE`
  invocations for `ProteinHandle`, `NucleicAcidHandle`, etc. These typed
  aliases collapse to `MoleculeHandle` post-shrink and the macro arms
  vanish with them.
- `include/BALL/KERNEL/extractorsHandle.h` — `detail::isMoleculeKind_` /
  `detail::isFragmentKind_` predicate group sets (H3b.4 / H3b-CR).
  Post-shrink each reduces to a single `==` check.
- `source/KERNEL/system.C::kindOfContainer_` — v0 dispatch table.

Treated as Class L (planned mechanical edits, not silent breakage):
deliberately INTENDED hard-fails at the shrink commit so every caller is
caught and updated in one go, not over time. Codex H3b close-review FLAW 2.

---

## Sign-off log

| Commit | Break classes landed | Codex round | Verdict |
|---|---|---|---|
| (none yet — H1a is dual-existence, non-invasive) | — | — | — |

## Notes

- H1a–H3 run under **dual existence** (D60): the v0 object tree stays
  the source of truth and still hands out raw pointers; the store mirror
  is verified by `HierarchyParity_test`. **No Class A–I break lands
  before H4.** This ledger is the pre-flip enumeration R31 asked for so
  reviewers can separate intended v2.2 scope from accidental erosion.
- The Chain `id`-vs-`name` question (R31 follow-up #2) is **resolved**:
  `Chain` has no own `id_` field — it uses the inherited
  `AtomContainer::name_`. No Chain-specific payload string; no break.
