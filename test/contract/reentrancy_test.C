// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.60 (v1.7.x-25) — Re-entrancy contract test.
//
// ARCHITECTURE-CONTRACT.md §2 / §11b: force a Controller to apply while a
// notification it is firing is still in flight; assert the re-entrant
// apply() is DROPPED (returns false), not stacked. This is exactly why
// apply() returns bool.
//
// HARNESS-FIRST SEQUENCING: apply() returns `void` today (the bool
// contract lands in 999.59-01). So the "returns false on re-entry"
// assertion ships DISABLED (CONTRACT_FIXTURE_DISABLED). The ENABLED
// portion asserts the precursor invariant that already holds: the
// ControllerApplyGuard is held during the appliedStub() emission, so a
// re-entrant apply() is a no-op (the owner keeps the outer apply()'s
// value, not the value staged inside the re-entrant slot). This is the
// sanity check the plan's acceptance criterion calls for — it must pass
// today and the bool assertion flips green after 999.59-01.
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

START_TEST(Reentrancy)

qputenv("QT_QPA_PLATFORM", "offscreen");
int argc = 1;
char argv0[] = "reentrancy_test";
char* argv[] = { argv0, nullptr };
QApplication app(argc, argv);

// ENABLED today — the re-entrancy GUARD shields a nested apply().
// A slot connected to appliedStub() (fired while the guard is still held)
// attempts a re-entrant apply() with a DIFFERENT value; the guard must make
// it a no-op, so the Stage keeps the outer apply()'s value.
CHECK(re-entrant apply during own emission is dropped by the guard)
	Stage stage;
	StageController c(&stage, nullptr);
	c.setFogIntensity(0.42f);

	bool guard_held_mid_apply = false;
	bool attempted_reentry = false;
	QObject::connect(&c, &StageController::appliedStub, &c, [&]() {
		if (attempted_reentry) return;       // one-shot
		attempted_reentry = true;
		guard_held_mid_apply = c.isApplying();
		c.setFogIntensity(0.99f);            // change the mirror only
		c.apply();                           // re-entrant => must be shielded
	});

	c.apply();

	TEST_EQUAL(guard_held_mid_apply, true)             // guard active during emit
	TEST_EQUAL(c.isApplying(), false)                  // RAII cleared on return
	TEST_REAL_EQUAL(stage.getFogIntensity(), 0.42f)    // nested apply pushed nothing

	// Not wedged: a later non-reentrant apply() pushes the pending value.
	c.apply();
	TEST_REAL_EQUAL(stage.getFogIntensity(), 0.99f)
RESULT

// DISABLED until 999.59-03 cuts StageController over to bool apply() — the
// re-entry must report false (dropped), the outer apply() must report true
// (mutated). This block exercises StageController, which is NOT cut over in
// 999.59-02 (that plan harmonises Model/Coloring/Material only), so it stays
// on the global CONTRACT_FIXTURE_DISABLED (pinned to 1) rather than a
// per-domain switch. 999.59-03's Stage cut-over flips it green.
CHECK(DISABLED_second_apply_returns_false_on_reentry)
#if CONTRACT_FIXTURE_DISABLED
	STATUS("DISABLED: StageController::apply() returns void until 999.59-03 cuts "
	       "it over (999.59-02 harmonises Model/Coloring/Material only). Flip green "
	       "when StageController returns bool and assert: outer apply()==true, "
	       "re-entrant apply()==false.")
	break;
#else
	Stage stage;
	StageController c(&stage, nullptr);
	c.setFogIntensity(0.42f);
	bool drop_observed = false;
	QObject::connect(&c, &StageController::appliedStub, &c, [&]() {
		// Re-enter while emitting the first applied event.
		drop_observed = !c.apply();          // expect false (dropped) => true
	});
	bool first = c.apply();                  // first apply mutated => true
	TEST_EQUAL(first, true)
	TEST_EQUAL(drop_observed, true)
#endif
RESULT

END_TEST
