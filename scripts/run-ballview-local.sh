#!/usr/bin/env bash
#
# run-ballview-local.sh — Launch the locally-built BALLView.app with the
# environment it needs at runtime (BALL_DATA_PATH, BALLVIEW_DATA_PATH,
# DYLD_FALLBACK_LIBRARY_PATH for libVIEW/libBALL).
#
# Usage:
#   /Users/kohlbach/Claude/BALL/ball/scripts/run-ballview-local.sh
#
# Or open in Finder by running:
#   open /Users/kohlbach/Claude/BALL/ball/build/bin/BALLView.app
#
# Either form works AFTER the env is set; this script just makes sure
# you don't need to remember the env vars.
#
# Logs go to /tmp/ballview-local.log so you can grep crash markers
# (BALLVIEW_GL_DIAG, EXC_BAD_ACCESS, renderSphere_, etc.) after the run.

set -euo pipefail

REPO_ROOT="/Users/kohlbach/Claude/BALL/ball"
APP_BUNDLE="${REPO_ROOT}/build/bin/BALLView.app"
APP_BINARY="${APP_BUNDLE}/Contents/MacOS/BALLView"
LOG_FILE="/tmp/ballview-local.log"

if [ ! -x "${APP_BINARY}" ]; then
  echo "ERROR: BALLView binary not found at ${APP_BINARY}" >&2
  echo "Build it first:" >&2
  echo "  cd ${REPO_ROOT}/build && cmake --build . --target BALLView -j 8" >&2
  exit 1
fi

# Required at runtime — BALL looks up molecular data + sample structures
# via these.
export BALL_DATA_PATH="${REPO_ROOT}/data"
export BALLVIEW_DATA_PATH="${REPO_ROOT}/data"

# Required so the .app bundle resolves libVIEW.dylib + libBALL.dylib
# from the build tree instead of any system-installed copy.
export DYLD_FALLBACK_LIBRARY_PATH="${REPO_ROOT}/build/lib${DYLD_FALLBACK_LIBRARY_PATH:+:${DYLD_FALLBACK_LIBRARY_PATH}}"

echo "[run-ballview-local] BALL_DATA_PATH=${BALL_DATA_PATH}"
echo "[run-ballview-local] BALLVIEW_DATA_PATH=${BALLVIEW_DATA_PATH}"
echo "[run-ballview-local] DYLD_FALLBACK_LIBRARY_PATH=${DYLD_FALLBACK_LIBRARY_PATH}"
echo "[run-ballview-local] Launching ${APP_BINARY}"
echo "[run-ballview-local] Log file: ${LOG_FILE}"
echo "[run-ballview-local] (close BALLView or Ctrl-C this terminal to stop)"

# Run in foreground so terminal Ctrl-C stops the app cleanly + you see
# stdout/stderr live. Tee to log for post-mortem grep.
exec "${APP_BINARY}" 2>&1 | tee "${LOG_FILE}"
