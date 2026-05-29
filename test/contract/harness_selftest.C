// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.60 (v1.7.x-25) — Contract-test harness self-test.
//
// Proves the harness itself boots headlessly, snapshots the owner, and
// reports surface availability honestly. This is the "harness self-test"
// the plan's acceptance criterion calls for:
//
//   "The toolbar/menu helpers invoke the registered command body, not
//    action->trigger() — verified by a harness self-test."
//
// Today (harness-first, before 999.59-03) the toolbar/menu surfaces are
// UNAVAILABLE because the §8 command body has not been migrated and
// CommandRegistry is compiled out. The self-test asserts exactly that
// contract: surfaceAvailable(Toolbar/Menu) == false until 999.59-03, and
// that the Inspector surface is available. When 999.59-03 lands and flips
// the surface to the registered command body, this self-test is updated
// to assert availability + registered-body routing.
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

#include "contractTestHarness.h"
#include <BALL/VIEW/KERNEL/stageMutation.h>

#include <BALL/VIEW/KERNEL/stage.h>          // Stage complete type (setFogIntensity)

#include <QtWidgets/QApplication>

using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Contract;

START_TEST(ContractHarnessSelfTest)

// Headless Qt — offscreen QPA, matching tabOrder_test discipline.
qputenv("QT_QPA_PLATFORM", "offscreen");
int argc = 1;
char argv0[] = "harness_selftest";
char* argv[] = { argv0, nullptr };
QApplication app(argc, argv);

CHECK(harness boots + tears down cleanly)
	ContractTestHarness h;
	h.setUp();
	TEST_NOT_EQUAL(h.stage(), 0)
	TEST_NOT_EQUAL(h.representation(), 0)
	h.tearDown();
	TEST_EQUAL(h.stage(), 0)
	TEST_EQUAL(h.representation(), 0)
RESULT

CHECK(snapshotOwner is deterministic + reflects owner state)
	ContractTestHarness h;
	h.setUp();
	OwnerSnapshot a = h.snapshotOwner();
	OwnerSnapshot b = h.snapshotOwner();
	TEST_EQUAL(a == b, true)               // no spurious diff on repeat
	// Mutating the Stage directly changes the snapshot (sanity: the
	// snapshot is actually reading owner state, not a constant).
	// §3b — Stage mutation flows through the single StageMutation friend
	// (setFogIntensity is now owner-narrowed after 999.59-04).
	StageMutation(*h.stage()).fogIntensity(0.37f);
	OwnerSnapshot c = h.snapshotOwner();
	TEST_EQUAL(a != c, true)
	h.tearDown();
RESULT

CHECK(surface availability is honest for the harness-first window)
	// Inspector is always available (controllers exist in-tree).
	TEST_EQUAL(surfaceAvailable(SurfaceKind::Inspector), true)
	// Toolbar/Menu require the §8 registered command body (999.59-03);
	// they MUST be unavailable today, never silently faked via
	// action->trigger(). This is the contract the plan acceptance
	// criterion locks in.
	TEST_EQUAL(surfaceAvailable(SurfaceKind::Toolbar), false)
	TEST_EQUAL(surfaceAvailable(SurfaceKind::Menu), false)
RESULT

CHECK(surfaceName labels every surface)
	TEST_EQUAL(std::string(surfaceName(SurfaceKind::Inspector)), std::string("Inspector"))
	TEST_EQUAL(std::string(surfaceName(SurfaceKind::Toolbar)), std::string("Toolbar"))
	TEST_EQUAL(std::string(surfaceName(SurfaceKind::Menu)), std::string("Menu"))
	TEST_EQUAL(std::string(surfaceName(SurfaceKind::Python)), std::string("Python"))
RESULT

END_TEST
