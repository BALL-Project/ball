# BALL v2.x Roadmap

**Status:** Snapshot 2026-05-17. Live source is
`/Users/kohlbach/Claude/BALL/ball/.planning/ROADMAP.md`.
**Scope:** v2.0 / v2.1 / v2.2 / v2.3 phase inventory. Does **not** cover
v1.8 (see [V1.8-WAVE.md](V1.8-WAVE.md)) or v1.7-tail items still in flight.

## v2.x release sequence

Per 2026-05-16 user direction "2.0 should start with the kernel":

| Release | Theme | Phases |
|---|---|---|
| **v2.0** | Substrate rewrite | 999.24 (LEAD, KERNEL) + 999.6 PIPE-01 + 999.9 YAML config + 999.11 mmCIF→gemmi + 999.12 deprecated-code removal |
| **v2.1** | Python surface | 6 (PyBALL bake-off, FIRST step) + 999.15 PyBALL bulk wrap |
| **v2.2** | Remote control | 999.10 REST API + PyBALL SDK |
| **v2.3** | Docs portal | 999.13 Doxygen → Read the Docs + Swagger UI (LAST v2.x phase) |

KERNEL leads v2.0 — K0+K1 establish `MoleculeStore` as the canonical
hot-path data structure. The four substrate phases run in parallel or after
K0+K1; v2.0 ships when all five complete. Downstream v2.1/v2.2/v2.3 then
build on the stable MoleculeStore facade.

---

## v2.0 phases (substrate rewrite)

### Phase 999.24 — KERNEL MoleculeStore (SoA) parallel-model migration (LEAD)

**Goal:** Introduce `MoleculeStore` (SoA columns + handle facade) **alongside**
the legacy Composite-pattern molecular hierarchy. Hot paths route through the
store; legacy code continues to use Composite. KERNEL v2.0 ships when
MoleculeStore is the canonical data path for force-field workflows and
selection queries.

**Why deferred from v1.7:** This is the substrate rewrite that the v1.6/v1.7
modernization arc deliberately did not touch. v1.x preserved the Composite
ABI so existing client code recompiles unchanged across the modernization;
v2.0 is the natural major-version break for the data-layout change. Per
2026-05-16 user direction it is the **lead** v2.0 phase — K0+K1 establish
the canonical hot-path data structure that 999.6/999.9/999.11/999.12 then
align to.

**Hard dependencies:** v1.7 tagged (so v1.x stays binary-compatible until
the v2.0 break). No external prerequisites.

**Scope:** Large. Per `KERNELV2.md` release milestones:
- v2.0-preview: K0 + K1 + K2 — 8-14 months
- v2.0.0: + K3 minimum (FORMAT + MOLMEC bonded + SCORING) + K4 Python-binding
  bridge — 16-24 months total
- v2.0.1+: remaining K3 subsystems — +6-10 months
- v2.0 final: all of K0-K4 — 22-34 months total

**Source of truth:** `/Users/kohlbach/Claude/BALL/ball/.planning/KERNELV2.md`
(554 lines, post-Codex Round 3 v3 baseline). Read this before promoting
any K0 plan. ROADMAP.md §"Phase 999.24" has the high-level summary +
sequencing rules.

**Compatibility promise (locked in KERNELV2.md):** source-compatible across
v1.7 → v2.0; ABI-compatible for all KERNEL+CONCEPT exported types; file
formats unchanged in v2.0; `BALL_FOREACH_*` macros preserved (deprecated
for new internal code only).

**Memory budget:** Hot-path additional footprint ≤150 B/atom.

---

### Phase 999.6 — PIPE-01: fixed-function GL → modern backend (QRhi)

**Goal:** Replace BALL's ~100 fixed-function OpenGL calls in `glRenderer.C`
with a modern programmable-pipeline backend. Eliminates the macOS-OpenGL
dead-end risk (Apple stuck at OpenGL 4.1 since 2018) and matches Qt's
strategic direction.

**Why deferred from v1.7:** Phase 5's two-arm spike (Plans 05-05/05-06)
landed the **split-pattern decision** in
`05-SPIKE-DECISION.md`: **GL-Core for v1.6.x/v1.7** (which ships with the
compat profile), **QRhi for v2.0**. The decision was wedged by the Qt 6.5
LTS floor + the Ubuntu 24.04 system-Qt deployment constraint — QRhi
requires Qt 6.7+.

**Hard dependencies:**
- Phase 02.1 — Renderer boundary (DONE 2026-05-14): the `Renderer` /
  `RenderSurface` / `RendererFactory` seam PIPE-01 swaps behind.
- Phase 5 SPIKE-02 — DONE 2026-05-15. Decision recorded.
- Externally, dropping the GL fallback fully depends on the macOS deploy
  target accepting MoltenVK or Apple's Metal path. Open question — see
  [OPEN-QUESTIONS.md](OPEN-QUESTIONS.md) §1.

**Scope:** Large. ~125 fixed-function GL deprecation warnings dissolve
naturally (Phase 999.22b in CENSUS.md). Carries SEED-005-4 (post-PIPE-01
Windows driver-behaviour recapture) inline.

**Source of truth:** `/Users/kohlbach/Claude/BALL/ball/.planning/phases/999.6-pipe-01-pipeline-rewrite/PIPE-01-BACKLOG.md`
— full known transition surface (4 documented leaks + 6 additional touch
points), QRhi vs GL-core research, 8 open questions for the spike to
resolve, research provenance.

---

### Phase 999.9 — INIFile → YAML config

**Goal:** Replace BALL's INIFile-based config (force-field parameter files,
BALLView preferences, scoring weights) with a YAML-based config + schema
validation.

**Why deferred from v1.7:** Not core value (build + render on 3 OSes). INI
files work today. The win is modernization + tooling + schema validation.
v1.7 is the UI refresh; data-format churn during a UI cycle would invalidate
the "same scientific output" guarantee. v2.0 is the natural home: substrate
modernization (renderer + config) together, post-v1.7-UI.

**Hard dependencies:** None upstream (independent of 999.24 KERNEL). Touches
force-field parameter loading which downstream-affects every chemistry
calculation, so lands after v1.7 stable + v1.8 UI churn settles.

**Scope:** Medium. Pairs naturally with 999.6 (PIPE-01) in the v2.0
"substrate modernization" bundle.

**Source of truth:** ROADMAP.md §"Phase 999.9: Replace INIFile with YAML config".

---

### Phase 999.11 — In-tree mmCIF parser → gemmi

**Goal:** Replace BALL's in-tree mmCIF parser (the one with the Bison
grammar shift-reduce conflicts captured in Phase 999.23) with [gemmi](https://gemmi.readthedocs.io/),
an upstream-maintained mmCIF/PDB/MTZ parser that handles modern PDBx/mmCIF
correctly.

**Why deferred from v1.7:** The in-tree parser works for typical PDB-derived
mmCIF inputs. The shift-reduce conflicts (5 conflicts on Bison 3.8.2 + 19
`-Wformat-truncation` warnings, see Phase 999.23) are latent — they don't
break the v1.7 ship. gemmi is correctness-sensitive substrate work that fits
v2.0.

**Hard dependencies:** None upstream. Can run in parallel with 999.9 / 999.6
— independent files.

**Scope:** Medium. Optional follow-on: PDB/MOL2 also through gemmi if
downstream parsers show similar maintenance issues at v2.0 time. Default:
don't expand scope — replace CIF cleanly, leave other formats alone.

**Source of truth:** ROADMAP.md §"Phase 999.11: Replace in-tree mmCIF parser with gemmi".
Adjacent context: `.planning/phases/999.23-cif-grammar-audit/` (in main repo).

---

### Phase 999.12 — Bulk deprecated-code removal

**Goal:** Audit every `BALL_DEPRECATED` / `BALL_VIEW_DEPRECATED` /
`[[deprecated]]` marker in libBALL + libVIEW + applications + bindings.
For each: **delete** the code (+ in-tree callers) OR **un-deprecate** it
(if no replacement exists and the class is still load-bearing, per the
Phase 5.1 Plan-09 precedent).

**Why deferred from v1.7:** v1.x stays binary-compatible with the deprecation
warnings; v2.0 is the major-version break that lets us actually ship the
removal. Per ROADMAP scope notes: ~30 files across 7 modules carry
deprecation markers as of v1.6.

**Why this runs LAST in v2.0:** Each of 999.6 / 999.9 / 999.10 / 999.11
removes a slice of the deprecated surface as a side-effect. 999.12 mops
up what's left.

**Slices already handled (pull-forwards):**
- GENETICDOCK subsystem — handled by **Phase 999.29 v1.6.2 pull-forward**
  (18 files, 2722 LOC deleted). Removed from 999.12 scope.
- VRMLRenderer — handled by **Phase 999.37 v1.7 inline** (146 + 487 LOC
  deleted). Removed from 999.12 scope.

**Hard dependencies:** Runs after 999.6, 999.9, 999.11 so the side-effect
slices are accounted for. KERNEL v2.0 K0+K1 should be in place so the
v2.0 binary-break is already committed.

**Scope:** Medium. ~2-3 weeks per ROADMAP estimate. The audit phase is the
most contentious (each class needs a real disposition call); the per-class
RELEASE-NOTES migration entry is the most labour-intensive.

**Remaining v2.0 scope after pull-forwards:** VIEW/WIDGETS slice
(HelpViewer, MyTextBrowser, ComboBoxDelegate, HotkeyTable), VIEW/DIALOGS
slice (ExportGeometryDialog), RENDERING residuals.

**Source of truth:** ROADMAP.md §"Phase 999.12: Remove deprecated code from
core library and beyond".

---

## v2.1 phases (Python surface)

### Phase 6 — PyBALL generator bake-off (FIRST step of v2.1)

**Goal:** Empirically decide BALL's Python-bindings tool — **autowrap+Cython
vs nanobind** (with SIP 6 as optional incumbent baseline) — by running both
against a real cross-platform vertical slice of seven BALL classes covering
all hard patterns (templates, ownership transfer, exceptions, iterators,
Composite/processor callbacks, file I/O, NumPy interop). The tool decision
is the deliverable.

**Why deferred from v1.7:** Moved out of v1.6.x to v2.1 per 2026-05-16 user
direction ("PyBALL changes should move to 2.x"). v1.7 ships C++ only — no
Python bindings shipped (SIP 4 disabled, no replacement yet).

**Why FIRST in v2.1:** PYBALLV2.md §6's bake-off must complete before
Phase 999.15 bulk wrap fires.

**Hard dependencies:** v2.0 substrate (KERNEL MoleculeStore) **should be**
landed first — PyBALL wraps the v2.0 surface, so KERNEL ABI must be stable.

**Scope:** Medium. 7.5 weeks per PYBALLV2.md §2 (P6-01 1.5w + P6-02 3w +
P6-03 2.5w + P6-04 0.5w). Parallelizable if 2 engineers split the autowrap
and nanobind tracks.

**Hard stop/pivot gates:** see PYBALLV2.md §10 — tri-OS green; behavior
spec parity; exception/iterator/ownership parity; perf within 2× of SIP 4
baseline; wheel import ≤500 ms.

**Source of truth:** `/Users/kohlbach/Claude/BALL/ball/.planning/PYBALLV2.md`
§6 (4-round CODEX-reviewed). Working autowrap POC at
`/Users/kohlbach/Claude/BALL/ball/.planning/pyball-poc/poc1-simple/`.

---

### Phase 999.15 — PyBALL bulk wrap (CONDITIONAL on Phase 6)

**Goal:** Replace BALL's SIP 4.9-based Python bindings (237 hand-written
`.sip` files, 73 of them with custom Python integration code) with a
modern Python-3-only binding layer covering a substantially broader API
surface than today's hand-curated subset, in sync with C++ headers.

**Why deferred from v1.7:** Per 2026-05-16 user direction — v1.7 ships
C++ only; PyBALL is v2.1. Also conditional on Phase 6 bake-off picking a
tool.

**Hard dependencies:** Phase 6 has picked a tool AND that tool has passed
the PYBALLV2.md §10 hard stop/pivot gates.

**Scope:** Large. Planning envelope 18-25 weeks (autowrap path) or 21.5-32.5
weeks (nanobind path); ~30% larger for nanobind because no codegen. See
PYBALLV2.md §11 for the nanobind variant plan.

**In scope:** BALL-side wrapping (171 `.sip` files). KERNEL + FORMAT +
STRUCTURE + MOLMEC + ENERGY + SOLVATION + NMR + QSAR + DOCKING + SCORING
+ MATHS + COMMON + DATATYPE.

**Out of scope (deferred to v2.2 or later):** VIEW-side wrapping (66 `.sip`
files) — Qt/PyQt6 interaction needs its own design pass.

**Output:** `pip install pyball` + `conda install -c conda-forge pyball`
deliverable. Built green on macOS-arm64 / Linux-x64 / Windows-x64 in CI
via **cibuildwheel** (one wheel per Python × OS), with **type stubs** (.pyi).

**Source of truth:** `/Users/kohlbach/Claude/BALL/ball/.planning/PYBALLV2.md`
§7-12 + ROADMAP.md §"Phase 999.15".

---

## v2.2 phases (remote control)

### Phase 999.10 — REST API + PyBALL SDK (replaces TCP `ServerWidget`)

**Goal:** Replace BALL's BALL-proper TCP-socket remote-control interface
(`BALL::Client` / `ServerWidget`, deprecated through v1.x) with a REST API
+ a PyBALL SDK class layer (`pyball.viewer.RemoteViewer`). Loopback-only
by default; single-user model; no auth needed.

**Why deferred from v1.7:** Not core value. The TCP server compiles
(`boost::asio` API breakage fixed in Phase 1) and `BALL::VIEW::ServerWidget`
is `BALL_DEPRECATED` but not removed. v1.7 is the UI refresh, not the
wire-protocol refresh.

**Why v2.2 (not v2.0):** Per 2026-05-16 user direction, KERNEL v2.0 K0+K1
must establish MoleculeStore as the canonical hot-path data structure
first. The REST surface targets the post-MoleculeStore facade with PyBALL
SDK classes built on top.

**Hard dependencies:**
- KERNEL v2.0 K0+K1 (999.24) shipped — stable MoleculeStore facade.
- PyBALL v2.1 (999.15) shipped — SDK side needs PyBALL.

**Supersedes:** backlog 999.3 (Networking rework). 999.3's goal was to
modernize the BALL-proper networking code; 999.10's goal is to remove it.
If 999.10 promotes, 999.3 closes as superseded.

**Scope:** Medium-Large. Authors `doc/REST-API.yaml` (OpenAPI 3.x), which
becomes the input to v2.3's Swagger UI page.

**Out of scope (deferred to later v2.x):** WebSocket push notifications
(initial SDK polls); LAN exposure (loopback-only default); backwards-compat
shim for the old TCP protocol (users migrate at v2.0; old code is
`BALL_DEPRECATED` through v1.7).

**Source of truth:** ROADMAP.md §"Phase 999.10" (large entry with full
endpoint sketch + migration story).

---

## v2.3 phases (docs portal)

### Phase 999.13 — Doxygen → Read the Docs + Swagger UI (LAST v2.x phase)

**Goal:** Convert BALL's existing Doxygen-only C++ API documentation (659
header files carrying `/** ... */` comments across 25 modules) into a hosted
Read the Docs site at `ball-project.readthedocs.io`. The Doxygen comments
stay in the source (single source-of-truth); the OUTPUT format flips from
"locally-built HTML you only see if you run `cmake --build --target doc`"
to "online portal with stable URL + search + cross-references + version
selector." Also publishes a Swagger UI page rendering 999.10's `doc/REST-API.yaml`.

**Two distinct deliverables, one phase:**
1. Doxygen-generated C++ API → Read the Docs (Breathe + Exhale bridge over
   existing Doxygen XML; no per-class rewrite).
2. Swagger UI page consuming `doc/REST-API.yaml` via `sphinxcontrib-openapi`.

**Why deferred from v1.7:** Without 999.10's REST API spec there's no
Swagger UI to publish; without 999.15's PyBALL there's no Python SDK to
document; v2.x release cadence has docs at the end so the docs portal
ships against fully-stable content.

**Why LAST v2.x phase:** Sequencing chain — KERNEL v2.0 (999.24) → PyBALL
v2.1 (999.15) → REST v2.2 (999.10) → v2.3 docs portal consuming all three.

**Hard dependencies:**
- Phase 999.10 — DONE (authors `doc/REST-API.yaml`, the Swagger UI input).
- Phase 6 / 999.15 — DONE (PyBALL must exist for the Python SDK docs).
- Phase 8 (Packaging) — DONE (provides install instructions for
  `docs/getting-started/`).

**Scope:** Medium-Large. ~2-3 weeks per ROADMAP estimate. Plan 2 (Breathe
integration) is the trickiest because BALL's API surface is large and
Doxygen comment quality varies; Plan 3 (Swagger UI) is mechanical once
the OpenAPI spec is stable.

**Out of scope:** Translations of the docs; versioned docs for pre-1.6
BALL; custom domain (`docs.ball-project.org`); auto-generating REST
endpoint examples (Swagger UI already does this).

**Source of truth:** ROADMAP.md §"Phase 999.13" (long entry with 6-plan
sketch).

---

## Cross-cutting notes

### Compatibility-promise chain

- **v1.7 → v2.0:** source-compatible (existing client code recompiles
  unchanged); ABI-compatible for KERNEL+CONCEPT exports; file formats
  unchanged. The KERNEL break is **additive** — new `MoleculeStore` /
  `AtomView` / `CompiledExpression` types are added; the legacy Composite
  ABI stays.
- **v2.0 → v2.1:** v2.1 PyBALL bulk wrap targets the v2.0 MoleculeStore
  facade. Existing PyBALL clients (BALLAXY / PresentaBall / Galaxy /
  Jupyter) break at v2.0; the migration window is v1.7 release notes
  (deprecation banner) → v2.0 release notes (migration guide) → v2.1
  `pyball` package.
- **v2.0 → v2.2:** `BALL::Client` / `ServerWidget` (TCP remote-control)
  deleted at v2.0; clients migrate to `pyball.viewer.RemoteViewer` against
  the v2.2 REST API.
- **v2.x intra-version:** Read the Docs portal (v2.3) is additive
  documentation only — no API impact.

### Pull-forwards already done (don't re-plan)

- **Phase 999.29 GENETICDOCK removal** — pulled forward from 999.12 v2.0
  slice into v1.6.2 (2026-05-16). 18 files, 2722 LOC deleted. Phase 999.12
  v2.0 scope reduced by this slice.
- **Phase 999.37 VRMLRenderer removal** — pulled forward from 999.12 v2.0
  slice into v1.7 inline (2026-05-17). 146 + 487 LOC deleted, atomic with
  `STLNR` UI rename + INI key migration. Phase 999.12 v2.0 scope reduced
  by this slice.

### Things v2.x deliberately doesn't touch

- **Translation infrastructure beyond v1.8 community round.** The de_DE +
  zh_TW `.ts` files get a community round during v1.8 (per maintainer-Q4
  resolution); v2.x continues the same pattern, no engineered i18n shift.
- **Custom-domain docs** at `docs.ball-project.org`. v2.3 ships at
  `ball-project.readthedocs.io`. Custom-domain hosting is a separate
  post-v2.3 question.
- **Auto-update protocol changes.** Sparkle 2.6.x + WinSparkle + Ed25519
  appcasts on GitHub Pages stays; v2.x is a feature release line, not an
  update-infrastructure rework.

### Risk inheritance from v1.7

Items the v1.4 → v1.7 retrospective flagged as "deferred to v2.0+" that
this roadmap now owns:

- KERNEL MoleculeStore migration (Phase 999.24) → covered, LEAD v2.0.
- PIPE-01 renderer rewrite (Phase 999.6) → covered, v2.0.
- INIFile → YAML config (Phase 999.9) → covered, v2.0.
- In-tree mmCIF → gemmi (Phase 999.11) → covered, v2.0.
- Bulk deprecated-code removal (Phase 999.12) → covered, v2.0.
- PyBALL v2 (Phase 6 + 999.15) → covered, v2.1.
- REST API + PyBALL SDK (Phase 999.10) → covered, v2.2.
- Read the Docs + Swagger UI (Phase 999.13) → covered, v2.3.
- Phase 999.22b OpenGL deprecation-warning cleanup → folds into 999.6.
- Apple-GL deprecation banner → Metal/MoltenVK via 999.6.
- Tier-C warning census (b) subset → folds into 999.6.

Items deliberately NOT in v2.x:
- Phase 999.22c per-site code-review subset — owned by v1.7 cycle, see
  ROADMAP.md.
- CIF Bison grammar shift-reduce conflicts (Phase 999.23) — resolved
  when 999.11 deletes the in-tree parser.
- FFTW GPL-SKU owner gap — open question, see [OPEN-QUESTIONS.md](OPEN-QUESTIONS.md) §4.
- BUILD-windows.md — doc gap, see [GAPS-FROM-V1.7.md](GAPS-FROM-V1.7.md).
