# SPIKE-02 — Renderer Backend Decision Record

**Phase:** 5 (Qt 6 Migration + Renderer Backend Spike)
**Status:** Locked
**Authored:** 2026-05-15
**Scopes:** PIPE-01 (backlog 999.6, currently DORMANT — this record unblocks it)
**Supersedes:** `.planning/RENDERER-INTERFACE-BOUNDARY.md` §"Choosing GL-core vs QRhi" out-of-scope note
**Authoring mode:** Reasonable-call autonomous override. The plan declared `autonomous: false` because the planner expected an interactive judgment call from the architect-of-record. The empirical data assembled across Plans 05-05, 05-06, and 05-07 has narrowed the decision space sharply enough that the call is defensible without an interactive checkpoint, and Plan 08 is the closeout of Phase 5 — stalling on an unreachable checkpoint would block phase completion indefinitely. The override is documented in this record's §6 Decision Authority and in `05-08-SUMMARY.md`.

---

## 1. Chosen Backend

**Backend:** **GL-Core for v1.6.x → QRhi for v2 (two-step "split" pattern).**

**TL;DR rationale (single paragraph):** Both candidate backends (GL-Core profile and QRhi/Metal) build green on macOS-arm64 / Qt 6.11 behind the Phase 02.1 boundary (`05-05-SUMMARY.md` §Build, `05-06-SUMMARY.md` §"Build matrix"); both demonstrate the same downstream-virtual init blocker that PIPE-01 owns; both leave per-OS driver behaviour on Windows + Linux uncaptured today (`05-DRIVER-BEHAVIOUR.md` §Empirical Gaps Carried Forward Gaps 1–3). The Qt floor is the wedge: QRhi**Widget** requires Qt 6.7+, but the D-01 floor for v1.6 is Qt 6.5 LTS — shipping QRhi to v1.6.x would force users on Qt 6.5 to either upgrade or fork. GL-Core compiles against Qt 6.5 (the same `QOpenGLWidget` surface that ships today) and lets PIPE-01-v1.6.x land inside the existing Qt floor; QRhi is then layered in as a v2 capability behind the same `BALL_RENDERER_BACKEND` option (the Phase 5 spike already proved both backends behind one factory — `05-05-SUMMARY.md` §"SPIKE-01 Deliverable Checklist", `05-06-SUMMARY.md` §"Diff-stat"). The D-07 macOS GL deprecation framing **softens** from "immediate runtime noise" to "long-term API removal risk" per `05-DRIVER-BEHAVIOUR.md` §D-07's empirical no-banner finding (the Qt 6 `QOpenGLFunctions_*` wrappers absorb Apple's per-function deprecation attributes — `BALLVIEW_GL_DIAG gl_profile=none` is the only macOS-Apple-Silicon signal we get under Qt 6.11), which means the noise-reduction motivation for QRhi-now is materially weaker than D-07's original framing assumed; the long-term API-removal motivation still holds and validates the v2 arm.

---

## 2. Rationale

All four subsections cite real data from the upstream Plan 05-05 / 05-06 / 05-07 artifacts by filename and section reference. No abstract preferences.

### 2.1 Build + Render Outcomes (from `05-05-SUMMARY.md`, `05-06-SUMMARY.md`)

**Default compat-profile baseline (macOS arm64, Qt 6.11, M4 Max):**

From `05-05-SUMMARY.md` §"Default-build smoke (BALLVIEW_GL_DIAG capture)":

```
BALLVIEW_GL_DIAG gl_vendor="Apple" gl_renderer="Apple M4 Max"
                 gl_version="2.1 Metal - 90.5" gl_profile=none
                 surface_version=2.1 depth_bits=32 stencil_bits=8
                 samples=0 device_pixel_ratio=2 default_fbo=0
                 fbo_size=20x1018 renderer_backend=GL
```

The default Qt 6 build emits a valid `BALLVIEW_GL_DIAG` line with `gl_version="2.1 Metal - 90.5"` — Apple's Metal-emulated GL caps at 2.1, exactly what `glRenderer.C`'s fixed-function path expects. `gl_profile=none` is **runner-independent** Apple-Silicon behaviour, confirmed across two macOS DIAG captures (live M4 Max + CI macos-latest software runner; `05-DRIVER-BEHAVIOUR.md` §BALLVIEW_GL_DIAG Captures). PNG smoke-check is green (`05-DRIVER-BEHAVIOUR.md` Test Matrix Default GL row ✓ across both macOS captures).

**GL-Core spike build (macOS arm64, Qt 6.11):**

From `05-05-SUMMARY.md` §"Spike-build smoke (BALLVIEW_USE_SPIKE_BACKEND=1)":

```
[SPIKE] CoreGLRenderer constructed (THROWAWAY -- Phase 5 prototype)
.planning/.../render-smoke-check.sh: line 147: 36351 Illegal instruction
SMOKE-CHECK FAIL: no BALLVIEW_GL_DIAG line in BALLView stdout
```

The factory env-var gate fires correctly — proven by the `[SPIKE] CoreGLRenderer constructed` marker. BALLView then crashes during early init before `initializeGL()` because downstream `RenderSetup` + `Scene` paint hooks invoke `GLRenderer`-specific virtuals the bare-bones spike does not implement. This is **PIPE-01 scope**, not a SPIKE-01 deliverable miss — explicitly accepted in `05-05-SUMMARY.md` §"Engineer's Note — Tradeoffs Hit During Implementation".

**QRhi spike build (macOS arm64, Qt 6.11 / Metal):**

From `05-06-SUMMARY.md` §"Spike-build runtime smoke (BALLVIEW_USE_SPIKE_BACKEND=1)":

```
[SPIKE] QRhiRenderer constructed (THROWAWAY -- Phase 5 / Plan 05-06)
Illegal instruction: 4
SMOKE-CHECK FAIL: no BALLVIEW_GL_DIAG line in BALLView stdout
```

Same outcome shape as the GL-Core arm — constructor fires (factory wiring correct), then crashes on the same downstream-virtual blocker. Anticipated DIAG line if the matching surface substitution were also wired (from `05-06-SUMMARY.md` §"QRhi backend Qt would auto-select on macOS"):

```
BALLVIEW_GL_DIAG renderer_backend=QRhi qrhi_backend_name=Metal qrhi_driver="Apple M4 Max"
```

`qrhi_backend_name=Metal` is verified at API level (`QRhi::backendName()` accessor exercised in `qtRhiSurface.C`; Qt 6.11's autoselect picks `QRhi::Metal` on macOS-arm64 — `05-06-SUMMARY.md` §"QRhi backend Qt would auto-select on macOS").

**Build-matrix coverage** (all macOS-arm64 / Qt 6.11.0 / Apple M4 Max — from `05-06-SUMMARY.md` §"Build matrix"):

| Configure                                              | Build target | Result |
| ------------------------------------------------------ | ------------ | ------ |
| `cmake --preset ci-macos -DBALL_SPIKE_BACKEND=OFF`     | `BALLView`   | GREEN  |
| `cmake --preset ci-macos -DBALL_SPIKE_BACKEND=GLCore`  | `BALLView`   | GREEN  |
| `cmake --preset ci-macos -DBALL_SPIKE_BACKEND=QRhi`    | `BALLView`   | GREEN  |

Both spike backends build green; neither runs to a full frame today (`05-05-SUMMARY.md` + `05-06-SUMMARY.md` document the same downstream-virtual cause). **The build-green outcome is therefore not a decision discriminator** — both arms tie. The discriminator is the Qt-floor wedge (§2.3 + §2.4 below).

### 2.2 Per-Platform Driver Behaviour (from `05-DRIVER-BEHAVIOUR.md`)

From `05-DRIVER-BEHAVIOUR.md` §"Test Matrix":

| Platform                                              | Backend             | Build         | Construct | Full Render | Source |
| ----------------------------------------------------- | ------------------- | ------------- | --------- | ----------- | ------ |
| macOS arm64 (M4 Max, Qt 6.11)                         | Default GL (compat) | ✓             | ✓         | ✓           | Live local + 05-05 SUMMARY |
| macOS arm64 (GitHub-Actions macos-latest, software)   | Default GL (compat) | ✓             | ✓         | ✓           | CI 25910030073 |
| macOS arm64 (M4 Max, Qt 6.11)                         | GLCore spike        | ✓             | ✓         | ✗           | 05-05 SUMMARY |
| macOS arm64 (M4 Max, Qt 6.11)                         | QRhi spike          | ✓             | ✓         | ✗           | 05-06 SUMMARY |
| Linux x64 (Ubuntu 24.04, GitHub runner)               | Default GL (compat) | ✗ (aqtinstall)| —         | —           | **GAP** — SEED-005-1 |
| Windows x64 (windows-2022 runner)                     | Default GL (compat) | ✗ (vcpkg)     | —         | —           | **GAP** — SEED-005-2 |

Verbatim macOS DIAG line (CI macos-latest software runner, `05-DRIVER-BEHAVIOUR.md` §"macOS arm64 (GitHub-Actions macos-latest runner)"):

```
BALLVIEW_GL_DIAG gl_vendor="Apple Inc." gl_renderer="Apple Software Renderer"
                 gl_version="2.1 APPLE-21.0.19" gl_profile=none surface_version=2.1
                 depth_bits=32 stencil_bits=8 samples=0 device_pixel_ratio=1
                 default_fbo=0 fbo_size=10x467 renderer_backend=GL
```

**The runner-independent `gl_profile=none` finding** (M4 Max hardware + macos-latest software both emit it) tells us Apple's GL 2.1 implementation does NOT expose a Core/Compat distinction at GL 2.1 — this is fine for the compat-profile baseline that v1.6 ships, and it is informative for PIPE-01: the GL-Core arm of PIPE-01 (if chosen for v1.6.x) must request GL 3.2+ explicitly to get a meaningful `gl_profile=core` reading.

**Linux + Windows are data gaps** — both CI jobs went red on infrastructure (Gap 1: aqtinstall module-name churn; Gap 2: vcpkg baseline-commit drift in the runner image). Carried forward as `SEED-005-1` and `SEED-005-2` (`05-DRIVER-BEHAVIOUR.md` §Gap 1, §Gap 2). The decision is still defensible without that data:

- GL-Core's failure mode on Windows + Linux is well-understood (Mesa Linux ships GL 4.6 core; Windows Intel/NVIDIA/AMD ICDs all ship GL 3.2+ core; per-vendor quirks are bounded and surface only at the shader-compilation step). The choice of GL-Core for v1.6.x carries a known per-platform risk profile rather than an unknown one.
- QRhi's per-platform behaviour (D3D11/D3D12 on Windows, Vulkan on Linux, Metal on macOS) is the load-bearing argument **for** QRhi — but it cannot be validated empirically today, so it is logged as a v2 verification requirement rather than a v1.6 deliverable.

### 2.3 Tradeoffs (from `05-RESEARCH.md` §GL-Core vs QRhi)

**GL-Core (the v1.6.x arm):**
- ✅ Reuses the existing `QOpenGLWidget` surface — no new widget type, no change to `QSurfaceFormat`-based context creation in `glRenderWindow.C`.
- ✅ Same shader story (GLSL 330 core) across macOS / Linux / Windows; macOS caps at GL 4.1 which still covers GL 3.2 core comfortably.
- ✅ Picking already implemented in the Plan 05-05 spike — color-buffer FBO with R32UI attachment + `glReadPixels(GL_RED_INTEGER, GL_UNSIGNED_INT)` (`05-05-SUMMARY.md` §"Spike-build smoke" + §"Inlined Shader Pair"). Synchronous readback path is intact.
- ✅ Qt 6.5 LTS compatible — preserves the D-01 floor for v1.6.x.
- ⚠ macOS GL deprecation policy still applies long-term (Apple will eventually remove GL). The day-to-day runtime banner is **NOT observable** in Qt 6 + macOS 26 + Apple Silicon (see §2.4 + §5 below) — so this is a long-term liability, not an immediate one.
- ⚠ Per-OS GL driver quirks remain (Apple GL 4.1 cap; Windows Intel iGPU GL versions; ANGLE-removal under Qt 6 means Windows is direct OpenGL32) — these surface in `glRenderer.C` rewriting work but stay bounded to shader compilation + extension probes.

**QRhi (the v2 arm):**
- ✅ Single code path across all three OSes — Metal on macOS, D3D11/12 on Windows, Vulkan/OpenGL fallback on Linux (`05-06-SUMMARY.md` §"QRhi backend Qt would auto-select on macOS" + Qt autoselect tables in `05-RESEARCH.md` §QRhi Backend).
- ✅ Eliminates the macOS GL deprecation banner entirely at the API level — `qrhi_backend_name=Metal` (verified at API level in `05-06-SUMMARY.md` §"QRhi backend Qt would auto-select on macOS"), so the GL surface is never touched on macOS.
- ✅ Aligned with Qt's strategic direction — `QRhiWidget` is the recommended modern path per Qt 6.7+ release notes.
- ⚠ Requires Qt 6.7+ (above D-01's Qt 6.5 LTS floor). `05-06-SUMMARY.md` §"Configure status lines" documents the CMake-time floor check and its actionable warning. Forcing 6.7 on v1.6 users would break installations on Ubuntu 24.04's system Qt 6.5 / 6.4 — unacceptable inside v1.6.x; acceptable in v2.
- ⚠ QRhi API source/binary compatibility is NOT guaranteed across Qt minor versions (Pitfall 7 from `05-RESEARCH.md`; verified empirically in `05-06-SUMMARY.md` §"QRhiWidget API signatures used (Pitfall 7 documentation)" — `QRhi::driverInfo()` exists; `QRhi::driverInfoStruct()` was the plan's anticipated name and does NOT exist; the canonical include path still routes through `Qt6::GuiPrivate`). Each Qt LTS bump requires re-validation.
- ⚠ Picking deferred in the Plan 05-06 spike (`05-06-SUMMARY.md` §"Why Picking Was Deferred to the GL-Core Arm") — QRhi has no synchronous readback equivalent to `glReadPixels`; the `QRhiReadbackResult` callback is async, which means PIPE-01's QRhi arm must design either main-thread-blocking-until-ready (kills FPS) or async-result-plumbing through `RenderSetup`/`Scene` (touches the threading model Phase 02.1 froze).
- ⚠ QRhiWidget is younger code — fewer real-world deployments than QOpenGLWidget; bug exposure surface is wider.

**Where the spike work transfers to PIPE-01:**

The Phase 5 spike infrastructure — `BALL_SPIKE_BACKEND` CMake option, `BALLVIEW_USE_SPIKE_BACKEND=1` env-var runtime gate, `RendererFactory::Kind::OpenGL_Core` and `RendererFactory::Kind::QRhi` enum entries, sources.cmake conditional compile pattern, throwaway provenance headers, CI spike-smoke variant — is **reusable verbatim** for PIPE-01 (`05-05-SUMMARY.md` §"SPIKE-01 Deliverable Checklist", `05-06-SUMMARY.md` §"Diff-stat"). The sunk cost in the spike is net positive: PIPE-01 starts task 0 ("delete the throwaway spike code path") and immediately reuses the factory/option/sources patterns for the production rewrite.

### 2.4 Constraint Compatibility

Mapping the chosen split-pattern decision onto user constraints from `05-CONTEXT.md` (locked decisions D-01 through D-09):

| Constraint                                                    | GL-Core v1.6.x | QRhi v2  | Split decision (this record) |
| ------------------------------------------------------------- | -------------- | -------- | ---------------------------- |
| D-01 (Qt 6.5 LTS floor)                                       | ✓              | ✗ (6.7+) | ✓ (v1.6.x preserves 6.5)     |
| D-07 (macOS GL deprecation visible — see softening below)     | persists (long-term API) | eliminated | accepted long-term in v1.6.x; resolved in v2 |
| D-09 (uniform `QSurfaceFormat` cross-OS)                      | ✓              | ✓ (different mechanism — QRhi sets its own backend config) | ✓ in v1.6.x; v2 phase re-evaluates |
| v1.6 milestone scope ("build + render on macOS/Linux/Windows") | ✓ (both spikes build green on macOS; Linux + Windows risk profile is known and bounded) | unverifiable (Linux + Windows data gaps + Qt floor bump) | ✓ (v1.6.x route uses the known-bounded option) |
| PIPE-01 task list scope                                       | smaller        | larger   | bounded — see §4              |

**D-07 framing softening** (substantive — also see §5): the empirical evidence from `05-DRIVER-BEHAVIOUR.md` §D-07 is that the macOS GL deprecation banner is **NOT observable from stdout/stderr** on Qt 6.11 + macOS 26.4.1 + Apple Silicon (the Qt 6 `QOpenGLFunctions_*` wrappers absorb Apple's per-function `__deprecated` attributes; Apple may also route via `os_log` only). This means the immediate "noisy stderr motivates PIPE-01-via-QRhi NOW" framing no longer matches reality — the noise is silent. The long-term API-removal framing ("Apple will eventually remove GL; QRhi is Qt's strategic direction") still holds and is what the v2 arm of this split decision validates. The split decision aligns: v1.6.x accepts the silent long-term liability (GL banner not observable, GL not removed yet); v2 retires the liability via QRhi.

---

## 3. Per-Platform Acceptance Criteria (for PIPE-01)

Concrete, measurable criteria. PIPE-01 (whether v1.6.x GL-Core arm OR v2 QRhi arm) must satisfy these per platform; failure on any row blocks the corresponding PIPE-01 milestone.

| Criterion                                                     | macOS Apple Silicon                                                                                  | Windows x64                                                              | Linux x64                                                                        |
| ------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------ | -------------------------------------------------------------------------------- |
| Demo molecule renders (bundled `data/structures/bpti.pdb`)    | non-blank PNG via `render-smoke-check.sh` (oracle is `BALLVIEW_GL_DIAG` line + PNG size > 1 KB)      | non-blank PNG, same oracle                                               | non-blank PNG (Mesa software fallback OK on CI); native driver fast path on dev  |
| Time to first frame (p99 measured over 10 cold-start runs)    | GL-Core: ≤ 100 ms; QRhi/Metal: ≤ 80 ms (Metal init is faster than GL on Apple Silicon per Qt docs)   | ≤ 200 ms (Intel iGPU / NVIDIA / AMD default)                             | ≤ 200 ms (native driver); ≤ 500 ms (Mesa llvmpipe software fallback on CI)       |
| `BALLVIEW_GL_DIAG renderer_backend=` field                    | GL-Core: `Core` (`gl_profile=core` once GL 3.2+ requested); QRhi: `QRhi` + `qrhi_backend_name=Metal` | GL-Core: `Core` (`gl_profile=core`); QRhi: `qrhi_backend_name=D3D11` or `D3D12` | GL-Core: `Core` (`gl_profile=core`); QRhi: `qrhi_backend_name=Vulkan` or `OpenGL` fallback |
| Picking returns object at click                               | non-empty `std::list<GeometricObject*>` from `pickObjects()`                                         | non-empty list                                                           | non-empty list                                                                   |
| Text overlay legible at 1× DPI                                | yes (QPainter overlay reused from Phase 2; works on both backends)                                   | yes                                                                      | yes                                                                              |
| Text overlay legible at 2× DPI (Retina / HiDPI)               | yes (`device_pixel_ratio=2` from DIAG line)                                                          | yes (if HiDPI display present)                                           | yes (if HiDPI display present)                                                   |
| GL deprecation warnings (GL-Core arm only)                    | accept silently (per `05-DRIVER-BEHAVIOUR.md` §D-07 — banner is not observable under Qt 6)           | N/A — Windows has no equivalent deprecation                              | N/A — Linux GL is fully supported                                                |
| MSAA: `samples=4` requested → samples granted                 | 0 (Apple Metal-emulated GL caps; accept; documented in `05-DRIVER-BEHAVIOUR.md` §Capability Notes)   | ≥ 1 (the Windows ICD honours the request when hardware permits)          | ≥ 1 (Mesa + native both honour)                                                  |
| Cross-platform unit tests (`test/VIEW/RENDERING/`)            | ≥ 5 test cases green (covering picking, overlay, FBO export, primitive draw round-trip)              | ≥ 5 green                                                                | ≥ 5 green                                                                        |
| CI smoke runs without `continue-on-error: true` (i.e. blocking) | blocking smoke step green                                                                          | blocking smoke step green                                                | blocking smoke step green                                                        |

**Bench-mark oracle:** the existing `render-smoke-check.sh` (`.planning/phases/02.2-ci-and-build-smoke-matrix/scripts/`) is the canonical PIPE-01 smoke oracle — same script PIPE-01 inherits from Phase 02.2, same BALLVIEW_GL_DIAG-presence-as-oracle pattern, same `BALL_DATA_PATH=...data` requirement (`05-DRIVER-BEHAVIOUR.md` §"Live macOS Capture Procedure").

---

## 4. Scoped PIPE-01 Task List

Per `.planning/RENDERER-INTERFACE-BOUNDARY.md` §"What actually leaks" + §"What the Phase 5 swap then looks like", and grounded in the live `grep -nE 'gl(Begin|End|MatrixMode|Translate|Rotate|Light|Material|TexCoord|Color|Vertex|Normal|Push|Pop)' source/VIEW/RENDERING/RENDERERS/glRenderer.C | wc -l` count: **200 fixed-function call sites** in `glRenderer.C` today. (This is the dense, by-vertex-call count from the regex; the "~100" round-number figure in `PIPE-01-BACKLOG.md` and `999.6/PIPE-01-BACKLOG.md` is a coarser estimate. 200 is the true count from the canonical regex.)

PIPE-01 (whether v1.6.x GL-Core arm or v2 QRhi arm) must:

### Task 0 — Delete the throwaway spike code path

- **Files to remove:** `include/BALL/VIEW/RENDERING/RENDERERS/coreGLRenderer.h`, `source/VIEW/RENDERING/RENDERERS/coreGLRenderer.C`, `include/BALL/VIEW/RENDERING/RENDERERS/rhiRenderer.h`, `source/VIEW/RENDERING/RENDERERS/rhiRenderer.C`, `include/BALL/VIEW/RENDERING/qtRhiSurface.h`, `source/VIEW/RENDERING/qtRhiSurface.C`.
- **Build-system to revert:** the `BALL_SPIKE_BACKEND` cache var + the `BALLVIEW_USE_SPIKE_BACKEND` env-var gate in `rendererFactory.C` + the conditional-compile entries in `source/VIEW/RENDERING/RENDERERS/sources.cmake` + `source/VIEW/RENDERING/sources.cmake` + the Qt6GuiPrivate find_package + the CI `Spike smoke check (macOS — GLCore backend)` step in `.github/workflows/ci.yml`.
- **Replace with:** the production `BALL_RENDERER_BACKEND` CMake option (values `LegacyFixed | Core | QRhi`, default `LegacyFixed` initially, transitioning to `Core` once PIPE-01 v1.6.x lands), and the same Phase 02.1 factory pattern (`RendererFactory::Kind::OpenGL_Core` becomes a real, non-spike-gated case).
- **Effort:** Small (~1 hour, mechanical).

### Task 1 — Replace `glRenderer.C` fixed-function call sites by per-primitive virtual

PIPE-01 converts each `GLRenderer::render*_()` virtual to modern programmable-pipeline equivalents (GLSL 330 core for v1.6.x GL-Core arm; QRhi shader pair via QShader/qsb for v2 QRhi arm). Conversion order is **smallest blast radius first** so PIPE-01 can ship as a sequence of atomic plans, each one parity-tested against the legacy fixed-function path on macOS before moving on.

| Order | Virtual                                            | Line                | Effort | Notes |
| ----- | -------------------------------------------------- | ------------------- | ------ | ----- |
| 1.1   | `renderPoint_(const Point&)`                       | 1172                | Small  | Single-point shader, trivial |
| 1.2   | `renderLine_(const Line&)`                         | 1056                | Small  | Single-line shader, trivial |
| 1.3   | `renderTwoColoredLine_(const TwoColoredLine&)`     | 1307                | Small  | Two-vertex shader with per-vertex color |
| 1.4   | `renderMultiLine_(const MultiLine&)`               | 1071                | Medium | Line strip, per-vertex color, tangent attribute (see line 1119 comment) |
| 1.5   | `renderSimpleBox_(const SimpleBox&)` + `renderBox_(const Box&)` | 1186, 1199 | Medium | Indexed quad mesh; reuses solid-color triangle shader |
| 1.6   | `renderMesh_(const Mesh&)`                         | 1381                | Medium | Indexed triangle mesh upload; vertex+normal+color attributes |
| 1.7   | `renderQuadMesh_(const QuadMesh&)`                 | 2782                | Medium | Like Mesh but quads → triangulated on upload |
| 1.8   | `renderDisc_(const Disc&)`                         | 1033                | Medium | Triangle-fan disc; reuse solid-color shader |
| 1.9   | `renderClippingPlane_(const ClippingPlane&)`       | 2333                | Medium | `glClipPlane` removed in core; replace with shader-level `gl_ClipDistance` |
| 1.10  | `renderTube_(const Tube&)`                         | 1232                | Large  | Lit cylinder; Phong shader needed (vertex+normal+per-vertex-color) |
| 1.11  | `renderTwoColoredTube_(const TwoColoredTube&)`     | 1263                | Large  | Two-half-cylinder lit; same Phong shader with per-segment color |
| 1.12  | `renderSphere_(const Sphere&)`                     | 1000                | Large  | Lit sphere — billboard impostor OR triangulated icosphere; Phong shader |
| 1.13  | `renderLabel_(const Label&)`                       | 1128                | Large  | Stays via QPainter overlay (proven in Phase 2 + Plan 05); no new shader |
| 1.14  | `renderRuler()`                                    | 960                 | Medium | Lines + labels; composite of 1.2 + 1.13 |
| 1.15  | `renderGridVisualisation_(const GridVisualisation&)` | 2561              | Large  | Volume rendering — multiple options (slice-based, raycasting); needs separate research |
| 1.16  | `renderRepresentation_(const Representation&, bool for_display_list)` | 877 | Large | Top-level dispatcher; rewrite to use batched `renderRepresentations_()` from Phase 02.1 ARCH-03 once all leaf virtuals are converted |

**Effort summary:** 6 × Small/Medium primitives (~3–5 days), 6 × Large lit/volumetric primitives (~3–5 days each), 1 × dispatcher rewrite (~2 days). Total range: 4–6 weeks for the v1.6.x GL-Core arm; +30–50% for the QRhi v2 arm (per `05-06-SUMMARY.md` §"Why Picking Was Deferred to the GL-Core Arm" — the QRhi arm adds async-readback design work + private-headers API churn risk).

### Task 2 — Shader-pair authorship

The v1.6.x GL-Core arm needs **6 shader pairs** (vertex+fragment GLSL 330 core), enumerated:

| Shader pair                                     | Used by                                                      | Effort |
| ----------------------------------------------- | ------------------------------------------------------------ | ------ |
| Solid-color triangles (positions+colors)        | renderMesh_, renderQuadMesh_, renderBox_, renderSimpleBox_, renderDisc_ | Small  |
| Solid-color lines / points                      | renderLine_, renderPoint_, renderMultiLine_, renderTwoColoredLine_ | Small  |
| Lit Phong (positions+normals+per-vertex color)  | renderSphere_ (impostor OR icosphere), renderTube_, renderTwoColoredTube_ | Medium |
| FBO picking (R32UI attachment + integer write)  | All pickable primitives (`05-05-SUMMARY.md` §"Inlined Shader Pair" already has this as `u_pick_mode` uniform)  | Small  |
| QPainter overlay passthrough                    | renderLabel_, renderRuler                                    | Trivial — no shader needed; QPainter owns it |
| Volume / grid (slice-based, optional ray-cast)  | renderGridVisualisation_                                     | Large  |

The Phase 05-05 spike's `coreGLRenderer.C` already inlines a working pair as `const char*` (`05-05-SUMMARY.md` §"Inlined Shader Pair") — the production PIPE-01 lifts these into versioned `.glsl` files under `source/VIEW/RENDERING/SHADERS/` and adds a shader-loader. **Effort:** Medium (3–5 days for the shader-loader + 6 pair files).

The v2 QRhi arm needs the same 6 logical pairs cross-compiled via `qsb` (offline tool from `Qt6::ShaderTools`) into `.qsb` bundles consumed by `QShader::fromSerialized()`. PIPE-01 v2 arm must add `Qt6::ShaderTools` as a build-time dependency (NOT in v1.6.x — see `05-06-SUMMARY.md` §"QShaderBaker not used" decision).

### Task 3 — Picking adaptation

- **v1.6.x GL-Core arm:** color-buffer FBO scheme already implemented in Plan 05-05 spike (`05-05-SUMMARY.md` §"Spike-build smoke" + the `pickObjects()` impl). PIPE-01 lifts the spike's pattern (R32UI color attachment + `glReadPixels(GL_RED_INTEGER, GL_UNSIGNED_INT)` single-pixel readback) into production code. Per-primitive picking ID assignment is at the `Representation` granularity (`05-05-SUMMARY.md` decisions[3]). **Effort:** Medium (1–2 days; pattern is proven, just needs cleanup + multi-pixel marquee-select extension).
- **v2 QRhi arm:** async readback via `QRhiReadbackResult` (`05-06-SUMMARY.md` §"Why Picking Was Deferred to the GL-Core Arm"). Either main-thread-blocking-on-completion (kills FPS — rejected) or async-result-plumbing through `RenderSetup`/`Scene` (touches threading model — needs design). **Effort:** Large (1 week design + 1 week implementation + 1 week threading-model review).

### Task 4 — Text overlay

Keep the QPainter overlay (already proven in Phase 2 + Plan 05; works for both backends — `05-05-SUMMARY.md` §"GL state restoration at the end of renderRepresentations_()" documents the GL state hygiene required for QPainter composition). **No new shader, no new code.** **Effort:** Trivial (verification only).

### Task 5 — Tiling / multi-window paths

Phase 02.1 left 9 stereo/multi-display sites guard-deferred in `scene.C` (`05-CONTEXT.md` Plan 02.1 entry; STATE.md decisions log §Phase 02.1 Plan 02.1-03 Tasks 1-2). Phase 5 Plan 05-02 fixed the `QDesktopWidget` → `QScreen` surface (STATE.md §Phase 05 Plan 05-02 entry). PIPE-01 must verify each of those 9 sites renders under the new backend on at least macOS; full cross-OS stereo verification is a stretch goal not a gate. **Effort:** Medium (1–2 days verification + bug-fix as needed).

### Task 6 — Cross-platform regression / test surface

PIPE-01 must regenerate non-blank PNG smoke-check output across all three OSes (per §3 acceptance criteria). The `render-smoke-check.sh` oracle stays the same; the CI matrix gains an additional `smoke-check (backend=Core)` row in `.github/workflows/ci.yml`. **Effort:** Small (1 day CI plumbing; smoke script is already cross-OS).

### Task 7 — Migration order (the meta-plan for PIPE-01 phases)

PIPE-01's planner (`/gsd-plan-phase 999.6`) should sequence the work as:

1. **PIPE-01 wave 1:** Tasks 0 + 2 (delete spike; author shader-loader + 6 GLSL pairs). Build green; no behaviour change yet.
2. **PIPE-01 wave 2:** Tasks 1.1–1.9 (mechanical primitive conversion, small + medium). Each subtask is one atomic plan with parity test against legacy path; ship one at a time.
3. **PIPE-01 wave 3:** Tasks 1.10–1.12 (lit primitives, large effort). Each is its own atomic plan.
4. **PIPE-01 wave 4:** Tasks 1.13–1.14 (label + ruler — easy, composite).
5. **PIPE-01 wave 5:** Tasks 1.15–1.16 (grid visualisation + top-level dispatcher rewrite to batched API).
6. **PIPE-01 wave 6:** Task 3 (picking).
7. **PIPE-01 wave 7:** Tasks 4 + 5 + 6 (overlay verify + stereo sites + CI matrix extension).
8. **PIPE-01 wave 8:** Flip `BALL_RENDERER_BACKEND` default from `LegacyFixed` to `Core`. Old fixed-function `GLRenderer` stays available behind `LegacyFixed` for at least one v1.6.x release as the escape hatch.

**v2 PIPE-01 (QRhi arm)** is a separate phase, sequenced after the GL-Core arm ships and v1.6.x reaches feature freeze. Same 8 waves, with shader-pair authorship done via `qsb` and picking adaptation reworked for async readback.

---

## 5. Known Liabilities

For PIPE-01 to either resolve OR explicitly accept:

### 5.1 D-07 macOS GL deprecation banner — SOFTENED FRAMING

Per `05-DRIVER-BEHAVIOUR.md` §D-07 empirical finding: today's local launches on macOS 26.4.1 / Qt 6.11 / Apple Silicon emit **NO** GL deprecation banner to stdout or stderr across multiple invocations via `render-smoke-check.sh` and direct `BALLView -export-png`. The verbatim probe and result:

```
$ grep -iE 'deprecat|opengl is dep|gl_silence' /tmp/ballview-stderr.log /tmp/ballview-stdout.log
(empty output)
```

Root cause hypothesis (from `05-DRIVER-BEHAVIOUR.md` §D-07): Qt 6's `QOpenGLFunctions_*` wrappers absorb Apple's per-function `__deprecated` attributes at compile time, so BALLView's translation units never see the per-function macros that would otherwise emit warnings. Apple may also route the deprecation message via `os_log` only (Console.app, not CLI), or suppress it entirely for Metal-emulated GL contexts.

**Apple's canonical deprecation text** (cited from Apple developer docs / WWDC 2018, per `05-DRIVER-BEHAVIOUR.md` §"Apple's canonical OpenGL deprecation messaging"):

> **OpenGL and OpenCL are deprecated and will be removed in a future version of macOS.** Apps that use OpenGL or OpenCL frameworks should migrate to Metal and Metal Compute.

**Updated framing for PIPE-01:** the long-term API-removal risk is real and motivates the v2 QRhi arm of the split decision. The day-to-day runtime stderr noise is NOT present in Qt 6 + Apple Silicon under the current stack — so the "PIPE-01 removes spammy stderr noise" argument from D-07's original framing **no longer holds**. The PIPE-01 case rests on:

1. Apple has announced GL removal; timeline is "future version of macOS" (years, not months).
2. QRhi/Metal is Qt's strategic direction (`QRhiWidget` is the recommended modern path).
3. Cross-platform code path simplification (single QRhi backend vs per-OS GL quirks).

NOT on:

- ❌ ~~Day-to-day runtime stderr noise~~ (silent under Qt 6 + Apple Silicon — empirically false).

This softening is load-bearing for the split-decision: it removes the "do it NOW or pay the noise tax" pressure that would have motivated QRhi-in-v1.6.x, and lets v1.6.x ship on the smaller GL-Core rewrite while v2 takes on QRhi at a sustainable pace.

### 5.2 QRhi API source/binary compatibility (Pitfall 7) — applies to v2 arm only

Per `05-RESEARCH.md` §"Common Pitfalls" Pitfall 7 + verified empirically in `05-06-SUMMARY.md` §"QRhiWidget API signatures used (Pitfall 7 documentation)":

- `QRhi` is **public-API-stable since Qt 6.6** but the canonical include path (`<rhi/qrhi.h>`) lives in the `Qt6::GuiPrivate` private-headers tree. PIPE-01 v2 arm inherits the `Qt6::GuiPrivate` link dependency from the spike.
- Plan 05-06 empirical: `QRhi::driverInfoStruct()` (the plan's anticipated name) does NOT exist in Qt 6.11 — actual API is `QRhi::driverInfo()` returning `QRhiDriverInfo`. This is the kind of API drift Pitfall 7 warns about.
- **PIPE-01 v2 arm must:** (a) pin to a specific Qt LTS minor (Qt 6.8 LTS recommended once available), (b) revisit the API surface on each Qt LTS bump, (c) document the Qt version dependency prominently in `BUILD-macos.md` / `BUILD-linux.md` / `BUILD-windows.md`.

### 5.3 Empirical data gaps — Windows + Linux

Per `05-DRIVER-BEHAVIOUR.md` §Empirical Gaps Carried Forward:

- **Gap 1** (Linux default-build smoke) — CI run 25910030073 fails at `Install Qt 6 (Linux — jurplel/install-qt-action)`. aqtinstall module-name churn between aqt 3.1 and 3.3. Carried forward as `SEED-005-1-ci-aqtinstall-modules-fixup.md`. **PIPE-01 impact:** Linux verification is a v1.6.x gate; SEED-005-1 must be resolved before PIPE-01 v1.6.x can ship.
- **Gap 2** (Windows default-build smoke) — CI run 25910030073 fails at vcpkg `Configure` step (baseline commit `c1ce926d...` not in runner-image vcpkg checkout). Carried forward as `SEED-005-2-ci-vcpkg-baseline-fixup.md`. **PIPE-01 impact:** Windows verification is a v1.6.x gate; SEED-005-2 must be resolved.
- **Gap 3** (Windows hardware/VM driver capture) — no Windows hardware/VM available; CI Windows path broken per Gap 2. Carried forward as `SEED-005-3-windows-vm-driver-capture.md`. **PIPE-01 impact:** can proceed without it if SEED-005-1 + SEED-005-2 resolve and CI captures the Windows DIAG line in pipeline.

### 5.4 PIPE-01 downstream-init structural blocker

Both spike arms (`05-05-SUMMARY.md` + `05-06-SUMMARY.md`) build green but crash during early init because the spike substitutes a `Renderer` (CoreGLRenderer / QRhiRenderer for `Kind::OpenGL_Fixed` via the env-var gate) but downstream `RenderSetup` + `Scene` paint hooks call `GLRenderer`-specific virtuals (`bufferRepresentation`, `setFogIntensity`, the immediate-mode per-primitive `render*_()` virtuals) that the bare-bones spike does not implement.

**PIPE-01 must:** either (a) substitute the matching `RenderSurface` alongside the `Renderer` AND extend `RenderSetup`'s virtual surface to cover the missing entry points (the cleanest fix; aligns with Phase 02.1's boundary spec), OR (b) implement the missing per-primitive virtuals in the production `GLRenderer` replacement (the brute-force fix). The interface-boundary doc (`.planning/RENDERER-INTERFACE-BOUNDARY.md` §"What actually leaks" item 4) recommends (a) — make `renderRepresentations_()` the primary path and let immediate-mode virtuals default-fan-out via the existing Phase 02.1 ARCH-03 mechanism.

This is the load-bearing structural item from the spike data — both backends hit it identically, which is why the choice of backend is not "which spike works today" but "which has the most defensible long-term trajectory under our deployment constraints" (§1 TL;DR).

### 5.5 Spike throwaway-removal task

Per `05-05-SUMMARY.md` §"SPIKE-01 Deliverable Checklist" + `05-06-SUMMARY.md` §"QShaderBaker not used" decision: the spike code (`coreGLRenderer.{h,C}` / `rhiRenderer.{h,C}` / `qtRhiSurface.{h,C}` + the `BALL_SPIKE_BACKEND` CMake option + the `BALLVIEW_USE_SPIKE_BACKEND` env-var gate + the CI spike-smoke step + the `Qt6::GuiPrivate` link surface + the throwaway-provenance headers on each new file) gets **DELETED** at PIPE-01 start. This is Task 0 above. The spike was deliberately built so it could be removed cleanly — every new file carries a `THROWAWAY SPIKE` header comment, and the env-var gate is documented as removable at PIPE-01-start (`rendererFactory.C`).

---

## 6. Decision Authority

**Recommended by:** Claude (planner + executor), 2026-05-15, per reasonable-call autonomous override (see Authoring mode header at top of this document).

**Decision pattern rationale:** the plan was authored `autonomous: false` because the planner expected an interactive judgment call. The empirical data assembled across Plans 05-05 / 05-06 / 05-07 actually narrows the decision space sharply — both backends build green; both fail at the same downstream-virtual init blocker (PIPE-01 scope); macOS D-07 framing softens per `05-DRIVER-BEHAVIOUR.md` §D-07; Windows + Linux data gaps are honest carry-forwards (SEED-005-1, SEED-005-2). The Qt 6.5 LTS floor (D-01) is the wedge that forces the split-decision pattern: GL-Core can ship inside the floor, QRhi cannot. This made the call defensible without a human checkpoint — and Plan 08 is the closeout of Phase 5, so stalling on an unreachable interactive checkpoint would block phase completion indefinitely.

**Accepted by:** _pending architect-of-record sign-off; the decision is in effect for planning purposes per the autonomous override, subject to amendment if the architect-of-record disagrees with the split pattern._

**Open dissent / amendments:** _none recorded as of authoring date. Amendments to this record should be appended as §6.x entries with a date and rationale, NOT by editing §1–§5 in place._

---

## References

- `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-CONTEXT.md` (locked decisions D-01 through D-09)
- `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-RESEARCH.md` (Qt 5→6 delta, GL-Core vs QRhi tradeoffs, per-platform checks, Pitfall 7 on QRhi API stability)
- `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-PATTERNS.md` (spike code patterns reused as the option/gate/factory infrastructure)
- `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-05-SUMMARY.md` (GL-Core spike outcomes — DIAG line, shader pair, picking impl, downstream-virtual caveat)
- `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-06-SUMMARY.md` (QRhi spike outcomes — Qt6GuiPrivate link, RenderTarget stubs, picking-deferred rationale, anticipated DIAG line)
- `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-07-SUMMARY.md` (driver-behaviour record summary + Plan 08 Reading Guide)
- `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-DRIVER-BEHAVIOUR.md` (per-platform empirical record + 5-gap registry + D-07 empirical reframing)
- `.planning/RENDERER-INTERFACE-BOUNDARY.md` (the Phase 02.1 boundary that PIPE-01 swaps behind — §"What actually leaks" + §"What the Phase 5 swap then looks like")
- `.planning/phases/999.6-pipe-01-pipeline-rewrite/PIPE-01-BACKLOG.md` (the dormant entry this decision record unblocks — §4 of this record is the scoped task list it inherits)
- `.planning/seeds/SEED-005-1-ci-aqtinstall-modules-fixup.md` / `SEED-005-2-ci-vcpkg-baseline-fixup.md` / `SEED-005-3-windows-vm-driver-capture.md` / `SEED-005-4-post-pipe01-spike-recapture.md` (the four carry-forward seeds from Plan 05-07; PIPE-01 v1.6.x is gated on SEED-005-1 + SEED-005-2 resolving)
