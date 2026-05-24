// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.60 (v1.7.x-25) — ColoringController cross-surface parity.
//
// One of the 9 per-controller parity fixtures (ARCHITECTURE-CONTRACT.md
// §11a). Runs representative coloring operations through every available
// surface and asserts identical owner postconditions.
//
// HARNESS-FIRST SEQUENCING (Codex MEDIUM #7): the cross-surface parity
// body ships DISABLED (CONTRACT_FIXTURE_DISABLED) because it encodes the
// §2 `bool apply()` contract that 999.59 has not landed yet, and the
// toolbar/menu/python surfaces are not wired until 999.59-03/06. The
// 999.59 plan that cuts ColoringController over flips this green by
// defining BALL_VIEW_CONTROLLER_APPLY_RETURNS_BOOL and wiring the
// non-Inspector surfaces. The enabled portion below asserts the
// Inspector→owner postcondition, which holds today.
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

#include "contractTestHarness.h"

#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/controllers/coloringController.h>

#include "contractFixtureActivation.h"

#include <QtWidgets/QApplication>

using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Contract;

START_TEST(ColoringContract)

qputenv("QT_QPA_PLATFORM", "offscreen");
int argc = 1;
char argv0[] = "coloring_contract_test";
char* argv[] = { argv0, nullptr };
QApplication app(argc, argv);

// ---------------------------------------------------------------------------
// ENABLED today — Inspector surface postcondition.
// The Inspector path (controller_->setX(); controller_->apply()) must reach
// the owner. This holds in the current tree and is the safety net the
// 999.59 cut-over must not break.
// ---------------------------------------------------------------------------
CHECK(ColoringController Inspector surface reaches the owner)
	Representation rep;
	ColoringController c(&rep);
	c.setColoringMethod(COLORING_CHAIN);
	c.apply();
	TEST_EQUAL(static_cast<int>(rep.getColoringMethod()),
	           static_cast<int>(COLORING_CHAIN))
RESULT

// ---------------------------------------------------------------------------
// DISABLED until 999.59 cuts ColoringController over — cross-surface parity.
// Encodes the target contract: the same op through Inspector / Toolbar /
// Menu / Python leaves the owner bit-equal, and bool apply() reports
// mutated==true on each.
// ---------------------------------------------------------------------------
CHECK(DISABLED_coloring_method_parity_across_surfaces)
#if CONTRACT_FIXTURE_DISABLED_COLORING
	STATUS("DISABLED: awaits 999.59 ColoringController cut-over to bool apply() "
	       "+ §8 command-body migration. Flip green by defining "
	       "BALL_VIEW_CONTROLLER_APPLY_RETURNS_BOOL and wiring the toolbar/menu/"
	       "python surfaces.")
	break; // documented skip — compiles + registers, does not fail
#else
	ContractTestHarness h;
	h.setUp();

	std::vector<OwnerSnapshot> snaps;
	const SurfaceKind surfaces[] = {
		SurfaceKind::Inspector, SurfaceKind::Toolbar,
		SurfaceKind::Menu,      SurfaceKind::Python
	};
	for (SurfaceKind s : surfaces)
	{
		if (!surfaceAvailable(s)) continue;
		ColoringController c(h.representation());
		c.setColoringMethod(COLORING_ELEMENT);
		bool mutated = c.apply();      // 999.59: apply() returns bool
		TEST_EQUAL(mutated, true)
		snaps.push_back(h.snapshotOwner());
	}
	for (size_t i = 1; i < snaps.size(); ++i)
	{
		TEST_EQUAL(snaps[i - 1] == snaps[i], true)
	}
	h.tearDown();
#endif
RESULT

END_TEST
