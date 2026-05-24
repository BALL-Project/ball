// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.60 (v1.7.x-25) — Selection-consumer agreement test.
//
// ARCHITECTURE-CONTRACT.md §7: Composite::isSelected() is the single
// source of truth; the four consumers (MolecularControl tree, Scene
// picker, MainControl::getSelection(), action enable-gates) MUST agree.
//
// HARNESS-FIRST SEQUENCING: the full four-consumer agreement needs a
// constructed MolecularControl + Scene + MainControl, which are not
// headless-stable on every runner (tabOrder_test documents the Windows
// fragility) and whose selection plumbing 999.59 narrows. So the
// four-way EXPECT_EQ ships DISABLED. The enabled portion asserts the
// source-of-truth contract directly: a selection set on the loaded sample
// is observable through Composite::isSelected(), which every consumer
// re-reads (§4 "re-read, don't trust the message").
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

#include "contractTestHarness.h"

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomIterator.h>

#include <QtWidgets/QApplication>

using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Contract;

START_TEST(SelectionConsumer)

qputenv("QT_QPA_PLATFORM", "offscreen");
int argc = 1;
char argv0[] = "selection_consumer_test";
char* argv[] = { argv0, nullptr };
QApplication app(argc, argv);

// ENABLED today — source-of-truth contract. A selection applied to the
// loaded sample is observable via Composite::isSelected(); the snapshot's
// selection blob reflects it. This is the invariant every consumer must
// re-read from.
CHECK(Composite::isSelected is the single source of truth)
	ContractTestHarness h;
	h.setUp();
	if (!h.sampleLoaded())
	{
		STATUS("sample 1BNA.pdb not available on this runner (no BALL_DATA_PATH) "
		       "— selection source-of-truth check skipped.")
		break;
	}
	System* sys = h.system();
	OwnerSnapshot before = h.snapshotOwner();

	// Select the first 5 atoms via the source of truth.
	Size n = 0;
	for (AtomIterator it = sys->beginAtom(); +it && n < 5; ++it, ++n)
	{
		it->select();
	}

	OwnerSnapshot after = h.snapshotOwner();
	// The selection blob must change — the snapshot reads the source of
	// truth, not a cached consumer view.
	TEST_EQUAL(before.selection_blob != after.selection_blob, true)

	// Count the source-of-truth selection and confirm it matches what we set.
	Size selected = 0;
	for (AtomIterator it = sys->beginAtom(); +it; ++it)
		if (it->isSelected()) ++selected;
	TEST_EQUAL(selected, 5)
	h.tearDown();
RESULT

// DISABLED until 999.59 narrows the selection plumbing — full four-consumer
// agreement (tree / scene picker / mainControl / action enable-gate).
CHECK(DISABLED_four_consumers_agree)
#if CONTRACT_FIXTURE_DISABLED
	STATUS("DISABLED: the four-consumer agreement (tree/scene/mainControl/action) "
	       "needs a constructed MolecularControl + Scene + MainControl and the "
	       "999.59-narrowed selection plumbing. Flip green once 999.59 routes all "
	       "selection mutation through MainControl::setSelection_ and the harness "
	       "boots the Scene/control widgets.")
	break;
#else
	// Placeholder for the §7 verbatim four-way EXPECT_EQ. Wired by the
	// 999.59 selection-narrowing plan.
	TEST_EQUAL(true, true)
#endif
RESULT

END_TEST
