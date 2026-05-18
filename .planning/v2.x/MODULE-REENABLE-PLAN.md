# BALL 2.0 Module Re-enablement Plan

**Status:** Authored 2026-05-18 from the in-tree dependency audit
(Explore agent run; full report in commit message of this file's
introducing commit).

**Audience:** Track B of `V2.0-ROADMAP.md` — starts after KERNEL v2.0
ships (i.e., after K0.8 sign-off).

---

## 1. Dependency matrix (from audit)

| Module | # sources | Incoming deps from other disabled modules | Depends on |
|---|---:|---:|---|
| **PLUGIN** | 3 | **0** | (core only) |
| **PYTHON** | 11 | 0 | 7 modules (covers most of BALL) |
| **ENERGY** | 5 | 3 (PYTHON, SCORING, SOLVATION) | STRUCTURE |
| **XRAY** | 3 | 1 (FORMAT) | FORMAT, STRUCTURE |
| **NMR** | 17 | 2 (PYTHON, MOLMEC?) | FORMAT, STRUCTURE |
| **SOLVATION** | 16 | 2 (PYTHON, SCORING) | FORMAT, MOLMEC, STRUCTURE |
| **QSAR** | 50 | 4 | FORMAT, MOLMEC, STRUCTURE |
| **DOCKING** | 16 | 5 | FORMAT, MOLMEC, QSAR, SCORING, STRUCTURE |
| **SCORING** | 47 | 3 | DOCKING, FORMAT, MOLMEC, ENERGY, QSAR, SOLVATION, STRUCTURE |
| **MOLMEC** | 57 | 6 | FORMAT, QSAR, SCORING, SOLVATION, STRUCTURE, DOCKING |
| **FORMAT** | 40 | **9** | (core only — unblocks the most) |
| **STRUCTURE** | 77 | **10** | (core only — lynchpin) |

`# sources` = .C files in `source/<module>/`. `Incoming deps` = number
of other currently-disabled modules that include this one's headers.

---

## 2. Recommended re-enable order

Designed to:
1. Unblock the most modules per step (FORMAT first satisfies 9
   downstream dependencies).
2. Re-enable modules with the least surface area first to catch
   integration bugs cheaply.
3. Defer Python bindings to last so the entire stack is stable before
   wrapping.

### Phase B0 — Smoke test
**B0.1.** Re-enable PLUGIN (3 sources, 0 deps).
- Cheapest possible toggle.
- Catches any "BALL_CORE_ONLY guard sloppiness" that hid a real bug.
- Tests re-enabled: 0 (no PLUGIN-specific tests in the K0 deletion list).
- **Estimate:** 1 commit, ~1 hour.

### Phase B1 — Core-only-dependent modules (parallel)
Three modules depend only on the core (no inter-disabled-module deps).
Can re-enable in parallel.

**B1.1.** FORMAT (40 sources, 0 outgoing-disabled deps).
- **Unblocks 9 modules** — highest leverage step.
- Brings back: PDB / HIN / mol2 / INI parsers + writers.
- Tests re-enabled: `Residue_test1/2`, `Expression_test`,
  `ExpressionParser_test`, `Selector_test`, `StandardPredicates_test1..4`,
  `StdIteratorWrapper_test` (FORMAT-pulling tests from the K0 13-test
  deferred set). After this: K0 green-bar grows from 36 → 47.
- **Estimate:** ~1 week. Likely needs a few small fixes for v2.x atom
  field access patterns in FORMAT readers/writers.

**B1.2.** STRUCTURE (77 sources, 0 outgoing-disabled deps).
- **Lynchpin** — 10 downstream modules need it.
- Brings back: bond perception, ring perception, fragment DB, geometric
  properties.
- Tests re-enabled: `FragmentDB_test`, `DefaultProcessors_test` (the
  K0-deferred tests). K0 green-bar grows from 47 → 49 = approaches the
  48 locked target (49 because DefaultProcessors_test was deferred,
  not deleted).
- **Estimate:** ~2 weeks. STRUCTURE has the deepest core integration
  (379 KERNEL refs); atom-field flips (K0.3b.LATER) may have broken
  some patterns.

**B1.3.** PYTHON binding header (just so it compiles; not the full
SIP generator).
- **Not actually independent** — PYTHON pulls 7 modules in its
  bindings. Defer to final phase (B5).
- *Skip in B1.*

### Phase B2 — Single-FORMAT-dependent (parallel)
After FORMAT is in.

**B2.1.** XRAY (3 sources, depends on FORMAT + STRUCTURE)
**B2.2.** NMR (17 sources, depends on FORMAT + STRUCTURE)
**B2.3.** ENERGY (5 sources, depends on STRUCTURE only)
- ENERGY can technically start in B1 since it doesn't need FORMAT —
  but its 3 downstream consumers (PYTHON, SCORING, SOLVATION) need
  FORMAT, so no advantage to splitting.

These three are small and independent — parallel-assignable.

**Tests re-enabled in B2:**
- `EnergyMinimizer_test`, `EnergyProcessor_test`, `AtomicContactEnergy_test`
  (ENERGY-pulling)
- `NMRStarFile_test`, `Bruker1DFile_test`, `Bruker2DFile_test`
  (NMR-pulling)
- `CCP4File_test` (XRAY-pulling)

**Estimate:** ~3-5 days each in parallel.

### Phase B3 — MOLMEC + dependents
After STRUCTURE + FORMAT.

**B3.1.** MOLMEC (57 sources).
- Critical force-field hub. Brings back AMBER, CHARMM, MMFF94.
- Tests re-enabled: `RuleEvaluator_test`, `NormalizeNamesProcessor_test`,
  `MMFF94_test*`, `AmberFF_test`, `CharmmFF_test`, ...
- **Estimate:** ~2 weeks. Many tests; some likely need updates for the
  store-backed Atom layout.

### Phase B4 — Higher-level (parallel after B3)
After MOLMEC.

**B4.1.** QSAR (50 sources)
**B4.2.** SOLVATION (16 sources)
**B4.3.** SCORING (47 sources) — depends on QSAR + SOLVATION + MOLMEC

QSAR + SOLVATION can run in parallel; SCORING follows when both done.

**Tests re-enabled:** all QSAR_test*, SOLVATION_test*, SCORING_test*.

**Estimate:** B4.1 + B4.2 parallel ~1 week; B4.3 ~1 week.

### Phase B5 — Top of stack
**B5.1.** DOCKING (16 sources) — depends on QSAR + SCORING + STRUCTURE
**B5.2.** PYTHON bindings — wraps everything; lands last.

**Estimate:** B5.1 ~1 week; B5.2 ~3-4 weeks (SIP rebuild is non-trivial
and gates the v2.1 PyBALL phase per ROADMAP-v2.md).

---

## 3. Per-module re-enable recipe

For each module being re-enabled:

1. **Configure**: edit `cmake/BALLIncludes.cmake` — move the module out
   of the `IF(NOT BALL_CORE_ONLY)` block (or, if doing it differently:
   add a per-module `BALL_ENABLE_<MOD>` toggle).
2. **Build**: `ninja BALL`. Expect compile errors.
3. **Fix compile errors**: most likely store-side API mismatches. For
   example, code that does `atom->position_` direct access (post K0.3b.LATER
   when position_ is deleted) needs to use `atom->getPosition()`.
4. **Re-enable module's tests**: edit `test/CMakeLists.txt` to add the
   module's group to `BALL_CORE_KERNEL_TESTS` (or rename that var to
   `BALL_PROGRESSIVE_TESTS` and grow it).
5. **Build tests**: `ninja <test_group>`.
6. **Run tests**: capture pass/fail.
7. **Fix test failures**: per-test analysis. Some may be legitimate
   regressions from the KERNEL rewrite that need behavior fixes.
8. **Commit**: one commit per module enable (with all the associated
   fixes squashed in).

**Gate per commit:** all previously-enabled modules' tests stay green;
new module's tests pass.

---

## 4. Tri-OS validation

After every Phase Bx completes, run the tri-OS CI matrix
(macOS-arm64 + Ubuntu x64 + Windows x64). Some store-side patterns
may behave differently on Windows (template export discipline, MSVC's
stricter friend semantics).

---

## 5. v2.0 SHIP GATE

Achieved when:
- All 12 modules re-enabled
- All ~246 tests pass (or explicitly quarantined per the v1.7 triage
  pattern with WILL_FAIL guards)
- Tri-OS CI green
- KERNEL v2.0 RELEASE-NOTES-v2.0.md final
- File-format v1→v2 converter ships in `bin/`
- Memory + perf benchmark report in release notes

---

*End of MODULE-REENABLE-PLAN.md.*
