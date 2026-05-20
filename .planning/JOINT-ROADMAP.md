# Joint Roadmap — BALL / BALLView Program

**Collated:** 2026-05-14
**Scope:** Program-level view across both planned milestones + backlog. This is a
*read-only consolidation* of existing planning artifacts — it does not replace
`ROADMAP.md` (the canonical Milestone 1 roadmap) or `SEED-001` (Milestone 2).

**Sources collated:**
- `ROADMAP-1.6.md` (repo root) — original human-authored code review + roadmap
- `ball/.planning/ROADMAP.md` — canonical Milestone 1 roadmap (11 phases + backlog)
- `ball/.planning/REQUIREMENTS.md` — 37 v1 requirements + feature matrix
- `ball/.planning/STATE.md` — execution state (5 phases complete)
- `ball/.planning/DESIGN-HANDOVER-INTEGRATION.md` — UI-package roadmap-fit analysis
- `ball/.planning/seeds/SEED-001-ballview-refresh-ui-milestone.md` — Milestone 2 seed
- `Claude Design Handover/` — the 8-phase UI/UX modernization package

---

## Program shape

Two sequential milestones plus a backlog. They are intentionally **separate**
milestones, not one merged roadmap — the UI refresh hard-depends on the
foundation milestone's Qt 6 port (Phase 5).

```
Milestone 1: BALLView 1.6  — Toolchain / Renderer / Deps / Packaging foundation
   └── Phase 5 (Qt 6) ──────────────────────── HARD GATE ──────────┐
                                                                   ▼
Milestone 2: BALLView 1.7  — "BALLView Refresh" UI/UX modernization (8 phases)
```

**Version policy (resolves the "1.6" collision):** the foundation work is **1.6**;
the UI refresh re-bases to **1.7**. The Design Handover's internal "1.5.x → 1.6.0"
cadence becomes "1.6.x → 1.7.0".

---

## Milestone 1 — BALLView 1.6: Toolchain/Renderer Modernization

**Core value:** BALLView must build and visibly render molecules on macOS, Linux,
and Windows from current, supported dependencies.

**Status:** 5 / 11 active phases complete (13/13 planned plans executed). Next: **Phase 4**.

| Phase | Name | Status | Depends on |
|-------|------|--------|------------|
| 1 | Build Baseline | ✅ Complete | — |
| 2 | Rendering Port (4a) — `QGLWidget`→`QOpenGLWidget` | ✅ Complete (human-verified macOS) | 1 |
| 02.1 | Renderer boundary extraction (`RenderSurface`/`RendererFactory`) | ✅ Complete | 2 |
| 02.2 | CI & build-smoke matrix (3-OS GitHub Actions) | ✅ Complete (CI green ×4) | 2 |
| 3 | Language Modernization (C++14 → C++17) | ✅ Complete (CI green ×4) | 1 |
| **4** | **Dependency System Overhaul** — delete `ball_contrib`, Homebrew/system + vcpkg, `CMakePresets.json`, feature matrix | ⬜ **NEXT — not yet planned** | 1 (benefits from 3) |
| 4.1 | Config Color-Defaults Fix — stop `~/.BALLView` shadowing compiled colors | ⬜ Not started *(promoted from backlog 999.4)* | 1 |
| 5 | Qt 6 Migration (4b) + Renderer Backend Spike — keep compat-profile GL working, then prototype GL-core vs QRhi behind the 02.1 boundary | ⬜ Not started *(former Phase 05.1 folded in)* | 02.1, 2, 4 |
| 6 | Python Bindings — decision-first vertical slice (SIP 6 vs pybind11/nanobind) | ⬜ Not started | 3, 4 |
| 8 | Packaging & Distribution — notarizable macOS bundle, Linux/Windows build-from-source | ⬜ Not started | 5 |
| 9 | Test Suite Triage — wire `test/` into `ctest` | ⬜ Not started | 02.2, 3, 4 |

*(Phase 7 "Networking Rework" was removed from the active roadmap → backlog 999.3.)*

**Requirements coverage:** 38 active reqs, all mapped. Complete: BUILD ×4, RENDER ×8,
ARCH ×4, CI/DIAG ×2, LANG ×3 (= 21). Pending: DEPS ×6, CONFIG ×1, QT6 ×2, SPIKE ×2,
PY ×2, PKG ×3, CI-02 ×1 (= 17). NET-01 deferred to 1.6.x.

---

## Milestone 2 — BALLView 1.7: "BALLView Refresh" (UI/UX)

**Source:** `Claude Design Handover/` package. Tracked as **SEED-001** (dormant).
**Trigger:** Milestone 1 **Phase 5 (Qt 6)** complete — Qt 6.5 is a hard prerequisite
(`QStyleHints::colorScheme()`).
**Estimate:** ~8 phases, ~6 person-months. **Do not fold into Milestone 1.**

| Phase | Name | Risk | Effort | Flag |
|-------|------|------|--------|------|
| 0 | Design system & QSS foundation (tokens, theme infra) | low | 1 wk | — |
| 1 | Strip hardcoded palettes from `.ui` (unlocks dark mode) | low | 3 d | — |
| 2 | SVG icons + HiDPI (replace XPM) | low | 2 wk | — |
| 3 | Dialog cleanup — About / simple settings | low | 1 wk | — |
| 4 | **Unified Inspector** (replaces 8+ modal dialogs) — the big one, sub-PRs 4.1–4.6 | med | 6 wk | `BALL_UI_V2` |
| 5 | Default workspace & dock consolidation | med | 1 wk | `BALL_UI_V2` |
| 6 | Menu re-org + command palette (⌘K) | low | 2 wk | `BALL_UI_V2` |
| 7 | Welcome screen + in-product help | low | 2 wk | `BALL_UI_V2` |
| 8 | Accessibility + dark-mode finalization + flag removal | low | 2 wk | — |

**Cross-cutting (de-duplicate against Milestone 1):**
- CI matrix → M1 Phase 9 builds the base 3-OS matrix; M2 adds a `ui_v2` axis to the *existing* workflow.
- macOS packaging → M1 Phase 8 owns it; the Handover's packaging notes are a *checklist input*.
- `BALL_UI_V2` CMake option → introduced in M2 Phase 0; don't pre-create.
- Inspector rebuild (M2 Phase 4) binds to the **post-02.1 `Renderer`/`RenderSurface` contract**, not today's GL types.

**Operating contract (from the package, GSD-compatible):** `main` always shippable;
one PR per phase; all 3 platforms green every commit; every keyboard shortcut survives
(enforced by test from Phase 6); no new external runtime deps.

---

## Backlog (999.x)

| Item | Name | Notes |
|------|------|-------|
| 999.1 | BALLView UI maintainer open-questions | 4 questions needing maintainer decisions before M2 Phase 4 — long lead time, raise early. Written up issue-ready in `.planning/MAINTAINER-QUESTIONS-999.1.md`, awaiting publication. |
| 999.2 | Ninja build generator | Switch CMake generator to Ninja on all 3 OSes. Zero source changes. Sequence after Phase 4, with Phase 9. |
| 999.3 | Networking rework | `TCPServer`/`TCPServerThread` onto modern Boost.Asio + unit test (NET-01). Code already compiles; this is 1.6.x polish. |
| ~~999.4~~ | Config shadows compiled color defaults | **Promoted to active Phase 4.1** (2026-05-14) — real user-facing bug. |
| 999.5 | Open-PR triage | Review every open PR on `BALL-Project/ball` against current state — merge / rebase / close / defer. |

---

## Open issues this collation surfaced

1. ~~**Two `.planning/` directories.**~~ **Resolved 2026-05-14** — codebase map moved
   into `ball/.planning/codebase/` (paths re-based), orphaned root `.planning/` and
   root `.git` removed. Canonical GSD project root is `ball/`.

2. **`ROADMAP-1.6.md` at the repo root** is the original human-authored review and
   is now mirrored/superseded by `ball/.planning/ROADMAP.md`. Keep as a historical
   artifact or fold a pointer into it.

3. **gsd-tools decimal-phase bug** (noted in STATE.md): `phase complete` mis-reports
   next-phase for decimal phases. Hand-verify roadmap/STATE after phase ops. Real
   next phase is **4**.

---
*Joint roadmap collated 2026-05-14. Canonical sources remain `ball/.planning/ROADMAP.md`
(Milestone 1) and `ball/.planning/seeds/SEED-001-*` (Milestone 2).*

---

## 2.x roadmap addition — full libBALL / libVIEW+BALLView / pyBALL separation

**Added:** 2026-05-20 (maintainer directive, during v2.2 H2a).
**Status:** roadmap item (not yet scheduled; sequence after the v2.2
handle redesign stabilises the libBALL API surface).

**Goal:** clean, independent separation of the three deliverables into
their own repos + build systems + test suites:

1. **libBALL (BALL 2.x)** — the C++ molecular-modelling kernel + domain
   layers (COMMON…KERNEL…FORMAT/STRUCTURE/MOLMEC/QSAR/…). Its own repo,
   CMake build, and ctest suite. No VIEW/Qt/Python dependency. This is
   the foundation the other two consume.
2. **libVIEW + BALLView** — the Qt/OpenGL rendering library + the GUI
   application. Own repo + build + tests; depends on libBALL as an
   external (installed/find_package) dependency, NOT a monorepo subdir.
3. **pyBALL** — Python bindings, depending on libBALL (and libVIEW where
   visualization bindings are needed). Own repo + build + tests.

**Requirements:**
- Each component builds independently from a clean checkout, consuming
  its dependencies via a stable installed interface (CMake
  `find_package(BALL)` / exported targets), not in-tree source coupling.
- Independent CI per repo (libBALL CI must not require Qt; BALLView CI
  pulls a built libBALL; pyBALL CI pulls libBALL/libVIEW).
- Independent versioning + release cadence, with a documented
  compatibility matrix (which BALLView/pyBALL works with which libBALL).
- Independent test suites (libBALL ctest stands alone; VIEW/Python tests
  live with their components).

**Sequencing note:** do this AFTER the v2.2 handle redesign lands — the
handle API is the libBALL public surface BALLView + pyBALL will consume,
so separating before it stabilises would churn three repos. The v2.2
work already keeps SIP/Python off and VIEW deferred to H7, which is
compatible with a later clean split.

**Open questions for scheduling:**
- Repo topology: 3 sibling repos vs. libBALL repo + 2 dependents using
  submodules/`FetchContent`/system-install.
- Where the shared CMake package config + version files live.
- Migration of the existing monorepo history (subtree split vs. fresh).
