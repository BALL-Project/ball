# FFTW GPL Compliance Decision — Phase 8c

**Date:** 2026-05-17
**Phase:** 08c-packaging-linux
**Risk-register link:** v1.7-PLAN.md row "FFTW GPL compliance review needs a named owner (NEW post-review P0)"
**Plan link:** `08c-01-PLAN.md` Task 4

## Decision

**SHIP-LGPL-ONLY** for v1.7.

The Linux release matrix in `release.yml` will ship a **single SKU** per architecture:
`BALLView-lgpl-${VERSION}-linux-${ARCH}.{AppImage,deb,rpm}` — built with
`-DBALL_HAS_FFTW=OFF` and without `libfftw3-dev` installed on the runner. No
GPL-licensed BALLView binary ships from v1.7's release pipeline.

## GPL-compliance owner

**Owner: NONE assigned for v1.7.**

The Phase 8c executor (autonomous agent context, 2026-05-17) is not in a
position to name a human GPL-compliance maintainer with the legal authority
to accept ongoing responsibility for distributing GPL-relicensed BALLView
binaries on behalf of the BALL Project. Per the v1.7-PLAN P0 risk-register
mitigation, this triggers the documented fallback: ship the LGPL SKU only,
do **not** build or upload the GPL-with-FFTW SKU.

The candidate owner proposed in `08c-01-PLAN.md` Task 4 (Oliver Kohlbacher,
project lead) was not contacted in-session and has not explicitly accepted
the responsibility, so the SHIP-BOTH path is not unlocked here.

## Rationale

1. **GPL contagion is real.** FFTW 3.x is GPL-2.0-or-later. BALL is
   LGPL-2.1-or-later. Linking BALL against FFTW produces a derivative work
   that, when distributed, must be distributed under GPL-2.0-or-later terms
   — including making the corresponding source available, propagating the
   GPL notice, and accepting the GPL's distribution conditions on every
   downstream redistributor.

2. **A "ship and apologize later" posture is not acceptable for v1.7.**
   Uploading a GPL-relicensed binary to a GitHub Release without a named
   accountable maintainer creates ambiguity about who is distributing
   under what terms — that's exactly the legal exposure the risk register
   row is meant to prevent.

3. **The LGPL-only path is fully functional.** `cmake/BALLConfigFFTW.cmake`
   honors `-DBALL_HAS_FFTW=OFF`, and BALL's FFTW-dependent code paths
   (NMR shift prediction's spectral analysis, parts of `STRUCTURE` /
   `SOLVATION` that use FFTs for grid-based density work) compile out
   cleanly via the existing `BALL_HAS_FFTW` guards. Users who need FFTW
   build from source per `BUILD-linux.md` (which documents the optional
   `apt install libfftw3-dev` step alongside the GPL contagion warning).

4. **The dual-SKU scaffolding is preserved.** `cmake/BALLPackageConfig.cmake`
   contains the SKU-name flip (`-lgpl` vs `-gpl`) keyed on `BALL_HAS_FFTW`;
   `packaging/linux/COPYING.GPL` + `packaging/linux/README-LICENSING.md`
   ship the attribution text in-tree; the CPack DEB/RPM stanzas
   generator-expression-flip the license string. **Re-enabling the GPL SKU
   is a release.yml matrix-entry addition only — no code or packaging work
   is needed beyond Task 4's already-landed scaffolding.**

## REOPEN criteria

Reopen this decision and pursue SHIP-BOTH if **any** of:

- A named individual (project lead, core maintainer, or designated legal
  representative) explicitly accepts being the GPL-compliance owner — in
  writing, in the project repo (issue, ADR, or this file's history).
- Downstream user demand for an FFTW-enabled prebuilt binary surfaces via
  a GitHub issue with concrete use case (NMR shift prediction at scale,
  grid-based docking acceleration).
- The BALL Project adopts an organizational stance on GPL distribution
  (e.g. a board decision, a CONTRIBUTING.md update) that resolves the
  owner question.

When reopened: the SHIP-BOTH instructions in `08c-01-PLAN.md` Task 4 step 4
are **unblocked at that point** — add 2 matrix entries to `release.yml`
(`ubuntu-24.04` + `ubuntu-24.04-arm` × `BALL_HAS_FFTW=ON`), confirm the
GPL SKU's DEB/RPM/AppImage all include `COPYING.GPL` + `README-LICENSING.md`
adjacent to the binary, and update this file's "Decision" section to
SHIP-BOTH with the named owner.

## FFTW upstream license reference

- Upstream: https://www.fftw.org/
- License: GPL-2.0-or-later (FFTW's `COPYING` is the GPL v2 text with the
  "or later" clause).
- Bundled text: `packaging/linux/COPYING.GPL` (full GPL-2.0 text — matches
  FFTW's actual license; the previous draft of bundling GPL-3.0 would have
  been an over-relicensing).

## Attribution chain (for the eventual GPL SKU)

When SHIP-BOTH is re-enabled, these files MUST be packaged adjacent to the
GPL-SKU binary in all three formats (DEB / RPM / AppImage):

1. `packaging/linux/COPYING.GPL` — full GPL-2.0-or-later text.
2. `packaging/linux/README-LICENSING.md` — plain-language explanation of
   the LGPL → GPL upgrade triggered by the FFTW link.
3. The BALL `COPYRIGHT` file (already in-tree at repo root) — BALL's own
   LGPL notice + author list, preserved through the relicensing.

CMake `install(FILES ... DESTINATION share/doc/BALLView)` covers DEB/RPM;
the AppImage path is handled via `linuxdeploy`'s `--custom-apprun` or by
copying into `AppDir/usr/share/doc/BALLView/` before invoking
`linuxdeploy ... --output appimage`.
