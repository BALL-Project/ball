# Gaps surfaced during v1.7 — what v2.x should fix early

**Status:** Snapshot 2026-05-17. Source: Phase 999.25 retrospective
§5.1 + planning-tree structural audit done while building this handover.

These are doc-tree, tag-history, and per-phase artifact gaps that v1.7
didn't close. A v2.x effort should clean them up before/while spinning up
v2.x phases — not because they block v2.x, but because they're the kind
of thing that compounds: v2.x sub-phases would inherit the same gaps if
the structural shape isn't fixed first.

---

## 1. No `BUILD-windows.md`

**Severity:** Medium.

**What's missing:** `BUILD-macos.md` and `BUILD-linux.md` ship as the
user-facing from-source build guides. The Windows from-source build path
exists — but it lives in `.github/workflows/ci.yml` + `vcpkg.json` + the
Phase 08b SECRETS-RUNBOOK files. There is no single from-source guide
for a downstream user who wants to build BALL on Windows without
deploying a CI runner.

**Why it didn't land in v1.7:** Phase 999.25 (the retrospective) flagged
it as a doc-tree gap. v1.7 packaging work focused on producing the
signed NSIS installer, not on the from-source story. The from-source
story was implicitly "read ci.yml," which is fine for contributors and
unkind to scientific users.

**v2.x fix:** Author `BUILD-windows.md` in the same shape as
`BUILD-macos.md` + `BUILD-linux.md`. Lift content from ci.yml + vcpkg.json
+ 08b runbooks; verify against a clean Windows VM build; cross-link from
the project README. Estimate: 1-2 days.

**When:** As part of v2.0 work — the v2.0 substrate phases will touch the
Windows build path (QRhi D3D11 backend, gemmi vcpkg port, possibly
PocketFFT FFTW substitution); doc the path while it's being touched.

---

## 2. No git `v1.4` / `v1.5` tags

**Severity:** Low (documented caveat, not a blocker).

**What's missing:** The earliest annotated tag in the repo is `v1.6.0`.
"v1.4 era" and "v1.5" are conceptual baselines anchored via CVS `$Id$`
keywords inside `doc/TUTORIAL/*.tex` (e.g. `tutorial2.C,v 1.4 2002/02/27`).

**Why it didn't land in v1.7:** The v1.4/v1.5 era predates this
repository's git history (the repo started ~2022 from a CVS migration);
tagging retroactively would require synthesizing tags against commits
that don't exist in the git log.

**v2.x fix:** Don't try to back-tag. Instead, document the gap in
RELEASE-NOTES-v2.0.md so users searching for v1.4 / v1.5 documentation
find a pointer to the Phase 999.25 retrospective + the v1.4-era CVS
tutorials. This is a documentation-conventions fix, not a tag-graph fix.

**When:** Anytime in v2.x; lowest priority.

---

## 3. No `RELEASE-NOTES-v1.7.md` yet

**Severity:** Low — expected. v1.7 hasn't tagged.

**What's missing:** v1.6.1 and v1.6.2 have RELEASE-NOTES files in
`/Users/kohlbach/Claude/BALL/ball/.planning/` (RELEASE-NOTES-v1.6.1.md,
RELEASE-NOTES-v1.6.2.md). v1.7 doesn't yet.

**Why it didn't land yet:** v1.7 is still in tail-end pre-RC. RELEASE-NOTES
is authored at tag time, not before.

**v2.x bearing:** None directly. But: v2.x release-notes shape inherits
from the v1.6.x precedent. When v1.7 tags + RELEASE-NOTES-v1.7.md lands,
copy its structure for RELEASE-NOTES-v2.0.md / -v2.1.md / etc.

**When:** v1.7 tag closure — not a v2.x action. Just don't start v2.x
work assuming RELEASE-NOTES-v1.7.md exists; check first.

---

## 4. Phases 999.27, 999.28, 999.37 have no per-phase directories

**Severity:** Medium — process consistency.

**What's missing:** ROADMAP.md has entries for Phase 999.27 (Coverage-build
Linux compile-clean), Phase 999.28 (v1.6.2 warning-execution omnibus +
Apple-GL silence), and Phase 999.37 (VRMLRenderer removal). These phases
landed inline as commits but no per-phase directories were created under
`.planning/phases/`. The ROADMAP entries are the only artifacts.

**Why it didn't land in v1.7:** Each phase was small/mechanical enough
that an executor judged a directory + SUMMARY.md was unnecessary overhead.
That's a defensible judgment per-case but it breaks the
"every phase has a `.planning/phases/<phase-dir>/`" invariant the rest of
the tree relies on.

**v2.x fix:** Two options.
1. **Backfill:** create `.planning/phases/999.27-coverage-build-fixes/`,
   `999.28-v162-warning-omnibus/`, `999.37-vrml-removal/` each with a
   SUMMARY.md that summarizes the landed commits. Cheap; restores the
   invariant.
2. **Codify the inline-phase pattern:** add a section to GSD's phase
   model that says "trivial-mechanical phases can land inline; ROADMAP
   entry is the only required artifact." Then this isn't a gap.

**Recommendation:** Option 1 for these three (one-time backfill). Then
document the "inline phase" pattern in GSD if maintainers actually want
the option going forward.

**When:** Early v2.x — fix before any new "inline phase" lands and
multiplies the gap.

---

## 5. No `MILESTONE-CONTEXT-v1.7.md`

**Severity:** Medium.

**What's missing:** `/Users/kohlbach/Claude/BALL/ball/.planning/` has
`MILESTONE-CONTEXT-v1.6.1.md` (the predecessor) but no
`MILESTONE-CONTEXT-v1.7.md`. The v1.7 cycle has plenty of context — it
just isn't in the canonical `MILESTONE-CONTEXT-vX.X.md` slot.

**Why it didn't land in v1.7:** The v1.7 cycle's context is spread across
v1.7-PLAN.md, v1.7-WARNING-CENSUS-PRESEED.md, v1.7-CODEX-REVIEW.md,
DESIGN-HANDOVER-INTEGRATION.md, MAINTAINER-QUESTIONS-999.1.md, and the
Phase 999.25 retrospective. The information exists; it just isn't
consolidated into the predecessor-pattern file.

**v2.x fix:** Author MILESTONE-CONTEXT-v1.7.md at v1.7 tag closure (NOT a
v2.x action item; happens during v1.7 closure). For v2.x:
**author MILESTONE-CONTEXT-v2.0.md early** — at v2.0 cycle open,
not at v2.0 tag closure. The v1.6.1 precedent has the right shape; the
v1.7 cycle should have had one and didn't, so v2.0 should fix the pattern.

**When:** v2.0 cycle kickoff — first artifact authored, before any v2.0
phase planning starts.

---

## 6. Wave-4 stubs: 999.42 / 999.43 planning dirs don't exist yet

**Severity:** Low — phases are in flight.

**What's missing:** 999.40 landed inline (commit `fdeca85b`); 999.41 is
in flight with theme.qrc edits in commit `fcda567a`. 999.42 (QSS theming
+ palette removal) and 999.43 (simple-dialog cleanup) have ROADMAP entries
but no `.planning/phases/999.42-*/` or `.planning/phases/999.43-*/`
directories yet — they'll be created when the phases promote.

**Why it didn't land in v1.7 (yet):** The phases haven't promoted yet
in the v1.7 tail. They will before v1.7 RC. This is a "expected, will
be fixed during v1.7 close" gap, not a v2.x action.

**v2.x bearing:** None. But: if a v2.x planning session sees gaps in the
.42/.43 directories, that's why — not a missing artifact, just an
unpromoted phase.

---

## 7. v1.7-PLAN.md may not survive into v2.x cleanly

**Severity:** Low — process artifact.

**What's the issue:** v1.7-PLAN.md is the v1.7-specific wave structure +
risk register + adversarial-review record. It's a v1.7 artifact. v2.x
should have v2.0-PLAN.md / v2.1-PLAN.md / etc. in the same shape, not
inherit v1.7-PLAN.md.

**v2.x fix:** Author v2.0-PLAN.md as the first deliverable of v2.0 cycle
kickoff (after MILESTONE-CONTEXT-v2.0.md per Gap 5 above). Mirror the
v1.7-PLAN.md structure:
- Wave breakdown (KERNEL K0 → K1 → K2 → ...)
- Risk register
- Adversarial-review record
- "Out of scope" section so deferrals are explicit
- CI matrix gating strategy

Don't try to retrofit v1.7-PLAN.md.

**When:** v2.0 cycle kickoff. Co-author with MILESTONE-CONTEXT-v2.0.md.

---

## 8. Per-phase `PLANS/` vs `<NN>-PLAN.md` inconsistency

**Severity:** Low — convention drift.

**What's the issue:** Most v1.6/v1.7 phases use `<NN>-PLAN.md` files at
the phase-dir root (e.g., `999.18-01-PLAN.md` directly under
`.planning/phases/999.18-path-aware-ci-triggers/`). A few phases use a
`PLANS/` subdirectory (Phase 5 has 8 plans; the 05-NN-*-PLAN.md files
live at the phase-dir root, not under a subdirectory). Both patterns
work; neither is wrong. Just inconsistent.

**v2.x fix:** Pick one and document it. Probably "flat per-phase-dir
with `<phase>-<NN>-PLAN.md` naming" since that's the dominant pattern.
For very-many-plan phases (KERNEL v2.0 K0+K1+K2+K3+K4 will easily exceed
20 plans), allow a `PLANS/` subdirectory with the rule "use it when ≥6
plans accumulate; flat under that threshold."

**When:** Codify before KERNEL v2.0 K0 plans start landing — KERNEL will
multiply the inconsistency.

---

## 9. No v1.7 CI badge / dashboard

**Severity:** Low — visibility.

**What's the issue:** v1.6.x had ad-hoc CI run links in PR descriptions.
v1.7 didn't add a README CI badge for the v1.7-modernization branch. v2.x
shouldn't ship without a current-status badge.

**v2.x fix:** Add README badges for: build status per OS, release status,
docs build status (post-999.13). Lifted from any modern OSS project's
README — purely conventional.

**When:** Early v2.x — README update during v2.0 cycle kickoff is the
right slot.

---

## Summary of v2.x action items from gaps

These are net-new v2.x work items that should fold into v2.0 cycle
kickoff:

1. Author `BUILD-windows.md` (1-2 days, fold into v2.0 substrate work).
2. Backfill phase dirs for 999.27 / 999.28 / 999.37 (1 day, do it before
   any new "inline phase" lands).
3. Author `MILESTONE-CONTEXT-v2.0.md` at v2.0 cycle open (not at close).
4. Author `v2.0-PLAN.md` co-with MILESTONE-CONTEXT-v2.0.md.
5. Codify the `PLANS/` subdirectory convention before KERNEL K0 plans
   land.
6. Add README CI badges + (post-999.13) docs-build badge.
7. (Pre-v2.x) confirm `RELEASE-NOTES-v1.7.md` lands when v1.7 tags;
   don't start v2.x assuming it exists.
8. (v2.x continuous) PocketFFT substitution as the long-term answer to
   the FFTW GPL-SKU owner gap (see [OPEN-QUESTIONS.md](OPEN-QUESTIONS.md) §4).

Estimated total kickoff overhead: ~1 week of doc + structural work
before the first v2.0 phase actually plans.

This is not optional — without these items, v2.x phases inherit the
same shape inconsistencies that v1.7 surfaced, and the next retrospective
(v2.x → v3.0 someday) will flag the same gaps.
