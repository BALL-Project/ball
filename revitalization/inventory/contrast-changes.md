# Contrast changes — Phase 999.48 §8.4

Audit run: 2026-05-17.

Tool: `revitalization/scripts/contrast-check.py` (Python, stdlib only).
Target: `source/VIEW/KERNEL/theme/theme-neutral.qss` (Q3 single-neutral
theme — no `theme-light.qss` / `theme-dark.qss` to audit).
Threshold: 4.5:1 (WCAG 2.1 AA, normal body text).

## Result

PASS — 16 of 16 non-exempt (foreground, background) rule pairs meet
WCAG 2.1 AA 4.5:1 on first run. No token adjustments needed.

## Exempt pairs (WCAG 2.1 §1.4.3)

WCAG 2.1 §1.4.3 explicitly exempts disabled UI components from the
contrast requirement. The audit reports them as `[EXEMPT]`:

| Selector              | fg       | bg       | Ratio   |
|-----------------------|----------|----------|---------|
| `QPushButton:disabled`| `#7a7f87`| `#f7f7f8`| 3.76:1  |
| `QLineEdit:disabled`  | `#7a7f87`| `#efeff1`| 3.51:1  |

Both pairs use `kInkMuted` (#7a7f87) on the neutral background tokens.
This is the intended visual signal that the control is non-interactive
— bumping the muted text to 4.5:1 would erode the visual distinction
from enabled controls.

## Token mapping (from theme-neutral.qss)

| Token     | Hex       | Usage                                      |
|-----------|-----------|--------------------------------------------|
| `kBg`     | `#f7f7f8` | window background                          |
| `kBgElev` | `#ffffff` | elevated surfaces (menus, buttons, fields) |
| `kBgSoft` | `#efeff1` | toolbars, headers, statusbar               |
| `kInk`    | `#1a1c1f` | body text (15.95:1 on `kBg`)              |
| `kInkSoft`| `#4c5158` | secondary text (6.97:1 on `kBgSoft`)      |
| `kInkMuted`|`#7a7f87` | disabled / hints (exempt per WCAG §1.4.3) |
| `kAccent` | `#2a6fdb` | focus / primary action                     |
| `kAccentBg`|`#e7efff` | selection background                       |

## Re-run

```bash
python3 revitalization/scripts/contrast-check.py
# Or, for the full-pair report with a custom threshold:
python3 revitalization/scripts/contrast-check.py --threshold 3.0
```

Exits non-zero on any non-exempt pair below threshold — suitable for a
CI gate. Wire-in deferred to v1.7 RC patch (the lint script in Task 5
is a separate gate and ships in this same phase).
