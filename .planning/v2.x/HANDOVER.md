# BALL v2.x Planning — Handover Package

**Status:** Forward-looking planning bundle. **Authored:** 2026-05-17.
**Branch:** `v2.x-planning-handover`, off `v1.7-modernization` HEAD `a6bfa604ee`.
**Audience:** a fresh parallel session that needs to start v2.x planning work without v1.7 context loaded.

This document is the entry point. Read it first.

---

## 1. What is BALL v2.x?

BALL (Biochemical ALgorithms Library) is a C++ molecular-modelling framework;
BALLView is its Qt/OpenGL viewer. The repository is `BALL-Project/ball`.

The release lineage that matters here:

- **v1.6.x** (shipped) — substrate modernization: CMake bring-up, Qt 6.8 LTS
  port, C++17, Homebrew/apt/vcpkg dep system (replacing dead `ball_contrib`),
  tri-OS CI matrix, fixed-function GL on the compat profile. v1.6.2 was tagged.
- **v1.7** (in tail-end pre-RC) — the modernization release: signed +
  notarized installers (macOS DMG, Windows NSIS via SignPath, Linux
  AppImage/DEB/RPM LGPL-only SKU), Sparkle/WinSparkle auto-update, dead-code
  removal (VRMLRenderer, GENETICDOCK), BALLView Refresh Wave 4 foundation
  (Phases 999.40-999.43, behind `BALL_UI_V2` flag default OFF).
- **v1.8** — the **BALLView Refresh release**. Not a v2.x phase; this is the
  flag-flip milestone where `BALL_UI_V2` defaults ON and the heavier UI
  phases land (Inspector, Workspace, Menus + command palette, Onboarding,
  a11y). Documented separately in [V1.8-WAVE.md](V1.8-WAVE.md).
- **v2.x** — the next-feature horizon. This is what this handover package
  is about. Four point releases:

  | Release | Theme | Lead phase(s) |
  |---|---|---|
  | **v2.0** | Substrate rewrite | 999.24 KERNEL MoleculeStore (LEAD) + 999.6 PIPE-01 renderer rewrite + 999.9 INIFile→YAML + 999.11 in-tree mmCIF→gemmi + 999.12 bulk deprecated-code removal |
  | **v2.1** | Python surface | 6 (PyBALL generator bake-off, FIRST step) + 999.15 PyBALL bulk wrap |
  | **v2.2** | Remote control | 999.10 REST API + PyBALL SDK over MoleculeStore |
  | **v2.3** | Docs portal | 999.13 Doxygen → Read the Docs + Swagger UI |

  Full per-phase scope in [ROADMAP-v2.md](ROADMAP-v2.md).

v1.7 was the modernization; v2.x is what gets built on top of the modern
substrate once v1.7 ships and v1.8 lands the UI refresh.

---

## 2. Branch / commit anchor

This package lives on its own branch so the fresh session can sparse-checkout
just `.planning/v2.x/` without pulling the in-flight v1.7 work.

- **Branch:** `v2.x-planning-handover`
- **Branched from:** `v1.7-modernization` HEAD `a6bfa604ee` ("fix(999.25b/02):
  bounding_box.C — fix latent Representation/Composite type error + document
  out-of-tree build")
- **Worktree:** authored in `/tmp/ball-v2x-handover` (a worktree of the BALL
  repo at `/Users/kohlbach/Claude/BALL/ball`).

If the parallel session needs to re-anchor against v1.7-modernization or
diff against later v1.7 work, the SHA above is the canonical merge-base.

---

## 3. Current shipping state (snapshot 2026-05-17)

- **v1.6.0** — tagged (the v1.6 modernization release)
- **v1.6.1** — tagged (auto-update appcast + Ninja switch + path-aware CI)
- **v1.6.2** — tagged (warning census, GENETICDOCK removal, VRMLRenderer
  removal, ARM Linux baseline, Qt 6.8 LTS bump)
- **v1.7** — NOT YET TAGGED. In tail-end pre-RC. SignPath cert procurement
  externally gated; BALLView Refresh Wave 4 foundation (Phases 999.40-999.43)
  partially landed inline; warning census tail still wrapping up; v1.4 → v1.7
  retrospective authored as Phase 999.25 (2026-05-17). v1.7-modernization
  HEAD is the anchor `a6bfa604ee` above.
- Earlier tags (v1.4, v1.5) **do not exist as git refs**. v1.4 is a
  conceptual baseline only, anchored via CVS `$Id$` keywords inside
  `doc/TUTORIAL/*.tex`. The v1.4 → v1.7 retrospective at
  `/Users/kohlbach/Claude/BALL/ball/.planning/phases/999.25-v1.4-to-v1.7-retrospective/RETROSPECTIVE.md`
  documents this caveat (§7).

---

## 4. Docs in this package

All six docs live under `.planning/v2.x/` in the worktree root. They are
self-contained — every cross-reference out of this subtree is by full path,
so a sparse checkout of just `.planning/v2.x/` works.

| File | Purpose |
|---|---|
| **HANDOVER.md** (this file) | Entry point. Framing, branch anchor, shipping state, where-to-start. |
| **ROADMAP-v2.md** | The v2.0/v2.1/v2.2/v2.3 phase list. Per-phase: goal, why-deferred-from-v1.7, dependencies, scope, source-of-truth pointer. |
| **V1.8-WAVE.md** | The v1.8 BALLView Refresh remainder (Phases 999.44-999.48). Distinct milestone before v2.0 — the `BALL_UI_V2` flag-flip release. Maintainer answers Q1-Q4 copied in. |
| **SEEDS-FORWARD.md** | Active `.planning/seeds/` items with v2.x bearing — current status, trigger condition, scope, breadcrumbs. |
| **OPEN-QUESTIONS.md** | Pre-staged maintainer decisions that need resolution BEFORE v2.x phase planning can finish. Same pattern as the v1.6→v1.7 transition (MAINTAINER-QUESTIONS-999.1.md). |
| **GAPS-FROM-V1.7.md** | Doc-tree and process gaps surfaced during v1.7 that v2.x should fix early. |

---

## 5. Where to start (fresh-session recipe)

1. **Read [ROADMAP-v2.md](ROADMAP-v2.md)** first. That's the v2.x phase
   inventory; it sets the shape of the work.
2. **Read [SEEDS-FORWARD.md](SEEDS-FORWARD.md)** next. Confirms which active
   seeds map to which v2.x phase, and which seeds are already resolved /
   superseded so you don't re-plan them.
3. **Read [V1.8-WAVE.md](V1.8-WAVE.md)** if you've been asked to do anything
   touching the UI. v1.8 is upstream of v2.0; the Refresh phases must close
   before v2.0 starts in earnest.
4. **Skim [OPEN-QUESTIONS.md](OPEN-QUESTIONS.md)** before drafting any v2.x
   plan. Several of those questions gate the lead phases (999.24 KERNEL,
   999.6 PIPE-01, 999.15 PyBALL).
5. **Skim [GAPS-FROM-V1.7.md](GAPS-FROM-V1.7.md)** before doing any doc work
   — there's a queue of doc-tree gaps that v2.x should clean up rather than
   inherit.

Once a v2.x phase is selected for promotion, the per-phase source-of-truth
docs are in the **main repo** (NOT this branch) under:
- `/Users/kohlbach/Claude/BALL/ball/.planning/KERNELV2.md` — 554-line v2.0
  lead phase plan (post-Codex Round 3).
- `/Users/kohlbach/Claude/BALL/ball/.planning/PYBALLV2.md` — v2.1 PyBALL
  rewrite plan (post-CODEX rework v2).
- `/Users/kohlbach/Claude/BALL/ball/.planning/ROADMAP.md` — 2083-line live
  source. Per-phase entries for 999.6, 999.9, 999.10, 999.11, 999.12,
  999.13, 999.15, 999.24, 999.40-999.48.
- `/Users/kohlbach/Claude/BALL/ball/.planning/phases/999.25-v1.4-to-v1.7-retrospective/RETROSPECTIVE.md`
  — the v1.7 retrospective with the "v2.0+ deferred items" table that
  seeds [ROADMAP-v2.md](ROADMAP-v2.md).

---

## 6. Operating contract (carry-over from v1.6/v1.7)

The same GSD phase model + operating discipline that ran v1.6 modernization
and v1.7 should run v2.x. Key carry-overs:

- **Phase model:** every v2.x phase gets a directory under
  `.planning/phases/<phase-dir>/` with `SUMMARY.md` + per-plan
  `<NN>-PLAN.md` artifacts. Backlog promotion via `/gsd-review-backlog`.
- **`BALL_UI_V2` CMake flag:** introduced in Phase 999.40 (v1.7 tail),
  default-OFF in v1.7, **flip to default-ON in v1.8 Phase 999.48**.
  v2.x phases that touch UI assume `BALL_UI_V2=ON` and the Refresh
  Inspector/Workspace/Menu structure (999.44-999.46) is in place.
- **Tri-OS CI matrix:** every v2.x phase must keep CI green on the existing
  matrix (macOS-arm64 + Ubuntu x86_64 + Ubuntu ARM64 + Windows x64).
  Path-aware triggers (Phase 999.18) and concurrency groups already wired.
- **No unsigned releases:** v1.7 established the signing chain
  (Developer ID Application on macOS, SignPath Foundation on Windows,
  Ed25519 appcast keys). v2.x inherits all three; key rotation runbooks
  are in `08a-SECRETS-RUNBOOK.md` / `08b/SECRETS-RUNBOOK.md` /
  `ED25519-KEY-RUNBOOK.md` in the main repo.
- **One PR per phase**, main always shippable, all-platforms-green CI before
  merge. Same as v1.6/v1.7.
- **File conventions** unchanged from `CLAUDE.md` in the main repo: `.h`
  headers + `.C` impl, 2-space indent + Allman braces, `BALL_EXPORT` on
  shared-library classes, `BALL::Exception` hierarchy for errors, Doxygen
  `/** ... */` on public APIs.

---

## 7. Things this package is deliberately NOT

- **Not a v2.x execution plan.** The phase ordering, K0/K1/K2 sequencing
  inside KERNEL v2.0, and the bake-off design for PyBALL are in
  `KERNELV2.md` / `PYBALLV2.md` in the main repo — not here. This package
  points at them; it doesn't reproduce them.
- **Not a replacement for `.planning/ROADMAP.md`.** ROADMAP is the live
  source. This package is a forward-looking snapshot bundled for a fresh
  session that doesn't need 2083 lines of phase history.
- **Not a commitment to versions.** v2.0/v2.1/v2.2/v2.3 sequencing comes
  from 2026-05-16 user direction ("2.0 should start with the kernel");
  see ROADMAP.md §"v2.x release sequence". A new direction can re-shuffle.
- **Not a v1.7 closure document.** The v1.4 → v1.7 retrospective at
  `/Users/kohlbach/Claude/BALL/ball/.planning/phases/999.25-v1.4-to-v1.7-retrospective/RETROSPECTIVE.md`
  is the closure document. This package picks up where that one ends.
