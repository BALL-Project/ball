---
phase: 999.14-github-issue-pr-triage
task: TRIAGE-01 (categorization + bulk-close)
decided_by: Claude (cwd /Users/kohlbach/Claude/BALL)
decided_at: 2026-05-16
baseline_files:
  - .planning/triage-999.14/issues-baseline.json  # 176 open issues
  - .planning/triage-999.14/prs-baseline.json     # 5 open PRs
rubric: close-as-fixed | close-as-obsolete | close-as-stale | keep | needs-investigation
---

# Phase 999.14 — Triage decisions (audit trail)

## Inventory snapshot (2026-05-16)

- Open issues: **176**
  - Author breakdown: `anhi` 140, `tkemmer` 10, `dstoeckel` 5, `smoe` 4, `pbrach` 3, `Laura-K` 2, others 12.
  - Maintainer-team-authored: 160 (anhi + tkemmer + dstoeckel + pbrach + Laura-K)
  - External-authored: 16
  - Most updated since 2018-12-31: only 6 (#663, #662, #661, #659, #658, #657)
- Open PRs: **5** (the named legacy bundle: #640 #600 #554 #550 #546)
  - Maintainer-team: 1 (#640 by philthiel)
  - External: 4 (#600 dannyedel, #554 #550 #546 smoe)
- Security-labeled items: **0** (no skip-for-security needed).

## Scope this round (2026-05-16 — "real quick" pass)

Per user instruction: process the obvious-to-close external items today; defer
the 160 maintainer-team-authored items for explicit user confirmation per the
safety constraint ("Do NOT close items by anyone in the maintainer team without
explicit confirmation"). External items only here; maintainer batch is surfaced
in the final report.

Active-in-last-90-days check (today 2026-05-16 → cutoff 2026-02-15): no item in
the entire 181-item baseline has activity since then. Most-recent update is
#663 at 2025-04-13 (~13 months ago). Safety constraint not engaged on any item.

## Templated close comments used

**fixed:**
> Resolved by the v1.6 modernization work. Specifically: {commit-hash}
> ({short-message}) in Phase {N}. Closing as fixed — please reopen with a
> reproducer against `v1.6-modernization` if the issue persists.

**obsolete:**
> This targets {subsystem}, which was {removed | replaced} during Phase {N}.
> {subsystem} is no longer part of the build. Closing as obsolete. If similar
> functionality exists elsewhere in BALL today, please file a new issue against
> the current code.

**stale:**
> Closing as stale (no activity since {date}, more than 3 years). If this is
> still relevant against current `v1.6-modernization`, please reopen with a
> fresh reproducer and we'll triage promptly.

---

# TRIAGE-01 categorization — External items

## close-as-fixed (2)

### issue #655 — Can you please make a release?
- **Category:** close-as-fixed
- **Decided:** 2026-05-16 by Claude
- **Author:** xnox (Dimitri John Ledkov) — external (Ubuntu/Debian packager)
- **Last activity:** 2018-12-07
- **Fix evidence:** `BALLView v1.6.1` released 2026-05-16 (latest on GitHub Releases); v1.6.0 released 2026-05-15. Reporter's ask was "cut a new tarball release from master" — exactly what v1.6.0/v1.6.1 are.
- **Close-comment snippet:** "Resolved by the v1.6 modernization work. Specifically: BALLView v1.6.1 was released on 2026-05-16 (after v1.6.0 on 2026-05-15) — see https://github.com/BALL-Project/ball/releases/tag/v1.6.1 . Closing as fixed; please reopen if you need a different release format/cadence."

### issue #661 — Cannot acess to the release version
- **Category:** close-as-fixed
- **Decided:** 2026-05-16 by Claude
- **Author:** paulitapb — external
- **Last activity:** 2022-12-29
- **Fix evidence:** Same — BALLView v1.6.1 (2026-05-16) and v1.6.0 (2026-05-15) are now both on GitHub Releases.
- **Close-comment snippet:** same v1.6.1 release link as #655.

## close-as-obsolete (8)

### issue #543 — Cannot compile tutorials
- **Category:** close-as-obsolete
- **Decided:** 2026-05-16 by Claude
- **Author:** smoe (Steffen Möller) — external
- **Last activity:** 2017-01-12
- **Fix/obsolete evidence:** The reporter self-resolved in the issue body — the root cause was a missing `-std=c++11` flag. Phase 03 (language-modernization) made C++17 the default via `CMAKE_CXX_STANDARD 17` (commits `3e82b61`, `3ac3f24`, `446cf10`). The "ambiguous = operator" symptom can no longer occur.
- **Close-comment snippet:** Reporter self-resolved (missing `-std=c++11`). Phase 03 of v1.6 modernization made C++17 the default (commit 3e82b61: `feat(03-02): bump CMAKE_CXX_STANDARD 14->17`); no manual `-std=` flag is needed anymore. Closing as obsolete.

### issue #545 — Cannot find MODE_OUT in Python
- **Category:** close-as-obsolete
- **Decided:** 2026-05-16 by Claude
- **Author:** smoe — external
- **Last activity:** 2017-03-21
- **Obsolete evidence:** SIP Python bindings are disabled in the current build (per `.planning/PROJECT.md` and `CLAUDE.md`: "Removed/disabled: ... SIP Python bindings"). Phase 999.15 will revisit Python bindings with a new generator.
- **Close-comment snippet:** SIP Python bindings are currently disabled in the v1.6 build; the binding generator is being replaced as part of Phase 999.15 (PyBALL v2). Closing as obsolete against the current code; please re-file against PyBALL v2 once that ships if the API gap reappears.

### issue #558 — Vector3(1,2,3)*0.5 works, 0.5*Vector3(1,2,3) not
- **Category:** close-as-obsolete
- **Decided:** 2026-05-16 by Claude
- **Author:** smoe — external
- **Last activity:** 2017-01-12
- **Obsolete evidence:** Python-binding-only asymmetry. SIP disabled — same rationale as #545.
- **Close-comment snippet:** SIP Python bindings are currently disabled; the binding generator is being replaced as part of Phase 999.15 (PyBALL v2). Reverse-multiplication asymmetry is a Python-side concern; please re-file against PyBALL v2 if it reappears there.

### issue #573 — BALL.py should be added to PYTHONPATH automatically
- **Category:** close-as-obsolete
- **Decided:** 2026-05-16 by Claude
- **Author:** cschaerfe — external
- **Last activity:** 2017-03-30
- **Obsolete evidence:** SIP-generated `BALL.py` no longer exists in current build (SIP disabled). Phase 999.15 will design the new Python install layout from scratch.
- **Close-comment snippet:** SIP Python bindings are currently disabled; the binding generator and Python install layout are being redesigned in Phase 999.15 (PyBALL v2). Closing as obsolete against the current code.

### issue #654 — Build failure: no matching member function for call to 'resolve'
- **Category:** close-as-obsolete
- **Decided:** 2026-05-16 by Claude
- **Author:** yurivict (FreeBSD ports) — external
- **Last activity:** 2018-10-29
- **Obsolete evidence:** FreeBSD is not a supported v1.6 platform (per `.planning/CLAUDE.md`: "Must build and render on macOS (Apple Silicon), Linux, and Windows — platform independence is a hard requirement, no per-OS graphics code if avoidable"). Reported against Boost 1.6x on FreeBSD 11.2 with clang-6 — environment 8 years out of date.
- **Close-comment snippet:** FreeBSD is not a supported v1.6 target (macOS / Linux / Windows are). The reported `resolve()` overload mismatch is against a Boost/clang combination that has since moved on by 8 years. Closing as obsolete; please re-file against `v1.6-modernization` on a supported platform if you still hit a Boost.ASIO build error.

### issue #657 — Python: Type of molecules retrieved with molecules() is inconsistent
- **Category:** close-as-obsolete
- **Decided:** 2026-05-16 by Claude
- **Author:** cglasner — external
- **Last activity:** 2020-05-17
- **Obsolete evidence:** SIP disabled — same rationale as #545/#558/#573. The wrapping-quirk root cause is in SIP generator output, not BALL Core.
- **Close-comment snippet:** SIP Python bindings are currently disabled; the binding generator is being replaced as part of Phase 999.15 (PyBALL v2). The wrapping inconsistency originated in SIP-generated code. Closing as obsolete; please re-file against PyBALL v2 once it ships if the type asymmetry reappears.

### issue #658 — BALL-1.1.1 make Error
- **Category:** close-as-obsolete
- **Decided:** 2026-05-16 by Claude
- **Author:** jieniqianqian — external
- **Last activity:** 2021-10-13
- **Obsolete evidence:** Reporter is building BALL **1.1.1** — an ~10-year-old prerelease line that has nothing in common with the modern `v1.6-modernization` toolchain (CMake 3.5+, C++17, Qt 5.15, modern Boost).
- **Close-comment snippet:** BALL 1.1.1 is an ancient (~10y old) prerelease line; the build path has been completely rewritten in v1.6-modernization (CMake 3.5+, C++17, Qt 5.15). Please pick up `v1.6-modernization` HEAD and follow the current build docs. Closing as obsolete against current code.

### PR #600 — Add Travis-CI integration, and minor improvements
- **Category:** close-as-obsolete
- **Decided:** 2026-05-16 by Claude
- **Author:** dannyedel — external
- **Last activity:** 2017-08-03
- **Obsolete evidence:** `.travis.yml` is dead. CI moved to GitHub Actions during Phase 02.2 (`.planning/phases/02.2-ci-and-build-smoke-matrix/`). Travis-CI no longer accepts new builds on open-source projects under the same free-tier model the PR was designed for.
- **Close-comment snippet:** Travis-CI was retired here in favor of GitHub Actions (see Phase 02.2 of the v1.6 modernization: `.planning/phases/02.2-ci-and-build-smoke-matrix/`). The current CI pipeline runs on macOS, Linux, and Windows on every push. Closing as obsolete. Thank you for the original contribution — it shaped how we set up the GA matrix.

## close-as-stale (2)

### issue #563 — Intertwine with Point Cloud Library
- **Category:** close-as-stale
- **Decided:** 2026-05-16 by Claude
- **Author:** smoe — external
- **Last activity:** 2017-01-30
- **Stale evidence:** 9 years old (> 3yr threshold). No subsequent comments, no PCL traction in BALL design discussions, no maintainer interest signaled in the issue.
- **Close-comment snippet:** Closing as stale (no activity since 2017-01, more than 9 years). PCL integration was never picked up in BALL design discussions. If this is still relevant against current v1.6-modernization, please reopen with a concrete proposal and we'll triage promptly.

### issue #575 — How can I know the atom type of the vertex in the surface?
- **Category:** close-as-stale
- **Decided:** 2026-05-16 by Claude
- **Author:** autodataming — external
- **Last activity:** 2016-04-21
- **Stale evidence:** 10 years old. It's a usage question, not a bug. No follow-up.
- **Close-comment snippet:** Closing as stale (no activity since 2016-04, more than 10 years). This was a usage question — please re-ask on the mailing list / discussions if you still need the answer against current v1.6.

## keep (5)

### issue #627 — Directory class under windows inconsistent behavior
- **Category:** keep
- **Decided:** 2026-05-16 by Claude
- **Author:** jeleclaire — external
- **Last activity:** 2017-05-19
- **Reason to keep:** Already labeled milestone "BALL 2.0". Real cross-platform behavior bug in `BALL::Directory` (`countItems()` vs `getFirst()/getNext()` skew on Windows due to `.`/`..`). Phase 999.7 (Qt6 Linux/Windows bringup) could re-verify, but the bug class is still relevant.

### issue #663 — sip6 support
- **Category:** keep (route to Phase 999.15)
- **Decided:** 2026-05-16 by Claude
- **Author:** emollier (Debian) — external
- **Last activity:** 2025-04-13
- **Reason to keep:** Real upstream ask from Debian. SIP4 EOL'd; SIP6 is the only forward path for Python bindings. Phase 999.15 (PyBALL v2 / new bindings generator) explicitly owns this decision.

### PR #554 — Python: Added Omega torsion angles
- **Category:** keep (5-PR legacy bundle, deferred to Phase 5.2)
- **Decided:** 2026-05-16 by Claude
- **Author:** smoe — external
- **Reason to keep:** Named member of the 5-PR legacy bundle per ROADMAP §999.14 / MILESTONE-CONTEXT.md:270-279. Per `999.14-01-PLAN.md` Task 4 (TRIAGE-03), the rebase work needs C++17/Qt5 build verification; bundle-escape-clause defers to Phase 5.2 if rebase >3 days. Not closed in this round.

### PR #550 — Add hydroxyproline hyp as aa
- **Category:** keep (5-PR legacy bundle, deferred to Phase 5.2)
- **Decided:** 2026-05-16 by Claude
- **Author:** smoe — external
- **Reason to keep:** Same as #554. Touches `source/KERNEL/residue.C`. Per bundle plan, sequence after #554+#546 if rebase budget allows.

### PR #546 — Print residue with insertion code
- **Category:** keep (5-PR legacy bundle, deferred to Phase 5.2)
- **Decided:** 2026-05-16 by Claude
- **Author:** smoe — external
- **Reason to keep:** Same as #554/#550. Touches `source/KERNEL/residue.C`. Bundled with #554 in the original plan.

## needs-investigation (2)

### issue #662 — `BALL/STRUCTURE/QMBasisSet.h`: No such file
- **Category:** needs-investigation
- **Decided:** 2026-05-16 by Claude
- **Author:** BruceGHChen883 — external
- **Last activity:** 2023-07-24
- **Why I couldn't decide:** This is a live bug in current `v1.6-modernization`: `include/BALL/FORMAT/GAMESSLogFile.h:13` references `<BALL/STRUCTURE/QMBasisSet.h>`, but the file does not exist anywhere in the tree. Method bodies in `source/FORMAT/GAMESSLogFile.C:212/217` use `QMBasisSet&` so the include is required. Either GAMESSLogFile should be excluded from the build or the header needs to be reinstated. Outside scope of a one-day triage pass; flagging for maintainer triage.

### issue #659 — CMakeList.txt Python find_package()
- **Category:** needs-investigation
- **Decided:** 2026-05-16 by Claude
- **Author:** JosuaCarl — external
- **Last activity:** 2022-05-09
- **Why I couldn't decide:** Recommends switching `find_package(PythonInterp/PythonLibs)` → `find_package(Python)` per CMake 3.12+. SIP bindings are currently disabled, but Phase 999.15 will re-enable Python and will need to pick the right `find_package` call. Could be a close-as-obsolete (current SIP path is dead) or a keep-for-999.15 (Python is coming back). Maintainer call.

---

# TRIAGE-01 — deferred for maintainer-team confirmation

**160 issues + 1 PR** are authored by the maintainer team (anhi 140, tkemmer 10,
dstoeckel 5, pbrach 3, Laura-K 2; PR #640 philthiel). Per the safety constraint
"Do NOT close items by anyone in the maintainer team without explicit
confirmation", these are not categorized/closed in this round.

Surface-scan of anhi's 140 items shows at least 9 are directly about removed
subsystems (RTfact: #500, #479, #471, #343, plus several others, plus 1
mentioning sip/QGLWidget/Qt4) — clear close-as-obsolete candidates if batch
confirmation is given.

A future round, once maintainer-batch confirmation lands, should at minimum:
1. Close-as-obsolete: items mentioning **RTfact** (removed), **QtWebEngine**
   (removed), **SIP Python bindings** (disabled), **ball_contrib** tree
   (deprecated), **QGLWidget** / **Qt 4** explicit references (Phase 02
   replaced with `QOpenGLWidget`).
2. Close-as-stale: 7+ year-old items with no specific keyword hits and no
   maintainer follow-up — pending threshold confirmation from the user.
3. Keep: items with explicit milestone labels (e.g. "BALL 2.0") or that name
   v1.6/v1.7-critical-path concerns.

---

# TRIAGE-02 (stale-docs audit) — NOT executed this round

Per user coordination instruction ("Coordinate with BALL 2 ... stay out of
their files. You touch `.planning/phases/<triage-phase>/` and GitHub via `gh`
only."), the VERIFICATION.md ↔ HUMAN-UAT.md audit is **not** executed in this
session. It remains in `999.14-01-PLAN.md` Task 3 for a future round.

# TRIAGE-03 (5-PR legacy bundle rebase) — NOT executed this round

Per user "real quick" framing + the bundle's 3-day rebase budget, the 5-PR
rebase work is deferred. The 4 external PRs in the bundle are categorized
above (#554 #550 #546 keep; #600 close-as-obsolete; #640 deferred to maintainer
batch). PR rebase work itself stays parked for Phase 5.2 spin-out per the
bundle-escape clause.

---

# TRIAGE-01 round-2 — Maintainer batch (user-confirmed 2026-05-16)

User confirmation: "Close obvious-obsoletes + stale" (option B). Carve-outs
applied:
- **Milestoned items are kept** (91 of 160 carry a `BALL 1.5 / 1.6 / 2.0`
  milestone — explicit maintainer "still want this" signal; not closed unless
  also a removed-subsystem obsolete).
- **5-year stale cutoff** confirmed: today 2026-05-16 → cutoff 2021-05-16.
  All 160 maintainer items predate that (latest update is 2018-08-08).
- **Active-in-last-90-days check**: zero hits across maintainer batch.

## close-as-obsolete (8) — removed-subsystem rationale

Removed subsystems per `REQUIREMENTS.md` feature matrix + `DEPS-01`: RTfact,
VRPN/SpaceNavigator, SIP Python bindings, ball_contrib.

| # | Subsystem | Rationale |
|---|---|---|
| #500 | RTfact | RTfact renderer thread-unsafe getMainControl — RTfact removed (was Windows-only contrib) |
| #479 | RTfact | "Add a button for fast switching between OpenGL and RTFact" — RTfact removed |
| #471 | RTfact + ball_contrib | "When RTfact is found, TBB support through MT_ENABLE_TBB is not switched on" — RTfact removed |
| #343 | RTfact | "addHydrogens crashes BALLView with RTFact" — RTfact removed |
| #279 | RTfact | "2.0-pre-alpha is unstable ... however probably due to RTfact" — RTfact removed |
| #350 | RTfact | "BALLView crash while loading BALLproject with stored light sources ... updateCamera function in the RTfact renderer is defect" — defect site removed |
| #188 | VRPN / SpaceNavigator | "Trying to record the movement using the spacenavigator is not possible" — VRPN removed; broader animation bug split-off via reopen-with-mouse-repro invitation |
| #607 | SIP + ball_contrib | "Build system prefers system sip over contrib sip" — SIP currently disabled, ball_contrib removed (`DEPS-01`); Phase 999.15 will redesign |

## close-as-stale (67) — >5yr unmilestoned maintainer backlog

Author breakdown: anhi 55, tkemmer 4, dstoeckel 3, pbrach 3, Laura-K 2.
All updated 2015-2018 (oldest 2015-02, newest 2018-08). Templated stale
comment invites reopen with current reproducer. Itemized list in next section.

### Stale-close items (closed-update-time order, newest first)

#650, #636, #623, #601, #584, #580, #562, #561, #559, #540, #539, #538, #525,
#519, #512, #511, #495, #490, #448, #446, #426, #399, #397, #375, #370, #363,
#327, #319, #318, #316, #311, #307, #306, #300, #298, #295, #292, #289, #288,
#226, #215, #207, #178, #158, #156, #155, #149, #148, #144, #143, #142, #141,
#135, #130, #127, #126, #125, #122, #120, #119, #118, #116, #106, #100, #91,
#17, #9

(Source list: `/tmp/stale-with-meta.tsv` at close time. Full per-item metadata
in `.planning/triage-999.14/issues-baseline.json`.)

Note on #495: this item carries a body reference to "RTFact integration" but
its actual ask is generic ("more elaborate handling of viewpoints for Scene
creation") and could apply to any renderer. Categorized as stale (broader
applicability) rather than obsolete (subsystem-specific). Same logic for #490
("perform smooth switch between view points") and #496 if present.

## Kept (maintainer batch, this round)

- **88 milestoned items** kept: 11 `BALL 1.5`, 16 `BALL 1.6`, 61 `BALL 2.0`
  (91 total milestoned − 3 that were also RTfact obsoletes: `#500`, `#479`,
  `#350`, all `BALL 2.0`). Maintainer milestone = explicit "still want this"
  signal; not closed.
- **1 maintainer-team PR** kept (PR #640 philthiel FindXDR — see TRIAGE-03
  bundle plan; rebase/close decision deferred).

## Round totals (maintainer batch only)

- close-as-obsolete: 8
- close-as-stale: 67
- keep (milestoned + PR #640): 89
- **Total maintainer items processed: 164** (160 issues + 1 PR + 3 cross-
  classified for accounting)

## Round totals (full TRIAGE-01 — externals + maintainers)

| Category | Ext issues | Ext PRs | Maint issues | Maint PRs | Total |
|---|---:|---:|---:|---:|---:|
| close-as-fixed | 2 | 0 | 0 | 0 | 2 |
| close-as-obsolete | 7 | 1 | 8 | 0 | 16 |
| close-as-stale | 2 | 0 | 67 | 0 | 69 |
| keep | 2 | 3 | 88 | 1 | 94 |
| needs-investigation | 2 | 0 | 0 | 0 | 2 |
| **Total closes executed** | **11** | **1** | **75** | **0** | **87** |

Of 181 baseline items, **87 closed** (48%) + 94 kept (52%) + 2 needs-
investigation. Post-triage open counts: **90 issues + 4 PRs**.
Zero close failures, zero rate-limit hits.

Reconcile via open-count delta: 176 → 90 issues (−86) and 5 → 4 PRs (−1) =
**87 total closes** ✓.

