# Track B Cluster B — Codex CLI Round 16 (2026-05-19)

**Status:** Complete. Verdict: **NEEDS-FIXES → 2/3 [BUG]s fixed,
1 documented → GO for v2.0.0-rc3**.

**Reviewer:** Codex CLI 0.128.0 (`codex exec`).
**Subject:** Track B Cluster B (commits `2d6a820e3` + `73a640a80`) —
QSAR + STRUCTURE restore + MOLMEC AMBER/MMFF94 + SCORING
COMPONENTS/FUNCTIONS + SOLVATION + DOCKING + remaining FORMAT.
Cluster B brought CORE_ONLY ctest from 174/174 (rc2) to 279/279 (+105).

---

## Verdict (Codex, verbatim)

> Final: NEEDS-FIXES
>
> Minimum rc3 blockers: fix SmilesParser adoption order, restore the
> three missing CORE_ONLY tests into `BALL_STRUCTURE_TESTS`, and either
> implement bond-property JSON persistence or explicitly downgrade
> the K0.6 persistence claim before tagging.

## Probe answers

| # | Probe | Verdict | Status |
|---|---|---|---|
| C-B1 | SmilesParser_test failure root cause | [BUG] — K0 adoption-order break in public parser, not a harmless WILL_FAIL | **Fixed** (build off-system, insert populated) |
| C-B2 | scoringComponent.C un-ifdef'd back-transform | OK with caveat | — |
| C-B3 | scoringFunction.C un-ifdef'd SideChainOptimizer | OK | — |
| C-B4 | Test dedupe silently lost 3 CORE_ONLY tests | [BUG] — FragmentDB/DefaultProcessors/NormalizeNamesProcessor missing | **Fixed** (added to BALL_STRUCTURE_TESTS) |
| C-B5 | AssignBondOrderProcessor_test2 quarantine gate | OK | — |
| C-B6 | Lock ordering / thread-safety regressions | OK; residual Bond::setOrder data race documented | — |
| C-B7 | Bond-property JSON persistence | [BUG] — MMFF94/HBond bond annotations don't round-trip | **Documented** (v2.1 backlog V21-BOND-PROPERTY-JSON) |
| C-B8 | sizeof(Atom)/sizeof(Bond) unchanged | OK — Sizeof_test still pins 360 B / 288 B | — |
| C-B9 | Ship rc3 with SmilesParser WILL_FAIL? | [BUG] No — fix before tagging | **Fixed** via C-B1 |

## Fixes applied this commit

### C-B1 fix — SmilesParser adoption order

`source/STRUCTURE/smilesParser.C` line 158-163:

Pre-fix:
```cpp
Molecule* molecule = new Molecule;
system_.insert(*molecule);                     // empty molecule into system FIRST
for (atom : all_atoms_) molecule->insert(*atom); // single-atom adopt rejected
                                                 //  by canAdopt — partners in orphan
```

Post-fix:
```cpp
Molecule* molecule = new Molecule;
for (atom : all_atoms_) molecule->insert(*atom); // off-system: no adopt
system_.insert(*molecule);                       // triggers adoptSubtree
                                                 //  which migrates bonded atoms
                                                 //  in a single batched pass
```

SmilesParser_test WILL_FAIL quarantine removed in test/CMakeLists.txt.

### C-B4 fix — Restore 3 CORE_ONLY tests into BALL_STRUCTURE_TESTS

`test/cmake/BALLTestExecutables.cmake`: added FragmentDB_test,
DefaultProcessors_test, NormalizeNamesProcessor_test to
BALL_STRUCTURE_TESTS. They had been listed only in BALL_KERNEL_TESTS,
so when the CB-3 dedupe removed them from BALL_CORE_KERNEL_TESTS the
CORE_ONLY build silently dropped them. Net: +3 tests.

### C-B7 — Bond-property JSON gap documented (v2.1)

`.planning/RELEASE-NOTES-v2.0.md`: new "Known gaps" subsection
explicitly stating that Bond's `PropertyManager` bag is NOT
serialised in K0.6 JSON. Affected: MMFF94 bond-type properties
(MMFF94SBMB, MMFF94RBL), VIRTUAL__BOND markers, HBondProcessor
annotations. Most workloads don't persist bond properties (they're
ephemeral during force-field setup), so shipping rc3 with the gap
documented is acceptable.

V21-BOND-PROPERTY-JSON filed as v2.1 backlog item. Implementation
needs: writer exposes `bond_back_ptr_[i]` to find Bond handle,
serialises its PropertyManager bag inline with the bond record;
reader finds the Bond handle after restore (Bond is heap-owned by
the AtomContainer hierarchy, not in the store).

## Verification

- CORE_ONLY ctest: **282/282 PASS** (was 279 at Cluster B close;
  +3 from C-B4 restoration; +0 from C-B1 since SmilesParser_test
  was already counted but WILL_FAIL'd; net +3).
- MoleculeStore_test 100-run stress: 0/100 fails.
- Heap-System repro: exit 0.
- 0 WILL_FAIL tests in CORE_ONLY mode. 0 disabled tests.

## v2.0.0-rc3 readiness

All R16 blocker items are either fixed (C-B1, C-B4) or explicitly
documented with v2.1 backlog (C-B7). Cluster B closure is clean.

**Verdict: GO for v2.0.0-rc3 tag.**
