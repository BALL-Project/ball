# Building BALL / VIEW / BALLView on Windows (x64)

This document describes how to build and run the BALL library, the VIEW
visualization library, and the BALLView GUI application on Windows x64 with
Visual Studio 2022 (MSVC) + Ninja, mirroring the exact toolchain the CI
release pipeline uses (`.github/workflows/release.yml`, Windows job;
`.github/workflows/ci.yml`, `build (windows-x64)` matrix entry).

**Dependency model (hybrid, since Phase 999.66):**

- **Qt 6.8** comes from a **prebuilt aqtinstall kit** (the
  `jurplel/install-qt-action` wrapper around `aqtinstall`), arch
  `win64_msvc2022_64`. Qt is **no longer built by vcpkg** — it is not in
  `vcpkg.json`.
- **Everything else** (Boost, Eigen3, FFTW3, oneTBB, GLEW, libsvm,
  WinSparkle) comes from **vcpkg in manifest mode** (`vcpkg.json` at the
  repo root), built into the `x64-windows-release` overlay triplet
  (`cmake/vcpkg-triplets/x64-windows-release.cmake`).

**Verified against:** the `windows-2025` GitHub-hosted runner. Local Windows
11 / Windows 10 x64 builds with the same toolchain versions should be
identical.

> The shipped Windows release artifact is **LGPL** — the Windows Configure
> step does **not** pass `-DBALL_LICENSE=GPL`, so the GPL-gated optional deps
> (OpenBabel; FFTW *as compiled into BALL*) are inactive even though the
> `fftw3` vcpkg port is present in the manifest. See `LICENSE-REVIEW.md`.

---

## 1. Prerequisites

### 1a. Visual Studio 2022 + MSVC

Install **Visual Studio 2022** (Community is fine) with the
**"Desktop development with C++"** workload, which provides the MSVC v143
toolset (`cl.exe`), the Windows 10/11 SDK, and the CMake/Ninja components.

The CI runners use the VS 2022 MSVC toolchain via `vcvars64.bat`. For a
manual build, open an **"x64 Native Tools Command Prompt for VS 2022"** (or
run `vcvarsall.bat x64`) so `cl.exe` is the active compiler — this is
**mandatory** under the Ninja generator. Ninja invokes `cl.exe` directly and
does NOT auto-discover the MSVC environment the way the Visual Studio
generator did; without `vcvars64`, CMake may pick up MinGW `gcc` from `PATH`
and fail on Windows headers (`LARGE_INTEGER not declared`, etc.).

### 1b. Build tools — flex, bison, ccache, ninja (Chocolatey)

CI provisions these via Chocolatey (see
`.github/actions/setup-windows-deps/action.yml`). The flex/bison package is
**`winflexbison3`**, which installs `win_flex.exe` and `win_bison.exe` into
`C:\ProgramData\chocolatey\bin\`:

```powershell
choco install winflexbison3 ccache ninja -y --no-progress

# Confirm the executables are reachable (CI does the same probe):
& "C:\ProgramData\chocolatey\bin\win_flex.exe" --version
& "C:\ProgramData\chocolatey\bin\win_bison.exe" --version
ccache --version
ninja --version
```

`win_flex.exe` / `win_bison.exe` are passed to CMake explicitly via
`-DFLEX_EXECUTABLE` / `-DBISON_EXECUTABLE` (see §4) — they do not need to be
on `PATH`, but the Chocolatey install puts them there anyway.

### 1c. vcpkg

vcpkg drives Boost / Eigen3 / FFTW3 / oneTBB / GLEW / libsvm / WinSparkle in
**manifest mode** (the `vcpkg.json` at the repo root is auto-detected). The
GitHub runners ship a preinstalled vcpkg at `%VCPKG_INSTALLATION_ROOT%`; the
Configure step sets `VCPKG_ROOT` to it. For a local build, either:

- Use an existing vcpkg checkout and set `VCPKG_ROOT` to its path, **or**
- Clone + bootstrap one:
  ```powershell
  git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
  C:\vcpkg\bootstrap-vcpkg.bat
  $env:VCPKG_ROOT = "C:\vcpkg"
  ```

You do **not** run `vcpkg install` by hand — the `windows-vcpkg` CMake
preset wires the vcpkg toolchain file, so CMake's configure step restores /
builds the manifest dependencies automatically into
`build/ci-windows/vcpkg_installed/x64-windows-release/`.

> **Qt is NOT in `vcpkg.json`.** Do not add it back — Qt comes from aqt
> (next step). Adding Qt to the manifest would trigger a ~1.5–2h cold Qt
> source build and reintroduce the `windeployqt`-missing failure class.

### 1d. Qt 6.8 via aqtinstall (prebuilt kit)

CI installs Qt 6.8 through `jurplel/install-qt-action@v4` with
`arch: win64_msvc2022_64`, `version: '6.8.*'`. For a local build, either use
that prebuilt kit via `aqtinstall` directly, or install Qt 6.8 with the
official Qt online installer.

**Option A — aqtinstall (CI-equivalent):**

```powershell
pip install aqtinstall
aqt install-qt windows desktop 6.8.3 win64_msvc2022_64 -O C:\Qt
# The kit lands at C:\Qt\6.8.3\msvc2022_64 — this is your QT_ROOT_DIR.
```

**Option B — Qt online installer** (https://www.qt.io/download-qt-installer):
install **Qt 6.8.x → MSVC 2022 64-bit** to `C:\Qt\`. The kit directory
(e.g. `C:\Qt\6.8.3\msvc2022_64`) is your `QT_ROOT_DIR`.

The aqt / online kit ships its own self-contained `windeployqt.exe` at
`<QT_ROOT_DIR>\bin\windeployqt.exe` — this is what you use for deployment in
§6 (there is no vcpkg-built `qttools` anymore).

---

## 2. Clone

```powershell
git clone https://github.com/BALL-Project/ball.git
cd ball
```

---

## 3. Build presets

`CMakePresets.json` (repo root) provides the named presets. For Windows:

| Preset | Use case |
|--------|----------|
| `windows-vcpkg` | Local contributor builds on Windows (requires `VCPKG_ROOT`) |
| `ci-windows` | CI runners — inherits `windows-vcpkg`, adds Ninja generator + ccache compiler launchers |

The `ci-windows` preset is the **authoritative** release/CI configuration.
This guide uses it so a local build matches CI byte-for-byte.

---

## 4. Configure

Run from an **x64 Native Tools Command Prompt for VS 2022** (so MSVC is the
active toolchain — see §1a). The exact flags mirror release.yml's
"Configure (Windows)" step:

```powershell
# Point VCPKG_ROOT at your vcpkg checkout (CI uses VCPKG_INSTALLATION_ROOT).
$env:VCPKG_ROOT = "C:\vcpkg"

cmake --preset ci-windows `
  -DCMAKE_PREFIX_PATH="C:\Qt\6.8.3\msvc2022_64" `
  -DUSE_QTWEBENGINE=OFF `
  -DBISON_EXECUTABLE="C:/ProgramData/chocolatey/bin/win_bison.exe" `
  -DFLEX_EXECUTABLE="C:/ProgramData/chocolatey/bin/win_flex.exe" `
  -DBALL_ENABLE_PACKAGING=ON `
  "-DVCPKG_INSTALL_OPTIONS=--clean-after-build"
```

Notes:

- `CMAKE_PREFIX_PATH` must point at the **aqt/online Qt kit root** so
  `FIND_PACKAGE(Qt6)` resolves the prebuilt kit (CI passes
  `$env:QT_ROOT_DIR`, which the install-qt-action exports). The vcpkg
  toolchain no longer provides Qt6 config files.
- `BALL_ENABLE_PACKAGING=ON` is **required** for `cpack -G NSIS` later (§7)
  — it gates `INCLUDE(BALLPackageConfig)`, which registers the `CPACK_NSIS_*`
  variables (per-user install root, Start Menu shortcuts, MUI icons). Without
  it the installer falls back to bare CPack defaults.
- `--clean-after-build` is passed as the CMake cache variable
  `VCPKG_INSTALL_OPTIONS` (NOT an env var) so vcpkg's manifest-mode CMake
  integration honors it — keeps peak disk usage down during the cold vcpkg
  build.
- This configures into `build/ci-windows/` (all presets use
  `build/<preset-name>/`).

The **first** configure is slow — vcpkg cold-builds Boost/Eigen/FFTW/TBB/
GLEW/libsvm/WinSparkle into `build/ci-windows/vcpkg_installed/`. Subsequent
configures reuse the vcpkg binary cache.

---

## 5. Build

```powershell
cmake --build --preset ci-windows --config Release --target BALL VIEW BALLView
```

`--config Release` is a no-op under the Ninja single-config generator but is
kept for explicit intent (and to stay correct if the preset is ever reverted
to a multi-config generator). ccache accelerates rebuilds; the `ci-windows`
preset wires `CMAKE_<LANG>_COMPILER_LAUNCHER=ccache`.

Build output (`BALLView.exe`, `BALL.dll`, `VIEW.dll`) lands under
`build/ci-windows/` (search recursively — the exact bin subdir depends on
the generator layout).

---

## 6. Deploy Qt + third-party DLLs (windeployqt + WinSparkle)

`windeployqt` from the **aqt kit** copies the required Qt 6 DLLs + plugins
next to `BALLView.exe`. Mirror release.yml's "windeployqt (Windows)" step:

```powershell
& "C:\Qt\6.8.3\msvc2022_64\bin\windeployqt.exe" --release `
    --no-translations --no-system-d3d-compiler --no-compiler-runtime `
    --no-opengl-sw `
    path\to\BALLView.exe
```

- Qt 6's `windeployqt` **dropped** the `--no-angle` flag (ANGLE was removed
  from Qt 6) — passing it errors with `Unknown option 'no-angle'`. Do not
  add it.
- `--no-opengl-sw` skips `opengl32sw.dll` (the software-Mesa OpenGL
  fallback). BALL requires hardware OpenGL.

`windeployqt` only copies Qt DLLs. **WinSparkle.dll** (auto-update) is a
vcpkg third-party DLL and must be copied manually — vcpkg drops it at
`build/ci-windows/vcpkg_installed/x64-windows-release/bin/WinSparkle.dll`:

```powershell
Copy-Item build\ci-windows\vcpkg_installed\x64-windows-release\bin\WinSparkle.dll `
  -Destination path\to\BALLView.exe\..\ -Force
```

(If WinSparkle.dll is absent, BALLView still runs — auto-update is simply
disabled.)

---

## 7. Build the NSIS installer (optional — what CI ships)

CI packages a per-user NSIS installer via CPack. NSIS is preinstalled on the
`windows-2025` runner; install it locally with Chocolatey if absent:

```powershell
# NSIS may already be on PATH; install only if `makensis` is missing:
choco install nsis -y --no-progress

cd build\ci-windows
cpack -G NSIS -C Release --verbose
# Produces BALLView-<version>-win64-installer.exe (per-user install to
# %LOCALAPPDATA%\Programs\BALLView — no admin/UAC required).
```

CPack reads the `CPACK_NSIS_*` variables from
`cmake/BALLPackageConfig.cmake` (registered only because `BALL_ENABLE_PACKAGING=ON`
was set at configure time). The **release** installer is additionally signed
via SignPath Foundation in CI when the signing secret is present — see
`.planning/phases/999.68-packaging-signing-license/SIGNPATH-SECRETS-RUNBOOK.md`.
Until SignPath enrollment completes, the shipped installer is unsigned and
Windows SmartScreen warns on first run (see
`packaging/windows/README-SMARTSCREEN.md`).

---

## 8. Run — BALL_DATA_PATH

BALLView needs `BALL_DATA_PATH` to point at the repo's `data/` directory at
runtime so it can find force-field parameter files, fragment DB, shaders,
etc. When running from a build tree:

```powershell
$env:BALL_DATA_PATH = "$PWD\data"
path\to\BALLView.exe
```

The CI deploy step bundles `data/` next to `BALLView.exe`, and the NSIS
installer installs it alongside the binary — installed copies set / discover
the data path relative to the install root, so end users do not set
`BALL_DATA_PATH` manually. It is only needed when launching directly from a
clone / build tree.

---

## 9. Known issues

- **Must use the x64 Native Tools prompt (vcvars64).** Under Ninja, a plain
  PowerShell without the MSVC environment lets CMake pick up MinGW `gcc`,
  which fails on Windows SDK headers. Always configure + build from the
  VS 2022 x64 Native Tools prompt.
- **Cold vcpkg is slow + disk-hungry.** The first configure builds all
  manifest deps from source. `--clean-after-build` (already in the
  Configure command) trims intermediate build trees to keep peak disk use
  manageable.
- **Qt must be the aqt/online kit, not vcpkg.** If `FIND_PACKAGE(Qt6)`
  fails, confirm `CMAKE_PREFIX_PATH` points at the kit root
  (`...\msvc2022_64`), not a `bin/` subdir.
- Without `BALL_DATA_PATH` set when running from a build tree, BALLView
  starts but rendering fails with "no data" errors.

---

## See also

- `BUILD-macos.md` — macOS Apple Silicon build (Homebrew)
- `BUILD-linux.md` — Linux build (apt + aqt Qt)
- `LICENSE-REVIEW.md` — per-component license audit + shipped-artifact license
- `.github/workflows/release.yml` — CI's authoritative Windows build + package commands
- `.github/actions/setup-windows-deps/action.yml` — canonical Windows tooling install
- `packaging/windows/README-SMARTSCREEN.md` — SmartScreen first-run guidance (unsigned installer)
- `.planning/phases/999.68-packaging-signing-license/SIGNPATH-SECRETS-RUNBOOK.md` — SignPath signing setup
