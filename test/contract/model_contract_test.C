// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.60 (v1.7.x-25) — ModelController cross-surface parity.
// One of the 9 per-controller parity fixtures (ARCHITECTURE-CONTRACT.md
// §11a). See coloring_contract_test.C for the harness-first sequencing
// rationale (cross-surface parity ships DISABLED until 999.59).
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

#include "contractTestHarness.h"

#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/controllers/modelController.h>

#include <QtWidgets/QApplication>

using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Contract;

START_TEST(ModelContract)

qputenv("QT_QPA_PLATFORM", "offscreen");
int argc = 1;
char argv0[] = "model_contract_test";
char* argv[] = { argv0, nullptr };
QApplication app(argc, argv);

// ENABLED today — Inspector surface postcondition. ModelController's
// apply() is Scene-independent for the Representation mirrors (see the
// existing ControllerContract_test rationale).
CHECK(ModelController Inspector surface reaches the owner)
	Representation rep;
	ModelController c(&rep);
	c.setModelType(MODEL_VDW);
	c.setDrawingPrecision(2);
	c.setTransparency(64);
	c.apply();
	TEST_EQUAL(static_cast<int>(rep.getModelType()), static_cast<int>(MODEL_VDW))
	TEST_EQUAL(static_cast<int>(rep.getDrawingPrecision()), 2)
	TEST_EQUAL(static_cast<int>(rep.getTransparency()), 64)
RESULT

// DISABLED until 999.59 cuts ModelController over — cross-surface parity.
// Model declares update(true) (display-list rebuild, §2a) — the parity
// snapshot must remain bit-equal across surfaces despite the rebuild.
CHECK(DISABLED_model_type_parity_across_surfaces)
#if CONTRACT_FIXTURE_DISABLED
	STATUS("DISABLED: awaits 999.59 ModelController cut-over to bool apply(). "
	       "Flip green by defining BALL_VIEW_CONTROLLER_APPLY_RETURNS_BOOL + "
	       "wiring the toolbar/menu/python surfaces.")
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
		ModelController c(h.representation());
		c.setModelType(MODEL_VDW);
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
