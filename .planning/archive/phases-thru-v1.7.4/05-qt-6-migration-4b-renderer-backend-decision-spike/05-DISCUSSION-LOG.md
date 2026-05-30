# Phase 5: Qt 6 Migration (4b) + Renderer Backend Decision Spike - Discussion Log

> **Audit trail only.** Do not use as input to planning, research, or execution agents.
> Decisions are captured in CONTEXT.md — this log preserves the alternatives considered.

**Date:** 2026-05-15
**Phase:** 5-qt-6-migration-4b-renderer-backend-decision-spike
**Areas discussed:** Qt 6 version target, macOS GL profile under Qt 6
**Areas presented but skipped:** Spike backend scope, Spike acceptance bar (left to Claude's discretion with locked defaults — see CONTEXT.md)

---

## Qt 6 version target

### Q1 — How should the Qt 6 version be pinned?

| Option | Description | Selected |
|--------|-------------|----------|
| Pin to 6.5 LTS (Recommended) | FIND_PACKAGE(Qt6 6.5 ...). Broad availability (brew, vcpkg, Ubuntu 24.04 apt). Supported through 2026+. Lowest-risk Qt 6 jump. | ✓ |
| Pin to 6.8 LTS | Newer features (better RHI, Wayland). Not in Ubuntu 22.04 apt — forces CI to 24.04 or PPA. Less mature in some Homebrew bottles. | |
| Float (latest available) | No specific minor pin. Cheap bring-up; biggest risk of asymmetric platform rollforward. Hard to reproduce builds. | |

**User's choice:** Pin to 6.5 LTS (Recommended).

### Q2 — Qt 6.5 LTS provisioning on Ubuntu 22.04 (apt ships 6.2, too old). Fallback?

| Option | Description | Selected |
|--------|-------------|----------|
| Bump CI to ubuntu-24.04 (Recommended) | One-line matrix.runner change. 24.04 apt ships Qt 6.4/6.5. Clean, reproducible, aligns Linux CI with a modern LTS. Picks up newer GCC. | ✓ |
| Stay on 22.04, install Qt via KDE neon PPA / Qt online installer | Layer Qt 6.5 on top of 22.04. More fragile, adds an external apt source. | |
| Stay on 22.04, install Qt via aqtinstall | Fetch Qt 6.5 binaries via Python tool. Reproducible by exact version. Adds a tool dep. | |

**User's choice:** Bump CI to ubuntu-24.04 (Recommended).

### Q3 — Qt 6 component set handling

| Option | Description | Selected |
|--------|-------------|----------|
| Same component set, Qt 6 spellings (Recommended) | Keep the existing list, FIND_PACKAGE(Qt6 ...) with same components. Add Qt6::OpenGLWidgets (Qt 6 split QOpenGLWidget out). Minimal scope. | ✓ |
| Same set + Qt6::Core5Compat | Add Core5Compat to ease QRegExp / QDesktopWidget migration via drop-in shims. Larger dep; should be removed eventually. | |
| Audit and minimize | Drop unused components as part of the migration. Risk: scope creep, harder to bisect regressions. | |

**User's choice:** Same component set, Qt 6 spellings (Recommended). No Core5Compat — clean break, QRegExp/QDesktopWidget replaced inline.

### Q4 — Qt 6.5 sourcing on macOS / Windows

| Option | Description | Selected |
|--------|-------------|----------|
| Homebrew qt + vcpkg qt6-base (Recommended) — next area | macOS: brew install qt (6.9.x, satisfies 6.5 floor). Windows: vcpkg qt6-base + qt6-tools as needed. Mirrors Phase 4's per-platform dep policy. | ✓ |
| Pin macOS to qt@6.5 (tap or aqtinstall) | Force Homebrew to a specific 6.5 bottle. Reproducible by exact version, but extra plumbing for marginal benefit (FIND_PACKAGE floor already enforces the minimum). | |
| More questions — still have version concerns | Stay in this area. | |

**User's choice:** Homebrew qt + vcpkg qt6-base — next area (Recommended).

---

## macOS GL profile under Qt 6

### Q1 — Apple deprecated legacy GL. Under Qt 6, QOpenGLWidget on macOS picks GL 4.1 core by default. How to keep fixed-function rendering working?

| Option | Description | Selected |
|--------|-------------|----------|
| Force compat profile via QSurfaceFormat (Recommended) | Explicitly request CompatibilityProfile + GL 2.1/3.0 in main.C / GLRenderWindow. macOS emits deprecation noise but serves a compat context up to 4.1 hardware-limit. Lowest risk for the Qt 6 jump. | ✓ |
| Accept Qt 6 default (4.1 core) | Effectively breaks BALLView rendering on macOS until GL-core or QRhi backend exists. Conflates Qt 6 migration with the spike. | |
| Conditional: compat on Linux/Windows, core on macOS | #ifdef out the compat request on macOS. Breaks the rendering invariant unevenly per platform — hard to debug. Better as a spike output. | |

**User's choice:** Force compat profile via QSurfaceFormat (Recommended).

### Q2 — How should macOS GL deprecation warnings be handled at runtime?

| Option | Description | Selected |
|--------|-------------|----------|
| Silence Apple's GL-deprecation warnings; document the source in SPIKE-02 (Recommended) | GL_SILENCE_DEPRECATION=1 (compile time / QSurfaceFormat / Info.plist). Clean macOS console. SPIKE-02 references the silenced liability. | |
| Leave them visible — they signal the spike's deadline | No silencing. Every macOS run prints the deprecation banner; standing reminder that fixed-function path is borrowed time. | ✓ |
| Suppress just for Release, keep for Debug | Conditional silencing by build type. Splits behavior by audience. Adds CMake plumbing. | |

**User's choice:** Leave them visible — they signal the spike's deadline. (Override of the recommended option.)

### Q3 — Phase 02.2 render smoke check expectation under Qt 6 + forced compat profile

| Option | Description | Selected |
|--------|-------------|----------|
| Same smoke check, must still pass (Recommended) | RENDER-08 / criterion 3 says "no pixel regression". The check stays as-is and must go green on Qt 6. macOS compat-profile silently degrading to black framebuffer = Phase 5 blocker. | ✓ |
| Smoke check passes BUT pixel-identity is not required — just non-blank | Already what the script asserts. Tolerates minor Qt 6 rendering quirks. Real regressions still caught. | |
| Smoke check gets a deliberate baseline refresh as part of Phase 5 | Re-baseline expected PNGs after the Qt 6 jump. Risk: hides regressions. | |

**User's choice:** Same smoke check, must still pass (Recommended).

### Q4 — Windows GL profile under Qt 6

| Option | Description | Selected |
|--------|-------------|----------|
| Same QSurfaceFormat for all 3 OSes (Recommended) | Force CompatibilityProfile + GL 2.1 across platforms. Single code path; Windows drivers expose compat reliably. | ✓ |
| Platform-specific QSurfaceFormat | Per-OS format with different versions / profiles. Risk: each platform takes a slightly different path. Discouraged without spike data. | |
| More questions about macOS GL profile — stay here | Keep digging. | |

**User's choice:** Same QSurfaceFormat for all 3 OSes (Recommended).

---

## Final Check — explore more areas or write CONTEXT.md?

| Option | Description | Selected |
|--------|-------------|----------|
| I'm ready for context | Write CONTEXT.md with what we have. Planner can default the unspecified spike-scope / spike-acceptance areas (or ask when planning). | ✓ |
| Discuss Spike backend scope now | Lock the QRhi vs GL-core vs both decision before planning. Saves a planner-time round-trip. | |
| Discuss Spike acceptance bar now | Lock what "done" means for the throwaway prototype before planning. | |
| Discuss both remaining areas | Run through both spike-related areas before writing CONTEXT.md. | |

**User's choice:** I'm ready for context.

---

## Claude's Discretion

The user explicitly deferred two gray areas to planner / researcher discretion. CONTEXT.md captures locked defaults so the planner can proceed without re-asking, but is free to choose otherwise with rationale:

- **Spike backend scope** (QRhi vs GL-core vs both) — locked default: **both, with rationale in 05-RESEARCH.md** if the planner trims to one.
- **Spike acceptance bar** — locked default: **Comparison-level** (renders + picks + text overlay + pixel-equivalent on canonical scene + basic FPS on macOS+Windows).
- **Sequencing the inline `QRegExp` / `QDesktopWidget` replacement** — dedicated early plan vs. folded into main Qt 6 bring-up. Planner picks.
- **CMake hygiene of `Qt6::OpenGLWidgets` discovery** — pure plumbing call.

## Deferred Ideas

- The full `PIPE-01` programmable-pipeline rewrite. SPIKE-02 scopes its task list; the work is a separate future phase (v2).
- Phase 5.1 build-warnings & latent-bug cleanup — waits on Phase 5 (Qt 5 deprecation noise clears with Qt 6).
- `-Wdeprecated-declarations` cleanup at large — bulk disappears with Qt 6; remainder in Phase 5.1 Tier C.
- Audit-and-minimize Qt component set — rejected as scope creep.
- Per-OS `QSurfaceFormat` tuning — rejected for Phase 5; revisit only if the spike produces evidence one platform needs a different profile.
