#!/usr/bin/env bash
# check-deletelater-discipline.sh
#
# v1.7.x-26 (phase 999.61) — Qt-lifetime CI lint.
#
# Enforces rule 5 of .planning/codebase/QT-LIFETIME.md:
#   "No deleteLater() inside destructors. Qt does the parent walk for us."
#
# A deleteLater() call inside a ~Class() destructor body schedules a deferred
# delete that fires AFTER the object (and its Qt parent walk) is already being
# torn down — the classic dtor-order / dangling-pointer foot-gun (UFG-30/32).
# Qt already deletes parented children; an explicit deleteLater() in a dtor is
# at best redundant and at worst a use-after-free.
#
# This is the lightweight grep-class guard shipped in v1.7.4. The full
# clang-tidy custom check (ball-no-raw-deletelater) is deferred to v1.7.5.
#
# Usage:   cmake/scripts/check-deletelater-discipline.sh [DIR ...]
# Exit:    0 = clean, 1 = at least one violation (prints file:line), 2 = usage error.
#
# Heuristic: walk each .C/.cpp file, detect a destructor signature
# `::~Name(` (an out-of-line dtor definition), then track brace depth from the
# opening `{` of its body. Any deleteLater() seen before depth returns to 0 is
# a violation. Comment-only lines (// ...) are ignored so documentation that
# mentions deleteLater() in a dtor comment does not trip the lint.

set -euo pipefail

# Default scan roots: the VIEW / BALLView Qt surface where QObject lifetimes
# live. Callers may override by passing explicit directories.
if [[ $# -gt 0 ]]; then
  ROOTS=("$@")
else
  ROOTS=("source/VIEW" "source/APPLICATIONS/BALLVIEW")
fi

for r in "${ROOTS[@]}"; do
  if [[ ! -d "$r" ]]; then
    echo "check-deletelater-discipline: scan root not found: $r" >&2
    exit 2
  fi
done

# Collect candidate translation units (.C is BALL's impl extension; .cpp for
# any generated/auxiliary sources).
mapfile -t FILES < <(find "${ROOTS[@]}" -type f \( -name '*.C' -o -name '*.cpp' \) | sort)

VIOLATIONS=$(
  for f in "${FILES[@]}"; do
    awk -v FNAME="$f" '
      BEGIN { in_dtor = 0; depth = 0 }
      {
        line = $0
        # Strip a trailing // line-comment so a documented mention of
        # deleteLater() inside a dtor comment is not flagged. (Block comments
        # are rare in dtor bodies; the line-comment strip covers the doc case
        # this lint cares about without a full C++ tokenizer.)
        code = line
        sub(/\/\/.*/, "", code)

        if (in_dtor == 0) {
          # Out-of-line destructor definition: `Type::~Name(`
          if (code ~ /::~[A-Za-z_][A-Za-z0-9_]*[[:space:]]*\(/) {
            # A defaulted/deleted dtor on one line has no body to scan.
            if (code ~ /=[[:space:]]*(default|delete)[[:space:]]*;/) next
            in_dtor = 1
            depth = 0
            # Count braces already on the signature line (e.g. `{ ... }`).
            n = gsub(/{/, "{", code); depth += n
            m = gsub(/}/, "}", code); depth -= m
            if (code ~ /deleteLater[[:space:]]*\(/)
              printf "%s:%d: deleteLater() inside destructor body\n", FNAME, NR
            if (depth <= 0 && line ~ /{/) { in_dtor = 0 }
            next
          }
        } else {
          n = gsub(/{/, "{", code); depth += n
          if (code ~ /deleteLater[[:space:]]*\(/)
            printf "%s:%d: deleteLater() inside destructor body\n", FNAME, NR
          m = gsub(/}/, "}", code); depth -= m
          if (depth <= 0) in_dtor = 0
        }
      }
    ' "$f"
  done
)

if [[ -n "$VIOLATIONS" ]]; then
  echo "Qt-lifetime discipline violation(s) — deleteLater() inside a destructor:" >&2
  echo "$VIOLATIONS" >&2
  echo "" >&2
  echo "Rule 5 (.planning/codebase/QT-LIFETIME.md): never deleteLater() in a dtor." >&2
  echo "Qt's parent walk deletes children; an explicit dtor deleteLater() races" >&2
  echo "teardown order (UFG-30/32). If you need ordered destruction, do it BEFORE" >&2
  echo "the parent dtor runs, not via a deferred-delete posted during teardown." >&2
  exit 1
fi

echo "check-deletelater-discipline: clean — no deleteLater() in any destructor."
exit 0
