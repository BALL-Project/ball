---
phase: 05-qt-6-migration-4b-renderer-backend-decision-spike
plan: 08
type: execute
wave: 6
depends_on: [05, 06, 07]
files_modified:
  - .planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-SPIKE-DECISION.md
  - .planning/REQUIREMENTS.md
autonomous: false
requirements: [SPIKE-02]
must_haves:
  truths:
    - "SPIKE-02 deliverable artifact (.planning/phases/05-.../05-SPIKE-DECISION.md) exists with all five mandatory sections: Chosen Backend, Rationale, Per-Platform Acceptance Criteria, Scoped PIPE-01 Task List, Known Liabilities"
    - "Chosen backend is named explicitly (GL-Core or QRhi or 'GL-Core for v1.6.x → QRhi for v2'); the rationale references real data from 05-05-SUMMARY.md, 05-06-SUMMARY.md, 05-07-SUMMARY.md, and 05-DRIVER-BEHAVIOUR.md — not abstract opinion"
    - "Per-platform criteria are concrete (e.g., 'macOS Apple Silicon: Metal RHI initialised without error, demo molecule rendered in ≤ 100 ms', not 'works on macOS')"
    - "PIPE-01 task list is scoped — enumerates the leak sites in glRenderer.C (per RENDERER-INTERFACE-BOUNDARY.md), the shader pairs needed, and the order of conversion"
    - "Known Liabilities references D-07 (macOS GL deprecation), Pitfall 7 (QRhi API stability), and any data gaps from 05-DRIVER-BEHAVIOUR.md"
    - "REQUIREMENTS.md SPIKE-01 + SPIKE-02 rows flip to [x] Complete with the file reference"
  artifacts:
    - path: ".planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-SPIKE-DECISION.md"
      provides: "The SPIKE-02 decision record — chosen backend, rationale, per-platform criteria, scoped PIPE-01 task list, known liabilities"
      contains: "Chosen Backend"
      min_lines: 120
    - path: ".planning/REQUIREMENTS.md"
      provides: "SPIKE-01 + SPIKE-02 status flipped to Complete with file ref"
      contains: "SPIKE-02"
  key_links:
    - from: ".planning/phases/05-.../05-SPIKE-DECISION.md"
      to: "Data sources from Plans 05/06/07"
      via: "explicit references to 05-05-SUMMARY.md, 05-06-SUMMARY.md, 05-07-SUMMARY.md, 05-DRIVER-BEHAVIOUR.md"
      pattern: "05-(05|06|07)-SUMMARY|05-DRIVER-BEHAVIOUR"
    - from: ".planning/REQUIREMENTS.md"
      to: "05-SPIKE-DECISION.md"
      via: "Traceability row update"
      pattern: "05-SPIKE-DECISION.md"
---

<objective>
Author the SPIKE-02 decision record — the single artifact this entire phase exists to produce. SPIKE-02 reads "A decision record names the chosen backend, the rationale, per-platform (macOS/Windows) acceptance criteria, and a scoped task list for the `PIPE-01` full rewrite" — this plan delivers exactly that, grounded in the real prototype data from Plans 05-07.

Purpose: PIPE-01 (backlog 999.6) is currently DORMANT pending this decision. Without a real decision record, PIPE-01 stays dormant indefinitely. This plan flips PIPE-01 from "we should do this someday" to "here is the named backend, here is why, here is the task list" — which is the deliverable Codex review explicitly demanded when it split Phase 5 from PIPE-01.

Output: 1 new doc (`05-SPIKE-DECISION.md`) + REQUIREMENTS.md updates to flip SPIKE-01 + SPIKE-02 to Complete with the file reference. Includes a `checkpoint:decision` task because the chosen backend is a user-facing strategic decision (the user is the architect-of-record per CONTEXT.md "spike backend scope" being Claude's Discretion); Claude proposes a recommendation with evidence; user confirms or amends.

This plan is the closeout of Phase 5. After Plan 08, the phase is COMPLETE.
</objective>

<execution_context>
@$HOME/.claude/get-shit-done/workflows/execute-plan.md
@$HOME/.claude/get-shit-done/templates/summary.md
</execution_context>

<context>
@.planning/STATE.md
@.planning/ROADMAP.md
@.planning/REQUIREMENTS.md
@.planning/RENDERER-INTERFACE-BOUNDARY.md
@.planning/phases/999.6-pipe-01-pipeline-rewrite/PIPE-01-BACKLOG.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-CONTEXT.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-RESEARCH.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-PATTERNS.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-VALIDATION.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-05-SUMMARY.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-06-SUMMARY.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-07-SUMMARY.md
@.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-DRIVER-BEHAVIOUR.md

<interfaces>
<!-- SPIKE-02 decision record schema. -->

`.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-SPIKE-DECISION.md` schema:

# SPIKE-02 — Renderer Backend Decision Record

**Phase:** 5 (Qt 6 Migration + Renderer Backend Spike)
**Status:** Locked
**Authored:** YYYY-MM-DD
**Scopes:** PIPE-01 (backlog 999.6, currently DORMANT)
**Supersedes:** RENDERER-INTERFACE-BOUNDARY.md §"Choosing GL-core vs QRhi" out-of-scope note

## 1. Chosen Backend

**Backend:** <GL-Core | QRhi | "GL-Core for 1.6.x; QRhi for v2 (PIPE-01 v2)">

**TL;DR rationale (single paragraph):** ...

## 2. Rationale

Subsections grounded in real data from Plan 05-07 artifacts:

### 2.1 Build + Render Outcomes (from 05-05-SUMMARY, 05-06-SUMMARY)
- GL-Core spike on macOS Apple Silicon: PNG size <bytes>, time to first render <ms>.
- QRhi spike on macOS Apple Silicon (Metal backend): PNG size <bytes>, time to first render <ms>, QRhi backendName: <Metal>.
- Default compat-profile baseline: PNG size <bytes>.

### 2.2 Per-Platform Driver Behaviour (from 05-DRIVER-BEHAVIOUR.md)
Quote 1-2 verbatim BALLVIEW_GL_DIAG lines per platform×backend that informed the call.

### 2.3 Tradeoffs (from 05-RESEARCH §GL-Core vs QRhi)
- GL-Core: + reuses QOpenGLWidget surface; + same shader story across OSes; + picking via color-buffer FBO already implemented in spike; − Qt 6.5+ ABI stable, but per-OS GL quirks remain (macOS GL 4.1 cap, Windows driver variance); − macOS still emits the deprecation banner (D-07).
- QRhi: + single code path (Metal/D3D/Vulkan); + zero GL deprecation on macOS; + future-aligned with Qt's direction; − requires Qt 6.7+ floor (above D-01's 6.5); − ABI not source-stable across Qt minors per Pitfall 7; − picking not implemented in spike — color-buffer pattern needs adaptation to QRhiReadbackResult.

### 2.4 Constraint Compatibility
Map decision to user constraints from CONTEXT.md:
- D-01 (Qt 6.5 LTS minimum) — GL-Core compatible; QRhi adds an implicit 6.7 floor.
- D-07 (macOS GL deprecation visible) — GL-Core preserves; QRhi eliminates.
- v1.6 milestone scope ("build and visibly render on macOS/Linux/Windows") — both backends satisfy in the spike.

## 3. Per-Platform Acceptance Criteria (for PIPE-01)

Concrete numbers / behaviours, not "works":

| Criterion | macOS Apple Silicon | Windows | Linux |
|-----------|--------------------|---------|-------|
| Demo molecule renders | non-blank PNG (smoke check) | non-blank PNG | non-blank PNG (Mesa software OK) |
| Time to first frame | ≤ <ms> p99 | ≤ <ms> p99 | ≤ <ms> p99 |
| Picking returns object at click | non-empty list | non-empty list | non-empty list |
| Text overlay legible at 1× DPI | yes | yes | yes |
| GL deprecation warnings (if GL-Core) | suppress in release builds OR accept (per phase) | N/A | N/A |
| QRhi backend (if QRhi) | Metal | D3D11 or D3D12 | Vulkan or OpenGL fallback |
| Cross-platform unit tests | <count> green | <count> green | <count> green |

## 4. Scoped PIPE-01 Task List

Per RENDERER-INTERFACE-BOUNDARY.md §"What actually leaks" + 05-RESEARCH.md, PIPE-01 must:

1. **Replace `glRenderer.C` fixed-function calls** — list the categories (matrix stack ops; immediate-mode begin/end; light/material state; texture-state; display lists). Cite the line ranges in `glRenderer.C` for each. Estimate task count.
2. **Shader pair authorship** — one vertex+fragment per geometry primitive type used by BALLView (spheres / sticks / surfaces / labels / lines). Enumerate.
3. **Picking adaptation** — color-buffer FBO scheme (GL-Core) or QRhiReadbackResult (QRhi). One task per primitive type's picking code.
4. **Text overlay** — keep QPainter overlay (works for both backends; already proven in Phase 2 + Plan 05).
5. **Tiling / multi-window paths** — Phase 02.1 left 9 stereo/multi-display sites guard-deferred in scene.C; Phase 5 Plan 02 fixed the QDesktopWidget→QScreen surface but PIPE-01 must verify they render under the new backend. Enumerate.
6. **Cross-platform test surface** — enumerate the .C files that must regenerate non-blank PNG across all three OSes.
7. **Migration order** — propose a sequence (smallest blast radius first) so PIPE-01 can ship in atomic plans.

Mark each entry with rough effort indicator (small/medium/large) so a future `/gsd-plan-phase 999.6` can size phases.

## 5. Known Liabilities (for PIPE-01 to resolve OR explicitly accept)

- **D-07 macOS GL deprecation banner** — verbatim text from 05-DRIVER-BEHAVIOUR.md. If chosen backend is GL-Core, this banner persists in v1.6.x and is the standing reminder; PIPE-01-v2-via-QRhi eliminates it.
- **QRhi API source compatibility** (Pitfall 7) — if chosen, PIPE-01 must lock to a Qt minor (6.8 LTS recommended) and revisit on each Qt LTS bump.
- **Windows driver variance** — record what 05-DRIVER-BEHAVIOUR.md found for Intel iGPU GL versions vs NVIDIA; PIPE-01 verifies on both.
- **Spike picking deferred for QRhi** — Plan 06's documented caveat; PIPE-01 must implement.
- **Spike is THROWAWAY** — the spike code (CoreGLRenderer / QRhiRenderer / QtRhiSurface and the BALL_SPIKE_BACKEND option) gets DELETED at PIPE-01 start. PIPE-01's first task is the spike-removal task.

## 6. Decision Authority

Recommended by: Claude (planner).
Accepted by: <user>, YYYY-MM-DD.
Open dissent / amendments: <if any>.

## References

- 05-CONTEXT.md (locked decisions)
- 05-RESEARCH.md (Qt 5→6 delta, GL-Core vs QRhi tradeoffs, per-platform checks)
- 05-PATTERNS.md (spike code patterns)
- 05-05-SUMMARY.md (GL-Core spike outcomes)
- 05-06-SUMMARY.md (QRhi spike outcomes)
- 05-07-SUMMARY.md, 05-DRIVER-BEHAVIOUR.md (per-platform driver record)
- RENDERER-INTERFACE-BOUNDARY.md (the boundary that PIPE-01 swaps behind)
- .planning/phases/999.6-pipe-01-pipeline-rewrite/PIPE-01-BACKLOG.md (where the scoped task list lands long-term)

REQUIREMENTS.md updates:

| Row | Before | After |
|-----|--------|-------|
| SPIKE-01 traceability | Pending | Complete — `.planning/phases/05-.../05-SPIKE-DECISION.md` |
| SPIKE-02 traceability | Pending | Complete — `.planning/phases/05-.../05-SPIKE-DECISION.md` |
| Bullet list under §Renderer Backend Spike (Phase 5) | both `[ ]` | both `[x]` |

ROADMAP.md is updated by the orchestrator at phase-close (not in this plan).
</interfaces>
</context>

<tasks>

<task type="checkpoint:decision" gate="blocking">
  <decision>Which renderer backend does PIPE-01 (v2 pipeline rewrite) target?</decision>
  <context>
    Plans 05 + 06 produced working prototypes of both candidates behind the Phase 02.1 boundary. Plan 07 captured per-platform driver behaviour. The choice now informs PIPE-01's scope: a GL-Core PIPE-01 is the smaller rewrite (~100 fixed-function call sites in glRenderer.C → equivalent shader code + VBO uploads), while a QRhi PIPE-01 eliminates macOS GL deprecation entirely and removes per-OS branching from the rendering path but raises the Qt floor to 6.7+ and locks BALLView to Qt's QRhi API stability cadence.

    Per CONTEXT.md D-07 framing, the macOS GL deprecation banner is *intentionally* a standing reminder that compat profile is borrowed time. The decision record's "Chosen Backend" field directly informs whether 1.6.x continues to ship with that banner (GL-Core path) or whether v2 / PIPE-01 buys it out (QRhi path).

    Real data from the spikes (PNG sizes, render times, QRhi backendName captured on macOS Metal, Windows driver outcomes from 05-DRIVER-BEHAVIOUR.md) is in the upstream SUMMARYs — the executor reads those before this checkpoint fires and proposes a recommendation with the evidence inline.
  </context>
  <options>
    <option id="gl-core">
      <name>GL-Core profile (single backend for both 1.6.x and v2)</name>
      <pros>
        - Smaller PIPE-01 rewrite — shader pair + VBO upload per primitive; no per-OS code change.
        - Sticks to D-01 floor (Qt 6.5) — no Qt-version pressure.
        - Picking already implemented in Plan 05's spike (color-buffer FBO + glReadPixels).
        - Compatible with the existing QOpenGLWidget surface — no new widget type.
        - Cross-platform GL drivers all support GL 3.2+ core (macOS 4.1, Linux Mesa, Windows ICD).
      </pros>
      <cons>
        - macOS GL deprecation banner remains for the foreseeable future (D-07 standing reminder persists).
        - Per-platform GL driver quirks (Apple Silicon GL 4.1 cap; Intel iGPU on Windows; ANGLE removal) all still surface in BALLView code.
        - Future-incompatible with Qt's strategic direction (Qt is moving toward QRhi for new widget types).
      </cons>
    </option>
    <option id="qrhi">
      <name>QRhi (Metal/D3D/Vulkan abstraction, single backend for v2)</name>
      <pros>
        - Single code path across all three OSes — Metal on macOS, D3D11/12 on Windows, Vulkan on Linux.
        - Eliminates the macOS GL deprecation banner entirely (Metal native).
        - Aligned with Qt's strategic direction (QRhiWidget is Qt's recommended modern path).
        - QRhi handles shader transpilation via QShader / qsb — write GLSL once, run anywhere.
      </pros>
      <cons>
        - Raises Qt floor to 6.7+ (above D-01's 6.5) — affects user CI matrices and downstream packagers.
        - QRhi API has limited source/binary compatibility guarantees (Pitfall 7) — re-validation on every Qt LTS bump.
        - Larger PIPE-01 rewrite — QRhiBuffer / QRhiGraphicsPipeline / QRhiShaderResourceBindings boilerplate per primitive type.
        - Picking deferred in Plan 06 spike — needs QRhiReadbackResult adaptation work in PIPE-01.
        - QRhiWidget is younger code — fewer real-world deployments than QOpenGLWidget.
      </cons>
    </option>
    <option id="split">
      <name>Split: GL-Core for 1.6.x (BALL_RENDERER_BACKEND option default GLCore once PIPE-01 ships); QRhi added later as a v2.0 capability</name>
      <pros>
        - 1.6.x ships sooner — GL-Core PIPE-01 is the smaller rewrite per the GL-Core option.
        - Preserves D-01 Qt 6.5 floor for 1.6.x users.
        - QRhi added later when Qt 6.8 LTS or 6.9 stabilises and BALL ships v2.0; same BALL_RENDERER_BACKEND option just gains a `QRhi` value.
      </pros>
      <cons>
        - Two PIPE-01s long-term — first GL-Core, then QRhi — instead of one decisive rewrite.
        - macOS GL deprecation banner persists through 1.6.x.
        - More code to carry: the Phase 02.1 boundary supports it (the spike already proved both backends behind the factory), but maintainers carry the GL-Core path for the life of 1.6.x.
      </cons>
    </option>
  </options>
  <resume-signal>Select: gl-core, qrhi, or split. Optionally add a one-paragraph rationale Claude should incorporate into Section 2.1 of the decision record.</resume-signal>
</task>

<task type="auto" tdd="false">
  <name>Task 2: Write 05-SPIKE-DECISION.md and update REQUIREMENTS.md SPIKE-* rows</name>
  <files>.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-SPIKE-DECISION.md, .planning/REQUIREMENTS.md</files>
  <read_first>
    - 05-05-SUMMARY.md and 05-06-SUMMARY.md and 05-07-SUMMARY.md (the three upstream spike + driver records — sources for sections 2.1 and 2.2).
    - 05-DRIVER-BEHAVIOUR.md (per-platform table — section 2.2).
    - .planning/REQUIREMENTS.md §Renderer Backend Spike (Phase 5) + §Traceability (the rows being flipped).
    - .planning/RENDERER-INTERFACE-BOUNDARY.md §"What actually leaks" + §"What the Phase 5 swap then looks like" (PIPE-01 task list anchor).
    - .planning/phases/999.6-pipe-01-pipeline-rewrite/PIPE-01-BACKLOG.md (the existing PIPE-01 backlog entry — section 4 of the decision record will be its long-term home).
    - The Task 1 checkpoint resume signal — captures the chosen option (gl-core / qrhi / split) and any user-supplied rationale to weave into section 2.1.
  </read_first>
  <behavior>
    - `05-SPIKE-DECISION.md` exists at the artifact path with the six sections from `<interfaces>`. The "Chosen Backend" field names exactly one option (or the "split" pattern); the rationale references real data (verbatim numbers from PNG bytes / render times / DIAG lines) — abstract preferences without data citation are rejected.
    - Section 3 has measurable per-platform criteria (concrete numbers OR explicit "non-blank PNG via Phase 02.2 smoke check" — not "looks right").
    - Section 4 enumerates PIPE-01 tasks by category and effort indicator; the list is dense enough that `/gsd-plan-phase 999.6` could be invoked against it without further reading.
    - Section 5 references D-07, Pitfall 7, and any 05-DRIVER-BEHAVIOUR.md data gaps explicitly.
    - REQUIREMENTS.md SPIKE-01 + SPIKE-02 rows in the Renderer Backend Spike section + the Traceability table both flip to `[x]` / `Complete — .planning/phases/05-.../05-SPIKE-DECISION.md`.
  </behavior>
  <action>
    1. Create `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-SPIKE-DECISION.md` populating the schema in `<interfaces>`. Use the chosen option from Task 1's resume signal. Each rationale subsection (2.1-2.4) must cite at least one concrete data point from the upstream summaries (PNG byte counts; render times if captured; verbatim DIAG lines; QRhi backendName; deprecation banner text). Do NOT fabricate numbers — if 05-07-SUMMARY left a row as a data gap, section 5 mentions it.

    2. Section 4 (PIPE-01 task list): work through `glRenderer.C` (and use `grep -nE 'gl(Begin|End|MatrixMode|Translate|Rotate|Light|Material|TexCoord|Color|Vertex|Normal|Push|Pop)' source/VIEW/RENDERING/RENDERERS/glRenderer.C | wc -l` to count fixed-function call sites; report the number as evidence of scope). Enumerate the categories from `<interfaces>` §4 with effort indicators (small / medium / large). Mark spike-removal as task 0 — the first thing PIPE-01 does is delete the throwaway spike code path.

    3. Update REQUIREMENTS.md:
       - In §Renderer Backend Spike (Phase 5 — folded in from former Phase 05.1): flip both `[ ]` to `[x]`.
       - In §Traceability table: change "SPIKE-01 / SPIKE-02" status from "Pending" to "Complete — `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-SPIKE-DECISION.md`".

    4. Do NOT update ROADMAP.md — phase-close updates are the orchestrator's job (the `/gsd-execute-phase` exit step handles phase status / ROADMAP). Mention this explicitly in 05-08-SUMMARY.md so the orchestrator knows the next step.
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; test -f .planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-SPIKE-DECISION.md &amp;&amp; grep -qE '^# SPIKE-02 — Renderer Backend Decision Record' .planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-SPIKE-DECISION.md &amp;&amp; grep -qE '^## 1\. Chosen Backend' .planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-SPIKE-DECISION.md &amp;&amp; grep -qE '^## 2\. Rationale' .planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-SPIKE-DECISION.md &amp;&amp; grep -qE '^## 3\. Per-Platform Acceptance Criteria' .planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-SPIKE-DECISION.md &amp;&amp; grep -qE '^## 4\. Scoped PIPE-01 Task List' .planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-SPIKE-DECISION.md &amp;&amp; grep -qE '^## 5\. Known Liabilities' .planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-SPIKE-DECISION.md &amp;&amp; grep -qE 'SPIKE-01.*\[x\]|\[x\].*SPIKE-01' .planning/REQUIREMENTS.md &amp;&amp; grep -qE 'SPIKE-02.*\[x\]|\[x\].*SPIKE-02' .planning/REQUIREMENTS.md &amp;&amp; grep -q '05-SPIKE-DECISION.md' .planning/REQUIREMENTS.md &amp;&amp; wc -l .planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-SPIKE-DECISION.md | awk '$1 >= 120 {ok=1} END{exit !ok}'</automated>
  </verify>
  <acceptance_criteria>
    - 05-SPIKE-DECISION.md exists, ≥ 120 lines, with all six required H2 sections (`## 1.` through `## 6.`) plus references section.
    - Section 1 (Chosen Backend) names a specific backend (or the "split" hybrid), not "TBD".
    - Section 2 (Rationale) cites at least 3 concrete data points from upstream summaries: at least 1 PNG byte count, at least 1 verbatim DIAG line, at least 1 deprecation banner reference. (`grep -cE 'BALLVIEW_GL_DIAG|\.png|PNG.*[0-9]+ KB|deprecat' 05-SPIKE-DECISION.md >= 3`.)
    - Section 3 (Per-Platform Criteria) is a table with rows for macOS, Windows, Linux.
    - Section 4 (Scoped PIPE-01 Task List) enumerates at least 6 numbered task categories, each with an effort indicator.
    - Section 5 (Known Liabilities) references D-07, Pitfall 7, AND the throwaway-spike-removal task as PIPE-01 task 0.
    - REQUIREMENTS.md SPIKE-01 + SPIKE-02 bullet entries in §Renderer Backend Spike show `[x]`; Traceability rows for SPIKE-01 / SPIKE-02 reference the decision-record path.
    - Section 4 reports a concrete count of fixed-function call sites from `grep -nE 'gl(Begin|End|MatrixMode|...)' source/VIEW/RENDERING/RENDERERS/glRenderer.C` (real number, not "approximately ~100").
  </acceptance_criteria>
  <done>
    SPIKE-02 deliverable is committed to git with a named backend, a data-grounded rationale, measurable per-platform criteria, a scoped PIPE-01 task list, and an explicit known-liabilities ledger. REQUIREMENTS.md reflects SPIKE-01 + SPIKE-02 as complete. Phase 5 closes after the orchestrator's phase-close step.
  </done>
</task>

</tasks>

<verification>
- `test -f .planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-SPIKE-DECISION.md`.
- `wc -l .planning/phases/05-.../05-SPIKE-DECISION.md` returns `>= 120`.
- All six required H2 sections present (regex check above).
- Section 1 names a backend (regex `## 1\. Chosen Backend\s*\n\n\*\*Backend:\*\* (GL-Core|QRhi|GL-Core for|Split)` matches with non-trivial value — NOT "TBD" or "<choice>").
- REQUIREMENTS.md SPIKE-01 + SPIKE-02 entries flipped to `[x]`; Traceability rows updated.
- `git diff --stat .planning/phases/05-.../05-SPIKE-DECISION.md .planning/REQUIREMENTS.md` shows non-empty additions in both files.
</verification>

<success_criteria>
- Phase 5 closeout artifact exists, is committed, and is referenceable from PIPE-01-BACKLOG.md.
- The dormant PIPE-01 (backlog 999.6) entry can now legitimately be promoted by `/gsd-review-backlog` because its prerequisite (SPIKE-02) is complete.
- REQUIREMENTS.md SPIKE-01 + SPIKE-02 + the milestone progress reflect Phase 5 done.
- Phase 5 (Qt 6 Migration + Renderer Backend Spike) is COMPLETE pending the orchestrator's phase-close step.
</success_criteria>

<output>
After completion, create `.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike/05-08-SUMMARY.md` with: the chosen backend (one line), section-counts in 05-SPIKE-DECISION.md, the REQUIREMENTS.md diff snippet showing the SPIKE-* flips, and a 1-line note for the orchestrator: "Next step: `/gsd-execute-phase 5` phase-close updates ROADMAP.md + STATE.md."
</output>
