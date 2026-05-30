#!/usr/bin/env bash
#
# check-no-legacy-gl-symbols.sh
#
# Wave 0 verification gate for Phase 2 (Rendering Port 4a).
#
# Scans source/VIEW and include/BALL/VIEW for legacy Qt OpenGL symbols that the
# QGLWidget -> QOpenGLWidget port (RENDER-01/03/07) must eliminate. Intended to
# be run after every later-wave task as a regression gate, and eventually as a
# CI lint step.
#
# Exit status:
#   0  - zero legacy symbol references found (port complete / no regression)
#   1  - one or more legacy symbol references found (port incomplete / regressed)
#
# Usage:
#   check-no-legacy-gl-symbols.sh [REPO_ROOT]
#
# REPO_ROOT defaults to the repository root inferred from this script's location
# (.planning/phases/02-rendering-port-4a/scripts/ -> five levels up).
#
# NOTE: On the current pre-port tree this script is EXPECTED to exit 1 — the
# legacy symbols still exist because the port has not happened yet.

set -u

# --- Resolve repo root -------------------------------------------------------
if [[ $# -ge 1 && -n "${1:-}" ]]; then
  REPO_ROOT="$1"
else
  SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
  # scripts/ -> 02-rendering-port-4a/ -> phases/ -> .planning/ -> <repo root>
  REPO_ROOT="$(cd "${SCRIPT_DIR}/../../../.." && pwd)"
fi

if [[ ! -d "${REPO_ROOT}/source/VIEW" || ! -d "${REPO_ROOT}/include/BALL/VIEW" ]]; then
  echo "ERROR: could not locate source/VIEW and include/BALL/VIEW under REPO_ROOT='${REPO_ROOT}'" >&2
  echo "       pass the repository root as the first argument." >&2
  exit 2
fi

# --- Legacy symbol set -------------------------------------------------------
# QGLWidget, QGLFormat, QGLContext, QGL:: (enum namespace), QGLPixelBuffer,
# QtOpenGL/qgl (the legacy header), and the manual buffer-swap / framebuffer
# helpers that QOpenGLWidget makes obsolete.
LEGACY_PATTERN='QGLWidget|QGLFormat|QGLContext|QGL::|QGLPixelBuffer|QtOpenGL/qgl|swapBuffers|setAutoBufferSwap|safeBufferSwap|grabFrameBuffer|convertToGLFormat'

SCAN_DIRS=(
  "${REPO_ROOT}/source/VIEW"
  "${REPO_ROOT}/include/BALL/VIEW"
)

# --- Scan --------------------------------------------------------------------
# Match the legacy symbols, then drop hits that occur on comment-only lines
# (`file:N:<ws>//...`, `file:N:<ws>*...`, `file:N:<ws>/*...`). Historical
# comments that mention "QGLWidget" while explaining the port are not code and
# must not fail the gate — only real code references should.
HITS="$(grep -rEn "${LEGACY_PATTERN}" "${SCAN_DIRS[@]}" 2>/dev/null \
  | grep -vE ':[0-9]+:[[:space:]]*(//|\*|/\*)' || true)"

if [[ -n "${HITS}" ]]; then
  echo "${HITS}"
  COUNT="$(printf '%s\n' "${HITS}" | grep -c '')"
  echo ""
  echo "${COUNT} legacy Qt GL symbol reference(s) remain in code"
  exit 1
fi

echo "0 legacy Qt GL symbol references remain in code"
exit 0
