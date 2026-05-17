---
id: SEED-001
status: promoted-full
planted: 2026-05-14
planted_during: BALLView 1.6 Modernization · Phase 2 (Rendering Port)
trigger_when: Phase 5 (Qt 6 + Pipeline) is complete — Qt 6.5 is a hard prerequisite (SATISFIED — Qt 6.8 LTS floor since v1.6.2)
scope: Large
promotion: 2026-05-17 — user direction "roll the BALLView refresh into the tail-end of v1.7"
v1.7_full: All 8 Handover phases (Phase 0 design system + Phase 1 palette removal + Phase 2 SVG/HiDPI icons + Phase 3 simple dialogs + Phase 4 Inspector + Phase 5 Workspace + Phase 6 Menus + palette + Phase 7 Onboarding + Phase 8 a11y) → ROADMAP phases 999.40-999.48 + NEW 999.49 Classic-delete; full Wave 4 in v1.7-PLAN.md. v1.7 final ships with BALL_UI_V2=ON default (Handover-Phase-8 flip lands in v1.7, not v1.8). v1.8 milestone scope wholly TBD as of 2026-05-17 audit pull-in.
---

> **Status change 2026-05-17 (audit pull-in):** promoted-subset → promoted-full. Per user direction "keep everything in 1.7" (audit follow-up to original "roll the BALLView refresh into the tail-end of v1.7"), the previously-deferred v1.8 remainder (Inspector/Workspace/Menus/Onboarding/a11y = Handover Phases 4-8) pulled into v1.7 Wave 4 alongside the original v1.7-subset (Phases 0-3). A new **Phase 999.49** (Delete Classic 5-dock workspace, Q2 commitment) appended after 999.48. Sequencing accommodates the longer Wave 4 by running 999.44 (Inspector) and 999.45 (Workspace) in parallel — different file trees. See `.planning/v1.7-PLAN.md` Wave 4 + `.planning/ROADMAP.md` phases 999.40-999.49.

> **Status change 2026-05-17 (original):** dormant → promoted-subset. User directed "roll the BALLView refresh into the tail-end of v1.7" — the foundation + maintainer-independent subset lands in v1.7 as **Wave 4** (phases 999.40-999.43, all behind `BALL_UI_V2` build flag default OFF). At that time the 5 architecturally-heavier phases were forwarded to v1.8; the audit-pull-in (above) supersedes that forwarding.

> **Maintainer answers received 2026-05-17:** Q1 = keep inline menubar (`QAction::setMenuRole(...Standard)` for About/Prefs/Quit is fine, just not full QMenuBar global-menu mode); Q2 = retire Classic 5-dock after one release (Phase 999.45 ships both with picker, **Phase 999.49 deletes Classic** post-flag-removal in the same v1.7 RC cycle per audit pull-in); **Q3 = single neutral theme only (no Light/Dark/Follow-System)**; Q4 = community translation round during the **v1.7** cycle (re-pinned from v1.8 per audit). Q3 departs from Handover recommendation. Impact: 999.42 + 999.43 ship in v1.7 tail (value reframes from "dark mode unlock" to "OS-palette inheritance hygiene"); ThemeManager (999.40) built single-theme; `QStyleHints::colorScheme()` reactor wired but inactive; Phase 999.48 sub-deliverable "dark-mode finalize" drops; **999.42 ThemedIconEngine state-tints (Normal/Active/Selected/Disabled) are NOT theme-tints and Q3 does NOT block them**. Full answer record: `.planning/MAINTAINER-QUESTIONS-999.1.md`.

---

# SEED-001: Milestone 2 — "BALLView Refresh" UI/UX modernization

## Why This Matters

A complete, externally-prepared **UI/UX modernization plan** for BALLView
already exists: the "Claude Design Handover" package at
`/Users/kohlbach/Claude/BALL/Claude Design Handover/`. It is 8 phases of
surface modernization — design-system/QSS theming, SVG icons + HiDPI, dialog
cleanup, a unified Inspector (replacing 8+ modal dialogs), workspace
consolidation, menu re-org + command palette, onboarding, and accessibility —
roughly 6 person-months, with mockups, a CI matrix, a branching model, and a
release cadence already specified.

It addresses the half of "pulling BALLView into 1.6" that the current
milestone deliberately does **not**: the current milestone is the
toolchain/renderer/Qt6/deps/packaging/CI **foundation**; this is the visible
**refresh**. The package explicitly states the renderer migration is tracked
separately and assumes it has landed first — i.e. it is designed to follow
this milestone, not replace it.

Losing track of this package would mean re-deriving a UI plan that has
already been done well.

## When to Surface

**Trigger:** Phase 5 (Qt 6 + Pipeline) of the current milestone is complete.

The Design Handover's Phase 0 (design-system foundation) hard-depends on
`QStyleHints::colorScheme()`, which is **Qt 6.5+**. The entire package is
blocked until the Qt 6 port lands. Surface this seed during
`/gsd-new-milestone` when the new milestone scope matches any of:
- "UI", "UX", "theming", "dark mode", "Inspector", "redesign", "refresh"
- "the second BALLView milestone" / "what's after the modernization"
- Right after Phase 5 completes and the user asks "what's next"

## Scope Estimate

**Large** — a full milestone of its own (~8 phases, ~6 person-months in the
package's own estimate). Do NOT fold it into the current 9-phase roadmap.

## How to Action (when triggered)

1. `/gsd-new-milestone` for **"BALLView Refresh"** — target version **1.7**,
   NOT 1.6. There is a version-numbering collision: the package internally
   assumes "1.6.0 = the UI refresh", but the current milestone owns 1.6 as
   the foundation release. Re-base the package's "1.5.x → 1.6.0" cadence to
   "1.6.x → 1.7.0". (Full reasoning in `.planning/DESIGN-HANDOVER-INTEGRATION.md`.)
2. `/gsd-import` the `revitalization/*.md` docs — they are already in GSD
   phase-doc shape (Goal / Scope / Steps / Files / Acceptance / Cross-platform
   / Risks), so they drop in with light adaptation.
3. **De-duplicate cross-cutting work:** the package's CI matrix overlaps this
   milestone's Phase 9 (extend the existing matrix with a `ui_v2` axis, don't
   rebuild it); its macOS packaging notes overlap Phase 8 (consume them as a
   checklist input).
4. Milestone 2 Phase 0 `depends_on` this milestone's Phase 5.
5. Bind the Inspector rebuild (its Phase 4) to the **post-Phase-02.1 renderer
   interface** (`Renderer` / `RenderSurface`), not today's GL types — several
   replaced dialogs touch renderer/scene settings.

## Breadcrumbs

- `/Users/kohlbach/Claude/BALL/Claude Design Handover/` — the package itself
  (`START-HERE.md`, `BALLView UI Audit.html`, `revitalization/` with 8 phase
  docs + `09-cross-platform.md` + `10-migration-playbook.md` + mockups)
- `.planning/DESIGN-HANDOVER-INTEGRATION.md` — full analysis and roadmap-fit
  reasoning written this session
- `.planning/RENDERER-INTERFACE-BOUNDARY.md` — the stable renderer contract
  the Inspector rebuild should bind to
- `.planning/ROADMAP.md` — current milestone; Phase 5 is the trigger gate
- See also SEED/backlog item for the 4 maintainer open-questions the package
  needs answered before its Phase 4 (long lead time — raise early)

## Notes

The package's "operating contract" (main always shippable, one PR per phase,
`BALL_UI_V2` flag default OFF until its Phase 8, all-platforms-green CI) is
sound and compatible with GSD's phase model. The 4 open questions it flags
for maintainers (macOS native menubar, retire the legacy 5-dock workspace?,
theme-picker scope, translation-churn plan) are tracked separately in the
backlog so they can be raised with maintainers well before Milestone 2 starts.
