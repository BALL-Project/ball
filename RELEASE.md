# BALLView — Release Formats

Authoritative contract for what gets shipped, in which format, on which
platform, signed how. The release workflow that implements this contract
lives at [`.github/workflows/release.yml`](.github/workflows/release.yml)
and fires on any `v*.*.*` tag push (or via manual workflow dispatch).

## Quick reference

| Platform     | Format                  | Architecture | Signed?              | Notarized? | Auto-update? | Status |
|--------------|-------------------------|--------------|----------------------|------------|--------------|--------|
| macOS        | **`.dmg`**              | arm64        | No (ad-hoc resign)   | No         | No           | Shipping v1.6.x |
| Windows      | **`.zip`** (extract+run) | x64          | No                   | n/a        | No           | Shipping v1.6.x |
| Linux        | **build from source**   | x64 + arm64  | n/a                  | n/a        | n/a          | Build instructions only |

**Tag-shape convention:** `vMAJOR.MINOR.PATCH` (semver). Tags must match
the glob `v*.*.*` to auto-trigger `release.yml`.

**Artifact name convention:** `BALLView-vMAJOR.MINOR.PATCH-{platform}.{ext}`
where `{platform}` is `macos-arm64` or `windows-x64` and `{ext}` is `dmg`
or `zip` per the table above.

## Per-platform details

### macOS (`.dmg`, arm64)

Asset: `BALLView-v{VERSION}-macos-arm64.dmg`

The `.dmg` is built via `hdiutil create -format UDZO -fs HFS+ -volname
BALLView` after the standard macdeployqt + data-bundle + ad-hoc-resign
pipeline. Mounts in Finder as `/Volumes/BALLView` containing
`BALLView.app`. Standard install gesture: double-click the .dmg →
drag `BALLView.app` to `/Applications`.

The bundle:

- Runs on macOS arm64 (Apple Silicon). Intel Macs are not currently
  supported (universal binary is a Phase 8 deliverable).
- Built against Homebrew Qt 6 (currently 6.11+), bundled into the
  `.app` via `macdeployqt`.
- Carries the runtime `data/` tree at `Contents/Resources/data/`
  (force-field parameters, fragment DB, rotamer libraries, training
  data). `BALL::Path::find()` resolves these without any env var.
- Is **not** code-signed with a Developer ID, and is **not** notarized
  by Apple. The `codesign --force --deep --sign -` ad-hoc seal is only
  there to satisfy macOS 26 (Tahoe)'s post-modification load-policy
  check — it is **not** a trust signature.

**First-launch dance** (because there's no Developer ID signature):

- Right-click `BALLView.app` → Open → confirm the "unidentified
  developer" warning. Or
- Once before launching: `xattr -dr com.apple.quarantine
  /Applications/BALLView.app`

### Windows (`.zip`, x64)

Asset: `BALLView-v{VERSION}-windows-x64.zip`

The `.zip` contains a `windeployqt`-processed install tree
(`BALLView.exe` + Qt DLLs + vcpkg-provided third-party DLLs + `data/`).
Extract anywhere; run `BALLView.exe`.

The bundle:

- Runs on Windows 10 / 11 x64. ARM Windows is not currently
  supported.
- Built against vcpkg's Qt 6.8 LTS manifest.
- Has Microsoft VC++ runtime DLLs bundled (no separate redistributable
  install required).
- Is **not** code-signed. SmartScreen will warn — choose "More info →
  Run anyway" on first launch.

### Linux — build from source

No Linux binary installer is currently shipped. Linux users build from
source against system Qt 6.8 LTS + apt-provided dependencies. See
`BUILD-macos.md` for the macOS recipe and the GitHub Actions CI
workflow at the released tag for the Linux + Windows verbatim recipes.

Linux installer formats (AppImage / Flatpak / .deb / .rpm) are deferred
to v2.0+ — not a v1.6.x or v1.7 deliverable.

## Format choice rationale

- **macOS = DMG, not ZIP.** DMG is the canonical macOS distribution
  format — Finder double-click → mount → drag-to-Applications is the
  install gesture Mac users expect. ZIP works but breaks the native
  flow and feels off. v1.6.2 initially shipped `.zip`, was flagged
  2026-05-17, re-tagged with `.dmg` (iteration 5+) and the contract
  was promoted into this file (iteration 6+).
- **Windows = ZIP, not MSI / EXE installer.** Until SignPath
  Foundation provisions a signing cert for BALL (Phase 8 v1.7 work),
  an unsigned MSI / EXE installer hits the same SmartScreen wall as
  the .exe inside a .zip — but the .zip extraction step makes the
  trust prompt clearer to the user ("I extracted this and chose to
  run it") versus a silent installer that drops binaries somewhere
  and runs them. Phase 8 ships the signed CPack-NSIS `.exe`.
- **Linux = source, no binary.** Linux distros prefer their own
  packaging conventions (apt, dnf, flatpak) and v1.6.x ships a too-small
  team to maintain four sets of distribution packaging concurrently.
  AppImage is the most likely first binary target in v2.0.

## Future: Phase 8 / v1.7 packaging upgrades

Phase 8 ([roadmap entry](.planning/ROADMAP.md), targeted for v1.7)
upgrades each row of this contract:

| Platform | Today (v1.6.x)                    | Phase 8 (v1.7)                                          |
|----------|------------------------------------|---------------------------------------------------------|
| macOS    | unsigned `.dmg` via `hdiutil`      | signed + notarized `.dmg` via `create-dmg` (branded, drag-to-Applications symlink, BALL background); Apple Developer ID + notarytool + stapler |
| Windows  | unsigned `.zip`                   | signed `.exe` installer via CPack-NSIS; SignPath Foundation cert (BALL's LGPL-2.1 qualifies as free OSS) |
| Linux    | build from source                 | (no change in v1.7; AppImage deferred to v2.0)         |
| Both     | manual download + replace install | auto-update via Sparkle (macOS) + WinSparkle (Windows) — Phase 999.8, sequenced after Phase 8 |

See:
- [`.planning/phases/08-packaging-and-distribution/08-SIGNING-RESEARCH.md`](.planning/phases/08-packaging-and-distribution/08-SIGNING-RESEARCH.md)
- [`.planning/phases/08-packaging-and-distribution/08-INSTALLER-FORMATS-RESEARCH.md`](.planning/phases/08-packaging-and-distribution/08-INSTALLER-FORMATS-RESEARCH.md)
- ROADMAP §"Phase 8: Packaging & Distribution" — has the v1.7 decisions locked

## Release process (operator notes)

1. Land all release content on the milestone branch
   (`v1.6-modernization` for v1.6.x; future milestones get their own).
2. Verify CI green on the milestone branch HEAD across the full
   matrix (currently macOS arm64 + Linux x64 + Linux arm64 +
   Windows x64).
3. Update `.planning/RELEASE-NOTES-vMAJOR.MINOR.PATCH.md` with the
   final scope summary + user-facing notes.
4. Create the tag: `git tag -a vMAJOR.MINOR.PATCH HEAD -m "..."`.
5. Push the tag: `git push origin vMAJOR.MINOR.PATCH`.
6. `release.yml` fires automatically on tag push; it builds the
   per-platform artifacts and creates a **draft** GitHub Release with
   placeholder body.
7. Replace the placeholder body with the finalized release-notes:
   `gh release edit vMAJOR.MINOR.PATCH --notes-file .planning/RELEASE-NOTES-vMAJOR.MINOR.PATCH.md --draft=false`

**Re-tag (iteration) protocol** when a release iteration is needed
post-publish:

1. `gh release delete vMAJOR.MINOR.PATCH --yes --cleanup-tag=false`
2. `git tag -d vMAJOR.MINOR.PATCH`
3. `git push origin :refs/tags/vMAJOR.MINOR.PATCH`
4. `git tag -a vMAJOR.MINOR.PATCH <new-commit-sha> -m "iteration N: ..."`
5. `git push origin vMAJOR.MINOR.PATCH`
6. Wait for new `release.yml` run, then re-edit body + publish.

Force-moving tags is acceptable for in-development iterations on a
release that has NOT yet been pulled by users (verify via
`gh release view <tag> --json assets` showing `downloadCount: 0`).
Once a release has been downloaded by users, **do not force-move the
tag** — cut a patch release instead (e.g., v1.6.3).
