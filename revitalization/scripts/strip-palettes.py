#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Phase 999.42 — BALLView Refresh: palette-block strip script.
#
# Re-ported from the spec in
#   /Users/kohlbach/Claude/BALL/Claude Design Handover/revitalization/01-phase-theming.md
# §1.1 ("Write the strip script").
#
# Removes hardcoded <palette> blocks from Qt Designer .ui files so the
# dialogs inherit the OS / theme palette set by 999.40's ThemeManager
# (gated behind BALL_UI_V2). Also removes the "empty <font></font>"
# property pattern that frequently coexists with the legacy palette.
#
# Usage:
#   python3 revitalization/scripts/strip-palettes.py
#
# Idempotent — safe to re-run. Prints one line per modified file.
# Exit status: 0 always (no errors are fatal here).
#
# This script must run from the repo root; it walks `source/` for *.ui.

from __future__ import annotations
import re
import sys
import pathlib

# Match a full <property name="palette"><palette>...</palette></property>
# spanning multiple lines. The palette block is always wrapped in a
# <property name="palette"> opener, so we anchor on that to avoid greedy
# matches across unrelated <property> blocks.
PALETTE_PROPERTY = re.compile(
    r"^[ \t]*<property name=\"palette\">\s*"
    r"<palette>.*?</palette>\s*"
    r"</property>\s*\n",
    re.DOTALL | re.MULTILINE,
)

# Match the empty-<font></font> property pattern (no semantic intent
# preserved; Qt Designer often produces these when an inherited font is
# resaved). We do NOT touch <font> blocks that contain actual
# specifications (size, family, bold, etc.) — those represent intent.
EMPTY_FONT_PROPERTY = re.compile(
    r"^[ \t]*<property name=\"font\">\s*"
    r"<font>\s*</font>\s*"
    r"</property>\s*\n",
    re.MULTILINE,
)


def strip_one(path: pathlib.Path) -> bool:
    """Return True if the file was modified."""
    original = path.read_text(encoding="utf-8")
    stripped = PALETTE_PROPERTY.sub("", original)
    stripped = EMPTY_FONT_PROPERTY.sub("", stripped)
    if stripped != original:
        path.write_text(stripped, encoding="utf-8")
        return True
    return False


def main() -> int:
    root = pathlib.Path("source")
    if not root.is_dir():
        print(f"strip-palettes: cannot find {root.resolve()}", file=sys.stderr)
        return 1
    touched = 0
    for ui in sorted(root.rglob("*.ui")):
        if strip_one(ui):
            print(f"stripped {ui}")
            touched += 1
    print(f"strip-palettes: {touched} file(s) modified")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
