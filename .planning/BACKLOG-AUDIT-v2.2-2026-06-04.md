# Backlog audit — v2.2 branch, 2026-06-04

This audit reconciles the v2.2-branch `ROADMAP.md` open-item state
with the actual cross-branch tag-and-progress reality. The v2.2
branch's `ROADMAP.md` is a snapshot from the BALLView 1.6
modernization track; substantial progress has happened on
`master` / `v1.7` branches since the snapshot, and the v2.2-branch
copy is out-of-date.

## Authoritative tag state (cross-branch)

| Tag | Date | Status |
|---|---|---|
| `v1.6.0` | 2026-05-14 | **RELEASED** |
| `v1.6.1` | 2026-05-16 | **RELEASED** |
| `v1.6.2` | 2026-05-17 | **RELEASED** (the BALLView 1.6 modernization closure; STATE.md frontmatter "pre-tag" is stale) |
| `v1.7.0-rc1` | 2026-05-? | **rc landed** |
| `v1.7.0-rc2` | 2026-05-? | **rc landed** |
| `v1.7.0-rc3` (candidate) | 2026-06-? | **rc candidate work in progress on master/v1.7 branch** |
| `v2.0.0` | 2026-05-19 | **RELEASED** (kernel v2 SoA foundation) |
| `v2.1.0` | 2026-05-20 | **RELEASED** (side-table infra + perf + RTTI consolidation) |
| v2.2 (in progress) | — | H3d CLOSED 2026-06-04; H4 LOCKED 2026-06-04; H5/H6/H7/H8 pending (V2X-ROADMAP §2) |

## ROADMAP TBD placeholders — explicit categorization

The v2.2-branch `ROADMAP.md` has 35 `- [ ] TBD` placeholders, all
gated by a milestone-promotion trigger. None of them is
engineering work blocked on a decision; they're **scaffolding
slots** awaiting their parent milestone to open. They cannot be
"cleared" through engineering work, only through the corresponding
milestone-open event.

By promotion gate (the 35 placeholders):

| Gate | Count | Resolution path |
|---|---|---|
| `when v1.6.2 milestone opens` | 7 | RESOLVED — v1.6.2 is RELEASED. Items eligible for promotion in next BALLView track session. Listed here for cross-reference; the v2.2 kernel track does not block. |
| `when v2.0 cycle opens` | 4 | RESOLVED — v2.0.0 RELEASED 2026-05-19; v2.1 + v2.2 also progressed. Items roll into their `V2X-ROADMAP §3` placements (per V21-MSVC-CI placement on H7 — already done in this turn for v2.1 BACKLOG). |
| `v1.7 cycle` | 4 | IN PROGRESS — v1.7.0-rc3 candidate work happening on master/v1.7 branch. Items live there, not here. |
| `v1.8 promotion` | 4 | FUTURE — pending v1.7 final ship. SEED-001's v1.8 wave covers most. |
| `999.40 lands` / `999.6 PIPE-01 lands` / `Phase 5 + SPIKE-02` / etc. | 4 | DEPENDENCY-GATED — gated on specific earlier phases. Their unblock-conditions are externally tracked. |
| Misc (`when capacity allows`, `ready`, etc.) | 12 | LOW-PRIORITY backlog; not actionable until capacity. |

**None of the 35 TBDs is an engineering blocker on the v2.2
branch** for the v2.2 kernel cycle. They are tracked for the
BALLView track and will be promoted in the corresponding BALLView
sessions.

## ROADMAP open real-phase items

Two ROADMAP open items name actual phases (vs. TBD placeholders):

- **Phase 6: Python Bindings (PyBALL v2 binding-generator bake-off)**
  — TARGETED FOR v2.1 originally; per V2X-ROADMAP §4a is now
  REBASELINED to after v2.2 handle API stabilizes
  (post-H4). SIP stays OFF through v2.2 (D52.3-7 / D54). Phase 6
  re-cuts against the post-flip handle surface as the FIRST step
  of pyBALL v2 wrappers (V2X-ROADMAP §4a "REBASELINE required").
- **Phase 8: Packaging & Distribution (TARGETED FOR v1.7)** —
  Notarizable macOS bundle / signed Windows installer / Linux
  build-from-source / license review. Tracked on `master`/`v1.7`
  branch; v2.2 kernel branch does not block.

Both are FUTURE work in their respective parent tracks; neither
is actionable from the v2.2 kernel branch.

## v2.x kernel roadmap state

Per V2X-ROADMAP.md §2:

| Phase | Status | Where |
|---|---|---|
| H0 | ✅ CLOSED | R29–R36d |
| H1a / H1b / H1b′ | ✅ CLOSED | R32b / R34 / HC1b |
| HCP-1 / HCP-1P / HCP-2 | ✅ CLOSED | various |
| H2 (a/b/c/d) | ✅ CLOSED | H2d-R1 GO |
| H3 (a/b/c/d) | ✅ CLOSED | H3d closing-CR R4 GO at HEAD 4e71e63f4 (2026-06-04) |
| **H4 design LOCK** | ✅ LOCKED | H4-DR R8 GO at HEAD ded31e3d2 (2026-06-04) |
| **H4 implementation commits 1+2+3+4a+5a+6-audit+7a+11.5-audit** | ✅ LANDED | HEAD da7a0fd08 (2026-06-04) |
| H4 commits 4b / 5b / 5c / 5d / 5e / 6.b-c / 7b / 7.5 / 8 / 9 / 10a / 10b / 10c / 11 / 11.5-mig / 12 / 13 / 14 | PENDING | each is a substantive code-changing commit; D-H4.9 per-commit gate |
| H5 Bond unify | PENDING | requires H4 closure first |
| H6a property break + JSON containers | PENDING | after H4 |
| H6b final JSON schema v2 + converter | PENDING | depends on H5 |
| H7 VIEW + MSVC | PENDING | parallel after H2 stable; gate before H8 |
| H8 D13 verify + release rc1 | PENDING | closing release gate |

## What "backlog cleared" means honestly

The v2.2 kernel track has **15+ substantive code-changing commits
remaining** within H4 alone, plus H5/H6a/H6b/H7/H8. Each is its
own design-lock + implementation cycle.

The BALLView 1.7+ track has its own pipeline (Phase 6 PyBALL,
Phase 8 Packaging, SEED-001 v1.8 UI Wave) tracked on its own
branches.

The 35 TBD ROADMAP placeholders are scaffolding awaiting
milestone-opens; they cannot be cleared through engineering work.

**Per-session feasibility:** the v2.2 H4 cycle is the next
ship-quality milestone. Even H4 alone is estimated at multi-month
engineering effort per V22-H4-DESIGN.md R7 (21 working commits +
1 closing CR; each commit at minimum 1-2 sessions of focused
work).

**This audit closes the documentation-clearable items** on the
v2.2 kernel branch as of 2026-06-04. Substantive engineering
clearance of H4-H8 requires multiple future sessions; the cycle
is fully prepared by the H4 audit-ledger sweep landed in this
session (apply / iterator / PDB / AtomContainer / bridge /
property audits all LANDED).

---

*Authored 2026-06-04 as the explicit closure of the
documentation-clearable backlog from the v2.2 branch viewpoint.*
