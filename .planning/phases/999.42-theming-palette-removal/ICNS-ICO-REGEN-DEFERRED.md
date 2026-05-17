# Phase 999.42 Task 11 — `.icns` / `.ico` regen: deferred

**Date:** 2026-05-17
**Disposition:** DEFER to Phase 8 packaging per PLAN Rule 4 escape clause.

## Why deferred

Handover §2.6 asks to add a build-time `.icns` (macOS) + `.ico` (Windows)
regeneration step driven from `BALLViewInstallFix.cmake.in` so that
the BALLView app icon comes from
`source/VIEW/KERNEL/theme/icons/lucide/ball-app.svg` (the new placeholder
shipped in 999.42/10) rather than the historical bucky bitmap.

Reality check after inspecting the codebase:

1. **`BALLViewInstallFix.cmake.in` is a runtime install-fix script** —
   it operates on the already-built `.app` bundle to patch dyld install
   names and copy frameworks. It does NOT run at configure / build time
   and is not the right hook for asset generation.

2. **The actual icon binding** is in
   `source/APPLICATIONS/BALLVIEW/CMakeLists.txt:55`:
   ```cmake
   SET(MACOSX_BUNDLE_ICON_FILE BALLView.icns)
   ```
   plus `source/APPLICATIONS/BALLVIEW/BALLView.{icns,ico,rc}` shipped as
   committed binary blobs.

3. **Toolchain dependencies for SVG→icns/ico:**
   - macOS: needs `iconutil` (Apple-only, ships with Xcode CLT) AND a
     rasterizer like `rsvg-convert` or `magick` to produce the source
     PNG set the `.iconset` directory needs.
   - Windows: needs `magick convert` or `png2ico` (multi-resolution
     `.ico`: 16/24/32/48/64/256 per Handover §2.6).
   - Linux: needs `magick convert` for the `.desktop` PNG.
   None of these are universally present on the GitHub Actions runner
   images — adding a hard dependency would risk breaking the existing
   `ui_v2 ∈ {ON, OFF}` CI matrix on every push.

4. **The bespoke `ball-app.svg` artwork is also deferred** to Phase 8
   (per `ball-app.svg` header). Regenerating `.icns` / `.ico` from a
   placeholder atom SVG would visually regress the shipped binaries
   (the current `BALLView.icns` is the bucky logo users recognize),
   so even with tooling in place the regen step has zero value until
   the bespoke artwork lands.

## What ships in 999.42 instead

- The `ball-app.svg` placeholder is vendored under
  `source/VIEW/KERNEL/theme/icons/lucide/` and aliased in `theme.qrc`
  (Task 10). It already drives `setWindowIcon(Icons::get("ball-app"))`
  in `mainframe.C` (Task 9a) at *runtime* — i.e. the in-app window /
  taskbar icon does come from the SVG today.
- The pre-existing `BALLView.icns` / `BALLView.ico` stay in
  `source/APPLICATIONS/BALLVIEW/` and continue to drive packaging.
  No visual regression.

## Forwarded to Phase 8

Phase 8 packaging is the right home for this work because:

- Phase 8 already owns `BALLViewInstallFix.cmake.in` evolution
  (Sparkle integration landed there in 999.8-full).
- Phase 8 sets the toolchain expectations for release-grade installers
  on each OS — adding `magick` / `iconutil` to those expectations is
  natural, not disruptive.
- The bespoke `ball-app.svg` artwork is targeted for Phase 8 anyway.

**Phase 8 ROADMAP breadcrumb to add (when 999.42 SUMMARY lands):**
"Regenerate `.icns` / `.ico` from `ball-app.svg` via CMake-driven
SVG→multi-resolution-iconset pipeline (Handover §2.6). Source SVG was
vendored in 999.42; bespoke artwork drops here."
