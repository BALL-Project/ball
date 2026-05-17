# Backlog 999.1 — Maintainer UI/UX Decisions (RESOLVED 2026-05-17)

**Status:** RESOLVED — answers received from maintainer 2026-05-17 (in-session via
structured-question flow; no GitHub issue needed). Original issue draft preserved
below for history. The 4 questions gated the **BALLView Refresh** phases
(999.42/.43/.44/.45/.46); with answers in hand, Wave 4 sequencing rules update and
v1.8 phase scopes propagate the answer impacts.

---

## Maintainer answers (2026-05-17)

| # | Question | Answer | Departs from Handover rec? | Impact |
|---|---|---|---|---|
| Q1 | macOS menu bar | **Keep inline menubar** | YES (Handover rec was native global) | Phase 999.46 (menu reorg) skips `QAction::setMenuRole` plumbing; cross-platform consistency preserved |
| Q2 | Classic 5-dock workspace | **Retire after one release** | NO (recommended option) | v1.8 ships both layouts with sunset notice on Classic; v1.9 removes Classic; 999.44 + 999.45 design accommodates Classic-coexistence period |
| Q3 | Theme picker scope | **Single neutral theme only** | YES (Handover rec was Follow-System) | NO dark mode / Follow-System support. 999.42 + 999.43 still ship in v1.7 tail (value reframes: design-system consistency + OS palette inheritance, NOT dark-mode unlock). 999.48 sub-deliverable "dark-mode finalize" drops. ThemeManager (999.40) built single-theme; `QStyleHints::colorScheme()` reactor inactive. |
| Q4 | Translation churn | **Accept; community round during v1.8 cycle** | NO (recommended option) | v1.8 ships English + best-effort de_DE; community round during v1.8 cycle; v1.8.x picks up completed translations. No v1.8 ship-date blocker. |

**Propagated to:**
- `.planning/ROADMAP.md` — Phase 999.1 marked RESOLVED; Phases 999.42/.43 reframed; 999.46 simplified; 999.48 scope-reduced
- `.planning/v1.7-PLAN.md` — Wave 4: 999.42 + 999.43 now UNCONDITIONAL in v1.7 tail (the "conditional on maintainer-Q3" gate is satisfied with Q3 answered)
- `.planning/seeds/SEED-001-ballview-refresh-ui-milestone.md` — maintainer-answer note appended

---

## (Original publish-ready issue draft preserved for history)


## Suggested issue title

`BALLView Refresh (1.7): 4 UI/UX decisions needed from maintainers`

## Suggested issue body

### Context

The **BALLView Refresh** UI/UX modernization milestone (target 1.7, the "Claude
Design Handover" package) is gated on the 1.6 foundation milestone's Qt 6 port
(Phase 5). Four design decisions need maintainer input and have a long lead time —
raising them now so they're settled well before the UI milestone's Inspector and
menu phases start.

Tracked internally as backlog item **999.1** (`.planning/ROADMAP.md`); see
`.planning/DESIGN-HANDOVER-INTEGRATION.md` and
`.planning/seeds/SEED-001-ballview-refresh-ui-milestone.md` for full context.

### Decisions needed

**1. macOS menu bar**
Keep the current inline menu bar, or adopt the native global macOS menubar via
`QAction::setMenuRole`?

**2. "Classic" workspace**
Keep the legacy 5-dock workspace layout as a long-term opt-in preset, or retire it
after one release once the consolidated workspace ships?

**3. Theme picker scope**
Ship a single neutral theme, or expose Light / Dark / Follow-System?
*(Handover recommendation: Follow-System, no custom color picker.)*

**4. Translation churn**
The Phase 6 menu re-org invalidates roughly 40% of `BALLView-de_DE.ts` strings.
Accept this and plan a community translation round — and if so, when?

### Why now

These block the UI milestone's Phase 4 (unified Inspector) and Phase 6 (menu
re-org). They need a deliberate decision rather than a default; translation
coordination especially needs lead time.

---
*When published, record the issue URL here and update backlog 999.1 in `ROADMAP.md`.*
