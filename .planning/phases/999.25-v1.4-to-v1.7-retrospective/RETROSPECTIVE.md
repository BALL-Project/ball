# BALL / BALLView — v1.4 → v1.7 Retrospective

**Status:** v1.7 modernization, doc-tree authored 2026-05-17 (Phase 999.25, Wave 3).
**Audience:** maintainers, downstream packagers, and curious users.
**Tone:** factual. This is not a release announcement.

---

## 1. Framing

This document captures what changed in BALL (the C++ molecular modelling
library) and BALLView (its Qt/OpenGL viewer) between the v1.4 era and
the v1.7 release.

**A note on the baseline.** v1.4 is a *conceptual* baseline, not a git
tag. This repository's earliest annotated tag is `v1.6.0`. The "v1.4
era" is the last period (~2002 to ~2010) during which BALL's tutorials,
build instructions, and Python bindings were freshly written; CVS `$Id$`
keywords across `doc/TUTORIAL/*.tex` (e.g. `tutorial2.C,v 1.4
2002/02/27`) anchor that era. v1.5, v1.6, the long freeze (~2016-2022),
and the v1.6.x modernization happened on top of that doc baseline
without a corresponding doc refresh. v1.7 is the first release where
the user-facing surface (build docs, signed installers, auto-update,
RELEASE-NOTES) matches the modernized substrate.

The detailed source-of-truth for everything below lives in
`.planning/ROADMAP.md` (2083 lines, 63 phase entries) and in the
per-phase `SUMMARY.md` files under `.planning/phases/`. This document
is the index.

---

## 2. What changed at a glance

### 2.1 Toolchain and dependency floor

| Area | v1.4 era (~2002-2010) | v1.6.x freeze (~2016-2022) | v1.7 (2026) |
|---|---|---|---|
| Build system | CVS + autotools | CMake (~3.0, with bundled `Find*.cmake` and `ball_contrib`) | CMake **3.5+** (tested through 3.31), `CMakePresets.json`, config-mode `find_package`, **Ninja** as the canonical generator on all three OSes |
| C++ standard | C++98/03 | C++14 bridge flag (load-bearing) | **C++17** via `CMAKE_CXX_STANDARD`, no raw `-std=` flags (Phase 3) |
| Qt | Qt 3 | Qt 5.x (4.x rendering path in places) | **Qt 6.8 LTS** floor (Phases 5, 5.1, 999.7); QOpenGLWidget; QPainter overlay |
| OpenGL | Fixed-function GL 1.x via `QGLWidget` | Fixed-function via `QGLWidget` (removed in Qt 6) | Fixed-function via **compat profile** on `QOpenGLWidget`; QRhi spike completed; modern backend deferred to v2.0 PIPE-01 (Phase 999.6) |
| OpenBabel | 2.x (when present) | 2.x | **3.x** API (Phase 04-03) |
| Boost | Pre-Asio-modernized | Pre-Asio + Asio API breakage patched in Phase 1 | Boost 1.90 |
| Eigen | Eigen2 / Eigen3 (bundled) | Eigen3 (bundled) | Eigen3 from system / Homebrew / vcpkg (Phase 4) |
| Python bindings | SIP 4, Python 2 | SIP, disabled | **None shipped** in v1.7; PyBALL v2 rewrite is the lead step of v2.1 (Phase 6, Phase 999.15) |
| Dependency source | In-tree `ball_contrib` tarballs (~2016) | `ball_contrib` (broken on modern toolchains) | **Homebrew** (macOS), **system apt** (Linux), **vcpkg manifest** (Windows) — `ball_contrib` removed from the build path (Phase 4) |
| CI | None | None | **GitHub Actions tri-OS matrix** (macOS-arm64 + Ubuntu x86_64 + Ubuntu ARM64 + Windows x64); ccache; ninja; render smoke check; non-blocking coverage on Linux (Phase 02.2, Phase 999.2, Phase 999.17, Phase 999.27) |

### 2.2 Platforms

| Platform | v1.6.0 status | v1.7 status |
|---|---|---|
| macOS Apple Silicon | Built (Phase 1 baseline) | **Native arm64**, signed + notarized `.dmg`, Sparkle auto-update |
| macOS Intel | Tier-2 (no CI gate) | Same; not separately signed |
| Linux x86_64 (Ubuntu 24.04) | Built (Phase 4 vcpkg + system path) | AppImage + `.deb` + `.rpm`, LGPL-only SKU |
| Linux ARM64 | Not present | **Added** (`ubuntu-24.04-arm` runner) — same packaging matrix |
| Windows x64 | Built behind vcpkg (Phase 4); Qt 6 bring-up via Phase 5.1 / 999.7 | NSIS installer, **SignPath** Foundation OSS signing (externally gated), WinSparkle auto-update |

### 2.3 Packaging

| Artifact | v1.6.x | v1.7 |
|---|---|---|
| macOS | `.zip` of `BALLView.app`, ad-hoc resign step | Signed + notarized `.dmg` via `create-dmg`, Developer ID Application cert, stapled (Phase 08a) |
| Windows | None | NSIS installer signed via **SignPath Foundation** (OSS path; ~weeks lead time externally gated) (Phase 08b) |
| Linux | None | **AppImage + DEB + RPM**, single LGPL SKU per arch (no FFTW); GPL SKU scaffolded but unshipped pending named GPL-compliance owner (Phase 08c, see `.planning/phases/08c-packaging-linux/08c-FFTW-GPL-DECISION.md`) |
| Auto-update | None | **Sparkle 2.6.x** (macOS) + **WinSparkle** (Windows); Ed25519 signed appcast hosted on **GitHub Pages** (Phase 999.8, Phase 999.8-full) |

---

## 3. Removed or disabled subsystems

Everything in this table was carrying real maintenance cost (broken
builds, deprecation warnings, dead-code linkage) and had no live caller.
Each row links to the phase where the decision and the deletion landed.

| Subsystem | Status in v1.7 | Rationale | Phase |
|---|---|---|---|
| `ball_contrib` (in-tree dep tarballs) | **Removed from build path** | ~2016-era source bundles do not build on modern toolchains; replaced by Homebrew / apt / vcpkg | Phase 4 |
| `RTfact` raytracer | Disabled | External dep dead since pre-2016; raytracer path retained via in-tree CPU pixel-buffer renderer | Phase 1, Phase 2 (A1 finding) |
| `QtWebEngine` (BALLView help viewer) | Disabled | Heavy dep with no functional payoff at current help-content size | Phase 4 (`USE_QTWEBENGINE=OFF` on Windows) |
| SIP Python bindings | Disabled (not shipped) | SIP toolchain incompatible with modern Python; full PyBALL v2 rewrite is the lead step of **v2.1** (autowrap + Cython vs nanobind bake-off — Phase 6); v1.7 ships C++ only | Phase 6, Phase 999.15 |
| `VRMLRenderer` | **Deleted** (146 + 487 LOC removed) | `BALL_DEPRECATED` since pre-v1.6; "Export VRML" UI action actually called `STLRenderer` (historical misnomer); class + factory enum + UI rename landed atomically with backward-compat `VRMLNR`→`STLNR` INI key migration | Phase 999.37 |
| `DOCKING/GENETICDOCK` (entire subsystem) | **Deleted** (18 files, 2722 LOC) | Zero external references in `include/`, `source/`, `APPLICATIONS/`, `test/`, `PYTHON/`; zero UI exposure (`DockingController` never registered a `GENETIC_*` enum); zero test coverage; eliminated ~150 deprecation warnings per platform per build | Phase 999.29 |
| `GAMESSLog` parser | **Deleted** | Never-functional since 2005; surfaced during issue-triage (#662) — code only ever returned skeleton data | Phase 999.14 (commit `2dced9ea`) |
| FFTW in shipped Linux binaries | Disabled (LGPL SKU) | FFTW is GPL-2.0+; linking BALL (LGPL-2.1+) against it relicenses the binary. v1.7 lacks a named GPL-compliance owner, so the GPL SKU is scaffolded but not built/uploaded; users who need FFTW build from source per `BUILD-linux.md` | Phase 08c |
| Bundled `Find*.cmake` (Boost, TBB, Eigen3, OpenBabel) | Removed in favor of upstream config-mode `find_package` | Stale, drifted from upstream behaviour | Phase 4 (Plan 04-01) |
| Bundled `ResidueNames=` / `ResidueNameColors=` config keys | Migrated to diff-against-defaults pattern | Persisted user config silently shadowed compiled defaults; editing source had no visible effect for any user with `~/.BALLView` | Phase 4.1, Phase 999.4b |
| `QGLWidget` / `QGLFormat` / `QGLContext` | **All references removed** | Class removed in Qt 6 | Phase 2 (the rendering port) |

---

## 4. New capabilities

### 4.1 Tri-OS CI matrix (Phase 02.2, Phase 999.2, Phase 999.17, Phase 999.27)

Three operating systems × two architectures (macOS-arm64, Ubuntu x86_64,
Ubuntu ARM64, Windows x64). Ninja generator on every job. ccache on
every job. Apt-archives cache on Linux. Homebrew cache on macOS. vcpkg
artifact cache on Windows. CMake build-tree cache on Windows (Phase
999.17) — keyed on hash of `CMakeLists.txt`, `cmake/**`, `vcpkg.json`,
`CMakePresets.json` (key bumped v1→v2 after the cb392cc4 source-defect
mask incident; structural source-aware-key fix tracked as Phase 999.36).

Headline result: Windows Build dropped **4818 s cold → 55 s warm** (87×
speedup, ~98.9% effective ccache hit rate) after Phase 999.2 (Ninja
switch) — total Windows job ~4.7 min, clearing the 10-min standing
disable threshold.

A non-blocking coverage job on Linux Debug + gcov went green in Phase
999.27 (commit `c0d439c`) — the first time in the v1.6-modernization
branch's recorded history that coverage built end-to-end.

Stage-gated promotion for v1.7 (Stage 1 macOS-only → Stage 2 +Linux →
Stage 2.5 +weekly Windows cron + path-aware Windows PR triggers →
Stage 3 +Windows blocking) is documented in `v1.7-PLAN.md` §"CI matrix
gating strategy".

### 4.2 Renderer-boundary extraction (Phase 02.1) and the QRhi spike (Phase 5)

The `Renderer` / `RenderSurface` / `RendererFactory` boundary in
`include/BALL/VIEW/RENDERING/` is the stable contract for a future
backend swap. Behind it, Phase 5's two-arm spike (GL-Core in Plan 05-05;
QRhi in Plan 05-06) produced the **split-pattern decision** at
`.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-SPIKE-DECISION.md`:
**GL-Core for v1.6.x / v1.7** (which ships with the compat profile),
**QRhi for v2.0** (PIPE-01, Phase 999.6). The decision was wedged by the
Qt 6.5 LTS floor and the Ubuntu 24.04 system-Qt deployment constraint —
QRhi requires Qt 6.7+.

### 4.3 Auto-update via Sparkle + WinSparkle (Phase 999.8)

Sparkle 2.6.x on macOS, WinSparkle on Windows. Ed25519-signed appcasts
hosted on **GitHub Pages** (`https://ball-project.github.io/ball/appcast-{macos,windows}.xml`).
Spike captured in `.planning/phases/999.8-auto-update-sparkle-winsparkle/999.8-SPIKE.md`;
key-management runbook in `ED25519-KEY-RUNBOOK.md`; appcast publishing
runbook in `APPCAST-PUBLISHING-RUNBOOK.md`. Sparkle hooks into the
QApplication event loop and runs the v1.x → v1.x+1 update flow without
the user re-visiting GitHub Releases.

### 4.4 BALLView Refresh — Wave 4 design-system foundation (Phase 999.40-999.43)

A handover package (`/Users/kohlbach/Claude/BALL/Claude Design Handover/`)
specifies 8 UI revitalization phases. v1.7 ships the **foundation
subset** (Phases 999.40-999.43) **behind the `BALL_UI_V2` CMake flag,
which defaults OFF**. v1.7 RC ships with the classic UI; users can opt
in via `-DBALL_UI_V2=ON`. v1.8 flips the default and lands the heavier
phases (999.44 Unified Inspector, 999.45 Workspace consolidation,
999.46 Menus + command palette, 999.47 Onboarding, 999.48 a11y).

Maintainer-question resolutions (2026-05-17, Phase 999.1):
- macOS menu bar: keep inline, do NOT use `QAction::setMenuRole`
- Classic 5-dock workspace: retire after v1.8
- Theme picker: **single neutral theme** (no Light/Dark/Follow-System)
- Translation churn: community translation round during v1.8 cycle

The single-theme decision (Q3) reframed Phase 999.42's value from
"dark-mode unlock" to "OS-palette inheritance hygiene". 999.42 still
ships in v1.7 tail; the Phase 999.48 sub-deliverable "dark-mode
finalize" drops.

### 4.5 Dual-SKU Linux packaging scaffold (Phase 08c)

`cmake/BALLPackageConfig.cmake` carries the SKU-name flip (`-lgpl` vs
`-gpl`) keyed on `BALL_HAS_FFTW`. `packaging/linux/COPYING.GPL` and
`README-LICENSING.md` ship in-tree. CPack DEB/RPM stanzas
generator-expression-flip the license string. **Re-enabling the GPL SKU
is a `release.yml` matrix-entry addition only** — no code or packaging
work required once a GPL-compliance owner is named.

### 4.6 Source-aware regression discipline

Several phases landed defensive infrastructure that did not exist in
the v1.6 era:

- **`BALL_VIEW_GL_DIAG`** startup line (Phase 02.2) — emits GL vendor,
  version, profile, `QSurfaceFormat`, device-pixel ratio, default FBO
  size, selected backend. Doubles as the smoke-check oracle.
- **Render smoke check** (Phase 02.2) — launches BALLView headless with
  a known molecule, asserts non-blank pixels at the expected viewport.
- **`check-no-legacy-gl-symbols.sh` CI lint** (Phase 02.2) — grep gate
  preventing `QGLWidget`/`QGLContext`/`QGLFormat` from re-appearing.
- **Per-TU build-profiling artifact** (Phase 999.19) — uploads
  `compile_commands.json` + ninja timing breakdown to identify
  compile-time outliers.
- **Path-aware CI triggers + concurrency groups** (Phase 999.18,
  CI-HARDEN-W1) — skip CI on doc/planning-only changes; cancel
  obsolete runs.
- **Action artifact pins bumped to v6/v7** (Phase 999.20) — eliminates
  the Node-20 deprecation surface.

---

## 5. Risk and known-issues survival

### 5.1 Unresolved at v1.7 tag

Items that did not close inside v1.7 and survive into v1.8 / v2.0.

| Item | Where | Disposition |
|---|---|---|
| **GPL-compliance owner for the FFTW Linux SKU** | Phase 08c | Documented gap. SHIP-LGPL-ONLY for v1.7. REOPEN criteria captured in `08c-FFTW-GPL-DECISION.md` §"REOPEN criteria"; ship-both is a matrix-entry-addition once a named human accepts. |
| **Bus-factor on signing infrastructure** | Phase 08a, Phase 08b | Apple Developer credentials, SignPath admin access, notarytool API keys, Ed25519 appcast keys all require ≥2 humans aware of recovery path. Runbooks landed (`08a-SECRETS-RUNBOOK.md`, `08b/SECRETS-RUNBOOK.md`, `ED25519-KEY-RUNBOOK.md`); credential storage is the human-process gap. |
| **SignPath OSS certificate procurement (Phase 08b)** | Phase 08b | Externally gated; OSS approval can take weeks. Fallback paths documented: Path B Azure ~$120/yr cert; Path C commercial ~$200-500/yr. Default plan ships unsigned-with-warning if SignPath stalls past v1.7-RC-1. |
| **Source-aware Windows cmake-tree cache key** | Phase 999.36 | The cb392cc4 → 9042af07 incident exposed that the 999.17 cache key does not hash `source/MATHS/vector3.C`-style export-contract surface. Stopgap: cache-key bump v1→v2. Structural fix queued for v1.6.3 / v1.7. |
| **Apple-GL deprecation banner** | Phase 999.28 | `GL_SILENCE_DEPRECATION` on the `VIEW` target via Apple's documented escape hatch — eliminates ~1500 warnings until **v2.0** Metal/MoltenVK migration via QRhi (PIPE-01, Phase 999.6). |
| **Tier-C warning census tail** | Phase 999.22 (a/b/c) | Census-only in v1.6.2; (a) mechanical-fix subset and (c) per-site review run as v1.7 Wave 2. (b) PIPE-01-dependent subset stays v2.0. |
| **CIF Bison grammar shift-reduce conflicts** | Phase 999.23 | 5 shift/reduce conflicts on Bison 3.8.2 + 19 `-Wformat-truncation` warnings on the generated parser. Audit phase queued. |
| **No `BUILD-windows.md`** | Doc-tree gap | `BUILD-macos.md` and `BUILD-linux.md` ship. The Windows from-source build path lives in `.github/workflows/ci.yml` + `vcpkg.json` + Phase 08b runbooks but lacks a single from-source guide for downstream users. Filed as a gap by Phase 999.25 (this retrospective). |
| **Doc-rot in `doc/TUTORIAL/`** | Phase 999.25b (parallel async sweep) | Tutorial sample compile-check + update sweep runs async post-v1.7-tag (2-5 days). Most v1.4-era samples likely fail against the v1.7 API. |
| **Translation churn** | Phase 999.46 (v1.8) | `BALLView-de_DE.ts` and `BALLView-zh_TW.ts` carry orphan VRML strings + ~40% stale from the v1.8 menu reorg. Community translation round runs during v1.8 cycle. |

### 5.2 Deferred to v2.0+

These are explicit out-of-scope items from `.planning/v1.7-PLAN.md`
§"Out of scope for v1.7":

- **KERNEL MoleculeStore (SoA) parallel-model migration** — Phase 999.24,
  the lead v2.0 phase. See `.planning/KERNELV2.md` (554 lines,
  post-Codex Round 3 v3 baseline).
- **PIPE-01 — fixed-function GL → modern backend** — Phase 999.6
  (dormant). Spike (Phase 5) already decided: QRhi for v2.0.
- **INIFile → YAML config** — Phase 999.9.
- **In-tree mmCIF parser → gemmi** — Phase 999.11.
- **Bulk deprecated-code removal** — Phase 999.12 (GENETICDOCK subset
  pulled forward to v1.7 as Phase 999.29).
- **PyBALL v2 (autowrap+Cython vs nanobind bake-off)** — Phase 6 is
  the first step of v2.1; bulk wrap is Phase 999.15.
- **Remote-control narrowing → REST API + PyBALL SDK** — Phase 999.10
  (v2.2).
- **Read the Docs portal + Swagger UI** — Phase 999.13 (v2.3, last v2.x
  phase).

### 5.3 Active seeds (`.planning/seeds/`)

- `SEED-001-ballview-refresh-ui-milestone.md` — promoted-subset (Phases
  999.40-999.43 in v1.7; 999.44-999.48 in v1.8).
- `SEED-005-1-ci-aqtinstall-modules-fixup.md` — resolved by Phase 5.1 D2/D3/D6 + commit `54da903`.
- `SEED-005-2-ci-vcpkg-baseline-fixup.md` — resolved by Phase 4 vcpkg manifest + Phase 5.1 Tier-B.
- `SEED-005-3-windows-vm-driver-capture.md` — superseded; user-verified Windows VM render 2026-05-16.
- `SEED-005-4-post-pipe01-spike-recapture.md` — folds into Phase 999.6 (v2.0 PIPE-01).

---

## 6. Cross-references (phase index)

Use this index to drill from the retrospective into the source-of-truth
for any item above. All paths are relative to repo root.
ROADMAP entries live in `.planning/ROADMAP.md`. Per-phase artifacts live
under `.planning/phases/<phase-dir>/`.

### v1.6 active phases (substrate modernization)

| Phase | Title | Path |
|---|---|---|
| 1 | Build Baseline | `.planning/phases/01-build-baseline/` |
| 2 | Rendering Port (QGLWidget → QOpenGLWidget) | `.planning/phases/02-rendering-port-4a/` |
| 02.1 | Renderer boundary extraction | `.planning/phases/02.1-renderer-boundary-extraction/` |
| 02.2 | CI and build-smoke matrix | `.planning/phases/02.2-ci-and-build-smoke-matrix/` |
| 3 | Language Modernization (C++17) | `.planning/phases/03-language-modernization/` |
| 4 | Dependency System Overhaul (Homebrew/apt/vcpkg; `ball_contrib` removed; OpenBabel 3.x) | `.planning/phases/04-dependency-system-overhaul/` |
| 4.1 | Config Color-Defaults Fix (CONFIG-01) | `.planning/phases/04.1-config-color-defaults-fix/` |
| 5 | Qt 6 Migration + Renderer Backend Spike (SPIKE-02 decision: GL-Core for v1.6.x → QRhi for v2.0) | `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/` |
| 5.1 | Build Warnings & Latent Bug Cleanup (14 plans) | `.planning/phases/05.1-build-warnings-and-latent-bugs/` |
| 9 | Test Suite Triage (`ctest` wired, gatekeeper flipped) | `.planning/phases/09-test-suite-triage/` |

### v1.6.x patch phases (build acceleration + cleanups)

| Phase | Title | Path |
|---|---|---|
| 999.2 | Ninja build generator switch (Windows 4818 s → 55 s) | `.planning/phases/999.2-ninja-generator-switch/` |
| 999.16 | PCH for BALL + VIEW | `.planning/phases/999.16-pch-ball-view/` |
| 999.17 | Windows CMake build-tree cache | `.planning/phases/999.17-windows-build-tree-cache/` |
| 999.18 | Path-aware CI triggers + concurrency groups | `.planning/phases/999.18-path-aware-ci-triggers/` |
| 999.19 | Per-TU build profiling artifact | `.planning/phases/999.19-per-tu-build-profiling/` |
| 999.20 | Bump action artifact pins to v6/v7 | `.planning/phases/999.20-action-artifact-pins/` |
| 999.21 | DockResultFile QtXml dead-code cleanup | `.planning/phases/999.21-dockresultfile-dead-code/` |
| 999.22 | Whole-tree warning census (CENSUS-ONLY) | `.planning/phases/999.22-warning-census/` |
| 999.23 | CIF Bison grammar shift-reduce audit | `.planning/phases/999.23-cif-grammar-audit/` |
| 999.26 | atom.h C4910 single-line pragma suppress | `.planning/phases/999.26-atom-c4910-suppress/` |
| 999.27 | Coverage-build Linux compile-clean trio | (ROADMAP entry; no separate dir) |
| 999.28 | v1.6.2 warning-execution omnibus + Apple-GL silence | (ROADMAP entry) |
| 999.14 | GitHub issue + PR triage (incl. `GAMESSLog` removal #662) | `.planning/phases/999.14-github-issue-pr-triage/` |

### v1.7 phases (this milestone)

| Phase | Title | Path |
|---|---|---|
| 8a | Packaging macOS (sign + notarize + DMG) | `.planning/phases/08a-packaging-macos/` |
| 8b | Packaging Windows (SignPath → installer) | `.planning/phases/08b-packaging-windows/` |
| 8c | Packaging Linux (AppImage + DEB + RPM; LGPL-only SKU) | `.planning/phases/08c-packaging-linux/` |
| 999.1 | BALLView UI maintainer open-questions (RESOLVED) | `.planning/phases/999.1-ballview-ui-maintainer-open-questions/` |
| 999.4b | Residue color persistence (4.1 fix pattern) | `.planning/phases/999.4b-residue-color-persistence/` |
| 999.7 | Qt 6 Linux + Windows bring-up (CLOSED) | `.planning/phases/999.7-qt6-linux-windows-bringup/` |
| 999.8 | Sparkle + WinSparkle auto-update (spike + full) | `.planning/phases/999.8-auto-update-sparkle-winsparkle/` |
| 999.29 | DOCKING/GENETICDOCK removal | `.planning/phases/999.29-genetic-dock-removal/` |
| 999.37 | VRMLRenderer removal + STL-export rename | (ROADMAP entry; landed inline 2026-05-17) |
| 999.40 | BALLView Refresh — Design system & QSS foundation | (Wave 4 backlog dir; landed inline as commit `fdeca85b`) |
| 999.41 | BALLView Refresh — SVG icons + HiDPI pipeline | (Wave 4) |
| 999.42 | BALLView Refresh — QSS theming, palette removal | (Wave 4) |
| 999.43 | BALLView Refresh — Simple-dialog cleanup | (Wave 4) |
| **999.25** | **v1.4 → v1.7 retrospective (this document)** | `.planning/phases/999.25-v1.4-to-v1.7-retrospective/` |
| 999.25b | v1.4-era tutorial sample compile-check + update sweep (async) | (parallel agent, separate file tree) |

### v2.0+ deferred phases (substrate rewrite)

| Phase | Title | Milestone |
|---|---|---|
| 999.24 | KERNEL MoleculeStore (SoA) parallel-model migration — lead v2.0 phase | v2.0 (see `.planning/KERNELV2.md`) |
| 999.6 | PIPE-01 — fixed-function GL → modern backend (QRhi per Phase 5 decision) | v2.0 |
| 999.9 | INIFile → YAML config | v2.0 |
| 999.11 | In-tree mmCIF → gemmi | v2.0 |
| 999.12 | Bulk deprecated-code removal (residual after 999.29 + 999.37) | v2.0 |
| 6 | Python Bindings bake-off (autowrap+Cython vs nanobind) — lead v2.1 phase | v2.1 (see `.planning/PYBALLV2.md`) |
| 999.15 | PyBALL bulk wrap | v2.1 |
| 999.10 | REST API + PyBALL SDK | v2.2 |
| 999.13 | Read the Docs + Swagger UI | v2.3 |

---

## 7. Caveats

1. The v1.4 era predates this repository's tag history. Numbers and
   dates anchored to "v1.4 era" come from CVS `$Id$` keywords inside
   `doc/TUTORIAL/*.tex` and from `doc/examples/TUTORIAL/*.C`, not from
   reachable git refs. Phase 999.25b (parallel async sweep) reconciles
   those tutorials against the v1.7 API.

2. The "v1.6.x freeze" range is approximate. The pre-modernization
   working baseline is git commit `d85d2dd` (`"[CMake] Disable MPI
   support by default"`, ~2022) which Phase 1 patched-and-bumped to
   `1.6.0-dev`.

3. Where this document cites build times (e.g. "4818 s cold → 55 s warm
   Windows Build"), the numbers come from the Phase 999.2 SUMMARY
   verified on CI run `25953405453`. They are wall-clock, not CPU time,
   on GitHub-hosted runners as of 2026-05-16.

4. The "≥150 deprecation warnings eliminated by Phase 999.29" figure
   is from the Phase 999.29 verification log. Per-platform delta varies
   (linux-arm64 saw the largest absolute drop because `-Wdeprecated-
   declarations` fired more aggressively under GCC).

5. This document was authored by the Phase 999.25 executor on
   2026-05-17 against ROADMAP state at that time. The ROADMAP is the
   live source; this document is a snapshot.

---

## 8. See also

- `.planning/ROADMAP.md` — 63 phase entries, full execution history.
- `.planning/v1.7-PLAN.md` — wave structure, risk register, adversarial-review record.
- `.planning/PROJECT.md` — project framing.
- `.planning/REQUIREMENTS.md` — REQ traceability table.
- `.planning/RENDERER-INTERFACE-BOUNDARY.md` — the Phase 02.1 design that the Phase 5 spike validates.
- `.planning/KERNELV2.md` — the v2.0 MoleculeStore plan.
- `.planning/PYBALLV2.md` — the v2.1 Python-bindings plan.
- `.planning/DESIGN-HANDOVER-INTEGRATION.md` — the SEED-001 BALLView Refresh handover mapping.
- `BUILD-macos.md`, `BUILD-linux.md` — user-facing from-source build guides. (No `BUILD-windows.md` yet — see §5.1.)
- `RELEASE-NOTES-v1.6.1.md`, `RELEASE-NOTES-v1.6.2.md` — already-shipped user-facing notes for the patch releases that preceded v1.7.
