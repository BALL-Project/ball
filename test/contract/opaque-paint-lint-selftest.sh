#!/usr/bin/env bash
# opaque-paint-lint-selftest.sh — Phase 999.62 close-out (v1.7.4)
#
# Synthetic self-test for cmake/scripts/check-opaque-paint.sh: proves the lint
# (a) passes a compliant fixture, (b) FAILS a height-animating fixture with no
# opaque marker, and (c) passes once that fixture is whitelisted. Guards the
# lint against silently rotting into a vacuous always-green check.
#
# Exit: 0 = all three self-test cases behaved as expected; 1 otherwise.

set -uo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
LINT="$ROOT/cmake/scripts/check-opaque-paint.sh"
tmp="$(mktemp -d)"
trap 'rm -rf "$tmp"' EXIT

rc=0

# Case A — compliant: animates height AND declares opaque marker → expect PASS.
cat > "$tmp/Compliant.C" <<'EOF'
void Compliant::setup() {
  setAttribute(Qt::WA_OpaquePaintEvent);
  setProperty("opaqueContainer", true);
  anim->setPropertyName("maximumHeight");
}
EOF
if bash "$LINT" "$tmp" >/dev/null 2>&1; then echo "A ok (compliant passes)"; else echo "A FAIL: compliant fixture rejected"; rc=1; fi

# Case B — violation: animates height, NO opaque marker → expect FAIL.
cat > "$tmp/Bad.C" <<'EOF'
void Bad::setup() {
  anim->setPropertyName("maximumHeight");
  child->setMaximumHeight(0);
}
EOF
if bash "$LINT" "$tmp" >/dev/null 2>&1; then echo "B FAIL: violation not caught"; rc=1; else echo "B ok (violation caught)"; fi

# Case C — whitelisted: same Bad.C but basename whitelisted → expect PASS.
# (Temporarily append to the real whitelist, then restore.)
WL="$ROOT/cmake/scripts/opaque-paint-whitelist.txt"
cp "$WL" "$tmp/wl.bak"
echo "Bad.C   # selftest temporary exemption" >> "$WL"
if bash "$LINT" "$tmp" >/dev/null 2>&1; then echo "C ok (whitelist exempts)"; else echo "C FAIL: whitelist not honored"; rc=1; fi
cp "$tmp/wl.bak" "$WL"

[ $rc -eq 0 ] && echo "SELFTEST OK" || echo "SELFTEST FAILED"
exit $rc
