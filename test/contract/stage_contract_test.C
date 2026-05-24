// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.60 (v1.7.x-25) — StageController cross-surface parity.
// One of the 9 per-controller parity fixtures (ARCHITECTURE-CONTRACT.md
// §11a). See coloring_contract_test.C for the harness-first sequencing.
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

#include "contractTestHarness.h"

#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/controllers/stageController.h>

#include <QtWidgets/QApplication>

using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Contract;

START_TEST(StageContract)

qputenv("QT_QPA_PLATFORM", "offscreen");
int argc = 1;
char argv0[] = "stage_contract_test";
char* argv[] = { argv0, nullptr };
QApplication app(argc, argv);

// ENABLED today — Inspector surface postcondition (Stage-attached
// controllers are headless-testable: apply() guards the Scene/MainControl
// singletons, so only the Stage is mutated).
CHECK(StageController Inspector surface reaches the Stage)
	Stage stage;
	StageController c(&stage, nullptr);
	c.setFogIntensity(0.42f);
	c.setEyeDistance(1.5f);
	c.setFocalDistance(7.25f);
	c.apply();
	TEST_REAL_EQUAL(stage.getFogIntensity(), 0.42f)
	TEST_REAL_EQUAL(stage.getEyeDistance(), 1.5f)
	TEST_REAL_EQUAL(stage.getFocalDistance(), 7.25f)
RESULT

// DISABLED until 999.59 cuts StageController over — cross-surface parity.
// Stage declares heavy invalidation (rep-delete + renderer switch, §2a);
// the snapshot must remain bit-equal across surfaces regardless.
CHECK(DISABLED_stage_parity_across_surfaces)
#if CONTRACT_FIXTURE_DISABLED
	STATUS("DISABLED: awaits 999.59 StageController cut-over to bool apply() + "
	       "§8 command-body migration. Flip green by defining "
	       "BALL_VIEW_CONTROLLER_APPLY_RETURNS_BOOL.")
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
		StageController c(h.stage(), nullptr);
		c.setFogIntensity(0.42f);
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
