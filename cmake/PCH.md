# cmake/PCH.md — Precompiled Headers: Audit, Header Sets, and ccache Contract

**Phase:** 999.16 (BUILD-ACCEL-01)
**Date:** 2026-05-16
**Targets:** `BALL` library, `VIEW` library (BALLView executable is deliberately excluded)
**Purpose:** Canonical reference for the PCH header sets chosen for each target, the
  ccache sloppiness requirement, and the audit method used to derive them.
  Re-audits should follow the method in "Re-audit Commands" and document
  results in this file.

---

## Acceptance Baselines (from Phase 999.2 SUMMARY)

| Platform | Generator | Cold Build (Phase 999.2 baseline) | Warm Build |
|---|---|---:|---:|
| macOS arm64 | Ninja | 31s | 39s |
| Linux x64 | Ninja | 25s | 34s |
| Windows x64 | Ninja | 4818s (cold-cache populate) | 55s |

**Reduction targets (per ROADMAP §999.16):**
- Cold-cache Windows Build step: ≥ 15% reduction (target: ≤ 4095s cold)
- Cold-cache Linux + macOS Build step: ≥ 10% reduction
- Warm-cache build: no regression on any platform

---

## Audit Method

### Re-audit Commands

Run these commands from the repo root to reproduce the frequency audit.

**BALL target (prescribed scope: KERNEL, CONCEPT, COMMON, DATATYPE, MATHS, SYSTEM):**

```bash
# TU count denominator
find source/KERNEL source/CONCEPT source/COMMON source/DATATYPE source/MATHS source/SYSTEM \
  -name '*.C' | wc -l

# Top-25 includes across BALL core modules
grep -h '#include' \
  source/KERNEL/*.C source/CONCEPT/*.C source/COMMON/*.C \
  source/DATATYPE/*.C source/MATHS/*.C source/SYSTEM/*.C \
  2>/dev/null | sed 's/^[[:space:]]*//' | sort | uniq -c | sort -rn | head -25
```

**BALL broader scope (for completeness — all library modules):**

```bash
grep -rh '#include' \
  source/KERNEL/ source/CONCEPT/ source/COMMON/ source/DATATYPE/ \
  source/MATHS/ source/SYSTEM/ source/FORMAT/ source/STRUCTURE/ \
  source/ENERGY/ source/MOLMEC/ source/NMR/ source/SOLVATION/ source/QSAR/ \
  2>/dev/null | sed 's/^[[:space:]]*//' | grep -E '^#include <BALL/' \
  | sort | uniq -c | sort -rn | head -25
```

**VIEW target:**

```bash
# TU count denominator
find source/VIEW -name '*.C' | wc -l

# Top-25 BALL includes
find source/VIEW -name '*.C' -exec grep -h '#include' {} + 2>/dev/null \
  | sed 's/^[[:space:]]*//' | grep -E '^#include <BALL/' \
  | sort | uniq -c | sort -rn | head -25

# Top-25 Qt includes
find source/VIEW -name '*.C' -exec grep -h '#include' {} + 2>/dev/null \
  | sed 's/^[[:space:]]*//' | grep -E '^#include <Qt' \
  | sort | uniq -c | sort -rn | head -25
```

### Filter Criteria

PCH candidates must satisfy all three:

1. **Frequency** — included by ≥ 50% of audited TUs in the target's prescribed scope,
   OR ranked in the top 10 by inclusion count (structural note below).
2. **Weight** — heavy or template-dense (Qt, Boost, Eigen, BALL/CONCEPT, BALL/COMMON),
   or foundational to many transitive include chains.
3. **Stability** — no modification in the last 3 months (check via
   `git log --since="3 months ago" --oneline -- include/BALL/...`).

**Structural note on the 50% TU criterion:** BALL's `.C` files are tightly paired
with their own module `.h` files (each `.C` typically includes one or two module
headers). This means no single BALL internal header is directly included by ≥ 50% of
TUs in the narrow core-module scope (99 TUs). Even in the broader 362-TU scope, the
highest-frequency header (`BALL/KERNEL/PTE.h`) appears in only 65 TUs (17.9%). This is
structural — BALL uses conditional include guards throughout, and transitive coverage
is high even when direct inclusion count is low. The PCH set below is therefore chosen
by **top-N by frequency + stability + heaviness**, with the strict 50% criterion
documented as "not achievable in this codebase's header layout."

---

## Audit Results — 2026-05-16

### BALL Target Audit

**Scope:** KERNEL, CONCEPT, COMMON, DATATYPE, MATHS, SYSTEM  
**TU count:** 99 (prescribed narrow scope) / 362 (broader all-module scope)  
**50% threshold:** 50 TUs (narrow) / 181 TUs (broader)

#### Top-25 includes — narrow scope (KERNEL/CONCEPT/COMMON/DATATYPE/MATHS/SYSTEM)

| Rank | Include | Count | % of 99 TUs | PCH decision | Reason |
|---|---|---:|---:|---|---|
| 1 | `<BALL/KERNEL/protein.h>` | 9 | 9.1% | kept | top-ranked; stable; heavy (KERNEL hierarchy) |
| 2 | `<algorithm>` | 7 | 7.1% | kept | top-ranked; STL; stable |
| 3 | `<BALL/KERNEL/system.h>` | 6 | 6.1% | kept | top-ranked; stable; molecular hierarchy root |
| 4 | `<BALL/KERNEL/chain.h>` | 6 | 6.1% | kept | top-ranked; stable; molecular hierarchy |
| 5 | `<BALL/KERNEL/bond.h>` | 6 | 6.1% | kept | top-ranked; stable |
| 6 | `<cstdio>` | 5 | 5.1% | dropped | lightweight C header; no PCH benefit |
| 7 | `<BALL/KERNEL/residue.h>` | 5 | 5.1% | kept | top-ranked; stable |
| 8 | `<BALL/KERNEL/nucleicAcid.h>` | 5 | 5.1% | kept | top-ranked; stable |
| 9 | `<BALL/KERNEL/atom.h>` | 5 | 5.1% | dropped | churning: 4 commits in last 3 months (C4910 suppress fix, revert chains) — poor PCH candidate |
| 10 | `<BALL/DATATYPE/string.h>` | 4 | 4.0% | kept | top-ranked; stable; transitively included widely |
| 11 | `<BALL/CONCEPT/persistenceManager.h>` | 4 | 4.0% | kept | top-ranked; stable; heavy (template serialization) |
| 12 | `<BALL/COMMON/exception.h>` | 4 | 4.0% | kept | top-ranked; stable; foundation header |
| 13 | `<iostream>` | 3 | 3.0% | kept | top-ranked; STL; stable |
| 14 | `<cmath>` | 3 | 3.0% | dropped | lightweight C header; no PCH benefit |
| 15 | `<BALL/MATHS/vector3.h>` | 3 | 3.0% | kept | top-ranked; stable; template-heavy Eigen-backed |
| 16 | `<BALL/KERNEL/secondaryStructure.h>` | 3 | 3.0% | kept | top-ranked; stable |
| 17 | `<BALL/KERNEL/molecule.h>` | 3 | 3.0% | kept | top-ranked; stable |
| 18 | `<BALL/KERNEL/PTE.h>` | 3 | 3.0% | kept | top-ranked; stable; Periodic Table (wide transitive reach in broader scope: 65/362 = 17.9%) |
| 19 | `<BALL/DATATYPE/regularExpression.h>` | 3 | 3.0% | kept | top-ranked; stable |
| 20 | `<BALL/CONCEPT/textPersistenceManager.h>` | 3 | 3.0% | kept | top-ranked; stable |
| 21 | `<BALL/COMMON/global.h>` | 3 | 3.0% | kept | top-ranked; stable; BALL foundation root |
| 22 | `<windows.h>` | 2 | 2.0% | dropped | platform-specific; breaks non-Windows PCH |
| 23 | `<typeinfo>` | 2 | 2.0% | dropped | lightweight; no PCH benefit |
| 24 | `<sys/types.h>` | 2 | 2.0% | dropped | platform-specific POSIX |
| 25 | `<set>` | 2 | 2.0% | kept | STL container; stable |

#### Broader scope cross-check (top 10 by count across all 362 BALL TUs)

| Include | Count | % of 362 TUs | Notes |
|---|---:|---:|---|
| `<BALL/KERNEL/PTE.h>` | 65 | 17.9% | highest-frequency BALL header |
| `<BALL/KERNEL/bond.h>` | 47 | 13.0% | |
| `<BALL/KERNEL/forEach.h>` | 39 | 10.8% | macro-based iterator; stable |
| `<BALL/KERNEL/atom.h>` | 34 | 9.4% | dropped (churning) |
| `<BALL/KERNEL/system.h>` | 29 | 8.0% | |
| `<BALL/SYSTEM/path.h>` | 27 | 7.5% | stable; added to BALL PCH set |
| `<BALL/STRUCTURE/geometricProperties.h>` | 17 | 4.7% | kept |
| `<BALL/KERNEL/residue.h>` | 17 | 4.7% | |
| `<BALL/KERNEL/protein.h>` | 15 | 4.1% | |
| `<BALL/KERNEL/molecule.h>` | 14 | 3.9% | |

---

### VIEW Target Audit

**Scope:** source/VIEW (all subdirectories)  
**TU count:** 169  
**50% threshold:** 85 TUs (structural note above applies equally to VIEW)

#### Top-25 BALL includes in VIEW TUs

| Rank | Include | Count | % of 169 TUs | PCH decision | Reason |
|---|---|---:|---:|---|---|
| 1 | `<BALL/VIEW/KERNEL/mainControl.h>` | 66 | 39.1% | kept | highest-frequency; stable; heavy VIEW root |
| 2 | `<BALL/VIEW/KERNEL/common.h>` | 50 | 29.6% | kept | stable; VIEW common defs |
| 3 | `<BALL/VIEW/KERNEL/message.h>` | 40 | 23.7% | kept | stable; message/notification types |
| 4 | `<BALL/VIEW/WIDGETS/scene.h>` | 32 | 18.9% | dropped | recently changed (Phase 02, Phase 05.1); churning |
| 5 | `<BALL/VIEW/PRIMITIVES/mesh.h>` | 24 | 14.2% | kept | stable; heavy primitive type |
| 6 | `<BALL/SYSTEM/path.h>` | 19 | 11.2% | kept | stable; shared with BALL target |
| 7 | `<BALL/VIEW/PRIMITIVES/sphere.h>` | 16 | 9.5% | kept | stable; geometric primitive |
| 8 | `<BALL/KERNEL/system.h>` | 14 | 8.3% | kept | stable; molecular hierarchy root |
| 9 | `<BALL/KERNEL/PTE.h>` | 13 | 7.7% | kept | stable; Periodic Table |
| 10 | `<BALL/VIEW/PRIMITIVES/tube.h>` | 12 | 7.1% | kept | stable; geometric primitive |
| 11 | `<BALL/VIEW/PRIMITIVES/label.h>` | 12 | 7.1% | kept | stable; text primitive |
| 12 | `<BALL/VIEW/PRIMITIVES/disc.h>` | 12 | 7.1% | kept | stable; geometric primitive |
| 13 | `<BALL/VIEW/KERNEL/stage.h>` | 11 | 6.5% | kept | stable; rendering stage types |
| 14 | `<BALL/KERNEL/forEach.h>` | 11 | 6.5% | kept | stable; iterator macros |
| 15 | `<BALL/KERNEL/bond.h>` | 11 | 6.5% | kept | stable |
| 16 | `<BALL/KERNEL/atom.h>` | 11 | 6.5% | dropped | churning (same as BALL target) |
| 17 | `<BALL/VIEW/PRIMITIVES/twoColoredTube.h>` | 10 | 5.9% | kept | stable; geometric primitive |
| 18 | `<BALL/VIEW/KERNEL/clippingPlane.h>` | 10 | 5.9% | kept | stable; VIEW rendering |
| 19 | `<BALL/VIEW/DATATYPE/standardDatasets.h>` | 10 | 5.9% | kept | stable; VIEW data types |
| 20 | `<BALL/VIEW/PRIMITIVES/twoColoredLine.h>` | 9 | 5.3% | kept | stable; geometric primitive |
| 21 | `<BALL/VIEW/PRIMITIVES/simpleBox.h>` | 9 | 5.3% | kept | stable; geometric primitive |
| 22 | `<BALL/VIEW/PRIMITIVES/point.h>` | 9 | 5.3% | kept | stable; geometric primitive |
| 23 | `<BALL/VIEW/PRIMITIVES/line.h>` | 9 | 5.3% | kept | stable; geometric primitive |
| 24 | `<BALL/VIEW/PRIMITIVES/box.h>` | 9 | 5.3% | kept | stable; geometric primitive |
| 25 | `<BALL/VIEW/KERNEL/threads.h>` | 9 | 5.3% | kept | stable; threading types |

#### Top-10 Qt includes in VIEW TUs (cross-check)

Qt headers are intentionally NOT in the VIEW PCH set: they are auto-included via
Qt's own precompilation mechanism (via `target_precompile_headers` on Qt targets),
and duplicating them in BALL's PCH risks macro-expansion ordering issues with
Qt's `Q_OBJECT` macro preprocessor and MOC-generated code. The VIEW PCH set
focuses on BALL headers.

| Include | Count | Notes |
|---|---:|---|
| `<QtWidgets/QPushButton>` | 32 | excluded — Qt MOC risk |
| `<QtWidgets/QLineEdit>` | 32 | excluded — Qt MOC risk |
| `<QtWidgets/QFileDialog>` | 23 | excluded — Qt MOC risk |
| `<QtWidgets/QRadioButton>` | 17 | excluded — Qt MOC risk |
| `<QtWidgets/QCheckBox>` | 17 | excluded — Qt MOC risk |

---

## Chosen PCH Header Sets

### BALL Target

`target_precompile_headers(BALL PRIVATE ...)` — wired in `CMakeLists.txt` after the
CUDA/non-CUDA `ADD_LIBRARY` / `CUDA_ADD_LIBRARY` block.

```cmake
target_precompile_headers(BALL PRIVATE
    <algorithm>
    <iostream>
    <set>
    <BALL/COMMON/global.h>
    <BALL/COMMON/exception.h>
    <BALL/DATATYPE/string.h>
    <BALL/DATATYPE/regularExpression.h>
    <BALL/CONCEPT/persistenceManager.h>
    <BALL/CONCEPT/textPersistenceManager.h>
    <BALL/KERNEL/PTE.h>
    <BALL/KERNEL/forEach.h>
    <BALL/KERNEL/system.h>
    <BALL/KERNEL/chain.h>
    <BALL/KERNEL/bond.h>
    <BALL/KERNEL/residue.h>
    <BALL/KERNEL/molecule.h>
    <BALL/KERNEL/protein.h>
    <BALL/KERNEL/nucleicAcid.h>
    <BALL/KERNEL/secondaryStructure.h>
    <BALL/MATHS/vector3.h>
    <BALL/SYSTEM/path.h>
)
```

**Dropped candidates:**
- `<BALL/KERNEL/atom.h>` — 4 commits in last 3 months (C4910 MSVC suppress chain);
  re-audit in 3 months once churn settles.
- `<cstdio>`, `<cmath>`, `<typeinfo>` — lightweight C headers; PCH overhead not justified.
- `<windows.h>`, `<sys/types.h>` — platform-specific; breaks non-Windows/POSIX PCH compilation.

### VIEW Target

`target_precompile_headers(VIEW PRIVATE ...)` — wired in `CMakeLists.txt` inside the
`IF(BALL_HAS_VIEW)` block, after the CUDA/non-CUDA `ADD_LIBRARY` / `CUDA_ADD_LIBRARY` block.

```cmake
target_precompile_headers(VIEW PRIVATE
    <BALL/VIEW/KERNEL/mainControl.h>
    <BALL/VIEW/KERNEL/common.h>
    <BALL/VIEW/KERNEL/message.h>
    <BALL/VIEW/KERNEL/stage.h>
    <BALL/VIEW/KERNEL/threads.h>
    <BALL/VIEW/KERNEL/clippingPlane.h>
    <BALL/VIEW/PRIMITIVES/mesh.h>
    <BALL/VIEW/PRIMITIVES/sphere.h>
    <BALL/VIEW/PRIMITIVES/tube.h>
    <BALL/VIEW/PRIMITIVES/twoColoredTube.h>
    <BALL/VIEW/PRIMITIVES/twoColoredLine.h>
    <BALL/VIEW/PRIMITIVES/disc.h>
    <BALL/VIEW/PRIMITIVES/label.h>
    <BALL/VIEW/PRIMITIVES/simpleBox.h>
    <BALL/VIEW/PRIMITIVES/box.h>
    <BALL/VIEW/PRIMITIVES/point.h>
    <BALL/VIEW/PRIMITIVES/line.h>
    <BALL/VIEW/DATATYPE/standardDatasets.h>
    <BALL/SYSTEM/path.h>
    <BALL/KERNEL/system.h>
    <BALL/KERNEL/PTE.h>
    <BALL/KERNEL/forEach.h>
    <BALL/KERNEL/bond.h>
)
```

**Dropped candidates:**
- `<BALL/VIEW/WIDGETS/scene.h>` — recently changed in Phase 02 + Phase 05.1; churning.
- `<BALL/KERNEL/atom.h>` — same churn reason as BALL target.
- Qt headers (`<QtWidgets/Q*>`) — excluded due to MOC macro-ordering risk; see note above.

**Why PRIVATE:** Downstream consumers of `libBALL` and `libVIEW` (PyBALL, test targets,
BALLView executable) must NOT inherit the PCH state. `PRIVATE` keeps the precompiled
header as an internal implementation detail of each library target. This avoids PCH
dependency-chain propagation to targets that should compile independently.

**BALLView executable is deliberately NOT PCH'd:** Its TU count (~20-30 application
`.C` files) is small enough that PCH overhead (PCH file generation) would not recoup
itself in build-time savings. Only the large-TU library targets (`BALL`, `VIEW`) benefit.

---

## ccache Sloppiness Contract

For `cmake`-native precompiled headers to share ccache hits across CI runs, ccache
must be configured with the following sloppiness flags:

```
pch_defines,time_macros,include_file_mtime
```

Set via the `CCACHE_SLOPPINESS` environment variable in `.github/workflows/ci.yml`
(single source of truth, no per-platform shell quoting drift):

```yaml
env:
  # Phase 999.16 (BUILD-ACCEL-01): ccache + PCH compatibility.
  # Without these three keys, PCH-enabled builds silently bust ccache hits:
  # pch_defines     — accept hits when macro state at PCH creation matches
  # time_macros     — accept hits when __DATE__/__TIME__/__TIMESTAMP__ differ only
  # include_file_mtime — accept hits based on content hash, not mtime (CI runners
  #                      reset mtimes on every checkout)
  CCACHE_SLOPPINESS: "pch_defines,time_macros,include_file_mtime"
```

### Why each flag is required

| Flag | What it does | Without it |
|---|---|---|
| `pch_defines` | Accept cache hits when the macro state at PCH creation matches | Every TU that uses the PCH must have identical macro state including compiler-injected defines — routinely fails across runners |
| `time_macros` | Accept hits when the only difference is `__DATE__`/`__TIME__`/`__TIMESTAMP__` | Build timestamps embedded in macros bust every cache hit on every run |
| `include_file_mtime` | Accept hits based on content hash rather than mtime | CI runners reset mtimes on every `git checkout`; without this flag, ccache treats every file as modified even when content is identical |

**Combined effect without sloppiness:** PCH-enabled builds frequently bust ccache hits
silently — the build still succeeds, but is 2-3× slower than non-PCH because ccache
falls back to full compilation on every cache miss.

---

## When to Re-audit

Trigger a re-audit (re-run the commands in "Re-audit Commands") and update the tables
in this file when any of these conditions occur:

1. **Heavy header added to BALL/COMMON or BALL/CONCEPT** — a new template-heavy
   utility header that lands in the transitive include of many TUs.
2. **Qt major version bump (Qt 6.x → Qt 7.x)** — Qt's PCH behavior and header
   layout may change across major versions.
3. **Boost major version bump** — Boost headers that appear transitively may change
   enough to warrant re-checking inclusion depth.
4. **New heavy template-library dependency** — e.g., range-v3, abseil, or a molecular
   dynamics library with large template headers.
5. **The churning headers stabilize** — re-check `BALL/KERNEL/atom.h` and
   `BALL/VIEW/WIDGETS/scene.h` after 3 months of no changes; they may become
   PCH-eligible again.
6. **Cold-build performance regresses** — if a future CI run shows cold-cache
   Build times creeping up, re-audit the PCH set for drift (headers removed but
   still in the PCH list waste PCH compilation time).

**How to update:** Re-run the audit commands above, fill in a new results table
with the new date, update the "Chosen PCH header sets" section, and add a changelog
entry at the bottom of this file.

---

## References

- `CMakeLists.txt` — `target_precompile_headers` wiring (after the CUDA/non-CUDA block)
- `.github/workflows/ci.yml` — `CCACHE_SLOPPINESS` environment variable
- Phase 999.2 SUMMARY (`.planning/phases/999.2-ninja-generator-switch/999.2-SUMMARY.md`)
  — the cold/warm Build baseline used as the measurement reference
- CMake docs: [`target_precompile_headers`](https://cmake.org/cmake/help/latest/command/target_precompile_headers.html)
- ccache docs: [Precompiled headers](https://ccache.dev/manual/latest.html#_precompiled_headers)
- ROADMAP §999.16 — acceptance thresholds (≥15% Windows cold, ≥10% Linux/macOS cold)
