# v2.x Open Questions — pre-staged maintainer decisions

**Status:** Forward-looking. **Authored:** 2026-05-17.
**Pattern:** Same as `MAINTAINER-QUESTIONS-999.1.md` for the v1.6→v1.7
transition — surface decisions that gate v2.x phase planning before the
phase planner gets blocked.

These questions need answers **before** the corresponding v2.x phase can
finish planning. Don't promote the phase until the question resolves.

---

## Q1 — PyBALL v2 binding generator (gates Phase 6)

**Context:** PyBALL today is SIP 4.9 (237 hand-written `.sip` files,
disabled in CI). SIP 4 is upstream-deprecated. PYBALLV2.md restructured
the original "Decision committed: autowrap+Cython" v1 into a
"candidate-pending-bake-off v2" after adversarial CODEX review. Phase 6
runs a real bake-off; Phase 999.15 bulk wrap fires after the bake-off
picks a tool AND that tool clears the PYBALLV2.md §10 hard stop/pivot
gates. But the bake-off **scope** itself needs maintainer input on three
sub-decisions:

**Options:**
1. **Bake-off arms: autowrap+Cython vs nanobind** — Claude's recommendation.
   Two arms, both run against the same 7-class vertical slice.
2. **Bake-off arms: autowrap+Cython vs nanobind vs SIP 6** — include SIP 6
   as an incumbent baseline. Adds ~1.5 weeks; helps argue against the "we
   never even tried staying on SIP" objection.
3. **Bake-off arms: include cppyy as a third** — runtime introspection,
   no per-class wrapping code. Adds another ~2 weeks but if it works it
   eliminates 999.15-03/04/05 entirely.

**Sub-question A — generator strategy:** Codegen-first (autowrap, cppyy)
or hand-written (nanobind, pybind11, SIP 6)? Codegen-first is a stated
user preference per PYBALLV2.md, but PYBALLV2.md also flags pyOpenMS's
GSoC 2026 nanobind evaluation as counter-signal.

**Sub-question B — SIP compat shim:** Drop the legacy SIP 4 toolchain
entirely at v2.1 (clean break, PyBALL v2 only) OR maintain a SIP 4
compat shim through v2.x for downstream packagers (BALLAXY, PresentaBall,
Galaxy, Jupyter notebooks) to migrate at their pace?

**Recommendation (Claude):**
- Option 1 (2-arm bake-off). SIP 6 inclusion is low-value — SIP 4 → SIP 6
  isn't "stay on SIP," it's a major migration in its own right, so doesn't
  earn an incumbent-baseline slot.
- Sub-A: codegen-first; autowrap+Cython is the candidate; nanobind is the
  fallback escape hatch for hot paths (per PYBALLV2.md §5 "Why nanobind is
  the optional escape hatch").
- Sub-B: clean break at v2.1, no SIP 4 shim. Per PYBALLV2.md, v1.7 ships
  C++ only — there's a one-version window (v1.7 → v2.0 → v2.1) where
  PyBALL doesn't exist anyway, so downstream packagers don't have a
  compat-shim to lose.

**Status:** OPEN. Gates Phase 6 promotion.

---

## Q2 — PIPE-01 renderer rewrite scope (gates Phase 999.6)

**Context:** Phase 5 SPIKE-02 decision (`05-SPIKE-DECISION.md`) committed
"GL-Core for v1.6.x/v1.7, QRhi for v2.0" on the split pattern — the QRhi
arm was wedged by Qt 6.5 LTS floor + Ubuntu 24.04 system-Qt deployment.
Now Qt 6.8 LTS is the floor (since v1.6.2). QRhi requires Qt 6.7+, so the
spike's blocker is gone. But the spike itself was **time-boxed** and
documented as "inconclusive on three open points": (1) Metal-vs-MoltenVK
on macOS, (2) D3D11 vs OpenGL backend on Windows when QRhi auto-selects,
(3) Linux fallback if the Mesa version on the target distro doesn't
expose GL 4.3+ for QRhi-OpenGL.

**Options:**
1. **Finish the QRhi spike first** — open a Phase 999.6.0 pre-flight that
   resolves the three open points against current Qt 6.8 LTS before
   committing to the v2.0 rewrite scope. Risk: another time-boxed spike
   that doesn't land production code. Adds 2-4 weeks.
2. **Skip the pre-flight; start the rewrite** — commit to QRhi-on-Metal
   (macOS), QRhi-on-D3D11 (Windows), QRhi-on-OpenGL (Linux fallback) as
   the v2.0 baseline. Risk: the three open points become production
   blockers mid-rewrite.
3. **Drop fixed-function GL entirely at v2.0** — no compat-profile
   fallback. macOS-OpenGL deprecation forces the migration anyway; Linux
   users on ancient Mesa would need to upgrade their distro.

**Sub-question — when:** v2.0 sequencing has 999.6 in parallel with
999.24 KERNEL (per ROADMAP). Should it land before/after KERNEL K0+K1, or
truly in parallel? KERNEL touches Atom layout; renderer reads from Atom
positions. There's an interaction.

**Recommendation (Claude):**
- Option 1 (finish the spike). The three open points are the same kind of
  "wedged spike" the v1.7 cycle paid for by deferring PIPE-01 in the
  first place; landing them up-front is cheaper than landing them
  mid-rewrite. The pre-flight runs against the existing v1.7-modernization
  Qt 6.8 LTS substrate; no v2.0 work blocks on it.
- Drop fixed-function GL fully (Option 3) — macOS deprecation forces it;
  Linux users on Mesa <4.3 are below BALL's documented system requirements
  anyway.
- Sequencing: 999.6 runs AFTER KERNEL K0+K1 lands. Renderer reads from
  MoleculeStore.position columns; if KERNEL re-lays-out atoms, renderer
  reads break. Land K0+K1 first; let the renderer rewrite consume the
  stable MoleculeStore facade.

**Status:** OPEN. Gates Phase 999.6 promotion.

---

## Q3 — Doxygen → Read the Docs migration shape (gates Phase 999.13)

**Context:** ROADMAP §"Phase 999.13" sketches Breathe + Exhale +
sphinxcontrib-openapi as the bridge. There are alternatives.

**Options:**
1. **Sphinx + Breathe + Exhale** — ROADMAP's current sketch. Keeps
   Doxygen as the comment extractor; Breathe is the Doxygen-XML → reST
   bridge; Exhale generates a navigable API tree. Read the Docs hosts.
2. **mkdocs + mkdoxy** — alternative stack: mkdocs (Markdown-native, no
   reST), mkdoxy as the Doxygen bridge. Simpler config, less mature on
   large C++ codebases (BALL's 659 header surface is large).
3. **Sphinx + Doxygen XML directly (no Breathe)** — write the Doxygen-XML
   → Sphinx integration ourselves. High control, high maintenance cost.
4. **Hugo + custom Doxygen integration** — JAMstack approach. Faster
   builds; needs custom CI infra (RTD doesn't host Hugo).

**Sub-question A — in-tree or separate docs repo:** ROADMAP §999.13
implies in-tree at `docs/` (new directory next to the existing `doc/`
Doxygen output). Alternative: separate `ball-project/ball-docs` repo,
git-submodule'd from main, deployed independently.

**Sub-question B — versioning model:** RTD supports versioned docs.
v1.6 / v1.7 / v1.8 / v2.0 each as a permalink, plus a `latest` for the
current main branch. Should pre-v2.0 versions get docs (back-fill
Doxygen XML from old tags + re-run RTD build per tag), or v2.0+ only?

**Recommendation (Claude):**
- Option 1 (Sphinx + Breathe + Exhale). It's the industry-standard
  scientific-software docs stack (NumPy, SciPy, RDKit, OpenBabel, gemmi
  all use Sphinx); RTD-hosted; well-understood failure modes; community
  knowledge for troubleshooting.
- Sub-A: in-tree at `docs/`. Avoids submodule complexity; doc edits
  ship in the same PR as code edits; one CI gate.
- Sub-B: v2.0+ only. Pre-v2.0 docs would be useful but require running
  Doxygen + Sphinx + RTD build against historical tags, which fights
  the v1.x toolchain churn. Ship v2.0 as the docs floor.

**Status:** OPEN. Gates Phase 999.13 promotion (which is itself the
last v2.x phase, so not urgent — but pre-staging the answer avoids a
v2.3 mid-phase scope battle).

---

## Q4 — FFTW GPL-SKU owner gap (revisits v1.7 LGPL-only decision)

**Context:** Phase 08c (Linux packaging) shipped the AppImage / DEB / RPM
as a single **LGPL-only SKU** because FFTW is GPL-2.0+; linking BALL
(LGPL-2.1+) against FFTW relicenses the binary to GPL. v1.7 lacks a
**named GPL-compliance owner**, so the GPL SKU is scaffolded
(`packaging/linux/COPYING.GPL`, `README-LICENSING.md`,
`cmake/BALLPackageConfig.cmake` SKU-name flip keyed on `BALL_HAS_FFTW`)
but not built/uploaded. Users who need FFTW build from source per
`BUILD-linux.md`.

**REOPEN criteria** (per `08c-FFTW-GPL-DECISION.md` §"REOPEN criteria"):
named human accepts GPL-compliance ownership; ship-both becomes a
`release.yml` matrix-entry addition only — no code or packaging work
required.

**Options:**
1. **Stay LGPL-only through v2.x.** Continue the v1.7 status. Users
   needing FFTW build from source. Risk: scientific users who don't
   want to build BALL from source remain stuck without FFTW-dependent
   features (some NMR / SOLVATION code paths).
2. **Recruit a GPL-compliance owner before v2.0 release; ship dual SKU.**
   Owner triages user reports of GPL-compliance violations in distribution
   chains, monitors downstream packagers (conda-forge, Homebrew tap) for
   GPL-mixed binaries, signs off on each release. Real human work, ~quarterly.
3. **Replace FFTW with a non-GPL FFT library.** PocketFFT (BSD-3) is the
   obvious candidate — used by NumPy. ~2-4 weeks of substitution work +
   verification that the NMR/SOLVATION outputs match. Eliminates the
   licensing question.

**Recommendation (Claude):**
- Option 3 (PocketFFT substitution) is the cleanest long-term answer and
  fits the v2.0 "substrate rewrite" theme. Schedule it as a small v2.0
  sub-phase under 999.9-or-adjacent. Removes the need for any GPL-compliance
  owner; removes the dual-SKU complexity from `release.yml`; lets the
  binary stay single-SKU LGPL forever.
- If PocketFFT verification turns out to be non-trivial (e.g.,
  numerical-precision drift in NMR), fall back to Option 2.
- Option 1 (status quo) is acceptable through v2.0 if the substitution
  work doesn't fit the v2.0 envelope — but document the v2.1 substitution
  intent in v2.0 RELEASE-NOTES so users know.

**Status:** OPEN. Doesn't block v2.0 LEAD phases; gates the v2.0 packaging
sub-phase (which is small — a `release.yml` matrix-entry addition under
Option 2, or a code substitution under Option 3).

---

## Q5 — Source-aware Windows cmake-tree cache key (carry-over from v1.7)

**Context:** From the v1.7 retrospective §5.1: the cb392cc4 → 9042af07
incident exposed that the Phase 999.17 Windows CMake build-tree cache key
does not hash `source/MATHS/vector3.C`-style export-contract surface.
Stopgap: cache-key bump v1 → v2 in commit `c9d8de383`. Structural fix
queued as Phase 999.36 for "v1.6.3 or v1.7 or later."

**Why this is here in v2.x questions:** the structural fix didn't land
before v1.7 tail. v2.x inherits the stopgap; another silent-mask incident
could happen on any future source-defect that touches export contracts.

**Options:**
1. **Land Phase 999.36 in v1.8.** Doesn't fit the v1.8 UI marketing
   release shape, but it's a pure CI infrastructure change with no
   user-visible impact.
2. **Land Phase 999.36 in v2.0 alongside 999.12 (deprecated removal).**
   v2.0 already touches the export surface (some deprecated classes have
   `BALL_EXPORT`); cache-key fix lands as part of that work.
3. **Live with the stopgap through v2.x.** Bump the cache key version
   manually each time a future incident triggers it.

**Recommendation (Claude):**
- Option 2. v2.0 is the natural home — the export surface is changing
  there anyway. Bundle Phase 999.36 into v2.0 substrate work.

**Status:** OPEN. Low priority — stopgap works. Promote when planning
v2.0 substrate work.

---

## Status summary

| # | Question | Status | Blocks |
|---|---|---|---|
| Q1 | PyBALL v2 binding generator | OPEN | Phase 6 promotion (v2.1 lead) |
| Q2 | PIPE-01 renderer rewrite scope | OPEN | Phase 999.6 promotion (v2.0) |
| Q3 | Read the Docs migration shape | OPEN | Phase 999.13 promotion (v2.3, last v2.x) |
| Q4 | FFTW GPL-SKU owner gap | OPEN | v2.0 packaging sub-phase (small) |
| Q5 | Source-aware Windows cache key | OPEN | v2.0 substrate work bundle (low priority) |

**No question is "answered" yet** — this is the pre-staging exercise; the
v2.x cycle's `MAINTAINER-QUESTIONS-v2.x.md` will record actual answers
once the maintainer reviews this list.

**Recommended order to raise with maintainer:** Q2 (most architecturally
load-bearing — wedges the v2.0 lead) → Q1 (long lead time on the
bake-off) → Q4 (PocketFFT substitution if pursued is itself a small
scoped phase to plan into v2.0) → Q5 (low priority, bundle with v2.0
substrate) → Q3 (last v2.x phase, plenty of lead time).
