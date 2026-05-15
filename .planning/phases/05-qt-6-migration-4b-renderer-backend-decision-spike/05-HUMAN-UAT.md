---
status: partial
phase: 05-qt-6-migration-4b-renderer-backend-decision-spike
source: [05-VERIFICATION.md]
started: 2026-05-15T12:30:00Z
updated: 2026-05-15T12:30:00Z
---

## Current Test

[awaiting human testing]

## Tests

### 1. Acceptance of CR-01 / CR-02 / CR-03 Qt 6 migration regressions
expected: Architect either (a) flips them into a Phase 5.1 backlog row and closes Phase 5, OR (b) requires a follow-up plan in Phase 5 before close. The three findings are: CR-01 HTMLPage::certificateError dead under Qt 6 (TLS behaviour regression — `ignore_ssl_errors=true` silently rejects); CR-02 QSignalMapper::mapped(int) → mappedInt rename missed at molecularControl.C:498 + PresentaBALLView.C:40 (rotamer menu silently broken; live evidence in Plan 05-06 smoke output); CR-03 QLayout::setMargin still present in pyWidget.C:348+412 (gated by BALL_PYTHON_SUPPORT=OFF — latent compile error for Python-enabled builds). REQUIREMENTS.md QT6-01/QT6-02 rows stay Pending, consistent with these gaps being acknowledged-open.
result: [pending]

### 2. Acceptance of gl_profile=compatibility CI assertion mismatch
expected: Architect signs off on a one-line ci.yml relaxation (gl_profile=(compatibility|none) on macOS) OR confirms it stays as-is until SEED-005-1 + SEED-005-2 land. Empirically: 2 verbatim live macOS captures (M4 Max local + macos-latest software runner) both emit `gl_profile=none` because Apple GL 2.1 doesn't expose Core/Compat distinction. The Plan 05-04 CI assertion at ci.yml:331+387 is dormant (Linux + Windows CI is red upstream of the smoke step) so the mismatch is unobserved today, but will fire RED the moment SEED-005-1/2 land.
result: [pending]

### 3. Acceptance of autonomous override precedent (Plans 05-07 + 05-08)
expected: Architect either signs §6 of 05-SPIKE-DECISION.md ("Accepted by: _pending architect-of-record sign-off_") OR appends a §6.x amendment if disagreeing with the split-pattern decision. The plans declared `autonomous: false` expecting human judgment; execution applied a reasonable-call autonomous override grounded in three reality constraints (spike full-render structurally PIPE-01-blocked, no Windows hardware, Plan 08 phase-closeout gated on Plan 07 artifact). Decision authored: "GL-Core for v1.6.x → QRhi for v2" (split pattern). Documented in both SUMMARYs.
result: [pending]

## Summary

total: 3
passed: 0
issues: 0
pending: 3
skipped: 0
blocked: 0

## Gaps
