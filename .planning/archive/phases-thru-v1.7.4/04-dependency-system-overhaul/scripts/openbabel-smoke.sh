#!/usr/bin/env bash
# -*- Mode: Shell; tab-width: 2; -*-
#
# openbabel-smoke.sh — Build and run the OpenBabel 3.x port smoke check.
#
# Compiles openbabel-smoke.C against the libBALL already built in the repo's
# build tree, then executes the resulting driver. The driver asserts:
#   1. MolecularSimilarity::createOBMol + OBConversion produces non-empty SMILES.
#   2. The same molecule produces identical SMILES twice (determinism).
#   3. A benzene ring's canonical SMILES contains lowercase aromatic 'c'.
#
# Prints OPENBABEL_SMOKE_OK and exits 0 on success.
# Prints failure details and exits 1 on any error.
#
# Usage:
#   bash openbabel-smoke.sh [path-to-build-tree]
#
# Environment:
#   BALL_BUILD_DIR   — explicit build directory override (default: auto-detected
#                      by looking for build/macos-homebrew, build/ci-macos, or
#                      build/ci-linux relative to the repo root).
#   BALL_DATA_PATH   — data directory; defaults to <repo-root>/data.
#
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/../../../.." && pwd)"

fail() {
  echo "openbabel-smoke FAIL: $*" >&2
  exit 1
}

# ----------------------------------------------------------------------------
# 1. Locate the build directory.
# ----------------------------------------------------------------------------
BUILD_DIR="${BALL_BUILD_DIR:-${1:-}}"

if [ -z "${BUILD_DIR}" ]; then
  # Auto-detect: prefer the CI preset directories, fall back to macos-homebrew.
  # libBALL lives in <build_dir>/lib/, so check that subdirectory.
  for candidate in \
    "${REPO_ROOT}/build/ci-macos" \
    "${REPO_ROOT}/build/ci-linux" \
    "${REPO_ROOT}/build/macos-homebrew" \
    "${REPO_ROOT}/build/linux-system" \
    "${REPO_ROOT}/build"
  do
    if [ -d "${candidate}" ] && \
       { [ -f "${candidate}/lib/libBALL.dylib" ] || [ -f "${candidate}/lib/libBALL.so" ] || \
         [ -f "${candidate}/libBALL.dylib" ] || [ -f "${candidate}/libBALL.so" ]; }; then
      BUILD_DIR="${candidate}"
      break
    fi
  done
fi

[ -n "${BUILD_DIR}" ] || fail "could not locate a libBALL build tree (pass build dir as \$1 or set BALL_BUILD_DIR)"
[ -d "${BUILD_DIR}" ] || fail "build directory not found: ${BUILD_DIR}"

# Locate libBALL — may be in lib/ subdirectory
LIBBALL=""
for f in \
  "${BUILD_DIR}/lib/libBALL.dylib" \
  "${BUILD_DIR}/lib/libBALL.so" \
  "${BUILD_DIR}/libBALL.dylib" \
  "${BUILD_DIR}/libBALL.so"
do
  if [ -f "${f}" ]; then
    LIBBALL="${f}"
    break
  fi
done
[ -n "${LIBBALL}" ] || fail "libBALL not found in ${BUILD_DIR}"

echo "openbabel-smoke: build dir = ${BUILD_DIR}"
echo "openbabel-smoke: libBALL   = ${LIBBALL}"

# ----------------------------------------------------------------------------
# 2. Locate OpenBabel include/lib paths (via CMakeCache or pkg-config fallback).
# ----------------------------------------------------------------------------
OBABEL_INCLUDE=""
OBABEL_LIB_DIR=""

# Try the CMakeCache for reliable paths (records exactly what was used at configure time).
CACHE_FILE="${BUILD_DIR}/CMakeCache.txt"
if [ -f "${CACHE_FILE}" ]; then
  # OpenBabel3_INCLUDE_DIRS is the most direct source
  OB_INC_CACHE="$(grep -m1 '^OpenBabel3_INCLUDE_DIRS\b' "${CACHE_FILE}" 2>/dev/null | cut -d= -f2 | tr -d '[:space:]' || true)"
  if [ -n "${OB_INC_CACHE}" ] && [ -d "${OB_INC_CACHE}" ]; then
    OBABEL_INCLUDE="${OB_INC_CACHE}"
  fi
  # Derive lib dir from OpenBabel3_DIR if include still not found
  if [ -z "${OBABEL_INCLUDE}" ]; then
    OB_ROOT="$(grep -m1 '^OpenBabel3_DIR' "${CACHE_FILE}" 2>/dev/null | cut -d= -f2 | tr -d '[:space:]' || true)"
    if [ -n "${OB_ROOT}" ]; then
      # OpenBabel3_DIR is the cmake/ dir; walk up to find the install prefix
      # (may be 2 or 3 levels up depending on Linux distro layout vs Homebrew)
      for levels in 2 3 4; do
        OB_CANDIDATE="${OB_ROOT}"
        for i in $(seq 1 ${levels}); do OB_CANDIDATE="$(dirname "${OB_CANDIDATE}")"; done
        if [ -d "${OB_CANDIDATE}/include/openbabel3" ]; then
          OBABEL_INCLUDE="${OB_CANDIDATE}/include/openbabel3"
          OBABEL_LIB_DIR="${OB_CANDIDATE}/lib"
          break
        fi
      done
    fi
  fi
fi

# Fallback: pkg-config (most reliable on Linux distros)
if [ -z "${OBABEL_INCLUDE}" ] && command -v pkg-config >/dev/null 2>&1; then
  OB_PC_INC="$(pkg-config --variable=includedir openbabel-3 2>/dev/null || true)"
  if [ -n "${OB_PC_INC}" ] && [ -d "${OB_PC_INC}" ]; then
    OBABEL_INCLUDE="${OB_PC_INC}"
    OBABEL_LIB_DIR="$(pkg-config --variable=libdir openbabel-3 2>/dev/null || true)"
  fi
fi

# Well-known system paths fallback
if [ -z "${OBABEL_INCLUDE}" ] && [ -d "/opt/homebrew/include/openbabel3" ]; then
  OBABEL_INCLUDE="/opt/homebrew/include/openbabel3"
  OBABEL_LIB_DIR="/opt/homebrew/lib"
fi
if [ -z "${OBABEL_INCLUDE}" ] && [ -d "/usr/include/openbabel3" ]; then
  OBABEL_INCLUDE="/usr/include/openbabel3"
  OBABEL_LIB_DIR="/usr/lib"
fi
# Ubuntu: libopenbabel-dev may install to /usr/include (openbabel3 is a subdirectory)
if [ -z "${OBABEL_INCLUDE}" ] && [ -d "/usr/include" ] && [ -f "/usr/include/openbabel3/openbabel/mol.h" ]; then
  OBABEL_INCLUDE="/usr/include/openbabel3"
  OBABEL_LIB_DIR="/usr/lib"
fi

[ -n "${OBABEL_INCLUDE}" ] || fail "could not find OpenBabel include directory"
# Verify the include dir actually has the expected headers
[ -f "${OBABEL_INCLUDE}/openbabel/mol.h" ] || \
  fail "OpenBabel include dir '${OBABEL_INCLUDE}' does not contain openbabel/mol.h — check install"
echo "openbabel-smoke: OB include = ${OBABEL_INCLUDE}"

# ----------------------------------------------------------------------------
# 3. Gather BALL include/lib paths from the build tree.
# ----------------------------------------------------------------------------
BALL_INCLUDE="${REPO_ROOT}/include"
BALL_GEN_INCLUDE="${BUILD_DIR}/include"       # config.h is generated here
BALL_LIB_DIR="$(dirname "${LIBBALL}")"        # dir containing libBALL (may be lib/ subdir)

# Check config.h exists and BALL_HAS_OPENBABEL is defined
CONFIG_H="${BALL_GEN_INCLUDE}/BALL/CONFIG/config.h"
[ -f "${CONFIG_H}" ] || fail "config.h not found at ${CONFIG_H} — run cmake --preset first"
grep -q "BALL_HAS_OPENBABEL" "${CONFIG_H}" || \
  fail "BALL_HAS_OPENBABEL is not defined in ${CONFIG_H} — configure with BALL_LICENSE=GPL"

# Discover Boost include path from CMakeCache or well-known paths
BOOST_INCLUDE=""
if [ -f "${CACHE_FILE}" ]; then
  BOOST_INCLUDE="$(grep -m1 '^Boost_INCLUDE_DIR' "${CACHE_FILE}" 2>/dev/null | cut -d= -f2 | tr -d '[:space:]' || true)"
fi
if [ -z "${BOOST_INCLUDE}" ] && [ -d "/opt/homebrew/include" ]; then
  BOOST_INCLUDE="/opt/homebrew/include"
elif [ -z "${BOOST_INCLUDE}" ] && [ -d "/usr/include" ]; then
  BOOST_INCLUDE="/usr/include"
fi

BOOST_INC_FLAG="${BOOST_INCLUDE:+-I${BOOST_INCLUDE}}"

# ----------------------------------------------------------------------------
# 4. Compile the smoke driver.
# ----------------------------------------------------------------------------
SMOKE_SRC="${SCRIPT_DIR}/openbabel-smoke.C"
SMOKE_BIN="$(mktemp /tmp/openbabel-smoke.XXXXXX)"

# Determine the compiler — prefer clang++, fall back to g++
CXX="${CXX:-$(command -v clang++ || command -v g++ || echo "")}"
[ -n "${CXX}" ] || fail "no C++ compiler found (set CXX env var)"

echo "openbabel-smoke: compiler  = ${CXX}"
echo "openbabel-smoke: compiling ${SMOKE_SRC}"

# Build cflags to re-enable std::binary_function (removed in C++17 — needed by
# OpenBabel 3.1.1's plugin.h on macOS libc++ and Linux libstdc++)
COMPAT_DEFS=""
case "$(uname -s)" in
  Darwin) COMPAT_DEFS="-D_LIBCPP_ENABLE_CXX17_REMOVED_UNARY_BINARY_FUNCTION" ;;
  Linux)  COMPAT_DEFS="-D_GLIBCXX_USE_DEPRECATED=1" ;;
esac

"${CXX}" -std=c++17 \
  -I"${BALL_INCLUDE}" \
  -I"${BALL_GEN_INCLUDE}" \
  -I"${OBABEL_INCLUDE}" \
  ${BOOST_INC_FLAG} \
  ${COMPAT_DEFS} \
  -Wno-deprecated-declarations \
  "${SMOKE_SRC}" \
  "${LIBBALL}" \
  ${OBABEL_LIB_DIR:+-L"${OBABEL_LIB_DIR}"} -lopenbabel \
  -o "${SMOKE_BIN}" \
  2>&1 || fail "compilation of ${SMOKE_SRC} failed"

echo "openbabel-smoke: compiled  = ${SMOKE_BIN}"

# ----------------------------------------------------------------------------
# 5. Set up runtime environment and run the driver.
# ----------------------------------------------------------------------------
export BALL_DATA_PATH="${BALL_DATA_PATH:-${REPO_ROOT}/data}"

# Help the dynamic linker find libBALL + libopenbabel
if [ "$(uname -s)" = "Darwin" ]; then
  export DYLD_LIBRARY_PATH="${BALL_LIB_DIR}${DYLD_LIBRARY_PATH:+:${DYLD_LIBRARY_PATH}}"
  [ -n "${OBABEL_LIB_DIR}" ] && export DYLD_LIBRARY_PATH="${OBABEL_LIB_DIR}:${DYLD_LIBRARY_PATH}"
else
  export LD_LIBRARY_PATH="${BALL_LIB_DIR}${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}"
  [ -n "${OBABEL_LIB_DIR}" ] && export LD_LIBRARY_PATH="${OBABEL_LIB_DIR}:${LD_LIBRARY_PATH}"
fi

echo "openbabel-smoke: running smoke check..."
"${SMOKE_BIN}"
STATUS=$?

rm -f "${SMOKE_BIN}"

if [ "${STATUS}" -ne 0 ]; then
  fail "smoke driver exited with status ${STATUS}"
fi

exit 0
