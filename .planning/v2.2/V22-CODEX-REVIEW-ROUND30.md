# Track A v2.2 Cluster R30 — Codex CLI Round 30 (2026-05-20)

**Status:** Complete
**Verdict:** NEEDS-FIXES
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.2 design re-review -- option A + A2 flat SoA topology

## Probe answers

**P30-1. A2 resolves R29's core hole, but by accepting a larger break.** R29's blocker was exact: a side-table node cannot be passed as a C++ `Composite&`, and current traversal is built on `Composite*`, `dynamic_cast<T*>`, and `UnaryProcessor<T>(T&)` (`include/BALL/CONCEPT/composite.h` lines 1244-1297, 1807-1822, 2052-2076). A2 genuinely dissolves that hole because atoms and molecular containers are no longer pretending to be `Composite` objects.

The cost is larger than A1. Under A1, the main incompatible class was atom-as-leaf. Under A2, every `Molecule*`, `Chain*`, `Residue*`, `Protein*`, `AtomContainer*`, `Composite& before/after`, and typed iterator consumer is also invalidated. The architecture doc acknowledges "largest consumer break", but it still understates how much current code is pointer/object-shaped: `AtomContainer::getAtom`, `getAtomContainer`, `getSuperAtomContainer`, insertion-before-`Composite&`, and subcontainer mutation are all object APIs (`include/BALL/KERNEL/atomContainer.h` lines 162-220 and 247-334).

**P30-2. Container-specific state is flattenable in principle, but the metadata-row schema is underspecified.** The checked headers do not show object-shaped state that is impossible to store in rows. The state is mostly scalar/string/tag/property plus relationships: `AtomContainer::name_` (`atomContainer.h` lines 152-160 and 401-405), `Residue::id_` and `insertion_code_` (`residue.h` lines 271-302 and 450+), `Protein::id_` (`protein.h` lines 229-237 and 309+), `SecondaryStructure::type_` (`secondaryStructure.h` lines 122-126 and 362-365). Methods such as residue torsions and terminal predicates can be handle methods querying atoms, bonds, properties, and ordered siblings (`residue.h` lines 177-219 and 361-386).

But the design only says `{ kind, parent_container_idx, name, type-specific fields, generation }`. That is not rich enough as a spec. It must define a tagged payload for each kind, property storage for containers, persistence mapping, and replacement return types for all pointer APIs. It also must decide whether methods like `Protein::getResidueByID`, `Chain::getNTerminal`, and `Residue::getPDBAtom` return nullable handles, optionals, or pointer-compatible proxy references (`protein.h` lines 173-213, `chain.h` lines 172-227, `residue.h` lines 257-339).

**P30-3. Preorder can be reconstructed, but deterministic order is not specified.** Current `Composite::apply*` walks `first_child_` / `next_` in preorder and postorder (`composite.h` lines 1790-2142). CSR by `parent_container_idx` can reconstruct a traversal if every parent has ordered child-container and direct-atom child lists. A grouping CSR built only by parent id is not enough if it sorts by container index or atom slot index, because store free-list reuse means slot order is not insertion/tree order.

A2 needs an explicit order column or ordered edge table: for each container row, a deterministic child sequence containing both child containers and direct atoms. If grouping CSR is rebuilt from `(parent, ordinal, child_kind, child_idx)`, it can match today's tree order. If it is rebuilt from parent ids alone, it will be deterministic only accidentally and may break PDB/naming/terminal-residue semantics.

**P30-4. System-only ownership has an unresolved construction-before-insertion lifecycle.** Today detached objects are common and tested: create `Molecule`, `Chain`, `Residue`, insert atoms/children, then later `System::insert`. The current code even has an orphan store for atoms and migration from orphan to system (`source/KERNEL/system.C` lines 37-67, 89-120, 338-361). A2 deletes the container object state, so a detached `Molecule` handle needs somewhere for its container row, child rows, properties, and atom parent links to live before a `System` owns it.

The architecture does not specify this. It needs either a container orphan store parallel to the atom orphan store, an explicit builder/topology object that owns detached hierarchy data, or a breaking rule that containers cannot exist outside a `System`. The last option would break a lot of construction patterns, including file readers and tests using `new Molecule; sys.insert(*m)` or `mol.insert(atom); sys.insert(mol)`.

**P30-5. Dual existence for the full hierarchy is the biggest incremental-risk gap.** H1-H3 says old objects and new handles coexist, then H4 deletes the object tree. For atoms-only A1, that is already hard but bounded. For A2, every container state mutation must dual-write: object fields, `Composite` links, container metadata rows, child ordering, parent links, properties, selection counters, and persistence. The current `System` still owns an object tree and store side by side (`system.C` lines 15-35 and 338-361), and `System::getMolecule` / counts still return object pointers via iterators (`system.C` lines 380-449).

This can be made incremental only if H1 declares exactly which source of truth wins on every mutation and has invariant checks comparing object tree vs container table. Without that, H1-H3 are likely to diverge silently. The A2 dual layer is not "store-backed reads alongside objects"; it is a complete mirrored hierarchy.

**P30-6. Scope phasing is not executable enough.** H1-H8 is directionally plausible, but H3 is too large: PDB/naming/residue semantics plus full container-object deletion is a cluster of clusters. Before H1, split A2 into smaller design/implementation gates: container row schema, ordered child edge/CSR, detached construction/orphan topology, handle iterators, mutation parity, persistence, then module migrations.

The pragmatic smaller first milestone remains A1: atom/bond handles with container objects. That captures the D13 atom-memory win and de-risks handle storage/traversal before flattening the whole hierarchy. If the maintainer still wants A2-now, H1 must be narrowed to a non-invasive prototype of container rows and ordering with parity tests, not broad dual existence across all containers.

**P30-7. A2 reintroduces R29-class blockers if the new handle APIs are not specified up front.** Known high-risk surfaces:

- `AtomContainer::getAtom(name)` and `Protein::getResidueByID` need indexed or specified linear lookups over stable child order (`atomContainer.h` lines 206-220, `protein.h` lines 173-185).
- Kernel iterators are all `CompositeIteratorTraits`-based today; typed iterators for atom, molecule, chain, residue, secondary structure, fragment, nucleotide, nucleic acid, and PDBAtom all inherit the old traversal model.
- `Composite::apply<T>` cannot be partly retained for molecular handles; it is template code with direct `dynamic_cast<T*>` and inline child pointer traversal.
- Selection propagation must move off `Composite` counters into store-side subtree aggregates or a container-counter table. The current design says selection counters exist but not where under A2.
- JSON/persistence must serialize the container metadata table and ordered child edges, not just atoms and bonds.

**P30-8. Overall verdict: NEEDS-FIXES.** A2 is coherent as an end-state direction. It does answer R29 better than the side-table Composite node design. It is not design-locked enough to start H1 because several required contracts are missing.

## New findings

1. **BLOCKER: no detached-container storage model.** A2 has System-owned stores and store-backed container handles, but current BALL supports detached container construction and later insertion. The atom orphan-store precedent exists; the container equivalent is missing.

2. **BLOCKER: child order is not a derived property of `parent_container_idx`.** Current behavior depends on insertion order and mixed child order. A2 needs an ordered edge table or ordinal columns for child containers and direct atoms before `apply`, iterators, terminal residue detection, and PDB round-tripping can be correct.

3. **HIGH: "container metadata row" is too vague for current container APIs.** The headers show type-specific IDs, insertion codes, secondary-structure type, name, property predicates, terminal predicates, torsions, subcontainer counts, and PDBAtom-specific access. These are implementable on handles, but not from the current schema sketch.

4. **HIGH: H1-H3 dual existence is under-specified.** Full hierarchy mirroring requires bidirectional parity on every insert/remove/splice/swap/clear/copy/persistentRead path. The design needs a source-of-truth rule and invariant tests before implementation.

5. **HIGH: pointer-returning APIs need an explicit break policy.** Returning `Residue*` / `Chain*` / `AtomContainer*` no longer makes sense for value handles unless proxy objects are introduced, which would partially recreate the object identity problem. The ledger must list these breaks before H1.

6. **MEDIUM: selection and timestamps need an A2 storage contract.** Current `Composite` owns selection propagation and modification/selection timestamps. A2 deletes those objects for molecular hierarchy, so equivalent container-store fields or a deliberate API break are required.

7. **MEDIUM: persistence must include topology, not just payload.** A2 JSON/persistent IO needs container rows, type payloads, ordered child edges, atom parent ids, generation policy, and validation against cycles/orphans.

## Overall verdict

**NEEDS-FIXES before H1.** A2 resolves R29's exact conceptual hole: if the molecular hierarchy is no longer a C++ `Composite` tree, there is no false claim that a side-table node is a `Composite&`. That is the right architectural answer to R29.

The design is still not buildable incrementally as written. The missing contracts are not minor implementation details; they define the representation: detached construction, child ordering, container-row payload schema, dual-write parity, pointer API breaks, selection/timestamp storage, and persistence. Lock those in a short H0 addendum before starting H1. If schedule or risk tolerance matters, do A1 first and move A2 to a follow-on milestone; if A2 remains mandatory now, split H1 into a prototype/parity gate rather than beginning broad hierarchy migration.
