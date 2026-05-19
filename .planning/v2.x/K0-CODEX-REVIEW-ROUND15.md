# Track B Cluster A — Codex CLI Round 15 (2026-05-19)

**Status:** Complete. Verdict: **GO** — no bugs found in Cluster A
re-enable. One planning correction for Cluster B sequencing.

**Reviewer:** Codex CLI 0.128.0 (`codex exec`).
**Subject:** Track B Cluster A (commit `47c7dd079`) — MOLMEC subset +
SCORING/COMMON subset + FORMAT trajectory file re-enable that
brought CORE_ONLY ctest from 164/164 → 174/174.

---

## Verdict (Codex, verbatim)

> Final: **GO** for Cluster A. No [BUG] found in the landed MOLMEC +
> SCORING subset + FORMAT trajectory re-enable. Cluster B ordering
> should explicitly land `SCORING/COMPONENTS` before or with
> AMBER/DOCKING.

## Probe answers

| # | Probe | Verdict |
|---|---|---|
| C-A1 | ScoringComponent inheritance chain | OK — bottoms out in kept scoringComponent.C |
| C-A2 | scoringComponent.C ScoringFunction calls | OK — only getExpEnergyStddev, now #ifdef'd |
| C-A3 | MOLMEC/COMMON/forceField.C force-field-agnostic | OK — manages abstract ForceFieldComponent* |
| C-A4 | MINIMIZATION/MDSIMULATION not coupled to AmberFF | OK — abstract ForceField& everywhere |
| C-A5 | FORMAT trajectory files clean | OK — no DOCKING/QSAR refs |
| C-A6 | SCORING/COMPONENTS not link-needed by kept MOLMEC | OK; only amberNonBonded.C in trimmed AMBER pulls it |
| C-A7 | Test skip list reasonable | OK; could split AmberFF-vs-pure-minimizer later |
| C-A8 | Cluster B sequencing | **CORRECTION**: AMBER needs SCORING/COMPONENTS (amberNonBonded.C includes advElectrostatic.h), not just QSAR |

## Planning correction for Cluster B

The original Cluster B plan (in commit message): "QSAR + DOCKING +
remaining SCORING + AMBER + MMFF94 + ..." needs reordering:

```
Cluster B ordering (revised, per Codex R15):
  B1. STRUCTURE trimmed bits (smartsMatcher, atomTyper, kekulizer,
      ringAnalyser, sdGenerator, rotamerLibrary, sideChainPlacementProcessor)
      — needs QSAR's RingPerceptionProcessor + AromaticityProcessor
  B2. QSAR (50 sources) — depends on FORMAT + STRUCTURE + descriptors,
      NOT on SCORING/COMPONENTS
  B3. SCORING/COMPONENTS (47 sources) — depends on MOLMEC (in Cluster A)
      + ENERGY + QSAR
  B4. SOLVATION — depends on MOLMEC + QSAR
  B5. AMBER + MMFF94 — depend on QSAR for SMARTS, PLUS amberNonBonded.C
      includes SCORING/COMPONENTS/advElectrostatic.h, so SCORING/COMPONENTS
      MUST land before AMBER
  B6. SCORING/FUNCTIONS — depends on QSAR + MOLMEC AMBER/MMFF94
  B7. DOCKING — depends on full SCORING + MOLMEC FFs
```

Updated dependency: AMBER → SCORING/COMPONENTS not just QSAR. Plan
B5 lands AFTER B3, not after just QSAR.

## Conclusion

Cluster A is correct, clean, and committed. 174/174 CORE_ONLY ctest
PASS. No bugs, no regressions, no architectural concerns.

Cluster B is ~7 sub-phases with the revised ordering above.

**Verdict: GO for v2.0.0-rc2 candidate / Cluster B start.**
