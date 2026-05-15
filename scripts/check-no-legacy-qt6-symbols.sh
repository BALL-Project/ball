#!/usr/bin/env bash
#
# check-no-legacy-qt6-symbols.sh
#
# Phase 5 (Qt 6 Migration) regression gate.
#
# Scans source/ and include/BALL/ for Qt 5-only symbols that the Qt 6 migration
# (Plans 05-01 / 05-02 / 05-03) must eliminate. Intended to run as a CI lint
# step on every push so any future re-introduction of a Qt 5 footgun fails the
# build before it can reach `master`.
#
# Modeled verbatim on `.planning/phases/02-rendering-port-4a/scripts/
# check-no-legacy-gl-symbols.sh` (the Phase 2 grep gate) — same shebang, same
# `set -u`, same `REPO_ROOT` resolution, same `grep -rEn ... | grep -vE` comment
# filter pipeline, same exit-0/exit-1 messaging. See 05-PATTERNS.md
# §`scripts/check-no-legacy-qt6-symbols.sh` for the canonical structure.
#
# Exit status:
#   0  - zero legacy Qt 5 symbol references found
#   1  - one or more legacy Qt 5 symbol references found
#   2  - REPO_ROOT does not contain source/ + include/BALL/
#
# Usage:
#   check-no-legacy-qt6-symbols.sh [REPO_ROOT]
#
# REPO_ROOT defaults to the repository root inferred from this script's location
# (scripts/ -> one level up).

set -u

# --- Resolve repo root -------------------------------------------------------
if [[ $# -ge 1 && -n "${1:-}" ]]; then
  REPO_ROOT="$1"
else
  SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
  # scripts/ -> <repo root>
  REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
fi

if [[ ! -d "${REPO_ROOT}/source" || ! -d "${REPO_ROOT}/include/BALL" ]]; then
  echo "ERROR: could not locate source/ and include/BALL/ under REPO_ROOT='${REPO_ROOT}'" >&2
  echo "       pass the repository root as the first argument." >&2
  exit 2
fi

# --- Legacy Qt 5 symbol set --------------------------------------------------
# These all map to hard Qt 6 compile errors or silently-broken behaviour:
#   QRegExp                  -> removed; use QRegularExpression
#   QDesktopWidget           -> removed; use QScreen / QGuiApplication::screens()
#   QApplication::desktop    -> removed; same replacement
#   QString::SkipEmptyParts  -> moved to Qt:: namespace (Qt::SkipEmptyParts)
#   QtWidgets/QOpenGLWidget  -> moved to QtOpenGLWidgets/QOpenGLWidget
#   QtWidgets/QDesktopWidget -> include path gone (header removed)
#   Qt5::                    -> Qt 6 uses Qt6:: target prefix in CMake (catches
#                               stray Qt5:: references in build files that
#                               happen to live under source/ or include/BALL/)
LEGACY_PATTERN='QRegExp|QDesktopWidget|QApplication::desktop|QString::SkipEmptyParts|QtWidgets/QOpenGLWidget|QtWidgets/QDesktopWidget|Qt5::'

SCAN_DIRS=(
  "${REPO_ROOT}/source"
  "${REPO_ROOT}/include/BALL"
)

# --- Scan --------------------------------------------------------------------
# Match the legacy symbols, then drop hits that occur on comment-only lines
# (`file:N:<ws>//...`, `file:N:<ws>*...`, `file:N:<ws>/*...`). Historical
# comments / Doxygen blocks that mention "QRegExp" while explaining the port
# are not code and must not fail the gate — only real code references should.
HITS="$(grep -rEn "${LEGACY_PATTERN}" "${SCAN_DIRS[@]}" 2>/dev/null \
  | grep -vE ':[0-9]+:[[:space:]]*(//|\*|/\*)' || true)"

if [[ -n "${HITS}" ]]; then
  echo "${HITS}"
  COUNT="$(printf '%s\n' "${HITS}" | grep -c '')"
  echo ""
  echo "${COUNT} legacy Qt 5 symbol reference(s) remain in code"
  exit 1
fi

echo "0 legacy Qt 5 symbol references remain in code"
exit 0
