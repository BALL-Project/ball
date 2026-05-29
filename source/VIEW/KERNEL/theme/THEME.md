# BALLView Theme & Opaque-Paint Contract

Design-system reference for the BALLView VIEW layer. Companion to
`.planning/v1.7.4-ARCHITECTURE-CONTRACT.md` §10 (opaque-paint) and the neutral
theme in `source/VIEW/KERNEL/theme/theme-neutral.qss`.

## 1. Single neutral theme

Per maintainer decision (Phase 999.40, Q3) BALLView ships **one neutral
theme**. Light/dark variants are explicitly out of scope for v1.7; dark mode is
a v1.8 concern. The opaque-paint contract below is the structural prerequisite
that lets a future dark theme land without re-auditing every container for
flicker.

## 2. The opaque-paint contract (§10)

**Problem (UFG-05 / 09 / 10).** A container that animates a child's height —
the collapsible Inspector-section pattern (`QPropertyAnimation` on
`maximumHeight`, or `setMaximumHeight()` during expand/collapse) — repaints a
shrinking/growing region every frame. If the container is not opaque, the
*parent* shows through the uncovered region mid-animation as flicker, smear, or
a stale-pixel trail.

**Contract.** Every height-animating container MUST declare itself opaque on
both halves:

1. **Paint guarantee (C++).** In the widget's constructor:
   ```cpp
   setAttribute(Qt::WA_OpaquePaintEvent);   // Qt won't pre-fill; we own every pixel
   setAutoFillBackground(true);              // ...and we fill it
   setProperty("opaqueContainer", true);     // the themeable marker (below)
   ```
2. **Colour token (QSS).** `theme-neutral.qss` centralizes the fill as one rule
   keyed on the dynamic property — so the colour lives in the theme, not in N
   scattered `fillRect()` calls:
   ```css
   *[opaqueContainer="true"] { background-color: palette(window); }
   ```

The dynamic property is the single source of truth a future theme (dark mode)
re-skins by changing one rule, and the marker the lint keys on.

## 3. Enforcement

`cmake/scripts/check-opaque-paint.sh` (CI job `opaque-paint`, blocking, Linux)
flags any `source/VIEW/WIDGETS` file that animates a child's height but sets no
opaque-paint marker. Legitimate exceptions go in
`cmake/scripts/opaque-paint-whitelist.txt` with a rationale. The lint is
self-tested by `test/contract/opaque-paint-lint-selftest.sh` (compliant passes,
violation fails, whitelist exempts) so it cannot rot into a vacuous green.

As of v1.7.4 every height-animating container in the VIEW layer is compliant
and the whitelist is empty.

## 4. Compliant containers (v1.7.4)

`inspectorBody`, `inspectorSection`, `inspectorTabs`, `inspectorView`,
`inspectorEmptyState`, `sectionHeader`, `welcomeScreen` — all carry the
`WA_OpaquePaintEvent` + `autoFillBackground` + `opaqueContainer` triple.

## 5. See also

- `.planning/v1.7.4-ARCHITECTURE-CONTRACT.md` §10 — the full contract rationale.
- `.planning/codebase/QT-LIFETIME.md` — the sibling Qt-object-lifetime contract.
- `theme-neutral.qss` — the neutral palette + the `opaqueContainer` token.
