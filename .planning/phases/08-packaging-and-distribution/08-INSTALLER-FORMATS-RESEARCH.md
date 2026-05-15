---
phase: 08-packaging-and-distribution
type: research
status: dormant (research only; Phase 8 not yet active)
researched: 2026-05-15
parent_phase: 08
---

# Phase 8: Installer Formats — macOS DMG + Windows Installer Research

## Context

v1.6.0 ships **zips** on both platforms (`release.yml`, commit `afa00c2`):
- macOS: `ditto -c -k --sequesterRsrc --keepParent BALLView.app …` → `.zip`
- Windows: `Compress-Archive` of the windeployqt'd directory → `.zip`

Zips work but aren't what users expect from a polished release:
- **macOS users expect a `.dmg`** with a drag-to-Applications hint (the conventional macOS install ritual)
- **Windows users expect a `.exe` installer or `.msi`** that registers in Add/Remove Programs, places shortcuts, and handles uninstall cleanly

This document captures the **research (2026-05-15)** on what tools to use for each, with recommendations for Phase 8 to pick from. Sibling to [`08-SIGNING-RESEARCH.md`](08-SIGNING-RESEARCH.md) — the two are coupled because signing is applied to the installer artifact, not the contents alone.

## BALL's existing CPack foundation

BALL has historical CPack config already in place:
- [`cmake/BALLPackageConfig.cmake`](../../../cmake/BALLPackageConfig.cmake) — sets `CPACK_PACKAGE_NAME`, version, vendor (`ball-project.org`), description, welcome RTF, license file. References `CPACK_NSIS_INSTALL_ROOT` for Windows.
- [`cmake/BALLComponents.cmake`](../../../cmake/BALLComponents.cmake) — declares CPack components (BALL libs, VIEW, BALLView, data, headers) with display names, descriptions, dependencies.
- `CPACK_PACKAGE_FILE_NAME` switches between MSVC 2010 / 2012 / 2013 / 2015 (historical artifact — needs an MSVC 2022 / vcpkg update for v1.7).

So **CPack with NSIS is partially wired**. Phase 8 picks up an existing scaffold rather than starting from zero.

## macOS — DMG creation options

### Option A — `create-dmg` (sindresorhus) — **RECOMMENDED**

[github.com/sindresorhus/create-dmg](https://github.com/sindresorhus/create-dmg). Maintained, MIT-licensed.

```bash
# In the workflow, AFTER macdeployqt + ad-hoc resign (or Developer ID resign in Phase 8):
brew install create-dmg
create-dmg \
  --volname "BALLView ${VERSION}" \
  --volicon "data/icons/BALLView.icns" \
  --background "packaging/macos/dmg-background.png" \
  --window-pos 200 120 \
  --window-size 600 400 \
  --icon "BALLView.app" 175 190 \
  --hide-extension "BALLView.app" \
  --app-drop-link 425 190 \
  --codesign "Developer ID Application: <Org> (<TEAMID>)" \
  --notarize "<keychain-profile-stored-credentials>" \
  "BALLView-${VERSION}.dmg" \
  "build/ci-macos/bin/"
```

**Pros:**
- Single command, sensible defaults
- Drag-to-Applications shortcut included automatically (`--app-drop-link`)
- Custom volume background image (BALL branding opportunity)
- Code-signs the DMG itself (in addition to the .app inside, which is signed separately)
- Notarizes the DMG (`--notarize` flag uses keychain-stored credentials — interop with Phase 8 signing)
- `--no-code-sign` flag for CI builds where the cert isn't available (e.g. PR runs)
- Available via `brew install create-dmg` — no compile, no Python deps

**Cons:**
- Requires Homebrew on the runner (already true for our macOS CI)
- The volume background image is an asset to design and commit (~1 hour of design work)

### Option B — `hdiutil` (built-in)

macOS's native command, no external tool. Simpler but produces a plain DMG with no Applications shortcut, no custom background, no icon positioning.

```bash
hdiutil create -volname "BALLView ${VERSION}" \
               -srcfolder BALLView.app \
               -ov -format UDZO \
               "BALLView-${VERSION}.dmg"
```

**Pros:** zero deps, smallest installer surface.
**Cons:** looks like a 2008 DMG. No drag-to-Applications. Users get confused.

### Option C — `dmgbuild` (Python)

[dmgbuild on PyPI](https://pypi.org/project/dmgbuild/). Programmable via Python config files. More flexible than `create-dmg`, more complex. Used by some Python-app-packaging workflows.

**Pros:** Python-config flexibility, can do things create-dmg can't (e.g. custom URL handlers, multi-image DMGs).
**Cons:** Python on the runner, more setup, overkill for BALL's needs.

### Recommendation: **Option A — `create-dmg`**

Path of least resistance + best UX. Single brew install, integrates with Phase 8 signing flow via `--codesign` and `--notarize` flags. Add a `packaging/macos/dmg-background.png` asset to the repo (1024×640 PNG with arrow showing "drag here →") and the workflow step becomes ~10 lines.

## Windows — Installer options

### Option A — NSIS (Nullsoft Scriptable Install System) — **RECOMMENDED for Phase 8 minimum**

Free, open-source, CPack-native (`CPACK_GENERATOR=NSIS`). Current version: NSIS 3.11 (March 2025). BALL's CPack config already has `CPACK_NSIS_INSTALL_ROOT` referenced — historical scaffolding to revive.

**Pros:**
- **CPack-native** — `cpack -G NSIS` Just Works once `CPACK_NSIS_*` variables are set
- BALL's `cmake/BALLPackageConfig.cmake` and `BALLComponents.cmake` already partially configured for NSIS
- Small installer footprint (~typical: tens of KB for the installer shell + your bundle)
- Modern UI plugin available (NSIS Modern UI 2 ships with NSIS)
- Available on Windows runners via `choco install nsis` (~30 sec to install)

**Cons:**
- Not Windows-native installer format — produces a self-extracting `.exe`, not `.msi`. Some enterprise IT departments require MSI for Group Policy deployment.
- The `.nsh` scripting language is its own thing (legacy, but well-documented).

### Option B — WiX Toolset (MSI) — **future upgrade for enterprise**

Microsoft's MSI generator. Produces native `.msi` files. CPack supports it via `CPACK_GENERATOR=WIX` (CMake 2.8.11+).

**Pros:**
- Produces native `.msi` — Windows-default installer format. Enterprise IT departments deploy MSI files via Group Policy without friction.
- Native uninstaller in Windows Settings (NSIS also handles this but via a different mechanism)
- Modern UI possible (WixToolset's BootstrapperApplication framework, but complex)
- Better fit for institutional / university distribution where IT staff manage installs

**Cons:**
- Steeper learning curve than NSIS — XML-based, more verbose, more concepts (Features, Components, MergeModules, …)
- Heavier installer (a few MB shell overhead vs NSIS's KBs)
- CPack's WIX generator is less mature than NSIS — more `CPACK_WIX_*` variables to set, more gotchas

### Option C — Inno Setup — middle ground

Free, between NSIS and WiX in complexity. Pascal-script-based. **Not CPack-native** — would require a separate `.iss` script in the repo + an Inno Setup build step.

**Pros:** Easier syntax than NSIS, modern UI, popular in the indie / SaaS space.
**Cons:** Not CPack-native — abandons BALL's existing CPack scaffolding. Pascal scripts are an additional language to maintain.

### Option D — MSIX — modern Microsoft Store format

Sandboxed, modern, signed-by-default. Required for Microsoft Store distribution.

**Pros:** Modern security model, auto-update infrastructure, Microsoft Store eligible.
**Cons:** Overkill for a desktop molecular tool. Requires MSIX SDK + signing identity. Microsoft Store distribution adds review process, age ratings, etc. — not where BALL wants to go for v1.x.

### Recommendation: **NSIS for Phase 8 v1.x, WiX as a v2 / institutional upgrade**

NSIS leverages BALL's existing CPack scaffolding (delete the MSVC 2010-2015 hardcoding, modernize the variables, add `choco install nsis` to the workflow). Produces a working `.exe` installer in ~20 lines of CMake updates + ~50 lines of workflow steps. Enterprise customers requesting MSI can be a Phase 8.1 / v2 upgrade.

## Integration with Phase 8 signing

Both installer formats need to be **signed**, in addition to the .app/.exe they contain:

- **macOS DMG**: sign with `codesign --sign "Developer ID Application: ..."` after creation, or use `create-dmg --codesign` to do it inline. Then notarize the DMG itself (`xcrun notarytool submit BALLView-${VERSION}.dmg --wait`) and staple (`xcrun stapler staple`).
- **Windows installer**: sign the installer `.exe` with `signtool` (NSIS) or via WiX's signing hooks (WiX). The SignPath Foundation flow ([signing research](08-SIGNING-RESEARCH.md) Path A) handles installer signing via the same `signpath/github-action-submit-signing-request` action — just pass the installer path as the artifact.

The signing flow is:
1. Build `.app` / `.exe` and dependencies (existing release.yml machinery)
2. Sign the inner binaries (codesign on macOS, signtool on Windows)
3. Build the installer (`create-dmg` / `cpack -G NSIS`)
4. Sign the installer itself
5. Notarize (macOS only)
6. Staple (macOS only)
7. Upload to GitHub Release

Steps 2 and 4 use the same signing identities — just different inputs. Coupling-wise: implementing Phase 8 signing first (since it's the harder problem) and dropping installers in afterward is the natural sequencing.

## CMake / CPack integration sketch

Phase 8 work (NSIS path) would look roughly like:

```cmake
# In BALLPackageConfig.cmake — modernize:
SET(CPACK_PACKAGE_FILE_NAME "BALLView-${PROJECT_VERSION}-${BALL_PLATFORM_TAG}")
SET(CPACK_GENERATOR "")          # default empty; per-platform appended below

IF(APPLE)
  # macOS: use create-dmg outside CPack (better DMG quality than CPack's
  # built-in DragNDrop generator). CPack here only assembles the .app
  # contents via the install() rules; create-dmg wraps the result.
  SET(CPACK_GENERATOR "TGZ")     # fallback for `cpack -G` invocation
ELSEIF(WIN32)
  SET(CPACK_GENERATOR "NSIS")
  SET(CPACK_NSIS_DISPLAY_NAME "BALLView ${PROJECT_VERSION}")
  SET(CPACK_NSIS_PACKAGE_NAME "BALLView ${PROJECT_VERSION}")
  SET(CPACK_NSIS_HELP_LINK "https://ball-project.org")
  SET(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/BALL-Project/ball")
  SET(CPACK_NSIS_CONTACT "https://github.com/BALL-Project/ball/issues")
  # NOTE: CPACK_NSIS_MODIFY_PATH is intentionally NOT set — per-user
  # install (Decision #3, 2026-05-15). Modifying system PATH would
  # require admin elevation; we want the installer to work for
  # academic / non-IT-admin users without UAC prompts.
  SET(CPACK_NSIS_INSTALL_ROOT "$LOCALAPPDATA\\Programs")  # per-user default
  SET(CPACK_NSIS_MENU_LINKS
    "bin/BALLView.exe" "BALLView"
    "https://ball-project.org" "BALL Project Homepage")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin/BALLView.exe")
  # NSIS installer chrome stays English-only (Decision #4, 2026-05-15).
ENDIF()

INCLUDE(CPack)
```

CI workflow step:
```yaml
- name: Install NSIS (Windows)
  if: matrix.os == 'windows-x64'
  run: choco install nsis -y

- name: Build NSIS installer (Windows)
  if: matrix.os == 'windows-x64'
  shell: pwsh
  run: |
    cd build/ci-windows
    cpack -G NSIS -C Release
    # Output lands at BALLView-<version>-win64.exe
```

Plus signing on top per [08-SIGNING-RESEARCH.md](08-SIGNING-RESEARCH.md).

## What Phase 8 needs to deliver (installer format additions)

Adding to the existing Phase 8 deliverables list:

7. **macOS DMG via `create-dmg`** — replaces the current `ditto -c -k` zip. Includes drag-to-Applications shortcut, custom volume background, BALL branding. Signed and notarized via the Phase 8 signing flow.
8. **Windows installer via CPack NSIS** — replaces the current `Compress-Archive` zip. Produces `BALLView-${VERSION}-win64.exe`. Modernize `BALLPackageConfig.cmake` (drop MSVC 2010-2015 hardcoding). Signed via SignPath Foundation.
9. **`packaging/macos/dmg-background.png`** — design asset, 1024×640 PNG with drag-to-Applications visual hint. Commits to the repo.
10. **License + welcome RTF** — already referenced in `BALLPackageConfig.cmake:11-13` (Welcome.rtf, COPYRIGHT). Verify these exist and have current content; update for v1.6 → v1.7.

## Decisions locked (2026-05-15)

1. **DMG vs PKG for macOS — DMG.** BALLView is GUI-only; DMG is the right answer. PKG only enters the picture if BALL adds command-line tool distribution as a separate offering (not in current scope).
2. **NSIS vs WiX timing — NSIS for Phase 8 v1.x.** WiX MSI is the future enterprise upgrade (Phase 8.1 or v2) for institutional / Group Policy distribution. NSIS leverages BALL's existing CPack scaffolding and produces a working `.exe` installer with the least friction.
3. **Per-user vs per-machine install on Windows — per-user (no admin required).** Academic / non-IT-admin users are the primary distribution audience. NSIS produces a per-user installer by default; do NOT set `CPACK_NSIS_MODIFY_PATH ON` (avoids the per-machine PATH modification, removes the admin requirement). Installer drops into `%LOCALAPPDATA%\Programs\BALLView` or similar user-writable location. Per-machine install would need an explicit `/D=C:\Program Files\BALLView` invocation by the user; we don't expose that as a default checkbox in v1.x.
4. **Installer-level translations — English only for v1.x.** BALLView's own UI translations are handled by Qt and continue to support multiple languages internally. The NSIS installer chrome (welcome page, license dialog, install location prompt) stays English-only — adding translations is a backlog item if user demand surfaces.
5. **Auto-update mechanism — DEFERRED (filed as backlog `999.8`).** Sparkle (macOS) + WinSparkle (Windows) are the canonical OSS auto-update frameworks but adding them is non-trivial (appcast hosting, signed update manifests, rollback handling). Phase 8 ships installers without auto-update; users manually re-download for v1.7 → v1.8. Auto-update is its own backlog phase that Phase 8 unblocks.

## References

### macOS DMG
- [sindresorhus/create-dmg (GitHub)](https://github.com/sindresorhus/create-dmg) — the canonical modern tool
- [Usman AR — Complete Guide: Build Qt C++ macOS Installer (.app to .dmg) with Code Signing & Notarization](https://usmantsmz.github.io/qt-macos-dmg-installer-codesign-notarization)
- [Qt for macOS — Deployment docs](https://doc.qt.io/qt-6/macos-deployment.html)
- [Last Viking — How to make a dmg package for macOS for a QT Application](https://lastviking.eu/make_dmg_for_macos.html)
- [ICS — Your Guide for Deploying Qt Applications to MacOS](https://www.ics.com/blog/your-guide-deploying-qt-applications-macos)

### Windows installer
- [CopyProgramming — NSIS vs WIX vs Other Installation Packages: 2026 Guide](https://copyprogramming.com/howto/nsis-vs-wix-vs-anyother-installation-package)
- [NSIS official (3.11)](https://nsis.sourceforge.io/) — current stable
- [WiX Toolset official](https://wixtoolset.org/)
- [Inno Setup official](https://jrsoftware.org/isinfo.php)
- [Advanced Installer — Choosing the Right Windows Packaging Tool](https://www.advancedinstaller.com/choosing-the-right-windows-packaging-tool-as-developer.html)
- [doubleSlash Blog — NSIS vs. WiX comparison](https://blog.doubleslash.de/en/software-technologien/nsis-vs-wix-a-comparison-of-two-installers/)
- [CMake `CPackNSIS` documentation](https://cmake.org/cmake/help/latest/cpack_gen/nsis.html)
- [CMake `CPackWIX` documentation](https://cmake.org/cmake/help/latest/cpack_gen/wix.html)
