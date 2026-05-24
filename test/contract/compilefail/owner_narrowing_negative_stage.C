// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.60 (v1.7.x-25) — Owner-narrowing NEGATIVE compile test (Stage).
//
// ARCHITECTURE-CONTRACT.md §3 / §11c. This TU mutates the Stage owner
// DIRECTLY from a non-Controller translation unit. Per §3b the only legal
// Stage mutation surface is the StageMutation facade; direct setX() from
// outside a Controller must become a COMPILE ERROR once 999.59-04
// privatises the setters behind `friend class StageMutation`.
//
// HARNESS-FIRST SEQUENCING: the setters are still PUBLIC today, so this
// file COMPILES. The compile-test driver (run_compile_check.cmake) records
// the EXPECTED outcome as "compiles" for the current phase and asserts the
// actual outcome matches. After 999.59-04 privatises setBackgroundColor /
// setFogIntensity / setCamera, the expected outcome flips to
// "does-not-compile" and this file's failure to compile becomes the green
// signal (with a diagnostic matching `is private` / `no matching ...`).
//

#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/DATATYPE/colorRGBA.h>

using namespace BALL;
using namespace BALL::VIEW;

// A non-Controller free function that bypasses the mutation funnel.
void bypass_stage_mutation_from_outside_a_controller()
{
	Stage stage;
	// §3: these are the calls that MUST NOT compile from a non-Controller
	// TU after 999.59-04. Today they compile (setters public).
	stage.setFogIntensity(0.5f);
	stage.setEyeDistance(1.0f);
	stage.setFocalDistance(5.0f);
	stage.setBackgroundColor(ColorRGBA(0.1f, 0.2f, 0.3f, 1.0f));
}
