// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.60 (v1.7.x-25) — StereoController cross-surface parity.
// One of the 9 per-controller parity fixtures (ARCHITECTURE-CONTRACT.md
// §11a). See coloring_contract_test.C for the harness-first sequencing.
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

#include "contractTestHarness.h"

#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/controllers/stereoController.h>

#include <QtWidgets/QApplication>

using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Contract;

START_TEST(StereoContract)

qputenv("QT_QPA_PLATFORM", "offscreen");
int argc = 1;
char argv0[] = "stereo_contract_test";
char* argv[] = { argv0, nullptr };
QApplication app(argc, argv);

// ENABLED today — Inspector surface postcondition (headless-testable).
CHECK(StereoController Inspector surface reaches the Stage)
	Stage stage;
	StereoController c(&stage);
	c.setEyeDistance(2.0f);
	c.setFocalDistance(9.0f);
	c.setSwapSideBySide(true);
	c.apply();
	StereoController back(&stage);
	TEST_REAL_EQUAL(back.eyeDistance(), 2.0f)
	TEST_REAL_EQUAL(back.focalDistance(), 9.0f)
	TEST_EQUAL(back.swapSideBySide(), true)
RESULT

// DISABLED until 999.59 cuts StereoController over — cross-surface parity.
CHECK(DISABLED_stereo_parity_across_surfaces)
#if CONTRACT_FIXTURE_DISABLED
	STATUS("DISABLED: awaits 999.59 StereoController cut-over to bool apply(). "
	       "Flip green by defining BALL_VIEW_CONTROLLER_APPLY_RETURNS_BOOL.")
	break;
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
		StereoController c(h.stage());
		c.setEyeDistance(2.0f);
		bool mutated = c.apply();
		TEST_EQUAL(mutated, true)
		snaps.push_back(h.snapshotOwner());
	}
	for (size_t i = 1; i < snaps.size(); ++i)
		TEST_EQUAL(snaps[i - 1] == snaps[i], true)
	h.tearDown();
#endif
RESULT

END_TEST
