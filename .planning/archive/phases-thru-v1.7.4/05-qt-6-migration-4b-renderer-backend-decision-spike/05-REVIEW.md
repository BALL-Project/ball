---
phase: 05-qt-6-migration-4b-renderer-backend-decision-spike
reviewed: 2026-05-15T00:00:00Z
depth: standard
files_reviewed: 79
files_reviewed_list:
  - .github/workflows/ci.yml
  - CMakeLists.txt
  - include/BALL/FORMAT/dockResultFile.h
  - include/BALL/SYSTEM/mutex.h
  - include/BALL/VIEW/DIALOGS/FDPBDialog.h
  - include/BALL/VIEW/DIALOGS/bondProperties.h
  - include/BALL/VIEW/DIALOGS/coloringSettingsDialog.h
  - include/BALL/VIEW/DIALOGS/compositeProperties.h
  - include/BALL/VIEW/DIALOGS/downloadElectronDensity.h
  - include/BALL/VIEW/DIALOGS/downloadPDBFile.h
  - include/BALL/VIEW/DIALOGS/editSettings.h
  - include/BALL/VIEW/DIALOGS/editSingleShortcut.h
  - include/BALL/VIEW/DIALOGS/generateCrystalDialog.h
  - include/BALL/VIEW/DIALOGS/gridVisualizationDialog.h
  - include/BALL/VIEW/DIALOGS/lightSettings.h
  - include/BALL/VIEW/DIALOGS/mainControlPreferences.h
  - include/BALL/VIEW/DIALOGS/materialSettings.h
  - include/BALL/VIEW/DIALOGS/modelSettingsDialog.h
  - include/BALL/VIEW/DIALOGS/modifyRepresentationDialog.h
  - include/BALL/VIEW/DIALOGS/networkPreferences.h
  - include/BALL/VIEW/DIALOGS/openSavePreferences.h
  - include/BALL/VIEW/DIALOGS/peptideDialog.h
  - include/BALL/VIEW/DIALOGS/raytraceableContourSurfaceDialog.h
  - include/BALL/VIEW/DIALOGS/setCamera.h
  - include/BALL/VIEW/DIALOGS/shortcutDialog.h
  - include/BALL/VIEW/DIALOGS/stageSettings.h
  - include/BALL/VIEW/DIALOGS/undoManagerDialog.h
  - include/BALL/VIEW/RENDERING/RENDERERS/coreGLRenderer.h
  - include/BALL/VIEW/RENDERING/RENDERERS/rhiRenderer.h
  - include/BALL/VIEW/RENDERING/glRenderWindow.h
  - include/BALL/VIEW/RENDERING/qtRhiSurface.h
  - include/BALL/VIEW/RENDERING/rendererFactory.h
  - include/BALL/VIEW/WIDGETS/HTMLPage.h
  - include/BALL/VIEW/WIDGETS/scene.h
  - source/EXTENSIONS/BALLAXY/include/BALLaxyInterfacePreferences.h
  - source/EXTENSIONS/JUPYTER/include/jupyterPreferences.h
  - source/EXTENSIONS/PRESENTABALL/include/PresentaBALLSettings.h
  - source/FORMAT/dockResultFile.C
  - source/VIEW/DATATYPE/dataset.C
  - source/VIEW/DATATYPE/standardDatasets.C
  - source/VIEW/DIALOGS/FDPBDialog.C
  - source/VIEW/DIALOGS/atomOverview.C
  - source/VIEW/DIALOGS/coloringSettingsDialog.C
  - source/VIEW/DIALOGS/dockDialog.C
  - source/VIEW/DIALOGS/downloadElectronDensity.C
  - source/VIEW/DIALOGS/exportGeometryDialog.C
  - source/VIEW/DIALOGS/labelDialog.ui
  - source/VIEW/DIALOGS/lightSettings.C
  - source/VIEW/DIALOGS/molecularFileDialog.C
  - source/VIEW/DIALOGS/pluginDialog.C
  - source/VIEW/DIALOGS/preferences.C
  - source/VIEW/DIALOGS/pythonSettings.C
  - source/VIEW/KERNEL/MODES/editMode.C
  - source/VIEW/KERNEL/MODES/interactionMode.C
  - source/VIEW/KERNEL/MODES/interactionModeManager.C
  - source/VIEW/KERNEL/MODES/moveMode.C
  - source/VIEW/KERNEL/MODES/rotateMode.C
  - source/VIEW/KERNEL/mainControl.C
  - source/VIEW/KERNEL/serverWidget.C
  - source/VIEW/KERNEL/shortcutRegistry.C
  - source/VIEW/RENDERING/RENDERERS/coreGLRenderer.C
  - source/VIEW/RENDERING/RENDERERS/rhiRenderer.C
  - source/VIEW/RENDERING/RENDERERS/sources.cmake
  - source/VIEW/RENDERING/glOffscreenTarget.C
  - source/VIEW/RENDERING/qtRhiSurface.C
  - source/VIEW/RENDERING/rendererFactory.C
  - source/VIEW/RENDERING/sources.cmake
  - source/VIEW/WIDGETS/HTMLPage.C
  - source/VIEW/WIDGETS/SDWidget.C
  - source/VIEW/WIDGETS/colorButton.C
  - source/VIEW/WIDGETS/colorTable.C
  - source/VIEW/WIDGETS/dockWidget.C
  - source/VIEW/WIDGETS/geometricControl.C
  - source/VIEW/WIDGETS/molecularControl.C
  - source/VIEW/WIDGETS/propertyEditor.C
  - source/VIEW/WIDGETS/pyWidget.C
  - source/VIEW/WIDGETS/scene.C
  - source/VIEW/WIDGETS/testFramework.C
  - source/VIEW/WIDGETS/textEditorWidget.C
findings:
  critical: 3
  warning: 7
  info: 4
  total: 14
status: issues_found
---

# Phase 5: Code Review Report

**Reviewed:** 2026-05-15
**Depth:** standard
**Files Reviewed:** 79
**Status:** issues_found

## Summary

Phase 5 is a Qt 5 → Qt 6 migration plus two throwaway renderer spikes (GL-core, QRhi). The CMake/CI/lint scaffolding and the spike gating (CMake option + env var + ifdef + Qt 6.7 floor check) are all correctly structured — Qt6::GuiPrivate and the spike sources are properly walled off from production builds. The lint script `scripts/check-no-legacy-qt6-symbols.sh` passes against the current tree (verified locally).

However, the Qt 6 API sweep (commit `a0c28bc`, BLOCKER-E) is **incomplete**, and three substantive Qt-6-behaviour regressions slipped through:

1. **`HTMLPage::certificateError` is silently dead** — Qt 6 turned the virtual hook into a *signal*, the header dropped `override` but no new `connect()` rewires the now-method-shaped function. Any caller passing `ignore_ssl_errors=true` will silently see SSL errors REJECTED in Qt 6 (Qt 5 honoured the flag). Behaviour regression in a security-adjacent path.
2. **`QSignalMapper::mapped(int)` SIGNAL connections are runtime-broken** — Qt 6 renamed the signal to `mappedInt(int)`. Two `connect(..., SIGNAL(mapped(int)), ...)` sites (`molecularControl.C:498`, `PresentaBALLView.C:40`) now silently no-op — confirmed by the live Qt 6 evidence in 05-06-SUMMARY (`QObject::connect: No such signal QSignalMapper::mapped(int)`). The rotamer-menu feature is broken; PresentaBALL action signals likewise.
3. **`pyWidget.C` still uses `QLayout::setMargin()`** (removed in Qt 6) on `QGridLayout` and `QHBoxLayout` at lines 348 and 412. This file is gated by `BALL_PYTHON_SUPPORT` (OFF in CI), so the build is green today, but flipping that option ON will fail to compile. Plan 05-05's Qt 6 sweep documented `QLayout::setMargin` as resolved, but two sites were missed.

Two of these three were potentially observable in the runtime evidence captured during Plan 05-05/06 (the `QObject::connect: No such signal` warning is in the 05-06 SUMMARY's runtime smoke output) but were not back-fed into the fix sweep. They should be addressed before BALLView is considered Qt-6-clean.

Additional issues: spike code quality concerns around hardcoded GL hex literals and a non-strict env-var gate; the QXmlAttributes overload stub is correct in structure but the surrounding name-vs-qName subtlety is worth flagging; the QRhi spike has lifetime-ordering concerns around QRhi resources when surface/renderer destruction interleaves.

The CI workflow is well-structured (matrix-driven, smoke-checks teed to log files, USE_QTWEBENGINE=OFF on Windows for vcpkg manifest gap). The lint script structure is sound and exits 0 against the current tree.

## Critical Issues

### CR-01: `HTMLPage::certificateError` silently no-ops under Qt 6 — `ignore_ssl_errors_` flag is now dead

**File:** `include/BALL/VIEW/WIDGETS/HTMLPage.h:28`, `source/VIEW/WIDGETS/HTMLPage.C:38-41`
**Issue:** In Qt 5, `QWebEnginePage::certificateError(const QWebEngineCertificateError&)` was a `virtual` hook callers could override to choose whether to ignore an SSL error. In Qt 6, `certificateError` was changed to a **signal** (per Qt 6.2+ release notes). The Phase 5 sweep correctly dropped the `override` annotation (header comment lines 25-27 document the change), but it did **not** rewire the function as a slot connected to the now-signal. The result: `HTMLPage::certificateError(...)` exists as a plain non-virtual method that is **never invoked**. Any caller constructing `HTMLPage(parent, /*ignore_ssl_errors=*/true)` will, under Qt 6, see the page DEFAULT-reject the certificate — the opposite of the requested behaviour. Silent behaviour regression in a TLS-trust path.

**Fix:**

```cpp
// HTMLPage.h — change from non-virtual member to a slot, kept on the class
private slots:
    bool certificateErrorSlot(const QWebEngineCertificateError& error);

// HTMLPage.C — connect the signal in both ctors:
HTMLPage::HTMLPage(QObject* parent, bool ignore_ssl_errors)
    : QWebEnginePage(parent), ignore_ssl_errors_(ignore_ssl_errors)
{
    connect(this, &QWebEnginePage::certificateError,
            this, &HTMLPage::certificateErrorSlot);
}

bool HTMLPage::certificateErrorSlot(const QWebEngineCertificateError& error)
{
    if (ignore_ssl_errors_) {
        const_cast<QWebEngineCertificateError&>(error).acceptCertificate();
        return true;
    }
    return false;
}
```

Note Qt 6's signal-based API now expects `error.acceptCertificate()` / `error.rejectCertificate()` to be called explicitly on the error object — the return-value-as-decision pattern of Qt 5 is gone. The fix above honours `ignore_ssl_errors_` correctly.

### CR-02: `QSignalMapper::mapped(int)` SIGNAL connections silently fail under Qt 6 (rotamer menu broken)

**File:** `source/VIEW/WIDGETS/molecularControl.C:498`, `source/EXTENSIONS/PRESENTABALL/source/PresentaBALLView.C:40`
**Issue:** Qt 6.0 renamed the `QSignalMapper::mapped()` overloads to disambiguate them — the legacy `mapped(int)` is now `mappedInt(int)` (similarly `mapped(QString)` → `mappedString`, etc.). The string-based `SIGNAL(mapped(int))` macro silently fails at connection time in Qt 6: Qt prints `QObject::connect: No such signal QSignalMapper::mapped(int)` at the warning log and the connection is never wired. This means:

- `molecularControl.C:498` — clicking the rotamer-application menu does nothing (the `changeRotamer_` slot is never invoked).
- `PresentaBALLView.C:40` — the PresentaBALL action signal is never fanned out.

Direct evidence: the 05-06-SUMMARY captured this exact warning in the spike smoke-check output (`QObject::connect: No such signal QSignalMapper::mapped(int) (unknown context)`), so the failure is observable today but was not fed back into the fix sweep. (`PresentaBALLView.C` is not in the changed-files list, but `molecularControl.C` is — it was touched as part of the Qt 6 sweep.)

**Fix (per-site):**

```cpp
// molecularControl.C — switch to the renamed signal:
connect(rotamer_mapper_, &QSignalMapper::mappedInt,
        this, &MolecularControl::changeRotamer_);

// PresentaBALLView.C — same pattern:
connect(signalMapper_, &QSignalMapper::mappedInt,
        signal_, &SomeSignalClass::actionSignal);
```

Both string-based SIGNAL macro connections are also vulnerable to typo bugs at connect time; prefer the function-pointer syntax which catches mismatches at compile time.

### CR-03: `QLayout::setMargin()` removed in Qt 6 — two sites in `pyWidget.C` are latent build failures

**File:** `source/VIEW/WIDGETS/pyWidget.C:348`, `source/VIEW/WIDGETS/pyWidget.C:412`
**Issue:** Qt 6 removed `QLayout::setMargin(int)` (replaced by `setContentsMargins(int, int, int, int)`). Plan 05-05's Qt 6 API sweep (commit `a0c28bc`) explicitly listed this rename and converted the two scene.C sites — but missed two pyWidget.C sites:

```cpp
// Line 348 — on a QGridLayout:
QGridLayout* lay = new QGridLayout(widget);
lay->setMargin(0);                          // <-- Qt 6 compile error

// Line 412 — on a QHBoxLayout:
QHBoxLayout *hlayout = new QHBoxLayout;
hlayout->setMargin(1);                      // <-- Qt 6 compile error
```

`pyWidget.C` is gated by `BALL_PYTHON_SUPPORT` (OFF by default and in CI), so the current build is green. The build will hard-fail the moment Python support is re-enabled. This is a latent Qt 6 BLOCKER, not a current one — but flagging as Critical because the project's own sweep claimed completeness and the bug is a hard compile error on any future Python build.

**Fix:**

```cpp
// Line 348:
lay->setContentsMargins(0, 0, 0, 0);

// Line 412:
hlayout->setContentsMargins(1, 1, 1, 1);
```

## Warnings

### WR-01: Spike env-var gate accepts any non-empty value (including `=0`) — opposite of "==1" semantics in comment

**File:** `source/VIEW/RENDERING/rendererFactory.C:51`
**Issue:** The comment at lines 42-50 says the spike is activated "when `BALLVIEW_USE_SPIKE_BACKEND=1` is set at launch", but the actual check is:

```cpp
if (kind == Kind::OpenGL_Fixed && std::getenv("BALLVIEW_USE_SPIKE_BACKEND") != nullptr)
```

`std::getenv` returns non-null even when the variable is set to `0`, `false`, `off`, or any non-empty value. A user explicitly setting `BALLVIEW_USE_SPIKE_BACKEND=0` to disable the spike will instead activate it. Spike is throwaway, but the comment-vs-code mismatch is a footgun for any developer trying to disable the spike from a shell script.

**Fix:**

```cpp
const char* env = std::getenv("BALLVIEW_USE_SPIKE_BACKEND");
if (kind == Kind::OpenGL_Fixed && env != nullptr && env[0] == '1' && env[1] == '\0')
{
  // activate spike
}
```

Or accept any "truthy" value but document it. Either way: code and comment must agree.

### WR-02: QRhi resource destruction in `~QRhiRenderer()` may UAF if surface dies first

**File:** `source/VIEW/RENDERING/RENDERERS/rhiRenderer.C:84-92`
**Issue:** `QRhiRenderer::~QRhiRenderer()` deletes `pipeline_`, `srb_`, `vbuf_`, all of which are owned by the QRhi instance that lives on the paired `QtRhiSurface` (QRhiWidget). In Qt's normal QObject ownership flow, the surface is deleted when its parent is deleted, which may happen before `QRhiRenderer` is destroyed. If the QRhi instance is gone when the renderer destructor runs, `delete pipeline_` etc. on dangling pointers is undefined behaviour (likely a crash on exit).

The header comment (line 86-87) asserts "QRhi resources are reference-counted by the QRhi instance" — that is not accurate. QRhi resources (`QRhiBuffer`, `QRhiGraphicsPipeline`, `QRhiShaderResourceBindings`) must be deleted before the QRhi instance is destroyed; they are not reference-counted.

Spike is throwaway and currently crashes during init anyway, so this is not exercised today. Flagging for spike code review and PIPE-01 carry-forward.

**Fix:** Either (a) explicitly clear the resources via `releaseResources()` at the end of a frame before the surface dies, (b) check `vbuf_ != nullptr && rhi_still_alive()` via a kept QPointer, or (c) document the destruction ordering contract clearly.

### WR-03: dockResultFile attributesToHashMap — Qt 5 `qName` vs Qt 6 `name()` semantic delta

**File:** `source/FORMAT/dockResultFile.C:2065-2082`
**Issue:** The Qt 5 SAX overload (now ifdef-stubbed) iterates attributes using `attributes.qName(i)` (qualified name including namespace prefix). The Qt 6 QXmlStreamReader overload iterates using `attributes.at(i).name()` — the **local** name only, namespace prefix stripped. For the DockResultFile XML format, which uses no namespaces, this is functionally equivalent. But if an external producer ever emits namespaced attributes, the two code paths will silently disagree. Worth a comment, since the file already calls out the Qt 5/6 split.

**Fix:** Either add a comment documenting the no-namespace assumption, or — if you want byte-identical behaviour to the Qt 5 path — use `attributes.at(i).qualifiedName()` on the QXmlStreamAttributes side.

### WR-04: QRhi spike pipeline build silently leaks `srb_` if `pipeline_->create()` succeeds but later resource setup fails

**File:** `source/VIEW/RENDERING/RENDERERS/rhiRenderer.C:122-170`
**Issue:** The early-return paths in `ensureResources_()` partially construct resources but never release them on the failure path:

- If `vbuf_->create()` fails (line 136): `vbuf_` is allocated (`rhi->newBuffer`) but not stored; `srb_` and `pipeline_` not yet allocated. The `vbuf_` raw pointer is leaked unless the QRhi destructor sweeps unattached resources (it doesn't).
- If `srb_->create()` fails (line 148): `vbuf_` is now leaked.
- The function returns `false` but `resources_built_` stays `false`, so a future call retries from the top and overwrites the pointers, leaking the previous attempts.

Spike is throwaway, but the leak is real even on the happy retry path. The `resources_built_` guard at line 124 only prevents re-entry once everything succeeds.

**Fix:** On every early-return failure path, delete the partially-allocated resources and null the pointers:

```cpp
if (!vbuf_ || !vbuf_->create()) {
    delete vbuf_; vbuf_ = nullptr;
    return false;
}
// repeat for srb_, pipeline_
```

### WR-05: coreGLRenderer.C uses hardcoded hex literals for every GL constant instead of including the headers

**File:** `source/VIEW/RENDERING/RENDERERS/coreGLRenderer.C` (lines 149, 161, 162, 171, 195-216, 282-284, 319-332, 358-389, etc.)
**Issue:** Every GL constant is a magic hex literal with a comment naming what it should be (`/*GL_TEXTURE_2D=*/0x0DE1`). A typo in any of these compiles cleanly and produces silently-wrong GL state. The spike is throwaway, but this pattern is fragile: e.g., I caught one nearly-correct-looking literal (`/*GL_COLOR=*/0x1800` at line 363) which is in fact correct for `glClearBuffer*` semantics — but a reviewer cannot tell without consulting the GL headers, defeating the comment.

**Fix:** Include the GL constants properly. `QOpenGLFunctions_3_2_Core` doesn't expose them as members, but they are available via:

```cpp
#include <QtOpenGL/QOpenGLFunctions_3_2_Core>
// GL constants are macros from <GL/gl.h> or QtGui/qopenglext.h; include one explicitly:
#include <QtGui/qopengl.h>
// Then use GL_TEXTURE_2D, GL_R32UI, etc. directly.
```

Spike-tolerable but a real maintenance risk.

### WR-06: Picking pass uses stale MVP uniform — `pickObjects()` does not re-set `u_mvp`

**File:** `source/VIEW/RENDERING/RENDERERS/coreGLRenderer.C:345-403`
**Issue:** `pickObjects()` re-uses the shader program but does not re-issue `glUniformMatrix4fv(uloc_mvp_, ...)`. Comment at lines 366-370 documents the assumption that "RenderSetup invokes pickObjects immediately after rendering, so the program/VAO/MVP state is still set" — but this is fragile:

- If picking is invoked without a prior main-pass render in the same frame, the MVP is whatever was last bound (possibly garbage on first call).
- The main pass at line 339-342 unbinds the program (`glUseProgram(0)`). The pick pass at line 359 re-binds it. **Uniforms are program state, not global state**, so they survive the unbind/rebind cycle — the documented assumption is technically correct here. But this only works because uniforms persist across program use cycles; many GL programmers would mistakenly think otherwise.

Spike-tolerable, but should be made explicit (re-set MVP in pickObjects defensively).

**Fix:** In `pickObjects()`, after `glUseProgram(program_id_)`, explicitly set the MVP again (cheap):

```cpp
fns->glUniformMatrix4fv(uloc_mvp_, 1, /*transpose=*/0, cached_mvp_);
```

Store `cached_mvp_` as a member from the most recent `renderRepresentations_` call.

### WR-07: `coreGLRenderer.C` `compile_one` lambda leaks shader handle on failure

**File:** `source/VIEW/RENDERING/RENDERERS/coreGLRenderer.C:144-159`
**Issue:** The local lambda creates a shader, fails compilation, calls `glDeleteShader(s)`, and returns 0. So far so good. But the caller path at line 161-164:

```cpp
unsigned int vs = compile_one(GL_VERTEX_SHADER, k_vertex_shader_src_);
unsigned int fs = compile_one(GL_FRAGMENT_SHADER, k_fragment_shader_src_);
if (!vs || !fs)
    return false;
```

…leaks `vs` if `fs == 0`: the vertex shader succeeded but the function returns without deleting `vs`. Conversely, `fs` is properly deleted (the lambda calls `glDeleteShader` before returning 0). Small leak, but a leak nonetheless. The shaders are also detached/deleted at lines 172-173 only on the success path.

**Fix:**

```cpp
if (!vs || !fs) {
    if (vs) fns->glDeleteShader(vs);
    if (fs) fns->glDeleteShader(fs);
    return false;
}
```

## Info

### IN-01: `QtRhiSurface` lacks `Q_OBJECT` despite inheriting `QRhiWidget` (a QObject)

**File:** `include/BALL/VIEW/RENDERING/qtRhiSurface.h:63-65`
**Issue:** `QtRhiSurface` inherits both `RenderSurface` and `QRhiWidget`, but does not declare `Q_OBJECT`. Inherited signals/slots from `QRhiWidget` still work, but no new signals/slots can be defined on `QtRhiSurface`, and `qobject_cast<QtRhiSurface*>` will not succeed. Throwaway-tolerable. No fix required unless the spike grows.

### IN-02: `[SPIKE]` log lines go to `std::cerr` in QRhi files but `Log.info()` in CoreGLRenderer — inconsistent

**File:** `source/VIEW/RENDERING/RENDERERS/rhiRenderer.C:81`, `source/VIEW/RENDERING/qtRhiSurface.C:34`, vs `source/VIEW/RENDERING/RENDERERS/coreGLRenderer.C:75, 119, 134`
**Issue:** The GL-core spike uses BALL's `Log.info()`/`Log.warn()`; the QRhi spike uses raw `std::cerr`. Both work but the inconsistency may cause confusion in log routing — `Log` writes to BALL's `LogStream`, while `std::cerr` bypasses it.

**Fix:** Pick one. Recommended: `Log.info()` everywhere (matches project convention).

### IN-03: dockResultFile.C ctor uses raw `0` literal where `nullptr` is canonical

**File:** `source/FORMAT/dockResultFile.C:26-37, 40`
**Issue:** The ctor initializer list and the body still write `xmlIn_ = 0;`, `receptor_(0)`, etc. Not a Phase 5 regression (existed pre-Qt-6 port), but the file was touched and the Phase 5 work converted other `0` → `nullptr` in some places. Consistency drift.

**Fix:** Mechanical `0` → `nullptr` for pointer initializers. Optional cleanup, not blocking.

### IN-04: `glOffscreenTarget.C` line 84 falls through to `share_from_->grabFramebuffer()` without bounds-checking nullness

**File:** `source/VIEW/RENDERING/glOffscreenTarget.C:84`
**Issue:** `getImage()` first checks `if (fbo_)`, then `else if (share_from_)`, then returns an empty `QImage()`. If `share_from_` is null AND `fbo_` is null, you get an empty image — defensive. But `updateImageTile()` at lines 102-122 does the same fbo/share_from check but then calls `QPainter p(&current_image_)`; if `current_image_` is null/empty, the QPainter constructor will fail-soft and any draws are no-ops. Pre-existing behaviour. Flagging because the file was touched by Phase 5.

**Fix:** Optional — add an assertion or explicit early-return if `current_image_.isNull()`.

---

_Reviewed: 2026-05-15_
_Reviewer: Claude (gsd-code-reviewer)_
_Depth: standard_
