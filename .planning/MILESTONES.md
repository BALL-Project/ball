# Milestones — BALLView Modernization

Shipped history. (Backfilled 2026-05-30 — earlier milestones were driven via
direct execution + ROADMAP.md without the formal GSD milestone-transition
ceremony, so the GSD machinery had drifted; this restores the record.)

| Milestone | Tag | Shipped | Theme |
|-----------|-----|---------|-------|
| **v1.6.0** | `v1.6.0` | 2026-05-15 | First cross-platform build+render (macOS/Linux/Windows) off current deps |
| **v1.6.1** | `v1.6.1` | 2026-05-16 | Strict-corrective patch |
| **v1.6.2** | `v1.6.2` | 2026-05-17 | Build-acceleration cluster (Ninja 87× Windows, PCH, CI caches) + scope cleanup; Qt 6.8 LTS floor; linux-arm64 blocking |
| **v1.7.0** | `v1.7.0` | 2026-05-19 | BALLView Refresh — neutral theme, SVG icons, unified Inspector dock, command palette, onboarding, a11y; `BALL_UI_V2` retired |
| **v1.7.1** | `v1.7.1` | 2026-05-21 | First signed + notarized macOS release (Developer ID + Apple notarization) |
| **v1.7.2** | `v1.7.2` | 2026-05-24 | Build accel — Ward serializer de-Boost; Inspector controller cut-overs (Clipping/Label/transparency); renderer-plugin warning |
| **v1.7.4** | `v1.7.4` | 2026-05-29 | Inspector Depth + the Controller Contract — command-shaped `Controller::apply()` on all 9 controllers; owner-API narrowing; contract-test harness; coloring histogram; per-section reset; Windows Qt → prebuilt aqtinstall (release ~2h→~25min) |

**Notes**
- **v1.7.3** was reserved for the legacy-dialog deletion (Phase 999.53); it was
  deferred and its work parked on the `v1.7.3-legacy-deletion` branch. v1.7.4
  shipped ahead of it (timeline skips 1.7.3). The deletion is folded into **v1.8**.
- GitHub Releases: v1.7.4 published as **Latest**; v1.7.0/v1.7.1 published;
  v1.7.2 remains a reviewable draft (superseded by v1.7.4).

---

## ▶ Current: v1.8 — "Clear the v1.x decks"

Started 2026-05-30. v1.x-compatible close-out (no API/substrate breaks). See
`PROJECT.md` (Current Milestone) + `ROADMAP.md`.
