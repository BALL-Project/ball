---
phase: 08c-packaging-linux
plan: 01
type: execute
wave: 1
depends_on: []
files_modified:
  - cmake/BALLPackageConfig.cmake   # add DEB + RPM generator stanza, Linux-arm bits
  - cmake/BALLComponents.cmake      # confirm component → package mapping is Linux-clean
  - cmake/BALLConfigFFTW.cmake      # no-op read; dual-SKU is driven from outside (-DBALL_HAS_FFTW=OFF)
  - .github/workflows/release.yml   # add ubuntu-24.04 (x64) + ubuntu-24.04-arm matrix entries + AppImage/DEB/RPM steps
  - BUILD-linux.md                  # NEW — clean-VM build recipe for Ubuntu 24.04
  - packaging/linux/                # NEW dir: AppRun, .desktop file, icon symlink, COPYING.GPL
  - packaging/linux/COPYING.GPL     # NEW — full FFTW GPL text bundled in the with-FFTW SKU
  - packaging/linux/README-LICENSING.md # NEW — explains the two SKUs + which is which
autonomous: true
requirements:
  - PKG-LINUX-01     # AppImage produced for Linux x64 + arm64
  - PKG-LINUX-02     # .deb produced via CPack DEB generator
  - PKG-LINUX-03     # .rpm produced via CPack RPM generator
  - PKG-LINUX-04     # FFTW GPL compliance: dual SKU + attribution chain (LGPL-only DEFAULT, GPL with FFTW SECONDARY)
  - PKG-LINUX-05     # BUILD-linux.md authored and clean-VM validated
must_haves:
  truths:
    - "Two SKUs ship per Linux arch: BALLView-LGPL (no FFTW, default link target) and BALLView-GPL (with FFTW, secondary)"
    - "Named GPL-compliance owner recorded in 08c-FFTW-GPL-DECISION.md before any with-FFTW artifact is uploaded to a release"
    - "release.yml Linux jobs run on ubuntu-24.04 AND ubuntu-24.04-arm runners (no QEMU emulation for arm64)"
    - "AppImage built with linuxdeploy + linuxdeploy-plugin-qt (the canonical Qt 6 path); NO appimagetool-only fallback (loses Qt plugins)"
    - "DEB + RPM produced via CPack generators (NOT fpm) — leverages existing BALLPackageConfig.cmake scaffolding; fpm is documented fallback only"
    - "BUILD-linux.md commands copy-pasteable on a clean Ubuntu 24.04 cloud image with no warm caches; smoke-validated end-to-end at least once"
    - "No system-package signing (Linux convention) — AppImage zsync optional and DEFERRED to a 999.x stub"
    - "Existing macOS + Windows release jobs UNTOUCHED — Linux is purely additive"
  artifacts:
    - path: ".planning/phases/08c-packaging-linux/08c-FFTW-GPL-DECISION.md"
      provides: "Named GPL-compliance owner + decision record (ship-both | ship-LGPL-only). Created in Task 4."
      contains: "GPL-compliance owner:"
    - path: ".github/workflows/release.yml"
      provides: "Linux x64 + arm64 matrix entries producing .AppImage + .deb + .rpm per SKU (LGPL + GPL)"
      contains: "ubuntu-24.04-arm"
    - path: "BUILD-linux.md"
      provides: "Clean-VM build recipe; smoke-validated"
      contains: "apt-get install"
    - path: "packaging/linux/COPYING.GPL"
      provides: "Bundled GPL text for the with-FFTW SKU"
      contains: "GNU GENERAL PUBLIC LICENSE"
  key_links:
    - from: "cmake/BALLConfigFFTW.cmake (BALL_HAS_FFTW path)"
      to: "release.yml dual-SKU matrix (-DBALL_HAS_FFTW=OFF for LGPL, =ON for GPL)"
      via: "CMake cache var flipped per matrix entry; no source-side change needed"
      pattern: "BALL_HAS_FFTW"
    - from: ".planning/phases/08-packaging-and-distribution/08-INSTALLER-FORMATS-RESEARCH.md"
      to: "Phase 8c (this plan) — Linux installer-format decisions (AppImage + .deb + .rpm)"
      via: "Research doc is macOS+Windows only; Linux decisions are LOCKED in this plan's interfaces block"
      pattern: "AppImage|linuxdeploy"
---

<objective>
Ship Linux installers for BALLView v1.7: **AppImage** (portable, the canonical "double-click to run" Linux artifact), **.deb** (Debian / Ubuntu), and **.rpm** (Fedora / RHEL / openSUSE) — for both **x64** and **arm64** architectures, in two SKUs that resolve the FFTW GPL contagion question:

- **BALLView-LGPL** (default download): no FFTW, ships under BALL's native LGPL terms.
- **BALLView-GPL** (secondary download): with FFTW bundled, ships under GPL with full attribution + `COPYING.GPL` adjacent.

Adds Linux to release.yml (currently macOS + Windows only), authors `BUILD-linux.md` (parity with `BUILD-macos.md`), smoke-validates on a clean Ubuntu 24.04 VM, and records a **named GPL-compliance owner** before any GPL-SKU artifact is uploaded — closing the P0 risk register entry from v1.7-PLAN.md.

Output: 6 atomic tasks producing 6 artifacts per release (2 archs × 3 formats) × 2 SKUs = **12 release-asset slots** + 1 decision record + 1 build doc.
</objective>

<execution_context>
@$HOME/.claude/get-shit-done/workflows/execute-plan.md
@$HOME/.claude/get-shit-done/templates/summary.md
</execution_context>

<context>
@.planning/PROJECT.md
@.planning/ROADMAP.md
@.planning/v1.7-PLAN.md
@.planning/phases/08-packaging-and-distribution/08-INSTALLER-FORMATS-RESEARCH.md
@cmake/BALLConfiguration.cmake
@cmake/BALLConfigFFTW.cmake
@cmake/BALLPackageConfig.cmake
@cmake/BALLComponents.cmake
@.github/workflows/release.yml
@BUILD-macos.md

<interfaces>
**Linux installer-format decisions (LOCKED 2026-05-17 in this plan; not re-litigated in tasks):**

1. **AppImage tool: `linuxdeploy` + `linuxdeploy-plugin-qt`** — the canonical Qt 6 path. Alternatives rejected: raw `appimagetool` (loses Qt plugins → broken QML/svg/imageformat support); `cqtdeployer` (less mature, less Qt-team-aligned). linuxdeploy is single-binary, no build step, fetched per-arch from its GitHub releases.
2. **DEB + RPM via CPack generators** (`CPACK_GENERATOR=DEB;RPM`) — leverages existing `cmake/BALLPackageConfig.cmake` scaffolding. **fpm is documented fallback only** in BUILD-linux.md, NOT wired into CI. CPack DEB needs `dpkg-dev`; CPack RPM needs `rpm-build` (Ubuntu has both via apt).
3. **No signing for system packages.** .deb signing requires a project-level `gpg` keyring + Debian-style infrastructure (`debsign`, `dput`, an APT repo); .rpm signing similarly needs an RPM keyring + a YUM/DNF repo. Out of scope for v1.7 — users `apt install ./BALLView_*.deb` / `dnf install ./BALLView-*.rpm` directly from the GitHub Release page; verification is GitHub-release-page-as-trust-anchor (same as macOS/Windows .zip today). AppImage zsync (delta updates) deferred to 999.x.
4. **Architectures: x64 (`ubuntu-24.04`) + arm64 (`ubuntu-24.04-arm`)** — both first-class GitHub runners as of late 2025; no QEMU emulation. arm64 covers the rising-share Raspberry Pi / Ampere / Graviton-based dev workstation population.
5. **Distro target: Ubuntu 24.04 LTS as the canonical build base.** Produces .deb that installs cleanly on Ubuntu 24.04 + Debian 12 (glibc 2.39 baseline); .rpm tested on Fedora 40+ (glibc 2.39+). AppImage is glibc-version-portable down to whatever Ubuntu 22.04's glibc 2.35 supports given linuxdeploy's `--appdir` packaging.

**FFTW GPL dual-SKU mechanics:**

`cmake/BALLConfigFFTW.cmake:1` sets `BALL_HAS_FFTW TRUE` unconditionally when `FFTW*_FOUND` resolves. The off-switch is `-DBALL_HAS_FFTW=OFF` at the top-level CMake configure invocation — this short-circuits FFTW detection. Already-supported build path (the no-FFTW SKU = the LGPL SKU; just don't `apt install libfftw3-dev`).

Two `cpack` invocations per Linux job, each producing 3 generator outputs (DEB + RPM + AppImage-via-script):
- **Invocation 1 (LGPL SKU):** configure with FFTW absent (`apt`'s `libfftw3-dev` NOT installed; `-DBALL_HAS_FFTW=OFF` belt-and-braces); build + package. Output filenames suffixed `-lgpl`. Files: `BALLView-lgpl-1.7.0-linux-x86_64.AppImage`, `BALLView-lgpl_1.7.0_amd64.deb`, `BALLView-lgpl-1.7.0-1.x86_64.rpm`.
- **Invocation 2 (GPL SKU):** configure with FFTW present (`apt install libfftw3-dev libfftw3-3`; FFTW auto-detected); build + package. Output filenames suffixed `-gpl`. **`packaging/linux/COPYING.GPL` MUST be packaged adjacent to the binary in all 3 formats** (CPack supports this via `install(FILES ... DESTINATION share/doc/BALLView)`).
- **Two builds, two CMake binary dirs** (`build/ci-linux-lgpl` + `build/ci-linux-gpl`) — DO NOT share; FFTW detection caches in `CMakeCache.txt` and a single-dir flip leaves stale objects.

**linuxdeploy invocation shape** (target for Task 1, AppImage):
```bash
# Fetched per-arch (linuxdeploy-x86_64.AppImage / linuxdeploy-aarch64.AppImage)
linuxdeploy --appdir AppDir \
  --executable build/ci-linux-${SKU}/bin/BALLView \
  --desktop-file packaging/linux/BALLView.desktop \
  --icon-file data/icons/BALLView.png \
  --plugin qt \
  --output appimage
# Plugin auto-discovers Qt 6 install via qmake6/Qt6_DIR; bundles QML, plugins/, translations/
mv BALLView-*.AppImage BALLView-${SKU}-${VERSION}-linux-${ARCH}.AppImage
```
`linuxdeploy-plugin-qt` is the official Qt-plugin sidecar — fetched from the same `linuxdeploy/linuxdeploy-plugin-qt` GitHub release.

**CPack DEB stanza** (target for Task 2, to add to `BALLPackageConfig.cmake`):
```cmake
IF(UNIX AND NOT APPLE)
  LIST(APPEND CPACK_GENERATOR "DEB" "RPM")
  SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "BALL Project <admin@ball-project.org>")
  SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)   # auto-resolves Qt6, Boost, etc. via dpkg-shlibdeps
  SET(CPACK_DEBIAN_PACKAGE_SECTION "science")
  SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
  SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://ball-project.org")
  IF(BALL_HAS_FFTW)
    SET(CPACK_PACKAGE_FILE_NAME "BALLView-gpl-${PROJECT_VERSION}-linux-${CMAKE_SYSTEM_PROCESSOR}")
  ELSE()
    SET(CPACK_PACKAGE_FILE_NAME "BALLView-lgpl-${PROJECT_VERSION}-linux-${CMAKE_SYSTEM_PROCESSOR}")
  ENDIF()
ENDIF()
```

**CPack RPM stanza** (target for Task 3):
```cmake
SET(CPACK_RPM_PACKAGE_LICENSE  $<IF:$<BOOL:${BALL_HAS_FFTW}>,GPL-3.0-or-later,LGPL-2.1-or-later>)
SET(CPACK_RPM_PACKAGE_GROUP    "Applications/Engineering")
SET(CPACK_RPM_PACKAGE_URL      "https://ball-project.org")
SET(CPACK_RPM_PACKAGE_AUTOREQ  ON)   # auto-resolve runtime deps
SET(CPACK_RPM_PACKAGE_RELOCATABLE FALSE)
```

**Risk acknowledgement:** if no GPL-savvy owner can be named in Task 4, the GPL SKU is **NOT BUILT** (matrix entry skipped); LGPL SKU still ships. Task 4 records this disposition in `08c-FFTW-GPL-DECISION.md`. This is the v1.7-PLAN P0 risk-register mitigation made concrete.
</interfaces>
</context>

<tasks>

<task type="auto">
  <name>Task 1: AppImage build wiring — linuxdeploy + linuxdeploy-plugin-qt, both archs</name>
  <files>
    .github/workflows/release.yml
    packaging/linux/BALLView.desktop
    packaging/linux/AppRun  (optional; linuxdeploy can synth)
  </files>
  <read_first>
    - .github/workflows/release.yml (the existing macOS macdeployqt block at lines ~154-160 as a structural template)
    - cmake/BALLConfigFFTW.cmake (confirm the FFTW off-path is honoured by `-DBALL_HAS_FFTW=OFF`)
  </read_first>
  <action>
    1. Create `packaging/linux/BALLView.desktop` (FreeDesktop .desktop file: `Name=BALLView`, `Exec=BALLView`, `Icon=BALLView`, `Categories=Science;Chemistry;Education;`, `Type=Application`).
    2. Add to `release.yml` package matrix:
       ```yaml
       - os: ubuntu-24.04
         arch: x86_64
       - os: ubuntu-24.04-arm
         arch: aarch64
       ```
       Place AFTER the existing `macos-arm64` + `windows-x64` entries (additive only — UNTOUCHED rule).
    3. Add a Linux job block parallel to the macOS one. Steps:
       - `apt install` deps: `qt6-base-dev qt6-svg-dev qt6-tools-dev libboost-all-dev libeigen3-dev libglew-dev libtbb-dev libopenbabel-dev cmake ninja-build flex bison dpkg-dev rpm`.
       - Fetch linuxdeploy + linuxdeploy-plugin-qt for the matrix arch (`linuxdeploy-x86_64.AppImage` or `linuxdeploy-aarch64.AppImage`); `chmod +x`.
       - Configure + build into `build/ci-linux-lgpl` with `-DBALL_HAS_FFTW=OFF` (NOTE: do NOT install libfftw3-dev for the LGPL SKU; Task 4 covers the GPL second-build).
       - Run linuxdeploy per the interfaces invocation shape.
       - Output: `BALLView-lgpl-${VERSION}-linux-${ARCH}.AppImage`.
    4. Upload as workflow artifact (mirror existing macOS upload step).
    5. Validate workflow YAML: `actionlint .github/workflows/release.yml` OR `python3 -c "import yaml; yaml.safe_load(open('.github/workflows/release.yml'))"`.
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; python3 -c "import yaml; yaml.safe_load(open('.github/workflows/release.yml'))" &amp;&amp; grep -c "ubuntu-24.04-arm" .github/workflows/release.yml &amp;&amp; test -f packaging/linux/BALLView.desktop</automated>
  </verify>
  <acceptance_criteria>
    - release.yml has 4 new matrix entries: ubuntu-24.04 + ubuntu-24.04-arm × LGPL (GPL SKU added in Task 4).
    - AppImage produced for both archs on push of a v1.7-rc tag; downloads + chmod +x + runs `--appimage-version`.
    - linuxdeploy-plugin-qt bundles Qt 6 plugins (verify by unpacking AppImage: `plugins/imageformats/`, `plugins/platforms/libqxcb.so` present).
    - .desktop file Lint-clean (`desktop-file-validate packaging/linux/BALLView.desktop`).
  </acceptance_criteria>
  <done>
    AppImage shipped for Linux x64 + arm64, LGPL SKU.
  </done>
</task>

<task type="auto">
  <name>Task 2: CPack DEB generator wiring + Ubuntu 24.04 install smoke-test</name>
  <files>
    cmake/BALLPackageConfig.cmake
    .github/workflows/release.yml
  </files>
  <read_first>
    - cmake/BALLPackageConfig.cmake (the existing `IF(APPLE)` / `ELSEIF(WIN32)` scaffolding to extend with `ELSEIF(UNIX)`)
    - cmake/BALLComponents.cmake (confirm component layout works for DEB single-package output, OR split components if multi-package needed)
  </read_first>
  <action>
    1. Add the `IF(UNIX AND NOT APPLE)` DEB stanza per interfaces block to `BALLPackageConfig.cmake`.
    2. In release.yml Linux job, after the build step, add `cd build/ci-linux-lgpl && cpack -G DEB`.
    3. Output: `BALLView-lgpl_${VERSION}_${ARCH}.deb` (DEB naming convention: underscore-separated, lowercase arch — CPack handles).
    4. Smoke-test step in CI: `dpkg -i BALLView-lgpl_*.deb || true; apt-get install -f -y; which BALLView` — confirms install succeeds, dependencies resolve via shlibdeps, binary lands on PATH.
    5. Upload .deb as workflow artifact.
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; grep -c "CPACK_DEBIAN_PACKAGE_MAINTAINER" cmake/BALLPackageConfig.cmake &amp;&amp; grep -c "DEB" cmake/BALLPackageConfig.cmake</automated>
  </verify>
  <acceptance_criteria>
    - CPack DEB stanza in BALLPackageConfig.cmake under `IF(UNIX AND NOT APPLE)`.
    - `cpack -G DEB` in CI produces `BALLView-lgpl_*.deb` for both archs.
    - `dpkg -i` + `apt-get install -f` smoke-test passes (binary on PATH; no missing-dep failures).
    - `dpkg-deb -I BALLView-lgpl_*.deb | grep Depends` shows Qt6, Boost, libglew, etc. auto-resolved by `CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON`.
  </acceptance_criteria>
  <done>
    .deb shipped for Linux x64 + arm64, LGPL SKU, install-tested in CI.
  </done>
</task>

<task type="auto">
  <name>Task 3: CPack RPM generator wiring + cross-distro rpm-build smoke-test</name>
  <files>
    cmake/BALLPackageConfig.cmake
    .github/workflows/release.yml
  </files>
  <read_first>
    - cmake/BALLPackageConfig.cmake (after Task 2 lands — add RPM stanza adjacent to DEB)
  </read_first>
  <action>
    1. Add CPack RPM stanza per interfaces block to `BALLPackageConfig.cmake`.
    2. In release.yml Linux job, after `cpack -G DEB`, add `cpack -G RPM` (single CPack invocation can produce both; this is just for staging clarity — `cpack -G "DEB;RPM"` is the production form).
    3. Smoke-test: `rpm -qpi BALLView-lgpl-*.rpm` (read package metadata) + `rpm -qpR BALLView-lgpl-*.rpm` (list runtime requires) — confirms RPM is well-formed and `CPACK_RPM_PACKAGE_AUTOREQ ON` produced sensible deps. **Do NOT `rpm -i`** on Ubuntu runners — install-test happens on Fedora-side CI which is OUT OF SCOPE for v1.7 (filed as backlog stub if cross-distro install-test demand surfaces).
    4. Upload .rpm as workflow artifact.
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; grep -c "CPACK_RPM_PACKAGE_LICENSE" cmake/BALLPackageConfig.cmake &amp;&amp; grep -c '"RPM"' cmake/BALLPackageConfig.cmake</automated>
  </verify>
  <acceptance_criteria>
    - CPack RPM stanza in BALLPackageConfig.cmake; `CPACK_RPM_PACKAGE_LICENSE` generator-expression-flips between LGPL-2.1-or-later (no-FFTW) and GPL-3.0-or-later (with-FFTW).
    - `cpack -G RPM` in CI produces `BALLView-lgpl-*.rpm` for both archs.
    - `rpm -qpi` reads metadata; `rpm -qpR` lists requires (Qt6, Boost, libglew, etc.).
    - Cross-distro `rpm -i` install-test deferred — filed as 999.x backlog stub if requested.
  </acceptance_criteria>
  <done>
    .rpm shipped for Linux x64 + arm64, LGPL SKU; metadata-validated in CI.
  </done>
</task>

<task type="auto">
  <name>Task 4: FFTW GPL dual-SKU build + named owner decision record</name>
  <files>
    .planning/phases/08c-packaging-linux/08c-FFTW-GPL-DECISION.md (NEW)
    packaging/linux/COPYING.GPL (NEW)
    packaging/linux/README-LICENSING.md (NEW)
    .github/workflows/release.yml
    cmake/BALLPackageConfig.cmake
  </files>
  <read_first>
    - cmake/BALLConfigFFTW.cmake (confirm `-DBALL_HAS_FFTW=OFF` is honoured before FFTW detection runs)
    - .planning/v1.7-PLAN.md risk register row "FFTW GPL compliance review needs a named owner"
  </read_first>
  <action>
    1. Author `08c-FFTW-GPL-DECISION.md` with:
       - **Named GPL-compliance owner** (proposed: Oliver Kohlbacher per project lead position; record acceptance / refusal explicitly).
       - Decision: SHIP-BOTH-SKUs or SHIP-LGPL-ONLY (default to SHIP-LGPL-ONLY if owner unnamed by Wave 1 close, per v1.7-PLAN risk-register mitigation).
       - Attribution chain: which files must accompany GPL SKU (`COPYING.GPL`, `README-LICENSING.md` explaining the LGPL→GPL upgrade triggered by FFTW link).
       - FFTW upstream version + license URL (FFTW 3.3.x, GPL-2.0-or-later).
       - Cross-ref to v1.7-PLAN risk row.
    2. Drop full GPL-3.0 text into `packaging/linux/COPYING.GPL` (canonical source: `https://www.gnu.org/licenses/gpl-3.0.txt` OR GPL-2.0 to match FFTW's actual license — verify FFTW's `COPYING` in upstream; FFTW is GPL-2.0-or-later, so we ship under GPL-2.0-or-later for the with-FFTW SKU).
    3. Author `packaging/linux/README-LICENSING.md`:
       - "BALLView is LGPL-2.1-or-later. The LGPL SKU ships without FFTW."
       - "The GPL SKU links FFTW (GPL-2.0-or-later). Per GPL contagion, the resulting binary is distributed under GPL-2.0-or-later. See COPYING.GPL adjacent to this file."
       - "If you cannot accept GPL terms, use the LGPL SKU."
    4. **If owner is named (SHIP-BOTH path):** add 2 new matrix entries to release.yml (`ubuntu-24.04-gpl-x64`, `ubuntu-24.04-arm-gpl`) that:
       - Install `libfftw3-dev libfftw3-3` via apt.
       - Configure into `build/ci-linux-gpl` (separate binary dir from LGPL — DO NOT share).
       - Build + `cpack -G "DEB;RPM"` + linuxdeploy → AppImage.
       - Ensure `COPYING.GPL` + `README-LICENSING.md` are packaged adjacent (via `install(FILES packaging/linux/COPYING.GPL packaging/linux/README-LICENSING.md DESTINATION share/doc/BALLView)` in CMake, OR `--custom-apprun` for the AppImage).
       - Output filenames suffixed `-gpl` (CPack `IF(BALL_HAS_FFTW)` branch already covers DEB+RPM; AppImage rename in workflow step).
    5. **If owner is NOT named (SHIP-LGPL-ONLY path):** record the decision in `08c-FFTW-GPL-DECISION.md` with rationale; do NOT add GPL matrix entries; release.yml stays LGPL-only for Linux.
    6. Update `README.md` (root) with a one-sentence pointer to `packaging/linux/README-LICENSING.md` if SHIP-BOTH.
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; test -f .planning/phases/08c-packaging-linux/08c-FFTW-GPL-DECISION.md &amp;&amp; grep -c "GPL-compliance owner" .planning/phases/08c-packaging-linux/08c-FFTW-GPL-DECISION.md &amp;&amp; test -f packaging/linux/COPYING.GPL &amp;&amp; test -f packaging/linux/README-LICENSING.md</automated>
  </verify>
  <acceptance_criteria>
    - `08c-FFTW-GPL-DECISION.md` exists with a NAMED owner OR a documented LGPL-only fallback.
    - `COPYING.GPL` is the full GPL-2.0-or-later text (matches FFTW upstream license).
    - `README-LICENSING.md` explains the dual-SKU + contagion in 1 paragraph each, layman-readable.
    - If SHIP-BOTH: release.yml has 2 additional matrix entries; GPL SKU artifacts package COPYING.GPL adjacent.
    - If SHIP-LGPL-ONLY: GPL matrix entries NOT added; risk explicitly closed in decision doc.
    - v1.7-PLAN.md risk register entry for FFTW GPL is updated to point at this decision record (do this update in the SUMMARY).
  </acceptance_criteria>
  <done>
    FFTW GPL compliance resolved with a named-owner decision record; dual-SKU shipping iff owner exists.
  </done>
</task>

<task type="auto">
  <name>Task 5: BUILD-linux.md authoring + clean-VM smoke validation</name>
  <files>
    BUILD-linux.md (NEW)
  </files>
  <read_first>
    - BUILD-macos.md (the template — match structure: prereqs → Homebrew/apt commands → cmake configure → cmake build → run-from-build-tree section → packaging section pointing at this Phase)
  </read_first>
  <action>
    1. Author `BUILD-linux.md` with sections:
       - **Prereqs** — Ubuntu 24.04 LTS (canonical), Debian 12, or Fedora 40+. apt-get OR dnf install lines for: build-essential cmake ninja-build flex bison qt6-base-dev qt6-svg-dev qt6-tools-dev libboost-all-dev libeigen3-dev libglew-dev libtbb-dev libopenbabel-dev (Ubuntu); equivalent dnf lines for Fedora.
       - **Optional FFTW (GPL upgrade)** — `apt install libfftw3-dev libfftw3-3` (note: this triggers GPL relicensing of the resulting binary; see `packaging/linux/README-LICENSING.md`).
       - **Configure + build** — `cmake -B build -S . -G Ninja -DCMAKE_BUILD_TYPE=Release` then `cmake --build build -j`.
       - **Run from build tree** — `export BALL_DATA_PATH=$(pwd)/data && ./build/bin/BALLView`.
       - **Packaging** — pointer to this Phase 8c artifacts (AppImage, .deb, .rpm) + commands: `cd build && cpack -G "DEB;RPM"`; linuxdeploy invocation for AppImage; `fpm` fallback paragraph for when CPack DEB/RPM aren't an option.
       - **Troubleshooting** — Qt 6 plugin path env (`QT_PLUGIN_PATH`), the `BALL_DATA_PATH` requirement, `libGLU` / mesa-utils on headless servers.
    2. Smoke-validate the doc:
       - Spin up a clean Ubuntu 24.04 environment. Options (lowest friction → highest):
         a. **GitHub Codespaces** Ubuntu 24.04 image (free tier; ~5min provision).
         b. **Multipass** locally (`multipass launch 24.04 --name ball-smoke --memory 8G --cpus 4`).
         c. **Docker** `ubuntu:24.04` image (limited — no GUI/GL, but compile + ctest works).
         d. CI-side: a one-off `ubuntu-24.04` workflow_dispatch run that copy-pastes the BUILD-linux.md commands verbatim into a single shell step and asserts exit 0.
       - **Preferred: option (d)** — captures the smoke-test in CI history; no out-of-band steps.
    3. Record the smoke-test run URL + timestamp in `BUILD-linux.md` footer ("Last validated on clean Ubuntu 24.04: <URL>, <date>").
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; test -f BUILD-linux.md &amp;&amp; grep -c "apt-get install\|apt install" BUILD-linux.md &amp;&amp; grep -c "BALL_DATA_PATH" BUILD-linux.md &amp;&amp; grep -c "Last validated" BUILD-linux.md</automated>
  </verify>
  <acceptance_criteria>
    - BUILD-linux.md exists, mirrors BUILD-macos.md structure.
    - apt-get install commands copy-paste cleanly on Ubuntu 24.04 (verified via Task 5 step 2 smoke run).
    - BALL_DATA_PATH export pattern explicit (matches macOS doc).
    - Footer cites a CI smoke-test URL + date.
    - Packaging section points at this Phase 8c (AppImage + .deb + .rpm) without re-deriving the recipes.
  </acceptance_criteria>
  <done>
    BUILD-linux.md authored, smoke-validated, and parity with BUILD-macos.md.
  </done>
</task>

<task type="auto">
  <name>Task 6: Full release.yml Linux matrix run — ubuntu-24.04 + ubuntu-24.04-arm × {LGPL, GPL (if owner named)} green</name>
  <files>(none — full-pipeline verification)</files>
  <read_first>
    - .planning/phases/08c-packaging-linux/08c-FFTW-GPL-DECISION.md (Task 4 output — determines matrix size: 2 or 4 entries)
  </read_first>
  <action>
    1. Push Tasks 1-5 commits as a single PR branch.
    2. Trigger release.yml via `workflow_dispatch` with a test tag (e.g. `v1.7.0-test-08c`).
    3. `gh run watch <id>`; wait to completion.
    4. Verify per matrix-entry artifacts uploaded:
       - `BALLView-lgpl-1.7.0-test-08c-linux-x86_64.AppImage` (size 40-100MB expected with Qt 6 bundle).
       - `BALLView-lgpl_1.7.0-test-08c_amd64.deb`.
       - `BALLView-lgpl-1.7.0-test-08c-1.x86_64.rpm`.
       - Same triplet for `linux-aarch64` arch.
       - If SHIP-BOTH: same triplet × 2 archs for `-gpl` suffix; COPYING.GPL must be present inside DEB (`dpkg-deb -c BALLView-gpl_*.deb | grep COPYING.GPL`), inside RPM (`rpm -qpl BALLView-gpl-*.rpm | grep COPYING.GPL`), inside AppImage (`./BALLView-gpl-*.AppImage --appimage-extract; ls squashfs-root/usr/share/doc/BALLView/COPYING.GPL`).
    5. macOS + Windows existing jobs MUST stay green (untouched-rule check).
    6. Record run URL + per-artifact sizes in SUMMARY.md.
  </action>
  <verify>
    <automated>echo "MANUAL — see SUMMARY.md for release.yml matrix run URL + per-artifact validation"</automated>
  </verify>
  <acceptance_criteria>
    - release.yml matrix run completes green on all Linux entries (2 or 4 depending on SHIP-BOTH).
    - Per-arch artifact triplet (AppImage + DEB + RPM) uploaded for LGPL SKU.
    - If SHIP-BOTH: GPL-SKU triplet uploaded with COPYING.GPL inside each.
    - macOS + Windows jobs unchanged + green (additive-only check).
    - AppImage runs `--appimage-version` on ubuntu-24.04 runner without segfault.
  </acceptance_criteria>
  <done>
    Linux release pipeline green end-to-end across arch × SKU matrix; Phase 8c PKG-LINUX-01..05 satisfied.
  </done>
</task>

</tasks>

<verification>
- release.yml has ubuntu-24.04 + ubuntu-24.04-arm matrix entries producing AppImage + .deb + .rpm per SKU.
- cmake/BALLPackageConfig.cmake has DEB + RPM CPack generators wired under `IF(UNIX AND NOT APPLE)`.
- `08c-FFTW-GPL-DECISION.md` records a named GPL-compliance owner (or LGPL-only fallback rationale).
- BUILD-linux.md exists, parity-validated against BUILD-macos.md, smoke-tested on clean Ubuntu 24.04.
- `packaging/linux/COPYING.GPL` + `packaging/linux/README-LICENSING.md` exist; both packaged adjacent to GPL-SKU binary in DEB / RPM / AppImage.
- macOS + Windows release jobs untouched + still green.
</verification>

<success_criteria>
- AppImage + .deb + .rpm produced for Linux x64 + arm64 in the LGPL SKU.
- GPL SKU shipped iff a named GPL-compliance owner is recorded (P0 v1.7-PLAN risk closed either way).
- BUILD-linux.md merged and smoke-validated.
- Linux matrix run green; existing macOS + Windows jobs unaffected.
- v1.7-PLAN.md risk-register entry for "FFTW GPL compliance review needs a named owner" updated to cite 08c-FFTW-GPL-DECISION.md.
</success_criteria>

<output>
Create `.planning/phases/08c-packaging-linux/08c-01-SUMMARY.md` per template. Include:
- Per-artifact size table (LGPL × {x64, arm64} × {AppImage, DEB, RPM}; same for GPL if SHIP-BOTH).
- FFTW GPL decision summary (owner name OR LGPL-only fallback rationale).
- BUILD-linux.md smoke-test CI run URL.
- release.yml diff block (matrix entries added + Linux job block).
- CPack DEB + RPM stanza diff block.
- Risk-register update pointer for v1.7-PLAN.md.
</output>
