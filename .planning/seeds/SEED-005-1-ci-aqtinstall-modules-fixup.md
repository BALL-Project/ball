---
id: SEED-005-1
status: dormant
planted: 2026-05-15
planted_during: BALLView 1.6 Modernization · Phase 5 (Qt 6 + Renderer Backend Spike) · Plan 05-07
trigger_when: Phase 5 complete OR a Phase 5.1 CI-fixup wave is scheduled
scope: Small (~1 line of ci.yml)
---

# SEED-005-1: CI Linux Qt 6 install — aqtinstall module-name fixup

## Why This Matters

Plan 05-04 wired `jurplel/install-qt-action@v4` (which wraps aqtinstall) into the
Linux CI matrix entry to satisfy D-01 (Qt 6.5+ floor), since Ubuntu 24.04 apt
ships only Qt 6.4.2. The action step specifies:

```yaml
modules: qtbase qttools qtopengl
```

This was the canonical aqt module set as of the original research. But CI run
**25910030073** on 2026-05-15 surfaced the following failure at the Qt-install
step:

```
ERROR : The packages ['qtbase', 'qtopengl', 'qttools'] were not found while
parsing XML of package information!
```

The aqt module-name convention has churned between `aqtversion: ==3.3.*`
(currently pinned in the workflow) and the original ~3.1 version this set was
written for. `qtbase` is the default-included module set on Linux desktop in
modern aqt — listing it explicitly is what causes the lookup to fail.

This is the **load-bearing CI blocker** for Plan 05-07's Gap 1 (Linux
default-build smoke under Qt 6.5+) — Linux CI cannot reach `cmake configure`
until the install step works.

## Empirical Gap This Resolves

Per `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-DRIVER-BEHAVIOUR.md`
§Gap 1 — Linux default-build smoke. Resolving this seed unblocks:

- A live `BALLVIEW_GL_DIAG` from Linux Mesa/llvmpipe.
- The Plan 05-04 `Assert GL compat profile (macOS — D-06 / D-08)` step's Linux
  equivalent — currently the Linux smoke step never fires.
- Plan 08 (SPIKE-02) per-platform driver-behaviour table: the Linux row could
  move from GAP to ✓.

## Proposed Fix (one line)

Edit `.github/workflows/ci.yml`. Locate the Linux matrix entry's
`Install Qt 6 (Linux — jurplel/install-qt-action / aqtinstall)` step. Try
each in order until aqt accepts the input:

1. **Drop the modules field entirely**:
   ```yaml
   - name: Install Qt 6 (Linux — jurplel/install-qt-action / aqtinstall)
     uses: jurplel/install-qt-action@v4
     with:
       version: '6.5.*'
       host: linux
       target: desktop
       # modules: qtbase qttools qtopengl  <-- DELETE this line
       cache: true
   ```
   Rationale: `qtbase` is the default-included module on Linux desktop in aqt
   3.3.x; `qtopengl` was folded into qtbase; `qttools` may be unnecessary if
   the CMake `Qt6LinguistTools` find is optional.

2. **OR retain only the genuinely-needed extras**: keep `modules: qttools`
   only (and only if `Qt6LinguistTools_FOUND` is required at configure time).
   If the optional `Qt6LinguistTools` is not load-bearing for the Linux CI
   build, option (1) is preferred.

3. **OR bump aqtversion to a known-stable pin**: `aqtversion: ==3.1.*` if the
   3.3.x metadata format genuinely differs. Less preferred — pins to an older
   tool.

## Verification

After the fix lands:

```bash
gh workflow run ci.yml --ref <branch>
gh run watch <run-id>
```

Expected: `build (linux-x64)` job's `Install Qt 6` step completes green; the
`Configure (Linux)` step runs; the `Build (Linux)` step builds BALLView green;
the `Render smoke check (Linux — xvfb + software-Mesa)` step emits a
`BALLVIEW_GL_DIAG` line containing `gl_vendor="Mesa"` (or `"llvmpipe"`).

The Plan 05-04 `Assert GL compat profile` step on Linux is expected to PASS
(unlike macOS, Linux Mesa exposes the Core/Compat distinction at GL 3.0+, so
`gl_profile=compatibility` should be emitted).

## Suggested Home

Phase 5.1 (build warnings + latent bugs cleanup) — its scope already includes
"CI noise / churn" items. Alternative: a dedicated 1-task hotfix plan.

Estimated effort: <30 minutes (1-line yaml edit + 1 CI run-and-watch cycle).
