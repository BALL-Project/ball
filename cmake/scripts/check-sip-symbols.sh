#!/usr/bin/env bash
# -----------------------------------------------------------------------------
# check-sip-symbols.sh — Phase 999.59-06 (v1.7.4)
#
# Lightweight symbol-reference lint for the VIEW Controller SIP bindings.
#
# WHY: SIP is disabled in the modernization build (CLAUDE.md) and is slated for
# full replacement in v2.1 (Phase 6 generator bake-off), so resurrecting SIP 4.9
# to *regenerate* the bindings in CI is not worth the fragility. But the real
# risk the §3d contract guards against is concrete and durable: a future C++
# setter rename silently rotting the Python surface (exactly what 999.59-04's
# privatization would have done). This script catches that without SIP: for each
# plain method declared in controllers.sip, it verifies the method name exists in
# the C++ header that class %TypeHeaderCode-includes.
#
# It deliberately SKIPS %MethodCode blocks (hand-written, e.g. the
# representation.sip deprecation shims that forward to controllers) and
# constructors/destructors. Full regen + runtime coverage ride with v2.1.
#
# Exit 0 = every plain SIP method maps to a real C++ symbol; non-zero otherwise.
# -----------------------------------------------------------------------------
set -uo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
SIP="$ROOT/source/PYTHON/EXTENSIONS/VIEW/controllers.sip"
INC="$ROOT/include"

[ -f "$SIP" ] || { echo "FAIL: $SIP not found"; exit 1; }

fail=0
header=""
class=""
in_methodcode=0

while IFS= read -r line; do
	# Track %MethodCode ... %End blocks and skip them (hand-written).
	if [[ "$line" == *"%MethodCode"* ]]; then in_methodcode=1; continue; fi
	if [[ $in_methodcode -eq 1 ]]; then
		[[ "$line" == *"%End"* ]] && in_methodcode=0
		continue
	fi

	# Capture the header a class block maps to via %TypeHeaderCode include.
	if [[ "$line" =~ ^class[[:space:]]+([A-Za-z0-9_]+) ]]; then
		class="${BASH_REMATCH[1]}"
		header=""
		continue
	fi
	if [[ "$line" =~ \#include[[:space:]]*\<(BALL/[^\>]+\.h)\> ]]; then
		# first include in the class's %TypeHeaderCode is its defining header
		[ -z "$header" ] && header="${BASH_REMATCH[1]}"
		continue
	fi

	# Plain method declaration: "<ret> name(...);" — capture the identifier
	# immediately before '(' on any line that declares a method.
	if [[ "$line" =~ ([A-Za-z0-9_]+)\(.*\)\;[[:space:]]*$ ]]; then
		method="${BASH_REMATCH[1]}"
		# skip constructors (method name == class name) and obvious non-methods
		[ "$method" = "$class" ] && continue
		[ -z "$header" ] && continue
		hpath="$INC/$header"
		if [ ! -f "$hpath" ]; then
			echo "FAIL: $class — header $header not found"
			fail=1
			continue
		fi
		if ! grep -qE "[[:space:]\*&]${method}\(" "$hpath"; then
			echo "FAIL: ${class}::${method}() declared in controllers.sip but not found in $header"
			fail=1
		fi
	fi
done < "$SIP"

if [ $fail -eq 0 ]; then
	echo "OK: every plain controllers.sip method maps to a real C++ symbol."
fi
exit $fail
