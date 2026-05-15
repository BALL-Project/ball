# BALLView v1.6.1 — corrective patch for v1.6.0 (DRAFT)

**Status:** draft (committed to `.planning/` ahead of tag; final version posted to GitHub Release when v1.6.1 ships)

**v1.6.1 is the strict corrective release for v1.6.0.** Same modernization payload, but with the Windows-installer-shipping pipeline aligned to the Qt 6 codebase, the macOS bundle metadata populated, the persisted-config color-defaults user-facing bug fixed (Phase 4.1), and the Phase 5.1 source-level warning census applied. No new features — purely "ship what v1.6.0 should have shipped."

If you're using v1.6.0, **upgrade to v1.6.1**. The v1.6.0 release is left tagged for historical reference; the v1.6.0 release notes have been updated to point at v1.6.1.

---

## What's in v1.6.1

### Critical: release pipeline aligned with the Qt 6 codebase

v1.6.0 shipped from a Qt 5 codebase. [Phase 5 (2026-05-15)](https://github.com/BALL-Project/ball/tree/v1.6-modernization/.planning/phases/05-qt-6-migration-4b-renderer-backend-decision-spike) migrated to Qt 6, but `.github/workflows/release.yml` was never updated — it still installed `qt@5` via Homebrew and called `qt@5/bin/macdeployqt`. Any v1.6.1 release attempt would have failed at CMake configure.

**Fix:** `release.yml` now installs `qt` (Qt 6 on current Homebrew) and uses `qt/bin/macdeployqt`. CI's `release.yml` workflow runs against the same Qt 6 toolchain as `ci.yml` for the first time.

### macOS bundle identity populated

v1.6.0's `BALLView.app/Contents/Info.plist` had empty `CFBundleVersion`, `CFBundleShortVersionString`, and `CFBundleLongVersionString` because `${PROJECT_VERSION}` was shadowed by BALLView's nested `PROJECT(BALLView)` declaration (under `CMP0048 NEW`, the inner `PROJECT()` without a `VERSION` argument resets the variable).

**Fix:** `set_target_properties(BALLView ... MACOSX_BUNDLE_*)` now uses `${CMAKE_PROJECT_VERSION}` (always the top-level project version `1.6.0`). All seven bundle-identity keys are populated:

```
CFBundleIdentifier         de.uni-tuebingen.ball.ballview
CFBundleName               BALLView
CFBundleVersion            1.6.1
CFBundleShortVersionString 1.6.1
CFBundleLongVersionString  BALLView 1.6.1
CFBundleGetInfoString      BALLView 1.6.1 — molecular visualization (LGPL-2.1)
NSHumanReadableCopyright   © BALL maintainers. LGPL-2.1.
```

Phase 8 (notarization) prerequisite cleared — `notarytool` no longer rejects on empty `CFBundleIdentifier`/`CFBundleVersion`.

### User-facing bug fix: persisted config no longer shadows compiled color defaults

[Phase 4.1](https://github.com/BALL-Project/ball/tree/v1.6-modernization/.planning/phases/04.1-config-color-defaults-fix) — persisted `~/.BALLView` config was silently shadowing the compiled element/residue color defaults. Stale or partial saved color blocks (the documented v1.6 case had indices 0-20 all `ffffffff` white) overrode the in-source defaults; the only workaround was deleting `~/.BALLView`. Phase 4.1 ships:

- Version/checksum-tagged color block with fallback-on-mismatch
- All-white sanity check (stale saved data → use compiled defaults)
- "Reset element colors to defaults" Preferences action (no `~/.BALLView` deletion required)

### Phase 5.1: Source-level warning census + latent bug cleanup (14 plans + 2 in-cycle gap fixes)

Phase 4's tri-OS CI matrix surfaced a large warning surface that Phase 5.1 cleaned up. Tier-A correctness fixes, Tier-B Windows DLL hygiene, Tier-D build configuration:

**Tier A — correctness (real bugs):**
- `C4717` infinite recursion in `std::getline(istream&, BALL::String&&, ...)` ([include/BALL/DATATYPE/string.iC](https://github.com/BALL-Project/ball/blob/v1.6.1/include/BALL/DATATYPE/string.iC)) — guaranteed stack overflow on any caller, now fixed by dropping the spurious `std::move` wrap. Codex-verified.
- `C4311` pointer truncation `T* → long` on 64-bit Windows (LLP64) in `hash.C`, `MMFF94StretchBend.C`, `triangulatedSurface.C`. End-to-end audit: cast sites widened to `BALL::PointerSizeUInt` (existing typedef in `global.h:207`); downstream `HashMap<long, Position>` storage migrated to match. Codex-flagged D-08 audit confirmed only function-local storage was affected (no header changes needed).
- `-Wself-assign-field` at `pairExpInteractionEnergyProcessor.C:124` — copy-assign operator was `rdf_parameter_ = rdf_parameter_;` (copy-paste bug). Fixed.
- `-Wtautological-constant-out-of-range-compare` at `representation.C:269-270` — three tautological halves dropped while preserving the meaningful `DrawingPrecision < 0` sentinel check. Plan executor caught a Codex/BACKLOG misclassification: clang was actually warning on the `> MAXIMAL_*` upper-bound halves, not the `< 0` halves.
- `-Wformat-overflow` ×20 in the Bison-generated CIF parser — `sprintf` → `snprintf` with explicit buffer sizing across the grammar.
- `-Wstringop-truncation` ×10 in PDB / CIF lexers — `strncpy` → `memcpy + explicit nul-termination` for every flagged site (plus 17 structurally-identical neighbors for consistency). Introduced a single `static inline CIF_copy_text` helper to consolidate the lexer pattern.

**Tier B — Windows DLL hygiene:**
- `C4910` `extern template class BALL_EXPORT` mismatch in `vector3.h` + `atom.h` — moved `BALL_EXPORT` from the `extern template class` declarations to the matching `template class` definitions in the `.C` files. Canonical pattern for DLL-exported instantiations.
- `C4251` STL-members-of-DLL-exported-classes — project-wide `#pragma warning(disable: 4251)` under `BALL_COMPILER_MSVC` in `global.h:54`. Cosmetic-only on BALL's supported matrix (Phase 4's vcpkg pin guarantees single-MSVC-toolchain across libBALL + clients).
- `C4996` `BALL::GeneticIndividual` deprecation noise (5 instances) — un-deprecated the class because no replacement exists and the class is still load-bearing for `GeneticAlgorithm` / `EvolutionaryDocking`. The deprecation was wrong; rationale documented in source.
- `C4834` discarded `[[nodiscard]]` at `regressionModel.C:258` — deleted the dead `descriptor_IDs_.begin();` line (pure observer, real iterator obtained 3 lines below).

**Tier D — build configuration:**
- `Qt6LinguistTools` not found warning on Windows vcpkg — `QUIET` on `find_package` + downgrade `MESSAGE(WARNING)` → `MESSAGE(STATUS)`. `qttools` port added to `vcpkg.json` as root-cause fix.
- GitHub Actions Node 20 deprecation — `actions/checkout@v5` + `actions/cache@v5` Node-24-native pins across `ci.yml` + `release.yml`.
- Linux apt-archives cache `tar exit code 2` warning — narrowed `path: /var/cache/apt/archives` → `path: /var/cache/apt/archives/*.deb` (excludes root-owned `lock` + `partial/`). BACKLOG had misclassified the failing step as ccache; actual fix was at apt-archives.
- Windows release `LNK1104: tbb12_debug.lib` — `--config Release` already in place in both `ci.yml:315` (from `e63f061`) and `release.yml:193` (from `381c129`); Plan 13 closed as no-op verification.
- Windows release vcpkg baseline regression repair — `08ec9c5` re-applied the earlier `8bd5294` fix (drop unresolvable `builtin-baseline` from `vcpkg.json` — the pinned commit doesn't exist in the GitHub runner's vcpkg checkout).

**CI infrastructure carry-forward (4 fixes verifying tri-OS green):**
- Linux: dropped `modules:` from `jurplel/install-qt-action` (Qt 6.5+ rejects every module name in BALL's prior list — they're all default-installed).
- Windows: `CIFParserParser.y` `yyerror(char*)` → `yyerror(const char*)` (MSVC rejects implicit `const char*` → `char*` conversion that gcc/clang accept).
- Windows: `source/SYSTEM/directory.C` `_T(variable)` macro misuse fixed across 28 sites — `_T()` is for string LITERALS, not variables; swapped to explicit `*A` Win32 calls (`CreateFileA`, `FindFirstFileA`, `FindNextFileA`, `WIN32_FIND_DATAA`).
- Windows: `atom.h:1010` `extern template class std::vector<Atom*>;` guarded by `!BALL_BUILD_DLL` — MSVC's pointer-element-vector implicit instantiation tripped C4910 on libBALL's own TUs; vector3.h's same pattern doesn't trip because value-element vectors don't share the instantiation path.

### Documentation

- `.planning/phases/05.1-build-warnings-and-latent-bugs/05.1-LEARNINGS.md` — 9 decisions, 7 lessons, 6 patterns, 8 surprises extracted from Phase 5.1 (BACKLOG misclassification patterns, Codex cross-check value, vcpkg baseline trap, nested-PROJECT shadow).
- `.planning/REVIEW-v1.6.1.md` — synthesis adversarial review of the v1.6.1 plan (claude + codex). 4 critical findings, 6 significant concerns, 5 notable risks.
- `.planning/MILESTONE-CONTEXT.md` — v1.6.1 milestone scope, release criteria, deferrals to v1.6.2 and v2.0.

---

## Migration from v1.6.0

No API changes. v1.6.1 is a drop-in replacement for v1.6.0:

1. Download `BALLView-v1.6.1-macos-arm64.zip` (macOS) or `BALLView-v1.6.1-windows-x64.zip` (Windows).
2. On macOS, the ad-hoc resign step from v1.6.0 is preserved — `codesign --verify --deep --strict` passes; right-click → Open the first launch (standard for non-notarized bundles on macOS 26+).
3. If you have a `~/.BALLView` config that shipped with stale color overrides, Phase 4.1's auto-recovery will detect and use compiled defaults; no manual cleanup needed.

---

## Out of scope (deferred to v1.6.2 / v1.7 / v2.0)

Per the synthesis review:

- **Upstream PR triage** (#640 FindXDR.cmake, #600 Travis CI, #554 omega torsion, #550 HYP residue, #546 insertion-code printing) — deferred to v1.6.2 via existing Phase 999.5 slot; PRs #554/#546 partially-overlap with the current `Residue` API and need archaeology before cherry-pick.
- **Phase 8 macOS notarization slice** — requires Apple Developer ID Application certificate; deferred to v1.6.2 or v1.7 depending on certificate availability.
- **Phase 6 Python bindings** — vertical-slice decision spike (SIP 6 vs pybind11 vs nanobind); v1.7-track work.
- **Phase 9 test-suite triage** — `b2bb718` + `61bf5a7` landed test infrastructure during the v1.6.1 cycle; treated as v1.6.2 deliverable for accounting clarity.
- **Tier-C warning bulk cleanup** (~3700 instances of `-Wdeprecated-copy`, `-Wunqualified-std-cast-call`, `-Wcatch-value`) — deferred to v1.6.2 / v1.7.
- **v2.0 substrate-modernization** — Ninja generator, PIPE-01 renderer rewrite, YAML config migration, REST API + PyBALL SDK, gemmi mmCIF parser, deprecated-code mop-up, Read the Docs site + Swagger UI. See `.planning/ROADMAP.md`.

---

## Acknowledgements

Phase 5.1's source-level cleanup was Codex CLI 0.128.0 cross-checked (`codex exec --sandbox read-only`) on 2026-05-15. The synthesis adversarial review pre-tag was claude + codex 0.128.0 in parallel; codex caught two showstoppers that the orchestrator missed (release.yml Qt 5 mismatch, PRs #554/#546 partial-overlap with current `Residue`).

---

*v1.6.1 was tagged on 2026-05-15 from `v1.6-modernization` HEAD.*
