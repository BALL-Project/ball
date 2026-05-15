---
id: SEED-005-2
status: dormant
planted: 2026-05-15
planted_during: BALLView 1.6 Modernization · Phase 5 (Qt 6 + Renderer Backend Spike) · Plan 05-07
trigger_when: Phase 5 complete OR a Phase 5.1 CI-fixup wave is scheduled
scope: Small (vcpkg baseline pin OR git-fetch step in ci.yml)
---

# SEED-005-2: CI Windows vcpkg baseline commit not in runner image

## Why This Matters

Plan 05-01 added `qtbase` to the vcpkg manifest (replacing the legacy
`qt5-base`) and pinned `"builtin-baseline":
"c1ce926ddf75f166cbdd444bbaf463410ac99b17"` in `vcpkg.json`. CI run
**25910030073** on 2026-05-15 surfaced the following failure on the
`build (windows-x64)` job at the `Configure (Windows)` step:

```
error: while checking out baseline from commit
'c1ce926ddf75f166cbdd444bbaf463410ac99b17', failed to `git show`
versions/baseline.json. This may be fixed by fetching commits with
`git fetch`.

fatal: path 'versions/baseline.json' exists on disk, but not in
'c1ce926ddf75f166cbdd444bbaf463410ac99b17'

while loading baseline version for boost-any
while loading baseline version for boost-asio
while loading baseline version for boost-bind
[... repeated for every transitive dep ...]
```

The GitHub `windows-2022` runner image's pre-installed vcpkg checkout at
`C:\vcpkg` is a shallow clone (and/or the runner image has rolled forward
past the pinned baseline, dropping the older history that the baseline pin
references). vcpkg cannot resolve `versions/baseline.json` at that commit
because the commit is not reachable in the runner's local clone.

This is the **load-bearing CI blocker** for Plan 05-07's Gap 2 (Windows
default-build smoke under Qt 6) — Windows CI cannot reach `cmake build`
until the vcpkg configure works.

## Empirical Gap This Resolves

Per `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-DRIVER-BEHAVIOUR.md`
§Gap 2 — Windows default-build smoke. Resolving this seed unblocks:

- A live `BALLVIEW_GL_DIAG` from Windows (CI-runner Intel iGPU).
- ANGLE-removal verification (the Qt-6 Windows GL path should be OpenGL32,
  not ANGLE — only verifiable empirically once Windows builds).
- Plan 08 (SPIKE-02) per-platform driver-behaviour table: the Windows row
  could move from GAP to ✓ (subject to also resolving SEED-005-3 if
  interactive picking validation is required).

## Proposed Fixes (in order of preference)

### Option A — Re-pin baseline (cheapest)

Edit `vcpkg.json`. Replace `"builtin-baseline"` with a more recent vcpkg
commit hash that is reachable in the current `windows-2022` runner image's
local vcpkg checkout. Cross-check against:

- The vcpkg tag list at https://github.com/microsoft/vcpkg/tags
- The runner image's vcpkg version recorded in the image release notes:
  https://github.com/actions/runner-images/blob/main/images/windows/Windows2022-Readme.md

Pick the most recent baseline commit that:
1. Is < 30 days old (high probability the runner image has it).
2. Has `qtbase`, `boost-*`, `eigen3`, `tbb`, `libsvm` ports available
   (the BALL vcpkg manifest dep set).

Update `vcpkg.json`, regenerate the cache key (the existing
`vcpkg-windows-x64-...` actions/cache key already hashes vcpkg.json, so the
cache will rebuild on first push — no key change needed in ci.yml).

### Option B — git fetch --unshallow on the runner's vcpkg checkout

Edit `.github/workflows/ci.yml`. Add a step before `Configure (Windows)` on
the Windows matrix entry:

```yaml
- name: Ensure vcpkg baseline commit is reachable
  if: matrix.os == 'windows-2022'
  shell: pwsh
  run: |
    cd $env:VCPKG_INSTALLATION_ROOT
    git fetch --depth=1 origin c1ce926ddf75f166cbdd444bbaf463410ac99b17
```

Rationale: explicit fetch of the pinned commit into the runner's vcpkg clone.
More robust than Option A because future baseline rolls don't require
manifest churn. Downside: adds a per-CI-run network fetch (~seconds).

### Option C — Vendored registry (most robust, most work)

Add a `vcpkg-configuration.json` with a vendored git registry pin. Out of
scope for a hotfix — only consider if Options A/B churn proves unworkable.

## Verification

After the fix lands:

```bash
gh workflow run ci.yml --ref <branch>
gh run watch <run-id>
```

Expected: `build (windows-x64)` job's `Configure (Windows)` step completes
green (vcpkg installs `qtbase`, boost-*, eigen3, etc.); `Build (Windows)`
links `BALLView.exe` green. If a Windows smoke-check step is added (see
SEED-005-3), it emits a `BALLVIEW_GL_DIAG` line with `gl_vendor="Intel"`
or `"NVIDIA"` (whatever the runner image's iGPU/dGPU reports).

## Suggested Home

Phase 5.1 (build warnings + latent bugs cleanup). Alternative: dedicated
1-task hotfix plan. Estimated effort: ~30 minutes for Option A (manifest
re-pin + 1 CI run-and-watch); ~45 minutes for Option B.
