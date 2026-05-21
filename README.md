# BALL — Biochemical Algorithms Library

[![Latest release](https://img.shields.io/github/v/release/BALL-Project/ball?sort=semver&label=release)](https://github.com/BALL-Project/ball/releases/latest)
[![Downloads](https://img.shields.io/github/downloads/BALL-Project/ball/total?label=downloads)](https://github.com/BALL-Project/ball/releases)
[![CI](https://github.com/BALL-Project/ball/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/BALL-Project/ball/actions/workflows/ci.yml)
[![License: LGPL v2.1](https://img.shields.io/badge/license-LGPL%20v2.1-blue.svg)](LICENSE)

**BALL** is a C++ framework for molecular modelling and computational structural
bioinformatics. **BALLView** is its OpenGL-based molecular-visualization GUI
application.

## Download

Pre-built, ready-to-run **BALLView** packages for macOS, Windows, and Linux are
published on the **[latest release](https://github.com/BALL-Project/ball/releases/latest)**
page. macOS builds are signed + notarized (as of v1.7.1) and Windows installers
are code-signed.

| Platform | Download |
|----------|----------|
| **macOS** (Apple Silicon) | `BALLView-<version>-macos-arm64.dmg` |
| **Windows** (x64) | `BALLView-<version>-windows-x64-installer.exe` (installer) or `BALLView-<version>-windows-x64.zip` (portable) |
| **Linux** (x64 / arm64) | `.deb`, `.rpm`, `.AppImage`, or `.tar.gz` |

→ See **[all releases](https://github.com/BALL-Project/ball/releases)** for older
versions and release notes.

## Build from source

BALL uses the CMake build system. Dependencies come from system/Homebrew packages
on macOS/Linux and from vcpkg on Windows. Per-platform, step-by-step instructions
live in:

- **macOS:** [`BUILD-macos.md`](BUILD-macos.md)
- **Linux:** [`BUILD-linux.md`](BUILD-linux.md)
- **Windows / general:** the [Development Environment wiki](https://github.com/BALL-Project/ball/wiki/DevelopmentEnvironment)

The generic flow, once the dependencies are in place:

```sh
# From the top-level directory (the one with CMakeLists.txt):
cmake -B build -S .
cmake --build build
```

To produce installable packages (macOS/Windows/Linux), configure with
`-DBALL_ENABLE_PACKAGING=ON` and build the `package` target:

```sh
cmake -B build -S . -DBALL_ENABLE_PACKAGING=ON
cmake --build build --target package
```

On Windows, run the commands from a Visual Studio command shell.

## Documentation

- API documentation can be built locally with `cmake --build build --target doc`
  (requires Doxygen).
- Developer and build documentation: the
  [project wiki](https://github.com/BALL-Project/ball/wiki).

## Feedback and contact

Please help us improve BALL by reporting problems, bugs, criticism, and feature
requests — either via the GitHub issue tracker or the mailing lists:

- Issues: <https://github.com/BALL-Project/ball/issues>
- User list: <https://groups.google.com/forum/#!forum/ball-user-list>
- Developer list: <https://groups.google.com/forum/#!forum/ball-developer-list>
- Project website: <https://ball-project.org>

## Citing BALL

If you use BALL, please cite (DOI:
[10.1186/1471-2105-11-531](https://doi.org/10.1186/1471-2105-11-531)):

> Andreas Hildebrandt, Anna Katharina Dehof, Alexander Rurainski, Andreas Bertsch,
> Marcel Schumann, Nora C Toussaint, Andreas Moll, Daniel Stöckel, Stefan Nickels,
> Sabine C Müller, Hans-Peter Lenhof, Oliver Kohlbacher:
> *BALL — Biochemical Algorithms Library 1.3*, BMC Bioinformatics, 11:531 (2010).

## License

BALL is distributed under the GNU Lesser General Public License v2.1
([`LICENSE`](LICENSE)).
