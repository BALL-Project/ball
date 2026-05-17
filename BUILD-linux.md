# BUILD-linux.md — from-source build on Ubuntu 24.04 (x86_64 + arm64)

**Target:** BALL + VIEW + BALLView, LGPL-only (no FFTW per Phase 8c's SHIP-LGPL-ONLY decision; see `.planning/phases/08c-packaging-linux/08c-FFTW-GPL-DECISION.md` for rationale).

**Tested on:** ubuntu-24.04 (x86_64) and ubuntu-24.04-arm (ARM64) GitHub-hosted runners. Local Ubuntu 24.04 LTS builds should be identical.

---

## 1. System packages

```bash
sudo apt-get update
sudo apt-get install -y \
  build-essential cmake ninja-build flex bison ccache \
  libboost-all-dev libeigen3-dev libglew-dev libtbb-dev \
  libopenbabel-dev liblpsolve55-dev libsvm-dev \
  xvfb mesa-utils libgl1-mesa-dev libglu1-mesa-dev \
  patchelf
```

**NOTE:** `libfftw3-dev` is intentionally NOT installed (SHIP-LGPL-ONLY for v1.7 — FFTW would change the binary's effective license to GPL via static linking, requiring a named GPL-compliance maintainer that v1.7 doesn't have).

## 2. Qt 6.8 LTS

Ubuntu 24.04's apt-shipped Qt is 6.4.2 (below BALL's 6.8 LTS floor). Two options:

**Option A — `jurplel/install-qt-action` (CI-equivalent path):** see `.github/workflows/ci.yml` for the exact step. Local equivalent:

```bash
pip install aqtinstall
aqt install-qt linux desktop 6.8.3 gcc_64 -m qtimageformats
export PATH=$HOME/Qt/6.8.3/gcc_64/bin:$PATH
export CMAKE_PREFIX_PATH=$HOME/Qt/6.8.3/gcc_64:$CMAKE_PREFIX_PATH
```

**Option B — Qt online installer** (https://www.qt.io/download-qt-installer): install Qt 6.8.3 to `$HOME/Qt/`. Same `PATH` + `CMAKE_PREFIX_PATH` exports.

## 3. Configure + build

```bash
cmake --preset ci-linux \
  -DBALL_HAS_FFTW=OFF \
  -DCMAKE_INSTALL_PREFIX=/usr
cmake --build --preset ci-linux --target BALL VIEW BALLView
```

The `ci-linux` preset sets Ninja generator, ccache compiler-launcher, and Qt 6.8 LTS floor checks.

## 4. Run

```bash
export BALL_DATA_PATH=$PWD/data
./build/ci-linux/bin/BALLView
```

`BALL_DATA_PATH` must point at the repo's `data/` directory at runtime — without it, BALLView can't find force-field parameter files, shaders, etc.

## 5. Package as portable tarball

```bash
cd build/ci-linux
cpack -G TGZ -B $PWD/dist
ls dist/*.tar.gz
```

For .AppImage, .deb, .rpm — deferred to Phase 8c part-2 (linuxdeploy-plugin-qt + CPack DEB + CPack RPM generators).

## 6. Known issues

- Without `BALL_DATA_PATH` set, BALLView starts but rendering fails with "no data" errors. Always export before launching.
- The portable tarball produced by `cpack -G TGZ` does NOT bundle Qt 6 — the target system must have Qt 6.8+ installed. Phase 8c part-2 will produce AppImage with bundled Qt.
- FFTW-dependent code paths (NMR spectra processing, X-ray crystallography helpers) compile but emit a runtime warning and return empty results. Acceptable for the LGPL SKU.

---

## See also

- `BUILD-macos.md` — macOS Apple Silicon build
- `.github/workflows/release.yml` — CI's authoritative build commands per platform
- `.planning/phases/08c-packaging-linux/08c-01-PLAN.md` — full Phase 8c plan (including AppImage / .deb / .rpm parts deferred to part-2)
