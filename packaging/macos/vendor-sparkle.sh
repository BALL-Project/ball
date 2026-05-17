#!/usr/bin/env bash
# packaging/macos/vendor-sparkle.sh
# -----------------------------------------------------------------------------
# Phase 999.8-full — Sparkle XCFramework vendoring (macOS auto-update).
#
# FIRST-RUN: execute this script ONCE before the first signed macOS release.
# CI re-uses the committed Sparkle.xcframework (see .gitignore exception) OR
# downloads on demand if absent. Local developer machines should run it once
# after cloning if they want to build BALLView with auto-update enabled.
#
# Output: packaging/macos/Sparkle.xcframework/
# Source: https://github.com/sparkle-project/Sparkle/releases (Sparkle 2.6.x)
# License: MIT (compatible with BALL LGPL-2.1).
#
# CMake gate: source/APPLICATIONS/BALLVIEW/CMakeLists.txt defines
# BALL_HAS_SPARKLE iff this directory exists at configure time. Without it,
# Sparkle integration is compiled out and BALLView builds + runs without
# auto-update (used by local devs and the unsigned-fallback CI path).
# -----------------------------------------------------------------------------

set -euo pipefail

# --- Configuration -----------------------------------------------------------
SPARKLE_VERSION="${SPARKLE_VERSION:-2.6.4}"
# SHA256 of the official Sparkle-${VERSION}.tar.xz release artifact, pinned
# for supply-chain integrity. Update both fields together when bumping the
# version. Look up the current value on the GH release page:
#   https://github.com/sparkle-project/Sparkle/releases/tag/${SPARKLE_VERSION}
# -- the SHA256 is in the release notes ("Code Signing & Checksums" section).
SPARKLE_TARBALL_SHA256="${SPARKLE_TARBALL_SHA256:-REPLACE_WITH_RELEASE_SHA256_BEFORE_FIRST_USE}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DEST_DIR="${SCRIPT_DIR}/Sparkle.xcframework"
TARBALL_URL="https://github.com/sparkle-project/Sparkle/releases/download/${SPARKLE_VERSION}/Sparkle-${SPARKLE_VERSION}.tar.xz"

if [ -d "${DEST_DIR}" ]; then
  echo "Sparkle.xcframework already present at:"
  echo "  ${DEST_DIR}"
  echo "Remove it manually and re-run to re-vendor (e.g. on version bump)."
  exit 0
fi

# --- Download + verify + extract --------------------------------------------
TMP_DIR="$(mktemp -d)"
trap 'rm -rf "${TMP_DIR}"' EXIT
TARBALL="${TMP_DIR}/Sparkle-${SPARKLE_VERSION}.tar.xz"

echo "Downloading Sparkle ${SPARKLE_VERSION} from:"
echo "  ${TARBALL_URL}"
curl -fSL --retry 3 -o "${TARBALL}" "${TARBALL_URL}"

if [ "${SPARKLE_TARBALL_SHA256}" != "REPLACE_WITH_RELEASE_SHA256_BEFORE_FIRST_USE" ]; then
  echo "Verifying SHA256..."
  echo "${SPARKLE_TARBALL_SHA256}  ${TARBALL}" | shasum -a 256 -c -
else
  echo "::warning::SPARKLE_TARBALL_SHA256 is the placeholder — set it before"
  echo "::warning::first signed release. Computed hash for reference:"
  shasum -a 256 "${TARBALL}"
fi

echo "Extracting..."
tar -xJf "${TARBALL}" -C "${TMP_DIR}"

# The Sparkle release tarball contains a top-level Sparkle.xcframework
# alongside support binaries (generate_keys, sign_update). We vendor only
# the XCFramework; the signing tools live in vendor/bin/ via a separate
# manual procurement step (see ED25519-KEY-RUNBOOK.md).
if [ ! -d "${TMP_DIR}/Sparkle.xcframework" ]; then
  echo "::error::Sparkle.xcframework not found in extracted tarball"
  ls -la "${TMP_DIR}"
  exit 1
fi

mv "${TMP_DIR}/Sparkle.xcframework" "${DEST_DIR}"
echo "Sparkle.xcframework vendored to:"
echo "  ${DEST_DIR}"
echo ""
echo "Next: re-run \`cmake --preset ci-macos\` to pick up BALL_HAS_SPARKLE=ON."
