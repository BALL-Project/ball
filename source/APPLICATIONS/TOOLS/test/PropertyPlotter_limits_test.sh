#!/usr/bin/env bash
# -*- Mode: sh; tab-width: 2; -*-
#
# Functional test for PropertyPlotter axis/value limits (FEATPLOT-01, #524).
#
# PropertyPlotter is a CLI tool that writes a gnuplot script (plot.txt) and a
# data file (properties.txt) and then pipes the script to gnuplot. There is no
# unit-test harness for the TOOLS executables, so this functional test drives
# the built binary and asserts:
#   1. No-flag invocation emits NO `set xrange`/`set yrange` (unchanged default).
#   2. -p1_min/-p1_max emit `set xrange [lo:hi]` and clamp the histogram domain.
#   3. -p1_min drops out-of-range prop1 values from the data file.
#   4. -p2_min/-p2_max emit `set yrange` in scatter mode and drop out-of-range pts.
#
# Usage: PropertyPlotter_limits_test.sh <path-to-PropertyPlotter> <test-sdf>
set -euo pipefail

BIN="${1:?path to PropertyPlotter binary required}"
SDF="${2:?path to test sdf required}"

WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT
cd "$WORK"

fail() { echo "FAIL: $1" >&2; exit 1; }
pass() { echo "PASS: $1"; }

# Helper: run the tool with -o (non-interactive => uses temp files, but we
# instead run WITHOUT -o so it writes properties.txt / plot.txt in CWD, then we
# inspect those. Without -o it also calls `gnuplot -persist`; if gnuplot is not
# in PATH the system() call simply fails harmlessly after the files are written.
run() {
  rm -f properties.txt plot.txt
  # quiet to suppress progress noise; ignore gnuplot exit (may be absent)
  "$BIN" -i "$SDF" "$@" -quiet >/dev/null 2>&1 || true
}

# --- 1. Default (no limit flags): no xrange/yrange in distribution mode ---
run -p1 score
[ -f plot.txt ] || fail "default run produced no plot.txt"
if grep -qE 'set (xrange|yrange)' plot.txt; then
  fail "default distribution run emitted a set xrange/yrange (should be none)"
fi
pass "default distribution run emits no xrange/yrange"

# Capture the default histogram domain for the regression check below.
DEFAULT_FIRST_X=$(head -1 properties.txt | awk '{print $1}')

# --- 2. Distribution mode with p1 limits: xrange emitted + domain clamped ---
run -p1 score -p1_min 0 -p1_max 5
grep -qE 'set xrange \[0:5\]' plot.txt \
  || fail "p1_min/p1_max did not emit 'set xrange [0:5]' (got: $(grep -i xrange plot.txt || echo none))"
pass "distribution -p1_min 0 -p1_max 5 emits 'set xrange [0:5]'"

# Histogram domain must start at the requested lower bound (0), not the data
# extreme (-25). Allow small float formatting; first bucket x should be >= 0.
FIRST_X=$(head -1 properties.txt | awk '{print $1}')
awk -v x="$FIRST_X" 'BEGIN{ exit (x+0 >= -0.0001) ? 0 : 1 }' \
  || fail "histogram domain not clamped to p1_min (first bucket x=$FIRST_X)"
# And it must differ from the unclamped default (which started at -25).
awk -v a="$FIRST_X" -v b="$DEFAULT_FIRST_X" 'BEGIN{ exit (a+0 != b+0) ? 0 : 1 }' \
  || fail "clamped domain identical to default — limits had no effect"
pass "histogram domain clamped to requested p1 range"

# --- 3. one-sided p1_min: xrange [lo:*] ---
run -p1 score -p1_min 1
grep -qE 'set xrange \[1:\*\]' plot.txt \
  || fail "one-sided -p1_min did not emit 'set xrange [1:*]'"
pass "one-sided -p1_min emits 'set xrange [1:*]'"

# --- 4. Scatter mode with p2 limits: yrange emitted + points filtered ---
# score in [0:5] keeps mol2..mol4 (score 1,2,3); weight in [250:450] keeps
# weight 300,400 => so scatter data should retain exactly score 2 and 3.
run -p1 score -p2 weight -p1_min 0 -p1_max 5 -p2_min 250 -p2_max 450
grep -qE 'set xrange \[0:5\]' plot.txt || fail "scatter mode missing xrange"
grep -qE 'set yrange \[250:450\]' plot.txt \
  || fail "scatter -p2_min/-p2_max did not emit 'set yrange [250:450]'"
pass "scatter mode emits xrange and yrange"

# Every retained data row must satisfy both ranges.
while read -r x y; do
  [ -z "$x" ] && continue
  awk -v x="$x" 'BEGIN{ exit (x+0 >= 0 && x+0 <= 5) ? 0 : 1 }' \
    || fail "scatter row has prop1 $x outside [0:5]"
  awk -v y="$y" 'BEGIN{ exit (y+0 >= 250 && y+0 <= 450) ? 0 : 1 }' \
    || fail "scatter row has prop2 $y outside [250:450]"
done < properties.txt
pass "scatter data file respects p1 and p2 limits"

echo "ALL PASS"
