# Deferred Items — Phase 02.1

## From Plan 02.1-01

- **Pre-existing legacy GL symbol references** (`check-no-legacy-gl-symbols.sh` exits 1):
  7 references remain, all pre-existing and NOT regressed by Plan 01:
  - 6 historical-context comments mentioning "QGLWidget" in glRenderWindow.C / scene.C / scene.h
  - 1 real include `#include <QtOpenGL/qgl.h>` in `include/BALL/VIEW/RENDERING/glDisplayList.h`
  Out of scope for the boundary extraction. `glDisplayList.h` cleanup belongs to a
  later GL-modernization pass (Phase 5). No new legacy symbols were introduced by this plan.
