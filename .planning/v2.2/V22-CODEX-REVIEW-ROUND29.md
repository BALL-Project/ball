# Track A v2.2 Cluster R29 — Codex CLI Round 29 (2026-05-20)

**Status:** Complete
**Verdict:** NO-GO
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.2 W0 design lock (D45-D51, thin-handle flip)

## Probe answers

**P29-1. D45 teardown is not yet sufficient.** `System::~System()` currently calls `destroy()` before severing store-backed atom handles. That enters `AtomContainer::destroy()` / `Composite::destroy()` / `Composite::clear()` / `destroyChildren_()`, and each child destructor still hits `Composite::~Composite()`. `Composite::~Composite()` calls `parent_->removeChild(*this)` when `parent_ != 0`; `removeChild()` mutates parent topology, decrements selection counters when `child.contains_selection_`, calls `updateSelection_()`, and stamps modification state. Even if `~Atom` itself becomes pure slot-release, the parent-side destruction path is still write-heavy with respect to Composite tree state.

This does not necessarily walk `selected_bits_`, but it does walk and mutate selection/topology state while teardown is in progress. If W1 mirrors `removeChild()` into `composite_nodes_` as a normal forward mutation, it recreates the P2.1.1 failure mode. D45 needs an explicit teardown contract: destruction cascade must either suppress store-side topology/selection writes from parent `removeChild()`/`clear()` paths, or use a store-owned bulk teardown path that bypasses normal mutation mirroring.

**P29-2. D46/D47 heterogeneous tree is under-specified.** The decisions say Atom counters move off Atom while non-Atom Composites keep counters inline "via the side-table node or their own still-inline state." That is not a locked design; those are different architectures. The current `CompositeNode` has only parent/child/sibling handles and `child_count`; it has no `Composite*` back pointer and no selection counters. A heterogeneous tree can be coherent only if every side-table handle can resolve to the live object/handle it represents and if counters have a single owner. Today that is not specified.

If Molecule/Chain/Residue/System stay inline and Atom alone flips, an Atom-side node may point at an inline parent, but the node stores a `CompositeHandle`, not a direct `Composite*`. D47's "forward getParent through the node table" requires a handle-to-object map for non-Atom nodes and a handle-to-Atom-handle map for Atom nodes. The current side-table shape does not provide that.

**P29-3. AtomIterator unchanged is false against the actual code.** `Composite::CompositeIteratorTraits` in `composite.h` directly reads `first_child_`, `last_child_`, `next_`, `previous_`, and `parent_`. `ChildCompositeIteratorTraits` and `AncestorIteratorTraits` do the same. There is a `Composite::getNode_()` abstraction, but current iterator traits do not use it; `getNode_()` itself currently returns the inline pointers.

More seriously, `AtomIterator` is `BidirectionalIterator<Composite, Atom, Composite::CompositeIterator, AtomIteratorTraits>`. Dereference is a C-style cast from `Composite&` to `Atom&` through the iterator templates. If `Atom` no longer derives from `Composite`, this is not a valid preserved model. Keeping `AtomIterator` "unchanged" requires either keeping Atom in the C++ Composite inheritance hierarchy, or redesigning the iterator payload/traits to yield Atom handles from side-table identity. The current W0 answer does neither.

**P29-4. D49 named-property iteration is not fully feasible as stated.** The public type is `typedef std::vector<NamedProperty>::iterator NamedPropertyIterator`, and `beginNamedProperty()`/`endNamedProperty()` return iterators into the private `named_properties_` vector. `getNamedProperty(Position)` returns `NamedProperty&`, so callers can mutate the returned object in place.

A column+sparse store cannot synthesize such iterators without either materializing a real vector whose lifetime outlives the iterator, or changing the iterator type/API. Even a materialized vector would not support mutation-through-reference unless writes are committed back to the columns on every mutation of `NamedProperty&`, which the type cannot observe. This is a controlled-break candidate unless v2.2 keeps a per-Atom mutable `NamedProperty` cache, which undermines the memory goal and needs explicit sizing.

**P29-5. D45 vs D48 is not a contradiction, but reparenting is not covered tightly enough.** Adoption migration is not destruction, so copy-then-release side-table writes are consistent with D45. Re-parenting is different: `appendChild()` and `prependChild()` remove from the old parent and then attach to the new parent. W1 forward wiring must update the old parent, new parent, child, and neighboring siblings in the side table as one coherent mutation.

Current comments in `removeChild()` already show why this is delicate: sibling pointers must be captured before detach. D48 covers cross-store adoption, but W0 does not yet explicitly require complete side-table updates for same-store reparent/swap/splice/insertParent/replace. Those are forward mutations and must be in W1 parity scope.

**P29-6. D51 Bond sequencing is plausible but incomplete.** Removing `Atom::bond_[]` does not directly affect the Composite tree iterators; those are separate structures. The bond iterators, however, are currently hardwired to `number_of_bonds_` and `bond_[position_]` in `Atom::BondIteratorTraits`, so D51 must rewrite that iterator surface too.

W3 can follow W2, but it is not wholly independent: Atom thin-handle sizing still includes `number_of_bonds_` and `bond_[]` unless W3 removes them. If the v2.2 success gate claims Atom is about 32 B before W3, that claim is inconsistent with the current Atom layout. Either W2's size gate must exclude bond state explicitly, or W3 must be treated as part of the Atom memory closure.

**P29-7. D50 is too vague for a gate.** The ledger plus Codex sign-off is useful after the fact, but "where forwarding is impractical" gives too much room to discover expected breaks opportunistically during implementation. W0 should list anticipated breaks before W1/W2 begin: Atom no longer implicitly upcasts to `Composite*`, mutable `NamedProperty&`/`NamedPropertyIterator`, `BitVector&`, SIP signatures, and any API returning addresses/references to removed inline base state.

Without an a-priori expected-break list, reviewers cannot separate intended v2.2 scope from accidental API erosion.

**P29-8. Missing design answer: Composite* -> Atom* after the flip is a BLOCKER.** The suspicion is confirmed. `detail::compositeAsAtom_(Composite*)` is defined in `source/CONCEPT/composite.C` as `dynamic_cast<Atom*>(c)`. Once `Atom` is no longer derived from `Composite`, that cast is ill-formed at compile time, not merely a null result. The helper consolidation reduced the number of CORE_ONLY sites, but it did not solve identity.

D47 also says `CompositeIterator` still yields the Atom because "the Atom still has a tree node." A tree node is not a C++ object of type `Composite`. Existing processors, predicates, visitors, iterators, and RTTI routes consume `Composite&`/`Composite*`. A side-table node cannot be passed to those APIs unless there is either a stable proxy object deriving from `Composite`, a redesigned iterator/visitor API, or Atom continues to inherit `Composite`. W0 has no design for this.

SIP is also not just a release-note issue if Python support remains ON by default in CMake. The kickoff says PyBALL v2 is out of scope, but current build configuration still exposes Python/SIP machinery. W0 must decide whether v2.2 disables Python support for the thin-handle phases, updates bindings in scope, or carries a CI configuration that proves SIP is not built.

## New findings

1. **BLOCKER: side-table topology lacks object resolution.** `CompositeNode` stores handles, but there is no visible reverse table from `CompositeHandle` to `Composite*` or `Atom*`. `CompositeTopologyView` requires `Composite*` fields. After Atom stops being a Composite, even a reverse map cannot return `Composite*` for Atom unless a proxy exists.

2. **BLOCKER: `Composite::getNode_()` is only a placeholder.** It currently returns inline pointers and comments that side-table reads are a later phase. D47 treats it as an already-available uniform abstraction. It is not.

3. **BLOCKER: many Composite methods beyond iterators read inline links directly.** `getDepth()`, `getRoot()`, `getChild()`, `getSibling()`, `isDescendantOf()`, `isValid()`, `select_()`, `deselect_()`, `clear()`, `destroyChildren_()`, `swap()`, and the apply helpers all use inline `Composite` members directly. W0 focuses on iterators, but the flip requires a broader rewrite or a strict statement that these methods do not apply to Atom handles.

4. **HIGH: selection counters have no post-flip storage for Atom subtree semantics.** If Atom is always a leaf, this may be acceptable, but the design should say so and assert that Atom cannot have children post-flip. If Atom can host children as a Composite node, then selected/contains counters for Atom have no stated storage.

5. **HIGH: property full-surface promise conflicts with memory target.** Preserving mutable vector iterator semantics likely requires a per-object vector or a breaking API. The decision currently promises both full forwarding and a materializing adapter, but the existing API type prevents that from being a transparent adapter.

6. **MEDIUM: adoption migration does not mention property sparse/dense ordering or conflict policy.** Copying dense columns plus sparse bag must preserve v0 `named_properties_` order if D49 keeps iteration semantics. The store currently has column registry order plus sparse vector order, not necessarily v0 vector order.

7. **MEDIUM: W1 parity scope must include all topology mutations, not only append/remove.** `insertBefore`, `insertAfter`, `insertParent`, `spliceBefore`, `spliceAfter`, `splice`, `replace`, `swap`, `clear`, `destroy`, and `deleteChildrenList_` all mutate topology or counters.

## Overall verdict

**NO-GO for W1 as a design-locked milestone.** D45-D51 do not yet close the R20 blocker class. The largest unresolved issue is conceptual: after deleting `Atom : Composite`, the current system still expects atoms to be returned, filtered, cast, visited, selected, and iterated as `Composite` objects. `composite_nodes_` preserves topology data, but it does not preserve C++ object identity.

Before W1 starts, W0 needs explicit decisions for:

1. The post-flip identity model: proxy `Composite` node, redesigned AtomIterator/store iterator, or no deletion of `Atom : Composite` in v2.2.
2. Replacement for `detail::compositeAsAtom_(Composite*)` that compiles and resolves real atoms.
3. Iterator and traversal rewrite scope, including `CompositeIteratorTraits`, child/ancestor iterators, apply traversal, predicates, and visitors.
4. Destruction-mode rules that prevent parent `removeChild()`/`clear()` from writing side-table topology/selection during `~System` cascade.
5. Property API break list, especially mutable `NamedProperty&`, `NamedPropertyIterator`, and `BitVector&`.
6. Heterogeneous-tree storage contract: where counters live, how handles resolve to objects, and whether non-Atom Composites are side-table nodes or inline-only.
7. SIP/Python policy for v2.2 CI and release.

With those unresolved, W1 mutation wiring would be building toward an incoherent flip target and risks reintroducing the exact destruction-order corruption that P2.1.1 exposed.
