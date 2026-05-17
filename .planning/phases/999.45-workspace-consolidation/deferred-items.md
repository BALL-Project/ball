# Phase 999.45 — Deferred / out-of-scope items

## Pre-existing build break from concurrent Phase 999.44 work

**Discovered:** during Task 3 cmake reconfigure
**Status:** out of 999.45 scope — owned by Phase 999.44 agent

The `ADD_VIEW_HEADERS` macro in `cmake/BALLMacros.cmake:117` calls
`GET_FILENAME_COMPONENT(i ${i} NAME)` which strips the directory
prefix from header paths. Phase 999.44 placed its inspector headers
under `include/BALL/VIEW/WIDGETS/inspector/*.h` and updated
`include/BALL/VIEW/WIDGETS/sources.cmake` to glob that subdir, but the
macro then re-roots each header to `include/BALL/VIEW/WIDGETS/<basename>.h`,
producing the CMake error:

```
CMake Error at CMakeLists.txt:633 (ADD_LIBRARY):
  Cannot find source file:

    include/BALL/VIEW/WIDGETS/inspectorBody.h
```

**Root cause:** macro design assumption (flat per-group header layout).

**Repro without 999.45 changes:** Yes — verified by stashing my 999.45
working-tree changes and re-running `cmake -S . -B build/ci-macos`; the
same error occurs.

**Resolution paths (for 999.44 agent):**
1. Patch `ADD_VIEW_HEADERS` to preserve the relative subpath after the
   GROUP prefix (drop the `GET_FILENAME_COMPONENT` call or replace
   with a relative-path computation).
2. OR flatten the `inspector/` subdir so all 999.44 headers live at
   `include/BALL/VIEW/WIDGETS/*.h` like every other VIEW header today.

**Impact on 999.45:** Cannot run full cmake reconfigure / link of the
VIEW shared library locally. Worked around by direct `clang++` compile
of the 3 new translation units (workspaceManager.C, projectDock.C,
bottomDrawer.C) with project flags — all 3 compile clean against the
checked-in include tree. CI (clean check-out, both agents'
PRs merged) will reconfigure cleanly once 999.44 lands its macro fix.

**Note for verifier:** If you see the same configure error post-merge,
the 999.44 macro fix did not land. File a follow-up against 999.44.

---

## 999.44 OFF-cell linker failure — Inspector vtables

**Discovered:** during Task 11 SUMMARY-time OFF-cell smoke build
**Status:** out of 999.45 scope — owned by Phase 999.44 agent

After 999.44 fixed the cmake macro (resolved above), the BALL_UI_V2=OFF
cell still fails at link time:

```
"vtable for BALL::VIEW::InspectorTabs", referenced from:
    vtable for BALL::VIEW::InspectorTabs in mocs_compilation.cpp.o
... (same for InspectorView/InspectorSection/InspectorEmptyState/InspectorBody/InspectorDock)
ld: symbol(s) not found for architecture arm64
```

**Root cause:** 999.44 headers (`include/BALL/VIEW/WIDGETS/inspector/*.h`)
declare Q_OBJECT classes WITHOUT an `#ifdef BALL_UI_V2` ifdef wrap around
the class declarations. CMake's AUTOMOC therefore picks up the Q_OBJECT
macro in the OFF cell and generates moc code that references the
class destructors. The `.C` implementations ARE ifdef'd to empty TUs
when OFF, so the destructors don't exist, and the link fails.

**Resolution paths (for 999.44 agent):**
1. Wrap the entire class declaration inside each `inspector/*.h` in
   `#ifdef BALL_UI_V2` (the pattern Phase 999.43 used for iconBrowser.h).
2. OR move the headers out of the AUTOMOC scan (e.g., header-only
   inside the BALL_UI_V2 ifdef in a `.h.in` file).
3. OR provide stub destructors in an unconditional small TU when
   BALL_UI_V2 is OFF.

**Verified:** Phase 999.45 source files (workspaceManager.{h,C},
configMigration.{h,C}, projectDock.{h,C}, bottomDrawer.{h,C}) all
compile clean in both ON and OFF cells (verified by direct
clang++ smoke and by the OFF-cell link reaching the Inspector
symbols WITHOUT any 999.45 symbol failures).

**Impact on 999.45 OFF cell:** zero — 999.45 has no classes with
Q_OBJECT-without-an-ifdef-around-the-class-decl. mainframe.C's
BALL_UI_V2-gated includes ensure none of the 999.45 types are
referenced when the flag is OFF.

**ON cell:** builds cleanly with full BALLView executable, including
all 999.45 + 999.44 source files (verified locally at HEAD).

## ~~Windows MSVC BALL_VIEW_EXPORT linkage bug on WorkspaceManager~~ FIXED (Phase 999.45 follow-up)

**Resolution (2026-05-17, post-CI run 25996765175):** Root cause was
`workspaceManager.h` + `configMigration.h` using the wrong export macro —
`BALL_EXPORT` (the BALL DLL's export attribute) instead of
`BALL_VIEW_EXPORT` (the VIEW DLL's). On Windows MSVC, the two DLLs have
distinct export contracts; the moc-generated `staticMetaObject` definition
was tagged dllimport (because BALL_EXPORT in a VIEW TU resolves to import
when not building the BALL DLL), conflicting with the moc-generated definition.

Fixed by replacing `BALL_EXPORT` → `BALL_VIEW_EXPORT` in 4 declarations:
  - workspaceManager.h `class WorkspaceManager` (line 73)
  - workspaceManager.h `class WorkspaceStatusLabel` (line 180)
  - configMigration.h `struct MigrationResult` (line 48)
  - configMigration.h `class ConfigMigration` (line 69)

All other 999.45 classes (`ProjectDock`, `BottomDrawer`) already used
`BALL_VIEW_EXPORT` correctly. Verified by comparison against
`mainControl.h`, `molecularControl.h`, and the 999.44 Inspector headers
— all use `BALL_VIEW_EXPORT`.

**Original diagnostic notes preserved below for traceability:**

---

**Discovered:** CI run 25996765175 on HEAD `1d76af2e70` (Phase 999.44 plan 01 verification push)
**Status:** out of 999.44 scope — owned by Phase 999.45 follow-up

**Discovered:** CI run 25996765175 on HEAD `1d76af2e70` (Phase 999.44 plan 01 verification push)
**Status:** out of 999.44 scope — owned by Phase 999.45 follow-up

Both windows-x64 cells (ui_v2=ON AND ui_v2=OFF) failed compiling
`moc_workspaceManager.cpp` with:

```
moc_workspaceManager.cpp(61): error C2491:
  'BALL::VIEW::WorkspaceManager::staticMetaObject':
  definition of dllimport static data member not allowed

moc_workspaceManager.cpp(153): error C2491:
  'BALL::VIEW::WorkspaceStatusLabel::staticMetaObject':
  definition of dllimport static data member not allowed
```

Plus matching warnings on `qt_static_metacall` / `metaObject` / vtable
("inconsistent dll linkage").

**Root cause:** the `BALL_VIEW_EXPORT` macro (which expands to
`__declspec(dllexport)` when BUILDING the VIEW DLL and `__declspec(dllimport)`
otherwise — controlled by `VIEW_EXPORTS` or equivalent) is being mis-emitted
as **`__declspec(dllimport)`** during the VIEW shared-library build itself,
so MSVC refuses to define the Q_OBJECT meta-object's `staticMetaObject` as
"dllimport". The compile is happening as if the TU were importing, not
exporting, the symbol.

Most likely cause: `workspaceManager.h` is being included in a TU that
doesn't set the right "we are building VIEW" preprocessor flag, OR the
`#include` order leaves a stale `BALL_VIEW_EXPORT` definition.

**Inspector + Controller + StageSection + LegacySettingsHelper compile
cleanly on Windows** — they all use `BALL_VIEW_EXPORT` identically and
none of them appear in the error list.

**Reproduction:** push any commit to v1.7-modernization; Windows cells fail.
This was masked by the 999.45 SUMMARY's "local BALL_UI_V2=ON full BALLView
build clean on macOS-arm64" — Windows wasn't locally tested, and the
parallel agent's CI run was cancelled before its OFF cell completed.

**Resolution paths (for 999.45 agent or a v1.7 RC patch):**
1. Check whether `workspaceManager.h` is using a non-standard `BALL_VIEW_EXPORT`
   placement (compare against `mainControl.h` or `geometricControl.h` — both
   compile fine on Windows).
2. Verify the WorkspaceStatusLabel `Q_OBJECT` is in a class with the
   `BALL_VIEW_EXPORT` macro applied correctly (inline class definitions
   inside a header sometimes need explicit per-class export attributes).
3. Bisect-check by temporarily removing `BALL_VIEW_EXPORT` from one of the
   two classes and seeing whether the C2491 error follows the macro.

**Inspector classes are unaffected:** all 6 macos + linux cells (both ON
and OFF) pass on the same CI run.
