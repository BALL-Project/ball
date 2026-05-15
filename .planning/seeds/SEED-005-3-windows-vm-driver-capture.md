---
id: SEED-005-3
status: dormant
planted: 2026-05-15
planted_during: BALLView 1.6 Modernization · Phase 5 (Qt 6 + Renderer Backend Spike) · Plan 05-07
trigger_when: SEED-005-2 resolved (Windows CI green) OR user obtains Windows hardware/VM
scope: Medium (1× Windows VM setup OR 1× CI artifact-upload step + manual log inspection)
---

# SEED-005-3: Windows driver-behaviour capture for SPIKE-02

## Why This Matters

Phase 5's SPIKE-01 success criterion 4 calls for "macOS (Apple Silicon) **and
Windows** driver behaviour are checked and recorded." The macOS rows of
`.planning/phases/05-.../05-DRIVER-BEHAVIOUR.md` are live-captured (M4 Max
local + GitHub macos-latest software runner). The Windows row is a GAP.

Resolving this seed lets Plan 08 (SPIKE-02 decision record) cite a real
multi-OS dataset rather than "macOS-only + Windows-deferred-as-limitation."
The CLAUDE.md Core Value is explicit: "BALLView must build and visibly render
on macOS, Linux, **and Windows**" — a SPIKE-02 backend decision based only on
macOS data is incomplete by project-charter definition.

## Empirical Gaps This Resolves

Per `.planning/phases/05-.../05-DRIVER-BEHAVIOUR.md` §Gap 3 — Windows
hardware/VM driver capture. Specifically:

- Live `BALLVIEW_GL_DIAG` from a Windows host (Intel iGPU on CI OR
  user-owned NVIDIA/AMD box).
- Picking + text overlay click-through validation on Windows (currently
  blocked on the spike paths by PIPE-01 anyway — see Gap 4 — but should
  be validated on the **default** non-spike Qt 6 GL path as the Phase 2
  cross-OS regression confirmation).
- ANGLE-removal verification (Qt 6 removed ANGLE; Windows GL path should
  be direct OpenGL32 — verifiable via `gl_vendor` + `gl_renderer` strings
  in the DIAG line; an ANGLE-mediated context would report something like
  `gl_vendor="Google Inc."`, a native one reports `gl_vendor="Intel"` /
  `"NVIDIA"` / `"AMD"`).

## Proposed Resolution Paths

### Path A — CI-driven capture (preferred, no Windows hardware required)

Pre-requisite: **SEED-005-2 resolved** (Windows CI default build green).

Edit `.github/workflows/ci.yml`. Add to the `build (windows-x64)` matrix
entry, after the `Build (Windows)` step:

```yaml
- name: Render smoke check (Windows — D-06 / D-08)
  if: matrix.smoke && matrix.os == 'windows-2022'
  shell: pwsh
  run: |
    $exe = "build\ci-windows\bin\BALLView.exe"
    $env:BALL_DATA_PATH = "$PWD\data"
    $env:BALLVIEW_GL_DIAG = "1"
    # Use Start-Process with -PassThru + Wait-Process -Timeout
    $proc = Start-Process -FilePath $exe -ArgumentList @(
      "-export-png", "$env:TEMP\windows-render-smoke.png",
      "-load", "data\structures\bpti.pdb"
    ) -RedirectStandardOutput "$env:TEMP\ballview-smoke.log" -RedirectStandardError "$env:TEMP\ballview-stderr.log" -PassThru
    Wait-Process -Id $proc.Id -Timeout 60 -ErrorAction SilentlyContinue
    if (!$proc.HasExited) { Stop-Process -Id $proc.Id -Force }
    Get-Content "$env:TEMP\ballview-smoke.log" | Select-String "BALLVIEW_GL_DIAG"

- name: Upload Windows DIAG artifact
  if: matrix.os == 'windows-2022'
  uses: actions/upload-artifact@v4
  with:
    name: windows-ballview-diag
    path: |
      ${{ runner.temp }}\ballview-smoke.log
      ${{ runner.temp }}\ballview-stderr.log
      ${{ runner.temp }}\windows-render-smoke.png
```

After the next CI run, fetch the artifact via `gh run download <run-id>
-n windows-ballview-diag` and paste the captured `BALLVIEW_GL_DIAG` line
into the Windows row of `05-DRIVER-BEHAVIOUR.md`. Update the Plan 08
(SPIKE-02) row from GAP to ✓.

### Path B — Local Windows VM (if CI Path A produces inconclusive results)

Pre-requisite: User has VM software (Parallels / UTM / VMware) on the M4
Max host OR a separate Windows test box.

Procedure:

1. Provision a Windows 11 ARM64 or x86_64 VM.
2. Install Qt 6.5+ via vcpkg manifest OR Qt online installer.
3. Build BALLView per `BUILD-windows.md` (if it exists) or the
   `cmake --preset ci-windows` flow.
4. Run BALLView interactively with a demo molecule. Capture:
   - The full `BALLVIEW_GL_DIAG` line from stdout.
   - A screenshot of the text overlay rendering correctly.
   - A click-and-pick interaction confirming picking returns a non-empty
     object list (verifiable via the selection panel OR a stderr log line).
5. Paste verbatim into `05-DRIVER-BEHAVIOUR.md` §Picking + Text Overlay
   Verification.

### Path C — User runs on personal Windows box

Pre-requisite: User has a Windows machine with build toolchain (or accepts
binary distribution from a Path A green CI run).

If only the binary is available (no rebuild needed locally), run with:

```powershell
$env:BALL_DATA_PATH = "C:\path\to\ball\data"
BALLView.exe 2>&1 | Tee-Object -FilePath ballview-windows.log
# Open a demo molecule, click an atom, check the selection panel
# Paste ballview-windows.log lines containing "BALLVIEW_GL_DIAG" into
# the driver-behaviour artifact's Windows row
```

## Verification

After resolution:

- `.planning/phases/05-.../05-DRIVER-BEHAVIOUR.md` Windows row in the Test
  Matrix moves from `✗ (vcpkg baseline)` / `—` to `✓` (default build) +
  the Picking/Overlay row gets `yes/yes`.
- Plan 08 (SPIKE-02) cites Windows driver behaviour as live data, not as
  a gap.

## Suggested Home

Path A: Phase 5.1 hotfix (depends on SEED-005-2). Path B: Carry-forward
to PIPE-01's Phase opening (when cross-OS validation gates the pipeline
swap anyway). Path C: opportunistic — anytime user has Windows access.

Estimated effort: Path A is 1-2 hours (CI step authoring + 2 CI cycles
to converge). Path B is half a day (VM provisioning + build chain).
Path C is 30 minutes if the binary already exists.
