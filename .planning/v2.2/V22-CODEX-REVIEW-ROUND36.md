# BALL v2.2 H2a Code Review - Round 36

**Status:** COMPLETE (R36 NEEDS-FIXES → R36b → R36c → R36d GO)
**Verdict:** R36/R36b/R36c = NEEDS-FIXES; **R36d = GO — proceed to H2b**
(see the R36b/R36c/R36d re-review sections appended below). The crux-risk
H2a mutation mirror converged over four implementation rounds: 2 BLOCKERs
(reparent duplicate; atom-insert edge), the same-store-atom-move + lost
positional order, then the re-append duplicate — all closed. The
destruction guard (P2.1.1) + atom-index timing were correct from R36.
**Reviewer:** Codex CLI 0.128.0  
**Subject:** H2a mutation mirror close review (commits `0ddb69f91` + `92eab60c6`)

## Probe Answers

### P36-1. Destruction guard correctness

The destruction guard itself is directionally correct: `Composite::removeChild()` mirrors after the v0 detach (`source/CONCEPT/composite.C:1335`, `source/CONCEPT/composite.C:1422`), and `mirrorRemoveChild_()` gates on the **parent** (`this`) being destroyed (`source/CONCEPT/composite.C:216`, `source/CONCEPT/composite.C:221`).

- `delete` heap residue under live chain: `~Residue` sets the child flag first (`source/KERNEL/residue.C:78`-`source/KERNEL/residue.C:81`), then `destroy()` detaches from the live parent through `Composite::destroy()` (`source/CONCEPT/composite.C:1527`-`source/CONCEPT/composite.C:1534`). The parent chain is not being destroyed, so the chain row correctly drops the residue. Deeper atom deletion then calls `residue.removeChild(atom)` while the residue parent is flagged, so the dying residue row is not mutated.
- `~System` full cascade: `~System` sets the flag before `destroy()` (`source/KERNEL/system.C:439`-`source/KERNEL/system.C:442`), and each derived molecular destructor does the same before its own `destroy()` call. Therefore parent-side detach mirrors are skipped throughout the cascade, which avoids the P2.1.1 trap.
- explicit `chain.removeChild(residue)` on a live tree: the parent flag is false, so the mirror fires and removes the child ref. Correct for a live mutation.
- re-parent within one live System: the old-parent remove part is correct, but the insert/materialization part is not; see BLOCKER-1.

All 11 named molecular destructors are flagged before `destroy()`: `~Atom` (`source/KERNEL/atom.C:289`-`source/KERNEL/atom.C:292`), `~AtomContainer` (`source/KERNEL/atomContainer.C:52`-`source/KERNEL/atomContainer.C:55`), `~Molecule` (`source/KERNEL/molecule.C:32`-`source/KERNEL/molecule.C:35`), `~Protein` (`source/KERNEL/protein.C:30`-`source/KERNEL/protein.C:33`), `~NucleicAcid` (`source/KERNEL/nucleicAcid.C:30`-`source/KERNEL/nucleicAcid.C:33`), `~Chain` (`source/KERNEL/chain.C:27`-`source/KERNEL/chain.C:30`), `~Residue` (`source/KERNEL/residue.C:78`-`source/KERNEL/residue.C:81`), `~Nucleotide` (`source/KERNEL/nucleotide.C:36`-`source/KERNEL/nucleotide.C:39`), `~SecondaryStructure` (`source/KERNEL/secondaryStructure.C:32`-`source/KERNEL/secondaryStructure.C:35`), `~Fragment` (`source/KERNEL/fragment.C:26`-`source/KERNEL/fragment.C:29`), and `~System` (`source/KERNEL/system.C:439`-`source/KERNEL/system.C:442`). `~Composite` also has the backstop before detach/cascade (`source/CONCEPT/composite.C:289`-`source/CONCEPT/composite.C:301`).

### P36-2. adoptSubtree materialisation correctness

First-time materialization is mostly correct. Pass 1 migrates atoms and calls `migrateTo_(dst, di)` before Pass 4 (`source/KERNEL/system.C:336`-`source/KERNEL/system.C:350`), so Pass 4's atom child refs use post-migration store indices (`source/KERNEL/system.C:407`-`source/KERNEL/system.C:414`, `source/KERNEL/system.C:287`-`source/KERNEL/system.C:290`). Atoms already in `dst` are skipped by the migration collection (`source/KERNEL/system.C:309`-`source/KERNEL/system.C:316`) and retain their valid dst indices.

The recursion visits v0 child order via `getDegree()` and `getChild(i)` (`source/KERNEL/system.C:284`-`source/KERNEL/system.C:286`) and appends child refs in that order (`source/KERNEL/system.C:287`-`source/KERNEL/system.C:295`). Top-level molecules remain roots because the root is linked only if the v0 parent is an `AtomContainer` already bound to a dst row (`source/KERNEL/system.C:414`-`source/KERNEL/system.C:421`); `System` is not a row.

This correctness does **not** hold on idempotent re-entry; see BLOCKER-1.

### P36-3. Idempotent re-entry / double-append

Not correct. `materialiseContainer_()` reuses an existing row when the container is already bound to `dst` (`source/KERNEL/system.C:255`-`source/KERNEL/system.C:264`) but then unconditionally appends every existing v0 child again (`source/KERNEL/system.C:284`-`source/KERNEL/system.C:295`). `ContainerTable::append_child()` does not deduplicate (`source/KERNEL/moleculeStore.C:386`-`source/KERNEL/moleculeStore.C:390`), so re-entry duplicates child refs. Pass 4 also unconditionally appends the root row under the current parent when that parent has a row (`source/KERNEL/system.C:414`-`source/KERNEL/system.C:421`).

This hits ordinary same-System reparent/reinsert flows, not just malicious double calls: `AtomContainer::append(AtomContainer&)` always calls `adoptSubtree()` after the v0 append (`source/KERNEL/atomContainer.C:436`-`source/KERNEL/atomContainer.C:440`), while `Composite::appendChild()` first removes the child from its old parent if needed (`source/CONCEPT/composite.C:873`-`source/CONCEPT/composite.C:877`). Reparenting an already-materialized residue from chain A to chain B removes the old chain edge, then `adoptSubtree(residue)` reuses the residue row and appends its atom children a second time.

### P36-4. Self-healing / binding

The missing D74 self-healing is acceptable only for first-time free-standing adoption and cross-store adoption where `materialiseContainer_()` sees `container_row_store_ != dst` and allocates new rows (`source/KERNEL/system.C:255`-`source/KERNEL/system.C:264`). It is not acceptable for same-store detach/reinsert because D74 explicitly says detached containers keep their binding and re-insertion into the same store reuses it (`.planning/v2.2/V22-H2-DESIGN.md:286`-`.planning/v2.2/V22-H2-DESIGN.md:291`). Current reuse is not a no-op: it rebuilds children by appending duplicates. That makes the binding stale with respect to the row's edge vector even though the store pointer still matches.

### P36-5. removeChild atom-index timing

The atom index timing is safe. `~Atom` sets `being_destroyed_`, calls `destroy()`, and only after that releases its store slot (`source/KERNEL/atom.C:289`-`source/KERNEL/atom.C:313`). `Atom::destroy()` calls `Composite::destroy()` (`source/KERNEL/atom.C:326`-`source/KERNEL/atom.C:331`), and `Composite::destroy()` detaches from the parent before clearing children (`source/CONCEPT/composite.C:1527`-`source/CONCEPT/composite.C:1534`). Therefore `mirrorRemoveChild_()` reads `a->getStoreIndex()` before `release_atom()` can free or recycle the slot (`source/CONCEPT/composite.C:232`-`source/CONCEPT/composite.C:235`).

### P36-6. Deferred clear/destroyChildren_ branch and other bypasses

The non-auto-deletable `clear()` direct-detach branch is still deferred and the H2a test correctly avoids asserting the dying row's emptying (`test/HierarchyParity_test.C:568`-`test/HierarchyParity_test.C:573`). The branch still bypasses `removeChild()` by directly nulling child links (`source/CONCEPT/composite.C:1491`-`source/CONCEPT/composite.C:1505`), so H2b must hook it.

There is another H2a-scope bypass: direct atom insertion into a live, materialized container. `AtomContainer::append(Atom&)` mutates the v0 tree and then calls `System::adopt(atom)` (`source/KERNEL/atomContainer.C:308`-`source/KERNEL/atomContainer.C:317`), but single-atom `System::adopt()` only migrates the store slot and never appends an `ATOM` ChildRef to the parent container row (`source/KERNEL/system.C:101`-`source/KERNEL/system.C:203`). H2a covers atom removal (`source/CONCEPT/composite.C:232`-`source/CONCEPT/composite.C:235`) but not atom insertion after a container row already exists.

### P36-7. Layering + encapsulation

`source/KERNEL/system.C` uses `detail::compositeAsAtom_()` for atom discrimination in materialization (`source/KERNEL/system.C:250`-`source/KERNEL/system.C:252`, `source/KERNEL/system.C:287`-`source/KERNEL/system.C:290`) and does not introduce `dynamic_cast<Atom*>` or `RTTI::isKindOf<Atom>` in `source/KERNEL`. Container type discrimination uses RTTI/dynamic casts for non-Atom container types (`source/KERNEL/system.C:233`-`source/KERNEL/system.C:243`, `source/KERNEL/system.C:267`-`source/KERNEL/system.C:282`), which is within the stated scope.

No private `ContainerTable`, `ContainerRow`, or internal `ChildRef` type leaked into public H2a APIs. Public write/read accessors expose `ContainerKind`, `ContainerChildRef`, strings, and integer indices only (`include/BALL/KERNEL/moleculeStore.h:441`-`include/BALL/KERNEL/moleculeStore.h:475`). `AtomContainer` exposes only the binding hooks and raw store/row values (`include/BALL/KERNEL/atomContainer.h:401`-`include/BALL/KERNEL/atomContainer.h:410`).

### P36-8. Overall

H2a is not safe to build H2b on as-is. The destruction guard is correctly placed, and first-time subtree materialization is close, but the mutation mirror is already parity-divergent for same-System reparent/reinsert and incomplete for direct atom insertion into live materialized containers. Those are not H2b polish issues; they affect the core H2a insert/remove surface.

## New Findings

### BLOCKER-1: Re-adopting or same-System reparenting an already-materialized container duplicates its child refs

`materialiseContainer_()` treats "already bound to dst" as idempotent (`source/KERNEL/system.C:255`-`source/KERNEL/system.C:259`) but still appends all children again (`source/KERNEL/system.C:284`-`source/KERNEL/system.C:295`). The table append path is a blind vector push (`source/KERNEL/moleculeStore.C:386`-`source/KERNEL/moleculeStore.C:390`), so no deduplication saves this.

Concrete failing path: build `sys/prot/chainA/residue/atom`, then create `chainB` in the same `sys` and call `chainB.insert(residue)`. The v0 append removes the residue from chain A (`source/CONCEPT/composite.C:873`-`source/CONCEPT/composite.C:877`), then `AtomContainer::append(AtomContainer&)` calls `adoptSubtree(residue)` (`source/KERNEL/atomContainer.C:436`-`source/KERNEL/atomContainer.C:440`). Since the residue row is already bound to the same store, H2a reuses it and appends its atom child refs again. The reachable table now has duplicate atoms under the residue and diverges from v0.

Fix requirement: make materialization distinguish "new/rebuilt row" from "already-current row". Reuse must not rebuild existing child edges. Alternatively, explicitly clear/rebuild the row's children before appending, but then reverse edges and parent linkage must be handled coherently. Add parity tests for same-System container reparent, detach+reinsert into same store, and explicit repeated `adoptSubtree()` on an already-bound root.

### BLOCKER-2: Direct atom insertion into a live materialized container is not mirrored

`AtomContainer::append(Atom&)` and siblings mutate the v0 tree and call `System::adopt(atom)` (`source/KERNEL/atomContainer.C:291`-`source/KERNEL/atomContainer.C:317`, `source/KERNEL/atomContainer.C:325`-`source/KERNEL/atomContainer.C:347`). `System::adopt(Atom&)` migrates atom storage only (`source/KERNEL/system.C:101`-`source/KERNEL/system.C:203`); it never appends `ChildRef{ATOM, atom_idx}` to the parent row. Because H2a has no append/prepend/insert mirror hook in `Composite`, inserting an atom into an already-materialized residue silently leaves the table missing that atom.

Fix requirement: either implement H2a atom insert mirroring in the v0 insertion paths, or explicitly narrow H2a's scope and block/avoid parity claims for direct live atom insertion. Given D70 defines `ref(C)=ATOM(getStoreIndex())` and H2a is the insert/remove sub-step, the correct fix is to mirror atom insertion and add tests for live `append/prepend/insertBefore/insertAfter(Atom&)`.

### MEDIUM-1: H2a tests do not exercise the two highest-risk insert paths

The new tests cover first adoption, remove container/atom, live `destroy()` detach, delete cascade, and `~System` teardown (`test/HierarchyParity_test.C:492`-`test/HierarchyParity_test.C:615`). They do not cover same-System reparent/reinsert of an already-bound container or direct atom insertion after materialization. Both are production v0 APIs wired through `AtomContainer` and both currently diverge.

### LOW-1: Header comment overclaims D74 self-healing

`AtomContainer` says the mirror self-heals by re-materializing when the bound store differs (`include/BALL/KERNEL/atomContainer.h:401`-`include/BALL/KERNEL/atomContainer.h:406`), but H2a implements that only inside adoption materialization, not generally in every mirror hook. This is documentation drift today; it becomes a correctness issue if later hooks assume D74 already exists.

## Overall Verdict

**NEEDS-FIXES.** Do not proceed to H2b until:

1. `materialiseContainer_()` is made genuinely idempotent for already-bound same-store containers, with tests for same-System reparent, same-store detach+reinsert, and repeated adoption.
2. Direct atom insertion into a live materialized container appends the correct `ATOM` ChildRef in parent order, with parity tests for append/prepend/insert-before/insert-after atom insertion.
3. The H2a parity test suite is extended to cover those paths before rerunning the full suite.

Verification note: `ctest -R HierarchyParity -V` from the repository root found no tests, but the configured build directory passed `HierarchyParity_test` (`build`, test 132) in 0.02s. That test does not cover the failing paths above.

## R36b re-review (post-fix)

**Verdict: NEEDS-FIXES** -- do not proceed to H2b yet.

**R36 BLOCKER-1: CLOSED for the tested append/reparent duplicate case.** `materialiseContainer_()` now returns an existing same-store row immediately (`source/KERNEL/system.C:271`-`source/KERNEL/system.C:274`), so a same-System `chB.insert(r)` no longer re-runs the child materialisation loop (`source/KERNEL/system.C:297`-`source/KERNEL/system.C:309`). `Composite::appendChild()` still detaches from the old parent first (`source/CONCEPT/composite.C:873`-`source/CONCEPT/composite.C:877`), `removeChild()` mirrors the old-parent edge removal (`source/CONCEPT/composite.C:1413`-`source/CONCEPT/composite.C:1422`), and `adoptSubtree()` appends the root under the new parent once (`source/KERNEL/system.C:458`-`source/KERNEL/system.C:462`). The new regression covers this exact path (`test/HierarchyParity_test.C:644`-`test/HierarchyParity_test.C:672`).

**R36 BLOCKER-2: NOT CLOSED as a general H2a atom-insert fix.** The orphan/cross-store append case is fixed: `System::adopt(Atom&)` migrates the atom and then appends an ATOM child edge (`source/KERNEL/system.C:207`-`source/KERNEL/system.C:211`), and the added test covers `r.insert(a2)` for a new orphan atom (`test/HierarchyParity_test.C:617`-`test/HierarchyParity_test.C:641`). But same-System atom moves/reinserts are still broken: `System::adopt(Atom&)` returns immediately when `src == dst` (`source/KERNEL/system.C:108`-`source/KERNEL/system.C:111`), before calling `mirrorAppendChildEdge_()`. In a live move such as `r2.insert(a)` where `a` was already under `r1` in the same `System`, `Composite::appendChild()` removes `r1 -> a` first (`source/CONCEPT/composite.C:873`-`source/CONCEPT/composite.C:877`; mirror at `source/CONCEPT/composite.C:232`-`source/CONCEPT/composite.C:235`), then the early return skips adding `r2 -> a`. The table loses the atom edge while v0 has it under `r2`.

### New Findings

**H2a-blocker: The applied fix is not the described `rebuildParentRowFromV0_` design and does not preserve v0 order for positional live inserts.** There is no `rebuildParentRowFromV0_` in `source/KERNEL/system.C`; the actual helper is append-only (`source/KERNEL/system.C:327`-`source/KERNEL/system.C:342`). `AtomContainer::prepend(Atom&)`, `insertBefore(Atom&)`, and `insertAfter(Atom&)` all call `System::adopt(atom)` after mutating v0 (`source/KERNEL/atomContainer.C:291`-`source/KERNEL/atomContainer.C:347`), but the mirror helper always appends (`source/KERNEL/system.C:333`-`source/KERNEL/system.C:340`). The same append-only limitation applies to container positional inserts because `adoptSubtree()` also calls the append-only helper (`source/KERNEL/system.C:458`-`source/KERNEL/system.C:462`) after `insertBefore/insertAfter(AtomContainer&)` (`source/KERNEL/atomContainer.C:447`-`source/KERNEL/atomContainer.C:456`). This is an H2a blocker if H2a claims mutation-mirror parity for live insert/prepend/insertBefore/insertAfter; the code comments themselves mark these paths as only "positionally approximate" (`source/KERNEL/system.C:321`-`source/KERNEL/system.C:326`).

**Deferrable-to-H2d: The O(degree) concern is currently avoided, but by weakening correctness.** The expected full-parent rebuild would be O(degree) per single-atom insert; the committed implementation uses O(1) append (`source/KERNEL/system.C:313`-`source/KERNEL/system.C:327`). That avoids the H2d performance concern, but it is not a substitute for v0-order correctness in H2a.

**No new destruction-path regression found.** The destruction guard is unchanged: `mirrorRemoveChild_()` still returns when the parent is being destroyed (`source/CONCEPT/composite.C:215`-`source/CONCEPT/composite.C:221`), and `System::~System()` still marks destruction before teardown (`source/KERNEL/system.C:479`-`source/KERNEL/system.C:482`).

Verification: `ctest --test-dir build -R HierarchyParity -V` passes (`HierarchyParity_test`, 0.02s), but the added tests do not cover same-System atom reparent/reinsert or positional live inserts.

## R36c re-review (post-fix-2)

**Verdict: NEEDS-FIXES** -- do not proceed to H2b yet.

**R36b BLOCKER-2 same-store atom move: CLOSED.** The edge mirror is now in the insert method after v0 append (`source/KERNEL/atomContainer.C:377`-`source/KERNEL/atomContainer.C:379`), so `System::adopt(Atom&)` may still early-return for `src == dst` (`source/KERNEL/system.C:101`-`source/KERNEL/system.C:105`) without losing the new parent edge. The regression covers `r2.insert(a1)` from `r1` to `r2` in the same `System` (`test/HierarchyParity_test.C:675`-`test/HierarchyParity_test.C:704`).

**R36b positional-order finding: CLOSED.** Positional atom inserts re-derive the parent row after v0 mutation (`source/KERNEL/atomContainer.C:359`-`source/KERNEL/atomContainer.C:367`, `source/KERNEL/atomContainer.C:395`-`source/KERNEL/atomContainer.C:397`, `source/KERNEL/atomContainer.C:409`-`source/KERNEL/atomContainer.C:411`), and positional container inserts do the same (`source/KERNEL/atomContainer.C:491`-`source/KERNEL/atomContainer.C:498`, `source/KERNEL/atomContainer.C:513`-`source/KERNEL/atomContainer.C:524`). `mirrorRederiveParent_()` clears and rebuilds from current v0 `getChild()` order (`source/KERNEL/atomContainer.C:51`-`source/KERNEL/atomContainer.C:71`). The bulk append path remains O(1), using only `mirrorAppendEdge_()` (`source/KERNEL/atomContainer.C:369`-`source/KERNEL/atomContainer.C:379`, `source/KERNEL/atomContainer.C:501`-`source/KERNEL/atomContainer.C:505`).

### New Findings

**H2a-blocker: Re-appending an already-last rooted child duplicates the container-table edge.** `Composite::appendChild()` is a no-op when the child is already `last_child_` (`source/CONCEPT/composite.C:867`-`source/CONCEPT/composite.C:870`), but `AtomContainer::append(Atom&)` still calls `mirrorAppendEdge_()` afterward (`source/KERNEL/atomContainer.C:377`-`source/KERNEL/atomContainer.C:379`). The same is true for `AtomContainer::append(AtomContainer&)` (`source/KERNEL/atomContainer.C:503`-`source/KERNEL/atomContainer.C:505`). `ContainerTable::append_child()` is a blind push (`source/KERNEL/moleculeStore.C:386`-`source/KERNEL/moleculeStore.C:390`), so a normal rooted no-op like `r.append(a)` when `a` is already `r`'s last atom leaves v0 unchanged but appends a duplicate `ATOM` ChildRef; likewise for an already-last child container. This violates the R36c append assumption: the child is last, but it was not necessarily "just inserted." Fix requirement: mirror only when the v0 append actually changed the tree, or make append use the idempotent parent re-derive path when the child was already present.

**No new double-mirror finding for actual inserted subtree roots.** `System::adoptSubtree()` now only calls `materialiseContainer_()` (`source/KERNEL/system.C:423`-`source/KERNEL/system.C:427`); the parent-link edge is added by the insert method. `materialiseContainer_()` returns immediately for already-bound same-store containers (`source/KERNEL/system.C:257`-`source/KERNEL/system.C:269`) and otherwise builds only the subtree's own rows/children (`source/KERNEL/system.C:271`-`source/KERNEL/system.C:305`), so root-under-parent and internal edges do not overlap on a real insert.

**Free-standing build then adopt: OK.** Inserts into unrooted parents have no bound parent row and therefore no-op in the mirror helpers (`source/KERNEL/atomContainer.C:31`-`source/KERNEL/atomContainer.C:35`, `source/KERNEL/atomContainer.C:53`-`source/KERNEL/atomContainer.C:58`). On later adoption, `materialiseContainer_()` builds from v0 order (`source/KERNEL/system.C:292`-`source/KERNEL/system.C:304`). For a top-level container inserted under `System`, `mirrorAppendEdge_()` sees no parent container row and no-ops, so no duplicate root edge is introduced.

**getParent() target: OK for successful inserts, except the no-op append case above.** `Composite::appendChild()` sets `parent_ = this` after a real append (`source/CONCEPT/composite.C:879`-`source/CONCEPT/composite.C:894`), and the positional paths are re-derived after their v0 mutation. When append is a no-op because the child is already last, `getParent()` is non-null and points at the existing parent, which is exactly why the append-only mirror duplicates.

**Layering/gate: OK.** `atomContainer.C` uses `detail::compositeAsAtom_()` for Atom discrimination in the new helpers (`source/KERNEL/atomContainer.C:36`-`source/KERNEL/atomContainer.C:43`, `source/KERNEL/atomContainer.C:62`-`source/KERNEL/atomContainer.C:69`); its `dynamic_cast` usage is for non-Atom container/System types. No private `ContainerTable`, `ContainerRow`, or internal `ChildRef` type is exposed in public headers; the public write surface remains scalar/public-typed (`include/BALL/KERNEL/moleculeStore.h:459`-`include/BALL/KERNEL/moleculeStore.h:478`).

**Destruction guard + removeChild mirror: unchanged and still OK.** `mirrorRemoveChild_()` still skips when the parent is being destroyed (`source/CONCEPT/composite.C:215`-`source/CONCEPT/composite.C:221`) and otherwise removes atom/container edges after v0 detach (`source/CONCEPT/composite.C:1335`-`source/CONCEPT/composite.C:1422`). `System::~System()` still marks itself destroying before teardown (`source/KERNEL/system.C:444`-`source/KERNEL/system.C:447`).

Verification: `ctest --test-dir build -R HierarchyParity -V` passes (`HierarchyParity_test`, 0.02s). This test set covers the two R36b fixes but does not cover the newly found no-op append/reappend duplication path.

## R36d re-review (post-fix-3)

**Verdict: GO -- proceed to H2b.**

**R36c BLOCKER: CLOSED.** `ContainerTable::append_child()` now treats appending the exact current last `ChildRef` as idempotent (`source/KERNEL/moleculeStore.C:386`-`source/KERNEL/moleculeStore.C:397`). This matches the v0 no-op in `Composite::appendChild()` when the argument is already `last_child_` (`source/CONCEPT/composite.C:867`-`source/CONCEPT/composite.C:870`) while allowing real appends/moves to continue through `push_back()` + `set_reverse_edge_()`. The regression covers both an already-last atom re-append and an already-last container re-append (`test/HierarchyParity_test.C:734`-`test/HierarchyParity_test.C:764`).

**No wrong-skip found.** A `ChildRef` is the identity of one child edge, not a multiset value, and v0 gives a child one position under one parent. If the same ref is already the table's last child, the only valid v0 append outcome is the already-last no-op. If the child is already in the same parent but not last, v0 removes it first (`source/CONCEPT/composite.C:873`-`source/CONCEPT/composite.C:877`), the remove hook deletes the old edge (`source/CONCEPT/composite.C:1413`-`source/CONCEPT/composite.C:1422`), and then `append_child()` sees a different current back and appends the moved ref. Positional paths still clear and rebuild from current v0 order (`source/KERNEL/atomContainer.C:51`-`source/KERNEL/atomContainer.C:71`), so the append guard does not mask a legitimate positional edge.

**Reverse edge handling: OK.** On the skipped path the first append already set the reverse edge through `set_reverse_edge_()` (`source/KERNEL/moleculeStore.C:370`-`source/KERNEL/moleculeStore.C:397`), and the no-op v0 append leaves parentage unchanged. On real moves, the old reverse edge is cleared by `remove_child()` (`source/KERNEL/moleculeStore.C:425`-`source/KERNEL/moleculeStore.C:435`) before the new append sets it again.

**No remaining H2a insert duplicate/divergence path found.** Atom and container append still call the O(1) mirror after v0 mutation (`source/KERNEL/atomContainer.C:369`-`source/KERNEL/atomContainer.C:379`, `source/KERNEL/atomContainer.C:501`-`source/KERNEL/atomContainer.C:505`), and the new table idempotency closes the already-last no-op hole for both. Prepend/insertBefore/insertAfter re-derive the parent row after v0 mutation for atoms and containers (`source/KERNEL/atomContainer.C:351`-`source/KERNEL/atomContainer.C:411`, `source/KERNEL/atomContainer.C:491`-`source/KERNEL/atomContainer.C:524`), so they do not accumulate duplicates. `System::adoptSubtree()` only materialises internal rows/edges now (`source/KERNEL/system.C:416`-`source/KERNEL/system.C:427`), and `materialiseContainer_()` returns immediately for already-bound same-store rows (`source/KERNEL/system.C:257`-`source/KERNEL/system.C:269`), so free-standing-then-adopt and same-System reparent do not double-build.

**Prior gate checks remain valid.** The destruction guard still skips remove mirroring only when the parent is being destroyed (`source/CONCEPT/composite.C:215`-`source/CONCEPT/composite.C:221`), atom store-slot release still happens after `Atom::destroy()` / parent detach (`source/KERNEL/atom.C:289`-`source/KERNEL/atom.C:312`), and `System::~System()` still marks destruction before teardown (`source/KERNEL/system.C:444`-`source/KERNEL/system.C:447`). Layering is unchanged: the public mutation surface remains scalar/public typed (`include/BALL/KERNEL/moleculeStore.h:459`-`include/BALL/KERNEL/moleculeStore.h:478`), and the H2a atom tests continue to use the approved `detail::compositeAsAtom_()` gate in KERNEL code (`source/KERNEL/atomContainer.C:36`-`source/KERNEL/atomContainer.C:43`, `source/KERNEL/system.C:292`-`source/KERNEL/system.C:303`).

**New findings: none.** No H2a-blocker or deferrable issue found in the R36c fix.

Verification: `ctest --test-dir build -R HierarchyParity -V` passes (`HierarchyParity_test`, 0.03s). A local full `ctest --test-dir build` did not reproduce the claimed 286/286 green in this workspace: 248/286 passed, with 38 failures in unrelated tests such as `Path_test`, `CreateSpectrumProcessor_test`, resource/force-field/descriptor tests, and one `Peptides_test` segfault. I did not treat those unrelated full-suite failures as H2a blockers.
