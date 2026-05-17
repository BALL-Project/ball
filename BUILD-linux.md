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
  -DBALL_ENABLE_PACKAGING=ON \
  -DCMAKE_INSTALL_PREFIX=/usr
cmake --build --preset ci-linux --target BALL VIEW BALLView
```

`BALL_ENABLE_PACKAGING=ON` is required if you intend to run `cpack` later (see §5a) — without it the CMake build never includes `BALLPackageConfig.cmake` and `cpack` errors with "CPack generator not specified".

The `ci-linux` preset sets Ninja generator, ccache compiler-launcher, and Qt 6.8 LTS floor checks.

## 4. Run

```bash
export BALL_DATA_PATH=$PWD/data
./build/ci-linux/bin/BALLView
```

`BALL_DATA_PATH` must point at the repo's `data/` directory at runtime — without it, BALLView can't find force-field parameter files, shaders, etc.

## 5. Package as AppImage / DEB / RPM (Phase 8c part-2)

### 5a. CPack: TGZ + DEB + RPM in one pass

```bash
# Install CPack DEB/RPM tooling (the RPM generator works on Ubuntu via
# rpmbuild from apt's `rpm` package — distro-independent, the resulting
# .rpm installs fine on Fedora 40+).
sudo apt-get install -y dpkg-dev rpm desktop-file-utils file

cd build/ci-linux
# NOTE: do NOT pre-install via `cmake --install . --prefix /usr` here. CPack
# runs its own staged install into _CPack_Packages/ before each generator
# rolls its artifact — a manual install to a system prefix (/usr) only
# fails with EACCES on non-root systems and adds nothing CPack needs.
cpack -B $PWD/dist
ls dist/
# Produces (under dist/):
#   BALLView-lgpl-1.7.0-linux-x86_64.tar.gz
#   BALLView-lgpl-1.7.0-linux-x86_64.deb
#   BALLView-lgpl-1.7.0-linux-x86_64.rpm
```

CPack reads `CPACK_GENERATOR="TGZ;DEB;RPM"` from `cmake/BALLPackageConfig.cmake`'s Linux branch — one invocation, three artifacts. DEB dependencies (Qt6, Boost, TBB, Eigen3, OpenBabel) are declared explicitly; RPM uses `Requires:` against the canonical Fedora package names.

### 5b. AppImage via linuxdeploy + linuxdeploy-plugin-qt

The canonical Qt 6 portable artifact — bundles Qt 6.8 + QML + plugins/ + translations/ into a single `.AppImage` that runs on any glibc 2.35+ system (Ubuntu 22.04, Debian 12, Fedora 40+).

```bash
# Fetch linuxdeploy + linuxdeploy-plugin-qt (rolling `continuous` release).
ARCH=$(uname -m)   # x86_64 or aarch64
mkdir -p /tmp/ld && cd /tmp/ld
curl -fLO https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-${ARCH}.AppImage
curl -fLO https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-${ARCH}.AppImage
chmod +x linuxdeploy-${ARCH}.AppImage linuxdeploy-plugin-qt-${ARCH}.AppImage

# Stage AppDir from the cmake install tree.
APPDIR=/tmp/AppDir
rm -rf "$APPDIR"
DESTDIR="$APPDIR" cmake --install $REPO/build/ci-linux --prefix /usr --strip

# Bundle data/ for BALL_DATA_PATH discovery inside the AppImage.
mkdir -p "$APPDIR/usr/share/BALL"
cp -R $REPO/data "$APPDIR/usr/share/BALL/"

# Point QMAKE at Qt 6.8's qmake6 so the plugin's qmake probe finds the right Qt.
export QMAKE=$HOME/Qt/6.8.3/gcc_64/bin/qmake6

# Headless CI (no FUSE) needs the extract-and-run escape hatch — harmless
# locally, required on GitHub-hosted runners.
export APPIMAGE_EXTRACT_AND_RUN=1

./linuxdeploy-${ARCH}.AppImage \
  --appdir "$APPDIR" \
  --desktop-file $REPO/packaging/linux/BALLView.desktop \
  --icon-file $REPO/packaging/linux/BALLView.svg \
  --plugin qt \
  --output appimage
# Produces BALLView-*.AppImage in the current directory.

chmod +x BALLView-*.AppImage
./BALLView-*.AppImage    # remember: export BALL_DATA_PATH if running from a clone tree
```

`packaging/linux/BALLView.desktop` is the FreeDesktop launcher entry (categories: Science / Chemistry / Education). `packaging/linux/BALLView.svg` is the placeholder icon — replaceable without touching CI.

### 5c. Fallback: `fpm` (when CPack DEB/RPM aren't an option)

If CPack's generators give trouble (rare — typically a CMake-install vs. component mismatch), `fpm` re-wraps an existing `cmake --install` tree into a .deb or .rpm without a full CMake-CPack round trip:

```bash
cmake --install build/ci-linux --prefix /tmp/ballview-stage/usr
fpm -s dir -t deb -n ballview -v 1.7.0 -C /tmp/ballview-stage \
  --description "BALL molecular visualization" \
  --depends "libqt6core6 (>= 6.8)" --depends "libqt6widgets6 (>= 6.8)" \
  .
```

This is a documented fallback — CPack is the primary path in CI.

## 6. Known issues

- Without `BALL_DATA_PATH` set, BALLView starts but rendering fails with "no data" errors. Always export before launching. (AppImage bundles `data/` under `$APPDIR/usr/share/BALL/data` — set `BALL_DATA_PATH` to that path or wrap the AppImage in an AppRun script for v2.)
- The portable tarball produced by `cpack -G TGZ` does NOT bundle Qt 6 — the target system must have Qt 6.8+ installed. The `.AppImage` produced by linuxdeploy-plugin-qt DOES bundle Qt.
- FFTW-dependent code paths (NMR spectra processing, X-ray crystallography helpers) compile but emit a runtime warning and return empty results. Acceptable for the LGPL SKU.
- AppImage on headless CI requires `APPIMAGE_EXTRACT_AND_RUN=1` (no FUSE on GitHub runners). Local desktops with FUSE2/3 don't need it.
- `.deb` declares Boost 1.83 (Ubuntu 24.04) | 1.74 (Debian 12) as an alternation — installs cleanly on both. Older Boost (Ubuntu 22.04's 1.74) is technically below project minimum but the alternation keeps `apt install` from failing on the dep-resolver before BALL itself rejects an old Boost at runtime.

---

## See also

- `BUILD-macos.md` — macOS Apple Silicon build
- `.github/workflows/release.yml` — CI's authoritative build commands per platform
- `.planning/phases/08c-packaging-linux/08c-01-PLAN.md` — full Phase 8c plan (including AppImage / .deb / .rpm parts deferred to part-2)
