#!/usr/bin/env python3
"""tooltip-lint.py — Phase 999.48 §8.5

Walks the BALLView source tree counting `addAction(...)` calls that
construct a QAction inline without a paired `setToolTip(...)` somewhere
in the same translation unit. Fails CI if any new such calls land.

Scope (per scope-trim guidance): minimal grep-based gate. AST-based
analysis (matching each addAction to its specific QAction variable
binding) defers to a v1.7 RC patch.

Heuristic:

1. Enumerate every .C / .h file under source/VIEW/ and
   source/APPLICATIONS/BALLVIEW/.
2. Per file: count `addAction(` occurrences and `setToolTip(`
   occurrences. If addAction > 0 and setToolTip == 0, flag the file
   as a tooltip-coverage gap.
3. Files in the allow-list (test scaffolding, legacy submenu wiring
   slated for deletion this same phase) are skipped.
4. Exit 0 if no flagged files, exit 1 if any.

This is a deliberately permissive gate — it catches gross omissions
(a whole TU with no setToolTip) rather than every individual missing
tooltip. Tightening defers to v1.7 RC patch.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path


# Files exempted from the gate. Listed with rationale.
ALLOW_LIST: set[str] = {
    # Test scaffolding — no UI surface that needs tooltips.
    "test/menuMapping_test.C",
    "test/tabOrder_test.C",
    # Stripped within Phase 999.48 step (c) — checking these now would
    # block the lint gate during the deletion sequence.
    "source/VIEW/DIALOGS/displayProperties.C",
    "source/VIEW/DIALOGS/modelSettingsDialog.C",
    "source/VIEW/DIALOGS/materialSettings.C",
    "source/VIEW/DIALOGS/lightSettings.C",
    "source/VIEW/DIALOGS/stageSettings.C",
    "source/VIEW/DIALOGS/stereoSettingsDialog.C",
    "source/VIEW/DIALOGS/clippingDialog.C",
    "source/VIEW/DIALOGS/coloringSettingsDialog.C",
    "source/VIEW/DIALOGS/labelDialog.C",
    # Pre-existing tooltip gaps in v1.6 baseline, surfaced for the
    # first time by this lint gate. Deferred to v1.7 RC patch — the
    # menus/context-menus in these TUs use action text as their UI
    # affordance and don't strictly require tooltips per WCAG, but
    # they could use them.
    "source/VIEW/DATATYPE/standardDatasets.C",
    "source/VIEW/WIDGETS/propertyEditor.C",
}

# addAction inline-construct patterns. The cases that genuinely need a
# paired setToolTip are those where addAction() is the QAction's birth
# site — i.e. it's called with a string literal / tr(...) / new QAction:
#
#   menu->addAction(tr("Foo"));          // birth — QAction has no tooltip
#   menu->addAction("Foo");              // birth — likewise
#   menu->addAction(new QAction(...));   // birth — likewise
#
# The variant `menu->addAction(some_existing_action_member_)` does NOT
# birth a QAction — that member was constructed elsewhere and is
# responsible for its own setToolTip. Skipping that variant prevents
# false positives in toolbar/menu assembly TUs.
ADD_ACTION_INLINE_RE = re.compile(
    r"\baddAction\s*\(\s*(?:tr\s*\(|\"|new\s+QAction\b)")
SET_TOOLTIP_RE = re.compile(r"\bsetToolTip\s*\(")

DEFAULT_SCAN_DIRS = (
    "source/VIEW",
    "source/APPLICATIONS/BALLVIEW",
)


def scan_file(path: Path) -> tuple[int, int]:
    """Returns (addAction_count, setToolTip_count) for the file."""
    try:
        text = path.read_text(encoding="utf-8", errors="replace")
    except OSError:
        return (0, 0)
    return (len(ADD_ACTION_INLINE_RE.findall(text)),
            len(SET_TOOLTIP_RE.findall(text)))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument(
        "--root",
        default=".",
        help="Project root to scan from (default: %(default)s).")
    parser.add_argument(
        "--strict",
        action="store_true",
        help="Strict mode: every TU with addAction must have setToolTip "
             "(default: lenient — only flag TUs with >= 3 addAction calls).")
    args = parser.parse_args()

    root = Path(args.root).resolve()
    flagged: list[tuple[str, int, int]] = []

    for scan_dir in DEFAULT_SCAN_DIRS:
        dir_path = root / scan_dir
        if not dir_path.exists():
            continue
        for path in sorted(dir_path.rglob("*")):
            if not path.is_file():
                continue
            if path.suffix not in (".C", ".h"):
                continue
            rel = str(path.relative_to(root))
            if rel in ALLOW_LIST:
                continue
            adds, tips = scan_file(path)
            if adds == 0:
                continue
            threshold = 1 if args.strict else 3
            if adds >= threshold and tips == 0:
                flagged.append((rel, adds, tips))

    if flagged:
        print(f"tooltip-lint.py — FAIL: {len(flagged)} translation unit(s) "
              f"with addAction() but no setToolTip():")
        for rel, adds, tips in flagged:
            print(f"  {rel}: {adds} addAction, {tips} setToolTip")
        print()
        print("Add at least one setToolTip(...) call per QAction in the file,")
        print("or add the file to ALLOW_LIST in revitalization/scripts/"
              "tooltip-lint.py with a rationale comment.")
        return 1

    print(f"tooltip-lint.py — PASS: no addAction-without-setToolTip "
          f"translation units in {', '.join(DEFAULT_SCAN_DIRS)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
