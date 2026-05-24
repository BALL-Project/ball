// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.60 (v1.7.x-25) — CameraController cross-surface parity.
// One of the 9 per-controller parity fixtures (ARCHITECTURE-CONTRACT.md
// §11a). See coloring_contract_test.C for the harness-first sequencing.
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

#include "contractTestHarness.h"

#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/controllers/cameraController.h>

#include <QtWidgets/QApplication>
#include <QtGui/QVector3D>

using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Contract;

START_TEST(CameraContract)

qputenv("QT_QPA_PLATFORM", "offscreen");
int argc = 1;
char argv0[] = "camera_contract_test";
char* argv[] = { argv0, nullptr };
QApplication app(argc, argv);

// ENABLED today — Inspector surface postcondition (headless-testable).
CHECK(CameraController Inspector surface reaches the Stage camera)
	Stage stage;
	CameraController c(&stage);
	c.setPosition(QVector3D(1.0f, 2.0f, 3.0f));
	c.setLookAt(QVector3D(4.0f, 5.0f, 6.0f));
	c.apply();
	CameraController back(&stage);
	TEST_REAL_EQUAL(back.position().x(), 1.0f)
	TEST_REAL_EQUAL(back.position().y(), 2.0f)
	TEST_REAL_EQUAL(back.position().z(), 3.0f)
	TEST_REAL_EQUAL(back.lookAt().x(), 4.0f)
RESULT

// DISABLED until 999.59 cuts CameraController over — cross-surface parity.
CHECK(DISABLED_camera_parity_across_surfaces)
#if CONTRACT_FIXTURE_DISABLED
	STATUS("DISABLED: awaits 999.59 CameraController cut-over to bool apply(). "
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
		CameraController c(h.stage());
		c.setPosition(QVector3D(1.0f, 2.0f, 3.0f));
		c.setLookAt(QVector3D(4.0f, 5.0f, 6.0f));
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
