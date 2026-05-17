# Active seeds with v2.x bearing

**Snapshot:** 2026-05-17. Live source is
`/Users/kohlbach/Claude/BALL/ball/.planning/seeds/`.

This is the curated subset of seeds that a v2.x planning session needs to
know about. Resolved or superseded seeds are noted so a fresh session
doesn't re-plan them.

---

## SEED-001 — BALLView Refresh UI/UX modernization

**Status:** `promoted-subset` (since 2026-05-17).

**Original trigger:** Phase 5 (Qt 6 + Pipeline) of the v1.6 milestone
complete — Qt 6.5+ was the hard prerequisite for `QStyleHints::colorScheme()`
(satisfied since v1.6.2 — Qt 6.8 LTS floor).

**Current routing:**
- **v1.7 tail subset (Wave 4, partially landed):** Phases 999.40-999.43.
  Behind `BALL_UI_V2=OFF`. Foundation + maintainer-decision-independent
  subset.
- **v1.8 remainder (planned):** Phases 999.44-999.48. Five architecturally
  heavier UI phases. v1.8 closes with `BALL_UI_V2` default flip ON.
  Documented separately in [V1.8-WAVE.md](V1.8-WAVE.md).
- **v2.x bearing:** v1.9 will delete the Classic 5-dock workspace per
  maintainer-Q2. v2.0 inherits a single Modern layout. SEED-001 itself
  closes when 999.48 ships in v1.8.

**Scope:** Large (originally ~6 person-months for the full 8-phase
Handover package; current split has ~4 weeks already landed in v1.7 tail
+ ~7-10 weeks remaining in v1.8 + a 1.9 delete-Classic phase TBD).

**Breadcrumbs:**
- Seed file: `/Users/kohlbach/Claude/BALL/ball/.planning/seeds/SEED-001-ballview-refresh-ui-milestone.md`
- Handover package: `/Users/kohlbach/Claude/BALL/Claude Design Handover/`
  (`START-HERE.md`, `BALLView UI Audit.html`, `revitalization/` with
  8 phase docs + `09-cross-platform.md` + `10-migration-playbook.md` +
  mockups).
- Integration analysis: `/Users/kohlbach/Claude/BALL/ball/.planning/DESIGN-HANDOVER-INTEGRATION.md`.
- Maintainer answers: `/Users/kohlbach/Claude/BALL/ball/.planning/MAINTAINER-QUESTIONS-999.1.md`
  (copied into [V1.8-WAVE.md](V1.8-WAVE.md) §"Maintainer answers").

**v2.x-relevant carry-over:**
- The `BALL_UI_V2=OFF` codepath becomes dead code post-v1.8. Schedule
  deletion in v2.0 Phase 999.12 (bulk deprecated-code removal).
- Single neutral theme decision (Q3) means `QStyleHints::colorScheme()`
  reactor wiring in ThemeManager stays inactive through v2.x unless a
  later maintainer reverses Q3.

---

## SEED-005-1 — CI aqtinstall modules fixup

**Status:** RESOLVED — closed by Phase 5.1 D2/D3/D6 + commit `54da903`,
verified by CI run [25899905204](https://github.com/BALL-Project/ball/actions/runs/25899905204).

**Why mentioned here:** for completeness — a fresh session should not re-plan
this. The Linux Qt 6 install path is solved.

**Breadcrumb:** `/Users/kohlbach/Claude/BALL/ball/.planning/seeds/SEED-005-1-ci-aqtinstall-modules-fixup.md`.

---

## SEED-005-2 — CI vcpkg baseline fixup

**Status:** RESOLVED — closed by Phase 4 vcpkg manifest + Phase 5.1 Tier-B.

**Why mentioned here:** for completeness — Windows vcpkg path is solved.

**Breadcrumb:** `/Users/kohlbach/Claude/BALL/ball/.planning/seeds/SEED-005-2-ci-vcpkg-baseline-fixup.md`.

---

## SEED-005-3 — Windows VM driver capture

**Status:** SUPERSEDED — user-verified Windows VM render 2026-05-16
(BALLView confirmed working perfectly on a Windows VM since v1.6).

**Why mentioned here:** for completeness — driver-capture-as-documentation is
no longer blocking. The more useful artifact is the post-PIPE-01 capture
(SEED-005-4) which folds into Phase 999.6 v2.0 work.

**Breadcrumb:** `/Users/kohlbach/Claude/BALL/ball/.planning/seeds/SEED-005-3-windows-vm-driver-capture.md`.

---

## SEED-005-4 — Post-PIPE-01 spike recapture

**Status:** `dormant` — folds into Phase 999.6 (v2.0 PIPE-01) when promoted.
NOT a separate phase.

**Trigger condition:** Phase 999.6 PIPE-01 is in flight AND the QRhi backend
has produced a first build that runs on Windows. At that point, recapture
Windows driver behaviour on the new backend to document the QRhi
performance + correctness envelope (vs the GL-compat-profile baseline that
v1.7 Windows VMs verified against).

**Why dormant:** Pre-PIPE-01 there's nothing to capture. The Windows VM
already works on the v1.7 GL fallback — the value of a recapture is the
delta against the v2.0 QRhi backend, which doesn't exist yet.

**Scope:** Small (≤1 day) when Phase 999.6 reaches a Windows build green
state. Mostly: run the existing render smoke check + `BALL_VIEW_GL_DIAG`
oracle on the new backend + diff against the v1.7 baseline + commit the
result to `.planning/phases/999.6-pipe-01-pipeline-rewrite/POST-SPIKE-WINDOWS-CAPTURE.md`.

**Breadcrumbs:**
- `/Users/kohlbach/Claude/BALL/ball/.planning/seeds/SEED-005-4-post-pipe01-spike-recapture.md`
- 999.6 backlog dir: `/Users/kohlbach/Claude/BALL/ball/.planning/phases/999.6-pipe-01-pipeline-rewrite/PIPE-01-BACKLOG.md`.

---

## Seed audit summary for v2.x

After the 2026-05-17 audit:

| Seed | Status | Routing |
|---|---|---|
| SEED-001 | promoted-subset | v1.7 Wave 4 (in-flight) + v1.8 (planned) + v1.9 (delete Classic) |
| SEED-005-1 | resolved | n/a |
| SEED-005-2 | resolved | n/a |
| SEED-005-3 | superseded | n/a |
| SEED-005-4 | dormant | folds into Phase 999.6 v2.0 |

**Net active v2.x seeds:** 1 (SEED-005-4, dormant, folds into 999.6).

A fresh v2.x planning session should re-check
`/Users/kohlbach/Claude/BALL/ball/.planning/seeds/` for new seeds added
after 2026-05-17 — the seed pattern is "plant ahead of time, surface at
the right milestone trigger," so new seeds may have appeared during v1.7
tail-end work.

---

## Other "active seeds" cited by the v1.7 retrospective

The Phase 999.25 retrospective lists exactly five seeds (the four above
plus SEED-001). There are no additional dormant seeds the retrospective
references that haven't been covered above.

If a fresh session finds a new dormant/promoted seed under
`/Users/kohlbach/Claude/BALL/ball/.planning/seeds/` after 2026-05-17,
audit it against this list before assuming it's net-new — the v1.7 cycle
may have planted late seeds during tail-end work (warning census tail,
SignPath procurement, doc-rot follow-ups) that aren't catalogued here yet.
