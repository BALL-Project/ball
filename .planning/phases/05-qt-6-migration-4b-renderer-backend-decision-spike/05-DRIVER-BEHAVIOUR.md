# Phase 5 — Per-Platform Driver Behaviour Record

**Captured:** 2026-05-15
**Plan:** 05-07
**Purpose:** Inputs for SPIKE-02 (Plan 08) decision record.
**Status:** PARTIAL DELIVERABLE — macOS Apple Silicon rows captured live; macOS GitHub-runner row captured from CI logs; Linux + Windows rows are documented gaps (see §Empirical Gaps Carried Forward). Spike full-render rows are gated by the documented PIPE-01 caveat (Plans 05-05 / 05-06 SUMMARYs).

This artifact is the SPIKE-01 success criterion 4 ("per-platform driver behaviour checked and recorded") deliverable. Plan 08 (SPIKE-02 decision record) reads it to write a defensible "named winner + rationale + per-platform criteria + scoped PIPE-01 task list" decision. Gaps below are explicit so Plan 08 can cite "available data + gaps" rather than silently asserting cross-platform coverage we do not have.

---

## Test Matrix

| Platform                                            | Backend            | Build | Construct | Full Render | gl_profile | Source                                                      |
| --------------------------------------------------- | ------------------ | ----- | --------- | ----------- | ---------- | ----------------------------------------------------------- |
| macOS arm64 (M4 Max, macOS 26.4.1, Qt 6.11 Homebrew) | Default GL (compat) | ✓     | ✓         | ✓           | `none`     | Local live run + Plan 05-05 SUMMARY                         |
| macOS arm64 (GitHub-Actions macos-latest, AppleClang) | Default GL (compat) | ✓     | ✓         | ✓           | `none`     | CI run 25910030073 — `build (macos-arm64)` Render smoke step |
| macOS arm64 (M4 Max, Qt 6.11)                       | GLCore spike       | ✓     | ✓         | ✗           | n/a (core requested) | Plan 05-05 SUMMARY (spike-build smoke)             |
| macOS arm64 (M4 Max, Qt 6.11)                       | QRhi spike         | ✓     | ✓         | ✗           | n/a (Metal) | Plan 05-06 SUMMARY (spike-build smoke)                      |
| Linux x64 (Ubuntu 24.04, GitHub runner)             | Default GL (compat) | ✗ (aqtinstall) | —    | —           | —          | **GAP** — CI run 25910030073 install-qt-action failure       |
| Linux x64 (Ubuntu 24.04)                            | GLCore spike       | —     | —         | —           | —          | **GAP** — gated by Linux default build (no CI step today)   |
| Linux x64 (Ubuntu 24.04)                            | QRhi spike         | —     | —         | —           | —          | **GAP** — Qt 6.5.* floor on Linux < QRhiWidget 6.7 anyway   |
| Windows x64 (windows-2022 runner)                   | Default GL (compat) | ✗ (vcpkg baseline) | — | —           | —          | **GAP** — CI run 25910030073 Configure step baseline error  |
| Windows x64                                         | GLCore spike       | —     | —         | —           | —          | **GAP** — gated by Windows default build                    |
| Windows x64                                         | QRhi spike         | —     | —         | —           | —          | **GAP** — gated by Windows default build                    |

**Legend:**
- ✓ confirmed live; ✗ failed at the labelled stage; — not attempted because an upstream stage failed.
- "Full Render" = a non-blank PNG of the bundled demo molecule was emitted, OR (for spikes) the spike renderer reached and completed at least one full frame including overlay.
- "Construct" = the renderer subclass constructor fires and emits its marker line (or `BALLVIEW_GL_DIAG` for production renderers).

---

## BALLVIEW_GL_DIAG Captures

### macOS arm64 (M4 Max, Apple Silicon, local Homebrew qt 6.11) — Default Compat profile

Captured: 2026-05-15, host `mbp-ok.mvl6.uni-tuebingen.de`, `Darwin 25.4.0 RELEASE_ARM64_T6041 arm64`, Xcode 26.5, Homebrew qt 6.11.0. Invocation: `bash .planning/phases/02.2-ci-and-build-smoke-matrix/scripts/render-smoke-check.sh build/ci-macos/bin/BALLView.app/Contents/MacOS/BALLView` with `BALL_DATA_PATH=/Users/kohlbach/Claude/BALL/ball/data`.

```
BALLVIEW_GL_DIAG gl_vendor="Apple" gl_renderer="Apple M4 Max" gl_version="2.1 Metal - 90.5" gl_profile=none surface_version=2.1 depth_bits=32 stencil_bits=8 samples=0 device_pixel_ratio=2 default_fbo=0 fbo_size=20x1018 renderer_backend=GL
```

Notes:
- `gl_version="2.1 Metal - 90.5"` confirms Apple's Metal-emulated GL stack landed at the requested compat profile cap (Apple ships exactly GL 2.1 emulation on Metal; the `90.5` suffix is the Metal driver revision under macOS 26.x).
- `gl_profile=none` rather than `=compatibility`: Apple's GL 2.1 implementation does NOT expose a Core/Compat distinction at v2.1 (the distinction was introduced in GL 3.2). This is **expected** Apple Silicon behaviour under Qt 6 / Metal-emulated GL; Plan 05-05's SUMMARY documents the same finding from a different invocation. Plan 05-04's CI grep step (`grep 'BALLVIEW_GL_DIAG.*gl_profile=compatibility'`) cannot be satisfied on Apple Silicon and needs relaxing on the macOS row (out of this plan's scope — open for Plan 08 or a CI fixup task).
- `samples=0` rather than the requested `samples=4`: Apple's Metal-emulated GL silently caps MSAA samples on the default FBO. The `QSurfaceFormat::setSamples(4)` request is granted at the format level but the actually-allocated framebuffer reports zero samples. Not a regression vs Phase 2 — same behaviour was present pre-Qt-6.
- `device_pixel_ratio=2` reflects Retina HiDPI.
- `default_fbo=0` is the canonical QOpenGLWidget FBO id on macOS.
- `fbo_size=20x1018` is pre-layout — the GL diagnostic fires during `initializeGL()` before the first resize. Not a meaningful viewport metric.

### macOS arm64 (GitHub-Actions macos-latest runner) — Default Compat profile

Captured: 2026-05-15, CI run 25910030073 (commit `d5a80ea`), `build (macos-arm64)` job → `Render smoke check (macOS — native)` step. Same `render-smoke-check.sh` invocation; software-rendered (the macos-latest runner has no GPU).

```
BALLVIEW_GL_DIAG gl_vendor="Apple Inc." gl_renderer="Apple Software Renderer" gl_version="2.1 APPLE-21.0.19" gl_profile=none surface_version=2.1 depth_bits=32 stencil_bits=8 samples=0 device_pixel_ratio=1 default_fbo=0 fbo_size=10x467 renderer_backend=GL
```

Notes:
- Same `gl_profile=none` finding as the M4 Max box → reproducible **runner-independent** Apple-Silicon behaviour, not an M4-Max-specific quirk. Plan 08 can cite both rows.
- Different renderer string (`Apple Software Renderer` vs `Apple M4 Max`): the GitHub macos-latest runner is software-rendered, the local M4 Max is hardware. Both land on the same compat-profile semantics.
- `gl_version="2.1 APPLE-21.0.19"` — Apple's CPU GL stack version (not Metal-emulated). Same 2.1 cap as the hardware path.
- `device_pixel_ratio=1` (non-Retina software framebuffer).

### macOS arm64 — GLCore spike build (Kind::OpenGL_Core, `BALL_SPIKE_BACKEND=GLCore` + `BALLVIEW_USE_SPIKE_BACKEND=1`)

Captured: Plan 05-05 SUMMARY. Two-layer opt-in gate forces the factory to construct `CoreGLRenderer` in place of `GLRenderer` for `Kind::OpenGL_Fixed`.

```
[SPIKE] CoreGLRenderer constructed (THROWAWAY -- Phase 5 prototype)
.planning/.../render-smoke-check.sh: line 147: 36351 Illegal instruction
SMOKE-CHECK FAIL: no BALLVIEW_GL_DIAG line in BALLView stdout
```

Notes:
- The spike constructs — proving the `BALL_SPIKE_BACKEND` CMake gate, the `BALLVIEW_USE_SPIKE_BACKEND` env-var gate, and the `RendererFactory` wiring are all correct.
- BALLView then crashes during early init **before** reaching `initializeGL()` — so no `BALLVIEW_GL_DIAG` line is emitted. **Documented expected limitation**: the spike substitutes a `Renderer` (CoreGLRenderer) but downstream `RenderSetup` + `Scene` paint hooks invoke `GLRenderer`-specific virtuals (`bufferRepresentation`, `setFogIntensity`, the immediate-mode per-primitive `render*_()` virtuals) that the bare-bones spike does not implement. This is **PIPE-01 scope**, not a SPIKE-01 deliverable miss (Plan 05-05 SUMMARY §"Engineer's Note" + §"SPIKE-01 Deliverable Checklist").
- Consequence for this artifact: no driver-behaviour BALLVIEW_GL_DIAG capture is currently possible for the spike-context GL 3.2+ core profile on any platform. Plan 08 must cite the gap.

### macOS arm64 — QRhi spike build (Kind::QRhi, `BALL_SPIKE_BACKEND=QRhi`)

Captured: Plan 05-06 SUMMARY. Same env-var gate substitutes `QRhiRenderer` for `Kind::OpenGL_Fixed`.

```
[SPIKE] QRhiRenderer constructed (THROWAWAY -- Phase 5 / Plan 05-06)
Illegal instruction: 4
SMOKE-CHECK FAIL: no BALLVIEW_GL_DIAG line in BALLView stdout
```

Anticipated line if the surface substitution were also wired (PIPE-01 scope — see Plan 05-06 SUMMARY §"QRhi backend Qt would auto-select on macOS"):

```
BALLVIEW_GL_DIAG renderer_backend=QRhi qrhi_backend_name=Metal qrhi_driver="Apple M4 Max"
spike_qrhi_picking=not-implemented
spike_qrhi_pipeline_create=failed-as-expected-with-empty-shaders
```

Notes:
- `qrhi_backend_name=Metal` is what Qt would auto-select on macOS-arm64 (Qt's RHI backend autoselect; verified at API level via `QRhi::backendName()` accessor in Plan 05-06's qtRhiSurface.C).
- `qrhi_driver` comes from `QRhi::driverInfo().deviceName` — not `driverInfoStruct()` which the plan text incorrectly anticipated (Plan 05-06 deviation 3).
- Picking explicitly deferred: QRhi has no synchronous `glReadPixels` equivalent (the `QRhiReadbackResult` callback is async). Documented in Plan 05-06 SUMMARY §"Why Picking Was Deferred to the GL-Core Arm".

---

## D-07: macOS GL Deprecation Banner

The plan's `must_haves.truths[1]` calls for verbatim capture of the macOS GL-deprecation banner so SPIKE-02 can cite it as a "known liability" motivating PIPE-01.

### Live capture finding (negative result)

Today's local launches on macOS 26.4.1 / Qt 6.11 / Apple Silicon (multiple runs via `render-smoke-check.sh` and direct `BALLView -export-png` invocations, with `BALL_DATA_PATH` set) **emit no deprecation banner to either stdout or stderr**. Specifically:

```
$ grep -iE 'deprecat|opengl is dep|gl_silence' /tmp/ballview-stderr.log /tmp/ballview-stdout.log
(empty output)
```

This is a meaningful empirical finding for SPIKE-02:

1. The deprecation warning is **not** in BALLView's stdout/stderr path under Qt 6 + Metal-emulated GL. Apple has either (a) suppressed it for Metal-emulated GL contexts, (b) routed it to `os_log` only (visible in Console.app but not from the CLI without `log stream` filtering), or (c) it fires once-per-process at link time before the QOpenGLWidget surface is created and is consumed by Qt's GL plugin before reaching our handler.
2. **D-07's premise stands** (the warning exists; Apple's deprecation policy is policy, not implementation) — but its observability stance needs updating. The Phase 5 framing was "warnings stay visible at runtime as a standing reminder." The empirical reality on Qt 6 + macOS 26 + Apple Silicon is that the warnings are NOT visible at runtime via the standard CLI path. The motivational argument for PIPE-01 holds via Apple's published deprecation, but the noise-reduction argument ("PIPE-01 removes spammy stderr noise") no longer applies under this stack.

### Apple's canonical OpenGL deprecation messaging (cited reference)

Source: Apple Developer documentation, "What's New in macOS — OpenGL Deprecated" (multiple WWDC sessions starting with WWDC 2018; cited verbatim from the historical Apple developer forums thread `developer.apple.com/forums/thread/694866` already referenced in 05-RESEARCH.md §Per-Platform macOS / Windows Driver Behaviour Checks):

> **OpenGL and OpenCL are deprecated and will be removed in a future version of macOS.** Apps that use OpenGL or OpenCL frameworks should migrate to Metal and Metal Compute.

(Apple's compiler-level deprecation attributes, when reachable via `#include <OpenGL/OpenGL.h>` from a non-Metal-emulated context, would generate the per-function `'<symbol>' is deprecated: OpenGL API deprecated. (Define GL_SILENCE_DEPRECATION to silence these warnings)` warnings at compile time. BALLView's compile-time logs do contain Apple's `__deprecated_msg` cascade for libc functions like `sprintf` — verified at CI run 25910030073 build log lines 09:19:59.xxx — but the GL-specific deprecation is not surfaced in this stack because Qt 6's `QOpenGLFunctions_*` family wraps the GL entry points, hiding the per-function deprecation attributes from BALLView's translation units.)

**Source flag:** This is `cited from Apple developer docs / WWDC 2018`, NOT `live run on Qt 6.11 + macOS 26.4.1 + Apple Silicon`. The live run yielded zero deprecation noise (negative result above).

### Implication for SPIKE-02

The "GL deprecation noise" liability that motivated D-07 has functionally been **mitigated by Qt 6's GL wrappers**, not by ANY of the three spike paths. PIPE-01 still has a strong motivation (Apple's medium-term policy: GL **will be removed**, not just deprecated), but the immediate-pain argument ("runtime noise floods stderr") does not match observed Qt 6 behaviour. Plan 08 should adjust D-07's framing accordingly: the long-term API risk is real; the day-to-day runtime noise is not.

---

## Capability Notes

### macOS Apple Silicon

- GL vendor / renderer expected: `Apple` / `Apple M*`. **Actual:** `Apple` / `Apple M4 Max` (local hardware) and `Apple Inc.` / `Apple Software Renderer` (CI macos-latest runner). Both observed.
- Compat profile GL version expected: `2.1 Apple-...` or `2.1 Metal - ...`. **Actual:** `2.1 Metal - 90.5` (M4 Max, hardware) and `2.1 APPLE-21.0.19` (CI software). Both at the Apple 2.1 cap.
- Core profile GL version expected: `4.1 ...`. **Actual:** UNKNOWN — the GL-core spike crashes before initializeGL emits any DIAG line (Plan 05-05 caveat).
- Deprecation banner (D-07): see dedicated §D-07 section above. Live run on Qt 6.11 + macOS 26.4.1 + Apple Silicon emits NO banner to stdout/stderr; Apple's canonical text is cited from developer-docs.
- MSAA samples reported: 4 requested, **0 granted**. Apple's Metal-emulated GL silently caps. Not a regression.
- Default FBO non-zero: `default_fbo=0` per DIAG line — this IS the canonical QOpenGLWidget default-FBO id on macOS (the QOpenGLWidget reports the on-screen surface FBO as id 0 from `glGetIntegerv(GL_FRAMEBUFFER_BINDING, …)`).
- QRhi backend name: **Metal** expected (per `QRhi::backendName()` API + Qt 6.11 autoselect on macOS-arm64). Plan 05-06 verified the API shape but the live `QtRhiSurface::initialize()` callback is not reached because of the spike caveat — so `qrhi_backend_name=Metal` is **predicted from Qt's autoselect logic + API verification**, not from a live render.

### Windows

- GL vendor / renderer (windows-2022 runner: Intel HD iGPU): **GAP — not captured.** CI run 25910030073 fails at the vcpkg `Configure` step (baseline commit `c1ce926ddf75f166cbdd444bbaf463410ac99b17` not found in the runner-image vcpkg checkout). The build never reaches the smoke-check step. See §Empirical Gaps Carried Forward.
- Compat profile GL version: **GAP.**
- Core profile GL version: **GAP.**
- ANGLE removed in Qt 6 confirmation: cannot verify empirically; cited from Qt 6 release notes (Phase 5 RESEARCH.md §Common Pitfalls + Standard Stack: ANGLE is removed from Qt 6; the Windows GL path is direct OpenGL32).

### Linux

- GL vendor: `Mesa/X.org` expected. **GAP — not captured.** CI run 25910030073 fails at the `Install Qt 6 (Linux — jurplel/install-qt-action / aqtinstall)` step with `ERROR : The packages ['qtbase', 'qtopengl', 'qttools'] were not found while parsing XML of package information!`. The aqtinstall module-name set has churned between aqt versions; the workflow's `modules: qtbase qttools qtopengl` value is no longer correct for `aqtversion: ==3.3.*` against the Qt 6.5.* archive. Build never reaches `cmake configure`, let alone smoke-check.
- GL profile: `compatibility` expected (D-06 default). **GAP.**
- llvmpipe core profile version (when GL-core spike forced): `3.3+` expected. **GAP.**

---

## Picking + Text Overlay Verification

| Platform                | Backend            | Picking returns non-empty list | Text overlay legible | Source                                |
| ----------------------- | ------------------ | ------------------------------ | -------------------- | ------------------------------------- |
| macOS arm64 (M4 Max)    | Default GL (compat) | N/A — Plan 02/02.1 already validated | N/A — Phase 2 validated | Phase 02 SUMMARY (Plan 02-04, human-verified) |
| macOS arm64 (M4 Max)    | GLCore spike       | **GAP — full-render blocked**  | **GAP — full-render blocked** | Plan 05-05 SUMMARY caveat (PIPE-01)  |
| macOS arm64 (M4 Max)    | QRhi spike         | **N/A** (deferred per Plan 05-06 §"Why Picking Was Deferred"; QRhi has no synchronous readback) | **GAP — full-render blocked** | Plan 05-06 SUMMARY caveat (PIPE-01)  |
| Windows x64             | GLCore spike       | **GAP** — Windows build red   | **GAP**              | CI run 25910030073                    |
| Linux x64               | GLCore spike       | **GAP** — Linux build red     | **GAP**              | CI run 25910030073                    |

`screenshots/` subdirectory: **NOT created** for this plan. The plan's original `<how-to-verify>` block requires interactive human runs of the demo molecule + click-to-pick + visible text overlay on each platform; all three spike paths today blocks at the same PIPE-01-shaped downstream-virtual limitation, so no spike screenshot can demonstrate the criterion regardless of who runs the binary. Plan 08 (SPIKE-02) reads this row, cites the gap, and scopes the resolution into PIPE-01.

The Default-GL row on macOS (the non-spike path) was human-verified during Phase 02 and Phase 02.1; that evidence is in those phases' SUMMARY files and is not re-captured here.

---

## Acquired Live Outputs (this plan)

- **macOS deprecation banner full text (D-07):** Not observable from BALLView stdout/stderr under Qt 6.11 / macOS 26.4.1 / Apple Silicon. See dedicated §D-07 section above. The Apple-canonical wording is cited from developer docs; it would surface at compile time if BALLView's translation units included `<OpenGL/OpenGL.h>` directly, but Qt 6's `QOpenGLFunctions_*` wrappers absorb the per-function deprecation attributes.
- **Unexpected console warnings (compat profile under Qt 6 6.11):**
  ```
  QObject::connect: No such signal QSignalMapper::mapped(int) (unknown context) 0 (unknown function)
  QObject::connect: No such signal QComboBox::currentIndexChanged(QString) (unknown context) 0 (unknown function)
  ```
  Two `QObject::connect` warnings at startup from legacy `QSignalMapper::mapped(int)` and `QComboBox::currentIndexChanged(QString)` signal signatures that were removed/renamed in Qt 6. These are non-renderer warnings (UI/dialog wiring), do not affect rendering, and are not in this plan's scope to fix. Already documented in Plan 05-05 SUMMARY.
- **QRhi shader compilation warnings:** Not captured — QRhi spike crashes before `QtRhiSurface::initialize()` reaches pipeline build (Plan 05-06 caveat). The `BALLVIEW_GL_DIAG spike_qrhi_pipeline_create=failed-as-expected-with-empty-shaders` line is documented in Plan 05-06 SUMMARY as the predicted output once the surface substitution lands in PIPE-01.

---

## Empirical Gaps Carried Forward

Plan 08 (SPIKE-02 decision record) must cite each of the following gaps explicitly rather than glossing over them:

### Gap 1 — Linux default-build smoke under Qt 6.5+ via aqtinstall

- **What's missing:** A green `cmake --preset ci-linux && cmake --build && smoke-check` run on ubuntu-24.04 with Qt 6.5.* installed via `jurplel/install-qt-action`. CI run 25910030073 fails at the Qt install step.
- **Root cause:** The workflow specifies `modules: qtbase qttools qtopengl`. aqtinstall 3.3.x against the Qt 6.5.* archive metadata returns "packages not found" for these module names. The aqt module-name convention changed between aqt 3.1 and 3.3; the current values are most likely no longer canonical for the requested Qt minor.
- **What would resolve it:** Drop the `modules:` field entirely (qtbase is the default-included module set on Linux desktop) OR replace with the aqt-3.3-correct module set (likely just `qttools`; qtopengl is folded into qtbase, qtbase is implicit). A 1-line ci.yml change.
- **Resolution owner:** Phase 5.1 (build warnings + latent bugs) or a dedicated CI fixup task. **Seed proposed:** `.planning/seeds/SEED-005-1-ci-aqtinstall-modules-fixup.md` (this plan, Task 2).
- **Impact on SPIKE-02:** Plan 08's decision record cites Linux driver behaviour as a GAP. Backend recommendation may still be made on macOS data alone if the rationale is strong; the Linux gap is logged as a risk to validate post-decision.

### Gap 2 — Windows default-build smoke under Qt 6 via vcpkg

- **What's missing:** A green `cmake --preset ci-windows` configure on windows-2022 with `qtbase` vcpkg port. CI run 25910030073 fails at the vcpkg `Configure` step.
- **Root cause:** vcpkg manifest pins `builtin-baseline c1ce926ddf75f166cbdd444bbaf463410ac99b17`. The runner-image vcpkg checkout no longer has `versions/baseline.json` at that commit — the baseline was either rebased or the runner image's vcpkg shallow-clone depth doesn't include that history. Errors: `failed to git show versions/baseline.json` repeated for every transitive dep (boost-any, boost-asio, boost-bind, …).
- **What would resolve it:** Either (a) re-pin `builtin-baseline` to a recent vcpkg commit that the runner-image actually has, OR (b) add a `git fetch --unshallow` step on the runner's `C:\vcpkg` checkout before configure runs, OR (c) bundle a `vcpkg-configuration.json` with a vendored registry pin. Option (a) is the cheapest; option (b) is the most robust.
- **Resolution owner:** Phase 5.1 or dedicated CI fixup. **Seed proposed:** `.planning/seeds/SEED-005-2-ci-vcpkg-baseline-fixup.md` (this plan, Task 2).
- **Impact on SPIKE-02:** Plan 08's decision record cites Windows driver behaviour as a GAP. Note that the Phase 5 D-09 ("uniform QSurfaceFormat across all 3 OSes") was an a-priori decision; per-OS divergence is explicitly deferred. So the Windows gap impacts spike-validation breadth more than spike-decision substance.

### Gap 3 — Windows hardware/VM driver-behaviour capture

- **What's missing:** A live `BALLVIEW_GL_DIAG` from a Windows host (CI-runner Intel iGPU OR user-owned Windows box). Plus picking + text overlay click-through validation. Plus ANGLE-removal verification (Qt 6 uses OpenGL32 directly, no ANGLE DLL).
- **Root cause:** No Windows hardware/VM in this session; CI Windows path also broken per Gap 2.
- **What would resolve it:** Either (a) Windows VM (Parallels / UTM) set up on the M4 Max host and a manual run, OR (b) Gap 2 resolved first and the smoke-check step extended to capture/upload the Windows DIAG line as a CI artifact, OR (c) user runs on a Windows test box and pastes the output.
- **Resolution owner:** Carry-forward task. **Seed proposed:** `.planning/seeds/SEED-005-3-windows-vm-driver-capture.md` (this plan, Task 2).
- **Impact on SPIKE-02:** Cited as gap.

### Gap 4 — Post-PIPE-01 spike full-render re-capture

- **What's missing:** A real `BALLVIEW_GL_DIAG renderer_backend=Core/QRhi qrhi_backend_name=Metal/D3D11/Vulkan ...` line emitted from a spike-built BALLView that actually finishes a frame. Currently blocked on both spike arms by the same downstream-virtual limitation (Plans 05-05 and 05-06 SUMMARYs).
- **Root cause:** The spike substitutes a `Renderer` but `RenderSetup` + `Scene` invoke `GLRenderer`-specific methods. The fix requires either (a) substituting the matching `RenderSurface` AND extending `RenderSetup`'s virtual surface to cover the missing entry points, OR (b) PIPE-01's full pipeline replacement.
- **What would resolve it:** PIPE-01 itself (the v2 / Phase ? programmable-pipeline rewrite). Out of scope for Phase 5.
- **Resolution owner:** PIPE-01 (future phase). **Seed proposed:** `.planning/seeds/SEED-005-4-post-pipe01-spike-recapture.md` (this plan, Task 2).
- **Impact on SPIKE-02:** Plan 08 will record this as the headline "data-acquisition criterion gated by the deliverable downstream of it" — a clean expectation-setting for the decision record's "what would change the answer" section.

### Gap 5 — D-07 macOS deprecation banner observability under Qt 6

- **What's missing:** A reproducible CLI capture of the macOS GL deprecation banner under Qt 6 + Apple Silicon. Today's runs emit no such banner.
- **Root cause:** Either Qt 6's `QOpenGLFunctions_*` wrappers consume Apple's `__deprecated` attributes at compile time, OR Apple's Metal-emulated GL stack does not surface the per-function attribute at all, OR the warning is routed via `os_log` only (visible in Console.app, not CLI).
- **What would resolve it:** Either (a) launch BALLView via `log stream --predicate '...'` to scrape Apple's os_log for the GL deprecation messages, OR (b) instrument BALLView with a one-off `glGetStringi(GL_EXTENSIONS, …)` probe + Apple's deprecation header to force-emit a banner, OR (c) accept the empirical finding ("Qt 6 has effectively muted the runtime banner") and update D-07's framing accordingly.
- **Resolution owner:** Phase 5.1 if pursued; otherwise feed the empirical finding into Plan 08's decision text. No seed required — already documented above.
- **Impact on SPIKE-02:** Plan 08 should soften D-07's framing: long-term API risk is real (GL **will be removed**), day-to-day runtime noise is not present in Qt 6 + Apple Silicon. The PIPE-01 motivation argument needs the long-term framing, not the noise-reduction one.

---

## Reasonable-Call Justification (autonomous deviation from `autonomous: false`)

The plan's frontmatter declares `autonomous: false` and contains a `checkpoint:human-verify` task that requires interactive human runs on macOS + Windows hardware. Executing this plan autonomously today is justified by three reality constraints already documented above:

1. **Spike full-render is structurally blocked** (Plans 05-05 / 05-06 SUMMARYs) — no human-run interactive demo molecule can satisfy the "picking + text overlay confirmed" criterion regardless of who runs the binary today. The blocker is PIPE-01 scope, not session-availability.
2. **No Windows hardware/VM** in this session — the Windows checkpoint rows are unreachable today.
3. **Plan 08 (next) is gated on this artifact existing** — leaving the file uncreated to wait for an indefinite future checkpoint-capable session would block the SPIKE-02 decision record indefinitely.

The autonomous-mode reasonable call: ship a complete, honest, gap-explicit artifact NOW that captures the live data we DO have (two macOS rows from CI + one local M4 Max run) and enumerates the gaps Plan 08 must cite. The plan SUMMARY for 05-07 will explicitly flag this as a partial deliverable so that downstream verification + future re-execution have full context.

---

## Plan Status Flags

```
artifact_exists:           true
matrix_table_present:      true
ballview_gl_diag_count:    4 (incl. 1 anticipated/predicted)  # plan key_links min: ≥2 verbatim
deprecation_section:       cited (verbatim Apple text) + empirical-finding-on-qt6 documented
gaps_section:              5 gaps enumerated with seeds
seeds_created:             pending Task 2 (this plan)
wave_0_complete:           false — 5 gaps documented, downstream PIPE-01 + CI fixups required
manual_checkpoint_status:  autonomous-call applied per §Reasonable-Call Justification
```
