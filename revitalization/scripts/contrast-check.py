#!/usr/bin/env python3
"""contrast-check.py — Phase 999.48 §8.4

Validates the BALLView Refresh single neutral theme against WCAG 2.1
AA contrast requirements:

  - 4.5:1 for normal-size body text
  - 3:1 for large text (>= 18pt) and UI components / focus indicators

Per maintainer-Q3 the v1.7 BALLView Refresh ships a single neutral
theme only — no light/dark/follow-system variants — so this script
runs against `source/VIEW/KERNEL/theme/theme-neutral.qss` exclusively.

Algorithm:

1. Parse the QSS, extracting every (`color:`, `background-color:`)
   pair that appears within the same rule block.
2. Compute relative luminance per WCAG 2.1 §1.4.3 / §1.4.11.
3. Compute contrast ratio = (L1 + 0.05) / (L2 + 0.05).
4. Print PASS / FAIL per pair.
5. Exit 0 if all pairs pass, 1 if any fail.

Scope (per scope-trim guidance): minimal, pass/fail only. A fancier
WCAG report formatter (per-token annotations, suggested replacements,
HTML output) defers to a v1.7 RC patch.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path


HEX_RE = re.compile(r"#([0-9a-fA-F]{6})")
RULE_RE = re.compile(r"([^{}]+)\{([^{}]+)\}", re.MULTILINE)
COLOR_DECL_RE = re.compile(r"(?<!-)\bcolor\s*:\s*#([0-9a-fA-F]{6})", re.IGNORECASE)
BG_DECL_RE = re.compile(r"\bbackground-color\s*:\s*#([0-9a-fA-F]{6})", re.IGNORECASE)


def srgb_to_linear(c: float) -> float:
    """Convert an sRGB channel (0-1) to linear-light per WCAG 2.1."""
    if c <= 0.03928:
        return c / 12.92
    return ((c + 0.055) / 1.055) ** 2.4


def relative_luminance(rgb_hex: str) -> float:
    """Compute relative luminance per WCAG 2.1 §1.4.3."""
    r = int(rgb_hex[0:2], 16) / 255.0
    g = int(rgb_hex[2:4], 16) / 255.0
    b = int(rgb_hex[4:6], 16) / 255.0
    rl = srgb_to_linear(r)
    gl = srgb_to_linear(g)
    bl = srgb_to_linear(b)
    return 0.2126 * rl + 0.7152 * gl + 0.0722 * bl


def contrast_ratio(fg_hex: str, bg_hex: str) -> float:
    l1 = relative_luminance(fg_hex)
    l2 = relative_luminance(bg_hex)
    if l1 < l2:
        l1, l2 = l2, l1
    return (l1 + 0.05) / (l2 + 0.05)


def extract_color_pairs(qss_text: str) -> list[tuple[str, str, str]]:
    """Walk the QSS, returning (selector, fg_hex, bg_hex) tuples for
    every rule that declares BOTH `color:` and `background-color:`."""
    pairs: list[tuple[str, str, str]] = []
    for selector, body in RULE_RE.findall(qss_text):
        fg = COLOR_DECL_RE.search(body)
        bg = BG_DECL_RE.search(body)
        if fg and bg:
            pairs.append((selector.strip().splitlines()[-1].strip(),
                          fg.group(1).lower(), bg.group(1).lower()))
    return pairs


def is_disabled_selector(selector: str) -> bool:
    """WCAG 2.1 §1.4.3 explicitly exempts disabled UI components from
    contrast requirements. Returns True for selectors targeting the
    `:disabled` pseudo-state."""
    return ":disabled" in selector


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument(
        "qss_path",
        nargs="?",
        default="source/VIEW/KERNEL/theme/theme-neutral.qss",
        help="Path to the QSS file to validate (default: %(default)s).")
    parser.add_argument(
        "--threshold",
        type=float,
        default=4.5,
        help="WCAG ratio threshold (default 4.5 = AA normal body text).")
    args = parser.parse_args()

    qss = Path(args.qss_path)
    if not qss.exists():
        print(f"ERROR: {qss} not found", file=sys.stderr)
        return 2

    text = qss.read_text(encoding="utf-8")
    pairs = extract_color_pairs(text)
    if not pairs:
        print(f"WARNING: no (color, background-color) rule pairs found in {qss}",
              file=sys.stderr)
        return 0

    print(f"contrast-check.py — checking {len(pairs)} (fg, bg) pairs against "
          f"WCAG 2.1 AA ({args.threshold}:1 threshold)")
    print("-" * 72)
    failures = 0
    exempt = 0
    for selector, fg, bg in pairs:
        ratio = contrast_ratio(fg, bg)
        if is_disabled_selector(selector):
            # WCAG 2.1 §1.4.3 — disabled UI components are exempt.
            status = "EXEMPT"
            exempt += 1
        elif ratio >= args.threshold:
            status = "PASS"
        else:
            status = "FAIL"
            failures += 1
        print(f"  [{status:6s}] #{fg} on #{bg}  ratio={ratio:5.2f}:1  "
              f"({selector})")

    print("-" * 72)
    if failures:
        print(f"FAIL: {failures} of {len(pairs)} pairs below "
              f"{args.threshold}:1 WCAG AA threshold "
              f"({exempt} exempt :disabled pairs ignored)")
        return 1
    print(f"PASS: all {len(pairs) - exempt} non-exempt pairs meet WCAG 2.1 AA "
          f"({args.threshold}:1) on {qss} "
          f"({exempt} :disabled pairs exempted per WCAG §1.4.3)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
