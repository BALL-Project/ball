# Codebase Concerns

**Analysis Date:** 2026-05-14

## Tech Debt

**C++14 Language Bridge (Load-bearing limitation):**
- Issue: Project uses C++14 to maintain access to `std::unary_function`, `std::binary_function`, `std::bind2nd`, `std::bind1st`, `std::ptr_fun`, `std::mem_fun` and `std::auto_ptr` which were removed in C++17
- Files: `cmake/BALLCompilerSpecific.cmake` (sets `-std=c++14`), `include/BALL/CONCEPT/predicate.h`, `include/BALL/DATATYPE/hashGrid.h`, `include/BALL/KERNEL/standardPredicates.h`, `include/BALL/MATHS/parsedFunction.h`, `include/BALL/STRUCTURE/residueRotamerSet.h`, `source/CONCEPT/enumerator.C`, `source/STRUCTURE/DNAMutator.C`
- Impact: Cannot adopt C++17+ modern practices; blocks any new dependencies requiring C++17 headers; prevents use of modern standard library algorithms and containers. This is the primary blocker for Phase 2 modernization.
- Fix approach: Replace all occurrences with C++11 lambdas, `std::function`, and explicit functor structs. Phase 2 (ROADMAP) dedicated to this — must complete before moving to C++17.

**Deprecated Python Support Infrastructure:**
- Issue: SIP (Swig Interface Generator) 4.16 from bundled `ball_contrib` is obsolete; modern SIP 6 is incompatible with the BALL build glue; Python bindings have ~237 `.sip` files that are no longer maintained
- Files: `include/BALL/PYTHON/pyKernel.h`, `include/BALL/PYTHON/pyCAPIKernel.h`, `include/BALL/PYTHON/pyInterpreter.h` (all marked with 8 `BALL_DEPRECATED` methods)
- Impact: `BALL_PYTHON_SUPPORT=OFF` required to build; Python bindings completely unavailable; end users cannot use BALL from Python
- Fix approach: Phase 5 (ROADMAP): Either modernize to SIP 6 or migrate to pybind11/nanobind. Large but mechanical rewrite — worth deferring to post-1.6.

**Deprecated Macro Utilities (50+ obsolete functions):**
- Issue: `include/BALL/COMMON/macros.h` contains ~50 functions (e.g., `ball_macro_max`, `ball_macro_abs`, `ball_macro_real_equal`, `ball_macro_bit_set`, etc.) all marked `BALL_DEPRECATED` — legacy pre-C++11 macro patterns
- Files: `include/BALL/COMMON/macros.h` (lines 20–257)
- Impact: Cruft in public API; misleads users about what to use; build warnings if consumed
- Fix approach: Remove wholesale once refactoring is complete. Short-term: leave as-is (marked deprecated).

**Deprecated Genetic Docking Classes:**
- Issue: `include/BALL/DOCKING/GENETICDOCK/parameter.h` exports `GenericParameter`, `DoubleParameter`, `QuaternionParameter` all marked `BALL_DEPRECATED` since at least 2022
- Files: `include/BALL/DOCKING/GENETICDOCK/parameter.h` (lines 14, 47, 73)
- Impact: API surface contains non-functional classes; misleads users about genetic docking availability
- Fix approach: Remove in next major version after 1.6.

**Deprecated Qt/Platform APIs:**
- Issue: `BALL_DEPRECATED` markers on Python interpreter APIs that relied on old Qt binding infrastructure
- Files: `include/BALL/PYTHON/pyInterpreter.h` (lines 67, 73, 76, 79, 94, 101, 121) — methods `setSysPath`, `getSysPath`, `isValid`, `getStartupLog`, `run`, `runFile`, `execute`
- Impact: None if Python support is off; non-functional if enabled
- Fix approach: Defer to Phase 5 (Python bindings rewrite).

---

## Critical Bugs (Pre-existing)

**ContourSurface Operator== — NOT A BUG (entry retained for history):**
- Status: Verified 2026-05-16 by ROADMAP-AUDIT-V1.6.2.md §D1 — implementation at `include/BALL/DATATYPE/contourSurface.h:302-306` correctly returns `((threshold_ == data.threshold_) && Surface::operator == (data))`. The original concerns audit (2026-05-14) inspected only the declaration at line 107 and inferred a defect; the implementation lives lower in the same template-inline header and is correct.
- Action: none. Lesson for future audits: verify implementation lines, not just declarations, before flagging template-inline classes as bugs.

---

## Architecture & Design Issues

**Single-threaded Rendering Architecture (Highest Risk for Phase 4):**
- Issue: `GLRenderWindow` derives from **deprecated `QGLWidget`** (Qt4-era API, no layer-backing support on modern Cocoa); `RenderSetup` is a `QThread` that calls `makeCurrent()` from worker thread while GUI thread does buffer swaps. `QOpenGLWidget` (Qt 5.15+ standard) requires all rendering on GUI thread.
- Files: `source/VIEW/RENDERING/glRenderWindow.h`, `source/VIEW/RENDERING/glRenderWindow.C`, `source/VIEW/RENDERING/renderSetup.C`, `source/VIEW/RENDERING/glRenderer.C`, `source/VIEW/WIDGETS/scene.C`
- Impact: **Blocks rendering on macOS Tahoe:** Qt Cocoa plugin layer-backs all `NSView`s; `QGLWidget` cannot be composited, so Qt detaches it to its own invisible `QNSWindow` (100x30 pixels off-screen). 3D scene stays blank on macOS despite app launching successfully. Works on older macOS/Windows but architecture is obsolete.
- Safe modification: Phase 4 (ROADMAP) requires:
  1. **Rewrite threading model:** Render threads draw into **offscreen `QOpenGLFramebufferObject` + shared `QOffscreenSurface`**; `QOpenGLWidget::paintGL()` blits FBO texture to screen (pattern already exists for raytracer path)
  2. Move `makeCurrent()` calls from worker threads; use context sharing instead
  3. Update: `renderSetup.C` (`makeCurrent`, `QGLContext::currentContext()`), `glOffscreenTarget.C`, `scene.C` (`updateGL()`, `grabFrameBuffer()`), `glRenderer.C` (`QGLWidget::convertToGLFormat`)
- Test coverage: No existing render-threading unit tests; risk is high

**Fixed-Function OpenGL Renderer (100+ calls, platform-deprecated):**
- Issue: `source/VIEW/RENDERING/RENDERERS/glRenderer.C` uses ~100 fixed-function GL calls (`glBegin`, `glVertex`, `glMatrixMode`, `gluLookAt`, etc.) — deprecated on macOS, removed entirely in GL 4.0 core profile
- Files: `source/VIEW/RENDERING/RENDERERS/glRenderer.C` (~1155 lines, line 1155 contains `QGLWidget::convertToGLFormat`)
- Impact: Will not run on Apple's future macOS versions (GL removed platform-wide); non-portable to modern GL; very slow. Currently survives because `QGLWidget` defaulted to GL 2.1 compat profile. `QOpenGLWidget` can request compat profile short-term (Phase 4), but this is a long-term liability.
- Safe modification: Phase 4 requests compat profile via `QSurfaceFormat`; fixed-function still runs. Phase 5+ (post-1.6): Programmable-pipeline rewrite (shaders + VAOs/VBOs) — significant effort.
- Test coverage: No render-correctness tests; only visual verification in GUI

---

## Networking Fragility (Phase 6)

**Boost.Asio API Fragility:**
- Issue: `include/BALL/SYSTEM/networking.h` and `source/SYSTEM/networking.C` minimally patched from Boost 1.60 to 1.90+ API (io_service→io_context, post() moved, TCPIOStream socket base made private). Design uses legacy synchronous `tcp::iostream` model which is fragile.
- Files: `include/BALL/SYSTEM/networking.h`, `source/SYSTEM/networking.C`, `include/BALL/SYSTEM/mutex.h` (line 210 TODO)
- Impact: Networking code is a patchwork; no unit tests; synchronous blocking design doesn't scale; hard to debug connection issues
- Fix approach: Phase 6 (ROADMAP): Rewrite `TCPServer` onto modern Asio async acceptor/socket model, or replace with Qt networking (already a dependency in VIEW). Add comprehensive unit tests.

**Missing Unit Tests for Networking:**
- Issue: No test coverage for `TCPServer`, `TCPServerThread`, `TCPIOStream`
- Files: All networking classes lack corresponding test files
- Impact: Silent breakage on Boost version changes; threading bugs undetected

**Mutex Template Instantiation Incomplete:**
- Issue: `include/BALL/SYSTEM/mutex.h` line 210 contains TODO: `// TODO: similar instantiation for TReadWriteLock` — suggests incomplete C++ template instantiation coverage
- Files: `include/BALL/SYSTEM/mutex.h`
- Impact: `TReadWriteLock` may not be fully compiled in; undefined behavior if instantiated with custom types

---

## Performance & Scalability Concerns

**Binary Data Serialization Missing Endianness Handling:**
- Issue: `include/BALL/DATATYPE/regularData3D.h` lines 1298 and 1350 contain TODOs: `// TODO: check for endiannes and swap bytes accordingly` in `binaryWrite()` and `binaryRead()`
- Files: `include/BALL/DATATYPE/regularData3D.h`
- Impact: Binary 3D volume files written on big-endian systems are unreadable on little-endian systems (and vice versa); silently produces corrupted data with no error checking
- Fix approach: Implement endianness detection and byte-swapping in both methods; add round-trip tests

**HashMap Size Type Narrowing (Persistence Breaking):**
- Issue: `include/BALL/DATATYPE/hashMap.h` line 121 TODO: `// TODO: Remove this. This narrows the precision of size_t! (But of course breaks persistence...)`
- Files: `include/BALL/DATATYPE/hashMap.h`, method `size()`
- Impact: `size()` returns `Size` (which narrows `size_t` precision); blocks future 64-bit large-scale simulations; persistence format is locked
- Fix approach: Cannot fix without breaking serialization compatibility. Blocks scaling to large problem sizes.

---

## Dependency Issues

**Embedded Old SVM Library (libsvm):**
- Issue: `include/BALL/QSAR/libsvmModel.h` line 64 contains unexplained `// XXX` comment; wraps libsvm with internal struct redefinition (lines 49–67) due to `svm.h` not exporting the struct
- Files: `include/BALL/QSAR/libsvmModel.h`, `source/QSAR/libsvmModel.C`
- Impact: Fragile hack; libsvm API changes would break the wrapper; unclear intent of TODO
- Fix approach: Investigate libsvm version pin; document the reason for struct redefinition; consider modern ML library alternatives (scikit-learn Python bindings, TensorFlow, etc.)

**Deprecated OpenBabel 2.4:**
- Issue: ROADMAP notes OpenBabel 2.4 left OFF in build; Homebrew provides 3.1.1 with breaking API changes. `cmake/FindOpenBabel2.cmake` looks for 2.x API only.
- Files: `cmake/FindOpenBabel2.cmake`, BALL structure I/O that depends on OpenBabel
- Impact: Cannot use modern OpenBabel; structure I/O from many formats disabled if `BALL_HAS_OPENBABEL=ON` attempted
- Fix approach: Port to OpenBabel 3.x API; isolate behind `BALL_HAS_OPENBABEL`; document API changes. Phase 3 (ROADMAP).

**TBB (Intel Threading Building Blocks) Version Check Obsolete:**
- Issue: `cmake/FindTBB.cmake` predates oneTBB; `BALL_HAS_TBB` gate checks `< 2017`. Homebrew provides oneTBB 2023.0.0 which this old finder cannot locate.
- Files: `cmake/FindTBB.cmake`, any code guarded by `BALL_HAS_TBB`
- Impact: TBB disabled; parallelization unavailable; build doesn't see Homebrew TBB
- Fix approach: Replace with modern `find_package(TBB CONFIG)` and update version gate. Phase 3 (ROADMAP).

**Eigen Version Header Moved (5.0 API Break):**
- Issue: ROADMAP notes Eigen 5.0.1 moved version macros from `src/Core/util/Macros.h` to `Eigen/Version`. `cmake/FindEigen3.cmake` needs updating to check new location.
- Files: `cmake/FindEigen3.cmake`
- Impact: Eigen 5.0+ versions may not be detected; MATHS code that heavily uses Eigen may compile but produce incorrect results if version gates fail
- Fix approach: Update `FindEigen3.cmake` to read version from `Eigen/Version`; test Eigen 3.4 and 5.0 compatibility. Phase 3 (ROADMAP).

---

## Documentation & API Clarity Issues

**Bond Order Assignment Strategy Documentation Obsolete:**
- Issue: `include/BALL/STRUCTURE/BONDORDERS/FPTBondOrderStrategy.h` line 57: `//TODO: documentation is obsolete!` — class documentation refers to non-existent or incorrect behavior
- Files: `include/BALL/STRUCTURE/BONDORDERS/FPTBondOrderStrategy.h`
- Impact: Users cannot understand algorithm; unsafe to modify; unclear algorithm complexity / assumptions
- Fix approach: Audit algorithm against current implementation; rewrite documentation with correct examples and complexity analysis

**Molecular Graph Processing Stub:**
- Issue: `include/BALL/DATATYPE/GRAPH/molecularGraph.h` line 50: `///@TODO do something useful with this` — suggests incomplete feature
- Files: `include/BALL/DATATYPE/GRAPH/molecularGraph.h`
- Impact: Unclear what the incomplete component does; may cause silent failures if called
- Fix approach: Clarify what "useful" means; either complete the feature or document its limitations explicitly

**Bond Order Processor Missing Comments:**
- Issue: `include/BALL/STRUCTURE/assignBondOrderProcessor.h` lines 158, 309, 603 contain TODOs: `//TODO`, `// constructor with parameter filename //TODO`, `//TODO: move to solution!`
- Files: `include/BALL/STRUCTURE/assignBondOrderProcessor.h`
- Impact: Unclear what these features should do; brittle processor; hard to debug incorrect bond orders
- Fix approach: Complete the documentation; either implement or mark as abandoned

**Amino Acid Name Lookup Inefficient:**
- Issue: `include/BALL/STRUCTURE/peptides.h` line 269: `//TODO: unse a StringHashMap of amino acid names to simplify the access` (note typo: "unse" instead of "use")
- Files: `include/BALL/STRUCTURE/peptides.h`
- Impact: Linear lookup performance for every residue access; not a blocker but inefficient for large proteins
- Fix approach: Replace with `HashMap<String, AminoAcid*>` lookup table

---

## Testing & Verification Gaps

**Untested Rendering Paths:**
- What's not tested: GLRenderer correctness; threading interaction between `RenderSetup` and `GLRenderWindow`; all QOpenGL* integration; render-to-FBO path on macOS
- Files: `source/VIEW/RENDERING/glRenderer.C`, `source/VIEW/RENDERING/renderSetup.C`, `source/VIEW/RENDERING/glRenderWindow.C`, `source/VIEW/WIDGETS/scene.C`
- Risk: Visual glitches and blank scenes (confirmed macOS issue); race conditions undetected; Phase 4 rework will be high-risk without render-correctness tests
- Priority: **HIGH** — blocks shipping Phase 4

**Untested Networking Code:**
- What's not tested: `TCPServer`, `TCPServerThread`, `TCPIOStream`; connection handling; Boost.Asio integration
- Files: All `source/SYSTEM/networking.C` implementation; `include/BALL/SYSTEM/networking.h` interfaces
- Risk: Silent corruption on network errors; threading deadlocks; Boost version change breakage
- Priority: **MEDIUM** — blocks Phase 6 (networking rework)

**Untested Serialization Edge Cases:**
- What's not tested: Endianness round-trips for `regularData3D::binaryRead/Write()`; HashMap persistence format across size_t changes
- Files: `source/DATATYPE/regularData3D.C`, `source/DATATYPE/hashMap.C`
- Risk: Data corruption on cross-platform use; silent loss of precision in large grids
- Priority: **MEDIUM**

**Python Binding Tests Disabled:**
- What's not tested: Any Python interaction (module import, class instantiation, method calls)
- Files: All `.sip` files (237 files); `source/PYTHON/*`
- Risk: Python bindings entirely untested and non-functional
- Priority: **LOW** (Python support OFF in current build; Phase 5 concern)

---

## Platform-Specific Risks

**macOS OpenGL Deprecation:**
- Issue: Apple deprecated OpenGL in macOS 10.14, removed in Monterey for Metal-only graphics. Future macOS may remove GL entirely.
- Impact: BALLView's fixed-function GL renderer will stop working; cross-platform rendering code is non-portable
- Fix approach: Phase 4 uses GL 2.1 compat profile as bridge (works for now on macOS 14/15). Medium-term (Phase 5+): Migrate to OpenGL 3.3+ core profile with shaders, or adopt Metal backend (per-platform).

**macOS Bundle Packaging Missing:**
- Issue: ROADMAP notes `BALLView.app` does not embed data files; requires `BALL_DATA_PATH` env var at runtime
- Files: `source/SYSTEM/path.C`, `cmake/BALLViewInstallFix.cmake.in`
- Impact: Double-click launch from Finder fails with "Could not read FragmentDB" error; only usable from shell with env vars
- Fix approach: Phase 7 (ROADMAP): Copy `data/` into `BALLView.app/Contents/Resources`; add macOS fallback in `path.C` to check `../Resources/data` relative to bundle executable

**Windows/Vcpkg Build Path Unproven:**
- Issue: Dependency strategy is "Homebrew on macOS/Linux; vcpkg on Windows" but Windows CI/build never tested
- Files: No vcpkg.json or Windows CI configuration
- Impact: Windows build may be broken on arrival; unknown Homebrew→vcpkg package name mismatches
- Fix approach: Phase 8 (ROADMAP): Set up GitHub Actions Windows runner; build against vcpkg manifest; document any package name differences

---

## Known Limitations (Accepted in Roadmap)

**Components Intentionally Disabled (Not Bugs):**
- RTfact raytracer: Windows-only, off by design
- QtWebEngine plugins (PresentaBALL, BALLaxy, Jupyter): Requires Qt 5 WebEngine (not in Homebrew `qt@5`)
- VRPN / SpaceNavigator: Input device libraries not available
- LPSolve: Homebrew `lp_solve` not matched by `FindLPSolve.cmake`

These are not bugs; they're scoped out of 1.6.

---

*Concerns audit: 2026-05-14. Analysis informed by ROADMAP-1.6.md and direct code inspection.*
