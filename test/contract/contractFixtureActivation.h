// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.59-02 (v1.7.x-24) — PER-CONTROLLER contract-fixture activation.
//
// WHY THIS HEADER EXISTS (Codex MEDIUM #7, incremental-green design).
//
// 999.59-01 introduced a SINGLE coarse feature macro,
// BALL_VIEW_CONTROLLER_APPLY_RETURNS_BOOL, which — once defined by the base
// Controller header — flips CONTRACT_FIXTURE_DISABLED to 0 GLOBALLY in
// contractTestHarness.h. That activates the cross-surface parity block of
// ALL 9 contract fixtures at once. Each parity block does
// `bool mutated = controller.apply();`, but the 999.59 cut-over is
// INCREMENTAL: only a subset of controllers gain a `bool apply()` in each
// plan. A controller still returning `void` makes `bool mutated = c.apply();`
// fail to compile, breaking the entire contract_tests target and defeating
// the per-domain `ctest -R 'Model|Coloring|Material'` validation the
// incremental design depends on.
//
// THE PER-CONTROLLER MECHANISM. A controller signals "I have been cut over
// to the §2 `bool apply()` shape" by defining its own per-domain symbol
// `BALL_VIEW_<DOMAIN>_APPLY_BOOL` in its public header. This header reads
// those symbols and derives a per-domain `CONTRACT_FIXTURE_DISABLED_<DOMAIN>`
// switch (0 = live parity block, 1 = STATUS-skip). Each fixture gates its
// parity block on its OWN domain switch instead of the global
// CONTRACT_FIXTURE_DISABLED, so a fixture compiles + runs the live parity
// block ONLY when its controller actually returns bool.
//
// INCLUDE ORDER CONTRACT. A fixture MUST include its controller header
// (which defines the per-domain symbol when cut over) BEFORE including this
// header, so the `#if defined(...)` checks below see the symbol. This header
// is then included, and the fixture's parity `#if` tests the derived
// per-domain switch. The order in every fixture is therefore:
//
//     #include "contractTestHarness.h"
//     #include <BALL/VIEW/KERNEL/controllers/<domain>Controller.h>
//     #include "contractFixtureActivation.h"   // <- AFTER the controller hdr
//
// CUT-OVER STATUS (this plan, 999.59-02):
//   - Model / Coloring / Material : cut over → per-domain symbol defined in
//     their headers → parity block LIVE (green target).
//   - Stage / Camera / Light / Stereo / Clipping / Label : NOT cut over yet
//     (999.59-03) → symbol absent → parity block STATUS-skip, so
//     contract_tests still COMPILES with their `void apply()` intact.
//
// Add a `#define BALL_VIEW_<DOMAIN>_APPLY_BOOL 1` to a controller's header
// the moment its `apply()` returns bool; the matching fixture then flips
// green automatically. Remove this whole shim once all 9 are cut over and
// the global CONTRACT_FIXTURE_DISABLED can be retired.
//

#ifndef BALL_TEST_CONTRACT_CONTRACTFIXTUREACTIVATION_H
#define BALL_TEST_CONTRACT_CONTRACTFIXTUREACTIVATION_H

// The base-class feature macro must be present (the harness includes the
// base Controller header). Per-domain activation is a refinement of it: a
// domain's parity block is live only when BOTH the base contract has landed
// AND that specific controller has been cut over to bool apply().
#ifndef BALL_TEST_CONTRACT_CONTRACTTESTHARNESS_H
#	include "contractTestHarness.h"
#endif

// ---------------------------------------------------------------------------
// Per-domain switch derivation. CONTRACT_FIXTURE_DISABLED_<DOMAIN> is:
//   0  when the controller's per-domain BALL_VIEW_<DOMAIN>_APPLY_BOOL symbol
//      is defined (controller cut over → run the live parity block), AND the
//      global base contract has landed.
//   1  otherwise (STATUS-skip; the void-apply() controller must NOT have its
//      parity block compiled).
//
// Gating on the GLOBAL base macro too means that if 999.59-01 were reverted
// the whole suite falls back to skip — the same safe default the original
// single-macro design had.
// ---------------------------------------------------------------------------
#if defined(BALL_VIEW_CONTROLLER_APPLY_RETURNS_BOOL) && defined(BALL_VIEW_MODEL_APPLY_BOOL)
#	define CONTRACT_FIXTURE_DISABLED_MODEL 0
#else
#	define CONTRACT_FIXTURE_DISABLED_MODEL 1
#endif

#if defined(BALL_VIEW_CONTROLLER_APPLY_RETURNS_BOOL) && defined(BALL_VIEW_COLORING_APPLY_BOOL)
#	define CONTRACT_FIXTURE_DISABLED_COLORING 0
#else
#	define CONTRACT_FIXTURE_DISABLED_COLORING 1
#endif

#if defined(BALL_VIEW_CONTROLLER_APPLY_RETURNS_BOOL) && defined(BALL_VIEW_MATERIAL_APPLY_BOOL)
#	define CONTRACT_FIXTURE_DISABLED_MATERIAL 0
#else
#	define CONTRACT_FIXTURE_DISABLED_MATERIAL 1
#endif

// --- Not-yet-cut-over domains (999.59-03). Symbol absent ⇒ STATUS-skip. ---
#if defined(BALL_VIEW_CONTROLLER_APPLY_RETURNS_BOOL) && defined(BALL_VIEW_STAGE_APPLY_BOOL)
#	define CONTRACT_FIXTURE_DISABLED_STAGE 0
#else
#	define CONTRACT_FIXTURE_DISABLED_STAGE 1
#endif

#if defined(BALL_VIEW_CONTROLLER_APPLY_RETURNS_BOOL) && defined(BALL_VIEW_CAMERA_APPLY_BOOL)
#	define CONTRACT_FIXTURE_DISABLED_CAMERA 0
#else
#	define CONTRACT_FIXTURE_DISABLED_CAMERA 1
#endif

#if defined(BALL_VIEW_CONTROLLER_APPLY_RETURNS_BOOL) && defined(BALL_VIEW_LIGHT_APPLY_BOOL)
#	define CONTRACT_FIXTURE_DISABLED_LIGHT 0
#else
#	define CONTRACT_FIXTURE_DISABLED_LIGHT 1
#endif

#if defined(BALL_VIEW_CONTROLLER_APPLY_RETURNS_BOOL) && defined(BALL_VIEW_STEREO_APPLY_BOOL)
#	define CONTRACT_FIXTURE_DISABLED_STEREO 0
#else
#	define CONTRACT_FIXTURE_DISABLED_STEREO 1
#endif

#if defined(BALL_VIEW_CONTROLLER_APPLY_RETURNS_BOOL) && defined(BALL_VIEW_CLIPPING_APPLY_BOOL)
#	define CONTRACT_FIXTURE_DISABLED_CLIPPING 0
#else
#	define CONTRACT_FIXTURE_DISABLED_CLIPPING 1
#endif

#if defined(BALL_VIEW_CONTROLLER_APPLY_RETURNS_BOOL) && defined(BALL_VIEW_LABEL_APPLY_BOOL)
#	define CONTRACT_FIXTURE_DISABLED_LABEL 0
#else
#	define CONTRACT_FIXTURE_DISABLED_LABEL 1
#endif

#endif // BALL_TEST_CONTRACT_CONTRACTFIXTUREACTIVATION_H
