# BALL / BALLView — License & Distribution Review

**Phase:** 999.68 (Packaging & Distribution completion)
**Scope:** per-component license audit of every dependency linked into the
shipped BALL / VIEW / BALLView binaries, the resulting effective license of
each platform's distributed artifact, and the license texts / NOTICE
material that must accompany the binaries.
**Requirement:** LICREVIEW-01.
**Last reviewed:** 2026-05-30.

> This is an engineering license audit, **not legal advice**. It records the
> licenses as declared by each upstream project and the maintainer's
> distribution decisions (the LGPL/GPL build gate). Have counsel confirm
> before a formal redistribution if institutional policy requires it.

---

## 1. BALL's own license

BALL / VIEW / BALLView are licensed under the **GNU Lesser General Public
License, version 2.1** (`LICENSE` at the repo root — "GNU LESSER GENERAL
PUBLIC LICENSE, Version 2.1, February 1999").

The build exposes a license **gate** (`CMakeLists.txt`):

```cmake
SET(BALL_LICENSE "LGPL" CACHE STRING "BALL licensing model: LGPL (default) or GPL")
```

- **`BALL_LICENSE=LGPL`** (the default) — the GPL-only optional dependencies
  (FFTW and OpenBabel) are **not searched for and not linked**. Both
  `FIND_PACKAGE(FFTW)` and `FIND_PACKAGE(OpenBabel3)` live *inside* the
  `IF(BALL_LICENSE_GPL)` block, so an LGPL build never pulls a GPL library
  into the link.
- **`BALL_LICENSE=GPL`** — FFTW + OpenBabel are searched for and, if found,
  linked. Because FFTW and OpenBabel are GPL, the **combined work becomes
  GPL** (the LGPL→GPL "upgrade" path that LGPL §3 explicitly permits).

This gate is the single most important fact for the distribution license:
**the shipped artifact's license is determined per-platform by whether CI
passes `-DBALL_LICENSE=GPL`** (see §3).

---

## 2. Per-component license audit

| Component | Upstream license | Linkage | Notes |
|---|---|---|---|
| **BALL / VIEW / BALLView** (this project) | **LGPL-2.1** | — | Library itself; LGPL §3 allows an LGPL work to be combined into a GPL whole. |
| **Qt 6.8** | **LGPL-3.0** (open-source edition) | Dynamic | LGPL-3 permits proprietary/closed combination **only if dynamically linked + relinkable**. BALL ships Qt as separate shared libs (windeployqt / macdeployqt / linuxdeploy bundle the `.dll`/`.dylib`/`.so` — never static), satisfying the LGPL-3 dynamic-linking + replaceability obligation. |
| **Boost** | **BSL-1.0** (Boost Software License) | Static (headers + libs) | Permissive; no copyleft. Attribution in NOTICE; no source-offer obligation. |
| **Eigen3** | **MPL-2.0** (a few files LGPL/BSD) | Header-only | File-level copyleft only — using Eigen headers does not impose MPL on BALL. The MPL-2 LICENSE text must accompany binaries. |
| **FFTW3** | **GPL-2.0-or-later** | Static/dynamic | **GPL** — this is *the* reason for the `BALL_LICENSE=GPL` gate. Only linked in GPL builds. (FFTW is also available under a paid non-GPL MIT license from MIT, which BALL does **not** use.) |
| **OpenBabel** | **GPL-2.0** | Dynamic | GPL; GPL-gated alongside FFTW. Only linked when `BALL_LICENSE=GPL` and OpenBabel3 is found. |
| **oneTBB (TBB)** | **Apache-2.0** | Dynamic | Permissive; patent grant. Include Apache-2.0 LICENSE + NOTICE. |
| **GLEW** | **BSD-3-Clause / MIT (Modified BSD + MIT)** | Static/dynamic | Permissive; attribution only. |
| **libsvm** | **BSD-3-Clause** | Static | Permissive; attribution only. |
| **WinSparkle** (Windows) | **MIT** | Dynamic (`WinSparkle.dll`) | Permissive. Bundles zlib/Expat internally (BSD/MIT-family) — its own NOTICE covers those. |
| **Sparkle** (macOS) | **MIT** | Dynamic (`Sparkle.framework`) | Permissive; vendored into the `.app` for auto-update. |
| **lp_solve** | **LGPL-2.1** | Dynamic (when enabled) | LGPL; `USE_LPSOLVE=OFF` on the Linux & Windows presets, so it is only potentially linked on macOS (Homebrew `lp_solve`). LGPL dynamic-linking obligation applies wherever it is linked. |

### Notes on the copyleft components

- **Qt (LGPL-3):** the binding obligation is *dynamic linking + the ability
  for a user to relink against a modified Qt*. BALL ships Qt as bundled
  shared libraries (never static), which is the standard way to satisfy
  this. The full LGPL-3 text + a written offer / source availability for Qt
  must be conveyed (Qt sources are publicly available from the Qt Project;
  pointing to the upstream tag suffices).
- **FFTW + OpenBabel (GPL):** linking either makes the entire BALL binary a
  GPL combined work. This is intentional and explicit (the `BALL_LICENSE=GPL`
  flag). GPL builds must offer complete corresponding source for the whole
  work (BALL's source is public on GitHub at the released tag — that
  satisfies GPL §3's source-offer for BALL itself; FFTW/OpenBabel sources
  are likewise public upstream).
- **lp_solve (LGPL):** same dynamic-linking + relink obligation as Qt;
  applies on macOS only under current preset settings.

---

## 3. Effective license of each shipped artifact

The license of a *distributed binary* is the union of everything statically
or dynamically linked into it. Because the GPL deps are gated, **the three
platforms ship under different effective licenses**:

| Platform | CI configure flags (release.yml) | FFTW | OpenBabel | **Effective artifact license** |
|---|---|---|---|---|
| **macOS** (arm64 `.dmg`) | `cmake --preset ci-macos -DBALL_LICENSE=GPL` | linked | linked | **GPL-2.0-or-later** (combined work) |
| **Linux** (`.AppImage`/`.deb`/`.rpm`/`.tar.gz`, x86_64 + arm64) | `cmake --preset ci-linux -DBALL_HAS_FFTW=OFF` (no `-DBALL_LICENSE=GPL`) | **not linked** | **not linked** | **LGPL-2.1** (FFTW deliberately excluded — SHIP-LGPL-ONLY) |
| **Windows** (NSIS `.exe` + portable `.zip`) | `cmake --preset ci-windows ...` (no `-DBALL_LICENSE=GPL`) | **not linked** (FFTW gated off despite the `fftw3` vcpkg port being present) | **not linked** | **LGPL-2.1** |

> **macOS ships GPL; Linux and Windows ship LGPL.** A frequent
> misconception is that "the release builds with `-DBALL_LICENSE=GPL`" — that
> is true for **macOS only**. The Windows Configure step does not pass it
> (so the `fftw3` entry in `vcpkg.json` is built by vcpkg but never compiled
> into BALL), and the Linux release ships LGPL-only with FFTW explicitly off
> (`08c-FFTW-GPL-DECISION.md`, SHIP-LGPL-ONLY until a GPL-compliance owner is
> named).

The CI test matrix (`ci.yml`) *does* build Linux with `-DBALL_LICENSE=GPL`
to keep the OpenBabel/FFTW code paths exercised — but that is the **CI test
build**, not the **shipped Linux release artifact**. Do not conflate the two.

---

## 4. What must accompany the binaries (NOTICE / license texts)

Each distributed artifact must carry the license texts for everything it
links. Recommended layout: a `licenses/` (or `THIRD-PARTY-LICENSES/`)
directory inside the installer / bundle, plus a top-level `LICENSE`.

### 4a. All platforms (always linked)

- **`LICENSE`** — BALL's LGPL-2.1 (already at the repo root).
- **Qt** — LGPL-3.0 full text + a note that Qt is dynamically linked and
  user-relinkable, with a pointer to Qt's public sources for the bundled
  version.
- **Boost** — BSL-1.0 text + attribution.
- **Eigen** — MPL-2.0 text.
- **oneTBB** — Apache-2.0 LICENSE + NOTICE.
- **GLEW** — Modified-BSD + MIT text.
- **libsvm** — BSD-3-Clause text.

### 4b. macOS (`.dmg`) — additionally

- **FFTW** — GPL-2.0 text. Because FFTW is linked, the **whole macOS
  artifact is GPL**: the bundle must convey the combined work under GPL and
  provide/offer complete corresponding source (BALL @ the release tag on
  GitHub; FFTW + OpenBabel upstream sources).
- **OpenBabel** — GPL-2.0 text.
- **Sparkle** — MIT text.
- **lp_solve** — LGPL-2.1 text (if `USE_LPSOLVE` left ON on macOS).

### 4c. Windows (`.exe` / `.zip`) — additionally

- **WinSparkle** — MIT text (covers its bundled zlib/Expat).
- (No FFTW/OpenBabel — LGPL build.)

### 4d. Linux (`.AppImage` / `.deb` / `.rpm`) — additionally

- The `.deb`/`.rpm` declare their Qt/Boost/TBB/Eigen deps; the standard
  per-distro `/usr/share/doc/<pkg>/copyright` convention covers attribution.
- No FFTW/OpenBabel (LGPL-only Linux SKU).

---

## 5. Distribution-license summary (one-liner per artifact)

- **macOS `.dmg`** → **GPL-2.0-or-later** (FFTW + OpenBabel linked).
- **Linux SKUs** → **LGPL-2.1** (FFTW intentionally excluded).
- **Windows installer / portable** → **LGPL-2.1** (GPL deps gated off).

All three remain **LGPL-2.1 at the BALL-source level**; only the macOS
*binary* is GPL because of its linked GPL libraries. Anyone redistributing
the macOS binary must comply with GPL (source offer for the whole combined
work); the Linux/Windows binaries carry the lighter LGPL obligations
(dynamic-linking + relink for Qt and any LGPL deps).

---

## See also

- `LICENSE` — BALL's LGPL-2.1 text (repo root)
- `CMakeLists.txt` — the `BALL_LICENSE` gate (§77) and GPL-gated FFTW/OpenBabel (§168)
- `BUILD-windows.md` / `BUILD-linux.md` / `BUILD-macos.md` — per-platform build flags
- `.github/workflows/release.yml` — authoritative per-platform configure flags
- `.planning/archive/phases-thru-v1.7.4/08c-packaging-linux/08c-FFTW-GPL-DECISION.md` — the SHIP-LGPL-ONLY decision for Linux
