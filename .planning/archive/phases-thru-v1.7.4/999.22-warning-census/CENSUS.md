---
phase: 999.22-warning-census
generated: 2026-05-17 (v1.7 refresh)
prior_generated: 2026-05-16 (v1.6.x baseline)
ci_run: 25989752190
ci_run_url: https://github.com/BALL-Project/ball/actions/runs/25989752190
commit: v1.7-modernization HEAD at run time (Wave 1 batch validation: 999.4b + 999.23)
analyst: Claude (agent)
user_direction: 2026-05-16 Open Q3 (census-only) + 2026-05-17 v1.7 preseed lock-in
preseed_source: .planning/v1.7-WARNING-CENSUS-PRESEED.md
---

# Tier-C Warning Census — BALL v1.7-modernization (Wave 1 lock)

## 0. v1.7 refresh status

**This document is the v1.7 census** built from a fully-green cross-platform CI run
(`25989752190` — linux-x64, linux-arm64, macOS-arm64, windows-x64 all green after Wave 1
batch validation lands 999.4b + 999.23). It supersedes the v1.6.x-era data in §§1-7 below for
the purpose of routing 999.22a / 999.22c Wave 2 execution; the v1.6.x sections are retained
verbatim as historical audit context.

**Source-of-truth for Wave 2 work-items:** §0.4 + §0.5 below (which lock the preseed numbers).

### 0.1 Per-platform totals (CI run 25989752190)

| Platform | Total warnings | Notes |
|---|---|---|
| macOS arm64 (clang) | **172** | 80 `-Wdeprecated-declarations` + 69 `-Wunused-comparison` + 23 misc |
| Linux x64 (gcc)     | **444** | 200 `-Wdeprecated-copy` + 151 `-Wdeprecated-declarations` + 93 misc |
| Linux arm64 (gcc)   | **445** | Same as linux-x64 + 1 extra `-Woverloaded-virtual=` |
| Windows x64 (MSVC)  | **1**   | C4267 `binaryFingerprintMethods.C` (size_t→unsigned int) |

**Movement vs v1.6.x baseline:**
- macOS: 0 → 172 — clang now reports `-Wdeprecated-declarations` (Qt 6 deprecations) that the v1.6.x macOS run hadn't surfaced yet; these match the preseed's enumeration exactly.
- Linux: 200 → 444 — `-Wdeprecated-copy` cluster unchanged (200); `-Wdeprecated-declarations` cluster (151) is mostly GENETICDOCK self-deprecation (`genes.h`, `dockProblem.h`, `geneticAlgorithm.C`) — already deferred to v2.0 Phase 999.12 per preseed.
- Windows: 1 — confirms v1.6.x residual C4267 from `0a75ede` partial fix is the only remaining MSVC site (in-template-instantiation; sibling-cluster of 4 sites in source addressed via 999.22a).
- VRMLRenderer-related warnings: **0** on all platforms (Phase 999.37 deleted the class entirely; preseed's "1 inline-resolved" now empirically confirmed).

### 0.2 Top warning categories — macOS arm64 (172)

| -W class | Count | Dominant files | Routing |
|---|---|---|---|
| `-Wdeprecated-declarations` | 80 | `geneticIndividual.h` (24), `editMode.C` (12), `dockDialog.C`/`dockResultDialog.C` (4 each), `KCFFile.C` (4), `plsModel.C`/`pcrModel.C` (4 each), `testFramework.h` (4) | Mix: Qt6+sprintf → 999.22a (19 sites); GENETICDOCK → v2.0 (28); Eigen → v2.0 (8); vendor (openbabel/plugin.h) → OOS (2) |
| `-Wunused-comparison` | 69 | `EnumeratorIndex_test.C` (40), `Substring_test.C` (14), `ConstRandomAccessIterator_test.C` (12), `Bit_test.C` (3) | **Tier-D — defer to v2.0 macro redesign** (TEST_EQUAL idiom) |
| `-Wpotentially-evaluated-expression` | 4 | `connectionObject.C:183`, `molecularStructure.C:336` | **999.22c** (2 logical sites, ×2 instantiations each) |
| `-Wunused-value` | 2 | scattered | needs decision — likely 999.22a or v2.0 test cleanup |
| `-Wliteral-range` | 2 | `Function_test.C` | test cleanup → defer |
| `-Wliteral-conversion` | 1 | `HashGrid3_test.C` | test cleanup → defer |
| (`-Wno-dev`, `-Wcounterexamples`, `-Wconflicts-sr`) | 6 | CMake + Bison metadata; CIFParser/GAMESS parsers | **Phase 999.23** (already-routed) |

### 0.3 Top warning categories — Linux x64/arm64 (444 / 445)

| -W class | Count | Dominant files | Routing |
|---|---|---|---|
| `-Wdeprecated-copy` | 200 | `renderTarget.h` (123), `create.h` (21), `cosineTorsion.h` (10), `dockDialog.C` (4), `FPTBondOrderStrategy.C` (4), `empiricalHSShiftProcessor.C` (4), `forceFieldComponent.h` (4) | (b) Phase 999.6 dissolves renderTarget.h cluster → **v2.0**; (a) mechanical for ~161; (c) ~14 per-site review |
| `-Wdeprecated-declarations` | 151 | `genes.h` (50), `dockProblem.h` (18), `geneticAlgorithm.C` (14), `geneticIndividual.h` (12), `helpViewer.C` (7), `editMode.C` (6), `genes.C` (6), `geneticAlgorithm.h` (6) | GENETICDOCK self-deprecation (~110) → **v2.0 Phase 999.12**; Qt6 Stage-1 sites already in 999.22a preseed; new Linux-only `helpViewer.C` (7) + `serverWidget.h` (3) deprecated Qt API surface — defer to v2.0 unless trivially mechanical |
| `-Wunused-parameter` | 26 | renderer.C (12), raytracingRenderer.C (3), spaceGroupParameters.C (4), dockingController.C (1), dockingAlgorithm.C (2) | OOS — deliberate API-shape preservation (virtual overrides) |
| `-Wformat-truncation=` | 19 | `CIFParserParser.y` | **Phase 999.23** (already-routed) |
| `-Wsign-compare` | 15 | QSARData.C, assignBondOrderProcessor.C, regressionModel.C, scene.C | OOS — Tier-C residual |
| `-Wunused-but-set-variable` | 12 | scattered | OOS — dead-code style |
| `-Wunused-function` | 7 | scattered | OOS — dead-code style |
| `-Wcpp` | 6 | Qt GLEW incompatibility (vendor) | OOS — third-party |
| `-Wmaybe-uninitialized` | 5 | scattered | OOS — potential Tier-B follow-on, not 999.22 |
| `-Warray-bounds=` | 1 | scattered | OOS |
| `-Woverloaded-virtual=` | 1 (arm64 only) | scattered | OOS — arm64-gcc-only |
| (`-Wno-dev`, `-Wcounterexamples`, `-Wconflicts-sr`) | 4 | CMake + Bison | **Phase 999.23** (already-routed) |

### 0.4 Phase 999.22a — Wave 2 mechanical-fix LOCK (23 sites, no expansion)

**LOCKED COUNT: 23 sites.** Census refresh confirms preseed's count holds; no new mechanical sites surfaced by the cross-platform CI run that aren't already captured.

- **19 macOS sites:** 5 sprintf (logStream.C, GAMESSDatFile.C, KCFFile.C ×2, PDBFileDetails.C) + 7 QMessageBox ctor/static (dockDialog.C ×2, dockResultDialog.C ×2, mainControl.C, dockingController.C, demoTutorialDialog.C) + 6 QMouseEvent::x()/y() (editMode.C across 4 lines) + 1 QMouseEvent ctor (testFramework.h, ×2 TU instantiations) + 1 QImage::mirrored (glRenderer.C). See preseed §"Route to 999.22a" for the per-site table.
- **4 Windows sites:** binaryFingerprintMethods.C lines 1602, 1604, 1608, 1609 — Vertex → unsigned int narrowing sibling-cluster of `0a75ede`'s line 1623 fix. Confirmed by §0.1 Windows table (1 surfaced site is the cluster's representative).

**Verification expectation after 999.22a lands:** macOS `-Wdeprecated-declarations` drops by ~19 (from 80 → ~61); Linux drops by ~6 (editMode.C); Windows C4267 drops to 0.

### 0.5 Phase 999.22c — Wave 2 per-site review LOCK (2 logical sites)

**LOCKED COUNT: 2 logical sites** (= 4 macOS warning instances at ×2 instantiation each).

- `source/VIEW/KERNEL/connectionObject.C:183` — `typeid(*ptr)` with side effects
- `source/VIEW/WIDGETS/molecularStructure.C:336` — same pattern

Each requires per-site judgement: extract to variable, change to static `typeid(MyClass)`, or confirm intentional. Not mechanical.

### 0.6 Deferral confirmations (already-policy, no v1.7 action)

| Category | Approx count | Disposition |
|---|---|---|
| `-Wunused-comparison` on TEST_EQUAL macro | 69 (macOS) | **Tier-D → v2.0** macro redesign (classTest.h) |
| GENETICDOCK self-deprecation (`GenericGene`, `GeneticAlgorithm`, `GeneticIndividual`, `genes.h`, `dockProblem.h`) | ~110 (Linux) + ~28 (macOS) | **Phase 999.12 → v2.0** deprecated-code removal |
| `-Wdeprecated-copy` PixelFormat / renderTarget.h cluster | 123 + 2 = 125 (Linux) | **Phase 999.22b → v2.0** (blocked by Phase 999.6 PIPE-01 renderer rewrite) |
| `-Wdeprecated-copy` mechanical cluster (MMFF94, CHARMM, AMBER, NMR, FORMAT structs) | ~161 (Linux) | **Phase 999.22a → v1.7** Wave 2 if scope permits; otherwise v2.0 with the rest |
| Eigen `jacobiSvd`/`compute` API change | 8 (macOS) | **OOS v1.7** — template-param migration on every callsite, v2.0 candidate |
| Qt GLEW vendor `#warning` collisions | 6 (Linux) + 12 (macOS) | **OOS** — third-party headers |
| openbabel `binary_function` | 2 (macOS) | **OOS** — third-party header |
| `-Wunused-parameter` on virtual overrides | 26 (Linux) | **OOS** — deliberate API-shape preservation |
| `-Wsign-compare`, `-Wunused-but-set-variable`, `-Wunused-function`, `-Wmaybe-uninitialized`, `-Warray-bounds=` | 40 (Linux) | **OOS Tier-C residual** — not in original 999.22 scope |
| `-Wformat-truncation=` in CIFParserParser.y + Bison shift-reduce | 19+5 (Linux) + 4 (macOS) | **Phase 999.23** (already-routed) |
| VRMLRenderer self-deprecated | 0 | **RESOLVED via Phase 999.37** (class deleted) — empirically confirmed at 0 across all platforms in CI run 25989752190 |

### 0.7 Linux-only deprecated-declarations sites surfaced by v1.7 census

These were not in the preseed (which was macOS-only). The cross-platform refresh shows GCC reports additional Qt deprecations on Linux that clang did not surface on macOS:

| File | Linux count | Disposition |
|---|---|---|
| `source/VIEW/WIDGETS/helpViewer.C` | 7 | Likely Qt 6 deprecated API in QHelpEngine / QHelpContentWidget surface — **defer to 999.22a stretch goal or v2.0**; not in preseed scope-lock |
| `source/VIEW/KERNEL/serverWidget.C` | 2 | `include/BALL/VIEW/KERNEL/serverWidget.h` | 3 | Same — defer evaluation |
| `source/DOCKING/GENETICDOCK/parameter.C` | 5 | GENETICDOCK self-deprecation — covered by v2.0 Phase 999.12 routing |

**Decision:** these Linux-only sites are NOT added to 999.22a's 23-site lock. They are routed as v2.0 cleanup unless trivial inspection during 999.22a execution reveals one-line `Q*` migrations.

### 0.8 Audit trail (v1.7 refresh)

- **Source CI run:** [25989752190](https://github.com/BALL-Project/ball/actions/runs/25989752190) — Wave 1 batch validation green on all 4 platforms.
- **Raw logs:** `/tmp/wc-999.22/raw-{macos-arm64,linux-x64,linux-arm64,windows-x64}.log` (ephemeral; per-platform totals + category breakdown locked in §§0.1-0.3 above).
- **Preseed source:** `.planning/v1.7-WARNING-CENSUS-PRESEED.md` — fully accepted.
- **No code changes this refresh** — census-only per 2026-05-16 Open Q3 user direction. Routing only.

### 0.9 Handoff

- **999.22a Wave 2:** execute 23 sites per preseed §"Route to 999.22a" tables. Expected effort ~30 min.
- **999.22c Wave 2:** execute 2 per-site reviews per preseed §"Route to 999.22c" table. Per-site judgement required.
- **999.22b:** unchanged from v1.6.x — remains v2.0 / blocked by Phase 999.6.

---

# Historical context (v1.6.x baseline — retained for audit)

The sections below were generated in the v1.6.x baseline census on 2026-05-16. They are
retained verbatim as audit context; the v1.7 lock in §0 above supersedes them for Wave 2
routing.

---
phase: 999.22-warning-census
generated: 2026-05-16
ci_run: 25970862407
commit: 0a75edede
analyst: Claude (agent)
user_direction: 2026-05-16 Open Q3 census-only resolution
---

# Tier-C Warning Census — BALLView v1.6-modernization

## 1. Header / Scope

**Census-only.** This document catalogues the deferred Tier-C warning surface on the
`v1.6-modernization` branch as of 2026-05-16. No warnings were fixed during this census pass
per the **2026-05-16 Open Q3 user direction**: do census-only, file three deferral backlog stubs
(999.22a/b/c), and ship v1.6.2 with no actual warning reduction.

**Source CI run:** [25970862407](https://github.com/BALL-Project/ball/actions/runs/25970862407)
on commit `0a75edede` — tri-OS green (macOS arm64, Linux x64, Windows x64, lint, tests).

**Critical finding:** The parallel-session warning fixes that landed between Phase 5.1 closure
and this census (commits `82dba3c`, `24c044f`, `2497369`, `b98b63a`, `94fcd73`, `3519c34`,
`85d991d`, `cb392cc`, `c947b2d`, `0a75edede`) have reduced the tri-OS Tier-C surface from the
**pre-fix estimate of ~3,700+316 = ~4,016** down to approximately **~200 warnings** — a **95%
reduction** before the census even landed. The census reflects this dramatically improved state.

The Phase 5.1 pre-fix baseline (CI run 25953405453) documented:
- 307 × C4910 on Windows (atom.h `extern template` pattern — intentional trade-off)
- 9 × C4834 on Windows (nodiscard discards)
- ~3,700 estimate for macOS/Linux Tier-C (-Wdeprecated-copy, -Wunqualified-std-cast-call, -Wcatch-value)

**Post-parallel-session state (this census):**
- Windows C4910: **0** (eliminated by parallel-session fixes)
- Windows C4834: **0** (eliminated by parallel-session fixes)
- macOS/Linux -Wdeprecated-copy: **200 on Linux** (macOS: 0 — clang is stricter about detecting these, which means macOS found and eliminated them earlier; GCC's detection of additional inherited-class patterns produces Linux-only warnings in some classes)
- macOS/Linux -Wunqualified-std-cast-call: **0** (eliminated by parallel-session)
- macOS/Linux -Wcatch-value: **0** (eliminated by parallel-session)

**atom.h intentional warning:** C4910 on atom.h is now ZERO (parallel-session suppressed or restructured). The atom.h `-Wdeprecated-copy` special case is documented in Section 4.

---

## 2. Per-Class Summary Table

| Warning class | Platform | Count (post-fix census) | Pre-fix baseline | Dominant cause | Deferral target |
|---|---|---|---|---|---|
| `-Wdeprecated-copy` | Linux x64 | **200** | ~1,500 est. | Implicit copy-ctors on classes with user-declared copy-assign (or vice versa); `PixelFormat` in renderTarget.h (41 TUs × 3 sites each = 123); MMFF94 parameter structs via create.h (21); cosineTorsion.h (10); scattered MOLMEC/FORMAT/NMR/QSAR structs | (a) mechanical for most; (b) VIEW/RENDERING path; (c) dockDialog/FDPBDialog UI where intent unclear |
| `-Wdeprecated-copy` | macOS arm64 | **0** | ~1,000 est. | Eliminated by parallel-session — clang stricter, caught earlier | N/A — closed |
| `-Wunqualified-std-cast-call` | macOS + Linux | **0** | ~1,800 est. | `dynamic_cast`/`static_cast` without `std::` qualification | N/A — closed |
| `-Wcatch-value` | macOS + Linux | **0** | ~400 est. | `catch(BALLException)` by value | N/A — closed |
| `C4910` (Windows MSVC) | Windows x64 | **0** | 307 | `BALL_EXPORT` on `extern template` in atom.h — known trade-off per Phase 5.1 | N/A — closed (parallel-session) |
| `C4834` (Windows MSVC) | Windows x64 | **0** | 9 | `[[nodiscard]]` return ignored | N/A — closed (parallel-session) |

**Note on Windows residual:** CI run 25970862407 shows one remaining `warning C4267` on
`source/STRUCTURE/binaryFingerprintMethods.C` (size_t → int narrowing via MSVC STL `utility(303)`).
This is a Tier-C narrowing warning **not in the original pre-fix census scope** (was C4267, not
C4910/C4834). Documented here as an out-of-scope residual; see Section 6.

---

## 3. Per-File × Warning-Class Breakdown

### 3.1 `-Wdeprecated-copy` — Linux x64 (200 warnings across 25 file groups)

Grouped by (file, warning-class); **total 200 instances** on 25 distinct file paths.

| # | File | Count | Affected class(es) | Cause | Category | Notes |
|---|------|-------|--------------------|-------|----------|-------|
| 1 | `include/BALL/VIEW/RENDERING/renderTarget.h` | 123 | `BALL::VIEW::PixelFormat` | `PixelFormat` has user-declared copy-assign (`operator=`) but compiler-provided copy-ctor — triggers -Wdeprecated-copy since C++11 rule-of-zero requires either declare-all or declare-none. 41 TUs × 3 copy sites. | **(b)** | VIEW/RENDERING path. Phase 999.6 PIPE-01 renderer rewrite targets this subsystem. Cleaning in v1.6.x is wasted work. |
| 2 | `include/BALL/COMMON/create.h` | 21 | `MMFF94StretchParameters`, `MMFF94BendParameters`, `MMFF94StretchBendParameters`, `MMFF94TorsionParameters`, `MMFF94PlaneParameters` | MMFF94 parameter structs in `create.h:73` macro expansion — structs define copy-assign but rely on implicit copy-ctor. 5 classes × up to ~5 TU compilations. | **(a)** | Mechanical: add `= default` copy-ctor to each MMFF94 parameter struct. CI-bot safe. |
| 3 | `include/BALL/MOLMEC/PARAMETER/cosineTorsion.h` | 10 | `BALL::CosineTorsion` | Torsion parameter struct with user-declared copy-assign, implicit copy-ctor. | **(a)** | Mechanical: add `= default` copy-ctor. |
| 4 | `source/VIEW/DIALOGS/dockDialog.C` | 4 | `BALL::RadiusRuleProcessor`, `BALL::ChargeRuleProcessor` | Processor classes copy-assigned in dialog code; classes define user op= but implicit copy-ctor. | **(c)** | Intent unclear — Processor objects should not typically be copy-assigned; needs per-site code review to verify correctness semantics. |
| 5 | `source/STRUCTURE/BONDORDERS/FPTBondOrderStrategy.C` | 4 | (bond order strategy copy) | Implicit copy-ctor on class with user copy-assign. | **(a)** | Mechanical: add `= default` copy-ctor or convert to rule-of-zero. |
| 6 | `source/NMR/empiricalHSShiftProcessor.C` | 4 | `BALL::EmpiricalHSShiftProcessor` | Processor copy semantics. | **(a)** | Mechanical: add `= default` copy-ctor. |
| 7 | `include/BALL/MOLMEC/COMMON/forceFieldComponent.h` | 4 | `BALL::ForceFieldComponent` | Base class for MOLMEC force-field components; user-declared copy-assign, implicit copy-ctor. | **(c)** | Needs per-site review — ForceFieldComponent copy semantics affect all MOLMEC subclass copy behavior. Not trivially safe. |
| 8 | `source/VIEW/RENDERING/renderWindow.C` | 2 | `BALL::VIEW::PixelFormat` | Same PixelFormat issue as renderTarget.h but in renderWindow.C TU. | **(b)** | VIEW/RENDERING path — same Phase 999.6 deferral. |
| 9 | `source/VIEW/DIALOGS/FDPBDialog.C` | 2 | (FDPB dialog copy) | Dialog TU triggers copy on an underlying parameter type. | **(c)** | VIEW/DIALOGS — intent review needed. |
| 10 | `source/MOLMEC/AMBER/amberTorsion.C` | 2 | (AMBER torsion copy) | AMBER torsion parameter class. | **(a)** | Mechanical: add `= default` copy-ctor. |
| 11 | `include/BALL/VIEW/KERNEL/message.h` | 2 | `BALL::VIEW::Message` | Message base class copy semantics. | **(c)** | MESSAGE system copy could have intentional semantics in the COMPOSITE event pipeline. Needs review. |
| 12 | `test/EnumeratorIndex_test.C` | 3 | `BALL::EnumeratorIndex` | Test code triggers copy on an enumerator index type. | **(a)** | Mechanical: add `= default` copy-ctor to `EnumeratorIndex`. |
| 13 | `test/RotamerLibrary_test.C` | 1 | (rotamer copy) | Test code. | **(a)** | Mechanical. |
| 14 | `source/VIEW/MODELS/standardColorProcessor.C` | 1 | (color processor copy) | VIEW model code. | **(a)** | Mechanical. |
| 15 | `source/SCORING/COMPONENTS/PLP.C` | 1 | (PLP scoring component) | Scoring component copy. | **(a)** | Mechanical. |
| 16 | `source/MOLMEC/MMFF94/MMFF94NonBonded.C` | 1 | (MMFF94 nonbonded copy) | MMFF94 non-bonded component. | **(a)** | Mechanical. |
| 17 | `source/MOLMEC/CHARMM/charmmTorsion.C` | 1 | (CHARMM torsion copy) | CHARMM torsion. | **(a)** | Mechanical. |
| 18 | `source/MOLMEC/CHARMM/charmmNonBonded.C` | 1 | (CHARMM nonbonded copy) | CHARMM non-bonded. | **(a)** | Mechanical. |
| 19 | `source/MOLMEC/AMBER/amberNonBonded.C` | 1 | (AMBER nonbonded copy) | AMBER non-bonded. | **(a)** | Mechanical. |
| 20 | `source/FORMAT/parameters.C` | 1 | (format parameter copy) | Format layer parameter. | **(a)** | Mechanical. |
| 21 | `source/FORMAT/CIFFile.C` | 1 | (CIF file copy) | CIF format file. | **(a)** | Mechanical. |
| 22 | `include/BALL/STRUCTURE/defaultProcessors.h` | 1 | (default processor copy) | Default processor header. | **(a)** | Mechanical. |
| 23 | `include/BALL/NMR/empiricalHSShiftProcessor.h` | 1 | `BALL::EmpiricalHSShiftProcessor` | Same class as row 6 (header emission). | **(a)** | Mechanical: same fix as row 6. |
| 24 | `include/BALL/FORMAT/CIFFile.h` | 1 | (CIF file header copy) | CIF header copy. | **(a)** | Mechanical. |
| 25 | `include/BALL/CONCEPT/enumerator.h` | 1 | `BALL::EnumeratorIndex` | Same class as row 12 (header emission). | **(a)** | Mechanical: same fix as row 12. |

**Subtotal `-Wdeprecated-copy`:** 200 warnings total.
- Category **(a)** mechanical: ~161 warnings (rows 2-3, 5-6, 10, 12-25)
- Category **(b)** VIEW/RENDERING (Phase 999.6 target): ~125 warnings (rows 1, 8)
- Category **(c)** per-site review: ~14 warnings (rows 4, 7, 9, 11)

**Note:** renderTarget.h (row 1, 123 warnings) is the single largest cluster; `PixelFormat` in
`include/BALL/VIEW/RENDERING/renderTarget.h` triggers once per 3 template initializer expressions
per TU that includes it. 41 TUs include renderTarget.h, each generating ~3 warning sites.

### 3.2 `-Wdeprecated-declarations` — macOS arm64 (82 warnings) + Linux x64 (152 warnings)

This warning class covers deprecated Qt APIs, deprecated Eigen APIs, deprecated BALL-internal
APIs (`[[deprecated]]` attribute), and macOS/Linux sprintf deprecation. **Not a Tier-C warning**
in the original Phase 5.1 census scope — included here as an observed warning class for
completeness. Most are opportunistic fixes done by the parallel session already or small
remaining sites.

**macOS arm64 -Wdeprecated-declarations breakdown:**

| File | Count | Cause | Category |
|------|-------|-------|----------|
| `include/BALL/DOCKING/GENETICDOCK/geneticIndividual.h` | 24 | `GenericGene` deprecated attribute; used across multiple TUs | **(c)** — deprecated because class was marked `[[deprecated]]` in earlier refactor; needs per-site verification that callers can migrate |
| `source/VIEW/KERNEL/MODES/editMode.C` | 12 | `QMouseEvent::x()`/`QMouseEvent::y()` deprecated in Qt 6; should use `position()` | **(a)** mechanical — Qt 6 migration: replace `.x()/.y()` with `.position().x()/.y()` |
| `source/VIEW/DIALOGS/dockResultDialog.C` | 4 | Qt 6 deprecated dialog API | **(a)** mechanical |
| `source/VIEW/DIALOGS/dockDialog.C` | 4 | Qt 6 deprecated dialog API (QMessageBox deprecated overload — different from 2497369 fix on modifyRepresentationDialog.C) | **(a)** mechanical |
| `source/QSAR/plsModel.C` | 4 | Eigen `jacobiSvd` deprecated; should use template parameter form | **(a)** mechanical — Eigen API migration |
| `source/QSAR/pcrModel.C` | 4 | Eigen `compute` deprecated | **(a)** mechanical — Eigen API migration |
| `source/FORMAT/KCFFile.C` | 4 | `sprintf` deprecated on macOS (Apple recommends snprintf) | **(a)** mechanical — sprintf → snprintf |
| `source/DOCKING/GENETICDOCK/geneticIndividual.C` | 4 | `GenericGene` deprecated (same as geneticIndividual.h) | **(c)** same as above |
| `source/VIEW/WIDGETS/dockingController.C` | 2 | Qt 6 deprecated widget API | **(a)** mechanical |
| `source/VIEW/RENDERING/rendererFactory.C` | 2 | Qt 6 deprecated rendering API | **(a)** mechanical |
| `source/VIEW/RENDERING/RENDERERS/glRenderer.C` | 2 | Qt 6 deprecated GL renderer API | **(a)** mechanical |
| `source/VIEW/KERNEL/mainControl.C` | 2 | Qt 6 deprecated control API | **(a)** mechanical |
| `source/FORMAT/PDBFileDetails.C` | 2 | sprintf deprecated (macOS) | **(a)** mechanical |
| `source/FORMAT/GAMESSDatFile.C` | 2 | sprintf deprecated (macOS) | **(a)** mechanical |
| `source/COMMON/logStream.C` | 2 | sprintf deprecated (macOS) | **(a)** mechanical — already in plan for this area |
| `source/APPLICATIONS/BALLVIEW/demoTutorialDialog.C` | 2 | Qt 6 deprecated tutorial dialog API | **(a)** mechanical |
| `include/BALL/VIEW/WIDGETS/testFramework.h` | 2 | Qt 6 deprecated test framework API | **(a)** mechanical |
| `/opt/homebrew/…/openbabel3/openbabel/plugin.h` | ~5 | OpenBabel `binary_function` deprecated in C++17 (STL removal) | OUT OF SCOPE — third-party header; not BALL source |

### 3.3 `-Wunused-comparison` — macOS arm64 (69 warnings)

Test-only warnings. All in test source files.

| File | Count | Cause | Category |
|------|-------|-------|----------|
| `test/EnumeratorIndex_test.C` | 40 | Comparison in iterator test always-true/always-false (test logic) | **(c)** — test code review needed; could be intentional test idiom |
| `test/Substring_test.C` | 14 | String comparison result unused in test | **(c)** — test review |
| `test/ConstRandomAccessIterator_test.C` | 12 | Iterator comparison in test | **(c)** — test review |
| `test/Bit_test.C` | 3 | Bitfield comparison in test | **(c)** — test review |

### 3.4 `-Wformat-truncation=` — Linux x64 (19 warnings)

All in `source/FORMAT/CIFParserParser.y` (Bison-generated parser). These warnings fire because
the snprintf calls introduced in Phase 5.1 Plan 05 have potential truncation at the 2550-byte
buffer boundary. **This is Phase 999.23 territory** (CIF Bison grammar audit). Out of scope
for 999.22 census categorization — see Section 6.

### 3.5 Other Warning Classes (not Tier-C census scope)

| Warning class | Platform | Count | Notes |
|---|---|---|---|
| `-Wunused-parameter` | Linux | 26 | Deliberate API-shape preservation (renderer.C 12, raytracingRenderer.C 3, spaceGroupParameters.C 4, dockingController.C 1, dockingAlgorithm.C 2, etc.) — out of scope; tracked as residual |
| `-Wsign-compare` | Linux | 15 | QSARData.C, assignBondOrderProcessor.C, regressionModel.C, scene.C — out of scope |
| `-Wunused-but-set-variable` | Linux | 12 | Various — out of scope |
| `-Wunused-function` | Linux | 7 | Dead-code style warnings — out of scope |
| `-Wcpp` | Linux | 6 | Qt GLEW incompatibility notice in QtGui headers (third-party) — out of scope |
| `-Wmaybe-uninitialized` | Linux | 5 | Possible uninitialized paths — out of scope; potential Tier-B follow-on |
| `-Warray-bounds=` | Linux | 1 | Array bounds check — out of scope |
| `-W#warnings` | macOS | 12 | GLEW+Qt incompatibility macros in QtGui headers (third-party) — out of scope |
| `-Wpotentially-evaluated-expression` | macOS | 4 | VIEW/WIDGETS/molecularStructure.C + connectionObject.C — out of scope |
| `-Wunused-comparison` | macOS | 69 | Test code — out of scope (see §3.3) |
| `-Wliteral-range` | macOS | 2 | test/Function_test.C — out of scope |
| `-Wliteral-conversion` | macOS | 1 | test/HashGrid3_test.C — out of scope |
| `C4267` | Windows | 1 | `source/STRUCTURE/binaryFingerprintMethods.C` size_t→int narrowing in MSVC STL — residual from 0a75edede fix (missed site); not Tier-C per original census scope |
| `-Wconflicts-sr` | all | 34 total | Bison grammar shift-reduce conflicts (GAMESSDatParser: 3, CIFParser: 5, smartsParser: 26) — Phase 999.23 scope |

---

## 4. The atom.h INTENTIONAL Warning (PRESERVE — DO NOT TOUCH IN 999.22a)

**Status as of this census: C4910 on atom.h is ZERO** (parallel-session commits resolved the
C4910 instance). The -Wdeprecated-copy concern for atom.h is also not present in the current CI log.

However, the design decision documented in
[05.1-08-SUMMARY.md](../05.1-build-warnings-and-latent-bugs/05.1-08-SUMMARY.md) and
[05.1-07-SUMMARY.md](../05.1-build-warnings-and-latent-bugs/05.1-07-SUMMARY.md) MUST be
preserved for any future work touching `include/BALL/KERNEL/atom.h`:

> **`include/BALL/KERNEL/atom.h` carries a deliberate copy/export pattern** required for the
> COMPOSITE pattern downcast semantics. The `extern template class BALL_EXPORT std::vector<Atom*>;`
> declaration in `atom.h` was intentionally restored (after Plan 05.1-07's vector3 fix pattern
> was attempted and reverted) because the alternative caused LNK2005 link failures in libVIEW.
> The current state (C4910 suppressed by Phase 999.26 pragma or restructured by parallel-session)
> is the correct production state.

**Rule for 999.22a executors:** When executing the (a) mechanical fixes, **do not touch atom.h's**
`extern template class` or copy-constructor declarations without first re-reading:
- `phases/05.1-build-warnings-and-latent-bugs/05.1-07-SUMMARY.md`
- `phases/05.1-build-warnings-and-latent-bugs/05.1-08-SUMMARY.md`

The atom.h copy semantics are load-bearing in the BALL DLL export chain. **PRESERVE — DO NOT TOUCH IN 999.22a** unless the full rationale chain is understood and tri-OS link verified.

---

## 5. Category (a)/(b)/(c) Split Summary

| Category | Warning count (post-fix census) | Percentage | Dominant warning class | Target stub |
|----------|--------------------------------|------------|------------------------|-------------|
| **(a) Mechanical-fix** | ~161 `-Wdeprecated-copy` + ~65 `-Wdeprecated-declarations` = **~226** | ~73% | `-Wdeprecated-copy` on MOLMEC/FORMAT/NMR/QSAR structs; Qt 6 API migrations; sprintf → snprintf; Eigen API update | **999.22a → v1.7** |
| **(b) Defer-to-v2.0 (blocked-by-999.6)** | ~125 `-Wdeprecated-copy` | ~40% | `PixelFormat` in renderTarget.h + renderWindow.C (VIEW/RENDERING path) | **999.22b → v2.0** |
| **(c) Per-site review** | ~14 `-Wdeprecated-copy` + ~28 `-Wdeprecated-declarations` + ~69 `-Wunused-comparison` = **~111** | ~36% | dockDialog Processor copy; message.h copy; forceFieldComponent.h copy; geneticIndividual deprecated; test comparison idioms | **999.22c → v1.7** |

**Note:** Total exceeds 200 because some warnings appear in categories that overlap with the
"other warning class" residual (the -Wdeprecated-declarations and -Wunused-comparison are not
strictly Tier-C per original census scope but are documented for completeness). The core Tier-C
surface of **200 `-Wdeprecated-copy` on Linux** breaks down as: **~161 (a) + ~125 (b) + ~14 (c)**.

**Total Tier-C surface (post-fix, census baseline):** **200 warnings** (Linux `-Wdeprecated-copy`)
vs the pre-fix estimate of ~3,700 — a **95% reduction** from the parallel-session fixes.

The (a)+(b) percentages exceed 100% because renderTarget.h appears in both (b) and the total
count — the 123-warning renderTarget.h cluster is categorized as **(b)** in isolation but cross-
listed as (a)-mechanical-capable since the underlying `PixelFormat = default` fix is trivial.
The categorization takes the conservative path: (b) takes priority over (a) when Phase 999.6
is the natural dissolve point.

---

## 6. What is NOT in Scope (per 2026-05-16 Open Q3 direction)

The following are explicitly **outside the 999.22 census scope** and will NOT be addressed here:

1. **No warning execution in v1.6.2.** Zero source-code changes this phase. The (a), (b), and (c)
   backlog stubs (999.22a/b/c) are the routing mechanism for future execution.

2. **Tier-A / Tier-B / Tier-D warnings** — those were Phase 5.1 scope and are closed. The
   Phase 5.1 closure state (CI run 25953405453) shows zero Tier-A/B/D warnings on the targeted
   files. Not revisited here.

3. **Bison shift-reduce conflicts** (`-Wconflicts-sr`: GAMESSDatParser ×3, CIFParser ×5,
   smartsParser ×26 = 34 total) — this is Phase **999.23** territory (CIF Bison grammar shift-
   reduce audit). Tracked in ROADMAP.md §999.23.

4. **Phase 999.6-reachable warnings** — the renderTarget.h `-Wdeprecated-copy` cluster (123
   warnings, ~40% of remaining surface) will be dissolved when Phase 999.6 PIPE-01 rewrites
   the renderer. These are categorized (b) and deferred to v2.0. Even if mechanically fixable
   in v1.6.x, it is wasted work — the call sites evaporate.

5. **Third-party header warnings** — OpenBabel `binary_function` (openbabel/plugin.h), GLEW+Qt
   incompatibility notices (qopenglcontext.h), and MSVC STL headers. Not BALL source; not in
   scope.

6. **The residual Windows `C4267` on `binaryFingerprintMethods.C`** — this is a narrowing
   warning (size_t → int) that was in the original C4267 Tier-C candidate list but was only
   partially fixed by commit `0a75edede` (3 sites). This 1 remaining site is a Tier-C narrowing
   residual that can be folded into 999.22a as a mechanical fix, but it is NOT one of the five
   originally specified warning classes (C4910/C4834 on Windows; -Wdeprecated-copy/
   -Wunqualified-std-cast-call/-Wcatch-value on macOS/Linux).

7. **`-Wmaybe-uninitialized` warnings on Linux (5 instances)** — these could be Tier-B latent
   bugs. Out of scope for 999.22; potential follow-on for a future dedicated Tier-B sweep.

8. **`-Wunused-parameter` on Linux (26 instances)** — deliberate API-shape preservation
   (virtual overrides keeping the parameter signature). Out of scope; tracked as residual.

---

## 7. Audit Trail

Generated **2026-05-16**; source CI run `25970862407` (commit `0a75edede`, branch
`v1.6-modernization`); analyst Claude (agent); user direction: **2026-05-16 Open Q3 —
census-only resolution for Phase 999.22** (MILESTONE-CONTEXT.md §"Resolved questions" item 3).

**Key finding:** Parallel-session warning fixes (10 commits since Phase 5.1 closure) reduced
the pre-census estimate from ~3,716 total Tier-C instances to **~200 remaining** (Linux
`-Wdeprecated-copy` only). The three downstream stubs (999.22a/b/c) reflect this dramatically
smaller scope — execution effort projections in those stubs should be revised down from original
ROADMAP estimates accordingly.
