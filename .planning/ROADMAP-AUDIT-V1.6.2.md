# Roadmap audit — v1.6.2 milestone scope + overall ROADMAP cross-check (v3 — post-Codex-v2 rework)

**Authored:** 2026-05-16
**Author:** Claude (Opus 4.7, 1M context); cross-checked with Codex CLI (v1 REJECT-AND-REWORK → v2 APPROVE-WITH-FIXES → v3 below).
**Scope:** find items deferred to v1.6.2 (or "v1.6.x housekeeping") that never made it into the backlog; check inflight work for tracking gaps; review overall ROADMAP for stale/duplicated entries.

> **v1 → v2 → v3 changes (post-Codex):**
> - v1 claimed "6 forgotten items" — 2 were false positives (ContourSurface bug; B3 baseline already done), 2 were tracking-fixes-not-backlog (Phase 9 row; glwidget debug doc).
> - v2 added Phase 5 verification regressions CR-01/CR-02/CR-03 as new §A1 (Phase 999.21) based on `05-VERIFICATION.md`. **Codex flagged this as a TIER-1 blocker: CR-01/02/03 are ALL fixed in current code** — `05-VERIFICATION.md` is itself stale, current state lives in `05-HUMAN-UAT.md` + the code. v2 also overstated 999.7 remaining scope (`gl_profile` work also landed in commit `7b28685`).
> - v3 removes the false-positive §A1, narrows §C3 (999.7 scope), narrows §A5 Bison scope to CIF specifically (the other 7 parsers may need audit but evidence is CIF-only), and adds a new §D5 doc-hygiene item: **reconcile Phase 5 verification docs against current code** — both v1 and v2 of this audit were misled by stale planning docs, which is its own warning sign.

---

## TL;DR

**2 real source-level v1.6.x candidates are missing from ROADMAP backlog, plus 5 hygiene fixes for stale entries.** Currently slated for v1.6.2: 999.16-999.20 (five build-acceleration / CI-hygiene phases). The recommended v1.6.2 shape stays a **strict patch release** — the Phase 5 Qt 6 migration is actually fully resolved (commit `7b28685` fix-sweep), Phase 5.1 carry-forwards are all closed, and the only remaining source-level work that genuinely belongs in v1.6.2 is: focused PR-bundle merge via 999.14 (no new Phase 5.2), a **Tier-C census phase** producing a v1.7 split-list rather than fixing all ~3700, the DockResultFile QtXml cleanup, and a **CIF parser** Bison grammar audit.

| Codex flag (across v1+v2 reviews) | v3 disposition |
|---|---|
| v1 A4 ContourSurface bug | **REMOVED** — bug doesn't exist; implementation at [contourSurface.h:302](../include/BALL/DATATYPE/contourSurface.h) is correct. `CONCERNS.md` is stale (§D1). |
| v1 A5 B3 baseline | **REMOVED** — already recorded in [`05.1-08-SUMMARY.md`](phases/05.1-build-warnings-and-latent-bugs/05.1-08-SUMMARY.md) and [`05.1-UAT.md`](phases/05.1-build-warnings-and-latent-bugs/05.1-UAT.md) (3495 → 0 on CI run [25953405453](https://github.com/BALL-Project/ball/actions/runs/25953405453)). ROADMAP progress-table cell is stale (§C4). |
| v1 A3 Phase 9 | **RECLASSIFIED** as progress-table-fix (§D2), not new backlog. ROADMAP `### Phase 9` detail section already records partial progress; only the table row is stale. |
| v1 A6 glwidget | **RECLASSIFIED** as debug-session reconciliation (§D4), debug-doc cleanup only — not tied to Phase 5 CR-01/02/03 since those are also resolved. |
| **v2 §A1 (Phase 999.21 Qt 6 regressions CR-01/02/03)** | **REMOVED** — all three already fixed in current code (HTMLPage.C:36 has `connect(this, &QWebEnginePage::certificateError, ...)`; molecularControl.C:500 and PresentaBALLView.C:41 use `&QSignalMapper::mappedInt`; pyWidget.C:349,414 use `setContentsMargins`). v2 was misled by stale `05-VERIFICATION.md`; `05-HUMAN-UAT.md` is the current source of truth. |
| **v2 §C3 (999.7 gl_profile work)** | **NARROWED** — `gl_profile=(compatibility|none)` relaxation also landed in commit `7b28685`. Only Windows driver capture (and optional post-PIPE recapture) actually remain. |
| **v2 §A5 Bison scope** | **NARROWED** to CIF parser (3 conflicts per `05.1-BACKLOG.md:196`, 5 conflicts per `05.1-05-SUMMARY.md:85` — count needs reconciliation). 7 other Bison grammars exist in tree but no audit evidence yet; leave unscoped. |
| v1 §F open questions | **REPLACED** with decisions made in v2 §A/B/D. |

---

## A. Forgotten v1.6.2 backlog items (verified, scope-corrected)

### A1. Phase 5.1 deferred warning census & split (Tier-C + residual C4910/C4834) — **scope-corrected per Codex**

- **Cited as v1.6.2 OR v1.7 deferral in:** [`MILESTONE-CONTEXT.md:230`](MILESTONE-CONTEXT.md) (note: "v1.6.2 **or v1.7**", not v1.6.2-only — v1 of this audit overclaimed); [`05.1-CONTEXT.md:86`](phases/05.1-build-warnings-and-latent-bugs/05.1-CONTEXT.md) decision D-04; [`RELEASE-NOTES-v1.6.1.md:102`](RELEASE-NOTES-v1.6.1.md).
- **Specifics:**
  - **Tier-C styling warnings: ~3700** (`-Wdeprecated-copy`, `-Wunqualified-std-cast-call`, `-Wcatch-value`) on macOS-arm64 + Linux-x64.
  - **Residual Windows warnings post-Phase-5.1:** **C4910: 307** instances, **C4834: 9** instances — documented in [`05.1-UAT.md`](phases/05.1-build-warnings-and-latent-bugs/05.1-UAT.md:65) but never given a backlog entry.
- **Why "census and split", not "clean all":** wholesale ~3700-warning cleanup is **not patch-release shape**. Per Codex correction: the right v1.6.2 deliverable is a **census + decision document** that splits the warnings into (a) mechanical-fix-now-in-v1.6.2 (e.g., `-Wdeprecated-copy` cases that need `= default`), (b) defer-to-v2.0 because Phase 999.6 PIPE-01 renderer rewrite makes them disappear (fixed-function GL deprecation noise), (c) defer-to-v1.7 because they need real code-review per site. The "clean now" subset should be small enough to fit a patch release.
- **Recommendation:** Add as **Phase 999.22** (v1.6.2) — single PLAN with two phases of work: (1) census-and-categorize task producing `.planning/phases/999.22-warning-census/CENSUS.md`; (2) execute the (a) subset only. Carry (b) into a v2.0 dependency on 999.6; carry (c) into a v1.7 backlog stub. Also include C4910/C4834 cleanup in the (a) subset. **Estimated effort:** census 2-3 days; (a)-subset cleanup TBD by census but should cap at 1 week. If (a) subset exceeds 1 week, defer to v1.7.

### A2. Phase 9 — Test Suite Triage (CLOSE, not redo)

- **Codex correction:** v1 mis-classified as backlog gap. Phase 9 detail section in [`ROADMAP.md:241`](ROADMAP.md) already records partial progress. The actual gap is the Progress table row at [`ROADMAP.md:266`](ROADMAP.md) which says `0/0 | Not started`.
- **Work landed:** commits [`b2bb718`](https://github.com/BALL-Project/ball/commit/b2bb718) (CI wiring, 2026-05-15) + [`61bf5a7`](https://github.com/BALL-Project/ball/commit/61bf5a7) (coverage + PR reporter, 2026-05-15); [`PHASE-9-BASELINE.md`](phases/09-test-suite-triage/PHASE-9-BASELINE.md) + JUnit baselines committed.
- **Work remaining (the actual v1.6.2 phase):** triage the 3 baseline failures from PHASE-9-BASELINE.md (`Directory_test` macOS path; `AmberFF_test` 2.6% energy off; `AssignBondOrderProcessor_test2` fine-penalty assertion); flip CI gatekeeper from `continue-on-error: true` to blocking once stable.
- **Recommendation:**
  - **Hygiene fix (§D3):** update ROADMAP progress-table row to reflect partial completion (CI wiring done; 3-failure triage remaining).
  - **Backlog promotion:** Add as **Phase 9 active** (already in active list) — write [`.planning/phases/09-test-suite-triage/09-CONTEXT.md`](phases/09-test-suite-triage/) capturing the 3 specific failures + remaining triage work. Promote with `/gsd-plan-phase 9` to produce a PLAN. Target v1.6.2.

### A3. Phase 5.1 BLOCKER-A2 — DockResultFile QtXml cleanup (deferred, never picked up)

- **Cited in:** [`phases/05-qt-6-migration-4b-renderer-backend-decision-spike/deferred-items.md:44`](phases/05-qt-6-migration-4b-renderer-backend-decision-spike/deferred-items.md).
- **Specifics:** Qt 5 SAX overload `DockResultFile::attributesToHashMap(const QXmlAttributes&)` is stubbed under `#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)` (commit `3691232`). All current BALL readers use `QXmlStreamReader`. The stubbed overload is dead code that should be formally retired from the header signature. **Suggested home was Phase 5.1; never landed.**
- **Recommendation:** Add as **Phase 999.21** (v1.6.2) — single-task PLAN: drop the dead `QXmlAttributes` overload from `DockResultFile.h`, remove the stubbed implementation, clean up the include. **Estimated effort:** 1 hour. Smallest item on the list — fold into 999.22 (warning census) commit if you want fewer phases.

### A4. Bison `-Wconflicts-sr` grammar audit (CIF parser specifically; deferred from Phase 5.1)

- **Cited in:** [`05.1-CONTEXT.md:61`](phases/05.1-build-warnings-and-latent-bugs/05.1-CONTEXT.md) as explicitly deferred from Phase 5.1 scope ("Bison `-Wconflicts-sr` grammar conflicts (CIF parser) — separate investigation"); also [`05.1-BACKLOG.md:196`](phases/05.1-build-warnings-and-latent-bugs/05.1-BACKLOG.md): "Shift-reduce conflicts in CIF grammar — real ambiguities worth investigating."
- **Specifics (corrected per Codex):** Scope is **CIF parser only** based on documented evidence — [`source/FORMAT/CIFParserParser.y`](../source/FORMAT/CIFParserParser.y). Count needs reconciliation: `05.1-BACKLOG.md:196` says **3 conflicts**, `05.1-05-SUMMARY.md:85` says **5 conflicts**. Pick one ground truth (probably the latter; it's the per-plan SUMMARY post-edit count) and document.
- **Other Bison grammars in tree** (parsedFunctionParser.y, GAMESSDatParserParser.y, GAMESSLogParserParser.y, smartsParserParser.y, smilesParserParser.y, expressionParserParser.y, GAFFCESParserParser.y) — no documented audit; may or may not have their own conflicts. Out of scope for v1.6.2; **could spin out a separate v1.7 backlog stub** if evidence surfaces of conflicts in any of them.
- **Why v1.6.2 (could defer to v1.7):** isolated grammar work; one parser; no architectural dependency. Cheap to dispatch.
- **Recommendation:** Add as **Phase 999.23** (v1.6.2 OR v1.7). Per-conflict triage: confirm benign (default-precedence-resolves-correctly) or fix grammar rule. **Estimated effort:** 1-2 days. Defer to v1.7 if v1.6.2 fills up — not patch-release-critical.

---

## B. PR triage decision (replaces v1's split-into-Phase-5.2-vs-999.14 question)

**Decision (per Codex):** **do NOT create a separate Phase 5.2.** Use **Phase 999.14** as the canonical PR + issue triage phase, with a named "5-PR legacy bundle" subtask covering [#640](https://github.com/BALL-Project/ball/pull/640) FindXDR, [#600](https://github.com/BALL-Project/ball/pull/600) Travis-CI close, [#554](https://github.com/BALL-Project/ball/pull/554) Omega torsion, [#550](https://github.com/BALL-Project/ball/pull/550) hydroxyproline AA, [#546](https://github.com/BALL-Project/ball/pull/546) residue insertion code.

**Why no Phase 5.2:** Codex's reasoning — splitting triage machinery across two phases creates duplicate scaffolding (per-phase audit doc, decision rationale doc, separate "merged-vs-closed" tracking) that has no payoff unless we're actually committing to write merge code right now. The 5-PR bundle is conditional on triage outcomes — Plan A (#640) might close-as-obsolete (XDR may be gone); Plan B (#600) is .travis.yml deletion + maybe a cherry-pick; Plan C/D are real code rebases. If after triage the 5-PR bundle resolves to 2-3 close-as-obsolete + 1-2 small cherry-picks, that fits inside 999.14 cleanly. If it turns out to need 3+ days of real rebase work, **then** spin out Phase 5.2 at that point.

**Recommendation:** 
- Update 999.14 entry to add a named subtask "Legacy PR bundle (#640, #600, #554, #550, #546)" with explicit Plan A/B/C/D notes from `MILESTONE-CONTEXT.md:270-279`.
- Close [Phase 999.5 (Open-PR triage)](ROADMAP.md) as superseded-by-999.14 (cosmetic — 999.5 already says 999.14 supersedes it).
- Promote 999.14 to v1.6.2 (currently labeled "v1.6.x housekeeping" — make explicit).

---

## C. Roadmap hygiene — overall (not specific to v1.6.2)

### C1. Phase 999.3 (Networking rework) — KEEP-WITH-CONDITION (not close)

- Codex correction: don't close 999.3 yet. Survival condition is preserved in [`ROADMAP.md:345`](ROADMAP.md): "if 999.10 is itself deferred past v2.0 AND BALL still needs the TCP server for legacy clients."
- **Recommendation:** Leave 999.3 as-is. When 999.10 is promoted to active for v2.0, **then** close 999.3 as superseded. Add a TODO in the v2.0 milestone-context (when written): "Close 999.3 as superseded once 999.10 promotes."

### C2. Phase 999.4 (Config color-defaults) — CLOSE as promoted/completed

- 999.4 was promoted to active Phase 4.1 and Phase 4.1 is COMPLETE (2026-05-15). The 999.4 entry still says "PROMOTED to Phase 4.1" without "COMPLETED" — should mark closed.
- **Recommendation:** Edit 999.4 entry header to `### Phase 999.4: Config shadows compiled color defaults — COMPLETED via Phase 4.1 (2026-05-15)`. Trivial.

### C3. Phase 999.7 (Qt 6 Linux + Windows Bring-Up) — NARROW SCOPE; build-system AND gl_profile both closed

- v1 overstated closure; v2 over-corrected by listing gl_profile as still-open. Codex v3 correction: macOS `gl_profile=(compatibility|none)` relaxation **also landed in commit `7b28685`** (per [`05-HUMAN-UAT.md:23`](phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-HUMAN-UAT.md)).
- **Actually still open:** Windows driver-behaviour capture (only); optional post-PIPE-01 recapture once Phase 999.6 renderer rewrite lands.
- **Recommendation:** Edit 999.7 entry to scope-narrow to Windows driver capture only, with a note that all the build-system and macOS gl_profile work was closed by Phases 4/5/5.1/999.2 + commit `7b28685`. The remaining scope is small enough to either (a) keep as 999.7 dormant until post-PIPE-01 v2.0, or (b) fold into a v1.6.2 task if Windows driver capture matters for the patch release. Recommend (a) — Windows driver capture is a documentation deliverable, not a code fix, and post-PIPE-01 recapture is the more useful version.

### C4. Phase 5.1 progress-table cell — UPDATE B3 carry-forward to RESOLVED

- ROADMAP Progress table for Phase 5.1 says "Carry-forward: B3 baseline measurement on next clean tri-OS CI run." That carry-forward was completed on CI run 25953405453 (Phase 999.2 verification). 
- **Recommendation:** Edit ROADMAP Progress table line for Phase 5.1 to remove the carry-forward note OR mark it explicitly resolved: "Carry-forward (RESOLVED 2026-05-16): B3 baseline 3495 → 0 on CI run 25953405453."

---

## D. Doc-hygiene fixes (no new backlog needed)

### D1. CONCERNS.md has a stale "Critical Bug" claim

- [`.planning/codebase/CONCERNS.md:40-46`](codebase/CONCERNS.md) "ContourSurface Operator== Latent Bug" describes a bug that does not exist in current code. [`contourSurface.h:302-306`](../include/BALL/DATATYPE/contourSurface.h) implementation is correct: `return ((threshold_ == data.threshold_) && Surface::operator == (data));`.
- **Recommendation:** Remove the entry from CONCERNS.md (or replace with: "Resolved before audit, ~2024. Header declaration at line 107, implementation at line 302-306, both correct."). Trivial. **Action: I can do this in the audit-followup commit.**

### D2. ROADMAP Progress-table cell for Phase 9 is stale

- ROADMAP progress-table line 266: `9. Test Suite Triage | 0/0 | Not started | -`. Phase 9 detail section (line 241) records partial progress.
- **Recommendation:** Update to: `9. Test Suite Triage | (partial) | CI wiring + coverage + JUnit reporter landed (commits b2bb718, 61bf5a7, 2026-05-15); 3 baseline failures pending triage (PHASE-9-BASELINE.md). Target close: v1.6.2. | (in progress)`.

### D3. Same as D2 but for the "Phase 9 row vs Phase 9 detail" mismatch — included above in §A3 / §D2; merge.

### D4. glwidget-port-misprojection debug session — reconcile-or-close

- Status `awaiting_human_verify` since 2026-05-14; last touched 2026-05-15 17:04. Phase 5 verification notes the GL render path was rewritten (Plan 05-05/06 GL-core spike). The original Phase 2 bug (renderToBuffer outside paintGL) was fixed; the debug doc's next_action is "user visual recheck of the three symptoms."
- **Recommendation:** Either (a) the user has visually verified — flip status to `resolved`, move file to `.planning/debug/resolved/glwidget-port-misprojection.md` (create the dir; it doesn't exist), OR (b) it's still awaiting — schedule a 30-min headless smoke check on macOS to confirm the three symptoms (mis-projected cylinders, off-viewport, 0×0 startup error). Either way: 30-60 min of cleanup, NOT a v1.6.2 backlog item.

### D5. Phase 5 verification docs are stale — reconcile against current code

- **The recurring problem in this audit:** v1 was misled by stale `CONCERNS.md` (ContourSurface bug). v2 was misled by stale `05-VERIFICATION.md` (CR-01/02/03). Both planning docs claim issues that the source code resolved later.
- **Specifics:** [`05-VERIFICATION.md:117-130`](phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-VERIFICATION.md) lists CR-01, CR-02, CR-03 as `✗ FAIL (regression unresolved)` / `⚠ LATENT` — but [`05-HUMAN-UAT.md:23-65`](phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-HUMAN-UAT.md) records all three as "fixed inline" via commit fix-sweep (`fb0f7ed`..`1271a2a` + `7b28685` for `gl_profile`).
- **Why this matters:** future audits, future agents, and future contributors will hit the same trap. `05-VERIFICATION.md` (snapshot at phase close) and `05-HUMAN-UAT.md` (rolling resolution log) disagree, and the resolved status lives in the rolling log — but the older verification doc is the one referenced from the phase header.
- **Recommendation:** Add an explicit "Updates" section to `05-VERIFICATION.md` (or a "Resolution log" link at the top) pointing readers at `05-HUMAN-UAT.md` for current state. Apply the same pattern to any other VERIFICATION.md that has resolution-via-HUMAN-UAT.md. **Estimated effort:** 15 min for the Phase 5 reconciliation. Could be done by anyone touching the audit-followup commit.

---

## E. Workflow gap: v1.6.2 MILESTONE-CONTEXT.md doesn't exist

- v1.6.1 has [`MILESTONE-CONTEXT.md`](MILESTONE-CONTEXT.md). v1.6.2 has 5 currently-slated backlog phases + the §A items pending promotion — but no scope doc.
- **Recommendation:** Once v1.6.1 tags:
  1. Rename current `MILESTONE-CONTEXT.md` to `MILESTONE-CONTEXT-v1.6.1.md` (preserve history).
  2. Create new `MILESTONE-CONTEXT.md` for v1.6.2 capturing:
     - Goal: build-acceleration cluster + Phase 9 close + scoped PR triage + dead-code cleanups (the items deferred from v1.6.1, post-audit-corrected — note Qt 6 regressions CR-01/02/03 are NOT in scope, they are already fixed in current code per §D5 reconciliation).
     - Recommended v1.6.2 phase list per §F below.
     - Release criteria (model after v1.6.1's 8 criteria).
     - Out-of-scope deferrals to v1.7 / v2.0.
  3. Pre-flight before `/gsd-new-milestone v1.6.2` (which short-circuits its requirements-gathering when MILESTONE-CONTEXT.md is present).

---

## F. Recommended v1.6.2 final shape (audit-decided, not user-deferred)

If §A items land per recommendations, v1.6.2 becomes:

| Phase | Type | Source | Effort |
|-------|------|--------|--------|
| **999.2 — Ninja generator switch** | Build accel | ✅ DONE (2026-05-16) | — |
| **999.16 — PCH BALL+VIEW** | Build accel | Existing backlog | 0.5-1 day |
| **999.17 — Windows CMake build-tree cache** | Build accel | Existing backlog | 0.5-1 day |
| **999.18 — Path-aware CI triggers** | CI hygiene | Existing backlog | 0.5 day |
| **999.19 — Per-TU build profiling** | CI hygiene | Existing backlog | 0.5 day |
| **999.20 — Action artifact pins** | CI hygiene | Existing backlog | 0.5 day |
| **Phase 9 (close)** | Source-level | §A2 | 2-3 days (3 baseline failures + flip gatekeeper) |
| **999.14 — Issue/PR triage + 5-PR bundle** | Cleanup | §B | 2-5 days (depends on triage outcome) |
| **999.21 — DockResultFile QtXml cleanup** | Source-level | §A3 (NEW) | 1 hour |
| **(STRETCH) 999.22 — Warning census + (a)-subset** | Source-level | §A1 (NEW; census-and-decide, not wholesale) | 3-5 days |
| **(STRETCH) 999.23 — CIF Bison grammar audit** | Source-level | §A4 (NEW); could defer to v1.7 | 1-2 days |

**Total core effort:** ~7-11 days for build-accel cluster + Phase 9 close + focused PR triage + DockResultFile cleanup.
**Plus stretch:** ~4-7 days for warning census + Bison audit if v1.6.2 capacity allows.

This is **patch-release-shape**. The build-acceleration cluster is small individual items (most 0.5-1 day). Phase 9 close is targeted triage of 3 known failures. PR bundle is 5 specific PRs. DockResultFile is a 1-hour dead-code removal.

**What's NOT in v1.6.2:**
- Tier-C wholesale cleanup (~3700 warnings) → recast as census+split, only the (a)-subset lands in v1.6.2
- Phase 8 (full notarization + signed installers) → stays whole, defers to v1.7
- Qt 6 regressions (CR-01/02/03) → REMOVED, already fixed in current code (`HTMLPage.C:36`, `molecularControl.C:500`, `PresentaBALLView.C:41`, `pyWidget.C:349,414`)
- 999.7 macOS `gl_profile` work → REMOVED, already landed in commit `7b28685`
- ContourSurface bug → REMOVED, doesn't exist
- B3 baseline measurement → DONE, only the ROADMAP cell needs updating
- glwidget debug session → debug-doc cleanup, not v1.6.2 phase

---

## G. Action items (no questions to user; decisions made in audit)

In order, with effort:

1. **Apply §D1**: edit `CONCERNS.md` to remove the false ContourSurface bug claim. — 5 min
2. **Apply §D2**: update ROADMAP Progress table row for Phase 9 (mark partial). — 5 min
3. **Apply §C4**: update ROADMAP Progress table row for Phase 5.1 (mark B3 carry-forward RESOLVED). — 5 min
4. **Apply §C2**: edit Phase 999.4 entry header to mark COMPLETED via Phase 4.1. — 5 min
5. **Apply §C3**: edit Phase 999.7 entry to narrow to Windows driver capture only (note `gl_profile` landed in `7b28685`). — 10 min
6. **Apply §B**: update Phase 999.14 entry to add the named "5-PR legacy bundle" subtask; close Phase 999.5 as superseded. — 15 min
7. **Reconcile §D4**: ask the user whether the glwidget visual recheck has happened; if yes, flip status + move file. If no, schedule a 30-min headless smoke. — 5 min orchestration
8. **Apply §D5**: add an "Updates / Resolution" pointer to `05-VERIFICATION.md` referencing `05-HUMAN-UAT.md` as the rolling-resolution source of truth. — 10 min
9. **Add §A3**: new Phase 999.21 entry (DockResultFile QtXml cleanup). 1 hour of work; pre-promote at any time. — 15 min for the entry
10. **Add §A1**: new Phase 999.22 entry (warning census + (a)-subset). — 20 min
11. **Add §A4**: new Phase 999.23 entry (CIF Bison grammar audit) — could be v1.7 instead. — 15 min
12. **Apply §E**: when v1.6.1 tags, create `MILESTONE-CONTEXT.md` for v1.6.2 capturing the new shape from §F. — 30 min

**Total audit-followup effort:** ~2 hours of edits, single focused commit.

---

## H. Sources

- [`ROADMAP.md`](ROADMAP.md)
- [`MILESTONE-CONTEXT.md`](MILESTONE-CONTEXT.md) (v1.6.1 scope)
- [`REVIEW-v1.6.1.md`](REVIEW-v1.6.1.md)
- [`RELEASE-NOTES-v1.6.1.md`](RELEASE-NOTES-v1.6.1.md)
- [`.planning/codebase/CONCERNS.md`](codebase/CONCERNS.md)
- [`.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-VERIFICATION.md`](phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-VERIFICATION.md)
- [`.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/deferred-items.md`](phases/05-qt-6-migration-4b-renderer-backend-decision-spike/deferred-items.md)
- [`.planning/phases/05.1-build-warnings-and-latent-bugs/05.1-CONTEXT.md`](phases/05.1-build-warnings-and-latent-bugs/05.1-CONTEXT.md)
- [`.planning/phases/05.1-build-warnings-and-latent-bugs/05.1-08-SUMMARY.md`](phases/05.1-build-warnings-and-latent-bugs/05.1-08-SUMMARY.md)
- [`.planning/phases/05.1-build-warnings-and-latent-bugs/05.1-UAT.md`](phases/05.1-build-warnings-and-latent-bugs/05.1-UAT.md)
- [`.planning/phases/09-test-suite-triage/PHASE-9-BASELINE.md`](phases/09-test-suite-triage/PHASE-9-BASELINE.md)
- [`.planning/debug/glwidget-port-misprojection.md`](debug/glwidget-port-misprojection.md)
- Codex CLI adversarial review of v1 (REJECT-AND-REWORK), preserved at `/tmp/codex_audit_output.txt`. Key corrections incorporated.
