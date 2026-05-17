# Building BALL / VIEW / BALLView on macOS

This document describes how to build and run the BALL library, the VIEW
visualization library, and the BALLView GUI application on macOS (Apple
Silicon) against Homebrew-provided dependencies. These are the exact,
verified commands used to build the v1.6 modernization baseline on
macOS Tahoe (arm64).

**`ball_contrib` is NOT used** by this build flow and should not be
revived — its bundled ~2016-era dependency tarballs do not build on
modern macOS toolchains. All dependencies come from Homebrew instead.

## Dependencies

Install the required libraries and build tools with Homebrew:

```sh
brew install qt@5 boost eigen fftw tbb glew open-babel lp_solve libsvm flex bison
```

`bison` and `flex` are keg-only. Their `bin` directories must be put on
`PATH` when using the manual configure command below. With the preset
invocation the executable paths are set via `BISON_EXECUTABLE`/
`FLEX_EXECUTABLE` cache variables, so the `export PATH=...` step is
optional.

## Configure

Use the named preset:

```sh
cmake --preset macos-homebrew
```

This configures into `build/macos-homebrew/` and sets `CMAKE_BUILD_TYPE`,
`CMAKE_PREFIX_PATH`, `BISON_EXECUTABLE`, `FLEX_EXECUTABLE`, and the other
required cache variables automatically (see `CMakePresets.json`).

## Build

```sh
cmake --build --preset macos-homebrew --target BALL VIEW BALLView
```

## Run

Launch BALLView from the build directory, pointing the data-path
environment variables at the source `data/` tree and the dynamic
linker at the freshly built libraries:

```sh
BALL_DATA_PATH=$PWD/data BALLVIEW_DATA_PATH=$PWD/data \
  DYLD_LIBRARY_PATH=$PWD/build/macos-homebrew/lib \
  build/macos-homebrew/bin/BALLView.app/Contents/MacOS/BALLView
```

## Build presets

`CMakePresets.json` (repo root) provides four named configure/build presets:

| Preset | Platform | Use case |
|--------|----------|----------|
| `macos-homebrew` | macOS / Homebrew | Local contributor builds on macOS |
| `linux-system` | Linux / system packages | Local contributor builds on Linux |
| `windows-vcpkg` | Windows / vcpkg | Local contributor builds on Windows (requires `VCPKG_ROOT`) |
| `ci-macos` / `ci-linux` / `ci-windows` | CI runners | GitHub Actions matrix jobs |

The `ci-*` family (`ci-macos`, `ci-linux`, `ci-windows`) collectively satisfies
D-07's fourth named preset ("ci"). A single `ci` preset cannot carry
per-platform cache variables (compiler launchers, toolchain file), so it is
split per-platform — see RESEARCH Open Question 2. The `ci-*` presets also
enable ccache and set `BALL_HAS_OPENBABEL=ON` on macOS/Linux (the single
source of truth for that flag — Plan 03 adds the smoke step, not this cache
variable).

All presets use `"binaryDir": "${sourceDir}/build/${presetName}"`, so the
build tree is always `build/<preset-name>/`. Debug variants can be added as
`*-debug` presets that inherit the release one and override `CMAKE_BUILD_TYPE`
without rewriting anything (D-07).

## Code signing

**Local builds do NOT sign or notarize the bundle.** Apple Developer ID
signing + Apple notarization happen only in CI
(`.github/workflows/release.yml`) on tag pushes.

This means:

- You do **not** need an Apple Developer ID for local development.
- A locally-built `BALLView.app` carries no Developer ID signature; if
  you move it to another machine, macOS Gatekeeper will block first
  launch. Workaround: `xattr -dr com.apple.quarantine path/to/BALLView.app`
  once, or right-click → Open → confirm.
- The hardened-runtime entitlements live in
  `packaging/macos/BALLView.entitlements` and are applied only by the CI
  signing step. Local builds run without them.
- Released `.dmg`s from tagged builds carry a Developer ID signature +
  stapled notarization ticket and double-click cleanly on macOS 14 / 26.

The signing pipeline, GitHub secrets, and 2-human recovery procedure are
documented in
`.planning/phases/08a-packaging-macos/08a-SECRETS-RUNBOOK.md`.

## Notes

- `ball_contrib` is NOT used and should not be revived — the build
  relies entirely on Homebrew/system packages.
- Python bindings (`BALL_PYTHON_SUPPORT=OFF`), the RTfact raytracer
  (`USE_RTFACT=OFF`), and VRPN/SpaceNavigator are disabled.
- `CMAKE_POLICY_VERSION_MINIMUM=3.5` (set by the preset) is required so
  CMake 3.21+ accepts the project's historical minimum policy version.
