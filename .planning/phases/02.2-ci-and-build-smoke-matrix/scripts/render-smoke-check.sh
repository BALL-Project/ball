#!/usr/bin/env bash
#
# render-smoke-check.sh — BALLView headless render smoke check (Phase 02.2)
#
# Launches a freshly built BALLView headless, loads a known molecule, drives
# BALL's existing Scene::exportPNG() path (via the minimal `-export-png` CLI
# flag added to main.C), then asserts that:
#
#   1. the PNG file was actually written,
#   2. the PNG is NON-BLANK (a real molecule render is not a single flat color),
#   3. BALLView emitted the DIAG-01 `BALLVIEW_GL_DIAG` line to stdout — i.e. a
#      real GL context was created. Its absence is exactly the Phase 2
#      "renders blank because there is no GL context" bug class, so a missing
#      diag line is a hard failure (D-05: the diag line is the smoke check's
#      oracle).
#
# Exit 0 on a good render; non-zero (with a clear diagnostic) on any failure.
# On failure the PNG + captured log are left in place and their paths printed
# so Plan 02's CI workflow can upload them as build artifacts.
#
# Usage:
#   render-smoke-check.sh [path-to-BALLView-executable]
#
# Environment:
#   BALL_DATA_PATH / BALLVIEW_DATA_PATH
#       Data-path env vars BALLView needs at runtime. If unset, both default
#       to "<build>/../data" per BUILD-macos.md.
#   BALLVIEW_SMOKE_RUNNER
#       Optional command prefix the executable is launched under. Empty on
#       macOS; on Linux CI (Plan 02) this is set to "xvfb-run -a" (and Plan 02
#       also exports LIBGL_ALWAYS_SOFTWARE=1 per D-08). This script just
#       respects the prefix — it does not hardcode xvfb.
#   BALLVIEW_SMOKE_INPUT
#       Optional path to the molecule file to render. Defaults to a fixed
#       known PDB shipped in the source tree (data/structures/bpti.pdb).
#   BALLVIEW_SMOKE_TIMEOUT
#       Seconds to wait for the headless export before giving up. Default 60.
#
set -euo pipefail

# ----------------------------------------------------------------------------
# Locate the repo root (this script lives in
# .planning/phases/02.2-ci-and-build-smoke-matrix/scripts/).
# ----------------------------------------------------------------------------
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/../../../.." && pwd)"

fail() {
	echo "SMOKE-CHECK FAIL: $*" >&2
	exit 1
}

# ----------------------------------------------------------------------------
# 1. Resolve the BALLView executable + build directory.
# ----------------------------------------------------------------------------
BALLVIEW_BIN="${1:-}"
if [ -z "${BALLVIEW_BIN}" ]; then
	# Autodetect: macOS .app bundle layout first, then a plain Linux/Windows binary.
	for candidate in \
		"${REPO_ROOT}/build/bin/BALLView.app/Contents/MacOS/BALLView" \
		"${REPO_ROOT}/build/bin/BALLView" \
		"${REPO_ROOT}/build/BALLView" \
		"${REPO_ROOT}/build/bin/BALLView.exe"
	do
		if [ -x "${candidate}" ]; then
			BALLVIEW_BIN="${candidate}"
			break
		fi
	done
fi

[ -n "${BALLVIEW_BIN}" ] || fail "could not locate a BALLView executable (pass one as \$1)"
[ -x "${BALLVIEW_BIN}" ] || fail "BALLView executable not found or not executable: ${BALLVIEW_BIN}"

# Derive the build directory from the executable path. For the macOS .app
# bundle the build dir is four levels up from the inner MacOS/ binary; for a
# plain binary it is simply the containing directory (or its parent if the
# binary sits in build/bin/).
case "${BALLVIEW_BIN}" in
	*/BALLView.app/Contents/MacOS/BALLView)
		BUILD_DIR="$(cd "$(dirname "${BALLVIEW_BIN}")/../../../.." && pwd)"
		;;
	*/bin/BALLView|*/bin/BALLView.exe)
		BUILD_DIR="$(cd "$(dirname "${BALLVIEW_BIN}")/.." && pwd)"
		;;
	*)
		BUILD_DIR="$(cd "$(dirname "${BALLVIEW_BIN}")" && pwd)"
		;;
esac

# ----------------------------------------------------------------------------
# 2. Runtime environment — data paths and the dynamic-linker path.
# ----------------------------------------------------------------------------
DEFAULT_DATA_PATH="${REPO_ROOT}/data"
export BALL_DATA_PATH="${BALL_DATA_PATH:-${DEFAULT_DATA_PATH}}"
export BALLVIEW_DATA_PATH="${BALLVIEW_DATA_PATH:-${DEFAULT_DATA_PATH}}"

# Help the dynamic linker find the freshly built libBALL / libVIEW.
if [ -d "${BUILD_DIR}/lib" ]; then
	export DYLD_LIBRARY_PATH="${BUILD_DIR}/lib${DYLD_LIBRARY_PATH:+:${DYLD_LIBRARY_PATH}}"
	export LD_LIBRARY_PATH="${BUILD_DIR}/lib${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}"
fi

SMOKE_RUNNER="${BALLVIEW_SMOKE_RUNNER:-}"
SMOKE_INPUT="${BALLVIEW_SMOKE_INPUT:-${REPO_ROOT}/data/structures/bpti.pdb}"
SMOKE_TIMEOUT="${BALLVIEW_SMOKE_TIMEOUT:-60}"

[ -f "${SMOKE_INPUT}" ] || fail "smoke-check input molecule not found: ${SMOKE_INPUT}"

# ----------------------------------------------------------------------------
# 3. Output locations (temp dir, kept on failure for CI artifact upload).
# ----------------------------------------------------------------------------
WORK_DIR="$(mktemp -d "${TMPDIR:-/tmp}/ballview-smoke.XXXXXX")"
PNG_PATH="${WORK_DIR}/render-smoke.png"
LOG_PATH="${WORK_DIR}/ballview-smoke.log"

echo "render-smoke-check: executable = ${BALLVIEW_BIN}"
echo "render-smoke-check: build dir  = ${BUILD_DIR}"
echo "render-smoke-check: input mol  = ${SMOKE_INPUT}"
echo "render-smoke-check: png        = ${PNG_PATH}"
echo "render-smoke-check: log        = ${LOG_PATH}"
[ -n "${SMOKE_RUNNER}" ] && echo "render-smoke-check: runner     = ${SMOKE_RUNNER}"

# ----------------------------------------------------------------------------
# 4. Launch BALLView headless and trigger the existing exportPNG() path via the
#    `-export-png` flag. Bound the run with a watchdog so a hung GUI cannot
#    wedge CI; the `-export-png` handler quits BALLView itself on success.
# ----------------------------------------------------------------------------
# v1.7.0-rc3 R8 workaround: prevent the WelcomeScreen-as-startup-
# central-widget swap (UFG-08) from keeping Scene's QOpenGLWidget
# hidden, which prevents initializeGL() from firing and starves the
# smoke check of the BALLVIEW_GL_DIAG oracle line. Mainframe's
# showWelcomeScreen_() honors this env var as an early-return.
# No effect on interactive runs (env var only set here).
export BALLVIEW_NO_WELCOME=1

# shellcheck disable=SC2086
${SMOKE_RUNNER} "${BALLVIEW_BIN}" -export-png "${PNG_PATH}" "${SMOKE_INPUT}" \
	> "${LOG_PATH}" 2>&1 &
BALLVIEW_PID=$!

watchdog() {
	local waited=0
	while kill -0 "${BALLVIEW_PID}" 2>/dev/null; do
		if [ "${waited}" -ge "${SMOKE_TIMEOUT}" ]; then
			echo "render-smoke-check: timeout (${SMOKE_TIMEOUT}s) — killing BALLView" >&2
			kill "${BALLVIEW_PID}" 2>/dev/null || true
			return
		fi
		sleep 1
		waited=$((waited + 1))
	done
}
watchdog

# Reap the process; tolerate a non-zero status (we judge success by the
# artifacts + diag line below, not by BALLView's exit code, since the watchdog
# may have killed it).
wait "${BALLVIEW_PID}" 2>/dev/null || true

# ----------------------------------------------------------------------------
# 5. Assert: the DIAG-01 GL-capability line was emitted (the oracle, D-05).
# ----------------------------------------------------------------------------
DIAG_LINE="$(grep -m1 'BALLVIEW_GL_DIAG' "${LOG_PATH}" || true)"
if [ -z "${DIAG_LINE}" ]; then
	echo "----- captured BALLView log -----" >&2
	cat "${LOG_PATH}" >&2 || true
	echo "---------------------------------" >&2
	fail "no BALLVIEW_GL_DIAG line in BALLView stdout — no GL context was created.
       PNG: ${PNG_PATH}
       LOG: ${LOG_PATH}"
fi
echo "render-smoke-check: diag line  = ${DIAG_LINE}"

# Sanity-check the diag line carries a real GL context (non-empty version, a
# recognised profile). This is the D-05 cross-check: it proves the render path
# had a live context rather than a stub.
case "${DIAG_LINE}" in
	*'gl_version=""'*|*'gl_version="unknown"'*)
		fail "BALLVIEW_GL_DIAG reports no usable gl_version — GL context is not live.
       LOG: ${LOG_PATH}" ;;
esac
case "${DIAG_LINE}" in
	*'renderer_backend=GL'*) : ;;
	*) fail "BALLVIEW_GL_DIAG missing expected renderer_backend=GL field.
       LOG: ${LOG_PATH}" ;;
esac

# ----------------------------------------------------------------------------
# 6. Assert: the PNG exists and is non-blank, and report its dimensions.
#    Uses python3's standard library only (struct + zlib) — no PIL, no
#    ImageMagick — so the script has zero extra dependencies on every runner.
#    "Non-blank" = the fully-decompressed image data contains more than a
#    handful of distinct byte values; a blank/flat render collapses to ~1-2.
# ----------------------------------------------------------------------------
[ -f "${PNG_PATH}" ] || fail "no PNG was produced at ${PNG_PATH}
       LOG: ${LOG_PATH}"

command -v python3 >/dev/null 2>&1 || fail "python3 is required for the non-blank PNG check"

PNG_INFO="$(
	PNG_PATH="${PNG_PATH}" python3 - <<'PYEOF'
import os, struct, zlib, sys

path = os.environ["PNG_PATH"]
data = open(path, "rb").read()

if data[:8] != b"\x89PNG\r\n\x1a\n":
    print("ERROR not-a-png")
    sys.exit(0)

# IHDR is always the first chunk: width/height are the 8 bytes at offset 16.
width, height = struct.unpack(">II", data[16:24])

# Concatenate every IDAT chunk, then inflate to raw (filtered) scanlines.
idat = bytearray()
i = 8
while i + 12 <= len(data):
    (length,) = struct.unpack(">I", data[i:i + 4])
    ctype = data[i + 4:i + 8]
    if ctype == b"IDAT":
        idat += data[i + 8:i + 8 + length]
    i += 12 + length
    if ctype == b"IEND":
        break

try:
    raw = zlib.decompress(bytes(idat))
except Exception as exc:  # noqa: BLE001
    print("ERROR bad-idat %s" % exc)
    sys.exit(0)

distinct = len(set(raw))
print("OK %d %d %d" % (width, height, distinct))
PYEOF
)"

set -- ${PNG_INFO}
STATUS="${1:-ERROR}"

if [ "${STATUS}" != "OK" ]; then
	fail "PNG could not be parsed (${PNG_INFO}).
       PNG: ${PNG_PATH}
       LOG: ${LOG_PATH}"
fi

PNG_WIDTH="${2}"
PNG_HEIGHT="${3}"
PNG_DISTINCT="${4}"

# Expected viewport: a real rendered scene must have a sane, non-degenerate
# size. (The DIAG-01 fbo_size field is captured at initializeGL() time, before
# the widget is laid out, so it is not an exact match for the final export
# dimensions — the diag line's job here is to prove a context exists, see
# step 5. The dimension assertion below guards against a degenerate viewport.)
if [ "${PNG_WIDTH}" -lt 64 ] || [ "${PNG_HEIGHT}" -lt 64 ]; then
	fail "rendered PNG has a degenerate viewport size ${PNG_WIDTH}x${PNG_HEIGHT}.
       PNG: ${PNG_PATH}
       LOG: ${LOG_PATH}"
fi

# Non-blank: a flat single-color image decompresses to ~1-2 distinct byte
# values. A genuine molecule render has many. Require a comfortable margin.
if [ "${PNG_DISTINCT}" -lt 16 ]; then
	fail "rendered PNG appears BLANK (only ${PNG_DISTINCT} distinct byte values).
       PNG: ${PNG_PATH}
       LOG: ${LOG_PATH}"
fi

# ----------------------------------------------------------------------------
# 7. All assertions passed.
# ----------------------------------------------------------------------------
echo "SMOKE-CHECK PASS: non-blank PNG ${PNG_WIDTH}x${PNG_HEIGHT}" \
     "(${PNG_DISTINCT} distinct byte values), DIAG-01 line present."

# Clean up the temp dir only on success; on failure (handled by fail()) it is
# deliberately left in place for CI artifact upload.
rm -rf "${WORK_DIR}"
exit 0
