// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.60 (v1.7.x-25) — LabelController cross-surface parity.
// One of the 9 per-controller parity fixtures (ARCHITECTURE-CONTRACT.md
// §11a). See coloring_contract_test.C for the harness-first sequencing.
//
// NOTE: LabelController is the second of the two Log.info stubs (§1).
// The cross-surface parity body is disabled-pending; the enabled portion
// asserts the staged mirror (headless-safe).
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

#include "contractTestHarness.h"

#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/controllers/labelController.h>

#include <QtWidgets/QApplication>

using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Contract;

START_TEST(LabelContract)

qputenv("QT_QPA_PLATFORM", "offscreen");
int argc = 1;
char argv0[] = "label_contract_test";
char* argv[] = { argv0, nullptr };
QApplication app(argc, argv);

// ENABLED today — staged-mirror sanity.
CHECK(LabelController stages its mirror)
	Representation rep;
	LabelController c(&rep);
	c.setLabelText(QStringLiteral("residue"));
	c.setLabelType(2);
	c.setFontSize(14);
	TEST_EQUAL(c.labelText().toStdString(), std::string("residue"))
	TEST_EQUAL(c.labelType(), 2)
	TEST_EQUAL(c.fontSize(), 14)
RESULT

// DISABLED until 999.51/999.59 give LabelController a real bool apply().
CHECK(DISABLED_label_parity_across_surfaces)
#if CONTRACT_FIXTURE_DISABLED
	STATUS("DISABLED: LabelController is still a Log.info stub (§1). Flip green "
	       "once 999.51/999.59 give it a real bool apply() + the Scene-backed "
	       "surfaces.")
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
		LabelController c(h.representation());
		c.setLabelText(QStringLiteral("residue"));
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
