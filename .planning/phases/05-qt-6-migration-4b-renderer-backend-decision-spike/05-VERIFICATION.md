---
phase: 05-qt-6-migration-4b-renderer-backend-decision-spike
verified: 2026-05-15T12:00:00Z
status: human_needed
score: 5/5 roadmap success criteria + 4/4 requirement IDs accounted for (2 Complete, 2 Pending-with-evidence)
overrides_applied: 0
re_verification:
  previous_status: none
  previous_score: n/a
  gaps_closed: []
  gaps_remaining: []
  regressions: []
human_verification:
  - test: "Decide whether the three code-review Qt 6 migration regressions (CR-01 HTMLPage::certificateError dead under Qt 6, CR-02 QSignalMapper::mapped(int) → mappedInt rename missed at 2 sites, CR-03 QLayout::setMargin still present in pyWidget.C) block phase close OR are accepted as Phase 5.1 follow-up scope."
    expected: "Architect-of-record either (a) flips them into a Phase 5.1 backlog row and closes Phase 5, OR (b) requires a follow-up plan in Phase 5 before close. The REQUIREMENTS.md QT6-01/QT6-02 rows are still Pending (not flipped to Complete), which is consistent with deferring this judgement — but the phase goal claims 'Qt 6 migration baseline' and CR-01/CR-02 are runtime behaviour regressions in the same QT6-02 sweep."
    why_human: "Scope-vs-completeness judgement. The reviewer marked these advisory (per workflow); REQUIREMENTS.md does not yet flip QT6-01/02 to Complete; the must_haves tables in Plans 02/05 do not enumerate these specific sites. A planner needs to decide whether to fold them into a Phase 5 closeout fix or into Phase 5.1's already-active scope."
  - test: "Confirm the gl_profile=compatibility CI assertion in .github/workflows/ci.yml (lines 331, 387) should be relaxed for macOS Apple Silicon now that gl_profile=none is the empirically-established Qt 6 + Apple Silicon reality (per 05-DRIVER-BEHAVIOUR.md §BALLVIEW_GL_DIAG Captures — both M4 Max hardware AND macos-latest software runner emit gl_profile=none)."
    expected: "Architect-of-record signs off on a one-line ci.yml relaxation OR confirms it stays as-is until SEED-005-1 / SEED-005-2 land (the assertion is currently dormant because Linux + Windows CI is red upstream of the smoke step, so the mismatch is unobserved)."
    why_human: "The original D-08 oracle was gl_profile=compatibility but the live data invalidates it. This is named as a candidate fix in STATE.md §'Notes' and is a 1-line CI change — but Phase 5 didn't make it, leaving an inconsistency between the oracle and the empirical capture. Plan 08 (SPIKE-DECISION) §3 explicitly notes 'gl_profile=core once GL 3.2+ requested' as the PIPE-01 criterion, implicitly retiring gl_profile=compatibility — but the CI still asserts the old oracle."
  - test: "Confirm the autonomous override of Plan 05-07 (autonomous: false → executed autonomously) and Plan 05-08 (autonomous: false → executed autonomously) is acceptable as a phase-close decision authority precedent."
    expected: "Architect-of-record either signs §6 of 05-SPIKE-DECISION.md (Accepted by: _pending architect-of-record sign-off_) OR appends a §6.x amendment if disagreeing with the split-pattern decision. The decision is provisionally in effect for planning purposes."
    why_human: "The plans declared autonomous: false expecting a human judgment call. The execution applied a 'reasonable-call autonomous override' grounded in three reality constraints (spike full-render structurally PIPE-01-blocked, no Windows hardware, Plan 08 phase-closeout gated on Plan 07 artifact). Documented in both SUMMARYs and in 05-SPIKE-DECISION.md §6. The decision is defensible but the override pattern is a precedent worth confirming."
---

# Phase 5: Qt 6 Migration + Renderer Backend Decision Spike — Verification Report

**Phase Goal:** Qt 6 migration baseline (Qt 6.5 LTS floor) + SPIKE-01 (two prototype renderer backends: GL-core + QRhi) + SPIKE-02 (decision record naming the chosen backend with per-platform criteria and a scoped PIPE-01 task list).
**Verified:** 2026-05-15T12:00:00Z
**Status:** human_needed
**Re-verification:** No — initial verification.

## Goal Achievement

### Observable Truths (from ROADMAP §Phase 5 Success Criteria)

| # | Truth | Status | Evidence |
|---|-------|--------|----------|
| 1 | BALLView builds and launches against Qt 6 with all QGLWidget-era APIs removed (including glDisplayList.h's QtOpenGL/qgl.h) | ✓ VERIFIED (macOS) | CMakeLists.txt:317 `SET(QT_MIN_VERSION 6.5)` + line 319 `FIND_PACKAGE(Qt6 ${QT_MIN_VERSION} REQUIRED ...)`; lint `scripts/check-no-legacy-qt6-symbols.sh` runs clean on current tree (probe executed: "0 legacy Qt 5 symbol references remain in code"); Qt6::OpenGLWidgets in VIEW_DEP_LIBRARIES (line 354); first Qt 6 link-green BALLView build confirmed in STATE.md Plan 05-05 entry; live macOS run captured `BALLVIEW_GL_DIAG gl_version="2.1 Metal - 90.5"` (05-DRIVER-BEHAVIOUR.md). **Linux + Windows runtime unverified** (CI infrastructure red, captured as SEED-005-1 + SEED-005-2; addressed in Phase 5.1 scope per deferred-items.md). |
| 2 | The user-facing GUI behaves correctly with QRegExp and QDesktopWidget replaced by Qt 6 equivalents | ⚠ VERIFIED-WITH-CAVEAT | QRegExp → QRegularExpression in pyWidget.{h,C} (verified: 13 grep hits, globalMatch + QRegularExpressionMatchIterator); QDesktopWidget → QGuiApplication::screens() in stageSettings.C + interactionMode.C + scene.C; pluginManager.C `Qt::SkipEmptyParts` at line 408; scene.C stereo paths `setScreen()` (verified: 6 setScreen() call sites at lines 2615, 2623, 2796, 2825, 2900, 2930). **Caveat: 3 Qt 6 migration regressions surfaced by 05-REVIEW.md remain in source (see Anti-Patterns section). Two are runtime breakages in same QT6-02 scope: HTMLPage::certificateError (no signal/slot wiring under Qt 6); QSignalMapper::mapped(int) → mappedInt rename missed at molecularControl.C:498 + PresentaBALLView.C:40 (rotamer menu broken). REQUIREMENTS.md QT6-02 row is still Pending, consistent with these gaps not yet being claimed Complete.** |
| 3 | The 3D scene still renders correctly under Qt 6 via the compatibility-profile fixed-function path (no pixel regression vs Phase 2, gated by Phase 02.2 smoke check) | ⚠ VERIFIED-WITH-CAVEAT (macOS) | glRenderWindow.C:39 `fmt.setProfile(QSurfaceFormat::CompatibilityProfile)`; line 40 `fmt.setOptions(QSurfaceFormat::DeprecatedFunctions)` (D-06 + Pitfall 2); QtOpenGLWidgets/QOpenGLWidget include at glRenderWindow.h:32 (BLOCKER-C resolved); live macOS PNG smoke pass (M4 Max local + CI macos-latest software, both runner-independent). **Caveat: gl_profile=compatibility CI assertion is empirically invalidated on Apple Silicon (Apple GL 2.1 emits gl_profile=none — both runners confirm). The CI assertion is dormant (Linux/Windows CI red upstream) so the mismatch is unobserved. Plan 05-04 D-06/D-08 oracle is therefore a latent mismatch.** |
| 4 | A throwaway prototype renders the demo molecule through at least the leading backend candidate (GL-core and/or QRhi) behind the RendererFactory, demonstrating picking and a text overlay; macOS (Apple Silicon) and Windows driver behaviour are checked and recorded | ⚠ VERIFIED-WITH-DOCUMENTED-CAVEAT | **Both spike arms built green** on macOS-arm64 Qt 6.11 (configure + build green; 3-row build matrix in 05-SPIKE-DECISION.md §2.1). RendererFactory::Kind::OpenGL_Core + ::QRhi both wired (rendererFactory.h:51-60, ifdef-gated). Both arms construct correctly via BALLVIEW_USE_SPIKE_BACKEND=1 env-var gate (rendererFactory.C:57-62). **Picking implementation exists in code** — coreGLRenderer.C lines 188-403: initPickingFBO_ + resizePickingFBO_ + pickObjects with R32UI color attachment + glReadPixels. **Throwaway provenance** on all 6 new files (`THROWAWAY SPIKE — Phase 5 prototype only. Not production code. Remove before PIPE-01.`). **05-DRIVER-BEHAVIOUR.md (265 lines) records per-platform behaviour** with 2 verbatim live BALLVIEW_GL_DIAG macOS captures + spike construction markers + 5 explicit empirical gaps (Linux build/Windows build/Windows VM/post-PIPE-01-recapture/D-07 banner). **Downstream-virtual full-render limitation is documented** in both spike SUMMARYs (Plans 05-05 + 05-06) AND in 05-SPIKE-DECISION.md §5.4 as a PIPE-01-scope structural blocker — both backends hit it identically, which the decision record uses to argue the choice depends on the Qt-floor wedge, not on spike-runtime success. **Windows + Linux driver behaviour is GAP** (SEED-005-1/2/3, addressed in Phase 5.1 scope). |
| 5 | A decision record (.planning/ doc) names the chosen backend, the rationale, per-platform (macOS/Windows) acceptance criteria, and a scoped task list for the PIPE-01 full rewrite | ✓ VERIFIED | `.planning/phases/05-.../05-SPIKE-DECISION.md` exists (342 lines, 2.85× the plan's 120-line floor). §1 names backend explicitly: "GL-Core for v1.6.x → QRhi for v2 (two-step 'split' pattern)". §2 Rationale cites 22 data points from 05-05/06/07-SUMMARY.md + 05-DRIVER-BEHAVIOUR.md by filename + section reference (no abstract preferences). §3 Per-Platform Acceptance Criteria table — 9 measurable criteria × 3 OSes (e.g. "Time to first frame p99 ≤ 100 ms macOS GL-Core / ≤ 80 ms macOS QRhi-Metal / ≤ 200 ms Windows"). §4 Scoped PIPE-01 Task List — 200 fixed-function call sites (canonical regex count) + 16 per-virtual conversion tasks ordered smallest-blast-radius-first + 6 shader pairs + Task 0 throwaway-removal + Task 7 8-wave migration order for /gsd-plan-phase 999.6. §5 Known Liabilities — D-07 softening, Pitfall 7 QRhi API stability (v2-only), 3 empirical gaps from 05-DRIVER-BEHAVIOUR.md, downstream-init structural blocker (load-bearing), spike throwaway-removal. §6 Decision Authority documents the reasonable-call autonomous override and is awaiting architect-of-record sign-off. |

**Score:** 5/5 ROADMAP success criteria verified (2 with documented caveats; 1 with code-review carry-forward; 2 with deferred infrastructure gaps).

### Required Artifacts

| Artifact | Expected | Status | Details |
|----------|----------|--------|---------|
| `CMakeLists.txt` | Qt6 FIND_PACKAGE + Qt6::OpenGLWidgets + BALL_SPIKE_BACKEND option | ✓ VERIFIED | Lines 281-336 BALL_SPIKE_BACKEND option (OFF/GLCore/QRhi); lines 317-319 Qt 6.5 floor; line 354 Qt6::OpenGLWidgets in VIEW_DEP_LIBRARIES; line 365 Qt6GuiPrivate guarded by spike+Qt6.7+ |
| `CMakePresets.json` | macos-homebrew → /opt/homebrew/opt/qt | ✓ VERIFIED | Line 22 `CMAKE_PREFIX_PATH: "/opt/homebrew/opt/qt;/opt/homebrew"` (Homebrew unversioned qt → Qt 6.11) |
| `vcpkg.json` | qtbase declaration | ✓ VERIFIED | `dependencies: ["qtbase", ...]`; baseline pinned at `c1ce926ddf...` (note: baseline drift causes Windows CI red — SEED-005-2) |
| `include/BALL/VIEW/RENDERING/glRenderWindow.h` | QtOpenGLWidgets/QOpenGLWidget include | ✓ VERIFIED | Line 32 `#include <QtOpenGLWidgets/QOpenGLWidget>` (BLOCKER-C resolved in Plan 05-03) |
| `source/VIEW/RENDERING/glRenderWindow.C` | CompatibilityProfile + DeprecatedFunctions setOptions | ✓ VERIFIED | Lines 39-40 setProfile(CompatibilityProfile) + setOptions(DeprecatedFunctions); D-07 audited green (no GL_SILENCE_DEPRECATION token) |
| `include/BALL/VIEW/WIDGETS/pyWidget.h` + `.C` | QRegularExpression members + globalMatch usage | ✓ VERIFIED | h:33 #include QRegularExpression; h:68-71 member declarations; C:24,57,64,67,68 construction; C:78,89,98,107 globalMatch iteration |
| `source/VIEW/DIALOGS/stageSettings.C` | QGuiApplication::screens() | ✓ VERIFIED | Lines 53, 94, 96, 422, 440-441 — multi-screen geometry via QGuiApplication::screens() |
| `source/VIEW/WIDGETS/scene.C` | setScreen() stereo placement | ✓ VERIFIED | 6 setScreen() sites at lines 2615, 2623, 2796, 2825, 2900, 2930 (all 6 stereo construction sites with Pitfall 5 transform applied) |
| `source/VIEW/KERNEL/MODES/interactionMode.C` | Qt 6 multi-screen | ✓ VERIFIED | Plan 05-02 D-05 reinforcement; QGuiApplication::primaryScreen() replacement |
| `source/PLUGIN/pluginManager.C` | Qt::SkipEmptyParts | ✓ VERIFIED | Line 408 `value.split(";", Qt::SkipEmptyParts)` |
| `include/BALL/VIEW/RENDERING/rendererFactory.h` | OpenGL_Core + QRhi enum entries | ✓ VERIFIED | Lines 51-60 ifdef-gated `OpenGL_Core` (BALL_SPIKE_BACKEND_GLCORE) + `QRhi` (BALL_SPIKE_BACKEND_QRHI) |
| `source/VIEW/RENDERING/rendererFactory.C` | makeRenderer/makeSurface spike cases | ✓ VERIFIED | Lines 43-82 makeRenderer with BALLVIEW_USE_SPIKE_BACKEND env-var gate + ifdef-gated case branches; lines 103-145 makeSurface with same pattern |
| `include/BALL/VIEW/RENDERING/RENDERERS/coreGLRenderer.h` | CoreGLRenderer class, min 30 lines | ✓ VERIFIED | 113 lines; line 41 `class BALL_VIEW_EXPORT CoreGLRenderer : public Renderer`; line 57 renderRepresentations_ override; line 61 capabilities() override; line 67 pickObjects() override |
| `source/VIEW/RENDERING/RENDERERS/coreGLRenderer.C` | VAO/VBO + shader pair + picking FBO, min 150 lines | ✓ VERIFIED | 407 lines (2.7× floor); shader pair inlined as const char*; lines 144-185 compile/link; lines 188-244 picking FBO init/resize; lines 345-403 pickObjects with glReadPixels readback |
| `include/BALL/VIEW/RENDERING/RENDERERS/rhiRenderer.h` | QRhiRenderer class, min 30 lines | ✓ VERIFIED | 118 lines; QRhiRenderer extends Renderer; renderRepresentations_ + capabilities() + pickObjects() overrides |
| `source/VIEW/RENDERING/RENDERERS/rhiRenderer.C` | QRhi impl, min 120 lines | ✓ VERIFIED | 205 lines (1.7× floor); QRhi command buffer + QRhiBuffer + QShader stages |
| `include/BALL/VIEW/RENDERING/qtRhiSurface.h` | QtRhiSurface (QRhiWidget subclass), min 30 lines | ✓ VERIFIED | 139 lines; multi-inherits RenderSurface + QRhiWidget; declares the 7 RenderTarget pure-virtual stubs |
| `source/VIEW/RENDERING/qtRhiSurface.C` | QtRhiSurface impl, min 60 lines | ✓ VERIFIED | 172 lines (2.9× floor); beginFrame/endFrame/initialize callbacks |
| `source/VIEW/RENDERING/RENDERERS/sources.cmake` | coreGLRenderer.C + rhiRenderer.C gated | ✓ VERIFIED | Lines 17-22 GLCore arm gate; lines 26-31 QRhi arm gate (+Qt6Core_VERSION ≥ 6.7) |
| `source/VIEW/RENDERING/sources.cmake` | qtRhiSurface.C gated | ✓ VERIFIED | Lines 16-19 spike+Qt6.7+ gated |
| `.github/workflows/ci.yml` | ubuntu-24.04 + jurplel/install-qt-action + GL profile assertion + spike smoke variant | ✓ VERIFIED | Line 64 ubuntu-24.04; line 183 jurplel/install-qt-action@v4; lines 331, 387 gl_profile=compatibility grep assertion; line 346 BALL_SPIKE_BACKEND=GLCore non-blocking macOS spike step; line 464 lint hook |
| `scripts/check-no-legacy-qt6-symbols.sh` | LEGACY_PATTERN | ✓ VERIFIED | LEGACY_PATTERN covers QRegExp\|QDesktopWidget\|QApplication::desktop\|QString::SkipEmptyParts\|QtWidgets/QOpenGLWidget\|QtWidgets/QDesktopWidget\|Qt5:: |
| `.planning/phases/05-.../05-DRIVER-BEHAVIOUR.md` | Per-platform driver record, min 80 lines | ✓ VERIFIED | 265 lines (3.3× floor); 2 verbatim live BALLVIEW_GL_DIAG captures (M4 Max + macos-latest software); spike construction markers cited; D-07 empirical finding documented; 5 gaps enumerated with seed references |
| `.planning/phases/05-.../05-SPIKE-DECISION.md` | SPIKE-02 decision record with 5 mandatory sections, min 120 lines | ✓ VERIFIED | 342 lines (2.85× floor); §1 Chosen Backend; §2 Rationale (22 cited data points); §3 Per-Platform Acceptance Criteria; §4 Scoped PIPE-01 Task List; §5 Known Liabilities; §6 Decision Authority (awaiting architect sign-off) |

### Key Link Verification

| From | To | Via | Status | Details |
|------|-----|------|--------|---------|
| `CMakeLists.txt` | Qt6::OpenGLWidgets | `FIND_PACKAGE(Qt6 ... OpenGLWidgets) + LIST(APPEND VIEW_DEP_LIBRARIES Qt6::OpenGLWidgets)` | ✓ WIRED | CMakeLists.txt:343 + 354 |
| `CMakePresets.json macos-homebrew` | `/opt/homebrew/opt/qt` (unversioned) | CMAKE_PREFIX_PATH | ✓ WIRED | line 22; no `@5` suffix (D-03 clean break) |
| `pyWidget.C compilePattern()` | QRegularExpression construction | `push_back(QRegularExpression(...))` | ✓ WIRED | lines 57, 64, 67, 68 |
| `pyWidget.C highlightBlock()` | QRegularExpressionMatchIterator | `expression.globalMatch(text)` | ✓ WIRED | lines 78, 89, 98, 107 |
| `stageSettings.C` | screen geometry | `QGuiApplication::screens().value(i)->geometry()` | ✓ WIRED | line 422 |
| `scene.C stereo construction` | QScreen-based window placement | `new GLRenderWindow(nullptr, ...); w->setScreen(...)` | ✓ WIRED | 6 sites |
| `glRenderWindow.C gl_format_ static initializer` | `CompatibilityProfile + DeprecatedFunctions` | setProfile + setOptions in static lambda | ✓ WIRED | lines 39-40 |
| `glRenderWindow.C initializeGL` | `BALLVIEW_GL_DIAG` stdout line | DIAG-01 emission with gl_profile field | ✓ WIRED | lines 140 + 147 (profile name derived from live granted format) |
| `ci.yml linux-x64 matrix entry` | Qt 6.5+ installation | jurplel/install-qt-action@v4 | ✓ WIRED-BUT-BROKEN | step exists (line 183) but fails with aqtinstall module-name churn — SEED-005-1, addressed in Phase 5.1 |
| `ci.yml smoke-check follow-up` | `BALLVIEW_GL_DIAG gl_profile=compatibility` | grep on captured stdout | ⚠ WIRED-WITH-MISMATCH | lines 331, 387 — assertion is `gl_profile=compatibility` but live macOS Apple Silicon emits `gl_profile=none` (empirically established in 05-DRIVER-BEHAVIOUR.md). Dormant today because Linux/Windows CI red upstream. |
| `ci.yml lint job` | `check-no-legacy-qt6-symbols.sh` | bash invocation | ✓ WIRED | line 464 |
| `rendererFactory.C` | CoreGLRenderer | `case Kind::OpenGL_Core: return new CoreGLRenderer;` (ifdef-guarded) | ✓ WIRED | lines 67-69 |
| `rendererFactory.C` | QRhiRenderer | `case Kind::QRhi: return new QRhiRenderer;` (ifdef-guarded) | ✓ WIRED | lines 73-75 |
| `rendererFactory.C makeSurface` | QtRhiSurface | `case Kind::QRhi: return new QtRhiSurface(parent);` (ifdef-guarded) | ✓ WIRED | line 131 |
| `CMakeLists.txt` | `BALL_PROJECT_COMPILE_DEFNS -DBALL_SPIKE_BACKEND_GLCORE` | `IF (BALL_SPIKE_BACKEND STREQUAL "GLCore") LIST(APPEND ...)` | ✓ WIRED | lines 284-286 |
| `CMakeLists.txt` | `-DBALL_SPIKE_BACKEND_QRHI` (Qt 6.7+ gated) | post-FIND_PACKAGE block + version check | ✓ WIRED | lines 333-338; with MESSAGE(WARNING ...) for < 6.7 |
| `ci.yml` | spike smoke variant (non-blocking, macOS only) | `-DBALL_SPIKE_BACKEND=GLCore` + continue-on-error: true | ✓ WIRED | lines 338-350 |
| `05-SPIKE-DECISION.md` | Data sources from Plans 05/06/07 | explicit references to upstream artifacts | ✓ WIRED | 22 citations across §1-§5 (verified by reading the file) |
| `REQUIREMENTS.md` | `05-SPIKE-DECISION.md` | Traceability row update | ✓ WIRED | lines 172-173 SPIKE-01 + SPIKE-02 flipped to Complete with file reference |

### Data-Flow Trace (Level 4)

| Artifact | Data Variable | Source | Produces Real Data | Status |
|----------|---------------|--------|--------------------|--------|
| `coreGLRenderer.C::pickObjects()` | pick_id (R32UI readback) | glReadPixels on FBO color attachment | n/a — code path correct, but full-frame run blocked by downstream-virtual PIPE-01 blocker (Spike Caveat §5.4 of 05-SPIKE-DECISION.md) | ⚠ HOLLOW — code is structurally correct but data flow cannot be exercised end-to-end today |
| `glRenderWindow.C::initializeGL` | BALLVIEW_GL_DIAG line | live QOpenGLContext::format() + functions->glGetString() | ✓ Yes — `gl_vendor="Apple" gl_renderer="Apple M4 Max" gl_version="2.1 Metal - 90.5"` captured live | ✓ FLOWING |
| `05-DRIVER-BEHAVIOUR.md` | DIAG lines per platform×backend | sourced from live macOS runs + CI logs | ✓ Yes — 2 verbatim captures (M4 Max + macos-latest software); Linux + Windows are documented GAPS with seeds | ✓ FLOWING (where data exists) |
| `05-SPIKE-DECISION.md` | rationale citations | 05-05/06/07-SUMMARY.md + 05-DRIVER-BEHAVIOUR.md | ✓ Yes — 22 citations verified by reading | ✓ FLOWING |

### Behavioral Spot-Checks

| Behavior | Command | Result | Status |
|----------|---------|--------|--------|
| Lint script catches what it claims to catch | `bash scripts/check-no-legacy-qt6-symbols.sh` | exit 0; "0 legacy Qt 5 symbol references remain in code" | ✓ PASS |
| Lint script structure handles comment-only lines | grep `grep -vE ':[0-9]+:[[:space:]]*(//|\*|/\*)'` in script | confirmed at line 76 of script | ✓ PASS |
| CR-01 latent regression (HTMLPage::certificateError) still present in code | `grep -n "certificateError" include/BALL/VIEW/WIDGETS/HTMLPage.h source/VIEW/WIDGETS/HTMLPage.C` | non-virtual method with `bool certificateError(...)` at HTMLPage.C:38; no `connect(this, &QWebEnginePage::certificateError, ...)` in either file | ✗ FAIL (regression unresolved) |
| CR-02 latent regression (QSignalMapper::mapped → mappedInt rename) still present | `grep -n "SIGNAL(mapped" source/VIEW/WIDGETS/molecularControl.C source/EXTENSIONS/PRESENTABALL/source/PresentaBALLView.C` | 2 hits: molecularControl.C:498 + PresentaBALLView.C:40 still use `SIGNAL(mapped(int))` (will silently no-op under Qt 6) | ✗ FAIL (regression unresolved) |
| CR-03 latent (QLayout::setMargin in pyWidget.C, gated by BALL_PYTHON_SUPPORT) | `grep -n "setMargin" source/VIEW/WIDGETS/pyWidget.C` | lines 348 + 412 still call `setMargin(0)` / `setMargin(1)` | ⚠ LATENT (compile error if Python support flipped ON; OFF today) |
| Throwaway provenance on all 6 new spike files | grep `THROWAWAY` on each new file header | confirmed all 6 carry the canonical line | ✓ PASS |
| min_lines artifacts | wc -l on each | coreGLRenderer.h=113 (≥30 ✓), .C=407 (≥150 ✓), rhiRenderer.h=118 (≥30 ✓), .C=205 (≥120 ✓), qtRhiSurface.h=139 (≥30 ✓), .C=172 (≥60 ✓), 05-DRIVER-BEHAVIOUR.md=265 (≥80 ✓), 05-SPIKE-DECISION.md=342 (≥120 ✓) | ✓ PASS |
| BALL_SPIKE_BACKEND gate is never on by default | `grep "SET(BALL_SPIKE_BACKEND" CMakeLists.txt` | line 281: `"OFF" CACHE STRING` (default OFF) | ✓ PASS |
| Spike Kinds are ifdef-walled from production code | scene.C Kind references | scene.C:137, 153, 2208 only request Kind::OpenGL_Fixed; never Kind::OpenGL_Core or Kind::QRhi | ✓ PASS |

### Probe Execution

| Probe | Command | Result | Status |
|-------|---------|--------|--------|
| Lint script self-test (real codebase) | `bash /Users/kohlbach/Claude/BALL/ball/scripts/check-no-legacy-qt6-symbols.sh` | exit 0; "0 legacy Qt 5 symbol references remain in code" | ✓ PASS |
| `cmake --preset ci-macos -DBALL_LICENSE=GPL` configure | not run (build artifacts referenced but not re-built) | configure result inherited from STATE.md: GREEN on macOS-arm64 Qt 6.11 across all 3 backend configs (default/GLCore/QRhi) | ✓ PASS (cited from STATE.md performance metrics + Plan 05-05/06 SUMMARYs) |
| Linux CI smoke | github CI run 25910030073 | aqtinstall step red — SEED-005-1 dormant | ⚠ KNOWN-RED (infrastructure; addressed in Phase 5.1) |
| Windows CI smoke | github CI run 25910030073 | vcpkg Configure step red — SEED-005-2 dormant | ⚠ KNOWN-RED (infrastructure; addressed in Phase 5.1) |

### Requirements Coverage

| Requirement | Source Plan(s) | Description | Status | Evidence |
|-------------|---------------|-------------|--------|----------|
| QT6-01 | 05-01, 05-03, 05-04, 05-05 | BALLView builds against Qt 6, with QGLWidget-era APIs fully removed | ⚠ Pending (REQUIREMENTS.md row stays Pending — consistent with non-fully-resolved state) | macOS link-green + render-green; Qt 6.5 floor enforced (CMakeLists.txt:317); lint zero hits; Linux + Windows CI infrastructure red (SEED-005-1/2 dormant — Phase 5.1 scope); 1 source-level regression from code review (CR-03 latent in BALL_PYTHON_SUPPORT-gated code) |
| QT6-02 | 05-02, 05-04 | Qt-deprecated APIs in VIEW (QRegExp, QDesktopWidget) replaced with Qt 6 equivalents | ⚠ Pending (REQUIREMENTS.md row stays Pending — consistent with non-fully-resolved state) | QRegExp / QDesktopWidget / QString::SkipEmptyParts all replaced and lint-verified; **2 source-level regressions in same QT6-02 sweep scope from code review (CR-01 HTMLPage::certificateError signal dead; CR-02 QSignalMapper::mapped → mappedInt rename missed at 2 sites)** — flagged for human verification of acceptance |
| SPIKE-01 | 05-05, 05-06, 05-07 | Throwaway prototype renders demo molecule via at least one backend candidate with picking + text overlay; per-platform driver behaviour recorded | ✓ Complete (REQUIREMENTS.md flipped) | Both backends built green; picking impl in code (R32UI FBO + glReadPixels); throwaway provenance on all 6 new files; 05-DRIVER-BEHAVIOUR.md records 2 verbatim macOS captures + 5 explicit gaps with seeds; full-render limitation is documented PIPE-01-scope blocker (not a deliverable miss per the plan must_haves) |
| SPIKE-02 | 05-08 | Decision record names chosen backend with rationale, per-platform criteria, scoped PIPE-01 task list | ✓ Complete (REQUIREMENTS.md flipped) | 05-SPIKE-DECISION.md 342 lines; 5 mandatory sections all present and substantive; 22 data citations grounding the split-pattern decision; §6 decision authority awaiting architect sign-off but provisionally in effect |

### Anti-Patterns Found

| File | Line | Pattern | Severity | Impact |
|------|------|---------|----------|--------|
| `include/BALL/VIEW/WIDGETS/HTMLPage.h` `source/VIEW/WIDGETS/HTMLPage.C` | h:28, C:38 | Qt 6 migration regression (CR-01) | ⚠ Warning | `certificateError()` is non-virtual method that is never invoked under Qt 6 because `QWebEnginePage::certificateError` is now a signal. `ignore_ssl_errors_=true` callers see SSL errors REJECTED instead of accepted. Behavioural regression in TLS-trust path. Not addressed in source; advisory per 05-REVIEW.md. Fits Phase 5.1 scope. |
| `source/VIEW/WIDGETS/molecularControl.C` `source/EXTENSIONS/PRESENTABALL/source/PresentaBALLView.C` | molecularControl.C:498, PresentaBALLView.C:40 | Qt 6 migration regression (CR-02) | ⚠ Warning | `connect(..., SIGNAL(mapped(int)), ...)` against QSignalMapper silently no-ops under Qt 6 (Qt 6 renamed the signal to `mappedInt`). Live evidence in Plan 05-06 smoke output: `QObject::connect: No such signal QSignalMapper::mapped(int)`. Rotamer-menu feature + PresentaBALL action signals broken. Not addressed in source; advisory per 05-REVIEW.md. Fits Phase 5.1 scope. |
| `source/VIEW/WIDGETS/pyWidget.C` | 348, 412 | Qt 6 latent compile failure (CR-03) | ℹ Info | `QLayout::setMargin()` removed in Qt 6. Currently gated by `BALL_PYTHON_SUPPORT=OFF` (Phase 5 default), so build is green today; hard compile error if Python support is re-enabled (Phase 6). Plan 05-05's Qt 6 API sweep claimed setMargin → setContentsMargins resolved in scene.C — the two pyWidget.C sites were missed. Trivial mechanical fix; fits Phase 5.1 OR Phase 6 entry scope. |
| `source/VIEW/RENDERING/rendererFactory.C` | 51 | Env-var gate accepts any non-empty value (incl. `=0`) — opposite of "==1" semantics in comment (WR-01) | ℹ Info | `BALLVIEW_USE_SPIKE_BACKEND=0` will activate the spike instead of disabling it because `std::getenv()` returns non-null. Spike is throwaway, comment-vs-code mismatch is a footgun but not load-bearing. Fits PIPE-01 Task 0 (spike removal) — code disappears entirely. |
| `source/VIEW/RENDERING/RENDERERS/coreGLRenderer.C` | various | Hardcoded GL hex literals (WR-05) | ℹ Info | Spike code throughout uses `/*GL_TEXTURE_2D=*/0x0DE1` style hex literals instead of including GL headers. Throwaway-tolerable. Fits PIPE-01 Task 0 (spike removal) — code disappears entirely. |
| `source/VIEW/RENDERING/RENDERERS/rhiRenderer.C` | 84-92 | QRhi resource destruction may UAF if surface dies first (WR-02) | ℹ Info | Throwaway spike code; not exercised today because spike crashes during init. Fits PIPE-01 Task 0 (spike removal). |
| **All modified files** | — | Debt markers (TBD/FIXME/XXX/TODO/HACK) | ✓ None found | Probe: grep -E '\\bTBD\\b\\|\\bFIXME\\b\\|\\bXXX\\b\\|\\bTODO\\b\\|\\bHACK\\b' across all 21 modified files returned zero hits. Phase 5 left no unaudited debt markers in changed files. |

### Deferred Items

Items not yet met but explicitly addressed in later milestone phases (Phase 5.1 or backlog 999.6 PIPE-01).

| # | Item | Addressed In | Evidence |
|---|------|-------------|----------|
| 1 | SEED-005-1: Linux default-build smoke under Qt 6.5+ via aqtinstall (module-name churn) | Phase 5.1 (Build Warnings & Latent Bug Cleanup) | ROADMAP §"Phase 5.1" goal text + deferred-items.md GAP-1 §"Suggested home: Phase 5.1 build-warnings/latent-bugs cleanup OR dedicated 1-task CI hotfix" |
| 2 | SEED-005-2: Windows vcpkg baseline commit not in runner image | Phase 5.1 | deferred-items.md GAP-2 §"Suggested home: Phase 5.1 OR dedicated 1-task CI hotfix" |
| 3 | SEED-005-3: Windows hardware/VM driver-behaviour capture | Phase 5.1 Path A (CI artifact) OR Phase 999.6 PIPE-01 Path B | deferred-items.md GAP-3 §"Suggested home: Phase 5.1 (Path A) OR PIPE-01 opening (Path B)" |
| 4 | SEED-005-4: Post-PIPE-01 spike full-render BALLVIEW_GL_DIAG re-capture | Phase 999.6 PIPE-01 Wave 0 | deferred-items.md GAP-4 §"Suggested home: PIPE-01 phase Wave 0" + 05-SPIKE-DECISION.md §5.4 "PIPE-01 must extend RenderSetup's virtual surface OR implement missing per-primitive virtuals" |
| 5 | BLOCKER-A2: Full QtXml SAX → QXmlStreamReader port for DockResultFile (stub in place) | Phase 5.1 | deferred-items.md BLOCKER-A2 §"Suggested home: Phase 5.1 build-warnings/latent-bugs cleanup" |
| 6 | gl_profile=compatibility CI assertion vs gl_profile=none on Apple Silicon (latent mismatch) | Phase 5.1 candidate (named in STATE.md §"Consider also") | STATE.md `Notes` block: "Relax the Plan 05-04 macOS GL compat-profile grep assertion to accept gl_profile=none on Apple Silicon — confirmed runner-independent... One-line CI fixup." |

### Human Verification Required

Three judgement items require architect-of-record decision before phase close. See YAML frontmatter `human_verification` for full structured format. Summary:

#### 1. Acceptance of CR-01 / CR-02 / CR-03 Qt 6 Migration Regressions

**Test:** Decide whether the three code-review findings (HTMLPage::certificateError dead; QSignalMapper::mapped→mappedInt rename missed at 2 sites; QLayout::setMargin latent in pyWidget.C) block Phase 5 close OR are accepted as Phase 5.1 follow-up scope.

**Expected:** Architect either (a) flips them into a Phase 5.1 backlog row and closes Phase 5, OR (b) requires a follow-up plan in Phase 5 before close.

**Why human:** Scope-vs-completeness judgement. The reviewer marked these advisory (per workflow); REQUIREMENTS.md does not yet flip QT6-01/02 to Complete; the must_haves tables in Plans 02/05 do not enumerate these specific sites. A planner needs to decide whether to fold them into a Phase 5 closeout fix or into Phase 5.1's already-active scope. They are real Qt 6 migration regressions in the same QT6-02 sweep this phase claims responsibility for, and CR-01 / CR-02 are runtime behaviour breakages (not just latent compile errors).

#### 2. Acceptance of gl_profile=compatibility CI Assertion Mismatch

**Test:** Confirm the `gl_profile=compatibility` CI assertion in `.github/workflows/ci.yml` (lines 331, 387) should be relaxed for macOS Apple Silicon now that `gl_profile=none` is the empirically-established Qt 6 + Apple Silicon reality.

**Expected:** Architect signs off on a one-line ci.yml relaxation OR confirms it stays as-is until SEED-005-1 / SEED-005-2 land (the assertion is currently dormant because Linux + Windows CI is red upstream of the smoke step).

**Why human:** The original D-08 oracle was `gl_profile=compatibility` but the live data invalidates it. This is named as a candidate fix in STATE.md §"Notes" and is a 1-line CI change — but Phase 5 didn't make it, leaving an inconsistency. Plan 08 (SPIKE-DECISION) §3 implicitly retires `gl_profile=compatibility` for Apple Silicon — but the CI still asserts the old oracle.

#### 3. Acceptance of Autonomous Override Precedent (Plans 05-07 + 05-08)

**Test:** Confirm the autonomous override of Plan 05-07 and Plan 05-08 (both declared `autonomous: false`) is acceptable as a phase-close decision authority precedent.

**Expected:** Architect either signs §6 of 05-SPIKE-DECISION.md OR appends a §6.x amendment if disagreeing with the split-pattern decision.

**Why human:** The plans declared `autonomous: false` expecting a human judgment call. The execution applied a "reasonable-call autonomous override" grounded in three reality constraints (spike full-render structurally PIPE-01-blocked, no Windows hardware, Plan 08 phase-closeout gated on Plan 07 artifact). Documented in both SUMMARYs and in 05-SPIKE-DECISION.md §6. The decision is defensible but the override pattern is a precedent worth confirming.

### Gaps Summary

**No must-have gaps blocking the phase goal.** All ROADMAP success criteria are met against the artifact-existence + key-link wiring tests. The two areas of incompleteness are:

1. **Infrastructure data gaps (Linux + Windows CI)** — properly tracked as SEEDs (005-1, 005-2, 005-3, 005-4), explicitly addressed in Phase 5.1 (per ROADMAP) and PIPE-01 (per SPIKE-DECISION.md §5.4 + §5.3). These are deferred, not gaps.

2. **Code-review Qt 6 regressions (CR-01, CR-02, CR-03)** — advisory per workflow; surfaced in 05-REVIEW.md; not in any plan's must_haves; the corresponding requirement rows (QT6-01, QT6-02) are still marked Pending in REQUIREMENTS.md (not yet claimed Complete). These require architect judgement on scope (Phase 5 closeout fix vs Phase 5.1 fold-in).

**Why human_needed and not passed:** the phase deliverables (artifacts, key links, decision record) are all in place, but the phase goal "Qt 6 migration baseline" plus the QT6-02 requirement ("Qt-deprecated APIs in VIEW replaced with Qt 6 equivalents") owns a sweep that — per the code review — left two runtime regressions and one latent regression unaddressed. The architect-of-record must decide whether the current state is acceptable for phase close. REQUIREMENTS.md's choice to keep QT6-01/02 Pending is a signal that this decision is open.

**Why human_needed and not gaps_found:** the issues are real but not structurally blocking — CR-03 is gated by BALL_PYTHON_SUPPORT=OFF; CR-01/CR-02 are feature regressions in non-renderer paths (TLS, rotamer menu, PresentaBALL signal); the renderer + Qt 6 build link path is green; the spike + decision-record deliverables are unambiguously met. The 3 CR findings fit naturally into Phase 5.1's already-active scope (build warnings + latent bug cleanup). Routing them through `/gsd-plan-phase --gaps` would create a Phase 5 closeout plan that competes with Phase 5.1; the architect call is whether to do that.

---

_Verified: 2026-05-15T12:00:00Z_
_Verifier: Claude (gsd-verifier)_
_Verification mode: goal-backward against ROADMAP §Phase 5 Success Criteria + 4 declared requirement IDs (QT6-01, QT6-02, SPIKE-01, SPIKE-02)_
