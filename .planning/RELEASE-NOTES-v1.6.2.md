# BALLView v1.6.2 — warning-cleanup + ARM Linux + Qt 6.8 LTS baseline (DRAFT)

**Status:** draft (committed to `.planning/` ahead of tag; final version posted to GitHub Release when v1.6.2 ships)

**v1.6.2 is a corrective + hygiene release for v1.6.1.** Same binary surface, same supported platforms, no new user-facing features. The headlines are: **Qt 6.8 LTS** as the new project floor (enables ARM Linux desktop binaries), **ARM Linux (ubuntu-24.04-arm) added to the CI matrix as a blocking peer** of x86_64, and **the Windows MSVC + Linux gcc warning surfaces cleared from ~4 000 to under ~10**. Plus several latent bug fixes flushed out by the warning census (chief among them: a real `va_start` undefined-behavior site in `PDBFile::writeRecord_` that had been latent in the PDB writer since the codebase was first targeted at C++17, and four `QFile::open()` discarded-bool sites where failure was silently producing zero-byte downloads).

If you're on v1.6.1, **upgrading to v1.6.2 is recommended but optional** — v1.6.1 functions correctly; v1.6.2 is cleaner and more diagnosable.

---

## What's new in v1.6.2

### Qt 6.8 LTS as the project-wide floor

`QT_MIN_VERSION` bumped from **6.5 → 6.8 LTS** across all platforms. The bump was driven by ARM Linux: `aqtinstall` has no Linux ARM64 desktop binaries for Qt 6.5 / 6.6 / 6.7 — they were first published with Qt 6.8 LTS (October 2024). Pinning x86_64 to 6.8 too keeps the toolchain uniform across the matrix and enables consistent use of Qt 6.8+ APIs (`QDomDocument::ParseOptions`, `QMouseEvent::globalPosition`, etc.).

Build implications:
- macOS: Homebrew Qt 6.11+ satisfies the floor (no action required).
- Linux x86_64 + ARM64: CI installs Qt 6.8.3 via `jurplel/install-qt-action`; users on Ubuntu 24.04 whose `apt`-shipped Qt is 6.4.2 will need to install Qt 6.8+ separately.
- Windows: vcpkg pulls Qt 6.8+ via the existing manifest.

### ARM Linux (ubuntu-24.04-arm) added to CI as a blocking platform

The CI matrix grew a fourth blocking build job: `build (linux-arm64)` on `ubuntu-24.04-arm` (free standard GitHub-hosted runner since late 2024). Two consecutive green runs (25963533636 + 25963900386) established the ARM baseline on the new Qt 6.8 floor, after which the entry was flipped from `blocking: false` → `blocking: true` to match `linux-x64` / `macos-arm64` / `windows-x64`. ARM-specific regressions (alignment, byte-order, char-signedness defaults, pointer-truncation under LP64-vs-LLP64 differences) are now caught at PR time.

The first ARM regression caught and fixed by this matrix entry was the **`-Wnarrowing` error on `hashGrid.C`'s neighbour-offset table** — ARM gcc defaults `char` to UNSIGNED, so the `-1` literals in the 27×3 table tripped `-Wnarrowing` errors that x86_64 (signed `char` default) had never seen. Fix: declare the table as `signed char` (commit `fdea8c7`). Cross-platform, no behavior change.

### CI cycle-time and signal improvements

- **`paths-ignore` for docs commits.** Push/PR triggers now skip the full 4-OS rebuild for `.planning/**`, `**.md`, `doc/**`, and `LICENSE*` changes (saves ~45–60 min of runner time per ROADMAP/CLAUDE.md/docs commit). Source dirs remain in-scope by default — the list is a deny-list, not allow-list.
- **macOS runner version aligned with OpenMS** (`macos-14` / Sonoma) for known-stable parity; **Windows bumped `windows-2022` → `windows-2025`** (VS 2022 17.x with current MSVC 14.4x).
- **Per-TU build profiling.** `Top-20 slowest TUs` step added to each platform (Phase 999.19-01); ninja-log artifact uploaded with each build for offline regression analysis. SIGPIPE-on-`sort | head` pipefail bug fixed so the telemetry step never silently fails a blocking job.
- **Windows CMake build-tree cache** (Phase 999.17-01) — separate from ccache; caches the configure step's output across runs, cutting Windows job time meaningfully on warm cache.
- **GitHub Actions bumps** (Phase 999.20-01) — `actions/upload-artifact@v4 → v6`, `actions/download-artifact@v4 → v7`. Both are pre-emptive bumps before the Node 20 deprecation deadline.

### Warning-surface cleanup (massive)

The pre-v1.6.2 warning census measured **~4 000 warnings on linux-arm64**, ~2 400 on linux-x64, ~1 800 on macos-arm64, ~700 on windows-x64 (already low after Phase 5.1). v1.6.2 cuts every platform to **under ~10 actionable warnings** (the residual being intentional `[[deprecated]]` markers on GENETICDOCK self-deprecated classes that the v2.0 cleanup phase will address).

The fixes split across two parallel work streams, both folded into v1.6.2:

**Stream A — file-by-file warning category execution (commits `5ef5509`, `d77aab3`, `94fcd73`, `85d991d`, `3519c34`, `cb392cc`, `c947b2d`):**
- `-Wcatch-value` → catch polymorphic `BALL::Exception::*` by `const&` instead of by value (5ef5509, d77aab3) — 35 sites
- Latin-1 source bytes → ASCII in TODO comments (94fcd73) — fixes MSVC C4828 source-encoding warnings
- `sprintf` / `vsprintf` → `snprintf` / `vsnprintf` across all platforms (85d991d) — fixes Apple SDK deprecation advisory + general security improvement
- `-Woverloaded-virtual` sweep on QSAR `simpleBase.h` subclasses (3519c34, c947b2d) — adds `using SimpleBase::operator=;` to ~40 derived classes, killing the largest single warning category (~2 800 hits on Linux ARM64)
- Init-list reorder + dead-write removal + empty-body-loop clarification (5ef5509)
- GCC `-Wmaybe-uninitialized` false-positive suppression on system headers (cb392cc)

**Stream B — codex-reviewed shortlist execution (commits `c6ecd48`, `82dba3c`, `24c044f`, `d730518`, `0a75ede`):**
- `c6ecd48` — Codex CLI adversarial-review-approved P0+P1 bundle: 11 fixes across 18 files
  - **P0: `PDBFile::writeRecord_` va_start UB.** The function declared `void writeRecord_(PDB::RecordType, ...)`, but the C++ standard's `va_start` macro requires the anchor parameter to be a type that does not undergo default argument promotion. Enums *can* be int-promoted, which is **undefined behavior** per [cstdarg]. Real latent bug — happened to work on x86_64 gcc/clang/MSVC but UB everywhere. Fix: change signature to `void writeRecord_(int record_type, ...)`, cast back to the strong enum after `va_start` for switch + format-table lookups. Callers unchanged (enum constants implicitly convert to int).
  - **P1: Defaulted copy-assign on iterator templates.** `(Const)BidirectionalIterator` + `(Const)RandomAccessIterator` had user-provided copy ctors but no user-provided copy-assign, triggering `-Wdeprecated-copy` on ~740 template instantiations. Added `Class& operator=(const Class&) = default;` to all four.
  - **P1: Qt 6 mechanical migrations** — `QMouseEvent::globalPos()` → `globalPosition().toPoint()` (6 sites), `QByteArray::count()` → `size()` (6 sites in `simpleDownloader.C`), `QDomDocument::setContent` 5-arg → `ParseOptions` overload returning `ParseResult` (2 sites), `QMenu::addAction(text, recv, slot, accel)` → `(text, accel, recv, slot)` (4 sites).
  - **P1: Smaller cleanups** — `kekulizer.h` `AtomInfo` default-init all members; `glRenderer` ctor mem-init reorder to match declaration order; drop dead `pm.createAlphaMask()` × 2 sites; fold `QTranslator::load()` `[[nodiscard]]` result; `Hash_test.C` `char*` → `const char*`.
- `82dba3c` — Windows MSVC trio: silence C4834 in `simpleDownloader.C:134` (real latent bug — discarded `QFile::open()` bool was producing zero-byte downloads with no diagnostic), add cross-platform `-DBOOST_BIND_GLOBAL_PLACEHOLDERS` (silences Boost 1.73+ global-placeholder deprecation), pin `-D_WIN32_WINNT=0x0A00` on Windows (was defaulting to Windows 7 / 0x0601 with a pragma message).
- `24c044f` — Sweep 5 sibling C4834 sites missed by the original census: `dockResultFile.C:586,594,602` (truncated docking-file output on open failure) + `downloadPDBFile.C:125` + `downloadElectronDensity.C:167` (zero-byte temp files → cryptic downstream parser failures). All applied with the same `Log.error()`-on-failure pattern.
- `d730518` — `GL_SILENCE_DEPRECATION` on macOS VIEW target: Apple deprecated all of OpenGL system-wide in 10.14 (2018); BALL's renderer stays on fixed-function GL until the v2.0 Metal/MoltenVK migration (ROADMAP Phase 999.6 PIPE-01). Apple's documented escape hatch. Eliminated ~1 500 deprecation warnings on macOS.
- `0a75ede` — C4267 narrowing mop-up: 3 sites where `size_t` was implicitly narrowing to `BALL::Index` (32-bit on Windows LLP64). Explicit `static_cast<Index>(container.size())` at `CIFFile.C:447`, `binaryFingerprintMethods.C:1618`, `conformationSet.C:46`.

**Coverage build now green for the first time** (Phase 999.27 — commits `78ecda5`, `2497369`, `b98b63a`): three defects that were causing the non-blocking `coverage (Linux, gcov, non-blocking)` job to fail on every push. Root cause was simpler than originally guessed — `BALL_VIEW_DEBUG=TRUE` is gated on `CMAKE_BUILD_TYPE STREQUAL "Debug"`, which the coverage job sets and the regular Linux job does not, so the offending unqualified `endl` (`networkPreferences.C:44`) only compiles under coverage. First green coverage run: 25968988067.

### Latent bug fixes summary

Independent of the warning count, several real bugs were flushed out by the cleanup process:

| File | Bug | Severity |
|---|---|---|
| `PDBFileGeneral.C:1602` | `va_start` UB on default-promoted enum parameter | **Real UB; latent since C++17 adoption** |
| `simpleDownloader.C:134` | `QFile::open()` failure produced zero-byte download with no diagnostic | Real latent silent failure |
| `dockResultFile.C:586,594,602` | Same pattern × 3 — truncated docking-file output on open failure | Real latent silent failure |
| `downloadPDBFile.C:125` | Same pattern → cryptic "no atoms found" parser failure | Real latent UX bug |
| `downloadElectronDensity.C:167` | Same pattern → DSN6 parser failure on zero-byte map file | Real latent UX bug |
| `hashGrid.C` neighbour table | ARM gcc unsigned-char default narrowed `-1` literals (build failure on ARM) | Build-blocking on ARM |

### Build infrastructure (Phase 999.16-19 carry-forward / completion)

- **Phase 999.16** PCH for BALL + VIEW (BUILD-ACCEL-01) — Linux warm-cache 15% faster; AppleClang exclusion (ccache + PCH non-deterministic on macOS).
- **Phase 999.17** Windows CMake build-tree cache (BUILD-ACCEL-02) — separate cache for Windows configure output.
- **Phase 999.18** doc/CLAUDE.md paths-ignore (BUILD-ACCEL-03 / -04) — already described above.
- **Phase 999.19** per-TU build profiling (BUILD-ACCEL-05) — `Top-20 slowest TUs` + ninja-log artifact.
- **Phase 999.20** GH Actions version bumps (BUILD-ACCEL-05) — upload-artifact v6, download-artifact v7.

---

## What's NOT in v1.6.2 (deferred)

- **Phase 999.6 (PIPE-01)** OpenGL → modern backend renderer rewrite — v2.0.
- **Phase 999.22** whole-tree Tier-C warning census execution — remains census-only; the systematic execution is staged for v1.7.
- **Phase 999.26** atom.h C4910 pragma suppression — independent backlog item, can land alongside v1.6.3 if it materializes.
- **GENETICDOCK self-deprecation chain** (`GenericGene`, `GeneticAlgorithm`, etc., ~120 `[[deprecated]]` markers) — architectural decision, not a 1.6.x patch.

---

## Upgrade notes

- **No breaking API changes.** All Qt 6 deprecation migrations preserve call semantics.
- **`PDBFile::writeRecord_` signature changed** from `(PDB::RecordType, ...)` → `(int, ...)`. Callers passing `PDB::RECORD_TYPE__*` enum constants are unaffected — implicit enum → int conversion. Direct callers with a `PDB::RecordType` *variable* will need an explicit cast.
- **Qt 6.8 LTS required.** macOS Homebrew users on Qt 6.5–6.7 should `brew upgrade qt`; Linux users on Ubuntu's apt-shipped Qt 6.4.2 must install Qt 6.8+ separately.

---

## Verification

Final pre-tag CI run: **25970862407** — all blocking jobs green (lint ✓, linux-arm64 ✓, linux-x64 ✓, macos-arm64 ✓, windows-x64 ✓, Phase 9 gatekeeper ✓, coverage ✓). Test suite passing.

Final commit on `v1.6-modernization`: `0a75ede`.

Tag: `v1.6.2` → GitHub Release artifacts (macOS `.dmg`, Linux `.AppImage`, Windows `.exe`) produced by `release.yml` on tag push.
