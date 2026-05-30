# Phase 02.2 — Deferred / Out-of-Scope Items

Items discovered during execution that are outside the current plan's scope.

## From Plan 02.2-02 (CI workflow)

### The legacy-GL grep gate exits 1 on the current tree (lint job will be RED)

- **Discovered during:** Plan 02.2-02 Task 2 (local sanity check of the lint job).
- **Plan expectation:** Task 2's acceptance criteria stated
  `check-no-legacy-gl-symbols.sh "$PWD"` exits 0 ("the Phase 2 port already
  removed the legacy symbols"). It does NOT — it exits 1 with 7 hits.
- **Breakdown of the 7 hits:**
  - **1 real reference:** `include/BALL/VIEW/RENDERING/glDisplayList.h:13` —
    `#include <QtOpenGL/qgl.h>` (a legacy Qt4-era header). `glDisplayList.C`
    *is* compiled (listed in `source/VIEW/RENDERING/sources.cmake`), so this is
    a genuine surviving legacy include the Phase 2 port missed.
  - **6 false positives:** historical *comments* in `glRenderWindow.C` (×2),
    `scene.C` (×3), and `scene.h` (×1) that mention the word `QGLWidget` while
    explaining what changed (e.g. "unlike the old QGLWidget which created it
    eagerly"). The grep gate's `LEGACY_PATTERN` is a bare substring match with
    no comment-stripping, so prose mentioning the old class name trips it.
- **Why deferred (not auto-fixed here):** This is out of scope for the CI
  workflow plan (02.2-02 only creates `.github/workflows/ci.yml`). Fixing it
  touches Phase 2 source (`glDisplayList.h`) and/or the Phase 2 grep script
  (`check-no-legacy-gl-symbols.sh`) — neither is in this plan's `files_modified`.
  It is a pre-existing condition from Phase 2 / 02.1, not caused by 02.2-02.
- **Impact on the checkpoint:** The `lint` job in `ci.yml` is wired correctly
  and IS blocking — so it WILL go red on the first CI run until this is
  resolved. The macOS + Linux build/smoke jobs are unaffected.
- **Recommended resolution (for the checkpoint / a follow-up quick task):**
  1. Port `glDisplayList.h` off `<QtOpenGL/qgl.h>` (or confirm `glDisplayList`
     is dead code and drop it) — the real fix.
  2. Tighten `check-no-legacy-gl-symbols.sh` so it ignores `//`-comment matches
     (or scope the symbol patterns to `#include`/type-usage contexts) so prose
     referencing the old class name doesn't trip the gate.
  Either a Phase 2 follow-up quick task or an explicit decision at the 02.2-02
  checkpoint. Tracked here so the workflow plan stays scoped to the workflow.
