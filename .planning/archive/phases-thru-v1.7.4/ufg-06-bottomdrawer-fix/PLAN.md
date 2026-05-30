---
phase: ufg-06
plan: 00
type: fix
autonomous: true
wave: ufg-rc2
depends_on: []
requirements: []
---

# UFG-06 — BottomDrawer (Logs) does not expand from collapsed strip

## Objective

Fix the `BottomDrawer` so that clicking the collapsed `Logs ▾` strip (24 px) at
the bottom of the BALLView window actually animates the dock open to its 240 px
expanded state and reveals the `Log` / `Files` tabbed content.

## Context

- @source/VIEW/WIDGETS/bottomDrawer.h
- @source/VIEW/WIDGETS/bottomDrawer.C
- @source/APPLICATIONS/BALLVIEW/mainframe.C (lines 219-244 — drawer wiring)
- @.planning/v1.7-USER-FEEDBACK-GATE.md (UFG-06, lines 157-170)

## Root cause (confirmed by read of `bottomDrawer.C`)

`setExpanded(true)` (called from `toggle()` which is connected to the chevron's
`clicked` signal — wiring is correct) performs these steps in this order:

```cpp
// bottomDrawer.C:171-184
body_->setVisible(true);
setMinimumHeight(kCollapsedHeight);   // 24
setMaximumHeight(kExpandedHeight);    // 240   <-- pre-emptive
animateTo(kExpandedHeight);           // animates maximumHeight 240 -> 240
```

…and `animateTo` reads the *current* `maximumHeight()` as the animation's start
value:

```cpp
// bottomDrawer.C:198-202
height_animation_->stop();
height_animation_->setStartValue(maximumHeight());  // already 240 after line 182
height_animation_->setEndValue(targetHeight);       // 240
height_animation_->start();
```

So the animation runs from 240 → 240 — a no-op — and the `QDockWidget`'s actual
laid-out height in the `QMainWindow` bottom dock area stays pinned at 24 px
because:

1. `minimumHeight` is still 24 px (line 178).
2. `QPropertyAnimation` on `maximumHeight` never sees a delta, so it never
   nudges the layout to relayout the dock area.
3. The `finished` slot fires immediately (zero-distance animation completes on
   the next event loop turn) and sets `maximumHeight = QWIDGETSIZE_MAX`, but by
   then the bottom dock row has already been laid out at 24 px and Qt does not
   spontaneously re-grow it.

Net effect: chevron click is detected, `expanded_` flips to `true`, `body_` is
shown but is squashed to ~0 visible px inside the 24 px dock row, and the user
sees nothing change.

The collapse path (`setExpanded(false)`) coincidentally works visually because
the start value `maximumHeight()` is `QWIDGETSIZE_MAX` (set by the previous
expand's `finished` slot, if it ever ran) — but with the expand broken, the
collapse never gets exercised either.

## Fix recipe

**File:** `source/VIEW/WIDGETS/bottomDrawer.C`

**Edit 1 — `BottomDrawer::setExpanded` (lines 166-190):** stop pre-setting
`maximumHeight` to the target before animating; lift the cap to a value that
the animation can drive *toward*, and force `minimumHeight` up during the
expand so the `QMainWindow` dock-area layout actually re-allocates row space.

```cpp
void BottomDrawer::setExpanded(bool expanded)
{
    if (expanded_ == expanded) return;
    expanded_ = expanded;

    if (expanded_)
    {
        chevron_->setIcon(VIEW::Icons::get("actions/chevron-down"));
        body_->setVisible(true);
        // Raise the cap so the animation has room to grow into.
        // Keep minimumHeight at kCollapsedHeight so the animation can start
        // from the current 24 px floor — animateTo() will drive maximumHeight
        // upward from its current 24 px value to kExpandedHeight, and Qt's
        // dock layout will follow because we also raise minimumHeight in
        // the finished slot.
        animateTo(kExpandedHeight);
    }
    else
    {
        chevron_->setIcon(VIEW::Icons::get("actions/chevron-up"));
        // Allow the animation to shrink from whatever the current size is.
        // Drop the minimumHeight floor first so the animation can pull the
        // dock back down past the current min.
        setMinimumHeight(kCollapsedHeight);
        animateTo(kCollapsedHeight);
    }
}
```

**Edit 2 — `BottomDrawer::animateTo` (lines 197-203):** use the current
*rendered* `height()` (not the `maximumHeight` cap) as the animation start
value so the animation always has a non-zero delta when there is real visible
work to do.

```cpp
void BottomDrawer::animateTo(int targetHeight)
{
    height_animation_->stop();
    // Drive maximumHeight from the current rendered height (which is what the
    // user actually sees) to the target. Using maximumHeight() as the start
    // would give a 24->240 step the first time (because max was capped at 24)
    // OR a 240->240 no-op (after a prior animation lifted the cap) — neither
    // matches the user's visual starting point.
    setMaximumHeight(std::max(height(), targetHeight));
    height_animation_->setStartValue(height());
    height_animation_->setEndValue(targetHeight);
    height_animation_->start();
}
```

**Edit 3 — `BottomDrawer::ctor` finished-slot (lines 65-82):** raise
`minimumHeight` to a small fraction of `kExpandedHeight` on expand so the dock
row is forced to allocate space; on collapse, pin `minimumHeight` back to
`kCollapsedHeight`. This is the layout nudge that makes
`QMainWindow::resizeDocks` follow the animated `maximumHeight`.

```cpp
connect(height_animation_, &QPropertyAnimation::finished, this, [this]() {
    if (expanded_)
    {
        setMaximumHeight(QWIDGETSIZE_MAX);
        // Pin the floor at kCollapsedHeight so user can still drag-resize
        // smaller; the visible row is now whatever the animation grew it to.
        setMinimumHeight(kCollapsedHeight);
        body_->setVisible(true);
    }
    else
    {
        setMaximumHeight(kCollapsedHeight);
        setMinimumHeight(kCollapsedHeight);
        body_->setVisible(false);
    }
    Q_EMIT expandedChanged(expanded_);
});
```

**Edit 4 — `QMainWindow::resizeDocks` nudge.** Even with the cap raised, Qt's
dock-area layout may not enlarge a 24 px dock row without an explicit hint.
After `animateTo(kExpandedHeight)` returns, call `parentWidget`-cast to
`QMainWindow` and invoke `resizeDocks({this}, {kExpandedHeight}, Qt::Vertical)`
to force the layout to allocate the row. Mirror on collapse.

This is the only call site that actually moves the dock-row pixels in a
`QMainWindow` layout — without it, animating `maximumHeight` alone is
insufficient on QDockWidget.

**Add include:** `#include <QtWidgets/QMainWindow>` and `#include <algorithm>`
(for `std::max`).

## Tasks

### Task 1: Fix expand/collapse animation + force dock-row resize (type=auto)

**Files:**
- `source/VIEW/WIDGETS/bottomDrawer.C`

**Behavior:**
- Clicking the chevron on the 24 px collapsed strip animates the dock open to
  240 px over 200 ms, showing the `Log` / `Files` tab content.
- Clicking the chevron when expanded animates the dock closed back to 24 px.
- Drag-resize beyond 240 px still works after expansion (cap lifted in
  `finished` slot).

**Implementation:** apply Edits 1-4 above. Use `qobject_cast<QMainWindow*>` to
locate the parent `QMainWindow`; if cast fails (drawer is parented elsewhere
in tests), skip the `resizeDocks` call.

**Verification (no GUI):**
- `grep -n "setMaximumHeight\|setMinimumHeight\|resizeDocks\|animateTo" source/VIEW/WIDGETS/bottomDrawer.C`
  shows the new order.
- `setStartValue(height())` (not `setStartValue(maximumHeight())`) appears in
  `animateTo`.
- The pre-emptive `setMaximumHeight(kExpandedHeight)` before the animation in
  `setExpanded(true)` is gone.
- Local incremental rebuild succeeds (`cmake --build build/macos-arm64 --target VIEW -j`).
- `BALLView` target relinks.

**Done when:**
- Code changes match the recipe.
- `make VIEW` and `make BALLView` compile clean (no new warnings).
- Commit `fix(ufg-06/bottomdrawer): drive maximumHeight from current height and resizeDocks` lands.

## Success criteria

- [ ] `BottomDrawer` source contains the corrected `setExpanded` / `animateTo`
      pair + `QMainWindow::resizeDocks` call.
- [ ] `libVIEW` + `BALLView` rebuild cleanly on macOS-arm64.
- [ ] No new compiler warnings introduced in `bottomDrawer.C`.
- [ ] `.planning/v1.7-USER-FEEDBACK-GATE.md` UFG-06 entry updated to `✅ FIXED`
      with commit hash.

## Out of scope (defer)

- Click-on-strip-anywhere toggle (today only the chevron is clickable — line
  111-112 explicitly notes "we keep that simple"). Not regressing UFG-06.
- Replacing `QDockWidget` + custom title bar with a non-dock collapsible
  widget. Would simplify the animation story but is architectural (>200 LOC,
  touches `WorkspaceManager` preset addressing by objectName). Defer.
- LogView / FileObserver inner-widget reparenting (already handled in
  `buildBody`; not the root cause of UFG-06).
