// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.60 (v1.7.x-25) — MaterialController cross-surface parity.
// One of the 9 per-controller parity fixtures (ARCHITECTURE-CONTRACT.md
// §11a). See coloring_contract_test.C for the harness-first sequencing.
//
// NOTE: MaterialController::apply() bails when Scene::getInstance(0) is
// null (material is per-rep Scene/Renderer state) — so the Inspector
// postcondition is NOT headless-testable here. The whole fixture is the
// disabled cross-surface parity body; it flips green once 999.59 wires the
// Scene-backed surfaces. The enabled portion only asserts the staged
// (pending) mirror, which is headless-safe.
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

#include "contractTestHarness.h"

#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/controllers/materialController.h>

#include "contractFixtureActivation.h"

#include <QtWidgets/QApplication>

using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Contract;

START_TEST(MaterialContract)

qputenv("QT_QPA_PLATFORM", "offscreen");
int argc = 1;
char argv0[] = "material_contract_test";
char* argv[] = { argv0, nullptr };
QApplication app(argc, argv);

// ENABLED today — staged-mirror sanity (headless-safe: does not require a
// Scene). The full owner postcondition needs a Scene and is covered by the
// disabled parity body below.
CHECK(MaterialController stages transparency on its mirror)
	Representation rep;
	MaterialController c(&rep);
	c.setTransparency(50);
	TEST_EQUAL(c.transparency(), 50)
RESULT

// DISABLED until 999.59 cuts MaterialController over — cross-surface parity.
CHECK(DISABLED_material_transparency_parity_across_surfaces)
#if CONTRACT_FIXTURE_DISABLED_MATERIAL
	STATUS("DISABLED: awaits 999.59 MaterialController cut-over to bool apply() "
	       "+ Scene-backed surface wiring. Flip green by defining "
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
		MaterialController c(h.representation());
		c.setTransparency(50);
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
