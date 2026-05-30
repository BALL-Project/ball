# Deferred Items — Phase 02 (Rendering Port 4a)

## Discovered during Plan 02-03

- **`include/BALL/VIEW/RENDERING/glDisplayList.h:13`** — `#include <QtOpenGL/qgl.h>`
  Legacy Qt4-era GL header. Surfaced by the `check-no-legacy-gl-symbols.sh` gate
  while verifying Plan 02-03. This file is **not** in any 02-rendering-port-4a
  plan's `files_modified` list (the phase scoped the port to glRenderWindow,
  renderSetup, glRenderer, glOffscreenTarget, scene). `glDisplayList` compiles
  today because `qgl.h` still exists in Qt 5.15 — it is a Qt6-removal blocker,
  not a Qt5 build blocker. Recommend folding into the Phase 5 (Qt 6 migration)
  cleanup, or a follow-up task in this phase if a fully-clean gate is required
  before phase close. Out of scope for Plan 02-03 (the convertToGLFormat /
  FBO / renderSetup cleanup).
