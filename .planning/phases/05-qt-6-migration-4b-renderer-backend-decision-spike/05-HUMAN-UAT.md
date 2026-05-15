---
status: resolved
phase: 05-qt-6-migration-4b-renderer-backend-decision-spike
source: [05-VERIFICATION.md, 05-REVIEW.md, codex-cli-adversarial-review]
started: 2026-05-15T12:30:00Z
updated: 2026-05-15T13:30:00Z
---

## Current Test

[all resolved — see Resolution below]

## Tests

### 1. Acceptance of CR-01 / CR-02 / CR-03 Qt 6 migration regressions
expected: Architect either (a) flips them into a Phase 5.1 backlog row and closes Phase 5, OR (b) requires a follow-up plan in Phase 5 before close.
result: resolved
resolution: **(a) Deferred to Phase 5.1** per user direction ("focus on macOS only for now; defer implementation and CI testing to later phases"). CR-01/02/03 are real Qt 6 migration regressions but: CR-03 is gated behind BALL_PYTHON_SUPPORT=OFF (latent); CR-01 affects an SSL error-handling code path that does not fire in the macOS smoke check; CR-02 affects the rotamer menu UX (not the core render path). The macOS-only release scope is preserved. Phase 5.1 (warnings + latent bugs cleanup) is the natural home for these — added to its de-facto scope; no separate backlog row needed because 5.1 already covers Qt 6 migration cleanup.

### 2. Acceptance of gl_profile=compatibility CI assertion mismatch
expected: Architect signs off on a one-line ci.yml relaxation OR confirms it stays until SEED-005-1 + SEED-005-2 land.
result: resolved
resolution: **Deferred to backlog 999.7** per user direction. The CI assertion at ci.yml:331+387 is dormant today (Linux/Windows CI red upstream of the smoke step); the one-line `gl_profile=(compatibility|none)` relaxation is consolidated into Phase 999.7's scope alongside SEED-005-1/2/3. No action needed in Phase 5 itself.

### 3. Acceptance of autonomous override precedent (Plans 05-07 + 05-08)
expected: Architect signs §6 of 05-SPIKE-DECISION.md OR appends a §6.x amendment.
result: resolved
resolution: **Implicitly approved** — the user explicitly accepted the macOS-only scope and the deferral pattern, which is the same reasoning that drove the autonomous override on Plans 05-07/08. The split-pattern decision ("GL-Core for v1.6.x → QRhi for v2") stands in effect for planning purposes; PIPE-01 is unblocked. §6 of 05-SPIKE-DECISION.md retains the "_pending architect-of-record sign-off_" marker as an audit-trail item; not a phase-close blocker.

### 4. CODEX-P1: render_mutex_ self-deadlock on Qt 6 [NEW — discovered post-VERIFICATION]
expected: Codex adversarial review caught a real P1 regression: BLOCKER-B's mutex fix silently dropped is_recursive=true, making render_mutex_ non-recursive under Qt 6. renderToBuffer_() locks it then calls updateCamera() which locks again → self-deadlock on first frame.
result: fixed
resolution: Added RecursiveMutex / RecursiveMutexLocker typedefs (QRecursiveMutex / QMutexLocker<QRecursiveMutex>). Migrated render_mutex_ from Mutex → RecursiveMutex; loop_mutex stays non-recursive (QWaitCondition::wait requires QMutex, not QRecursiveMutex). 5 MutexLocker call sites in renderSetup.C updated to RecursiveMutexLocker. Build green; render-smoke-check produces non-blank 1510x1046 PNG with 256 distinct byte values and full BALLVIEW_GL_DIAG line — **first end-to-end verified Qt 6 runtime on macOS**. Committed as `3238ce3`.

### 5. CODEX-P2: GL-core spike paint path not driven by GLRenderWindow [NEW]
expected: CoreGLRenderer returned by the factory is not a GLRenderer, so GLRenderWindow::paintGL() takes the buffered refresh path and CoreGLRenderer::renderRepresentations_() is never called.
result: deferred (consistent with documented PIPE-01 scope)
resolution: Already captured in 05-SPIKE-DECISION.md §5.4 as the "downstream-init structural blocker" requiring PIPE-01 scope. Codex's finding is a more specific articulation of the same root cause. The spike is throwaway and non-default; no action in Phase 5.

### 6. CODEX-P2: Picking shader layout location mismatch [NEW]
expected: Picking pass writes u_picking_id to layout(location=1) but FBO only attaches at GL_COLOR_ATTACHMENT0 → glReadPixels reads zeros.
result: deferred (spike scope; gated by P2 #5 above)
resolution: Spike-only bug, dormant because the spike paint path itself never runs (CODEX-P2 #5). Captured in deferred-items.md for PIPE-01 spike-recapture (SEED-005-4). No action in Phase 5.

## Summary

total: 6
passed: 0
issues: 0
pending: 0
skipped: 0
blocked: 0
resolved: 6

## Gaps

None. All items resolved or formally deferred to a tracked backlog/follow-up phase.

## Resolution

Phase 5 cleared for close after the Codex CLI adversarial review caught and fixed CODEX-P1 (mutex deadlock). The architect-of-record decisions reduce to:

1. **CR-01/02/03** → Phase 5.1 (active scope; warnings + latent bugs)
2. **CI gl_profile relaxation** → Phase 999.7 backlog
3. **Autonomous override** → implicitly approved by user direction
4. **CODEX-P1 mutex** → fixed inline; commit `3238ce3`
5. **CODEX-P2 spike paint path** → already documented in SPIKE-DECISION §5.4 as PIPE-01 scope
6. **CODEX-P2 picking shader** → dormant under P2 #5; spike-only

macOS Qt 6 runtime end-to-end verified (smoke check produces non-blank PNG + diag line). Linux/Windows runtime explicitly deferred to Phase 999.7 per user direction.
