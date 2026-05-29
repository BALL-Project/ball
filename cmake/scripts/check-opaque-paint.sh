#!/usr/bin/env bash
# check-opaque-paint.sh
#
# Phase 999.62 close-out (v1.7.4) — opaque-paint CI lint.
#
# Enforces the ARCHITECTURE-CONTRACT.md §10 opaque-paint contract (the
# UFG-05/09/10 lesson): any container widget that ANIMATES a child's height
# (setMaximumHeight / QPropertyAnimation on "maximumHeight" — the collapsible
# Inspector-section pattern) MUST declare itself opaque, otherwise the parent
# shows through during the animation as flicker / smear. "Declares itself
# opaque" = sets WA_OpaquePaintEvent, OR setAutoFillBackground(true), OR the
# opaqueContainer dynamic property (the themeable token, see THEME.md /
# theme-neutral.qss).
#
# This is the lightweight grep-class guard shipped in v1.7.4. A precise
# clang-AST check is deferred to v1.7.5. Files with a legitimate reason to
# animate height without an opaque marker are listed in
# cmake/scripts/opaque-paint-whitelist.txt (with a rationale comment).
#
# Usage:   cmake/scripts/check-opaque-paint.sh [DIR ...]
# Exit:    0 = clean, 1 = at least one violation (prints file), 2 = usage error.

set -uo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
WHITELIST="$ROOT/cmake/scripts/opaque-paint-whitelist.txt"
roots=("$@")
[ ${#roots[@]} -eq 0 ] && roots=("$ROOT/source/VIEW/WIDGETS")

# Load whitelist basenames (ignore blank lines + comments).
declare -A wl=()
if [ -f "$WHITELIST" ]; then
	while IFS= read -r line; do
		line="${line%%#*}"; line="${line// /}"
		[ -n "$line" ] && wl["$line"]=1
	done < "$WHITELIST"
fi

fail=0
# Animates a child's maximum height → the flicker-prone pattern.
anim_re='setMaximumHeight|"maximumHeight"|maximumHeight"'
# Declares itself opaque.
opaque_re='WA_OpaquePaintEvent|setAutoFillBackground|opaqueContainer'

while IFS= read -r f; do
	# only count real animation (a QPropertyAnimation/assignment), not a comment
	if grep -Eq "$anim_re" "$f" 2>/dev/null; then
		if ! grep -Eq "$opaque_re" "$f" 2>/dev/null; then
			base="$(basename "$f")"
			if [ -n "${wl[$base]:-}" ]; then
				continue
			fi
			echo "VIOLATION: $f animates child height but sets no opaque-paint marker"
			echo "  → add WA_OpaquePaintEvent + setAutoFillBackground(true) (and the"
			echo "    opaqueContainer dynamic property), or whitelist with a rationale in"
			echo "    cmake/scripts/opaque-paint-whitelist.txt. See THEME.md / §10."
			fail=1
		fi
	fi
done < <(find "${roots[@]}" -type f \( -name '*.C' -o -name '*.cpp' \) 2>/dev/null)

if [ $fail -eq 0 ]; then
	echo "OK: every height-animating container declares an opaque-paint marker."
fi
exit $fail
