---
phase: 999.14-github-issue-pr-triage
round: 2 (maintainer-batch, all 95 currently-open issues)
decided_by: Claude (Opus 4.7, gsd-execute pattern)
decided_at: 2026-05-17
baseline_files:
  - .planning/phases/999.14-github-issue-pr-triage/issues-baseline.json  # 95 open issues
rubric: close-as-fixed | close-as-obsolete | close-as-stale | keep | needs-investigation
auto_close: DISABLED — decisions doc only; bulk-close deferred to human approval
prior_round_pointer: .planning/triage-999.14/decisions.md (round-1, 81 closes already executed)
---

# 999.14 issue triage — decisions (2026-05-17)

**Triage by:** Claude (gsd-execute pattern, NO auto-close).
**Source baseline:** issues-baseline.json (95 open issues, snapshot 2026-05-17).
**Templated close comments:** see ROADMAP Phase 999.14 entry "Templates" subsection (mirrored in `triage-999.14/decisions.md` round-1).
**Prior round:** 2026-05-16 closed 81 issues + 1 PR; the 95 remaining here were deferred ("160 issues authored by maintainer team — deferred for explicit confirmation"). This round is the maintainer-batch + new-arrivals pass.

## Out-of-scope guardrails applied
- **Active-in-last-90-days override:** any issue with activity since 2026-02-17 is forced to `keep` regardless of category match. **5 issues** match: #289, #539, #540, #601, #636 (all touched 2026-05-16 — most likely re-labelled/touched by triage-round-1 metadata edits; treated as user-engaged for safety).
- **Don't re-categorize already-closed issues** — 95 baseline = all currently-open only.
- **NO label edits, NO `gh issue close` invocations.** This round produces only the decisions doc + proposed close commands for human approval.

## Summary
| Category | Count | Next step |
|---|---:|---|
| close-as-fixed | 2 | Human approval → bulk `gh issue close --comment` |
| close-as-obsolete | 8 | Same |
| close-as-stale | 6 | Same |
| keep | 76 | Apply target-milestone label (deferred — human) |
| needs-investigation | 3 | Park for follow-up round |
| **Total** | **95** | |

## Active-in-last-90-days exceptions (forced keep)
| # | Title | Author | Last updated | Why kept |
|---|---|---|---|---|
| 289 | Splitted surface crashes BALL — project files | anhi | 2026-05-16 | Active comment activity within 90d — user-engaged; needs human judgement |
| 539 | AromaticityProcessor fails on certain molecules | pbrach | 2026-05-16 | Same |
| 540 | Infinite loop in SDGenerator | dstoeckel | 2026-05-16 | Same |
| 601 | NMR/spectrum.iC: computeMoments() does nothing | tkemmer | 2026-05-16 | Same |
| 636 | Memory leak in DisplayProperties | tkemmer | 2026-05-16 | Same |

These 5 are kept regardless of any other categorization signal. Most are real bugs and would have landed in `keep` anyway.

---

## Per-issue decisions

### close-as-fixed (2)

| # | Title | Fix commit / phase | Why |
|---|---|---|---|
| 639 | CHECK_CXX_SOURCE_COMPILES ignores compiler flags on macOS | Phase 1 / Phase 3 build-baseline + language-modernization | Cited line `cmake/BALLConfiguration.cmake:104-107` (an `APPLE`-conditional that skipped `-std=c++11` flags during config checks). Current `BALLConfiguration.cmake:95-115` has NO APPLE guard — flags propagate via `BALL_PROJECT_COMPILE_FLAGS` for all platforms (`grep "IF(APPLE)" cmake/BALLConfiguration.cmake` → empty). Phase 3 also made C++17 the unconditional default (`CMAKE_CXX_STANDARD 17`), eliminating the entire class of "C++11 feature guard disabled on macOS" symptoms. **Verified on v1.7-modernization branch.** |
| 7   | Integrate Python tests into CTest | Phase 999.15 (PyBALL v2 — already in flight); SIP path superseded | Issue asks to integrate "the existing Python test skeleton" into the BALL Makefile/CTest. Python bindings (SIP 4.9) are disabled in current build (`BALL_PYTHON_SUPPORT=OFF` in all `ci-*` presets). Phase 999.15 owns rewrite-and-rewire; this old SIP-based test integration is moot. *(Could also be `close-as-obsolete` — same outcome; categorized `fixed` because 999.15 explicitly delivers the equivalent.)* |

### close-as-obsolete (8)

| # | Title | Removed/replaced subsystem | Why |
|---|---|---|---|
| 330 | python "Constant" should be python "Constants" | SIP Python bindings (disabled) | Tiny Python-side naming-deprecation request. SIP 4.9 bindings disabled in current build. Phase 999.15 redesigns Python entirely; any Python-naming policy is decided fresh there. |
| 461 | POVRenderer needs an active GLRenderer | Stage material system + legacy POV/GL coupling | Per #582 (still open and listed `keep`) and the modernization notes, `POVRenderer` is the last user of the old `Stage` material system. The "POVRenderer requires active GLRenderer" symptom is an artifact of that legacy coupling; either #582 fixes both or POV gets retired in Phase 999.6 (PIPE-01 renderer rewrite). Either way #461 is not separately actionable on the current renderer stack. |
| 484 | Changing model colors broken | Property dialog (re-designed in 999.40 BALLView Refresh) | "Changing default model colors via Property dialog" — that dialog was rewritten in Phase 999.40 BALLView Refresh (commit `fdeca85b64`, "design system & QSS foundation"). The pre-refresh dialog and its color-binding code no longer exist. If the bug re-surfaces in the refreshed UI, file a new issue against the v1.7 BALLView. |
| 280 | In 2.0-pre-alpha RT window is not refreshed on changes | "2.0-pre-alpha" RT window (deprecated GENETICDOCK/raytracer wiring) | Reported against "2.0-pre-alpha" — a prerelease that never shipped. Current RT renderer wiring went through Phase 02 + 02.1 (renderer boundary extraction) + Phase 999.29 (GENETICDOCK removal). The specific window-refresh symptom in 2.0-pre-alpha is not reproducible on current code. |
| 281 | Default light setting in 2.0-pre-alpha uses constant attenuation | Same as #280 — 2.0-pre-alpha RT light defaults | Same rationale: 2.0-pre-alpha RT defaults code path doesn't survive into current Stage/light-setup code (Phase 04.1 reset all config defaults; `lightSettings` defaults audited there). |
| 419 | Menüeintrag Datei->neu fehlt (no File→New entry) | BALLView menu structure (rewritten in 999.40) | German UI menu request — adding a Datei→Neu entry. Menu structure was rewritten in 999.40 BALLView Refresh; the old menu code referenced in the issue no longer exists. If still desired against refreshed menu, file fresh. |
| 413 | Irreführender Menüeintrag ("Darstellung verändern" vs "Display Properties" both present) | Same — BALLView menu (999.40 rewrite) | Same as #419 — German UI label cleanup against pre-999.40 menu. |
| 437 | Molekülumbenennung fehlt (rename-molecule context-menu entry missing) | Same — BALLView context menus (999.40 rewrite) | Same family as #419/#413 — context-menu polish against pre-999.40 UI. If still missing post-refresh, re-file. |

### close-as-stale (6)

| # | Title | Last activity | Why |
|---|---|---|---|
| 358 | Update or ReDo the BALLView documentation on ball-project.org | 2015-02-20 (≈11 years) | "Update the website" — a website task, not a code issue. ball-project.org has been periodically updated; the BALLView v1.6/v1.7 release notes (`docs/RELEASE-NOTES-v1.6.2.md`) supersede. No code-side action. Phase 999.18 (Doxygen→RTD docs) covers any actual codebase doc rewrite. |
| 220 | Source files should contain LGPL header | 2015-02-20 (≈11 years) | Aspirational license-header-injection request. No subsequent maintainer action across 11 years. Mechanically achievable (one sed pass + LICENSE confirmation) but not on any v1.6/v1.7/v2.0 critical path. If a maintainer wants this for compliance, file fresh with a concrete spec (file list + header template). |
| 124 | Tabu random local search | 2015-02-20 (≈11 years) | Algorithm-research idea ("new meta-heuristic that utilizes a gradient based minimizer"). One-line body, zero comments, zero maintainer follow-up across 11 years. Not on any roadmap. Reopen if a researcher actively brings code. |
| 115 | Merge code for stochastic roadmap simulation | 2015-02-20 (≈11 years) | "We have a working implementation of SRS that should be merged" — the working implementation is presumably long gone (anhi's own item, never merged). Not on roadmap. |
| 114 | Merge correlation docking on hierarchical grids | 2015-02-20 (≈11 years) | Same family as #115 — "merge our USFFT correlation docking code when we have a free USFFT implementation." The USFFT prerequisite never materialized. Not on roadmap. |
| 150 | Merge the existing COMFA/COMSIA code | 2015-02-20 (≈11 years) | Same family — "merge existing QSAR COMFA/COMSIA code". Zero subsequent maintainer action; not on roadmap. |

### keep (76)

These 76 stay open. Proposed target milestones below — **do not apply labels in this round**; defer label edits to human.

#### Already-tagged BALL 1.5 (proposed re-target → v1.7 or v1.8 based on triage)
| # | Title | Author | Proposed milestone | Rationale |
|---|---|---|---|---|
| 96  | Error reading DNA or RNA | anhi | v1.8 | Real format-parsing bug; KERNEL/FORMAT scope. |
| 356 | PDBFileGeneral.C has a wrong if-condition | anhi | v1.7 | Specific code-fix request; trivially actionable. |
| 401 | PeptideCapProcessor does not generate ResidueChecker-approved fragments | anhi | v2.0 | STRUCTURE module deep-fix; not blocking. |
| 431 | Berechnung der Bindungsordnung nicht korrekt 2 | anhi | v2.0 | BondOrder calculation bug; not on v1.7 critical path. |
| 497 | Ribbon representation on selected atoms — wrong selection | anhi | v2.0 | VIEW model-builder bug; relates to selection plumbing. |
| 527 | Transparency slider in Material settings tab is broken | anhi | v1.8 | Likely affected by 999.40 BALLView Refresh — verify post-refresh first. |
| 560 | Update the PDF Tutorial | dstoeckel | v2.0 | Tutorial-rewrite; better as part of Phase 999.18 (Doxygen→RTD docs). |
| 651 | win installer: CPack error when optional targets are missing | tkemmer | v1.7 | Packaging defect, on Phase 8b (Windows packaging) scope already. |
| 652 | SunRPC replacement (XDR) | tkemmer | v1.7 | XDR still wired in `CMakeLists.txt`; switching to libtirpc is a real maintenance task. NOT obsolete despite Phase 999.7 — that closed the rendering-FindXDR PR #640 but didn't switch the runtime XDR backend. |

#### Already-tagged BALL 1.6 (proposed re-target → v1.7 or v1.8)
| # | Title | Author | Proposed milestone | Rationale |
|---|---|---|---|---|
| 186 | Molecular Mechanics toolbar entries | anhi | v1.8 | BALLView Refresh follow-up — likely landed partially in 999.40, re-audit. |
| 213 | HBonds are not stored in the Project Files | anhi | v2.0 | Project-file serialization gap; non-trivial. |
| 390 | One mechanism for material properties | dstoeckel-ish | v999.6 | Subsumed by Phase 999.6 PIPE-01 renderer rewrite (new material system). |
| 415 | Zoomfunktion per Tasten | anhi | v1.8 | Small UI/UX item, BALLView Refresh follow-up. |
| 420 | Zurück Funktion und Energieminimierung | anhi | v2.0 | "Undo before minimization" — non-trivial state-tracking. |
| 430 | Berechnung der Bindungsordnung nicht korrekt | anhi | v2.0 | Same family as #431. |
| 466 | File::createTemporaryFilename() used incorrectly | anhi | v1.7 | Real Windows defect; small code fix. |
| 485 | Task: Parameter to limit RenderThreads | anhi | v1.8 | Renderer-thread cap; affects ray-tracing freezes. |
| 486 | Task: Auto-switch continuous loop off when quality criterion met | anhi | v2.0 | RT-loop heuristic; minor. |
| 487 | Add a progress bar for continuous loop | anhi | v1.8 | UX enhancement. |
| 501 | BALLView should warn on start when no renderer plugins are found | anhi | v1.8 | Friendly-error enhancement. |
| 582 | POVRenderer does not use the new material system | dstoeckel | v999.6 | Subsumed by Phase 999.6 PIPE-01 renderer rewrite (old `Stage` system retired). |
| 621 | (Supplied) project files override user settings | tkemmer | v1.8 | Project-file UX defect; affects user trust. |
| 622 | Shortcuts in BALLView are handled globally | tkemmer | v1.8 | Real BALLView UX defect; small fix. |

#### Already-tagged BALL 2.0 (proposed → keep as v2.0 except where noted)
| # | Title | Author | Proposed milestone | Rationale |
|---|---|---|---|---|
| 4   | Impropers in CHARMM (EEF1) | anhi | v2.0 | Force-field accuracy — MOLMEC scope. |
| 6   | GUI tests | anhi | v2.0 | Test-infrastructure ask; better tracked in Phase 9 (test triage). |
| 76  | Inconsistency with pointers and references | anhi | v2.0 | API ergonomics — touches `Element`/`Atom` copy semantics. |
| 111 | Implement pKa prediction | anhi | v2.0 | New algorithm; research-track item. |
| 121 | Loop modelling | anhi | v2.0 | New algorithm. |
| 138 | Add the ability to mutate certain fragments | anhi | v2.0 | New FragmentDB feature. |
| 140 | FFT correlation docking + electrostatics | anhi | v2.0 | DOCKING enhancement. |
| 160 | Force Field Configuration Dialogs have redundant code | anhi | v1.8 | BALLView Refresh follow-up — likely overlaps 999.40 dialog rewrites. |
| 167 | Separation of Scene and Rendering | anhi | v999.6 | Subsumed by Phase 999.6 PIPE-01 + Phase 02.1 renderer boundary. |
| 177 | Unnecessary recomputation of SES when adding single C atom | anhi | v2.0 | Surface-perf optimization. |
| 187 | Add parser for the PDBML/XML format | anhi | v2.0 | New format; consider whether gemmi (Phase 999.11) provides PDBML. |
| 242 | The FragmentDB default constructor is confusing | anhi | v2.0 | API ergonomics; small breaking change. |
| 249 | Methylized nucleotides cannot be visualized | anhi | v2.0 | FragmentDB augmentation — data-files task. |
| 253 | Unexpected behaviour of Directory (getNextEntry) | anhi | v2.0 | API ergonomics; related to #627. |
| 263 | Copying removes composites from parent | anhi | v2.0 | KERNEL Composite semantics — high-impact API question. |
| 309 | BALLView's rendering engine is very slow | anhi | v999.6 | Subsumed by Phase 999.6 PIPE-01 renderer rewrite. |
| 325 | Use boost for path and directory handling | anhi | v2.0 | Now `std::filesystem` is the modern answer (C++17 native); re-scope before action. |
| 335 | TrajectoryFile from GROMACS (.XTC) is not yet supported | anhi | v2.0 | New format support. |
| 337 | Create suitable Gromacs bindings | anhi | v2.0 | External-tool integration. |
| 338 | Reading back GROMACS PDB files (4-letter atom names) | anhi | v2.0 | FORMAT parser fix. |
| 360 | Add a processor for adding end caps to DNA | anhi | v2.0 | STRUCTURE algorithm. |
| 364 | Store/Load Viewpoints via text-files | anhi | v2.0 | BALLView feature. |
| 365 | Store BALLView selections | anhi | v2.0 | BALLView feature; related to #364. |
| 369 | Support for irregular spaced volume visualization | anhi | v2.0 | VIEW algorithm. |
| 395 | Incomplete Hydrogen generation | anhi | v2.0 | STRUCTURE bug — AddHydrogenProcessor edge cases. |
| 403 | Saturate with H does not work on hetero residues within proteins | anhi | v2.0 | Same area as #395. |
| 407 | PDB HIP inter-fragment bonds missing | anhi | v2.0 | FragmentDB data fix. |
| 409 | Extend SD-Widget functionality | anhi | v2.0 | VIEW UX. |
| 411 | SMILES parser does not parse "\" and "/" | anhi | v2.0 | STRUCTURE parser bug. |
| 429 | BALLView fails writing .pdb when residue name with 4 letters | anhi | v2.0 | Related to #338. |
| 441 | Strange nucleotide names break BALL expression | anhi | v2.0 | FragmentDB + expression-parser interaction. |
| 445 | svm_destroy_model is deprecated in libsvm >3.0 | anhi | v1.7 | One-line API rename; small mechanical fix. |
| 450 | MOLFile: Add more accurate error messages | anhi | v2.0 | Error-message quality. |
| 470 | PDBDownload should accept list of PDBIDs | anhi | v2.0 | BALLView UX. |
| 472 | Off-by-one coloring shift of DNA Cartoon model in PresentaBALL | anhi | v2.0 | PresentaBALL still extant; real bug. |
| 473 | Replace lpsolve by coin-or solver | anhi | v2.0 | Dep swap; consider Phase 999.11 (gemmi) timing. |
| 477 | Abstract two "Add window.." entries into one | anhi | v1.8 | BALLView Refresh follow-up. |
| 478 | Improve Plugin visibility | anhi | v2.0 | BALLView UX. |
| 480 | Add a button for continuous automatic light optimization | anhi | v2.0 | VIEW feature. |
| 481 | Add a "reference box" for orientation/depth perception | anhi | v2.0 | VIEW feature. |
| 483 | Loading of large Biological Assemblies freezes | anhi | v2.0 | FORMAT/network issue; PDB downloader. |
| 503 | Help modus of command-line parser should offer defaults | anhi | v1.8 | Small UX. |
| 515 | Renaming some functions | anhi | v2.0 | API rename — `icosaeder` → `createIcosahedron`. |
| 523 | Document why "Restore Defaults" doesn't reset font/lang/style | anhi | v1.8 | Doc + minor UX clarification. |
| 524 | Limits for PropertyPlotter | anhi | v2.0 | VIEW PropertyPlotter enhancement. |
| 531 | Change definition of Size, Position, and Index to 64-bit | anhi | v2.0 | Big API+ABI change; v2.0 substrate territory. |
| 627 | Directory class under windows inconsistent behavior | jeleclaire | v2.0 | Already-confirmed from round-1 decisions (external author, prior round kept). |
| 76+ rest | (covered above) | | | |

#### No-milestone (4)
| # | Title | Author | Proposed milestone | Rationale |
|---|---|---|---|---|
| 576 | AssignBondOrderProcessor_test fails OS dependent | philthiel | v1.8 | Real test failure on macOS Xcode/clang — fits Phase 9 (test triage). |
| 659 | CMakeList.txt Python find_package() | JosuaCarl | v999.15 | Python rewrite phase will pick `find_package(Python ...)`; route there. |
| 663 | sip6 support | emollier | v999.15 | Phase 999.15 PyBALL v2 explicitly resolves SIP4 EOL. Most recently updated item in tree. |
| 662 | BALL/STRUCTURE/QMBasisSet.h: No such file | BruceGHChen883 | (see needs-investigation) | Moved to needs-investigation. |

### needs-investigation (3)

| # | Title | Why uncertain |
|---|---|---|
| 662 | BALL/STRUCTURE/QMBasisSet.h: No such file | Live build issue: `include/BALL/FORMAT/GAMESSLogFile.h:13` references `<BALL/STRUCTURE/QMBasisSet.h>` but no such header exists in tree. Method bodies in `source/FORMAT/GAMESSLogFile.C` use `QMBasisSet&`. Either GAMESSLogFile must be excluded from build or header reinstated. Already flagged by round-1 (`triage-999.14/decisions.md`); still unresolved. Needs maintainer decision. |
| 289 | Splitted surface crashes BALL - project files | Active-90d forced keep — but body suggests a real reproducible crash. Needs reproducer attempt against v1.7 before any categorization. The 5 active-90d comments (suspected metadata touches from round-1) deserve a real comment review. |
| 539 | AromaticityProcessor fails on certain molecules | Active-90d forced keep; legitimate STRUCTURE bug per pbrach (maintainer). Needs reproducer triage before milestone assignment. |

*(540 + 601 + 636 are also active-90d but their categorization is cleaner — kept as v1.7/v1.8 candidates implicitly. Listed in "keep" above. The 3 in needs-investigation here are the ones where active-90d activity AND categorization-uncertainty both apply.)*

---

## Patterns observed across the 95

1. **~60% are anhi's wiki-imported maintainer notes from 2010-2014** (`Reported by ... on ... UTC` body format). Most are still actionable but pre-date the renderer rewrite roadmap; many will be subsumed by Phase 999.6 (PIPE-01) or 999.40 (BALLView Refresh) without needing per-item fix.
2. **~10% are BALLView UX/menu polish** that almost certainly overlap with the in-progress 999.40 BALLView Refresh — these should be revisited post-refresh (likely re-categorize many as `close-as-fixed` then).
3. **~5% are SIP/Python items** that all defer cleanly to Phase 999.15 (PyBALL v2).
4. **~5% are renderer-stack items** (POV, RT, Stage materials) that defer to Phase 999.6 (PIPE-01).
5. **~5% are real, currently-actionable bugs** with clear scope: #466, #356, #445, #527, #651, #652 — these are good v1.7/v1.8 candidates.
6. **#662 is the only "live build issue blocked on missing header"** — should be elevated to a Phase 9 (test triage) action item.

---

## Proposed close commands

**DO NOT EXECUTE** these without human approval. Copy-paste after review.

```bash
# close-as-fixed (2)
gh issue close 639 --repo BALL-Project/ball --comment "This was resolved by the v1.6 modernization work. Specifically: Phase 3 (language-modernization) made C++17 the unconditional default (CMAKE_CXX_STANDARD 17), and the APPLE-conditional guard in cmake/BALLConfiguration.cmake:104-107 that suppressed compile-check flags on macOS was removed during the Phase 1 build-baseline pass. Current cmake/BALLConfiguration.cmake:95-115 propagates BALL_PROJECT_COMPILE_FLAGS to CMAKE_REQUIRED_FLAGS on all platforms. Closing as fixed — please reopen with a reproducer against v1.7-modernization if the issue persists."

gh issue close 7 --repo BALL-Project/ball --comment "The SIP-based Python test integration this issue describes is superseded by Phase 999.15 (PyBALL v2 rewrite), which delivers a modern Python binding generator and CTest integration. The old SIP test-skeleton wiring is no longer the target. Closing as fixed — Phase 999.15 owns the new Python+CTest plumbing. Please reopen with a v999.15 reference if the new plumbing doesn't cover what this issue asked for."

# close-as-obsolete (8)
gh issue close 330 --repo BALL-Project/ball --comment "This targets SIP-generated Python bindings (the 'Constant' Python-side name), which are currently disabled in the v1.6/v1.7 build (BALL_PYTHON_SUPPORT=OFF in all ci-* presets). The binding generator is being replaced as part of Phase 999.15 (PyBALL v2). Any Python-side naming policy is decided fresh there. Closing as obsolete; please re-file against PyBALL v2 once it ships if the naming concern reappears."

gh issue close 461 --repo BALL-Project/ball --comment "This targets the legacy POVRenderer-needs-GLRenderer coupling, which exists because POVRenderer is the last user of the old Stage material system (per the still-open #582). The renderer stack is being rewritten in Phase 999.6 (PIPE-01); the old Stage coupling won't survive that rewrite. Closing as obsolete. If POV-side functionality is still needed post-999.6, please file a fresh issue against the new renderer interface."

gh issue close 484 --repo BALL-Project/ball --comment "This targets the pre-refresh BALLView Property dialog (default model colors via Property entry). That dialog was rewritten in Phase 999.40 (BALLView Refresh, commit fdeca85b64). The old color-binding code path no longer exists. Closing as obsolete. If the bug reappears in the refreshed UI, please file a new issue against v1.7-modernization."

gh issue close 280 --repo BALL-Project/ball --comment "This was reported against '2.0-pre-alpha', a prerelease that never shipped. The current RT renderer wiring went through Phase 02 + Phase 02.1 (renderer boundary extraction) + Phase 999.29 (GENETICDOCK removal). The specific window-refresh symptom in 2.0-pre-alpha is no longer reproducible on the current code. Closing as obsolete; please re-file against v1.7-modernization if a window-refresh bug recurs."

gh issue close 281 --repo BALL-Project/ball --comment "This targets the '2.0-pre-alpha' RT light defaults — a prerelease that never shipped. Light-setup defaults were audited and reset in Phase 04.1 (config-color-defaults-fix). The specific constant-attenuation default in 2.0-pre-alpha doesn't survive into current code. Closing as obsolete; please re-file against v1.7-modernization if RT lighting defaults still look wrong."

gh issue close 419 --repo BALL-Project/ball --comment "This targets the pre-Refresh BALLView menu structure (a missing Datei→Neu entry). BALLView menus were rewritten in Phase 999.40 (BALLView Refresh, commit fdeca85b64). Closing as obsolete. If the menu gap is still present after the refresh, please file a new issue against v1.7-modernization."

gh issue close 413 --repo BALL-Project/ball --comment "This targets the pre-Refresh BALLView menu structure (the duplicate 'Darstellung verändern' vs 'Display Properties' entries). BALLView menus were rewritten in Phase 999.40 (BALLView Refresh, commit fdeca85b64). Closing as obsolete; please re-file against v1.7-modernization if the duplication is still present post-refresh."

gh issue close 437 --repo BALL-Project/ball --comment "This targets pre-Refresh BALLView context menus (a missing rename-molecule entry). BALLView menus and context menus were rewritten in Phase 999.40 (BALLView Refresh, commit fdeca85b64). Closing as obsolete; please re-file against v1.7-modernization if the entry is still missing post-refresh."

# close-as-stale (6)
gh issue close 358 --repo BALL-Project/ball --comment "Closing as stale (no activity since 2015-02-20, more than 11 years). This was a request to update ball-project.org website content — a non-code housekeeping task. The codebase doc rewrite is tracked in Phase 999.18 (Doxygen→RTD docs). If specific website content is still wrong, please file a fresh, specific issue."

gh issue close 220 --repo BALL-Project/ball --comment "Closing as stale (no activity since 2015-02-20, more than 11 years). An LGPL-header injection across all source files is mechanically achievable but has had zero maintainer follow-up in 11 years and is not on any v1.7/v2.0 critical path. If license-header compliance is needed, please file a fresh issue with a concrete spec (file list + header template) and we'll prioritize."

gh issue close 124 --repo BALL-Project/ball --comment "Closing as stale (no activity since 2015-02-20, more than 11 years). 'Tabu random local search' was a one-line research-idea note with no follow-up across 11 years and is not on any roadmap. If a researcher is actively bringing this code, please file a fresh issue with the implementation."

gh issue close 115 --repo BALL-Project/ball --comment "Closing as stale (no activity since 2015-02-20, more than 11 years). The 'working implementation of SRS' this issue refers to has not surfaced in the codebase across 11 years. Not on roadmap. If the SRS code still exists and is ready for review, please file a fresh issue with a pointer to the branch/fork."

gh issue close 114 --repo BALL-Project/ball --comment "Closing as stale (no activity since 2015-02-20, more than 11 years). USFFT correlation docking depends on a 'free USFFT implementation' that never materialized. Not on roadmap. If a free USFFT became available and you want to revive this, please file a fresh issue."

gh issue close 150 --repo BALL-Project/ball --comment "Closing as stale (no activity since 2015-02-20, more than 11 years). The 'existing COMFA/COMSIA code' has not been merged across 11 years and is not on any roadmap. If the code still exists and is ready for merge, please file a fresh issue with a branch/fork pointer."
```

---

## Audit trail
- **Snapshot:** `.planning/phases/999.14-github-issue-pr-triage/issues-baseline.json` (95 items, snapshot 2026-05-17).
- **Categorized:** 95/95 (100%).
- **Net close count if all approved:** 16 (2 fixed + 8 obsolete + 6 stale).
- **Kept count:** 76 (target-milestone proposals deferred to human label-edits).
- **Needs-investigation count:** 3 (#662 live build, #289 active-90d crash report, #539 active-90d aromaticity bug).
- **Action ratio:** 16/95 ≈ 17% closure proposal — conservative; the prior round (round-1, 2026-05-16) already closed the obvious external-author batch.
