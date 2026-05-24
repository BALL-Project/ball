// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.60 (v1.7.x-25) — ClippingController cross-surface parity.
// One of the 9 per-controller parity fixtures (ARCHITECTURE-CONTRACT.md
// §11a). See coloring_contract_test.C for the harness-first sequencing.
//
// NOTE: ClippingController is one of the two remaining Log.info stubs
// (§1) and has no Inspector consumer yet; its apply() is a no-op until
// 999.51/999.59 give it a real body + the §2 guard. The whole fixture is
// therefore disabled-pending; the enabled portion asserts only the staged
// mirror, which is headless-safe.
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

#include "contractTestHarness.h"

#include <BALL/VIEW/KERNEL/controllers/clippingController.h>

#include <QtWidgets/QApplication>

using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Contract;

START_TEST(ClippingContract)

qputenv("QT_QPA_PLATFORM", "offscreen");
int argc = 1;
char argv0[] = "clipping_contract_test";
char* argv[] = { argv0, nullptr };
QApplication app(argc, argv);

// ENABLED today — staged-mirror sanity.
CHECK(ClippingController stages its mirror)
	ClippingController c(nullptr);
	c.setEnabled(true);
	c.setOffset(1.5f);
	c.setCapped(true);
	TEST_EQUAL(c.enabled(), true)
	TEST_REAL_EQUAL(c.offset(), 1.5f)
	TEST_EQUAL(c.capped(), true)
RESULT

// DISABLED until 999.51/999.59 give ClippingController a real apply() body
// + the §2 re-entrancy guard (it currently omits the guard, Codex HIGH #6).
CHECK(DISABLED_clipping_parity_across_surfaces)
#if CONTRACT_FIXTURE_DISABLED
	STATUS("DISABLED: ClippingController is still a Log.info stub (omits the "
	       "§2 guard, Codex HIGH #6). Flip green once 999.51/999.59 give it a "
	       "real bool apply() body + the Scene-backed surfaces.")
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
		ClippingController c(nullptr);
		c.setEnabled(true);
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
