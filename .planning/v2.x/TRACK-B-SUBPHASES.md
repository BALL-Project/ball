# Track B — Module re-enable: sub-phase breakdown

**Status:** PLANNING. **Authored:** 2026-05-18, post K0.8 close.
**Source:** `MODULE-REENABLE-PLAN.md` (dependency audit) +
`V2.0-ROADMAP.md` Track B row.

K0 (the v2.0 KERNEL rewrite) closed in commit `6a2c60d01`. Track B
re-enables the 12 extension modules that K0 disabled via
`BALL_CORE_ONLY=ON`. v2.0 ships tag-ready against the K0 core; Track B
patch releases follow.

This document slices the re-enable into atomic sub-phases per the
proven K0.5/K0.6/K0.7 pattern (one cohesive commit per sub-phase,
its own green-tests gate, Codex review at each major boundary).

---

## Dependency-driven sequence

Per the audit table in `MODULE-REENABLE-PLAN.md` §1:

```
Wave 0:  PLUGIN                            (smoke test, zero deps)
Wave 1:  FORMAT ∥ STRUCTURE                (lynchpins; FORMAT unblocks 9, STRUCTURE 10)
Wave 2:  XRAY ∥ NMR ∥ ENERGY               (after FORMAT+STRUCTURE)
Wave 3:  MOLMEC                            (huge, blocks downstream)
Wave 4:  QSAR ∥ SOLVATION                  (after MOLMEC)
Wave 5:  SCORING                           (after QSAR+SOLVATION)
Wave 6:  DOCKING                           (after SCORING)
Wave 7:  PYTHON / SIP bindings             (bake-off result — gates v2.1)
```

Waves 1, 2, 4 have intra-wave parallelism (independent modules). Each
wave is one or more sub-phases below.

---

## Sub-phase breakdown

| Sub-phase | Scope | Output | Gate |
|---|---|---|---|
| **B0.0** | This doc + Track B kickoff state-of-the-world | this file | — |
| **B0.1** | Wave 0: PLUGIN (3 sources, 0 deps) — smoke-test the BALL_CORE_ONLY removal path | enable PLUGIN in build; +0 new tests; existing K0 sweep green | 24/24 still green |
| **B0.2** | K0.6.4 v1→v2 JSON converter (paired with B0.1 since TextPersistenceManager is in CONCEPT not FORMAT — can ship without FORMAT re-enable) | new `ball-convert-v1-v2-json` tool + corpus round-trip test | converter round-trip green on test corpus |
| **B1.1** | Wave 1a: FORMAT (40 sources) — PDB, HIN, mol2, INI parsers + writers | tests re-enabled: Residue_test1/2, Expression_test, ExpressionParser_test, Selector_test, StandardPredicates_test1..4, StdIteratorWrapper_test. K0 green-bar 24 → ~35 | FORMAT-pulling tests pass |
| **B1.2** | Wave 1b: STRUCTURE (77 sources) — bond perception, rings, fragment DB | tests re-enabled: FragmentDB_test, DefaultProcessors_test. Green-bar → ~37 | STRUCTURE tests pass |
| **B1.3** | Codex Round 10 — Track B Wave 1 review (FORMAT + STRUCTURE integration with K0 kernel) | `K0-CODEX-REVIEW-ROUND10.md` | verdict: go for Wave 2 |
| **B2.1** | Wave 2a: XRAY (3 sources) | XRAY tests | green |
| **B2.2** | Wave 2b: NMR (17 sources) | NMR tests | green |
| **B2.3** | Wave 2c: ENERGY (5 sources) | ENERGY tests | green |
| **B3.1** | Wave 3 cluster: **MOLMEC + QSAR + SCORING-force-field** (revised 2026-05-18 — see decision log §3) | Combined module surface | green |
| **B3.2** | Codex Round 11 — Track B Wave 3 review (MOLMEC + force-field integration) | review doc | verdict: go for Wave 4 |
| **B4.1** | Wave 4: SOLVATION (16 sources) — split from prior Wave 4 since QSAR moved to B3.1 | SOLVATION tests | green |
| **B5.1** | Wave 5: SCORING (47 sources) — remaining SCORING after B3.1 took the FF subset | SCORING tests | green |
| **B6.1** | Wave 6: DOCKING (16 sources) | DOCKING tests | green |
| **B6.2** | Codex Round 12 — Track B full-stack review (FORMAT → DOCKING) | review doc | verdict: go for Wave 7 (PYTHON) gated on bake-off |
| **B7.x** | Wave 7: PYTHON bindings — gated on Phase 6 binding-tool bake-off (`OPEN-QUESTIONS.md` Q1) | bake-off result + chosen tool's binding skeleton | v2.1 work; not v2.0.x |

**Total estimate:** B0 + B1 + B2 ≈ 4 weeks; B3 + B4 + B5 + B6 ≈ 4-6
weeks. Wave 7 timing depends on the bake-off decision (PYBALLV2.md).

---

## Per-wave acceptance gates

Each module re-enable wave is "done" when:

1. The module's source files build cleanly under `BALL_CORE_ONLY=OFF`.
2. The module's `BALL_*_TESTS` group in `test/cmake/BALLTestExecutables.cmake`
   is re-enabled and all its tests are green.
3. Any K0 atom-field-access patterns the module relied on are migrated
   (typical: replace direct `atom->position_` style access — which doesn't
   compile post-K0.3b.LATER deletion — with `atom->getPosition()`).
4. No regression on the K0 24-binary green sweep.
5. The next downstream wave's modules now compile when toggled (proves
   the dependency chain unblocks correctly).

---

## What rides with Track B but isn't a re-enable

- **K0.6.4 v1→v2 JSON converter** — packaged with B0.2 since
  TextPersistenceManager lives in CONCEPT (already core); the converter
  just needs the reader + the new `saveSystemJSON` writer.
- **K0.5 full test surface verification** — the K0.5 selector benchmark
  claimed 61.8× median on a synthetic corpus. After B1.1 (FORMAT) the
  v1.x Selector_test/Expression_test/StandardPredicates_test1..4 become
  buildable; B1.1 closure re-verifies the speedup claim on the v1.x
  test corpus.
- **D13 partial-met re-measurement** — D13 was measured at K0.7.3 on
  100k synthetic orphan atoms (modeled). After B1.2 (STRUCTURE), the
  FragmentDB corpus can be used for a more representative measurement.

---

## Decision log

- **2026-05-18:** Track B sequenced per dependency audit; matches the
  pattern in `MODULE-REENABLE-PLAN.md` with sub-phase boundaries added
  for atomic-commit + per-Codex-round review discipline.
- **B0.2 (v1→v2 converter)** moved earlier than the original spec —
  K0.6.4 was deferred to "Track B FORMAT re-enable" but converter
  actually doesn't need FORMAT (TextPersistenceManager is in CONCEPT).
  Lands paired with PLUGIN smoke-test for parallel work.
- **PYTHON bindings (Wave 7)** depend on the bake-off outcome from
  `OPEN-QUESTIONS.md` Q1. Until that decision lands they're effectively
  out-of-scope.
- **2026-05-18 (§3) — Wave 3 cluster revision:** the original audit
  in `MODULE-REENABLE-PLAN.md` assumed MOLMEC depends only on
  FORMAT+STRUCTURE+core. A B3.1 attempt found that MMFF94, AMBER
  (`GAFFTypeProcessor`), and COMMON/`forceFieldComponent` reach
  forward into QSAR (`RingPerceptionProcessor`, `AromaticityProcessor`)
  and SCORING (`ScoringComponent`). Force-field implementations and
  ring-perception/aromaticity have been a single co-evolved subsystem
  since the early 2000s of BALL development, so they have to land
  together. Wave 3 is therefore **MOLMEC + QSAR + the force-field
  portion of SCORING** as one cluster, then SOLVATION (Wave 4),
  remaining SCORING (Wave 5), DOCKING (Wave 6) follow.
- **Current state (2026-05-18, post B2.3):** K0+B0+B1+B2 complete.
  CORE_ONLY ctest 162/162 PASS (1 disabled = TextPersistenceManager
  PDBInfo round-trip — v2.1 backlog). Modules in:
  CONCEPT, COMMON, DATATYPE, KERNEL, MATHS, SYSTEM (always-on core);
  PLUGIN, FORMAT (subset), STRUCTURE (subset), XRAY, NMR, ENERGY
  (Track B Wave 0–2). Wave 3 cluster pending in a separate session.
