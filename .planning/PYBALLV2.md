# PYBALLV2 — PyBALL Wrapping Rewrite Plan (Detailed, v2 — post-CODEX rework)

**Status:** Draft for roadmap insertion. **NOT promotion-ready** — bake-off must complete first.
**Authored:** 2026-05-16 (v2, after adversarial CODEX review of v1).
**Author:** Claude (Opus 4.7, 1M context) with hands-on POC validation + cross-AI review.

> **Note on v1 → v2:** v1 of this document declared "Decision committed: autowrap+Cython" and was rejected by adversarial CODEX review. v2 demotes the recommendation to "candidate pending bake-off" and restructures the plan to make the tool-choice empirical, not pre-decided. The bake-off lives in **Phase 6 (retained, not superseded)**; Phase 999.15 only fires after Phase 6 picks a tool.

---

## 1. Concise summary

PyBALL today is built on **SIP 4.9** with **237 hand-written `.sip` files** (171 in BALL, 66 in VIEW) and is **disabled in CI** (`"BALL_PYTHON_SUPPORT": "OFF"` in all three `ci-*` presets in [`CMakePresets.json`](../CMakePresets.json)). SIP 4 is upstream-deprecated. **73 of the 171 BALL .sip files (43%)** contain custom Python integration code (`%MethodCode`, `%MappedType`, `%ConvertToTypeCode`, `/Transfer/`, `SIP_PYOBJECT`, `%Exception`) — that is **not** "bulk declaration translation"; it is a large manual behavior port that any replacement toolchain must reproduce.

This plan proposes replacing SIP 4 in **two staged phases**:

1. **Phase 6 (kept active in v1.6)**: a real cross-platform bake-off comparing **autowrap+Cython 3** against **nanobind** (and optionally SIP 6 as an incumbent baseline) on the same vertical slice of BALL classes covering all the hard patterns (templates, ownership transfer, exceptions, iterators, Composite/processor callbacks, file I/O, NumPy interop). **Tool decision is the output of Phase 6, not its premise.**
2. **Phase 999.15 (targeted v2.1)**: the bulk wrap. Only fires after Phase 6 picks a tool AND the chosen tool passes the hard stop/pivot gate (see §10). If autowrap wins → execute the autowrap-specific plan below. If nanobind wins → execute the nanobind-specific variant of the plan (sketched in §11).

**Why this restructuring (the v1 → v2 change):** The honest counter-signal to autowrap is that **pyOpenMS — the canonical autowrap reference — is running a 350-hour GSoC 2026 project explicitly evaluating nanobind as a replacement**, citing "maintenance overhead, compile times, and certain Python integration features" with current autowrap+Cython (source: [openms.de/news/gsoc2026/](https://openms.de/news/gsoc2026/)). This is an *evaluation*, not a migration commitment — autowrap is still actively used and maintained — but the signal cannot be wished away. The right response is to do the same evaluation for BALL ourselves, on a real BALL slice, with the same instrumented criteria, before committing.

**Why "autowrap+Cython" is still my candidate going into the bake-off** (vs nanobind / pybind11 / SIP 6 / SWIG): codegen-first is a stated user requirement; autowrap+Cython is the only mature codegen-first option (SWIG's template handling is too fragile for BALL's QSAR/STRUCTURE; pybind11/nanobind/SIP 6 are all manual per-class). But "candidate" ≠ "decided" — the bake-off must confirm.

**Scope (when 999.15 fires):** replace the BALL-side wrapping (171 `.sip` files). VIEW-side wrapping (66 `.sip` files) is **deferred** to a post-v2.1 phase (Qt/PyQt6 interaction needs its own design pass; the BALLView GUI is the canonical "use VIEW from C++" surface).

**Output of Phase 999.15 (when promoted):** a `pip install pyball` + `conda install -c conda-forge pyball` deliverable that exposes BALL's KERNEL + FORMAT + STRUCTURE + MOLMEC + ENERGY + SOLVATION + NMR + QSAR + DOCKING + SCORING + MATHS + COMMON + DATATYPE C++ APIs in Python 3.10+, built green on macOS-arm64 / Linux-x64 / Windows-x64 in CI via **cibuildwheel** (one wheel per Python × OS), with **type stubs** (.pyi), with API parity tested against the previously SIP-exposed surface, and with the legacy SIP 4 toolchain removed.

## 2. Estimated effort (revised; v1 was understated)

**Planning envelope: 26-40 engineer-weeks (≈ 6-10 months) for one full-time engineer.** Branched by tool decision:

- **autowrap+Cython path: 25.5-32.5 weeks** (Phase 6 7.5w + Phase 999.15 18-25w)
- **nanobind path: 29-40 weeks** (Phase 6 7.5w + Phase 999.15 21.5-32.5w; ~30% larger because no codegen; see §11 for derivation)

Use 26-40 weeks as the single planning envelope until the bake-off (Phase 6) commits the path.

**v1's 10-14 weeks was demonstrably too low** — it ignored: (a) the 73 BALL .sip files with custom Python code that must be ported (not just translated), (b) the packaging story (cibuildwheel + scikit-build-core + conda-forge), (c) the API-parity test suite, (d) Windows wheel-tagging + DLL discovery for libBALL deps, (e) type-stub generation + IDE story, (f) downstream migration support (BALLAXY/PresentaBall/Galaxy/Jupyter), (g) profiling + performance regression gate.

Per task (refer to §6 for task definitions). The **bake-off tasks (P6-01..P6-04)** are estimated separately from the **bulk-wrap tasks (999.15-NN)**:

| Task | Scope | Effort | Phase | Critical path? |
|------|-------|--------|-------|----------------|
| P6-01 SIP behavior inventory | Audit 171 BALL .sip files; tag every `%MethodCode`, `%MappedType`, `%ConvertToTypeCode`, `/Transfer/`, `SIP_PYOBJECT`, `%Exception`. Output: a Python-API spec the new bindings must match. | 1.5 weeks | Phase 6 | yes |
| P6-02 Real vertical-slice POC (autowrap track) | Wrap Vector3 (TVector3<float> template), Atom (Composite+iterators+ownership), PDBFile (exceptions+file I/O), HBondProcessor (Composite/apply callback), RegularData3D (NumPy/buffer), one OWNED-pointer case from composite.sip, one HashMap<K,V> case (e.g., StringHashMap from `standardColorProcessor.h`). Tri-OS build matrix. | 3 weeks | Phase 6 | yes |
| P6-03 Real vertical-slice POC (nanobind track) | Same seven slice cases, nanobind. Comparable test surface. | 2.5 weeks | Phase 6 | yes (parallel with P6-02 if 2 engineers) |
| P6-04 Bake-off comparison + tool decision | Quantitative comparison: compile time, wheel size, runtime overhead, lines-of-binding-code per class, API-parity gap count, Windows pain points. Write decision doc. | 0.5 weeks | Phase 6 | yes |
| **Phase 6 subtotal** | | **7.5 weeks** | | |
| 999.15-01 Full SIP-behavior + downstream-consumer audit | Extend P6-01 audit to all 171 BALL .sip; enumerate BALLAXY/PresentaBall/Galaxy/Jupyter consumers; lock the API-parity target. | 1 week | 999.15 | yes |
| 999.15-02 CMake + scikit-build-core + cibuildwheel + tri-OS CI | Build integration; wheel matrix (Python 3.10..3.13 × macOS/Linux/Windows); CI green. | 2 weeks | 999.15 | yes |
| 999.15-03 Wrap KERNEL + FORMAT + COMMON + DATATYPE + MATHS | Bulk core wrapping including all custom-code ports (the 73 .sip files' worth of behavior) for these modules. | 4-5 weeks | 999.15 | yes |
| 999.15-04 Wrap STRUCTURE + MOLMEC + ENERGY + SOLVATION + NMR | Second-tier modules. STRUCTURE has the processor patterns. | 3-4 weeks | 999.15 | partial parallel with 03 |
| 999.15-05 Wrap QSAR + DOCKING + SCORING | Template-heavy; expect manual edge cases. | 3-4 weeks | 999.15 | sequential after 03 |
| 999.15-06 Exception parity | Reproduce BALL's specific exception class hierarchy on Python side (not the lossy `except +` default). | 1 week | 999.15 | yes |
| 999.15-07 Iterator + lifetime parity | Port the manual iteration patterns from `iterators.sip`; validate parent-object lifetime, end invalidation, returned-reference ownership. | 1-2 weeks | 999.15 | yes |
| 999.15-08 Ownership / `/Transfer/` parity | Port the 7 .sip files using `/Transfer/` (chain, composite, molecule, nucleotide, residue, secondaryStructure, system); choose autowrap-or-nanobind equivalent ownership semantics; validate Composite tree insert/remove. | 1-2 weeks | 999.15 | yes |
| 999.15-09 Performance benchmark + (optional) hot-path escape hatch | Profile against representative workloads; mandatory profiling gate before teardown. | 1-2 weeks | 999.15 | yes (mandatory) |
| 999.15-10 SIP teardown | Delete 171 BALL .sip files, FindSIP.cmake, helpers; CMakeLists cleanup. | 0.5 weeks | 999.15 | yes |
| 999.15-11 Type stubs (.pyi) generation + IDE story | Configure stub generation; ship in wheel; verify mypy/pyright/PyCharm autocomplete on stub-import. | 0.5-1 week | 999.15 | yes |
| 999.15-12 Docs + downstream migration | Migration guide; conda-forge feedstock PR; coordinate with BALLAXY/PresentaBall/Galaxy. | 1-2 weeks | 999.15 | yes |
| **Phase 999.15 subtotal** | | **18-25 weeks** | | |
| **GRAND TOTAL (Phase 6 + Phase 999.15)** | | **25.5-32.5 weeks** | | |

**Calendar duration:** 6-8 months single-engineer; ~3.5-5 months two engineers if P6-02 and P6-03 parallelize (and bulk-wrap modules split across engineers). Overrun risk concentrated in QSAR template edge cases + Windows packaging + ownership/lifetime parity.

## 3. POC validation status (honest)

**A self-contained autowrap POC at [`.planning/pyball-poc/poc1-simple/`](pyball-poc/poc1-simple/) (mirror of `../pyball-poc/poc1-simple/`)** validates that **the toolchain works end-to-end** on macOS-arm64/Python 3.12/Cython 3.2.4/autowrap 0.27.0 for a *trivial* class (`SimpleVec`: constructors, getters/setters, +/-/==/!= operators, throw-bearing methods). All 12 tests pass.

**The POC does NOT validate any of the hard cases that drive the tool decision:**
- ❌ Ownership transfer (`/Transfer/` in SIP)
- ❌ Polymorphic inheritance (Composite hierarchy)
- ❌ Template specialization (TVector3<T> → Vector3 typedef)
- ❌ Iterators with parent-object lifetime hazards
- ❌ Custom container types (BALL's `HashMap`, `List`, `StringHashMap`)
- ❌ Specific BALL exception class propagation (not just `RuntimeError`)
- ❌ Python-side override callbacks (UnaryProcessor inheritance)
- ❌ File I/O exception chains (PDBFile + ParseError)
- ❌ Windows packaging and wheel-tagging
- ❌ NumPy/buffer interop for `Vector3`, `Matrix3x3`, `RegularData3D`

**Therefore: the bake-off (Phase 6) is the real validation, not POC1.** POC1 only proves "autowrap can build at all on macOS" — necessary but far from sufficient.

### Six pitfalls discovered in POC1 (preserved for the bake-off team)

These are not speculative; they broke real builds:

| # | Pitfall | Symptom | Fix | Generalizes to BALL? |
|---|---------|---------|-----|---------------------|
| 1 | `operator*` must return the class type | `AssertionError: can only return same type for operator*` | Drop dot/scale operators from `operator*`; rename to `dot()`/`scale()` via addons | YES — BALL `Vector3` has dot-product and scalar-multiply both as `operator*` |
| 2 | `std::string` import name | `NameError: no converter for string` | `cimport string as libcpp_utf8_string` (Unicode) or `as libcpp_string` (bytes) | YES — pervasive |
| 3 | Reference types in operator/ctor args | Invalid `_T & * this` in generated Cython | Drop `&` from operator args; declare as value (Cython still passes by ref) | YES — every `Vector3` operator |
| 4 | Inline comments may be parsed as `wrap-*` annotations | `ValueError: Failed to parse line annotations` | **NUANCED (CODEX correction)**: Inline `# wrap-doc:` and other `# wrap-*` annotations ARE supported. The crash is specifically for `#` comments at end-of-line whose text doesn't match a `wrap-*` directive. Workaround: put non-annotation comments on their own line ABOVE the method | YES |
| 5 | `autowrap_tools.hpp` include dir | `fatal error: ... file not found` | Add `Path(autowrap.__file__).parent / "data_files" / "autowrap"` to `include_dirs` | YES |
| 6 | Int → double overload resolution | `Exception: can not handle type of (1, 2, 3)` | Callers must pass floats; OR custom converter | YES — BALL APIs take `float`/`double` extensively |

### Pitfalls FORECAST for the real BALL POC (not yet validated)

These come from reading BALL headers + .sip files, not from POC build attempts. Phase 6 must validate or refute each.

- **Template typedef** (HIGH likelihood): `Vector3 = TVector3<float>` at [vector3.h:1084](../include/BALL/MATHS/vector3.h). Need `# wrap-instances: Vector3 := TVector3[float]`.
- **Composite::apply trampoline** (HIGH): Python override of `UnaryProcessor::operator()` requires C++-side trampoline. autowrap's support here is limited; nanobind has explicit trampoline macros (`NB_TRAMPOLINE`).
- **Custom containers** (MEDIUM): BALL's `HashMap<K,V>` is not `std::unordered_map`. Cython's built-in `libcpp_unordered_map` converter doesn't apply. Need explicit declaration + (probably) custom Python ↔ HashMap converter.
- **Ownership transfer** (HIGH, gate-relevant): 7 BALL .sip files use `/Transfer/` annotations: [`chain.sip`](../source/PYTHON/EXTENSIONS/BALL/chain.sip), [`composite.sip`](../source/PYTHON/EXTENSIONS/BALL/composite.sip), [`molecule.sip`](../source/PYTHON/EXTENSIONS/BALL/molecule.sip), [`nucleotide.sip`](../source/PYTHON/EXTENSIONS/BALL/nucleotide.sip), [`residue.sip`](../source/PYTHON/EXTENSIONS/BALL/residue.sip), [`secondaryStructure.sip`](../source/PYTHON/EXTENSIONS/BALL/secondaryStructure.sip), [`system.sip`](../source/PYTHON/EXTENSIONS/BALL/system.sip) — all Composite tree insert/remove operations. Autowrap defaults will NOT match SIP semantics. Behavior port required.
- **Exception parity** (HIGH, gate-relevant): BALL has a rich `BALL::Exception::*` hierarchy. SIP exposes specific exception classes to Python. `except +` collapses everything to Python's generic `RuntimeError`/`ValueError`. **API regression if not addressed.**
- **Iterator parity** (HIGH, gate-relevant): BALL's iterators in [iterators.sip](../source/PYTHON/EXTENSIONS/BALL/iterators.sip) implement Python iteration manually by returning raw referenced objects and incrementing the C++ iterator — they do **not** guard against parent-object destruction, mutation during iteration, or end invalidation. The new bindings must reproduce the same iteration semantics AND should ideally improve on the lifetime safety (the SIP defaults are unsafe by design). autowrap's iterator support must be validated against mutation, parent-object lifetime, end invalidation.
- **Windows packaging** (HIGH likelihood, HIGH impact): MSVC + Python ABI + DLL discovery for libBALL/libVIEW + cibuildwheel + vcpkg interaction is the most likely source of multi-week overrun.

## 4. Cross-platform plan (revised; Windows risk reclassified to HIGH/HIGH)

| OS | Status | Known issue |
|----|---|---|
| macOS-arm64 | POC1 green on Python 3.12 + Cython 3.2.4 + autowrap 0.27.0. Brew-installed Python. | None for POC; for real bulk wrap, validate Apple Silicon wheel architecture (universal2 vs arm64-only). |
| Linux-x64 | Untested in POC. Expected to work. apt-installed Python 3.12 + pip-installed Cython + autowrap. | None expected. Validate at P6-02 end. |
| Windows-x64 | **UNTESTED, HIGH RISK.** | Cython-on-MSVC has historical thorns: (a) Python debug/release ABI mismatch on debug builds; (b) MSVC runtime version drift; (c) DLL discovery — libBALL.dll + libVIEW.dll + Boost + Qt + Eigen + lp_solve + libsvm + zlib must all be reachable from the loaded Python extension at import time; (d) wheel tagging (which Python ABI tag, which platform tag — `win_amd64` vs `pyXY-none-win_amd64`); (e) cibuildwheel + scikit-build-core interaction with the existing CMake-based build; (f) vcpkg vs conda-forge divergence for downstream packages; (g) generated C++ compile time on Windows is notoriously worse than POSIX. **Mitigation:** validate POC2 on Windows in week 1 of Phase 6, before any bulk-wrap decision. **If autowrap/Cython does not build on Windows in Phase 6, the tool is rejected.** |

## 5. Package architecture (NEW — was missing from v1)

| Concern | Decision (proposed; finalize in P6-04) |
|---------|----------------------------------------|
| Build orchestration | **scikit-build-core** ([scikit-build-core docs](https://scikit-build-core.readthedocs.io)) bridges CMake to PEP 517 (`pyproject.toml`). Lets us keep BALL's CMake setup as source of truth while producing `pip install`-able wheels. |
| Wheel building (CI) | **cibuildwheel** ([cibuildwheel docs](https://cibuildwheel.readthedocs.io)) — produces wheels for Python 3.10..3.13 × macOS-arm64 / macOS-x86_64 / linux-x86_64 / linux-aarch64 / windows-amd64. |
| Distribution channels | (a) **PyPI** wheels via cibuildwheel; (b) **conda-forge** via the existing `ball` feedstock (separate PR coordinated with maintainer). |
| Wheel contents | The compiled extension module(s) + bundled `libBALL.{dylib,so,dll}` + dependent libs (Boost, FFTW, Qt6 runtime via `libBALL`'s indirect chain) via `delocate-wheel` (macOS), `auditwheel` (Linux), `delvewheel` (Windows). **`libVIEW` is NOT bundled** — VIEW is unwrapped (deferred to post-v2.1) AND `libBALL` does not depend on `libVIEW` (verified via `otool -L libBALL.dylib`: only Boost, Qt6, FFTW, libSystem). Out-of-process resources (`data/`) bundled via `package_data`. |
| Type stubs | `.pyi` files. autowrap can generate stubs; nanobind has built-in stub generator. Stubs ship inside the wheel under `pyball/py.typed` + per-module `.pyi`. |
| Generated-file policy | **Regenerate at build time** (not committed to repo). Caching via CMake `add_custom_command` DEPENDS list (re-trigger on .pxd / .h changes). Reproducibility via pinned `autowrap==X.Y.Z` + `cython==X.Y.Z` in `pyproject.toml`. |
| Versioning | Follow BALL's `CMAKE_PROJECT_VERSION` (currently 1.6.x). Wheel version = `pyball-1.6.0`. |

## 6. Detailed phase plan (revised; bake-off first, then bulk wrap)

### Phase 6 (retained, in v1.6 active list): the bake-off

**Goal:** Empirically pick the binding generator for v2.1's bulk wrap. **The tool decision is the deliverable, not a premise.**

**P6-01 SIP behavior inventory** (1.5 weeks)
- Inventory 171 BALL .sip files.
- Tag every `%MethodCode`, `%MappedType`, `%ConvertToTypeCode`, `/Transfer/`, `SIP_PYOBJECT`, `%Exception`, `__str__`, `__repr__`, `__iter__`, `__getitem__`, `__setitem__`, ownership policy directive.
- Output: `.planning/phases/06-python-bindings/01-SIP-BEHAVIOR-AUDIT.md` — a Python-API specification that the new bindings MUST match. Cross-tagged with sample old-PyBALL example scripts (from BALLAXY / PresentaBall / wiki / tutorials) that exercise the API surface.
- **Success:** every custom-code-bearing .sip file has its Python-visible behavior documented as testable claims.

**P6-02 Real vertical-slice POC — autowrap track** (3 weeks)
- Implement these **seven slice cases**: (1) `Vector3` (TVector3<float> template), (2) `Atom` (Composite+iterators+ownership), (3) `PDBFile` (exceptions+file I/O), (4) `HBondProcessor` (Composite/apply Python-override), (5) `RegularData3D` (NumPy/buffer interop), (6) one OWNED-pointer case from `composite.sip` (e.g., `Composite::insertAfter`), (7) one custom mapped container — `StringHashMap<ColorRGBA>` from `standardColorProcessor.h`, which has no Cython built-in converter.
- Build green on **macOS-arm64 AND linux-x64 AND windows-x64**. Wheel built via `scikit-build-core` + `cibuildwheel`. Type stubs generated.
- Test against P6-01's behavior spec: read a real PDB, iterate atoms, compute H-bonds, modify a Composite tree (ownership), trigger a parse exception, override a UnaryProcessor in Python, round-trip a `StringHashMap` through Python dict.
- **Success:** every test from P6-01 passes for the seven slice cases; Windows wheel imports cleanly.

**P6-03 Real vertical-slice POC — nanobind track** (2.5 weeks; PARALLEL with P6-02 if 2 engineers)
- Same seven slice cases, nanobind. Same test surface. Hand-written bindings (nanobind doesn't have codegen).
- Build green on all 3 OSes via the same cibuildwheel setup.
- **Success:** same as P6-02.

**P6-04 Bake-off comparison + tool decision** (0.5 weeks)
- Quantitative comparison table: (a) compile time per class; (b) wheel size; (c) runtime overhead microbenchmark (per-call + per-method-dispatch); (d) lines-of-binding-code per class; (e) API-parity gap count vs P6-01 spec; (f) Windows-specific pain points encountered; (g) maintainer-time-spent-per-class.
- Output: `.planning/phases/06-python-bindings/04-BAKE-OFF-DECISION.md` with the decision and verbatim numbers.
- **Hard stop/pivot gates** (any one trips → rejection of the tool):
  - Windows wheel doesn't build OR can't be imported in a clean Windows env
  - API-parity gap > 5% on Vector3 + Atom + PDBFile + HBondProcessor surface
  - Compile time > 30 min for the 7-case POC on a 4-core CI runner
  - Iterator parity fails (parent-object segfault or end-invalidation hazard)
  - Ownership/`/Transfer/` semantics cannot be faithfully reproduced

**Phase 6 total: 7.5 weeks.** Output: a tool decision + a working seven-case slice proof on tri-OS.

### Phase 999.15 (v2.1, conditional on Phase 6 success): the bulk wrap

Only fires if Phase 6 passes hard stop/pivot gates. If autowrap+Cython wins, this plan applies as-is. If nanobind wins, see §11 for the variant.

**Tasks 999.15-01..12: see effort table in §2.** Detailed descriptions:

- **999.15-01 Full SIP-behavior + downstream-consumer audit** (1 wk): extend P6-01 across all 171 BALL .sip files; enumerate BALLAXY/PresentaBall/Galaxy/Jupyter consumers; lock API-parity target.
- **999.15-02 CMake + scikit-build-core + cibuildwheel + tri-OS CI** (2 wk): build integration; wheel matrix; CI green for the empty/POC build. **Windows-specific subtasks (all required):** (a) decide vcpkg vs conda-forge for runtime dependency provenance (recommend vcpkg for wheel builds since cibuildwheel runs on GitHub-hosted runners; conda-forge feedstock is a separate distribution channel handled in 999.15-12); (b) DLL search path policy — `os.add_dll_directory()` (introduced in Python 3.8) is the standard mechanism for telling Windows where to find sibling-bundled DLLs at extension import time; we target Python 3.10+ so this is the supported path. Call it from the package `__init__.py` pointing at the wheel's bundled `pyball.libs/` (or equivalent) directory before importing the compiled extension; (c) `delvewheel repair` validation — every Windows wheel passes `delvewheel show` audit before upload; (d) **clean-VM import test** as a CI gate — a fresh Windows runner with only `pip install pyball-*.whl` (no BALL source, no MSVC) must import and exercise the smoke tests.
- **999.15-03 KERNEL + FORMAT + COMMON + DATATYPE + MATHS bulk wrap** (4-5 wk): core modules with custom-code ports.
- **999.15-04 STRUCTURE + MOLMEC + ENERGY + SOLVATION + NMR** (3-4 wk).
- **999.15-05 QSAR + DOCKING + SCORING** (3-4 wk; template-heavy; expect 1-week overruns).
- **999.15-06 Exception parity** (1 wk): reproduce BALL's specific exception hierarchy on Python side (not the lossy `except +` default). **MANDATORY**. Test matrix MUST include at minimum: (1) base `BALL::Exception::GeneralException` → Python class that subclasses Python `Exception`; (2) bounds-style error — e.g., `IndexOverflow` from `Atom::getBond(Position)`; (3) file error — `FileNotFound` from `PDBFile` constructor; (4) parser error — `ParseError` from `PDBFile::read`; (5) one domain-specific nested name — e.g., `MolecularFileDialog::TooManyMolecules` or `Exception::NotInitialized`. See [`exception.sip`](../source/PYTHON/EXTENSIONS/BALL/exception.sip) (346 lines, defines the full SIP-exposed Python exception class set) for the parity target.
- **999.15-07 Iterator + lifetime parity** (1-2 wk): port `iterators.sip` patterns. **MANDATORY**.
- **999.15-08 Ownership/`/Transfer/` parity** (1-2 wk): port the 7 `/Transfer/`-bearing .sip files — `chain.sip`, `composite.sip`, `molecule.sip`, `nucleotide.sip`, `residue.sip`, `secondaryStructure.sip`, `system.sip` — ownership semantics. **MANDATORY**.
- **999.15-09 Performance benchmark + (optional) hot-path escape hatch** (1-2 wk): **MANDATORY profiling gate** (CODEX promotion from v1 optional). If chosen tool's wrapping overhead exceeds the bake-off baseline by >2× on identified hot paths, re-wrap those classes with the alternate tool (mixed extension modules; transparent to Python users).
- **999.15-10 SIP teardown** (0.5 wk).
- **999.15-11 Type stubs (.pyi) + IDE story** (0.5-1 wk).
- **999.15-12 Docs + downstream migration** (1-2 wk): migration guide; conda-forge feedstock PR; deprecation aliases for any renamed APIs; coordinate with BALLAXY/PresentaBall/Galaxy/Jupyter.

## 7. Where this slots in the existing roadmap (revised)

- **Phase 6 (Python Bindings) — RETAINED.** Originally scoped as "decide the generator via a vertical slice." v2 confirms that scope and makes it the bake-off described in §6. **Do NOT delete Phase 6 from the active phase list.**
- **Phase 999.15 (PyBALL wrapping rewrite — bulk) — CONDITIONAL.** Only fires if Phase 6 passes the hard stop/pivot gates. Target milestone v2.1.

Dependency on other phases:
- **Hard prerequisite:** Phase 999.10 (REST API + PyBALL SDK reshape) reshapes how Python interacts with BALL; affects whether `pyInterpreter.{h,C}` survives, and changes the C++ API surface that needs wrapping. 999.15 cannot finalize until 999.10 lands. (Phase 6 can run before 999.10 lands — the bake-off is forward-compatible.)
- **Soft prerequisites:** Phase 999.2 (Ninja) helps Cython-on-Windows compile times. Phase 999.13 (Read the Docs) provides docs home for 999.15-12.

## 8. Risk register (recalibrated per CODEX)

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|-----------|
| **Cython-on-Windows packaging failure** (cibuildwheel + scikit-build-core + DLL discovery) | **HIGH** (was Medium in v1) | **HIGH** (was Medium) | Validate POC2 on Windows in Phase 6 week 1. **Hard stop:** if Windows wheel won't build/import, autowrap is rejected. nanobind track is parallel insurance. |
| **Custom .sip behavior port (73 files) blows up effort** | HIGH | HIGH | Audit-first (P6-01); budget 25-50% of bulk-wrap effort to behavior porting (already reflected in 26-40 week planning envelope from §2). |
| **Composite/processor callback (Python override) cannot be made faithful in autowrap** | HIGH | MEDIUM | P6-02 explicitly tests HBondProcessor with Python-side override. If it fails, falls to nanobind track at decision time. |
| **Ownership/`/Transfer/` parity** | HIGH | HIGH | Explicit task 999.15-08; hard-stop gate in P6-04. |
| **Exception parity** (`except +` too lossy) | HIGH | MEDIUM | Explicit task 999.15-06; hard-stop gate in P6-04. |
| **Iterator parity** | HIGH | MEDIUM | Explicit task 999.15-07; hard-stop gate in P6-04. |
| **QSAR/STRUCTURE template edge cases overrun** | HIGH | MEDIUM | 999.15-05 budgets 3-4 weeks with 1-week overrun acceptance. nanobind escape hatch (999.15-09) for the worst offenders. |
| **pyOpenMS GSoC evaluation lands a public migration to nanobind** | LOW-MEDIUM | MEDIUM-HIGH | Track GSoC outcome (results typically published Sept). If nanobind wins the OpenMS bake-off and a migration begins, re-evaluate BALL's tool choice. `.pxd` files survive as a *partial* (declarations only) artifact; manual addons + ownership policy + Pythonic methods + trampolines do NOT (CODEX correction). |
| **BALLAXY / PresentaBall ABI-dependent breakage** | LOW (audit risk) | HIGH | 999.15-01 audit task catches; coordinate joint upgrade; ship deprecation aliases. |
| **conda-forge feedstock maintainer churn** | LOW | MEDIUM | Open feedstock PR early in 999.15-12; engage maintainer before tag. |
| **autowrap project stagnates** | LOW (still active Jan 2026) | MEDIUM | Track release cadence; trigger re-evaluation if no release in 18 months. |

## 9. CODEX TIER-3 items addressed

- **Scope to "previously SIP-exposed" not "all C++ public API"**: adopted in §6 (P6-01 + 999.15-01 use the SIP audit as the parity target).
- **Acceptance tests from old PyBALL examples**: adopted in P6-01 (sample scripts from BALLAXY / PresentaBall / wiki / tutorials).
- **Type stub story**: explicit task 999.15-11. Stubs ship in wheel; mypy/pyright/PyCharm autocomplete validated.
- **NumPy/buffer strategy**: P6-02 includes `RegularData3D` as a NumPy-interop test case. Decision point in P6-04: do we expose Vector3/Matrix3x3/Vector4 as buffer-protocol-aware for free numpy interop, or only via explicit conversion?
- **Import-time + wheel-size budgets**: target import time ≤ 300ms on a 4-core dev machine; wheel size ≤ 50MB (BALL itself dominates; bindings should add <10MB).
- **Release migration support**: explicit task 999.15-12; deprecation aliases for any renamed APIs; changelog; old/new import compat (`import BALLCore` continues to work).
- **Hard stop/pivot gate**: built into P6-04.

## 10. Hard stop/pivot gate (after Phase 6)

Before promoting Phase 999.15, the chosen tool MUST pass ALL of the following:

1. **Tri-OS wheel build green.** macOS-arm64 + linux-x64 + windows-x64 produce importable wheels in CI.
2. **All seven P6-02 slice cases pass behavior spec.** Vector3 (TVector3<float> template), Atom (Composite+iterators+ownership), PDBFile (exceptions+file I/O), HBondProcessor (Composite/apply Python-override), RegularData3D (NumPy/buffer), the OWNED-pointer Composite case, and the `StringHashMap<ColorRGBA>` mapped-container case.
3. **Exception parity demonstrated** on the same five-case matrix as 999.15-06 (base `GeneralException`, one bounds error, one file error, one parser error, one domain-specific nested name) — testing only `PDBFile`/`ParseError` is insufficient.
4. **Iterator parity demonstrated** on Atom iteration with parent-System lifetime.
5. **Ownership parity demonstrated** on Composite insert/remove with `/Transfer/`-equivalent semantics.
6. **Performance microbenchmark** within 2× of the SIP 4 baseline on identified hot paths OR a working alternate-tool implementation of the hot path exists and passes the same API tests. (Documentation of a planned fix is NOT acceptable — only working code counts.)
7. **Wheel import time** ≤ 500ms on the slowest CI runner.

**If ANY gate fails, the tool is rejected.** Default fallback: re-run P6-02 with the next-best tool (autowrap → nanobind, or vice versa) within 4 weeks. If both fail on Windows, escalate to maintainer for SIP-6-as-incumbent-baseline decision (last resort; doesn't solve the codegen-first goal).

## 11. Variant plan if nanobind wins the bake-off

If P6-04 picks **nanobind** instead of autowrap+Cython, the Phase 999.15 plan changes as follows.

### 11.1 File/module inventory (sizing unit, not just "171 classes")

nanobind bindings live in `bind_<module>.cpp` per the pyOpenMS pattern (one file per BALL module). For BALL's 13 wrapped modules that produces approximately 13 binding source files:

| Module | Approx classes to wrap | Source file |
|--------|----------------------|-------------|
| KERNEL | ~30 (System/Molecule/Residue/Chain/Atom/Bond/PDBAtom/PTE_/Composite/…) | `bind_kernel.cpp` |
| FORMAT | ~20 (PDBFile/MOL2File/HINFile/KCFFile/NMRStarFile/DCDFile/INIFile/…) | `bind_format.cpp` |
| COMMON | ~5 (Exception hierarchy, Logger, …) | `bind_common.cpp` |
| DATATYPE | ~10 (String, HashMap, List, BitVector, Quaternion, …) | `bind_datatype.cpp` |
| MATHS | ~15 (Vector3, Matrix3x3, Vector4, Angle, Box3, Line3, …) | `bind_maths.cpp` |
| STRUCTURE | ~25 (HBondProcessor, SmartsParser, BindingPocketProcessor, FragmentDB, RotamerLibrary, …) | `bind_structure.cpp` |
| MOLMEC | ~20 (AMBER, MMFF94, CHARMM force fields, GAFF, …) | `bind_molmec.cpp` |
| ENERGY | ~10 (energy components) | `bind_energy.cpp` |
| SOLVATION | ~6 (PB, GB, generalized Born variants) | `bind_solvation.cpp` |
| NMR | ~8 (NMRDescriptors, ShiftModule, …) | `bind_nmr.cpp` |
| QSAR | ~25 (descriptors, models, regression, kernels) | `bind_qsar.cpp` |
| DOCKING | ~10 (DockingAlgorithm, GeneticAlgorithm, IMGDock, …) | `bind_docking.cpp` |
| SCORING | ~8 (ScoreGridSet, MM-PBSA, …) | `bind_scoring.cpp` |
| **Total** | **~192 nanobind-exposed classes** | **13 binding files** |

(The 192 estimate is higher than 171 because the SIP wrap was selective; nanobind would target the previously SIP-exposed surface plus the additional classes the bake-off P6-02 prove are needed. Final target locked in 999.15-01.)

### 11.2 Reusable nanobind templates (write once per family, use 100+ times)

To make handwriting tractable, the bake-off (P6-03) MUST produce these reusable templates:

- **Ownership policy template** — a header `bind_helpers/ownership.hpp` with:
  - `nb::rv_policy::take_ownership` shorthand for `/Transfer/`-equivalent Composite tree insertions
  - `nb::rv_policy::reference_internal` for parent-owned references (default for getters returning `Type&`)
  - `nb::rv_policy::copy` for value returns
  - Documented per-class mapping table from SIP `/Transfer/` → nanobind `rv_policy::*`
- **Exception translator template** — `bind_helpers/exceptions.hpp` with `nb::register_exception_translator` for each BALL exception class, plus a catch-all that maps `BALL::Exception::GeneralException` → `pybindings.BALLError`. Output: one entry per [`exception.sip`](../source/PYTHON/EXTENSIONS/BALL/exception.sip) line ~50-340.
- **Iterator helper pattern** — `bind_helpers/iterators.hpp` using `nb::make_iterator` with `nb::keep_alive<0, 1>` to keep the parent alive while iterating. Generic over BALL's `AtomIterator`, `ResidueIterator`, `BondIterator`, etc.
- **Trampoline pattern for processors** — `bind_helpers/trampolines.hpp` with `NB_TRAMPOLINE` macros for the `UnaryProcessor<T>` family (one per common T: `Atom`, `Residue`, `Bond`, `Composite`).
- **HashMap converter pattern** — `bind_helpers/hashmap.hpp` with a generic `bind_hashmap<K, V>(m, name)` that exposes BALL's `HashMap<K, V>` as a Python `dict`-like type with `__getitem__`/`__setitem__`/`__iter__`/`__len__`/`__contains__`. Replicated for `StringHashMap<V>`.
- **NumPy interop pattern** — `bind_helpers/numpy.hpp` with buffer-protocol descriptors for `Vector3`, `Matrix3x3`, `RegularData3D`, `Box3`.

### 11.3 Per-task delta vs autowrap track

- **999.15-03/04/05 bulk-wrap modules:** hand-written `bind_<module>.cpp` files using the §11.2 templates. Pace: with templates done, ~5-8 classes/day sustainable on the bulk. ~192 classes / 6/day ≈ 32 days = **6.5 weeks of pure typing** spread across the three tasks. The remainder of the time goes to per-class edge cases (custom-code ports, ownership decisions, iteration patterns).
- **999.15-06 Exception parity:** *easier* than autowrap — `nb::register_exception_translator` is one line per exception. Estimated 0.5 weeks instead of 1.
- **999.15-07 Iterator parity:** *easier* — `nb::make_iterator` + `nb::keep_alive` covers most cases out of the box. Estimated 0.5-1 week instead of 1-2.
- **999.15-08 Ownership/`/Transfer/` parity:** comparable effort — `nb::rv_policy::*` explicit per-method tagging is straightforward but per-method-tedious. Estimated 1-2 weeks (unchanged).
- **999.15-09 performance:** lower wrapping overhead vs autowrap baseline (the whole point of nanobind); the hot-path escape hatch is rarely needed. Estimated 0.5-1 week instead of 1-2.
- **999.15-11 type stubs:** *easier* — nanobind has built-in `nb::stubgen`. Estimated 0.3 weeks instead of 0.5-1.

### 11.4 Effort estimate basis (nanobind path)

| Task | Effort | Note |
|------|--------|------|
| P6 bake-off | 7.5 wk | unchanged |
| 999.15-01 audit | 1 wk | unchanged |
| 999.15-02 packaging + CI | 2 wk | unchanged (nanobind makes Windows DLL story slightly easier — header-only — but the rest of the packaging work is the same) |
| 999.15-03/04/05 bulk wrap | **9-12 wk** (vs autowrap's 10-13 wk) | hand-writing offset by easier per-class effort with templates |
| 999.15-06 exception parity | 0.5 wk | easier |
| 999.15-07 iterator parity | 0.5-1 wk | easier |
| 999.15-08 ownership parity | 1-2 wk | unchanged |
| 999.15-09 performance | 0.5-1 wk | easier |
| 999.15-10 SIP teardown | 0.5 wk | unchanged |
| 999.15-11 type stubs | 0.3 wk | easier (built-in stubgen) |
| 999.15-12 docs + migration | 1-2 wk | unchanged |
| **Total** | **22.8-30.3 wk core + 7.5 wk bake-off = 30.3-37.8 wk** | **rounded: 29-40 weeks** |

The nanobind path comes out within 5 weeks of the autowrap path. The cost difference is dominated by hand-writing-vs-codegen for the bulk modules; the parity tasks are *cheaper* under nanobind because the primitives are better-fit.

### 11.5 Risk profile delta

- **Windows packaging easier** (nanobind is header-only; no Cython compile path on MSVC).
- **Long-term ecosystem alignment stronger** (pyOpenMS GSoC eval direction; nanobind picked up by FEniCS/dolfinx, MLIR, pyopencl, ITK eval).
- **Loss of codegen-first goal** is the cost — `.pxd` files-as-spec is gone; the source of truth is the `bind_<module>.cpp` files themselves.
- **Smaller binaries, faster import time** (nanobind's stated advantages: 3-5× smaller binaries, 4× faster compile vs pybind11; comparable to or better than autowrap+Cython for these metrics).

## 12. POC artifacts

The validation POC lives at [`.planning/pyball-poc/poc1-simple/`](pyball-poc/poc1-simple/) (mirror of `../pyball-poc/poc1-simple/`):

- `simple_vec.{h,cpp}` — self-contained C++ class
- `SimpleVec.pxd` — annotated autowrap PXD with inline documentation of pitfalls #1-#6
- `setup.py` — build driver (runs `autowrap.Main.run` → `cythonize` → builds `.so`)
- `test_poc.py` — Python smoke test (12 cases, all pass)
- `simple_vec_autowrap.pyx.generated` — sample autowrap output (committed for reference)

Run locally:
```bash
cd /Users/kohlbach/Claude/BALL/pyball-poc/poc1-simple
source ../venv/bin/activate    # Python 3.12 + Cython 3.2.4 + autowrap 0.27.0
rm -f simple_vec_autowrap.pyx simple_vec_autowrap*.cpp && rm -rf build
python setup.py build_ext --inplace
python test_poc.py             # expect "ALL POC1 TESTS PASSED"
```

**POC1 proves:** autowrap toolchain works on macOS for trivial cases.
**POC1 does NOT prove:** any of the hard things that drive the tool decision (see §3).

The bake-off (Phase 6) produces POC2 (real BALL classes) on all three OSes. That is the real evidence.

## 13. Sources

- [nanobind: Why another binding library?](https://nanobind.readthedocs.io/en/latest/why.html)
- [nanobind benchmarks](https://nanobind.readthedocs.io/en/latest/benchmark.html)
- [OpenMS/autowrap GitHub](https://github.com/OpenMS/autowrap) — current state (0.27.0 released 2026-01-14)
- [OpenMS GSoC 2026 — pyOpenMS nanobind evaluation project](https://openms.de/news/gsoc2026/) — 350h research project, not a migration commitment; cites "maintenance overhead, compile times, and certain Python integration features" as motivations
- [pyOpenMS 3.5.0dev latest docs (nanobind)](https://pyopenms.readthedocs.io/en/latest/community/wrapping_workflows_new_classes.html) — describes the nanobind workflow being evaluated
- [pyOpenMS 2.5.0 docs (autowrap, historical)](https://pyopenms.readthedocs.io/en/release_2.5.0/wrap_classes.html)
- [SIP release notes](https://python-sip.readthedocs.io/en/stable/releases.html)
- [scikit-build-core docs](https://scikit-build-core.readthedocs.io)
- [cibuildwheel docs](https://cibuildwheel.readthedocs.io)
- BALL repo, particularly: [`composite.sip`](../source/PYTHON/EXTENSIONS/BALL/composite.sip), [`molecule.sip`](../source/PYTHON/EXTENSIONS/BALL/molecule.sip), [`system.sip`](../source/PYTHON/EXTENSIONS/BALL/system.sip), [`iterators.sip`](../source/PYTHON/EXTENSIONS/BALL/iterators.sip), [`exception.sip`](../source/PYTHON/EXTENSIONS/BALL/exception.sip), [`pyAtomDict.sip`](../source/PYTHON/EXTENSIONS/BALL/pyAtomDict.sip) — examples of custom-code-heavy SIP wrappers
- Hands-on POC at [`.planning/pyball-poc/poc1-simple/`](pyball-poc/poc1-simple/) — 6 pitfalls discovered through actual build attempts; CODEX-corrected pitfall #4 wording
- Adversarial CODEX review of v1 (REJECT-AND-REWORK) — preserved at `/tmp/codex_review_output.txt` during session; key findings incorporated into v2

## 14. Changelog

- **v1 (2026-05-16)**: initial draft. "Decision committed: autowrap+Cython." REJECTED by CODEX.
- **v2 (2026-05-16)**: post-CODEX rework. Demoted decision to "candidate pending bake-off"; restructured plan into Phase 6 (bake-off) + Phase 999.15 (conditional bulk wrap); fixed inventory numbers (237/171/66, was 244/174/70); raised effort estimate to 24-40 weeks (was 10-14); added missing tasks (SIP behavior port, exception/iterator/ownership parity, type stubs, package architecture, downstream migration); Windows risk → HIGH/HIGH; added hard stop/pivot gate; added nanobind-wins variant plan (§11); promoted performance benchmark from optional to mandatory; corrected pitfall #4 wording per CODEX. **CODEX verdict: APPROVE-WITH-FIXES** (up from REJECT-AND-REWORK).
- **v3 (2026-05-16)**: applied CODEX v2 must-fixes. Effort range consolidated to "autowrap path: 25.5-32.5w; nanobind path: 29-40w; planning envelope: 26-40w" (was inconsistent 24-40 vs 25.5-32.5 vs 29-40); custom-code parenthetical now includes `%Exception` (matches the 73 number); `/Transfer/` file count corrected 10→7 with file list; P6-02 explicitly enumerates seven slice cases including `StringHashMap`; hard gate 6 tightened — "documentation alone" loophole removed (working code only); 999.15-06 exception parity expanded to mandatory test matrix (5 specific exception types); 999.15-02 expanded with Windows-specific subtasks (vcpkg vs conda-forge decision, DLL search path policy, delvewheel validation, clean-VM import test); §11 nanobind variant expanded with concrete file/module inventory (~192 classes / 13 binding files), reusable template list (ownership, exception, iterator, trampoline, hashmap, numpy), per-task delta vs autowrap, explicit effort derivation; libVIEW removed from wheel contents (verified `libBALL.dylib` has no libVIEW dependency); iterator wording corrected (SIP doesn't guard lifetime); "six classes" → "seven slice cases". **CODEX v2→v3 verdict: APPROVE-WITH-FIXES** (down from REJECT-AND-REWORK on v1).
- **v4 (2026-05-16)**: applied CODEX v3 cleanup fixes. Stale "24-40 week" reference in risk table updated to "26-40 week planning envelope"; stale "10 .sip files" in 999.15-08 task corrected to 7 (with file list); stale "six classes/P6-02 classes" in P6-03 row, hard-gate #2, Phase 6 summary, and compile-time gate all updated to "seven slice cases"; hard gate #3 (exception parity) expanded to require the same 5-case matrix as 999.15-06 (not just PDBFile/ParseError); `os.add_dll_directory()` wording fixed (was incorrectly described as "pre-Python-3.8 fallback" — it was *introduced* in 3.8 and is the correct path on 3.10+). **CODEX v3→v4: pending final review.**
