---
phase: 999.25
plan: 01
type: execute
wave: 3
autonomous: true
depends_on: [01, 02, 02.1, 02.2, 03, 04, 04.1, 05, 05.1, 08a, 08b, 08c, 999.2, 999.7, 999.8, 999.17, 999.19, 999.20, 999.22, 999.27, 999.28, 999.29, 999.37]
files_modified:
  - .planning/phases/999.25-v1.4-to-v1.7-retrospective/RETROSPECTIVE.md  # NEW
  - .planning/phases/999.25-v1.4-to-v1.7-retrospective/PLAN.md           # NEW (this file)
requirements:
  - DOC-V17-RETRO-01  # v1.4 → v1.7 retrospective document published
must_haves:
  truths:
    - "RETROSPECTIVE.md exists at .planning/phases/999.25-v1.4-to-v1.7-retrospective/RETROSPECTIVE.md and is self-contained"
    - "Document covers (1) toolchain/dep floor bumps, (2) removed/disabled subsystems with rationale, (3) new capabilities, (4) unresolved risk/known-issues survival, (5) cross-references by phase number"
    - "Audience framing explicit: maintainers + downstream packagers + curious users; factual tone, not promotional"
    - "Cross-references to ROADMAP phase numbers + per-phase SUMMARY.md paths; readers can drill from retrospective into source-of-truth"
    - "Baseline framing acknowledges v1.4 was the last freshly-documented release era (~2002-2010 CVS) — there is no `v1.4` git tag in this repo; the comparison is conceptual"
  artifacts:
    - path: ".planning/phases/999.25-v1.4-to-v1.7-retrospective/RETROSPECTIVE.md"
      provides: "single-document retrospective covering v1.4 → v1.7 modernization arc"
      contains: "Toolchain floor|Removed/disabled|New capabilities|Risk survival|Cross-references"
---

<objective>
Produce a single retrospective document at
`.planning/phases/999.25-v1.4-to-v1.7-retrospective/RETROSPECTIVE.md`
covering everything that changed between BALL v1.4 (the last properly
documented release, ~2002-2010 CVS era) and v1.7 (the
about-to-ship modernization).

The document is the meta-artifact that lets future maintainers and
downstream packagers see the full arc at a glance, then drill into
the ROADMAP + per-phase SUMMARY.md files for source-of-truth detail.
This plan is doc-only; no source code touched, no build runs.
</objective>

<context>
Inputs are already in-tree:
- @.planning/ROADMAP.md (2083 lines — authoritative phase log)
- @.planning/v1.7-PLAN.md (242 lines — wave structure + risk register)
- .planning/phases/*/SUMMARY.md (per-plan execution records)
- @BUILD-macos.md + @BUILD-linux.md (user-facing build guides; no
  BUILD-windows.md exists yet — call it out as a gap)
- @RELEASE-NOTES-v1.6.1.md + @RELEASE-NOTES-v1.6.2.md (already-written
  user-facing notes for the two patch releases)
- @.planning/MILESTONE-CONTEXT.md (v1.6.2 scope)
- @.planning/PROJECT.md (project framing)
- @.planning/seeds/SEED-001-ballview-refresh-ui-milestone.md (UI refresh
  source)
- git log v1.6.0..HEAD (samples; do NOT enumerate all commits)
  (Note: v1.4 has no git tag in this repo. v1.6.0 is the earliest tag.)
</context>

<tasks>
  <task id="1" type="auto">
    <name>Author RETROSPECTIVE.md</name>
    <behavior>
      A maintainer reading the document understands, in order: what changed
      at a glance (toolchain/deps/platforms/packaging), what was removed
      and why (each item paired with rationale), what is new (auto-update,
      UI refresh, CI matrix, appcast), what unresolved risks survive into
      v1.7, and how to drill down (cross-references by phase number).
    </behavior>
    <implementation>
      Write the document with these top-level sections:
      1. Framing (what this is, audience, baseline-era caveat)
      2. What changed at a glance (toolchain/dep floor + platforms + packaging table)
      3. Removed or disabled subsystems (table: subsystem, rationale, phase ref)
      4. New capabilities (auto-update, BALL_UI_V2 refresh, CI matrix, appcast hosting, dual-SKU Linux packaging)
      5. Risk and known-issues survival (open phases, active seeds, GPL-FFTW gap, bus-factor on signing)
      6. Cross-references (phase index by number with one-line summary + path)
      Tone: factual. No marketing copy. No emoji.
    </implementation>
    <files>
      <file path=".planning/phases/999.25-v1.4-to-v1.7-retrospective/RETROSPECTIVE.md" action="create" />
    </files>
    <verification>
      File exists, is non-empty, contains all five required section
      headings, contains at least 10 phase-number cross-references
      (e.g., `Phase 02.1`, `Phase 999.37`, `Phase 999.29`).
    </verification>
    <done>
      RETROSPECTIVE.md exists; the 6 must_have truths are all satisfied
      against its contents.
    </done>
  </task>
</tasks>

<verification>
- `[ -f .planning/phases/999.25-v1.4-to-v1.7-retrospective/RETROSPECTIVE.md ]` exits 0
- Document contains the 5 mandatory section headings
- Document references at least 10 distinct phase numbers from the ROADMAP
- Document acknowledges the missing BUILD-windows.md gap (per inputs)
</verification>

<success_criteria>
1. RETROSPECTIVE.md published at the path above.
2. Self-contained — a reader who has not read ROADMAP can still follow.
3. Cross-references resolve (ROADMAP phase numbers all exist in
   .planning/ROADMAP.md).
4. Tone is factual; the doc could plausibly be linked from a public
   release page without rewrite.
</success_criteria>
