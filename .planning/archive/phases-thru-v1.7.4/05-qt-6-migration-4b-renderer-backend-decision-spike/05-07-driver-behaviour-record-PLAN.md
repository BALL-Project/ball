---
phase: 05-qt-6-migration-4b-renderer-backend-decision-spike
plan: 07
type: execute
wave: 6
depends_on: [04, 05, 06]
files_modified:
  - .planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-DRIVER-BEHAVIOUR.md
autonomous: false
requirements: [SPIKE-01]
must_haves:
  truths:
    - "Per-platform driver behaviour is recorded for macOS Apple Silicon (compat profile + GL-core spike + QRhi/Metal spike) AND Windows (compat profile + GL-core spike) — SPIKE-01 criterion 4 deliverable"
    - "D-07: the macOS GL-deprecation warning text is captured verbatim from a live macOS run — SPIKE-02 (Plan 08) cites it as a 'known liability' motivating PIPE-01"
    - "BALLVIEW_GL_DIAG stdout lines from each platform×backend combination are saved in the artifact"
    - "Spike-mode picking + text overlay are confirmed by a human running the demo molecule on each platform — the manual-only validations from 05-VALIDATION.md §Manual-Only Verifications"
  artifacts:
    - path: ".planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-DRIVER-BEHAVIOUR.md"
      provides: "Per-platform driver behaviour table consumable by Plan 08's decision record"
      contains: "macOS Apple Silicon"
      min_lines: 80
  key_links:
    - from: "05-DRIVER-BEHAVIOUR.md"
      to: "BALLVIEW_GL_DIAG raw lines for each platform×backend"
      via: "fenced-code block per cell"
      pattern: "BALLVIEW_GL_DIAG"
    - from: "05-DRIVER-BEHAVIOUR.md macOS GL deprecation row"
      to: "Verbatim deprecation banner text"
      via: "captured stdout/stderr from a live macOS launch"
      pattern: "OpenGL is deprecated|deprecation"
---

<objective>
Collect the per-platform driver-behaviour observations that SPIKE-02 (Plan 08) needs to write a defensible decision. SPIKE-01 success criterion 4 explicitly says "macOS (Apple Silicon) and Windows driver behaviour are checked and recorded" — this plan is where that recording happens.

Purpose: A backend decision based only on macOS data is incomplete because the entire point of the BALL/v1.6 cross-platform constraint (CLAUDE.md "Core Value") is "build and visibly render on macOS, Linux, and Windows". Plan 07 captures the data table — GL vendor / renderer / version strings, profile, MSAA samples, framebuffer format, deprecation warning text, QRhi backend name — for each platform×backend combination. Plan 08 then writes the decision.

Output: One markdown artifact (`05-DRIVER-BEHAVIOUR.md`) consumable by Plan 08. This plan contains a `checkpoint:human-verify` task because the per-platform runs require physical or VM access to Apple Silicon AND Windows hardware/VMs that Claude cannot directly drive headlessly — the manual-only validations in 05-VALIDATION.md.
</objective>

<execution_context>
@$HOME/.claude/get-shit-done/workflows/execute-plan.md
@$HOME/.claude/get-shit-done/templates/summary.md
</execution_context>

<context>
@.planning/STATE.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-CONTEXT.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-RESEARCH.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-PATTERNS.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-VALIDATION.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-05-SUMMARY.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-06-SUMMARY.md

<interfaces>
<!-- Plan 07 produces a markdown artifact. Schema is fixed below. -->

05-DRIVER-BEHAVIOUR.md schema (per 05-RESEARCH.md §Per-Platform macOS / Windows Driver Behaviour Checks):

# Phase 5 — Per-Platform Driver Behaviour Record

**Captured:** YYYY-MM-DD
**Purpose:** Inputs for SPIKE-02 (Plan 08) decision record.

## Test Matrix

| Platform | Backend | Captured | Notes |
|----------|---------|----------|-------|
| macOS Apple Silicon | Compat profile (default) | ✓/✗ | |
| macOS Apple Silicon | GL-Core spike | ✓/✗ | |
| macOS Apple Silicon | QRhi spike (Metal) | ✓/✗ | |
| Windows (CI runner Intel iGPU OR user box) | Compat profile (default) | ✓/✗ | |
| Windows | GL-Core spike | ✓/✗ | |
| Linux (CI software Mesa) | Compat profile (default) | ✓/✗ | |
| Linux | GL-Core spike | ✓/✗ | |

## BALLVIEW_GL_DIAG Captures

For each cell with ✓ above, paste the verbatim `BALLVIEW_GL_DIAG ...` stdout line in a fenced code block under a subheading.

### macOS Apple Silicon — Compat profile (default)
```
BALLVIEW_GL_DIAG gl_vendor=... gl_renderer=... gl_version=... gl_profile=compatibility fbo_size=... samples=4 ...
```

### macOS Apple Silicon — GL-Core spike
```
BALLVIEW_GL_DIAG gl_vendor=... gl_renderer=... gl_version=4.1 ... gl_profile=core ...
```

(... and so on for each row ...)

## Capability Notes (per 05-RESEARCH §Per-Platform Checks)

### macOS Apple Silicon
- GL vendor / renderer expected: `Apple` / `Apple M*`. Actual: <fill in>
- Compat profile GL version expected: `2.1 Apple-...`. Actual: <fill in>
- Core profile GL version expected: `4.1 ...`. Actual: <fill in>
- Deprecation banner (D-07) — verbatim text captured from a live launch: <paste here>
- MSAA samples reported: 4. Actual: <fill in>
- Default FBO non-zero: yes/no.
- QRhi backend name (from QRhiWidget `rhi()->backendName()` or QSG_INFO=1 trace): `Metal` expected.

### Windows
- GL vendor / renderer (windows-2022 runner: Intel HD iGPU): <fill in>
- Compat profile GL version: <fill in>
- Core profile GL version: <fill in>
- ANGLE removed in Qt 6 confirmation: the GL path is OpenGL32 (not ANGLE).

### Linux (CI software Mesa)
- GL vendor: `Mesa/X.org` expected. Actual: <fill in>
- GL profile: `compatibility` expected (D-06 default). Actual: <fill in>
- llvmpipe core profile version (when GL-core spike forced): `3.3+`. Actual: <fill in>

## Picking + Text Overlay Verification

| Platform | Backend | Picking returns non-empty list | Text overlay legible |
|----------|---------|--------------------------------|---------------------|
| macOS Apple Silicon | GL-Core spike | yes/no | yes/no |
| macOS Apple Silicon | QRhi spike | N/A (deferred caveat) | yes/no |
| Windows | GL-Core spike | yes/no | yes/no |

Attach screenshots in `05-DRIVER-BEHAVIOUR/screenshots/` (subdirectory of this plan's phase dir). Reference each from the table row.

## Acquired Live Outputs

- macOS deprecation banner full text: <paste here>
- Any unexpected console warnings (compat profile under Qt 6 6.5+): <paste here>
- QRhi shader compilation warnings (if any): <paste here>

---

Capture procedure (one-liner per row):
- macOS local: `<binary>` launched manually + DIAG line scraped from stdout; deprecation banner captured from Console.app or `stderr`.
- Linux: CI run logs (jurplel/install-qt-action job's smoke-check step + spike variants).
- Windows: CI run logs (windows-x64 job's smoke-check step, if matrix.smoke is true on Windows — Phase 02.2 left it on macOS+Linux; for Windows, the executor may need to extend the CI matrix temporarily OR ask the user to run on a Windows machine).

If any cell cannot be captured (no Windows hardware / Apple Silicon access / etc.), leave it as ✗ with a one-line rationale; Plan 08 then explicitly notes the data gap in the decision record. Do NOT fake data.
</interfaces>
</context>

<tasks>

<task type="auto" tdd="false">
  <name>Task 1: Scaffold 05-DRIVER-BEHAVIOUR.md and capture all CI-reachable rows</name>
  <files>.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-DRIVER-BEHAVIOUR.md</files>
  <read_first>
    - 05-VALIDATION.md §Manual-Only Verifications (the 4 manual rows + their test instructions).
    - 05-RESEARCH.md §Per-Platform macOS / Windows Driver Behaviour Checks (the table of "Check / How to Verify / What to Record" rows — these become the template's column headers).
    - 05-05-SUMMARY.md and 05-06-SUMMARY.md (the immediate-upstream artifacts: spike build outcomes, CI artifact URLs to fetch BALLVIEW_GL_DIAG lines from).
    - The latest CI run logs reachable via `gh run list --workflow=ci.yml --limit=5` and `gh run view --log <run-id>` — capture the BALLVIEW_GL_DIAG lines from the macOS + Linux smoke-check jobs (both default and spike variants).
  </read_first>
  <behavior>
    - The artifact exists and follows the schema in `<interfaces>` (matrix table + per-row BALLVIEW_GL_DIAG fenced code blocks + capability-notes section + picking/overlay verification table + acquired-live-outputs section).
    - Every row that CAN be captured from CI logs (macOS default, macOS GL-core spike, Linux default, Linux GL-core spike if Plan 05's CI step is non-blocking-green) is filled in with verbatim BALLVIEW_GL_DIAG lines, not summarised.
    - Every row that requires manual physical access (Windows runs, QRhi Metal verification of "backendName=Metal", deprecation banner full text from Console.app, picking interactive demo) is left as ✗ with a one-line note pointing to Task 2's manual checkpoint.
    - The "macOS deprecation banner" cell is the SPIKE-02-quoted "known liability" — even if the full Console.app text isn't yet captured, the captured `qDebug`/`qWarning` lines from the CI macOS job's smoke-check stdout that contain the substring `deprecat` are pasted as a partial capture, with a TODO for the manual full-banner capture.
  </behavior>
  <action>
    Create `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-DRIVER-BEHAVIOUR.md` from the schema in `<interfaces>`. Use real `gh` CLI fetches to populate CI-reachable rows:

    1. Run `gh run list --workflow=ci.yml --branch=$(git rev-parse --abbrev-ref HEAD) --limit 5` to find the most recent run.
    2. For each platform (macos-arm64, linux-x64, windows-x64), invoke `gh run view --log <run-id> --job <job-id>` and `grep -E 'BALLVIEW_GL_DIAG|deprecat' <log>` to extract the diagnostic lines from the regular smoke-check step. Paste them verbatim into the matching row.
    3. Same for the spike variant step (macOS only per Plan 05): extract the GL-Core spike DIAG line and any QRhi DIAG line if Plan 06's CI step exists.
    4. For QRhi backend name, the `rhi()->backendName()` is emitted by Plan 06's `QRhiRenderer::init()` (verify in 05-06-SUMMARY). If not emitted, note as a Task 2 manual-capture gap.

    Do NOT fabricate values. Do NOT mark a row ✓ unless the verbatim line is pasted into the corresponding fenced code block. Cells with no live capture stay ✗ with a 1-line rationale (e.g., "✗ — Windows CI smoke not enabled in matrix; needs ⓜ checkpoint").

    Set status flags at the bottom: `wave_0_complete: <true|false depending on which Manual-Only rows still need Task 2>`.
  </action>
  <verify>
    <automated>test -f /Users/kohlbach/Claude/BALL/ball/.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-DRIVER-BEHAVIOUR.md &amp;&amp; grep -q '# Phase 5 — Per-Platform Driver Behaviour Record' /Users/kohlbach/Claude/BALL/ball/.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-DRIVER-BEHAVIOUR.md &amp;&amp; grep -cE '^### (macOS|Windows|Linux)' /Users/kohlbach/Claude/BALL/ball/.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-DRIVER-BEHAVIOUR.md | grep -qE '^[3-9]' &amp;&amp; grep -c 'BALLVIEW_GL_DIAG' /Users/kohlbach/Claude/BALL/ball/.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-DRIVER-BEHAVIOUR.md | grep -qE '^[2-9]'</automated>
  </verify>
  <acceptance_criteria>
    - File exists at the artifact path.
    - File contains the H1 `# Phase 5 — Per-Platform Driver Behaviour Record`.
    - File contains at least 3 `### <Platform> — <Backend>` subheadings (macOS compat + macOS GL-core spike at minimum; Linux default usually CI-reachable too).
    - File contains at least 2 verbatim `BALLVIEW_GL_DIAG ...` lines pasted from CI logs (macOS compat + Linux compat are the baseline expectations).
    - Cells with no live data are marked ✗ with a 1-line rationale (no fabrication).
    - Min-line check: `wc -l 05-DRIVER-BEHAVIOUR.md` returns `>= 80` (schema is verbose; covers all rows).
  </acceptance_criteria>
  <done>
    The driver-behaviour record is scaffolded; every CI-reachable cell is filled in with verbatim live data; remaining gaps are explicitly marked for the Task 2 manual capture.
  </done>
</task>

<task type="checkpoint:human-verify" gate="blocking">
  <what-built>
    The driver-behaviour record scaffold (Task 1) is filled with all data Claude could fetch from CI logs. Three categories of data still require physical access that Claude can't drive headlessly:
    1. **macOS GL deprecation banner full text** (D-07 quoted-liability for SPIKE-02): launch BALLView on a local macOS Apple Silicon box; capture the deprecation banner text from Console.app or the launch stderr; paste into the "Acquired Live Outputs" section.
    2. **Picking + text overlay interactive verification** (05-VALIDATION.md §Manual-Only): launch the spike build (`BALL_SPIKE_BACKEND=GLCore` build, `BALLVIEW_USE_SPIKE_BACKEND=1` env) with the demo molecule; click an atom; confirm the selection panel (or equivalent log line) reports the picked atom; confirm the text overlay is legible at default DPI. Repeat for the QRhi spike build (`BALL_SPIKE_BACKEND=QRhi`; picking is N/A per Plan 06's documented caveat — text overlay only). Save screenshots under `05-DRIVER-BEHAVIOUR/screenshots/`.
    3. **Windows driver behaviour** (if no Windows CI smoke is enabled or the user has a Windows test box): launch BALLView on a Windows box; capture `BALLVIEW_GL_DIAG`, `gl_vendor`, `gl_renderer`, `gl_version`, picking + text overlay confirmation. If unavailable, mark Windows row as "Data gap — SPIKE-02 will note as limitation."
  </what-built>
  <how-to-verify>
    Open `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-DRIVER-BEHAVIOUR.md`. Verify:
    1. macOS deprecation banner full text is pasted into the "Acquired Live Outputs" → "macOS deprecation banner full text" cell (not the placeholder).
    2. The "Picking + Text Overlay Verification" table has rows for macOS GL-Core spike + QRhi spike + (optionally) Windows GL-Core spike, with yes/no values and screenshot paths.
    3. `05-DRIVER-BEHAVIOUR/screenshots/` exists and contains at least one screenshot of: (a) GL-Core spike rendering the demo molecule with text overlay visible, (b) GL-Core spike picking confirmation (atom selected after click), (c) QRhi spike rendering the demo molecule.
    4. Any Windows rows that could not be captured carry a 1-line "Data gap — ..." note (no fabricated values).

    Run the manual procedure for macOS:
      - `cmake --preset ci-macos -S . -DBALL_SPIKE_BACKEND=GLCore && cmake --build --preset ci-macos --target BALLView`
      - `BALLVIEW_USE_SPIKE_BACKEND=1 build/ci-macos/bin/BALLView.app/Contents/MacOS/BALLView 2>/tmp/macos-glcore-spike.stderr.log` (launch interactively)
      - Open the bundled demo PDB; click an atom; verify the panel; screenshot.
      - Capture `/tmp/macos-glcore-spike.stderr.log` lines containing `deprecat` → paste into the artifact.
      - Repeat for QRhi: `-DBALL_SPIKE_BACKEND=QRhi`, same launch + screenshot (picking skipped per documented caveat).

    Run the manual procedure for Windows (if available):
      - Build on the Windows box with the relevant preset; launch; capture DIAG line + screenshots.

    After all rows filled (or gaps marked), commit the artifact + screenshots.
  </how-to-verify>
  <resume-signal>Type "approved" once 05-DRIVER-BEHAVIOUR.md and the screenshots subdirectory are filled in, or describe what's still missing.</resume-signal>
</task>

</tasks>

<verification>
- `test -f .planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-DRIVER-BEHAVIOUR.md`.
- `grep -c 'BALLVIEW_GL_DIAG' 05-DRIVER-BEHAVIOUR.md >= 4` (compat + spike per platform; bar lowered if Windows unavailable, in which case data-gap note must be present).
- macOS deprecation banner verbatim text present in Acquired Live Outputs (D-07 SPIKE-02 input).
- Screenshots subdirectory exists with at least one image per platform×spike combination.
- All ✗ cells have 1-line rationale; no fabricated ✓ rows.
</verification>

<success_criteria>
- Plan 08 has a real, multi-platform, multi-backend driver-behaviour dataset to cite — not just macOS-only impressions.
- D-07 macOS GL-deprecation banner text is verbatim-captured for use as a SPIKE-02 "known liability" quote.
- Data gaps (Windows / etc.) are explicitly noted, not silently dropped.
</success_criteria>

<output>
After completion, create `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-07-SUMMARY.md` noting which rows were captured live vs left as documented gaps, the screenshot file count, and the verbatim macOS deprecation banner text (one-line excerpt; full text stays in the artifact).
</output>
